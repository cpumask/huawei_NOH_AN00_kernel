

#ifndef __HMAC_TX_DATA_H__
#define __HMAC_TX_DATA_H__


/* 1 ����ͷ�ļ����� */
#include "mac_frame.h"
#include "mac_common.h"
#include "hmac_vap.h"
#include "hmac_user.h"
#include "hmac_main.h"
#include "hmac_mgmt_classifier.h"
#include "mac_resource.h"

#include "wlan_chip_i.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_TX_DATA_H

/* 2 �궨�� */
/* ����������Ϣ�й����Ƿ���QOS������λ */
#define HMAC_CAP_INFO_QOS_MASK 0x0200

#define WLAN_TOS_TO_TID(_tos) ( \
    (((_tos) == 0) || ((_tos) == 3)) ? WLAN_TIDNO_BEST_EFFORT : (((_tos) == 1) || ((_tos) == 2)) ?   \
    WLAN_TIDNO_BACKGROUND : (((_tos) == 4) || ((_tos) == 5)) ? WLAN_TIDNO_VIDEO : WLAN_TIDNO_VOICE)

#define WLAN_BA_CNT_INTERVAL 100

#define HMAC_TXQUEUE_DROP_LIMIT_LOW  600
#define HMAC_TXQUEUE_DROP_LIMIT_HIGH 800

#define HMAC_TX_PKTS_STAT(_pkt) (g_host_tx_pkts.ul_snd_pkts += (_pkt))

#define WLAN_AMPDU_THROUGHPUT_THRESHOLD_HIGH            300 /* 300Mb/s */
#define WLAN_AMPDU_THROUGHPUT_THRESHOLD_MIDDLE          250 /* 250Mb/s */
#define WLAN_AMPDU_THROUGHPUT_THRESHOLD_LOW             200 /* 200Mb/s */
#define WLAN_SMALL_AMSDU_THROUGHPUT_THRESHOLD_HIGH      300 /* 300Mb/s */
#define WLAN_SMALL_AMSDU_THROUGHPUT_THRESHOLD_LOW       200 /* 200Mb/s */
#define WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH      90  /* 100Mb/s */
#define WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_LOW       30  /* 30Mb/s */
#define WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_40M  300 /* 300Mb/s */
#define WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_LOW_40M   150 /* 150Mb/s */
#define WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_80M  550 /* 500Mb/s */
#define WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_LOW_80M   450 /* 300Mb/s */
#define WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_160M 800 /* 500Mb/s */
#define WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_LOW_160M  700 /* 300Mb/s */

#define WLAN_DYN_BYPASS_EXTLNA_THROUGHPUT_THRESHOLD_HIGH 100   /* 100Mb/s */
#define WLAN_DYN_BYPASS_EXTLNA_THROUGHPUT_THRESHOLD_LOW  50    /* 50Mb/s */
#define WLAN_SMALL_AMSDU_PPS_THRESHOLD_HIGH              25000 /* pps 25000 */
#define WLAN_SMALL_AMSDU_PPS_THRESHOLD_LOW               5000  /* pps 5000 */
#define WLAN_TCP_ACK_FILTER_THROUGHPUT_TH_HIGH           50
#define WLAN_TCP_ACK_FILTER_THROUGHPUT_TH_LOW            20

#define WLAN_AMSDU_THROUGHPUT_TH_HE_VHT_DIFF             100 /* 100Mb/s */

#define WLAN_AMPDU_HW_SWITCH_PERIOD 100 /* Ӳ���ۺ��л�����ۺ�ʱ�䣺10s */

#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
#define WLAN_HID2D_ONE_SEC_CNT    31250  /* 1s�����32us */
#define WLAN_HID2D_100MS_CNT    3125  /* 100ms�����32us, cbֻ��6bit���洫��ʱ��       (ms)     */
#define WLAN_HID2D_1500US_CNT    47  /* 1500us�����32us */
#endif

#define DATA_TYPE_802_3_SNAP (BIT(2))

/* 3 ö�ٶ��� */
typedef enum {
    HMAC_TX_BSS_NOQOS = 0,
    HMAC_TX_BSS_QOS = 1,

    HMAC_TX_BSS_QOS_BUTT
} hmac_tx_bss_qos_type_enum;

typedef enum {
    DATA_TYPE_ETH = 0,    /* Eth II��̫����������, ��vlan */
    DATA_TYPE_1_VLAN_ETH = 1,    /* Eth II��̫����������, ��1��vlan */
    DATA_TYPE_2_VLAN_ETH = 2,    /* Eth II��̫����������, ��2��vlan */
    DATA_TYPE_80211 = 3,         /* 802.11�������� */
    DATA_TYPE_0_VLAN_8023 = 4,   /* 802.3��̫����������, ��vlan */
    DATA_TYPE_1_VLAN_8023 = 5,   /* 802.3��̫����������, ��1��vlan */
    DATA_TYPE_2_VLAN_8023 = 6,   /* 802.3��̫����������, ��2��vlan */
    DATA_TYPE_MESH = 7,          /* Mesh��������, ��extend addresss */
    DATA_TYPE_MESH_EXT_ADDR = 8, /* Mesh��������, ��extend addresss */

    DATA_TYPE_BUTT
} data_type_enum;

/* 4 ��Ϣͷ���� */
/* 5 ��Ϣ���� */
/* 6 STRUCT���� */
typedef struct {
    uint32_t ul_pkt_len;    /* HOST��֡��ͳ�� */
    uint32_t ul_snd_pkts;   /* ����ʵ�ʷ���֡ͳ�� */
    uint32_t ul_start_time; /* ����ͳ�ƿ�ʼʱ�� */
} hmac_tx_pkts_stat_stru;
extern hmac_tx_pkts_stat_stru g_host_tx_pkts;

#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
typedef struct {
    /* ���ƻ�Ӳ���ۺ��Ƿ���Ч,Ĭ��Ϊ����ۺ� */
    uint8_t uc_ampdu_hw_en;
    /* ��ǰ�ۺ���Ӳ���ۺϻ�������ۺ� */
    uint8_t uc_ampdu_hw_enable;
    uint16_t us_remain_hw_cnt;
    uint16_t us_throughput_high;
    uint16_t us_throughput_low;
} hmac_tx_ampdu_hw_stru;
extern hmac_tx_ampdu_hw_stru g_st_ampdu_hw;
#endif

typedef enum {
    TX_UPDATE_MODE_EVENT = 0,
    TX_UPDATE_MODE_WRITE_REG = 1,

    TX_UPDATE_MODE_BUTT,
} wlan_tx_update_mode_enum;

typedef struct {
    uint16_t tx_switch;
    uint16_t tx_update_mode;
} wlan_tx_switch_stru;

extern wlan_tx_switch_stru g_tx_switch;
extern uint8_t g_uc_tx_ba_policy_select;

/* 8 UNION���� */
/* 10 �������� */
uint32_t hmac_tx_encap(hmac_vap_stru *pst_vap,
                                    hmac_user_stru *pst_user,
                                    oal_netbuf_stru *pst_buf);
uint32_t hmac_tx_ucast_process(hmac_vap_stru *pst_vap,
                                            oal_netbuf_stru *pst_buf,
                                            hmac_user_stru *pst_user,
                                            mac_tx_ctl_stru *pst_tx_ctl);

void hmac_tx_ba_setup(hmac_vap_stru *pst_vap,
                                     hmac_user_stru *pst_user,
                                     uint8_t uc_tidno);

void hmac_tx_ba_cnt_vary(hmac_vap_stru *pst_hmac_vap,
                                        hmac_user_stru *pst_hmac_user,
                                        uint8_t uc_tidno,
                                        oal_netbuf_stru *pst_buf);

uint8_t hmac_tx_wmm_acm(oal_bool_enum_uint8 en_wmm,
                                     hmac_vap_stru *pst_hmac_vap, uint8_t *puc_tid);
#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
void hmac_tx_ampdu_switch(uint32_t ul_tx_throughput_mbps);
#endif
void hmac_rx_dyn_bypass_extlna_switch(uint32_t ul_tx_throughput_mbps,
                                                 uint32_t ul_rx_throughput_mbps);

#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
void hmac_tx_amsdu_ampdu_switch(uint32_t ul_tx_throughput_mbps);
#endif
#ifdef _PRE_WLAN_TCP_OPT
void hmac_tcp_ack_filter_switch(uint32_t ul_rx_throughput_mbps);
#endif

void hmac_tx_small_amsdu_switch(uint32_t ul_rx_throughput_mbps, uint32_t ul_pps);

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
void hmac_tx_tcp_ack_buf_switch(uint32_t ul_rx_throughput_mbps);
#endif

uint32_t hmac_tx_filter_security(hmac_vap_stru *hmac_vap, oal_netbuf_stru *netbuf, hmac_user_stru *hmac_user);
void hmac_tx_classify(hmac_vap_stru *hmac_vap, mac_user_stru *mac_user, oal_netbuf_stru *netbuf);
uint32_t hmac_tx_need_frag(
    hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf, mac_tx_ctl_stru *tx_ctl);
void hmac_update_amsdu_num(mac_vap_stru *pst_mac_vap,
    uint32_t ul_tx_throughput_mbps, oal_bool_enum_uint8 en_mu_vap_flag, wlan_tx_amsdu_enum_uint8 *pen_tx_amsdu);
void hmac_update_amsdu_num_1105(mac_vap_stru *pst_mac_vap,
    uint32_t ul_tx_throughput_mbps, oal_bool_enum_uint8 en_mu_vap_flag, wlan_tx_amsdu_enum_uint8 *pen_tx_amsdu);


/* 9 OTHERS���� */

OAL_STATIC OAL_INLINE wlan_tx_switch_enum hmac_get_tx_switch(void)
{
    return g_tx_switch.tx_switch;
}


OAL_STATIC OAL_INLINE oal_netbuf_stru *hmac_tx_get_next_mpdu(oal_netbuf_stru *pst_buf,
                                                             uint8_t uc_netbuf_num)
{
    oal_netbuf_stru *pst_next_buf = OAL_PTR_NULL;
    uint32_t ul_netbuf_index;

    if (oal_unlikely(pst_buf == OAL_PTR_NULL)) {
        return OAL_PTR_NULL;
    }

    pst_next_buf = pst_buf;
    for (ul_netbuf_index = 0; ul_netbuf_index < uc_netbuf_num; ul_netbuf_index++) {
        pst_next_buf = oal_netbuf_list_next(pst_next_buf);
    }

    return pst_next_buf;
}


OAL_STATIC OAL_INLINE void hmac_tx_netbuf_list_enqueue(oal_netbuf_head_stru *pst_head,
                                                           oal_netbuf_stru *pst_buf,
                                                           uint8_t uc_netbuf_num)
{
    uint32_t ul_netbuf_index;
    oal_netbuf_stru *pst_buf_next = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_head, pst_buf))) {
        return;
    }

    for (ul_netbuf_index = 0; ul_netbuf_index < uc_netbuf_num; ul_netbuf_index++) {
        pst_buf_next = oal_netbuf_list_next(pst_buf);
        oal_netbuf_add_to_list_tail(pst_buf, pst_head);
        pst_buf = pst_buf_next;
    }
}


OAL_STATIC OAL_INLINE void hmac_tx_get_addr(mac_ieee80211_qos_htc_frame_addr4_stru *pst_hdr,
                                                uint8_t *puc_saddr,
                                                uint8_t *puc_daddr)
{
    uint8_t uc_to_ds;
    uint8_t uc_from_ds;

    uc_to_ds = mac_hdr_get_to_ds((uint8_t *)pst_hdr);
    uc_from_ds = mac_hdr_get_from_ds((uint8_t *)pst_hdr);

    if ((uc_to_ds == 1) && (uc_from_ds == 0)) {
        /* to AP */
        oal_set_mac_addr(puc_saddr, pst_hdr->auc_address2);
        oal_set_mac_addr(puc_daddr, pst_hdr->auc_address3);
    } else if ((uc_to_ds == 0) && (uc_from_ds == 0)) {
        /* IBSS */
        oal_set_mac_addr(puc_saddr, pst_hdr->auc_address2);
        oal_set_mac_addr(puc_daddr, pst_hdr->auc_address1);
    } else if ((uc_to_ds == 1) && (uc_from_ds == 1)) {
        /* WDS */
        oal_set_mac_addr(puc_saddr, pst_hdr->auc_address4);
        oal_set_mac_addr(puc_daddr, pst_hdr->auc_address3);
    } else {
        /* From AP */
        oal_set_mac_addr(puc_saddr, pst_hdr->auc_address3);
        oal_set_mac_addr(puc_daddr, pst_hdr->auc_address1);
    }
}


OAL_STATIC OAL_INLINE void hmac_tx_set_frame_ctrl(uint32_t ul_qos,
                                                      mac_tx_ctl_stru *pst_tx_ctl,
                                                      mac_ieee80211_qos_htc_frame_addr4_stru *pst_hdr_addr4)
{
    mac_ieee80211_qos_htc_frame_stru *pst_hdr = OAL_PTR_NULL;
    if (ul_qos == HMAC_TX_BSS_QOS) {
        /* ����֡�����ֶ� */
        mac_hdr_set_frame_control((uint8_t *)pst_hdr_addr4, (WLAN_FC0_TYPE_DATA | WLAN_FC0_SUBTYPE_QOS));

        /* ����֡ͷ���� */
        if (OAL_FALSE == MAC_GET_CB_IS_4ADDRESS(pst_tx_ctl)) {
            pst_hdr = (mac_ieee80211_qos_htc_frame_stru *)pst_hdr_addr4;
            /* ����QOS�����ֶ� */
            pst_hdr->bit_qc_tid = MAC_GET_CB_WME_TID_TYPE(pst_tx_ctl);
            pst_hdr->bit_qc_eosp = 0;
            pst_hdr->bit_qc_ack_polocy = MAC_GET_CB_ACK_POLACY(pst_tx_ctl);
            pst_hdr->bit_qc_amsdu = MAC_GET_CB_IS_AMSDU(pst_tx_ctl);
            pst_hdr->qos_control.bit_qc_txop_limit = 0;
            MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl) = MAC_80211_QOS_FRAME_LEN;
        } else {
            /* ����QOS�����ֶ� */
            pst_hdr_addr4->bit_qc_tid = MAC_GET_CB_WME_TID_TYPE(pst_tx_ctl);
            pst_hdr_addr4->bit_qc_eosp = 0;
            pst_hdr_addr4->bit_qc_ack_polocy = MAC_GET_CB_ACK_POLACY(pst_tx_ctl);
            pst_hdr_addr4->bit_qc_amsdu = MAC_GET_CB_IS_AMSDU(pst_tx_ctl);
            pst_hdr_addr4->qos_control.qc_txop_limit = 0;
            MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl) = MAC_80211_QOS_4ADDR_FRAME_LEN;
        }

        /* ��DMAC�����Ƿ���ҪHTC */
    } else {
        /* ����֡�����ֶ� */
        mac_hdr_set_frame_control((uint8_t *)pst_hdr_addr4, WLAN_FC0_TYPE_DATA | WLAN_FC0_SUBTYPE_DATA);

        /* ��QOS����֡֡�����ֶ����� */
        if (MAC_GET_CB_IS_4ADDRESS(pst_tx_ctl)) {
            MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl) = MAC_80211_4ADDR_FRAME_LEN;
        } else {
            MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl) = MAC_80211_FRAME_LEN;
        }
    }
}

#ifdef _PRE_WLAN_FEATURE_11AX
#ifdef _PRE_WLAN_FEATURE_HIEX

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_tx_enable_himit_htc(hmac_user_stru *user, mac_tx_ctl_stru *tx_ctl)
{
    return ((MAC_USER_HIMIT_ENABLED(&user->st_user_base_info) == OAL_TRUE) &&
             MAC_GET_CB_IS_GAME_MARKED_FRAME(tx_ctl) &&
             ((MAC_GET_CB_ACK_POLACY(tx_ctl) == WLAN_TX_NORMAL_ACK) ||
             (MAC_GET_CB_ACK_POLACY(tx_ctl) == WLAN_TX_BLOCK_ACK))) ? OAL_TRUE : OAL_FALSE;
}
#endif

OAL_STATIC OAL_INLINE void hmac_tx_set_frame_htc(hmac_vap_stru *pst_hmac_vap, uint32_t ul_qos,
                                                     mac_tx_ctl_stru *pst_tx_ctl, hmac_user_stru *pst_hmac_user,
                                                     mac_ieee80211_qos_htc_frame_addr4_stru *pst_hdr_addr4)
{
    mac_ieee80211_qos_htc_frame_stru *pst_hdr = OAL_PTR_NULL;
    uint32_t                        ul_htc_val = 0;

    if (ul_qos != HMAC_TX_BSS_QOS) {
        return;
    }
    if (WLAN_DATA_VIP_TID == MAC_GET_CB_WME_TID_TYPE(pst_tx_ctl)) {
        return;
    }
    if (pst_hmac_vap->uc_htc_order_flag == HAL_HE_HTC_CONFIG_NO_HTC) {
        MAC_GET_CB_HTC_FLAG(pst_tx_ctl) = 0;
        return;
    }

    /* ��������ʱ��host���himit֡����htc�ֶ�����ΪȫF,��dmac�ж�OM��ǰ�Ƿ�ʹ��HTC�ֶ�,OM���Բ���ʱ,himit��������HTC */
#ifdef _PRE_WLAN_FEATURE_HIEX
    if (g_wlan_spec_cfg->feature_hiex_is_open) {
        if (hmac_tx_enable_himit_htc(pst_hmac_user, pst_tx_ctl)) {
            ul_htc_val = HTC_INVALID_VALUE;
            MAC_GET_CB_IS_HIMIT_FRAME(pst_tx_ctl) = OAL_TRUE;
        } else if (MAC_USER_TX_DATA_INCLUDE_HTC(&pst_hmac_user->st_user_base_info)) {
            ul_htc_val = HTC_INVALID_VALUE;
        } else {
            return;
        }
    } else {
        if (MAC_USER_TX_DATA_INCLUDE_HTC(&pst_hmac_user->st_user_base_info)) {
            ul_htc_val = HTC_INVALID_VALUE;
        } else {
            return;
        }
    }
#endif

    MAC_GET_CB_HTC_FLAG(pst_tx_ctl) = 1;  // ���Ϊ����HTC�ֶ�
    /* ����֡ͷ���� */
    if (OAL_FALSE == MAC_GET_CB_IS_4ADDRESS(pst_tx_ctl)) {
        pst_hdr = (mac_ieee80211_qos_htc_frame_stru *)pst_hdr_addr4;
        pst_hdr->st_frame_control.bit_order = 1;
        MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl) = MAC_80211_QOS_HTC_FRAME_LEN;
        pst_hdr->ul_htc = ul_htc_val;
    } else {
        /* ����QOS�����ֶ� */
        pst_hdr_addr4->st_frame_control.bit_order = 1;
        MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl) = MAC_80211_QOS_HTC_4ADDR_FRAME_LEN;
        pst_hdr_addr4->ul_htc = ul_htc_val;
    }
}
#endif


OAL_STATIC OAL_INLINE uint32_t hmac_tx_set_addresses(hmac_vap_stru *pst_vap,
                                                       hmac_user_stru *pst_user,
                                                       mac_tx_ctl_stru *pst_tx_ctl,
                                                       mac_ether_header_stru *pst_ether_hdr,
                                                       mac_ieee80211_qos_htc_frame_addr4_stru *pst_hdr)
{
    /* ��Ƭ���ó�0��������Ƭ������Ҫ���¸�ֵ */
    pst_hdr->bit_frag_num = 0;
    pst_hdr->bit_seq_num = 0;

    if ((pst_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) &&
        (!(MAC_GET_CB_IS_4ADDRESS(pst_tx_ctl)))) { /* From AP */
        /* From DS��ʶλ���� */
        mac_hdr_set_from_ds((uint8_t *)pst_hdr, 1);

        /* to DS��ʶλ���� */
        mac_hdr_set_to_ds((uint8_t *)pst_hdr, 0);

        /* Set Address1 field in the WLAN Header with destination address */
        oal_set_mac_addr(pst_hdr->auc_address1, (uint8_t *)pst_ether_hdr->auc_ether_dhost);

        /* Set Address2 field in the WLAN Header with the BSSID */
        oal_set_mac_addr(pst_hdr->auc_address2, pst_vap->st_vap_base_info.auc_bssid);

        if (MAC_GET_CB_IS_AMSDU(pst_tx_ctl)) { /* AMSDU�������ַ3��дBSSID */
            /* Set Address3 field in the WLAN Header with the BSSID */
            oal_set_mac_addr(pst_hdr->auc_address3, pst_vap->st_vap_base_info.auc_bssid);
        } else {
            /* Set Address3 field in the WLAN Header with the source address */
            oal_set_mac_addr(pst_hdr->auc_address3, (uint8_t *)pst_ether_hdr->auc_ether_shost);
        }
    } else if ((pst_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) &&
               (!(MAC_GET_CB_IS_4ADDRESS(pst_tx_ctl)))) {
        /* From DS��ʶλ���� */
        mac_hdr_set_from_ds((uint8_t *)pst_hdr, 0);

        /* to DS��ʶλ���� */
        mac_hdr_set_to_ds((uint8_t *)pst_hdr, 1);

        /* Set Address1 field in the WLAN Header with BSSID */
        oal_set_mac_addr(pst_hdr->auc_address1, pst_user->st_user_base_info.auc_user_mac_addr);

        if (pst_ether_hdr->us_ether_type == oal_byteorder_host_to_net_uint16(ETHER_LLTD_TYPE)) {
            /* Set Address2 field in the WLAN Header with the source address */
            oal_set_mac_addr(pst_hdr->auc_address2, (uint8_t *)pst_ether_hdr->auc_ether_shost);
        } else {
            /* Set Address2 field in the WLAN Header with the source address */
            oal_set_mac_addr(pst_hdr->auc_address2, mac_mib_get_StationID(&pst_vap->st_vap_base_info));
        }

        if (MAC_GET_CB_IS_AMSDU(pst_tx_ctl)) { /* AMSDU�������ַ3��дBSSID */
            /* Set Address3 field in the WLAN Header with the BSSID */
            oal_set_mac_addr(pst_hdr->auc_address3, pst_user->st_user_base_info.auc_user_mac_addr);
        } else {
            /* Set Address3 field in the WLAN Header with the destination address */
            oal_set_mac_addr(pst_hdr->auc_address3, (uint8_t *)pst_ether_hdr->auc_ether_dhost);
        }
    } else if (MAC_GET_CB_IS_4ADDRESS(pst_tx_ctl)) {
        /* WDS */
        /* TO DS��ʶλ���� */
        mac_hdr_set_to_ds((uint8_t *)pst_hdr, 1);

        /* From DS��ʶλ���� */
        mac_hdr_set_from_ds((uint8_t *)pst_hdr, 1);

        /* ��ַ1�� RA */
        oal_set_mac_addr(pst_hdr->auc_address1, pst_user->st_user_base_info.auc_user_mac_addr);

        /* ��ַ2�� TA (��ǰֻ��BSSID) */
        oal_set_mac_addr(pst_hdr->auc_address2, mac_mib_get_StationID(&pst_vap->st_vap_base_info));

        if (MAC_GET_CB_IS_AMSDU(pst_tx_ctl)) { /* AMSDU�������ַ3�͵�ַ4��дBSSID */
            if (pst_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
                /* ��ַ3�� BSSID */
                oal_set_mac_addr(pst_hdr->auc_address3, pst_user->st_user_base_info.auc_user_mac_addr);

                /* ��ַ4Ҳ�� BSSID */
                oal_set_mac_addr(pst_hdr->auc_address4, pst_user->st_user_base_info.auc_user_mac_addr);
            } else if (pst_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
                /* ��ַ3�� BSSID */
                oal_set_mac_addr(pst_hdr->auc_address3, mac_mib_get_StationID(&pst_vap->st_vap_base_info));

                /* ��ַ4Ҳ�� BSSID */
                oal_set_mac_addr(pst_hdr->auc_address4, mac_mib_get_StationID(&pst_vap->st_vap_base_info));
            }
        } else {
            /* ��ַ3�� DA */
            oal_set_mac_addr(pst_hdr->auc_address3, (uint8_t *)pst_ether_hdr->auc_ether_dhost);

            /* ��ַ4�� SA */
            oal_set_mac_addr(pst_hdr->auc_address4, (uint8_t *)pst_ether_hdr->auc_ether_shost);
        }
    }

    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_tid_ba_is_setup(hmac_user_stru *pst_hmac_user, uint8_t uc_tidno)
{
    if (oal_unlikely(pst_hmac_user == OAL_PTR_NULL) || uc_tidno >= WLAN_TID_MAX_NUM) {
        return OAL_FALSE;
    }
    return (pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.en_ba_status == DMAC_BA_COMPLETE);
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_vap_ba_is_setup(hmac_user_stru *pst_hmac_user)
{
    uint8_t uc_tidno;

    if (pst_hmac_user == OAL_PTR_NULL) {
        return OAL_FALSE;
    }

    for (uc_tidno = 0; uc_tidno < WLAN_TID_MAX_NUM; uc_tidno++) {
        if (pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.en_ba_status == DMAC_BA_COMPLETE) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}

OAL_STATIC OAL_INLINE void hmac_tx_ba_del(hmac_vap_stru *pst_hmac_vap,
                                              hmac_user_stru *pst_hmac_user,
                                              uint8_t uc_tidno)
{
    mac_action_mgmt_args_stru st_action_args; /* ������дACTION֡�Ĳ��� */

    st_action_args.uc_category = MAC_ACTION_CATEGORY_BA;
    st_action_args.uc_action = MAC_BA_ACTION_DELBA;
    st_action_args.ul_arg1 = uc_tidno;
    st_action_args.ul_arg2 = MAC_ORIGINATOR_DELBA;
    st_action_args.ul_arg3 = MAC_UNSPEC_REASON;
    st_action_args.puc_arg5 = pst_hmac_user->st_user_base_info.auc_user_mac_addr;
    hmac_mgmt_tx_action(pst_hmac_vap, pst_hmac_user, &st_action_args);
}
OAL_STATIC OAL_INLINE void hmac_need_del_ba_check(hmac_vap_stru *pst_hmac_vap,
    hmac_user_stru *pst_hmac_user, uint8_t uc_tidno)
{
#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
    /* �ж���Ҫɾ��BA,������������Ѿ�ɾ��BA,�����Ҫ�ٴ��ж�ɾ��BA���� */
    oal_spin_lock_bh(&pst_hmac_vap->st_ampdu_lock);
    if (OAL_FALSE == mac_mib_get_CfgAmpduTxAtive(&pst_hmac_vap->st_vap_base_info) &&
        (OAL_TRUE == hmac_tid_ba_is_setup(pst_hmac_user, uc_tidno))) {
        hmac_tx_ba_del(pst_hmac_vap, pst_hmac_user, uc_tidno);
    }

    oal_spin_unlock_bh(&pst_hmac_vap->st_ampdu_lock);
#else
    hmac_tx_ba_del(pst_hmac_vap, pst_hmac_user, uc_tidno);
#endif
}

OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_tid_need_ba_session(hmac_vap_stru *pst_hmac_vap,
                                                                   hmac_user_stru *pst_hmac_user,
                                                                   uint8_t uc_tidno,
                                                                   oal_netbuf_stru *pst_buf)
{
    hmac_tid_stru *pst_hmac_tid_info = OAL_PTR_NULL;
    wlan_ciper_protocol_type_enum_uint8 en_cipher_type;
    oal_bool_enum_uint8 en_ampdu_support;

    /* ��tid�²�����BA�������������� */
    if (pst_hmac_user->ast_tid_info[uc_tidno].en_ba_handle_tx_enable == OAL_FALSE) {
        return OAL_FALSE;
    }

    if (OAL_TRUE == hmac_tid_ba_is_setup(pst_hmac_user, uc_tidno)) {
        if (OAL_TRUE == mac_mib_get_CfgAmpduTxAtive(&pst_hmac_vap->st_vap_base_info)) {
            return OAL_FALSE;
        }

        hmac_need_del_ba_check(pst_hmac_vap, pst_hmac_user, uc_tidno);
    }

    /* ��������������ۺ�ʱ���� */
    if (OAL_FALSE == mac_mib_get_CfgAmpduTxAtive(&pst_hmac_vap->st_vap_base_info)) {
        return OAL_FALSE;
    }

    /* ����BA�Ự����Ҫ�ж�VAP��AMPDU��֧���������Ϊ��Ҫʵ�ֽ���BA�Ựʱ��һ����AMPDU */
    if (OAL_FALSE == hmac_user_xht_support(pst_hmac_user)) {
        if (pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.en_ba_status != DMAC_BA_INIT) {
            hmac_tx_ba_del(pst_hmac_vap, pst_hmac_user, uc_tidno);
        }

        return OAL_FALSE;
    }

    /* wep/tkip��֧��11n������Э�� */
    en_cipher_type = pst_hmac_user->st_user_base_info.st_key_info.en_cipher_type;
    en_ampdu_support = ((en_cipher_type == WLAN_80211_CIPHER_SUITE_NO_ENCRYP) ||
                        (en_cipher_type == WLAN_80211_CIPHER_SUITE_CCMP) ||
                        (en_cipher_type == WLAN_80211_CIPHER_SUITE_CCMP_256) ||
                        (en_cipher_type == WLAN_80211_CIPHER_SUITE_GCMP) ||
                        (en_cipher_type == WLAN_80211_CIPHER_SUITE_GCMP_256))
                       ? OAL_TRUE
                       : OAL_FALSE;
    if (en_ampdu_support == OAL_FALSE) {
        return OAL_FALSE;
    }

    if (WLAN_ADDBA_MODE_AUTO != mac_mib_get_AddBaMode(&pst_hmac_vap->st_vap_base_info)) {
        return OAL_FALSE;
    }

    /* tid7�������ۺϣ���VIP֡ʹ�� */
    if (uc_tidno == WLAN_TIDNO_BCAST) {
        return OAL_FALSE;
    }

    if (wlan_chip_check_need_setup_ba_session() == OAL_FALSE) {
        return OAL_FALSE;
    }

    /* �ж�HMAC VAP���Ƿ�֧�־ۺ� */
    if (!(mac_mib_get_TxAggregateActived(&pst_hmac_vap->st_vap_base_info) ||
          (pst_hmac_vap->st_vap_base_info.st_cap_flag.bit_rifs_tx_on))) {
        oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                      "{hmac_tid_need_ba_session::en_tx_aggr_on of vap is off");
        return OAL_FALSE;
    }

    if (mac_mib_get_TxBASessionNumber(&pst_hmac_vap->st_vap_base_info) >= WLAN_MAX_TX_BA) {
        oam_info_log1(0, OAM_SF_BA,
                      "{hmac_tid_need_ba_session::uc_tx_ba_session_num[%d] exceed spec",
                      mac_mib_get_TxBASessionNumber(&pst_hmac_vap->st_vap_base_info));
        return OAL_FALSE;
    }

    /* ����BA�Ự�Ľ��� */
    if ((pst_hmac_user->st_user_base_info.st_cap_info.bit_qos == OAL_TRUE) &&
        (pst_hmac_user->auc_ba_flag[uc_tidno] < pst_hmac_user->uc_tx_ba_limit)) {
        oam_info_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                      "{hmac_tid_need_ba_session::auc_ba_flag[%d]}", pst_hmac_user->auc_ba_flag[uc_tidno]);
        hmac_tx_ba_cnt_vary(pst_hmac_vap, pst_hmac_user, uc_tidno, pst_buf);
        return OAL_FALSE;
    } else if (pst_hmac_user->st_user_base_info.st_cap_info.bit_qos == OAL_FALSE) { /* ��Թر�WMM����QOS֡���� */
        oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{UnQos Frame pass!!}");
        return OAL_FALSE;
    }

    pst_hmac_tid_info = &(pst_hmac_user->ast_tid_info[uc_tidno]);
    if ((pst_hmac_tid_info->st_ba_tx_info.en_ba_status == DMAC_BA_INIT) &&
        (pst_hmac_tid_info->st_ba_tx_info.uc_addba_attemps < HMAC_ADDBA_EXCHANGE_ATTEMPTS)) {
        pst_hmac_tid_info->st_ba_tx_info.en_ba_status = DMAC_BA_INPROGRESS;
        pst_hmac_tid_info->st_ba_tx_info.uc_addba_attemps++;
    } else {
        oam_info_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                      "{hmac_tid_need_ba_session::addba_attemps[%d] of tid[%d] is COMPLETE}",
                      pst_hmac_tid_info->st_ba_tx_info.uc_addba_attemps, uc_tidno);
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_tx_data.h */
