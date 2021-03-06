

#ifndef __WAL_REGDB_H__
#define __WAL_REGDB_H__

/* 1 其他头文件包含 */
#include "oal_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_REGDB_H
/* 2 宏定义 */
#define HI1103_SPECIFIC_COUNTRY_CODE_IDX (0)

/* 3 枚举定义 */
/* 4 全局变量声明 */
extern OAL_CONST oal_ieee80211_regdomain_stru g_st_default_regdom;

/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
OAL_CONST oal_ieee80211_regdomain_stru *wal_regdb_find_db(int8_t *str);
#endif /* end of wal_regdb.h */
