

#include "hal_common_ops.h"
#include "hi1106_common_ops.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HAL_MAIN_C

#ifdef CONFIG_ARCH_KIRIN_PCIE

const struct hal_common_ops_stru g_hal_common_ops_1106 = {
    .host_chip_irq_init          = hi1106_host_chip_irq_init,
    .host_rx_add_buff            = hi1106_host_rx_add_buff,
    .host_get_rx_msdu_status     = hi1106_host_get_rx_msdu_status,
    .host_rx_amsdu_list_build    = hi1106_host_rx_amsdu_list_build,
    .rx_get_next_sub_msdu        = hi1106_rx_get_next_sub_msdu,
    .host_rx_get_msdu_info_dscr  = hi1106_host_rx_get_msdu_info_dscr,
    .rx_host_start_dscr_queue    = hi1106_rx_host_start_dscr_queue,
    .rx_host_init_dscr_queue     = hi1106_rx_host_init_dscr_queue,
    .rx_host_stop_dscr_queue     = hi1106_rx_host_stop_dscr_queue,
    .rx_free_res                 = hi1106_rx_free_res,
    .tx_ba_info_dscr_get         = hi1106_tx_ba_info_dscr_get,
    .host_tx_intr_init           = hi1106_host_tx_intr_init,
    .host_rx_reset_smac_handler  = hi1106_host_rx_reset_smac_handler,
    .rx_alloc_list_free          = hi1106_rx_alloc_list_free,
    .host_al_rx_fcs_info         = hi1106_host_al_rx_fcs_info,
};
#endif
