/*
 * 版权所有 (c) 华为技术有限公司 2020-2020
 * 功能说明 :
 * 创建日期 : 2020年6月17日
 */
#include "wal_cfg_ioctl.h"
#include "oal_types.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "mac_vap.h"
#include "wal_config.h"
#include "hmac_package_params.h"
#include "hmac_vap.h"
#include "hmac_scan.h"
#include "oal_util.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_CFG_IOCTL_C
extern uint32_t wal_get_cmd_one_arg(int8_t *pc_cmd, int8_t *pc_arg,
    uint32_t ul_arg_len, uint32_t *pul_cmd_offset);
uint32_t wal_get_host_cmd_table_size(void);
host_cmd_stru *wal_get_cmd_info(uint32_t cmd_id);

#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
/* host处理函数 */
OAL_STATIC uint32_t wal_set_hid2d_debug_switch(mac_vap_stru *mac_vap, uint32_t *params)
{
    hmac_vap_stru *hmac_vap;
    uint32_t params_index = 0;
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_process_hid2d_debug_switch::hmac_vap is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    g_st_hid2d_debug_switch.uc_hid2d_debug_en = (uint8_t)params[params_index++];
    g_st_hid2d_debug_switch.uc_hid2d_delay_time = (uint8_t)params[params_index++];
    hmac_vap->en_is_hid2d_state = g_st_hid2d_debug_switch.uc_hid2d_debug_en;
    oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_CFG, "{set_hid2d_debug_switch::ENABLE LEVEL:%d,delay time %d.}",
        g_st_hid2d_debug_switch.uc_hid2d_debug_en, g_st_hid2d_debug_switch.uc_hid2d_delay_time);
    return OAL_SUCC;
}
OAL_STATIC uint32_t wal_set_hid2d_state(mac_vap_stru *mac_vap, uint32_t *params)
{
    hmac_vap_stru *hmac_vap;
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_set_hid2d_state::hmac_vap is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    hmac_vap->en_is_hid2d_state = (uint8_t)params[0];
    return OAL_SUCC;
}
#endif
OAL_STATIC uint32_t wal_set_bgscan_type(mac_vap_stru *mac_vap, uint32_t *params)
{
    hmac_scan_state_enum_uint8 bgscan_state;
    bgscan_state = (hmac_scan_state_enum_uint8)params[0];
    return hmac_bgscan_enable(mac_vap, OAL_SIZEOF(bgscan_state), &bgscan_state);
}


uint32_t wal_params_check(uint32_t cmd_id, int32_t *params)
{
    host_cmd_stru *cmd_info = wal_get_cmd_info(cmd_id);
    uint32_t index;
    int32_t *check_table = NULL;
    /* 如果check_table为空说明参数不需要检查 */
    if (cmd_info->param_check_table == NULL) {
        return OAL_SUCC;
    }
    check_table = cmd_info->param_check_table;
    for (index = 0; index < cmd_info->param_num; index++) {
        /* 小于最小值, 大于最大值,为非法值 */
        if ((params[index] < check_table[index * PARAM_CHECK_TABLE_LEN + MIN_VALUE_INDEX]) ||
            (params[index] > check_table[index * PARAM_CHECK_TABLE_LEN + MAX_VALUE_INDEX])) {
            oam_error_log4(0, OAM_SF_ANY, "wal_params_check:check fail,cmd_id=%d, param=%d,min=%d,max=%d",
                cmd_id, params[index], check_table[index * PARAM_CHECK_TABLE_LEN + MIN_VALUE_INDEX],
                check_table[index * PARAM_CHECK_TABLE_LEN + MAX_VALUE_INDEX]);
            return OAL_FAIL;
        }
    }
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_get_cmd_vap(int8_t *cmd, mac_vap_stru **pp_mac_vap, uint32_t *offset)
{
    oal_net_device_stru *netdev = OAL_PTR_NULL;
    int8_t dev_name[CMD_NAME_MAX_LEN];
    uint32_t ret;

    if (oal_any_null_ptr3(cmd, pp_mac_vap, offset)) {
        oam_error_log3(0, OAM_SF_ANY, "{wal_get_cmd_vap::cmd/pp_mac_vap/pul_off_set null[%x][%x][%x]}",
            (uintptr_t)cmd, (uintptr_t)pp_mac_vap, (uintptr_t)offset);
        return OAL_ERR_CODE_PTR_NULL;
    }
    ret = wal_get_cmd_one_arg(cmd, dev_name, CMD_NAME_MAX_LEN, offset);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_get_cmd_vap::get cmd one arg err[%d]!}", ret);
        return ret;
    }
    /* 根据dev_name找到dev */
    netdev = wal_config_get_netdev(dev_name, OAL_STRLEN(dev_name));
    if (netdev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_get_cmd_vap::wal_config_get_netdev return null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */
    oal_dev_put(netdev);
    *pp_mac_vap = oal_net_dev_priv(netdev);
    if (*pp_mac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "wal_get_cmd_vap:*pp_mac_vap is null");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}


uint32_t wal_get_cmd_id(int8_t *cmd, uint32_t *cmd_id, uint32_t *offset)
{
    int8_t cmd_name[CMD_NAME_MAX_LEN];
    uint32_t ret;
    uint32_t cmd_index;
    uint32_t host_cmd_table_size = wal_get_host_cmd_table_size();
    host_cmd_stru *cmd_info = NULL;
    ret = wal_get_cmd_one_arg(cmd, cmd_name, CMD_NAME_MAX_LEN, offset);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_get_cmd_id::get cmd one arg err[%d]!}", ret);
        return ret;
    }
    for (cmd_index = 0; cmd_index < host_cmd_table_size; cmd_index++) {
        cmd_info = wal_get_cmd_info(cmd_index);
        if (oal_strcmp(cmd_info->name, cmd_name) == 0) {
            *cmd_id = cmd_index;
            return OAL_SUCC;
        }
    }
    oam_warning_log0(0, 0, "wal_get_cmd_id:cannot find cmd_id");
    return OAL_FAIL;
}


uint32_t wal_get_cmd_params(int8_t *cmd, uint32_t cmd_id, int32_t *params)
{
    uint32_t param_index;
    uint32_t offset = 0;
    uint32_t ret = 0;
    int8_t arg[CMD_VALUE_MAX_LEN];
    host_cmd_stru *cmd_info = wal_get_cmd_info(cmd_id);
    /* 获取参数 */
    for (param_index = 0; param_index < cmd_info->param_num; param_index++) {
        ret = wal_get_cmd_one_arg(cmd, arg, CMD_VALUE_MAX_LEN, &offset);
        if (ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_process_cmd_params::get cmd one arg err[%d]!}", ret);
            return ret;
        }
        params[param_index] = (uint32_t)oal_atoi(arg);
        cmd += offset;
    }
    return OAL_SUCC;
}

uint32_t wal_send_cmd_params_to_device(mac_vap_stru *mac_vap, host_cmd_stru *cmd_info, int32_t *params, uint32_t cmd_id)
{
    uint16_t new_params_len = 0;
    uint8_t  new_params[CMD_DEVICE_PARAMS_MAX_LEN] = {0};
    uint32_t ret;

    if (cmd_info->package_params == NULL) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_ANY,
            "wal_send_cmd_params_to_device:cmd_id[%d],package_params func is null!", cmd_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 参数组装 */
    ret = cmd_info->package_params(mac_vap, params, new_params, &new_params_len);
    if (ret != OAL_SUCC) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_ANY,
            "wal_send_cmd_params_to_device:cmd_id[%d],package_params fail!", cmd_id);
        return ret;
    }
    /* 抛事件 */
    ret = hmac_config_send_event(mac_vap, cmd_info->cfg_id, new_params_len, new_params);
    if (ret != OAL_SUCC) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_ANY,
            "wal_send_cmd_params_to_device:cmd_id[%d],send event fail!", cmd_id);
    }
    return ret;
}

uint32_t wal_process_cmd(int8_t *cmd)
{
    uint32_t offset = 0;
    mac_vap_stru *mac_vap = NULL;
    uint32_t ret;
    uint32_t cmd_id;
    int32_t  params[CMD_PARAMS_MAX_CNT] = {0};
    host_cmd_stru *cmd_info = NULL;

    /* 查找mac_vap */
    ret = wal_get_cmd_vap(cmd, &mac_vap, &offset);
    if (wal_get_cmd_vap(cmd, &mac_vap, &offset) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "wal_process_cmd:find vap fail");
        return ret;
    }
    cmd += offset;
    /* 查找命令ID */
    ret = wal_get_cmd_id(cmd, &cmd_id, &offset);
    if (ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "wal_process_cmd:find id fail");
        return ret;
    }
    cmd += offset;
    /* 处理参数 */
    ret = wal_get_cmd_params(cmd, cmd_id, params);
    if (ret != OAL_SUCC) {
        return ret;
    }
    /* 检查参数有效性 */
    ret = wal_params_check(cmd_id, params);
    if (ret != OAL_SUCC) {
        return ret;
    }
    cmd_info = wal_get_cmd_info(cmd_id);
    /* host命令处理 */
    if (cmd_info->process_cmd != NULL) {
        ret = cmd_info->process_cmd(mac_vap, params);
        if (ret != OAL_SUCC) {
            oam_error_log1(mac_vap->uc_vap_id, OAM_SF_ANY, "wal_process_cmd:cmd_id[%d],cmd process fail!", cmd_id);
            return ret;
        }
    }
    /* 判断是否抛事件到device */
    if (cmd_info->to_device == OAL_FALSE) {
        return OAL_SUCC;
    }
    /* 抛事件 */
    ret = wal_send_cmd_params_to_device(mac_vap, cmd_info, params, cmd_id);
    if (ret != OAL_SUCC) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_ANY, "wal_process_cmd:cmd_id[%d],send event fail!", cmd_id);
        return ret;
    }
    return ret;
}

#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
/* 参数检查列表 */
int32_t hid2d_debug_switch_check_table[] = {
    /* hid2d_debug_en */
    0, 1,
    /* hid2d_delay_time */
    0, 256 };
#endif
host_cmd_stru host_cmd_table[] = {
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
    { "hid2d_debug_switch", 2, hid2d_debug_switch_check_table, wal_set_hid2d_debug_switch,
    hmac_package_tlv_params, OAL_TRUE, WLAN_CFGID_HID2D_DEBUG_SWITCH},
    { "hid2d_switch", 1, NULL, wal_set_hid2d_state,
    hmac_package_uint8_params, OAL_TRUE, WLAN_CFGID_SET_HID2D_STATE },
#endif
    { "bgscan_enable", 1, NULL, wal_set_bgscan_type, NULL, OAL_FALSE, 0 },
    { "arp_offload_enable", 1, NULL, NULL, hmac_package_uint8_params, OAL_TRUE, WLAN_CFGID_ENABLE_ARP_OFFLOAD}
};
uint32_t wal_get_host_cmd_table_size(void)
{
    return oal_array_size(host_cmd_table);
}
host_cmd_stru *wal_get_cmd_info(uint32_t cmd_id)
{
    if (cmd_id >= oal_array_size(host_cmd_table)) {
        oam_error_log2(0, OAM_SF_ANY, "hmac_get_cmd_info:cmd_id=%d > %d is error",
            cmd_id, oal_array_size(host_cmd_table));
        return NULL;
    }
    return &host_cmd_table[cmd_id];
}
