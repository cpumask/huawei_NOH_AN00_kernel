

/* 1 ͷ�ļ����� */
#include "wal_regdb.h"
#include "oam_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_REGDB_C

/* 2 ȫ�ֱ������� */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "wal_regdb_1103.c"

/* Ĭ�Ϲ�������Ϣ */
OAL_CONST oal_ieee80211_regdomain_stru g_st_default_regdom = {
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
    .n_reg_rules = 6, // 6������
#else
    .n_reg_rules = 4,
#endif
    .alpha2 = "99",
    .reg_rules = {
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
        REG_RULE(2402, 2482, 40, 6, 25, 0), // chan:1~13
        REG_RULE(5170, 5250, 160, 6, 25, 0), // chan:36~52
        REG_RULE(5250, 5330, 160, 6, 25, NL80211_RRF_DFS | 0), // chan:52~64 DFS Passive scan
        REG_RULE(5490, 5710, 160, 6, 25, NL80211_RRF_DFS | 0), // chan:100~140 DFS Passive scan
        REG_RULE(5735, 5835, 160, 6, 25, 0), // chan:149~165
        REG_RULE(4910, 4990, 80, 6, 25, 0), // chan:184~196
#else
        /* IEEE 802.11b/g, �ŵ� 1..13 */
        REG_RULE(2412 - 10, 2472 + 10, 40, 6, 25, 0), /*lint !e651*/
        /* �ŵ� 36 - 64 */
        REG_RULE(5150 - 10, 5350 + 10, 160, 6, 25, 0),
        /* �ŵ� 100 - 165 */
        REG_RULE(5470 - 10, 5850 + 10, 160, 6, 25, 0),
        /* IEEE 802.11 �ŵ� 184,188,192,196 �������ձ�4.9G */
        REG_RULE(4920 - 10, 4980 + 10, 80, 6, 25, 0),
#endif
    }
};

#elif (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
/* Win32�´���ֻ��װ�������ҵĹ�������Ϣ�� Ϊ��UT�� */
OAL_STATIC OAL_CONST oal_ieee80211_regdomain_stru regdom_AU = {
    5,
    { 'A', 'U' },
    { 0, 0 },
    {
        REG_RULE(2402, 2482, 40, 0, 20, 0),
        REG_RULE(5170, 5250, 80, 3, 17, 0),
        REG_RULE(5250, 5330, 80, 3, 24,
                 NL80211_RRF_DFS | 0),
        REG_RULE(5490, 5710, 80, 3, 24, 0),
        REG_RULE(5735, 5835, 80, 3, 30, 0),
    }
};

OAL_STATIC OAL_CONST oal_ieee80211_regdomain_stru regdom_AT = {
    4,
    { 'A', 'T' },
    { 0, 0 },
    {
        REG_RULE(2402, 2482, 40, 0, 20, 0),
        REG_RULE(5170, 5250, 80, 0, 23, 0),
        REG_RULE(5250, 5330, 80, 0, 23,
                 NL80211_RRF_DFS | 0),
        REG_RULE(5490, 5710, 80, 0, 23,
                 NL80211_RRF_DFS | 0),
    }
};

OAL_STATIC OAL_CONST oal_ieee80211_regdomain_stru regdom_CN = {
    2,
    { 'C', 'N' },
    { 0, 0 },
    {
        REG_RULE(2402, 2482, 40, 0, 20, 0),
        REG_RULE(5735, 5835, 80, 0, 30, 0),
    }
};

OAL_STATIC OAL_CONST oal_ieee80211_regdomain_stru regdom_JP = {
    7,
    { 'J', 'P' },
    { 0, 0 },
    {
        REG_RULE(2402, 2482, 40, 0, 20, 0),
        REG_RULE(2474, 2494, 20, 0, 20,
                 NL80211_RRF_NO_OFDM | 0),
        REG_RULE(4910, 4990, 40, 0, 23, 0),
        REG_RULE(5030, 5090, 80, 0, 23, 0),
        REG_RULE(5170, 5250, 80, 0, 20, 0),
        REG_RULE(5250, 5330, 80, 0, 20,
                 NL80211_RRF_DFS | 0),
        REG_RULE(5490, 5710, 80, 0, 23,
                 NL80211_RRF_DFS | 0),
    }
};

OAL_STATIC OAL_CONST oal_ieee80211_regdomain_stru regdom_GB = {
    4,
    { 'G', 'B' },
    { 0, 0 },
    {
        REG_RULE(2402, 2482, 40, 0, 20, 0),
        REG_RULE(5170, 5250, 80, 0, 20, 0),
        REG_RULE(5250, 5330, 80, 0, 20,
                 NL80211_RRF_DFS | 0),
        REG_RULE(5490, 5710, 80, 0, 27,
                 NL80211_RRF_DFS | 0),
    }
};

OAL_STATIC OAL_CONST oal_ieee80211_regdomain_stru regdom_US = {
    6,
    { 'U', 'S' },
    { 0, 0 },
    {
        REG_RULE(2402, 2472, 40, 3, 27, 0),
        REG_RULE(5170, 5250, 80, 3, 17, 0),
        REG_RULE(5250, 5330, 80, 3, 20,
                 NL80211_RRF_DFS | 0),
        REG_RULE(5490, 5600, 80, 3, 20,
                 NL80211_RRF_DFS | 0),
        REG_RULE(5650, 5710, 80, 3, 20,
                 NL80211_RRF_DFS | 0),
        REG_RULE(5735, 5835, 80, 3, 30, 0),
    },
};

OAL_CONST oal_ieee80211_regdomain_stru *reg_regdb[] = {
    &regdom_AU,
    &regdom_AT,
    &regdom_CN,
    &regdom_JP,
    &regdom_GB,
    &regdom_US,
};

int reg_regdb_size = oal_array_size(reg_regdb);

/* Ĭ�Ϲ�������Ϣ */
OAL_CONST oal_ieee80211_regdomain_stru g_st_default_regdom = {
    4,            /* n_reg_rules */
    { '9', '9' }, /* alpha2 */
    { 0, 0 },
    {
        /* reg_rules */
        /* IEEE 802.11b/g, �ŵ� 1..13 */
        REG_RULE(2402, 2482, 40, 6, 20, 0),

        /* �ŵ� 36 - 64 */
        REG_RULE(5150 - 10, 5350 + 10, 40, 6, 20, (0)),

        /* �ŵ� 100 - 165 */
        REG_RULE(5470 - 10, 5850 + 10, 40, 6, 20, (0)),

        /* IEEE 802.11 �ŵ� 184,188,192,196 ���ձ�ʹ�� 4.9G */
        REG_RULE(4920 - 10, 4980 + 10, 40, 6, 23, (0)),
    },
};

#endif

/* 3 ����ʵ�� */
#ifdef _PRE_WLAN_FEATURE_11D

OAL_CONST oal_ieee80211_regdomain_stru *wal_regdb_find_db(int8_t *str)
{
    int32_t db_idx;

    if (oal_unlikely(str == OAL_PTR_NULL)) {
        return OAL_PTR_NULL;
    }

    /* Ĭ�Ϲ����� */
    if ((str[0] == '9') && (str[1] == '9')) {
        return &g_st_default_regdom;
    }

    for (db_idx = 0; db_idx < reg_regdb_size; db_idx++) {
        if ((str[0] == reg_regdb[db_idx]->alpha2[0]) &&
            (str[1] == reg_regdb[db_idx]->alpha2[1])) {
            return reg_regdb[db_idx];
        }
    }
    /* �Ҳ�����������Ϣʱ������һ���ض������� */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oam_warning_log4(0, OAM_SF_ANY,
                     "{wal_regdb_find_db::regdomain %c,%c was not found,return default regdomain %c,%c.}\r\n",
                     str[0], str[1], reg_regdb[HI1103_SPECIFIC_COUNTRY_CODE_IDX]->alpha2[0],
                     reg_regdb[HI1103_SPECIFIC_COUNTRY_CODE_IDX]->alpha2[1]);
    return reg_regdb[HI1103_SPECIFIC_COUNTRY_CODE_IDX];
#else
    return OAL_PTR_NULL;
#endif
}
#endif





