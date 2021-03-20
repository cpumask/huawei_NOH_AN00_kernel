

#ifndef __HMAC_ROAM_CONNECT_H__
#define __HMAC_ROAM_CONNECT_H__

#include "mac_vap.h"
#include "mac_device.h"
#include "mac_common.h"
#include "hmac_vap.h"
#include "hmac_user.h"
#include "oam_wdk.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ROAM_CONNECT_H

#define ROAM_JOIN_TIME_MAX      (1 * 1000) /* JOIN��ʱʱ�� ��λms */
#define ROAM_AUTH_TIME_MAX       300      /* AUTH��ʱʱ�� ��λms */
#define ROAM_ASSOC_TIME_MAX      300      /* ASSOC��ʱʱ�� ��λms */
#define ROAM_HANDSHAKE_TIME_MAX (7 * 1000) /* ���ֳ�ʱʱ�� ��λms */

#define ROAM_MAC_ADDR(_puc_mac)     \
    ((uint32_t)(((uint32_t)(_puc_mac)[2] << 24) |  ((uint32_t)(_puc_mac)[3] << 16) | \
                  ((uint32_t)(_puc_mac)[4] << 8) | ((uint32_t)(_puc_mac)[5])))

/* ����connect״̬��״̬ */
typedef enum {
    ROAM_CONNECT_STATE_INIT = 0,
    ROAM_CONNECT_STATE_FAIL = ROAM_CONNECT_STATE_INIT,
    ROAM_CONNECT_STATE_WAIT_JOIN = 1,
    ROAM_CONNECT_STATE_WAIT_FT_COMP = 2,
    ROAM_CONNECT_STATE_WAIT_AUTH_COMP = 3,
    ROAM_CONNECT_STATE_WAIT_ASSOC_COMP = 4,
    ROAM_CONNECT_STATE_HANDSHAKING = 5,
    ROAM_CONNECT_STATE_UP = 6,
    ROAM_CONNECT_STATE_WAIT_PREAUTH_COMP  = 7,

    ROAM_CONNECT_STATE_BUTT
} roam_connect_state_enum;
typedef uint8_t roam_connect_state_enum_uint8;

/* ����connect״̬���¼����� */
typedef enum {
    ROAM_CONNECT_FSM_EVENT_START = 0,
    ROAM_CONNECT_FSM_EVENT_MGMT_RX = 1,
    ROAM_CONNECT_FSM_EVENT_KEY_DONE = 2,
    ROAM_CONNECT_FSM_EVENT_TIMEOUT = 3,
    ROAM_CONNECT_FSM_EVENT_FT_OVER_DS = 4,
    ROAM_CONNECT_FSM_EVENT_TYPE_BUTT
} roam_connect_fsm_event_type_enum;

uint32_t hmac_roam_connect_set_join_reg(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user);
uint32_t hmac_roam_connect_set_dtim_param(mac_vap_stru *mac_vap, uint8_t dtim_cnt, uint8_t dtim_period);
uint32_t hmac_roam_connect_timeout(void *arg);
uint32_t hmac_roam_connect_start(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr);
uint32_t hmac_roam_connect_stop(hmac_vap_stru *hmac_vap);
void hmac_roam_connect_rx_mgmt(hmac_vap_stru *hmac_vap, dmac_wlan_crx_event_stru *crx_event);
void hmac_roam_connect_fsm_init(void);
void hmac_roam_connect_key_done(hmac_vap_stru *hmac_vap);
#ifdef _PRE_WLAN_FEATURE_11R
uint32_t hmac_roam_connect_ft_reassoc(hmac_vap_stru *hmac_vap);
uint32_t hmac_roam_connect_ft_preauth(hmac_vap_stru *hmac_vap);
#endif  // _PRE_WLAN_FEATURE_11R

#ifdef _PRE_WLAN_FEATURE_SAE
uint32_t hmac_roam_sae_config_reassoc_req(hmac_vap_stru *hmac_vap);
#endif

#endif /* end of hmac_roam_connect.h */
