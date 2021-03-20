

#include "oal_kernel_file.h"
#include "oneimage.h"
#if (_PRE_OS_VERSION == _PRE_OS_VERSION_LINUX)
#include "board.h"
#endif
#include "hal_common_ops.h"
#include "hi1106_common_ops.h"

OAL_STATIC const struct hal_common_ops_stru g_hal_dummy_ops = {
    NULL,
};
const struct hal_common_ops_stru *g_hal_common_ops = &g_hal_dummy_ops;

int hal_main_init(void)
{
#ifdef CONFIG_ARCH_KIRIN_PCIE
    /* 根据不同芯片类型，挂接不同回调函数 */
    if (is_hisi_chiptype(BOARD_VERSION_HI1103)) {
        g_hal_common_ops = &g_hal_dummy_ops;
    } else if (is_hisi_chiptype(BOARD_VERSION_HI1105)) {
        g_hal_common_ops = &g_hal_dummy_ops;
    } else if (is_hisi_chiptype(BOARD_VERSION_HI1106)) {
        g_hal_common_ops = &g_hal_common_ops_1106;
    } else {
        g_hal_common_ops = &g_hal_dummy_ops;
        oam_error_log1(0, OAM_SF_ANY, "hi110x wifi unsupport chitype!%d\n",
            get_hi110x_subchip_type());
        return -OAL_EFAIL;
    }

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    hal_host_board_init();
#endif
#endif
    return OAL_SUCC;
}

void hal_main_exit(void)
{
    /* WTODO:110x暂未实现驱动卸载功能 */
}

