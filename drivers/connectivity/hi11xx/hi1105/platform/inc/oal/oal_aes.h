

#ifndef __OAL_AES_H__
#define __OAL_AES_H__

/* ����ͷ�ļ����� */
#include "oal_schedule.h"
#include "oal_util.h"
#include "oal_types.h"
#include "oal_net.h"

/* �궨�� */
#define OAL_MMIE_IPN_LEN   6
#define OAL_MMIE_MIC_LEN   8
#define OAL_AES_BLOCK_SIZE 16
#define OAL_AES_BLOCK_LEN  16 /* AES?��?��?��3��?�� */

#define OAL_ETH_ALEN 6 /* ��?��?��???����3��?�� */

#define OAL_AES_KEYSIZE_128 16
#define OAL_AES_KEYSIZE_192 24
#define OAL_AES_KEYSIZE_256 32

#define OAL_BIP_AAD_LEN       20 /* BIP ADD3��?�� */
#define OAL_AES_MAX_KEYLENGTH 60

#define OAL_AES_CTX_PN_LEN       6
#define OAL_AES_RCO_TAB_LEN      10
#define OAL_AES_CRYPTO_ARRY_SIZE 4   /* ��Կ����� */
#define OAL_AES_CRYPTO_KEY_SIZE  256 /* һ����Կ���� */

#define OAL_AES_MAX_KEYLENGTH_U32 (OAL_AES_MAX_KEYLENGTH / OAL_SIZEOF(uint32_t))
#define OAL_MANAGEMENT_FRAME_LEN  292
#define OAL_MGMT_FRAME_MMIE_LEN   18

#define OAL_AES_FRAME_HEAD_LEN 24 /* ֡ͷ���� */

#define OAL_EID_MMIE           76
#define OAL_IE_HDR_LEN         2
#define OAL_MAX_IGTK_KEY_INDEX 5
#define OAL_NUM_IGTK           2

#define oal_host_to_le16(n) n
#define oal_host_to_le32(n) n
#define oal_le32_to_host(n) n

#define OAL_CMAC_TLEN 8  /* CMAC TLen = 64 bits (8 octets) */
#define OAL_AAD_LEN   20 /* BIP ADD3��?�� */

/* STRUCT���� */
/* AES�㷨��Կ�ṹ */
struct oal_aes_key_stru {
    uint32_t ul_key_enc[OAL_AES_MAX_KEYLENGTH];
    uint32_t ul_key_dec[OAL_AES_MAX_KEYLENGTH];
    uint32_t ul_key_length;
} __OAL_DECLARE_PACKED;
typedef struct oal_aes_key_stru oal_aes_key_stru;

struct oal_aes_ctx_stru {
    oal_aes_key_stru key;
    uint8_t pn[OAL_AES_CTX_PN_LEN];
    uint8_t key_idx;
    uint8_t resv[1]; /* crypto_buf�ĵ�ַ����4�ֽڶ��룬�㷨Ҫ�� */
    uint8_t crypto_buf[OAL_AES_BLOCK_LEN * 6];
} __OAL_DECLARE_PACKED;
typedef struct oal_aes_ctx_stru oal_aes_ctx_stru;

struct oal_mmie_stru {
    uint8_t element_id;
    uint8_t length;
    uint16_t key_id;
    uint8_t sequence_number[OAL_MMIE_IPN_LEN];
    uint8_t mic[OAL_MMIE_MIC_LEN];
} __OAL_DECLARE_PACKED;
typedef struct oal_mmie_stru oal_mmie_stru;

/* �������� */
extern uint32_t oal_aes_expand_key(oal_aes_key_stru *pst_aes_key,
                                         OAL_CONST uint8_t *in_key,
                                         unsigned int key_len);
extern uint32_t oal_aes_encrypt(oal_aes_key_stru *pst_aes_key,
                                      uint8_t *puc_ciphertext,
                                      OAL_CONST uint8_t *puc_plaintext);
extern uint32_t oal_aes_decrypt(oal_aes_key_stru *pst_aes_key,
                                      uint8_t *puc_plaintext,
                                      OAL_CONST uint8_t *puc_ciphertext);

extern uint32_t oal_crypto_bip_demic(uint8_t uc_igtk_keyid, uint8_t *pst_igtk_key,
                                           uint8_t *pst_igtk_seq, oal_netbuf_stru *pst_netbuf);

#endif /* end of oal_aes.h */
