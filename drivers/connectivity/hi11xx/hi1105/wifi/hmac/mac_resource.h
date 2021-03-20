

#ifndef __MAC_RESOURCE_H__
#define __MAC_RESOURCE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "oal_queue.h"
#include "mac_device.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_RESOURCE_H

/* 2 宏定义 */
/* 最大dev数量 */
#define MAC_RES_MAX_DEV_NUM     WLAN_SERVICE_DEVICE_SUPPORT_MAX_NUM_SPEC

/* board最大关联用户数 = 1个CHIP支持的最大关联用户数 * board上面的CHIP数目 */
#define MAC_RES_MAX_ASOC_USER_NUM (WLAN_ASSOC_USER_MAX_NUM * WLAN_CHIP_MAX_NUM_PER_BOARD)
/* board最大组播用户数 = 整board支持的业务vap个数(每个业务vap一个组播用户) */
#define MAC_RES_MAX_BCAST_USER_NUM (WLAN_MULTI_USER_MAX_NUM_LIMIT)
/* board资源最大用户数 = 最大关联用户数 + 组播用户个数 */
#define MAC_RES_MAX_USER_LIMIT (MAC_RES_MAX_ASOC_USER_NUM + MAC_RES_MAX_BCAST_USER_NUM)

typedef struct {
    unsigned long ul_user_idx_size;
} mac_res_user_idx_size_stru;

typedef struct {
    uint8_t ul_user_cnt_size;
} mac_res_user_cnt_size_stru;

typedef struct {
    mac_device_stru ast_dev_info[MAC_RES_MAX_DEV_NUM];
    oal_queue_stru st_queue;
    unsigned long aul_idx[MAC_RES_MAX_DEV_NUM];
    uint8_t auc_user_cnt[MAC_RES_MAX_DEV_NUM];
    uint8_t auc_resv[2];
} mac_res_device_stru;

typedef struct {
    void *past_vap_info[WLAN_VAP_SUPPORT_MAX_NUM_LIMIT];
    oal_queue_stru st_queue;
    unsigned long aul_idx[WLAN_VAP_SUPPORT_MAX_NUM_LIMIT];
    uint16_t us_hmac_priv_size;
    uint8_t auc_user_cnt[WLAN_VAP_SUPPORT_MAX_NUM_LIMIT];
    uint8_t auc_resv[2];
} mac_res_vap_stru;

typedef struct {
    void *past_user_info[MAC_RES_MAX_USER_LIMIT];
    oal_queue_stru_16 st_queue;
    unsigned long *pul_idx;
    uint16_t us_hmac_priv_size;
    uint8_t auc_resv[2];
    uint8_t *puc_user_cnt;

} mac_res_user_stru;

typedef struct {
    mac_res_device_stru st_dev_res;
    mac_res_vap_stru st_vap_res;
    mac_res_user_stru st_user_res;
} mac_res_stru;

extern mac_res_stru *g_pst_mac_res;

/* 10 函数声明 */
uint32_t mac_res_alloc_hmac_dev(uint8_t *puc_dev_idx);
uint32_t mac_res_free_dev(uint32_t ul_dev_idx);
uint32_t mac_res_free_mac_user(uint16_t us_idx);
uint32_t mac_res_free_mac_vap(uint32_t ul_idx);
uint32_t mac_res_init(void);
uint32_t mac_res_exit(void);
mac_chip_stru *mac_res_get_mac_chip(uint32_t ul_chip_idx);
uint16_t mac_chip_get_max_asoc_user(uint8_t uc_chip_id);
uint8_t mac_chip_get_max_active_user(void);
uint16_t mac_board_get_max_user(void);
wlan_mib_ieee802dot11_stru *mac_res_get_mib_info(uint8_t uc_vap_id);


OAL_STATIC OAL_INLINE uint32_t mac_res_alloc_hmac_vap(uint8_t *puc_idx, uint16_t us_hmac_priv_size)
{
    unsigned long ul_idx_temp;

    if (oal_unlikely(puc_idx == OAL_PTR_NULL)) {
        return OAL_FAIL;
    }

    ul_idx_temp = (unsigned long)(uintptr_t)oal_queue_dequeue(&(g_pst_mac_res->st_vap_res.st_queue));

    /* 0为无效值 */
    if (ul_idx_temp == 0) {
        return OAL_FAIL;
    }

    *puc_idx = (uint8_t)(ul_idx_temp - 1);

    g_pst_mac_res->st_vap_res.us_hmac_priv_size = us_hmac_priv_size;

    (g_pst_mac_res->st_vap_res.auc_user_cnt[ul_idx_temp - 1])++;

    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE uint32_t mac_res_alloc_dmac_vap(uint8_t uc_idx)
{
    if (oal_unlikely(uc_idx >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT)) {
        return OAL_FAIL;
    }

    (g_pst_mac_res->st_vap_res.auc_user_cnt[uc_idx])++;

    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE mac_device_stru *mac_res_get_dev(uint32_t ul_dev_idx)
{
    if (oal_unlikely(ul_dev_idx >= MAC_RES_MAX_DEV_NUM)) {
        return OAL_PTR_NULL;
    }

    return &(g_pst_mac_res->st_dev_res.ast_dev_info[ul_dev_idx]);
}

OAL_STATIC OAL_INLINE mac_device_stru *mac_res_get_mac_dev(void)
{
    return &(g_pst_mac_res->st_dev_res.ast_dev_info[0]);
}

OAL_STATIC OAL_INLINE void *mac_res_get_hmac_vap(uint32_t ul_idx)
{
    if (oal_unlikely(ul_idx >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT)) {
        return OAL_PTR_NULL;
    }

    return (void *)(g_pst_mac_res->st_vap_res.past_vap_info[ul_idx]);
}


OAL_STATIC OAL_INLINE void *mac_res_get_mac_vap(uint8_t uc_idx)
{
    if (oal_unlikely(uc_idx >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT)) {
        return OAL_PTR_NULL;
    }

    /* 这里返回偏移内存空间 */
    return (void *)((uint8_t *)(g_pst_mac_res->st_vap_res.past_vap_info[uc_idx])
                        + g_pst_mac_res->st_vap_res.us_hmac_priv_size);
}


OAL_STATIC OAL_INLINE void *mac_res_get_dmac_vap(uint8_t uc_idx)
{
    return mac_res_get_mac_vap(uc_idx);
}


OAL_STATIC OAL_INLINE uint32_t mac_res_alloc_hmac_user(uint16_t *pus_idx, uint16_t us_hmac_priv_size)
{
    unsigned long ul_idx_temp;

    if (oal_unlikely(pus_idx == OAL_PTR_NULL)) {
        return OAL_FAIL;
    }

    ul_idx_temp = (unsigned long)(uintptr_t)oal_queue_dequeue_16(&(g_pst_mac_res->st_user_res.st_queue));

    /* 0为无效值 */
    if (ul_idx_temp == 0) {
        return OAL_FAIL;
    }

    *pus_idx = (uint16_t)(ul_idx_temp - 1);

    g_pst_mac_res->st_user_res.us_hmac_priv_size = us_hmac_priv_size;

    (g_pst_mac_res->st_user_res.puc_user_cnt[ul_idx_temp - 1])++;

    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE void *_mac_res_get_hmac_user(uint16_t us_idx)
{
    if (oal_unlikely(us_idx >= g_wlan_spec_cfg->max_user_limit)) {
        return OAL_PTR_NULL;
    }

    return (void *)(g_pst_mac_res->st_user_res.past_user_info[us_idx]);
}


OAL_STATIC OAL_INLINE void *_mac_res_get_mac_user(uint16_t us_idx)
{
    if (oal_unlikely(us_idx >= g_wlan_spec_cfg->max_user_limit)) {
        return OAL_PTR_NULL;
    }

    /* 这里偏移内存空间 */
    return (void *)((uint8_t *)(g_pst_mac_res->st_user_res.past_user_info[us_idx])
                        + g_pst_mac_res->st_user_res.us_hmac_priv_size);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of mac_resource.h */
