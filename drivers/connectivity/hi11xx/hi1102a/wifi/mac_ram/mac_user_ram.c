

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "mac_user.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_USER_RAM_C

oal_rom_cb_result_enum_uint8 mac_user_init_cb(mac_user_stru *pst_mac_user, oal_uint32 *pul_cb_ret)
{
    *pul_cb_ret = OAL_SUCC;

    /* �鲥�û�portĬ�ϴ�,����aput�����ܳ����������鲥֡ */
    if (pst_mac_user->en_is_multi_user == OAL_TRUE) {
        mac_user_set_port(pst_mac_user, OAL_TRUE);
    }

    return OAL_CONTINUE;
}


