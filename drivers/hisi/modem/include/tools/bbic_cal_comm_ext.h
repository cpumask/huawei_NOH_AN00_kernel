/*
 * Huawei Technologies Sweden AB (C), 2001-2019
 * @author    Automatically generated by DAISY
 * @version
 * @date      2017-04-28 15:27:40
 * @file
 * @brief
 * @copyright Huawei Technologies Sweden AB
 */
#ifndef BBIC_CAL_COMM_EXT_H
#define BBIC_CAL_COMM_EXT_H

/*
 * 1. Other files included
*/
#include "vos.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#pragma pack(push, 4)

/*
 * 2. Macro definitions
 */

/*
 * 3. Enumerations declarations
 */

/*lint -save -e761*/
typedef unsigned short UINT16;
typedef short          INT16;
typedef unsigned char  UINT8;
typedef unsigned int   UINT32;
typedef signed int     INT32;
typedef signed char    SINT8;
/*lint -restore*/

/*
 * RX Cal Max Ant num
 */
#ifndef RFHAL_CAL_MAX_ANT_NUM
#define RFHAL_CAL_MAX_ANT_NUM 2
#endif

/*
 * GSM RF TX SLOT MAX NUM
 */
#define BBIC_CAL_GSM_TX_SLOT_NUM 8

#define BBIC_CAL_MAX_WR_MIPI_COUNT 16

#define BBIC_CAL_MAX_RD_MIPI_COUNT 2

#define BBIC_CAL_MAX_REGISTER_COUNT 10

/* RX֧������CC���� */
#define RX_MAX_CC_NUM 5

/* RX֧������ANT���� */
#define RX_MAX_ANT_NUM 8


enum TOOL_BBIC_CAL_MSG_TYPE_ENUM {
    /* Self cal for fast calibration */
    ID_TOOL_BBIC_CAL_DCXO_START_REQ      = 0x1001, /* < @sa BBIC_CAL_DCXO_START_REQ_STRU */
    ID_TOOL_BBIC_CAL_RX_IQ_START_REQ     = 0x1002, /* < @sa BBIC_CAL_RX_IQ_START_REQ_STRU */
    ID_TOOL_BBIC_CAL_TX_IQ_START_REQ     = 0x1003, /* < @sa BBIC_CAL_TX_IQ_START_REQ_STRU */
    ID_TOOL_BBIC_CAL_TX_LO_START_REQ     = 0x1004, /* < @sa BBIC_CAL_TX_LO_START_REQ_STRU */
    ID_TOOL_BBIC_CAL_IP2_START_REQ       = 0x1005, /* < @sa BBIC_CAL_IP2_START_REQ_STRU */
    ID_TOOL_BBIC_CAL_MRX_DCOC_START_REQ  = 0x1006, /* < @sa BBIC_CAL_MRX_DCOC_START_REQ_STRU */
    ID_TOOL_BBIC_CAL_RX_DCOC_START_REQ   = 0x1007, /* < @sa BBIC_CAL_RX_DCOC_START_REQ_STRU */
    ID_TOOL_BBIC_CAL_RFIC_SELFCAL_REQ    = 0x1008, /* < @sa BBIC_CAL_MSG_HDR_STRU */
    ID_TOOL_BBIC_CAL_TX_FILTER_REQ       = 0x1009, /* < @sa BBIC_CAL_TX_FLITER_START_REQ_STRU */
    ID_TOOL_BBIC_CAL_MRX_DELAY_REQ       = 0x100A, /* < @sa BBIC_CAL_MRX_DELAY_START_REQ_STRU */
    ID_TOOL_BBIC_HFCAL_TRXFDIQ_REQ       = 0x1010,
    ID_TOOL_BBIC_HFCAL_RXFIIQ_REQ        = 0x1011,
    ID_TOOL_BBIC_HFCAL_RXADC_REQ         = 0x1012,
    ID_TOOL_BBIC_HFCAL_RX_DCOC_START_REQ = 0x1017, /* < @sa BBIC_CAL_HF_RX_DCOC_START_REQ_STRU */
    ID_TOOL_BBIC_CAL_DCXO_DCOC_REQ       = 0x1018, /**< @sa BBIC_CAL_DcxoDcocCalReq */

    /* trx cal for fast calibration */
    ID_TOOL_BBIC_CAL_RX_START_REQ         = 0x1021, /* < @sa BBIC_CAL_RX_START_REQ_STRU */
    ID_TOOL_BBIC_CAL_TX_START_REQ         = 0x1022, /* < @sa BBIC_CAL_TX_START_REQ_STRU */
    ID_TOOL_BBIC_CAL_GETX_POWER_START_REQ = 0x1023, /* < @sa BBIC_CAL_GETX_APC_START_REQ_STRU */

    /* hf cal  */
    ID_TOOL_BBIC_HFCAL_RX_START_REQ = 0x1031, /* < @sa BBIC_CAL_HF_RX_START_REQ_STRU */
    ID_TOOL_BBIC_HFCAL_TX_START_REQ = 0x1032, /* < @sa BBIC_CAL_HF_TX_START_REQ_STRU */
    ID_TOOL_BBIC_HF_DPD_START_REQ   = 0x1033, /* < @sa BBIC_CAL_HfDpdStartReq */
    ID_TOOL_BBIC_HF_DPD_TRIGGER_REQ = 0x1034, /* < @sa BBIC_CAL_HfDpdTriggerReq */

    /* self cal for rf debug */
    ID_TOOL_BBIC_CAL_RF_DEBUG_DCXO_CFIX_REQ = 0x1040, /* < @sa BBIC_CAL_RF_DEBUG_DCXO_CFIX_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_IP2_REQ       = 0x1041, /* < @sa BBIC_CAL_RF_DEBUG_IP2_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_RX_DCOC_REQ   = 0x1042, /* < @sa BBIC_CAL_RF_DEBUG_RX_DCOC_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_MRX_DCOC_REQ  = 0x1043, /* < @sa BBIC_CAL_RF_DEBUG_MRX_DCOC_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_TX_IQ_REQ     = 0x1044, /* < @sa BBIC_CAL_RF_DEBUG_TX_IQ_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_TX_LO_REQ     = 0x1045, /* < @sa BBIC_CAL_RF_DEBUG_TX_LO_REQ_STRU */

    /* TRX cal for rf debug */
    ID_TOOL_BBIC_CAL_RF_DEBUG_TX_REQ      = 0x1060, /* < @sa BBIC_CAL_RF_DEBUG_TX_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_GSM_TX_REQ  = 0x1061, /* < @sa BBIC_CAL_RF_DEBUG_GSM_TX_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_RX_REQ      = 0x1062, /* < @sa BBIC_CAL_RF_DEBUG_RX_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_RSSI_REQ    = 0x1063, /* < @sa BBIC_CAL_RF_DEBUG_RSSI_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_RX_EVM_REQ  = 0x1064, /* < @sa BBIC_CAL_RF_DEBUG_RX_EVM_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_DCXO_FE_REQ = 0x1065, /* < @sa BBIC_CAL_RF_DEBUG_DCXO_FE_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_DCXO_REQ    = 0x1066, /* < @sa BBIC_CAL_RF_DEBUG_DCXO_FE_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_TRX_REQ     = 0x1067, /* < @sa BBIC_CAL_RF_DEBUG_TRX_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_INT_REQ     = 0x1068, /* < @sa BBIC_CAL_RF_DEBUG_INIT_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_SRS_REQ     = 0x1069, /* < @sa BBIC_CAL_RF_DEBUG_SRS_REQ_STRU */
    ID_TOOL_BBIC_CAL_RF_DEBUG_TRX_TAS_REQ = 0x106a, /* < @sa BBIC_CAL_RF_DEBUG_TRA_TAS_REQ_STRU */

    ID_TOOL_BBIC_HFCAL_RF_DEBUG_TX_REQ   = 0x1070, /* < @sa BBIC_CAL_RF_DEBUG_HF_TX_REQ_STRU */
    ID_TOOL_BBIC_HFCAL_RF_DEBUG_RX_REQ   = 0x1071, /* < @sa BBIC_CAL_RF_DEBUG_HF_RX_REQ_STRU */
    ID_TOOL_BBIC_HFCAL_RF_DEBUG_RSSI_REQ = 0x1072, /* < @sa BBIC_CAL_RF_DEBUG_HF_RSSI_REQ_STRU */
    ID_TOOL_BBIC_HFCAL_ANA_TEMP_REQ      = 0x1073, /* < @sa BBIC_CAL_RF_HF_ANA_TEMP_REQ_PARA_STRU */
    ID_TOOL_BBIC_HFCAL_PRESET_REQ        = 0x1074, /* < @sa RFDEBUG_HFRX_PM2mPreSetReq */

    ID_TOOL_BBIC_REGISTER_READ_REQ   = 0x1080,
    ID_TOOL_BBIC_REGISTER_WRITE_REQ  = 0x1081,
    ID_TOOL_BBIC_MIPI_READ_REQ       = 0x1082,
    ID_TOOL_BBIC_MIPI_WRITE_REQ      = 0x1083,
    ID_TOOL_BBIC_ALINK_REG_READ_REQ  = 0x1084,
    ID_TOOL_BBIC_ALINK_REG_WRITE_REQ = 0x1085,
    ID_TOOL_BBIC_TEMP_QRY_REQ        = 0x1086,
    ID_TOOL_BBIC_PLL_QRY_REQ         = 0x1087,
    ID_TOOL_BBIC_DPDT_REQ            = 0x1088,
    ID_TOOL_BBIC_NV_RESET_REQ        = 0x1089,
    ID_TOOL_BBIC_RELOAD_NV_REQ       = 0x1090,
    ID_TOOL_BBIC_SET_BU_TYPE_IND     = 0x1091,
    ID_TOOL_BBIC_TRX_TAS_REQ         = 0x1092,
    ID_TOLL_BBIC_RELOAD_HF_NV_REQ    = 0x1093,

    /* Vcc Lut fast calibration */
    ID_TOOL_BBIC_CAL_TX_VCC_LUT_REQ  = 0x1095,
    ID_TOOL_BBIC_CAL_TX_VCC_COMP_REQ = 0x1096,

    /* ET Delay calibration */
    ID_TOOL_BBIC_CAL_ET_DELAY_REQ = 0x1097,

    /* Pa Gain Delta fast calibration */
    ID_TOOL_BBIC_CAL_PA_GAIN_DELTA_REQ = 0x1098,

    /* DPD calibration */
    ID_TOOL_BBIC_CAL_DPD_START_REQ = 0x1099,

    /* FASTCAL RFDEBUG TX REQ */
    ID_TOOL_BBIC_CT_TX_REQ  = 0x10a0,

    /* MT RX REQ */
    ID_AT_BBIC_CAL_MT_RX_REQ = 0x10a1,

    /* MT RX RSSI REQ */
    ID_AT_BBIC_CAL_MT_RX_RSSI_REQ = 0x10a2,

    /* MT TX REQ */
    ID_AT_BBIC_CAL_MT_TX_REQ = 0x10a3,

    ID_TOOL_BBIC_WRITE_NV_REQ       = 0x8023,
    ID_TOOL_BBIC_NV_WRITE_FLASH_REQ = 0x802D,

    ID_TOOL_BBIC_SET_LTEV_FTM_REQ = 0x90e5,

    ID_TOOL_BBIC_CAL_BUTT
};
typedef VOS_UINT16 TOOL_BBIC_CAL_MSG_TYPE_ENUM_UINT16;

enum BBIC_TOOL_CAL_MSG_TYPE_ENUM {
    ID_BBIC_TOOL_CAL_MSG_CNF = 0x2000, /* < @sa BBIC_CAL_MSG_CNF_STRU */

    /* Self cal for fast calibration */
    ID_BBIC_TOOL_CAL_DCXO_RESULT_IND      = 0x2001, /* < @sa BBIC_CAL_DCXO_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_RX_IQ_RESULT_IND     = 0x2002, /* < @sa BBIC_CAL_RX_IQ_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_TX_IQ_RESULT_IND     = 0x2003, /* < @sa BBIC_CAL_TX_IQ_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_TX_LO_RESULT_IND     = 0x2004, /* < @sa BBIC_CAL_TX_LO_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_IP2_RESULT_IND       = 0x2005, /* < @sa BBIC_CAL_IP2_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_MRX_DCOC_RESULT_IND  = 0x2006, /* < @sa BBIC_CAL_MRX_DCOC_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_RX_DCOC_RESULT_IND   = 0x2007, /* < @sa BBIC_CAL_RX_DCOC_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_RFIC_SELFCAL_IND     = 0x2008, /* < @sa BBIC_CAL_RFIC_SELFCAL_IND_STRU */
    ID_BBIC_TOOL_CAL_TX_FILTER_IND        = 0x2009, /* < @sa BBIC_CAL_TX_FLITER_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_MRX_DELAY_IND        = 0x200A, /* < @sa BBIC_CAL_MRX_DELAY_RESULT_IND_STRU */
    ID_BBIC_TOOL_HFCAL_TRXFDIQ_IND        = 0x2010,
    ID_BBIC_TOOL_HFCAL_RXFIIQ_IND         = 0x2011,
    ID_BBIC_TOOL_HFCAL_RXADC_IND          = 0x2012,
    ID_BBIC_TOOL_HFCAL_RX_DCOC_RESULT_IND = 0x2017, /* < @sa BBIC_CAL_HF_RX_DCOC_RESULT_IND_STRU */
    ID_TOOL_BBIC_CAL_DCXO_DCOC_RESULT_IND = 0x2018, /* < @sa BBIC_CAL_DcxoDcocCalInd */

    /* trx cal for fast calibration */
    ID_BBIC_TOOL_CAL_RX_RESULT_IND         = 0x2021, /* < @sa BBIC_CAL_RX_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_TX_RESULT_IND         = 0x2022, /* < @sa BBIC_CAL_TX_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_GETX_POWER_RESULT_IND = 0x2023, /* < @sa BBIC_CAL_GETX_APC_START_IND_STRU */

    ID_BBIC_TOOL_HFCAL_RX_RESULT_IND  = 0x2031, /* < @sa BBIC_CAL_HF_RX_RESULT_IND_STRU */
    ID_BBIC_TOOL_HFCAL_TX_RESULT_IND  = 0x2032, /* < @sa BBIC_CAL_HF_TX_RESULT_IND_STRU */
    ID_BBIC_TOOL_HFCAL_DPD_RESULT_IND = 0x2033, /* < @sa BBIC_CAL_HfDpdResultInd */
    ID_BBIC_TOOL_HFCAL_DPD_TRIGGER_CNF = 0x2034,

    /* self cal for rf debug */
    ID_BBIC_TOOL_CAL_RF_DEBUG_DCXO_RESULT_IND     = 0x2040, /* < @sa BBIC_CAL_RF_DEBUG_DCXO_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_RF_DEBUG_IP2_RESULT_IND      = 0x2041, /* < @sa BBIC_CAL_RF_DEBUG_IP2_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_RF_DEBUG_RX_DCOC_RESULT_IND  = 0x2042, /* < @sa BBIC_CAL_RF_DEBUG_RX_DCOC_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_RF_DEBUG_MRX_DCOC_RESULT_IND = 0x2043, /* < @sa BBIC_CAL_RF_DEBUG_MRX_DCOC_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_RF_DEBUG_TX_IQ_RESULT_IND    = 0x2044, /* < @sa BBIC_CAL_RF_DEBUG_TX_IQ_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_RF_DEBUG_TX_LO_RESULT_IND    = 0x2045, /* < @sa BBIC_CAL_RF_DEBUG_TX_LO_RESULT_IND_STRU */

    /* TRX cal for rf debug */
    ID_BBIC_TOOL_CAL_RF_DEBUG_TX_RESULT_IND = 0x2060, /* < @sa BBIC_CAL_RF_DEBUG_TX_RESULT_IND_STRU */
    ID_BBIC_TOOL_CAL_RF_DEBUG_RX_RSSI_IND   = 0x2061, /* < @sa BBIC_CAL_RF_DEBUG_RX_RSSI_IND_STRU */
    ID_BBIC_TOOL_CAL_RF_DEBUG_RX_EVM_IND    = 0x2062, /* < @sa BBIC_CAL_RF_DEBUG_RX_EVM_IND_STRU */
    ID_BBIC_TOOL_CAL_RF_DEBUG_GTX_MRX_IND   = 0x2063, /* < @sa BBIC_CAL_RF_DEBUG_GTX_MRX_IND_STRU */
    ID_BBIC_TOOL_CAL_RF_DEBUG_DCXO_IND      = 0x2066, /* < @sa BBIC_CAL_RF_DEBUG_GTX_MRX_IND_STRU */

    ID_BBIC_TOOL_HFCAL_RF_DEBUG_RX_RSSI_IND = 0x2071, /* < @sa BBIC_CAL_RF_DEBUG_HF_RX_RSSI_IND_STRU */
    ID_BBIC_TOOL_HFCAL_ANA_TEMP_IND         = 0x2072, /* < @sa BBIC_CAL_RF_HF_ANA_TEMP_IND_STRU */
    ID_TOOL_BBIC_HFCAL_PRESET_IND           = 0x2073, /* < @sa */

    ID_BBIC_TOOL_REGISTER_READ_IND   = 0x2080,
    ID_BBIC_TOOL_REGISTER_WRITE_IND  = 0x2081,
    ID_BBIC_TOOL_MIPI_READ_IND       = 0x2082,
    ID_BBIC_TOOL_MIPI_WRITE_IND      = 0x2083,
    ID_BBIC_TOOL_ALINK_REG_READ_IND  = 0x2084,
    ID_BBIC_TOOL_ALINK_REG_WRITE_IND = 0x2085,
    ID_BBIC_TOOL_TEMP_QRY_IND        = 0x2086,
    ID_BBIC_TOOL_PLL_QRY_IND         = 0x2087,
    ID_BBIC_TOOL_DPDT_IND            = 0x2088,
    ID_BBIC_TOOL_NV_RESET_IND        = 0x2089,
    ID_BBIC_TOOL_RELOAD_NV_IND       = 0x2090,
    ID_TOOL_BBIC_TRX_TAS_CNF         = 0x2092,

    ID_BBIC_TOOL_TX_VCC_LUT_RESULT_IND    = 0x2095,
    ID_BBIC_TOOL_ET_DELAY_K_RESULT_IND    = 0x2096, /* < @sa BBIC_CAL_ET_DELAY_K_RESULT_IND_STRU */
    ID_BBIC_TOOL_ET_DELAY_CAL_RESULT_IND  = 0x2097, /* < @sa BBIC_CAL_ET_DELAY_RESULT_IND_STRU */
    ID_BBIC_TOOL_PA_GAIN_DELTA_RESULT_IND = 0x2098,
    ID_BBIC_TOOL_CAL_DPD_RESULT_IND       = 0x2099,

    /* MT TX IND */
    ID_BBIC_CAL_AT_MT_TX_CNF    = 0x20a0,
    ID_BBIC_CAL_AT_MT_TX_PD_IND = 0x20a1,

    /* MT RX IND */
    ID_BBIC_CAL_AT_MT_RX_CNF      = 0x20a2,
    ID_BBIC_CAL_AT_MT_RX_RSSI_IND = 0x20a3,
    ID_BBIC_TOOL_CAL_SRS_CNF      = 0x20a4, /* < @sa BBIC_CAL_MSG_CNF_STRU */
    ID_BBIC_TOOL_CAL_TRX_TAS_CNF  = 0x20a5, /* < @sa BBIC_CAL_MSG_CNF_STRU */

    ID_BBIC_TOOL_WRITE_NV_CNF       = 0x8024,
    ID_BBIC_TOOL_NV_WRITE_FLASH_CNF = 0x802E,

    ID_BBIC_TOOL_SET_LTEV_FTM_CNF   = 0x90e6,

    ID_BBIC_TOOL_CAL_BUTT
};
typedef VOS_UINT16 BBIC_TOOL_CAL_MSG_TYPE_ENUM_UINT16;

enum MODU_TYPE_ENUM {
    MODU_TYPE_LTE_BPSK     = 0x0000, /* < BPSK */
    MODU_TYPE_LTE_PI2_BPSK = 0x0001, /* < PI/2 BPSK */
    MODU_TYPE_LTE_QPSK     = 0x0002,
    MODU_TYPE_LTE_16QAM    = 0x0003,
    MODU_TYPE_LTE_64QAM    = 0x0004,
    MODU_TYPE_LTE_256QAM   = 0x0005,
    MODU_TYPE_GMSK         = 0x0006,
    MODU_TYPE_8PSK         = 0x0007,
    MODU_TYPE_BUTT
};
typedef VOS_UINT16 MODU_TYPE_ENUM_UINT16;


enum NR_SCS_TYPE_COMM_ENUM {
    NR_SCS_TYPE_COMM_15   = 0x00,
    NR_SCS_TYPE_COMM_30   = 0x01,
    NR_SCS_TYPE_COMM_60   = 0x02,
    NR_SCS_TYPE_COMM_120  = 0x03,
    NR_SCS_TYPE_COMM_240  = 0x04,
    NR_SCS_TYPE_COMM_BUTT = 0x05,
};
typedef VOS_UINT8 NR_SCS_TYPE_COMM_ENUM_UINT8;



enum BBIC_CAL_GSM_RX_MODE_ENUM {
    BBIC_CAL_GSM_RX_MODE_BRUST      = 0x00, /* < Brust Measure */
    BBIC_CAL_GSM_RX_MODE_CONTINUOUS = 0x01, /* < Continuous Measure */
    BBIC_CAL_GSM_RX_MODE_BUTT       = 0x03
};
typedef VOS_UINT8 BBIC_CAL_GSM_RX_MODE_ENUM_UINT8;

enum BBIC_CAL_RX_AGC_MODE_ENUM {
    BBIC_CAL_RX_AGC_MODE_FAST = 0x00, /* < Fast Mode */
    BBIC_CAL_RX_AGC_MODE_SLOW = 0x01, /* < Slow Mode */
    BIC_CAL_RX_AGC_MODE_BUTT  = 0x03
};
typedef VOS_UINT8 BBIC_CAL_RX_AGC_MODE_ENUM_UINT8;


enum MRX_ESTIMATE_ENUM {
    MRX_ESTIMATE_POWER_MODE = 0x00, /* < MRX estimate with power mode */
    MRX_ESTIMATE_GAIN_MODE  = 0x01, /* < MRX estimate with gain mode */
    MRX_ESTIMATE_BUTT
};
typedef VOS_UINT8 MRX_ESTIMATE_ENUM_UINT8;

enum RAT_MODE_ENUM {
    RAT_MODE_GSM   = 0x0000,
    RAT_MODE_WCDMA = 0x0001,
    RAT_MODE_CDMA  = 0x0002,
    RAT_MODE_LTE   = 0x0003,
    RAT_MODE_NR    = 0x0004,
    RAT_MODE_BUTT  = 0x0005,
    RAT_MODE_NORAT = 0xFFFF,
};
typedef VOS_UINT16 RAT_MODE_ENUM_UINT16;

enum BAND_ENUM {
    BAND_1    = 0x0001,
    BAND_2    = 0x0002,
    BAND_3    = 0x0003,
    BAND_4    = 0x0004,
    BAND_5    = 0x0005,
    BAND_6    = 0x0006,
    BAND_7    = 0x0007,
    BAND_8    = 0x0008,
    BAND_9    = 0x0009,
    BAND_10   = 0x000A,
    BAND_11   = 0x000B,
    BAND_12   = 0x000C,
    BAND_13   = 0x000D,
    BAND_14   = 0x000E,
    BAND_15   = 0x000F,
    BAND_16   = 0x0010,
    BAND_17   = 0x0011,
    BAND_18   = 0x0012,
    BAND_19   = 0x0013,
    BAND_20   = 0x0014,
    BAND_21   = 0x0015,
    BAND_22   = 0x0016,
    BAND_23   = 0x0017,
    BAND_24   = 0x0018,
    BAND_25   = 0x0019,
    BAND_26   = 0x001A,
    BAND_27   = 0x001B,
    BAND_28   = 0x001C,
    BAND_29   = 0x001D,
    BAND_30   = 0x001E,
    BAND_31   = 0x001F,
    BAND_32   = 0x0020,
    BAND_33   = 0x0021,
    BAND_34   = 0x0022,
    BAND_35   = 0x0023,
    BAND_36   = 0x0024,
    BAND_37   = 0x0025,
    BAND_38   = 0x0026,
    BAND_39   = 0x0027,
    BAND_40   = 0x0028,
    BAND_41   = 0x0029,
    BAND_42   = 0x002A,
    BAND_43   = 0x002B,
    BAND_44   = 0x002C,
    BAND_45   = 0x002D,
    BAND_46   = 0x002E,
    BAND_47   = 0x002F,
    BAND_48   = 0x0030,
    BAND_49   = 0x0031,
    BAND_50   = 0x0032,
    BAND_51   = 0x0033,
    BAND_52   = 0x0034,
    BAND_53   = 0x0035,
    BAND_54   = 0x0036,
    BAND_55   = 0x0037,
    BAND_56   = 0x0038,
    BAND_57   = 0x0039,
    BAND_58   = 0x003A,
    BAND_59   = 0x003B,
    BAND_60   = 0x003C,
    BAND_61   = 0x003D,
    BAND_62   = 0x003E,
    BAND_63   = 0x003F,
    BAND_64   = 0x0040,
    BAND_65   = 0x0041,
    BAND_66   = 0x0042,
    BAND_70   = 0x0046,
    BAND_71   = 0x0047,
    BAND_74   = 0x004A,
    BAND_75   = 0x004B,
    BAND_76   = 0x004C,
    BAND_77   = 0x004D,
    BAND_78   = 0x004E,
    BAND_79   = 0x004F,
    BAND_80   = 0x0050,
    BAND_81   = 0x0051,
    BAND_82   = 0x0052,
    BAND_83   = 0x0053,
    BAND_84   = 0x0054,
    BAND_257  = 0x0101,
    BAND_258  = 0x0102,
    BAND_259  = 0x0103,
    BAND_260  = 0x0104,
    BAND_261  = 0x0105,
    BAND_BUTT = 0x0106,
};
typedef VOS_UINT16 BAND_ENUM_UINT16;

/*
 * Description: Bandwidth
 */
enum BANDWIDTH_ENUM {
    BANDWIDTH_200K   = 0x0000, /* < GSM */
    BANDWIDTH_1M2288 = 0x0001, /* < 1x/EVDO */
    BANDWIDTH_1M28   = 0x0002, /* < TDSCDMA */
    BANDWIDTH_1M4    = 0x0003, /* < LTE 1.4M */
    BANDWIDTH_3M     = 0x0004, /* < LTE 3M */
    BANDWIDTH_5M     = 0x0005,
    BANDWIDTH_10M    = 0x0006,
    BANDWIDTH_15M    = 0x0007,
    BANDWIDTH_20M    = 0x0008,
    BANDWIDTH_25M    = 0x0009,
    BANDWIDTH_30M    = 0x000A,
    BANDWIDTH_40M    = 0x000B,
    BANDWIDTH_50M    = 0x000C,
    BANDWIDTH_60M    = 0x000D,
    BANDWIDTH_80M    = 0x000E,
    BANDWIDTH_90M    = 0x000F,
    BANDWIDTH_100M   = 0x0010,
    BANDWIDTH_200M   = 0x0011,
    BANDWIDTH_400M   = 0x0012,
    BANDWIDTH_800M   = 0x0013,
    BANDWIDTH_1G     = 0x0014,
    BANDWIDTH_BUTT   = 0x0015
};
typedef VOS_UINT16 BANDWIDTH_ENUM_UINT16;

/*
 * Description: �ź�Դ��ʽ
 */
enum BBIC_CAL_SIGNAL_TYPE_ENUM {
    BBIC_CAL_SIGNAL_TYPE_TONE = 0x00, /* ����ģʽ */
    BBIC_CAL_SIGNAL_TYPE_DUAL_TONE,   /* ˫��ģʽ */
    BBIC_CAL_SIGNAL_TYPE_MODU,        /* ����ģʽ */
    BBIC_CAL_SIGNAL_TYPE_QUAD,        /* ����ģʽ */
    BBIC_CAL_SIGNAL_TYPE_TYPE_BUTT,
};
typedef VOS_UINT16 BBIC_CAL_SIGNAL_TYPE_ENUM_UINT16;


enum WAVEFORM_TYPE_ENUM {
    WAVEFORM_TYPE_CP_OFDM    = 0x0000,
    WAVEFORM_TYPE_DFT_S_OFDM = 0x0001,
    WAVEFORM_TYPE_BUTT
};
typedef VOS_UINT16 WAVEFORM_TYPE_ENUM_UINT16;


enum POWER_CTRL_MODE_ENUM {
    POWER_CTRL_MODE_POWER         = 0x00, /* < Power Mode */
    POWER_CTRL_MODE_RFIC_REGISTER = 0x01, /* < RFIC Register Mode */
    POWER_CTRL_MODE_GSM_VRAMP     = 0x02, /* < GSM Vramp mode */
    POWER_CTRL_MODE_BUTT          = 0x03
};
typedef VOS_UINT8 POWER_CTRL_MODE_ENUM_UINT8;

/*
 * Description: ETM��λģʽö�ٶ���
 */
enum BBIC_CAL_ETM_GAIN_MODE_ENUM {
    BBIC_CAL_ETM_MODE_AUTO = 0x00, /* ETM�Զ�ģʽ */
    BBIC_CAL_ETM_MODE_HIGH = 0x01, /* ETM������ģʽ */
    BBIC_CAL_ETM_MODE_MID  = 0x02, /* ETM������ģʽ */
    BBIC_CAL_ETM_MODE_BUTT = 0x03
};
typedef VOS_UINT8 BBIC_CAL_ETM_GAIN_MODE_ENUM_UINT8;

enum G_TX_MODU_ENUM {
    G_TX_MODU_GMSK = 0x00, /* < GMSK modulation */
    G_TX_MODU_8PSK = 0x01, /* < 8PSK modulation */
    G_TX_MODU_BUTT
};
typedef VOS_UINT16 G_TX_MODU_ENUM_UINT16;

enum BBIC_TEMP_CHANNEL_TYPE_ENUM {
    BBIC_TEMP_CHANNEL_TYPE_LOGIC = 0,
    BBIC_TEMP_CHANNEL_TYPE_PHY   = 1,

    BBIC_TEMP_CHANNEL_TYPE_BUTT,
};
typedef VOS_UINT16 BBIC_TEMP_CHANNEL_TYPE_ENUM_UINT16;

/*
 * �ṹ˵��: DPDT���̴���ҵ������ö��
 */
enum BBIC_DPDT_OPERTYPE_ENUM {
    BBIC_DPDT_OPERTYPE_SET = 0,
    BBIC_DPDT_OPERTYPE_GET = 1,
    BBIC_DPDT_OPERTYPE_BUTT
};
typedef VOS_UINT16 BBIC_DPDT_OPERTYPE_ENUM_UINT16;

/*
 * �ṹ˵��: DPDT���̴���ҵ������ö��
 */
enum BBIC_CAL_DCXO_SET_ENUM {
    BBIC_DCXO_SET_DISABLE = 0,
    BBIC_DCXO_SET_ENABLE  = 1,

    BBIC_DCXO_SET_BUTT
};
typedef VOS_UINT16 BBIC_CAL_DCXO_SET_ENUM_UINT16;

enum CBT_BUSINESS_TYPE_ENUM {

    CBT_BUSINESS_TYPE_HIRF = 0,
    CBT_BUSINESS_TYPE_CT   = 1,
    CBT_BUSINESS_TYPE_BT   = 2,
    CBT_BUSINESS_TYPE_MT   = 3,
    CBT_BUSINESS_TYPE_SAR  = 4,
    CBT_BUSINESS_TYPE_BUTT
};
typedef VOS_UINT16 CBT_BUSINESS_TYPE_ENUM_UINT16;

enum CBT_NosigType {

    CBT_NOSIG_TYPE_LF = 0,
    CBT_NOSIG_TYPE_HF = 1,
    CBT_NOSIG_TYPE_BUTT
};
typedef VOS_UINT16 CBT_NosigTypeUint16;

enum BBIC_CAL_PA_MODE_ENUM {
    BBIC_CAL_PA_GAIN_MODE_HIGH      = 0x00, /* < PA high gain mode */
    BBIC_CAL_PA_GAIN_MODE_MID       = 0x01, /* < PA medium gain mode */
    BBIC_CAL_PA_GAIN_MODE_LOW       = 0x02, /* < PA low gain mode */
    BBIC_CAL_PA_GAIN_MODE_ULTRA_LOW = 0x03, /* < PA ultra low gain mode */
    BBIC_CAL_PA_GAIN_MODE_BUTT      = 0x04
};
typedef VOS_UINT8 BBIC_CAL_PA_MODE_ENUM_UINT8;

/*
 * Description: Component ID
 */
typedef struct {
    VOS_UINT32 uwMsgId : 16;
    VOS_UINT32 uwRsv : 8;
    VOS_UINT32 uwComponentType : 4;
    VOS_UINT32 uwComponentID : 4;
} BBIC_CAL_COMPONENT_ID_STRU;

typedef struct _Component_Id {
    VOS_UINT8 ucRsv;
    VOS_UINT8 ucComponentType : 4;   /* 0x0: LTE, 0x1: TDS, 0x2: GSM, 0x3: UMTS, 0x4: CDMA, 0x5: NR, 0x7: LteV, 0xF: ��ģ�޹� */
    VOS_UINT8 ucComponentID   : 4;   /* OM/MSP:0x1, nosig:0x2, PHY:0x3 */
} COMPONENT_ID_STRU;


/*
 * Description: IQ result struct
 */
typedef struct {
    VOS_UINT16 uhwDcICode;   /* < DCI CODE */
    VOS_UINT16 uhwDcQCode;   /* < DCQ CODE */
    VOS_INT16  shwRemainDcI; /* < I Remain dc */
    VOS_INT16  shwRemainDcQ; /* < Q Remain dc */
} BBIC_CAL_DC_RESULT_STRU;

/*
 * Description: MRX Gain ctrl mode , use code(need dc result under this mode).
 */
typedef struct {
    VOS_UINT32    uwMrxGainCode; /* < Mrx Gain Code */
    BBIC_CAL_DC_RESULT_STRU stMrxDcConfig;
} MRX_AGC_GAIN_CODE_STRU;

/*
 * Description: PA config
 */
typedef struct {
    VOS_INT16  shwPaVccPwr; /* PA Vcc Target Power */
    VOS_INT16  shwPaVcc;    /* Vcc Code */
    VOS_UINT16 uhwPaBias;
    VOS_UINT16 uhwPaBias1;
    VOS_UINT16 uhwPaBias2;
    VOS_UINT16 uhwPaBias3;
    VOS_UINT8  ucPaVccMode : 1; /* < PA Vcc power mode ,0:apt , 1:et */
    VOS_UINT8  ucVccType : 2;   /* 0: ��ʾʹ��APC��������Vcc; 1: ��ʾʹ��Pa Vcc���ʲ�Vcc;
                         2: ��ʾֱ��ʹ��Vcc */
    VOS_UINT8                             ucRsv : 5;
    VOS_UINT8                             ucTxPaMode; /* < PA Mode, reference BBIC_CAL_PA_MODE_ENUM_UINT8 */
    BBIC_CAL_ETM_GAIN_MODE_ENUM_UINT8 enEtmMode;  /* ETM��λ����ģʽ, ���嶨��ο�BBIC_CAL_ETM_GAIN_MODE_ENUM_UINT8 */
    VOS_UINT8                             ucRsv1;
} TX_PA_CONFIG_STRU;

typedef struct {
    VOS_UINT32 uwRficGainCtrl; /* < RFIC CTRL */
    VOS_INT32  swDbbAtten;     /* < Dbb atten */
} TX_REG_CTRL_STRU;

/*
 * Description: Power para, Power mode or RFIC code mode
 */
typedef union {
    VOS_UINT16           uhwGsmTxVramp;   /* < Choice(POWER_CTRL_MODE_GSM_VRAMP) Gsm Tx Vpa Ctrl */
    VOS_INT16            shwTxPower;      /* < Choice(POWER_CTRL_MODE_POWER) Power */
    TX_REG_CTRL_STRU     stTxRegCtrlPara; /* < Choice(POWER_CTRL_MODE_RFIC_REGISTER) RFIC Ctrl */
} TX_POWER_CFG_UNION;

/*
 * Description: RF DEBUG COMMON Config para
 */
typedef struct {
    MODEM_ID_ENUM_UINT16             enModemId;     /* < Modem Id */
    RAT_MODE_ENUM_UINT16             enRatMode;     /* < Rat Mode */
    BAND_ENUM_UINT16                 enBand;        /* < Band No. */
    BANDWIDTH_ENUM_UINT16            enBandWith;    /* < BandWidth */
    BBIC_CAL_SIGNAL_TYPE_ENUM_UINT16 uhwSignalType; /* < refer to BBIC_CAL_SIGNAL_TYPE_ENUM_UINT16 */
    MODU_TYPE_ENUM_UINT16            enModType;     /* < valid under modulation signal */
    WAVEFORM_TYPE_ENUM_UINT16        enWaveType;    /* < valid under nr-sub6g modulation signal, */
    VOS_UINT16                       uhwMimoType;   /* < Mimo����,Ŀǰ����MTʹ��: 2 - 2T/2R, 4 - 4R, 8 - 8R */
    VOS_UINT32                       uwTxFreqInfo;  /* < Tx Frequence info, unit is khz */
    VOS_UINT32                       uwRxFreqInfo;  /* < Rx Frequence info, unit is khz */
    VOS_UINT16                       uhwUlPath;     /* < Ul Path Info */
    VOS_UINT16                       uhwDlPath;     /* < Dl Path Info */
} BBIC_CAL_RF_DEBUG_COMMON_PARA_STRU;

/*
 * Description: RF DEBUG TX Power Config para
 */
typedef struct {
    POWER_CTRL_MODE_ENUM_UINT8 enPowerCtrlMode; /* < Power Ctrl Mode */
    VOS_UINT8                  ucRsv;
    VOS_UINT16                 uhwVIq; /* < IQ Value,if signal tone */
    TX_PA_CONFIG_STRU          stPaPara;
    TX_POWER_CFG_UNION         unPowerPara;
    VOS_UINT32                 rfSetttingCode; /* 2G RF setting������ */
} BBIC_CAL_RF_DEBUG_TX_POWER_PARA_STRU;

/*
 * Description: GSM TX Slow debug config para
 */
typedef struct {
    G_TX_MODU_ENUM_UINT16 uhwModuType;
    VOS_UINT16            uhwRsv;
    BBIC_CAL_RF_DEBUG_TX_POWER_PARA_STRU stTxPowerPara;
    MRX_AGC_GAIN_CODE_STRU stMRxPara;
} G_TX_SINGLE_SLOT_PARA_STRU;

/*
 * Description: GSM TX Slow debug config para
 */
typedef struct {
    VOS_UINT16                         uhwTxEnable;  /* < 1--Tx enable, 0--Tx Disable */
    VOS_UINT16                         uhwMrxEanble; /* < Mrx Enable, 1--Enable, 0--Disable */
    BBIC_CAL_RF_DEBUG_COMMON_PARA_STRU stCommonInfo;
    VOS_UINT16                         uhwDataPattern; /* < 0��All 0��1��All 1��2��Random */
    MRX_ESTIMATE_ENUM_UINT8            enMrxEstMode;   /* < Mrx Measure Mode */
    VOS_UINT8                          ucRsv;
    VOS_UINT16                         uhwRsv;
    VOS_UINT16                         uhwSlotCnt; /* < ����ʱ϶������
                                         1~4 - ����ָ������ʹ����Щʱ϶��DSP�Զ�����1~4ʱ϶����
                                         8 - 8ʱ϶ȫ����ʹ�õ�һ��astSlotCfg�Ĳ��������� */
    G_TX_SINGLE_SLOT_PARA_STRU astSlotPara[4];     /* < ÿ��ʱ϶��������� */
} BBIC_CAL_RF_DEBUG_G_TX_PARA_STRU;

/*
 * Description: Tx Result Ind
 */
typedef struct {
    VOS_UINT32 swTxCorrValue;
    VOS_UINT32 swMrxCorrValue;
    VOS_INT16  antPower;
    VOS_INT16  rsv;
} GSM_MRX_IND_SINGLE_SLOT_STRU;

/*
 * Description: Tx Result Ind
 */
typedef struct {
    VOS_UINT32                       uwErrorCode;
    VOS_UINT32                       uwRsv;
    GSM_MRX_IND_SINGLE_SLOT_STRU     astSlotValue[BBIC_CAL_GSM_TX_SLOT_NUM];
} BBIC_CAL_RF_DEBUG_GTX_MRX_IND_PARA_STRU;

/*
 * Description: Msg cnf struct
 */
typedef struct {
    VOS_UINT16 uhwRecMsgInd;
    VOS_UINT16 uhwRsv;
    VOS_UINT32 uwErrorCode;
} BBIC_CAL_MSG_CNF_PARA_STRU;

typedef struct {
    VOS_UINT32 bitByteCnt : 5;     /* Byte Cnt */
    VOS_UINT32 bitMipiPortSel : 3; /* MIPI PORTѡ�� */
    VOS_UINT32 bitSecondaryId : 4;     /* Secondary Id */
    VOS_UINT32 bitRegAddr : 16;    /* �Ĵ�����ַ */
    VOS_UINT32 bitRsv : 4;
} BBIC_CAL_MIPIDEV_CMD_STRU;

typedef struct {
    VOS_UINT32 bitByte0 : 8;
    VOS_UINT32 bitByte1 : 8;
    VOS_UINT32 bitByte2 : 8;
    VOS_UINT32 bitByte3 : 8;
} BBIC_CAL_MIPIDEV_DATA_STRU;

typedef struct {
    BBIC_CAL_MIPIDEV_CMD_STRU  stCmd;
    BBIC_CAL_MIPIDEV_DATA_STRU stData;
} BBIC_CAL_MIPIDEV_UNIT_STRU;


typedef struct {
    VOS_UINT32                     uwMipiNum;
    BBIC_CAL_MIPIDEV_UNIT_STRU astData[BBIC_CAL_MAX_WR_MIPI_COUNT];
} BBIC_CAL_WRITE_MIPI_DATA_REQ_STRU;

/*
 * Description: ��MIPI������Ϣ
 */
typedef struct {
    VOS_UINT8                     uwMipiNum;
    VOS_UINT8                     readSpeedType;
    VOS_UINT8                     rsv[2];
    BBIC_CAL_MIPIDEV_CMD_STRU astCMD[BBIC_CAL_MAX_RD_MIPI_COUNT];
} BBIC_CAL_READ_MIPI_DATA_REQ_STRU;



typedef struct {
    VOS_UINT32 uwRegisterAddr;
    VOS_UINT32 uwRegisterData;
} BBIC_CAL_CPU_REGISTER_UNIT_STRU;

/*
 * Description: дCPU REG������Ϣ
 */
typedef struct {
    VOS_UINT32                          uwCpuRegisterNum;
    BBIC_CAL_CPU_REGISTER_UNIT_STRU astCpuData[BBIC_CAL_MAX_REGISTER_COUNT];
} BBIC_CAL_WRITE_REG_DATA_REQ_STRU;

/*
 * Description: ��CPU REG������Ϣ
 */
typedef struct {
    VOS_UINT32 uwCpuRegisterNum;
    VOS_UINT32 astCpuAddress[BBIC_CAL_MAX_REGISTER_COUNT];
} BBIC_CAL_READ_REG_DATA_REQ_STRU;

/*
 * Name        : BBIC_CAL_WRITE_MIPI_DATA_REQ_STRU
 * Description : ��MIPI���
 */
typedef BBIC_CAL_WRITE_MIPI_DATA_REQ_STRU BBIC_CAL_READ_MIPI_DATA_IND_STRU;

/*
 * Name        : BBIC_CAL_WRITE_REG_DATA_REQ_STRU
 * Description : ��CPU REG���
 */
typedef BBIC_CAL_WRITE_REG_DATA_REQ_STRU BBIC_CAL_READ_REG_DATA_IND_STRU;

/*
 * Description: PLL״̬��ȡ�ӿ�
 */
typedef struct {
    RAT_MODE_ENUM_UINT16  enRatMode; /* Ratģʽ */
    MODU_TYPE_ENUM_UINT16 enModType; /* ����ģʽ GSMʹ�� */
    VOS_UINT16            uhwBand;   /* Band */
    VOS_UINT16            uhwRsv;
} BBIC_CAL_PLL_QRY_REQ_PARA_STRU;

/*
 * Description: PLL״̬��ȡ�ϱ��ӿ�
 */
typedef struct {
    VOS_UINT32 uwErrorCode;
    VOS_UINT32 uwRxPllStatus;
    VOS_UINT32 uwTxPllStatus;
} BBIC_CAL_PLL_QRY_IND_PARA_STRU;

/*
 * Description: DPDT���ýӿ�
 */
typedef struct {
    RAT_MODE_ENUM_UINT16           enRatMode;
    BBIC_DPDT_OPERTYPE_ENUM_UINT16 enOperType; /* ����DPDT״̬���ǻ�ȡDPDT״̬, 0--��ʾ����DPDT״̬,1--��ȡDPDT״̬ */
    VOS_UINT32                     uwValue;    /* Ŀǰֻ���˵�8bit����4bit��ʾTAS/MAS״̬,��4bit��ʾHALL״̬,
���е�4BIT��������:
0: TAS-ֱͨ  MAS-ֱͨ
1: TAS-����  MAS-ֱͨ
2: TAS-ֱͨ  MAS-����
3: TAS-����  MAS-���� */
    VOS_UINT16 uhwModemWorkType;                   /* ҵ��������,0--��ʾҵ��1--���� */
    VOS_UINT16 uhwRsv;
} BBIC_CAL_DPDT_REQ_PARA_STRU;

/*
 * Description: DPDT�����ϱ��ӿ�
 */
typedef struct {
    VOS_UINT32                         uwErrorCode;
    BBIC_DPDT_OPERTYPE_ENUM_UINT16     unOperType;
    VOS_UINT16                         uhwRsv;
    VOS_UINT32                         uwValue;
} BBIC_CAL_DPDT_IND_PARA_STRU;

/*
 * Description: TRX TAS���õľ������
 */
typedef struct {
    VOS_UINT16 ratMode;
    VOS_UINT16 band;
    VOS_UINT32 trxTasValue;
} BBIC_CAL_SetTrxTasReqParaStru;

/*
 * Description: TRX TAS���ý���ϱ��ӿ�
 */
typedef struct {
    VOS_UINT32 errorCode;
} BBIC_CAL_SetTrxTasCnfParaStru;

/*
 * Description: �¶Ȼ�ȡ�ӿ�
 */
typedef struct {
    BBIC_TEMP_CHANNEL_TYPE_ENUM_UINT16 enChannelType;
    VOS_INT16                              hwChannelNum;
} BBIC_CAL_TEMP_QRY_REQ_PARA_STRU;

/*
 * Description: �¶Ȼ�ȡ�ϱ��ӿ�
 */
typedef struct {
    VOS_UINT32                             uwErrorCode;
    BBIC_TEMP_CHANNEL_TYPE_ENUM_UINT16 enChannelType;
    VOS_INT16                              hwChannelNum;
    VOS_INT32                              wTemperature;
} BBIC_CAL_TEMP_QRY_IND_PARA_STRU;

/*
 * Description: PLL״̬��ȡ����ϱ���Ϣ�ṹ��
 */
typedef struct {
    BBIC_CAL_DCXO_SET_ENUM_UINT16 enSetType;
    VOS_UINT16                    usBand;
    VOS_UINT32                    txArfcn; /* ����Ƶ��� */
} BBIC_CAL_DCXO_REQ_PARA_STRU;

/*
 * Description: ÿ��Band�µ�RSSI�ϱ������֧��8R
 */
typedef struct {
    VOS_UINT16 validAntNum;
    VOS_UINT16 usRsv;
    VOS_INT16  rxRssi[RX_MAX_ANT_NUM];
} BBIC_CAL_RX_RSSI_IND_BAND_STRU;

/*
 * Description: ���CC��RSSI�ϱ������֧��5CC
 */
typedef struct {
    VOS_UINT32                         errorCode;
    VOS_UINT32                         uwRsv;
    VOS_UINT16                         validCcNum;
    VOS_UINT16                         usRsv;
    BBIC_CAL_RX_RSSI_IND_BAND_STRU rssiRpt[RX_MAX_CC_NUM];
} BBIC_CAL_RF_DEBUG_RX_RSSI_REPORT_PARA_STRU;

/*
 * Description: LOADPHY�Ĳ���
 */
typedef struct {
    CBT_BUSINESS_TYPE_ENUM_UINT16 enBusiness : 4;   /* ҵ�����ͣ��������㶨��һ�� 0:HIRF 1:CT 2:NSBT 3:MT */
    VOS_UINT16                    usRfChanType : 4; /* 0:ҵ��       1:����  ֻ��GUC BT����Ч */
    VOS_UINT16                    usModeType : 4;   /* 0:��ģ����   1:˫ģ����(��ʱֻ֧��HIRF�µ�L+NR) */
    CBT_NosigTypeUint16           nosigType : 4;    /* ����ָʾ��Ҫ���ѵ�Ƶ���Ǹ�Ƶ */
    /* ��enBusinessΪ0:HIRFʱ��ͬʱ���ѵ�Ƶ�͸�Ƶ */
    /* ��enBusinessΪ2:NSBT�����軽�ѣ���Phy���л��� */
    /* ��enBusinessΪ����ֵʱ��GUCLģʱǿ�ƻ��ѵ�Ƶ��NRģʱ���ݴ˲�������������˭ */
    VOS_UINT16               usRsv1;        /* ���ں�����չ */
    RAT_MODE_ENUM_UINT16 aenRatMode[2]; /* ��ģ������ʹ��ausRatMode[0]
                       ˫ģ������ʹ��ausRatMode[0]��ausRatMode[1]
                       RatMode       0:GSM 1:WCDMA 2:CDMA 3:LTE 4:NR
                                     0xffff:BUTT(���ڲ�����ģ�ĳ���ʱ����RFDSP) */
} LOADPHY_PARA_STRU;


/*
 * Description: Msg cnf struct
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    BBIC_CAL_MSG_CNF_PARA_STRU stPara;
} BBIC_CAL_MSG_CNF_STRU;


typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU       stComponentID;
    BBIC_CAL_READ_MIPI_DATA_REQ_STRU stPara;
} BBIC_CAL_RF_DEBUG_READ_MIPI_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU       stComponentID;
    VOS_UINT32                       uwResult;
    BBIC_CAL_READ_MIPI_DATA_IND_STRU stPara;
} BBIC_CAL_RF_DEBUG_READ_MIPI_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU        stComponentID;
    BBIC_CAL_WRITE_MIPI_DATA_REQ_STRU stPara;
} BBIC_CAL_RF_DEBUG_WRITE_MIPI_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    VOS_UINT32                 uwResult;
} BBIC_CAL_RF_DEBUG_WRITE_MIPI_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    VOS_UINT32                 uwResult;
} BBIC_CAL_RF_DEBUG_WRITE_MIPI_REQ_IND_STRU;

/*
 * Description: PLL״̬��ȡ������Ϣ�ṹ��
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    BBIC_CAL_PLL_QRY_REQ_PARA_STRU stPara;
} BBIC_CAL_PLL_QRY_REQ_STRU;

/*
 * Description: PLL״̬��ȡ����ϱ���Ϣ�ṹ��
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU     stComponentID;
    BBIC_CAL_PLL_QRY_IND_PARA_STRU stPara;
} BBIC_CAL_PLL_QRY_IND_STRU;

/*
 * Description: DPDT���ò�ѯ������Ϣ�ṹ��
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU  stComponentID;
    BBIC_CAL_DPDT_REQ_PARA_STRU stPara;
} BBIC_CAL_DPDT_REQ_STRU;

/*
 * Description: DPDT���ò�ѯ����ϱ���Ϣ�ṹ��
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU  stComponentID;
    BBIC_CAL_DPDT_IND_PARA_STRU stPara;
} BBIC_CAL_DPDT_IND_STRU;

/*
 * Description: TRX TAS����ԭ��
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU    componentId;
    BBIC_CAL_SetTrxTasReqParaStru data;
} BBIC_CAL_SetTrxTasReqStru;

/*
 * Description: TRX TAS CNF PARA
 */
typedef struct {
    UINT16 uhwRecMsgInd;
    UINT16 uhwRsv;
    UINT32 uwErrorCode; /* ������ */
    UINT8  ucNvPortIdx;
    UINT8  ucRsv[3];
} BBIC_CAL_RF_DEBUG_TRX_TAS_CNF_PARA_STRU;

/*
 * Description: TRX TAS CNF
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU              stComponentID;
    BBIC_CAL_RF_DEBUG_TRX_TAS_CNF_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_TRX_TAS_CNF_STRU;

/*
 * Description: TRX TAS������Ӧԭ��
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU    componentId;
    BBIC_CAL_SetTrxTasCnfParaStru data;
} BBIC_CAL_SetTrxTasCnfStru;


typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU      stComponentID;
    BBIC_CAL_TEMP_QRY_REQ_PARA_STRU stPara;
} BBIC_CAL_TEMP_QRY_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU      stComponentID;
    BBIC_CAL_TEMP_QRY_IND_PARA_STRU stPara;
} BBIC_CAL_TEMP_QRY_IND_STRU;


/*
 * Description: PLL״̬��ȡ����ϱ���Ϣ�ṹ��
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU  stComponentID;
    BBIC_CAL_DCXO_REQ_PARA_STRU stPara;
} BBIC_CAL_DCXO_REQ_STRU;

/*
 * Description: PLL״̬��ȡ����ϱ���Ϣ�ṹ��
 */
typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    VOS_UINT32                 ulErrorCode;
} BBIC_CAL_DCXO_IND_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU stComponentID;
    VOS_UINT32                     modemId;
} BBIC_CAL_RF_DEBUG_RSSI_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER
    BBIC_CAL_COMPONENT_ID_STRU                 stComponentID;
    BBIC_CAL_RF_DEBUG_RX_RSSI_REPORT_PARA_STRU stPara;
} BBIC_CAL_RF_DEBUG_RX_RSSI_IND_STRU;


/*
 * 7. OTHER declarations
 */

/*
 * 8. Global  declaration
 */

/*
 * 9. Function declarations
 */

#pragma pack(pop)

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif