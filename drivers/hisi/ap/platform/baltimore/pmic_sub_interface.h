#ifndef __SUB_PMIC_HI6423V200_INTERFACE_H__
#define __SUB_PMIC_HI6423V200_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SUB_PMIC_CHIP_VERSION_0_ADDR(base) ((base) + (0x000UL))
#define SUB_PMIC_CHIP_VERSION_1_ADDR(base) ((base) + (0x001UL))
#define SUB_PMIC_CHIP_VERSION_2_ADDR(base) ((base) + (0x002UL))
#define SUB_PMIC_CHIP_VERSION_3_ADDR(base) ((base) + (0x003UL))
#define SUB_PMIC_CHIP_VERSION_4_ADDR(base) ((base) + (0x004UL))
#define SUB_PMIC_CHIP_VERSION_5_ADDR(base) ((base) + (0x005UL))
#define SUB_PMIC_DUMMY_SPMI_ADDR(base) ((base) + (0x006UL))
#define SUB_PMIC_STATUS0_ADDR(base) ((base) + (0x010UL))
#define SUB_PMIC_BUCK_OCP_STATUS0_ADDR(base) ((base) + (0x011UL))
#define SUB_PMIC_BUCK_OCP_STATUS1_ADDR(base) ((base) + (0x012UL))
#define SUB_PMIC_LDO_OCP_STATUS_ADDR(base) ((base) + (0x013UL))
#define SUB_PMIC_BUCK_SCP_STATUS0_ADDR(base) ((base) + (0x014UL))
#define SUB_PMIC_BUCK_SCP_STATUS1_ADDR(base) ((base) + (0x015UL))
#define SUB_PMIC_BUCK_OVP_STATUS0_ADDR(base) ((base) + (0x016UL))
#define SUB_PMIC_BUCK_CUR_STATUS_ADDR(base) ((base) + (0x017UL))
#define SUB_PMIC_SYS_CTRL_STATUS_ADDR(base) ((base) + (0x018UL))
#define SUB_PMIC_LOCK_ADDR(base) ((base) + (0x020UL))
#define SUB_PMIC_BUCK00_ONOFF_ECO_ADDR(base) ((base) + (0x021UL))
#define SUB_PMIC_BUCK30_ONOFF_ECO_ADDR(base) ((base) + (0x022UL))
#define SUB_PMIC_BUCK6_ONOFF_ECO_ADDR(base) ((base) + (0x023UL))
#define SUB_PMIC_BUCK7_ONOFF_ECO_ADDR(base) ((base) + (0x024UL))
#define SUB_PMIC_BUCK8_ONOFF_ECO_ADDR(base) ((base) + (0x025UL))
#define SUB_PMIC_BUCK13_ONOFF_ECO_ADDR(base) ((base) + (0x026UL))
#define SUB_PMIC_LDO9_ONOFF_ECO_ADDR(base) ((base) + (0x027UL))
#define SUB_PMIC_LDO34_ONOFF_ECO_ADDR(base) ((base) + (0x028UL))
#define SUB_PMIC_LDO50_ONOFF_ECO_ADDR(base) ((base) + (0x029UL))
#define SUB_PMIC_LDO51_ONOFF_ECO_ADDR(base) ((base) + (0x02AUL))
#define SUB_PMIC_LDO53_ONOFF_ECO_ADDR(base) ((base) + (0x02BUL))
#define SUB_PMIC_LDO54_ONOFF_ECO_ADDR(base) ((base) + (0x02CUL))
#define SUB_PMIC_PMUH_ONOFF_ECO_ADDR(base) ((base) + (0x02DUL))
#define SUB_PMIC_BUCKBOOST_EN_ADDR(base) ((base) + (0x02EUL))
#define SUB_PMIC_BG_THSD_EN_ADDR(base) ((base) + (0x02FUL))
#define SUB_PMIC_BG_REF_EN_ADDR(base) ((base) + (0x030UL))
#define SUB_PMIC_PALDO_EN_SEL_ADDR(base) ((base) + (0x031UL))
#define SUB_PMIC_BG_REF_CTRL_ADDR(base) ((base) + (0x032UL))
#define SUB_PMIC_BUCK00_VSET_ADDR(base) ((base) + (0x040UL))
#define SUB_PMIC_BUCK00_VSET_ECO_ADDR(base) ((base) + (0x041UL))
#define SUB_PMIC_BUCK30_VSET_ADDR(base) ((base) + (0x042UL))
#define SUB_PMIC_BUCK6_VSET_ADDR(base) ((base) + (0x043UL))
#define SUB_PMIC_BUCK7_VSET_ADDR(base) ((base) + (0x044UL))
#define SUB_PMIC_BUCK8_VSET_ADDR(base) ((base) + (0x045UL))
#define SUB_PMIC_BUCK13_VSET_ADDR(base) ((base) + (0x046UL))
#define SUB_PMIC_BUCK13_VSET_ECO_ADDR(base) ((base) + (0x047UL))
#define SUB_PMIC_LDO9_VSET_ADDR(base) ((base) + (0x048UL))
#define SUB_PMIC_LDO34_VSET_ADDR(base) ((base) + (0x049UL))
#define SUB_PMIC_LDO50_VSET_ADDR(base) ((base) + (0x04AUL))
#define SUB_PMIC_LDO51_VSET_ADDR(base) ((base) + (0x04BUL))
#define SUB_PMIC_LDO53_VSET_ADDR(base) ((base) + (0x04CUL))
#define SUB_PMIC_LDO54_VSET_ADDR(base) ((base) + (0x04DUL))
#define SUB_PMIC_PMUH_VSET_ADDR(base) ((base) + (0x04EUL))
#define SUB_PMIC_AO_MODE_ADDR(base) ((base) + (0x060UL))
#define SUB_PMIC_PMU_IDLE_EN_ADDR(base) ((base) + (0x061UL))
#define SUB_PMIC_FAULT_M_N_MASK_ADDR(base) ((base) + (0x062UL))
#define SUB_PMIC_OSC_9M6_ECO_EN_ADDR(base) ((base) + (0x063UL))
#define SUB_PMIC_OSC_9M6_EN_BYPASS_ADDR(base) ((base) + (0x064UL))
#define SUB_PMIC_PERI_VSET_CTRL0_ADDR(base) ((base) + (0x070UL))
#define SUB_PMIC_SYS_CLK_EN_VSET_CTRL_ADDR(base) ((base) + (0x071UL))
#define SUB_PMIC_HARDWIRE_CTRL0_ADDR(base) ((base) + (0x072UL))
#define SUB_PMIC_PERI_ONOFF_CTRL0_ADDR(base) ((base) + (0x073UL))
#define SUB_PMIC_PERI_ONOFF_CTRL1_ADDR(base) ((base) + (0x074UL))
#define SUB_PMIC_PERI_ECO_CTRL0_ADDR(base) ((base) + (0x075UL))
#define SUB_PMIC_PERI_ECO_CTRL1_ADDR(base) ((base) + (0x076UL))
#define SUB_PMIC_PERI_PLACE_CTRL0_ADDR(base) ((base) + (0x077UL))
#define SUB_PMIC_PERI_PLACE_CTRL1_ADDR(base) ((base) + (0x078UL))
#define SUB_PMIC_PERI_PLACE_CTRL2_ADDR(base) ((base) + (0x079UL))
#define SUB_PMIC_PERI_PLACE_CTRL3_ADDR(base) ((base) + (0x07AUL))
#define SUB_PMIC_PERI_PLACE_CTRL5_ADDR(base) ((base) + (0x07BUL))
#define SUB_PMIC_PERI_PLACE_CTRL6_ADDR(base) ((base) + (0x07CUL))
#define SUB_PMIC_PERI_PLACE_CTRL7_ADDR(base) ((base) + (0x07DUL))
#define SUB_PMIC_PERI_PLACE_CTRL8_ADDR(base) ((base) + (0x07EUL))
#define SUB_PMIC_PERI_PLACE_CTRL9_ADDR(base) ((base) + (0x07FUL))
#define SUB_PMIC_PERI_PLACE_CTRL10_ADDR(base) ((base) + (0x080UL))
#define SUB_PMIC_PERI_PLACE_CTRL11_ADDR(base) ((base) + (0x081UL))
#define SUB_PMIC_PERI_PLACE_CTRL12_ADDR(base) ((base) + (0x082UL))
#define SUB_PMIC_PERI_CNT_CFG_ADDR(base) ((base) + (0x083UL))
#define SUB_PMIC_THSD_ECO_CTRL_ADDR(base) ((base) + (0x90UL))
#define SUB_PMIC_IDLE_ONOFF_CTRL0_ADDR(base) ((base) + (0x91UL))
#define SUB_PMIC_IDLE_ONOFF_CTRL1_ADDR(base) ((base) + (0x92UL))
#define SUB_PMIC_IDLE_ECO_CTRL0_ADDR(base) ((base) + (0x93UL))
#define SUB_PMIC_IDLE_ECO_CTRL1_ADDR(base) ((base) + (0x94UL))
#define SUB_PMIC_IDLE_PLACE_CTRL0_ADDR(base) ((base) + (0x95UL))
#define SUB_PMIC_IDLE_PLACE_CTRL1_ADDR(base) ((base) + (0x96UL))
#define SUB_PMIC_IDLE_PLACE_CTRL2_ADDR(base) ((base) + (0x97UL))
#define SUB_PMIC_IDLE_PLACE_CTRL3_ADDR(base) ((base) + (0x98UL))
#define SUB_PMIC_IDLE_PLACE_CTRL5_ADDR(base) ((base) + (0x99UL))
#define SUB_PMIC_IDLE_PLACE_CTRL6_ADDR(base) ((base) + (0x9AUL))
#define SUB_PMIC_IDLE_PLACE_CTRL7_ADDR(base) ((base) + (0x9BUL))
#define SUB_PMIC_IDLE_PLACE_CTRL8_ADDR(base) ((base) + (0x9CUL))
#define SUB_PMIC_IDLE_PLACE_CTRL9_ADDR(base) ((base) + (0x9DUL))
#define SUB_PMIC_IDLE_PLACE_CTRL10_ADDR(base) ((base) + (0x9EUL))
#define SUB_PMIC_IDLE_PLACE_CTRL11_ADDR(base) ((base) + (0x9FUL))
#define SUB_PMIC_IDLE_PLACE_CTRL12_ADDR(base) ((base) + (0xA0UL))
#define SUB_PMIC_IDLE_CNT_CFG_ADDR(base) ((base) + (0xA1UL))
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_ADDR(base) ((base) + (0x0B0UL))
#define SUB_PMIC_BUCK_OCP_MODE_CTRL1_ADDR(base) ((base) + (0x0B1UL))
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ADDR(base) ((base) + (0x0B2UL))
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_ADDR(base) ((base) + (0x0B3UL))
#define SUB_PMIC_BUCK_SCP_MODE_CTRL1_ADDR(base) ((base) + (0x0B4UL))
#define SUB_PMIC_BUCK_OVP_MODE_CTRL0_ADDR(base) ((base) + (0x0B5UL))
#define SUB_PMIC_BUCK_OCP_ABN_CTRL0_ADDR(base) ((base) + (0x0B6UL))
#define SUB_PMIC_OCP_SCP_ONOFF_ADDR(base) ((base) + (0x0D0UL))
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL0_ADDR(base) ((base) + (0x0D1UL))
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL1_ADDR(base) ((base) + (0x0D2UL))
#define SUB_PMIC_PWROFF_DEB_CTRL_ADDR(base) ((base) + (0x0D3UL))
#define SUB_PMIC_RAMP_BUCK00_CTRL0_ADDR(base) ((base) + (0x100UL))
#define SUB_PMIC_RAMP_BUCK00_CTRL1_ADDR(base) ((base) + (0x101UL))
#define SUB_PMIC_BUCK00_RAMPUP_STATE_ADDR(base) ((base) + (0x102UL))
#define SUB_PMIC_BUCK00_RAMPDOWN_STATE_ADDR(base) ((base) + (0x103UL))
#define SUB_PMIC_BUCK00_RAMP_CHG_RATE_ADDR(base) ((base) + (0x104UL))
#define SUB_PMIC_BUCK00_RAMP_CHG_VSET_ADDR(base) ((base) + (0x105UL))
#define SUB_PMIC_BUCK00_CHG_RATE_SEL_ADDR(base) ((base) + (0x106UL))
#define SUB_PMIC_BUCK00_RAMP_STATE_ADDR(base) ((base) + (0x107UL))
#define SUB_PMIC_RAMP_BUCK30_CTRL0_ADDR(base) ((base) + (0x108UL))
#define SUB_PMIC_RAMP_BUCK30_CTRL1_ADDR(base) ((base) + (0x109UL))
#define SUB_PMIC_BUCK30_RAMPUP_STATE_ADDR(base) ((base) + (0x10AUL))
#define SUB_PMIC_BUCK30_RAMPDOWN_STATE_ADDR(base) ((base) + (0x10BUL))
#define SUB_PMIC_BUCK30_RAMP_CHG_RATE_ADDR(base) ((base) + (0x10CUL))
#define SUB_PMIC_BUCK30_RAMP_CHG_VSET_ADDR(base) ((base) + (0x10DUL))
#define SUB_PMIC_BUCK30_CHG_RATE_SEL_ADDR(base) ((base) + (0x10EUL))
#define SUB_PMIC_BUCK30_RAMP_STATE_ADDR(base) ((base) + (0x10FUL))
#define SUB_PMIC_RAMP_BUCK6_CTRL0_ADDR(base) ((base) + (0x110UL))
#define SUB_PMIC_RAMP_BUCK6_CTRL1_ADDR(base) ((base) + (0x111UL))
#define SUB_PMIC_BUCK6_RAMPUP_STATE_ADDR(base) ((base) + (0x112UL))
#define SUB_PMIC_BUCK6_RAMPDOWN_STATE_ADDR(base) ((base) + (0x113UL))
#define SUB_PMIC_BUCK6_RAMP_CHG_RATE_ADDR(base) ((base) + (0x114UL))
#define SUB_PMIC_BUCK6_RAMP_CHG_VSET_ADDR(base) ((base) + (0x115UL))
#define SUB_PMIC_BUCK6_CHG_RATE_SEL_ADDR(base) ((base) + (0x116UL))
#define SUB_PMIC_BUCK6_RAMP_STATE_ADDR(base) ((base) + (0x117UL))
#define SUB_PMIC_RAMP_BUCK7_CTRL0_ADDR(base) ((base) + (0x118UL))
#define SUB_PMIC_RAMP_BUCK7_CTRL1_ADDR(base) ((base) + (0x119UL))
#define SUB_PMIC_BUCK7_RAMPUP_STATE_ADDR(base) ((base) + (0x11AUL))
#define SUB_PMIC_BUCK7_RAMPDOWN_STATE_ADDR(base) ((base) + (0x11BUL))
#define SUB_PMIC_BUCK7_RAMP_CHG_RATE_ADDR(base) ((base) + (0x11CUL))
#define SUB_PMIC_BUCK7_RAMP_CHG_VSET_ADDR(base) ((base) + (0x11DUL))
#define SUB_PMIC_BUCK7_CHG_RATE_SEL_ADDR(base) ((base) + (0x11EUL))
#define SUB_PMIC_BUCK7_RAMP_STATE_ADDR(base) ((base) + (0x11FUL))
#define SUB_PMIC_LRA_SOFT_RST_ADDR(base) ((base) + (0x143UL))
#define SUB_PMIC_RAMP_CLK_BYPASS_ADDR(base) ((base) + (0x144UL))
#define SUB_PMIC_SIM_CTRL0_ADDR(base) ((base) + (0x160UL))
#define SUB_PMIC_SIM_DEB_CTRL1_ADDR(base) ((base) + (0x161UL))
#define SUB_PMIC_GPIODATA_ADDR(base) ((base) + (0x162UL))
#define SUB_PMIC_GPIO_DATA_RESERVE_ADDR(base) ((base) + (0x163UL))
#define SUB_PMIC_GPIODIR_ADDR(base) ((base) + (0x164UL))
#define SUB_PMIC_GPIO_DIR_RESERVE_ADDR(base) ((base) + (0x165UL))
#define SUB_PMIC_GPIOIS_ADDR(base) ((base) + (0x166UL))
#define SUB_PMIC_GPIOIBE_ADDR(base) ((base) + (0x167UL))
#define SUB_PMIC_GPIOIEV_ADDR(base) ((base) + (0x168UL))
#define SUB_PMIC_GPIOAFSEL_ADDR(base) ((base) + (0x169UL))
#define SUB_PMIC_GPIODSSEL_ADDR(base) ((base) + (0x16AUL))
#define SUB_PMIC_GPIOPUSEL_ADDR(base) ((base) + (0x16BUL))
#define SUB_PMIC_GPIOPDSEL_ADDR(base) ((base) + (0x16CUL))
#define SUB_PMIC_GPIODEBSEL_ADDR(base) ((base) + (0x16DUL))
#define SUB_PMIC_SIMPUSEL_ADDR(base) ((base) + (0x16EUL))
#define SUB_PMIC_SIMPDSEL_ADDR(base) ((base) + (0x16FUL))
#define SUB_PMIC_GPIO0DATA_ADDR(base) ((base) + (0x170UL))
#define SUB_PMIC_GPIO0_DATA_RESERVE_ADDR(base) ((base) + (0x171UL))
#define SUB_PMIC_GPIO0DIR_ADDR(base) ((base) + (0x172UL))
#define SUB_PMIC_GPIO0_DIR_RESERVE_ADDR(base) ((base) + (0x173UL))
#define SUB_PMIC_GPIO0IS_ADDR(base) ((base) + (0x174UL))
#define SUB_PMIC_GPIO0IBE_ADDR(base) ((base) + (0x175UL))
#define SUB_PMIC_GPIO0IEV_ADDR(base) ((base) + (0x176UL))
#define SUB_PMIC_GPIO0DSSEL_ADDR(base) ((base) + (0x177UL))
#define SUB_PMIC_GPIO0PUSEL_ADDR(base) ((base) + (0x178UL))
#define SUB_PMIC_GPIO0PDSEL_ADDR(base) ((base) + (0x179UL))
#define SUB_PMIC_GPIO0DEBSEL_ADDR(base) ((base) + (0x17AUL))
#define SUB_PMIC_OTP_CLK_EN_ADDR(base) ((base) + (0x1A0UL))
#define SUB_PMIC_OTP0_0_ADDR(base) ((base) + (0x1A1UL))
#define SUB_PMIC_OTP0_1_ADDR(base) ((base) + (0x1A2UL))
#define SUB_PMIC_OTP0_CTRL0_ADDR(base) ((base) + (0x1A3UL))
#define SUB_PMIC_OTP0_CTRL1_ADDR(base) ((base) + (0x1A4UL))
#define SUB_PMIC_OTP0_CTRL2_ADDR(base) ((base) + (0x1A5UL))
#define SUB_PMIC_OTP0_WDATA_ADDR(base) ((base) + (0x1A6UL))
#define SUB_PMIC_OTP0_0_W_ADDR(base) ((base) + (0x1A7UL))
#define SUB_PMIC_OTP0_1_W_ADDR(base) ((base) + (0x1A8UL))
#define SUB_PMIC_OTP0_2_W_ADDR(base) ((base) + (0x1A9UL))
#define SUB_PMIC_OTP0_3_W_ADDR(base) ((base) + (0x1AAUL))
#define SUB_PMIC_OTP0_4_W_ADDR(base) ((base) + (0x1ABUL))
#define SUB_PMIC_OTP0_5_W_ADDR(base) ((base) + (0x1ACUL))
#define SUB_PMIC_OTP0_6_W_ADDR(base) ((base) + (0x1ADUL))
#define SUB_PMIC_OTP0_7_W_ADDR(base) ((base) + (0x1AEUL))
#define SUB_PMIC_OTP0_8_W_ADDR(base) ((base) + (0x1AFUL))
#define SUB_PMIC_OTP0_9_W_ADDR(base) ((base) + (0x1B0UL))
#define SUB_PMIC_OTP0_10_W_ADDR(base) ((base) + (0x1B1UL))
#define SUB_PMIC_OTP0_11_W_ADDR(base) ((base) + (0x1B2UL))
#define SUB_PMIC_OTP0_12_W_ADDR(base) ((base) + (0x1B3UL))
#define SUB_PMIC_OTP0_13_W_ADDR(base) ((base) + (0x1B4UL))
#define SUB_PMIC_OTP0_14_W_ADDR(base) ((base) + (0x1B5UL))
#define SUB_PMIC_OTP0_15_W_ADDR(base) ((base) + (0x1B6UL))
#define SUB_PMIC_OTP0_16_W_ADDR(base) ((base) + (0x1B7UL))
#define SUB_PMIC_OTP0_17_W_ADDR(base) ((base) + (0x1B8UL))
#define SUB_PMIC_OTP0_18_W_ADDR(base) ((base) + (0x1B9UL))
#define SUB_PMIC_OTP0_19_W_ADDR(base) ((base) + (0x1BAUL))
#define SUB_PMIC_OTP0_20_W_ADDR(base) ((base) + (0x1BBUL))
#define SUB_PMIC_OTP0_21_W_ADDR(base) ((base) + (0x1BCUL))
#define SUB_PMIC_OTP0_22_W_ADDR(base) ((base) + (0x1BDUL))
#define SUB_PMIC_OTP0_23_W_ADDR(base) ((base) + (0x1BEUL))
#define SUB_PMIC_OTP0_24_W_ADDR(base) ((base) + (0x1BFUL))
#define SUB_PMIC_OTP0_25_W_ADDR(base) ((base) + (0x1C0UL))
#define SUB_PMIC_OTP0_26_W_ADDR(base) ((base) + (0x1C1UL))
#define SUB_PMIC_OTP0_27_W_ADDR(base) ((base) + (0x1C2UL))
#define SUB_PMIC_OTP0_28_W_ADDR(base) ((base) + (0x1C3UL))
#define SUB_PMIC_OTP0_29_W_ADDR(base) ((base) + (0x1C4UL))
#define SUB_PMIC_OTP0_30_W_ADDR(base) ((base) + (0x1C5UL))
#define SUB_PMIC_OTP0_31_W_ADDR(base) ((base) + (0x1C6UL))
#define SUB_PMIC_OTP0_32_W_ADDR(base) ((base) + (0x1C7UL))
#define SUB_PMIC_OTP0_33_W_ADDR(base) ((base) + (0x1C8UL))
#define SUB_PMIC_OTP0_34_W_ADDR(base) ((base) + (0x1C9UL))
#define SUB_PMIC_OTP0_35_W_ADDR(base) ((base) + (0x1CAUL))
#define SUB_PMIC_OTP0_36_W_ADDR(base) ((base) + (0x1CBUL))
#define SUB_PMIC_OTP0_37_W_ADDR(base) ((base) + (0x1CCUL))
#define SUB_PMIC_OTP0_38_W_ADDR(base) ((base) + (0x1CDUL))
#define SUB_PMIC_OTP0_39_W_ADDR(base) ((base) + (0x1CEUL))
#define SUB_PMIC_OTP0_40_W_ADDR(base) ((base) + (0x1CFUL))
#define SUB_PMIC_OTP0_41_W_ADDR(base) ((base) + (0x1D0UL))
#define SUB_PMIC_OTP0_42_W_ADDR(base) ((base) + (0x1D1UL))
#define SUB_PMIC_OTP0_43_W_ADDR(base) ((base) + (0x1D2UL))
#define SUB_PMIC_OTP0_44_W_ADDR(base) ((base) + (0x1D3UL))
#define SUB_PMIC_OTP0_45_W_ADDR(base) ((base) + (0x1D4UL))
#define SUB_PMIC_OTP0_46_W_ADDR(base) ((base) + (0x1D5UL))
#define SUB_PMIC_OTP0_47_W_ADDR(base) ((base) + (0x1D6UL))
#define SUB_PMIC_OTP0_48_W_ADDR(base) ((base) + (0x1D7UL))
#define SUB_PMIC_OTP0_49_W_ADDR(base) ((base) + (0x1D8UL))
#define SUB_PMIC_OTP0_50_W_ADDR(base) ((base) + (0x1D9UL))
#define SUB_PMIC_OTP0_51_W_ADDR(base) ((base) + (0x1DAUL))
#define SUB_PMIC_OTP0_52_W_ADDR(base) ((base) + (0x1DBUL))
#define SUB_PMIC_OTP0_53_W_ADDR(base) ((base) + (0x1DCUL))
#define SUB_PMIC_OTP0_54_W_ADDR(base) ((base) + (0x1DDUL))
#define SUB_PMIC_OTP0_55_W_ADDR(base) ((base) + (0x1DEUL))
#define SUB_PMIC_OTP0_56_W_ADDR(base) ((base) + (0x1DFUL))
#define SUB_PMIC_OTP0_57_W_ADDR(base) ((base) + (0x1E0UL))
#define SUB_PMIC_OTP0_58_W_ADDR(base) ((base) + (0x1E1UL))
#define SUB_PMIC_OTP0_59_W_ADDR(base) ((base) + (0x1E2UL))
#define SUB_PMIC_OTP0_60_W_ADDR(base) ((base) + (0x1E3UL))
#define SUB_PMIC_OTP0_61_W_ADDR(base) ((base) + (0x1E4UL))
#define SUB_PMIC_OTP0_62_W_ADDR(base) ((base) + (0x1E5UL))
#define SUB_PMIC_OTP0_63_W_ADDR(base) ((base) + (0x1E6UL))
#define SUB_PMIC_OTP1_0_ADDR(base) ((base) + (0x1E7UL))
#define SUB_PMIC_OTP1_1_ADDR(base) ((base) + (0x1E8UL))
#define SUB_PMIC_OTP1_CTRL0_ADDR(base) ((base) + (0x1E9UL))
#define SUB_PMIC_OTP1_CTRL1_ADDR(base) ((base) + (0x1EAUL))
#define SUB_PMIC_OTP1_CTRL2_ADDR(base) ((base) + (0x1EBUL))
#define SUB_PMIC_OTP1_WDATA_ADDR(base) ((base) + (0x1ECUL))
#define SUB_PMIC_OTP1_1_W_ADDR(base) ((base) + (0x1EDUL))
#define SUB_PMIC_OTP1_2_W_ADDR(base) ((base) + (0x1EEUL))
#define SUB_PMIC_OTP1_3_W_ADDR(base) ((base) + (0x1EFUL))
#define SUB_PMIC_OTP1_4_W_ADDR(base) ((base) + (0x1F0UL))
#define SUB_PMIC_OTP1_5_W_ADDR(base) ((base) + (0x1F1UL))
#define SUB_PMIC_OTP1_26_W_ADDR(base) ((base) + (0x1F2UL))
#define SUB_PMIC_OTP1_27_W_ADDR(base) ((base) + (0x1F3UL))
#define SUB_PMIC_OTP1_28_W_ADDR(base) ((base) + (0x1F4UL))
#define SUB_PMIC_OTP1_29_W_ADDR(base) ((base) + (0x1F5UL))
#define SUB_PMIC_OTP1_30_W_ADDR(base) ((base) + (0x1F6UL))
#define SUB_PMIC_OTP1_31_W_ADDR(base) ((base) + (0x1F7UL))
#define SUB_PMIC_OTP1_32_W_ADDR(base) ((base) + (0x1F8UL))
#define SUB_PMIC_OTP1_33_W_ADDR(base) ((base) + (0x1F9UL))
#else
#define SUB_PMIC_CHIP_VERSION_0_ADDR(base) ((base) + (0x000))
#define SUB_PMIC_CHIP_VERSION_1_ADDR(base) ((base) + (0x001))
#define SUB_PMIC_CHIP_VERSION_2_ADDR(base) ((base) + (0x002))
#define SUB_PMIC_CHIP_VERSION_3_ADDR(base) ((base) + (0x003))
#define SUB_PMIC_CHIP_VERSION_4_ADDR(base) ((base) + (0x004))
#define SUB_PMIC_CHIP_VERSION_5_ADDR(base) ((base) + (0x005))
#define SUB_PMIC_DUMMY_SPMI_ADDR(base) ((base) + (0x006))
#define SUB_PMIC_STATUS0_ADDR(base) ((base) + (0x010))
#define SUB_PMIC_BUCK_OCP_STATUS0_ADDR(base) ((base) + (0x011))
#define SUB_PMIC_BUCK_OCP_STATUS1_ADDR(base) ((base) + (0x012))
#define SUB_PMIC_LDO_OCP_STATUS_ADDR(base) ((base) + (0x013))
#define SUB_PMIC_BUCK_SCP_STATUS0_ADDR(base) ((base) + (0x014))
#define SUB_PMIC_BUCK_SCP_STATUS1_ADDR(base) ((base) + (0x015))
#define SUB_PMIC_BUCK_OVP_STATUS0_ADDR(base) ((base) + (0x016))
#define SUB_PMIC_BUCK_CUR_STATUS_ADDR(base) ((base) + (0x017))
#define SUB_PMIC_SYS_CTRL_STATUS_ADDR(base) ((base) + (0x018))
#define SUB_PMIC_LOCK_ADDR(base) ((base) + (0x020))
#define SUB_PMIC_BUCK00_ONOFF_ECO_ADDR(base) ((base) + (0x021))
#define SUB_PMIC_BUCK30_ONOFF_ECO_ADDR(base) ((base) + (0x022))
#define SUB_PMIC_BUCK6_ONOFF_ECO_ADDR(base) ((base) + (0x023))
#define SUB_PMIC_BUCK7_ONOFF_ECO_ADDR(base) ((base) + (0x024))
#define SUB_PMIC_BUCK8_ONOFF_ECO_ADDR(base) ((base) + (0x025))
#define SUB_PMIC_BUCK13_ONOFF_ECO_ADDR(base) ((base) + (0x026))
#define SUB_PMIC_LDO9_ONOFF_ECO_ADDR(base) ((base) + (0x027))
#define SUB_PMIC_LDO34_ONOFF_ECO_ADDR(base) ((base) + (0x028))
#define SUB_PMIC_LDO50_ONOFF_ECO_ADDR(base) ((base) + (0x029))
#define SUB_PMIC_LDO51_ONOFF_ECO_ADDR(base) ((base) + (0x02A))
#define SUB_PMIC_LDO53_ONOFF_ECO_ADDR(base) ((base) + (0x02B))
#define SUB_PMIC_LDO54_ONOFF_ECO_ADDR(base) ((base) + (0x02C))
#define SUB_PMIC_PMUH_ONOFF_ECO_ADDR(base) ((base) + (0x02D))
#define SUB_PMIC_BUCKBOOST_EN_ADDR(base) ((base) + (0x02E))
#define SUB_PMIC_BG_THSD_EN_ADDR(base) ((base) + (0x02F))
#define SUB_PMIC_BG_REF_EN_ADDR(base) ((base) + (0x030))
#define SUB_PMIC_PALDO_EN_SEL_ADDR(base) ((base) + (0x031))
#define SUB_PMIC_BG_REF_CTRL_ADDR(base) ((base) + (0x032))
#define SUB_PMIC_BUCK00_VSET_ADDR(base) ((base) + (0x040))
#define SUB_PMIC_BUCK00_VSET_ECO_ADDR(base) ((base) + (0x041))
#define SUB_PMIC_BUCK30_VSET_ADDR(base) ((base) + (0x042))
#define SUB_PMIC_BUCK6_VSET_ADDR(base) ((base) + (0x043))
#define SUB_PMIC_BUCK7_VSET_ADDR(base) ((base) + (0x044))
#define SUB_PMIC_BUCK8_VSET_ADDR(base) ((base) + (0x045))
#define SUB_PMIC_BUCK13_VSET_ADDR(base) ((base) + (0x046))
#define SUB_PMIC_BUCK13_VSET_ECO_ADDR(base) ((base) + (0x047))
#define SUB_PMIC_LDO9_VSET_ADDR(base) ((base) + (0x048))
#define SUB_PMIC_LDO34_VSET_ADDR(base) ((base) + (0x049))
#define SUB_PMIC_LDO50_VSET_ADDR(base) ((base) + (0x04A))
#define SUB_PMIC_LDO51_VSET_ADDR(base) ((base) + (0x04B))
#define SUB_PMIC_LDO53_VSET_ADDR(base) ((base) + (0x04C))
#define SUB_PMIC_LDO54_VSET_ADDR(base) ((base) + (0x04D))
#define SUB_PMIC_PMUH_VSET_ADDR(base) ((base) + (0x04E))
#define SUB_PMIC_AO_MODE_ADDR(base) ((base) + (0x060))
#define SUB_PMIC_PMU_IDLE_EN_ADDR(base) ((base) + (0x061))
#define SUB_PMIC_FAULT_M_N_MASK_ADDR(base) ((base) + (0x062))
#define SUB_PMIC_OSC_9M6_ECO_EN_ADDR(base) ((base) + (0x063))
#define SUB_PMIC_OSC_9M6_EN_BYPASS_ADDR(base) ((base) + (0x064))
#define SUB_PMIC_PERI_VSET_CTRL0_ADDR(base) ((base) + (0x070))
#define SUB_PMIC_SYS_CLK_EN_VSET_CTRL_ADDR(base) ((base) + (0x071))
#define SUB_PMIC_HARDWIRE_CTRL0_ADDR(base) ((base) + (0x072))
#define SUB_PMIC_PERI_ONOFF_CTRL0_ADDR(base) ((base) + (0x073))
#define SUB_PMIC_PERI_ONOFF_CTRL1_ADDR(base) ((base) + (0x074))
#define SUB_PMIC_PERI_ECO_CTRL0_ADDR(base) ((base) + (0x075))
#define SUB_PMIC_PERI_ECO_CTRL1_ADDR(base) ((base) + (0x076))
#define SUB_PMIC_PERI_PLACE_CTRL0_ADDR(base) ((base) + (0x077))
#define SUB_PMIC_PERI_PLACE_CTRL1_ADDR(base) ((base) + (0x078))
#define SUB_PMIC_PERI_PLACE_CTRL2_ADDR(base) ((base) + (0x079))
#define SUB_PMIC_PERI_PLACE_CTRL3_ADDR(base) ((base) + (0x07A))
#define SUB_PMIC_PERI_PLACE_CTRL5_ADDR(base) ((base) + (0x07B))
#define SUB_PMIC_PERI_PLACE_CTRL6_ADDR(base) ((base) + (0x07C))
#define SUB_PMIC_PERI_PLACE_CTRL7_ADDR(base) ((base) + (0x07D))
#define SUB_PMIC_PERI_PLACE_CTRL8_ADDR(base) ((base) + (0x07E))
#define SUB_PMIC_PERI_PLACE_CTRL9_ADDR(base) ((base) + (0x07F))
#define SUB_PMIC_PERI_PLACE_CTRL10_ADDR(base) ((base) + (0x080))
#define SUB_PMIC_PERI_PLACE_CTRL11_ADDR(base) ((base) + (0x081))
#define SUB_PMIC_PERI_PLACE_CTRL12_ADDR(base) ((base) + (0x082))
#define SUB_PMIC_PERI_CNT_CFG_ADDR(base) ((base) + (0x083))
#define SUB_PMIC_THSD_ECO_CTRL_ADDR(base) ((base) + (0x90))
#define SUB_PMIC_IDLE_ONOFF_CTRL0_ADDR(base) ((base) + (0x91))
#define SUB_PMIC_IDLE_ONOFF_CTRL1_ADDR(base) ((base) + (0x92))
#define SUB_PMIC_IDLE_ECO_CTRL0_ADDR(base) ((base) + (0x93))
#define SUB_PMIC_IDLE_ECO_CTRL1_ADDR(base) ((base) + (0x94))
#define SUB_PMIC_IDLE_PLACE_CTRL0_ADDR(base) ((base) + (0x95))
#define SUB_PMIC_IDLE_PLACE_CTRL1_ADDR(base) ((base) + (0x96))
#define SUB_PMIC_IDLE_PLACE_CTRL2_ADDR(base) ((base) + (0x97))
#define SUB_PMIC_IDLE_PLACE_CTRL3_ADDR(base) ((base) + (0x98))
#define SUB_PMIC_IDLE_PLACE_CTRL5_ADDR(base) ((base) + (0x99))
#define SUB_PMIC_IDLE_PLACE_CTRL6_ADDR(base) ((base) + (0x9A))
#define SUB_PMIC_IDLE_PLACE_CTRL7_ADDR(base) ((base) + (0x9B))
#define SUB_PMIC_IDLE_PLACE_CTRL8_ADDR(base) ((base) + (0x9C))
#define SUB_PMIC_IDLE_PLACE_CTRL9_ADDR(base) ((base) + (0x9D))
#define SUB_PMIC_IDLE_PLACE_CTRL10_ADDR(base) ((base) + (0x9E))
#define SUB_PMIC_IDLE_PLACE_CTRL11_ADDR(base) ((base) + (0x9F))
#define SUB_PMIC_IDLE_PLACE_CTRL12_ADDR(base) ((base) + (0xA0))
#define SUB_PMIC_IDLE_CNT_CFG_ADDR(base) ((base) + (0xA1))
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_ADDR(base) ((base) + (0x0B0))
#define SUB_PMIC_BUCK_OCP_MODE_CTRL1_ADDR(base) ((base) + (0x0B1))
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ADDR(base) ((base) + (0x0B2))
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_ADDR(base) ((base) + (0x0B3))
#define SUB_PMIC_BUCK_SCP_MODE_CTRL1_ADDR(base) ((base) + (0x0B4))
#define SUB_PMIC_BUCK_OVP_MODE_CTRL0_ADDR(base) ((base) + (0x0B5))
#define SUB_PMIC_BUCK_OCP_ABN_CTRL0_ADDR(base) ((base) + (0x0B6))
#define SUB_PMIC_OCP_SCP_ONOFF_ADDR(base) ((base) + (0x0D0))
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL0_ADDR(base) ((base) + (0x0D1))
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL1_ADDR(base) ((base) + (0x0D2))
#define SUB_PMIC_PWROFF_DEB_CTRL_ADDR(base) ((base) + (0x0D3))
#define SUB_PMIC_RAMP_BUCK00_CTRL0_ADDR(base) ((base) + (0x100))
#define SUB_PMIC_RAMP_BUCK00_CTRL1_ADDR(base) ((base) + (0x101))
#define SUB_PMIC_BUCK00_RAMPUP_STATE_ADDR(base) ((base) + (0x102))
#define SUB_PMIC_BUCK00_RAMPDOWN_STATE_ADDR(base) ((base) + (0x103))
#define SUB_PMIC_BUCK00_RAMP_CHG_RATE_ADDR(base) ((base) + (0x104))
#define SUB_PMIC_BUCK00_RAMP_CHG_VSET_ADDR(base) ((base) + (0x105))
#define SUB_PMIC_BUCK00_CHG_RATE_SEL_ADDR(base) ((base) + (0x106))
#define SUB_PMIC_BUCK00_RAMP_STATE_ADDR(base) ((base) + (0x107))
#define SUB_PMIC_RAMP_BUCK30_CTRL0_ADDR(base) ((base) + (0x108))
#define SUB_PMIC_RAMP_BUCK30_CTRL1_ADDR(base) ((base) + (0x109))
#define SUB_PMIC_BUCK30_RAMPUP_STATE_ADDR(base) ((base) + (0x10A))
#define SUB_PMIC_BUCK30_RAMPDOWN_STATE_ADDR(base) ((base) + (0x10B))
#define SUB_PMIC_BUCK30_RAMP_CHG_RATE_ADDR(base) ((base) + (0x10C))
#define SUB_PMIC_BUCK30_RAMP_CHG_VSET_ADDR(base) ((base) + (0x10D))
#define SUB_PMIC_BUCK30_CHG_RATE_SEL_ADDR(base) ((base) + (0x10E))
#define SUB_PMIC_BUCK30_RAMP_STATE_ADDR(base) ((base) + (0x10F))
#define SUB_PMIC_RAMP_BUCK6_CTRL0_ADDR(base) ((base) + (0x110))
#define SUB_PMIC_RAMP_BUCK6_CTRL1_ADDR(base) ((base) + (0x111))
#define SUB_PMIC_BUCK6_RAMPUP_STATE_ADDR(base) ((base) + (0x112))
#define SUB_PMIC_BUCK6_RAMPDOWN_STATE_ADDR(base) ((base) + (0x113))
#define SUB_PMIC_BUCK6_RAMP_CHG_RATE_ADDR(base) ((base) + (0x114))
#define SUB_PMIC_BUCK6_RAMP_CHG_VSET_ADDR(base) ((base) + (0x115))
#define SUB_PMIC_BUCK6_CHG_RATE_SEL_ADDR(base) ((base) + (0x116))
#define SUB_PMIC_BUCK6_RAMP_STATE_ADDR(base) ((base) + (0x117))
#define SUB_PMIC_RAMP_BUCK7_CTRL0_ADDR(base) ((base) + (0x118))
#define SUB_PMIC_RAMP_BUCK7_CTRL1_ADDR(base) ((base) + (0x119))
#define SUB_PMIC_BUCK7_RAMPUP_STATE_ADDR(base) ((base) + (0x11A))
#define SUB_PMIC_BUCK7_RAMPDOWN_STATE_ADDR(base) ((base) + (0x11B))
#define SUB_PMIC_BUCK7_RAMP_CHG_RATE_ADDR(base) ((base) + (0x11C))
#define SUB_PMIC_BUCK7_RAMP_CHG_VSET_ADDR(base) ((base) + (0x11D))
#define SUB_PMIC_BUCK7_CHG_RATE_SEL_ADDR(base) ((base) + (0x11E))
#define SUB_PMIC_BUCK7_RAMP_STATE_ADDR(base) ((base) + (0x11F))
#define SUB_PMIC_LRA_SOFT_RST_ADDR(base) ((base) + (0x143))
#define SUB_PMIC_RAMP_CLK_BYPASS_ADDR(base) ((base) + (0x144))
#define SUB_PMIC_SIM_CTRL0_ADDR(base) ((base) + (0x160))
#define SUB_PMIC_SIM_DEB_CTRL1_ADDR(base) ((base) + (0x161))
#define SUB_PMIC_GPIODATA_ADDR(base) ((base) + (0x162))
#define SUB_PMIC_GPIO_DATA_RESERVE_ADDR(base) ((base) + (0x163))
#define SUB_PMIC_GPIODIR_ADDR(base) ((base) + (0x164))
#define SUB_PMIC_GPIO_DIR_RESERVE_ADDR(base) ((base) + (0x165))
#define SUB_PMIC_GPIOIS_ADDR(base) ((base) + (0x166))
#define SUB_PMIC_GPIOIBE_ADDR(base) ((base) + (0x167))
#define SUB_PMIC_GPIOIEV_ADDR(base) ((base) + (0x168))
#define SUB_PMIC_GPIOAFSEL_ADDR(base) ((base) + (0x169))
#define SUB_PMIC_GPIODSSEL_ADDR(base) ((base) + (0x16A))
#define SUB_PMIC_GPIOPUSEL_ADDR(base) ((base) + (0x16B))
#define SUB_PMIC_GPIOPDSEL_ADDR(base) ((base) + (0x16C))
#define SUB_PMIC_GPIODEBSEL_ADDR(base) ((base) + (0x16D))
#define SUB_PMIC_SIMPUSEL_ADDR(base) ((base) + (0x16E))
#define SUB_PMIC_SIMPDSEL_ADDR(base) ((base) + (0x16F))
#define SUB_PMIC_GPIO0DATA_ADDR(base) ((base) + (0x170))
#define SUB_PMIC_GPIO0_DATA_RESERVE_ADDR(base) ((base) + (0x171))
#define SUB_PMIC_GPIO0DIR_ADDR(base) ((base) + (0x172))
#define SUB_PMIC_GPIO0_DIR_RESERVE_ADDR(base) ((base) + (0x173))
#define SUB_PMIC_GPIO0IS_ADDR(base) ((base) + (0x174))
#define SUB_PMIC_GPIO0IBE_ADDR(base) ((base) + (0x175))
#define SUB_PMIC_GPIO0IEV_ADDR(base) ((base) + (0x176))
#define SUB_PMIC_GPIO0DSSEL_ADDR(base) ((base) + (0x177))
#define SUB_PMIC_GPIO0PUSEL_ADDR(base) ((base) + (0x178))
#define SUB_PMIC_GPIO0PDSEL_ADDR(base) ((base) + (0x179))
#define SUB_PMIC_GPIO0DEBSEL_ADDR(base) ((base) + (0x17A))
#define SUB_PMIC_OTP_CLK_EN_ADDR(base) ((base) + (0x1A0))
#define SUB_PMIC_OTP0_0_ADDR(base) ((base) + (0x1A1))
#define SUB_PMIC_OTP0_1_ADDR(base) ((base) + (0x1A2))
#define SUB_PMIC_OTP0_CTRL0_ADDR(base) ((base) + (0x1A3))
#define SUB_PMIC_OTP0_CTRL1_ADDR(base) ((base) + (0x1A4))
#define SUB_PMIC_OTP0_CTRL2_ADDR(base) ((base) + (0x1A5))
#define SUB_PMIC_OTP0_WDATA_ADDR(base) ((base) + (0x1A6))
#define SUB_PMIC_OTP0_0_W_ADDR(base) ((base) + (0x1A7))
#define SUB_PMIC_OTP0_1_W_ADDR(base) ((base) + (0x1A8))
#define SUB_PMIC_OTP0_2_W_ADDR(base) ((base) + (0x1A9))
#define SUB_PMIC_OTP0_3_W_ADDR(base) ((base) + (0x1AA))
#define SUB_PMIC_OTP0_4_W_ADDR(base) ((base) + (0x1AB))
#define SUB_PMIC_OTP0_5_W_ADDR(base) ((base) + (0x1AC))
#define SUB_PMIC_OTP0_6_W_ADDR(base) ((base) + (0x1AD))
#define SUB_PMIC_OTP0_7_W_ADDR(base) ((base) + (0x1AE))
#define SUB_PMIC_OTP0_8_W_ADDR(base) ((base) + (0x1AF))
#define SUB_PMIC_OTP0_9_W_ADDR(base) ((base) + (0x1B0))
#define SUB_PMIC_OTP0_10_W_ADDR(base) ((base) + (0x1B1))
#define SUB_PMIC_OTP0_11_W_ADDR(base) ((base) + (0x1B2))
#define SUB_PMIC_OTP0_12_W_ADDR(base) ((base) + (0x1B3))
#define SUB_PMIC_OTP0_13_W_ADDR(base) ((base) + (0x1B4))
#define SUB_PMIC_OTP0_14_W_ADDR(base) ((base) + (0x1B5))
#define SUB_PMIC_OTP0_15_W_ADDR(base) ((base) + (0x1B6))
#define SUB_PMIC_OTP0_16_W_ADDR(base) ((base) + (0x1B7))
#define SUB_PMIC_OTP0_17_W_ADDR(base) ((base) + (0x1B8))
#define SUB_PMIC_OTP0_18_W_ADDR(base) ((base) + (0x1B9))
#define SUB_PMIC_OTP0_19_W_ADDR(base) ((base) + (0x1BA))
#define SUB_PMIC_OTP0_20_W_ADDR(base) ((base) + (0x1BB))
#define SUB_PMIC_OTP0_21_W_ADDR(base) ((base) + (0x1BC))
#define SUB_PMIC_OTP0_22_W_ADDR(base) ((base) + (0x1BD))
#define SUB_PMIC_OTP0_23_W_ADDR(base) ((base) + (0x1BE))
#define SUB_PMIC_OTP0_24_W_ADDR(base) ((base) + (0x1BF))
#define SUB_PMIC_OTP0_25_W_ADDR(base) ((base) + (0x1C0))
#define SUB_PMIC_OTP0_26_W_ADDR(base) ((base) + (0x1C1))
#define SUB_PMIC_OTP0_27_W_ADDR(base) ((base) + (0x1C2))
#define SUB_PMIC_OTP0_28_W_ADDR(base) ((base) + (0x1C3))
#define SUB_PMIC_OTP0_29_W_ADDR(base) ((base) + (0x1C4))
#define SUB_PMIC_OTP0_30_W_ADDR(base) ((base) + (0x1C5))
#define SUB_PMIC_OTP0_31_W_ADDR(base) ((base) + (0x1C6))
#define SUB_PMIC_OTP0_32_W_ADDR(base) ((base) + (0x1C7))
#define SUB_PMIC_OTP0_33_W_ADDR(base) ((base) + (0x1C8))
#define SUB_PMIC_OTP0_34_W_ADDR(base) ((base) + (0x1C9))
#define SUB_PMIC_OTP0_35_W_ADDR(base) ((base) + (0x1CA))
#define SUB_PMIC_OTP0_36_W_ADDR(base) ((base) + (0x1CB))
#define SUB_PMIC_OTP0_37_W_ADDR(base) ((base) + (0x1CC))
#define SUB_PMIC_OTP0_38_W_ADDR(base) ((base) + (0x1CD))
#define SUB_PMIC_OTP0_39_W_ADDR(base) ((base) + (0x1CE))
#define SUB_PMIC_OTP0_40_W_ADDR(base) ((base) + (0x1CF))
#define SUB_PMIC_OTP0_41_W_ADDR(base) ((base) + (0x1D0))
#define SUB_PMIC_OTP0_42_W_ADDR(base) ((base) + (0x1D1))
#define SUB_PMIC_OTP0_43_W_ADDR(base) ((base) + (0x1D2))
#define SUB_PMIC_OTP0_44_W_ADDR(base) ((base) + (0x1D3))
#define SUB_PMIC_OTP0_45_W_ADDR(base) ((base) + (0x1D4))
#define SUB_PMIC_OTP0_46_W_ADDR(base) ((base) + (0x1D5))
#define SUB_PMIC_OTP0_47_W_ADDR(base) ((base) + (0x1D6))
#define SUB_PMIC_OTP0_48_W_ADDR(base) ((base) + (0x1D7))
#define SUB_PMIC_OTP0_49_W_ADDR(base) ((base) + (0x1D8))
#define SUB_PMIC_OTP0_50_W_ADDR(base) ((base) + (0x1D9))
#define SUB_PMIC_OTP0_51_W_ADDR(base) ((base) + (0x1DA))
#define SUB_PMIC_OTP0_52_W_ADDR(base) ((base) + (0x1DB))
#define SUB_PMIC_OTP0_53_W_ADDR(base) ((base) + (0x1DC))
#define SUB_PMIC_OTP0_54_W_ADDR(base) ((base) + (0x1DD))
#define SUB_PMIC_OTP0_55_W_ADDR(base) ((base) + (0x1DE))
#define SUB_PMIC_OTP0_56_W_ADDR(base) ((base) + (0x1DF))
#define SUB_PMIC_OTP0_57_W_ADDR(base) ((base) + (0x1E0))
#define SUB_PMIC_OTP0_58_W_ADDR(base) ((base) + (0x1E1))
#define SUB_PMIC_OTP0_59_W_ADDR(base) ((base) + (0x1E2))
#define SUB_PMIC_OTP0_60_W_ADDR(base) ((base) + (0x1E3))
#define SUB_PMIC_OTP0_61_W_ADDR(base) ((base) + (0x1E4))
#define SUB_PMIC_OTP0_62_W_ADDR(base) ((base) + (0x1E5))
#define SUB_PMIC_OTP0_63_W_ADDR(base) ((base) + (0x1E6))
#define SUB_PMIC_OTP1_0_ADDR(base) ((base) + (0x1E7))
#define SUB_PMIC_OTP1_1_ADDR(base) ((base) + (0x1E8))
#define SUB_PMIC_OTP1_CTRL0_ADDR(base) ((base) + (0x1E9))
#define SUB_PMIC_OTP1_CTRL1_ADDR(base) ((base) + (0x1EA))
#define SUB_PMIC_OTP1_CTRL2_ADDR(base) ((base) + (0x1EB))
#define SUB_PMIC_OTP1_WDATA_ADDR(base) ((base) + (0x1EC))
#define SUB_PMIC_OTP1_1_W_ADDR(base) ((base) + (0x1ED))
#define SUB_PMIC_OTP1_2_W_ADDR(base) ((base) + (0x1EE))
#define SUB_PMIC_OTP1_3_W_ADDR(base) ((base) + (0x1EF))
#define SUB_PMIC_OTP1_4_W_ADDR(base) ((base) + (0x1F0))
#define SUB_PMIC_OTP1_5_W_ADDR(base) ((base) + (0x1F1))
#define SUB_PMIC_OTP1_26_W_ADDR(base) ((base) + (0x1F2))
#define SUB_PMIC_OTP1_27_W_ADDR(base) ((base) + (0x1F3))
#define SUB_PMIC_OTP1_28_W_ADDR(base) ((base) + (0x1F4))
#define SUB_PMIC_OTP1_29_W_ADDR(base) ((base) + (0x1F5))
#define SUB_PMIC_OTP1_30_W_ADDR(base) ((base) + (0x1F6))
#define SUB_PMIC_OTP1_31_W_ADDR(base) ((base) + (0x1F7))
#define SUB_PMIC_OTP1_32_W_ADDR(base) ((base) + (0x1F8))
#define SUB_PMIC_OTP1_33_W_ADDR(base) ((base) + (0x1F9))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SUB_PMIC_IRQ_MASK0_ADDR(base) ((base) + (0x3A0UL))
#define SUB_PMIC_IRQ_OCP_MASK0_ADDR(base) ((base) + (0x3A1UL))
#define SUB_PMIC_IRQ_OCP_MASK1_ADDR(base) ((base) + (0x3A2UL))
#define SUB_PMIC_IRQ_OCP_MASK2_ADDR(base) ((base) + (0x3A3UL))
#define SUB_PMIC_IRQ_SCP_MASK0_ADDR(base) ((base) + (0x3A4UL))
#define SUB_PMIC_IRQ_SCP_MASK1_ADDR(base) ((base) + (0x3A5UL))
#define SUB_PMIC_IRQ_OVP_MASK_ADDR(base) ((base) + (0x3A6UL))
#define SUB_PMIC_IRQ_CUR_DET_MASK_ADDR(base) ((base) + (0x3A7UL))
#define SUB_PMIC_IRQ_SIM_GPIO_MASK_ADDR(base) ((base) + (0x3A8UL))
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_ADDR(base) ((base) + (0x3A9UL))
#define SUB_PMIC_IRQ_LRA_MASK_ADDR(base) ((base) + (0x3AAUL))
#else
#define SUB_PMIC_IRQ_MASK0_ADDR(base) ((base) + (0x3A0))
#define SUB_PMIC_IRQ_OCP_MASK0_ADDR(base) ((base) + (0x3A1))
#define SUB_PMIC_IRQ_OCP_MASK1_ADDR(base) ((base) + (0x3A2))
#define SUB_PMIC_IRQ_OCP_MASK2_ADDR(base) ((base) + (0x3A3))
#define SUB_PMIC_IRQ_SCP_MASK0_ADDR(base) ((base) + (0x3A4))
#define SUB_PMIC_IRQ_SCP_MASK1_ADDR(base) ((base) + (0x3A5))
#define SUB_PMIC_IRQ_OVP_MASK_ADDR(base) ((base) + (0x3A6))
#define SUB_PMIC_IRQ_CUR_DET_MASK_ADDR(base) ((base) + (0x3A7))
#define SUB_PMIC_IRQ_SIM_GPIO_MASK_ADDR(base) ((base) + (0x3A8))
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_ADDR(base) ((base) + (0x3A9))
#define SUB_PMIC_IRQ_LRA_MASK_ADDR(base) ((base) + (0x3AA))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SUB_PMIC_IRQ0_ADDR(base) ((base) + (0x3C0UL))
#define SUB_PMIC_IRQ_OCP0_ADDR(base) ((base) + (0x3C1UL))
#define SUB_PMIC_IRQ_OCP1_ADDR(base) ((base) + (0x3C2UL))
#define SUB_PMIC_IRQ_OCP2_ADDR(base) ((base) + (0x3C3UL))
#define SUB_PMIC_IRQ_SCP0_ADDR(base) ((base) + (0x3C4UL))
#define SUB_PMIC_IRQ_SCP1_ADDR(base) ((base) + (0x3C5UL))
#define SUB_PMIC_IRQ_OVP_ADDR(base) ((base) + (0x3C6UL))
#define SUB_PMIC_IRQ_CUR_DET_ADDR(base) ((base) + (0x3C7UL))
#define SUB_PMIC_IRQ_SIM_GPIO_ADDR(base) ((base) + (0x3C8UL))
#define SUB_PMIC_IRQ_BUCK_RAMP_ADDR(base) ((base) + (0x3C9UL))
#define SUB_PMIC_IRQ_LRA_ADDR(base) ((base) + (0x3CAUL))
#else
#define SUB_PMIC_IRQ0_ADDR(base) ((base) + (0x3C0))
#define SUB_PMIC_IRQ_OCP0_ADDR(base) ((base) + (0x3C1))
#define SUB_PMIC_IRQ_OCP1_ADDR(base) ((base) + (0x3C2))
#define SUB_PMIC_IRQ_OCP2_ADDR(base) ((base) + (0x3C3))
#define SUB_PMIC_IRQ_SCP0_ADDR(base) ((base) + (0x3C4))
#define SUB_PMIC_IRQ_SCP1_ADDR(base) ((base) + (0x3C5))
#define SUB_PMIC_IRQ_OVP_ADDR(base) ((base) + (0x3C6))
#define SUB_PMIC_IRQ_CUR_DET_ADDR(base) ((base) + (0x3C7))
#define SUB_PMIC_IRQ_SIM_GPIO_ADDR(base) ((base) + (0x3C8))
#define SUB_PMIC_IRQ_BUCK_RAMP_ADDR(base) ((base) + (0x3C9))
#define SUB_PMIC_IRQ_LRA_ADDR(base) ((base) + (0x3CA))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SUB_PMIC_NP_RECORD0_ADDR(base) ((base) + (0x3E0UL))
#define SUB_PMIC_NP_OCP_RECORD0_ADDR(base) ((base) + (0x3E1UL))
#define SUB_PMIC_NP_OCP_RECORD1_ADDR(base) ((base) + (0x3E2UL))
#define SUB_PMIC_NP_OCP_RECORD3_ADDR(base) ((base) + (0x3E3UL))
#define SUB_PMIC_NP_SCP_RECORD0_ADDR(base) ((base) + (0x3E4UL))
#define SUB_PMIC_NP_SCP_RECORD1_ADDR(base) ((base) + (0x3E5UL))
#define SUB_PMIC_NP_OVP_RECORD_ADDR(base) ((base) + (0x3E6UL))
#define SUB_PMIC_NP_CUR_DET_ADDR(base) ((base) + (0x3E7UL))
#define SUB_PMIC_NP_IRQ_RAMP_ABN_ADDR(base) ((base) + (0x3E8UL))
#define SUB_PMIC_NP_RECORD1_ADDR(base) ((base) + (0x3E9UL))
#define SUB_PMIC_NP_RECORD2_ADDR(base) ((base) + (0x3EAUL))
#define SUB_PMIC_NP_RAMP_ECO_ADDR(base) ((base) + (0x3EBUL))
#else
#define SUB_PMIC_NP_RECORD0_ADDR(base) ((base) + (0x3E0))
#define SUB_PMIC_NP_OCP_RECORD0_ADDR(base) ((base) + (0x3E1))
#define SUB_PMIC_NP_OCP_RECORD1_ADDR(base) ((base) + (0x3E2))
#define SUB_PMIC_NP_OCP_RECORD3_ADDR(base) ((base) + (0x3E3))
#define SUB_PMIC_NP_SCP_RECORD0_ADDR(base) ((base) + (0x3E4))
#define SUB_PMIC_NP_SCP_RECORD1_ADDR(base) ((base) + (0x3E5))
#define SUB_PMIC_NP_OVP_RECORD_ADDR(base) ((base) + (0x3E6))
#define SUB_PMIC_NP_CUR_DET_ADDR(base) ((base) + (0x3E7))
#define SUB_PMIC_NP_IRQ_RAMP_ABN_ADDR(base) ((base) + (0x3E8))
#define SUB_PMIC_NP_RECORD1_ADDR(base) ((base) + (0x3E9))
#define SUB_PMIC_NP_RECORD2_ADDR(base) ((base) + (0x3EA))
#define SUB_PMIC_NP_RAMP_ECO_ADDR(base) ((base) + (0x3EB))
#endif
#define PMU_CTRLB_BASE (0x0400)
#ifndef __SOC_H_FOR_ASM__
#define SUB_PMIC_BUCK00_CFG_REG_0_ADDR(base) ((base) + (0x000UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_1_ADDR(base) ((base) + (0x001UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_2_ADDR(base) ((base) + (0x002UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_3_ADDR(base) ((base) + (0x003UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_4_ADDR(base) ((base) + (0x004UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_5_ADDR(base) ((base) + (0x005UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_6_ADDR(base) ((base) + (0x006UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_7_ADDR(base) ((base) + (0x007UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_8_ADDR(base) ((base) + (0x008UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_9_ADDR(base) ((base) + (0x009UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_10_ADDR(base) ((base) + (0x00AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_11_ADDR(base) ((base) + (0x00BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_12_ADDR(base) ((base) + (0x00CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_13_ADDR(base) ((base) + (0x00DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_14_ADDR(base) ((base) + (0x00EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_15_ADDR(base) ((base) + (0x00FUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_16_ADDR(base) ((base) + (0x010UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_17_ADDR(base) ((base) + (0x011UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_18_ADDR(base) ((base) + (0x012UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_19_ADDR(base) ((base) + (0x013UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_20_ADDR(base) ((base) + (0x014UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_21_ADDR(base) ((base) + (0x015UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_22_ADDR(base) ((base) + (0x016UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_23_ADDR(base) ((base) + (0x017UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_24_ADDR(base) ((base) + (0x018UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_25_ADDR(base) ((base) + (0x019UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_26_ADDR(base) ((base) + (0x01AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_27_ADDR(base) ((base) + (0x01BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_28_ADDR(base) ((base) + (0x01CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_29_ADDR(base) ((base) + (0x01DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_30_ADDR(base) ((base) + (0x01EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_31_ADDR(base) ((base) + (0x01FUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_32_ADDR(base) ((base) + (0x020UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_33_ADDR(base) ((base) + (0x021UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_34_ADDR(base) ((base) + (0x022UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_35_ADDR(base) ((base) + (0x023UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_36_ADDR(base) ((base) + (0x024UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_37_ADDR(base) ((base) + (0x025UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_38_ADDR(base) ((base) + (0x026UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_39_ADDR(base) ((base) + (0x027UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_40_ADDR(base) ((base) + (0x028UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_41_ADDR(base) ((base) + (0x029UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_42_ADDR(base) ((base) + (0x02AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_43_ADDR(base) ((base) + (0x02BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_44_ADDR(base) ((base) + (0x02CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_45_ADDR(base) ((base) + (0x02DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_46_ADDR(base) ((base) + (0x02EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_47_ADDR(base) ((base) + (0x02FUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_48_ADDR(base) ((base) + (0x030UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_49_ADDR(base) ((base) + (0x031UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_50_ADDR(base) ((base) + (0x032UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_51_ADDR(base) ((base) + (0x033UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_52_ADDR(base) ((base) + (0x034UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_53_ADDR(base) ((base) + (0x035UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_54_ADDR(base) ((base) + (0x036UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_55_ADDR(base) ((base) + (0x037UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_56_ADDR(base) ((base) + (0x038UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_57_ADDR(base) ((base) + (0x039UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_58_ADDR(base) ((base) + (0x03AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_59_ADDR(base) ((base) + (0x03BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_RO_REG_60_ADDR(base) ((base) + (0x03CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_0_ADDR(base) ((base) + (0x03DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_1_ADDR(base) ((base) + (0x03EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_2_ADDR(base) ((base) + (0x03FUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_3_ADDR(base) ((base) + (0x040UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_4_ADDR(base) ((base) + (0x041UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_5_ADDR(base) ((base) + (0x042UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_6_ADDR(base) ((base) + (0x043UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_7_ADDR(base) ((base) + (0x044UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_8_ADDR(base) ((base) + (0x045UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_9_ADDR(base) ((base) + (0x046UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_10_ADDR(base) ((base) + (0x047UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_0_ADDR(base) ((base) + (0x080UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_1_ADDR(base) ((base) + (0x081UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_2_ADDR(base) ((base) + (0x082UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_3_ADDR(base) ((base) + (0x083UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_4_ADDR(base) ((base) + (0x084UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_5_ADDR(base) ((base) + (0x085UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_6_ADDR(base) ((base) + (0x086UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_7_ADDR(base) ((base) + (0x087UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_8_ADDR(base) ((base) + (0x088UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_9_ADDR(base) ((base) + (0x089UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_10_ADDR(base) ((base) + (0x08AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_11_ADDR(base) ((base) + (0x08BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_12_ADDR(base) ((base) + (0x08CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_13_ADDR(base) ((base) + (0x08DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_14_ADDR(base) ((base) + (0x08EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_15_ADDR(base) ((base) + (0x08FUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_16_ADDR(base) ((base) + (0x090UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_17_ADDR(base) ((base) + (0x091UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_18_ADDR(base) ((base) + (0x092UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_19_ADDR(base) ((base) + (0x093UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_20_ADDR(base) ((base) + (0x094UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_21_ADDR(base) ((base) + (0x095UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_22_ADDR(base) ((base) + (0x096UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_23_ADDR(base) ((base) + (0x097UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_24_ADDR(base) ((base) + (0x098UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_25_ADDR(base) ((base) + (0x099UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_26_ADDR(base) ((base) + (0x09AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_27_ADDR(base) ((base) + (0x09BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_28_ADDR(base) ((base) + (0x09CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_29_ADDR(base) ((base) + (0x09DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_30_ADDR(base) ((base) + (0x09EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_31_ADDR(base) ((base) + (0x09FUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_32_ADDR(base) ((base) + (0x0A0UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_33_ADDR(base) ((base) + (0x0A1UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_34_ADDR(base) ((base) + (0x0A2UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_35_ADDR(base) ((base) + (0x0A3UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_36_ADDR(base) ((base) + (0x0A4UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_37_ADDR(base) ((base) + (0x0A5UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_38_ADDR(base) ((base) + (0x0A6UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_39_ADDR(base) ((base) + (0x0A7UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_40_ADDR(base) ((base) + (0x0A8UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_41_ADDR(base) ((base) + (0x0A9UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_42_ADDR(base) ((base) + (0x0AAUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_43_ADDR(base) ((base) + (0x0ABUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_44_ADDR(base) ((base) + (0x0ACUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_45_ADDR(base) ((base) + (0x0ADUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_46_ADDR(base) ((base) + (0x0AEUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_47_ADDR(base) ((base) + (0x0AFUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_48_ADDR(base) ((base) + (0x0B0UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_49_ADDR(base) ((base) + (0x0B1UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_50_ADDR(base) ((base) + (0x0B2UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_51_ADDR(base) ((base) + (0x0B3UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_52_ADDR(base) ((base) + (0x0B4UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_53_ADDR(base) ((base) + (0x0B5UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_54_ADDR(base) ((base) + (0x0B6UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_55_ADDR(base) ((base) + (0x0B7UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_56_ADDR(base) ((base) + (0x0B8UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_57_ADDR(base) ((base) + (0x0B9UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_58_ADDR(base) ((base) + (0x0BAUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_59_ADDR(base) ((base) + (0x0BBUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_RO_REG_60_ADDR(base) ((base) + (0x0BCUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_0_ADDR(base) ((base) + (0x0BDUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_1_ADDR(base) ((base) + (0x0BEUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_2_ADDR(base) ((base) + (0x0BFUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_3_ADDR(base) ((base) + (0x0C0UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_4_ADDR(base) ((base) + (0x0C1UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_5_ADDR(base) ((base) + (0x0C2UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_6_ADDR(base) ((base) + (0x0C3UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_7_ADDR(base) ((base) + (0x0C4UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_8_ADDR(base) ((base) + (0x0C5UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_9_ADDR(base) ((base) + (0x0C6UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_10_ADDR(base) ((base) + (0x0C7UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_0_ADDR(base) ((base) + (0x140UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_1_ADDR(base) ((base) + (0x141UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_2_ADDR(base) ((base) + (0x142UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_3_ADDR(base) ((base) + (0x143UL))
#define SUB_PMIC_BUCK6_CFG_REG_4_ADDR(base) ((base) + (0x144UL))
#define SUB_PMIC_BUCK6_CFG_REG_5_ADDR(base) ((base) + (0x145UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_6_ADDR(base) ((base) + (0x146UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_7_ADDR(base) ((base) + (0x147UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_8_ADDR(base) ((base) + (0x148UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_9_ADDR(base) ((base) + (0x149UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_10_ADDR(base) ((base) + (0x14AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_11_ADDR(base) ((base) + (0x14BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_12_ADDR(base) ((base) + (0x14CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_13_ADDR(base) ((base) + (0x14DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_14_ADDR(base) ((base) + (0x14EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_RO_REG_15_ADDR(base) ((base) + (0x14FUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_0_ADDR(base) ((base) + (0x180UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_1_ADDR(base) ((base) + (0x181UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_2_ADDR(base) ((base) + (0x182UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_3_ADDR(base) ((base) + (0x183UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_4_ADDR(base) ((base) + (0x184UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_5_ADDR(base) ((base) + (0x185UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_6_ADDR(base) ((base) + (0x186UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_7_ADDR(base) ((base) + (0x187UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_8_ADDR(base) ((base) + (0x188UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_9_ADDR(base) ((base) + (0x189UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_10_ADDR(base) ((base) + (0x18AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_11_ADDR(base) ((base) + (0x18BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_12_ADDR(base) ((base) + (0x18CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_13_ADDR(base) ((base) + (0x18DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_14_ADDR(base) ((base) + (0x18EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_RO_REG_15_ADDR(base) ((base) + (0x18FUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_0_ADDR(base) ((base) + (0x1C0UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_1_ADDR(base) ((base) + (0x1C1UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_2_ADDR(base) ((base) + (0x1C2UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_3_ADDR(base) ((base) + (0x1C3UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_4_ADDR(base) ((base) + (0x1C4UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_5_ADDR(base) ((base) + (0x1C5UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_6_ADDR(base) ((base) + (0x1C6UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_7_ADDR(base) ((base) + (0x1C7UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_8_ADDR(base) ((base) + (0x1C8UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_9_ADDR(base) ((base) + (0x1C9UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_10_ADDR(base) ((base) + (0x1CAUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_11_ADDR(base) ((base) + (0x1CBUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_12_ADDR(base) ((base) + (0x1CCUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_13_ADDR(base) ((base) + (0x1CDUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_RO_REG_14_ADDR(base) ((base) + (0x1CEUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_0_ADDR(base) ((base) + (0x200UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_1_ADDR(base) ((base) + (0x201UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_2_ADDR(base) ((base) + (0x202UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_3_ADDR(base) ((base) + (0x203UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_4_ADDR(base) ((base) + (0x204UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_5_ADDR(base) ((base) + (0x205UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_6_ADDR(base) ((base) + (0x206UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_7_ADDR(base) ((base) + (0x207UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_8_ADDR(base) ((base) + (0x208UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_9_ADDR(base) ((base) + (0x209UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_10_ADDR(base) ((base) + (0x20AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_11_ADDR(base) ((base) + (0x20BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_12_ADDR(base) ((base) + (0x20CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_13_ADDR(base) ((base) + (0x20DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_RO_REG_14_ADDR(base) ((base) + (0x20EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OSC_9M6_CFG_ADDR(base) ((base) + (0x230UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_0_ADDR(base) ((base) + (0x231UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_1_ADDR(base) ((base) + (0x232UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_2_ADDR(base) ((base) + (0x233UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_3_ADDR(base) ((base) + (0x234UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_4_ADDR(base) ((base) + (0x235UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_5_ADDR(base) ((base) + (0x236UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_SIM_CFG_0_ADDR(base) ((base) + (0x270UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_SIM_CFG_1_ADDR(base) ((base) + (0x271UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_SIM_CFG_2_ADDR(base) ((base) + (0x272UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_SIM_CFG_3_ADDR(base) ((base) + (0x273UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_SIM_CFG_4_ADDR(base) ((base) + (0x274UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO9_CFG_0_ADDR(base) ((base) + (0x300UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO34_CFG_0_ADDR(base) ((base) + (0x301UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO50_CFG_0_ADDR(base) ((base) + (0x302UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO51_CFG_0_ADDR(base) ((base) + (0x303UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO53_CFG_0_ADDR(base) ((base) + (0x304UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO54_CFG_0_ADDR(base) ((base) + (0x305UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_PMUH_CFG_0_ADDR(base) ((base) + (0x306UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO_RESERVE_CFG_0_ADDR(base) ((base) + (0x307UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO_RESERVE_CFG_1_ADDR(base) ((base) + (0x308UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_THSD_CFG_0_ADDR(base) ((base) + (0x320UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_SYS_CTRL_RO_REG_0_ADDR(base) ((base) + (0x321UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_D2A_RESERVE_CFG_0_ADDR(base) ((base) + (0x322UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_D2A_RESERVE_CFG_1_ADDR(base) ((base) + (0x323UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_A2D_RESERVE0_ADDR(base) ((base) + (0x324UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_ANA_TEST0_ADDR(base) ((base) + (0x325UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_ANA_TEST1_ADDR(base) ((base) + (0x326UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_TSENSOR_EN_ADDR(base) ((base) + (0x327UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_TSENSOR_CODE_ADDR(base) ((base) + (0x328UL) + PMU_CTRLB_BASE)
#else
#define SUB_PMIC_BUCK00_CFG_REG_0_ADDR(base) ((base) + (0x000) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_1_ADDR(base) ((base) + (0x001) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_2_ADDR(base) ((base) + (0x002) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_3_ADDR(base) ((base) + (0x003) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_4_ADDR(base) ((base) + (0x004) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_5_ADDR(base) ((base) + (0x005) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_6_ADDR(base) ((base) + (0x006) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_7_ADDR(base) ((base) + (0x007) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_8_ADDR(base) ((base) + (0x008) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_9_ADDR(base) ((base) + (0x009) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_10_ADDR(base) ((base) + (0x00A) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_11_ADDR(base) ((base) + (0x00B) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_12_ADDR(base) ((base) + (0x00C) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_13_ADDR(base) ((base) + (0x00D) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_14_ADDR(base) ((base) + (0x00E) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_15_ADDR(base) ((base) + (0x00F) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_16_ADDR(base) ((base) + (0x010) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_17_ADDR(base) ((base) + (0x011) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_18_ADDR(base) ((base) + (0x012) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_19_ADDR(base) ((base) + (0x013) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_20_ADDR(base) ((base) + (0x014) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_21_ADDR(base) ((base) + (0x015) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_22_ADDR(base) ((base) + (0x016) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_23_ADDR(base) ((base) + (0x017) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_24_ADDR(base) ((base) + (0x018) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_25_ADDR(base) ((base) + (0x019) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_26_ADDR(base) ((base) + (0x01A) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_27_ADDR(base) ((base) + (0x01B) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_28_ADDR(base) ((base) + (0x01C) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_29_ADDR(base) ((base) + (0x01D) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_30_ADDR(base) ((base) + (0x01E) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_31_ADDR(base) ((base) + (0x01F) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_32_ADDR(base) ((base) + (0x020) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_33_ADDR(base) ((base) + (0x021) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_34_ADDR(base) ((base) + (0x022) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_35_ADDR(base) ((base) + (0x023) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_36_ADDR(base) ((base) + (0x024) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_37_ADDR(base) ((base) + (0x025) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_38_ADDR(base) ((base) + (0x026) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_39_ADDR(base) ((base) + (0x027) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_40_ADDR(base) ((base) + (0x028) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_41_ADDR(base) ((base) + (0x029) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_42_ADDR(base) ((base) + (0x02A) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_43_ADDR(base) ((base) + (0x02B) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_44_ADDR(base) ((base) + (0x02C) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_45_ADDR(base) ((base) + (0x02D) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_46_ADDR(base) ((base) + (0x02E) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_47_ADDR(base) ((base) + (0x02F) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_48_ADDR(base) ((base) + (0x030) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_49_ADDR(base) ((base) + (0x031) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_50_ADDR(base) ((base) + (0x032) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_51_ADDR(base) ((base) + (0x033) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_52_ADDR(base) ((base) + (0x034) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_53_ADDR(base) ((base) + (0x035) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_54_ADDR(base) ((base) + (0x036) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_55_ADDR(base) ((base) + (0x037) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_56_ADDR(base) ((base) + (0x038) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_57_ADDR(base) ((base) + (0x039) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_58_ADDR(base) ((base) + (0x03A) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_CFG_REG_59_ADDR(base) ((base) + (0x03B) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_RO_REG_60_ADDR(base) ((base) + (0x03C) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_0_ADDR(base) ((base) + (0x03D) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_1_ADDR(base) ((base) + (0x03E) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_2_ADDR(base) ((base) + (0x03F) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_3_ADDR(base) ((base) + (0x040) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_4_ADDR(base) ((base) + (0x041) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_5_ADDR(base) ((base) + (0x042) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_6_ADDR(base) ((base) + (0x043) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_7_ADDR(base) ((base) + (0x044) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_8_ADDR(base) ((base) + (0x045) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_9_ADDR(base) ((base) + (0x046) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK01_CFG_REG_10_ADDR(base) ((base) + (0x047) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_0_ADDR(base) ((base) + (0x080) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_1_ADDR(base) ((base) + (0x081) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_2_ADDR(base) ((base) + (0x082) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_3_ADDR(base) ((base) + (0x083) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_4_ADDR(base) ((base) + (0x084) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_5_ADDR(base) ((base) + (0x085) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_6_ADDR(base) ((base) + (0x086) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_7_ADDR(base) ((base) + (0x087) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_8_ADDR(base) ((base) + (0x088) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_9_ADDR(base) ((base) + (0x089) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_10_ADDR(base) ((base) + (0x08A) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_11_ADDR(base) ((base) + (0x08B) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_12_ADDR(base) ((base) + (0x08C) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_13_ADDR(base) ((base) + (0x08D) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_14_ADDR(base) ((base) + (0x08E) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_15_ADDR(base) ((base) + (0x08F) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_16_ADDR(base) ((base) + (0x090) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_17_ADDR(base) ((base) + (0x091) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_18_ADDR(base) ((base) + (0x092) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_19_ADDR(base) ((base) + (0x093) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_20_ADDR(base) ((base) + (0x094) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_21_ADDR(base) ((base) + (0x095) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_22_ADDR(base) ((base) + (0x096) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_23_ADDR(base) ((base) + (0x097) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_24_ADDR(base) ((base) + (0x098) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_25_ADDR(base) ((base) + (0x099) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_26_ADDR(base) ((base) + (0x09A) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_27_ADDR(base) ((base) + (0x09B) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_28_ADDR(base) ((base) + (0x09C) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_29_ADDR(base) ((base) + (0x09D) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_30_ADDR(base) ((base) + (0x09E) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_31_ADDR(base) ((base) + (0x09F) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_32_ADDR(base) ((base) + (0x0A0) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_33_ADDR(base) ((base) + (0x0A1) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_34_ADDR(base) ((base) + (0x0A2) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_35_ADDR(base) ((base) + (0x0A3) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_36_ADDR(base) ((base) + (0x0A4) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_37_ADDR(base) ((base) + (0x0A5) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_38_ADDR(base) ((base) + (0x0A6) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_39_ADDR(base) ((base) + (0x0A7) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_40_ADDR(base) ((base) + (0x0A8) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_41_ADDR(base) ((base) + (0x0A9) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_42_ADDR(base) ((base) + (0x0AA) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_43_ADDR(base) ((base) + (0x0AB) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_44_ADDR(base) ((base) + (0x0AC) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_45_ADDR(base) ((base) + (0x0AD) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_46_ADDR(base) ((base) + (0x0AE) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_47_ADDR(base) ((base) + (0x0AF) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_48_ADDR(base) ((base) + (0x0B0) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_49_ADDR(base) ((base) + (0x0B1) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_50_ADDR(base) ((base) + (0x0B2) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_51_ADDR(base) ((base) + (0x0B3) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_52_ADDR(base) ((base) + (0x0B4) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_53_ADDR(base) ((base) + (0x0B5) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_54_ADDR(base) ((base) + (0x0B6) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_55_ADDR(base) ((base) + (0x0B7) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_56_ADDR(base) ((base) + (0x0B8) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_57_ADDR(base) ((base) + (0x0B9) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_58_ADDR(base) ((base) + (0x0BA) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_CFG_REG_59_ADDR(base) ((base) + (0x0BB) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_RO_REG_60_ADDR(base) ((base) + (0x0BC) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_0_ADDR(base) ((base) + (0x0BD) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_1_ADDR(base) ((base) + (0x0BE) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_2_ADDR(base) ((base) + (0x0BF) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_3_ADDR(base) ((base) + (0x0C0) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_4_ADDR(base) ((base) + (0x0C1) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_5_ADDR(base) ((base) + (0x0C2) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_6_ADDR(base) ((base) + (0x0C3) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_7_ADDR(base) ((base) + (0x0C4) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_8_ADDR(base) ((base) + (0x0C5) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_9_ADDR(base) ((base) + (0x0C6) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK31_CFG_REG_10_ADDR(base) ((base) + (0x0C7) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_0_ADDR(base) ((base) + (0x140) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_1_ADDR(base) ((base) + (0x141) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_2_ADDR(base) ((base) + (0x142) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_3_ADDR(base) ((base) + (0x143) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_4_ADDR(base) ((base) + (0x144) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_5_ADDR(base) ((base) + (0x145) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_6_ADDR(base) ((base) + (0x146) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_7_ADDR(base) ((base) + (0x147) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_8_ADDR(base) ((base) + (0x148) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_9_ADDR(base) ((base) + (0x149) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_10_ADDR(base) ((base) + (0x14A) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_11_ADDR(base) ((base) + (0x14B) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_12_ADDR(base) ((base) + (0x14C) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_13_ADDR(base) ((base) + (0x14D) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_CFG_REG_14_ADDR(base) ((base) + (0x14E) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_RO_REG_15_ADDR(base) ((base) + (0x14F) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_0_ADDR(base) ((base) + (0x180) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_1_ADDR(base) ((base) + (0x181) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_2_ADDR(base) ((base) + (0x182) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_3_ADDR(base) ((base) + (0x183) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_4_ADDR(base) ((base) + (0x184) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_5_ADDR(base) ((base) + (0x185) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_6_ADDR(base) ((base) + (0x186) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_7_ADDR(base) ((base) + (0x187) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_8_ADDR(base) ((base) + (0x188) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_9_ADDR(base) ((base) + (0x189) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_10_ADDR(base) ((base) + (0x18A) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_11_ADDR(base) ((base) + (0x18B) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_12_ADDR(base) ((base) + (0x18C) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_13_ADDR(base) ((base) + (0x18D) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_CFG_REG_14_ADDR(base) ((base) + (0x18E) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_RO_REG_15_ADDR(base) ((base) + (0x18F) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_0_ADDR(base) ((base) + (0x1C0) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_1_ADDR(base) ((base) + (0x1C1) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_2_ADDR(base) ((base) + (0x1C2) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_3_ADDR(base) ((base) + (0x1C3) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_4_ADDR(base) ((base) + (0x1C4) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_5_ADDR(base) ((base) + (0x1C5) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_6_ADDR(base) ((base) + (0x1C6) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_7_ADDR(base) ((base) + (0x1C7) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_8_ADDR(base) ((base) + (0x1C8) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_9_ADDR(base) ((base) + (0x1C9) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_10_ADDR(base) ((base) + (0x1CA) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_11_ADDR(base) ((base) + (0x1CB) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_12_ADDR(base) ((base) + (0x1CC) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_CFG_REG_13_ADDR(base) ((base) + (0x1CD) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_RO_REG_14_ADDR(base) ((base) + (0x1CE) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_0_ADDR(base) ((base) + (0x200) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_1_ADDR(base) ((base) + (0x201) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_2_ADDR(base) ((base) + (0x202) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_3_ADDR(base) ((base) + (0x203) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_4_ADDR(base) ((base) + (0x204) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_5_ADDR(base) ((base) + (0x205) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_6_ADDR(base) ((base) + (0x206) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_7_ADDR(base) ((base) + (0x207) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_8_ADDR(base) ((base) + (0x208) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_9_ADDR(base) ((base) + (0x209) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_10_ADDR(base) ((base) + (0x20A) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_11_ADDR(base) ((base) + (0x20B) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_12_ADDR(base) ((base) + (0x20C) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_CFG_REG_13_ADDR(base) ((base) + (0x20D) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_RO_REG_14_ADDR(base) ((base) + (0x20E) + PMU_CTRLB_BASE)
#define SUB_PMIC_OSC_9M6_CFG_ADDR(base) ((base) + (0x230) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_0_ADDR(base) ((base) + (0x231) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_1_ADDR(base) ((base) + (0x232) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_2_ADDR(base) ((base) + (0x233) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_3_ADDR(base) ((base) + (0x234) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_4_ADDR(base) ((base) + (0x235) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_5_ADDR(base) ((base) + (0x236) + PMU_CTRLB_BASE)
#define SUB_PMIC_SIM_CFG_0_ADDR(base) ((base) + (0x270) + PMU_CTRLB_BASE)
#define SUB_PMIC_SIM_CFG_1_ADDR(base) ((base) + (0x271) + PMU_CTRLB_BASE)
#define SUB_PMIC_SIM_CFG_2_ADDR(base) ((base) + (0x272) + PMU_CTRLB_BASE)
#define SUB_PMIC_SIM_CFG_3_ADDR(base) ((base) + (0x273) + PMU_CTRLB_BASE)
#define SUB_PMIC_SIM_CFG_4_ADDR(base) ((base) + (0x274) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO9_CFG_0_ADDR(base) ((base) + (0x300) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO34_CFG_0_ADDR(base) ((base) + (0x301) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO50_CFG_0_ADDR(base) ((base) + (0x302) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO51_CFG_0_ADDR(base) ((base) + (0x303) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO53_CFG_0_ADDR(base) ((base) + (0x304) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO54_CFG_0_ADDR(base) ((base) + (0x305) + PMU_CTRLB_BASE)
#define SUB_PMIC_PMUH_CFG_0_ADDR(base) ((base) + (0x306) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO_RESERVE_CFG_0_ADDR(base) ((base) + (0x307) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO_RESERVE_CFG_1_ADDR(base) ((base) + (0x308) + PMU_CTRLB_BASE)
#define SUB_PMIC_THSD_CFG_0_ADDR(base) ((base) + (0x320) + PMU_CTRLB_BASE)
#define SUB_PMIC_SYS_CTRL_RO_REG_0_ADDR(base) ((base) + (0x321) + PMU_CTRLB_BASE)
#define SUB_PMIC_D2A_RESERVE_CFG_0_ADDR(base) ((base) + (0x322) + PMU_CTRLB_BASE)
#define SUB_PMIC_D2A_RESERVE_CFG_1_ADDR(base) ((base) + (0x323) + PMU_CTRLB_BASE)
#define SUB_PMIC_A2D_RESERVE0_ADDR(base) ((base) + (0x324) + PMU_CTRLB_BASE)
#define SUB_PMIC_ANA_TEST0_ADDR(base) ((base) + (0x325) + PMU_CTRLB_BASE)
#define SUB_PMIC_ANA_TEST1_ADDR(base) ((base) + (0x326) + PMU_CTRLB_BASE)
#define SUB_PMIC_TSENSOR_EN_ADDR(base) ((base) + (0x327) + PMU_CTRLB_BASE)
#define SUB_PMIC_TSENSOR_CODE_ADDR(base) ((base) + (0x328) + PMU_CTRLB_BASE)
#endif
#define NP_PMU_CTRL_BASE 0x800
#ifndef __SOC_H_FOR_ASM__
#define SUB_PMIC_ONOFF_PLACE_SEL_ADDR(base) ((base) + (0x000UL) + NP_PMU_CTRL_BASE)
#define SUB_PMIC_BUCK13_ON_PLACE_ADDR(base) ((base) + (0x001UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_ON_PLACE_ADDR(base) ((base) + (0x002UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_ON_PLACE_ADDR(base) ((base) + (0x003UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_ON_PLACE_ADDR(base) ((base) + (0x004UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_ON_PLACE_ADDR(base) ((base) + (0x005UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK12_ON_PLACE_ADDR(base) ((base) + (0x006UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK10_ON_PLACE_ADDR(base) ((base) + (0x007UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK20_ON_PLACE_ADDR(base) ((base) + (0x008UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_ON_PLACE_ADDR(base) ((base) + (0x009UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO9_ON_PLACE_ADDR(base) ((base) + (0x00AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_OFF_PLACE_ADDR(base) ((base) + (0x00BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_OFF_PLACE_ADDR(base) ((base) + (0x00CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_OFF_PLACE_ADDR(base) ((base) + (0x00DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_OFF_PLACE_ADDR(base) ((base) + (0x00EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_OFF_PLACE_ADDR(base) ((base) + (0x00FUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK12_OFF_PLACE_ADDR(base) ((base) + (0x010UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK10_OFF_PLACE_ADDR(base) ((base) + (0x011UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK20_OFF_PLACE_ADDR(base) ((base) + (0x012UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_OFF_PLACE_ADDR(base) ((base) + (0x013UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO9_OFF_PLACE_ADDR(base) ((base) + (0x014UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_DEBUG_LOCK_ADDR(base) ((base) + (0x030UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_SYS_DEBUG0_ADDR(base) ((base) + (0x031UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_SYS_DEBUG1_ADDR(base) ((base) + (0x032UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_SYS_DEBUG2_ADDR(base) ((base) + (0x033UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_SYS_DEBUG3_ADDR(base) ((base) + (0x034UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_VSYS_LOW_SET_ADDR(base) ((base) + (0x035UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_VSYS_PWROFF_MASK_ADDR(base) ((base) + (0x036UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_SYS_RESERVE_ADDR(base) ((base) + (0x037UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_DA_RESERVE0_ADDR(base) ((base) + (0x038UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_DA_RESERVE1_ADDR(base) ((base) + (0x039UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCK10_CFG_0_ADDR(base) ((base) + (0x0A0UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCK20_CFG_0_ADDR(base) ((base) + (0x0A1UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCK30_CFG_0_ADDR(base) ((base) + (0x0A2UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCK12_CFG_0_ADDR(base) ((base) + (0x0A3UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCKBOOST_VSET_ADDR(base) ((base) + (0x100UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCKBOOST_CFG_0_ADDR(base) ((base) + (0x101UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCKBOOST_CFG_1_ADDR(base) ((base) + (0x102UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCKBOOST_CFG_2_ADDR(base) ((base) + (0x103UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCKBOOST_CFG_3_ADDR(base) ((base) + (0x104UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE0_ADDR(base) ((base) + (0x105UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE1_ADDR(base) ((base) + (0x106UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE2_ADDR(base) ((base) + (0x107UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE3_ADDR(base) ((base) + (0x108UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE4_ADDR(base) ((base) + (0x109UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE5_ADDR(base) ((base) + (0x10AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE6_ADDR(base) ((base) + (0x10BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE7_ADDR(base) ((base) + (0x10CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE8_ADDR(base) ((base) + (0x10DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE9_ADDR(base) ((base) + (0x10EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE10_ADDR(base) ((base) + (0x10FUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE11_ADDR(base) ((base) + (0x110UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE12_ADDR(base) ((base) + (0x111UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE13_ADDR(base) ((base) + (0x112UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE14_ADDR(base) ((base) + (0x113UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE15_ADDR(base) ((base) + (0x114UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE16_ADDR(base) ((base) + (0x115UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE17_ADDR(base) ((base) + (0x116UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE18_ADDR(base) ((base) + (0x117UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE19_ADDR(base) ((base) + (0x118UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE20_ADDR(base) ((base) + (0x119UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE21_ADDR(base) ((base) + (0x11AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE22_ADDR(base) ((base) + (0x11BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE23_ADDR(base) ((base) + (0x11CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE24_ADDR(base) ((base) + (0x11DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE25_ADDR(base) ((base) + (0x11EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_REF_CFG0_ADDR(base) ((base) + (0x150UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BG_THSD_CFG1_ADDR(base) ((base) + (0x151UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_BG_THSD_CFG2_ADDR(base) ((base) + (0x152UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_REF_RESERVE0_ADDR(base) ((base) + (0x153UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_REF_RESERVE1_ADDR(base) ((base) + (0x154UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_SPMI_DATA_DS_CFG_ADDR(base) ((base) + (0x360UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_IRQ_DS_CFG_ADDR(base) ((base) + (0x361UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_0_R_ADDR(base) ((base) + (0x362UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_1_R_ADDR(base) ((base) + (0x363UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_2_R_ADDR(base) ((base) + (0x364UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_3_R_ADDR(base) ((base) + (0x365UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_4_R_ADDR(base) ((base) + (0x366UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_5_R_ADDR(base) ((base) + (0x367UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_6_R_ADDR(base) ((base) + (0x368UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_7_R_ADDR(base) ((base) + (0x369UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_8_R_ADDR(base) ((base) + (0x36AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_9_R_ADDR(base) ((base) + (0x36BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_10_R_ADDR(base) ((base) + (0x36CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_11_R_ADDR(base) ((base) + (0x36DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_12_R_ADDR(base) ((base) + (0x36EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_13_R_ADDR(base) ((base) + (0x36FUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_14_R_ADDR(base) ((base) + (0x370UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_15_R_ADDR(base) ((base) + (0x371UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_16_R_ADDR(base) ((base) + (0x372UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_17_R_ADDR(base) ((base) + (0x373UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_18_R_ADDR(base) ((base) + (0x374UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_19_R_ADDR(base) ((base) + (0x375UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_20_R_ADDR(base) ((base) + (0x376UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_21_R_ADDR(base) ((base) + (0x377UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_22_R_ADDR(base) ((base) + (0x378UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_23_R_ADDR(base) ((base) + (0x379UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_24_R_ADDR(base) ((base) + (0x37AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_25_R_ADDR(base) ((base) + (0x37BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_26_R_ADDR(base) ((base) + (0x37CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_27_R_ADDR(base) ((base) + (0x37DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_28_R_ADDR(base) ((base) + (0x37EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_29_R_ADDR(base) ((base) + (0x37FUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_30_R_ADDR(base) ((base) + (0x380UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_31_R_ADDR(base) ((base) + (0x381UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_32_R_ADDR(base) ((base) + (0x382UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_33_R_ADDR(base) ((base) + (0x383UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_34_R_ADDR(base) ((base) + (0x384UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_35_R_ADDR(base) ((base) + (0x385UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_36_R_ADDR(base) ((base) + (0x386UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_37_R_ADDR(base) ((base) + (0x387UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_38_R_ADDR(base) ((base) + (0x388UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_39_R_ADDR(base) ((base) + (0x389UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_40_R_ADDR(base) ((base) + (0x38AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_41_R_ADDR(base) ((base) + (0x38BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_42_R_ADDR(base) ((base) + (0x38CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_43_R_ADDR(base) ((base) + (0x38DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_44_R_ADDR(base) ((base) + (0x38EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_45_R_ADDR(base) ((base) + (0x38FUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_46_R_ADDR(base) ((base) + (0x390UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_47_R_ADDR(base) ((base) + (0x391UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_48_R_ADDR(base) ((base) + (0x392UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_49_R_ADDR(base) ((base) + (0x393UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_50_R_ADDR(base) ((base) + (0x394UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_51_R_ADDR(base) ((base) + (0x395UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_52_R_ADDR(base) ((base) + (0x396UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_53_R_ADDR(base) ((base) + (0x397UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_54_R_ADDR(base) ((base) + (0x398UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_55_R_ADDR(base) ((base) + (0x399UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_56_R_ADDR(base) ((base) + (0x39AUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_57_R_ADDR(base) ((base) + (0x39BUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_58_R_ADDR(base) ((base) + (0x39CUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_59_R_ADDR(base) ((base) + (0x39DUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_60_R_ADDR(base) ((base) + (0x39EUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_61_R_ADDR(base) ((base) + (0x39FUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_62_R_ADDR(base) ((base) + (0x3A0UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_63_R_ADDR(base) ((base) + (0x3A1UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_0_R_ADDR(base) ((base) + (0x3A2UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_1_R_ADDR(base) ((base) + (0x3A3UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_2_R_ADDR(base) ((base) + (0x3A4UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_3_R_ADDR(base) ((base) + (0x3A5UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_4_R_ADDR(base) ((base) + (0x3A6UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_5_R_ADDR(base) ((base) + (0x3A7UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_6_R_ADDR(base) ((base) + (0x3A8UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_7_R_ADDR(base) ((base) + (0x3A9UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_8_R_ADDR(base) ((base) + (0x3AAUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_9_R_ADDR(base) ((base) + (0x3ABUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_10_R_ADDR(base) ((base) + (0x3ACUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_11_R_ADDR(base) ((base) + (0x3ADUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_12_R_ADDR(base) ((base) + (0x3AEUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_13_R_ADDR(base) ((base) + (0x3AFUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_14_R_ADDR(base) ((base) + (0x3B0UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_15_R_ADDR(base) ((base) + (0x3B1UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_16_R_ADDR(base) ((base) + (0x3B2UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_17_R_ADDR(base) ((base) + (0x3B3UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_18_R_ADDR(base) ((base) + (0x3B4UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_19_R_ADDR(base) ((base) + (0x3B5UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_20_R_ADDR(base) ((base) + (0x3B6UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_21_R_ADDR(base) ((base) + (0x3B7UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_22_R_ADDR(base) ((base) + (0x3B8UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_23_R_ADDR(base) ((base) + (0x3B9UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_24_R_ADDR(base) ((base) + (0x3BAUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_25_R_ADDR(base) ((base) + (0x3BBUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_26_R_ADDR(base) ((base) + (0x3BCUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_27_R_ADDR(base) ((base) + (0x3BDUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_28_R_ADDR(base) ((base) + (0x3BEUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_29_R_ADDR(base) ((base) + (0x3BFUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_30_R_ADDR(base) ((base) + (0x3C0UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_31_R_ADDR(base) ((base) + (0x3C1UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_32_R_ADDR(base) ((base) + (0x3C2UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_33_R_ADDR(base) ((base) + (0x3C3UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_34_R_ADDR(base) ((base) + (0x3C4UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_35_R_ADDR(base) ((base) + (0x3C5UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_36_R_ADDR(base) ((base) + (0x3C6UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_37_R_ADDR(base) ((base) + (0x3C7UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_38_R_ADDR(base) ((base) + (0x3C8UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_39_R_ADDR(base) ((base) + (0x3C9UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_40_R_ADDR(base) ((base) + (0x3CAUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_41_R_ADDR(base) ((base) + (0x3CBUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_42_R_ADDR(base) ((base) + (0x3CCUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_43_R_ADDR(base) ((base) + (0x3CDUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_44_R_ADDR(base) ((base) + (0x3CEUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_45_R_ADDR(base) ((base) + (0x3CFUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_46_R_ADDR(base) ((base) + (0x3D0UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_47_R_ADDR(base) ((base) + (0x3D1UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_48_R_ADDR(base) ((base) + (0x3D2UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_49_R_ADDR(base) ((base) + (0x3D3UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_50_R_ADDR(base) ((base) + (0x3D4UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_51_R_ADDR(base) ((base) + (0x3D5UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_52_R_ADDR(base) ((base) + (0x3D6UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_53_R_ADDR(base) ((base) + (0x3D7UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_54_R_ADDR(base) ((base) + (0x3D8UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_55_R_ADDR(base) ((base) + (0x3D9UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_56_R_ADDR(base) ((base) + (0x3DAUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_57_R_ADDR(base) ((base) + (0x3DBUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_58_R_ADDR(base) ((base) + (0x3DCUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_59_R_ADDR(base) ((base) + (0x3DDUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_60_R_ADDR(base) ((base) + (0x3DEUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_61_R_ADDR(base) ((base) + (0x3DFUL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_62_R_ADDR(base) ((base) + (0x3E0UL) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_63_R_ADDR(base) ((base) + (0x3E1UL) + PMU_CTRLB_BASE)
#else
#define SUB_PMIC_ONOFF_PLACE_SEL_ADDR(base) ((base) + (0x000) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_ON_PLACE_ADDR(base) ((base) + (0x001) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_ON_PLACE_ADDR(base) ((base) + (0x002) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_ON_PLACE_ADDR(base) ((base) + (0x003) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_ON_PLACE_ADDR(base) ((base) + (0x004) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_ON_PLACE_ADDR(base) ((base) + (0x005) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK12_ON_PLACE_ADDR(base) ((base) + (0x006) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK10_ON_PLACE_ADDR(base) ((base) + (0x007) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK20_ON_PLACE_ADDR(base) ((base) + (0x008) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_ON_PLACE_ADDR(base) ((base) + (0x009) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO9_ON_PLACE_ADDR(base) ((base) + (0x00A) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK13_OFF_PLACE_ADDR(base) ((base) + (0x00B) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK00_OFF_PLACE_ADDR(base) ((base) + (0x00C) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK6_OFF_PLACE_ADDR(base) ((base) + (0x00D) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK8_OFF_PLACE_ADDR(base) ((base) + (0x00E) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK7_OFF_PLACE_ADDR(base) ((base) + (0x00F) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK12_OFF_PLACE_ADDR(base) ((base) + (0x010) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK10_OFF_PLACE_ADDR(base) ((base) + (0x011) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK20_OFF_PLACE_ADDR(base) ((base) + (0x012) + PMU_CTRLB_BASE)
#define SUB_PMIC_BUCK30_OFF_PLACE_ADDR(base) ((base) + (0x013) + PMU_CTRLB_BASE)
#define SUB_PMIC_LDO9_OFF_PLACE_ADDR(base) ((base) + (0x014) + PMU_CTRLB_BASE)
#define SUB_PMIC_DEBUG_LOCK_ADDR(base) ((base) + (0x030) + PMU_CTRLB_BASE)
#define SUB_PMIC_SYS_DEBUG0_ADDR(base) ((base) + (0x031) + PMU_CTRLB_BASE)
#define SUB_PMIC_SYS_DEBUG1_ADDR(base) ((base) + (0x032) + PMU_CTRLB_BASE)
#define SUB_PMIC_SYS_DEBUG2_ADDR(base) ((base) + (0x033) + PMU_CTRLB_BASE)
#define SUB_PMIC_SYS_DEBUG3_ADDR(base) ((base) + (0x034) + PMU_CTRLB_BASE)
#define SUB_PMIC_VSYS_LOW_SET_ADDR(base) ((base) + (0x035) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_VSYS_PWROFF_MASK_ADDR(base) ((base) + (0x036) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_SYS_RESERVE_ADDR(base) ((base) + (0x037) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_DA_RESERVE0_ADDR(base) ((base) + (0x038) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_DA_RESERVE1_ADDR(base) ((base) + (0x039) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCK10_CFG_0_ADDR(base) ((base) + (0x0A0) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCK20_CFG_0_ADDR(base) ((base) + (0x0A1) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCK30_CFG_0_ADDR(base) ((base) + (0x0A2) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCK12_CFG_0_ADDR(base) ((base) + (0x0A3) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCKBOOST_VSET_ADDR(base) ((base) + (0x100) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCKBOOST_CFG_0_ADDR(base) ((base) + (0x101) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCKBOOST_CFG_1_ADDR(base) ((base) + (0x102) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCKBOOST_CFG_2_ADDR(base) ((base) + (0x103) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BUCKBOOST_CFG_3_ADDR(base) ((base) + (0x104) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE0_ADDR(base) ((base) + (0x105) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE1_ADDR(base) ((base) + (0x106) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE2_ADDR(base) ((base) + (0x107) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE3_ADDR(base) ((base) + (0x108) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE4_ADDR(base) ((base) + (0x109) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE5_ADDR(base) ((base) + (0x10A) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE6_ADDR(base) ((base) + (0x10B) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE7_ADDR(base) ((base) + (0x10C) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE8_ADDR(base) ((base) + (0x10D) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE9_ADDR(base) ((base) + (0x10E) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE10_ADDR(base) ((base) + (0x10F) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE11_ADDR(base) ((base) + (0x110) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE12_ADDR(base) ((base) + (0x111) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE13_ADDR(base) ((base) + (0x112) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE14_ADDR(base) ((base) + (0x113) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE15_ADDR(base) ((base) + (0x114) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE16_ADDR(base) ((base) + (0x115) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE17_ADDR(base) ((base) + (0x116) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE18_ADDR(base) ((base) + (0x117) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE19_ADDR(base) ((base) + (0x118) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE20_ADDR(base) ((base) + (0x119) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE21_ADDR(base) ((base) + (0x11A) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE22_ADDR(base) ((base) + (0x11B) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE23_ADDR(base) ((base) + (0x11C) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE24_ADDR(base) ((base) + (0x11D) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_BB_RESERVE25_ADDR(base) ((base) + (0x11E) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_REF_CFG0_ADDR(base) ((base) + (0x150) + PMU_CTRLB_BASE)
#define SUB_PMIC_BG_THSD_CFG1_ADDR(base) ((base) + (0x151) + PMU_CTRLB_BASE)
#define SUB_PMIC_BG_THSD_CFG2_ADDR(base) ((base) + (0x152) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_REF_RESERVE0_ADDR(base) ((base) + (0x153) + PMU_CTRLB_BASE)
#define SUB_PMIC_NP_REF_RESERVE1_ADDR(base) ((base) + (0x154) + PMU_CTRLB_BASE)
#define SUB_PMIC_SPMI_DATA_DS_CFG_ADDR(base) ((base) + (0x360) + PMU_CTRLB_BASE)
#define SUB_PMIC_IRQ_DS_CFG_ADDR(base) ((base) + (0x361) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_0_R_ADDR(base) ((base) + (0x362) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_1_R_ADDR(base) ((base) + (0x363) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_2_R_ADDR(base) ((base) + (0x364) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_3_R_ADDR(base) ((base) + (0x365) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_4_R_ADDR(base) ((base) + (0x366) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_5_R_ADDR(base) ((base) + (0x367) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_6_R_ADDR(base) ((base) + (0x368) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_7_R_ADDR(base) ((base) + (0x369) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_8_R_ADDR(base) ((base) + (0x36A) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_9_R_ADDR(base) ((base) + (0x36B) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_10_R_ADDR(base) ((base) + (0x36C) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_11_R_ADDR(base) ((base) + (0x36D) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_12_R_ADDR(base) ((base) + (0x36E) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_13_R_ADDR(base) ((base) + (0x36F) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_14_R_ADDR(base) ((base) + (0x370) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_15_R_ADDR(base) ((base) + (0x371) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_16_R_ADDR(base) ((base) + (0x372) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_17_R_ADDR(base) ((base) + (0x373) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_18_R_ADDR(base) ((base) + (0x374) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_19_R_ADDR(base) ((base) + (0x375) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_20_R_ADDR(base) ((base) + (0x376) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_21_R_ADDR(base) ((base) + (0x377) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_22_R_ADDR(base) ((base) + (0x378) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_23_R_ADDR(base) ((base) + (0x379) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_24_R_ADDR(base) ((base) + (0x37A) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_25_R_ADDR(base) ((base) + (0x37B) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_26_R_ADDR(base) ((base) + (0x37C) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_27_R_ADDR(base) ((base) + (0x37D) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_28_R_ADDR(base) ((base) + (0x37E) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_29_R_ADDR(base) ((base) + (0x37F) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_30_R_ADDR(base) ((base) + (0x380) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_31_R_ADDR(base) ((base) + (0x381) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_32_R_ADDR(base) ((base) + (0x382) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_33_R_ADDR(base) ((base) + (0x383) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_34_R_ADDR(base) ((base) + (0x384) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_35_R_ADDR(base) ((base) + (0x385) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_36_R_ADDR(base) ((base) + (0x386) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_37_R_ADDR(base) ((base) + (0x387) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_38_R_ADDR(base) ((base) + (0x388) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_39_R_ADDR(base) ((base) + (0x389) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_40_R_ADDR(base) ((base) + (0x38A) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_41_R_ADDR(base) ((base) + (0x38B) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_42_R_ADDR(base) ((base) + (0x38C) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_43_R_ADDR(base) ((base) + (0x38D) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_44_R_ADDR(base) ((base) + (0x38E) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_45_R_ADDR(base) ((base) + (0x38F) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_46_R_ADDR(base) ((base) + (0x390) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_47_R_ADDR(base) ((base) + (0x391) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_48_R_ADDR(base) ((base) + (0x392) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_49_R_ADDR(base) ((base) + (0x393) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_50_R_ADDR(base) ((base) + (0x394) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_51_R_ADDR(base) ((base) + (0x395) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_52_R_ADDR(base) ((base) + (0x396) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_53_R_ADDR(base) ((base) + (0x397) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_54_R_ADDR(base) ((base) + (0x398) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_55_R_ADDR(base) ((base) + (0x399) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_56_R_ADDR(base) ((base) + (0x39A) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_57_R_ADDR(base) ((base) + (0x39B) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_58_R_ADDR(base) ((base) + (0x39C) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_59_R_ADDR(base) ((base) + (0x39D) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_60_R_ADDR(base) ((base) + (0x39E) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_61_R_ADDR(base) ((base) + (0x39F) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_62_R_ADDR(base) ((base) + (0x3A0) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP0_63_R_ADDR(base) ((base) + (0x3A1) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_0_R_ADDR(base) ((base) + (0x3A2) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_1_R_ADDR(base) ((base) + (0x3A3) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_2_R_ADDR(base) ((base) + (0x3A4) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_3_R_ADDR(base) ((base) + (0x3A5) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_4_R_ADDR(base) ((base) + (0x3A6) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_5_R_ADDR(base) ((base) + (0x3A7) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_6_R_ADDR(base) ((base) + (0x3A8) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_7_R_ADDR(base) ((base) + (0x3A9) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_8_R_ADDR(base) ((base) + (0x3AA) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_9_R_ADDR(base) ((base) + (0x3AB) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_10_R_ADDR(base) ((base) + (0x3AC) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_11_R_ADDR(base) ((base) + (0x3AD) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_12_R_ADDR(base) ((base) + (0x3AE) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_13_R_ADDR(base) ((base) + (0x3AF) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_14_R_ADDR(base) ((base) + (0x3B0) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_15_R_ADDR(base) ((base) + (0x3B1) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_16_R_ADDR(base) ((base) + (0x3B2) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_17_R_ADDR(base) ((base) + (0x3B3) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_18_R_ADDR(base) ((base) + (0x3B4) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_19_R_ADDR(base) ((base) + (0x3B5) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_20_R_ADDR(base) ((base) + (0x3B6) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_21_R_ADDR(base) ((base) + (0x3B7) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_22_R_ADDR(base) ((base) + (0x3B8) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_23_R_ADDR(base) ((base) + (0x3B9) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_24_R_ADDR(base) ((base) + (0x3BA) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_25_R_ADDR(base) ((base) + (0x3BB) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_26_R_ADDR(base) ((base) + (0x3BC) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_27_R_ADDR(base) ((base) + (0x3BD) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_28_R_ADDR(base) ((base) + (0x3BE) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_29_R_ADDR(base) ((base) + (0x3BF) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_30_R_ADDR(base) ((base) + (0x3C0) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_31_R_ADDR(base) ((base) + (0x3C1) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_32_R_ADDR(base) ((base) + (0x3C2) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_33_R_ADDR(base) ((base) + (0x3C3) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_34_R_ADDR(base) ((base) + (0x3C4) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_35_R_ADDR(base) ((base) + (0x3C5) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_36_R_ADDR(base) ((base) + (0x3C6) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_37_R_ADDR(base) ((base) + (0x3C7) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_38_R_ADDR(base) ((base) + (0x3C8) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_39_R_ADDR(base) ((base) + (0x3C9) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_40_R_ADDR(base) ((base) + (0x3CA) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_41_R_ADDR(base) ((base) + (0x3CB) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_42_R_ADDR(base) ((base) + (0x3CC) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_43_R_ADDR(base) ((base) + (0x3CD) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_44_R_ADDR(base) ((base) + (0x3CE) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_45_R_ADDR(base) ((base) + (0x3CF) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_46_R_ADDR(base) ((base) + (0x3D0) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_47_R_ADDR(base) ((base) + (0x3D1) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_48_R_ADDR(base) ((base) + (0x3D2) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_49_R_ADDR(base) ((base) + (0x3D3) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_50_R_ADDR(base) ((base) + (0x3D4) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_51_R_ADDR(base) ((base) + (0x3D5) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_52_R_ADDR(base) ((base) + (0x3D6) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_53_R_ADDR(base) ((base) + (0x3D7) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_54_R_ADDR(base) ((base) + (0x3D8) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_55_R_ADDR(base) ((base) + (0x3D9) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_56_R_ADDR(base) ((base) + (0x3DA) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_57_R_ADDR(base) ((base) + (0x3DB) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_58_R_ADDR(base) ((base) + (0x3DC) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_59_R_ADDR(base) ((base) + (0x3DD) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_60_R_ADDR(base) ((base) + (0x3DE) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_61_R_ADDR(base) ((base) + (0x3DF) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_62_R_ADDR(base) ((base) + (0x3E0) + PMU_CTRLB_BASE)
#define SUB_PMIC_OTP1_63_R_ADDR(base) ((base) + (0x3E1) + PMU_CTRLB_BASE)
#endif
 #define PMU_SER_BASE 0xC00
#ifndef __SOC_H_FOR_ASM__
#define SUB_PMIC_BUCK10_ONOFF_ECO_ADDR(base) ((base) + (0x000UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_VSET_ADDR(base) ((base) + (0x001UL) + PMU_SER_BASE)
#define SUB_PMIC_RAMP_BUCK10_CTRL0_ADDR(base) ((base) + (0x002UL) + PMU_SER_BASE)
#define SUB_PMIC_RAMP_BUCK10_CTRL1_ADDR(base) ((base) + (0x003UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_RAMPUP_STATE_ADDR(base) ((base) + (0x004UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_RAMPDOWN_STATE_ADDR(base) ((base) + (0x005UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_RAMP_CHG_RATE_ADDR(base) ((base) + (0x006UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_RAMP_CHG_VSET_ADDR(base) ((base) + (0x007UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CHG_RATE_SEL_ADDR(base) ((base) + (0x008UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_RAMP_STATE_ADDR(base) ((base) + (0x009UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_0_ADDR(base) ((base) + (0x030UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_1_ADDR(base) ((base) + (0x031UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_2_ADDR(base) ((base) + (0x032UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_3_ADDR(base) ((base) + (0x033UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_4_ADDR(base) ((base) + (0x034UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_5_ADDR(base) ((base) + (0x035UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_6_ADDR(base) ((base) + (0x036UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_7_ADDR(base) ((base) + (0x037UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_8_ADDR(base) ((base) + (0x038UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_9_ADDR(base) ((base) + (0x039UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_10_ADDR(base) ((base) + (0x03AUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_11_ADDR(base) ((base) + (0x03BUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_12_ADDR(base) ((base) + (0x03CUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_13_ADDR(base) ((base) + (0x03DUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_14_ADDR(base) ((base) + (0x03EUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_15_ADDR(base) ((base) + (0x03FUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_16_ADDR(base) ((base) + (0x040UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_17_ADDR(base) ((base) + (0x041UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_18_ADDR(base) ((base) + (0x042UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_19_ADDR(base) ((base) + (0x043UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_20_ADDR(base) ((base) + (0x044UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_21_ADDR(base) ((base) + (0x045UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_22_ADDR(base) ((base) + (0x046UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_23_ADDR(base) ((base) + (0x047UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_24_ADDR(base) ((base) + (0x048UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_25_ADDR(base) ((base) + (0x049UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_26_ADDR(base) ((base) + (0x04AUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_27_ADDR(base) ((base) + (0x04BUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_28_ADDR(base) ((base) + (0x04CUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_29_ADDR(base) ((base) + (0x04DUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_30_ADDR(base) ((base) + (0x04EUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_31_ADDR(base) ((base) + (0x04FUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_32_ADDR(base) ((base) + (0x050UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_33_ADDR(base) ((base) + (0x051UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_34_ADDR(base) ((base) + (0x052UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_35_ADDR(base) ((base) + (0x053UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_36_ADDR(base) ((base) + (0x054UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_37_ADDR(base) ((base) + (0x055UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_38_ADDR(base) ((base) + (0x056UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_39_ADDR(base) ((base) + (0x057UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_40_ADDR(base) ((base) + (0x058UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_41_ADDR(base) ((base) + (0x059UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_42_ADDR(base) ((base) + (0x05AUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_43_ADDR(base) ((base) + (0x05BUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_44_ADDR(base) ((base) + (0x05CUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_45_ADDR(base) ((base) + (0x05DUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_46_ADDR(base) ((base) + (0x05EUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_47_ADDR(base) ((base) + (0x05FUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_48_ADDR(base) ((base) + (0x060UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_49_ADDR(base) ((base) + (0x061UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_50_ADDR(base) ((base) + (0x062UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_51_ADDR(base) ((base) + (0x063UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_52_ADDR(base) ((base) + (0x064UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_53_ADDR(base) ((base) + (0x065UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_54_ADDR(base) ((base) + (0x066UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_55_ADDR(base) ((base) + (0x067UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_56_ADDR(base) ((base) + (0x068UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_57_ADDR(base) ((base) + (0x069UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_58_ADDR(base) ((base) + (0x06AUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_59_ADDR(base) ((base) + (0x06BUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_RO_REG_60_ADDR(base) ((base) + (0x06CUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_0_ADDR(base) ((base) + (0x06DUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_1_ADDR(base) ((base) + (0x06EUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_2_ADDR(base) ((base) + (0x06FUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_3_ADDR(base) ((base) + (0x070UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_4_ADDR(base) ((base) + (0x071UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_5_ADDR(base) ((base) + (0x072UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_6_ADDR(base) ((base) + (0x073UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_7_ADDR(base) ((base) + (0x074UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_8_ADDR(base) ((base) + (0x075UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_9_ADDR(base) ((base) + (0x076UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_10_ADDR(base) ((base) + (0x077UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_ONOFF_ECO_ADDR(base) ((base) + (0x100UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_VSET_ADDR(base) ((base) + (0x101UL) + PMU_SER_BASE)
#define SUB_PMIC_RAMP_BUCK20_CTRL0_ADDR(base) ((base) + (0x102UL) + PMU_SER_BASE)
#define SUB_PMIC_RAMP_BUCK20_CTRL1_ADDR(base) ((base) + (0x103UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_RAMPUP_STATE_ADDR(base) ((base) + (0x104UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_RAMPDOWN_STATE_ADDR(base) ((base) + (0x105UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_RAMP_CHG_RATE_ADDR(base) ((base) + (0x106UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_RAMP_CHG_VSET_ADDR(base) ((base) + (0x107UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CHG_RATE_SEL_ADDR(base) ((base) + (0x108UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_RAMP_STATE_ADDR(base) ((base) + (0x109UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_0_ADDR(base) ((base) + (0x130UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_1_ADDR(base) ((base) + (0x131UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_2_ADDR(base) ((base) + (0x132UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_3_ADDR(base) ((base) + (0x133UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_4_ADDR(base) ((base) + (0x134UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_5_ADDR(base) ((base) + (0x135UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_6_ADDR(base) ((base) + (0x136UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_7_ADDR(base) ((base) + (0x137UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_8_ADDR(base) ((base) + (0x138UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_9_ADDR(base) ((base) + (0x139UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_10_ADDR(base) ((base) + (0x13AUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_11_ADDR(base) ((base) + (0x13BUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_12_ADDR(base) ((base) + (0x13CUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_13_ADDR(base) ((base) + (0x13DUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_14_ADDR(base) ((base) + (0x13EUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_15_ADDR(base) ((base) + (0x13FUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_16_ADDR(base) ((base) + (0x140UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_17_ADDR(base) ((base) + (0x141UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_18_ADDR(base) ((base) + (0x142UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_19_ADDR(base) ((base) + (0x143UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_20_ADDR(base) ((base) + (0x144UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_21_ADDR(base) ((base) + (0x145UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_22_ADDR(base) ((base) + (0x146UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_23_ADDR(base) ((base) + (0x147UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_24_ADDR(base) ((base) + (0x148UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_25_ADDR(base) ((base) + (0x149UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_26_ADDR(base) ((base) + (0x14AUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_27_ADDR(base) ((base) + (0x14BUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_28_ADDR(base) ((base) + (0x14CUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_29_ADDR(base) ((base) + (0x14DUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_30_ADDR(base) ((base) + (0x14EUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_31_ADDR(base) ((base) + (0x14FUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_32_ADDR(base) ((base) + (0x150UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_33_ADDR(base) ((base) + (0x151UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_34_ADDR(base) ((base) + (0x152UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_35_ADDR(base) ((base) + (0x153UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_36_ADDR(base) ((base) + (0x154UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_37_ADDR(base) ((base) + (0x155UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_38_ADDR(base) ((base) + (0x156UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_39_ADDR(base) ((base) + (0x157UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_40_ADDR(base) ((base) + (0x158UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_41_ADDR(base) ((base) + (0x159UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_42_ADDR(base) ((base) + (0x15AUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_43_ADDR(base) ((base) + (0x15BUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_44_ADDR(base) ((base) + (0x15CUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_45_ADDR(base) ((base) + (0x15DUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_46_ADDR(base) ((base) + (0x15EUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_47_ADDR(base) ((base) + (0x15FUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_48_ADDR(base) ((base) + (0x160UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_49_ADDR(base) ((base) + (0x161UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_50_ADDR(base) ((base) + (0x162UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_51_ADDR(base) ((base) + (0x163UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_52_ADDR(base) ((base) + (0x164UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_53_ADDR(base) ((base) + (0x165UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_54_ADDR(base) ((base) + (0x166UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_55_ADDR(base) ((base) + (0x167UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_56_ADDR(base) ((base) + (0x168UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_57_ADDR(base) ((base) + (0x169UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_58_ADDR(base) ((base) + (0x16AUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_59_ADDR(base) ((base) + (0x16BUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_RO_REG_60_ADDR(base) ((base) + (0x16CUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_0_ADDR(base) ((base) + (0x16DUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_1_ADDR(base) ((base) + (0x16EUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_2_ADDR(base) ((base) + (0x16FUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_3_ADDR(base) ((base) + (0x170UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_4_ADDR(base) ((base) + (0x171UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_5_ADDR(base) ((base) + (0x172UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_6_ADDR(base) ((base) + (0x173UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_7_ADDR(base) ((base) + (0x174UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_8_ADDR(base) ((base) + (0x175UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_9_ADDR(base) ((base) + (0x176UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_10_ADDR(base) ((base) + (0x177UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_ONOFF_ECO_ADDR(base) ((base) + (0x200UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_VSET_ADDR(base) ((base) + (0x201UL) + PMU_SER_BASE)
#define SUB_PMIC_RAMP_BUCK12_CTRL0_ADDR(base) ((base) + (0x202UL) + PMU_SER_BASE)
#define SUB_PMIC_RAMP_BUCK12_CTRL1_ADDR(base) ((base) + (0x203UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_RAMPUP_STATE_ADDR(base) ((base) + (0x204UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_RAMPDOWN_STATE_ADDR(base) ((base) + (0x205UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_RAMP_CHG_RATE_ADDR(base) ((base) + (0x206UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_RAMP_CHG_VSET_ADDR(base) ((base) + (0x207UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CHG_RATE_SEL_ADDR(base) ((base) + (0x208UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_RAMP_STATE_ADDR(base) ((base) + (0x209UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_0_ADDR(base) ((base) + (0x230UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_1_ADDR(base) ((base) + (0x231UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_2_ADDR(base) ((base) + (0x232UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_3_ADDR(base) ((base) + (0x233UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_4_ADDR(base) ((base) + (0x234UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_5_ADDR(base) ((base) + (0x235UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_6_ADDR(base) ((base) + (0x236UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_7_ADDR(base) ((base) + (0x237UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_8_ADDR(base) ((base) + (0x238UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_9_ADDR(base) ((base) + (0x239UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_10_ADDR(base) ((base) + (0x23AUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_11_ADDR(base) ((base) + (0x23BUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_12_ADDR(base) ((base) + (0x23CUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_13_ADDR(base) ((base) + (0x23DUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_14_ADDR(base) ((base) + (0x23EUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_15_ADDR(base) ((base) + (0x23FUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_16_ADDR(base) ((base) + (0x240UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_17_ADDR(base) ((base) + (0x241UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_18_ADDR(base) ((base) + (0x242UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_19_ADDR(base) ((base) + (0x243UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_20_ADDR(base) ((base) + (0x244UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_21_ADDR(base) ((base) + (0x245UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_22_ADDR(base) ((base) + (0x246UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_23_ADDR(base) ((base) + (0x247UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_24_ADDR(base) ((base) + (0x248UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_25_ADDR(base) ((base) + (0x249UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_26_ADDR(base) ((base) + (0x24AUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_27_ADDR(base) ((base) + (0x24BUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_28_ADDR(base) ((base) + (0x24CUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_29_ADDR(base) ((base) + (0x24DUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_30_ADDR(base) ((base) + (0x24EUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_31_ADDR(base) ((base) + (0x24FUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_32_ADDR(base) ((base) + (0x250UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_33_ADDR(base) ((base) + (0x251UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_34_ADDR(base) ((base) + (0x252UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_35_ADDR(base) ((base) + (0x253UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_36_ADDR(base) ((base) + (0x254UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_37_ADDR(base) ((base) + (0x255UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_38_ADDR(base) ((base) + (0x256UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_39_ADDR(base) ((base) + (0x257UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_40_ADDR(base) ((base) + (0x258UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_41_ADDR(base) ((base) + (0x259UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_42_ADDR(base) ((base) + (0x25AUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_43_ADDR(base) ((base) + (0x25BUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_44_ADDR(base) ((base) + (0x25CUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_45_ADDR(base) ((base) + (0x25DUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_46_ADDR(base) ((base) + (0x25EUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_47_ADDR(base) ((base) + (0x25FUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_48_ADDR(base) ((base) + (0x260UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_49_ADDR(base) ((base) + (0x261UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_50_ADDR(base) ((base) + (0x262UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_51_ADDR(base) ((base) + (0x263UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_52_ADDR(base) ((base) + (0x264UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_53_ADDR(base) ((base) + (0x265UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_54_ADDR(base) ((base) + (0x266UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_55_ADDR(base) ((base) + (0x267UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_56_ADDR(base) ((base) + (0x268UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_57_ADDR(base) ((base) + (0x269UL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_58_ADDR(base) ((base) + (0x26AUL) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_RO_REG_59_ADDR(base) ((base) + (0x26BUL) + PMU_SER_BASE)
#define SUB_PMIC_LDO52_ONOFF_ECO_ADDR(base) ((base) + (0x300UL) + PMU_SER_BASE)
#define SUB_PMIC_LDO52_VSET_ADDR(base) ((base) + (0x301UL) + PMU_SER_BASE)
#define SUB_PMIC_LDO52_CFG_0_ADDR(base) ((base) + (0x302UL) + PMU_SER_BASE)
#define SUB_PMIC_SW10_ONOFF_ECO_ADDR(base) ((base) + (0x370UL) + PMU_SER_BASE)
#define SUB_PMIC_SW10_CFG_0_ADDR(base) ((base) + (0x371UL) + PMU_SER_BASE)
#define SUB_PMIC_D2A_SER_RESERV0_ADDR(base) ((base) + (0x380UL) + PMU_SER_BASE)
#define SUB_PMIC_D2A_SER_RESERV1_ADDR(base) ((base) + (0x381UL) + PMU_SER_BASE)
#else
#define SUB_PMIC_BUCK10_ONOFF_ECO_ADDR(base) ((base) + (0x000) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_VSET_ADDR(base) ((base) + (0x001) + PMU_SER_BASE)
#define SUB_PMIC_RAMP_BUCK10_CTRL0_ADDR(base) ((base) + (0x002) + PMU_SER_BASE)
#define SUB_PMIC_RAMP_BUCK10_CTRL1_ADDR(base) ((base) + (0x003) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_RAMPUP_STATE_ADDR(base) ((base) + (0x004) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_RAMPDOWN_STATE_ADDR(base) ((base) + (0x005) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_RAMP_CHG_RATE_ADDR(base) ((base) + (0x006) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_RAMP_CHG_VSET_ADDR(base) ((base) + (0x007) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CHG_RATE_SEL_ADDR(base) ((base) + (0x008) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_RAMP_STATE_ADDR(base) ((base) + (0x009) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_0_ADDR(base) ((base) + (0x030) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_1_ADDR(base) ((base) + (0x031) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_2_ADDR(base) ((base) + (0x032) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_3_ADDR(base) ((base) + (0x033) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_4_ADDR(base) ((base) + (0x034) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_5_ADDR(base) ((base) + (0x035) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_6_ADDR(base) ((base) + (0x036) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_7_ADDR(base) ((base) + (0x037) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_8_ADDR(base) ((base) + (0x038) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_9_ADDR(base) ((base) + (0x039) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_10_ADDR(base) ((base) + (0x03A) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_11_ADDR(base) ((base) + (0x03B) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_12_ADDR(base) ((base) + (0x03C) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_13_ADDR(base) ((base) + (0x03D) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_14_ADDR(base) ((base) + (0x03E) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_15_ADDR(base) ((base) + (0x03F) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_16_ADDR(base) ((base) + (0x040) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_17_ADDR(base) ((base) + (0x041) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_18_ADDR(base) ((base) + (0x042) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_19_ADDR(base) ((base) + (0x043) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_20_ADDR(base) ((base) + (0x044) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_21_ADDR(base) ((base) + (0x045) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_22_ADDR(base) ((base) + (0x046) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_23_ADDR(base) ((base) + (0x047) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_24_ADDR(base) ((base) + (0x048) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_25_ADDR(base) ((base) + (0x049) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_26_ADDR(base) ((base) + (0x04A) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_27_ADDR(base) ((base) + (0x04B) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_28_ADDR(base) ((base) + (0x04C) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_29_ADDR(base) ((base) + (0x04D) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_30_ADDR(base) ((base) + (0x04E) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_31_ADDR(base) ((base) + (0x04F) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_32_ADDR(base) ((base) + (0x050) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_33_ADDR(base) ((base) + (0x051) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_34_ADDR(base) ((base) + (0x052) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_35_ADDR(base) ((base) + (0x053) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_36_ADDR(base) ((base) + (0x054) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_37_ADDR(base) ((base) + (0x055) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_38_ADDR(base) ((base) + (0x056) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_39_ADDR(base) ((base) + (0x057) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_40_ADDR(base) ((base) + (0x058) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_41_ADDR(base) ((base) + (0x059) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_42_ADDR(base) ((base) + (0x05A) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_43_ADDR(base) ((base) + (0x05B) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_44_ADDR(base) ((base) + (0x05C) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_45_ADDR(base) ((base) + (0x05D) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_46_ADDR(base) ((base) + (0x05E) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_47_ADDR(base) ((base) + (0x05F) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_48_ADDR(base) ((base) + (0x060) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_49_ADDR(base) ((base) + (0x061) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_50_ADDR(base) ((base) + (0x062) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_51_ADDR(base) ((base) + (0x063) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_52_ADDR(base) ((base) + (0x064) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_53_ADDR(base) ((base) + (0x065) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_54_ADDR(base) ((base) + (0x066) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_55_ADDR(base) ((base) + (0x067) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_56_ADDR(base) ((base) + (0x068) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_57_ADDR(base) ((base) + (0x069) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_58_ADDR(base) ((base) + (0x06A) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_CFG_REG_59_ADDR(base) ((base) + (0x06B) + PMU_SER_BASE)
#define SUB_PMIC_BUCK10_RO_REG_60_ADDR(base) ((base) + (0x06C) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_0_ADDR(base) ((base) + (0x06D) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_1_ADDR(base) ((base) + (0x06E) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_2_ADDR(base) ((base) + (0x06F) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_3_ADDR(base) ((base) + (0x070) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_4_ADDR(base) ((base) + (0x071) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_5_ADDR(base) ((base) + (0x072) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_6_ADDR(base) ((base) + (0x073) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_7_ADDR(base) ((base) + (0x074) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_8_ADDR(base) ((base) + (0x075) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_9_ADDR(base) ((base) + (0x076) + PMU_SER_BASE)
#define SUB_PMIC_BUCK11_CFG_REG_10_ADDR(base) ((base) + (0x077) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_ONOFF_ECO_ADDR(base) ((base) + (0x100) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_VSET_ADDR(base) ((base) + (0x101) + PMU_SER_BASE)
#define SUB_PMIC_RAMP_BUCK20_CTRL0_ADDR(base) ((base) + (0x102) + PMU_SER_BASE)
#define SUB_PMIC_RAMP_BUCK20_CTRL1_ADDR(base) ((base) + (0x103) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_RAMPUP_STATE_ADDR(base) ((base) + (0x104) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_RAMPDOWN_STATE_ADDR(base) ((base) + (0x105) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_RAMP_CHG_RATE_ADDR(base) ((base) + (0x106) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_RAMP_CHG_VSET_ADDR(base) ((base) + (0x107) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CHG_RATE_SEL_ADDR(base) ((base) + (0x108) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_RAMP_STATE_ADDR(base) ((base) + (0x109) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_0_ADDR(base) ((base) + (0x130) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_1_ADDR(base) ((base) + (0x131) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_2_ADDR(base) ((base) + (0x132) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_3_ADDR(base) ((base) + (0x133) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_4_ADDR(base) ((base) + (0x134) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_5_ADDR(base) ((base) + (0x135) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_6_ADDR(base) ((base) + (0x136) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_7_ADDR(base) ((base) + (0x137) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_8_ADDR(base) ((base) + (0x138) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_9_ADDR(base) ((base) + (0x139) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_10_ADDR(base) ((base) + (0x13A) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_11_ADDR(base) ((base) + (0x13B) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_12_ADDR(base) ((base) + (0x13C) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_13_ADDR(base) ((base) + (0x13D) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_14_ADDR(base) ((base) + (0x13E) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_15_ADDR(base) ((base) + (0x13F) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_16_ADDR(base) ((base) + (0x140) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_17_ADDR(base) ((base) + (0x141) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_18_ADDR(base) ((base) + (0x142) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_19_ADDR(base) ((base) + (0x143) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_20_ADDR(base) ((base) + (0x144) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_21_ADDR(base) ((base) + (0x145) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_22_ADDR(base) ((base) + (0x146) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_23_ADDR(base) ((base) + (0x147) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_24_ADDR(base) ((base) + (0x148) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_25_ADDR(base) ((base) + (0x149) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_26_ADDR(base) ((base) + (0x14A) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_27_ADDR(base) ((base) + (0x14B) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_28_ADDR(base) ((base) + (0x14C) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_29_ADDR(base) ((base) + (0x14D) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_30_ADDR(base) ((base) + (0x14E) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_31_ADDR(base) ((base) + (0x14F) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_32_ADDR(base) ((base) + (0x150) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_33_ADDR(base) ((base) + (0x151) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_34_ADDR(base) ((base) + (0x152) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_35_ADDR(base) ((base) + (0x153) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_36_ADDR(base) ((base) + (0x154) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_37_ADDR(base) ((base) + (0x155) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_38_ADDR(base) ((base) + (0x156) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_39_ADDR(base) ((base) + (0x157) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_40_ADDR(base) ((base) + (0x158) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_41_ADDR(base) ((base) + (0x159) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_42_ADDR(base) ((base) + (0x15A) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_43_ADDR(base) ((base) + (0x15B) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_44_ADDR(base) ((base) + (0x15C) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_45_ADDR(base) ((base) + (0x15D) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_46_ADDR(base) ((base) + (0x15E) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_47_ADDR(base) ((base) + (0x15F) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_48_ADDR(base) ((base) + (0x160) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_49_ADDR(base) ((base) + (0x161) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_50_ADDR(base) ((base) + (0x162) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_51_ADDR(base) ((base) + (0x163) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_52_ADDR(base) ((base) + (0x164) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_53_ADDR(base) ((base) + (0x165) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_54_ADDR(base) ((base) + (0x166) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_55_ADDR(base) ((base) + (0x167) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_56_ADDR(base) ((base) + (0x168) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_57_ADDR(base) ((base) + (0x169) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_58_ADDR(base) ((base) + (0x16A) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_CFG_REG_59_ADDR(base) ((base) + (0x16B) + PMU_SER_BASE)
#define SUB_PMIC_BUCK20_RO_REG_60_ADDR(base) ((base) + (0x16C) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_0_ADDR(base) ((base) + (0x16D) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_1_ADDR(base) ((base) + (0x16E) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_2_ADDR(base) ((base) + (0x16F) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_3_ADDR(base) ((base) + (0x170) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_4_ADDR(base) ((base) + (0x171) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_5_ADDR(base) ((base) + (0x172) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_6_ADDR(base) ((base) + (0x173) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_7_ADDR(base) ((base) + (0x174) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_8_ADDR(base) ((base) + (0x175) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_9_ADDR(base) ((base) + (0x176) + PMU_SER_BASE)
#define SUB_PMIC_BUCK21_CFG_REG_10_ADDR(base) ((base) + (0x177) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_ONOFF_ECO_ADDR(base) ((base) + (0x200) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_VSET_ADDR(base) ((base) + (0x201) + PMU_SER_BASE)
#define SUB_PMIC_RAMP_BUCK12_CTRL0_ADDR(base) ((base) + (0x202) + PMU_SER_BASE)
#define SUB_PMIC_RAMP_BUCK12_CTRL1_ADDR(base) ((base) + (0x203) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_RAMPUP_STATE_ADDR(base) ((base) + (0x204) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_RAMPDOWN_STATE_ADDR(base) ((base) + (0x205) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_RAMP_CHG_RATE_ADDR(base) ((base) + (0x206) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_RAMP_CHG_VSET_ADDR(base) ((base) + (0x207) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CHG_RATE_SEL_ADDR(base) ((base) + (0x208) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_RAMP_STATE_ADDR(base) ((base) + (0x209) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_0_ADDR(base) ((base) + (0x230) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_1_ADDR(base) ((base) + (0x231) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_2_ADDR(base) ((base) + (0x232) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_3_ADDR(base) ((base) + (0x233) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_4_ADDR(base) ((base) + (0x234) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_5_ADDR(base) ((base) + (0x235) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_6_ADDR(base) ((base) + (0x236) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_7_ADDR(base) ((base) + (0x237) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_8_ADDR(base) ((base) + (0x238) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_9_ADDR(base) ((base) + (0x239) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_10_ADDR(base) ((base) + (0x23A) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_11_ADDR(base) ((base) + (0x23B) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_12_ADDR(base) ((base) + (0x23C) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_13_ADDR(base) ((base) + (0x23D) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_14_ADDR(base) ((base) + (0x23E) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_15_ADDR(base) ((base) + (0x23F) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_16_ADDR(base) ((base) + (0x240) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_17_ADDR(base) ((base) + (0x241) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_18_ADDR(base) ((base) + (0x242) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_19_ADDR(base) ((base) + (0x243) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_20_ADDR(base) ((base) + (0x244) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_21_ADDR(base) ((base) + (0x245) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_22_ADDR(base) ((base) + (0x246) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_23_ADDR(base) ((base) + (0x247) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_24_ADDR(base) ((base) + (0x248) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_25_ADDR(base) ((base) + (0x249) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_26_ADDR(base) ((base) + (0x24A) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_27_ADDR(base) ((base) + (0x24B) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_28_ADDR(base) ((base) + (0x24C) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_29_ADDR(base) ((base) + (0x24D) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_30_ADDR(base) ((base) + (0x24E) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_31_ADDR(base) ((base) + (0x24F) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_32_ADDR(base) ((base) + (0x250) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_33_ADDR(base) ((base) + (0x251) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_34_ADDR(base) ((base) + (0x252) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_35_ADDR(base) ((base) + (0x253) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_36_ADDR(base) ((base) + (0x254) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_37_ADDR(base) ((base) + (0x255) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_38_ADDR(base) ((base) + (0x256) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_39_ADDR(base) ((base) + (0x257) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_40_ADDR(base) ((base) + (0x258) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_41_ADDR(base) ((base) + (0x259) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_42_ADDR(base) ((base) + (0x25A) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_43_ADDR(base) ((base) + (0x25B) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_44_ADDR(base) ((base) + (0x25C) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_45_ADDR(base) ((base) + (0x25D) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_46_ADDR(base) ((base) + (0x25E) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_47_ADDR(base) ((base) + (0x25F) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_48_ADDR(base) ((base) + (0x260) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_49_ADDR(base) ((base) + (0x261) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_50_ADDR(base) ((base) + (0x262) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_51_ADDR(base) ((base) + (0x263) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_52_ADDR(base) ((base) + (0x264) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_53_ADDR(base) ((base) + (0x265) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_54_ADDR(base) ((base) + (0x266) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_55_ADDR(base) ((base) + (0x267) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_56_ADDR(base) ((base) + (0x268) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_57_ADDR(base) ((base) + (0x269) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_CFG_REG_58_ADDR(base) ((base) + (0x26A) + PMU_SER_BASE)
#define SUB_PMIC_BUCK12_RO_REG_59_ADDR(base) ((base) + (0x26B) + PMU_SER_BASE)
#define SUB_PMIC_LDO52_ONOFF_ECO_ADDR(base) ((base) + (0x300) + PMU_SER_BASE)
#define SUB_PMIC_LDO52_VSET_ADDR(base) ((base) + (0x301) + PMU_SER_BASE)
#define SUB_PMIC_LDO52_CFG_0_ADDR(base) ((base) + (0x302) + PMU_SER_BASE)
#define SUB_PMIC_SW10_ONOFF_ECO_ADDR(base) ((base) + (0x370) + PMU_SER_BASE)
#define SUB_PMIC_SW10_CFG_0_ADDR(base) ((base) + (0x371) + PMU_SER_BASE)
#define SUB_PMIC_D2A_SER_RESERV0_ADDR(base) ((base) + (0x380) + PMU_SER_BASE)
#define SUB_PMIC_D2A_SER_RESERV1_ADDR(base) ((base) + (0x381) + PMU_SER_BASE)
#endif
 #define PMU_LRA_BASE 0x1000
#ifndef __SOC_H_FOR_ASM__
#define SUB_PMIC_LRA_CTRL_ADDR(base) ((base) + (0x000UL) + PMU_LRA_BASE)
#define SUB_PMIC_BRAKE_CTRL_ADDR(base) ((base) + (0x001UL) + PMU_LRA_BASE)
#define SUB_PMIC_DRV_H_L_NUM_ADDR(base) ((base) + (0x002UL) + PMU_LRA_BASE)
#define SUB_PMIC_BRK_H_NUM_ADDR(base) ((base) + (0x003UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_CFG0_ADDR(base) ((base) + (0x004UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_CFG1_ADDR(base) ((base) + (0x005UL) + PMU_LRA_BASE)
#define SUB_PMIC_DRV_TIME_CFG_ADDR(base) ((base) + (0x006UL) + PMU_LRA_BASE)
#define SUB_PMIC_MAX_TIME_CFG_ADDR(base) ((base) + (0x007UL) + PMU_LRA_BASE)
#define SUB_PMIC_BEMF_OUT_CFG_ADDR(base) ((base) + (0x008UL) + PMU_LRA_BASE)
#define SUB_PMIC_BEMF_TIME_ADDR(base) ((base) + (0x009UL) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_INIT_CFG_LOW_ADDR(base) ((base) + (0x00AUL) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_INIT_CFG_HIGH_ADDR(base) ((base) + (0x00BUL) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_OVDR_CPS_ADDR(base) ((base) + (0x00CUL) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_OVDR_CFG_LOW_ADDR(base) ((base) + (0x00DUL) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_OVDR_CFG_HIGH_ADDR(base) ((base) + (0x00EUL) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_NORMAL_CPS_ADDR(base) ((base) + (0x00FUL) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_NORMAL_CFG_LOW_ADDR(base) ((base) + (0x010UL) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_NORMAL_CFG_HIGH_ADDR(base) ((base) + (0x011UL) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_BRAKE_CPS_ADDR(base) ((base) + (0x012UL) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_BRAKE_CFG_LOW_ADDR(base) ((base) + (0x013UL) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_BRAKE_CFG_HIGH_ADDR(base) ((base) + (0x014UL) + PMU_LRA_BASE)
#define SUB_PMIC_EOB_K_1_LOW_ADDR(base) ((base) + (0x015UL) + PMU_LRA_BASE)
#define SUB_PMIC_EOB_K_1_HIGH_ADDR(base) ((base) + (0x016UL) + PMU_LRA_BASE)
#define SUB_PMIC_EOB_K_2_LOW_ADDR(base) ((base) + (0x017UL) + PMU_LRA_BASE)
#define SUB_PMIC_EOB_K_2_HIGH_ADDR(base) ((base) + (0x018UL) + PMU_LRA_BASE)
#define SUB_PMIC_ADC_MAX_MIN_TH_ADDR(base) ((base) + (0x019UL) + PMU_LRA_BASE)
#define SUB_PMIC_TH_BOOST_V_CFG_LOW_ADDR(base) ((base) + (0x01AUL) + PMU_LRA_BASE)
#define SUB_PMIC_TH_BOOST_V_CFG_HIGH_ADDR(base) ((base) + (0x01BUL) + PMU_LRA_BASE)
#define SUB_PMIC_GATE_VOLTAGE1_LOW_ADDR(base) ((base) + (0x01CUL) + PMU_LRA_BASE)
#define SUB_PMIC_GATE_VOLTAGE1_HIGH_ADDR(base) ((base) + (0x01DUL) + PMU_LRA_BASE)
#define SUB_PMIC_GATE_VOLTAGE2_LOW_ADDR(base) ((base) + (0x01EUL) + PMU_LRA_BASE)
#define SUB_PMIC_GATE_VOLTAGE2_HIGH_ADDR(base) ((base) + (0x01FUL) + PMU_LRA_BASE)
#define SUB_PMIC_GATE_VOLTAGE3_LOW_ADDR(base) ((base) + (0x020UL) + PMU_LRA_BASE)
#define SUB_PMIC_GATE_VOLTAGE3_HIGH_ADDR(base) ((base) + (0x021UL) + PMU_LRA_BASE)
#define SUB_PMIC_BRK_BEMF_MIN_CFG_L_ADDR(base) ((base) + (0x022UL) + PMU_LRA_BASE)
#define SUB_PMIC_BRK_BEMF_MIN_CFG_H_ADDR(base) ((base) + (0x023UL) + PMU_LRA_BASE)
#define SUB_PMIC_REF_U_CFG_L_ADDR(base) ((base) + (0x024UL) + PMU_LRA_BASE)
#define SUB_PMIC_REF_U_CFG_H_ADDR(base) ((base) + (0x025UL) + PMU_LRA_BASE)
#define SUB_PMIC_KFIT_DATA_SEL_ADDR(base) ((base) + (0x026UL) + PMU_LRA_BASE)
#define SUB_PMIC_K_RVS_LOW_ADDR(base) ((base) + (0x027UL) + PMU_LRA_BASE)
#define SUB_PMIC_K_RVS_HIGH_ADDR(base) ((base) + (0x028UL) + PMU_LRA_BASE)
#define SUB_PMIC_B_RVS_LOW_ADDR(base) ((base) + (0x029UL) + PMU_LRA_BASE)
#define SUB_PMIC_B_RVS_HIGH_ADDR(base) ((base) + (0x02AUL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA1_CFG_ADDR(base) ((base) + (0x02BUL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA2_CFG_ADDR(base) ((base) + (0x02CUL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA3_CFG_ADDR(base) ((base) + (0x02DUL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA4_CFG_ADDR(base) ((base) + (0x02EUL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA5_CFG_ADDR(base) ((base) + (0x02FUL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA6_CFG_ADDR(base) ((base) + (0x030UL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA7_CFG_ADDR(base) ((base) + (0x031UL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA8_CFG_ADDR(base) ((base) + (0x032UL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA9_CFG_ADDR(base) ((base) + (0x033UL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA10_CFG_ADDR(base) ((base) + (0x034UL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA11_CFG_ADDR(base) ((base) + (0x035UL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA12_CFG_ADDR(base) ((base) + (0x036UL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA13_CFG_ADDR(base) ((base) + (0x037UL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA14_CFG_ADDR(base) ((base) + (0x038UL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA15_CFG_ADDR(base) ((base) + (0x039UL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA16_CFG_ADDR(base) ((base) + (0x03AUL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA17_CFG_ADDR(base) ((base) + (0x03BUL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA18_CFG_ADDR(base) ((base) + (0x03CUL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA19_CFG_ADDR(base) ((base) + (0x03DUL) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA20_CFG_ADDR(base) ((base) + (0x03EUL) + PMU_LRA_BASE)
#define SUB_PMIC_OUT_BEMF_VOL_L_ADDR(base) ((base) + (0x03FUL) + PMU_LRA_BASE)
#define SUB_PMIC_OUT_BEMF_VOL_H_ADDR(base) ((base) + (0x040UL) + PMU_LRA_BASE)
#define SUB_PMIC_OUT_NML_VBEMF_L_ADDR(base) ((base) + (0x041UL) + PMU_LRA_BASE)
#define SUB_PMIC_OUT_NML_VBEMF_H_ADDR(base) ((base) + (0x042UL) + PMU_LRA_BASE)
#define SUB_PMIC_OV_BRK_HPRIOD_ADDR(base) ((base) + (0x043UL) + PMU_LRA_BASE)
#define SUB_PMIC_BRK_HPRIOD_ADDR(base) ((base) + (0x044UL) + PMU_LRA_BASE)
#define SUB_PMIC_UREAL_DATA_L_ADDR(base) ((base) + (0x045UL) + PMU_LRA_BASE)
#define SUB_PMIC_UREAL_DATA_H_ADDR(base) ((base) + (0x046UL) + PMU_LRA_BASE)
#define SUB_PMIC_ADC_CALI_MAX_L_ADDR(base) ((base) + (0x047UL) + PMU_LRA_BASE)
#define SUB_PMIC_ADC_CALI_MAX_H_ADDR(base) ((base) + (0x048UL) + PMU_LRA_BASE)
#define SUB_PMIC_ADC_CALI_MIN_L_ADDR(base) ((base) + (0x049UL) + PMU_LRA_BASE)
#define SUB_PMIC_ADC_CALI_MIN_H_ADDR(base) ((base) + (0x04AUL) + PMU_LRA_BASE)
#define SUB_PMIC_ADC_ZERO_L_ADDR(base) ((base) + (0x04BUL) + PMU_LRA_BASE)
#define SUB_PMIC_ADC_ZERO_H_ADDR(base) ((base) + (0x04CUL) + PMU_LRA_BASE)
#define SUB_PMIC_KFIT_DATA_L_ADDR(base) ((base) + (0x04DUL) + PMU_LRA_BASE)
#define SUB_PMIC_KFIT_DATA_H_ADDR(base) ((base) + (0x04EUL) + PMU_LRA_BASE)
#define SUB_PMIC_FSM_STATE_ADDR(base) ((base) + (0x04FUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_1_ADDR(base) ((base) + (0x050UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_2_ADDR(base) ((base) + (0x051UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_3_ADDR(base) ((base) + (0x052UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_4_ADDR(base) ((base) + (0x053UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_5_ADDR(base) ((base) + (0x054UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_6_ADDR(base) ((base) + (0x055UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_7_ADDR(base) ((base) + (0x056UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_8_ADDR(base) ((base) + (0x057UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_9_ADDR(base) ((base) + (0x058UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_10_ADDR(base) ((base) + (0x059UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_11_ADDR(base) ((base) + (0x05AUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_12_ADDR(base) ((base) + (0x05BUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_13_ADDR(base) ((base) + (0x05CUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_14_ADDR(base) ((base) + (0x05DUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_15_ADDR(base) ((base) + (0x05EUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_16_ADDR(base) ((base) + (0x05FUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_17_ADDR(base) ((base) + (0x060UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_18_ADDR(base) ((base) + (0x061UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_19_ADDR(base) ((base) + (0x062UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_20_ADDR(base) ((base) + (0x063UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_CPS_ADDR(base) ((base) + (0x064UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_1_L_ADDR(base) ((base) + (0x065UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_1_H_ADDR(base) ((base) + (0x066UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_2_L_ADDR(base) ((base) + (0x067UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_2_H_ADDR(base) ((base) + (0x068UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_3_L_ADDR(base) ((base) + (0x069UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_3_H_ADDR(base) ((base) + (0x06AUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_4_L_ADDR(base) ((base) + (0x06BUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_4_H_ADDR(base) ((base) + (0x06CUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_5_L_ADDR(base) ((base) + (0x06DUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_5_H_ADDR(base) ((base) + (0x06EUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_6_L_ADDR(base) ((base) + (0x06FUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_6_H_ADDR(base) ((base) + (0x070UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_7_L_ADDR(base) ((base) + (0x071UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_7_H_ADDR(base) ((base) + (0x072UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_8_L_ADDR(base) ((base) + (0x073UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_8_H_ADDR(base) ((base) + (0x074UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_9_L_ADDR(base) ((base) + (0x075UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_9_H_ADDR(base) ((base) + (0x076UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_10_L_ADDR(base) ((base) + (0x077UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_10_H_ADDR(base) ((base) + (0x078UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_11_L_ADDR(base) ((base) + (0x079UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_11_H_ADDR(base) ((base) + (0x07AUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_12_L_ADDR(base) ((base) + (0x07BUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_12_H_ADDR(base) ((base) + (0x07CUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_13_L_ADDR(base) ((base) + (0x07DUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_13_H_ADDR(base) ((base) + (0x07EUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_14_L_ADDR(base) ((base) + (0x07FUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_14_H_ADDR(base) ((base) + (0x080UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_15_L_ADDR(base) ((base) + (0x081UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_15_H_ADDR(base) ((base) + (0x082UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_16_L_ADDR(base) ((base) + (0x083UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_16_H_ADDR(base) ((base) + (0x084UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_17_L_ADDR(base) ((base) + (0x085UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_17_H_ADDR(base) ((base) + (0x086UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_18_L_ADDR(base) ((base) + (0x087UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_18_H_ADDR(base) ((base) + (0x088UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_19_L_ADDR(base) ((base) + (0x089UL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_19_H_ADDR(base) ((base) + (0x08AUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_20_L_ADDR(base) ((base) + (0x08BUL) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_20_H_ADDR(base) ((base) + (0x08CUL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_ANA_CFG0_ADDR(base) ((base) + (0x08DUL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_ANA_CFG1_ADDR(base) ((base) + (0x08EUL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_ANA_CFG2_ADDR(base) ((base) + (0x08FUL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_ANA_CFG3_ADDR(base) ((base) + (0x090UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_OCP_CFG_ADDR(base) ((base) + (0x091UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_IBIAS_SEL_ADDR(base) ((base) + (0x092UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_OVP_CFG_ADDR(base) ((base) + (0x093UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_ANA_RESERVED0_ADDR(base) ((base) + (0x094UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_ANA_RESERVED1_ADDR(base) ((base) + (0x095UL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_SAMPLE_EN_ADDR(base) ((base) + (0x096UL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_SAMPLE_VALID_ADDR(base) ((base) + (0x097UL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_MAX_L_ADDR(base) ((base) + (0x098UL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_MAX_H_ADDR(base) ((base) + (0x099UL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_MIN_L_ADDR(base) ((base) + (0x09AUL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_MIN_H_ADDR(base) ((base) + (0x09BUL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_FIRST_L_ADDR(base) ((base) + (0x09CUL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_FIRST_H_ADDR(base) ((base) + (0x09DUL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_LAST_L_ADDR(base) ((base) + (0x09EUL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_LAST_H_ADDR(base) ((base) + (0x09FUL) + PMU_LRA_BASE)
#define SUB_PMIC_HP_DR_TIME_ADDR(base) ((base) + (0x0A0UL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_NEXT_DR_ADDR(base) ((base) + (0x0A1UL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_TIME_L_ADDR(base) ((base) + (0x0A2UL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_TIME_H_ADDR(base) ((base) + (0x0A3UL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_DUTY_NUM_ADDR(base) ((base) + (0x0A4UL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_RANK_NUM_L_ADDR(base) ((base) + (0x0A5UL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_RANK_NUM_H_ADDR(base) ((base) + (0x0A6UL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_OCP_NUM_ADDR(base) ((base) + (0x0A7UL) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_FSM_ADDR(base) ((base) + (0x0A8UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_DEBUG_EN_ADDR(base) ((base) + (0x0A9UL) + PMU_LRA_BASE)
#define SUB_PMIC_DEBUG_ADC_CTRL_ADDR(base) ((base) + (0x0AAUL) + PMU_LRA_BASE)
#define SUB_PMIC_DEBUG_ADC_VALID_ADDR(base) ((base) + (0x0ABUL) + PMU_LRA_BASE)
#define SUB_PMIC_DEBUG_ADC_DATA_L_ADDR(base) ((base) + (0x0ACUL) + PMU_LRA_BASE)
#define SUB_PMIC_DEBUG_ADC_DATA_H_ADDR(base) ((base) + (0x0ADUL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_CLK_G_CTRL_ADDR(base) ((base) + (0x0AEUL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_NML_TIME_L_ADDR(base) ((base) + (0x0AFUL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_NML_TIME_M_ADDR(base) ((base) + (0x0B0UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_NML_TIME_H_ADDR(base) ((base) + (0x0B1UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_NML_BEMF_L_ADDR(base) ((base) + (0x0B2UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_NML_BEMF_H_ADDR(base) ((base) + (0x0B3UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_GATE_CTRL_REG_ADDR(base) ((base) + (0x0B4UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_DUTYRES_REG_ADDR(base) ((base) + (0x0B5UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_BFST_OK_REG_ADDR(base) ((base) + (0x0B6UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_BEMF_FST_REG_LOW_ADDR(base) ((base) + (0x0B7UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_BEMF_FST_REG_HIGH_ADDR(base) ((base) + (0x0B8UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_OFF_L_ADDR(base) ((base) + (0x0B9UL) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_OFF_H_ADDR(base) ((base) + (0x0BAUL) + PMU_LRA_BASE)
#define SUB_PMIC_OFFSET_TIME_ADDR(base) ((base) + (0x0BBUL) + PMU_LRA_BASE)
#define SUB_PMIC_WE_ERM_LOCK_ADDR(base) ((base) + (0x0BCUL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_MODE_CFG_ADDR(base) ((base) + (0x0BDUL) + PMU_LRA_BASE)
#define SUB_PMIC_FRQC_SEL_CFG_ADDR(base) ((base) + (0x0BEUL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DUTY_OVDR_CFG_L_ADDR(base) ((base) + (0x0BFUL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DUTY_OVDR_CFG_H_ADDR(base) ((base) + (0x0C0UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DUTY_NML_CFG_L_ADDR(base) ((base) + (0x0C1UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DUTY_NML_CFG_H_ADDR(base) ((base) + (0x0C2UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DUTY_BRK_CFG_L_ADDR(base) ((base) + (0x0C3UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DUTY_BRK_CFG_H_ADDR(base) ((base) + (0x0C4UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_K_EOB_L_ADDR(base) ((base) + (0x0C5UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_K_EOB_H_ADDR(base) ((base) + (0x0C6UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_OVDR_NUM_ADDR(base) ((base) + (0x0C7UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_BRK_NUM_ADDR(base) ((base) + (0x0C8UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DRT_CFG_ADDR(base) ((base) + (0x0C9UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DETE_TIME_CFG_ADDR(base) ((base) + (0x0CAUL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_TWAIT_CFG_ADDR(base) ((base) + (0x0CBUL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_OVDR_THRE_L_ADDR(base) ((base) + (0x0CCUL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_OVDR_THRE_H_ADDR(base) ((base) + (0x0CDUL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_BRK_THRE_L_ADDR(base) ((base) + (0x0CEUL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_BRK_THRE_H_ADDR(base) ((base) + (0x0CFUL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_OPL_OVDR_NUM_ADDR(base) ((base) + (0x0D0UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_OPL_BRK_NUM_ADDR(base) ((base) + (0x0D1UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_ADCAVE_CFG_ADDR(base) ((base) + (0x0D2UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_ABN_ONOFF_ADDR(base) ((base) + (0x0D3UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_ABN_TH_L_ADDR(base) ((base) + (0x0D4UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_ABN_TH_H_ADDR(base) ((base) + (0x0D5UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_OFF_L_ADDR(base) ((base) + (0x0D6UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_OFF_H_ADDR(base) ((base) + (0x0D7UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_ANA_CFG0_ADDR(base) ((base) + (0x0D8UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_PRD_SMPL_EN_ADDR(base) ((base) + (0x0D9UL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_PRD_SMPL_VALID_ADDR(base) ((base) + (0x0DAUL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_PRD_BEMF_L_ADDR(base) ((base) + (0x0DBUL) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_PRD_BEMF_H_ADDR(base) ((base) + (0x0DCUL) + PMU_LRA_BASE)
#else
#define SUB_PMIC_LRA_CTRL_ADDR(base) ((base) + (0x000) + PMU_LRA_BASE)
#define SUB_PMIC_BRAKE_CTRL_ADDR(base) ((base) + (0x001) + PMU_LRA_BASE)
#define SUB_PMIC_DRV_H_L_NUM_ADDR(base) ((base) + (0x002) + PMU_LRA_BASE)
#define SUB_PMIC_BRK_H_NUM_ADDR(base) ((base) + (0x003) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_CFG0_ADDR(base) ((base) + (0x004) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_CFG1_ADDR(base) ((base) + (0x005) + PMU_LRA_BASE)
#define SUB_PMIC_DRV_TIME_CFG_ADDR(base) ((base) + (0x006) + PMU_LRA_BASE)
#define SUB_PMIC_MAX_TIME_CFG_ADDR(base) ((base) + (0x007) + PMU_LRA_BASE)
#define SUB_PMIC_BEMF_OUT_CFG_ADDR(base) ((base) + (0x008) + PMU_LRA_BASE)
#define SUB_PMIC_BEMF_TIME_ADDR(base) ((base) + (0x009) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_INIT_CFG_LOW_ADDR(base) ((base) + (0x00A) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_INIT_CFG_HIGH_ADDR(base) ((base) + (0x00B) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_OVDR_CPS_ADDR(base) ((base) + (0x00C) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_OVDR_CFG_LOW_ADDR(base) ((base) + (0x00D) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_OVDR_CFG_HIGH_ADDR(base) ((base) + (0x00E) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_NORMAL_CPS_ADDR(base) ((base) + (0x00F) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_NORMAL_CFG_LOW_ADDR(base) ((base) + (0x010) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_NORMAL_CFG_HIGH_ADDR(base) ((base) + (0x011) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_BRAKE_CPS_ADDR(base) ((base) + (0x012) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_BRAKE_CFG_LOW_ADDR(base) ((base) + (0x013) + PMU_LRA_BASE)
#define SUB_PMIC_DUTY_BRAKE_CFG_HIGH_ADDR(base) ((base) + (0x014) + PMU_LRA_BASE)
#define SUB_PMIC_EOB_K_1_LOW_ADDR(base) ((base) + (0x015) + PMU_LRA_BASE)
#define SUB_PMIC_EOB_K_1_HIGH_ADDR(base) ((base) + (0x016) + PMU_LRA_BASE)
#define SUB_PMIC_EOB_K_2_LOW_ADDR(base) ((base) + (0x017) + PMU_LRA_BASE)
#define SUB_PMIC_EOB_K_2_HIGH_ADDR(base) ((base) + (0x018) + PMU_LRA_BASE)
#define SUB_PMIC_ADC_MAX_MIN_TH_ADDR(base) ((base) + (0x019) + PMU_LRA_BASE)
#define SUB_PMIC_TH_BOOST_V_CFG_LOW_ADDR(base) ((base) + (0x01A) + PMU_LRA_BASE)
#define SUB_PMIC_TH_BOOST_V_CFG_HIGH_ADDR(base) ((base) + (0x01B) + PMU_LRA_BASE)
#define SUB_PMIC_GATE_VOLTAGE1_LOW_ADDR(base) ((base) + (0x01C) + PMU_LRA_BASE)
#define SUB_PMIC_GATE_VOLTAGE1_HIGH_ADDR(base) ((base) + (0x01D) + PMU_LRA_BASE)
#define SUB_PMIC_GATE_VOLTAGE2_LOW_ADDR(base) ((base) + (0x01E) + PMU_LRA_BASE)
#define SUB_PMIC_GATE_VOLTAGE2_HIGH_ADDR(base) ((base) + (0x01F) + PMU_LRA_BASE)
#define SUB_PMIC_GATE_VOLTAGE3_LOW_ADDR(base) ((base) + (0x020) + PMU_LRA_BASE)
#define SUB_PMIC_GATE_VOLTAGE3_HIGH_ADDR(base) ((base) + (0x021) + PMU_LRA_BASE)
#define SUB_PMIC_BRK_BEMF_MIN_CFG_L_ADDR(base) ((base) + (0x022) + PMU_LRA_BASE)
#define SUB_PMIC_BRK_BEMF_MIN_CFG_H_ADDR(base) ((base) + (0x023) + PMU_LRA_BASE)
#define SUB_PMIC_REF_U_CFG_L_ADDR(base) ((base) + (0x024) + PMU_LRA_BASE)
#define SUB_PMIC_REF_U_CFG_H_ADDR(base) ((base) + (0x025) + PMU_LRA_BASE)
#define SUB_PMIC_KFIT_DATA_SEL_ADDR(base) ((base) + (0x026) + PMU_LRA_BASE)
#define SUB_PMIC_K_RVS_LOW_ADDR(base) ((base) + (0x027) + PMU_LRA_BASE)
#define SUB_PMIC_K_RVS_HIGH_ADDR(base) ((base) + (0x028) + PMU_LRA_BASE)
#define SUB_PMIC_B_RVS_LOW_ADDR(base) ((base) + (0x029) + PMU_LRA_BASE)
#define SUB_PMIC_B_RVS_HIGH_ADDR(base) ((base) + (0x02A) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA1_CFG_ADDR(base) ((base) + (0x02B) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA2_CFG_ADDR(base) ((base) + (0x02C) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA3_CFG_ADDR(base) ((base) + (0x02D) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA4_CFG_ADDR(base) ((base) + (0x02E) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA5_CFG_ADDR(base) ((base) + (0x02F) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA6_CFG_ADDR(base) ((base) + (0x030) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA7_CFG_ADDR(base) ((base) + (0x031) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA8_CFG_ADDR(base) ((base) + (0x032) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA9_CFG_ADDR(base) ((base) + (0x033) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA10_CFG_ADDR(base) ((base) + (0x034) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA11_CFG_ADDR(base) ((base) + (0x035) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA12_CFG_ADDR(base) ((base) + (0x036) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA13_CFG_ADDR(base) ((base) + (0x037) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA14_CFG_ADDR(base) ((base) + (0x038) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA15_CFG_ADDR(base) ((base) + (0x039) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA16_CFG_ADDR(base) ((base) + (0x03A) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA17_CFG_ADDR(base) ((base) + (0x03B) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA18_CFG_ADDR(base) ((base) + (0x03C) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA19_CFG_ADDR(base) ((base) + (0x03D) + PMU_LRA_BASE)
#define SUB_PMIC_LADD_PARA20_CFG_ADDR(base) ((base) + (0x03E) + PMU_LRA_BASE)
#define SUB_PMIC_OUT_BEMF_VOL_L_ADDR(base) ((base) + (0x03F) + PMU_LRA_BASE)
#define SUB_PMIC_OUT_BEMF_VOL_H_ADDR(base) ((base) + (0x040) + PMU_LRA_BASE)
#define SUB_PMIC_OUT_NML_VBEMF_L_ADDR(base) ((base) + (0x041) + PMU_LRA_BASE)
#define SUB_PMIC_OUT_NML_VBEMF_H_ADDR(base) ((base) + (0x042) + PMU_LRA_BASE)
#define SUB_PMIC_OV_BRK_HPRIOD_ADDR(base) ((base) + (0x043) + PMU_LRA_BASE)
#define SUB_PMIC_BRK_HPRIOD_ADDR(base) ((base) + (0x044) + PMU_LRA_BASE)
#define SUB_PMIC_UREAL_DATA_L_ADDR(base) ((base) + (0x045) + PMU_LRA_BASE)
#define SUB_PMIC_UREAL_DATA_H_ADDR(base) ((base) + (0x046) + PMU_LRA_BASE)
#define SUB_PMIC_ADC_CALI_MAX_L_ADDR(base) ((base) + (0x047) + PMU_LRA_BASE)
#define SUB_PMIC_ADC_CALI_MAX_H_ADDR(base) ((base) + (0x048) + PMU_LRA_BASE)
#define SUB_PMIC_ADC_CALI_MIN_L_ADDR(base) ((base) + (0x049) + PMU_LRA_BASE)
#define SUB_PMIC_ADC_CALI_MIN_H_ADDR(base) ((base) + (0x04A) + PMU_LRA_BASE)
#define SUB_PMIC_ADC_ZERO_L_ADDR(base) ((base) + (0x04B) + PMU_LRA_BASE)
#define SUB_PMIC_ADC_ZERO_H_ADDR(base) ((base) + (0x04C) + PMU_LRA_BASE)
#define SUB_PMIC_KFIT_DATA_L_ADDR(base) ((base) + (0x04D) + PMU_LRA_BASE)
#define SUB_PMIC_KFIT_DATA_H_ADDR(base) ((base) + (0x04E) + PMU_LRA_BASE)
#define SUB_PMIC_FSM_STATE_ADDR(base) ((base) + (0x04F) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_1_ADDR(base) ((base) + (0x050) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_2_ADDR(base) ((base) + (0x051) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_3_ADDR(base) ((base) + (0x052) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_4_ADDR(base) ((base) + (0x053) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_5_ADDR(base) ((base) + (0x054) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_6_ADDR(base) ((base) + (0x055) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_7_ADDR(base) ((base) + (0x056) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_8_ADDR(base) ((base) + (0x057) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_9_ADDR(base) ((base) + (0x058) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_10_ADDR(base) ((base) + (0x059) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_11_ADDR(base) ((base) + (0x05A) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_12_ADDR(base) ((base) + (0x05B) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_13_ADDR(base) ((base) + (0x05C) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_14_ADDR(base) ((base) + (0x05D) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_15_ADDR(base) ((base) + (0x05E) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_16_ADDR(base) ((base) + (0x05F) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_17_ADDR(base) ((base) + (0x060) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_18_ADDR(base) ((base) + (0x061) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_19_ADDR(base) ((base) + (0x062) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_TIME_20_ADDR(base) ((base) + (0x063) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_CPS_ADDR(base) ((base) + (0x064) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_1_L_ADDR(base) ((base) + (0x065) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_1_H_ADDR(base) ((base) + (0x066) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_2_L_ADDR(base) ((base) + (0x067) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_2_H_ADDR(base) ((base) + (0x068) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_3_L_ADDR(base) ((base) + (0x069) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_3_H_ADDR(base) ((base) + (0x06A) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_4_L_ADDR(base) ((base) + (0x06B) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_4_H_ADDR(base) ((base) + (0x06C) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_5_L_ADDR(base) ((base) + (0x06D) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_5_H_ADDR(base) ((base) + (0x06E) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_6_L_ADDR(base) ((base) + (0x06F) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_6_H_ADDR(base) ((base) + (0x070) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_7_L_ADDR(base) ((base) + (0x071) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_7_H_ADDR(base) ((base) + (0x072) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_8_L_ADDR(base) ((base) + (0x073) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_8_H_ADDR(base) ((base) + (0x074) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_9_L_ADDR(base) ((base) + (0x075) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_9_H_ADDR(base) ((base) + (0x076) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_10_L_ADDR(base) ((base) + (0x077) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_10_H_ADDR(base) ((base) + (0x078) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_11_L_ADDR(base) ((base) + (0x079) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_11_H_ADDR(base) ((base) + (0x07A) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_12_L_ADDR(base) ((base) + (0x07B) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_12_H_ADDR(base) ((base) + (0x07C) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_13_L_ADDR(base) ((base) + (0x07D) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_13_H_ADDR(base) ((base) + (0x07E) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_14_L_ADDR(base) ((base) + (0x07F) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_14_H_ADDR(base) ((base) + (0x080) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_15_L_ADDR(base) ((base) + (0x081) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_15_H_ADDR(base) ((base) + (0x082) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_16_L_ADDR(base) ((base) + (0x083) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_16_H_ADDR(base) ((base) + (0x084) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_17_L_ADDR(base) ((base) + (0x085) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_17_H_ADDR(base) ((base) + (0x086) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_18_L_ADDR(base) ((base) + (0x087) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_18_H_ADDR(base) ((base) + (0x088) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_19_L_ADDR(base) ((base) + (0x089) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_19_H_ADDR(base) ((base) + (0x08A) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_20_L_ADDR(base) ((base) + (0x08B) + PMU_LRA_BASE)
#define SUB_PMIC_SPEL_DUTY_20_H_ADDR(base) ((base) + (0x08C) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_ANA_CFG0_ADDR(base) ((base) + (0x08D) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_ANA_CFG1_ADDR(base) ((base) + (0x08E) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_ANA_CFG2_ADDR(base) ((base) + (0x08F) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_ANA_CFG3_ADDR(base) ((base) + (0x090) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_OCP_CFG_ADDR(base) ((base) + (0x091) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_IBIAS_SEL_ADDR(base) ((base) + (0x092) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_OVP_CFG_ADDR(base) ((base) + (0x093) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_ANA_RESERVED0_ADDR(base) ((base) + (0x094) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_ANA_RESERVED1_ADDR(base) ((base) + (0x095) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_SAMPLE_EN_ADDR(base) ((base) + (0x096) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_SAMPLE_VALID_ADDR(base) ((base) + (0x097) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_MAX_L_ADDR(base) ((base) + (0x098) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_MAX_H_ADDR(base) ((base) + (0x099) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_MIN_L_ADDR(base) ((base) + (0x09A) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_MIN_H_ADDR(base) ((base) + (0x09B) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_FIRST_L_ADDR(base) ((base) + (0x09C) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_FIRST_H_ADDR(base) ((base) + (0x09D) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_LAST_L_ADDR(base) ((base) + (0x09E) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_BEMF_LAST_H_ADDR(base) ((base) + (0x09F) + PMU_LRA_BASE)
#define SUB_PMIC_HP_DR_TIME_ADDR(base) ((base) + (0x0A0) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_NEXT_DR_ADDR(base) ((base) + (0x0A1) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_TIME_L_ADDR(base) ((base) + (0x0A2) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_TIME_H_ADDR(base) ((base) + (0x0A3) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_DUTY_NUM_ADDR(base) ((base) + (0x0A4) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_RANK_NUM_L_ADDR(base) ((base) + (0x0A5) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_RANK_NUM_H_ADDR(base) ((base) + (0x0A6) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_OCP_NUM_ADDR(base) ((base) + (0x0A7) + PMU_LRA_BASE)
#define SUB_PMIC_HPRD_FSM_ADDR(base) ((base) + (0x0A8) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_DEBUG_EN_ADDR(base) ((base) + (0x0A9) + PMU_LRA_BASE)
#define SUB_PMIC_DEBUG_ADC_CTRL_ADDR(base) ((base) + (0x0AA) + PMU_LRA_BASE)
#define SUB_PMIC_DEBUG_ADC_VALID_ADDR(base) ((base) + (0x0AB) + PMU_LRA_BASE)
#define SUB_PMIC_DEBUG_ADC_DATA_L_ADDR(base) ((base) + (0x0AC) + PMU_LRA_BASE)
#define SUB_PMIC_DEBUG_ADC_DATA_H_ADDR(base) ((base) + (0x0AD) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_CLK_G_CTRL_ADDR(base) ((base) + (0x0AE) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_NML_TIME_L_ADDR(base) ((base) + (0x0AF) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_NML_TIME_M_ADDR(base) ((base) + (0x0B0) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_NML_TIME_H_ADDR(base) ((base) + (0x0B1) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_NML_BEMF_L_ADDR(base) ((base) + (0x0B2) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_NML_BEMF_H_ADDR(base) ((base) + (0x0B3) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_GATE_CTRL_REG_ADDR(base) ((base) + (0x0B4) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_DUTYRES_REG_ADDR(base) ((base) + (0x0B5) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_BFST_OK_REG_ADDR(base) ((base) + (0x0B6) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_BEMF_FST_REG_LOW_ADDR(base) ((base) + (0x0B7) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_BEMF_FST_REG_HIGH_ADDR(base) ((base) + (0x0B8) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_OFF_L_ADDR(base) ((base) + (0x0B9) + PMU_LRA_BASE)
#define SUB_PMIC_LRA_OFF_H_ADDR(base) ((base) + (0x0BA) + PMU_LRA_BASE)
#define SUB_PMIC_OFFSET_TIME_ADDR(base) ((base) + (0x0BB) + PMU_LRA_BASE)
#define SUB_PMIC_WE_ERM_LOCK_ADDR(base) ((base) + (0x0BC) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_MODE_CFG_ADDR(base) ((base) + (0x0BD) + PMU_LRA_BASE)
#define SUB_PMIC_FRQC_SEL_CFG_ADDR(base) ((base) + (0x0BE) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DUTY_OVDR_CFG_L_ADDR(base) ((base) + (0x0BF) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DUTY_OVDR_CFG_H_ADDR(base) ((base) + (0x0C0) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DUTY_NML_CFG_L_ADDR(base) ((base) + (0x0C1) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DUTY_NML_CFG_H_ADDR(base) ((base) + (0x0C2) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DUTY_BRK_CFG_L_ADDR(base) ((base) + (0x0C3) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DUTY_BRK_CFG_H_ADDR(base) ((base) + (0x0C4) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_K_EOB_L_ADDR(base) ((base) + (0x0C5) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_K_EOB_H_ADDR(base) ((base) + (0x0C6) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_OVDR_NUM_ADDR(base) ((base) + (0x0C7) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_BRK_NUM_ADDR(base) ((base) + (0x0C8) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DRT_CFG_ADDR(base) ((base) + (0x0C9) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_DETE_TIME_CFG_ADDR(base) ((base) + (0x0CA) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_TWAIT_CFG_ADDR(base) ((base) + (0x0CB) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_OVDR_THRE_L_ADDR(base) ((base) + (0x0CC) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_OVDR_THRE_H_ADDR(base) ((base) + (0x0CD) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_BRK_THRE_L_ADDR(base) ((base) + (0x0CE) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_BRK_THRE_H_ADDR(base) ((base) + (0x0CF) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_OPL_OVDR_NUM_ADDR(base) ((base) + (0x0D0) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_OPL_BRK_NUM_ADDR(base) ((base) + (0x0D1) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_ADCAVE_CFG_ADDR(base) ((base) + (0x0D2) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_ABN_ONOFF_ADDR(base) ((base) + (0x0D3) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_ABN_TH_L_ADDR(base) ((base) + (0x0D4) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_ABN_TH_H_ADDR(base) ((base) + (0x0D5) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_OFF_L_ADDR(base) ((base) + (0x0D6) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_OFF_H_ADDR(base) ((base) + (0x0D7) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_ANA_CFG0_ADDR(base) ((base) + (0x0D8) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_PRD_SMPL_EN_ADDR(base) ((base) + (0x0D9) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_PRD_SMPL_VALID_ADDR(base) ((base) + (0x0DA) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_PRD_BEMF_L_ADDR(base) ((base) + (0x0DB) + PMU_LRA_BASE)
#define SUB_PMIC_ERM_PRD_BEMF_H_ADDR(base) ((base) + (0x0DC) + PMU_LRA_BASE)
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char project_num0 : 8;
    } reg;
} SUB_PMIC_CHIP_VERSION_0_UNION;
#endif
#define SUB_PMIC_CHIP_VERSION_0_project_num0_START (0)
#define SUB_PMIC_CHIP_VERSION_0_project_num0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char project_num1 : 8;
    } reg;
} SUB_PMIC_CHIP_VERSION_1_UNION;
#endif
#define SUB_PMIC_CHIP_VERSION_1_project_num1_START (0)
#define SUB_PMIC_CHIP_VERSION_1_project_num1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char project_num2 : 8;
    } reg;
} SUB_PMIC_CHIP_VERSION_2_UNION;
#endif
#define SUB_PMIC_CHIP_VERSION_2_project_num2_START (0)
#define SUB_PMIC_CHIP_VERSION_2_project_num2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char project_num3 : 8;
    } reg;
} SUB_PMIC_CHIP_VERSION_3_UNION;
#endif
#define SUB_PMIC_CHIP_VERSION_3_project_num3_START (0)
#define SUB_PMIC_CHIP_VERSION_3_project_num3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char version : 8;
    } reg;
} SUB_PMIC_CHIP_VERSION_4_UNION;
#endif
#define SUB_PMIC_CHIP_VERSION_4_version_START (0)
#define SUB_PMIC_CHIP_VERSION_4_version_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char chip_id : 8;
    } reg;
} SUB_PMIC_CHIP_VERSION_5_UNION;
#endif
#define SUB_PMIC_CHIP_VERSION_5_chip_id_START (0)
#define SUB_PMIC_CHIP_VERSION_5_chip_id_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_dummy_spmi : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_DUMMY_SPMI_UNION;
#endif
#define SUB_PMIC_DUMMY_SPMI_sc_dummy_spmi_START (0)
#define SUB_PMIC_DUMMY_SPMI_sc_dummy_spmi_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char st_thsd_otmp140_d1mr : 1;
        unsigned char st_thsd_otmp125_d1mr : 1;
        unsigned char st_vsys_pwroff_abs_d20nr : 1;
        unsigned char st_vsys_pwroff_deb_d80mr : 1;
        unsigned char st_vsys_pwron_d60ur : 1;
        unsigned char st_vsys_ov_d200ur : 1;
        unsigned char st_pmu_rc32k_sel : 1;
        unsigned char st_sim_hpd_d540u : 1;
    } reg;
} SUB_PMIC_STATUS0_UNION;
#endif
#define SUB_PMIC_STATUS0_st_thsd_otmp140_d1mr_START (0)
#define SUB_PMIC_STATUS0_st_thsd_otmp140_d1mr_END (0)
#define SUB_PMIC_STATUS0_st_thsd_otmp125_d1mr_START (1)
#define SUB_PMIC_STATUS0_st_thsd_otmp125_d1mr_END (1)
#define SUB_PMIC_STATUS0_st_vsys_pwroff_abs_d20nr_START (2)
#define SUB_PMIC_STATUS0_st_vsys_pwroff_abs_d20nr_END (2)
#define SUB_PMIC_STATUS0_st_vsys_pwroff_deb_d80mr_START (3)
#define SUB_PMIC_STATUS0_st_vsys_pwroff_deb_d80mr_END (3)
#define SUB_PMIC_STATUS0_st_vsys_pwron_d60ur_START (4)
#define SUB_PMIC_STATUS0_st_vsys_pwron_d60ur_END (4)
#define SUB_PMIC_STATUS0_st_vsys_ov_d200ur_START (5)
#define SUB_PMIC_STATUS0_st_vsys_ov_d200ur_END (5)
#define SUB_PMIC_STATUS0_st_pmu_rc32k_sel_START (6)
#define SUB_PMIC_STATUS0_st_pmu_rc32k_sel_END (6)
#define SUB_PMIC_STATUS0_st_sim_hpd_d540u_START (7)
#define SUB_PMIC_STATUS0_st_sim_hpd_d540u_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck13_ocp_masked : 1;
        unsigned char buck12_ocp_masked : 1;
        unsigned char buck8_ocp_masked : 1;
        unsigned char buck7_ocp_masked : 1;
        unsigned char buck6_ocp_masked : 1;
        unsigned char sw10_ocp_masked : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK_OCP_STATUS0_UNION;
#endif
#define SUB_PMIC_BUCK_OCP_STATUS0_buck13_ocp_masked_START (0)
#define SUB_PMIC_BUCK_OCP_STATUS0_buck13_ocp_masked_END (0)
#define SUB_PMIC_BUCK_OCP_STATUS0_buck12_ocp_masked_START (1)
#define SUB_PMIC_BUCK_OCP_STATUS0_buck12_ocp_masked_END (1)
#define SUB_PMIC_BUCK_OCP_STATUS0_buck8_ocp_masked_START (2)
#define SUB_PMIC_BUCK_OCP_STATUS0_buck8_ocp_masked_END (2)
#define SUB_PMIC_BUCK_OCP_STATUS0_buck7_ocp_masked_START (3)
#define SUB_PMIC_BUCK_OCP_STATUS0_buck7_ocp_masked_END (3)
#define SUB_PMIC_BUCK_OCP_STATUS0_buck6_ocp_masked_START (4)
#define SUB_PMIC_BUCK_OCP_STATUS0_buck6_ocp_masked_END (4)
#define SUB_PMIC_BUCK_OCP_STATUS0_sw10_ocp_masked_START (5)
#define SUB_PMIC_BUCK_OCP_STATUS0_sw10_ocp_masked_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck31_ocp_masked : 1;
        unsigned char buck30_ocp_masked : 1;
        unsigned char buck21_ocp_masked : 1;
        unsigned char buck20_ocp_masked : 1;
        unsigned char buck11_ocp_masked : 1;
        unsigned char buck10_ocp_masked : 1;
        unsigned char buck01_ocp_masked : 1;
        unsigned char buck00_ocp_masked : 1;
    } reg;
} SUB_PMIC_BUCK_OCP_STATUS1_UNION;
#endif
#define SUB_PMIC_BUCK_OCP_STATUS1_buck31_ocp_masked_START (0)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck31_ocp_masked_END (0)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck30_ocp_masked_START (1)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck30_ocp_masked_END (1)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck21_ocp_masked_START (2)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck21_ocp_masked_END (2)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck20_ocp_masked_START (3)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck20_ocp_masked_END (3)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck11_ocp_masked_START (4)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck11_ocp_masked_END (4)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck10_ocp_masked_START (5)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck10_ocp_masked_END (5)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck01_ocp_masked_START (6)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck01_ocp_masked_END (6)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck00_ocp_masked_START (7)
#define SUB_PMIC_BUCK_OCP_STATUS1_buck00_ocp_masked_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pmuh_ocp_masked : 1;
        unsigned char ldo54_ocp_masked : 1;
        unsigned char ldo53_ocp_masked : 1;
        unsigned char ldo52_ocp_masked : 1;
        unsigned char ldo51_ocp_masked : 1;
        unsigned char ldo50_ocp_masked : 1;
        unsigned char ldo34_ocp_masked : 1;
        unsigned char ldo9_ocp_masked : 1;
    } reg;
} SUB_PMIC_LDO_OCP_STATUS_UNION;
#endif
#define SUB_PMIC_LDO_OCP_STATUS_pmuh_ocp_masked_START (0)
#define SUB_PMIC_LDO_OCP_STATUS_pmuh_ocp_masked_END (0)
#define SUB_PMIC_LDO_OCP_STATUS_ldo54_ocp_masked_START (1)
#define SUB_PMIC_LDO_OCP_STATUS_ldo54_ocp_masked_END (1)
#define SUB_PMIC_LDO_OCP_STATUS_ldo53_ocp_masked_START (2)
#define SUB_PMIC_LDO_OCP_STATUS_ldo53_ocp_masked_END (2)
#define SUB_PMIC_LDO_OCP_STATUS_ldo52_ocp_masked_START (3)
#define SUB_PMIC_LDO_OCP_STATUS_ldo52_ocp_masked_END (3)
#define SUB_PMIC_LDO_OCP_STATUS_ldo51_ocp_masked_START (4)
#define SUB_PMIC_LDO_OCP_STATUS_ldo51_ocp_masked_END (4)
#define SUB_PMIC_LDO_OCP_STATUS_ldo50_ocp_masked_START (5)
#define SUB_PMIC_LDO_OCP_STATUS_ldo50_ocp_masked_END (5)
#define SUB_PMIC_LDO_OCP_STATUS_ldo34_ocp_masked_START (6)
#define SUB_PMIC_LDO_OCP_STATUS_ldo34_ocp_masked_END (6)
#define SUB_PMIC_LDO_OCP_STATUS_ldo9_ocp_masked_START (7)
#define SUB_PMIC_LDO_OCP_STATUS_ldo9_ocp_masked_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck13_scp_masked : 1;
        unsigned char buck12_scp_masked : 1;
        unsigned char buck8_scp_masked : 1;
        unsigned char buck7_scp_masked : 1;
        unsigned char buck6_scp_masked : 1;
        unsigned char bb_scp_masked : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK_SCP_STATUS0_UNION;
#endif
#define SUB_PMIC_BUCK_SCP_STATUS0_buck13_scp_masked_START (0)
#define SUB_PMIC_BUCK_SCP_STATUS0_buck13_scp_masked_END (0)
#define SUB_PMIC_BUCK_SCP_STATUS0_buck12_scp_masked_START (1)
#define SUB_PMIC_BUCK_SCP_STATUS0_buck12_scp_masked_END (1)
#define SUB_PMIC_BUCK_SCP_STATUS0_buck8_scp_masked_START (2)
#define SUB_PMIC_BUCK_SCP_STATUS0_buck8_scp_masked_END (2)
#define SUB_PMIC_BUCK_SCP_STATUS0_buck7_scp_masked_START (3)
#define SUB_PMIC_BUCK_SCP_STATUS0_buck7_scp_masked_END (3)
#define SUB_PMIC_BUCK_SCP_STATUS0_buck6_scp_masked_START (4)
#define SUB_PMIC_BUCK_SCP_STATUS0_buck6_scp_masked_END (4)
#define SUB_PMIC_BUCK_SCP_STATUS0_bb_scp_masked_START (5)
#define SUB_PMIC_BUCK_SCP_STATUS0_bb_scp_masked_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck30_scp_masked : 1;
        unsigned char buck20_scp_masked : 1;
        unsigned char buck10_scp_masked : 1;
        unsigned char buck00_scp_masked : 1;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_BUCK_SCP_STATUS1_UNION;
#endif
#define SUB_PMIC_BUCK_SCP_STATUS1_buck30_scp_masked_START (0)
#define SUB_PMIC_BUCK_SCP_STATUS1_buck30_scp_masked_END (0)
#define SUB_PMIC_BUCK_SCP_STATUS1_buck20_scp_masked_START (1)
#define SUB_PMIC_BUCK_SCP_STATUS1_buck20_scp_masked_END (1)
#define SUB_PMIC_BUCK_SCP_STATUS1_buck10_scp_masked_START (2)
#define SUB_PMIC_BUCK_SCP_STATUS1_buck10_scp_masked_END (2)
#define SUB_PMIC_BUCK_SCP_STATUS1_buck00_scp_masked_START (3)
#define SUB_PMIC_BUCK_SCP_STATUS1_buck00_scp_masked_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char bb_ovp_masked : 1;
        unsigned char buck12_ovp_masked : 1;
        unsigned char buck8_ovp_masked : 1;
        unsigned char buck30_ovp_masked : 1;
        unsigned char buck20_ovp_masked : 1;
        unsigned char buck10_ovp_masked : 1;
        unsigned char buck00_ovp_masked : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK_OVP_STATUS0_UNION;
#endif
#define SUB_PMIC_BUCK_OVP_STATUS0_bb_ovp_masked_START (0)
#define SUB_PMIC_BUCK_OVP_STATUS0_bb_ovp_masked_END (0)
#define SUB_PMIC_BUCK_OVP_STATUS0_buck12_ovp_masked_START (1)
#define SUB_PMIC_BUCK_OVP_STATUS0_buck12_ovp_masked_END (1)
#define SUB_PMIC_BUCK_OVP_STATUS0_buck8_ovp_masked_START (2)
#define SUB_PMIC_BUCK_OVP_STATUS0_buck8_ovp_masked_END (2)
#define SUB_PMIC_BUCK_OVP_STATUS0_buck30_ovp_masked_START (3)
#define SUB_PMIC_BUCK_OVP_STATUS0_buck30_ovp_masked_END (3)
#define SUB_PMIC_BUCK_OVP_STATUS0_buck20_ovp_masked_START (4)
#define SUB_PMIC_BUCK_OVP_STATUS0_buck20_ovp_masked_END (4)
#define SUB_PMIC_BUCK_OVP_STATUS0_buck10_ovp_masked_START (5)
#define SUB_PMIC_BUCK_OVP_STATUS0_buck10_ovp_masked_END (5)
#define SUB_PMIC_BUCK_OVP_STATUS0_buck00_ovp_masked_START (6)
#define SUB_PMIC_BUCK_OVP_STATUS0_buck00_ovp_masked_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck12_cur_masked : 1;
        unsigned char buck30_cur_masked : 1;
        unsigned char buck20_cur_masked : 1;
        unsigned char buck10_cur_masked : 1;
        unsigned char buck00_cur_masked : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK_CUR_STATUS_UNION;
#endif
#define SUB_PMIC_BUCK_CUR_STATUS_buck12_cur_masked_START (0)
#define SUB_PMIC_BUCK_CUR_STATUS_buck12_cur_masked_END (0)
#define SUB_PMIC_BUCK_CUR_STATUS_buck30_cur_masked_START (1)
#define SUB_PMIC_BUCK_CUR_STATUS_buck30_cur_masked_END (1)
#define SUB_PMIC_BUCK_CUR_STATUS_buck20_cur_masked_START (2)
#define SUB_PMIC_BUCK_CUR_STATUS_buck20_cur_masked_END (2)
#define SUB_PMIC_BUCK_CUR_STATUS_buck10_cur_masked_START (3)
#define SUB_PMIC_BUCK_CUR_STATUS_buck10_cur_masked_END (3)
#define SUB_PMIC_BUCK_CUR_STATUS_buck00_cur_masked_START (4)
#define SUB_PMIC_BUCK_CUR_STATUS_buck00_cur_masked_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char st_pmu_en_a2d : 1;
        unsigned char st_pa2_en_a2d : 1;
        unsigned char st_pa1_en_a2d : 1;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_SYS_CTRL_STATUS_UNION;
#endif
#define SUB_PMIC_SYS_CTRL_STATUS_st_pmu_en_a2d_START (0)
#define SUB_PMIC_SYS_CTRL_STATUS_st_pmu_en_a2d_END (0)
#define SUB_PMIC_SYS_CTRL_STATUS_st_pa2_en_a2d_START (1)
#define SUB_PMIC_SYS_CTRL_STATUS_st_pa2_en_a2d_END (1)
#define SUB_PMIC_SYS_CTRL_STATUS_st_pa1_en_a2d_START (2)
#define SUB_PMIC_SYS_CTRL_STATUS_st_pa1_en_a2d_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lock : 8;
    } reg;
} SUB_PMIC_LOCK_UNION;
#endif
#define SUB_PMIC_LOCK_lock_START (0)
#define SUB_PMIC_LOCK_lock_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck00_en : 1;
        unsigned char st_buck00_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck00_eco_en : 1;
        unsigned char st_buck00_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK00_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_BUCK00_ONOFF_ECO_reg_buck00_en_START (0)
#define SUB_PMIC_BUCK00_ONOFF_ECO_reg_buck00_en_END (0)
#define SUB_PMIC_BUCK00_ONOFF_ECO_st_buck00_en_START (1)
#define SUB_PMIC_BUCK00_ONOFF_ECO_st_buck00_en_END (1)
#define SUB_PMIC_BUCK00_ONOFF_ECO_reg_buck00_eco_en_START (4)
#define SUB_PMIC_BUCK00_ONOFF_ECO_reg_buck00_eco_en_END (4)
#define SUB_PMIC_BUCK00_ONOFF_ECO_st_buck00_eco_en_START (5)
#define SUB_PMIC_BUCK00_ONOFF_ECO_st_buck00_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck30_en : 1;
        unsigned char st_buck30_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck30_eco_en : 1;
        unsigned char st_buck30_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK30_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_BUCK30_ONOFF_ECO_reg_buck30_en_START (0)
#define SUB_PMIC_BUCK30_ONOFF_ECO_reg_buck30_en_END (0)
#define SUB_PMIC_BUCK30_ONOFF_ECO_st_buck30_en_START (1)
#define SUB_PMIC_BUCK30_ONOFF_ECO_st_buck30_en_END (1)
#define SUB_PMIC_BUCK30_ONOFF_ECO_reg_buck30_eco_en_START (4)
#define SUB_PMIC_BUCK30_ONOFF_ECO_reg_buck30_eco_en_END (4)
#define SUB_PMIC_BUCK30_ONOFF_ECO_st_buck30_eco_en_START (5)
#define SUB_PMIC_BUCK30_ONOFF_ECO_st_buck30_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck6_en : 1;
        unsigned char st_buck6_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck6_eco_en : 1;
        unsigned char st_buck6_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK6_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_BUCK6_ONOFF_ECO_reg_buck6_en_START (0)
#define SUB_PMIC_BUCK6_ONOFF_ECO_reg_buck6_en_END (0)
#define SUB_PMIC_BUCK6_ONOFF_ECO_st_buck6_en_START (1)
#define SUB_PMIC_BUCK6_ONOFF_ECO_st_buck6_en_END (1)
#define SUB_PMIC_BUCK6_ONOFF_ECO_reg_buck6_eco_en_START (4)
#define SUB_PMIC_BUCK6_ONOFF_ECO_reg_buck6_eco_en_END (4)
#define SUB_PMIC_BUCK6_ONOFF_ECO_st_buck6_eco_en_START (5)
#define SUB_PMIC_BUCK6_ONOFF_ECO_st_buck6_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck7_en : 1;
        unsigned char st_buck7_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck7_eco_en : 1;
        unsigned char st_buck7_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK7_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_BUCK7_ONOFF_ECO_reg_buck7_en_START (0)
#define SUB_PMIC_BUCK7_ONOFF_ECO_reg_buck7_en_END (0)
#define SUB_PMIC_BUCK7_ONOFF_ECO_st_buck7_en_START (1)
#define SUB_PMIC_BUCK7_ONOFF_ECO_st_buck7_en_END (1)
#define SUB_PMIC_BUCK7_ONOFF_ECO_reg_buck7_eco_en_START (4)
#define SUB_PMIC_BUCK7_ONOFF_ECO_reg_buck7_eco_en_END (4)
#define SUB_PMIC_BUCK7_ONOFF_ECO_st_buck7_eco_en_START (5)
#define SUB_PMIC_BUCK7_ONOFF_ECO_st_buck7_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck8_en : 1;
        unsigned char st_buck8_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck8_eco_en : 1;
        unsigned char st_buck8_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK8_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_BUCK8_ONOFF_ECO_reg_buck8_en_START (0)
#define SUB_PMIC_BUCK8_ONOFF_ECO_reg_buck8_en_END (0)
#define SUB_PMIC_BUCK8_ONOFF_ECO_st_buck8_en_START (1)
#define SUB_PMIC_BUCK8_ONOFF_ECO_st_buck8_en_END (1)
#define SUB_PMIC_BUCK8_ONOFF_ECO_reg_buck8_eco_en_START (4)
#define SUB_PMIC_BUCK8_ONOFF_ECO_reg_buck8_eco_en_END (4)
#define SUB_PMIC_BUCK8_ONOFF_ECO_st_buck8_eco_en_START (5)
#define SUB_PMIC_BUCK8_ONOFF_ECO_st_buck8_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck13_en : 1;
        unsigned char st_buck13_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck13_eco_en : 1;
        unsigned char st_buck13_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK13_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_BUCK13_ONOFF_ECO_reg_buck13_en_START (0)
#define SUB_PMIC_BUCK13_ONOFF_ECO_reg_buck13_en_END (0)
#define SUB_PMIC_BUCK13_ONOFF_ECO_st_buck13_en_START (1)
#define SUB_PMIC_BUCK13_ONOFF_ECO_st_buck13_en_END (1)
#define SUB_PMIC_BUCK13_ONOFF_ECO_reg_buck13_eco_en_START (4)
#define SUB_PMIC_BUCK13_ONOFF_ECO_reg_buck13_eco_en_END (4)
#define SUB_PMIC_BUCK13_ONOFF_ECO_st_buck13_eco_en_START (5)
#define SUB_PMIC_BUCK13_ONOFF_ECO_st_buck13_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo9_en : 1;
        unsigned char st_ldo9_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo9_eco_en : 1;
        unsigned char st_ldo9_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_LDO9_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_LDO9_ONOFF_ECO_reg_ldo9_en_START (0)
#define SUB_PMIC_LDO9_ONOFF_ECO_reg_ldo9_en_END (0)
#define SUB_PMIC_LDO9_ONOFF_ECO_st_ldo9_en_START (1)
#define SUB_PMIC_LDO9_ONOFF_ECO_st_ldo9_en_END (1)
#define SUB_PMIC_LDO9_ONOFF_ECO_reg_ldo9_eco_en_START (4)
#define SUB_PMIC_LDO9_ONOFF_ECO_reg_ldo9_eco_en_END (4)
#define SUB_PMIC_LDO9_ONOFF_ECO_st_ldo9_eco_en_START (5)
#define SUB_PMIC_LDO9_ONOFF_ECO_st_ldo9_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo34_en : 1;
        unsigned char st_ldo34_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo34_eco_en : 1;
        unsigned char st_ldo34_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_LDO34_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_LDO34_ONOFF_ECO_reg_ldo34_en_START (0)
#define SUB_PMIC_LDO34_ONOFF_ECO_reg_ldo34_en_END (0)
#define SUB_PMIC_LDO34_ONOFF_ECO_st_ldo34_en_START (1)
#define SUB_PMIC_LDO34_ONOFF_ECO_st_ldo34_en_END (1)
#define SUB_PMIC_LDO34_ONOFF_ECO_reg_ldo34_eco_en_START (4)
#define SUB_PMIC_LDO34_ONOFF_ECO_reg_ldo34_eco_en_END (4)
#define SUB_PMIC_LDO34_ONOFF_ECO_st_ldo34_eco_en_START (5)
#define SUB_PMIC_LDO34_ONOFF_ECO_st_ldo34_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo50_en : 1;
        unsigned char st_ldo50_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo50_eco_en : 1;
        unsigned char st_ldo50_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_LDO50_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_LDO50_ONOFF_ECO_reg_ldo50_en_START (0)
#define SUB_PMIC_LDO50_ONOFF_ECO_reg_ldo50_en_END (0)
#define SUB_PMIC_LDO50_ONOFF_ECO_st_ldo50_en_START (1)
#define SUB_PMIC_LDO50_ONOFF_ECO_st_ldo50_en_END (1)
#define SUB_PMIC_LDO50_ONOFF_ECO_reg_ldo50_eco_en_START (4)
#define SUB_PMIC_LDO50_ONOFF_ECO_reg_ldo50_eco_en_END (4)
#define SUB_PMIC_LDO50_ONOFF_ECO_st_ldo50_eco_en_START (5)
#define SUB_PMIC_LDO50_ONOFF_ECO_st_ldo50_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo51_en : 1;
        unsigned char st_ldo51_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo51_eco_en : 1;
        unsigned char st_ldo51_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_LDO51_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_LDO51_ONOFF_ECO_reg_ldo51_en_START (0)
#define SUB_PMIC_LDO51_ONOFF_ECO_reg_ldo51_en_END (0)
#define SUB_PMIC_LDO51_ONOFF_ECO_st_ldo51_en_START (1)
#define SUB_PMIC_LDO51_ONOFF_ECO_st_ldo51_en_END (1)
#define SUB_PMIC_LDO51_ONOFF_ECO_reg_ldo51_eco_en_START (4)
#define SUB_PMIC_LDO51_ONOFF_ECO_reg_ldo51_eco_en_END (4)
#define SUB_PMIC_LDO51_ONOFF_ECO_st_ldo51_eco_en_START (5)
#define SUB_PMIC_LDO51_ONOFF_ECO_st_ldo51_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo53_en : 1;
        unsigned char st_ldo53_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo53_eco_en : 1;
        unsigned char st_ldo53_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_LDO53_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_LDO53_ONOFF_ECO_reg_ldo53_en_START (0)
#define SUB_PMIC_LDO53_ONOFF_ECO_reg_ldo53_en_END (0)
#define SUB_PMIC_LDO53_ONOFF_ECO_st_ldo53_en_START (1)
#define SUB_PMIC_LDO53_ONOFF_ECO_st_ldo53_en_END (1)
#define SUB_PMIC_LDO53_ONOFF_ECO_reg_ldo53_eco_en_START (4)
#define SUB_PMIC_LDO53_ONOFF_ECO_reg_ldo53_eco_en_END (4)
#define SUB_PMIC_LDO53_ONOFF_ECO_st_ldo53_eco_en_START (5)
#define SUB_PMIC_LDO53_ONOFF_ECO_st_ldo53_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo54_en : 1;
        unsigned char st_ldo54_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo54_eco_en : 1;
        unsigned char st_ldo54_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_LDO54_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_LDO54_ONOFF_ECO_reg_ldo54_en_START (0)
#define SUB_PMIC_LDO54_ONOFF_ECO_reg_ldo54_en_END (0)
#define SUB_PMIC_LDO54_ONOFF_ECO_st_ldo54_en_START (1)
#define SUB_PMIC_LDO54_ONOFF_ECO_st_ldo54_en_END (1)
#define SUB_PMIC_LDO54_ONOFF_ECO_reg_ldo54_eco_en_START (4)
#define SUB_PMIC_LDO54_ONOFF_ECO_reg_ldo54_eco_en_END (4)
#define SUB_PMIC_LDO54_ONOFF_ECO_st_ldo54_eco_en_START (5)
#define SUB_PMIC_LDO54_ONOFF_ECO_st_ldo54_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_pmuh_en : 1;
        unsigned char st_pmuh_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_pmuh_eco_en : 1;
        unsigned char st_pmuh_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_PMUH_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_PMUH_ONOFF_ECO_reg_pmuh_en_START (0)
#define SUB_PMIC_PMUH_ONOFF_ECO_reg_pmuh_en_END (0)
#define SUB_PMIC_PMUH_ONOFF_ECO_st_pmuh_en_START (1)
#define SUB_PMIC_PMUH_ONOFF_ECO_st_pmuh_en_END (1)
#define SUB_PMIC_PMUH_ONOFF_ECO_reg_pmuh_eco_en_START (4)
#define SUB_PMIC_PMUH_ONOFF_ECO_reg_pmuh_eco_en_END (4)
#define SUB_PMIC_PMUH_ONOFF_ECO_st_pmuh_eco_en_START (5)
#define SUB_PMIC_PMUH_ONOFF_ECO_st_pmuh_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char bb_en_d2a : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_BUCKBOOST_EN_UNION;
#endif
#define SUB_PMIC_BUCKBOOST_EN_bb_en_d2a_START (0)
#define SUB_PMIC_BUCKBOOST_EN_bb_en_d2a_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_thsd_en : 1;
        unsigned char st_thsd_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_thsd_eco_en : 1;
        unsigned char st_thsd_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BG_THSD_EN_UNION;
#endif
#define SUB_PMIC_BG_THSD_EN_reg_thsd_en_START (0)
#define SUB_PMIC_BG_THSD_EN_reg_thsd_en_END (0)
#define SUB_PMIC_BG_THSD_EN_st_thsd_en_START (1)
#define SUB_PMIC_BG_THSD_EN_st_thsd_en_END (1)
#define SUB_PMIC_BG_THSD_EN_reg_thsd_eco_en_START (4)
#define SUB_PMIC_BG_THSD_EN_reg_thsd_eco_en_END (4)
#define SUB_PMIC_BG_THSD_EN_st_thsd_eco_en_START (5)
#define SUB_PMIC_BG_THSD_EN_st_thsd_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char st_ref_en : 1;
        unsigned char reserved_0 : 3;
        unsigned char reg_ref_eco_en : 1;
        unsigned char st_ref_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BG_REF_EN_UNION;
#endif
#define SUB_PMIC_BG_REF_EN_st_ref_en_START (0)
#define SUB_PMIC_BG_REF_EN_st_ref_en_END (0)
#define SUB_PMIC_BG_REF_EN_reg_ref_eco_en_START (4)
#define SUB_PMIC_BG_REF_EN_reg_ref_eco_en_END (4)
#define SUB_PMIC_BG_REF_EN_st_ref_eco_en_START (5)
#define SUB_PMIC_BG_REF_EN_st_ref_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo51_en_sel : 1;
        unsigned char reserved_0 : 3;
        unsigned char ldo50_en_sel : 1;
        unsigned char reserved_1 : 3;
    } reg;
} SUB_PMIC_PALDO_EN_SEL_UNION;
#endif
#define SUB_PMIC_PALDO_EN_SEL_ldo51_en_sel_START (0)
#define SUB_PMIC_PALDO_EN_SEL_ldo51_en_sel_END (0)
#define SUB_PMIC_PALDO_EN_SEL_ldo50_en_sel_START (4)
#define SUB_PMIC_PALDO_EN_SEL_ldo50_en_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ref_res_sel : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_BG_REF_CTRL_UNION;
#endif
#define SUB_PMIC_BG_REF_CTRL_ref_res_sel_START (0)
#define SUB_PMIC_BG_REF_CTRL_ref_res_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck00_vset_cfg : 8;
    } reg;
} SUB_PMIC_BUCK00_VSET_UNION;
#endif
#define SUB_PMIC_BUCK00_VSET_buck00_vset_cfg_START (0)
#define SUB_PMIC_BUCK00_VSET_buck00_vset_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck00_vset_eco : 8;
    } reg;
} SUB_PMIC_BUCK00_VSET_ECO_UNION;
#endif
#define SUB_PMIC_BUCK00_VSET_ECO_buck00_vset_eco_START (0)
#define SUB_PMIC_BUCK00_VSET_ECO_buck00_vset_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck30_vset_cfg : 8;
    } reg;
} SUB_PMIC_BUCK30_VSET_UNION;
#endif
#define SUB_PMIC_BUCK30_VSET_buck30_vset_cfg_START (0)
#define SUB_PMIC_BUCK30_VSET_buck30_vset_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck6_vset_cfg : 8;
    } reg;
} SUB_PMIC_BUCK6_VSET_UNION;
#endif
#define SUB_PMIC_BUCK6_VSET_buck6_vset_cfg_START (0)
#define SUB_PMIC_BUCK6_VSET_buck6_vset_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck7_vset_cfg : 8;
    } reg;
} SUB_PMIC_BUCK7_VSET_UNION;
#endif
#define SUB_PMIC_BUCK7_VSET_buck7_vset_cfg_START (0)
#define SUB_PMIC_BUCK7_VSET_buck7_vset_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck8_vset : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK8_VSET_UNION;
#endif
#define SUB_PMIC_BUCK8_VSET_buck8_vset_START (0)
#define SUB_PMIC_BUCK8_VSET_buck8_vset_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck13_vset_cfg : 8;
    } reg;
} SUB_PMIC_BUCK13_VSET_UNION;
#endif
#define SUB_PMIC_BUCK13_VSET_buck13_vset_cfg_START (0)
#define SUB_PMIC_BUCK13_VSET_buck13_vset_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck13_vset_eco : 8;
    } reg;
} SUB_PMIC_BUCK13_VSET_ECO_UNION;
#endif
#define SUB_PMIC_BUCK13_VSET_ECO_buck13_vset_eco_START (0)
#define SUB_PMIC_BUCK13_VSET_ECO_buck13_vset_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo9_vset : 6;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_LDO9_VSET_UNION;
#endif
#define SUB_PMIC_LDO9_VSET_ldo9_vset_START (0)
#define SUB_PMIC_LDO9_VSET_ldo9_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo34_vset : 6;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_LDO34_VSET_UNION;
#endif
#define SUB_PMIC_LDO34_VSET_ldo34_vset_START (0)
#define SUB_PMIC_LDO34_VSET_ldo34_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo50_vset_cfg : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_LDO50_VSET_UNION;
#endif
#define SUB_PMIC_LDO50_VSET_ldo50_vset_cfg_START (0)
#define SUB_PMIC_LDO50_VSET_ldo50_vset_cfg_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo51_vset_cfg : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_LDO51_VSET_UNION;
#endif
#define SUB_PMIC_LDO51_VSET_ldo51_vset_cfg_START (0)
#define SUB_PMIC_LDO51_VSET_ldo51_vset_cfg_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo53_vset : 6;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_LDO53_VSET_UNION;
#endif
#define SUB_PMIC_LDO53_VSET_ldo53_vset_START (0)
#define SUB_PMIC_LDO53_VSET_ldo53_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo54_vset : 6;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_LDO54_VSET_UNION;
#endif
#define SUB_PMIC_LDO54_VSET_ldo54_vset_START (0)
#define SUB_PMIC_LDO54_VSET_ldo54_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pmuh_vset_cfg : 3;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_PMUH_VSET_UNION;
#endif
#define SUB_PMIC_PMUH_VSET_pmuh_vset_cfg_START (0)
#define SUB_PMIC_PMUH_VSET_pmuh_vset_cfg_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ao_mode : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_AO_MODE_UNION;
#endif
#define SUB_PMIC_AO_MODE_ao_mode_START (0)
#define SUB_PMIC_AO_MODE_ao_mode_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pmu_idle_en : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_PMU_IDLE_EN_UNION;
#endif
#define SUB_PMIC_PMU_IDLE_EN_pmu_idle_en_START (0)
#define SUB_PMIC_PMU_IDLE_EN_pmu_idle_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char fault_m_n_mask : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_FAULT_M_N_MASK_UNION;
#endif
#define SUB_PMIC_FAULT_M_N_MASK_fault_m_n_mask_START (0)
#define SUB_PMIC_FAULT_M_N_MASK_fault_m_n_mask_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char osc_9m6_eco_en : 8;
    } reg;
} SUB_PMIC_OSC_9M6_ECO_EN_UNION;
#endif
#define SUB_PMIC_OSC_9M6_ECO_EN_osc_9m6_eco_en_START (0)
#define SUB_PMIC_OSC_9M6_ECO_EN_osc_9m6_eco_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char osc_9m6_en_bypass : 8;
    } reg;
} SUB_PMIC_OSC_9M6_EN_BYPASS_UNION;
#endif
#define SUB_PMIC_OSC_9M6_EN_BYPASS_osc_9m6_en_bypass_START (0)
#define SUB_PMIC_OSC_9M6_EN_BYPASS_osc_9m6_en_bypass_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_buck13_vset : 1;
        unsigned char peri_en_buck00_vset : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_PERI_VSET_CTRL0_UNION;
#endif
#define SUB_PMIC_PERI_VSET_CTRL0_peri_en_buck13_vset_START (0)
#define SUB_PMIC_PERI_VSET_CTRL0_peri_en_buck13_vset_END (0)
#define SUB_PMIC_PERI_VSET_CTRL0_peri_en_buck00_vset_START (1)
#define SUB_PMIC_PERI_VSET_CTRL0_peri_en_buck00_vset_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_clk_en_buck13_vset : 1;
        unsigned char sys_clk_en_buck00_vset : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_SYS_CLK_EN_VSET_CTRL_UNION;
#endif
#define SUB_PMIC_SYS_CLK_EN_VSET_CTRL_sys_clk_en_buck13_vset_START (0)
#define SUB_PMIC_SYS_CLK_EN_VSET_CTRL_sys_clk_en_buck13_vset_END (0)
#define SUB_PMIC_SYS_CLK_EN_VSET_CTRL_sys_clk_en_buck00_vset_START (1)
#define SUB_PMIC_SYS_CLK_EN_VSET_CTRL_sys_clk_en_buck00_vset_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eco_in_hd_mask : 1;
        unsigned char reg_eco_in_ao_mask : 1;
        unsigned char reg_idle_hd_mask : 1;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_HARDWIRE_CTRL0_UNION;
#endif
#define SUB_PMIC_HARDWIRE_CTRL0_reg_eco_in_hd_mask_START (0)
#define SUB_PMIC_HARDWIRE_CTRL0_reg_eco_in_hd_mask_END (0)
#define SUB_PMIC_HARDWIRE_CTRL0_reg_eco_in_ao_mask_START (1)
#define SUB_PMIC_HARDWIRE_CTRL0_reg_eco_in_ao_mask_END (1)
#define SUB_PMIC_HARDWIRE_CTRL0_reg_idle_hd_mask_START (2)
#define SUB_PMIC_HARDWIRE_CTRL0_reg_idle_hd_mask_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_buck8_on : 1;
        unsigned char peri_en_buck7_on : 1;
        unsigned char peri_en_buck6_on : 1;
        unsigned char reserved : 1;
        unsigned char peri_en_buck30_on : 1;
        unsigned char peri_en_buck20_on : 1;
        unsigned char peri_en_buck10_on : 1;
        unsigned char peri_en_buck00_on : 1;
    } reg;
} SUB_PMIC_PERI_ONOFF_CTRL0_UNION;
#endif
#define SUB_PMIC_PERI_ONOFF_CTRL0_peri_en_buck8_on_START (0)
#define SUB_PMIC_PERI_ONOFF_CTRL0_peri_en_buck8_on_END (0)
#define SUB_PMIC_PERI_ONOFF_CTRL0_peri_en_buck7_on_START (1)
#define SUB_PMIC_PERI_ONOFF_CTRL0_peri_en_buck7_on_END (1)
#define SUB_PMIC_PERI_ONOFF_CTRL0_peri_en_buck6_on_START (2)
#define SUB_PMIC_PERI_ONOFF_CTRL0_peri_en_buck6_on_END (2)
#define SUB_PMIC_PERI_ONOFF_CTRL0_peri_en_buck30_on_START (4)
#define SUB_PMIC_PERI_ONOFF_CTRL0_peri_en_buck30_on_END (4)
#define SUB_PMIC_PERI_ONOFF_CTRL0_peri_en_buck20_on_START (5)
#define SUB_PMIC_PERI_ONOFF_CTRL0_peri_en_buck20_on_END (5)
#define SUB_PMIC_PERI_ONOFF_CTRL0_peri_en_buck10_on_START (6)
#define SUB_PMIC_PERI_ONOFF_CTRL0_peri_en_buck10_on_END (6)
#define SUB_PMIC_PERI_ONOFF_CTRL0_peri_en_buck00_on_START (7)
#define SUB_PMIC_PERI_ONOFF_CTRL0_peri_en_buck00_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_ldo34_on : 1;
        unsigned char peri_en_ldo9_on : 1;
        unsigned char peri_en_buck13_on : 1;
        unsigned char peri_en_buck12_on : 1;
        unsigned char peri_en_pmuh_on : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_PERI_ONOFF_CTRL1_UNION;
#endif
#define SUB_PMIC_PERI_ONOFF_CTRL1_peri_en_ldo34_on_START (0)
#define SUB_PMIC_PERI_ONOFF_CTRL1_peri_en_ldo34_on_END (0)
#define SUB_PMIC_PERI_ONOFF_CTRL1_peri_en_ldo9_on_START (1)
#define SUB_PMIC_PERI_ONOFF_CTRL1_peri_en_ldo9_on_END (1)
#define SUB_PMIC_PERI_ONOFF_CTRL1_peri_en_buck13_on_START (2)
#define SUB_PMIC_PERI_ONOFF_CTRL1_peri_en_buck13_on_END (2)
#define SUB_PMIC_PERI_ONOFF_CTRL1_peri_en_buck12_on_START (3)
#define SUB_PMIC_PERI_ONOFF_CTRL1_peri_en_buck12_on_END (3)
#define SUB_PMIC_PERI_ONOFF_CTRL1_peri_en_pmuh_on_START (4)
#define SUB_PMIC_PERI_ONOFF_CTRL1_peri_en_pmuh_on_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_buck8_eco : 1;
        unsigned char peri_en_buck7_eco : 1;
        unsigned char peri_en_buck6_eco : 1;
        unsigned char reserved : 1;
        unsigned char peri_en_buck30_eco : 1;
        unsigned char peri_en_buck20_eco : 1;
        unsigned char peri_en_buck10_eco : 1;
        unsigned char peri_en_buck00_eco : 1;
    } reg;
} SUB_PMIC_PERI_ECO_CTRL0_UNION;
#endif
#define SUB_PMIC_PERI_ECO_CTRL0_peri_en_buck8_eco_START (0)
#define SUB_PMIC_PERI_ECO_CTRL0_peri_en_buck8_eco_END (0)
#define SUB_PMIC_PERI_ECO_CTRL0_peri_en_buck7_eco_START (1)
#define SUB_PMIC_PERI_ECO_CTRL0_peri_en_buck7_eco_END (1)
#define SUB_PMIC_PERI_ECO_CTRL0_peri_en_buck6_eco_START (2)
#define SUB_PMIC_PERI_ECO_CTRL0_peri_en_buck6_eco_END (2)
#define SUB_PMIC_PERI_ECO_CTRL0_peri_en_buck30_eco_START (4)
#define SUB_PMIC_PERI_ECO_CTRL0_peri_en_buck30_eco_END (4)
#define SUB_PMIC_PERI_ECO_CTRL0_peri_en_buck20_eco_START (5)
#define SUB_PMIC_PERI_ECO_CTRL0_peri_en_buck20_eco_END (5)
#define SUB_PMIC_PERI_ECO_CTRL0_peri_en_buck10_eco_START (6)
#define SUB_PMIC_PERI_ECO_CTRL0_peri_en_buck10_eco_END (6)
#define SUB_PMIC_PERI_ECO_CTRL0_peri_en_buck00_eco_START (7)
#define SUB_PMIC_PERI_ECO_CTRL0_peri_en_buck00_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_ldo34_eco : 1;
        unsigned char peri_en_ldo9_eco : 1;
        unsigned char peri_en_ldo54_eco : 1;
        unsigned char peri_en_ldo53_eco : 1;
        unsigned char peri_en_buck13_eco : 1;
        unsigned char peri_en_buck12_eco : 1;
        unsigned char peri_en_pmuh_eco : 1;
        unsigned char peri_en_ref_eco : 1;
    } reg;
} SUB_PMIC_PERI_ECO_CTRL1_UNION;
#endif
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_ldo34_eco_START (0)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_ldo34_eco_END (0)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_ldo9_eco_START (1)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_ldo9_eco_END (1)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_ldo54_eco_START (2)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_ldo54_eco_END (2)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_ldo53_eco_START (3)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_ldo53_eco_END (3)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_buck13_eco_START (4)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_buck13_eco_END (4)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_buck12_eco_START (5)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_buck12_eco_END (5)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_pmuh_eco_START (6)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_pmuh_eco_END (6)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_ref_eco_START (7)
#define SUB_PMIC_PERI_ECO_CTRL1_peri_en_ref_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck00_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_PERI_PLACE_CTRL0_UNION;
#endif
#define SUB_PMIC_PERI_PLACE_CTRL0_peri_buck00_sel_START (0)
#define SUB_PMIC_PERI_PLACE_CTRL0_peri_buck00_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck10_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_PERI_PLACE_CTRL1_UNION;
#endif
#define SUB_PMIC_PERI_PLACE_CTRL1_peri_buck10_sel_START (0)
#define SUB_PMIC_PERI_PLACE_CTRL1_peri_buck10_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck20_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_PERI_PLACE_CTRL2_UNION;
#endif
#define SUB_PMIC_PERI_PLACE_CTRL2_peri_buck20_sel_START (0)
#define SUB_PMIC_PERI_PLACE_CTRL2_peri_buck20_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck30_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_PERI_PLACE_CTRL3_UNION;
#endif
#define SUB_PMIC_PERI_PLACE_CTRL3_peri_buck30_sel_START (0)
#define SUB_PMIC_PERI_PLACE_CTRL3_peri_buck30_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck6_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_PERI_PLACE_CTRL5_UNION;
#endif
#define SUB_PMIC_PERI_PLACE_CTRL5_peri_buck6_sel_START (0)
#define SUB_PMIC_PERI_PLACE_CTRL5_peri_buck6_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck7_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_PERI_PLACE_CTRL6_UNION;
#endif
#define SUB_PMIC_PERI_PLACE_CTRL6_peri_buck7_sel_START (0)
#define SUB_PMIC_PERI_PLACE_CTRL6_peri_buck7_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck8_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_PERI_PLACE_CTRL7_UNION;
#endif
#define SUB_PMIC_PERI_PLACE_CTRL7_peri_buck8_sel_START (0)
#define SUB_PMIC_PERI_PLACE_CTRL7_peri_buck8_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck12_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_PERI_PLACE_CTRL8_UNION;
#endif
#define SUB_PMIC_PERI_PLACE_CTRL8_peri_buck12_sel_START (0)
#define SUB_PMIC_PERI_PLACE_CTRL8_peri_buck12_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck13_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_PERI_PLACE_CTRL9_UNION;
#endif
#define SUB_PMIC_PERI_PLACE_CTRL9_peri_buck13_sel_START (0)
#define SUB_PMIC_PERI_PLACE_CTRL9_peri_buck13_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo9_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_PERI_PLACE_CTRL10_UNION;
#endif
#define SUB_PMIC_PERI_PLACE_CTRL10_peri_ldo9_sel_START (0)
#define SUB_PMIC_PERI_PLACE_CTRL10_peri_ldo9_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo34_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_PERI_PLACE_CTRL11_UNION;
#endif
#define SUB_PMIC_PERI_PLACE_CTRL11_peri_ldo34_sel_START (0)
#define SUB_PMIC_PERI_PLACE_CTRL11_peri_ldo34_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_pmuh_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_PERI_PLACE_CTRL12_UNION;
#endif
#define SUB_PMIC_PERI_PLACE_CTRL12_peri_pmuh_sel_START (0)
#define SUB_PMIC_PERI_PLACE_CTRL12_peri_pmuh_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_cnt_cfg : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_PERI_CNT_CFG_UNION;
#endif
#define SUB_PMIC_PERI_CNT_CFG_peri_cnt_cfg_START (0)
#define SUB_PMIC_PERI_CNT_CFG_peri_cnt_cfg_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_thsd_eco : 1;
        unsigned char idle_en_thsd_eco : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_THSD_ECO_CTRL_UNION;
#endif
#define SUB_PMIC_THSD_ECO_CTRL_peri_en_thsd_eco_START (0)
#define SUB_PMIC_THSD_ECO_CTRL_peri_en_thsd_eco_END (0)
#define SUB_PMIC_THSD_ECO_CTRL_idle_en_thsd_eco_START (1)
#define SUB_PMIC_THSD_ECO_CTRL_idle_en_thsd_eco_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_en_buck8_on : 1;
        unsigned char idle_en_buck7_on : 1;
        unsigned char idle_en_buck6_on : 1;
        unsigned char reserved : 1;
        unsigned char idle_en_buck30_on : 1;
        unsigned char idle_en_buck20_on : 1;
        unsigned char idle_en_buck10_on : 1;
        unsigned char idle_en_buck00_on : 1;
    } reg;
} SUB_PMIC_IDLE_ONOFF_CTRL0_UNION;
#endif
#define SUB_PMIC_IDLE_ONOFF_CTRL0_idle_en_buck8_on_START (0)
#define SUB_PMIC_IDLE_ONOFF_CTRL0_idle_en_buck8_on_END (0)
#define SUB_PMIC_IDLE_ONOFF_CTRL0_idle_en_buck7_on_START (1)
#define SUB_PMIC_IDLE_ONOFF_CTRL0_idle_en_buck7_on_END (1)
#define SUB_PMIC_IDLE_ONOFF_CTRL0_idle_en_buck6_on_START (2)
#define SUB_PMIC_IDLE_ONOFF_CTRL0_idle_en_buck6_on_END (2)
#define SUB_PMIC_IDLE_ONOFF_CTRL0_idle_en_buck30_on_START (4)
#define SUB_PMIC_IDLE_ONOFF_CTRL0_idle_en_buck30_on_END (4)
#define SUB_PMIC_IDLE_ONOFF_CTRL0_idle_en_buck20_on_START (5)
#define SUB_PMIC_IDLE_ONOFF_CTRL0_idle_en_buck20_on_END (5)
#define SUB_PMIC_IDLE_ONOFF_CTRL0_idle_en_buck10_on_START (6)
#define SUB_PMIC_IDLE_ONOFF_CTRL0_idle_en_buck10_on_END (6)
#define SUB_PMIC_IDLE_ONOFF_CTRL0_idle_en_buck00_on_START (7)
#define SUB_PMIC_IDLE_ONOFF_CTRL0_idle_en_buck00_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_en_ldo34_on : 1;
        unsigned char idle_en_ldo9_on : 1;
        unsigned char reserved_0 : 2;
        unsigned char idle_en_buck13_on : 1;
        unsigned char idle_en_buck12_on : 1;
        unsigned char idle_en_pmuh_on : 1;
        unsigned char reserved_1 : 1;
    } reg;
} SUB_PMIC_IDLE_ONOFF_CTRL1_UNION;
#endif
#define SUB_PMIC_IDLE_ONOFF_CTRL1_idle_en_ldo34_on_START (0)
#define SUB_PMIC_IDLE_ONOFF_CTRL1_idle_en_ldo34_on_END (0)
#define SUB_PMIC_IDLE_ONOFF_CTRL1_idle_en_ldo9_on_START (1)
#define SUB_PMIC_IDLE_ONOFF_CTRL1_idle_en_ldo9_on_END (1)
#define SUB_PMIC_IDLE_ONOFF_CTRL1_idle_en_buck13_on_START (4)
#define SUB_PMIC_IDLE_ONOFF_CTRL1_idle_en_buck13_on_END (4)
#define SUB_PMIC_IDLE_ONOFF_CTRL1_idle_en_buck12_on_START (5)
#define SUB_PMIC_IDLE_ONOFF_CTRL1_idle_en_buck12_on_END (5)
#define SUB_PMIC_IDLE_ONOFF_CTRL1_idle_en_pmuh_on_START (6)
#define SUB_PMIC_IDLE_ONOFF_CTRL1_idle_en_pmuh_on_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_en_buck8_eco : 1;
        unsigned char idle_en_buck7_eco : 1;
        unsigned char idle_en_buck6_eco : 1;
        unsigned char reserved : 1;
        unsigned char idle_en_buck30_eco : 1;
        unsigned char idle_en_buck20_eco : 1;
        unsigned char idle_en_buck10_eco : 1;
        unsigned char idle_en_buck00_eco : 1;
    } reg;
} SUB_PMIC_IDLE_ECO_CTRL0_UNION;
#endif
#define SUB_PMIC_IDLE_ECO_CTRL0_idle_en_buck8_eco_START (0)
#define SUB_PMIC_IDLE_ECO_CTRL0_idle_en_buck8_eco_END (0)
#define SUB_PMIC_IDLE_ECO_CTRL0_idle_en_buck7_eco_START (1)
#define SUB_PMIC_IDLE_ECO_CTRL0_idle_en_buck7_eco_END (1)
#define SUB_PMIC_IDLE_ECO_CTRL0_idle_en_buck6_eco_START (2)
#define SUB_PMIC_IDLE_ECO_CTRL0_idle_en_buck6_eco_END (2)
#define SUB_PMIC_IDLE_ECO_CTRL0_idle_en_buck30_eco_START (4)
#define SUB_PMIC_IDLE_ECO_CTRL0_idle_en_buck30_eco_END (4)
#define SUB_PMIC_IDLE_ECO_CTRL0_idle_en_buck20_eco_START (5)
#define SUB_PMIC_IDLE_ECO_CTRL0_idle_en_buck20_eco_END (5)
#define SUB_PMIC_IDLE_ECO_CTRL0_idle_en_buck10_eco_START (6)
#define SUB_PMIC_IDLE_ECO_CTRL0_idle_en_buck10_eco_END (6)
#define SUB_PMIC_IDLE_ECO_CTRL0_idle_en_buck00_eco_START (7)
#define SUB_PMIC_IDLE_ECO_CTRL0_idle_en_buck00_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_en_ldo34_eco : 1;
        unsigned char idle_en_ldo9_eco : 1;
        unsigned char idle_en_ldo54_eco : 1;
        unsigned char idle_en_ldo53_eco : 1;
        unsigned char idle_en_buck13_eco : 1;
        unsigned char idle_en_buck12_eco : 1;
        unsigned char idle_en_pmuh_eco : 1;
        unsigned char idle_en_ref_eco : 1;
    } reg;
} SUB_PMIC_IDLE_ECO_CTRL1_UNION;
#endif
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_ldo34_eco_START (0)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_ldo34_eco_END (0)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_ldo9_eco_START (1)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_ldo9_eco_END (1)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_ldo54_eco_START (2)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_ldo54_eco_END (2)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_ldo53_eco_START (3)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_ldo53_eco_END (3)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_buck13_eco_START (4)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_buck13_eco_END (4)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_buck12_eco_START (5)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_buck12_eco_END (5)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_pmuh_eco_START (6)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_pmuh_eco_END (6)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_ref_eco_START (7)
#define SUB_PMIC_IDLE_ECO_CTRL1_idle_en_ref_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck00_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IDLE_PLACE_CTRL0_UNION;
#endif
#define SUB_PMIC_IDLE_PLACE_CTRL0_idle_buck00_sel_START (0)
#define SUB_PMIC_IDLE_PLACE_CTRL0_idle_buck00_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck10_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IDLE_PLACE_CTRL1_UNION;
#endif
#define SUB_PMIC_IDLE_PLACE_CTRL1_idle_buck10_sel_START (0)
#define SUB_PMIC_IDLE_PLACE_CTRL1_idle_buck10_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck20_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IDLE_PLACE_CTRL2_UNION;
#endif
#define SUB_PMIC_IDLE_PLACE_CTRL2_idle_buck20_sel_START (0)
#define SUB_PMIC_IDLE_PLACE_CTRL2_idle_buck20_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck30_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IDLE_PLACE_CTRL3_UNION;
#endif
#define SUB_PMIC_IDLE_PLACE_CTRL3_idle_buck30_sel_START (0)
#define SUB_PMIC_IDLE_PLACE_CTRL3_idle_buck30_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck6_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IDLE_PLACE_CTRL5_UNION;
#endif
#define SUB_PMIC_IDLE_PLACE_CTRL5_idle_buck6_sel_START (0)
#define SUB_PMIC_IDLE_PLACE_CTRL5_idle_buck6_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck7_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IDLE_PLACE_CTRL6_UNION;
#endif
#define SUB_PMIC_IDLE_PLACE_CTRL6_idle_buck7_sel_START (0)
#define SUB_PMIC_IDLE_PLACE_CTRL6_idle_buck7_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck8_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IDLE_PLACE_CTRL7_UNION;
#endif
#define SUB_PMIC_IDLE_PLACE_CTRL7_idle_buck8_sel_START (0)
#define SUB_PMIC_IDLE_PLACE_CTRL7_idle_buck8_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck12_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IDLE_PLACE_CTRL8_UNION;
#endif
#define SUB_PMIC_IDLE_PLACE_CTRL8_idle_buck12_sel_START (0)
#define SUB_PMIC_IDLE_PLACE_CTRL8_idle_buck12_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck13_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IDLE_PLACE_CTRL9_UNION;
#endif
#define SUB_PMIC_IDLE_PLACE_CTRL9_idle_buck13_sel_START (0)
#define SUB_PMIC_IDLE_PLACE_CTRL9_idle_buck13_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo9_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IDLE_PLACE_CTRL10_UNION;
#endif
#define SUB_PMIC_IDLE_PLACE_CTRL10_idle_ldo9_sel_START (0)
#define SUB_PMIC_IDLE_PLACE_CTRL10_idle_ldo9_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo34_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IDLE_PLACE_CTRL11_UNION;
#endif
#define SUB_PMIC_IDLE_PLACE_CTRL11_idle_ldo34_sel_START (0)
#define SUB_PMIC_IDLE_PLACE_CTRL11_idle_ldo34_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_pmuh_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IDLE_PLACE_CTRL12_UNION;
#endif
#define SUB_PMIC_IDLE_PLACE_CTRL12_idle_pmuh_sel_START (0)
#define SUB_PMIC_IDLE_PLACE_CTRL12_idle_pmuh_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_cnt_cfg : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IDLE_CNT_CFG_UNION;
#endif
#define SUB_PMIC_IDLE_CNT_CFG_idle_cnt_cfg_START (0)
#define SUB_PMIC_IDLE_CNT_CFG_idle_cnt_cfg_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck8_ocp_auto_stop : 1;
        unsigned char buck7_ocp_auto_stop : 1;
        unsigned char buck6_ocp_auto_stop : 1;
        unsigned char reserved : 1;
        unsigned char buck30_ocp_auto_stop : 1;
        unsigned char buck20_ocp_auto_stop : 1;
        unsigned char buck10_ocp_auto_stop : 1;
        unsigned char buck00_ocp_auto_stop : 1;
    } reg;
} SUB_PMIC_BUCK_OCP_MODE_CTRL0_UNION;
#endif
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_buck8_ocp_auto_stop_START (0)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_buck8_ocp_auto_stop_END (0)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_buck7_ocp_auto_stop_START (1)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_buck7_ocp_auto_stop_END (1)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_buck6_ocp_auto_stop_START (2)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_buck6_ocp_auto_stop_END (2)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_buck30_ocp_auto_stop_START (4)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_buck30_ocp_auto_stop_END (4)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_buck20_ocp_auto_stop_START (5)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_buck20_ocp_auto_stop_END (5)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_buck10_ocp_auto_stop_START (6)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_buck10_ocp_auto_stop_END (6)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_buck00_ocp_auto_stop_START (7)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL0_buck00_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck13_ocp_auto_stop : 1;
        unsigned char buck12_ocp_auto_stop : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK_OCP_MODE_CTRL1_UNION;
#endif
#define SUB_PMIC_BUCK_OCP_MODE_CTRL1_buck13_ocp_auto_stop_START (0)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL1_buck13_ocp_auto_stop_END (0)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL1_buck12_ocp_auto_stop_START (1)
#define SUB_PMIC_BUCK_OCP_MODE_CTRL1_buck12_ocp_auto_stop_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo54_ocp_auto_stop : 1;
        unsigned char ldo53_ocp_auto_stop : 1;
        unsigned char ldo52_ocp_auto_stop : 1;
        unsigned char ldo51_ocp_auto_stop : 1;
        unsigned char ldo50_ocp_auto_stop : 1;
        unsigned char ldo34_ocp_auto_stop : 1;
        unsigned char ldo9_ocp_auto_stop : 1;
        unsigned char sw10_ocp_auto_stop : 1;
    } reg;
} SUB_PMIC_LDO_OCP_MODE_CTRL0_UNION;
#endif
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ldo54_ocp_auto_stop_START (0)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ldo54_ocp_auto_stop_END (0)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ldo53_ocp_auto_stop_START (1)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ldo53_ocp_auto_stop_END (1)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ldo52_ocp_auto_stop_START (2)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ldo52_ocp_auto_stop_END (2)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ldo51_ocp_auto_stop_START (3)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ldo51_ocp_auto_stop_END (3)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ldo50_ocp_auto_stop_START (4)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ldo50_ocp_auto_stop_END (4)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ldo34_ocp_auto_stop_START (5)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ldo34_ocp_auto_stop_END (5)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ldo9_ocp_auto_stop_START (6)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_ldo9_ocp_auto_stop_END (6)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_sw10_ocp_auto_stop_START (7)
#define SUB_PMIC_LDO_OCP_MODE_CTRL0_sw10_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck8_scp_auto_stop : 1;
        unsigned char buck7_scp_auto_stop : 1;
        unsigned char buck6_scp_auto_stop : 1;
        unsigned char reserved : 1;
        unsigned char buck30_scp_auto_stop : 1;
        unsigned char buck20_scp_auto_stop : 1;
        unsigned char buck10_scp_auto_stop : 1;
        unsigned char buck00_scp_auto_stop : 1;
    } reg;
} SUB_PMIC_BUCK_SCP_MODE_CTRL0_UNION;
#endif
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_buck8_scp_auto_stop_START (0)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_buck8_scp_auto_stop_END (0)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_buck7_scp_auto_stop_START (1)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_buck7_scp_auto_stop_END (1)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_buck6_scp_auto_stop_START (2)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_buck6_scp_auto_stop_END (2)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_buck30_scp_auto_stop_START (4)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_buck30_scp_auto_stop_END (4)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_buck20_scp_auto_stop_START (5)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_buck20_scp_auto_stop_END (5)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_buck10_scp_auto_stop_START (6)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_buck10_scp_auto_stop_END (6)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_buck00_scp_auto_stop_START (7)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL0_buck00_scp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck13_scp_auto_stop : 1;
        unsigned char buck12_scp_auto_stop : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK_SCP_MODE_CTRL1_UNION;
#endif
#define SUB_PMIC_BUCK_SCP_MODE_CTRL1_buck13_scp_auto_stop_START (0)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL1_buck13_scp_auto_stop_END (0)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL1_buck12_scp_auto_stop_START (1)
#define SUB_PMIC_BUCK_SCP_MODE_CTRL1_buck12_scp_auto_stop_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck12_ovp_auto_stop : 1;
        unsigned char buck8_ovp_auto_stop : 1;
        unsigned char buck30_ovp_auto_stop : 1;
        unsigned char buck20_ovp_auto_stop : 1;
        unsigned char buck10_ovp_auto_stop : 1;
        unsigned char buck00_ovp_auto_stop : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK_OVP_MODE_CTRL0_UNION;
#endif
#define SUB_PMIC_BUCK_OVP_MODE_CTRL0_buck12_ovp_auto_stop_START (0)
#define SUB_PMIC_BUCK_OVP_MODE_CTRL0_buck12_ovp_auto_stop_END (0)
#define SUB_PMIC_BUCK_OVP_MODE_CTRL0_buck8_ovp_auto_stop_START (1)
#define SUB_PMIC_BUCK_OVP_MODE_CTRL0_buck8_ovp_auto_stop_END (1)
#define SUB_PMIC_BUCK_OVP_MODE_CTRL0_buck30_ovp_auto_stop_START (2)
#define SUB_PMIC_BUCK_OVP_MODE_CTRL0_buck30_ovp_auto_stop_END (2)
#define SUB_PMIC_BUCK_OVP_MODE_CTRL0_buck20_ovp_auto_stop_START (3)
#define SUB_PMIC_BUCK_OVP_MODE_CTRL0_buck20_ovp_auto_stop_END (3)
#define SUB_PMIC_BUCK_OVP_MODE_CTRL0_buck10_ovp_auto_stop_START (4)
#define SUB_PMIC_BUCK_OVP_MODE_CTRL0_buck10_ovp_auto_stop_END (4)
#define SUB_PMIC_BUCK_OVP_MODE_CTRL0_buck00_ovp_auto_stop_START (5)
#define SUB_PMIC_BUCK_OVP_MODE_CTRL0_buck00_ovp_auto_stop_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck30_ocp_sel : 1;
        unsigned char buck20_ocp_sel : 1;
        unsigned char buck10_ocp_sel : 1;
        unsigned char buck00_ocp_sel : 1;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_BUCK_OCP_ABN_CTRL0_UNION;
#endif
#define SUB_PMIC_BUCK_OCP_ABN_CTRL0_buck30_ocp_sel_START (0)
#define SUB_PMIC_BUCK_OCP_ABN_CTRL0_buck30_ocp_sel_END (0)
#define SUB_PMIC_BUCK_OCP_ABN_CTRL0_buck20_ocp_sel_START (1)
#define SUB_PMIC_BUCK_OCP_ABN_CTRL0_buck20_ocp_sel_END (1)
#define SUB_PMIC_BUCK_OCP_ABN_CTRL0_buck10_ocp_sel_START (2)
#define SUB_PMIC_BUCK_OCP_ABN_CTRL0_buck10_ocp_sel_END (2)
#define SUB_PMIC_BUCK_OCP_ABN_CTRL0_buck00_ocp_sel_START (3)
#define SUB_PMIC_BUCK_OCP_ABN_CTRL0_buck00_ocp_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char en_vin_ldoh_deb : 1;
        unsigned char en_ldo_ocp_deb : 1;
        unsigned char en_buck_scp_deb : 1;
        unsigned char en_buck_ocp_deb : 1;
        unsigned char en_buck_ovp_deb : 1;
        unsigned char en_buck_cur_det_deb : 1;
        unsigned char en_buck8_ovp_deb : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_OCP_SCP_ONOFF_UNION;
#endif
#define SUB_PMIC_OCP_SCP_ONOFF_en_vin_ldoh_deb_START (0)
#define SUB_PMIC_OCP_SCP_ONOFF_en_vin_ldoh_deb_END (0)
#define SUB_PMIC_OCP_SCP_ONOFF_en_ldo_ocp_deb_START (1)
#define SUB_PMIC_OCP_SCP_ONOFF_en_ldo_ocp_deb_END (1)
#define SUB_PMIC_OCP_SCP_ONOFF_en_buck_scp_deb_START (2)
#define SUB_PMIC_OCP_SCP_ONOFF_en_buck_scp_deb_END (2)
#define SUB_PMIC_OCP_SCP_ONOFF_en_buck_ocp_deb_START (3)
#define SUB_PMIC_OCP_SCP_ONOFF_en_buck_ocp_deb_END (3)
#define SUB_PMIC_OCP_SCP_ONOFF_en_buck_ovp_deb_START (4)
#define SUB_PMIC_OCP_SCP_ONOFF_en_buck_ovp_deb_END (4)
#define SUB_PMIC_OCP_SCP_ONOFF_en_buck_cur_det_deb_START (5)
#define SUB_PMIC_OCP_SCP_ONOFF_en_buck_cur_det_deb_END (5)
#define SUB_PMIC_OCP_SCP_ONOFF_en_buck8_ovp_deb_START (6)
#define SUB_PMIC_OCP_SCP_ONOFF_en_buck8_ovp_deb_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_ocp_deb_sel : 2;
        unsigned char buck_scp_deb_sel : 2;
        unsigned char buck_ocp_deb_sel : 2;
        unsigned char buck_ovp_deb_sel : 2;
    } reg;
} SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL0_UNION;
#endif
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL0_ldo_ocp_deb_sel_START (0)
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL0_ldo_ocp_deb_sel_END (1)
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL0_buck_scp_deb_sel_START (2)
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL0_buck_scp_deb_sel_END (3)
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL0_buck_ocp_deb_sel_START (4)
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL0_buck_ocp_deb_sel_END (5)
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL0_buck_ovp_deb_sel_START (6)
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL0_buck_ovp_deb_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char vin_ldoh_deb_sel : 2;
        unsigned char buck_cur_det_deb_sel : 2;
        unsigned char buck8_ovp_deb_sel : 2;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL1_UNION;
#endif
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL1_vin_ldoh_deb_sel_START (0)
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL1_vin_ldoh_deb_sel_END (1)
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL1_buck_cur_det_deb_sel_START (2)
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL1_buck_cur_det_deb_sel_END (3)
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL1_buck8_ovp_deb_sel_START (4)
#define SUB_PMIC_BUCK_LDO_ABN_DEB_CTRL1_buck8_ovp_deb_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char vsys_pwroff_deb_sel : 3;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_PWROFF_DEB_CTRL_UNION;
#endif
#define SUB_PMIC_PWROFF_DEB_CTRL_vsys_pwroff_deb_sel_START (0)
#define SUB_PMIC_PWROFF_DEB_CTRL_vsys_pwroff_deb_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck00_rampdown_ctrl : 3;
        unsigned char buck00_rampup_ctrl : 3;
        unsigned char buck00_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_RAMP_BUCK00_CTRL0_UNION;
#endif
#define SUB_PMIC_RAMP_BUCK00_CTRL0_buck00_rampdown_ctrl_START (0)
#define SUB_PMIC_RAMP_BUCK00_CTRL0_buck00_rampdown_ctrl_END (2)
#define SUB_PMIC_RAMP_BUCK00_CTRL0_buck00_rampup_ctrl_START (3)
#define SUB_PMIC_RAMP_BUCK00_CTRL0_buck00_rampup_ctrl_END (5)
#define SUB_PMIC_RAMP_BUCK00_CTRL0_buck00_ramp_en_cfg_START (6)
#define SUB_PMIC_RAMP_BUCK00_CTRL0_buck00_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck00_pull_down_off_cfg : 3;
        unsigned char buck00_pull_down_on_cfg : 3;
        unsigned char buck00_pull_down_cfg : 1;
        unsigned char buck00_pull_down_mode : 1;
    } reg;
} SUB_PMIC_RAMP_BUCK00_CTRL1_UNION;
#endif
#define SUB_PMIC_RAMP_BUCK00_CTRL1_buck00_pull_down_off_cfg_START (0)
#define SUB_PMIC_RAMP_BUCK00_CTRL1_buck00_pull_down_off_cfg_END (2)
#define SUB_PMIC_RAMP_BUCK00_CTRL1_buck00_pull_down_on_cfg_START (3)
#define SUB_PMIC_RAMP_BUCK00_CTRL1_buck00_pull_down_on_cfg_END (5)
#define SUB_PMIC_RAMP_BUCK00_CTRL1_buck00_pull_down_cfg_START (6)
#define SUB_PMIC_RAMP_BUCK00_CTRL1_buck00_pull_down_cfg_END (6)
#define SUB_PMIC_RAMP_BUCK00_CTRL1_buck00_pull_down_mode_START (7)
#define SUB_PMIC_RAMP_BUCK00_CTRL1_buck00_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck00_dly_ru_off_cfg : 3;
        unsigned char buck00_dly_ru_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK00_RAMPUP_STATE_UNION;
#endif
#define SUB_PMIC_BUCK00_RAMPUP_STATE_buck00_dly_ru_off_cfg_START (0)
#define SUB_PMIC_BUCK00_RAMPUP_STATE_buck00_dly_ru_off_cfg_END (2)
#define SUB_PMIC_BUCK00_RAMPUP_STATE_buck00_dly_ru_on_cfg_START (3)
#define SUB_PMIC_BUCK00_RAMPUP_STATE_buck00_dly_ru_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck00_dly_rd_pd_off_cfg : 3;
        unsigned char buck00_dly_rd_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK00_RAMPDOWN_STATE_UNION;
#endif
#define SUB_PMIC_BUCK00_RAMPDOWN_STATE_buck00_dly_rd_pd_off_cfg_START (0)
#define SUB_PMIC_BUCK00_RAMPDOWN_STATE_buck00_dly_rd_pd_off_cfg_END (2)
#define SUB_PMIC_BUCK00_RAMPDOWN_STATE_buck00_dly_rd_on_cfg_START (3)
#define SUB_PMIC_BUCK00_RAMPDOWN_STATE_buck00_dly_rd_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck00_rampdown_chg_rate : 2;
        unsigned char reserved_0 : 2;
        unsigned char buck00_rampup_chg_rate : 2;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK00_RAMP_CHG_RATE_UNION;
#endif
#define SUB_PMIC_BUCK00_RAMP_CHG_RATE_buck00_rampdown_chg_rate_START (0)
#define SUB_PMIC_BUCK00_RAMP_CHG_RATE_buck00_rampdown_chg_rate_END (1)
#define SUB_PMIC_BUCK00_RAMP_CHG_RATE_buck00_rampup_chg_rate_START (4)
#define SUB_PMIC_BUCK00_RAMP_CHG_RATE_buck00_rampup_chg_rate_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck00_rampdown_chg_vset : 3;
        unsigned char reserved_0 : 1;
        unsigned char buck00_rampup_chg_vset : 3;
        unsigned char reserved_1 : 1;
    } reg;
} SUB_PMIC_BUCK00_RAMP_CHG_VSET_UNION;
#endif
#define SUB_PMIC_BUCK00_RAMP_CHG_VSET_buck00_rampdown_chg_vset_START (0)
#define SUB_PMIC_BUCK00_RAMP_CHG_VSET_buck00_rampdown_chg_vset_END (2)
#define SUB_PMIC_BUCK00_RAMP_CHG_VSET_buck00_rampup_chg_vset_START (4)
#define SUB_PMIC_BUCK00_RAMP_CHG_VSET_buck00_rampup_chg_vset_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck00_rd_nochg_rate_sel : 1;
        unsigned char reserved_0 : 3;
        unsigned char buck00_ru_nochg_rate_sel : 1;
        unsigned char reserved_1 : 3;
    } reg;
} SUB_PMIC_BUCK00_CHG_RATE_SEL_UNION;
#endif
#define SUB_PMIC_BUCK00_CHG_RATE_SEL_buck00_rd_nochg_rate_sel_START (0)
#define SUB_PMIC_BUCK00_CHG_RATE_SEL_buck00_rd_nochg_rate_sel_END (0)
#define SUB_PMIC_BUCK00_CHG_RATE_SEL_buck00_ru_nochg_rate_sel_START (4)
#define SUB_PMIC_BUCK00_CHG_RATE_SEL_buck00_ru_nochg_rate_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck00_ramp_state : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_BUCK00_RAMP_STATE_UNION;
#endif
#define SUB_PMIC_BUCK00_RAMP_STATE_buck00_ramp_state_START (0)
#define SUB_PMIC_BUCK00_RAMP_STATE_buck00_ramp_state_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck30_rampdown_ctrl : 3;
        unsigned char buck30_rampup_ctrl : 3;
        unsigned char buck30_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_RAMP_BUCK30_CTRL0_UNION;
#endif
#define SUB_PMIC_RAMP_BUCK30_CTRL0_buck30_rampdown_ctrl_START (0)
#define SUB_PMIC_RAMP_BUCK30_CTRL0_buck30_rampdown_ctrl_END (2)
#define SUB_PMIC_RAMP_BUCK30_CTRL0_buck30_rampup_ctrl_START (3)
#define SUB_PMIC_RAMP_BUCK30_CTRL0_buck30_rampup_ctrl_END (5)
#define SUB_PMIC_RAMP_BUCK30_CTRL0_buck30_ramp_en_cfg_START (6)
#define SUB_PMIC_RAMP_BUCK30_CTRL0_buck30_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck30_pull_down_off_cfg : 3;
        unsigned char buck30_pull_down_on_cfg : 3;
        unsigned char buck30_pull_down_cfg : 1;
        unsigned char buck30_pull_down_mode : 1;
    } reg;
} SUB_PMIC_RAMP_BUCK30_CTRL1_UNION;
#endif
#define SUB_PMIC_RAMP_BUCK30_CTRL1_buck30_pull_down_off_cfg_START (0)
#define SUB_PMIC_RAMP_BUCK30_CTRL1_buck30_pull_down_off_cfg_END (2)
#define SUB_PMIC_RAMP_BUCK30_CTRL1_buck30_pull_down_on_cfg_START (3)
#define SUB_PMIC_RAMP_BUCK30_CTRL1_buck30_pull_down_on_cfg_END (5)
#define SUB_PMIC_RAMP_BUCK30_CTRL1_buck30_pull_down_cfg_START (6)
#define SUB_PMIC_RAMP_BUCK30_CTRL1_buck30_pull_down_cfg_END (6)
#define SUB_PMIC_RAMP_BUCK30_CTRL1_buck30_pull_down_mode_START (7)
#define SUB_PMIC_RAMP_BUCK30_CTRL1_buck30_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck30_dly_ru_off_cfg : 3;
        unsigned char buck30_dly_ru_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK30_RAMPUP_STATE_UNION;
#endif
#define SUB_PMIC_BUCK30_RAMPUP_STATE_buck30_dly_ru_off_cfg_START (0)
#define SUB_PMIC_BUCK30_RAMPUP_STATE_buck30_dly_ru_off_cfg_END (2)
#define SUB_PMIC_BUCK30_RAMPUP_STATE_buck30_dly_ru_on_cfg_START (3)
#define SUB_PMIC_BUCK30_RAMPUP_STATE_buck30_dly_ru_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck30_dly_rd_pd_off_cfg : 3;
        unsigned char buck30_dly_rd_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK30_RAMPDOWN_STATE_UNION;
#endif
#define SUB_PMIC_BUCK30_RAMPDOWN_STATE_buck30_dly_rd_pd_off_cfg_START (0)
#define SUB_PMIC_BUCK30_RAMPDOWN_STATE_buck30_dly_rd_pd_off_cfg_END (2)
#define SUB_PMIC_BUCK30_RAMPDOWN_STATE_buck30_dly_rd_on_cfg_START (3)
#define SUB_PMIC_BUCK30_RAMPDOWN_STATE_buck30_dly_rd_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck30_rampdown_chg_rate : 2;
        unsigned char reserved_0 : 2;
        unsigned char buck30_rampup_chg_rate : 2;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK30_RAMP_CHG_RATE_UNION;
#endif
#define SUB_PMIC_BUCK30_RAMP_CHG_RATE_buck30_rampdown_chg_rate_START (0)
#define SUB_PMIC_BUCK30_RAMP_CHG_RATE_buck30_rampdown_chg_rate_END (1)
#define SUB_PMIC_BUCK30_RAMP_CHG_RATE_buck30_rampup_chg_rate_START (4)
#define SUB_PMIC_BUCK30_RAMP_CHG_RATE_buck30_rampup_chg_rate_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck30_rampdown_chg_vset : 3;
        unsigned char reserved_0 : 1;
        unsigned char buck30_rampup_chg_vset : 3;
        unsigned char reserved_1 : 1;
    } reg;
} SUB_PMIC_BUCK30_RAMP_CHG_VSET_UNION;
#endif
#define SUB_PMIC_BUCK30_RAMP_CHG_VSET_buck30_rampdown_chg_vset_START (0)
#define SUB_PMIC_BUCK30_RAMP_CHG_VSET_buck30_rampdown_chg_vset_END (2)
#define SUB_PMIC_BUCK30_RAMP_CHG_VSET_buck30_rampup_chg_vset_START (4)
#define SUB_PMIC_BUCK30_RAMP_CHG_VSET_buck30_rampup_chg_vset_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck30_rd_nochg_rate_sel : 1;
        unsigned char reserved_0 : 3;
        unsigned char buck30_ru_nochg_rate_sel : 1;
        unsigned char reserved_1 : 3;
    } reg;
} SUB_PMIC_BUCK30_CHG_RATE_SEL_UNION;
#endif
#define SUB_PMIC_BUCK30_CHG_RATE_SEL_buck30_rd_nochg_rate_sel_START (0)
#define SUB_PMIC_BUCK30_CHG_RATE_SEL_buck30_rd_nochg_rate_sel_END (0)
#define SUB_PMIC_BUCK30_CHG_RATE_SEL_buck30_ru_nochg_rate_sel_START (4)
#define SUB_PMIC_BUCK30_CHG_RATE_SEL_buck30_ru_nochg_rate_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck30_ramp_state : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_BUCK30_RAMP_STATE_UNION;
#endif
#define SUB_PMIC_BUCK30_RAMP_STATE_buck30_ramp_state_START (0)
#define SUB_PMIC_BUCK30_RAMP_STATE_buck30_ramp_state_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck6_rampdown_ctrl : 3;
        unsigned char buck6_rampup_ctrl : 3;
        unsigned char buck6_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_RAMP_BUCK6_CTRL0_UNION;
#endif
#define SUB_PMIC_RAMP_BUCK6_CTRL0_buck6_rampdown_ctrl_START (0)
#define SUB_PMIC_RAMP_BUCK6_CTRL0_buck6_rampdown_ctrl_END (2)
#define SUB_PMIC_RAMP_BUCK6_CTRL0_buck6_rampup_ctrl_START (3)
#define SUB_PMIC_RAMP_BUCK6_CTRL0_buck6_rampup_ctrl_END (5)
#define SUB_PMIC_RAMP_BUCK6_CTRL0_buck6_ramp_en_cfg_START (6)
#define SUB_PMIC_RAMP_BUCK6_CTRL0_buck6_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck6_pull_down_off_cfg : 3;
        unsigned char buck6_pull_down_on_cfg : 3;
        unsigned char buck6_pull_down_cfg : 1;
        unsigned char buck6_pull_down_mode : 1;
    } reg;
} SUB_PMIC_RAMP_BUCK6_CTRL1_UNION;
#endif
#define SUB_PMIC_RAMP_BUCK6_CTRL1_buck6_pull_down_off_cfg_START (0)
#define SUB_PMIC_RAMP_BUCK6_CTRL1_buck6_pull_down_off_cfg_END (2)
#define SUB_PMIC_RAMP_BUCK6_CTRL1_buck6_pull_down_on_cfg_START (3)
#define SUB_PMIC_RAMP_BUCK6_CTRL1_buck6_pull_down_on_cfg_END (5)
#define SUB_PMIC_RAMP_BUCK6_CTRL1_buck6_pull_down_cfg_START (6)
#define SUB_PMIC_RAMP_BUCK6_CTRL1_buck6_pull_down_cfg_END (6)
#define SUB_PMIC_RAMP_BUCK6_CTRL1_buck6_pull_down_mode_START (7)
#define SUB_PMIC_RAMP_BUCK6_CTRL1_buck6_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck6_dly_ru_off_cfg : 3;
        unsigned char buck6_dly_ru_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK6_RAMPUP_STATE_UNION;
#endif
#define SUB_PMIC_BUCK6_RAMPUP_STATE_buck6_dly_ru_off_cfg_START (0)
#define SUB_PMIC_BUCK6_RAMPUP_STATE_buck6_dly_ru_off_cfg_END (2)
#define SUB_PMIC_BUCK6_RAMPUP_STATE_buck6_dly_ru_on_cfg_START (3)
#define SUB_PMIC_BUCK6_RAMPUP_STATE_buck6_dly_ru_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck6_dly_rd_pd_off_cfg : 3;
        unsigned char buck6_dly_rd_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK6_RAMPDOWN_STATE_UNION;
#endif
#define SUB_PMIC_BUCK6_RAMPDOWN_STATE_buck6_dly_rd_pd_off_cfg_START (0)
#define SUB_PMIC_BUCK6_RAMPDOWN_STATE_buck6_dly_rd_pd_off_cfg_END (2)
#define SUB_PMIC_BUCK6_RAMPDOWN_STATE_buck6_dly_rd_on_cfg_START (3)
#define SUB_PMIC_BUCK6_RAMPDOWN_STATE_buck6_dly_rd_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck6_rampdown_chg_rate : 2;
        unsigned char reserved_0 : 2;
        unsigned char buck6_rampup_chg_rate : 2;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK6_RAMP_CHG_RATE_UNION;
#endif
#define SUB_PMIC_BUCK6_RAMP_CHG_RATE_buck6_rampdown_chg_rate_START (0)
#define SUB_PMIC_BUCK6_RAMP_CHG_RATE_buck6_rampdown_chg_rate_END (1)
#define SUB_PMIC_BUCK6_RAMP_CHG_RATE_buck6_rampup_chg_rate_START (4)
#define SUB_PMIC_BUCK6_RAMP_CHG_RATE_buck6_rampup_chg_rate_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck6_rampdown_chg_vset : 3;
        unsigned char reserved_0 : 1;
        unsigned char buck6_rampup_chg_vset : 3;
        unsigned char reserved_1 : 1;
    } reg;
} SUB_PMIC_BUCK6_RAMP_CHG_VSET_UNION;
#endif
#define SUB_PMIC_BUCK6_RAMP_CHG_VSET_buck6_rampdown_chg_vset_START (0)
#define SUB_PMIC_BUCK6_RAMP_CHG_VSET_buck6_rampdown_chg_vset_END (2)
#define SUB_PMIC_BUCK6_RAMP_CHG_VSET_buck6_rampup_chg_vset_START (4)
#define SUB_PMIC_BUCK6_RAMP_CHG_VSET_buck6_rampup_chg_vset_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck6_rd_nochg_rate_sel : 1;
        unsigned char reserved_0 : 3;
        unsigned char buck6_ru_nochg_rate_sel : 1;
        unsigned char reserved_1 : 3;
    } reg;
} SUB_PMIC_BUCK6_CHG_RATE_SEL_UNION;
#endif
#define SUB_PMIC_BUCK6_CHG_RATE_SEL_buck6_rd_nochg_rate_sel_START (0)
#define SUB_PMIC_BUCK6_CHG_RATE_SEL_buck6_rd_nochg_rate_sel_END (0)
#define SUB_PMIC_BUCK6_CHG_RATE_SEL_buck6_ru_nochg_rate_sel_START (4)
#define SUB_PMIC_BUCK6_CHG_RATE_SEL_buck6_ru_nochg_rate_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck6_ramp_state : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_BUCK6_RAMP_STATE_UNION;
#endif
#define SUB_PMIC_BUCK6_RAMP_STATE_buck6_ramp_state_START (0)
#define SUB_PMIC_BUCK6_RAMP_STATE_buck6_ramp_state_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck7_rampdown_ctrl : 3;
        unsigned char buck7_rampup_ctrl : 3;
        unsigned char buck7_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_RAMP_BUCK7_CTRL0_UNION;
#endif
#define SUB_PMIC_RAMP_BUCK7_CTRL0_buck7_rampdown_ctrl_START (0)
#define SUB_PMIC_RAMP_BUCK7_CTRL0_buck7_rampdown_ctrl_END (2)
#define SUB_PMIC_RAMP_BUCK7_CTRL0_buck7_rampup_ctrl_START (3)
#define SUB_PMIC_RAMP_BUCK7_CTRL0_buck7_rampup_ctrl_END (5)
#define SUB_PMIC_RAMP_BUCK7_CTRL0_buck7_ramp_en_cfg_START (6)
#define SUB_PMIC_RAMP_BUCK7_CTRL0_buck7_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck7_pull_down_off_cfg : 3;
        unsigned char buck7_pull_down_on_cfg : 3;
        unsigned char buck7_pull_down_cfg : 1;
        unsigned char buck7_pull_down_mode : 1;
    } reg;
} SUB_PMIC_RAMP_BUCK7_CTRL1_UNION;
#endif
#define SUB_PMIC_RAMP_BUCK7_CTRL1_buck7_pull_down_off_cfg_START (0)
#define SUB_PMIC_RAMP_BUCK7_CTRL1_buck7_pull_down_off_cfg_END (2)
#define SUB_PMIC_RAMP_BUCK7_CTRL1_buck7_pull_down_on_cfg_START (3)
#define SUB_PMIC_RAMP_BUCK7_CTRL1_buck7_pull_down_on_cfg_END (5)
#define SUB_PMIC_RAMP_BUCK7_CTRL1_buck7_pull_down_cfg_START (6)
#define SUB_PMIC_RAMP_BUCK7_CTRL1_buck7_pull_down_cfg_END (6)
#define SUB_PMIC_RAMP_BUCK7_CTRL1_buck7_pull_down_mode_START (7)
#define SUB_PMIC_RAMP_BUCK7_CTRL1_buck7_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck7_dly_ru_off_cfg : 3;
        unsigned char buck7_dly_ru_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK7_RAMPUP_STATE_UNION;
#endif
#define SUB_PMIC_BUCK7_RAMPUP_STATE_buck7_dly_ru_off_cfg_START (0)
#define SUB_PMIC_BUCK7_RAMPUP_STATE_buck7_dly_ru_off_cfg_END (2)
#define SUB_PMIC_BUCK7_RAMPUP_STATE_buck7_dly_ru_on_cfg_START (3)
#define SUB_PMIC_BUCK7_RAMPUP_STATE_buck7_dly_ru_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck7_dly_rd_pd_off_cfg : 3;
        unsigned char buck7_dly_rd_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK7_RAMPDOWN_STATE_UNION;
#endif
#define SUB_PMIC_BUCK7_RAMPDOWN_STATE_buck7_dly_rd_pd_off_cfg_START (0)
#define SUB_PMIC_BUCK7_RAMPDOWN_STATE_buck7_dly_rd_pd_off_cfg_END (2)
#define SUB_PMIC_BUCK7_RAMPDOWN_STATE_buck7_dly_rd_on_cfg_START (3)
#define SUB_PMIC_BUCK7_RAMPDOWN_STATE_buck7_dly_rd_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck7_rampdown_chg_rate : 2;
        unsigned char reserved_0 : 2;
        unsigned char buck7_rampup_chg_rate : 2;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK7_RAMP_CHG_RATE_UNION;
#endif
#define SUB_PMIC_BUCK7_RAMP_CHG_RATE_buck7_rampdown_chg_rate_START (0)
#define SUB_PMIC_BUCK7_RAMP_CHG_RATE_buck7_rampdown_chg_rate_END (1)
#define SUB_PMIC_BUCK7_RAMP_CHG_RATE_buck7_rampup_chg_rate_START (4)
#define SUB_PMIC_BUCK7_RAMP_CHG_RATE_buck7_rampup_chg_rate_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck7_rampdown_chg_vset : 3;
        unsigned char reserved_0 : 1;
        unsigned char buck7_rampup_chg_vset : 3;
        unsigned char reserved_1 : 1;
    } reg;
} SUB_PMIC_BUCK7_RAMP_CHG_VSET_UNION;
#endif
#define SUB_PMIC_BUCK7_RAMP_CHG_VSET_buck7_rampdown_chg_vset_START (0)
#define SUB_PMIC_BUCK7_RAMP_CHG_VSET_buck7_rampdown_chg_vset_END (2)
#define SUB_PMIC_BUCK7_RAMP_CHG_VSET_buck7_rampup_chg_vset_START (4)
#define SUB_PMIC_BUCK7_RAMP_CHG_VSET_buck7_rampup_chg_vset_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck7_rd_nochg_rate_sel : 1;
        unsigned char reserved_0 : 3;
        unsigned char buck7_ru_nochg_rate_sel : 1;
        unsigned char reserved_1 : 3;
    } reg;
} SUB_PMIC_BUCK7_CHG_RATE_SEL_UNION;
#endif
#define SUB_PMIC_BUCK7_CHG_RATE_SEL_buck7_rd_nochg_rate_sel_START (0)
#define SUB_PMIC_BUCK7_CHG_RATE_SEL_buck7_rd_nochg_rate_sel_END (0)
#define SUB_PMIC_BUCK7_CHG_RATE_SEL_buck7_ru_nochg_rate_sel_START (4)
#define SUB_PMIC_BUCK7_CHG_RATE_SEL_buck7_ru_nochg_rate_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck7_ramp_state : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_BUCK7_RAMP_STATE_UNION;
#endif
#define SUB_PMIC_BUCK7_RAMP_STATE_buck7_ramp_state_START (0)
#define SUB_PMIC_BUCK7_RAMP_STATE_buck7_ramp_state_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_lra_soft_rst_n : 8;
    } reg;
} SUB_PMIC_LRA_SOFT_RST_UNION;
#endif
#define SUB_PMIC_LRA_SOFT_RST_sc_lra_soft_rst_n_START (0)
#define SUB_PMIC_LRA_SOFT_RST_sc_lra_soft_rst_n_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_ramp_clk_en : 1;
        unsigned char reserved_0 : 3;
        unsigned char ramp_gt_debug : 1;
        unsigned char reserved_1 : 3;
    } reg;
} SUB_PMIC_RAMP_CLK_BYPASS_UNION;
#endif
#define SUB_PMIC_RAMP_CLK_BYPASS_sc_ramp_clk_en_START (0)
#define SUB_PMIC_RAMP_CLK_BYPASS_sc_ramp_clk_en_END (0)
#define SUB_PMIC_RAMP_CLK_BYPASS_ramp_gt_debug_START (4)
#define SUB_PMIC_RAMP_CLK_BYPASS_ramp_gt_debug_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim_hpd_r_pd_en : 1;
        unsigned char sim_hpd_f_pd_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char sim_hpd_pd_ldo54_en : 1;
        unsigned char sim_hpd_pd_ldo53_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_SIM_CTRL0_UNION;
#endif
#define SUB_PMIC_SIM_CTRL0_sim_hpd_r_pd_en_START (0)
#define SUB_PMIC_SIM_CTRL0_sim_hpd_r_pd_en_END (0)
#define SUB_PMIC_SIM_CTRL0_sim_hpd_f_pd_en_START (1)
#define SUB_PMIC_SIM_CTRL0_sim_hpd_f_pd_en_END (1)
#define SUB_PMIC_SIM_CTRL0_sim_hpd_pd_ldo54_en_START (4)
#define SUB_PMIC_SIM_CTRL0_sim_hpd_pd_ldo54_en_END (4)
#define SUB_PMIC_SIM_CTRL0_sim_hpd_pd_ldo53_en_START (5)
#define SUB_PMIC_SIM_CTRL0_sim_hpd_pd_ldo53_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim_hpd_deb_sel : 5;
        unsigned char sim_del_sel : 3;
    } reg;
} SUB_PMIC_SIM_DEB_CTRL1_UNION;
#endif
#define SUB_PMIC_SIM_DEB_CTRL1_sim_hpd_deb_sel_START (0)
#define SUB_PMIC_SIM_DEB_CTRL1_sim_hpd_deb_sel_END (4)
#define SUB_PMIC_SIM_DEB_CTRL1_sim_del_sel_START (5)
#define SUB_PMIC_SIM_DEB_CTRL1_sim_del_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio_data : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIODATA_UNION;
#endif
#define SUB_PMIC_GPIODATA_gpio_data_START (0)
#define SUB_PMIC_GPIODATA_gpio_data_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} SUB_PMIC_GPIO_DATA_RESERVE_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio_dir : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIODIR_UNION;
#endif
#define SUB_PMIC_GPIODIR_gpio_dir_START (0)
#define SUB_PMIC_GPIODIR_gpio_dir_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} SUB_PMIC_GPIO_DIR_RESERVE_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio_int_sense : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIOIS_UNION;
#endif
#define SUB_PMIC_GPIOIS_gpio_int_sense_START (0)
#define SUB_PMIC_GPIOIS_gpio_int_sense_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio_int_edge : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIOIBE_UNION;
#endif
#define SUB_PMIC_GPIOIBE_gpio_int_edge_START (0)
#define SUB_PMIC_GPIOIBE_gpio_int_edge_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio_int_f : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIOIEV_UNION;
#endif
#define SUB_PMIC_GPIOIEV_gpio_int_f_START (0)
#define SUB_PMIC_GPIOIEV_gpio_int_f_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio_sim_mode_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIOAFSEL_UNION;
#endif
#define SUB_PMIC_GPIOAFSEL_gpio_sim_mode_ctrl_START (0)
#define SUB_PMIC_GPIOAFSEL_gpio_sim_mode_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio_out_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIODSSEL_UNION;
#endif
#define SUB_PMIC_GPIODSSEL_gpio_out_ctrl_START (0)
#define SUB_PMIC_GPIODSSEL_gpio_out_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio_pullup_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIOPUSEL_UNION;
#endif
#define SUB_PMIC_GPIOPUSEL_gpio_pullup_ctrl_START (0)
#define SUB_PMIC_GPIOPUSEL_gpio_pullup_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio_pulldown_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIOPDSEL_UNION;
#endif
#define SUB_PMIC_GPIOPDSEL_gpio_pulldown_ctrl_START (0)
#define SUB_PMIC_GPIOPDSEL_gpio_pulldown_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio_del_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_GPIODEBSEL_UNION;
#endif
#define SUB_PMIC_GPIODEBSEL_gpio_del_sel_START (0)
#define SUB_PMIC_GPIODEBSEL_gpio_del_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim_pullup_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_SIMPUSEL_UNION;
#endif
#define SUB_PMIC_SIMPUSEL_sim_pullup_ctrl_START (0)
#define SUB_PMIC_SIMPUSEL_sim_pullup_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim_pulldown_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_SIMPDSEL_UNION;
#endif
#define SUB_PMIC_SIMPDSEL_sim_pulldown_ctrl_START (0)
#define SUB_PMIC_SIMPDSEL_sim_pulldown_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_data : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIO0DATA_UNION;
#endif
#define SUB_PMIC_GPIO0DATA_gpio0_data_START (0)
#define SUB_PMIC_GPIO0DATA_gpio0_data_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} SUB_PMIC_GPIO0_DATA_RESERVE_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_dir : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIO0DIR_UNION;
#endif
#define SUB_PMIC_GPIO0DIR_gpio0_dir_START (0)
#define SUB_PMIC_GPIO0DIR_gpio0_dir_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} SUB_PMIC_GPIO0_DIR_RESERVE_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_int_sense : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIO0IS_UNION;
#endif
#define SUB_PMIC_GPIO0IS_gpio0_int_sense_START (0)
#define SUB_PMIC_GPIO0IS_gpio0_int_sense_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_int_edge : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIO0IBE_UNION;
#endif
#define SUB_PMIC_GPIO0IBE_gpio0_int_edge_START (0)
#define SUB_PMIC_GPIO0IBE_gpio0_int_edge_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_int_f : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIO0IEV_UNION;
#endif
#define SUB_PMIC_GPIO0IEV_gpio0_int_f_START (0)
#define SUB_PMIC_GPIO0IEV_gpio0_int_f_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_out_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIO0DSSEL_UNION;
#endif
#define SUB_PMIC_GPIO0DSSEL_gpio0_out_ctrl_START (0)
#define SUB_PMIC_GPIO0DSSEL_gpio0_out_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_pullup_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIO0PUSEL_UNION;
#endif
#define SUB_PMIC_GPIO0PUSEL_gpio0_pullup_ctrl_START (0)
#define SUB_PMIC_GPIO0PUSEL_gpio0_pullup_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_pulldown_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_GPIO0PDSEL_UNION;
#endif
#define SUB_PMIC_GPIO0PDSEL_gpio0_pulldown_ctrl_START (0)
#define SUB_PMIC_GPIO0PDSEL_gpio0_pulldown_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_del_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_GPIO0DEBSEL_UNION;
#endif
#define SUB_PMIC_GPIO0DEBSEL_gpio0_del_sel_START (0)
#define SUB_PMIC_GPIO0DEBSEL_gpio0_del_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_otp_clk_en : 8;
    } reg;
} SUB_PMIC_OTP_CLK_EN_UNION;
#endif
#define SUB_PMIC_OTP_CLK_EN_sc_otp_clk_en_START (0)
#define SUB_PMIC_OTP_CLK_EN_sc_otp_clk_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob0 : 8;
    } reg;
} SUB_PMIC_OTP0_0_UNION;
#endif
#define SUB_PMIC_OTP0_0_otp0_pdob0_START (0)
#define SUB_PMIC_OTP0_0_otp0_pdob0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob1 : 8;
    } reg;
} SUB_PMIC_OTP0_1_UNION;
#endif
#define SUB_PMIC_OTP0_1_otp0_pdob1_START (0)
#define SUB_PMIC_OTP0_1_otp0_pdob1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pwe_int : 1;
        unsigned char otp0_pwe_pulse : 1;
        unsigned char otp0_por_int : 1;
        unsigned char otp0_por_pulse : 1;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_OTP0_CTRL0_UNION;
#endif
#define SUB_PMIC_OTP0_CTRL0_otp0_pwe_int_START (0)
#define SUB_PMIC_OTP0_CTRL0_otp0_pwe_int_END (0)
#define SUB_PMIC_OTP0_CTRL0_otp0_pwe_pulse_START (1)
#define SUB_PMIC_OTP0_CTRL0_otp0_pwe_pulse_END (1)
#define SUB_PMIC_OTP0_CTRL0_otp0_por_int_START (2)
#define SUB_PMIC_OTP0_CTRL0_otp0_por_int_END (2)
#define SUB_PMIC_OTP0_CTRL0_otp0_por_pulse_START (3)
#define SUB_PMIC_OTP0_CTRL0_otp0_por_pulse_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pprog : 1;
        unsigned char otp0_inctrl_sel : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_OTP0_CTRL1_UNION;
#endif
#define SUB_PMIC_OTP0_CTRL1_otp0_pprog_START (0)
#define SUB_PMIC_OTP0_CTRL1_otp0_pprog_END (0)
#define SUB_PMIC_OTP0_CTRL1_otp0_inctrl_sel_START (1)
#define SUB_PMIC_OTP0_CTRL1_otp0_inctrl_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pa_cfg : 6;
        unsigned char otp0_ptm : 2;
    } reg;
} SUB_PMIC_OTP0_CTRL2_UNION;
#endif
#define SUB_PMIC_OTP0_CTRL2_otp0_pa_cfg_START (0)
#define SUB_PMIC_OTP0_CTRL2_otp0_pa_cfg_END (5)
#define SUB_PMIC_OTP0_CTRL2_otp0_ptm_START (6)
#define SUB_PMIC_OTP0_CTRL2_otp0_ptm_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdin : 8;
    } reg;
} SUB_PMIC_OTP0_WDATA_UNION;
#endif
#define SUB_PMIC_OTP0_WDATA_otp0_pdin_START (0)
#define SUB_PMIC_OTP0_WDATA_otp0_pdin_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob0_w : 8;
    } reg;
} SUB_PMIC_OTP0_0_W_UNION;
#endif
#define SUB_PMIC_OTP0_0_W_otp0_pdob0_w_START (0)
#define SUB_PMIC_OTP0_0_W_otp0_pdob0_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob1_w : 8;
    } reg;
} SUB_PMIC_OTP0_1_W_UNION;
#endif
#define SUB_PMIC_OTP0_1_W_otp0_pdob1_w_START (0)
#define SUB_PMIC_OTP0_1_W_otp0_pdob1_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob2_w : 8;
    } reg;
} SUB_PMIC_OTP0_2_W_UNION;
#endif
#define SUB_PMIC_OTP0_2_W_otp0_pdob2_w_START (0)
#define SUB_PMIC_OTP0_2_W_otp0_pdob2_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob3_w : 8;
    } reg;
} SUB_PMIC_OTP0_3_W_UNION;
#endif
#define SUB_PMIC_OTP0_3_W_otp0_pdob3_w_START (0)
#define SUB_PMIC_OTP0_3_W_otp0_pdob3_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob4_w : 8;
    } reg;
} SUB_PMIC_OTP0_4_W_UNION;
#endif
#define SUB_PMIC_OTP0_4_W_otp0_pdob4_w_START (0)
#define SUB_PMIC_OTP0_4_W_otp0_pdob4_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob5_w : 8;
    } reg;
} SUB_PMIC_OTP0_5_W_UNION;
#endif
#define SUB_PMIC_OTP0_5_W_otp0_pdob5_w_START (0)
#define SUB_PMIC_OTP0_5_W_otp0_pdob5_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob6_w : 8;
    } reg;
} SUB_PMIC_OTP0_6_W_UNION;
#endif
#define SUB_PMIC_OTP0_6_W_otp0_pdob6_w_START (0)
#define SUB_PMIC_OTP0_6_W_otp0_pdob6_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob7_w : 8;
    } reg;
} SUB_PMIC_OTP0_7_W_UNION;
#endif
#define SUB_PMIC_OTP0_7_W_otp0_pdob7_w_START (0)
#define SUB_PMIC_OTP0_7_W_otp0_pdob7_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob8_w : 8;
    } reg;
} SUB_PMIC_OTP0_8_W_UNION;
#endif
#define SUB_PMIC_OTP0_8_W_otp0_pdob8_w_START (0)
#define SUB_PMIC_OTP0_8_W_otp0_pdob8_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob9_w : 8;
    } reg;
} SUB_PMIC_OTP0_9_W_UNION;
#endif
#define SUB_PMIC_OTP0_9_W_otp0_pdob9_w_START (0)
#define SUB_PMIC_OTP0_9_W_otp0_pdob9_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob10_w : 8;
    } reg;
} SUB_PMIC_OTP0_10_W_UNION;
#endif
#define SUB_PMIC_OTP0_10_W_otp0_pdob10_w_START (0)
#define SUB_PMIC_OTP0_10_W_otp0_pdob10_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob11_w : 8;
    } reg;
} SUB_PMIC_OTP0_11_W_UNION;
#endif
#define SUB_PMIC_OTP0_11_W_otp0_pdob11_w_START (0)
#define SUB_PMIC_OTP0_11_W_otp0_pdob11_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob12_w : 8;
    } reg;
} SUB_PMIC_OTP0_12_W_UNION;
#endif
#define SUB_PMIC_OTP0_12_W_otp0_pdob12_w_START (0)
#define SUB_PMIC_OTP0_12_W_otp0_pdob12_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob13_w : 8;
    } reg;
} SUB_PMIC_OTP0_13_W_UNION;
#endif
#define SUB_PMIC_OTP0_13_W_otp0_pdob13_w_START (0)
#define SUB_PMIC_OTP0_13_W_otp0_pdob13_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob14_w : 8;
    } reg;
} SUB_PMIC_OTP0_14_W_UNION;
#endif
#define SUB_PMIC_OTP0_14_W_otp0_pdob14_w_START (0)
#define SUB_PMIC_OTP0_14_W_otp0_pdob14_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob15_w : 8;
    } reg;
} SUB_PMIC_OTP0_15_W_UNION;
#endif
#define SUB_PMIC_OTP0_15_W_otp0_pdob15_w_START (0)
#define SUB_PMIC_OTP0_15_W_otp0_pdob15_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob16_w : 8;
    } reg;
} SUB_PMIC_OTP0_16_W_UNION;
#endif
#define SUB_PMIC_OTP0_16_W_otp0_pdob16_w_START (0)
#define SUB_PMIC_OTP0_16_W_otp0_pdob16_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob17_w : 8;
    } reg;
} SUB_PMIC_OTP0_17_W_UNION;
#endif
#define SUB_PMIC_OTP0_17_W_otp0_pdob17_w_START (0)
#define SUB_PMIC_OTP0_17_W_otp0_pdob17_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob18_w : 8;
    } reg;
} SUB_PMIC_OTP0_18_W_UNION;
#endif
#define SUB_PMIC_OTP0_18_W_otp0_pdob18_w_START (0)
#define SUB_PMIC_OTP0_18_W_otp0_pdob18_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob19_w : 8;
    } reg;
} SUB_PMIC_OTP0_19_W_UNION;
#endif
#define SUB_PMIC_OTP0_19_W_otp0_pdob19_w_START (0)
#define SUB_PMIC_OTP0_19_W_otp0_pdob19_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob20_w : 8;
    } reg;
} SUB_PMIC_OTP0_20_W_UNION;
#endif
#define SUB_PMIC_OTP0_20_W_otp0_pdob20_w_START (0)
#define SUB_PMIC_OTP0_20_W_otp0_pdob20_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob21_w : 8;
    } reg;
} SUB_PMIC_OTP0_21_W_UNION;
#endif
#define SUB_PMIC_OTP0_21_W_otp0_pdob21_w_START (0)
#define SUB_PMIC_OTP0_21_W_otp0_pdob21_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob22_w : 8;
    } reg;
} SUB_PMIC_OTP0_22_W_UNION;
#endif
#define SUB_PMIC_OTP0_22_W_otp0_pdob22_w_START (0)
#define SUB_PMIC_OTP0_22_W_otp0_pdob22_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob23_w : 8;
    } reg;
} SUB_PMIC_OTP0_23_W_UNION;
#endif
#define SUB_PMIC_OTP0_23_W_otp0_pdob23_w_START (0)
#define SUB_PMIC_OTP0_23_W_otp0_pdob23_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob24_w : 8;
    } reg;
} SUB_PMIC_OTP0_24_W_UNION;
#endif
#define SUB_PMIC_OTP0_24_W_otp0_pdob24_w_START (0)
#define SUB_PMIC_OTP0_24_W_otp0_pdob24_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob25_w : 8;
    } reg;
} SUB_PMIC_OTP0_25_W_UNION;
#endif
#define SUB_PMIC_OTP0_25_W_otp0_pdob25_w_START (0)
#define SUB_PMIC_OTP0_25_W_otp0_pdob25_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob26_w : 8;
    } reg;
} SUB_PMIC_OTP0_26_W_UNION;
#endif
#define SUB_PMIC_OTP0_26_W_otp0_pdob26_w_START (0)
#define SUB_PMIC_OTP0_26_W_otp0_pdob26_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob27_w : 8;
    } reg;
} SUB_PMIC_OTP0_27_W_UNION;
#endif
#define SUB_PMIC_OTP0_27_W_otp0_pdob27_w_START (0)
#define SUB_PMIC_OTP0_27_W_otp0_pdob27_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob28_w : 8;
    } reg;
} SUB_PMIC_OTP0_28_W_UNION;
#endif
#define SUB_PMIC_OTP0_28_W_otp0_pdob28_w_START (0)
#define SUB_PMIC_OTP0_28_W_otp0_pdob28_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob29_w : 8;
    } reg;
} SUB_PMIC_OTP0_29_W_UNION;
#endif
#define SUB_PMIC_OTP0_29_W_otp0_pdob29_w_START (0)
#define SUB_PMIC_OTP0_29_W_otp0_pdob29_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob30_w : 8;
    } reg;
} SUB_PMIC_OTP0_30_W_UNION;
#endif
#define SUB_PMIC_OTP0_30_W_otp0_pdob30_w_START (0)
#define SUB_PMIC_OTP0_30_W_otp0_pdob30_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob31_w : 8;
    } reg;
} SUB_PMIC_OTP0_31_W_UNION;
#endif
#define SUB_PMIC_OTP0_31_W_otp0_pdob31_w_START (0)
#define SUB_PMIC_OTP0_31_W_otp0_pdob31_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob32_w : 8;
    } reg;
} SUB_PMIC_OTP0_32_W_UNION;
#endif
#define SUB_PMIC_OTP0_32_W_otp0_pdob32_w_START (0)
#define SUB_PMIC_OTP0_32_W_otp0_pdob32_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob33_w : 8;
    } reg;
} SUB_PMIC_OTP0_33_W_UNION;
#endif
#define SUB_PMIC_OTP0_33_W_otp0_pdob33_w_START (0)
#define SUB_PMIC_OTP0_33_W_otp0_pdob33_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob34_w : 8;
    } reg;
} SUB_PMIC_OTP0_34_W_UNION;
#endif
#define SUB_PMIC_OTP0_34_W_otp0_pdob34_w_START (0)
#define SUB_PMIC_OTP0_34_W_otp0_pdob34_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob35_w : 8;
    } reg;
} SUB_PMIC_OTP0_35_W_UNION;
#endif
#define SUB_PMIC_OTP0_35_W_otp0_pdob35_w_START (0)
#define SUB_PMIC_OTP0_35_W_otp0_pdob35_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob36_w : 8;
    } reg;
} SUB_PMIC_OTP0_36_W_UNION;
#endif
#define SUB_PMIC_OTP0_36_W_otp0_pdob36_w_START (0)
#define SUB_PMIC_OTP0_36_W_otp0_pdob36_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob37_w : 8;
    } reg;
} SUB_PMIC_OTP0_37_W_UNION;
#endif
#define SUB_PMIC_OTP0_37_W_otp0_pdob37_w_START (0)
#define SUB_PMIC_OTP0_37_W_otp0_pdob37_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob38_w : 8;
    } reg;
} SUB_PMIC_OTP0_38_W_UNION;
#endif
#define SUB_PMIC_OTP0_38_W_otp0_pdob38_w_START (0)
#define SUB_PMIC_OTP0_38_W_otp0_pdob38_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob39_w : 8;
    } reg;
} SUB_PMIC_OTP0_39_W_UNION;
#endif
#define SUB_PMIC_OTP0_39_W_otp0_pdob39_w_START (0)
#define SUB_PMIC_OTP0_39_W_otp0_pdob39_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob40_w : 8;
    } reg;
} SUB_PMIC_OTP0_40_W_UNION;
#endif
#define SUB_PMIC_OTP0_40_W_otp0_pdob40_w_START (0)
#define SUB_PMIC_OTP0_40_W_otp0_pdob40_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob41_w : 8;
    } reg;
} SUB_PMIC_OTP0_41_W_UNION;
#endif
#define SUB_PMIC_OTP0_41_W_otp0_pdob41_w_START (0)
#define SUB_PMIC_OTP0_41_W_otp0_pdob41_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob42_w : 8;
    } reg;
} SUB_PMIC_OTP0_42_W_UNION;
#endif
#define SUB_PMIC_OTP0_42_W_otp0_pdob42_w_START (0)
#define SUB_PMIC_OTP0_42_W_otp0_pdob42_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob43_w : 8;
    } reg;
} SUB_PMIC_OTP0_43_W_UNION;
#endif
#define SUB_PMIC_OTP0_43_W_otp0_pdob43_w_START (0)
#define SUB_PMIC_OTP0_43_W_otp0_pdob43_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob44_w : 8;
    } reg;
} SUB_PMIC_OTP0_44_W_UNION;
#endif
#define SUB_PMIC_OTP0_44_W_otp0_pdob44_w_START (0)
#define SUB_PMIC_OTP0_44_W_otp0_pdob44_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob45_w : 8;
    } reg;
} SUB_PMIC_OTP0_45_W_UNION;
#endif
#define SUB_PMIC_OTP0_45_W_otp0_pdob45_w_START (0)
#define SUB_PMIC_OTP0_45_W_otp0_pdob45_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob46_w : 8;
    } reg;
} SUB_PMIC_OTP0_46_W_UNION;
#endif
#define SUB_PMIC_OTP0_46_W_otp0_pdob46_w_START (0)
#define SUB_PMIC_OTP0_46_W_otp0_pdob46_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob47_w : 8;
    } reg;
} SUB_PMIC_OTP0_47_W_UNION;
#endif
#define SUB_PMIC_OTP0_47_W_otp0_pdob47_w_START (0)
#define SUB_PMIC_OTP0_47_W_otp0_pdob47_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob48_w : 8;
    } reg;
} SUB_PMIC_OTP0_48_W_UNION;
#endif
#define SUB_PMIC_OTP0_48_W_otp0_pdob48_w_START (0)
#define SUB_PMIC_OTP0_48_W_otp0_pdob48_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob49_w : 8;
    } reg;
} SUB_PMIC_OTP0_49_W_UNION;
#endif
#define SUB_PMIC_OTP0_49_W_otp0_pdob49_w_START (0)
#define SUB_PMIC_OTP0_49_W_otp0_pdob49_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob50_w : 8;
    } reg;
} SUB_PMIC_OTP0_50_W_UNION;
#endif
#define SUB_PMIC_OTP0_50_W_otp0_pdob50_w_START (0)
#define SUB_PMIC_OTP0_50_W_otp0_pdob50_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob51_w : 8;
    } reg;
} SUB_PMIC_OTP0_51_W_UNION;
#endif
#define SUB_PMIC_OTP0_51_W_otp0_pdob51_w_START (0)
#define SUB_PMIC_OTP0_51_W_otp0_pdob51_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob52_w : 8;
    } reg;
} SUB_PMIC_OTP0_52_W_UNION;
#endif
#define SUB_PMIC_OTP0_52_W_otp0_pdob52_w_START (0)
#define SUB_PMIC_OTP0_52_W_otp0_pdob52_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob53_w : 8;
    } reg;
} SUB_PMIC_OTP0_53_W_UNION;
#endif
#define SUB_PMIC_OTP0_53_W_otp0_pdob53_w_START (0)
#define SUB_PMIC_OTP0_53_W_otp0_pdob53_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob54_w : 8;
    } reg;
} SUB_PMIC_OTP0_54_W_UNION;
#endif
#define SUB_PMIC_OTP0_54_W_otp0_pdob54_w_START (0)
#define SUB_PMIC_OTP0_54_W_otp0_pdob54_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob55_w : 8;
    } reg;
} SUB_PMIC_OTP0_55_W_UNION;
#endif
#define SUB_PMIC_OTP0_55_W_otp0_pdob55_w_START (0)
#define SUB_PMIC_OTP0_55_W_otp0_pdob55_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob56_w : 8;
    } reg;
} SUB_PMIC_OTP0_56_W_UNION;
#endif
#define SUB_PMIC_OTP0_56_W_otp0_pdob56_w_START (0)
#define SUB_PMIC_OTP0_56_W_otp0_pdob56_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob57_w : 8;
    } reg;
} SUB_PMIC_OTP0_57_W_UNION;
#endif
#define SUB_PMIC_OTP0_57_W_otp0_pdob57_w_START (0)
#define SUB_PMIC_OTP0_57_W_otp0_pdob57_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob58_w : 8;
    } reg;
} SUB_PMIC_OTP0_58_W_UNION;
#endif
#define SUB_PMIC_OTP0_58_W_otp0_pdob58_w_START (0)
#define SUB_PMIC_OTP0_58_W_otp0_pdob58_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob59_w : 8;
    } reg;
} SUB_PMIC_OTP0_59_W_UNION;
#endif
#define SUB_PMIC_OTP0_59_W_otp0_pdob59_w_START (0)
#define SUB_PMIC_OTP0_59_W_otp0_pdob59_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob60_w : 8;
    } reg;
} SUB_PMIC_OTP0_60_W_UNION;
#endif
#define SUB_PMIC_OTP0_60_W_otp0_pdob60_w_START (0)
#define SUB_PMIC_OTP0_60_W_otp0_pdob60_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob61_w : 8;
    } reg;
} SUB_PMIC_OTP0_61_W_UNION;
#endif
#define SUB_PMIC_OTP0_61_W_otp0_pdob61_w_START (0)
#define SUB_PMIC_OTP0_61_W_otp0_pdob61_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob62_w : 8;
    } reg;
} SUB_PMIC_OTP0_62_W_UNION;
#endif
#define SUB_PMIC_OTP0_62_W_otp0_pdob62_w_START (0)
#define SUB_PMIC_OTP0_62_W_otp0_pdob62_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob63_w : 8;
    } reg;
} SUB_PMIC_OTP0_63_W_UNION;
#endif
#define SUB_PMIC_OTP0_63_W_otp0_pdob63_w_START (0)
#define SUB_PMIC_OTP0_63_W_otp0_pdob63_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob0 : 8;
    } reg;
} SUB_PMIC_OTP1_0_UNION;
#endif
#define SUB_PMIC_OTP1_0_otp1_pdob0_START (0)
#define SUB_PMIC_OTP1_0_otp1_pdob0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob1 : 8;
    } reg;
} SUB_PMIC_OTP1_1_UNION;
#endif
#define SUB_PMIC_OTP1_1_otp1_pdob1_START (0)
#define SUB_PMIC_OTP1_1_otp1_pdob1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pwe_int : 1;
        unsigned char otp1_pwe_pulse : 1;
        unsigned char otp1_por_int : 1;
        unsigned char otp1_por_pulse : 1;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_OTP1_CTRL0_UNION;
#endif
#define SUB_PMIC_OTP1_CTRL0_otp1_pwe_int_START (0)
#define SUB_PMIC_OTP1_CTRL0_otp1_pwe_int_END (0)
#define SUB_PMIC_OTP1_CTRL0_otp1_pwe_pulse_START (1)
#define SUB_PMIC_OTP1_CTRL0_otp1_pwe_pulse_END (1)
#define SUB_PMIC_OTP1_CTRL0_otp1_por_int_START (2)
#define SUB_PMIC_OTP1_CTRL0_otp1_por_int_END (2)
#define SUB_PMIC_OTP1_CTRL0_otp1_por_pulse_START (3)
#define SUB_PMIC_OTP1_CTRL0_otp1_por_pulse_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pprog : 1;
        unsigned char otp1_inctrl_sel : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_OTP1_CTRL1_UNION;
#endif
#define SUB_PMIC_OTP1_CTRL1_otp1_pprog_START (0)
#define SUB_PMIC_OTP1_CTRL1_otp1_pprog_END (0)
#define SUB_PMIC_OTP1_CTRL1_otp1_inctrl_sel_START (1)
#define SUB_PMIC_OTP1_CTRL1_otp1_inctrl_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pa_cfg : 6;
        unsigned char otp1_ptm : 2;
    } reg;
} SUB_PMIC_OTP1_CTRL2_UNION;
#endif
#define SUB_PMIC_OTP1_CTRL2_otp1_pa_cfg_START (0)
#define SUB_PMIC_OTP1_CTRL2_otp1_pa_cfg_END (5)
#define SUB_PMIC_OTP1_CTRL2_otp1_ptm_START (6)
#define SUB_PMIC_OTP1_CTRL2_otp1_ptm_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdin : 8;
    } reg;
} SUB_PMIC_OTP1_WDATA_UNION;
#endif
#define SUB_PMIC_OTP1_WDATA_otp1_pdin_START (0)
#define SUB_PMIC_OTP1_WDATA_otp1_pdin_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob1_w : 8;
    } reg;
} SUB_PMIC_OTP1_1_W_UNION;
#endif
#define SUB_PMIC_OTP1_1_W_otp1_pdob1_w_START (0)
#define SUB_PMIC_OTP1_1_W_otp1_pdob1_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob2_w : 8;
    } reg;
} SUB_PMIC_OTP1_2_W_UNION;
#endif
#define SUB_PMIC_OTP1_2_W_otp1_pdob2_w_START (0)
#define SUB_PMIC_OTP1_2_W_otp1_pdob2_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob3_w : 8;
    } reg;
} SUB_PMIC_OTP1_3_W_UNION;
#endif
#define SUB_PMIC_OTP1_3_W_otp1_pdob3_w_START (0)
#define SUB_PMIC_OTP1_3_W_otp1_pdob3_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob4_w : 8;
    } reg;
} SUB_PMIC_OTP1_4_W_UNION;
#endif
#define SUB_PMIC_OTP1_4_W_otp1_pdob4_w_START (0)
#define SUB_PMIC_OTP1_4_W_otp1_pdob4_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob5_w : 8;
    } reg;
} SUB_PMIC_OTP1_5_W_UNION;
#endif
#define SUB_PMIC_OTP1_5_W_otp1_pdob5_w_START (0)
#define SUB_PMIC_OTP1_5_W_otp1_pdob5_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob26_w : 8;
    } reg;
} SUB_PMIC_OTP1_26_W_UNION;
#endif
#define SUB_PMIC_OTP1_26_W_otp1_pdob26_w_START (0)
#define SUB_PMIC_OTP1_26_W_otp1_pdob26_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob27_w : 8;
    } reg;
} SUB_PMIC_OTP1_27_W_UNION;
#endif
#define SUB_PMIC_OTP1_27_W_otp1_pdob27_w_START (0)
#define SUB_PMIC_OTP1_27_W_otp1_pdob27_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob28_w : 8;
    } reg;
} SUB_PMIC_OTP1_28_W_UNION;
#endif
#define SUB_PMIC_OTP1_28_W_otp1_pdob28_w_START (0)
#define SUB_PMIC_OTP1_28_W_otp1_pdob28_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob29_w : 8;
    } reg;
} SUB_PMIC_OTP1_29_W_UNION;
#endif
#define SUB_PMIC_OTP1_29_W_otp1_pdob29_w_START (0)
#define SUB_PMIC_OTP1_29_W_otp1_pdob29_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob30_w : 8;
    } reg;
} SUB_PMIC_OTP1_30_W_UNION;
#endif
#define SUB_PMIC_OTP1_30_W_otp1_pdob30_w_START (0)
#define SUB_PMIC_OTP1_30_W_otp1_pdob30_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob31_w : 8;
    } reg;
} SUB_PMIC_OTP1_31_W_UNION;
#endif
#define SUB_PMIC_OTP1_31_W_otp1_pdob31_w_START (0)
#define SUB_PMIC_OTP1_31_W_otp1_pdob31_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob32_w : 8;
    } reg;
} SUB_PMIC_OTP1_32_W_UNION;
#endif
#define SUB_PMIC_OTP1_32_W_otp1_pdob32_w_START (0)
#define SUB_PMIC_OTP1_32_W_otp1_pdob32_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob33_w : 8;
    } reg;
} SUB_PMIC_OTP1_33_W_UNION;
#endif
#define SUB_PMIC_OTP1_33_W_otp1_pdob33_w_START (0)
#define SUB_PMIC_OTP1_33_W_otp1_pdob33_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_thsd_otmp125_d1mr_mk : 1;
        unsigned char irq_ocp_scp_ovp_r_mk : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_IRQ_MASK0_UNION;
#endif
#define SUB_PMIC_IRQ_MASK0_irq_thsd_otmp125_d1mr_mk_START (0)
#define SUB_PMIC_IRQ_MASK0_irq_thsd_otmp125_d1mr_mk_END (0)
#define SUB_PMIC_IRQ_MASK0_irq_ocp_scp_ovp_r_mk_START (1)
#define SUB_PMIC_IRQ_MASK0_irq_ocp_scp_ovp_r_mk_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_b31_ocp_mk : 1;
        unsigned char irq_b30_ocp_mk : 1;
        unsigned char irq_b21_ocp_mk : 1;
        unsigned char irq_b20_ocp_mk : 1;
        unsigned char irq_b11_ocp_mk : 1;
        unsigned char irq_b10_ocp_mk : 1;
        unsigned char irq_b01_ocp_mk : 1;
        unsigned char irq_b00_ocp_mk : 1;
    } reg;
} SUB_PMIC_IRQ_OCP_MASK0_UNION;
#endif
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b31_ocp_mk_START (0)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b31_ocp_mk_END (0)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b30_ocp_mk_START (1)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b30_ocp_mk_END (1)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b21_ocp_mk_START (2)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b21_ocp_mk_END (2)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b20_ocp_mk_START (3)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b20_ocp_mk_END (3)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b11_ocp_mk_START (4)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b11_ocp_mk_END (4)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b10_ocp_mk_START (5)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b10_ocp_mk_END (5)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b01_ocp_mk_START (6)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b01_ocp_mk_END (6)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b00_ocp_mk_START (7)
#define SUB_PMIC_IRQ_OCP_MASK0_irq_b00_ocp_mk_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_b13_ocp_mk : 1;
        unsigned char irq_b12_ocp_mk : 1;
        unsigned char irq_b8_ocp_mk : 1;
        unsigned char irq_b7_ocp_mk : 1;
        unsigned char irq_b6_ocp_mk : 1;
        unsigned char irq_pmuh_ocp_mk : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_IRQ_OCP_MASK1_UNION;
#endif
#define SUB_PMIC_IRQ_OCP_MASK1_irq_b13_ocp_mk_START (0)
#define SUB_PMIC_IRQ_OCP_MASK1_irq_b13_ocp_mk_END (0)
#define SUB_PMIC_IRQ_OCP_MASK1_irq_b12_ocp_mk_START (1)
#define SUB_PMIC_IRQ_OCP_MASK1_irq_b12_ocp_mk_END (1)
#define SUB_PMIC_IRQ_OCP_MASK1_irq_b8_ocp_mk_START (2)
#define SUB_PMIC_IRQ_OCP_MASK1_irq_b8_ocp_mk_END (2)
#define SUB_PMIC_IRQ_OCP_MASK1_irq_b7_ocp_mk_START (3)
#define SUB_PMIC_IRQ_OCP_MASK1_irq_b7_ocp_mk_END (3)
#define SUB_PMIC_IRQ_OCP_MASK1_irq_b6_ocp_mk_START (4)
#define SUB_PMIC_IRQ_OCP_MASK1_irq_b6_ocp_mk_END (4)
#define SUB_PMIC_IRQ_OCP_MASK1_irq_pmuh_ocp_mk_START (5)
#define SUB_PMIC_IRQ_OCP_MASK1_irq_pmuh_ocp_mk_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_ldo54_ocp_mk : 1;
        unsigned char irq_ldo53_ocp_mk : 1;
        unsigned char irq_ldo52_ocp_mk : 1;
        unsigned char irq_ldo51_ocp_mk : 1;
        unsigned char irq_ldo50_ocp_mk : 1;
        unsigned char irq_ldo34_ocp_mk : 1;
        unsigned char irq_ldo9_ocp_mk : 1;
        unsigned char irq_sw10_ocp_mk : 1;
    } reg;
} SUB_PMIC_IRQ_OCP_MASK2_UNION;
#endif
#define SUB_PMIC_IRQ_OCP_MASK2_irq_ldo54_ocp_mk_START (0)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_ldo54_ocp_mk_END (0)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_ldo53_ocp_mk_START (1)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_ldo53_ocp_mk_END (1)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_ldo52_ocp_mk_START (2)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_ldo52_ocp_mk_END (2)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_ldo51_ocp_mk_START (3)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_ldo51_ocp_mk_END (3)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_ldo50_ocp_mk_START (4)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_ldo50_ocp_mk_END (4)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_ldo34_ocp_mk_START (5)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_ldo34_ocp_mk_END (5)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_ldo9_ocp_mk_START (6)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_ldo9_ocp_mk_END (6)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_sw10_ocp_mk_START (7)
#define SUB_PMIC_IRQ_OCP_MASK2_irq_sw10_ocp_mk_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_b30_scp_mk : 1;
        unsigned char irq_b20_scp_mk : 1;
        unsigned char irq_b10_scp_mk : 1;
        unsigned char irq_b00_scp_mk : 1;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_IRQ_SCP_MASK0_UNION;
#endif
#define SUB_PMIC_IRQ_SCP_MASK0_irq_b30_scp_mk_START (0)
#define SUB_PMIC_IRQ_SCP_MASK0_irq_b30_scp_mk_END (0)
#define SUB_PMIC_IRQ_SCP_MASK0_irq_b20_scp_mk_START (1)
#define SUB_PMIC_IRQ_SCP_MASK0_irq_b20_scp_mk_END (1)
#define SUB_PMIC_IRQ_SCP_MASK0_irq_b10_scp_mk_START (2)
#define SUB_PMIC_IRQ_SCP_MASK0_irq_b10_scp_mk_END (2)
#define SUB_PMIC_IRQ_SCP_MASK0_irq_b00_scp_mk_START (3)
#define SUB_PMIC_IRQ_SCP_MASK0_irq_b00_scp_mk_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_b13_scp_mk : 1;
        unsigned char irq_b12_scp_mk : 1;
        unsigned char irq_b8_scp_mk : 1;
        unsigned char irq_b7_scp_mk : 1;
        unsigned char irq_b6_scp_mk : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IRQ_SCP_MASK1_UNION;
#endif
#define SUB_PMIC_IRQ_SCP_MASK1_irq_b13_scp_mk_START (0)
#define SUB_PMIC_IRQ_SCP_MASK1_irq_b13_scp_mk_END (0)
#define SUB_PMIC_IRQ_SCP_MASK1_irq_b12_scp_mk_START (1)
#define SUB_PMIC_IRQ_SCP_MASK1_irq_b12_scp_mk_END (1)
#define SUB_PMIC_IRQ_SCP_MASK1_irq_b8_scp_mk_START (2)
#define SUB_PMIC_IRQ_SCP_MASK1_irq_b8_scp_mk_END (2)
#define SUB_PMIC_IRQ_SCP_MASK1_irq_b7_scp_mk_START (3)
#define SUB_PMIC_IRQ_SCP_MASK1_irq_b7_scp_mk_END (3)
#define SUB_PMIC_IRQ_SCP_MASK1_irq_b6_scp_mk_START (4)
#define SUB_PMIC_IRQ_SCP_MASK1_irq_b6_scp_mk_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_b30_ovp_mk : 1;
        unsigned char irq_b20_ovp_mk : 1;
        unsigned char irq_b10_ovp_mk : 1;
        unsigned char irq_b00_ovp_mk : 1;
        unsigned char irq_b12_ovp_mk : 1;
        unsigned char irq_b8_ovp_mk : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_IRQ_OVP_MASK_UNION;
#endif
#define SUB_PMIC_IRQ_OVP_MASK_irq_b30_ovp_mk_START (0)
#define SUB_PMIC_IRQ_OVP_MASK_irq_b30_ovp_mk_END (0)
#define SUB_PMIC_IRQ_OVP_MASK_irq_b20_ovp_mk_START (1)
#define SUB_PMIC_IRQ_OVP_MASK_irq_b20_ovp_mk_END (1)
#define SUB_PMIC_IRQ_OVP_MASK_irq_b10_ovp_mk_START (2)
#define SUB_PMIC_IRQ_OVP_MASK_irq_b10_ovp_mk_END (2)
#define SUB_PMIC_IRQ_OVP_MASK_irq_b00_ovp_mk_START (3)
#define SUB_PMIC_IRQ_OVP_MASK_irq_b00_ovp_mk_END (3)
#define SUB_PMIC_IRQ_OVP_MASK_irq_b12_ovp_mk_START (4)
#define SUB_PMIC_IRQ_OVP_MASK_irq_b12_ovp_mk_END (4)
#define SUB_PMIC_IRQ_OVP_MASK_irq_b8_ovp_mk_START (5)
#define SUB_PMIC_IRQ_OVP_MASK_irq_b8_ovp_mk_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_b30_cur_mk : 1;
        unsigned char irq_b20_cur_mk : 1;
        unsigned char irq_b10_cur_mk : 1;
        unsigned char irq_b00_cur_mk : 1;
        unsigned char irq_b12_cur_mk : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IRQ_CUR_DET_MASK_UNION;
#endif
#define SUB_PMIC_IRQ_CUR_DET_MASK_irq_b30_cur_mk_START (0)
#define SUB_PMIC_IRQ_CUR_DET_MASK_irq_b30_cur_mk_END (0)
#define SUB_PMIC_IRQ_CUR_DET_MASK_irq_b20_cur_mk_START (1)
#define SUB_PMIC_IRQ_CUR_DET_MASK_irq_b20_cur_mk_END (1)
#define SUB_PMIC_IRQ_CUR_DET_MASK_irq_b10_cur_mk_START (2)
#define SUB_PMIC_IRQ_CUR_DET_MASK_irq_b10_cur_mk_END (2)
#define SUB_PMIC_IRQ_CUR_DET_MASK_irq_b00_cur_mk_START (3)
#define SUB_PMIC_IRQ_CUR_DET_MASK_irq_b00_cur_mk_END (3)
#define SUB_PMIC_IRQ_CUR_DET_MASK_irq_b12_cur_mk_START (4)
#define SUB_PMIC_IRQ_CUR_DET_MASK_irq_b12_cur_mk_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_sim_gpio_mk : 1;
        unsigned char irq_sim_hpd_r_mk : 1;
        unsigned char irq_sim_hpd_f_mk : 1;
        unsigned char irq_gpio0_mk : 1;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_IRQ_SIM_GPIO_MASK_UNION;
#endif
#define SUB_PMIC_IRQ_SIM_GPIO_MASK_irq_sim_gpio_mk_START (0)
#define SUB_PMIC_IRQ_SIM_GPIO_MASK_irq_sim_gpio_mk_END (0)
#define SUB_PMIC_IRQ_SIM_GPIO_MASK_irq_sim_hpd_r_mk_START (1)
#define SUB_PMIC_IRQ_SIM_GPIO_MASK_irq_sim_hpd_r_mk_END (1)
#define SUB_PMIC_IRQ_SIM_GPIO_MASK_irq_sim_hpd_f_mk_START (2)
#define SUB_PMIC_IRQ_SIM_GPIO_MASK_irq_sim_hpd_f_mk_END (2)
#define SUB_PMIC_IRQ_SIM_GPIO_MASK_irq_gpio0_mk_START (3)
#define SUB_PMIC_IRQ_SIM_GPIO_MASK_irq_gpio0_mk_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_b7_ramp_abn_mk : 1;
        unsigned char irq_b6_ramp_abn_mk : 1;
        unsigned char irq_b12_ramp_abn_mk : 1;
        unsigned char irq_b30_ramp_abn_mk : 1;
        unsigned char irq_b20_ramp_abn_mk : 1;
        unsigned char irq_b10_ramp_abn_mk : 1;
        unsigned char irq_b00_ramp_abn_mk : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_IRQ_BUCK_RAMP_MASK_UNION;
#endif
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_irq_b7_ramp_abn_mk_START (0)
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_irq_b7_ramp_abn_mk_END (0)
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_irq_b6_ramp_abn_mk_START (1)
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_irq_b6_ramp_abn_mk_END (1)
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_irq_b12_ramp_abn_mk_START (2)
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_irq_b12_ramp_abn_mk_END (2)
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_irq_b30_ramp_abn_mk_START (3)
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_irq_b30_ramp_abn_mk_END (3)
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_irq_b20_ramp_abn_mk_START (4)
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_irq_b20_ramp_abn_mk_END (4)
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_irq_b10_ramp_abn_mk_START (5)
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_irq_b10_ramp_abn_mk_END (5)
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_irq_b00_ramp_abn_mk_START (6)
#define SUB_PMIC_IRQ_BUCK_RAMP_MASK_irq_b00_ramp_abn_mk_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_lra_ocp_mk : 1;
        unsigned char irq_lra_init_abn_mk : 1;
        unsigned char irq_lra_vsys_uvp_mk : 1;
        unsigned char irq_lra_vsys_ovp_mk : 1;
        unsigned char irq_lra_adc_err_mk : 1;
        unsigned char irq_lra_acct_off_mk : 1;
        unsigned char irq_erm_bemf_abn_mk : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_IRQ_LRA_MASK_UNION;
#endif
#define SUB_PMIC_IRQ_LRA_MASK_irq_lra_ocp_mk_START (0)
#define SUB_PMIC_IRQ_LRA_MASK_irq_lra_ocp_mk_END (0)
#define SUB_PMIC_IRQ_LRA_MASK_irq_lra_init_abn_mk_START (1)
#define SUB_PMIC_IRQ_LRA_MASK_irq_lra_init_abn_mk_END (1)
#define SUB_PMIC_IRQ_LRA_MASK_irq_lra_vsys_uvp_mk_START (2)
#define SUB_PMIC_IRQ_LRA_MASK_irq_lra_vsys_uvp_mk_END (2)
#define SUB_PMIC_IRQ_LRA_MASK_irq_lra_vsys_ovp_mk_START (3)
#define SUB_PMIC_IRQ_LRA_MASK_irq_lra_vsys_ovp_mk_END (3)
#define SUB_PMIC_IRQ_LRA_MASK_irq_lra_adc_err_mk_START (4)
#define SUB_PMIC_IRQ_LRA_MASK_irq_lra_adc_err_mk_END (4)
#define SUB_PMIC_IRQ_LRA_MASK_irq_lra_acct_off_mk_START (5)
#define SUB_PMIC_IRQ_LRA_MASK_irq_lra_acct_off_mk_END (5)
#define SUB_PMIC_IRQ_LRA_MASK_irq_erm_bemf_abn_mk_START (6)
#define SUB_PMIC_IRQ_LRA_MASK_irq_erm_bemf_abn_mk_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_thsd_otmp125_d1mr : 1;
        unsigned char irq_ocp_scp_ovp_r : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_IRQ0_UNION;
#endif
#define SUB_PMIC_IRQ0_irq_thsd_otmp125_d1mr_START (0)
#define SUB_PMIC_IRQ0_irq_thsd_otmp125_d1mr_END (0)
#define SUB_PMIC_IRQ0_irq_ocp_scp_ovp_r_START (1)
#define SUB_PMIC_IRQ0_irq_ocp_scp_ovp_r_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_b31_ocp : 1;
        unsigned char irq_b30_ocp : 1;
        unsigned char irq_b21_ocp : 1;
        unsigned char irq_b20_ocp : 1;
        unsigned char irq_b11_ocp : 1;
        unsigned char irq_b10_ocp : 1;
        unsigned char irq_b01_ocp : 1;
        unsigned char irq_b00_ocp : 1;
    } reg;
} SUB_PMIC_IRQ_OCP0_UNION;
#endif
#define SUB_PMIC_IRQ_OCP0_irq_b31_ocp_START (0)
#define SUB_PMIC_IRQ_OCP0_irq_b31_ocp_END (0)
#define SUB_PMIC_IRQ_OCP0_irq_b30_ocp_START (1)
#define SUB_PMIC_IRQ_OCP0_irq_b30_ocp_END (1)
#define SUB_PMIC_IRQ_OCP0_irq_b21_ocp_START (2)
#define SUB_PMIC_IRQ_OCP0_irq_b21_ocp_END (2)
#define SUB_PMIC_IRQ_OCP0_irq_b20_ocp_START (3)
#define SUB_PMIC_IRQ_OCP0_irq_b20_ocp_END (3)
#define SUB_PMIC_IRQ_OCP0_irq_b11_ocp_START (4)
#define SUB_PMIC_IRQ_OCP0_irq_b11_ocp_END (4)
#define SUB_PMIC_IRQ_OCP0_irq_b10_ocp_START (5)
#define SUB_PMIC_IRQ_OCP0_irq_b10_ocp_END (5)
#define SUB_PMIC_IRQ_OCP0_irq_b01_ocp_START (6)
#define SUB_PMIC_IRQ_OCP0_irq_b01_ocp_END (6)
#define SUB_PMIC_IRQ_OCP0_irq_b00_ocp_START (7)
#define SUB_PMIC_IRQ_OCP0_irq_b00_ocp_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_b13_ocp : 1;
        unsigned char irq_b12_ocp : 1;
        unsigned char irq_b8_ocp : 1;
        unsigned char irq_b7_ocp : 1;
        unsigned char irq_b6_ocp : 1;
        unsigned char irq_pmuh_ocp : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_IRQ_OCP1_UNION;
#endif
#define SUB_PMIC_IRQ_OCP1_irq_b13_ocp_START (0)
#define SUB_PMIC_IRQ_OCP1_irq_b13_ocp_END (0)
#define SUB_PMIC_IRQ_OCP1_irq_b12_ocp_START (1)
#define SUB_PMIC_IRQ_OCP1_irq_b12_ocp_END (1)
#define SUB_PMIC_IRQ_OCP1_irq_b8_ocp_START (2)
#define SUB_PMIC_IRQ_OCP1_irq_b8_ocp_END (2)
#define SUB_PMIC_IRQ_OCP1_irq_b7_ocp_START (3)
#define SUB_PMIC_IRQ_OCP1_irq_b7_ocp_END (3)
#define SUB_PMIC_IRQ_OCP1_irq_b6_ocp_START (4)
#define SUB_PMIC_IRQ_OCP1_irq_b6_ocp_END (4)
#define SUB_PMIC_IRQ_OCP1_irq_pmuh_ocp_START (5)
#define SUB_PMIC_IRQ_OCP1_irq_pmuh_ocp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_ldo54_ocp : 1;
        unsigned char irq_ldo53_ocp : 1;
        unsigned char irq_ldo52_ocp : 1;
        unsigned char irq_ldo51_ocp : 1;
        unsigned char irq_ldo50_ocp : 1;
        unsigned char irq_ldo34_ocp : 1;
        unsigned char irq_ldo9_ocp : 1;
        unsigned char irq_sw10_ocp : 1;
    } reg;
} SUB_PMIC_IRQ_OCP2_UNION;
#endif
#define SUB_PMIC_IRQ_OCP2_irq_ldo54_ocp_START (0)
#define SUB_PMIC_IRQ_OCP2_irq_ldo54_ocp_END (0)
#define SUB_PMIC_IRQ_OCP2_irq_ldo53_ocp_START (1)
#define SUB_PMIC_IRQ_OCP2_irq_ldo53_ocp_END (1)
#define SUB_PMIC_IRQ_OCP2_irq_ldo52_ocp_START (2)
#define SUB_PMIC_IRQ_OCP2_irq_ldo52_ocp_END (2)
#define SUB_PMIC_IRQ_OCP2_irq_ldo51_ocp_START (3)
#define SUB_PMIC_IRQ_OCP2_irq_ldo51_ocp_END (3)
#define SUB_PMIC_IRQ_OCP2_irq_ldo50_ocp_START (4)
#define SUB_PMIC_IRQ_OCP2_irq_ldo50_ocp_END (4)
#define SUB_PMIC_IRQ_OCP2_irq_ldo34_ocp_START (5)
#define SUB_PMIC_IRQ_OCP2_irq_ldo34_ocp_END (5)
#define SUB_PMIC_IRQ_OCP2_irq_ldo9_ocp_START (6)
#define SUB_PMIC_IRQ_OCP2_irq_ldo9_ocp_END (6)
#define SUB_PMIC_IRQ_OCP2_irq_sw10_ocp_START (7)
#define SUB_PMIC_IRQ_OCP2_irq_sw10_ocp_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_b30_scp : 1;
        unsigned char irq_b20_scp : 1;
        unsigned char irq_b10_scp : 1;
        unsigned char irq_b00_scp : 1;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_IRQ_SCP0_UNION;
#endif
#define SUB_PMIC_IRQ_SCP0_irq_b30_scp_START (0)
#define SUB_PMIC_IRQ_SCP0_irq_b30_scp_END (0)
#define SUB_PMIC_IRQ_SCP0_irq_b20_scp_START (1)
#define SUB_PMIC_IRQ_SCP0_irq_b20_scp_END (1)
#define SUB_PMIC_IRQ_SCP0_irq_b10_scp_START (2)
#define SUB_PMIC_IRQ_SCP0_irq_b10_scp_END (2)
#define SUB_PMIC_IRQ_SCP0_irq_b00_scp_START (3)
#define SUB_PMIC_IRQ_SCP0_irq_b00_scp_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_b13_scp : 1;
        unsigned char irq_b12_scp : 1;
        unsigned char irq_b8_scp : 1;
        unsigned char irq_b7_scp : 1;
        unsigned char irq_b6_scp : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IRQ_SCP1_UNION;
#endif
#define SUB_PMIC_IRQ_SCP1_irq_b13_scp_START (0)
#define SUB_PMIC_IRQ_SCP1_irq_b13_scp_END (0)
#define SUB_PMIC_IRQ_SCP1_irq_b12_scp_START (1)
#define SUB_PMIC_IRQ_SCP1_irq_b12_scp_END (1)
#define SUB_PMIC_IRQ_SCP1_irq_b8_scp_START (2)
#define SUB_PMIC_IRQ_SCP1_irq_b8_scp_END (2)
#define SUB_PMIC_IRQ_SCP1_irq_b7_scp_START (3)
#define SUB_PMIC_IRQ_SCP1_irq_b7_scp_END (3)
#define SUB_PMIC_IRQ_SCP1_irq_b6_scp_START (4)
#define SUB_PMIC_IRQ_SCP1_irq_b6_scp_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_b30_ovp : 1;
        unsigned char irq_b20_ovp : 1;
        unsigned char irq_b10_ovp : 1;
        unsigned char irq_b00_ovp : 1;
        unsigned char irq_b12_ovp : 1;
        unsigned char irq_b8_ovp : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_IRQ_OVP_UNION;
#endif
#define SUB_PMIC_IRQ_OVP_irq_b30_ovp_START (0)
#define SUB_PMIC_IRQ_OVP_irq_b30_ovp_END (0)
#define SUB_PMIC_IRQ_OVP_irq_b20_ovp_START (1)
#define SUB_PMIC_IRQ_OVP_irq_b20_ovp_END (1)
#define SUB_PMIC_IRQ_OVP_irq_b10_ovp_START (2)
#define SUB_PMIC_IRQ_OVP_irq_b10_ovp_END (2)
#define SUB_PMIC_IRQ_OVP_irq_b00_ovp_START (3)
#define SUB_PMIC_IRQ_OVP_irq_b00_ovp_END (3)
#define SUB_PMIC_IRQ_OVP_irq_b12_ovp_START (4)
#define SUB_PMIC_IRQ_OVP_irq_b12_ovp_END (4)
#define SUB_PMIC_IRQ_OVP_irq_b8_ovp_START (5)
#define SUB_PMIC_IRQ_OVP_irq_b8_ovp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_b30_cur : 1;
        unsigned char irq_b20_cur : 1;
        unsigned char irq_b10_cur : 1;
        unsigned char irq_b00_cur : 1;
        unsigned char irq_b12_cur : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_IRQ_CUR_DET_UNION;
#endif
#define SUB_PMIC_IRQ_CUR_DET_irq_b30_cur_START (0)
#define SUB_PMIC_IRQ_CUR_DET_irq_b30_cur_END (0)
#define SUB_PMIC_IRQ_CUR_DET_irq_b20_cur_START (1)
#define SUB_PMIC_IRQ_CUR_DET_irq_b20_cur_END (1)
#define SUB_PMIC_IRQ_CUR_DET_irq_b10_cur_START (2)
#define SUB_PMIC_IRQ_CUR_DET_irq_b10_cur_END (2)
#define SUB_PMIC_IRQ_CUR_DET_irq_b00_cur_START (3)
#define SUB_PMIC_IRQ_CUR_DET_irq_b00_cur_END (3)
#define SUB_PMIC_IRQ_CUR_DET_irq_b12_cur_START (4)
#define SUB_PMIC_IRQ_CUR_DET_irq_b12_cur_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_sim_gpio : 1;
        unsigned char irq_sim_hpd_r : 1;
        unsigned char irq_sim_hpd_f : 1;
        unsigned char irq_gpio0 : 1;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_IRQ_SIM_GPIO_UNION;
#endif
#define SUB_PMIC_IRQ_SIM_GPIO_irq_sim_gpio_START (0)
#define SUB_PMIC_IRQ_SIM_GPIO_irq_sim_gpio_END (0)
#define SUB_PMIC_IRQ_SIM_GPIO_irq_sim_hpd_r_START (1)
#define SUB_PMIC_IRQ_SIM_GPIO_irq_sim_hpd_r_END (1)
#define SUB_PMIC_IRQ_SIM_GPIO_irq_sim_hpd_f_START (2)
#define SUB_PMIC_IRQ_SIM_GPIO_irq_sim_hpd_f_END (2)
#define SUB_PMIC_IRQ_SIM_GPIO_irq_gpio0_START (3)
#define SUB_PMIC_IRQ_SIM_GPIO_irq_gpio0_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_b7_ramp_abn : 1;
        unsigned char irq_b6_ramp_abn : 1;
        unsigned char irq_b12_ramp_abn : 1;
        unsigned char irq_b30_ramp_abn : 1;
        unsigned char irq_b20_ramp_abn : 1;
        unsigned char irq_b10_ramp_abn : 1;
        unsigned char irq_b00_ramp_abn : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_IRQ_BUCK_RAMP_UNION;
#endif
#define SUB_PMIC_IRQ_BUCK_RAMP_irq_b7_ramp_abn_START (0)
#define SUB_PMIC_IRQ_BUCK_RAMP_irq_b7_ramp_abn_END (0)
#define SUB_PMIC_IRQ_BUCK_RAMP_irq_b6_ramp_abn_START (1)
#define SUB_PMIC_IRQ_BUCK_RAMP_irq_b6_ramp_abn_END (1)
#define SUB_PMIC_IRQ_BUCK_RAMP_irq_b12_ramp_abn_START (2)
#define SUB_PMIC_IRQ_BUCK_RAMP_irq_b12_ramp_abn_END (2)
#define SUB_PMIC_IRQ_BUCK_RAMP_irq_b30_ramp_abn_START (3)
#define SUB_PMIC_IRQ_BUCK_RAMP_irq_b30_ramp_abn_END (3)
#define SUB_PMIC_IRQ_BUCK_RAMP_irq_b20_ramp_abn_START (4)
#define SUB_PMIC_IRQ_BUCK_RAMP_irq_b20_ramp_abn_END (4)
#define SUB_PMIC_IRQ_BUCK_RAMP_irq_b10_ramp_abn_START (5)
#define SUB_PMIC_IRQ_BUCK_RAMP_irq_b10_ramp_abn_END (5)
#define SUB_PMIC_IRQ_BUCK_RAMP_irq_b00_ramp_abn_START (6)
#define SUB_PMIC_IRQ_BUCK_RAMP_irq_b00_ramp_abn_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_lra_ocp : 1;
        unsigned char irq_lra_init_abn : 1;
        unsigned char irq_lra_vsys_uvp : 1;
        unsigned char irq_lra_vsys_ovp : 1;
        unsigned char irq_lra_adc_err : 1;
        unsigned char irq_lra_acct_off : 1;
        unsigned char irq_erm_bemf_abn : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_IRQ_LRA_UNION;
#endif
#define SUB_PMIC_IRQ_LRA_irq_lra_ocp_START (0)
#define SUB_PMIC_IRQ_LRA_irq_lra_ocp_END (0)
#define SUB_PMIC_IRQ_LRA_irq_lra_init_abn_START (1)
#define SUB_PMIC_IRQ_LRA_irq_lra_init_abn_END (1)
#define SUB_PMIC_IRQ_LRA_irq_lra_vsys_uvp_START (2)
#define SUB_PMIC_IRQ_LRA_irq_lra_vsys_uvp_END (2)
#define SUB_PMIC_IRQ_LRA_irq_lra_vsys_ovp_START (3)
#define SUB_PMIC_IRQ_LRA_irq_lra_vsys_ovp_END (3)
#define SUB_PMIC_IRQ_LRA_irq_lra_adc_err_START (4)
#define SUB_PMIC_IRQ_LRA_irq_lra_adc_err_END (4)
#define SUB_PMIC_IRQ_LRA_irq_lra_acct_off_START (5)
#define SUB_PMIC_IRQ_LRA_irq_lra_acct_off_END (5)
#define SUB_PMIC_IRQ_LRA_irq_erm_bemf_abn_START (6)
#define SUB_PMIC_IRQ_LRA_irq_erm_bemf_abn_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_thsd_otmp125_d1mr : 1;
        unsigned char np_thsd_otmp140_d1mr : 1;
        unsigned char np_vsys_pwroff_abs_2d : 1;
        unsigned char np_vsys_pwroff_deb_d80mr : 1;
        unsigned char np_vsys_ov_d200ur : 1;
        unsigned char np_vsys_pwron_shutdown : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_NP_RECORD0_UNION;
#endif
#define SUB_PMIC_NP_RECORD0_np_thsd_otmp125_d1mr_START (0)
#define SUB_PMIC_NP_RECORD0_np_thsd_otmp125_d1mr_END (0)
#define SUB_PMIC_NP_RECORD0_np_thsd_otmp140_d1mr_START (1)
#define SUB_PMIC_NP_RECORD0_np_thsd_otmp140_d1mr_END (1)
#define SUB_PMIC_NP_RECORD0_np_vsys_pwroff_abs_2d_START (2)
#define SUB_PMIC_NP_RECORD0_np_vsys_pwroff_abs_2d_END (2)
#define SUB_PMIC_NP_RECORD0_np_vsys_pwroff_deb_d80mr_START (3)
#define SUB_PMIC_NP_RECORD0_np_vsys_pwroff_deb_d80mr_END (3)
#define SUB_PMIC_NP_RECORD0_np_vsys_ov_d200ur_START (4)
#define SUB_PMIC_NP_RECORD0_np_vsys_ov_d200ur_END (4)
#define SUB_PMIC_NP_RECORD0_np_vsys_pwron_shutdown_START (5)
#define SUB_PMIC_NP_RECORD0_np_vsys_pwron_shutdown_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_b31_ocp : 1;
        unsigned char np_b30_ocp : 1;
        unsigned char np_b21_ocp : 1;
        unsigned char np_b20_ocp : 1;
        unsigned char np_b11_ocp : 1;
        unsigned char np_b10_ocp : 1;
        unsigned char np_b01_ocp : 1;
        unsigned char np_b00_ocp : 1;
    } reg;
} SUB_PMIC_NP_OCP_RECORD0_UNION;
#endif
#define SUB_PMIC_NP_OCP_RECORD0_np_b31_ocp_START (0)
#define SUB_PMIC_NP_OCP_RECORD0_np_b31_ocp_END (0)
#define SUB_PMIC_NP_OCP_RECORD0_np_b30_ocp_START (1)
#define SUB_PMIC_NP_OCP_RECORD0_np_b30_ocp_END (1)
#define SUB_PMIC_NP_OCP_RECORD0_np_b21_ocp_START (2)
#define SUB_PMIC_NP_OCP_RECORD0_np_b21_ocp_END (2)
#define SUB_PMIC_NP_OCP_RECORD0_np_b20_ocp_START (3)
#define SUB_PMIC_NP_OCP_RECORD0_np_b20_ocp_END (3)
#define SUB_PMIC_NP_OCP_RECORD0_np_b11_ocp_START (4)
#define SUB_PMIC_NP_OCP_RECORD0_np_b11_ocp_END (4)
#define SUB_PMIC_NP_OCP_RECORD0_np_b10_ocp_START (5)
#define SUB_PMIC_NP_OCP_RECORD0_np_b10_ocp_END (5)
#define SUB_PMIC_NP_OCP_RECORD0_np_b01_ocp_START (6)
#define SUB_PMIC_NP_OCP_RECORD0_np_b01_ocp_END (6)
#define SUB_PMIC_NP_OCP_RECORD0_np_b00_ocp_START (7)
#define SUB_PMIC_NP_OCP_RECORD0_np_b00_ocp_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_b13_ocp : 1;
        unsigned char np_b12_ocp : 1;
        unsigned char np_b8_ocp : 1;
        unsigned char np_b7_ocp : 1;
        unsigned char np_b6_ocp : 1;
        unsigned char np_pmuh_ocp : 1;
        unsigned char np_bb_ocp : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_NP_OCP_RECORD1_UNION;
#endif
#define SUB_PMIC_NP_OCP_RECORD1_np_b13_ocp_START (0)
#define SUB_PMIC_NP_OCP_RECORD1_np_b13_ocp_END (0)
#define SUB_PMIC_NP_OCP_RECORD1_np_b12_ocp_START (1)
#define SUB_PMIC_NP_OCP_RECORD1_np_b12_ocp_END (1)
#define SUB_PMIC_NP_OCP_RECORD1_np_b8_ocp_START (2)
#define SUB_PMIC_NP_OCP_RECORD1_np_b8_ocp_END (2)
#define SUB_PMIC_NP_OCP_RECORD1_np_b7_ocp_START (3)
#define SUB_PMIC_NP_OCP_RECORD1_np_b7_ocp_END (3)
#define SUB_PMIC_NP_OCP_RECORD1_np_b6_ocp_START (4)
#define SUB_PMIC_NP_OCP_RECORD1_np_b6_ocp_END (4)
#define SUB_PMIC_NP_OCP_RECORD1_np_pmuh_ocp_START (5)
#define SUB_PMIC_NP_OCP_RECORD1_np_pmuh_ocp_END (5)
#define SUB_PMIC_NP_OCP_RECORD1_np_bb_ocp_START (6)
#define SUB_PMIC_NP_OCP_RECORD1_np_bb_ocp_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo54_ocp : 1;
        unsigned char np_ldo53_ocp : 1;
        unsigned char np_ldo52_ocp : 1;
        unsigned char np_ldo51_ocp : 1;
        unsigned char np_ldo50_ocp : 1;
        unsigned char np_ldo34_ocp : 1;
        unsigned char np_ldo9_ocp : 1;
        unsigned char np_sw10_ocp : 1;
    } reg;
} SUB_PMIC_NP_OCP_RECORD3_UNION;
#endif
#define SUB_PMIC_NP_OCP_RECORD3_np_ldo54_ocp_START (0)
#define SUB_PMIC_NP_OCP_RECORD3_np_ldo54_ocp_END (0)
#define SUB_PMIC_NP_OCP_RECORD3_np_ldo53_ocp_START (1)
#define SUB_PMIC_NP_OCP_RECORD3_np_ldo53_ocp_END (1)
#define SUB_PMIC_NP_OCP_RECORD3_np_ldo52_ocp_START (2)
#define SUB_PMIC_NP_OCP_RECORD3_np_ldo52_ocp_END (2)
#define SUB_PMIC_NP_OCP_RECORD3_np_ldo51_ocp_START (3)
#define SUB_PMIC_NP_OCP_RECORD3_np_ldo51_ocp_END (3)
#define SUB_PMIC_NP_OCP_RECORD3_np_ldo50_ocp_START (4)
#define SUB_PMIC_NP_OCP_RECORD3_np_ldo50_ocp_END (4)
#define SUB_PMIC_NP_OCP_RECORD3_np_ldo34_ocp_START (5)
#define SUB_PMIC_NP_OCP_RECORD3_np_ldo34_ocp_END (5)
#define SUB_PMIC_NP_OCP_RECORD3_np_ldo9_ocp_START (6)
#define SUB_PMIC_NP_OCP_RECORD3_np_ldo9_ocp_END (6)
#define SUB_PMIC_NP_OCP_RECORD3_np_sw10_ocp_START (7)
#define SUB_PMIC_NP_OCP_RECORD3_np_sw10_ocp_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_b30_scp : 1;
        unsigned char np_b20_scp : 1;
        unsigned char np_b10_scp : 1;
        unsigned char np_b00_scp : 1;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_NP_SCP_RECORD0_UNION;
#endif
#define SUB_PMIC_NP_SCP_RECORD0_np_b30_scp_START (0)
#define SUB_PMIC_NP_SCP_RECORD0_np_b30_scp_END (0)
#define SUB_PMIC_NP_SCP_RECORD0_np_b20_scp_START (1)
#define SUB_PMIC_NP_SCP_RECORD0_np_b20_scp_END (1)
#define SUB_PMIC_NP_SCP_RECORD0_np_b10_scp_START (2)
#define SUB_PMIC_NP_SCP_RECORD0_np_b10_scp_END (2)
#define SUB_PMIC_NP_SCP_RECORD0_np_b00_scp_START (3)
#define SUB_PMIC_NP_SCP_RECORD0_np_b00_scp_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_b13_scp : 1;
        unsigned char np_b12_scp : 1;
        unsigned char np_b8_scp : 1;
        unsigned char np_b7_scp : 1;
        unsigned char np_b6_scp : 1;
        unsigned char np_bb_scp : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_NP_SCP_RECORD1_UNION;
#endif
#define SUB_PMIC_NP_SCP_RECORD1_np_b13_scp_START (0)
#define SUB_PMIC_NP_SCP_RECORD1_np_b13_scp_END (0)
#define SUB_PMIC_NP_SCP_RECORD1_np_b12_scp_START (1)
#define SUB_PMIC_NP_SCP_RECORD1_np_b12_scp_END (1)
#define SUB_PMIC_NP_SCP_RECORD1_np_b8_scp_START (2)
#define SUB_PMIC_NP_SCP_RECORD1_np_b8_scp_END (2)
#define SUB_PMIC_NP_SCP_RECORD1_np_b7_scp_START (3)
#define SUB_PMIC_NP_SCP_RECORD1_np_b7_scp_END (3)
#define SUB_PMIC_NP_SCP_RECORD1_np_b6_scp_START (4)
#define SUB_PMIC_NP_SCP_RECORD1_np_b6_scp_END (4)
#define SUB_PMIC_NP_SCP_RECORD1_np_bb_scp_START (5)
#define SUB_PMIC_NP_SCP_RECORD1_np_bb_scp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_b30_ovp : 1;
        unsigned char np_b20_ovp : 1;
        unsigned char np_b10_ovp : 1;
        unsigned char np_b00_ovp : 1;
        unsigned char np_b12_ovp : 1;
        unsigned char np_b8_ovp : 1;
        unsigned char np_bb_ovp : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_NP_OVP_RECORD_UNION;
#endif
#define SUB_PMIC_NP_OVP_RECORD_np_b30_ovp_START (0)
#define SUB_PMIC_NP_OVP_RECORD_np_b30_ovp_END (0)
#define SUB_PMIC_NP_OVP_RECORD_np_b20_ovp_START (1)
#define SUB_PMIC_NP_OVP_RECORD_np_b20_ovp_END (1)
#define SUB_PMIC_NP_OVP_RECORD_np_b10_ovp_START (2)
#define SUB_PMIC_NP_OVP_RECORD_np_b10_ovp_END (2)
#define SUB_PMIC_NP_OVP_RECORD_np_b00_ovp_START (3)
#define SUB_PMIC_NP_OVP_RECORD_np_b00_ovp_END (3)
#define SUB_PMIC_NP_OVP_RECORD_np_b12_ovp_START (4)
#define SUB_PMIC_NP_OVP_RECORD_np_b12_ovp_END (4)
#define SUB_PMIC_NP_OVP_RECORD_np_b8_ovp_START (5)
#define SUB_PMIC_NP_OVP_RECORD_np_b8_ovp_END (5)
#define SUB_PMIC_NP_OVP_RECORD_np_bb_ovp_START (6)
#define SUB_PMIC_NP_OVP_RECORD_np_bb_ovp_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_b30_cur : 1;
        unsigned char np_b20_cur : 1;
        unsigned char np_b10_cur : 1;
        unsigned char np_b00_cur : 1;
        unsigned char np_b12_cur : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_NP_CUR_DET_UNION;
#endif
#define SUB_PMIC_NP_CUR_DET_np_b30_cur_START (0)
#define SUB_PMIC_NP_CUR_DET_np_b30_cur_END (0)
#define SUB_PMIC_NP_CUR_DET_np_b20_cur_START (1)
#define SUB_PMIC_NP_CUR_DET_np_b20_cur_END (1)
#define SUB_PMIC_NP_CUR_DET_np_b10_cur_START (2)
#define SUB_PMIC_NP_CUR_DET_np_b10_cur_END (2)
#define SUB_PMIC_NP_CUR_DET_np_b00_cur_START (3)
#define SUB_PMIC_NP_CUR_DET_np_b00_cur_END (3)
#define SUB_PMIC_NP_CUR_DET_np_b12_cur_START (4)
#define SUB_PMIC_NP_CUR_DET_np_b12_cur_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_b7_ramp_abn : 1;
        unsigned char np_b6_ramp_abn : 1;
        unsigned char np_b12_ramp_abn : 1;
        unsigned char np_b30_ramp_abn : 1;
        unsigned char np_b20_ramp_abn : 1;
        unsigned char np_b10_ramp_abn : 1;
        unsigned char np_b00_ramp_abn : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_NP_IRQ_RAMP_ABN_UNION;
#endif
#define SUB_PMIC_NP_IRQ_RAMP_ABN_np_b7_ramp_abn_START (0)
#define SUB_PMIC_NP_IRQ_RAMP_ABN_np_b7_ramp_abn_END (0)
#define SUB_PMIC_NP_IRQ_RAMP_ABN_np_b6_ramp_abn_START (1)
#define SUB_PMIC_NP_IRQ_RAMP_ABN_np_b6_ramp_abn_END (1)
#define SUB_PMIC_NP_IRQ_RAMP_ABN_np_b12_ramp_abn_START (2)
#define SUB_PMIC_NP_IRQ_RAMP_ABN_np_b12_ramp_abn_END (2)
#define SUB_PMIC_NP_IRQ_RAMP_ABN_np_b30_ramp_abn_START (3)
#define SUB_PMIC_NP_IRQ_RAMP_ABN_np_b30_ramp_abn_END (3)
#define SUB_PMIC_NP_IRQ_RAMP_ABN_np_b20_ramp_abn_START (4)
#define SUB_PMIC_NP_IRQ_RAMP_ABN_np_b20_ramp_abn_END (4)
#define SUB_PMIC_NP_IRQ_RAMP_ABN_np_b10_ramp_abn_START (5)
#define SUB_PMIC_NP_IRQ_RAMP_ABN_np_b10_ramp_abn_END (5)
#define SUB_PMIC_NP_IRQ_RAMP_ABN_np_b00_ramp_abn_START (6)
#define SUB_PMIC_NP_IRQ_RAMP_ABN_np_b00_ramp_abn_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_fault_m_n : 1;
        unsigned char np_fault_s_n : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_NP_RECORD1_UNION;
#endif
#define SUB_PMIC_NP_RECORD1_np_fault_m_n_START (0)
#define SUB_PMIC_NP_RECORD1_np_fault_m_n_END (0)
#define SUB_PMIC_NP_RECORD1_np_fault_s_n_START (1)
#define SUB_PMIC_NP_RECORD1_np_fault_s_n_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_pmud_short_f : 1;
        unsigned char np_pmuh_short_f : 1;
        unsigned char np_vin_ldoh : 1;
        unsigned char np_pmu_rc_clk_sel_r : 1;
        unsigned char np_pmu_rc_clk_sel_f : 1;
        unsigned char np_core_io18_vld_f : 1;
        unsigned char np_core_io12_vld_f : 1;
        unsigned char np_clk32_pmu_dis : 1;
    } reg;
} SUB_PMIC_NP_RECORD2_UNION;
#endif
#define SUB_PMIC_NP_RECORD2_np_pmud_short_f_START (0)
#define SUB_PMIC_NP_RECORD2_np_pmud_short_f_END (0)
#define SUB_PMIC_NP_RECORD2_np_pmuh_short_f_START (1)
#define SUB_PMIC_NP_RECORD2_np_pmuh_short_f_END (1)
#define SUB_PMIC_NP_RECORD2_np_vin_ldoh_START (2)
#define SUB_PMIC_NP_RECORD2_np_vin_ldoh_END (2)
#define SUB_PMIC_NP_RECORD2_np_pmu_rc_clk_sel_r_START (3)
#define SUB_PMIC_NP_RECORD2_np_pmu_rc_clk_sel_r_END (3)
#define SUB_PMIC_NP_RECORD2_np_pmu_rc_clk_sel_f_START (4)
#define SUB_PMIC_NP_RECORD2_np_pmu_rc_clk_sel_f_END (4)
#define SUB_PMIC_NP_RECORD2_np_core_io18_vld_f_START (5)
#define SUB_PMIC_NP_RECORD2_np_core_io18_vld_f_END (5)
#define SUB_PMIC_NP_RECORD2_np_core_io12_vld_f_START (6)
#define SUB_PMIC_NP_RECORD2_np_core_io12_vld_f_END (6)
#define SUB_PMIC_NP_RECORD2_np_clk32_pmu_dis_START (7)
#define SUB_PMIC_NP_RECORD2_np_clk32_pmu_dis_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_b7_ramp_eco : 1;
        unsigned char np_b6_ramp_eco : 1;
        unsigned char np_b12_ramp_eco : 1;
        unsigned char np_b30_ramp_eco : 1;
        unsigned char np_b20_ramp_eco : 1;
        unsigned char np_b10_ramp_eco : 1;
        unsigned char np_b00_ramp_eco : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_NP_RAMP_ECO_UNION;
#endif
#define SUB_PMIC_NP_RAMP_ECO_np_b7_ramp_eco_START (0)
#define SUB_PMIC_NP_RAMP_ECO_np_b7_ramp_eco_END (0)
#define SUB_PMIC_NP_RAMP_ECO_np_b6_ramp_eco_START (1)
#define SUB_PMIC_NP_RAMP_ECO_np_b6_ramp_eco_END (1)
#define SUB_PMIC_NP_RAMP_ECO_np_b12_ramp_eco_START (2)
#define SUB_PMIC_NP_RAMP_ECO_np_b12_ramp_eco_END (2)
#define SUB_PMIC_NP_RAMP_ECO_np_b30_ramp_eco_START (3)
#define SUB_PMIC_NP_RAMP_ECO_np_b30_ramp_eco_END (3)
#define SUB_PMIC_NP_RAMP_ECO_np_b20_ramp_eco_START (4)
#define SUB_PMIC_NP_RAMP_ECO_np_b20_ramp_eco_END (4)
#define SUB_PMIC_NP_RAMP_ECO_np_b10_ramp_eco_START (5)
#define SUB_PMIC_NP_RAMP_ECO_np_b10_ramp_eco_END (5)
#define SUB_PMIC_NP_RAMP_ECO_np_b00_ramp_eco_START (6)
#define SUB_PMIC_NP_RAMP_ECO_np_b00_ramp_eco_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_bw_sel : 1;
        unsigned char b00_buffer_bias : 2;
        unsigned char b00_b00_ea_r : 3;
        unsigned char b00_avg_curr_filter_sel : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_0_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_0_b00_bw_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_0_b00_bw_sel_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_0_b00_buffer_bias_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_0_b00_buffer_bias_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_0_b00_b00_ea_r_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_0_b00_b00_ea_r_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_0_b00_avg_curr_filter_sel_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_0_b00_avg_curr_filter_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_c1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_1_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_1_b00_c1_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_1_b00_c1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_c2_sel : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_2_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_2_b00_c2_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_2_b00_c2_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_clk_chopper_sel : 2;
        unsigned char b00_c3_sel : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_3_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_3_b00_clk_chopper_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_3_b00_clk_chopper_sel_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_3_b00_c3_sel_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_3_b00_c3_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_clock_atest : 1;
        unsigned char b00_clk9m6_pulse_sel : 2;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_4_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_4_b00_clock_atest_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_4_b00_clock_atest_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_4_b00_clk9m6_pulse_sel_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_4_b00_clk9m6_pulse_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_clock_sel : 8;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_5_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_5_b00_clock_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_5_b00_clock_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_clp_reg_idrp : 4;
        unsigned char b00_clp_ref_sel : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_6_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_6_b00_clp_reg_idrp_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_6_b00_clp_reg_idrp_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_6_b00_clp_ref_sel_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_6_b00_clp_ref_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_cot_cmphys : 1;
        unsigned char b00_cot_atest : 1;
        unsigned char b00_code_mode_sel : 1;
        unsigned char b00_cmp_atest : 1;
        unsigned char b00_clpeco_bias : 4;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_7_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_7_b00_cot_cmphys_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_7_b00_cot_cmphys_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_7_b00_cot_atest_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_7_b00_cot_atest_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_7_b00_code_mode_sel_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_7_b00_code_mode_sel_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_7_b00_cmp_atest_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_7_b00_cmp_atest_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_7_b00_clpeco_bias_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_7_b00_clpeco_bias_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_cot_sel : 1;
        unsigned char b00_cot_sche_neco : 1;
        unsigned char b00_cot_sche_eco : 1;
        unsigned char b00_cot_res_sel : 3;
        unsigned char b00_cot_iboost_sel : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_8_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_8_b00_cot_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_8_b00_cot_sel_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_8_b00_cot_sche_neco_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_8_b00_cot_sche_neco_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_8_b00_cot_sche_eco_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_8_b00_cot_sche_eco_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_8_b00_cot_res_sel_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_8_b00_cot_res_sel_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_8_b00_cot_iboost_sel_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_8_b00_cot_iboost_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_cot_ton_ecoa : 8;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_9_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_9_b00_cot_ton_ecoa_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_9_b00_cot_ton_ecoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_cot_ton_isen_hys : 2;
        unsigned char b00_cot_ton_isen_bias : 2;
        unsigned char b00_cot_ton_ecob : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_10_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_10_b00_cot_ton_isen_hys_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_10_b00_cot_ton_isen_hys_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_10_b00_cot_ton_isen_bias_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_10_b00_cot_ton_isen_bias_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_10_b00_cot_ton_ecob_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_10_b00_cot_ton_ecob_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_cot_ton_necoa : 8;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_11_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_11_b00_cot_ton_necoa_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_11_b00_cot_ton_necoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_crtshare_bw_sel : 2;
        unsigned char b00_crtshare_atest : 1;
        unsigned char b00_cot_ton_necob : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_12_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_12_b00_crtshare_bw_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_12_b00_crtshare_bw_sel_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_12_b00_crtshare_atest_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_12_b00_crtshare_atest_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_12_b00_cot_ton_necob_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_12_b00_cot_ton_necob_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_cs_test_sel : 1;
        unsigned char b00_crtshare_trim : 1;
        unsigned char b00_crtshare_filter_r_sel : 1;
        unsigned char b00_crtshare_clp_sel : 4;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_13_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_13_b00_cs_test_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_13_b00_cs_test_sel_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_13_b00_crtshare_trim_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_13_b00_crtshare_trim_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_13_b00_crtshare_filter_r_sel_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_13_b00_crtshare_filter_r_sel_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_13_b00_crtshare_clp_sel_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_13_b00_crtshare_clp_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_dbias : 3;
        unsigned char b00_ctlogic_smph : 1;
        unsigned char b00_ctlogic_sgph : 1;
        unsigned char b00_ctlogic_modsw : 1;
        unsigned char b00_ctlogic_crson_td : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_14_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_14_b00_dbias_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_14_b00_dbias_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_14_b00_ctlogic_smph_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_14_b00_ctlogic_smph_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_14_b00_ctlogic_sgph_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_14_b00_ctlogic_sgph_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_14_b00_ctlogic_modsw_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_14_b00_ctlogic_modsw_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_14_b00_ctlogic_crson_td_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_14_b00_ctlogic_crson_td_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_dmd_auto_eco : 1;
        unsigned char b00_dmd_auto_cmpib : 2;
        unsigned char b00_dmd_auto_ccm : 1;
        unsigned char b00_dmd_atest : 1;
        unsigned char b00_dbias_eco : 3;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_15_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_15_b00_dmd_auto_eco_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_15_b00_dmd_auto_eco_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_15_b00_dmd_auto_cmpib_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_15_b00_dmd_auto_cmpib_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_15_b00_dmd_auto_ccm_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_15_b00_dmd_auto_ccm_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_15_b00_dmd_atest_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_15_b00_dmd_atest_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_15_b00_dbias_eco_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_15_b00_dbias_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_dmd_bck_sel : 1;
        unsigned char b00_dmd_auto_vos : 3;
        unsigned char b00_dmd_auto_vbc : 1;
        unsigned char b00_dmd_auto_sel : 1;
        unsigned char b00_dmd_auto_sampt : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_16_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_16_b00_dmd_bck_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_16_b00_dmd_bck_sel_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_16_b00_dmd_auto_vos_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_16_b00_dmd_auto_vos_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_16_b00_dmd_auto_vbc_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_16_b00_dmd_auto_vbc_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_16_b00_dmd_auto_sel_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_16_b00_dmd_auto_sel_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_16_b00_dmd_auto_sampt_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_16_b00_dmd_auto_sampt_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_dmd_ngc_sel : 1;
        unsigned char b00_dmd_ngc_disch : 1;
        unsigned char b00_dmd_man_set : 4;
        unsigned char b00_dmd_eco_sel : 1;
        unsigned char b00_dmd_clp_sel : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_17_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_17_b00_dmd_ngc_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_17_b00_dmd_ngc_sel_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_17_b00_dmd_ngc_disch_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_17_b00_dmd_ngc_disch_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_17_b00_dmd_man_set_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_17_b00_dmd_man_set_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_17_b00_dmd_eco_sel_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_17_b00_dmd_eco_sel_END (6)
#define SUB_PMIC_BUCK00_CFG_REG_17_b00_dmd_clp_sel_START (7)
#define SUB_PMIC_BUCK00_CFG_REG_17_b00_dmd_clp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_drv_sw_sel : 1;
        unsigned char b00_drv_dt_sel : 4;
        unsigned char b00_dmd_ngc_set : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_18_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_18_b00_drv_sw_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_18_b00_drv_sw_sel_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_18_b00_drv_dt_sel_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_18_b00_drv_dt_sel_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_18_b00_dmd_ngc_set_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_18_b00_dmd_ngc_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_dt_sel : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_19_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_19_b00_dt_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_19_b00_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_eabias : 1;
        unsigned char b00_eaamp_clp_sel : 1;
        unsigned char b00_eaamp_clp : 2;
        unsigned char b00_ea_ecocur_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_20_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_20_b00_eabias_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_20_b00_eabias_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_20_b00_eaamp_clp_sel_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_20_b00_eaamp_clp_sel_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_20_b00_eaamp_clp_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_20_b00_eaamp_clp_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_20_b00_ea_ecocur_sel_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_20_b00_ea_ecocur_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_eaclp_ecogap_sel : 2;
        unsigned char b00_eaclp_brgap_sel : 4;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_21_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_21_b00_eaclp_ecogap_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_21_b00_eaclp_ecogap_sel_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_21_b00_eaclp_brgap_sel_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_21_b00_eaclp_brgap_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_eacomp_eco_shield : 1;
        unsigned char b00_eaclp_srgap_sel : 4;
        unsigned char b00_eaclp_gap_sel : 3;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_22_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_22_b00_eacomp_eco_shield_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_22_b00_eacomp_eco_shield_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_22_b00_eaclp_srgap_sel_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_22_b00_eaclp_srgap_sel_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_22_b00_eaclp_gap_sel_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_22_b00_eaclp_gap_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_ecdt_ct2cm_ref : 3;
        unsigned char b00_ecdt_ct2cm_ft : 2;
        unsigned char b00_ecdt_cmphys : 1;
        unsigned char b00_eatrim_en : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_23_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_23_b00_ecdt_ct2cm_ref_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_23_b00_ecdt_ct2cm_ref_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_23_b00_ecdt_ct2cm_ft_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_23_b00_ecdt_ct2cm_ft_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_23_b00_ecdt_cmphys_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_23_b00_ecdt_cmphys_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_23_b00_eatrim_en_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_23_b00_eatrim_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_ecdt_ec2ct_ref : 3;
        unsigned char b00_ecdt_ct2ec_ref : 3;
        unsigned char b00_ecdt_ct2ec_ft : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_24_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_24_b00_ecdt_ec2ct_ref_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_24_b00_ecdt_ec2ct_ref_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_24_b00_ecdt_ct2ec_ref_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_24_b00_ecdt_ct2ec_ref_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_24_b00_ecdt_ct2ec_ft_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_24_b00_ecdt_ct2ec_ft_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_en_psns : 1;
        unsigned char b00_en_nsense : 1;
        unsigned char b00_en_chopper : 1;
        unsigned char b00_ecotrim_shield : 1;
        unsigned char b00_eco_shield_cmp : 1;
        unsigned char b00_eco_shield : 1;
        unsigned char b00_ecdt_filter_rc : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_25_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_25_b00_en_psns_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_25_b00_en_psns_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_25_b00_en_nsense_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_25_b00_en_nsense_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_25_b00_en_chopper_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_25_b00_en_chopper_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_25_b00_ecotrim_shield_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_25_b00_ecotrim_shield_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_25_b00_eco_shield_cmp_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_25_b00_eco_shield_cmp_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_25_b00_eco_shield_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_25_b00_eco_shield_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_25_b00_ecdt_filter_rc_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_25_b00_ecdt_filter_rc_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_ldline_forced_on : 1;
        unsigned char b00_ibalance_gm_sel : 2;
        unsigned char b00_forcenormal : 1;
        unsigned char b00_forceeco : 1;
        unsigned char b00_fastnmos_off : 1;
        unsigned char b00_en_vo : 1;
        unsigned char b00_en_test : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_26_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_26_b00_ldline_forced_on_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_26_b00_ldline_forced_on_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_26_b00_ibalance_gm_sel_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_26_b00_ibalance_gm_sel_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_26_b00_forcenormal_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_26_b00_forcenormal_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_26_b00_forceeco_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_26_b00_forceeco_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_26_b00_fastnmos_off_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_26_b00_fastnmos_off_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_26_b00_en_vo_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_26_b00_en_vo_END (6)
#define SUB_PMIC_BUCK00_CFG_REG_26_b00_en_test_START (7)
#define SUB_PMIC_BUCK00_CFG_REG_26_b00_en_test_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_load_det_sel : 2;
        unsigned char b00_ldline_mohm_sel : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_27_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_27_b00_load_det_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_27_b00_load_det_sel_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_27_b00_ldline_mohm_sel_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_27_b00_ldline_mohm_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_min_ton_sel : 2;
        unsigned char b00_min_toff_sel : 2;
        unsigned char b00_load_state_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_28_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_28_b00_min_ton_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_28_b00_min_ton_sel_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_28_b00_min_toff_sel_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_28_b00_min_toff_sel_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_28_b00_load_state_sel_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_28_b00_load_state_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_ng_dt_sel : 3;
        unsigned char b00_negdmdton_add : 1;
        unsigned char b00_negdmd_ton_sel : 2;
        unsigned char b00_n_sel : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_29_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_29_b00_ng_dt_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_29_b00_ng_dt_sel_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_29_b00_negdmdton_add_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_29_b00_negdmdton_add_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_29_b00_negdmd_ton_sel_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_29_b00_negdmd_ton_sel_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_29_b00_n_sel_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_29_b00_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_nmos_off : 1;
        unsigned char b00_ng_p_sel : 3;
        unsigned char b00_ng_n_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_30_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_30_b00_nmos_off_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_30_b00_nmos_off_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_30_b00_ng_p_sel_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_30_b00_ng_p_sel_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_30_b00_ng_n_sel_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_30_b00_ng_n_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_nocp_low_sel : 2;
        unsigned char b00_nocp_low_eco_sel : 1;
        unsigned char b00_nocp_low_delay_sel : 2;
        unsigned char b00_nocp_en : 1;
        unsigned char b00_nocp_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_31_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_31_b00_nocp_low_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_31_b00_nocp_low_sel_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_31_b00_nocp_low_eco_sel_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_31_b00_nocp_low_eco_sel_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_31_b00_nocp_low_delay_sel_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_31_b00_nocp_low_delay_sel_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_31_b00_nocp_en_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_31_b00_nocp_en_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_31_b00_nocp_delay_sel_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_31_b00_nocp_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_ocp_2us_delay_sel : 1;
        unsigned char b00_nsense_sel : 3;
        unsigned char b00_npocp_atest : 1;
        unsigned char b00_normaltrim_shield : 1;
        unsigned char b00_nocp_sel : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_32_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_32_b00_ocp_2us_delay_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_32_b00_ocp_2us_delay_sel_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_32_b00_nsense_sel_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_32_b00_nsense_sel_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_32_b00_npocp_atest_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_32_b00_npocp_atest_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_32_b00_normaltrim_shield_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_32_b00_normaltrim_shield_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_32_b00_nocp_sel_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_32_b00_nocp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_p_sel : 2;
        unsigned char b00_ovp_shield : 1;
        unsigned char b00_ovp_sel : 2;
        unsigned char b00_oneph : 1;
        unsigned char b00_ocp_short_en : 1;
        unsigned char b00_ocp_shield : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_33_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_33_b00_p_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_33_b00_p_sel_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_33_b00_ovp_shield_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_33_b00_ovp_shield_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_33_b00_ovp_sel_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_33_b00_ovp_sel_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_33_b00_oneph_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_33_b00_oneph_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_33_b00_ocp_short_en_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_33_b00_ocp_short_en_END (6)
#define SUB_PMIC_BUCK00_CFG_REG_33_b00_ocp_shield_START (7)
#define SUB_PMIC_BUCK00_CFG_REG_33_b00_ocp_shield_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_pg_p_sel : 3;
        unsigned char b00_pg_n_sel : 3;
        unsigned char b00_pg_dt_sel : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_34_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_34_b00_pg_p_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_34_b00_pg_p_sel_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_34_b00_pg_n_sel_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_34_b00_pg_n_sel_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_34_b00_pg_dt_sel_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_34_b00_pg_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_pocp_sw_ccm_cmp : 1;
        unsigned char b00_pocp_sw_atest : 1;
        unsigned char b00_pocp_sel : 2;
        unsigned char b00_pocp_en : 1;
        unsigned char b00_pocp_eco_sel : 1;
        unsigned char b00_pocp_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_35_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_35_b00_pocp_sw_ccm_cmp_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_35_b00_pocp_sw_ccm_cmp_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_35_b00_pocp_sw_atest_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_35_b00_pocp_sw_atest_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_35_b00_pocp_sel_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_35_b00_pocp_sel_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_35_b00_pocp_en_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_35_b00_pocp_en_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_35_b00_pocp_eco_sel_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_35_b00_pocp_eco_sel_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_35_b00_pocp_delay_sel_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_35_b00_pocp_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_pulldn_pd_sel : 1;
        unsigned char b00_pulldn_pd_en : 1;
        unsigned char b00_pocp_sw_sel : 3;
        unsigned char b00_pocp_sw_eco_cmp : 1;
        unsigned char b00_pocp_sw_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_36_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_36_b00_pulldn_pd_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_36_b00_pulldn_pd_sel_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_36_b00_pulldn_pd_en_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_36_b00_pulldn_pd_en_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_36_b00_pocp_sw_sel_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_36_b00_pocp_sw_sel_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_36_b00_pocp_sw_eco_cmp_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_36_b00_pocp_sw_eco_cmp_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_36_b00_pocp_sw_delay_sel_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_36_b00_pocp_sw_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_r1_sel : 4;
        unsigned char b00_pvdd_ton_sel : 1;
        unsigned char b00_pulldn_sel : 3;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_37_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_37_b00_r1_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_37_b00_r1_sel_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_37_b00_pvdd_ton_sel_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_37_b00_pvdd_ton_sel_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_37_b00_pulldn_sel_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_37_b00_pulldn_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_r2_sel : 6;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_38_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_38_b00_r2_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_38_b00_r2_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_ramp_big_sel : 1;
        unsigned char b00_r3_sel : 6;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_39_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_39_b00_ramp_big_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_39_b00_ramp_big_sel_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_39_b00_r3_sel_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_39_b00_r3_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_ramp_ccm_outsel : 1;
        unsigned char b00_ramp_c : 2;
        unsigned char b00_ramp_buff_trim_test : 1;
        unsigned char b00_ramp_buff_trim : 1;
        unsigned char b00_ramp_buff_bias : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_40_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_40_b00_ramp_ccm_outsel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_40_b00_ramp_ccm_outsel_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_40_b00_ramp_c_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_40_b00_ramp_c_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_40_b00_ramp_buff_trim_test_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_40_b00_ramp_buff_trim_test_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_40_b00_ramp_buff_trim_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_40_b00_ramp_buff_trim_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_40_b00_ramp_buff_bias_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_40_b00_ramp_buff_bias_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_ramp_cot_outsel : 1;
        unsigned char b00_ramp_clk_sel : 1;
        unsigned char b00_ramp_change_mode : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_41_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_41_b00_ramp_cot_outsel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_41_b00_ramp_cot_outsel_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_41_b00_ramp_clk_sel_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_41_b00_ramp_clk_sel_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_41_b00_ramp_change_mode_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_41_b00_ramp_change_mode_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_ramp_line_sel : 1;
        unsigned char b00_ramp_freq_sel : 1;
        unsigned char b00_ramp_force_eco_test : 1;
        unsigned char b00_ramp_fast_sel : 1;
        unsigned char b00_ramp_eco_buffer : 1;
        unsigned char b00_ramp_dblclk_sel : 1;
        unsigned char b00_ramp_counter : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_42_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_42_b00_ramp_line_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_42_b00_ramp_line_sel_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_42_b00_ramp_freq_sel_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_42_b00_ramp_freq_sel_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_42_b00_ramp_force_eco_test_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_42_b00_ramp_force_eco_test_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_42_b00_ramp_fast_sel_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_42_b00_ramp_fast_sel_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_42_b00_ramp_eco_buffer_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_42_b00_ramp_eco_buffer_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_42_b00_ramp_dblclk_sel_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_42_b00_ramp_dblclk_sel_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_42_b00_ramp_counter_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_42_b00_ramp_counter_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_ramp_r_ccm : 4;
        unsigned char b00_ramp_mid_sel : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_43_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_43_b00_ramp_r_ccm_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_43_b00_ramp_r_ccm_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_43_b00_ramp_mid_sel_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_43_b00_ramp_mid_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_ramp_r_eco : 4;
        unsigned char b00_ramp_r_cot : 4;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_44_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_44_b00_ramp_r_eco_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_44_b00_ramp_r_eco_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_44_b00_ramp_r_cot_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_44_b00_ramp_r_cot_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_ramptrim_sel : 1;
        unsigned char b00_ramp_valley_sel : 1;
        unsigned char b00_ramp_trim_sel : 1;
        unsigned char b00_ramp_sw_sel : 1;
        unsigned char b00_ramp_sel : 1;
        unsigned char b00_ramp_res_test : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_45_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_45_b00_ramptrim_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_45_b00_ramptrim_sel_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_45_b00_ramp_valley_sel_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_45_b00_ramp_valley_sel_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_45_b00_ramp_trim_sel_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_45_b00_ramp_trim_sel_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_45_b00_ramp_sw_sel_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_45_b00_ramp_sw_sel_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_45_b00_ramp_sel_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_45_b00_ramp_sel_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_45_b00_ramp_res_test_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_45_b00_ramp_res_test_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_reg_idrp : 3;
        unsigned char b00_reg_idrn : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_46_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_46_b00_reg_idrp_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_46_b00_reg_idrp_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_46_b00_reg_idrn_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_46_b00_reg_idrn_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_reg_sense_res_sel : 3;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_47_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_47_b00_reg_sense_res_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_47_b00_reg_sense_res_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_sft : 1;
        unsigned char b00_sense_ratio_sel : 2;
        unsigned char b00_rtr_cap_sel : 2;
        unsigned char b00_ron_test_sel : 3;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_48_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_48_b00_sft_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_48_b00_sft_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_48_b00_sense_ratio_sel_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_48_b00_sense_ratio_sel_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_48_b00_rtr_cap_sel_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_48_b00_rtr_cap_sel_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_48_b00_ron_test_sel_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_48_b00_ron_test_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_stsw_atest : 1;
        unsigned char b00_state_sw_ocp_sel : 2;
        unsigned char b00_state_sw_ocp_off : 1;
        unsigned char b00_slave_sel : 1;
        unsigned char b00_short_shield : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_49_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_49_b00_stsw_atest_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_49_b00_stsw_atest_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_49_b00_state_sw_ocp_sel_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_49_b00_state_sw_ocp_sel_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_49_b00_state_sw_ocp_off_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_49_b00_state_sw_ocp_off_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_49_b00_slave_sel_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_49_b00_slave_sel_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_49_b00_short_shield_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_49_b00_short_shield_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_stsw_fstsch_eco : 3;
        unsigned char b00_stsw_fstps_ith : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_50_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_50_b00_stsw_fstsch_eco_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_50_b00_stsw_fstsch_eco_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_50_b00_stsw_fstps_ith_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_50_b00_stsw_fstps_ith_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_stsw_slw_ct2cm_hys : 2;
        unsigned char b00_stsw_rpup_ccm : 1;
        unsigned char b00_stsw_man_set : 2;
        unsigned char b00_stsw_fstsch_neco : 3;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_51_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_51_b00_stsw_slw_ct2cm_hys_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_51_b00_stsw_slw_ct2cm_hys_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_51_b00_stsw_rpup_ccm_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_51_b00_stsw_rpup_ccm_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_51_b00_stsw_man_set_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_51_b00_stsw_man_set_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_51_b00_stsw_fstsch_neco_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_51_b00_stsw_fstsch_neco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_stsw_slw_hys_shield : 1;
        unsigned char b00_stsw_slw_delay_l : 2;
        unsigned char b00_stsw_slw_delay_h : 2;
        unsigned char b00_stsw_slw_ct2cm_ith : 3;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_52_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_52_b00_stsw_slw_hys_shield_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_52_b00_stsw_slw_hys_shield_END (0)
#define SUB_PMIC_BUCK00_CFG_REG_52_b00_stsw_slw_delay_l_START (1)
#define SUB_PMIC_BUCK00_CFG_REG_52_b00_stsw_slw_delay_l_END (2)
#define SUB_PMIC_BUCK00_CFG_REG_52_b00_stsw_slw_delay_h_START (3)
#define SUB_PMIC_BUCK00_CFG_REG_52_b00_stsw_slw_delay_h_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_52_b00_stsw_slw_ct2cm_ith_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_52_b00_stsw_slw_ct2cm_ith_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_ton_det : 4;
        unsigned char b00_stsw_slwsch_neco : 2;
        unsigned char b00_stsw_slwsch_eco : 2;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_53_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_53_b00_ton_det_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_53_b00_ton_det_END (3)
#define SUB_PMIC_BUCK00_CFG_REG_53_b00_stsw_slwsch_neco_START (4)
#define SUB_PMIC_BUCK00_CFG_REG_53_b00_stsw_slwsch_neco_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_53_b00_stsw_slwsch_eco_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_53_b00_stsw_slwsch_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_vref_test_sel : 2;
        unsigned char b00_vo_ton_res_sel : 3;
        unsigned char b00_trim2 : 1;
        unsigned char b00_trim1 : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_54_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_54_b00_vref_test_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_54_b00_vref_test_sel_END (1)
#define SUB_PMIC_BUCK00_CFG_REG_54_b00_vo_ton_res_sel_START (2)
#define SUB_PMIC_BUCK00_CFG_REG_54_b00_vo_ton_res_sel_END (4)
#define SUB_PMIC_BUCK00_CFG_REG_54_b00_trim2_START (5)
#define SUB_PMIC_BUCK00_CFG_REG_54_b00_trim2_END (5)
#define SUB_PMIC_BUCK00_CFG_REG_54_b00_trim1_START (6)
#define SUB_PMIC_BUCK00_CFG_REG_54_b00_trim1_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_vref_vo_cap_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_55_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_55_b00_vref_vo_cap_sel_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_55_b00_vref_vo_cap_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_reserve0 : 8;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_56_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_56_b00_reserve0_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_56_b00_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_reserve1 : 8;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_57_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_57_b00_reserve1_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_57_b00_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_reserve2 : 8;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_58_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_58_b00_reserve2_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_58_b00_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_reserve3 : 8;
    } reg;
} SUB_PMIC_BUCK00_CFG_REG_59_UNION;
#endif
#define SUB_PMIC_BUCK00_CFG_REG_59_b00_reserve3_START (0)
#define SUB_PMIC_BUCK00_CFG_REG_59_b00_reserve3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b00_state_a2d : 2;
        unsigned char b00_pg : 1;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK00_RO_REG_60_UNION;
#endif
#define SUB_PMIC_BUCK00_RO_REG_60_b00_state_a2d_START (0)
#define SUB_PMIC_BUCK00_RO_REG_60_b00_state_a2d_END (1)
#define SUB_PMIC_BUCK00_RO_REG_60_b00_pg_START (2)
#define SUB_PMIC_BUCK00_RO_REG_60_b00_pg_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b01_cot_atest : 1;
        unsigned char b01_cmp_atest : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK01_CFG_REG_0_UNION;
#endif
#define SUB_PMIC_BUCK01_CFG_REG_0_b01_cot_atest_START (0)
#define SUB_PMIC_BUCK01_CFG_REG_0_b01_cot_atest_END (0)
#define SUB_PMIC_BUCK01_CFG_REG_0_b01_cmp_atest_START (1)
#define SUB_PMIC_BUCK01_CFG_REG_0_b01_cmp_atest_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b01_cot_ton_ecoa : 8;
    } reg;
} SUB_PMIC_BUCK01_CFG_REG_1_UNION;
#endif
#define SUB_PMIC_BUCK01_CFG_REG_1_b01_cot_ton_ecoa_START (0)
#define SUB_PMIC_BUCK01_CFG_REG_1_b01_cot_ton_ecoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b01_cot_ton_ecob : 3;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK01_CFG_REG_2_UNION;
#endif
#define SUB_PMIC_BUCK01_CFG_REG_2_b01_cot_ton_ecob_START (0)
#define SUB_PMIC_BUCK01_CFG_REG_2_b01_cot_ton_ecob_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b01_cot_ton_necoa : 8;
    } reg;
} SUB_PMIC_BUCK01_CFG_REG_3_UNION;
#endif
#define SUB_PMIC_BUCK01_CFG_REG_3_b01_cot_ton_necoa_START (0)
#define SUB_PMIC_BUCK01_CFG_REG_3_b01_cot_ton_necoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b01_dmd_man_set : 4;
        unsigned char b01_dmd_atest : 1;
        unsigned char b01_cot_ton_necob : 3;
    } reg;
} SUB_PMIC_BUCK01_CFG_REG_4_UNION;
#endif
#define SUB_PMIC_BUCK01_CFG_REG_4_b01_dmd_man_set_START (0)
#define SUB_PMIC_BUCK01_CFG_REG_4_b01_dmd_man_set_END (3)
#define SUB_PMIC_BUCK01_CFG_REG_4_b01_dmd_atest_START (4)
#define SUB_PMIC_BUCK01_CFG_REG_4_b01_dmd_atest_END (4)
#define SUB_PMIC_BUCK01_CFG_REG_4_b01_cot_ton_necob_START (5)
#define SUB_PMIC_BUCK01_CFG_REG_4_b01_cot_ton_necob_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b01_drv_sw_sel : 1;
        unsigned char b01_drv_dt_sel : 4;
        unsigned char b01_dmd_ngc_set : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK01_CFG_REG_5_UNION;
#endif
#define SUB_PMIC_BUCK01_CFG_REG_5_b01_drv_sw_sel_START (0)
#define SUB_PMIC_BUCK01_CFG_REG_5_b01_drv_sw_sel_END (0)
#define SUB_PMIC_BUCK01_CFG_REG_5_b01_drv_dt_sel_START (1)
#define SUB_PMIC_BUCK01_CFG_REG_5_b01_drv_dt_sel_END (4)
#define SUB_PMIC_BUCK01_CFG_REG_5_b01_dmd_ngc_set_START (5)
#define SUB_PMIC_BUCK01_CFG_REG_5_b01_dmd_ngc_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b01_dt_sel : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK01_CFG_REG_6_UNION;
#endif
#define SUB_PMIC_BUCK01_CFG_REG_6_b01_dt_sel_START (0)
#define SUB_PMIC_BUCK01_CFG_REG_6_b01_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b01_ng_n_sel : 3;
        unsigned char b01_ng_dt_sel : 3;
        unsigned char b01_n_sel : 2;
    } reg;
} SUB_PMIC_BUCK01_CFG_REG_7_UNION;
#endif
#define SUB_PMIC_BUCK01_CFG_REG_7_b01_ng_n_sel_START (0)
#define SUB_PMIC_BUCK01_CFG_REG_7_b01_ng_n_sel_END (2)
#define SUB_PMIC_BUCK01_CFG_REG_7_b01_ng_dt_sel_START (3)
#define SUB_PMIC_BUCK01_CFG_REG_7_b01_ng_dt_sel_END (5)
#define SUB_PMIC_BUCK01_CFG_REG_7_b01_n_sel_START (6)
#define SUB_PMIC_BUCK01_CFG_REG_7_b01_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b01_pg_dt_sel : 1;
        unsigned char b01_p_sel : 2;
        unsigned char b01_npocp_atest : 1;
        unsigned char b01_ng_p_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK01_CFG_REG_8_UNION;
#endif
#define SUB_PMIC_BUCK01_CFG_REG_8_b01_pg_dt_sel_START (0)
#define SUB_PMIC_BUCK01_CFG_REG_8_b01_pg_dt_sel_END (0)
#define SUB_PMIC_BUCK01_CFG_REG_8_b01_p_sel_START (1)
#define SUB_PMIC_BUCK01_CFG_REG_8_b01_p_sel_END (2)
#define SUB_PMIC_BUCK01_CFG_REG_8_b01_npocp_atest_START (3)
#define SUB_PMIC_BUCK01_CFG_REG_8_b01_npocp_atest_END (3)
#define SUB_PMIC_BUCK01_CFG_REG_8_b01_ng_p_sel_START (4)
#define SUB_PMIC_BUCK01_CFG_REG_8_b01_ng_p_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b01_ramp_buff_trim_test : 1;
        unsigned char b01_pocp_sw_atest : 1;
        unsigned char b01_pg_p_sel : 3;
        unsigned char b01_pg_n_sel : 3;
    } reg;
} SUB_PMIC_BUCK01_CFG_REG_9_UNION;
#endif
#define SUB_PMIC_BUCK01_CFG_REG_9_b01_ramp_buff_trim_test_START (0)
#define SUB_PMIC_BUCK01_CFG_REG_9_b01_ramp_buff_trim_test_END (0)
#define SUB_PMIC_BUCK01_CFG_REG_9_b01_pocp_sw_atest_START (1)
#define SUB_PMIC_BUCK01_CFG_REG_9_b01_pocp_sw_atest_END (1)
#define SUB_PMIC_BUCK01_CFG_REG_9_b01_pg_p_sel_START (2)
#define SUB_PMIC_BUCK01_CFG_REG_9_b01_pg_p_sel_END (4)
#define SUB_PMIC_BUCK01_CFG_REG_9_b01_pg_n_sel_START (5)
#define SUB_PMIC_BUCK01_CFG_REG_9_b01_pg_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b01_ron_test_sel : 3;
        unsigned char b01_ramp_res_test : 1;
        unsigned char b01_ramp_r_ccm : 4;
    } reg;
} SUB_PMIC_BUCK01_CFG_REG_10_UNION;
#endif
#define SUB_PMIC_BUCK01_CFG_REG_10_b01_ron_test_sel_START (0)
#define SUB_PMIC_BUCK01_CFG_REG_10_b01_ron_test_sel_END (2)
#define SUB_PMIC_BUCK01_CFG_REG_10_b01_ramp_res_test_START (3)
#define SUB_PMIC_BUCK01_CFG_REG_10_b01_ramp_res_test_END (3)
#define SUB_PMIC_BUCK01_CFG_REG_10_b01_ramp_r_ccm_START (4)
#define SUB_PMIC_BUCK01_CFG_REG_10_b01_ramp_r_ccm_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_bw_sel : 1;
        unsigned char b30_buffer_bias : 2;
        unsigned char b30_b30_ea_r : 3;
        unsigned char b30_avg_curr_filter_sel : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_0_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_0_b30_bw_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_0_b30_bw_sel_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_0_b30_buffer_bias_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_0_b30_buffer_bias_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_0_b30_b30_ea_r_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_0_b30_b30_ea_r_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_0_b30_avg_curr_filter_sel_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_0_b30_avg_curr_filter_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_c1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_1_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_1_b30_c1_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_1_b30_c1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_c2_sel : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_2_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_2_b30_c2_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_2_b30_c2_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_clk_chopper_sel : 2;
        unsigned char b30_c3_sel : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_3_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_3_b30_clk_chopper_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_3_b30_clk_chopper_sel_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_3_b30_c3_sel_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_3_b30_c3_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_clock_atest : 1;
        unsigned char b30_clk9m6_pulse_sel : 2;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_4_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_4_b30_clock_atest_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_4_b30_clock_atest_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_4_b30_clk9m6_pulse_sel_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_4_b30_clk9m6_pulse_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_clock_sel : 8;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_5_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_5_b30_clock_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_5_b30_clock_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_clp_reg_idrp : 4;
        unsigned char b30_clp_ref_sel : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_6_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_6_b30_clp_reg_idrp_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_6_b30_clp_reg_idrp_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_6_b30_clp_ref_sel_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_6_b30_clp_ref_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_cot_cmphys : 1;
        unsigned char b30_cot_atest : 1;
        unsigned char b30_code_mode_sel : 1;
        unsigned char b30_cmp_atest : 1;
        unsigned char b30_clpeco_bias : 4;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_7_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_7_b30_cot_cmphys_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_7_b30_cot_cmphys_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_7_b30_cot_atest_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_7_b30_cot_atest_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_7_b30_code_mode_sel_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_7_b30_code_mode_sel_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_7_b30_cmp_atest_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_7_b30_cmp_atest_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_7_b30_clpeco_bias_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_7_b30_clpeco_bias_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_cot_sel : 1;
        unsigned char b30_cot_sche_neco : 1;
        unsigned char b30_cot_sche_eco : 1;
        unsigned char b30_cot_res_sel : 3;
        unsigned char b30_cot_iboost_sel : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_8_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_8_b30_cot_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_8_b30_cot_sel_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_8_b30_cot_sche_neco_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_8_b30_cot_sche_neco_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_8_b30_cot_sche_eco_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_8_b30_cot_sche_eco_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_8_b30_cot_res_sel_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_8_b30_cot_res_sel_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_8_b30_cot_iboost_sel_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_8_b30_cot_iboost_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_cot_ton_ecoa : 8;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_9_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_9_b30_cot_ton_ecoa_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_9_b30_cot_ton_ecoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_cot_ton_isen_hys : 2;
        unsigned char b30_cot_ton_isen_bias : 2;
        unsigned char b30_cot_ton_ecob : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_10_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_10_b30_cot_ton_isen_hys_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_10_b30_cot_ton_isen_hys_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_10_b30_cot_ton_isen_bias_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_10_b30_cot_ton_isen_bias_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_10_b30_cot_ton_ecob_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_10_b30_cot_ton_ecob_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_cot_ton_necoa : 8;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_11_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_11_b30_cot_ton_necoa_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_11_b30_cot_ton_necoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_crtshare_bw_sel : 2;
        unsigned char b30_crtshare_atest : 1;
        unsigned char b30_cot_ton_necob : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_12_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_12_b30_crtshare_bw_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_12_b30_crtshare_bw_sel_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_12_b30_crtshare_atest_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_12_b30_crtshare_atest_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_12_b30_cot_ton_necob_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_12_b30_cot_ton_necob_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_cs_test_sel : 1;
        unsigned char b30_crtshare_trim : 1;
        unsigned char b30_crtshare_filter_r_sel : 1;
        unsigned char b30_crtshare_clp_sel : 4;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_13_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_13_b30_cs_test_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_13_b30_cs_test_sel_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_13_b30_crtshare_trim_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_13_b30_crtshare_trim_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_13_b30_crtshare_filter_r_sel_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_13_b30_crtshare_filter_r_sel_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_13_b30_crtshare_clp_sel_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_13_b30_crtshare_clp_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_dbias : 3;
        unsigned char b30_ctlogic_smph : 1;
        unsigned char b30_ctlogic_sgph : 1;
        unsigned char b30_ctlogic_modsw : 1;
        unsigned char b30_ctlogic_crson_td : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_14_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_14_b30_dbias_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_14_b30_dbias_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_14_b30_ctlogic_smph_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_14_b30_ctlogic_smph_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_14_b30_ctlogic_sgph_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_14_b30_ctlogic_sgph_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_14_b30_ctlogic_modsw_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_14_b30_ctlogic_modsw_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_14_b30_ctlogic_crson_td_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_14_b30_ctlogic_crson_td_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_dmd_auto_eco : 1;
        unsigned char b30_dmd_auto_cmpib : 2;
        unsigned char b30_dmd_auto_ccm : 1;
        unsigned char b30_dmd_atest : 1;
        unsigned char b30_dbias_eco : 3;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_15_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_15_b30_dmd_auto_eco_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_15_b30_dmd_auto_eco_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_15_b30_dmd_auto_cmpib_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_15_b30_dmd_auto_cmpib_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_15_b30_dmd_auto_ccm_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_15_b30_dmd_auto_ccm_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_15_b30_dmd_atest_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_15_b30_dmd_atest_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_15_b30_dbias_eco_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_15_b30_dbias_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_dmd_bck_sel : 1;
        unsigned char b30_dmd_auto_vos : 3;
        unsigned char b30_dmd_auto_vbc : 1;
        unsigned char b30_dmd_auto_sel : 1;
        unsigned char b30_dmd_auto_sampt : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_16_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_16_b30_dmd_bck_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_16_b30_dmd_bck_sel_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_16_b30_dmd_auto_vos_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_16_b30_dmd_auto_vos_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_16_b30_dmd_auto_vbc_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_16_b30_dmd_auto_vbc_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_16_b30_dmd_auto_sel_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_16_b30_dmd_auto_sel_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_16_b30_dmd_auto_sampt_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_16_b30_dmd_auto_sampt_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_dmd_ngc_sel : 1;
        unsigned char b30_dmd_ngc_disch : 1;
        unsigned char b30_dmd_man_set : 4;
        unsigned char b30_dmd_eco_sel : 1;
        unsigned char b30_dmd_clp_sel : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_17_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_17_b30_dmd_ngc_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_17_b30_dmd_ngc_sel_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_17_b30_dmd_ngc_disch_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_17_b30_dmd_ngc_disch_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_17_b30_dmd_man_set_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_17_b30_dmd_man_set_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_17_b30_dmd_eco_sel_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_17_b30_dmd_eco_sel_END (6)
#define SUB_PMIC_BUCK30_CFG_REG_17_b30_dmd_clp_sel_START (7)
#define SUB_PMIC_BUCK30_CFG_REG_17_b30_dmd_clp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_drv_sw_sel : 1;
        unsigned char b30_drv_dt_sel : 4;
        unsigned char b30_dmd_ngc_set : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_18_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_18_b30_drv_sw_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_18_b30_drv_sw_sel_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_18_b30_drv_dt_sel_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_18_b30_drv_dt_sel_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_18_b30_dmd_ngc_set_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_18_b30_dmd_ngc_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_dt_sel : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_19_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_19_b30_dt_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_19_b30_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_eabias : 1;
        unsigned char b30_eaamp_clp_sel : 1;
        unsigned char b30_eaamp_clp : 2;
        unsigned char b30_ea_ecocur_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_20_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_20_b30_eabias_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_20_b30_eabias_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_20_b30_eaamp_clp_sel_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_20_b30_eaamp_clp_sel_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_20_b30_eaamp_clp_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_20_b30_eaamp_clp_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_20_b30_ea_ecocur_sel_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_20_b30_ea_ecocur_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_eaclp_ecogap_sel : 2;
        unsigned char b30_eaclp_brgap_sel : 4;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_21_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_21_b30_eaclp_ecogap_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_21_b30_eaclp_ecogap_sel_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_21_b30_eaclp_brgap_sel_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_21_b30_eaclp_brgap_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_eacomp_eco_shield : 1;
        unsigned char b30_eaclp_srgap_sel : 4;
        unsigned char b30_eaclp_gap_sel : 3;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_22_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_22_b30_eacomp_eco_shield_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_22_b30_eacomp_eco_shield_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_22_b30_eaclp_srgap_sel_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_22_b30_eaclp_srgap_sel_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_22_b30_eaclp_gap_sel_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_22_b30_eaclp_gap_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_ecdt_ct2cm_ref : 3;
        unsigned char b30_ecdt_ct2cm_ft : 2;
        unsigned char b30_ecdt_cmphys : 1;
        unsigned char b30_eatrim_en : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_23_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_23_b30_ecdt_ct2cm_ref_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_23_b30_ecdt_ct2cm_ref_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_23_b30_ecdt_ct2cm_ft_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_23_b30_ecdt_ct2cm_ft_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_23_b30_ecdt_cmphys_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_23_b30_ecdt_cmphys_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_23_b30_eatrim_en_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_23_b30_eatrim_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_ecdt_ec2ct_ref : 3;
        unsigned char b30_ecdt_ct2ec_ref : 3;
        unsigned char b30_ecdt_ct2ec_ft : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_24_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_24_b30_ecdt_ec2ct_ref_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_24_b30_ecdt_ec2ct_ref_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_24_b30_ecdt_ct2ec_ref_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_24_b30_ecdt_ct2ec_ref_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_24_b30_ecdt_ct2ec_ft_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_24_b30_ecdt_ct2ec_ft_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_en_psns : 1;
        unsigned char b30_en_nsense : 1;
        unsigned char b30_en_chopper : 1;
        unsigned char b30_ecotrim_shield : 1;
        unsigned char b30_eco_shield_cmp : 1;
        unsigned char b30_eco_shield : 1;
        unsigned char b30_ecdt_filter_rc : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_25_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_25_b30_en_psns_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_25_b30_en_psns_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_25_b30_en_nsense_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_25_b30_en_nsense_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_25_b30_en_chopper_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_25_b30_en_chopper_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_25_b30_ecotrim_shield_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_25_b30_ecotrim_shield_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_25_b30_eco_shield_cmp_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_25_b30_eco_shield_cmp_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_25_b30_eco_shield_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_25_b30_eco_shield_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_25_b30_ecdt_filter_rc_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_25_b30_ecdt_filter_rc_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_ldline_forced_on : 1;
        unsigned char b30_ibalance_gm_sel : 2;
        unsigned char b30_forcenormal : 1;
        unsigned char b30_forceeco : 1;
        unsigned char b30_fastnmos_off : 1;
        unsigned char b30_en_vo : 1;
        unsigned char b30_en_test : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_26_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_26_b30_ldline_forced_on_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_26_b30_ldline_forced_on_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_26_b30_ibalance_gm_sel_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_26_b30_ibalance_gm_sel_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_26_b30_forcenormal_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_26_b30_forcenormal_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_26_b30_forceeco_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_26_b30_forceeco_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_26_b30_fastnmos_off_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_26_b30_fastnmos_off_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_26_b30_en_vo_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_26_b30_en_vo_END (6)
#define SUB_PMIC_BUCK30_CFG_REG_26_b30_en_test_START (7)
#define SUB_PMIC_BUCK30_CFG_REG_26_b30_en_test_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_load_det_sel : 2;
        unsigned char b30_ldline_mohm_sel : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_27_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_27_b30_load_det_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_27_b30_load_det_sel_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_27_b30_ldline_mohm_sel_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_27_b30_ldline_mohm_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_min_ton_sel : 2;
        unsigned char b30_min_toff_sel : 2;
        unsigned char b30_load_state_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_28_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_28_b30_min_ton_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_28_b30_min_ton_sel_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_28_b30_min_toff_sel_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_28_b30_min_toff_sel_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_28_b30_load_state_sel_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_28_b30_load_state_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_ng_dt_sel : 3;
        unsigned char b30_negdmdton_add : 1;
        unsigned char b30_negdmd_ton_sel : 2;
        unsigned char b30_n_sel : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_29_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_29_b30_ng_dt_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_29_b30_ng_dt_sel_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_29_b30_negdmdton_add_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_29_b30_negdmdton_add_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_29_b30_negdmd_ton_sel_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_29_b30_negdmd_ton_sel_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_29_b30_n_sel_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_29_b30_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_nmos_off : 1;
        unsigned char b30_ng_p_sel : 3;
        unsigned char b30_ng_n_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_30_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_30_b30_nmos_off_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_30_b30_nmos_off_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_30_b30_ng_p_sel_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_30_b30_ng_p_sel_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_30_b30_ng_n_sel_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_30_b30_ng_n_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_nocp_low_sel : 2;
        unsigned char b30_nocp_low_eco_sel : 1;
        unsigned char b30_nocp_low_delay_sel : 2;
        unsigned char b30_nocp_en : 1;
        unsigned char b30_nocp_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_31_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_31_b30_nocp_low_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_31_b30_nocp_low_sel_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_31_b30_nocp_low_eco_sel_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_31_b30_nocp_low_eco_sel_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_31_b30_nocp_low_delay_sel_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_31_b30_nocp_low_delay_sel_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_31_b30_nocp_en_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_31_b30_nocp_en_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_31_b30_nocp_delay_sel_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_31_b30_nocp_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_ocp_2us_delay_sel : 1;
        unsigned char b30_nsense_sel : 3;
        unsigned char b30_npocp_atest : 1;
        unsigned char b30_normaltrim_shield : 1;
        unsigned char b30_nocp_sel : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_32_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_32_b30_ocp_2us_delay_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_32_b30_ocp_2us_delay_sel_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_32_b30_nsense_sel_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_32_b30_nsense_sel_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_32_b30_npocp_atest_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_32_b30_npocp_atest_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_32_b30_normaltrim_shield_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_32_b30_normaltrim_shield_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_32_b30_nocp_sel_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_32_b30_nocp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_p_sel : 2;
        unsigned char b30_ovp_shield : 1;
        unsigned char b30_ovp_sel : 2;
        unsigned char b30_oneph : 1;
        unsigned char b30_ocp_short_en : 1;
        unsigned char b30_ocp_shield : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_33_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_33_b30_p_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_33_b30_p_sel_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_33_b30_ovp_shield_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_33_b30_ovp_shield_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_33_b30_ovp_sel_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_33_b30_ovp_sel_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_33_b30_oneph_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_33_b30_oneph_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_33_b30_ocp_short_en_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_33_b30_ocp_short_en_END (6)
#define SUB_PMIC_BUCK30_CFG_REG_33_b30_ocp_shield_START (7)
#define SUB_PMIC_BUCK30_CFG_REG_33_b30_ocp_shield_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_pg_p_sel : 3;
        unsigned char b30_pg_n_sel : 3;
        unsigned char b30_pg_dt_sel : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_34_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_34_b30_pg_p_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_34_b30_pg_p_sel_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_34_b30_pg_n_sel_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_34_b30_pg_n_sel_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_34_b30_pg_dt_sel_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_34_b30_pg_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_pocp_sw_ccm_cmp : 1;
        unsigned char b30_pocp_sw_atest : 1;
        unsigned char b30_pocp_sel : 2;
        unsigned char b30_pocp_en : 1;
        unsigned char b30_pocp_eco_sel : 1;
        unsigned char b30_pocp_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_35_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_35_b30_pocp_sw_ccm_cmp_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_35_b30_pocp_sw_ccm_cmp_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_35_b30_pocp_sw_atest_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_35_b30_pocp_sw_atest_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_35_b30_pocp_sel_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_35_b30_pocp_sel_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_35_b30_pocp_en_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_35_b30_pocp_en_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_35_b30_pocp_eco_sel_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_35_b30_pocp_eco_sel_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_35_b30_pocp_delay_sel_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_35_b30_pocp_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_pulldn_pd_sel : 1;
        unsigned char b30_pulldn_pd_en : 1;
        unsigned char b30_pocp_sw_sel : 3;
        unsigned char b30_pocp_sw_eco_cmp : 1;
        unsigned char b30_pocp_sw_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_36_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_36_b30_pulldn_pd_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_36_b30_pulldn_pd_sel_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_36_b30_pulldn_pd_en_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_36_b30_pulldn_pd_en_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_36_b30_pocp_sw_sel_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_36_b30_pocp_sw_sel_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_36_b30_pocp_sw_eco_cmp_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_36_b30_pocp_sw_eco_cmp_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_36_b30_pocp_sw_delay_sel_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_36_b30_pocp_sw_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_r1_sel : 4;
        unsigned char b30_pvdd_ton_sel : 1;
        unsigned char b30_pulldn_sel : 3;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_37_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_37_b30_r1_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_37_b30_r1_sel_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_37_b30_pvdd_ton_sel_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_37_b30_pvdd_ton_sel_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_37_b30_pulldn_sel_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_37_b30_pulldn_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_r2_sel : 6;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_38_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_38_b30_r2_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_38_b30_r2_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_ramp_big_sel : 1;
        unsigned char b30_r3_sel : 6;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_39_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_39_b30_ramp_big_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_39_b30_ramp_big_sel_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_39_b30_r3_sel_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_39_b30_r3_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_ramp_ccm_outsel : 1;
        unsigned char b30_ramp_c : 2;
        unsigned char b30_ramp_buff_trim_test : 1;
        unsigned char b30_ramp_buff_trim : 1;
        unsigned char b30_ramp_buff_bias : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_40_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_40_b30_ramp_ccm_outsel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_40_b30_ramp_ccm_outsel_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_40_b30_ramp_c_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_40_b30_ramp_c_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_40_b30_ramp_buff_trim_test_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_40_b30_ramp_buff_trim_test_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_40_b30_ramp_buff_trim_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_40_b30_ramp_buff_trim_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_40_b30_ramp_buff_bias_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_40_b30_ramp_buff_bias_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_ramp_cot_outsel : 1;
        unsigned char b30_ramp_clk_sel : 1;
        unsigned char b30_ramp_change_mode : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_41_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_41_b30_ramp_cot_outsel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_41_b30_ramp_cot_outsel_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_41_b30_ramp_clk_sel_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_41_b30_ramp_clk_sel_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_41_b30_ramp_change_mode_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_41_b30_ramp_change_mode_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_ramp_line_sel : 1;
        unsigned char b30_ramp_freq_sel : 1;
        unsigned char b30_ramp_force_eco_test : 1;
        unsigned char b30_ramp_fast_sel : 1;
        unsigned char b30_ramp_eco_buffer : 1;
        unsigned char b30_ramp_dblclk_sel : 1;
        unsigned char b30_ramp_counter : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_42_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_42_b30_ramp_line_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_42_b30_ramp_line_sel_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_42_b30_ramp_freq_sel_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_42_b30_ramp_freq_sel_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_42_b30_ramp_force_eco_test_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_42_b30_ramp_force_eco_test_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_42_b30_ramp_fast_sel_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_42_b30_ramp_fast_sel_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_42_b30_ramp_eco_buffer_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_42_b30_ramp_eco_buffer_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_42_b30_ramp_dblclk_sel_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_42_b30_ramp_dblclk_sel_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_42_b30_ramp_counter_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_42_b30_ramp_counter_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_ramp_r_ccm : 4;
        unsigned char b30_ramp_mid_sel : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_43_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_43_b30_ramp_r_ccm_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_43_b30_ramp_r_ccm_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_43_b30_ramp_mid_sel_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_43_b30_ramp_mid_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_ramp_r_eco : 4;
        unsigned char b30_ramp_r_cot : 4;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_44_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_44_b30_ramp_r_eco_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_44_b30_ramp_r_eco_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_44_b30_ramp_r_cot_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_44_b30_ramp_r_cot_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_ramptrim_sel : 1;
        unsigned char b30_ramp_valley_sel : 1;
        unsigned char b30_ramp_trim_sel : 1;
        unsigned char b30_ramp_sw_sel : 1;
        unsigned char b30_ramp_sel : 1;
        unsigned char b30_ramp_res_test : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_45_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_45_b30_ramptrim_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_45_b30_ramptrim_sel_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_45_b30_ramp_valley_sel_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_45_b30_ramp_valley_sel_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_45_b30_ramp_trim_sel_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_45_b30_ramp_trim_sel_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_45_b30_ramp_sw_sel_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_45_b30_ramp_sw_sel_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_45_b30_ramp_sel_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_45_b30_ramp_sel_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_45_b30_ramp_res_test_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_45_b30_ramp_res_test_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_reg_idrp : 3;
        unsigned char b30_reg_idrn : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_46_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_46_b30_reg_idrp_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_46_b30_reg_idrp_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_46_b30_reg_idrn_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_46_b30_reg_idrn_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_reg_sense_res_sel : 3;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_47_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_47_b30_reg_sense_res_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_47_b30_reg_sense_res_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_reserve0 : 8;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_48_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_48_b30_reserve0_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_48_b30_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_reserve1 : 8;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_49_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_49_b30_reserve1_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_49_b30_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_reserve2 : 8;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_50_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_50_b30_reserve2_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_50_b30_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_reserve3 : 8;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_51_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_51_b30_reserve3_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_51_b30_reserve3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_sft : 1;
        unsigned char b30_sense_ratio_sel : 2;
        unsigned char b30_rtr_cap_sel : 2;
        unsigned char b30_ron_test_sel : 3;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_52_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_52_b30_sft_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_52_b30_sft_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_52_b30_sense_ratio_sel_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_52_b30_sense_ratio_sel_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_52_b30_rtr_cap_sel_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_52_b30_rtr_cap_sel_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_52_b30_ron_test_sel_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_52_b30_ron_test_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_stsw_atest : 1;
        unsigned char b30_state_sw_ocp_sel : 2;
        unsigned char b30_state_sw_ocp_off : 1;
        unsigned char b30_slave_sel : 1;
        unsigned char b30_short_shield : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_53_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_53_b30_stsw_atest_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_53_b30_stsw_atest_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_53_b30_state_sw_ocp_sel_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_53_b30_state_sw_ocp_sel_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_53_b30_state_sw_ocp_off_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_53_b30_state_sw_ocp_off_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_53_b30_slave_sel_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_53_b30_slave_sel_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_53_b30_short_shield_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_53_b30_short_shield_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_stsw_fstsch_eco : 3;
        unsigned char b30_stsw_fstps_ith : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_54_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_54_b30_stsw_fstsch_eco_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_54_b30_stsw_fstsch_eco_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_54_b30_stsw_fstps_ith_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_54_b30_stsw_fstps_ith_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_stsw_slw_ct2cm_hys : 2;
        unsigned char b30_stsw_rpup_ccm : 1;
        unsigned char b30_stsw_man_set : 2;
        unsigned char b30_stsw_fstsch_neco : 3;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_55_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_55_b30_stsw_slw_ct2cm_hys_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_55_b30_stsw_slw_ct2cm_hys_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_55_b30_stsw_rpup_ccm_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_55_b30_stsw_rpup_ccm_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_55_b30_stsw_man_set_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_55_b30_stsw_man_set_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_55_b30_stsw_fstsch_neco_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_55_b30_stsw_fstsch_neco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_stsw_slw_hys_shield : 1;
        unsigned char b30_stsw_slw_delay_l : 2;
        unsigned char b30_stsw_slw_delay_h : 2;
        unsigned char b30_stsw_slw_ct2cm_ith : 3;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_56_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_56_b30_stsw_slw_hys_shield_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_56_b30_stsw_slw_hys_shield_END (0)
#define SUB_PMIC_BUCK30_CFG_REG_56_b30_stsw_slw_delay_l_START (1)
#define SUB_PMIC_BUCK30_CFG_REG_56_b30_stsw_slw_delay_l_END (2)
#define SUB_PMIC_BUCK30_CFG_REG_56_b30_stsw_slw_delay_h_START (3)
#define SUB_PMIC_BUCK30_CFG_REG_56_b30_stsw_slw_delay_h_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_56_b30_stsw_slw_ct2cm_ith_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_56_b30_stsw_slw_ct2cm_ith_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_ton_det : 4;
        unsigned char b30_stsw_slwsch_neco : 2;
        unsigned char b30_stsw_slwsch_eco : 2;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_57_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_57_b30_ton_det_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_57_b30_ton_det_END (3)
#define SUB_PMIC_BUCK30_CFG_REG_57_b30_stsw_slwsch_neco_START (4)
#define SUB_PMIC_BUCK30_CFG_REG_57_b30_stsw_slwsch_neco_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_57_b30_stsw_slwsch_eco_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_57_b30_stsw_slwsch_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_vref_test_sel : 2;
        unsigned char b30_vo_ton_res_sel : 3;
        unsigned char b30_trim2 : 1;
        unsigned char b30_trim1 : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_58_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_58_b30_vref_test_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_58_b30_vref_test_sel_END (1)
#define SUB_PMIC_BUCK30_CFG_REG_58_b30_vo_ton_res_sel_START (2)
#define SUB_PMIC_BUCK30_CFG_REG_58_b30_vo_ton_res_sel_END (4)
#define SUB_PMIC_BUCK30_CFG_REG_58_b30_trim2_START (5)
#define SUB_PMIC_BUCK30_CFG_REG_58_b30_trim2_END (5)
#define SUB_PMIC_BUCK30_CFG_REG_58_b30_trim1_START (6)
#define SUB_PMIC_BUCK30_CFG_REG_58_b30_trim1_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_vref_vo_cap_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK30_CFG_REG_59_UNION;
#endif
#define SUB_PMIC_BUCK30_CFG_REG_59_b30_vref_vo_cap_sel_START (0)
#define SUB_PMIC_BUCK30_CFG_REG_59_b30_vref_vo_cap_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b30_state_a2d : 2;
        unsigned char b30_pg : 1;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK30_RO_REG_60_UNION;
#endif
#define SUB_PMIC_BUCK30_RO_REG_60_b30_state_a2d_START (0)
#define SUB_PMIC_BUCK30_RO_REG_60_b30_state_a2d_END (1)
#define SUB_PMIC_BUCK30_RO_REG_60_b30_pg_START (2)
#define SUB_PMIC_BUCK30_RO_REG_60_b30_pg_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b31_cot_atest : 1;
        unsigned char b31_cmp_atest : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK31_CFG_REG_0_UNION;
#endif
#define SUB_PMIC_BUCK31_CFG_REG_0_b31_cot_atest_START (0)
#define SUB_PMIC_BUCK31_CFG_REG_0_b31_cot_atest_END (0)
#define SUB_PMIC_BUCK31_CFG_REG_0_b31_cmp_atest_START (1)
#define SUB_PMIC_BUCK31_CFG_REG_0_b31_cmp_atest_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b31_cot_ton_ecoa : 8;
    } reg;
} SUB_PMIC_BUCK31_CFG_REG_1_UNION;
#endif
#define SUB_PMIC_BUCK31_CFG_REG_1_b31_cot_ton_ecoa_START (0)
#define SUB_PMIC_BUCK31_CFG_REG_1_b31_cot_ton_ecoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b31_cot_ton_ecob : 3;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK31_CFG_REG_2_UNION;
#endif
#define SUB_PMIC_BUCK31_CFG_REG_2_b31_cot_ton_ecob_START (0)
#define SUB_PMIC_BUCK31_CFG_REG_2_b31_cot_ton_ecob_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b31_cot_ton_necoa : 8;
    } reg;
} SUB_PMIC_BUCK31_CFG_REG_3_UNION;
#endif
#define SUB_PMIC_BUCK31_CFG_REG_3_b31_cot_ton_necoa_START (0)
#define SUB_PMIC_BUCK31_CFG_REG_3_b31_cot_ton_necoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b31_dmd_man_set : 4;
        unsigned char b31_dmd_atest : 1;
        unsigned char b31_cot_ton_necob : 3;
    } reg;
} SUB_PMIC_BUCK31_CFG_REG_4_UNION;
#endif
#define SUB_PMIC_BUCK31_CFG_REG_4_b31_dmd_man_set_START (0)
#define SUB_PMIC_BUCK31_CFG_REG_4_b31_dmd_man_set_END (3)
#define SUB_PMIC_BUCK31_CFG_REG_4_b31_dmd_atest_START (4)
#define SUB_PMIC_BUCK31_CFG_REG_4_b31_dmd_atest_END (4)
#define SUB_PMIC_BUCK31_CFG_REG_4_b31_cot_ton_necob_START (5)
#define SUB_PMIC_BUCK31_CFG_REG_4_b31_cot_ton_necob_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b31_drv_sw_sel : 1;
        unsigned char b31_drv_dt_sel : 4;
        unsigned char b31_dmd_ngc_set : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK31_CFG_REG_5_UNION;
#endif
#define SUB_PMIC_BUCK31_CFG_REG_5_b31_drv_sw_sel_START (0)
#define SUB_PMIC_BUCK31_CFG_REG_5_b31_drv_sw_sel_END (0)
#define SUB_PMIC_BUCK31_CFG_REG_5_b31_drv_dt_sel_START (1)
#define SUB_PMIC_BUCK31_CFG_REG_5_b31_drv_dt_sel_END (4)
#define SUB_PMIC_BUCK31_CFG_REG_5_b31_dmd_ngc_set_START (5)
#define SUB_PMIC_BUCK31_CFG_REG_5_b31_dmd_ngc_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b31_dt_sel : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK31_CFG_REG_6_UNION;
#endif
#define SUB_PMIC_BUCK31_CFG_REG_6_b31_dt_sel_START (0)
#define SUB_PMIC_BUCK31_CFG_REG_6_b31_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b31_ng_n_sel : 3;
        unsigned char b31_ng_dt_sel : 3;
        unsigned char b31_n_sel : 2;
    } reg;
} SUB_PMIC_BUCK31_CFG_REG_7_UNION;
#endif
#define SUB_PMIC_BUCK31_CFG_REG_7_b31_ng_n_sel_START (0)
#define SUB_PMIC_BUCK31_CFG_REG_7_b31_ng_n_sel_END (2)
#define SUB_PMIC_BUCK31_CFG_REG_7_b31_ng_dt_sel_START (3)
#define SUB_PMIC_BUCK31_CFG_REG_7_b31_ng_dt_sel_END (5)
#define SUB_PMIC_BUCK31_CFG_REG_7_b31_n_sel_START (6)
#define SUB_PMIC_BUCK31_CFG_REG_7_b31_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b31_pg_dt_sel : 1;
        unsigned char b31_p_sel : 2;
        unsigned char b31_npocp_atest : 1;
        unsigned char b31_ng_p_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK31_CFG_REG_8_UNION;
#endif
#define SUB_PMIC_BUCK31_CFG_REG_8_b31_pg_dt_sel_START (0)
#define SUB_PMIC_BUCK31_CFG_REG_8_b31_pg_dt_sel_END (0)
#define SUB_PMIC_BUCK31_CFG_REG_8_b31_p_sel_START (1)
#define SUB_PMIC_BUCK31_CFG_REG_8_b31_p_sel_END (2)
#define SUB_PMIC_BUCK31_CFG_REG_8_b31_npocp_atest_START (3)
#define SUB_PMIC_BUCK31_CFG_REG_8_b31_npocp_atest_END (3)
#define SUB_PMIC_BUCK31_CFG_REG_8_b31_ng_p_sel_START (4)
#define SUB_PMIC_BUCK31_CFG_REG_8_b31_ng_p_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b31_ramp_buff_trim_test : 1;
        unsigned char b31_pocp_sw_atest : 1;
        unsigned char b31_pg_p_sel : 3;
        unsigned char b31_pg_n_sel : 3;
    } reg;
} SUB_PMIC_BUCK31_CFG_REG_9_UNION;
#endif
#define SUB_PMIC_BUCK31_CFG_REG_9_b31_ramp_buff_trim_test_START (0)
#define SUB_PMIC_BUCK31_CFG_REG_9_b31_ramp_buff_trim_test_END (0)
#define SUB_PMIC_BUCK31_CFG_REG_9_b31_pocp_sw_atest_START (1)
#define SUB_PMIC_BUCK31_CFG_REG_9_b31_pocp_sw_atest_END (1)
#define SUB_PMIC_BUCK31_CFG_REG_9_b31_pg_p_sel_START (2)
#define SUB_PMIC_BUCK31_CFG_REG_9_b31_pg_p_sel_END (4)
#define SUB_PMIC_BUCK31_CFG_REG_9_b31_pg_n_sel_START (5)
#define SUB_PMIC_BUCK31_CFG_REG_9_b31_pg_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b31_ron_test_sel : 3;
        unsigned char b31_ramp_res_test : 1;
        unsigned char b31_ramp_r_ccm : 4;
    } reg;
} SUB_PMIC_BUCK31_CFG_REG_10_UNION;
#endif
#define SUB_PMIC_BUCK31_CFG_REG_10_b31_ron_test_sel_START (0)
#define SUB_PMIC_BUCK31_CFG_REG_10_b31_ron_test_sel_END (2)
#define SUB_PMIC_BUCK31_CFG_REG_10_b31_ramp_res_test_START (3)
#define SUB_PMIC_BUCK31_CFG_REG_10_b31_ramp_res_test_END (3)
#define SUB_PMIC_BUCK31_CFG_REG_10_b31_ramp_r_ccm_START (4)
#define SUB_PMIC_BUCK31_CFG_REG_10_b31_ramp_r_ccm_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_adj_rlx : 4;
        unsigned char b6_adj_clx : 2;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_0_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_0_b6_adj_rlx_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_0_b6_adj_rlx_END (3)
#define SUB_PMIC_BUCK6_CFG_REG_0_b6_adj_clx_START (4)
#define SUB_PMIC_BUCK6_CFG_REG_0_b6_adj_clx_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_dmd_blanktime_sel : 1;
        unsigned char b6_cot_rlx : 1;
        unsigned char b6_cmp_ibias : 4;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_1_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_1_b6_dmd_blanktime_sel_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_1_b6_dmd_blanktime_sel_END (0)
#define SUB_PMIC_BUCK6_CFG_REG_1_b6_cot_rlx_START (1)
#define SUB_PMIC_BUCK6_CFG_REG_1_b6_cot_rlx_END (1)
#define SUB_PMIC_BUCK6_CFG_REG_1_b6_cmp_ibias_START (2)
#define SUB_PMIC_BUCK6_CFG_REG_1_b6_cmp_ibias_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_dmd_sel_eco : 4;
        unsigned char b6_dmd_sel : 4;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_2_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_2_b6_dmd_sel_eco_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_2_b6_dmd_sel_eco_END (3)
#define SUB_PMIC_BUCK6_CFG_REG_2_b6_dmd_sel_START (4)
#define SUB_PMIC_BUCK6_CFG_REG_2_b6_dmd_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_ea_clp_eco_dis : 1;
        unsigned char b6_dt_sel : 2;
        unsigned char b6_dmd_type_sel : 1;
        unsigned char b6_dmd_ton : 3;
        unsigned char b6_dmd_shield_ton : 1;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_3_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_3_b6_ea_clp_eco_dis_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_3_b6_ea_clp_eco_dis_END (0)
#define SUB_PMIC_BUCK6_CFG_REG_3_b6_dt_sel_START (1)
#define SUB_PMIC_BUCK6_CFG_REG_3_b6_dt_sel_END (2)
#define SUB_PMIC_BUCK6_CFG_REG_3_b6_dmd_type_sel_START (3)
#define SUB_PMIC_BUCK6_CFG_REG_3_b6_dmd_type_sel_END (3)
#define SUB_PMIC_BUCK6_CFG_REG_3_b6_dmd_ton_START (4)
#define SUB_PMIC_BUCK6_CFG_REG_3_b6_dmd_ton_END (6)
#define SUB_PMIC_BUCK6_CFG_REG_3_b6_dmd_shield_ton_START (7)
#define SUB_PMIC_BUCK6_CFG_REG_3_b6_dmd_shield_ton_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_fb_cap_sel : 1;
        unsigned char b6_fastramp_ctrl : 4;
        unsigned char b6_eco_ibias : 1;
        unsigned char b6_ea_ibias : 1;
        unsigned char b6_ea_eco_dis : 1;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_4_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_4_b6_fb_cap_sel_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_4_b6_fb_cap_sel_END (0)
#define SUB_PMIC_BUCK6_CFG_REG_4_b6_fastramp_ctrl_START (1)
#define SUB_PMIC_BUCK6_CFG_REG_4_b6_fastramp_ctrl_END (4)
#define SUB_PMIC_BUCK6_CFG_REG_4_b6_eco_ibias_START (5)
#define SUB_PMIC_BUCK6_CFG_REG_4_b6_eco_ibias_END (5)
#define SUB_PMIC_BUCK6_CFG_REG_4_b6_ea_ibias_START (6)
#define SUB_PMIC_BUCK6_CFG_REG_4_b6_ea_ibias_END (6)
#define SUB_PMIC_BUCK6_CFG_REG_4_b6_ea_eco_dis_START (7)
#define SUB_PMIC_BUCK6_CFG_REG_4_b6_ea_eco_dis_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_min_ton : 3;
        unsigned char b6_lx_dt : 1;
        unsigned char b6_ibias_dmd_ctrl : 2;
        unsigned char b6_fix_ton : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_5_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_5_b6_min_ton_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_5_b6_min_ton_END (2)
#define SUB_PMIC_BUCK6_CFG_REG_5_b6_lx_dt_START (3)
#define SUB_PMIC_BUCK6_CFG_REG_5_b6_lx_dt_END (3)
#define SUB_PMIC_BUCK6_CFG_REG_5_b6_ibias_dmd_ctrl_START (4)
#define SUB_PMIC_BUCK6_CFG_REG_5_b6_ibias_dmd_ctrl_END (5)
#define SUB_PMIC_BUCK6_CFG_REG_5_b6_fix_ton_START (6)
#define SUB_PMIC_BUCK6_CFG_REG_5_b6_fix_ton_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_ntp_dt_ctrl : 1;
        unsigned char b6_nonlinear_driver : 1;
        unsigned char b6_ng_p_sel : 3;
        unsigned char b6_ng_n_sel : 3;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_6_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_6_b6_ntp_dt_ctrl_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_6_b6_ntp_dt_ctrl_END (0)
#define SUB_PMIC_BUCK6_CFG_REG_6_b6_nonlinear_driver_START (1)
#define SUB_PMIC_BUCK6_CFG_REG_6_b6_nonlinear_driver_END (1)
#define SUB_PMIC_BUCK6_CFG_REG_6_b6_ng_p_sel_START (2)
#define SUB_PMIC_BUCK6_CFG_REG_6_b6_ng_p_sel_END (4)
#define SUB_PMIC_BUCK6_CFG_REG_6_b6_ng_n_sel_START (5)
#define SUB_PMIC_BUCK6_CFG_REG_6_b6_ng_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_ocp_sel : 3;
        unsigned char b6_ocp_delay : 1;
        unsigned char b6_ocp_cmp_ibias : 1;
        unsigned char b6_ntp_dt_sel : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_7_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_7_b6_ocp_sel_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_7_b6_ocp_sel_END (2)
#define SUB_PMIC_BUCK6_CFG_REG_7_b6_ocp_delay_START (3)
#define SUB_PMIC_BUCK6_CFG_REG_7_b6_ocp_delay_END (3)
#define SUB_PMIC_BUCK6_CFG_REG_7_b6_ocp_cmp_ibias_START (4)
#define SUB_PMIC_BUCK6_CFG_REG_7_b6_ocp_cmp_ibias_END (4)
#define SUB_PMIC_BUCK6_CFG_REG_7_b6_ntp_dt_sel_START (5)
#define SUB_PMIC_BUCK6_CFG_REG_7_b6_ntp_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_offres_shut : 1;
        unsigned char b6_offres_sel : 1;
        unsigned char b6_ocpibias : 2;
        unsigned char b6_ocp_toff : 2;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_8_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_8_b6_offres_shut_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_8_b6_offres_shut_END (0)
#define SUB_PMIC_BUCK6_CFG_REG_8_b6_offres_sel_START (1)
#define SUB_PMIC_BUCK6_CFG_REG_8_b6_offres_sel_END (1)
#define SUB_PMIC_BUCK6_CFG_REG_8_b6_ocpibias_START (2)
#define SUB_PMIC_BUCK6_CFG_REG_8_b6_ocpibias_END (3)
#define SUB_PMIC_BUCK6_CFG_REG_8_b6_ocp_toff_START (4)
#define SUB_PMIC_BUCK6_CFG_REG_8_b6_ocp_toff_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_ptn_dt_ctrl : 1;
        unsigned char b6_pg_p_sel : 3;
        unsigned char b6_pg_n_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_9_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_9_b6_ptn_dt_ctrl_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_9_b6_ptn_dt_ctrl_END (0)
#define SUB_PMIC_BUCK6_CFG_REG_9_b6_pg_p_sel_START (1)
#define SUB_PMIC_BUCK6_CFG_REG_9_b6_pg_p_sel_END (3)
#define SUB_PMIC_BUCK6_CFG_REG_9_b6_pg_n_sel_START (4)
#define SUB_PMIC_BUCK6_CFG_REG_9_b6_pg_n_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_rampdown_ndmd : 3;
        unsigned char b6_ramp_ton_ctrl : 1;
        unsigned char b6_ramp_prebias : 1;
        unsigned char b6_ptn_dt_sel : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_10_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_10_b6_rampdown_ndmd_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_10_b6_rampdown_ndmd_END (2)
#define SUB_PMIC_BUCK6_CFG_REG_10_b6_ramp_ton_ctrl_START (3)
#define SUB_PMIC_BUCK6_CFG_REG_10_b6_ramp_ton_ctrl_END (3)
#define SUB_PMIC_BUCK6_CFG_REG_10_b6_ramp_prebias_START (4)
#define SUB_PMIC_BUCK6_CFG_REG_10_b6_ramp_prebias_END (4)
#define SUB_PMIC_BUCK6_CFG_REG_10_b6_ptn_dt_sel_START (5)
#define SUB_PMIC_BUCK6_CFG_REG_10_b6_ptn_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_reg_r : 2;
        unsigned char b6_reg_op_c : 1;
        unsigned char b6_reg_ibias : 1;
        unsigned char b6_reg_en : 1;
        unsigned char b6_reg_dr : 3;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_11_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_11_b6_reg_r_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_11_b6_reg_r_END (1)
#define SUB_PMIC_BUCK6_CFG_REG_11_b6_reg_op_c_START (2)
#define SUB_PMIC_BUCK6_CFG_REG_11_b6_reg_op_c_END (2)
#define SUB_PMIC_BUCK6_CFG_REG_11_b6_reg_ibias_START (3)
#define SUB_PMIC_BUCK6_CFG_REG_11_b6_reg_ibias_END (3)
#define SUB_PMIC_BUCK6_CFG_REG_11_b6_reg_en_START (4)
#define SUB_PMIC_BUCK6_CFG_REG_11_b6_reg_en_END (4)
#define SUB_PMIC_BUCK6_CFG_REG_11_b6_reg_dr_START (5)
#define SUB_PMIC_BUCK6_CFG_REG_11_b6_reg_dr_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_reserve0 : 8;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_12_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_12_b6_reserve0_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_12_b6_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_reserve1 : 8;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_13_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_13_b6_reserve1_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_13_b6_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_softime_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK6_CFG_REG_14_UNION;
#endif
#define SUB_PMIC_BUCK6_CFG_REG_14_b6_softime_sel_START (0)
#define SUB_PMIC_BUCK6_CFG_REG_14_b6_softime_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b6_ocp_state : 1;
        unsigned char b6_ccm_state : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK6_RO_REG_15_UNION;
#endif
#define SUB_PMIC_BUCK6_RO_REG_15_b6_ocp_state_START (0)
#define SUB_PMIC_BUCK6_RO_REG_15_b6_ocp_state_END (0)
#define SUB_PMIC_BUCK6_RO_REG_15_b6_ccm_state_START (1)
#define SUB_PMIC_BUCK6_RO_REG_15_b6_ccm_state_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_adj_rlx : 4;
        unsigned char b7_adj_clx : 2;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_0_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_0_b7_adj_rlx_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_0_b7_adj_rlx_END (3)
#define SUB_PMIC_BUCK7_CFG_REG_0_b7_adj_clx_START (4)
#define SUB_PMIC_BUCK7_CFG_REG_0_b7_adj_clx_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_dmd_blanktime_sel : 1;
        unsigned char b7_cot_rlx : 1;
        unsigned char b7_cmp_ibias : 4;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_1_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_1_b7_dmd_blanktime_sel_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_1_b7_dmd_blanktime_sel_END (0)
#define SUB_PMIC_BUCK7_CFG_REG_1_b7_cot_rlx_START (1)
#define SUB_PMIC_BUCK7_CFG_REG_1_b7_cot_rlx_END (1)
#define SUB_PMIC_BUCK7_CFG_REG_1_b7_cmp_ibias_START (2)
#define SUB_PMIC_BUCK7_CFG_REG_1_b7_cmp_ibias_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_dmd_sel_eco : 4;
        unsigned char b7_dmd_sel : 4;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_2_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_2_b7_dmd_sel_eco_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_2_b7_dmd_sel_eco_END (3)
#define SUB_PMIC_BUCK7_CFG_REG_2_b7_dmd_sel_START (4)
#define SUB_PMIC_BUCK7_CFG_REG_2_b7_dmd_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_ea_clp_eco_dis : 1;
        unsigned char b7_dt_sel : 2;
        unsigned char b7_dmd_type_sel : 1;
        unsigned char b7_dmd_ton : 3;
        unsigned char b7_dmd_shield_ton : 1;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_3_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_3_b7_ea_clp_eco_dis_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_3_b7_ea_clp_eco_dis_END (0)
#define SUB_PMIC_BUCK7_CFG_REG_3_b7_dt_sel_START (1)
#define SUB_PMIC_BUCK7_CFG_REG_3_b7_dt_sel_END (2)
#define SUB_PMIC_BUCK7_CFG_REG_3_b7_dmd_type_sel_START (3)
#define SUB_PMIC_BUCK7_CFG_REG_3_b7_dmd_type_sel_END (3)
#define SUB_PMIC_BUCK7_CFG_REG_3_b7_dmd_ton_START (4)
#define SUB_PMIC_BUCK7_CFG_REG_3_b7_dmd_ton_END (6)
#define SUB_PMIC_BUCK7_CFG_REG_3_b7_dmd_shield_ton_START (7)
#define SUB_PMIC_BUCK7_CFG_REG_3_b7_dmd_shield_ton_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_fb_cap_sel : 1;
        unsigned char b7_fastramp_ctrl : 4;
        unsigned char b7_eco_ibias : 1;
        unsigned char b7_ea_ibias : 1;
        unsigned char b7_ea_eco_dis : 1;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_4_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_4_b7_fb_cap_sel_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_4_b7_fb_cap_sel_END (0)
#define SUB_PMIC_BUCK7_CFG_REG_4_b7_fastramp_ctrl_START (1)
#define SUB_PMIC_BUCK7_CFG_REG_4_b7_fastramp_ctrl_END (4)
#define SUB_PMIC_BUCK7_CFG_REG_4_b7_eco_ibias_START (5)
#define SUB_PMIC_BUCK7_CFG_REG_4_b7_eco_ibias_END (5)
#define SUB_PMIC_BUCK7_CFG_REG_4_b7_ea_ibias_START (6)
#define SUB_PMIC_BUCK7_CFG_REG_4_b7_ea_ibias_END (6)
#define SUB_PMIC_BUCK7_CFG_REG_4_b7_ea_eco_dis_START (7)
#define SUB_PMIC_BUCK7_CFG_REG_4_b7_ea_eco_dis_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_min_ton : 3;
        unsigned char b7_lx_dt : 1;
        unsigned char b7_ibias_dmd_ctrl : 2;
        unsigned char b7_fix_ton : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_5_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_5_b7_min_ton_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_5_b7_min_ton_END (2)
#define SUB_PMIC_BUCK7_CFG_REG_5_b7_lx_dt_START (3)
#define SUB_PMIC_BUCK7_CFG_REG_5_b7_lx_dt_END (3)
#define SUB_PMIC_BUCK7_CFG_REG_5_b7_ibias_dmd_ctrl_START (4)
#define SUB_PMIC_BUCK7_CFG_REG_5_b7_ibias_dmd_ctrl_END (5)
#define SUB_PMIC_BUCK7_CFG_REG_5_b7_fix_ton_START (6)
#define SUB_PMIC_BUCK7_CFG_REG_5_b7_fix_ton_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_ntp_dt_ctrl : 1;
        unsigned char b7_nonlinear_driver : 1;
        unsigned char b7_ng_p_sel : 3;
        unsigned char b7_ng_n_sel : 3;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_6_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_6_b7_ntp_dt_ctrl_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_6_b7_ntp_dt_ctrl_END (0)
#define SUB_PMIC_BUCK7_CFG_REG_6_b7_nonlinear_driver_START (1)
#define SUB_PMIC_BUCK7_CFG_REG_6_b7_nonlinear_driver_END (1)
#define SUB_PMIC_BUCK7_CFG_REG_6_b7_ng_p_sel_START (2)
#define SUB_PMIC_BUCK7_CFG_REG_6_b7_ng_p_sel_END (4)
#define SUB_PMIC_BUCK7_CFG_REG_6_b7_ng_n_sel_START (5)
#define SUB_PMIC_BUCK7_CFG_REG_6_b7_ng_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_ocp_sel : 3;
        unsigned char b7_ocp_delay : 1;
        unsigned char b7_ocp_cmp_ibias : 1;
        unsigned char b7_ntp_dt_sel : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_7_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_7_b7_ocp_sel_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_7_b7_ocp_sel_END (2)
#define SUB_PMIC_BUCK7_CFG_REG_7_b7_ocp_delay_START (3)
#define SUB_PMIC_BUCK7_CFG_REG_7_b7_ocp_delay_END (3)
#define SUB_PMIC_BUCK7_CFG_REG_7_b7_ocp_cmp_ibias_START (4)
#define SUB_PMIC_BUCK7_CFG_REG_7_b7_ocp_cmp_ibias_END (4)
#define SUB_PMIC_BUCK7_CFG_REG_7_b7_ntp_dt_sel_START (5)
#define SUB_PMIC_BUCK7_CFG_REG_7_b7_ntp_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_offres_shut : 1;
        unsigned char b7_offres_sel : 1;
        unsigned char b7_ocpibias : 2;
        unsigned char b7_ocp_toff : 2;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_8_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_8_b7_offres_shut_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_8_b7_offres_shut_END (0)
#define SUB_PMIC_BUCK7_CFG_REG_8_b7_offres_sel_START (1)
#define SUB_PMIC_BUCK7_CFG_REG_8_b7_offres_sel_END (1)
#define SUB_PMIC_BUCK7_CFG_REG_8_b7_ocpibias_START (2)
#define SUB_PMIC_BUCK7_CFG_REG_8_b7_ocpibias_END (3)
#define SUB_PMIC_BUCK7_CFG_REG_8_b7_ocp_toff_START (4)
#define SUB_PMIC_BUCK7_CFG_REG_8_b7_ocp_toff_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_ptn_dt_ctrl : 1;
        unsigned char b7_pg_p_sel : 3;
        unsigned char b7_pg_n_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_9_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_9_b7_ptn_dt_ctrl_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_9_b7_ptn_dt_ctrl_END (0)
#define SUB_PMIC_BUCK7_CFG_REG_9_b7_pg_p_sel_START (1)
#define SUB_PMIC_BUCK7_CFG_REG_9_b7_pg_p_sel_END (3)
#define SUB_PMIC_BUCK7_CFG_REG_9_b7_pg_n_sel_START (4)
#define SUB_PMIC_BUCK7_CFG_REG_9_b7_pg_n_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_rampdown_ndmd : 3;
        unsigned char b7_ramp_ton_ctrl : 1;
        unsigned char b7_ramp_prebias : 1;
        unsigned char b7_ptn_dt_sel : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_10_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_10_b7_rampdown_ndmd_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_10_b7_rampdown_ndmd_END (2)
#define SUB_PMIC_BUCK7_CFG_REG_10_b7_ramp_ton_ctrl_START (3)
#define SUB_PMIC_BUCK7_CFG_REG_10_b7_ramp_ton_ctrl_END (3)
#define SUB_PMIC_BUCK7_CFG_REG_10_b7_ramp_prebias_START (4)
#define SUB_PMIC_BUCK7_CFG_REG_10_b7_ramp_prebias_END (4)
#define SUB_PMIC_BUCK7_CFG_REG_10_b7_ptn_dt_sel_START (5)
#define SUB_PMIC_BUCK7_CFG_REG_10_b7_ptn_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_reg_r : 2;
        unsigned char b7_reg_op_c : 1;
        unsigned char b7_reg_ibias : 1;
        unsigned char b7_reg_en : 1;
        unsigned char b7_reg_dr : 3;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_11_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_11_b7_reg_r_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_11_b7_reg_r_END (1)
#define SUB_PMIC_BUCK7_CFG_REG_11_b7_reg_op_c_START (2)
#define SUB_PMIC_BUCK7_CFG_REG_11_b7_reg_op_c_END (2)
#define SUB_PMIC_BUCK7_CFG_REG_11_b7_reg_ibias_START (3)
#define SUB_PMIC_BUCK7_CFG_REG_11_b7_reg_ibias_END (3)
#define SUB_PMIC_BUCK7_CFG_REG_11_b7_reg_en_START (4)
#define SUB_PMIC_BUCK7_CFG_REG_11_b7_reg_en_END (4)
#define SUB_PMIC_BUCK7_CFG_REG_11_b7_reg_dr_START (5)
#define SUB_PMIC_BUCK7_CFG_REG_11_b7_reg_dr_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_reserve0 : 8;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_12_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_12_b7_reserve0_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_12_b7_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_reserve1 : 8;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_13_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_13_b7_reserve1_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_13_b7_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_softime_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK7_CFG_REG_14_UNION;
#endif
#define SUB_PMIC_BUCK7_CFG_REG_14_b7_softime_sel_START (0)
#define SUB_PMIC_BUCK7_CFG_REG_14_b7_softime_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b7_ocp_state : 1;
        unsigned char b7_ccm_state : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK7_RO_REG_15_UNION;
#endif
#define SUB_PMIC_BUCK7_RO_REG_15_b7_ocp_state_START (0)
#define SUB_PMIC_BUCK7_RO_REG_15_b7_ocp_state_END (0)
#define SUB_PMIC_BUCK7_RO_REG_15_b7_ccm_state_START (1)
#define SUB_PMIC_BUCK7_RO_REG_15_b7_ccm_state_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_adj_rlx : 4;
        unsigned char b8_adj_clx : 2;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK8_CFG_REG_0_UNION;
#endif
#define SUB_PMIC_BUCK8_CFG_REG_0_b8_adj_rlx_START (0)
#define SUB_PMIC_BUCK8_CFG_REG_0_b8_adj_rlx_END (3)
#define SUB_PMIC_BUCK8_CFG_REG_0_b8_adj_clx_START (4)
#define SUB_PMIC_BUCK8_CFG_REG_0_b8_adj_clx_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_dmd_blanktime_sel : 1;
        unsigned char b8_cot_rlx : 1;
        unsigned char b8_cmp_ibias : 4;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK8_CFG_REG_1_UNION;
#endif
#define SUB_PMIC_BUCK8_CFG_REG_1_b8_dmd_blanktime_sel_START (0)
#define SUB_PMIC_BUCK8_CFG_REG_1_b8_dmd_blanktime_sel_END (0)
#define SUB_PMIC_BUCK8_CFG_REG_1_b8_cot_rlx_START (1)
#define SUB_PMIC_BUCK8_CFG_REG_1_b8_cot_rlx_END (1)
#define SUB_PMIC_BUCK8_CFG_REG_1_b8_cmp_ibias_START (2)
#define SUB_PMIC_BUCK8_CFG_REG_1_b8_cmp_ibias_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_dmd_sel_eco : 4;
        unsigned char b8_dmd_sel : 4;
    } reg;
} SUB_PMIC_BUCK8_CFG_REG_2_UNION;
#endif
#define SUB_PMIC_BUCK8_CFG_REG_2_b8_dmd_sel_eco_START (0)
#define SUB_PMIC_BUCK8_CFG_REG_2_b8_dmd_sel_eco_END (3)
#define SUB_PMIC_BUCK8_CFG_REG_2_b8_dmd_sel_START (4)
#define SUB_PMIC_BUCK8_CFG_REG_2_b8_dmd_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_ea_clp_eco_dis : 1;
        unsigned char b8_dt_sel : 2;
        unsigned char b8_dmd_type_sel : 1;
        unsigned char b8_dmd_ton : 3;
        unsigned char b8_dmd_shield_ton : 1;
    } reg;
} SUB_PMIC_BUCK8_CFG_REG_3_UNION;
#endif
#define SUB_PMIC_BUCK8_CFG_REG_3_b8_ea_clp_eco_dis_START (0)
#define SUB_PMIC_BUCK8_CFG_REG_3_b8_ea_clp_eco_dis_END (0)
#define SUB_PMIC_BUCK8_CFG_REG_3_b8_dt_sel_START (1)
#define SUB_PMIC_BUCK8_CFG_REG_3_b8_dt_sel_END (2)
#define SUB_PMIC_BUCK8_CFG_REG_3_b8_dmd_type_sel_START (3)
#define SUB_PMIC_BUCK8_CFG_REG_3_b8_dmd_type_sel_END (3)
#define SUB_PMIC_BUCK8_CFG_REG_3_b8_dmd_ton_START (4)
#define SUB_PMIC_BUCK8_CFG_REG_3_b8_dmd_ton_END (6)
#define SUB_PMIC_BUCK8_CFG_REG_3_b8_dmd_shield_ton_START (7)
#define SUB_PMIC_BUCK8_CFG_REG_3_b8_dmd_shield_ton_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_lx_dt : 1;
        unsigned char b8_ibias_dmd_ctrl : 2;
        unsigned char b8_fix_ton : 1;
        unsigned char b8_fb_cap_sel : 1;
        unsigned char b8_eco_ibias : 1;
        unsigned char b8_ea_ibias : 1;
        unsigned char b8_ea_eco_dis : 1;
    } reg;
} SUB_PMIC_BUCK8_CFG_REG_4_UNION;
#endif
#define SUB_PMIC_BUCK8_CFG_REG_4_b8_lx_dt_START (0)
#define SUB_PMIC_BUCK8_CFG_REG_4_b8_lx_dt_END (0)
#define SUB_PMIC_BUCK8_CFG_REG_4_b8_ibias_dmd_ctrl_START (1)
#define SUB_PMIC_BUCK8_CFG_REG_4_b8_ibias_dmd_ctrl_END (2)
#define SUB_PMIC_BUCK8_CFG_REG_4_b8_fix_ton_START (3)
#define SUB_PMIC_BUCK8_CFG_REG_4_b8_fix_ton_END (3)
#define SUB_PMIC_BUCK8_CFG_REG_4_b8_fb_cap_sel_START (4)
#define SUB_PMIC_BUCK8_CFG_REG_4_b8_fb_cap_sel_END (4)
#define SUB_PMIC_BUCK8_CFG_REG_4_b8_eco_ibias_START (5)
#define SUB_PMIC_BUCK8_CFG_REG_4_b8_eco_ibias_END (5)
#define SUB_PMIC_BUCK8_CFG_REG_4_b8_ea_ibias_START (6)
#define SUB_PMIC_BUCK8_CFG_REG_4_b8_ea_ibias_END (6)
#define SUB_PMIC_BUCK8_CFG_REG_4_b8_ea_eco_dis_START (7)
#define SUB_PMIC_BUCK8_CFG_REG_4_b8_ea_eco_dis_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_ng_n_sel : 3;
        unsigned char b8_min_ton : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK8_CFG_REG_5_UNION;
#endif
#define SUB_PMIC_BUCK8_CFG_REG_5_b8_ng_n_sel_START (0)
#define SUB_PMIC_BUCK8_CFG_REG_5_b8_ng_n_sel_END (2)
#define SUB_PMIC_BUCK8_CFG_REG_5_b8_min_ton_START (3)
#define SUB_PMIC_BUCK8_CFG_REG_5_b8_min_ton_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_ocp_cmp_ibias : 1;
        unsigned char b8_ntp_dt_sel : 2;
        unsigned char b8_ntp_dt_ctrl : 1;
        unsigned char b8_nonlinear_driver : 1;
        unsigned char b8_ng_p_sel : 3;
    } reg;
} SUB_PMIC_BUCK8_CFG_REG_6_UNION;
#endif
#define SUB_PMIC_BUCK8_CFG_REG_6_b8_ocp_cmp_ibias_START (0)
#define SUB_PMIC_BUCK8_CFG_REG_6_b8_ocp_cmp_ibias_END (0)
#define SUB_PMIC_BUCK8_CFG_REG_6_b8_ntp_dt_sel_START (1)
#define SUB_PMIC_BUCK8_CFG_REG_6_b8_ntp_dt_sel_END (2)
#define SUB_PMIC_BUCK8_CFG_REG_6_b8_ntp_dt_ctrl_START (3)
#define SUB_PMIC_BUCK8_CFG_REG_6_b8_ntp_dt_ctrl_END (3)
#define SUB_PMIC_BUCK8_CFG_REG_6_b8_nonlinear_driver_START (4)
#define SUB_PMIC_BUCK8_CFG_REG_6_b8_nonlinear_driver_END (4)
#define SUB_PMIC_BUCK8_CFG_REG_6_b8_ng_p_sel_START (5)
#define SUB_PMIC_BUCK8_CFG_REG_6_b8_ng_p_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_ocpibias : 2;
        unsigned char b8_ocp_toff : 2;
        unsigned char b8_ocp_sel : 3;
        unsigned char b8_ocp_delay : 1;
    } reg;
} SUB_PMIC_BUCK8_CFG_REG_7_UNION;
#endif
#define SUB_PMIC_BUCK8_CFG_REG_7_b8_ocpibias_START (0)
#define SUB_PMIC_BUCK8_CFG_REG_7_b8_ocpibias_END (1)
#define SUB_PMIC_BUCK8_CFG_REG_7_b8_ocp_toff_START (2)
#define SUB_PMIC_BUCK8_CFG_REG_7_b8_ocp_toff_END (3)
#define SUB_PMIC_BUCK8_CFG_REG_7_b8_ocp_sel_START (4)
#define SUB_PMIC_BUCK8_CFG_REG_7_b8_ocp_sel_END (6)
#define SUB_PMIC_BUCK8_CFG_REG_7_b8_ocp_delay_START (7)
#define SUB_PMIC_BUCK8_CFG_REG_7_b8_ocp_delay_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_pg_p_sel : 3;
        unsigned char b8_pg_n_sel : 3;
        unsigned char b8_offres_shut : 1;
        unsigned char b8_offres_sel : 1;
    } reg;
} SUB_PMIC_BUCK8_CFG_REG_8_UNION;
#endif
#define SUB_PMIC_BUCK8_CFG_REG_8_b8_pg_p_sel_START (0)
#define SUB_PMIC_BUCK8_CFG_REG_8_b8_pg_p_sel_END (2)
#define SUB_PMIC_BUCK8_CFG_REG_8_b8_pg_n_sel_START (3)
#define SUB_PMIC_BUCK8_CFG_REG_8_b8_pg_n_sel_END (5)
#define SUB_PMIC_BUCK8_CFG_REG_8_b8_offres_shut_START (6)
#define SUB_PMIC_BUCK8_CFG_REG_8_b8_offres_shut_END (6)
#define SUB_PMIC_BUCK8_CFG_REG_8_b8_offres_sel_START (7)
#define SUB_PMIC_BUCK8_CFG_REG_8_b8_offres_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_reg_dr : 3;
        unsigned char b8_ramp_ton_ctrl : 1;
        unsigned char b8_ramp_prebias : 1;
        unsigned char b8_ptn_dt_sel : 2;
        unsigned char b8_ptn_dt_ctrl : 1;
    } reg;
} SUB_PMIC_BUCK8_CFG_REG_9_UNION;
#endif
#define SUB_PMIC_BUCK8_CFG_REG_9_b8_reg_dr_START (0)
#define SUB_PMIC_BUCK8_CFG_REG_9_b8_reg_dr_END (2)
#define SUB_PMIC_BUCK8_CFG_REG_9_b8_ramp_ton_ctrl_START (3)
#define SUB_PMIC_BUCK8_CFG_REG_9_b8_ramp_ton_ctrl_END (3)
#define SUB_PMIC_BUCK8_CFG_REG_9_b8_ramp_prebias_START (4)
#define SUB_PMIC_BUCK8_CFG_REG_9_b8_ramp_prebias_END (4)
#define SUB_PMIC_BUCK8_CFG_REG_9_b8_ptn_dt_sel_START (5)
#define SUB_PMIC_BUCK8_CFG_REG_9_b8_ptn_dt_sel_END (6)
#define SUB_PMIC_BUCK8_CFG_REG_9_b8_ptn_dt_ctrl_START (7)
#define SUB_PMIC_BUCK8_CFG_REG_9_b8_ptn_dt_ctrl_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_reg_r : 2;
        unsigned char b8_reg_op_c : 1;
        unsigned char b8_reg_ibias : 1;
        unsigned char b8_reg_en : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK8_CFG_REG_10_UNION;
#endif
#define SUB_PMIC_BUCK8_CFG_REG_10_b8_reg_r_START (0)
#define SUB_PMIC_BUCK8_CFG_REG_10_b8_reg_r_END (1)
#define SUB_PMIC_BUCK8_CFG_REG_10_b8_reg_op_c_START (2)
#define SUB_PMIC_BUCK8_CFG_REG_10_b8_reg_op_c_END (2)
#define SUB_PMIC_BUCK8_CFG_REG_10_b8_reg_ibias_START (3)
#define SUB_PMIC_BUCK8_CFG_REG_10_b8_reg_ibias_END (3)
#define SUB_PMIC_BUCK8_CFG_REG_10_b8_reg_en_START (4)
#define SUB_PMIC_BUCK8_CFG_REG_10_b8_reg_en_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_reserve0 : 8;
    } reg;
} SUB_PMIC_BUCK8_CFG_REG_11_UNION;
#endif
#define SUB_PMIC_BUCK8_CFG_REG_11_b8_reserve0_START (0)
#define SUB_PMIC_BUCK8_CFG_REG_11_b8_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_reserve1 : 8;
    } reg;
} SUB_PMIC_BUCK8_CFG_REG_12_UNION;
#endif
#define SUB_PMIC_BUCK8_CFG_REG_12_b8_reserve1_START (0)
#define SUB_PMIC_BUCK8_CFG_REG_12_b8_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_softime_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK8_CFG_REG_13_UNION;
#endif
#define SUB_PMIC_BUCK8_CFG_REG_13_b8_softime_sel_START (0)
#define SUB_PMIC_BUCK8_CFG_REG_13_b8_softime_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b8_ocp_state : 1;
        unsigned char b8_ccm_state : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK8_RO_REG_14_UNION;
#endif
#define SUB_PMIC_BUCK8_RO_REG_14_b8_ocp_state_START (0)
#define SUB_PMIC_BUCK8_RO_REG_14_b8_ocp_state_END (0)
#define SUB_PMIC_BUCK8_RO_REG_14_b8_ccm_state_START (1)
#define SUB_PMIC_BUCK8_RO_REG_14_b8_ccm_state_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_adj_rlx : 4;
        unsigned char b13_adj_clx : 2;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK13_CFG_REG_0_UNION;
#endif
#define SUB_PMIC_BUCK13_CFG_REG_0_b13_adj_rlx_START (0)
#define SUB_PMIC_BUCK13_CFG_REG_0_b13_adj_rlx_END (3)
#define SUB_PMIC_BUCK13_CFG_REG_0_b13_adj_clx_START (4)
#define SUB_PMIC_BUCK13_CFG_REG_0_b13_adj_clx_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_dmd_blanktime_sel : 1;
        unsigned char b13_cot_rlx : 1;
        unsigned char b13_cmp_ibias : 4;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK13_CFG_REG_1_UNION;
#endif
#define SUB_PMIC_BUCK13_CFG_REG_1_b13_dmd_blanktime_sel_START (0)
#define SUB_PMIC_BUCK13_CFG_REG_1_b13_dmd_blanktime_sel_END (0)
#define SUB_PMIC_BUCK13_CFG_REG_1_b13_cot_rlx_START (1)
#define SUB_PMIC_BUCK13_CFG_REG_1_b13_cot_rlx_END (1)
#define SUB_PMIC_BUCK13_CFG_REG_1_b13_cmp_ibias_START (2)
#define SUB_PMIC_BUCK13_CFG_REG_1_b13_cmp_ibias_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_dmd_sel_eco : 4;
        unsigned char b13_dmd_sel : 4;
    } reg;
} SUB_PMIC_BUCK13_CFG_REG_2_UNION;
#endif
#define SUB_PMIC_BUCK13_CFG_REG_2_b13_dmd_sel_eco_START (0)
#define SUB_PMIC_BUCK13_CFG_REG_2_b13_dmd_sel_eco_END (3)
#define SUB_PMIC_BUCK13_CFG_REG_2_b13_dmd_sel_START (4)
#define SUB_PMIC_BUCK13_CFG_REG_2_b13_dmd_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_ea_clp_eco_dis : 1;
        unsigned char b13_dt_sel : 2;
        unsigned char b13_dmd_type_sel : 1;
        unsigned char b13_dmd_ton : 3;
        unsigned char b13_dmd_shield_ton : 1;
    } reg;
} SUB_PMIC_BUCK13_CFG_REG_3_UNION;
#endif
#define SUB_PMIC_BUCK13_CFG_REG_3_b13_ea_clp_eco_dis_START (0)
#define SUB_PMIC_BUCK13_CFG_REG_3_b13_ea_clp_eco_dis_END (0)
#define SUB_PMIC_BUCK13_CFG_REG_3_b13_dt_sel_START (1)
#define SUB_PMIC_BUCK13_CFG_REG_3_b13_dt_sel_END (2)
#define SUB_PMIC_BUCK13_CFG_REG_3_b13_dmd_type_sel_START (3)
#define SUB_PMIC_BUCK13_CFG_REG_3_b13_dmd_type_sel_END (3)
#define SUB_PMIC_BUCK13_CFG_REG_3_b13_dmd_ton_START (4)
#define SUB_PMIC_BUCK13_CFG_REG_3_b13_dmd_ton_END (6)
#define SUB_PMIC_BUCK13_CFG_REG_3_b13_dmd_shield_ton_START (7)
#define SUB_PMIC_BUCK13_CFG_REG_3_b13_dmd_shield_ton_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_lx_dt : 1;
        unsigned char b13_ibias_dmd_ctrl : 2;
        unsigned char b13_fix_ton : 1;
        unsigned char b13_fb_cap_sel : 1;
        unsigned char b13_eco_ibias : 1;
        unsigned char b13_ea_ibias : 1;
        unsigned char b13_ea_eco_dis : 1;
    } reg;
} SUB_PMIC_BUCK13_CFG_REG_4_UNION;
#endif
#define SUB_PMIC_BUCK13_CFG_REG_4_b13_lx_dt_START (0)
#define SUB_PMIC_BUCK13_CFG_REG_4_b13_lx_dt_END (0)
#define SUB_PMIC_BUCK13_CFG_REG_4_b13_ibias_dmd_ctrl_START (1)
#define SUB_PMIC_BUCK13_CFG_REG_4_b13_ibias_dmd_ctrl_END (2)
#define SUB_PMIC_BUCK13_CFG_REG_4_b13_fix_ton_START (3)
#define SUB_PMIC_BUCK13_CFG_REG_4_b13_fix_ton_END (3)
#define SUB_PMIC_BUCK13_CFG_REG_4_b13_fb_cap_sel_START (4)
#define SUB_PMIC_BUCK13_CFG_REG_4_b13_fb_cap_sel_END (4)
#define SUB_PMIC_BUCK13_CFG_REG_4_b13_eco_ibias_START (5)
#define SUB_PMIC_BUCK13_CFG_REG_4_b13_eco_ibias_END (5)
#define SUB_PMIC_BUCK13_CFG_REG_4_b13_ea_ibias_START (6)
#define SUB_PMIC_BUCK13_CFG_REG_4_b13_ea_ibias_END (6)
#define SUB_PMIC_BUCK13_CFG_REG_4_b13_ea_eco_dis_START (7)
#define SUB_PMIC_BUCK13_CFG_REG_4_b13_ea_eco_dis_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_ng_n_sel : 3;
        unsigned char b13_min_ton : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK13_CFG_REG_5_UNION;
#endif
#define SUB_PMIC_BUCK13_CFG_REG_5_b13_ng_n_sel_START (0)
#define SUB_PMIC_BUCK13_CFG_REG_5_b13_ng_n_sel_END (2)
#define SUB_PMIC_BUCK13_CFG_REG_5_b13_min_ton_START (3)
#define SUB_PMIC_BUCK13_CFG_REG_5_b13_min_ton_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_ocp_cmp_ibias : 1;
        unsigned char b13_ntp_dt_sel : 2;
        unsigned char b13_ntp_dt_ctrl : 1;
        unsigned char b13_nonlinear_driver : 1;
        unsigned char b13_ng_p_sel : 3;
    } reg;
} SUB_PMIC_BUCK13_CFG_REG_6_UNION;
#endif
#define SUB_PMIC_BUCK13_CFG_REG_6_b13_ocp_cmp_ibias_START (0)
#define SUB_PMIC_BUCK13_CFG_REG_6_b13_ocp_cmp_ibias_END (0)
#define SUB_PMIC_BUCK13_CFG_REG_6_b13_ntp_dt_sel_START (1)
#define SUB_PMIC_BUCK13_CFG_REG_6_b13_ntp_dt_sel_END (2)
#define SUB_PMIC_BUCK13_CFG_REG_6_b13_ntp_dt_ctrl_START (3)
#define SUB_PMIC_BUCK13_CFG_REG_6_b13_ntp_dt_ctrl_END (3)
#define SUB_PMIC_BUCK13_CFG_REG_6_b13_nonlinear_driver_START (4)
#define SUB_PMIC_BUCK13_CFG_REG_6_b13_nonlinear_driver_END (4)
#define SUB_PMIC_BUCK13_CFG_REG_6_b13_ng_p_sel_START (5)
#define SUB_PMIC_BUCK13_CFG_REG_6_b13_ng_p_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_ocpibias : 2;
        unsigned char b13_ocp_toff : 2;
        unsigned char b13_ocp_sel : 3;
        unsigned char b13_ocp_delay : 1;
    } reg;
} SUB_PMIC_BUCK13_CFG_REG_7_UNION;
#endif
#define SUB_PMIC_BUCK13_CFG_REG_7_b13_ocpibias_START (0)
#define SUB_PMIC_BUCK13_CFG_REG_7_b13_ocpibias_END (1)
#define SUB_PMIC_BUCK13_CFG_REG_7_b13_ocp_toff_START (2)
#define SUB_PMIC_BUCK13_CFG_REG_7_b13_ocp_toff_END (3)
#define SUB_PMIC_BUCK13_CFG_REG_7_b13_ocp_sel_START (4)
#define SUB_PMIC_BUCK13_CFG_REG_7_b13_ocp_sel_END (6)
#define SUB_PMIC_BUCK13_CFG_REG_7_b13_ocp_delay_START (7)
#define SUB_PMIC_BUCK13_CFG_REG_7_b13_ocp_delay_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_pg_p_sel : 3;
        unsigned char b13_pg_n_sel : 3;
        unsigned char b13_offres_shut : 1;
        unsigned char b13_offres_sel : 1;
    } reg;
} SUB_PMIC_BUCK13_CFG_REG_8_UNION;
#endif
#define SUB_PMIC_BUCK13_CFG_REG_8_b13_pg_p_sel_START (0)
#define SUB_PMIC_BUCK13_CFG_REG_8_b13_pg_p_sel_END (2)
#define SUB_PMIC_BUCK13_CFG_REG_8_b13_pg_n_sel_START (3)
#define SUB_PMIC_BUCK13_CFG_REG_8_b13_pg_n_sel_END (5)
#define SUB_PMIC_BUCK13_CFG_REG_8_b13_offres_shut_START (6)
#define SUB_PMIC_BUCK13_CFG_REG_8_b13_offres_shut_END (6)
#define SUB_PMIC_BUCK13_CFG_REG_8_b13_offres_sel_START (7)
#define SUB_PMIC_BUCK13_CFG_REG_8_b13_offres_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_reg_dr : 3;
        unsigned char b13_ramp_ton_ctrl : 1;
        unsigned char b13_ramp_prebias : 1;
        unsigned char b13_ptn_dt_sel : 2;
        unsigned char b13_ptn_dt_ctrl : 1;
    } reg;
} SUB_PMIC_BUCK13_CFG_REG_9_UNION;
#endif
#define SUB_PMIC_BUCK13_CFG_REG_9_b13_reg_dr_START (0)
#define SUB_PMIC_BUCK13_CFG_REG_9_b13_reg_dr_END (2)
#define SUB_PMIC_BUCK13_CFG_REG_9_b13_ramp_ton_ctrl_START (3)
#define SUB_PMIC_BUCK13_CFG_REG_9_b13_ramp_ton_ctrl_END (3)
#define SUB_PMIC_BUCK13_CFG_REG_9_b13_ramp_prebias_START (4)
#define SUB_PMIC_BUCK13_CFG_REG_9_b13_ramp_prebias_END (4)
#define SUB_PMIC_BUCK13_CFG_REG_9_b13_ptn_dt_sel_START (5)
#define SUB_PMIC_BUCK13_CFG_REG_9_b13_ptn_dt_sel_END (6)
#define SUB_PMIC_BUCK13_CFG_REG_9_b13_ptn_dt_ctrl_START (7)
#define SUB_PMIC_BUCK13_CFG_REG_9_b13_ptn_dt_ctrl_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_reg_r : 2;
        unsigned char b13_reg_op_c : 1;
        unsigned char b13_reg_ibias : 1;
        unsigned char b13_reg_en : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK13_CFG_REG_10_UNION;
#endif
#define SUB_PMIC_BUCK13_CFG_REG_10_b13_reg_r_START (0)
#define SUB_PMIC_BUCK13_CFG_REG_10_b13_reg_r_END (1)
#define SUB_PMIC_BUCK13_CFG_REG_10_b13_reg_op_c_START (2)
#define SUB_PMIC_BUCK13_CFG_REG_10_b13_reg_op_c_END (2)
#define SUB_PMIC_BUCK13_CFG_REG_10_b13_reg_ibias_START (3)
#define SUB_PMIC_BUCK13_CFG_REG_10_b13_reg_ibias_END (3)
#define SUB_PMIC_BUCK13_CFG_REG_10_b13_reg_en_START (4)
#define SUB_PMIC_BUCK13_CFG_REG_10_b13_reg_en_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_reserve0 : 8;
    } reg;
} SUB_PMIC_BUCK13_CFG_REG_11_UNION;
#endif
#define SUB_PMIC_BUCK13_CFG_REG_11_b13_reserve0_START (0)
#define SUB_PMIC_BUCK13_CFG_REG_11_b13_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_reserve1 : 8;
    } reg;
} SUB_PMIC_BUCK13_CFG_REG_12_UNION;
#endif
#define SUB_PMIC_BUCK13_CFG_REG_12_b13_reserve1_START (0)
#define SUB_PMIC_BUCK13_CFG_REG_12_b13_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_softime_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK13_CFG_REG_13_UNION;
#endif
#define SUB_PMIC_BUCK13_CFG_REG_13_b13_softime_sel_START (0)
#define SUB_PMIC_BUCK13_CFG_REG_13_b13_softime_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b13_ocp_state : 1;
        unsigned char b13_ccm_state : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK13_RO_REG_14_UNION;
#endif
#define SUB_PMIC_BUCK13_RO_REG_14_b13_ocp_state_START (0)
#define SUB_PMIC_BUCK13_RO_REG_14_b13_ocp_state_END (0)
#define SUB_PMIC_BUCK13_RO_REG_14_b13_ccm_state_START (1)
#define SUB_PMIC_BUCK13_RO_REG_14_b13_ccm_state_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char osc_9m6_sel : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_OSC_9M6_CFG_UNION;
#endif
#define SUB_PMIC_OSC_9M6_CFG_osc_9m6_sel_START (0)
#define SUB_PMIC_OSC_9M6_CFG_osc_9m6_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b_hys_ocp_dis : 1;
        unsigned char b_hys_lx_trim : 1;
        unsigned char b_hys_filter_ton : 2;
        unsigned char b_hys_dmd_clp : 1;
        unsigned char b_hys_cmp_filter : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK_HYS_TOP_CFG_REG_0_UNION;
#endif
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_0_b_hys_ocp_dis_START (0)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_0_b_hys_ocp_dis_END (0)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_0_b_hys_lx_trim_START (1)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_0_b_hys_lx_trim_END (1)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_0_b_hys_filter_ton_START (2)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_0_b_hys_filter_ton_END (3)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_0_b_hys_dmd_clp_START (4)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_0_b_hys_dmd_clp_END (4)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_0_b_hys_cmp_filter_START (5)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_0_b_hys_cmp_filter_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b_hys_regop_clp : 1;
        unsigned char b_hys_reg_ss : 1;
        unsigned char b_hys_ocptrim_ctl : 3;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK_HYS_TOP_CFG_REG_1_UNION;
#endif
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_1_b_hys_regop_clp_START (0)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_1_b_hys_regop_clp_END (0)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_1_b_hys_reg_ss_START (1)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_1_b_hys_reg_ss_END (1)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_1_b_hys_ocptrim_ctl_START (2)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_1_b_hys_ocptrim_ctl_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b_hys_reserve0 : 8;
    } reg;
} SUB_PMIC_BUCK_HYS_TOP_CFG_REG_2_UNION;
#endif
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_2_b_hys_reserve0_START (0)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_2_b_hys_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b_hys_reserve1 : 8;
    } reg;
} SUB_PMIC_BUCK_HYS_TOP_CFG_REG_3_UNION;
#endif
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_3_b_hys_reserve1_START (0)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_3_b_hys_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b_hys_reserve2 : 8;
    } reg;
} SUB_PMIC_BUCK_HYS_TOP_CFG_REG_4_UNION;
#endif
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_4_b_hys_reserve2_START (0)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_4_b_hys_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b_hys_ton_trim : 1;
        unsigned char b_hys_short_pdp : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK_HYS_TOP_CFG_REG_5_UNION;
#endif
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_5_b_hys_ton_trim_START (0)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_5_b_hys_ton_trim_END (0)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_5_b_hys_short_pdp_START (1)
#define SUB_PMIC_BUCK_HYS_TOP_CFG_REG_5_b_hys_short_pdp_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim0_en : 1;
        unsigned char sim0_data_simrp : 1;
        unsigned char sim0_data_bbrp : 1;
        unsigned char sim0_200ns_en : 1;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SIM_CFG_0_UNION;
#endif
#define SUB_PMIC_SIM_CFG_0_sim0_en_START (0)
#define SUB_PMIC_SIM_CFG_0_sim0_en_END (0)
#define SUB_PMIC_SIM_CFG_0_sim0_data_simrp_START (1)
#define SUB_PMIC_SIM_CFG_0_sim0_data_simrp_END (1)
#define SUB_PMIC_SIM_CFG_0_sim0_data_bbrp_START (2)
#define SUB_PMIC_SIM_CFG_0_sim0_data_bbrp_END (2)
#define SUB_PMIC_SIM_CFG_0_sim0_200ns_en_START (3)
#define SUB_PMIC_SIM_CFG_0_sim0_200ns_en_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim0_reserve : 8;
    } reg;
} SUB_PMIC_SIM_CFG_1_UNION;
#endif
#define SUB_PMIC_SIM_CFG_1_sim0_reserve_START (0)
#define SUB_PMIC_SIM_CFG_1_sim0_reserve_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim1_en : 1;
        unsigned char sim1_data_simrp : 1;
        unsigned char sim1_data_bbrp : 1;
        unsigned char sim1_200ns_en : 1;
        unsigned char sim0_rst_simrp : 1;
        unsigned char sim0_rst_bbrp : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_SIM_CFG_2_UNION;
#endif
#define SUB_PMIC_SIM_CFG_2_sim1_en_START (0)
#define SUB_PMIC_SIM_CFG_2_sim1_en_END (0)
#define SUB_PMIC_SIM_CFG_2_sim1_data_simrp_START (1)
#define SUB_PMIC_SIM_CFG_2_sim1_data_simrp_END (1)
#define SUB_PMIC_SIM_CFG_2_sim1_data_bbrp_START (2)
#define SUB_PMIC_SIM_CFG_2_sim1_data_bbrp_END (2)
#define SUB_PMIC_SIM_CFG_2_sim1_200ns_en_START (3)
#define SUB_PMIC_SIM_CFG_2_sim1_200ns_en_END (3)
#define SUB_PMIC_SIM_CFG_2_sim0_rst_simrp_START (4)
#define SUB_PMIC_SIM_CFG_2_sim0_rst_simrp_END (4)
#define SUB_PMIC_SIM_CFG_2_sim0_rst_bbrp_START (5)
#define SUB_PMIC_SIM_CFG_2_sim0_rst_bbrp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim1_reserve : 8;
    } reg;
} SUB_PMIC_SIM_CFG_3_UNION;
#endif
#define SUB_PMIC_SIM_CFG_3_sim1_reserve_START (0)
#define SUB_PMIC_SIM_CFG_3_sim1_reserve_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim1_rst_simrp : 1;
        unsigned char sim1_rst_bbrp : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_SIM_CFG_4_UNION;
#endif
#define SUB_PMIC_SIM_CFG_4_sim1_rst_simrp_START (0)
#define SUB_PMIC_SIM_CFG_4_sim1_rst_simrp_END (0)
#define SUB_PMIC_SIM_CFG_4_sim1_rst_bbrp_START (1)
#define SUB_PMIC_SIM_CFG_4_sim1_rst_bbrp_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo9_vrset : 2;
        unsigned char ldo9_vo_sel : 1;
        unsigned char ldo9_ocp_enn : 1;
        unsigned char ldo9_eco_config : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_LDO9_CFG_0_UNION;
#endif
#define SUB_PMIC_LDO9_CFG_0_ldo9_vrset_START (0)
#define SUB_PMIC_LDO9_CFG_0_ldo9_vrset_END (1)
#define SUB_PMIC_LDO9_CFG_0_ldo9_vo_sel_START (2)
#define SUB_PMIC_LDO9_CFG_0_ldo9_vo_sel_END (2)
#define SUB_PMIC_LDO9_CFG_0_ldo9_ocp_enn_START (3)
#define SUB_PMIC_LDO9_CFG_0_ldo9_ocp_enn_END (3)
#define SUB_PMIC_LDO9_CFG_0_ldo9_eco_config_START (4)
#define SUB_PMIC_LDO9_CFG_0_ldo9_eco_config_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo34_vrset : 3;
        unsigned char ldo34_ocp_set : 2;
        unsigned char ldo34_ocp_enn : 1;
        unsigned char ldo34_eco_set_enn : 1;
        unsigned char sw_set_enn : 1;
    } reg;
} SUB_PMIC_LDO34_CFG_0_UNION;
#endif
#define SUB_PMIC_LDO34_CFG_0_ldo34_vrset_START (0)
#define SUB_PMIC_LDO34_CFG_0_ldo34_vrset_END (2)
#define SUB_PMIC_LDO34_CFG_0_ldo34_ocp_set_START (3)
#define SUB_PMIC_LDO34_CFG_0_ldo34_ocp_set_END (4)
#define SUB_PMIC_LDO34_CFG_0_ldo34_ocp_enn_START (5)
#define SUB_PMIC_LDO34_CFG_0_ldo34_ocp_enn_END (5)
#define SUB_PMIC_LDO34_CFG_0_ldo34_eco_set_enn_START (6)
#define SUB_PMIC_LDO34_CFG_0_ldo34_eco_set_enn_END (6)
#define SUB_PMIC_LDO34_CFG_0_sw_set_enn_START (7)
#define SUB_PMIC_LDO34_CFG_0_sw_set_enn_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo50_vrset : 3;
        unsigned char ldo50_ocp_set : 2;
        unsigned char ldo50_ocp_enn : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_LDO50_CFG_0_UNION;
#endif
#define SUB_PMIC_LDO50_CFG_0_ldo50_vrset_START (0)
#define SUB_PMIC_LDO50_CFG_0_ldo50_vrset_END (2)
#define SUB_PMIC_LDO50_CFG_0_ldo50_ocp_set_START (3)
#define SUB_PMIC_LDO50_CFG_0_ldo50_ocp_set_END (4)
#define SUB_PMIC_LDO50_CFG_0_ldo50_ocp_enn_START (5)
#define SUB_PMIC_LDO50_CFG_0_ldo50_ocp_enn_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo51_vrset : 3;
        unsigned char ldo51_ocp_set : 2;
        unsigned char ldo51_ocp_enn : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_LDO51_CFG_0_UNION;
#endif
#define SUB_PMIC_LDO51_CFG_0_ldo51_vrset_START (0)
#define SUB_PMIC_LDO51_CFG_0_ldo51_vrset_END (2)
#define SUB_PMIC_LDO51_CFG_0_ldo51_ocp_set_START (3)
#define SUB_PMIC_LDO51_CFG_0_ldo51_ocp_set_END (4)
#define SUB_PMIC_LDO51_CFG_0_ldo51_ocp_enn_START (5)
#define SUB_PMIC_LDO51_CFG_0_ldo51_ocp_enn_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo53_vrset : 2;
        unsigned char ldo53_ocp_enn : 1;
        unsigned char ldo53_eco_config : 1;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_LDO53_CFG_0_UNION;
#endif
#define SUB_PMIC_LDO53_CFG_0_ldo53_vrset_START (0)
#define SUB_PMIC_LDO53_CFG_0_ldo53_vrset_END (1)
#define SUB_PMIC_LDO53_CFG_0_ldo53_ocp_enn_START (2)
#define SUB_PMIC_LDO53_CFG_0_ldo53_ocp_enn_END (2)
#define SUB_PMIC_LDO53_CFG_0_ldo53_eco_config_START (3)
#define SUB_PMIC_LDO53_CFG_0_ldo53_eco_config_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo54_vrset : 2;
        unsigned char ldo54_ocp_enn : 1;
        unsigned char ldo54_eco_config : 1;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_LDO54_CFG_0_UNION;
#endif
#define SUB_PMIC_LDO54_CFG_0_ldo54_vrset_START (0)
#define SUB_PMIC_LDO54_CFG_0_ldo54_vrset_END (1)
#define SUB_PMIC_LDO54_CFG_0_ldo54_ocp_enn_START (2)
#define SUB_PMIC_LDO54_CFG_0_ldo54_ocp_enn_END (2)
#define SUB_PMIC_LDO54_CFG_0_ldo54_eco_config_START (3)
#define SUB_PMIC_LDO54_CFG_0_ldo54_eco_config_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pmuh_vrset : 3;
        unsigned char pmuh_stb_set : 4;
        unsigned char pmuh_ocp_enn : 1;
    } reg;
} SUB_PMIC_PMUH_CFG_0_UNION;
#endif
#define SUB_PMIC_PMUH_CFG_0_pmuh_vrset_START (0)
#define SUB_PMIC_PMUH_CFG_0_pmuh_vrset_END (2)
#define SUB_PMIC_PMUH_CFG_0_pmuh_stb_set_START (3)
#define SUB_PMIC_PMUH_CFG_0_pmuh_stb_set_END (6)
#define SUB_PMIC_PMUH_CFG_0_pmuh_ocp_enn_START (7)
#define SUB_PMIC_PMUH_CFG_0_pmuh_ocp_enn_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve0 : 8;
    } reg;
} SUB_PMIC_LDO_RESERVE_CFG_0_UNION;
#endif
#define SUB_PMIC_LDO_RESERVE_CFG_0_ldo_reserve0_START (0)
#define SUB_PMIC_LDO_RESERVE_CFG_0_ldo_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve1 : 8;
    } reg;
} SUB_PMIC_LDO_RESERVE_CFG_1_UNION;
#endif
#define SUB_PMIC_LDO_RESERVE_CFG_1_ldo_reserve1_START (0)
#define SUB_PMIC_LDO_RESERVE_CFG_1_ldo_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ref_ibias_trim_en : 2;
        unsigned char ref_bg_test : 1;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_THSD_CFG_0_UNION;
#endif
#define SUB_PMIC_THSD_CFG_0_ref_ibias_trim_en_START (0)
#define SUB_PMIC_THSD_CFG_0_ref_ibias_trim_en_END (1)
#define SUB_PMIC_THSD_CFG_0_ref_bg_test_START (2)
#define SUB_PMIC_THSD_CFG_0_ref_bg_test_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lpddr_sel_a2d : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_SYS_CTRL_RO_REG_0_UNION;
#endif
#define SUB_PMIC_SYS_CTRL_RO_REG_0_lpddr_sel_a2d_START (0)
#define SUB_PMIC_SYS_CTRL_RO_REG_0_lpddr_sel_a2d_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char d2a_reserve0 : 8;
    } reg;
} SUB_PMIC_D2A_RESERVE_CFG_0_UNION;
#endif
#define SUB_PMIC_D2A_RESERVE_CFG_0_d2a_reserve0_START (0)
#define SUB_PMIC_D2A_RESERVE_CFG_0_d2a_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char d2a_reserve1 : 8;
    } reg;
} SUB_PMIC_D2A_RESERVE_CFG_1_UNION;
#endif
#define SUB_PMIC_D2A_RESERVE_CFG_1_d2a_reserve1_START (0)
#define SUB_PMIC_D2A_RESERVE_CFG_1_d2a_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char a2d_reserve0 : 8;
    } reg;
} SUB_PMIC_A2D_RESERVE0_UNION;
#endif
#define SUB_PMIC_A2D_RESERVE0_a2d_reserve0_START (0)
#define SUB_PMIC_A2D_RESERVE0_a2d_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ana_test0 : 8;
    } reg;
} SUB_PMIC_ANA_TEST0_UNION;
#endif
#define SUB_PMIC_ANA_TEST0_ana_test0_START (0)
#define SUB_PMIC_ANA_TEST0_ana_test0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ana_test1 : 8;
    } reg;
} SUB_PMIC_ANA_TEST1_UNION;
#endif
#define SUB_PMIC_ANA_TEST1_ana_test1_START (0)
#define SUB_PMIC_ANA_TEST1_ana_test1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char tsensor_en : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_TSENSOR_EN_UNION;
#endif
#define SUB_PMIC_TSENSOR_EN_tsensor_en_START (0)
#define SUB_PMIC_TSENSOR_EN_tsensor_en_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char tsensor_code_a2d : 8;
    } reg;
} SUB_PMIC_TSENSOR_CODE_UNION;
#endif
#define SUB_PMIC_TSENSOR_CODE_tsensor_code_a2d_START (0)
#define SUB_PMIC_TSENSOR_CODE_tsensor_code_a2d_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_onoff_place_sel : 8;
    } reg;
} SUB_PMIC_ONOFF_PLACE_SEL_UNION;
#endif
#define SUB_PMIC_ONOFF_PLACE_SEL_np_onoff_place_sel_START (0)
#define SUB_PMIC_ONOFF_PLACE_SEL_np_onoff_place_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck13_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK13_ON_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK13_ON_PLACE_np_buck13_on_place_START (0)
#define SUB_PMIC_BUCK13_ON_PLACE_np_buck13_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck00_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK00_ON_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK00_ON_PLACE_np_buck00_on_place_START (0)
#define SUB_PMIC_BUCK00_ON_PLACE_np_buck00_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck6_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK6_ON_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK6_ON_PLACE_np_buck6_on_place_START (0)
#define SUB_PMIC_BUCK6_ON_PLACE_np_buck6_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck8_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK8_ON_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK8_ON_PLACE_np_buck8_on_place_START (0)
#define SUB_PMIC_BUCK8_ON_PLACE_np_buck8_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck7_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK7_ON_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK7_ON_PLACE_np_buck7_on_place_START (0)
#define SUB_PMIC_BUCK7_ON_PLACE_np_buck7_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck12_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK12_ON_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK12_ON_PLACE_np_buck12_on_place_START (0)
#define SUB_PMIC_BUCK12_ON_PLACE_np_buck12_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck10_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK10_ON_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK10_ON_PLACE_np_buck10_on_place_START (0)
#define SUB_PMIC_BUCK10_ON_PLACE_np_buck10_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck20_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK20_ON_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK20_ON_PLACE_np_buck20_on_place_START (0)
#define SUB_PMIC_BUCK20_ON_PLACE_np_buck20_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck30_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK30_ON_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK30_ON_PLACE_np_buck30_on_place_START (0)
#define SUB_PMIC_BUCK30_ON_PLACE_np_buck30_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo9_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_LDO9_ON_PLACE_UNION;
#endif
#define SUB_PMIC_LDO9_ON_PLACE_np_ldo9_on_place_START (0)
#define SUB_PMIC_LDO9_ON_PLACE_np_ldo9_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck13_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK13_OFF_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK13_OFF_PLACE_np_buck13_off_place_START (0)
#define SUB_PMIC_BUCK13_OFF_PLACE_np_buck13_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck00_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK00_OFF_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK00_OFF_PLACE_np_buck00_off_place_START (0)
#define SUB_PMIC_BUCK00_OFF_PLACE_np_buck00_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck6_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK6_OFF_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK6_OFF_PLACE_np_buck6_off_place_START (0)
#define SUB_PMIC_BUCK6_OFF_PLACE_np_buck6_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck8_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK8_OFF_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK8_OFF_PLACE_np_buck8_off_place_START (0)
#define SUB_PMIC_BUCK8_OFF_PLACE_np_buck8_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck7_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK7_OFF_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK7_OFF_PLACE_np_buck7_off_place_START (0)
#define SUB_PMIC_BUCK7_OFF_PLACE_np_buck7_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck12_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK12_OFF_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK12_OFF_PLACE_np_buck12_off_place_START (0)
#define SUB_PMIC_BUCK12_OFF_PLACE_np_buck12_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck10_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK10_OFF_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK10_OFF_PLACE_np_buck10_off_place_START (0)
#define SUB_PMIC_BUCK10_OFF_PLACE_np_buck10_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck20_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK20_OFF_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK20_OFF_PLACE_np_buck20_off_place_START (0)
#define SUB_PMIC_BUCK20_OFF_PLACE_np_buck20_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck30_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK30_OFF_PLACE_UNION;
#endif
#define SUB_PMIC_BUCK30_OFF_PLACE_np_buck30_off_place_START (0)
#define SUB_PMIC_BUCK30_OFF_PLACE_np_buck30_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo9_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_LDO9_OFF_PLACE_UNION;
#endif
#define SUB_PMIC_LDO9_OFF_PLACE_np_ldo9_off_place_START (0)
#define SUB_PMIC_LDO9_OFF_PLACE_np_ldo9_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_debug_lock : 8;
    } reg;
} SUB_PMIC_DEBUG_LOCK_UNION;
#endif
#define SUB_PMIC_DEBUG_LOCK_np_debug_lock_START (0)
#define SUB_PMIC_DEBUG_LOCK_np_debug_lock_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sys_debug0 : 8;
    } reg;
} SUB_PMIC_SYS_DEBUG0_UNION;
#endif
#define SUB_PMIC_SYS_DEBUG0_np_sys_debug0_START (0)
#define SUB_PMIC_SYS_DEBUG0_np_sys_debug0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sys_debug1 : 8;
    } reg;
} SUB_PMIC_SYS_DEBUG1_UNION;
#endif
#define SUB_PMIC_SYS_DEBUG1_np_sys_debug1_START (0)
#define SUB_PMIC_SYS_DEBUG1_np_sys_debug1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sys_debug2 : 8;
    } reg;
} SUB_PMIC_SYS_DEBUG2_UNION;
#endif
#define SUB_PMIC_SYS_DEBUG2_np_sys_debug2_START (0)
#define SUB_PMIC_SYS_DEBUG2_np_sys_debug2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sys_debug3 : 8;
    } reg;
} SUB_PMIC_SYS_DEBUG3_UNION;
#endif
#define SUB_PMIC_SYS_DEBUG3_np_sys_debug3_START (0)
#define SUB_PMIC_SYS_DEBUG3_np_sys_debug3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_vsys_pwroff_deb_set : 2;
        unsigned char np_vsys_pwroff_abs_set : 2;
        unsigned char np_vsys_pwron_set : 2;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_VSYS_LOW_SET_UNION;
#endif
#define SUB_PMIC_VSYS_LOW_SET_np_vsys_pwroff_deb_set_START (0)
#define SUB_PMIC_VSYS_LOW_SET_np_vsys_pwroff_deb_set_END (1)
#define SUB_PMIC_VSYS_LOW_SET_np_vsys_pwroff_abs_set_START (2)
#define SUB_PMIC_VSYS_LOW_SET_np_vsys_pwroff_abs_set_END (3)
#define SUB_PMIC_VSYS_LOW_SET_np_vsys_pwron_set_START (4)
#define SUB_PMIC_VSYS_LOW_SET_np_vsys_pwron_set_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_vsys_pwroff_deb_mask : 1;
        unsigned char np_vsys_pwroff_abs_mask : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_NP_VSYS_PWROFF_MASK_UNION;
#endif
#define SUB_PMIC_NP_VSYS_PWROFF_MASK_np_vsys_pwroff_deb_mask_START (0)
#define SUB_PMIC_NP_VSYS_PWROFF_MASK_np_vsys_pwroff_deb_mask_END (0)
#define SUB_PMIC_NP_VSYS_PWROFF_MASK_np_vsys_pwroff_abs_mask_START (1)
#define SUB_PMIC_NP_VSYS_PWROFF_MASK_np_vsys_pwroff_abs_mask_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sys_ctrl_reserve : 8;
    } reg;
} SUB_PMIC_NP_SYS_RESERVE_UNION;
#endif
#define SUB_PMIC_NP_SYS_RESERVE_np_sys_ctrl_reserve_START (0)
#define SUB_PMIC_NP_SYS_RESERVE_np_sys_ctrl_reserve_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_d2a_reserve0 : 8;
    } reg;
} SUB_PMIC_NP_DA_RESERVE0_UNION;
#endif
#define SUB_PMIC_NP_DA_RESERVE0_np_d2a_reserve0_START (0)
#define SUB_PMIC_NP_DA_RESERVE0_np_d2a_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_d2a_reserve1 : 8;
    } reg;
} SUB_PMIC_NP_DA_RESERVE1_UNION;
#endif
#define SUB_PMIC_NP_DA_RESERVE1_np_d2a_reserve1_START (0)
#define SUB_PMIC_NP_DA_RESERVE1_np_d2a_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_b10_pd_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_NP_BUCK10_CFG_0_UNION;
#endif
#define SUB_PMIC_NP_BUCK10_CFG_0_np_b10_pd_sel_START (0)
#define SUB_PMIC_NP_BUCK10_CFG_0_np_b10_pd_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_b20_pd_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_NP_BUCK20_CFG_0_UNION;
#endif
#define SUB_PMIC_NP_BUCK20_CFG_0_np_b20_pd_sel_START (0)
#define SUB_PMIC_NP_BUCK20_CFG_0_np_b20_pd_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_b30_pd_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_NP_BUCK30_CFG_0_UNION;
#endif
#define SUB_PMIC_NP_BUCK30_CFG_0_np_b30_pd_sel_START (0)
#define SUB_PMIC_NP_BUCK30_CFG_0_np_b30_pd_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_b12_pd_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_NP_BUCK12_CFG_0_UNION;
#endif
#define SUB_PMIC_NP_BUCK12_CFG_0_np_b12_pd_sel_START (0)
#define SUB_PMIC_NP_BUCK12_CFG_0_np_b12_pd_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_vset_cfg : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_NP_BUCKBOOST_VSET_UNION;
#endif
#define SUB_PMIC_NP_BUCKBOOST_VSET_np_bb_vset_cfg_START (0)
#define SUB_PMIC_NP_BUCKBOOST_VSET_np_bb_vset_cfg_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_b_dt_p2n : 2;
        unsigned char reserved_0 : 2;
        unsigned char np_bb_b_dt_n2p : 3;
        unsigned char reserved_1 : 1;
    } reg;
} SUB_PMIC_NP_BUCKBOOST_CFG_0_UNION;
#endif
#define SUB_PMIC_NP_BUCKBOOST_CFG_0_np_bb_b_dt_p2n_START (0)
#define SUB_PMIC_NP_BUCKBOOST_CFG_0_np_bb_b_dt_p2n_END (1)
#define SUB_PMIC_NP_BUCKBOOST_CFG_0_np_bb_b_dt_n2p_START (4)
#define SUB_PMIC_NP_BUCKBOOST_CFG_0_np_bb_b_dt_n2p_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_bst_dt_n2p : 3;
        unsigned char reserved_0 : 1;
        unsigned char np_bb_bst_dt_p2n : 1;
        unsigned char reserved_1 : 3;
    } reg;
} SUB_PMIC_NP_BUCKBOOST_CFG_1_UNION;
#endif
#define SUB_PMIC_NP_BUCKBOOST_CFG_1_np_bb_bst_dt_n2p_START (0)
#define SUB_PMIC_NP_BUCKBOOST_CFG_1_np_bb_bst_dt_n2p_END (2)
#define SUB_PMIC_NP_BUCKBOOST_CFG_1_np_bb_bst_dt_p2n_START (4)
#define SUB_PMIC_NP_BUCKBOOST_CFG_1_np_bb_bst_dt_p2n_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_b_ng_n : 2;
        unsigned char np_bb_b_ng_p : 2;
        unsigned char np_bb_b_pg_n : 2;
        unsigned char np_bb_b_pg_p : 2;
    } reg;
} SUB_PMIC_NP_BUCKBOOST_CFG_2_UNION;
#endif
#define SUB_PMIC_NP_BUCKBOOST_CFG_2_np_bb_b_ng_n_START (0)
#define SUB_PMIC_NP_BUCKBOOST_CFG_2_np_bb_b_ng_n_END (1)
#define SUB_PMIC_NP_BUCKBOOST_CFG_2_np_bb_b_ng_p_START (2)
#define SUB_PMIC_NP_BUCKBOOST_CFG_2_np_bb_b_ng_p_END (3)
#define SUB_PMIC_NP_BUCKBOOST_CFG_2_np_bb_b_pg_n_START (4)
#define SUB_PMIC_NP_BUCKBOOST_CFG_2_np_bb_b_pg_n_END (5)
#define SUB_PMIC_NP_BUCKBOOST_CFG_2_np_bb_b_pg_p_START (6)
#define SUB_PMIC_NP_BUCKBOOST_CFG_2_np_bb_b_pg_p_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_bst_ng_n : 2;
        unsigned char np_bb_bst_ng_p : 2;
        unsigned char np_bb_bst_pg_n : 2;
        unsigned char np_bb_bst_pg_p : 2;
    } reg;
} SUB_PMIC_NP_BUCKBOOST_CFG_3_UNION;
#endif
#define SUB_PMIC_NP_BUCKBOOST_CFG_3_np_bb_bst_ng_n_START (0)
#define SUB_PMIC_NP_BUCKBOOST_CFG_3_np_bb_bst_ng_n_END (1)
#define SUB_PMIC_NP_BUCKBOOST_CFG_3_np_bb_bst_ng_p_START (2)
#define SUB_PMIC_NP_BUCKBOOST_CFG_3_np_bb_bst_ng_p_END (3)
#define SUB_PMIC_NP_BUCKBOOST_CFG_3_np_bb_bst_pg_n_START (4)
#define SUB_PMIC_NP_BUCKBOOST_CFG_3_np_bb_bst_pg_n_END (5)
#define SUB_PMIC_NP_BUCKBOOST_CFG_3_np_bb_bst_pg_p_START (6)
#define SUB_PMIC_NP_BUCKBOOST_CFG_3_np_bb_bst_pg_p_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve0 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE0_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE0_np_bb_reserve0_START (0)
#define SUB_PMIC_NP_BB_RESERVE0_np_bb_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve1 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE1_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE1_np_bb_reserve1_START (0)
#define SUB_PMIC_NP_BB_RESERVE1_np_bb_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve2 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE2_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE2_np_bb_reserve2_START (0)
#define SUB_PMIC_NP_BB_RESERVE2_np_bb_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve3 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE3_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE3_np_bb_reserve3_START (0)
#define SUB_PMIC_NP_BB_RESERVE3_np_bb_reserve3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve4 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE4_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE4_np_bb_reserve4_START (0)
#define SUB_PMIC_NP_BB_RESERVE4_np_bb_reserve4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve5 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE5_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE5_np_bb_reserve5_START (0)
#define SUB_PMIC_NP_BB_RESERVE5_np_bb_reserve5_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve6 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE6_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE6_np_bb_reserve6_START (0)
#define SUB_PMIC_NP_BB_RESERVE6_np_bb_reserve6_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve7 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE7_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE7_np_bb_reserve7_START (0)
#define SUB_PMIC_NP_BB_RESERVE7_np_bb_reserve7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve8 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE8_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE8_np_bb_reserve8_START (0)
#define SUB_PMIC_NP_BB_RESERVE8_np_bb_reserve8_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve9 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE9_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE9_np_bb_reserve9_START (0)
#define SUB_PMIC_NP_BB_RESERVE9_np_bb_reserve9_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve10 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE10_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE10_np_bb_reserve10_START (0)
#define SUB_PMIC_NP_BB_RESERVE10_np_bb_reserve10_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve11 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE11_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE11_np_bb_reserve11_START (0)
#define SUB_PMIC_NP_BB_RESERVE11_np_bb_reserve11_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve12 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE12_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE12_np_bb_reserve12_START (0)
#define SUB_PMIC_NP_BB_RESERVE12_np_bb_reserve12_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve13 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE13_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE13_np_bb_reserve13_START (0)
#define SUB_PMIC_NP_BB_RESERVE13_np_bb_reserve13_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve14 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE14_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE14_np_bb_reserve14_START (0)
#define SUB_PMIC_NP_BB_RESERVE14_np_bb_reserve14_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve15 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE15_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE15_np_bb_reserve15_START (0)
#define SUB_PMIC_NP_BB_RESERVE15_np_bb_reserve15_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve16 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE16_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE16_np_bb_reserve16_START (0)
#define SUB_PMIC_NP_BB_RESERVE16_np_bb_reserve16_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve17 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE17_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE17_np_bb_reserve17_START (0)
#define SUB_PMIC_NP_BB_RESERVE17_np_bb_reserve17_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve18 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE18_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE18_np_bb_reserve18_START (0)
#define SUB_PMIC_NP_BB_RESERVE18_np_bb_reserve18_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve19 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE19_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE19_np_bb_reserve19_START (0)
#define SUB_PMIC_NP_BB_RESERVE19_np_bb_reserve19_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve20 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE20_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE20_np_bb_reserve20_START (0)
#define SUB_PMIC_NP_BB_RESERVE20_np_bb_reserve20_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve21 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE21_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE21_np_bb_reserve21_START (0)
#define SUB_PMIC_NP_BB_RESERVE21_np_bb_reserve21_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve22 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE22_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE22_np_bb_reserve22_START (0)
#define SUB_PMIC_NP_BB_RESERVE22_np_bb_reserve22_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve23 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE23_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE23_np_bb_reserve23_START (0)
#define SUB_PMIC_NP_BB_RESERVE23_np_bb_reserve23_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve24 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE24_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE24_np_bb_reserve24_START (0)
#define SUB_PMIC_NP_BB_RESERVE24_np_bb_reserve24_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_bb_reserve25 : 8;
    } reg;
} SUB_PMIC_NP_BB_RESERVE25_UNION;
#endif
#define SUB_PMIC_NP_BB_RESERVE25_np_bb_reserve25_START (0)
#define SUB_PMIC_NP_BB_RESERVE25_np_bb_reserve25_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_thsd_tmp_set : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_NP_REF_CFG0_UNION;
#endif
#define SUB_PMIC_NP_REF_CFG0_np_thsd_tmp_set_START (0)
#define SUB_PMIC_NP_REF_CFG0_np_thsd_tmp_set_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ref_buffer_sel : 6;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BG_THSD_CFG1_UNION;
#endif
#define SUB_PMIC_BG_THSD_CFG1_np_ref_buffer_sel_START (0)
#define SUB_PMIC_BG_THSD_CFG1_np_ref_buffer_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ref_chop_sel : 1;
        unsigned char np_ref_ibias_sel : 1;
        unsigned char np_ref_eco_shield : 1;
        unsigned char reserved_0 : 1;
        unsigned char np_ref_chop_clk_sel : 2;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BG_THSD_CFG2_UNION;
#endif
#define SUB_PMIC_BG_THSD_CFG2_np_ref_chop_sel_START (0)
#define SUB_PMIC_BG_THSD_CFG2_np_ref_chop_sel_END (0)
#define SUB_PMIC_BG_THSD_CFG2_np_ref_ibias_sel_START (1)
#define SUB_PMIC_BG_THSD_CFG2_np_ref_ibias_sel_END (1)
#define SUB_PMIC_BG_THSD_CFG2_np_ref_eco_shield_START (2)
#define SUB_PMIC_BG_THSD_CFG2_np_ref_eco_shield_END (2)
#define SUB_PMIC_BG_THSD_CFG2_np_ref_chop_clk_sel_START (4)
#define SUB_PMIC_BG_THSD_CFG2_np_ref_chop_clk_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ref_reserve0 : 8;
    } reg;
} SUB_PMIC_NP_REF_RESERVE0_UNION;
#endif
#define SUB_PMIC_NP_REF_RESERVE0_np_ref_reserve0_START (0)
#define SUB_PMIC_NP_REF_RESERVE0_np_ref_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ref_reserve1 : 8;
    } reg;
} SUB_PMIC_NP_REF_RESERVE1_UNION;
#endif
#define SUB_PMIC_NP_REF_RESERVE1_np_ref_reserve1_START (0)
#define SUB_PMIC_NP_REF_RESERVE1_np_ref_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_spmi_data_ds : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_SPMI_DATA_DS_CFG_UNION;
#endif
#define SUB_PMIC_SPMI_DATA_DS_CFG_np_spmi_data_ds_START (0)
#define SUB_PMIC_SPMI_DATA_DS_CFG_np_spmi_data_ds_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_pmu1_irq_ds : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_IRQ_DS_CFG_UNION;
#endif
#define SUB_PMIC_IRQ_DS_CFG_np_pmu1_irq_ds_START (0)
#define SUB_PMIC_IRQ_DS_CFG_np_pmu1_irq_ds_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob0_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_0_R_UNION;
#endif
#define SUB_PMIC_OTP0_0_R_np_otp0_pdob0_d2a_START (0)
#define SUB_PMIC_OTP0_0_R_np_otp0_pdob0_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob1_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_1_R_UNION;
#endif
#define SUB_PMIC_OTP0_1_R_np_otp0_pdob1_d2a_START (0)
#define SUB_PMIC_OTP0_1_R_np_otp0_pdob1_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob2_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_2_R_UNION;
#endif
#define SUB_PMIC_OTP0_2_R_np_otp0_pdob2_d2a_START (0)
#define SUB_PMIC_OTP0_2_R_np_otp0_pdob2_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob3_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_3_R_UNION;
#endif
#define SUB_PMIC_OTP0_3_R_np_otp0_pdob3_d2a_START (0)
#define SUB_PMIC_OTP0_3_R_np_otp0_pdob3_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob4_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_4_R_UNION;
#endif
#define SUB_PMIC_OTP0_4_R_np_otp0_pdob4_d2a_START (0)
#define SUB_PMIC_OTP0_4_R_np_otp0_pdob4_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob5_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_5_R_UNION;
#endif
#define SUB_PMIC_OTP0_5_R_np_otp0_pdob5_d2a_START (0)
#define SUB_PMIC_OTP0_5_R_np_otp0_pdob5_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob6_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_6_R_UNION;
#endif
#define SUB_PMIC_OTP0_6_R_np_otp0_pdob6_d2a_START (0)
#define SUB_PMIC_OTP0_6_R_np_otp0_pdob6_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob7_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_7_R_UNION;
#endif
#define SUB_PMIC_OTP0_7_R_np_otp0_pdob7_d2a_START (0)
#define SUB_PMIC_OTP0_7_R_np_otp0_pdob7_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob8_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_8_R_UNION;
#endif
#define SUB_PMIC_OTP0_8_R_np_otp0_pdob8_d2a_START (0)
#define SUB_PMIC_OTP0_8_R_np_otp0_pdob8_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob9_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_9_R_UNION;
#endif
#define SUB_PMIC_OTP0_9_R_np_otp0_pdob9_d2a_START (0)
#define SUB_PMIC_OTP0_9_R_np_otp0_pdob9_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob10_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_10_R_UNION;
#endif
#define SUB_PMIC_OTP0_10_R_np_otp0_pdob10_d2a_START (0)
#define SUB_PMIC_OTP0_10_R_np_otp0_pdob10_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob11_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_11_R_UNION;
#endif
#define SUB_PMIC_OTP0_11_R_np_otp0_pdob11_d2a_START (0)
#define SUB_PMIC_OTP0_11_R_np_otp0_pdob11_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob12_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_12_R_UNION;
#endif
#define SUB_PMIC_OTP0_12_R_np_otp0_pdob12_d2a_START (0)
#define SUB_PMIC_OTP0_12_R_np_otp0_pdob12_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob13_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_13_R_UNION;
#endif
#define SUB_PMIC_OTP0_13_R_np_otp0_pdob13_d2a_START (0)
#define SUB_PMIC_OTP0_13_R_np_otp0_pdob13_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob14_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_14_R_UNION;
#endif
#define SUB_PMIC_OTP0_14_R_np_otp0_pdob14_d2a_START (0)
#define SUB_PMIC_OTP0_14_R_np_otp0_pdob14_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob15_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_15_R_UNION;
#endif
#define SUB_PMIC_OTP0_15_R_np_otp0_pdob15_d2a_START (0)
#define SUB_PMIC_OTP0_15_R_np_otp0_pdob15_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob16_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_16_R_UNION;
#endif
#define SUB_PMIC_OTP0_16_R_np_otp0_pdob16_d2a_START (0)
#define SUB_PMIC_OTP0_16_R_np_otp0_pdob16_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob17_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_17_R_UNION;
#endif
#define SUB_PMIC_OTP0_17_R_np_otp0_pdob17_d2a_START (0)
#define SUB_PMIC_OTP0_17_R_np_otp0_pdob17_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob18_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_18_R_UNION;
#endif
#define SUB_PMIC_OTP0_18_R_np_otp0_pdob18_d2a_START (0)
#define SUB_PMIC_OTP0_18_R_np_otp0_pdob18_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob19_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_19_R_UNION;
#endif
#define SUB_PMIC_OTP0_19_R_np_otp0_pdob19_d2a_START (0)
#define SUB_PMIC_OTP0_19_R_np_otp0_pdob19_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob20_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_20_R_UNION;
#endif
#define SUB_PMIC_OTP0_20_R_np_otp0_pdob20_d2a_START (0)
#define SUB_PMIC_OTP0_20_R_np_otp0_pdob20_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob21_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_21_R_UNION;
#endif
#define SUB_PMIC_OTP0_21_R_np_otp0_pdob21_d2a_START (0)
#define SUB_PMIC_OTP0_21_R_np_otp0_pdob21_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob22_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_22_R_UNION;
#endif
#define SUB_PMIC_OTP0_22_R_np_otp0_pdob22_d2a_START (0)
#define SUB_PMIC_OTP0_22_R_np_otp0_pdob22_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob23_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_23_R_UNION;
#endif
#define SUB_PMIC_OTP0_23_R_np_otp0_pdob23_d2a_START (0)
#define SUB_PMIC_OTP0_23_R_np_otp0_pdob23_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob24_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_24_R_UNION;
#endif
#define SUB_PMIC_OTP0_24_R_np_otp0_pdob24_d2a_START (0)
#define SUB_PMIC_OTP0_24_R_np_otp0_pdob24_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob25_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_25_R_UNION;
#endif
#define SUB_PMIC_OTP0_25_R_np_otp0_pdob25_d2a_START (0)
#define SUB_PMIC_OTP0_25_R_np_otp0_pdob25_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob26_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_26_R_UNION;
#endif
#define SUB_PMIC_OTP0_26_R_np_otp0_pdob26_d2a_START (0)
#define SUB_PMIC_OTP0_26_R_np_otp0_pdob26_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob27_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_27_R_UNION;
#endif
#define SUB_PMIC_OTP0_27_R_np_otp0_pdob27_d2a_START (0)
#define SUB_PMIC_OTP0_27_R_np_otp0_pdob27_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob28_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_28_R_UNION;
#endif
#define SUB_PMIC_OTP0_28_R_np_otp0_pdob28_d2a_START (0)
#define SUB_PMIC_OTP0_28_R_np_otp0_pdob28_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob29_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_29_R_UNION;
#endif
#define SUB_PMIC_OTP0_29_R_np_otp0_pdob29_d2a_START (0)
#define SUB_PMIC_OTP0_29_R_np_otp0_pdob29_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob30_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_30_R_UNION;
#endif
#define SUB_PMIC_OTP0_30_R_np_otp0_pdob30_d2a_START (0)
#define SUB_PMIC_OTP0_30_R_np_otp0_pdob30_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob31_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_31_R_UNION;
#endif
#define SUB_PMIC_OTP0_31_R_np_otp0_pdob31_d2a_START (0)
#define SUB_PMIC_OTP0_31_R_np_otp0_pdob31_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob32_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_32_R_UNION;
#endif
#define SUB_PMIC_OTP0_32_R_np_otp0_pdob32_d2a_START (0)
#define SUB_PMIC_OTP0_32_R_np_otp0_pdob32_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob33_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_33_R_UNION;
#endif
#define SUB_PMIC_OTP0_33_R_np_otp0_pdob33_d2a_START (0)
#define SUB_PMIC_OTP0_33_R_np_otp0_pdob33_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob34_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_34_R_UNION;
#endif
#define SUB_PMIC_OTP0_34_R_np_otp0_pdob34_d2a_START (0)
#define SUB_PMIC_OTP0_34_R_np_otp0_pdob34_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob35_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_35_R_UNION;
#endif
#define SUB_PMIC_OTP0_35_R_np_otp0_pdob35_d2a_START (0)
#define SUB_PMIC_OTP0_35_R_np_otp0_pdob35_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob36_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_36_R_UNION;
#endif
#define SUB_PMIC_OTP0_36_R_np_otp0_pdob36_d2a_START (0)
#define SUB_PMIC_OTP0_36_R_np_otp0_pdob36_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob37_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_37_R_UNION;
#endif
#define SUB_PMIC_OTP0_37_R_np_otp0_pdob37_d2a_START (0)
#define SUB_PMIC_OTP0_37_R_np_otp0_pdob37_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob38_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_38_R_UNION;
#endif
#define SUB_PMIC_OTP0_38_R_np_otp0_pdob38_d2a_START (0)
#define SUB_PMIC_OTP0_38_R_np_otp0_pdob38_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob39_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_39_R_UNION;
#endif
#define SUB_PMIC_OTP0_39_R_np_otp0_pdob39_d2a_START (0)
#define SUB_PMIC_OTP0_39_R_np_otp0_pdob39_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob40_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_40_R_UNION;
#endif
#define SUB_PMIC_OTP0_40_R_np_otp0_pdob40_d2a_START (0)
#define SUB_PMIC_OTP0_40_R_np_otp0_pdob40_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob41_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_41_R_UNION;
#endif
#define SUB_PMIC_OTP0_41_R_np_otp0_pdob41_d2a_START (0)
#define SUB_PMIC_OTP0_41_R_np_otp0_pdob41_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob42_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_42_R_UNION;
#endif
#define SUB_PMIC_OTP0_42_R_np_otp0_pdob42_d2a_START (0)
#define SUB_PMIC_OTP0_42_R_np_otp0_pdob42_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob43_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_43_R_UNION;
#endif
#define SUB_PMIC_OTP0_43_R_np_otp0_pdob43_d2a_START (0)
#define SUB_PMIC_OTP0_43_R_np_otp0_pdob43_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob44_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_44_R_UNION;
#endif
#define SUB_PMIC_OTP0_44_R_np_otp0_pdob44_d2a_START (0)
#define SUB_PMIC_OTP0_44_R_np_otp0_pdob44_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob45_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_45_R_UNION;
#endif
#define SUB_PMIC_OTP0_45_R_np_otp0_pdob45_d2a_START (0)
#define SUB_PMIC_OTP0_45_R_np_otp0_pdob45_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob46_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_46_R_UNION;
#endif
#define SUB_PMIC_OTP0_46_R_np_otp0_pdob46_d2a_START (0)
#define SUB_PMIC_OTP0_46_R_np_otp0_pdob46_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob47_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_47_R_UNION;
#endif
#define SUB_PMIC_OTP0_47_R_np_otp0_pdob47_d2a_START (0)
#define SUB_PMIC_OTP0_47_R_np_otp0_pdob47_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob48_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_48_R_UNION;
#endif
#define SUB_PMIC_OTP0_48_R_np_otp0_pdob48_d2a_START (0)
#define SUB_PMIC_OTP0_48_R_np_otp0_pdob48_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob49_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_49_R_UNION;
#endif
#define SUB_PMIC_OTP0_49_R_np_otp0_pdob49_d2a_START (0)
#define SUB_PMIC_OTP0_49_R_np_otp0_pdob49_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob50_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_50_R_UNION;
#endif
#define SUB_PMIC_OTP0_50_R_np_otp0_pdob50_d2a_START (0)
#define SUB_PMIC_OTP0_50_R_np_otp0_pdob50_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob51_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_51_R_UNION;
#endif
#define SUB_PMIC_OTP0_51_R_np_otp0_pdob51_d2a_START (0)
#define SUB_PMIC_OTP0_51_R_np_otp0_pdob51_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob52_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_52_R_UNION;
#endif
#define SUB_PMIC_OTP0_52_R_np_otp0_pdob52_d2a_START (0)
#define SUB_PMIC_OTP0_52_R_np_otp0_pdob52_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob53_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_53_R_UNION;
#endif
#define SUB_PMIC_OTP0_53_R_np_otp0_pdob53_d2a_START (0)
#define SUB_PMIC_OTP0_53_R_np_otp0_pdob53_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob54_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_54_R_UNION;
#endif
#define SUB_PMIC_OTP0_54_R_np_otp0_pdob54_d2a_START (0)
#define SUB_PMIC_OTP0_54_R_np_otp0_pdob54_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob55_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_55_R_UNION;
#endif
#define SUB_PMIC_OTP0_55_R_np_otp0_pdob55_d2a_START (0)
#define SUB_PMIC_OTP0_55_R_np_otp0_pdob55_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob56_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_56_R_UNION;
#endif
#define SUB_PMIC_OTP0_56_R_np_otp0_pdob56_d2a_START (0)
#define SUB_PMIC_OTP0_56_R_np_otp0_pdob56_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob57_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_57_R_UNION;
#endif
#define SUB_PMIC_OTP0_57_R_np_otp0_pdob57_d2a_START (0)
#define SUB_PMIC_OTP0_57_R_np_otp0_pdob57_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob58_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_58_R_UNION;
#endif
#define SUB_PMIC_OTP0_58_R_np_otp0_pdob58_d2a_START (0)
#define SUB_PMIC_OTP0_58_R_np_otp0_pdob58_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob59_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_59_R_UNION;
#endif
#define SUB_PMIC_OTP0_59_R_np_otp0_pdob59_d2a_START (0)
#define SUB_PMIC_OTP0_59_R_np_otp0_pdob59_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob60_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_60_R_UNION;
#endif
#define SUB_PMIC_OTP0_60_R_np_otp0_pdob60_d2a_START (0)
#define SUB_PMIC_OTP0_60_R_np_otp0_pdob60_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob61_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_61_R_UNION;
#endif
#define SUB_PMIC_OTP0_61_R_np_otp0_pdob61_d2a_START (0)
#define SUB_PMIC_OTP0_61_R_np_otp0_pdob61_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob62_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_62_R_UNION;
#endif
#define SUB_PMIC_OTP0_62_R_np_otp0_pdob62_d2a_START (0)
#define SUB_PMIC_OTP0_62_R_np_otp0_pdob62_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob63_d2a : 8;
    } reg;
} SUB_PMIC_OTP0_63_R_UNION;
#endif
#define SUB_PMIC_OTP0_63_R_np_otp0_pdob63_d2a_START (0)
#define SUB_PMIC_OTP0_63_R_np_otp0_pdob63_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob0 : 8;
    } reg;
} SUB_PMIC_OTP1_0_R_UNION;
#endif
#define SUB_PMIC_OTP1_0_R_np_otp1_pdob0_START (0)
#define SUB_PMIC_OTP1_0_R_np_otp1_pdob0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob1 : 8;
    } reg;
} SUB_PMIC_OTP1_1_R_UNION;
#endif
#define SUB_PMIC_OTP1_1_R_np_otp1_pdob1_START (0)
#define SUB_PMIC_OTP1_1_R_np_otp1_pdob1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob2 : 8;
    } reg;
} SUB_PMIC_OTP1_2_R_UNION;
#endif
#define SUB_PMIC_OTP1_2_R_np_otp1_pdob2_START (0)
#define SUB_PMIC_OTP1_2_R_np_otp1_pdob2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob3 : 8;
    } reg;
} SUB_PMIC_OTP1_3_R_UNION;
#endif
#define SUB_PMIC_OTP1_3_R_np_otp1_pdob3_START (0)
#define SUB_PMIC_OTP1_3_R_np_otp1_pdob3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob4 : 8;
    } reg;
} SUB_PMIC_OTP1_4_R_UNION;
#endif
#define SUB_PMIC_OTP1_4_R_np_otp1_pdob4_START (0)
#define SUB_PMIC_OTP1_4_R_np_otp1_pdob4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob5 : 8;
    } reg;
} SUB_PMIC_OTP1_5_R_UNION;
#endif
#define SUB_PMIC_OTP1_5_R_np_otp1_pdob5_START (0)
#define SUB_PMIC_OTP1_5_R_np_otp1_pdob5_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob6 : 8;
    } reg;
} SUB_PMIC_OTP1_6_R_UNION;
#endif
#define SUB_PMIC_OTP1_6_R_np_otp1_pdob6_START (0)
#define SUB_PMIC_OTP1_6_R_np_otp1_pdob6_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob7 : 8;
    } reg;
} SUB_PMIC_OTP1_7_R_UNION;
#endif
#define SUB_PMIC_OTP1_7_R_np_otp1_pdob7_START (0)
#define SUB_PMIC_OTP1_7_R_np_otp1_pdob7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob8 : 8;
    } reg;
} SUB_PMIC_OTP1_8_R_UNION;
#endif
#define SUB_PMIC_OTP1_8_R_np_otp1_pdob8_START (0)
#define SUB_PMIC_OTP1_8_R_np_otp1_pdob8_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob9 : 8;
    } reg;
} SUB_PMIC_OTP1_9_R_UNION;
#endif
#define SUB_PMIC_OTP1_9_R_np_otp1_pdob9_START (0)
#define SUB_PMIC_OTP1_9_R_np_otp1_pdob9_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob10 : 8;
    } reg;
} SUB_PMIC_OTP1_10_R_UNION;
#endif
#define SUB_PMIC_OTP1_10_R_np_otp1_pdob10_START (0)
#define SUB_PMIC_OTP1_10_R_np_otp1_pdob10_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob11 : 8;
    } reg;
} SUB_PMIC_OTP1_11_R_UNION;
#endif
#define SUB_PMIC_OTP1_11_R_np_otp1_pdob11_START (0)
#define SUB_PMIC_OTP1_11_R_np_otp1_pdob11_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob12 : 8;
    } reg;
} SUB_PMIC_OTP1_12_R_UNION;
#endif
#define SUB_PMIC_OTP1_12_R_np_otp1_pdob12_START (0)
#define SUB_PMIC_OTP1_12_R_np_otp1_pdob12_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob13 : 8;
    } reg;
} SUB_PMIC_OTP1_13_R_UNION;
#endif
#define SUB_PMIC_OTP1_13_R_np_otp1_pdob13_START (0)
#define SUB_PMIC_OTP1_13_R_np_otp1_pdob13_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob14 : 8;
    } reg;
} SUB_PMIC_OTP1_14_R_UNION;
#endif
#define SUB_PMIC_OTP1_14_R_np_otp1_pdob14_START (0)
#define SUB_PMIC_OTP1_14_R_np_otp1_pdob14_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob15 : 8;
    } reg;
} SUB_PMIC_OTP1_15_R_UNION;
#endif
#define SUB_PMIC_OTP1_15_R_np_otp1_pdob15_START (0)
#define SUB_PMIC_OTP1_15_R_np_otp1_pdob15_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob16 : 8;
    } reg;
} SUB_PMIC_OTP1_16_R_UNION;
#endif
#define SUB_PMIC_OTP1_16_R_np_otp1_pdob16_START (0)
#define SUB_PMIC_OTP1_16_R_np_otp1_pdob16_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob17 : 8;
    } reg;
} SUB_PMIC_OTP1_17_R_UNION;
#endif
#define SUB_PMIC_OTP1_17_R_np_otp1_pdob17_START (0)
#define SUB_PMIC_OTP1_17_R_np_otp1_pdob17_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob18 : 8;
    } reg;
} SUB_PMIC_OTP1_18_R_UNION;
#endif
#define SUB_PMIC_OTP1_18_R_np_otp1_pdob18_START (0)
#define SUB_PMIC_OTP1_18_R_np_otp1_pdob18_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob19 : 8;
    } reg;
} SUB_PMIC_OTP1_19_R_UNION;
#endif
#define SUB_PMIC_OTP1_19_R_np_otp1_pdob19_START (0)
#define SUB_PMIC_OTP1_19_R_np_otp1_pdob19_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob20 : 8;
    } reg;
} SUB_PMIC_OTP1_20_R_UNION;
#endif
#define SUB_PMIC_OTP1_20_R_np_otp1_pdob20_START (0)
#define SUB_PMIC_OTP1_20_R_np_otp1_pdob20_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob21 : 8;
    } reg;
} SUB_PMIC_OTP1_21_R_UNION;
#endif
#define SUB_PMIC_OTP1_21_R_np_otp1_pdob21_START (0)
#define SUB_PMIC_OTP1_21_R_np_otp1_pdob21_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob22 : 8;
    } reg;
} SUB_PMIC_OTP1_22_R_UNION;
#endif
#define SUB_PMIC_OTP1_22_R_np_otp1_pdob22_START (0)
#define SUB_PMIC_OTP1_22_R_np_otp1_pdob22_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob23 : 8;
    } reg;
} SUB_PMIC_OTP1_23_R_UNION;
#endif
#define SUB_PMIC_OTP1_23_R_np_otp1_pdob23_START (0)
#define SUB_PMIC_OTP1_23_R_np_otp1_pdob23_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob24 : 8;
    } reg;
} SUB_PMIC_OTP1_24_R_UNION;
#endif
#define SUB_PMIC_OTP1_24_R_np_otp1_pdob24_START (0)
#define SUB_PMIC_OTP1_24_R_np_otp1_pdob24_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob25 : 8;
    } reg;
} SUB_PMIC_OTP1_25_R_UNION;
#endif
#define SUB_PMIC_OTP1_25_R_np_otp1_pdob25_START (0)
#define SUB_PMIC_OTP1_25_R_np_otp1_pdob25_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob26 : 8;
    } reg;
} SUB_PMIC_OTP1_26_R_UNION;
#endif
#define SUB_PMIC_OTP1_26_R_np_otp1_pdob26_START (0)
#define SUB_PMIC_OTP1_26_R_np_otp1_pdob26_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob27 : 8;
    } reg;
} SUB_PMIC_OTP1_27_R_UNION;
#endif
#define SUB_PMIC_OTP1_27_R_np_otp1_pdob27_START (0)
#define SUB_PMIC_OTP1_27_R_np_otp1_pdob27_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob28 : 8;
    } reg;
} SUB_PMIC_OTP1_28_R_UNION;
#endif
#define SUB_PMIC_OTP1_28_R_np_otp1_pdob28_START (0)
#define SUB_PMIC_OTP1_28_R_np_otp1_pdob28_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob29 : 8;
    } reg;
} SUB_PMIC_OTP1_29_R_UNION;
#endif
#define SUB_PMIC_OTP1_29_R_np_otp1_pdob29_START (0)
#define SUB_PMIC_OTP1_29_R_np_otp1_pdob29_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob30 : 8;
    } reg;
} SUB_PMIC_OTP1_30_R_UNION;
#endif
#define SUB_PMIC_OTP1_30_R_np_otp1_pdob30_START (0)
#define SUB_PMIC_OTP1_30_R_np_otp1_pdob30_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob31 : 8;
    } reg;
} SUB_PMIC_OTP1_31_R_UNION;
#endif
#define SUB_PMIC_OTP1_31_R_np_otp1_pdob31_START (0)
#define SUB_PMIC_OTP1_31_R_np_otp1_pdob31_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob32 : 8;
    } reg;
} SUB_PMIC_OTP1_32_R_UNION;
#endif
#define SUB_PMIC_OTP1_32_R_np_otp1_pdob32_START (0)
#define SUB_PMIC_OTP1_32_R_np_otp1_pdob32_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob33 : 8;
    } reg;
} SUB_PMIC_OTP1_33_R_UNION;
#endif
#define SUB_PMIC_OTP1_33_R_np_otp1_pdob33_START (0)
#define SUB_PMIC_OTP1_33_R_np_otp1_pdob33_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob34 : 8;
    } reg;
} SUB_PMIC_OTP1_34_R_UNION;
#endif
#define SUB_PMIC_OTP1_34_R_np_otp1_pdob34_START (0)
#define SUB_PMIC_OTP1_34_R_np_otp1_pdob34_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob35 : 8;
    } reg;
} SUB_PMIC_OTP1_35_R_UNION;
#endif
#define SUB_PMIC_OTP1_35_R_np_otp1_pdob35_START (0)
#define SUB_PMIC_OTP1_35_R_np_otp1_pdob35_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob36 : 8;
    } reg;
} SUB_PMIC_OTP1_36_R_UNION;
#endif
#define SUB_PMIC_OTP1_36_R_np_otp1_pdob36_START (0)
#define SUB_PMIC_OTP1_36_R_np_otp1_pdob36_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob37 : 8;
    } reg;
} SUB_PMIC_OTP1_37_R_UNION;
#endif
#define SUB_PMIC_OTP1_37_R_np_otp1_pdob37_START (0)
#define SUB_PMIC_OTP1_37_R_np_otp1_pdob37_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob38 : 8;
    } reg;
} SUB_PMIC_OTP1_38_R_UNION;
#endif
#define SUB_PMIC_OTP1_38_R_np_otp1_pdob38_START (0)
#define SUB_PMIC_OTP1_38_R_np_otp1_pdob38_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob39 : 8;
    } reg;
} SUB_PMIC_OTP1_39_R_UNION;
#endif
#define SUB_PMIC_OTP1_39_R_np_otp1_pdob39_START (0)
#define SUB_PMIC_OTP1_39_R_np_otp1_pdob39_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob40 : 8;
    } reg;
} SUB_PMIC_OTP1_40_R_UNION;
#endif
#define SUB_PMIC_OTP1_40_R_np_otp1_pdob40_START (0)
#define SUB_PMIC_OTP1_40_R_np_otp1_pdob40_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob41 : 8;
    } reg;
} SUB_PMIC_OTP1_41_R_UNION;
#endif
#define SUB_PMIC_OTP1_41_R_np_otp1_pdob41_START (0)
#define SUB_PMIC_OTP1_41_R_np_otp1_pdob41_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob42 : 8;
    } reg;
} SUB_PMIC_OTP1_42_R_UNION;
#endif
#define SUB_PMIC_OTP1_42_R_np_otp1_pdob42_START (0)
#define SUB_PMIC_OTP1_42_R_np_otp1_pdob42_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob43 : 8;
    } reg;
} SUB_PMIC_OTP1_43_R_UNION;
#endif
#define SUB_PMIC_OTP1_43_R_np_otp1_pdob43_START (0)
#define SUB_PMIC_OTP1_43_R_np_otp1_pdob43_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob44 : 8;
    } reg;
} SUB_PMIC_OTP1_44_R_UNION;
#endif
#define SUB_PMIC_OTP1_44_R_np_otp1_pdob44_START (0)
#define SUB_PMIC_OTP1_44_R_np_otp1_pdob44_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob45 : 8;
    } reg;
} SUB_PMIC_OTP1_45_R_UNION;
#endif
#define SUB_PMIC_OTP1_45_R_np_otp1_pdob45_START (0)
#define SUB_PMIC_OTP1_45_R_np_otp1_pdob45_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob46 : 8;
    } reg;
} SUB_PMIC_OTP1_46_R_UNION;
#endif
#define SUB_PMIC_OTP1_46_R_np_otp1_pdob46_START (0)
#define SUB_PMIC_OTP1_46_R_np_otp1_pdob46_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob47 : 8;
    } reg;
} SUB_PMIC_OTP1_47_R_UNION;
#endif
#define SUB_PMIC_OTP1_47_R_np_otp1_pdob47_START (0)
#define SUB_PMIC_OTP1_47_R_np_otp1_pdob47_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob48 : 8;
    } reg;
} SUB_PMIC_OTP1_48_R_UNION;
#endif
#define SUB_PMIC_OTP1_48_R_np_otp1_pdob48_START (0)
#define SUB_PMIC_OTP1_48_R_np_otp1_pdob48_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob49 : 8;
    } reg;
} SUB_PMIC_OTP1_49_R_UNION;
#endif
#define SUB_PMIC_OTP1_49_R_np_otp1_pdob49_START (0)
#define SUB_PMIC_OTP1_49_R_np_otp1_pdob49_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob50 : 8;
    } reg;
} SUB_PMIC_OTP1_50_R_UNION;
#endif
#define SUB_PMIC_OTP1_50_R_np_otp1_pdob50_START (0)
#define SUB_PMIC_OTP1_50_R_np_otp1_pdob50_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob51 : 8;
    } reg;
} SUB_PMIC_OTP1_51_R_UNION;
#endif
#define SUB_PMIC_OTP1_51_R_np_otp1_pdob51_START (0)
#define SUB_PMIC_OTP1_51_R_np_otp1_pdob51_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob52 : 8;
    } reg;
} SUB_PMIC_OTP1_52_R_UNION;
#endif
#define SUB_PMIC_OTP1_52_R_np_otp1_pdob52_START (0)
#define SUB_PMIC_OTP1_52_R_np_otp1_pdob52_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob53 : 8;
    } reg;
} SUB_PMIC_OTP1_53_R_UNION;
#endif
#define SUB_PMIC_OTP1_53_R_np_otp1_pdob53_START (0)
#define SUB_PMIC_OTP1_53_R_np_otp1_pdob53_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob54 : 8;
    } reg;
} SUB_PMIC_OTP1_54_R_UNION;
#endif
#define SUB_PMIC_OTP1_54_R_np_otp1_pdob54_START (0)
#define SUB_PMIC_OTP1_54_R_np_otp1_pdob54_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob55 : 8;
    } reg;
} SUB_PMIC_OTP1_55_R_UNION;
#endif
#define SUB_PMIC_OTP1_55_R_np_otp1_pdob55_START (0)
#define SUB_PMIC_OTP1_55_R_np_otp1_pdob55_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob56 : 8;
    } reg;
} SUB_PMIC_OTP1_56_R_UNION;
#endif
#define SUB_PMIC_OTP1_56_R_np_otp1_pdob56_START (0)
#define SUB_PMIC_OTP1_56_R_np_otp1_pdob56_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob57 : 8;
    } reg;
} SUB_PMIC_OTP1_57_R_UNION;
#endif
#define SUB_PMIC_OTP1_57_R_np_otp1_pdob57_START (0)
#define SUB_PMIC_OTP1_57_R_np_otp1_pdob57_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob58 : 8;
    } reg;
} SUB_PMIC_OTP1_58_R_UNION;
#endif
#define SUB_PMIC_OTP1_58_R_np_otp1_pdob58_START (0)
#define SUB_PMIC_OTP1_58_R_np_otp1_pdob58_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob59 : 8;
    } reg;
} SUB_PMIC_OTP1_59_R_UNION;
#endif
#define SUB_PMIC_OTP1_59_R_np_otp1_pdob59_START (0)
#define SUB_PMIC_OTP1_59_R_np_otp1_pdob59_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob60 : 8;
    } reg;
} SUB_PMIC_OTP1_60_R_UNION;
#endif
#define SUB_PMIC_OTP1_60_R_np_otp1_pdob60_START (0)
#define SUB_PMIC_OTP1_60_R_np_otp1_pdob60_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob61 : 8;
    } reg;
} SUB_PMIC_OTP1_61_R_UNION;
#endif
#define SUB_PMIC_OTP1_61_R_np_otp1_pdob61_START (0)
#define SUB_PMIC_OTP1_61_R_np_otp1_pdob61_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob62 : 8;
    } reg;
} SUB_PMIC_OTP1_62_R_UNION;
#endif
#define SUB_PMIC_OTP1_62_R_np_otp1_pdob62_START (0)
#define SUB_PMIC_OTP1_62_R_np_otp1_pdob62_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob63 : 8;
    } reg;
} SUB_PMIC_OTP1_63_R_UNION;
#endif
#define SUB_PMIC_OTP1_63_R_np_otp1_pdob63_START (0)
#define SUB_PMIC_OTP1_63_R_np_otp1_pdob63_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck10_en : 1;
        unsigned char st_buck10_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck10_eco_en : 1;
        unsigned char st_buck10_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK10_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_BUCK10_ONOFF_ECO_reg_buck10_en_START (0)
#define SUB_PMIC_BUCK10_ONOFF_ECO_reg_buck10_en_END (0)
#define SUB_PMIC_BUCK10_ONOFF_ECO_st_buck10_en_START (1)
#define SUB_PMIC_BUCK10_ONOFF_ECO_st_buck10_en_END (1)
#define SUB_PMIC_BUCK10_ONOFF_ECO_reg_buck10_eco_en_START (4)
#define SUB_PMIC_BUCK10_ONOFF_ECO_reg_buck10_eco_en_END (4)
#define SUB_PMIC_BUCK10_ONOFF_ECO_st_buck10_eco_en_START (5)
#define SUB_PMIC_BUCK10_ONOFF_ECO_st_buck10_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck10_vset_cfg : 8;
    } reg;
} SUB_PMIC_BUCK10_VSET_UNION;
#endif
#define SUB_PMIC_BUCK10_VSET_buck10_vset_cfg_START (0)
#define SUB_PMIC_BUCK10_VSET_buck10_vset_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck10_rampdown_ctrl : 3;
        unsigned char buck10_rampup_ctrl : 3;
        unsigned char buck10_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_RAMP_BUCK10_CTRL0_UNION;
#endif
#define SUB_PMIC_RAMP_BUCK10_CTRL0_buck10_rampdown_ctrl_START (0)
#define SUB_PMIC_RAMP_BUCK10_CTRL0_buck10_rampdown_ctrl_END (2)
#define SUB_PMIC_RAMP_BUCK10_CTRL0_buck10_rampup_ctrl_START (3)
#define SUB_PMIC_RAMP_BUCK10_CTRL0_buck10_rampup_ctrl_END (5)
#define SUB_PMIC_RAMP_BUCK10_CTRL0_buck10_ramp_en_cfg_START (6)
#define SUB_PMIC_RAMP_BUCK10_CTRL0_buck10_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck10_pull_down_off_cfg : 3;
        unsigned char buck10_pull_down_on_cfg : 3;
        unsigned char buck10_pull_down_cfg : 1;
        unsigned char buck10_pull_down_mode : 1;
    } reg;
} SUB_PMIC_RAMP_BUCK10_CTRL1_UNION;
#endif
#define SUB_PMIC_RAMP_BUCK10_CTRL1_buck10_pull_down_off_cfg_START (0)
#define SUB_PMIC_RAMP_BUCK10_CTRL1_buck10_pull_down_off_cfg_END (2)
#define SUB_PMIC_RAMP_BUCK10_CTRL1_buck10_pull_down_on_cfg_START (3)
#define SUB_PMIC_RAMP_BUCK10_CTRL1_buck10_pull_down_on_cfg_END (5)
#define SUB_PMIC_RAMP_BUCK10_CTRL1_buck10_pull_down_cfg_START (6)
#define SUB_PMIC_RAMP_BUCK10_CTRL1_buck10_pull_down_cfg_END (6)
#define SUB_PMIC_RAMP_BUCK10_CTRL1_buck10_pull_down_mode_START (7)
#define SUB_PMIC_RAMP_BUCK10_CTRL1_buck10_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck10_dly_ru_off_cfg : 3;
        unsigned char buck10_dly_ru_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK10_RAMPUP_STATE_UNION;
#endif
#define SUB_PMIC_BUCK10_RAMPUP_STATE_buck10_dly_ru_off_cfg_START (0)
#define SUB_PMIC_BUCK10_RAMPUP_STATE_buck10_dly_ru_off_cfg_END (2)
#define SUB_PMIC_BUCK10_RAMPUP_STATE_buck10_dly_ru_on_cfg_START (3)
#define SUB_PMIC_BUCK10_RAMPUP_STATE_buck10_dly_ru_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck10_dly_rd_pd_off_cfg : 3;
        unsigned char buck10_dly_rd_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK10_RAMPDOWN_STATE_UNION;
#endif
#define SUB_PMIC_BUCK10_RAMPDOWN_STATE_buck10_dly_rd_pd_off_cfg_START (0)
#define SUB_PMIC_BUCK10_RAMPDOWN_STATE_buck10_dly_rd_pd_off_cfg_END (2)
#define SUB_PMIC_BUCK10_RAMPDOWN_STATE_buck10_dly_rd_on_cfg_START (3)
#define SUB_PMIC_BUCK10_RAMPDOWN_STATE_buck10_dly_rd_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck10_rampdown_chg_rate : 2;
        unsigned char reserved_0 : 2;
        unsigned char buck10_rampup_chg_rate : 2;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK10_RAMP_CHG_RATE_UNION;
#endif
#define SUB_PMIC_BUCK10_RAMP_CHG_RATE_buck10_rampdown_chg_rate_START (0)
#define SUB_PMIC_BUCK10_RAMP_CHG_RATE_buck10_rampdown_chg_rate_END (1)
#define SUB_PMIC_BUCK10_RAMP_CHG_RATE_buck10_rampup_chg_rate_START (4)
#define SUB_PMIC_BUCK10_RAMP_CHG_RATE_buck10_rampup_chg_rate_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck10_rampdown_chg_vset : 3;
        unsigned char reserved_0 : 1;
        unsigned char buck10_rampup_chg_vset : 3;
        unsigned char reserved_1 : 1;
    } reg;
} SUB_PMIC_BUCK10_RAMP_CHG_VSET_UNION;
#endif
#define SUB_PMIC_BUCK10_RAMP_CHG_VSET_buck10_rampdown_chg_vset_START (0)
#define SUB_PMIC_BUCK10_RAMP_CHG_VSET_buck10_rampdown_chg_vset_END (2)
#define SUB_PMIC_BUCK10_RAMP_CHG_VSET_buck10_rampup_chg_vset_START (4)
#define SUB_PMIC_BUCK10_RAMP_CHG_VSET_buck10_rampup_chg_vset_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck10_rd_nochg_rate_sel : 1;
        unsigned char reserved_0 : 3;
        unsigned char buck10_ru_nochg_rate_sel : 1;
        unsigned char reserved_1 : 3;
    } reg;
} SUB_PMIC_BUCK10_CHG_RATE_SEL_UNION;
#endif
#define SUB_PMIC_BUCK10_CHG_RATE_SEL_buck10_rd_nochg_rate_sel_START (0)
#define SUB_PMIC_BUCK10_CHG_RATE_SEL_buck10_rd_nochg_rate_sel_END (0)
#define SUB_PMIC_BUCK10_CHG_RATE_SEL_buck10_ru_nochg_rate_sel_START (4)
#define SUB_PMIC_BUCK10_CHG_RATE_SEL_buck10_ru_nochg_rate_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck10_ramp_state : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_BUCK10_RAMP_STATE_UNION;
#endif
#define SUB_PMIC_BUCK10_RAMP_STATE_buck10_ramp_state_START (0)
#define SUB_PMIC_BUCK10_RAMP_STATE_buck10_ramp_state_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_bw_sel : 1;
        unsigned char b10_buffer_bias : 2;
        unsigned char b10_b10_ea_r : 3;
        unsigned char b10_avg_curr_filter_sel : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_0_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_0_b10_bw_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_0_b10_bw_sel_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_0_b10_buffer_bias_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_0_b10_buffer_bias_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_0_b10_b10_ea_r_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_0_b10_b10_ea_r_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_0_b10_avg_curr_filter_sel_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_0_b10_avg_curr_filter_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_c1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_1_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_1_b10_c1_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_1_b10_c1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_c2_sel : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_2_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_2_b10_c2_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_2_b10_c2_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_clk_chopper_sel : 2;
        unsigned char b10_c3_sel : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_3_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_3_b10_clk_chopper_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_3_b10_clk_chopper_sel_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_3_b10_c3_sel_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_3_b10_c3_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_clock_atest : 1;
        unsigned char b10_clk9m6_pulse_sel : 2;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_4_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_4_b10_clock_atest_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_4_b10_clock_atest_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_4_b10_clk9m6_pulse_sel_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_4_b10_clk9m6_pulse_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_clock_sel : 8;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_5_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_5_b10_clock_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_5_b10_clock_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_clp_reg_idrp : 4;
        unsigned char b10_clp_ref_sel : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_6_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_6_b10_clp_reg_idrp_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_6_b10_clp_reg_idrp_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_6_b10_clp_ref_sel_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_6_b10_clp_ref_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_cot_cmphys : 1;
        unsigned char b10_cot_atest : 1;
        unsigned char b10_code_mode_sel : 1;
        unsigned char b10_cmp_atest : 1;
        unsigned char b10_clpeco_bias : 4;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_7_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_7_b10_cot_cmphys_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_7_b10_cot_cmphys_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_7_b10_cot_atest_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_7_b10_cot_atest_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_7_b10_code_mode_sel_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_7_b10_code_mode_sel_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_7_b10_cmp_atest_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_7_b10_cmp_atest_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_7_b10_clpeco_bias_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_7_b10_clpeco_bias_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_cot_sel : 1;
        unsigned char b10_cot_sche_neco : 1;
        unsigned char b10_cot_sche_eco : 1;
        unsigned char b10_cot_res_sel : 3;
        unsigned char b10_cot_iboost_sel : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_8_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_8_b10_cot_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_8_b10_cot_sel_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_8_b10_cot_sche_neco_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_8_b10_cot_sche_neco_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_8_b10_cot_sche_eco_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_8_b10_cot_sche_eco_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_8_b10_cot_res_sel_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_8_b10_cot_res_sel_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_8_b10_cot_iboost_sel_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_8_b10_cot_iboost_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_cot_ton_ecoa : 8;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_9_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_9_b10_cot_ton_ecoa_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_9_b10_cot_ton_ecoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_cot_ton_isen_hys : 2;
        unsigned char b10_cot_ton_isen_bias : 2;
        unsigned char b10_cot_ton_ecob : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_10_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_10_b10_cot_ton_isen_hys_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_10_b10_cot_ton_isen_hys_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_10_b10_cot_ton_isen_bias_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_10_b10_cot_ton_isen_bias_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_10_b10_cot_ton_ecob_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_10_b10_cot_ton_ecob_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_cot_ton_necoa : 8;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_11_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_11_b10_cot_ton_necoa_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_11_b10_cot_ton_necoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_crtshare_bw_sel : 2;
        unsigned char b10_crtshare_atest : 1;
        unsigned char b10_cot_ton_necob : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_12_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_12_b10_crtshare_bw_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_12_b10_crtshare_bw_sel_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_12_b10_crtshare_atest_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_12_b10_crtshare_atest_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_12_b10_cot_ton_necob_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_12_b10_cot_ton_necob_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_cs_test_sel : 1;
        unsigned char b10_crtshare_trim : 1;
        unsigned char b10_crtshare_filter_r_sel : 1;
        unsigned char b10_crtshare_clp_sel : 4;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_13_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_13_b10_cs_test_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_13_b10_cs_test_sel_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_13_b10_crtshare_trim_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_13_b10_crtshare_trim_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_13_b10_crtshare_filter_r_sel_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_13_b10_crtshare_filter_r_sel_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_13_b10_crtshare_clp_sel_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_13_b10_crtshare_clp_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_dbias : 3;
        unsigned char b10_ctlogic_smph : 1;
        unsigned char b10_ctlogic_sgph : 1;
        unsigned char b10_ctlogic_modsw : 1;
        unsigned char b10_ctlogic_crson_td : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_14_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_14_b10_dbias_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_14_b10_dbias_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_14_b10_ctlogic_smph_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_14_b10_ctlogic_smph_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_14_b10_ctlogic_sgph_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_14_b10_ctlogic_sgph_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_14_b10_ctlogic_modsw_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_14_b10_ctlogic_modsw_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_14_b10_ctlogic_crson_td_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_14_b10_ctlogic_crson_td_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_dmd_auto_eco : 1;
        unsigned char b10_dmd_auto_cmpib : 2;
        unsigned char b10_dmd_auto_ccm : 1;
        unsigned char b10_dmd_atest : 1;
        unsigned char b10_dbias_eco : 3;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_15_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_15_b10_dmd_auto_eco_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_15_b10_dmd_auto_eco_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_15_b10_dmd_auto_cmpib_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_15_b10_dmd_auto_cmpib_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_15_b10_dmd_auto_ccm_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_15_b10_dmd_auto_ccm_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_15_b10_dmd_atest_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_15_b10_dmd_atest_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_15_b10_dbias_eco_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_15_b10_dbias_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_dmd_bck_sel : 1;
        unsigned char b10_dmd_auto_vos : 3;
        unsigned char b10_dmd_auto_vbc : 1;
        unsigned char b10_dmd_auto_sel : 1;
        unsigned char b10_dmd_auto_sampt : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_16_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_16_b10_dmd_bck_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_16_b10_dmd_bck_sel_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_16_b10_dmd_auto_vos_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_16_b10_dmd_auto_vos_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_16_b10_dmd_auto_vbc_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_16_b10_dmd_auto_vbc_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_16_b10_dmd_auto_sel_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_16_b10_dmd_auto_sel_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_16_b10_dmd_auto_sampt_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_16_b10_dmd_auto_sampt_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_dmd_ngc_sel : 1;
        unsigned char b10_dmd_ngc_disch : 1;
        unsigned char b10_dmd_man_set : 4;
        unsigned char b10_dmd_eco_sel : 1;
        unsigned char b10_dmd_clp_sel : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_17_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_17_b10_dmd_ngc_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_17_b10_dmd_ngc_sel_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_17_b10_dmd_ngc_disch_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_17_b10_dmd_ngc_disch_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_17_b10_dmd_man_set_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_17_b10_dmd_man_set_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_17_b10_dmd_eco_sel_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_17_b10_dmd_eco_sel_END (6)
#define SUB_PMIC_BUCK10_CFG_REG_17_b10_dmd_clp_sel_START (7)
#define SUB_PMIC_BUCK10_CFG_REG_17_b10_dmd_clp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_drv_sw_sel : 1;
        unsigned char b10_drv_dt_sel : 4;
        unsigned char b10_dmd_ngc_set : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_18_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_18_b10_drv_sw_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_18_b10_drv_sw_sel_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_18_b10_drv_dt_sel_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_18_b10_drv_dt_sel_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_18_b10_dmd_ngc_set_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_18_b10_dmd_ngc_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_dt_sel : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_19_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_19_b10_dt_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_19_b10_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_eabias : 1;
        unsigned char b10_eaamp_clp_sel : 1;
        unsigned char b10_eaamp_clp : 2;
        unsigned char b10_ea_ecocur_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_20_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_20_b10_eabias_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_20_b10_eabias_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_20_b10_eaamp_clp_sel_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_20_b10_eaamp_clp_sel_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_20_b10_eaamp_clp_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_20_b10_eaamp_clp_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_20_b10_ea_ecocur_sel_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_20_b10_ea_ecocur_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_eaclp_ecogap_sel : 2;
        unsigned char b10_eaclp_brgap_sel : 4;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_21_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_21_b10_eaclp_ecogap_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_21_b10_eaclp_ecogap_sel_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_21_b10_eaclp_brgap_sel_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_21_b10_eaclp_brgap_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_eacomp_eco_shield : 1;
        unsigned char b10_eaclp_srgap_sel : 4;
        unsigned char b10_eaclp_gap_sel : 3;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_22_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_22_b10_eacomp_eco_shield_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_22_b10_eacomp_eco_shield_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_22_b10_eaclp_srgap_sel_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_22_b10_eaclp_srgap_sel_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_22_b10_eaclp_gap_sel_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_22_b10_eaclp_gap_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_ecdt_ct2cm_ref : 3;
        unsigned char b10_ecdt_ct2cm_ft : 2;
        unsigned char b10_ecdt_cmphys : 1;
        unsigned char b10_eatrim_en : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_23_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_23_b10_ecdt_ct2cm_ref_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_23_b10_ecdt_ct2cm_ref_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_23_b10_ecdt_ct2cm_ft_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_23_b10_ecdt_ct2cm_ft_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_23_b10_ecdt_cmphys_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_23_b10_ecdt_cmphys_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_23_b10_eatrim_en_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_23_b10_eatrim_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_ecdt_ec2ct_ref : 3;
        unsigned char b10_ecdt_ct2ec_ref : 3;
        unsigned char b10_ecdt_ct2ec_ft : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_24_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_24_b10_ecdt_ec2ct_ref_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_24_b10_ecdt_ec2ct_ref_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_24_b10_ecdt_ct2ec_ref_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_24_b10_ecdt_ct2ec_ref_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_24_b10_ecdt_ct2ec_ft_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_24_b10_ecdt_ct2ec_ft_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_en_psns : 1;
        unsigned char b10_en_nsense : 1;
        unsigned char b10_en_chopper : 1;
        unsigned char b10_ecotrim_shield : 1;
        unsigned char b10_eco_shield_cmp : 1;
        unsigned char b10_eco_shield : 1;
        unsigned char b10_ecdt_filter_rc : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_25_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_25_b10_en_psns_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_25_b10_en_psns_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_25_b10_en_nsense_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_25_b10_en_nsense_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_25_b10_en_chopper_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_25_b10_en_chopper_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_25_b10_ecotrim_shield_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_25_b10_ecotrim_shield_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_25_b10_eco_shield_cmp_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_25_b10_eco_shield_cmp_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_25_b10_eco_shield_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_25_b10_eco_shield_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_25_b10_ecdt_filter_rc_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_25_b10_ecdt_filter_rc_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_ldline_forced_on : 1;
        unsigned char b10_ibalance_gm_sel : 2;
        unsigned char b10_forcenormal : 1;
        unsigned char b10_forceeco : 1;
        unsigned char b10_fastnmos_off : 1;
        unsigned char b10_en_vo : 1;
        unsigned char b10_en_test : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_26_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_26_b10_ldline_forced_on_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_26_b10_ldline_forced_on_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_26_b10_ibalance_gm_sel_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_26_b10_ibalance_gm_sel_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_26_b10_forcenormal_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_26_b10_forcenormal_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_26_b10_forceeco_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_26_b10_forceeco_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_26_b10_fastnmos_off_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_26_b10_fastnmos_off_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_26_b10_en_vo_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_26_b10_en_vo_END (6)
#define SUB_PMIC_BUCK10_CFG_REG_26_b10_en_test_START (7)
#define SUB_PMIC_BUCK10_CFG_REG_26_b10_en_test_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_load_det_sel : 2;
        unsigned char b10_ldline_mohm_sel : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_27_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_27_b10_load_det_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_27_b10_load_det_sel_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_27_b10_ldline_mohm_sel_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_27_b10_ldline_mohm_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_min_ton_sel : 2;
        unsigned char b10_min_toff_sel : 2;
        unsigned char b10_load_state_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_28_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_28_b10_min_ton_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_28_b10_min_ton_sel_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_28_b10_min_toff_sel_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_28_b10_min_toff_sel_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_28_b10_load_state_sel_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_28_b10_load_state_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_ng_dt_sel : 3;
        unsigned char b10_negdmdton_add : 1;
        unsigned char b10_negdmd_ton_sel : 2;
        unsigned char b10_n_sel : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_29_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_29_b10_ng_dt_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_29_b10_ng_dt_sel_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_29_b10_negdmdton_add_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_29_b10_negdmdton_add_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_29_b10_negdmd_ton_sel_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_29_b10_negdmd_ton_sel_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_29_b10_n_sel_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_29_b10_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_nmos_off : 1;
        unsigned char b10_ng_p_sel : 3;
        unsigned char b10_ng_n_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_30_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_30_b10_nmos_off_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_30_b10_nmos_off_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_30_b10_ng_p_sel_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_30_b10_ng_p_sel_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_30_b10_ng_n_sel_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_30_b10_ng_n_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_nocp_low_sel : 2;
        unsigned char b10_nocp_low_eco_sel : 1;
        unsigned char b10_nocp_low_delay_sel : 2;
        unsigned char b10_nocp_en : 1;
        unsigned char b10_nocp_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_31_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_31_b10_nocp_low_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_31_b10_nocp_low_sel_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_31_b10_nocp_low_eco_sel_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_31_b10_nocp_low_eco_sel_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_31_b10_nocp_low_delay_sel_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_31_b10_nocp_low_delay_sel_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_31_b10_nocp_en_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_31_b10_nocp_en_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_31_b10_nocp_delay_sel_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_31_b10_nocp_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_ocp_2us_delay_sel : 1;
        unsigned char b10_nsense_sel : 3;
        unsigned char b10_npocp_atest : 1;
        unsigned char b10_normaltrim_shield : 1;
        unsigned char b10_nocp_sel : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_32_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_32_b10_ocp_2us_delay_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_32_b10_ocp_2us_delay_sel_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_32_b10_nsense_sel_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_32_b10_nsense_sel_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_32_b10_npocp_atest_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_32_b10_npocp_atest_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_32_b10_normaltrim_shield_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_32_b10_normaltrim_shield_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_32_b10_nocp_sel_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_32_b10_nocp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_p_sel : 2;
        unsigned char b10_ovp_shield : 1;
        unsigned char b10_ovp_sel : 2;
        unsigned char b10_oneph : 1;
        unsigned char b10_ocp_short_en : 1;
        unsigned char b10_ocp_shield : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_33_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_33_b10_p_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_33_b10_p_sel_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_33_b10_ovp_shield_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_33_b10_ovp_shield_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_33_b10_ovp_sel_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_33_b10_ovp_sel_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_33_b10_oneph_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_33_b10_oneph_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_33_b10_ocp_short_en_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_33_b10_ocp_short_en_END (6)
#define SUB_PMIC_BUCK10_CFG_REG_33_b10_ocp_shield_START (7)
#define SUB_PMIC_BUCK10_CFG_REG_33_b10_ocp_shield_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_pg_p_sel : 3;
        unsigned char b10_pg_n_sel : 3;
        unsigned char b10_pg_dt_sel : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_34_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_34_b10_pg_p_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_34_b10_pg_p_sel_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_34_b10_pg_n_sel_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_34_b10_pg_n_sel_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_34_b10_pg_dt_sel_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_34_b10_pg_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_pocp_sw_ccm_cmp : 1;
        unsigned char b10_pocp_sw_atest : 1;
        unsigned char b10_pocp_sel : 2;
        unsigned char b10_pocp_en : 1;
        unsigned char b10_pocp_eco_sel : 1;
        unsigned char b10_pocp_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_35_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_35_b10_pocp_sw_ccm_cmp_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_35_b10_pocp_sw_ccm_cmp_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_35_b10_pocp_sw_atest_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_35_b10_pocp_sw_atest_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_35_b10_pocp_sel_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_35_b10_pocp_sel_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_35_b10_pocp_en_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_35_b10_pocp_en_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_35_b10_pocp_eco_sel_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_35_b10_pocp_eco_sel_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_35_b10_pocp_delay_sel_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_35_b10_pocp_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_pulldn_pd_sel : 1;
        unsigned char b10_pulldn_pd_en : 1;
        unsigned char b10_pocp_sw_sel : 3;
        unsigned char b10_pocp_sw_eco_cmp : 1;
        unsigned char b10_pocp_sw_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_36_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_36_b10_pulldn_pd_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_36_b10_pulldn_pd_sel_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_36_b10_pulldn_pd_en_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_36_b10_pulldn_pd_en_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_36_b10_pocp_sw_sel_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_36_b10_pocp_sw_sel_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_36_b10_pocp_sw_eco_cmp_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_36_b10_pocp_sw_eco_cmp_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_36_b10_pocp_sw_delay_sel_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_36_b10_pocp_sw_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_r1_sel : 4;
        unsigned char b10_pvdd_ton_sel : 1;
        unsigned char b10_pulldn_sel : 3;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_37_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_37_b10_r1_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_37_b10_r1_sel_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_37_b10_pvdd_ton_sel_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_37_b10_pvdd_ton_sel_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_37_b10_pulldn_sel_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_37_b10_pulldn_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_r2_sel : 6;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_38_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_38_b10_r2_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_38_b10_r2_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_ramp_big_sel : 1;
        unsigned char b10_r3_sel : 6;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_39_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_39_b10_ramp_big_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_39_b10_ramp_big_sel_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_39_b10_r3_sel_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_39_b10_r3_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_ramp_ccm_outsel : 1;
        unsigned char b10_ramp_c : 2;
        unsigned char b10_ramp_buff_trim_test : 1;
        unsigned char b10_ramp_buff_trim : 1;
        unsigned char b10_ramp_buff_bias : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_40_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_40_b10_ramp_ccm_outsel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_40_b10_ramp_ccm_outsel_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_40_b10_ramp_c_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_40_b10_ramp_c_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_40_b10_ramp_buff_trim_test_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_40_b10_ramp_buff_trim_test_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_40_b10_ramp_buff_trim_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_40_b10_ramp_buff_trim_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_40_b10_ramp_buff_bias_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_40_b10_ramp_buff_bias_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_ramp_cot_outsel : 1;
        unsigned char b10_ramp_clk_sel : 1;
        unsigned char b10_ramp_change_mode : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_41_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_41_b10_ramp_cot_outsel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_41_b10_ramp_cot_outsel_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_41_b10_ramp_clk_sel_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_41_b10_ramp_clk_sel_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_41_b10_ramp_change_mode_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_41_b10_ramp_change_mode_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_ramp_line_sel : 1;
        unsigned char b10_ramp_freq_sel : 1;
        unsigned char b10_ramp_force_eco_test : 1;
        unsigned char b10_ramp_fast_sel : 1;
        unsigned char b10_ramp_eco_buffer : 1;
        unsigned char b10_ramp_dblclk_sel : 1;
        unsigned char b10_ramp_counter : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_42_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_42_b10_ramp_line_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_42_b10_ramp_line_sel_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_42_b10_ramp_freq_sel_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_42_b10_ramp_freq_sel_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_42_b10_ramp_force_eco_test_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_42_b10_ramp_force_eco_test_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_42_b10_ramp_fast_sel_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_42_b10_ramp_fast_sel_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_42_b10_ramp_eco_buffer_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_42_b10_ramp_eco_buffer_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_42_b10_ramp_dblclk_sel_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_42_b10_ramp_dblclk_sel_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_42_b10_ramp_counter_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_42_b10_ramp_counter_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_ramp_r_ccm : 4;
        unsigned char b10_ramp_mid_sel : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_43_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_43_b10_ramp_r_ccm_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_43_b10_ramp_r_ccm_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_43_b10_ramp_mid_sel_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_43_b10_ramp_mid_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_ramp_r_eco : 4;
        unsigned char b10_ramp_r_cot : 4;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_44_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_44_b10_ramp_r_eco_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_44_b10_ramp_r_eco_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_44_b10_ramp_r_cot_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_44_b10_ramp_r_cot_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_ramptrim_sel : 1;
        unsigned char b10_ramp_valley_sel : 1;
        unsigned char b10_ramp_trim_sel : 1;
        unsigned char b10_ramp_sw_sel : 1;
        unsigned char b10_ramp_sel : 1;
        unsigned char b10_ramp_res_test : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_45_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_45_b10_ramptrim_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_45_b10_ramptrim_sel_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_45_b10_ramp_valley_sel_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_45_b10_ramp_valley_sel_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_45_b10_ramp_trim_sel_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_45_b10_ramp_trim_sel_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_45_b10_ramp_sw_sel_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_45_b10_ramp_sw_sel_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_45_b10_ramp_sel_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_45_b10_ramp_sel_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_45_b10_ramp_res_test_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_45_b10_ramp_res_test_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_reg_idrp : 3;
        unsigned char b10_reg_idrn : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_46_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_46_b10_reg_idrp_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_46_b10_reg_idrp_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_46_b10_reg_idrn_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_46_b10_reg_idrn_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_reg_sense_res_sel : 3;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_47_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_47_b10_reg_sense_res_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_47_b10_reg_sense_res_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_reserve0 : 8;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_48_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_48_b10_reserve0_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_48_b10_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_reserve1 : 8;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_49_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_49_b10_reserve1_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_49_b10_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_reserve2 : 8;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_50_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_50_b10_reserve2_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_50_b10_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_reserve3 : 8;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_51_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_51_b10_reserve3_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_51_b10_reserve3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_sft : 1;
        unsigned char b10_sense_ratio_sel : 2;
        unsigned char b10_rtr_cap_sel : 2;
        unsigned char b10_ron_test_sel : 3;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_52_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_52_b10_sft_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_52_b10_sft_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_52_b10_sense_ratio_sel_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_52_b10_sense_ratio_sel_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_52_b10_rtr_cap_sel_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_52_b10_rtr_cap_sel_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_52_b10_ron_test_sel_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_52_b10_ron_test_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_stsw_atest : 1;
        unsigned char b10_state_sw_ocp_sel : 2;
        unsigned char b10_state_sw_ocp_off : 1;
        unsigned char b10_slave_sel : 1;
        unsigned char b10_short_shield : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_53_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_53_b10_stsw_atest_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_53_b10_stsw_atest_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_53_b10_state_sw_ocp_sel_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_53_b10_state_sw_ocp_sel_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_53_b10_state_sw_ocp_off_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_53_b10_state_sw_ocp_off_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_53_b10_slave_sel_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_53_b10_slave_sel_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_53_b10_short_shield_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_53_b10_short_shield_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_stsw_fstsch_eco : 3;
        unsigned char b10_stsw_fstps_ith : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_54_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_54_b10_stsw_fstsch_eco_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_54_b10_stsw_fstsch_eco_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_54_b10_stsw_fstps_ith_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_54_b10_stsw_fstps_ith_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_stsw_slw_ct2cm_hys : 2;
        unsigned char b10_stsw_rpup_ccm : 1;
        unsigned char b10_stsw_man_set : 2;
        unsigned char b10_stsw_fstsch_neco : 3;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_55_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_55_b10_stsw_slw_ct2cm_hys_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_55_b10_stsw_slw_ct2cm_hys_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_55_b10_stsw_rpup_ccm_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_55_b10_stsw_rpup_ccm_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_55_b10_stsw_man_set_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_55_b10_stsw_man_set_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_55_b10_stsw_fstsch_neco_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_55_b10_stsw_fstsch_neco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_stsw_slw_hys_shield : 1;
        unsigned char b10_stsw_slw_delay_l : 2;
        unsigned char b10_stsw_slw_delay_h : 2;
        unsigned char b10_stsw_slw_ct2cm_ith : 3;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_56_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_56_b10_stsw_slw_hys_shield_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_56_b10_stsw_slw_hys_shield_END (0)
#define SUB_PMIC_BUCK10_CFG_REG_56_b10_stsw_slw_delay_l_START (1)
#define SUB_PMIC_BUCK10_CFG_REG_56_b10_stsw_slw_delay_l_END (2)
#define SUB_PMIC_BUCK10_CFG_REG_56_b10_stsw_slw_delay_h_START (3)
#define SUB_PMIC_BUCK10_CFG_REG_56_b10_stsw_slw_delay_h_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_56_b10_stsw_slw_ct2cm_ith_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_56_b10_stsw_slw_ct2cm_ith_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_ton_det : 4;
        unsigned char b10_stsw_slwsch_neco : 2;
        unsigned char b10_stsw_slwsch_eco : 2;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_57_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_57_b10_ton_det_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_57_b10_ton_det_END (3)
#define SUB_PMIC_BUCK10_CFG_REG_57_b10_stsw_slwsch_neco_START (4)
#define SUB_PMIC_BUCK10_CFG_REG_57_b10_stsw_slwsch_neco_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_57_b10_stsw_slwsch_eco_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_57_b10_stsw_slwsch_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_vref_test_sel : 2;
        unsigned char b10_vo_ton_res_sel : 3;
        unsigned char b10_trim2 : 1;
        unsigned char b10_trim1 : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_58_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_58_b10_vref_test_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_58_b10_vref_test_sel_END (1)
#define SUB_PMIC_BUCK10_CFG_REG_58_b10_vo_ton_res_sel_START (2)
#define SUB_PMIC_BUCK10_CFG_REG_58_b10_vo_ton_res_sel_END (4)
#define SUB_PMIC_BUCK10_CFG_REG_58_b10_trim2_START (5)
#define SUB_PMIC_BUCK10_CFG_REG_58_b10_trim2_END (5)
#define SUB_PMIC_BUCK10_CFG_REG_58_b10_trim1_START (6)
#define SUB_PMIC_BUCK10_CFG_REG_58_b10_trim1_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_vref_vo_cap_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK10_CFG_REG_59_UNION;
#endif
#define SUB_PMIC_BUCK10_CFG_REG_59_b10_vref_vo_cap_sel_START (0)
#define SUB_PMIC_BUCK10_CFG_REG_59_b10_vref_vo_cap_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b10_state_a2d : 2;
        unsigned char b10_pg : 1;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK10_RO_REG_60_UNION;
#endif
#define SUB_PMIC_BUCK10_RO_REG_60_b10_state_a2d_START (0)
#define SUB_PMIC_BUCK10_RO_REG_60_b10_state_a2d_END (1)
#define SUB_PMIC_BUCK10_RO_REG_60_b10_pg_START (2)
#define SUB_PMIC_BUCK10_RO_REG_60_b10_pg_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b11_cot_atest : 1;
        unsigned char b11_cmp_atest : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK11_CFG_REG_0_UNION;
#endif
#define SUB_PMIC_BUCK11_CFG_REG_0_b11_cot_atest_START (0)
#define SUB_PMIC_BUCK11_CFG_REG_0_b11_cot_atest_END (0)
#define SUB_PMIC_BUCK11_CFG_REG_0_b11_cmp_atest_START (1)
#define SUB_PMIC_BUCK11_CFG_REG_0_b11_cmp_atest_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b11_cot_ton_ecoa : 8;
    } reg;
} SUB_PMIC_BUCK11_CFG_REG_1_UNION;
#endif
#define SUB_PMIC_BUCK11_CFG_REG_1_b11_cot_ton_ecoa_START (0)
#define SUB_PMIC_BUCK11_CFG_REG_1_b11_cot_ton_ecoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b11_cot_ton_ecob : 3;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK11_CFG_REG_2_UNION;
#endif
#define SUB_PMIC_BUCK11_CFG_REG_2_b11_cot_ton_ecob_START (0)
#define SUB_PMIC_BUCK11_CFG_REG_2_b11_cot_ton_ecob_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b11_cot_ton_necoa : 8;
    } reg;
} SUB_PMIC_BUCK11_CFG_REG_3_UNION;
#endif
#define SUB_PMIC_BUCK11_CFG_REG_3_b11_cot_ton_necoa_START (0)
#define SUB_PMIC_BUCK11_CFG_REG_3_b11_cot_ton_necoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b11_dmd_man_set : 4;
        unsigned char b11_dmd_atest : 1;
        unsigned char b11_cot_ton_necob : 3;
    } reg;
} SUB_PMIC_BUCK11_CFG_REG_4_UNION;
#endif
#define SUB_PMIC_BUCK11_CFG_REG_4_b11_dmd_man_set_START (0)
#define SUB_PMIC_BUCK11_CFG_REG_4_b11_dmd_man_set_END (3)
#define SUB_PMIC_BUCK11_CFG_REG_4_b11_dmd_atest_START (4)
#define SUB_PMIC_BUCK11_CFG_REG_4_b11_dmd_atest_END (4)
#define SUB_PMIC_BUCK11_CFG_REG_4_b11_cot_ton_necob_START (5)
#define SUB_PMIC_BUCK11_CFG_REG_4_b11_cot_ton_necob_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b11_drv_sw_sel : 1;
        unsigned char b11_drv_dt_sel : 4;
        unsigned char b11_dmd_ngc_set : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK11_CFG_REG_5_UNION;
#endif
#define SUB_PMIC_BUCK11_CFG_REG_5_b11_drv_sw_sel_START (0)
#define SUB_PMIC_BUCK11_CFG_REG_5_b11_drv_sw_sel_END (0)
#define SUB_PMIC_BUCK11_CFG_REG_5_b11_drv_dt_sel_START (1)
#define SUB_PMIC_BUCK11_CFG_REG_5_b11_drv_dt_sel_END (4)
#define SUB_PMIC_BUCK11_CFG_REG_5_b11_dmd_ngc_set_START (5)
#define SUB_PMIC_BUCK11_CFG_REG_5_b11_dmd_ngc_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b11_dt_sel : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK11_CFG_REG_6_UNION;
#endif
#define SUB_PMIC_BUCK11_CFG_REG_6_b11_dt_sel_START (0)
#define SUB_PMIC_BUCK11_CFG_REG_6_b11_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b11_ng_n_sel : 3;
        unsigned char b11_ng_dt_sel : 3;
        unsigned char b11_n_sel : 2;
    } reg;
} SUB_PMIC_BUCK11_CFG_REG_7_UNION;
#endif
#define SUB_PMIC_BUCK11_CFG_REG_7_b11_ng_n_sel_START (0)
#define SUB_PMIC_BUCK11_CFG_REG_7_b11_ng_n_sel_END (2)
#define SUB_PMIC_BUCK11_CFG_REG_7_b11_ng_dt_sel_START (3)
#define SUB_PMIC_BUCK11_CFG_REG_7_b11_ng_dt_sel_END (5)
#define SUB_PMIC_BUCK11_CFG_REG_7_b11_n_sel_START (6)
#define SUB_PMIC_BUCK11_CFG_REG_7_b11_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b11_pg_dt_sel : 1;
        unsigned char b11_p_sel : 2;
        unsigned char b11_npocp_atest : 1;
        unsigned char b11_ng_p_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK11_CFG_REG_8_UNION;
#endif
#define SUB_PMIC_BUCK11_CFG_REG_8_b11_pg_dt_sel_START (0)
#define SUB_PMIC_BUCK11_CFG_REG_8_b11_pg_dt_sel_END (0)
#define SUB_PMIC_BUCK11_CFG_REG_8_b11_p_sel_START (1)
#define SUB_PMIC_BUCK11_CFG_REG_8_b11_p_sel_END (2)
#define SUB_PMIC_BUCK11_CFG_REG_8_b11_npocp_atest_START (3)
#define SUB_PMIC_BUCK11_CFG_REG_8_b11_npocp_atest_END (3)
#define SUB_PMIC_BUCK11_CFG_REG_8_b11_ng_p_sel_START (4)
#define SUB_PMIC_BUCK11_CFG_REG_8_b11_ng_p_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b11_ramp_buff_trim_test : 1;
        unsigned char b11_pocp_sw_atest : 1;
        unsigned char b11_pg_p_sel : 3;
        unsigned char b11_pg_n_sel : 3;
    } reg;
} SUB_PMIC_BUCK11_CFG_REG_9_UNION;
#endif
#define SUB_PMIC_BUCK11_CFG_REG_9_b11_ramp_buff_trim_test_START (0)
#define SUB_PMIC_BUCK11_CFG_REG_9_b11_ramp_buff_trim_test_END (0)
#define SUB_PMIC_BUCK11_CFG_REG_9_b11_pocp_sw_atest_START (1)
#define SUB_PMIC_BUCK11_CFG_REG_9_b11_pocp_sw_atest_END (1)
#define SUB_PMIC_BUCK11_CFG_REG_9_b11_pg_p_sel_START (2)
#define SUB_PMIC_BUCK11_CFG_REG_9_b11_pg_p_sel_END (4)
#define SUB_PMIC_BUCK11_CFG_REG_9_b11_pg_n_sel_START (5)
#define SUB_PMIC_BUCK11_CFG_REG_9_b11_pg_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b11_ron_test_sel : 3;
        unsigned char b11_ramp_res_test : 1;
        unsigned char b11_ramp_r_ccm : 4;
    } reg;
} SUB_PMIC_BUCK11_CFG_REG_10_UNION;
#endif
#define SUB_PMIC_BUCK11_CFG_REG_10_b11_ron_test_sel_START (0)
#define SUB_PMIC_BUCK11_CFG_REG_10_b11_ron_test_sel_END (2)
#define SUB_PMIC_BUCK11_CFG_REG_10_b11_ramp_res_test_START (3)
#define SUB_PMIC_BUCK11_CFG_REG_10_b11_ramp_res_test_END (3)
#define SUB_PMIC_BUCK11_CFG_REG_10_b11_ramp_r_ccm_START (4)
#define SUB_PMIC_BUCK11_CFG_REG_10_b11_ramp_r_ccm_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck20_en : 1;
        unsigned char st_buck20_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck20_eco_en : 1;
        unsigned char st_buck20_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK20_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_BUCK20_ONOFF_ECO_reg_buck20_en_START (0)
#define SUB_PMIC_BUCK20_ONOFF_ECO_reg_buck20_en_END (0)
#define SUB_PMIC_BUCK20_ONOFF_ECO_st_buck20_en_START (1)
#define SUB_PMIC_BUCK20_ONOFF_ECO_st_buck20_en_END (1)
#define SUB_PMIC_BUCK20_ONOFF_ECO_reg_buck20_eco_en_START (4)
#define SUB_PMIC_BUCK20_ONOFF_ECO_reg_buck20_eco_en_END (4)
#define SUB_PMIC_BUCK20_ONOFF_ECO_st_buck20_eco_en_START (5)
#define SUB_PMIC_BUCK20_ONOFF_ECO_st_buck20_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck20_vset_cfg : 8;
    } reg;
} SUB_PMIC_BUCK20_VSET_UNION;
#endif
#define SUB_PMIC_BUCK20_VSET_buck20_vset_cfg_START (0)
#define SUB_PMIC_BUCK20_VSET_buck20_vset_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck20_rampdown_ctrl : 3;
        unsigned char buck20_rampup_ctrl : 3;
        unsigned char buck20_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_RAMP_BUCK20_CTRL0_UNION;
#endif
#define SUB_PMIC_RAMP_BUCK20_CTRL0_buck20_rampdown_ctrl_START (0)
#define SUB_PMIC_RAMP_BUCK20_CTRL0_buck20_rampdown_ctrl_END (2)
#define SUB_PMIC_RAMP_BUCK20_CTRL0_buck20_rampup_ctrl_START (3)
#define SUB_PMIC_RAMP_BUCK20_CTRL0_buck20_rampup_ctrl_END (5)
#define SUB_PMIC_RAMP_BUCK20_CTRL0_buck20_ramp_en_cfg_START (6)
#define SUB_PMIC_RAMP_BUCK20_CTRL0_buck20_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck20_pull_down_off_cfg : 3;
        unsigned char buck20_pull_down_on_cfg : 3;
        unsigned char buck20_pull_down_cfg : 1;
        unsigned char buck20_pull_down_mode : 1;
    } reg;
} SUB_PMIC_RAMP_BUCK20_CTRL1_UNION;
#endif
#define SUB_PMIC_RAMP_BUCK20_CTRL1_buck20_pull_down_off_cfg_START (0)
#define SUB_PMIC_RAMP_BUCK20_CTRL1_buck20_pull_down_off_cfg_END (2)
#define SUB_PMIC_RAMP_BUCK20_CTRL1_buck20_pull_down_on_cfg_START (3)
#define SUB_PMIC_RAMP_BUCK20_CTRL1_buck20_pull_down_on_cfg_END (5)
#define SUB_PMIC_RAMP_BUCK20_CTRL1_buck20_pull_down_cfg_START (6)
#define SUB_PMIC_RAMP_BUCK20_CTRL1_buck20_pull_down_cfg_END (6)
#define SUB_PMIC_RAMP_BUCK20_CTRL1_buck20_pull_down_mode_START (7)
#define SUB_PMIC_RAMP_BUCK20_CTRL1_buck20_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck20_dly_ru_off_cfg : 3;
        unsigned char buck20_dly_ru_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK20_RAMPUP_STATE_UNION;
#endif
#define SUB_PMIC_BUCK20_RAMPUP_STATE_buck20_dly_ru_off_cfg_START (0)
#define SUB_PMIC_BUCK20_RAMPUP_STATE_buck20_dly_ru_off_cfg_END (2)
#define SUB_PMIC_BUCK20_RAMPUP_STATE_buck20_dly_ru_on_cfg_START (3)
#define SUB_PMIC_BUCK20_RAMPUP_STATE_buck20_dly_ru_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck20_dly_rd_pd_off_cfg : 3;
        unsigned char buck20_dly_rd_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK20_RAMPDOWN_STATE_UNION;
#endif
#define SUB_PMIC_BUCK20_RAMPDOWN_STATE_buck20_dly_rd_pd_off_cfg_START (0)
#define SUB_PMIC_BUCK20_RAMPDOWN_STATE_buck20_dly_rd_pd_off_cfg_END (2)
#define SUB_PMIC_BUCK20_RAMPDOWN_STATE_buck20_dly_rd_on_cfg_START (3)
#define SUB_PMIC_BUCK20_RAMPDOWN_STATE_buck20_dly_rd_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck20_rampdown_chg_rate : 2;
        unsigned char reserved_0 : 2;
        unsigned char buck20_rampup_chg_rate : 2;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK20_RAMP_CHG_RATE_UNION;
#endif
#define SUB_PMIC_BUCK20_RAMP_CHG_RATE_buck20_rampdown_chg_rate_START (0)
#define SUB_PMIC_BUCK20_RAMP_CHG_RATE_buck20_rampdown_chg_rate_END (1)
#define SUB_PMIC_BUCK20_RAMP_CHG_RATE_buck20_rampup_chg_rate_START (4)
#define SUB_PMIC_BUCK20_RAMP_CHG_RATE_buck20_rampup_chg_rate_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck20_rampdown_chg_vset : 3;
        unsigned char reserved_0 : 1;
        unsigned char buck20_rampup_chg_vset : 3;
        unsigned char reserved_1 : 1;
    } reg;
} SUB_PMIC_BUCK20_RAMP_CHG_VSET_UNION;
#endif
#define SUB_PMIC_BUCK20_RAMP_CHG_VSET_buck20_rampdown_chg_vset_START (0)
#define SUB_PMIC_BUCK20_RAMP_CHG_VSET_buck20_rampdown_chg_vset_END (2)
#define SUB_PMIC_BUCK20_RAMP_CHG_VSET_buck20_rampup_chg_vset_START (4)
#define SUB_PMIC_BUCK20_RAMP_CHG_VSET_buck20_rampup_chg_vset_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck20_rd_nochg_rate_sel : 1;
        unsigned char reserved_0 : 3;
        unsigned char buck20_ru_nochg_rate_sel : 1;
        unsigned char reserved_1 : 3;
    } reg;
} SUB_PMIC_BUCK20_CHG_RATE_SEL_UNION;
#endif
#define SUB_PMIC_BUCK20_CHG_RATE_SEL_buck20_rd_nochg_rate_sel_START (0)
#define SUB_PMIC_BUCK20_CHG_RATE_SEL_buck20_rd_nochg_rate_sel_END (0)
#define SUB_PMIC_BUCK20_CHG_RATE_SEL_buck20_ru_nochg_rate_sel_START (4)
#define SUB_PMIC_BUCK20_CHG_RATE_SEL_buck20_ru_nochg_rate_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck20_ramp_state : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_BUCK20_RAMP_STATE_UNION;
#endif
#define SUB_PMIC_BUCK20_RAMP_STATE_buck20_ramp_state_START (0)
#define SUB_PMIC_BUCK20_RAMP_STATE_buck20_ramp_state_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_bw_sel : 1;
        unsigned char b20_buffer_bias : 2;
        unsigned char b20_b20_ea_r : 3;
        unsigned char b20_avg_curr_filter_sel : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_0_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_0_b20_bw_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_0_b20_bw_sel_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_0_b20_buffer_bias_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_0_b20_buffer_bias_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_0_b20_b20_ea_r_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_0_b20_b20_ea_r_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_0_b20_avg_curr_filter_sel_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_0_b20_avg_curr_filter_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_c1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_1_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_1_b20_c1_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_1_b20_c1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_c2_sel : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_2_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_2_b20_c2_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_2_b20_c2_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_clk_chopper_sel : 2;
        unsigned char b20_c3_sel : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_3_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_3_b20_clk_chopper_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_3_b20_clk_chopper_sel_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_3_b20_c3_sel_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_3_b20_c3_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_clock_atest : 1;
        unsigned char b20_clk9m6_pulse_sel : 2;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_4_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_4_b20_clock_atest_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_4_b20_clock_atest_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_4_b20_clk9m6_pulse_sel_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_4_b20_clk9m6_pulse_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_clock_sel : 8;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_5_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_5_b20_clock_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_5_b20_clock_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_clp_reg_idrp : 4;
        unsigned char b20_clp_ref_sel : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_6_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_6_b20_clp_reg_idrp_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_6_b20_clp_reg_idrp_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_6_b20_clp_ref_sel_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_6_b20_clp_ref_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_cot_cmphys : 1;
        unsigned char b20_cot_atest : 1;
        unsigned char b20_code_mode_sel : 1;
        unsigned char b20_cmp_atest : 1;
        unsigned char b20_clpeco_bias : 4;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_7_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_7_b20_cot_cmphys_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_7_b20_cot_cmphys_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_7_b20_cot_atest_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_7_b20_cot_atest_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_7_b20_code_mode_sel_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_7_b20_code_mode_sel_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_7_b20_cmp_atest_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_7_b20_cmp_atest_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_7_b20_clpeco_bias_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_7_b20_clpeco_bias_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_cot_sel : 1;
        unsigned char b20_cot_sche_neco : 1;
        unsigned char b20_cot_sche_eco : 1;
        unsigned char b20_cot_res_sel : 3;
        unsigned char b20_cot_iboost_sel : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_8_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_8_b20_cot_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_8_b20_cot_sel_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_8_b20_cot_sche_neco_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_8_b20_cot_sche_neco_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_8_b20_cot_sche_eco_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_8_b20_cot_sche_eco_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_8_b20_cot_res_sel_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_8_b20_cot_res_sel_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_8_b20_cot_iboost_sel_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_8_b20_cot_iboost_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_cot_ton_ecoa : 8;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_9_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_9_b20_cot_ton_ecoa_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_9_b20_cot_ton_ecoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_cot_ton_isen_hys : 2;
        unsigned char b20_cot_ton_isen_bias : 2;
        unsigned char b20_cot_ton_ecob : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_10_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_10_b20_cot_ton_isen_hys_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_10_b20_cot_ton_isen_hys_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_10_b20_cot_ton_isen_bias_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_10_b20_cot_ton_isen_bias_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_10_b20_cot_ton_ecob_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_10_b20_cot_ton_ecob_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_cot_ton_necoa : 8;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_11_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_11_b20_cot_ton_necoa_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_11_b20_cot_ton_necoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_crtshare_bw_sel : 2;
        unsigned char b20_crtshare_atest : 1;
        unsigned char b20_cot_ton_necob : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_12_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_12_b20_crtshare_bw_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_12_b20_crtshare_bw_sel_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_12_b20_crtshare_atest_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_12_b20_crtshare_atest_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_12_b20_cot_ton_necob_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_12_b20_cot_ton_necob_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_cs_test_sel : 1;
        unsigned char b20_crtshare_trim : 1;
        unsigned char b20_crtshare_filter_r_sel : 1;
        unsigned char b20_crtshare_clp_sel : 4;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_13_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_13_b20_cs_test_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_13_b20_cs_test_sel_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_13_b20_crtshare_trim_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_13_b20_crtshare_trim_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_13_b20_crtshare_filter_r_sel_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_13_b20_crtshare_filter_r_sel_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_13_b20_crtshare_clp_sel_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_13_b20_crtshare_clp_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_dbias : 3;
        unsigned char b20_ctlogic_smph : 1;
        unsigned char b20_ctlogic_sgph : 1;
        unsigned char b20_ctlogic_modsw : 1;
        unsigned char b20_ctlogic_crson_td : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_14_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_14_b20_dbias_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_14_b20_dbias_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_14_b20_ctlogic_smph_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_14_b20_ctlogic_smph_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_14_b20_ctlogic_sgph_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_14_b20_ctlogic_sgph_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_14_b20_ctlogic_modsw_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_14_b20_ctlogic_modsw_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_14_b20_ctlogic_crson_td_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_14_b20_ctlogic_crson_td_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_dmd_auto_eco : 1;
        unsigned char b20_dmd_auto_cmpib : 2;
        unsigned char b20_dmd_auto_ccm : 1;
        unsigned char b20_dmd_atest : 1;
        unsigned char b20_dbias_eco : 3;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_15_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_15_b20_dmd_auto_eco_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_15_b20_dmd_auto_eco_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_15_b20_dmd_auto_cmpib_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_15_b20_dmd_auto_cmpib_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_15_b20_dmd_auto_ccm_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_15_b20_dmd_auto_ccm_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_15_b20_dmd_atest_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_15_b20_dmd_atest_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_15_b20_dbias_eco_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_15_b20_dbias_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_dmd_bck_sel : 1;
        unsigned char b20_dmd_auto_vos : 3;
        unsigned char b20_dmd_auto_vbc : 1;
        unsigned char b20_dmd_auto_sel : 1;
        unsigned char b20_dmd_auto_sampt : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_16_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_16_b20_dmd_bck_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_16_b20_dmd_bck_sel_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_16_b20_dmd_auto_vos_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_16_b20_dmd_auto_vos_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_16_b20_dmd_auto_vbc_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_16_b20_dmd_auto_vbc_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_16_b20_dmd_auto_sel_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_16_b20_dmd_auto_sel_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_16_b20_dmd_auto_sampt_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_16_b20_dmd_auto_sampt_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_dmd_ngc_sel : 1;
        unsigned char b20_dmd_ngc_disch : 1;
        unsigned char b20_dmd_man_set : 4;
        unsigned char b20_dmd_eco_sel : 1;
        unsigned char b20_dmd_clp_sel : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_17_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_17_b20_dmd_ngc_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_17_b20_dmd_ngc_sel_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_17_b20_dmd_ngc_disch_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_17_b20_dmd_ngc_disch_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_17_b20_dmd_man_set_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_17_b20_dmd_man_set_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_17_b20_dmd_eco_sel_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_17_b20_dmd_eco_sel_END (6)
#define SUB_PMIC_BUCK20_CFG_REG_17_b20_dmd_clp_sel_START (7)
#define SUB_PMIC_BUCK20_CFG_REG_17_b20_dmd_clp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_drv_sw_sel : 1;
        unsigned char b20_drv_dt_sel : 4;
        unsigned char b20_dmd_ngc_set : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_18_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_18_b20_drv_sw_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_18_b20_drv_sw_sel_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_18_b20_drv_dt_sel_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_18_b20_drv_dt_sel_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_18_b20_dmd_ngc_set_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_18_b20_dmd_ngc_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_dt_sel : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_19_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_19_b20_dt_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_19_b20_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_eabias : 1;
        unsigned char b20_eaamp_clp_sel : 1;
        unsigned char b20_eaamp_clp : 2;
        unsigned char b20_ea_ecocur_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_20_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_20_b20_eabias_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_20_b20_eabias_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_20_b20_eaamp_clp_sel_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_20_b20_eaamp_clp_sel_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_20_b20_eaamp_clp_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_20_b20_eaamp_clp_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_20_b20_ea_ecocur_sel_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_20_b20_ea_ecocur_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_eaclp_ecogap_sel : 2;
        unsigned char b20_eaclp_brgap_sel : 4;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_21_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_21_b20_eaclp_ecogap_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_21_b20_eaclp_ecogap_sel_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_21_b20_eaclp_brgap_sel_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_21_b20_eaclp_brgap_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_eacomp_eco_shield : 1;
        unsigned char b20_eaclp_srgap_sel : 4;
        unsigned char b20_eaclp_gap_sel : 3;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_22_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_22_b20_eacomp_eco_shield_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_22_b20_eacomp_eco_shield_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_22_b20_eaclp_srgap_sel_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_22_b20_eaclp_srgap_sel_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_22_b20_eaclp_gap_sel_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_22_b20_eaclp_gap_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_ecdt_ct2cm_ref : 3;
        unsigned char b20_ecdt_ct2cm_ft : 2;
        unsigned char b20_ecdt_cmphys : 1;
        unsigned char b20_eatrim_en : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_23_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_23_b20_ecdt_ct2cm_ref_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_23_b20_ecdt_ct2cm_ref_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_23_b20_ecdt_ct2cm_ft_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_23_b20_ecdt_ct2cm_ft_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_23_b20_ecdt_cmphys_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_23_b20_ecdt_cmphys_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_23_b20_eatrim_en_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_23_b20_eatrim_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_ecdt_ec2ct_ref : 3;
        unsigned char b20_ecdt_ct2ec_ref : 3;
        unsigned char b20_ecdt_ct2ec_ft : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_24_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_24_b20_ecdt_ec2ct_ref_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_24_b20_ecdt_ec2ct_ref_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_24_b20_ecdt_ct2ec_ref_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_24_b20_ecdt_ct2ec_ref_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_24_b20_ecdt_ct2ec_ft_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_24_b20_ecdt_ct2ec_ft_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_en_psns : 1;
        unsigned char b20_en_nsense : 1;
        unsigned char b20_en_chopper : 1;
        unsigned char b20_ecotrim_shield : 1;
        unsigned char b20_eco_shield_cmp : 1;
        unsigned char b20_eco_shield : 1;
        unsigned char b20_ecdt_filter_rc : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_25_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_25_b20_en_psns_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_25_b20_en_psns_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_25_b20_en_nsense_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_25_b20_en_nsense_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_25_b20_en_chopper_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_25_b20_en_chopper_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_25_b20_ecotrim_shield_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_25_b20_ecotrim_shield_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_25_b20_eco_shield_cmp_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_25_b20_eco_shield_cmp_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_25_b20_eco_shield_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_25_b20_eco_shield_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_25_b20_ecdt_filter_rc_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_25_b20_ecdt_filter_rc_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_ldline_forced_on : 1;
        unsigned char b20_ibalance_gm_sel : 2;
        unsigned char b20_forcenormal : 1;
        unsigned char b20_forceeco : 1;
        unsigned char b20_fastnmos_off : 1;
        unsigned char b20_en_vo : 1;
        unsigned char b20_en_test : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_26_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_26_b20_ldline_forced_on_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_26_b20_ldline_forced_on_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_26_b20_ibalance_gm_sel_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_26_b20_ibalance_gm_sel_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_26_b20_forcenormal_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_26_b20_forcenormal_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_26_b20_forceeco_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_26_b20_forceeco_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_26_b20_fastnmos_off_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_26_b20_fastnmos_off_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_26_b20_en_vo_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_26_b20_en_vo_END (6)
#define SUB_PMIC_BUCK20_CFG_REG_26_b20_en_test_START (7)
#define SUB_PMIC_BUCK20_CFG_REG_26_b20_en_test_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_load_det_sel : 2;
        unsigned char b20_ldline_mohm_sel : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_27_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_27_b20_load_det_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_27_b20_load_det_sel_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_27_b20_ldline_mohm_sel_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_27_b20_ldline_mohm_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_min_ton_sel : 2;
        unsigned char b20_min_toff_sel : 2;
        unsigned char b20_load_state_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_28_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_28_b20_min_ton_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_28_b20_min_ton_sel_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_28_b20_min_toff_sel_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_28_b20_min_toff_sel_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_28_b20_load_state_sel_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_28_b20_load_state_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_ng_dt_sel : 3;
        unsigned char b20_negdmdton_add : 1;
        unsigned char b20_negdmd_ton_sel : 2;
        unsigned char b20_n_sel : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_29_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_29_b20_ng_dt_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_29_b20_ng_dt_sel_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_29_b20_negdmdton_add_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_29_b20_negdmdton_add_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_29_b20_negdmd_ton_sel_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_29_b20_negdmd_ton_sel_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_29_b20_n_sel_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_29_b20_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_nmos_off : 1;
        unsigned char b20_ng_p_sel : 3;
        unsigned char b20_ng_n_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_30_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_30_b20_nmos_off_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_30_b20_nmos_off_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_30_b20_ng_p_sel_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_30_b20_ng_p_sel_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_30_b20_ng_n_sel_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_30_b20_ng_n_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_nocp_low_sel : 2;
        unsigned char b20_nocp_low_eco_sel : 1;
        unsigned char b20_nocp_low_delay_sel : 2;
        unsigned char b20_nocp_en : 1;
        unsigned char b20_nocp_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_31_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_31_b20_nocp_low_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_31_b20_nocp_low_sel_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_31_b20_nocp_low_eco_sel_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_31_b20_nocp_low_eco_sel_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_31_b20_nocp_low_delay_sel_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_31_b20_nocp_low_delay_sel_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_31_b20_nocp_en_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_31_b20_nocp_en_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_31_b20_nocp_delay_sel_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_31_b20_nocp_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_ocp_2us_delay_sel : 1;
        unsigned char b20_nsense_sel : 3;
        unsigned char b20_npocp_atest : 1;
        unsigned char b20_normaltrim_shield : 1;
        unsigned char b20_nocp_sel : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_32_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_32_b20_ocp_2us_delay_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_32_b20_ocp_2us_delay_sel_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_32_b20_nsense_sel_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_32_b20_nsense_sel_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_32_b20_npocp_atest_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_32_b20_npocp_atest_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_32_b20_normaltrim_shield_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_32_b20_normaltrim_shield_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_32_b20_nocp_sel_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_32_b20_nocp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_p_sel : 2;
        unsigned char b20_ovp_shield : 1;
        unsigned char b20_ovp_sel : 2;
        unsigned char b20_oneph : 1;
        unsigned char b20_ocp_short_en : 1;
        unsigned char b20_ocp_shield : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_33_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_33_b20_p_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_33_b20_p_sel_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_33_b20_ovp_shield_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_33_b20_ovp_shield_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_33_b20_ovp_sel_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_33_b20_ovp_sel_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_33_b20_oneph_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_33_b20_oneph_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_33_b20_ocp_short_en_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_33_b20_ocp_short_en_END (6)
#define SUB_PMIC_BUCK20_CFG_REG_33_b20_ocp_shield_START (7)
#define SUB_PMIC_BUCK20_CFG_REG_33_b20_ocp_shield_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_pg_p_sel : 3;
        unsigned char b20_pg_n_sel : 3;
        unsigned char b20_pg_dt_sel : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_34_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_34_b20_pg_p_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_34_b20_pg_p_sel_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_34_b20_pg_n_sel_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_34_b20_pg_n_sel_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_34_b20_pg_dt_sel_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_34_b20_pg_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_pocp_sw_ccm_cmp : 1;
        unsigned char b20_pocp_sw_atest : 1;
        unsigned char b20_pocp_sel : 2;
        unsigned char b20_pocp_en : 1;
        unsigned char b20_pocp_eco_sel : 1;
        unsigned char b20_pocp_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_35_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_35_b20_pocp_sw_ccm_cmp_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_35_b20_pocp_sw_ccm_cmp_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_35_b20_pocp_sw_atest_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_35_b20_pocp_sw_atest_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_35_b20_pocp_sel_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_35_b20_pocp_sel_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_35_b20_pocp_en_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_35_b20_pocp_en_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_35_b20_pocp_eco_sel_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_35_b20_pocp_eco_sel_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_35_b20_pocp_delay_sel_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_35_b20_pocp_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_pulldn_pd_sel : 1;
        unsigned char b20_pulldn_pd_en : 1;
        unsigned char b20_pocp_sw_sel : 3;
        unsigned char b20_pocp_sw_eco_cmp : 1;
        unsigned char b20_pocp_sw_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_36_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_36_b20_pulldn_pd_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_36_b20_pulldn_pd_sel_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_36_b20_pulldn_pd_en_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_36_b20_pulldn_pd_en_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_36_b20_pocp_sw_sel_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_36_b20_pocp_sw_sel_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_36_b20_pocp_sw_eco_cmp_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_36_b20_pocp_sw_eco_cmp_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_36_b20_pocp_sw_delay_sel_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_36_b20_pocp_sw_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_r1_sel : 4;
        unsigned char b20_pvdd_ton_sel : 1;
        unsigned char b20_pulldn_sel : 3;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_37_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_37_b20_r1_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_37_b20_r1_sel_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_37_b20_pvdd_ton_sel_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_37_b20_pvdd_ton_sel_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_37_b20_pulldn_sel_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_37_b20_pulldn_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_r2_sel : 6;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_38_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_38_b20_r2_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_38_b20_r2_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_ramp_big_sel : 1;
        unsigned char b20_r3_sel : 6;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_39_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_39_b20_ramp_big_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_39_b20_ramp_big_sel_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_39_b20_r3_sel_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_39_b20_r3_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_ramp_ccm_outsel : 1;
        unsigned char b20_ramp_c : 2;
        unsigned char b20_ramp_buff_trim_test : 1;
        unsigned char b20_ramp_buff_trim : 1;
        unsigned char b20_ramp_buff_bias : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_40_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_40_b20_ramp_ccm_outsel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_40_b20_ramp_ccm_outsel_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_40_b20_ramp_c_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_40_b20_ramp_c_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_40_b20_ramp_buff_trim_test_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_40_b20_ramp_buff_trim_test_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_40_b20_ramp_buff_trim_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_40_b20_ramp_buff_trim_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_40_b20_ramp_buff_bias_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_40_b20_ramp_buff_bias_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_ramp_cot_outsel : 1;
        unsigned char b20_ramp_clk_sel : 1;
        unsigned char b20_ramp_change_mode : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_41_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_41_b20_ramp_cot_outsel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_41_b20_ramp_cot_outsel_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_41_b20_ramp_clk_sel_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_41_b20_ramp_clk_sel_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_41_b20_ramp_change_mode_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_41_b20_ramp_change_mode_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_ramp_line_sel : 1;
        unsigned char b20_ramp_freq_sel : 1;
        unsigned char b20_ramp_force_eco_test : 1;
        unsigned char b20_ramp_fast_sel : 1;
        unsigned char b20_ramp_eco_buffer : 1;
        unsigned char b20_ramp_dblclk_sel : 1;
        unsigned char b20_ramp_counter : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_42_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_42_b20_ramp_line_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_42_b20_ramp_line_sel_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_42_b20_ramp_freq_sel_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_42_b20_ramp_freq_sel_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_42_b20_ramp_force_eco_test_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_42_b20_ramp_force_eco_test_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_42_b20_ramp_fast_sel_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_42_b20_ramp_fast_sel_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_42_b20_ramp_eco_buffer_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_42_b20_ramp_eco_buffer_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_42_b20_ramp_dblclk_sel_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_42_b20_ramp_dblclk_sel_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_42_b20_ramp_counter_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_42_b20_ramp_counter_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_ramp_r_ccm : 4;
        unsigned char b20_ramp_mid_sel : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_43_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_43_b20_ramp_r_ccm_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_43_b20_ramp_r_ccm_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_43_b20_ramp_mid_sel_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_43_b20_ramp_mid_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_ramp_r_eco : 4;
        unsigned char b20_ramp_r_cot : 4;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_44_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_44_b20_ramp_r_eco_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_44_b20_ramp_r_eco_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_44_b20_ramp_r_cot_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_44_b20_ramp_r_cot_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_ramptrim_sel : 1;
        unsigned char b20_ramp_valley_sel : 1;
        unsigned char b20_ramp_trim_sel : 1;
        unsigned char b20_ramp_sw_sel : 1;
        unsigned char b20_ramp_sel : 1;
        unsigned char b20_ramp_res_test : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_45_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_45_b20_ramptrim_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_45_b20_ramptrim_sel_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_45_b20_ramp_valley_sel_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_45_b20_ramp_valley_sel_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_45_b20_ramp_trim_sel_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_45_b20_ramp_trim_sel_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_45_b20_ramp_sw_sel_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_45_b20_ramp_sw_sel_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_45_b20_ramp_sel_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_45_b20_ramp_sel_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_45_b20_ramp_res_test_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_45_b20_ramp_res_test_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_reg_idrp : 3;
        unsigned char b20_reg_idrn : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_46_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_46_b20_reg_idrp_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_46_b20_reg_idrp_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_46_b20_reg_idrn_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_46_b20_reg_idrn_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_reg_sense_res_sel : 3;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_47_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_47_b20_reg_sense_res_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_47_b20_reg_sense_res_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_reserve0 : 8;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_48_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_48_b20_reserve0_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_48_b20_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_reserve1 : 8;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_49_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_49_b20_reserve1_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_49_b20_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_reserve2 : 8;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_50_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_50_b20_reserve2_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_50_b20_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_reserve3 : 8;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_51_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_51_b20_reserve3_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_51_b20_reserve3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_sft : 1;
        unsigned char b20_sense_ratio_sel : 2;
        unsigned char b20_rtr_cap_sel : 2;
        unsigned char b20_ron_test_sel : 3;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_52_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_52_b20_sft_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_52_b20_sft_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_52_b20_sense_ratio_sel_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_52_b20_sense_ratio_sel_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_52_b20_rtr_cap_sel_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_52_b20_rtr_cap_sel_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_52_b20_ron_test_sel_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_52_b20_ron_test_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_stsw_atest : 1;
        unsigned char b20_state_sw_ocp_sel : 2;
        unsigned char b20_state_sw_ocp_off : 1;
        unsigned char b20_slave_sel : 1;
        unsigned char b20_short_shield : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_53_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_53_b20_stsw_atest_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_53_b20_stsw_atest_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_53_b20_state_sw_ocp_sel_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_53_b20_state_sw_ocp_sel_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_53_b20_state_sw_ocp_off_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_53_b20_state_sw_ocp_off_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_53_b20_slave_sel_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_53_b20_slave_sel_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_53_b20_short_shield_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_53_b20_short_shield_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_stsw_fstsch_eco : 3;
        unsigned char b20_stsw_fstps_ith : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_54_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_54_b20_stsw_fstsch_eco_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_54_b20_stsw_fstsch_eco_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_54_b20_stsw_fstps_ith_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_54_b20_stsw_fstps_ith_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_stsw_slw_ct2cm_hys : 2;
        unsigned char b20_stsw_rpup_ccm : 1;
        unsigned char b20_stsw_man_set : 2;
        unsigned char b20_stsw_fstsch_neco : 3;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_55_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_55_b20_stsw_slw_ct2cm_hys_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_55_b20_stsw_slw_ct2cm_hys_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_55_b20_stsw_rpup_ccm_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_55_b20_stsw_rpup_ccm_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_55_b20_stsw_man_set_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_55_b20_stsw_man_set_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_55_b20_stsw_fstsch_neco_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_55_b20_stsw_fstsch_neco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_stsw_slw_hys_shield : 1;
        unsigned char b20_stsw_slw_delay_l : 2;
        unsigned char b20_stsw_slw_delay_h : 2;
        unsigned char b20_stsw_slw_ct2cm_ith : 3;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_56_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_56_b20_stsw_slw_hys_shield_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_56_b20_stsw_slw_hys_shield_END (0)
#define SUB_PMIC_BUCK20_CFG_REG_56_b20_stsw_slw_delay_l_START (1)
#define SUB_PMIC_BUCK20_CFG_REG_56_b20_stsw_slw_delay_l_END (2)
#define SUB_PMIC_BUCK20_CFG_REG_56_b20_stsw_slw_delay_h_START (3)
#define SUB_PMIC_BUCK20_CFG_REG_56_b20_stsw_slw_delay_h_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_56_b20_stsw_slw_ct2cm_ith_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_56_b20_stsw_slw_ct2cm_ith_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_ton_det : 4;
        unsigned char b20_stsw_slwsch_neco : 2;
        unsigned char b20_stsw_slwsch_eco : 2;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_57_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_57_b20_ton_det_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_57_b20_ton_det_END (3)
#define SUB_PMIC_BUCK20_CFG_REG_57_b20_stsw_slwsch_neco_START (4)
#define SUB_PMIC_BUCK20_CFG_REG_57_b20_stsw_slwsch_neco_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_57_b20_stsw_slwsch_eco_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_57_b20_stsw_slwsch_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_vref_test_sel : 2;
        unsigned char b20_vo_ton_res_sel : 3;
        unsigned char b20_trim2 : 1;
        unsigned char b20_trim1 : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_58_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_58_b20_vref_test_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_58_b20_vref_test_sel_END (1)
#define SUB_PMIC_BUCK20_CFG_REG_58_b20_vo_ton_res_sel_START (2)
#define SUB_PMIC_BUCK20_CFG_REG_58_b20_vo_ton_res_sel_END (4)
#define SUB_PMIC_BUCK20_CFG_REG_58_b20_trim2_START (5)
#define SUB_PMIC_BUCK20_CFG_REG_58_b20_trim2_END (5)
#define SUB_PMIC_BUCK20_CFG_REG_58_b20_trim1_START (6)
#define SUB_PMIC_BUCK20_CFG_REG_58_b20_trim1_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_vref_vo_cap_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK20_CFG_REG_59_UNION;
#endif
#define SUB_PMIC_BUCK20_CFG_REG_59_b20_vref_vo_cap_sel_START (0)
#define SUB_PMIC_BUCK20_CFG_REG_59_b20_vref_vo_cap_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b20_state_a2d : 2;
        unsigned char b20_pg : 1;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK20_RO_REG_60_UNION;
#endif
#define SUB_PMIC_BUCK20_RO_REG_60_b20_state_a2d_START (0)
#define SUB_PMIC_BUCK20_RO_REG_60_b20_state_a2d_END (1)
#define SUB_PMIC_BUCK20_RO_REG_60_b20_pg_START (2)
#define SUB_PMIC_BUCK20_RO_REG_60_b20_pg_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b21_cot_atest : 1;
        unsigned char b21_cmp_atest : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK21_CFG_REG_0_UNION;
#endif
#define SUB_PMIC_BUCK21_CFG_REG_0_b21_cot_atest_START (0)
#define SUB_PMIC_BUCK21_CFG_REG_0_b21_cot_atest_END (0)
#define SUB_PMIC_BUCK21_CFG_REG_0_b21_cmp_atest_START (1)
#define SUB_PMIC_BUCK21_CFG_REG_0_b21_cmp_atest_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b21_cot_ton_ecoa : 8;
    } reg;
} SUB_PMIC_BUCK21_CFG_REG_1_UNION;
#endif
#define SUB_PMIC_BUCK21_CFG_REG_1_b21_cot_ton_ecoa_START (0)
#define SUB_PMIC_BUCK21_CFG_REG_1_b21_cot_ton_ecoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b21_cot_ton_ecob : 3;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK21_CFG_REG_2_UNION;
#endif
#define SUB_PMIC_BUCK21_CFG_REG_2_b21_cot_ton_ecob_START (0)
#define SUB_PMIC_BUCK21_CFG_REG_2_b21_cot_ton_ecob_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b21_cot_ton_necoa : 8;
    } reg;
} SUB_PMIC_BUCK21_CFG_REG_3_UNION;
#endif
#define SUB_PMIC_BUCK21_CFG_REG_3_b21_cot_ton_necoa_START (0)
#define SUB_PMIC_BUCK21_CFG_REG_3_b21_cot_ton_necoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b21_dmd_man_set : 4;
        unsigned char b21_dmd_atest : 1;
        unsigned char b21_cot_ton_necob : 3;
    } reg;
} SUB_PMIC_BUCK21_CFG_REG_4_UNION;
#endif
#define SUB_PMIC_BUCK21_CFG_REG_4_b21_dmd_man_set_START (0)
#define SUB_PMIC_BUCK21_CFG_REG_4_b21_dmd_man_set_END (3)
#define SUB_PMIC_BUCK21_CFG_REG_4_b21_dmd_atest_START (4)
#define SUB_PMIC_BUCK21_CFG_REG_4_b21_dmd_atest_END (4)
#define SUB_PMIC_BUCK21_CFG_REG_4_b21_cot_ton_necob_START (5)
#define SUB_PMIC_BUCK21_CFG_REG_4_b21_cot_ton_necob_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b21_drv_sw_sel : 1;
        unsigned char b21_drv_dt_sel : 4;
        unsigned char b21_dmd_ngc_set : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK21_CFG_REG_5_UNION;
#endif
#define SUB_PMIC_BUCK21_CFG_REG_5_b21_drv_sw_sel_START (0)
#define SUB_PMIC_BUCK21_CFG_REG_5_b21_drv_sw_sel_END (0)
#define SUB_PMIC_BUCK21_CFG_REG_5_b21_drv_dt_sel_START (1)
#define SUB_PMIC_BUCK21_CFG_REG_5_b21_drv_dt_sel_END (4)
#define SUB_PMIC_BUCK21_CFG_REG_5_b21_dmd_ngc_set_START (5)
#define SUB_PMIC_BUCK21_CFG_REG_5_b21_dmd_ngc_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b21_dt_sel : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK21_CFG_REG_6_UNION;
#endif
#define SUB_PMIC_BUCK21_CFG_REG_6_b21_dt_sel_START (0)
#define SUB_PMIC_BUCK21_CFG_REG_6_b21_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b21_ng_n_sel : 3;
        unsigned char b21_ng_dt_sel : 3;
        unsigned char b21_n_sel : 2;
    } reg;
} SUB_PMIC_BUCK21_CFG_REG_7_UNION;
#endif
#define SUB_PMIC_BUCK21_CFG_REG_7_b21_ng_n_sel_START (0)
#define SUB_PMIC_BUCK21_CFG_REG_7_b21_ng_n_sel_END (2)
#define SUB_PMIC_BUCK21_CFG_REG_7_b21_ng_dt_sel_START (3)
#define SUB_PMIC_BUCK21_CFG_REG_7_b21_ng_dt_sel_END (5)
#define SUB_PMIC_BUCK21_CFG_REG_7_b21_n_sel_START (6)
#define SUB_PMIC_BUCK21_CFG_REG_7_b21_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b21_pg_dt_sel : 1;
        unsigned char b21_p_sel : 2;
        unsigned char b21_npocp_atest : 1;
        unsigned char b21_ng_p_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK21_CFG_REG_8_UNION;
#endif
#define SUB_PMIC_BUCK21_CFG_REG_8_b21_pg_dt_sel_START (0)
#define SUB_PMIC_BUCK21_CFG_REG_8_b21_pg_dt_sel_END (0)
#define SUB_PMIC_BUCK21_CFG_REG_8_b21_p_sel_START (1)
#define SUB_PMIC_BUCK21_CFG_REG_8_b21_p_sel_END (2)
#define SUB_PMIC_BUCK21_CFG_REG_8_b21_npocp_atest_START (3)
#define SUB_PMIC_BUCK21_CFG_REG_8_b21_npocp_atest_END (3)
#define SUB_PMIC_BUCK21_CFG_REG_8_b21_ng_p_sel_START (4)
#define SUB_PMIC_BUCK21_CFG_REG_8_b21_ng_p_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b21_ramp_buff_trim_test : 1;
        unsigned char b21_pocp_sw_atest : 1;
        unsigned char b21_pg_p_sel : 3;
        unsigned char b21_pg_n_sel : 3;
    } reg;
} SUB_PMIC_BUCK21_CFG_REG_9_UNION;
#endif
#define SUB_PMIC_BUCK21_CFG_REG_9_b21_ramp_buff_trim_test_START (0)
#define SUB_PMIC_BUCK21_CFG_REG_9_b21_ramp_buff_trim_test_END (0)
#define SUB_PMIC_BUCK21_CFG_REG_9_b21_pocp_sw_atest_START (1)
#define SUB_PMIC_BUCK21_CFG_REG_9_b21_pocp_sw_atest_END (1)
#define SUB_PMIC_BUCK21_CFG_REG_9_b21_pg_p_sel_START (2)
#define SUB_PMIC_BUCK21_CFG_REG_9_b21_pg_p_sel_END (4)
#define SUB_PMIC_BUCK21_CFG_REG_9_b21_pg_n_sel_START (5)
#define SUB_PMIC_BUCK21_CFG_REG_9_b21_pg_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b21_ron_test_sel : 3;
        unsigned char b21_ramp_res_test : 1;
        unsigned char b21_ramp_r_ccm : 4;
    } reg;
} SUB_PMIC_BUCK21_CFG_REG_10_UNION;
#endif
#define SUB_PMIC_BUCK21_CFG_REG_10_b21_ron_test_sel_START (0)
#define SUB_PMIC_BUCK21_CFG_REG_10_b21_ron_test_sel_END (2)
#define SUB_PMIC_BUCK21_CFG_REG_10_b21_ramp_res_test_START (3)
#define SUB_PMIC_BUCK21_CFG_REG_10_b21_ramp_res_test_END (3)
#define SUB_PMIC_BUCK21_CFG_REG_10_b21_ramp_r_ccm_START (4)
#define SUB_PMIC_BUCK21_CFG_REG_10_b21_ramp_r_ccm_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck12_en : 1;
        unsigned char st_buck12_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck12_eco_en : 1;
        unsigned char st_buck12_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK12_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_BUCK12_ONOFF_ECO_reg_buck12_en_START (0)
#define SUB_PMIC_BUCK12_ONOFF_ECO_reg_buck12_en_END (0)
#define SUB_PMIC_BUCK12_ONOFF_ECO_st_buck12_en_START (1)
#define SUB_PMIC_BUCK12_ONOFF_ECO_st_buck12_en_END (1)
#define SUB_PMIC_BUCK12_ONOFF_ECO_reg_buck12_eco_en_START (4)
#define SUB_PMIC_BUCK12_ONOFF_ECO_reg_buck12_eco_en_END (4)
#define SUB_PMIC_BUCK12_ONOFF_ECO_st_buck12_eco_en_START (5)
#define SUB_PMIC_BUCK12_ONOFF_ECO_st_buck12_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck12_vset_cfg : 8;
    } reg;
} SUB_PMIC_BUCK12_VSET_UNION;
#endif
#define SUB_PMIC_BUCK12_VSET_buck12_vset_cfg_START (0)
#define SUB_PMIC_BUCK12_VSET_buck12_vset_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck12_rampdown_ctrl : 3;
        unsigned char buck12_rampup_ctrl : 3;
        unsigned char buck12_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_RAMP_BUCK12_CTRL0_UNION;
#endif
#define SUB_PMIC_RAMP_BUCK12_CTRL0_buck12_rampdown_ctrl_START (0)
#define SUB_PMIC_RAMP_BUCK12_CTRL0_buck12_rampdown_ctrl_END (2)
#define SUB_PMIC_RAMP_BUCK12_CTRL0_buck12_rampup_ctrl_START (3)
#define SUB_PMIC_RAMP_BUCK12_CTRL0_buck12_rampup_ctrl_END (5)
#define SUB_PMIC_RAMP_BUCK12_CTRL0_buck12_ramp_en_cfg_START (6)
#define SUB_PMIC_RAMP_BUCK12_CTRL0_buck12_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck12_pull_down_off_cfg : 3;
        unsigned char buck12_pull_down_on_cfg : 3;
        unsigned char buck12_pull_down_cfg : 1;
        unsigned char buck12_pull_down_mode : 1;
    } reg;
} SUB_PMIC_RAMP_BUCK12_CTRL1_UNION;
#endif
#define SUB_PMIC_RAMP_BUCK12_CTRL1_buck12_pull_down_off_cfg_START (0)
#define SUB_PMIC_RAMP_BUCK12_CTRL1_buck12_pull_down_off_cfg_END (2)
#define SUB_PMIC_RAMP_BUCK12_CTRL1_buck12_pull_down_on_cfg_START (3)
#define SUB_PMIC_RAMP_BUCK12_CTRL1_buck12_pull_down_on_cfg_END (5)
#define SUB_PMIC_RAMP_BUCK12_CTRL1_buck12_pull_down_cfg_START (6)
#define SUB_PMIC_RAMP_BUCK12_CTRL1_buck12_pull_down_cfg_END (6)
#define SUB_PMIC_RAMP_BUCK12_CTRL1_buck12_pull_down_mode_START (7)
#define SUB_PMIC_RAMP_BUCK12_CTRL1_buck12_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck12_dly_ru_off_cfg : 3;
        unsigned char buck12_dly_ru_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK12_RAMPUP_STATE_UNION;
#endif
#define SUB_PMIC_BUCK12_RAMPUP_STATE_buck12_dly_ru_off_cfg_START (0)
#define SUB_PMIC_BUCK12_RAMPUP_STATE_buck12_dly_ru_off_cfg_END (2)
#define SUB_PMIC_BUCK12_RAMPUP_STATE_buck12_dly_ru_on_cfg_START (3)
#define SUB_PMIC_BUCK12_RAMPUP_STATE_buck12_dly_ru_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck12_dly_rd_pd_off_cfg : 3;
        unsigned char buck12_dly_rd_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK12_RAMPDOWN_STATE_UNION;
#endif
#define SUB_PMIC_BUCK12_RAMPDOWN_STATE_buck12_dly_rd_pd_off_cfg_START (0)
#define SUB_PMIC_BUCK12_RAMPDOWN_STATE_buck12_dly_rd_pd_off_cfg_END (2)
#define SUB_PMIC_BUCK12_RAMPDOWN_STATE_buck12_dly_rd_on_cfg_START (3)
#define SUB_PMIC_BUCK12_RAMPDOWN_STATE_buck12_dly_rd_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck12_rampdown_chg_rate : 2;
        unsigned char reserved_0 : 2;
        unsigned char buck12_rampup_chg_rate : 2;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_BUCK12_RAMP_CHG_RATE_UNION;
#endif
#define SUB_PMIC_BUCK12_RAMP_CHG_RATE_buck12_rampdown_chg_rate_START (0)
#define SUB_PMIC_BUCK12_RAMP_CHG_RATE_buck12_rampdown_chg_rate_END (1)
#define SUB_PMIC_BUCK12_RAMP_CHG_RATE_buck12_rampup_chg_rate_START (4)
#define SUB_PMIC_BUCK12_RAMP_CHG_RATE_buck12_rampup_chg_rate_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck12_rampdown_chg_vset : 3;
        unsigned char reserved_0 : 1;
        unsigned char buck12_rampup_chg_vset : 3;
        unsigned char reserved_1 : 1;
    } reg;
} SUB_PMIC_BUCK12_RAMP_CHG_VSET_UNION;
#endif
#define SUB_PMIC_BUCK12_RAMP_CHG_VSET_buck12_rampdown_chg_vset_START (0)
#define SUB_PMIC_BUCK12_RAMP_CHG_VSET_buck12_rampdown_chg_vset_END (2)
#define SUB_PMIC_BUCK12_RAMP_CHG_VSET_buck12_rampup_chg_vset_START (4)
#define SUB_PMIC_BUCK12_RAMP_CHG_VSET_buck12_rampup_chg_vset_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck12_rd_nochg_rate_sel : 1;
        unsigned char reserved_0 : 3;
        unsigned char buck12_ru_nochg_rate_sel : 1;
        unsigned char reserved_1 : 3;
    } reg;
} SUB_PMIC_BUCK12_CHG_RATE_SEL_UNION;
#endif
#define SUB_PMIC_BUCK12_CHG_RATE_SEL_buck12_rd_nochg_rate_sel_START (0)
#define SUB_PMIC_BUCK12_CHG_RATE_SEL_buck12_rd_nochg_rate_sel_END (0)
#define SUB_PMIC_BUCK12_CHG_RATE_SEL_buck12_ru_nochg_rate_sel_START (4)
#define SUB_PMIC_BUCK12_CHG_RATE_SEL_buck12_ru_nochg_rate_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck12_ramp_state : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_BUCK12_RAMP_STATE_UNION;
#endif
#define SUB_PMIC_BUCK12_RAMP_STATE_buck12_ramp_state_START (0)
#define SUB_PMIC_BUCK12_RAMP_STATE_buck12_ramp_state_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_bw_sel : 1;
        unsigned char b12_buffer_bias : 2;
        unsigned char b12_b12_ea_r : 3;
        unsigned char b12_avg_curr_filter_sel : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_0_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_0_b12_bw_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_0_b12_bw_sel_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_0_b12_buffer_bias_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_0_b12_buffer_bias_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_0_b12_b12_ea_r_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_0_b12_b12_ea_r_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_0_b12_avg_curr_filter_sel_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_0_b12_avg_curr_filter_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_c1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_1_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_1_b12_c1_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_1_b12_c1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_c2_sel : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_2_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_2_b12_c2_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_2_b12_c2_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_clk_chopper_sel : 2;
        unsigned char b12_c3_sel : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_3_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_3_b12_clk_chopper_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_3_b12_clk_chopper_sel_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_3_b12_c3_sel_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_3_b12_c3_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_clock_en_4p : 1;
        unsigned char b12_clock_atest : 1;
        unsigned char b12_clk9m6_pulse_sel : 2;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_4_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_4_b12_clock_en_4p_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_4_b12_clock_en_4p_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_4_b12_clock_atest_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_4_b12_clock_atest_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_4_b12_clk9m6_pulse_sel_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_4_b12_clk9m6_pulse_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_clp_ref_sel : 1;
        unsigned char b12_clock_sel : 7;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_5_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_5_b12_clp_ref_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_5_b12_clp_ref_sel_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_5_b12_clock_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_5_b12_clock_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_clpeco_bias : 4;
        unsigned char b12_clp_reg_idrp : 4;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_6_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_6_b12_clpeco_bias_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_6_b12_clpeco_bias_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_6_b12_clp_reg_idrp_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_6_b12_clp_reg_idrp_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_cot_res_sel : 3;
        unsigned char b12_cot_iboost_sel : 1;
        unsigned char b12_cot_cmphys : 1;
        unsigned char b12_cot_atest : 1;
        unsigned char b12_code_mode_sel : 1;
        unsigned char b12_cmp_atest : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_7_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_7_b12_cot_res_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_7_b12_cot_res_sel_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_7_b12_cot_iboost_sel_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_7_b12_cot_iboost_sel_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_7_b12_cot_cmphys_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_7_b12_cot_cmphys_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_7_b12_cot_atest_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_7_b12_cot_atest_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_7_b12_code_mode_sel_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_7_b12_code_mode_sel_END (6)
#define SUB_PMIC_BUCK12_CFG_REG_7_b12_cmp_atest_START (7)
#define SUB_PMIC_BUCK12_CFG_REG_7_b12_cmp_atest_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_cot_sel : 1;
        unsigned char b12_cot_sche_neco : 1;
        unsigned char b12_cot_sche_eco : 1;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_8_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_8_b12_cot_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_8_b12_cot_sel_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_8_b12_cot_sche_neco_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_8_b12_cot_sche_neco_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_8_b12_cot_sche_eco_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_8_b12_cot_sche_eco_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_cot_ton_ecoa : 8;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_9_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_9_b12_cot_ton_ecoa_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_9_b12_cot_ton_ecoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_cot_ton_isen_hys : 2;
        unsigned char b12_cot_ton_isen_bias : 2;
        unsigned char b12_cot_ton_ecob : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_10_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_10_b12_cot_ton_isen_hys_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_10_b12_cot_ton_isen_hys_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_10_b12_cot_ton_isen_bias_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_10_b12_cot_ton_isen_bias_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_10_b12_cot_ton_ecob_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_10_b12_cot_ton_ecob_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_cot_ton_necoa : 8;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_11_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_11_b12_cot_ton_necoa_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_11_b12_cot_ton_necoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_ctlogic_sgph : 1;
        unsigned char b12_ctlogic_modsw : 1;
        unsigned char b12_ctlogic_crson_td : 2;
        unsigned char b12_cs_test_sel : 1;
        unsigned char b12_cot_ton_necob : 3;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_12_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_12_b12_ctlogic_sgph_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_12_b12_ctlogic_sgph_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_12_b12_ctlogic_modsw_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_12_b12_ctlogic_modsw_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_12_b12_ctlogic_crson_td_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_12_b12_ctlogic_crson_td_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_12_b12_cs_test_sel_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_12_b12_cs_test_sel_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_12_b12_cot_ton_necob_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_12_b12_cot_ton_necob_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_dmd_atest : 1;
        unsigned char b12_dbias_eco : 3;
        unsigned char b12_dbias : 3;
        unsigned char b12_ctlogic_smph : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_13_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_13_b12_dmd_atest_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_13_b12_dmd_atest_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_13_b12_dbias_eco_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_13_b12_dbias_eco_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_13_b12_dbias_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_13_b12_dbias_END (6)
#define SUB_PMIC_BUCK12_CFG_REG_13_b12_ctlogic_smph_START (7)
#define SUB_PMIC_BUCK12_CFG_REG_13_b12_ctlogic_smph_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_dmd_auto_vbc : 1;
        unsigned char b12_dmd_auto_sel : 1;
        unsigned char b12_dmd_auto_sampt : 2;
        unsigned char b12_dmd_auto_eco : 1;
        unsigned char b12_dmd_auto_cmpib : 2;
        unsigned char b12_dmd_auto_ccm : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_14_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_14_b12_dmd_auto_vbc_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_14_b12_dmd_auto_vbc_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_14_b12_dmd_auto_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_14_b12_dmd_auto_sel_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_14_b12_dmd_auto_sampt_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_14_b12_dmd_auto_sampt_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_14_b12_dmd_auto_eco_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_14_b12_dmd_auto_eco_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_14_b12_dmd_auto_cmpib_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_14_b12_dmd_auto_cmpib_END (6)
#define SUB_PMIC_BUCK12_CFG_REG_14_b12_dmd_auto_ccm_START (7)
#define SUB_PMIC_BUCK12_CFG_REG_14_b12_dmd_auto_ccm_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_dmd_eco_sel : 1;
        unsigned char b12_dmd_clp_sel : 1;
        unsigned char b12_dmd_bck_sel : 1;
        unsigned char b12_dmd_auto_vos : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_15_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_15_b12_dmd_eco_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_15_b12_dmd_eco_sel_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_15_b12_dmd_clp_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_15_b12_dmd_clp_sel_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_15_b12_dmd_bck_sel_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_15_b12_dmd_bck_sel_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_15_b12_dmd_auto_vos_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_15_b12_dmd_auto_vos_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_dmd_ngc_set : 2;
        unsigned char b12_dmd_ngc_ramp : 1;
        unsigned char b12_dmd_ngc_disch : 1;
        unsigned char b12_dmd_man_set : 4;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_16_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_16_b12_dmd_ngc_set_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_16_b12_dmd_ngc_set_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_16_b12_dmd_ngc_ramp_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_16_b12_dmd_ngc_ramp_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_16_b12_dmd_ngc_disch_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_16_b12_dmd_ngc_disch_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_16_b12_dmd_man_set_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_16_b12_dmd_man_set_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_drv_sw_sel : 1;
        unsigned char b12_drv_dt_sel : 4;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_17_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_17_b12_drv_sw_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_17_b12_drv_sw_sel_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_17_b12_drv_dt_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_17_b12_drv_dt_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_dt_sel : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_18_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_18_b12_dt_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_18_b12_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_eabias : 1;
        unsigned char b12_eaamp_clp_sel : 1;
        unsigned char b12_eaamp_clp : 2;
        unsigned char b12_ea_ecocur_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_19_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_19_b12_eabias_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_19_b12_eabias_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_19_b12_eaamp_clp_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_19_b12_eaamp_clp_sel_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_19_b12_eaamp_clp_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_19_b12_eaamp_clp_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_19_b12_ea_ecocur_sel_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_19_b12_ea_ecocur_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_eaclp_ecogap_sel : 2;
        unsigned char b12_eaclp_brgap_sel : 4;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_20_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_20_b12_eaclp_ecogap_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_20_b12_eaclp_ecogap_sel_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_20_b12_eaclp_brgap_sel_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_20_b12_eaclp_brgap_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_eacomp_eco_shield : 1;
        unsigned char b12_eaclp_srgap_sel : 4;
        unsigned char b12_eaclp_gap_sel : 3;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_21_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_21_b12_eacomp_eco_shield_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_21_b12_eacomp_eco_shield_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_21_b12_eaclp_srgap_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_21_b12_eaclp_srgap_sel_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_21_b12_eaclp_gap_sel_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_21_b12_eaclp_gap_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_ecdt_ct2cm_ref : 3;
        unsigned char b12_ecdt_ct2cm_ft : 2;
        unsigned char b12_ecdt_cmphys : 1;
        unsigned char b12_eatrim_en : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_22_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_22_b12_ecdt_ct2cm_ref_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_22_b12_ecdt_ct2cm_ref_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_22_b12_ecdt_ct2cm_ft_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_22_b12_ecdt_ct2cm_ft_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_22_b12_ecdt_cmphys_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_22_b12_ecdt_cmphys_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_22_b12_eatrim_en_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_22_b12_eatrim_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_ecdt_ec2ct_ref : 3;
        unsigned char b12_ecdt_ct2ec_ref : 3;
        unsigned char b12_ecdt_ct2ec_ft : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_23_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_23_b12_ecdt_ec2ct_ref_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_23_b12_ecdt_ec2ct_ref_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_23_b12_ecdt_ct2ec_ref_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_23_b12_ecdt_ct2ec_ref_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_23_b12_ecdt_ct2ec_ft_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_23_b12_ecdt_ct2ec_ft_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_en_psns : 1;
        unsigned char b12_en_nsense : 1;
        unsigned char b12_en_chopper : 1;
        unsigned char b12_ecotrim_shield : 1;
        unsigned char b12_eco_shield_cmp : 1;
        unsigned char b12_eco_shield : 1;
        unsigned char b12_ecdt_filter_rc : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_24_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_24_b12_en_psns_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_24_b12_en_psns_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_24_b12_en_nsense_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_24_b12_en_nsense_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_24_b12_en_chopper_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_24_b12_en_chopper_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_24_b12_ecotrim_shield_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_24_b12_ecotrim_shield_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_24_b12_eco_shield_cmp_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_24_b12_eco_shield_cmp_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_24_b12_eco_shield_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_24_b12_eco_shield_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_24_b12_ecdt_filter_rc_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_24_b12_ecdt_filter_rc_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_ldline_forced_on : 1;
        unsigned char b12_ibalance_gm_sel : 2;
        unsigned char b12_forcenormal : 1;
        unsigned char b12_forceeco : 1;
        unsigned char b12_fastnmos_off : 1;
        unsigned char b12_en_vo : 1;
        unsigned char b12_en_test : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_25_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_25_b12_ldline_forced_on_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_25_b12_ldline_forced_on_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_25_b12_ibalance_gm_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_25_b12_ibalance_gm_sel_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_25_b12_forcenormal_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_25_b12_forcenormal_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_25_b12_forceeco_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_25_b12_forceeco_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_25_b12_fastnmos_off_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_25_b12_fastnmos_off_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_25_b12_en_vo_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_25_b12_en_vo_END (6)
#define SUB_PMIC_BUCK12_CFG_REG_25_b12_en_test_START (7)
#define SUB_PMIC_BUCK12_CFG_REG_25_b12_en_test_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_load_det_sel : 2;
        unsigned char b12_ldline_mohm_sel : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_26_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_26_b12_load_det_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_26_b12_load_det_sel_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_26_b12_ldline_mohm_sel_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_26_b12_ldline_mohm_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_min_ton_sel : 2;
        unsigned char b12_min_toff_sel : 2;
        unsigned char b12_load_state_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_27_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_27_b12_min_ton_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_27_b12_min_ton_sel_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_27_b12_min_toff_sel_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_27_b12_min_toff_sel_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_27_b12_load_state_sel_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_27_b12_load_state_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_ng_dt_sel : 3;
        unsigned char b12_negdmdton_add : 1;
        unsigned char b12_negdmd_ton_sel : 2;
        unsigned char b12_n_sel : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_28_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_28_b12_ng_dt_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_28_b12_ng_dt_sel_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_28_b12_negdmdton_add_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_28_b12_negdmdton_add_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_28_b12_negdmd_ton_sel_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_28_b12_negdmd_ton_sel_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_28_b12_n_sel_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_28_b12_n_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_nmos_off : 1;
        unsigned char b12_ng_p_sel : 3;
        unsigned char b12_ng_n_sel : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_29_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_29_b12_nmos_off_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_29_b12_nmos_off_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_29_b12_ng_p_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_29_b12_ng_p_sel_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_29_b12_ng_n_sel_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_29_b12_ng_n_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_nocp_low_sel : 2;
        unsigned char b12_nocp_low_eco_sel : 1;
        unsigned char b12_nocp_low_delay_sel : 2;
        unsigned char b12_nocp_en : 1;
        unsigned char b12_nocp_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_30_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_30_b12_nocp_low_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_30_b12_nocp_low_sel_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_30_b12_nocp_low_eco_sel_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_30_b12_nocp_low_eco_sel_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_30_b12_nocp_low_delay_sel_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_30_b12_nocp_low_delay_sel_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_30_b12_nocp_en_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_30_b12_nocp_en_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_30_b12_nocp_delay_sel_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_30_b12_nocp_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_ocp_2us_delay_sel : 1;
        unsigned char b12_nsense_sel : 3;
        unsigned char b12_npocp_atest : 1;
        unsigned char b12_normaltrim_shield : 1;
        unsigned char b12_nocp_sel : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_31_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_31_b12_ocp_2us_delay_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_31_b12_ocp_2us_delay_sel_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_31_b12_nsense_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_31_b12_nsense_sel_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_31_b12_npocp_atest_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_31_b12_npocp_atest_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_31_b12_normaltrim_shield_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_31_b12_normaltrim_shield_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_31_b12_nocp_sel_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_31_b12_nocp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_p_sel : 2;
        unsigned char b12_ovp_shield : 1;
        unsigned char b12_ovp_sel : 2;
        unsigned char b12_oneph : 1;
        unsigned char b12_ocp_short_en : 1;
        unsigned char b12_ocp_shield : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_32_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_32_b12_p_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_32_b12_p_sel_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_32_b12_ovp_shield_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_32_b12_ovp_shield_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_32_b12_ovp_sel_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_32_b12_ovp_sel_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_32_b12_oneph_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_32_b12_oneph_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_32_b12_ocp_short_en_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_32_b12_ocp_short_en_END (6)
#define SUB_PMIC_BUCK12_CFG_REG_32_b12_ocp_shield_START (7)
#define SUB_PMIC_BUCK12_CFG_REG_32_b12_ocp_shield_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_pg_p_sel : 3;
        unsigned char b12_pg_n_sel : 3;
        unsigned char b12_pg_dt_sel : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_33_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_33_b12_pg_p_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_33_b12_pg_p_sel_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_33_b12_pg_n_sel_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_33_b12_pg_n_sel_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_33_b12_pg_dt_sel_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_33_b12_pg_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_pocp_sw_ccm_cmp : 1;
        unsigned char b12_pocp_sw_atest : 1;
        unsigned char b12_pocp_sel : 2;
        unsigned char b12_pocp_en : 1;
        unsigned char b12_pocp_eco_sel : 1;
        unsigned char b12_pocp_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_34_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_34_b12_pocp_sw_ccm_cmp_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_34_b12_pocp_sw_ccm_cmp_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_34_b12_pocp_sw_atest_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_34_b12_pocp_sw_atest_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_34_b12_pocp_sel_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_34_b12_pocp_sel_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_34_b12_pocp_en_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_34_b12_pocp_en_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_34_b12_pocp_eco_sel_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_34_b12_pocp_eco_sel_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_34_b12_pocp_delay_sel_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_34_b12_pocp_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_pulldn_pd_sel : 1;
        unsigned char b12_pulldn_pd_en : 1;
        unsigned char b12_pocp_sw_sel : 3;
        unsigned char b12_pocp_sw_eco_cmp : 1;
        unsigned char b12_pocp_sw_delay_sel : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_35_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_35_b12_pulldn_pd_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_35_b12_pulldn_pd_sel_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_35_b12_pulldn_pd_en_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_35_b12_pulldn_pd_en_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_35_b12_pocp_sw_sel_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_35_b12_pocp_sw_sel_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_35_b12_pocp_sw_eco_cmp_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_35_b12_pocp_sw_eco_cmp_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_35_b12_pocp_sw_delay_sel_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_35_b12_pocp_sw_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_r1_sel : 4;
        unsigned char b12_pvdd_ton_sel : 1;
        unsigned char b12_pulldn_sel : 3;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_36_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_36_b12_r1_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_36_b12_r1_sel_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_36_b12_pvdd_ton_sel_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_36_b12_pvdd_ton_sel_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_36_b12_pulldn_sel_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_36_b12_pulldn_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_r2_sel : 6;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_37_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_37_b12_r2_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_37_b12_r2_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_ramp_big_sel : 1;
        unsigned char b12_r3_sel : 6;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_38_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_38_b12_ramp_big_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_38_b12_ramp_big_sel_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_38_b12_r3_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_38_b12_r3_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_ramp_ccm_outsel : 1;
        unsigned char b12_ramp_c : 2;
        unsigned char b12_ramp_buff_trim_test : 1;
        unsigned char b12_ramp_buff_trim : 1;
        unsigned char b12_ramp_buff_bias : 2;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_39_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_39_b12_ramp_ccm_outsel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_39_b12_ramp_ccm_outsel_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_39_b12_ramp_c_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_39_b12_ramp_c_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_39_b12_ramp_buff_trim_test_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_39_b12_ramp_buff_trim_test_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_39_b12_ramp_buff_trim_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_39_b12_ramp_buff_trim_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_39_b12_ramp_buff_bias_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_39_b12_ramp_buff_bias_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_ramp_cot_outsel : 1;
        unsigned char b12_ramp_clk_sel : 1;
        unsigned char b12_ramp_change_mode : 5;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_40_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_40_b12_ramp_cot_outsel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_40_b12_ramp_cot_outsel_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_40_b12_ramp_clk_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_40_b12_ramp_clk_sel_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_40_b12_ramp_change_mode_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_40_b12_ramp_change_mode_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_ramp_line_sel : 1;
        unsigned char b12_ramp_freq_sel : 1;
        unsigned char b12_ramp_force_eco_test : 1;
        unsigned char b12_ramp_fast_sel : 1;
        unsigned char b12_ramp_eco_buffer : 1;
        unsigned char b12_ramp_dblclk_sel : 1;
        unsigned char b12_ramp_counter : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_41_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_41_b12_ramp_line_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_41_b12_ramp_line_sel_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_41_b12_ramp_freq_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_41_b12_ramp_freq_sel_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_41_b12_ramp_force_eco_test_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_41_b12_ramp_force_eco_test_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_41_b12_ramp_fast_sel_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_41_b12_ramp_fast_sel_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_41_b12_ramp_eco_buffer_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_41_b12_ramp_eco_buffer_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_41_b12_ramp_dblclk_sel_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_41_b12_ramp_dblclk_sel_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_41_b12_ramp_counter_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_41_b12_ramp_counter_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_ramp_r_ccm : 4;
        unsigned char b12_ramp_mid_sel : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_42_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_42_b12_ramp_r_ccm_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_42_b12_ramp_r_ccm_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_42_b12_ramp_mid_sel_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_42_b12_ramp_mid_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_ramp_r_eco : 4;
        unsigned char b12_ramp_r_cot : 4;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_43_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_43_b12_ramp_r_eco_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_43_b12_ramp_r_eco_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_43_b12_ramp_r_cot_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_43_b12_ramp_r_cot_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_ramptrim_sel : 1;
        unsigned char b12_ramp_valley_sel : 1;
        unsigned char b12_ramp_trim_sel : 1;
        unsigned char b12_ramp_sw_sel : 1;
        unsigned char b12_ramp_sel : 1;
        unsigned char b12_ramp_res_test : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_44_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_44_b12_ramptrim_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_44_b12_ramptrim_sel_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_44_b12_ramp_valley_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_44_b12_ramp_valley_sel_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_44_b12_ramp_trim_sel_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_44_b12_ramp_trim_sel_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_44_b12_ramp_sw_sel_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_44_b12_ramp_sw_sel_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_44_b12_ramp_sel_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_44_b12_ramp_sel_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_44_b12_ramp_res_test_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_44_b12_ramp_res_test_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_reg_idrp : 3;
        unsigned char b12_reg_idrn : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_45_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_45_b12_reg_idrp_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_45_b12_reg_idrp_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_45_b12_reg_idrn_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_45_b12_reg_idrn_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_reg_sense_res_sel : 3;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_46_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_46_b12_reg_sense_res_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_46_b12_reg_sense_res_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_sft : 1;
        unsigned char b12_sense_ratio_sel : 2;
        unsigned char b12_rtr_cap_sel : 2;
        unsigned char b12_ron_test_sel : 3;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_47_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_47_b12_sft_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_47_b12_sft_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_47_b12_sense_ratio_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_47_b12_sense_ratio_sel_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_47_b12_rtr_cap_sel_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_47_b12_rtr_cap_sel_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_47_b12_ron_test_sel_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_47_b12_ron_test_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_stsw_atest : 1;
        unsigned char b12_state_sw_ocp_sel : 2;
        unsigned char b12_state_sw_ocp_off : 1;
        unsigned char b12_slave_sel : 1;
        unsigned char b12_short_shield : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_48_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_48_b12_stsw_atest_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_48_b12_stsw_atest_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_48_b12_state_sw_ocp_sel_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_48_b12_state_sw_ocp_sel_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_48_b12_state_sw_ocp_off_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_48_b12_state_sw_ocp_off_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_48_b12_slave_sel_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_48_b12_slave_sel_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_48_b12_short_shield_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_48_b12_short_shield_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_stsw_fstsch_eco : 3;
        unsigned char b12_stsw_fstps_ith : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_49_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_49_b12_stsw_fstsch_eco_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_49_b12_stsw_fstsch_eco_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_49_b12_stsw_fstps_ith_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_49_b12_stsw_fstps_ith_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_stsw_slw_ct2cm_hys : 2;
        unsigned char b12_stsw_rpup_ccm : 1;
        unsigned char b12_stsw_man_set : 2;
        unsigned char b12_stsw_fstsch_neco : 3;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_50_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_50_b12_stsw_slw_ct2cm_hys_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_50_b12_stsw_slw_ct2cm_hys_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_50_b12_stsw_rpup_ccm_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_50_b12_stsw_rpup_ccm_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_50_b12_stsw_man_set_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_50_b12_stsw_man_set_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_50_b12_stsw_fstsch_neco_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_50_b12_stsw_fstsch_neco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_stsw_slw_hys_shield : 1;
        unsigned char b12_stsw_slw_delay_l : 2;
        unsigned char b12_stsw_slw_delay_h : 2;
        unsigned char b12_stsw_slw_ct2cm_ith : 3;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_51_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_51_b12_stsw_slw_hys_shield_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_51_b12_stsw_slw_hys_shield_END (0)
#define SUB_PMIC_BUCK12_CFG_REG_51_b12_stsw_slw_delay_l_START (1)
#define SUB_PMIC_BUCK12_CFG_REG_51_b12_stsw_slw_delay_l_END (2)
#define SUB_PMIC_BUCK12_CFG_REG_51_b12_stsw_slw_delay_h_START (3)
#define SUB_PMIC_BUCK12_CFG_REG_51_b12_stsw_slw_delay_h_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_51_b12_stsw_slw_ct2cm_ith_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_51_b12_stsw_slw_ct2cm_ith_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_ton_det : 4;
        unsigned char b12_stsw_slwsch_neco : 2;
        unsigned char b12_stsw_slwsch_eco : 2;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_52_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_52_b12_ton_det_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_52_b12_ton_det_END (3)
#define SUB_PMIC_BUCK12_CFG_REG_52_b12_stsw_slwsch_neco_START (4)
#define SUB_PMIC_BUCK12_CFG_REG_52_b12_stsw_slwsch_neco_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_52_b12_stsw_slwsch_eco_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_52_b12_stsw_slwsch_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_vref_test_sel : 2;
        unsigned char b12_vo_ton_res_sel : 3;
        unsigned char b12_trim2 : 1;
        unsigned char b12_trim1 : 1;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_53_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_53_b12_vref_test_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_53_b12_vref_test_sel_END (1)
#define SUB_PMIC_BUCK12_CFG_REG_53_b12_vo_ton_res_sel_START (2)
#define SUB_PMIC_BUCK12_CFG_REG_53_b12_vo_ton_res_sel_END (4)
#define SUB_PMIC_BUCK12_CFG_REG_53_b12_trim2_START (5)
#define SUB_PMIC_BUCK12_CFG_REG_53_b12_trim2_END (5)
#define SUB_PMIC_BUCK12_CFG_REG_53_b12_trim1_START (6)
#define SUB_PMIC_BUCK12_CFG_REG_53_b12_trim1_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_vref_vo_cap_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_54_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_54_b12_vref_vo_cap_sel_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_54_b12_vref_vo_cap_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_reserve0 : 8;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_55_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_55_b12_reserve0_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_55_b12_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_reserve1 : 8;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_56_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_56_b12_reserve1_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_56_b12_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_reserve2 : 8;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_57_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_57_b12_reserve2_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_57_b12_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_reserve3 : 8;
    } reg;
} SUB_PMIC_BUCK12_CFG_REG_58_UNION;
#endif
#define SUB_PMIC_BUCK12_CFG_REG_58_b12_reserve3_START (0)
#define SUB_PMIC_BUCK12_CFG_REG_58_b12_reserve3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b12_state_a2d : 2;
        unsigned char b12_pg : 1;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_BUCK12_RO_REG_59_UNION;
#endif
#define SUB_PMIC_BUCK12_RO_REG_59_b12_state_a2d_START (0)
#define SUB_PMIC_BUCK12_RO_REG_59_b12_state_a2d_END (1)
#define SUB_PMIC_BUCK12_RO_REG_59_b12_pg_START (2)
#define SUB_PMIC_BUCK12_RO_REG_59_b12_pg_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo52_en : 1;
        unsigned char st_ldo52_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo52_eco_en : 1;
        unsigned char st_ldo52_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} SUB_PMIC_LDO52_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_LDO52_ONOFF_ECO_reg_ldo52_en_START (0)
#define SUB_PMIC_LDO52_ONOFF_ECO_reg_ldo52_en_END (0)
#define SUB_PMIC_LDO52_ONOFF_ECO_st_ldo52_en_START (1)
#define SUB_PMIC_LDO52_ONOFF_ECO_st_ldo52_en_END (1)
#define SUB_PMIC_LDO52_ONOFF_ECO_reg_ldo52_eco_en_START (4)
#define SUB_PMIC_LDO52_ONOFF_ECO_reg_ldo52_eco_en_END (4)
#define SUB_PMIC_LDO52_ONOFF_ECO_st_ldo52_eco_en_START (5)
#define SUB_PMIC_LDO52_ONOFF_ECO_st_ldo52_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo52_vset_cfg : 3;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_LDO52_VSET_UNION;
#endif
#define SUB_PMIC_LDO52_VSET_ldo52_vset_cfg_START (0)
#define SUB_PMIC_LDO52_VSET_ldo52_vset_cfg_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo52_vrset : 3;
        unsigned char ldo52_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_LDO52_CFG_0_UNION;
#endif
#define SUB_PMIC_LDO52_CFG_0_ldo52_vrset_START (0)
#define SUB_PMIC_LDO52_CFG_0_ldo52_vrset_END (2)
#define SUB_PMIC_LDO52_CFG_0_ldo52_ocp_enn_START (3)
#define SUB_PMIC_LDO52_CFG_0_ldo52_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_sw10_en : 1;
        unsigned char st_sw10_en : 1;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_SW10_ONOFF_ECO_UNION;
#endif
#define SUB_PMIC_SW10_ONOFF_ECO_reg_sw10_en_START (0)
#define SUB_PMIC_SW10_ONOFF_ECO_reg_sw10_en_END (0)
#define SUB_PMIC_SW10_ONOFF_ECO_st_sw10_en_START (1)
#define SUB_PMIC_SW10_ONOFF_ECO_st_sw10_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sw10_sst_set : 3;
        unsigned char sw10_offset : 4;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_SW10_CFG_0_UNION;
#endif
#define SUB_PMIC_SW10_CFG_0_sw10_sst_set_START (0)
#define SUB_PMIC_SW10_CFG_0_sw10_sst_set_END (2)
#define SUB_PMIC_SW10_CFG_0_sw10_offset_START (3)
#define SUB_PMIC_SW10_CFG_0_sw10_offset_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_d2a_reserve0 : 8;
    } reg;
} SUB_PMIC_D2A_SER_RESERV0_UNION;
#endif
#define SUB_PMIC_D2A_SER_RESERV0_ser_d2a_reserve0_START (0)
#define SUB_PMIC_D2A_SER_RESERV0_ser_d2a_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_d2a_reserve1 : 8;
    } reg;
} SUB_PMIC_D2A_SER_RESERV1_UNION;
#endif
#define SUB_PMIC_D2A_SER_RESERV1_ser_d2a_reserve1_START (0)
#define SUB_PMIC_D2A_SER_RESERV1_ser_d2a_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_actt_en : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_LRA_CTRL_UNION;
#endif
#define SUB_PMIC_LRA_CTRL_reg_actt_en_START (0)
#define SUB_PMIC_LRA_CTRL_reg_actt_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_brk_en : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_BRAKE_CTRL_UNION;
#endif
#define SUB_PMIC_BRAKE_CTRL_reg_brk_en_START (0)
#define SUB_PMIC_BRAKE_CTRL_reg_brk_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ovd_low_num : 3;
        unsigned char ovd_high_num : 5;
    } reg;
} SUB_PMIC_DRV_H_L_NUM_UNION;
#endif
#define SUB_PMIC_DRV_H_L_NUM_ovd_low_num_START (0)
#define SUB_PMIC_DRV_H_L_NUM_ovd_low_num_END (2)
#define SUB_PMIC_DRV_H_L_NUM_ovd_high_num_START (3)
#define SUB_PMIC_DRV_H_L_NUM_ovd_high_num_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char brk_high_num : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BRK_H_NUM_UNION;
#endif
#define SUB_PMIC_BRK_H_NUM_brk_high_num_START (0)
#define SUB_PMIC_BRK_H_NUM_brk_high_num_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char adc_ave_cfg : 3;
        unsigned char max_dete_num : 1;
        unsigned char th_rgt_num : 1;
        unsigned char lra_ocp_th : 3;
    } reg;
} SUB_PMIC_LRA_CFG0_UNION;
#endif
#define SUB_PMIC_LRA_CFG0_adc_ave_cfg_START (0)
#define SUB_PMIC_LRA_CFG0_adc_ave_cfg_END (2)
#define SUB_PMIC_LRA_CFG0_max_dete_num_START (3)
#define SUB_PMIC_LRA_CFG0_max_dete_num_END (3)
#define SUB_PMIC_LRA_CFG0_th_rgt_num_START (4)
#define SUB_PMIC_LRA_CFG0_th_rgt_num_END (4)
#define SUB_PMIC_LRA_CFG0_lra_ocp_th_START (5)
#define SUB_PMIC_LRA_CFG0_lra_ocp_th_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char drv_step_time : 4;
        unsigned char bemf_wait_time : 3;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LRA_CFG1_UNION;
#endif
#define SUB_PMIC_LRA_CFG1_drv_step_time_START (0)
#define SUB_PMIC_LRA_CFG1_drv_step_time_END (3)
#define SUB_PMIC_LRA_CFG1_bemf_wait_time_START (4)
#define SUB_PMIC_LRA_CFG1_bemf_wait_time_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char fst_ovdr_time : 8;
    } reg;
} SUB_PMIC_DRV_TIME_CFG_UNION;
#endif
#define SUB_PMIC_DRV_TIME_CFG_fst_ovdr_time_START (0)
#define SUB_PMIC_DRV_TIME_CFG_fst_ovdr_time_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char drv_max_time : 8;
    } reg;
} SUB_PMIC_MAX_TIME_CFG_UNION;
#endif
#define SUB_PMIC_MAX_TIME_CFG_drv_max_time_START (0)
#define SUB_PMIC_MAX_TIME_CFG_drv_max_time_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char bemf_out_time : 8;
    } reg;
} SUB_PMIC_BEMF_OUT_CFG_UNION;
#endif
#define SUB_PMIC_BEMF_OUT_CFG_bemf_out_time_START (0)
#define SUB_PMIC_BEMF_OUT_CFG_bemf_out_time_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_bemf_time : 6;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_BEMF_TIME_UNION;
#endif
#define SUB_PMIC_BEMF_TIME_lra_bemf_time_START (0)
#define SUB_PMIC_BEMF_TIME_lra_bemf_time_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char duty_init_num_l : 8;
    } reg;
} SUB_PMIC_DUTY_INIT_CFG_LOW_UNION;
#endif
#define SUB_PMIC_DUTY_INIT_CFG_LOW_duty_init_num_l_START (0)
#define SUB_PMIC_DUTY_INIT_CFG_LOW_duty_init_num_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char duty_init_num_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_DUTY_INIT_CFG_HIGH_UNION;
#endif
#define SUB_PMIC_DUTY_INIT_CFG_HIGH_duty_init_num_h_START (0)
#define SUB_PMIC_DUTY_INIT_CFG_HIGH_duty_init_num_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char duty_ovdr_cps : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_DUTY_OVDR_CPS_UNION;
#endif
#define SUB_PMIC_DUTY_OVDR_CPS_duty_ovdr_cps_START (0)
#define SUB_PMIC_DUTY_OVDR_CPS_duty_ovdr_cps_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char duty_ovdr_num_l : 8;
    } reg;
} SUB_PMIC_DUTY_OVDR_CFG_LOW_UNION;
#endif
#define SUB_PMIC_DUTY_OVDR_CFG_LOW_duty_ovdr_num_l_START (0)
#define SUB_PMIC_DUTY_OVDR_CFG_LOW_duty_ovdr_num_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char duty_ovdr_num_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_DUTY_OVDR_CFG_HIGH_UNION;
#endif
#define SUB_PMIC_DUTY_OVDR_CFG_HIGH_duty_ovdr_num_h_START (0)
#define SUB_PMIC_DUTY_OVDR_CFG_HIGH_duty_ovdr_num_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char duty_nml_cps : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_DUTY_NORMAL_CPS_UNION;
#endif
#define SUB_PMIC_DUTY_NORMAL_CPS_duty_nml_cps_START (0)
#define SUB_PMIC_DUTY_NORMAL_CPS_duty_nml_cps_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char duty_nml_num_l : 8;
    } reg;
} SUB_PMIC_DUTY_NORMAL_CFG_LOW_UNION;
#endif
#define SUB_PMIC_DUTY_NORMAL_CFG_LOW_duty_nml_num_l_START (0)
#define SUB_PMIC_DUTY_NORMAL_CFG_LOW_duty_nml_num_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char duty_nml_num_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_DUTY_NORMAL_CFG_HIGH_UNION;
#endif
#define SUB_PMIC_DUTY_NORMAL_CFG_HIGH_duty_nml_num_h_START (0)
#define SUB_PMIC_DUTY_NORMAL_CFG_HIGH_duty_nml_num_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char duty_brk_cps : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_DUTY_BRAKE_CPS_UNION;
#endif
#define SUB_PMIC_DUTY_BRAKE_CPS_duty_brk_cps_START (0)
#define SUB_PMIC_DUTY_BRAKE_CPS_duty_brk_cps_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char duty_brk_num_l : 8;
    } reg;
} SUB_PMIC_DUTY_BRAKE_CFG_LOW_UNION;
#endif
#define SUB_PMIC_DUTY_BRAKE_CFG_LOW_duty_brk_num_l_START (0)
#define SUB_PMIC_DUTY_BRAKE_CFG_LOW_duty_brk_num_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char duty_brk_num_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_DUTY_BRAKE_CFG_HIGH_UNION;
#endif
#define SUB_PMIC_DUTY_BRAKE_CFG_HIGH_duty_brk_num_h_START (0)
#define SUB_PMIC_DUTY_BRAKE_CFG_HIGH_duty_brk_num_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eob_k_1_l : 8;
    } reg;
} SUB_PMIC_EOB_K_1_LOW_UNION;
#endif
#define SUB_PMIC_EOB_K_1_LOW_eob_k_1_l_START (0)
#define SUB_PMIC_EOB_K_1_LOW_eob_k_1_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eob_k_1_h : 8;
    } reg;
} SUB_PMIC_EOB_K_1_HIGH_UNION;
#endif
#define SUB_PMIC_EOB_K_1_HIGH_eob_k_1_h_START (0)
#define SUB_PMIC_EOB_K_1_HIGH_eob_k_1_h_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eob_k_2_l : 8;
    } reg;
} SUB_PMIC_EOB_K_2_LOW_UNION;
#endif
#define SUB_PMIC_EOB_K_2_LOW_eob_k_2_l_START (0)
#define SUB_PMIC_EOB_K_2_LOW_eob_k_2_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eob_k_2_h : 8;
    } reg;
} SUB_PMIC_EOB_K_2_HIGH_UNION;
#endif
#define SUB_PMIC_EOB_K_2_HIGH_eob_k_2_h_START (0)
#define SUB_PMIC_EOB_K_2_HIGH_eob_k_2_h_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char max_min_th : 8;
    } reg;
} SUB_PMIC_ADC_MAX_MIN_TH_UNION;
#endif
#define SUB_PMIC_ADC_MAX_MIN_TH_max_min_th_START (0)
#define SUB_PMIC_ADC_MAX_MIN_TH_max_min_th_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char th_boost_v_l : 8;
    } reg;
} SUB_PMIC_TH_BOOST_V_CFG_LOW_UNION;
#endif
#define SUB_PMIC_TH_BOOST_V_CFG_LOW_th_boost_v_l_START (0)
#define SUB_PMIC_TH_BOOST_V_CFG_LOW_th_boost_v_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char th_boost_v_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_TH_BOOST_V_CFG_HIGH_UNION;
#endif
#define SUB_PMIC_TH_BOOST_V_CFG_HIGH_th_boost_v_h_START (0)
#define SUB_PMIC_TH_BOOST_V_CFG_HIGH_th_boost_v_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char th_gate1_l : 8;
    } reg;
} SUB_PMIC_GATE_VOLTAGE1_LOW_UNION;
#endif
#define SUB_PMIC_GATE_VOLTAGE1_LOW_th_gate1_l_START (0)
#define SUB_PMIC_GATE_VOLTAGE1_LOW_th_gate1_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char th_gate1_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_GATE_VOLTAGE1_HIGH_UNION;
#endif
#define SUB_PMIC_GATE_VOLTAGE1_HIGH_th_gate1_h_START (0)
#define SUB_PMIC_GATE_VOLTAGE1_HIGH_th_gate1_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char th_gate2_l : 8;
    } reg;
} SUB_PMIC_GATE_VOLTAGE2_LOW_UNION;
#endif
#define SUB_PMIC_GATE_VOLTAGE2_LOW_th_gate2_l_START (0)
#define SUB_PMIC_GATE_VOLTAGE2_LOW_th_gate2_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char th_gate2_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_GATE_VOLTAGE2_HIGH_UNION;
#endif
#define SUB_PMIC_GATE_VOLTAGE2_HIGH_th_gate2_h_START (0)
#define SUB_PMIC_GATE_VOLTAGE2_HIGH_th_gate2_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char th_gate3_l : 8;
    } reg;
} SUB_PMIC_GATE_VOLTAGE3_LOW_UNION;
#endif
#define SUB_PMIC_GATE_VOLTAGE3_LOW_th_gate3_l_START (0)
#define SUB_PMIC_GATE_VOLTAGE3_LOW_th_gate3_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char th_gate3_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_GATE_VOLTAGE3_HIGH_UNION;
#endif
#define SUB_PMIC_GATE_VOLTAGE3_HIGH_th_gate3_h_START (0)
#define SUB_PMIC_GATE_VOLTAGE3_HIGH_th_gate3_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char brk_bemf_min_th_l : 8;
    } reg;
} SUB_PMIC_BRK_BEMF_MIN_CFG_L_UNION;
#endif
#define SUB_PMIC_BRK_BEMF_MIN_CFG_L_brk_bemf_min_th_l_START (0)
#define SUB_PMIC_BRK_BEMF_MIN_CFG_L_brk_bemf_min_th_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char brk_bemf_min_th_h : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_BRK_BEMF_MIN_CFG_H_UNION;
#endif
#define SUB_PMIC_BRK_BEMF_MIN_CFG_H_brk_bemf_min_th_h_START (0)
#define SUB_PMIC_BRK_BEMF_MIN_CFG_H_brk_bemf_min_th_h_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ref_u_l : 8;
    } reg;
} SUB_PMIC_REF_U_CFG_L_UNION;
#endif
#define SUB_PMIC_REF_U_CFG_L_ref_u_l_START (0)
#define SUB_PMIC_REF_U_CFG_L_ref_u_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ref_u_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_REF_U_CFG_H_UNION;
#endif
#define SUB_PMIC_REF_U_CFG_H_ref_u_h_START (0)
#define SUB_PMIC_REF_U_CFG_H_ref_u_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char kfit_sel : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_KFIT_DATA_SEL_UNION;
#endif
#define SUB_PMIC_KFIT_DATA_SEL_kfit_sel_START (0)
#define SUB_PMIC_KFIT_DATA_SEL_kfit_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char data_k_rvs_l : 8;
    } reg;
} SUB_PMIC_K_RVS_LOW_UNION;
#endif
#define SUB_PMIC_K_RVS_LOW_data_k_rvs_l_START (0)
#define SUB_PMIC_K_RVS_LOW_data_k_rvs_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char data_k_rvs_h : 6;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_K_RVS_HIGH_UNION;
#endif
#define SUB_PMIC_K_RVS_HIGH_data_k_rvs_h_START (0)
#define SUB_PMIC_K_RVS_HIGH_data_k_rvs_h_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char data_b_rvs_l : 8;
    } reg;
} SUB_PMIC_B_RVS_LOW_UNION;
#endif
#define SUB_PMIC_B_RVS_LOW_data_b_rvs_l_START (0)
#define SUB_PMIC_B_RVS_LOW_data_b_rvs_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char data_b_rvs_h : 8;
    } reg;
} SUB_PMIC_B_RVS_HIGH_UNION;
#endif
#define SUB_PMIC_B_RVS_HIGH_data_b_rvs_h_START (0)
#define SUB_PMIC_B_RVS_HIGH_data_b_rvs_h_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_1 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA1_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA1_CFG_ladd_para_1_START (0)
#define SUB_PMIC_LADD_PARA1_CFG_ladd_para_1_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_2 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA2_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA2_CFG_ladd_para_2_START (0)
#define SUB_PMIC_LADD_PARA2_CFG_ladd_para_2_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_3 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA3_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA3_CFG_ladd_para_3_START (0)
#define SUB_PMIC_LADD_PARA3_CFG_ladd_para_3_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_4 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA4_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA4_CFG_ladd_para_4_START (0)
#define SUB_PMIC_LADD_PARA4_CFG_ladd_para_4_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_5 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA5_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA5_CFG_ladd_para_5_START (0)
#define SUB_PMIC_LADD_PARA5_CFG_ladd_para_5_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_6 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA6_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA6_CFG_ladd_para_6_START (0)
#define SUB_PMIC_LADD_PARA6_CFG_ladd_para_6_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_7 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA7_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA7_CFG_ladd_para_7_START (0)
#define SUB_PMIC_LADD_PARA7_CFG_ladd_para_7_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_8 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA8_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA8_CFG_ladd_para_8_START (0)
#define SUB_PMIC_LADD_PARA8_CFG_ladd_para_8_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_9 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA9_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA9_CFG_ladd_para_9_START (0)
#define SUB_PMIC_LADD_PARA9_CFG_ladd_para_9_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_10 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA10_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA10_CFG_ladd_para_10_START (0)
#define SUB_PMIC_LADD_PARA10_CFG_ladd_para_10_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_11 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA11_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA11_CFG_ladd_para_11_START (0)
#define SUB_PMIC_LADD_PARA11_CFG_ladd_para_11_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_12 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA12_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA12_CFG_ladd_para_12_START (0)
#define SUB_PMIC_LADD_PARA12_CFG_ladd_para_12_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_13 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA13_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA13_CFG_ladd_para_13_START (0)
#define SUB_PMIC_LADD_PARA13_CFG_ladd_para_13_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_14 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA14_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA14_CFG_ladd_para_14_START (0)
#define SUB_PMIC_LADD_PARA14_CFG_ladd_para_14_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_15 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA15_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA15_CFG_ladd_para_15_START (0)
#define SUB_PMIC_LADD_PARA15_CFG_ladd_para_15_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_16 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA16_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA16_CFG_ladd_para_16_START (0)
#define SUB_PMIC_LADD_PARA16_CFG_ladd_para_16_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_17 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA17_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA17_CFG_ladd_para_17_START (0)
#define SUB_PMIC_LADD_PARA17_CFG_ladd_para_17_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_18 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA18_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA18_CFG_ladd_para_18_START (0)
#define SUB_PMIC_LADD_PARA18_CFG_ladd_para_18_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_19 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA19_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA19_CFG_ladd_para_19_START (0)
#define SUB_PMIC_LADD_PARA19_CFG_ladd_para_19_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ladd_para_20 : 7;
        unsigned char reserved : 1;
    } reg;
} SUB_PMIC_LADD_PARA20_CFG_UNION;
#endif
#define SUB_PMIC_LADD_PARA20_CFG_ladd_para_20_START (0)
#define SUB_PMIC_LADD_PARA20_CFG_ladd_para_20_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_out_brk_bemf_l : 8;
    } reg;
} SUB_PMIC_OUT_BEMF_VOL_L_UNION;
#endif
#define SUB_PMIC_OUT_BEMF_VOL_L_ap_out_brk_bemf_l_START (0)
#define SUB_PMIC_OUT_BEMF_VOL_L_ap_out_brk_bemf_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_out_brk_bemf_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_OUT_BEMF_VOL_H_UNION;
#endif
#define SUB_PMIC_OUT_BEMF_VOL_H_ap_out_brk_bemf_h_START (0)
#define SUB_PMIC_OUT_BEMF_VOL_H_ap_out_brk_bemf_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_out_nml_bemf_l : 8;
    } reg;
} SUB_PMIC_OUT_NML_VBEMF_L_UNION;
#endif
#define SUB_PMIC_OUT_NML_VBEMF_L_ap_out_nml_bemf_l_START (0)
#define SUB_PMIC_OUT_NML_VBEMF_L_ap_out_nml_bemf_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_out_nml_bemf_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_OUT_NML_VBEMF_H_UNION;
#endif
#define SUB_PMIC_OUT_NML_VBEMF_H_ap_out_nml_bemf_h_START (0)
#define SUB_PMIC_OUT_NML_VBEMF_H_ap_out_nml_bemf_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_ovdr_hprd : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_OV_BRK_HPRIOD_UNION;
#endif
#define SUB_PMIC_OV_BRK_HPRIOD_ap_ovdr_hprd_START (0)
#define SUB_PMIC_OV_BRK_HPRIOD_ap_ovdr_hprd_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_brk_hprd : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_BRK_HPRIOD_UNION;
#endif
#define SUB_PMIC_BRK_HPRIOD_ap_brk_hprd_START (0)
#define SUB_PMIC_BRK_HPRIOD_ap_brk_hprd_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_real_u_l : 8;
    } reg;
} SUB_PMIC_UREAL_DATA_L_UNION;
#endif
#define SUB_PMIC_UREAL_DATA_L_ap_real_u_l_START (0)
#define SUB_PMIC_UREAL_DATA_L_ap_real_u_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_real_u_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_UREAL_DATA_H_UNION;
#endif
#define SUB_PMIC_UREAL_DATA_H_ap_real_u_h_START (0)
#define SUB_PMIC_UREAL_DATA_H_ap_real_u_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_adc_max_l : 8;
    } reg;
} SUB_PMIC_ADC_CALI_MAX_L_UNION;
#endif
#define SUB_PMIC_ADC_CALI_MAX_L_ap_adc_max_l_START (0)
#define SUB_PMIC_ADC_CALI_MAX_L_ap_adc_max_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_adc_max_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_ADC_CALI_MAX_H_UNION;
#endif
#define SUB_PMIC_ADC_CALI_MAX_H_ap_adc_max_h_START (0)
#define SUB_PMIC_ADC_CALI_MAX_H_ap_adc_max_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_adc_min_l : 8;
    } reg;
} SUB_PMIC_ADC_CALI_MIN_L_UNION;
#endif
#define SUB_PMIC_ADC_CALI_MIN_L_ap_adc_min_l_START (0)
#define SUB_PMIC_ADC_CALI_MIN_L_ap_adc_min_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_adc_min_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_ADC_CALI_MIN_H_UNION;
#endif
#define SUB_PMIC_ADC_CALI_MIN_H_ap_adc_min_h_START (0)
#define SUB_PMIC_ADC_CALI_MIN_H_ap_adc_min_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_adc_zero_l : 8;
    } reg;
} SUB_PMIC_ADC_ZERO_L_UNION;
#endif
#define SUB_PMIC_ADC_ZERO_L_ap_adc_zero_l_START (0)
#define SUB_PMIC_ADC_ZERO_L_ap_adc_zero_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_adc_zero_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_ADC_ZERO_H_UNION;
#endif
#define SUB_PMIC_ADC_ZERO_H_ap_adc_zero_h_START (0)
#define SUB_PMIC_ADC_ZERO_H_ap_adc_zero_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_kfit_data_l : 8;
    } reg;
} SUB_PMIC_KFIT_DATA_L_UNION;
#endif
#define SUB_PMIC_KFIT_DATA_L_ap_kfit_data_l_START (0)
#define SUB_PMIC_KFIT_DATA_L_ap_kfit_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ap_kfit_data_h : 8;
    } reg;
} SUB_PMIC_KFIT_DATA_H_UNION;
#endif
#define SUB_PMIC_KFIT_DATA_H_ap_kfit_data_h_START (0)
#define SUB_PMIC_KFIT_DATA_H_ap_kfit_data_h_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char st_fsm_lra : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_FSM_STATE_UNION;
#endif
#define SUB_PMIC_FSM_STATE_st_fsm_lra_START (0)
#define SUB_PMIC_FSM_STATE_st_fsm_lra_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_1 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_1_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_1_spel_drv_time_1_START (0)
#define SUB_PMIC_SPEL_TIME_1_spel_drv_time_1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_2 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_2_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_2_spel_drv_time_2_START (0)
#define SUB_PMIC_SPEL_TIME_2_spel_drv_time_2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_3 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_3_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_3_spel_drv_time_3_START (0)
#define SUB_PMIC_SPEL_TIME_3_spel_drv_time_3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_4 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_4_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_4_spel_drv_time_4_START (0)
#define SUB_PMIC_SPEL_TIME_4_spel_drv_time_4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_5 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_5_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_5_spel_drv_time_5_START (0)
#define SUB_PMIC_SPEL_TIME_5_spel_drv_time_5_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_6 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_6_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_6_spel_drv_time_6_START (0)
#define SUB_PMIC_SPEL_TIME_6_spel_drv_time_6_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_7 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_7_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_7_spel_drv_time_7_START (0)
#define SUB_PMIC_SPEL_TIME_7_spel_drv_time_7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_8 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_8_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_8_spel_drv_time_8_START (0)
#define SUB_PMIC_SPEL_TIME_8_spel_drv_time_8_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_9 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_9_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_9_spel_drv_time_9_START (0)
#define SUB_PMIC_SPEL_TIME_9_spel_drv_time_9_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_10 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_10_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_10_spel_drv_time_10_START (0)
#define SUB_PMIC_SPEL_TIME_10_spel_drv_time_10_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_11 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_11_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_11_spel_drv_time_11_START (0)
#define SUB_PMIC_SPEL_TIME_11_spel_drv_time_11_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_12 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_12_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_12_spel_drv_time_12_START (0)
#define SUB_PMIC_SPEL_TIME_12_spel_drv_time_12_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_13 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_13_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_13_spel_drv_time_13_START (0)
#define SUB_PMIC_SPEL_TIME_13_spel_drv_time_13_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_14 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_14_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_14_spel_drv_time_14_START (0)
#define SUB_PMIC_SPEL_TIME_14_spel_drv_time_14_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_15 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_15_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_15_spel_drv_time_15_START (0)
#define SUB_PMIC_SPEL_TIME_15_spel_drv_time_15_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_16 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_16_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_16_spel_drv_time_16_START (0)
#define SUB_PMIC_SPEL_TIME_16_spel_drv_time_16_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_17 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_17_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_17_spel_drv_time_17_START (0)
#define SUB_PMIC_SPEL_TIME_17_spel_drv_time_17_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_18 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_18_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_18_spel_drv_time_18_START (0)
#define SUB_PMIC_SPEL_TIME_18_spel_drv_time_18_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_19 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_19_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_19_spel_drv_time_19_START (0)
#define SUB_PMIC_SPEL_TIME_19_spel_drv_time_19_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_drv_time_20 : 8;
    } reg;
} SUB_PMIC_SPEL_TIME_20_UNION;
#endif
#define SUB_PMIC_SPEL_TIME_20_spel_drv_time_20_START (0)
#define SUB_PMIC_SPEL_TIME_20_spel_drv_time_20_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char duty_spel_cps : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_CPS_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_CPS_duty_spel_cps_START (0)
#define SUB_PMIC_SPEL_DUTY_CPS_duty_spel_cps_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_1_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_1_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_1_L_spel_duty_1_l_START (0)
#define SUB_PMIC_SPEL_DUTY_1_L_spel_duty_1_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_1_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_1_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_1_H_spel_duty_1_h_START (0)
#define SUB_PMIC_SPEL_DUTY_1_H_spel_duty_1_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_2_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_2_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_2_L_spel_duty_2_l_START (0)
#define SUB_PMIC_SPEL_DUTY_2_L_spel_duty_2_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_2_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_2_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_2_H_spel_duty_2_h_START (0)
#define SUB_PMIC_SPEL_DUTY_2_H_spel_duty_2_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_3_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_3_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_3_L_spel_duty_3_l_START (0)
#define SUB_PMIC_SPEL_DUTY_3_L_spel_duty_3_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_3_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_3_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_3_H_spel_duty_3_h_START (0)
#define SUB_PMIC_SPEL_DUTY_3_H_spel_duty_3_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_4_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_4_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_4_L_spel_duty_4_l_START (0)
#define SUB_PMIC_SPEL_DUTY_4_L_spel_duty_4_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_4_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_4_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_4_H_spel_duty_4_h_START (0)
#define SUB_PMIC_SPEL_DUTY_4_H_spel_duty_4_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_5_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_5_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_5_L_spel_duty_5_l_START (0)
#define SUB_PMIC_SPEL_DUTY_5_L_spel_duty_5_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_5_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_5_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_5_H_spel_duty_5_h_START (0)
#define SUB_PMIC_SPEL_DUTY_5_H_spel_duty_5_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_6_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_6_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_6_L_spel_duty_6_l_START (0)
#define SUB_PMIC_SPEL_DUTY_6_L_spel_duty_6_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_6_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_6_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_6_H_spel_duty_6_h_START (0)
#define SUB_PMIC_SPEL_DUTY_6_H_spel_duty_6_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_7_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_7_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_7_L_spel_duty_7_l_START (0)
#define SUB_PMIC_SPEL_DUTY_7_L_spel_duty_7_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_7_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_7_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_7_H_spel_duty_7_h_START (0)
#define SUB_PMIC_SPEL_DUTY_7_H_spel_duty_7_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_8_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_8_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_8_L_spel_duty_8_l_START (0)
#define SUB_PMIC_SPEL_DUTY_8_L_spel_duty_8_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_8_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_8_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_8_H_spel_duty_8_h_START (0)
#define SUB_PMIC_SPEL_DUTY_8_H_spel_duty_8_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_9_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_9_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_9_L_spel_duty_9_l_START (0)
#define SUB_PMIC_SPEL_DUTY_9_L_spel_duty_9_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_9_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_9_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_9_H_spel_duty_9_h_START (0)
#define SUB_PMIC_SPEL_DUTY_9_H_spel_duty_9_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_10_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_10_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_10_L_spel_duty_10_l_START (0)
#define SUB_PMIC_SPEL_DUTY_10_L_spel_duty_10_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_10_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_10_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_10_H_spel_duty_10_h_START (0)
#define SUB_PMIC_SPEL_DUTY_10_H_spel_duty_10_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_11_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_11_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_11_L_spel_duty_11_l_START (0)
#define SUB_PMIC_SPEL_DUTY_11_L_spel_duty_11_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_11_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_11_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_11_H_spel_duty_11_h_START (0)
#define SUB_PMIC_SPEL_DUTY_11_H_spel_duty_11_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_12_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_12_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_12_L_spel_duty_12_l_START (0)
#define SUB_PMIC_SPEL_DUTY_12_L_spel_duty_12_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_12_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_12_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_12_H_spel_duty_12_h_START (0)
#define SUB_PMIC_SPEL_DUTY_12_H_spel_duty_12_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_13_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_13_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_13_L_spel_duty_13_l_START (0)
#define SUB_PMIC_SPEL_DUTY_13_L_spel_duty_13_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_13_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_13_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_13_H_spel_duty_13_h_START (0)
#define SUB_PMIC_SPEL_DUTY_13_H_spel_duty_13_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_14_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_14_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_14_L_spel_duty_14_l_START (0)
#define SUB_PMIC_SPEL_DUTY_14_L_spel_duty_14_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_14_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_14_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_14_H_spel_duty_14_h_START (0)
#define SUB_PMIC_SPEL_DUTY_14_H_spel_duty_14_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_15_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_15_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_15_L_spel_duty_15_l_START (0)
#define SUB_PMIC_SPEL_DUTY_15_L_spel_duty_15_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_15_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_15_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_15_H_spel_duty_15_h_START (0)
#define SUB_PMIC_SPEL_DUTY_15_H_spel_duty_15_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_16_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_16_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_16_L_spel_duty_16_l_START (0)
#define SUB_PMIC_SPEL_DUTY_16_L_spel_duty_16_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_16_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_16_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_16_H_spel_duty_16_h_START (0)
#define SUB_PMIC_SPEL_DUTY_16_H_spel_duty_16_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_17_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_17_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_17_L_spel_duty_17_l_START (0)
#define SUB_PMIC_SPEL_DUTY_17_L_spel_duty_17_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_17_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_17_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_17_H_spel_duty_17_h_START (0)
#define SUB_PMIC_SPEL_DUTY_17_H_spel_duty_17_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_18_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_18_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_18_L_spel_duty_18_l_START (0)
#define SUB_PMIC_SPEL_DUTY_18_L_spel_duty_18_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_18_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_18_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_18_H_spel_duty_18_h_START (0)
#define SUB_PMIC_SPEL_DUTY_18_H_spel_duty_18_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_19_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_19_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_19_L_spel_duty_19_l_START (0)
#define SUB_PMIC_SPEL_DUTY_19_L_spel_duty_19_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_19_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_19_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_19_H_spel_duty_19_h_START (0)
#define SUB_PMIC_SPEL_DUTY_19_H_spel_duty_19_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_20_l : 8;
    } reg;
} SUB_PMIC_SPEL_DUTY_20_L_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_20_L_spel_duty_20_l_START (0)
#define SUB_PMIC_SPEL_DUTY_20_L_spel_duty_20_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spel_duty_20_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_SPEL_DUTY_20_H_UNION;
#endif
#define SUB_PMIC_SPEL_DUTY_20_H_spel_duty_20_h_START (0)
#define SUB_PMIC_SPEL_DUTY_20_H_spel_duty_20_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_adc_clk_cfg : 1;
        unsigned char lra_vsys_gain_sel : 2;
        unsigned char lra_pga_gain_sel : 3;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_LRA_ANA_CFG0_UNION;
#endif
#define SUB_PMIC_LRA_ANA_CFG0_lra_adc_clk_cfg_START (0)
#define SUB_PMIC_LRA_ANA_CFG0_lra_adc_clk_cfg_END (0)
#define SUB_PMIC_LRA_ANA_CFG0_lra_vsys_gain_sel_START (1)
#define SUB_PMIC_LRA_ANA_CFG0_lra_vsys_gain_sel_END (2)
#define SUB_PMIC_LRA_ANA_CFG0_lra_pga_gain_sel_START (3)
#define SUB_PMIC_LRA_ANA_CFG0_lra_pga_gain_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_pd_res_sel : 2;
        unsigned char lra_power_enhance : 2;
        unsigned char lra_init_vld_shield : 1;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_LRA_ANA_CFG1_UNION;
#endif
#define SUB_PMIC_LRA_ANA_CFG1_lra_pd_res_sel_START (0)
#define SUB_PMIC_LRA_ANA_CFG1_lra_pd_res_sel_END (1)
#define SUB_PMIC_LRA_ANA_CFG1_lra_power_enhance_START (2)
#define SUB_PMIC_LRA_ANA_CFG1_lra_power_enhance_END (3)
#define SUB_PMIC_LRA_ANA_CFG1_lra_init_vld_shield_START (4)
#define SUB_PMIC_LRA_ANA_CFG1_lra_init_vld_shield_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_pg_n_sel : 2;
        unsigned char lra_pg_p_sel : 2;
        unsigned char lra_ng_n_sel : 2;
        unsigned char lra_ng_p_sel : 2;
    } reg;
} SUB_PMIC_LRA_ANA_CFG2_UNION;
#endif
#define SUB_PMIC_LRA_ANA_CFG2_lra_pg_n_sel_START (0)
#define SUB_PMIC_LRA_ANA_CFG2_lra_pg_n_sel_END (1)
#define SUB_PMIC_LRA_ANA_CFG2_lra_pg_p_sel_START (2)
#define SUB_PMIC_LRA_ANA_CFG2_lra_pg_p_sel_END (3)
#define SUB_PMIC_LRA_ANA_CFG2_lra_ng_n_sel_START (4)
#define SUB_PMIC_LRA_ANA_CFG2_lra_ng_n_sel_END (5)
#define SUB_PMIC_LRA_ANA_CFG2_lra_ng_p_sel_START (6)
#define SUB_PMIC_LRA_ANA_CFG2_lra_ng_p_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_ocl_shield_nmos : 1;
        unsigned char lra_ocl_shield_pmos : 1;
        unsigned char lra_uvp_shield : 1;
        unsigned char lra_adc_chop_enb : 1;
        unsigned char lra_pga_chop_enb : 1;
        unsigned char lra_buff_chop_enb : 1;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_LRA_ANA_CFG3_UNION;
#endif
#define SUB_PMIC_LRA_ANA_CFG3_lra_ocl_shield_nmos_START (0)
#define SUB_PMIC_LRA_ANA_CFG3_lra_ocl_shield_nmos_END (0)
#define SUB_PMIC_LRA_ANA_CFG3_lra_ocl_shield_pmos_START (1)
#define SUB_PMIC_LRA_ANA_CFG3_lra_ocl_shield_pmos_END (1)
#define SUB_PMIC_LRA_ANA_CFG3_lra_uvp_shield_START (2)
#define SUB_PMIC_LRA_ANA_CFG3_lra_uvp_shield_END (2)
#define SUB_PMIC_LRA_ANA_CFG3_lra_adc_chop_enb_START (3)
#define SUB_PMIC_LRA_ANA_CFG3_lra_adc_chop_enb_END (3)
#define SUB_PMIC_LRA_ANA_CFG3_lra_pga_chop_enb_START (4)
#define SUB_PMIC_LRA_ANA_CFG3_lra_pga_chop_enb_END (4)
#define SUB_PMIC_LRA_ANA_CFG3_lra_buff_chop_enb_START (5)
#define SUB_PMIC_LRA_ANA_CFG3_lra_buff_chop_enb_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_ocp_shield : 1;
        unsigned char lra_ocp_thp_sel : 2;
        unsigned char lra_ocp_thn_sel : 2;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_LRA_OCP_CFG_UNION;
#endif
#define SUB_PMIC_LRA_OCP_CFG_lra_ocp_shield_START (0)
#define SUB_PMIC_LRA_OCP_CFG_lra_ocp_shield_END (0)
#define SUB_PMIC_LRA_OCP_CFG_lra_ocp_thp_sel_START (1)
#define SUB_PMIC_LRA_OCP_CFG_lra_ocp_thp_sel_END (2)
#define SUB_PMIC_LRA_OCP_CFG_lra_ocp_thn_sel_START (3)
#define SUB_PMIC_LRA_OCP_CFG_lra_ocp_thn_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_ibias_sel : 8;
    } reg;
} SUB_PMIC_LRA_IBIAS_SEL_UNION;
#endif
#define SUB_PMIC_LRA_IBIAS_SEL_lra_ibias_sel_START (0)
#define SUB_PMIC_LRA_IBIAS_SEL_lra_ibias_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_ovp_shield : 1;
        unsigned char lra_ovp_th_sel : 2;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_LRA_OVP_CFG_UNION;
#endif
#define SUB_PMIC_LRA_OVP_CFG_lra_ovp_shield_START (0)
#define SUB_PMIC_LRA_OVP_CFG_lra_ovp_shield_END (0)
#define SUB_PMIC_LRA_OVP_CFG_lra_ovp_th_sel_START (1)
#define SUB_PMIC_LRA_OVP_CFG_lra_ovp_th_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_ana_reserve0 : 8;
    } reg;
} SUB_PMIC_LRA_ANA_RESERVED0_UNION;
#endif
#define SUB_PMIC_LRA_ANA_RESERVED0_lra_ana_reserve0_START (0)
#define SUB_PMIC_LRA_ANA_RESERVED0_lra_ana_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_ana_reserve1 : 8;
    } reg;
} SUB_PMIC_LRA_ANA_RESERVED1_UNION;
#endif
#define SUB_PMIC_LRA_ANA_RESERVED1_lra_ana_reserve1_START (0)
#define SUB_PMIC_LRA_ANA_RESERVED1_lra_ana_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_smpl_en : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_HPRD_SAMPLE_EN_UNION;
#endif
#define SUB_PMIC_HPRD_SAMPLE_EN_hp_smpl_en_START (0)
#define SUB_PMIC_HPRD_SAMPLE_EN_hp_smpl_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_smpl_valid : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_HPRD_SAMPLE_VALID_UNION;
#endif
#define SUB_PMIC_HPRD_SAMPLE_VALID_hp_smpl_valid_START (0)
#define SUB_PMIC_HPRD_SAMPLE_VALID_hp_smpl_valid_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_bemf_max_l : 8;
    } reg;
} SUB_PMIC_HPRD_BEMF_MAX_L_UNION;
#endif
#define SUB_PMIC_HPRD_BEMF_MAX_L_hp_bemf_max_l_START (0)
#define SUB_PMIC_HPRD_BEMF_MAX_L_hp_bemf_max_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_bemf_max_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_HPRD_BEMF_MAX_H_UNION;
#endif
#define SUB_PMIC_HPRD_BEMF_MAX_H_hp_bemf_max_h_START (0)
#define SUB_PMIC_HPRD_BEMF_MAX_H_hp_bemf_max_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_bemf_min_l : 8;
    } reg;
} SUB_PMIC_HPRD_BEMF_MIN_L_UNION;
#endif
#define SUB_PMIC_HPRD_BEMF_MIN_L_hp_bemf_min_l_START (0)
#define SUB_PMIC_HPRD_BEMF_MIN_L_hp_bemf_min_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_bemf_min_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_HPRD_BEMF_MIN_H_UNION;
#endif
#define SUB_PMIC_HPRD_BEMF_MIN_H_hp_bemf_min_h_START (0)
#define SUB_PMIC_HPRD_BEMF_MIN_H_hp_bemf_min_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_bemf_fst_l : 8;
    } reg;
} SUB_PMIC_HPRD_BEMF_FIRST_L_UNION;
#endif
#define SUB_PMIC_HPRD_BEMF_FIRST_L_hp_bemf_fst_l_START (0)
#define SUB_PMIC_HPRD_BEMF_FIRST_L_hp_bemf_fst_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_bemf_fst_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_HPRD_BEMF_FIRST_H_UNION;
#endif
#define SUB_PMIC_HPRD_BEMF_FIRST_H_hp_bemf_fst_h_START (0)
#define SUB_PMIC_HPRD_BEMF_FIRST_H_hp_bemf_fst_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_bemf_lst_l : 8;
    } reg;
} SUB_PMIC_HPRD_BEMF_LAST_L_UNION;
#endif
#define SUB_PMIC_HPRD_BEMF_LAST_L_hp_bemf_lst_l_START (0)
#define SUB_PMIC_HPRD_BEMF_LAST_L_hp_bemf_lst_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_bemf_lst_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_HPRD_BEMF_LAST_H_UNION;
#endif
#define SUB_PMIC_HPRD_BEMF_LAST_H_hp_bemf_lst_h_START (0)
#define SUB_PMIC_HPRD_BEMF_LAST_H_hp_bemf_lst_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_dr_time : 8;
    } reg;
} SUB_PMIC_HP_DR_TIME_UNION;
#endif
#define SUB_PMIC_HP_DR_TIME_hp_dr_time_START (0)
#define SUB_PMIC_HP_DR_TIME_hp_dr_time_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_nxt_dr_t : 8;
    } reg;
} SUB_PMIC_HPRD_NEXT_DR_UNION;
#endif
#define SUB_PMIC_HPRD_NEXT_DR_hp_nxt_dr_t_START (0)
#define SUB_PMIC_HPRD_NEXT_DR_hp_nxt_dr_t_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_time_l : 8;
    } reg;
} SUB_PMIC_HPRD_TIME_L_UNION;
#endif
#define SUB_PMIC_HPRD_TIME_L_hp_time_l_START (0)
#define SUB_PMIC_HPRD_TIME_L_hp_time_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_time_h : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_HPRD_TIME_H_UNION;
#endif
#define SUB_PMIC_HPRD_TIME_H_hp_time_h_START (0)
#define SUB_PMIC_HPRD_TIME_H_hp_time_h_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_duty_num : 8;
    } reg;
} SUB_PMIC_HPRD_DUTY_NUM_UNION;
#endif
#define SUB_PMIC_HPRD_DUTY_NUM_hp_duty_num_START (0)
#define SUB_PMIC_HPRD_DUTY_NUM_hp_duty_num_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_rank_num_l : 8;
    } reg;
} SUB_PMIC_HPRD_RANK_NUM_L_UNION;
#endif
#define SUB_PMIC_HPRD_RANK_NUM_L_hp_rank_num_l_START (0)
#define SUB_PMIC_HPRD_RANK_NUM_L_hp_rank_num_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_rank_num_h : 8;
    } reg;
} SUB_PMIC_HPRD_RANK_NUM_H_UNION;
#endif
#define SUB_PMIC_HPRD_RANK_NUM_H_hp_rank_num_h_START (0)
#define SUB_PMIC_HPRD_RANK_NUM_H_hp_rank_num_h_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_ocp_num : 8;
    } reg;
} SUB_PMIC_HPRD_OCP_NUM_UNION;
#endif
#define SUB_PMIC_HPRD_OCP_NUM_hp_ocp_num_START (0)
#define SUB_PMIC_HPRD_OCP_NUM_hp_ocp_num_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hp_fsm : 4;
        unsigned char hp_bemf_out : 1;
        unsigned char hp_tmax : 1;
        unsigned char hp_t1 : 1;
        unsigned char hp_drv_drec : 1;
    } reg;
} SUB_PMIC_HPRD_FSM_UNION;
#endif
#define SUB_PMIC_HPRD_FSM_hp_fsm_START (0)
#define SUB_PMIC_HPRD_FSM_hp_fsm_END (3)
#define SUB_PMIC_HPRD_FSM_hp_bemf_out_START (4)
#define SUB_PMIC_HPRD_FSM_hp_bemf_out_END (4)
#define SUB_PMIC_HPRD_FSM_hp_tmax_START (5)
#define SUB_PMIC_HPRD_FSM_hp_tmax_END (5)
#define SUB_PMIC_HPRD_FSM_hp_t1_START (6)
#define SUB_PMIC_HPRD_FSM_hp_t1_END (6)
#define SUB_PMIC_HPRD_FSM_hp_drv_drec_START (7)
#define SUB_PMIC_HPRD_FSM_hp_drv_drec_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_debug_en : 8;
    } reg;
} SUB_PMIC_LRA_DEBUG_EN_UNION;
#endif
#define SUB_PMIC_LRA_DEBUG_EN_lra_debug_en_START (0)
#define SUB_PMIC_LRA_DEBUG_EN_lra_debug_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dbg_lra_ana_pre_en : 1;
        unsigned char dbg_lra_ana_drv_en : 1;
        unsigned char dbg_lra_adc_en : 1;
        unsigned char dbg_lra_adc_start : 1;
        unsigned char dbg_adc_chanl_sel : 2;
        unsigned char reserved : 2;
    } reg;
} SUB_PMIC_DEBUG_ADC_CTRL_UNION;
#endif
#define SUB_PMIC_DEBUG_ADC_CTRL_dbg_lra_ana_pre_en_START (0)
#define SUB_PMIC_DEBUG_ADC_CTRL_dbg_lra_ana_pre_en_END (0)
#define SUB_PMIC_DEBUG_ADC_CTRL_dbg_lra_ana_drv_en_START (1)
#define SUB_PMIC_DEBUG_ADC_CTRL_dbg_lra_ana_drv_en_END (1)
#define SUB_PMIC_DEBUG_ADC_CTRL_dbg_lra_adc_en_START (2)
#define SUB_PMIC_DEBUG_ADC_CTRL_dbg_lra_adc_en_END (2)
#define SUB_PMIC_DEBUG_ADC_CTRL_dbg_lra_adc_start_START (3)
#define SUB_PMIC_DEBUG_ADC_CTRL_dbg_lra_adc_start_END (3)
#define SUB_PMIC_DEBUG_ADC_CTRL_dbg_adc_chanl_sel_START (4)
#define SUB_PMIC_DEBUG_ADC_CTRL_dbg_adc_chanl_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dbg_lra_adc_vld : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_DEBUG_ADC_VALID_UNION;
#endif
#define SUB_PMIC_DEBUG_ADC_VALID_dbg_lra_adc_vld_START (0)
#define SUB_PMIC_DEBUG_ADC_VALID_dbg_lra_adc_vld_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dbg_lra_adc_data_l : 8;
    } reg;
} SUB_PMIC_DEBUG_ADC_DATA_L_UNION;
#endif
#define SUB_PMIC_DEBUG_ADC_DATA_L_dbg_lra_adc_data_l_START (0)
#define SUB_PMIC_DEBUG_ADC_DATA_L_dbg_lra_adc_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dbg_lra_adc_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_DEBUG_ADC_DATA_H_UNION;
#endif
#define SUB_PMIC_DEBUG_ADC_DATA_H_dbg_lra_adc_data_h_START (0)
#define SUB_PMIC_DEBUG_ADC_DATA_H_dbg_lra_adc_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_clk_gate_bp : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_LRA_CLK_G_CTRL_UNION;
#endif
#define SUB_PMIC_LRA_CLK_G_CTRL_lra_clk_gate_bp_START (0)
#define SUB_PMIC_LRA_CLK_G_CTRL_lra_clk_gate_bp_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_nml_16hpt_l : 8;
    } reg;
} SUB_PMIC_LRA_NML_TIME_L_UNION;
#endif
#define SUB_PMIC_LRA_NML_TIME_L_lra_nml_16hpt_l_START (0)
#define SUB_PMIC_LRA_NML_TIME_L_lra_nml_16hpt_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_nml_16hpt_m : 8;
    } reg;
} SUB_PMIC_LRA_NML_TIME_M_UNION;
#endif
#define SUB_PMIC_LRA_NML_TIME_M_lra_nml_16hpt_m_START (0)
#define SUB_PMIC_LRA_NML_TIME_M_lra_nml_16hpt_m_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_nml_16hpt_h : 8;
    } reg;
} SUB_PMIC_LRA_NML_TIME_H_UNION;
#endif
#define SUB_PMIC_LRA_NML_TIME_H_lra_nml_16hpt_h_START (0)
#define SUB_PMIC_LRA_NML_TIME_H_lra_nml_16hpt_h_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_nml_16hpb_l : 8;
    } reg;
} SUB_PMIC_LRA_NML_BEMF_L_UNION;
#endif
#define SUB_PMIC_LRA_NML_BEMF_L_lra_nml_16hpb_l_START (0)
#define SUB_PMIC_LRA_NML_BEMF_L_lra_nml_16hpb_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_nml_16hpb_h : 8;
    } reg;
} SUB_PMIC_LRA_NML_BEMF_H_UNION;
#endif
#define SUB_PMIC_LRA_NML_BEMF_H_lra_nml_16hpb_h_START (0)
#define SUB_PMIC_LRA_NML_BEMF_H_lra_nml_16hpb_h_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lrag_ctrl_sel : 8;
    } reg;
} SUB_PMIC_LRA_GATE_CTRL_REG_UNION;
#endif
#define SUB_PMIC_LRA_GATE_CTRL_REG_lrag_ctrl_sel_START (0)
#define SUB_PMIC_LRA_GATE_CTRL_REG_lrag_ctrl_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_duty_sel : 8;
    } reg;
} SUB_PMIC_LRA_DUTYRES_REG_UNION;
#endif
#define SUB_PMIC_LRA_DUTYRES_REG_lra_duty_sel_START (0)
#define SUB_PMIC_LRA_DUTYRES_REG_lra_duty_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_bemf_zero_ok : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_LRA_BFST_OK_REG_UNION;
#endif
#define SUB_PMIC_LRA_BFST_OK_REG_lra_bemf_zero_ok_START (0)
#define SUB_PMIC_LRA_BFST_OK_REG_lra_bemf_zero_ok_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_bemf_zero_l : 8;
    } reg;
} SUB_PMIC_LRA_BEMF_FST_REG_LOW_UNION;
#endif
#define SUB_PMIC_LRA_BEMF_FST_REG_LOW_lra_bemf_zero_l_START (0)
#define SUB_PMIC_LRA_BEMF_FST_REG_LOW_lra_bemf_zero_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lra_bemf_zero_h : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_LRA_BEMF_FST_REG_HIGH_UNION;
#endif
#define SUB_PMIC_LRA_BEMF_FST_REG_HIGH_lra_bemf_zero_h_START (0)
#define SUB_PMIC_LRA_BEMF_FST_REG_HIGH_lra_bemf_zero_h_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lbemf_dete_th_l : 8;
    } reg;
} SUB_PMIC_LRA_OFF_L_UNION;
#endif
#define SUB_PMIC_LRA_OFF_L_lbemf_dete_th_l_START (0)
#define SUB_PMIC_LRA_OFF_L_lbemf_dete_th_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lbemf_dete_th_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_LRA_OFF_H_UNION;
#endif
#define SUB_PMIC_LRA_OFF_H_lbemf_dete_th_h_START (0)
#define SUB_PMIC_LRA_OFF_H_lbemf_dete_th_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char zero_offset_time : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_OFFSET_TIME_UNION;
#endif
#define SUB_PMIC_OFFSET_TIME_zero_offset_time_START (0)
#define SUB_PMIC_OFFSET_TIME_zero_offset_time_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_mode_lock : 8;
    } reg;
} SUB_PMIC_WE_ERM_LOCK_UNION;
#endif
#define SUB_PMIC_WE_ERM_LOCK_erm_mode_lock_START (0)
#define SUB_PMIC_WE_ERM_LOCK_erm_mode_lock_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_mode : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_ERM_MODE_CFG_UNION;
#endif
#define SUB_PMIC_ERM_MODE_CFG_erm_mode_START (0)
#define SUB_PMIC_ERM_MODE_CFG_erm_mode_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char frqc_sel : 2;
        unsigned char reserved : 6;
    } reg;
} SUB_PMIC_FRQC_SEL_CFG_UNION;
#endif
#define SUB_PMIC_FRQC_SEL_CFG_frqc_sel_START (0)
#define SUB_PMIC_FRQC_SEL_CFG_frqc_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_duty_ovdr_l : 8;
    } reg;
} SUB_PMIC_ERM_DUTY_OVDR_CFG_L_UNION;
#endif
#define SUB_PMIC_ERM_DUTY_OVDR_CFG_L_erm_duty_ovdr_l_START (0)
#define SUB_PMIC_ERM_DUTY_OVDR_CFG_L_erm_duty_ovdr_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_duty_ovdr_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_ERM_DUTY_OVDR_CFG_H_UNION;
#endif
#define SUB_PMIC_ERM_DUTY_OVDR_CFG_H_erm_duty_ovdr_h_START (0)
#define SUB_PMIC_ERM_DUTY_OVDR_CFG_H_erm_duty_ovdr_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_duty_nml_l : 8;
    } reg;
} SUB_PMIC_ERM_DUTY_NML_CFG_L_UNION;
#endif
#define SUB_PMIC_ERM_DUTY_NML_CFG_L_erm_duty_nml_l_START (0)
#define SUB_PMIC_ERM_DUTY_NML_CFG_L_erm_duty_nml_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_duty_nml_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_ERM_DUTY_NML_CFG_H_UNION;
#endif
#define SUB_PMIC_ERM_DUTY_NML_CFG_H_erm_duty_nml_h_START (0)
#define SUB_PMIC_ERM_DUTY_NML_CFG_H_erm_duty_nml_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_duty_brk_l : 8;
    } reg;
} SUB_PMIC_ERM_DUTY_BRK_CFG_L_UNION;
#endif
#define SUB_PMIC_ERM_DUTY_BRK_CFG_L_erm_duty_brk_l_START (0)
#define SUB_PMIC_ERM_DUTY_BRK_CFG_L_erm_duty_brk_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_duty_brk_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_ERM_DUTY_BRK_CFG_H_UNION;
#endif
#define SUB_PMIC_ERM_DUTY_BRK_CFG_H_erm_duty_brk_h_START (0)
#define SUB_PMIC_ERM_DUTY_BRK_CFG_H_erm_duty_brk_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_k_eob_l : 8;
    } reg;
} SUB_PMIC_ERM_K_EOB_L_UNION;
#endif
#define SUB_PMIC_ERM_K_EOB_L_erm_k_eob_l_START (0)
#define SUB_PMIC_ERM_K_EOB_L_erm_k_eob_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_k_eob_h : 8;
    } reg;
} SUB_PMIC_ERM_K_EOB_H_UNION;
#endif
#define SUB_PMIC_ERM_K_EOB_H_erm_k_eob_h_START (0)
#define SUB_PMIC_ERM_K_EOB_H_erm_k_eob_h_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_ovdr_nlow : 3;
        unsigned char erm_ovdr_nhigh : 5;
    } reg;
} SUB_PMIC_ERM_OVDR_NUM_UNION;
#endif
#define SUB_PMIC_ERM_OVDR_NUM_erm_ovdr_nlow_START (0)
#define SUB_PMIC_ERM_OVDR_NUM_erm_ovdr_nlow_END (2)
#define SUB_PMIC_ERM_OVDR_NUM_erm_ovdr_nhigh_START (3)
#define SUB_PMIC_ERM_OVDR_NUM_erm_ovdr_nhigh_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_brk_nlow : 3;
        unsigned char erm_brk_nhigh : 5;
    } reg;
} SUB_PMIC_ERM_BRK_NUM_UNION;
#endif
#define SUB_PMIC_ERM_BRK_NUM_erm_brk_nlow_START (0)
#define SUB_PMIC_ERM_BRK_NUM_erm_brk_nlow_END (2)
#define SUB_PMIC_ERM_BRK_NUM_erm_brk_nhigh_START (3)
#define SUB_PMIC_ERM_BRK_NUM_erm_brk_nhigh_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_prd_time : 8;
    } reg;
} SUB_PMIC_ERM_DRT_CFG_UNION;
#endif
#define SUB_PMIC_ERM_DRT_CFG_erm_prd_time_START (0)
#define SUB_PMIC_ERM_DRT_CFG_erm_prd_time_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_dete_time : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_ERM_DETE_TIME_CFG_UNION;
#endif
#define SUB_PMIC_ERM_DETE_TIME_CFG_erm_dete_time_START (0)
#define SUB_PMIC_ERM_DETE_TIME_CFG_erm_dete_time_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_twait : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_ERM_TWAIT_CFG_UNION;
#endif
#define SUB_PMIC_ERM_TWAIT_CFG_erm_twait_START (0)
#define SUB_PMIC_ERM_TWAIT_CFG_erm_twait_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_ovdr_thre_l : 8;
    } reg;
} SUB_PMIC_ERM_OVDR_THRE_L_UNION;
#endif
#define SUB_PMIC_ERM_OVDR_THRE_L_erm_ovdr_thre_l_START (0)
#define SUB_PMIC_ERM_OVDR_THRE_L_erm_ovdr_thre_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_ovdr_thre_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_ERM_OVDR_THRE_H_UNION;
#endif
#define SUB_PMIC_ERM_OVDR_THRE_H_erm_ovdr_thre_h_START (0)
#define SUB_PMIC_ERM_OVDR_THRE_H_erm_ovdr_thre_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_brk_thre_l : 8;
    } reg;
} SUB_PMIC_ERM_BRK_THRE_L_UNION;
#endif
#define SUB_PMIC_ERM_BRK_THRE_L_erm_brk_thre_l_START (0)
#define SUB_PMIC_ERM_BRK_THRE_L_erm_brk_thre_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_brk_thre_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_ERM_BRK_THRE_H_UNION;
#endif
#define SUB_PMIC_ERM_BRK_THRE_H_erm_brk_thre_h_START (0)
#define SUB_PMIC_ERM_BRK_THRE_H_erm_brk_thre_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_opl_ovdr_n : 8;
    } reg;
} SUB_PMIC_ERM_OPL_OVDR_NUM_UNION;
#endif
#define SUB_PMIC_ERM_OPL_OVDR_NUM_erm_opl_ovdr_n_START (0)
#define SUB_PMIC_ERM_OPL_OVDR_NUM_erm_opl_ovdr_n_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_opl_brk_n : 8;
    } reg;
} SUB_PMIC_ERM_OPL_BRK_NUM_UNION;
#endif
#define SUB_PMIC_ERM_OPL_BRK_NUM_erm_opl_brk_n_START (0)
#define SUB_PMIC_ERM_OPL_BRK_NUM_erm_opl_brk_n_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_abn_adc_ave : 3;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_ERM_ADCAVE_CFG_UNION;
#endif
#define SUB_PMIC_ERM_ADCAVE_CFG_erm_abn_adc_ave_START (0)
#define SUB_PMIC_ERM_ADCAVE_CFG_erm_abn_adc_ave_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_abn_dete_en : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_ERM_ABN_ONOFF_UNION;
#endif
#define SUB_PMIC_ERM_ABN_ONOFF_erm_abn_dete_en_START (0)
#define SUB_PMIC_ERM_ABN_ONOFF_erm_abn_dete_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_abn_thre_l : 8;
    } reg;
} SUB_PMIC_ERM_ABN_TH_L_UNION;
#endif
#define SUB_PMIC_ERM_ABN_TH_L_erm_abn_thre_l_START (0)
#define SUB_PMIC_ERM_ABN_TH_L_erm_abn_thre_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_abn_thre_h : 5;
        unsigned char reserved : 3;
    } reg;
} SUB_PMIC_ERM_ABN_TH_H_UNION;
#endif
#define SUB_PMIC_ERM_ABN_TH_H_erm_abn_thre_h_START (0)
#define SUB_PMIC_ERM_ABN_TH_H_erm_abn_thre_h_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ebemf_dete_th_l : 8;
    } reg;
} SUB_PMIC_ERM_OFF_L_UNION;
#endif
#define SUB_PMIC_ERM_OFF_L_ebemf_dete_th_l_START (0)
#define SUB_PMIC_ERM_OFF_L_ebemf_dete_th_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ebemf_dete_th_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_ERM_OFF_H_UNION;
#endif
#define SUB_PMIC_ERM_OFF_H_ebemf_dete_th_h_START (0)
#define SUB_PMIC_ERM_OFF_H_ebemf_dete_th_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_ocp_delay : 3;
        unsigned char reserved : 5;
    } reg;
} SUB_PMIC_ERM_ANA_CFG0_UNION;
#endif
#define SUB_PMIC_ERM_ANA_CFG0_erm_ocp_delay_START (0)
#define SUB_PMIC_ERM_ANA_CFG0_erm_ocp_delay_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_smpl_en : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_ERM_PRD_SMPL_EN_UNION;
#endif
#define SUB_PMIC_ERM_PRD_SMPL_EN_erm_smpl_en_START (0)
#define SUB_PMIC_ERM_PRD_SMPL_EN_erm_smpl_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_smpl_valid : 1;
        unsigned char reserved : 7;
    } reg;
} SUB_PMIC_ERM_PRD_SMPL_VALID_UNION;
#endif
#define SUB_PMIC_ERM_PRD_SMPL_VALID_erm_smpl_valid_START (0)
#define SUB_PMIC_ERM_PRD_SMPL_VALID_erm_smpl_valid_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_p_bemf_ave_l : 8;
    } reg;
} SUB_PMIC_ERM_PRD_BEMF_L_UNION;
#endif
#define SUB_PMIC_ERM_PRD_BEMF_L_erm_p_bemf_ave_l_START (0)
#define SUB_PMIC_ERM_PRD_BEMF_L_erm_p_bemf_ave_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char erm_p_bemf_ave_h : 4;
        unsigned char reserved : 4;
    } reg;
} SUB_PMIC_ERM_PRD_BEMF_H_UNION;
#endif
#define SUB_PMIC_ERM_PRD_BEMF_H_erm_p_bemf_ave_h_START (0)
#define SUB_PMIC_ERM_PRD_BEMF_H_erm_p_bemf_ave_h_END (3)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
