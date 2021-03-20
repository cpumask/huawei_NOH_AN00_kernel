#ifndef __BSP_POWER_STATE__
#define  __BSP_POWER_STATE__

enum modem_state_node
{
    modem_state_e,
    modem_reset0_e,
    modem_reset1_e,
    modem_reset2_e,
};

enum modem_state_index {
    MODEM_NOT_READY = 0,
    MODEM_READY,
    MODEM_INIT_OK,
    MODEM_INVALID,
};

static const char* const modem_state_str[] = {
    "MODEM_STATE_OFF\n",
    "MODEM_STATE_READY\n",
    "MODEM_STATE_INIT_OK\n",
    "MODEM_STATE_INVALID\n",
};

int bsp_set_modem_state(enum modem_state_node node, unsigned int state);

typedef int (*state_cb)(char* buf, int len);

int bsp_modem_state_cb_register(state_cb);

#endif