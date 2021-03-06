/*
 * TypeC_Types.h
 *
 * TypeC_Types driver
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

#ifndef __FSC_TYPEC_TYPES_H__
#define __FSC_TYPEC_TYPES_H__

typedef enum {
    USBTypeC_Sink = 0,
    USBTypeC_Source,
    USBTypeC_DRP,
    USBTypeC_Debug,
    USBTypeC_UNDEFINED = 99
} USBTypeCPort;

typedef enum {
    Disabled = 0,
    ErrorRecovery,
    Unattached,
    AttachWaitSink,
    AttachedSink,
    AttachWaitSource,
    AttachedSource,
    TrySource,
    TryWaitSink,
    TrySink,
    TryWaitSource,
    AudioAccessory,
    DebugAccessorySource,
    AttachWaitAccessory,
    PoweredAccessory,
    UnsupportedAccessory,
    DelayUnattached,
    UnattachedSource,
    DebugAccessorySink,
    AttachWaitDebSink,
    AttachedDebSink,
    AttachWaitDebSource,
    AttachedDebSource,
    TryDebSource,
    TryWaitDebSink,
    UnattachedDebSource,
    IllegalCable,
} ConnectionState;

enum{
	FUSB_DPM_CC_VOLT_OPEN = 0,

	FUSB_DPM_CC_VOLT_SNK_DFT = 5,
	FUSB_DPM_CC_VOLT_SNK_1_5 = 6,
	FUSB_DPM_CC_VOLT_SNK_3_0 = 7,
};

typedef enum {
    CCTypeOpen = 0,
    CCTypeRa,
    CCTypeRdUSB,
    CCTypeRd1p5,
    CCTypeRd3p0,
    CCTypeUndefined
} CCTermType;

typedef enum {
    TypeCPin_None = 0,
    TypeCPin_GND1,
    TypeCPin_TXp1,
    TypeCPin_TXn1,
    TypeCPin_VBUS1,
    TypeCPin_CC1,
    TypeCPin_Dp1,
    TypeCPin_Dn1,
    TypeCPin_SBU1,
    TypeCPin_VBUS2,
    TypeCPin_RXn2,
    TypeCPin_RXp2,
    TypeCPin_GND2,
    TypeCPin_GND3,
    TypeCPin_TXp2,
    TypeCPin_TXn2,
    TypeCPin_VBUS3,
    TypeCPin_CC2,
    TypeCPin_Dp2,
    TypeCPin_Dn2,
    TypeCPin_SBU2,
    TypeCPin_VBUS4,
    TypeCPin_RXn1,
    TypeCPin_RXp1,
    TypeCPin_GND4
} TypeCPins_t;

typedef enum {
    utccNone = 0,
    utccDefault,
    utcc1p5A,
    utcc3p0A
} USBTypeCCurrent;

#endif // __FSC_TYPEC_TYPES_H__
