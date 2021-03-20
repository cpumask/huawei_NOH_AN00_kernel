

#ifdef __cplusplus // windows ut���벻����������������
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ͷ�ļ����� */
#include "mac_device.h"
#include "mac_resource.h"
#include "hmac_vap.h"
#include "oal_cfg80211.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RESET_C

/* 2 ȫ�ֱ������� */
extern uint32_t hmac_config_send_event(mac_vap_stru *pst_mac_vap,
                                             wlan_cfgid_enum_uint16 en_cfg_id,
                                             uint16_t us_len,
                                             uint8_t *puc_param);

/* 3 ����ʵ�� */

uint32_t hmac_reset_sys_event(mac_vap_stru *pst_mac_vap, uint8_t uc_len, uint8_t *puc_param)
{
    mac_device_stru *pst_mac_dev;
    mac_reset_sys_stru *pst_reset_sys;

    pst_reset_sys = (mac_reset_sys_stru *)puc_param;

    pst_mac_dev = mac_res_get_dev(pst_mac_vap->uc_device_id);

    if (pst_mac_dev == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_reset_sys_event::pst_mac_dev null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    switch (pst_reset_sys->en_reset_sys_type) {
        case MAC_RESET_STATUS_SYS_TYPE:
            if ((pst_mac_dev->uc_device_reset_in_progress == OAL_TRUE) &&
                (pst_reset_sys->uc_value == OAL_FALSE)) {
                pst_mac_dev->us_device_reset_num++;
            }

            hmac_config_set_reset_state(pst_mac_vap, uc_len, puc_param);

            break;
        case MAC_RESET_SWITCH_SYS_TYPE:
            pst_mac_dev->en_reset_switch = pst_reset_sys->uc_value;

            break;
        default:
            break;
    }

    return OAL_SUCC;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0))

OAL_STATIC uint8_t hmac_proc_query_response_flag(hmac_vap_stru *hmac_vap,
    dmac_query_station_info_response_event *query_sta_event)
{
    uint8_t flag = 0;

    flag |= ((query_sta_event->rate_info.txrx_rate.tx_rate.flags & MAC_RATE_INFO_FLAGS_MCS) ?
        RATE_INFO_FLAGS_MCS : 0);

    flag |= ((query_sta_event->rate_info.txrx_rate.tx_rate.flags & MAC_RATE_INFO_FLAGS_VHT_MCS) ?
        RATE_INFO_FLAGS_VHT_MCS : 0);

    flag |= ((query_sta_event->rate_info.txrx_rate.tx_rate.flags & MAC_RATE_INFO_FLAGS_SHORT_GI) ?
        RATE_INFO_FLAGS_SHORT_GI : 0);

    flag |= ((query_sta_event->rate_info.txrx_rate.tx_rate.flags & MAC_RATE_INFO_FLAGS_HE_MCS) ?
        MAC_RATE_INFO_FLAGS_HE_MCS : 0);

    hmac_vap->station_info.txrate.bw =
        query_sta_event->rate_info.txrx_rate.tx_rate.flags & MAC_RATE_INFO_FLAGS_40_MHZ_WIDTH ? RATE_INFO_BW_40 :
        (query_sta_event->rate_info.txrx_rate.tx_rate.flags & MAC_RATE_INFO_FLAGS_80_MHZ_WIDTH ? RATE_INFO_BW_80 :
        (query_sta_event->rate_info.txrx_rate.tx_rate.flags & MAC_RATE_INFO_FLAGS_160_MHZ_WIDTH ?
        RATE_INFO_BW_160 : RATE_INFO_BW_20));

    return flag;
}
#endif


uint32_t hmac_proc_query_response_event(mac_vap_stru *pst_mac_vap, uint8_t uc_len, uint8_t *puc_param)
{
    hmac_vap_stru *pst_hmac_vap;
    uint8_t uc_flag = 0;
    dmac_query_station_info_response_event *pst_query_station_reponse_event = OAL_PTR_NULL;

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_query_response::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_query_station_reponse_event = (dmac_query_station_info_response_event *)(puc_param);
    if (pst_query_station_reponse_event->query_event == OAL_QUERY_STATION_INFO_EVENT) {
        pst_hmac_vap->station_info.signal = pst_query_station_reponse_event->c_signal;
        pst_hmac_vap->station_info.rx_packets = pst_query_station_reponse_event->ul_rx_packets;
        pst_hmac_vap->station_info.tx_packets = pst_query_station_reponse_event->ul_tx_packets;
        pst_hmac_vap->station_info.rx_bytes = pst_query_station_reponse_event->ul_rx_bytes;
        pst_hmac_vap->station_info.tx_bytes = pst_query_station_reponse_event->ul_tx_bytes;

#ifdef CONFIG_HW_GET_EXT_SIG
        pst_hmac_vap->station_info.noise = pst_query_station_reponse_event->s_free_power;
        pst_hmac_vap->station_info.chload = pst_query_station_reponse_event->s_chload;
#endif
#ifdef CONFIG_HW_GET_EXT_SIG_ULDELAY
        pst_hmac_vap->station_info.ul_delay = pst_query_station_reponse_event->uplink_dealy;
#endif
        pst_hmac_vap->station_info.rxrate.legacy =
            pst_query_station_reponse_event->rate_info.txrx_rate.rx_rate_legacy;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37))
        pst_hmac_vap->station_info.tx_retries = pst_query_station_reponse_event->ul_tx_retries;
        // ��������������һ����ǰ���е����ˣ���һ��֡+ 1���ⲻ��Ҫ�ۼӣ��ۼӲ�����dmac_rx_process_frame
        pst_hmac_vap->station_info.rx_dropped_misc = pst_query_station_reponse_event->ul_rx_dropped_misc;
        pst_hmac_vap->station_info.tx_failed = pst_query_station_reponse_event->ul_tx_failed;
#endif
        pst_hmac_vap->station_info.txrate.mcs =
            pst_query_station_reponse_event->rate_info.txrx_rate.tx_rate.mcs;
        pst_hmac_vap->station_info.txrate.legacy =
            pst_query_station_reponse_event->rate_info.txrx_rate.tx_rate.legacy;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0))
        pst_hmac_vap->station_info.txrate.nss =
            pst_query_station_reponse_event->rate_info.txrx_rate.tx_rate.nss;
#endif

        /*
        * ����flag���ں˰汾���죬��DMAC��3.8.0Ϊ׼����ȫ��������ڴ���ȡ��ת��flags
        * linux version < 3.5.0
        *     RATE_INFO_FLAGS_MCS             = 1<<0,
        *     RATE_INFO_FLAGS_40_MHZ_WIDTH    = 1<<1,
        *     RATE_INFO_FLAGS_SHORT_GI        = 1<<2,
        *
        * linux version >= 3.5.0 && linux version < 3.8.0
        *    RATE_INFO_FLAGS_MCS             = 1<<0,
        *    RATE_INFO_FLAGS_40_MHZ_WIDTH    = 1<<1,
        *    RATE_INFO_FLAGS_SHORT_GI        = 1<<2,
        *    RATE_INFO_FLAGS_60G             = 1<<3
        *
        * linux version >= 3.8.0 && linux version < 4.0.0
        *    RATE_INFO_FLAGS_MCS             = BIT(0),
        *    RATE_INFO_FLAGS_VHT_MCS         = BIT(1),
        *    RATE_INFO_FLAGS_40_MHZ_WIDTH    = BIT(2),
        *    RATE_INFO_FLAGS_80_MHZ_WIDTH    = BIT(3),
        *    RATE_INFO_FLAGS_80P80_MHZ_WIDTH = BIT(4),
        *    RATE_INFO_FLAGS_160_MHZ_WIDTH   = BIT(5),
        *    RATE_INFO_FLAGS_SHORT_GI        = BIT(6),
        *    RATE_INFO_FLAGS_60G             = BIT(7),
        *
        * linux version >= 4.0.0
        *    RATE_INFO_FLAGS_MCS               = BIT(0),
        *    RATE_INFO_FLAGS_VHT_MCS           = BIT(1),
        *    RATE_INFO_FLAGS_SHORT_GI          = BIT(2),
        *    RATE_INFO_FLAGS_60G               = BIT(3),
        * rate_info_bw
        *    RATE_INFO_BW_5
        *    RATE_INFO_BW_10
        *    RATE_INFO_BW_20
        *    RATE_INFO_BW_40
        *    RATE_INFO_BW_80
        *    RATE_INFO_BW_160
        */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 8, 0))
            uc_flag |=
                ((pst_query_station_reponse_event->rate_info.txrx_rate.tx_rate.flags & MAC_RATE_INFO_FLAGS_MCS) ?
                RATE_INFO_FLAGS_MCS : 0);
            uc_flag |=
                ((pst_query_station_reponse_event->rate_info.txrx_rate.tx_rate.flags &
                MAC_RATE_INFO_FLAGS_40_MHZ_WIDTH) ? RATE_INFO_FLAGS_40_MHZ_WIDTH : 0);
            uc_flag |=
                ((pst_query_station_reponse_event->rate_info.txrx_rate.tx_rate.flags & MAC_RATE_INFO_FLAGS_SHORT_GI) ?
                RATE_INFO_FLAGS_SHORT_GI : 0);
        
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0))
            uc_flag = pst_query_station_reponse_event->rate_info.txrx_rate.tx_rate.flags;
#else
            uc_flag |= hmac_proc_query_response_flag(pst_hmac_vap, pst_query_station_reponse_event);
#endif
        pst_hmac_vap->station_info.txrate.flags = uc_flag;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
        pst_hmac_vap->center_freq = oal_ieee80211_channel_to_frequency(pst_mac_vap->st_channel.uc_chan_number,
            (enum nl80211_band)pst_mac_vap->st_channel.en_band);
#else
        pst_hmac_vap->center_freq = oal_ieee80211_channel_to_frequency(pst_mac_vap->st_channel.uc_chan_number,
            (enum ieee80211_band)pst_mac_vap->st_channel.en_band);
#endif
        pst_hmac_vap->s_free_power = pst_query_station_reponse_event->s_free_power;
        pst_hmac_vap->st_station_info_extend.uc_distance =
            pst_query_station_reponse_event->st_station_info_extend.uc_distance;
        pst_hmac_vap->st_station_info_extend.uc_cca_intr =
            pst_query_station_reponse_event->st_station_info_extend.uc_cca_intr;
        pst_hmac_vap->st_station_info_extend.c_snr_ant0 =
            pst_query_station_reponse_event->st_station_info_extend.c_snr_ant0;
        pst_hmac_vap->st_station_info_extend.c_snr_ant1 =
            pst_query_station_reponse_event->st_station_info_extend.c_snr_ant1;
        pst_hmac_vap->st_station_info_extend.ul_bcn_cnt =
            pst_query_station_reponse_event->st_station_info_extend.ul_bcn_cnt;
        pst_hmac_vap->st_station_info_extend.ul_bcn_tout_cnt =
            pst_query_station_reponse_event->st_station_info_extend.ul_bcn_tout_cnt;
    }

    /* ����wal_sdt_recv_reg_cmd�ȴ��Ľ��� */
    pst_hmac_vap->station_info_query_completed_flag = OAL_TRUE;
    oal_wait_queue_wake_up_interrupt(&(pst_hmac_vap->query_wait_q));

    return OAL_SUCC;
}


uint32_t hmac_config_reset_operate(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_device_stru *pst_mac_dev;
    mac_reset_sys_stru st_reset_sys;
    uint32_t ul_ret;
    int8_t *pc_token = OAL_PTR_NULL;
    int8_t *pc_end = OAL_PTR_NULL;
    int8_t *pc_ctx = OAL_PTR_NULL;
    int8_t *pc_sep = " ";

    pst_mac_dev = mac_res_get_dev(pst_mac_vap->uc_device_id);

    if (pst_mac_dev == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_config_reset_operate::pst_mac_dev null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ��λ��Ϣ */
    pc_token = oal_strtok((int8_t *)puc_param, pc_sep, &pc_ctx);
    if (pc_token == NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_config_reset_operate::pc_token null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    st_reset_sys.en_reset_sys_type = (mac_reset_sys_type_enum_uint8)oal_strtol(pc_token, &pc_end, 16);

    if (st_reset_sys.en_reset_sys_type == MAC_RESET_SWITCH_SET_TYPE) {
        /* ��ȡChannel List */
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        if (pc_token == NULL) {
            oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_config_reset_operate::pc_token null.}");
            return OAL_ERR_CODE_PTR_NULL;
        }

        st_reset_sys.uc_value = (uint8_t)oal_strtol(pc_token, &pc_end, 16);
        pst_mac_dev->en_reset_switch = st_reset_sys.uc_value;
        ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_RESET_HW_OPERATE,
                                            us_len, (uint8_t *)&st_reset_sys);
        if (oal_unlikely(ul_ret != OAL_SUCC)) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                             "{hmac_config_reset_operate::hmac_config_send_event failed[%d].}", ul_ret);
        }

        return ul_ret;
    } else if (st_reset_sys.en_reset_sys_type == MAC_RESET_SWITCH_GET_TYPE) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{hmac_config_reset_operate::the reset switch is %d.}",
                         pst_mac_dev->en_reset_switch);
    } else if (st_reset_sys.en_reset_sys_type == MAC_RESET_STATUS_GET_TYPE) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{hmac_config_reset_operate::the reset status is %d, the reset num is %d.}",
                         pst_mac_dev->uc_device_reset_in_progress, pst_mac_dev->us_device_reset_num);
    } else {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{hmac_config_reset_operate::the reset tpye is %d, out of limit.}",
                         st_reset_sys.en_reset_sys_type);
    }

    return OAL_SUCC;
}

/*lint -e19 */
oal_module_symbol(hmac_config_reset_operate);
/*lint +e19 */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

