

#ifndef __MAC_DATA_H__
#define __MAC_DATA_H__

// �˴�����extern "C" UT���벻��
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "wlan_mib.h"
#include "mac_user.h"
#include "oam_ext_if.h"
#include "mac_regdomain.h"

/* 2 �궨�� */
/* ��ʱ���������Ҫ����oal_net.h */
#define OAL_EAPOL_INFO_POS 13
#define OAL_EAPOL_TYPE_POS 9
#define OAL_EAPOL_TYPE_KEY 3

#define MAC_GET_SNAP_TYPE(_pst_snap) (*(uint8_t *)(_pst_snap))

#define ETHER_TYPE_VLAN_88A8 0x88a8 /* VLAN TAG TPID */
#define ETHER_TYPE_VLAN_9100 0x9100 /* VLAN TAG TPID */

/* 3 ö�ٶ��� */
/* ����֡������ö�ٶ��� */
typedef enum {
    MAC_DATA_DHCP = 0,
    MAC_DATA_EAPOL,
    MAC_DATA_ARP_RSP,
    MAC_DATA_ARP_REQ, /* ע��: ǰ4��vip֡����˳����� */
    MAC_DATA_DHCPV6,
    MAC_DATA_PPPOE,
    MAC_DATA_WAPI,
    MAC_DATA_HS20,
    MAC_DATA_CHARIOT_SIG,                      /* chariot ����� */
    MAC_DATA_VIP_FRAME = MAC_DATA_CHARIOT_SIG, /* ����ΪVIP DATA FRAME */
    MAC_DATA_TDLS,
    MAC_DATA_VLAN,
    MAC_DATA_ND,

    MAC_DATA_BUTT
} mac_data_type_enum_uint8;

typedef enum {
    MAC_NETBUFF_PAYLOAD_ETH = 0,
    MAC_NETBUFF_PAYLOAD_SNAP,

    MAC_NETBUFF_PAYLOAD_BUTT
} mac_netbuff_payload_type;
typedef uint8_t mac_netbuff_payload_type_uint8;

typedef enum {
    PKT_TRACE_DATA_DHCP = 0,
    PKT_TRACE_DATA_ARP_REQ,
    PKT_TRACE_DATA_ARP_RSP,
    PKT_TRACE_DATA_EAPOL,
    PKT_TRACE_DATA_ICMP,
    PKT_TRACE_MGMT_ASSOC_REQ,
    PKT_TRACE_MGMT_ASSOC_RSP,
    PKT_TRACE_MGMT_REASSOC_REQ,
    PKT_TRACE_MGMT_REASSOC_RSP,
    PKT_TRACE_MGMT_DISASOC,
    PKT_TRACE_MGMT_AUTH,
    PKT_TRACE_MGMT_DEAUTH,
    PKT_TRACE_BUTT
} pkt_trace_type_enum;
typedef uint8_t pkt_trace_type_enum_uint8;

uint8_t mac_get_dhcp_frame_type(oal_ip_header_stru *pst_rx_ip_hdr);
mac_eapol_type_enum_uint8 mac_get_eapol_key_type(uint8_t *pst_payload);
oal_bool_enum_uint8 mac_is_dhcp_port(mac_ip_header_stru *pst_ip_hdr);
oal_bool_enum_uint8 mac_is_nd(oal_ipv6hdr_stru *pst_ipv6hdr);
oal_bool_enum_uint8 mac_is_dhcp6(oal_ipv6hdr_stru *pst_ether_hdr);
uint8_t mac_get_data_type(oal_netbuf_stru *pst_netbuff);
uint8_t mac_get_data_type_from_8023(uint8_t *puc_frame_hdr,
                                                 mac_netbuff_payload_type uc_hdr_type);
uint8_t mac_get_data_type_by_snap(oal_netbuf_stru *netbuff);
oal_bool_enum_uint8 mac_is_eapol_key_ptk(mac_eapol_header_stru *pst_eapol_header);
uint8_t mac_get_data_type_from_80211(oal_netbuf_stru *pst_netbuff,
                                                  uint16_t us_mac_hdr_len);
uint16_t mac_get_eapol_keyinfo(oal_netbuf_stru *pst_netbuff);
uint8_t mac_get_eapol_type(oal_netbuf_stru *pst_netbuff);
oal_bool_enum_uint8 mac_is_eapol_key_ptk_4_4(oal_netbuf_stru *pst_netbuff);
pkt_trace_type_enum_uint8 mac_pkt_should_trace(uint8_t *puc_frame_hdr,
                                                      mac_netbuff_payload_type uc_hdr_type);
oal_bool_enum mac_snap_is_protocol_type(uint8_t ul_snap_type);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of mac_vap.h */

