

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 头文件包含 */
#include "hmac_main.h"
#include "mac_vap.h"
#include "hmac_vap.h"
#include "mac_resource.h"
#include "hmac_user.h"
#include "hmac_mgmt_ap.h"
#include "hmac_tx_data.h"
#include "hmac_wds.h"
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WDS_C

#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
/* 2 全局变量定义 */
/* 3 函数实现 */

OAL_STATIC uint32_t hmac_wds_find_node(hmac_vap_stru *pst_hmac_vap,
                                         uint8_t *puc_addr,
                                         hmac_wds_node_stru **ppst_wds_node)
{
    uint32_t ul_hash_value;
    hmac_wds_node_stru *pst_node = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;

    ul_hash_value = WDS_CALC_MAC_HASH_VAL(puc_addr);

    oal_dlist_search_for_each(pst_entry, &(pst_hmac_vap->st_wds_table.st_peer_node[ul_hash_value]))
    {
        pst_node = (hmac_wds_node_stru *)pst_entry;

        if (pst_node == OAL_PTR_NULL) {
            oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                           "{hmac_wds_find_node::pst_node null.sta idx %d}", ul_hash_value);
            continue;
        }

        /* 相同的MAC地址 */
        if (!oal_compare_mac_addr(pst_node->auc_mac, puc_addr)) {
            *ppst_wds_node = pst_node;
            return OAL_SUCC;
        }
    }

    return OAL_FAIL;
}


uint32_t hmac_wds_find_sta(hmac_vap_stru *pst_hmac_vap,
                             uint8_t *puc_addr,
                             hmac_wds_stas_stru **ppst_wds_sta)
{
    uint32_t ul_hash_value;
    hmac_wds_stas_stru *pst_sta = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;

    ul_hash_value = WDS_CALC_MAC_HASH_VAL(puc_addr);

    oal_dlist_search_for_each(pst_entry, &(pst_hmac_vap->st_wds_table.st_wds_stas[ul_hash_value]))
    {
        pst_sta = (hmac_wds_stas_stru *)pst_entry;

        if (pst_sta == OAL_PTR_NULL) {
            oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                           "{hmac_wds_find_sta::pst_sta null.sta idx %d}", ul_hash_value);
            continue;
        }

        /* 相同的MAC地址 */
        if (!oal_compare_mac_addr(pst_sta->auc_mac, puc_addr)) {
            *ppst_wds_sta = pst_sta;
            return OAL_SUCC;
        }
    }

    return OAL_FAIL;
}


uint32_t hmac_wds_del_sta_entry(hmac_vap_stru *pst_hmac_vap,
                                  uint8_t *puc_addr,
                                  hmac_wds_stas_stru *pst_sta)
{
    if ((pst_sta == OAL_PTR_NULL) && (hmac_wds_find_sta(pst_hmac_vap, puc_addr, &pst_sta) != OAL_SUCC)) {
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                         "{hmac_wds_del_sta_entry::hmac_wds_find_sta failed for mac[%02x:%02x:%02x:%02x]}",
                         puc_addr[2], puc_addr[3], puc_addr[4], puc_addr[5]);
        return OAL_SUCC;
    }

    if (pst_sta->pst_related_node != OAL_PTR_NULL) {
        pst_sta->pst_related_node->uc_stas_num--;
    }

    oal_dlist_delete_entry(&(pst_sta->st_entry));

    oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                     "{hmac_wds_del_sta_entry::del sta success for mac[%02x:%02x:%02x:%02x]}",
                     puc_addr[2], puc_addr[3], puc_addr[4], puc_addr[5]);

    oal_mem_free_m(pst_sta, OAL_TRUE);
    pst_hmac_vap->st_wds_table.uc_wds_stas_num--;

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_wds_add_sta_entry(hmac_vap_stru *pst_hmac_vap,
                                             uint8_t *puc_addr,
                                             hmac_wds_node_stru *pst_wds_node,
                                             hmac_wds_stas_stru **ppst_wds_sta)
{
    uint32_t ul_hash_value;
    hmac_wds_stas_stru *pst_sta = OAL_PTR_NULL;

    pst_sta = (hmac_wds_stas_stru *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, OAL_SIZEOF(hmac_wds_stas_stru), OAL_TRUE);

    if (oal_unlikely(pst_sta == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wds_add_sta_entry::oal_mem_alloc_m wds sta struct fail!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    memset_s(pst_sta, OAL_SIZEOF(hmac_wds_stas_stru), 0, OAL_SIZEOF(hmac_wds_stas_stru));
    oal_set_mac_addr(pst_sta->auc_mac, puc_addr);
    pst_sta->pst_related_node = pst_wds_node;
    pst_wds_node->uc_stas_num++;

    pst_sta->ul_last_pkt_age = OAL_TIME_JIFFY;

    ul_hash_value = WDS_CALC_MAC_HASH_VAL(puc_addr);

    oal_dlist_add_head(&(pst_sta->st_entry), &pst_hmac_vap->st_wds_table.st_wds_stas[ul_hash_value]);

    *ppst_wds_sta = pst_sta;
    pst_hmac_vap->st_wds_table.uc_wds_stas_num++;

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_wds_update_sta_entry(hmac_vap_stru *pst_hmac_vap,
                                                hmac_wds_node_stru *pst_new_wds_node,
                                                hmac_wds_stas_stru *pst_wds_sta)
{
    if (pst_wds_sta->pst_related_node != OAL_PTR_NULL) {
        pst_wds_sta->pst_related_node->uc_stas_num--;
    }

    pst_wds_sta->pst_related_node = pst_new_wds_node;
    pst_new_wds_node->uc_stas_num++;
    pst_wds_sta->ul_last_pkt_age = OAL_TIME_JIFFY;

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_wds_del_all_node_sta_entry(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_node_mac)
{
    uint8_t uc_hash_value = 0;
    uint32_t ul_ret = OAL_SUCC;
    hmac_wds_stas_stru *pst_sta = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_next_entry = OAL_PTR_NULL;

    for (uc_hash_value = 0; uc_hash_value < WDS_HASH_NUM; uc_hash_value++) {
        oal_dlist_search_for_each_safe(pst_entry, pst_next_entry,
            &(pst_hmac_vap->st_wds_table.st_wds_stas[uc_hash_value]))
        {
            pst_sta = (hmac_wds_stas_stru *)pst_entry;

            if (pst_sta == OAL_PTR_NULL) {
                oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                                 "{hmac_wds_del_all_node_sta_entry::pst_sta null.sta idx %d}",
                                 (uint32_t)uc_hash_value);
                ul_ret = OAL_FAIL;
                break;
            }

            /* 相同的MAC地址 */
            if (!oal_compare_mac_addr(pst_sta->pst_related_node->auc_mac, puc_node_mac) &&
                (OAL_SUCC != hmac_wds_del_sta_entry(pst_hmac_vap, pst_sta->auc_mac, pst_sta))) {
                oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                    "{hmac_wds_del_all_node_sta_entry::hmac_wds_del_sta_entry fail for wds node[%02x:%02x:%02x:%02x].}",
                    puc_node_mac[2], puc_node_mac[3], puc_node_mac[4], puc_node_mac[5]);

                ul_ret = OAL_FAIL;
                break;
            }
        }

        if (ul_ret != OAL_SUCC) {
            oam_error_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                "{hmac_wds_del_all_node_sta_entry::Del related wds sta fail for wds node[%02x:%02x:%02x:%02x].}",
                puc_node_mac[2], puc_node_mac[3], puc_node_mac[4], puc_node_mac[5]);
            break;
        }
    }

    return ul_ret;
}


OAL_STATIC uint32_t hmac_wds_del_node_entry(hmac_vap_stru *pst_hmac_vap,
                                              uint8_t *puc_addr,
                                              hmac_wds_node_stru *pst_node)
{
    if (OAL_SUCC != hmac_wds_del_all_node_sta_entry(pst_hmac_vap, puc_addr)) {
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_wds_del_node_entry::hmac_wds_del_all_node_sta_entry failed for mac[%02x:%02x:%02x:%02x]}",
            puc_addr[2], puc_addr[3], puc_addr[4], puc_addr[5]);
        return OAL_FAIL;
    }

    if ((pst_node == OAL_PTR_NULL) && (OAL_SUCC != hmac_wds_find_node(pst_hmac_vap, puc_addr, &pst_node))) {
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                         "{hmac_wds_del_node_entry::hmac_wds_find_node failed for mac[%02x:%02x:%02x:%02x]}",
                         puc_addr[2], puc_addr[3], puc_addr[4], puc_addr[5]);
        return OAL_SUCC;
    }

    oal_dlist_delete_entry(&(pst_node->st_entry));

    oam_info_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                  "{hmac_wds_del_node_entry::del node success for mac[%02x:%02x:%02x:%02x]}",
                  puc_addr[2], puc_addr[3], puc_addr[4], puc_addr[5]);

    oal_mem_free_m(pst_node, OAL_TRUE);
    pst_hmac_vap->st_wds_table.uc_wds_node_num--;

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_wds_add_node_entry(hmac_vap_stru *pst_hmac_vap,
                                              uint8_t *puc_addr,
                                              hmac_wds_node_stru **ppst_wds_node)
{
    uint32_t ul_hash_value;
    hmac_wds_node_stru *pst_node = OAL_PTR_NULL;

    pst_node = (hmac_wds_node_stru *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, OAL_SIZEOF(hmac_wds_node_stru), OAL_TRUE);

    if (oal_unlikely(pst_node == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wds_add_node_entry::oal_mem_alloc_m wds sta struct fail!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    memset_s(pst_node, OAL_SIZEOF(hmac_wds_node_stru), 0, OAL_SIZEOF(hmac_wds_node_stru));
    oal_set_mac_addr(pst_node->auc_mac, puc_addr);

    ul_hash_value = WDS_CALC_MAC_HASH_VAL(puc_addr);
    oal_dlist_add_head(&(pst_node->st_entry), &pst_hmac_vap->st_wds_table.st_peer_node[ul_hash_value]);

    *ppst_wds_node = pst_node;
    pst_hmac_vap->st_wds_table.uc_wds_node_num++;

    return OAL_SUCC;
}


uint32_t hmac_wds_add_node(hmac_vap_stru *pst_hmac_vap,
                             uint8_t *puc_node_mac)
{
    uint8_t ul_ret;
    hmac_wds_node_stru *pst_wds_node = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_hmac_vap, puc_node_mac))) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wds_add_node::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_rw_lock_read_lock(&pst_hmac_vap->st_wds_table.st_lock);
    if (pst_hmac_vap->st_wds_table.uc_wds_node_num >= WDS_MAX_NODE_NUM) {
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WDS,
            "{hmac_wds_add_node::Exceed max node %d, fail to add wds node[xx:xx:xx:%02x:%02x:%02x].}",
            (uint32_t)pst_hmac_vap->st_wds_table.uc_wds_node_num, puc_node_mac[3], puc_node_mac[4], puc_node_mac[5]);

        oal_rw_lock_read_unlock(&pst_hmac_vap->st_wds_table.st_lock);
        return OAL_FAIL;
    }

    if (OAL_SUCC == hmac_wds_find_node(pst_hmac_vap, puc_node_mac, &pst_wds_node)) {
        oam_info_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WDS,
                      "{hmac_wds_add_node::Same wds sta to the same wds node[%02x:%02x:%02x:%02x].}",
                      puc_node_mac[2], puc_node_mac[3], puc_node_mac[4], puc_node_mac[5]);

        oal_rw_lock_read_unlock(&pst_hmac_vap->st_wds_table.st_lock);
        return OAL_SUCC;
    }
    oal_rw_lock_read_unlock(&pst_hmac_vap->st_wds_table.st_lock);

    oal_rw_lock_write_lock(&pst_hmac_vap->st_wds_table.st_lock);
    ul_ret = hmac_wds_add_node_entry(pst_hmac_vap, puc_node_mac, &pst_wds_node);
    oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);

    oam_info_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WDS,
                  "{hmac_wds_add_node::ul_ret: %d to add wds node[xx:xx:xx:%02x:%02x:%02x].}",
                  ul_ret, puc_node_mac[3], puc_node_mac[4], puc_node_mac[5]);

    return ul_ret;
}


uint32_t hmac_wds_del_node(hmac_vap_stru *pst_hmac_vap,
                             uint8_t *puc_node_mac)
{
    uint32_t ul_ret = OAL_SUCC;
    hmac_wds_node_stru *pst_wds_node = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_hmac_vap, puc_node_mac))) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wds_del_node::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_rw_lock_write_lock(&pst_hmac_vap->st_wds_table.st_lock);
    if (OAL_SUCC != hmac_wds_find_node(pst_hmac_vap, puc_node_mac, &pst_wds_node)) {
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WDS,
                         "{hmac_wds_del_node::hmac_wds_find_node fail for wds node[%02x:%02x:%02x:%02x].}",
                         puc_node_mac[2], puc_node_mac[3], puc_node_mac[4], puc_node_mac[5]);
        oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);
        return OAL_FAIL;
    }

    if (OAL_SUCC != hmac_wds_del_node_entry(pst_hmac_vap, puc_node_mac, pst_wds_node)) {
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                         "{hmac_wds_del_node::hmac_wds_del_node_entry fail for mac[%02x:%02x:%02x:%02x]}",
                         puc_node_mac[2], puc_node_mac[3], puc_node_mac[4], puc_node_mac[5]);
        oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);
        return OAL_FAIL;
    }
    oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);

    oam_info_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WDS,
                  "{hmac_wds_del_node::ul_ret: %d to del wds node[xx:xx:xx:%02x:%02x:%02x].}",
                  ul_ret, puc_node_mac[3], puc_node_mac[4], puc_node_mac[5]);

    return ul_ret;
}


uint32_t hmac_wds_add_sta(hmac_vap_stru *pst_hmac_vap,
                            uint8_t *puc_node_mac,
                            uint8_t *puc_sta_mac)
{
    uint32_t ul_ret = OAL_SUCC;
    hmac_wds_stas_stru *pst_wds_sta = OAL_PTR_NULL;
    hmac_wds_node_stru *pst_wds_node = OAL_PTR_NULL;
    hmac_wds_node_stru *pst_old_wds_node = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr3(pst_hmac_vap, puc_node_mac, puc_sta_mac))) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wds_add_sta::param null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_rw_lock_read_lock(&pst_hmac_vap->st_wds_table.st_lock);
    if (pst_hmac_vap->st_wds_table.uc_wds_stas_num >= WDS_MAX_STAS_NUM) {
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WDS,
            "{hmac_wds_add_sta::Exceed the max wds sta number %d.Fail to add for mac [xx:xx:xx:%02x:%02x:%02x].}",
            pst_hmac_vap->st_wds_table.uc_wds_stas_num, puc_sta_mac[3], puc_sta_mac[4], puc_sta_mac[5]);

        oal_rw_lock_read_unlock(&pst_hmac_vap->st_wds_table.st_lock);
        return OAL_FAIL;
    }
    oal_rw_lock_read_unlock(&pst_hmac_vap->st_wds_table.st_lock);

    oal_rw_lock_write_lock(&pst_hmac_vap->st_wds_table.st_lock);
    if (hmac_wds_find_sta(pst_hmac_vap, puc_sta_mac, &pst_wds_sta) == OAL_SUCC) {
        if (!oal_compare_mac_addr(pst_wds_sta->pst_related_node->auc_mac, puc_node_mac)) {
            oam_info_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WDS,
                          "{hmac_wds_add_sta::Same wds sta to the same wds node[%02x:%02x:%02x:%02x].}",
                          puc_node_mac[2], puc_node_mac[3], puc_node_mac[4], puc_node_mac[5]);

            ul_ret = OAL_SUCC;
        } else if (OAL_SUCC == hmac_wds_find_node(pst_hmac_vap, puc_node_mac, &pst_wds_node)) {
            pst_old_wds_node = pst_wds_sta->pst_related_node;
            ul_ret = hmac_wds_update_sta_entry(pst_hmac_vap, pst_wds_node, pst_wds_sta);
            if ((ul_ret == OAL_SUCC) && (pst_old_wds_node != OAL_PTR_NULL) && (pst_old_wds_node->uc_stas_num == 0)) {
                hmac_wds_del_node_entry(pst_hmac_vap, pst_old_wds_node->auc_mac, pst_old_wds_node);
            }
        }
    } else {
        if (OAL_SUCC == hmac_wds_find_node(pst_hmac_vap, puc_node_mac, &pst_wds_node)) {
            ul_ret = hmac_wds_add_sta_entry(pst_hmac_vap, puc_sta_mac, pst_wds_node, &pst_wds_sta);
        } else {
            oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WDS,
                             "{hmac_wds_add_sta::No wds node for mac [%02x:%02x:%02x:%02x].}",
                             puc_node_mac[2], puc_node_mac[3], puc_node_mac[4], puc_node_mac[5]);
            ul_ret = OAL_FAIL;
        }
    }

    if (ul_ret == OAL_SUCC) {
        pst_wds_sta->ul_last_pkt_age = OAL_TIME_JIFFY;
    }
    oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);

    oam_info_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WDS,
                  "{hmac_wds_add_sta::ul_ret: %d to add wds sta[xx:xx:xx:%02x:%02x:%02x].}",
                  ul_ret, puc_sta_mac[3], puc_sta_mac[4], puc_sta_mac[5]);

    return ul_ret;
}


uint32_t hmac_wds_del_sta(hmac_vap_stru *pst_hmac_vap,
                            uint8_t *puc_addr)
{
    hmac_wds_stas_stru *pst_sta = OAL_PTR_NULL;
    hmac_wds_node_stru *pst_node = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_hmac_vap, puc_addr))) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wds_del_sta::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_rw_lock_write_lock(&pst_hmac_vap->st_wds_table.st_lock);

    if ((pst_sta == OAL_PTR_NULL) && (hmac_wds_find_sta(pst_hmac_vap, puc_addr, &pst_sta) != OAL_SUCC)) {
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                         "{hmac_wds_del_sta::hmac_wds_find_sta failed for mac[%02x:%02x:%02x:%02x]}",
                         puc_addr[2], puc_addr[3], puc_addr[4], puc_addr[5]);
        oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);
        return OAL_SUCC;
    }

    pst_node = pst_sta->pst_related_node;

    if (OAL_SUCC != hmac_wds_del_sta_entry(pst_hmac_vap, puc_addr, pst_sta)) {
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                         "{hmac_wds_del_sta::hmac_wds_del_sta_entry fail for mac[%02x:%02x:%02x:%02x]}",
                         puc_addr[2], puc_addr[3], puc_addr[4], puc_addr[5]);
        oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);
        return OAL_FAIL;
    }

    if ((pst_node != OAL_PTR_NULL) && (pst_node->uc_stas_num == 0)) {
        hmac_wds_del_node_entry(pst_hmac_vap, pst_node->auc_mac, pst_node);
    }

    oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);

    return OAL_SUCC;
}


uint32_t hmac_find_valid_user_by_wds_sta(hmac_vap_stru *pst_hmac_vap,
                                           uint8_t *puc_sta_mac_addr,
                                           uint16_t *pus_user_idx)
{
    uint32_t ul_ret = OAL_FAIL;
    uint32_t ul_now;
    hmac_wds_stas_stru *pst_wds_sta = OAL_PTR_NULL;
    hmac_wds_node_stru *pst_node = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr3(pst_hmac_vap, puc_sta_mac_addr, pus_user_idx))) {
        oam_error_log3(0, OAM_SF_ANY,
            "{hmac_find_valid_user_by_wds_sta::param null: pst_hmac_vap[%x]  puc_sta_mac_addr[%x]  pus_user_idx[%x]}",
            (uintptr_t)pst_hmac_vap, (uintptr_t)puc_sta_mac_addr, (uintptr_t)pus_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 放在指针空判断后 vap开启了Multi-STA本地能力位 */
    if (pst_hmac_vap->st_wds_table.en_wds_vap_mode != WDS_MODE_ROOTAP) {
        return ul_ret;
    }

    oal_rw_lock_write_lock(&pst_hmac_vap->st_wds_table.st_lock);
    if (hmac_wds_find_sta(pst_hmac_vap, puc_sta_mac_addr, &pst_wds_sta) != OAL_SUCC) {
        oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                      "{hmac_find_valid_user_by_wds_sta::hmac_wds_find_sta fail.}");
        oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);
        return ul_ret;
    }

    ul_now = OAL_TIME_JIFFY;
    if (oal_time_get_runtime(pst_wds_sta->ul_last_pkt_age, ul_now) > pst_hmac_vap->st_wds_table.ul_wds_aging) {
        pst_node = pst_wds_sta->pst_related_node;
        if ((OAL_SUCC == hmac_wds_del_sta_entry(pst_hmac_vap, puc_sta_mac_addr, pst_wds_sta)) &&
            (pst_node != OAL_PTR_NULL) && (pst_node->uc_stas_num == 0)) {
            hmac_wds_del_node_entry(pst_hmac_vap, pst_node->auc_mac, pst_node);
        }
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_find_valid_user_by_wds_sta::Wds sta rule timeout, to remove the sta [%02x:%02x:%02x:%02x]}",
            puc_sta_mac_addr[2], puc_sta_mac_addr[3], puc_sta_mac_addr[4], puc_sta_mac_addr[5]);
        oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);
        return ul_ret;
    }

    ul_ret = mac_vap_find_user_by_macaddr(&(pst_hmac_vap->st_vap_base_info),
                                              pst_wds_sta->pst_related_node->auc_mac, pus_user_idx);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_find_valid_user_by_wds_sta::mac_vap_find_user_by_macaddr failed. mac[%02x:%02x:%02x:%02x]}",
            puc_sta_mac_addr[2], puc_sta_mac_addr[3], puc_sta_mac_addr[4], puc_sta_mac_addr[5]);
    }
    oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);

    return ul_ret;
}


uint32_t hmac_wds_update_table(hmac_vap_stru *pst_hmac_vap,
                                 uint8_t *puc_node_mac,
                                 uint8_t *puc_sta_mac,
                                 uint8_t uc_update_mode)
{
    uint32_t ul_ret = OAL_SUCC;

    if (uc_update_mode == WDS_TABLE_ADD_ENTRY) {
        hmac_wds_add_node(pst_hmac_vap, puc_node_mac);
        hmac_wds_add_sta(pst_hmac_vap, puc_node_mac, puc_sta_mac);
    } else if (uc_update_mode == WDS_TABLE_DEL_ENTRY) {
        hmac_wds_del_sta(pst_hmac_vap, puc_sta_mac);
    } else {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WDS,
                         "{hmac_wds_update_table::Not supported update mode: %d.}", uc_update_mode);
        ul_ret = OAL_FAIL;
    }

    return ul_ret;
}


uint32_t hmac_wds_reset_sta_mapping_table(hmac_vap_stru *pst_hmac_vap)
{
    uint8_t uc_hash_value = 0;
    uint32_t ul_ret = OAL_SUCC;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_next_entry = OAL_PTR_NULL;
    hmac_wds_node_stru *pst_node = OAL_PTR_NULL;

    if ((pst_hmac_vap == OAL_PTR_NULL) || (pst_hmac_vap->st_wds_table.en_wds_vap_mode != WDS_MODE_ROOTAP)) {
        return OAL_SUCC;
    }

    oal_rw_lock_write_lock(&pst_hmac_vap->st_wds_table.st_lock);
    for (uc_hash_value = 0; uc_hash_value < WDS_HASH_NUM; uc_hash_value++) {
        oam_info_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                      "{hmac_wds_reset_sta_mapping_table::hash idx %d}",
                      (uint32_t)uc_hash_value);
        oal_dlist_search_for_each_safe(pst_entry, pst_next_entry,
            &(pst_hmac_vap->st_wds_table.st_peer_node[uc_hash_value]))
        {
            pst_node = (hmac_wds_node_stru *)pst_entry;
            if (pst_node == OAL_PTR_NULL) {
                oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                                 "{hmac_wds_reset_sta_mapping_table::pst_sta null.hash idx %d}",
                                 (uint32_t)uc_hash_value);
                ul_ret = OAL_ERR_CODE_PTR_NULL;
                break;
            }

            if (OAL_SUCC != hmac_wds_del_node_entry(pst_hmac_vap, pst_node->auc_mac, pst_node)) {
                oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                                 "{hmac_wds_reset_sta_mapping_table::hmac_wds_del_node fail for hash idx %d}",
                                 (uint32_t)uc_hash_value);
                ul_ret = OAL_FAIL;
                break;
            }
        }
    }
    oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);

    oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY, "{hmac_wds_reset_sta_mapping_table::Leave.}");

    return ul_ret;
}


OAL_STATIC uint32_t hmac_wds_table_timerout_handler(void *p_arg)
{
    uint8_t uc_hash_value = 0;
    uint32_t ul_ret = OAL_SUCC;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_next_entry = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_wds_node_stru *pst_node = OAL_PTR_NULL;
    hmac_wds_stas_stru *pst_wds_sta = OAL_PTR_NULL;

    pst_hmac_vap = (hmac_vap_stru *)p_arg;

    if ((pst_hmac_vap == OAL_PTR_NULL) || (pst_hmac_vap->st_wds_table.en_wds_vap_mode != WDS_MODE_ROOTAP)) {
        return OAL_SUCC;
    }

    if (pst_hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_UP) {
        return OAL_SUCC;
    }

    oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY, "{hmac_wds_table_timerout_handler::in}");

    oal_rw_lock_write_lock(&pst_hmac_vap->st_wds_table.st_lock);
    for (uc_hash_value = 0; uc_hash_value < WDS_HASH_NUM; uc_hash_value++) {
        oal_dlist_search_for_each_safe(pst_entry, pst_next_entry,
            &(pst_hmac_vap->st_wds_table.st_wds_stas[uc_hash_value]))
        {
            pst_wds_sta = (hmac_wds_stas_stru *)pst_entry;
            if (pst_wds_sta == OAL_PTR_NULL) {
                oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                                 "{hmac_wds_table_timerout_handler::pst_sta null.hash idx %d}",
                                 (uint32_t)uc_hash_value);
                ul_ret = OAL_ERR_CODE_PTR_NULL;
                break;
            }

            if (oal_time_get_runtime(pst_wds_sta->ul_last_pkt_age, OAL_TIME_JIFFY) >
                pst_hmac_vap->st_wds_table.ul_wds_aging) {
                pst_node = pst_wds_sta->pst_related_node;
                if ((OAL_SUCC == hmac_wds_del_sta_entry(pst_hmac_vap, pst_wds_sta->auc_mac, pst_wds_sta)) &&
                    (pst_node != OAL_PTR_NULL) && (pst_node->uc_stas_num == 0)) {
                    hmac_wds_del_node_entry(pst_hmac_vap, pst_node->auc_mac, pst_node);
                }
            }
        }
    }
    oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);

    return ul_ret;
}


uint32_t hmac_wds_node_ergodic(hmac_vap_stru *pst_hmac_vap,
                                 uint8_t *src_addr,
                                 p_hmac_wds_node_func pst_hmac_wds_node,
                                 void *pst_arg)
{
    uint32_t find_sta_ret;
    hmac_wds_stas_stru *pst_wds_sta = OAL_PTR_NULL;
    uint32_t ul_hash_value = 0;
    hmac_wds_node_stru *pst_node = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;

    if (oal_any_null_ptr2(pst_hmac_vap, pst_hmac_wds_node)) {
        return OAL_FAIL;
    }

    oal_rw_lock_read_lock(&pst_hmac_vap->st_wds_table.st_lock);

    if (pst_hmac_vap->st_wds_table.uc_wds_node_num == 0) {
        oal_rw_lock_read_unlock(&pst_hmac_vap->st_wds_table.st_lock);
        return OAL_SUCC;
    }

    /* 查找报文源MAC是否是Root AP WDS Table里的STA */
    find_sta_ret = hmac_wds_find_sta(pst_hmac_vap, src_addr, &pst_wds_sta);

    for (ul_hash_value = 0; ul_hash_value < WDS_HASH_NUM; ul_hash_value++) {
        oal_dlist_search_for_each(pst_entry, &(pst_hmac_vap->st_wds_table.st_peer_node[ul_hash_value]))
        {
            pst_node = (hmac_wds_node_stru *)pst_entry;

            if (pst_node == OAL_PTR_NULL) {
                oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                               "{hmac_wds_node_ergodic::pst_node null.node idx %d}", ul_hash_value);
                continue;
            }

            /* 如果检测到广播报文源地址是对应repeater ap下的sta 不应该往这个repeater发送广播报文 */
            if ((find_sta_ret == OAL_SUCC) && (pst_wds_sta != OAL_PTR_NULL) &&
                (pst_wds_sta->pst_related_node != OAL_PTR_NULL) &&
                !oal_compare_mac_addr(pst_node->auc_mac, pst_wds_sta->pst_related_node->auc_mac)) {
                oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                                 "{hmac_wds_node_ergodic::repeater ap broadcast pkt, cannot send back!}");
                continue;
            }

            /* 执行函数 */
            (*pst_hmac_wds_node)(pst_hmac_vap, pst_node->auc_mac, pst_arg);
        }
    }

    oal_rw_lock_read_unlock(&pst_hmac_vap->st_wds_table.st_lock);

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_wds_find_neigh(hmac_vap_stru *pst_hmac_vap,
                                          uint8_t *puc_addr,
                                          hmac_wds_neigh_stru **ppst_neigh)
{
    uint32_t ul_hash_value;
    hmac_wds_neigh_stru *pst_neigh = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_hmac_vap, puc_addr))) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wds_find_neigh::param null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_hash_value = WDS_CALC_MAC_HASH_VAL(puc_addr);

    oal_dlist_search_for_each(pst_entry, &(pst_hmac_vap->st_wds_table.st_neigh[ul_hash_value]))
    {
        pst_neigh = (hmac_wds_neigh_stru *)pst_entry;

        if (pst_neigh == OAL_PTR_NULL) {
            oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                           "{hmac_wds_find_neigh::pst_sta null.sta idx %d}", ul_hash_value);
            continue;
        }

        /* 相同的MAC地址 */
        if (!oal_compare_mac_addr(pst_neigh->auc_mac, puc_addr)) {
            *ppst_neigh = pst_neigh;
            return OAL_SUCC;
        }
    }

    return OAL_FAIL;
}


uint32_t hmac_wds_update_neigh(hmac_vap_stru *pst_hmac_vap,
                                 uint8_t *puc_mac)
{
    uint32_t ul_hash_value;
    hmac_wds_neigh_stru *pst_neigh = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_hmac_vap, puc_mac))) {
        oam_error_log0(0, OAM_SF_WDS, "{hmac_wds_update_neigh::param null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_rw_lock_write_lock(&pst_hmac_vap->st_wds_table.st_lock);
    /* Find related mac already exist */
    if (OAL_SUCC == hmac_wds_find_neigh(pst_hmac_vap, puc_mac, &pst_neigh)) {
        pst_neigh->ul_last_pkt_age = OAL_TIME_JIFFY;
        oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);
        return OAL_SUCC;
    }

    if (pst_hmac_vap->st_wds_table.uc_neigh_num >= WDS_MAX_NEIGH_NUM) {
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WDS,
            "{hmac_wds_update_neigh::Exceed max wds sa from br number %d.Fail add for mac [xx:xx:xx:%02x:%02x:%02x].}",
            pst_hmac_vap->st_wds_table.uc_neigh_num, puc_mac[3], puc_mac[4], puc_mac[5]);
        oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);
        return OAL_FAIL;
    }

    /* Add to hash table if not exist */
    pst_neigh = (hmac_wds_neigh_stru *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, OAL_SIZEOF(hmac_wds_neigh_stru), OAL_TRUE);

    if (oal_unlikely(pst_neigh == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wds_update_neigh::oal_mem_alloc_m wds sa from br struct fail!}");
        oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);
        return OAL_ERR_CODE_PTR_NULL;
    }

    memset_s(pst_neigh, OAL_SIZEOF(hmac_wds_neigh_stru), 0, OAL_SIZEOF(hmac_wds_neigh_stru));
    oal_set_mac_addr(pst_neigh->auc_mac, puc_mac);
    pst_neigh->ul_last_pkt_age = OAL_TIME_JIFFY;
    ul_hash_value = WDS_CALC_MAC_HASH_VAL(puc_mac);
    oal_dlist_add_head(&(pst_neigh->st_entry), &pst_hmac_vap->st_wds_table.st_neigh[ul_hash_value]);

    pst_hmac_vap->st_wds_table.uc_neigh_num++;
    oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);

    return OAL_SUCC;
}


uint32_t hmac_wds_neigh_not_expired(hmac_vap_stru *pst_hmac_vap,
                                      uint8_t *puc_mac)
{
    uint32_t ul_ret;
    hmac_wds_neigh_stru *pst_neigh = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_hmac_vap, puc_mac))) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wds_neigh_not_expired::param null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_rw_lock_write_lock(&pst_hmac_vap->st_wds_table.st_lock);
    /* Find related mac already exist */
    ul_ret = hmac_wds_find_neigh(pst_hmac_vap, puc_mac, &pst_neigh);

    /* Delete the entry if expired */
    if ((ul_ret == OAL_SUCC) &&
        (oal_time_get_runtime(pst_neigh->ul_last_pkt_age, OAL_TIME_JIFFY) > pst_hmac_vap->st_wds_table.ul_wds_aging)) {
        ul_ret = OAL_FAIL;
        oal_dlist_delete_entry(&(pst_neigh->st_entry));
        oal_mem_free_m(pst_neigh, OAL_TRUE);
        pst_hmac_vap->st_wds_table.uc_neigh_num--;
        oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                         "{hmac_wds_neigh_not_expired::Expired for br neigh[xx:xx:%02x:%02x:%02x:%02x].}",
                         pst_neigh->auc_mac[2], pst_neigh->auc_mac[3], pst_neigh->auc_mac[4], pst_neigh->auc_mac[5]);
    }
    oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);

    return ul_ret;
}


uint32_t hmac_wds_reset_neigh_table(hmac_vap_stru *pst_hmac_vap)
{
    uint8_t uc_hash_value = 0;
    uint32_t ul_ret = OAL_SUCC;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_next_entry = OAL_PTR_NULL;
    hmac_wds_neigh_stru *pst_neigh = OAL_PTR_NULL;
    uint8_t *puc_addr;

    if ((pst_hmac_vap == OAL_PTR_NULL) || (pst_hmac_vap->st_wds_table.en_wds_vap_mode != WDS_MODE_REPEATER_STA)) {
        return OAL_SUCC;
    }

    oal_rw_lock_write_lock(&pst_hmac_vap->st_wds_table.st_lock);
    for (uc_hash_value = 0; uc_hash_value < WDS_HASH_NUM; uc_hash_value++) {
        oam_info_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                      "{hmac_wds_reset_neigh_table::hash idx %d}",
                      (uint32_t)uc_hash_value);
        oal_dlist_search_for_each_safe(pst_entry, pst_next_entry, &(pst_hmac_vap->st_wds_table.st_neigh[uc_hash_value]))
        {
            pst_neigh = (hmac_wds_neigh_stru *)pst_entry;
            if (pst_neigh == OAL_PTR_NULL) {
                oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                                 "{hmac_wds_reset_neigh_table::pst_sta null.hash idx %d}",
                                 (uint32_t)uc_hash_value);
                ul_ret = OAL_FAIL;
                break;
            }

            oal_dlist_delete_entry(&(pst_neigh->st_entry));
            puc_addr = pst_neigh->auc_mac;

            oam_info_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                          "{hmac_wds_reset_neigh_table::del sa from bridge success for mac[%02x:%02x:%02x:%02x]}",
                          puc_addr[2], puc_addr[3], puc_addr[4], puc_addr[5]);

            oal_mem_free_m(pst_neigh, OAL_TRUE);
            pst_hmac_vap->st_wds_table.uc_neigh_num--;
        }
    }

    if (pst_hmac_vap->st_wds_table.uc_neigh_num != 0) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_wds_reset_neigh_table::pst_vap->st_wds_table.uc_neigh_num[%d] != 0.}",
                       pst_hmac_vap->st_wds_table.uc_neigh_num);

        pst_hmac_vap->st_wds_table.uc_neigh_num = 0;
    }
    oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);

    return ul_ret;
}


OAL_STATIC uint32_t hmac_wds_neigh_timeout_handler(void *p_arg)
{
    uint8_t uc_hash_value = 0;
    uint32_t ul_ret = OAL_SUCC;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_next_entry = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_wds_neigh_stru *pst_neigh = OAL_PTR_NULL;
    uint8_t *puc_addr;

    pst_hmac_vap = (hmac_vap_stru *)p_arg;

    if ((pst_hmac_vap == OAL_PTR_NULL) || (pst_hmac_vap->st_wds_table.en_wds_vap_mode != WDS_MODE_REPEATER_STA)) {
        return OAL_SUCC;
    }

    if (pst_hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_UP) {
        return OAL_SUCC;
    }

    oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY, "{hmac_wds_neigh_timeout_handler::in}");

    oal_rw_lock_write_lock(&pst_hmac_vap->st_wds_table.st_lock);
    for (uc_hash_value = 0; uc_hash_value < WDS_HASH_NUM; uc_hash_value++) {
        oal_dlist_search_for_each_safe(pst_entry, pst_next_entry, &(pst_hmac_vap->st_wds_table.st_neigh[uc_hash_value]))
        {
            pst_neigh = (hmac_wds_neigh_stru *)pst_entry;
            if (pst_neigh == OAL_PTR_NULL) {
                oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                                 "{hmac_wds_neigh_timeout_handler::pst_sta null.hash idx %d}",
                                 (uint32_t)uc_hash_value);
                ul_ret = OAL_FAIL;
                break;
            }

            if (oal_time_get_runtime(pst_neigh->ul_last_pkt_age, OAL_TIME_JIFFY) >
                pst_hmac_vap->st_wds_table.ul_wds_aging) {
                oal_dlist_delete_entry(&(pst_neigh->st_entry));
                puc_addr = pst_neigh->auc_mac;

                oam_info_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                    "{hmac_wds_neigh_timeout_handler::del sa from bridge success for mac[%02x:%02x:%02x:%02x]}",
                    puc_addr[2], puc_addr[3], puc_addr[4], puc_addr[5]);

                oal_mem_free_m(pst_neigh, OAL_TRUE);
                pst_hmac_vap->st_wds_table.uc_neigh_num--;
            }
        }
    }

    oal_rw_lock_write_unlock(&pst_hmac_vap->st_wds_table.st_lock);

    return ul_ret;
}


uint32_t hmac_wds_table_create_timer(hmac_vap_stru *pst_hmac_vap)
{
    if ((pst_hmac_vap == OAL_PTR_NULL) || (pst_hmac_vap->st_wds_table.en_wds_vap_mode == WDS_MODE_NONE)) {
        return OAL_SUCC;
    }

    /* 删除WDS定时器 */
    if (pst_hmac_vap->st_wds_table.st_wds_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_destroy_timer(&(pst_hmac_vap->st_wds_table.st_wds_timer));
    }

    if (pst_hmac_vap->st_wds_table.en_wds_vap_mode == WDS_MODE_ROOTAP) {
        frw_timer_create_timer_m(&(pst_hmac_vap->st_wds_table.st_wds_timer),
                               hmac_wds_table_timerout_handler,
                               WDS_TABLE_DEF_TIMER,
                               (void *)pst_hmac_vap,
                               OAL_TRUE,
                               OAM_MODULE_ID_HMAC,
                               pst_hmac_vap->st_vap_base_info.ul_core_id);
    } else if (pst_hmac_vap->st_wds_table.en_wds_vap_mode == WDS_MODE_REPEATER_STA) {
        frw_timer_create_timer_m(&(pst_hmac_vap->st_wds_table.st_wds_timer),
                               hmac_wds_neigh_timeout_handler,
                               WDS_TABLE_DEF_TIMER,
                               (void *)pst_hmac_vap,
                               OAL_TRUE,
                               OAM_MODULE_ID_HMAC,
                               pst_hmac_vap->st_vap_base_info.ul_core_id);
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA

/* 4地址ie check接口 用于驱动上报帧给产品,产品判断后输出用户是否支持4地址 */
typedef int32_t (*p_vmsta_a4_check_func)(uint8_t *puc_data, uint32_t ul_len);

OAL_STATIC p_vmsta_a4_check_func g_p_vmsta_user_a4_support = OAL_PTR_NULL;
OAL_STATIC p_vmsta_a4_check_func g_p_vmsta_vap_a4_support = OAL_PTR_NULL;

void hmac_vmsta_user_check_reg(p_vmsta_a4_check_func p_func)
{
    g_p_vmsta_user_a4_support = p_func;
}

void hmac_vmsta_vap_check_reg(p_vmsta_a4_check_func p_func)
{
    g_p_vmsta_vap_a4_support = p_func;
}

/*lint -e578*/ /*lint -e19*/
oal_module_symbol(hmac_vmsta_user_check_reg);
oal_module_symbol(hmac_vmsta_vap_check_reg);
/*lint +e578*/ /*lint +e19*/

oal_bool_enum_uint8 hmac_vmsta_get_user_a4_support(hmac_vap_stru *pst_hmac_vap,
                                                   uint8_t *pst_addr)
{
    uint16_t us_user_idx = 0xffff;
    uint32_t ul_ret;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;

    ul_ret = mac_vap_find_user_by_macaddr(&(pst_hmac_vap->st_vap_base_info), pst_addr, &us_user_idx);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_error_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_vmsta_get_user_a4_support::mac_vap_get_user_by_addr failed. mac[XX%02x:%02x:%02x%02x]}",
            pst_addr[2], pst_addr[3], pst_addr[4], pst_addr[5]);
        return OAL_FALSE;
    }

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_idx);
    /* 复用wds用户标识，如果能力交互是用户支持4地址通信，则会设置该标识为TRUE */
    return pst_hmac_user->uc_is_wds;
}


uint32_t hmac_vmsta_set_vap_a4_enable(mac_vap_stru *pst_mac_vap)
{
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                       "{hmac_vmsta_set_vap_a4_enable::mac_res_get_hmac_vap null pointer}");
        return OAL_FAIL;
    }
    /* 复用wds VAP标识，如果上层传入4地址IE，则会设置VAP为对应的wds模式 */
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        pst_hmac_vap->st_wds_table.en_wds_vap_mode = WDS_MODE_ROOTAP;
    } else if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        pst_hmac_vap->st_wds_table.en_wds_vap_mode = WDS_MODE_REPEATER_STA;
    } else {
        pst_hmac_vap->st_wds_table.en_wds_vap_mode = WDS_MODE_NONE;
    }
    return OAL_SUCC;
}


oal_bool_enum_uint8 hmac_vmsta_check_vap_a4_support(uint8_t *puc_ie, uint32_t ul_ie_len)
{
    /* 产品已注册接口则调用接口进行a4 check */
    if (g_p_vmsta_vap_a4_support != OAL_PTR_NULL) {
        if (VMSTA_4ADDR_SUPPORT == g_p_vmsta_vap_a4_support(puc_ie, ul_ie_len)) {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}


oal_bool_enum_uint8 hmac_vmsta_check_user_a4_support(uint8_t *puc_frame, uint32_t ul_frame_len)
{
    /* 产品已注册接口则调用接口进行a4 check */
    if (g_p_vmsta_user_a4_support != OAL_PTR_NULL) {
        if (VMSTA_4ADDR_SUPPORT == g_p_vmsta_user_a4_support(puc_frame, ul_frame_len)) {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}

#endif  // _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA


void hmac_wds_init_table(hmac_vap_stru *pst_hmac_vap)
{
    uint32_t ul_loop = 0;

    oal_rw_lock_init(&pst_hmac_vap->st_wds_table.st_lock);

    pst_hmac_vap->st_wds_table.en_wds_vap_mode = WDS_MODE_NONE;
    pst_hmac_vap->st_wds_table.uc_wds_node_num = 0;
    pst_hmac_vap->st_wds_table.uc_wds_stas_num = 0;
    pst_hmac_vap->st_wds_table.uc_neigh_num = 0;
    pst_hmac_vap->st_wds_table.ul_wds_aging = WDS_AGING_TIME;

    for (ul_loop = 0; ul_loop < WDS_HASH_NUM; ul_loop++) {
        oal_dlist_init_head(&(pst_hmac_vap->st_wds_table.st_peer_node[ul_loop]));
        oal_dlist_init_head(&(pst_hmac_vap->st_wds_table.st_wds_stas[ul_loop]));
        oal_dlist_init_head(&(pst_hmac_vap->st_wds_table.st_neigh[ul_loop]));
    }
}


OAL_STATIC uint32_t hmac_wds_tx_event(hmac_vap_stru *pst_vap, hmac_user_stru *pst_user, oal_netbuf_stru *pst_buf)
{
    frw_event_stru *pst_event; /* 事件结构体 */
    frw_event_mem_stru *pst_event_mem;
    dmac_tx_event_stru *pst_dtx_stru;
    uint32_t ul_ret;

    /* 封装802.11头 */
    ul_ret = hmac_tx_encap(pst_vap, pst_user, pst_buf);
    if (oal_unlikely((ul_ret != OAL_SUCC))) {
        oam_warning_log1(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                         "{hmac_wds_tx_event::hmac_tx_encap failed[%d].}", ul_ret);
        oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, tx_abnormal_msdu_dropped, 1);
        return ul_ret;
    }

    /* 抛事件，传给DMAC */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_tx_event_stru));
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_warning_log0(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{hmac_wds_tx_event::pst_event_mem null.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_DRX,
                       DMAC_TX_HOST_DRX,
                       OAL_SIZEOF(dmac_tx_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_vap->st_vap_base_info.uc_chip_id,
                       pst_vap->st_vap_base_info.uc_device_id,
                       pst_vap->st_vap_base_info.uc_vap_id);

    pst_dtx_stru = (dmac_tx_event_stru *)pst_event->auc_event_data;
    pst_dtx_stru->pst_netbuf = pst_buf;

    /* 调度事件 */
    ul_ret = frw_event_dispatch_event(pst_event_mem);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                         "{hmac_wds_tx_event::frw_event_dispatch_event failed[%d].}", ul_ret);
        oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, tx_abnormal_msdu_dropped, 1);
    }

    /* 释放事件 */
    frw_event_free_m(pst_event_mem);

    return ul_ret;
}


uint32_t hmac_wds_tx_broadcast_pkt(hmac_vap_stru *pst_vap, uint8_t *puc_addr, void *pst_arg)
{
    oal_netbuf_stru *pst_buf = (oal_netbuf_stru *)pst_arg;
    oal_netbuf_stru *pst_copy_buf;
    hmac_user_stru *pst_user; /* 目标STA结构体 */
    uint32_t ul_ret;
    uint16_t us_user_idx = g_wlan_spec_cfg->invalid_user_id;

    ul_ret = mac_vap_find_user_by_macaddr(&(pst_vap->st_vap_base_info), puc_addr, &us_user_idx);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        return OAL_FALSE;
    }

    pst_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_idx);
    if (oal_unlikely(pst_user == OAL_PTR_NULL)) {
        return OAL_FALSE;
    }

    pst_copy_buf = oal_netbuf_copy(pst_buf, GFP_ATOMIC);
    ul_ret = hmac_wds_tx_event(pst_vap, pst_user, pst_copy_buf);
    if (oal_unlikely((ul_ret != OAL_SUCC))) {
        if (pst_copy_buf != OAL_PTR_NULL) {
            oal_netbuf_free(pst_copy_buf);
        }
        return OAL_FALSE;
    }

    oam_warning_log4(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                     "{hmac_wds_tx_broadcast_pkt::send broadcast pkt to mac[%02x:%02x:%02x:%02x]}",
                     puc_addr[2], puc_addr[3], puc_addr[4], puc_addr[5]);

    return OAL_TRUE;
}

oal_module_symbol(hmac_wds_update_table);
oal_module_symbol(hmac_wds_add_node);
oal_module_symbol(hmac_wds_del_node);
oal_module_symbol(hmac_wds_reset_sta_mapping_table);
oal_module_symbol(hmac_wds_add_sta);
oal_module_symbol(hmac_wds_del_sta);
oal_module_symbol(hmac_find_valid_user_by_wds_sta);
oal_module_symbol(hmac_wds_node_ergodic);
oal_module_symbol(hmac_wds_update_neigh);
oal_module_symbol(hmac_wds_neigh_not_expired);
oal_module_symbol(hmac_wds_reset_neigh_table);
oal_module_symbol(hmac_wds_table_create_timer);
oal_module_symbol(hmac_wds_init_table);
oal_module_symbol(hmac_wds_find_sta);
oal_module_symbol(hmac_wds_tx_broadcast_pkt);

#endif /* defined (_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA) */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

