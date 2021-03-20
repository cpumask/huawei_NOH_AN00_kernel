

/* 1 ͷ�ļ����� */
#include "frw_ext_if.h"
#include "hmac_arp_offload.h"
#include "hmac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ARP_OFFLOAD_C

#ifdef _PRE_WLAN_FEATURE_ARP_OFFLOAD
/* 2 ȫ�ֱ������� */
/* 3 ����ʵ�� */

oal_uint32 hmac_arp_offload_set_ip_addr(mac_vap_stru *pst_mac_vap,
                                        dmac_ip_type_enum_uint8 en_type,
                                        dmac_ip_oper_enum_uint8 en_oper,
                                        oal_void *pst_ip_addr,
                                        oal_void *pst_mask_addr)
{
    dmac_ip_addr_config_stru  st_ip_addr_config;
    oal_uint32                ul_ret;
    oal_int32                 l_ret = EOK;

    if (OAL_UNLIKELY((pst_mac_vap == OAL_PTR_NULL) ||
                     (pst_ip_addr == OAL_PTR_NULL) ||
                     (pst_mask_addr == OAL_PTR_NULL))) {
        OAM_ERROR_LOG3(0, OAM_SF_PWR,
                       "{hmac_arp_offload_set_ip_addr::The pst_mac_vap or ip_addr or mask_addr is NULL. %p, %p, %p}",
                       (uintptr_t)pst_mac_vap,
                       (uintptr_t)pst_ip_addr,
                       (uintptr_t)pst_mask_addr);
        return OAL_ERR_CODE_PTR_NULL;
    }

    memset_s(&st_ip_addr_config, OAL_SIZEOF(st_ip_addr_config), 0, OAL_SIZEOF(st_ip_addr_config));

    if (en_type == DMAC_CONFIG_IPV4) {
        l_ret += memcpy_s(st_ip_addr_config.auc_ip_addr, OAL_IP_ADDR_MAX_SIZE, pst_ip_addr, OAL_IPV4_ADDR_SIZE);
        l_ret += memcpy_s(st_ip_addr_config.auc_mask_addr, OAL_IP_ADDR_MAX_SIZE, pst_mask_addr, OAL_IPV4_ADDR_SIZE);
    } else if (en_type == DMAC_CONFIG_IPV6) {
        l_ret += memcpy_s(st_ip_addr_config.auc_ip_addr, OAL_IP_ADDR_MAX_SIZE, pst_ip_addr, OAL_IPV6_ADDR_SIZE);
    } else {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR,
                       "{hmac_arp_offload_set_ip_addr::The type[%d] is error.}", en_type);
        return OAL_ERR_CODE_MAGIC_NUM_FAIL;
    }
    if (l_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_PWR, "hmac_arp_offload_set_ip_addr::memcpy fail!");
        return OAL_FAIL;
    }
    st_ip_addr_config.en_type = en_type;

    if (OAL_UNLIKELY(en_oper >= DMAC_IP_OPER_BUTT)) {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR,
                       "{hmac_arp_offload_set_ip_addr::The operation[%d] is error.}", en_type);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }
    st_ip_addr_config.en_oper = en_oper;

    ul_ret = hmac_config_set_ip_addr(pst_mac_vap,
                                     OAL_SIZEOF(dmac_ip_addr_config_stru),
                                     (oal_uint8 *)&st_ip_addr_config);
    return ul_ret;
}
#endif

