#ifndef _TAURUS_ARMPC_PARTITION_H_
#define _TAURUS_ARMPC_PARTITION_H_

#include "hisi_partition.h"
#include "partition_def.h"

static const struct partition partition_table_emmc[] =
{
  {"0", 0, 0, 0},                                        /* total 11848M*/
};

static struct partition partition_table_ufs[] =
{
  {PART_XLOADER,                   0,                 2*1024,    UFS_PART_0},
  {PART_VARIABLE,                  0,                20*1024,    UFS_PART_1},/* variable          20M    p0*/
  {PART_PTABLE,                    0,                    512,    UFS_PART_2},/* ptable           512K    p0*/
  {PART_VRL,                       512,                  512,    UFS_PART_2},/* vrl              512K    p1*/
  {PART_VRL_BACKUP,                1024,                 512,    UFS_PART_2},/* vrl backup       512K    p2*/
  {PART_RESERVED1,                 1024+512,             512,    UFS_PART_2},/* reserved1       1024K    p3*/
  {PART_BL2,                       2*1024,            4*1024,    UFS_PART_2},/* bl2                4M    p2*/
  {PART_FW_LPM3,                   6*1024,            1*1024,    UFS_PART_2},/* fw_lpm3            1M    p3*/
  {PART_TRUSTFIRMWARE,             7*1024,            2*1024,    UFS_PART_2},/* trustfirmware      2M    p4*/
  {PART_TEEOS,                     9*1024,           10*1024,    UFS_PART_2},/* teeos             10M    p5*/
  {PART_DTS,                       19*1024,          20*1024,    UFS_PART_2},/* dtsimage          20M    p6*/
  {PART_FASTBOOT,                  39*1024,          12*1024,    UFS_PART_2},/* fastboot          12M    p7*/
  {PART_DDR_PARA,                  51*1024,           1*1024,    UFS_PART_2},/* DDR_PARA           1M    p8*/
  {PART_CTF,                       52*1024,           1*1024,    UFS_PART_2},/* PART_CTF           1M    p9*/
  {PART_NVME,                      53*1024,           5*1024,    UFS_PART_2},/* nvme               5M    p11*/
  {PART_OEMINFO,                   58*1024,          96*1024,    UFS_PART_2},/* oeminfo           96M    p12*/
  {PART_SENSORHUB,                 154*1024,         16*1024,    UFS_PART_2},/* sensorhub         16M    p13*/
  {PART_HHEE,                      170*1024,          6*1024,    UFS_PART_2},/* hhee               6M    p14*/
  {PART_FW_DTB,                    176*1024,          2*1024,    UFS_PART_2},/* fw_dtb             2M    p15*/
  {PART_RECOVERY,                  178*1024,          800*1024,  UFS_PART_2},/* recovery         800M    p16*/
  {PART_LOWPOWER_PARA,             978*1024,          1*1024,    UFS_PART_2},/* lowpower_para      1M    p17*/
  {PART_VECTOR,                    979*1024,          4*1024,    UFS_PART_2},/* vector             4M    p18*/
  {PART_PTABLE_LU3,                0,                    512,    UFS_PART_3},/* ptable_lu3       512K    p0*/
  {PART_RESERVED2,                 512,                  512,    UFS_PART_3},/* reserved2        512K    p1*/
  {PART_BOOT,                      1*1024,           65*1024,    UFS_PART_3},/* boot              65M    p2*/
  {PART_ROOT,                      66*1024, (32UL)*1024*1024,    UFS_PART_3},/* root              32G    p3*/
  {PART_EFI,                       32834*1024,(1UL)*1024*1024,   UFS_PART_3},/* efi                1G    p4*/
  {PART_DFX,                       33858*1024,        16*1024,   UFS_PART_3},/* dfx               16M    p5*/
#ifdef CONFIG_FACTORY_MODE
  {PART_HIBENCH_IMG,             33874*1024,        128*1024,    UFS_PART_3},/* hibench_img      128M    p6*/
  {PART_HIBENCH_DATA,            34002*1024,        512*1024,    UFS_PART_3},/* hibench_data     512M    p7*/
  {PART_HIBENCH_LOG,             34514*1024,         32*1024,    UFS_PART_3},/* HIBENCH_LOG       32M    p8*/
  {PART_HIBENCH_LPM3,            34546*1024,         32*1024,    UFS_PART_3},/* HIBENCH_LPM3      32M    p9*/
  {PART_KERNEL,                  34578*1024,  (8UL)*1024*1024,   UFS_PART_3},/* kerneldump         8G    p10*/
  {PART_HISEE_FS,                42770*1024,           8*1024,   UFS_PART_3},/* hisee_fs           8M    p11*/
  {PART_HOME,                    42778*1024, (64UL)*1024*1024,   UFS_PART_3},/* home              64G    p12*/
#else
#ifdef CONFIG_PRODUCT_ARMPC_USER
  {PART_HISEE_FS,                33874*1024,           8*1024,   UFS_PART_3},/* hisee_fs           8M    p6*/
  {PART_HOME,                    33882*1024,  (64UL)*1024*1024,  UFS_PART_3},/* home              64G    p7*/
#else
  {PART_KERNEL,                  33874*1024,  (8UL)*1024*1024,   UFS_PART_3},/* kerneldump         8G    p6*/
  {PART_HISEE_FS,                42066*1024,           8*1024,   UFS_PART_3},/* hisee_fs           8M    p7*/
  {PART_HOME,                    42074*1024,  (64UL)*1024*1024,  UFS_PART_3},/* home              64G    p8*/
#endif
#endif
  {"0", 0, 0, 0},
};

#endif
