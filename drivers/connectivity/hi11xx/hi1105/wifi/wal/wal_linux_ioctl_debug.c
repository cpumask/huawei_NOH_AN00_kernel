

#ifdef _PRE_WLAN_CFGID_DEBUG

#include "oal_ext_if.h"
#include "oal_kernel_file.h"
#include "oal_cfg80211.h"
#include "oal_util.h"

#include "oam_ext_if.h"
#include "frw_ext_if.h"

#include "wlan_spec.h"
#include "wlan_types.h"

#include "mac_vap.h"
#include "mac_resource.h"
#include "mac_regdomain.h"
#include "mac_ie.h"
#include "hmac_ext_if.h"
#include "hmac_chan_mgmt.h"
#include "wal_main.h"
#include "wal_ext_if.h"
#include "wal_config.h"
#include "wal_regdb.h"
#include "wal_linux_scan.h"
#include "wal_linux_ioctl.h"
#include "wal_linux_bridge.h"
#include "wal_linux_flowctl.h"
#include "wal_linux_atcmdsrv.h"
#include "wal_linux_event.h"
#include "hmac_resource.h"
#include "hmac_p2p.h"

#include "wal_linux_cfg80211.h"

#include "wal_dfx.h"

#include "oal_hcc_host_if.h"
#include "plat_cali.h"

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include <linux/notifier.h>
#include <linux/inetdevice.h>
#include <net/addrconf.h>
#endif
#include "hmac_arp_offload.h"

#include "hmac_auto_adjust_freq.h"

#include "hmac_roam_main.h"
#include "hisi_customize_wifi.h"

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif

#ifdef _PRE_MEM_TRACE
#include "mem_trace.h"
#endif

#if defined(WIN32)
#include "hisi_customize_wifi_hi110x.h"
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "board.h"
#endif
#include "securec.h"
#include "securectype.h"
#include "wal_linux_netdev_ops.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID      OAM_FILE_ID_WAL_LINUX_IOCTL_DEBUG_C
#define MAX_PRIV_CMD_SIZE 4096
#define MCM_CONFIG_CHAIN_STEP 10
#define MCM_CHAIN_NUM_MAX 4

/* 2 结构体定义 */
/* 3 全局变量定义 */
#ifdef _PRE_WLAN_FEATURE_DFR
extern hmac_dfr_info_stru g_st_dfr_info;
#endif  // _PRE_WLAN_FEATURE_DFR
extern OAL_CONST int8_t *pauc_tx_dscr_param_name[];
extern OAL_CONST int8_t *pauc_tx_pow_param_name[];
extern OAL_CONST wal_ioctl_alg_cfg_stru g_ast_alg_cfg_map[];
extern OAL_CONST wal_ioctl_dyn_cali_stru g_ast_dyn_cali_cfg_map[];
extern OAL_CONST wal_ioctl_tlv_stru g_ast_set_tlv_table[];
/* 4 函数实现 */

OAL_STATIC uint32_t wal_hipriv_global_log_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    int32_t l_switch_val;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;

    /* 获取开关状态值 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_global_log_switch::error code[%d]}\r\n", ul_ret);
        return ul_ret;
    }

    if ((0 != oal_strcmp("0", ac_name)) && (0 != oal_strcmp("1", ac_name))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_global_log_switch::invalid switch value}\r\n");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    l_switch_val = oal_atoi(ac_name);

    return oam_log_set_global_switch((oal_switch_enum_uint8)l_switch_val);
}


OAL_STATIC uint32_t wal_hipriv_feature_log_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oam_feature_enum_uint8 en_feature_id;
    uint8_t uc_switch_vl;
    uint32_t ul_off_set;
    int8_t ac_param[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    oam_log_level_enum_uint8 en_log_lvl;
    wal_msg_write_stru st_write_msg;

    /* OAM log模块的开关的命令: hipriv "Hisilicon0[vapx] feature_log_switch {feature_name} {0/1}"
       1-2(error与warning)级别日志以vap级别为维度；
    */

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_feature_log_switch::null pointer.}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获取特性名称 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_param, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }
    pc_param += ul_off_set;

    /* 提供特性名的帮助信息 */
    if ('?' == ac_param[0]) {
        oal_io_print("please input abbr feature name. \r\n");
        oam_show_feature_list();
        return OAL_SUCC;
    }

    /* 获取特性ID */
    ul_ret = oam_get_feature_id((uint8_t *)ac_param, &en_feature_id);
    if (ul_ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_feature_log_switch::invalid feature name}\r\n");
        return ul_ret;
    }

    /* 获取开关值 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_param, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }
    pc_param += ul_off_set;

    /* 获取INFO级别开关状态 */
    if ((0 != oal_strcmp("0", ac_param)) && (0 != oal_strcmp("1", ac_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_feature_log_switch::invalid switch value}\r\n");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }
    uc_switch_vl = (uint8_t)oal_atoi(ac_param);

    /* 关闭INFO日志级别时，恢复成默认的日志级别 */
    en_log_lvl = (uc_switch_vl == OAL_SWITCH_ON) ? OAM_LOG_LEVEL_INFO : OAM_LOG_DEFAULT_LEVEL;
    ul_ret = oam_log_set_feature_level(pst_mac_vap->uc_vap_id, en_feature_id, en_log_lvl);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_FEATURE_LOG, OAL_SIZEOF(int32_t));
    *((uint16_t *)(st_write_msg.auc_value)) = ((en_feature_id << 8) | en_log_lvl);
    ul_ret |= (uint32_t)wal_send_cfg_event(pst_net_dev,
                                                 WAL_MSG_TYPE_WRITE,
                                                 WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                                 (uint8_t *)&st_write_msg,
                                                 OAL_FALSE,
                                                 OAL_PTR_NULL);

    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_feature_log_switch::return err code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    return ul_ret;
}


OAL_STATIC uint32_t wal_hipriv_pm_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_tmp;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;

    /* OAM event模块的开关的命令: hipriv "Hisilicon0 wal_hipriv_pm_switch 0 | 1"
        此处将解析出"1"或"0"存入ac_name
    */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_pm_switch::wal_get_cmd_one_arg return err_code[%d]}\r\n", ul_ret);
        return ul_ret;
    }

    /* 针对解析出的不同命令，对event模块进行不同的设置 */
    l_tmp = (uint8_t)oal_atoi(ac_name);

    if (l_tmp < 0 || l_tmp > 5) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_pm_switch::the log switch command is error [%x]!}\r\n", (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_PM_SWITCH, OAL_SIZEOF(int32_t));
    *((int32_t *)(st_write_msg.auc_value)) = l_tmp; /* 设置配置命令参数 */

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_pm_switch::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_power_test(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_tmp;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;

    /* OAM event模块的开关的命令: hipriv "Hisilicon0 wal_hipriv_power_test 0 | 1"
        此处将解析出"1"或"0"存入ac_name
    */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_pm_switch::wal_get_cmd_one_arg return err_code[%d]}\r\n", ul_ret);
        return ul_ret;
    }

    /* 针对解析出的不同命令，对event模块进行不同的设置 */
    l_tmp = (uint8_t)oal_atoi(ac_name);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_POWER_TEST, OAL_SIZEOF(int32_t));
    *((int32_t *)(st_write_msg.auc_value)) = l_tmp; /* 设置配置命令参数 */

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_pm_switch::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint8_t wal_get_tx_pow_param(int8_t *pc_param, uint8_t uc_param_len)
{
    wal_tx_pow_param_enum_uint8 en_param_index;

    /* 解析是设置哪一个字段 */
    for (en_param_index = 0; en_param_index < WAL_TX_POW_PARAM_BUTT; en_param_index++) {
        if (uc_param_len <= OAL_STRLEN(pauc_tx_pow_param_name[en_param_index])) {
            return WAL_TX_POW_PARAM_BUTT;
        }

        if (!oal_strcmp(pauc_tx_pow_param_name[en_param_index], pc_param)) {
            break;
        }
    }

    return en_param_index;
}


OAL_STATIC void wal_get_tx_pow_log_param(mac_cfg_set_tx_pow_param_stru *pst_set_tx_pow_param, int8_t *pc_param)
{
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_off_set;

    if (pst_set_tx_pow_param->auc_value[0] == 0) {
        // pow
        if (pst_set_tx_pow_param->auc_value[1] >= HAL_POW_RATE_CODE_TBL_LEN) {
            // rate_idx
            oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::err rate idx [%d]!}", pst_set_tx_pow_param->auc_value[1]);
            pst_set_tx_pow_param->auc_value[1] = 0;
        }
    } else {
        // evm
        if (OAL_SUCC != wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set)) {
            oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::evm arg err input!}");
            return;
        }
        // chn_idx
        pst_set_tx_pow_param->auc_value[2] = (uint8_t)oal_atoi(ac_arg);
    }

    return;
}


extern oal_bool_enum_uint8 g_tas_switch_en[];
OAL_STATIC uint32_t wal_hipriv_set_tx_pow_param(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    mac_cfg_set_tx_pow_param_stru *pst_set_tx_pow_param = OAL_PTR_NULL;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret = OAL_SUCC;
    int32_t l_sec_ret = EOK;
    wal_tx_pow_param_enum_uint8 en_param_index;
    uint8_t uc_value = 0;
#ifdef _PRE_WLAN_FEATURE_TPC_OPT
    uint8_t auc_sar_ctrl_params[CUS_NUM_OF_SAR_PARAMS][WLAN_RF_CHANNEL_NUMS];
    uint8_t *puc_sar_ctrl_params;
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    mac_device_stru *pst_mac_device;
    mac_cfg_tas_pwr_ctrl_stru st_tas_pow_ctrl_params;
#endif
#endif

    /* 命令格式: hipriv "vap0 set_tx_pow rf_reg_ctl 0/1" ,   0:不使能, 1:使能            */
    /* 命令格式: hipriv "vap0 set_tx_pow fix_level 0/1/2/3"  设置数据帧功率等级, 仅data0 */
    /* 命令格式: hipriv "vap0 set_tx_pow mag_level 0/1/2/3"  设置管理帧功率等级          */
    /* 命令格式: hipriv "vap0 set_tx_pow ctl_level 0/1/2/3"  设置控制帧功率等级          */
    /* 命令格式: hipriv "vap0 set_tx_pow amend <value>"      修正upc code                */
    /* 命令格式: hipriv "vap0 set_tx_pow no_margin"          功率不留余量设置, 仅51用    */
    /* 命令格式: hipriv "vap0 set_tx_pow show_log type (bw chn)" 显示功率维测日志
       type: 0/1 pow/evm
       bw:   rate_idx(pow) 0/1/2 20M/80M/160M(evm)
       chn:  0~6(20M/80M) 0/1(160M)  */
    /* 命令格式: hipriv "vap0 set_tx_pow sar_level 0/1/2/3"  设置降sar等级               */
    /* 命令格式: hipriv "vap0 set_tx_pow tas_pwr_ctrl 0/1 0/1" tas功率控制               */
    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_TX_POW, OAL_SIZEOF(mac_cfg_set_tx_pow_param_stru));

    /* 解析并设置配置命令参数 */
    pst_set_tx_pow_param = (mac_cfg_set_tx_pow_param_stru *)(st_write_msg.auc_value);
    memset_s(pst_set_tx_pow_param, OAL_SIZEOF(mac_cfg_set_tx_pow_param_stru), 0, OAL_SIZEOF(mac_cfg_set_tx_pow_param_stru));

    /* 获取描述符字段设置命令字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    /* 解析是设置哪一个字段 */
    en_param_index = wal_get_tx_pow_param(ac_arg, sizeof(ac_arg));

    /* 检查命令是否打错 */
    if (en_param_index == WAL_TX_POW_PARAM_BUTT) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::no such param for tx pow!}\r\n");
        return OAL_FAIL;
    }

    pst_set_tx_pow_param->en_type = en_param_index;

    /* 获取下一个参数 */
    pc_param += ul_off_set;
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
    } else {
        uc_value = (uint8_t)oal_atoi(ac_arg);
    }
    pst_set_tx_pow_param->auc_value[0] = uc_value;

    /*lint -e571 */
    /* 参数校验 */
    switch (en_param_index) {
        case WAL_TX_POW_PARAM_SET_RF_REG_CTL:
        case WAL_TX_POW_PARAM_SET_NO_MARGIN:
            if (uc_value >= 2) {
                return OAL_ERR_CODE_INVALID_CONFIG;
            }
            break;
        case WAL_TX_POW_PARAM_SET_FIX_LEVEL:
        case WAL_TX_POW_PARAM_SET_MAG_LEVEL:
        case WAL_TX_POW_PARAM_SET_CTL_LEVEL:
            if (uc_value > 4) {
                return OAL_ERR_CODE_INVALID_CONFIG;
            }
            break;

        case WAL_TX_POW_PARAM_SET_SHOW_LOG:
            /* 获取下一个参数 */
            pc_param += ul_off_set;
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::show log arg1 missing!}");
                break;
            }
            pst_set_tx_pow_param->auc_value[1] = (uint8_t)oal_atoi(ac_arg);
            /* to reduce cycle complexity */
            pc_param += ul_off_set;
            wal_get_tx_pow_log_param(pst_set_tx_pow_param, pc_param);
            break;

        case WAL_TX_POW_PARAM_SET_AMEND:
        case WAL_TX_POW_PARAM_SHOW_TPC_TABLE_GAIN:
        case WAL_TX_POW_POW_SAVE:
        case WAL_TX_POW_SET_TPC_IDX:
            break;

        case WAL_TX_POW_PARAM_SET_SAR_LEVEL:
            oam_warning_log1(0, OAM_SF_TPC, "{wal_hipriv_set_tx_pow_param::input reduce SAR level [%d]!}\r\n", uc_value);
#ifdef _PRE_WLAN_FEATURE_TPC_OPT
            if ((uc_value > CUS_NUM_OF_SAR_LVL) || (uc_value == 0)) {
                oam_error_log0(0, OAM_SF_TPC, "{wal_hipriv_set_tx_pow_param::reduce SAR STOP!}\r\n");
                memset_s(auc_sar_ctrl_params, OAL_SIZEOF(auc_sar_ctrl_params), 0xFF, OAL_SIZEOF(auc_sar_ctrl_params));
                puc_sar_ctrl_params = &auc_sar_ctrl_params[0][0];
            } else {
                puc_sar_ctrl_params = wal_get_reduce_sar_ctrl_params(uc_value);
            }

            if (puc_sar_ctrl_params) {
                l_sec_ret = memcpy_s(pst_set_tx_pow_param->auc_value, sizeof(pst_set_tx_pow_param->auc_value), puc_sar_ctrl_params, sizeof(auc_sar_ctrl_params));
            }
#endif
            break;

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
        case WAL_TX_POW_PARAM_TAS_POW_CTRL:
            pst_mac_device = mac_res_get_dev(0);
            /* 如果非单VAP,则不处理 */
            if (mac_device_calc_up_vap_num(pst_mac_device) > 1) {
                oam_error_log0(0, OAM_SF_ANY, "wal_hipriv_set_tx_pow_param::WAL_TX_POW_PARAM_TAS_POW_CTRL more than 1 vap");
                return OAL_FAIL;
            }

            memset_s(&st_tas_pow_ctrl_params, OAL_SIZEOF(mac_cfg_tas_pwr_ctrl_stru),
                     0, OAL_SIZEOF(mac_cfg_tas_pwr_ctrl_stru));
            st_tas_pow_ctrl_params.uc_core_idx = !!uc_value;

            if (g_tas_switch_en[!!uc_value] == OAL_FALSE) {
                return OAL_FAIL;
            }

            /* 获取下一个参数 */
            pc_param += ul_off_set;
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::TAS pwr ctrl core or needimproved lost return err_code [%d]!}\r\n");
                return OAL_ERR_CODE_INVALID_CONFIG;
            }
            st_tas_pow_ctrl_params.en_need_improved = (uint8_t) !!oal_atoi(ac_arg);
            l_sec_ret = memcpy_s(pst_set_tx_pow_param->auc_value, sizeof(pst_set_tx_pow_param->auc_value),
                                 &st_tas_pow_ctrl_params, OAL_SIZEOF(mac_cfg_tas_pwr_ctrl_stru));

            oam_warning_log2(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::WAL_TX_POW_PARAM_TAS_POW_CTRL core[%d] improved_flag[%d]!}\r\n",
                             st_tas_pow_ctrl_params.uc_core_idx, st_tas_pow_ctrl_params.en_need_improved);
            break;

        case WAL_TX_POW_PARAM_TAS_RSSI_MEASURE:
            if (g_tas_switch_en[!!uc_value] == OAL_FALSE) {
                oam_error_log1(0, OAM_SF_ANY, "wal_hipriv_set_tx_pow_param::WAL_TX_POW_PARAM_TAS_RSSI_MEASURE core[%d]!", uc_value);
                return OAL_FAIL;
            }
            break;

        case WAL_TX_POW_PARAM_TAS_ANT_SWITCH:
            if (oal_any_true_value2(g_tas_switch_en[WLAN_RF_CHANNEL_ZERO], g_tas_switch_en[WLAN_RF_CHANNEL_ONE])) {
                /* 0:默认态 1:tas态 */
                oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::CMD_SET_MEMO_CHANGE antIndex[%d].}", uc_value);
                l_ret = board_wifi_tas_set(uc_value);
            }
            return (uint32_t)l_ret;
#endif
        case WAL_TX_POW_GET_PD_INFO:
#ifdef _PRE_WLAN_FEATURE_FULL_QUAN_PROD_CAL
            /* 表示当前开始产线全量校准的数据采集,获取下一个参数状态 */
            if (pst_set_tx_pow_param->auc_value[0] == 1) {
                pc_param += ul_off_set;
                if (OAL_SUCC == wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set)) {
                    pst_set_tx_pow_param->auc_value[1] = (uint8_t)oal_atoi(ac_arg);
                }
            }
            oam_warning_log2(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::pdet info param[%d %d]!}",
                             pst_set_tx_pow_param->auc_value[0], pst_set_tx_pow_param->auc_value[1]);
#endif
            break;

        default:
            oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::error input type!}\r\n");
            return OAL_FAIL;
    }
    /*lint +e571 */
    if (l_sec_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::memcpy fail!}");
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_set_tx_pow_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_ucast_data_dscr_param(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_set_dscr_param_stru *pst_set_dscr_param = OAL_PTR_NULL;
    wal_dscr_param_enum_uint8 en_param_index;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    /* 解析并设置配置命令参数 */
    pst_set_dscr_param = (mac_cfg_set_dscr_param_stru *)(st_write_msg.auc_value);

    /* 获取描述符字段设置命令字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ucast_data_dscr_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    /* 解析是设置哪一个字段 */
    for (en_param_index = 0; en_param_index < WAL_DSCR_PARAM_BUTT; en_param_index++) {
        if (!oal_strcmp(pauc_tx_dscr_param_name[en_param_index], ac_arg)) {
            break;
        }
    }

    /* 检查命令是否打错 */
    if (en_param_index == WAL_DSCR_PARAM_BUTT) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_ucast_data_dscr_param::CMD ERR!cmd type::wlan0 set_ucast_data CMD VALUE!}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->ta: set tx rts antenna![0] or [1]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->ra: set rx cts/ack/ba antenna![0] or [1]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->cc: set channel code![0][BCC] or [1][LDPC]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->power: set lpf/pa/upc/dac!}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->shortgi: set tx dscr short gi or long gi![0] or [1] }");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->preamble: set tx dscr preamble mode![0] or [1]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->rtscts: set tx dscr enable rts or not![0] or [1]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->lsigtxop: set tx dscr enable lsigtxop or not![0] or [1]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->smooth: set rx channel matrix with smooth or not![0] or [1]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->snding: set sounding mode![0][NON]-[1][NDP]-[2][STAGGERD]-[3][LEGACY]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->txbf: set txbf mode![0][NON]-[1][EXPLICIT]-[2][LEGACY]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->stbc: set STBC or not![0] or [1]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->rd_ess: expand spatial stream}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->dyn_bw: set rts/cts dynamic signaling or not![0][STATIC]-[1][DYNAMIC]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->dyn_bw_exist: set tx dscr dynamic when no ht exist!set[1]] }");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->ch_bw_exist: set tx dscr dynamic when no ht exist!set[1]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->rate: set 11a/b/g rate as rate table}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->mcs: set 11n rate as rate table mcs index}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->mcsac: set 11ac rate as rate table mcs index}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->mcsax: set 11ax rate as rate table mcs index}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->mcsax_er: set 11ax_er rate as rate table mcs index}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->nss: set 11ac nss mode [1][SINGLE]-[2][DOUBLE]-[3][TRIBLE]-[4][QUAD]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->bw: set tx dscr channel bw![20][20M]-[40][40M]-[d40][40MDUP]-[80][80M]-[d80][80MDUP]-[160][160M]-[d160][160MDUP]-[80_80][80+80M]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->ltf: set tx dscr ltf [0][1*]-[1][2*]-[2][4*]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->gi: set tx dscr gi [0][lgi]-[1][sgi]-[2][mgi]}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->txchain: set tx dscr txchain![1]CH0-[2]CH1-[3]DOUBLE}");
        oam_warning_log0(0, OAM_SF_ANY, "{CMD-->dcm: set tx dscr enable dcm![0] or [1] }");
        return OAL_FAIL;
    }

    pst_set_dscr_param->uc_function_index = en_param_index;

    /* 配置速率、空间流数、带宽 */
    if (en_param_index >= WAL_DSCR_PARAM_RATE && en_param_index <= WAL_DSCR_PARAM_BW) {
        ul_ret = wal_hipriv_process_rate_params(pst_net_dev, pc_param, pst_set_dscr_param);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ucast_data_dscr_param::wal_hipriv_process_ucast_params return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }
    } else {
        /* 解析要设置为多大的速率 */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ucast_data_dscr_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }
        pc_param += ul_off_set;
        pst_set_dscr_param->l_value = oal_strtol(ac_arg, OAL_PTR_NULL, 0);
    }

    /* 单播数据帧描述符设置 tpye = MAC_VAP_CONFIG_UCAST_DATA */
    pst_set_dscr_param->en_type = MAC_VAP_CONFIG_UCAST_DATA;
    oam_warning_log2(0, OAM_SF_ANY, "{wal_hipriv_set_ucast_data_dscr_param::en_param_index [%d]!,value[%d]}\r\n", pst_set_dscr_param->uc_function_index, pst_set_dscr_param->l_value);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_DSCR, OAL_SIZEOF(mac_cfg_set_dscr_param_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_set_dscr_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ucast_data_dscr_param::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_bcast_data_dscr_param(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_set_dscr_param_stru *pst_set_dscr_param = OAL_PTR_NULL;
    wal_dscr_param_enum_uint8 en_param_index;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_DSCR, OAL_SIZEOF(mac_cfg_set_dscr_param_stru));

    /* 解析并设置配置命令参数 */
    pst_set_dscr_param = (mac_cfg_set_dscr_param_stru *)(st_write_msg.auc_value);

    /* 获取描述符字段设置命令字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_bcast_data_dscr_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    /* 解析是设置哪一个字段 */
    for (en_param_index = 0; en_param_index < WAL_DSCR_PARAM_BUTT; en_param_index++) {
        if (!oal_strcmp(pauc_tx_dscr_param_name[en_param_index], ac_arg)) {
            break;
        }
    }

    /* 检查命令是否打错 */
    if (en_param_index == WAL_DSCR_PARAM_BUTT) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_bcast_data_dscr_param::no such param for tx dscr!}\r\n");
        return OAL_FAIL;
    }
    pst_set_dscr_param->uc_function_index = en_param_index;

    /* 配置速率、空间流数、带宽 */
    if (en_param_index >= WAL_DSCR_PARAM_RATE && en_param_index <= WAL_DSCR_PARAM_BW) {
        ul_ret = wal_hipriv_process_rate_params(pst_net_dev, pc_param, pst_set_dscr_param);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_bcast_data_dscr_param::wal_hipriv_process_ucast_params return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }
    } else {
        /* 解析要设置为多大的速率 */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_bcast_data_dscr_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }
        pc_param += ul_off_set;
        pst_set_dscr_param->l_value = oal_strtol(ac_arg, OAL_PTR_NULL, 0);
    }

    /* 广播数据帧描述符设置 tpye = MAC_VAP_CONFIG_BCAST_DATA */
    pst_set_dscr_param->en_type = MAC_VAP_CONFIG_BCAST_DATA;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_set_dscr_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_bcast_data_dscr_param::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_ucast_mgmt_dscr_param(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_set_dscr_param_stru *pst_set_dscr_param = OAL_PTR_NULL;
    wal_dscr_param_enum_uint8 en_param_index;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint8_t uc_band;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_DSCR, OAL_SIZEOF(mac_cfg_set_dscr_param_stru));

    /* 解析并设置配置命令参数 */
    pst_set_dscr_param = (mac_cfg_set_dscr_param_stru *)(st_write_msg.auc_value);

    /***************************************************************************
             sh hipriv.sh "vap0 set_ucast_mgmt data0 2 8389137"
    ***************************************************************************/
    /* 解析data0 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ucast_mgmt_dscr_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    /* 解析是设置哪一个字段 */
    for (en_param_index = 0; en_param_index < WAL_DSCR_PARAM_BUTT; en_param_index++) {
        if (!oal_strcmp(pauc_tx_dscr_param_name[en_param_index], ac_arg)) {
            break;
        }
    }

    /* 检查命令是否打错 */
    if (en_param_index == WAL_DSCR_PARAM_BUTT) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_ucast_mgmt_dscr_param::no such param for tx dscr!}\r\n");
        return OAL_FAIL;
    }

    pst_set_dscr_param->uc_function_index = en_param_index;

    /* 解析要设置为哪个频段的单播管理帧 2G or 5G */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ucast_mgmt_dscr_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    uc_band = (uint8_t)oal_atoi(ac_arg);

    /* 单播管理帧描述符设置 tpye = MAC_VAP_CONFIG_UCAST_MGMT 2为2G,否则为5G  */
    if (WLAN_BAND_2G == uc_band) {
        pst_set_dscr_param->en_type = MAC_VAP_CONFIG_UCAST_MGMT_2G;
    } else {
        pst_set_dscr_param->en_type = MAC_VAP_CONFIG_UCAST_MGMT_5G;
    }

    /* 配置速率、空间流数、带宽 */
    if (en_param_index >= WAL_DSCR_PARAM_RATE && en_param_index <= WAL_DSCR_PARAM_BW) {
        ul_ret = wal_hipriv_process_rate_params(pst_net_dev, pc_param, pst_set_dscr_param);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ucast_mgmt_dscr_param::wal_hipriv_process_ucast_params return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }
    } else {
        /* 解析要设置为多大的速率 */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ucast_mgmt_dscr_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }
        pc_param += ul_off_set;
        pst_set_dscr_param->l_value = oal_strtol(ac_arg, OAL_PTR_NULL, 0);
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_set_dscr_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ucast_mgmt_dscr_param::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_mbcast_mgmt_dscr_param(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_set_dscr_param_stru *pst_set_dscr_param = OAL_PTR_NULL;
    wal_dscr_param_enum_uint8 en_param_index;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint8_t uc_band;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_DSCR, OAL_SIZEOF(mac_cfg_set_dscr_param_stru));

    /* 解析并设置配置命令参数 */
    pst_set_dscr_param = (mac_cfg_set_dscr_param_stru *)(st_write_msg.auc_value);

    /***************************************************************************
             sh hipriv.sh "vap0 set_mcast_mgmt data0 5 8389137"
    ***************************************************************************/
    /* 解析data0 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mbcast_mgmt_dscr_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    /* 解析是设置哪一个字段 */
    for (en_param_index = 0; en_param_index < WAL_DSCR_PARAM_BUTT; en_param_index++) {
        if (!oal_strcmp(pauc_tx_dscr_param_name[en_param_index], ac_arg)) {
            break;
        }
    }

    /* 检查命令是否打错 */
    if (en_param_index == WAL_DSCR_PARAM_BUTT) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_mbcast_mgmt_dscr_param::no such param for tx dscr!}\r\n");
        return OAL_FAIL;
    }

    pst_set_dscr_param->uc_function_index = en_param_index;

    /* 解析要设置为哪个频段的单播管理帧 2G or 5G */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mbcast_mgmt_dscr_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    uc_band = (uint8_t)oal_atoi(ac_arg);

    /* 单播管理帧描述符设置 tpye = MAC_VAP_CONFIG_UCAST_MGMT 2为2G,否则为5G  */
    if (WLAN_BAND_2G == uc_band) {
        pst_set_dscr_param->en_type = MAC_VAP_CONFIG_MBCAST_MGMT_2G;
    } else {
        pst_set_dscr_param->en_type = MAC_VAP_CONFIG_MBCAST_MGMT_5G;
    }

    /* 配置速率、空间流数、带宽 */
    if (en_param_index >= WAL_DSCR_PARAM_RATE && en_param_index <= WAL_DSCR_PARAM_BW) {
        ul_ret = wal_hipriv_process_rate_params(pst_net_dev, pc_param, pst_set_dscr_param);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mbcast_mgmt_dscr_param::wal_hipriv_process_ucast_params return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }
    } else {
        /* 解析要设置为多大的速率 */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mbcast_mgmt_dscr_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }
        pc_param += ul_off_set;

        pst_set_dscr_param->l_value = oal_strtol(ac_arg, OAL_PTR_NULL, 0);
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_set_dscr_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mbcast_mgmt_dscr_param::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


uint32_t wal_hipriv_set_nss(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_tx_comp_stru *pst_set_nss_param = OAL_PTR_NULL;
    int32_t l_nss;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_idx = 0;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_NSS, OAL_SIZEOF(mac_cfg_tx_comp_stru));

    /* 解析并设置配置命令参数 */
    pst_set_nss_param = (mac_cfg_tx_comp_stru *)(st_write_msg.auc_value);

    /* 获取速率值字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_nss::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    /* 输入命令合法性检测 */
    while ('\0' != ac_arg[l_idx]) {
        if (isdigit(ac_arg[l_idx])) {
            l_idx++;
            continue;
        } else {
            l_idx++;
            oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_nss::input illegal!}\r\n");
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }

    /* 解析要设置为多大的值 */
    l_nss = oal_atoi(ac_arg);

    if (l_nss < WAL_HIPRIV_NSS_MIN || l_nss > WAL_HIPRIV_NSS_MAX) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_nss::input val out of range [%d]!}\r\n", l_nss);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pst_set_nss_param->uc_param = (uint8_t)(l_nss - 1);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tx_comp_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_nss::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }
    /* 读取返回的错误码 */
    ul_ret = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_set_freq fail, err code[%u]!}\r\n", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


uint32_t wal_hipriv_set_rfch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_tx_comp_stru *pst_set_rfch_param = OAL_PTR_NULL;
    uint8_t uc_ch;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int8_t c_ch_idx;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_RFCH, OAL_SIZEOF(mac_cfg_tx_comp_stru));

    /* 解析并设置配置命令参数 */
    pst_set_rfch_param = (mac_cfg_tx_comp_stru *)(st_write_msg.auc_value);

    /* 获取速率值字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_rfch::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    /* 解析要设置为多大的值 */
    uc_ch = 0;
    for (c_ch_idx = 0; c_ch_idx < WAL_HIPRIV_CH_NUM; c_ch_idx++) {
        if ('0' == ac_arg[c_ch_idx]) {
            continue;
        } else if ('1' == ac_arg[c_ch_idx]) {
            uc_ch += (uint8_t)(1 << (WAL_HIPRIV_CH_NUM - (uint8_t)c_ch_idx - 1));
        }
        /* 输入数据有非01数字，或数字少于4位，异常 */
        else {
            oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_rfch::input err!}\r\n");
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }

    /* 输入参数多于四位，异常 */
    if ('\0' != ac_arg[c_ch_idx]) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_rfch::input err!}\r\n");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pst_set_rfch_param->uc_param = uc_ch;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tx_comp_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_rfch::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    /* 读取返回的错误码 */
    ul_ret = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_set_rfch fail, err code[%u]!}\r\n", ul_ret);
        return ul_ret;
    }
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_NARROW_BAND

OAL_STATIC uint32_t wal_hipriv_narrow_bw(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    mac_cfg_narrow_bw_stru *pst_nrw_bw;
    uint32_t ul_ret;
    uint32_t ul_off_set;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_NARROW_BW, OAL_SIZEOF(mac_cfg_narrow_bw_stru));

    /* 解析并设置配置命令参数 */
    pst_nrw_bw = (mac_cfg_narrow_bw_stru *)(st_write_msg.auc_value);

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_narrow_bw::get switch  [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param = pc_param + ul_off_set;

    pst_nrw_bw->en_open = (uint8_t)oal_atoi(ac_arg);

    /* 窄带打开模式下强制关闭ampdu amsdu聚合和若干扰免疫算法 */
    if (pst_nrw_bw->en_open == OAL_TRUE) {
        wal_hipriv_alg_cfg(pst_net_dev, "anti_inf_unlock_en 0");
        wal_hipriv_ampdu_tx_on(pst_net_dev, "0");
        wal_hipriv_amsdu_tx_on(pst_net_dev, "0");
    } else {
        wal_hipriv_alg_cfg(pst_net_dev, "anti_inf_unlock_en 1");
        wal_hipriv_ampdu_tx_on(pst_net_dev, "1");
        wal_hipriv_amsdu_tx_on(pst_net_dev, "1");
    }

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_narrow_bw::get switch  [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param = pc_param + ul_off_set;

    if (0 == (oal_strcmp("1m", ac_arg))) {
        pst_nrw_bw->en_bw = NARROW_BW_1M;
    } else if (0 == (oal_strcmp("5m", ac_arg))) {
        pst_nrw_bw->en_bw = NARROW_BW_5M;
    } else if (0 == (oal_strcmp("10m", ac_arg))) {
        pst_nrw_bw->en_bw = NARROW_BW_10M;
    } else {
        pst_nrw_bw->en_bw = NARROW_BW_BUTT;
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_narrow_bw::bw should be 1/5/10 m");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_narrow_bw_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#endif


OAL_STATIC uint32_t wal_hipriv_ota_beacon_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_param;
    uint32_t ul_off_set = 0;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    int32_t l_ret;
    uint32_t ul_ret;

    /* OAM ota模块的开关的命令: hipriv "Hisilicon0 ota_beacon_switch 0 | 1"
    */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_ota_beacon_switch::wal_get_cmd_one_arg fails!}\r\n");
        return ul_ret;
    }
    l_param = oal_atoi((const int8_t *)ac_name);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_OTA_BEACON_SWITCH, OAL_SIZEOF(uint32_t));
    *((int32_t *)(st_write_msg.auc_value)) = l_param;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_ota_beacon_switch::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_ota_rx_dscr_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_param;
    uint32_t ul_off_set = 0;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    int32_t l_ret;
    uint32_t ul_ret;

    /* OAM ota模块的开关的命令: hipriv "Hisilicon0 ota_rx_dscr_switch 0 | 1"
    */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_ota_rx_dscr_switch::wal_get_cmd_one_arg fails!}\r\n");
        return ul_ret;
    }

    /* 解析参数 */
    ul_ret = wal_get_cmd_one_arg(pc_param + ul_off_set, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_ota_rx_dscr_switch::wal_get_cmd_one_arg fails!}\r\n");
        return ul_ret;
    }
    l_param = oal_atoi((const int8_t *)ac_name);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_OTA_RX_DSCR_SWITCH, OAL_SIZEOF(uint32_t));
    *((int32_t *)(st_write_msg.auc_value)) = l_param;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_ota_rx_dscr_switch::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_ether_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;
    mac_cfg_eth_switch_param_stru st_eth_switch_param;

    /* "vap0 ether_switch user_macaddr oam_ota_frame_direction_type_enum(帧方向) 0|1(开关)" */
    memset_s(&st_eth_switch_param, OAL_SIZEOF(mac_cfg_eth_switch_param_stru),
             0, OAL_SIZEOF(mac_cfg_eth_switch_param_stru));

    /* 获取mac地址 */
    ul_ret = wal_hipriv_get_mac_addr(pc_param, st_eth_switch_param.auc_user_macaddr, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ether_switch::wal_hipriv_get_mac_addr return err_code[%d]}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    /* 获取以太网帧方向 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ether_switch::wal_get_cmd_one_arg return err_code[%d]}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_eth_switch_param.en_frame_direction = (uint8_t)oal_atoi(ac_name);

    /* 获取开关 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ether_switch::wal_get_cmd_one_arg return err_code[%d]}\r\n", ul_ret);
        return ul_ret;
    }
    st_eth_switch_param.en_switch = (uint8_t)oal_atoi(ac_name);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ETH_SWITCH, OAL_SIZEOF(st_eth_switch_param));

    /* 设置配置命令参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                        (const void *)&st_eth_switch_param,
                        sizeof(st_eth_switch_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_ether_switch::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_eth_switch_param),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ether_switch::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC void wal_get_ucast_sub_switch(mac_cfg_80211_ucast_switch_stru *mac_80211_ucast_switch, int8_t *param,
    uint32_t len)
{
    uint32_t ret;
    uint32_t off_set;
    int8_t name[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    /* 获取MSDU描述符打印开关 */
    ret = wal_get_cmd_one_arg(param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_get_ucast_switch::get msdu dscr switch return err_code[%d]!}\r\n", ret);
        return;
    }
    param += off_set;
    mac_80211_ucast_switch->sub_switch.stru.bit_msdu_dscr = (uint8_t)oal_atoi(name) ? OAL_TRUE : OAL_FALSE;

    /* 获取BA_INFO打印开关 */
    ret = wal_get_cmd_one_arg(param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_get_ucast_switch::get ba info switch return err_code[%d]!}\r\n", ret);
        return;
    }
    param += off_set;
    mac_80211_ucast_switch->sub_switch.stru.bit_ba_info = (uint8_t)oal_atoi(name) ? OAL_TRUE : OAL_FALSE;

    /* 获取HIMIT描述符打印开关 */
    ret = wal_get_cmd_one_arg(param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_get_ucast_switch::get himit dscr switch return err_code[%d]!}\r\n", ret);
        return;
    }
    param += off_set;
    mac_80211_ucast_switch->sub_switch.stru.bit_himit_dscr = (uint8_t)oal_atoi(name) ? OAL_TRUE : OAL_FALSE;

    /* 获取MU描述符打印开关 */
    ret = wal_get_cmd_one_arg(param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_get_ucast_switch::get mu dscr switch return err_code[%d]!}\r\n", ret);
        return;
    }
    param += off_set;
    mac_80211_ucast_switch->sub_switch.stru.bit_mu_dscr = (uint8_t)oal_atoi(name) ? OAL_TRUE : OAL_FALSE;
}

OAL_STATIC uint32_t wal_get_ucast_switch(mac_cfg_80211_ucast_switch_stru *mac_80211_ucast_switch, int8_t *param,
    uint32_t len)
{
    uint32_t ret;
    uint32_t off_set;
    int8_t name[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    /* 获取80211帧方向 */
    ret = wal_get_cmd_one_arg(param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_get_ucast_switch::get 80211 ucast frame direction return err_code[%d]!}\r\n", ret);
        return ret;
    }
    param += off_set;
    mac_80211_ucast_switch->en_frame_direction = (uint8_t)oal_atoi(name);

    /* 获取帧类型 */
    ret = wal_get_cmd_one_arg(param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_get_ucast_switch::get ucast frame type return err_code[%d]!}\r\n", ret);
        return ret;
    }
    param += off_set;
    mac_80211_ucast_switch->en_frame_type = (uint8_t)oal_atoi(name);

    /* 获取帧内容打印开关 */
    ret = wal_get_cmd_one_arg(param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_get_ucast_switch::get frame content switch return err_code[%d]!}\r\n", ret);
        return ret;
    }
    param += off_set;
    mac_80211_ucast_switch->en_frame_switch = (uint8_t)oal_atoi(name) ? OAL_SWITCH_ON : OAL_SWITCH_OFF;

    /* 获取帧CB字段打印开关 */
    ret = wal_get_cmd_one_arg(param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_get_ucast_switch::get frame cb switch return err_code[%d]!}\r\n", ret);
        return ret;
    }
    param += off_set;
    mac_80211_ucast_switch->en_cb_switch = (uint8_t)oal_atoi(name) ? OAL_SWITCH_ON : OAL_SWITCH_OFF;

    /* 获取描述符打印开关 */
    ret = wal_get_cmd_one_arg(param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_get_ucast_switch::get frame dscr switch return err_code[%d]!}\r\n", ret);
        return ret;
    }
    param += off_set;
    mac_80211_ucast_switch->en_dscr_switch = (uint8_t)oal_atoi(name) ? OAL_SWITCH_ON : OAL_SWITCH_OFF;

    wal_get_ucast_sub_switch(mac_80211_ucast_switch, param, OAL_STRLEN(param));

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_80211_ucast_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int32_t l_ret;
    uint32_t ul_ret;
    mac_cfg_80211_ucast_switch_stru st_80211_ucast_switch;

    /* sh hipriv.sh "vap0 80211_uc_switch user_macaddr 0|1(帧方向tx|rx) 0|1(帧类型:管理帧|数据帧)
     *  0|1(帧内容开关) 0|1(CB开关) 0|1(描述符开关) 0|1(MSDU描述符) 0|1(BA_INFO) 0|1(HIMIT描述符) 0|1(MU描述符)"
    */
    memset_s(&st_80211_ucast_switch, OAL_SIZEOF(mac_cfg_80211_ucast_switch_stru),
             0, OAL_SIZEOF(mac_cfg_80211_ucast_switch_stru));

    /* 获取mac地址 */
    ul_ret = wal_hipriv_get_mac_addr(pc_param, st_80211_ucast_switch.auc_user_macaddr, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_hipriv_set_80211_ucast_switch::wal_hipriv_get_mac_addr return err_code[%d]}", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    ul_ret = wal_get_ucast_switch(&st_80211_ucast_switch, pc_param, OAL_STRLEN(pc_param));
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_80211_ucast_switch::get ucast switch fail [%d]!}", ul_ret);
        return ul_ret;
    }

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_80211_UCAST_SWITCH, OAL_SIZEOF(st_80211_ucast_switch));

    /* 设置配置命令参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                        (const void *)&st_80211_ucast_switch,
                        OAL_SIZEOF(st_80211_ucast_switch))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_80211_ucast_switch::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_80211_ucast_switch),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_80211_ucast_switch::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_TXOPPS

OAL_STATIC uint32_t wal_hipriv_set_txop_ps_machw(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;
    mac_txopps_machw_param_stru st_txopps_machw_param = { 0 };

    /* sh hipriv.sh "stavap_name txopps_hw_en 0|1(txop_ps_en) 0|1(condition1) 0|1(condition2)" */
    /* 获取txop ps使能开关 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_TXOP, "{wal_hipriv_set_txop_ps_machw::get machw txop_ps en return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_txopps_machw_param.en_machw_txopps_en = (oal_switch_enum_uint8)oal_atoi(ac_name);

    /* 获取txop ps condition1使能开关 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_TXOP, "{wal_hipriv_set_txop_ps_machw::get machw txop_ps condition1 return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_txopps_machw_param.en_machw_txopps_condition1 = (oal_switch_enum_uint8)oal_atoi(ac_name);

    /* 获取txop ps condition2使能开关 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_TXOP, "{wal_hipriv_set_txop_ps_machw::get machw txop_ps condition2 return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_txopps_machw_param.en_machw_txopps_condition2 = (oal_switch_enum_uint8)oal_atoi(ac_name);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_TXOP_PS_MACHW, OAL_SIZEOF(st_txopps_machw_param));

    /* 设置配置命令参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                        (const void *)&st_txopps_machw_param,
                        OAL_SIZEOF(st_txopps_machw_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_txop_ps_machw::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_txopps_machw_param),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_TXOP, "{wal_hipriv_set_txop_ps_machw::return err code[%d]!}\r\n", ul_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif


OAL_STATIC uint32_t wal_hipriv_set_80211_mcast_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;
    mac_cfg_80211_mcast_switch_stru st_80211_mcast_switch = { 0 };

    memset_s((uint8_t *)&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
    /*
     * sh hipriv.sh "Hisilicon0 80211_mc_switch 0|1(帧方向tx|rx) 0|1(帧类型:管理帧|数据帧)
     * 0|1(帧内容开关) 0|1(CB开关) 0|1(描述符开关)"
     */
    /* 获取80211帧方向 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_80211_mcast_switch::get 80211 mcast frame direction return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_80211_mcast_switch.en_frame_direction = (uint8_t)oal_atoi(ac_name);

    /* 获取帧类型 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_80211_mcast_switch::get mcast frame type return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_80211_mcast_switch.en_frame_type = (uint8_t)oal_atoi(ac_name);

    /* 获取帧内容打印开关 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_80211_mcast_switch::get mcast frame content switch return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_80211_mcast_switch.en_frame_switch = (uint8_t)oal_atoi(ac_name);

    /* 获取帧CB字段打印开关 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_80211_mcast_switch::get mcast frame cb switch return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_80211_mcast_switch.en_cb_switch = (uint8_t)oal_atoi(ac_name);

    /* 获取描述符打印开关 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_80211_mcast_switch::get mcast frame dscr switch return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_80211_mcast_switch.en_dscr_switch = (uint8_t)oal_atoi(ac_name);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_80211_MCAST_SWITCH, OAL_SIZEOF(st_80211_mcast_switch));

    /* 设置配置命令参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                        (const void *)&st_80211_mcast_switch,
                        OAL_SIZEOF(st_80211_mcast_switch))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_80211_mcast_switch::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_80211_mcast_switch),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_80211_mcast_switch::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_all_80211_ucast(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_ret;
    mac_cfg_80211_ucast_switch_stru st_80211_ucast_switch;
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    /* sh hipriv.sh "Hisilicon0 80211_uc_all 0|1(帧方向tx|rx) 0|1(帧类型:管理帧|数据帧)
     *  0|1(帧内容开关) 0|1(CB开关) 0|1(描述符开关) 0|1(MSDU描述符) 0|1(BA_INFO) 0|1(HIMIT描述符) 0|1(MU描述符)"
    */
    memset_s(&st_80211_ucast_switch, OAL_SIZEOF(mac_cfg_80211_ucast_switch_stru),
             0, OAL_SIZEOF(mac_cfg_80211_ucast_switch_stru));
    ul_ret = wal_get_ucast_switch(&st_80211_ucast_switch, pc_param, OAL_STRLEN(pc_param));
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_all_80211_ucast::get ucast switch fail [%d]!}", ul_ret);
        return ul_ret;
    }

    /* 设置广播mac地址 */
    memcpy_s(st_80211_ucast_switch.auc_user_macaddr, WLAN_MAC_ADDR_LEN, BROADCAST_MACADDR, WLAN_MAC_ADDR_LEN);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_80211_UCAST_SWITCH, OAL_SIZEOF(st_80211_ucast_switch));

    /* 设置配置命令参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                        (const void *)&st_80211_ucast_switch,
                        OAL_SIZEOF(st_80211_ucast_switch))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_all_80211_ucast::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_80211_ucast_switch),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_80211_all_ucast_switch::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_all_ether_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    uint16_t us_user_num;
    uint8_t uc_frame_direction;
    uint8_t uc_switch;

    /* sh hipriv.sh "Hisilicon0 ether_all 0|1(帧方向tx|rx) 0|1(开关)" */
    /* 获取以太网帧方向 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_all_ether_switch::get eth frame direction return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    uc_frame_direction = (uint8_t)oal_atoi(ac_name);

    /* 获取帧开关 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_all_ether_switch::get eth type return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    uc_switch = (uint8_t)oal_atoi(ac_name);

    /* 设置开关 */
    for (us_user_num = 0; us_user_num < WLAN_USER_MAX_USER_LIMIT; us_user_num++) {
        oam_report_eth_frame_set_switch(us_user_num, uc_switch, uc_frame_direction);
    }

    /* 同时设置广播arp dhcp帧的上报开关 */
    oam_report_dhcp_arp_set_switch(uc_switch);

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_dhcp_arp_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint8_t uc_switch;

    /* sh hipriv.sh "Hisilicon0 dhcp_arp_switch 0|1(开关)" */
    /* 获取帧方向 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_dhcp_arp_switch::get switch return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    uc_switch = (uint8_t)oal_atoi(ac_name);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_DHCP_ARP, OAL_SIZEOF(uint32_t));
    *((int32_t *)(st_write_msg.auc_value)) = (uint32_t)uc_switch;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_dhcp_arp_switch::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_report_vap_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_flag_value;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;

    /* sh hipriv.sh "wlan0 report_vap_info  flags_value" */

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_report_vap_info::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    ul_flag_value = (uint32_t)oal_atoi(ac_name);

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_REPORT_VAP_INFO, OAL_SIZEOF(ul_flag_value));

    /* 填写消息体，参数 */
    *(uint32_t *)(st_write_msg.auc_value) = ul_flag_value;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(ul_flag_value),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_report_vap_info::wal_send_cfg_event return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_trlr_info(mac_phy_debug_switch_stru *pst_phy_debug_switch, int8_t c_value)
{
    if (isdigit(c_value)) {
        pst_phy_debug_switch->auc_trlr_sel_info[pst_phy_debug_switch->uc_trlr_sel_num] = (uint8_t)(c_value - '0');
    } else if ((c_value >= 'a') && (c_value <= 'f')) {
        pst_phy_debug_switch->auc_trlr_sel_info[pst_phy_debug_switch->uc_trlr_sel_num] = (uint8_t)(c_value - 'a' + 10);
    } else if ((c_value >= 'A') || (c_value <= 'F')) {
        pst_phy_debug_switch->auc_trlr_sel_info[pst_phy_debug_switch->uc_trlr_sel_num] = (uint8_t)(c_value - 'A' + 10);
    } else {
        oam_error_log0(0, OAM_SF_CFG, "{wal_hipriv_set_trlr_info:: param input illegal, should be [0-f/F].!!}\r\n");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_get_tone_tran_para(int8_t **pc_param, int8_t *ac_value, uint32_t ul_value_len, mac_phy_debug_switch_stru *pst_phy_debug_switch)
{
    uint32_t ul_ret = 0;
    uint32_t ul_off_set = 0;

    if (0 == oal_strcmp("help", ac_value)) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_get_tone_tran_para:tone tansmit command.!!}\r\n");
        return OAL_SUCC;
    }

    pst_phy_debug_switch->st_tone_tran.uc_tone_tran_switch = (uint8_t)oal_atoi(ac_value);
    if (pst_phy_debug_switch->st_tone_tran.uc_tone_tran_switch == 1) {
        ul_ret = wal_get_cmd_one_arg(*pc_param, ac_value, ul_value_len, &ul_off_set);
        if ((ul_ret != OAL_SUCC) || (ul_off_set == 0)) {
            oam_error_log0(0, OAM_SF_CFG, "{wal_get_tone_tran_para:chain index is illegal'!!}\r\n");
            return ul_ret;
        }
        *pc_param += ul_off_set;
        pst_phy_debug_switch->st_tone_tran.uc_chain_idx = (uint16_t)oal_atoi(ac_value);

        if ((pst_phy_debug_switch->st_tone_tran.uc_chain_idx != 0) && (pst_phy_debug_switch->st_tone_tran.uc_chain_idx != 1)) {
            oam_error_log1(0, OAM_SF_CFG, "{wal_get_tone_tran_para:chain index[%d] is invalid'!!}\r\n", pst_phy_debug_switch->st_tone_tran.uc_chain_idx);
            return OAL_FAIL;
        }

        /*  获取数据长度  */
        ul_ret = wal_get_cmd_one_arg(*pc_param, ac_value, ul_value_len, &ul_off_set);
        if ((ul_ret != OAL_SUCC) || (ul_off_set == 0) || (0 == oal_atoi(ac_value))) {
            oam_error_log0(0, OAM_SF_CFG, "{wal_get_tone_tran_para:tone data len is illegal'!!}\r\n");
            return ul_ret;
        }
        *pc_param += ul_off_set;
        pst_phy_debug_switch->st_tone_tran.us_data_len = (uint16_t)oal_atoi(ac_value);
    }
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_print_phy_debug_help_info(int8_t *pc_param, uint8_t uc_para_len)
{
    if (uc_para_len < OAL_STRLEN("help")) {
        return OAL_FAIL;
    }

    if (0 == oal_strcmp("help", pc_param)) {
        oam_warning_log0(0, OAM_SF_CFG, "{CMD[rssi]Print the rssi of rx packets, reported from rx dscr of MAC, range [-128 ~ +127] dBm.!!}\r\n");
        oam_warning_log0(0, OAM_SF_CFG, "{CMD[tsensor]Print the code of T-sensor.!!}\r\n");
        oam_warning_log0(0, OAM_SF_CFG, "{CMD[snr]Print snr values of two rx ants, 11B not included, reported from rx dscr of MAC, range [-10 ~ +55] dBm.!!}\r\n");
        oam_warning_log0(0, OAM_SF_CFG, "{CMD[evm]Print evm values of two rx ants, 11B not included, reported from rx dscr of MAC, range [-10 ~ +55] dBm.!!}\r\n");
        oam_warning_log0(0, OAM_SF_CFG, "{CMD[spatial_reuse]Print Sptial Reuse of MAC srg and non-srg frame time and rssi!!}\r\n");
        oam_warning_log0(0, OAM_SF_CFG, "{CMD[trlr/vect]Sel range::trailer [0~f/F], vector [0~7], eg1: trlr 01234 eg2: vec123 trlr12, sum of both is less than 5.!!}\r\n");
        oam_warning_log0(0, OAM_SF_CFG, "{CMD[count]Set the interval of print (packets), range [0 ~ 2^32].!!}\r\n");

        return OAL_SUCC;
    }

    return OAL_FAIL;
}


uint32_t wal_hipriv_set_phy_debug_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set = 0;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int8_t ac_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN];
    mac_phy_debug_switch_stru st_phy_debug_switch;
    uint32_t ul_ret = 0;
    int32_t l_ret;
    oal_bool_enum_uint8 en_cmd_updata = OAL_FALSE;
    uint8_t uc_data_cnt;  // 单次命令的最大计数

    /* sh hipriv.sh "wlan0 phy_debug snr 0|1(关闭|打开) rssi 0|1(关闭|打开) trlr 1234a count N(每个N个报文打印一次)" */

    memset_s(&st_phy_debug_switch, OAL_SIZEOF(st_phy_debug_switch), 0, OAL_SIZEOF(st_phy_debug_switch));

    st_phy_debug_switch.ul_rx_comp_isr_interval = 10;  // 如果没有设置，则默认10个包打印一次，命令码可以更新
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    st_phy_debug_switch.uc_trlr_sel_num = (BOARD_VERSION_HI1105 == get_hi110x_subchip_type()) ? 0 : 1;  // 1103mpw2上rx hder占用一个，因此这里从1开始计数, 1105从0开始
#endif
    st_phy_debug_switch.uc_force_work_switch = 0xff;
    st_phy_debug_switch.st_tone_tran.uc_tone_tran_switch = 0xF; /*  默认单音发送不处于发送/关闭状态  */
    st_phy_debug_switch.uc_dfr_reset_switch = 0xff;
    st_phy_debug_switch.uc_extlna_chg_bypass_switch = 0xff;
    st_phy_debug_switch.uc_edca_param_switch = 0x0;

    do {
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if ((ul_ret != OAL_SUCC) && (ul_off_set != 0)) {
            oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_set_phy_debug_switch::cmd format err, ret:%d;!!}\r\n", ul_ret);
            return ul_ret;
        }
        pc_param += ul_off_set;

        if (en_cmd_updata == OAL_FALSE) {
            en_cmd_updata = OAL_TRUE;
        } else if (ul_off_set == 0) {
            break;
        }

        ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oal_io_print("CMD format::sh hipriv.sh 'wlan0 phy_debug [rssi 0|1] [snr 0|1] [trlr xxxxx] [vect yyyyy] [count N] [edca 0-3(tid_no)] [aifsn N] [cwmin N] [cwmax N] [txoplimit N]'");
            oam_error_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 phy_debug [rssi 0|1] [snr 0|1] [trlr xxxxx] [vect yyyyy] [count N] [edca 0-3(tid_no)] [aifsn N] [cwmin N] [cwmax N] [txoplimit N]'!!}\r\n");
            return ul_ret;
        }
        pc_param += ul_off_set;
        ul_off_set = 0;

        /* 打印help信息 */
        ul_ret = wal_hipriv_print_phy_debug_help_info(ac_value, sizeof(ac_value));
        if (ul_ret == OAL_SUCC) {
            return ul_ret;
        }

        /* 查询各个命令 */
        if (0 == oal_strcmp("rssi", ac_name)) {
            st_phy_debug_switch.en_rssi_debug_switch = ((uint8_t)oal_atoi(ac_value)) & OAL_TRUE;
        } else if (0 == oal_strcmp("tsensor", ac_name)) {
            st_phy_debug_switch.en_tsensor_debug_switch = ((uint8_t)oal_atoi(ac_value)) & OAL_TRUE;
        } else if (0 == oal_strcmp("snr", ac_name)) {
            st_phy_debug_switch.en_snr_debug_switch = ((uint8_t)oal_atoi(ac_value)) & OAL_TRUE;
        } else if (0 == oal_strcmp("evm", ac_name)) {
            st_phy_debug_switch.en_evm_debug_switch = ((uint8_t)oal_atoi(ac_value)) & OAL_TRUE;
        } else if (0 == oal_strcmp("spatial_reuse", ac_name)) {
            st_phy_debug_switch.en_sp_reuse_debug_switch = ((uint8_t)oal_atoi(ac_value)) & OAL_TRUE;
        } else if (oal_any_zero_value2(oal_strcmp("trlr", ac_name), oal_strcmp("vect", ac_name))) {
            uc_data_cnt = 0;
            while (ac_value[uc_data_cnt] != '\0') {
                /* 输入参数合法性检查 */
                if (st_phy_debug_switch.uc_trlr_sel_num >= WAL_PHY_DEBUG_TEST_WORD_CNT) {
                    oam_error_log1(0, OAM_SF_CFG, "{Param input illegal, cnt [%d] reached 5!!}\r\n", st_phy_debug_switch.uc_trlr_sel_num);
                    return OAL_ERR_CODE_ARRAY_OVERFLOW;
                }

                ul_ret = wal_hipriv_set_trlr_info(&st_phy_debug_switch, ac_value[uc_data_cnt]);
                if (ul_ret != OAL_SUCC) {
                    return ul_ret;
                }

                if (0 == oal_strcmp("vect", ac_name)) {
                    if (ac_value[uc_data_cnt] > '7') {
                        oam_error_log0(0, OAM_SF_CFG, "{param input illegal, vect should be [0-7].!!}\r\n");
                        return OAL_ERR_CODE_INVALID_CONFIG;
                    }
                } else {
                    /* vector bit4为0，trailer的bit4置1, 设置寄存器可以一并带上 */
                    st_phy_debug_switch.auc_trlr_sel_info[st_phy_debug_switch.uc_trlr_sel_num] |= 0x10;
                }

                uc_data_cnt++;
                st_phy_debug_switch.uc_trlr_sel_num++;
            }

            /* 若输入正常，则打开trailer开关 */
            if (uc_data_cnt != 0) {
                st_phy_debug_switch.en_trlr_debug_switch = OAL_TRUE;
            }
        } else if (0 == oal_strcmp("count", ac_name)) {
            st_phy_debug_switch.ul_rx_comp_isr_interval = (uint32_t)oal_atoi(ac_value);
        } else if (0 == oal_strcmp("iq_cali", ac_name)) {
            st_phy_debug_switch.uc_iq_cali_switch = (uint8_t)oal_atoi(ac_value);
        } else if (0 == oal_strcmp("tone_tran", ac_name)) {
            ul_ret = wal_get_tone_tran_para(&pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &st_phy_debug_switch);
            if (ul_ret != OAL_SUCC) {
                return ul_ret;
            }
        } else if (0 == oal_strcmp("pdet", ac_name)) {
            st_phy_debug_switch.en_pdet_debug_switch = ((uint8_t)oal_atoi(ac_value)) & OAL_TRUE;
        } else if (0 == oal_strcmp("force_work", ac_name)) {
            st_phy_debug_switch.uc_force_work_switch = ((uint8_t)oal_atoi(ac_value));
        } else if (0 == oal_strcmp("dfr_reset", ac_name)) {
            st_phy_debug_switch.uc_dfr_reset_switch = ((uint8_t)oal_atoi(ac_value));
        } else if (0 == oal_strcmp("fsm_info", ac_name)) {
            st_phy_debug_switch.uc_fsm_info_switch = ((uint8_t)oal_atoi(ac_value)) & OAL_TRUE;
        } else if (0 == oal_strcmp("report_radar", ac_name)) {
            st_phy_debug_switch.uc_report_radar_switch = OAL_TRUE;
        } else if (0 == oal_strcmp("extlna_bypass", ac_name)) {
            st_phy_debug_switch.uc_extlna_chg_bypass_switch = ((uint8_t)oal_atoi(ac_value));
        } else if (0 == oal_strcmp("edca", ac_name)) {
            st_phy_debug_switch.uc_edca_param_switch |= ((uint8_t)oal_atoi(ac_value)) << 4;
        } else if (0 == oal_strcmp("aifsn", ac_name)) {
            st_phy_debug_switch.uc_edca_param_switch |= (uint8_t)BIT3;
            st_phy_debug_switch.uc_edca_aifsn = (uint8_t)oal_atoi(ac_value);
        } else if (0 == oal_strcmp("cwmin", ac_name)) {
            st_phy_debug_switch.uc_edca_param_switch |= (uint8_t)BIT2;
            st_phy_debug_switch.uc_edca_cwmin = (uint8_t)oal_atoi(ac_value);
        } else if (0 == oal_strcmp("cwmax", ac_name)) {
            st_phy_debug_switch.uc_edca_param_switch |= (uint8_t)BIT1;
            st_phy_debug_switch.uc_edca_cwmax = (uint8_t)oal_atoi(ac_value);
        } else if (0 == oal_strcmp("txoplimit", ac_name)) {
            st_phy_debug_switch.uc_edca_param_switch |= (uint8_t)BIT0;
            st_phy_debug_switch.us_edca_txoplimit = (uint16_t)oal_atoi(ac_value);
        } else {
            oal_io_print("CMD format::sh hipriv.sh 'wlan0 phy_debug [rssi 0|1] [snr 0|1] [trlr xxxxx] [vect yyyyy] [count N] [edca 0-3(tid_no)] [aifsn N] [cwmin N] [cwmax N] [txoplimit N]'");
            oam_warning_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 phy_debug [rssi 0|1] [snr 0|1] [trlr xxxx] [vect yyyy] [count N] [edca 0-3(tid_no)] [aifsn N] [cwmin N] [cwmax N] [txoplimit N]'!!}\r\n");
            return OAL_FAIL;
        }
    } while (*pc_param != '\0');

    /* 将打印总开关保存到 */
    st_phy_debug_switch.en_debug_switch = st_phy_debug_switch.en_rssi_debug_switch | st_phy_debug_switch.en_snr_debug_switch | st_phy_debug_switch.en_trlr_debug_switch \
                                          | st_phy_debug_switch.uc_iq_cali_switch | st_phy_debug_switch.en_tsensor_debug_switch | st_phy_debug_switch.en_evm_debug_switch;

    oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_set_phy_debug_switch:: phy_debug switch [%d].}\r\n", st_phy_debug_switch.en_debug_switch);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_PHY_DEBUG_SWITCH, OAL_SIZEOF(st_phy_debug_switch));

    /* 设置配置命令参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                        (const void *)&st_phy_debug_switch,
                        OAL_SIZEOF(st_phy_debug_switch))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_phy_debug_switch::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_phy_debug_switch),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_phy_debug_switch::return err code[%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC OAL_INLINE void protocol_debug_cmd_format_info(void)
{
    oam_warning_log0(0, OAM_SF_ANY, "{CMD format::sh hipriv.sh 'wlan0 protocol_debug\
    [band_force_switch 0|1|2(20M|40M+|40M-)]\
    [2040_ch_swt_prohi 0|1]\
    [40_intol 0|1]'!!}\r\n");
    oam_warning_log0(0, OAM_SF_ANY,
                     "{[csa 0(csa mode) 1(csa channel) 10(csa cnt) 1(debug  flag,0:normal channel channel,1:only include csa ie 2:cannel debug)]\
    [2040_user_switch 0|1]'!!}\r\n");
    oam_warning_log0(0, OAM_SF_ANY,
                     "[lsig 0|1]\
    '!!}\r\n");
}


OAL_STATIC uint32_t wal_protocol_debug_parase_csa_cmd(int8_t *pc_param, mac_protocol_debug_switch_stru *pst_debug_info, uint32_t *pul_offset)
{
    uint32_t ul_ret;
    int8_t ac_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN];
    uint32_t ul_off_set = 0;
    uint8_t uc_value;

    *pul_offset = 0;
    /* 解析csa mode */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_protocol_debug_parase_csa_cmd::get csa mode error,return.}");
        return ul_ret;
    }
    uc_value = (uint8_t)oal_atoi(ac_value);
    if (uc_value > 1) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_protocol_debug_parase_csa_cmd::csa mode = [%d] invalid,return.}", uc_value);
        return OAL_FAIL;
    }
    *pul_offset += ul_off_set;
    pst_debug_info->st_csa_debug_bit3.en_mode = uc_value;
    pc_param += ul_off_set;
    ul_off_set = 0;

    /* 解析csa channel */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_protocol_debug_parase_csa_cmd::get csa channel error,return.}");
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    pst_debug_info->st_csa_debug_bit3.uc_channel = (uint8_t)oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    /* 解析bandwidth */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_protocol_debug_parase_csa_cmd::get bandwidth error,return.}");
        return ul_ret;
    }
    uc_value = (uint8_t)oal_atoi(ac_value);
    if (uc_value >= WLAN_BAND_WIDTH_BUTT) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_protocol_debug_parase_csa_cmd::invalid bandwidth = %d,return.}", uc_value);
        return OAL_FAIL;
    }
    *pul_offset += ul_off_set;
    pst_debug_info->st_csa_debug_bit3.en_bandwidth = uc_value;
    pc_param += ul_off_set;
    ul_off_set = 0;

    /* 解析csa cnt */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_protocol_debug_parase_csa_cmd::get csa cnt error,return.}");
        return ul_ret;
    }
    uc_value = (uint8_t)oal_atoi(ac_value);
    if (uc_value >= 255) {
        uc_value = 255;
    }
    *pul_offset += ul_off_set;
    pst_debug_info->st_csa_debug_bit3.uc_cnt = uc_value;
    pc_param += ul_off_set;
    ul_off_set = 0;

    /* 解析debug flag */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_protocol_debug_parase_csa_cmd::get debug flag error,return.}");
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    uc_value = (uint8_t)oal_atoi(ac_value);
    if (uc_value >= MAC_CSA_FLAG_BUTT) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_protocol_debug_parase_csa_cmd::invalid debug flag = %d,return.}", uc_value);
        return OAL_FAIL;
    }
    pst_debug_info->st_csa_debug_bit3.en_debug_flag = (mac_csa_flag_enum_uint8)oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_protocol_debug_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set = 0;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int8_t ac_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN];
    mac_protocol_debug_switch_stru st_protocol_debug;
    uint32_t ul_ret = 0;
    int32_t l_ret;
    oal_bool_enum_uint8 en_cmd_updata = OAL_FALSE;

    /* sh hipriv.sh "wlan0 protocol_debug band_force_switch 0|1|2(20|40-|40+) 2040_ch_swt_prohi 0|1(关闭|打开) 2040_intolerant 0|1(关闭|打开)" */
    memset_s(&st_protocol_debug, OAL_SIZEOF(st_protocol_debug), 0, OAL_SIZEOF(st_protocol_debug));

    do {
        /* 获取命令关键字 */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if ((ul_ret != OAL_SUCC) && (ul_off_set != 0)) {
            protocol_debug_cmd_format_info();
            return ul_ret;
        }
        pc_param += ul_off_set;

        if (en_cmd_updata == OAL_FALSE) {
            en_cmd_updata = OAL_TRUE;
        } else if (ul_off_set == 0) {
            break;
        }

        /* 命令分类 */
        if (0 == oal_strcmp("band_force_switch", ac_name)) {
            /* 取命令配置值 */
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
            if ((ul_ret != OAL_SUCC) || oal_value_not_in_valid_range(ac_value[0], '0', '9')) {
                protocol_debug_cmd_format_info();
                return ul_ret;
            }
            pc_param += ul_off_set;
            ul_off_set = 0;
            /* 填写结构体 */
            st_protocol_debug.en_band_force_switch_bit0 = ((uint8_t)oal_atoi(ac_value));
            st_protocol_debug.ul_cmd_bit_map |= BIT0;
        } else if (0 == oal_strcmp("2040_ch_swt_prohi", ac_name)) {
            /* 取命令配置值 */
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
            if ((ul_ret != OAL_SUCC) || oal_value_not_in_valid_range(ac_value[0], '0', '9')) {
                protocol_debug_cmd_format_info();
                return ul_ret;
            }
            pc_param += ul_off_set;
            ul_off_set = 0;
            /* 填写结构体 */
            st_protocol_debug.en_2040_ch_swt_prohi_bit1 = ((uint8_t)oal_atoi(ac_value)) & BIT0;
            st_protocol_debug.ul_cmd_bit_map |= BIT1;
        } else if (0 == oal_strcmp("40_intol", ac_name)) {
            /* 取命令配置值 */
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
            if ((ul_ret != OAL_SUCC) || oal_value_not_in_valid_range(ac_value[0], '0', '9')) {
                protocol_debug_cmd_format_info();
                return ul_ret;
            }
            pc_param += ul_off_set;
            ul_off_set = 0;
            /* 填写结构体 */
            st_protocol_debug.en_40_intolerant_bit2 = ((uint8_t)oal_atoi(ac_value)) & BIT0;
            st_protocol_debug.ul_cmd_bit_map |= BIT2;
        } else if (0 == oal_strcmp("csa", ac_name)) {
            ul_ret = wal_protocol_debug_parase_csa_cmd(pc_param, &st_protocol_debug, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                protocol_debug_cmd_format_info();
                return ul_ret;
            }
            pc_param += ul_off_set;
            ul_off_set = 0;
            st_protocol_debug.ul_cmd_bit_map |= BIT3;
        }
        else if (0 == oal_strcmp("lsig", ac_name)) {
            /* 取命令配置值 */
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
            if ((ul_ret != OAL_SUCC) || oal_value_not_in_valid_range(ac_value[0], '0', '9')) {
                protocol_debug_cmd_format_info();
                return ul_ret;
            }
            pc_param += ul_off_set;
            ul_off_set = 0;
            /* 填写结构体 */
            st_protocol_debug.en_lsigtxop_bit5 = ((uint8_t)oal_atoi(ac_value)) & BIT0;
            st_protocol_debug.ul_cmd_bit_map |= BIT5;
        }
#ifdef _PRE_WLAN_FEATURE_11AX
        else if (g_wlan_spec_cfg->feature_11ax_is_open && 0 == oal_strcmp("11ax", ac_name)) {
            st_protocol_debug.ul_cmd_bit_map |= BIT6;
        }
#endif
        else {
            protocol_debug_cmd_format_info();
            return OAL_FAIL;
        }
    } while (*pc_param != '\0');

    oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_show_protocol_debug_info::ul_cmd_bit_map: 0x%08x.}", st_protocol_debug.ul_cmd_bit_map);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_PROTOCOL_DBG, OAL_SIZEOF(st_protocol_debug));

    /* 设置配置命令参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                        (const void *)&st_protocol_debug, OAL_SIZEOF(st_protocol_debug))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_show_protocol_debug_info::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_protocol_debug),
                                   (uint8_t *)&st_write_msg, OAL_FALSE,  OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_show_protocol_debug_info::return err code[%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_set_pm_debug_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    uint32_t ul_ret = 0;
    int32_t l_ret;
    uint32_t ul_offset;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int8_t ac_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN];
    mac_pm_debug_cfg_stru *pst_pm_debug_cfg = OAL_PTR_NULL;
    uint8_t uc_switch;
    oal_bool_enum_uint8 en_cmd_updata = OAL_FALSE;

    /* sh hipriv.sh "wlan0 pm_debug srb " */

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_pm_debug_switch::pst_mac_vap is null!}");
        return OAL_FAIL;
    }

    pst_pm_debug_cfg = (mac_pm_debug_cfg_stru *)(st_write_msg.auc_value);

    memset_s(pst_pm_debug_cfg, OAL_SIZEOF(mac_pm_debug_cfg_stru), 0, OAL_SIZEOF(mac_pm_debug_cfg_stru));

    do {
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_offset);
        if ((ul_ret != OAL_SUCC) && (0 != ul_offset)) {
            oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_set_pm_debug_switch::cmd format err, ret:%d;!!}\r\n", ul_ret);
            return ul_ret;
        }
        pc_param += ul_offset;

        if (en_cmd_updata == OAL_FALSE) {
            en_cmd_updata = OAL_TRUE;
        } else if (0 == ul_offset) {
            break;
        }

        ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_offset);
        if ((ul_ret != OAL_SUCC) || ((!isdigit(ac_value[0])) && (0 != oal_strcmp("help", ac_value)))) {
            oal_io_print("CMD format::sh hipriv.sh 'wlan0 pm_debug [srb 0|1]'\r\n");
            oam_error_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug [srb 0|1]'!!}\r\n");
            return ul_ret;
        }
        pc_param += ul_offset;

        if (0 == oal_strcmp("srb", ac_name)) {
            if (0 == oal_strcmp("help", ac_value)) {
                oam_warning_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug srb [0|1]'}");
                return OAL_SUCC;
            }

            uc_switch = (uint8_t)oal_atoi(ac_value);
            if (uc_switch > 1) {
                oam_error_log1(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug srb [0|1]', input[%d]!!}", uc_switch);
                return OAL_FAIL;
            }
            pst_pm_debug_cfg->ul_cmd_bit_map |= BIT(MAC_PM_DEBUG_SISO_RECV_BCN);
            pst_pm_debug_cfg->uc_srb_switch = uc_switch;

            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_hipriv_set_pm_debug_switch::siso recv beacon switch[%d].}", uc_switch);
        } else if (0 == oal_strcmp("dto", ac_name)) {
            if (0 == oal_strcmp("help", ac_value)) {
                oam_warning_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug dto [0|1]'}");
                return OAL_SUCC;
            }

            uc_switch = (uint8_t)oal_atoi(ac_value);
            if (uc_switch > 1) {
                oam_error_log1(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug dto [0|1]', input[%d]!!}", uc_switch);
                return OAL_FAIL;
            }
            pst_pm_debug_cfg->ul_cmd_bit_map |= BIT(MAC_PM_DEBUG_DYN_TBTT_OFFSET);
            pst_pm_debug_cfg->uc_dto_switch = uc_switch;

            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_hipriv_set_pm_debug_switch::dyn tbtt offset switch[%d].}", uc_switch);
        } else if (0 == oal_strcmp("nfi", ac_name)) {
            if (0 == oal_strcmp("help", ac_value)) {
                oam_warning_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug nfi [0|1]'}");
                return OAL_SUCC;
            }

            uc_switch = (uint8_t)oal_atoi(ac_value);
            if (uc_switch > 1) {
                oam_error_log1(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug nfi [0|1]', input[%d]!!}", uc_switch);
                return OAL_FAIL;
            }
            pst_pm_debug_cfg->ul_cmd_bit_map |= BIT(MAC_PM_DEBUG_NO_PS_FRM_INT);
            pst_pm_debug_cfg->uc_nfi_switch = uc_switch;

            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_hipriv_set_pm_debug_switch::no ps frm int switch[%d].}", uc_switch);
        } else if (0 == oal_strcmp("apf", ac_name)) {
            if (0 == oal_strcmp("help", ac_value)) {
                oam_warning_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug apf [0|1]'}");
                return OAL_SUCC;
            }

            uc_switch = (uint8_t)oal_atoi(ac_value);
            if (uc_switch > 1) {
                oam_error_log1(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug apf [0|1]', input[%d]!!}", uc_switch);
                return OAL_FAIL;
            }
            pst_pm_debug_cfg->ul_cmd_bit_map |= BIT(MAC_PM_DEBUG_APF);
            pst_pm_debug_cfg->uc_apf_switch = uc_switch;

            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_hipriv_set_pm_debug_switch::apf switch[%d].}", uc_switch);
        } else if (0 == oal_strcmp("ao", ac_name)) {
            if (0 == oal_strcmp("help", ac_value)) {
                oam_warning_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug ao [0|1]'}");
                return OAL_SUCC;
            }

            uc_switch = (uint8_t)oal_atoi(ac_value);
            if (uc_switch > 1) {
                oam_error_log1(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug ao [0|1]', input[%d]!!}", uc_switch);
                return OAL_FAIL;
            }
            pst_pm_debug_cfg->ul_cmd_bit_map |= BIT(MAC_PM_DEBUG_AO);
            pst_pm_debug_cfg->uc_ao_switch = uc_switch;

            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_hipriv_set_pm_debug_switch::arp offload switch[%d].}", uc_switch);
        } else {
            oal_io_print("CMD format::sh hipriv.sh 'wlan0 pm_debug [srb 0|1] [dto 0|1] [nfi 0|1] [apf 0|1] [ao 0|1]'");
            oam_error_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug [srb 0|1] [dto 0|1] [nfi 0|1] [apf 0|1]'!!}");
            return OAL_FAIL;
        }
    } while (*pc_param != '\0');

    /***************************************************************************
                               抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_PM_DEBUG_SWITCH, OAL_SIZEOF(mac_pm_debug_cfg_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_pm_debug_cfg_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_dbdc_debug_switch::return err code[%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_dbdc_debug_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set = 0;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int8_t ac_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN];
    mac_dbdc_debug_switch_stru st_dbdc_debug_switch;
    mac_vap_stru *pst_mac_vap= NULL;
    uint32_t ul_ret = 0;
    int32_t l_ret;
    uint8_t en_dbdc_enable;
    oal_bool_enum_uint8 en_cmd_updata = OAL_FALSE;

    /* sh hipriv.sh "wlan0 dbdc_debug change_hal_dev 0|1(hal 0|hal 1)" */

    memset_s(&st_dbdc_debug_switch, OAL_SIZEOF(st_dbdc_debug_switch), 0, OAL_SIZEOF(st_dbdc_debug_switch));

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_dbdc_debug_switch::pst_mac_vap is null!}");
        return OAL_FAIL;
    }

    do {
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if ((ul_ret != OAL_SUCC) && (ul_off_set != 0)) {
            oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_set_dbdc_debug_switch::cmd format err, ret:%d;!!}\r\n", ul_ret);
            return ul_ret;
        }
        pc_param += ul_off_set;

        if (en_cmd_updata == OAL_FALSE) {
            en_cmd_updata = OAL_TRUE;
        } else if (ul_off_set == 0) {
            break;
        }

        ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
        if ((ul_ret != OAL_SUCC) || (!isdigit(ac_value[0]))) {
            oal_io_print("CMD format::sh hipriv.sh 'wlan0 dbdc_debug [change_hal_dev 0|1]'\r\n");
            oam_error_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 dbdc_debug [change_hal_dev 0|1]'!!}\r\n");
            return ul_ret;
        }
        pc_param += ul_off_set;
        ul_off_set = 0;

        if (0 == oal_strcmp("dbdc_enable", ac_name)) {
            en_dbdc_enable = (uint8_t)oal_atoi(ac_value);
            if (en_dbdc_enable > 1) {
                oam_error_log1(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 dbdc_debug [change_hal_dev 0|1],input[%d]'!!}", en_dbdc_enable);
                return OAL_FAIL;
            }
            st_dbdc_debug_switch.ul_cmd_bit_map |= BIT(MAC_DBDC_SWITCH);
            st_dbdc_debug_switch.uc_dbdc_enable = ((uint8_t)oal_atoi(ac_value));

            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_hipriv_set_dbdc_debug_switch::dbdc enable[%d].}", st_dbdc_debug_switch.uc_dbdc_enable);
        } else {
            oal_io_print("CMD format::sh hipriv.sh 'wlan0 dbdc_debug [change_hal_dev 0|1]'");
            oam_error_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 dbdc_debug [change_hal_dev 0|1]'!!}");
            return OAL_FAIL;
        }
    } while (*pc_param != '\0');

    /***************************************************************************
                            抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DBDC_DEBUG_SWITCH, OAL_SIZEOF(st_dbdc_debug_switch));

    /* 设置配置命令参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                        (const void *)&st_dbdc_debug_switch,
                        OAL_SIZEOF(st_dbdc_debug_switch))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_dbdc_debug_switch::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_dbdc_debug_switch),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_dbdc_debug_switch::return err code[%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_probe_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;
    mac_cfg_probe_switch_stru st_probe_switch = { 0 };

    /*
     * sh hipriv.sh "Hisilicon0 probe_switch 0|1(帧方向tx|rx) 0|1(帧内容开关)
     * 0|1(CB开关) 0|1(描述符开关)"
     */
    /* 获取帧方向 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_probe_switch::get probe direction return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_probe_switch.en_frame_direction = (uint8_t)oal_atoi(ac_name);

    /* 获取帧内容打印开关 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_probe_switch::get probe frame content switch return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_probe_switch.en_frame_switch = (uint8_t)oal_atoi(ac_name);

    /* 获取帧CB字段打印开关 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_probe_switch::get probe frame cb switch return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_probe_switch.en_cb_switch = (uint8_t)oal_atoi(ac_name);

    /* 获取描述符打印开关 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_probe_switch::get probe frame dscr switch return err_code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_probe_switch.en_dscr_switch = (uint8_t)oal_atoi(ac_name);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_PROBE_SWITCH, OAL_SIZEOF(st_probe_switch));

    /* 设置配置命令参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                        (const void *)&st_probe_switch,
                        OAL_SIZEOF(st_probe_switch))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_80211_ucast_switch::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_probe_switch),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_80211_ucast_switch::return err code[%d]!}\r\n", ul_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_all_ota(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_ret;
    int32_t l_ret;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_param;
    wal_msg_write_stru st_write_msg;

    /* 获取开关 sh hipriv.sh "Hisilicon0 set_all_ota 0|1" */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    l_param = oal_atoi((const int8_t *)ac_name);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_ALL_OTA, OAL_SIZEOF(uint32_t));
    *((int32_t *)(st_write_msg.auc_value)) = l_param;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_all_ota::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_addba_req(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_addba_req_param_stru *pst_addba_req_param = OAL_PTR_NULL;
    mac_cfg_addba_req_param_stru st_addba_req_param; /* 临时保存获取的addba req的信息 */
    uint32_t ul_get_addr_idx;

    /*
     * 设置AMPDU关闭的配置命令:
     * hipriv "Hisilicon0 addba_req xx xx xx xx xx xx(mac地址) tidno ba_policy buffsize timeout"
     */
    /* 获取mac地址 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addba_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    memset_s((uint8_t *)&st_addba_req_param, OAL_SIZEOF(st_addba_req_param), 0, OAL_SIZEOF(st_addba_req_param));
    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), st_addba_req_param.auc_mac_addr, WLAN_MAC_ADDR_LEN);

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    /* 获取tid */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addba_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    if (OAL_STRLEN(ac_name) > 2) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addba_req::the addba req command is error[%x]!}\r\n", (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    st_addba_req_param.uc_tidno = (uint8_t)oal_atoi(ac_name);
    if (st_addba_req_param.uc_tidno >= WLAN_TID_MAX_NUM) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addba_req::the addba req command is error!uc_tidno is [%d]!}\r\n", st_addba_req_param.uc_tidno);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pc_param = pc_param + ul_off_set;

    /* 获取ba_policy */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addba_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_addba_req_param.en_ba_policy = (uint8_t)oal_atoi(ac_name);
    if (st_addba_req_param.en_ba_policy != MAC_BA_POLICY_IMMEDIATE) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addba_req::the ba policy is not correct! ba_policy is[%d]!}\r\n", st_addba_req_param.en_ba_policy);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pc_param = pc_param + ul_off_set;

    /* 获取buffsize */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addba_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_addba_req_param.us_buff_size = (uint16_t)oal_atoi(ac_name);

    pc_param = pc_param + ul_off_set;

    /* 获取timeout时间 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addba_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_addba_req_param.us_timeout = (uint16_t)oal_atoi(ac_name);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADDBA_REQ, OAL_SIZEOF(mac_cfg_addba_req_param_stru));

    /* 设置配置命令参数 */
    pst_addba_req_param = (mac_cfg_addba_req_param_stru *)(st_write_msg.auc_value);
    for (ul_get_addr_idx = 0; ul_get_addr_idx < WLAN_MAC_ADDR_LEN; ul_get_addr_idx++) {
        pst_addba_req_param->auc_mac_addr[ul_get_addr_idx] = st_addba_req_param.auc_mac_addr[ul_get_addr_idx];
    }

    pst_addba_req_param->uc_tidno = st_addba_req_param.uc_tidno;
    pst_addba_req_param->en_ba_policy = st_addba_req_param.en_ba_policy;
    pst_addba_req_param->us_buff_size = st_addba_req_param.us_buff_size;
    pst_addba_req_param->us_timeout = st_addba_req_param.us_timeout;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_addba_req_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addba_req::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_delba_req(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_delba_req_param_stru *pst_delba_req_param = OAL_PTR_NULL;
    mac_cfg_delba_req_param_stru st_delba_req_param; /* 临时保存获取的addba req的信息 */
    uint32_t ul_get_addr_idx;

    /*
     * 设置AMPDU关闭的配置命令:
     * hipriv "Hisilicon0 delba_req xx xx xx xx xx xx(mac地址) tidno direction reason_code"
     */
    /* 获取mac地址 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_delba_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    memset_s((uint8_t *)&st_delba_req_param, OAL_SIZEOF(st_delba_req_param), 0, OAL_SIZEOF(st_delba_req_param));
    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), st_delba_req_param.auc_mac_addr, WLAN_MAC_ADDR_LEN);
    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    /* 获取tid */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_delba_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    if (OAL_STRLEN(ac_name) > 2) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_delba_req::the delba_req req command is error[%x]!}\r\n", (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    st_delba_req_param.uc_tidno = (uint8_t)oal_atoi(ac_name);
    if (st_delba_req_param.uc_tidno >= WLAN_TID_MAX_NUM) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_delba_req::the delba_req req command is error! uc_tidno is[%d]!}\r\n", st_delba_req_param.uc_tidno);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pc_param = pc_param + ul_off_set;

    /* 获取direction */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_delba_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_delba_req_param.en_direction = (uint8_t)oal_atoi(ac_name);
    if (st_delba_req_param.en_direction >= MAC_BUTT_DELBA) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_delba_req::the direction is not correct! direction is[%d]!}\r\n", st_delba_req_param.en_direction);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DELBA_REQ, OAL_SIZEOF(mac_cfg_delba_req_param_stru));

    /* 设置配置命令参数 */
    pst_delba_req_param = (mac_cfg_delba_req_param_stru *)(st_write_msg.auc_value);
    for (ul_get_addr_idx = 0; ul_get_addr_idx < WLAN_MAC_ADDR_LEN; ul_get_addr_idx++) {
        pst_delba_req_param->auc_mac_addr[ul_get_addr_idx] = st_delba_req_param.auc_mac_addr[ul_get_addr_idx];
    }

    pst_delba_req_param->uc_tidno = st_delba_req_param.uc_tidno;
    pst_delba_req_param->en_direction = st_delba_req_param.en_direction;
    pst_delba_req_param->en_trigger = MAC_DELBA_TRIGGER_COMM;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_delba_req_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_delba_req::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_WMMAC

OAL_STATIC uint32_t wal_hipriv_addts_req(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_wmm_tspec_stru_param_stru *pst_addts_req_param;
    mac_cfg_wmm_tspec_stru_param_stru st_addts_req_param; /* 临时保存获取的addts req的信息 */

    /*
     * 设置发送ADDTS REQ配置命令:
     * hipriv "vap0 addts_req tid direction psb up nominal_msdu_size maximum_data_rate
     * minimum_data_rate mean_data_rate peak_data_rate minimum_phy_rate surplus_bandwidth_allowance"
     */
    /***********************************************************************************************
    TSPEC字段:
          --------------------------------------------------------------------------------------
          |TS Info|Nominal MSDU Size|Max MSDU Size|Min Serv Itvl|Max Serv Itvl|
          ---------------------------------------------------------------------------------------
    Octets:  | 3     |  2              |   2         |4            |4            |
          ---------------------------------------------------------------------------------------
          | Inactivity Itvl | Suspension Itvl | Serv Start Time |Min Data Rate | Mean Data Rate |
          ---------------------------------------------------------------------------------------
    Octets:  |4                | 4               | 4               |4             |  4             |
          ---------------------------------------------------------------------------------------
          |Peak Data Rate|Burst Size|Delay Bound|Min PHY Rate|Surplus BW Allowance  |Medium Time|
          ---------------------------------------------------------------------------------------
    Octets:  |4             |4         | 4         | 4          |  2                   |2          |
          ---------------------------------------------------------------------------------------

    TS info字段:
          ---------------------------------------------------------------------------------------
          |Reserved |TSID |Direction |1 |0 |Reserved |PSB |UP |Reserved |Reserved |Reserved |
          ---------------------------------------------------------------------------------------
    Bits:  |1        |4    |2         |  2  |1        |1   |3  |2        |1        |7        |
          ----------------------------------------------------------------------------------------
    ***********************************************************************************************/
    /* 获取tid，取值范围0~7 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    if (OAL_STRLEN(ac_name) > 2) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::the addba req command is error[%x]!}\r\n", (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    st_addts_req_param.ts_info.bit_tsid = (uint16_t)oal_atoi(ac_name);
    if (st_addts_req_param.ts_info.bit_tsid >= WLAN_TID_MAX_NUM) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::the addts req command is error!uc_tidno is [%d]!}\r\n", st_addts_req_param.ts_info.bit_tsid);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pc_param = pc_param + ul_off_set;

    /* 获取direction 00:uplink 01:downlink 10:reserved 11:Bi-directional */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_addts_req_param.ts_info.bit_direction = (uint16_t)oal_atoi(ac_name);
    if (st_addts_req_param.ts_info.bit_direction == MAC_WMMAC_DIRECTION_RESERVED) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::the direction is not correct! direction is[%d]!}\r\n", st_addts_req_param.ts_info.bit_direction);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pc_param = pc_param + ul_off_set;

    /* 获取PSB，1表示U-APSD，0表示legacy */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_addts_req_param.ts_info.bit_apsd = (uint16_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /* 获取UP */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    st_addts_req_param.ts_info.bit_user_prio = (uint16_t)oal_atoi(ac_name);

    pc_param = pc_param + ul_off_set;

    /* 获取Nominal MSDU Size ,第一位为1 */
    /*
        ------------
        |fixed|size|
        ------------
    bits:  |1    |15  |
        ------------
    */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    st_addts_req_param.us_norminal_msdu_size = (uint16_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /* 获取maximum MSDU size */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    st_addts_req_param.us_max_msdu_size = (uint16_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /* 获取minimum data rate */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    st_addts_req_param.ul_min_data_rate = (uint32_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /* 获取mean data rate */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    st_addts_req_param.ul_mean_data_rate = (uint32_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /* 获取peak data rate */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    st_addts_req_param.ul_peak_data_rate = (uint32_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /* 获取minimum PHY Rate */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    st_addts_req_param.ul_min_phy_rate = (uint32_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /* 获取surplus bandwidth allowance */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    st_addts_req_param.us_surplus_bw = (uint16_t)oal_atoi(ac_name);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADDTS_REQ, OAL_SIZEOF(mac_cfg_wmm_tspec_stru_param_stru));

    /* 设置配置命令参数 */
    pst_addts_req_param = (mac_cfg_wmm_tspec_stru_param_stru *)(st_write_msg.auc_value);

    pst_addts_req_param->ts_info.bit_tsid = st_addts_req_param.ts_info.bit_tsid;
    pst_addts_req_param->ts_info.bit_direction = st_addts_req_param.ts_info.bit_direction;
    pst_addts_req_param->ts_info.bit_apsd = st_addts_req_param.ts_info.bit_apsd;
    pst_addts_req_param->ts_info.bit_user_prio = st_addts_req_param.ts_info.bit_user_prio;
    pst_addts_req_param->us_norminal_msdu_size = st_addts_req_param.us_norminal_msdu_size;
    pst_addts_req_param->us_max_msdu_size = st_addts_req_param.us_max_msdu_size;
    pst_addts_req_param->ul_min_data_rate = st_addts_req_param.ul_min_data_rate;
    pst_addts_req_param->ul_mean_data_rate = st_addts_req_param.ul_mean_data_rate;
    pst_addts_req_param->ul_peak_data_rate = st_addts_req_param.ul_peak_data_rate;
    pst_addts_req_param->ul_min_phy_rate = st_addts_req_param.ul_min_phy_rate;
    pst_addts_req_param->us_surplus_bw = st_addts_req_param.us_surplus_bw;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_wmm_tspec_stru_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_addts_req::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_delts(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_wmm_tspec_stru_param_stru *pst_delts_param;
    mac_cfg_wmm_tspec_stru_param_stru st_delts_param;

    /* 设置删除TS的配置命令: hipriv "Hisilicon0 delts tidno" */
    /* 获取tsid */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_delts::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_delts_param.ts_info.bit_tsid = (uint8_t)oal_atoi(ac_name);
    if (st_delts_param.ts_info.bit_tsid >= WLAN_TID_MAX_NUM) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_delts::the delts command is error! tsid is[%d]!}\r\n", st_delts_param.ts_info.bit_tsid);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DELTS, OAL_SIZEOF(mac_cfg_wmm_tspec_stru_param_stru));

    /* 设置配置命令参数 */
    pst_delts_param = (mac_cfg_wmm_tspec_stru_param_stru *)(st_write_msg.auc_value);
    memset_s(pst_delts_param, OAL_SIZEOF(mac_cfg_wmm_tspec_stru_param_stru), 0, OAL_SIZEOF(mac_cfg_wmm_tspec_stru_param_stru));

    pst_delts_param->ts_info.bit_tsid = st_delts_param.ts_info.bit_tsid;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_wmm_tspec_stru_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_delts::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_wmmac_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint8_t uc_wmmac_switch;
    mac_cfg_wmm_ac_param_stru st_wmm_ac_param;

    /* 设置删除TS的配置命令: hipriv "vap0 wmmac_switch 1/0(使能) 0|1(WMM_AC认证使能) AC xxx(limit_medium_time)" */

    memset_s(&st_wmm_ac_param, OAL_SIZEOF(mac_cfg_wmm_ac_param_stru), 0, OAL_SIZEOF(mac_cfg_wmm_ac_param_stru));
    /* 获取mac地址 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_wmmac_switch::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    uc_wmmac_switch = (uint8_t)oal_atoi(ac_name);
    if (uc_wmmac_switch != OAL_FALSE) {
        uc_wmmac_switch = OAL_TRUE;
    }
    st_wmm_ac_param.en_wmm_ac_switch = uc_wmmac_switch;

    /* 获取auth flag */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_wmmac_switch::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    st_wmm_ac_param.en_auth_flag = (uint8_t)oal_atoi(ac_name);
    pc_param += ul_off_set;

    /* timeout period ms */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_wmmac_switch::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    st_wmm_ac_param.us_timeout_period = (uint16_t)oal_atoi(ac_name);
    pc_param += ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_wmmac_switch::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    st_wmm_ac_param.uc_factor = (uint8_t)oal_atoi(ac_name);
    pc_param += ul_off_set;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    /* 设置配置命令参数 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_WMMAC_SWITCH, OAL_SIZEOF(st_wmm_ac_param));
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                        (const void *)&st_wmm_ac_param,
                        OAL_SIZEOF(st_wmm_ac_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_delts::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_wmm_ac_param),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_delts::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#endif  // #ifdef _PRE_WLAN_FEATURE_WMMAC


OAL_STATIC uint32_t wal_hipriv_mem_info(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    int8_t auc_token[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_mem_pool_id_enum_uint8 en_pool_id;
    uint32_t ul_off_set;
    uint32_t ul_ret;

    /* 入参检查 */
    if (oal_unlikely(oal_any_null_ptr2(pst_cfg_net_dev, pc_param))) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_hipriv_mem_info::pst_net_dev or pc_param null ptr error [%x] [%x]!}\r\n", (uintptr_t)pst_cfg_net_dev, (uintptr_t)pc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获取内存池ID */
    ul_ret = wal_get_cmd_one_arg(pc_param, auc_token, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_mem_info::wal_get_cmd_one_arg return error code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    en_pool_id = (oal_mem_pool_id_enum_uint8)oal_atoi(auc_token);

    /* 打印内存池信息 */
    oal_mem_info(en_pool_id);

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_mem_leak(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    int8_t auc_token[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_mem_pool_id_enum_uint8 en_pool_id;
    uint32_t ul_off_set;
    uint32_t ul_ret;

    /* 入参检查 */
    if (oal_unlikely(oal_any_null_ptr2(pst_cfg_net_dev, pc_param))) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_hipriv_mem_leak::pst_net_dev or pc_param null ptr error [%x] [%x]!}\r\n", (uintptr_t)pst_cfg_net_dev, (uintptr_t)pc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获取内存池ID */
    ul_ret = wal_get_cmd_one_arg(pc_param, auc_token, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_mem_leak::wal_get_cmd_one_arg return error code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    en_pool_id = (oal_mem_pool_id_enum_uint8)oal_atoi(auc_token);
    if (en_pool_id > OAL_MEM_POOL_ID_SDT_NETBUF) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_mem_leak::mem pool id exceeds,en_pool_id[%d]!}\r\n", en_pool_id);
        return OAL_SUCC;
    }

    /* 检查内存池泄漏内存块 */
    oal_mem_leak(en_pool_id);

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_memory_info(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    uint8_t uc_meminfo_type = MAC_MEMINFO_BUTT;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    wal_msg_write_stru st_write_msg;
    mac_cfg_meminfo_stru *pst_meminfo_param = OAL_PTR_NULL;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 memoryinfo [all|dscr|netbuff|user|vap|sdio_sch_q] [pool_usage/pool_debug 0|1,..7<pool_id>]'!}\r\n");
        return ul_ret;
    }

    if (0 == (oal_strcmp("host", ac_name))) {
        oal_mem_print_pool_info();
        return OAL_SUCC;
    } else if (0 == (oal_strcmp("device", ac_name))) {
        hcc_print_device_mem_info();
        return OAL_SUCC;
    } else if (0 == (oal_strcmp("all", ac_name))) {
        uc_meminfo_type = MAC_MEMINFO_ALL;
    } else if (0 == (oal_strcmp("dscr", ac_name))) {
        uc_meminfo_type = MAC_MEMINFO_DSCR;
    } else if (0 == (oal_strcmp("netbuff", ac_name))) {
        uc_meminfo_type = MAC_MEMINFO_NETBUFF;
    } else if (0 == (oal_strcmp("user", ac_name))) {
        uc_meminfo_type = MAC_MEMINFO_USER;
    } else if (0 == (oal_strcmp("vap", ac_name))) {
        uc_meminfo_type = MAC_MEMINFO_VAP;
    } else if (0 == (oal_strcmp("sdio_sch_q", ac_name))) {
        uc_meminfo_type = MAC_MEMINFO_SDIO_TRX;
    } else if (0 == (oal_strcmp("pool_usage", ac_name))) {
        uc_meminfo_type = MAC_MEMINFO_POOL_INFO;
    } else if (0 == (oal_strcmp("pool_debug", ac_name))) {
        uc_meminfo_type = MAC_MEMINFO_POOL_DBG;
    } else if (0 == (oal_strcmp("sample_alloc", ac_name))) {
        uc_meminfo_type = MAC_MEMINFO_SAMPLE_ALLOC;
    } else if (0 == (oal_strcmp("sample_free", ac_name))) {
        uc_meminfo_type = MAC_MEMINFO_SAMPLE_FREE;
    } else {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_memory_info::wal_get_cmd_one_arg::second arg:: please check input!}\r\n");
        return OAL_FAIL;
    }

    /***************************************************************************
        抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DEVICE_MEM_INFO, OAL_SIZEOF(mac_cfg_meminfo_stru));
    pst_meminfo_param = (mac_cfg_meminfo_stru *)(st_write_msg.auc_value);
    pst_meminfo_param->uc_meminfo_type = uc_meminfo_type;
    /* host和device mempool个数不一致 ，这里用0xff给个default值 */
    pst_meminfo_param->uc_object_index = 0xff;

    if ((uc_meminfo_type == MAC_MEMINFO_POOL_INFO) || (uc_meminfo_type == MAC_MEMINFO_POOL_DBG) || (uc_meminfo_type == MAC_MEMINFO_SAMPLE_ALLOC)) {
        pc_param = pc_param + ul_off_set;

        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        /* 没有后续参数不退出 */
        if (ul_ret == OAL_SUCC) {
            pst_meminfo_param->uc_object_index = (uint8_t)oal_atoi(ac_name);
        }
    }

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_meminfo_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_memory_info::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_beacon_chain_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_tmp;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr2(pst_net_dev, pc_param))) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_hipriv_beacon_chain_switch::pst_net_dev or pc_param null ptr error %x, %x!}\r\n", (uintptr_t)pst_net_dev, (uintptr_t)pc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* beacon通道(0/1/2)切换开关的命令: hipriv "vap0 beacon_chain_switch 0 | 1 | 2"
        此处将解析出"0"或"1"或"2"存入ac_name
    */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_beacon_chain_switch::wal_get_cmd_one_arg return err_code %d!}\r\n", ul_ret);
        return ul_ret;
    }

    /* 针对解析出的不同命令，配置不同的通道 */
    if (0 == (oal_strcmp("0", ac_name))) {
        l_tmp = 0;
    } else if (0 == (oal_strcmp("1", ac_name))) {
        l_tmp = 1;
    } else if (0 == (oal_strcmp("2", ac_name))) {
        l_tmp = 2;
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_beacon_chain_switch::the beacon chain switch command is error %x!}\r\n", (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
        抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_BEACON_CHAIN_SWITCH, OAL_SIZEOF(int32_t));
    *((int32_t *)(st_write_msg.auc_value)) = l_tmp; /* 设置配置命令参数 */

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_beacon_chain_switch::return err code %d!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_btcoex_preempt_type(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    mac_btcoex_preempt_mgr_stru *pst_btcoex_preempt_mgr = NULL;
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;

    memset_s((uint8_t *)&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));

    /* 设置配置命令参数 */
    pst_btcoex_preempt_mgr = (mac_btcoex_preempt_mgr_stru *)st_write_msg.auc_value;

    /* 1.获取第一个参数: mode */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_preempt_type::wal_get_cmd_one_arg1 return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    pst_btcoex_preempt_mgr->uc_cfg_preempt_mode = (uint8_t)oal_atoi(ac_name);
    if (pst_btcoex_preempt_mgr->uc_cfg_preempt_mode > 6) {
        oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_preempt_type:: pst_btcoex_preempt_mgr->uc_cfg_preempt_mode [%d]!}\r\n",
                         pst_btcoex_preempt_mgr->uc_cfg_preempt_mode);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    /* 2.获取第2个参数 */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_preempt_type::wal_get_cmd_one_arg2 return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    /* 参数放宽配置，约束配置行为 */
    pst_btcoex_preempt_mgr->uc_cfg_preempt_type = (uint8_t)oal_atoi(ac_name);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_BTCOEX_PREEMPT_TYPE, OAL_SIZEOF(mac_btcoex_preempt_mgr_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_btcoex_preempt_mgr_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_preempt_type::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_btcoex_set_perf_param(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint32_t ul_ret;
    mac_btcoex_mgr_stru *pst_btcoex_mgr = NULL;
    uint32_t ul_off_set = 0;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };

    /* st_write_msg作清零操作 */
    memset_s(&st_write_msg, OAL_SIZEOF(wal_msg_write_stru), 0, OAL_SIZEOF(wal_msg_write_stru));

    /* 设置配置命令参数 */
    pst_btcoex_mgr = (mac_btcoex_mgr_stru *)st_write_msg.auc_value;

    /* 1.获取第一个参数: mode */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg1 return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    pst_btcoex_mgr->uc_cfg_btcoex_mode = (uint8_t)oal_atoi(ac_name);

    if (pst_btcoex_mgr->uc_cfg_btcoex_mode == 0) {
    } else if (pst_btcoex_mgr->uc_cfg_btcoex_mode == 1) {
        /* 偏移，取下一个参数 */
        pc_param = pc_param + ul_off_set;

        /* 2.获取第二个参数 */
        ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg2 return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }

        pst_btcoex_mgr->uc_cfg_btcoex_type = (uint8_t)oal_atoi(ac_name);
        if (pst_btcoex_mgr->uc_cfg_btcoex_type > 2) {
            oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param:: pst_btcoex_mgr->uc_cfg_btcoex_type error [%d], [0/1/2]!}\r\n",
                             pst_btcoex_mgr->uc_cfg_btcoex_type);
            return OAL_ERR_CODE_INVALID_CONFIG;
        }

        /* 偏移，取下一个参数 */
        pc_param = pc_param + ul_off_set;

        if (pst_btcoex_mgr->uc_cfg_btcoex_type == 0) {
            /* 3.获取第三个参数 */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg3 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.rx_size.en_btcoex_nss = (wlan_nss_enum_uint8)oal_atoi(ac_name);

            /* 偏移，取下一个参数 */
            pc_param = pc_param + ul_off_set;

            /* 获取第四个参数 */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg4 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.threhold.uc_20m_low = (uint8_t)oal_atoi(ac_name);

            /* 偏移，取下一个参数 */
            pc_param = pc_param + ul_off_set;

            /* 获取第五个参数 */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg5 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.threhold.uc_20m_high = (uint8_t)oal_atoi(ac_name);

            /* 偏移，取下一个参数 */
            pc_param = pc_param + ul_off_set;

            /* 获取第六个参数: */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg6 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.threhold.uc_40m_low = (uint8_t)oal_atoi(ac_name);

            /* 偏移，取下一个参数 */
            pc_param = pc_param + ul_off_set;

            /* 获取第七个参数: */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg7 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.threhold.us_40m_high = (uint16_t)oal_atoi(ac_name);
        } else if (pst_btcoex_mgr->uc_cfg_btcoex_type == 1) {
            /* 3.获取第三个参数 */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg3 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.rx_size.en_btcoex_nss = (wlan_nss_enum_uint8)oal_atoi(ac_name);

            /* 偏移，取下一个参数 */
            pc_param = pc_param + ul_off_set;

            /* 获取第四个参数 */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg3 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.rx_size.uc_grade = (uint8_t)oal_atoi(ac_name);

            /* 偏移，取下一个参数 */
            pc_param = pc_param + ul_off_set;

            /* 获取第五个参数 */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg4 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.rx_size.uc_rx_size0 = (uint8_t)oal_atoi(ac_name);

            /* 偏移，取下一个参数 */
            pc_param = pc_param + ul_off_set;

            /* 获取第六个参数: */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg5 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.rx_size.uc_rx_size1 = (uint8_t)oal_atoi(ac_name);

            /* 偏移，取下一个参数 */
            pc_param = pc_param + ul_off_set;

            /* 获取第七个参数: */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg6 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.rx_size.uc_rx_size2 = (uint8_t)oal_atoi(ac_name);

            /* 偏移，取下一个参数 */
            pc_param = pc_param + ul_off_set;

            /* 获取第八个参数: */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg7 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.rx_size.uc_rx_size3 = (uint8_t)oal_atoi(ac_name);
        } else {
            /* 3.获取第三个参数 */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg3 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.rssi_param.en_rssi_limit_on = (oal_bool_enum_uint8)oal_atoi(ac_name);

            /* 偏移，取下一个参数 */
            pc_param = pc_param + ul_off_set;

            /* 获取第四个参数: */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg6 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.rssi_param.en_rssi_log_on = (oal_bool_enum_uint8)oal_atoi(ac_name);

            /* 偏移，取下一个参数 */
            pc_param = pc_param + ul_off_set;

            /* 获取第五个参数 */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg3 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.rssi_param.uc_cfg_rssi_detect_cnt = (uint8_t)oal_atoi(ac_name);

            /* 偏移，取下一个参数 */
            pc_param = pc_param + ul_off_set;

            /* 获取第六个参数 */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg4 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.rssi_param.c_cfg_rssi_detect_mcm_down_th = (int8_t)oal_atoi(ac_name);

            /* 偏移，取下一个参数 */
            pc_param = pc_param + ul_off_set;

            /* 获取第七个参数: */
            ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::wal_get_cmd_one_arg5 return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            pst_btcoex_mgr->pri_data.rssi_param.c_cfg_rssi_detect_mcm_up_th = (int8_t)oal_atoi(ac_name);
        }
    } else {
        oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param::pst_btcoex_mgr->uc_cfg_btcoex_mode err_code [%d]!}\r\n",
                         pst_btcoex_mgr->uc_cfg_btcoex_mode);
        return OAL_FAIL;
    }

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_BTCOEX_SET_PERF_PARAM, OAL_SIZEOF(mac_btcoex_mgr_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_btcoex_mgr_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_btcoex_set_perf_param:: return err code = [%d].}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE

OAL_STATIC uint32_t wal_hipriv_dev_customize_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint32_t ul_tmp = 0;
    uint32_t ul_ret;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    hwifi_get_cfg_params();

    memset_s((uint8_t *)&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_COEX, "{wal_hipriv_dev_customize_info::wal_get_cmd_one_arg return err_code %d!}\r\n", ul_ret);
        return ul_ret;
    }

    /* 针对解析出的不同命令 */
    if (0 == (oal_strcmp("1", ac_name))) {
        ul_tmp = 1;
    }

    /***************************************************************************
        抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SHOW_DEV_CUSTOMIZE_INFOS, OAL_SIZEOF(int32_t));
    *((uint32_t *)(st_write_msg.auc_value)) = ul_tmp; /* 设置配置命令参数 */

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_dev_customize_info::return err code %d!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_ONLINE_DPD
#ifdef _PRE_WLAN_RF_CALI_DEBUG

OAL_STATIC uint32_t wal_hipriv_dpd_cfg(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;

    /***************************************************************************
                              抛事件到wal层处理
    ***************************************************************************/
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value), pc_param, OAL_STRLEN(pc_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_dpd_cfg::memcpy fail!}");
        return OAL_FAIL;
    }

    st_write_msg.auc_value[OAL_STRLEN(pc_param)] = '\0';

    us_len = (uint16_t)(OAL_STRLEN(pc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DPD, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_reg_write::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_NRCOEX
/*
 * 函 数 名  : wal_hipriv_nrcoex_set_prior
 * 功能描述  : 设置NR共存WiFi优先级
 */
OAL_STATIC uint32_t wal_hipriv_nrcoex_set_prior(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint8_t uc_val;
    wal_msg_write_stru st_write_msg;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_nrcoex_set_prior::wal_get_cmd_one_arg return err_code %d!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    uc_val = (uint8_t)oal_atoi((const int8_t *)ac_name);

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_NRCOEX_PRIORITY_SET, OAL_SIZEOF(uint32_t));

    /* 设置配置命令参数 */
    *((uint8_t *)(st_write_msg.auc_value)) = uc_val;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint8_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_nrcoex_set_prior::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_nrcoex_cfg_test(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;

    /***************************************************************************
                              抛事件到wal层处理
    ***************************************************************************/
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value), pc_param, OAL_STRLEN(pc_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_nrcoex_cfg_test::memcpy fail!}");
        return OAL_FAIL;
    }

    st_write_msg.auc_value[OAL_STRLEN(pc_param)] = '\0';

    us_len = (uint16_t)(OAL_STRLEN(pc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_NRCOEX_TEST, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_reg_write::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif


uint32_t wal_hipriv_set_txpower(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    int32_t l_pwer;
    uint32_t ul_off_set;
    int8_t ac_val[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_idx = 0;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_val, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_txpower::wal_get_cmd_one_arg vap name return err_code %d!}\r\n", ul_ret);
        return ul_ret;
    }

    if (ac_val[0] == '-') {
        l_idx++;
    }

    /* 输入命令合法性检测 */
    while ('\0' != ac_val[l_idx]) {
        if (isdigit(ac_val[l_idx])) {
            l_idx++;
            continue;
        } else {
            l_idx++;
            oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_txpower::input illegal!}\r\n");
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }

    l_pwer = oal_atoi(ac_val);

    if (l_pwer >= WLAN_MAX_TXPOWER * 10 || l_pwer <= WLAN_MIN_TXPOWER * 10) { /* 参数异常: 功率限制大于1W */
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_txpower::invalid argument!}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_txpower::val[%d]!}", l_pwer);
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_TX_POWER, OAL_SIZEOF(int32_t));
    *((int32_t *)(st_write_msg.auc_value)) = l_pwer;
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_txpower::return err code %d!}", l_ret);
        return (uint32_t)l_ret;
    }
    /* 读取返回的错误码 */
    ul_ret = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_set_bw fail, err code[%u]!}\r\n", ul_ret);
    }

    return ul_ret;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))

OAL_STATIC uint32_t wal_ioctl_set_beacon_interval(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_beacon_interval;
    uint32_t ul_off_set;
    int8_t ac_beacon_interval[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;

    /* 设备在up状态不允许配置，必须先down */
    if (0 != (OAL_IFF_RUNNING & oal_netdevice_flags(pst_net_dev))) {
        oam_error_log1(0, OAM_SF_CFG, "{wal_ioctl_set_beacon_interval::device is busy, please down it firs %d!}\r\n", oal_netdevice_flags(pst_net_dev));
        return OAL_ERR_CODE_CONFIG_BUSY;
    }

    /* pc_param指向新创建的net_device的name, 将其取出存放到ac_name中 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_beacon_interval, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_ioctl_set_beacon_interval::wal_get_cmd_one_arg vap name return err_code %d!}\r\n", ul_ret);
        return ul_ret;
    }

    l_beacon_interval = oal_atoi(ac_beacon_interval);
    oam_info_log1(0, OAM_SF_ANY, "{wal_ioctl_set_beacon_interval::l_beacon_interval = %d!}\r\n", l_beacon_interval);

    /***************************************************************************
        抛事件到wal层处理
    ***************************************************************************/
    /* 填写消息 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_BEACON_INTERVAL, OAL_SIZEOF(int32_t));
    *((int32_t *)(st_write_msg.auc_value)) = l_beacon_interval;

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_ioctl_set_beacon_interval::return err code %d!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_start_vap(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    oam_error_log0(0, OAM_SF_CFG, "DEBUG:: priv start enter.");
    wal_netdev_open(pst_net_dev, OAL_FALSE);
    return OAL_SUCC;
}
#endif


OAL_STATIC uint32_t wal_hipriv_list_sta(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_LIST_STA, OAL_SIZEOF(int32_t));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_list_sta::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_list_channel(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_LIST_CHAN, OAL_SIZEOF(int32_t));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_list_channel::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_start_scan(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint8_t uc_is_p2p0_scan;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_START_SCAN, OAL_SIZEOF(int32_t));

    uc_is_p2p0_scan = (oal_memcmp(pst_net_dev->name, "p2p0", OAL_STRLEN("p2p0")) == 0) ? 1 : 0;
    // uc_is_p2p0_scan = (pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_P2P_DEVICE)?1:0;
    st_write_msg.auc_value[0] = uc_is_p2p0_scan;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_start_scan::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_start_join(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint32_t ul_ret;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_START_JOIN, OAL_SIZEOF(int32_t));

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_start_join::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    /* 将要关联AP的编号复制到事件msg中，AP编号是数字的ASSCI码，不超过4个字节 */
    if (EOK != memcpy_s((int8_t *)st_write_msg.auc_value, sizeof(st_write_msg.auc_value), (int8_t *)ac_name, OAL_SIZEOF(int32_t))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_start_join::memcpy fail}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_start_join::return err codereturn err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_start_deauth(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_START_DEAUTH, OAL_SIZEOF(int32_t));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_start_deauth::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_kick_user(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_kick_user_param_stru *pst_kick_user_param = OAL_PTR_NULL;
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN] = { 0, 0, 0, 0, 0, 0 };

    /* 去关联1个用户的命令 hipriv "vap0 kick_user xx:xx:xx:xx:xx:xx" */
    /* 获取mac地址 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_kick_user::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), auc_mac_addr, WLAN_MAC_ADDR_LEN);
    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_KICK_USER, OAL_SIZEOF(mac_cfg_kick_user_param_stru));

    /* 设置配置命令参数 */
    pst_kick_user_param = (mac_cfg_kick_user_param_stru *)(st_write_msg.auc_value);
    oal_set_mac_addr(pst_kick_user_param->auc_mac_addr, auc_mac_addr);

    /* 填写去关联reason code */
    pst_kick_user_param->us_reason_code = MAC_UNSPEC_REASON;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_kick_user_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_kick_user::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_get_hipkt_stat(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    // sh hipriv.sh "wlan0 get_hipkt_stat"
    /* 申请事件内存 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_GET_HIPKT_STAT, OAL_SIZEOF(uint8_t));

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint8_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_get_hipkt_stat:: return err_code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_flowctl_param(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_ret;
    uint32_t ul_off_set = 0;
    int32_t l_ret;
    int8_t ac_param[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    mac_cfg_flowctl_param_stru st_flowctl_param;
    mac_cfg_flowctl_param_stru *pst_param = NULL;

    // sh hipriv.sh "Hisilicon0 set_flowctl_param 0/1/2/3 20 20 40"
    // 0/1/2/3 分别代表be,bk,vi,vo
    /* 获取队列类型参数 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_param, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_flowctl_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    st_flowctl_param.uc_queue_type = (uint8_t)oal_atoi(ac_param);

    /* 设置队列对应的每次调度报文个数 */
    pc_param += ul_off_set;
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_param, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_flowctl_param::wal_get_cmd_one_arg burst_limit return err_code %d!}\r\n", ul_ret);
        return (uint32_t)ul_ret;
    }
    st_flowctl_param.us_burst_limit = (uint16_t)oal_atoi(ac_param);

    /* 设置队列对应的流控low_waterline */
    pc_param += ul_off_set;
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_param, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_flowctl_param::wal_get_cmd_one_arg low_waterline return err_code %d!}\r\n", ul_ret);
        return (uint32_t)ul_ret;
    }
    st_flowctl_param.us_low_waterline = (uint16_t)oal_atoi(ac_param);

    /* 设置队列对应的流控high_waterline */
    pc_param += ul_off_set;
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_param, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_flowctl_param::wal_get_cmd_one_arg high_waterline return err_code %d!}\r\n", ul_ret);
        return (uint32_t)ul_ret;
    }
    st_flowctl_param.us_high_waterline = (uint16_t)oal_atoi(ac_param);

    /* 申请事件内存 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_FLOWCTL_PARAM, OAL_SIZEOF(mac_cfg_flowctl_param_stru));
    pst_param = (mac_cfg_flowctl_param_stru *)(st_write_msg.auc_value);

    pst_param->uc_queue_type = st_flowctl_param.uc_queue_type;
    pst_param->us_burst_limit = st_flowctl_param.us_burst_limit;
    pst_param->us_low_waterline = st_flowctl_param.us_low_waterline;
    pst_param->us_high_waterline = st_flowctl_param.us_high_waterline;

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_flowctl_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_flowctl_param:: return err_code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_get_flowctl_stat(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    // sh hipriv.sh "Hisilicon0 get_flowctl_stat"
    /* 申请事件内存 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_GET_FLOWCTL_STAT, OAL_SIZEOF(uint8_t));

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint8_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_get_flowctl_stat:: return err_code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_event_queue_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    return frw_event_queue_info();
}


OAL_STATIC uint32_t wal_hipriv_send_bar(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_pause_tid_param_stru *pst_pause_tid_param = OAL_PTR_NULL;
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN] = { 0, 0, 0, 0, 0, 0 };
    uint8_t uc_tid;

    /* 获取mac地址 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_send_bar::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), auc_mac_addr, WLAN_MAC_ADDR_LEN);
    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_send_bar::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    uc_tid = (uint8_t)oal_atoi(ac_name);

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SEND_BAR, OAL_SIZEOF(mac_cfg_pause_tid_param_stru));

    /* 设置配置命令参数 */
    pst_pause_tid_param = (mac_cfg_pause_tid_param_stru *)(st_write_msg.auc_value);
    oal_set_mac_addr(pst_pause_tid_param->auc_mac_addr, auc_mac_addr);
    pst_pause_tid_param->uc_tid = uc_tid;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_pause_tid_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_send_bar::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


uint32_t wal_hipriv_amsdu_tx_on(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_ampdu_tx_on_param_stru *pst_aggr_tx_on_param = OAL_PTR_NULL;
    uint8_t uc_aggr_tx_on;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_amsdu_tx_on::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    uc_aggr_tx_on = (uint8_t)oal_atoi(ac_name);

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_AMSDU_TX_ON, OAL_SIZEOF(mac_cfg_ampdu_tx_on_param_stru));

    /* 设置配置命令参数 */
    pst_aggr_tx_on_param = (mac_cfg_ampdu_tx_on_param_stru *)(st_write_msg.auc_value);
    pst_aggr_tx_on_param->uc_aggr_tx_on = uc_aggr_tx_on;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ampdu_tx_on_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_amsdu_tx_on::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_frag_threshold(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_ret;
    int32_t l_cfg_rst;
    uint16_t us_len;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_off_set = 0;
    mac_cfg_frag_threshold_stru *pst_threshold = OAL_PTR_NULL;
    uint32_t ul_threshold;

    /* 获取分片门限 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_frag_threshold::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    ul_threshold = (uint16_t)oal_atoi(ac_name);
    pc_param += ul_off_set;

    if ((ul_threshold < WLAN_FRAG_THRESHOLD_MIN) ||
        (ul_threshold > WLAN_FRAG_THRESHOLD_MAX)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_frag_threshold::ul_threshold value error [%d]!}\r\n", ul_threshold);
        return OAL_FAIL;
    }

    pst_threshold = (mac_cfg_frag_threshold_stru *)(st_write_msg.auc_value);
    pst_threshold->ul_frag_threshold = ul_threshold;

    /***************************************************************************
                              抛事件到wal层处理
    ***************************************************************************/
    us_len = OAL_SIZEOF(mac_cfg_frag_threshold_stru);
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_FRAG_THRESHOLD_REG, us_len);

    l_cfg_rst = wal_send_cfg_event(pst_net_dev,
                                       WAL_MSG_TYPE_WRITE,
                                       WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                       (uint8_t *)&st_write_msg,
                                       OAL_FALSE,
                                       OAL_PTR_NULL);

    if (oal_unlikely(l_cfg_rst != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_frag_threshold::return err code [%d]!}\r\n", l_cfg_rst);
        return (uint32_t)l_cfg_rst;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_wmm_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_ret;
    int32_t l_cfg_rst;
    uint16_t us_len;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_off_set = 0;
    uint8_t uc_open_wmm;

    /* 获取设定的值 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_wmm_switch::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    uc_open_wmm = (uint8_t)oal_atoi(ac_name);
    pc_param += ul_off_set;

    /***************************************************************************
                              抛事件到wal层处理
    ***************************************************************************/
    us_len = OAL_SIZEOF(uint8_t);
    *(uint8_t *)(st_write_msg.auc_value) = uc_open_wmm;
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_WMM_SWITCH, us_len);

    l_cfg_rst = wal_send_cfg_event(pst_net_dev,
                                       WAL_MSG_TYPE_WRITE,
                                       WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                       (uint8_t *)&st_write_msg,
                                       OAL_FALSE,
                                       OAL_PTR_NULL);
    if (oal_unlikely(l_cfg_rst != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_wmm_switch::return err code [%d]!}\r\n", l_cfg_rst);
        return (uint32_t)l_cfg_rst;
    }

    return OAL_SUCC;
}

uint32_t wal_hipriv_ampdu_tx_on(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_ampdu_tx_on_param_stru *pst_aggr_tx_on_param = OAL_PTR_NULL;
    uint8_t uc_aggr_tx_on;
    uint8_t uc_snd_type = 0;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_ampdu_tx_on::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    uc_aggr_tx_on = (uint8_t)oal_atoi(ac_name);

    /* 只有硬件聚合需要配置第二参数 */
    if ((uc_aggr_tx_on & BIT3) || (uc_aggr_tx_on & BIT2) || (uc_aggr_tx_on & BIT1)) {
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_ampdu_tx_on::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }

        uc_snd_type = (uint8_t)oal_atoi(ac_name);
    }

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_AMPDU_TX_ON, OAL_SIZEOF(mac_cfg_ampdu_tx_on_param_stru));

    /* 设置配置命令参数 */
    pst_aggr_tx_on_param = (mac_cfg_ampdu_tx_on_param_stru *)(st_write_msg.auc_value);
    pst_aggr_tx_on_param->uc_aggr_tx_on = uc_aggr_tx_on;
    if (uc_snd_type > 1) {
        pst_aggr_tx_on_param->uc_snd_type = 1;
        pst_aggr_tx_on_param->en_aggr_switch_mode = AMPDU_SWITCH_BY_BA_LUT;
    } else {
        pst_aggr_tx_on_param->uc_snd_type = uc_snd_type;
        pst_aggr_tx_on_param->en_aggr_switch_mode = AMPDU_SWITCH_BY_DEL_BA;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ampdu_tx_on_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_ampdu_tx_on::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_HIEX

OAL_STATIC uint32_t wal_hipriv_set_user_hiex_enable(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru             st_write_msg;
    uint32_t                     ul_off_set;
    int8_t                       ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    uint32_t                     ul_ret;
    int32_t                      l_ret;
    mac_cfg_user_hiex_param_stru  *pst_user_hiex_param = OAL_PTR_NULL;
    uint8_t                      auc_mac_addr[WLAN_MAC_ADDR_LEN] = { 0 };
    uint32_t                     ul_hiex_flag;

    /* 获取mac地址 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
                         "{wal_hipriv_set_user_hiex_enable::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), auc_mac_addr, WLAN_MAC_ADDR_LEN);
    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
                         "{wal_hipriv_set_user_hiex_enable::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    ul_hiex_flag = oal_strtol(ac_name, NULL, NUM_16_BITS);

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_USER_HIEX_ENABLE, OAL_SIZEOF(mac_cfg_user_hiex_param_stru));

    /* 设置配置命令参数 */
    pst_user_hiex_param = (mac_cfg_user_hiex_param_stru *)(st_write_msg.auc_value);
    oal_set_mac_addr(pst_user_hiex_param->auc_mac_addr, auc_mac_addr);
    *(uint32_t *)&pst_user_hiex_param->st_hiex_cap = ul_hiex_flag;

    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_user_hiex_param_stru), (uint8_t *)&st_write_msg,
        OAL_FALSE, OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_user_hiex_enable::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t _wal_hipriv_set_user_hiex_enable(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    if (g_wlan_spec_cfg->feature_hiex_is_open) {
        return wal_hipriv_set_user_hiex_enable(pst_net_dev, pc_param);
    }
    return OAL_SUCC;
}
#endif


OAL_STATIC uint32_t wal_hipriv_reset_device(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value), pc_param, OAL_STRLEN(pc_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_reset_device::memcpy fail}");
        return OAL_FAIL;
    }

    st_write_msg.auc_value[OAL_STRLEN(pc_param)] = '\0';

    us_len = (uint16_t)(OAL_STRLEN(pc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_RESET_HW, us_len);

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_reset_device::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_reset_operate(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;

    if (oal_unlikely(WAL_MSG_WRITE_MAX_LEN <= OAL_STRLEN(pc_param))) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_reset_operate:: pc_param overlength is %d}\n", OAL_STRLEN(pc_param));
        return OAL_FAIL;
    }

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value), pc_param, OAL_STRLEN(pc_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_reset_operate::memcpy fail!}");
        return OAL_FAIL;
    }

    st_write_msg.auc_value[OAL_STRLEN(pc_param)] = '\0';

    us_len = (uint16_t)(OAL_STRLEN(pc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_RESET_HW_OPERATE, us_len);

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_reset_operate::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_DFT_STAT

OAL_STATIC uint32_t wal_hipriv_usr_queue_stat(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_usr_queue_param_stru st_usr_queue_param;

    /* sh hipriv.sh "vap_name usr_queue_stat XX:XX:XX:XX:XX:XX 0|1" */
    memset_s((uint8_t *)&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
    memset_s((uint8_t *)&st_usr_queue_param, OAL_SIZEOF(st_usr_queue_param), 0, OAL_SIZEOF(st_usr_queue_param));

    /* 获取用户mac地址 */
    ul_ret = wal_hipriv_get_mac_addr(pc_param, st_usr_queue_param.auc_user_macaddr, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_usr_queue_stat::wal_hipriv_get_mac_addr return [%d].}", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_usr_queue_stat::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_usr_queue_param.uc_param = (uint8_t)oal_atoi(ac_name);

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_USR_QUEUE_STAT, OAL_SIZEOF(st_usr_queue_param));

    /* 填写消息体，参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                        &st_usr_queue_param, OAL_SIZEOF(st_usr_queue_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_usr_queue_stat::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_usr_queue_param),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_usr_queue_stat::wal_send_cfg_event return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_report_vap_stat(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint8_t uc_param;

    /* sh hipriv.sh "vap_name vap _stat  0|1" */

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_report_vap_stat::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    uc_param = (uint8_t)oal_atoi(ac_name);

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_VAP_STAT, OAL_SIZEOF(uc_param));

    /* 填写消息体，参数 */
    st_write_msg.auc_value[0] = uc_param;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uc_param),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_report_vap_stat::wal_send_cfg_event return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_report_all_stat(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;

    /* sh hipriv.sh "Hisilicon0 reprt_all_stat type(phy/machw/mgmt/irq/all)  0|1" */
    /* 获取repot类型 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value), pc_param, OAL_STRLEN(pc_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_report_all_stat::memcpy fail!}");
        return OAL_FAIL;
    }

    st_write_msg.auc_value[OAL_STRLEN(pc_param)] = '\0';
    us_len = (uint16_t)(OAL_STRLEN(pc_param) + 1);
    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ALL_STAT, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_report_all_stat::wal_send_cfg_event return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif


OAL_STATIC uint32_t wal_hipriv_set_ampdu_aggr_num(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    mac_cfg_aggr_num_stru st_aggr_num_ctl = { 0 };
    uint32_t ul_ret;
    int32_t l_ret;

    memset_s((uint8_t *)&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ampdu_aggr_num::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    st_aggr_num_ctl.uc_aggr_num_switch = (uint8_t)oal_atoi(ac_name);
    if (st_aggr_num_ctl.uc_aggr_num_switch == 0) {
        /* 不指定聚合个数时，聚合个数恢复为0 */
        st_aggr_num_ctl.uc_aggr_num = 0;
    } else {
        /* 获取聚合个数 */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ampdu_aggr_num::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }

        st_aggr_num_ctl.uc_aggr_num = (uint8_t)oal_atoi(ac_name);

        /* 超过聚合最大限制判断 */
        if (st_aggr_num_ctl.uc_aggr_num > WLAN_AMPDU_TX_MAX_NUM) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ampdu_aggr_num::exceed max aggr num [%d]!}\r\n", st_aggr_num_ctl.uc_aggr_num);
            return ul_ret;
        }
    }

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_AGGR_NUM, OAL_SIZEOF(st_aggr_num_ctl));

    /* 填写消息体，参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                        &st_aggr_num_ctl, OAL_SIZEOF(st_aggr_num_ctl))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_ampdu_aggr_num::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_aggr_num_ctl),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ampdu_aggr_num::wal_send_cfg_event return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}



OAL_STATIC uint32_t wal_hipriv_set_amsdu_aggr_num(oal_net_device_stru *netdev, int8_t *param)
{
    wal_msg_write_stru write_msg;
    uint32_t offset;
    uint32_t ret;
    int8_t cmd_param[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint8_t amsdu_aggr_num;

    memset_s((uint8_t *)&write_msg, OAL_SIZEOF(wal_msg_write_stru), 0, OAL_SIZEOF(wal_msg_write_stru));

    ret = wal_get_cmd_one_arg(param, cmd_param, WAL_HIPRIV_CMD_NAME_MAX_LEN, &offset);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_amsdu_aggr_num::wal_get_cmd_one_arg err_code[%d]}", ret);
        return ret;
    }

    amsdu_aggr_num = (uint8_t)oal_atoi(cmd_param);

    WAL_WRITE_MSG_HDR_INIT(&write_msg, WLAN_CFGID_SET_AMSDU_AGGR_NUM, OAL_SIZEOF(uint8_t));
    if (memcpy_s(write_msg.auc_value, OAL_SIZEOF(write_msg.auc_value), &amsdu_aggr_num, OAL_SIZEOF(uint8_t)) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_amsdu_aggr_num::write_msg.auc_value memcpy fail}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    return wal_send_cfg_event(netdev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint8_t),
                              (uint8_t *)&write_msg, OAL_FALSE, OAL_PTR_NULL);
}


OAL_STATIC uint32_t wal_hipriv_set_stbc_cap(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint32_t ul_value;

    if (oal_unlikely(oal_any_null_ptr2(pst_cfg_net_dev, pc_param))) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_hipriv_set_stbc_cap::pst_cfg_net_dev or pc_param null ptr error %x, %x!}\r\n", (uintptr_t)pst_cfg_net_dev, (uintptr_t)pc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* STBC设置开关的命令: hipriv "vap0 set_stbc_cap 0 | 1"
            此处将解析出"1"或"0"存入ac_name
    */

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_stbc_cap::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    /* 针对解析出的不同命令，设置TDLS禁用开关 */
    if (0 == (oal_strcmp("0", ac_name))) {
        ul_value = 0;
    } else if (0 == (oal_strcmp("1", ac_name))) {
        ul_value = 1;
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_stbc_cap::the set stbc command is error %x!}\r\n", (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_STBC_CAP, OAL_SIZEOF(uint32_t));

    /* 设置配置命令参数 */
    *((uint32_t *)(st_write_msg.auc_value)) = ul_value;

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_stbc_cap::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_ldpc_cap(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint32_t ul_value;

    if (oal_unlikely(oal_any_null_ptr2(pst_cfg_net_dev, pc_param))) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_hipriv_set_ldpc_cap::pst_cfg_net_dev or pc_param null ptr error %x, %x!}\r\n", (uintptr_t)pst_cfg_net_dev, (uintptr_t)pc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* LDPC设置开关的命令: hipriv "vap0 set_ldpc_cap 0 | 1"
            此处将解析出"1"或"0"存入ac_name
    */

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ldpc_cap::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    /* 针对解析出的不同命令，设置TDLS禁用开关 */
    if (0 == (oal_strcmp("0", ac_name))) {
        ul_value = 0;
    } else if (0 == (oal_strcmp("1", ac_name))) {
        ul_value = 1;
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ldpc_cap::the set ldpc command is error %x!}\r\n", (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_LDPC_CAP, OAL_SIZEOF(uint32_t));

    /* 设置配置命令参数 */
    *((uint32_t *)(st_write_msg.auc_value)) = ul_value;

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ldpc_cap::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_txbf_cap(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint32_t ul_value;

    if (oal_unlikely(oal_any_null_ptr2(pst_net_dev, pc_param))) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_hipriv_set_txbf_cap::v or pc_param null ptr error %p, %p!}\r\n", (uintptr_t)pst_net_dev, (uintptr_t)pc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /************************************************************
      TXBF设置开关的命令: sh hipriv "vap0 set_txbf_cap 0 | 1 | 2 |3"
             bit0表示RX(bfee)能力     bit1表示TX(bfer)能力
    *************************************************************/

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if ((ul_ret != OAL_SUCC) || (0 == (oal_strcmp("help", ac_name)))) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_txbf_cap::set txbf cap [bit0 indicates RX(bfee), bit1 TX(bfer)], ret[%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    ul_value = (uint32_t)oal_atoi(ac_name);

    if (ul_value > 3) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_set_txbf_cap::set txbf cap [0 ~ 3]!}\r\n", ul_value);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_TXBF_SWITCH, OAL_SIZEOF(uint32_t));

    /* 设置配置命令参数 */
    *((uint32_t *)(st_write_msg.auc_value)) = ul_value;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_txbf_cap::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_alg(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;  // FIXME : st_write_msg can only carry bytes less than 48
    int32_t l_ret;
    uint32_t ul_off_set;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int8_t *pc_tmp = (int8_t *)pc_param;
    uint16_t us_config_len;
    uint16_t us_param_len;

    mac_ioctl_alg_config_stru st_alg_config;

    st_alg_config.uc_argc = 0;
    while (OAL_SUCC == wal_get_cmd_one_arg(pc_tmp, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set)) {
        st_alg_config.auc_argv_offset[st_alg_config.uc_argc] = (uint8_t)((uint8_t)(pc_tmp - pc_param) + (uint8_t)ul_off_set - (uint8_t)OAL_STRLEN(ac_arg));
        pc_tmp += ul_off_set;
        st_alg_config.uc_argc++;

        if (st_alg_config.uc_argc > DMAC_ALG_CONFIG_MAX_ARG) {
            oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_alg::wal_hipriv_alg error, argc too big [%d]!}\r\n", st_alg_config.uc_argc);
            return OAL_FAIL;
        }
    }

    if (st_alg_config.uc_argc == 0) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_alg::argc=0!}\r\n");
        return OAL_FAIL;
    }

    us_param_len = (uint16_t)OAL_STRLEN(pc_param);
    if (us_param_len > WAL_MSG_WRITE_MAX_LEN - 1 - OAL_SIZEOF(mac_ioctl_alg_config_stru)) {
        return OAL_FAIL;
    }

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    us_config_len = OAL_SIZEOF(mac_ioctl_alg_config_stru) + us_param_len + 1;
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ALG, us_config_len);
    l_ret = memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value), &st_alg_config, OAL_SIZEOF(mac_ioctl_alg_config_stru));
    l_ret += memcpy_s(st_write_msg.auc_value + OAL_SIZEOF(mac_ioctl_alg_config_stru),
                      sizeof(st_write_msg.auc_value) - OAL_SIZEOF(mac_ioctl_alg_config_stru), pc_param, us_param_len + 1);
    if (l_ret != EOK) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_alg::memcpy fail! [%d]}", l_ret);
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_config_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_alg::wal_send_cfg_event return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_show_stat_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    oam_stats_report_info_to_sdt(OAM_OTA_TYPE_DEV_STAT_INFO);
    oam_stats_report_info_to_sdt(OAM_OTA_TYPE_VAP_STAT_INFO);
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_intf_det_log(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    mac_ioctl_alg_intfdet_log_param_stru *pst_alg_intfdet_log_param = OAL_PTR_NULL;
    wal_ioctl_alg_cfg_stru st_alg_cfg;
    uint8_t uc_map_index = 0;
    int32_t l_ret;

    pst_alg_intfdet_log_param = (mac_ioctl_alg_intfdet_log_param_stru *)(st_write_msg.auc_value);

    /* 获取配置参数名称 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_intf_det_log::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param = pc_param + ul_off_set;

    /* 寻找匹配的命令 */
    st_alg_cfg = g_ast_alg_cfg_map[0];
    while (st_alg_cfg.pc_name != OAL_PTR_NULL) {
        if (0 == oal_strcmp(st_alg_cfg.pc_name, ac_name)) {
            break;
        }
        st_alg_cfg = g_ast_alg_cfg_map[++uc_map_index];
    }

    /* 没有找到对应的命令，则报错 */
    if (st_alg_cfg.pc_name == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_intf_det_log::invalid alg_cfg command!}\r\n");
        return OAL_FAIL;
    }

    /* 记录命令对应的枚举值 */
    pst_alg_intfdet_log_param->en_alg_cfg = g_ast_alg_cfg_map[uc_map_index].en_alg_cfg;

    /* 区分获取特定帧功率和统计日志命令处理:获取功率只需获取帧名字 */
    if (pst_alg_intfdet_log_param->en_alg_cfg == MAC_ALG_CFG_INTF_DET_STAT_LOG_START) {
        /* 获取配置参数名称 */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_intf_det_log::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }

        /* 记录参数 */
        pst_alg_intfdet_log_param->us_value = (uint16_t)oal_atoi(ac_name);
        // en_stop_flag = OAL_TRUE;
    }

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ALG_PARAM, OAL_SIZEOF(mac_ioctl_alg_intfdet_log_param_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_ioctl_alg_intfdet_log_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_clear_stat_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    oam_stats_clear_stat_info();
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_user_stat_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{

    int32_t l_tmp;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;

    /* sh hipriv.sh "Hisilicon0 usr_stat_info usr_id" */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_user_stat_info::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    l_tmp = oal_atoi(ac_name);
    if ((l_tmp < 0) || (l_tmp >= WLAN_ASSOC_USER_MAX_NUM)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_user_stat_info::user id invalid [%d]!}\r\n", l_tmp);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    oam_stats_report_usr_info((uint16_t)l_tmp);
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_DFT_STAT

OAL_STATIC uint32_t wal_hipriv_clear_vap_stat_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint8_t uc_vap_id;

    if (OAL_PTR_NULL == oal_net_dev_priv(pst_net_dev)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_clear_vap_stat_info::OAL_NET_DEV_PRIV(pst_net_dev) is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    uc_vap_id = ((mac_vap_stru *)oal_net_dev_priv(pst_net_dev))->uc_vap_id;
    oam_stats_clear_vap_stat_info(uc_vap_id);

    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_FEATURE_DFR

OAL_STATIC uint32_t wal_hipriv_test_dfr_start(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_cfg_rst;
    oal_wireless_dev_stru *pst_wdev;
    mac_wiphy_priv_stru *pst_wiphy_priv = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;

    pst_wdev = oal_netdevice_wdev(pst_net_dev);
    if (pst_wdev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_test_dfr_start::pst_wdev is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_wiphy_priv = (mac_wiphy_priv_stru *)oal_wiphy_priv(pst_wdev->wiphy);
    if (pst_wiphy_priv == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_test_dfr_start::pst_wiphy_priv is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_device = pst_wiphy_priv->pst_mac_device;
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_test_dfr_start::pst_mac_device is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if ((!g_st_dfr_info.bit_device_reset_enable) || g_st_dfr_info.bit_device_reset_process_flag) {
        oam_warning_log2(0, OAM_SF_ANY, "{wal_hipriv_test_dfr_start::now DFR disabled or in DFR process, enable=%d, reset_flag=%d}",
                         g_st_dfr_info.bit_device_reset_enable, g_st_dfr_info.bit_device_reset_process_flag);
        return OAL_ERR_CODE_RESET_INPROGRESS;
    }

    g_st_dfr_info.bit_device_reset_enable = OAL_TRUE;
    g_st_dfr_info.bit_device_reset_process_flag = OAL_FALSE;
    g_st_dfr_info.ul_netdev_num = 0;
    memset_s((uint8_t *)(g_st_dfr_info.past_netdev),
             OAL_SIZEOF(g_st_dfr_info.past_netdev[0]) * WLAN_VAP_SUPPORT_MAX_NUM_LIMIT, 0,
             OAL_SIZEOF(g_st_dfr_info.past_netdev[0]) * WLAN_VAP_SUPPORT_MAX_NUM_LIMIT);

    ul_cfg_rst = wal_dfr_excp_rx(pst_mac_device->uc_device_id, 0);
    if (oal_unlikely(ul_cfg_rst != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_test_dfr_start::wal_send_cfg_event return err_code [%d]!}\r\n", ul_cfg_rst);
        return ul_cfg_rst;
    }

    return OAL_SUCC;
}

#endif  // _PRE_WLAN_FEATURE_DFR


OAL_STATIC uint32_t wal_hipriv_ar_log(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    mac_ioctl_alg_ar_log_param_stru *pst_alg_ar_log_param = OAL_PTR_NULL;
    wal_ioctl_alg_cfg_stru st_alg_cfg;
    uint8_t uc_map_index = 0;
    int32_t l_ret;
    oal_bool_enum_uint8 en_stop_flag = OAL_FALSE;

    pst_alg_ar_log_param = (mac_ioctl_alg_ar_log_param_stru *)(st_write_msg.auc_value);

    /* 获取配置参数名称 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_ar_log::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param = pc_param + ul_off_set;

    /* 寻找匹配的命令 */
    st_alg_cfg = g_ast_alg_cfg_map[0];
    while (st_alg_cfg.pc_name != OAL_PTR_NULL) {
        if (0 == oal_strcmp(st_alg_cfg.pc_name, ac_name)) {
            break;
        }
        st_alg_cfg = g_ast_alg_cfg_map[++uc_map_index];
    }

    /* 没有找到对应的命令，则报错 */
    if (st_alg_cfg.pc_name == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_ar_log::invalid alg_cfg command!}\r\n");
        return OAL_FAIL;
    }

    /* 记录命令对应的枚举值 */
    pst_alg_ar_log_param->en_alg_cfg = g_ast_alg_cfg_map[uc_map_index].en_alg_cfg;

    ul_ret = wal_hipriv_get_mac_addr(pc_param, pst_alg_ar_log_param->auc_mac_addr, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_ar_log::wal_hipriv_get_mac_addr failed!}\r\n");
        return ul_ret;
    }
    pc_param += ul_off_set;

    while ((*pc_param == ' ') || (*pc_param == '\0')) {
        if (*pc_param == '\0') {
            en_stop_flag = OAL_TRUE;
            break;
        }
        ++pc_param;
    }

    /* 获取业务类型值 */
    if (en_stop_flag != OAL_TRUE) {
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_ar_log::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }

        pst_alg_ar_log_param->uc_ac_no = (uint8_t)oal_atoi(ac_name);
        pc_param = pc_param + ul_off_set;

        en_stop_flag = OAL_FALSE;
        while ((*pc_param == ' ') || (*pc_param == '\0')) {
            if (*pc_param == '\0') {
                en_stop_flag = OAL_TRUE;
                break;
            }
            ++pc_param;
        }

        if (en_stop_flag != OAL_TRUE) {
            /* 获取参数配置值 */
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_ar_log::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            /* 记录参数配置值 */
            pst_alg_ar_log_param->us_value = (uint16_t)oal_atoi(ac_name);
        }
    }

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ALG_PARAM, OAL_SIZEOF(mac_ioctl_alg_ar_log_param_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_ioctl_alg_ar_log_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_txbf_log(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    mac_ioctl_alg_txbf_log_param_stru *pst_alg_txbf_log_param = OAL_PTR_NULL;
    wal_ioctl_alg_cfg_stru st_alg_cfg;
    uint8_t uc_map_index = 0;
    int32_t l_ret;
    oal_bool_enum_uint8 en_stop_flag = OAL_FALSE;
    pst_alg_txbf_log_param = (mac_ioctl_alg_txbf_log_param_stru *)(st_write_msg.auc_value);
    /* 获取配置参数名称 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_txbf_log::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param = pc_param + ul_off_set;
    /* 寻找匹配的命令 */
    st_alg_cfg = g_ast_alg_cfg_map[0];
    while (st_alg_cfg.pc_name != OAL_PTR_NULL) {
        if (0 == oal_strcmp(st_alg_cfg.pc_name, ac_name)) {
            break;
        }
        st_alg_cfg = g_ast_alg_cfg_map[++uc_map_index];
    }
    /* 没有找到对应的命令，则报错 */
    if (st_alg_cfg.pc_name == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_ar_log::invalid alg_cfg command!}\r\n");
        return OAL_FAIL;
    }
    /* 记录命令对应的枚举值 */
    pst_alg_txbf_log_param->en_alg_cfg = g_ast_alg_cfg_map[uc_map_index].en_alg_cfg;
    ul_ret = wal_hipriv_get_mac_addr(pc_param, pst_alg_txbf_log_param->auc_mac_addr, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_txbf_log::wal_hipriv_get_mac_addr failed!}\r\n");
        return ul_ret;
    }
    pc_param += ul_off_set;
    while (*pc_param == ' ') {
        ++pc_param;
        if (*pc_param == '\0') {
            en_stop_flag = OAL_TRUE;
            break;
        }
    }
    /* 获取参数配置值 */
    if (en_stop_flag != OAL_TRUE) {
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_txbf_log::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }

        pst_alg_txbf_log_param->uc_ac_no = (uint8_t)oal_atoi(ac_name);
        pc_param = pc_param + ul_off_set;

        en_stop_flag = OAL_FALSE;
        while ((*pc_param == ' ') || (*pc_param == '\0')) {
            if (*pc_param == '\0') {
                en_stop_flag = OAL_TRUE;
                break;
            }
            ++pc_param;
        }

        if (en_stop_flag != OAL_TRUE) {
            /* 获取参数配置值 */
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_txbf_log::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
                return ul_ret;
            }

            /* 记录参数配置值 */
            pst_alg_txbf_log_param->us_value = (uint16_t)oal_atoi(ac_name);
        }
    }
    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ALG_PARAM, OAL_SIZEOF(mac_ioctl_alg_txbf_log_param_stru));
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_ioctl_alg_txbf_log_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        return (uint32_t)l_ret;
    }
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_ar_test(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_offset;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    mac_ioctl_alg_ar_test_param_stru *pst_alg_ar_test_param = OAL_PTR_NULL;
    wal_ioctl_alg_cfg_stru st_alg_cfg;
    uint8_t uc_map_index = 0;
    int32_t l_ret;

    pst_alg_ar_test_param = (mac_ioctl_alg_ar_test_param_stru *)(st_write_msg.auc_value);

    /* 获取配置参数名称 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_offset);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_ar_test::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param = pc_param + ul_offset;

    /* 寻找匹配的命令 */
    st_alg_cfg = g_ast_alg_cfg_map[0];
    while (st_alg_cfg.pc_name != OAL_PTR_NULL) {
        if (0 == oal_strcmp(st_alg_cfg.pc_name, ac_name)) {
            break;
        }
        st_alg_cfg = g_ast_alg_cfg_map[++uc_map_index];
    }

    /* 没有找到对应的命令，则报错 */
    if (st_alg_cfg.pc_name == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_ar_test::invalid alg_cfg command!}\r\n");
        return OAL_FAIL;
    }

    /* 记录命令对应的枚举值 */
    pst_alg_ar_test_param->en_alg_cfg = g_ast_alg_cfg_map[uc_map_index].en_alg_cfg;

    ul_ret = wal_hipriv_get_mac_addr(pc_param, pst_alg_ar_test_param->auc_mac_addr, &ul_offset);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_ar_test::wal_hipriv_get_mac_addr failed!}\r\n");
        return ul_ret;
    }
    pc_param += ul_offset;

    /* 获取参数配置值 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_offset);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_ar_test::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    /* 记录参数配置值 */
    pst_alg_ar_test_param->us_value = (uint16_t)oal_atoi(ac_name);

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ALG_PARAM, OAL_SIZEOF(mac_ioctl_alg_ar_test_param_stru));
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_ioctl_alg_ar_test_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_default_key(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    mac_setdefaultkey_param_stru st_payload_params = { 0 };
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;
    wal_msg_write_stru st_write_msg;

    /* 1.1 入参检查 */
    if (oal_any_null_ptr2(pst_net_dev, pc_param)) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_hipriv_set_default_key::Param Check ERROR,pst_netdev, pst_params %x, %x!}\r\n", (uintptr_t)pst_net_dev, (uintptr_t)pc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获取key_index */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_test_add_key::wal_get_cmd_one_arg fail.err code[%u]}", ul_ret);
        return ul_ret;
    }
    st_payload_params.uc_key_index = (uint8_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /* 获取en_unicast */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_test_add_key::wal_get_cmd_one_arg fail.err code[%u]}", ul_ret);
        return ul_ret;
    }
    st_payload_params.en_unicast = (uint8_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /* 获取multicast */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_test_add_key::wal_get_cmd_one_arg fail.err code[%u]}", ul_ret);
        return ul_ret;
    }
    st_payload_params.en_multicast = (uint8_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /***************************************************************************
                              抛事件到wal层处理
    ***************************************************************************/
    /* 3.2 填写 msg 消息体 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                        &st_payload_params, OAL_SIZEOF(mac_setdefaultkey_param_stru))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_test_add_key::memcpy fail!}");
        return OAL_FAIL;
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DEFAULT_KEY, OAL_SIZEOF(mac_setdefaultkey_param_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_setdefaultkey_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_default_key::wal_send_cfg_event return err_code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_test_add_key(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    mac_addkey_param_stru st_payload_params;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    int32_t l_sec_ret;
    uint32_t ul_ret;
    uint32_t ul_off_set;
    uint8_t auc_key[OAL_WPA_KEY_LEN] = { 0 };
    int8_t *pc_key = OAL_PTR_NULL;
    uint32_t ul_char_index;
    uint16_t us_len;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

    /* 1.1 入参检查 */
    if (oal_any_null_ptr2(pst_net_dev, pc_param)) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_hipriv_test_add_key::Param Check ERROR,pst_netdev, pst_params %x, %x!}\r\n", (uintptr_t)pst_net_dev, (uintptr_t)pc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }
    /*
     * xxx(cipher) xx(en_pairwise) xx(key_len) xxx(key_index) xxxx:xx:xx:xx:xx:xx...
     *  (key 小于32字节) xx:xx:xx:xx:xx:xx(目的地址)
     */
    memset_s(&st_payload_params, OAL_SIZEOF(st_payload_params), 0, OAL_SIZEOF(st_payload_params));
    memset_s(&st_payload_params.st_key, OAL_SIZEOF(mac_key_params_stru), 0, OAL_SIZEOF(mac_key_params_stru));
    st_payload_params.st_key.seq_len = 6;
    memset_s(st_payload_params.auc_mac_addr, WLAN_MAC_ADDR_LEN, 0, WLAN_MAC_ADDR_LEN);

    /* 获取cipher */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_test_add_key::wal_get_cmd_one_arg fail.err code[%u]}", ul_ret);
        return ul_ret;
    }
    st_payload_params.st_key.cipher = (uint32_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /* 获取en_pairwise */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_test_add_key::wal_get_cmd_one_arg fail.err code[%u]}", ul_ret);
        return ul_ret;
    }
    st_payload_params.en_pairwise = (uint8_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /* 获取key_len */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_test_add_key::wal_get_cmd_one_arg fail.err code[%u]}", ul_ret);
        return ul_ret;
    }

    pc_param = pc_param + ul_off_set;
    st_payload_params.st_key.key_len = (uint8_t)oal_atoi(ac_name);
    if (oal_value_not_in_valid_range(st_payload_params.st_key.key_len, 0, OAL_WPA_KEY_LEN)) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_test_add_key::Param Check ERROR! key_len[%x]  }\r\n",
                       (int32_t)st_payload_params.st_key.key_len);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* 获取key_index */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_test_add_key::wal_get_cmd_one_arg fail.err code[%u]}", ul_ret);
        return ul_ret;
    }
    st_payload_params.uc_key_index = (uint8_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /* 获取key */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_test_add_key::wal_get_cmd_one_arg fail.err code[%u]}", ul_ret);
        return ul_ret;
    }
    pc_param = pc_param + ul_off_set;
    pc_key = ac_name;
    /* 16进制转换 */
    for (ul_char_index = 0; ul_char_index < ul_off_set; ul_char_index++) {
        if (*pc_key == '-') {
            pc_key++;
            if (ul_char_index != 0) {
                ul_char_index--;
            }

            continue;
        }

        auc_key[ul_char_index / 2] = (uint8_t)(auc_key[ul_char_index / 2] * 16 * (ul_char_index % 2) + oal_strtohex(pc_key));
        pc_key++;
    }

    l_sec_ret = memcpy_s(st_payload_params.st_key.auc_key, sizeof(st_payload_params.st_key.auc_key),
                         auc_key, (uint32_t)st_payload_params.st_key.key_len);

    /* 获取目的地址 */
    memset_s(ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, 0, WAL_HIPRIV_CMD_NAME_MAX_LEN);
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_test_add_key::wal_get_cmd_one_arg fail.err code[%u]}", ul_ret);
        return ul_ret;
    }
    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), st_payload_params.auc_mac_addr, WLAN_MAC_ADDR_LEN);

    oam_info_log3(0, OAM_SF_ANY, "{wal_hipriv_test_add_key::key_len:%d, seq_len:%d, cipher:0x%08x!}\r\n",
                  st_payload_params.st_key.key_len, st_payload_params.st_key.seq_len, st_payload_params.st_key.cipher);

    /***************************************************************************
                              抛事件到wal层处理
    ***************************************************************************/
    /* 3.2 填写 msg 消息体 */
    us_len = (uint32_t)OAL_SIZEOF(mac_addkey_param_stru);

    l_sec_ret += memcpy_s((int8_t *)st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                          (int8_t *)&st_payload_params, (uint32_t)us_len);

    if (l_sec_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_test_add_key::memcpy fail!}");
        return OAL_FAIL;
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADD_KEY, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_enable_pmf::wal_send_cfg_event return err_code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    if (OAL_SUCC != wal_check_and_release_msg_resp(pst_rsp_msg)) {
        oam_warning_log0(0, OAM_SF_ANY, "wal_hipriv_test_add_key::wal_check_and_release_msg_resp fail");
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_str_cmd(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;
    wal_ioctl_str_stru st_cmd_table;
    wal_ioctl_tlv_stru *pst_sub_cmd_table = OAL_PTR_NULL;
    mac_cfg_set_str_stru *pst_set_cmd = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    uint8_t uc_len;
    uint8_t uc_cmd_cnt;
    uint16_t us_cfg_id;
    uint8_t uc_map_index = 0;
    uint8_t uc_value_base;
    uint8_t *pc_value = OAL_PTR_NULL;

    /* 获取配置参数名称 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_str_cmd::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    /* 寻找匹配的命令 */
    st_cmd_table = g_ast_set_str_table[0];
    while (st_cmd_table.pc_name != OAL_PTR_NULL) {
        if (0 == oal_strcmp(st_cmd_table.pc_name, ac_name)) {
            break;
        }
        st_cmd_table = g_ast_set_str_table[++uc_map_index];
    }

    /* 没有找到对应的命令，则报错 */
    if (st_cmd_table.pc_name == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_str_cmd::invalid alg_cfg command!}\r\n");
        return OAL_FAIL;
    }

    /* 记录主命令对应的枚举值 */
    us_cfg_id = g_ast_set_str_table[uc_map_index].en_tlv_cfg_id;
    pst_sub_cmd_table = g_ast_set_str_table[uc_map_index].pst_cfg_table;

    /* 获取设定mib名称 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_str_cmd::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    /* 当前命令的参数个数 */
    uc_cmd_cnt = (uint8_t)oal_atoi(ac_name);
    if (uc_cmd_cnt > MAC_STR_CMD_MAX_SIZE) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_set_str_cmd::too many para [%d]!}\r\n", uc_cmd_cnt);
        return OAL_FAIL;
    }

    uc_len = OAL_SIZEOF(mac_cfg_set_str_stru);
    pst_set_cmd = (mac_cfg_set_str_stru *)(st_write_msg.auc_value);
    pst_set_cmd->us_cfg_id = us_cfg_id;
    pst_set_cmd->uc_cmd_cnt = uc_cmd_cnt;
    pst_set_cmd->uc_len = uc_len;
    oam_warning_log3(0, OAM_SF_ANY, "{wal_hipriv_set_str_cmd:: cfg id[%d] cfg len[%d] cmd cnt[%d]!}",
                     us_cfg_id, uc_len, uc_cmd_cnt);

    /* 遍历获取每个字符参数 */
    for (uc_map_index = 0; uc_map_index < uc_cmd_cnt; uc_map_index++) {
        /* 获取子命令 */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_str_cmd::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }
        pc_param += ul_off_set;

        /* 子命令表查找对应的子命令 */
        while (pst_sub_cmd_table->pc_name != OAL_PTR_NULL) {
            if (0 == oal_strcmp(pst_sub_cmd_table->pc_name, ac_name)) {
                break;
            }
            pst_sub_cmd_table++;
        }

        /* 没有找到对应的命令，则报错 */
        if (pst_sub_cmd_table->pc_name == OAL_PTR_NULL) {
            oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_str_cmd::invalid alg_cfg command!}\r\n");
            return OAL_FAIL;
        }

        /* 获取设定置 */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_str_cmd::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
            return ul_ret;
        }
        pc_value = ac_name;
        uc_value_base = 10;
        if (basic_value_is_hex(ac_name)) {
            uc_value_base = 16;
            pc_value = ac_name + 2;
        }
        pc_param += ul_off_set;

        /* 记录每个子命令的id号和具体的命令值 */
        pst_set_cmd->us_set_id[uc_map_index] = pst_sub_cmd_table->en_tlv_cfg_id;
        pst_set_cmd->ul_value[uc_map_index] = (uint32_t)oal_strtol(pc_value, OAL_PTR_NULL, uc_value_base);
        oam_warning_log4(0, OAM_SF_ANY, "{wal_hipriv_set_str_cmd:: index[%d] sub cfg id[%d] ,uc_value_base=[%s] ,value[%d]!}",
                         uc_map_index, pst_set_cmd->us_set_id[uc_map_index], uc_value_base, pst_set_cmd->ul_value[uc_map_index]);
    }

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, us_cfg_id, uc_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + uc_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_tlv_cmd::wal_send_cfg_event return err_code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


uint32_t wal_hipriv_set_tlv_cmd(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;
    wal_ioctl_tlv_stru st_tlv_cfg;
    mac_cfg_set_tlv_stru *pst_set_tlv = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    uint8_t uc_len;
    uint8_t uc_cmd_type;
    uint16_t us_cfg_id;
    uint32_t ul_value;
    uint8_t uc_map_index = 0;

    /* 获取配置参数名称 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_tlv_cmd::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    /* 寻找匹配的命令 */
    st_tlv_cfg = g_ast_set_tlv_table[0];
    while (st_tlv_cfg.pc_name != OAL_PTR_NULL) {
        if (0 == oal_strcmp(st_tlv_cfg.pc_name, ac_name)) {
            break;
        }
        st_tlv_cfg = g_ast_set_tlv_table[++uc_map_index];
    }

    /* 没有找到对应的命令，则报错 */
    if (st_tlv_cfg.pc_name == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_tlv_cmd::invalid alg_cfg command!}\r\n");
        return OAL_FAIL;
    }

    /* 记录命令对应的枚举值 */
    us_cfg_id = g_ast_set_tlv_table[uc_map_index].en_tlv_cfg_id;

    /* 获取设定mib名称 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_tlv_cmd::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    pc_param += ul_off_set;
    uc_cmd_type = (uint8_t)oal_atoi(ac_name);

    /* 获取设定置 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_tlv_cmd::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    ul_value = (uint32_t)oal_atoi(ac_name);

    uc_len = OAL_SIZEOF(mac_cfg_set_tlv_stru);

    pst_set_tlv = (mac_cfg_set_tlv_stru *)(st_write_msg.auc_value);
    pst_set_tlv->uc_cmd_type = uc_cmd_type;
    pst_set_tlv->us_cfg_id = us_cfg_id;
    pst_set_tlv->uc_len = uc_len;
    pst_set_tlv->ul_value = ul_value;

    oam_warning_log4(0, OAM_SF_ANY, "{wal_hipriv_set_tlv_cmd:: cfg id[%d] cfg len[%d] cmd type[%d], set val[%d]!}",
                     us_cfg_id, uc_len, uc_cmd_type, ul_value);
    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, us_cfg_id, uc_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + uc_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_tlv_cmd::wal_send_cfg_event return err_code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


uint32_t wal_hipriv_set_val_cmd(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;
    wal_ioctl_tlv_stru st_tlv_cfg;
    mac_cfg_set_tlv_stru *pst_set_tlv = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    uint8_t uc_len;
    uint16_t us_cfg_id;
    uint32_t ul_value;
    uint8_t uc_map_index = 0;

    /* 获取配置参数名称 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_val_cmd::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    /* 寻找匹配的命令 */
    st_tlv_cfg = g_ast_set_tlv_table[0];
    while (st_tlv_cfg.pc_name != OAL_PTR_NULL) {
        if (0 == oal_strcmp(st_tlv_cfg.pc_name, ac_name)) {
            break;
        }
        st_tlv_cfg = g_ast_set_tlv_table[++uc_map_index];
    }

    /* 没有找到对应的命令，则报错 */
    if (st_tlv_cfg.pc_name == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_val_cmd::invalid alg_cfg command!}\r\n");
        return OAL_FAIL;
    }

    /* 记录命令对应的枚举值 */
    us_cfg_id = g_ast_set_tlv_table[uc_map_index].en_tlv_cfg_id;

    /* 获取设定值 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_val_cmd::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    pc_param += ul_off_set;
    ul_value = (uint32_t)oal_atoi(ac_name);

    uc_len = OAL_SIZEOF(mac_cfg_set_tlv_stru);

    pst_set_tlv = (mac_cfg_set_tlv_stru *)(st_write_msg.auc_value);
    pst_set_tlv->uc_cmd_type = 0xFF;
    pst_set_tlv->us_cfg_id = us_cfg_id;
    pst_set_tlv->uc_len = uc_len;
    pst_set_tlv->ul_value = ul_value;

    oam_warning_log3(0, OAM_SF_ANY, "{wal_hipriv_set_val_cmd:: cfg id[%d], cfg len[%d],set_val[%d]!}", us_cfg_id, uc_len, ul_value);
    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, us_cfg_id, uc_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + uc_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_val_cmd::wal_send_cfg_event return err_code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_auto_protection(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_ret;
    int32_t l_cfg_rst;
    uint16_t us_len;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_off_set = 0;
    uint32_t ul_auto_protection_flag;

    /* 获取mib名称 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_auto_protection::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    ul_auto_protection_flag = (uint32_t)oal_atoi(ac_name);

    us_len = OAL_SIZEOF(uint32_t);
    *(uint32_t *)(st_write_msg.auc_value) = ul_auto_protection_flag;
    /***************************************************************************
                              抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_AUTO_PROTECTION, us_len);

    l_cfg_rst = wal_send_cfg_event(pst_net_dev,
                                       WAL_MSG_TYPE_WRITE,
                                       WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                       (uint8_t *)&st_write_msg,
                                       OAL_FALSE,
                                       OAL_PTR_NULL);

    if (oal_unlikely(l_cfg_rst != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_auto_protection::wal_send_cfg_event return err_code [%d]!}\r\n", l_cfg_rst);
        return (uint32_t)l_cfg_rst;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_WAPI

#ifdef _PRE_WAPI_DEBUG
OAL_STATIC uint32_t wal_hipriv_show_wapi_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    mac_vap_stru *pst_mac_vap;
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    mac_cfg_user_info_param_stru *pst_user_info_param;
    uint8_t auc_mac_addr[6] = { 0 }; /* 临时保存获取的use的mac地址信息 */
    uint8_t uc_char_index;
    uint16_t us_user_idx;
    /* 去除字符串的空格 */
    pc_param++;

    /* 获取mac地址,16进制转换 */
    for (uc_char_index = 0; uc_char_index < 12; uc_char_index++) {
        if (*pc_param == ':') {
            pc_param++;
            if (uc_char_index != 0) {
                uc_char_index--;
            }

            continue;
        }

        auc_mac_addr[uc_char_index / 2] =
            (uint8_t)(auc_mac_addr[uc_char_index / 2] * 16 * (uc_char_index % 2) +
                        oal_strtohex(pc_param));
        pc_param++;
    }

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_WAPI_INFO, OAL_SIZEOF(mac_cfg_user_info_param_stru));

    /* 根据mac地址找用户 */
    pst_mac_vap = oal_net_dev_priv(pst_net_dev);

    l_ret = (int32_t)mac_vap_find_user_by_macaddr(pst_mac_vap, auc_mac_addr, &us_user_idx);
    if (l_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_user_info::no such user!}\r\n");
        return OAL_FAIL;
    }

    /* 设置配置命令参数 */
    pst_user_info_param = (mac_cfg_user_info_param_stru *)(st_write_msg.auc_value);
    pst_user_info_param->us_user_idx = us_user_idx;

    oam_warning_log1(0, OAM_SF_ANY, "wal_hipriv_show_wapi_info::us_user_idx %u", us_user_idx);
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_user_info_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_user_info::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif /* #ifdef WAPI_DEBUG_MODE */

#endif /* #ifdef _PRE_WLAN_FEATURE_WAPI */

#ifdef _PRE_WLAN_FEATURE_FTM
OAL_STATIC wal_ftm_cmd_entry_stru g_ast_ftm_common_cmd[] = {
    { "enable_ftm_initiator", OAL_TRUE, BIT0 },
    { "enable",               OAL_TRUE, BIT2 },
    { "cali",                 OAL_TRUE, BIT3 },
    { "enable_ftm_responder", OAL_TRUE, BIT5 },
    { "enable_ftm_range",     OAL_TRUE, BIT8 },
    { "en_multipath",         OAL_TRUE, BIT12 },
};

OAL_STATIC OAL_INLINE void ftm_debug_cmd_format_info(void)
{
    oam_warning_log0(0, OAM_SF_FTM,
        "{CMD format::hipriv.sh wlan0 ftm_debug \
        enable_ftm_initiator[0|1] \
        enable[0|1] \
        cali[0|1] \
        enable_ftm_responder[0|1] \
        set_ftm_time t1[] t2[] t3[] t4[] \
        set_ftm_timeout ul_bursts_timeout ul_ftms_timeout \
        enable_ftm_responder[0|1] \
        enable_ftm_range[0|1] \
        get_cali \
        !!}\r\n");
    oam_warning_log0(0, OAM_SF_FTM,
        "{CMD format::hipriv.sh wlan0 ftm_debug \
        send_initial_ftm_request channel[] ftms_per_burst[n] burst_num[n] en_lci_civic_request[0|1] asap[0|1] \
        bssid[xx:xx:xx:xx:xx:xx] format_bw[9~13] send_ftm bssid[xx:xx:xx:xx:xx:xx] \
        send_range_req mac[] num_rpt[0-65535] delay[] ap_cnt[] bssid[] channel[] bssid[] channel[] ...\
        set_location type[] mac[] mac[] mac[] \
        white_list index mac[xx:xx:xx:xx:xx:xx] \
        !!}\r\n");
}


OAL_STATIC uint32_t ftm_debug_parase_iftmr_cmd(int8_t *pc_param, mac_ftm_debug_switch_stru *pst_debug_info, uint32_t *pul_offset)
{
    uint32_t ul_ret;
    int8_t ac_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN] = { 0 };
    uint32_t ul_off_set = 0;
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN] = { 0, 0, 0, 0, 0, 0 };

    *pul_offset = 0;

    /* 命令格式:send_initial_ftm_request channel[] ftms_per_burst[n] burst_num[n]---代表(2^n)个burst
                                    en_lci_civic_request[0|1] asap[0|1] bssid[xx:xx:xx:xx:xx:xx]
                                    format_bw[9~13] */
    /* 解析channel */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_iftmr_cmd::get iftmr mode error, return.}");
        return ul_ret;
    }

    *pul_offset += ul_off_set;
    pst_debug_info->st_send_iftmr_bit1.uc_channel_num = (uint8_t)oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    /* 解析ftms_per_burst */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_iftmr_cmd::get uc_ftms_per_burst error, return.}");
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    pst_debug_info->st_send_iftmr_bit1.uc_ftms_per_burst = (uint8_t)oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    /* 解析burst */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_iftmr_cmd::get burst error, return.}");
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    pst_debug_info->st_send_iftmr_bit1.uc_burst_num = (uint8_t)oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    /* 解析LCI and Location Civic Measurement request使能标志 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_iftmr_cmd::get en_lci_civic_request error, return.}");
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    pst_debug_info->st_send_iftmr_bit1.en_lci_civic_request = (uint8_t)oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    /* 解析asap,as soon as possible使能标志 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_iftmr_cmd::get asap error, return.}");
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    pst_debug_info->st_send_iftmr_bit1.en_is_asap_on = (uint8_t)oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    /* 获取BSSID */
    ul_ret = wal_hipriv_get_mac_addr(pc_param, auc_mac_addr, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_iftmr_cmd::No bssid, set the associated bssid.}");
        *pul_offset += ul_off_set;
        memset_s(pst_debug_info->st_send_iftmr_bit1.auc_bssid, OAL_SIZEOF(pst_debug_info->st_send_iftmr_bit1.auc_bssid),
                 0, OAL_SIZEOF(pst_debug_info->st_send_iftmr_bit1.auc_bssid));
        pc_param += ul_off_set;
        ul_off_set = 0;

        return OAL_SUCC;
    }
    *pul_offset += ul_off_set;
    oal_set_mac_addr(pst_debug_info->st_send_iftmr_bit1.auc_bssid, auc_mac_addr);
    pc_param += ul_off_set;
    ul_off_set = 0;

    oam_warning_log3(0, OAM_SF_ANY, "{ftm_debug_parase_iftmr_cmd::bssid=*:*:*:%x:%x:%x}",
                     auc_mac_addr[3],
                     auc_mac_addr[4],
                     auc_mac_addr[5]);

    /* 解析format bw */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_iftmr_cmd::get format bw error, return.}");
        return OAL_SUCC;
    }
    *pul_offset += ul_off_set;
    pst_debug_info->st_send_iftmr_bit1.uc_format_bw = (uint8_t)oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    return OAL_SUCC;
}


OAL_STATIC uint32_t ftm_debug_parase_ftm_cmd(int8_t *pc_param, mac_ftm_debug_switch_stru *pst_debug_info, uint32_t *pul_offset)
{
    uint32_t ul_ret;
    int8_t ac_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN];
    uint32_t ul_off_set = 0;

    *pul_offset = 0;
    /* 解析mac */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_ftm_cmd::ger mac error.}");
        return OAL_FAIL;
    }
    *pul_offset += ul_off_set;
    oal_strtoaddr(ac_value, OAL_SIZEOF(ac_value), pst_debug_info->st_send_ftm_bit4.auc_mac, WLAN_MAC_ADDR_LEN);
    pc_param += ul_off_set;
    ul_off_set = 0;

    return OAL_SUCC;
}


OAL_STATIC uint32_t ftm_debug_parase_ftm_timeout_cmd(int8_t *pc_param, mac_ftm_debug_switch_stru *pst_debug_info, uint32_t *pul_offset)
{
    uint32_t ul_ret;
    int8_t ac_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN];
    uint32_t ul_off_set = 0;

    *pul_offset = 0;
    /* 解析ftm bursts timeout */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_ftm_time_cmd::get correct time1 error,return.}");
        return ul_ret;
    }

    *pul_offset += ul_off_set;
    pst_debug_info->st_ftm_timeout_bit14.initiator_tsf_fix_offset = oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    /* 解析ftms timeout */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_ftm_time_cmd::get correct time2 error,return.}");
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    pst_debug_info->st_ftm_timeout_bit14.responder_ptsf_offset = oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    return OAL_SUCC;
}


OAL_STATIC uint32_t ftm_debug_set_cali(int8_t *pc_param, mac_ftm_debug_switch_stru *pst_debug_info, uint32_t *pul_offset)
{
    uint32_t ul_ret;
    int8_t ac_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN] = { 0 };
    uint32_t ul_off_set = 0;

    *pul_offset = 0;

    /* 解析t1 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_ftm_time_cmd::get correct time1 error, return.}");
        return ul_ret;
    }

    *pul_offset += ul_off_set;
    pst_debug_info->ul_ftm_cali = (uint32_t)oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    return OAL_SUCC;
}


OAL_STATIC uint32_t ftm_debug_parase_range_req_cmd(int8_t *pc_param, mac_ftm_debug_switch_stru *pst_debug_info, uint32_t *pul_offset)
{
    uint32_t ul_ret;
    int8_t ac_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN];
    uint32_t ul_off_set = 0;
    uint8_t uc_ap_cnt;

    *pul_offset = 0;

    /* 解析mac */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_range_req_cmd::mac error!.}");
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    oal_strtoaddr(ac_value, OAL_SIZEOF(ac_value), pst_debug_info->st_send_range_req_bit7.auc_mac, WLAN_MAC_ADDR_LEN);
    pc_param += ul_off_set;
    ul_off_set = 0;

    /* 解析num_rpt */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_range_req_cmd::get num_rpt error,return.}");
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    pst_debug_info->st_send_range_req_bit7.us_num_rpt = (uint16_t)oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    /* 解析ap_cnt */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_range_req_cmd::get ap_cnt error,return.}");
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    pst_debug_info->st_send_range_req_bit7.uc_minimum_ap_count = (uint8_t)oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    for (uc_ap_cnt = 0; uc_ap_cnt < pst_debug_info->st_send_range_req_bit7.uc_minimum_ap_count; uc_ap_cnt++) {
        /* 解析bssid */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_range_req_cmd::bssid error!.}");
            return ul_ret;
        }
        *pul_offset += ul_off_set;
        oal_strtoaddr(ac_value, OAL_SIZEOF(ac_value), pst_debug_info->st_send_range_req_bit7.aauc_bssid[uc_ap_cnt], WLAN_MAC_ADDR_LEN);
        pc_param += ul_off_set;
        ul_off_set = 0;

        /* 解析channel */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_range_req_cmd::get channel error,return.}");
            return ul_ret;
        }
        *pul_offset += ul_off_set;
        pst_debug_info->st_send_range_req_bit7.auc_channel[uc_ap_cnt] = (uint8_t)oal_atoi(ac_value);
        pc_param += ul_off_set;
        ul_off_set = 0;
    }
    return OAL_SUCC;
}


OAL_STATIC uint32_t ftm_debug_parase_m2s_cmd(int8_t *pc_param, mac_ftm_debug_switch_stru *pst_debug_info, uint32_t *pul_offset)
{
    uint32_t ul_ret;
    int8_t ac_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN];
    uint32_t ul_off_set = 0;
    ftm_m2s_stru *pst_m2s = &(pst_debug_info->st_m2s_bit11);

    *pul_offset = 0;

    /* 解析m2s */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_m2s_cmd::get m2s error,return.}");
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    pst_m2s->en_is_mimo = (uint8_t)oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    /* 解析tx_chain */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_m2s_cmd::get tx_chain error,return.}");
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    pst_m2s->uc_tx_chain_selection = (uint8_t)oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    return OAL_SUCC;
}


OAL_STATIC uint32_t ftm_debug_parase_neighbor_report_req_cmd(int8_t *pc_param, mac_ftm_debug_switch_stru *pst_debug_info, uint32_t *pul_offset)
{
    uint32_t ul_ret;
    uint32_t ul_off_set = 0;
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN] = { 0, 0, 0, 0, 0, 0 };

    *pul_offset = 0;

    /* 解析bssid */
    ul_ret = wal_hipriv_get_mac_addr(pc_param, auc_mac_addr, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM,
            "{ftm_debug_parase_neighbor_report_req_cmd::No bssid,set the associated bssid.}");
        *pul_offset += ul_off_set;
        memset_s(pst_debug_info->st_neighbor_report_req_bit15.auc_bssid, OAL_SIZEOF(pst_debug_info->st_neighbor_report_req_bit15.auc_bssid),
                 0, OAL_SIZEOF(pst_debug_info->st_neighbor_report_req_bit15.auc_bssid));
        pc_param += ul_off_set;
        ul_off_set = 0;
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    oal_set_mac_addr(pst_debug_info->st_neighbor_report_req_bit15.auc_bssid, auc_mac_addr);
    pc_param += ul_off_set;
    ul_off_set = 0;

    oam_warning_log3(0, OAM_SF_ANY,
        "{ftm_debug_parase_neighbor_report_req_cmd::send neighbor request frame to ap[*:*:*:%x:%x:%x]}",
        auc_mac_addr[3], auc_mac_addr[4], auc_mac_addr[5]);

    return OAL_SUCC;
}


OAL_STATIC uint32_t ftm_debug_parase_gas_init_req_cmd(int8_t *pc_param,
    mac_ftm_debug_switch_stru *pst_debug_info, uint32_t *pul_offset)
{
    uint32_t ul_ret;
    int8_t ac_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN];
    uint32_t ul_off_set = 0;
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN] = { 0, 0, 0, 0, 0, 0 };

    /* 命令格式:send_gas_init_req lci_enable[0|1] bssid[xx:xx:xx:xx:xx:xx] */

    *pul_offset = 0;

    /* 解析lci_enable */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM,
            "{ftm_debug_parase_gas_init_req_cmd::get lci_enable error, use default value [true].}");
        ac_value[0] = OAL_TRUE;
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    pst_debug_info->st_gas_init_req_bit16.en_lci_enable = (uint8_t)oal_atoi(ac_value);
    pc_param += ul_off_set;
    ul_off_set = 0;

    /* 解析bssid */
    ul_ret = wal_hipriv_get_mac_addr(pc_param, auc_mac_addr, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_FTM, "{ftm_debug_parase_gas_init_req_cmd::No bssid, set the associated bssid.}");
        *pul_offset += ul_off_set;
        memset_s(pst_debug_info->st_gas_init_req_bit16.auc_bssid, OAL_SIZEOF(pst_debug_info->st_gas_init_req_bit16.auc_bssid),
                 0, OAL_SIZEOF(pst_debug_info->st_gas_init_req_bit16.auc_bssid));
        pc_param += ul_off_set;
        ul_off_set = 0;
        return ul_ret;
    }
    *pul_offset += ul_off_set;
    oal_set_mac_addr(pst_debug_info->st_gas_init_req_bit16.auc_bssid, auc_mac_addr);
    pc_param += ul_off_set;
    ul_off_set = 0;

    oam_warning_log3(0, OAM_SF_ANY,
        "{ftm_debug_parase_gas_init_req_cmd::send gas initial request frame to ap[*:*:*:%x:%x:%x]}",
        auc_mac_addr[3], auc_mac_addr[4], auc_mac_addr[5]);
    return OAL_SUCC;
}


OAL_STATIC uint8_t wal_hipriv_ftm_is_common_cmd(int8_t ac_name[], uint8_t *puc_cmd_index)
{
    uint8_t uc_cmd_index;

    for (uc_cmd_index = 0; uc_cmd_index < OAL_SIZEOF(g_ast_ftm_common_cmd) / OAL_SIZEOF(wal_ftm_cmd_entry_stru); uc_cmd_index++) {
        if (0 == oal_strcmp(g_ast_ftm_common_cmd[uc_cmd_index].pc_cmd_name, ac_name)) {
            break;
        }
    }

    *puc_cmd_index = uc_cmd_index;
    if (uc_cmd_index == OAL_SIZEOF(g_ast_ftm_common_cmd) / OAL_SIZEOF(wal_ftm_cmd_entry_stru)) {
        return OAL_FALSE;
    } else {
        return OAL_TRUE;
    }
}


OAL_STATIC uint32_t wal_hipriv_ftm_set_common_cmd(int8_t *pc_param,
    uint8_t uc_cmd_index, mac_ftm_debug_switch_stru *pst_ftm_debug, uint32_t *pul_offset)
{
    uint32_t ul_ret;
    uint32_t ul_off_set = 0;
    int8_t ac_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN];
    *pul_offset = 0;

    oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_ftm_set_common_cmd:: ftm common cmd [%d].}", uc_cmd_index);

    /* 取命令配置值 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        ftm_debug_cmd_format_info();
        return ul_ret;
    }

    /* 检查命令的配置值 */
    if ((g_ast_ftm_common_cmd[uc_cmd_index].uc_is_check_para) &&
        oal_value_not_in_valid_range(ac_value[0], '0', '9')) {
        ftm_debug_cmd_format_info();
        return OAL_FAIL;
    }

    *pul_offset += ul_off_set;
    ul_off_set = 0;

    /* 填写命令结构体 */
    switch (g_ast_ftm_common_cmd[uc_cmd_index].ul_bit) {
        case BIT2:
            pst_ftm_debug->en_enable_bit2 = ((uint8_t)oal_atoi(ac_value));
            pst_ftm_debug->ul_cmd_bit_map |= BIT2;
            break;
        case BIT3:
            pst_ftm_debug->en_cali_bit3 = ((uint8_t)oal_atoi(ac_value));
            pst_ftm_debug->ul_cmd_bit_map |= BIT3;
            break;
        case BIT12:
            pst_ftm_debug->en_multipath_bit12 = ((uint8_t)oal_atoi(ac_value));
            pst_ftm_debug->ul_cmd_bit_map |= BIT12;
            break;
        default:
            oam_warning_log1(0, OAM_SF_FTM, "{wal_hipriv_ftm_set_common_cmd::invalid cmd bit[0x%x]!}", g_ast_ftm_common_cmd[uc_cmd_index].ul_bit);
            break;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_ftm(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set = 0;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    mac_ftm_debug_switch_stru st_ftm_debug;
    uint32_t ul_ret = 0;
    int32_t l_ret;
    uint8_t uc_cmd_index = 0;

    memset_s(&st_ftm_debug, OAL_SIZEOF(st_ftm_debug), 0, OAL_SIZEOF(st_ftm_debug));

    do {
        /* 获取命令关键字 */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if ((ul_ret != OAL_SUCC) && (ul_off_set != 0)) {
            ftm_debug_cmd_format_info();
            return ul_ret;
        }
        pc_param += ul_off_set;

        if (ul_off_set == 0) {
            break;
        }

        /* 命令分类 */
        if (wal_hipriv_ftm_is_common_cmd(ac_name, &uc_cmd_index)) {
            ul_ret = wal_hipriv_ftm_set_common_cmd(pc_param, uc_cmd_index, &st_ftm_debug, &ul_off_set);
            pc_param += ul_off_set;
            ul_off_set = 0;
            if (ul_ret != OAL_SUCC) {
                return ul_ret;
            }
        } else if (0 == oal_strcmp("send_initial_ftm_request", ac_name)) {
            ul_ret = ftm_debug_parase_iftmr_cmd(pc_param, &st_ftm_debug, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                ftm_debug_cmd_format_info();
                return ul_ret;
            }
            pc_param += ul_off_set;
            ul_off_set = 0;
            st_ftm_debug.ul_cmd_bit_map |= BIT1;
        } else if (0 == oal_strcmp("send_ftm", ac_name)) {
            ul_ret = ftm_debug_parase_ftm_cmd(pc_param, &st_ftm_debug, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                ftm_debug_cmd_format_info();
                return ul_ret;
            }
            pc_param += ul_off_set;
            ul_off_set = 0;
            st_ftm_debug.ul_cmd_bit_map |= BIT4;
        } else if (0 == oal_strcmp("set_ftm_timeout", ac_name)) {
            ul_ret = ftm_debug_parase_ftm_timeout_cmd(pc_param, &st_ftm_debug, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                ftm_debug_cmd_format_info();
                return ul_ret;
            }
            pc_param += ul_off_set;
            ul_off_set = 0;
            st_ftm_debug.ul_cmd_bit_map |= BIT14;
        } else if (0 == oal_strcmp("set_cali", ac_name)) {
            ul_ret = ftm_debug_set_cali(pc_param, &st_ftm_debug, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                ftm_debug_cmd_format_info();
                return ul_ret;
            }
            pc_param += ul_off_set;
            ul_off_set = 0;
            st_ftm_debug.ul_cmd_bit_map |= BIT13;
        } else if (0 == oal_strcmp("send_range_req", ac_name)) {
            ul_ret = ftm_debug_parase_range_req_cmd(pc_param, &st_ftm_debug, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                ftm_debug_cmd_format_info();
                return ul_ret;
            }
            pc_param += ul_off_set;
            ul_off_set = 0;
            st_ftm_debug.ul_cmd_bit_map |= BIT7;
        } else if (0 == oal_strcmp("get_cali", ac_name)) {
            st_ftm_debug.ul_cmd_bit_map |= BIT9;
        } else if (0 == oal_strcmp("set_ftm_m2s", ac_name)) {
            ul_ret = ftm_debug_parase_m2s_cmd(pc_param, &st_ftm_debug, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                ftm_debug_cmd_format_info();
                return ul_ret;
            }
            pc_param += ul_off_set;
            ul_off_set = 0;
            st_ftm_debug.ul_cmd_bit_map |= BIT11;
        } else if (0 == oal_strcmp("send_nbr_rpt_req", ac_name)) {
            ul_ret = ftm_debug_parase_neighbor_report_req_cmd(pc_param, &st_ftm_debug, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                ftm_debug_cmd_format_info();
                return ul_ret;
            }
            pc_param += ul_off_set;
            ul_off_set = 0;
            st_ftm_debug.ul_cmd_bit_map |= BIT15;
        } else if (0 == oal_strcmp("send_gas_init_req", ac_name)) {
            ul_ret = ftm_debug_parase_gas_init_req_cmd(pc_param, &st_ftm_debug, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                ftm_debug_cmd_format_info();
                return ul_ret;
            }
            pc_param += ul_off_set;
            ul_off_set = 0;
            st_ftm_debug.ul_cmd_bit_map |= BIT16;
        } else {
            ftm_debug_cmd_format_info();
            return OAL_FAIL;
        }
    } while (*pc_param != '\0');

    oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_ftm::ul_cmd_bit_map: 0x%08x.}", st_ftm_debug.ul_cmd_bit_map);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_FTM_DBG, OAL_SIZEOF(st_ftm_debug));

    /* 设置配置命令参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value),
                        (const void *)&st_ftm_debug, OAL_SIZEOF(st_ftm_debug))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_ftm::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_ftm_debug),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_FTM, "{wal_hipriv_ftm::return err code[%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t _wal_hipriv_ftm(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    if (g_wlan_spec_cfg->feature_ftm_is_open) {
        return wal_hipriv_ftm(pst_net_dev, pc_param);
    }

    return OAL_SUCC;
}
#endif


OAL_STATIC uint32_t wal_hipriv_send_2040_coext(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint32_t ul_ret;
    uint32_t ul_off_set = 0;
    mac_cfg_set_2040_coexist_stru *pst_2040_coexist;

    /***************************************************************************
                              抛事件到wal层处理
    ***************************************************************************/
    pst_2040_coexist = (mac_cfg_set_2040_coexist_stru *)st_write_msg.auc_value;
    /* 获取mib名称 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_send_2040_coext::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    pst_2040_coexist->ul_coext_info = (uint32_t)oal_atoi(ac_name);

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_send_2040_coext::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    pst_2040_coexist->ul_channel_report = (uint32_t)oal_atoi(ac_name);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SEND_2040_COEXT,
                           OAL_SIZEOF(mac_cfg_set_2040_coexist_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_set_2040_coexist_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_send_2040_coext::wal_send_cfg_event return err_code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_get_version(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;

    /***************************************************************************
                              抛事件到wal层处理
    ***************************************************************************/
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value), pc_param, OAL_STRLEN(pc_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_get_version::memcpy fail!}");
        return OAL_FAIL;
    }

    st_write_msg.auc_value[OAL_STRLEN(pc_param)] = '\0';

    us_len = (uint16_t)(OAL_STRLEN(pc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_GET_VERSION, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_get_version::wal_send_cfg_event return err_code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY

OAL_STATIC uint32_t wal_hipriv_get_user_nssbw(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_add_user_param_stru *pst_add_user_param;
    mac_cfg_add_user_param_stru st_add_user_param; /* 临时保存获取的use的信息 */
    uint32_t ul_get_addr_idx;

    /* 获取用户带宽和空间流信息: hipriv "vap0 add_user xx xx xx xx xx xx(mac地址)" */
    memset_s((void *)&st_add_user_param, OAL_SIZEOF(mac_cfg_add_user_param_stru),
             0, OAL_SIZEOF(mac_cfg_add_user_param_stru));
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_add_user::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), st_add_user_param.auc_mac_addr, WLAN_MAC_ADDR_LEN);

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    /* 设置配置命令参数 */
    pst_add_user_param = (mac_cfg_add_user_param_stru *)(st_write_msg.auc_value);
    for (ul_get_addr_idx = 0; ul_get_addr_idx < WLAN_MAC_ADDR_LEN; ul_get_addr_idx++) {
        pst_add_user_param->auc_mac_addr[ul_get_addr_idx] = st_add_user_param.auc_mac_addr[ul_get_addr_idx];
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_GET_USER_RSSBW, OAL_SIZEOF(mac_cfg_add_user_param_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_add_user_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_add_user::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_FEATURE_M2S


uint32_t wal_hipriv_mss_param_handle(mac_m2s_mgr_stru *pst_m2s_mgr, int8_t *pc_param)
{
    uint32_t ul_ret;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    uint32_t ul_off_set = 0;

    /* 1.获取第二参数: 期望切换到的状态 */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_mss_param_handle::wal_get_cmd_one_arg2 return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    pst_m2s_mgr->pri_data.mss_mode.en_mss_on = (uint8_t)oal_atoi(ac_name);
    if (pst_m2s_mgr->pri_data.mss_mode.en_mss_on >= 2) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_mss_param_handle::pst_m2s_mgr->pri_data.mss_mode.en_mss_on error [%d] (0 or 1)!}\r\n",
                         pst_m2s_mgr->pri_data.mss_mode.en_mss_on);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_M2S_MODEM

uint32_t wal_hipriv_modem_param_handle(mac_m2s_mgr_stru *pst_m2s_mgr, int8_t *pc_param)
{
    uint32_t ul_ret;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    uint32_t ul_off_set = 0;

    /* 1.获取第二参数: 期望切换到的状态 */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_mss_param_handle::wal_get_cmd_one_arg2 return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    pst_m2s_mgr->pri_data.modem_mode.en_m2s_state = (uint8_t)oal_atoi(ac_name);

    return OAL_SUCC;
}
#endif


uint32_t wal_hipriv_rssi_param_handle(mac_m2s_mgr_stru *pst_m2s_mgr, int8_t *pc_param)
{
    uint32_t ul_ret;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    uint32_t ul_off_set = 0;

    /* 1.获取第二个参数 */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_set_m2s_switch::wal_get_cmd_one_arg2 return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    pst_m2s_mgr->pri_data.rssi_mode.uc_opt = (uint8_t)oal_atoi(ac_name);

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_set_m2s_switch::wal_get_cmd_one_arg2 return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    pst_m2s_mgr->pri_data.rssi_mode.c_value = (int8_t)oal_atoi(ac_name);

    return OAL_SUCC;
}

uint8_t wal_hipriv_m2s_switch_get_mcm_chain(uint16_t param)
{
    uint8_t chain_index;
    uint8_t mcm_chain = 0;

    /* 切换命令第二个参数含义为使用字符串表示的二进制如C2SISO为0100 */
    for (chain_index = 0; chain_index < MCM_CHAIN_NUM_MAX; chain_index++) {
        if ((param % MCM_CONFIG_CHAIN_STEP) != 0) {
            mcm_chain |= ((uint8_t)(1 << chain_index));
        }
        param /= MCM_CONFIG_CHAIN_STEP;
    }
    return mcm_chain;
}


uint32_t wal_hipriv_m2s_switch_param_handle(mac_m2s_mgr_stru *m2s_mgr, uint8_t *param,
    uint32_t *cmd_off_set, uint8_t *name)
{
    uint32_t ret;

    ret = wal_get_cmd_one_arg((uint8_t *)param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, cmd_off_set);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_m2s_switch_param_handle::wal_get_cmd_one_arg2 \
            return err_code [%d]!}\r\n", ret);
        return ret;
    }
    if (get_hi110x_subchip_type() == BOARD_VERSION_HI1106) {
        m2s_mgr->pri_data.test_mode.uc_m2s_state = wal_hipriv_m2s_switch_get_mcm_chain((uint16_t)oal_atoi(name));
    } else {
        m2s_mgr->pri_data.test_mode.uc_m2s_state = (uint8_t)oal_atoi(name);
    }
    /* 偏移，取下一个参数 */
    param = param + (*cmd_off_set);

    /* 2.获取第三个参数:主路还是辅路 0为主路 1为辅路 暂时不使用 空缺TBD */
    ret = wal_get_cmd_one_arg((uint8_t *)param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, cmd_off_set);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_m2s_switch_param_handle::wal_get_cmd_one_arg3 \
            return err_code [%d]!}\r\n", ret);
        return ret;
    }

    m2s_mgr->pri_data.test_mode.uc_master_id = (uint8_t)oal_atoi(name);
    if (m2s_mgr->pri_data.test_mode.uc_master_id >= HAL_DEVICE_ID_BUTT) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_m2s_switch_param_handle:: \
            pst_m2s_mgr->pri_data.test_mode.uc_master_id error [%d] 0/1!}\r\n",
            m2s_mgr->pri_data.test_mode.uc_master_id);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }
    /* true: 硬切换测试模式采用默认软切换配置 */
    /* false: 业务切换，软切换测试模式采用默认软切换配置 */
    m2s_mgr->pri_data.test_mode.en_m2s_type = (m2s_mgr->en_cfg_m2s_mode == MAC_M2S_MODE_HW_TEST) ?
        WLAN_M2S_TYPE_HW : WLAN_M2S_TYPE_SW;

    /* 标识业务类型 */
    m2s_mgr->pri_data.test_mode.uc_trigger_mode =
        (m2s_mgr->en_cfg_m2s_mode == MAC_M2S_MODE_DELAY_SWITCH) ?
        WLAN_M2S_TRIGGER_MODE_COMMAND : WLAN_M2S_TRIGGER_MODE_TEST;
    return OAL_SUCC;
}



uint32_t wal_hipriv_set_m2s_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint32_t ul_ret;
    mac_m2s_mgr_stru *pst_m2s_mgr;
    uint32_t ul_off_set = 0;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };

    /* st_write_msg作清零操作 */
    memset_s(&st_write_msg, OAL_SIZEOF(wal_msg_write_stru),
             0, OAL_SIZEOF(wal_msg_write_stru));

    /* 设置配置命令参数 */
    pst_m2s_mgr = (mac_m2s_mgr_stru *)st_write_msg.auc_value;

    /* 1.获取第一个参数: mode */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_set_m2s_switch::wal_get_cmd_one_arg1 return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    pst_m2s_mgr->en_cfg_m2s_mode = (mac_m2s_mode_enum_uint8)oal_atoi(ac_name);

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    switch (pst_m2s_mgr->en_cfg_m2s_mode) {
        /* mimo-siso切换参数查询 */
        case MAC_M2S_MODE_QUERY:
            /* 抛事件dmac打印全局管理参数即可 */
            break;

        case MAC_M2S_MODE_MSS:
            ul_ret = wal_hipriv_mss_param_handle(pst_m2s_mgr, pc_param);
            if (ul_ret != OAL_SUCC) {
                return ul_ret;
            }
            break;

        case MAC_M2S_MODE_DELAY_SWITCH:
        case MAC_M2S_MODE_SW_TEST:
        case MAC_M2S_MODE_HW_TEST:
            ul_ret = wal_hipriv_m2s_switch_param_handle(pst_m2s_mgr, pc_param, &ul_off_set, ac_name);
            if (ul_ret != OAL_SUCC) {
                return ul_ret;
            }
            break;
        case MAC_M2S_MODE_RSSI:
            ul_ret = wal_hipriv_rssi_param_handle(pst_m2s_mgr, pc_param);
            if (ul_ret != OAL_SUCC) {
                return ul_ret;
            }
            break;

#ifdef _PRE_WLAN_FEATURE_M2S_MODEM
        case MAC_M2S_MODE_MODEM:
            if (g_wlan_spec_cfg->feature_m2s_modem_is_open) {
                ul_ret = wal_hipriv_modem_param_handle(pst_m2s_mgr, pc_param);
                if (ul_ret != OAL_SUCC) {
                    return ul_ret;
                }
                break;
            }
            /* fall through */
#endif

        default:
            oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_set_m2s_switch: en_cfg_m2s_mode[%d] error!}", pst_m2s_mgr->en_cfg_m2s_mode);
            return OAL_FAIL;
    }

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_M2S_SWITCH, OAL_SIZEOF(mac_m2s_mgr_stru));

    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_m2s_mgr_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE, OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_set_m2s_stitch::wal_hipriv_reset_device return err code = [%d].}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif


OAL_STATIC uint32_t wal_hipriv_radar_set(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    memset_s(&st_write_msg, OAL_SIZEOF(wal_msg_write_stru), 0, OAL_SIZEOF(wal_msg_write_stru));

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_RADAR, 0);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_radar_set::wal_send_cfg_event return err code = [%d].}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_blacklist_add(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;
    uint32_t ul_ret;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    uint32_t ul_off_set = 0;
    mac_blacklist_stru *pst_blklst = NULL;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_blacklist_add:wal_get_cmd_one_arg fail!}\r\n");
        return ul_ret;
    }
    memset_s((uint8_t *)&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
    pst_blklst = (mac_blacklist_stru *)(st_write_msg.auc_value);

    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), pst_blklst->auc_mac_addr, WLAN_MAC_ADDR_LEN); /* 将字符 ac_name 转换成数组 mac_add[6] */

    us_len = OAL_SIZEOF(mac_blacklist_stru);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADD_BLACK_LIST, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_blacklist_add:wal_send_cfg_event return[%d].}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_blacklist_del(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;
    uint32_t ul_ret;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    uint32_t ul_off_set = 0;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_blacklist_add:wal_get_cmd_one_arg fail!}\r\n");
        return ul_ret;
    }
    memset_s((uint8_t *)&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));

    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), st_write_msg.auc_value, WLAN_MAC_ADDR_LEN); /* 将字符 ac_name 转换成数组 mac_add[6] */

    us_len = OAL_MAC_ADDR_LEN; /* OAL_SIZEOF(uint8_t); */

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DEL_BLACK_LIST, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_blacklist_add:wal_send_cfg_event return[%d].}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_blacklist_mode(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    return wal_hipriv_send_cfg_uint32_data(pst_net_dev, pc_param, WLAN_CFGID_BLACKLIST_MODE);
}

OAL_STATIC uint32_t wal_hipriv_blacklist_show(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    memset_s(&st_write_msg, OAL_SIZEOF(wal_msg_write_stru), 0, OAL_SIZEOF(wal_msg_write_stru));

    us_len = 4; /* OAL_SIZEOF(uint32_t) */
    *(uint32_t *)(st_write_msg.auc_value) = 1;

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_BLACKLIST_SHOW, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_blacklist_show:wal_send_cfg_event return [%d].}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_vap_classify_en(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint32_t ul_val = 0xff;
    wal_msg_write_stru st_write_msg;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret == OAL_SUCC) {
        ul_val = (uint32_t)oal_atoi(ac_name);
    }

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_VAP_CLASSIFY_EN, OAL_SIZEOF(uint32_t));

    /* 设置配置命令参数 */
    *((uint32_t *)(st_write_msg.auc_value)) = ul_val;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_packet_xmit::wal_send_cfg_event fail.return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_vap_classify_tid(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint32_t ul_val = 0xff;
    wal_msg_write_stru st_write_msg;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret == OAL_SUCC) {
        ul_val = (uint32_t)oal_atoi(ac_name);
    }

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_VAP_CLASSIFY_TID, OAL_SIZEOF(uint32_t));

    /* 设置配置命令参数 */
    *((uint32_t *)(st_write_msg.auc_value)) = ul_val;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_packet_xmit::wal_send_cfg_event fail.return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_sta_psm_param(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint16_t us_beacon_timeout;
    uint16_t us_tbtt_offset;
    uint16_t us_ext_tbtt_offset;
    uint16_t us_dtim3_on;
    mac_cfg_ps_param_stru *pst_ps_para = NULL;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_sta_psm_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    us_beacon_timeout = (uint16_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_sta_psm_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    us_tbtt_offset = (uint16_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_sta_psm_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    us_ext_tbtt_offset = (uint16_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_sta_psm_param::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    us_dtim3_on = (uint16_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_PSM_PARAM, OAL_SIZEOF(mac_cfg_ps_param_stru));

    /* 设置配置命令参数 */
    pst_ps_para = (mac_cfg_ps_param_stru *)(st_write_msg.auc_value);
    pst_ps_para->us_beacon_timeout = us_beacon_timeout;
    pst_ps_para->us_tbtt_offset = us_tbtt_offset;
    pst_ps_para->us_ext_tbtt_offset = us_ext_tbtt_offset;
    pst_ps_para->us_dtim3_on = us_dtim3_on;

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ps_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_PWR, "{wal_hipriv_sta_psm_param::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


uint32_t wal_hipriv_sta_pm_on(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint8_t uc_sta_pm_open;
    mac_cfg_ps_open_stru *pst_sta_pm_open = NULL;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_sta_pm_open::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    uc_sta_pm_open = (uint8_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_STA_PM_ON, OAL_SIZEOF(mac_cfg_ps_open_stru));

    /* 设置配置命令参数 */
    pst_sta_pm_open = (mac_cfg_ps_open_stru *)(st_write_msg.auc_value);
    /* MAC_STA_PM_SWITCH_ON / MAC_STA_PM_SWITCH_OFF */
    pst_sta_pm_open->uc_pm_enable = uc_sta_pm_open;
    pst_sta_pm_open->uc_pm_ctrl_type = MAC_STA_PM_CTRL_TYPE_CMD;

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ps_open_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_PWR, "{wal_hipriv_sta_pm_open::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_TCP_OPT

OAL_STATIC uint32_t wal_hipriv_get_tcp_ack_stream_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_GET_TCP_ACK_STREAM_INFO, OAL_SIZEOF(uint32_t));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_show_arpoffload_info::wal_send_cfg_event fail.return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_tcp_tx_ack_opt_enable(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint32_t ul_val;
    wal_msg_write_stru st_write_msg;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_tcp_tx_ack_opt_enable::wal_get_cmd_one_arg vap name return err_code %d!}\r\n", ul_ret);
        return ul_ret;
    }

    pc_param += ul_off_set;

    ul_val = (uint32_t)oal_atoi((const int8_t *)ac_name);

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_TX_TCP_ACK_OPT_ENALBE, OAL_SIZEOF(uint32_t));

    /* 设置配置命令参数 */
    *((uint32_t *)(st_write_msg.auc_value)) = ul_val;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_show_arpoffload_info::wal_send_cfg_event fail.return err code [%ud]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_tcp_rx_ack_opt_enable(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint32_t ul_val;
    wal_msg_write_stru st_write_msg;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_tcp_rx_ack_opt_enable::wal_get_cmd_one_arg vap name return err_code %d!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    ul_val = (uint32_t)oal_atoi((const int8_t *)ac_name);

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_RX_TCP_ACK_OPT_ENALBE, OAL_SIZEOF(uint32_t));

    /* 设置配置命令参数 */
    *((uint32_t *)(st_write_msg.auc_value)) = ul_val;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_show_arpoffload_info::wal_send_cfg_event fail.return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_tcp_tx_ack_limit(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint32_t ul_val;
    wal_msg_write_stru st_write_msg;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_tcp_tx_ack_limit::wal_get_cmd_one_arg vap name return err_code %d!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    ul_val = (uint32_t)oal_atoi((const int8_t *)ac_name);

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_TX_TCP_ACK_OPT_LIMIT, OAL_SIZEOF(uint32_t));

    /* 设置配置命令参数 */
    *((uint32_t *)(st_write_msg.auc_value)) = ul_val;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_show_arpoffload_info::wal_send_cfg_event fail.return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_tcp_rx_ack_limit(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint32_t ul_val;
    wal_msg_write_stru st_write_msg;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_tcp_tx_ack_limit::wal_get_cmd_one_arg vap name return err_code %d!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    ul_val = (uint32_t)oal_atoi((const int8_t *)ac_name);

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_RX_TCP_ACK_OPT_LIMIT, OAL_SIZEOF(uint32_t));

    /* 设置配置命令参数 */
    *((uint32_t *)(st_write_msg.auc_value)) = ul_val;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_show_arpoffload_info::wal_send_cfg_event fail.return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif


OAL_STATIC uint32_t wal_hipriv_enable_2040bss(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    uint8_t uc_2040bss_switch;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_enable_2040bss::wal_get_cmd_one_arg return err_code %d!}\r\n", ul_ret);
        return ul_ret;
    }

    if ((0 != oal_strcmp("0", ac_name)) && (0 != oal_strcmp("1", ac_name))) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_hipriv_enable_2040bss::invalid parameter.}\r\n");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    uc_2040bss_switch = (uint8_t)oal_atoi(ac_name);

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_2040BSS_ENABLE, OAL_SIZEOF(uint8_t));
    *((uint8_t *)(st_write_msg.auc_value)) = uc_2040bss_switch; /* 设置配置命令参数 */

    ul_ret = (uint32_t)wal_send_cfg_event(pst_net_dev,
                                                WAL_MSG_TYPE_WRITE,
                                                WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint8_t),
                                                (uint8_t *)&st_write_msg,
                                                OAL_FALSE,
                                                OAL_PTR_NULL);

    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_enable_2040bss::return err code %d!}\r\n", ul_ret);
    }

    return ul_ret;
}

#if defined(_PRE_WLAN_FEATURE_11V_ENABLE)

OAL_STATIC uint32_t wal_hipriv_11v_cfg_bsst(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint8_t uc_debug_switch;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_11v_cfg_bsst::wal_get_cmd_one_arg return err_code %d!}\r\n", ul_ret);
        return ul_ret;
    }
    /* 参数0为关闭能力位 1位打开能力位，其他参数不支持 */
    uc_debug_switch = (uint8_t)oal_atoi((const int8_t *)ac_name);
    switch (uc_debug_switch) {
        case 0:
        case 1:
            break;
        default:
            oam_warning_log0(0, OAM_SF_CFG, "{wal_hipriv_11v_cfg_wl_mgmt::command param is error!}\r\n");
            return OAL_ERR_CODE_INVALID_CONFIG;
    }
    /***************************************************************************
                              抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_11V_BSST_SWITCH, OAL_SIZEOF(uint8_t));
    st_write_msg.auc_value[0] = uc_debug_switch; /* 设置配置命令参数 */

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint8_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_11v_cfg_bsst::return err code %d!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_11v_tx_query(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint32_t ul_ret;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN] = { 0 };

    /* 获取mac地址 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_11v_tx_query::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), auc_mac_addr, WLAN_MAC_ADDR_LEN);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_11V_TX_QUERY, OAL_SIZEOF(auc_mac_addr));

    /* 设置配置命令参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value), auc_mac_addr, WLAN_MAC_ADDR_LEN)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_dpd_cfg::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(auc_mac_addr),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_11v_tx_query::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif


OAL_STATIC uint32_t wal_hipriv_set_bw_fixed(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    int8_t ac_flag[2];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    oal_bool_enum_uint8 *puc_bw_fixed_flag = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_net_dev, pc_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_bw_fixed::pst_net_dev/p_param null ptr error!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (oal_unlikely(pst_mac_vap == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_hipriv_set_bw_fixed::can't get mac vap from netdevice priv data!}");
        return OAL_FAIL;
    }

    /* 设备在up状态不允许配置，必须先down */
    if (pst_mac_vap->en_vap_state != MAC_VAP_STATE_INIT) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_set_bw_fixed::device is busy, please down it first %d!}\r\n", pst_mac_vap->en_vap_state);
        return OAL_FAIL;
    }

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_flag, OAL_SIZEOF(ac_flag), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "wal_hipriv_set_bw_fixed: get first arg fail.");
        return OAL_FAIL;
    }

    /***************************************************************************
                            抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_BW_FIXED, OAL_SIZEOF(uint8_t));

    /* 设置配置命令参数 */
    puc_bw_fixed_flag = (uint8_t *)(st_write_msg.auc_value);
    *puc_bw_fixed_flag = (uint8_t)oal_atoi(ac_flag);

    *puc_bw_fixed_flag = !!(*puc_bw_fixed_flag);

    oam_warning_log1(0, OAM_SF_ANY, "wal_hipriv_set_bw_fixed:: bw_fixed = %d.", *puc_bw_fixed_flag);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_bool_enum_uint8),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_bw_fixed::wal_send_cfg_event fail.return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_PSD_ANALYSIS

OAL_STATIC uint32_t wal_hipriv_set_psd(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint32_t ul_value;

    if (oal_unlikely(oal_any_null_ptr2(pst_cfg_net_dev, pc_param))) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_hipriv_set_psd::pst_cfg_net_dev or pc_param null ptr error %x, %x!}\r\n", (uintptr_t)pst_cfg_net_dev, (uintptr_t)pc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* PSD设置开关的命令: hipriv "vap0 set_psd 0 | 1"
            此处将解析出"1"或"0"存入ac_name
    */

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_psd::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    if (0 == (oal_strcmp("0", ac_name))) {
        ul_value = 0;
    } else if (0 == (oal_strcmp("1", ac_name))) {
        ul_value = 1;
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_psd::the set psd command is error %x!}\r\n", (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_PSD, OAL_SIZEOF(uint32_t));

    /* 设置配置命令参数 */
    *((uint32_t *)(st_write_msg.auc_value)) = ul_value;

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_psd::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_cfg_psd(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    int32_t l_ret;
    oal_bool_enum_uint8 en_enable;
    wal_msg_write_stru st_write_msg;

    /* PSD设置开关的命令: hipriv "vap0 cfg_psd */

    en_enable = 1;
    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CFG_PSD, OAL_SIZEOF(uint32_t));

    /* 设置配置命令参数 */
    *((oal_bool_enum_uint8 *)(st_write_msg.auc_value)) = en_enable;

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_bool_enum_uint8),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_cfg_psd::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#endif
#ifdef _PRE_WLAN_FEATURE_CSI

OAL_STATIC uint32_t wal_hipriv_set_csi(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    return wal_ioctl_set_csi_switch(pst_cfg_net_dev, pc_param);
}
#endif

OAL_STATIC uint32_t wal_hipriv_parase_send_frame_body(int8_t *p_str_body,
                                                        uint8_t uc_str_body_len, uint8_t *p_char_body, uint8_t uc_char_body_lenth, uint8_t *p_char_body_len)
{
    uint8_t uc_index;
    uint8_t uc_value = 0;
    uint8_t uc_hvalue = 0;
    uint8_t uc_char_body_len = 0;

    if (oal_any_null_ptr3(p_str_body, p_char_body, p_char_body_len)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_parase_send_frame_body::param is NULL,return!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if ((uc_str_body_len % 2) != 0) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_parase_send_frame_body::uc_str_body_len=%d invalid,return!}\r\n", uc_str_body_len);
        return OAL_FAIL;
    }

    for (uc_index = 0; uc_index < uc_str_body_len; uc_index++) {
        if (p_str_body[uc_index] >= '0' && p_str_body[uc_index] <= '9') {
            uc_value = (uint8_t)(p_str_body[uc_index] - '0');
        } else if (p_str_body[uc_index] >= 'A' && p_str_body[uc_index] <= 'F') {
            uc_value = (uint8_t)(p_str_body[uc_index] - 'A' + 10);
        } else {
            oam_warning_log2(0, OAM_SF_ANY, "{wal_hipriv_parase_send_frame_body::p_str_body[%d]=%c invalid,return!}\r\n", uc_index, (uint8_t)p_str_body[uc_index]);
            return OAL_FAIL;
        }

        if ((uc_index % 2) == 0) {
            uc_hvalue = ((uc_value << 4) & 0XF0);
        } else if ((uc_index % 2) == 1) {
            p_char_body[uc_char_body_len] = uc_value + uc_hvalue;
            uc_value = 0;
            uc_char_body_len++;
            if (uc_char_body_len >= uc_char_body_lenth) {
                oam_warning_log2(0, OAM_SF_ANY, "{wal_hipriv_parase_send_frame_body::uc_char_body_len =%d over_max = %d!}\r\n",
                                 uc_char_body_len, MAC_TEST_INCLUDE_FRAME_BODY_LEN);
                return OAL_FAIL;
            }
        } else {
            uc_value = 0;
        }
    }

    *p_char_body_len = uc_char_body_len;
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_send_frame(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_offset;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    mac_cfg_send_frame_param_stru *pst_test_send_frame = OAL_PTR_NULL;
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN] = { 0 };

    mac_test_frame_type_enum_uint8 en_frame_type;
    uint8_t uc_pkt_num;
    int8_t ac_str_frame_body[MAC_TEST_INCLUDE_FRAME_BODY_LEN * 2] = { 0 };
    uint8_t uc_str_frame_body_len;
    uint8_t ac_frame_body[MAC_TEST_INCLUDE_FRAME_BODY_LEN] = { 0 };
    uint8_t uc_frame_body_len = 0;

    /* 获取帧类型 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_offset);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_send_frame::get frame type err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    en_frame_type = (mac_test_frame_type_enum_uint8)oal_atoi(ac_name);
    pc_param = pc_param + ul_offset;

    /* 获取帧数目 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_offset);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_send_frame::get frame num err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    uc_pkt_num = (uint8_t)oal_atoi(ac_name);
    pc_param += ul_offset;

    /* 获取MAC地址字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_offset);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_send_frame::get mac err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    /* 地址字符串转地址数组 */
    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), auc_mac_addr, WLAN_MAC_ADDR_LEN);
    pc_param += ul_offset;

    if (en_frame_type == MAC_TEST_MGMT_ACTION || en_frame_type == MAC_TEST_MGMT_BEACON_INCLUDE_IE) {
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_str_frame_body, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_offset);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_send_frame::get frame body err_code = [%d]!}\r\n", ul_ret);
            return ul_ret;
        }

        pc_param += ul_offset;
        uc_str_frame_body_len = (uint8_t)(OAL_STRLEN(ac_str_frame_body));
        ul_ret = wal_hipriv_parase_send_frame_body(ac_str_frame_body, uc_str_frame_body_len, ac_frame_body, sizeof(ac_frame_body), &uc_frame_body_len);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_send_frame::parase_send_frame_body err_code = [%d]!}\r\n", ul_ret);
            return ul_ret;
        }
    }

    /***************************************************************************
                                 抛事件到dmac层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SEND_FRAME, OAL_SIZEOF(mac_cfg_send_frame_param_stru));

    /* 设置配置命令参数 */
    pst_test_send_frame = (mac_cfg_send_frame_param_stru *)(st_write_msg.auc_value);
    memset_s(pst_test_send_frame, OAL_SIZEOF(mac_cfg_send_frame_param_stru),
             0, OAL_SIZEOF(mac_cfg_send_frame_param_stru));
    oal_set_mac_addr(pst_test_send_frame->auc_mac_ra, auc_mac_addr);
    pst_test_send_frame->en_frame_type = en_frame_type;
    pst_test_send_frame->uc_pkt_num = uc_pkt_num;
    pst_test_send_frame->uc_frame_body_length = uc_frame_body_len;
    if (EOK != memcpy_s(pst_test_send_frame->uc_frame_body, sizeof(pst_test_send_frame->uc_frame_body),
                        ac_frame_body, uc_frame_body_len)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_send_frame::memcpy fail!}");
        return OAL_FAIL;
    }

    oam_warning_log3(0, OAM_SF_ANY, "{wal_hipriv_send_frame:: frame_type = [%d] send_times = %d body_len=%d!}\r\n",
                     pst_test_send_frame->en_frame_type, pst_test_send_frame->uc_pkt_num, pst_test_send_frame->uc_frame_body_length);
    ul_ret = (uint32_t)wal_send_cfg_event(pst_net_dev,
                                                WAL_MSG_TYPE_WRITE,
                                                WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_send_frame_param_stru),
                                                (uint8_t *)&st_write_msg,
                                                OAL_FALSE,
                                                OAL_PTR_NULL);

    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_send_frame::wal_send_cfg_event return err_code [%d]!}\r\n", ul_ret);
        return (uint32_t)ul_ret;
    }

    return OAL_SUCC;
}

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)

OAL_STATIC uint32_t wal_hipriv_enable_pmf(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_ret;

    int32_t l_cfg_rst;
    uint16_t us_len;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_off_set = 0;
    uint8_t uc_chip_test_open;

    /* 获取设定的值 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_enable_pmf::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    uc_chip_test_open = (uint8_t)oal_atoi(ac_name);
    pc_param += ul_off_set;

    /***************************************************************************
                              抛事件到wal层处理
    ***************************************************************************/
    us_len = OAL_SIZEOF(uint8_t);
    *(uint8_t *)(st_write_msg.auc_value) = uc_chip_test_open;
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_PMF_ENABLE, us_len);

    l_cfg_rst = wal_send_cfg_event(pst_net_dev,
                                       WAL_MSG_TYPE_WRITE,
                                       WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                       (uint8_t *)&st_write_msg,
                                       OAL_FALSE,
                                       OAL_PTR_NULL);

    if (oal_unlikely(l_cfg_rst != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_enable_pmf::wal_send_cfg_event return err_code [%d]!}\r\n", l_cfg_rst);
        return (uint32_t)l_cfg_rst;
    }

    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG

uint32_t wal_hipriv_show_rr_time_info(oal_net_device_stru *net_dev, int8_t *param)
{
    uint32_t ret;
    int32_t ret_event;
    uint32_t off_set;
    oal_switch_enum_uint8 rr_switch;
    int8_t name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    wal_msg_write_stru write_msg;

    ret = wal_get_cmd_one_arg(param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_CFG, "wal_hipriv_show_rr_time_info return err_code: %d", ret);
        return ret;
    }
    rr_switch = (oal_switch_enum_uint8)oal_atoi(name);
    if (rr_switch != OAL_SWITCH_OFF && rr_switch != OAL_SWITCH_ON) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_show_rr_time_info::command param is error!}\r\n");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&write_msg, WLAN_CFGID_ENABLE_RR_TIMEINFO, OAL_SIZEOF(oal_switch_enum_uint8));
    *(oal_switch_enum_uint8 *)(write_msg.auc_value) = rr_switch;

    ret_event = wal_send_cfg_event(net_dev,
                                       WAL_MSG_TYPE_WRITE,
                                       WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_switch_enum_uint8),
                                       (uint8_t *)&write_msg,
                                       OAL_FALSE,
                                       OAL_PTR_NULL);

    if (oal_unlikely(ret_event != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_show_rr_time_info::return err code[%d]!}\r\n", ret_event);
        return (uint32_t)ret_event;
    }

    return OAL_SUCC;
}
#endif

uint32_t wal_hipriv_show_arpoffload_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint8_t uc_show_ip_addr;
    uint8_t uc_show_arpoffload_info;
    mac_cfg_arpoffload_info_stru *pst_arpoffload_info = NULL;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_show_arpoffload_info::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    uc_show_ip_addr = (uint8_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_show_arpoffload_info::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    uc_show_arpoffload_info = (uint8_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SHOW_ARPOFFLOAD_INFO, OAL_SIZEOF(mac_cfg_arpoffload_info_stru));

    /* 设置配置命令参数 */
    pst_arpoffload_info = (mac_cfg_arpoffload_info_stru *)(st_write_msg.auc_value);
    pst_arpoffload_info->uc_show_ip_addr = uc_show_ip_addr;
    pst_arpoffload_info->uc_show_arpoffload_info = uc_show_arpoffload_info;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_arpoffload_info_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_PWR, "{wal_hipriv_show_arpoffload_info::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_roam_enable(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_ret;
    int32_t l_ret;
    uint32_t ul_off_set;
    oal_bool_enum_uint8 en_enable;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    wal_msg_write_stru st_write_msg;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ROAM, "roam enable type return err_code [%d]", ul_ret);
        return ul_ret;
    }
    en_enable = (oal_bool_enum_uint8)oal_atoi(ac_name);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ROAM_ENABLE, OAL_SIZEOF(uint32_t));
    *((oal_bool_enum_uint8 *)(st_write_msg.auc_value)) = en_enable;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_bool_enum_uint8),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_roam_enable::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_roam_start(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    int32_t l_ret;
    uint32_t ul_ret;
    uint8_t uc_scan_type = ROAM_SCAN_CHANNEL_ORG_FULL;
    oal_bool_enum_uint8 en_current_bss_ignore = OAL_TRUE;
    uint8_t auc_bssid[OAL_MAC_ADDR_LEN] = { 0 };
    wal_msg_write_stru st_write_msg;
    mac_cfg_set_roam_start_stru *pst_roam_start = NULL;

    uint32_t ul_off_set;
    uint8_t uc_param;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret == OAL_ERR_CODE_CONFIG_ARGS_OVER) {
        /* 默认扫描+漫游 */
        // uc_scan_type = ROAM_SCAN_CHANNEL_ORG_BUTT;
        // en_current_bss_ignore = OAL_TRUE;
        memset_s(auc_bssid, sizeof(auc_bssid), 0, sizeof(auc_bssid));
    } else if (ul_ret == OAL_SUCC) {
        /* 指定漫游时刻是否搭配扫描操作 */
        uc_scan_type = (uint8_t)oal_atoi(ac_name);

        pc_param += ul_off_set;
        /* 获取是否可以关联到自己的参数 */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret == OAL_ERR_CODE_CONFIG_ARGS_OVER) {
            /* 默认不能关联到自己 */
            // en_current_bss_ignore = OAL_TRUE;
            memset_s(auc_bssid, sizeof(auc_bssid), 0, sizeof(auc_bssid));
        } else if (ul_ret == OAL_SUCC) {
            uc_param = (uint8_t)oal_atoi(ac_name);
            en_current_bss_ignore = (uc_param == 0) ? OAL_TRUE : OAL_FALSE; /* 0/TRUE或者参数缺失表示漫游到其它AP, 1/FALSE表示漫游到自己 */

            /* 获取BSSID */
            pc_param = pc_param + ul_off_set;
            ul_ret = wal_hipriv_get_mac_addr(pc_param, auc_bssid, &ul_off_set);
            if (ul_ret == OAL_ERR_CODE_CONFIG_ARGS_OVER) {
                /* 默认不指定BSSID */
                memset_s(auc_bssid, sizeof(auc_bssid), 0, sizeof(auc_bssid));
            } else if (ul_ret != OAL_SUCC) {
                oam_warning_log0(0, OAM_SF_ROAM, "{wal_hipriv_roam_start::bssid failed!}\r\n");
                return ul_ret;
            }
        } else {
            oam_warning_log1(0, OAM_SF_ROAM, "{wal_hipriv_roam_start::roam_start return err_code [%d]!}\r\n", ul_ret);
            return (uint32_t)ul_ret;
        }
    } else {
        oam_warning_log1(0, OAM_SF_ROAM, "{wal_hipriv_roam_start::roam_start cfg_cmd error[%d]}", ul_ret);
        return ul_ret;
    }

    oam_warning_log4(0, OAM_SF_ROAM, "{wal_hipriv_roam_start::roam_start uc_scan_type[%d], en_current_bss_ignore[%d], target bssid[XXXX:%02X:%02X]}",
                     uc_scan_type, en_current_bss_ignore, auc_bssid[4], auc_bssid[5]);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    memset_s(&st_write_msg, OAL_SIZEOF(wal_msg_write_stru), 0, OAL_SIZEOF(wal_msg_write_stru));
    pst_roam_start = (mac_cfg_set_roam_start_stru *)(st_write_msg.auc_value);
    pst_roam_start->uc_scan_type = uc_scan_type;
    pst_roam_start->en_current_bss_ignore = en_current_bss_ignore;
    if (EOK != memcpy_s(pst_roam_start->auc_bssid, sizeof(pst_roam_start->auc_bssid),
                        auc_bssid, sizeof(auc_bssid))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_roam_start::memcpy fail!}");
        return OAL_FAIL;
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ROAM_START, OAL_SIZEOF(mac_cfg_set_roam_start_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_set_roam_start_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ROAM, "{wal_hipriv_roam_start::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI

uint32_t wal_hipriv_dyn_cali_cfg(oal_net_device_stru *pst_net_dev, int8_t *puc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    mac_ioctl_dyn_cali_param_stru *pst_dyn_cali_param = OAL_PTR_NULL;
    wal_ioctl_dyn_cali_stru st_cyn_cali_cfg;
    uint32_t ul_ret;
    uint8_t uc_map_index = 0;
    int32_t l_send_event_ret;

    pst_dyn_cali_param = (mac_ioctl_dyn_cali_param_stru *)(st_write_msg.auc_value);
    memset_s(pst_dyn_cali_param, OAL_SIZEOF(mac_ioctl_dyn_cali_param_stru),
             0, OAL_SIZEOF(mac_ioctl_dyn_cali_param_stru));

    if (oal_unlikely(oal_any_null_ptr2(pst_net_dev, puc_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_dyn_cali_cfg::pst_cfg_net_dev or puc_param null ptr error }\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = wal_get_cmd_one_arg(puc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_dyn_cali_cfg::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    /* 寻找匹配的命令 */
    st_cyn_cali_cfg = g_ast_dyn_cali_cfg_map[0];
    while (st_cyn_cali_cfg.pc_name != OAL_PTR_NULL) {
        if (0 == oal_strcmp(st_cyn_cali_cfg.pc_name, ac_name)) {
            break;
        }
        st_cyn_cali_cfg = g_ast_dyn_cali_cfg_map[++uc_map_index];
    }

    /* 没有找到对应的命令，则报错 */
    if (st_cyn_cali_cfg.pc_name == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_dyn_cali_cfg::invalid alg_cfg command!}\r\n");
        return OAL_FAIL;
    }

    /* 记录命令对应的枚举值 */
    pst_dyn_cali_param->en_dyn_cali_cfg = g_ast_dyn_cali_cfg_map[uc_map_index].en_dyn_cali_cfg;
    /* 获取参数配置值 */
    ul_ret = wal_get_cmd_one_arg(puc_param + ul_off_set, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret == OAL_SUCC) {
        /* 记录参数配置值 */
        pst_dyn_cali_param->us_value = (uint16_t)oal_atoi(ac_name);
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_dyn_cali_cfg::wal_get_cmd_one_arg [%d]!}\r\n", pst_dyn_cali_param->us_value);
    }

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DYN_CALI_CFG, OAL_SIZEOF(uint32_t));

    l_send_event_ret = wal_send_cfg_event(pst_net_dev,
                                              WAL_MSG_TYPE_WRITE,
                                              WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                              (uint8_t *)&st_write_msg,
                                              OAL_FALSE,
                                              OAL_PTR_NULL);
    if (oal_unlikely(l_send_event_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_cyn_cali_set_dscr_interval::wal_send_cfg_event return err code [%d]!}\r\n", l_send_event_ret);
        return (uint32_t)l_send_event_ret;
    }
    return OAL_SUCC;
}

#endif

#define WAL_HIPRIV_OWE_MAX 8
uint32_t wal_hipriv_set_owe(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_ret;
    int32_t l_ret;
    uint8_t uc_owe_group;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_off_set;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

    /* 获取字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_owe::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    /* 解析要设置的值 */
    uc_owe_group = (uint8_t)oal_atoi(ac_arg);
    if (uc_owe_group >= WAL_HIPRIV_OWE_MAX) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_owe::input value out of range [%d]!}\r\n", uc_owe_group);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_OWE, OAL_SIZEOF(uint8_t));

    /* 解析并设置配置命令参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value), &uc_owe_group, OAL_SIZEOF(uint8_t))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_owe::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint8_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_owe::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    /* 读取返回的错误码 */
    ul_ret = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_set_owe fail, err code[%u]!}\r\n", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_TWT

OAL_STATIC uint32_t wal_hipriv_twt_setup_req(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_twt_setup_req_param_stru *pst_twt_setup_req_param;
    mac_cfg_twt_setup_req_param_stru st_twt_setup_req_param; /* 临时保存获取的twt setup req的信息 */
    uint32_t ul_get_addr_idx;

    /*
        设置AMPDU关闭的配置命令:
        hipriv "Hisilicon0 twt_setup_req xx xx xx xx xx xx(mac地址)  setup_cmd flow_type flow_ID start_time_offset interval_exponent wake_duration interval_mantissa"
    */
    oam_warning_log0(0, OAM_SF_11AX, "{wal_hipriv_twt_setup_req\r\n");

    /* 获取mac地址 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_setup_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    memset_s((uint8_t *)&st_twt_setup_req_param, OAL_SIZEOF(st_twt_setup_req_param),
             0, OAL_SIZEOF(st_twt_setup_req_param));
    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), st_twt_setup_req_param.auc_mac_addr, WLAN_MAC_ADDR_LEN);
    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_setup_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_twt_setup_req_param.uc_twt_setup_cmd = (uint8_t)oal_atoi(ac_name);

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_setup_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_twt_setup_req_param.uc_twt_flow_type = (uint8_t)oal_atoi(ac_name);

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_setup_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_twt_setup_req_param.uc_twt_flow_id = (uint8_t)oal_atoi(ac_name);

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_setup_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_twt_setup_req_param.ul_twt_start_time_offset = (uint32_t)oal_atoi(ac_name);

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    /* 获取interval_exponent */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_setup_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_twt_setup_req_param.ul_twt_exponent = (uint32_t)oal_atoi(ac_name);
    if (st_twt_setup_req_param.ul_twt_exponent > 0x1F) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_setup_req::the ul_twt_exponent is not correct! ul_twt_exponent is[%d]!}\r\n", st_twt_setup_req_param.ul_twt_exponent);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pc_param = pc_param + ul_off_set;

    /* 获取wake_duration */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_setup_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_twt_setup_req_param.ul_twt_duration = (uint32_t)oal_atoi(ac_name);
    if (st_twt_setup_req_param.ul_twt_duration > 0xFF) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_setup_req::the ul_twt_duration is not correct! ul_twt_duration is[%d]!}\r\n", st_twt_setup_req_param.ul_twt_duration);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pc_param = pc_param + ul_off_set;

    /* 获取interval_mantissa */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_setup_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_twt_setup_req_param.ul_intrval_mantissa = (uint16_t)oal_atoi(ac_name);
    if (st_twt_setup_req_param.ul_intrval_mantissa > 0xFFFF) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_setup_req::the ul_intrval_mantissa is not correct! ul_intrval_mantissa is[%d]!}\r\n", st_twt_setup_req_param.ul_intrval_mantissa);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }
    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_TWT_SETUP_REQ, OAL_SIZEOF(mac_cfg_twt_setup_req_param_stru));

    /* 设置配置命令参数 */
    pst_twt_setup_req_param = (mac_cfg_twt_setup_req_param_stru *)(st_write_msg.auc_value);
    for (ul_get_addr_idx = 0; ul_get_addr_idx < WLAN_MAC_ADDR_LEN; ul_get_addr_idx++) {
        pst_twt_setup_req_param->auc_mac_addr[ul_get_addr_idx] = st_twt_setup_req_param.auc_mac_addr[ul_get_addr_idx];
    }

    pst_twt_setup_req_param->uc_twt_setup_cmd = st_twt_setup_req_param.uc_twt_setup_cmd;
    pst_twt_setup_req_param->uc_twt_flow_type = st_twt_setup_req_param.uc_twt_flow_type;
    pst_twt_setup_req_param->uc_twt_flow_id = st_twt_setup_req_param.uc_twt_flow_id;
    pst_twt_setup_req_param->ul_twt_start_time_offset = st_twt_setup_req_param.ul_twt_start_time_offset;
    pst_twt_setup_req_param->ul_twt_exponent = st_twt_setup_req_param.ul_twt_exponent;
    pst_twt_setup_req_param->ul_twt_duration = st_twt_setup_req_param.ul_twt_duration;
    pst_twt_setup_req_param->ul_intrval_mantissa = st_twt_setup_req_param.ul_intrval_mantissa;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_twt_setup_req_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_setup_req::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t _wal_hipriv_twt_setup_req(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    if (g_wlan_spec_cfg->feature_twt_is_open) {
        return wal_hipriv_twt_setup_req(pst_net_dev, pc_param);
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_twt_teardown_req(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_twt_teardown_req_param_stru *pst_twt_teardown_req_param;
    mac_cfg_twt_teardown_req_param_stru st_twt_teardown_req_param; /* 临时保存获取的twt teardown req的信息 */
    uint32_t ul_get_addr_idx;

    /*
     * 设置AMPDU关闭的配置命令:
     * hipriv "Hisilicon0 twt_teardown_req xx xx xx xx xx xx(mac地址) flwo_id"
     */
    /* 获取mac地址 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_teardown_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    memset_s((uint8_t *)&st_twt_teardown_req_param, OAL_SIZEOF(st_twt_teardown_req_param),
             0, OAL_SIZEOF(st_twt_teardown_req_param));
    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), st_twt_teardown_req_param.auc_mac_addr, WLAN_MAC_ADDR_LEN);
    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    /* 获取flwo_id */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_teardown_req::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_twt_teardown_req_param.uc_twt_flow_id = (uint8_t)oal_atoi(ac_name);
    if (st_twt_teardown_req_param.uc_twt_flow_id > 0x07) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_teardown_req::the twt teardown req command is error!uc_twt_flow_id is [%d]!}\r\n", st_twt_teardown_req_param.uc_twt_flow_id);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_TWT_TEARDOWN_REQ, OAL_SIZEOF(mac_cfg_twt_teardown_req_param_stru));

    /* 设置配置命令参数 */
    pst_twt_teardown_req_param = (mac_cfg_twt_teardown_req_param_stru *)(st_write_msg.auc_value);
    for (ul_get_addr_idx = 0; ul_get_addr_idx < WLAN_MAC_ADDR_LEN; ul_get_addr_idx++) {
        pst_twt_teardown_req_param->auc_mac_addr[ul_get_addr_idx] = st_twt_teardown_req_param.auc_mac_addr[ul_get_addr_idx];
    }

    pst_twt_teardown_req_param->uc_twt_flow_id = st_twt_teardown_req_param.uc_twt_flow_id;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_twt_teardown_req_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_11AX, "{wal_hipriv_twt_teardown_req::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t _wal_hipriv_twt_teardown_req(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    if (g_wlan_spec_cfg->feature_twt_is_open) {
        return wal_hipriv_twt_teardown_req(pst_net_dev, pc_param);
    }

    return OAL_SUCC;
}
#endif


uint32_t wal_hipriv_fem_lowpower(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_ret;
    int32_t l_ret;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_param;
    wal_msg_write_stru st_write_msg;

    /* 获取开关 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    l_param = oal_atoi((const int8_t *)ac_name);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SPEC_FEMLP_EN, OAL_SIZEOF(uint32_t));
    *((int32_t *)(st_write_msg.auc_value)) = l_param;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_fem_lowpower::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_icmp_filter(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
#ifndef WIN32
    uint32_t ul_ret;
    int32_t l_ret;
    uint16_t us_len;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    wal_msg_write_stru st_write_msg;
    mac_assigned_filter_cmd_stru st_assigned_filter_cmd;

    /* 准备配置命令 */
    us_len = OAL_SIZEOF(st_assigned_filter_cmd);
    memset_s((uint8_t *)&st_assigned_filter_cmd, us_len, 0, us_len);

    /* 获取filter id */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_icmp_filter::first para return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    if (MAC_ICMP_FILTER != (uint8_t)oal_atoi(ac_name)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_icmp_filter::icmp filter id not match.}");
        return OAL_FAIL;
    }

    st_assigned_filter_cmd.en_filter_id = MAC_ICMP_FILTER;

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    /* 获取开关 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_icmp_filter::second para return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    st_assigned_filter_cmd.en_enable = ((uint8_t)oal_atoi(ac_name) > 0) ? OAL_TRUE : OAL_FALSE;

    oam_warning_log2(0, OAM_SF_ANY, "{wal_hipriv_set_icmp_filter::filter id [%d] on/off[%d].}",
                     st_assigned_filter_cmd.en_filter_id, st_assigned_filter_cmd.en_enable);

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ASSIGNED_FILTER, us_len);
    /* 将申请的netbuf首地址填写到msg消息体内 */
    if (EOK != memcpy_s(st_write_msg.auc_value, sizeof(st_write_msg.auc_value), (uint8_t *)&st_assigned_filter_cmd, us_len)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_icmp_filter::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_icmp_filter::return err code[%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

#endif
    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_psm_flt_stat(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_off_set;
    uint32_t ul_ret;
    uint8_t uc_query_type;
    /* 获取query type */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_psm_flt_stat::first para return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    uc_query_type = (uint8_t)oal_atoi((const int8_t *)ac_name);
    wal_ioctl_get_psm_stat(pst_net_dev, uc_query_type, OAL_PTR_NULL);
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_GET_STATION_INFO_EXT
OAL_STATIC void wal_hipriv_printf_station_info(hmac_vap_stru *hmac_vap)
{
#ifdef CONFIG_HW_GET_EXT_SIG_ULDELAY
    oam_warning_log4(0, OAM_SF_ANY,
        "{wal_hipriv_get_station_info::tid delay[%d]ms, tx drop[%d], tx pkts[%d], tx bytes[%d]}",
        hmac_vap->station_info.ul_delay,
        hmac_vap->station_info.tx_failed,
        hmac_vap->station_info.tx_packets,
        hmac_vap->station_info.tx_bytes);
#else
    oam_warning_log3(0, OAM_SF_ANY,
        "{wal_hipriv_get_station_info::tx drop[%d], tx pkts[%d], tx bytes[%d]}",
        hmac_vap->station_info.tx_failed,
        hmac_vap->station_info.tx_packets,
        hmac_vap->station_info.tx_bytes);
#endif
}

OAL_STATIC uint32_t wal_hipriv_get_station_info(oal_net_device_stru *net_dev, int8_t *param)
{
    mac_vap_stru *mac_vap = OAL_PTR_NULL;
    hmac_vap_stru *hmac_vap = OAL_PTR_NULL;
    dmac_query_request_event dmac_query_request_event;
    dmac_query_station_info_request_event *query_station_info = OAL_PTR_NULL;
    wal_msg_write_stru write_msg;
    signed long leftime;
    int32_t ret;
    uint8_t vap_id;
    if (oal_any_null_ptr2(net_dev, param)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_vap = oal_net_dev_priv(net_dev);
    if (mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_get_station_info::OAL_NET_DEV_PRIV, return null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    vap_id = mac_vap->uc_vap_id;

    query_station_info = (dmac_query_station_info_request_event *)&dmac_query_request_event;
    query_station_info->query_event = OAL_QUERY_STATION_INFO_EVENT;

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(vap_id);
    if (hmac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }


    hmac_vap->station_info_query_completed_flag = OAL_FALSE;

    /********************************************************************************
        抛事件到wal层处理 ，对于低功耗需要做额外处理，不能像下层抛事件，直接起定时器
        低功耗会在接收beacon帧的时候主动上报信息。
    ********************************************************************************/
    /* 3.1 填写 msg 消息头 */
    write_msg.en_wid = WLAN_CFGID_QUERY_STATION_STATS;
    write_msg.us_len = OAL_SIZEOF(dmac_query_request_event);

    /* 3.2 填写 msg 消息体 */
    if (EOK != memcpy_s(write_msg.auc_value, OAL_SIZEOF(dmac_query_station_info_request_event),
                        query_station_info, OAL_SIZEOF(dmac_query_station_info_request_event))) {
        return OAL_FAIL;
    }

    ret = wal_send_cfg_event(net_dev, WAL_MSG_TYPE_WRITE,
            WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(dmac_query_station_info_request_event),
            (uint8_t *)&write_msg, OAL_FALSE, OAL_PTR_NULL);

    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(vap_id, 0, "{wal_hipriv_get_station_info::wal_send_cfg_event err code %d!}", ret);
        return OAL_FAIL;
    }
    /*lint -e730*/ /* info, boolean argument to function */
    leftime = oal_wait_event_interruptible_timeout_m(hmac_vap->query_wait_q,
        (OAL_TRUE == hmac_vap->station_info_query_completed_flag), 5 * OAL_TIME_HZ); /* 5s超时 */
    /*lint +e730*/
    if (leftime <= 0) {
        oam_warning_log0(vap_id, OAM_SF_ANY, "{wal_hipriv_get_station_info::query info wait for 5 s timeout!}");
        return OAL_FAIL;
    } else {
        /* 正常结束  */
        wal_hipriv_printf_station_info(hmac_vap);
        return OAL_SUCC;
    }
}
#endif
#ifdef _PRE_WLAN_FEATURE_NAN
OAL_STATIC uint32_t wal_hipriv_nan_set_param(oal_net_device_stru *net_device, int8_t *param)
{
    wal_msg_write_stru write_msg;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t offset;
    uint32_t ret;
    mac_nan_cfg_msg_stru *nan_msg;
    mac_nan_param_stru *nan_param;

    nan_msg = (mac_nan_cfg_msg_stru*)write_msg.auc_value;
    nan_msg->type = NAN_CFG_TYPE_SET_PARAM;
    nan_param = &nan_msg->param;

    ret = wal_get_cmd_one_arg(param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &offset);
    if (ret != OAL_SUCC) {
        return ret;
    }
    nan_param->duration = oal_atoi(ac_name);

    param += offset;
    ret = wal_get_cmd_one_arg(param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &offset);
    if (ret != OAL_SUCC) {
        return ret;
    }
    nan_param->period = oal_atoi(ac_name);

    param += offset;
    ret = wal_get_cmd_one_arg(param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &offset);
    if (ret != OAL_SUCC) {
        return ret;
    }
    nan_param->band = (uint8_t)oal_atoi(ac_name);

    param += offset;
    ret = wal_get_cmd_one_arg(param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &offset);
    if (ret != OAL_SUCC) {
        return ret;
    }
    nan_param->channel = (uint8_t)oal_atoi(ac_name);

    WAL_WRITE_MSG_HDR_INIT(&write_msg, WLAN_CFGID_NAN, OAL_SIZEOF(mac_nan_cfg_msg_stru));
    if (OAL_SUCC != wal_send_cfg_event(net_device, WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_nan_cfg_msg_stru), (uint8_t*)&write_msg, OAL_FALSE,
        OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfgvendor_nan_send_event::wal_send_cfg_event fail!}");
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_nan_tx_mgmt(oal_net_device_stru *net_device, int8_t *param)
{
    wal_msg_write_stru write_msg;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t offset;
    uint32_t ret;
    mac_nan_cfg_msg_stru *nan_msg;
    mac_nan_mgmt_info_stru *mgmt_info;
    wal_msg_stru *pst_rsp_msg = NULL;
    uint8_t ac_frame_body[WAL_HIPRIV_CMD_NAME_MAX_LEN >> 1] = { 0 };
    uint8_t str_frame_len;
    uint8_t frame_len = 0;
    nan_msg = (mac_nan_cfg_msg_stru*)write_msg.auc_value;
    nan_msg->type = NAN_CFG_TYPE_SET_TX_MGMT;
    mgmt_info = &nan_msg->mgmt_info;

    ret = wal_get_cmd_one_arg(param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &offset);
    if (ret != OAL_SUCC) {
        return ret;
    }
    mgmt_info->periodic = !(uint8_t)oal_atoi(ac_name);
    mgmt_info->action = WLAN_ACTION_NAN_FLLOWUP; /* 调试命令写死 */

    param += offset;
    ret = wal_get_cmd_one_arg(param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &offset);
    if (ret != OAL_SUCC) {
        return ret;
    }

    str_frame_len = (uint8_t)(OAL_STRLEN(ac_name));
    wal_hipriv_parase_send_frame_body(ac_name, str_frame_len, ac_frame_body,
        OAL_SIZEOF(ac_frame_body), &frame_len);
    mgmt_info->len = frame_len;
    mgmt_info->data = ac_frame_body;

    WAL_WRITE_MSG_HDR_INIT(&write_msg, WLAN_CFGID_NAN, OAL_SIZEOF(mac_nan_cfg_msg_stru));
    if (OAL_SUCC != wal_send_cfg_event(net_device, WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_nan_cfg_msg_stru), (uint8_t*)&write_msg, OAL_TRUE,
        &pst_rsp_msg)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_nan_tx_mgmt::wal_send_cfg_event fail!}");
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}
OAL_STATIC uint32_t wal_hipriv_nan_dbg(oal_net_device_stru *net_device, int8_t *param)
{
    uint32_t ret;
    uint32_t offset;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    if (oal_any_null_ptr2(net_device, param)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    ret = wal_get_cmd_one_arg(param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &offset);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_nan_dbg::first para return err_code [%d]!}", ret);
        return ret;
    }
    param += offset;
    if (oal_strcmp(ac_name, "set_param") == 0) {
        ret = wal_hipriv_nan_set_param(net_device, param);
    } else if (oal_strcmp(ac_name, "tx_mgmt") == 0) {
        ret = wal_hipriv_nan_tx_mgmt(net_device, param);
    } else {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_nan_dbg::not support the sub cmd!}");
    }
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_nan_dbg::execute cmd failed, error[%d]!}", ret);
    }
    return ret;
}
#endif
OAL_CONST wal_hipriv_cmd_entry_stru g_ast_hipriv_cmd_debug[] = {
    /***********************调试命令***********************/
    /* 设置TLV类型通用命令: hipriv "wlan0 set_tlv xxx 0 0"  */
    { "set_tlv", wal_hipriv_set_tlv_cmd },
    /* 设置Value类型通用命令: hipriv "wlan0 set_val xxx 0"  */
    { "set_val", wal_hipriv_set_val_cmd },
    /* 设置String类型通用命令: hipriv "wlan0 set_str 11ax_debug 3 tid 0 val 1 cnt 2"  */
    { "set_str", wal_hipriv_set_str_cmd },

#ifdef _PRE_WLAN_FEATURE_TWT
    { "twt_setup_req", _wal_hipriv_twt_setup_req },
    { "twt_teardown_req", _wal_hipriv_twt_teardown_req },
#endif

    { "destroy", wal_hipriv_del_vap }, /* 删除vap私有命令为: hipriv "vap0 destroy" */

    { "global_log_switch",  wal_hipriv_global_log_switch },   /* 全局日志开关:  hipriv "Hisilicon0 global_log_switch 0 | 1 */
    { "feature_log_switch", wal_hipriv_feature_log_switch }, /* 特性的INFO级别日志开关 hipriv "VAPX feature_name {0/1}" */
    { "pm_switch",  wal_hipriv_pm_switch },   /* log低功耗模式: hipriv "Hisilicon0 pm_switch 0 | 1"，log pm模式开关 */
    { "power_test", wal_hipriv_power_test }, /* log低功耗模式: hipriv "Hisilicon0 power_test 0 | 1"，功耗测试模式开 */
#ifdef _PRE_WLAN_NARROW_BAND
    { "narrow_bw", wal_hipriv_narrow_bw }, /* Start DPD Calibration */
#endif
    { "ota_beacon_on",    wal_hipriv_ota_beacon_switch },   /* 设置是否上报beacon帧开关: hipriv "Hisilicon0 ota_beacon_switch 0 | 1"，该命令针对所有的VAP */
    { "ota_switch",       wal_hipriv_ota_rx_dscr_switch },     /* 设置是否上报接收描述符帧开关: hipriv "Hisilicon0 ota_rx_dscr_switch 0 | 1"，该命令针对所有的VAP */
    { "addba_req",        wal_hipriv_addba_req },               /* 设置建立BA会话的配置命令:hipriv "vap0 addba_req xx xx xx xx xx xx(mac地址) tidno ba_policy buffsize timeout" 该命令针对某一个VAP */
    { "delba_req",        wal_hipriv_delba_req },               /* 设置删除BA会话的配置命令: hipriv "vap0 delba_req xx xx xx xx xx xx(mac地址) tidno direction" 该命令针对某一个VAP */
#ifdef _PRE_WLAN_FEATURE_WMMAC
    { "addts_req",    wal_hipriv_addts_req },       /* 设置建立TS，即发送ADDTS REQ的配置命令:hipriv "vap0 addts_req tid direction apsd up nominal_msdu_size max_msdu_size                                                                       minimum_data_rate mean_data_rate peak_data_rate minimum_phy_rate surplus_bandwidth_allowance" 该命令针对某一个VAP */
    { "delts",        wal_hipriv_delts },               /* 设置删除TS，即发送DELTS的配置命令: hipriv "vap0 tidno" 该命令针对某一个VAP */
    { "wmmac_switch", wal_hipriv_wmmac_switch }, /* 设置WMMAC开关，配置命令: hipriv "vap0 wmmac_switch 1/0(使能) 0|1(WMM_AC认证使能) xxx(timeout_period) factor" 整个Device */
#endif
    { "meminfo", wal_hipriv_mem_info }, /* 打印内存池信息: hipriv "Hisilicon0 meminfo poolid" */
    { "memleak", wal_hipriv_mem_leak }, /* 打印内存池信息: hipriv "Hisilicon0 memleak poolid" */
    { "memoryinfo", wal_hipriv_memory_info },        /* 打印内存池信息: hipriv "Hisilicon0 memoryinfo host/device" */
    { "beacon_chain_switch", wal_hipriv_beacon_chain_switch }, /* 设置beacon帧发送策略配置命令: hipriv "vap0 beacon_chain_switch 0/1" 目前采取的是单通道模式(使用通道0)，0表示关闭双路轮流发送，1表示开启，该命令针对某一个VAP */

    { "coex_preempt_type",   wal_hipriv_btcoex_preempt_type },     /* 配置preempt_type，sh hipriv.sh "vap0 coex_preempt_type  0/1(硬件或者软件) 0-3/0-1"  0 noframe 1 self-cts 2 nulldata 3 qosnull  软件ps打开或者关闭 2 slot提前量 */
    { "set_coex_perf_param", wal_hipriv_btcoex_set_perf_param }, /* 配置BA删建门限，sh hipriv.sh "vap0 set_coex_perf_param  0/1/2/3" */
#ifdef _PRE_WLAN_FEATURE_NRCOEX
    { "nrcoex_set_prior", wal_hipriv_nrcoex_set_prior }, /* 配置NR共存参数，sh hipriv.sh "nrcoex_set_prior 0/1/2/3" */
    { "nrcoex_test", wal_hipriv_nrcoex_cfg_test },       /* 配置NR共存参数，sh hipriv.sh "nrcoex_test 0~20" */
#endif

    { "protocol_debug", wal_hipriv_set_protocol_debug_info }, /* 设置打印phy维测的相关信息，sh hipriv.sh "wlan0 protocol_debug [band_force_switch 0|1|2] [2040_ch_swt_prohi 0|1] [40_intol 0|1]" */

    { "set_tx_pow", wal_hipriv_set_tx_pow_param }, /* 配置发送功率参数: hipriv "vap0 set_tx_pow <param name> <value>" */

    { "set_ucast_data",  wal_hipriv_set_ucast_data_dscr_param },   /* 打印描述符信息: hipriv "vap0 set_ucast_data <param name> <value>" */
    { "set_bcast_data",  wal_hipriv_set_bcast_data_dscr_param },   /* 打印描述符信息: hipriv "vap0 set_bcast_data <param name> <value>" */
    { "set_ucast_mgmt",  wal_hipriv_set_ucast_mgmt_dscr_param },   /* 打印描述符信息: hipriv "vap0 set_ucast_mgmt <param name> <value>" */
    { "set_mbcast_mgmt", wal_hipriv_set_mbcast_mgmt_dscr_param }, /* 打印描述符信息: hipriv "vap0 set_mbcast_mgmt <param name> <value>" */
    { "nss",           wal_hipriv_set_nss },                 /* 设置HT模式下的空间流个数:   hipriv "vap0 nss   <value>" */
    { "txch",          wal_hipriv_set_rfch },               /* 设置发射通道:               hipriv "vap0 rfch  <value>" */

    { "list_sta",     wal_hipriv_list_sta },           /* 打印AP关联的STA列表: hipriv "sta0 list_sta" */
    { "start_scan",   wal_hipriv_start_scan },       /* 触发sta扫描: hipriv "sta0 start_scan" */
    { "start_join",   wal_hipriv_start_join },       /* 触发sta加入并认证关联: hipriv "sta0 start_join 1" 1表示扫描到的AP在device写数组下标号 */
    { "start_deauth", wal_hipriv_start_deauth },   /* 触发sta去认证: hipriv "vap0 start_deauth" */
    { "kick_user",    wal_hipriv_kick_user },         /* 删除1个用户 hipriv "vap0 kick_user xx xx xx xx xx xx(mac地址)" */

    { "ampdu_tx_on",  wal_hipriv_ampdu_tx_on },     /* 开启或关闭ampdu发送功能 hipriv "vap0 ampdu_tx_on 0\1" */
    { "amsdu_tx_on",  wal_hipriv_amsdu_tx_on },     /* 开启或关闭ampdu发送功能 hipriv "vap0 amsdu_tx_on 0\1" */
    { "send_bar",     wal_hipriv_send_bar },           /* 指定tid发送bar hipriv "vap0 send_bar A6C758662817(mac地址) tid_num" */

    { "wifi_stat_info",       wal_hipriv_show_stat_info },              /* 获取所有维测统计信息: hipriv "Hisilicon0 wifi_stat_info" */
    { "clear_stat_info",      wal_hipriv_clear_stat_info },            /* 清零所有维测统计信息: hipriv "Hisilicon0 clear_stat_info" */
    { "usr_stat_info",        wal_hipriv_user_stat_info },               /* 上报某个user下的维测统计信息: sh hipriv.sh "Hisilicon0 usr_stat_info usr_id" */
    { "reset_hw",             wal_hipriv_reset_device },                      /* 复位硬件phy&mac: hipriv "Hisilicon0 reset_hw 0|1|2|3|4|5|6|8|9|10|11
                                                                                                                    (all|phy|mac|debug|mac_tsf|mac_cripto|mac_non_cripto|phy_AGC|phy_HT_optional|phy_VHT_optional|phy_dadar )
                                                                                                                    0|1(reset phy reg) 0|1(reset mac reg) */
    { "reset_operate",       wal_hipriv_reset_operate },                /* 复位硬件phy&mac: hipriv "Hisilicon0 reset_hw 0|1|2|3(all|phy|mac|debug) 0|1(reset phy reg) 0|1(reset mac reg) */
    { "list_channel",        wal_hipriv_list_channel },                  /* 支持信道列表， hipriv "Hisilicon0 list_channel" */
    { "event_queue",         wal_hipriv_event_queue_info },               /* 打印事件队列信息，将打印出每一个非空事件队列中事件的个数，以及每一个事件头信息, hipriv "Hisilicon0 event_queue" */
    { "frag_threshold",      wal_hipriv_frag_threshold },              /* 设置分片门限的配置命令: hipriv "vap0 frag_threshold (len)" 该命令针对某一个VAP */
    { "wmm_switch",          wal_hipriv_wmm_switch },                      /* 动态开启或者关闭wmm hipriv "vap0 wmm_switch 0|1"(0不使能，1使能)  */
    { "ether_switch",        wal_hipriv_set_ether_switch },              /* 设置以太网帧上报的开关，sh hipriv.sh "vap0 ether_switch user_macaddr oam_ota_frame_direction_type_enum(帧方向) 0|1(开关)" */
    { "80211_uc_switch",     wal_hipriv_set_80211_ucast_switch },     /* 设置80211单播帧上报的开关，sh hipriv.sh "vap0 80211_uc_switch user_macaddr 0|1(帧方向tx|rx) 0|1(帧类型:管理帧|数据帧) 0|1(帧内容开关) 0|1(CB开关) 0|1(描述符开关)" */
    { "80211_mc_switch",     wal_hipriv_set_80211_mcast_switch },     /* 设置80211组播\广播帧上报的开关，sh hipriv.sh "Hisilicon0 80211_mc_switch 0|1(帧方向tx|rx) 0|1(帧类型:管理帧|数据帧) 0|1(帧内容开关) 0|1(CB开关) 0|1(描述符开关)" */
    { "probe_switch",        wal_hipriv_set_probe_switch },              /* 设置probe req与rsp上报的开关，sh hipriv.sh "Hisilicon0 probe_switch 0|1(帧方向tx|rx) 0|1(帧内容开关) 0|1(CB开关) 0|1(描述符开关)" */
    { "phy_debug",           wal_hipriv_set_phy_debug_switch },             /* 设置打印phy维测的相关信息，sh hipriv.sh "Hisilicon0 phy_debug [snr 0|1] [rssi 0|1] [trlr 0|1] [count N]" */
    { "report_vap_info", wal_hipriv_report_vap_info }, /* 根据标记位上报vap的对应信息 sh hipriv.sh "wlan0 report_vap_info 1" */
    { "set_all_ota",     wal_hipriv_set_all_ota },             /* 设置所有ota上报，如果为1，则所有类型帧的cb描述符都报，如果为0，什么都不报，sh hipriv.sh "Hisilicon0 set_all_ota 0|1" */
    { "80211_uc_all",    wal_hipriv_set_all_80211_ucast },    /* 设置所有用户的单播开关，sh hipriv.sh "Hisilicon0 80211_uc_all 0|1(帧方向tx|rx) 0|1(帧类型:管理帧|数据帧) 0|1(帧内容开关) 0|1(CB开关) 0|1(描述符开关)" */
    { "ether_all",       wal_hipriv_set_all_ether_switch },      /* 设置所有用户的以太网开关，sh hipriv.sh "Hisilicon0 ether_all 0|1(帧方向tx|rx) 0|1(开关)" */
    { "dhcp_arp_switch", wal_hipriv_set_dhcp_arp_switch }, /* 设置发送广播arp和dhcp开关，sh hipriv.sh "Hisilicon0 dhcp_arp_switch 0|1(开关)" */
#ifdef _PRE_WLAN_DFT_STAT
    { "clear_vap_stat_info", wal_hipriv_clear_vap_stat_info }, /* 清零指定VAP的统计信息: hipriv "vap_name clear_vap_stat_info" */
#endif
#ifdef _PRE_WLAN_FEATURE_TXOPPS
    { "txopps_hw_en", wal_hipriv_set_txop_ps_machw }, /* 设置mac txop ps使能寄存器，sh hipriv.sh "stavap_name txopps_hw_en 0|1(txop_ps_en) 0|1(condition1) 0|1(condition2)" */
#endif
#ifdef _PRE_WLAN_DFT_STAT
    { "usr_queue_stat", wal_hipriv_usr_queue_stat },  /* 上报或者清零用户队列统计信息: sh hipriv.sh "vap_name usr_queue_stat XX:XX:XX:XX:XX:XX 0|1" */
    { "vap_stat",       wal_hipriv_report_vap_stat },       /* 上报或者停止上报vap吞吐统计信息: sh hipriv.sh "vap_name vap _stat  0|1" */
    { "reprt_all_stat", wal_hipriv_report_all_stat }, /* 上报或者清零所有维测统计信息: sh hipriv.sh "Hisilicon0 reprt_all_stat type(phy/machw/mgmt/irq/all)  0|1" */
#endif

    { "ampdu_aggr_num", wal_hipriv_set_ampdu_aggr_num }, /* 设置AMPDU聚合个数: sh hipriv.sh "Hisilicon0 ampdu_aggr_num aggr_num_switch aggr_num" ,aggr_num_switch非0时，aggr_num有效 */
    { "amsdu_aggr_num", wal_hipriv_set_amsdu_aggr_num }, /* 设置AMSDU聚合个数: sh hipriv.sh "wlan0 amsdu_aggr_num num" */

    { "set_stbc_cap", wal_hipriv_set_stbc_cap }, /* 设置STBC能力 */
    { "set_ldpc_cap", wal_hipriv_set_ldpc_cap }, /* 设置LDPC能力 */
    { "set_txbf_cap", wal_hipriv_set_txbf_cap }, /* 开启或关闭txbf的 接收bit0/发送bit1 能力 hipriv "vap0 alg_txbf_switch 0|1|2|3" */
    { "set_psm_para", wal_hipriv_sta_psm_param },  /* sh hipriv.sh 'wlan0 set_psm_para 100 40 */
    { "set_sta_pm_on", wal_hipriv_sta_pm_on }, /* sh hipriv.sh 'wlan0 set_sta_pm_on xx xx xx xx */
    { "send_frame", wal_hipriv_send_frame }, /* 指定tid发送bar hipriv "vap0 send_frame (type) (num) (目的mac)" */
#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    { "enable_pmf", wal_hipriv_enable_pmf }, /* 设置chip test中强制使能pmf能力 (用于关联之后)sh hipriv.sh "vap0 enable_pmf 0|1|2(0不使能，1 enable, 2强制)  */
#endif
    { "set_default_key", wal_hipriv_set_default_key }, /* 设置chip test中强制使能pmf能力 (用于关联之后)sh hipriv.sh "vap0 set_default_key x(key_index) 0|1(en_unicast) 0|1(multicast)"  */
    { "add_key", wal_hipriv_test_add_key },            /* chip test配置add key操作的私有配置命令接口
                          sh hipriv.sh "xxx(cipher) xx(en_pairwise) xx(key_len) xxx(key_index) xxxx:xx:xx:xx:xx:xx...(key 小于32字节) xx:xx:xx:xx:xx:xx(目的地址)  */

#ifdef _PRE_WLAN_FEATURE_DFR
    { "dfr_start", wal_hipriv_test_dfr_start }, /* dfr功能打桩触发接口sh hipriv.sh "vap0 dfr_start 0(dfr子功能:0-device异常复位 )" */
#endif                                          // _PRE_WLAN_FEATURE_DFR
    /* 算法相关的命令 */
    { "alg_ar_log",  wal_hipriv_ar_log },   /* autorate算法日志参数配置: */
    { "alg_ar_test", wal_hipriv_ar_test }, /* autorate算法系统测试命令 */
    { "alg",         wal_hipriv_alg },             /* alg */

    { "alg_txbf_log", wal_hipriv_txbf_log },         /* txbf算法日志参数配置: */
    { "alg_intf_det_log", wal_hipriv_intf_det_log }, /* 干扰检测算法日志参数配置: */
    { "bw_fixed", wal_hipriv_set_bw_fixed },    /* 设置ap模式的vap带宽固定 */

    { "get_hipkt_stat",    wal_hipriv_get_hipkt_stat },       /* 获取高优先级报文的统计情况 */
    { "set_flowctl_param", wal_hipriv_set_flowctl_param }, /* 设置流控相关参数 */
    { "get_flowctl_stat",  wal_hipriv_get_flowctl_stat },   /* 获取流控相关状态信息 */

    { "auto_protection", wal_hipriv_set_auto_protection }, /* 设置自动保护开关 */

    /* 共存维测相关 */
    { "send_2040_coext", wal_hipriv_send_2040_coext }, /* 发送20/40共存管理帧: hipriv "Hisilicon0 send_2040_coext coext_info chan_report" */
    { "get_version",     wal_hipriv_get_version },         /* 获取软件版本: hipriv "vap0 get_version" */

#ifdef _PRE_WLAN_FEATURE_FTM
    /* hipriv.sh "wlan0 ftm_debug ---------------------------------------------------------------------------------------------------------*/
    /* -------------------------- enable_ftm_initiator [0|1] */
    /* -------------------------- send_initial_ftm_request channel[] burst[0|1] ftms_per_burst[n] \
                                  en_lci_civic_request[0|1] asap[0|1] bssid[xx:xx:xx:xx:xx:xx] */
    /* -------------------------- enable [0|1] */
    /* -------------------------- cali [0|1] */
    /* -------------------------- send_ftm bssid[xx:xx:xx:xx:xx:xx] */
    /* -------------------------- set_ftm_time t1[] t2[] t3[] t4[] */
    /* -------------------------- enable_ftm_responder [0|1] */
    /* -------------------------- send_range_req mac[] num_rpt[] delay[] ap_cnt[] bssid[] channel[] \
                                  bssid[] channel[] ... */
    /* -------------------------- enable_ftm_range [0|1] */
    /* -------------------------- get_cali */
    /* -------------------------- set_location type[] mac[] mac[] mac[] */
    /* -------------------------- set_ftm_m2s en_is_mimo[] uc_tx_chain_selection[] */
    { "ftm_debug", _wal_hipriv_ftm },
#endif

#ifdef _PRE_WLAN_FEATURE_PSD_ANALYSIS
    { "set_psd", wal_hipriv_set_psd }, /* 使能PSD采集: hipriv "Hisilicon0 set_psd 0/1"，使能之前先config  */
    { "cfg_psd", wal_hipriv_cfg_psd }, /* 使能PSD采集: hipriv "Hisilicon0 cfg_psd" */
#endif
#ifdef _PRE_WLAN_FEATURE_CSI
    { "set_csi", wal_hipriv_set_csi }, /* 使能CSI上报: hipriv "Hisilicon0 set_csi xx xx xx xx xx xx(mac地址) ta_check csi_en" */
#endif

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
    { "get_user_nssbw", wal_hipriv_get_user_nssbw },       /* 设置添加用户的配置命令: hipriv "vap0 get_user_nssbw xx xx xx xx xx xx(mac地址) "  该命令针对某一个VAP */
#endif

#ifdef _PRE_WLAN_FEATURE_M2S
    { "set_m2s_switch", wal_hipriv_set_m2s_switch }, /* mimo和siso切换: hipriv "Hisilicon0 set_m2s_switch  0/1/2/3/4/5...(参数查询/配置模式/resv/软切换或硬切换测试模式) */
#endif

    { "blacklist_add",  wal_hipriv_blacklist_add },       /* 1 */
    { "blacklist_del",  wal_hipriv_blacklist_del },       /* 2 */
    { "blacklist_mode", wal_hipriv_set_blacklist_mode }, /* 3 */
    { "blacklist_show", wal_hipriv_blacklist_show },     /* 4 wal_config_blacklist_show */

    { "vap_classify_en", wal_hipriv_vap_classify_en },   /* device级别配置命令 设置基于vap的业务分类是否使能 hipriv "Hisilicon0 vap_classify_en 0/1" */
    { "vap_classify_tid", wal_hipriv_vap_classify_tid }, /* 设置vap的流等级 hipriv "vap0 classify_tid 0~7" */

#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG
    /* 打印RR各点的时间戳，sh hipriv.sh "Hisilicon0 show_rr_timeinfo 0|1" */
    { "show_rr_timeinfo", wal_hipriv_show_rr_time_info },
#endif

    { "txpower", wal_hipriv_set_txpower }, /* 设置最大发送功率，要求功率值按照扩大10倍来输入，例如最大功率要限制为20，输入200 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
    { "essid",   wal_hipriv_set_essid },        /* 设置AP ssid */
    { "bintval", wal_ioctl_set_beacon_interval }, /* 设置AP beacon 周期 */
    { "up",      wal_hipriv_start_vap },
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,44)) */
    { "show_arpoffload_info", wal_hipriv_show_arpoffload_info }, /* 显示Device侧记录的IP地址:sh hipriv.sh "wlan0 show_ip_addr" */
#ifdef _PRE_WLAN_TCP_OPT
    /* 显示TCP ACK 过滤统计值 sh hipriv.sh "vap0 get_tx_ack_stream_info */
    { "get_tcp_ack_stream_info", wal_hipriv_get_tcp_ack_stream_info },
    /* 设置发送TCP ACK优化使能  sh hipriv.sh "vap0 tcp_tx_ack_opt_enable 0 | 1 */
    { "tcp_tx_ack_opt_enable",   wal_hipriv_tcp_tx_ack_opt_enable },
    /* 设置接收TCP ACK优化使能 sh hipriv.sh "vap0 tcp_rx_ack_opt_enable 0 | 1 */
    { "tcp_rx_ack_opt_enable",   wal_hipriv_tcp_rx_ack_opt_enable },
    /* 设置发送TCP ACK LIMIT sh hipriv.sh "vap0 tcp_tx_ack_opt_limit X */
    { "tcp_tx_ack_opt_limit",    wal_hipriv_tcp_tx_ack_limit },
    /* 设置接收TCP ACKLIMIT  sh hipriv.sh "vap0 tcp_tx_ack_opt_limit X */
    { "tcp_rx_ack_opt_limit",    wal_hipriv_tcp_rx_ack_limit },

#endif

#ifdef _PRE_WLAN_FEATURE_WAPI
#ifdef _PRE_WAPI_DEBUG
    { "wapi_info", wal_hipriv_show_wapi_info }, /* wapi hipriv "vap0 wal_hipriv_show_wapi_info " */
#endif
#endif                                          /* #ifdef _PRE_WLAN_FEATURE_WAPI */

    { "roam_enable",      wal_hipriv_roam_enable },               /* 设置漫游开关 */
    { "roam_start",       wal_hipriv_roam_start }, /* 漫游测试命令 sh hipriv.sh "wlan0 roam_start 0|1|2|3|4 0/1"
                                                            * 0或者参数缺失表示漫游前不扫描, 1|2|3|4表示扫描+漫游
                                                            */
    /* 第二个参数0或者参数缺失表示漫游到其它AP, 1表示漫游到自己 */
    { "2040bss_enable", wal_hipriv_enable_2040bss }, /* 设置20/40 bss使能: hipriv "Hisilicon0 2040bss_enable 0|1" 0表示20/40 bss判断关闭，1表示使能 */

#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
    { "dyn_cali", wal_hipriv_dyn_cali_cfg }, /*  动态校准参数配置 sh hipriv "wlan0 dyn_cali   " */
#endif

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    { "custom_info", wal_hipriv_dev_customize_info }, /* 打印定制化信息 */
#endif

#ifdef _PRE_WLAN_ONLINE_DPD
#ifdef _PRE_WLAN_RF_CALI_DEBUG
    { "dpd_cfg", wal_hipriv_dpd_cfg },     /* 打印定制化信息 */
#endif
#endif
#if defined(_PRE_WLAN_FEATURE_11V_ENABLE)
    { "11v_cfg_bsst", wal_hipriv_11v_cfg_bsst }, /* 11v bss transition特性配置 sh hipriv.sh "vap0 11v_cfg_bsst 0|1"
                                                                        0:关闭11V特性；1:打开11V特性 */
    { "11v_tx_query", wal_hipriv_11v_tx_query }, /* 11v特性配置:  触发sta发送11v Query帧: hipriv "vap[x] 11v_tx_query [mac-addr]"   */
#endif

    { "dbdc_debug", wal_hipriv_set_dbdc_debug_switch }, /* DBDC特性开关，sh hipriv.sh "wlan0 dbdc_debug [change_hal_dev 0|1] " */

    { "pm_debug", wal_hipriv_set_pm_debug_switch }, /* 低功耗debug命令 */

    { "set_owe", wal_hipriv_set_owe }, /* 设置APUT模式支持的OWE group: hipriv "wlan0 set_owe <value>" */
    /* bit 0/1/2分别表示支持owe group 19/20/21 */
    { "fem_lowpower", wal_hipriv_fem_lowpower }, /* fem低功耗命令: sh hipriv.sh "wlan0 fem_lowpower 0|1 |2" */

    { "radar_set", wal_hipriv_radar_set }, /* 设置雷达事件 */

    { "icmp_filter", wal_hipriv_set_icmp_filter }, /* icmp过滤命令: sh hipriv.sh "wlan0 icmp_filter 1 0|1" */

    { "psm_flt_stat", wal_hipriv_psm_flt_stat }, /* 打印低功耗帧过滤统计 sh hipriv.sh "wlan0 psm_flt_stat [0|1|2|...]" */

#ifdef _PRE_WLAN_FEATURE_HIEX
    /* 设置某个用户hiex能力，sh hipriv.sh "wlan0 set_user_himit xx:xx:xx:xx:xx:xx(mac地址) 0xXXX" */
    { "set_user_hiex", _wal_hipriv_set_user_hiex_enable },
#endif
#ifdef _PRE_WLAN_FEATURE_GET_STATION_INFO_EXT
    { "get_station_info", wal_hipriv_get_station_info },
#endif
#ifdef _PRE_WLAN_FEATURE_NAN
    { "nan_dbg", wal_hipriv_nan_dbg},
#endif
};

uint32_t wal_hipriv_get_debug_cmd_size(void)
{
    return oal_array_size(g_ast_hipriv_cmd_debug);
}

#endif
