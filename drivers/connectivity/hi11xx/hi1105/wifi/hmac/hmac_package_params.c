/*
 * 版权所有 (c) 华为技术有限公司 2020-2020
 * 功能说明 :
 * 创建日期 : 2020年6月17日
 */
#include "hmac_package_params.h"
#include "oal_ext_if.h"
#include "hmac_vap.h"
#include "mac_vap_common.h"
#include "hmac_main.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_PACKAGE_PARAMS_C
uint32_t hmac_package_uint8_params(
    mac_vap_stru *mac_vap, uint32_t *params, uint8_t *new_params, uint16_t *new_params_len)
{
    new_params[0] = (uint8_t)params[0];
    *new_params_len = OAL_SIZEOF(uint8_t);
    return OAL_SUCC;
}
uint32_t hmac_package_uint16_params(
    mac_vap_stru *mac_vap, uint32_t *params, uint8_t *new_params, uint16_t *new_params_len)
{
    *(uint16_t *)new_params = (uint16_t)params[0];
    *new_params_len = OAL_SIZEOF(uint16_t);
    return OAL_SUCC;
}
uint32_t hmac_package_uint32_params(
    mac_vap_stru *mac_vap, uint32_t *params, uint8_t *new_params, uint16_t *new_params_len)
{
    *(uint32_t*)new_params = params[0];
    *new_params_len = OAL_SIZEOF(uint32_t);
    return OAL_SUCC;
}
uint32_t hmac_package_tlv_params(
    mac_vap_stru *mac_vap, uint32_t *params, uint8_t *new_params, uint16_t *new_params_len)
{
    mac_cfg_set_tlv_stru *tlv_params = NULL;
    if (OAL_SIZEOF(mac_cfg_set_tlv_stru) > CMD_DEVICE_PARAMS_MAX_LEN) {
        oam_error_log1(0, OAM_SF_ANY, "hmac_package_tlv_params:mac_cfg_set_tlv_stru size = %d is too long",
            OAL_SIZEOF(mac_cfg_set_tlv_stru));
    }
    tlv_params = (mac_cfg_set_tlv_stru *)new_params;
    tlv_params->uc_cmd_type = (uint8_t)params[0];
    tlv_params->ul_value = (uint32_t)params[1];
    *new_params_len = OAL_SIZEOF(mac_cfg_set_tlv_stru);
    return OAL_SUCC;
}
