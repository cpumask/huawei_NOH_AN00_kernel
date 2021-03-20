

#ifndef __WLAN_TYPES_H__
#define __WLAN_TYPES_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "platform_spec.h"
#include "oal_ext_if.h"
#include "wlan_cfg_id.h"
#include "wlan_cali_1105.h"
#include "wlan_cali_1106.h"

/* 2 �궨�� */
/* 2.1 �����궨�� */
#define MAC_BYTE_ALIGN_VALUE 4 /* 4�ֽڶ��� */

#define WLAN_MAC_ADDR_LEN          6    /* MAC��ַ���Ⱥ� */
#define WLAN_MAX_FRAME_HEADER_LEN  36   /* ����MAC֡ͷ���ȣ�����֡36������֡Ϊ28 */
#define WLAN_MIN_FRAME_HEADER_LEN  10   /* ack��cts��֡ͷ����Ϊ10 */
#define WLAN_MAX_FRAME_LEN         1600 /* ά���ã���ֹԽ�� */
#define WLAN_MGMT_FRAME_HEADER_LEN 24   /* ����֡��MAC֡ͷ���ȣ�����֡36������֡Ϊ28 */
#define WLAN_IWPRIV_MAX_BUFF_LEN   100  /* iwpriv�ϴ����ַ�����󳤶� */

/* SSID��󳤶�, +1Ϊ\0Ԥ���ռ� */
#define WLAN_SSID_MAX_LEN (32 + 1)

/* 80211MAC֡ͷFC�ֶκ궨�� */
#define WLAN_PROTOCOL_VERSION 0x00 /* Э��汾 */
#define WLAN_FC0_TYPE_MGT     0x00 /* ����֡ */
#define WLAN_FC0_TYPE_CTL     0x04 /* ����֡ */
#define WLAN_FC0_TYPE_DATA    0x08 /* ����֡ */

/* ����֡subtype */
#define WLAN_FC0_SUBTYPE_ASSOC_REQ     0x00
#define WLAN_FC0_SUBTYPE_ASSOC_RSP     0x10
#define WLAN_FC0_SUBTYPE_REASSOC_REQ   0x20
#define WLAN_FC0_SUBTYPE_REASSOC_RSP   0x30
#define WLAN_FC0_SUBTYPE_PROBE_REQ     0x40
#define WLAN_FC0_SUBTYPE_PROBE_RSP     0x50
#define WLAN_FC0_SUBTYPE_BEACON        0x80
#define WLAN_FC0_SUBTYPE_ATIM          0x90
#define WLAN_FC0_SUBTYPE_DISASSOC      0xa0
#define WLAN_FC0_SUBTYPE_AUTH          0xb0
#define WLAN_FC0_SUBTYPE_DEAUTH        0xc0
#define WLAN_FC0_SUBTYPE_ACTION        0xd0
#define WLAN_FC0_SUBTYPE_ACTION_NO_ACK 0xe0

/* ����֡subtype */
#define WLAN_FC0_SUBTYPE_TRIG            0x20
#define WLAN_FC0_SUBTYPE_NDPA            0x50
#define WLAN_FC0_SUBTYPE_Control_Wrapper 0x70 /* For TxBF RC */
#define WLAN_FC0_SUBTYPE_BAR             0x80
#define WLAN_FC0_SUBTYPE_BA              0x90
#define WLAN_FC0_SUBTYPE_BFRP            0x40

#define WLAN_FC0_SUBTYPE_PS_POLL    0xa0
#define WLAN_FC0_SUBTYPE_RTS        0xb0
#define WLAN_FC0_SUBTYPE_CTS        0xc0
#define WLAN_FC0_SUBTYPE_ACK        0xd0
#define WLAN_FC0_SUBTYPE_CF_END     0xe0
#define WLAN_FC0_SUBTYPE_CF_END_ACK 0xf0

/* ����֡subtype */
#define WLAN_FC0_SUBTYPE_DATA          0x00
#define WLAN_FC0_SUBTYPE_CF_ACK        0x10
#define WLAN_FC0_SUBTYPE_CF_POLL       0x20
#define WLAN_FC0_SUBTYPE_CF_ACPL       0x30
#define WLAN_FC0_SUBTYPE_NODATA        0x40
#define WLAN_FC0_SUBTYPE_CFACK         0x50
#define WLAN_FC0_SUBTYPE_CFPOLL        0x60
#define WLAN_FC0_SUBTYPE_CF_ACK_CF_ACK 0x70
#define WLAN_FC0_SUBTYPE_QOS           0x80
#define WLAN_FC0_SUBTYPE_QOS_NULL      0xc0

#define WLAN_FC1_DIR_MASK   0x03
#define WLAN_FC1_DIR_NODS   0x00 /* STA->STA */
#define WLAN_FC1_DIR_TODS   0x01 /* STA->AP  */
#define WLAN_FC1_DIR_FROMDS 0x02 /* AP ->STA */
#define WLAN_FC1_DIR_DSTODS 0x03 /* AP ->AP  */

#define WLAN_FC1_MORE_FRAG 0x04
#define WLAN_FC1_RETRY     0x08
#define WLAN_FC1_PWR_MGT   0x10
#define WLAN_FC1_MORE_DATA 0x20
#define WLAN_FC1_WEP       0x40
#define WLAN_FC1_ORDER     0x80

#define WLAN_HDR_DUR_OFFSET   2  /* duartion�����macͷ���ֽ�ƫ�� */
#define WLAN_HDR_ADDR1_OFFSET 4  /* addr1�����macͷ���ֽ�ƫ�� */
#define WLAN_HDR_ADDR2_OFFSET 10 /* addr1�����macͷ���ֽ�ƫ�� */
#define WLAN_HDR_ADDR3_OFFSET 16 /* addr1�����macͷ���ֽ�ƫ�� */
#define WLAN_HDR_FRAG_OFFSET  22 /* ��Ƭ��������mac���ֽ�ƫ�� */

#define WLAN_REASON_CODE_LEN 2

/* ֡ͷDSλ */
#define WLAN_FRAME_TO_AP   0x0100
#define WLAN_FRAME_FROM_AP 0x0200
/* FCS����(4�ֽ�) */
#define WLAN_HDR_FCS_LENGTH 4

#define WLAN_RANDOM_MAC_OUI_LEN 3 /* ���mac��ַOUI���� */

#define WLAN_MAX_BAR_DATA_LEN 20  /* BAR֡����󳤶� */
#define WLAN_CHTXT_SIZE       128 /* challenge text�ĳ��� */

#define WLAN_SEQ_SHIFT 4

#define WLAN_TB_PPDU_SCHEDULE_TIMEOUT 15
#define WLAN_MAC_FRAME_LEN  1560


/* AMPDU Delimeter����(4�ֽ�) */
#define WLAN_DELIMETER_LENGTH 4

/* ȡ����ֵ */
#define GET_ABS(val) ((val) > 0 ? (val) : -(val))

/* ����������󳤶�: ���㷨���ƿ�ʼ���𣬲�����"alg" */
#define DMAC_ALG_CONFIG_MAX_ARG 7

#define WLAN_CFG_MAX_ARGS_NUM               27
#define WLAN_CALI_CFG_MAX_ARGS_NUM          20
#define WLAN_CALI_CFG_CMD_MAX_LEN           20

/* �ŵ��л����� */
#define WLAN_CHAN_SWITCH_DEFAULT_CNT      6
#define WLAN_CHAN_SWITCH_DETECT_RADAR_CNT 1

/* 5G��Ƶ����Ŀ */
#define WLAN_5G_SUB_BAND_NUM      7

#define WLAN_DIEID_MAX_LEN 40

#define WLAN_AID(AID)       ((AID) & ~0xc000)

#define WLAN_2G_SUB_BAND_NUM      13

#define WLAN_BW_CAP_TO_BANDWIDTH(_bw_cap) \
    ((WLAN_BW_CAP_20M == (_bw_cap)) ? \
    WLAN_BAND_ASSEMBLE_20M : (WLAN_BW_CAP_40M == (_bw_cap)) ? \
    WLAN_BAND_ASSEMBLE_40M : (WLAN_BW_CAP_80M == (_bw_cap)) ? \
    WLAN_BAND_ASSEMBLE_80M : (WLAN_BW_CAP_160M == (_bw_cap)) ? \
    WLAN_BAND_ASSEMBLE_160M : (WLAN_BAND_ASSEMBLE_20M))

#ifdef _PRE_WLAN_FEATURE_160M
#define WLAN_BANDWIDTH_TO_BW_CAP(_bw) \
    ((WLAN_BAND_WIDTH_20M == (_bw)) ? \
    WLAN_BW_CAP_20M : ((WLAN_BAND_WIDTH_40PLUS == (_bw)) || (WLAN_BAND_WIDTH_40MINUS == (_bw))) ? \
    WLAN_BW_CAP_40M : (((_bw) >= WLAN_BAND_WIDTH_80PLUSPLUS) && ((_bw) <= WLAN_BAND_WIDTH_80MINUSMINUS)) ? \
    WLAN_BW_CAP_80M : (((_bw) >= WLAN_BAND_WIDTH_160PLUSPLUSPLUS) && ((_bw) <= WLAN_BAND_WIDTH_160MINUSMINUSMINUS)) ? \
    WLAN_BW_CAP_160M : WLAN_BW_CAP_BUTT)
#else
#define WLAN_BANDWIDTH_TO_BW_CAP(_bw) \
     ((WLAN_BAND_WIDTH_20M == (_bw)) ? WLAN_BW_CAP_20M : \
     ((WLAN_BAND_WIDTH_40PLUS == (_bw)) || (WLAN_BAND_WIDTH_40MINUS == (_bw))) ?  WLAN_BW_CAP_40M : \
     (((_bw) >= WLAN_BAND_WIDTH_80PLUSPLUS) && ((_bw) <= WLAN_BAND_WIDTH_80MINUSMINUS)) ?  WLAN_BW_CAP_80M : \
      WLAN_BW_CAP_BUTT)
#endif

/* kernel oal_nl80211_chan_width convertion */
#define WLAN_BANDWIDTH_TO_IEEE_CHAN_WIDTH(_bw, _ht) \
    ((((_bw) == WLAN_BAND_WIDTH_20M) && (!(_ht))) ? NL80211_CHAN_WIDTH_20_NOHT : \
    (((_bw) == WLAN_BAND_WIDTH_20M) && (_ht)) ? NL80211_CHAN_WIDTH_20 : \
    (((_bw) == WLAN_BAND_WIDTH_40PLUS) || ((_bw) == WLAN_BAND_WIDTH_40MINUS)) ?  NL80211_CHAN_WIDTH_40 : \
    (((_bw) >= WLAN_BAND_WIDTH_80PLUSPLUS) && ((_bw) <= WLAN_BAND_WIDTH_80MINUSMINUS)) ?  NL80211_CHAN_WIDTH_80 : \
    (((_bw) >= WLAN_BAND_WIDTH_160PLUSPLUSPLUS) && ((_bw) <= WLAN_BAND_WIDTH_160MINUSMINUSMINUS)) ?  \
    NL80211_CHAN_WIDTH_160 : WLAN_BW_CAP_BUTT)

#define WLAN_INVALD_VHT_MCS                           0xff
#define WLAN_GET_VHT_MAX_SUPPORT_MCS(_us_vht_mcs_map) \
    ((3 == (_us_vht_mcs_map)) ? \
    WLAN_INVALD_VHT_MCS : (2 == (_us_vht_mcs_map)) ? \
    WLAN_VHT_MCS9 : (1 == (_us_vht_mcs_map)) ? \
    WLAN_VHT_MCS8 : WLAN_VHT_MCS7)

/* 2.2 WME�궨�� */
#define WLAN_WME_AC_TO_TID(_ac) ( \
    ((_ac) == WLAN_WME_AC_VO) ? 6 : ((_ac) == WLAN_WME_AC_VI) ? 5 : ((_ac) == WLAN_WME_AC_BK) ? 1 : 0)

/* ���bitmap */
#define WLAN_WME_AC_TO_ALL_TID(_bitmap, _ac)   \
    do { \
        ((_ac) == WLAN_WME_AC_VO) ? (_bitmap |= (uint8_t)BIT(WLAN_TIDNO_VOICE)) :   \
        ((_ac) == WLAN_WME_AC_VI) ? (_bitmap |= (uint8_t)BIT(WLAN_TIDNO_VIDEO)) :   \
        ((_ac) == WLAN_WME_AC_BE) ? (_bitmap |= (uint8_t)BIT(WLAN_TIDNO_BEST_EFFORT)) : _bitmap; \
    } while (0)

/* ���bitmap */
#define WLAN_WME_AC_CLEAR_ALL_TID(_bitmap, _ac)   \
    do { \
        ((_ac) == WLAN_WME_AC_VO) ? (_bitmap &= (uint8_t)(~(BIT(WLAN_TIDNO_VOICE)))) :   \
        ((_ac) == WLAN_WME_AC_VI) ? (_bitmap &= (uint8_t)(~(BIT(WLAN_TIDNO_VIDEO)))) :   \
        ((_ac) == WLAN_WME_AC_BE) ? (_bitmap &= (uint8_t)(~(BIT(WLAN_TIDNO_BEST_EFFORT)))) :  \
        (_bitmap &= (uint8_t)(~(BIT(WLAN_TIDNO_BACKGROUND)))); \
    } while (0)

#define WLAN_WME_TID_TO_AC(_tid) ( \
    (((_tid) == 0) || ((_tid) == 3)) ? \
    WLAN_WME_AC_BE : (((_tid) == 1) || ((_tid) == 2)) ? \
    WLAN_WME_AC_BK : (((_tid) == 4) || ((_tid) == 5)) ? \
    WLAN_WME_AC_VI : WLAN_WME_AC_VO)

#define WLAN_INVALID_FRAME_TYPE(_uc_frame_type) \
    ((WLAN_FC0_TYPE_DATA != (_uc_frame_type)) \
        && (WLAN_FC0_TYPE_MGT != (_uc_frame_type)) \
        && (WLAN_FC0_TYPE_CTL != (_uc_frame_type)))

/* 2.3 HT�궨�� */
/*
 * 11n: Maximum A-MSDU Length Indicates maximum A-MSDU length.See 9.11.
 * Set to 0 for 3839 octetsSet to 1 for 7935 octets
 */
/* 11AC(9.11): A VHT STA that sets the Maximum MPDU Length in the VHT Capabilities element to indicate 3895 octets
   shall set the Maximum A-MSDU Length in the HT Capabilities element to indicate 3839 octets. A VHT STA
   that sets the Maximum MPDU Length in the VHT Capabilities element to indicate 7991 octets or 11 454 oc-
   tets shall set the Maximum A-MSDU Length in the HT Capabilities element to indicate 7935 octets. */

#define WLAN_AMSDU_FRAME_MAX_LEN_SHORT 3839
#define WLAN_AMSDU_FRAME_MAX_LEN_LONG  7935

/* Maximum A-MSDU Length Indicates maximum A-MSDU length.See 9.11. Set to 0 for 3839 octetsSet to 1 for 7935 octets */
#define WLAN_HT_GET_AMSDU_MAX_LEN(_bit_amsdu_max_length) \
    ((0 == (_bit_amsdu_max_length)) ? WLAN_AMSDU_FRAME_MAX_LEN_SHORT : WLAN_AMSDU_FRAME_MAX_LEN_LONG)

/* RSSIͳ���˲���RSSI��Χ��-128~127, һ�㲻�����127��ô�����Խ�127����ΪMARKER,����ʼֵ */
#define WLAN_RSSI_DUMMY_MARKER 0x7F

/* 2.4 ��ȫ��غ궨�� */
/* �ں�����Ѿ�������ʹ���ں˵ĺ궨�壬��Ҫע���ں˺궨��ֵ�Ƿ����Ԥ��!! */
/* cipher suite selectors */
#ifndef WLAN_CIPHER_SUITE_USE_GROUP
#define WLAN_CIPHER_SUITE_USE_GROUP 0x000FAC00
#endif

#ifndef WLAN_CIPHER_SUITE_WEP40
#define WLAN_CIPHER_SUITE_WEP40 0x000FAC01
#endif

#ifndef WLAN_CIPHER_SUITE_TKIP
#define WLAN_CIPHER_SUITE_TKIP 0x000FAC02
#endif

/* reserved:                0x000FAC03 */
#ifndef WLAN_CIPHER_SUITE_CCMP
#define WLAN_CIPHER_SUITE_CCMP 0x000FAC04
#endif

#ifndef WLAN_CIPHER_SUITE_WEP104
#define WLAN_CIPHER_SUITE_WEP104 0x000FAC05
#endif

#ifndef WLAN_CIPHER_SUITE_AES_CMAC
#define WLAN_CIPHER_SUITE_AES_CMAC 0x000FAC06
#endif

#ifndef WLAN_CIPHER_SUITE_GCMP
#define WLAN_CIPHER_SUITE_GCMP 0x000FAC08
#endif

#ifndef WLAN_CIPHER_SUITE_GCMP_256
#define WLAN_CIPHER_SUITE_GCMP_256 0x000FAC09
#endif

#ifndef WLAN_CIPHER_SUITE_CCMP_256
#define WLAN_CIPHER_SUITE_CCMP_256 0x000FAC0A
#endif

#ifndef WLAN_CIPHER_SUITE_BIP_GMAC_128
#define WLAN_CIPHER_SUITE_BIP_GMAC_128 0x000FAC0B
#endif

#ifndef WLAN_CIPHER_SUITE_BIP_GMAC_256
#define WLAN_CIPHER_SUITE_BIP_GMAC_256 0x000FAC0C
#endif

#ifndef WLAN_CIPHER_SUITE_BIP_CMAC_256
#define WLAN_CIPHER_SUITE_BIP_CMAC_256 0x000FAC0D
#endif

#undef WLAN_CIPHER_SUITE_SMS4
#define WLAN_CIPHER_SUITE_SMS4 0x00147201

/* AKM suite selectors */
#define WITP_WLAN_AKM_SUITE_8021X     0x000FAC01
#define WITP_WLAN_AKM_SUITE_PSK       0x000FAC02
#define WITP_WLAN_AKM_SUITE_WAPI_PSK  0x000FAC04
#define WITP_WLAN_AKM_SUITE_WAPI_CERT 0x000FAC12

#define WLAN_PMKID_LEN        16
#define WLAN_PMKID_CACHE_SIZE 32
#define WLAN_NONCE_LEN        32
#define WLAN_PTK_PREFIX_LEN   22
#define WLAN_GTK_PREFIX_LEN   19
#define WLAN_GTK_DATA_LEN     (NONCE_LEN + WLAN_MAC_ADDR_LEN)
#define WLAN_PTK_DATA_LEN     (2 * NONCE_LEN + 2 * WLAN_MAC_ADDR_LEN)

#define WLAN_KCK_LENGTH          16
#define WLAN_KEK_LENGTH          16
#define WLAN_TEMPORAL_KEY_LENGTH 16
#define WLAN_MIC_KEY_LENGTH      8

#define WLAN_PMK_SIZE 32 /* In Bytes */
#define WLAN_PTK_SIZE 64 /* In Bytes */
#define WLAN_GTK_SIZE 32 /* In Bytes */
#define WLAN_GMK_SIZE 32 /* In Bytes */

#define WLAN_WEP40_KEY_LEN  5
#define WLAN_WEP104_KEY_LEN 13
#define WLAN_TKIP_KEY_LEN   32 /* TKIP KEY length 256 BIT */
#define WLAN_CCMP_KEY_LEN   16 /* CCMP KEY length 128 BIT */
#define WLAN_BIP_KEY_LEN    16 /* BIP KEY length 128 BIT */

#define WLAN_NUM_DOT11WEPDEFAULTKEYVALUE 4
#define WLAN_MAX_WEP_STR_SIZE            27 /* 5 for WEP 40; 13 for WEP 104 */
#define WLAN_WEP_SIZE_OFFSET             0
#define WLAN_WEP_KEY_VALUE_OFFSET        1
#define WLAN_WEP_40_KEY_SIZE             40
#define WLAN_WEP_104_KEY_SIZE            104

#define WLAN_COUNTRY_STR_LEN 3


/* 11i���� */
/* WPA ��Կ���� */
#define WLAN_WPA_KEY_LEN    32
#define WLAN_CIPHER_KEY_LEN 16
#define WLAN_TKIP_MIC_LEN   16
/* WPA ��ų��� */
#define WLAN_WPA_SEQ_LEN 16


#define WLAN_WITP_CAPABILITY_PRIVACY BIT4

#define WLAN_NUM_TK             4
#define WLAN_NUM_IGTK           2
#define WLAN_MAX_IGTK_KEY_INDEX 5
#define WLAN_MAX_WEP_KEY_COUNT  4

/* 2.5 ��������궨�� */
#define WLAN_BIP_REPLAY_FAIL_FLT     BIT0 /* BIP�طŹ������� */
#define WLAN_CCMP_REPLAY_FAIL_FLT    BIT1 /* CCMP�طŹ������� */
#define WLAN_OTHER_CTRL_FRAME_FLT    BIT2 /* direct����֡���� */
#define WLAN_BCMC_MGMT_OTHER_BSS_FLT BIT3 /* ����BSS������鲥����֡���� */
/* ֻ��1151V100��Ҫ���� */
#define WLAN_UCAST_MGMT_OTHER_BSS_FLT BIT4 /* ����BSS����ĵ�������֡���� */

#define WLAN_UCAST_DATA_OTHER_BSS_FLT BIT5 /* ����BSS����ĵ�������֡���� */

/* 2.6 TXBF��غ궨�� */
/* 2.7  TX & RX Chain Macro */
/* RFͨ������� */
/* RF0��Ӧbit0 */
#define WLAN_RF_CHANNEL_ZERO 0
/* RF1��Ӧbit1 */
#define WLAN_RF_CHANNEL_ONE 1
/* RF2��Ӧbit2 */
#define WLAN_RF_CHANNEL_TWO 2
/* RF3��Ӧbit3 */
#define WLAN_RF_CHANNEL_THREE 3
/* �弶���������� */
#define WLAN_ANT_NUM_FOUR 4
#define WLAN_ANT_NUM_THREE 3
#define WLAN_ANT_NUM_TWO 2
/* PHYͨ��ѡ�� */
/* ͨ��0��Ӧbit0 */
#define WLAN_PHY_CHAIN_ZERO_IDX 0
/* ͨ��1��Ӧbit1 */
#define WLAN_PHY_CHAIN_ONE_IDX 1
/* ͨ��2��Ӧbit2 */
#define WLAN_PHY_CHAIN_TWO_IDX 2
/* ͨ��3��Ӧbit3 */
#define WLAN_PHY_CHAIN_THREE_IDX 3

#define WLAN_RF_CHAIN_QUAD (BIT3 | BIT2 | BIT1 | BIT0)
#define WLAN_RF_CHAIN_C0C1C2 (BIT2 | BIT1 | BIT0)
#define WLAN_RF_CHAIN_C0C1C3 (BIT3 | BIT1 | BIT0)
#define WLAN_RF_CHAIN_C0C2C3 (BIT3 | BIT2 | BIT0)
#define WLAN_RF_CHAIN_C1C2C3 (BIT3 | BIT2 | BIT1)
#define WLAN_RF_CHAIN_C0C1 (BIT1 | BIT0)
#define WLAN_RF_CHAIN_C0C2 (BIT2 | BIT0)
#define WLAN_RF_CHAIN_C1C2 (BIT2 | BIT1)
#define WLAN_RF_CHAIN_C0C3 (BIT3 | BIT0)
#define WLAN_RF_CHAIN_C1C3 (BIT3 | BIT1)
#define WLAN_RF_CHAIN_C2C3 (BIT3 | BIT2)
/* ˫������,��Ӧ0305,06��ʹ�� */
#define WLAN_RF_CHAIN_DOUBLE 3
#define WLAN_RF_CHAIN_ONE    2
#define WLAN_RF_CHAIN_ZERO   1

/* ��ͨ������ */
#define WLAN_PHY_CHAIN_QUAD (BIT3 | BIT2 | BIT1 | BIT0)
#define WLAN_TX_CHAIN_QUAD  WLAN_PHY_CHAIN_QUAD
#define WLAN_RX_CHAIN_QUAD  WLAN_PHY_CHAIN_QUAD
/* ��ͨ������ */
/* C0C1C2 */
#define WLAN_PHY_CHAIN_C0C1C2 (BIT2 | BIT1 | BIT0)
#define WLAN_TX_CHAIN_C0C1C2  WLAN_PHY_CHAIN_C0C1C2
#define WLAN_RX_CHAIN_C0C1C2  WLAN_PHY_CHAIN_C0C1C2
/* C0C1C3 */
#define WLAN_PHY_CHAIN_C0C1C3 (BIT3 | BIT1 | BIT0)
#define WLAN_TX_CHAIN_C0C1C3  WLAN_PHY_CHAIN_C0C1C3
#define WLAN_RX_CHAIN_C0C1C3  WLAN_PHY_CHAIN_C0C1C3
/* C0C2C3 */
#define WLAN_PHY_CHAIN_C0C2C3 (BIT3 | BIT2 | BIT0)
#define WLAN_TX_CHAIN_C0C2C3  WLAN_PHY_CHAIN_C0C2C3
#define WLAN_RX_CHAIN_C0C2C3  WLAN_PHY_CHAIN_C0C2C3
/* C1C2C3 */
#define WLAN_PHY_CHAIN_C1C2C3 (BIT3 | BIT2 | BIT1)
#define WLAN_TX_CHAIN_C1C2C3  WLAN_PHY_CHAIN_C1C2C3
#define WLAN_RX_CHAIN_C1C2C3  WLAN_PHY_CHAIN_C1C2C3
/* ��ͨ������ */
/* C0C1 */
#define WLAN_PHY_CHAIN_C0C1 (BIT1 | BIT0)
#define WLAN_TX_CHAIN_C0C1  WLAN_PHY_CHAIN_C0C1
#define WLAN_RX_CHAIN_C0C1  WLAN_PHY_CHAIN_C0C1
/* C0C2 */
#define WLAN_PHY_CHAIN_C0C2 (BIT2 | BIT0)
#define WLAN_TX_CHAIN_C0C2  WLAN_PHY_CHAIN_C0C2
#define WLAN_RX_CHAIN_C0C2  WLAN_PHY_CHAIN_C0C2
/* C1C2 */
#define WLAN_PHY_CHAIN_C1C2 (BIT2 | BIT1)
#define WLAN_TX_CHAIN_C1C2  WLAN_PHY_CHAIN_C1C2
#define WLAN_RX_CHAIN_C1C2  WLAN_PHY_CHAIN_C1C2
/* C0C3 */
#define WLAN_PHY_CHAIN_C0C3 (BIT3 | BIT0)
#define WLAN_TX_CHAIN_C0C3  WLAN_PHY_CHAIN_C0C3
#define WLAN_RX_CHAIN_C0C3  WLAN_PHY_CHAIN_C0C3
/* C1C3 */
#define WLAN_PHY_CHAIN_C1C3 (BIT3 | BIT1)
#define WLAN_TX_CHAIN_C1C3  WLAN_PHY_CHAIN_C1C3
#define WLAN_RX_CHAIN_C1C3  WLAN_PHY_CHAIN_C1C3
/* C2C3 */
#define WLAN_PHY_CHAIN_C2C3 (BIT3 | BIT2)
#define WLAN_TX_CHAIN_C2C3  WLAN_PHY_CHAIN_C2C3
#define WLAN_RX_CHAIN_C2C3  WLAN_PHY_CHAIN_C2C3

/* ˫������,��Ӧ0305,06��ʹ�� */
#define WLAN_PHY_CHAIN_DOUBLE 3
#define WLAN_TX_CHAIN_DOUBLE  WLAN_PHY_CHAIN_DOUBLE
#define WLAN_RX_CHAIN_DOUBLE  WLAN_PHY_CHAIN_DOUBLE

/*  ͨ��0 ����0001 */
#define WLAN_PHY_CHAIN_ZERO BIT0
#define WLAN_TX_CHAIN_ZERO  WLAN_PHY_CHAIN_ZERO
#define WLAN_RX_CHAIN_ZERO  WLAN_PHY_CHAIN_ZERO
/*  ͨ��1 ����0010 */
#define WLAN_PHY_CHAIN_ONE BIT1
#define WLAN_TX_CHAIN_ONE  WLAN_PHY_CHAIN_ONE
#define WLAN_RX_CHAIN_ONE  WLAN_PHY_CHAIN_ONE
/*  ͨ��2 ����0100 */
#define WLAN_PHY_CHAIN_TWO BIT2
#define WLAN_TX_CHAIN_TWO  WLAN_PHY_CHAIN_TWO
#define WLAN_RX_CHAIN_TWO  WLAN_PHY_CHAIN_TWO
/*  ͨ��3 ����1000 */
#define WLAN_PHY_CHAIN_THREE BIT3
#define WLAN_TX_CHAIN_THREE  WLAN_PHY_CHAIN_THREE
#define WLAN_RX_CHAIN_THREE  WLAN_PHY_CHAIN_THREE

#define WLAN_2G_CHANNEL_NUM 14
#define WLAN_5G_CHANNEL_NUM 29
/* wifi 5G 2.4Gȫ���ŵ����� */
#define WLAN_MAX_CHANNEL_NUM (WLAN_2G_CHANNEL_NUM + WLAN_5G_CHANNEL_NUM)

/* PLL0 */
#define WLAN_RF_PLL_ID_0 0
/* PLL1 */
#define WLAN_RF_PLL_ID_1 1
/* 2.8 linkloss */
#define WLAN_LINKLOSS_REPORT        10  /* linklossÿ10�δ�ӡһ��ά�� */
#define WLAN_LINKLOSS_MIN_THRESHOLD 10  /* linkloss������С���ֵ */
#define WLAN_LINKLOSS_MAX_THRESHOLD 250 /* linkloss����������ֵ */

#ifdef _PRE_WLAN_FEATURE_11AX
/* 11AX MCS��ص����� */
#define MAC_MAX_SUP_MCS7_11AX_EACH_NSS    0 /* 11AX���ռ���֧�ֵ����MCS��ţ�֧��0-7 */
#define MAC_MAX_SUP_MCS9_11AX_EACH_NSS    1 /* 11AX���ռ���֧�ֵ����MCS��ţ�֧��0-9 */
#define MAC_MAX_SUP_MCS11_11AX_EACH_NSS   2 /* 11AX���ռ���֧�ֵ����MCS��ţ�֧��0-11 */
#define MAC_MAX_SUP_INVALID_11AX_EACH_NSS 3 /* ��֧�� */

#define MAC_MAX_RATE_SINGLE_NSS_20M_11AX 106 /* 1���ռ���20MHz���������-��axЭ��28.5-HE-MCSs */
#define MAC_MAX_RATE_SINGLE_NSS_40M_11AX 212 /* 1���ռ���40MHz�����Long GI���� */
#define MAC_MAX_RATE_SINGLE_NSS_80M_11AX 212 /* 1���ռ���40MHz�����Long GI���� */

#define MAC_MAX_RATE_DOUBLE_NSS_20M_11AX 211 /* 2���ռ���20MHz�����Long GI���� */
#define MAC_MAX_RATE_DOUBLE_NSS_40M_11AX 423 /* 2���ռ���20MHz�����Long GI���� */
#define MAC_MAX_RATE_DOUBLE_NSS_80M_11AX 869 /* 2���ռ���20MHz�����Long GI���� */

#define MAC_TRIGGER_FRAME_PADDING_DURATION0us  0
#define MAC_TRIGGER_FRAME_PADDING_DURATION8us  1
#define MAC_TRIGGER_FRAME_PADDING_DURATION16us 2
#define MAC_HE_MAX_AMPDU_LEN_EXP_0             0x0 /* HE����AMPDU����ϵ��2^(20+factor)-1�ֽ� */
#endif
/*
 * 1����Э��涨ʱ��5.484ms�£�HE������������Ϊ2.4Gbps(VHTΪ2.1667Gbps)��
 *    ����֧�ֵ�����ֽ�Ϊ1645200 Bytes(VHT:1484793Bytes)��
 * 2������2^21 = 2097152��2^20 = 1048576������������Ϊ20��
 * 3�����ԣ�HE Max A-MPDU Length Exponent Extension�ֶ���д0��
 * 4��VHT Max A-MPDU Length Exponent Extension�ֶ���Ҫ�޸�Ϊ7
 */
#define MAC_VHT_AMPDU_MAX_LEN_EXP 7 /* 2^(13+factor)-1�ֽ� */
/* 3 ö�ٶ��� */
/* 3.1 ����ö������ */
/* ����֡������ */
typedef enum {
    WLAN_ASSOC_REQ = 0,           /* 0000 */
    WLAN_ASSOC_RSP = 1,           /* 0001 */
    WLAN_REASSOC_REQ = 2,         /* 0010 */
    WLAN_REASSOC_RSP = 3,         /* 0011 */
    WLAN_PROBE_REQ = 4,           /* 0100 */
    WLAN_PROBE_RSP = 5,           /* 0101 */
    WLAN_TIMING_AD = 6,           /* 0110 */
    WLAN_MGMT_SUBTYPE_RESV1 = 7,  /* 0111 */
    WLAN_BEACON = 8,              /* 1000 */
    WLAN_ATIM = 9,                /* 1001 */
    WLAN_DISASOC = 10,            /* 1010 */
    WLAN_AUTH = 11,               /* 1011 */
    WLAN_DEAUTH = 12,             /* 1100 */
    WLAN_ACTION = 13,             /* 1101 */
    WLAN_ACTION_NO_ACK = 14,      /* 1110 */
    WLAN_MGMT_SUBTYPE_RESV2 = 15, /* 1111 */

    WLAN_MGMT_SUBTYPE_BUTT = 16, /* һ��16�ֹ���֡������ */
} wlan_frame_mgmt_subtype_enum;

/* Ĭ�ϵ���������ҵ���TID */
#define WLAN_TID_FOR_DATA 0

/* TBD����Ӧ��BUTT��������ö�� */
typedef enum {
    WLAN_WME_AC_BE = 0, /* best effort */
    WLAN_WME_AC_BK = 1, /* background */
    WLAN_WME_AC_VI = 2, /* video */
    WLAN_WME_AC_VO = 3, /* voice */

    WLAN_WME_AC_BUTT = 4,
    WLAN_WME_AC_MGMT = WLAN_WME_AC_BUTT, /* ����AC��Э��û��,��ӦӲ�������ȼ����� */

    WLAN_WME_AC_PSM = 5 /* ����AC, Э��û�У���ӦӲ���鲥���� */
} wlan_wme_ac_type_enum;
typedef uint8_t wlan_wme_ac_type_enum_uint8;

/* TID����Ϊ8,0~7 */
#define WLAN_TID_MAX_NUM WLAN_TIDNO_BUTT

/* TID������ */
typedef enum {
    WLAN_TIDNO_BEST_EFFORT = 0,            /* BEҵ�� */
    WLAN_TIDNO_BACKGROUND = 1,             /* BKҵ�� */
    WLAN_TIDNO_UAPSD = 2,                  /* U-APSD */
    WLAN_TIDNO_ANT_TRAINING_LOW_PRIO = 3,  /* �������ߵ����ȼ�ѵ��֡ */
    WLAN_TIDNO_ANT_TRAINING_HIGH_PRIO = 4, /* �������߸����ȼ�ѵ��֡ */
    WLAN_TIDNO_VIDEO = 5,                  /* VIҵ�� */
    WLAN_TIDNO_VOICE = 6,                  /* VOҵ�� */
    WLAN_TIDNO_BCAST = 7,                  /* �㲥�û��Ĺ㲥�����鲥���� */

    WLAN_TIDNO_BUTT
} wlan_tidno_enum;
typedef uint8_t wlan_tidno_enum_uint8;

/* TID���������ƽ̨ */
/* VAP�Ĺ���ģʽ */
typedef enum {
    WLAN_VAP_MODE_CONFIG,   /* ����ģʽ */
    WLAN_VAP_MODE_BSS_STA,  /* BSS STAģʽ */
    WLAN_VAP_MODE_BSS_AP,   /* BSS APģʽ */
    WLAN_VAP_MODE_WDS,      /* WDSģʽ */
    WLAN_VAP_MODE_MONITOER, /* ����ģʽ */

    WLAN_VAP_HW_TEST,

    WLAN_VAP_MODE_BUTT
} wlan_vap_mode_enum;
typedef uint8_t wlan_vap_mode_enum_uint8;

/* ��֤��transaction number */
typedef enum {
    WLAN_AUTH_TRASACTION_NUM_ONE = 0x0001,
    WLAN_AUTH_TRASACTION_NUM_TWO = 0x0002,
    WLAN_AUTH_TRASACTION_NUM_THREE = 0x0003,
    WLAN_AUTH_TRASACTION_NUM_FOUR = 0x0004,

    WLAN_AUTH_TRASACTION_NUM_BUTT
} wlan_auth_transaction_number_enum;
typedef uint16_t wlan_auth_transaction_number_enum_uint16;
typedef enum {
    WLAN_BAND_2G,
    WLAN_BAND_5G,

    WLAN_BAND_BUTT
} wlan_channel_band_enum;
typedef uint8_t wlan_channel_band_enum_uint8;

typedef enum {
    WLAN_TX_SCHEDULE_TYPE_ENQ_TID = 0,
    WLAN_TX_SCHEDULE_TYPE_TX_COMP,
    WLAN_TX_SCHEDULE_TYPE_OTHERS,

    WLAN_TX_SCHEDULE_TYPE_BUTT
} wlan_tx_schedule_type_enum;
typedef uint8_t wlan_tx_schedule_type_enum_uint8;

/* Protection mode for MAC */
typedef enum {
    WLAN_PROT_NO,  /* Do not use any protection       */
    WLAN_PROT_ERP, /* Protect all ERP frame exchanges */
    WLAN_PROT_HT,  /* Protect all HT frame exchanges  */
    WLAN_PROT_GF,  /* Protect all GF frame exchanges  */

    WLAN_PROT_BUTT
} wlan_prot_mode_enum;
typedef uint8_t wlan_prot_mode_enum_uint8;

typedef enum {
    WLAN_RTS_RATE_SELECT_MODE_REG,  /* 0: RTS���� = PROT_DATARATE������                           */
    WLAN_RTS_RATE_SELECT_MODE_DESC, /* 1: RTS���� = Ӳ������TX�������������ֵ                    */
    WLAN_RTS_RATE_SELECT_MODE_MIN,  /* 2: RTS���� = min(PROT_DATARATE,Ӳ������TX�������������ֵ) */
    WLAN_RTS_RATE_SELECT_MODE_MAX,  /* 3: RTS���� = max(PROT_DATARATE,Ӳ������TX�������������ֵ) */

    WLAN_RTS_RATE_SELECT_MODE_BUTT
} wlan_rts_rate_select_mode_enum;

#if IS_HOST
typedef enum {
    WLAN_WITP_AUTH_OPEN_SYSTEM = 0,
    WLAN_WITP_AUTH_SHARED_KEY,
    WLAN_WITP_AUTH_FT,
    WLAN_WITP_AUTH_NETWORK_EAP,
    WLAN_WITP_AUTH_SAE,
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 10, 0))
    /* 4.10���ϰ汾����3��FILS�����֤���� */
    WLAN_WITP_AUTH_FILS_SK,
    WLAN_WITP_AUTH_FILS_SK_PFS,
    WLAN_WITP_AUTH_FILS_PK,
#endif
    WLAN_WITP_AUTH_NUM,
    WLAN_WITP_AUTH_MAX = WLAN_WITP_AUTH_NUM - 1,
    WLAN_WITP_AUTH_AUTOMATIC,
    WLAN_WITP_AUTH_BUTT
} wlan_auth_alg_mode_enum;
#else
typedef enum {
    WLAN_WITP_AUTH_OPEN_SYSTEM = 0,
    WLAN_WITP_AUTH_SHARED_KEY,
    WLAN_WITP_AUTH_FT,
    WLAN_WITP_AUTH_NETWORK_EAP,
    WLAN_WITP_AUTH_SAE,
} wlan_auth_alg_mode_enum;
#endif

typedef uint8_t wlan_auth_alg_mode_enum_uint8;

typedef enum {
    /*
     *  ע��: wlan_cipher_key_type_enum��hal_key_type_enum ֵһ��,
     *        ���Ӳ���иı䣬��Ӧ����HAL ���װ
     */
    /* TBD:�˴����ֺ�Э��涨����Կ����һ�� */
    WLAN_KEY_TYPE_TX_GTK = 0,  /* TX GTK */
    WLAN_KEY_TYPE_PTK = 1,     /* PTK */
    WLAN_KEY_TYPE_RX_GTK = 2,  /* RX GTK */
    WLAN_KEY_TYPE_RX_GTK2 = 3, /* RX GTK2 51���� */
    WLAN_KEY_TYPE_BUTT
} wlan_cipher_key_type_enum;
typedef uint8_t wlan_cipher_key_type_enum_uint8;

typedef enum {
    /*
     *  ע��: wlan_key_origin_enum_uint8��hal_key_origin_enum_uint8 ֵһ��,
     *        ���Ӳ���иı䣬��Ӧ����HAL ���װ
     */
    WLAN_AUTH_KEY = 0, /* Indicates that for this key, this STA is the authenticator */
    WLAN_SUPP_KEY = 1, /* Indicates that for this key, this STA is the supplicant */

    WLAN_KEY_ORIGIN_BUTT,
} wlan_key_origin_enum;
typedef uint8_t wlan_key_origin_enum_uint8;

typedef enum {
    NARROW_BW_10M = 0x80,
    NARROW_BW_5M = 0x81,
    NARROW_BW_1M = 0x82,
    NARROW_BW_BUTT
} mac_narrow_bw_enum;
typedef uint8_t mac_narrow_bw_enum_uint8;

typedef enum {
    WLAN_ADDBA_MODE_AUTO,
    WLAN_ADDBA_MODE_MANUAL,

    WLAN_ADDBA_MODE_BUTT
} wlan_addba_mode_enum;
typedef uint8_t wlan_addba_mode_enum_uint8;

typedef enum {
    /* ����80211-2012/ 11ac-2013 Э�� Table 8-99 Cipher suite selectors ���� */
    WLAN_80211_CIPHER_SUITE_GROUP_CIPHER = 0,
    WLAN_80211_CIPHER_SUITE_WEP_40 = 1,
    WLAN_80211_CIPHER_SUITE_TKIP = 2,
    WLAN_80211_CIPHER_SUITE_RSV = 3,
    WLAN_80211_CIPHER_SUITE_NO_ENCRYP = WLAN_80211_CIPHER_SUITE_RSV, /* ����Э�鶨��ı���ֵ������������ */
    WLAN_80211_CIPHER_SUITE_CCMP = 4,
    WLAN_80211_CIPHER_SUITE_WEP_104 = 5,
    WLAN_80211_CIPHER_SUITE_BIP = 6,
    WLAN_80211_CIPHER_SUITE_GROUP_DENYD = 7,
    WLAN_80211_CIPHER_SUITE_GCMP = 8, /* GCMP-128 default for a DMG STA */
    WLAN_80211_CIPHER_SUITE_GCMP_256 = 9,
    WLAN_80211_CIPHER_SUITE_CCMP_256 = 10,
    WLAN_80211_CIPHER_SUITE_BIP_GMAC_128 = 11, /* BIP GMAC 128 */
    WLAN_80211_CIPHER_SUITE_BIP_GMAC_256 = 12, /* BIP GMAC 256 */
    WLAN_80211_CIPHER_SUITE_BIP_CMAC_256 = 13, /* BIP CMAC 256 */

    WLAN_80211_CIPHER_SUITE_WAPI = 14, /* ���ⶨ����Ӱ��11i���� */
} wlan_ciper_protocol_type_enum;
typedef uint8_t wlan_ciper_protocol_type_enum_uint8;

/* ����80211-2012 Э�� Table 8-101 AKM suite selectors ���� */
#define WLAN_AUTH_SUITE_RSV        0
#define WLAN_AUTH_SUITE_1X         1
#define WLAN_AUTH_SUITE_PSK        2
#define WLAN_AUTH_SUITE_FT_1X      3
#define WLAN_AUTH_SUITE_FT_PSK     4
#define WLAN_AUTH_SUITE_1X_SHA256  5
#define WLAN_AUTH_SUITE_PSK_SHA256 6
#define WLAN_AUTH_SUITE_TDLS       7
#define WLAN_AUTH_SUITE_SAE_SHA256 8
#define WLAN_AUTH_SUITE_FT_SHA256  9
#define WLAN_AUTH_SUITE_OWE        18

#define MAC_OWE_GROUP_SUPPORTED_NUM (3)
#define MAC_OWE_GROUP_19            (19)
#define MAC_OWE_GROUP_20            (20)
#define MAC_OWE_GROUP_21            (21)
#define MAC_HID2D_SEQNUM_CNT 3    /* ����֮ǰ�ϱ���3�����к� */
#define MAC_HID2D_SEQNUM_AGE_CYCLE 300    /* �������кŵ��ϻ����ڣ����кų���300������Ϊ�ϻ� */
#define MAC_HID2D_MAX_SEQNUM 0xFFF    /* ������к� */
#define MAC_HID2D_INVALID_SEQNUM    0xFFFF
typedef enum {
    WITP_WPA_VERSION_1 = 1,
    WITP_WPA_VERSION_2 = 2,
#ifdef _PRE_WLAN_FEATURE_WAPI
    WITP_WAPI_VERSION = 1 << 2,
#endif
} witp_wpa_versions_enum;
typedef uint8_t witp_wpa_versions_enum_uint8;

typedef struct {
    wlan_cipher_key_type_enum_uint8 en_cipher_key_type; /* ��ԿID/���� */
    wlan_ciper_protocol_type_enum_uint8 en_cipher_protocol_type;
    uint8_t uc_cipher_key_id;
    uint8_t auc_resv[1];
} wlan_security_txop_params_stru;

/* Э������ö�� */
typedef enum {
    WLAN_PROTOCOL_CAP_LEGACY,
    WLAN_PROTOCOL_CAP_HT,
    WLAN_PROTOCOL_CAP_VHT,
#ifdef _PRE_WLAN_FEATURE_11AX
    WLAN_PROTOCOL_CAP_HE,
#endif

    WLAN_PROTOCOL_CAP_BUTT,
} wlan_protocol_cap_enum;
typedef uint8_t wlan_protocol_cap_enum_uint8;

/* Ƶ������ö�� */
typedef enum {
    WLAN_BAND_CAP_2G,    /* ֻ֧��2G */
    WLAN_BAND_CAP_5G,    /* ֻ֧��5G */
    WLAN_BAND_CAP_2G_5G, /* ֧��2G 5G */

    WLAN_BAND_CAP_BUTT
} wlan_band_cap_enum;
typedef uint8_t wlan_band_cap_enum_uint8;

/* ��������ö�� */
typedef enum {
    WLAN_BW_CAP_20M,
    WLAN_BW_CAP_40M,
    WLAN_BW_CAP_80M,
    WLAN_BW_CAP_160M,
    WLAN_BW_CAP_80PLUS80, /* ������80+80 */

    WLAN_BW_CAP_BUTT
} wlan_bw_cap_enum;
typedef uint8_t wlan_bw_cap_enum_uint8;

/* ���Ʒ�ʽö�� */
typedef enum {
    WLAN_MOD_DSSS,
    WLAN_MOD_OFDM,

    WLAN_MOD_BUTT
} wlan_mod_enum;
typedef uint8_t wlan_mod_enum_uint8;

typedef enum {
    REGDOMAIN_FCC = 0,
    REGDOMAIN_ETSI = 1,
    REGDOMAIN_JAPAN = 2,
    REGDOMAIN_COMMON = 3,

    REGDOMAIN_COUNT
} regdomain_enum;
typedef uint8_t regdomain_enum_uint8;

/* 3.4 HEö������ */
/* 3.4 VHTö������ */
typedef enum {
    WLAN_VHT_MCS0,
    WLAN_VHT_MCS1,
    WLAN_VHT_MCS2,
    WLAN_VHT_MCS3,
    WLAN_VHT_MCS4,
    WLAN_VHT_MCS5,
    WLAN_VHT_MCS6,
    WLAN_VHT_MCS7,
    WLAN_VHT_MCS8,
    WLAN_VHT_MCS9,
#ifdef _PRE_WLAN_FEATURE_1024QAM
    WLAN_VHT_MCS10,
    WLAN_VHT_MCS11,
#endif

    WLAN_VHT_MCS_BUTT,
} wlan_vht_mcs_enum;
typedef uint8_t wlan_vht_mcs_enum_uint8;

/*
 * ����1101�����˳��
 * TBD�������S���ۺ��������ʵ��Ⱥ�˳��
 */
typedef enum {
    WLAN_LEGACY_11b_RESERVED1 = 0,
    WLAN_SHORT_11b_2M_BPS = 1,
    WLAN_SHORT_11b_5_HALF_M_BPS = 2,
    WLAN_SHORT_11b_11_M_BPS = 3,

    WLAN_LONG_11b_1_M_BPS = 4,
    WLAN_LONG_11b_2_M_BPS = 5,
    WLAN_LONG_11b_5_HALF_M_BPS = 6,
    WLAN_LONG_11b_11_M_BPS = 7,

    WLAN_LEGACY_OFDM_48M_BPS = 8,
    WLAN_LEGACY_OFDM_24M_BPS = 9,
    WLAN_LEGACY_OFDM_12M_BPS = 10,
    WLAN_LEGACY_OFDM_6M_BPS = 11,
    WLAN_LEGACY_OFDM_54M_BPS = 12,
    WLAN_LEGACY_OFDM_36M_BPS = 13,
    WLAN_LEGACY_OFDM_18M_BPS = 14,
    WLAN_LEGACY_OFDM_9M_BPS = 15,

    WLAN_LEGACY_RATE_VALUE_BUTT
} wlan_legacy_rate_value_enum;
typedef uint8_t wlan_legacy_rate_value_enum_uint8;

/* WIFIЭ�����Ͷ��� */
typedef enum {
    WLAN_LEGACY_11A_MODE = 0,    /* 11a, 5G, OFDM */
    WLAN_LEGACY_11B_MODE = 1,    /* 11b, 2.4G */
    WLAN_LEGACY_11G_MODE = 2,    /* �ɵ�11g only�ѷ���, 2.4G, OFDM */
    WLAN_MIXED_ONE_11G_MODE = 3, /* 11bg, 2.4G */
    WLAN_MIXED_TWO_11G_MODE = 4, /* 11g only, 2.4G */
    WLAN_HT_MODE = 5,            /* 11n(11bgn����11an������Ƶ���ж�) */
    WLAN_VHT_MODE = 6,           /* 11ac */
    WLAN_HT_ONLY_MODE = 7,       /* 11n only mode,ֻ�д�HT���豸�ſ��Խ��� */
    WLAN_VHT_ONLY_MODE = 8,      /* 11ac only mode ֻ�д�VHT���豸�ſ��Խ��� */
    WLAN_HT_11G_MODE = 9,        /* 11ng,������11b */
    WLAN_HE_MODE = 10, /* 11ax */
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
    WLAN_HE_ER_MODE = 11, /* ��������ʹ�� */
#endif

    WLAN_PROTOCOL_BUTT
} wlan_protocol_enum;
typedef uint8_t wlan_protocol_enum_uint8;

/* ��Ҫ:����VAP��preambleЭ��������ʹ�ø�ö�٣�0��ʾlong preamble; 1��ʾshort preamble */
typedef enum {
    WLAN_LEGACY_11B_MIB_LONG_PREAMBLE = 0,
    WLAN_LEGACY_11B_MIB_SHORT_PREAMBLE = 1,
} wlan_11b_mib_preamble_enum;
typedef uint8_t wlan_11b_mib_preamble_enum_uint8;

/* ��Ҫ:�����������ӿ�ʹ��(��ʾ���͸�֡ʹ�õ�pramble����)��0��ʾshort preamble; 1��ʾlong preamble */
typedef enum {
    WLAN_LEGACY_11B_DSCR_SHORT_PREAMBLE = 0,
    WLAN_LEGACY_11B_DSCR_LONG_PREAMBLE = 1,

    WLAN_LEGACY_11b_PREAMBLE_BUTT
} wlan_11b_dscr_preamble_enum;
typedef uint8_t wlan_11b_dscr_preamble_enum_uint8;

/* 3.3 HTö������ */
typedef enum {
    WLAN_BAND_WIDTH_20M = 0,
    WLAN_BAND_WIDTH_40PLUS,       /* ��20�ŵ�+1 */
    WLAN_BAND_WIDTH_40MINUS,      /* ��20�ŵ�-1 */
    WLAN_BAND_WIDTH_80PLUSPLUS,   /* ��20�ŵ�+1, ��40�ŵ�+1 */
    WLAN_BAND_WIDTH_80PLUSMINUS,  /* ��20�ŵ�+1, ��40�ŵ�-1 */
    WLAN_BAND_WIDTH_80MINUSPLUS,  /* ��20�ŵ�-1, ��40�ŵ�+1 */
    WLAN_BAND_WIDTH_80MINUSMINUS, /* ��20�ŵ�-1, ��40�ŵ�-1 */
#ifdef _PRE_WLAN_FEATURE_160M
    WLAN_BAND_WIDTH_160PLUSPLUSPLUS,    /* ��20�ŵ�+1, ��40�ŵ�+1, ��80�ŵ�+1 */
    WLAN_BAND_WIDTH_160PLUSPLUSMINUS,   /* ��20�ŵ�+1, ��40�ŵ�+1, ��80�ŵ�-1 */
    WLAN_BAND_WIDTH_160PLUSMINUSPLUS,   /* ��20�ŵ�+1, ��40�ŵ�-1, ��80�ŵ�+1 */
    WLAN_BAND_WIDTH_160PLUSMINUSMINUS,  /* ��20�ŵ�+1, ��40�ŵ�-1, ��80�ŵ�-1 */
    WLAN_BAND_WIDTH_160MINUSPLUSPLUS,   /* ��20�ŵ�-1, ��40�ŵ�+1, ��80�ŵ�+1 */
    WLAN_BAND_WIDTH_160MINUSPLUSMINUS,  /* ��20�ŵ�-1, ��40�ŵ�+1, ��80�ŵ�-1 */
    WLAN_BAND_WIDTH_160MINUSMINUSPLUS,  /* ��20�ŵ�-1, ��40�ŵ�-1, ��80�ŵ�+1 */
    WLAN_BAND_WIDTH_160MINUSMINUSMINUS, /* ��20�ŵ�-1, ��40�ŵ�-1, ��80�ŵ�-1 */
#endif
    WLAN_BAND_WIDTH_40M,
    WLAN_BAND_WIDTH_80M,

    WLAN_BAND_WIDTH_BUTT
} wlan_channel_bandwidth_enum;
typedef uint8_t wlan_channel_bandwidth_enum_uint8;

typedef enum {
    WLAN_AP_BANDWIDTH_20M = 1,
    WLAN_AP_BANDWIDTH_40M = 2,
    WLAN_AP_BANDWIDTH_80M = 4,
    WLAN_AP_BANDWIDTH_160M = 8,
} wlan_vendor_ap_bandwidth_enum;
typedef uint8_t wlan_vendor_ap_bandwidth_enum_uint8;

typedef enum {
    WLAN_CH_SWITCH_DONE = 0,     /* �ŵ��л��Ѿ���ɣ�AP�����ŵ����� */
    WLAN_CH_SWITCH_STATUS_1 = 1, /* AP���ڵ�ǰ�ŵ���׼�������ŵ��л�(����CSA֡/IE) */
    WLAN_CH_SWITCH_STATUS_2 = 2,

    WLAN_CH_SWITCH_BUTT
} wlan_ch_switch_status_enum;
typedef uint8_t wlan_ch_switch_status_enum_uint8;
typedef enum {
    WLAN_AP_CHIP_OUI_NORMAL = 0,
    WLAN_AP_CHIP_OUI_RALINK = 1, /* оƬ����ΪRALINK */
    WLAN_AP_CHIP_OUI_RALINK1 = 2,
    WLAN_AP_CHIP_OUI_ATHEROS = 3, /* оƬ����ΪATHEROS */
    WLAN_AP_CHIP_OUI_BCM = 4,     /* оƬ����ΪBROADCOM */
    WLAN_AP_CHIP_OUI_QLM = 5,     /* оƬ����ΪQUALCOMM */

    WLAN_AP_CHIP_OUI_BUTT
} wlan_ap_chip_oui_enum;
typedef uint8_t wlan_ap_chip_oui_enum_uint8;

typedef enum {
    WLAN_CSA_MODE_TX_ENABLE = 0,
    WLAN_CSA_MODE_TX_DISABLE,

    WLAN_CSA_MODE_TX_BUTT
} wlan_csa_mode_tx_enum;
typedef uint8_t wlan_csa_mode_tx_enum_uint8;

typedef enum {
    WLAN_BW_SWITCH_DONE = 0,     /* Ƶ���л������ */
    WLAN_BW_SWITCH_40_TO_20 = 1, /* ��40MHz�����л���20MHz���� */
    WLAN_BW_SWITCH_20_TO_40 = 2, /* ��20MHz�����л���40MHz���� */

    /* ������� */
    WLAN_BW_SWITCH_BUTT
} wlan_bw_switch_status_enum;
typedef uint8_t wlan_bw_switch_status_enum_uint8;

typedef enum {
    WLAN_HT_MIXED_PREAMBLE = 0,
    WLAN_HT_GF_PREAMBLE = 1,

    WLAN_HT_PREAMBLE_BUTT
} wlan_ht_preamble_enum;
typedef uint8_t wlan_ht_preamble_enum_uint8;

typedef enum {
    WLAN_HT_MCS0,
    WLAN_HT_MCS1,
    WLAN_HT_MCS2,
    WLAN_HT_MCS3,
    WLAN_HT_MCS4,
    WLAN_HT_MCS5,
    WLAN_HT_MCS6,
    WLAN_HT_MCS7,
    WLAN_HT_MCS8,
    WLAN_HT_MCS9,
    WLAN_HT_MCS10,
    WLAN_HT_MCS11,
    WLAN_HT_MCS12,
    WLAN_HT_MCS13,
    WLAN_HT_MCS14,
    WLAN_HT_MCS15,

    WLAN_HT_MCS_BUTT
} wlan_ht_mcs_enum;
typedef uint8_t wlan_ht_mcs_enum_uint8;
#define WLAN_HT_MCS32                       32
#define WLAN_MIN_MPDU_LEN_FOR_MCS32         12
#define WLAN_MIN_MPDU_LEN_FOR_MCS32_SHORTGI 13

/* �ռ������� */
#define WLAN_SINGLE_NSS 0
#define WLAN_DOUBLE_NSS 1
#define WLAN_TRIPLE_NSS 2
#define WLAN_FOUR_NSS   3
#define WLAN_NSS_LIMIT  4
/* ��ΪҪ����Ԥ���룬������ö�ٸĳɺ꣬Ϊ�˱�����⣬���������ת���Ȳ��� */
typedef uint8_t wlan_nss_enum_uint8;

typedef enum {
    WLAN_HT_NON_STBC = 0,
    WLAN_HT_ADD_ONE_NTS = 1,
    WLAN_HT_ADD_TWO_NTS = 2,

    WLAN_HT_STBC_BUTT
} wlan_ht_stbc_enum;
typedef uint8_t wlan_ht_stbc_enum_uint8;

typedef enum {
    WLAN_NO_SOUNDING = 0,
    WLAN_NDP_SOUNDING = 1,
    WLAN_STAGGERED_SOUNDING = 2,
    WLAN_LEGACY_SOUNDING = 3,

    WLAN_SOUNDING_BUTT
} wlan_sounding_enum;
typedef uint8_t wlan_sounding_enum_uint8;

typedef enum {
    WLAN_PROT_DATARATE_CHN0_1M = 0,
    WLAN_PROT_DATARATE_CHN1_1M,
    WLAN_PROT_DATARATE_CHN0_6M,
    WLAN_PROT_DATARATE_CHN1_6M,
    WLAN_PROT_DATARATE_CHN0_12M,
    WLAN_PROT_DATARATE_CHN1_12M,
    WLAN_PROT_DATARATE_CHN0_24M,
    WLAN_PROT_DATARATE_CHN1_24M,

    WLAN_PROT_DATARATE_BUTT
} wlan_prot_datarate_enum;
typedef uint8_t wlan_prot_datarate_enum_uint8;

typedef struct {
    uint8_t uc_group_id; /* group_id   */
    uint8_t uc_txop_ps_not_allowed;
    uint16_t us_partial_aid; /* partial_aid */
} wlan_groupid_partial_aid_stru;

/* channel�ṹ�� */
typedef struct {
    uint8_t uc_chan_number;                       /* ��20MHz�ŵ��� */
    wlan_channel_band_enum_uint8 en_band;           /* Ƶ�� */
    wlan_channel_bandwidth_enum_uint8 en_bandwidth; /* ����ģʽ */
    uint8_t uc_chan_idx;                          /* �ŵ������� */
} mac_channel_stru;

/* 3.4 �㷨��,ö������ */
typedef enum {
    WLAN_NON_TXBF = 0,
    WLAN_EXPLICIT_TXBF = 1,
    WLAN_LEGACY_TXBF = 2,

    WLAN_TXBF_BUTT
} wlan_txbf_enum;
typedef uint8_t wlan_txbf_enum_uint8;

/* TPC����ģʽ */
typedef enum {
    /* ����TPC��̬��������ģʽ: ֱ�Ӳ��ù̶�����ģʽ������֡��Data0�������õ�, Data1~3�Լ�����֡������֡��������� */
    WLAN_TPC_WORK_MODE_DISABLE = 0,
    /* ����Ӧ����ģʽ: ����֡��Data0��������Ӧ����, Data1~3�Լ�����֡������֡��������� */
    WLAN_TPC_WORK_MODE_ENABLE = 1,

    WLAN_TPC_WORK_MODE_BUTT
} wlan_tpc_work_mode_enum;
typedef uint8_t wlan_tpc_mode_enum_uint8;

typedef enum {
    WLAN_TX_AMSDU_NONE = 0,
    WLAN_TX_AMSDU_BY_2 = 1,
    WLAN_TX_AMSDU_BY_3 = 2,
    WLAN_TX_AMSDU_BY_4 = 3,

    WLAN_TX_AMSDU_BUTT
} wlan_tx_amsdu_enum;
typedef uint8_t wlan_tx_amsdu_enum_uint8;

/* CCA_OPT����ģʽ */
#define WLAN_CCA_OPT_DISABLE 0 /* �����κ��Ż� */
#define WLAN_CCA_OPT_ENABLE  1 /* ����ͬƵʶ���EDCA�Ż� */

/* EDCA_OPT STAģʽ�¹���ģʽ */
#define WLAN_EDCA_OPT_STA_DISABLE 0 /* �����κ��Ż� */
#define WLAN_EDCA_OPT_STA_ENABLE  1 /* ����ͬƵʶ���EDCA�Ż� */

/* EDCA_OPT APģʽ�¹���ģʽ */
#define WLAN_EDCA_OPT_AP_EN_DISABLE   0 /* �����κ��Ż� */
#define WLAN_EDCA_OPT_AP_EN_DEFAULT   1 /* ����ͬƵʶ���EDCA�Ż� */
#define WLAN_EDCA_OPT_AP_EN_WITH_COCH 2 /* ����ͬƵʶ����Ż� */

/* �����������㷨ʹ��ģʽ */
#define WLAN_ANTI_INTF_EN_OFF   0 /* �㷨�ر� */
#define WLAN_ANTI_INTF_EN_ON    1 /* �㷨�� */
#define WLAN_ANTI_INTF_EN_PROBE 2 /* ̽����ƽ����㷨��/�� */

/* ��̬byass����LNAʹ��ģʽ */
#define WLAN_EXTLNA_BYPASS_EN_OFF   0 /* ��̬bypass����LNA�� */
#define WLAN_EXTLNA_BYPASS_EN_ON    1 /* ��̬bypass����LNA�� */
#define WLAN_EXTLNA_BYPASS_EN_FORCE 2 /* ��̬bypass����LNAǿ�ƿ� */

/* DFSʹ��ģʽ */
#define WLAN_DFS_EN_OFF       0 /* �㷨�ر� */
#define WLAN_DFS_EN_ON        1 /* �㷨��,��⵽�״��л��ŵ� */
#define WLAN_DFS_EN_ALL_PULSE 2 /* �㷨��,��⵽�״��л��ŵ��Ҵ�ӡ����Pulse */

#define WLAN_BAND_WIDTH_IS_40M(_uc_bandwidth) \
    ((WLAN_BAND_WIDTH_40MINUS == (_uc_bandwidth)) || (WLAN_BAND_WIDTH_40PLUS == (_uc_bandwidth)))
#define WLAN_BAND_WIDTH_IS_80M(_uc_bandwidth) \
    (((_uc_bandwidth) >= WLAN_BAND_WIDTH_80PLUSPLUS) && ((_uc_bandwidth) <= WLAN_BAND_WIDTH_80MINUSMINUS))
#ifdef _PRE_WLAN_FEATURE_160M
#define WLAN_BAND_WIDTH_IS_160M(_uc_bandwidth) \
    (((_uc_bandwidth) >= WLAN_BAND_WIDTH_160PLUSPLUSPLUS) && ((_uc_bandwidth) <= WLAN_BAND_WIDTH_160MINUSMINUSMINUS))
#endif
#define WLAN_BAND_WIDTH_IS_20M_OR_40M(_uc_bandwidth) \
    ((WLAN_BAND_WIDTH_20M == (_uc_bandwidth)) \
        || (WLAN_BAND_WIDTH_40PLUS == (_uc_bandwidth)) \
        || (WLAN_BAND_WIDTH_40MINUS == (_uc_bandwidth)))

/* 3.5 WMEö������ */
/* WMMö�����ͷ���ƽ̨ */
/* ֡���� (2-bit) */
typedef enum {
    WLAN_MANAGEMENT = 0,
    WLAN_CONTROL = 1,
    WLAN_DATA_BASICTYPE = 2,
    WLAN_RESERVED = 3,

    WLAN_FRAME_TYPE_BUTT
} wlan_frame_type_enum;
typedef uint8_t wlan_frame_type_enum_uint8;

/* ֡������ (4-bit) */
/* ����֡�����ͷ���ƽ̨SPEC */
/* ����֡֡������ */
typedef enum {
    /* 0~6 reserved */
    WLAN_HE_TRIG_FRAME = 2, /* 0010 */
    WLAN_VHT_NDPA = 5,      /* 0101 */
    WLAN_CONTROL_WRAPPER = 7,
    WLAN_BLOCKACK_REQ = 8,
    WLAN_BLOCKACK = 9,
    WLAN_PS_POLL = 10,
    WLAN_RTS = 11,
    WLAN_CTS = 12,
    WLAN_ACK = 13,
    WLAN_CF_END = 14,
    WLAN_CF_END_CF_ACK = 15,

    WLAN_CONTROL_SUBTYPE_BUTT = 16,
} wlan_frame_control_subtype_enum;

/* ����֡������ */
typedef enum {
    WLAN_DATA = 0,
    WLAN_DATA_CF_ACK = 1,
    WLAN_DATA_CF_POLL = 2,
    WLAN_DATA_CF_ACK_POLL = 3,
    WLAN_NULL_FRAME = 4,
    WLAN_CF_ACK = 5,
    WLAN_CF_POLL = 6,
    WLAN_CF_ACK_POLL = 7,
    WLAN_QOS_DATA = 8,
    WLAN_QOS_DATA_CF_ACK = 9,
    WLAN_QOS_DATA_CF_POLL = 10,
    WLAN_QOS_DATA_CF_ACK_POLL = 11,
    WLAN_QOS_NULL_FRAME = 12,
    WLAN_DATA_SUBTYPE_RESV1 = 13,
    WLAN_QOS_CF_POLL = 14,
    WLAN_QOS_CF_ACK_POLL = 15,

    WLAN_DATA_SUBTYPE_MGMT = 16,
} wlan_frame_data_subtype_enum;

/* ACK���Ͷ��� */
typedef enum {
    WLAN_TX_NORMAL_ACK = 0,
    WLAN_TX_NO_ACK,
    WLAN_TX_NO_EXPLICIT_ACK,
    WLAN_TX_BLOCK_ACK,

    WLAN_TX_NUM_ACK_BUTT
} wlan_tx_ack_policy_enum;
typedef uint8_t wlan_tx_ack_policy_enum_uint8;

/* Trig֡֡������ */
typedef enum {
    WLAN_HE_BASIC_TRIG = 0,
    WLAN_BEAM_REPORT_POLL = 1,
    WLAN_MU_BAR = 2,
    WLAN_MU_RTS = 3,
    WLAN_BUFFER_STATUS_REPORT_POLL = 4,
    WLAN_GCR_MU_BAR = 5,
    WLAN_BW_QUERY_REPORT_POLL = 6,
    WLAN_NDP_FEEDBACK_REPORT_POLL = 7,

    WLAN_HE_TRIG_TYPE_BUTT
} wlan_frame_trig_type_enum;

/* 3.6 �ŵ�ö�� */
/* �ŵ����뷽ʽ */
typedef enum {
    WLAN_BCC_CODE = 0,
    WLAN_LDPC_CODE = 1,
    WLAN_CHANNEL_CODE_BUTT
} wlan_channel_code_enum;
typedef uint8_t wlan_channel_code_enum_uint8;

/* ɨ������ */
typedef enum {
    WLAN_SCAN_TYPE_PASSIVE = 0,
    WLAN_SCAN_TYPE_ACTIVE = 1,

    WLAN_SCAN_TYPE_BUTT
} wlan_scan_type_enum;
typedef uint8_t wlan_scan_type_enum_uint8;

/* ɨ��ģʽ */
typedef enum {
    WLAN_SCAN_MODE_FOREGROUND = 0,      /* ǰ��ɨ�費��AP/STA(����ʼɨ�裬����ʽ) */
    WLAN_SCAN_MODE_BACKGROUND_STA = 1,  /* STA����ɨ�� */
    WLAN_SCAN_MODE_BACKGROUND_AP = 2,   /* AP����ɨ��(���ʽ) */
    WLAN_SCAN_MODE_ROAM_SCAN = 3,
    WLAN_SCAN_MODE_BACKGROUND_OBSS = 4, /* 20/40MHz�����obssɨ�� */
    WLAN_SCAN_MODE_BACKGROUND_PNO = 5, /* PNO����ɨ�� */
    WLAN_SCAN_MODE_RRM_BEACON_REQ = 6,
    WLAN_SCAN_MODE_BACKGROUND_CSA = 7,    /* �ŵ��л�ɨ�� */
    WLAN_SCAN_MODE_BACKGROUND_HILINK = 8, /* hilinkɨ��δ�����û� */
#ifdef _PRE_WLAN_FEATURE_FTM
    WLAN_SCAN_MODE_FTM_REQ = 9,
#endif
    WLAN_SCAN_MODE_GNSS_SCAN = 10,
    WLAN_SCAN_MODE_BACKGROUND_CCA = 11,
#ifdef _PRE_WLAN_FEATURE_NAN
    WLAN_SCAN_MODE_NAN_ROC = 12,
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D
    WLAN_SCAN_MODE_HID2D_SCAN = 13, /* HiD2DͶ������ɨ�� */
#endif
    WLAN_SCAN_MODE_BUTT
} wlan_scan_mode_enum;
typedef uint8_t wlan_scan_mode_enum_uint8;

/* �ں˶���flag��־λ��BIT9�Ƿ���������ɨ���־λ */
typedef enum {
    WLAN_SCAN_FLAG_LOW_PRIORITY = BIT0,
    WLAN_SCAN_FLAG_LOW_FLUSH = BIT1,
    WLAN_SCAN_FLAG_AP = BIT2,
    WLAN_SCAN_FLAG_RANDOM_ADDR = BIT3,
    WLAN_SCAN_FLAG_LOW_SPAN = BIT8,
    WLAN_SCAN_FLAG_LOW_POWER = BIT9,      /* ����ɨ�� */
    WLAN_SCAN_FLAG_HIFH_ACCURACY = BIT10, /* ˳��ɨ��,�ǲ��� */
    WLAN_SCAN_FLAG_BUTT
} wlan_scan_flag_enum;

/* ɨ����ö�� */
typedef enum {
    WLAN_SCAN_EVENT_COMPLETE = 0,
    WLAN_SCAN_EVENT_FAILED,
    WLAN_SCAN_EVENT_ABORT, /* ǿ����ֹ������ж�� */
    WLAN_SCAN_EVENT_TIMEOUT,
    WLAN_SCAN_EVENT_BUTT
} dmac_scan_event_enum;
typedef uint8_t wlan_scan_event_enum_uint8;

typedef enum {
    WLAN_LEGACY_VAP_MODE = 0, /* ��P2P�豸 */
    WLAN_P2P_GO_MODE = 1,     /* P2P_GO */
    WLAN_P2P_DEV_MODE = 2,    /* P2P_Device */
    WLAN_P2P_CL_MODE = 3,     /* P2P_CL */

    WLAN_P2P_BUTT
} wlan_p2p_mode_enum;
typedef uint8_t wlan_p2p_mode_enum_uint8;

/* 3.7 ����ö�� */
/* pmf������ */
typedef enum {
    MAC_PMF_DISABLED = 0, /* ��֧��pmf���� */
    MAC_PMF_ENABLED,      /* ֧��pmf�������Ҳ�ǿ�� */
    MAC_PMF_REQUIRED,     /* �ϸ�ִ��pmf���� */

    MAC_PMF_BUTT
} wlan_pmf_cap_status;
typedef uint8_t wlan_pmf_cap_status_uint8;

/* �û�����״̬ */
typedef enum {
    WLAN_DISTANCE_NEAR = 0,
    WLAN_DISTANCE_NORMAL = 1,
    WLAN_DISTANCE_FAR = 2,

    WLAN_DISTANCE_BUTT
} wlan_user_distance_enum;
typedef uint8_t wlan_user_distance_enum_uint8;

/* 3.8 linkloss����ö�� */
/* linkloss����ö�� */
typedef enum {
    WLAN_LINKLOSS_MODE_BT = 0,
    WLAN_LINKLOSS_MODE_DBAC,
    WLAN_LINKLOSS_MODE_NORMAL,

    WLAN_LINKLOSS_MODE_BUTT
} wlan_linkloss_mode_enum;
typedef uint8_t wlan_linkloss_mode_enum_uint8;

typedef enum {
    WALN_LINKLOSS_SCAN_SWITCH_CHAN_DISABLE = 0,
    WALN_LINKLOSS_SCAN_SWITCH_CHAN_EN = 1,

    WALN_LINKLOSS_SCAN_SWITCH_CHAN_BUTT
} wlan_linkloss_scan_switch_chan_enum;
typedef uint8_t wlan_linkloss_scan_switch_chan_enum_uint8;

/* 3.9 roc����ö�� */
typedef enum {
    IEEE80211_ROC_TYPE_NORMAL = 0,
    IEEE80211_ROC_TYPE_MGMT_TX,
    IEEE80211_ROC_TYPE_BUTT,
} wlan_ieee80211_roc_type;
typedef uint8_t wlan_ieee80211_roc_type_uint8;

/* 3.10 roam����ö�� */
/* �����л�״̬ */
typedef enum {
    WLAN_ROAM_MAIN_BAND_STATE_2TO2 = 0,
    WLAN_ROAM_MAIN_BAND_STATE_5TO2 = 1,
    WLAN_ROAM_MAIN_BAND_STATE_2TO5 = 2,
    WLAN_ROAM_MAIN_BAND_STATE_5TO5 = 3,

    WLAN_ROAM_MAIN_BAND_STATE_BUTT
} wlan_roam_main_band_state_enum;
typedef uint8_t wlan_roam_main_band_state_enum_uint8;

/* 3.10 m2s�л�ö�� */
/* mimo-siso�л�tpye */
typedef enum {
    WLAN_M2S_TYPE_SW = 0,
    WLAN_M2S_TYPE_HW = 1,

    WLAN_M2S_TYPE_BUTT,
} wlan_m2s_tpye_enum;
typedef uint8_t wlan_m2s_type_enum_uint8;

/* ldac m2s ���޳���ö�� */
typedef enum {
    WLAN_M2S_LDAC_RSSI_TO_SISO = 0,
    WLAN_M2S_LDAC_RSSI_TO_MIMO,

    WLAN_M2S_LDAC_RSSI_BUTT
} wlan_m2s_ldac_rssi_th_enum;
typedef uint8_t wlan_m2s_ldac_rssi_th_enum_uint8;

/* btcoex mcm  ���޳���ö�� */
typedef enum {
    WLAN_BTCOEX_RSSI_MCM_DOWN = 0,
    WLAN_BTCOEX_RSSI_MCM_UP,

    WLAN_BTCOEX_RSSI_MCM_BUTT
} wlan_mcm_btcoex_rssi_th_enum;
typedef uint8_t wlan_mcm_btcoex_rssi_th_enum_uint8;

typedef enum {
    DEVICE_TX = 0,
    HOST_TX = 1,

    TX_SWITCH_BUTT,
} wlan_tx_switch_enum;

/* mimo-siso�л�mode */
typedef enum {
    WLAN_M2S_TRIGGER_MODE_DBDC = BIT0,
    WLAN_M2S_TRIGGER_MODE_FAST_SCAN = BIT1,
    WLAN_M2S_TRIGGER_MODE_RSSI = BIT2,
    WLAN_M2S_TRIGGER_MODE_BTCOEX = BIT3,
    WLAN_M2S_TRIGGER_MODE_COMMAND = BIT4,
    WLAN_M2S_TRIGGER_MODE_TEST = BIT5,
    WLAN_M2S_TRIGGER_MODE_CUSTOM = BIT6, /* ���ƻ����ȼ��ߣ�ֻ�ܱ�������� */
    WLAN_M2S_TRIGGER_MODE_SPEC = BIT7,   /* spec���ȼ���ߣ��������� */

    WLAN_M2S_TRIGGER_MODE_BUTT,
} wlan_m2s_trigger_mode_enum;
typedef uint8_t wlan_m2s_trigger_mode_enum_uint8;

typedef enum {
    WLAN_M2S_TRIGGER_EXT_MODE_MODEM = BIT0,

    WLAN_M2S_TRIGGER_EXT_MODE_BUTT,
} wlan_m2s_trigger_ext_mode_enum;

typedef uint8_t wlan_m2s_trigger_ext_mode_enum_uint8;

typedef enum {
    WLAN_OFDM_ACK_CTS_TYPE_24M = 0,
    WLAN_OFDM_ACK_CTS_TYPE_36M = 1,
    WLAN_OFDM_ACK_CTS_TYPE_48M = 2,
    WLAN_OFDM_ACK_CTS_TYPE_54M = 3,
    WLAN_OFDM_ACK_CTS_TYPE_BUTT,
} wlan_ofdm_ack_cts_type_enum;
typedef uint8_t wlan_ofdm_ack_cts_type_enum_uint8;

/* 3.11 m2s�л�ö�� */
typedef enum {
    WLAN_SPECIAL_FRM_RTS,
    WLAN_SPECIAL_FRM_ONE_PKT,
    WLAN_SPECIAL_FRM_ABORT_SELFCTS,
    WLAN_SPECIAL_FRM_ABORT_CFEND,
    WLAN_SPECIAL_FRM_CFEND,
    WLAN_SPECIAL_FRM_NDP,
    WLAN_SPECIAL_FRM_VHT_REPORT,
    WLAN_SPECIAL_FRM_ABORT_NULL_DATA,
    WLAN_SPECIAL_FRM_BUTT,
} wlan_special_frm_enum;
typedef uint8_t wlan_special_frm_enum_uint8;

/* 3.12 HEö�� */
typedef enum {
    WLAN_HE_MCS0,
    WLAN_HE_MCS1,
    WLAN_HE_MCS2,
    WLAN_HE_MCS3,
    WLAN_HE_MCS4,
    WLAN_HE_MCS5,
    WLAN_HE_MCS6,
    WLAN_HE_MCS7,
    WLAN_HE_MCS8,
    WLAN_HE_MCS9,
    WLAN_HE_MCS10,
    WLAN_HE_MCS11,
    WLAN_HE_MCS_BUTT,
} wlan_he_mcs_enum;
typedef uint8_t wlan_he_mcs_enum_uint8;
/* ER��MCSö�٣�����DCM���� */
typedef enum {
    WLAN_HE_ER_MCS0     = WLAN_HE_MCS0,
    WLAN_HE_ER_MCS1,
    WLAN_HE_ER_MCS2,
    WLAN_HE_ER_DCM_MCS0,
}wlan_he_er_mcs_enum;

/* �㷨��, �޸�GI˳����Ҫ��Ӧ�޸� hal_get_wlan_gi_type �� hal_get_phy_gi_typeӳ���ϵ */
typedef enum {
    WLAN_GI_LONG = 0,   /* ��11axʱ0.8us; axʱ3.2us */
    WLAN_LONG_GI = WLAN_GI_LONG,
    WLAN_GI_SHORT = 1,  /* ��11axʱ0.4us; axʱ0.8us  */
    WLAN_SHORT_GI = WLAN_GI_SHORT,
    WLAN_GI_MIDDLE = 2, /* ��11ax, 1.6us */
    WLAN_NON_HE_GI_TYPE_BUTT = WLAN_GI_MIDDLE,
    WLAN_GI_TYPE_BUTT = WLAN_GI_MIDDLE,
} wlan_gi_type_enum;
typedef uint8_t wlan_gi_type_enum_uint8;

typedef enum {
    WLAN_HE_GI_1 = 0, /* base gi:      0.8us */
    WLAN_HE_GI_2 = 1, /* double gi:    1.6us */
    WLAN_HE_GI_4 = 2, /* quadruple gi: 3.2us */

    WLAN_HE_GI_TYPE_BUTT,
} wlan_he_gi_type_enum;
typedef oal_uint8 wlan_he_gi_type_enum_uint8;

typedef enum {
    WLAN_HE_LTF_1X = 0, /* LTF: 1X */
    WLAN_HE_LTF_2X = 1, /* LTF: 2X */
    WLAN_HE_LTF_4X = 2, /* LTF: 4X */

    WLAN_HE_LTF_TYPE_BUTT,
} wlan_he_ltf_type_enum;
typedef uint8_t wlan_he_ltf_type_enum_uint8;

/* axЭ��֧�ֵ�RU��С */
typedef enum {
    WLAN_RUSIZE_26_TONES = 0,    /* 26-tones��С��RU */
    WLAN_RUSIZE_52_TONES = 1,    /* 52-tones��С��RU */
    WLAN_RUSIZE_106_TONES = 2,   /* 106-tones��С��RU */
    WLAN_RUSIZE_242_TONES = 3,   /* 242-tones��С��RU */
    WLAN_RUSIZE_484_TONES = 4,   /* 484-tones��С��RU */
    WLAN_RUSIZE_996_TONES = 5,   /* 996-tones��С��RU */
    WLAN_RUSIZE_2X996_TONES = 6, /* 2x996-tones��С��RU */

    WLAN_RUSIZE_BUTT
} wlan_rusize_type_enum;
typedef oal_uint8 wlan_rusize_type_enum_uint8;


/* BQR ÿ20M����ö�� */
#ifdef _PRE_WLAN_FEATURE_BQRP
typedef enum {
    WLAN_PER20M_0 = 0,
    WLAN_PER20M_1,
    WLAN_PER20M_2,
    WLAN_PER20M_3,
    WLAN_PER20M_4,
    WLAN_PER20M_5,
    WLAN_PER20M_6,
    WLAN_PER20M_7,
    WLAN_PER20M_BUTT,
} wlan_per20m_cca_status_enum;
typedef uint8_t wlan_per20m_cca_status_enum_uint8;
#endif

/* 3.14 NULL DATAö�� */
typedef enum {
    SEND_OM_QOSNULL,               /* 0---OM qosnull */
    SEND_OM_QOSNULL_ANNOUNCE_USER, /* 1---֪ͨ�û������ı� */
    SEND_QOSNULL_UAPSD,            /* 2---wmm�͹��� */
    SEND_QOSNULL_TO_STA_KEEPLIVE,  /* 3---qosnull KEEPLIVE */
    SEND_NULL_TO_STA_KEEPLIVE,     /* 4---null KEEPLIVE */
    SEND_NULL_RESV_PS_POLL,        /* 5---�յ��Զ�ps_poll */
    SEND_NULL_TO_AP_NOA,           /* 6---P2P�͹��� */
    SEND_NULL_TO_AP_DOZE,          /* 7---�͹��� */
    SEND_NULL_TO_AP_ACTIVE,        /* 8---�͹��� */

    NULL_DATA_BUTT,
} mac_null_data_enum;
typedef uint8_t mac_null_data_enum_uint8;

/* 3.15 mcm�л�ö�� */
typedef enum {
    WLAN_MCM_TRIGGER_MODE_DBDC = BIT0,
    WLAN_MCM_TRIGGER_MODE_FAST_SCAN = BIT1,
    WLAN_MCM_TRIGGER_MODE_RSSI = BIT2,
    WLAN_MCM_TRIGGER_MODE_BTCOEX = BIT3,
    WLAN_MCM_TRIGGER_MODE_COMMAND = BIT4,
    WLAN_MCM_TRIGGER_MODE_TEST = BIT5,
    WLAN_MCM_TRIGGER_MODE_CUSTOM = BIT6, /* ���ƻ����ȼ��ߣ�ֻ�ܱ�������� */
    WLAN_MCM_TRIGGER_MODE_SPEC = BIT7,   /* spec���ȼ���ߣ��������� */
    WLAN_MCM_TRIGGER_MODE_MODEM = BIT8,  /* modemռ������ */
    WLAN_MCM_TRIGGER_MODE_GNSS = BIT9,   /* gnssռ������ */

    WLAN_MCM_TRIGGER_MODE_BUTT,
} wlan_mcm_trigger_mode_enum;
typedef uint16_t wlan_mcm_trigger_mode_enum_uint16;

/* 3.16 HAL��MCM���¼����� */
typedef enum {
    HAL_MCM_EVENT_IDLE = 0,
    HAL_MCM_EVENT_WORK,
    HAL_MCM_EVENT_SCAN_BEGIN,
    HAL_MCM_EVENT_SCAN_PREPARE,
    HAL_MCM_EVENT_SCAN_CHANNEL_BACK,
    HAL_MCM_EVENT_SCAN_END,

    HAL_MCM_EVENT_BUTT
} hal_mcm_event_enum;
typedef uint8_t hal_mcm_event_enum_uint8;

/* 3.17 MCM�л����� */
typedef enum {
    WLAN_MCM_TYPE_SW = 0,
    WLAN_MCM_TYPE_HW = 1,

    WLAN_MCM_TYPE_BUTT,
} wlan_mcm_tpye_enum;
typedef uint8_t wlan_mcm_type_enum_uint8;
/* 3.18 ��������ö�� */
typedef enum {
    RX_ADC_SISO_CH0 = 0,
    RX_ADC_SISO_CH1 = 1,
    RX_ADC_MIMO     = 2,
    TX_DFE_DAC_SISO_CH0 = 3,
    TX_DFE_DAC_SISO_CH1 = 4,
    TX_DFE_DAC_MIMO = 5,
    FFT_IN_SISO_CH0 = 6,
    FFT_IN_SISO_CH1 = 7,
    FFT_IN_MIMO     = 8,
    MIMO_DATA       = 9,
    NDP_TRIGGER     = 10,
    FCS_TRIGGER     = 11,
    RX_SCO_MIMO     = 12,
    NACI            = 13,
    EVENT_RPT_ALL   = 14,
    RX_AGC_C0       = 15,
    WLAN_DATA_COLLECT_TYPE_BUTT,
} wlan_data_collect_enum;
typedef uint8_t wlan_data_collect_enum_uint8;


/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
typedef struct {
    uint32_t ul_cipher;                /* �������� */
    uint32_t ul_key_len;               /* ��Կ�� */
    uint32_t ul_seq_len;               /* sequnece�� */
    uint8_t auc_key[WLAN_WPA_KEY_LEN]; /* ��Կ */
    uint8_t auc_seq[WLAN_WPA_SEQ_LEN]; /* sequence */
} wlan_priv_key_param_stru;

/* DMAC SCAN �ŵ�ͳ�Ʋ�������ṹ�� */
typedef struct {
    uint8_t uc_channel_number; /* �ŵ��� */
    uint8_t uc_stats_valid;
    uint8_t uc_stats_cnt;      /* �ŵ���æ��ͳ�ƴ��� */
    uint8_t uc_free_power_cnt; /* �ŵ����й��ʲ������� */

    /* PHY�ŵ�����ͳ�� */
    uint8_t uc_bandwidth_mode;
    uint8_t auc_resv[1];
    int16_t s_free_power_stats_20M;
    int16_t s_free_power_stats_40M;
    int16_t s_free_power_stats_80M;

    /* MAC�ŵ�����ͳ�� */
    uint32_t ul_total_stats_time_us;
    uint32_t ul_total_free_time_20M_us;
    uint32_t ul_total_free_time_40M_us;
    uint32_t ul_total_free_time_80M_us;
    uint32_t ul_total_send_time_us;
    uint32_t ul_total_recv_time_us;

    uint8_t uc_radar_detected;  // FIXME: feed value
    uint8_t uc_radar_bw;
    uint8_t uc_radar_type;
    uint8_t uc_radar_freq_offset;

    int16_t s_free_power_stats_160M;
    uint16_t us_phy_total_stats_time_ms;
} wlan_scan_chan_stats_stru;

typedef struct {
    int16_t per20m_idle_pwr[8]; /* ÿ��20M���й���ͳ���ϱ� */
    uint32_t free_chn_per20m_time[8]; /* ÿ��20M���й���ʱ��ͳ���ϱ�) */
} wlan_scan_per20m_status_stru;

typedef struct {
    uint16_t us_chan_ratio_20M;
    uint16_t us_chan_ratio_40M;
    uint16_t us_chan_ratio_80M;
    int8_t c_free_power_20M;
    int8_t c_free_power_40M;
    int8_t c_free_power_80M;
    uint8_t auc_resv[3];
    uint8_t _rom[4];
} wlan_chan_ratio_stru;

/* m2s�������� */
typedef struct {
    uint8_t bit_dbdc : 1, /* dbdc���� */
              bit_fast_on : 1,    /* ����ɨ�败�� */
              bit_rssi_snr : 1,   /* rssi/snr���� */
              bit_btcoex : 1,     /* btcoex���� */
              bit_command : 1,    /* �ϲ������ */
              bit_test : 1,       /* ��������� */
              bit_custom : 1,     /* ���ƻ����� */
              bit_spec : 1;       /* RF��񴥷� */
} wlan_m2s_mode_stru;

typedef struct {
    uint8_t bit_modem : 1; /* MODEM���� */
} wlan_m2s_mode_ext_stru;

/* action֡��������ö�� */
typedef enum {
    WLAN_M2S_ACTION_TYPE_SMPS = 0,   /* action����smps */
    WLAN_M2S_ACTION_TYPE_OPMODE = 1, /* action����opmode */
    WLAN_M2S_ACTION_TYPE_NONE = 2,   /* �л�����action֡ */

    WLAN_M2S_ACTION_TYPE_BUTT,
} wlan_m2s_action_type_enum;
typedef uint8_t wlan_m2s_action_type_enum_uint8;

typedef struct {
    uint8_t auc_user_mac_addr[WLAN_MAC_ADDR_LEN]; /* �л���Ӧ��AP MAC��ַ */
    wlan_m2s_action_type_enum_uint8 en_action_type; /* �л���Ӧ��Ҫ���͵�action֡���ͣ���Ҫ��vap��ori�������� */
    oal_bool_enum_uint8 en_m2s_result;              /* �л��Ƿ���ϳ�ʼaction֡����Ϊvap�л��ɹ�ʧ�ܱ�� */
} wlan_m2s_mgr_vap_stru;

/* 8 UNION���� */
/* 9 OTHERS���� */

OAL_STATIC OAL_INLINE uint8_t wlan_hdr_get_frame_type(uint8_t *puc_header)
{
    return ((puc_header[0] & (0x0c)) >> 2);
}

/* 10 �������� */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of wlan_types.h */
