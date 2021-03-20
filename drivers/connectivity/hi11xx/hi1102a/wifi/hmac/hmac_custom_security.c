

#ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY

/* 1 ͷ�ļ����� */
#include "hmac_main.h"
#include "hmac_custom_security.h"
#include "mac_vap.h"
#include "hmac_vap.h"
#include "mac_resource.h"
#include "hmac_user.h"
#include "hmac_mgmt_ap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_CUSTOM_SECURITY_C

/* 2 STRUCT���� */
#define BROADCAST_ISOLATION(_a) ((_a)&CS_ISOLATION_MODE_BROADCAST)
#define MULTICAST_ISOLATION(_a) ((_a)&CS_ISOLATION_MODE_MULTICAST)
#define UNICAST_ISOLATION(_a)   ((_a)&CS_ISOLATION_MODE_UNICAST)

/* 3 ȫ�ֱ������� */
/* 4 ����ʵ�� */

OAL_STATIC oal_uint32 hmac_blacklist_mac_is_zero(oal_uint8 *puc_mac_addr)
{
    if ((puc_mac_addr[0] == 0) &&
        (puc_mac_addr[1] == 0) &&
        (puc_mac_addr[2] == 0) && /* �ж�puc_mac_addr��2byte�Ƿ�Ϊ�� */
        (puc_mac_addr[3] == 0) && /* �ж�puc_mac_addr��3byte�Ƿ�Ϊ�� */
        (puc_mac_addr[4] == 0) && /* �ж�puc_mac_addr��4byte�Ƿ�Ϊ�� */
        (puc_mac_addr[5] == 0)) { /* �ж�puc_mac_addr��5byte�Ƿ�Ϊ�� */
        return 1;
    }

    return 0;
}


OAL_STATIC oal_uint32 hmac_blacklist_mac_is_bcast(oal_uint8 *puc_mac_addr)
{
    if ((puc_mac_addr[0] == 0xff) &&
        (puc_mac_addr[1] == 0xff) &&
        (puc_mac_addr[2] == 0xff) && /* �ж�puc_mac_addr��2byte�Ƿ�㲥��ַΪȫ0xff */
        (puc_mac_addr[3] == 0xff) && /* �ж�puc_mac_addr��3byte�Ƿ�㲥��ַΪȫ0xff */
        (puc_mac_addr[4] == 0xff) && /* �ж�puc_mac_addr��4byte�Ƿ�㲥��ַΪȫ0xff */
        (puc_mac_addr[5] == 0xff)) { /* �ж�puc_mac_addr��5byte�Ƿ�㲥��ַΪȫ0xff */
        return 1;
    }

    return 0;
}


OAL_STATIC oal_void hmac_blacklist_init(mac_vap_stru *pst_mac_vap, cs_blacklist_mode_enum_uint8 en_mode)
{
    oal_uint32               ul_size;
    mac_blacklist_info_stru *pst_blacklist_info;
    hmac_vap_stru           *pst_hmac_vap;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_init::pst_hmac_vap null.}");
        return;
    }

    pst_blacklist_info = &pst_hmac_vap->st_blacklist_info;
    ul_size = sizeof(mac_blacklist_info_stru);
    /* Max=32 => ������mac_vap�ṹ��С= 0x494 = 1172 ; Max=8 => size = 308 */
    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_blacklist_init stru_size=%d}", ul_size);
    memset_s(pst_blacklist_info, ul_size, 0, ul_size);

    pst_blacklist_info->uc_mode = en_mode;
}


OAL_STATIC oal_bool_enum_uint8 hmac_blacklist_is_aged(mac_vap_stru *pst_mac_vap, mac_blacklist_stru *pst_blacklist)
{
    oal_time_us_stru             st_cur_time;
    mac_blacklist_info_stru     *pst_blacklist_info;
    oal_uint8                   *puc_mac_addr;
    hmac_vap_stru               *pst_hmac_vap;

    /* 1.1 ȫ���ַ�����ڷǷ���ַ */
    if (hmac_blacklist_mac_is_zero(pst_blacklist->auc_mac_addr)) {
        return OAL_FALSE;
    }

    /* 2.1 �ϻ�ʱ��Ϊ0��ʾ����Ҫ�ϻ� */
    if (pst_blacklist->ul_aging_time == 0) {
        return OAL_FALSE;
    }

    /* 2.2 û�г����ϻ�ʱ�� */
    oal_time_get_stamp_us(&st_cur_time);
    if (st_cur_time.i_sec < (oal_int)(pst_blacklist->ul_cfg_time + pst_blacklist->ul_aging_time)) {
        return OAL_FALSE;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_is_aged::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    puc_mac_addr = pst_blacklist->auc_mac_addr;
    OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{aging time reach delete MAC:=%02X:XX:XX:%02X:%02X:%02X}",
        /* puc_mac_addr��0��3��4��5byteΪ���������ӡ���˴���ӡmac��ַ��һλ�ͺ���λ */
        puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);

    /* 3.1 ֱ�ӴӺ�������ɾ�� */
    pst_blacklist_info = &pst_hmac_vap->st_blacklist_info;
    if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_BLACK) {
        memset_s(pst_blacklist, OAL_SIZEOF(mac_blacklist_stru), 0, OAL_SIZEOF(mac_blacklist_stru));
        if (pst_blacklist_info->uc_list_num > 0) {
            pst_blacklist_info->uc_list_num--; /* 2014.7.23 bug fixed */
        }
        return OAL_TRUE;
    }

    /* 4.1 ֱ�ӴӰ������лָ� */
    if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_WHITE) {
        pst_blacklist->ul_aging_time = 0;
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


OAL_STATIC oal_uint32 hmac_blacklist_get(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr,
                                         mac_blacklist_stru **ppst_blacklist)
{
    oal_uint8                    ul_blacklist_index;
    mac_blacklist_info_stru     *pst_blacklist_info = OAL_PTR_NULL;
    hmac_vap_stru               *pst_hmac_vap;

    /* 1.1 �㲥��ַ�����ڷǷ���ַ */
    if (hmac_blacklist_mac_is_bcast(puc_mac_addr)) {
        return OAL_ERR_CODE_SECURITY_MAC_INVALID;
    }

    /* 1.2 ȫ���ַ�����ڷǷ���ַ */
    if (hmac_blacklist_mac_is_zero(puc_mac_addr)) {
        return OAL_ERR_CODE_SECURITY_MAC_INVALID;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_init::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_blacklist_info = &pst_hmac_vap->st_blacklist_info;

    /* 2.1 �ҵ��� */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        *ppst_blacklist = &pst_blacklist_info->ast_black_list[ul_blacklist_index];
        if (!oal_memcmp((*ppst_blacklist)->auc_mac_addr, puc_mac_addr, OAL_MAC_ADDR_LEN)) {
            break;
        }
        *ppst_blacklist = OAL_PTR_NULL;
    }

    return OAL_SUCC;
}


oal_uint32 hmac_backlist_get_drop(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr)
{
    /* 2014.9.2 Add UT and found no init value set to it ! add initial value null */
    mac_blacklist_stru *pst_blacklist = OAL_PTR_NULL;
    oal_uint32 ul_ret;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_backlist_get_drop::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_blacklist_get(pst_mac_vap, puc_mac_addr, &pst_blacklist);
    if (pst_blacklist != OAL_PTR_NULL) {
        return pst_blacklist->ul_drop_counter;
    }

    return OAL_FAIL;
}


OAL_STATIC oal_void hmac_backlist_sub_drop(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr)
{
    oal_uint32          ul_ret;
    /* 2014.9.2 Add UT and found no init value set to it ! add initial value null */
    mac_blacklist_stru *pst_blacklist = OAL_PTR_NULL;

    ul_ret = hmac_blacklist_get(pst_mac_vap, puc_mac_addr, &pst_blacklist);

    if (pst_blacklist != OAL_PTR_NULL) {
        if (pst_blacklist->ul_drop_counter > 0) {
            pst_blacklist->ul_drop_counter--;
        }
        ul_ret = OAL_SUCC;
    }
    return;
}


oal_uint8 hmac_backlist_get_list_num(mac_vap_stru *pst_mac_vap)
{
    mac_blacklist_info_stru *pst_blacklist_info;
    hmac_vap_stru           *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_backlist_get_list_num::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_backlist_get_list_num::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_blacklist_info = &pst_hmac_vap->st_blacklist_info;

    return pst_blacklist_info->uc_list_num;
}


OAL_STATIC oal_uint32 hmac_autoblacklist_get(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr,
                                             mac_autoblacklist_stru **ppst_autoblacklist)
{
    oal_uint8                        ul_blacklist_index;
    mac_autoblacklist_info_stru     *pst_autoblacklist_info;
    oal_time_us_stru                 st_cur_time;
    oal_uint32                       ul_cfg_time_old;
    oal_uint32                       ul_blacklist_index_old;
    mac_autoblacklist_stru          *pst_autoblacklist = OAL_PTR_NULL;
    hmac_vap_stru                   *pst_hmac_vap;

    /* 1.1 �㲥��ַ�����ڷǷ���ַ */
    if (hmac_blacklist_mac_is_bcast(puc_mac_addr)) {
        return OAL_ERR_CODE_SECURITY_MAC_INVALID;
    }

    /* 1.2 ȫ���ַ�����ڷǷ���ַ */
    if (hmac_blacklist_mac_is_zero(puc_mac_addr)) {
        return OAL_ERR_CODE_SECURITY_MAC_INVALID;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_init::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_autoblacklist_info = &pst_hmac_vap->st_blacklist_info.st_autoblacklist_info;

    oal_time_get_stamp_us(&st_cur_time);

    *ppst_autoblacklist = OAL_PTR_NULL;

    /* 2.1 �ҵ���ʷ�� */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_ASSOC_USER_MAX_NUM_SPEC; ul_blacklist_index++) {
        pst_autoblacklist = &pst_autoblacklist_info->ast_autoblack_list[ul_blacklist_index];
        /* 2.2 ��Ч�� */
        if (pst_autoblacklist->ul_cfg_time == 0) {
            continue;
        }

        /* 2.2 ���ڱ�ֱ����0 */
        if (st_cur_time.i_sec > (oal_int)(pst_autoblacklist->ul_cfg_time + pst_autoblacklist_info->ul_reset_time)) {
            if (pst_autoblacklist_info->list_num > 0) {
                pst_autoblacklist_info->list_num--;
            }
            memset_s(pst_autoblacklist, OAL_SIZEOF(mac_autoblacklist_stru), 0, OAL_SIZEOF(mac_autoblacklist_stru));
            continue;
        }

        /* 2.3 ��Ч����mac��ַ�ȶ� */
        if (!oal_memcmp(pst_autoblacklist->auc_mac_addr, puc_mac_addr, OAL_MAC_ADDR_LEN)) {
            *ppst_autoblacklist = pst_autoblacklist;
            return OAL_SUCC;
        }
    }
    /* 3.1 �ҵ��±� */
    ul_cfg_time_old = (st_cur_time.i_sec > 0) ? (oal_uint32)(st_cur_time.i_sec) : 0;
    ul_blacklist_index_old = 0;
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_ASSOC_USER_MAX_NUM_SPEC; ul_blacklist_index++) {
        pst_autoblacklist = &pst_autoblacklist_info->ast_autoblack_list[ul_blacklist_index];
        /* 2.2 �ҵ����б� */
        if (pst_autoblacklist->ul_cfg_time == 0) {
            pst_autoblacklist_info->list_num++;
            *ppst_autoblacklist = pst_autoblacklist;
            break;
        }

        /* 2.3 ��¼�������õı� */
        if (pst_autoblacklist->ul_cfg_time < ul_cfg_time_old) {
            ul_cfg_time_old = pst_autoblacklist->ul_cfg_time;
            ul_blacklist_index_old = ul_blacklist_index;
        }
    }

    /* 4.1 ���û�п��еı�����ʹ�����ϵı� */
    if (*ppst_autoblacklist == OAL_PTR_NULL) {
        pst_autoblacklist = &pst_autoblacklist_info->ast_autoblack_list[ul_blacklist_index_old];
        *ppst_autoblacklist = pst_autoblacklist;
    }

    /* 5.1 ���±� */
    if (pst_autoblacklist == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_autoblacklist_get::pst_autoblacklist NULL}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (memcpy_s(pst_autoblacklist->auc_mac_addr, OAL_MAC_ADDR_LEN, puc_mac_addr, OAL_MAC_ADDR_LEN) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hmac_autoblacklist_get::memcpy fail!");
        return OAL_FAIL;
    }
    pst_autoblacklist->ul_cfg_time = (oal_uint32)st_cur_time.i_sec;
    pst_autoblacklist->ul_asso_counter = 0; /* bug 1 => 0 fixed */

    return OAL_SUCC;
}


OAL_STATIC oal_void hmac_blacklist_update_delete_user(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user)
{
    mac_vap_stru  *pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    mac_user_stru *pst_mac_user = &(pst_hmac_user->st_user_base_info);

    OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                     "hmac_blacklist_update_delete_user: send disasoc frame to XX:XX:XX:%2X:%2X:%2X. blacklist_mode",
                     pst_mac_user->auc_user_mac_addr[3], /* auc_user_mac_addr��3byteΪ���������ӡ */
                     pst_mac_user->auc_user_mac_addr[4], /* auc_user_mac_addr��4byteΪ���������ӡ */
                     pst_mac_user->auc_user_mac_addr[5], /* auc_user_mac_addr��5byteΪ���������ӡ */
                     pst_hmac_vap->st_blacklist_info.uc_mode);

    hmac_mgmt_send_disassoc_frame(pst_mac_vap, pst_mac_user->auc_user_mac_addr, MAC_DISAS_LV_SS, OAL_FALSE);

    /* ɾ�����������û�����Ҫ�ϱ��ں� */
    hmac_handle_disconnect_rsp_ap(pst_hmac_vap, pst_hmac_user);

    hmac_user_del(pst_mac_vap, pst_hmac_user);

    return;
}


OAL_STATIC oal_uint32 hmac_whitelist_check_user(mac_vap_stru *pst_mac_vap)
{
    oal_uint32           ul_ret;
    hmac_vap_stru       *pst_hmac_vap;
    oal_dlist_head_stru *pst_entry;
    oal_dlist_head_stru *pst_dlist_tmp;
    mac_user_stru       *pst_user_tmp;
    hmac_user_stru      *pst_hmac_user_tmp;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "hmac_whitelist_check_user: pst_hmac_vap is null");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ����VAP�������û������ڰ��������ɾ�� */
    OAL_DLIST_SEARCH_FOR_EACH_SAFE(pst_entry, pst_dlist_tmp, &(pst_mac_vap->st_mac_user_list_head))
    {
        pst_user_tmp = OAL_DLIST_GET_ENTRY(pst_entry, mac_user_stru, st_user_dlist);

        pst_hmac_user_tmp = mac_res_get_hmac_user(pst_user_tmp->us_assoc_id);
        if (pst_hmac_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        ul_ret = hmac_blacklist_filter(pst_mac_vap, pst_user_tmp->auc_user_mac_addr);
        if (ul_ret != OAL_TRUE) {
            continue;
        }
        /* 2014.6.30 chenchongbao �����ϵ�hmac_blacklist_filter()�л� ul_drop_counter++ ������ʵ�ʵĹ�����������--�ָ� */
        hmac_backlist_sub_drop(pst_mac_vap, pst_user_tmp->auc_user_mac_addr);

        hmac_blacklist_update_delete_user(pst_hmac_vap, pst_hmac_user_tmp);
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 hmac_blacklist_vap_check_user_by_macaddr(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr)
{
    mac_user_stru       *pst_mac_user;
    hmac_user_stru      *pst_hmac_user;
    hmac_vap_stru       *pst_hmac_vap;
    oal_uint32           ul_ret;
    oal_uint16           us_idx;
    oal_bool_enum_uint8  bool_ret;

    ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, puc_mac_addr, &us_idx);
    if (ul_ret != OAL_SUCC) {
        /* ��mac��ַ�ڴ�vap�²������û�������Ҫɾ����ֱ�ӷ��سɹ� */
        return OAL_SUCC;
    }

    pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(us_idx);
    if (pst_mac_user == OAL_PTR_NULL) {
        return OAL_FAIL; /* �쳣���� */
    }

    bool_ret = hmac_blacklist_filter(pst_mac_vap, pst_mac_user->auc_user_mac_addr);
    if (bool_ret != OAL_TRUE) {
        return OAL_FAIL;
    }
    /* 2014.6.30 chenchongbao �����ϵ�hmac_blacklist_filter()�л� ul_drop_counter++ ������ʵ�ʵĹ�����������--�ָ� */
    hmac_backlist_sub_drop(pst_mac_vap, pst_mac_user->auc_user_mac_addr);

    pst_hmac_user = mac_res_get_hmac_user(us_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{blacklist mac_res_get_hmac_user fail IDX:%d}", us_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_blacklist_update_delete_user(pst_hmac_vap, pst_hmac_user);

    return ul_ret;
}


oal_uint32 hmac_blacklist_add(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr, oal_uint32 ul_aging_time)
{
    oal_uint8                ul_blacklist_index;
    mac_blacklist_info_stru *pst_blacklist_info;
    mac_blacklist_stru      *pst_blacklist = OAL_PTR_NULL;
    oal_time_us_stru         st_cur_time;
    hmac_vap_stru           *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_add::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_blacklist_info = &pst_hmac_vap->st_blacklist_info;

    /* 3.1 �ҵ��� */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_blacklist = &pst_blacklist_info->ast_black_list[ul_blacklist_index];
        if (!oal_memcmp(pst_blacklist->auc_mac_addr, puc_mac_addr, OAL_MAC_ADDR_LEN)) {
            break;
        }
        pst_blacklist = OAL_PTR_NULL;
    }

    /* 4.1 ���Ѿ����ڣ�ֻ�����ϻ�ʱ�� */
    if (pst_blacklist != OAL_PTR_NULL) {
        pst_blacklist->ul_aging_time = ul_aging_time;
        return OAL_SUCC;
    }

    /* 5.1 ��һ����ַΪ�յı� */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_blacklist = &pst_blacklist_info->ast_black_list[ul_blacklist_index];
        if (hmac_blacklist_mac_is_zero(pst_blacklist->auc_mac_addr)) {
            break;
        }
        pst_blacklist = OAL_PTR_NULL;
    }

    /* 6.1 �޿��ñ� */
    if (pst_blacklist == OAL_PTR_NULL) {
        OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{blacklistsrc mac=%02x.%02x.%02x. new count is %d; full}",
                         /* puc_mac_addr��3��4��5byteΪ���������ӡ */
                         puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5], pst_blacklist_info->uc_list_num);
        return OAL_ERR_CODE_SECURITY_LIST_FULL;
    }

    /* 7.1 ���±� */
    oal_time_get_stamp_us(&st_cur_time);
    if (memcpy_s(pst_blacklist->auc_mac_addr,
        OAL_SIZEOF(pst_blacklist->auc_mac_addr), puc_mac_addr, OAL_MAC_ADDR_LEN) != EOK) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_blacklist_add::memcpy fail!}");
        return OAL_FAIL;
    }
    pst_blacklist->ul_cfg_time = (oal_uint32)st_cur_time.i_sec;
    pst_blacklist->ul_aging_time = ul_aging_time;
    pst_blacklist->ul_drop_counter = 0;
    pst_blacklist_info->uc_list_num++;

    OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_blacklist_add:: mac=x.x.x.x.%02x.%02x. num%d, time%d}",
                     /* puc_mac_addr��4��5byteΪ���������ӡ */
                     puc_mac_addr[4], puc_mac_addr[5], pst_blacklist_info->uc_list_num, pst_blacklist->ul_cfg_time);

    /* �ڰ�������ӳɹ���ˢ��һ���û� */
    if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_BLACK) {
        hmac_blacklist_vap_check_user_by_macaddr(pst_mac_vap, puc_mac_addr);
    } else if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_WHITE) {
        hmac_whitelist_check_user(pst_mac_vap);
    }

    return OAL_SUCC;
}


oal_uint32 hmac_blacklist_add_only(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr, oal_uint32 ul_aging_time)
{
    oal_uint8                ul_blacklist_index;
    mac_blacklist_info_stru *pst_blacklist_info;
    mac_blacklist_stru      *pst_blacklist = OAL_PTR_NULL;
    oal_time_us_stru         st_cur_time;
    hmac_vap_stru           *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_add_only::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_add_only::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_blacklist_info = &pst_hmac_vap->st_blacklist_info;

    /* 3.1 �ҵ��� */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_blacklist = &pst_blacklist_info->ast_black_list[ul_blacklist_index];
        if (!oal_memcmp(pst_blacklist->auc_mac_addr, puc_mac_addr, OAL_MAC_ADDR_LEN)) {
            break;
        }
        pst_blacklist = OAL_PTR_NULL;
    }

    /* 4.1 ���Ѿ����ڣ�ֻ�����ϻ�ʱ�� */
    if (pst_blacklist != OAL_PTR_NULL) {
        pst_blacklist->ul_aging_time = ul_aging_time;
        return OAL_SUCC;
    }

    /* 5.1 ��һ����ַΪ�յı� */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_blacklist = &pst_blacklist_info->ast_black_list[ul_blacklist_index];
        if (hmac_blacklist_mac_is_zero(pst_blacklist->auc_mac_addr)) {
            break;
        }
        pst_blacklist = OAL_PTR_NULL;
    }

    /* 6.1 �޿��ñ� */
    if (pst_blacklist == OAL_PTR_NULL) {
        return OAL_ERR_CODE_SECURITY_LIST_FULL;
    }

    /* 7.1 ���±� */
    oal_time_get_stamp_us(&st_cur_time);
    if (memcpy_s(pst_blacklist->auc_mac_addr, OAL_MAC_ADDR_LEN, puc_mac_addr, OAL_MAC_ADDR_LEN) != EOK) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_blacklist_add_only::memcpy fail!}");
        return OAL_FAIL;
    }
    pst_blacklist->ul_cfg_time = (oal_uint32)st_cur_time.i_sec;
    pst_blacklist->ul_aging_time = ul_aging_time;
    pst_blacklist->ul_drop_counter = 0;
    pst_blacklist_info->uc_list_num++;

    OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
        "{>>>>>> blacklist_add:: src mac=x.x.x.x.%02x.%02x. new count is %d, time=%d}",
        /* puc_mac_addr��4��5byteΪ���������ӡ */
        puc_mac_addr[4], puc_mac_addr[5], pst_blacklist_info->uc_list_num, pst_blacklist->ul_cfg_time);

    return OAL_SUCC;
}


oal_uint32 hmac_blacklist_del(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr)
{
    oal_uint8                ul_blacklist_index;
    mac_blacklist_stru      *pst_blacklist = OAL_PTR_NULL;
    mac_blacklist_info_stru *pst_blacklist_info;
    hmac_vap_stru           *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_del::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{hmac_blacklist_del::pst_hmac_vap null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 ���ģʽ��һ�£�����Ҫɾ��������ʧ�ܼ��� */
    pst_blacklist_info = &pst_hmac_vap->st_blacklist_info;

    /* 3.1 �㲥��ַ����Ҫɾ�����б� */
    if (hmac_blacklist_mac_is_bcast(puc_mac_addr)) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_blacklist_del::broadcast addr; delete all hmac_blacklist}");

        hmac_blacklist_init(pst_mac_vap, pst_blacklist_info->uc_mode);

        return OAL_SUCC;
    }

    /* 4.1 �ҵ��� */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_blacklist = &pst_blacklist_info->ast_black_list[ul_blacklist_index];
        if (!oal_memcmp(pst_blacklist->auc_mac_addr, puc_mac_addr, OAL_MAC_ADDR_LEN)) {
            break;
        }
        pst_blacklist = OAL_PTR_NULL;
    }

    /* 5.1 ����ҵ�����ɾ�� */
    if (pst_blacklist != OAL_PTR_NULL) {
        memset_s(pst_blacklist, OAL_SIZEOF(mac_blacklist_stru), 0, OAL_SIZEOF(mac_blacklist_stru));
        pst_blacklist_info->uc_list_num--;
        OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                      "{hmac_blacklist_del:: mac=x.x.x.%02x.%02x.%02x. new count is %d}",
                      /* puc_mac_addr��3��4��5byteΪ���������ӡ */
                      puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5], pst_blacklist_info->uc_list_num);
    } else {
        OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_blacklist_del:: didn't find this mac: %02X:XX:XX:%02X:%02X:%02X}",
                         /* puc_mac_addr��0��3��4��5byteΪ���������ӡ */
                         puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);
    }

    return OAL_SUCC;
}


oal_uint32 hmac_blacklist_update(mac_vap_stru *pst_mac_vap, hmac_blacklist_cfg_stru *pst_blacklist_cfg)
{
    cs_blacklist_mode_enum_uint8     en_mode;
    oal_uint8                       *puc_mac_addr;
    oal_uint32                       ul_ret;

    /* 1.1 ��μ�� */
    if ((pst_mac_vap == OAL_PTR_NULL) || (pst_blacklist_cfg == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_update::null mac_vap or null cfg data}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 ��ȡ���� */
    en_mode = (cs_blacklist_type_enum_uint8)pst_blacklist_cfg->uc_mode;
    puc_mac_addr = (oal_uint8 *)pst_blacklist_cfg->auc_sa;

    /*  */
    if (hmac_blacklist_mac_is_zero(puc_mac_addr)) {
        return OAL_ERR_CODE_SECURITY_MAC_INVALID;
    }

    /* 3.1 ���ݲ�ͬģʽ���ӱ� */
    switch (en_mode) {
        case CS_BLACKLIST_MODE_BLACK:
        case CS_BLACKLIST_MODE_WHITE:
            ul_ret = hmac_blacklist_add(pst_mac_vap, puc_mac_addr, CS_INVALID_AGING_TIME);
            if (ul_ret != OAL_SUCC) {
                OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_blacklist_add::fail ret=%d}", ul_ret);
                return ul_ret;
            }
            break;

        default:
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{unknown mode=%d}", en_mode);
            return OAL_ERR_CODE_SECURITY_MODE_INVALID;
    }

    return OAL_SUCC;
}


oal_uint32 hmac_blacklist_set_mode(mac_vap_stru *pst_mac_vap, oal_uint8 uc_mode)
{
    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_set_mode::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_blacklist_set_mode::mode = %d}", uc_mode);

    /* 2.1 ���ݲ�ͬģʽ���ӱ� */
    switch (uc_mode) {
        case CS_BLACKLIST_MODE_NONE:
        case CS_BLACKLIST_MODE_BLACK:
        case CS_BLACKLIST_MODE_WHITE:
            hmac_blacklist_init(pst_mac_vap, uc_mode);
            break;

        default:
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{unknow mode = %d}", uc_mode);
            return OAL_ERR_CODE_SECURITY_MODE_INVALID;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_void hmac_show_autoblacklist_info(mac_autoblacklist_info_stru *pst_autoblacklist_info)
{
    oal_uint8                ul_blacklist_index;
    oal_uint8               *puc_sa;
    mac_autoblacklist_stru  *pst_autoblacklist;
    oal_int8                *pc_print_buff;
    oal_uint8                buff_index;

    pc_print_buff = (oal_int8 *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_LOCAL, OAM_REPORT_MAX_STRING_LEN, OAL_TRUE);
    if (pc_print_buff == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{hmac_show_autoblacklist_info::pc_print_buff null.}");
        return;
    }
    memset_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN, 0, OAM_REPORT_MAX_STRING_LEN);

    /* 2.1 ������������Ϣ */
    buff_index = snprintf_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN, OAM_REPORT_MAX_STRING_LEN - 1,
        "\nAUTOBLACKLIST[%d] info:\n"
        "  THRESHOLD: %u\n"
        "  RESET_TIME: %u sec\n"
        "  AGING_TIME: %u sec\n",
        pst_autoblacklist_info->uc_enabled, pst_autoblacklist_info->ul_threshold,
        pst_autoblacklist_info->ul_reset_time, pst_autoblacklist_info->ul_aging_time);

    /* 4.1 ��ӡ�������� */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_ASSOC_USER_MAX_NUM_SPEC; ul_blacklist_index++) {
        pst_autoblacklist = &pst_autoblacklist_info->ast_autoblack_list[ul_blacklist_index];
        if (hmac_blacklist_mac_is_zero(pst_autoblacklist->auc_mac_addr)) {
            continue;
        }
        puc_sa = pst_autoblacklist->auc_mac_addr;
        OAM_INFO_LOG4(0, OAM_SF_ANY,
                      /* puc_sa��0��3��4��5byteΪ���������ӡ */
                      "{src mac=%02X:XX:XX:%02X:%02X:%02X}", puc_sa[0], puc_sa[3], puc_sa[4], puc_sa[5]);
        OAM_INFO_LOG2(0, OAM_SF_ANY, "{ ASSO_CNT: %u. cfg_time=%d. }",
                      pst_autoblacklist->ul_asso_counter, pst_autoblacklist->ul_cfg_time);

        buff_index += snprintf_s(pc_print_buff + buff_index, (OAM_REPORT_MAX_STRING_LEN - buff_index),
            (OAM_REPORT_MAX_STRING_LEN - buff_index) - 1,
            "\n[%02d] MAC: %02X:XX:XX:%02X:%02X:%02X\n"
            " cfg_time=%d. ASSO_CNT: %d\n", ul_blacklist_index,
            pst_autoblacklist->auc_mac_addr[0],
            pst_autoblacklist->auc_mac_addr[3], /* auc_mac_addr��3byteΪ���������ӡ */
            pst_autoblacklist->auc_mac_addr[4], /* auc_mac_addr��4byteΪ���������ӡ */
            pst_autoblacklist->auc_mac_addr[5], /* auc_mac_addr��5byteΪ���������ӡ */
            pst_autoblacklist->ul_cfg_time, pst_autoblacklist->ul_asso_counter);
    }
    oam_print(pc_print_buff);
    OAL_MEM_FREE(pc_print_buff, OAL_TRUE);
}


oal_void hmac_show_blacklist_info(mac_vap_stru *pst_mac_vap)
{
    oal_uint8                   *puc_sa;
    oal_uint8                    buff_index;
    oal_uint8                    ul_blacklist_index;
    mac_blacklist_stru          *pst_blacklist;
    mac_blacklist_info_stru     *pst_info;
    oal_time_us_stru             st_cur_time;
    oal_int8                    *pc_print_buff;
    hmac_vap_stru               *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_show_blacklist_info::null mac_vap}");
        return;
    }

    pc_print_buff = (oal_int8 *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_LOCAL, OAM_REPORT_MAX_STRING_LEN, OAL_TRUE);
    if (pc_print_buff == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_show_blacklist_info::pc_print_buff null.}");
        return;
    }
    memset_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN, 0, OAM_REPORT_MAX_STRING_LEN);

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAL_MEM_FREE(pc_print_buff, OAL_TRUE);
        return;
    }

    pst_info = &pst_hmac_vap->st_blacklist_info;

    /* 3.1 ������ģʽ��Ϣ */
    if (pst_info->uc_mode == CS_BLACKLIST_MODE_BLACK) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{BLACKLIST[BLACK] num=%d info:}", pst_info->uc_list_num);
        buff_index = snprintf_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN, OAM_REPORT_MAX_STRING_LEN - 1,
            "BLACKLIST[BLACK] num=%d info:\n", pst_info->uc_list_num);
    } else if (pst_info->uc_mode == CS_BLACKLIST_MODE_WHITE) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{BLACKLIST[WHITE] num=%d info:}", pst_info->uc_list_num);
        buff_index = snprintf_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN, OAM_REPORT_MAX_STRING_LEN - 1,
            "BLACKLIST[WHITE] num=%d info:\n", pst_info->uc_list_num);
    } else {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{BLACKLIST disable num=%d info:}", pst_info->uc_list_num);
        buff_index = snprintf_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN, OAM_REPORT_MAX_STRING_LEN - 1,
            "BLACKLIST not enable! num=%d info:\n", pst_info->uc_list_num);
    }

    /* 5.1 ��ӡ�������� */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_blacklist = &pst_info->ast_black_list[ul_blacklist_index];
        if (hmac_blacklist_mac_is_zero(pst_blacklist->auc_mac_addr)) {
            continue;
        }
        puc_sa = pst_blacklist->auc_mac_addr;
        OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         /* puc_sa��0��3��4��5byteΪ���������ӡ */
                         "{src mac=%02X:XX:XX:%02X:%02X:%02X}", puc_sa[0], puc_sa[3], puc_sa[4], puc_sa[5]);
        OAM_WARNING_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{ CFG_TIME: %u. AGE_TIME: %u. DROP_CNT: %u.}",
                         pst_blacklist->ul_cfg_time, pst_blacklist->ul_aging_time, pst_blacklist->ul_drop_counter);

        buff_index += snprintf_s(pc_print_buff + buff_index,
            (OAM_REPORT_MAX_STRING_LEN - buff_index), (OAM_REPORT_MAX_STRING_LEN - buff_index) - 1,
            "\n[%02d] MAC: %02X:XX:XX:%02X:%02X:%02X\n"
            " CFG_TIME: %u\t AGE_TIME: %u\t DROP_CNT: %u\n",
            /* auc_mac_addr��0��3byte�����ӡ */
            ul_blacklist_index, pst_blacklist->auc_mac_addr[0], pst_blacklist->auc_mac_addr[3],
            pst_blacklist->auc_mac_addr[4], pst_blacklist->auc_mac_addr[5], /* auc_mac_addr��4��5byte�����ӡ */
            pst_blacklist->ul_cfg_time, pst_blacklist->ul_aging_time, pst_blacklist->ul_drop_counter);
    }
    oam_print(pc_print_buff);
    OAL_MEM_FREE(pc_print_buff, OAL_TRUE);

    /* 4.1 �Զ���������Ϣ */
    hmac_show_autoblacklist_info(&pst_info->st_autoblacklist_info);

    /* ���뵱ǰʱ�� */
    oal_time_get_stamp_us(&st_cur_time);
}


oal_uint32 hmac_autoblacklist_enable(mac_vap_stru *pst_mac_vap, oal_uint8 uc_enabled)
{
    mac_autoblacklist_info_stru *pst_autoblacklist_info;
    hmac_vap_stru               *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_autoblacklist_enable::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 ��ȡ���� */
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_autoblacklist_enable::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_autoblacklist_info = &pst_hmac_vap->st_blacklist_info.st_autoblacklist_info;

    /* ������ԭ��һ�� */
    if (uc_enabled == pst_autoblacklist_info->uc_enabled) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_autoblacklist_enable:autoblacklist is already %d}", uc_enabled);

        return OAL_SUCC;
    }

    /* ���³�ʼ�� */
    if (uc_enabled == 1) {
        hmac_blacklist_init(pst_mac_vap, CS_BLACKLIST_MODE_BLACK);
        pst_autoblacklist_info->uc_enabled = uc_enabled;
        pst_autoblacklist_info->ul_aging_time = CS_DEFAULT_AGING_TIME;
        pst_autoblacklist_info->ul_reset_time = CS_DEFAULT_RESET_TIME;
        pst_autoblacklist_info->ul_threshold = CS_DEFAULT_THRESHOLD;
    } else { /* �ر��Զ�������������������� */
        hmac_blacklist_init(pst_mac_vap, CS_BLACKLIST_MODE_NONE); /* 2013.7.23 add clean */
    }

    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                  "{hmac_autoblacklist_enable:autoblacklist is %d}", uc_enabled);

    return OAL_SUCC;
}


oal_uint32 hmac_autoblacklist_set_aging(mac_vap_stru *pst_mac_vap, oal_uint32 ul_aging_time)
{
    mac_autoblacklist_info_stru *pst_autoblacklist_info;
    hmac_vap_stru               *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_autoblacklist_set_aging::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 ��ȡ���� */
    if (ul_aging_time == 0) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{ul_aging_time should not be 0}");
        return OAL_ERR_CODE_SECURITY_AGING_INVALID;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_autoblacklist_set_aging::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_autoblacklist_info = &pst_hmac_vap->st_blacklist_info.st_autoblacklist_info;

    /* 3.1 �Զ�������û��ʹ�� */
    if (pst_autoblacklist_info->uc_enabled == 0) {
        hmac_autoblacklist_enable(pst_mac_vap, 1);
    }

    /* 4.1 �����ϻ�ʱ�� */
    pst_autoblacklist_info->ul_aging_time = ul_aging_time;

    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{blacklist set auto aging = %d}", ul_aging_time);
    return OAL_SUCC;
}


oal_uint32 hmac_autoblacklist_set_threshold(mac_vap_stru *pst_mac_vap, oal_uint32 ul_threshold)
{
    mac_autoblacklist_info_stru *pst_autoblacklist_info;
    hmac_vap_stru               *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_autoblacklist_set_threshold::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 ��ȡ���� */
    if (ul_threshold == 0) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{ul_threshold should not be 0}");
        return OAL_ERR_CODE_SECURITY_THRESHOLD_INVALID;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_autoblacklist_set_threshold::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_autoblacklist_info = &pst_hmac_vap->st_blacklist_info.st_autoblacklist_info;

    /* 3.1 �Զ�������û��ʹ�� */
    if (pst_autoblacklist_info->uc_enabled == 0) {
        hmac_autoblacklist_enable(pst_mac_vap, 1);
    }

    /* 4.1 �������� */
    pst_autoblacklist_info->ul_threshold = ul_threshold;

    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{blacklist set auto threshold = %d}", ul_threshold);
    return OAL_SUCC;
}


oal_uint32 hmac_autoblacklist_set_reset_time(mac_vap_stru *pst_mac_vap, oal_uint32 ul_reset_time)
{
    mac_autoblacklist_info_stru *pst_autoblacklist_info;
    hmac_vap_stru               *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_autoblacklist_set_reset_time::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 ��ȡ���� */
    if (ul_reset_time == 0) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{ul_aging_time should not be 0}");
        return OAL_ERR_CODE_SECURITY_RESETIME_INVALID;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_autoblacklist_set_reset_time::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_autoblacklist_info = &pst_hmac_vap->st_blacklist_info.st_autoblacklist_info;

    /* 3.1 �Զ�������û��ʹ�� */
    if (pst_autoblacklist_info->uc_enabled == 0) {
        hmac_autoblacklist_enable(pst_mac_vap, 1);
    }

    /* 4.1 ��������ʱ�� */
    pst_autoblacklist_info->ul_reset_time = ul_reset_time;

    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{blacklist set auto reset_time = %d}", ul_reset_time);
    return OAL_SUCC;
}


oal_uint32 hmac_isolation_set_mode(mac_vap_stru *pst_mac_vap, oal_uint8 uc_mode)
{
    mac_isolation_info_stru *pst_isolation_info;
    hmac_vap_stru           *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_isolation_set_mode::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_init::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 ��ȡ���� */
    uc_mode = uc_mode & 0x7;
    if (uc_mode == 0) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{isolation_set_mode invalid, The valid Para is:(1~7)}\n");
        /* add mode check chenchongbao 2014.7.7 */
        return OAL_ERR_CODE_SECURITY_MODE_INVALID;
    }

    pst_isolation_info = &pst_hmac_vap->st_isolation_info;

    /* 3.1 ���³�ʼ�� */
    pst_isolation_info->uc_mode = uc_mode;

    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{>>>>>> isolation mode is set to %d}", uc_mode);
    return OAL_SUCC;
}


oal_uint32 hmac_isolation_set_type(mac_vap_stru *pst_mac_vap, oal_uint8 uc_type)
{
    mac_isolation_info_stru *pst_isolation_info;
    hmac_vap_stru           *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_isolation_set_type::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 ��ȡ���� */
    if (uc_type >= CS_ISOLATION_TYPE_BUTT) {
        uc_type = CS_ISOLATION_TYPE_NONE;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_init::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_isolation_info = &pst_hmac_vap->st_isolation_info;

    /* 3.1 ���³�ʼ�� */
    pst_isolation_info->uc_type = uc_type;

    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{>>>>>> isolation type is set to %d}", uc_type);
    return OAL_SUCC;
}


oal_uint32 hmac_isolation_set_forward(mac_vap_stru *pst_mac_vap, oal_uint8 uc_forward)
{
    mac_isolation_info_stru *pst_isolation_info;
    hmac_vap_stru           *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_isolation_set_forward::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_init::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 ��ȡ���� */
    if (uc_forward >= CS_ISOLATION_FORWORD_BUTT) {
        uc_forward = CS_ISOLATION_FORWORD_TOLAN;
    }

    pst_isolation_info = &pst_hmac_vap->st_isolation_info;
    pst_isolation_info->uc_forward = uc_forward;
    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{>>>>>> isolation forward is set to %d}", uc_forward);

    return OAL_SUCC;
}


oal_uint32 hmac_isolation_clear_counter(mac_vap_stru *pst_mac_vap)
{
    mac_isolation_info_stru *pst_isolation_info;
    hmac_vap_stru           *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_isolation_clear_counter::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_isolation_clear_counter::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_isolation_info = &pst_hmac_vap->st_isolation_info;

    /* 2.1 ˢ�¼����� */
    pst_isolation_info->ul_counter_bcast = 0;
    pst_isolation_info->ul_counter_mcast = 0;
    pst_isolation_info->ul_counter_ucast = 0;

    OAM_INFO_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{isolation counters is cleared }");
    return OAL_SUCC;
}


oal_uint32 hmac_isolation_get_bcast_counter(mac_vap_stru *pst_mac_vap)
{
    mac_isolation_info_stru *pst_isolation_info;
    hmac_vap_stru           *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_isolation_get_bcast_counter::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_isolation_get_bcast_counter::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_isolation_info = &pst_hmac_vap->st_isolation_info;
    return pst_isolation_info->ul_counter_bcast;
}


oal_uint32 hmac_isolation_get_mcast_counter(mac_vap_stru *pst_mac_vap)
{
    mac_isolation_info_stru *pst_isolation_info;
    hmac_vap_stru           *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_isolation_get_mcast_counter::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_init::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_isolation_info = &pst_hmac_vap->st_isolation_info;
    return pst_isolation_info->ul_counter_mcast;
}


oal_uint32 hmac_isolation_get_ucast_counter(mac_vap_stru *pst_mac_vap)
{
    mac_isolation_info_stru *pst_isolation_info;
    hmac_vap_stru           *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_isolation_get_ucast_counter::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_isolation_get_ucast_counter::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_isolation_info = &pst_hmac_vap->st_isolation_info;
    return pst_isolation_info->ul_counter_ucast;
}


oal_void hmac_show_isolation_info(mac_vap_stru *pst_mac_vap)
{
    mac_isolation_info_stru *pst_isolation_info;
    oal_int8                *pc_print_buff;
    hmac_vap_stru           *pst_hmac_vap;

    /* 1.1 ��μ�� */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_show_isolation_info::null mac_vap}");
        return;
    }
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_show_isolation_info::pst_hmac_vap null.}");
        return;
    }

    pc_print_buff = (oal_int8 *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_LOCAL, OAM_REPORT_MAX_STRING_LEN, OAL_TRUE);
    if (pc_print_buff == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{hmac_show_autoblacklist_info::pc_print_buff null.}");
        return;
    }
    memset_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN, 0, OAM_REPORT_MAX_STRING_LEN);

    pst_isolation_info = &pst_hmac_vap->st_isolation_info;

    /* 1.2 ��ӡ������Ϣ */
    OAM_INFO_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{>>>>>> isolation info is :}");
    OAM_INFO_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{ mode:%d. type:%d. forward:%d.}",
                  pst_isolation_info->uc_mode, pst_isolation_info->uc_type, pst_isolation_info->uc_forward);

    snprintf_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN, OAM_REPORT_MAX_STRING_LEN - 1,
        "vap%d isolation info is :\n"
        "\tmode:%d. type:%d. forward:%d.\n"
        "\tbcast_cnt: %u\n"
        "\tmcast_cnt: %u\n"
        "\tucast_cnt: %u\n",
        pst_mac_vap->uc_vap_id,
        pst_isolation_info->uc_mode, pst_isolation_info->uc_type,
        pst_isolation_info->uc_forward,
        pst_isolation_info->ul_counter_bcast,
        pst_isolation_info->ul_counter_mcast,
        pst_isolation_info->ul_counter_ucast);

    oam_print(pc_print_buff);
    OAL_MEM_FREE(pc_print_buff, OAL_TRUE);

    return;
}


oal_bool_enum_uint8 hmac_blacklist_filter(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr)
{
    mac_blacklist_stru      *pst_blacklist;
    hmac_vap_stru           *pst_hmac_vap;
    mac_blacklist_info_stru *pst_blacklist_info;
    oal_uint32               ul_ret;
    oal_bool_enum_uint8      b_ret;

    /* 1.1 ��μ�� */
    if ((pst_mac_vap == OAL_PTR_NULL) || (puc_mac_addr == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_filter::null mac_vap or null mac addr}");
        return OAL_FALSE;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{hmac_blacklist_filter::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_blacklist_info = &pst_hmac_vap->st_blacklist_info;

    /* 1.1 ����û�п���������Ҫ���� */
    if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_NONE) {
        return OAL_FALSE;
    }

    /* 2.1 ������ģʽ�� */
    if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_BLACK) {
        ul_ret = hmac_blacklist_get(pst_mac_vap, puc_mac_addr, &pst_blacklist);
        /* ��ʾpuc_mac_addr��ַΪ�㲥��ַ����ȫ���ַ, ������ */
        if (ul_ret != OAL_SUCC) {
            return OAL_FALSE;
        }
        /* 2.2 ����Ҳ�����������Ҫ���� */
        if (pst_blacklist == OAL_PTR_NULL) {
            return OAL_FALSE;
        }

        /* 2.3 �ϻ�ʱ�䵽�ˣ�����Ҫ���� */
        b_ret = hmac_blacklist_is_aged(pst_mac_vap, pst_blacklist);
        if (b_ret == OAL_TRUE) {
            return OAL_FALSE;
        }
        /* 2.4 ��������¶���Ҫ���� */
        pst_blacklist->ul_drop_counter++;
        OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                      "{hmac_blacklist_filter::blacklist_filter drop_counter = %d MAC:=x.x.x.%02x.%02x.%02x}",
                      /* puc_mac_addr��3��4��5byteΪ���������ӡ */
                      pst_blacklist->ul_drop_counter, puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);

        return OAL_TRUE;
    }

    /* 3.1 ������ģʽ�� */
    if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_WHITE) {
        ul_ret = hmac_blacklist_get(pst_mac_vap, puc_mac_addr, &pst_blacklist);
        /* ��ʾpuc_mac_addr��ַΪ�㲥��ַ����ȫ���ַ, ������ */
        if (ul_ret != OAL_SUCC) {
            return OAL_FALSE;
        }
        /* 3.2 ����Ҳ���������Ҫ���� */
        if (pst_blacklist == OAL_PTR_NULL) {
            OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                          "{hmac_blacklist_filter::whitelist_filter MAC:=%02X:XX:XX:%02X:%02X:%02X}",
                          /* puc_mac_addr��0��3��4��5byteΪ���������ӡ */
                          puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);
            return OAL_TRUE;
        }

        /* 3.3 �ϻ�ʱ�䵽�ˣ�����Ҫ���� */
        b_ret = hmac_blacklist_is_aged(pst_mac_vap, pst_blacklist);
        if (b_ret == OAL_TRUE) {
            OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                          "{hmac_blacklist_filter::aging reach. go through=%02X:XX:XX:%02X:%02X:%02X}",
                          /* puc_mac_addr��0��3��4��5byteΪ���������ӡ */
                          puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);
            return OAL_FALSE;
        }

        /* 3.4 �ϻ�ʱ��û�е�����Ҫ���� */
        if (pst_blacklist->ul_aging_time != 0) {
            OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                          "{hmac_blacklist_filter::aging not zero; whilte_filter MAC=%02X:XX:XX:%02X:%02X:%02X}",
                          /* puc_mac_addr��0��3��4��5byteΪ���������ӡ */
                          puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);

            return OAL_TRUE;
        }
    }

    /* ������ڰ��������������ģ�������Ҫ���� */
    return OAL_FALSE;
}


oal_void hmac_autoblacklist_filter(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr)
{
    mac_autoblacklist_stru      *pst_autoblacklist = OAL_PTR_NULL; /* ��ӳ�ʼ��ֵ��������UT��׮���ֵ��null */
    mac_autoblacklist_info_stru *pst_autoblacklist_info;
    oal_uint32                   ul_ret;
    hmac_vap_stru               *pst_hmac_vap;
    mac_blacklist_stru          *pst_blacklist = OAL_PTR_NULL;

    /* 1.1 ��μ�� */
    if ((pst_mac_vap == OAL_PTR_NULL) || (puc_mac_addr == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_autoblacklist_filter::null mac_vap or null mac addr}");
        return;
    }
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_autoblacklist_filter::pst_hmac_vap null.}");
        return;
    }
    pst_autoblacklist_info = &pst_hmac_vap->st_blacklist_info.st_autoblacklist_info;
    /* 1.1 ����û�п��� */
    if (pst_autoblacklist_info->uc_enabled == 0) {
        return;
    }
    /* 1.2 ����Ƿ��Ѿ��ں�������, ���ں������У�ֱ�ӷ��� */
    ul_ret = hmac_blacklist_get(pst_mac_vap, puc_mac_addr, &pst_blacklist);
    if (pst_blacklist != OAL_PTR_NULL) {
        return;
    }
    /* 2.1 �ҵ���������ͳ�Ʊ�  */
    ul_ret = hmac_autoblacklist_get(pst_mac_vap, puc_mac_addr, &pst_autoblacklist);
    if (ul_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_autoblacklist_get failed %d}", ul_ret);
        return;
    }
    /* 2.2 ����Ҳ������������� */
    if (pst_autoblacklist == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{Can't find item to add}");
        return;
    }
    /* 3.1 �����������ж� */
    if (++pst_autoblacklist->ul_asso_counter > pst_autoblacklist_info->ul_threshold) {
        ul_ret = hmac_blacklist_add(pst_mac_vap, puc_mac_addr, pst_autoblacklist_info->ul_aging_time);
        if (ul_ret != OAL_SUCC) {
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_blacklist_add failed %d}", ul_ret);
            return;
        }
        /* 3.2 ɾ��ͳ�Ʊ� */
        memset_s(pst_autoblacklist, OAL_SIZEOF(mac_autoblacklist_stru), 0, OAL_SIZEOF(mac_autoblacklist_stru));
        if (pst_autoblacklist_info->list_num > 0) {
            pst_autoblacklist_info->list_num--;
        }
    }
    return;
}


cs_isolation_forward_enum hmac_isolation_filter(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr)
{
    mac_isolation_info_stru     *pst_isolation_info;
    oal_uint32                   ul_ret;
    mac_user_stru               *pst_mac_user;
    hmac_vap_stru               *pst_hmac_vap;
    oal_uint16                   us_user_idx;
    /* 1.1 ��μ�� */
    if ((pst_mac_vap == OAL_PTR_NULL) || (puc_mac_addr == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_isolation_filter::null mac_vap or null mac addr}");
        return CS_ISOLATION_FORWORD_NONE;
    }
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_isolation_filter::pst_hmac_vap null.}");
        return CS_ISOLATION_FORWORD_NONE;
    }

    pst_isolation_info = &pst_hmac_vap->st_isolation_info;

    /* (Ĭ��)û�п����û����빦�ܣ����� */
    if (pst_isolation_info->uc_type == CS_ISOLATION_TYPE_NONE) {
        return CS_ISOLATION_FORWORD_NONE;
    }

    /* 1.1 ��BSS���� */
    if (pst_isolation_info->uc_type == CS_ISOLATION_TYPE_MULTI_BSS) {
        /* 1.2 �㲥���� */
        if (ETHER_IS_BROADCAST(puc_mac_addr)) {
            if (BROADCAST_ISOLATION(pst_isolation_info->uc_mode)) {
                pst_isolation_info->ul_counter_bcast++;
                OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                              "{>>>>>>isolation MultiBSS Bcast=%d}", pst_isolation_info->ul_counter_bcast);
                return (cs_isolation_forward_enum)pst_isolation_info->uc_forward;
            }
            return CS_ISOLATION_FORWORD_NONE;
        }
        /* 1.3 �鲥���� */
        if (ETHER_IS_MULTICAST(puc_mac_addr)) {
            if (MULTICAST_ISOLATION(pst_isolation_info->uc_mode)) {
                pst_isolation_info->ul_counter_mcast++;
                OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                              "{>>>>>>isolation MultiBSS Mcast=%d}", pst_isolation_info->ul_counter_mcast);
                return (cs_isolation_forward_enum)pst_isolation_info->uc_forward;
            }
            return CS_ISOLATION_FORWORD_NONE;
        }
        /* 2.4 ��������,����ڱ�bss���ҵ��û��������봦��������Ҫ������bss���ң��ҵ��͸��� */
        if (UNICAST_ISOLATION(pst_isolation_info->uc_mode)) {
            ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, puc_mac_addr, &us_user_idx);
            if (ul_ret == OAL_SUCC) {
                /* return CS_ISOLATION_FORWORD_NONE; 2014.9.20 ��BSS����,ͬBSSҲ��Ҫ���� */
                return (cs_isolation_forward_enum)pst_isolation_info->uc_forward;
            }

            ul_ret = mac_device_find_user_by_macaddr(pst_mac_vap, puc_mac_addr, &us_user_idx);
            if (ul_ret != OAL_SUCC) {
                return CS_ISOLATION_FORWORD_NONE;
            }
            pst_mac_user = mac_res_get_mac_user(us_user_idx);
            if (pst_mac_user == OAL_PTR_NULL) {
                return CS_ISOLATION_FORWORD_NONE;
            }
            pst_isolation_info->ul_counter_ucast++;
            OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                          "{>>>>>>isolation MultiBSS Ucast=%d. to x.x.x.%02x.%02x.%02x}",
                          /* puc_mac_addr��3��4��5byteΪ���������ӡ */
                          pst_isolation_info->ul_counter_ucast, puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);

            return (cs_isolation_forward_enum)pst_isolation_info->uc_forward;
        }

        return CS_ISOLATION_FORWORD_NONE;
    }

    /* 2.1 ��BSS���� */
    if (pst_isolation_info->uc_type == CS_ISOLATION_TYPE_SINGLE_BSS) {
        /* 2.2 �㲥���� */
        if (ETHER_IS_BROADCAST(puc_mac_addr)) {
            if (BROADCAST_ISOLATION(pst_isolation_info->uc_mode)) {
                pst_isolation_info->ul_counter_bcast++;
                OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                              "{>>>>>>isolation SingleBSS Bcast=%d}", pst_isolation_info->ul_counter_bcast);
                return (cs_isolation_forward_enum)pst_isolation_info->uc_forward;
            }
            return CS_ISOLATION_FORWORD_NONE;
        }
        /* 2.3 �鲥���� */
        if (ETHER_IS_MULTICAST(puc_mac_addr)) {
            if (MULTICAST_ISOLATION(pst_isolation_info->uc_mode)) {
                pst_isolation_info->ul_counter_mcast++;
                OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                              "{>>>>>>isolation SingleBSS Mcast=%d}", pst_isolation_info->ul_counter_mcast);
                return (cs_isolation_forward_enum)pst_isolation_info->uc_forward;
            }
            return CS_ISOLATION_FORWORD_NONE;
        }
        /* 2.4 �������룬����ڱ�bss���ҵ��û��͸��룬���򲻴��� */
        if (UNICAST_ISOLATION(pst_isolation_info->uc_mode)) {
            ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, puc_mac_addr, &us_user_idx);
            if (ul_ret != OAL_SUCC) {
                return CS_ISOLATION_FORWORD_NONE;
            }

            pst_mac_user = mac_res_get_mac_user(us_user_idx);
            if (pst_mac_user == OAL_PTR_NULL) {
                return CS_ISOLATION_FORWORD_NONE;
            }
            pst_isolation_info->ul_counter_ucast++;

            OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                          "{>>>>>>isolation SingleBSS Ucast=%d. to x.x.x.%02x.%02x.%02x}",
                          /* puc_mac_addr��3��4��5byteΪ���������ӡ */
                          pst_isolation_info->ul_counter_ucast, puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);

            return (cs_isolation_forward_enum)pst_isolation_info->uc_forward;
        }
    }

    return CS_ISOLATION_FORWORD_NONE;
}

#endif /* #ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY */




