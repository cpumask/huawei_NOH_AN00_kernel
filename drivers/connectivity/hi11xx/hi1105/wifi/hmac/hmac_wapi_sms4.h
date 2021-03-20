
#ifndef __HMAC_WAPI_SMS4_H__
#define __HMAC_WAPI_SMS4_H__

/* 1 头文件包含 */
#include "oal_util.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WAPI_SMS4_H
#ifdef _PRE_WLAN_FEATURE_WAPI
/* 2 宏定义 */
#define Rotl(_x, _y)    (((_x) << (_y)) | ((_x) >> (32 - (_y)))) /* 循环左移 */
#define getbyte(_x, _y) (((unsigned char *)&(_x))[_y])              /* 以y为下标，获取x对应的字节值 */

#define ByteSub(_S, _A)                              \
    ((_S)[((uint32_t)(_A)) >> 24 & 0xFF] << 24 ^ \
    (_S)[((uint32_t)(_A)) >> 16 & 0xFF] << 16 ^  \
    (_S)[((uint32_t)(_A)) >> 8 & 0xFF] << 8 ^    \
    (_S)[((uint32_t)(_A)) & 0xFF])

#define L1(_B) ((_B) ^ Rotl(_B, 2) ^ Rotl(_B, 10) ^ Rotl(_B, 18) ^ Rotl(_B, 24))
#define L2(_B) ((_B) ^ Rotl(_B, 13) ^ Rotl(_B, 23))

/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
void hmac_sms4_crypt(uint8_t *puc_input, uint8_t *puc_output,
                                    uint32_t *puc_rk, uint32_t ul_rk_len);
void hmac_sms4_keyext(uint8_t *puc_key, uint32_t *pul_rk, uint32_t ul_rk_len);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_wapi_sms4.h */

