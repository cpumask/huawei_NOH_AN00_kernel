

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 头文件包含 */
#include "oal_ext_if.h"
#include "oal_types.h"
#include "hmac_wapi_sms4.h"
#include "hmac_wapi_wpi.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WAPI_WPI_C

#ifdef _PRE_WLAN_FEATURE_WAPI

/* 2 全局变量定义 */
/* 3 函数实现 */

uint32_t hmac_wpi_encrypt(uint8_t *puc_iv, uint8_t *puc_bufin, uint32_t ul_buflen,
                                uint8_t *puc_key, uint8_t *puc_bufout)
{
    uint32_t aul_iv_out[4];
    uint32_t *pul_in = OAL_PTR_NULL;
    uint32_t *pul_out = OAL_PTR_NULL;
    uint8_t *puc_out = OAL_PTR_NULL;
    uint8_t *puc_in = OAL_PTR_NULL;
    uint32_t ul_counter;
    uint32_t ul_comp;
    uint32_t ul_loop;
    uint32_t aul_pr_keyin[32] = { 0 };

    if (ul_buflen < 1) {
#ifdef WAPI_DEBUG_MODE
        g_stMacDriverStats.ulSms4OfbInParmInvalid++;
#endif
        return OAL_FAIL;
    }

    hmac_sms4_keyext(puc_key, aul_pr_keyin, OAL_SIZEOF(aul_pr_keyin));

    ul_counter = ul_buflen / 16;
    ul_comp = ul_buflen % 16;

    /* get the iv */
    hmac_sms4_crypt(puc_iv, (uint8_t *)aul_iv_out, aul_pr_keyin, sizeof(aul_pr_keyin));
    pul_in = (uint32_t *)puc_bufin;
    pul_out = (uint32_t *)puc_bufout;

    for (ul_loop = 0; ul_loop < ul_counter; ul_loop++) {
        pul_out[0] = pul_in[0] ^ aul_iv_out[0];
        pul_out[1] = pul_in[1] ^ aul_iv_out[1];
        pul_out[2] = pul_in[2] ^ aul_iv_out[2];
        pul_out[3] = pul_in[3] ^ aul_iv_out[3];
        hmac_sms4_crypt((uint8_t *)aul_iv_out, (uint8_t *)aul_iv_out, aul_pr_keyin, sizeof(aul_pr_keyin));
        pul_in += 4;
        pul_out += 4;
    }

    puc_in = (uint8_t *)pul_in;
    puc_out = (uint8_t *)pul_out;
    puc_iv = (uint8_t *)aul_iv_out;

    for (ul_loop = 0; ul_loop < ul_comp; ul_loop++) {
        puc_out[ul_loop] = puc_in[ul_loop] ^ puc_iv[ul_loop];
    }

    return OAL_SUCC;
}


uint32_t hmac_wpi_decrypt(uint8_t *puc_iv, uint8_t *puc_bufin, uint32_t ul_buflen,
                                uint8_t *puc_key, uint8_t *puc_bufout)
{
    return hmac_wpi_encrypt(puc_iv, puc_bufin, ul_buflen, puc_key, puc_bufout);
}


void hmac_wpi_swap_pn(uint8_t *puc_pn, uint8_t uc_len)
{
    uint8_t uc_index;
    uint8_t uc_temp;
    uint8_t uc_len_tmp;

    uc_len_tmp = uc_len / 2;
    for (uc_index = 0; uc_index < uc_len_tmp; uc_index++) {
        uc_temp = puc_pn[uc_index];
        puc_pn[uc_index] = puc_pn[uc_len - 1 - uc_index];
        puc_pn[uc_len - 1 - uc_index] = uc_temp;
    }
}


uint32_t hmac_wpi_pmac(uint8_t *puc_iv, uint8_t *puc_buf, uint32_t ul_pamclen,
                             uint8_t *puc_key, uint8_t *puc_mic, uint8_t uc_mic_len)
{
    uint32_t aul_mic_tmp[4];
    uint32_t ul_loop;
    uint32_t *pul_in = OAL_PTR_NULL;
    uint32_t aul_pr_mac_keyin[32] = { 0 };

    if ((ul_pamclen < 1) || (ul_pamclen > 4096)) {
        return OAL_FAIL;
    }

    if (uc_mic_len < OAL_SIZEOF(aul_mic_tmp)) {
        return OAL_FAIL;
    }

    hmac_sms4_keyext(puc_key, aul_pr_mac_keyin, OAL_SIZEOF(aul_pr_mac_keyin));
    pul_in = (uint32_t *)puc_buf;
    hmac_sms4_crypt(puc_iv, (uint8_t *)aul_mic_tmp, aul_pr_mac_keyin, sizeof(aul_pr_mac_keyin));

    for (ul_loop = 0; ul_loop < ul_pamclen; ul_loop++) {
        aul_mic_tmp[0] ^= pul_in[0];
        aul_mic_tmp[1] ^= pul_in[1];
        aul_mic_tmp[2] ^= pul_in[2];
        aul_mic_tmp[3] ^= pul_in[3];
        pul_in += 4;
        hmac_sms4_crypt((uint8_t *)aul_mic_tmp, (uint8_t *)aul_mic_tmp,
                            aul_pr_mac_keyin, sizeof(aul_pr_mac_keyin));
    }

    pul_in = (uint32_t *)puc_mic;
    pul_in[0] = aul_mic_tmp[0];
    pul_in[1] = aul_mic_tmp[1];
    pul_in[2] = aul_mic_tmp[2];
    pul_in[3] = aul_mic_tmp[3];

    return OAL_SUCC;
}

#endif /* #ifdef _PRE_WLAN_FEATURE_WAPI */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

