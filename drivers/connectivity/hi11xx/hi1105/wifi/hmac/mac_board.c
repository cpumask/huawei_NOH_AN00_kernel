


/* 1 ͷ�ļ����� */
#include "wlan_spec.h"
#include "mac_board.h"
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_BOARD_C

/* 2 ȫ�ֱ������� */
/* HOST CRX�ӱ� */
#if IS_HOST
frw_event_sub_table_item_stru g_ast_dmac_host_crx_table[HMAC_TO_DMAC_SYN_BUTT];
#endif
/* DMACģ�飬HOST_DRX�¼�������ע��ṹ���� */
frw_event_sub_table_item_stru g_ast_dmac_tx_host_drx[DMAC_TX_HOST_DRX_BUTT];

/* DMACģ�飬WLAN_DTX�¼�������ע��ṹ���� */
frw_event_sub_table_item_stru g_ast_dmac_tx_wlan_dtx[DMAC_TX_WLAN_DTX_BUTT];

#if IS_HOST
/* DMACģ�飬WLAN_CTX�¼�������ע��ṹ���� */
frw_event_sub_table_item_stru g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_BUTT];
#endif
/* DMACģ��,WLAN_DRX�¼�������ע��ṹ���� */
frw_event_sub_table_item_stru g_ast_dmac_wlan_drx_event_sub_table[HAL_WLAN_DRX_EVENT_SUB_TYPE_BUTT];

/* DMACģ�飬high prio�¼�������ע��ṹ�嶨�� */
frw_event_sub_table_item_stru g_ast_dmac_high_prio_event_sub_table[HAL_EVENT_DMAC_HIGH_PRIO_SUB_TYPE_BUTT];

/* DMACģ��,WLAN_CRX�¼�������ע��ṹ���� */
frw_event_sub_table_item_stru g_ast_dmac_wlan_crx_event_sub_table[HAL_WLAN_CRX_EVENT_SUB_TYPE_BUTT];

/* DMACģ�飬TX_COMP�¼�������ע��ṹ���� */
frw_event_sub_table_item_stru g_ast_dmac_tx_comp_event_sub_table[HAL_TX_COMP_SUB_TYPE_BUTT];

/* WLAN_DTX �¼������ͱ� */
frw_event_sub_table_item_stru g_ast_hmac_wlan_dtx_event_sub_table[DMAC_TX_WLAN_DTX_BUTT];

/* HMACģ�� WLAN_DRX�¼�������ע��ṹ���� */
frw_event_sub_table_item_stru g_ast_hmac_wlan_drx_event_sub_table[DMAC_WLAN_DRX_EVENT_SUB_TYPE_BUTT];

/* HMACģ�� WLAN_CRX�¼�������ע��ṹ���� */
frw_event_sub_table_item_stru g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_BUTT];

/* HMACģ�� ����HOST��������¼�������ע��ṹ���� */
frw_event_sub_table_item_stru g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_SYN_BUTT];

/* HMACģ�� MISC��ɢ�¼�������ע��ṹ���� */
frw_event_sub_table_item_stru g_ast_hmac_wlan_misc_event_sub_table[DMAC_MISC_SUB_TYPE_BUTT];

/* HMACģ�� DDR RX����ע��ṹ���� */
frw_event_sub_table_item_stru g_ast_hmac_ddr_drx_sub_table[HAL_WLAN_DDR_DRX_EVENT_SUB_TYPE_BUTT];

/* 3 ����ʵ�� */


void event_fsm_table_register(void)
{
    /* Part1: ������Dmac�յ��¼� */
    /* ע��DMACģ��HOST_CRX�¼� */
    frw_event_table_register(FRW_EVENT_TYPE_HOST_CRX, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_dmac_host_crx_table);

    /* ע��DMACģ��HOST_DRX�¼��������� */
    frw_event_table_register(FRW_EVENT_TYPE_HOST_DRX, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_dmac_tx_host_drx);

    /* ע��DMACģ��WLAN_DTX�¼��������� */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_DTX, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_dmac_tx_wlan_dtx);

    /* ע��DMACģ��WLAN_CTX�¼��������� */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_CTX, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_dmac_wlan_ctx_event_sub_table);

    /* ע��DMACģ��WLAN_DRX�¼��ӱ� */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_DRX, FRW_EVENT_PIPELINE_STAGE_0,
        g_ast_dmac_wlan_drx_event_sub_table);

    /* ע��DMACģ��WLAN_CRX�¼�pipeline 0�ӱ� */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_CRX, FRW_EVENT_PIPELINE_STAGE_0,
        g_ast_dmac_wlan_crx_event_sub_table);

    /* ע��DMACģ��TX_COMP�¼��ӱ� */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_TX_COMP, FRW_EVENT_PIPELINE_STAGE_0,
        g_ast_dmac_tx_comp_event_sub_table);

    frw_event_table_register(FRW_EVENT_TYPE_HIGH_PRIO, FRW_EVENT_PIPELINE_STAGE_0,
        g_ast_dmac_high_prio_event_sub_table);
    /* Part2: ������Hmac�յ��¼� */
    /* ע��WLAN_DTX�¼��ӱ� */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_DTX, FRW_EVENT_PIPELINE_STAGE_0,
        g_ast_hmac_wlan_dtx_event_sub_table);

    /* ע��WLAN_DRX�¼��ӱ� */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_DRX, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_hmac_wlan_drx_event_sub_table);

    /* ע��HMACģ��WLAN_CRX�¼��ӱ� */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_CRX, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_hmac_wlan_crx_event_sub_table);

    /* ע��DMACģ��MISC�¼��ֱ� */
    frw_event_table_register(FRW_EVENT_TYPE_DMAC_MISC, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_hmac_wlan_misc_event_sub_table);

    /* ע��ͳ�ƽ����ѯ�¼��ӱ� */
    frw_event_table_register(FRW_EVENT_TYPE_HOST_SDT_REG, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_hmac_wlan_ctx_event_sub_table);

    frw_event_table_register(FRW_EVENT_TYPE_HOST_DDR_DRX, FRW_EVENT_PIPELINE_STAGE_0,
        g_ast_hmac_ddr_drx_sub_table);
}


void event_fsm_unregister(void)
{
    /* Part1: ������Dmac����¼� */
    /* ȥע��DMACģ��HOST_CRX�¼� */
    memset_s(g_ast_dmac_host_crx_table, OAL_SIZEOF(g_ast_dmac_host_crx_table),
             0, OAL_SIZEOF(g_ast_dmac_host_crx_table));

    /* ȥע��DMACģ��HOST_DRX�¼��������� */
    memset_s(g_ast_dmac_tx_host_drx, OAL_SIZEOF(g_ast_dmac_tx_host_drx),
             0, OAL_SIZEOF(g_ast_dmac_tx_host_drx));

    /* ȥע��DMACģ��WLAN_DTX�¼��������� */
    memset_s(g_ast_dmac_tx_wlan_dtx, OAL_SIZEOF(g_ast_dmac_tx_wlan_dtx),
             0, OAL_SIZEOF(g_ast_dmac_tx_wlan_dtx));

    /* ȥע��DMACģ��WLAN_DRX�¼��ӱ� */
    memset_s(g_ast_dmac_wlan_drx_event_sub_table, OAL_SIZEOF(g_ast_dmac_wlan_drx_event_sub_table),
             0, OAL_SIZEOF(g_ast_dmac_wlan_drx_event_sub_table));

    /* ȥע��DMACģ��WLAN_CRX�¼��ӱ� */
    memset_s(g_ast_dmac_wlan_crx_event_sub_table, OAL_SIZEOF(g_ast_dmac_wlan_crx_event_sub_table),
             0, OAL_SIZEOF(g_ast_dmac_wlan_crx_event_sub_table));

    /* ȥע��DMACģ��TX_COMP�¼��ӱ� */
    memset_s(g_ast_dmac_tx_comp_event_sub_table, OAL_SIZEOF(g_ast_dmac_tx_comp_event_sub_table),
             0, OAL_SIZEOF(g_ast_dmac_tx_comp_event_sub_table));

    memset_s(g_ast_dmac_high_prio_event_sub_table, OAL_SIZEOF(g_ast_dmac_high_prio_event_sub_table),
             0, OAL_SIZEOF(g_ast_dmac_high_prio_event_sub_table));
    /* Part2: ������Hmac����¼� */
    memset_s(g_ast_hmac_wlan_dtx_event_sub_table, OAL_SIZEOF(g_ast_hmac_wlan_dtx_event_sub_table),
             0, OAL_SIZEOF(g_ast_hmac_wlan_dtx_event_sub_table));

    memset_s(g_ast_hmac_wlan_drx_event_sub_table, OAL_SIZEOF(g_ast_hmac_wlan_drx_event_sub_table),
             0, OAL_SIZEOF(g_ast_hmac_wlan_drx_event_sub_table));

    memset_s(g_ast_hmac_wlan_crx_event_sub_table, OAL_SIZEOF(g_ast_hmac_wlan_crx_event_sub_table),
             0, OAL_SIZEOF(g_ast_hmac_wlan_crx_event_sub_table));

    memset_s(g_ast_hmac_wlan_ctx_event_sub_table, OAL_SIZEOF(g_ast_hmac_wlan_ctx_event_sub_table),
             0, OAL_SIZEOF(g_ast_hmac_wlan_ctx_event_sub_table));

    memset_s(g_ast_hmac_wlan_misc_event_sub_table, OAL_SIZEOF(g_ast_hmac_wlan_misc_event_sub_table),
             0, OAL_SIZEOF(g_ast_hmac_wlan_misc_event_sub_table));

    memset_s(g_ast_hmac_ddr_drx_sub_table, OAL_SIZEOF(g_ast_hmac_ddr_drx_sub_table),
        0, OAL_SIZEOF(g_ast_hmac_ddr_drx_sub_table));
}

/*lint -e19*/
oal_module_symbol(g_ast_dmac_host_crx_table);
oal_module_symbol(g_ast_dmac_tx_host_drx);
oal_module_symbol(g_ast_dmac_tx_wlan_dtx);
oal_module_symbol(g_ast_dmac_wlan_ctx_event_sub_table);
oal_module_symbol(g_ast_dmac_wlan_drx_event_sub_table);
oal_module_symbol(g_ast_dmac_high_prio_event_sub_table);
oal_module_symbol(g_ast_dmac_wlan_crx_event_sub_table);
oal_module_symbol(g_ast_dmac_tx_comp_event_sub_table);
oal_module_symbol(g_ast_hmac_wlan_dtx_event_sub_table);
oal_module_symbol(g_ast_hmac_wlan_drx_event_sub_table);
oal_module_symbol(g_ast_hmac_wlan_crx_event_sub_table);
oal_module_symbol(g_ast_hmac_wlan_ctx_event_sub_table);
oal_module_symbol(g_ast_hmac_wlan_misc_event_sub_table);
oal_module_symbol(g_ast_hmac_ddr_drx_sub_table);

oal_module_symbol(event_fsm_table_register);
oal_module_symbol(event_fsm_unregister); /*lint +e19*/
/* 2 ȫ�ֱ������� */
/* DFX����ȫ�ֱ������� */
#ifdef _PRE_WLAN_DFT_STAT
dfx_performance_log_switch_enum_uint8 g_auc_dfx_performance_log_switch[DFX_PERFORMANCE_LOG_BUTT] = {0};
#endif

/* 3 ����ʵ�� */
/* lint +e19 */
/* DFX���ú���ʵ�� */
#ifdef _PRE_WLAN_DFT_STAT

uint32_t dfx_get_performance_log_switch_enable(
    dfx_performance_log_switch_enum_uint8 uc_performance_log_switch_type)
{
    return g_auc_dfx_performance_log_switch[uc_performance_log_switch_type];
}

void dfx_set_performance_log_switch_enable(dfx_performance_log_switch_enum_uint8 uc_performance_log_switch_type,
                                           uint8_t uc_value)
{
    if (uc_performance_log_switch_type >= DFX_PERFORMANCE_LOG_BUTT) {
        oam_warning_log1(0, OAM_SF_ANY,
                         "dfx_set_performance_log_switch_enable::log_switch type:%d.",
                         uc_performance_log_switch_type);
        return;
    }
    g_auc_dfx_performance_log_switch[uc_performance_log_switch_type] = uc_value;
}
#endif

