
#ifndef NAS_CCBT_H
#define NAS_CCBT_H

/*
 * 1. Other files included
 */

#ifdef WIN32
#pragma warning(disable : 4200) /* zero-sized array in struct/union */
#endif

#include "vos.h"
#include "bbic_mt_cal_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#pragma pack(push, 4)

/*
 * 2. Macro definitions
 */
#define ID_CCBT_MTA_LOAD_PS_RRC_REQ 0x0175
#define ID_MTA_CCBT_LOAD_PS_RRC_CNF 0x0175

#define ID_CCBT_MTA_QRY_PS_RRC_LOAD_STATUS_REQ 0x0176
#define ID_MTA_CCBT_QRY_PS_RRC_LOAD_STATUS_CNF 0x0176

#define ID_AT_CCBT_SET_WORK_MODE_REQ 0x0177
#define ID_CCBT_AT_SET_WORK_MODE_CNF 0x0177

#define ID_AT_UECBT_RFIC_MEM_TEST_REQ 0x0178
#define ID_UECBT_AT_RFIC_MEM_TEST_CNF 0x0178

#define ID_AT_UECBT_RFIC_DIE_IE_QUERY_REQ 0x0179
#define ID_UECBT_AT_RFIC_DIE_IE_QUERY_CNF 0x0179

#define ID_CCBT_MTA_NV_REFRESH_RSP 0x2002 /* ��ֹCCBT->MTA������Ϣ��ID_MTA_NV_REFRESH_RSPȡֵ�ظ� */

#define DIE_ID_MAX_LEN_BYTE 64
#define DIE_ID_QUERY_CHIP_MAX_NUM 20

/*
 * 3. Enumerations declarations
 */
/*
 * Name: DIE_ID_QUERY_CHIP_TYPE
 * Description: DIE ID��ѯ��оƬ����ö��
 */
enum DIE_ID_QUERY_CHIP_TYPE {
    DIE_ID_QUERY_CHIP_TYPE_RFIC_HI6365,
    DIE_ID_QUERY_CHIP_TYPE_RFIC_HI6370,
    DIE_ID_QUERY_CHIP_TYPE_AIP_6P01,
    DIE_ID_QUERY_CHIP_TYPE_PMU_6422V500,

    DIE_ID_QUERY_CHIP_TYPE_BUTT
};

typedef VOS_UINT16 DIE_ID_QUERY_CHIP_TYPE_UINT16;

/*
 * 4. Message Header declaration
 */

/*
 * 5. Message declaration
 */

/*
 * 6. STRUCT and UNION declaration
 */

/*
 * 7. OTHER declarations
 */

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 enMsgId;
    VOS_UINT16 usOpId;
} CCBT_MTA_MSG_HEADER_STRU;

/*
 * Name: CCBT_MTA_LOAD_PS_RRC_REQ_STRU
 * Description:
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16              enMsgId;
    VOS_UINT16              usOpId;
    VOS_RATMODE_ENUM_UINT32 enRatType;
} CCBT_MTA_LOAD_PS_RRC_REQ_STRU;

/*
 * Name: MTA_CCBT_LOAD_PS_RRC_CNF_STRU
 * Description:
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16              usTransPrimId; /* 0x6001 */
    VOS_UINT16              usReserve;
    VOS_UINT16              enMsgId;
    VOS_UINT16              usOpId;
    VOS_RATMODE_ENUM_UINT32 enRatType;
    VOS_UINT32              ulIsLoaded; /* VOS_OK(0)��ʾ�ɹ���VOS_ERR(1)��ʾʧ�� */
} MTA_CCBT_LOAD_PS_RRC_CNF_STRU;

/*
 * Name: CCBT_MTA_QRY_PS_RRC_LOAD_STATUS_REQ
 *       ID_CCBT_MTA_QRY_PS_RRC_LOAD_STATUS_REQ
 * Description:
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16              enMsgId;
    VOS_UINT16              usOpId;
    VOS_RATMODE_ENUM_UINT32 enRatType;
} CCBT_MTA_QRY_PS_RRC_LOAD_STATUS_REQ_STRU;

/*
 * Name: MTA_CCBT_QRY_PS_RRC_LOAD_STATUS_CNF
 * Description:
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16              usTransPrimId; /* 0x6001 */
    VOS_UINT16              usReserve;
    VOS_UINT16              enMsgId;
    VOS_UINT16              usOpId;
    VOS_RATMODE_ENUM_UINT32 enRatType;
    VOS_UINT32              ulIsLoaded; /* VOS_OK(0)��ʾ�ɹ���VOS_ERR(1)��ʾʧ�� */
} MTA_CCBT_QRY_PS_RRC_LOAD_STATUS_CNF_STRU;

/*
 * Name: AT_CCBT_LOAD_PHY_REQ_STRU
 * Description: AT->UE���������REQ��Ϣ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16        usMsgId;
    COMPONENT_ID_STRU stComponentId;
    LOADPHY_PARA_STRU stLoadphyPara;
} AT_CCBT_LOAD_PHY_REQ_STRU;

/*
 * Name: AT_CCBT_LOAD_PHY_CNF_STRU
 * Description: UE->AT���������Cnf��Ϣ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16        usMsgId;
    COMPONENT_ID_STRU stComponentId;
    VOS_UINT32        ulErrorCode; /* 0��ʾ�ɹ���N(1,2,3...)��ʾ��ͬ���� */
} AT_CCBT_LOAD_PHY_CNF_STRU;

/*
 * Name: AT_UECBT_RficMemTestReqStru
 * Description: ����RFIC��mem�ϻ��������󣬾������̵ײ����
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16        msgId;
    COMPONENT_ID_STRU componentId;
} AT_UECBT_RficMemTestReqStru;

/*
 * Name: UECBT_AT_RficMemTestCnfStru
 * Description: ����RFIC��mem�ϻ��������󣬾������̵ײ����
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16        msgId;
    COMPONENT_ID_STRU componentId;
    VOS_UINT32        errorCode; /* 0��ʾ�ɹ���ʧ�ܰ���bitλ��1 */
} UECBT_AT_RficMemTestCnfStru;

/*
 * Name: AT_UECBT_DieId_Query_Req
 * Description: ����UECBT��ѯDIE ID������
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16        msgId;
    COMPONENT_ID_STRU componentId;
} AT_UECBT_DieId_Query_Req;

/*
 * Name: DIE_ID_INFO
 * Description: DIE ID��Ϣ
 */
typedef struct {
    DIE_ID_QUERY_CHIP_TYPE_UINT16 chipType;
    VOS_UINT16                    infoSize; /* DIE ID��Ϣ��ʵ�ʳ��ȣ���λbyte */
    VOS_UINT8                     data[DIE_ID_MAX_LEN_BYTE];
} DIE_ID_INFO;

/*
 * Name: UECBT_AT_DieId_Query_Ind
 * Description: UECBT��ѯDIE ID�Ľ���ϱ�
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16        msgId;
    COMPONENT_ID_STRU componentId;
    VOS_UINT32        errorCode;
    VOS_UINT32        chipNum;
    DIE_ID_INFO       dieIdInfo[0]; /* �䳤������chipNum�����ΪDIE_ID_QUERY_CHIP_MAX_NUM 32 */
} UECBT_AT_DieId_Query_Ind;

/*
 * 8. Global  declaration
 */

/*
 * 9. Function declarations
 */

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif

