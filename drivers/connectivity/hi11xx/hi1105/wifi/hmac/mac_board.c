


/* 1 头文件包含 */
#include "wlan_spec.h"
#include "mac_board.h"
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_BOARD_C

/* 2 全局变量定义 */
/* HOST CRX子表 */
#if IS_HOST
frw_event_sub_table_item_stru g_ast_dmac_host_crx_table[HMAC_TO_DMAC_SYN_BUTT];
#endif
/* DMAC模块，HOST_DRX事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_ast_dmac_tx_host_drx[DMAC_TX_HOST_DRX_BUTT];

/* DMAC模块，WLAN_DTX事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_ast_dmac_tx_wlan_dtx[DMAC_TX_WLAN_DTX_BUTT];

#if IS_HOST
/* DMAC模块，WLAN_CTX事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_BUTT];
#endif
/* DMAC模块,WLAN_DRX事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_ast_dmac_wlan_drx_event_sub_table[HAL_WLAN_DRX_EVENT_SUB_TYPE_BUTT];

/* DMAC模块，high prio事件处理函数注册结构体定义 */
frw_event_sub_table_item_stru g_ast_dmac_high_prio_event_sub_table[HAL_EVENT_DMAC_HIGH_PRIO_SUB_TYPE_BUTT];

/* DMAC模块,WLAN_CRX事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_ast_dmac_wlan_crx_event_sub_table[HAL_WLAN_CRX_EVENT_SUB_TYPE_BUTT];

/* DMAC模块，TX_COMP事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_ast_dmac_tx_comp_event_sub_table[HAL_TX_COMP_SUB_TYPE_BUTT];

/* WLAN_DTX 事件子类型表 */
frw_event_sub_table_item_stru g_ast_hmac_wlan_dtx_event_sub_table[DMAC_TX_WLAN_DTX_BUTT];

/* HMAC模块 WLAN_DRX事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_ast_hmac_wlan_drx_event_sub_table[DMAC_WLAN_DRX_EVENT_SUB_TYPE_BUTT];

/* HMAC模块 WLAN_CRX事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_BUTT];

/* HMAC模块 发向HOST侧的配置事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_SYN_BUTT];

/* HMAC模块 MISC杂散事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_ast_hmac_wlan_misc_event_sub_table[DMAC_MISC_SUB_TYPE_BUTT];

/* HMAC模块 DDR RX函数注册结构定义 */
frw_event_sub_table_item_stru g_ast_hmac_ddr_drx_sub_table[HAL_WLAN_DDR_DRX_EVENT_SUB_TYPE_BUTT];

/* 3 函数实现 */


void event_fsm_table_register(void)
{
    /* Part1: 以下是Dmac收的事件 */
    /* 注册DMAC模块HOST_CRX事件 */
    frw_event_table_register(FRW_EVENT_TYPE_HOST_CRX, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_dmac_host_crx_table);

    /* 注册DMAC模块HOST_DRX事件处理函数表 */
    frw_event_table_register(FRW_EVENT_TYPE_HOST_DRX, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_dmac_tx_host_drx);

    /* 注册DMAC模块WLAN_DTX事件处理函数表 */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_DTX, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_dmac_tx_wlan_dtx);

    /* 注册DMAC模块WLAN_CTX事件处理函数表 */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_CTX, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_dmac_wlan_ctx_event_sub_table);

    /* 注册DMAC模块WLAN_DRX事件子表 */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_DRX, FRW_EVENT_PIPELINE_STAGE_0,
        g_ast_dmac_wlan_drx_event_sub_table);

    /* 注册DMAC模块WLAN_CRX事件pipeline 0子表 */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_CRX, FRW_EVENT_PIPELINE_STAGE_0,
        g_ast_dmac_wlan_crx_event_sub_table);

    /* 注册DMAC模块TX_COMP事件子表 */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_TX_COMP, FRW_EVENT_PIPELINE_STAGE_0,
        g_ast_dmac_tx_comp_event_sub_table);

    frw_event_table_register(FRW_EVENT_TYPE_HIGH_PRIO, FRW_EVENT_PIPELINE_STAGE_0,
        g_ast_dmac_high_prio_event_sub_table);
    /* Part2: 以下是Hmac收的事件 */
    /* 注册WLAN_DTX事件子表 */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_DTX, FRW_EVENT_PIPELINE_STAGE_0,
        g_ast_hmac_wlan_dtx_event_sub_table);

    /* 注册WLAN_DRX事件子表 */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_DRX, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_hmac_wlan_drx_event_sub_table);

    /* 注册HMAC模块WLAN_CRX事件子表 */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_CRX, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_hmac_wlan_crx_event_sub_table);

    /* 注册DMAC模块MISC事件字表 */
    frw_event_table_register(FRW_EVENT_TYPE_DMAC_MISC, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_hmac_wlan_misc_event_sub_table);

    /* 注册统计结果查询事件子表 */
    frw_event_table_register(FRW_EVENT_TYPE_HOST_SDT_REG, FRW_EVENT_PIPELINE_STAGE_1,
        g_ast_hmac_wlan_ctx_event_sub_table);

    frw_event_table_register(FRW_EVENT_TYPE_HOST_DDR_DRX, FRW_EVENT_PIPELINE_STAGE_0,
        g_ast_hmac_ddr_drx_sub_table);
}


void event_fsm_unregister(void)
{
    /* Part1: 以下是Dmac侧的事件 */
    /* 去注册DMAC模块HOST_CRX事件 */
    memset_s(g_ast_dmac_host_crx_table, OAL_SIZEOF(g_ast_dmac_host_crx_table),
             0, OAL_SIZEOF(g_ast_dmac_host_crx_table));

    /* 去注册DMAC模块HOST_DRX事件处理函数表 */
    memset_s(g_ast_dmac_tx_host_drx, OAL_SIZEOF(g_ast_dmac_tx_host_drx),
             0, OAL_SIZEOF(g_ast_dmac_tx_host_drx));

    /* 去注册DMAC模块WLAN_DTX事件处理函数表 */
    memset_s(g_ast_dmac_tx_wlan_dtx, OAL_SIZEOF(g_ast_dmac_tx_wlan_dtx),
             0, OAL_SIZEOF(g_ast_dmac_tx_wlan_dtx));

    /* 去注册DMAC模块WLAN_DRX事件子表 */
    memset_s(g_ast_dmac_wlan_drx_event_sub_table, OAL_SIZEOF(g_ast_dmac_wlan_drx_event_sub_table),
             0, OAL_SIZEOF(g_ast_dmac_wlan_drx_event_sub_table));

    /* 去注册DMAC模块WLAN_CRX事件子表 */
    memset_s(g_ast_dmac_wlan_crx_event_sub_table, OAL_SIZEOF(g_ast_dmac_wlan_crx_event_sub_table),
             0, OAL_SIZEOF(g_ast_dmac_wlan_crx_event_sub_table));

    /* 去注册DMAC模块TX_COMP事件子表 */
    memset_s(g_ast_dmac_tx_comp_event_sub_table, OAL_SIZEOF(g_ast_dmac_tx_comp_event_sub_table),
             0, OAL_SIZEOF(g_ast_dmac_tx_comp_event_sub_table));

    memset_s(g_ast_dmac_high_prio_event_sub_table, OAL_SIZEOF(g_ast_dmac_high_prio_event_sub_table),
             0, OAL_SIZEOF(g_ast_dmac_high_prio_event_sub_table));
    /* Part2: 以下是Hmac侧的事件 */
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
/* 2 全局变量定义 */
/* DFX公用全局变量定义 */
#ifdef _PRE_WLAN_DFT_STAT
dfx_performance_log_switch_enum_uint8 g_auc_dfx_performance_log_switch[DFX_PERFORMANCE_LOG_BUTT] = {0};
#endif

/* 3 函数实现 */
/* lint +e19 */
/* DFX公用函数实现 */
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

