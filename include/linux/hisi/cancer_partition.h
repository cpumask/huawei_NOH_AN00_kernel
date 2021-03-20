#ifndef _CANCER_PARTITION_H_
#define _CANCER_PARTITION_H_

#include "hisi_partition.h"
#include "partition_def.h"

static const struct partition partition_table_emmc[] =
{
  {PART_XLOADER,                           0,           2*1024,        EMMC_BOOT_MAJOR_PART},
  {PART_RESERVED0,                         0,           2*1024,        EMMC_BOOT_BACKUP_PART},
  {PART_PTABLE,                            0,              512,        EMMC_USER_PART},/* ptable          512K   */
  {PART_FRP,                             512,              512,        EMMC_USER_PART},/* frp             512K   p1*/
  {PART_PERSIST,                        1024,           6*1024,        EMMC_USER_PART},/* persist           6M   p2*/
  {PART_RESERVED1,                    7*1024,             1024,        EMMC_USER_PART},/* reserved1      1024K   p3*/
  {PART_RESERVED6,                    8*1024,              512,        EMMC_USER_PART},/* reserved6       512K   p4*/
  {PART_VRL,                            8704,              512,        EMMC_USER_PART},/* vrl             512K   p5*/
  {PART_VRL_BACKUP,                   9*1024,              512,        EMMC_USER_PART},/* vrl backup      512K   p6*/
  {PART_MODEM_SECURE,                   9728,             8704,        EMMC_USER_PART},/* modem_secure   8704K   p7*/
  {PART_NVME,                        18*1024,           5*1024,        EMMC_USER_PART},/* nvme              5M   p8*/
  {PART_CTF,                         23*1024,           1*1024,        EMMC_USER_PART},/* ctf               1M   p9*/
  {PART_OEMINFO,                     24*1024,          96*1024,        EMMC_USER_PART},/* oeminfo          96M   p10*/
  {PART_SECURE_STORAGE,             120*1024,          32*1024,        EMMC_USER_PART},/* secure storage   32M   p11*/
  {PART_MODEMNVM_FACTORY,           152*1024,          16*1024,        EMMC_USER_PART},/* modemnvm factory 16M   p12*/
  {PART_MODEMNVM_BACKUP,            168*1024,          16*1024,        EMMC_USER_PART},/* modemnvm backup  16M   p13*/
  {PART_MODEMNVM_IMG,               184*1024,          34*1024,        EMMC_USER_PART},/* modemnvm img     34M   p14*/
  {PART_HISEE_ENCOS,                218*1024,           4*1024,        EMMC_USER_PART},/* hisee_encos       4M   p15*/
  {PART_VERITYKEY,                  222*1024,           1*1024,        EMMC_USER_PART},/* veritykey         1M   p16*/
  {PART_DDR_PARA,                   223*1024,           1*1024,        EMMC_USER_PART},/* DDR_PARA          1M   p17*/
  {PART_LOWPOWER_PARA,              224*1024,           1*1024,        EMMC_USER_PART},/* lowpower_para     1M   p18*/
  {PART_BATT_TP_PARA,               225*1024,           1*1024,        EMMC_USER_PART},/* batt_tp_para      1M   p19*/
  {PART_RESERVED2,                  226*1024,          25*1024,        EMMC_USER_PART},/* reserved2        25M   p20*/
  {PART_SPLASH2,                    251*1024,          80*1024,        EMMC_USER_PART},/* splash2          80M   p21*/
  {PART_BOOTFAIL_INFO,              331*1024,           2*1024,        EMMC_USER_PART},/* bootfail info     2M   p22*/
  {PART_MISC,                       333*1024,           2*1024,        EMMC_USER_PART},/* misc              2M   p23*/
  {PART_DFX,                        335*1024,          16*1024,        EMMC_USER_PART},/* dfx              16M   p24*/
  {PART_RRECORD,                    351*1024,          16*1024,        EMMC_USER_PART},/* rrecord          16M   p25*/
  {PART_CACHE,                      367*1024,         104*1024,        EMMC_USER_PART},/* cache           104M   p26*/
  {PART_FW_LPM3,                    471*1024,             1024,        EMMC_USER_PART},/* fw_lpm3         256K   p27*/
  {PART_RESERVED3,                  472*1024,           6*1024,        EMMC_USER_PART},/* reserved3A        6M   p28*/
  {PART_IVP,                        478*1024,           3*1024,        EMMC_USER_PART},/* ivp               3M   p29*/
  {PART_HDCP,                       481*1024,           1*1024,        EMMC_USER_PART},/* PART_HDCP         1M   p30*/
  {PART_HISEE_IMG,                  482*1024,           4*1024,        EMMC_USER_PART},/* part_hisee_img    4M   p31*/
  {PART_HHEE,                       486*1024,           4*1024,        EMMC_USER_PART},/* hhee              4M   p32*/
  {PART_HISEE_FS,                   490*1024,           8*1024,        EMMC_USER_PART},/* hisee_fs          8M   p33*/
  {PART_FASTBOOT,                   498*1024,          12*1024,        EMMC_USER_PART},/* fastboot         12M   p34*/
  {PART_VECTOR,                     510*1024,           4*1024,        EMMC_USER_PART},/* vector            4M   p35*/
  {PART_ISP_BOOT,                   514*1024,           2*1024,        EMMC_USER_PART},/* isp_boot          2M   p36*/
  {PART_ISP_FIRMWARE,               516*1024,          14*1024,        EMMC_USER_PART},/* isp_firmware     14M   p37*/
  {PART_FW_HIFI,                    530*1024,          12*1024,        EMMC_USER_PART},/* hifi             12M   p38*/
  {PART_TEEOS,                      542*1024,           8*1024,        EMMC_USER_PART},/* teeos             8M   p39*/
  {PART_SENSORHUB,                  550*1024,          16*1024,        EMMC_USER_PART},/* sensorhub        16M   p40*/
#ifdef CONFIG_SANITIZER_ENABLE
  {PART_ERECOVERY_KERNEL,           566*1024,          12*1024,        EMMC_USER_PART},/* erecovery_kernel  12M    p41*/
  {PART_ERECOVERY_RAMDISK,          578*1024,          12*1024,        EMMC_USER_PART},/* erecovery_ramdisk 12M    p42*/
  {PART_ERECOVERY_VENDOR,           590*1024,           8*1024,        EMMC_USER_PART},/* erecovery_vendor   8M    p43*/
  {PART_KERNEL,                     598*1024,          64*1024,        EMMC_USER_PART},/* kernel            64M    p44*/
#else
  {PART_ERECOVERY_KERNEL,           566*1024,          24*1024,        EMMC_USER_PART},/* erecovery_kernel  24M    p41*/
  {PART_ERECOVERY_RAMDISK,          590*1024,          32*1024,        EMMC_USER_PART},/* erecovery_ramdisk 32M    p42*/
  {PART_ERECOVERY_VENDOR,           622*1024,          16*1024,        EMMC_USER_PART},/* erecovery_vendor  16M    p43*/
  {PART_KERNEL,                     638*1024,          24*1024,        EMMC_USER_PART},/* kernel            24M    p44*/
#endif
  {PART_ENG_SYSTEM,                 662*1024,          12*1024,        EMMC_USER_PART},/* eng_system        12M    p45*/
  {PART_RECOVERY_RAMDISK,           674*1024,          32*1024,        EMMC_USER_PART},/* recovery_ramdisk  32M    p46*/
  {PART_RECOVERY_VENDOR,            706*1024,          16*1024,        EMMC_USER_PART},/* recovery_vendor   16M    p47*/
  {PART_DTS,                        722*1024,           1*1024,        EMMC_USER_PART},/* dtimage            1M    p48*/
  {PART_DTO,                        723*1024,          20*1024,        EMMC_USER_PART},/* dtoimage          20M    p49*/
  {PART_TRUSTFIRMWARE,              743*1024,           2*1024,        EMMC_USER_PART},/* trustfirmware      2M    p50*/
  {PART_MODEM_FW,                   745*1024,          56*1024,        EMMC_USER_PART},/* modem_fw          56M    p51*/
  {PART_ENG_VENDOR,                 801*1024,          12*1024,        EMMC_USER_PART},/* eng_vendor        12M    p52*/
  {PART_MODEM_PATCH_NV,             813*1024,           4*1024,        EMMC_USER_PART},/* modem_patch_nv     4M    p53*/
  {PART_MODEM_DRIVER,               817*1024,          20*1024,        EMMC_USER_PART},/* modem_driver      20M    p54*/
  {PART_KPATCH,                     837*1024,          12*1024,        EMMC_USER_PART},/* kpatch            12M    p55*/
  {PART_RAMDISK,                    849*1024,           2*1024,        EMMC_USER_PART},/* ramdisk            2M    p56*/
  {PART_VBMETA_SYSTEM,              851*1024,           1*1024,        EMMC_USER_PART},/* vbmeta_system      1M    p57*/
  {PART_VBMETA_VENDOR,              852*1024,           1*1024,        EMMC_USER_PART},/* vbmeta_vendor      1M    p58*/
  {PART_VBMETA_ODM,                 853*1024,           1*1024,        EMMC_USER_PART},/* vbmeta_odm         1M    p59*/
  {PART_VBMETA_CUST,                854*1024,           1*1024,        EMMC_USER_PART},/* vbmeta_cust        1M    p60*/
  {PART_VBMETA_HW_PRODUCT,          855*1024,           1*1024,        EMMC_USER_PART},/* vbmeta_hw_product  1M    p61*/
  {PART_RECOVERY_VBMETA,            856*1024,           2*1024,        EMMC_USER_PART},/* recovery_vbmeta    2M    p62*/
  {PART_ERECOVERY_VBMETA,           858*1024,           2*1024,        EMMC_USER_PART},/* erecovery_vbmeta   2M    p63*/
  {PART_VBMETA,                     860*1024,           4*1024,        EMMC_USER_PART},/* PART_VBMETA        4M    p64*/
  {PART_MODEMNVM_UPDATE,            864*1024,          16*1024,        EMMC_USER_PART},/* modemnvm_update   16M    p65*/
  {PART_MODEMNVM_CUST,              880*1024,          16*1024,        EMMC_USER_PART},/* modemnvm_cust     16M    p66*/
  {PART_PATCH,                      896*1024,          32*1024,        EMMC_USER_PART},/* patch             32M    p67*/
#ifdef CONFIG_FACTORY_MODE
  {PART_PREAS,                      928*1024,        1216*1024,        EMMC_USER_PART},/* preas          1216M     p68*/
  {PART_PREAVS,                    2144*1024,          32*1024,        EMMC_USER_PART},/* preavs           32M     p69*/
  {PART_PRETS,                     2176*1024,          24*1024,        EMMC_USER_PART},/* prets             24M    p70*/
  {PART_PRETVS,                    2200*1024,           8*1024,        EMMC_USER_PART},/* pretvs             8M    p71*/
  {PART_SUPER,                     2208*1024,        7544*1024,        EMMC_USER_PART},/* super           7544M    p72*/
  {PART_VERSION,                   9752*1024,         576*1024,        EMMC_USER_PART},/* version          576M    p73*/
  {PART_PRELOAD,                  10328*1024,        1144*1024,        EMMC_USER_PART},/* preload_a       1144M    p74*/
  {PART_HIBENCH_IMG,              11472*1024,         128*1024,        EMMC_USER_PART},/* hibench_img      128M    p75*/
  {PART_HIBENCH_DATA,             11600*1024,         512*1024,        EMMC_USER_PART},/* hibench_data     512M    p76*/
  {PART_FLASH_AGEING,             12112*1024,         512*1024,        EMMC_USER_PART},/* FLASH_AGEING     512M    p77*/
  {PART_HIBENCH_LOG,              12624*1024,          32*1024,        EMMC_USER_PART},/* HIBENCH_LOG       32M    p78*/
  {PART_USERDATA,                 12656*1024,  (4UL)*1024*1024,        EMMC_USER_PART},/* userdata           4G    p79*/
#else
#ifdef CONFIG_USE_EROFS
#ifdef CONFIG_UPDATE_PRODUCT_EROFS
#ifdef CONFIG_MARKET_INTERNAL
  {PART_PREAS,                      928*1024,         360*1024,        EMMC_USER_PART},/* preas           360M     p68*/
  {PART_PREAVS,                    1288*1024,          32*1024,        EMMC_USER_PART},/* preavs           32M     p69*/
  {PART_PRETS,                     1320*1024,          24*1024,        EMMC_USER_PART},/* prets             24M    p70*/
  {PART_PRETVS,                    1344*1024,           8*1024,        EMMC_USER_PART},/* pretvs             8M    p71*/
  {PART_SUPER,                     1352*1024,        7448*1024,        EMMC_USER_PART},/* super           7448M    p72*/
  {PART_VERSION,                   8800*1024,         576*1024,        EMMC_USER_PART},/* version          576M    p73*/
  {PART_PRELOAD,                   9376*1024,        1144*1024,        EMMC_USER_PART},/* preload         1144M    p74*/
  {PART_USERDATA,                 10520*1024,  (4UL)*1024*1024,        EMMC_USER_PART},/* userdata           4G    p75*/
#else
  {PART_PREAS,                      928*1024,        1216*1024,        EMMC_USER_PART},/* preas          1216M     p68*/
  {PART_PREAVS,                    2144*1024,          32*1024,        EMMC_USER_PART},/* preavs           32M     p69*/
  {PART_PRETS,                     2176*1024,          24*1024,        EMMC_USER_PART},/* prets             24M    p70*/
  {PART_PRETVS,                    2200*1024,           8*1024,        EMMC_USER_PART},/* pretvs             8M    p71*/
  {PART_SUPER,                     2208*1024,        7512*1024,        EMMC_USER_PART},/* super           7512M    p72*/
  {PART_VERSION,                   9720*1024,         576*1024,        EMMC_USER_PART},/* version          576M    p73*/
  {PART_PRELOAD,                  10296*1024,        1144*1024,        EMMC_USER_PART},/* preload         1144M    p74*/
  {PART_USERDATA,                 11440*1024,  (4UL)*1024*1024,        EMMC_USER_PART},/* userdata           4G    p75*/
#endif
#else
#ifdef CONFIG_MARKET_INTERNAL
  {PART_PREAS,                      928*1024,         296*1024,        EMMC_USER_PART},/* preas           296M     p68*/
  {PART_PREAVS,                    1224*1024,          32*1024,        EMMC_USER_PART},/* preavs           32M     p69*/
  {PART_PRETS,                     1256*1024,          24*1024,        EMMC_USER_PART},/* prets             24M    p70*/
  {PART_PRETVS,                    1280*1024,           8*1024,        EMMC_USER_PART},/* pretvs             8M    p71*/
  {PART_SUPER,                     1288*1024,        5600*1024,        EMMC_USER_PART},/* super           5600M    p72*/
  {PART_VERSION,                   6888*1024,         576*1024,        EMMC_USER_PART},/* version          576M    p73*/
  {PART_PRELOAD,                   7464*1024,        1144*1024,        EMMC_USER_PART},/* preload         1144M    p74*/
  {PART_USERDATA,                  8608*1024,  (4UL)*1024*1024,        EMMC_USER_PART},/* userdata           4G    p75*/
#else
  {PART_PREAS,                      928*1024,         976*1024,        EMMC_USER_PART},/* preas           976M     p68*/
  {PART_PREAVS,                    1904*1024,          32*1024,        EMMC_USER_PART},/* preavs           32M     p69*/
  {PART_PRETS,                     1936*1024,          24*1024,        EMMC_USER_PART},/* prets             24M    p70*/
  {PART_PRETVS,                    1960*1024,           8*1024,        EMMC_USER_PART},/* pretvs             8M    p71*/
  {PART_SUPER,                     1968*1024,        5520*1024,        EMMC_USER_PART},/* super           5520M    p72*/
  {PART_VERSION,                   7488*1024,         576*1024,        EMMC_USER_PART},/* version          576M    p73*/
  {PART_PRELOAD,                   8064*1024,        1144*1024,        EMMC_USER_PART},/* preload         1144M    p74*/
  {PART_USERDATA,                  9208*1024,  (4UL)*1024*1024,        EMMC_USER_PART},/* userdata           4G    p75*/
#endif
#endif
#else
#ifdef CONFIG_MARKET_INTERNAL
  {PART_PREAS,                      928*1024,         360*1024,        EMMC_USER_PART},/* preas           360M     p68*/
  {PART_PREAVS,                    1288*1024,          32*1024,        EMMC_USER_PART},/* preavs           32M     p69*/
  {PART_PRETS,                     1320*1024,          24*1024,        EMMC_USER_PART},/* prets             24M    p70*/
  {PART_PRETVS,                    1344*1024,           8*1024,        EMMC_USER_PART},/* pretvs             8M    p71*/
  {PART_SUPER,                     1352*1024,        7448*1024,        EMMC_USER_PART},/* super           7448M    p72*/
  {PART_VERSION,                   8800*1024,         576*1024,        EMMC_USER_PART},/* version          576M    p73*/
  {PART_PRELOAD,                   9376*1024,        1144*1024,        EMMC_USER_PART},/* preload         1144M    p74*/
  {PART_USERDATA,                 10520*1024,  (4UL)*1024*1024,        EMMC_USER_PART},/* userdata           4G    p75*/
#else
  {PART_PREAS,                      928*1024,        1216*1024,        EMMC_USER_PART},/* preas          1216M     p68*/
  {PART_PREAVS,                    2144*1024,          32*1024,        EMMC_USER_PART},/* preavs           32M     p69*/
  {PART_PRETS,                     2176*1024,          24*1024,        EMMC_USER_PART},/* prets             24M    p70*/
  {PART_PRETVS,                    2200*1024,           8*1024,        EMMC_USER_PART},/* pretvs             8M    p71*/
  {PART_SUPER,                     2208*1024,        7512*1024,        EMMC_USER_PART},/* super           7512M    p72*/
  {PART_VERSION,                   9720*1024,         576*1024,        EMMC_USER_PART},/* version          576M    p73*/
  {PART_PRELOAD,                  10296*1024,        1144*1024,        EMMC_USER_PART},/* preload         1144M    p74*/
  {PART_USERDATA,                 11440*1024,  (4UL)*1024*1024,        EMMC_USER_PART},/* userdata           4G    p75*/
#endif
#endif
#endif
  {"0", 0, 0, 0},                                        /* total 11848M*/
};

static const struct partition partition_table_ufs[] =
{
  {PART_XLOADER,                           0,        2*1024,        UFS_PART_0},
  {PART_RESERVED0,                         0,        2*1024,        UFS_PART_1},
  {PART_PTABLE,                            0,           512,        UFS_PART_2},/* ptable                    512K    */
  {PART_FRP,                             512,           512,        UFS_PART_2},/* frp                       512K    p1*/
  {PART_PERSIST,                      1*1024,        6*1024,        UFS_PART_2},/* persist                     6M    p2*/
  {PART_RESERVED1,                    7*1024,          1024,        UFS_PART_2},/* reserved1                1024K    p3*/
  {PART_PTABLE_LU3,                        0,           512,        UFS_PART_3},/* ptable_lu3                512K    p0*/
  {PART_VRL,                             512,           512,        UFS_PART_3},/* vrl                       512K    p1*/
  {PART_VRL_BACKUP,                     1024,           512,        UFS_PART_3},/* vrl backup                512K    p2*/
  {PART_MODEM_SECURE,                   1536,          8704,        UFS_PART_3},/* modem_secure             8704K    p3*/
  {PART_NVME,                        10*1024,        5*1024,        UFS_PART_3},/* nvme                        5M    p4*/
  {PART_CTF,                         15*1024,        1*1024,        UFS_PART_3},/* PART_CTF                    1M    p5*/
  {PART_OEMINFO,                     16*1024,       96*1024,        UFS_PART_3},/* oeminfo                    96M    p6*/
  {PART_SECURE_STORAGE,             112*1024,       32*1024,        UFS_PART_3},/* secure storage             32M    p7*/
  {PART_MODEMNVM_FACTORY,           144*1024,       16*1024,        UFS_PART_3},/* modemnvm factory           16M    p8*/
  {PART_MODEMNVM_BACKUP,            160*1024,       16*1024,        UFS_PART_3},/* modemnvm backup            16M    p9*/
  {PART_MODEMNVM_IMG,               176*1024,       34*1024,        UFS_PART_3},/* modemnvm img               34M    p10*/
  {PART_HISEE_ENCOS,                210*1024,        4*1024,        UFS_PART_3},/* hisee_encos                 4M    p11*/
  {PART_VERITYKEY,                  214*1024,        1*1024,        UFS_PART_3},/* veritykey                   1M    p12*/
  {PART_DDR_PARA,                   215*1024,        1*1024,        UFS_PART_3},/* ddr_para                    1M    p13*/
  {PART_LOWPOWER_PARA,              216*1024,        1*1024,        UFS_PART_3},/* lowpower_para               1M    p14*/
  {PART_BATT_TP_PARA,               217*1024,        1*1024,        UFS_PART_3},/* batt_tp_para                1M    p15*/
  {PART_RESERVED2,                  218*1024,       25*1024,        UFS_PART_3},/* reserved2                  25M    p16*/
  {PART_SPLASH2,                    243*1024,       80*1024,        UFS_PART_3},/* splash2                    80M    p17*/
  {PART_BOOTFAIL_INFO,              323*1024,        2*1024,        UFS_PART_3},/* bootfail info              2MB    p18*/
  {PART_MISC,                       325*1024,        2*1024,        UFS_PART_3},/* misc                        2M    p19*/
  {PART_DFX,                        327*1024,       16*1024,        UFS_PART_3},/* dfx                        16M    p20*/
  {PART_RRECORD,                    343*1024,       16*1024,        UFS_PART_3},/* rrecord                    16M    p21*/
  {PART_CACHE,                      359*1024,      104*1024,        UFS_PART_3},/* cache                     104M    p22*/
  {PART_FW_LPM3,                    463*1024,          1024,        UFS_PART_3},/* fw_lpm3                   256K    p23*/
  {PART_RESERVED3,                  464*1024,        6*1024,        UFS_PART_3},/* reserved3A                  6M    p24*/
  {PART_IVP,                        470*1024,        3*1024,        UFS_PART_3},/* ivp                         3M    p25*/
  {PART_HDCP,                       473*1024,        1*1024,        UFS_PART_3},/* PART_HDCP                   1M    p26*/
  {PART_HISEE_IMG,                  474*1024,        4*1024,        UFS_PART_3},/* part_hisee_img              4M    p27*/
  {PART_HHEE,                       478*1024,        4*1024,        UFS_PART_3},/* hhee                        4M    p28*/
  {PART_HISEE_FS,                   482*1024,        8*1024,        UFS_PART_3},/* hisee_fs                    8M    p29*/
  {PART_FASTBOOT,                   490*1024,       12*1024,        UFS_PART_3},/* fastboot                   12M    p30*/
  {PART_VECTOR,                     502*1024,        4*1024,        UFS_PART_3},/* avs vector                  4M    p31*/
  {PART_ISP_BOOT,                   506*1024,        2*1024,        UFS_PART_3},/* isp_boot                    2M    p32*/
  {PART_ISP_FIRMWARE,               508*1024,       14*1024,        UFS_PART_3},/* isp_firmware               14M    p33*/
  {PART_FW_HIFI,                    522*1024,       12*1024,        UFS_PART_3},/* hifi                       12M    p34*/
  {PART_TEEOS,                      534*1024,        8*1024,        UFS_PART_3},/* teeos                       8M    p35*/
  {PART_SENSORHUB,                  542*1024,       16*1024,        UFS_PART_3},/* sensorhub                  16M    p36*/
#ifdef CONFIG_SANITIZER_ENABLE
  {PART_ERECOVERY_KERNEL,               558*1024,       12*1024,        UFS_PART_3},/* erecovery_kernel           12M    p37*/
  {PART_ERECOVERY_RAMDISK,              570*1024,       12*1024,        UFS_PART_3},/* erecovery_ramdisk          12M    p38*/
  {PART_ERECOVERY_VENDOR,               582*1024,        8*1024,        UFS_PART_3},/* erecovery_vendor            8M    p39*/
  {PART_KERNEL,                         590*1024,       64*1024,        UFS_PART_3},/* kernel                     64M    p40*/
#else
  {PART_ERECOVERY_KERNEL,               558*1024,       24*1024,        UFS_PART_3},/* erecovery_kernel           24M    p37*/
  {PART_ERECOVERY_RAMDISK,              582*1024,       32*1024,        UFS_PART_3},/* erecovery_ramdisk          32M    p38*/
  {PART_ERECOVERY_VENDOR,               614*1024,       16*1024,        UFS_PART_3},/* erecovery_vendor           16M    p39*/
  {PART_KERNEL,                         630*1024,       24*1024,        UFS_PART_3},/* kernel                     24M    p40*/
#endif
  {PART_ENG_SYSTEM,                     654*1024,       12*1024,        UFS_PART_3},/* eng_system                 12M    p41*/
  {PART_RECOVERY_RAMDISK,               666*1024,       32*1024,        UFS_PART_3},/* recovery_ramdisk           32M    p42*/
  {PART_RECOVERY_VENDOR,                698*1024,       16*1024,        UFS_PART_3},/* recovery_vendor            16M    p43*/
  {PART_DTS,                            714*1024,        1*1024,        UFS_PART_3},/* dtimage                     1M    p44*/
  {PART_DTO,                            715*1024,       20*1024,        UFS_PART_3},/* dtoimage                   20M    p45*/
  {PART_TRUSTFIRMWARE,                  735*1024,        2*1024,        UFS_PART_3},/* trustfirmware               2M    p46*/
  {PART_MODEM_FW,                       737*1024,       56*1024,        UFS_PART_3},/* modem_fw                   56M    p47*/
  {PART_ENG_VENDOR,                     793*1024,       12*1024,        UFS_PART_3},/* eng_vendor                 12M    p48*/
  {PART_MODEM_PATCH_NV,                 805*1024,        4*1024,        UFS_PART_3},/* modem_patch_nv              4M    p49*/
  {PART_MODEM_DRIVER,                   809*1024,       20*1024,        UFS_PART_3},/* modem_driver               20M    p50*/
  {PART_KPATCH,                         829*1024,       12*1024,        UFS_PART_3},/* kpatch                     12M    p51*/
  {PART_RAMDISK,                        841*1024,        2*1024,        UFS_PART_3},/* ramdisk                     2M    p52*/
  {PART_VBMETA_SYSTEM,                  843*1024,        1*1024,        UFS_PART_3},/* vbmeta_system               1M    p53*/
  {PART_VBMETA_VENDOR,                  844*1024,        1*1024,        UFS_PART_3},/* vbmeta_vendor               1M    p54*/
  {PART_VBMETA_ODM,                     845*1024,        1*1024,        UFS_PART_3},/* vbmeta_odm                  1M    p55*/
  {PART_VBMETA_CUST,                    846*1024,        1*1024,        UFS_PART_3},/* vbmeta_cust                 1M    p56*/
  {PART_VBMETA_HW_PRODUCT,              847*1024,        1*1024,        UFS_PART_3},/* vbmeta_hw_product           1M    p57*/
  {PART_RECOVERY_VBMETA,                848*1024,        2*1024,        UFS_PART_3},/* recovery_vbmeta             2M    p58*/
  {PART_ERECOVERY_VBMETA,               850*1024,        2*1024,        UFS_PART_3},/* erecovery_vbmeta            2M    p59*/
  {PART_VBMETA,                         852*1024,        4*1024,        UFS_PART_3},/* vbmeta                      4M    p60*/
  {PART_MODEMNVM_UPDATE,                856*1024,       16*1024,        UFS_PART_3},/* modemnvm_update            16M    p61*/
  {PART_MODEMNVM_CUST,                  872*1024,       16*1024,        UFS_PART_3},/* modemnvm_cust              16M    p62*/
  {PART_PATCH,                          888*1024,       32*1024,        UFS_PART_3},/* patch                      32M    p63*/
#ifdef CONFIG_FACTORY_MODE
  {PART_PREAS,                          920*1024,        1216*1024,        UFS_PART_3},/* preas           1216M          p64*/
  {PART_PREAVS,                        2136*1024,          32*1024,        UFS_PART_3},/* preavs            32M          p65*/
  {PART_PRETS,                         2168*1024,          24*1024,        UFS_PART_3},/* prets             24M          p66*/
  {PART_PRETVS,                        2192*1024,           8*1024,        UFS_PART_3},/* pretvs             8M          p67*/
  {PART_SUPER,                         2200*1024,        7544*1024,        UFS_PART_3},/* super           7544M          p68*/
  {PART_VERSION,                       9744*1024,         576*1024,        UFS_PART_3},/* version          576M          p69*/
  {PART_PRELOAD,                      10320*1024,        1144*1024,        UFS_PART_3},/* preload_a       1144M          p70*/
  {PART_HIBENCH_IMG,                  11464*1024,         128*1024,        UFS_PART_3},/* hibench_img      128M          p71*/
  {PART_HIBENCH_DATA,                 11592*1024,         512*1024,        UFS_PART_3},/* hibench_data     512M          p72*/
  {PART_FLASH_AGEING,                 12104*1024,         512*1024,        UFS_PART_3},/* FLASH_AGEING     512M          p73*/
  {PART_HIBENCH_LOG,                  12616*1024,          32*1024,        UFS_PART_3},/* HIBENCH_LOG       32M          p74*/
  {PART_USERDATA,                     12648*1024,  (4UL)*1024*1024,        UFS_PART_3},/* userdata           4G          p75*/
#else
#ifdef CONFIG_USE_EROFS
#ifdef CONFIG_UPDATE_PRODUCT_EROFS
#ifdef CONFIG_MARKET_INTERNAL
  {PART_PREAS,                          920*1024,         360*1024,        UFS_PART_3},/* preas            360M          p64*/
  {PART_PREAVS,                        1280*1024,          32*1024,        UFS_PART_3},/* preavs            32M          p65*/
  {PART_PRETS,                         1312*1024,          24*1024,        UFS_PART_3},/* prets             24M          p66*/
  {PART_PRETVS,                        1336*1024,           8*1024,        UFS_PART_3},/* pretvs             8M          p67*/
  {PART_SUPER,                         1344*1024,        7448*1024,        UFS_PART_3},/* super           7448M          p68*/
  {PART_VERSION,                       8792*1024,         576*1024,        UFS_PART_3},/* version          576M          p69*/
  {PART_PRELOAD,                       9368*1024,        1144*1024,        UFS_PART_3},/* preload         1144M          p70*/
  {PART_USERDATA,                     10512*1024,  (4UL)*1024*1024,        UFS_PART_3},/* userdata           4G          p71*/
#else
  {PART_PREAS,                          920*1024,        1216*1024,        UFS_PART_3},/* preas           1216M          p64*/
  {PART_PREAVS,                        2136*1024,          32*1024,        UFS_PART_3},/* preavs            32M          p65*/
  {PART_PRETS,                         2168*1024,          24*1024,        UFS_PART_3},/* prets             24M          p66*/
  {PART_PRETVS,                        2192*1024,           8*1024,        UFS_PART_3},/* pretvs             8M          p67*/
  {PART_SUPER,                         2200*1024,        7512*1024,        UFS_PART_3},/* super           7512M          p68*/
  {PART_VERSION,                       9712*1024,         576*1024,        UFS_PART_3},/* version          576M          p69*/
  {PART_PRELOAD,                      10288*1024,        1144*1024,        UFS_PART_3},/* preload         1144M          p70*/
  {PART_USERDATA,                     11432*1024,  (4UL)*1024*1024,        UFS_PART_3},/* userdata           4G          p71*/
#endif
#else
#ifdef CONFIG_MARKET_INTERNAL
  {PART_PREAS,                          920*1024,         296*1024,        UFS_PART_3},/* preas            296M          p64*/
  {PART_PREAVS,                        1216*1024,          32*1024,        UFS_PART_3},/* preavs            32M          p65*/
  {PART_PRETS,                         1248*1024,          24*1024,        UFS_PART_3},/* prets             24M          p66*/
  {PART_PRETVS,                        1272*1024,           8*1024,        UFS_PART_3},/* pretvs             8M          p67*/
  {PART_SUPER,                         1280*1024,        5600*1024,        UFS_PART_3},/* super           5600M          p68*/
  {PART_VERSION,                       6880*1024,         576*1024,        UFS_PART_3},/* version          576M          p69*/
  {PART_PRELOAD,                       7456*1024,        1144*1024,        UFS_PART_3},/* preload         1144M          p70*/
  {PART_USERDATA,                      8600*1024,  (4UL)*1024*1024,        UFS_PART_3},/* userdata           4G          p71*/
#else
  {PART_PREAS,                          920*1024,         976*1024,        UFS_PART_3},/* preas            976M          p64*/
  {PART_PREAVS,                        1896*1024,          32*1024,        UFS_PART_3},/* preavs            32M          p65*/
  {PART_PRETS,                         1928*1024,          24*1024,        UFS_PART_3},/* prets             24M          p66*/
  {PART_PRETVS,                        1952*1024,           8*1024,        UFS_PART_3},/* pretvs             8M          p67*/
  {PART_SUPER,                         1960*1024,        5520*1024,        UFS_PART_3},/* super           5520M          p68*/
  {PART_VERSION,                       7480*1024,         576*1024,        UFS_PART_3},/* version          576M          p69*/
  {PART_PRELOAD,                       8056*1024,        1144*1024,        UFS_PART_3},/* preload         1144M          p70*/
  {PART_USERDATA,                      9200*1024,  (4UL)*1024*1024,        UFS_PART_3},/* userdata           4G          p71*/
#endif
#endif
#else
#ifdef CONFIG_MARKET_INTERNAL
  {PART_PREAS,                          920*1024,         360*1024,        UFS_PART_3},/* preas            360M          p64*/
  {PART_PREAVS,                        1280*1024,          32*1024,        UFS_PART_3},/* preavs            32M          p65*/
  {PART_PRETS,                         1312*1024,          24*1024,        UFS_PART_3},/* prets             24M          p66*/
  {PART_PRETVS,                        1336*1024,           8*1024,        UFS_PART_3},/* pretvs             8M          p67*/
  {PART_SUPER,                         1344*1024,        7448*1024,        UFS_PART_3},/* super           7448M          p68*/
  {PART_VERSION,                       8792*1024,         576*1024,        UFS_PART_3},/* version          576M          p69*/
  {PART_PRELOAD,                       9368*1024,        1144*1024,        UFS_PART_3},/* preload         1144M          p70*/
  {PART_USERDATA,                     10512*1024,  (4UL)*1024*1024,        UFS_PART_3},/* userdata           4G          p71*/
#else
  {PART_PREAS,                          920*1024,        1216*1024,        UFS_PART_3},/* preas           1216M          p64*/
  {PART_PREAVS,                        2136*1024,          32*1024,        UFS_PART_3},/* preavs            32M          p65*/
  {PART_PRETS,                         2168*1024,          24*1024,        UFS_PART_3},/* prets             24M          p66*/
  {PART_PRETVS,                        2192*1024,           8*1024,        UFS_PART_3},/* pretvs             8M          p67*/
  {PART_SUPER,                         2200*1024,        7512*1024,        UFS_PART_3},/* super           7512M          p68*/
  {PART_VERSION,                       9712*1024,         576*1024,        UFS_PART_3},/* version          576M          p69*/
  {PART_PRELOAD,                      10288*1024,        1144*1024,        UFS_PART_3},/* preload         1144M          p70*/
  {PART_USERDATA,                     11432*1024,  (4UL)*1024*1024,        UFS_PART_3},/* userdata           4G          p71*/
#endif
#endif
#endif
  {"0", 0, 0, 0},
};

#endif
