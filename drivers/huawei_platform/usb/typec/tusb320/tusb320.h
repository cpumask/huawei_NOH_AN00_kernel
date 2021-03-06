/*
 * tusb320.h
 *
 * header file for TI tusb320 typec chip
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _TUSB320_H_
#define _TUSB320_H_

#include <linux/bitops.h>

#define TUSB320_REGISTER_NUM                    12

#define DISABLE_SET                             0
#define DISABLE_CLEAR                           1
#define TUSB320_RESET_DURATION_MS               25
#define TUSB320_UFP_RESET_DURATION_MS           100

/* Register address */
#define TUSB320_REG_DEVICE_ID                   0x00
#define TUSB320_REG_CURRENT_MODE                0x08
#define TUSB320_REG_ATTACH_STATUS               0x09
#define TUSB320_REG_MODE_SET                    0x0a
#define TUSB320_REG_DISABLE                     0x45

/* Register REG_CURRENT_MODE 08 */
#define TUSB320_REG_HOST_CUR_MID                BIT(6)
#define TUSB320_REG_HOST_CUR_HIGH               BIT(7)
#define TUSB320_REG_HOST_CUR_MODE               (BIT(7) | BIT(6))
#define TUSB320_REG_HOST_CUR_DEFAULT            0x00
#define TUSB320_REG_DEV_CUR_MID                 BIT(4)
#define TUSB320_REG_DEV_CUR_HIGH                (BIT(5) | BIT(4))
#define TUSB320_REG_DEV_CUR_MODE                (BIT(5) | BIT(4))
#define TUSB320_REG_DEV_CUR_DEFAULT             0x00

/* Register REG_ATTACH_STATUS 09 */
#define TUSB320_REG_STATUS_DFP                  BIT(6)
#define TUSB320_REG_STATUS_UFP                  BIT(7)
#define TUSB320_REG_STATUS_MODE                 (BIT(7) | BIT(6))
#define TUSB320_REG_STATUS_CC                   BIT(5)
#define TUSB320_REG_STATUS_INT                  BIT(4)

/* Register REG_MODE_SET 0a */
#define TUSB320_REG_SET_UFP                     BIT(4)
#define TUSB320_REG_SET_DFP                     BIT(5)
#define TUSB320_REG_SET_DRP                     (BIT(5) | BIT(4))
#define TUSB320_REG_SET_DEFAULT                 0x00
#define TUSB320_REG_SET_PORT_MODE               (BIT(5) | BIT(4))
#define TUSB320_REG_SET_SOFT_RESET              BIT(3)
#define TUSB320_REG_SET_DISABLE_RD_RP           BIT(2)
#define TUSB320_REG_SET_DISABLE_TERM            BIT(0)

#endif /* _TUSB320_H_ */
