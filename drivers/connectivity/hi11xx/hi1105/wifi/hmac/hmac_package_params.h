/*
 * ��Ȩ���� (c) ��Ϊ�������޹�˾ 2020-2020
 * ����˵�� :
 * �������� : 2020��6��17��
 */

#ifndef __HMAC_PACKAGE_PARAMS_H__
#define __HMAC_PACKAGE_PARAMS_H__
#include "oal_ext_if.h"
#include "mac_vap.h"

#define CMD_DEVICE_PARAMS_MAX_LEN 100 /* ����ת��Ϊdevice���������󳤶� */
uint32_t hmac_package_uint8_params(
    mac_vap_stru *mac_vap, uint32_t *params, uint8_t *new_params, uint16_t *new_params_len);
uint32_t hmac_package_uint16_params(
    mac_vap_stru *mac_vap, uint32_t *params, uint8_t *new_params, uint16_t *new_params_len);
uint32_t hmac_package_uint32_params(
    mac_vap_stru *mac_vap, uint32_t *params, uint8_t *new_params, uint16_t *new_params_len);
uint32_t hmac_package_tlv_params(
    mac_vap_stru *mac_vap, uint32_t *params, uint8_t *new_params, uint16_t *new_params_len);
#endif /* end of hmac_arp_offload.h */
