

#ifndef __OAL_LINUX_CFG80211_H__
#define __OAL_LINUX_CFG80211_H__

/* ����ͷ�ļ����� */
#include <net/genetlink.h>
#include <net/cfg80211.h>
#include <linux/nl80211.h>

/* �궨�� */
/* hostapd��supplicant�¼��ϱ���Ҫ�õ��� */
#define OAL_NLMSG_GOODSIZE            NLMSG_GOODSIZE
#define OAL_ETH_ALEN_SIZE             ETH_ALEN
#define OAL_NLMSG_DEFAULT_SIZE        NLMSG_DEFAULT_SIZE
#define OAL_IEEE80211_MIN_ACTION_SIZE IEEE80211_MIN_ACTION_SIZE

#define oal_nla_put_u32_m(skb, attrtype, value)     NLA_PUT_U32(skb, attrtype, value)
#define oal_nla_put_m(skb, attrtype, attrlen, data) NLA_PUT(skb, attrtype, attrlen, data)
#define oal_nla_put_u16_m(skb, attrtype, value)     NLA_PUT_U16(skb, attrtype, value)
#define oal_nla_put_u8(skb, attrtype, value)      NLA_PUT_U8(skb, attrtype, value)
#define oal_nla_put_flag(skb, attrtype)           NLA_PUT_FLAG(skb, attrtype)

typedef enum rate_info_flags oal_rate_info_flags;

/*
 * struct cfg80211_external_auth_params - Trigger External authentication.
 *
 * Commonly used across the external auth request and event interfaces.
 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
/* do nothing */
#else
struct cfg80211_external_auth_params {
    oal_nl80211_external_auth_action action;
    uint8_t bssid[OAL_ETH_ALEN_SIZE];
    oal_cfg80211_ssid_stru ssid;
    unsigned int key_mgmt_suite;
    uint16_t status;
};
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)) */
typedef struct cfg80211_external_auth_params oal_cfg80211_external_auth_stru;

/* �������� */
extern void cfg80211_drv_mss_result(struct net_device *dev, gfp_t gfp, const u8 *buf, size_t len);
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
extern void cfg80211_drv_tas_result(struct net_device *dev, gfp_t gfp, const u8 *buf, size_t len);
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
extern void cfg80211_drv_hid2d_seqnum_result(struct net_device *dev, gfp_t gfp, const u8 *buf, size_t len);
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
extern int cfg80211_external_auth_request(struct net_device *netdev,
                                          struct cfg80211_external_auth_params *params,
                                          gfp_t gfp);
#endif

#endif /* end of oal_cfg80211.h */
