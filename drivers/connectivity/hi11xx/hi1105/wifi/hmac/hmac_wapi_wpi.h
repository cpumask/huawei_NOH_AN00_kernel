
#ifndef __HMAC_WAPI_WPI_H__
#define __HMAC_WAPI_WPI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 头文件包含 */
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WAPI_WPI_H
#ifdef _PRE_WLAN_FEATURE_WAPI
/* 2 宏定义 */
/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
uint32_t hmac_wpi_encrypt(uint8_t *puc_iv, uint8_t *puc_bufin, uint32_t ul_buflen,
                                       uint8_t *puc_key, uint8_t *puc_bufout);

uint32_t hmac_wpi_decrypt(uint8_t *puc_iv, uint8_t *puc_bufin, uint32_t ul_buflen,
                                       uint8_t *puc_key, uint8_t *puc_bufout);

void hmac_wpi_swap_pn(uint8_t *puc_pn, uint8_t uc_len);

uint32_t hmac_wpi_pmac(uint8_t *puc_iv, uint8_t *puc_buf, uint32_t ul_pamclen,
                                    uint8_t *puc_key, uint8_t *puc_mic, uint8_t uc_mic_len);

#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_wapi_wpi.h */



