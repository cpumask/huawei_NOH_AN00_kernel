/*
 * hifi reset notify modem
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#ifndef __RDR_AUDIO_NOTIFY_MODEM_H__
#define __RDR_AUDIO_NOTIFY_MODEM_H__

enum hifi_reset_state {
	HIFI_RESET_STATE_OFF = 0, /* reset begin */
	HIFI_RESET_STATE_READY, /* reset end */
	HIFI_RESET_STATE_INVALID,
};

void hifi_reset_inform_modem(enum hifi_reset_state state);
void hifi_reset_dev_exit(void);
int hifi_reset_dev_init(void);
#endif
