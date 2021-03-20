/*
 * 版权所有 (c) 华为技术有限公司 2020-2020
 * 功能说明 :
 * 创建日期 : 2020年6月17日
 */

#ifndef __HMAC_PACKAGE_PARAMS_H__
#define __HMAC_PACKAGE_PARAMS_H__
#include "oal_ext_if.h"
#include "mac_vap.h"

#define CMD_DEVICE_PARAMS_MAX_LEN 100 /* 命令转换为device参数后的最大长度 */
uint32_t hmac_package_uint8_params(
    mac_vap_stru *mac_vap, uint32_t *params, uint8_t *new_params, uint16_t *new_params_len);
uint32_t hmac_package_uint16_params(
    mac_vap_stru *mac_vap, uint32_t *params, uint8_t *new_params, uint16_t *new_params_len);
uint32_t hmac_package_uint32_params(
    mac_vap_stru *mac_vap, uint32_t *params, uint8_t *new_params, uint16_t *new_params_len);
uint32_t hmac_package_tlv_params(
    mac_vap_stru *mac_vap, uint32_t *params, uint8_t *new_params, uint16_t *new_params_len);
#endif /* end of hmac_arp_offload.h */
