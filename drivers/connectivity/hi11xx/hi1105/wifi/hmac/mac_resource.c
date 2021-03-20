

#include "wlan_spec.h"
#include "mac_resource.h"
#include "hmac_vap.h"
#include "hmac_user.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_RESOURCE_C

/* 2 全局变量定义 */
OAL_STATIC wlan_mib_ieee802dot11_stru g_ast_mib_res[WLAN_SERVICE_VAP_SUPPORT_MAX_NUM_LIMIT];
OAL_STATIC hmac_vap_stru g_ast_hmac_vap[WLAN_VAP_SUPPORT_MAX_NUM_LIMIT];
OAL_STATIC hmac_user_stru g_ast_hmac_user[MAC_RES_MAX_USER_LIMIT];
OAL_STATIC mac_res_stru g_st_mac_res;

mac_res_stru *g_pst_mac_res = &g_st_mac_res;


uint32_t mac_res_alloc_hmac_dev(uint8_t *puc_dev_idx)
{
    unsigned long ul_dev_idx_temp;

    if (oal_unlikely(puc_dev_idx == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_res_alloc_hmac_dev::puc_dev_idx null.}");

        return OAL_FAIL;
    }

    ul_dev_idx_temp = (unsigned long)(uintptr_t)oal_queue_dequeue(&(g_pst_mac_res->st_dev_res.st_queue));

    /* 0为无效值 */
    if (ul_dev_idx_temp == 0) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_res_alloc_hmac_dev::ul_dev_idx_temp=0.}");

        return OAL_FAIL;
    }

    *puc_dev_idx = (uint8_t)(ul_dev_idx_temp - 1);

    (g_pst_mac_res->st_dev_res.auc_user_cnt[ul_dev_idx_temp - 1])++;

    return OAL_SUCC;
}

wlan_mib_ieee802dot11_stru *mac_res_get_mib_info(uint8_t uc_vap_id)
{
    if (oal_unlikely(uc_vap_id == 0 || uc_vap_id >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT)) {
        return OAL_PTR_NULL;
    }

    return &g_ast_mib_res[uc_vap_id - 1];
}


void mac_res_vap_init(void)
{
    uint32_t ul_loop;

    oal_queue_set(&(g_pst_mac_res->st_vap_res.st_queue),
                  g_pst_mac_res->st_vap_res.aul_idx,
                  (uint8_t)WLAN_VAP_SUPPORT_MAX_NUM_LIMIT);
    g_pst_mac_res->st_vap_res.us_hmac_priv_size = (uint16_t)OAL_OFFSET_OF(hmac_vap_stru, st_vap_base_info);

    for (ul_loop = 0; ul_loop < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; ul_loop++) {
        g_pst_mac_res->st_vap_res.past_vap_info[ul_loop] = (void *)&g_ast_hmac_vap[ul_loop];
        memset_s(g_pst_mac_res->st_vap_res.past_vap_info[ul_loop],
                 OAL_SIZEOF(hmac_vap_stru), 0,
                 OAL_SIZEOF(hmac_vap_stru));
        /* 初始值保存的是对应数组下标值加1 */
        oal_queue_enqueue (&(g_pst_mac_res->st_vap_res.st_queue), (void *)(uintptr_t)(ul_loop + 1));
        /* 初始化对应的引用计数值为0 */
        g_pst_mac_res->st_vap_res.auc_user_cnt[ul_loop] = 0;
    }
}


uint32_t mac_res_user_init(void)
{
    unsigned long  ul_loop;
    void *p_user_info = OAL_PTR_NULL;
    void *p_idx = OAL_PTR_NULL;
    void *p_user_cnt = OAL_PTR_NULL;
    uint32_t  ul_one_user_info_size;

    /* 初始化USER的资源管理内容 */
    /* 动态申请用户资源池相关内存 */
    p_user_info = (void *)g_ast_hmac_user;

    p_idx = oal_memalloc(OAL_SIZEOF(unsigned long) * MAC_RES_MAX_USER_LIMIT);
    p_user_cnt = oal_memalloc(OAL_SIZEOF(uint8_t) * MAC_RES_MAX_USER_LIMIT);

    if (oal_any_null_ptr3(p_user_info, p_idx, p_user_cnt)) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_res_user_init::param null.}");

        if (p_idx != OAL_PTR_NULL) {
            oal_free(p_idx);
        }

        if (p_user_cnt != OAL_PTR_NULL) {
            oal_free(p_user_cnt);
        }

        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* 内存初始清0 */
    memset_s(p_user_info, (OAL_SIZEOF(hmac_user_stru) * MAC_RES_MAX_USER_LIMIT),
             0, (OAL_SIZEOF(hmac_user_stru) * MAC_RES_MAX_USER_LIMIT));

    memset_s(p_idx, (OAL_SIZEOF(unsigned long) * MAC_RES_MAX_USER_LIMIT),
             0, (OAL_SIZEOF(unsigned long) * MAC_RES_MAX_USER_LIMIT));
    memset_s(p_user_cnt, (OAL_SIZEOF(uint8_t) * MAC_RES_MAX_USER_LIMIT),
             0, (OAL_SIZEOF(uint8_t) * MAC_RES_MAX_USER_LIMIT));

    g_pst_mac_res->st_user_res.pul_idx = p_idx;
    g_pst_mac_res->st_user_res.puc_user_cnt = p_user_cnt;

    oal_queue_set_16(&(g_pst_mac_res->st_user_res.st_queue),
                     g_pst_mac_res->st_user_res.pul_idx,
                     (uint16_t)g_wlan_spec_cfg->max_user_limit);
    g_pst_mac_res->st_user_res.us_hmac_priv_size = 0;

    ul_one_user_info_size = 0;
    g_pst_mac_res->st_user_res.past_user_info[0] = p_user_info;
    for (ul_loop = 0; ul_loop < g_wlan_spec_cfg->max_user_limit; ul_loop++) {
        /* 初始值保存的是对应数组下标值加1 */
        oal_queue_enqueue_16 (&(g_pst_mac_res->st_user_res.st_queue), (void *)(uintptr_t)(ul_loop + 1));

        /* 初始化对应的引用位置 */
        g_pst_mac_res->st_user_res.past_user_info[ul_loop] = (uint8_t *)p_user_info + ul_one_user_info_size;
        ul_one_user_info_size += OAL_SIZEOF(hmac_user_stru);
    }

    return OAL_SUCC;
}

uint32_t mac_res_exit(void)
{
    uint32_t ul_loop;

    oal_free(g_pst_mac_res->st_user_res.pul_idx);
    oal_free(g_pst_mac_res->st_user_res.puc_user_cnt);
    for (ul_loop = 0; ul_loop < g_wlan_spec_cfg->max_user_limit; ul_loop++) {
        g_pst_mac_res->st_user_res.past_user_info[ul_loop] = OAL_PTR_NULL;
    }
    g_pst_mac_res->st_user_res.pul_idx = OAL_PTR_NULL;
    g_pst_mac_res->st_user_res.puc_user_cnt = OAL_PTR_NULL;

    return OAL_SUCC;
}

/*
 * 功能描述  : 初始化MAC资源池内容
 */
uint32_t mac_res_init(void)
{
    unsigned long    ul_loop;

    memset_s(g_pst_mac_res, OAL_SIZEOF(mac_res_stru), 0, OAL_SIZEOF(mac_res_stru));
    /* 初始化DEV的资源管理内容 */
    oal_queue_set(&(g_pst_mac_res->st_dev_res.st_queue),
                  g_pst_mac_res->st_dev_res.aul_idx,
                  MAC_RES_MAX_DEV_NUM);

    for (ul_loop = 0; ul_loop < MAC_RES_MAX_DEV_NUM; ul_loop++) {
        /* 初始值保存的是对应数组下标值加1 */
        oal_queue_enqueue (&(g_pst_mac_res->st_dev_res.st_queue), (void *)(uintptr_t)(ul_loop + 1));

        /* 初始化对应的引用计数值为0 */
        g_pst_mac_res->st_dev_res.auc_user_cnt[ul_loop] = 0;
    }

    /* lint -e413 */
    /* 初始化VAP的资源管理内容 */
    mac_res_vap_init();

    return mac_res_user_init();
}

/*
 * 功能描述  : 释放对应HMAC USER的内存
 */
uint32_t mac_res_free_mac_user(uint16_t us_idx)
{
    if (oal_unlikely(us_idx >= g_wlan_spec_cfg->max_user_limit)) {
        return OAL_FAIL;
    }

    if (g_pst_mac_res->st_user_res.puc_user_cnt[us_idx] == 0) {
        oam_error_log1(0, OAM_SF_ANY, "mac_res_free_mac_user::cnt==0! idx:%d", us_idx);
        oal_dump_stack();
        return OAL_ERR_CODE_USER_RES_CNT_ZERO;
    }

    (g_pst_mac_res->st_user_res.puc_user_cnt[us_idx])--;

    if (g_pst_mac_res->st_user_res.puc_user_cnt[us_idx] != 0) {
        return OAL_SUCC;
    }

    /* 入队索引值需要加1操作 */
    oal_queue_enqueue_16(&(g_pst_mac_res->st_user_res.st_queue), (void *)(uintptr_t)((unsigned long) us_idx + 1));

    return OAL_SUCC;
}

/*
 * 功能描述  : 释放对应HMAC VAP的内存
 */
uint32_t mac_res_free_mac_vap(uint32_t ul_idx)
{
    if (oal_unlikely(ul_idx >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT)) {
        return OAL_FAIL;
    }

    if (g_pst_mac_res->st_vap_res.auc_user_cnt[ul_idx] == 0) {
        oam_error_log1(0, OAM_SF_ANY, "mac_res_free_mac_vap::cnt==0! idx:%d", ul_idx);
        oal_dump_stack();
        return OAL_SUCC;
    }

    (g_pst_mac_res->st_vap_res.auc_user_cnt[ul_idx])--;

    if (g_pst_mac_res->st_vap_res.auc_user_cnt[ul_idx] != 0) {
        return OAL_SUCC;
    }

    /* 入队索引值需要加1操作 */
    oal_queue_enqueue(&(g_pst_mac_res->st_vap_res.st_queue), (void *)(uintptr_t)((unsigned long) ul_idx + 1));

    return OAL_SUCC;
}

/*
 * 功能描述  : 释放对应DEV的内存
 */
uint32_t mac_res_free_dev(uint32_t ul_dev_idx)
{
    if (oal_unlikely(ul_dev_idx >= MAC_RES_MAX_DEV_NUM)) {
        oam_error_log1(0, OAM_SF_ANY, "{mac_res_free_dev::invalid ul_dev_idx[%d].}", ul_dev_idx);

        return OAL_FAIL;
    }

    if (g_pst_mac_res->st_dev_res.auc_user_cnt[ul_dev_idx] == 0) {
        oam_error_log1(0, OAM_SF_ANY, "mac_res_free_dev::cnt==0! idx:%d", ul_dev_idx);
        oal_dump_stack();
        return OAL_SUCC;
    }

    (g_pst_mac_res->st_dev_res.auc_user_cnt[ul_dev_idx])--;

    if (g_pst_mac_res->st_dev_res.auc_user_cnt[ul_dev_idx] != 0) {
        return OAL_SUCC;
    }

    /* 入队索引值需要加1操作 */
    oal_queue_enqueue(&(g_pst_mac_res->st_dev_res.st_queue), (void *)(uintptr_t)((unsigned long) ul_dev_idx + 1));

    return OAL_SUCC;
}

/*
 * 功能描述  : 获取对应MAC CHIP索引的内存
 */
mac_chip_stru *mac_res_get_mac_chip(uint32_t ul_chip_idx)
{
    if (oal_unlikely(ul_chip_idx >= WLAN_CHIP_MAX_NUM_PER_BOARD)) {
        oam_error_log1(0, OAM_SF_ANY, "{mac_res_get_mac_chip::invalid ul_chip_idx[%d].}", ul_chip_idx);
        return OAL_PTR_NULL;
    }

    return &(g_pst_mac_board->ast_chip[ul_chip_idx]);
}

uint16_t mac_chip_get_max_asoc_user(uint8_t uc_chip_id)
{
    /* 关联单播最大用户数 */
    return g_wlan_spec_cfg->max_asoc_user;
}

uint8_t mac_chip_get_max_active_user(void)
{
    /* 关联单播active最大用户数 */
    return g_wlan_spec_cfg->max_active_user;
}
uint16_t mac_board_get_max_user(void)
{
    /* 整板单播和组播用户总数，整板单播和组播用户数直接取用宏值即可 */
    return g_wlan_spec_cfg->max_user_limit;
}

