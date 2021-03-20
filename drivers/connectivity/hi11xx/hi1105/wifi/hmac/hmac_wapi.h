
#ifndef __HMAC_WAPI_H__
#define __HMAC_WAPI_H__

/* 1 ͷ�ļ����� */
#include "oal_net.h"
#include "mac_resource.h"
#include "hmac_vap.h"
#include "hmac_user.h"
#include "securec.h"
#include "securectype.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WAPI_H
#ifdef _PRE_WLAN_FEATURE_WAPI
/* 2 �궨�� */
#define WAPI_UCAST_INC 2                  /* ���ͻ��߽��յ���֡,pn�Ĳ���ֵ */
#define WAPI_BCAST_INC 1                  /* ���ͻ��߽����鲥֡,pn�Ĳ���ֵ */
#define WAPI_WAI_TYPE  (uint16_t)0x88B4 /* wapi����̫���� */

#define WAPI_BCAST_KEY_TYPE 1
#define WAPI_UCAST_KEY_TYPE 0

#define SMS4_MIC_LEN (uint8_t)16 /* SMS4���MIC�ĳ��� */

#define SMS4_PN_LEN           16 /* wapi pn�ĳ��� */
#define SMS4_KEY_IDX          1  /* WAPIͷ�� keyidxռ1���ֽ� */
#define SMS4_WAPI_HDR_RESERVE 1  /* WAPIͷ�б����ֶ� */
#define HMAC_WAPI_HDR_LEN     (uint8_t)(SMS4_PN_LEN + SMS4_KEY_IDX + SMS4_WAPI_HDR_RESERVE)
#define WAPI_PDU_LEN          2  /* wapiͷ�У�wapi pdu len�ֶ���ռ�ֽ��� */
#define SMS4_PADDING_LEN      16 /* mic data����16�ֽڶ��� */

#define SMS4_MIC_PART1_QOS_LEN    48 /* ����Э�飬�����qos�ֶΣ�mic��һ����16�ֽڶ����ĳ��� */
#define SMS4_MIC_PART1_NO_QOS_LEN 32 /* ����Э�飬���û��qos�ֶΣ�mic��һ����16�ֽڶ����ĳ��� */

#define WAPI_IE_VERSION        1  /* wapi��version */
#define WAPI_IE_VER_SIZE       2  /* wapi ver-ie ��ռ�ֽ��� */
#define WAPI_IE_SUIT_TYPE_SIZE 1  /* suit type size */
#define WAPI_IE_WAPICAP_SIZE   2  /* wapi cap�ֶ���ռ�ֽ��� */
#define WAPI_IE_BKIDCNT_SIZE   2  /* wapi bkid���ֶ���ռ�ֽ��� */
#define WAPI_IE_BKID_SIZE      16 /* һ��bkid��ռ�ֽ��� */
#define WAPI_IE_OUI_SIZE       3  /* wapi oui�ֽ��� */
#define WAPI_IE_SMS4           1  /* wapi��������Ϊsms4 */
#define WAPI_IE_SUITCNT_SIZE   2  /* wapi suit count��ռ�ֽ��� */
/* wapi key len */
#define WAPI_PORT_FLAG(pst_wapi) ((pst_wapi)->uc_port_valid)

#define WAPI_IS_WORK(pst_hmac_vap) ((pst_hmac_vap)->uc_wapi)

#ifdef _PRE_WAPI_DEBUG
#define WAPI_TX_DROP_INC(pst_wapi)       ((pst_wapi)->st_debug.ultx_ucast_drop++)
#define WAPI_TX_WAI_INC(pst_wapi)        ((pst_wapi)->st_debug.ultx_wai++)
#define WAPI_TX_PORT_VALID(pst_wapi)     ((pst_wapi)->st_debug.ultx_port_valid++)
#define WAPI_RX_PORT_VALID(pst_wapi)     ((pst_wapi)->st_debug.ulrx_port_valid++)
#define WAPI_RX_IDX_ERR(pst_wapi)        ((pst_wapi)->st_debug.ulrx_idx_err++)
#define WAPI_RX_NETBUF_LEN_ERR(pst_wapi) ((pst_wapi)->st_debug.ulrx_netbuff_len_err++)
#define WAPI_RX_IDX_UPDATE_ERR(pst_wapi) ((pst_wapi)->st_debug.ulrx_idx_update_err++)
#define WAPI_RX_KEY_EN_ERR(pst_wapi)     ((pst_wapi)->st_debug.ulrx_key_en_err++)

#define WAPI_RX_PN_ODD_ERR(pst_wapi, pn)                                                    \
    do {                                                                                    \
        (pst_wapi)->st_debug.ulrx_pn_odd_err++;                                             \
        if (EOK != memcpy_s((pst_wapi)->st_debug.aucrx_pn, WAPI_PN_LEN, pn, WAPI_PN_LEN)) { \
            oam_error_log0(0, OAM_SF_ANY, "WAPI_RX_PN_ODD_ERR::memcpy fail!");              \
        }                                                                                   \
    } while (0)

#define WAPI_RX_PN_REPLAY_ERR(pst_wapi, pn)                                                 \
    do {                                                                                    \
        (pst_wapi)->st_debug.ulrx_pn_replay_err++;                                          \
        if (EOK != memcpy_s((pst_wapi)->st_debug.aucrx_pn, WAPI_PN_LEN, pn, WAPI_PN_LEN)) { \
            oam_error_log0(0, OAM_SF_ANY, "WAPI_RX_PN_REPLAY_ERR::memcpy fail!");           \
        }                                                                                   \
    } while (0)

#define WAPI_RX_MEMALLOC_ERR(pst_wapi) ((pst_wapi)->st_debug.ulrx_memalloc_err++)
#define WAPI_RX_MIC_ERR(pst_wapi)      ((pst_wapi)->st_debug.ulrx_mic_calc_fail++)
#define WAPI_RX_DECRYPT_OK(pst_wapi)   ((pst_wapi)->st_debug.ulrx_decrypt_ok++)

#define WAPI_TX_MEMALLOC_ERR(pst_wapi) ((pst_wapi)->st_debug.ultx_memalloc_err++)
#define WAPI_TX_MIC_ERR(pst_wapi)      ((pst_wapi)->st_debug.ultx_mic_calc_fail++)
#define WAPI_TX_ENCRYPT_OK(pst_wapi)   ((pst_wapi)->st_debug.ultx_encrypt_ok++)

#else
#define WAPI_TX_DROP_INC(pst_wapi)
#define WAPI_TX_WAI_INC(pst_wapi)
#define WAPI_TX_PORT_VALID(pst_wapi)
#define WAPI_RX_PORT_VALID(pst_wapi)
#define WAPI_RX_IDX_ERR(pst_wapi)
#define WAPI_RX_NETBUF_LEN_ERR(pst_wapi)
#define WAPI_RX_IDX_UPDATE_ERR(pst_wapi)
#define WAPI_RX_KEY_EN_ERR(pst_wapi)
#define WAPI_RX_PN_ODD_ERR(pst_wapi, pn)
#define WAPI_RX_PN_REPLAY_ERR(pst_wapi, pn)
#define WAPI_RX_MEMALLOC_ERR(pst_wapi)
#define WAPI_RX_MIC_ERR(pst_wapi)
#define WAPI_RX_DECRYPT_OK(pst_wapi)
#define WAPI_TX_MEMALLOC_ERR(pst_wapi)
#define WAPI_TX_MIC_ERR(pst_wapi)
#define WAPI_TX_WAI_DROP_INC(pst_wapi)
#define WAPI_TX_ENCRYPT_OK(pst_wapi)

#endif /* #ifdef WAPI_DEBUG_MODE */

/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
void hmac_wapi_deinit(hmac_wapi_stru *pst_wapi);
uint32_t hmac_wapi_init(hmac_wapi_stru *pst_wapi, uint8_t uc_pairwise);
#ifdef _PRE_WAPI_DEBUG
uint32_t hmac_wapi_display_info(mac_vap_stru *pst_mac_vap, uint16_t us_usr_idx);
#endif
uint32_t hmac_wapi_add_key(hmac_wapi_stru *pst_wapi,
                                        uint8_t uc_key_index, uint8_t *puc_key);
void hmac_wapi_reset_port(hmac_wapi_stru *pst_wapi);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_wapi.h */



