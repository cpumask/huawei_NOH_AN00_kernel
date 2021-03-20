


#include "wlan_chip.h"
#include "oneimage.h"
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "board.h"
#endif
#include "oam_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WLAN_CHIP_C

const struct wlan_chip_ops g_wlan_chip_dummy_ops = { NULL, };

const struct wlan_chip_ops *g_wlan_chip_ops = &g_wlan_chip_dummy_ops;

#ifdef _PRE_LINUX_TEST
const struct wlan_chip_ops g_wlan_chip_ops_1103;
const struct wlan_chip_ops g_wlan_chip_ops_1105;
const struct wlan_chip_ops g_wlan_chip_ops_1106;
#endif

uint32_t wlan_chip_ops_init(void)
{
    /* ���ݲ�ͬоƬ���ͣ��ҽӲ�ͬ�ص����� */
    int32 chip_type = get_hi110x_subchip_type();
    if (chip_type == BOARD_VERSION_HI1103) {
        g_wlan_chip_ops = &g_wlan_chip_ops_1103;
    } else if (chip_type == BOARD_VERSION_HI1105) {
        g_wlan_chip_ops = &g_wlan_chip_ops_1105;
    } else if (chip_type == BOARD_VERSION_HI1106) {
        g_wlan_chip_ops = &g_wlan_chip_ops_1106;
    } else {
        g_wlan_chip_ops = &g_wlan_chip_dummy_ops;
        oam_error_log1(0, OAM_SF_ANY, "wlan_chip_ops_init:hi110x wifi unsupport chitype!%d\n",
            get_hi110x_subchip_type());
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}

