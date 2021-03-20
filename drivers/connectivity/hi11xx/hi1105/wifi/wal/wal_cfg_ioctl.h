/*
 * ��Ȩ���� (c) ��Ϊ�������޹�˾ 2020-2020
 * ����˵�� :
 * �������� : 2020��6��17��
 */

#ifndef __WAL_CFG_IOCTL_H__
#define __WAL_CFG_IOCTL_H__
#include "mac_vap.h"
#include "oal_ext_if.h"

#define CMD_NAME_MAX_LEN  80 /* �ַ�����ÿ�����ʵ���󳤶�(ԭ20) */
#define CMD_VALUE_MAX_LEN 10 /* �ַ�����ĳ����Ӧ����ȡֵ�����λ�� */
#define CMD_PARAMS_MAX_CNT 20 /* �������������� */
#define MIN_VALUE_INDEX 0 /* ���������Сֵ��index */
#define MAX_VALUE_INDEX 1 /* ����������ֵ��index */
#define PARAM_CHECK_TABLE_LEN 2 /* ÿ����������������ֵ���ֵ����Сֵ */
typedef uint32_t (*package_params_func)(
    mac_vap_stru *mac_vap, uint32_t *params, uint8_t *new_params, uint16_t *new_params_len);
typedef uint32_t (*process_cmd_func)(mac_vap_stru *mac_vap, uint32_t *params);
typedef struct {
    uint8_t *name;   /* ����ؼ��� */
    uint8_t  param_num; /* ����������� */
    int32_t  *param_check_table; /* ������Ч�Լ��� */
    process_cmd_func process_cmd; /* host���������ָ�� */
    package_params_func package_params; /* ������װ���� */
    oal_bool_enum_uint8 to_device; /* �Ƿ��·���device */
    uint32_t cfg_id; /* ���¼���ID�� */
} host_cmd_stru;
uint32_t wal_process_cmd(int8_t *cmd);
#endif

