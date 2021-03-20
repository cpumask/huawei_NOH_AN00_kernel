

#ifndef __HMAC_CRYPTO_FRAME_H__
#define __HMAC_CRYPTO_FRAME_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "mac_vap.h"

/* 2 宏定义 */
#define michael_block(l, r)  do { \
        (r) ^= rotl((l), 17);   \
        (l) += (r);             \
        (r) ^= xswap((l));      \
        (l) += (r);             \
        (r) ^= rotl((l), 3);    \
        (l) += (r);             \
        (r) ^= rotr((l), 2);    \
        (l) += (r);             \
    } while (0)
#define IEEE80211_WEP_MICLEN 8 /* trailing MIC */

#define IEEE80211_FC1_DIR_NODS   0x00 /* STA->STA */
#define IEEE80211_FC1_DIR_TODS   0x01 /* STA->AP  */
#define IEEE80211_FC1_DIR_FROMDS 0x02 /* AP ->STA */
#define IEEE80211_FC1_DIR_DSTODS 0x03 /* AP ->AP  */

#define IEEE80211_NON_QOS_SEQ     16 /* index for non-QoS (including management) sequence number space */
#define IEEE80211_FC0_TYPE_MASK   0x0c
#define IEEE80211_FC0_SUBTYPE_QOS 0x80
#define IEEE80211_FC0_TYPE_DATA   0x08

#define WEP_IV_FIELD_SIZE 4 /* wep IV field size */
#define EXT_IV_FIELD_SIZE 4 /* ext IV field size */

/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
OAL_STATIC OAL_INLINE uint32_t rotl(uint32_t val, int32_t bits)
{
    return (val << (uint32_t)bits) | (val >> (uint32_t)(32 - bits));
}

OAL_STATIC OAL_INLINE uint32_t rotr(uint32_t val, int32_t bits)
{
    return (val >> (uint32_t)bits) | (val << (uint32_t)(32 - bits));
}

OAL_STATIC OAL_INLINE uint32_t xswap(uint32_t val)
{
    return ((val & 0x00ff00ff) << 8) | ((val & 0xff00ff00) >> 8);
}

OAL_STATIC OAL_INLINE uint32_t get_le32_split(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

OAL_STATIC OAL_INLINE uint32_t get_le32(const unsigned char *p)
{
    return get_le32_split(p[0], p[1], p[2], p[3]);
}
OAL_STATIC OAL_INLINE void put_le32(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
}

uint32_t hmac_crypto_tkip_enmic(wlan_priv_key_param_stru *pst_key, oal_netbuf_stru *pst_netbuf);
uint32_t hmac_crypto_tkip_demic(wlan_priv_key_param_stru *pst_key, oal_netbuf_stru *pst_netbuf);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_crypto_tkip.h */
