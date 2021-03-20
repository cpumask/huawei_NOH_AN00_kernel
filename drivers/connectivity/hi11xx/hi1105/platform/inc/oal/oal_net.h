

#ifndef __OAL_NET_H__
#define __OAL_NET_H__

/* ����ͷ�ļ����� */
#include "platform_spec.h"
#include "oal_types.h"
#include "oal_mm.h"
#include "oal_util.h"
#include "oal_schedule.h"
#include "oal_list.h"
#include "arch/oal_net.h"
/* �궨�� */
#define OAL_IF_NAME_SIZE               16
#define OAL_NETBUF_DEFAULT_DATA_OFFSET 48 /* 5115��ʵ�ʲ��data��head��48������netbuf dataָ�븴λ */

#define OAL_ASSOC_REQ_IE_OFFSET     28 /* �ϱ��ں˹�������֡ƫ���� */
#define OAL_ASSOC_RSP_IE_OFFSET     30 /* �ϱ��ں˹�����Ӧ֡ƫ���� */
#define OAL_AUTH_IE_OFFSET          30
#define OAL_FT_ACTION_IE_OFFSET     40
#define OAL_ASSOC_RSP_FIXED_OFFSET  6 /* ������Ӧ֡֡��FIXED PARAMETERSƫ���� */
#define OAL_SEQ_CTL_LEN             2 /* ���п����ֶγ��� */
#define OAL_MAC_ADDR_LEN            6
#define OAL_PMKID_LEN               16
#define OAL_WPA_KEY_LEN             32
#define OAL_WPA_SEQ_LEN             16
#define OAL_WLAN_SA_QUERY_TR_ID_LEN 2
#define OAL_MAX_FT_ALL_LEN          518 /* MD:5 FT:257 RSN:256 */

#define OAL_NET_DEV_COUNTRY_CODE_LEN 4

#define OAL_STA_TX_MCS_LEN      16
#define OAL_STA_RX_MCS_LEN      16
#define OAL_STA_CHIP_VENDOR_LEN 3

#define OAL_VAP_CCA_LEN 2

#define OAL_ETH_HAEDWARE_ADDR_LEN 6

#define OAL_DHCP_CLIENT_ADDR_LEN      16
#define OAL_DHCP_SERVER_NAME_LEN      64
#define OAL_DHCP_BOOT_FILE_NAME_LEN   128
#define OAL_DHCP_VERIABLE_LEN_OPTIONS 4

#define OAL_SSID_ARRY_LEN 3
#define MAX_BLACKLIST_NUM 128
/*
 * Byte order/swapping support.
 */
#define OAL_LITTLE_ENDIAN 1234
#define OAL_BIG_ENDIAN    4321
#define OAL_BYTE_ORDER    OAL_BIG_ENDIAN

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && defined(_PRE_WLAN_FEATURE_DFR)
/* hi1102  �����ں�ע��˿�28 NETLINK_HISI_WIFI_MSG�ѱ�ռ�ã����Դ˴�ʹ��δʹ�õ�27 NETLINK_HISI_WIFI_PMF */
#define NETLINK_DEV_ERROR 27
#endif

/* 2.10 IP�궨�� */
#define WLAN_DSCP_PRI_SHIFT      2
#define WLAN_IP_PRI_SHIFT        5
#define WLAN_IPV6_PRIORITY_MASK  0x0FF00000
#define WLAN_IPV6_PRIORITY_SHIFT 20

/* 2.12 LLC SNAP�궨�� */
#define LLC_UI                    0x3
#define SNAP_LLC_FRAME_LEN        8
#define SNAP_LLC_LSAP             0xaa
#define SNAP_BRIDGE_SPANNING_TREE 0x42
#define SNAP_IBM_LAN_MANAGEMENT   0xf5
#define SNAP_RFC1042_ORGCODE_0    0x00
#define SNAP_RFC1042_ORGCODE_1    0x00
#define SNAP_RFC1042_ORGCODE_2    0x00
#define SNAP_BTEP_ORGCODE_0       0x00
#define SNAP_BTEP_ORGCODE_1       0x00
#define SNAP_BTEP_ORGCODE_2       0xf8

/* 2.13 ETHER�궨�� */
#define ETHER_ADDR_LEN 6 /* length of an Ethernet address */
#define ETHER_TYPE_LEN 2 /* length of the Ethernet type field */
#define ETHER_CRC_LEN  4 /* length of the Ethernet CRC */
#define ETHER_HDR_LEN  14
#define ETHER_MAX_LEN  1518
#define ETHER_MTU      (ETHER_MAX_LEN - ETHER_HDR_LEN - ETHER_CRC_LEN)

OAL_STATIC OAL_INLINE uint8_t a2x(const char c)
{
    if (c >= '0' && c <= '9') {
        return (uint8_t)oal_atoi(&c);
    }
    if (c >= 'a' && c <= 'f') {
        return (uint8_t)0xa + (uint8_t)(c - 'a');
    }
    if (c >= 'A' && c <= 'F') {
        return (uint8_t)0xa + (uint8_t)(c - 'A');
    }
    return 0;
}

/* convert a string,which length is 18, to a macaddress data type. */
#define MAC_STR_LEN            18
#define copy_str2mac(mac, str) \
    do {                                                                      \
        uint8_t i;                                                          \
        for (i = 0; i < OAL_MAC_ADDR_LEN; i++) {                              \
            (mac)[i] = (uint8_t)(a2x((str)[i * 3]) << 4) + a2x((str)[i * 3 + 1]); \
        }                                                                     \
    } while (0)

/* ipͷ��Э�������ֶε�ƫ�� */
#define IP_PROTOCOL_TYPE_OFFSET 9
#define IP_HDR_LEN              20

/* CCMP�����ֽ��� */
#define WLAN_CCMP_ENCRYP_LEN 16
/* CCMP256�����ֽ��� */
#define WLAN_CCMP256_GCMP_ENCRYP_LEN 24

/* is address mcast? */
#define ether_is_multicast(_a) ((uint8_t)(*(_a))&0x01)

/* is address bcast? */
#define ether_is_broadcast(_a) \
    ((_a)[0] == 0xff &&        \
        (_a)[1] == 0xff &&     \
        (_a)[2] == 0xff &&     \
        (_a)[3] == 0xff &&     \
        (_a)[4] == 0xff &&     \
        (_a)[5] == 0xff)

#define ether_is_all_zero(_a) \
    ((_a)[0] == 0x00 &&       \
        (_a)[1] == 0x00 &&    \
        (_a)[2] == 0x00 &&    \
        (_a)[3] == 0x00 &&    \
        (_a)[4] == 0x00 &&    \
        (_a)[5] == 0x00)

#define ether_is_ipv4_multicast(_a) (((_a)[0]) == 0x01 && \
                                     ((_a)[1]) == 0x00 && \
                                     ((_a)[2]) == 0x5e)

#define WLAN_DATA_VIP_TID WLAN_TIDNO_BCAST

/* wiphy  */
#define IEEE80211_HT_MCS_MASK_LEN 10

/* ICMP codes for neighbour discovery messages */
#define OAL_NDISC_ROUTER_SOLICITATION     133
#define OAL_NDISC_ROUTER_ADVERTISEMENT    134
#define OAL_NDISC_NEIGHBOUR_SOLICITATION  135
#define OAL_NDISC_NEIGHBOUR_ADVERTISEMENT 136
#define OAL_NDISC_REDIRECT                137

#define OAL_ND_OPT_TARGET_LL_ADDR 2
#define OAL_ND_OPT_SOURCE_LL_ADDR 1
#define OAL_IPV6_ADDR_ANY         0x0000U
#define OAL_IPV6_ADDR_MULTICAST   0x0002U
#define OAL_IPV6_MAC_ADDR_LEN     16


#define OAL_IPV4_ADDR_SIZE   4
#define OAL_IPV6_ADDR_SIZE   16
#define OAL_IP_ADDR_MAX_SIZE OAL_IPV6_ADDR_SIZE

/* IPv4�ಥ��Χ: 224.0.0.0--239.255.255.255 */
#define oal_ipv4_is_multicast(_a) ((uint8_t)((_a)[0]) >= 224 && ((uint8_t)((_a)[0]) <= 239))

/* IPv4�������ַ�ж�: 224.0.0.0��224.0.0.255Ϊ�������ַ */
#define OAL_IPV4_PERMANET_GROUP_ADDR    0x000000E0
#define oal_ipv4_is_permanent_group(_a) ((((_a)&0x00FFFFFF) ^ OAL_IPV4_PERMANET_GROUP_ADDR) == 0)

/* IPv6�鲥��ַ: FFXX:XXXX:XXXX:XXXX:XXXX:XXXX:XXXX:XXXX(��һ���ֽ�ȫһ) */
#define oal_ipv6_is_multicast(_a) ((uint8_t)((_a)[0]) == 0xff)
/* ipv6 �鲥mac��ַ */
#define ether_is_ipv6_multicast(_a) (((_a)[0]) == 0x33 && ((_a)[1]) == 0x33)
/* IPv6δָ����ַ: ::/128 ,�õ�ַ�����ڽӿڻ�û�б�����IPv6��ַʱ�������ڵ�
   ͨѶ��ΪԴ��ַ,�������ظ���ַ���DAD�л����. */
#define oal_ipv6_is_unspecified_addr(_a) \
    ((_a)[0] == 0x00 && (_a)[1] == 0x00 && (_a)[2] == 0x00 && (_a)[3] == 0x00 &&   \
     (_a)[4] == 0x00 && (_a)[5] == 0x00 && (_a)[6] == 0x00 && (_a)[7] == 0x00 &&   \
     (_a)[8] == 0x00 && (_a)[9] == 0x00 && (_a)[10] == 0x00 && (_a)[11] == 0x00 && \
     (_a)[12] == 0x00 && (_a)[13] == 0x00 && (_a)[14] == 0x00 && (_a)[15] == 0x00)

/* IPv6��·���ص�ַ: ���10λֵΪ1111111010, ����:FE80:XXXX:XXXX:XXXX:XXXX:XXXX:XXXX:XXXX  */
#define oal_ipv6_is_link_local_addr(_a) (((_a)[0] == 0xFE) && ((_a)[1] >> 6 == 2))

/* IGMP record type */
#define MAC_IGMP_QUERY_TYPE    0x11
#define MAC_IGMPV1_REPORT_TYPE 0x12
#define MAC_IGMPV2_REPORT_TYPE 0x16
#define MAC_IGMPV2_LEAVE_TYPE  0x17
#define MAC_IGMPV3_REPORT_TYPE 0x22

/* Is packet type is either leave or report */
#define is_igmp_report_leave_packet(type) ( \
    ((type) == MAC_IGMPV1_REPORT_TYPE) || \
    ((type) == MAC_IGMPV2_REPORT_TYPE) || \
    ((type) == MAC_IGMPV2_LEAVE_TYPE) || \
    ((type) == MAC_IGMPV3_REPORT_TYPE))

/* V3 group record types [grec_type] */
#define IGMPV3_MODE_IS_INCLUDE   1
#define IGMPV3_MODE_IS_EXCLUDE   2
#define IGMPV3_CHANGE_TO_INCLUDE 3
#define IGMPV3_CHANGE_TO_EXCLUDE 4
#define IGMPV3_ALLOW_NEW_SOURCES 5
#define IGMPV3_BLOCK_OLD_SOURCES 6

/* Is packet type is either leave or report */
#define is_igmpv3_mode(type) ( \
    ((type) == IGMPV3_MODE_IS_INCLUDE) || \
    ((type) == IGMPV3_MODE_IS_EXCLUDE) || \
    ((type) == IGMPV3_CHANGE_TO_INCLUDE) || \
    ((type) == IGMPV3_CHANGE_TO_EXCLUDE) || \
    ((type) == IGMPV3_ALLOW_NEW_SOURCES) || \
    ((type) == IGMPV3_BLOCK_OLD_SOURCES))

/* IGMP record type */
#define MLD_QUERY_TYPE    130
#define MLDV1_REPORT_TYPE 131
#define MLDV1_DONE_TYPE   132
#define MLDV2_REPORT_TYPE 143

/* Is packet type is either leave or report */
#define is_mld_report_leave_packet(type) ( \
    ((type) == MLDV1_REPORT_TYPE) || \
    ((type) == MLDV1_DONE_TYPE) || \
    ((type) == MLDV2_REPORT_TYPE))
/* MLD V2 group record types [grec_type] */
#define MLDV2_MODE_IS_INCLUDE   1
#define MLDV2_MODE_IS_EXCLUDE   2
#define MLDV2_CHANGE_TO_INCLUDE 3
#define MLDV2_CHANGE_TO_EXCLUDE 4
#define MLDV2_ALLOW_NEW_SOURCES 5
#define MLDV2_BLOCK_OLD_SOURCES 6

#define is_mldv2_mode(type) ( \
    ((type) == MLDV2_MODE_IS_INCLUDE) || \
    ((type) == MLDV2_MODE_IS_EXCLUDE) || \
    ((type) == MLDV2_CHANGE_TO_INCLUDE) || \
    ((type) == MLDV2_CHANGE_TO_EXCLUDE) || \
    ((type) == MLDV2_ALLOW_NEW_SOURCES) || \
    ((type) == MLDV2_BLOCK_OLD_SOURCES))

/* Calculate the group record length */
#define igmpv3_grp_rec_len(x) (8 + (((x)->us_grec_nsrcs + (x)->uc_grec_auxwords) << 2))
#define mldv2_grp_rec_len(x)  (OAL_SIZEOF(mac_mld_v2_group_record_stru) + \
                               ((x)->us_grec_srcaddr_num << 4) + \
                               (x)->uc_grec_auxwords)

/* Probe Rsp APP IE���ȳ�����ֵ������֡netbuf���ô�� */
#define OAL_MGMT_NETBUF_APP_PROBE_RSP_IE_LEN_LIMIT 450

#ifdef _PRE_WLAN_FEATURE_SPECIAL_PKT_LOG
/* dhcp */
#define DHCP_CHADDR_LEN 16
#define SERVERNAME_LEN  64
#define BOOTFILE_LEN    128

/* DHCP message type */
#define DHCP_DISCOVER 1
#define DHCP_OFFER    2
#define DHCP_REQUEST  3
#define DHCP_ACK      5
#define DHCP_NAK      6

#define DHO_PAD         0
#define DHO_IPADDRESS   50
#define DHO_MESSAGETYPE 53
#define DHO_SERVERID    54
#define DHO_END         255

#define dns_get_qr_from_flag(flag)     ((uint8_t)(((flag & 0x8000U) > 0) ? 1 : 0))
#define dns_get_opcode_from_flag(flag) ((uint8_t)((flag & 0x7400U) >> 11))
#define dns_get_rcode_from_flag(flag)  ((uint8_t)(flag & 0x000fU))

#define DNS_MAX_DOMAIN_LEN 100

#define DHCP_SERVER_PORT 67
#define DHCP_CLIENT_PORT 68
#define DNS_SERVER_PORT  53
#endif

/*
 * ö����  : oal_mem_state_enum_uint8
 * Э����:
 * ö��˵��: �ڴ��״̬
 */
typedef enum {
    OAL_MEM_STATE_FREE = 0, /* ���ڴ���� */
    OAL_MEM_STATE_ALLOC,    /* ���ڴ��ѷ��� */

    OAL_MEM_STATE_BUTT
} oal_mem_state_enum;
typedef uint8_t oal_mem_state_enum_uint8;

/* ö�ٶ��� */
/* ���²����ֲ���ϵͳ */
/* �ں��·���ɨ������ */
typedef enum {
    OAL_PASSIVE_SCAN = 0,
    OAL_ACTIVE_SCAN = 1,

    OAL_SCAN_BUTT
} oal_scan_enum;
typedef uint8_t oal_scan_enum_uint8;

/* �ں��·���ɨ��Ƶ�� */
typedef enum {
    OAL_SCAN_2G_BAND = 1,
    OAL_SCAN_5G_BAND = 2,
    OAL_SCAN_ALL_BAND = 3,

    OAL_SCAN_BAND_BUTT
} oal_scan_band_enum;
typedef uint8_t oal_scan_band_enum_uint8;

typedef enum {
    OAL_IEEE80211_MLME_AUTH = 0,        /* MLME�·���֤֡������� */
    OAL_IEEE80211_MLME_ASSOC_RSP = 1,   /* MLME�·�����֡������� */
    OAL_IEEE80211_MLME_REASSOC_RSP = 2, /* MLME�·��ع���֡������� */
    OAL_IEEE80211_MLME_NUM
} en_mlme_type_enum;
typedef uint8_t en_mlme_type_enum_uint8;

/* hostapd �·�˽������ */
enum HWIFI_IOCTL_CMD {
    /*
     * IOCTL_CMD����ʼֵ��0�޸�Ϊ0x8EE0���޸�ԭ��51 WiFiģ���������dhdutil֮�������ģ�鹲��ͬһ��ioctlͨ����
     * ��51�����ö��ֵ��0��ʼ������ģ���·���ioctl����Ҳ������0��ʼ���ⲿ�֣������ͻ�ͬʱ���鲥�����Լ���ģ���WiFiģ�飬
     * �Ӷ���WiFiģ��Ĺ��ܲ���Ӱ�졣���Խ�51 WiFiģ�������ö��ֵ������0x8EE0�𣬱���������ģ�������Ӱ�졣
     */
    HWIFI_IOCTL_CMD_GET_STA_ASSOC_REQ_IE = 0x8EE0, /* get sta assocate request ie */
    HWIFI_IOCTL_CMD_SET_AP_AUTH_ALG,               /* set auth alg to driver */
    HWIFI_IOCTL_CMD_SET_COUNTRY,                   /* ���ù����� */
    HWIFI_IOCTL_CMD_SET_SSID,                      /* ����ssid */
    HWIFI_IOCTL_CMD_SET_MAX_USER,                  /* ��������û��� */
    HWIFI_IOCTL_CMD_SET_FREQ,                      /* ����Ƶ�� */
    HWIFI_IOCTL_CMD_SET_WPS_IE,                    /* ����AP WPS ��ϢԪ�� */
    HWIFI_IOCTL_CMD_PRIV_CONNECT,                  /* linux-2.6.30 sta����connect */
    HWIFI_IOCTL_CMD_PRIV_DISCONNECT,               /* linux-2.6.30 sta����disconnect */
    HWIFI_IOCTL_CMD_SET_FRAG,                      /* ���÷�Ƭ����ֵ */
    HWIFI_IOCTL_CMD_SET_RTS,                       /* ����RTS ����ֵ */
    HWIFI_IOCTL_CMD_PRIV_KICK_USER,          /* AP�޳��û� */
    HWIFI_IOCTL_CMD_SET_VENDOR_IE,           /* AP ���˽��IE�ӿڣ�ԭΪHWIFI_IOCTL_CMD_SET_OKC_IE����hilink����okc ieר�� */
    HWIFI_IOCTL_CMD_SET_WHITE_LST_SSIDHIDEN, /* ����hlink������ */
    HWIFI_IOCTL_CMD_FBT_SCAN,                /* ������ֹͣhilink fbt���� */
    HWIFI_IOCTL_CMD_GET_ALL_STA_INFO,        /* ��ȡ�����ѹ���STA��·��Ϣ */
    HWIFI_IOCTL_CMD_GET_STA_INFO_BY_MAC,     /* ��ȡָ���ѹ���STA��·��Ϣ */
    HWIFI_IOCTL_CMD_GET_CUR_CHANNEL,         /* ��ȡ�����ŵ� */
    HWIFI_IOCTL_CMD_SET_SCAN_STAY_TIME,      /* ����ɨ��ʱ�����ŵ�פ��ʱ��������ŵ�פ��ʱ�� */
    HWIFI_IOCTL_CMD_SET_BEACON, /* hostapdֻ���ü��ܲ���˽�нӿ� */
    HWIFI_IOCTL_CMD_SET_MLME, /* ����MLME��������֤�������� */
    HWIFI_IOCTL_CMD_GET_NEIGHB_INFO,      /* ��ȡ�ھ�APɨ����Ϣ */
    HWIFI_IOCTL_CMD_GET_HW_STAT,          /* ��ȡӲ������ͳ�� */
    HWIFI_IOCTL_CMD_GET_WME_STAT,         /* ��ȡWME����ͳ�� */
    HWIFI_IOCTL_CMD_GET_STA_11V_ABILITY,  /* ��ȡָ��STA��11v������Ϣ */
    HWIFI_IOCTL_CMD_11V_CHANGE_AP,        /* ֪ͨsta�л���ָ��ap */
    HWIFI_IOCTL_CMD_GET_STA_11K_ABILITY,  /* ��ȡָ��STA��11v������Ϣ */
    HWIFI_IOCTL_CMD_SET_STA_BCN_REQUEST,  /* ֪ͨSTA���ϱ�beacon��Ϣ */
    HWIFI_IOCTL_CMD_GET_SNOOP_TABLE,      /* ��ȡ�鲥�鼰���Աmac */
    HWIFI_IOCTL_CMD_GET_ALL_STA_INFO_EXT, /* ��ȡ�����ѹ���STA��·��Ϣ�������������� */
    HWIFI_IOCTL_CMD_GET_VAP_WDS_INFO,     /* ��ȡWDS VAP�ڵ���Ϣ */
    HWIFI_IOCTL_CMD_GET_STA_11H_ABILITY,  /* ��ȡָ��STA��11h������Ϣ */
    HWIFI_IOCTL_CMD_GET_STA_11R_ABILITY,  /* ��ȡָ��STA��11r������Ϣ */
    HWIFI_IOCTL_CMD_GET_TX_DELAY_AC,      /* ��ȡָ������AC�ķ���ʱ����Ϣ */
    HWIFI_IOCTL_CMD_GET_CAR_INFO,         /* ��ȡָ��device�����car����������Ϣ */
    HWIFI_IOCTL_CMD_GET_BLKWHTLST,        /* ��ȡ�ڰ����� */

    HWIFI_IOCTL_CMD_NUM
};

enum APP_IE_TYPE {
    OAL_APP_BEACON_IE = 0,
    OAL_APP_PROBE_REQ_IE = 1,
    OAL_APP_PROBE_RSP_IE = 2,
    OAL_APP_ASSOC_REQ_IE = 3,
    OAL_APP_ASSOC_RSP_IE = 4,
    OAL_APP_FT_IE = 5,
    OAL_APP_REASSOC_REQ_IE = 6,
    OAL_APP_VENDOR_IE = 9,

    OAL_APP_IE_NUM
};
typedef uint8_t en_app_ie_type_uint8;

typedef enum _wlan_net_queue_type_ {
    WLAN_HI_QUEUE = 0,
    WLAN_NORMAL_QUEUE,

    WLAN_TCP_DATA_QUEUE,
    WLAN_TCP_ACK_QUEUE,

    WLAN_UDP_BK_QUEUE,
    WLAN_UDP_BE_QUEUE,
    WLAN_UDP_VI_QUEUE,
    WLAN_UDP_VO_QUEUE,

    WLAN_NET_QUEUE_BUTT
} wlan_net_queue_type;

/* net_device ioctl�ṹ�嶨�� */
/* hostapd/wpa_supplicant �·�����ϢԪ�ؽṹ */
/* �ýṹΪ�¼��ڴ�ش�С�������hostapd/wpa_supplicant�·���ie ��Ϣ */
/* ע��: �����ṹ�峤��Ϊ�¼��ڴ�ش�С������¼��ڴ�����޸ģ�����Ҫͬ���޸�app ���ݽṹ */
struct oal_app_ie {
    uint32_t ul_ie_len;
    en_app_ie_type_uint8 en_app_ie_type;
    uint8_t auc_rsv[3];
    /* auc_ie �б�����ϢԪ�أ����� = (�¼��ڴ�ش�С - ��������) */
    uint8_t auc_ie[WLAN_WPS_IE_MAX_SIZE];
};
typedef struct oal_app_ie oal_app_ie_stru;

/*
 * wal �㵽hmac�ഫ��ʹ�ø����ݣ�1103 WLAN_WPS_IE_MAX_SIZE���ݵ�608�ֽڣ������¼����д�С��
 * wal��hmac ie data����ָ�봫��
 */
struct oal_w2h_app_ie {
    uint32_t ul_ie_len;
    en_app_ie_type_uint8 en_app_ie_type;
    uint8_t auc_rsv[3];
    uint8_t *puc_data_ie;
    uint32_t ul_delivery_time; /* wal to hmac���¼���ʼ��ʱ��� */
} __OAL_DECLARE_PACKED;
typedef struct oal_w2h_app_ie oal_w2h_app_ie_stru;

struct oal_mlme_ie {
    en_mlme_type_enum_uint8 en_mlme_type; /* MLME���� */
    uint8_t uc_seq;                     /* ��֤֡���к� */
    uint16_t us_status;                 /* ״̬�� */
    uint8_t auc_macaddr[OAL_MAC_ADDR_LEN];
    uint16_t us_ie_len;
    uint8_t auc_ie[WLAN_WPS_IE_MAX_SIZE];
};
typedef struct oal_mlme_ie oal_mlme_ie_stru;

struct oal_w2h_mlme_ie {
    en_mlme_type_enum_uint8 en_mlme_type; /* MLME���� */
    uint8_t uc_seq;                     /* ��֤֡���к� */
    uint16_t us_status;                 /* ״̬�� */
    uint8_t auc_macaddr[OAL_MAC_ADDR_LEN];
    uint16_t us_ie_len;
    uint8_t *puc_data_ie;
};
typedef struct oal_w2h_mlme_ie oal_w2h_mlme_ie_stru;

/*
 * Structure of the IP frame
 */
struct oal_ip_header {
#if (_PRE_LITTLE_CPU_ENDIAN == _PRE_CPU_ENDIAN) /* LITTLE_ENDIAN */
    uint8_t us_ihl : 4,
              uc_version_ihl : 4;
#else
    uint8_t uc_version_ihl : 4,
              us_ihl : 4;
#endif
    /* liuming modifed proxyst end */
    uint8_t uc_tos;
    uint16_t us_tot_len;
    uint16_t us_id;
    uint16_t us_frag_off;
    uint8_t uc_ttl;
    uint8_t uc_protocol;
    uint16_t us_check;
    uint32_t ul_saddr;
    uint32_t ul_daddr;
    /* The options start here. */
} __OAL_DECLARE_PACKED;
typedef struct oal_ip_header oal_ip_header_stru;

typedef struct {
    uint16_t us_sport;
    uint16_t us_dport;
    uint32_t ul_seqnum;
    uint32_t ul_acknum;
    uint8_t uc_offset;
    uint8_t uc_flags;
    uint16_t us_window;
    uint16_t us_check;
    uint16_t us_urgent;
} oal_tcp_header_stru;

typedef struct {
    uint16_t source;
    uint16_t dest;
    uint16_t len;
    uint16_t check;
} oal_udp_header_stru;

/*
 *    Header in on cable format
 */
struct mac_igmp_header {
    uint8_t uc_type;
    uint8_t uc_code; /* For newer IGMP */
    uint16_t us_csum;
    uint32_t ul_group;

} __OAL_DECLARE_PACKED;
typedef struct mac_igmp_header mac_igmp_header_stru;

/*
 *  Group record format
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |  Record Type  |  Aux Data Len |     Number of Sources (N)     |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |                       Multicast Address                       |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |                       Source Address [1]                      |
 *    +-                                                             -+
 *    |                       Source Address [2]                      |
 *    +-                                                             -+
 *    .                               .                               .
 *    .                               .                               .
 *    .                               .                               .
 *    +-                                                             -+
 *    |                       Source Address [N]                      |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |                                                               |
 *    .                                                               .
 *    .                         Auxiliary Data                        .
 *    .                                                               .
 *    |                                                               |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct mac_igmp_v3_grec {
    uint8_t uc_grec_type;
    uint8_t uc_grec_auxwords;
    uint16_t us_grec_nsrcs;
    uint32_t ul_grec_group_ip;

} __OAL_DECLARE_PACKED;
typedef struct mac_igmp_v3_grec mac_igmp_v3_grec_stru;

/*
 * IGMPv3 report format
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |  Type = 0x22  |    Reserved   |           Checksum            |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |           Reserved            |  Number of Group Records (M)  |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   .                                                               .
 *   .                        Group Record [1]                       .
 *   .                                                               .
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   .                                                               .
 *   .                        Group Record [2]                       .
 *   .                                                               .
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                               .                               |
 *   .                               .                               .
 *   |                               .                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   .                                                               .
 *   .                        Group Record [M]                       .
 *   .                                                               .
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct mac_igmp_v3_report {
    uint8_t uc_type;
    uint8_t uc_resv1;
    uint16_t us_csum;
    uint16_t us_resv2;
    uint16_t us_ngrec;

} __OAL_DECLARE_PACKED;
typedef struct mac_igmp_v3_report mac_igmp_v3_report_stru;

struct mac_mld_v1_head {
    uint8_t uc_type;
    uint8_t uc_code;
    uint16_t us_check_sum;
    uint16_t us_max_response_delay;
    uint16_t us_reserved;
    uint8_t auc_group_ip[OAL_IPV6_ADDR_SIZE];
} __OAL_DECLARE_PACKED;
typedef struct mac_mld_v1_head mac_mld_v1_head_stru;

struct mac_mld_v2_report {
    uint8_t uc_type;
    uint8_t uc_code;
    uint16_t us_check_sum;
    uint16_t us_reserved;
    uint16_t us_group_address_num;

} __OAL_DECLARE_PACKED;
typedef struct mac_mld_v2_report mac_mld_v2_report_stru;

struct mac_mld_v2_group_record {
    uint8_t uc_grec_type;
    uint8_t uc_grec_auxwords;                  // �������ݳ���
    uint16_t us_grec_srcaddr_num;              //  ԭ��ַ����
    uint8_t auc_group_ip[OAL_IPV6_ADDR_SIZE];  // �鲥���ַ

} __OAL_DECLARE_PACKED;
typedef struct mac_mld_v2_group_record mac_mld_v2_group_record_stru;

struct mac_vlan_tag {
    uint16_t us_tpid;           // tag   ID
    uint16_t bit_user_pri : 3,  // ֡�����ȼ�
               bit_CFI : 1,
               bit_vlan_id : 12;  // �����õ�VLAN ID
} __OAL_DECLARE_PACKED;
typedef struct mac_vlan_tag mac_vlan_tag_stru;

/* WIN32��linux���ýṹ��  */
typedef struct {
    uint8_t uc_type;
    uint8_t uc_len;
    uint8_t uc_addr[OAL_ETH_HAEDWARE_ADDR_LEN]; /* hardware address */
} oal_eth_icmp6_lladdr_stru;

typedef struct {
    uint8_t op;                                     /* packet opcode type */
    uint8_t htype;                                  /* hardware addr type */
    uint8_t hlen;                                   /* hardware addr length */
    uint8_t hops;                                   /* gateway hops */
    uint32_t xid;                                   /* transaction ID */
    uint16_t secs;                                  /* seconds since boot began */
    uint16_t flags;                                 /* flags */
    uint32_t ciaddr;                                /* client IP address */
    uint32_t yiaddr;                                /* 'your' IP address */
    uint32_t siaddr;                                /* server IP address */
    uint32_t giaddr;                                /* gateway IP address */
    uint8_t chaddr[OAL_DHCP_CLIENT_ADDR_LEN];       /* client hardware address */
    uint8_t sname[OAL_DHCP_SERVER_NAME_LEN];        /* server host name */
    uint8_t file[OAL_DHCP_BOOT_FILE_NAME_LEN];      /* boot file name */
    uint8_t options[OAL_DHCP_VERIABLE_LEN_OPTIONS]; /* variable-length options field */
} oal_dhcp_packet_stru;

#ifdef _PRE_WLAN_FEATURE_SPECIAL_PKT_LOG
typedef struct {
    uint16_t id;      /* transaction id */
    uint16_t flags;   /* message future */
    uint16_t qdcount; /* question record count */
    uint16_t ancount; /* answer record count */
    uint16_t nscount; /* authority record count */
    uint16_t arcount; /* additional record count */
} oal_dns_hdr_stru;

typedef enum {
    OAL_NS_Q_REQUEST = 0,  /* request */
    OAL_NS_Q_RESPONSE = 1, /* response */
} oal_ns_qrcode;

typedef enum {
    OAL_NS_O_QUERY = 0,  /* Standard query. */
    OAL_NS_O_IQUERY = 1, /* Inverse query (deprecated/unsupported). */
} oal_ns_opcode;

/*
 * Currently defined response codes.
 */
typedef enum {
    OAL_NS_R_NOERROR = 0, /* No error occurred. */
} oal_ns_rcode;

typedef enum {
    OAL_NS_T_INVALID = 0, /* Cookie. */
    OAL_NS_T_A = 1,       /* Host address. */
} oal_ns_type;
#endif

/* ����ƽ̨ͨ�ýṹ�� */
typedef struct {
    uint8_t auc_ssid[OAL_IEEE80211_MAX_SSID_LEN]; /* ssid array */
    uint8_t uc_ssid_len;                          /* length of the array */
    uint8_t auc_arry[OAL_SSID_ARRY_LEN];
} oal_ssids_stru;

struct hostap_sta_link_info {
    uint8_t addr[OAL_MAC_ADDR_LEN];
    uint8_t rx_rssi;     /* 0 ~ 100 ,0xffΪ��Чֵ */
    uint8_t tx_pwr;      /* 0 ~ 100 */
    uint32_t rx_rate;    /* avr nego rate in kpbs */
    uint32_t tx_rate;    /* avr nego rate in kpbs */
    uint32_t rx_minrate; /* min nego rx rate in last duration, clean to 0 when app read over */
    uint32_t rx_maxrate; /* max nego rx rate in last duration, clean to 0 when app read over */
    uint32_t tx_minrate; /* min nego tx rate in last duration, clean to 0 when app read over */
    uint32_t tx_maxrate; /* max nego tx rate in last duration, clean to 0 when app read over */
    uint64_t rx_bytes;
    uint64_t tx_bytes;
    uint32_t tx_frames_rty;  /* tx frame retry cnt */
    uint32_t tx_frames_all;  /* tx total frame cnt */
    uint32_t tx_frames_fail; /* tx fail */
    uint32_t SNR;            /* snr */
};

struct hostap_all_sta_link_info {
    unsigned long buf_cnt; /* input: sta_info count provided  */
    unsigned long sta_cnt; /* outpu: how many sta really */
    unsigned int cur_channel;
    struct hostap_sta_link_info *sta_info; /* output */
};
typedef struct hostap_sta_link_info oal_net_sta_link_info_stru;
typedef struct hostap_all_sta_link_info oal_net_all_sta_link_info_stru;

/* sta����������Ϣ�ṹ�� */
struct hostap_sta_link_info_ext {
    uint8_t uc_auth_st;              /* ��֤״̬ */
    oal_bool_enum_uint8 en_band;       /* ����Ƶ�� */
    oal_bool_enum_uint8 en_wmm_switch; /* wmm�Ƿ�ʹ�� */
    uint8_t uc_ps_st;                /* ����״̬ */
    uint8_t uc_sta_num;              /* �ռ����� */
    uint8_t uc_work_mode;            /* Э��ģʽ */
    int8_t c_noise;                  /* �ڵ������ֵ */
    uint8_t auc_resv[1];
    uint32_t ul_associated_time; /* �û������ӵ�ʱ�� */
};

struct hostap_all_sta_link_info_ext {
    unsigned long buf_cnt; /* input: sta_info count provided  */
    unsigned long sta_cnt; /* outpu: how many sta really */
    unsigned int cur_channel;
    struct hostap_sta_link_info *sta_info;
    struct hostap_sta_link_info_ext *sta_info_ext;
};
typedef struct hostap_sta_link_info_ext oal_net_sta_link_info_ext_stru;
typedef struct hostap_all_sta_link_info_ext oal_net_all_sta_link_info_ext_stru;

/* lint -e958 */ /* �����ֽڶ������ */
/* RRM ENABLED CAP��ϢԪ�ؽṹ�� */
struct oal_rrm_enabled_cap_ie {
    uint8_t bit_link_cap : 1,                  /* bit0: Link Measurement capability enabled */
              bit_neighbor_rpt_cap : 1,          /* bit1: Neighbor Report capability enabled */
              bit_parallel_cap : 1,              /* bit2: Parallel Measurements capability enabled */
              bit_repeat_cap : 1,                /* bit3: Repeated Measurements capability enabled */
              bit_bcn_passive_cap : 1,           /* bit4: Beacon Passive Measurements capability enabled */
              bit_bcn_active_cap : 1,            /* bit5: Beacon Active Measurements capability enabled */
              bit_bcn_table_cap : 1,             /* bit6: Beacon Table Measurements capability enabled */
              bit_bcn_meas_rpt_cond_cap : 1;     /* bit7: Beacon Measurement Reporting Conditions capability enabled */
    uint8_t bit_frame_cap : 1,                 /* bit8: Frame Measurement capability enabled */
              bit_chn_load_cap : 1,              /* bit9: Channel Load Measurement capability enabled */
              bit_noise_histogram_cap : 1,       /* bit10: Noise Histogram Measurement capability enabled */
              bit_stat_cap : 1,                  /* bit11: Statistics Measurement capability enabled */
              bit_lci_cap : 1,                   /* bit12: LCI Measurement capability enabled */
              bit_lci_azimuth_cap : 1,           /* bit13: LCI Azimuth capability enabled */
              bit_tsc_cap : 1,                   /* bit14: Transmit Stream/Category Measurement capability enabled */
              bit_triggered_tsc_cap : 1;         /* bit15: Triggered  Transmit Stream/Category Measurement capability enabled */
    uint8_t bit_ap_chn_rpt_cap : 1,            /* bit16: AP Channel Report capability enabled */
              bit_rm_mib_cap : 1,                      /* bit17: RM MIB capability enabled */
              bit_oper_chn_max_meas_duration : 3,      /* bit18-20: Operating Channel Max Measurement Duration */
              bit_non_oper_chn_max_meas_duration : 3;  /* bit21-23: Non-operating Channel Max Measurement Durationg */
    uint8_t bit_meas_pilot_cap : 3,                  /* bit24-26: Measurement Pilot capability */
              bit_meas_pilot_trans_info_cap : 1,       /* bit27: Measurement Pilot Transmission Information capability enabled */
              bit_neighbor_rpt_tsf_offset_cap : 1,     /* bit28: Neighbor Report TSF Offset capability enabled */
              bit_rcpi_cap : 1,                        /* bit29: RCPI Measurement capability enabled */
              bit_rsni_cap : 1,                        /* bit30: RSNI Measurement capability enabled */
              bit_bss_avg_access_dly : 1;              /* bit31: BSS Average Access Delay capability enabled */
    uint8_t bit_avail_admission_capacity_cap : 1,    /* bit32: BSS Available Admission Capacity capability enabled */
              bit_antenna_cap : 1,                     /* bit33: Antenna capability enabled */
              bit_ftm_range_report_cap : 1,            /* bit34: FTM range report capability enabled */
              bit_rsv : 5;                             /* bit35-39: Reserved */
} __OAL_DECLARE_PACKED;
typedef struct oal_rrm_enabled_cap_ie oal_rrm_enabled_cap_ie_stru;
/*lint +e958*/
/* �ھ�AP�б��BSS������Ϣ�ṹ�� */
struct oal_bssid_infomation {
    uint8_t bit_ap_reachability : 2,           /* AP�Ŀɵ����� */
              bit_security : 1,                  /* ��AP�ļ��ܹ����뵱ǰ�����Ƿ�һ�� */
              bit_key_scope : 1,                 /* ��AP����֤��Ϣ�Ƿ��뵱ǰ�ϱ�һֱ */
              bit_spectrum_mgmt : 1,             /* ����λ: ֧��Ƶ�׹��� */ /* ����λ�ֶ���beacon����һ�� */
              bit_qos : 1,                       /* ����λ: ֧��QOS */
              bit_apsd : 1,                      /* ����λ: ֧��APSD */
              bit_radio_meas : 1;                /* ����λ: �������� */
    uint8_t bit_delay_block_ack : 1,           /* ����λ: �����ӳ�Ӧ�� */
              bit_immediate_block_ack : 1,       /* ����λ: ��������Ӧ�� */
              bit_mobility_domain : 1,           /* ��AP��beacon֡���Ƿ���MDE������˴��ϱ�һ�� */
              bit_high_throughput : 1,           /* ��AP��beacon֡���Ƿ��и�������Ԫ�أ�����˴��ϱ�һ�� */
              bit_resv1 : 4;                     /* Ԥ�� */
    uint8_t bit_resv2;
    uint8_t bit_resv3;
} __OAL_DECLARE_PACKED;
typedef struct oal_bssid_infomation oal_bssid_infomation_stru;

typedef struct {
    uint8_t auc_sta_mac[OAL_MAC_ADDR_LEN]; /* Ҫ��ȡ��sta��mac */
    oal_bool_enum_uint8 en_support_11h;      /* ����sta�Ƿ�֧��11h��Ϊ���� */
    uint8_t reserve;
} oal_hilink_get_sta_11h_ability;

typedef struct {
    uint8_t auc_sta_mac[OAL_MAC_ADDR_LEN]; /* Ҫ��ȡ��sta��mac */
    oal_bool_enum_uint8 en_support_11r;      /* ����sta�Ƿ�֧��11r��Ϊ���� */
    uint8_t reserve;
} oal_hilink_get_sta_11r_ability;

typedef struct {
    uint8_t uc_blkwhtlst_cnt; /* �ڰ��������� */
    uint8_t uc_mode;          /* �ڰ�����ģʽ */
    uint8_t auc_resv[2];
    uint8_t auc_blkwhtlst_mac[MAX_BLACKLIST_NUM][OAL_MAC_ADDR_LEN];
} oal_blkwhtlst_stru;

/* net_device ioctl�ṹ�嶨�� */
typedef struct oal_net_dev_ioctl_data_tag {
    int32_t l_cmd; /* ����� */
    union {
        struct {
            uint8_t auc_mac[OAL_MAC_ADDR_LEN];
            uint8_t auc_rsv[2];
            uint32_t ul_buf_size; /* �û��ռ�ie �����С */
            uint8_t *puc_buf;     /* �û��ռ�ie �����ַ */
        } assoc_req_ie;             /* AP ģʽ�����ڻ�ȡSTA ��������ie ��Ϣ */

        struct {
            uint32_t auth_alg;
        } auth_params;

        struct {
            uint8_t auc_country_code[OAL_NET_DEV_COUNTRY_CODE_LEN];
        } country_code;

        uint8_t ssid[OAL_IEEE80211_MAX_SSID_LEN + OAL_IEEE80211_EXTRA_SSID_LEN];
        uint32_t ul_vap_max_user;

        struct {
            int32_t l_freq;
            int32_t l_channel;
            int32_t l_ht_enabled;
            int32_t l_sec_channel_offset;
            int32_t l_vht_enabled;
            int32_t l_center_freq1;
            int32_t l_center_freq2;
            int32_t l_bandwidth;
        } freq;

        oal_app_ie_stru st_app_ie; /* beacon ie,index 0; proberesp ie index 1; assocresp ie index 2 */

        struct {
            int32_t l_freq;    /* ap����Ƶ�Σ���linux-2.6.34�ں��ж��岻ͬ */
            uint32_t ssid_len; /* SSID ���� */
            uint32_t ie_len;

            uint8_t *puc_ie;
            OAL_CONST uint8_t *puc_ssid;  /* ����������AP SSID  */
            OAL_CONST uint8_t *puc_bssid; /* ����������AP BSSID  */

            uint8_t en_privacy;                          /* �Ƿ���ܱ�־ */
            oal_nl80211_auth_type_enum_uint8 en_auth_type; /* ��֤���ͣ�OPEN or SHARE-KEY */

            uint8_t uc_wep_key_len;         /* WEP KEY���� */
            uint8_t uc_wep_key_index;       /* WEP KEY���� */
            OAL_CONST uint8_t *puc_wep_key; /* WEP KEY��Կ */

            oal_cfg80211_crypto_settings_stru st_crypto; /* ��Կ�׼���Ϣ */
        } cfg80211_connect_params;
        struct {
            uint8_t auc_mac[OAL_MAC_ADDR_LEN];
            uint16_t us_reason_code; /* ȥ���� reason code */
        } kick_user_params;

        oal_net_all_sta_link_info_stru all_sta_link_info;

        oal_hilink_get_sta_11h_ability fbt_get_sta_11h_ability;

        int32_t l_frag; /* ��Ƭ����ֵ */
        int32_t l_rts;  /* RTS ����ֵ */
        oal_net_all_sta_link_info_ext_stru all_sta_link_info_ext;

        oal_blkwhtlst_stru st_blkwhtlst;

    } pri_data;
} oal_net_dev_ioctl_data_stru;

/* �������� */
extern oal_bool_enum_uint8 oal_netbuf_is_dhcp_port(oal_udp_header_stru *pst_udp_hdr);
extern oal_bool_enum_uint8 oal_netbuf_is_nd(oal_ipv6hdr_stru *pst_ipv6hdr);
extern oal_bool_enum_uint8 oal_netbuf_is_dhcp6(oal_ipv6hdr_stru *pst_ether_hdr);

extern oal_bool_enum_uint8 oal_netbuf_is_tcp_ack6(oal_ipv6hdr_stru *pst_ipv6hdr);
extern uint16_t oal_netbuf_select_queue(oal_netbuf_stru *pst_buf);
extern oal_bool_enum_uint8 oal_netbuf_is_tcp_ack(oal_ip_header_stru *pst_ip_hdr);
extern oal_bool_enum_uint8 oal_netbuf_is_icmp(oal_ip_header_stru *pst_ip_hdr);
extern int32_t dev_netlink_send(uint8_t *data, int data_len);
extern int32_t init_dev_excp_handler(void);
extern void deinit_dev_excp_handler(void);
#endif /* end of oal_net.h */
