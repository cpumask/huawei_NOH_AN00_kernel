

#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "hmac_vap.h"
#include "hmac_ext_if.h"
#include "securec.h"
#include "securectype.h"
#include "hmac_vowifi.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_VOWIFI_C


void mac_vap_vowifi_init(mac_vap_stru *pst_mac_vap)
{
    if (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA) {
        return;
    }
    if (pst_mac_vap->pst_vowifi_cfg_param == OAL_PTR_NULL) {
        pst_mac_vap->pst_vowifi_cfg_param = oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL,
                                                            OAL_SIZEOF(mac_vowifi_param_stru),
                                                            OAL_TRUE);
        if (pst_mac_vap->pst_vowifi_cfg_param == OAL_PTR_NULL) {
            oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_VOWIFI,
                           "{mac_vap_vowifi_init::pst_vowifi_cfg_param alloc null, size[%d].}",
                           OAL_SIZEOF(mac_vowifi_param_stru));
            return;
        }
    }
    memset_s(pst_mac_vap->pst_vowifi_cfg_param,
             OAL_SIZEOF(mac_vowifi_param_stru), 0,
             OAL_SIZEOF(mac_vowifi_param_stru));
    pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_mode         = MAC_VAP_VOWIFI_MODE_DEFAULT;
    pst_mac_vap->pst_vowifi_cfg_param->uc_trigger_count_thres = MAC_VAP_VOWIFI_TRIGGER_COUNT_DEFAULT;
    pst_mac_vap->pst_vowifi_cfg_param->us_rssi_period_ms      = MAC_VAP_VOWIFI_PERIOD_DEFAULT_MS;
    pst_mac_vap->pst_vowifi_cfg_param->c_rssi_high_thres      = MAC_VAP_VOWIFI_HIGH_THRES_DEFAULT;
    pst_mac_vap->pst_vowifi_cfg_param->c_rssi_low_thres       = MAC_VAP_VOWIFI_LOW_THRES_DEFAULT;
}

void mac_vap_init_vowifi(mac_vap_stru *pst_vap, mac_cfg_add_vap_param_stru *pst_param)
{
    if (pst_param->en_p2p_mode == WLAN_LEGACY_VAP_MODE) {
        mac_vap_vowifi_init(pst_vap);
        if (pst_vap->pst_vowifi_cfg_param != OAL_PTR_NULL) {
            pst_vap->pst_vowifi_cfg_param->en_vowifi_mode = VOWIFI_MODE_BUTT;
        }
    }
}


void mac_vap_vowifi_exit(mac_vap_stru *pst_mac_vap)
{
    mac_vowifi_param_stru *pst_vowifi_cfg_param = NULL;

    if (pst_mac_vap->pst_vowifi_cfg_param == OAL_PTR_NULL) {
        return;
    }

    pst_vowifi_cfg_param = pst_mac_vap->pst_vowifi_cfg_param;

    /* 先置空再释放 */
    pst_mac_vap->pst_vowifi_cfg_param = OAL_PTR_NULL;
    oal_mem_free_m(pst_vowifi_cfg_param, OAL_TRUE);
}


uint32_t mac_vap_set_vowifi_param(mac_vap_stru *pst_mac_vap,
                                  mac_vowifi_cmd_enum_uint8 en_vowifi_cfg_cmd,
                                  uint8_t uc_value)
{
    int8_t c_value;

    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_VOWIFI, "{mac_vap_set_vowifi_param::pst_mac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    switch (en_vowifi_cfg_cmd) {
        case VOWIFI_SET_MODE: {
            /* 异常值 */
            if (uc_value >= VOWIFI_MODE_BUTT) {
                oam_error_log1(0, OAM_SF_VOWIFI, "{mac_vap_set_vowifi_param::MODE Value[%d] error!}", uc_value);
                return OAL_ERR_CODE_VOWIFI_SET_INVALID;
            }

            /*
             * MODE
             * 0: disable report of rssi change
             * 1: enable report when rssi lower than threshold(vowifi_low_thres)
             * 2: enable report when rssi higher than threshold(vowifi_high_thres)
             */
            pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_mode = uc_value;
            pst_mac_vap->pst_vowifi_cfg_param->uc_cfg_cmd_cnt |= BIT0;

            oam_warning_log1(0, OAM_SF_VOWIFI, "{mac_vap_set_vowifi_param::Set vowifi_mode=[%d]!}",
                             pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_mode);
            break;
        }
        case VOWIFI_SET_PERIOD: {
            /* 异常值 */
            if ((uc_value < MAC_VOWIFI_PERIOD_MIN) || (uc_value > MAC_VOWIFI_PERIOD_MAX)) {
                oam_error_log1(0, OAM_SF_VOWIFI, "{mac_vap_set_vowifi_param::PERIOD Value[%d] error!}", uc_value);
                return OAL_ERR_CODE_VOWIFI_SET_INVALID;
            }

            /* 单位ms,范围【1s，30s】, the period of monitor the RSSI when host suspended */
            pst_mac_vap->pst_vowifi_cfg_param->us_rssi_period_ms = (uint16_t)(uc_value * 1000);
            pst_mac_vap->pst_vowifi_cfg_param->uc_cfg_cmd_cnt |= BIT1;

            break;
        }
        case VOWIFI_SET_LOW_THRESHOLD: {
            c_value = (int8_t)uc_value;
            /* 异常值 */
            if ((c_value < MAC_VOWIFI_LOW_THRESHOLD_MIN) || (c_value > MAC_VOWIFI_LOW_THRESHOLD_MAX)) {
                oam_error_log1(0, OAM_SF_VOWIFI,
                               "{mac_vap_set_vowifi_param::LOW_THRESHOLD Value[%d] error!}", c_value);
                return OAL_ERR_CODE_VOWIFI_SET_INVALID;
            }

            /* [-1, -100],vowifi_low_thres */
            pst_mac_vap->pst_vowifi_cfg_param->c_rssi_low_thres = c_value;
            pst_mac_vap->pst_vowifi_cfg_param->uc_cfg_cmd_cnt |= BIT2;

            break;
        }
        case VOWIFI_SET_HIGH_THRESHOLD: {
            c_value = (int8_t)uc_value;
            /* 异常值 */
            if ((c_value < MAC_VOWIFI_HIGH_THRESHOLD_MIN) || (c_value > MAC_VOWIFI_HIGH_THRESHOLD_MAX)) {
                oam_error_log1(0, OAM_SF_VOWIFI,
                               "{mac_vap_set_vowifi_param::HIGH_THRESHOLD Value[%d] error!}", c_value);
                return OAL_ERR_CODE_VOWIFI_SET_INVALID;
            }

            /* [-1, -100],vowifi_high_thres */
            pst_mac_vap->pst_vowifi_cfg_param->c_rssi_high_thres = c_value;
            pst_mac_vap->pst_vowifi_cfg_param->uc_cfg_cmd_cnt |= BIT3;

            break;
        }
        case VOWIFI_SET_TRIGGER_COUNT: {
            /* 异常值 */
            if ((uc_value < MAC_VOWIFI_TRIGGER_COUNT_MIN) || (uc_value > MAC_VOWIFI_TRIGGER_COUNT_MAX)) {
                oam_error_log1(0, OAM_SF_VOWIFI,
                               "{mac_vap_set_vowifi_param::TRIGGER_COUNT Value[%d] error!}", uc_value);
                return OAL_ERR_CODE_VOWIFI_SET_INVALID;
            }

            /* 【1，100】, the continuous counters of lower or higher than threshold
             * which will trigger the report to host
             */
            pst_mac_vap->pst_vowifi_cfg_param->uc_trigger_count_thres = uc_value;
            pst_mac_vap->pst_vowifi_cfg_param->uc_cfg_cmd_cnt |= BIT4;

            break;
        }
        default:
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                             "{mac_vap_set_vowifi_param::invalid cmd = %d!!}",
                             en_vowifi_cfg_cmd);

            break;
    }

    /* 配置命令收集完毕，初始化vowifi相关上报状态 */
    if (BIT0 & pst_mac_vap->pst_vowifi_cfg_param->uc_cfg_cmd_cnt) {
        pst_mac_vap->pst_vowifi_cfg_param->uc_cfg_cmd_cnt = 0;
        pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_reported = OAL_FALSE;
    }

    return OAL_SUCC;
}


uint32_t hmac_config_vowifi_info(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;
    mac_cfg_vowifi_stru *pst_cfg_vowifi = NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_param))) {
        oam_error_log2(0, OAM_SF_CFG, "{hmac_config_vowifi_info::null param,pst_mac_vap=%d puc_param=%d.}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_mac_vap->pst_vowifi_cfg_param == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_config_vowifi_info::pst_vowifi_cfg_param is null.}");
        return OAL_SUCC;
    }

    pst_cfg_vowifi = (mac_cfg_vowifi_stru *)puc_param;

    ul_ret = mac_vap_set_vowifi_param(pst_mac_vap, pst_cfg_vowifi->en_vowifi_cfg_cmd, pst_cfg_vowifi->uc_value);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_config_vowifi_info::param[%d] set failed[%d].}",
                         pst_cfg_vowifi->en_vowifi_cfg_cmd, ul_ret);
        return ul_ret;
    }

    oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_VOWIFI,
        "{hmac_config_vowifi_info::Mode[%d],rssi_thres[%d],period_ms[%d],trigger_count[%d].}",
        pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_mode,
        ((pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_mode == VOWIFI_LOW_THRES_REPORT) ? pst_mac_vap->pst_vowifi_cfg_param->c_rssi_low_thres : pst_mac_vap->pst_vowifi_cfg_param->c_rssi_high_thres),
        pst_mac_vap->pst_vowifi_cfg_param->us_rssi_period_ms,
        pst_mac_vap->pst_vowifi_cfg_param->uc_trigger_count_thres);

    /***************************************************************************
        抛事件到DMAC层, 同步DMAC数据
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_VOWIFI_INFO, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{hmac_config_user_info::hmac_config_vowifi_info failed[%d].}", ul_ret);
    }

    return ul_ret;
}


uint32_t hmac_config_set_vowifi_nat_keep_alive_params(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;

    if (OAL_FALSE == IS_LEGACY_STA(pst_mac_vap)) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_config_set_vowifi_nat_keep_alive_params::vap is not legacy sta.}");
        return OAL_FAIL;
    }

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_config_set_vowifi_nat_keep_alive_params::pst_mac_device is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /***************************************************************************
        抛事件到DMAC层, 同步DMAC数据
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_SET_VOWIFI_KEEP_ALIVE, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{hmac_config_set_vowifi_nat_keep_alive_params::hmac_config_send_event failed[%d].}", ul_ret);
    }

    return ul_ret;
}


uint32_t hmac_config_vowifi_report(mac_vap_stru *pst_mac_vap, uint8_t uc_len, uint8_t *puc_param)
{
    frw_event_mem_stru *pst_event_mem = NULL;
    frw_event_stru *pst_event = NULL;

    /* 目前仅Legacy sta支持这种操作 */
    if (pst_mac_vap->pst_vowifi_cfg_param == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_config_vowifi_report::pst_vowifi_cfg_param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 设备up，切使能了vowifi状态才能触发切换vowifi状态 */
    if (pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_mode == VOWIFI_DISABLE_REPORT) {
        return OAL_SUCC;
    }

    /* "申请vowifi逻辑切换"仅上报一次直到重新更新vowifi模式 */
    if (pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_reported == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_config_vowifi_report::vowifi been reported once!}");
        return OAL_SUCC;
    }

    pst_event_mem = frw_event_alloc_m(0);
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_config_vowifi_report::frw_event_alloc_m fail,size=0!}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }
    pst_event = frw_get_event_stru(pst_event_mem);
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_VOWIFI_REPORT,
                       0,
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    /* 分发事件 */
    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);
    pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_reported = OAL_TRUE;
    return OAL_SUCC;
}