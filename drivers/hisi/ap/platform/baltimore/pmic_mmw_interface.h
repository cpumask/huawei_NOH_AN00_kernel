#ifndef __PMIC_MMW_MMW_INTERFACE_H__
#define __PMIC_MMW_MMW_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_MMW_VERSION0_ADDR(base) ((base) + (0x000UL))
#define PMIC_MMW_VERSION1_ADDR(base) ((base) + (0x001UL))
#define PMIC_MMW_VERSION2_ADDR(base) ((base) + (0x002UL))
#define PMIC_MMW_VERSION3_ADDR(base) ((base) + (0x003UL))
#define PMIC_MMW_VERSION4_ADDR(base) ((base) + (0x004UL))
#define PMIC_MMW_VERSION5_ADDR(base) ((base) + (0x005UL))
#define PMIC_MMW_STATUS0_ADDR(base) ((base) + (0x006UL))
#define PMIC_MMW_STATUS1_ADDR(base) ((base) + (0x007UL))
#define PMIC_MMW_LDO0_ONOFF_ADDR(base) ((base) + (0x00AUL))
#define PMIC_MMW_LDO1_ONOFF_ADDR(base) ((base) + (0x00BUL))
#define PMIC_MMW_LDO2_ONOFF_ADDR(base) ((base) + (0x00CUL))
#define PMIC_MMW_LDO3_ONOFF_ADDR(base) ((base) + (0x00DUL))
#define PMIC_MMW_LDO4_ONOFF_ADDR(base) ((base) + (0x00EUL))
#define PMIC_MMW_LDO5_ONOFF_ADDR(base) ((base) + (0x00FUL))
#define PMIC_MMW_LDO6_ONOFF_ADDR(base) ((base) + (0x010UL))
#define PMIC_MMW_LDO7_ONOFF_ECO_ADDR(base) ((base) + (0x011UL))
#define PMIC_MMW_LDO8_ONOFF_ECO_ADDR(base) ((base) + (0x012UL))
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_ADDR(base) ((base) + (0x013UL))
#define PMIC_MMW_LDO_BUF_RF_ONOFF_ADDR(base) ((base) + (0x014UL))
#define PMIC_MMW_LDO_BUF0_ONOFF_ADDR(base) ((base) + (0x015UL))
#define PMIC_MMW_LDO_BUF1_ONOFF_ADDR(base) ((base) + (0x016UL))
#define PMIC_MMW_LDO_RFBUF0_ONOFF_ADDR(base) ((base) + (0x017UL))
#define PMIC_MMW_LDO_RFBUF1_ONOFF_ADDR(base) ((base) + (0x018UL))
#define PMIC_MMW_LDO_RFBUF2_ONOFF_ADDR(base) ((base) + (0x019UL))
#define PMIC_MMW_LDO_RFBUF3_ONOFF_ADDR(base) ((base) + (0x01AUL))
#define PMIC_MMW_LDO_RFBUF4_ONOFF_ADDR(base) ((base) + (0x01BUL))
#define PMIC_MMW_PMUH_ONOFF_ADDR(base) ((base) + (0x01CUL))
#define PMIC_MMW_XO_CORE_ONOFF_ADDR(base) ((base) + (0x01DUL))
#define PMIC_MMW_THSD_ONOFF_ECO_ADDR(base) ((base) + (0x01EUL))
#define PMIC_MMW_CLK_UFS_EN_ADDR(base) ((base) + (0x020UL))
#define PMIC_MMW_CLK_RF0_EN_ADDR(base) ((base) + (0x021UL))
#define PMIC_MMW_CLK_RF1_EN_ADDR(base) ((base) + (0x022UL))
#define PMIC_MMW_CLK_RF2_EN_ADDR(base) ((base) + (0x023UL))
#define PMIC_MMW_CLK_RF3_EN_ADDR(base) ((base) + (0x024UL))
#define PMIC_MMW_CLK_RF4_EN_ADDR(base) ((base) + (0x025UL))
#define PMIC_MMW_CLK_SERDES_EN_ADDR(base) ((base) + (0x026UL))
#define PMIC_MMW_CLK_WIFI0_EN_ADDR(base) ((base) + (0x027UL))
#define PMIC_MMW_CLK_WIFI1_EN_ADDR(base) ((base) + (0x028UL))
#define PMIC_MMW_CLK_NFC_EN_ADDR(base) ((base) + (0x029UL))
#define PMIC_MMW_CLK_SYS_EN_ADDR(base) ((base) + (0x02AUL))
#define PMIC_MMW_CLK_PMU_EN_ADDR(base) ((base) + (0x02BUL))
#define PMIC_MMW_OSC32K_SYS_ONOFF_CTRL_ADDR(base) ((base) + (0x02CUL))
#define PMIC_MMW_LDO0_VSET_ADDR(base) ((base) + (0x02DUL))
#define PMIC_MMW_LDO1_VSET_ADDR(base) ((base) + (0x02EUL))
#define PMIC_MMW_LDO2_VSET_ADDR(base) ((base) + (0x02FUL))
#define PMIC_MMW_LDO3_VSET_ADDR(base) ((base) + (0x030UL))
#define PMIC_MMW_LDO4_VSET_ADDR(base) ((base) + (0x031UL))
#define PMIC_MMW_LDO5_VSET_ADDR(base) ((base) + (0x032UL))
#define PMIC_MMW_LDO6_VSET_ADDR(base) ((base) + (0x033UL))
#define PMIC_MMW_LDO7_VSET_ADDR(base) ((base) + (0x034UL))
#define PMIC_MMW_LDO8_VSET_ADDR(base) ((base) + (0x035UL))
#define PMIC_MMW_LDO_CORE_VSET_ADDR(base) ((base) + (0x036UL))
#define PMIC_MMW_LDO_CORE_VSET_ECO_ADDR(base) ((base) + (0x037UL))
#define PMIC_MMW_LDO_BUF_RF_VSET_ADDR(base) ((base) + (0x038UL))
#define PMIC_MMW_LDO_BUF0_VSET_ADDR(base) ((base) + (0x039UL))
#define PMIC_MMW_LDO_BUF1_VSET_ADDR(base) ((base) + (0x03AUL))
#define PMIC_MMW_LDO_RFBUF0_VSET_ADDR(base) ((base) + (0x03BUL))
#define PMIC_MMW_LDO_RFBUF1_VSET_ADDR(base) ((base) + (0x03CUL))
#define PMIC_MMW_LDO_RFBUF2_VSET_ADDR(base) ((base) + (0x03DUL))
#define PMIC_MMW_LDO_RFBUF3_VSET_ADDR(base) ((base) + (0x03EUL))
#define PMIC_MMW_LDO_RFBUF4_VSET_ADDR(base) ((base) + (0x03FUL))
#define PMIC_MMW_LDO_PMUH_VSET_ADDR(base) ((base) + (0x040UL))
#define PMIC_MMW_XO_CORE_CURR_ADDR(base) ((base) + (0x041UL))
#define PMIC_MMW_XO_CORE_CURR_ECO_ADDR(base) ((base) + (0x042UL))
#define PMIC_MMW_XO_INBUFF_CURR_ADDR(base) ((base) + (0x043UL))
#define PMIC_MMW_XO_INBUFF_CURR_ECO_ADDR(base) ((base) + (0x044UL))
#define PMIC_MMW_LDO0_CFG_REG_0_ADDR(base) ((base) + (0x045UL))
#define PMIC_MMW_LDO0_CFG_REG_1_ADDR(base) ((base) + (0x046UL))
#define PMIC_MMW_LDO0_CFG_REG_2_ADDR(base) ((base) + (0x047UL))
#define PMIC_MMW_LDO1_CFG_REG_0_ADDR(base) ((base) + (0x048UL))
#define PMIC_MMW_LDO1_CFG_REG_1_ADDR(base) ((base) + (0x049UL))
#define PMIC_MMW_LDO1_CFG_REG_2_ADDR(base) ((base) + (0x04AUL))
#define PMIC_MMW_LDO2_CFG_REG_0_ADDR(base) ((base) + (0x04BUL))
#define PMIC_MMW_LDO2_CFG_REG_1_ADDR(base) ((base) + (0x04CUL))
#define PMIC_MMW_LDO2_CFG_REG_2_ADDR(base) ((base) + (0x04DUL))
#define PMIC_MMW_LDO3_CFG_REG_0_ADDR(base) ((base) + (0x04EUL))
#define PMIC_MMW_LDO3_CFG_REG_1_ADDR(base) ((base) + (0x04FUL))
#define PMIC_MMW_LDO3_CFG_REG_2_ADDR(base) ((base) + (0x050UL))
#define PMIC_MMW_LDO4_CFG_REG_0_ADDR(base) ((base) + (0x051UL))
#define PMIC_MMW_LDO4_CFG_REG_1_ADDR(base) ((base) + (0x052UL))
#define PMIC_MMW_LDO4_CFG_REG_2_ADDR(base) ((base) + (0x053UL))
#define PMIC_MMW_LDO5_CFG_REG_0_ADDR(base) ((base) + (0x054UL))
#define PMIC_MMW_LDO5_CFG_REG_1_ADDR(base) ((base) + (0x055UL))
#define PMIC_MMW_LDO6_CFG_REG_0_ADDR(base) ((base) + (0x056UL))
#define PMIC_MMW_LDO6_CFG_REG_1_ADDR(base) ((base) + (0x057UL))
#define PMIC_MMW_LDO7_CFG_REG_0_ADDR(base) ((base) + (0x058UL))
#define PMIC_MMW_LDO7_CFG_REG_1_ADDR(base) ((base) + (0x059UL))
#define PMIC_MMW_LDO8_CFG_REG_0_ADDR(base) ((base) + (0x05AUL))
#define PMIC_MMW_LDO8_CFG_REG_1_ADDR(base) ((base) + (0x05BUL))
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ADDR(base) ((base) + (0x05CUL))
#define PMIC_MMW_LDO_BUF0_CFG_REG_0_ADDR(base) ((base) + (0x05DUL))
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ADDR(base) ((base) + (0x05EUL))
#define PMIC_MMW_LDO_BUF_RF_CFG_REG_0_ADDR(base) ((base) + (0x05FUL))
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ADDR(base) ((base) + (0x060UL))
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ADDR(base) ((base) + (0x061UL))
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ADDR(base) ((base) + (0x062UL))
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ADDR(base) ((base) + (0x063UL))
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ADDR(base) ((base) + (0x064UL))
#define PMIC_MMW_PMUH_CFG_REG_0_ADDR(base) ((base) + (0x065UL))
#define PMIC_MMW_LDO_RESERVE_CFG_REG_0_ADDR(base) ((base) + (0x066UL))
#define PMIC_MMW_LDO_RESERVE_CFG_REG_1_ADDR(base) ((base) + (0x067UL))
#define PMIC_MMW_LDO_RESERVE_CFG_REG_2_ADDR(base) ((base) + (0x068UL))
#define PMIC_MMW_LDO_RESERVE_CFG_REG_3_ADDR(base) ((base) + (0x069UL))
#define PMIC_MMW_CLK_UFS_CTRL_ADDR(base) ((base) + (0x06AUL))
#define PMIC_MMW_CLK_RF0_CTRL_ADDR(base) ((base) + (0x06BUL))
#define PMIC_MMW_CLK_RF1_CTRL_ADDR(base) ((base) + (0x06CUL))
#define PMIC_MMW_CLK_RF2_CTRL_ADDR(base) ((base) + (0x06DUL))
#define PMIC_MMW_CLK_RF3_CTRL_ADDR(base) ((base) + (0x06EUL))
#define PMIC_MMW_CLK_RF4_CTRL_ADDR(base) ((base) + (0x06FUL))
#define PMIC_MMW_CLK_SERDES_CTRL_ADDR(base) ((base) + (0x070UL))
#define PMIC_MMW_CLK_WIFI0_CTRL_ADDR(base) ((base) + (0x071UL))
#define PMIC_MMW_CLK_WIFI1_CTRL_ADDR(base) ((base) + (0x072UL))
#define PMIC_MMW_CLK_NFC_CTRL_ADDR(base) ((base) + (0x073UL))
#define PMIC_MMW_CLK_SYS_CTRL_ADDR(base) ((base) + (0x074UL))
#define PMIC_MMW_CLK_PMU_CTRL_ADDR(base) ((base) + (0x075UL))
#define PMIC_MMW_XO_CTRL0_ADDR(base) ((base) + (0x076UL))
#define PMIC_MMW_XO_RESERVE_0_ADDR(base) ((base) + (0x077UL))
#define PMIC_MMW_XO_DET_STATUS_ADDR(base) ((base) + (0x078UL))
#define PMIC_MMW_BG_THSD_CTRL0_ADDR(base) ((base) + (0x079UL))
#define PMIC_MMW_BG_THSD_CTRL1_ADDR(base) ((base) + (0x07AUL))
#define PMIC_MMW_BG_RESERVE_0_ADDR(base) ((base) + (0x07BUL))
#define PMIC_MMW_BG_RESERVE_1_ADDR(base) ((base) + (0x07CUL))
#define PMIC_MMW_BG_RESERVE_2_ADDR(base) ((base) + (0x07DUL))
#define PMIC_MMW_SYS_CTRL_RESERVE_ADDR(base) ((base) + (0x07EUL))
#define PMIC_MMW_SYS_CTRL0_ADDR(base) ((base) + (0x07FUL))
#define PMIC_MMW_SYS_CTRL1_ADDR(base) ((base) + (0x080UL))
#define PMIC_MMW_A2D_RES0_ADDR(base) ((base) + (0x084UL))
#define PMIC_MMW_D2A_RES0_ADDR(base) ((base) + (0x085UL))
#define PMIC_MMW_D2A_RES1_ADDR(base) ((base) + (0x086UL))
#define PMIC_MMW_D2A_RES2_ADDR(base) ((base) + (0x087UL))
#define PMIC_MMW_D2A_RES3_ADDR(base) ((base) + (0x088UL))
#define PMIC_MMW_OTP_0_ADDR(base) ((base) + (0x089UL))
#define PMIC_MMW_OTP_1_ADDR(base) ((base) + (0x08AUL))
#define PMIC_MMW_OTP_CLK_CTRL_ADDR(base) ((base) + (0x08BUL))
#define PMIC_MMW_OTP_CTRL0_ADDR(base) ((base) + (0x08CUL))
#define PMIC_MMW_OTP_CTRL1_ADDR(base) ((base) + (0x08DUL))
#define PMIC_MMW_OTP_CTRL2_ADDR(base) ((base) + (0x08EUL))
#define PMIC_MMW_OTP_WDATA_ADDR(base) ((base) + (0x08FUL))
#define PMIC_MMW_OTP_0_W_ADDR(base) ((base) + (0x090UL))
#define PMIC_MMW_OTP_1_W_ADDR(base) ((base) + (0x091UL))
#define PMIC_MMW_OTP_2_W_ADDR(base) ((base) + (0x092UL))
#define PMIC_MMW_OTP_3_W_ADDR(base) ((base) + (0x093UL))
#define PMIC_MMW_OTP_4_W_ADDR(base) ((base) + (0x094UL))
#define PMIC_MMW_OTP_5_W_ADDR(base) ((base) + (0x095UL))
#define PMIC_MMW_OTP_6_W_ADDR(base) ((base) + (0x096UL))
#define PMIC_MMW_OTP_7_W_ADDR(base) ((base) + (0x097UL))
#define PMIC_MMW_OTP_8_W_ADDR(base) ((base) + (0x098UL))
#define PMIC_MMW_OTP_9_W_ADDR(base) ((base) + (0x099UL))
#define PMIC_MMW_OTP_10_W_ADDR(base) ((base) + (0x09AUL))
#define PMIC_MMW_OTP_11_W_ADDR(base) ((base) + (0x09BUL))
#define PMIC_MMW_OTP_12_W_ADDR(base) ((base) + (0x09CUL))
#define PMIC_MMW_OTP_13_W_ADDR(base) ((base) + (0x09DUL))
#define PMIC_MMW_OTP_14_W_ADDR(base) ((base) + (0x09EUL))
#define PMIC_MMW_OTP_15_W_ADDR(base) ((base) + (0x09FUL))
#define PMIC_MMW_OTP_16_W_ADDR(base) ((base) + (0x0A0UL))
#define PMIC_MMW_OTP_17_W_ADDR(base) ((base) + (0x0A1UL))
#define PMIC_MMW_OTP_18_W_ADDR(base) ((base) + (0x0A2UL))
#define PMIC_MMW_OTP_19_W_ADDR(base) ((base) + (0x0A3UL))
#define PMIC_MMW_OTP_20_W_ADDR(base) ((base) + (0x0A4UL))
#define PMIC_MMW_OTP_21_W_ADDR(base) ((base) + (0x0A5UL))
#define PMIC_MMW_LDO0_3_OCP_CTRL_ADDR(base) ((base) + (0x0A6UL))
#define PMIC_MMW_LDO4_7_OCP_CTRL_ADDR(base) ((base) + (0x0A7UL))
#define PMIC_MMW_LDO8_12_OCP_CTRL_ADDR(base) ((base) + (0x0A8UL))
#define PMIC_MMW_OCP_DEB_CTRL0_ADDR(base) ((base) + (0x0A9UL))
#define PMIC_MMW_OCP_DEB_CTRL1_ADDR(base) ((base) + (0x0AAUL))
#define PMIC_MMW_THSD_140_DEB_CTRL_ADDR(base) ((base) + (0x0ABUL))
#define PMIC_MMW_OCP_SCP_ONOFF_ADDR(base) ((base) + (0x0ACUL))
#define PMIC_MMW_HARDWIRE_CTRL0_ADDR(base) ((base) + (0x0ADUL))
#define PMIC_MMW_HARDWIRE_CTRL1_ADDR(base) ((base) + (0x0AEUL))
#define PMIC_MMW_HARDWIRE_CTRL2_ADDR(base) ((base) + (0x0AFUL))
#define PMIC_MMW_HARDWIRE_CTRL3_ADDR(base) ((base) + (0x0B0UL))
#define PMIC_MMW_HARDWIRE_CTRL4_ADDR(base) ((base) + (0x0B1UL))
#define PMIC_MMW_HARDWIRE_CTRL5_ADDR(base) ((base) + (0x0B2UL))
#define PMIC_MMW_LOCK_ADDR(base) ((base) + (0x0B3UL))
#define PMIC_MMW_SPMI_ECO_GT_BYPASS_ADDR(base) ((base) + (0x0B4UL))
#define PMIC_MMW_LDO_SST_END_CFG_ADDR(base) ((base) + (0x0B5UL))
#define PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_ADDR(base) ((base) + (0x0B6UL))
#define PMIC_MMW_XO_RF_CLK_ON_DEBUG_ADDR(base) ((base) + (0x0B7UL))
#define PMIC_MMW_XO_CORE_CURR_DEBUG_ADDR(base) ((base) + (0x0B8UL))
#define PMIC_MMW_XO_INBUFF_CURR_DEBUG_ADDR(base) ((base) + (0x0B9UL))
#define PMIC_MMW_SPMI_DEBUG0_ADDR(base) ((base) + (0x0F0UL))
#define PMIC_MMW_SPMI_DEBUG1_ADDR(base) ((base) + (0x0F1UL))
#define PMIC_MMW_SPMI_DEBUG2_ADDR(base) ((base) + (0x0F2UL))
#define PMIC_MMW_SPMI_DEBUG3_ADDR(base) ((base) + (0x0F3UL))
#define PMIC_MMW_SPMI_DEBUG4_ADDR(base) ((base) + (0x0F4UL))
#define PMIC_MMW_SPMI_DEBUG5_ADDR(base) ((base) + (0x0F5UL))
#define PMIC_MMW_SPMI_DEBUG6_ADDR(base) ((base) + (0x0F6UL))
#define PMIC_MMW_SPMI_DEBUG7_ADDR(base) ((base) + (0x0F7UL))
#else
#define PMIC_MMW_VERSION0_ADDR(base) ((base) + (0x000))
#define PMIC_MMW_VERSION1_ADDR(base) ((base) + (0x001))
#define PMIC_MMW_VERSION2_ADDR(base) ((base) + (0x002))
#define PMIC_MMW_VERSION3_ADDR(base) ((base) + (0x003))
#define PMIC_MMW_VERSION4_ADDR(base) ((base) + (0x004))
#define PMIC_MMW_VERSION5_ADDR(base) ((base) + (0x005))
#define PMIC_MMW_STATUS0_ADDR(base) ((base) + (0x006))
#define PMIC_MMW_STATUS1_ADDR(base) ((base) + (0x007))
#define PMIC_MMW_LDO0_ONOFF_ADDR(base) ((base) + (0x00A))
#define PMIC_MMW_LDO1_ONOFF_ADDR(base) ((base) + (0x00B))
#define PMIC_MMW_LDO2_ONOFF_ADDR(base) ((base) + (0x00C))
#define PMIC_MMW_LDO3_ONOFF_ADDR(base) ((base) + (0x00D))
#define PMIC_MMW_LDO4_ONOFF_ADDR(base) ((base) + (0x00E))
#define PMIC_MMW_LDO5_ONOFF_ADDR(base) ((base) + (0x00F))
#define PMIC_MMW_LDO6_ONOFF_ADDR(base) ((base) + (0x010))
#define PMIC_MMW_LDO7_ONOFF_ECO_ADDR(base) ((base) + (0x011))
#define PMIC_MMW_LDO8_ONOFF_ECO_ADDR(base) ((base) + (0x012))
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_ADDR(base) ((base) + (0x013))
#define PMIC_MMW_LDO_BUF_RF_ONOFF_ADDR(base) ((base) + (0x014))
#define PMIC_MMW_LDO_BUF0_ONOFF_ADDR(base) ((base) + (0x015))
#define PMIC_MMW_LDO_BUF1_ONOFF_ADDR(base) ((base) + (0x016))
#define PMIC_MMW_LDO_RFBUF0_ONOFF_ADDR(base) ((base) + (0x017))
#define PMIC_MMW_LDO_RFBUF1_ONOFF_ADDR(base) ((base) + (0x018))
#define PMIC_MMW_LDO_RFBUF2_ONOFF_ADDR(base) ((base) + (0x019))
#define PMIC_MMW_LDO_RFBUF3_ONOFF_ADDR(base) ((base) + (0x01A))
#define PMIC_MMW_LDO_RFBUF4_ONOFF_ADDR(base) ((base) + (0x01B))
#define PMIC_MMW_PMUH_ONOFF_ADDR(base) ((base) + (0x01C))
#define PMIC_MMW_XO_CORE_ONOFF_ADDR(base) ((base) + (0x01D))
#define PMIC_MMW_THSD_ONOFF_ECO_ADDR(base) ((base) + (0x01E))
#define PMIC_MMW_CLK_UFS_EN_ADDR(base) ((base) + (0x020))
#define PMIC_MMW_CLK_RF0_EN_ADDR(base) ((base) + (0x021))
#define PMIC_MMW_CLK_RF1_EN_ADDR(base) ((base) + (0x022))
#define PMIC_MMW_CLK_RF2_EN_ADDR(base) ((base) + (0x023))
#define PMIC_MMW_CLK_RF3_EN_ADDR(base) ((base) + (0x024))
#define PMIC_MMW_CLK_RF4_EN_ADDR(base) ((base) + (0x025))
#define PMIC_MMW_CLK_SERDES_EN_ADDR(base) ((base) + (0x026))
#define PMIC_MMW_CLK_WIFI0_EN_ADDR(base) ((base) + (0x027))
#define PMIC_MMW_CLK_WIFI1_EN_ADDR(base) ((base) + (0x028))
#define PMIC_MMW_CLK_NFC_EN_ADDR(base) ((base) + (0x029))
#define PMIC_MMW_CLK_SYS_EN_ADDR(base) ((base) + (0x02A))
#define PMIC_MMW_CLK_PMU_EN_ADDR(base) ((base) + (0x02B))
#define PMIC_MMW_OSC32K_SYS_ONOFF_CTRL_ADDR(base) ((base) + (0x02C))
#define PMIC_MMW_LDO0_VSET_ADDR(base) ((base) + (0x02D))
#define PMIC_MMW_LDO1_VSET_ADDR(base) ((base) + (0x02E))
#define PMIC_MMW_LDO2_VSET_ADDR(base) ((base) + (0x02F))
#define PMIC_MMW_LDO3_VSET_ADDR(base) ((base) + (0x030))
#define PMIC_MMW_LDO4_VSET_ADDR(base) ((base) + (0x031))
#define PMIC_MMW_LDO5_VSET_ADDR(base) ((base) + (0x032))
#define PMIC_MMW_LDO6_VSET_ADDR(base) ((base) + (0x033))
#define PMIC_MMW_LDO7_VSET_ADDR(base) ((base) + (0x034))
#define PMIC_MMW_LDO8_VSET_ADDR(base) ((base) + (0x035))
#define PMIC_MMW_LDO_CORE_VSET_ADDR(base) ((base) + (0x036))
#define PMIC_MMW_LDO_CORE_VSET_ECO_ADDR(base) ((base) + (0x037))
#define PMIC_MMW_LDO_BUF_RF_VSET_ADDR(base) ((base) + (0x038))
#define PMIC_MMW_LDO_BUF0_VSET_ADDR(base) ((base) + (0x039))
#define PMIC_MMW_LDO_BUF1_VSET_ADDR(base) ((base) + (0x03A))
#define PMIC_MMW_LDO_RFBUF0_VSET_ADDR(base) ((base) + (0x03B))
#define PMIC_MMW_LDO_RFBUF1_VSET_ADDR(base) ((base) + (0x03C))
#define PMIC_MMW_LDO_RFBUF2_VSET_ADDR(base) ((base) + (0x03D))
#define PMIC_MMW_LDO_RFBUF3_VSET_ADDR(base) ((base) + (0x03E))
#define PMIC_MMW_LDO_RFBUF4_VSET_ADDR(base) ((base) + (0x03F))
#define PMIC_MMW_LDO_PMUH_VSET_ADDR(base) ((base) + (0x040))
#define PMIC_MMW_XO_CORE_CURR_ADDR(base) ((base) + (0x041))
#define PMIC_MMW_XO_CORE_CURR_ECO_ADDR(base) ((base) + (0x042))
#define PMIC_MMW_XO_INBUFF_CURR_ADDR(base) ((base) + (0x043))
#define PMIC_MMW_XO_INBUFF_CURR_ECO_ADDR(base) ((base) + (0x044))
#define PMIC_MMW_LDO0_CFG_REG_0_ADDR(base) ((base) + (0x045))
#define PMIC_MMW_LDO0_CFG_REG_1_ADDR(base) ((base) + (0x046))
#define PMIC_MMW_LDO0_CFG_REG_2_ADDR(base) ((base) + (0x047))
#define PMIC_MMW_LDO1_CFG_REG_0_ADDR(base) ((base) + (0x048))
#define PMIC_MMW_LDO1_CFG_REG_1_ADDR(base) ((base) + (0x049))
#define PMIC_MMW_LDO1_CFG_REG_2_ADDR(base) ((base) + (0x04A))
#define PMIC_MMW_LDO2_CFG_REG_0_ADDR(base) ((base) + (0x04B))
#define PMIC_MMW_LDO2_CFG_REG_1_ADDR(base) ((base) + (0x04C))
#define PMIC_MMW_LDO2_CFG_REG_2_ADDR(base) ((base) + (0x04D))
#define PMIC_MMW_LDO3_CFG_REG_0_ADDR(base) ((base) + (0x04E))
#define PMIC_MMW_LDO3_CFG_REG_1_ADDR(base) ((base) + (0x04F))
#define PMIC_MMW_LDO3_CFG_REG_2_ADDR(base) ((base) + (0x050))
#define PMIC_MMW_LDO4_CFG_REG_0_ADDR(base) ((base) + (0x051))
#define PMIC_MMW_LDO4_CFG_REG_1_ADDR(base) ((base) + (0x052))
#define PMIC_MMW_LDO4_CFG_REG_2_ADDR(base) ((base) + (0x053))
#define PMIC_MMW_LDO5_CFG_REG_0_ADDR(base) ((base) + (0x054))
#define PMIC_MMW_LDO5_CFG_REG_1_ADDR(base) ((base) + (0x055))
#define PMIC_MMW_LDO6_CFG_REG_0_ADDR(base) ((base) + (0x056))
#define PMIC_MMW_LDO6_CFG_REG_1_ADDR(base) ((base) + (0x057))
#define PMIC_MMW_LDO7_CFG_REG_0_ADDR(base) ((base) + (0x058))
#define PMIC_MMW_LDO7_CFG_REG_1_ADDR(base) ((base) + (0x059))
#define PMIC_MMW_LDO8_CFG_REG_0_ADDR(base) ((base) + (0x05A))
#define PMIC_MMW_LDO8_CFG_REG_1_ADDR(base) ((base) + (0x05B))
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ADDR(base) ((base) + (0x05C))
#define PMIC_MMW_LDO_BUF0_CFG_REG_0_ADDR(base) ((base) + (0x05D))
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ADDR(base) ((base) + (0x05E))
#define PMIC_MMW_LDO_BUF_RF_CFG_REG_0_ADDR(base) ((base) + (0x05F))
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ADDR(base) ((base) + (0x060))
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ADDR(base) ((base) + (0x061))
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ADDR(base) ((base) + (0x062))
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ADDR(base) ((base) + (0x063))
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ADDR(base) ((base) + (0x064))
#define PMIC_MMW_PMUH_CFG_REG_0_ADDR(base) ((base) + (0x065))
#define PMIC_MMW_LDO_RESERVE_CFG_REG_0_ADDR(base) ((base) + (0x066))
#define PMIC_MMW_LDO_RESERVE_CFG_REG_1_ADDR(base) ((base) + (0x067))
#define PMIC_MMW_LDO_RESERVE_CFG_REG_2_ADDR(base) ((base) + (0x068))
#define PMIC_MMW_LDO_RESERVE_CFG_REG_3_ADDR(base) ((base) + (0x069))
#define PMIC_MMW_CLK_UFS_CTRL_ADDR(base) ((base) + (0x06A))
#define PMIC_MMW_CLK_RF0_CTRL_ADDR(base) ((base) + (0x06B))
#define PMIC_MMW_CLK_RF1_CTRL_ADDR(base) ((base) + (0x06C))
#define PMIC_MMW_CLK_RF2_CTRL_ADDR(base) ((base) + (0x06D))
#define PMIC_MMW_CLK_RF3_CTRL_ADDR(base) ((base) + (0x06E))
#define PMIC_MMW_CLK_RF4_CTRL_ADDR(base) ((base) + (0x06F))
#define PMIC_MMW_CLK_SERDES_CTRL_ADDR(base) ((base) + (0x070))
#define PMIC_MMW_CLK_WIFI0_CTRL_ADDR(base) ((base) + (0x071))
#define PMIC_MMW_CLK_WIFI1_CTRL_ADDR(base) ((base) + (0x072))
#define PMIC_MMW_CLK_NFC_CTRL_ADDR(base) ((base) + (0x073))
#define PMIC_MMW_CLK_SYS_CTRL_ADDR(base) ((base) + (0x074))
#define PMIC_MMW_CLK_PMU_CTRL_ADDR(base) ((base) + (0x075))
#define PMIC_MMW_XO_CTRL0_ADDR(base) ((base) + (0x076))
#define PMIC_MMW_XO_RESERVE_0_ADDR(base) ((base) + (0x077))
#define PMIC_MMW_XO_DET_STATUS_ADDR(base) ((base) + (0x078))
#define PMIC_MMW_BG_THSD_CTRL0_ADDR(base) ((base) + (0x079))
#define PMIC_MMW_BG_THSD_CTRL1_ADDR(base) ((base) + (0x07A))
#define PMIC_MMW_BG_RESERVE_0_ADDR(base) ((base) + (0x07B))
#define PMIC_MMW_BG_RESERVE_1_ADDR(base) ((base) + (0x07C))
#define PMIC_MMW_BG_RESERVE_2_ADDR(base) ((base) + (0x07D))
#define PMIC_MMW_SYS_CTRL_RESERVE_ADDR(base) ((base) + (0x07E))
#define PMIC_MMW_SYS_CTRL0_ADDR(base) ((base) + (0x07F))
#define PMIC_MMW_SYS_CTRL1_ADDR(base) ((base) + (0x080))
#define PMIC_MMW_A2D_RES0_ADDR(base) ((base) + (0x084))
#define PMIC_MMW_D2A_RES0_ADDR(base) ((base) + (0x085))
#define PMIC_MMW_D2A_RES1_ADDR(base) ((base) + (0x086))
#define PMIC_MMW_D2A_RES2_ADDR(base) ((base) + (0x087))
#define PMIC_MMW_D2A_RES3_ADDR(base) ((base) + (0x088))
#define PMIC_MMW_OTP_0_ADDR(base) ((base) + (0x089))
#define PMIC_MMW_OTP_1_ADDR(base) ((base) + (0x08A))
#define PMIC_MMW_OTP_CLK_CTRL_ADDR(base) ((base) + (0x08B))
#define PMIC_MMW_OTP_CTRL0_ADDR(base) ((base) + (0x08C))
#define PMIC_MMW_OTP_CTRL1_ADDR(base) ((base) + (0x08D))
#define PMIC_MMW_OTP_CTRL2_ADDR(base) ((base) + (0x08E))
#define PMIC_MMW_OTP_WDATA_ADDR(base) ((base) + (0x08F))
#define PMIC_MMW_OTP_0_W_ADDR(base) ((base) + (0x090))
#define PMIC_MMW_OTP_1_W_ADDR(base) ((base) + (0x091))
#define PMIC_MMW_OTP_2_W_ADDR(base) ((base) + (0x092))
#define PMIC_MMW_OTP_3_W_ADDR(base) ((base) + (0x093))
#define PMIC_MMW_OTP_4_W_ADDR(base) ((base) + (0x094))
#define PMIC_MMW_OTP_5_W_ADDR(base) ((base) + (0x095))
#define PMIC_MMW_OTP_6_W_ADDR(base) ((base) + (0x096))
#define PMIC_MMW_OTP_7_W_ADDR(base) ((base) + (0x097))
#define PMIC_MMW_OTP_8_W_ADDR(base) ((base) + (0x098))
#define PMIC_MMW_OTP_9_W_ADDR(base) ((base) + (0x099))
#define PMIC_MMW_OTP_10_W_ADDR(base) ((base) + (0x09A))
#define PMIC_MMW_OTP_11_W_ADDR(base) ((base) + (0x09B))
#define PMIC_MMW_OTP_12_W_ADDR(base) ((base) + (0x09C))
#define PMIC_MMW_OTP_13_W_ADDR(base) ((base) + (0x09D))
#define PMIC_MMW_OTP_14_W_ADDR(base) ((base) + (0x09E))
#define PMIC_MMW_OTP_15_W_ADDR(base) ((base) + (0x09F))
#define PMIC_MMW_OTP_16_W_ADDR(base) ((base) + (0x0A0))
#define PMIC_MMW_OTP_17_W_ADDR(base) ((base) + (0x0A1))
#define PMIC_MMW_OTP_18_W_ADDR(base) ((base) + (0x0A2))
#define PMIC_MMW_OTP_19_W_ADDR(base) ((base) + (0x0A3))
#define PMIC_MMW_OTP_20_W_ADDR(base) ((base) + (0x0A4))
#define PMIC_MMW_OTP_21_W_ADDR(base) ((base) + (0x0A5))
#define PMIC_MMW_LDO0_3_OCP_CTRL_ADDR(base) ((base) + (0x0A6))
#define PMIC_MMW_LDO4_7_OCP_CTRL_ADDR(base) ((base) + (0x0A7))
#define PMIC_MMW_LDO8_12_OCP_CTRL_ADDR(base) ((base) + (0x0A8))
#define PMIC_MMW_OCP_DEB_CTRL0_ADDR(base) ((base) + (0x0A9))
#define PMIC_MMW_OCP_DEB_CTRL1_ADDR(base) ((base) + (0x0AA))
#define PMIC_MMW_THSD_140_DEB_CTRL_ADDR(base) ((base) + (0x0AB))
#define PMIC_MMW_OCP_SCP_ONOFF_ADDR(base) ((base) + (0x0AC))
#define PMIC_MMW_HARDWIRE_CTRL0_ADDR(base) ((base) + (0x0AD))
#define PMIC_MMW_HARDWIRE_CTRL1_ADDR(base) ((base) + (0x0AE))
#define PMIC_MMW_HARDWIRE_CTRL2_ADDR(base) ((base) + (0x0AF))
#define PMIC_MMW_HARDWIRE_CTRL3_ADDR(base) ((base) + (0x0B0))
#define PMIC_MMW_HARDWIRE_CTRL4_ADDR(base) ((base) + (0x0B1))
#define PMIC_MMW_HARDWIRE_CTRL5_ADDR(base) ((base) + (0x0B2))
#define PMIC_MMW_LOCK_ADDR(base) ((base) + (0x0B3))
#define PMIC_MMW_SPMI_ECO_GT_BYPASS_ADDR(base) ((base) + (0x0B4))
#define PMIC_MMW_LDO_SST_END_CFG_ADDR(base) ((base) + (0x0B5))
#define PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_ADDR(base) ((base) + (0x0B6))
#define PMIC_MMW_XO_RF_CLK_ON_DEBUG_ADDR(base) ((base) + (0x0B7))
#define PMIC_MMW_XO_CORE_CURR_DEBUG_ADDR(base) ((base) + (0x0B8))
#define PMIC_MMW_XO_INBUFF_CURR_DEBUG_ADDR(base) ((base) + (0x0B9))
#define PMIC_MMW_SPMI_DEBUG0_ADDR(base) ((base) + (0x0F0))
#define PMIC_MMW_SPMI_DEBUG1_ADDR(base) ((base) + (0x0F1))
#define PMIC_MMW_SPMI_DEBUG2_ADDR(base) ((base) + (0x0F2))
#define PMIC_MMW_SPMI_DEBUG3_ADDR(base) ((base) + (0x0F3))
#define PMIC_MMW_SPMI_DEBUG4_ADDR(base) ((base) + (0x0F4))
#define PMIC_MMW_SPMI_DEBUG5_ADDR(base) ((base) + (0x0F5))
#define PMIC_MMW_SPMI_DEBUG6_ADDR(base) ((base) + (0x0F6))
#define PMIC_MMW_SPMI_DEBUG7_ADDR(base) ((base) + (0x0F7))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_MMW_IRQ_MASK_0_ADDR(base) ((base) + (0x100UL))
#define PMIC_MMW_IRQ_MASK_1_ADDR(base) ((base) + (0x101UL))
#define PMIC_MMW_IRQ_MASK_2_ADDR(base) ((base) + (0x102UL))
#define PMIC_MMW_FAULT_N_MASK_0_ADDR(base) ((base) + (0x103UL))
#define PMIC_MMW_FAULT_N_MASK_1_ADDR(base) ((base) + (0x104UL))
#define PMIC_MMW_FAULT_N_MASK_2_ADDR(base) ((base) + (0x105UL))
#define PMIC_MMW_FAULT_N_MASK_3_ADDR(base) ((base) + (0x106UL))
#else
#define PMIC_MMW_IRQ_MASK_0_ADDR(base) ((base) + (0x100))
#define PMIC_MMW_IRQ_MASK_1_ADDR(base) ((base) + (0x101))
#define PMIC_MMW_IRQ_MASK_2_ADDR(base) ((base) + (0x102))
#define PMIC_MMW_FAULT_N_MASK_0_ADDR(base) ((base) + (0x103))
#define PMIC_MMW_FAULT_N_MASK_1_ADDR(base) ((base) + (0x104))
#define PMIC_MMW_FAULT_N_MASK_2_ADDR(base) ((base) + (0x105))
#define PMIC_MMW_FAULT_N_MASK_3_ADDR(base) ((base) + (0x106))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_MMW_IRQ0_ADDR(base) ((base) + (0x10AUL))
#define PMIC_MMW_OCP_IRQ0_ADDR(base) ((base) + (0x10BUL))
#define PMIC_MMW_OCP_IRQ1_ADDR(base) ((base) + (0x10CUL))
#define PMIC_MMW_FAULT_N_0_ADDR(base) ((base) + (0x10DUL))
#define PMIC_MMW_FAULT_N_1_ADDR(base) ((base) + (0x10EUL))
#define PMIC_MMW_FAULT_N_2_ADDR(base) ((base) + (0x10FUL))
#define PMIC_MMW_FAULT_N_3_ADDR(base) ((base) + (0x110UL))
#else
#define PMIC_MMW_IRQ0_ADDR(base) ((base) + (0x10A))
#define PMIC_MMW_OCP_IRQ0_ADDR(base) ((base) + (0x10B))
#define PMIC_MMW_OCP_IRQ1_ADDR(base) ((base) + (0x10C))
#define PMIC_MMW_FAULT_N_0_ADDR(base) ((base) + (0x10D))
#define PMIC_MMW_FAULT_N_1_ADDR(base) ((base) + (0x10E))
#define PMIC_MMW_FAULT_N_2_ADDR(base) ((base) + (0x10F))
#define PMIC_MMW_FAULT_N_3_ADDR(base) ((base) + (0x110))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_MMW_NP_RECORD0_ADDR(base) ((base) + (0x114UL))
#define PMIC_MMW_NP_RECORD1_ADDR(base) ((base) + (0x115UL))
#define PMIC_MMW_NP_RECORD2_ADDR(base) ((base) + (0x116UL))
#define PMIC_MMW_NP_RECORD3_ADDR(base) ((base) + (0x117UL))
#define PMIC_MMW_NP_RECORD4_ADDR(base) ((base) + (0x118UL))
#define PMIC_MMW_NP_RECORD5_ADDR(base) ((base) + (0x119UL))
#else
#define PMIC_MMW_NP_RECORD0_ADDR(base) ((base) + (0x114))
#define PMIC_MMW_NP_RECORD1_ADDR(base) ((base) + (0x115))
#define PMIC_MMW_NP_RECORD2_ADDR(base) ((base) + (0x116))
#define PMIC_MMW_NP_RECORD3_ADDR(base) ((base) + (0x117))
#define PMIC_MMW_NP_RECORD4_ADDR(base) ((base) + (0x118))
#define PMIC_MMW_NP_RECORD5_ADDR(base) ((base) + (0x119))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_MMW_CLK_RF0_WAVE_CTRL_ADDR(base) ((base) + (0x200UL))
#define PMIC_MMW_CLK_RF1_WAVE_CTRL_ADDR(base) ((base) + (0x201UL))
#define PMIC_MMW_CLK_RF2_WAVE_CTRL_ADDR(base) ((base) + (0x202UL))
#define PMIC_MMW_CLK_RF3_WAVE_CTRL_ADDR(base) ((base) + (0x203UL))
#define PMIC_MMW_CLK_RF4_WAVE_CTRL_ADDR(base) ((base) + (0x204UL))
#define PMIC_MMW_CLK_SERDES_WAVE_CTRL_ADDR(base) ((base) + (0x205UL))
#define PMIC_MMW_CLK_WIFI0_WAVE_CTRL_ADDR(base) ((base) + (0x206UL))
#define PMIC_MMW_CLK_WIFI1_WAVE_CTRL_ADDR(base) ((base) + (0x207UL))
#define PMIC_MMW_CLK_NFC_WAVE_CTRL_ADDR(base) ((base) + (0x208UL))
#define PMIC_MMW_CLK_SYS_WAVE_CTRL_ADDR(base) ((base) + (0x209UL))
#define PMIC_MMW_CLK_PMU_WAVE_CTRL_ADDR(base) ((base) + (0x20AUL))
#define PMIC_MMW_CLK_RF0_FREQ_CTRL_ADDR(base) ((base) + (0x20BUL))
#define PMIC_MMW_CLK_RF1_FREQ_CTRL_ADDR(base) ((base) + (0x20CUL))
#define PMIC_MMW_CLK_RF2_FREQ_CTRL_ADDR(base) ((base) + (0x20DUL))
#define PMIC_MMW_CLK_RF3_FREQ_CTRL_ADDR(base) ((base) + (0x20EUL))
#define PMIC_MMW_CLK_RF4_FREQ_CTRL_ADDR(base) ((base) + (0x20FUL))
#define PMIC_MMW_CLK_SERDES_FREQ_CTRL_ADDR(base) ((base) + (0x210UL))
#define PMIC_MMW_CLK_WIFI0_FREQ_CTRL_ADDR(base) ((base) + (0x211UL))
#define PMIC_MMW_CLK_WIFI1_FREQ_CTRL_ADDR(base) ((base) + (0x212UL))
#define PMIC_MMW_CLK_NFC_FREQ_CTRL_ADDR(base) ((base) + (0x213UL))
#define PMIC_MMW_CLK_SYS_FREQ_CTRL_ADDR(base) ((base) + (0x214UL))
#define PMIC_MMW_CLK_PMU_FREQ_CTRL_ADDR(base) ((base) + (0x215UL))
#define PMIC_MMW_CLK_UFS_FREQ_CTRL_ADDR(base) ((base) + (0x216UL))
#define PMIC_MMW_CLK_SYS_BUF_PWR_SEL_ADDR(base) ((base) + (0x217UL))
#define PMIC_MMW_EN_PMUD_ADDR(base) ((base) + (0x218UL))
#define PMIC_MMW_PMUD_VSET_ADDR(base) ((base) + (0x219UL))
#define PMIC_MMW_CLK_TOP_CTRL1_0_ADDR(base) ((base) + (0x21AUL))
#define PMIC_MMW_CLK_TOP_CTRL1_1_ADDR(base) ((base) + (0x21BUL))
#define PMIC_MMW_CLK_256K_CTRL0_ADDR(base) ((base) + (0x21CUL))
#define PMIC_MMW_CLK_256K_CTRL1_ADDR(base) ((base) + (0x21DUL))
#define PMIC_MMW_XO_DET_DATA_ADDR(base) ((base) + (0x21EUL))
#define PMIC_MMW_VSYS_LOW_SET_ADDR(base) ((base) + (0x21FUL))
#define PMIC_MMW_NP_D2A_RES0_ADDR(base) ((base) + (0x220UL))
#define PMIC_MMW_NP_D2A_RES1_ADDR(base) ((base) + (0x221UL))
#define PMIC_MMW_NP_D2A_RES2_ADDR(base) ((base) + (0x222UL))
#define PMIC_MMW_NP_DEB_CTRL0_ADDR(base) ((base) + (0x223UL))
#define PMIC_MMW_NP_DEB_ONOFF0_ADDR(base) ((base) + (0x224UL))
#define PMIC_MMW_NP_PWRON_TIME_CFG_ADDR(base) ((base) + (0x225UL))
#define PMIC_MMW_DEFAULT_FREQ_CFG_ADDR(base) ((base) + (0x226UL))
#define PMIC_MMW_DEFAULT_WAVE_CFG_ADDR(base) ((base) + (0x227UL))
#define PMIC_MMW_DEFAULT_ONOFF_CFG_ADDR(base) ((base) + (0x228UL))
#define PMIC_MMW_DIG_IO_DS_CFG0_ADDR(base) ((base) + (0x229UL))
#define PMIC_MMW_DEBUG_LOCK_ADDR(base) ((base) + (0x22AUL))
#define PMIC_MMW_SYS_DEBUG0_ADDR(base) ((base) + (0x22BUL))
#define PMIC_MMW_SYS_DEBUG1_ADDR(base) ((base) + (0x22CUL))
#define PMIC_MMW_SYS_DEBUG2_ADDR(base) ((base) + (0x22DUL))
#define PMIC_MMW_SYS_DEBUG3_ADDR(base) ((base) + (0x22EUL))
#define PMIC_MMW_SYS_DEBUG4_ADDR(base) ((base) + (0x22FUL))
#define PMIC_MMW_SYS_DEBUG5_ADDR(base) ((base) + (0x230UL))
#define PMIC_MMW_OTP_0_R_ADDR(base) ((base) + (0x231UL))
#define PMIC_MMW_OTP_1_R_ADDR(base) ((base) + (0x232UL))
#define PMIC_MMW_OTP_2_R_ADDR(base) ((base) + (0x233UL))
#define PMIC_MMW_OTP_3_R_ADDR(base) ((base) + (0x234UL))
#define PMIC_MMW_OTP_4_R_ADDR(base) ((base) + (0x235UL))
#define PMIC_MMW_OTP_5_R_ADDR(base) ((base) + (0x236UL))
#define PMIC_MMW_OTP_6_R_ADDR(base) ((base) + (0x237UL))
#define PMIC_MMW_OTP_7_R_ADDR(base) ((base) + (0x238UL))
#define PMIC_MMW_OTP_8_R_ADDR(base) ((base) + (0x239UL))
#define PMIC_MMW_OTP_9_R_ADDR(base) ((base) + (0x23AUL))
#define PMIC_MMW_OTP_10_R_ADDR(base) ((base) + (0x23BUL))
#define PMIC_MMW_OTP_11_R_ADDR(base) ((base) + (0x23CUL))
#define PMIC_MMW_OTP_12_R_ADDR(base) ((base) + (0x23DUL))
#define PMIC_MMW_OTP_13_R_ADDR(base) ((base) + (0x23EUL))
#define PMIC_MMW_OTP_14_R_ADDR(base) ((base) + (0x23FUL))
#define PMIC_MMW_OTP_15_R_ADDR(base) ((base) + (0x240UL))
#define PMIC_MMW_OTP_16_R_ADDR(base) ((base) + (0x241UL))
#define PMIC_MMW_OTP_17_R_ADDR(base) ((base) + (0x242UL))
#define PMIC_MMW_OTP_18_R_ADDR(base) ((base) + (0x243UL))
#define PMIC_MMW_OTP_19_R_ADDR(base) ((base) + (0x244UL))
#define PMIC_MMW_OTP_20_R_ADDR(base) ((base) + (0x245UL))
#define PMIC_MMW_OTP_21_R_ADDR(base) ((base) + (0x246UL))
#define PMIC_MMW_OTP_22_R_ADDR(base) ((base) + (0x247UL))
#define PMIC_MMW_OTP_23_R_ADDR(base) ((base) + (0x248UL))
#define PMIC_MMW_OTP_24_R_ADDR(base) ((base) + (0x249UL))
#define PMIC_MMW_OTP_25_R_ADDR(base) ((base) + (0x24AUL))
#define PMIC_MMW_OTP_26_R_ADDR(base) ((base) + (0x24BUL))
#define PMIC_MMW_OTP_27_R_ADDR(base) ((base) + (0x24CUL))
#define PMIC_MMW_OTP_28_R_ADDR(base) ((base) + (0x24DUL))
#define PMIC_MMW_OTP_29_R_ADDR(base) ((base) + (0x24EUL))
#define PMIC_MMW_OTP_30_R_ADDR(base) ((base) + (0x24FUL))
#define PMIC_MMW_OTP_31_R_ADDR(base) ((base) + (0x250UL))
#define PMIC_MMW_OTP_32_R_ADDR(base) ((base) + (0x251UL))
#define PMIC_MMW_OTP_33_R_ADDR(base) ((base) + (0x252UL))
#define PMIC_MMW_OTP_34_R_ADDR(base) ((base) + (0x253UL))
#define PMIC_MMW_OTP_35_R_ADDR(base) ((base) + (0x254UL))
#define PMIC_MMW_OTP_36_R_ADDR(base) ((base) + (0x255UL))
#define PMIC_MMW_OTP_37_R_ADDR(base) ((base) + (0x256UL))
#define PMIC_MMW_OTP_38_R_ADDR(base) ((base) + (0x257UL))
#define PMIC_MMW_OTP_39_R_ADDR(base) ((base) + (0x258UL))
#define PMIC_MMW_OTP_40_R_ADDR(base) ((base) + (0x259UL))
#define PMIC_MMW_OTP_41_R_ADDR(base) ((base) + (0x25AUL))
#define PMIC_MMW_OTP_42_R_ADDR(base) ((base) + (0x25BUL))
#define PMIC_MMW_OTP_43_R_ADDR(base) ((base) + (0x25CUL))
#define PMIC_MMW_OTP_44_R_ADDR(base) ((base) + (0x25DUL))
#define PMIC_MMW_OTP_45_R_ADDR(base) ((base) + (0x25EUL))
#define PMIC_MMW_OTP_46_R_ADDR(base) ((base) + (0x25FUL))
#define PMIC_MMW_OTP_47_R_ADDR(base) ((base) + (0x260UL))
#define PMIC_MMW_OTP_48_R_ADDR(base) ((base) + (0x261UL))
#define PMIC_MMW_OTP_49_R_ADDR(base) ((base) + (0x262UL))
#define PMIC_MMW_OTP_50_R_ADDR(base) ((base) + (0x263UL))
#define PMIC_MMW_OTP_51_R_ADDR(base) ((base) + (0x264UL))
#define PMIC_MMW_OTP_52_R_ADDR(base) ((base) + (0x265UL))
#define PMIC_MMW_OTP_53_R_ADDR(base) ((base) + (0x266UL))
#define PMIC_MMW_OTP_54_R_ADDR(base) ((base) + (0x267UL))
#define PMIC_MMW_OTP_55_R_ADDR(base) ((base) + (0x268UL))
#define PMIC_MMW_OTP_56_R_ADDR(base) ((base) + (0x269UL))
#define PMIC_MMW_OTP_57_R_ADDR(base) ((base) + (0x26AUL))
#define PMIC_MMW_OTP_58_R_ADDR(base) ((base) + (0x26BUL))
#define PMIC_MMW_OTP_59_R_ADDR(base) ((base) + (0x26CUL))
#define PMIC_MMW_OTP_60_R_ADDR(base) ((base) + (0x26DUL))
#define PMIC_MMW_OTP_61_R_ADDR(base) ((base) + (0x26EUL))
#define PMIC_MMW_OTP_62_R_ADDR(base) ((base) + (0x26FUL))
#define PMIC_MMW_OTP_63_R_ADDR(base) ((base) + (0x270UL))
#else
#define PMIC_MMW_CLK_RF0_WAVE_CTRL_ADDR(base) ((base) + (0x200))
#define PMIC_MMW_CLK_RF1_WAVE_CTRL_ADDR(base) ((base) + (0x201))
#define PMIC_MMW_CLK_RF2_WAVE_CTRL_ADDR(base) ((base) + (0x202))
#define PMIC_MMW_CLK_RF3_WAVE_CTRL_ADDR(base) ((base) + (0x203))
#define PMIC_MMW_CLK_RF4_WAVE_CTRL_ADDR(base) ((base) + (0x204))
#define PMIC_MMW_CLK_SERDES_WAVE_CTRL_ADDR(base) ((base) + (0x205))
#define PMIC_MMW_CLK_WIFI0_WAVE_CTRL_ADDR(base) ((base) + (0x206))
#define PMIC_MMW_CLK_WIFI1_WAVE_CTRL_ADDR(base) ((base) + (0x207))
#define PMIC_MMW_CLK_NFC_WAVE_CTRL_ADDR(base) ((base) + (0x208))
#define PMIC_MMW_CLK_SYS_WAVE_CTRL_ADDR(base) ((base) + (0x209))
#define PMIC_MMW_CLK_PMU_WAVE_CTRL_ADDR(base) ((base) + (0x20A))
#define PMIC_MMW_CLK_RF0_FREQ_CTRL_ADDR(base) ((base) + (0x20B))
#define PMIC_MMW_CLK_RF1_FREQ_CTRL_ADDR(base) ((base) + (0x20C))
#define PMIC_MMW_CLK_RF2_FREQ_CTRL_ADDR(base) ((base) + (0x20D))
#define PMIC_MMW_CLK_RF3_FREQ_CTRL_ADDR(base) ((base) + (0x20E))
#define PMIC_MMW_CLK_RF4_FREQ_CTRL_ADDR(base) ((base) + (0x20F))
#define PMIC_MMW_CLK_SERDES_FREQ_CTRL_ADDR(base) ((base) + (0x210))
#define PMIC_MMW_CLK_WIFI0_FREQ_CTRL_ADDR(base) ((base) + (0x211))
#define PMIC_MMW_CLK_WIFI1_FREQ_CTRL_ADDR(base) ((base) + (0x212))
#define PMIC_MMW_CLK_NFC_FREQ_CTRL_ADDR(base) ((base) + (0x213))
#define PMIC_MMW_CLK_SYS_FREQ_CTRL_ADDR(base) ((base) + (0x214))
#define PMIC_MMW_CLK_PMU_FREQ_CTRL_ADDR(base) ((base) + (0x215))
#define PMIC_MMW_CLK_UFS_FREQ_CTRL_ADDR(base) ((base) + (0x216))
#define PMIC_MMW_CLK_SYS_BUF_PWR_SEL_ADDR(base) ((base) + (0x217))
#define PMIC_MMW_EN_PMUD_ADDR(base) ((base) + (0x218))
#define PMIC_MMW_PMUD_VSET_ADDR(base) ((base) + (0x219))
#define PMIC_MMW_CLK_TOP_CTRL1_0_ADDR(base) ((base) + (0x21A))
#define PMIC_MMW_CLK_TOP_CTRL1_1_ADDR(base) ((base) + (0x21B))
#define PMIC_MMW_CLK_256K_CTRL0_ADDR(base) ((base) + (0x21C))
#define PMIC_MMW_CLK_256K_CTRL1_ADDR(base) ((base) + (0x21D))
#define PMIC_MMW_XO_DET_DATA_ADDR(base) ((base) + (0x21E))
#define PMIC_MMW_VSYS_LOW_SET_ADDR(base) ((base) + (0x21F))
#define PMIC_MMW_NP_D2A_RES0_ADDR(base) ((base) + (0x220))
#define PMIC_MMW_NP_D2A_RES1_ADDR(base) ((base) + (0x221))
#define PMIC_MMW_NP_D2A_RES2_ADDR(base) ((base) + (0x222))
#define PMIC_MMW_NP_DEB_CTRL0_ADDR(base) ((base) + (0x223))
#define PMIC_MMW_NP_DEB_ONOFF0_ADDR(base) ((base) + (0x224))
#define PMIC_MMW_NP_PWRON_TIME_CFG_ADDR(base) ((base) + (0x225))
#define PMIC_MMW_DEFAULT_FREQ_CFG_ADDR(base) ((base) + (0x226))
#define PMIC_MMW_DEFAULT_WAVE_CFG_ADDR(base) ((base) + (0x227))
#define PMIC_MMW_DEFAULT_ONOFF_CFG_ADDR(base) ((base) + (0x228))
#define PMIC_MMW_DIG_IO_DS_CFG0_ADDR(base) ((base) + (0x229))
#define PMIC_MMW_DEBUG_LOCK_ADDR(base) ((base) + (0x22A))
#define PMIC_MMW_SYS_DEBUG0_ADDR(base) ((base) + (0x22B))
#define PMIC_MMW_SYS_DEBUG1_ADDR(base) ((base) + (0x22C))
#define PMIC_MMW_SYS_DEBUG2_ADDR(base) ((base) + (0x22D))
#define PMIC_MMW_SYS_DEBUG3_ADDR(base) ((base) + (0x22E))
#define PMIC_MMW_SYS_DEBUG4_ADDR(base) ((base) + (0x22F))
#define PMIC_MMW_SYS_DEBUG5_ADDR(base) ((base) + (0x230))
#define PMIC_MMW_OTP_0_R_ADDR(base) ((base) + (0x231))
#define PMIC_MMW_OTP_1_R_ADDR(base) ((base) + (0x232))
#define PMIC_MMW_OTP_2_R_ADDR(base) ((base) + (0x233))
#define PMIC_MMW_OTP_3_R_ADDR(base) ((base) + (0x234))
#define PMIC_MMW_OTP_4_R_ADDR(base) ((base) + (0x235))
#define PMIC_MMW_OTP_5_R_ADDR(base) ((base) + (0x236))
#define PMIC_MMW_OTP_6_R_ADDR(base) ((base) + (0x237))
#define PMIC_MMW_OTP_7_R_ADDR(base) ((base) + (0x238))
#define PMIC_MMW_OTP_8_R_ADDR(base) ((base) + (0x239))
#define PMIC_MMW_OTP_9_R_ADDR(base) ((base) + (0x23A))
#define PMIC_MMW_OTP_10_R_ADDR(base) ((base) + (0x23B))
#define PMIC_MMW_OTP_11_R_ADDR(base) ((base) + (0x23C))
#define PMIC_MMW_OTP_12_R_ADDR(base) ((base) + (0x23D))
#define PMIC_MMW_OTP_13_R_ADDR(base) ((base) + (0x23E))
#define PMIC_MMW_OTP_14_R_ADDR(base) ((base) + (0x23F))
#define PMIC_MMW_OTP_15_R_ADDR(base) ((base) + (0x240))
#define PMIC_MMW_OTP_16_R_ADDR(base) ((base) + (0x241))
#define PMIC_MMW_OTP_17_R_ADDR(base) ((base) + (0x242))
#define PMIC_MMW_OTP_18_R_ADDR(base) ((base) + (0x243))
#define PMIC_MMW_OTP_19_R_ADDR(base) ((base) + (0x244))
#define PMIC_MMW_OTP_20_R_ADDR(base) ((base) + (0x245))
#define PMIC_MMW_OTP_21_R_ADDR(base) ((base) + (0x246))
#define PMIC_MMW_OTP_22_R_ADDR(base) ((base) + (0x247))
#define PMIC_MMW_OTP_23_R_ADDR(base) ((base) + (0x248))
#define PMIC_MMW_OTP_24_R_ADDR(base) ((base) + (0x249))
#define PMIC_MMW_OTP_25_R_ADDR(base) ((base) + (0x24A))
#define PMIC_MMW_OTP_26_R_ADDR(base) ((base) + (0x24B))
#define PMIC_MMW_OTP_27_R_ADDR(base) ((base) + (0x24C))
#define PMIC_MMW_OTP_28_R_ADDR(base) ((base) + (0x24D))
#define PMIC_MMW_OTP_29_R_ADDR(base) ((base) + (0x24E))
#define PMIC_MMW_OTP_30_R_ADDR(base) ((base) + (0x24F))
#define PMIC_MMW_OTP_31_R_ADDR(base) ((base) + (0x250))
#define PMIC_MMW_OTP_32_R_ADDR(base) ((base) + (0x251))
#define PMIC_MMW_OTP_33_R_ADDR(base) ((base) + (0x252))
#define PMIC_MMW_OTP_34_R_ADDR(base) ((base) + (0x253))
#define PMIC_MMW_OTP_35_R_ADDR(base) ((base) + (0x254))
#define PMIC_MMW_OTP_36_R_ADDR(base) ((base) + (0x255))
#define PMIC_MMW_OTP_37_R_ADDR(base) ((base) + (0x256))
#define PMIC_MMW_OTP_38_R_ADDR(base) ((base) + (0x257))
#define PMIC_MMW_OTP_39_R_ADDR(base) ((base) + (0x258))
#define PMIC_MMW_OTP_40_R_ADDR(base) ((base) + (0x259))
#define PMIC_MMW_OTP_41_R_ADDR(base) ((base) + (0x25A))
#define PMIC_MMW_OTP_42_R_ADDR(base) ((base) + (0x25B))
#define PMIC_MMW_OTP_43_R_ADDR(base) ((base) + (0x25C))
#define PMIC_MMW_OTP_44_R_ADDR(base) ((base) + (0x25D))
#define PMIC_MMW_OTP_45_R_ADDR(base) ((base) + (0x25E))
#define PMIC_MMW_OTP_46_R_ADDR(base) ((base) + (0x25F))
#define PMIC_MMW_OTP_47_R_ADDR(base) ((base) + (0x260))
#define PMIC_MMW_OTP_48_R_ADDR(base) ((base) + (0x261))
#define PMIC_MMW_OTP_49_R_ADDR(base) ((base) + (0x262))
#define PMIC_MMW_OTP_50_R_ADDR(base) ((base) + (0x263))
#define PMIC_MMW_OTP_51_R_ADDR(base) ((base) + (0x264))
#define PMIC_MMW_OTP_52_R_ADDR(base) ((base) + (0x265))
#define PMIC_MMW_OTP_53_R_ADDR(base) ((base) + (0x266))
#define PMIC_MMW_OTP_54_R_ADDR(base) ((base) + (0x267))
#define PMIC_MMW_OTP_55_R_ADDR(base) ((base) + (0x268))
#define PMIC_MMW_OTP_56_R_ADDR(base) ((base) + (0x269))
#define PMIC_MMW_OTP_57_R_ADDR(base) ((base) + (0x26A))
#define PMIC_MMW_OTP_58_R_ADDR(base) ((base) + (0x26B))
#define PMIC_MMW_OTP_59_R_ADDR(base) ((base) + (0x26C))
#define PMIC_MMW_OTP_60_R_ADDR(base) ((base) + (0x26D))
#define PMIC_MMW_OTP_61_R_ADDR(base) ((base) + (0x26E))
#define PMIC_MMW_OTP_62_R_ADDR(base) ((base) + (0x26F))
#define PMIC_MMW_OTP_63_R_ADDR(base) ((base) + (0x270))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char project_num0 : 8;
    } reg;
} PMIC_MMW_VERSION0_UNION;
#endif
#define PMIC_MMW_VERSION0_project_num0_START (0)
#define PMIC_MMW_VERSION0_project_num0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char project_num1 : 8;
    } reg;
} PMIC_MMW_VERSION1_UNION;
#endif
#define PMIC_MMW_VERSION1_project_num1_START (0)
#define PMIC_MMW_VERSION1_project_num1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char project_num2 : 8;
    } reg;
} PMIC_MMW_VERSION2_UNION;
#endif
#define PMIC_MMW_VERSION2_project_num2_START (0)
#define PMIC_MMW_VERSION2_project_num2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char project_num3 : 8;
    } reg;
} PMIC_MMW_VERSION3_UNION;
#endif
#define PMIC_MMW_VERSION3_project_num3_START (0)
#define PMIC_MMW_VERSION3_project_num3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char version : 8;
    } reg;
} PMIC_MMW_VERSION4_UNION;
#endif
#define PMIC_MMW_VERSION4_version_START (0)
#define PMIC_MMW_VERSION4_version_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char chip_id : 8;
    } reg;
} PMIC_MMW_VERSION5_UNION;
#endif
#define PMIC_MMW_VERSION5_chip_id_START (0)
#define PMIC_MMW_VERSION5_chip_id_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char st_vsys_ov_d200ur : 1;
        unsigned char st_vsys_pwroff_abs_d20nr : 1;
        unsigned char st_vsys_pwron_d60ur : 1;
        unsigned char st_thsd_otmp140_d540ur : 1;
        unsigned char st_thsd_otmp125_d120ur : 1;
        unsigned char st_hi6563_en_deb : 1;
        unsigned char st_buf_en_deb : 1;
        unsigned char st_vin_ldom_vld_deb : 1;
    } reg;
} PMIC_MMW_STATUS0_UNION;
#endif
#define PMIC_MMW_STATUS0_st_vsys_ov_d200ur_START (0)
#define PMIC_MMW_STATUS0_st_vsys_ov_d200ur_END (0)
#define PMIC_MMW_STATUS0_st_vsys_pwroff_abs_d20nr_START (1)
#define PMIC_MMW_STATUS0_st_vsys_pwroff_abs_d20nr_END (1)
#define PMIC_MMW_STATUS0_st_vsys_pwron_d60ur_START (2)
#define PMIC_MMW_STATUS0_st_vsys_pwron_d60ur_END (2)
#define PMIC_MMW_STATUS0_st_thsd_otmp140_d540ur_START (3)
#define PMIC_MMW_STATUS0_st_thsd_otmp140_d540ur_END (3)
#define PMIC_MMW_STATUS0_st_thsd_otmp125_d120ur_START (4)
#define PMIC_MMW_STATUS0_st_thsd_otmp125_d120ur_END (4)
#define PMIC_MMW_STATUS0_st_hi6563_en_deb_START (5)
#define PMIC_MMW_STATUS0_st_hi6563_en_deb_END (5)
#define PMIC_MMW_STATUS0_st_buf_en_deb_START (6)
#define PMIC_MMW_STATUS0_st_buf_en_deb_END (6)
#define PMIC_MMW_STATUS0_st_vin_ldom_vld_deb_START (7)
#define PMIC_MMW_STATUS0_st_vin_ldom_vld_deb_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char st_dcxo_clk_sel : 1;
        unsigned char st_ldo_core_vld_d20nf : 1;
        unsigned char st_vin_ldo_0p9_vld_d1mf : 1;
        unsigned char st_vin_ldo_1p1_vld_d1mf : 1;
        unsigned char st_vin_ldo_1p3_vld_d1mf : 1;
        unsigned char st_vin_io_vld_d5m : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_MMW_STATUS1_UNION;
#endif
#define PMIC_MMW_STATUS1_st_dcxo_clk_sel_START (0)
#define PMIC_MMW_STATUS1_st_dcxo_clk_sel_END (0)
#define PMIC_MMW_STATUS1_st_ldo_core_vld_d20nf_START (1)
#define PMIC_MMW_STATUS1_st_ldo_core_vld_d20nf_END (1)
#define PMIC_MMW_STATUS1_st_vin_ldo_0p9_vld_d1mf_START (2)
#define PMIC_MMW_STATUS1_st_vin_ldo_0p9_vld_d1mf_END (2)
#define PMIC_MMW_STATUS1_st_vin_ldo_1p1_vld_d1mf_START (3)
#define PMIC_MMW_STATUS1_st_vin_ldo_1p1_vld_d1mf_END (3)
#define PMIC_MMW_STATUS1_st_vin_ldo_1p3_vld_d1mf_START (4)
#define PMIC_MMW_STATUS1_st_vin_ldo_1p3_vld_d1mf_END (4)
#define PMIC_MMW_STATUS1_st_vin_io_vld_d5m_START (5)
#define PMIC_MMW_STATUS1_st_vin_io_vld_d5m_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo0_en : 1;
        unsigned char st_ldo0_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO0_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO0_ONOFF_reg_ldo0_en_START (0)
#define PMIC_MMW_LDO0_ONOFF_reg_ldo0_en_END (0)
#define PMIC_MMW_LDO0_ONOFF_st_ldo0_en_START (1)
#define PMIC_MMW_LDO0_ONOFF_st_ldo0_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo1_en : 1;
        unsigned char st_ldo1_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO1_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO1_ONOFF_reg_ldo1_en_START (0)
#define PMIC_MMW_LDO1_ONOFF_reg_ldo1_en_END (0)
#define PMIC_MMW_LDO1_ONOFF_st_ldo1_en_START (1)
#define PMIC_MMW_LDO1_ONOFF_st_ldo1_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo2_en : 1;
        unsigned char st_ldo2_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO2_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO2_ONOFF_reg_ldo2_en_START (0)
#define PMIC_MMW_LDO2_ONOFF_reg_ldo2_en_END (0)
#define PMIC_MMW_LDO2_ONOFF_st_ldo2_en_START (1)
#define PMIC_MMW_LDO2_ONOFF_st_ldo2_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo3_en : 1;
        unsigned char st_ldo3_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO3_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO3_ONOFF_reg_ldo3_en_START (0)
#define PMIC_MMW_LDO3_ONOFF_reg_ldo3_en_END (0)
#define PMIC_MMW_LDO3_ONOFF_st_ldo3_en_START (1)
#define PMIC_MMW_LDO3_ONOFF_st_ldo3_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo4_en : 1;
        unsigned char st_ldo4_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO4_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO4_ONOFF_reg_ldo4_en_START (0)
#define PMIC_MMW_LDO4_ONOFF_reg_ldo4_en_END (0)
#define PMIC_MMW_LDO4_ONOFF_st_ldo4_en_START (1)
#define PMIC_MMW_LDO4_ONOFF_st_ldo4_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo5_en : 1;
        unsigned char st_ldo5_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO5_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO5_ONOFF_reg_ldo5_en_START (0)
#define PMIC_MMW_LDO5_ONOFF_reg_ldo5_en_END (0)
#define PMIC_MMW_LDO5_ONOFF_st_ldo5_en_START (1)
#define PMIC_MMW_LDO5_ONOFF_st_ldo5_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo6_en : 1;
        unsigned char st_ldo6_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO6_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO6_ONOFF_reg_ldo6_en_START (0)
#define PMIC_MMW_LDO6_ONOFF_reg_ldo6_en_END (0)
#define PMIC_MMW_LDO6_ONOFF_st_ldo6_en_START (1)
#define PMIC_MMW_LDO6_ONOFF_st_ldo6_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo7_en : 1;
        unsigned char st_ldo7_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo7_eco_en : 1;
        unsigned char st_ldo7_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_MMW_LDO7_ONOFF_ECO_UNION;
#endif
#define PMIC_MMW_LDO7_ONOFF_ECO_reg_ldo7_en_START (0)
#define PMIC_MMW_LDO7_ONOFF_ECO_reg_ldo7_en_END (0)
#define PMIC_MMW_LDO7_ONOFF_ECO_st_ldo7_en_START (1)
#define PMIC_MMW_LDO7_ONOFF_ECO_st_ldo7_en_END (1)
#define PMIC_MMW_LDO7_ONOFF_ECO_reg_ldo7_eco_en_START (4)
#define PMIC_MMW_LDO7_ONOFF_ECO_reg_ldo7_eco_en_END (4)
#define PMIC_MMW_LDO7_ONOFF_ECO_st_ldo7_eco_en_START (5)
#define PMIC_MMW_LDO7_ONOFF_ECO_st_ldo7_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo8_en : 1;
        unsigned char st_ldo8_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo8_eco_en : 1;
        unsigned char st_ldo8_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_MMW_LDO8_ONOFF_ECO_UNION;
#endif
#define PMIC_MMW_LDO8_ONOFF_ECO_reg_ldo8_en_START (0)
#define PMIC_MMW_LDO8_ONOFF_ECO_reg_ldo8_en_END (0)
#define PMIC_MMW_LDO8_ONOFF_ECO_st_ldo8_en_START (1)
#define PMIC_MMW_LDO8_ONOFF_ECO_st_ldo8_en_END (1)
#define PMIC_MMW_LDO8_ONOFF_ECO_reg_ldo8_eco_en_START (4)
#define PMIC_MMW_LDO8_ONOFF_ECO_reg_ldo8_eco_en_END (4)
#define PMIC_MMW_LDO8_ONOFF_ECO_st_ldo8_eco_en_START (5)
#define PMIC_MMW_LDO8_ONOFF_ECO_st_ldo8_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo_core_en : 1;
        unsigned char st_ldo_core_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo_core_eco_en : 1;
        unsigned char st_ldo_core_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_MMW_LDO_CORE_ONOFF_ECO_UNION;
#endif
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_reg_ldo_core_en_START (0)
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_reg_ldo_core_en_END (0)
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_st_ldo_core_en_START (1)
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_st_ldo_core_en_END (1)
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_reg_ldo_core_eco_en_START (4)
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_reg_ldo_core_eco_en_END (4)
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_st_ldo_core_eco_en_START (5)
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_st_ldo_core_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo_buf_rf_en : 1;
        unsigned char st_ldo_buf_rf_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO_BUF_RF_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_BUF_RF_ONOFF_reg_ldo_buf_rf_en_START (0)
#define PMIC_MMW_LDO_BUF_RF_ONOFF_reg_ldo_buf_rf_en_END (0)
#define PMIC_MMW_LDO_BUF_RF_ONOFF_st_ldo_buf_rf_en_START (1)
#define PMIC_MMW_LDO_BUF_RF_ONOFF_st_ldo_buf_rf_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo_buf0_en : 1;
        unsigned char st_ldo_buf0_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO_BUF0_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_BUF0_ONOFF_reg_ldo_buf0_en_START (0)
#define PMIC_MMW_LDO_BUF0_ONOFF_reg_ldo_buf0_en_END (0)
#define PMIC_MMW_LDO_BUF0_ONOFF_st_ldo_buf0_en_START (1)
#define PMIC_MMW_LDO_BUF0_ONOFF_st_ldo_buf0_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo_buf1_en : 1;
        unsigned char st_ldo_buf1_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO_BUF1_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_BUF1_ONOFF_reg_ldo_buf1_en_START (0)
#define PMIC_MMW_LDO_BUF1_ONOFF_reg_ldo_buf1_en_END (0)
#define PMIC_MMW_LDO_BUF1_ONOFF_st_ldo_buf1_en_START (1)
#define PMIC_MMW_LDO_BUF1_ONOFF_st_ldo_buf1_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo_rfbuf0_en : 1;
        unsigned char st_ldo_rfbuf0_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO_RFBUF0_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF0_ONOFF_reg_ldo_rfbuf0_en_START (0)
#define PMIC_MMW_LDO_RFBUF0_ONOFF_reg_ldo_rfbuf0_en_END (0)
#define PMIC_MMW_LDO_RFBUF0_ONOFF_st_ldo_rfbuf0_en_START (1)
#define PMIC_MMW_LDO_RFBUF0_ONOFF_st_ldo_rfbuf0_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo_rfbuf1_en : 1;
        unsigned char st_ldo_rfbuf1_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO_RFBUF1_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF1_ONOFF_reg_ldo_rfbuf1_en_START (0)
#define PMIC_MMW_LDO_RFBUF1_ONOFF_reg_ldo_rfbuf1_en_END (0)
#define PMIC_MMW_LDO_RFBUF1_ONOFF_st_ldo_rfbuf1_en_START (1)
#define PMIC_MMW_LDO_RFBUF1_ONOFF_st_ldo_rfbuf1_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo_rfbuf2_en : 1;
        unsigned char st_ldo_rfbuf2_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO_RFBUF2_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF2_ONOFF_reg_ldo_rfbuf2_en_START (0)
#define PMIC_MMW_LDO_RFBUF2_ONOFF_reg_ldo_rfbuf2_en_END (0)
#define PMIC_MMW_LDO_RFBUF2_ONOFF_st_ldo_rfbuf2_en_START (1)
#define PMIC_MMW_LDO_RFBUF2_ONOFF_st_ldo_rfbuf2_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo_rfbuf3_en : 1;
        unsigned char st_ldo_rfbuf3_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO_RFBUF3_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF3_ONOFF_reg_ldo_rfbuf3_en_START (0)
#define PMIC_MMW_LDO_RFBUF3_ONOFF_reg_ldo_rfbuf3_en_END (0)
#define PMIC_MMW_LDO_RFBUF3_ONOFF_st_ldo_rfbuf3_en_START (1)
#define PMIC_MMW_LDO_RFBUF3_ONOFF_st_ldo_rfbuf3_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo_rfbuf4_en : 1;
        unsigned char st_ldo_rfbuf4_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO_RFBUF4_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF4_ONOFF_reg_ldo_rfbuf4_en_START (0)
#define PMIC_MMW_LDO_RFBUF4_ONOFF_reg_ldo_rfbuf4_en_END (0)
#define PMIC_MMW_LDO_RFBUF4_ONOFF_st_ldo_rfbuf4_en_START (1)
#define PMIC_MMW_LDO_RFBUF4_ONOFF_st_ldo_rfbuf4_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_pmuh_en : 1;
        unsigned char st_pmuh_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_PMUH_ONOFF_UNION;
#endif
#define PMIC_MMW_PMUH_ONOFF_reg_pmuh_en_START (0)
#define PMIC_MMW_PMUH_ONOFF_reg_pmuh_en_END (0)
#define PMIC_MMW_PMUH_ONOFF_st_pmuh_en_START (1)
#define PMIC_MMW_PMUH_ONOFF_st_pmuh_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_core_en : 1;
        unsigned char st_xo_core_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_XO_CORE_ONOFF_UNION;
#endif
#define PMIC_MMW_XO_CORE_ONOFF_reg_xo_core_en_START (0)
#define PMIC_MMW_XO_CORE_ONOFF_reg_xo_core_en_END (0)
#define PMIC_MMW_XO_CORE_ONOFF_st_xo_core_en_START (1)
#define PMIC_MMW_XO_CORE_ONOFF_st_xo_core_en_END (1)
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
} PMIC_MMW_THSD_ONOFF_ECO_UNION;
#endif
#define PMIC_MMW_THSD_ONOFF_ECO_reg_thsd_en_START (0)
#define PMIC_MMW_THSD_ONOFF_ECO_reg_thsd_en_END (0)
#define PMIC_MMW_THSD_ONOFF_ECO_st_thsd_en_START (1)
#define PMIC_MMW_THSD_ONOFF_ECO_st_thsd_en_END (1)
#define PMIC_MMW_THSD_ONOFF_ECO_reg_thsd_eco_en_START (4)
#define PMIC_MMW_THSD_ONOFF_ECO_reg_thsd_eco_en_END (4)
#define PMIC_MMW_THSD_ONOFF_ECO_st_thsd_eco_en_START (5)
#define PMIC_MMW_THSD_ONOFF_ECO_st_thsd_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_ufs_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_UFS_EN_UNION;
#endif
#define PMIC_MMW_CLK_UFS_EN_reg_xo_ufs_en_START (0)
#define PMIC_MMW_CLK_UFS_EN_reg_xo_ufs_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_rf0_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF0_EN_UNION;
#endif
#define PMIC_MMW_CLK_RF0_EN_reg_xo_rf0_en_START (0)
#define PMIC_MMW_CLK_RF0_EN_reg_xo_rf0_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_rf1_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF1_EN_UNION;
#endif
#define PMIC_MMW_CLK_RF1_EN_reg_xo_rf1_en_START (0)
#define PMIC_MMW_CLK_RF1_EN_reg_xo_rf1_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_rf2_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF2_EN_UNION;
#endif
#define PMIC_MMW_CLK_RF2_EN_reg_xo_rf2_en_START (0)
#define PMIC_MMW_CLK_RF2_EN_reg_xo_rf2_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_rf3_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF3_EN_UNION;
#endif
#define PMIC_MMW_CLK_RF3_EN_reg_xo_rf3_en_START (0)
#define PMIC_MMW_CLK_RF3_EN_reg_xo_rf3_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_rf4_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF4_EN_UNION;
#endif
#define PMIC_MMW_CLK_RF4_EN_reg_xo_rf4_en_START (0)
#define PMIC_MMW_CLK_RF4_EN_reg_xo_rf4_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_serdes_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_SERDES_EN_UNION;
#endif
#define PMIC_MMW_CLK_SERDES_EN_reg_xo_serdes_en_START (0)
#define PMIC_MMW_CLK_SERDES_EN_reg_xo_serdes_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_wifi0_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_WIFI0_EN_UNION;
#endif
#define PMIC_MMW_CLK_WIFI0_EN_reg_xo_wifi0_en_START (0)
#define PMIC_MMW_CLK_WIFI0_EN_reg_xo_wifi0_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_wifi1_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_WIFI1_EN_UNION;
#endif
#define PMIC_MMW_CLK_WIFI1_EN_reg_xo_wifi1_en_START (0)
#define PMIC_MMW_CLK_WIFI1_EN_reg_xo_wifi1_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_nfc_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_NFC_EN_UNION;
#endif
#define PMIC_MMW_CLK_NFC_EN_reg_xo_nfc_en_START (0)
#define PMIC_MMW_CLK_NFC_EN_reg_xo_nfc_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_sys_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_SYS_EN_UNION;
#endif
#define PMIC_MMW_CLK_SYS_EN_reg_xo_sys_en_START (0)
#define PMIC_MMW_CLK_SYS_EN_reg_xo_sys_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_pmu_en : 8;
    } reg;
} PMIC_MMW_CLK_PMU_EN_UNION;
#endif
#define PMIC_MMW_CLK_PMU_EN_reg_xo_pmu_en_START (0)
#define PMIC_MMW_CLK_PMU_EN_reg_xo_pmu_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char en_32k_sys : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_OSC32K_SYS_ONOFF_CTRL_UNION;
#endif
#define PMIC_MMW_OSC32K_SYS_ONOFF_CTRL_en_32k_sys_START (0)
#define PMIC_MMW_OSC32K_SYS_ONOFF_CTRL_en_32k_sys_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo0_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO0_VSET_UNION;
#endif
#define PMIC_MMW_LDO0_VSET_ldo0_vset_START (0)
#define PMIC_MMW_LDO0_VSET_ldo0_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo1_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO1_VSET_UNION;
#endif
#define PMIC_MMW_LDO1_VSET_ldo1_vset_START (0)
#define PMIC_MMW_LDO1_VSET_ldo1_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo2_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO2_VSET_UNION;
#endif
#define PMIC_MMW_LDO2_VSET_ldo2_vset_START (0)
#define PMIC_MMW_LDO2_VSET_ldo2_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo3_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO3_VSET_UNION;
#endif
#define PMIC_MMW_LDO3_VSET_ldo3_vset_START (0)
#define PMIC_MMW_LDO3_VSET_ldo3_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo4_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO4_VSET_UNION;
#endif
#define PMIC_MMW_LDO4_VSET_ldo4_vset_START (0)
#define PMIC_MMW_LDO4_VSET_ldo4_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo5_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO5_VSET_UNION;
#endif
#define PMIC_MMW_LDO5_VSET_ldo5_vset_START (0)
#define PMIC_MMW_LDO5_VSET_ldo5_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo6_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO6_VSET_UNION;
#endif
#define PMIC_MMW_LDO6_VSET_ldo6_vset_START (0)
#define PMIC_MMW_LDO6_VSET_ldo6_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo7_vset : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_MMW_LDO7_VSET_UNION;
#endif
#define PMIC_MMW_LDO7_VSET_ldo7_vset_START (0)
#define PMIC_MMW_LDO7_VSET_ldo7_vset_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo8_vset : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_MMW_LDO8_VSET_UNION;
#endif
#define PMIC_MMW_LDO8_VSET_ldo8_vset_START (0)
#define PMIC_MMW_LDO8_VSET_ldo8_vset_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_core_vset_cfg : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO_CORE_VSET_UNION;
#endif
#define PMIC_MMW_LDO_CORE_VSET_ldo_core_vset_cfg_START (0)
#define PMIC_MMW_LDO_CORE_VSET_ldo_core_vset_cfg_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_core_vset_eco_cfg : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO_CORE_VSET_ECO_UNION;
#endif
#define PMIC_MMW_LDO_CORE_VSET_ECO_ldo_core_vset_eco_cfg_START (0)
#define PMIC_MMW_LDO_CORE_VSET_ECO_ldo_core_vset_eco_cfg_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_rfbuf_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO_BUF_RF_VSET_UNION;
#endif
#define PMIC_MMW_LDO_BUF_RF_VSET_ldo_rfbuf_vset_START (0)
#define PMIC_MMW_LDO_BUF_RF_VSET_ldo_rfbuf_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_buf0_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO_BUF0_VSET_UNION;
#endif
#define PMIC_MMW_LDO_BUF0_VSET_ldo_buf0_vset_START (0)
#define PMIC_MMW_LDO_BUF0_VSET_ldo_buf0_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_buf1_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO_BUF1_VSET_UNION;
#endif
#define PMIC_MMW_LDO_BUF1_VSET_ldo_buf1_vset_START (0)
#define PMIC_MMW_LDO_BUF1_VSET_ldo_buf1_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_rfbuf0_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO_RFBUF0_VSET_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF0_VSET_ldo_rfbuf0_vset_START (0)
#define PMIC_MMW_LDO_RFBUF0_VSET_ldo_rfbuf0_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_rfbuf1_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO_RFBUF1_VSET_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF1_VSET_ldo_rfbuf1_vset_START (0)
#define PMIC_MMW_LDO_RFBUF1_VSET_ldo_rfbuf1_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_rfbuf2_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO_RFBUF2_VSET_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF2_VSET_ldo_rfbuf2_vset_START (0)
#define PMIC_MMW_LDO_RFBUF2_VSET_ldo_rfbuf2_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_rfbuf3_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO_RFBUF3_VSET_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF3_VSET_ldo_rfbuf3_vset_START (0)
#define PMIC_MMW_LDO_RFBUF3_VSET_ldo_rfbuf3_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_rfbuf4_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO_RFBUF4_VSET_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF4_VSET_ldo_rfbuf4_vset_START (0)
#define PMIC_MMW_LDO_RFBUF4_VSET_ldo_rfbuf4_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pmuh_vset : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_MMW_LDO_PMUH_VSET_UNION;
#endif
#define PMIC_MMW_LDO_PMUH_VSET_pmuh_vset_START (0)
#define PMIC_MMW_LDO_PMUH_VSET_pmuh_vset_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_core_curr_cfg : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_XO_CORE_CURR_UNION;
#endif
#define PMIC_MMW_XO_CORE_CURR_xo_core_curr_cfg_START (0)
#define PMIC_MMW_XO_CORE_CURR_xo_core_curr_cfg_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_core_curr_eco_cfg : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_XO_CORE_CURR_ECO_UNION;
#endif
#define PMIC_MMW_XO_CORE_CURR_ECO_xo_core_curr_eco_cfg_START (0)
#define PMIC_MMW_XO_CORE_CURR_ECO_xo_core_curr_eco_cfg_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_inbuff_curr_cfg : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_MMW_XO_INBUFF_CURR_UNION;
#endif
#define PMIC_MMW_XO_INBUFF_CURR_xo_inbuff_curr_cfg_START (0)
#define PMIC_MMW_XO_INBUFF_CURR_xo_inbuff_curr_cfg_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_inbuff_curr_eco_cfg : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_MMW_XO_INBUFF_CURR_ECO_UNION;
#endif
#define PMIC_MMW_XO_INBUFF_CURR_ECO_xo_inbuff_curr_eco_cfg_START (0)
#define PMIC_MMW_XO_INBUFF_CURR_ECO_xo_inbuff_curr_eco_cfg_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo0_ocpd_vos_set : 1;
        unsigned char ldo0_ocpd_set : 2;
        unsigned char ldo0_ocp_enn : 1;
        unsigned char ldo0_comp : 3;
        unsigned char ldo0_bw_en : 1;
    } reg;
} PMIC_MMW_LDO0_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_ocpd_vos_set_START (0)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_ocpd_vos_set_END (0)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_ocpd_set_START (1)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_ocpd_set_END (2)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_ocp_enn_START (3)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_ocp_enn_END (3)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_comp_START (4)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_comp_END (6)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_bw_en_START (7)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_bw_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo0_psrr1_set : 3;
        unsigned char ldo0_psrr_en : 1;
        unsigned char ldo0_ocps_set : 2;
        unsigned char ldo0_ocps_en : 1;
        unsigned char ldo0_ocps_bw_set : 1;
    } reg;
} PMIC_MMW_LDO0_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_psrr1_set_START (0)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_psrr1_set_END (2)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_psrr_en_START (3)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_psrr_en_END (3)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_ocps_set_START (4)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_ocps_set_END (5)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_ocps_en_START (6)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_ocps_en_END (6)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_ocps_bw_set_START (7)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_ocps_bw_set_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo0_vrset : 3;
        unsigned char ldo0_vgpr_en : 1;
        unsigned char ldo0_psrr2_set : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_MMW_LDO0_CFG_REG_2_UNION;
#endif
#define PMIC_MMW_LDO0_CFG_REG_2_ldo0_vrset_START (0)
#define PMIC_MMW_LDO0_CFG_REG_2_ldo0_vrset_END (2)
#define PMIC_MMW_LDO0_CFG_REG_2_ldo0_vgpr_en_START (3)
#define PMIC_MMW_LDO0_CFG_REG_2_ldo0_vgpr_en_END (3)
#define PMIC_MMW_LDO0_CFG_REG_2_ldo0_psrr2_set_START (4)
#define PMIC_MMW_LDO0_CFG_REG_2_ldo0_psrr2_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo1_ocpd_vos_set : 1;
        unsigned char ldo1_ocpd_set : 2;
        unsigned char ldo1_ocp_enn : 1;
        unsigned char ldo1_comp : 3;
        unsigned char ldo1_bw_en : 1;
    } reg;
} PMIC_MMW_LDO1_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_ocpd_vos_set_START (0)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_ocpd_vos_set_END (0)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_ocpd_set_START (1)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_ocpd_set_END (2)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_ocp_enn_START (3)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_ocp_enn_END (3)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_comp_START (4)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_comp_END (6)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_bw_en_START (7)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_bw_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo1_psrr1_set : 3;
        unsigned char ldo1_psrr_en : 1;
        unsigned char ldo1_ocps_set : 2;
        unsigned char ldo1_ocps_en : 1;
        unsigned char ldo1_ocps_bw_set : 1;
    } reg;
} PMIC_MMW_LDO1_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_psrr1_set_START (0)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_psrr1_set_END (2)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_psrr_en_START (3)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_psrr_en_END (3)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_ocps_set_START (4)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_ocps_set_END (5)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_ocps_en_START (6)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_ocps_en_END (6)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_ocps_bw_set_START (7)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_ocps_bw_set_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo1_vrset : 3;
        unsigned char ldo1_vgpr_en : 1;
        unsigned char ldo1_psrr2_set : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_MMW_LDO1_CFG_REG_2_UNION;
#endif
#define PMIC_MMW_LDO1_CFG_REG_2_ldo1_vrset_START (0)
#define PMIC_MMW_LDO1_CFG_REG_2_ldo1_vrset_END (2)
#define PMIC_MMW_LDO1_CFG_REG_2_ldo1_vgpr_en_START (3)
#define PMIC_MMW_LDO1_CFG_REG_2_ldo1_vgpr_en_END (3)
#define PMIC_MMW_LDO1_CFG_REG_2_ldo1_psrr2_set_START (4)
#define PMIC_MMW_LDO1_CFG_REG_2_ldo1_psrr2_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo2_ocpd_vos_set : 1;
        unsigned char ldo2_ocpd_set : 2;
        unsigned char ldo2_ocp_enn : 1;
        unsigned char ldo2_comp : 3;
        unsigned char ldo2_bw_en : 1;
    } reg;
} PMIC_MMW_LDO2_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_ocpd_vos_set_START (0)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_ocpd_vos_set_END (0)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_ocpd_set_START (1)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_ocpd_set_END (2)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_ocp_enn_START (3)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_ocp_enn_END (3)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_comp_START (4)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_comp_END (6)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_bw_en_START (7)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_bw_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo2_psrr1_set : 3;
        unsigned char ldo2_psrr_en : 1;
        unsigned char ldo2_ocps_set : 2;
        unsigned char ldo2_ocps_en : 1;
        unsigned char ldo2_ocps_bw_set : 1;
    } reg;
} PMIC_MMW_LDO2_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_psrr1_set_START (0)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_psrr1_set_END (2)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_psrr_en_START (3)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_psrr_en_END (3)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_ocps_set_START (4)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_ocps_set_END (5)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_ocps_en_START (6)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_ocps_en_END (6)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_ocps_bw_set_START (7)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_ocps_bw_set_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo2_vrset : 3;
        unsigned char ldo2_vgpr_en : 1;
        unsigned char ldo2_psrr2_set : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_MMW_LDO2_CFG_REG_2_UNION;
#endif
#define PMIC_MMW_LDO2_CFG_REG_2_ldo2_vrset_START (0)
#define PMIC_MMW_LDO2_CFG_REG_2_ldo2_vrset_END (2)
#define PMIC_MMW_LDO2_CFG_REG_2_ldo2_vgpr_en_START (3)
#define PMIC_MMW_LDO2_CFG_REG_2_ldo2_vgpr_en_END (3)
#define PMIC_MMW_LDO2_CFG_REG_2_ldo2_psrr2_set_START (4)
#define PMIC_MMW_LDO2_CFG_REG_2_ldo2_psrr2_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo3_ocpd_vos_set : 1;
        unsigned char ldo3_ocpd_set : 2;
        unsigned char ldo3_ocp_enn : 1;
        unsigned char ldo3_comp : 3;
        unsigned char ldo3_bw_en : 1;
    } reg;
} PMIC_MMW_LDO3_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_ocpd_vos_set_START (0)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_ocpd_vos_set_END (0)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_ocpd_set_START (1)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_ocpd_set_END (2)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_ocp_enn_START (3)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_ocp_enn_END (3)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_comp_START (4)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_comp_END (6)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_bw_en_START (7)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_bw_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo3_psrr1_set : 3;
        unsigned char ldo3_psrr_en : 1;
        unsigned char ldo3_ocps_set : 2;
        unsigned char ldo3_ocps_en : 1;
        unsigned char ldo3_ocps_bw_set : 1;
    } reg;
} PMIC_MMW_LDO3_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_psrr1_set_START (0)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_psrr1_set_END (2)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_psrr_en_START (3)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_psrr_en_END (3)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_ocps_set_START (4)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_ocps_set_END (5)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_ocps_en_START (6)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_ocps_en_END (6)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_ocps_bw_set_START (7)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_ocps_bw_set_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo3_vrset : 3;
        unsigned char ldo3_vgpr_en : 1;
        unsigned char ldo3_psrr2_set : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_MMW_LDO3_CFG_REG_2_UNION;
#endif
#define PMIC_MMW_LDO3_CFG_REG_2_ldo3_vrset_START (0)
#define PMIC_MMW_LDO3_CFG_REG_2_ldo3_vrset_END (2)
#define PMIC_MMW_LDO3_CFG_REG_2_ldo3_vgpr_en_START (3)
#define PMIC_MMW_LDO3_CFG_REG_2_ldo3_vgpr_en_END (3)
#define PMIC_MMW_LDO3_CFG_REG_2_ldo3_psrr2_set_START (4)
#define PMIC_MMW_LDO3_CFG_REG_2_ldo3_psrr2_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo4_ocpd_vos_set : 1;
        unsigned char ldo4_ocpd_set : 2;
        unsigned char ldo4_ocp_enn : 1;
        unsigned char ldo4_comp : 3;
        unsigned char ldo4_bw_en : 1;
    } reg;
} PMIC_MMW_LDO4_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_ocpd_vos_set_START (0)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_ocpd_vos_set_END (0)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_ocpd_set_START (1)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_ocpd_set_END (2)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_ocp_enn_START (3)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_ocp_enn_END (3)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_comp_START (4)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_comp_END (6)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_bw_en_START (7)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_bw_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo4_psrr1_set : 3;
        unsigned char ldo4_psrr_en : 1;
        unsigned char ldo4_ocps_set : 2;
        unsigned char ldo4_ocps_en : 1;
        unsigned char ldo4_ocps_bw_set : 1;
    } reg;
} PMIC_MMW_LDO4_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_psrr1_set_START (0)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_psrr1_set_END (2)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_psrr_en_START (3)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_psrr_en_END (3)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_ocps_set_START (4)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_ocps_set_END (5)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_ocps_en_START (6)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_ocps_en_END (6)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_ocps_bw_set_START (7)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_ocps_bw_set_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo4_vrset : 3;
        unsigned char ldo4_vgpr_en : 1;
        unsigned char ldo4_psrr2_set : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_MMW_LDO4_CFG_REG_2_UNION;
#endif
#define PMIC_MMW_LDO4_CFG_REG_2_ldo4_vrset_START (0)
#define PMIC_MMW_LDO4_CFG_REG_2_ldo4_vrset_END (2)
#define PMIC_MMW_LDO4_CFG_REG_2_ldo4_vgpr_en_START (3)
#define PMIC_MMW_LDO4_CFG_REG_2_ldo4_vgpr_en_END (3)
#define PMIC_MMW_LDO4_CFG_REG_2_ldo4_psrr2_set_START (4)
#define PMIC_MMW_LDO4_CFG_REG_2_ldo4_psrr2_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo5_psrr1_set : 3;
        unsigned char ldo5_psrr_en : 1;
        unsigned char ldo5_ocp_set : 2;
        unsigned char ldo5_ocp_enn : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_MMW_LDO5_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_psrr1_set_START (0)
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_psrr1_set_END (2)
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_psrr_en_START (3)
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_psrr_en_END (3)
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_ocp_set_START (4)
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_ocp_set_END (5)
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_ocp_enn_START (6)
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_ocp_enn_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo5_vrset : 3;
        unsigned char ldo5_psrr2_set : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_MMW_LDO5_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO5_CFG_REG_1_ldo5_vrset_START (0)
#define PMIC_MMW_LDO5_CFG_REG_1_ldo5_vrset_END (2)
#define PMIC_MMW_LDO5_CFG_REG_1_ldo5_psrr2_set_START (3)
#define PMIC_MMW_LDO5_CFG_REG_1_ldo5_psrr2_set_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo6_psrr1_set : 3;
        unsigned char ldo6_psrr_en : 1;
        unsigned char ldo6_ocp_set : 2;
        unsigned char ldo6_ocp_enn : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_MMW_LDO6_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_psrr1_set_START (0)
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_psrr1_set_END (2)
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_psrr_en_START (3)
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_psrr_en_END (3)
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_ocp_set_START (4)
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_ocp_set_END (5)
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_ocp_enn_START (6)
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_ocp_enn_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo6_vrset : 3;
        unsigned char ldo6_psrr2_set : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_MMW_LDO6_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO6_CFG_REG_1_ldo6_vrset_START (0)
#define PMIC_MMW_LDO6_CFG_REG_1_ldo6_vrset_END (2)
#define PMIC_MMW_LDO6_CFG_REG_1_ldo6_psrr2_set_START (3)
#define PMIC_MMW_LDO6_CFG_REG_1_ldo6_psrr2_set_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo7_ocps_set : 2;
        unsigned char ldo7_ocps_en : 1;
        unsigned char ldo7_ocps_bw_set : 1;
        unsigned char ldo7_ocpd_vos_set : 1;
        unsigned char ldo7_ocpd_set : 2;
        unsigned char ldo7_ocp_enn : 1;
    } reg;
} PMIC_MMW_LDO7_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocps_set_START (0)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocps_set_END (1)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocps_en_START (2)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocps_en_END (2)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocps_bw_set_START (3)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocps_bw_set_END (3)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocpd_vos_set_START (4)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocpd_vos_set_END (4)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocpd_set_START (5)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocpd_set_END (6)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocp_enn_START (7)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocp_enn_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo7_vrset : 3;
        unsigned char ldo7_vgpr_en : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO7_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO7_CFG_REG_1_ldo7_vrset_START (0)
#define PMIC_MMW_LDO7_CFG_REG_1_ldo7_vrset_END (2)
#define PMIC_MMW_LDO7_CFG_REG_1_ldo7_vgpr_en_START (3)
#define PMIC_MMW_LDO7_CFG_REG_1_ldo7_vgpr_en_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo8_ocps_set : 2;
        unsigned char ldo8_ocps_en : 1;
        unsigned char ldo8_ocps_bw_set : 1;
        unsigned char ldo8_ocpd_vos_set : 1;
        unsigned char ldo8_ocpd_set : 2;
        unsigned char ldo8_ocp_enn : 1;
    } reg;
} PMIC_MMW_LDO8_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocps_set_START (0)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocps_set_END (1)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocps_en_START (2)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocps_en_END (2)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocps_bw_set_START (3)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocps_bw_set_END (3)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocpd_vos_set_START (4)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocpd_vos_set_END (4)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocpd_set_START (5)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocpd_set_END (6)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocp_enn_START (7)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocp_enn_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo8_vrset : 3;
        unsigned char ldo8_vgpr_en : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO8_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO8_CFG_REG_1_ldo8_vrset_START (0)
#define PMIC_MMW_LDO8_CFG_REG_1_ldo8_vrset_END (2)
#define PMIC_MMW_LDO8_CFG_REG_1_ldo8_vgpr_en_START (3)
#define PMIC_MMW_LDO8_CFG_REG_1_ldo8_vgpr_en_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_core_vrset : 3;
        unsigned char ldo_core_ocp_set : 2;
        unsigned char ldo_core_ocp_enn : 1;
        unsigned char ldo_core_i_sst : 1;
        unsigned char ldo_core_eco_set : 1;
    } reg;
} PMIC_MMW_LDO_CORE_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_vrset_START (0)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_vrset_END (2)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_ocp_set_START (3)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_ocp_set_END (4)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_ocp_enn_START (5)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_ocp_enn_END (5)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_i_sst_START (6)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_i_sst_END (6)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_eco_set_START (7)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_eco_set_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_buf0_ocp_set : 2;
        unsigned char ldo_buf0_curr_sel : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO_BUF0_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_BUF0_CFG_REG_0_ldo_buf0_ocp_set_START (0)
#define PMIC_MMW_LDO_BUF0_CFG_REG_0_ldo_buf0_ocp_set_END (1)
#define PMIC_MMW_LDO_BUF0_CFG_REG_0_ldo_buf0_curr_sel_START (2)
#define PMIC_MMW_LDO_BUF0_CFG_REG_0_ldo_buf0_curr_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_buf1_test : 1;
        unsigned char ldo_buf1_sw : 2;
        unsigned char ldo_buf1_curr_sel : 2;
        unsigned char reserved : 3;
    } reg;
} PMIC_MMW_LDO_BUF1_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ldo_buf1_test_START (0)
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ldo_buf1_test_END (0)
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ldo_buf1_sw_START (1)
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ldo_buf1_sw_END (2)
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ldo_buf1_curr_sel_START (3)
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ldo_buf1_curr_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_rfbuf_ocp_set : 2;
        unsigned char ldo_rfbuf_curr_sel : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_LDO_BUF_RF_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_BUF_RF_CFG_REG_0_ldo_rfbuf_ocp_set_START (0)
#define PMIC_MMW_LDO_BUF_RF_CFG_REG_0_ldo_rfbuf_ocp_set_END (1)
#define PMIC_MMW_LDO_BUF_RF_CFG_REG_0_ldo_rfbuf_curr_sel_START (2)
#define PMIC_MMW_LDO_BUF_RF_CFG_REG_0_ldo_rfbuf_curr_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_rfbuf0_test : 1;
        unsigned char ldo_rfbuf0_sw : 2;
        unsigned char ldo_rfbuf0_curr_sel : 2;
        unsigned char reserved : 3;
    } reg;
} PMIC_MMW_LDO_RFBUF0_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ldo_rfbuf0_test_START (0)
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ldo_rfbuf0_test_END (0)
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ldo_rfbuf0_sw_START (1)
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ldo_rfbuf0_sw_END (2)
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ldo_rfbuf0_curr_sel_START (3)
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ldo_rfbuf0_curr_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_rfbuf1_test : 1;
        unsigned char ldo_rfbuf1_sw : 2;
        unsigned char ldo_rfbuf1_curr_sel : 2;
        unsigned char reserved : 3;
    } reg;
} PMIC_MMW_LDO_RFBUF1_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ldo_rfbuf1_test_START (0)
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ldo_rfbuf1_test_END (0)
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ldo_rfbuf1_sw_START (1)
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ldo_rfbuf1_sw_END (2)
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ldo_rfbuf1_curr_sel_START (3)
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ldo_rfbuf1_curr_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_rfbuf2_test : 1;
        unsigned char ldo_rfbuf2_sw : 2;
        unsigned char ldo_rfbuf2_curr_sel : 2;
        unsigned char reserved : 3;
    } reg;
} PMIC_MMW_LDO_RFBUF2_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ldo_rfbuf2_test_START (0)
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ldo_rfbuf2_test_END (0)
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ldo_rfbuf2_sw_START (1)
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ldo_rfbuf2_sw_END (2)
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ldo_rfbuf2_curr_sel_START (3)
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ldo_rfbuf2_curr_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_rfbuf3_test : 1;
        unsigned char ldo_rfbuf3_sw : 2;
        unsigned char ldo_rfbuf3_curr_sel : 2;
        unsigned char reserved : 3;
    } reg;
} PMIC_MMW_LDO_RFBUF3_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ldo_rfbuf3_test_START (0)
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ldo_rfbuf3_test_END (0)
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ldo_rfbuf3_sw_START (1)
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ldo_rfbuf3_sw_END (2)
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ldo_rfbuf3_curr_sel_START (3)
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ldo_rfbuf3_curr_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_rfbuf4_test : 1;
        unsigned char ldo_rfbuf4_sw : 2;
        unsigned char ldo_rfbuf4_curr_sel : 2;
        unsigned char reserved : 3;
    } reg;
} PMIC_MMW_LDO_RFBUF4_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ldo_rfbuf4_test_START (0)
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ldo_rfbuf4_test_END (0)
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ldo_rfbuf4_sw_START (1)
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ldo_rfbuf4_sw_END (2)
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ldo_rfbuf4_curr_sel_START (3)
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ldo_rfbuf4_curr_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pmuh_vrset : 3;
        unsigned char pmuh_ocp_set : 2;
        unsigned char pmuh_ocp_enn : 1;
        unsigned char pmuh_mode_sel : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_MMW_PMUH_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_vrset_START (0)
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_vrset_END (2)
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_ocp_set_START (3)
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_ocp_set_END (4)
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_ocp_enn_START (5)
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_ocp_enn_END (5)
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_mode_sel_START (6)
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_mode_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve0 : 8;
    } reg;
} PMIC_MMW_LDO_RESERVE_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_RESERVE_CFG_REG_0_ldo_reserve0_START (0)
#define PMIC_MMW_LDO_RESERVE_CFG_REG_0_ldo_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve1 : 8;
    } reg;
} PMIC_MMW_LDO_RESERVE_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO_RESERVE_CFG_REG_1_ldo_reserve1_START (0)
#define PMIC_MMW_LDO_RESERVE_CFG_REG_1_ldo_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve2 : 8;
    } reg;
} PMIC_MMW_LDO_RESERVE_CFG_REG_2_UNION;
#endif
#define PMIC_MMW_LDO_RESERVE_CFG_REG_2_ldo_reserve2_START (0)
#define PMIC_MMW_LDO_RESERVE_CFG_REG_2_ldo_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve3 : 8;
    } reg;
} PMIC_MMW_LDO_RESERVE_CFG_REG_3_UNION;
#endif
#define PMIC_MMW_LDO_RESERVE_CFG_REG_3_ldo_reserve3_START (0)
#define PMIC_MMW_LDO_RESERVE_CFG_REG_3_ldo_reserve3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_ufs_drv : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_CLK_UFS_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_UFS_CTRL_xo_ufs_drv_START (0)
#define PMIC_MMW_CLK_UFS_CTRL_xo_ufs_drv_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_rf0_drv : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_MMW_CLK_RF0_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF0_CTRL_xo_rf0_drv_START (0)
#define PMIC_MMW_CLK_RF0_CTRL_xo_rf0_drv_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_rf1_drv : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_MMW_CLK_RF1_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF1_CTRL_xo_rf1_drv_START (0)
#define PMIC_MMW_CLK_RF1_CTRL_xo_rf1_drv_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_rf2_drv : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_MMW_CLK_RF2_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF2_CTRL_xo_rf2_drv_START (0)
#define PMIC_MMW_CLK_RF2_CTRL_xo_rf2_drv_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_rf3_drv : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_MMW_CLK_RF3_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF3_CTRL_xo_rf3_drv_START (0)
#define PMIC_MMW_CLK_RF3_CTRL_xo_rf3_drv_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_rf4_drv : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_MMW_CLK_RF4_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF4_CTRL_xo_rf4_drv_START (0)
#define PMIC_MMW_CLK_RF4_CTRL_xo_rf4_drv_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_serdes_drv : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_MMW_CLK_SERDES_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_SERDES_CTRL_xo_serdes_drv_START (0)
#define PMIC_MMW_CLK_SERDES_CTRL_xo_serdes_drv_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_wifi0_drv : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_MMW_CLK_WIFI0_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_WIFI0_CTRL_xo_wifi0_drv_START (0)
#define PMIC_MMW_CLK_WIFI0_CTRL_xo_wifi0_drv_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_wifi1_drv : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_MMW_CLK_WIFI1_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_WIFI1_CTRL_xo_wifi1_drv_START (0)
#define PMIC_MMW_CLK_WIFI1_CTRL_xo_wifi1_drv_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_nfc_drv : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_CLK_NFC_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_NFC_CTRL_xo_nfc_drv_START (0)
#define PMIC_MMW_CLK_NFC_CTRL_xo_nfc_drv_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_sys_drv : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_CLK_SYS_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_SYS_CTRL_xo_sys_drv_START (0)
#define PMIC_MMW_CLK_SYS_CTRL_xo_sys_drv_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_pmu_drv : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_CLK_PMU_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_PMU_CTRL_xo_pmu_drv_START (0)
#define PMIC_MMW_CLK_PMU_CTRL_xo_pmu_drv_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_rf1_ph_sel : 1;
        unsigned char xo_tri_cap : 1;
        unsigned char xo_delay_sel : 2;
        unsigned char reserved : 2;
        unsigned char xo_eco_force_en : 1;
        unsigned char xo_eco_en_shield : 1;
    } reg;
} PMIC_MMW_XO_CTRL0_UNION;
#endif
#define PMIC_MMW_XO_CTRL0_xo_rf1_ph_sel_START (0)
#define PMIC_MMW_XO_CTRL0_xo_rf1_ph_sel_END (0)
#define PMIC_MMW_XO_CTRL0_xo_tri_cap_START (1)
#define PMIC_MMW_XO_CTRL0_xo_tri_cap_END (1)
#define PMIC_MMW_XO_CTRL0_xo_delay_sel_START (2)
#define PMIC_MMW_XO_CTRL0_xo_delay_sel_END (3)
#define PMIC_MMW_XO_CTRL0_xo_eco_force_en_START (6)
#define PMIC_MMW_XO_CTRL0_xo_eco_force_en_END (6)
#define PMIC_MMW_XO_CTRL0_xo_eco_en_shield_START (7)
#define PMIC_MMW_XO_CTRL0_xo_eco_en_shield_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_reserve0 : 8;
    } reg;
} PMIC_MMW_XO_RESERVE_0_UNION;
#endif
#define PMIC_MMW_XO_RESERVE_0_xo_reserve0_START (0)
#define PMIC_MMW_XO_RESERVE_0_xo_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_corner_det_valid : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_XO_DET_STATUS_UNION;
#endif
#define PMIC_MMW_XO_DET_STATUS_xo_corner_det_valid_START (0)
#define PMIC_MMW_XO_DET_STATUS_xo_corner_det_valid_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char thsd_tmp_set : 2;
        unsigned char reserved_0 : 1;
        unsigned char ref_out_sel : 2;
        unsigned char ref_ibias_trim_en : 2;
        unsigned char reserved_1 : 1;
    } reg;
} PMIC_MMW_BG_THSD_CTRL0_UNION;
#endif
#define PMIC_MMW_BG_THSD_CTRL0_thsd_tmp_set_START (0)
#define PMIC_MMW_BG_THSD_CTRL0_thsd_tmp_set_END (1)
#define PMIC_MMW_BG_THSD_CTRL0_ref_out_sel_START (3)
#define PMIC_MMW_BG_THSD_CTRL0_ref_out_sel_END (4)
#define PMIC_MMW_BG_THSD_CTRL0_ref_ibias_trim_en_START (5)
#define PMIC_MMW_BG_THSD_CTRL0_ref_ibias_trim_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ref_res_sel : 1;
        unsigned char ref_chop_clk_sel : 2;
        unsigned char ref_chop_sel : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_BG_THSD_CTRL1_UNION;
#endif
#define PMIC_MMW_BG_THSD_CTRL1_ref_res_sel_START (0)
#define PMIC_MMW_BG_THSD_CTRL1_ref_res_sel_END (0)
#define PMIC_MMW_BG_THSD_CTRL1_ref_chop_clk_sel_START (1)
#define PMIC_MMW_BG_THSD_CTRL1_ref_chop_clk_sel_END (2)
#define PMIC_MMW_BG_THSD_CTRL1_ref_chop_sel_START (3)
#define PMIC_MMW_BG_THSD_CTRL1_ref_chop_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ref_reserve0 : 8;
    } reg;
} PMIC_MMW_BG_RESERVE_0_UNION;
#endif
#define PMIC_MMW_BG_RESERVE_0_ref_reserve0_START (0)
#define PMIC_MMW_BG_RESERVE_0_ref_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ref_reserve1 : 8;
    } reg;
} PMIC_MMW_BG_RESERVE_1_UNION;
#endif
#define PMIC_MMW_BG_RESERVE_1_ref_reserve1_START (0)
#define PMIC_MMW_BG_RESERVE_1_ref_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ref_reserve2 : 8;
    } reg;
} PMIC_MMW_BG_RESERVE_2_UNION;
#endif
#define PMIC_MMW_BG_RESERVE_2_ref_reserve2_START (0)
#define PMIC_MMW_BG_RESERVE_2_ref_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_ctrl_reserve : 8;
    } reg;
} PMIC_MMW_SYS_CTRL_RESERVE_UNION;
#endif
#define PMIC_MMW_SYS_CTRL_RESERVE_sys_ctrl_reserve_START (0)
#define PMIC_MMW_SYS_CTRL_RESERVE_sys_ctrl_reserve_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pmu_test_buf_en : 1;
        unsigned char pmu_test_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_SYS_CTRL0_UNION;
#endif
#define PMIC_MMW_SYS_CTRL0_pmu_test_buf_en_START (0)
#define PMIC_MMW_SYS_CTRL0_pmu_test_buf_en_END (0)
#define PMIC_MMW_SYS_CTRL0_pmu_test_en_START (1)
#define PMIC_MMW_SYS_CTRL0_pmu_test_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pmu_test_sel : 8;
    } reg;
} PMIC_MMW_SYS_CTRL1_UNION;
#endif
#define PMIC_MMW_SYS_CTRL1_pmu_test_sel_START (0)
#define PMIC_MMW_SYS_CTRL1_pmu_test_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char a2d_reserve0 : 8;
    } reg;
} PMIC_MMW_A2D_RES0_UNION;
#endif
#define PMIC_MMW_A2D_RES0_a2d_reserve0_START (0)
#define PMIC_MMW_A2D_RES0_a2d_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char d2a_reserve0 : 8;
    } reg;
} PMIC_MMW_D2A_RES0_UNION;
#endif
#define PMIC_MMW_D2A_RES0_d2a_reserve0_START (0)
#define PMIC_MMW_D2A_RES0_d2a_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char d2a_reserve1 : 8;
    } reg;
} PMIC_MMW_D2A_RES1_UNION;
#endif
#define PMIC_MMW_D2A_RES1_d2a_reserve1_START (0)
#define PMIC_MMW_D2A_RES1_d2a_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char d2a_reserve2 : 8;
    } reg;
} PMIC_MMW_D2A_RES2_UNION;
#endif
#define PMIC_MMW_D2A_RES2_d2a_reserve2_START (0)
#define PMIC_MMW_D2A_RES2_d2a_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char d2a_reserve3 : 8;
    } reg;
} PMIC_MMW_D2A_RES3_UNION;
#endif
#define PMIC_MMW_D2A_RES3_d2a_reserve3_START (0)
#define PMIC_MMW_D2A_RES3_d2a_reserve3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob0 : 8;
    } reg;
} PMIC_MMW_OTP_0_UNION;
#endif
#define PMIC_MMW_OTP_0_otp_pdob0_START (0)
#define PMIC_MMW_OTP_0_otp_pdob0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob1 : 8;
    } reg;
} PMIC_MMW_OTP_1_UNION;
#endif
#define PMIC_MMW_OTP_1_otp_pdob1_START (0)
#define PMIC_MMW_OTP_1_otp_pdob1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_clk_ctrl : 8;
    } reg;
} PMIC_MMW_OTP_CLK_CTRL_UNION;
#endif
#define PMIC_MMW_OTP_CLK_CTRL_otp_clk_ctrl_START (0)
#define PMIC_MMW_OTP_CLK_CTRL_otp_clk_ctrl_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pwe_int : 1;
        unsigned char otp_pwe_pulse : 1;
        unsigned char otp_por_int : 1;
        unsigned char otp_por_pulse : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_OTP_CTRL0_UNION;
#endif
#define PMIC_MMW_OTP_CTRL0_otp_pwe_int_START (0)
#define PMIC_MMW_OTP_CTRL0_otp_pwe_int_END (0)
#define PMIC_MMW_OTP_CTRL0_otp_pwe_pulse_START (1)
#define PMIC_MMW_OTP_CTRL0_otp_pwe_pulse_END (1)
#define PMIC_MMW_OTP_CTRL0_otp_por_int_START (2)
#define PMIC_MMW_OTP_CTRL0_otp_por_int_END (2)
#define PMIC_MMW_OTP_CTRL0_otp_por_pulse_START (3)
#define PMIC_MMW_OTP_CTRL0_otp_por_pulse_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pprog : 1;
        unsigned char otp_inctrl_sel : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_OTP_CTRL1_UNION;
#endif
#define PMIC_MMW_OTP_CTRL1_otp_pprog_START (0)
#define PMIC_MMW_OTP_CTRL1_otp_pprog_END (0)
#define PMIC_MMW_OTP_CTRL1_otp_inctrl_sel_START (1)
#define PMIC_MMW_OTP_CTRL1_otp_inctrl_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pa_cfg : 6;
        unsigned char otp_ptm : 2;
    } reg;
} PMIC_MMW_OTP_CTRL2_UNION;
#endif
#define PMIC_MMW_OTP_CTRL2_otp_pa_cfg_START (0)
#define PMIC_MMW_OTP_CTRL2_otp_pa_cfg_END (5)
#define PMIC_MMW_OTP_CTRL2_otp_ptm_START (6)
#define PMIC_MMW_OTP_CTRL2_otp_ptm_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdin : 8;
    } reg;
} PMIC_MMW_OTP_WDATA_UNION;
#endif
#define PMIC_MMW_OTP_WDATA_otp_pdin_START (0)
#define PMIC_MMW_OTP_WDATA_otp_pdin_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob0_w : 8;
    } reg;
} PMIC_MMW_OTP_0_W_UNION;
#endif
#define PMIC_MMW_OTP_0_W_otp_pdob0_w_START (0)
#define PMIC_MMW_OTP_0_W_otp_pdob0_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob1_w : 8;
    } reg;
} PMIC_MMW_OTP_1_W_UNION;
#endif
#define PMIC_MMW_OTP_1_W_otp_pdob1_w_START (0)
#define PMIC_MMW_OTP_1_W_otp_pdob1_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob2_w : 8;
    } reg;
} PMIC_MMW_OTP_2_W_UNION;
#endif
#define PMIC_MMW_OTP_2_W_otp_pdob2_w_START (0)
#define PMIC_MMW_OTP_2_W_otp_pdob2_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob3_w : 8;
    } reg;
} PMIC_MMW_OTP_3_W_UNION;
#endif
#define PMIC_MMW_OTP_3_W_otp_pdob3_w_START (0)
#define PMIC_MMW_OTP_3_W_otp_pdob3_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob4_w : 8;
    } reg;
} PMIC_MMW_OTP_4_W_UNION;
#endif
#define PMIC_MMW_OTP_4_W_otp_pdob4_w_START (0)
#define PMIC_MMW_OTP_4_W_otp_pdob4_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob5_w : 8;
    } reg;
} PMIC_MMW_OTP_5_W_UNION;
#endif
#define PMIC_MMW_OTP_5_W_otp_pdob5_w_START (0)
#define PMIC_MMW_OTP_5_W_otp_pdob5_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob6_w : 8;
    } reg;
} PMIC_MMW_OTP_6_W_UNION;
#endif
#define PMIC_MMW_OTP_6_W_otp_pdob6_w_START (0)
#define PMIC_MMW_OTP_6_W_otp_pdob6_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob7_w : 8;
    } reg;
} PMIC_MMW_OTP_7_W_UNION;
#endif
#define PMIC_MMW_OTP_7_W_otp_pdob7_w_START (0)
#define PMIC_MMW_OTP_7_W_otp_pdob7_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob8_w : 8;
    } reg;
} PMIC_MMW_OTP_8_W_UNION;
#endif
#define PMIC_MMW_OTP_8_W_otp_pdob8_w_START (0)
#define PMIC_MMW_OTP_8_W_otp_pdob8_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob9_w : 8;
    } reg;
} PMIC_MMW_OTP_9_W_UNION;
#endif
#define PMIC_MMW_OTP_9_W_otp_pdob9_w_START (0)
#define PMIC_MMW_OTP_9_W_otp_pdob9_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob10_w : 8;
    } reg;
} PMIC_MMW_OTP_10_W_UNION;
#endif
#define PMIC_MMW_OTP_10_W_otp_pdob10_w_START (0)
#define PMIC_MMW_OTP_10_W_otp_pdob10_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob11_w : 8;
    } reg;
} PMIC_MMW_OTP_11_W_UNION;
#endif
#define PMIC_MMW_OTP_11_W_otp_pdob11_w_START (0)
#define PMIC_MMW_OTP_11_W_otp_pdob11_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob12_w : 8;
    } reg;
} PMIC_MMW_OTP_12_W_UNION;
#endif
#define PMIC_MMW_OTP_12_W_otp_pdob12_w_START (0)
#define PMIC_MMW_OTP_12_W_otp_pdob12_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob13_w : 8;
    } reg;
} PMIC_MMW_OTP_13_W_UNION;
#endif
#define PMIC_MMW_OTP_13_W_otp_pdob13_w_START (0)
#define PMIC_MMW_OTP_13_W_otp_pdob13_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob14_w : 8;
    } reg;
} PMIC_MMW_OTP_14_W_UNION;
#endif
#define PMIC_MMW_OTP_14_W_otp_pdob14_w_START (0)
#define PMIC_MMW_OTP_14_W_otp_pdob14_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob15_w : 8;
    } reg;
} PMIC_MMW_OTP_15_W_UNION;
#endif
#define PMIC_MMW_OTP_15_W_otp_pdob15_w_START (0)
#define PMIC_MMW_OTP_15_W_otp_pdob15_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob16_w : 8;
    } reg;
} PMIC_MMW_OTP_16_W_UNION;
#endif
#define PMIC_MMW_OTP_16_W_otp_pdob16_w_START (0)
#define PMIC_MMW_OTP_16_W_otp_pdob16_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob17_w : 8;
    } reg;
} PMIC_MMW_OTP_17_W_UNION;
#endif
#define PMIC_MMW_OTP_17_W_otp_pdob17_w_START (0)
#define PMIC_MMW_OTP_17_W_otp_pdob17_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob18_w : 8;
    } reg;
} PMIC_MMW_OTP_18_W_UNION;
#endif
#define PMIC_MMW_OTP_18_W_otp_pdob18_w_START (0)
#define PMIC_MMW_OTP_18_W_otp_pdob18_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob19_w : 8;
    } reg;
} PMIC_MMW_OTP_19_W_UNION;
#endif
#define PMIC_MMW_OTP_19_W_otp_pdob19_w_START (0)
#define PMIC_MMW_OTP_19_W_otp_pdob19_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob20_w : 8;
    } reg;
} PMIC_MMW_OTP_20_W_UNION;
#endif
#define PMIC_MMW_OTP_20_W_otp_pdob20_w_START (0)
#define PMIC_MMW_OTP_20_W_otp_pdob20_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp_pdob21_w : 8;
    } reg;
} PMIC_MMW_OTP_21_W_UNION;
#endif
#define PMIC_MMW_OTP_21_W_otp_pdob21_w_START (0)
#define PMIC_MMW_OTP_21_W_otp_pdob21_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo3_ocp_auto_stop : 2;
        unsigned char ldo2_ocp_auto_stop : 2;
        unsigned char ldo1_ocp_auto_stop : 2;
        unsigned char ldo0_ocp_auto_stop : 2;
    } reg;
} PMIC_MMW_LDO0_3_OCP_CTRL_UNION;
#endif
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo3_ocp_auto_stop_START (0)
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo3_ocp_auto_stop_END (1)
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo2_ocp_auto_stop_START (2)
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo2_ocp_auto_stop_END (3)
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo1_ocp_auto_stop_START (4)
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo1_ocp_auto_stop_END (5)
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo0_ocp_auto_stop_START (6)
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo0_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo7_ocp_auto_stop : 2;
        unsigned char ldo6_ocp_auto_stop : 2;
        unsigned char ldo5_ocp_auto_stop : 2;
        unsigned char ldo4_ocp_auto_stop : 2;
    } reg;
} PMIC_MMW_LDO4_7_OCP_CTRL_UNION;
#endif
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo7_ocp_auto_stop_START (0)
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo7_ocp_auto_stop_END (1)
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo6_ocp_auto_stop_START (2)
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo6_ocp_auto_stop_END (3)
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo5_ocp_auto_stop_START (4)
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo5_ocp_auto_stop_END (5)
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo4_ocp_auto_stop_START (6)
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo4_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_rfbuf_ocp_auto_stop : 2;
        unsigned char ldo_buf0_ocp_auto_stop : 2;
        unsigned char ldo_core_ocp_auto_stop : 2;
        unsigned char ldo8_ocp_auto_stop : 2;
    } reg;
} PMIC_MMW_LDO8_12_OCP_CTRL_UNION;
#endif
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo_rfbuf_ocp_auto_stop_START (0)
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo_rfbuf_ocp_auto_stop_END (1)
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo_buf0_ocp_auto_stop_START (2)
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo_buf0_ocp_auto_stop_END (3)
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo_core_ocp_auto_stop_START (4)
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo_core_ocp_auto_stop_END (5)
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo8_ocp_auto_stop_START (6)
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo8_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_ocp_deb_sel : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_OCP_DEB_CTRL0_UNION;
#endif
#define PMIC_MMW_OCP_DEB_CTRL0_ldo_ocp_deb_sel_START (0)
#define PMIC_MMW_OCP_DEB_CTRL0_ldo_ocp_deb_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char vin_ldo_0p9_deb_sel : 2;
        unsigned char vin_ldo_1p1_deb_sel : 2;
        unsigned char vin_ldo_1p3_deb_sel : 2;
        unsigned char reserved : 2;
    } reg;
} PMIC_MMW_OCP_DEB_CTRL1_UNION;
#endif
#define PMIC_MMW_OCP_DEB_CTRL1_vin_ldo_0p9_deb_sel_START (0)
#define PMIC_MMW_OCP_DEB_CTRL1_vin_ldo_0p9_deb_sel_END (1)
#define PMIC_MMW_OCP_DEB_CTRL1_vin_ldo_1p1_deb_sel_START (2)
#define PMIC_MMW_OCP_DEB_CTRL1_vin_ldo_1p1_deb_sel_END (3)
#define PMIC_MMW_OCP_DEB_CTRL1_vin_ldo_1p3_deb_sel_START (4)
#define PMIC_MMW_OCP_DEB_CTRL1_vin_ldo_1p3_deb_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char thsd_otmp140_deb_sel : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_THSD_140_DEB_CTRL_UNION;
#endif
#define PMIC_MMW_THSD_140_DEB_CTRL_thsd_otmp140_deb_sel_START (0)
#define PMIC_MMW_THSD_140_DEB_CTRL_thsd_otmp140_deb_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char en_vin_ldo_0p9_deb : 1;
        unsigned char en_vin_ldo_1p1_deb : 1;
        unsigned char en_vin_ldo_1p3_deb : 1;
        unsigned char en_ldo_ocp_deb : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_OCP_SCP_ONOFF_UNION;
#endif
#define PMIC_MMW_OCP_SCP_ONOFF_en_vin_ldo_0p9_deb_START (0)
#define PMIC_MMW_OCP_SCP_ONOFF_en_vin_ldo_0p9_deb_END (0)
#define PMIC_MMW_OCP_SCP_ONOFF_en_vin_ldo_1p1_deb_START (1)
#define PMIC_MMW_OCP_SCP_ONOFF_en_vin_ldo_1p1_deb_END (1)
#define PMIC_MMW_OCP_SCP_ONOFF_en_vin_ldo_1p3_deb_START (2)
#define PMIC_MMW_OCP_SCP_ONOFF_en_vin_ldo_1p3_deb_END (2)
#define PMIC_MMW_OCP_SCP_ONOFF_en_ldo_ocp_deb_START (3)
#define PMIC_MMW_OCP_SCP_ONOFF_en_ldo_ocp_deb_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo_buf0_hd_mask : 1;
        unsigned char reg_ldo_buf1_hd_mask : 1;
        unsigned char reg_pmuh_hd_mask : 1;
        unsigned char reg_ldo_core_hd_mask : 1;
        unsigned char reg_eco_in_hd_mask : 1;
        unsigned char reg_thsd_eco_hd_mask : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_MMW_HARDWIRE_CTRL0_UNION;
#endif
#define PMIC_MMW_HARDWIRE_CTRL0_reg_ldo_buf0_hd_mask_START (0)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_ldo_buf0_hd_mask_END (0)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_ldo_buf1_hd_mask_START (1)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_ldo_buf1_hd_mask_END (1)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_pmuh_hd_mask_START (2)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_pmuh_hd_mask_END (2)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_ldo_core_hd_mask_START (3)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_ldo_core_hd_mask_END (3)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_eco_in_hd_mask_START (4)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_eco_in_hd_mask_END (4)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_thsd_eco_hd_mask_START (5)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_thsd_eco_hd_mask_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo_core_vset_mask : 1;
        unsigned char reg_xo_inbuff_curr_mask : 1;
        unsigned char reg_xo_core_curr_mask : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_MMW_HARDWIRE_CTRL1_UNION;
#endif
#define PMIC_MMW_HARDWIRE_CTRL1_reg_ldo_core_vset_mask_START (0)
#define PMIC_MMW_HARDWIRE_CTRL1_reg_ldo_core_vset_mask_END (0)
#define PMIC_MMW_HARDWIRE_CTRL1_reg_xo_inbuff_curr_mask_START (1)
#define PMIC_MMW_HARDWIRE_CTRL1_reg_xo_inbuff_curr_mask_END (1)
#define PMIC_MMW_HARDWIRE_CTRL1_reg_xo_core_curr_mask_START (2)
#define PMIC_MMW_HARDWIRE_CTRL1_reg_xo_core_curr_mask_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_serdes_clk_hd_mask : 1;
        unsigned char reg_sys_clk_hd_mask : 1;
        unsigned char reg_wifi0_clk_hd_mask : 1;
        unsigned char reg_wifi1_clk_hd_mask : 1;
        unsigned char reg_nfc_clk_hd_mask : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_MMW_HARDWIRE_CTRL2_UNION;
#endif
#define PMIC_MMW_HARDWIRE_CTRL2_reg_serdes_clk_hd_mask_START (0)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_serdes_clk_hd_mask_END (0)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_sys_clk_hd_mask_START (1)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_sys_clk_hd_mask_END (1)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_wifi0_clk_hd_mask_START (2)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_wifi0_clk_hd_mask_END (2)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_wifi1_clk_hd_mask_START (3)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_wifi1_clk_hd_mask_END (3)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_nfc_clk_hd_mask_START (4)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_nfc_clk_hd_mask_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_sys_pmu_clk_hd_mask : 8;
    } reg;
} PMIC_MMW_HARDWIRE_CTRL3_UNION;
#endif
#define PMIC_MMW_HARDWIRE_CTRL3_reg_sys_pmu_clk_hd_mask_START (0)
#define PMIC_MMW_HARDWIRE_CTRL3_reg_sys_pmu_clk_hd_mask_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_wifi_pmu_clk_hd_mask : 8;
    } reg;
} PMIC_MMW_HARDWIRE_CTRL4_UNION;
#endif
#define PMIC_MMW_HARDWIRE_CTRL4_reg_wifi_pmu_clk_hd_mask_START (0)
#define PMIC_MMW_HARDWIRE_CTRL4_reg_wifi_pmu_clk_hd_mask_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_nfc_pmu_clk_hd_mask : 8;
    } reg;
} PMIC_MMW_HARDWIRE_CTRL5_UNION;
#endif
#define PMIC_MMW_HARDWIRE_CTRL5_reg_nfc_pmu_clk_hd_mask_START (0)
#define PMIC_MMW_HARDWIRE_CTRL5_reg_nfc_pmu_clk_hd_mask_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lock : 8;
    } reg;
} PMIC_MMW_LOCK_UNION;
#endif
#define PMIC_MMW_LOCK_lock_START (0)
#define PMIC_MMW_LOCK_lock_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_eco_gt_bypass : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_SPMI_ECO_GT_BYPASS_UNION;
#endif
#define PMIC_MMW_SPMI_ECO_GT_BYPASS_spmi_eco_gt_bypass_START (0)
#define PMIC_MMW_SPMI_ECO_GT_BYPASS_spmi_eco_gt_bypass_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo5_sst_end_sel : 1;
        unsigned char ldo6_sst_end_sel : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_LDO_SST_END_CFG_UNION;
#endif
#define PMIC_MMW_LDO_SST_END_CFG_ldo5_sst_end_sel_START (0)
#define PMIC_MMW_LDO_SST_END_CFG_ldo5_sst_end_sel_END (0)
#define PMIC_MMW_LDO_SST_END_CFG_ldo6_sst_end_sel_START (1)
#define PMIC_MMW_LDO_SST_END_CFG_ldo6_sst_end_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rfbuf_pwron_time_sel : 2;
        unsigned char xo_rf_pwron_time_sel : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_UNION;
#endif
#define PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_rfbuf_pwron_time_sel_START (0)
#define PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_rfbuf_pwron_time_sel_END (1)
#define PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_xo_rf_pwron_time_sel_START (2)
#define PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_xo_rf_pwron_time_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rfbuf_onoff_debug_mode : 8;
    } reg;
} PMIC_MMW_XO_RF_CLK_ON_DEBUG_UNION;
#endif
#define PMIC_MMW_XO_RF_CLK_ON_DEBUG_rfbuf_onoff_debug_mode_START (0)
#define PMIC_MMW_XO_RF_CLK_ON_DEBUG_rfbuf_onoff_debug_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_core_curr_debug_mode : 8;
    } reg;
} PMIC_MMW_XO_CORE_CURR_DEBUG_UNION;
#endif
#define PMIC_MMW_XO_CORE_CURR_DEBUG_xo_core_curr_debug_mode_START (0)
#define PMIC_MMW_XO_CORE_CURR_DEBUG_xo_core_curr_debug_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_inbuff_curr_debug_mode : 8;
    } reg;
} PMIC_MMW_XO_INBUFF_CURR_DEBUG_UNION;
#endif
#define PMIC_MMW_XO_INBUFF_CURR_DEBUG_xo_inbuff_curr_debug_mode_START (0)
#define PMIC_MMW_XO_INBUFF_CURR_DEBUG_xo_inbuff_curr_debug_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug0 : 8;
    } reg;
} PMIC_MMW_SPMI_DEBUG0_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG0_spmi_debug0_START (0)
#define PMIC_MMW_SPMI_DEBUG0_spmi_debug0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug1 : 8;
    } reg;
} PMIC_MMW_SPMI_DEBUG1_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG1_spmi_debug1_START (0)
#define PMIC_MMW_SPMI_DEBUG1_spmi_debug1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug2 : 8;
    } reg;
} PMIC_MMW_SPMI_DEBUG2_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG2_spmi_debug2_START (0)
#define PMIC_MMW_SPMI_DEBUG2_spmi_debug2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug3 : 8;
    } reg;
} PMIC_MMW_SPMI_DEBUG3_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG3_spmi_debug3_START (0)
#define PMIC_MMW_SPMI_DEBUG3_spmi_debug3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug4 : 8;
    } reg;
} PMIC_MMW_SPMI_DEBUG4_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG4_spmi_debug4_START (0)
#define PMIC_MMW_SPMI_DEBUG4_spmi_debug4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug5 : 8;
    } reg;
} PMIC_MMW_SPMI_DEBUG5_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG5_spmi_debug5_START (0)
#define PMIC_MMW_SPMI_DEBUG5_spmi_debug5_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug6 : 8;
    } reg;
} PMIC_MMW_SPMI_DEBUG6_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG6_spmi_debug6_START (0)
#define PMIC_MMW_SPMI_DEBUG6_spmi_debug6_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug7 : 8;
    } reg;
} PMIC_MMW_SPMI_DEBUG7_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG7_spmi_debug7_START (0)
#define PMIC_MMW_SPMI_DEBUG7_spmi_debug7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_0 : 8;
    } reg;
} PMIC_MMW_IRQ_MASK_0_UNION;
#endif
#define PMIC_MMW_IRQ_MASK_0_irq_mask_0_START (0)
#define PMIC_MMW_IRQ_MASK_0_irq_mask_0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_1 : 8;
    } reg;
} PMIC_MMW_IRQ_MASK_1_UNION;
#endif
#define PMIC_MMW_IRQ_MASK_1_irq_mask_1_START (0)
#define PMIC_MMW_IRQ_MASK_1_irq_mask_1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_2 : 8;
    } reg;
} PMIC_MMW_IRQ_MASK_2_UNION;
#endif
#define PMIC_MMW_IRQ_MASK_2_irq_mask_2_START (0)
#define PMIC_MMW_IRQ_MASK_2_irq_mask_2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char fault_mask_0 : 8;
    } reg;
} PMIC_MMW_FAULT_N_MASK_0_UNION;
#endif
#define PMIC_MMW_FAULT_N_MASK_0_fault_mask_0_START (0)
#define PMIC_MMW_FAULT_N_MASK_0_fault_mask_0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char fault_mask_1 : 8;
    } reg;
} PMIC_MMW_FAULT_N_MASK_1_UNION;
#endif
#define PMIC_MMW_FAULT_N_MASK_1_fault_mask_1_START (0)
#define PMIC_MMW_FAULT_N_MASK_1_fault_mask_1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char fault_mask_2 : 8;
    } reg;
} PMIC_MMW_FAULT_N_MASK_2_UNION;
#endif
#define PMIC_MMW_FAULT_N_MASK_2_fault_mask_2_START (0)
#define PMIC_MMW_FAULT_N_MASK_2_fault_mask_2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char fault_mask_3 : 8;
    } reg;
} PMIC_MMW_FAULT_N_MASK_3_UNION;
#endif
#define PMIC_MMW_FAULT_N_MASK_3_fault_mask_3_START (0)
#define PMIC_MMW_FAULT_N_MASK_3_fault_mask_3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ocp_r : 1;
        unsigned char irq_thsd_otmp125_d120ur : 1;
        unsigned char vin_ldo_0p9_vld_d1mf : 1;
        unsigned char vin_ldo_1p1_vld_d1mf : 1;
        unsigned char vin_ldo_1p3_vld_d1mf : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_MMW_IRQ0_UNION;
#endif
#define PMIC_MMW_IRQ0_ocp_r_START (0)
#define PMIC_MMW_IRQ0_ocp_r_END (0)
#define PMIC_MMW_IRQ0_irq_thsd_otmp125_d120ur_START (1)
#define PMIC_MMW_IRQ0_irq_thsd_otmp125_d120ur_END (1)
#define PMIC_MMW_IRQ0_vin_ldo_0p9_vld_d1mf_START (2)
#define PMIC_MMW_IRQ0_vin_ldo_0p9_vld_d1mf_END (2)
#define PMIC_MMW_IRQ0_vin_ldo_1p1_vld_d1mf_START (3)
#define PMIC_MMW_IRQ0_vin_ldo_1p1_vld_d1mf_END (3)
#define PMIC_MMW_IRQ0_vin_ldo_1p3_vld_d1mf_START (4)
#define PMIC_MMW_IRQ0_vin_ldo_1p3_vld_d1mf_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ocpldo0 : 1;
        unsigned char ocpldo1 : 1;
        unsigned char ocpldo2 : 1;
        unsigned char ocpldo3 : 1;
        unsigned char ocpldo4 : 1;
        unsigned char ocpldo5 : 1;
        unsigned char ocpldo6 : 1;
        unsigned char ocpldo7 : 1;
    } reg;
} PMIC_MMW_OCP_IRQ0_UNION;
#endif
#define PMIC_MMW_OCP_IRQ0_ocpldo0_START (0)
#define PMIC_MMW_OCP_IRQ0_ocpldo0_END (0)
#define PMIC_MMW_OCP_IRQ0_ocpldo1_START (1)
#define PMIC_MMW_OCP_IRQ0_ocpldo1_END (1)
#define PMIC_MMW_OCP_IRQ0_ocpldo2_START (2)
#define PMIC_MMW_OCP_IRQ0_ocpldo2_END (2)
#define PMIC_MMW_OCP_IRQ0_ocpldo3_START (3)
#define PMIC_MMW_OCP_IRQ0_ocpldo3_END (3)
#define PMIC_MMW_OCP_IRQ0_ocpldo4_START (4)
#define PMIC_MMW_OCP_IRQ0_ocpldo4_END (4)
#define PMIC_MMW_OCP_IRQ0_ocpldo5_START (5)
#define PMIC_MMW_OCP_IRQ0_ocpldo5_END (5)
#define PMIC_MMW_OCP_IRQ0_ocpldo6_START (6)
#define PMIC_MMW_OCP_IRQ0_ocpldo6_END (6)
#define PMIC_MMW_OCP_IRQ0_ocpldo7_START (7)
#define PMIC_MMW_OCP_IRQ0_ocpldo7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ocpldo8 : 1;
        unsigned char ocpldo_core : 1;
        unsigned char pmuh_ocp : 1;
        unsigned char ocpldo_buf0 : 1;
        unsigned char ocpldo_buf_rf : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_MMW_OCP_IRQ1_UNION;
#endif
#define PMIC_MMW_OCP_IRQ1_ocpldo8_START (0)
#define PMIC_MMW_OCP_IRQ1_ocpldo8_END (0)
#define PMIC_MMW_OCP_IRQ1_ocpldo_core_START (1)
#define PMIC_MMW_OCP_IRQ1_ocpldo_core_END (1)
#define PMIC_MMW_OCP_IRQ1_pmuh_ocp_START (2)
#define PMIC_MMW_OCP_IRQ1_pmuh_ocp_END (2)
#define PMIC_MMW_OCP_IRQ1_ocpldo_buf0_START (3)
#define PMIC_MMW_OCP_IRQ1_ocpldo_buf0_END (3)
#define PMIC_MMW_OCP_IRQ1_ocpldo_buf_rf_START (4)
#define PMIC_MMW_OCP_IRQ1_ocpldo_buf_rf_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char vsys_pwroff_abs_d20nr : 1;
        unsigned char vsys_ov_d200ur : 1;
        unsigned char vsys_pwron_d60ur : 1;
        unsigned char fault_thsd_otmp125_d120ur : 1;
        unsigned char thsd_otmp140_d540ur : 1;
        unsigned char vin_ldom_vld_deb : 1;
        unsigned char vin_io_vld_d5m : 1;
        unsigned char ldo_core_vld_d20nf : 1;
    } reg;
} PMIC_MMW_FAULT_N_0_UNION;
#endif
#define PMIC_MMW_FAULT_N_0_vsys_pwroff_abs_d20nr_START (0)
#define PMIC_MMW_FAULT_N_0_vsys_pwroff_abs_d20nr_END (0)
#define PMIC_MMW_FAULT_N_0_vsys_ov_d200ur_START (1)
#define PMIC_MMW_FAULT_N_0_vsys_ov_d200ur_END (1)
#define PMIC_MMW_FAULT_N_0_vsys_pwron_d60ur_START (2)
#define PMIC_MMW_FAULT_N_0_vsys_pwron_d60ur_END (2)
#define PMIC_MMW_FAULT_N_0_fault_thsd_otmp125_d120ur_START (3)
#define PMIC_MMW_FAULT_N_0_fault_thsd_otmp125_d120ur_END (3)
#define PMIC_MMW_FAULT_N_0_thsd_otmp140_d540ur_START (4)
#define PMIC_MMW_FAULT_N_0_thsd_otmp140_d540ur_END (4)
#define PMIC_MMW_FAULT_N_0_vin_ldom_vld_deb_START (5)
#define PMIC_MMW_FAULT_N_0_vin_ldom_vld_deb_END (5)
#define PMIC_MMW_FAULT_N_0_vin_io_vld_d5m_START (6)
#define PMIC_MMW_FAULT_N_0_vin_io_vld_d5m_END (6)
#define PMIC_MMW_FAULT_N_0_ldo_core_vld_d20nf_START (7)
#define PMIC_MMW_FAULT_N_0_ldo_core_vld_d20nf_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char fault_ocpldo0 : 1;
        unsigned char fault_ocpldo1 : 1;
        unsigned char fault_ocpldo2 : 1;
        unsigned char fault_ocpldo3 : 1;
        unsigned char fault_ocpldo4 : 1;
        unsigned char fault_ocpldo5 : 1;
        unsigned char fault_ocpldo6 : 1;
        unsigned char fault_ocpldo7 : 1;
    } reg;
} PMIC_MMW_FAULT_N_1_UNION;
#endif
#define PMIC_MMW_FAULT_N_1_fault_ocpldo0_START (0)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo0_END (0)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo1_START (1)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo1_END (1)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo2_START (2)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo2_END (2)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo3_START (3)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo3_END (3)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo4_START (4)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo4_END (4)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo5_START (5)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo5_END (5)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo6_START (6)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo6_END (6)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo7_START (7)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char fault_ocpldo8 : 1;
        unsigned char fault_ocpldo_core : 1;
        unsigned char fault_pmuh_ocp : 1;
        unsigned char fault_ocpldo_buf0 : 1;
        unsigned char fault_ocpldo_buf_rf : 1;
        unsigned char pmuh_short_n_d20nf : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_MMW_FAULT_N_2_UNION;
#endif
#define PMIC_MMW_FAULT_N_2_fault_ocpldo8_START (0)
#define PMIC_MMW_FAULT_N_2_fault_ocpldo8_END (0)
#define PMIC_MMW_FAULT_N_2_fault_ocpldo_core_START (1)
#define PMIC_MMW_FAULT_N_2_fault_ocpldo_core_END (1)
#define PMIC_MMW_FAULT_N_2_fault_pmuh_ocp_START (2)
#define PMIC_MMW_FAULT_N_2_fault_pmuh_ocp_END (2)
#define PMIC_MMW_FAULT_N_2_fault_ocpldo_buf0_START (3)
#define PMIC_MMW_FAULT_N_2_fault_ocpldo_buf0_END (3)
#define PMIC_MMW_FAULT_N_2_fault_ocpldo_buf_rf_START (4)
#define PMIC_MMW_FAULT_N_2_fault_ocpldo_buf_rf_END (4)
#define PMIC_MMW_FAULT_N_2_pmuh_short_n_d20nf_START (5)
#define PMIC_MMW_FAULT_N_2_pmuh_short_n_d20nf_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char fault_xo_19m2_dis : 1;
        unsigned char fault_19m2_dis_time_out : 1;
        unsigned char fault_buf_en_d5mf : 1;
        unsigned char fault_vin_ldom_time_out : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_FAULT_N_3_UNION;
#endif
#define PMIC_MMW_FAULT_N_3_fault_xo_19m2_dis_START (0)
#define PMIC_MMW_FAULT_N_3_fault_xo_19m2_dis_END (0)
#define PMIC_MMW_FAULT_N_3_fault_19m2_dis_time_out_START (1)
#define PMIC_MMW_FAULT_N_3_fault_19m2_dis_time_out_END (1)
#define PMIC_MMW_FAULT_N_3_fault_buf_en_d5mf_START (2)
#define PMIC_MMW_FAULT_N_3_fault_buf_en_d5mf_END (2)
#define PMIC_MMW_FAULT_N_3_fault_vin_ldom_time_out_START (3)
#define PMIC_MMW_FAULT_N_3_fault_vin_ldom_time_out_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char npirq_thsd_otmp125_d120ur : 1;
        unsigned char np_vin_ldo_0p9_vld_d1mf : 1;
        unsigned char np_vin_ldo_1p1_vld_d1mf : 1;
        unsigned char np_vin_ldo_1p3_vld_d1mf : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_NP_RECORD0_UNION;
#endif
#define PMIC_MMW_NP_RECORD0_npirq_thsd_otmp125_d120ur_START (0)
#define PMIC_MMW_NP_RECORD0_npirq_thsd_otmp125_d120ur_END (0)
#define PMIC_MMW_NP_RECORD0_np_vin_ldo_0p9_vld_d1mf_START (1)
#define PMIC_MMW_NP_RECORD0_np_vin_ldo_0p9_vld_d1mf_END (1)
#define PMIC_MMW_NP_RECORD0_np_vin_ldo_1p1_vld_d1mf_START (2)
#define PMIC_MMW_NP_RECORD0_np_vin_ldo_1p1_vld_d1mf_END (2)
#define PMIC_MMW_NP_RECORD0_np_vin_ldo_1p3_vld_d1mf_START (3)
#define PMIC_MMW_NP_RECORD0_np_vin_ldo_1p3_vld_d1mf_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_vsys_pwroff_abs_d20nr : 1;
        unsigned char np_vsys_ov_d200ur : 1;
        unsigned char np_vsys_pwron_d60ur : 1;
        unsigned char np_thsd_otmp125_d120ur : 1;
        unsigned char np_thsd_otmp140_d540ur : 1;
        unsigned char np_vin_ldom_vld_deb : 1;
        unsigned char np_vin_io_vld_d5m : 1;
        unsigned char np_ldo_core_vld_d20nf : 1;
    } reg;
} PMIC_MMW_NP_RECORD1_UNION;
#endif
#define PMIC_MMW_NP_RECORD1_np_vsys_pwroff_abs_d20nr_START (0)
#define PMIC_MMW_NP_RECORD1_np_vsys_pwroff_abs_d20nr_END (0)
#define PMIC_MMW_NP_RECORD1_np_vsys_ov_d200ur_START (1)
#define PMIC_MMW_NP_RECORD1_np_vsys_ov_d200ur_END (1)
#define PMIC_MMW_NP_RECORD1_np_vsys_pwron_d60ur_START (2)
#define PMIC_MMW_NP_RECORD1_np_vsys_pwron_d60ur_END (2)
#define PMIC_MMW_NP_RECORD1_np_thsd_otmp125_d120ur_START (3)
#define PMIC_MMW_NP_RECORD1_np_thsd_otmp125_d120ur_END (3)
#define PMIC_MMW_NP_RECORD1_np_thsd_otmp140_d540ur_START (4)
#define PMIC_MMW_NP_RECORD1_np_thsd_otmp140_d540ur_END (4)
#define PMIC_MMW_NP_RECORD1_np_vin_ldom_vld_deb_START (5)
#define PMIC_MMW_NP_RECORD1_np_vin_ldom_vld_deb_END (5)
#define PMIC_MMW_NP_RECORD1_np_vin_io_vld_d5m_START (6)
#define PMIC_MMW_NP_RECORD1_np_vin_io_vld_d5m_END (6)
#define PMIC_MMW_NP_RECORD1_np_ldo_core_vld_d20nf_START (7)
#define PMIC_MMW_NP_RECORD1_np_ldo_core_vld_d20nf_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ocpldo0 : 1;
        unsigned char np_ocpldo1 : 1;
        unsigned char np_ocpldo2 : 1;
        unsigned char np_ocpldo3 : 1;
        unsigned char np_ocpldo4 : 1;
        unsigned char np_ocpldo5 : 1;
        unsigned char np_ocpldo6 : 1;
        unsigned char np_ocpldo7 : 1;
    } reg;
} PMIC_MMW_NP_RECORD2_UNION;
#endif
#define PMIC_MMW_NP_RECORD2_np_ocpldo0_START (0)
#define PMIC_MMW_NP_RECORD2_np_ocpldo0_END (0)
#define PMIC_MMW_NP_RECORD2_np_ocpldo1_START (1)
#define PMIC_MMW_NP_RECORD2_np_ocpldo1_END (1)
#define PMIC_MMW_NP_RECORD2_np_ocpldo2_START (2)
#define PMIC_MMW_NP_RECORD2_np_ocpldo2_END (2)
#define PMIC_MMW_NP_RECORD2_np_ocpldo3_START (3)
#define PMIC_MMW_NP_RECORD2_np_ocpldo3_END (3)
#define PMIC_MMW_NP_RECORD2_np_ocpldo4_START (4)
#define PMIC_MMW_NP_RECORD2_np_ocpldo4_END (4)
#define PMIC_MMW_NP_RECORD2_np_ocpldo5_START (5)
#define PMIC_MMW_NP_RECORD2_np_ocpldo5_END (5)
#define PMIC_MMW_NP_RECORD2_np_ocpldo6_START (6)
#define PMIC_MMW_NP_RECORD2_np_ocpldo6_END (6)
#define PMIC_MMW_NP_RECORD2_np_ocpldo7_START (7)
#define PMIC_MMW_NP_RECORD2_np_ocpldo7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ocpldo8 : 1;
        unsigned char np_ocpldo_core : 1;
        unsigned char np_pmuh_ocp : 1;
        unsigned char np_ocpldo_buf0 : 1;
        unsigned char np_ocpldo_buf_rf : 1;
        unsigned char np_pmuh_short_n_d20nf : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_MMW_NP_RECORD3_UNION;
#endif
#define PMIC_MMW_NP_RECORD3_np_ocpldo8_START (0)
#define PMIC_MMW_NP_RECORD3_np_ocpldo8_END (0)
#define PMIC_MMW_NP_RECORD3_np_ocpldo_core_START (1)
#define PMIC_MMW_NP_RECORD3_np_ocpldo_core_END (1)
#define PMIC_MMW_NP_RECORD3_np_pmuh_ocp_START (2)
#define PMIC_MMW_NP_RECORD3_np_pmuh_ocp_END (2)
#define PMIC_MMW_NP_RECORD3_np_ocpldo_buf0_START (3)
#define PMIC_MMW_NP_RECORD3_np_ocpldo_buf0_END (3)
#define PMIC_MMW_NP_RECORD3_np_ocpldo_buf_rf_START (4)
#define PMIC_MMW_NP_RECORD3_np_ocpldo_buf_rf_END (4)
#define PMIC_MMW_NP_RECORD3_np_pmuh_short_n_d20nf_START (5)
#define PMIC_MMW_NP_RECORD3_np_pmuh_short_n_d20nf_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_19m2_dis : 1;
        unsigned char np_19m2_dis_time_out : 1;
        unsigned char buf_en_d5mf_shutdown : 1;
        unsigned char np_buf_en_d5mf : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_NP_RECORD4_UNION;
#endif
#define PMIC_MMW_NP_RECORD4_np_xo_19m2_dis_START (0)
#define PMIC_MMW_NP_RECORD4_np_xo_19m2_dis_END (0)
#define PMIC_MMW_NP_RECORD4_np_19m2_dis_time_out_START (1)
#define PMIC_MMW_NP_RECORD4_np_19m2_dis_time_out_END (1)
#define PMIC_MMW_NP_RECORD4_buf_en_d5mf_shutdown_START (2)
#define PMIC_MMW_NP_RECORD4_buf_en_d5mf_shutdown_END (2)
#define PMIC_MMW_NP_RECORD4_np_buf_en_d5mf_START (3)
#define PMIC_MMW_NP_RECORD4_np_buf_en_d5mf_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_dcxo_clk_sel_r : 1;
        unsigned char np_dcxo_clk_sel_f : 1;
        unsigned char np_hi6563_en_shutdown : 1;
        unsigned char np_hrstin : 1;
        unsigned char np_vin_ldom_time_out : 1;
        unsigned char np_rstin_time_out : 1;
        unsigned char np_fault_n : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_MMW_NP_RECORD5_UNION;
#endif
#define PMIC_MMW_NP_RECORD5_np_dcxo_clk_sel_r_START (0)
#define PMIC_MMW_NP_RECORD5_np_dcxo_clk_sel_r_END (0)
#define PMIC_MMW_NP_RECORD5_np_dcxo_clk_sel_f_START (1)
#define PMIC_MMW_NP_RECORD5_np_dcxo_clk_sel_f_END (1)
#define PMIC_MMW_NP_RECORD5_np_hi6563_en_shutdown_START (2)
#define PMIC_MMW_NP_RECORD5_np_hi6563_en_shutdown_END (2)
#define PMIC_MMW_NP_RECORD5_np_hrstin_START (3)
#define PMIC_MMW_NP_RECORD5_np_hrstin_END (3)
#define PMIC_MMW_NP_RECORD5_np_vin_ldom_time_out_START (4)
#define PMIC_MMW_NP_RECORD5_np_vin_ldom_time_out_END (4)
#define PMIC_MMW_NP_RECORD5_np_rstin_time_out_START (5)
#define PMIC_MMW_NP_RECORD5_np_rstin_time_out_END (5)
#define PMIC_MMW_NP_RECORD5_np_fault_n_START (6)
#define PMIC_MMW_NP_RECORD5_np_fault_n_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_rf0_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF0_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF0_WAVE_CTRL_np_xo_dig_rf0_sel_START (0)
#define PMIC_MMW_CLK_RF0_WAVE_CTRL_np_xo_dig_rf0_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_rf1_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF1_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF1_WAVE_CTRL_np_xo_dig_rf1_sel_START (0)
#define PMIC_MMW_CLK_RF1_WAVE_CTRL_np_xo_dig_rf1_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_rf2_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF2_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF2_WAVE_CTRL_np_xo_dig_rf2_sel_START (0)
#define PMIC_MMW_CLK_RF2_WAVE_CTRL_np_xo_dig_rf2_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_rf3_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF3_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF3_WAVE_CTRL_np_xo_dig_rf3_sel_START (0)
#define PMIC_MMW_CLK_RF3_WAVE_CTRL_np_xo_dig_rf3_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_rf4_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF4_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF4_WAVE_CTRL_np_xo_dig_rf4_sel_START (0)
#define PMIC_MMW_CLK_RF4_WAVE_CTRL_np_xo_dig_rf4_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_serdes_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_SERDES_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_SERDES_WAVE_CTRL_np_xo_dig_serdes_sel_START (0)
#define PMIC_MMW_CLK_SERDES_WAVE_CTRL_np_xo_dig_serdes_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_wifi0_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_WIFI0_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_WIFI0_WAVE_CTRL_np_xo_dig_wifi0_sel_START (0)
#define PMIC_MMW_CLK_WIFI0_WAVE_CTRL_np_xo_dig_wifi0_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_wifi1_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_WIFI1_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_WIFI1_WAVE_CTRL_np_xo_dig_wifi1_sel_START (0)
#define PMIC_MMW_CLK_WIFI1_WAVE_CTRL_np_xo_dig_wifi1_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_nfc_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_NFC_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_NFC_WAVE_CTRL_np_xo_dig_nfc_sel_START (0)
#define PMIC_MMW_CLK_NFC_WAVE_CTRL_np_xo_dig_nfc_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_sys_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_SYS_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_SYS_WAVE_CTRL_np_xo_dig_sys_sel_START (0)
#define PMIC_MMW_CLK_SYS_WAVE_CTRL_np_xo_dig_sys_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_pmu_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_PMU_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_PMU_WAVE_CTRL_np_xo_dig_pmu_sel_START (0)
#define PMIC_MMW_CLK_PMU_WAVE_CTRL_np_xo_dig_pmu_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_freq_rf0_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF0_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF0_FREQ_CTRL_np_xo_freq_rf0_sel_START (0)
#define PMIC_MMW_CLK_RF0_FREQ_CTRL_np_xo_freq_rf0_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_freq_rf1_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF1_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF1_FREQ_CTRL_np_xo_freq_rf1_sel_START (0)
#define PMIC_MMW_CLK_RF1_FREQ_CTRL_np_xo_freq_rf1_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_freq_rf2_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF2_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF2_FREQ_CTRL_np_xo_freq_rf2_sel_START (0)
#define PMIC_MMW_CLK_RF2_FREQ_CTRL_np_xo_freq_rf2_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_freq_rf3_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF3_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF3_FREQ_CTRL_np_xo_freq_rf3_sel_START (0)
#define PMIC_MMW_CLK_RF3_FREQ_CTRL_np_xo_freq_rf3_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_freq_rf4_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_RF4_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF4_FREQ_CTRL_np_xo_freq_rf4_sel_START (0)
#define PMIC_MMW_CLK_RF4_FREQ_CTRL_np_xo_freq_rf4_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_freq_serdes_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_SERDES_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_SERDES_FREQ_CTRL_np_xo_freq_serdes_sel_START (0)
#define PMIC_MMW_CLK_SERDES_FREQ_CTRL_np_xo_freq_serdes_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_freq_wifi0_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_WIFI0_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_WIFI0_FREQ_CTRL_np_xo_freq_wifi0_sel_START (0)
#define PMIC_MMW_CLK_WIFI0_FREQ_CTRL_np_xo_freq_wifi0_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_freq_wifi1_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_WIFI1_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_WIFI1_FREQ_CTRL_np_xo_freq_wifi1_sel_START (0)
#define PMIC_MMW_CLK_WIFI1_FREQ_CTRL_np_xo_freq_wifi1_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_freq_nfc_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_NFC_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_NFC_FREQ_CTRL_np_xo_freq_nfc_sel_START (0)
#define PMIC_MMW_CLK_NFC_FREQ_CTRL_np_xo_freq_nfc_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_freq_sys_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_SYS_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_SYS_FREQ_CTRL_np_xo_freq_sys_sel_START (0)
#define PMIC_MMW_CLK_SYS_FREQ_CTRL_np_xo_freq_sys_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_freq_pmu_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_PMU_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_PMU_FREQ_CTRL_np_xo_freq_pmu_sel_START (0)
#define PMIC_MMW_CLK_PMU_FREQ_CTRL_np_xo_freq_pmu_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_freq_ufs_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_UFS_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_UFS_FREQ_CTRL_np_xo_freq_ufs_sel_START (0)
#define PMIC_MMW_CLK_UFS_FREQ_CTRL_np_xo_freq_ufs_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sys_buf_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_CLK_SYS_BUF_PWR_SEL_UNION;
#endif
#define PMIC_MMW_CLK_SYS_BUF_PWR_SEL_np_sys_buf_sel_START (0)
#define PMIC_MMW_CLK_SYS_BUF_PWR_SEL_np_sys_buf_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_pmud_buck_en_cfg : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_EN_PMUD_UNION;
#endif
#define PMIC_MMW_EN_PMUD_np_pmud_buck_en_cfg_START (0)
#define PMIC_MMW_EN_PMUD_np_pmud_buck_en_cfg_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_pmud_vset : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_MMW_PMUD_VSET_UNION;
#endif
#define PMIC_MMW_PMUD_VSET_np_pmud_vset_START (0)
#define PMIC_MMW_PMUD_VSET_np_pmud_vset_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_trim_c2fix : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_CLK_TOP_CTRL1_0_UNION;
#endif
#define PMIC_MMW_CLK_TOP_CTRL1_0_np_xo_trim_c2fix_START (0)
#define PMIC_MMW_CLK_TOP_CTRL1_0_np_xo_trim_c2fix_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_trim_c1fix : 8;
    } reg;
} PMIC_MMW_CLK_TOP_CTRL1_1_UNION;
#endif
#define PMIC_MMW_CLK_TOP_CTRL1_1_np_xo_trim_c1fix_START (0)
#define PMIC_MMW_CLK_TOP_CTRL1_1_np_xo_trim_c1fix_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_reg_256k_en0 : 8;
    } reg;
} PMIC_MMW_CLK_256K_CTRL0_UNION;
#endif
#define PMIC_MMW_CLK_256K_CTRL0_np_reg_256k_en0_START (0)
#define PMIC_MMW_CLK_256K_CTRL0_np_reg_256k_en0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_reg_256k_en1 : 8;
    } reg;
} PMIC_MMW_CLK_256K_CTRL1_UNION;
#endif
#define PMIC_MMW_CLK_256K_CTRL1_np_reg_256k_en1_START (0)
#define PMIC_MMW_CLK_256K_CTRL1_np_reg_256k_en1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_corner_data : 8;
    } reg;
} PMIC_MMW_XO_DET_DATA_UNION;
#endif
#define PMIC_MMW_XO_DET_DATA_np_xo_corner_data_START (0)
#define PMIC_MMW_XO_DET_DATA_np_xo_corner_data_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_vsys_pwroff_abs_set : 2;
        unsigned char np_vsys_pwron_set : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_MMW_VSYS_LOW_SET_UNION;
#endif
#define PMIC_MMW_VSYS_LOW_SET_np_vsys_pwroff_abs_set_START (0)
#define PMIC_MMW_VSYS_LOW_SET_np_vsys_pwroff_abs_set_END (1)
#define PMIC_MMW_VSYS_LOW_SET_np_vsys_pwron_set_START (2)
#define PMIC_MMW_VSYS_LOW_SET_np_vsys_pwron_set_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_d2a_reserve0 : 8;
    } reg;
} PMIC_MMW_NP_D2A_RES0_UNION;
#endif
#define PMIC_MMW_NP_D2A_RES0_np_d2a_reserve0_START (0)
#define PMIC_MMW_NP_D2A_RES0_np_d2a_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_d2a_reserve1 : 8;
    } reg;
} PMIC_MMW_NP_D2A_RES1_UNION;
#endif
#define PMIC_MMW_NP_D2A_RES1_np_d2a_reserve1_START (0)
#define PMIC_MMW_NP_D2A_RES1_np_d2a_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_d2a_reserve2 : 8;
    } reg;
} PMIC_MMW_NP_D2A_RES2_UNION;
#endif
#define PMIC_MMW_NP_D2A_RES2_np_d2a_reserve2_START (0)
#define PMIC_MMW_NP_D2A_RES2_np_d2a_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hi6563_en_deb_sel : 2;
        unsigned char np_buf_en_deb_sel : 2;
        unsigned char np_vin_ldom_posdeb_sel : 2;
        unsigned char np_vin_ldom_negdeb_sel : 2;
    } reg;
} PMIC_MMW_NP_DEB_CTRL0_UNION;
#endif
#define PMIC_MMW_NP_DEB_CTRL0_np_hi6563_en_deb_sel_START (0)
#define PMIC_MMW_NP_DEB_CTRL0_np_hi6563_en_deb_sel_END (1)
#define PMIC_MMW_NP_DEB_CTRL0_np_buf_en_deb_sel_START (2)
#define PMIC_MMW_NP_DEB_CTRL0_np_buf_en_deb_sel_END (3)
#define PMIC_MMW_NP_DEB_CTRL0_np_vin_ldom_posdeb_sel_START (4)
#define PMIC_MMW_NP_DEB_CTRL0_np_vin_ldom_posdeb_sel_END (5)
#define PMIC_MMW_NP_DEB_CTRL0_np_vin_ldom_negdeb_sel_START (6)
#define PMIC_MMW_NP_DEB_CTRL0_np_vin_ldom_negdeb_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_en_vin_ldom_deb : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_NP_DEB_ONOFF0_UNION;
#endif
#define PMIC_MMW_NP_DEB_ONOFF0_np_en_vin_ldom_deb_START (0)
#define PMIC_MMW_NP_DEB_ONOFF0_np_en_vin_ldom_deb_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob0_d_w : 8;
    } reg;
} PMIC_MMW_NP_PWRON_TIME_CFG_UNION;
#endif
#define PMIC_MMW_NP_PWRON_TIME_CFG_np_otp_pdob0_d_w_START (0)
#define PMIC_MMW_NP_PWRON_TIME_CFG_np_otp_pdob0_d_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_default_freq_serdes_sel : 1;
        unsigned char np_default_freq_wifi0_sel : 1;
        unsigned char np_default_freq_wifi1_sel : 1;
        unsigned char np_default_freq_nfc_sel : 1;
        unsigned char np_default_freq_sys_sel : 1;
        unsigned char np_default_freq_pmu_sel : 1;
        unsigned char np_default_freq_ufs_sel : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_MMW_DEFAULT_FREQ_CFG_UNION;
#endif
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_serdes_sel_START (0)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_serdes_sel_END (0)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_wifi0_sel_START (1)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_wifi0_sel_END (1)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_wifi1_sel_START (2)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_wifi1_sel_END (2)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_nfc_sel_START (3)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_nfc_sel_END (3)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_sys_sel_START (4)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_sys_sel_END (4)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_pmu_sel_START (5)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_pmu_sel_END (5)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_ufs_sel_START (6)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_ufs_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_default_dig_serdes_sel : 1;
        unsigned char np_default_dig_wifi0_sel : 1;
        unsigned char np_default_dig_wifi1_sel : 1;
        unsigned char np_default_dig_nfc_sel : 1;
        unsigned char np_default_dig_sys_sel : 1;
        unsigned char np_default_dig_pmu_sel : 1;
        unsigned char np_default_sys_buf_sel : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_MMW_DEFAULT_WAVE_CFG_UNION;
#endif
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_serdes_sel_START (0)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_serdes_sel_END (0)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_wifi0_sel_START (1)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_wifi0_sel_END (1)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_wifi1_sel_START (2)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_wifi1_sel_END (2)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_nfc_sel_START (3)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_nfc_sel_END (3)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_sys_sel_START (4)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_sys_sel_END (4)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_pmu_sel_START (5)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_pmu_sel_END (5)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_sys_buf_sel_START (6)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_sys_buf_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_default_onoff_serdes_sel : 1;
        unsigned char np_default_onoff_wifi0_sel : 1;
        unsigned char np_default_onoff_wifi1_sel : 1;
        unsigned char np_default_onoff_nfc_sel : 1;
        unsigned char np_default_onoff_sys_sel : 1;
        unsigned char np_default_onoff_ufs_sel : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_MMW_DEFAULT_ONOFF_CFG_UNION;
#endif
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_serdes_sel_START (0)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_serdes_sel_END (0)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_wifi0_sel_START (1)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_wifi0_sel_END (1)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_wifi1_sel_START (2)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_wifi1_sel_END (2)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_nfc_sel_START (3)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_nfc_sel_END (3)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_sys_sel_START (4)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_sys_sel_END (4)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_ufs_sel_START (5)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_ufs_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_irq_n_ds : 2;
        unsigned char np_clk32_sys_ds : 2;
        unsigned char np_spmi_data_ds : 2;
        unsigned char reserved : 2;
    } reg;
} PMIC_MMW_DIG_IO_DS_CFG0_UNION;
#endif
#define PMIC_MMW_DIG_IO_DS_CFG0_np_irq_n_ds_START (0)
#define PMIC_MMW_DIG_IO_DS_CFG0_np_irq_n_ds_END (1)
#define PMIC_MMW_DIG_IO_DS_CFG0_np_clk32_sys_ds_START (2)
#define PMIC_MMW_DIG_IO_DS_CFG0_np_clk32_sys_ds_END (3)
#define PMIC_MMW_DIG_IO_DS_CFG0_np_spmi_data_ds_START (4)
#define PMIC_MMW_DIG_IO_DS_CFG0_np_spmi_data_ds_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_debug_lock : 8;
    } reg;
} PMIC_MMW_DEBUG_LOCK_UNION;
#endif
#define PMIC_MMW_DEBUG_LOCK_np_debug_lock_START (0)
#define PMIC_MMW_DEBUG_LOCK_np_debug_lock_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sys_debug0 : 8;
    } reg;
} PMIC_MMW_SYS_DEBUG0_UNION;
#endif
#define PMIC_MMW_SYS_DEBUG0_np_sys_debug0_START (0)
#define PMIC_MMW_SYS_DEBUG0_np_sys_debug0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sys_debug1 : 8;
    } reg;
} PMIC_MMW_SYS_DEBUG1_UNION;
#endif
#define PMIC_MMW_SYS_DEBUG1_np_sys_debug1_START (0)
#define PMIC_MMW_SYS_DEBUG1_np_sys_debug1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_reg_rc_debug : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_MMW_SYS_DEBUG2_UNION;
#endif
#define PMIC_MMW_SYS_DEBUG2_np_reg_rc_debug_START (0)
#define PMIC_MMW_SYS_DEBUG2_np_reg_rc_debug_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_vsys_pwroff_abs_pd_mask : 1;
        unsigned char np_vsys_ov_pd_mask : 1;
        unsigned char np_thsd_otmp140_pd_mask : 1;
        unsigned char np_vin_ldom_vld_pd_mask : 1;
        unsigned char np_vin_io_vld_pd_mask : 1;
        unsigned char np_xo_19m2_dis_pd_mask : 1;
        unsigned char np_clk_buf_en_pd_mask : 1;
        unsigned char np_19m2_dis_timeout_pd_mk : 1;
    } reg;
} PMIC_MMW_SYS_DEBUG3_UNION;
#endif
#define PMIC_MMW_SYS_DEBUG3_np_vsys_pwroff_abs_pd_mask_START (0)
#define PMIC_MMW_SYS_DEBUG3_np_vsys_pwroff_abs_pd_mask_END (0)
#define PMIC_MMW_SYS_DEBUG3_np_vsys_ov_pd_mask_START (1)
#define PMIC_MMW_SYS_DEBUG3_np_vsys_ov_pd_mask_END (1)
#define PMIC_MMW_SYS_DEBUG3_np_thsd_otmp140_pd_mask_START (2)
#define PMIC_MMW_SYS_DEBUG3_np_thsd_otmp140_pd_mask_END (2)
#define PMIC_MMW_SYS_DEBUG3_np_vin_ldom_vld_pd_mask_START (3)
#define PMIC_MMW_SYS_DEBUG3_np_vin_ldom_vld_pd_mask_END (3)
#define PMIC_MMW_SYS_DEBUG3_np_vin_io_vld_pd_mask_START (4)
#define PMIC_MMW_SYS_DEBUG3_np_vin_io_vld_pd_mask_END (4)
#define PMIC_MMW_SYS_DEBUG3_np_xo_19m2_dis_pd_mask_START (5)
#define PMIC_MMW_SYS_DEBUG3_np_xo_19m2_dis_pd_mask_END (5)
#define PMIC_MMW_SYS_DEBUG3_np_clk_buf_en_pd_mask_START (6)
#define PMIC_MMW_SYS_DEBUG3_np_clk_buf_en_pd_mask_END (6)
#define PMIC_MMW_SYS_DEBUG3_np_19m2_dis_timeout_pd_mk_START (7)
#define PMIC_MMW_SYS_DEBUG3_np_19m2_dis_timeout_pd_mk_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sys_debug2 : 8;
    } reg;
} PMIC_MMW_SYS_DEBUG4_UNION;
#endif
#define PMIC_MMW_SYS_DEBUG4_np_sys_debug2_START (0)
#define PMIC_MMW_SYS_DEBUG4_np_sys_debug2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sys_debug3 : 8;
    } reg;
} PMIC_MMW_SYS_DEBUG5_UNION;
#endif
#define PMIC_MMW_SYS_DEBUG5_np_sys_debug3_START (0)
#define PMIC_MMW_SYS_DEBUG5_np_sys_debug3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob0_d2a : 8;
    } reg;
} PMIC_MMW_OTP_0_R_UNION;
#endif
#define PMIC_MMW_OTP_0_R_np_otp_pdob0_d2a_START (0)
#define PMIC_MMW_OTP_0_R_np_otp_pdob0_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob1_d2a : 8;
    } reg;
} PMIC_MMW_OTP_1_R_UNION;
#endif
#define PMIC_MMW_OTP_1_R_np_otp_pdob1_d2a_START (0)
#define PMIC_MMW_OTP_1_R_np_otp_pdob1_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob2_d2a : 8;
    } reg;
} PMIC_MMW_OTP_2_R_UNION;
#endif
#define PMIC_MMW_OTP_2_R_np_otp_pdob2_d2a_START (0)
#define PMIC_MMW_OTP_2_R_np_otp_pdob2_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob3_d2a : 8;
    } reg;
} PMIC_MMW_OTP_3_R_UNION;
#endif
#define PMIC_MMW_OTP_3_R_np_otp_pdob3_d2a_START (0)
#define PMIC_MMW_OTP_3_R_np_otp_pdob3_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob4_d2a : 8;
    } reg;
} PMIC_MMW_OTP_4_R_UNION;
#endif
#define PMIC_MMW_OTP_4_R_np_otp_pdob4_d2a_START (0)
#define PMIC_MMW_OTP_4_R_np_otp_pdob4_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob5_d2a : 8;
    } reg;
} PMIC_MMW_OTP_5_R_UNION;
#endif
#define PMIC_MMW_OTP_5_R_np_otp_pdob5_d2a_START (0)
#define PMIC_MMW_OTP_5_R_np_otp_pdob5_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob6_d2a : 8;
    } reg;
} PMIC_MMW_OTP_6_R_UNION;
#endif
#define PMIC_MMW_OTP_6_R_np_otp_pdob6_d2a_START (0)
#define PMIC_MMW_OTP_6_R_np_otp_pdob6_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob7_d2a : 8;
    } reg;
} PMIC_MMW_OTP_7_R_UNION;
#endif
#define PMIC_MMW_OTP_7_R_np_otp_pdob7_d2a_START (0)
#define PMIC_MMW_OTP_7_R_np_otp_pdob7_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob8_d2a : 8;
    } reg;
} PMIC_MMW_OTP_8_R_UNION;
#endif
#define PMIC_MMW_OTP_8_R_np_otp_pdob8_d2a_START (0)
#define PMIC_MMW_OTP_8_R_np_otp_pdob8_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob9_d2a : 8;
    } reg;
} PMIC_MMW_OTP_9_R_UNION;
#endif
#define PMIC_MMW_OTP_9_R_np_otp_pdob9_d2a_START (0)
#define PMIC_MMW_OTP_9_R_np_otp_pdob9_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob10_d2a : 8;
    } reg;
} PMIC_MMW_OTP_10_R_UNION;
#endif
#define PMIC_MMW_OTP_10_R_np_otp_pdob10_d2a_START (0)
#define PMIC_MMW_OTP_10_R_np_otp_pdob10_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob11_d2a : 8;
    } reg;
} PMIC_MMW_OTP_11_R_UNION;
#endif
#define PMIC_MMW_OTP_11_R_np_otp_pdob11_d2a_START (0)
#define PMIC_MMW_OTP_11_R_np_otp_pdob11_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob12_d2a : 8;
    } reg;
} PMIC_MMW_OTP_12_R_UNION;
#endif
#define PMIC_MMW_OTP_12_R_np_otp_pdob12_d2a_START (0)
#define PMIC_MMW_OTP_12_R_np_otp_pdob12_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob13_d2a : 8;
    } reg;
} PMIC_MMW_OTP_13_R_UNION;
#endif
#define PMIC_MMW_OTP_13_R_np_otp_pdob13_d2a_START (0)
#define PMIC_MMW_OTP_13_R_np_otp_pdob13_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob14_d2a : 8;
    } reg;
} PMIC_MMW_OTP_14_R_UNION;
#endif
#define PMIC_MMW_OTP_14_R_np_otp_pdob14_d2a_START (0)
#define PMIC_MMW_OTP_14_R_np_otp_pdob14_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob15_d2a : 8;
    } reg;
} PMIC_MMW_OTP_15_R_UNION;
#endif
#define PMIC_MMW_OTP_15_R_np_otp_pdob15_d2a_START (0)
#define PMIC_MMW_OTP_15_R_np_otp_pdob15_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob16_d2a : 8;
    } reg;
} PMIC_MMW_OTP_16_R_UNION;
#endif
#define PMIC_MMW_OTP_16_R_np_otp_pdob16_d2a_START (0)
#define PMIC_MMW_OTP_16_R_np_otp_pdob16_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob17_d2a : 8;
    } reg;
} PMIC_MMW_OTP_17_R_UNION;
#endif
#define PMIC_MMW_OTP_17_R_np_otp_pdob17_d2a_START (0)
#define PMIC_MMW_OTP_17_R_np_otp_pdob17_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob18_d2a : 8;
    } reg;
} PMIC_MMW_OTP_18_R_UNION;
#endif
#define PMIC_MMW_OTP_18_R_np_otp_pdob18_d2a_START (0)
#define PMIC_MMW_OTP_18_R_np_otp_pdob18_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob19_d2a : 8;
    } reg;
} PMIC_MMW_OTP_19_R_UNION;
#endif
#define PMIC_MMW_OTP_19_R_np_otp_pdob19_d2a_START (0)
#define PMIC_MMW_OTP_19_R_np_otp_pdob19_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob20_d2a : 8;
    } reg;
} PMIC_MMW_OTP_20_R_UNION;
#endif
#define PMIC_MMW_OTP_20_R_np_otp_pdob20_d2a_START (0)
#define PMIC_MMW_OTP_20_R_np_otp_pdob20_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob21_d2a : 8;
    } reg;
} PMIC_MMW_OTP_21_R_UNION;
#endif
#define PMIC_MMW_OTP_21_R_np_otp_pdob21_d2a_START (0)
#define PMIC_MMW_OTP_21_R_np_otp_pdob21_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob22_d2a : 8;
    } reg;
} PMIC_MMW_OTP_22_R_UNION;
#endif
#define PMIC_MMW_OTP_22_R_np_otp_pdob22_d2a_START (0)
#define PMIC_MMW_OTP_22_R_np_otp_pdob22_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob23_d2a : 8;
    } reg;
} PMIC_MMW_OTP_23_R_UNION;
#endif
#define PMIC_MMW_OTP_23_R_np_otp_pdob23_d2a_START (0)
#define PMIC_MMW_OTP_23_R_np_otp_pdob23_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob24_d2a : 8;
    } reg;
} PMIC_MMW_OTP_24_R_UNION;
#endif
#define PMIC_MMW_OTP_24_R_np_otp_pdob24_d2a_START (0)
#define PMIC_MMW_OTP_24_R_np_otp_pdob24_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob25_d2a : 8;
    } reg;
} PMIC_MMW_OTP_25_R_UNION;
#endif
#define PMIC_MMW_OTP_25_R_np_otp_pdob25_d2a_START (0)
#define PMIC_MMW_OTP_25_R_np_otp_pdob25_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob26_d2a : 8;
    } reg;
} PMIC_MMW_OTP_26_R_UNION;
#endif
#define PMIC_MMW_OTP_26_R_np_otp_pdob26_d2a_START (0)
#define PMIC_MMW_OTP_26_R_np_otp_pdob26_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob27_d2a : 8;
    } reg;
} PMIC_MMW_OTP_27_R_UNION;
#endif
#define PMIC_MMW_OTP_27_R_np_otp_pdob27_d2a_START (0)
#define PMIC_MMW_OTP_27_R_np_otp_pdob27_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob28_d2a : 8;
    } reg;
} PMIC_MMW_OTP_28_R_UNION;
#endif
#define PMIC_MMW_OTP_28_R_np_otp_pdob28_d2a_START (0)
#define PMIC_MMW_OTP_28_R_np_otp_pdob28_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob29_d2a : 8;
    } reg;
} PMIC_MMW_OTP_29_R_UNION;
#endif
#define PMIC_MMW_OTP_29_R_np_otp_pdob29_d2a_START (0)
#define PMIC_MMW_OTP_29_R_np_otp_pdob29_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob30_d2a : 8;
    } reg;
} PMIC_MMW_OTP_30_R_UNION;
#endif
#define PMIC_MMW_OTP_30_R_np_otp_pdob30_d2a_START (0)
#define PMIC_MMW_OTP_30_R_np_otp_pdob30_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob31_d2a : 8;
    } reg;
} PMIC_MMW_OTP_31_R_UNION;
#endif
#define PMIC_MMW_OTP_31_R_np_otp_pdob31_d2a_START (0)
#define PMIC_MMW_OTP_31_R_np_otp_pdob31_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob32_d2a : 8;
    } reg;
} PMIC_MMW_OTP_32_R_UNION;
#endif
#define PMIC_MMW_OTP_32_R_np_otp_pdob32_d2a_START (0)
#define PMIC_MMW_OTP_32_R_np_otp_pdob32_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob33_d2a : 8;
    } reg;
} PMIC_MMW_OTP_33_R_UNION;
#endif
#define PMIC_MMW_OTP_33_R_np_otp_pdob33_d2a_START (0)
#define PMIC_MMW_OTP_33_R_np_otp_pdob33_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob34_d2a : 8;
    } reg;
} PMIC_MMW_OTP_34_R_UNION;
#endif
#define PMIC_MMW_OTP_34_R_np_otp_pdob34_d2a_START (0)
#define PMIC_MMW_OTP_34_R_np_otp_pdob34_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob35_d2a : 8;
    } reg;
} PMIC_MMW_OTP_35_R_UNION;
#endif
#define PMIC_MMW_OTP_35_R_np_otp_pdob35_d2a_START (0)
#define PMIC_MMW_OTP_35_R_np_otp_pdob35_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob36_d2a : 8;
    } reg;
} PMIC_MMW_OTP_36_R_UNION;
#endif
#define PMIC_MMW_OTP_36_R_np_otp_pdob36_d2a_START (0)
#define PMIC_MMW_OTP_36_R_np_otp_pdob36_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob37_d2a : 8;
    } reg;
} PMIC_MMW_OTP_37_R_UNION;
#endif
#define PMIC_MMW_OTP_37_R_np_otp_pdob37_d2a_START (0)
#define PMIC_MMW_OTP_37_R_np_otp_pdob37_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob38_d2a : 8;
    } reg;
} PMIC_MMW_OTP_38_R_UNION;
#endif
#define PMIC_MMW_OTP_38_R_np_otp_pdob38_d2a_START (0)
#define PMIC_MMW_OTP_38_R_np_otp_pdob38_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob39_d2a : 8;
    } reg;
} PMIC_MMW_OTP_39_R_UNION;
#endif
#define PMIC_MMW_OTP_39_R_np_otp_pdob39_d2a_START (0)
#define PMIC_MMW_OTP_39_R_np_otp_pdob39_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob40_d2a : 8;
    } reg;
} PMIC_MMW_OTP_40_R_UNION;
#endif
#define PMIC_MMW_OTP_40_R_np_otp_pdob40_d2a_START (0)
#define PMIC_MMW_OTP_40_R_np_otp_pdob40_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob41_d2a : 8;
    } reg;
} PMIC_MMW_OTP_41_R_UNION;
#endif
#define PMIC_MMW_OTP_41_R_np_otp_pdob41_d2a_START (0)
#define PMIC_MMW_OTP_41_R_np_otp_pdob41_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob42_d2a : 8;
    } reg;
} PMIC_MMW_OTP_42_R_UNION;
#endif
#define PMIC_MMW_OTP_42_R_np_otp_pdob42_d2a_START (0)
#define PMIC_MMW_OTP_42_R_np_otp_pdob42_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob43_d2a : 8;
    } reg;
} PMIC_MMW_OTP_43_R_UNION;
#endif
#define PMIC_MMW_OTP_43_R_np_otp_pdob43_d2a_START (0)
#define PMIC_MMW_OTP_43_R_np_otp_pdob43_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob44_d2a : 8;
    } reg;
} PMIC_MMW_OTP_44_R_UNION;
#endif
#define PMIC_MMW_OTP_44_R_np_otp_pdob44_d2a_START (0)
#define PMIC_MMW_OTP_44_R_np_otp_pdob44_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob45_d2a : 8;
    } reg;
} PMIC_MMW_OTP_45_R_UNION;
#endif
#define PMIC_MMW_OTP_45_R_np_otp_pdob45_d2a_START (0)
#define PMIC_MMW_OTP_45_R_np_otp_pdob45_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob46_d2a : 8;
    } reg;
} PMIC_MMW_OTP_46_R_UNION;
#endif
#define PMIC_MMW_OTP_46_R_np_otp_pdob46_d2a_START (0)
#define PMIC_MMW_OTP_46_R_np_otp_pdob46_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob47_d2a : 8;
    } reg;
} PMIC_MMW_OTP_47_R_UNION;
#endif
#define PMIC_MMW_OTP_47_R_np_otp_pdob47_d2a_START (0)
#define PMIC_MMW_OTP_47_R_np_otp_pdob47_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob48_d2a : 8;
    } reg;
} PMIC_MMW_OTP_48_R_UNION;
#endif
#define PMIC_MMW_OTP_48_R_np_otp_pdob48_d2a_START (0)
#define PMIC_MMW_OTP_48_R_np_otp_pdob48_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob49_d2a : 8;
    } reg;
} PMIC_MMW_OTP_49_R_UNION;
#endif
#define PMIC_MMW_OTP_49_R_np_otp_pdob49_d2a_START (0)
#define PMIC_MMW_OTP_49_R_np_otp_pdob49_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob50_d2a : 8;
    } reg;
} PMIC_MMW_OTP_50_R_UNION;
#endif
#define PMIC_MMW_OTP_50_R_np_otp_pdob50_d2a_START (0)
#define PMIC_MMW_OTP_50_R_np_otp_pdob50_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob51_d2a : 8;
    } reg;
} PMIC_MMW_OTP_51_R_UNION;
#endif
#define PMIC_MMW_OTP_51_R_np_otp_pdob51_d2a_START (0)
#define PMIC_MMW_OTP_51_R_np_otp_pdob51_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob52_d2a : 8;
    } reg;
} PMIC_MMW_OTP_52_R_UNION;
#endif
#define PMIC_MMW_OTP_52_R_np_otp_pdob52_d2a_START (0)
#define PMIC_MMW_OTP_52_R_np_otp_pdob52_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob53_d2a : 8;
    } reg;
} PMIC_MMW_OTP_53_R_UNION;
#endif
#define PMIC_MMW_OTP_53_R_np_otp_pdob53_d2a_START (0)
#define PMIC_MMW_OTP_53_R_np_otp_pdob53_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob54_d2a : 8;
    } reg;
} PMIC_MMW_OTP_54_R_UNION;
#endif
#define PMIC_MMW_OTP_54_R_np_otp_pdob54_d2a_START (0)
#define PMIC_MMW_OTP_54_R_np_otp_pdob54_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob55_d2a : 8;
    } reg;
} PMIC_MMW_OTP_55_R_UNION;
#endif
#define PMIC_MMW_OTP_55_R_np_otp_pdob55_d2a_START (0)
#define PMIC_MMW_OTP_55_R_np_otp_pdob55_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob56_d2a : 8;
    } reg;
} PMIC_MMW_OTP_56_R_UNION;
#endif
#define PMIC_MMW_OTP_56_R_np_otp_pdob56_d2a_START (0)
#define PMIC_MMW_OTP_56_R_np_otp_pdob56_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob57_d2a : 8;
    } reg;
} PMIC_MMW_OTP_57_R_UNION;
#endif
#define PMIC_MMW_OTP_57_R_np_otp_pdob57_d2a_START (0)
#define PMIC_MMW_OTP_57_R_np_otp_pdob57_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob58_d2a : 8;
    } reg;
} PMIC_MMW_OTP_58_R_UNION;
#endif
#define PMIC_MMW_OTP_58_R_np_otp_pdob58_d2a_START (0)
#define PMIC_MMW_OTP_58_R_np_otp_pdob58_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob59_d2a : 8;
    } reg;
} PMIC_MMW_OTP_59_R_UNION;
#endif
#define PMIC_MMW_OTP_59_R_np_otp_pdob59_d2a_START (0)
#define PMIC_MMW_OTP_59_R_np_otp_pdob59_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob60_d2a : 8;
    } reg;
} PMIC_MMW_OTP_60_R_UNION;
#endif
#define PMIC_MMW_OTP_60_R_np_otp_pdob60_d2a_START (0)
#define PMIC_MMW_OTP_60_R_np_otp_pdob60_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob61_d2a : 8;
    } reg;
} PMIC_MMW_OTP_61_R_UNION;
#endif
#define PMIC_MMW_OTP_61_R_np_otp_pdob61_d2a_START (0)
#define PMIC_MMW_OTP_61_R_np_otp_pdob61_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob62_d2a : 8;
    } reg;
} PMIC_MMW_OTP_62_R_UNION;
#endif
#define PMIC_MMW_OTP_62_R_np_otp_pdob62_d2a_START (0)
#define PMIC_MMW_OTP_62_R_np_otp_pdob62_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp_pdob63_d2a : 8;
    } reg;
} PMIC_MMW_OTP_63_R_UNION;
#endif
#define PMIC_MMW_OTP_63_R_np_otp_pdob63_d2a_START (0)
#define PMIC_MMW_OTP_63_R_np_otp_pdob63_d2a_END (7)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
