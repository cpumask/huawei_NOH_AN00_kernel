#ifndef __PMIC_INTERFACE_H__
#define __PMIC_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_VERSION0_ADDR(base) ((base) + (0x0000UL))
#define PMIC_VERSION1_ADDR(base) ((base) + (0x0001UL))
#define PMIC_VERSION2_ADDR(base) ((base) + (0x0002UL))
#define PMIC_VERSION3_ADDR(base) ((base) + (0x0003UL))
#define PMIC_VERSION4_ADDR(base) ((base) + (0x0004UL))
#define PMIC_VERSION5_ADDR(base) ((base) + (0x0005UL))
#define PMIC_STATUS0_ADDR(base) ((base) + (0x0006UL))
#define PMIC_STATUS1_ADDR(base) ((base) + (0x0007UL))
#define PMIC_STATUS2_ADDR(base) ((base) + (0x0008UL))
#define PMIC_STATUS3_ADDR(base) ((base) + (0x0009UL))
#define PMIC_STATUS4_ADDR(base) ((base) + (0x000AUL))
#define PMIC_STATUS5_ADDR(base) ((base) + (0x000BUL))
#define PMIC_STATUS6_ADDR(base) ((base) + (0x000CUL))
#define PMIC_BUCK1_ONOFF_ECO_ADDR(base) ((base) + (0x000DUL))
#define PMIC_BUCK2_ONOFF_ECO_ADDR(base) ((base) + (0x000EUL))
#define PMIC_BUCK3_ONOFF_ECO_ADDR(base) ((base) + (0x000FUL))
#define PMIC_BUCK40_ONOFF_ECO_ADDR(base) ((base) + (0x0010UL))
#define PMIC_BUCK41_ONOFF_ECO_ADDR(base) ((base) + (0x0011UL))
#define PMIC_BUCK42_ONOFF_ECO_ADDR(base) ((base) + (0x0012UL))
#define PMIC_BUCK5_ONOFF_ECO_ADDR(base) ((base) + (0x0013UL))
#define PMIC_BUCK70_ONOFF1_ECO_ADDR(base) ((base) + (0x0014UL))
#define PMIC_BUCK70_ONOFF2_ADDR(base) ((base) + (0x0015UL))
#define PMIC_BUCK70_ONOFF3_ADDR(base) ((base) + (0x0016UL))
#define PMIC_BUCK71_ONOFF_ECO_ADDR(base) ((base) + (0x0017UL))
#define PMIC_BUCK9_ONOFF_ECO_ADDR(base) ((base) + (0x0018UL))
#define PMIC_LDO0_ONOFF_ECO_ADDR(base) ((base) + (0x0019UL))
#define PMIC_LDO1_ONOFF_ECO_ADDR(base) ((base) + (0x001AUL))
#define PMIC_LDO3_ONOFF_ADDR(base) ((base) + (0x001BUL))
#define PMIC_LDO4_ONOFF_ECO_ADDR(base) ((base) + (0x001CUL))
#define PMIC_LDO6_ONOFF_ECO_ADDR(base) ((base) + (0x001DUL))
#define PMIC_LDO8_ONOFF_ECO_ADDR(base) ((base) + (0x001EUL))
#define PMIC_LDO11_ONOFF_ECO_ADDR(base) ((base) + (0x001FUL))
#define PMIC_LDO12_ONOFF_ECO_ADDR(base) ((base) + (0x0020UL))
#define PMIC_LDO14_ONOFF_ECO_ADDR(base) ((base) + (0x0021UL))
#define PMIC_LDO15_ONOFF_ECO_ADDR(base) ((base) + (0x0022UL))
#define PMIC_LDO16_ONOFF_ECO_ADDR(base) ((base) + (0x0023UL))
#define PMIC_LDO17_ONOFF_ECO_ADDR(base) ((base) + (0x0024UL))
#define PMIC_LDO18_ONOFF_ECO_ADDR(base) ((base) + (0x0025UL))
#define PMIC_LDO21_ONOFF_ECO_ADDR(base) ((base) + (0x0026UL))
#define PMIC_LDO22_ONOFF_ADDR(base) ((base) + (0x0027UL))
#define PMIC_LDO23_ONOFF_ECO_ADDR(base) ((base) + (0x0028UL))
#define PMIC_LDO24_ONOFF_ECO_ADDR(base) ((base) + (0x0029UL))
#define PMIC_LDO25_ONOFF_ADDR(base) ((base) + (0x002AUL))
#define PMIC_LDO26_ONOFF_ECO_ADDR(base) ((base) + (0x002BUL))
#define PMIC_LDO27_ONOFF_ECO_ADDR(base) ((base) + (0x002CUL))
#define PMIC_LDO28_ONOFF_ECO_ADDR(base) ((base) + (0x002DUL))
#define PMIC_LDO29_ONOFF_ECO_ADDR(base) ((base) + (0x002EUL))
#define PMIC_LDO30_ONOFF_ECO_ADDR(base) ((base) + (0x002FUL))
#define PMIC_LDO32_ONOFF_ECO_ADDR(base) ((base) + (0x0030UL))
#define PMIC_PMUH_ONOFF_ECO_ADDR(base) ((base) + (0x0031UL))
#define PMIC_LDO36_ONOFF_ECO_ADDR(base) ((base) + (0x0032UL))
#define PMIC_LDO37_ONOFF_ECO_ADDR(base) ((base) + (0x0033UL))
#define PMIC_LDO38_ONOFF_ECO_ADDR(base) ((base) + (0x0034UL))
#define PMIC_LDO40_ONOFF_ECO_ADDR(base) ((base) + (0x0035UL))
#define PMIC_LDO41_ONOFF_ECO_ADDR(base) ((base) + (0x0036UL))
#define PMIC_LDO42_ONOFF_ECO_ADDR(base) ((base) + (0x0037UL))
#define PMIC_LDO43_ONOFF_ECO_ADDR(base) ((base) + (0x0038UL))
#define PMIC_LDO44_ONOFF_ECO_ADDR(base) ((base) + (0x0039UL))
#define PMIC_LDO45_ONOFF_ECO_ADDR(base) ((base) + (0x003AUL))
#define PMIC_LDO46_ONOFF_ECO_ADDR(base) ((base) + (0x003BUL))
#define PMIC_SW1_ONOFF_ADDR(base) ((base) + (0x003CUL))
#define PMIC_LDO_SINK_ONOFF_ECO_ADDR(base) ((base) + (0x003DUL))
#define PMIC_REF_ECO_ADDR(base) ((base) + (0x003EUL))
#define PMIC_EXT_PMU_ONOFF_ADDR(base) ((base) + (0x003FUL))
#define PMIC_CLK_SERDES_EN_ADDR(base) ((base) + (0x0040UL))
#define PMIC_CLK_WIFI_EN_ADDR(base) ((base) + (0x0041UL))
#define PMIC_CLK_NFC_EN_ADDR(base) ((base) + (0x0042UL))
#define PMIC_CLK_RF0_EN_ADDR(base) ((base) + (0x0043UL))
#define PMIC_CLK_RF1_EN_ADDR(base) ((base) + (0x0044UL))
#define PMIC_CLK_SYS_EN_ADDR(base) ((base) + (0x0045UL))
#define PMIC_CLK_CODEC_EN_ADDR(base) ((base) + (0x0046UL))
#define PMIC_CLK_UFS_EN_ADDR(base) ((base) + (0x0047UL))
#define PMIC_CLK_EUSB_EN_ADDR(base) ((base) + (0x0048UL))
#define PMIC_OSC32K_GPS_ONOFF_CTRL_ADDR(base) ((base) + (0x0049UL))
#define PMIC_OSC32K_BT_ONOFF_CTRL_ADDR(base) ((base) + (0x004AUL))
#define PMIC_OSC32K_SYS_ONOFF_CTRL_ADDR(base) ((base) + (0x004BUL))
#define PMIC_CLK_32K_PMU_CTRL0_ADDR(base) ((base) + (0x004CUL))
#define PMIC_CLK_32K_PMU_CTRL1_ADDR(base) ((base) + (0x004DUL))
#define PMIC_LRA_CLK_ONOFF1_ADDR(base) ((base) + (0x004EUL))
#define PMIC_LRA_CLK_ONOFF2_ADDR(base) ((base) + (0x004FUL))
#define PMIC_BUCK1_VSET_ADDR(base) ((base) + (0x0050UL))
#define PMIC_BUCK2_VSET_ADDR(base) ((base) + (0x0051UL))
#define PMIC_BUCK2_VSET_ECO_ADDR(base) ((base) + (0x0052UL))
#define PMIC_BUCK3_VSET_ADDR(base) ((base) + (0x0053UL))
#define PMIC_BUCK3_VSET_ECO_ADDR(base) ((base) + (0x0054UL))
#define PMIC_BUCK40_VSET_ADDR(base) ((base) + (0x0055UL))
#define PMIC_BUCK42_VSET_ADDR(base) ((base) + (0x0056UL))
#define PMIC_BUCK5_VSET_ADDR(base) ((base) + (0x0057UL))
#define PMIC_BUCK5_VSET_ECO_ADDR(base) ((base) + (0x0058UL))
#define PMIC_BUCK70_VSET_ADDR(base) ((base) + (0x0059UL))
#define PMIC_BUCK71_VSET_ADDR(base) ((base) + (0x005AUL))
#define PMIC_BUCK9_VSET_ADDR(base) ((base) + (0x005BUL))
#define PMIC_BUCK9_VSET_ECO_ADDR(base) ((base) + (0x005CUL))
#define PMIC_LDO0_VSET_ADDR(base) ((base) + (0x005DUL))
#define PMIC_LDO0_VSET_ECO_ADDR(base) ((base) + (0x005EUL))
#define PMIC_LDO1_VSET_ADDR(base) ((base) + (0x005FUL))
#define PMIC_LDO3_VSET_ADDR(base) ((base) + (0x0060UL))
#define PMIC_LDO4_VSET_ADDR(base) ((base) + (0x0061UL))
#define PMIC_LDO6_VSET_ADDR(base) ((base) + (0x0062UL))
#define PMIC_LDO8_VSET_ADDR(base) ((base) + (0x0063UL))
#define PMIC_LDO11_VSET_ADDR(base) ((base) + (0x0064UL))
#define PMIC_LDO12_VSET_ADDR(base) ((base) + (0x0065UL))
#define PMIC_LDO14_VSET_ADDR(base) ((base) + (0x0066UL))
#define PMIC_LDO15_VSET_ADDR(base) ((base) + (0x0067UL))
#define PMIC_LDO16_VSET_ADDR(base) ((base) + (0x0068UL))
#define PMIC_LDO17_VSET_ADDR(base) ((base) + (0x0069UL))
#define PMIC_LDO18_VSET_ADDR(base) ((base) + (0x006AUL))
#define PMIC_LDO21_VSET_ADDR(base) ((base) + (0x006BUL))
#define PMIC_LDO22_VSET_ADDR(base) ((base) + (0x006CUL))
#define PMIC_LDO23_VSET_ADDR(base) ((base) + (0x006DUL))
#define PMIC_LDO24_VSET_ADDR(base) ((base) + (0x006EUL))
#define PMIC_LDO25_VSET_ADDR(base) ((base) + (0x006FUL))
#define PMIC_LDO26_VSET_ADDR(base) ((base) + (0x0070UL))
#define PMIC_LDO27_VSET_ADDR(base) ((base) + (0x0071UL))
#define PMIC_LDO28_VSET_ADDR(base) ((base) + (0x0072UL))
#define PMIC_LDO29_VSET_ADDR(base) ((base) + (0x0073UL))
#define PMIC_LDO30_VSET_ADDR(base) ((base) + (0x0074UL))
#define PMIC_LDO30_VSET_ECO_ADDR(base) ((base) + (0x0075UL))
#define PMIC_LDO32_VSET_ADDR(base) ((base) + (0x0076UL))
#define PMIC_PMUH_VSET_ADDR(base) ((base) + (0x0077UL))
#define PMIC_LDO36_VSET_ADDR(base) ((base) + (0x0078UL))
#define PMIC_LDO37_VSET_ADDR(base) ((base) + (0x0079UL))
#define PMIC_LDO38_VSET_ADDR(base) ((base) + (0x007AUL))
#define PMIC_LDO40_VSET_ADDR(base) ((base) + (0x007BUL))
#define PMIC_LDO40_VSET_ECO_ADDR(base) ((base) + (0x007CUL))
#define PMIC_LDO41_VSET_ADDR(base) ((base) + (0x007DUL))
#define PMIC_LDO42_VSET_ADDR(base) ((base) + (0x007EUL))
#define PMIC_LDO43_VSET_ADDR(base) ((base) + (0x007FUL))
#define PMIC_LDO44_VSET_ADDR(base) ((base) + (0x0080UL))
#define PMIC_LDO44_VSET_ECO_ADDR(base) ((base) + (0x0081UL))
#define PMIC_LDO45_VSET_ADDR(base) ((base) + (0x0082UL))
#define PMIC_LDO46_VSET_ADDR(base) ((base) + (0x0083UL))
#define PMIC_LDO_SINK_VSET_ADDR(base) ((base) + (0x0084UL))
#define PMIC_LDO_BUF_VSET_ADDR(base) ((base) + (0x0085UL))
#define PMIC_PMUD_VSET_ADDR(base) ((base) + (0x0086UL))
#define PMIC_BUCK1_40_OCP_CTRL_ADDR(base) ((base) + (0x008AUL))
#define PMIC_BUCK42_71_OCP_CTRL_ADDR(base) ((base) + (0x008BUL))
#define PMIC_BUCK9_OCP_CTRL_ADDR(base) ((base) + (0x008CUL))
#define PMIC_LDO0_3_OCP_CTRL_ADDR(base) ((base) + (0x008DUL))
#define PMIC_LDO4_11_OCP_CTRL_ADDR(base) ((base) + (0x008EUL))
#define PMIC_LDO12_16_OCP_CTRL_ADDR(base) ((base) + (0x008FUL))
#define PMIC_LDO17_22_OCP_CTRL_ADDR(base) ((base) + (0x0090UL))
#define PMIC_LDO23_26_OCP_CTRL_ADDR(base) ((base) + (0x0091UL))
#define PMIC_LDO27_30_OCP_CTRL_ADDR(base) ((base) + (0x0092UL))
#define PMIC_LDO32_38_OCP_CTRL_ADDR(base) ((base) + (0x0093UL))
#define PMIC_LDO39_42_OCP_CTRL_ADDR(base) ((base) + (0x0094UL))
#define PMIC_LDO43_46_OCP_CTRL_ADDR(base) ((base) + (0x0095UL))
#define PMIC_LDO_BUFF_SCP_CTRL_ADDR(base) ((base) + (0x0096UL))
#define PMIC_BUCK1_40_SCP_CTRL_ADDR(base) ((base) + (0x0097UL))
#define PMIC_BUCK42_71_SCP_CTRL_ADDR(base) ((base) + (0x0098UL))
#define PMIC_BUCK9_SCP_CTRL_ADDR(base) ((base) + (0x0099UL))
#define PMIC_BUCK40_71_OVP_CTRL_ADDR(base) ((base) + (0x009AUL))
#define PMIC_BUCK40_70_OCP_CTRL_ADDR(base) ((base) + (0x009BUL))
#define PMIC_SYS_CTRL_RESERVE_ADDR(base) ((base) + (0x009CUL))
#define PMIC_OCP_DEB_CTRL0_ADDR(base) ((base) + (0x009DUL))
#define PMIC_OCP_DEB_CTRL1_ADDR(base) ((base) + (0x009EUL))
#define PMIC_PWROFF_DEB_CTRL_ADDR(base) ((base) + (0x009FUL))
#define PMIC_VSYS_DROP_DEB_CTRL_ADDR(base) ((base) + (0x00A0UL))
#define PMIC_NPU_CUR_DEL_CTRL0_ADDR(base) ((base) + (0x00A1UL))
#define PMIC_NPU_CUR_DEL_CTRL1_ADDR(base) ((base) + (0x00A2UL))
#define PMIC_OCP_SCP_ONOFF_ADDR(base) ((base) + (0x00A3UL))
#define PMIC_CLK_SERDES_CTRL0_ADDR(base) ((base) + (0x00A4UL))
#define PMIC_CLK_WIFI_CTRL0_ADDR(base) ((base) + (0x00A5UL))
#define PMIC_CLK_WIFI_CTRL1_ADDR(base) ((base) + (0x00A6UL))
#define PMIC_CLK_NFC_CTRL0_ADDR(base) ((base) + (0x00A7UL))
#define PMIC_CLK_NFC_CTRL1_ADDR(base) ((base) + (0x00A8UL))
#define PMIC_CLK_RF0_CTRL0_ADDR(base) ((base) + (0x00A9UL))
#define PMIC_CLK_RF0_CTRL1_ADDR(base) ((base) + (0x00AAUL))
#define PMIC_CLK_RF1_CTRL0_ADDR(base) ((base) + (0x00ABUL))
#define PMIC_CLK_RF1_CTRL1_ADDR(base) ((base) + (0x00ACUL))
#define PMIC_CLK_SYS_CTRL_ADDR(base) ((base) + (0x00ADUL))
#define PMIC_CLK_CODEC_CTRL0_ADDR(base) ((base) + (0x00AEUL))
#define PMIC_CLK_CODEC_CTRL1_ADDR(base) ((base) + (0x00AFUL))
#define PMIC_CLK_UFS_CTRL_ADDR(base) ((base) + (0x00B0UL))
#define PMIC_CLK_EUSB_CTRL_ADDR(base) ((base) + (0x00B1UL))
#define PMIC_TON_ECO_SHIELD_CTRL_ADDR(base) ((base) + (0x00B2UL))
#define PMIC_CLK_TOP_CTRL0_ADDR(base) ((base) + (0x00B3UL))
#define PMIC_CLK_TOP_CTRL1_ADDR(base) ((base) + (0x00B4UL))
#define PMIC_CLK_TOP_CTRL2_ADDR(base) ((base) + (0x00B5UL))
#define PMIC_BG_THSD_CTRL0_ADDR(base) ((base) + (0x00B6UL))
#define PMIC_BG_THSD_CTRL1_ADDR(base) ((base) + (0x00B7UL))
#define PMIC_BG_TEST_ADDR(base) ((base) + (0x00B8UL))
#define PMIC_REF_RESERVE2_ADDR(base) ((base) + (0x00B9UL))
#define PMIC_TSENSOR_CTRL0_ADDR(base) ((base) + (0x00BAUL))
#define PMIC_TSENSOR_RESERVE0_ADDR(base) ((base) + (0x00BBUL))
#define PMIC_TSENSOR_RESERVE1_ADDR(base) ((base) + (0x00BCUL))
#define PMIC_TSENSOR_RESERVE2_ADDR(base) ((base) + (0x00BDUL))
#define PMIC_TSENSOR_RESERVE3_ADDR(base) ((base) + (0x00BEUL))
#define PMIC_HARDWIRE_CTRL0_ADDR(base) ((base) + (0x00BFUL))
#define PMIC_HARDWIRE_CTRL1_ADDR(base) ((base) + (0x00C0UL))
#define PMIC_LOW_POWER_CFG_ADDR(base) ((base) + (0x00C1UL))
#define PMIC_PERI_CTRL0_ADDR(base) ((base) + (0x00C2UL))
#define PMIC_PERI_CTRL1_ADDR(base) ((base) + (0x00C3UL))
#define PMIC_PERI_CTRL2_ADDR(base) ((base) + (0x00C4UL))
#define PMIC_PERI_CTRL3_ADDR(base) ((base) + (0x00C5UL))
#define PMIC_PERI_CTRL4_ADDR(base) ((base) + (0x00C6UL))
#define PMIC_PERI_CTRL5_ADDR(base) ((base) + (0x00C7UL))
#define PMIC_PERI_CTRL6_ADDR(base) ((base) + (0x00C8UL))
#define PMIC_PERI_CTRL7_ADDR(base) ((base) + (0x00C9UL))
#define PMIC_PERI_CTRL8_ADDR(base) ((base) + (0x00CAUL))
#define PMIC_PERI_CTRL9_ADDR(base) ((base) + (0x00CBUL))
#define PMIC_PERI_CNT_CFG_ADDR(base) ((base) + (0x00CCUL))
#define PMIC_PERI_CTRL10_ADDR(base) ((base) + (0x00CDUL))
#define PMIC_PERI_CTRL11_ADDR(base) ((base) + (0x00CEUL))
#define PMIC_PERI_CTRL12_ADDR(base) ((base) + (0x00CFUL))
#define PMIC_PERI_CTRL13_ADDR(base) ((base) + (0x00D0UL))
#define PMIC_PERI_CTRL14_ADDR(base) ((base) + (0x00D1UL))
#define PMIC_PERI_CTRL15_ADDR(base) ((base) + (0x00D2UL))
#define PMIC_PERI_CTRL16_ADDR(base) ((base) + (0x00D3UL))
#define PMIC_PERI_CTRL17_ADDR(base) ((base) + (0x00D4UL))
#define PMIC_PERI_CTRL18_ADDR(base) ((base) + (0x00D5UL))
#define PMIC_PERI_CTRL19_ADDR(base) ((base) + (0x00D6UL))
#define PMIC_PERI_CTRL20_ADDR(base) ((base) + (0x00D7UL))
#define PMIC_PERI_CTRL21_ADDR(base) ((base) + (0x00D8UL))
#define PMIC_PERI_CTRL22_ADDR(base) ((base) + (0x00D9UL))
#define PMIC_PERI_CTRL23_ADDR(base) ((base) + (0x00DAUL))
#define PMIC_PERI_CTRL24_ADDR(base) ((base) + (0x00DBUL))
#define PMIC_PERI_CTRL25_ADDR(base) ((base) + (0x00DCUL))
#define PMIC_PERI_CTRL26_ADDR(base) ((base) + (0x00DDUL))
#define PMIC_PERI_CTRL27_ADDR(base) ((base) + (0x00DEUL))
#define PMIC_PERI_CTRL28_ADDR(base) ((base) + (0x00DFUL))
#define PMIC_PERI_CTRL29_ADDR(base) ((base) + (0x00E0UL))
#define PMIC_PERI_CTRL30_ADDR(base) ((base) + (0x00E1UL))
#define PMIC_PERI_CTRL31_ADDR(base) ((base) + (0x00E2UL))
#define PMIC_PERI_CTRL32_ADDR(base) ((base) + (0x00E3UL))
#define PMIC_PERI_CTRL33_ADDR(base) ((base) + (0x00E4UL))
#define PMIC_PERI_CTRL34_ADDR(base) ((base) + (0x00E5UL))
#define PMIC_PERI_CTRL35_ADDR(base) ((base) + (0x00E6UL))
#define PMIC_PERI_CTRL36_ADDR(base) ((base) + (0x00E7UL))
#define PMIC_PERI_CTRL37_ADDR(base) ((base) + (0x00E8UL))
#define PMIC_PERI_CTRL38_ADDR(base) ((base) + (0x00E9UL))
#define PMIC_PERI_CTRL39_ADDR(base) ((base) + (0x00EAUL))
#define PMIC_PERI_CTRL40_ADDR(base) ((base) + (0x00EBUL))
#define PMIC_PERI_CTRL41_ADDR(base) ((base) + (0x00ECUL))
#define PMIC_PERI_CTRL42_ADDR(base) ((base) + (0x00EDUL))
#define PMIC_PERI_CTRL43_ADDR(base) ((base) + (0x00EEUL))
#define PMIC_PERI_CTRL44_ADDR(base) ((base) + (0x00EFUL))
#define PMIC_PERI_CTRL45_ADDR(base) ((base) + (0x00F0UL))
#define PMIC_WIFI_CTRL_ADDR(base) ((base) + (0x00F1UL))
#define PMIC_PERI_VSET_CTRL_ADDR(base) ((base) + (0x00F2UL))
#define PMIC_IDLE_CTRL0_ADDR(base) ((base) + (0x00F3UL))
#define PMIC_IDLE_CTRL1_ADDR(base) ((base) + (0x00F4UL))
#define PMIC_IDLE_CTRL2_ADDR(base) ((base) + (0x00F5UL))
#define PMIC_IDLE_CTRL3_ADDR(base) ((base) + (0x00F6UL))
#define PMIC_IDLE_CTRL4_ADDR(base) ((base) + (0x00F7UL))
#define PMIC_IDLE_CTRL5_ADDR(base) ((base) + (0x00F8UL))
#define PMIC_IDLE_CTRL6_ADDR(base) ((base) + (0x00F9UL))
#define PMIC_IDLE_CTRL7_ADDR(base) ((base) + (0x00FAUL))
#define PMIC_IDLE_CTRL8_ADDR(base) ((base) + (0x00FBUL))
#define PMIC_IDLE_CTRL9_ADDR(base) ((base) + (0x00FCUL))
#define PMIC_IDLE_CNT_CFG_ADDR(base) ((base) + (0x00FDUL))
#define PMIC_IDLE_CTRL10_ADDR(base) ((base) + (0x00FEUL))
#define PMIC_IDLE_CTRL11_ADDR(base) ((base) + (0x00FFUL))
#define PMIC_IDLE_CTRL12_ADDR(base) ((base) + (0x0100UL))
#define PMIC_IDLE_CTRL13_ADDR(base) ((base) + (0x0101UL))
#define PMIC_IDLE_CTRL14_ADDR(base) ((base) + (0x0102UL))
#define PMIC_IDLE_CTRL15_ADDR(base) ((base) + (0x0103UL))
#define PMIC_IDLE_CTRL16_ADDR(base) ((base) + (0x0104UL))
#define PMIC_IDLE_CTRL17_ADDR(base) ((base) + (0x0105UL))
#define PMIC_IDLE_CTRL18_ADDR(base) ((base) + (0x0106UL))
#define PMIC_IDLE_CTRL19_ADDR(base) ((base) + (0x0107UL))
#define PMIC_IDLE_CTRL20_ADDR(base) ((base) + (0x0108UL))
#define PMIC_IDLE_CTRL21_ADDR(base) ((base) + (0x0109UL))
#define PMIC_IDLE_CTRL22_ADDR(base) ((base) + (0x010AUL))
#define PMIC_IDLE_CTRL23_ADDR(base) ((base) + (0x010BUL))
#define PMIC_IDLE_CTRL24_ADDR(base) ((base) + (0x010CUL))
#define PMIC_IDLE_CTRL25_ADDR(base) ((base) + (0x010DUL))
#define PMIC_IDLE_CTRL26_ADDR(base) ((base) + (0x010EUL))
#define PMIC_IDLE_CTRL27_ADDR(base) ((base) + (0x010FUL))
#define PMIC_IDLE_CTRL28_ADDR(base) ((base) + (0x0110UL))
#define PMIC_IDLE_CTRL29_ADDR(base) ((base) + (0x0111UL))
#define PMIC_IDLE_CTRL30_ADDR(base) ((base) + (0x0112UL))
#define PMIC_IDLE_CTRL31_ADDR(base) ((base) + (0x0113UL))
#define PMIC_IDLE_CTRL32_ADDR(base) ((base) + (0x0114UL))
#define PMIC_IDLE_CTRL33_ADDR(base) ((base) + (0x0115UL))
#define PMIC_IDLE_CTRL34_ADDR(base) ((base) + (0x0116UL))
#define PMIC_IDLE_CTRL35_ADDR(base) ((base) + (0x0117UL))
#define PMIC_IDLE_CTRL36_ADDR(base) ((base) + (0x0118UL))
#define PMIC_IDLE_CTRL37_ADDR(base) ((base) + (0x0119UL))
#define PMIC_IDLE_CTRL38_ADDR(base) ((base) + (0x011AUL))
#define PMIC_IDLE_CTRL39_ADDR(base) ((base) + (0x011BUL))
#define PMIC_IDLE_CTRL40_ADDR(base) ((base) + (0x011CUL))
#define PMIC_IDLE_CTRL41_ADDR(base) ((base) + (0x011DUL))
#define PMIC_IDLE_CTRL42_ADDR(base) ((base) + (0x011EUL))
#define PMIC_IDLE_CTRL43_ADDR(base) ((base) + (0x011FUL))
#define PMIC_IDLE_CTRL44_ADDR(base) ((base) + (0x0120UL))
#define PMIC_IDLE_CTRL45_ADDR(base) ((base) + (0x0121UL))
#define PMIC_SYS_EN_CTRL0_ADDR(base) ((base) + (0x0122UL))
#define PMIC_SYS_EN_CTRL1_ADDR(base) ((base) + (0x0123UL))
#define PMIC_SYS_EN_CTRL2_ADDR(base) ((base) + (0x0124UL))
#define PMIC_SYS_EN_CTRL3_ADDR(base) ((base) + (0x0125UL))
#define PMIC_SYS_EN_CTRL4_ADDR(base) ((base) + (0x0126UL))
#define PMIC_SYS_EN_CTRL5_ADDR(base) ((base) + (0x0127UL))
#define PMIC_SYS_EN_CNT_CFG_ADDR(base) ((base) + (0x0128UL))
#define PMIC_SYS_EN_CTRL6_ADDR(base) ((base) + (0x0129UL))
#define PMIC_SYS_EN_CTRL7_ADDR(base) ((base) + (0x012AUL))
#define PMIC_SYS_EN_CTRL8_ADDR(base) ((base) + (0x012BUL))
#define PMIC_SYS_EN_CTRL9_ADDR(base) ((base) + (0x012CUL))
#define PMIC_SYS_EN_CTRL10_ADDR(base) ((base) + (0x012DUL))
#define PMIC_SYS_EN_CTRL11_ADDR(base) ((base) + (0x012EUL))
#define PMIC_SYS_EN_CTRL12_ADDR(base) ((base) + (0x012FUL))
#define PMIC_SYS_EN_CTRL13_ADDR(base) ((base) + (0x0130UL))
#define PMIC_SYS_EN_CTRL14_ADDR(base) ((base) + (0x0131UL))
#define PMIC_SYS_EN_CTRL15_ADDR(base) ((base) + (0x0132UL))
#define PMIC_SYS_EN_CTRL16_ADDR(base) ((base) + (0x0133UL))
#define PMIC_SYS_EN_CTRL17_ADDR(base) ((base) + (0x0134UL))
#define PMIC_SYS_EN_CTRL18_ADDR(base) ((base) + (0x0135UL))
#define PMIC_SYS_EN_CTRL19_ADDR(base) ((base) + (0x0136UL))
#define PMIC_SYS_EN_CTRL20_ADDR(base) ((base) + (0x0137UL))
#define PMIC_SYS_EN_CTRL21_ADDR(base) ((base) + (0x0138UL))
#define PMIC_SYS_EN_CTRL22_ADDR(base) ((base) + (0x0139UL))
#define PMIC_SYS_EN_CTRL23_ADDR(base) ((base) + (0x013AUL))
#define PMIC_SYS_EN_CTRL24_ADDR(base) ((base) + (0x013BUL))
#define PMIC_SYS_EN_CTRL25_ADDR(base) ((base) + (0x013CUL))
#define PMIC_SYS_EN_CTRL26_ADDR(base) ((base) + (0x013DUL))
#define PMIC_SYS_EN_VSET_CTRL_ADDR(base) ((base) + (0x013EUL))
#define PMIC_AO_MODE_ADDR(base) ((base) + (0x013FUL))
#define PMIC_AO_EUSB_MASK_ADDR(base) ((base) + (0x0140UL))
#define PMIC_COUL_ECO_MASK_ADDR(base) ((base) + (0x0141UL))
#define PMIC_DCR_ECO_MASK_ADDR(base) ((base) + (0x0142UL))
#define PMIC_PMU_SOFT_RST_ADDR(base) ((base) + (0x0143UL))
#define PMIC_LOCK_ADDR(base) ((base) + (0x0144UL))
#define PMIC_DR_EN_123_ADDR(base) ((base) + (0x0145UL))
#define PMIC_DR_EN_MODE_123_ADDR(base) ((base) + (0x0146UL))
#define PMIC_FLASH_PERIOD_DR1_ADDR(base) ((base) + (0x0147UL))
#define PMIC_FLASH_ON_DR1_ADDR(base) ((base) + (0x0148UL))
#define PMIC_FLASH_PERIOD_DR2_ADDR(base) ((base) + (0x0149UL))
#define PMIC_FLASH_ON_DR2_ADDR(base) ((base) + (0x014AUL))
#define PMIC_FLASH_PERIOD_DR3_ADDR(base) ((base) + (0x014BUL))
#define PMIC_FLASH_ON_DR3_ADDR(base) ((base) + (0x014CUL))
#define PMIC_DR_MODE_SEL_ADDR(base) ((base) + (0x014DUL))
#define PMIC_DR_BRE_CTRL_ADDR(base) ((base) + (0x014EUL))
#define PMIC_DR1_ISET_ADDR(base) ((base) + (0x014FUL))
#define PMIC_DR2_ISET_ADDR(base) ((base) + (0x0150UL))
#define PMIC_DR3_ISET_ADDR(base) ((base) + (0x0151UL))
#define PMIC_DR_OUT_CTRL_ADDR(base) ((base) + (0x0152UL))
#define PMIC_DR1_START_DEL_ADDR(base) ((base) + (0x0153UL))
#define PMIC_DR2_START_DEL_ADDR(base) ((base) + (0x0154UL))
#define PMIC_DR3_START_DEL_ADDR(base) ((base) + (0x0155UL))
#define PMIC_DR1_TIM_CONF0_ADDR(base) ((base) + (0x0156UL))
#define PMIC_DR1_TIM_CONF1_ADDR(base) ((base) + (0x0157UL))
#define PMIC_DR2_TIM_CONF0_ADDR(base) ((base) + (0x0158UL))
#define PMIC_DR2_TIM_CONF1_ADDR(base) ((base) + (0x0159UL))
#define PMIC_DR3_TIM_CONF0_ADDR(base) ((base) + (0x015AUL))
#define PMIC_DR3_TIM_CONF1_ADDR(base) ((base) + (0x015BUL))
#define PMIC_DR_CTRLRESERVE8_ADDR(base) ((base) + (0x015CUL))
#define PMIC_DR_CTRLRESERVE9_ADDR(base) ((base) + (0x015DUL))
#define PMIC_OTP0_0_ADDR(base) ((base) + (0x0160UL))
#define PMIC_OTP0_1_ADDR(base) ((base) + (0x0161UL))
#define PMIC_OTP0_CTRL0_ADDR(base) ((base) + (0x0162UL))
#define PMIC_OTP0_CTRL1_ADDR(base) ((base) + (0x0163UL))
#define PMIC_OTP0_CTRL2_ADDR(base) ((base) + (0x0164UL))
#define PMIC_OTP0_WDATA_ADDR(base) ((base) + (0x0165UL))
#define PMIC_OTP0_0_W_ADDR(base) ((base) + (0x0166UL))
#define PMIC_OTP0_1_W_ADDR(base) ((base) + (0x0167UL))
#define PMIC_OTP0_2_W_ADDR(base) ((base) + (0x0168UL))
#define PMIC_OTP0_3_W_ADDR(base) ((base) + (0x0169UL))
#define PMIC_OTP0_4_W_ADDR(base) ((base) + (0x016AUL))
#define PMIC_OTP0_5_W_ADDR(base) ((base) + (0x016BUL))
#define PMIC_OTP0_6_W_ADDR(base) ((base) + (0x016CUL))
#define PMIC_OTP0_7_W_ADDR(base) ((base) + (0x016DUL))
#define PMIC_OTP0_8_W_ADDR(base) ((base) + (0x016EUL))
#define PMIC_OTP0_9_W_ADDR(base) ((base) + (0x016FUL))
#define PMIC_OTP0_10_W_ADDR(base) ((base) + (0x0170UL))
#define PMIC_OTP0_11_W_ADDR(base) ((base) + (0x0171UL))
#define PMIC_OTP0_12_W_ADDR(base) ((base) + (0x0172UL))
#define PMIC_OTP0_13_W_ADDR(base) ((base) + (0x0173UL))
#define PMIC_OTP0_14_W_ADDR(base) ((base) + (0x0174UL))
#define PMIC_OTP0_15_W_ADDR(base) ((base) + (0x0175UL))
#define PMIC_OTP0_16_W_ADDR(base) ((base) + (0x0176UL))
#define PMIC_OTP0_17_W_ADDR(base) ((base) + (0x0177UL))
#define PMIC_OTP0_18_W_ADDR(base) ((base) + (0x0178UL))
#define PMIC_OTP0_19_W_ADDR(base) ((base) + (0x0179UL))
#define PMIC_OTP0_20_W_ADDR(base) ((base) + (0x017AUL))
#define PMIC_OTP0_21_W_ADDR(base) ((base) + (0x017BUL))
#define PMIC_OTP0_22_W_ADDR(base) ((base) + (0x017CUL))
#define PMIC_OTP0_23_W_ADDR(base) ((base) + (0x017DUL))
#define PMIC_OTP0_24_W_ADDR(base) ((base) + (0x017EUL))
#define PMIC_OTP0_25_W_ADDR(base) ((base) + (0x017FUL))
#define PMIC_OTP0_26_W_ADDR(base) ((base) + (0x0180UL))
#define PMIC_OTP0_27_W_ADDR(base) ((base) + (0x0181UL))
#define PMIC_OTP0_28_W_ADDR(base) ((base) + (0x0182UL))
#define PMIC_OTP0_29_W_ADDR(base) ((base) + (0x0183UL))
#define PMIC_OTP0_30_W_ADDR(base) ((base) + (0x0184UL))
#define PMIC_OTP0_31_W_ADDR(base) ((base) + (0x0185UL))
#define PMIC_OTP0_32_W_ADDR(base) ((base) + (0x0186UL))
#define PMIC_OTP0_33_W_ADDR(base) ((base) + (0x0187UL))
#define PMIC_OTP0_34_W_ADDR(base) ((base) + (0x0188UL))
#define PMIC_OTP0_35_W_ADDR(base) ((base) + (0x0189UL))
#define PMIC_OTP0_36_W_ADDR(base) ((base) + (0x018AUL))
#define PMIC_OTP0_37_W_ADDR(base) ((base) + (0x018BUL))
#define PMIC_OTP0_38_W_ADDR(base) ((base) + (0x018CUL))
#define PMIC_OTP0_39_W_ADDR(base) ((base) + (0x018DUL))
#define PMIC_OTP0_40_W_ADDR(base) ((base) + (0x018EUL))
#define PMIC_OTP0_41_W_ADDR(base) ((base) + (0x018FUL))
#define PMIC_OTP0_42_W_ADDR(base) ((base) + (0x0190UL))
#define PMIC_OTP0_43_W_ADDR(base) ((base) + (0x0191UL))
#define PMIC_OTP0_44_W_ADDR(base) ((base) + (0x0192UL))
#define PMIC_OTP0_45_W_ADDR(base) ((base) + (0x0193UL))
#define PMIC_OTP0_46_W_ADDR(base) ((base) + (0x0194UL))
#define PMIC_OTP0_47_W_ADDR(base) ((base) + (0x0195UL))
#define PMIC_OTP0_48_W_ADDR(base) ((base) + (0x0196UL))
#define PMIC_OTP0_49_W_ADDR(base) ((base) + (0x0197UL))
#define PMIC_OTP0_50_W_ADDR(base) ((base) + (0x0198UL))
#define PMIC_OTP0_51_W_ADDR(base) ((base) + (0x0199UL))
#define PMIC_OTP0_52_W_ADDR(base) ((base) + (0x019AUL))
#define PMIC_OTP0_53_W_ADDR(base) ((base) + (0x019BUL))
#define PMIC_OTP0_54_W_ADDR(base) ((base) + (0x019CUL))
#define PMIC_OTP0_55_W_ADDR(base) ((base) + (0x019DUL))
#define PMIC_OTP0_56_W_ADDR(base) ((base) + (0x019EUL))
#define PMIC_OTP0_57_W_ADDR(base) ((base) + (0x019FUL))
#define PMIC_OTP0_58_W_ADDR(base) ((base) + (0x01A0UL))
#define PMIC_OTP0_59_W_ADDR(base) ((base) + (0x01A1UL))
#define PMIC_OTP0_60_W_ADDR(base) ((base) + (0x01A2UL))
#define PMIC_OTP0_61_W_ADDR(base) ((base) + (0x01A3UL))
#define PMIC_OTP0_62_W_ADDR(base) ((base) + (0x01A4UL))
#define PMIC_OTP0_63_W_ADDR(base) ((base) + (0x01A5UL))
#define PMIC_OTP1_0_ADDR(base) ((base) + (0x01A6UL))
#define PMIC_OTP1_1_ADDR(base) ((base) + (0x01A7UL))
#define PMIC_OTP1_CTRL0_ADDR(base) ((base) + (0x01A8UL))
#define PMIC_OTP1_CTRL1_ADDR(base) ((base) + (0x01A9UL))
#define PMIC_OTP1_CTRL2_ADDR(base) ((base) + (0x01AAUL))
#define PMIC_OTP1_WDATA_ADDR(base) ((base) + (0x01ABUL))
#define PMIC_OTP1_0_W_ADDR(base) ((base) + (0x01ACUL))
#define PMIC_OTP1_1_W_ADDR(base) ((base) + (0x01ADUL))
#define PMIC_OTP1_2_W_ADDR(base) ((base) + (0x01AEUL))
#define PMIC_OTP1_3_W_ADDR(base) ((base) + (0x01AFUL))
#define PMIC_OTP1_4_W_ADDR(base) ((base) + (0x01B0UL))
#define PMIC_OTP1_5_W_ADDR(base) ((base) + (0x01B1UL))
#define PMIC_OTP1_6_W_ADDR(base) ((base) + (0x01B2UL))
#define PMIC_SIM_CTRL0_ADDR(base) ((base) + (0x01B3UL))
#define PMIC_SIM_CTRL1_ADDR(base) ((base) + (0x01B4UL))
#define PMIC_SIM_DEB_CTRL1_ADDR(base) ((base) + (0x01B5UL))
#define PMIC_SIM_DEB_CTRL2_ADDR(base) ((base) + (0x01B6UL))
#define PMIC_GPIO0DATA_ADDR(base) ((base) + (0x01B7UL))
#define PMIC_GPIO0_DATA_RESERVE_ADDR(base) ((base) + (0x01B8UL))
#define PMIC_GPIO0DIR_ADDR(base) ((base) + (0x01B9UL))
#define PMIC_GPIO0_DIR_RESERVE_ADDR(base) ((base) + (0x01BAUL))
#define PMIC_GPIO0IS_ADDR(base) ((base) + (0x01BBUL))
#define PMIC_GPIO0IBE_ADDR(base) ((base) + (0x01BCUL))
#define PMIC_GPIO0IEV_ADDR(base) ((base) + (0x01BDUL))
#define PMIC_GPIO0AFSEL_ADDR(base) ((base) + (0x01BEUL))
#define PMIC_GPIO0DSSEL_ADDR(base) ((base) + (0x01BFUL))
#define PMIC_GPIO0PUSEL_ADDR(base) ((base) + (0x01C0UL))
#define PMIC_GPIO0PDSEL_ADDR(base) ((base) + (0x01C1UL))
#define PMIC_GPIO0DEBSEL_ADDR(base) ((base) + (0x01C2UL))
#define PMIC_SIM0PUSEL_ADDR(base) ((base) + (0x01C3UL))
#define PMIC_SIM0PDSEL_ADDR(base) ((base) + (0x01C4UL))
#define PMIC_GPIO1DATA_ADDR(base) ((base) + (0x01C5UL))
#define PMIC_GPIO1_DATA_RESERVE_ADDR(base) ((base) + (0x01C6UL))
#define PMIC_GPIO1DIR_ADDR(base) ((base) + (0x01C7UL))
#define PMIC_GPIO1_DIR_RESERVE_ADDR(base) ((base) + (0x01C8UL))
#define PMIC_GPIO1IS_ADDR(base) ((base) + (0x01C9UL))
#define PMIC_GPIO1IBE_ADDR(base) ((base) + (0x01CAUL))
#define PMIC_GPIO1IEV_ADDR(base) ((base) + (0x01CBUL))
#define PMIC_GPIO1AFSEL_ADDR(base) ((base) + (0x01CCUL))
#define PMIC_GPIO1DSSEL_ADDR(base) ((base) + (0x01CDUL))
#define PMIC_GPIO1PUSEL_ADDR(base) ((base) + (0x01CEUL))
#define PMIC_GPIO1PDSEL_ADDR(base) ((base) + (0x01CFUL))
#define PMIC_GPIO1DEBSEL_ADDR(base) ((base) + (0x01D0UL))
#define PMIC_SIM1PUSEL_ADDR(base) ((base) + (0x01D1UL))
#define PMIC_SIM1PDSEL_ADDR(base) ((base) + (0x01D2UL))
#define PMIC_SIM1_RST_MODE_DEL_ADDR(base) ((base) + (0x01D3UL))
#define PMIC_RAMP_BUCK40_CTRL0_ADDR(base) ((base) + (0x01D4UL))
#define PMIC_RAMP_BUCK40_CTRL1_ADDR(base) ((base) + (0x01D5UL))
#define PMIC_BUCK40_RAMPUP_STATE_ADDR(base) ((base) + (0x01D6UL))
#define PMIC_BUCK40_RAMPDOWN_STATE_ADDR(base) ((base) + (0x01D7UL))
#define PMIC_BUCK40_RAMP_CHG_CTRL0_ADDR(base) ((base) + (0x01D8UL))
#define PMIC_BUCK40_RAMP_CHG_CTRL1_ADDR(base) ((base) + (0x01D9UL))
#define PMIC_RAMP_BUCK42_CTRL0_ADDR(base) ((base) + (0x01DAUL))
#define PMIC_RAMP_BUCK42_CTRL1_ADDR(base) ((base) + (0x01DBUL))
#define PMIC_BUCK42_RAMPUP_STATE_ADDR(base) ((base) + (0x01DCUL))
#define PMIC_BUCK42_RAMPDOWN_STATE_ADDR(base) ((base) + (0x01DDUL))
#define PMIC_BUCK42_RAMP_CHG_CTRL0_ADDR(base) ((base) + (0x01DEUL))
#define PMIC_BUCK42_RAMP_CHG_CTRL1_ADDR(base) ((base) + (0x01DFUL))
#define PMIC_RAMP_BUCK5_CTRL0_ADDR(base) ((base) + (0x01E0UL))
#define PMIC_RAMP_BUCK5_CTRL1_ADDR(base) ((base) + (0x01E1UL))
#define PMIC_BUCK5_RAMPUP_STATE_ADDR(base) ((base) + (0x01E2UL))
#define PMIC_BUCK5_RAMPDOWN_STATE_ADDR(base) ((base) + (0x01E3UL))
#define PMIC_BUCK5_RAMP_CHG_CTRL0_ADDR(base) ((base) + (0x01E4UL))
#define PMIC_BUCK5_RAMP_CHG_CTRL1_ADDR(base) ((base) + (0x01E5UL))
#define PMIC_RAMP_BUCK70_CTRL0_ADDR(base) ((base) + (0x01E6UL))
#define PMIC_RAMP_BUCK70_CTRL1_ADDR(base) ((base) + (0x01E7UL))
#define PMIC_BUCK70_RAMPUP_STATE_ADDR(base) ((base) + (0x01E8UL))
#define PMIC_BUCK70_RAMPDOWN_STATE_ADDR(base) ((base) + (0x01E9UL))
#define PMIC_BUCK70_RAMP_CHG_CTRL0_ADDR(base) ((base) + (0x01EAUL))
#define PMIC_BUCK70_RAMP_CHG_CTRL1_ADDR(base) ((base) + (0x01EBUL))
#define PMIC_RAMP_BUCK71_CTRL0_ADDR(base) ((base) + (0x01ECUL))
#define PMIC_RAMP_BUCK71_CTRL1_ADDR(base) ((base) + (0x01EDUL))
#define PMIC_BUCK71_RAMPUP_STATE_ADDR(base) ((base) + (0x01EEUL))
#define PMIC_BUCK71_RAMPDOWN_STATE_ADDR(base) ((base) + (0x01EFUL))
#define PMIC_BUCK71_RAMP_CHG_CTRL0_ADDR(base) ((base) + (0x01F0UL))
#define PMIC_BUCK71_RAMP_CHG_CTRL1_ADDR(base) ((base) + (0x01F1UL))
#define PMIC_RAMP_LDO0_CTRL0_ADDR(base) ((base) + (0x01F2UL))
#define PMIC_RAMP_LDO0_CTRL1_ADDR(base) ((base) + (0x01F3UL))
#define PMIC_RAMP_LDO44_CTRL0_ADDR(base) ((base) + (0x01F4UL))
#define PMIC_RAMP_LDO44_CTRL1_ADDR(base) ((base) + (0x01F5UL))
#define PMIC_RAMP_LDO45_CTRL0_ADDR(base) ((base) + (0x01F6UL))
#define PMIC_RAMP_LDO45_CTRL1_ADDR(base) ((base) + (0x01F7UL))
#define PMIC_SPMI_ECO_GT_BYPASS_ADDR(base) ((base) + (0x01F8UL))
#define PMIC_PWRONN_8S_XOADC_DEBUG_CTRL_ADDR(base) ((base) + (0x01F9UL))
#define PMIC_DIS_19M2_CTRL_ADDR(base) ((base) + (0x01FAUL))
#define PMIC_FAULT_S_N_CTRL_ADDR(base) ((base) + (0x01FBUL))
#define PMIC_PWRONN_CNT_ADDR(base) ((base) + (0x01FCUL))
#define PMIC_PWRONN_CNT_MASK_ADDR(base) ((base) + (0x01FDUL))
#else
#define PMIC_VERSION0_ADDR(base) ((base) + (0x0000))
#define PMIC_VERSION1_ADDR(base) ((base) + (0x0001))
#define PMIC_VERSION2_ADDR(base) ((base) + (0x0002))
#define PMIC_VERSION3_ADDR(base) ((base) + (0x0003))
#define PMIC_VERSION4_ADDR(base) ((base) + (0x0004))
#define PMIC_VERSION5_ADDR(base) ((base) + (0x0005))
#define PMIC_STATUS0_ADDR(base) ((base) + (0x0006))
#define PMIC_STATUS1_ADDR(base) ((base) + (0x0007))
#define PMIC_STATUS2_ADDR(base) ((base) + (0x0008))
#define PMIC_STATUS3_ADDR(base) ((base) + (0x0009))
#define PMIC_STATUS4_ADDR(base) ((base) + (0x000A))
#define PMIC_STATUS5_ADDR(base) ((base) + (0x000B))
#define PMIC_STATUS6_ADDR(base) ((base) + (0x000C))
#define PMIC_BUCK1_ONOFF_ECO_ADDR(base) ((base) + (0x000D))
#define PMIC_BUCK2_ONOFF_ECO_ADDR(base) ((base) + (0x000E))
#define PMIC_BUCK3_ONOFF_ECO_ADDR(base) ((base) + (0x000F))
#define PMIC_BUCK40_ONOFF_ECO_ADDR(base) ((base) + (0x0010))
#define PMIC_BUCK41_ONOFF_ECO_ADDR(base) ((base) + (0x0011))
#define PMIC_BUCK42_ONOFF_ECO_ADDR(base) ((base) + (0x0012))
#define PMIC_BUCK5_ONOFF_ECO_ADDR(base) ((base) + (0x0013))
#define PMIC_BUCK70_ONOFF1_ECO_ADDR(base) ((base) + (0x0014))
#define PMIC_BUCK70_ONOFF2_ADDR(base) ((base) + (0x0015))
#define PMIC_BUCK70_ONOFF3_ADDR(base) ((base) + (0x0016))
#define PMIC_BUCK71_ONOFF_ECO_ADDR(base) ((base) + (0x0017))
#define PMIC_BUCK9_ONOFF_ECO_ADDR(base) ((base) + (0x0018))
#define PMIC_LDO0_ONOFF_ECO_ADDR(base) ((base) + (0x0019))
#define PMIC_LDO1_ONOFF_ECO_ADDR(base) ((base) + (0x001A))
#define PMIC_LDO3_ONOFF_ADDR(base) ((base) + (0x001B))
#define PMIC_LDO4_ONOFF_ECO_ADDR(base) ((base) + (0x001C))
#define PMIC_LDO6_ONOFF_ECO_ADDR(base) ((base) + (0x001D))
#define PMIC_LDO8_ONOFF_ECO_ADDR(base) ((base) + (0x001E))
#define PMIC_LDO11_ONOFF_ECO_ADDR(base) ((base) + (0x001F))
#define PMIC_LDO12_ONOFF_ECO_ADDR(base) ((base) + (0x0020))
#define PMIC_LDO14_ONOFF_ECO_ADDR(base) ((base) + (0x0021))
#define PMIC_LDO15_ONOFF_ECO_ADDR(base) ((base) + (0x0022))
#define PMIC_LDO16_ONOFF_ECO_ADDR(base) ((base) + (0x0023))
#define PMIC_LDO17_ONOFF_ECO_ADDR(base) ((base) + (0x0024))
#define PMIC_LDO18_ONOFF_ECO_ADDR(base) ((base) + (0x0025))
#define PMIC_LDO21_ONOFF_ECO_ADDR(base) ((base) + (0x0026))
#define PMIC_LDO22_ONOFF_ADDR(base) ((base) + (0x0027))
#define PMIC_LDO23_ONOFF_ECO_ADDR(base) ((base) + (0x0028))
#define PMIC_LDO24_ONOFF_ECO_ADDR(base) ((base) + (0x0029))
#define PMIC_LDO25_ONOFF_ADDR(base) ((base) + (0x002A))
#define PMIC_LDO26_ONOFF_ECO_ADDR(base) ((base) + (0x002B))
#define PMIC_LDO27_ONOFF_ECO_ADDR(base) ((base) + (0x002C))
#define PMIC_LDO28_ONOFF_ECO_ADDR(base) ((base) + (0x002D))
#define PMIC_LDO29_ONOFF_ECO_ADDR(base) ((base) + (0x002E))
#define PMIC_LDO30_ONOFF_ECO_ADDR(base) ((base) + (0x002F))
#define PMIC_LDO32_ONOFF_ECO_ADDR(base) ((base) + (0x0030))
#define PMIC_PMUH_ONOFF_ECO_ADDR(base) ((base) + (0x0031))
#define PMIC_LDO36_ONOFF_ECO_ADDR(base) ((base) + (0x0032))
#define PMIC_LDO37_ONOFF_ECO_ADDR(base) ((base) + (0x0033))
#define PMIC_LDO38_ONOFF_ECO_ADDR(base) ((base) + (0x0034))
#define PMIC_LDO40_ONOFF_ECO_ADDR(base) ((base) + (0x0035))
#define PMIC_LDO41_ONOFF_ECO_ADDR(base) ((base) + (0x0036))
#define PMIC_LDO42_ONOFF_ECO_ADDR(base) ((base) + (0x0037))
#define PMIC_LDO43_ONOFF_ECO_ADDR(base) ((base) + (0x0038))
#define PMIC_LDO44_ONOFF_ECO_ADDR(base) ((base) + (0x0039))
#define PMIC_LDO45_ONOFF_ECO_ADDR(base) ((base) + (0x003A))
#define PMIC_LDO46_ONOFF_ECO_ADDR(base) ((base) + (0x003B))
#define PMIC_SW1_ONOFF_ADDR(base) ((base) + (0x003C))
#define PMIC_LDO_SINK_ONOFF_ECO_ADDR(base) ((base) + (0x003D))
#define PMIC_REF_ECO_ADDR(base) ((base) + (0x003E))
#define PMIC_EXT_PMU_ONOFF_ADDR(base) ((base) + (0x003F))
#define PMIC_CLK_SERDES_EN_ADDR(base) ((base) + (0x0040))
#define PMIC_CLK_WIFI_EN_ADDR(base) ((base) + (0x0041))
#define PMIC_CLK_NFC_EN_ADDR(base) ((base) + (0x0042))
#define PMIC_CLK_RF0_EN_ADDR(base) ((base) + (0x0043))
#define PMIC_CLK_RF1_EN_ADDR(base) ((base) + (0x0044))
#define PMIC_CLK_SYS_EN_ADDR(base) ((base) + (0x0045))
#define PMIC_CLK_CODEC_EN_ADDR(base) ((base) + (0x0046))
#define PMIC_CLK_UFS_EN_ADDR(base) ((base) + (0x0047))
#define PMIC_CLK_EUSB_EN_ADDR(base) ((base) + (0x0048))
#define PMIC_OSC32K_GPS_ONOFF_CTRL_ADDR(base) ((base) + (0x0049))
#define PMIC_OSC32K_BT_ONOFF_CTRL_ADDR(base) ((base) + (0x004A))
#define PMIC_OSC32K_SYS_ONOFF_CTRL_ADDR(base) ((base) + (0x004B))
#define PMIC_CLK_32K_PMU_CTRL0_ADDR(base) ((base) + (0x004C))
#define PMIC_CLK_32K_PMU_CTRL1_ADDR(base) ((base) + (0x004D))
#define PMIC_LRA_CLK_ONOFF1_ADDR(base) ((base) + (0x004E))
#define PMIC_LRA_CLK_ONOFF2_ADDR(base) ((base) + (0x004F))
#define PMIC_BUCK1_VSET_ADDR(base) ((base) + (0x0050))
#define PMIC_BUCK2_VSET_ADDR(base) ((base) + (0x0051))
#define PMIC_BUCK2_VSET_ECO_ADDR(base) ((base) + (0x0052))
#define PMIC_BUCK3_VSET_ADDR(base) ((base) + (0x0053))
#define PMIC_BUCK3_VSET_ECO_ADDR(base) ((base) + (0x0054))
#define PMIC_BUCK40_VSET_ADDR(base) ((base) + (0x0055))
#define PMIC_BUCK42_VSET_ADDR(base) ((base) + (0x0056))
#define PMIC_BUCK5_VSET_ADDR(base) ((base) + (0x0057))
#define PMIC_BUCK5_VSET_ECO_ADDR(base) ((base) + (0x0058))
#define PMIC_BUCK70_VSET_ADDR(base) ((base) + (0x0059))
#define PMIC_BUCK71_VSET_ADDR(base) ((base) + (0x005A))
#define PMIC_BUCK9_VSET_ADDR(base) ((base) + (0x005B))
#define PMIC_BUCK9_VSET_ECO_ADDR(base) ((base) + (0x005C))
#define PMIC_LDO0_VSET_ADDR(base) ((base) + (0x005D))
#define PMIC_LDO0_VSET_ECO_ADDR(base) ((base) + (0x005E))
#define PMIC_LDO1_VSET_ADDR(base) ((base) + (0x005F))
#define PMIC_LDO3_VSET_ADDR(base) ((base) + (0x0060))
#define PMIC_LDO4_VSET_ADDR(base) ((base) + (0x0061))
#define PMIC_LDO6_VSET_ADDR(base) ((base) + (0x0062))
#define PMIC_LDO8_VSET_ADDR(base) ((base) + (0x0063))
#define PMIC_LDO11_VSET_ADDR(base) ((base) + (0x0064))
#define PMIC_LDO12_VSET_ADDR(base) ((base) + (0x0065))
#define PMIC_LDO14_VSET_ADDR(base) ((base) + (0x0066))
#define PMIC_LDO15_VSET_ADDR(base) ((base) + (0x0067))
#define PMIC_LDO16_VSET_ADDR(base) ((base) + (0x0068))
#define PMIC_LDO17_VSET_ADDR(base) ((base) + (0x0069))
#define PMIC_LDO18_VSET_ADDR(base) ((base) + (0x006A))
#define PMIC_LDO21_VSET_ADDR(base) ((base) + (0x006B))
#define PMIC_LDO22_VSET_ADDR(base) ((base) + (0x006C))
#define PMIC_LDO23_VSET_ADDR(base) ((base) + (0x006D))
#define PMIC_LDO24_VSET_ADDR(base) ((base) + (0x006E))
#define PMIC_LDO25_VSET_ADDR(base) ((base) + (0x006F))
#define PMIC_LDO26_VSET_ADDR(base) ((base) + (0x0070))
#define PMIC_LDO27_VSET_ADDR(base) ((base) + (0x0071))
#define PMIC_LDO28_VSET_ADDR(base) ((base) + (0x0072))
#define PMIC_LDO29_VSET_ADDR(base) ((base) + (0x0073))
#define PMIC_LDO30_VSET_ADDR(base) ((base) + (0x0074))
#define PMIC_LDO30_VSET_ECO_ADDR(base) ((base) + (0x0075))
#define PMIC_LDO32_VSET_ADDR(base) ((base) + (0x0076))
#define PMIC_PMUH_VSET_ADDR(base) ((base) + (0x0077))
#define PMIC_LDO36_VSET_ADDR(base) ((base) + (0x0078))
#define PMIC_LDO37_VSET_ADDR(base) ((base) + (0x0079))
#define PMIC_LDO38_VSET_ADDR(base) ((base) + (0x007A))
#define PMIC_LDO40_VSET_ADDR(base) ((base) + (0x007B))
#define PMIC_LDO40_VSET_ECO_ADDR(base) ((base) + (0x007C))
#define PMIC_LDO41_VSET_ADDR(base) ((base) + (0x007D))
#define PMIC_LDO42_VSET_ADDR(base) ((base) + (0x007E))
#define PMIC_LDO43_VSET_ADDR(base) ((base) + (0x007F))
#define PMIC_LDO44_VSET_ADDR(base) ((base) + (0x0080))
#define PMIC_LDO44_VSET_ECO_ADDR(base) ((base) + (0x0081))
#define PMIC_LDO45_VSET_ADDR(base) ((base) + (0x0082))
#define PMIC_LDO46_VSET_ADDR(base) ((base) + (0x0083))
#define PMIC_LDO_SINK_VSET_ADDR(base) ((base) + (0x0084))
#define PMIC_LDO_BUF_VSET_ADDR(base) ((base) + (0x0085))
#define PMIC_PMUD_VSET_ADDR(base) ((base) + (0x0086))
#define PMIC_BUCK1_40_OCP_CTRL_ADDR(base) ((base) + (0x008A))
#define PMIC_BUCK42_71_OCP_CTRL_ADDR(base) ((base) + (0x008B))
#define PMIC_BUCK9_OCP_CTRL_ADDR(base) ((base) + (0x008C))
#define PMIC_LDO0_3_OCP_CTRL_ADDR(base) ((base) + (0x008D))
#define PMIC_LDO4_11_OCP_CTRL_ADDR(base) ((base) + (0x008E))
#define PMIC_LDO12_16_OCP_CTRL_ADDR(base) ((base) + (0x008F))
#define PMIC_LDO17_22_OCP_CTRL_ADDR(base) ((base) + (0x0090))
#define PMIC_LDO23_26_OCP_CTRL_ADDR(base) ((base) + (0x0091))
#define PMIC_LDO27_30_OCP_CTRL_ADDR(base) ((base) + (0x0092))
#define PMIC_LDO32_38_OCP_CTRL_ADDR(base) ((base) + (0x0093))
#define PMIC_LDO39_42_OCP_CTRL_ADDR(base) ((base) + (0x0094))
#define PMIC_LDO43_46_OCP_CTRL_ADDR(base) ((base) + (0x0095))
#define PMIC_LDO_BUFF_SCP_CTRL_ADDR(base) ((base) + (0x0096))
#define PMIC_BUCK1_40_SCP_CTRL_ADDR(base) ((base) + (0x0097))
#define PMIC_BUCK42_71_SCP_CTRL_ADDR(base) ((base) + (0x0098))
#define PMIC_BUCK9_SCP_CTRL_ADDR(base) ((base) + (0x0099))
#define PMIC_BUCK40_71_OVP_CTRL_ADDR(base) ((base) + (0x009A))
#define PMIC_BUCK40_70_OCP_CTRL_ADDR(base) ((base) + (0x009B))
#define PMIC_SYS_CTRL_RESERVE_ADDR(base) ((base) + (0x009C))
#define PMIC_OCP_DEB_CTRL0_ADDR(base) ((base) + (0x009D))
#define PMIC_OCP_DEB_CTRL1_ADDR(base) ((base) + (0x009E))
#define PMIC_PWROFF_DEB_CTRL_ADDR(base) ((base) + (0x009F))
#define PMIC_VSYS_DROP_DEB_CTRL_ADDR(base) ((base) + (0x00A0))
#define PMIC_NPU_CUR_DEL_CTRL0_ADDR(base) ((base) + (0x00A1))
#define PMIC_NPU_CUR_DEL_CTRL1_ADDR(base) ((base) + (0x00A2))
#define PMIC_OCP_SCP_ONOFF_ADDR(base) ((base) + (0x00A3))
#define PMIC_CLK_SERDES_CTRL0_ADDR(base) ((base) + (0x00A4))
#define PMIC_CLK_WIFI_CTRL0_ADDR(base) ((base) + (0x00A5))
#define PMIC_CLK_WIFI_CTRL1_ADDR(base) ((base) + (0x00A6))
#define PMIC_CLK_NFC_CTRL0_ADDR(base) ((base) + (0x00A7))
#define PMIC_CLK_NFC_CTRL1_ADDR(base) ((base) + (0x00A8))
#define PMIC_CLK_RF0_CTRL0_ADDR(base) ((base) + (0x00A9))
#define PMIC_CLK_RF0_CTRL1_ADDR(base) ((base) + (0x00AA))
#define PMIC_CLK_RF1_CTRL0_ADDR(base) ((base) + (0x00AB))
#define PMIC_CLK_RF1_CTRL1_ADDR(base) ((base) + (0x00AC))
#define PMIC_CLK_SYS_CTRL_ADDR(base) ((base) + (0x00AD))
#define PMIC_CLK_CODEC_CTRL0_ADDR(base) ((base) + (0x00AE))
#define PMIC_CLK_CODEC_CTRL1_ADDR(base) ((base) + (0x00AF))
#define PMIC_CLK_UFS_CTRL_ADDR(base) ((base) + (0x00B0))
#define PMIC_CLK_EUSB_CTRL_ADDR(base) ((base) + (0x00B1))
#define PMIC_TON_ECO_SHIELD_CTRL_ADDR(base) ((base) + (0x00B2))
#define PMIC_CLK_TOP_CTRL0_ADDR(base) ((base) + (0x00B3))
#define PMIC_CLK_TOP_CTRL1_ADDR(base) ((base) + (0x00B4))
#define PMIC_CLK_TOP_CTRL2_ADDR(base) ((base) + (0x00B5))
#define PMIC_BG_THSD_CTRL0_ADDR(base) ((base) + (0x00B6))
#define PMIC_BG_THSD_CTRL1_ADDR(base) ((base) + (0x00B7))
#define PMIC_BG_TEST_ADDR(base) ((base) + (0x00B8))
#define PMIC_REF_RESERVE2_ADDR(base) ((base) + (0x00B9))
#define PMIC_TSENSOR_CTRL0_ADDR(base) ((base) + (0x00BA))
#define PMIC_TSENSOR_RESERVE0_ADDR(base) ((base) + (0x00BB))
#define PMIC_TSENSOR_RESERVE1_ADDR(base) ((base) + (0x00BC))
#define PMIC_TSENSOR_RESERVE2_ADDR(base) ((base) + (0x00BD))
#define PMIC_TSENSOR_RESERVE3_ADDR(base) ((base) + (0x00BE))
#define PMIC_HARDWIRE_CTRL0_ADDR(base) ((base) + (0x00BF))
#define PMIC_HARDWIRE_CTRL1_ADDR(base) ((base) + (0x00C0))
#define PMIC_LOW_POWER_CFG_ADDR(base) ((base) + (0x00C1))
#define PMIC_PERI_CTRL0_ADDR(base) ((base) + (0x00C2))
#define PMIC_PERI_CTRL1_ADDR(base) ((base) + (0x00C3))
#define PMIC_PERI_CTRL2_ADDR(base) ((base) + (0x00C4))
#define PMIC_PERI_CTRL3_ADDR(base) ((base) + (0x00C5))
#define PMIC_PERI_CTRL4_ADDR(base) ((base) + (0x00C6))
#define PMIC_PERI_CTRL5_ADDR(base) ((base) + (0x00C7))
#define PMIC_PERI_CTRL6_ADDR(base) ((base) + (0x00C8))
#define PMIC_PERI_CTRL7_ADDR(base) ((base) + (0x00C9))
#define PMIC_PERI_CTRL8_ADDR(base) ((base) + (0x00CA))
#define PMIC_PERI_CTRL9_ADDR(base) ((base) + (0x00CB))
#define PMIC_PERI_CNT_CFG_ADDR(base) ((base) + (0x00CC))
#define PMIC_PERI_CTRL10_ADDR(base) ((base) + (0x00CD))
#define PMIC_PERI_CTRL11_ADDR(base) ((base) + (0x00CE))
#define PMIC_PERI_CTRL12_ADDR(base) ((base) + (0x00CF))
#define PMIC_PERI_CTRL13_ADDR(base) ((base) + (0x00D0))
#define PMIC_PERI_CTRL14_ADDR(base) ((base) + (0x00D1))
#define PMIC_PERI_CTRL15_ADDR(base) ((base) + (0x00D2))
#define PMIC_PERI_CTRL16_ADDR(base) ((base) + (0x00D3))
#define PMIC_PERI_CTRL17_ADDR(base) ((base) + (0x00D4))
#define PMIC_PERI_CTRL18_ADDR(base) ((base) + (0x00D5))
#define PMIC_PERI_CTRL19_ADDR(base) ((base) + (0x00D6))
#define PMIC_PERI_CTRL20_ADDR(base) ((base) + (0x00D7))
#define PMIC_PERI_CTRL21_ADDR(base) ((base) + (0x00D8))
#define PMIC_PERI_CTRL22_ADDR(base) ((base) + (0x00D9))
#define PMIC_PERI_CTRL23_ADDR(base) ((base) + (0x00DA))
#define PMIC_PERI_CTRL24_ADDR(base) ((base) + (0x00DB))
#define PMIC_PERI_CTRL25_ADDR(base) ((base) + (0x00DC))
#define PMIC_PERI_CTRL26_ADDR(base) ((base) + (0x00DD))
#define PMIC_PERI_CTRL27_ADDR(base) ((base) + (0x00DE))
#define PMIC_PERI_CTRL28_ADDR(base) ((base) + (0x00DF))
#define PMIC_PERI_CTRL29_ADDR(base) ((base) + (0x00E0))
#define PMIC_PERI_CTRL30_ADDR(base) ((base) + (0x00E1))
#define PMIC_PERI_CTRL31_ADDR(base) ((base) + (0x00E2))
#define PMIC_PERI_CTRL32_ADDR(base) ((base) + (0x00E3))
#define PMIC_PERI_CTRL33_ADDR(base) ((base) + (0x00E4))
#define PMIC_PERI_CTRL34_ADDR(base) ((base) + (0x00E5))
#define PMIC_PERI_CTRL35_ADDR(base) ((base) + (0x00E6))
#define PMIC_PERI_CTRL36_ADDR(base) ((base) + (0x00E7))
#define PMIC_PERI_CTRL37_ADDR(base) ((base) + (0x00E8))
#define PMIC_PERI_CTRL38_ADDR(base) ((base) + (0x00E9))
#define PMIC_PERI_CTRL39_ADDR(base) ((base) + (0x00EA))
#define PMIC_PERI_CTRL40_ADDR(base) ((base) + (0x00EB))
#define PMIC_PERI_CTRL41_ADDR(base) ((base) + (0x00EC))
#define PMIC_PERI_CTRL42_ADDR(base) ((base) + (0x00ED))
#define PMIC_PERI_CTRL43_ADDR(base) ((base) + (0x00EE))
#define PMIC_PERI_CTRL44_ADDR(base) ((base) + (0x00EF))
#define PMIC_PERI_CTRL45_ADDR(base) ((base) + (0x00F0))
#define PMIC_WIFI_CTRL_ADDR(base) ((base) + (0x00F1))
#define PMIC_PERI_VSET_CTRL_ADDR(base) ((base) + (0x00F2))
#define PMIC_IDLE_CTRL0_ADDR(base) ((base) + (0x00F3))
#define PMIC_IDLE_CTRL1_ADDR(base) ((base) + (0x00F4))
#define PMIC_IDLE_CTRL2_ADDR(base) ((base) + (0x00F5))
#define PMIC_IDLE_CTRL3_ADDR(base) ((base) + (0x00F6))
#define PMIC_IDLE_CTRL4_ADDR(base) ((base) + (0x00F7))
#define PMIC_IDLE_CTRL5_ADDR(base) ((base) + (0x00F8))
#define PMIC_IDLE_CTRL6_ADDR(base) ((base) + (0x00F9))
#define PMIC_IDLE_CTRL7_ADDR(base) ((base) + (0x00FA))
#define PMIC_IDLE_CTRL8_ADDR(base) ((base) + (0x00FB))
#define PMIC_IDLE_CTRL9_ADDR(base) ((base) + (0x00FC))
#define PMIC_IDLE_CNT_CFG_ADDR(base) ((base) + (0x00FD))
#define PMIC_IDLE_CTRL10_ADDR(base) ((base) + (0x00FE))
#define PMIC_IDLE_CTRL11_ADDR(base) ((base) + (0x00FF))
#define PMIC_IDLE_CTRL12_ADDR(base) ((base) + (0x0100))
#define PMIC_IDLE_CTRL13_ADDR(base) ((base) + (0x0101))
#define PMIC_IDLE_CTRL14_ADDR(base) ((base) + (0x0102))
#define PMIC_IDLE_CTRL15_ADDR(base) ((base) + (0x0103))
#define PMIC_IDLE_CTRL16_ADDR(base) ((base) + (0x0104))
#define PMIC_IDLE_CTRL17_ADDR(base) ((base) + (0x0105))
#define PMIC_IDLE_CTRL18_ADDR(base) ((base) + (0x0106))
#define PMIC_IDLE_CTRL19_ADDR(base) ((base) + (0x0107))
#define PMIC_IDLE_CTRL20_ADDR(base) ((base) + (0x0108))
#define PMIC_IDLE_CTRL21_ADDR(base) ((base) + (0x0109))
#define PMIC_IDLE_CTRL22_ADDR(base) ((base) + (0x010A))
#define PMIC_IDLE_CTRL23_ADDR(base) ((base) + (0x010B))
#define PMIC_IDLE_CTRL24_ADDR(base) ((base) + (0x010C))
#define PMIC_IDLE_CTRL25_ADDR(base) ((base) + (0x010D))
#define PMIC_IDLE_CTRL26_ADDR(base) ((base) + (0x010E))
#define PMIC_IDLE_CTRL27_ADDR(base) ((base) + (0x010F))
#define PMIC_IDLE_CTRL28_ADDR(base) ((base) + (0x0110))
#define PMIC_IDLE_CTRL29_ADDR(base) ((base) + (0x0111))
#define PMIC_IDLE_CTRL30_ADDR(base) ((base) + (0x0112))
#define PMIC_IDLE_CTRL31_ADDR(base) ((base) + (0x0113))
#define PMIC_IDLE_CTRL32_ADDR(base) ((base) + (0x0114))
#define PMIC_IDLE_CTRL33_ADDR(base) ((base) + (0x0115))
#define PMIC_IDLE_CTRL34_ADDR(base) ((base) + (0x0116))
#define PMIC_IDLE_CTRL35_ADDR(base) ((base) + (0x0117))
#define PMIC_IDLE_CTRL36_ADDR(base) ((base) + (0x0118))
#define PMIC_IDLE_CTRL37_ADDR(base) ((base) + (0x0119))
#define PMIC_IDLE_CTRL38_ADDR(base) ((base) + (0x011A))
#define PMIC_IDLE_CTRL39_ADDR(base) ((base) + (0x011B))
#define PMIC_IDLE_CTRL40_ADDR(base) ((base) + (0x011C))
#define PMIC_IDLE_CTRL41_ADDR(base) ((base) + (0x011D))
#define PMIC_IDLE_CTRL42_ADDR(base) ((base) + (0x011E))
#define PMIC_IDLE_CTRL43_ADDR(base) ((base) + (0x011F))
#define PMIC_IDLE_CTRL44_ADDR(base) ((base) + (0x0120))
#define PMIC_IDLE_CTRL45_ADDR(base) ((base) + (0x0121))
#define PMIC_SYS_EN_CTRL0_ADDR(base) ((base) + (0x0122))
#define PMIC_SYS_EN_CTRL1_ADDR(base) ((base) + (0x0123))
#define PMIC_SYS_EN_CTRL2_ADDR(base) ((base) + (0x0124))
#define PMIC_SYS_EN_CTRL3_ADDR(base) ((base) + (0x0125))
#define PMIC_SYS_EN_CTRL4_ADDR(base) ((base) + (0x0126))
#define PMIC_SYS_EN_CTRL5_ADDR(base) ((base) + (0x0127))
#define PMIC_SYS_EN_CNT_CFG_ADDR(base) ((base) + (0x0128))
#define PMIC_SYS_EN_CTRL6_ADDR(base) ((base) + (0x0129))
#define PMIC_SYS_EN_CTRL7_ADDR(base) ((base) + (0x012A))
#define PMIC_SYS_EN_CTRL8_ADDR(base) ((base) + (0x012B))
#define PMIC_SYS_EN_CTRL9_ADDR(base) ((base) + (0x012C))
#define PMIC_SYS_EN_CTRL10_ADDR(base) ((base) + (0x012D))
#define PMIC_SYS_EN_CTRL11_ADDR(base) ((base) + (0x012E))
#define PMIC_SYS_EN_CTRL12_ADDR(base) ((base) + (0x012F))
#define PMIC_SYS_EN_CTRL13_ADDR(base) ((base) + (0x0130))
#define PMIC_SYS_EN_CTRL14_ADDR(base) ((base) + (0x0131))
#define PMIC_SYS_EN_CTRL15_ADDR(base) ((base) + (0x0132))
#define PMIC_SYS_EN_CTRL16_ADDR(base) ((base) + (0x0133))
#define PMIC_SYS_EN_CTRL17_ADDR(base) ((base) + (0x0134))
#define PMIC_SYS_EN_CTRL18_ADDR(base) ((base) + (0x0135))
#define PMIC_SYS_EN_CTRL19_ADDR(base) ((base) + (0x0136))
#define PMIC_SYS_EN_CTRL20_ADDR(base) ((base) + (0x0137))
#define PMIC_SYS_EN_CTRL21_ADDR(base) ((base) + (0x0138))
#define PMIC_SYS_EN_CTRL22_ADDR(base) ((base) + (0x0139))
#define PMIC_SYS_EN_CTRL23_ADDR(base) ((base) + (0x013A))
#define PMIC_SYS_EN_CTRL24_ADDR(base) ((base) + (0x013B))
#define PMIC_SYS_EN_CTRL25_ADDR(base) ((base) + (0x013C))
#define PMIC_SYS_EN_CTRL26_ADDR(base) ((base) + (0x013D))
#define PMIC_SYS_EN_VSET_CTRL_ADDR(base) ((base) + (0x013E))
#define PMIC_AO_MODE_ADDR(base) ((base) + (0x013F))
#define PMIC_AO_EUSB_MASK_ADDR(base) ((base) + (0x0140))
#define PMIC_COUL_ECO_MASK_ADDR(base) ((base) + (0x0141))
#define PMIC_DCR_ECO_MASK_ADDR(base) ((base) + (0x0142))
#define PMIC_PMU_SOFT_RST_ADDR(base) ((base) + (0x0143))
#define PMIC_LOCK_ADDR(base) ((base) + (0x0144))
#define PMIC_DR_EN_123_ADDR(base) ((base) + (0x0145))
#define PMIC_DR_EN_MODE_123_ADDR(base) ((base) + (0x0146))
#define PMIC_FLASH_PERIOD_DR1_ADDR(base) ((base) + (0x0147))
#define PMIC_FLASH_ON_DR1_ADDR(base) ((base) + (0x0148))
#define PMIC_FLASH_PERIOD_DR2_ADDR(base) ((base) + (0x0149))
#define PMIC_FLASH_ON_DR2_ADDR(base) ((base) + (0x014A))
#define PMIC_FLASH_PERIOD_DR3_ADDR(base) ((base) + (0x014B))
#define PMIC_FLASH_ON_DR3_ADDR(base) ((base) + (0x014C))
#define PMIC_DR_MODE_SEL_ADDR(base) ((base) + (0x014D))
#define PMIC_DR_BRE_CTRL_ADDR(base) ((base) + (0x014E))
#define PMIC_DR1_ISET_ADDR(base) ((base) + (0x014F))
#define PMIC_DR2_ISET_ADDR(base) ((base) + (0x0150))
#define PMIC_DR3_ISET_ADDR(base) ((base) + (0x0151))
#define PMIC_DR_OUT_CTRL_ADDR(base) ((base) + (0x0152))
#define PMIC_DR1_START_DEL_ADDR(base) ((base) + (0x0153))
#define PMIC_DR2_START_DEL_ADDR(base) ((base) + (0x0154))
#define PMIC_DR3_START_DEL_ADDR(base) ((base) + (0x0155))
#define PMIC_DR1_TIM_CONF0_ADDR(base) ((base) + (0x0156))
#define PMIC_DR1_TIM_CONF1_ADDR(base) ((base) + (0x0157))
#define PMIC_DR2_TIM_CONF0_ADDR(base) ((base) + (0x0158))
#define PMIC_DR2_TIM_CONF1_ADDR(base) ((base) + (0x0159))
#define PMIC_DR3_TIM_CONF0_ADDR(base) ((base) + (0x015A))
#define PMIC_DR3_TIM_CONF1_ADDR(base) ((base) + (0x015B))
#define PMIC_DR_CTRLRESERVE8_ADDR(base) ((base) + (0x015C))
#define PMIC_DR_CTRLRESERVE9_ADDR(base) ((base) + (0x015D))
#define PMIC_OTP0_0_ADDR(base) ((base) + (0x0160))
#define PMIC_OTP0_1_ADDR(base) ((base) + (0x0161))
#define PMIC_OTP0_CTRL0_ADDR(base) ((base) + (0x0162))
#define PMIC_OTP0_CTRL1_ADDR(base) ((base) + (0x0163))
#define PMIC_OTP0_CTRL2_ADDR(base) ((base) + (0x0164))
#define PMIC_OTP0_WDATA_ADDR(base) ((base) + (0x0165))
#define PMIC_OTP0_0_W_ADDR(base) ((base) + (0x0166))
#define PMIC_OTP0_1_W_ADDR(base) ((base) + (0x0167))
#define PMIC_OTP0_2_W_ADDR(base) ((base) + (0x0168))
#define PMIC_OTP0_3_W_ADDR(base) ((base) + (0x0169))
#define PMIC_OTP0_4_W_ADDR(base) ((base) + (0x016A))
#define PMIC_OTP0_5_W_ADDR(base) ((base) + (0x016B))
#define PMIC_OTP0_6_W_ADDR(base) ((base) + (0x016C))
#define PMIC_OTP0_7_W_ADDR(base) ((base) + (0x016D))
#define PMIC_OTP0_8_W_ADDR(base) ((base) + (0x016E))
#define PMIC_OTP0_9_W_ADDR(base) ((base) + (0x016F))
#define PMIC_OTP0_10_W_ADDR(base) ((base) + (0x0170))
#define PMIC_OTP0_11_W_ADDR(base) ((base) + (0x0171))
#define PMIC_OTP0_12_W_ADDR(base) ((base) + (0x0172))
#define PMIC_OTP0_13_W_ADDR(base) ((base) + (0x0173))
#define PMIC_OTP0_14_W_ADDR(base) ((base) + (0x0174))
#define PMIC_OTP0_15_W_ADDR(base) ((base) + (0x0175))
#define PMIC_OTP0_16_W_ADDR(base) ((base) + (0x0176))
#define PMIC_OTP0_17_W_ADDR(base) ((base) + (0x0177))
#define PMIC_OTP0_18_W_ADDR(base) ((base) + (0x0178))
#define PMIC_OTP0_19_W_ADDR(base) ((base) + (0x0179))
#define PMIC_OTP0_20_W_ADDR(base) ((base) + (0x017A))
#define PMIC_OTP0_21_W_ADDR(base) ((base) + (0x017B))
#define PMIC_OTP0_22_W_ADDR(base) ((base) + (0x017C))
#define PMIC_OTP0_23_W_ADDR(base) ((base) + (0x017D))
#define PMIC_OTP0_24_W_ADDR(base) ((base) + (0x017E))
#define PMIC_OTP0_25_W_ADDR(base) ((base) + (0x017F))
#define PMIC_OTP0_26_W_ADDR(base) ((base) + (0x0180))
#define PMIC_OTP0_27_W_ADDR(base) ((base) + (0x0181))
#define PMIC_OTP0_28_W_ADDR(base) ((base) + (0x0182))
#define PMIC_OTP0_29_W_ADDR(base) ((base) + (0x0183))
#define PMIC_OTP0_30_W_ADDR(base) ((base) + (0x0184))
#define PMIC_OTP0_31_W_ADDR(base) ((base) + (0x0185))
#define PMIC_OTP0_32_W_ADDR(base) ((base) + (0x0186))
#define PMIC_OTP0_33_W_ADDR(base) ((base) + (0x0187))
#define PMIC_OTP0_34_W_ADDR(base) ((base) + (0x0188))
#define PMIC_OTP0_35_W_ADDR(base) ((base) + (0x0189))
#define PMIC_OTP0_36_W_ADDR(base) ((base) + (0x018A))
#define PMIC_OTP0_37_W_ADDR(base) ((base) + (0x018B))
#define PMIC_OTP0_38_W_ADDR(base) ((base) + (0x018C))
#define PMIC_OTP0_39_W_ADDR(base) ((base) + (0x018D))
#define PMIC_OTP0_40_W_ADDR(base) ((base) + (0x018E))
#define PMIC_OTP0_41_W_ADDR(base) ((base) + (0x018F))
#define PMIC_OTP0_42_W_ADDR(base) ((base) + (0x0190))
#define PMIC_OTP0_43_W_ADDR(base) ((base) + (0x0191))
#define PMIC_OTP0_44_W_ADDR(base) ((base) + (0x0192))
#define PMIC_OTP0_45_W_ADDR(base) ((base) + (0x0193))
#define PMIC_OTP0_46_W_ADDR(base) ((base) + (0x0194))
#define PMIC_OTP0_47_W_ADDR(base) ((base) + (0x0195))
#define PMIC_OTP0_48_W_ADDR(base) ((base) + (0x0196))
#define PMIC_OTP0_49_W_ADDR(base) ((base) + (0x0197))
#define PMIC_OTP0_50_W_ADDR(base) ((base) + (0x0198))
#define PMIC_OTP0_51_W_ADDR(base) ((base) + (0x0199))
#define PMIC_OTP0_52_W_ADDR(base) ((base) + (0x019A))
#define PMIC_OTP0_53_W_ADDR(base) ((base) + (0x019B))
#define PMIC_OTP0_54_W_ADDR(base) ((base) + (0x019C))
#define PMIC_OTP0_55_W_ADDR(base) ((base) + (0x019D))
#define PMIC_OTP0_56_W_ADDR(base) ((base) + (0x019E))
#define PMIC_OTP0_57_W_ADDR(base) ((base) + (0x019F))
#define PMIC_OTP0_58_W_ADDR(base) ((base) + (0x01A0))
#define PMIC_OTP0_59_W_ADDR(base) ((base) + (0x01A1))
#define PMIC_OTP0_60_W_ADDR(base) ((base) + (0x01A2))
#define PMIC_OTP0_61_W_ADDR(base) ((base) + (0x01A3))
#define PMIC_OTP0_62_W_ADDR(base) ((base) + (0x01A4))
#define PMIC_OTP0_63_W_ADDR(base) ((base) + (0x01A5))
#define PMIC_OTP1_0_ADDR(base) ((base) + (0x01A6))
#define PMIC_OTP1_1_ADDR(base) ((base) + (0x01A7))
#define PMIC_OTP1_CTRL0_ADDR(base) ((base) + (0x01A8))
#define PMIC_OTP1_CTRL1_ADDR(base) ((base) + (0x01A9))
#define PMIC_OTP1_CTRL2_ADDR(base) ((base) + (0x01AA))
#define PMIC_OTP1_WDATA_ADDR(base) ((base) + (0x01AB))
#define PMIC_OTP1_0_W_ADDR(base) ((base) + (0x01AC))
#define PMIC_OTP1_1_W_ADDR(base) ((base) + (0x01AD))
#define PMIC_OTP1_2_W_ADDR(base) ((base) + (0x01AE))
#define PMIC_OTP1_3_W_ADDR(base) ((base) + (0x01AF))
#define PMIC_OTP1_4_W_ADDR(base) ((base) + (0x01B0))
#define PMIC_OTP1_5_W_ADDR(base) ((base) + (0x01B1))
#define PMIC_OTP1_6_W_ADDR(base) ((base) + (0x01B2))
#define PMIC_SIM_CTRL0_ADDR(base) ((base) + (0x01B3))
#define PMIC_SIM_CTRL1_ADDR(base) ((base) + (0x01B4))
#define PMIC_SIM_DEB_CTRL1_ADDR(base) ((base) + (0x01B5))
#define PMIC_SIM_DEB_CTRL2_ADDR(base) ((base) + (0x01B6))
#define PMIC_GPIO0DATA_ADDR(base) ((base) + (0x01B7))
#define PMIC_GPIO0_DATA_RESERVE_ADDR(base) ((base) + (0x01B8))
#define PMIC_GPIO0DIR_ADDR(base) ((base) + (0x01B9))
#define PMIC_GPIO0_DIR_RESERVE_ADDR(base) ((base) + (0x01BA))
#define PMIC_GPIO0IS_ADDR(base) ((base) + (0x01BB))
#define PMIC_GPIO0IBE_ADDR(base) ((base) + (0x01BC))
#define PMIC_GPIO0IEV_ADDR(base) ((base) + (0x01BD))
#define PMIC_GPIO0AFSEL_ADDR(base) ((base) + (0x01BE))
#define PMIC_GPIO0DSSEL_ADDR(base) ((base) + (0x01BF))
#define PMIC_GPIO0PUSEL_ADDR(base) ((base) + (0x01C0))
#define PMIC_GPIO0PDSEL_ADDR(base) ((base) + (0x01C1))
#define PMIC_GPIO0DEBSEL_ADDR(base) ((base) + (0x01C2))
#define PMIC_SIM0PUSEL_ADDR(base) ((base) + (0x01C3))
#define PMIC_SIM0PDSEL_ADDR(base) ((base) + (0x01C4))
#define PMIC_GPIO1DATA_ADDR(base) ((base) + (0x01C5))
#define PMIC_GPIO1_DATA_RESERVE_ADDR(base) ((base) + (0x01C6))
#define PMIC_GPIO1DIR_ADDR(base) ((base) + (0x01C7))
#define PMIC_GPIO1_DIR_RESERVE_ADDR(base) ((base) + (0x01C8))
#define PMIC_GPIO1IS_ADDR(base) ((base) + (0x01C9))
#define PMIC_GPIO1IBE_ADDR(base) ((base) + (0x01CA))
#define PMIC_GPIO1IEV_ADDR(base) ((base) + (0x01CB))
#define PMIC_GPIO1AFSEL_ADDR(base) ((base) + (0x01CC))
#define PMIC_GPIO1DSSEL_ADDR(base) ((base) + (0x01CD))
#define PMIC_GPIO1PUSEL_ADDR(base) ((base) + (0x01CE))
#define PMIC_GPIO1PDSEL_ADDR(base) ((base) + (0x01CF))
#define PMIC_GPIO1DEBSEL_ADDR(base) ((base) + (0x01D0))
#define PMIC_SIM1PUSEL_ADDR(base) ((base) + (0x01D1))
#define PMIC_SIM1PDSEL_ADDR(base) ((base) + (0x01D2))
#define PMIC_SIM1_RST_MODE_DEL_ADDR(base) ((base) + (0x01D3))
#define PMIC_RAMP_BUCK40_CTRL0_ADDR(base) ((base) + (0x01D4))
#define PMIC_RAMP_BUCK40_CTRL1_ADDR(base) ((base) + (0x01D5))
#define PMIC_BUCK40_RAMPUP_STATE_ADDR(base) ((base) + (0x01D6))
#define PMIC_BUCK40_RAMPDOWN_STATE_ADDR(base) ((base) + (0x01D7))
#define PMIC_BUCK40_RAMP_CHG_CTRL0_ADDR(base) ((base) + (0x01D8))
#define PMIC_BUCK40_RAMP_CHG_CTRL1_ADDR(base) ((base) + (0x01D9))
#define PMIC_RAMP_BUCK42_CTRL0_ADDR(base) ((base) + (0x01DA))
#define PMIC_RAMP_BUCK42_CTRL1_ADDR(base) ((base) + (0x01DB))
#define PMIC_BUCK42_RAMPUP_STATE_ADDR(base) ((base) + (0x01DC))
#define PMIC_BUCK42_RAMPDOWN_STATE_ADDR(base) ((base) + (0x01DD))
#define PMIC_BUCK42_RAMP_CHG_CTRL0_ADDR(base) ((base) + (0x01DE))
#define PMIC_BUCK42_RAMP_CHG_CTRL1_ADDR(base) ((base) + (0x01DF))
#define PMIC_RAMP_BUCK5_CTRL0_ADDR(base) ((base) + (0x01E0))
#define PMIC_RAMP_BUCK5_CTRL1_ADDR(base) ((base) + (0x01E1))
#define PMIC_BUCK5_RAMPUP_STATE_ADDR(base) ((base) + (0x01E2))
#define PMIC_BUCK5_RAMPDOWN_STATE_ADDR(base) ((base) + (0x01E3))
#define PMIC_BUCK5_RAMP_CHG_CTRL0_ADDR(base) ((base) + (0x01E4))
#define PMIC_BUCK5_RAMP_CHG_CTRL1_ADDR(base) ((base) + (0x01E5))
#define PMIC_RAMP_BUCK70_CTRL0_ADDR(base) ((base) + (0x01E6))
#define PMIC_RAMP_BUCK70_CTRL1_ADDR(base) ((base) + (0x01E7))
#define PMIC_BUCK70_RAMPUP_STATE_ADDR(base) ((base) + (0x01E8))
#define PMIC_BUCK70_RAMPDOWN_STATE_ADDR(base) ((base) + (0x01E9))
#define PMIC_BUCK70_RAMP_CHG_CTRL0_ADDR(base) ((base) + (0x01EA))
#define PMIC_BUCK70_RAMP_CHG_CTRL1_ADDR(base) ((base) + (0x01EB))
#define PMIC_RAMP_BUCK71_CTRL0_ADDR(base) ((base) + (0x01EC))
#define PMIC_RAMP_BUCK71_CTRL1_ADDR(base) ((base) + (0x01ED))
#define PMIC_BUCK71_RAMPUP_STATE_ADDR(base) ((base) + (0x01EE))
#define PMIC_BUCK71_RAMPDOWN_STATE_ADDR(base) ((base) + (0x01EF))
#define PMIC_BUCK71_RAMP_CHG_CTRL0_ADDR(base) ((base) + (0x01F0))
#define PMIC_BUCK71_RAMP_CHG_CTRL1_ADDR(base) ((base) + (0x01F1))
#define PMIC_RAMP_LDO0_CTRL0_ADDR(base) ((base) + (0x01F2))
#define PMIC_RAMP_LDO0_CTRL1_ADDR(base) ((base) + (0x01F3))
#define PMIC_RAMP_LDO44_CTRL0_ADDR(base) ((base) + (0x01F4))
#define PMIC_RAMP_LDO44_CTRL1_ADDR(base) ((base) + (0x01F5))
#define PMIC_RAMP_LDO45_CTRL0_ADDR(base) ((base) + (0x01F6))
#define PMIC_RAMP_LDO45_CTRL1_ADDR(base) ((base) + (0x01F7))
#define PMIC_SPMI_ECO_GT_BYPASS_ADDR(base) ((base) + (0x01F8))
#define PMIC_PWRONN_8S_XOADC_DEBUG_CTRL_ADDR(base) ((base) + (0x01F9))
#define PMIC_DIS_19M2_CTRL_ADDR(base) ((base) + (0x01FA))
#define PMIC_FAULT_S_N_CTRL_ADDR(base) ((base) + (0x01FB))
#define PMIC_PWRONN_CNT_ADDR(base) ((base) + (0x01FC))
#define PMIC_PWRONN_CNT_MASK_ADDR(base) ((base) + (0x01FD))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_IRQ_MASK_0_ADDR(base) ((base) + (0x0290UL))
#define PMIC_IRQ_MASK_1_ADDR(base) ((base) + (0x0291UL))
#define PMIC_IRQ_MASK_2_ADDR(base) ((base) + (0x0292UL))
#define PMIC_IRQ_MASK_3_ADDR(base) ((base) + (0x0293UL))
#define PMIC_IRQ_MASK_4_ADDR(base) ((base) + (0x0294UL))
#define PMIC_IRQ_MASK_5_ADDR(base) ((base) + (0x0295UL))
#define PMIC_IRQ_MASK_6_ADDR(base) ((base) + (0x0296UL))
#define PMIC_IRQ_MASK_7_ADDR(base) ((base) + (0x0297UL))
#define PMIC_IRQ_MASK_8_ADDR(base) ((base) + (0x0298UL))
#define PMIC_IRQ_MASK_9_ADDR(base) ((base) + (0x0299UL))
#define PMIC_IRQ_MASK_10_ADDR(base) ((base) + (0x029AUL))
#define PMIC_IRQ_MASK_11_ADDR(base) ((base) + (0x029BUL))
#define PMIC_IRQ_MASK_12_ADDR(base) ((base) + (0x029CUL))
#define PMIC_IRQ_MASK_13_ADDR(base) ((base) + (0x029DUL))
#define PMIC_IRQ_MASK_14_ADDR(base) ((base) + (0x029EUL))
#define PMIC_IRQ_MASK_15_ADDR(base) ((base) + (0x029FUL))
#define PMIC_IRQ_MASK_16_ADDR(base) ((base) + (0x02A0UL))
#define PMIC_IRQ_MASK_17_ADDR(base) ((base) + (0x02A1UL))
#define PMIC_IRQ_MASK_18_ADDR(base) ((base) + (0x02A2UL))
#define PMIC_IRQ_MASK_19_ADDR(base) ((base) + (0x02A3UL))
#define PMIC_IRQ_MASK_20_ADDR(base) ((base) + (0x02A4UL))
#else
#define PMIC_IRQ_MASK_0_ADDR(base) ((base) + (0x0290))
#define PMIC_IRQ_MASK_1_ADDR(base) ((base) + (0x0291))
#define PMIC_IRQ_MASK_2_ADDR(base) ((base) + (0x0292))
#define PMIC_IRQ_MASK_3_ADDR(base) ((base) + (0x0293))
#define PMIC_IRQ_MASK_4_ADDR(base) ((base) + (0x0294))
#define PMIC_IRQ_MASK_5_ADDR(base) ((base) + (0x0295))
#define PMIC_IRQ_MASK_6_ADDR(base) ((base) + (0x0296))
#define PMIC_IRQ_MASK_7_ADDR(base) ((base) + (0x0297))
#define PMIC_IRQ_MASK_8_ADDR(base) ((base) + (0x0298))
#define PMIC_IRQ_MASK_9_ADDR(base) ((base) + (0x0299))
#define PMIC_IRQ_MASK_10_ADDR(base) ((base) + (0x029A))
#define PMIC_IRQ_MASK_11_ADDR(base) ((base) + (0x029B))
#define PMIC_IRQ_MASK_12_ADDR(base) ((base) + (0x029C))
#define PMIC_IRQ_MASK_13_ADDR(base) ((base) + (0x029D))
#define PMIC_IRQ_MASK_14_ADDR(base) ((base) + (0x029E))
#define PMIC_IRQ_MASK_15_ADDR(base) ((base) + (0x029F))
#define PMIC_IRQ_MASK_16_ADDR(base) ((base) + (0x02A0))
#define PMIC_IRQ_MASK_17_ADDR(base) ((base) + (0x02A1))
#define PMIC_IRQ_MASK_18_ADDR(base) ((base) + (0x02A2))
#define PMIC_IRQ_MASK_19_ADDR(base) ((base) + (0x02A3))
#define PMIC_IRQ_MASK_20_ADDR(base) ((base) + (0x02A4))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_IRQ0_ADDR(base) ((base) + (0x02B3UL))
#define PMIC_IRQ1_ADDR(base) ((base) + (0x02B4UL))
#define PMIC_OCP_IRQ0_ADDR(base) ((base) + (0x02B5UL))
#define PMIC_OCP_IRQ1_ADDR(base) ((base) + (0x02B6UL))
#define PMIC_OCP_IRQ2_ADDR(base) ((base) + (0x02B7UL))
#define PMIC_OCP_IRQ3_ADDR(base) ((base) + (0x02B8UL))
#define PMIC_OCP_IRQ4_ADDR(base) ((base) + (0x02B9UL))
#define PMIC_OCP_IRQ5_ADDR(base) ((base) + (0x02BAUL))
#define PMIC_OCP_IRQ6_ADDR(base) ((base) + (0x02BBUL))
#define PMIC_SCP_IRQ0_ADDR(base) ((base) + (0x02BCUL))
#define PMIC_SCP_IRQ1_ADDR(base) ((base) + (0x02BDUL))
#define PMIC_OVP_IRQ0_ADDR(base) ((base) + (0x02BEUL))
#define PMIC_CUR_DET_IRQ0_ADDR(base) ((base) + (0x02BFUL))
#define PMIC_GPIO_IRQ_ADDR(base) ((base) + (0x02C0UL))
#define PMIC_VIOCE_RESTART_IRQ_ADDR(base) ((base) + (0x02C1UL))
#define PMIC_ACR_SOH_IRQ_ADDR(base) ((base) + (0x02C2UL))
#define PMIC_DCR_IRQ_ADDR(base) ((base) + (0x02C3UL))
#define PMIC_EIS_IRQ_ADDR(base) ((base) + (0x02C4UL))
#define PMIC_RAMP_IRQ0_ADDR(base) ((base) + (0x02C5UL))
#define PMIC_RAMP_IRQ1_ADDR(base) ((base) + (0x02C6UL))
#define PMIC_VSYS_DROP_IRQ_ADDR(base) ((base) + (0x02C7UL))
#else
#define PMIC_IRQ0_ADDR(base) ((base) + (0x02B3))
#define PMIC_IRQ1_ADDR(base) ((base) + (0x02B4))
#define PMIC_OCP_IRQ0_ADDR(base) ((base) + (0x02B5))
#define PMIC_OCP_IRQ1_ADDR(base) ((base) + (0x02B6))
#define PMIC_OCP_IRQ2_ADDR(base) ((base) + (0x02B7))
#define PMIC_OCP_IRQ3_ADDR(base) ((base) + (0x02B8))
#define PMIC_OCP_IRQ4_ADDR(base) ((base) + (0x02B9))
#define PMIC_OCP_IRQ5_ADDR(base) ((base) + (0x02BA))
#define PMIC_OCP_IRQ6_ADDR(base) ((base) + (0x02BB))
#define PMIC_SCP_IRQ0_ADDR(base) ((base) + (0x02BC))
#define PMIC_SCP_IRQ1_ADDR(base) ((base) + (0x02BD))
#define PMIC_OVP_IRQ0_ADDR(base) ((base) + (0x02BE))
#define PMIC_CUR_DET_IRQ0_ADDR(base) ((base) + (0x02BF))
#define PMIC_GPIO_IRQ_ADDR(base) ((base) + (0x02C0))
#define PMIC_VIOCE_RESTART_IRQ_ADDR(base) ((base) + (0x02C1))
#define PMIC_ACR_SOH_IRQ_ADDR(base) ((base) + (0x02C2))
#define PMIC_DCR_IRQ_ADDR(base) ((base) + (0x02C3))
#define PMIC_EIS_IRQ_ADDR(base) ((base) + (0x02C4))
#define PMIC_RAMP_IRQ0_ADDR(base) ((base) + (0x02C5))
#define PMIC_RAMP_IRQ1_ADDR(base) ((base) + (0x02C6))
#define PMIC_VSYS_DROP_IRQ_ADDR(base) ((base) + (0x02C7))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_NP_RECORD0_ADDR(base) ((base) + (0x02CCUL))
#define PMIC_NP_RECORD1_ADDR(base) ((base) + (0x02CDUL))
#define PMIC_NP_RECORD2_ADDR(base) ((base) + (0x02CEUL))
#define PMIC_NP_RECORD3_ADDR(base) ((base) + (0x02CFUL))
#define PMIC_NP_RECORD4_ADDR(base) ((base) + (0x02D0UL))
#define PMIC_NP_RECORD5_ADDR(base) ((base) + (0x02D1UL))
#define PMIC_NP_OCP0_ADDR(base) ((base) + (0x02D2UL))
#define PMIC_NP_OCP1_ADDR(base) ((base) + (0x02D3UL))
#define PMIC_NP_OCP2_ADDR(base) ((base) + (0x02D4UL))
#define PMIC_NP_OCP3_ADDR(base) ((base) + (0x02D5UL))
#define PMIC_NP_OCP4_ADDR(base) ((base) + (0x02D6UL))
#define PMIC_NP_OCP5_ADDR(base) ((base) + (0x02D7UL))
#define PMIC_NP_OCP6_ADDR(base) ((base) + (0x02D8UL))
#define PMIC_NP_SCP0_ADDR(base) ((base) + (0x02D9UL))
#define PMIC_NP_SCP1_ADDR(base) ((base) + (0x02DAUL))
#define PMIC_NP_OVP0_ADDR(base) ((base) + (0x02DBUL))
#define PMIC_NP_CUR_DET0_ADDR(base) ((base) + (0x02DCUL))
#define PMIC_RAMP_EVENT0_ADDR(base) ((base) + (0x02DDUL))
#define PMIC_RAMP_EVENT1_ADDR(base) ((base) + (0x02DEUL))
#define PMIC_RAMP_EVENT2_ADDR(base) ((base) + (0x02DFUL))
#define PMIC_RAMP_EVENT3_ADDR(base) ((base) + (0x02E0UL))
#define PMIC_SOH_RECORD_ADDR(base) ((base) + (0x02E1UL))
#define PMIC_ACR_SOH_RECORD_ADDR(base) ((base) + (0x02E2UL))
#define PMIC_COUL_RECORD0_ADDR(base) ((base) + (0x02E3UL))
#define PMIC_COUL_RECORD1_ADDR(base) ((base) + (0x02E4UL))
#define PMIC_NP_FAULT_RECORD_ADDR(base) ((base) + (0x02E5UL))
#define PMIC_NP_VSYS_SURGE_RECORD_ADDR(base) ((base) + (0x02E6UL))
#define PMIC_NP_VSYS_DROP_RECORD_ADDR(base) ((base) + (0x02E7UL))
#else
#define PMIC_NP_RECORD0_ADDR(base) ((base) + (0x02CC))
#define PMIC_NP_RECORD1_ADDR(base) ((base) + (0x02CD))
#define PMIC_NP_RECORD2_ADDR(base) ((base) + (0x02CE))
#define PMIC_NP_RECORD3_ADDR(base) ((base) + (0x02CF))
#define PMIC_NP_RECORD4_ADDR(base) ((base) + (0x02D0))
#define PMIC_NP_RECORD5_ADDR(base) ((base) + (0x02D1))
#define PMIC_NP_OCP0_ADDR(base) ((base) + (0x02D2))
#define PMIC_NP_OCP1_ADDR(base) ((base) + (0x02D3))
#define PMIC_NP_OCP2_ADDR(base) ((base) + (0x02D4))
#define PMIC_NP_OCP3_ADDR(base) ((base) + (0x02D5))
#define PMIC_NP_OCP4_ADDR(base) ((base) + (0x02D6))
#define PMIC_NP_OCP5_ADDR(base) ((base) + (0x02D7))
#define PMIC_NP_OCP6_ADDR(base) ((base) + (0x02D8))
#define PMIC_NP_SCP0_ADDR(base) ((base) + (0x02D9))
#define PMIC_NP_SCP1_ADDR(base) ((base) + (0x02DA))
#define PMIC_NP_OVP0_ADDR(base) ((base) + (0x02DB))
#define PMIC_NP_CUR_DET0_ADDR(base) ((base) + (0x02DC))
#define PMIC_RAMP_EVENT0_ADDR(base) ((base) + (0x02DD))
#define PMIC_RAMP_EVENT1_ADDR(base) ((base) + (0x02DE))
#define PMIC_RAMP_EVENT2_ADDR(base) ((base) + (0x02DF))
#define PMIC_RAMP_EVENT3_ADDR(base) ((base) + (0x02E0))
#define PMIC_SOH_RECORD_ADDR(base) ((base) + (0x02E1))
#define PMIC_ACR_SOH_RECORD_ADDR(base) ((base) + (0x02E2))
#define PMIC_COUL_RECORD0_ADDR(base) ((base) + (0x02E3))
#define PMIC_COUL_RECORD1_ADDR(base) ((base) + (0x02E4))
#define PMIC_NP_FAULT_RECORD_ADDR(base) ((base) + (0x02E5))
#define PMIC_NP_VSYS_SURGE_RECORD_ADDR(base) ((base) + (0x02E6))
#define PMIC_NP_VSYS_DROP_RECORD_ADDR(base) ((base) + (0x02E7))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_PWRUP_CALI_END_ADDR(base) ((base) + (0x03A0UL))
#define PMIC_XOADC_AUTOCALI_AVE0_ADDR(base) ((base) + (0x03A1UL))
#define PMIC_XOADC_AUTOCALI_AVE1_ADDR(base) ((base) + (0x03A2UL))
#define PMIC_XOADC_AUTOCALI_AVE2_ADDR(base) ((base) + (0x03A3UL))
#define PMIC_XOADC_AUTOCALI_AVE3_ADDR(base) ((base) + (0x03A4UL))
#define PMIC_XOADC_CTRL_ADDR(base) ((base) + (0x03A5UL))
#define PMIC_XOADC_SAMP_PHASE_ADDR(base) ((base) + (0x03A6UL))
#define PMIC_XOADC_OPT_0_ADDR(base) ((base) + (0x03A7UL))
#define PMIC_XOADC_OPT_1_ADDR(base) ((base) + (0x03A8UL))
#define PMIC_XOADC_AIN_SEL_ADDR(base) ((base) + (0x03A9UL))
#define PMIC_XOADC_WIFI_ANA_EN_ADDR(base) ((base) + (0x03AAUL))
#define PMIC_XOADC_SOC_ANA_EN_ADDR(base) ((base) + (0x03ABUL))
#define PMIC_XOADC_STATE_ADDR(base) ((base) + (0x03ACUL))
#define PMIC_XOADC_DATA0_ADDR(base) ((base) + (0x03ADUL))
#define PMIC_XOADC_DATA1_ADDR(base) ((base) + (0x03AEUL))
#define PMIC_XOADC_CALI_DATA0_ADDR(base) ((base) + (0x03AFUL))
#define PMIC_XOADC_CALI_DATA1_ADDR(base) ((base) + (0x03B0UL))
#define PMIC_XOADC_CFG_EN_ADDR(base) ((base) + (0x03B1UL))
#define PMIC_XOADC_ARB_DEBUG_ADDR(base) ((base) + (0x03B2UL))
#define PMIC_XOADC_CTRL_S_ADDR(base) ((base) + (0x03B3UL))
#define PMIC_XOADC_SAMP_PHASE_S_ADDR(base) ((base) + (0x03B4UL))
#define PMIC_XOADC_OPT_0_S_ADDR(base) ((base) + (0x03B5UL))
#define PMIC_XOADC_OPT_1_S_ADDR(base) ((base) + (0x03B6UL))
#define PMIC_XOADC_AIN_SEL_S_ADDR(base) ((base) + (0x03B7UL))
#define PMIC_XOADC_ANA_EN_S_ADDR(base) ((base) + (0x03B8UL))
#define PMIC_XOADC_SOFT_CFG0_ADDR(base) ((base) + (0x03B9UL))
#define PMIC_XOADC_SOFT_CFG1_ADDR(base) ((base) + (0x03BAUL))
#define PMIC_XOADC_SOFT_CFG2_ADDR(base) ((base) + (0x03BBUL))
#define PMIC_XOADC_SOFT_CFG3_ADDR(base) ((base) + (0x03BCUL))
#define PMIC_XOADC_SOFT_CFG4_ADDR(base) ((base) + (0x03BDUL))
#define PMIC_XOADC_SOFT_CFG5_ADDR(base) ((base) + (0x03BEUL))
#define PMIC_XOADC_SOFT_CFG6_ADDR(base) ((base) + (0x03BFUL))
#define PMIC_XOADC_SOFT_CFG7_ADDR(base) ((base) + (0x03C0UL))
#define PMIC_XOADC_RESERVE_ADDR(base) ((base) + (0x03C1UL))
#define PMIC_HI1103_RDATA_OUT0_ADDR(base) ((base) + (0x03C2UL))
#define PMIC_HI1103_RDATA_OUT1_ADDR(base) ((base) + (0x03C3UL))
#define PMIC_HI1103_RDATA_OUT2_ADDR(base) ((base) + (0x03C4UL))
#define PMIC_HI1103_RDATA_OUT3_ADDR(base) ((base) + (0x03C5UL))
#define PMIC_HI1103_RDATA_OUT4_ADDR(base) ((base) + (0x03C6UL))
#define PMIC_HI1103_RDATA_OUT5_ADDR(base) ((base) + (0x03C7UL))
#define PMIC_HI1103_RDATA_OUT6_ADDR(base) ((base) + (0x03C8UL))
#define PMIC_HI1103_RDATA_OUT7_ADDR(base) ((base) + (0x03C9UL))
#define PMIC_RTC_LOAD_FLAG_ADDR(base) ((base) + (0x03CAUL))
#define PMIC_HI1103_REFRESH_LOCK_ADDR(base) ((base) + (0x03CBUL))
#define PMIC_SPMI_DEBUG0_ADDR(base) ((base) + (0x03CEUL))
#define PMIC_SPMI_DEBUG1_ADDR(base) ((base) + (0x03CFUL))
#define PMIC_SPMI_DEBUG2_ADDR(base) ((base) + (0x03D0UL))
#define PMIC_SPMI_DEBUG3_ADDR(base) ((base) + (0x03D1UL))
#define PMIC_SPMI_DEBUG4_ADDR(base) ((base) + (0x03D2UL))
#define PMIC_SPMI_DEBUG5_ADDR(base) ((base) + (0x03D3UL))
#define PMIC_SPMI_DEBUG6_ADDR(base) ((base) + (0x03D4UL))
#define PMIC_SPMI_DEBUG7_ADDR(base) ((base) + (0x03D5UL))
#define PMIC_DIG_IO_DS_CFG0_ADDR(base) ((base) + (0x03DAUL))
#define PMIC_DIG_IO_DS_CFG1_ADDR(base) ((base) + (0x03DBUL))
#define PMIC_DIG_IO_DS_CFG2_ADDR(base) ((base) + (0x03DCUL))
#define PMIC_DUMMY_XOADC_SSI_ADDR(base) ((base) + (0x03FFUL))
#else
#define PMIC_PWRUP_CALI_END_ADDR(base) ((base) + (0x03A0))
#define PMIC_XOADC_AUTOCALI_AVE0_ADDR(base) ((base) + (0x03A1))
#define PMIC_XOADC_AUTOCALI_AVE1_ADDR(base) ((base) + (0x03A2))
#define PMIC_XOADC_AUTOCALI_AVE2_ADDR(base) ((base) + (0x03A3))
#define PMIC_XOADC_AUTOCALI_AVE3_ADDR(base) ((base) + (0x03A4))
#define PMIC_XOADC_CTRL_ADDR(base) ((base) + (0x03A5))
#define PMIC_XOADC_SAMP_PHASE_ADDR(base) ((base) + (0x03A6))
#define PMIC_XOADC_OPT_0_ADDR(base) ((base) + (0x03A7))
#define PMIC_XOADC_OPT_1_ADDR(base) ((base) + (0x03A8))
#define PMIC_XOADC_AIN_SEL_ADDR(base) ((base) + (0x03A9))
#define PMIC_XOADC_WIFI_ANA_EN_ADDR(base) ((base) + (0x03AA))
#define PMIC_XOADC_SOC_ANA_EN_ADDR(base) ((base) + (0x03AB))
#define PMIC_XOADC_STATE_ADDR(base) ((base) + (0x03AC))
#define PMIC_XOADC_DATA0_ADDR(base) ((base) + (0x03AD))
#define PMIC_XOADC_DATA1_ADDR(base) ((base) + (0x03AE))
#define PMIC_XOADC_CALI_DATA0_ADDR(base) ((base) + (0x03AF))
#define PMIC_XOADC_CALI_DATA1_ADDR(base) ((base) + (0x03B0))
#define PMIC_XOADC_CFG_EN_ADDR(base) ((base) + (0x03B1))
#define PMIC_XOADC_ARB_DEBUG_ADDR(base) ((base) + (0x03B2))
#define PMIC_XOADC_CTRL_S_ADDR(base) ((base) + (0x03B3))
#define PMIC_XOADC_SAMP_PHASE_S_ADDR(base) ((base) + (0x03B4))
#define PMIC_XOADC_OPT_0_S_ADDR(base) ((base) + (0x03B5))
#define PMIC_XOADC_OPT_1_S_ADDR(base) ((base) + (0x03B6))
#define PMIC_XOADC_AIN_SEL_S_ADDR(base) ((base) + (0x03B7))
#define PMIC_XOADC_ANA_EN_S_ADDR(base) ((base) + (0x03B8))
#define PMIC_XOADC_SOFT_CFG0_ADDR(base) ((base) + (0x03B9))
#define PMIC_XOADC_SOFT_CFG1_ADDR(base) ((base) + (0x03BA))
#define PMIC_XOADC_SOFT_CFG2_ADDR(base) ((base) + (0x03BB))
#define PMIC_XOADC_SOFT_CFG3_ADDR(base) ((base) + (0x03BC))
#define PMIC_XOADC_SOFT_CFG4_ADDR(base) ((base) + (0x03BD))
#define PMIC_XOADC_SOFT_CFG5_ADDR(base) ((base) + (0x03BE))
#define PMIC_XOADC_SOFT_CFG6_ADDR(base) ((base) + (0x03BF))
#define PMIC_XOADC_SOFT_CFG7_ADDR(base) ((base) + (0x03C0))
#define PMIC_XOADC_RESERVE_ADDR(base) ((base) + (0x03C1))
#define PMIC_HI1103_RDATA_OUT0_ADDR(base) ((base) + (0x03C2))
#define PMIC_HI1103_RDATA_OUT1_ADDR(base) ((base) + (0x03C3))
#define PMIC_HI1103_RDATA_OUT2_ADDR(base) ((base) + (0x03C4))
#define PMIC_HI1103_RDATA_OUT3_ADDR(base) ((base) + (0x03C5))
#define PMIC_HI1103_RDATA_OUT4_ADDR(base) ((base) + (0x03C6))
#define PMIC_HI1103_RDATA_OUT5_ADDR(base) ((base) + (0x03C7))
#define PMIC_HI1103_RDATA_OUT6_ADDR(base) ((base) + (0x03C8))
#define PMIC_HI1103_RDATA_OUT7_ADDR(base) ((base) + (0x03C9))
#define PMIC_RTC_LOAD_FLAG_ADDR(base) ((base) + (0x03CA))
#define PMIC_HI1103_REFRESH_LOCK_ADDR(base) ((base) + (0x03CB))
#define PMIC_SPMI_DEBUG0_ADDR(base) ((base) + (0x03CE))
#define PMIC_SPMI_DEBUG1_ADDR(base) ((base) + (0x03CF))
#define PMIC_SPMI_DEBUG2_ADDR(base) ((base) + (0x03D0))
#define PMIC_SPMI_DEBUG3_ADDR(base) ((base) + (0x03D1))
#define PMIC_SPMI_DEBUG4_ADDR(base) ((base) + (0x03D2))
#define PMIC_SPMI_DEBUG5_ADDR(base) ((base) + (0x03D3))
#define PMIC_SPMI_DEBUG6_ADDR(base) ((base) + (0x03D4))
#define PMIC_SPMI_DEBUG7_ADDR(base) ((base) + (0x03D5))
#define PMIC_DIG_IO_DS_CFG0_ADDR(base) ((base) + (0x03DA))
#define PMIC_DIG_IO_DS_CFG1_ADDR(base) ((base) + (0x03DB))
#define PMIC_DIG_IO_DS_CFG2_ADDR(base) ((base) + (0x03DC))
#define PMIC_DUMMY_XOADC_SSI_ADDR(base) ((base) + (0x03FF))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_NP_SYS_CTRL0_ADDR(base) ((base) + (0x02E9UL))
#define PMIC_NP_SYS_CTRL1_ADDR(base) ((base) + (0x02EAUL))
#define PMIC_NP_CLK_WIFI_CTRL_ADDR(base) ((base) + (0x0400UL))
#define PMIC_NP_CLK_NFC_CTRL_ADDR(base) ((base) + (0x0401UL))
#define PMIC_NP_CLK_RF0_CTRL_ADDR(base) ((base) + (0x0402UL))
#define PMIC_NP_CLK_RF1_CTRL_ADDR(base) ((base) + (0x0403UL))
#define PMIC_NP_CLK_CODEC_CTRL_ADDR(base) ((base) + (0x0404UL))
#define PMIC_NP_CLK_TOP_CTRL0_ADDR(base) ((base) + (0x0405UL))
#define PMIC_NP_CLK_TOP_CTRL1_ADDR(base) ((base) + (0x0406UL))
#define PMIC_NP_CLK_256K_CTRL0_ADDR(base) ((base) + (0x0407UL))
#define PMIC_NP_CLK_256K_CTRL1_ADDR(base) ((base) + (0x0408UL))
#define PMIC_NP_CLK_UFS_FRE_CTRL_ADDR(base) ((base) + (0x0409UL))
#define PMIC_NP_VSYS_LOW_SET_ADDR(base) ((base) + (0x040AUL))
#define PMIC_NP_VSYS_DROP_SET_ADDR(base) ((base) + (0x040BUL))
#define PMIC_NP_NFC_ON_CTRL_ADDR(base) ((base) + (0x040CUL))
#define PMIC_NP_HRESET_PWRDOWN_CTRL_ADDR(base) ((base) + (0x040DUL))
#define PMIC_NP_SMPL_CTRL_ADDR(base) ((base) + (0x040EUL))
#define PMIC_NP_DEBUG_LOCK_ADDR(base) ((base) + (0x040FUL))
#define PMIC_NP_SYS_DEBUG0_ADDR(base) ((base) + (0x0410UL))
#define PMIC_NP_SYS_DEBUG1_ADDR(base) ((base) + (0x0411UL))
#define PMIC_NP_SYS_DEBUG2_ADDR(base) ((base) + (0x0412UL))
#define PMIC_NP_SYS_DEBUG3_ADDR(base) ((base) + (0x0413UL))
#define PMIC_NP_SYS_DEBUG4_ADDR(base) ((base) + (0x0414UL))
#define PMIC_NP_SYS_DEBUG5_ADDR(base) ((base) + (0x0415UL))
#define PMIC_NP_BACKUP_CHG_ADDR(base) ((base) + (0x0416UL))
#define PMIC_NP_CTRL_ADDR(base) ((base) + (0x0417UL))
#define PMIC_NP_SHIELD_RES_ADDR(base) ((base) + (0x0418UL))
#define PMIC_NP_D2A_RES0_ADDR(base) ((base) + (0x0419UL))
#define PMIC_NP_D2A_RES1_ADDR(base) ((base) + (0x041AUL))
#define PMIC_HRST_REG0_ADDR(base) ((base) + (0x041BUL))
#define PMIC_HRST_REG1_ADDR(base) ((base) + (0x041CUL))
#define PMIC_HRST_REG2_ADDR(base) ((base) + (0x041DUL))
#define PMIC_HRST_REG3_ADDR(base) ((base) + (0x041EUL))
#define PMIC_HRST_REG4_ADDR(base) ((base) + (0x041FUL))
#define PMIC_HRST_REG5_ADDR(base) ((base) + (0x0420UL))
#define PMIC_HRST_REG6_ADDR(base) ((base) + (0x0421UL))
#define PMIC_HRST_REG7_ADDR(base) ((base) + (0x0422UL))
#define PMIC_HRST_REG8_ADDR(base) ((base) + (0x0423UL))
#define PMIC_HRST_REG9_ADDR(base) ((base) + (0x0424UL))
#define PMIC_HRST_REG10_ADDR(base) ((base) + (0x0425UL))
#define PMIC_HRST_REG11_ADDR(base) ((base) + (0x0426UL))
#define PMIC_HRST_REG12_ADDR(base) ((base) + (0x0427UL))
#define PMIC_HRST_REG13_ADDR(base) ((base) + (0x0428UL))
#define PMIC_HRST_REG14_ADDR(base) ((base) + (0x0429UL))
#define PMIC_HRST_REG15_ADDR(base) ((base) + (0x042AUL))
#define PMIC_HRST_REG16_ADDR(base) ((base) + (0x042BUL))
#define PMIC_HRST_REG17_ADDR(base) ((base) + (0x042CUL))
#define PMIC_HRST_REG18_ADDR(base) ((base) + (0x042DUL))
#define PMIC_HRST_REG19_ADDR(base) ((base) + (0x042EUL))
#define PMIC_HRST_REG20_ADDR(base) ((base) + (0x042FUL))
#define PMIC_HRST_REG21_ADDR(base) ((base) + (0x0430UL))
#define PMIC_HRST_REG22_ADDR(base) ((base) + (0x0431UL))
#define PMIC_HRST_REG23_ADDR(base) ((base) + (0x0432UL))
#define PMIC_HRST_REG24_ADDR(base) ((base) + (0x0433UL))
#define PMIC_HRST_REG25_ADDR(base) ((base) + (0x0434UL))
#define PMIC_HRST_REG26_ADDR(base) ((base) + (0x0435UL))
#define PMIC_HRST_REG27_ADDR(base) ((base) + (0x0436UL))
#define PMIC_HRST_REG28_ADDR(base) ((base) + (0x0437UL))
#define PMIC_HRST_REG29_ADDR(base) ((base) + (0x0438UL))
#define PMIC_HRST_REG30_ADDR(base) ((base) + (0x0439UL))
#define PMIC_HRST_REG31_ADDR(base) ((base) + (0x043AUL))
#define PMIC_NP_OTP0_0_R_ADDR(base) ((base) + (0x043BUL))
#define PMIC_NP_OTP0_1_R_ADDR(base) ((base) + (0x043CUL))
#define PMIC_NP_OTP0_2_R_ADDR(base) ((base) + (0x043DUL))
#define PMIC_NP_OTP0_3_R_ADDR(base) ((base) + (0x043EUL))
#define PMIC_NP_OTP0_4_R_ADDR(base) ((base) + (0x043FUL))
#define PMIC_NP_OTP0_5_R_ADDR(base) ((base) + (0x0440UL))
#define PMIC_NP_OTP0_6_R_ADDR(base) ((base) + (0x0441UL))
#define PMIC_NP_OTP0_7_R_ADDR(base) ((base) + (0x0442UL))
#define PMIC_NP_OTP0_8_R_ADDR(base) ((base) + (0x0443UL))
#define PMIC_NP_OTP0_9_R_ADDR(base) ((base) + (0x0444UL))
#define PMIC_NP_OTP0_10_R_ADDR(base) ((base) + (0x0445UL))
#define PMIC_NP_OTP0_11_R_ADDR(base) ((base) + (0x0446UL))
#define PMIC_NP_OTP0_12_R_ADDR(base) ((base) + (0x0447UL))
#define PMIC_NP_OTP0_13_R_ADDR(base) ((base) + (0x0448UL))
#define PMIC_NP_OTP0_14_R_ADDR(base) ((base) + (0x0449UL))
#define PMIC_NP_OTP0_15_R_ADDR(base) ((base) + (0x044AUL))
#define PMIC_NP_OTP0_16_R_ADDR(base) ((base) + (0x044BUL))
#define PMIC_NP_OTP0_17_R_ADDR(base) ((base) + (0x044CUL))
#define PMIC_NP_OTP0_18_R_ADDR(base) ((base) + (0x044DUL))
#define PMIC_NP_OTP0_19_R_ADDR(base) ((base) + (0x044EUL))
#define PMIC_NP_OTP0_20_R_ADDR(base) ((base) + (0x044FUL))
#define PMIC_NP_OTP0_21_R_ADDR(base) ((base) + (0x0450UL))
#define PMIC_NP_OTP0_22_R_ADDR(base) ((base) + (0x0451UL))
#define PMIC_NP_OTP0_23_R_ADDR(base) ((base) + (0x0452UL))
#define PMIC_NP_OTP0_24_R_ADDR(base) ((base) + (0x0453UL))
#define PMIC_NP_OTP0_25_R_ADDR(base) ((base) + (0x0454UL))
#define PMIC_NP_OTP0_26_R_ADDR(base) ((base) + (0x0455UL))
#define PMIC_NP_OTP0_27_R_ADDR(base) ((base) + (0x0456UL))
#define PMIC_NP_OTP0_28_R_ADDR(base) ((base) + (0x0457UL))
#define PMIC_NP_OTP0_29_R_ADDR(base) ((base) + (0x0458UL))
#define PMIC_NP_OTP0_30_R_ADDR(base) ((base) + (0x0459UL))
#define PMIC_NP_OTP0_31_R_ADDR(base) ((base) + (0x045AUL))
#define PMIC_NP_OTP0_32_R_ADDR(base) ((base) + (0x045BUL))
#define PMIC_NP_OTP0_33_R_ADDR(base) ((base) + (0x045CUL))
#define PMIC_NP_OTP0_34_R_ADDR(base) ((base) + (0x045DUL))
#define PMIC_NP_OTP0_35_R_ADDR(base) ((base) + (0x045EUL))
#define PMIC_NP_OTP0_36_R_ADDR(base) ((base) + (0x045FUL))
#define PMIC_NP_OTP0_37_R_ADDR(base) ((base) + (0x0460UL))
#define PMIC_NP_OTP0_38_R_ADDR(base) ((base) + (0x0461UL))
#define PMIC_NP_OTP0_39_R_ADDR(base) ((base) + (0x0462UL))
#define PMIC_NP_OTP0_40_R_ADDR(base) ((base) + (0x0463UL))
#define PMIC_NP_OTP0_41_R_ADDR(base) ((base) + (0x0464UL))
#define PMIC_NP_OTP0_42_R_ADDR(base) ((base) + (0x0465UL))
#define PMIC_NP_OTP0_43_R_ADDR(base) ((base) + (0x0466UL))
#define PMIC_NP_OTP0_44_R_ADDR(base) ((base) + (0x0467UL))
#define PMIC_NP_OTP0_45_R_ADDR(base) ((base) + (0x0468UL))
#define PMIC_NP_OTP0_46_R_ADDR(base) ((base) + (0x0469UL))
#define PMIC_NP_OTP0_47_R_ADDR(base) ((base) + (0x046AUL))
#define PMIC_NP_OTP0_48_R_ADDR(base) ((base) + (0x046BUL))
#define PMIC_NP_OTP0_49_R_ADDR(base) ((base) + (0x046CUL))
#define PMIC_NP_OTP0_50_R_ADDR(base) ((base) + (0x046DUL))
#define PMIC_NP_OTP0_51_R_ADDR(base) ((base) + (0x046EUL))
#define PMIC_NP_OTP0_52_R_ADDR(base) ((base) + (0x046FUL))
#define PMIC_NP_OTP0_53_R_ADDR(base) ((base) + (0x0470UL))
#define PMIC_NP_OTP0_54_R_ADDR(base) ((base) + (0x0471UL))
#define PMIC_NP_OTP0_55_R_ADDR(base) ((base) + (0x0472UL))
#define PMIC_NP_OTP0_56_R_ADDR(base) ((base) + (0x0473UL))
#define PMIC_NP_OTP0_57_R_ADDR(base) ((base) + (0x0474UL))
#define PMIC_NP_OTP0_58_R_ADDR(base) ((base) + (0x0475UL))
#define PMIC_NP_OTP0_59_R_ADDR(base) ((base) + (0x0476UL))
#define PMIC_NP_OTP0_60_R_ADDR(base) ((base) + (0x0477UL))
#define PMIC_NP_OTP0_61_R_ADDR(base) ((base) + (0x0478UL))
#define PMIC_NP_OTP0_62_R_ADDR(base) ((base) + (0x0479UL))
#define PMIC_NP_OTP0_63_R_ADDR(base) ((base) + (0x047AUL))
#define PMIC_NP_OTP1_0_R_ADDR(base) ((base) + (0x047BUL))
#define PMIC_NP_OTP1_1_R_ADDR(base) ((base) + (0x047CUL))
#define PMIC_NP_OTP1_2_R_ADDR(base) ((base) + (0x047DUL))
#define PMIC_NP_OTP1_3_R_ADDR(base) ((base) + (0x047EUL))
#define PMIC_NP_OTP1_4_R_ADDR(base) ((base) + (0x047FUL))
#define PMIC_NP_OTP1_5_R_ADDR(base) ((base) + (0x0480UL))
#define PMIC_NP_OTP1_6_R_ADDR(base) ((base) + (0x0481UL))
#define PMIC_NP_OTP1_7_R_ADDR(base) ((base) + (0x0482UL))
#define PMIC_NP_OTP1_8_R_ADDR(base) ((base) + (0x0483UL))
#define PMIC_NP_OTP1_9_R_ADDR(base) ((base) + (0x0484UL))
#define PMIC_NP_OTP1_10_R_ADDR(base) ((base) + (0x0485UL))
#define PMIC_NP_OTP1_11_R_ADDR(base) ((base) + (0x0486UL))
#define PMIC_NP_OTP1_12_R_ADDR(base) ((base) + (0x0487UL))
#define PMIC_NP_OTP1_13_R_ADDR(base) ((base) + (0x0488UL))
#define PMIC_NP_OTP1_14_R_ADDR(base) ((base) + (0x0489UL))
#define PMIC_NP_OTP1_15_R_ADDR(base) ((base) + (0x048AUL))
#define PMIC_NP_OTP1_16_R_ADDR(base) ((base) + (0x048BUL))
#define PMIC_NP_OTP1_17_R_ADDR(base) ((base) + (0x048CUL))
#define PMIC_NP_OTP1_18_R_ADDR(base) ((base) + (0x048DUL))
#define PMIC_NP_OTP1_19_R_ADDR(base) ((base) + (0x048EUL))
#define PMIC_NP_OTP1_20_R_ADDR(base) ((base) + (0x048FUL))
#define PMIC_NP_OTP1_21_R_ADDR(base) ((base) + (0x0490UL))
#define PMIC_NP_OTP1_22_R_ADDR(base) ((base) + (0x0491UL))
#define PMIC_NP_OTP1_23_R_ADDR(base) ((base) + (0x0492UL))
#define PMIC_NP_OTP1_24_R_ADDR(base) ((base) + (0x0493UL))
#define PMIC_NP_OTP1_25_R_ADDR(base) ((base) + (0x0494UL))
#define PMIC_NP_OTP1_26_R_ADDR(base) ((base) + (0x0495UL))
#define PMIC_NP_OTP1_27_R_ADDR(base) ((base) + (0x0496UL))
#define PMIC_NP_OTP1_28_R_ADDR(base) ((base) + (0x0497UL))
#define PMIC_NP_OTP1_29_R_ADDR(base) ((base) + (0x0498UL))
#define PMIC_NP_OTP1_30_R_ADDR(base) ((base) + (0x0499UL))
#define PMIC_NP_OTP1_31_R_ADDR(base) ((base) + (0x049AUL))
#define PMIC_NP_OTP1_32_R_ADDR(base) ((base) + (0x049BUL))
#define PMIC_NP_OTP1_33_R_ADDR(base) ((base) + (0x049CUL))
#define PMIC_NP_OTP1_34_R_ADDR(base) ((base) + (0x049DUL))
#define PMIC_NP_OTP1_35_R_ADDR(base) ((base) + (0x049EUL))
#define PMIC_NP_OTP1_36_R_ADDR(base) ((base) + (0x049FUL))
#define PMIC_NP_OTP1_37_R_ADDR(base) ((base) + (0x04A0UL))
#define PMIC_NP_OTP1_38_R_ADDR(base) ((base) + (0x04A1UL))
#define PMIC_NP_OTP1_39_R_ADDR(base) ((base) + (0x04A2UL))
#define PMIC_NP_OTP1_40_R_ADDR(base) ((base) + (0x04A3UL))
#define PMIC_NP_OTP1_41_R_ADDR(base) ((base) + (0x04A4UL))
#define PMIC_NP_OTP1_42_R_ADDR(base) ((base) + (0x04A5UL))
#define PMIC_NP_OTP1_43_R_ADDR(base) ((base) + (0x04A6UL))
#define PMIC_NP_OTP1_44_R_ADDR(base) ((base) + (0x04A7UL))
#define PMIC_NP_OTP1_45_R_ADDR(base) ((base) + (0x04A8UL))
#define PMIC_NP_OTP1_46_R_ADDR(base) ((base) + (0x04A9UL))
#define PMIC_NP_OTP1_47_R_ADDR(base) ((base) + (0x04AAUL))
#define PMIC_NP_OTP1_48_R_ADDR(base) ((base) + (0x04ABUL))
#define PMIC_NP_OTP1_49_R_ADDR(base) ((base) + (0x04ACUL))
#define PMIC_NP_OTP1_50_R_ADDR(base) ((base) + (0x04ADUL))
#define PMIC_NP_OTP1_51_R_ADDR(base) ((base) + (0x04AEUL))
#define PMIC_NP_OTP1_52_R_ADDR(base) ((base) + (0x04AFUL))
#define PMIC_NP_OTP1_53_R_ADDR(base) ((base) + (0x04B0UL))
#define PMIC_NP_OTP1_54_R_ADDR(base) ((base) + (0x04B1UL))
#define PMIC_NP_OTP1_55_R_ADDR(base) ((base) + (0x04B2UL))
#define PMIC_NP_OTP1_56_R_ADDR(base) ((base) + (0x04B3UL))
#define PMIC_NP_OTP1_57_R_ADDR(base) ((base) + (0x04B4UL))
#define PMIC_NP_OTP1_58_R_ADDR(base) ((base) + (0x04B5UL))
#define PMIC_NP_OTP1_59_R_ADDR(base) ((base) + (0x04B6UL))
#define PMIC_NP_OTP1_60_R_ADDR(base) ((base) + (0x04B7UL))
#define PMIC_NP_OTP1_61_R_ADDR(base) ((base) + (0x04B8UL))
#define PMIC_NP_OTP1_62_R_ADDR(base) ((base) + (0x04B9UL))
#define PMIC_NP_OTP1_63_R_ADDR(base) ((base) + (0x04BAUL))
#define PMIC_NP_EN_PMUD_ADDR(base) ((base) + (0x04BBUL))
#define PMIC_NP_NFC_PWRON_TIME_ADDR(base) ((base) + (0x04BFUL))
#define PMIC_NP_VIOCE_RESTART_CTRL_ADDR(base) ((base) + (0x04C0UL))
#define PMIC_NP_PWR_ONOFF_SEQ_CTRL_ADDR(base) ((base) + (0x04C1UL))
#define PMIC_NP_BUCK1_ON_PLACE_ADDR(base) ((base) + (0x04C2UL))
#define PMIC_NP_BUCK40_ON_PLACE_ADDR(base) ((base) + (0x04C3UL))
#define PMIC_NP_BUCK42_ON_PLACE_ADDR(base) ((base) + (0x04C4UL))
#define PMIC_NP_BUCK5_ON_PLACE_ADDR(base) ((base) + (0x04C5UL))
#define PMIC_NP_BUCK70_ON_PLACE_ADDR(base) ((base) + (0x04C6UL))
#define PMIC_NP_BUCK71_ON_PLACE_ADDR(base) ((base) + (0x04C7UL))
#define PMIC_NP_BUCK9_ON_PLACE_ADDR(base) ((base) + (0x04C8UL))
#define PMIC_NP_LDO0_ON_PLACE_ADDR(base) ((base) + (0x04C9UL))
#define PMIC_NP_LDO4_ON_PLACE_ADDR(base) ((base) + (0x04CAUL))
#define PMIC_NP_LDO6_ON_PLACE_ADDR(base) ((base) + (0x04CBUL))
#define PMIC_NP_LDO8_ON_PLACE_ADDR(base) ((base) + (0x04CCUL))
#define PMIC_NP_LDO15_ON_PLACE_ADDR(base) ((base) + (0x04CDUL))
#define PMIC_NP_LDO17_ON_PLACE_ADDR(base) ((base) + (0x04CEUL))
#define PMIC_NP_LDO18_ON_PLACE_ADDR(base) ((base) + (0x04CFUL))
#define PMIC_NP_LDO23_ON_PLACE_ADDR(base) ((base) + (0x04D0UL))
#define PMIC_NP_LDO24_ON_PLACE_ADDR(base) ((base) + (0x04D1UL))
#define PMIC_NP_LDO28_ON_PLACE_ADDR(base) ((base) + (0x04D2UL))
#define PMIC_NP_LDO30_ON_PLACE_ADDR(base) ((base) + (0x04D3UL))
#define PMIC_NP_LDO32_ON_PLACE_ADDR(base) ((base) + (0x04D4UL))
#define PMIC_NP_LDO36_ON_PLACE_ADDR(base) ((base) + (0x04D5UL))
#define PMIC_NP_LDO37_ON_PLACE_ADDR(base) ((base) + (0x04D6UL))
#define PMIC_NP_LDO38_ON_PLACE_ADDR(base) ((base) + (0x04D7UL))
#define PMIC_NP_LDO39_ON_PLACE_ADDR(base) ((base) + (0x04D8UL))
#define PMIC_NP_LDO40_ON_PLACE_ADDR(base) ((base) + (0x04D9UL))
#define PMIC_NP_LDO42_ON_PLACE_ADDR(base) ((base) + (0x04DAUL))
#define PMIC_NP_LDO43_ON_PLACE_ADDR(base) ((base) + (0x04DBUL))
#define PMIC_NP_LDO44_ON_PLACE_ADDR(base) ((base) + (0x04DCUL))
#define PMIC_NP_LDO45_ON_PLACE_ADDR(base) ((base) + (0x04DDUL))
#define PMIC_NP_LDO46_ON_PLACE_ADDR(base) ((base) + (0x04DEUL))
#define PMIC_NP_LDO_BUF_ON_PLACE_ADDR(base) ((base) + (0x04DFUL))
#define PMIC_NP_LDO_SINK_ON_PLACE_ADDR(base) ((base) + (0x04E0UL))
#define PMIC_NP_SW1_ON_PLACE_ADDR(base) ((base) + (0x04E1UL))
#define PMIC_NP_PMU_EN_ON_PLACE_ADDR(base) ((base) + (0x04E2UL))
#define PMIC_NP_BUCK1_OFF_PLACE_ADDR(base) ((base) + (0x04E3UL))
#define PMIC_NP_BUCK40_OFF_PLACE_ADDR(base) ((base) + (0x04E4UL))
#define PMIC_NP_BUCK42_OFF_PLACE_ADDR(base) ((base) + (0x04E5UL))
#define PMIC_NP_BUCK5_OFF_PLACE_ADDR(base) ((base) + (0x04E6UL))
#define PMIC_NP_BUCK70_OFF_PLACE_ADDR(base) ((base) + (0x04E7UL))
#define PMIC_NP_BUCK71_OFF_PLACE_ADDR(base) ((base) + (0x04E8UL))
#define PMIC_NP_BUCK9_OFF_PLACE_ADDR(base) ((base) + (0x04E9UL))
#define PMIC_NP_LDO0_OFF_PLACE_ADDR(base) ((base) + (0x04EAUL))
#define PMIC_NP_LDO4_OFF_PLACE_ADDR(base) ((base) + (0x04EBUL))
#define PMIC_NP_LDO6_OFF_PLACE_ADDR(base) ((base) + (0x04ECUL))
#define PMIC_NP_LDO8_OFF_PLACE_ADDR(base) ((base) + (0x04EDUL))
#define PMIC_NP_LDO15_OFF_PLACE_ADDR(base) ((base) + (0x04EEUL))
#define PMIC_NP_LDO17_OFF_PLACE_ADDR(base) ((base) + (0x04EFUL))
#define PMIC_NP_LDO18_OFF_PLACE_ADDR(base) ((base) + (0x04F0UL))
#define PMIC_NP_LDO23_OFF_PLACE_ADDR(base) ((base) + (0x04F1UL))
#define PMIC_NP_LDO24_OFF_PLACE_ADDR(base) ((base) + (0x04F2UL))
#define PMIC_NP_LDO28_OFF_PLACE_ADDR(base) ((base) + (0x04F3UL))
#define PMIC_NP_LDO30_OFF_PLACE_ADDR(base) ((base) + (0x04F4UL))
#define PMIC_NP_LDO32_OFF_PLACE_ADDR(base) ((base) + (0x04F5UL))
#define PMIC_NP_LDO36_OFF_PLACE_ADDR(base) ((base) + (0x04F6UL))
#define PMIC_NP_LDO37_OFF_PLACE_ADDR(base) ((base) + (0x04F7UL))
#define PMIC_NP_LDO38_OFF_PLACE_ADDR(base) ((base) + (0x04F8UL))
#define PMIC_NP_LDO39_OFF_PLACE_ADDR(base) ((base) + (0x04F9UL))
#define PMIC_NP_LDO40_OFF_PLACE_ADDR(base) ((base) + (0x04FAUL))
#define PMIC_NP_LDO42_OFF_PLACE_ADDR(base) ((base) + (0x04FBUL))
#define PMIC_NP_LDO43_OFF_PLACE_ADDR(base) ((base) + (0x04FCUL))
#define PMIC_NP_LDO44_OFF_PLACE_ADDR(base) ((base) + (0x04FDUL))
#define PMIC_NP_LDO45_OFF_PLACE_ADDR(base) ((base) + (0x04FEUL))
#define PMIC_NP_LDO46_OFF_PLACE_ADDR(base) ((base) + (0x04FFUL))
#define PMIC_NP_LDO_BUF_OFF_PLACE_ADDR(base) ((base) + (0x0500UL))
#define PMIC_NP_LDO_SINK_OFF_PLACE_ADDR(base) ((base) + (0x0501UL))
#define PMIC_NP_SW1_OFF_PLACE_ADDR(base) ((base) + (0x0502UL))
#define PMIC_NP_PMU_EN_OFF_PLACE_ADDR(base) ((base) + (0x0503UL))
#else
#define PMIC_NP_SYS_CTRL0_ADDR(base) ((base) + (0x02E9))
#define PMIC_NP_SYS_CTRL1_ADDR(base) ((base) + (0x02EA))
#define PMIC_NP_CLK_WIFI_CTRL_ADDR(base) ((base) + (0x0400))
#define PMIC_NP_CLK_NFC_CTRL_ADDR(base) ((base) + (0x0401))
#define PMIC_NP_CLK_RF0_CTRL_ADDR(base) ((base) + (0x0402))
#define PMIC_NP_CLK_RF1_CTRL_ADDR(base) ((base) + (0x0403))
#define PMIC_NP_CLK_CODEC_CTRL_ADDR(base) ((base) + (0x0404))
#define PMIC_NP_CLK_TOP_CTRL0_ADDR(base) ((base) + (0x0405))
#define PMIC_NP_CLK_TOP_CTRL1_ADDR(base) ((base) + (0x0406))
#define PMIC_NP_CLK_256K_CTRL0_ADDR(base) ((base) + (0x0407))
#define PMIC_NP_CLK_256K_CTRL1_ADDR(base) ((base) + (0x0408))
#define PMIC_NP_CLK_UFS_FRE_CTRL_ADDR(base) ((base) + (0x0409))
#define PMIC_NP_VSYS_LOW_SET_ADDR(base) ((base) + (0x040A))
#define PMIC_NP_VSYS_DROP_SET_ADDR(base) ((base) + (0x040B))
#define PMIC_NP_NFC_ON_CTRL_ADDR(base) ((base) + (0x040C))
#define PMIC_NP_HRESET_PWRDOWN_CTRL_ADDR(base) ((base) + (0x040D))
#define PMIC_NP_SMPL_CTRL_ADDR(base) ((base) + (0x040E))
#define PMIC_NP_DEBUG_LOCK_ADDR(base) ((base) + (0x040F))
#define PMIC_NP_SYS_DEBUG0_ADDR(base) ((base) + (0x0410))
#define PMIC_NP_SYS_DEBUG1_ADDR(base) ((base) + (0x0411))
#define PMIC_NP_SYS_DEBUG2_ADDR(base) ((base) + (0x0412))
#define PMIC_NP_SYS_DEBUG3_ADDR(base) ((base) + (0x0413))
#define PMIC_NP_SYS_DEBUG4_ADDR(base) ((base) + (0x0414))
#define PMIC_NP_SYS_DEBUG5_ADDR(base) ((base) + (0x0415))
#define PMIC_NP_BACKUP_CHG_ADDR(base) ((base) + (0x0416))
#define PMIC_NP_CTRL_ADDR(base) ((base) + (0x0417))
#define PMIC_NP_SHIELD_RES_ADDR(base) ((base) + (0x0418))
#define PMIC_NP_D2A_RES0_ADDR(base) ((base) + (0x0419))
#define PMIC_NP_D2A_RES1_ADDR(base) ((base) + (0x041A))
#define PMIC_HRST_REG0_ADDR(base) ((base) + (0x041B))
#define PMIC_HRST_REG1_ADDR(base) ((base) + (0x041C))
#define PMIC_HRST_REG2_ADDR(base) ((base) + (0x041D))
#define PMIC_HRST_REG3_ADDR(base) ((base) + (0x041E))
#define PMIC_HRST_REG4_ADDR(base) ((base) + (0x041F))
#define PMIC_HRST_REG5_ADDR(base) ((base) + (0x0420))
#define PMIC_HRST_REG6_ADDR(base) ((base) + (0x0421))
#define PMIC_HRST_REG7_ADDR(base) ((base) + (0x0422))
#define PMIC_HRST_REG8_ADDR(base) ((base) + (0x0423))
#define PMIC_HRST_REG9_ADDR(base) ((base) + (0x0424))
#define PMIC_HRST_REG10_ADDR(base) ((base) + (0x0425))
#define PMIC_HRST_REG11_ADDR(base) ((base) + (0x0426))
#define PMIC_HRST_REG12_ADDR(base) ((base) + (0x0427))
#define PMIC_HRST_REG13_ADDR(base) ((base) + (0x0428))
#define PMIC_HRST_REG14_ADDR(base) ((base) + (0x0429))
#define PMIC_HRST_REG15_ADDR(base) ((base) + (0x042A))
#define PMIC_HRST_REG16_ADDR(base) ((base) + (0x042B))
#define PMIC_HRST_REG17_ADDR(base) ((base) + (0x042C))
#define PMIC_HRST_REG18_ADDR(base) ((base) + (0x042D))
#define PMIC_HRST_REG19_ADDR(base) ((base) + (0x042E))
#define PMIC_HRST_REG20_ADDR(base) ((base) + (0x042F))
#define PMIC_HRST_REG21_ADDR(base) ((base) + (0x0430))
#define PMIC_HRST_REG22_ADDR(base) ((base) + (0x0431))
#define PMIC_HRST_REG23_ADDR(base) ((base) + (0x0432))
#define PMIC_HRST_REG24_ADDR(base) ((base) + (0x0433))
#define PMIC_HRST_REG25_ADDR(base) ((base) + (0x0434))
#define PMIC_HRST_REG26_ADDR(base) ((base) + (0x0435))
#define PMIC_HRST_REG27_ADDR(base) ((base) + (0x0436))
#define PMIC_HRST_REG28_ADDR(base) ((base) + (0x0437))
#define PMIC_HRST_REG29_ADDR(base) ((base) + (0x0438))
#define PMIC_HRST_REG30_ADDR(base) ((base) + (0x0439))
#define PMIC_HRST_REG31_ADDR(base) ((base) + (0x043A))
#define PMIC_NP_OTP0_0_R_ADDR(base) ((base) + (0x043B))
#define PMIC_NP_OTP0_1_R_ADDR(base) ((base) + (0x043C))
#define PMIC_NP_OTP0_2_R_ADDR(base) ((base) + (0x043D))
#define PMIC_NP_OTP0_3_R_ADDR(base) ((base) + (0x043E))
#define PMIC_NP_OTP0_4_R_ADDR(base) ((base) + (0x043F))
#define PMIC_NP_OTP0_5_R_ADDR(base) ((base) + (0x0440))
#define PMIC_NP_OTP0_6_R_ADDR(base) ((base) + (0x0441))
#define PMIC_NP_OTP0_7_R_ADDR(base) ((base) + (0x0442))
#define PMIC_NP_OTP0_8_R_ADDR(base) ((base) + (0x0443))
#define PMIC_NP_OTP0_9_R_ADDR(base) ((base) + (0x0444))
#define PMIC_NP_OTP0_10_R_ADDR(base) ((base) + (0x0445))
#define PMIC_NP_OTP0_11_R_ADDR(base) ((base) + (0x0446))
#define PMIC_NP_OTP0_12_R_ADDR(base) ((base) + (0x0447))
#define PMIC_NP_OTP0_13_R_ADDR(base) ((base) + (0x0448))
#define PMIC_NP_OTP0_14_R_ADDR(base) ((base) + (0x0449))
#define PMIC_NP_OTP0_15_R_ADDR(base) ((base) + (0x044A))
#define PMIC_NP_OTP0_16_R_ADDR(base) ((base) + (0x044B))
#define PMIC_NP_OTP0_17_R_ADDR(base) ((base) + (0x044C))
#define PMIC_NP_OTP0_18_R_ADDR(base) ((base) + (0x044D))
#define PMIC_NP_OTP0_19_R_ADDR(base) ((base) + (0x044E))
#define PMIC_NP_OTP0_20_R_ADDR(base) ((base) + (0x044F))
#define PMIC_NP_OTP0_21_R_ADDR(base) ((base) + (0x0450))
#define PMIC_NP_OTP0_22_R_ADDR(base) ((base) + (0x0451))
#define PMIC_NP_OTP0_23_R_ADDR(base) ((base) + (0x0452))
#define PMIC_NP_OTP0_24_R_ADDR(base) ((base) + (0x0453))
#define PMIC_NP_OTP0_25_R_ADDR(base) ((base) + (0x0454))
#define PMIC_NP_OTP0_26_R_ADDR(base) ((base) + (0x0455))
#define PMIC_NP_OTP0_27_R_ADDR(base) ((base) + (0x0456))
#define PMIC_NP_OTP0_28_R_ADDR(base) ((base) + (0x0457))
#define PMIC_NP_OTP0_29_R_ADDR(base) ((base) + (0x0458))
#define PMIC_NP_OTP0_30_R_ADDR(base) ((base) + (0x0459))
#define PMIC_NP_OTP0_31_R_ADDR(base) ((base) + (0x045A))
#define PMIC_NP_OTP0_32_R_ADDR(base) ((base) + (0x045B))
#define PMIC_NP_OTP0_33_R_ADDR(base) ((base) + (0x045C))
#define PMIC_NP_OTP0_34_R_ADDR(base) ((base) + (0x045D))
#define PMIC_NP_OTP0_35_R_ADDR(base) ((base) + (0x045E))
#define PMIC_NP_OTP0_36_R_ADDR(base) ((base) + (0x045F))
#define PMIC_NP_OTP0_37_R_ADDR(base) ((base) + (0x0460))
#define PMIC_NP_OTP0_38_R_ADDR(base) ((base) + (0x0461))
#define PMIC_NP_OTP0_39_R_ADDR(base) ((base) + (0x0462))
#define PMIC_NP_OTP0_40_R_ADDR(base) ((base) + (0x0463))
#define PMIC_NP_OTP0_41_R_ADDR(base) ((base) + (0x0464))
#define PMIC_NP_OTP0_42_R_ADDR(base) ((base) + (0x0465))
#define PMIC_NP_OTP0_43_R_ADDR(base) ((base) + (0x0466))
#define PMIC_NP_OTP0_44_R_ADDR(base) ((base) + (0x0467))
#define PMIC_NP_OTP0_45_R_ADDR(base) ((base) + (0x0468))
#define PMIC_NP_OTP0_46_R_ADDR(base) ((base) + (0x0469))
#define PMIC_NP_OTP0_47_R_ADDR(base) ((base) + (0x046A))
#define PMIC_NP_OTP0_48_R_ADDR(base) ((base) + (0x046B))
#define PMIC_NP_OTP0_49_R_ADDR(base) ((base) + (0x046C))
#define PMIC_NP_OTP0_50_R_ADDR(base) ((base) + (0x046D))
#define PMIC_NP_OTP0_51_R_ADDR(base) ((base) + (0x046E))
#define PMIC_NP_OTP0_52_R_ADDR(base) ((base) + (0x046F))
#define PMIC_NP_OTP0_53_R_ADDR(base) ((base) + (0x0470))
#define PMIC_NP_OTP0_54_R_ADDR(base) ((base) + (0x0471))
#define PMIC_NP_OTP0_55_R_ADDR(base) ((base) + (0x0472))
#define PMIC_NP_OTP0_56_R_ADDR(base) ((base) + (0x0473))
#define PMIC_NP_OTP0_57_R_ADDR(base) ((base) + (0x0474))
#define PMIC_NP_OTP0_58_R_ADDR(base) ((base) + (0x0475))
#define PMIC_NP_OTP0_59_R_ADDR(base) ((base) + (0x0476))
#define PMIC_NP_OTP0_60_R_ADDR(base) ((base) + (0x0477))
#define PMIC_NP_OTP0_61_R_ADDR(base) ((base) + (0x0478))
#define PMIC_NP_OTP0_62_R_ADDR(base) ((base) + (0x0479))
#define PMIC_NP_OTP0_63_R_ADDR(base) ((base) + (0x047A))
#define PMIC_NP_OTP1_0_R_ADDR(base) ((base) + (0x047B))
#define PMIC_NP_OTP1_1_R_ADDR(base) ((base) + (0x047C))
#define PMIC_NP_OTP1_2_R_ADDR(base) ((base) + (0x047D))
#define PMIC_NP_OTP1_3_R_ADDR(base) ((base) + (0x047E))
#define PMIC_NP_OTP1_4_R_ADDR(base) ((base) + (0x047F))
#define PMIC_NP_OTP1_5_R_ADDR(base) ((base) + (0x0480))
#define PMIC_NP_OTP1_6_R_ADDR(base) ((base) + (0x0481))
#define PMIC_NP_OTP1_7_R_ADDR(base) ((base) + (0x0482))
#define PMIC_NP_OTP1_8_R_ADDR(base) ((base) + (0x0483))
#define PMIC_NP_OTP1_9_R_ADDR(base) ((base) + (0x0484))
#define PMIC_NP_OTP1_10_R_ADDR(base) ((base) + (0x0485))
#define PMIC_NP_OTP1_11_R_ADDR(base) ((base) + (0x0486))
#define PMIC_NP_OTP1_12_R_ADDR(base) ((base) + (0x0487))
#define PMIC_NP_OTP1_13_R_ADDR(base) ((base) + (0x0488))
#define PMIC_NP_OTP1_14_R_ADDR(base) ((base) + (0x0489))
#define PMIC_NP_OTP1_15_R_ADDR(base) ((base) + (0x048A))
#define PMIC_NP_OTP1_16_R_ADDR(base) ((base) + (0x048B))
#define PMIC_NP_OTP1_17_R_ADDR(base) ((base) + (0x048C))
#define PMIC_NP_OTP1_18_R_ADDR(base) ((base) + (0x048D))
#define PMIC_NP_OTP1_19_R_ADDR(base) ((base) + (0x048E))
#define PMIC_NP_OTP1_20_R_ADDR(base) ((base) + (0x048F))
#define PMIC_NP_OTP1_21_R_ADDR(base) ((base) + (0x0490))
#define PMIC_NP_OTP1_22_R_ADDR(base) ((base) + (0x0491))
#define PMIC_NP_OTP1_23_R_ADDR(base) ((base) + (0x0492))
#define PMIC_NP_OTP1_24_R_ADDR(base) ((base) + (0x0493))
#define PMIC_NP_OTP1_25_R_ADDR(base) ((base) + (0x0494))
#define PMIC_NP_OTP1_26_R_ADDR(base) ((base) + (0x0495))
#define PMIC_NP_OTP1_27_R_ADDR(base) ((base) + (0x0496))
#define PMIC_NP_OTP1_28_R_ADDR(base) ((base) + (0x0497))
#define PMIC_NP_OTP1_29_R_ADDR(base) ((base) + (0x0498))
#define PMIC_NP_OTP1_30_R_ADDR(base) ((base) + (0x0499))
#define PMIC_NP_OTP1_31_R_ADDR(base) ((base) + (0x049A))
#define PMIC_NP_OTP1_32_R_ADDR(base) ((base) + (0x049B))
#define PMIC_NP_OTP1_33_R_ADDR(base) ((base) + (0x049C))
#define PMIC_NP_OTP1_34_R_ADDR(base) ((base) + (0x049D))
#define PMIC_NP_OTP1_35_R_ADDR(base) ((base) + (0x049E))
#define PMIC_NP_OTP1_36_R_ADDR(base) ((base) + (0x049F))
#define PMIC_NP_OTP1_37_R_ADDR(base) ((base) + (0x04A0))
#define PMIC_NP_OTP1_38_R_ADDR(base) ((base) + (0x04A1))
#define PMIC_NP_OTP1_39_R_ADDR(base) ((base) + (0x04A2))
#define PMIC_NP_OTP1_40_R_ADDR(base) ((base) + (0x04A3))
#define PMIC_NP_OTP1_41_R_ADDR(base) ((base) + (0x04A4))
#define PMIC_NP_OTP1_42_R_ADDR(base) ((base) + (0x04A5))
#define PMIC_NP_OTP1_43_R_ADDR(base) ((base) + (0x04A6))
#define PMIC_NP_OTP1_44_R_ADDR(base) ((base) + (0x04A7))
#define PMIC_NP_OTP1_45_R_ADDR(base) ((base) + (0x04A8))
#define PMIC_NP_OTP1_46_R_ADDR(base) ((base) + (0x04A9))
#define PMIC_NP_OTP1_47_R_ADDR(base) ((base) + (0x04AA))
#define PMIC_NP_OTP1_48_R_ADDR(base) ((base) + (0x04AB))
#define PMIC_NP_OTP1_49_R_ADDR(base) ((base) + (0x04AC))
#define PMIC_NP_OTP1_50_R_ADDR(base) ((base) + (0x04AD))
#define PMIC_NP_OTP1_51_R_ADDR(base) ((base) + (0x04AE))
#define PMIC_NP_OTP1_52_R_ADDR(base) ((base) + (0x04AF))
#define PMIC_NP_OTP1_53_R_ADDR(base) ((base) + (0x04B0))
#define PMIC_NP_OTP1_54_R_ADDR(base) ((base) + (0x04B1))
#define PMIC_NP_OTP1_55_R_ADDR(base) ((base) + (0x04B2))
#define PMIC_NP_OTP1_56_R_ADDR(base) ((base) + (0x04B3))
#define PMIC_NP_OTP1_57_R_ADDR(base) ((base) + (0x04B4))
#define PMIC_NP_OTP1_58_R_ADDR(base) ((base) + (0x04B5))
#define PMIC_NP_OTP1_59_R_ADDR(base) ((base) + (0x04B6))
#define PMIC_NP_OTP1_60_R_ADDR(base) ((base) + (0x04B7))
#define PMIC_NP_OTP1_61_R_ADDR(base) ((base) + (0x04B8))
#define PMIC_NP_OTP1_62_R_ADDR(base) ((base) + (0x04B9))
#define PMIC_NP_OTP1_63_R_ADDR(base) ((base) + (0x04BA))
#define PMIC_NP_EN_PMUD_ADDR(base) ((base) + (0x04BB))
#define PMIC_NP_NFC_PWRON_TIME_ADDR(base) ((base) + (0x04BF))
#define PMIC_NP_VIOCE_RESTART_CTRL_ADDR(base) ((base) + (0x04C0))
#define PMIC_NP_PWR_ONOFF_SEQ_CTRL_ADDR(base) ((base) + (0x04C1))
#define PMIC_NP_BUCK1_ON_PLACE_ADDR(base) ((base) + (0x04C2))
#define PMIC_NP_BUCK40_ON_PLACE_ADDR(base) ((base) + (0x04C3))
#define PMIC_NP_BUCK42_ON_PLACE_ADDR(base) ((base) + (0x04C4))
#define PMIC_NP_BUCK5_ON_PLACE_ADDR(base) ((base) + (0x04C5))
#define PMIC_NP_BUCK70_ON_PLACE_ADDR(base) ((base) + (0x04C6))
#define PMIC_NP_BUCK71_ON_PLACE_ADDR(base) ((base) + (0x04C7))
#define PMIC_NP_BUCK9_ON_PLACE_ADDR(base) ((base) + (0x04C8))
#define PMIC_NP_LDO0_ON_PLACE_ADDR(base) ((base) + (0x04C9))
#define PMIC_NP_LDO4_ON_PLACE_ADDR(base) ((base) + (0x04CA))
#define PMIC_NP_LDO6_ON_PLACE_ADDR(base) ((base) + (0x04CB))
#define PMIC_NP_LDO8_ON_PLACE_ADDR(base) ((base) + (0x04CC))
#define PMIC_NP_LDO15_ON_PLACE_ADDR(base) ((base) + (0x04CD))
#define PMIC_NP_LDO17_ON_PLACE_ADDR(base) ((base) + (0x04CE))
#define PMIC_NP_LDO18_ON_PLACE_ADDR(base) ((base) + (0x04CF))
#define PMIC_NP_LDO23_ON_PLACE_ADDR(base) ((base) + (0x04D0))
#define PMIC_NP_LDO24_ON_PLACE_ADDR(base) ((base) + (0x04D1))
#define PMIC_NP_LDO28_ON_PLACE_ADDR(base) ((base) + (0x04D2))
#define PMIC_NP_LDO30_ON_PLACE_ADDR(base) ((base) + (0x04D3))
#define PMIC_NP_LDO32_ON_PLACE_ADDR(base) ((base) + (0x04D4))
#define PMIC_NP_LDO36_ON_PLACE_ADDR(base) ((base) + (0x04D5))
#define PMIC_NP_LDO37_ON_PLACE_ADDR(base) ((base) + (0x04D6))
#define PMIC_NP_LDO38_ON_PLACE_ADDR(base) ((base) + (0x04D7))
#define PMIC_NP_LDO39_ON_PLACE_ADDR(base) ((base) + (0x04D8))
#define PMIC_NP_LDO40_ON_PLACE_ADDR(base) ((base) + (0x04D9))
#define PMIC_NP_LDO42_ON_PLACE_ADDR(base) ((base) + (0x04DA))
#define PMIC_NP_LDO43_ON_PLACE_ADDR(base) ((base) + (0x04DB))
#define PMIC_NP_LDO44_ON_PLACE_ADDR(base) ((base) + (0x04DC))
#define PMIC_NP_LDO45_ON_PLACE_ADDR(base) ((base) + (0x04DD))
#define PMIC_NP_LDO46_ON_PLACE_ADDR(base) ((base) + (0x04DE))
#define PMIC_NP_LDO_BUF_ON_PLACE_ADDR(base) ((base) + (0x04DF))
#define PMIC_NP_LDO_SINK_ON_PLACE_ADDR(base) ((base) + (0x04E0))
#define PMIC_NP_SW1_ON_PLACE_ADDR(base) ((base) + (0x04E1))
#define PMIC_NP_PMU_EN_ON_PLACE_ADDR(base) ((base) + (0x04E2))
#define PMIC_NP_BUCK1_OFF_PLACE_ADDR(base) ((base) + (0x04E3))
#define PMIC_NP_BUCK40_OFF_PLACE_ADDR(base) ((base) + (0x04E4))
#define PMIC_NP_BUCK42_OFF_PLACE_ADDR(base) ((base) + (0x04E5))
#define PMIC_NP_BUCK5_OFF_PLACE_ADDR(base) ((base) + (0x04E6))
#define PMIC_NP_BUCK70_OFF_PLACE_ADDR(base) ((base) + (0x04E7))
#define PMIC_NP_BUCK71_OFF_PLACE_ADDR(base) ((base) + (0x04E8))
#define PMIC_NP_BUCK9_OFF_PLACE_ADDR(base) ((base) + (0x04E9))
#define PMIC_NP_LDO0_OFF_PLACE_ADDR(base) ((base) + (0x04EA))
#define PMIC_NP_LDO4_OFF_PLACE_ADDR(base) ((base) + (0x04EB))
#define PMIC_NP_LDO6_OFF_PLACE_ADDR(base) ((base) + (0x04EC))
#define PMIC_NP_LDO8_OFF_PLACE_ADDR(base) ((base) + (0x04ED))
#define PMIC_NP_LDO15_OFF_PLACE_ADDR(base) ((base) + (0x04EE))
#define PMIC_NP_LDO17_OFF_PLACE_ADDR(base) ((base) + (0x04EF))
#define PMIC_NP_LDO18_OFF_PLACE_ADDR(base) ((base) + (0x04F0))
#define PMIC_NP_LDO23_OFF_PLACE_ADDR(base) ((base) + (0x04F1))
#define PMIC_NP_LDO24_OFF_PLACE_ADDR(base) ((base) + (0x04F2))
#define PMIC_NP_LDO28_OFF_PLACE_ADDR(base) ((base) + (0x04F3))
#define PMIC_NP_LDO30_OFF_PLACE_ADDR(base) ((base) + (0x04F4))
#define PMIC_NP_LDO32_OFF_PLACE_ADDR(base) ((base) + (0x04F5))
#define PMIC_NP_LDO36_OFF_PLACE_ADDR(base) ((base) + (0x04F6))
#define PMIC_NP_LDO37_OFF_PLACE_ADDR(base) ((base) + (0x04F7))
#define PMIC_NP_LDO38_OFF_PLACE_ADDR(base) ((base) + (0x04F8))
#define PMIC_NP_LDO39_OFF_PLACE_ADDR(base) ((base) + (0x04F9))
#define PMIC_NP_LDO40_OFF_PLACE_ADDR(base) ((base) + (0x04FA))
#define PMIC_NP_LDO42_OFF_PLACE_ADDR(base) ((base) + (0x04FB))
#define PMIC_NP_LDO43_OFF_PLACE_ADDR(base) ((base) + (0x04FC))
#define PMIC_NP_LDO44_OFF_PLACE_ADDR(base) ((base) + (0x04FD))
#define PMIC_NP_LDO45_OFF_PLACE_ADDR(base) ((base) + (0x04FE))
#define PMIC_NP_LDO46_OFF_PLACE_ADDR(base) ((base) + (0x04FF))
#define PMIC_NP_LDO_BUF_OFF_PLACE_ADDR(base) ((base) + (0x0500))
#define PMIC_NP_LDO_SINK_OFF_PLACE_ADDR(base) ((base) + (0x0501))
#define PMIC_NP_SW1_OFF_PLACE_ADDR(base) ((base) + (0x0502))
#define PMIC_NP_PMU_EN_OFF_PLACE_ADDR(base) ((base) + (0x0503))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_DUMMY_SPMI_ADDR(base) ((base) + (0x0800UL))
#define PMIC_BUCK1_CTRL0_ADDR(base) ((base) + (0x0801UL))
#define PMIC_BUCK1_CTRL1_ADDR(base) ((base) + (0x0802UL))
#define PMIC_BUCK1_CTRL2_ADDR(base) ((base) + (0x0803UL))
#define PMIC_BUCK1_CTRL3_ADDR(base) ((base) + (0x0804UL))
#define PMIC_BUCK1_CTRL4_ADDR(base) ((base) + (0x0805UL))
#define PMIC_BUCK1_CTRL5_ADDR(base) ((base) + (0x0806UL))
#define PMIC_BUCK1_CTRL6_ADDR(base) ((base) + (0x0807UL))
#define PMIC_BUCK1_CTRL7_ADDR(base) ((base) + (0x0808UL))
#define PMIC_BUCK1_CTRL8_ADDR(base) ((base) + (0x0809UL))
#define PMIC_BUCK1_CTRL9_ADDR(base) ((base) + (0x080AUL))
#define PMIC_BUCK1_CTRL10_ADDR(base) ((base) + (0x080BUL))
#define PMIC_BUCK1_CTRL11_ADDR(base) ((base) + (0x080CUL))
#define PMIC_BUCK1_CTRL12_ADDR(base) ((base) + (0x080DUL))
#define PMIC_BUCK1_CTRL13_ADDR(base) ((base) + (0x080EUL))
#define PMIC_BUCK1_CTRL14_ADDR(base) ((base) + (0x080FUL))
#define PMIC_BUCK2_CTRL0_ADDR(base) ((base) + (0x0810UL))
#define PMIC_BUCK2_CTRL1_ADDR(base) ((base) + (0x0811UL))
#define PMIC_BUCK2_CTRL2_ADDR(base) ((base) + (0x0812UL))
#define PMIC_BUCK2_CTRL3_ADDR(base) ((base) + (0x0813UL))
#define PMIC_BUCK2_CTRL4_ADDR(base) ((base) + (0x0814UL))
#define PMIC_BUCK2_CTRL5_ADDR(base) ((base) + (0x0815UL))
#define PMIC_BUCK2_CTRL6_ADDR(base) ((base) + (0x0816UL))
#define PMIC_BUCK2_CTRL7_ADDR(base) ((base) + (0x0817UL))
#define PMIC_BUCK2_CTRL8_ADDR(base) ((base) + (0x0818UL))
#define PMIC_BUCK2_CTRL9_ADDR(base) ((base) + (0x0819UL))
#define PMIC_BUCK2_CTRL10_ADDR(base) ((base) + (0x081AUL))
#define PMIC_BUCK2_CTRL11_ADDR(base) ((base) + (0x081BUL))
#define PMIC_BUCK2_CTRL12_ADDR(base) ((base) + (0x081CUL))
#define PMIC_BUCK2_CTRL13_ADDR(base) ((base) + (0x081DUL))
#define PMIC_BUCK3_CTRL0_ADDR(base) ((base) + (0x081EUL))
#define PMIC_BUCK3_CTRL1_ADDR(base) ((base) + (0x081FUL))
#define PMIC_BUCK3_CTRL2_ADDR(base) ((base) + (0x0820UL))
#define PMIC_BUCK3_CTRL3_ADDR(base) ((base) + (0x0821UL))
#define PMIC_BUCK3_CTRL4_ADDR(base) ((base) + (0x0822UL))
#define PMIC_BUCK3_CTRL5_ADDR(base) ((base) + (0x0823UL))
#define PMIC_BUCK3_CTRL6_ADDR(base) ((base) + (0x0824UL))
#define PMIC_BUCK3_CTRL7_ADDR(base) ((base) + (0x0825UL))
#define PMIC_BUCK3_CTRL8_ADDR(base) ((base) + (0x0826UL))
#define PMIC_BUCK3_CTRL9_ADDR(base) ((base) + (0x0827UL))
#define PMIC_BUCK3_CTRL10_ADDR(base) ((base) + (0x0828UL))
#define PMIC_BUCK3_CTRL11_ADDR(base) ((base) + (0x0829UL))
#define PMIC_BUCK3_CTRL12_ADDR(base) ((base) + (0x082AUL))
#define PMIC_BUCK3_CTRL13_ADDR(base) ((base) + (0x082BUL))
#define PMIC_BUCK401_CTRL0_ADDR(base) ((base) + (0x082CUL))
#define PMIC_BUCK401_CTRL1_ADDR(base) ((base) + (0x082DUL))
#define PMIC_BUCK401_CTRL2_ADDR(base) ((base) + (0x082EUL))
#define PMIC_BUCK401_CTRL3_ADDR(base) ((base) + (0x082FUL))
#define PMIC_BUCK401_CTRL4_ADDR(base) ((base) + (0x0830UL))
#define PMIC_BUCK401_CTRL5_ADDR(base) ((base) + (0x0831UL))
#define PMIC_BUCK401_CTRL6_ADDR(base) ((base) + (0x0832UL))
#define PMIC_BUCK401_CTRL7_ADDR(base) ((base) + (0x0833UL))
#define PMIC_BUCK401_CTRL8_ADDR(base) ((base) + (0x0834UL))
#define PMIC_BUCK401_CTRL9_ADDR(base) ((base) + (0x0835UL))
#define PMIC_BUCK401_CTRL10_ADDR(base) ((base) + (0x0836UL))
#define PMIC_BUCK401_CTRL11_ADDR(base) ((base) + (0x0837UL))
#define PMIC_BUCK401_CTRL12_ADDR(base) ((base) + (0x0838UL))
#define PMIC_BUCK401_CTRL13_ADDR(base) ((base) + (0x0839UL))
#define PMIC_BUCK401_CTRL14_ADDR(base) ((base) + (0x083AUL))
#define PMIC_BUCK401_CTRL15_ADDR(base) ((base) + (0x083BUL))
#define PMIC_BUCK401_CTRL16_ADDR(base) ((base) + (0x083CUL))
#define PMIC_BUCK401_CTRL17_ADDR(base) ((base) + (0x083DUL))
#define PMIC_BUCK401_CTRL18_ADDR(base) ((base) + (0x083EUL))
#define PMIC_BUCK401_CTRL19_ADDR(base) ((base) + (0x083FUL))
#define PMIC_BUCK401_CTRL20_ADDR(base) ((base) + (0x0840UL))
#define PMIC_BUCK401_CTRL21_ADDR(base) ((base) + (0x0841UL))
#define PMIC_BUCK401_CTRL22_ADDR(base) ((base) + (0x0842UL))
#define PMIC_BUCK401_CTRL23_ADDR(base) ((base) + (0x0843UL))
#define PMIC_BUCK401_CTRL24_ADDR(base) ((base) + (0x0844UL))
#define PMIC_BUCK401_CTRL25_ADDR(base) ((base) + (0x0845UL))
#define PMIC_BUCK401_CTRL26_ADDR(base) ((base) + (0x0846UL))
#define PMIC_BUCK401_CTRL27_ADDR(base) ((base) + (0x0847UL))
#define PMIC_BUCK401_CTRL28_ADDR(base) ((base) + (0x0848UL))
#define PMIC_BUCK401_CTRL29_ADDR(base) ((base) + (0x0849UL))
#define PMIC_BUCK401_CTRL30_ADDR(base) ((base) + (0x084AUL))
#define PMIC_BUCK401_CTRL31_ADDR(base) ((base) + (0x084BUL))
#define PMIC_BUCK401_CTRL32_ADDR(base) ((base) + (0x084CUL))
#define PMIC_BUCK401_CTRL33_ADDR(base) ((base) + (0x084DUL))
#define PMIC_BUCK401_CTRL34_ADDR(base) ((base) + (0x084EUL))
#define PMIC_BUCK401_CTRL35_ADDR(base) ((base) + (0x084FUL))
#define PMIC_BUCK401_CTRL36_ADDR(base) ((base) + (0x0850UL))
#define PMIC_BUCK401_CTRL37_ADDR(base) ((base) + (0x0851UL))
#define PMIC_BUCK401_CTRL38_ADDR(base) ((base) + (0x0852UL))
#define PMIC_BUCK401_CTRL39_ADDR(base) ((base) + (0x0853UL))
#define PMIC_BUCK401_CTRL40_ADDR(base) ((base) + (0x0854UL))
#define PMIC_BUCK401_CTRL41_ADDR(base) ((base) + (0x0855UL))
#define PMIC_BUCK4012_CTRL0_ADDR(base) ((base) + (0x0856UL))
#define PMIC_BUCK4012_CTRL1_ADDR(base) ((base) + (0x0857UL))
#define PMIC_BUCK4012_CTRL2_ADDR(base) ((base) + (0x0858UL))
#define PMIC_BUCK4012_CTRL3_ADDR(base) ((base) + (0x0859UL))
#define PMIC_BUCK4012_CTRL4_ADDR(base) ((base) + (0x085AUL))
#define PMIC_BUCK4012_RESERVE0_ADDR(base) ((base) + (0x085BUL))
#define PMIC_BUCK4012_RESERVE1_ADDR(base) ((base) + (0x085CUL))
#define PMIC_BUCK4012_RESERVE2_ADDR(base) ((base) + (0x085DUL))
#define PMIC_BUCK4012_RESERVE3_ADDR(base) ((base) + (0x085EUL))
#define PMIC_BUCK40_CTRL0_ADDR(base) ((base) + (0x085FUL))
#define PMIC_BUCK40_CTRL1_ADDR(base) ((base) + (0x0860UL))
#define PMIC_BUCK40_CTRL2_ADDR(base) ((base) + (0x0861UL))
#define PMIC_BUCK40_CTRL3_ADDR(base) ((base) + (0x0862UL))
#define PMIC_BUCK40_CTRL4_ADDR(base) ((base) + (0x0863UL))
#define PMIC_BUCK40_CTRL5_ADDR(base) ((base) + (0x0864UL))
#define PMIC_BUCK40_CTRL6_ADDR(base) ((base) + (0x0865UL))
#define PMIC_BUCK40_CTRL7_ADDR(base) ((base) + (0x0866UL))
#define PMIC_BUCK40_CTRL8_ADDR(base) ((base) + (0x0867UL))
#define PMIC_BUCK40_CTRL9_ADDR(base) ((base) + (0x0868UL))
#define PMIC_BUCK40_CTRL10_ADDR(base) ((base) + (0x0869UL))
#define PMIC_BUCK41_CTRL0_ADDR(base) ((base) + (0x086AUL))
#define PMIC_BUCK41_CTRL1_ADDR(base) ((base) + (0x086BUL))
#define PMIC_BUCK41_CTRL2_ADDR(base) ((base) + (0x086CUL))
#define PMIC_BUCK41_CTRL3_ADDR(base) ((base) + (0x086DUL))
#define PMIC_BUCK41_CTRL4_ADDR(base) ((base) + (0x086EUL))
#define PMIC_BUCK41_CTRL5_ADDR(base) ((base) + (0x086FUL))
#define PMIC_BUCK41_CTRL6_ADDR(base) ((base) + (0x0870UL))
#define PMIC_BUCK41_CTRL7_ADDR(base) ((base) + (0x0871UL))
#define PMIC_BUCK41_CTRL8_ADDR(base) ((base) + (0x0872UL))
#define PMIC_BUCK41_CTRL9_ADDR(base) ((base) + (0x0873UL))
#define PMIC_BUCK41_CTRL10_ADDR(base) ((base) + (0x0874UL))
#define PMIC_BUCK42_CTRL0_ADDR(base) ((base) + (0x0875UL))
#define PMIC_BUCK42_CTRL1_ADDR(base) ((base) + (0x0876UL))
#define PMIC_BUCK42_CTRL2_ADDR(base) ((base) + (0x0877UL))
#define PMIC_BUCK42_CTRL3_ADDR(base) ((base) + (0x0878UL))
#define PMIC_BUCK42_CTRL4_ADDR(base) ((base) + (0x0879UL))
#define PMIC_BUCK42_CTRL5_ADDR(base) ((base) + (0x087AUL))
#define PMIC_BUCK42_CTRL6_ADDR(base) ((base) + (0x087BUL))
#define PMIC_BUCK42_CTRL7_ADDR(base) ((base) + (0x087CUL))
#define PMIC_BUCK42_CTRL8_ADDR(base) ((base) + (0x087DUL))
#define PMIC_BUCK42_CTRL9_ADDR(base) ((base) + (0x087EUL))
#define PMIC_BUCK42_CTRL10_ADDR(base) ((base) + (0x087FUL))
#define PMIC_BUCK42_CTRL11_ADDR(base) ((base) + (0x0880UL))
#define PMIC_BUCK42_CTRL12_ADDR(base) ((base) + (0x0881UL))
#define PMIC_BUCK42_CTRL13_ADDR(base) ((base) + (0x0882UL))
#define PMIC_BUCK42_CTRL14_ADDR(base) ((base) + (0x0883UL))
#define PMIC_BUCK42_CTRL15_ADDR(base) ((base) + (0x0884UL))
#define PMIC_BUCK42_CTRL16_ADDR(base) ((base) + (0x0885UL))
#define PMIC_BUCK42_CTRL17_ADDR(base) ((base) + (0x0886UL))
#define PMIC_BUCK42_CTRL18_ADDR(base) ((base) + (0x0887UL))
#define PMIC_BUCK42_CTRL19_ADDR(base) ((base) + (0x0888UL))
#define PMIC_BUCK42_CTRL20_ADDR(base) ((base) + (0x0889UL))
#define PMIC_BUCK42_CTRL21_ADDR(base) ((base) + (0x088AUL))
#define PMIC_BUCK42_CTRL22_ADDR(base) ((base) + (0x088BUL))
#define PMIC_BUCK42_CTRL23_ADDR(base) ((base) + (0x088CUL))
#define PMIC_BUCK42_CTRL24_ADDR(base) ((base) + (0x088DUL))
#define PMIC_BUCK42_CTRL25_ADDR(base) ((base) + (0x088EUL))
#define PMIC_BUCK42_CTRL26_ADDR(base) ((base) + (0x088FUL))
#define PMIC_BUCK42_CTRL27_ADDR(base) ((base) + (0x0890UL))
#define PMIC_BUCK42_CTRL28_ADDR(base) ((base) + (0x0891UL))
#define PMIC_BUCK42_CTRL29_ADDR(base) ((base) + (0x0892UL))
#define PMIC_BUCK42_CTRL30_ADDR(base) ((base) + (0x0893UL))
#define PMIC_BUCK42_CTRL31_ADDR(base) ((base) + (0x0894UL))
#define PMIC_BUCK42_CTRL32_ADDR(base) ((base) + (0x0895UL))
#define PMIC_BUCK42_CTRL33_ADDR(base) ((base) + (0x0896UL))
#define PMIC_BUCK42_CTRL34_ADDR(base) ((base) + (0x0897UL))
#define PMIC_BUCK42_CTRL35_ADDR(base) ((base) + (0x0898UL))
#define PMIC_BUCK42_CTRL36_ADDR(base) ((base) + (0x0899UL))
#define PMIC_BUCK42_CTRL37_ADDR(base) ((base) + (0x089AUL))
#define PMIC_BUCK42_CTRL38_ADDR(base) ((base) + (0x089BUL))
#define PMIC_BUCK42_CTRL39_ADDR(base) ((base) + (0x089CUL))
#define PMIC_BUCK42_CTRL40_ADDR(base) ((base) + (0x089DUL))
#define PMIC_BUCK42_CTRL41_ADDR(base) ((base) + (0x089EUL))
#define PMIC_BUCK42_CTRL42_ADDR(base) ((base) + (0x089FUL))
#define PMIC_BUCK42_CTRL43_ADDR(base) ((base) + (0x08A0UL))
#define PMIC_BUCK42_CTRL44_ADDR(base) ((base) + (0x08A1UL))
#define PMIC_BUCK5_CTRL0_ADDR(base) ((base) + (0x08A2UL))
#define PMIC_BUCK5_CTRL1_ADDR(base) ((base) + (0x08A3UL))
#define PMIC_BUCK5_CTRL2_ADDR(base) ((base) + (0x08A4UL))
#define PMIC_BUCK5_CTRL3_ADDR(base) ((base) + (0x08A5UL))
#define PMIC_BUCK5_CTRL4_ADDR(base) ((base) + (0x08A6UL))
#define PMIC_BUCK5_CTRL5_ADDR(base) ((base) + (0x08A7UL))
#define PMIC_BUCK5_CTRL6_ADDR(base) ((base) + (0x08A8UL))
#define PMIC_BUCK5_CTRL7_ADDR(base) ((base) + (0x08A9UL))
#define PMIC_BUCK5_CTRL8_ADDR(base) ((base) + (0x08AAUL))
#define PMIC_BUCK5_CTRL9_ADDR(base) ((base) + (0x08ABUL))
#define PMIC_BUCK5_CTRL10_ADDR(base) ((base) + (0x08ACUL))
#define PMIC_BUCK5_CTRL11_ADDR(base) ((base) + (0x08ADUL))
#define PMIC_BUCK5_CTRL12_ADDR(base) ((base) + (0x08AEUL))
#define PMIC_BUCK5_CTRL13_ADDR(base) ((base) + (0x08AFUL))
#define PMIC_BUCK5_CTRL14_ADDR(base) ((base) + (0x08B0UL))
#define PMIC_BUCK70_CTRL0_ADDR(base) ((base) + (0x08B1UL))
#define PMIC_BUCK70_CTRL1_ADDR(base) ((base) + (0x08B2UL))
#define PMIC_BUCK70_CTRL2_ADDR(base) ((base) + (0x08B3UL))
#define PMIC_BUCK70_CTRL3_ADDR(base) ((base) + (0x08B4UL))
#define PMIC_BUCK70_CTRL4_ADDR(base) ((base) + (0x08B5UL))
#define PMIC_BUCK70_CTRL5_ADDR(base) ((base) + (0x08B6UL))
#define PMIC_BUCK70_CTRL6_ADDR(base) ((base) + (0x08B7UL))
#define PMIC_BUCK70_CTRL7_ADDR(base) ((base) + (0x08B8UL))
#define PMIC_BUCK70_CTRL8_ADDR(base) ((base) + (0x08B9UL))
#define PMIC_BUCK70_CTRL9_ADDR(base) ((base) + (0x08BAUL))
#define PMIC_BUCK70_CTRL10_ADDR(base) ((base) + (0x08BBUL))
#define PMIC_BUCK70_CTRL11_ADDR(base) ((base) + (0x08BCUL))
#define PMIC_BUCK70_CTRL12_ADDR(base) ((base) + (0x08BDUL))
#define PMIC_BUCK70_CTRL13_ADDR(base) ((base) + (0x08BEUL))
#define PMIC_BUCK70_CTRL14_ADDR(base) ((base) + (0x08BFUL))
#define PMIC_BUCK70_CTRL15_ADDR(base) ((base) + (0x08C0UL))
#define PMIC_BUCK70_CTRL16_ADDR(base) ((base) + (0x08C1UL))
#define PMIC_BUCK70_CTRL17_ADDR(base) ((base) + (0x08C2UL))
#define PMIC_BUCK70_CTRL18_ADDR(base) ((base) + (0x08C3UL))
#define PMIC_BUCK70_CTRL19_ADDR(base) ((base) + (0x08C4UL))
#define PMIC_BUCK70_CTRL20_ADDR(base) ((base) + (0x08C5UL))
#define PMIC_BUCK70_CTRL21_ADDR(base) ((base) + (0x08C6UL))
#define PMIC_BUCK70_CTRL22_ADDR(base) ((base) + (0x08C7UL))
#define PMIC_BUCK70_CTRL23_ADDR(base) ((base) + (0x08C8UL))
#define PMIC_BUCK70_CTRL24_ADDR(base) ((base) + (0x08C9UL))
#define PMIC_BUCK70_CTRL25_ADDR(base) ((base) + (0x08CAUL))
#define PMIC_BUCK70_CTRL26_ADDR(base) ((base) + (0x08CBUL))
#define PMIC_BUCK70_CTRL27_ADDR(base) ((base) + (0x08CCUL))
#define PMIC_BUCK70_CTRL28_ADDR(base) ((base) + (0x08CDUL))
#define PMIC_BUCK70_CTRL29_ADDR(base) ((base) + (0x08CEUL))
#define PMIC_BUCK70_CTRL30_ADDR(base) ((base) + (0x08CFUL))
#define PMIC_BUCK70_CTRL31_ADDR(base) ((base) + (0x08D0UL))
#define PMIC_BUCK70_CTRL32_ADDR(base) ((base) + (0x08D1UL))
#define PMIC_BUCK70_CTRL33_ADDR(base) ((base) + (0x08D2UL))
#define PMIC_BUCK70_CTRL34_ADDR(base) ((base) + (0x08D3UL))
#define PMIC_BUCK70_CTRL35_ADDR(base) ((base) + (0x08D4UL))
#define PMIC_BUCK70_CTRL36_ADDR(base) ((base) + (0x08D5UL))
#define PMIC_BUCK70_CTRL37_ADDR(base) ((base) + (0x08D6UL))
#define PMIC_BUCK70_CTRL38_ADDR(base) ((base) + (0x08D7UL))
#define PMIC_BUCK70_CTRL39_ADDR(base) ((base) + (0x08D8UL))
#define PMIC_BUCK70_CTRL40_ADDR(base) ((base) + (0x08D9UL))
#define PMIC_BUCK70_CTRL41_ADDR(base) ((base) + (0x08DAUL))
#define PMIC_BUCK70_CTRL42_ADDR(base) ((base) + (0x08DBUL))
#define PMIC_BUCK70_CTRL43_ADDR(base) ((base) + (0x08DCUL))
#define PMIC_BUCK70_CTRL44_ADDR(base) ((base) + (0x08DDUL))
#define PMIC_BUCK70_CTRL45_ADDR(base) ((base) + (0x08DEUL))
#define PMIC_BUCK70_CTRL46_ADDR(base) ((base) + (0x08DFUL))
#define PMIC_BUCK70_CTRL47_ADDR(base) ((base) + (0x08E0UL))
#define PMIC_BUCK70_CTRL48_ADDR(base) ((base) + (0x08E1UL))
#define PMIC_BUCK70_CTRL49_ADDR(base) ((base) + (0x08E2UL))
#define PMIC_BUCK70_CTRL50_ADDR(base) ((base) + (0x08E3UL))
#define PMIC_BUCK70_CTRL51_ADDR(base) ((base) + (0x08E4UL))
#define PMIC_BUCK70_CTRL52_ADDR(base) ((base) + (0x08E5UL))
#define PMIC_BUCK70_CTRL53_ADDR(base) ((base) + (0x08E6UL))
#define PMIC_BUCK70_CTRL54_ADDR(base) ((base) + (0x08E7UL))
#define PMIC_BUCK701_CTRL0_ADDR(base) ((base) + (0x08E8UL))
#define PMIC_BUCK701_CTRL1_ADDR(base) ((base) + (0x08E9UL))
#define PMIC_BUCK701_CTRL2_ADDR(base) ((base) + (0x08EAUL))
#define PMIC_BUCK701_CTRL3_ADDR(base) ((base) + (0x08EBUL))
#define PMIC_BUCK701_CTRL4_ADDR(base) ((base) + (0x08ECUL))
#define PMIC_BUCK71_CTRL0_ADDR(base) ((base) + (0x08EDUL))
#define PMIC_BUCK71_CTRL1_ADDR(base) ((base) + (0x08EEUL))
#define PMIC_BUCK71_CTRL2_ADDR(base) ((base) + (0x08EFUL))
#define PMIC_BUCK71_CTRL3_ADDR(base) ((base) + (0x08F0UL))
#define PMIC_BUCK71_CTRL4_ADDR(base) ((base) + (0x08F1UL))
#define PMIC_BUCK71_CTRL5_ADDR(base) ((base) + (0x08F2UL))
#define PMIC_BUCK71_CTRL6_ADDR(base) ((base) + (0x08F3UL))
#define PMIC_BUCK71_CTRL7_ADDR(base) ((base) + (0x08F4UL))
#define PMIC_BUCK71_CTRL8_ADDR(base) ((base) + (0x08F5UL))
#define PMIC_BUCK71_CTRL9_ADDR(base) ((base) + (0x08F6UL))
#define PMIC_BUCK71_CTRL10_ADDR(base) ((base) + (0x08F7UL))
#define PMIC_BUCK71_CTRL11_ADDR(base) ((base) + (0x08F8UL))
#define PMIC_BUCK71_CTRL12_ADDR(base) ((base) + (0x08F9UL))
#define PMIC_BUCK71_CTRL13_ADDR(base) ((base) + (0x08FAUL))
#define PMIC_BUCK71_CTRL14_ADDR(base) ((base) + (0x08FBUL))
#define PMIC_BUCK71_CTRL15_ADDR(base) ((base) + (0x08FCUL))
#define PMIC_BUCK71_CTRL16_ADDR(base) ((base) + (0x08FDUL))
#define PMIC_BUCK71_CTRL17_ADDR(base) ((base) + (0x08FEUL))
#define PMIC_BUCK71_CTRL18_ADDR(base) ((base) + (0x08FFUL))
#define PMIC_BUCK71_CTRL19_ADDR(base) ((base) + (0x0900UL))
#define PMIC_BUCK71_CTRL20_ADDR(base) ((base) + (0x0901UL))
#define PMIC_BUCK71_CTRL21_ADDR(base) ((base) + (0x0902UL))
#define PMIC_BUCK71_CTRL22_ADDR(base) ((base) + (0x0903UL))
#define PMIC_BUCK71_CTRL23_ADDR(base) ((base) + (0x0904UL))
#define PMIC_BUCK71_CTRL24_ADDR(base) ((base) + (0x0905UL))
#define PMIC_BUCK71_CTRL25_ADDR(base) ((base) + (0x0906UL))
#define PMIC_BUCK71_CTRL26_ADDR(base) ((base) + (0x0907UL))
#define PMIC_BUCK71_CTRL27_ADDR(base) ((base) + (0x0908UL))
#define PMIC_BUCK71_CTRL28_ADDR(base) ((base) + (0x0909UL))
#define PMIC_BUCK71_CTRL29_ADDR(base) ((base) + (0x090AUL))
#define PMIC_BUCK71_CTRL30_ADDR(base) ((base) + (0x090BUL))
#define PMIC_BUCK71_CTRL31_ADDR(base) ((base) + (0x090CUL))
#define PMIC_BUCK71_CTRL32_ADDR(base) ((base) + (0x090DUL))
#define PMIC_BUCK71_CTRL33_ADDR(base) ((base) + (0x090EUL))
#define PMIC_BUCK71_CTRL34_ADDR(base) ((base) + (0x090FUL))
#define PMIC_BUCK71_CTRL35_ADDR(base) ((base) + (0x0910UL))
#define PMIC_BUCK71_CTRL36_ADDR(base) ((base) + (0x0911UL))
#define PMIC_BUCK71_CTRL37_ADDR(base) ((base) + (0x0912UL))
#define PMIC_BUCK71_CTRL38_ADDR(base) ((base) + (0x0913UL))
#define PMIC_BUCK71_CTRL39_ADDR(base) ((base) + (0x0914UL))
#define PMIC_BUCK71_CTRL40_ADDR(base) ((base) + (0x0915UL))
#define PMIC_BUCK71_CTRL41_ADDR(base) ((base) + (0x0916UL))
#define PMIC_BUCK71_CTRL42_ADDR(base) ((base) + (0x0917UL))
#define PMIC_BUCK71_CTRL43_ADDR(base) ((base) + (0x0918UL))
#define PMIC_BUCK71_CTRL44_ADDR(base) ((base) + (0x0919UL))
#define PMIC_BUCK701_RESERVE0_ADDR(base) ((base) + (0x091AUL))
#define PMIC_BUCK701_RESERVE1_ADDR(base) ((base) + (0x091BUL))
#define PMIC_BUCK701_RESERVE2_ADDR(base) ((base) + (0x091CUL))
#define PMIC_BUCK701_RESERVE3_ADDR(base) ((base) + (0x091DUL))
#define PMIC_BUCK9_CTRL0_ADDR(base) ((base) + (0x091EUL))
#define PMIC_BUCK9_CTRL1_ADDR(base) ((base) + (0x091FUL))
#define PMIC_BUCK9_CTRL2_ADDR(base) ((base) + (0x0920UL))
#define PMIC_BUCK9_CTRL3_ADDR(base) ((base) + (0x0921UL))
#define PMIC_BUCK9_CTRL4_ADDR(base) ((base) + (0x0922UL))
#define PMIC_BUCK9_CTRL5_ADDR(base) ((base) + (0x0923UL))
#define PMIC_BUCK9_CTRL6_ADDR(base) ((base) + (0x0924UL))
#define PMIC_BUCK9_CTRL7_ADDR(base) ((base) + (0x0925UL))
#define PMIC_BUCK9_CTRL8_ADDR(base) ((base) + (0x0926UL))
#define PMIC_BUCK9_CTRL9_ADDR(base) ((base) + (0x0927UL))
#define PMIC_BUCK9_CTRL10_ADDR(base) ((base) + (0x0928UL))
#define PMIC_BUCK9_CTRL11_ADDR(base) ((base) + (0x0929UL))
#define PMIC_BUCK9_CTRL12_ADDR(base) ((base) + (0x092AUL))
#define PMIC_BUCK9_CTRL13_ADDR(base) ((base) + (0x092BUL))
#define PMIC_BUCK9_CTRL14_ADDR(base) ((base) + (0x092CUL))
#define PMIC_BUCK_CTRL0_ADDR(base) ((base) + (0x092DUL))
#define PMIC_BUCK_CTRL1_ADDR(base) ((base) + (0x092EUL))
#define PMIC_BUCK_CTRL2_ADDR(base) ((base) + (0x092FUL))
#define PMIC_BUCK_RESERVE0_ADDR(base) ((base) + (0x0930UL))
#define PMIC_BUCK_RESERVE1_ADDR(base) ((base) + (0x0931UL))
#define PMIC_BUCK_RESERVE2_ADDR(base) ((base) + (0x0932UL))
#define PMIC_LDO0_CTRL0_ADDR(base) ((base) + (0x0933UL))
#define PMIC_LDO0_CTRL1_ADDR(base) ((base) + (0x0934UL))
#define PMIC_LDO0_CTRL2_ADDR(base) ((base) + (0x0935UL))
#define PMIC_LDO1_CTRL0_ADDR(base) ((base) + (0x0936UL))
#define PMIC_LDO1_CTRL1_ADDR(base) ((base) + (0x0937UL))
#define PMIC_LDO1_CTRL2_ADDR(base) ((base) + (0x0938UL))
#define PMIC_LDO1_CTRL3_ADDR(base) ((base) + (0x0939UL))
#define PMIC_LDO3_CTRL0_ADDR(base) ((base) + (0x093AUL))
#define PMIC_LDO3_CTRL1_ADDR(base) ((base) + (0x093BUL))
#define PMIC_LDO4_CTRL0_ADDR(base) ((base) + (0x093CUL))
#define PMIC_LDO4_CTRL1_ADDR(base) ((base) + (0x093DUL))
#define PMIC_LDO6_CTRL0_ADDR(base) ((base) + (0x093EUL))
#define PMIC_LDO6_CTRL1_ADDR(base) ((base) + (0x093FUL))
#define PMIC_LDO6_CTRL2_ADDR(base) ((base) + (0x0940UL))
#define PMIC_LDO8_CTRL_ADDR(base) ((base) + (0x0941UL))
#define PMIC_LDO11_CTRL_ADDR(base) ((base) + (0x0942UL))
#define PMIC_LDO12_CTRL_ADDR(base) ((base) + (0x0943UL))
#define PMIC_LDO14_CTRL_ADDR(base) ((base) + (0x0944UL))
#define PMIC_LDO15_CTRL_ADDR(base) ((base) + (0x0945UL))
#define PMIC_LDO16_CTRL_ADDR(base) ((base) + (0x0946UL))
#define PMIC_LDO17_CTRL_ADDR(base) ((base) + (0x0947UL))
#define PMIC_LDO18_CTRL_ADDR(base) ((base) + (0x0948UL))
#define PMIC_LDO21_CTRL0_ADDR(base) ((base) + (0x0949UL))
#define PMIC_LDO21_CTRL1_ADDR(base) ((base) + (0x094AUL))
#define PMIC_LDO22_CTRL0_ADDR(base) ((base) + (0x094BUL))
#define PMIC_LDO22_CTRL1_ADDR(base) ((base) + (0x094CUL))
#define PMIC_LDO22_CTRL2_ADDR(base) ((base) + (0x094DUL))
#define PMIC_LDO22_CTRL3_ADDR(base) ((base) + (0x094EUL))
#define PMIC_LDO23_CTRL_ADDR(base) ((base) + (0x094FUL))
#define PMIC_LDO24_CTRL_ADDR(base) ((base) + (0x0950UL))
#define PMIC_LDO25_CTRL0_ADDR(base) ((base) + (0x0951UL))
#define PMIC_LDO25_CTRL1_ADDR(base) ((base) + (0x0952UL))
#define PMIC_LDO25_CTRL2_ADDR(base) ((base) + (0x0953UL))
#define PMIC_LDO25_CTRL3_ADDR(base) ((base) + (0x0954UL))
#define PMIC_LDO26_CTRL_ADDR(base) ((base) + (0x0955UL))
#define PMIC_LDO27_CTRL_ADDR(base) ((base) + (0x0956UL))
#define PMIC_LDO28_CTRL_ADDR(base) ((base) + (0x0957UL))
#define PMIC_LDO29_CTRL0_ADDR(base) ((base) + (0x0958UL))
#define PMIC_LDO29_CTRL1_ADDR(base) ((base) + (0x0959UL))
#define PMIC_LDO29_CTRL2_ADDR(base) ((base) + (0x095AUL))
#define PMIC_LDO29_CTRL3_ADDR(base) ((base) + (0x095BUL))
#define PMIC_LDO30_CTRL0_ADDR(base) ((base) + (0x095CUL))
#define PMIC_LDO30_CTRL1_ADDR(base) ((base) + (0x095DUL))
#define PMIC_LDO30_CTRL2_ADDR(base) ((base) + (0x095EUL))
#define PMIC_LDO32_CTRL0_ADDR(base) ((base) + (0x095FUL))
#define PMIC_LDO32_CTRL1_ADDR(base) ((base) + (0x0960UL))
#define PMIC_LDO32_CTRL2_ADDR(base) ((base) + (0x0961UL))
#define PMIC_LDO32_CTRL3_ADDR(base) ((base) + (0x0962UL))
#define PMIC_PMUH_CTRL_ADDR(base) ((base) + (0x0963UL))
#define PMIC_LDO36_CTRL0_ADDR(base) ((base) + (0x0964UL))
#define PMIC_LDO36_CTRL1_ADDR(base) ((base) + (0x0965UL))
#define PMIC_LDO37_CTRL0_ADDR(base) ((base) + (0x0966UL))
#define PMIC_LDO37_CTRL1_ADDR(base) ((base) + (0x0967UL))
#define PMIC_LDO38_CTRL0_ADDR(base) ((base) + (0x0968UL))
#define PMIC_LDO38_CTRL1_ADDR(base) ((base) + (0x0969UL))
#define PMIC_LDO38_CTRL2_ADDR(base) ((base) + (0x096AUL))
#define PMIC_LDO38_CTRL3_ADDR(base) ((base) + (0x096BUL))
#define PMIC_LDO41_CTRL0_ADDR(base) ((base) + (0x096CUL))
#define PMIC_LDO41_CTRL1_ADDR(base) ((base) + (0x096DUL))
#define PMIC_LDO41_CTRL2_ADDR(base) ((base) + (0x096EUL))
#define PMIC_LDO41_CTRL3_ADDR(base) ((base) + (0x096FUL))
#define PMIC_LDO42_CTRL0_ADDR(base) ((base) + (0x0970UL))
#define PMIC_LDO42_CTRL1_ADDR(base) ((base) + (0x0971UL))
#define PMIC_LDO42_CTRL2_ADDR(base) ((base) + (0x0972UL))
#define PMIC_LDO43_CTRL0_ADDR(base) ((base) + (0x0973UL))
#define PMIC_LDO43_CTRL1_ADDR(base) ((base) + (0x0974UL))
#define PMIC_LDO43_CTRL2_ADDR(base) ((base) + (0x0975UL))
#define PMIC_LDO44_CTRL0_ADDR(base) ((base) + (0x0976UL))
#define PMIC_LDO44_CTRL1_ADDR(base) ((base) + (0x0977UL))
#define PMIC_LDO44_CTRL2_ADDR(base) ((base) + (0x0978UL))
#define PMIC_LDO45_CTRL0_ADDR(base) ((base) + (0x0979UL))
#define PMIC_LDO45_CTRL1_ADDR(base) ((base) + (0x097AUL))
#define PMIC_LDO45_CTRL2_ADDR(base) ((base) + (0x097BUL))
#define PMIC_LDO46_CTRL0_ADDR(base) ((base) + (0x097CUL))
#define PMIC_LDO46_CTRL1_ADDR(base) ((base) + (0x097DUL))
#define PMIC_SW1_CTRL_ADDR(base) ((base) + (0x097EUL))
#define PMIC_LDO_SINK_CTRL_ADDR(base) ((base) + (0x097FUL))
#define PMIC_LDO_BUF_REF_CTRL_ADDR(base) ((base) + (0x0980UL))
#define PMIC_LDO_RESERVE0_ADDR(base) ((base) + (0x0981UL))
#define PMIC_LDO_RESERVE1_ADDR(base) ((base) + (0x0982UL))
#define PMIC_LDO_RESERVE2_ADDR(base) ((base) + (0x0983UL))
#define PMIC_LDO_RESERVE3_ADDR(base) ((base) + (0x0984UL))
#define PMIC_LDO_RESERVE4_ADDR(base) ((base) + (0x0985UL))
#define PMIC_LDO_RESERVE5_ADDR(base) ((base) + (0x0986UL))
#define PMIC_LDO_RESERVE6_ADDR(base) ((base) + (0x0987UL))
#define PMIC_LDO_RESERVE7_ADDR(base) ((base) + (0x0988UL))
#define PMIC_LDO_RESERVE8_ADDR(base) ((base) + (0x0989UL))
#define PMIC_LDO_RESERVE9_ADDR(base) ((base) + (0x098AUL))
#define PMIC_D2A_RES0_ADDR(base) ((base) + (0x098BUL))
#define PMIC_D2A_RES1_ADDR(base) ((base) + (0x098CUL))
#define PMIC_A2D_RES0_ADDR(base) ((base) + (0x098DUL))
#else
#define PMIC_DUMMY_SPMI_ADDR(base) ((base) + (0x0800))
#define PMIC_BUCK1_CTRL0_ADDR(base) ((base) + (0x0801))
#define PMIC_BUCK1_CTRL1_ADDR(base) ((base) + (0x0802))
#define PMIC_BUCK1_CTRL2_ADDR(base) ((base) + (0x0803))
#define PMIC_BUCK1_CTRL3_ADDR(base) ((base) + (0x0804))
#define PMIC_BUCK1_CTRL4_ADDR(base) ((base) + (0x0805))
#define PMIC_BUCK1_CTRL5_ADDR(base) ((base) + (0x0806))
#define PMIC_BUCK1_CTRL6_ADDR(base) ((base) + (0x0807))
#define PMIC_BUCK1_CTRL7_ADDR(base) ((base) + (0x0808))
#define PMIC_BUCK1_CTRL8_ADDR(base) ((base) + (0x0809))
#define PMIC_BUCK1_CTRL9_ADDR(base) ((base) + (0x080A))
#define PMIC_BUCK1_CTRL10_ADDR(base) ((base) + (0x080B))
#define PMIC_BUCK1_CTRL11_ADDR(base) ((base) + (0x080C))
#define PMIC_BUCK1_CTRL12_ADDR(base) ((base) + (0x080D))
#define PMIC_BUCK1_CTRL13_ADDR(base) ((base) + (0x080E))
#define PMIC_BUCK1_CTRL14_ADDR(base) ((base) + (0x080F))
#define PMIC_BUCK2_CTRL0_ADDR(base) ((base) + (0x0810))
#define PMIC_BUCK2_CTRL1_ADDR(base) ((base) + (0x0811))
#define PMIC_BUCK2_CTRL2_ADDR(base) ((base) + (0x0812))
#define PMIC_BUCK2_CTRL3_ADDR(base) ((base) + (0x0813))
#define PMIC_BUCK2_CTRL4_ADDR(base) ((base) + (0x0814))
#define PMIC_BUCK2_CTRL5_ADDR(base) ((base) + (0x0815))
#define PMIC_BUCK2_CTRL6_ADDR(base) ((base) + (0x0816))
#define PMIC_BUCK2_CTRL7_ADDR(base) ((base) + (0x0817))
#define PMIC_BUCK2_CTRL8_ADDR(base) ((base) + (0x0818))
#define PMIC_BUCK2_CTRL9_ADDR(base) ((base) + (0x0819))
#define PMIC_BUCK2_CTRL10_ADDR(base) ((base) + (0x081A))
#define PMIC_BUCK2_CTRL11_ADDR(base) ((base) + (0x081B))
#define PMIC_BUCK2_CTRL12_ADDR(base) ((base) + (0x081C))
#define PMIC_BUCK2_CTRL13_ADDR(base) ((base) + (0x081D))
#define PMIC_BUCK3_CTRL0_ADDR(base) ((base) + (0x081E))
#define PMIC_BUCK3_CTRL1_ADDR(base) ((base) + (0x081F))
#define PMIC_BUCK3_CTRL2_ADDR(base) ((base) + (0x0820))
#define PMIC_BUCK3_CTRL3_ADDR(base) ((base) + (0x0821))
#define PMIC_BUCK3_CTRL4_ADDR(base) ((base) + (0x0822))
#define PMIC_BUCK3_CTRL5_ADDR(base) ((base) + (0x0823))
#define PMIC_BUCK3_CTRL6_ADDR(base) ((base) + (0x0824))
#define PMIC_BUCK3_CTRL7_ADDR(base) ((base) + (0x0825))
#define PMIC_BUCK3_CTRL8_ADDR(base) ((base) + (0x0826))
#define PMIC_BUCK3_CTRL9_ADDR(base) ((base) + (0x0827))
#define PMIC_BUCK3_CTRL10_ADDR(base) ((base) + (0x0828))
#define PMIC_BUCK3_CTRL11_ADDR(base) ((base) + (0x0829))
#define PMIC_BUCK3_CTRL12_ADDR(base) ((base) + (0x082A))
#define PMIC_BUCK3_CTRL13_ADDR(base) ((base) + (0x082B))
#define PMIC_BUCK401_CTRL0_ADDR(base) ((base) + (0x082C))
#define PMIC_BUCK401_CTRL1_ADDR(base) ((base) + (0x082D))
#define PMIC_BUCK401_CTRL2_ADDR(base) ((base) + (0x082E))
#define PMIC_BUCK401_CTRL3_ADDR(base) ((base) + (0x082F))
#define PMIC_BUCK401_CTRL4_ADDR(base) ((base) + (0x0830))
#define PMIC_BUCK401_CTRL5_ADDR(base) ((base) + (0x0831))
#define PMIC_BUCK401_CTRL6_ADDR(base) ((base) + (0x0832))
#define PMIC_BUCK401_CTRL7_ADDR(base) ((base) + (0x0833))
#define PMIC_BUCK401_CTRL8_ADDR(base) ((base) + (0x0834))
#define PMIC_BUCK401_CTRL9_ADDR(base) ((base) + (0x0835))
#define PMIC_BUCK401_CTRL10_ADDR(base) ((base) + (0x0836))
#define PMIC_BUCK401_CTRL11_ADDR(base) ((base) + (0x0837))
#define PMIC_BUCK401_CTRL12_ADDR(base) ((base) + (0x0838))
#define PMIC_BUCK401_CTRL13_ADDR(base) ((base) + (0x0839))
#define PMIC_BUCK401_CTRL14_ADDR(base) ((base) + (0x083A))
#define PMIC_BUCK401_CTRL15_ADDR(base) ((base) + (0x083B))
#define PMIC_BUCK401_CTRL16_ADDR(base) ((base) + (0x083C))
#define PMIC_BUCK401_CTRL17_ADDR(base) ((base) + (0x083D))
#define PMIC_BUCK401_CTRL18_ADDR(base) ((base) + (0x083E))
#define PMIC_BUCK401_CTRL19_ADDR(base) ((base) + (0x083F))
#define PMIC_BUCK401_CTRL20_ADDR(base) ((base) + (0x0840))
#define PMIC_BUCK401_CTRL21_ADDR(base) ((base) + (0x0841))
#define PMIC_BUCK401_CTRL22_ADDR(base) ((base) + (0x0842))
#define PMIC_BUCK401_CTRL23_ADDR(base) ((base) + (0x0843))
#define PMIC_BUCK401_CTRL24_ADDR(base) ((base) + (0x0844))
#define PMIC_BUCK401_CTRL25_ADDR(base) ((base) + (0x0845))
#define PMIC_BUCK401_CTRL26_ADDR(base) ((base) + (0x0846))
#define PMIC_BUCK401_CTRL27_ADDR(base) ((base) + (0x0847))
#define PMIC_BUCK401_CTRL28_ADDR(base) ((base) + (0x0848))
#define PMIC_BUCK401_CTRL29_ADDR(base) ((base) + (0x0849))
#define PMIC_BUCK401_CTRL30_ADDR(base) ((base) + (0x084A))
#define PMIC_BUCK401_CTRL31_ADDR(base) ((base) + (0x084B))
#define PMIC_BUCK401_CTRL32_ADDR(base) ((base) + (0x084C))
#define PMIC_BUCK401_CTRL33_ADDR(base) ((base) + (0x084D))
#define PMIC_BUCK401_CTRL34_ADDR(base) ((base) + (0x084E))
#define PMIC_BUCK401_CTRL35_ADDR(base) ((base) + (0x084F))
#define PMIC_BUCK401_CTRL36_ADDR(base) ((base) + (0x0850))
#define PMIC_BUCK401_CTRL37_ADDR(base) ((base) + (0x0851))
#define PMIC_BUCK401_CTRL38_ADDR(base) ((base) + (0x0852))
#define PMIC_BUCK401_CTRL39_ADDR(base) ((base) + (0x0853))
#define PMIC_BUCK401_CTRL40_ADDR(base) ((base) + (0x0854))
#define PMIC_BUCK401_CTRL41_ADDR(base) ((base) + (0x0855))
#define PMIC_BUCK4012_CTRL0_ADDR(base) ((base) + (0x0856))
#define PMIC_BUCK4012_CTRL1_ADDR(base) ((base) + (0x0857))
#define PMIC_BUCK4012_CTRL2_ADDR(base) ((base) + (0x0858))
#define PMIC_BUCK4012_CTRL3_ADDR(base) ((base) + (0x0859))
#define PMIC_BUCK4012_CTRL4_ADDR(base) ((base) + (0x085A))
#define PMIC_BUCK4012_RESERVE0_ADDR(base) ((base) + (0x085B))
#define PMIC_BUCK4012_RESERVE1_ADDR(base) ((base) + (0x085C))
#define PMIC_BUCK4012_RESERVE2_ADDR(base) ((base) + (0x085D))
#define PMIC_BUCK4012_RESERVE3_ADDR(base) ((base) + (0x085E))
#define PMIC_BUCK40_CTRL0_ADDR(base) ((base) + (0x085F))
#define PMIC_BUCK40_CTRL1_ADDR(base) ((base) + (0x0860))
#define PMIC_BUCK40_CTRL2_ADDR(base) ((base) + (0x0861))
#define PMIC_BUCK40_CTRL3_ADDR(base) ((base) + (0x0862))
#define PMIC_BUCK40_CTRL4_ADDR(base) ((base) + (0x0863))
#define PMIC_BUCK40_CTRL5_ADDR(base) ((base) + (0x0864))
#define PMIC_BUCK40_CTRL6_ADDR(base) ((base) + (0x0865))
#define PMIC_BUCK40_CTRL7_ADDR(base) ((base) + (0x0866))
#define PMIC_BUCK40_CTRL8_ADDR(base) ((base) + (0x0867))
#define PMIC_BUCK40_CTRL9_ADDR(base) ((base) + (0x0868))
#define PMIC_BUCK40_CTRL10_ADDR(base) ((base) + (0x0869))
#define PMIC_BUCK41_CTRL0_ADDR(base) ((base) + (0x086A))
#define PMIC_BUCK41_CTRL1_ADDR(base) ((base) + (0x086B))
#define PMIC_BUCK41_CTRL2_ADDR(base) ((base) + (0x086C))
#define PMIC_BUCK41_CTRL3_ADDR(base) ((base) + (0x086D))
#define PMIC_BUCK41_CTRL4_ADDR(base) ((base) + (0x086E))
#define PMIC_BUCK41_CTRL5_ADDR(base) ((base) + (0x086F))
#define PMIC_BUCK41_CTRL6_ADDR(base) ((base) + (0x0870))
#define PMIC_BUCK41_CTRL7_ADDR(base) ((base) + (0x0871))
#define PMIC_BUCK41_CTRL8_ADDR(base) ((base) + (0x0872))
#define PMIC_BUCK41_CTRL9_ADDR(base) ((base) + (0x0873))
#define PMIC_BUCK41_CTRL10_ADDR(base) ((base) + (0x0874))
#define PMIC_BUCK42_CTRL0_ADDR(base) ((base) + (0x0875))
#define PMIC_BUCK42_CTRL1_ADDR(base) ((base) + (0x0876))
#define PMIC_BUCK42_CTRL2_ADDR(base) ((base) + (0x0877))
#define PMIC_BUCK42_CTRL3_ADDR(base) ((base) + (0x0878))
#define PMIC_BUCK42_CTRL4_ADDR(base) ((base) + (0x0879))
#define PMIC_BUCK42_CTRL5_ADDR(base) ((base) + (0x087A))
#define PMIC_BUCK42_CTRL6_ADDR(base) ((base) + (0x087B))
#define PMIC_BUCK42_CTRL7_ADDR(base) ((base) + (0x087C))
#define PMIC_BUCK42_CTRL8_ADDR(base) ((base) + (0x087D))
#define PMIC_BUCK42_CTRL9_ADDR(base) ((base) + (0x087E))
#define PMIC_BUCK42_CTRL10_ADDR(base) ((base) + (0x087F))
#define PMIC_BUCK42_CTRL11_ADDR(base) ((base) + (0x0880))
#define PMIC_BUCK42_CTRL12_ADDR(base) ((base) + (0x0881))
#define PMIC_BUCK42_CTRL13_ADDR(base) ((base) + (0x0882))
#define PMIC_BUCK42_CTRL14_ADDR(base) ((base) + (0x0883))
#define PMIC_BUCK42_CTRL15_ADDR(base) ((base) + (0x0884))
#define PMIC_BUCK42_CTRL16_ADDR(base) ((base) + (0x0885))
#define PMIC_BUCK42_CTRL17_ADDR(base) ((base) + (0x0886))
#define PMIC_BUCK42_CTRL18_ADDR(base) ((base) + (0x0887))
#define PMIC_BUCK42_CTRL19_ADDR(base) ((base) + (0x0888))
#define PMIC_BUCK42_CTRL20_ADDR(base) ((base) + (0x0889))
#define PMIC_BUCK42_CTRL21_ADDR(base) ((base) + (0x088A))
#define PMIC_BUCK42_CTRL22_ADDR(base) ((base) + (0x088B))
#define PMIC_BUCK42_CTRL23_ADDR(base) ((base) + (0x088C))
#define PMIC_BUCK42_CTRL24_ADDR(base) ((base) + (0x088D))
#define PMIC_BUCK42_CTRL25_ADDR(base) ((base) + (0x088E))
#define PMIC_BUCK42_CTRL26_ADDR(base) ((base) + (0x088F))
#define PMIC_BUCK42_CTRL27_ADDR(base) ((base) + (0x0890))
#define PMIC_BUCK42_CTRL28_ADDR(base) ((base) + (0x0891))
#define PMIC_BUCK42_CTRL29_ADDR(base) ((base) + (0x0892))
#define PMIC_BUCK42_CTRL30_ADDR(base) ((base) + (0x0893))
#define PMIC_BUCK42_CTRL31_ADDR(base) ((base) + (0x0894))
#define PMIC_BUCK42_CTRL32_ADDR(base) ((base) + (0x0895))
#define PMIC_BUCK42_CTRL33_ADDR(base) ((base) + (0x0896))
#define PMIC_BUCK42_CTRL34_ADDR(base) ((base) + (0x0897))
#define PMIC_BUCK42_CTRL35_ADDR(base) ((base) + (0x0898))
#define PMIC_BUCK42_CTRL36_ADDR(base) ((base) + (0x0899))
#define PMIC_BUCK42_CTRL37_ADDR(base) ((base) + (0x089A))
#define PMIC_BUCK42_CTRL38_ADDR(base) ((base) + (0x089B))
#define PMIC_BUCK42_CTRL39_ADDR(base) ((base) + (0x089C))
#define PMIC_BUCK42_CTRL40_ADDR(base) ((base) + (0x089D))
#define PMIC_BUCK42_CTRL41_ADDR(base) ((base) + (0x089E))
#define PMIC_BUCK42_CTRL42_ADDR(base) ((base) + (0x089F))
#define PMIC_BUCK42_CTRL43_ADDR(base) ((base) + (0x08A0))
#define PMIC_BUCK42_CTRL44_ADDR(base) ((base) + (0x08A1))
#define PMIC_BUCK5_CTRL0_ADDR(base) ((base) + (0x08A2))
#define PMIC_BUCK5_CTRL1_ADDR(base) ((base) + (0x08A3))
#define PMIC_BUCK5_CTRL2_ADDR(base) ((base) + (0x08A4))
#define PMIC_BUCK5_CTRL3_ADDR(base) ((base) + (0x08A5))
#define PMIC_BUCK5_CTRL4_ADDR(base) ((base) + (0x08A6))
#define PMIC_BUCK5_CTRL5_ADDR(base) ((base) + (0x08A7))
#define PMIC_BUCK5_CTRL6_ADDR(base) ((base) + (0x08A8))
#define PMIC_BUCK5_CTRL7_ADDR(base) ((base) + (0x08A9))
#define PMIC_BUCK5_CTRL8_ADDR(base) ((base) + (0x08AA))
#define PMIC_BUCK5_CTRL9_ADDR(base) ((base) + (0x08AB))
#define PMIC_BUCK5_CTRL10_ADDR(base) ((base) + (0x08AC))
#define PMIC_BUCK5_CTRL11_ADDR(base) ((base) + (0x08AD))
#define PMIC_BUCK5_CTRL12_ADDR(base) ((base) + (0x08AE))
#define PMIC_BUCK5_CTRL13_ADDR(base) ((base) + (0x08AF))
#define PMIC_BUCK5_CTRL14_ADDR(base) ((base) + (0x08B0))
#define PMIC_BUCK70_CTRL0_ADDR(base) ((base) + (0x08B1))
#define PMIC_BUCK70_CTRL1_ADDR(base) ((base) + (0x08B2))
#define PMIC_BUCK70_CTRL2_ADDR(base) ((base) + (0x08B3))
#define PMIC_BUCK70_CTRL3_ADDR(base) ((base) + (0x08B4))
#define PMIC_BUCK70_CTRL4_ADDR(base) ((base) + (0x08B5))
#define PMIC_BUCK70_CTRL5_ADDR(base) ((base) + (0x08B6))
#define PMIC_BUCK70_CTRL6_ADDR(base) ((base) + (0x08B7))
#define PMIC_BUCK70_CTRL7_ADDR(base) ((base) + (0x08B8))
#define PMIC_BUCK70_CTRL8_ADDR(base) ((base) + (0x08B9))
#define PMIC_BUCK70_CTRL9_ADDR(base) ((base) + (0x08BA))
#define PMIC_BUCK70_CTRL10_ADDR(base) ((base) + (0x08BB))
#define PMIC_BUCK70_CTRL11_ADDR(base) ((base) + (0x08BC))
#define PMIC_BUCK70_CTRL12_ADDR(base) ((base) + (0x08BD))
#define PMIC_BUCK70_CTRL13_ADDR(base) ((base) + (0x08BE))
#define PMIC_BUCK70_CTRL14_ADDR(base) ((base) + (0x08BF))
#define PMIC_BUCK70_CTRL15_ADDR(base) ((base) + (0x08C0))
#define PMIC_BUCK70_CTRL16_ADDR(base) ((base) + (0x08C1))
#define PMIC_BUCK70_CTRL17_ADDR(base) ((base) + (0x08C2))
#define PMIC_BUCK70_CTRL18_ADDR(base) ((base) + (0x08C3))
#define PMIC_BUCK70_CTRL19_ADDR(base) ((base) + (0x08C4))
#define PMIC_BUCK70_CTRL20_ADDR(base) ((base) + (0x08C5))
#define PMIC_BUCK70_CTRL21_ADDR(base) ((base) + (0x08C6))
#define PMIC_BUCK70_CTRL22_ADDR(base) ((base) + (0x08C7))
#define PMIC_BUCK70_CTRL23_ADDR(base) ((base) + (0x08C8))
#define PMIC_BUCK70_CTRL24_ADDR(base) ((base) + (0x08C9))
#define PMIC_BUCK70_CTRL25_ADDR(base) ((base) + (0x08CA))
#define PMIC_BUCK70_CTRL26_ADDR(base) ((base) + (0x08CB))
#define PMIC_BUCK70_CTRL27_ADDR(base) ((base) + (0x08CC))
#define PMIC_BUCK70_CTRL28_ADDR(base) ((base) + (0x08CD))
#define PMIC_BUCK70_CTRL29_ADDR(base) ((base) + (0x08CE))
#define PMIC_BUCK70_CTRL30_ADDR(base) ((base) + (0x08CF))
#define PMIC_BUCK70_CTRL31_ADDR(base) ((base) + (0x08D0))
#define PMIC_BUCK70_CTRL32_ADDR(base) ((base) + (0x08D1))
#define PMIC_BUCK70_CTRL33_ADDR(base) ((base) + (0x08D2))
#define PMIC_BUCK70_CTRL34_ADDR(base) ((base) + (0x08D3))
#define PMIC_BUCK70_CTRL35_ADDR(base) ((base) + (0x08D4))
#define PMIC_BUCK70_CTRL36_ADDR(base) ((base) + (0x08D5))
#define PMIC_BUCK70_CTRL37_ADDR(base) ((base) + (0x08D6))
#define PMIC_BUCK70_CTRL38_ADDR(base) ((base) + (0x08D7))
#define PMIC_BUCK70_CTRL39_ADDR(base) ((base) + (0x08D8))
#define PMIC_BUCK70_CTRL40_ADDR(base) ((base) + (0x08D9))
#define PMIC_BUCK70_CTRL41_ADDR(base) ((base) + (0x08DA))
#define PMIC_BUCK70_CTRL42_ADDR(base) ((base) + (0x08DB))
#define PMIC_BUCK70_CTRL43_ADDR(base) ((base) + (0x08DC))
#define PMIC_BUCK70_CTRL44_ADDR(base) ((base) + (0x08DD))
#define PMIC_BUCK70_CTRL45_ADDR(base) ((base) + (0x08DE))
#define PMIC_BUCK70_CTRL46_ADDR(base) ((base) + (0x08DF))
#define PMIC_BUCK70_CTRL47_ADDR(base) ((base) + (0x08E0))
#define PMIC_BUCK70_CTRL48_ADDR(base) ((base) + (0x08E1))
#define PMIC_BUCK70_CTRL49_ADDR(base) ((base) + (0x08E2))
#define PMIC_BUCK70_CTRL50_ADDR(base) ((base) + (0x08E3))
#define PMIC_BUCK70_CTRL51_ADDR(base) ((base) + (0x08E4))
#define PMIC_BUCK70_CTRL52_ADDR(base) ((base) + (0x08E5))
#define PMIC_BUCK70_CTRL53_ADDR(base) ((base) + (0x08E6))
#define PMIC_BUCK70_CTRL54_ADDR(base) ((base) + (0x08E7))
#define PMIC_BUCK701_CTRL0_ADDR(base) ((base) + (0x08E8))
#define PMIC_BUCK701_CTRL1_ADDR(base) ((base) + (0x08E9))
#define PMIC_BUCK701_CTRL2_ADDR(base) ((base) + (0x08EA))
#define PMIC_BUCK701_CTRL3_ADDR(base) ((base) + (0x08EB))
#define PMIC_BUCK701_CTRL4_ADDR(base) ((base) + (0x08EC))
#define PMIC_BUCK71_CTRL0_ADDR(base) ((base) + (0x08ED))
#define PMIC_BUCK71_CTRL1_ADDR(base) ((base) + (0x08EE))
#define PMIC_BUCK71_CTRL2_ADDR(base) ((base) + (0x08EF))
#define PMIC_BUCK71_CTRL3_ADDR(base) ((base) + (0x08F0))
#define PMIC_BUCK71_CTRL4_ADDR(base) ((base) + (0x08F1))
#define PMIC_BUCK71_CTRL5_ADDR(base) ((base) + (0x08F2))
#define PMIC_BUCK71_CTRL6_ADDR(base) ((base) + (0x08F3))
#define PMIC_BUCK71_CTRL7_ADDR(base) ((base) + (0x08F4))
#define PMIC_BUCK71_CTRL8_ADDR(base) ((base) + (0x08F5))
#define PMIC_BUCK71_CTRL9_ADDR(base) ((base) + (0x08F6))
#define PMIC_BUCK71_CTRL10_ADDR(base) ((base) + (0x08F7))
#define PMIC_BUCK71_CTRL11_ADDR(base) ((base) + (0x08F8))
#define PMIC_BUCK71_CTRL12_ADDR(base) ((base) + (0x08F9))
#define PMIC_BUCK71_CTRL13_ADDR(base) ((base) + (0x08FA))
#define PMIC_BUCK71_CTRL14_ADDR(base) ((base) + (0x08FB))
#define PMIC_BUCK71_CTRL15_ADDR(base) ((base) + (0x08FC))
#define PMIC_BUCK71_CTRL16_ADDR(base) ((base) + (0x08FD))
#define PMIC_BUCK71_CTRL17_ADDR(base) ((base) + (0x08FE))
#define PMIC_BUCK71_CTRL18_ADDR(base) ((base) + (0x08FF))
#define PMIC_BUCK71_CTRL19_ADDR(base) ((base) + (0x0900))
#define PMIC_BUCK71_CTRL20_ADDR(base) ((base) + (0x0901))
#define PMIC_BUCK71_CTRL21_ADDR(base) ((base) + (0x0902))
#define PMIC_BUCK71_CTRL22_ADDR(base) ((base) + (0x0903))
#define PMIC_BUCK71_CTRL23_ADDR(base) ((base) + (0x0904))
#define PMIC_BUCK71_CTRL24_ADDR(base) ((base) + (0x0905))
#define PMIC_BUCK71_CTRL25_ADDR(base) ((base) + (0x0906))
#define PMIC_BUCK71_CTRL26_ADDR(base) ((base) + (0x0907))
#define PMIC_BUCK71_CTRL27_ADDR(base) ((base) + (0x0908))
#define PMIC_BUCK71_CTRL28_ADDR(base) ((base) + (0x0909))
#define PMIC_BUCK71_CTRL29_ADDR(base) ((base) + (0x090A))
#define PMIC_BUCK71_CTRL30_ADDR(base) ((base) + (0x090B))
#define PMIC_BUCK71_CTRL31_ADDR(base) ((base) + (0x090C))
#define PMIC_BUCK71_CTRL32_ADDR(base) ((base) + (0x090D))
#define PMIC_BUCK71_CTRL33_ADDR(base) ((base) + (0x090E))
#define PMIC_BUCK71_CTRL34_ADDR(base) ((base) + (0x090F))
#define PMIC_BUCK71_CTRL35_ADDR(base) ((base) + (0x0910))
#define PMIC_BUCK71_CTRL36_ADDR(base) ((base) + (0x0911))
#define PMIC_BUCK71_CTRL37_ADDR(base) ((base) + (0x0912))
#define PMIC_BUCK71_CTRL38_ADDR(base) ((base) + (0x0913))
#define PMIC_BUCK71_CTRL39_ADDR(base) ((base) + (0x0914))
#define PMIC_BUCK71_CTRL40_ADDR(base) ((base) + (0x0915))
#define PMIC_BUCK71_CTRL41_ADDR(base) ((base) + (0x0916))
#define PMIC_BUCK71_CTRL42_ADDR(base) ((base) + (0x0917))
#define PMIC_BUCK71_CTRL43_ADDR(base) ((base) + (0x0918))
#define PMIC_BUCK71_CTRL44_ADDR(base) ((base) + (0x0919))
#define PMIC_BUCK701_RESERVE0_ADDR(base) ((base) + (0x091A))
#define PMIC_BUCK701_RESERVE1_ADDR(base) ((base) + (0x091B))
#define PMIC_BUCK701_RESERVE2_ADDR(base) ((base) + (0x091C))
#define PMIC_BUCK701_RESERVE3_ADDR(base) ((base) + (0x091D))
#define PMIC_BUCK9_CTRL0_ADDR(base) ((base) + (0x091E))
#define PMIC_BUCK9_CTRL1_ADDR(base) ((base) + (0x091F))
#define PMIC_BUCK9_CTRL2_ADDR(base) ((base) + (0x0920))
#define PMIC_BUCK9_CTRL3_ADDR(base) ((base) + (0x0921))
#define PMIC_BUCK9_CTRL4_ADDR(base) ((base) + (0x0922))
#define PMIC_BUCK9_CTRL5_ADDR(base) ((base) + (0x0923))
#define PMIC_BUCK9_CTRL6_ADDR(base) ((base) + (0x0924))
#define PMIC_BUCK9_CTRL7_ADDR(base) ((base) + (0x0925))
#define PMIC_BUCK9_CTRL8_ADDR(base) ((base) + (0x0926))
#define PMIC_BUCK9_CTRL9_ADDR(base) ((base) + (0x0927))
#define PMIC_BUCK9_CTRL10_ADDR(base) ((base) + (0x0928))
#define PMIC_BUCK9_CTRL11_ADDR(base) ((base) + (0x0929))
#define PMIC_BUCK9_CTRL12_ADDR(base) ((base) + (0x092A))
#define PMIC_BUCK9_CTRL13_ADDR(base) ((base) + (0x092B))
#define PMIC_BUCK9_CTRL14_ADDR(base) ((base) + (0x092C))
#define PMIC_BUCK_CTRL0_ADDR(base) ((base) + (0x092D))
#define PMIC_BUCK_CTRL1_ADDR(base) ((base) + (0x092E))
#define PMIC_BUCK_CTRL2_ADDR(base) ((base) + (0x092F))
#define PMIC_BUCK_RESERVE0_ADDR(base) ((base) + (0x0930))
#define PMIC_BUCK_RESERVE1_ADDR(base) ((base) + (0x0931))
#define PMIC_BUCK_RESERVE2_ADDR(base) ((base) + (0x0932))
#define PMIC_LDO0_CTRL0_ADDR(base) ((base) + (0x0933))
#define PMIC_LDO0_CTRL1_ADDR(base) ((base) + (0x0934))
#define PMIC_LDO0_CTRL2_ADDR(base) ((base) + (0x0935))
#define PMIC_LDO1_CTRL0_ADDR(base) ((base) + (0x0936))
#define PMIC_LDO1_CTRL1_ADDR(base) ((base) + (0x0937))
#define PMIC_LDO1_CTRL2_ADDR(base) ((base) + (0x0938))
#define PMIC_LDO1_CTRL3_ADDR(base) ((base) + (0x0939))
#define PMIC_LDO3_CTRL0_ADDR(base) ((base) + (0x093A))
#define PMIC_LDO3_CTRL1_ADDR(base) ((base) + (0x093B))
#define PMIC_LDO4_CTRL0_ADDR(base) ((base) + (0x093C))
#define PMIC_LDO4_CTRL1_ADDR(base) ((base) + (0x093D))
#define PMIC_LDO6_CTRL0_ADDR(base) ((base) + (0x093E))
#define PMIC_LDO6_CTRL1_ADDR(base) ((base) + (0x093F))
#define PMIC_LDO6_CTRL2_ADDR(base) ((base) + (0x0940))
#define PMIC_LDO8_CTRL_ADDR(base) ((base) + (0x0941))
#define PMIC_LDO11_CTRL_ADDR(base) ((base) + (0x0942))
#define PMIC_LDO12_CTRL_ADDR(base) ((base) + (0x0943))
#define PMIC_LDO14_CTRL_ADDR(base) ((base) + (0x0944))
#define PMIC_LDO15_CTRL_ADDR(base) ((base) + (0x0945))
#define PMIC_LDO16_CTRL_ADDR(base) ((base) + (0x0946))
#define PMIC_LDO17_CTRL_ADDR(base) ((base) + (0x0947))
#define PMIC_LDO18_CTRL_ADDR(base) ((base) + (0x0948))
#define PMIC_LDO21_CTRL0_ADDR(base) ((base) + (0x0949))
#define PMIC_LDO21_CTRL1_ADDR(base) ((base) + (0x094A))
#define PMIC_LDO22_CTRL0_ADDR(base) ((base) + (0x094B))
#define PMIC_LDO22_CTRL1_ADDR(base) ((base) + (0x094C))
#define PMIC_LDO22_CTRL2_ADDR(base) ((base) + (0x094D))
#define PMIC_LDO22_CTRL3_ADDR(base) ((base) + (0x094E))
#define PMIC_LDO23_CTRL_ADDR(base) ((base) + (0x094F))
#define PMIC_LDO24_CTRL_ADDR(base) ((base) + (0x0950))
#define PMIC_LDO25_CTRL0_ADDR(base) ((base) + (0x0951))
#define PMIC_LDO25_CTRL1_ADDR(base) ((base) + (0x0952))
#define PMIC_LDO25_CTRL2_ADDR(base) ((base) + (0x0953))
#define PMIC_LDO25_CTRL3_ADDR(base) ((base) + (0x0954))
#define PMIC_LDO26_CTRL_ADDR(base) ((base) + (0x0955))
#define PMIC_LDO27_CTRL_ADDR(base) ((base) + (0x0956))
#define PMIC_LDO28_CTRL_ADDR(base) ((base) + (0x0957))
#define PMIC_LDO29_CTRL0_ADDR(base) ((base) + (0x0958))
#define PMIC_LDO29_CTRL1_ADDR(base) ((base) + (0x0959))
#define PMIC_LDO29_CTRL2_ADDR(base) ((base) + (0x095A))
#define PMIC_LDO29_CTRL3_ADDR(base) ((base) + (0x095B))
#define PMIC_LDO30_CTRL0_ADDR(base) ((base) + (0x095C))
#define PMIC_LDO30_CTRL1_ADDR(base) ((base) + (0x095D))
#define PMIC_LDO30_CTRL2_ADDR(base) ((base) + (0x095E))
#define PMIC_LDO32_CTRL0_ADDR(base) ((base) + (0x095F))
#define PMIC_LDO32_CTRL1_ADDR(base) ((base) + (0x0960))
#define PMIC_LDO32_CTRL2_ADDR(base) ((base) + (0x0961))
#define PMIC_LDO32_CTRL3_ADDR(base) ((base) + (0x0962))
#define PMIC_PMUH_CTRL_ADDR(base) ((base) + (0x0963))
#define PMIC_LDO36_CTRL0_ADDR(base) ((base) + (0x0964))
#define PMIC_LDO36_CTRL1_ADDR(base) ((base) + (0x0965))
#define PMIC_LDO37_CTRL0_ADDR(base) ((base) + (0x0966))
#define PMIC_LDO37_CTRL1_ADDR(base) ((base) + (0x0967))
#define PMIC_LDO38_CTRL0_ADDR(base) ((base) + (0x0968))
#define PMIC_LDO38_CTRL1_ADDR(base) ((base) + (0x0969))
#define PMIC_LDO38_CTRL2_ADDR(base) ((base) + (0x096A))
#define PMIC_LDO38_CTRL3_ADDR(base) ((base) + (0x096B))
#define PMIC_LDO41_CTRL0_ADDR(base) ((base) + (0x096C))
#define PMIC_LDO41_CTRL1_ADDR(base) ((base) + (0x096D))
#define PMIC_LDO41_CTRL2_ADDR(base) ((base) + (0x096E))
#define PMIC_LDO41_CTRL3_ADDR(base) ((base) + (0x096F))
#define PMIC_LDO42_CTRL0_ADDR(base) ((base) + (0x0970))
#define PMIC_LDO42_CTRL1_ADDR(base) ((base) + (0x0971))
#define PMIC_LDO42_CTRL2_ADDR(base) ((base) + (0x0972))
#define PMIC_LDO43_CTRL0_ADDR(base) ((base) + (0x0973))
#define PMIC_LDO43_CTRL1_ADDR(base) ((base) + (0x0974))
#define PMIC_LDO43_CTRL2_ADDR(base) ((base) + (0x0975))
#define PMIC_LDO44_CTRL0_ADDR(base) ((base) + (0x0976))
#define PMIC_LDO44_CTRL1_ADDR(base) ((base) + (0x0977))
#define PMIC_LDO44_CTRL2_ADDR(base) ((base) + (0x0978))
#define PMIC_LDO45_CTRL0_ADDR(base) ((base) + (0x0979))
#define PMIC_LDO45_CTRL1_ADDR(base) ((base) + (0x097A))
#define PMIC_LDO45_CTRL2_ADDR(base) ((base) + (0x097B))
#define PMIC_LDO46_CTRL0_ADDR(base) ((base) + (0x097C))
#define PMIC_LDO46_CTRL1_ADDR(base) ((base) + (0x097D))
#define PMIC_SW1_CTRL_ADDR(base) ((base) + (0x097E))
#define PMIC_LDO_SINK_CTRL_ADDR(base) ((base) + (0x097F))
#define PMIC_LDO_BUF_REF_CTRL_ADDR(base) ((base) + (0x0980))
#define PMIC_LDO_RESERVE0_ADDR(base) ((base) + (0x0981))
#define PMIC_LDO_RESERVE1_ADDR(base) ((base) + (0x0982))
#define PMIC_LDO_RESERVE2_ADDR(base) ((base) + (0x0983))
#define PMIC_LDO_RESERVE3_ADDR(base) ((base) + (0x0984))
#define PMIC_LDO_RESERVE4_ADDR(base) ((base) + (0x0985))
#define PMIC_LDO_RESERVE5_ADDR(base) ((base) + (0x0986))
#define PMIC_LDO_RESERVE6_ADDR(base) ((base) + (0x0987))
#define PMIC_LDO_RESERVE7_ADDR(base) ((base) + (0x0988))
#define PMIC_LDO_RESERVE8_ADDR(base) ((base) + (0x0989))
#define PMIC_LDO_RESERVE9_ADDR(base) ((base) + (0x098A))
#define PMIC_D2A_RES0_ADDR(base) ((base) + (0x098B))
#define PMIC_D2A_RES1_ADDR(base) ((base) + (0x098C))
#define PMIC_A2D_RES0_ADDR(base) ((base) + (0x098D))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_SER_RTCDR0_ADDR(base) ((base) + (0x0C00UL))
#define PMIC_SER_RTCDR1_ADDR(base) ((base) + (0x0C01UL))
#define PMIC_SER_RTCDR2_ADDR(base) ((base) + (0x0C02UL))
#define PMIC_SER_RTCDR3_ADDR(base) ((base) + (0x0C03UL))
#define PMIC_SER_RTCMR0_ADDR(base) ((base) + (0x0C04UL))
#define PMIC_SER_RTCMR1_ADDR(base) ((base) + (0x0C05UL))
#define PMIC_SER_RTCMR2_ADDR(base) ((base) + (0x0C06UL))
#define PMIC_SER_RTCMR3_ADDR(base) ((base) + (0x0C07UL))
#define PMIC_SER_RTCLR0_ADDR(base) ((base) + (0x0C08UL))
#define PMIC_SER_RTCLR1_ADDR(base) ((base) + (0x0C09UL))
#define PMIC_SER_RTCLR2_ADDR(base) ((base) + (0x0C0AUL))
#define PMIC_SER_RTCLR3_ADDR(base) ((base) + (0x0C0BUL))
#define PMIC_SER_RTCCTRL_ADDR(base) ((base) + (0x0C0CUL))
#define PMIC_SER_XO_THRESOLD0_ADDR(base) ((base) + (0x0C0DUL))
#define PMIC_SER_XO_THRESOLD1_ADDR(base) ((base) + (0x0C0EUL))
#define PMIC_SER_CRC_VAULE0_ADDR(base) ((base) + (0x0C0FUL))
#define PMIC_SER_CRC_VAULE1_ADDR(base) ((base) + (0x0C10UL))
#define PMIC_SER_CRC_VAULE2_ADDR(base) ((base) + (0x0C11UL))
#define PMIC_SER_RTC_PWRUP_TIMER0_ADDR(base) ((base) + (0x0C12UL))
#define PMIC_SER_RTC_PWRUP_TIMER1_ADDR(base) ((base) + (0x0C13UL))
#define PMIC_SER_RTC_PWRUP_TIMER2_ADDR(base) ((base) + (0x0C14UL))
#define PMIC_SER_RTC_PWRUP_TIMER3_ADDR(base) ((base) + (0x0C15UL))
#define PMIC_SER_RTC_PWRDOWN_TIMER0_ADDR(base) ((base) + (0x0C16UL))
#define PMIC_SER_RTC_PWRDOWN_TIMER1_ADDR(base) ((base) + (0x0C17UL))
#define PMIC_SER_RTC_PWRDOWN_TIMER2_ADDR(base) ((base) + (0x0C18UL))
#define PMIC_SER_RTC_PWRDOWN_TIMER3_ADDR(base) ((base) + (0x0C19UL))
#define PMIC_LDO2_ONOFF1_ECO_ADDR(base) ((base) + (0x0C1AUL))
#define PMIC_LDO2_ONOFF2_ADDR(base) ((base) + (0x0C1BUL))
#define PMIC_LDO2_VSET_ADDR(base) ((base) + (0x0C1CUL))
#define PMIC_LDO2_CTRL_ADDR(base) ((base) + (0x0C1DUL))
#define PMIC_LDO2_EN_CTRL_ADDR(base) ((base) + (0x0C1EUL))
#define PMIC_LDO39_ONOFF_ECO_ADDR(base) ((base) + (0x0C21UL))
#define PMIC_LDO39_VSET_ADDR(base) ((base) + (0x0C22UL))
#define PMIC_LDO39_CTRL0_ADDR(base) ((base) + (0x0C23UL))
#define PMIC_LDO39_CTRL1_ADDR(base) ((base) + (0x0C24UL))
#define PMIC_LDO39_CTRL2_ADDR(base) ((base) + (0x0C25UL))
#define PMIC_RAMP_LDO39_CTRL0_ADDR(base) ((base) + (0x0C28UL))
#define PMIC_RAMP_LDO39_CTRL1_ADDR(base) ((base) + (0x0C29UL))
#define PMIC_SER_SIDEKEY_MODE_ADDR(base) ((base) + (0x0C30UL))
#else
#define PMIC_SER_RTCDR0_ADDR(base) ((base) + (0x0C00))
#define PMIC_SER_RTCDR1_ADDR(base) ((base) + (0x0C01))
#define PMIC_SER_RTCDR2_ADDR(base) ((base) + (0x0C02))
#define PMIC_SER_RTCDR3_ADDR(base) ((base) + (0x0C03))
#define PMIC_SER_RTCMR0_ADDR(base) ((base) + (0x0C04))
#define PMIC_SER_RTCMR1_ADDR(base) ((base) + (0x0C05))
#define PMIC_SER_RTCMR2_ADDR(base) ((base) + (0x0C06))
#define PMIC_SER_RTCMR3_ADDR(base) ((base) + (0x0C07))
#define PMIC_SER_RTCLR0_ADDR(base) ((base) + (0x0C08))
#define PMIC_SER_RTCLR1_ADDR(base) ((base) + (0x0C09))
#define PMIC_SER_RTCLR2_ADDR(base) ((base) + (0x0C0A))
#define PMIC_SER_RTCLR3_ADDR(base) ((base) + (0x0C0B))
#define PMIC_SER_RTCCTRL_ADDR(base) ((base) + (0x0C0C))
#define PMIC_SER_XO_THRESOLD0_ADDR(base) ((base) + (0x0C0D))
#define PMIC_SER_XO_THRESOLD1_ADDR(base) ((base) + (0x0C0E))
#define PMIC_SER_CRC_VAULE0_ADDR(base) ((base) + (0x0C0F))
#define PMIC_SER_CRC_VAULE1_ADDR(base) ((base) + (0x0C10))
#define PMIC_SER_CRC_VAULE2_ADDR(base) ((base) + (0x0C11))
#define PMIC_SER_RTC_PWRUP_TIMER0_ADDR(base) ((base) + (0x0C12))
#define PMIC_SER_RTC_PWRUP_TIMER1_ADDR(base) ((base) + (0x0C13))
#define PMIC_SER_RTC_PWRUP_TIMER2_ADDR(base) ((base) + (0x0C14))
#define PMIC_SER_RTC_PWRUP_TIMER3_ADDR(base) ((base) + (0x0C15))
#define PMIC_SER_RTC_PWRDOWN_TIMER0_ADDR(base) ((base) + (0x0C16))
#define PMIC_SER_RTC_PWRDOWN_TIMER1_ADDR(base) ((base) + (0x0C17))
#define PMIC_SER_RTC_PWRDOWN_TIMER2_ADDR(base) ((base) + (0x0C18))
#define PMIC_SER_RTC_PWRDOWN_TIMER3_ADDR(base) ((base) + (0x0C19))
#define PMIC_LDO2_ONOFF1_ECO_ADDR(base) ((base) + (0x0C1A))
#define PMIC_LDO2_ONOFF2_ADDR(base) ((base) + (0x0C1B))
#define PMIC_LDO2_VSET_ADDR(base) ((base) + (0x0C1C))
#define PMIC_LDO2_CTRL_ADDR(base) ((base) + (0x0C1D))
#define PMIC_LDO2_EN_CTRL_ADDR(base) ((base) + (0x0C1E))
#define PMIC_LDO39_ONOFF_ECO_ADDR(base) ((base) + (0x0C21))
#define PMIC_LDO39_VSET_ADDR(base) ((base) + (0x0C22))
#define PMIC_LDO39_CTRL0_ADDR(base) ((base) + (0x0C23))
#define PMIC_LDO39_CTRL1_ADDR(base) ((base) + (0x0C24))
#define PMIC_LDO39_CTRL2_ADDR(base) ((base) + (0x0C25))
#define PMIC_RAMP_LDO39_CTRL0_ADDR(base) ((base) + (0x0C28))
#define PMIC_RAMP_LDO39_CTRL1_ADDR(base) ((base) + (0x0C29))
#define PMIC_SER_SIDEKEY_MODE_ADDR(base) ((base) + (0x0C30))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_ADC_CTRL_ADDR(base) ((base) + (0x2000UL))
#define PMIC_ADC_START_ADDR(base) ((base) + (0x2001UL))
#define PMIC_CONV_STATUS_ADDR(base) ((base) + (0x2002UL))
#define PMIC_ADC_DATA1_ADDR(base) ((base) + (0x2003UL))
#define PMIC_ADC_DATA0_ADDR(base) ((base) + (0x2004UL))
#define PMIC_ADC_CONV_ADDR(base) ((base) + (0x2005UL))
#define PMIC_ADC_CURRENT_ADDR(base) ((base) + (0x2006UL))
#define PMIC_ADC_CALI_CTRL_ADDR(base) ((base) + (0x2007UL))
#define PMIC_ADC_CALI_VALUE_ADDR(base) ((base) + (0x2008UL))
#define PMIC_ADC_CALI_CFG_ADDR(base) ((base) + (0x2009UL))
#define PMIC_ADC_MODE_CFG_ADDR(base) ((base) + (0x200AUL))
#define PMIC_ADC_CHOPPER_1ST_DATA1_ADDR(base) ((base) + (0x200BUL))
#define PMIC_ADC_CHOPPER_1ST_DATA2_ADDR(base) ((base) + (0x200CUL))
#define PMIC_ADC_CHOPPER_2ND_DATA1_ADDR(base) ((base) + (0x200DUL))
#define PMIC_ADC_CHOPPER_2ND_DATA2_ADDR(base) ((base) + (0x200EUL))
#define PMIC_ADC_CALIVALUE_CFG1_ADDR(base) ((base) + (0x200FUL))
#define PMIC_ADC_CALIVALUE_CFG2_ADDR(base) ((base) + (0x2010UL))
#else
#define PMIC_ADC_CTRL_ADDR(base) ((base) + (0x2000))
#define PMIC_ADC_START_ADDR(base) ((base) + (0x2001))
#define PMIC_CONV_STATUS_ADDR(base) ((base) + (0x2002))
#define PMIC_ADC_DATA1_ADDR(base) ((base) + (0x2003))
#define PMIC_ADC_DATA0_ADDR(base) ((base) + (0x2004))
#define PMIC_ADC_CONV_ADDR(base) ((base) + (0x2005))
#define PMIC_ADC_CURRENT_ADDR(base) ((base) + (0x2006))
#define PMIC_ADC_CALI_CTRL_ADDR(base) ((base) + (0x2007))
#define PMIC_ADC_CALI_VALUE_ADDR(base) ((base) + (0x2008))
#define PMIC_ADC_CALI_CFG_ADDR(base) ((base) + (0x2009))
#define PMIC_ADC_MODE_CFG_ADDR(base) ((base) + (0x200A))
#define PMIC_ADC_CHOPPER_1ST_DATA1_ADDR(base) ((base) + (0x200B))
#define PMIC_ADC_CHOPPER_1ST_DATA2_ADDR(base) ((base) + (0x200C))
#define PMIC_ADC_CHOPPER_2ND_DATA1_ADDR(base) ((base) + (0x200D))
#define PMIC_ADC_CHOPPER_2ND_DATA2_ADDR(base) ((base) + (0x200E))
#define PMIC_ADC_CALIVALUE_CFG1_ADDR(base) ((base) + (0x200F))
#define PMIC_ADC_CALIVALUE_CFG2_ADDR(base) ((base) + (0x2010))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_COUL_IRQ_ADDR(base) ((base) + (0x4000UL))
#define PMIC_COUL_IRQ_MASK_ADDR(base) ((base) + (0x4001UL))
#define PMIC_COUL_IRQ_NP_ADDR(base) ((base) + (0x4002UL))
#define PMIC_CLJ_CTRL_REG_ADDR(base) ((base) + (0x4003UL))
#define PMIC_CLJ_CTRL_REGS2_ADDR(base) ((base) + (0x4004UL))
#define PMIC_CLJ_CTRL_REGS3_ADDR(base) ((base) + (0x4005UL))
#define PMIC_CLJ_CTRL_REGS4_ADDR(base) ((base) + (0x4006UL))
#define PMIC_CLJ_CTRL_REGS5_ADDR(base) ((base) + (0x4007UL))
#define PMIC_CIC_CTRL_ADDR(base) ((base) + (0x4008UL))
#define PMIC_COUL_TEMP_CTRL_ADDR(base) ((base) + (0x4009UL))
#define PMIC_COUL_VI_CTRL0_ADDR(base) ((base) + (0x400AUL))
#define PMIC_COUL_VI_CTRL1_ADDR(base) ((base) + (0x400BUL))
#define PMIC_COUL_VI_CTRL2_ADDR(base) ((base) + (0x400CUL))
#define PMIC_COUL_VI_CTRL3_ADDR(base) ((base) + (0x400DUL))
#define PMIC_COUL_VI_CTRL4_ADDR(base) ((base) + (0x400EUL))
#define PMIC_COUL_VI_CTRL5_ADDR(base) ((base) + (0x400FUL))
#define PMIC_CL_OUT0_ADDR(base) ((base) + (0x4010UL))
#define PMIC_CL_OUT1_ADDR(base) ((base) + (0x4011UL))
#define PMIC_CL_OUT2_ADDR(base) ((base) + (0x4012UL))
#define PMIC_CL_OUT3_ADDR(base) ((base) + (0x4013UL))
#define PMIC_CL_OUT4_ADDR(base) ((base) + (0x4014UL))
#define PMIC_CL_IN0_ADDR(base) ((base) + (0x4015UL))
#define PMIC_CL_IN1_ADDR(base) ((base) + (0x4016UL))
#define PMIC_CL_IN2_ADDR(base) ((base) + (0x4017UL))
#define PMIC_CL_IN3_ADDR(base) ((base) + (0x4018UL))
#define PMIC_CL_IN4_ADDR(base) ((base) + (0x4019UL))
#define PMIC_CHG_TIMER0_ADDR(base) ((base) + (0x401AUL))
#define PMIC_CHG_TIMER1_ADDR(base) ((base) + (0x401BUL))
#define PMIC_CHG_TIMER2_ADDR(base) ((base) + (0x401CUL))
#define PMIC_CHG_TIMER3_ADDR(base) ((base) + (0x401DUL))
#define PMIC_LOAD_TIMER0_ADDR(base) ((base) + (0x401EUL))
#define PMIC_LOAD_TIMER1_ADDR(base) ((base) + (0x401FUL))
#define PMIC_LOAD_TIMER2_ADDR(base) ((base) + (0x4020UL))
#define PMIC_LOAD_TIMER3_ADDR(base) ((base) + (0x4021UL))
#define PMIC_CL_INT0_ADDR(base) ((base) + (0x4022UL))
#define PMIC_CL_INT1_ADDR(base) ((base) + (0x4023UL))
#define PMIC_CL_INT2_ADDR(base) ((base) + (0x4024UL))
#define PMIC_CL_INT3_ADDR(base) ((base) + (0x4025UL))
#define PMIC_CL_INT4_ADDR(base) ((base) + (0x4026UL))
#define PMIC_V_INT0_ADDR(base) ((base) + (0x4027UL))
#define PMIC_V_INT1_ADDR(base) ((base) + (0x4028UL))
#define PMIC_V_INT2_ADDR(base) ((base) + (0x4029UL))
#define PMIC_I_OUT_GATE0_ADDR(base) ((base) + (0x402AUL))
#define PMIC_I_OUT_GATE1_ADDR(base) ((base) + (0x402BUL))
#define PMIC_I_OUT_GATE2_ADDR(base) ((base) + (0x402CUL))
#define PMIC_I_IN_GATE0_ADDR(base) ((base) + (0x402DUL))
#define PMIC_I_IN_GATE1_ADDR(base) ((base) + (0x402EUL))
#define PMIC_I_IN_GATE2_ADDR(base) ((base) + (0x402FUL))
#define PMIC_OFFSET_CURRENT_MOD_0_ADDR(base) ((base) + (0x4030UL))
#define PMIC_OFFSET_CURRENT_MOD_1_ADDR(base) ((base) + (0x4031UL))
#define PMIC_OFFSET_CURRENT_MOD_2_ADDR(base) ((base) + (0x4032UL))
#define PMIC_OFFSET_VOLTAGE_MOD_0_ADDR(base) ((base) + (0x4033UL))
#define PMIC_OFFSET_VOLTAGE_MOD_1_ADDR(base) ((base) + (0x4034UL))
#define PMIC_OFFSET_VOLTAGE_MOD_2_ADDR(base) ((base) + (0x4035UL))
#define PMIC_STATE_TEST_ADDR(base) ((base) + (0x4036UL))
#define PMIC_CURRENT_0_ADDR(base) ((base) + (0x4037UL))
#define PMIC_CURRENT_1_ADDR(base) ((base) + (0x4038UL))
#define PMIC_CURRENT_2_ADDR(base) ((base) + (0x4039UL))
#define PMIC_V_OUT_0_ADDR(base) ((base) + (0x403AUL))
#define PMIC_V_OUT_1_ADDR(base) ((base) + (0x403BUL))
#define PMIC_V_OUT_2_ADDR(base) ((base) + (0x403CUL))
#define PMIC_OFFSET_CURRENT0_ADDR(base) ((base) + (0x403DUL))
#define PMIC_OFFSET_CURRENT1_ADDR(base) ((base) + (0x403EUL))
#define PMIC_OFFSET_CURRENT2_ADDR(base) ((base) + (0x403FUL))
#define PMIC_OFFSET_VOLTAGE0_ADDR(base) ((base) + (0x4040UL))
#define PMIC_OFFSET_VOLTAGE1_ADDR(base) ((base) + (0x4041UL))
#define PMIC_OFFSET_VOLTAGE2_ADDR(base) ((base) + (0x4042UL))
#define PMIC_OCV_VOLTAGE0_ADDR(base) ((base) + (0x4043UL))
#define PMIC_OCV_VOLTAGE1_ADDR(base) ((base) + (0x4044UL))
#define PMIC_OCV_VOLTAGE2_ADDR(base) ((base) + (0x4045UL))
#define PMIC_OCV_CURRENT0_ADDR(base) ((base) + (0x4046UL))
#define PMIC_OCV_CURRENT1_ADDR(base) ((base) + (0x4047UL))
#define PMIC_OCV_CURRENT2_ADDR(base) ((base) + (0x4048UL))
#define PMIC_OCV_TEMP0_ADDR(base) ((base) + (0x4049UL))
#define PMIC_OCV_TEMP1_ADDR(base) ((base) + (0x404AUL))
#define PMIC_OCV_TEMP2_ADDR(base) ((base) + (0x404BUL))
#define PMIC_ECO_OUT_CLIN_0_ADDR(base) ((base) + (0x404CUL))
#define PMIC_ECO_OUT_CLIN_1_ADDR(base) ((base) + (0x404DUL))
#define PMIC_ECO_OUT_CLIN_2_ADDR(base) ((base) + (0x404EUL))
#define PMIC_ECO_OUT_CLIN_3_ADDR(base) ((base) + (0x404FUL))
#define PMIC_ECO_OUT_CLIN_4_ADDR(base) ((base) + (0x4050UL))
#define PMIC_ECO_OUT_CLOUT_0_ADDR(base) ((base) + (0x4051UL))
#define PMIC_ECO_OUT_CLOUT_1_ADDR(base) ((base) + (0x4052UL))
#define PMIC_ECO_OUT_CLOUT_2_ADDR(base) ((base) + (0x4053UL))
#define PMIC_ECO_OUT_CLOUT_3_ADDR(base) ((base) + (0x4054UL))
#define PMIC_ECO_OUT_CLOUT_4_ADDR(base) ((base) + (0x4055UL))
#define PMIC_ECO_OUT_TEMP_0_ADDR(base) ((base) + (0x4056UL))
#define PMIC_ECO_OUT_TEMP_1_ADDR(base) ((base) + (0x4057UL))
#define PMIC_ECO_OUT_TEMP_2_ADDR(base) ((base) + (0x4058UL))
#define PMIC_TEMP0_RDATA_ADDR(base) ((base) + (0x4059UL))
#define PMIC_TEMP1_RDATA_ADDR(base) ((base) + (0x405AUL))
#define PMIC_TEMP2_RDATA_ADDR(base) ((base) + (0x405BUL))
#define PMIC_V_PRE0_OUT0_ADDR(base) ((base) + (0x405CUL))
#define PMIC_V_PRE0_OUT1_ADDR(base) ((base) + (0x405DUL))
#define PMIC_V_PRE0_OUT2_ADDR(base) ((base) + (0x405EUL))
#define PMIC_V_PRE1_OUT0_ADDR(base) ((base) + (0x405FUL))
#define PMIC_V_PRE1_OUT1_ADDR(base) ((base) + (0x4060UL))
#define PMIC_V_PRE1_OUT2_ADDR(base) ((base) + (0x4061UL))
#define PMIC_V_PRE2_OUT0_ADDR(base) ((base) + (0x4062UL))
#define PMIC_V_PRE2_OUT1_ADDR(base) ((base) + (0x4063UL))
#define PMIC_V_PRE2_OUT2_ADDR(base) ((base) + (0x4064UL))
#define PMIC_V_PRE3_OUT0_ADDR(base) ((base) + (0x4065UL))
#define PMIC_V_PRE3_OUT1_ADDR(base) ((base) + (0x4066UL))
#define PMIC_V_PRE3_OUT2_ADDR(base) ((base) + (0x4067UL))
#define PMIC_V_PRE4_OUT0_ADDR(base) ((base) + (0x4068UL))
#define PMIC_V_PRE4_OUT1_ADDR(base) ((base) + (0x4069UL))
#define PMIC_V_PRE4_OUT2_ADDR(base) ((base) + (0x406AUL))
#define PMIC_V_PRE5_OUT0_ADDR(base) ((base) + (0x406BUL))
#define PMIC_V_PRE5_OUT1_ADDR(base) ((base) + (0x406CUL))
#define PMIC_V_PRE5_OUT2_ADDR(base) ((base) + (0x406DUL))
#define PMIC_V_PRE6_OUT0_ADDR(base) ((base) + (0x406EUL))
#define PMIC_V_PRE6_OUT1_ADDR(base) ((base) + (0x406FUL))
#define PMIC_V_PRE6_OUT2_ADDR(base) ((base) + (0x4070UL))
#define PMIC_V_PRE7_OUT0_ADDR(base) ((base) + (0x4071UL))
#define PMIC_V_PRE7_OUT1_ADDR(base) ((base) + (0x4072UL))
#define PMIC_V_PRE7_OUT2_ADDR(base) ((base) + (0x4073UL))
#define PMIC_CURRENT_PRE0_OUT0_ADDR(base) ((base) + (0x4074UL))
#define PMIC_CURRENT_PRE0_OUT1_ADDR(base) ((base) + (0x4075UL))
#define PMIC_CURRENT_PRE0_OUT2_ADDR(base) ((base) + (0x4076UL))
#define PMIC_CURRENT_PRE1_OUT0_ADDR(base) ((base) + (0x4077UL))
#define PMIC_CURRENT_PRE1_OUT1_ADDR(base) ((base) + (0x4078UL))
#define PMIC_CURRENT_PRE1_OUT2_ADDR(base) ((base) + (0x4079UL))
#define PMIC_CURRENT_PRE2_OUT0_ADDR(base) ((base) + (0x407AUL))
#define PMIC_CURRENT_PRE2_OUT1_ADDR(base) ((base) + (0x407BUL))
#define PMIC_CURRENT_PRE2_OUT2_ADDR(base) ((base) + (0x407CUL))
#define PMIC_CURRENT_PRE3_OUT0_ADDR(base) ((base) + (0x407DUL))
#define PMIC_CURRENT_PRE3_OUT1_ADDR(base) ((base) + (0x407EUL))
#define PMIC_CURRENT_PRE3_OUT2_ADDR(base) ((base) + (0x407FUL))
#define PMIC_CURRENT_PRE4_OUT0_ADDR(base) ((base) + (0x4080UL))
#define PMIC_CURRENT_PRE4_OUT1_ADDR(base) ((base) + (0x4081UL))
#define PMIC_CURRENT_PRE4_OUT2_ADDR(base) ((base) + (0x4082UL))
#define PMIC_CURRENT_PRE5_OUT0_ADDR(base) ((base) + (0x4083UL))
#define PMIC_CURRENT_PRE5_OUT1_ADDR(base) ((base) + (0x4084UL))
#define PMIC_CURRENT_PRE5_OUT2_ADDR(base) ((base) + (0x4085UL))
#define PMIC_CURRENT_PRE6_OUT0_ADDR(base) ((base) + (0x4086UL))
#define PMIC_CURRENT_PRE6_OUT1_ADDR(base) ((base) + (0x4087UL))
#define PMIC_CURRENT_PRE6_OUT2_ADDR(base) ((base) + (0x4088UL))
#define PMIC_CURRENT_PRE7_OUT0_ADDR(base) ((base) + (0x4089UL))
#define PMIC_CURRENT_PRE7_OUT1_ADDR(base) ((base) + (0x408AUL))
#define PMIC_CURRENT_PRE7_OUT2_ADDR(base) ((base) + (0x408BUL))
#define PMIC_V_OCV_PRE1_OUT0_ADDR(base) ((base) + (0x408CUL))
#define PMIC_V_OCV_PRE1_OUT1_ADDR(base) ((base) + (0x408DUL))
#define PMIC_V_OCV_PRE1_OUT2_ADDR(base) ((base) + (0x408EUL))
#define PMIC_V_OCV_PRE2_OUT0_ADDR(base) ((base) + (0x408FUL))
#define PMIC_V_OCV_PRE2_OUT1_ADDR(base) ((base) + (0x4090UL))
#define PMIC_V_OCV_PRE2_OUT2_ADDR(base) ((base) + (0x4091UL))
#define PMIC_V_OCV_PRE3_OUT0_ADDR(base) ((base) + (0x4092UL))
#define PMIC_V_OCV_PRE3_OUT1_ADDR(base) ((base) + (0x4093UL))
#define PMIC_V_OCV_PRE3_OUT2_ADDR(base) ((base) + (0x4094UL))
#define PMIC_V_OCV_PRE4_OUT0_ADDR(base) ((base) + (0x4095UL))
#define PMIC_V_OCV_PRE4_OUT1_ADDR(base) ((base) + (0x4096UL))
#define PMIC_V_OCV_PRE4_OUT2_ADDR(base) ((base) + (0x4097UL))
#define PMIC_I_OCV_PRE1_OUT0_ADDR(base) ((base) + (0x4098UL))
#define PMIC_I_OCV_PRE1_OUT1_ADDR(base) ((base) + (0x4099UL))
#define PMIC_I_OCV_PRE1_OUT2_ADDR(base) ((base) + (0x409AUL))
#define PMIC_I_OCV_PRE2_OUT0_ADDR(base) ((base) + (0x409BUL))
#define PMIC_I_OCV_PRE2_OUT1_ADDR(base) ((base) + (0x409CUL))
#define PMIC_I_OCV_PRE2_OUT2_ADDR(base) ((base) + (0x409DUL))
#define PMIC_I_OCV_PRE3_OUT0_ADDR(base) ((base) + (0x409EUL))
#define PMIC_I_OCV_PRE3_OUT1_ADDR(base) ((base) + (0x409FUL))
#define PMIC_I_OCV_PRE3_OUT2_ADDR(base) ((base) + (0x40A0UL))
#define PMIC_I_OCV_PRE4_OUT0_ADDR(base) ((base) + (0x40A1UL))
#define PMIC_I_OCV_PRE4_OUT1_ADDR(base) ((base) + (0x40A2UL))
#define PMIC_I_OCV_PRE4_OUT2_ADDR(base) ((base) + (0x40A3UL))
#define PMIC_T_OCV_PRE1_OUT0_ADDR(base) ((base) + (0x40A4UL))
#define PMIC_T_OCV_PRE1_OUT1_ADDR(base) ((base) + (0x40A5UL))
#define PMIC_T_OCV_PRE2_OUT0_ADDR(base) ((base) + (0x40A6UL))
#define PMIC_T_OCV_PRE2_OUT1_ADDR(base) ((base) + (0x40A7UL))
#define PMIC_T_OCV_PRE3_OUT0_ADDR(base) ((base) + (0x40A8UL))
#define PMIC_T_OCV_PRE3_OUT1_ADDR(base) ((base) + (0x40A9UL))
#define PMIC_T_OCV_PRE4_OUT0_ADDR(base) ((base) + (0x40AAUL))
#define PMIC_T_OCV_PRE4_OUT1_ADDR(base) ((base) + (0x40ABUL))
#define PMIC_RTC_OCV_OUT_ADDR(base) ((base) + (0x40ACUL))
#define PMIC_SLIDE_I_DATA0_OUT0_ADDR(base) ((base) + (0x40ADUL))
#define PMIC_SLIDE_I_DATA0_OUT1_ADDR(base) ((base) + (0x40AEUL))
#define PMIC_SLIDE_I_DATA0_OUT2_ADDR(base) ((base) + (0x40AFUL))
#define PMIC_SLIDE_I_DATA1_OUT0_ADDR(base) ((base) + (0x40B0UL))
#define PMIC_SLIDE_I_DATA1_OUT1_ADDR(base) ((base) + (0x40B1UL))
#define PMIC_SLIDE_I_DATA1_OUT2_ADDR(base) ((base) + (0x40B2UL))
#define PMIC_SLIDE_I_DATA2_OUT0_ADDR(base) ((base) + (0x40B3UL))
#define PMIC_SLIDE_I_DATA2_OUT1_ADDR(base) ((base) + (0x40B4UL))
#define PMIC_SLIDE_I_DATA2_OUT2_ADDR(base) ((base) + (0x40B5UL))
#define PMIC_SLIDE_I_DATA3_OUT0_ADDR(base) ((base) + (0x40B6UL))
#define PMIC_SLIDE_I_DATA3_OUT1_ADDR(base) ((base) + (0x40B7UL))
#define PMIC_SLIDE_I_DATA3_OUT2_ADDR(base) ((base) + (0x40B8UL))
#define PMIC_SLIDE_V_DATA0_OUT0_ADDR(base) ((base) + (0x40B9UL))
#define PMIC_SLIDE_V_DATA0_OUT1_ADDR(base) ((base) + (0x40BAUL))
#define PMIC_SLIDE_V_DATA0_OUT2_ADDR(base) ((base) + (0x40BBUL))
#define PMIC_SLIDE_V_DATA1_OUT0_ADDR(base) ((base) + (0x40BCUL))
#define PMIC_SLIDE_V_DATA1_OUT1_ADDR(base) ((base) + (0x40BDUL))
#define PMIC_SLIDE_V_DATA1_OUT2_ADDR(base) ((base) + (0x40BEUL))
#define PMIC_SLIDE_V_DATA2_OUT0_ADDR(base) ((base) + (0x40BFUL))
#define PMIC_SLIDE_V_DATA2_OUT1_ADDR(base) ((base) + (0x40C0UL))
#define PMIC_SLIDE_V_DATA2_OUT2_ADDR(base) ((base) + (0x40C1UL))
#define PMIC_SLIDE_V_DATA3_OUT0_ADDR(base) ((base) + (0x40C2UL))
#define PMIC_SLIDE_V_DATA3_OUT1_ADDR(base) ((base) + (0x40C3UL))
#define PMIC_SLIDE_V_DATA3_OUT2_ADDR(base) ((base) + (0x40C4UL))
#define PMIC_SLIDE_CNT_DATA_OUT0_ADDR(base) ((base) + (0x40C5UL))
#define PMIC_SLIDE_CNT_DATA_OUT1_ADDR(base) ((base) + (0x40C6UL))
#define PMIC_SLIDE_CNT_DATA_OUT2_ADDR(base) ((base) + (0x40C7UL))
#define PMIC_COUL_RESERVE0_ADDR(base) ((base) + (0x40C8UL))
#define PMIC_COUL_RESERVE1_ADDR(base) ((base) + (0x40C9UL))
#define PMIC_CLJ_DEBUG0_ADDR(base) ((base) + (0x40CAUL))
#define PMIC_CLJ_DEBUG1_ADDR(base) ((base) + (0x40CBUL))
#define PMIC_DEBUG_CIC_I0_ADDR(base) ((base) + (0x40CCUL))
#define PMIC_DEBUG_CIC_I1_ADDR(base) ((base) + (0x40CDUL))
#define PMIC_DEBUG_CIC_I2_ADDR(base) ((base) + (0x40CEUL))
#define PMIC_DEBUG_CIC_V0_ADDR(base) ((base) + (0x40CFUL))
#define PMIC_DEBUG_CIC_V1_ADDR(base) ((base) + (0x40D0UL))
#define PMIC_DEBUG_CIC_V2_ADDR(base) ((base) + (0x40D1UL))
#define PMIC_DEBUG_WRITE_PRO_ADDR(base) ((base) + (0x40D2UL))
#else
#define PMIC_COUL_IRQ_ADDR(base) ((base) + (0x4000))
#define PMIC_COUL_IRQ_MASK_ADDR(base) ((base) + (0x4001))
#define PMIC_COUL_IRQ_NP_ADDR(base) ((base) + (0x4002))
#define PMIC_CLJ_CTRL_REG_ADDR(base) ((base) + (0x4003))
#define PMIC_CLJ_CTRL_REGS2_ADDR(base) ((base) + (0x4004))
#define PMIC_CLJ_CTRL_REGS3_ADDR(base) ((base) + (0x4005))
#define PMIC_CLJ_CTRL_REGS4_ADDR(base) ((base) + (0x4006))
#define PMIC_CLJ_CTRL_REGS5_ADDR(base) ((base) + (0x4007))
#define PMIC_CIC_CTRL_ADDR(base) ((base) + (0x4008))
#define PMIC_COUL_TEMP_CTRL_ADDR(base) ((base) + (0x4009))
#define PMIC_COUL_VI_CTRL0_ADDR(base) ((base) + (0x400A))
#define PMIC_COUL_VI_CTRL1_ADDR(base) ((base) + (0x400B))
#define PMIC_COUL_VI_CTRL2_ADDR(base) ((base) + (0x400C))
#define PMIC_COUL_VI_CTRL3_ADDR(base) ((base) + (0x400D))
#define PMIC_COUL_VI_CTRL4_ADDR(base) ((base) + (0x400E))
#define PMIC_COUL_VI_CTRL5_ADDR(base) ((base) + (0x400F))
#define PMIC_CL_OUT0_ADDR(base) ((base) + (0x4010))
#define PMIC_CL_OUT1_ADDR(base) ((base) + (0x4011))
#define PMIC_CL_OUT2_ADDR(base) ((base) + (0x4012))
#define PMIC_CL_OUT3_ADDR(base) ((base) + (0x4013))
#define PMIC_CL_OUT4_ADDR(base) ((base) + (0x4014))
#define PMIC_CL_IN0_ADDR(base) ((base) + (0x4015))
#define PMIC_CL_IN1_ADDR(base) ((base) + (0x4016))
#define PMIC_CL_IN2_ADDR(base) ((base) + (0x4017))
#define PMIC_CL_IN3_ADDR(base) ((base) + (0x4018))
#define PMIC_CL_IN4_ADDR(base) ((base) + (0x4019))
#define PMIC_CHG_TIMER0_ADDR(base) ((base) + (0x401A))
#define PMIC_CHG_TIMER1_ADDR(base) ((base) + (0x401B))
#define PMIC_CHG_TIMER2_ADDR(base) ((base) + (0x401C))
#define PMIC_CHG_TIMER3_ADDR(base) ((base) + (0x401D))
#define PMIC_LOAD_TIMER0_ADDR(base) ((base) + (0x401E))
#define PMIC_LOAD_TIMER1_ADDR(base) ((base) + (0x401F))
#define PMIC_LOAD_TIMER2_ADDR(base) ((base) + (0x4020))
#define PMIC_LOAD_TIMER3_ADDR(base) ((base) + (0x4021))
#define PMIC_CL_INT0_ADDR(base) ((base) + (0x4022))
#define PMIC_CL_INT1_ADDR(base) ((base) + (0x4023))
#define PMIC_CL_INT2_ADDR(base) ((base) + (0x4024))
#define PMIC_CL_INT3_ADDR(base) ((base) + (0x4025))
#define PMIC_CL_INT4_ADDR(base) ((base) + (0x4026))
#define PMIC_V_INT0_ADDR(base) ((base) + (0x4027))
#define PMIC_V_INT1_ADDR(base) ((base) + (0x4028))
#define PMIC_V_INT2_ADDR(base) ((base) + (0x4029))
#define PMIC_I_OUT_GATE0_ADDR(base) ((base) + (0x402A))
#define PMIC_I_OUT_GATE1_ADDR(base) ((base) + (0x402B))
#define PMIC_I_OUT_GATE2_ADDR(base) ((base) + (0x402C))
#define PMIC_I_IN_GATE0_ADDR(base) ((base) + (0x402D))
#define PMIC_I_IN_GATE1_ADDR(base) ((base) + (0x402E))
#define PMIC_I_IN_GATE2_ADDR(base) ((base) + (0x402F))
#define PMIC_OFFSET_CURRENT_MOD_0_ADDR(base) ((base) + (0x4030))
#define PMIC_OFFSET_CURRENT_MOD_1_ADDR(base) ((base) + (0x4031))
#define PMIC_OFFSET_CURRENT_MOD_2_ADDR(base) ((base) + (0x4032))
#define PMIC_OFFSET_VOLTAGE_MOD_0_ADDR(base) ((base) + (0x4033))
#define PMIC_OFFSET_VOLTAGE_MOD_1_ADDR(base) ((base) + (0x4034))
#define PMIC_OFFSET_VOLTAGE_MOD_2_ADDR(base) ((base) + (0x4035))
#define PMIC_STATE_TEST_ADDR(base) ((base) + (0x4036))
#define PMIC_CURRENT_0_ADDR(base) ((base) + (0x4037))
#define PMIC_CURRENT_1_ADDR(base) ((base) + (0x4038))
#define PMIC_CURRENT_2_ADDR(base) ((base) + (0x4039))
#define PMIC_V_OUT_0_ADDR(base) ((base) + (0x403A))
#define PMIC_V_OUT_1_ADDR(base) ((base) + (0x403B))
#define PMIC_V_OUT_2_ADDR(base) ((base) + (0x403C))
#define PMIC_OFFSET_CURRENT0_ADDR(base) ((base) + (0x403D))
#define PMIC_OFFSET_CURRENT1_ADDR(base) ((base) + (0x403E))
#define PMIC_OFFSET_CURRENT2_ADDR(base) ((base) + (0x403F))
#define PMIC_OFFSET_VOLTAGE0_ADDR(base) ((base) + (0x4040))
#define PMIC_OFFSET_VOLTAGE1_ADDR(base) ((base) + (0x4041))
#define PMIC_OFFSET_VOLTAGE2_ADDR(base) ((base) + (0x4042))
#define PMIC_OCV_VOLTAGE0_ADDR(base) ((base) + (0x4043))
#define PMIC_OCV_VOLTAGE1_ADDR(base) ((base) + (0x4044))
#define PMIC_OCV_VOLTAGE2_ADDR(base) ((base) + (0x4045))
#define PMIC_OCV_CURRENT0_ADDR(base) ((base) + (0x4046))
#define PMIC_OCV_CURRENT1_ADDR(base) ((base) + (0x4047))
#define PMIC_OCV_CURRENT2_ADDR(base) ((base) + (0x4048))
#define PMIC_OCV_TEMP0_ADDR(base) ((base) + (0x4049))
#define PMIC_OCV_TEMP1_ADDR(base) ((base) + (0x404A))
#define PMIC_OCV_TEMP2_ADDR(base) ((base) + (0x404B))
#define PMIC_ECO_OUT_CLIN_0_ADDR(base) ((base) + (0x404C))
#define PMIC_ECO_OUT_CLIN_1_ADDR(base) ((base) + (0x404D))
#define PMIC_ECO_OUT_CLIN_2_ADDR(base) ((base) + (0x404E))
#define PMIC_ECO_OUT_CLIN_3_ADDR(base) ((base) + (0x404F))
#define PMIC_ECO_OUT_CLIN_4_ADDR(base) ((base) + (0x4050))
#define PMIC_ECO_OUT_CLOUT_0_ADDR(base) ((base) + (0x4051))
#define PMIC_ECO_OUT_CLOUT_1_ADDR(base) ((base) + (0x4052))
#define PMIC_ECO_OUT_CLOUT_2_ADDR(base) ((base) + (0x4053))
#define PMIC_ECO_OUT_CLOUT_3_ADDR(base) ((base) + (0x4054))
#define PMIC_ECO_OUT_CLOUT_4_ADDR(base) ((base) + (0x4055))
#define PMIC_ECO_OUT_TEMP_0_ADDR(base) ((base) + (0x4056))
#define PMIC_ECO_OUT_TEMP_1_ADDR(base) ((base) + (0x4057))
#define PMIC_ECO_OUT_TEMP_2_ADDR(base) ((base) + (0x4058))
#define PMIC_TEMP0_RDATA_ADDR(base) ((base) + (0x4059))
#define PMIC_TEMP1_RDATA_ADDR(base) ((base) + (0x405A))
#define PMIC_TEMP2_RDATA_ADDR(base) ((base) + (0x405B))
#define PMIC_V_PRE0_OUT0_ADDR(base) ((base) + (0x405C))
#define PMIC_V_PRE0_OUT1_ADDR(base) ((base) + (0x405D))
#define PMIC_V_PRE0_OUT2_ADDR(base) ((base) + (0x405E))
#define PMIC_V_PRE1_OUT0_ADDR(base) ((base) + (0x405F))
#define PMIC_V_PRE1_OUT1_ADDR(base) ((base) + (0x4060))
#define PMIC_V_PRE1_OUT2_ADDR(base) ((base) + (0x4061))
#define PMIC_V_PRE2_OUT0_ADDR(base) ((base) + (0x4062))
#define PMIC_V_PRE2_OUT1_ADDR(base) ((base) + (0x4063))
#define PMIC_V_PRE2_OUT2_ADDR(base) ((base) + (0x4064))
#define PMIC_V_PRE3_OUT0_ADDR(base) ((base) + (0x4065))
#define PMIC_V_PRE3_OUT1_ADDR(base) ((base) + (0x4066))
#define PMIC_V_PRE3_OUT2_ADDR(base) ((base) + (0x4067))
#define PMIC_V_PRE4_OUT0_ADDR(base) ((base) + (0x4068))
#define PMIC_V_PRE4_OUT1_ADDR(base) ((base) + (0x4069))
#define PMIC_V_PRE4_OUT2_ADDR(base) ((base) + (0x406A))
#define PMIC_V_PRE5_OUT0_ADDR(base) ((base) + (0x406B))
#define PMIC_V_PRE5_OUT1_ADDR(base) ((base) + (0x406C))
#define PMIC_V_PRE5_OUT2_ADDR(base) ((base) + (0x406D))
#define PMIC_V_PRE6_OUT0_ADDR(base) ((base) + (0x406E))
#define PMIC_V_PRE6_OUT1_ADDR(base) ((base) + (0x406F))
#define PMIC_V_PRE6_OUT2_ADDR(base) ((base) + (0x4070))
#define PMIC_V_PRE7_OUT0_ADDR(base) ((base) + (0x4071))
#define PMIC_V_PRE7_OUT1_ADDR(base) ((base) + (0x4072))
#define PMIC_V_PRE7_OUT2_ADDR(base) ((base) + (0x4073))
#define PMIC_CURRENT_PRE0_OUT0_ADDR(base) ((base) + (0x4074))
#define PMIC_CURRENT_PRE0_OUT1_ADDR(base) ((base) + (0x4075))
#define PMIC_CURRENT_PRE0_OUT2_ADDR(base) ((base) + (0x4076))
#define PMIC_CURRENT_PRE1_OUT0_ADDR(base) ((base) + (0x4077))
#define PMIC_CURRENT_PRE1_OUT1_ADDR(base) ((base) + (0x4078))
#define PMIC_CURRENT_PRE1_OUT2_ADDR(base) ((base) + (0x4079))
#define PMIC_CURRENT_PRE2_OUT0_ADDR(base) ((base) + (0x407A))
#define PMIC_CURRENT_PRE2_OUT1_ADDR(base) ((base) + (0x407B))
#define PMIC_CURRENT_PRE2_OUT2_ADDR(base) ((base) + (0x407C))
#define PMIC_CURRENT_PRE3_OUT0_ADDR(base) ((base) + (0x407D))
#define PMIC_CURRENT_PRE3_OUT1_ADDR(base) ((base) + (0x407E))
#define PMIC_CURRENT_PRE3_OUT2_ADDR(base) ((base) + (0x407F))
#define PMIC_CURRENT_PRE4_OUT0_ADDR(base) ((base) + (0x4080))
#define PMIC_CURRENT_PRE4_OUT1_ADDR(base) ((base) + (0x4081))
#define PMIC_CURRENT_PRE4_OUT2_ADDR(base) ((base) + (0x4082))
#define PMIC_CURRENT_PRE5_OUT0_ADDR(base) ((base) + (0x4083))
#define PMIC_CURRENT_PRE5_OUT1_ADDR(base) ((base) + (0x4084))
#define PMIC_CURRENT_PRE5_OUT2_ADDR(base) ((base) + (0x4085))
#define PMIC_CURRENT_PRE6_OUT0_ADDR(base) ((base) + (0x4086))
#define PMIC_CURRENT_PRE6_OUT1_ADDR(base) ((base) + (0x4087))
#define PMIC_CURRENT_PRE6_OUT2_ADDR(base) ((base) + (0x4088))
#define PMIC_CURRENT_PRE7_OUT0_ADDR(base) ((base) + (0x4089))
#define PMIC_CURRENT_PRE7_OUT1_ADDR(base) ((base) + (0x408A))
#define PMIC_CURRENT_PRE7_OUT2_ADDR(base) ((base) + (0x408B))
#define PMIC_V_OCV_PRE1_OUT0_ADDR(base) ((base) + (0x408C))
#define PMIC_V_OCV_PRE1_OUT1_ADDR(base) ((base) + (0x408D))
#define PMIC_V_OCV_PRE1_OUT2_ADDR(base) ((base) + (0x408E))
#define PMIC_V_OCV_PRE2_OUT0_ADDR(base) ((base) + (0x408F))
#define PMIC_V_OCV_PRE2_OUT1_ADDR(base) ((base) + (0x4090))
#define PMIC_V_OCV_PRE2_OUT2_ADDR(base) ((base) + (0x4091))
#define PMIC_V_OCV_PRE3_OUT0_ADDR(base) ((base) + (0x4092))
#define PMIC_V_OCV_PRE3_OUT1_ADDR(base) ((base) + (0x4093))
#define PMIC_V_OCV_PRE3_OUT2_ADDR(base) ((base) + (0x4094))
#define PMIC_V_OCV_PRE4_OUT0_ADDR(base) ((base) + (0x4095))
#define PMIC_V_OCV_PRE4_OUT1_ADDR(base) ((base) + (0x4096))
#define PMIC_V_OCV_PRE4_OUT2_ADDR(base) ((base) + (0x4097))
#define PMIC_I_OCV_PRE1_OUT0_ADDR(base) ((base) + (0x4098))
#define PMIC_I_OCV_PRE1_OUT1_ADDR(base) ((base) + (0x4099))
#define PMIC_I_OCV_PRE1_OUT2_ADDR(base) ((base) + (0x409A))
#define PMIC_I_OCV_PRE2_OUT0_ADDR(base) ((base) + (0x409B))
#define PMIC_I_OCV_PRE2_OUT1_ADDR(base) ((base) + (0x409C))
#define PMIC_I_OCV_PRE2_OUT2_ADDR(base) ((base) + (0x409D))
#define PMIC_I_OCV_PRE3_OUT0_ADDR(base) ((base) + (0x409E))
#define PMIC_I_OCV_PRE3_OUT1_ADDR(base) ((base) + (0x409F))
#define PMIC_I_OCV_PRE3_OUT2_ADDR(base) ((base) + (0x40A0))
#define PMIC_I_OCV_PRE4_OUT0_ADDR(base) ((base) + (0x40A1))
#define PMIC_I_OCV_PRE4_OUT1_ADDR(base) ((base) + (0x40A2))
#define PMIC_I_OCV_PRE4_OUT2_ADDR(base) ((base) + (0x40A3))
#define PMIC_T_OCV_PRE1_OUT0_ADDR(base) ((base) + (0x40A4))
#define PMIC_T_OCV_PRE1_OUT1_ADDR(base) ((base) + (0x40A5))
#define PMIC_T_OCV_PRE2_OUT0_ADDR(base) ((base) + (0x40A6))
#define PMIC_T_OCV_PRE2_OUT1_ADDR(base) ((base) + (0x40A7))
#define PMIC_T_OCV_PRE3_OUT0_ADDR(base) ((base) + (0x40A8))
#define PMIC_T_OCV_PRE3_OUT1_ADDR(base) ((base) + (0x40A9))
#define PMIC_T_OCV_PRE4_OUT0_ADDR(base) ((base) + (0x40AA))
#define PMIC_T_OCV_PRE4_OUT1_ADDR(base) ((base) + (0x40AB))
#define PMIC_RTC_OCV_OUT_ADDR(base) ((base) + (0x40AC))
#define PMIC_SLIDE_I_DATA0_OUT0_ADDR(base) ((base) + (0x40AD))
#define PMIC_SLIDE_I_DATA0_OUT1_ADDR(base) ((base) + (0x40AE))
#define PMIC_SLIDE_I_DATA0_OUT2_ADDR(base) ((base) + (0x40AF))
#define PMIC_SLIDE_I_DATA1_OUT0_ADDR(base) ((base) + (0x40B0))
#define PMIC_SLIDE_I_DATA1_OUT1_ADDR(base) ((base) + (0x40B1))
#define PMIC_SLIDE_I_DATA1_OUT2_ADDR(base) ((base) + (0x40B2))
#define PMIC_SLIDE_I_DATA2_OUT0_ADDR(base) ((base) + (0x40B3))
#define PMIC_SLIDE_I_DATA2_OUT1_ADDR(base) ((base) + (0x40B4))
#define PMIC_SLIDE_I_DATA2_OUT2_ADDR(base) ((base) + (0x40B5))
#define PMIC_SLIDE_I_DATA3_OUT0_ADDR(base) ((base) + (0x40B6))
#define PMIC_SLIDE_I_DATA3_OUT1_ADDR(base) ((base) + (0x40B7))
#define PMIC_SLIDE_I_DATA3_OUT2_ADDR(base) ((base) + (0x40B8))
#define PMIC_SLIDE_V_DATA0_OUT0_ADDR(base) ((base) + (0x40B9))
#define PMIC_SLIDE_V_DATA0_OUT1_ADDR(base) ((base) + (0x40BA))
#define PMIC_SLIDE_V_DATA0_OUT2_ADDR(base) ((base) + (0x40BB))
#define PMIC_SLIDE_V_DATA1_OUT0_ADDR(base) ((base) + (0x40BC))
#define PMIC_SLIDE_V_DATA1_OUT1_ADDR(base) ((base) + (0x40BD))
#define PMIC_SLIDE_V_DATA1_OUT2_ADDR(base) ((base) + (0x40BE))
#define PMIC_SLIDE_V_DATA2_OUT0_ADDR(base) ((base) + (0x40BF))
#define PMIC_SLIDE_V_DATA2_OUT1_ADDR(base) ((base) + (0x40C0))
#define PMIC_SLIDE_V_DATA2_OUT2_ADDR(base) ((base) + (0x40C1))
#define PMIC_SLIDE_V_DATA3_OUT0_ADDR(base) ((base) + (0x40C2))
#define PMIC_SLIDE_V_DATA3_OUT1_ADDR(base) ((base) + (0x40C3))
#define PMIC_SLIDE_V_DATA3_OUT2_ADDR(base) ((base) + (0x40C4))
#define PMIC_SLIDE_CNT_DATA_OUT0_ADDR(base) ((base) + (0x40C5))
#define PMIC_SLIDE_CNT_DATA_OUT1_ADDR(base) ((base) + (0x40C6))
#define PMIC_SLIDE_CNT_DATA_OUT2_ADDR(base) ((base) + (0x40C7))
#define PMIC_COUL_RESERVE0_ADDR(base) ((base) + (0x40C8))
#define PMIC_COUL_RESERVE1_ADDR(base) ((base) + (0x40C9))
#define PMIC_CLJ_DEBUG0_ADDR(base) ((base) + (0x40CA))
#define PMIC_CLJ_DEBUG1_ADDR(base) ((base) + (0x40CB))
#define PMIC_DEBUG_CIC_I0_ADDR(base) ((base) + (0x40CC))
#define PMIC_DEBUG_CIC_I1_ADDR(base) ((base) + (0x40CD))
#define PMIC_DEBUG_CIC_I2_ADDR(base) ((base) + (0x40CE))
#define PMIC_DEBUG_CIC_V0_ADDR(base) ((base) + (0x40CF))
#define PMIC_DEBUG_CIC_V1_ADDR(base) ((base) + (0x40D0))
#define PMIC_DEBUG_CIC_V2_ADDR(base) ((base) + (0x40D1))
#define PMIC_DEBUG_WRITE_PRO_ADDR(base) ((base) + (0x40D2))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_RTCDR0_ADDR(base) ((base) + (0x6000UL))
#define PMIC_RTCDR1_ADDR(base) ((base) + (0x6001UL))
#define PMIC_RTCDR2_ADDR(base) ((base) + (0x6002UL))
#define PMIC_RTCDR3_ADDR(base) ((base) + (0x6003UL))
#define PMIC_RTCMR0_ADDR(base) ((base) + (0x6004UL))
#define PMIC_RTCMR1_ADDR(base) ((base) + (0x6005UL))
#define PMIC_RTCMR2_ADDR(base) ((base) + (0x6006UL))
#define PMIC_RTCMR3_ADDR(base) ((base) + (0x6007UL))
#define PMIC_RTCLR0_ADDR(base) ((base) + (0x6008UL))
#define PMIC_RTCLR1_ADDR(base) ((base) + (0x6009UL))
#define PMIC_RTCLR2_ADDR(base) ((base) + (0x600AUL))
#define PMIC_RTCLR3_ADDR(base) ((base) + (0x600BUL))
#define PMIC_RTCCTRL_ADDR(base) ((base) + (0x600CUL))
#define PMIC_CRC_VAULE0_ADDR(base) ((base) + (0x600DUL))
#define PMIC_CRC_VAULE1_ADDR(base) ((base) + (0x600EUL))
#define PMIC_CRC_VAULE2_ADDR(base) ((base) + (0x600FUL))
#define PMIC_RTC_PWRUP_TIMER0_ADDR(base) ((base) + (0x6010UL))
#define PMIC_RTC_PWRUP_TIMER1_ADDR(base) ((base) + (0x6011UL))
#define PMIC_RTC_PWRUP_TIMER2_ADDR(base) ((base) + (0x6012UL))
#define PMIC_RTC_PWRUP_TIMER3_ADDR(base) ((base) + (0x6013UL))
#define PMIC_RTC_PWRDOWN_TIMER0_ADDR(base) ((base) + (0x6014UL))
#define PMIC_RTC_PWRDOWN_TIMER1_ADDR(base) ((base) + (0x6015UL))
#define PMIC_RTC_PWRDOWN_TIMER2_ADDR(base) ((base) + (0x6016UL))
#define PMIC_RTC_PWRDOWN_TIMER3_ADDR(base) ((base) + (0x6017UL))
#else
#define PMIC_RTCDR0_ADDR(base) ((base) + (0x6000))
#define PMIC_RTCDR1_ADDR(base) ((base) + (0x6001))
#define PMIC_RTCDR2_ADDR(base) ((base) + (0x6002))
#define PMIC_RTCDR3_ADDR(base) ((base) + (0x6003))
#define PMIC_RTCMR0_ADDR(base) ((base) + (0x6004))
#define PMIC_RTCMR1_ADDR(base) ((base) + (0x6005))
#define PMIC_RTCMR2_ADDR(base) ((base) + (0x6006))
#define PMIC_RTCMR3_ADDR(base) ((base) + (0x6007))
#define PMIC_RTCLR0_ADDR(base) ((base) + (0x6008))
#define PMIC_RTCLR1_ADDR(base) ((base) + (0x6009))
#define PMIC_RTCLR2_ADDR(base) ((base) + (0x600A))
#define PMIC_RTCLR3_ADDR(base) ((base) + (0x600B))
#define PMIC_RTCCTRL_ADDR(base) ((base) + (0x600C))
#define PMIC_CRC_VAULE0_ADDR(base) ((base) + (0x600D))
#define PMIC_CRC_VAULE1_ADDR(base) ((base) + (0x600E))
#define PMIC_CRC_VAULE2_ADDR(base) ((base) + (0x600F))
#define PMIC_RTC_PWRUP_TIMER0_ADDR(base) ((base) + (0x6010))
#define PMIC_RTC_PWRUP_TIMER1_ADDR(base) ((base) + (0x6011))
#define PMIC_RTC_PWRUP_TIMER2_ADDR(base) ((base) + (0x6012))
#define PMIC_RTC_PWRUP_TIMER3_ADDR(base) ((base) + (0x6013))
#define PMIC_RTC_PWRDOWN_TIMER0_ADDR(base) ((base) + (0x6014))
#define PMIC_RTC_PWRDOWN_TIMER1_ADDR(base) ((base) + (0x6015))
#define PMIC_RTC_PWRDOWN_TIMER2_ADDR(base) ((base) + (0x6016))
#define PMIC_RTC_PWRDOWN_TIMER3_ADDR(base) ((base) + (0x6017))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_ACR_CFG0_ADDR(base) ((base) + (0x8000UL))
#define PMIC_ACR_CFG1_ADDR(base) ((base) + (0x8001UL))
#define PMIC_ACR_RESERVE_CFG_ADDR(base) ((base) + (0x8002UL))
#define PMIC_ACRADC_CTRL_ADDR(base) ((base) + (0x8010UL))
#define PMIC_ACRADC_START_ADDR(base) ((base) + (0x8011UL))
#define PMIC_ACRCONV_STATUS_ADDR(base) ((base) + (0x8012UL))
#define PMIC_ACRADC_DATA_L_ADDR(base) ((base) + (0x8013UL))
#define PMIC_ACRADC_DATA_H_ADDR(base) ((base) + (0x8014UL))
#define PMIC_ACRADC_TDIE_DATA_L_ADDR(base) ((base) + (0x8015UL))
#define PMIC_ACRADC_TDIE_DATA_H_ADDR(base) ((base) + (0x8016UL))
#define PMIC_ACRADC_TBAT_DATA_L_ADDR(base) ((base) + (0x8017UL))
#define PMIC_ACRADC_TBAT_DATA_H_ADDR(base) ((base) + (0x8018UL))
#define PMIC_ACRADC_CONV_ADDR(base) ((base) + (0x8019UL))
#define PMIC_ACRADC_CURRENT_ADDR(base) ((base) + (0x801AUL))
#define PMIC_ACRADC_CALI_CTRL_ADDR(base) ((base) + (0x801BUL))
#define PMIC_ACRADC_CALI_VALUE_ADDR(base) ((base) + (0x801CUL))
#define PMIC_ACRADC_CALI_CFG_ADDR(base) ((base) + (0x801DUL))
#define PMIC_ACRADC_TDIE_CALI_DATA_ADDR(base) ((base) + (0x801EUL))
#define PMIC_ACRADC_TBAT_CALI_DATA_ADDR(base) ((base) + (0x801FUL))
#define PMIC_ACRADC_TDIE_CALI_CFG_ADDR(base) ((base) + (0x8020UL))
#define PMIC_ACRADC_TBAT_CALI_CFG_ADDR(base) ((base) + (0x8021UL))
#define PMIC_ACRADC_MODE_CFG_ADDR(base) ((base) + (0x8022UL))
#define PMIC_ACRADC_CALIVALUE_CFG1_ADDR(base) ((base) + (0x8023UL))
#define PMIC_ACRADC_CALIVALUE_CFG2_ADDR(base) ((base) + (0x8024UL))
#define PMIC_ACRADC_TDIE_OFFSET_CFGL_ADDR(base) ((base) + (0x8025UL))
#define PMIC_ACRADC_TDIE_OFFSET_CFGH_ADDR(base) ((base) + (0x8026UL))
#define PMIC_ACRADC_TBAT_OFFSET_CFGL_ADDR(base) ((base) + (0x8027UL))
#define PMIC_ACRADC_TBAT_OFFSET_CFGH_ADDR(base) ((base) + (0x8028UL))
#define PMIC_SOH_EN_ADDR(base) ((base) + (0x8040UL))
#define PMIC_SOH_DET_TIMER_ADDR(base) ((base) + (0x8041UL))
#define PMIC_SOH_TBAT_OVH_L_ADDR(base) ((base) + (0x8042UL))
#define PMIC_SOH_TBAT_OVH_H_ADDR(base) ((base) + (0x8043UL))
#define PMIC_SOH_VBAT_UVP_L_ADDR(base) ((base) + (0x8044UL))
#define PMIC_SOH_VBAT_UVP_H_ADDR(base) ((base) + (0x8045UL))
#define PMIC_SOH_MODE_ADDR(base) ((base) + (0x8060UL))
#define PMIC_ACR_PULSE_NUM_ADDR(base) ((base) + (0x8061UL))
#define PMIC_ACR_SAMPLE_TIME_H_ADDR(base) ((base) + (0x8062UL))
#define PMIC_ACR_SAMPLE_TIME_L_ADDR(base) ((base) + (0x8063UL))
#define PMIC_ACR_DATA0_L_ADDR(base) ((base) + (0x8064UL))
#define PMIC_ACR_DATA0_H_ADDR(base) ((base) + (0x8065UL))
#define PMIC_ACR_DATA1_L_ADDR(base) ((base) + (0x8066UL))
#define PMIC_ACR_DATA1_H_ADDR(base) ((base) + (0x8067UL))
#define PMIC_ACR_DATA2_L_ADDR(base) ((base) + (0x8068UL))
#define PMIC_ACR_DATA2_H_ADDR(base) ((base) + (0x8069UL))
#define PMIC_ACR_DATA3_L_ADDR(base) ((base) + (0x806AUL))
#define PMIC_ACR_DATA3_H_ADDR(base) ((base) + (0x806BUL))
#define PMIC_ACR_DATA4_L_ADDR(base) ((base) + (0x806CUL))
#define PMIC_ACR_DATA4_H_ADDR(base) ((base) + (0x806DUL))
#define PMIC_ACR_DATA5_L_ADDR(base) ((base) + (0x806EUL))
#define PMIC_ACR_DATA5_H_ADDR(base) ((base) + (0x806FUL))
#define PMIC_ACR_DATA6_L_ADDR(base) ((base) + (0x8070UL))
#define PMIC_ACR_DATA6_H_ADDR(base) ((base) + (0x8071UL))
#define PMIC_ACR_DATA7_L_ADDR(base) ((base) + (0x8072UL))
#define PMIC_ACR_DATA7_H_ADDR(base) ((base) + (0x8073UL))
#define PMIC_OVP_CTRL_ADDR(base) ((base) + (0x8090UL))
#define PMIC_OVP_VBAT_OVH_TH0_L_ADDR(base) ((base) + (0x8091UL))
#define PMIC_OVP_VBAT_OVH_TH0_H_ADDR(base) ((base) + (0x8092UL))
#define PMIC_OVP_TBAT_OVH_TH0_L_ADDR(base) ((base) + (0x8093UL))
#define PMIC_OVP_TBAT_OVH_TH0_H_ADDR(base) ((base) + (0x8094UL))
#define PMIC_OVP_VBAT_OVH_TH1_L_ADDR(base) ((base) + (0x8095UL))
#define PMIC_OVP_VBAT_OVH_TH1_H_ADDR(base) ((base) + (0x8096UL))
#define PMIC_OVP_TBAT_OVH_TH1_L_ADDR(base) ((base) + (0x8097UL))
#define PMIC_OVP_TBAT_OVH_TH1_H_ADDR(base) ((base) + (0x8098UL))
#define PMIC_OVP_VBAT_OVH_TH2_L_ADDR(base) ((base) + (0x8099UL))
#define PMIC_OVP_VBAT_OVH_TH2_H_ADDR(base) ((base) + (0x809AUL))
#define PMIC_OVP_TBAT_OVH_TH2_L_ADDR(base) ((base) + (0x809BUL))
#define PMIC_OVP_TBAT_OVH_TH2_H_ADDR(base) ((base) + (0x809CUL))
#define PMIC_SOH_OVP_REAL_ADDR(base) ((base) + (0x809DUL))
#define PMIC_OVP_DISCHARGE_CTRL_ADDR(base) ((base) + (0x809EUL))
#define PMIC_DCR_CONFIG_ADDR(base) ((base) + (0x80B0UL))
#define PMIC_DCR_TIMER_CONFIG0_ADDR(base) ((base) + (0x80B1UL))
#define PMIC_DCR_TIMER_CONFIG1_ADDR(base) ((base) + (0x80B2UL))
#define PMIC_DCR_COUNT_CONFIG_ADDR(base) ((base) + (0x80B3UL))
#define PMIC_IBAT1_0_ADDR(base) ((base) + (0x80B4UL))
#define PMIC_IBAT1_1_ADDR(base) ((base) + (0x80B5UL))
#define PMIC_IBAT1_2_ADDR(base) ((base) + (0x80B6UL))
#define PMIC_VBAT1_0_ADDR(base) ((base) + (0x80B7UL))
#define PMIC_VBAT1_1_ADDR(base) ((base) + (0x80B8UL))
#define PMIC_VBAT1_2_ADDR(base) ((base) + (0x80B9UL))
#define PMIC_TMR1_0_ADDR(base) ((base) + (0x80BAUL))
#define PMIC_TMR1_1_ADDR(base) ((base) + (0x80BBUL))
#define PMIC_IBAT2_0_ADDR(base) ((base) + (0x80BCUL))
#define PMIC_IBAT2_1_ADDR(base) ((base) + (0x80BDUL))
#define PMIC_IBAT2_2_ADDR(base) ((base) + (0x80BEUL))
#define PMIC_VBAT2_0_ADDR(base) ((base) + (0x80BFUL))
#define PMIC_VBAT2_1_ADDR(base) ((base) + (0x80C0UL))
#define PMIC_VBAT2_2_ADDR(base) ((base) + (0x80C1UL))
#define PMIC_TMR2_0_ADDR(base) ((base) + (0x80C2UL))
#define PMIC_TMR2_1_ADDR(base) ((base) + (0x80C3UL))
#define PMIC_SOH_SOFT_RST_ADDR(base) ((base) + (0x80E0UL))
#define PMIC_TEST_BUS_SEL_ADDR(base) ((base) + (0x80E1UL))
#define PMIC_ACR_TB_BUS_0_ADDR(base) ((base) + (0x80E2UL))
#define PMIC_ACR_TB_BUS_1_ADDR(base) ((base) + (0x80E3UL))
#define PMIC_ACR_CLK_GT_EN_ADDR(base) ((base) + (0x80E4UL))
#else
#define PMIC_ACR_CFG0_ADDR(base) ((base) + (0x8000))
#define PMIC_ACR_CFG1_ADDR(base) ((base) + (0x8001))
#define PMIC_ACR_RESERVE_CFG_ADDR(base) ((base) + (0x8002))
#define PMIC_ACRADC_CTRL_ADDR(base) ((base) + (0x8010))
#define PMIC_ACRADC_START_ADDR(base) ((base) + (0x8011))
#define PMIC_ACRCONV_STATUS_ADDR(base) ((base) + (0x8012))
#define PMIC_ACRADC_DATA_L_ADDR(base) ((base) + (0x8013))
#define PMIC_ACRADC_DATA_H_ADDR(base) ((base) + (0x8014))
#define PMIC_ACRADC_TDIE_DATA_L_ADDR(base) ((base) + (0x8015))
#define PMIC_ACRADC_TDIE_DATA_H_ADDR(base) ((base) + (0x8016))
#define PMIC_ACRADC_TBAT_DATA_L_ADDR(base) ((base) + (0x8017))
#define PMIC_ACRADC_TBAT_DATA_H_ADDR(base) ((base) + (0x8018))
#define PMIC_ACRADC_CONV_ADDR(base) ((base) + (0x8019))
#define PMIC_ACRADC_CURRENT_ADDR(base) ((base) + (0x801A))
#define PMIC_ACRADC_CALI_CTRL_ADDR(base) ((base) + (0x801B))
#define PMIC_ACRADC_CALI_VALUE_ADDR(base) ((base) + (0x801C))
#define PMIC_ACRADC_CALI_CFG_ADDR(base) ((base) + (0x801D))
#define PMIC_ACRADC_TDIE_CALI_DATA_ADDR(base) ((base) + (0x801E))
#define PMIC_ACRADC_TBAT_CALI_DATA_ADDR(base) ((base) + (0x801F))
#define PMIC_ACRADC_TDIE_CALI_CFG_ADDR(base) ((base) + (0x8020))
#define PMIC_ACRADC_TBAT_CALI_CFG_ADDR(base) ((base) + (0x8021))
#define PMIC_ACRADC_MODE_CFG_ADDR(base) ((base) + (0x8022))
#define PMIC_ACRADC_CALIVALUE_CFG1_ADDR(base) ((base) + (0x8023))
#define PMIC_ACRADC_CALIVALUE_CFG2_ADDR(base) ((base) + (0x8024))
#define PMIC_ACRADC_TDIE_OFFSET_CFGL_ADDR(base) ((base) + (0x8025))
#define PMIC_ACRADC_TDIE_OFFSET_CFGH_ADDR(base) ((base) + (0x8026))
#define PMIC_ACRADC_TBAT_OFFSET_CFGL_ADDR(base) ((base) + (0x8027))
#define PMIC_ACRADC_TBAT_OFFSET_CFGH_ADDR(base) ((base) + (0x8028))
#define PMIC_SOH_EN_ADDR(base) ((base) + (0x8040))
#define PMIC_SOH_DET_TIMER_ADDR(base) ((base) + (0x8041))
#define PMIC_SOH_TBAT_OVH_L_ADDR(base) ((base) + (0x8042))
#define PMIC_SOH_TBAT_OVH_H_ADDR(base) ((base) + (0x8043))
#define PMIC_SOH_VBAT_UVP_L_ADDR(base) ((base) + (0x8044))
#define PMIC_SOH_VBAT_UVP_H_ADDR(base) ((base) + (0x8045))
#define PMIC_SOH_MODE_ADDR(base) ((base) + (0x8060))
#define PMIC_ACR_PULSE_NUM_ADDR(base) ((base) + (0x8061))
#define PMIC_ACR_SAMPLE_TIME_H_ADDR(base) ((base) + (0x8062))
#define PMIC_ACR_SAMPLE_TIME_L_ADDR(base) ((base) + (0x8063))
#define PMIC_ACR_DATA0_L_ADDR(base) ((base) + (0x8064))
#define PMIC_ACR_DATA0_H_ADDR(base) ((base) + (0x8065))
#define PMIC_ACR_DATA1_L_ADDR(base) ((base) + (0x8066))
#define PMIC_ACR_DATA1_H_ADDR(base) ((base) + (0x8067))
#define PMIC_ACR_DATA2_L_ADDR(base) ((base) + (0x8068))
#define PMIC_ACR_DATA2_H_ADDR(base) ((base) + (0x8069))
#define PMIC_ACR_DATA3_L_ADDR(base) ((base) + (0x806A))
#define PMIC_ACR_DATA3_H_ADDR(base) ((base) + (0x806B))
#define PMIC_ACR_DATA4_L_ADDR(base) ((base) + (0x806C))
#define PMIC_ACR_DATA4_H_ADDR(base) ((base) + (0x806D))
#define PMIC_ACR_DATA5_L_ADDR(base) ((base) + (0x806E))
#define PMIC_ACR_DATA5_H_ADDR(base) ((base) + (0x806F))
#define PMIC_ACR_DATA6_L_ADDR(base) ((base) + (0x8070))
#define PMIC_ACR_DATA6_H_ADDR(base) ((base) + (0x8071))
#define PMIC_ACR_DATA7_L_ADDR(base) ((base) + (0x8072))
#define PMIC_ACR_DATA7_H_ADDR(base) ((base) + (0x8073))
#define PMIC_OVP_CTRL_ADDR(base) ((base) + (0x8090))
#define PMIC_OVP_VBAT_OVH_TH0_L_ADDR(base) ((base) + (0x8091))
#define PMIC_OVP_VBAT_OVH_TH0_H_ADDR(base) ((base) + (0x8092))
#define PMIC_OVP_TBAT_OVH_TH0_L_ADDR(base) ((base) + (0x8093))
#define PMIC_OVP_TBAT_OVH_TH0_H_ADDR(base) ((base) + (0x8094))
#define PMIC_OVP_VBAT_OVH_TH1_L_ADDR(base) ((base) + (0x8095))
#define PMIC_OVP_VBAT_OVH_TH1_H_ADDR(base) ((base) + (0x8096))
#define PMIC_OVP_TBAT_OVH_TH1_L_ADDR(base) ((base) + (0x8097))
#define PMIC_OVP_TBAT_OVH_TH1_H_ADDR(base) ((base) + (0x8098))
#define PMIC_OVP_VBAT_OVH_TH2_L_ADDR(base) ((base) + (0x8099))
#define PMIC_OVP_VBAT_OVH_TH2_H_ADDR(base) ((base) + (0x809A))
#define PMIC_OVP_TBAT_OVH_TH2_L_ADDR(base) ((base) + (0x809B))
#define PMIC_OVP_TBAT_OVH_TH2_H_ADDR(base) ((base) + (0x809C))
#define PMIC_SOH_OVP_REAL_ADDR(base) ((base) + (0x809D))
#define PMIC_OVP_DISCHARGE_CTRL_ADDR(base) ((base) + (0x809E))
#define PMIC_DCR_CONFIG_ADDR(base) ((base) + (0x80B0))
#define PMIC_DCR_TIMER_CONFIG0_ADDR(base) ((base) + (0x80B1))
#define PMIC_DCR_TIMER_CONFIG1_ADDR(base) ((base) + (0x80B2))
#define PMIC_DCR_COUNT_CONFIG_ADDR(base) ((base) + (0x80B3))
#define PMIC_IBAT1_0_ADDR(base) ((base) + (0x80B4))
#define PMIC_IBAT1_1_ADDR(base) ((base) + (0x80B5))
#define PMIC_IBAT1_2_ADDR(base) ((base) + (0x80B6))
#define PMIC_VBAT1_0_ADDR(base) ((base) + (0x80B7))
#define PMIC_VBAT1_1_ADDR(base) ((base) + (0x80B8))
#define PMIC_VBAT1_2_ADDR(base) ((base) + (0x80B9))
#define PMIC_TMR1_0_ADDR(base) ((base) + (0x80BA))
#define PMIC_TMR1_1_ADDR(base) ((base) + (0x80BB))
#define PMIC_IBAT2_0_ADDR(base) ((base) + (0x80BC))
#define PMIC_IBAT2_1_ADDR(base) ((base) + (0x80BD))
#define PMIC_IBAT2_2_ADDR(base) ((base) + (0x80BE))
#define PMIC_VBAT2_0_ADDR(base) ((base) + (0x80BF))
#define PMIC_VBAT2_1_ADDR(base) ((base) + (0x80C0))
#define PMIC_VBAT2_2_ADDR(base) ((base) + (0x80C1))
#define PMIC_TMR2_0_ADDR(base) ((base) + (0x80C2))
#define PMIC_TMR2_1_ADDR(base) ((base) + (0x80C3))
#define PMIC_SOH_SOFT_RST_ADDR(base) ((base) + (0x80E0))
#define PMIC_TEST_BUS_SEL_ADDR(base) ((base) + (0x80E1))
#define PMIC_ACR_TB_BUS_0_ADDR(base) ((base) + (0x80E2))
#define PMIC_ACR_TB_BUS_1_ADDR(base) ((base) + (0x80E3))
#define PMIC_ACR_CLK_GT_EN_ADDR(base) ((base) + (0x80E4))
#endif
#ifndef __SOC_H_FOR_ASM__
#define PMIC_EIS_ANA_CTRL0_ADDR(base) ((base) + (0xA000UL))
#define PMIC_EIS_ANA_CTRL1_ADDR(base) ((base) + (0xA001UL))
#define PMIC_EIS_ANA_CTRL2_ADDR(base) ((base) + (0xA002UL))
#define PMIC_EIS_IBIAS_SET_0_ADDR(base) ((base) + (0xA003UL))
#define PMIC_EIS_IBIAS_SET_1_ADDR(base) ((base) + (0xA004UL))
#define PMIC_EIS_TEST_CTRL_ADDR(base) ((base) + (0xA005UL))
#define PMIC_EIS_RESERVE0_ADDR(base) ((base) + (0xA006UL))
#define PMIC_EIS_RESERVE1_ADDR(base) ((base) + (0xA007UL))
#define PMIC_EIS_RESERVE2_ADDR(base) ((base) + (0xA008UL))
#define PMIC_EIS_CTRL0_ADDR(base) ((base) + (0xA010UL))
#define PMIC_EIS_CTRL1_ADDR(base) ((base) + (0xA011UL))
#define PMIC_EIS_CTRL2_ADDR(base) ((base) + (0xA012UL))
#define PMIC_EISADC_INIT_V_OFFSET_DATA_L_ADDR(base) ((base) + (0xA013UL))
#define PMIC_EISADC_INIT_V_OFFSET_DATA_H_ADDR(base) ((base) + (0xA014UL))
#define PMIC_EISADC_INIT_I_OFFSET_DATA_L_ADDR(base) ((base) + (0xA015UL))
#define PMIC_EISADC_INIT_I_OFFSET_DATA_H_ADDR(base) ((base) + (0xA016UL))
#define PMIC_EISADC_INIT_I_DATA_L_ADDR(base) ((base) + (0xA017UL))
#define PMIC_EISADC_INIT_I_DATA_H_ADDR(base) ((base) + (0xA018UL))
#define PMIC_EISADC_T_DATA_L_ADDR(base) ((base) + (0xA019UL))
#define PMIC_EISADC_T_DATA_H_ADDR(base) ((base) + (0xA01AUL))
#define PMIC_EISADC_INIT_V_ABS_DATA_L_ADDR(base) ((base) + (0xA01BUL))
#define PMIC_EISADC_INIT_V_ABS_DATA_H_ADDR(base) ((base) + (0xA01CUL))
#define PMIC_EISADC_INIT_V_RELA_DATA_L_ADDR(base) ((base) + (0xA01DUL))
#define PMIC_EISADC_INIT_V_RELA_DATA_H_ADDR(base) ((base) + (0xA01EUL))
#define PMIC_EISADC_INIT_VBAT0_DATA_L_ADDR(base) ((base) + (0xA01FUL))
#define PMIC_EISADC_INIT_VBAT0_DATA_H_ADDR(base) ((base) + (0xA020UL))
#define PMIC_EIS_DET_TIMER_ADDR(base) ((base) + (0xA021UL))
#define PMIC_EIS_DET_TIMER_N_ADDR(base) ((base) + (0xA022UL))
#define PMIC_EIS_DET_TIMER_M_L_ADDR(base) ((base) + (0xA023UL))
#define PMIC_EIS_DET_TIMER_M_H_ADDR(base) ((base) + (0xA024UL))
#define PMIC_EIS_IBAT_DET_CTRL_ADDR(base) ((base) + (0xA025UL))
#define PMIC_EIS_ADC_AVERAGE_ADDR(base) ((base) + (0xA026UL))
#define PMIC_EIS_VBAT0_OFFSET_L_ADDR(base) ((base) + (0xA027UL))
#define PMIC_EIS_VBAT0_OFFSET_H_ADDR(base) ((base) + (0xA028UL))
#define PMIC_EIS_VBAT0_OFFSET_SEL_ADDR(base) ((base) + (0xA029UL))
#define PMIC_EIS_HTHRESHOLD_ERROR_ADDR(base) ((base) + (0xA02AUL))
#define PMIC_EIS_ICOMP_HTHRESHOLD_L_ADDR(base) ((base) + (0xA02BUL))
#define PMIC_EIS_ICOMP_HTHRESHOLD_H_ADDR(base) ((base) + (0xA02CUL))
#define PMIC_EIS_LTHRESHOLD_ERROR_ADDR(base) ((base) + (0xA02DUL))
#define PMIC_EIS_ICOMP_LTHRESHOLD_L_ADDR(base) ((base) + (0xA02EUL))
#define PMIC_EIS_ICOMP_LTHRESHOLD_H_ADDR(base) ((base) + (0xA02FUL))
#define PMIC_EIS_ICOMP_STATE_CLR_ADDR(base) ((base) + (0xA030UL))
#define PMIC_EIS_I_CURRENT_L_ADDR(base) ((base) + (0xA031UL))
#define PMIC_EIS_I_CURRENT_H_ADDR(base) ((base) + (0xA032UL))
#define PMIC_EIS_VBAT_D1_L_ADDR(base) ((base) + (0xA033UL))
#define PMIC_EIS_VBAT_D1_H_ADDR(base) ((base) + (0xA034UL))
#define PMIC_EIS_VBAT_D2_L_ADDR(base) ((base) + (0xA035UL))
#define PMIC_EIS_VBAT_D2_H_ADDR(base) ((base) + (0xA036UL))
#define PMIC_EIS_VBAT_D3_L_ADDR(base) ((base) + (0xA037UL))
#define PMIC_EIS_VBAT_D3_H_ADDR(base) ((base) + (0xA038UL))
#define PMIC_EIS_VBAT_D4_L_ADDR(base) ((base) + (0xA039UL))
#define PMIC_EIS_VBAT_D4_H_ADDR(base) ((base) + (0xA03AUL))
#define PMIC_EIS_VBAT_D5_L_ADDR(base) ((base) + (0xA03BUL))
#define PMIC_EIS_VBAT_D5_H_ADDR(base) ((base) + (0xA03CUL))
#define PMIC_EIS_VBAT_D6_L_ADDR(base) ((base) + (0xA03DUL))
#define PMIC_EIS_VBAT_D6_H_ADDR(base) ((base) + (0xA03EUL))
#define PMIC_EIS_VBAT_D7_L_ADDR(base) ((base) + (0xA03FUL))
#define PMIC_EIS_VBAT_D7_H_ADDR(base) ((base) + (0xA040UL))
#define PMIC_EIS_VBAT_D8_L_ADDR(base) ((base) + (0xA041UL))
#define PMIC_EIS_VBAT_D8_H_ADDR(base) ((base) + (0xA042UL))
#define PMIC_EIS_VBAT_D9_L_ADDR(base) ((base) + (0xA043UL))
#define PMIC_EIS_VBAT_D9_H_ADDR(base) ((base) + (0xA044UL))
#define PMIC_EIS_VBAT_D10_L_ADDR(base) ((base) + (0xA045UL))
#define PMIC_EIS_VBAT_D10_H_ADDR(base) ((base) + (0xA046UL))
#define PMIC_EIS_VBAT_D11_L_ADDR(base) ((base) + (0xA047UL))
#define PMIC_EIS_VBAT_D11_H_ADDR(base) ((base) + (0xA048UL))
#define PMIC_EIS_VBAT_D12_L_ADDR(base) ((base) + (0xA049UL))
#define PMIC_EIS_VBAT_D12_H_ADDR(base) ((base) + (0xA04AUL))
#define PMIC_EIS_VBAT_D13_L_ADDR(base) ((base) + (0xA04BUL))
#define PMIC_EIS_VBAT_D13_H_ADDR(base) ((base) + (0xA04CUL))
#define PMIC_EIS_VBAT_D14_L_ADDR(base) ((base) + (0xA04DUL))
#define PMIC_EIS_VBAT_D14_H_ADDR(base) ((base) + (0xA04EUL))
#define PMIC_EIS_VBAT_D15_L_ADDR(base) ((base) + (0xA04FUL))
#define PMIC_EIS_VBAT_D15_H_ADDR(base) ((base) + (0xA050UL))
#define PMIC_EIS_VBAT_D16_L_ADDR(base) ((base) + (0xA051UL))
#define PMIC_EIS_VBAT_D16_H_ADDR(base) ((base) + (0xA052UL))
#define PMIC_EIS_IBAT_P_D4_L_ADDR(base) ((base) + (0xA053UL))
#define PMIC_EIS_IBAT_P_D4_H_ADDR(base) ((base) + (0xA054UL))
#define PMIC_EIS_IBAT_P_D8_L_ADDR(base) ((base) + (0xA055UL))
#define PMIC_EIS_IBAT_P_D8_H_ADDR(base) ((base) + (0xA056UL))
#define PMIC_EIS_IBAT_P_D12_L_ADDR(base) ((base) + (0xA057UL))
#define PMIC_EIS_IBAT_P_D12_H_ADDR(base) ((base) + (0xA058UL))
#define PMIC_EIS_IBAT_P_D16_L_ADDR(base) ((base) + (0xA059UL))
#define PMIC_EIS_IBAT_P_D16_H_ADDR(base) ((base) + (0xA05AUL))
#define PMIC_EIS_DET_T_NUM_L_ADDR(base) ((base) + (0xA05BUL))
#define PMIC_EIS_DET_T_NUM_H_ADDR(base) ((base) + (0xA05CUL))
#define PMIC_EIS_VBAT_DET_NUM_ADDR(base) ((base) + (0xA05DUL))
#define PMIC_EIS_DET_FLAG_ADDR(base) ((base) + (0xA05EUL))
#define PMIC_EIS_INIT_END_FLAG_ADDR(base) ((base) + (0xA05FUL))
#define PMIC_EISADC_RESERVE_ADDR(base) ((base) + (0xA070UL))
#define PMIC_EISADC_CONV_STATUS_ADDR(base) ((base) + (0xA071UL))
#define PMIC_EISADC_DATA_L_ADDR(base) ((base) + (0xA072UL))
#define PMIC_EISADC_DATA_H_ADDR(base) ((base) + (0xA073UL))
#define PMIC_EISADC_CONV_ADDR(base) ((base) + (0xA074UL))
#define PMIC_EIS_ADC_CURRENT_ADDR(base) ((base) + (0xA075UL))
#define PMIC_EIS_SOFT_RST_ADDR(base) ((base) + (0xA076UL))
#define PMIC_EIS_CLK_GT_CTRL_ADDR(base) ((base) + (0xA077UL))
#define PMIC_EIS_DEBUG_EN_ADDR(base) ((base) + (0xA080UL))
#define PMIC_DEBUG_EIS_EN_ADDR(base) ((base) + (0xA081UL))
#define PMIC_DEBUG_EIS_DISCHG_EN_ADDR(base) ((base) + (0xA082UL))
#define PMIC_DEBUG_EIS_CHANL_ADDR(base) ((base) + (0xA083UL))
#define PMIC_DEBUG_EISADC_CTRL_ADDR(base) ((base) + (0xA084UL))
#define PMIC_DEBUG_EISADC_START_ADDR(base) ((base) + (0xA085UL))
#define PMIC_EISADC_MODE_CFG_ADDR(base) ((base) + (0xA086UL))
#define PMIC_SPMI_TEST0_ADDR(base) ((base) + (0xD0ACUL))
#define PMIC_SPMI_TEST1_ADDR(base) ((base) + (0x2F53UL))
#else
#define PMIC_EIS_ANA_CTRL0_ADDR(base) ((base) + (0xA000))
#define PMIC_EIS_ANA_CTRL1_ADDR(base) ((base) + (0xA001))
#define PMIC_EIS_ANA_CTRL2_ADDR(base) ((base) + (0xA002))
#define PMIC_EIS_IBIAS_SET_0_ADDR(base) ((base) + (0xA003))
#define PMIC_EIS_IBIAS_SET_1_ADDR(base) ((base) + (0xA004))
#define PMIC_EIS_TEST_CTRL_ADDR(base) ((base) + (0xA005))
#define PMIC_EIS_RESERVE0_ADDR(base) ((base) + (0xA006))
#define PMIC_EIS_RESERVE1_ADDR(base) ((base) + (0xA007))
#define PMIC_EIS_RESERVE2_ADDR(base) ((base) + (0xA008))
#define PMIC_EIS_CTRL0_ADDR(base) ((base) + (0xA010))
#define PMIC_EIS_CTRL1_ADDR(base) ((base) + (0xA011))
#define PMIC_EIS_CTRL2_ADDR(base) ((base) + (0xA012))
#define PMIC_EISADC_INIT_V_OFFSET_DATA_L_ADDR(base) ((base) + (0xA013))
#define PMIC_EISADC_INIT_V_OFFSET_DATA_H_ADDR(base) ((base) + (0xA014))
#define PMIC_EISADC_INIT_I_OFFSET_DATA_L_ADDR(base) ((base) + (0xA015))
#define PMIC_EISADC_INIT_I_OFFSET_DATA_H_ADDR(base) ((base) + (0xA016))
#define PMIC_EISADC_INIT_I_DATA_L_ADDR(base) ((base) + (0xA017))
#define PMIC_EISADC_INIT_I_DATA_H_ADDR(base) ((base) + (0xA018))
#define PMIC_EISADC_T_DATA_L_ADDR(base) ((base) + (0xA019))
#define PMIC_EISADC_T_DATA_H_ADDR(base) ((base) + (0xA01A))
#define PMIC_EISADC_INIT_V_ABS_DATA_L_ADDR(base) ((base) + (0xA01B))
#define PMIC_EISADC_INIT_V_ABS_DATA_H_ADDR(base) ((base) + (0xA01C))
#define PMIC_EISADC_INIT_V_RELA_DATA_L_ADDR(base) ((base) + (0xA01D))
#define PMIC_EISADC_INIT_V_RELA_DATA_H_ADDR(base) ((base) + (0xA01E))
#define PMIC_EISADC_INIT_VBAT0_DATA_L_ADDR(base) ((base) + (0xA01F))
#define PMIC_EISADC_INIT_VBAT0_DATA_H_ADDR(base) ((base) + (0xA020))
#define PMIC_EIS_DET_TIMER_ADDR(base) ((base) + (0xA021))
#define PMIC_EIS_DET_TIMER_N_ADDR(base) ((base) + (0xA022))
#define PMIC_EIS_DET_TIMER_M_L_ADDR(base) ((base) + (0xA023))
#define PMIC_EIS_DET_TIMER_M_H_ADDR(base) ((base) + (0xA024))
#define PMIC_EIS_IBAT_DET_CTRL_ADDR(base) ((base) + (0xA025))
#define PMIC_EIS_ADC_AVERAGE_ADDR(base) ((base) + (0xA026))
#define PMIC_EIS_VBAT0_OFFSET_L_ADDR(base) ((base) + (0xA027))
#define PMIC_EIS_VBAT0_OFFSET_H_ADDR(base) ((base) + (0xA028))
#define PMIC_EIS_VBAT0_OFFSET_SEL_ADDR(base) ((base) + (0xA029))
#define PMIC_EIS_HTHRESHOLD_ERROR_ADDR(base) ((base) + (0xA02A))
#define PMIC_EIS_ICOMP_HTHRESHOLD_L_ADDR(base) ((base) + (0xA02B))
#define PMIC_EIS_ICOMP_HTHRESHOLD_H_ADDR(base) ((base) + (0xA02C))
#define PMIC_EIS_LTHRESHOLD_ERROR_ADDR(base) ((base) + (0xA02D))
#define PMIC_EIS_ICOMP_LTHRESHOLD_L_ADDR(base) ((base) + (0xA02E))
#define PMIC_EIS_ICOMP_LTHRESHOLD_H_ADDR(base) ((base) + (0xA02F))
#define PMIC_EIS_ICOMP_STATE_CLR_ADDR(base) ((base) + (0xA030))
#define PMIC_EIS_I_CURRENT_L_ADDR(base) ((base) + (0xA031))
#define PMIC_EIS_I_CURRENT_H_ADDR(base) ((base) + (0xA032))
#define PMIC_EIS_VBAT_D1_L_ADDR(base) ((base) + (0xA033))
#define PMIC_EIS_VBAT_D1_H_ADDR(base) ((base) + (0xA034))
#define PMIC_EIS_VBAT_D2_L_ADDR(base) ((base) + (0xA035))
#define PMIC_EIS_VBAT_D2_H_ADDR(base) ((base) + (0xA036))
#define PMIC_EIS_VBAT_D3_L_ADDR(base) ((base) + (0xA037))
#define PMIC_EIS_VBAT_D3_H_ADDR(base) ((base) + (0xA038))
#define PMIC_EIS_VBAT_D4_L_ADDR(base) ((base) + (0xA039))
#define PMIC_EIS_VBAT_D4_H_ADDR(base) ((base) + (0xA03A))
#define PMIC_EIS_VBAT_D5_L_ADDR(base) ((base) + (0xA03B))
#define PMIC_EIS_VBAT_D5_H_ADDR(base) ((base) + (0xA03C))
#define PMIC_EIS_VBAT_D6_L_ADDR(base) ((base) + (0xA03D))
#define PMIC_EIS_VBAT_D6_H_ADDR(base) ((base) + (0xA03E))
#define PMIC_EIS_VBAT_D7_L_ADDR(base) ((base) + (0xA03F))
#define PMIC_EIS_VBAT_D7_H_ADDR(base) ((base) + (0xA040))
#define PMIC_EIS_VBAT_D8_L_ADDR(base) ((base) + (0xA041))
#define PMIC_EIS_VBAT_D8_H_ADDR(base) ((base) + (0xA042))
#define PMIC_EIS_VBAT_D9_L_ADDR(base) ((base) + (0xA043))
#define PMIC_EIS_VBAT_D9_H_ADDR(base) ((base) + (0xA044))
#define PMIC_EIS_VBAT_D10_L_ADDR(base) ((base) + (0xA045))
#define PMIC_EIS_VBAT_D10_H_ADDR(base) ((base) + (0xA046))
#define PMIC_EIS_VBAT_D11_L_ADDR(base) ((base) + (0xA047))
#define PMIC_EIS_VBAT_D11_H_ADDR(base) ((base) + (0xA048))
#define PMIC_EIS_VBAT_D12_L_ADDR(base) ((base) + (0xA049))
#define PMIC_EIS_VBAT_D12_H_ADDR(base) ((base) + (0xA04A))
#define PMIC_EIS_VBAT_D13_L_ADDR(base) ((base) + (0xA04B))
#define PMIC_EIS_VBAT_D13_H_ADDR(base) ((base) + (0xA04C))
#define PMIC_EIS_VBAT_D14_L_ADDR(base) ((base) + (0xA04D))
#define PMIC_EIS_VBAT_D14_H_ADDR(base) ((base) + (0xA04E))
#define PMIC_EIS_VBAT_D15_L_ADDR(base) ((base) + (0xA04F))
#define PMIC_EIS_VBAT_D15_H_ADDR(base) ((base) + (0xA050))
#define PMIC_EIS_VBAT_D16_L_ADDR(base) ((base) + (0xA051))
#define PMIC_EIS_VBAT_D16_H_ADDR(base) ((base) + (0xA052))
#define PMIC_EIS_IBAT_P_D4_L_ADDR(base) ((base) + (0xA053))
#define PMIC_EIS_IBAT_P_D4_H_ADDR(base) ((base) + (0xA054))
#define PMIC_EIS_IBAT_P_D8_L_ADDR(base) ((base) + (0xA055))
#define PMIC_EIS_IBAT_P_D8_H_ADDR(base) ((base) + (0xA056))
#define PMIC_EIS_IBAT_P_D12_L_ADDR(base) ((base) + (0xA057))
#define PMIC_EIS_IBAT_P_D12_H_ADDR(base) ((base) + (0xA058))
#define PMIC_EIS_IBAT_P_D16_L_ADDR(base) ((base) + (0xA059))
#define PMIC_EIS_IBAT_P_D16_H_ADDR(base) ((base) + (0xA05A))
#define PMIC_EIS_DET_T_NUM_L_ADDR(base) ((base) + (0xA05B))
#define PMIC_EIS_DET_T_NUM_H_ADDR(base) ((base) + (0xA05C))
#define PMIC_EIS_VBAT_DET_NUM_ADDR(base) ((base) + (0xA05D))
#define PMIC_EIS_DET_FLAG_ADDR(base) ((base) + (0xA05E))
#define PMIC_EIS_INIT_END_FLAG_ADDR(base) ((base) + (0xA05F))
#define PMIC_EISADC_RESERVE_ADDR(base) ((base) + (0xA070))
#define PMIC_EISADC_CONV_STATUS_ADDR(base) ((base) + (0xA071))
#define PMIC_EISADC_DATA_L_ADDR(base) ((base) + (0xA072))
#define PMIC_EISADC_DATA_H_ADDR(base) ((base) + (0xA073))
#define PMIC_EISADC_CONV_ADDR(base) ((base) + (0xA074))
#define PMIC_EIS_ADC_CURRENT_ADDR(base) ((base) + (0xA075))
#define PMIC_EIS_SOFT_RST_ADDR(base) ((base) + (0xA076))
#define PMIC_EIS_CLK_GT_CTRL_ADDR(base) ((base) + (0xA077))
#define PMIC_EIS_DEBUG_EN_ADDR(base) ((base) + (0xA080))
#define PMIC_DEBUG_EIS_EN_ADDR(base) ((base) + (0xA081))
#define PMIC_DEBUG_EIS_DISCHG_EN_ADDR(base) ((base) + (0xA082))
#define PMIC_DEBUG_EIS_CHANL_ADDR(base) ((base) + (0xA083))
#define PMIC_DEBUG_EISADC_CTRL_ADDR(base) ((base) + (0xA084))
#define PMIC_DEBUG_EISADC_START_ADDR(base) ((base) + (0xA085))
#define PMIC_EISADC_MODE_CFG_ADDR(base) ((base) + (0xA086))
#define PMIC_SPMI_TEST0_ADDR(base) ((base) + (0xD0AC))
#define PMIC_SPMI_TEST1_ADDR(base) ((base) + (0x2F53))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char project_num0 : 8;
    } reg;
} PMIC_VERSION0_UNION;
#endif
#define PMIC_VERSION0_project_num0_START (0)
#define PMIC_VERSION0_project_num0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char project_num1 : 8;
    } reg;
} PMIC_VERSION1_UNION;
#endif
#define PMIC_VERSION1_project_num1_START (0)
#define PMIC_VERSION1_project_num1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char project_num2 : 8;
    } reg;
} PMIC_VERSION2_UNION;
#endif
#define PMIC_VERSION2_project_num2_START (0)
#define PMIC_VERSION2_project_num2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char project_num3 : 8;
    } reg;
} PMIC_VERSION3_UNION;
#endif
#define PMIC_VERSION3_project_num3_START (0)
#define PMIC_VERSION3_project_num3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char version : 8;
    } reg;
} PMIC_VERSION4_UNION;
#endif
#define PMIC_VERSION4_version_START (0)
#define PMIC_VERSION4_version_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char chip_id : 8;
    } reg;
} PMIC_VERSION5_UNION;
#endif
#define PMIC_VERSION5_chip_id_START (0)
#define PMIC_VERSION5_chip_id_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char st_vsys_ov_d200ur : 1;
        unsigned char st_vbus_det_insert_d20m : 1;
        unsigned char st_vsys_pwroff_abs_d20nr : 1;
        unsigned char st_vsys_pwroff_deb_d80mr : 1;
        unsigned char st_vsys_pwron_d60ur : 1;
        unsigned char st_thsd_otmp140_d1mr : 1;
        unsigned char st_thsd_otmp125_d1mr : 1;
        unsigned char st_pwron_d20m : 1;
    } reg;
} PMIC_STATUS0_UNION;
#endif
#define PMIC_STATUS0_st_vsys_ov_d200ur_START (0)
#define PMIC_STATUS0_st_vsys_ov_d200ur_END (0)
#define PMIC_STATUS0_st_vbus_det_insert_d20m_START (1)
#define PMIC_STATUS0_st_vbus_det_insert_d20m_END (1)
#define PMIC_STATUS0_st_vsys_pwroff_abs_d20nr_START (2)
#define PMIC_STATUS0_st_vsys_pwroff_abs_d20nr_END (2)
#define PMIC_STATUS0_st_vsys_pwroff_deb_d80mr_START (3)
#define PMIC_STATUS0_st_vsys_pwroff_deb_d80mr_END (3)
#define PMIC_STATUS0_st_vsys_pwron_d60ur_START (4)
#define PMIC_STATUS0_st_vsys_pwron_d60ur_END (4)
#define PMIC_STATUS0_st_thsd_otmp140_d1mr_START (5)
#define PMIC_STATUS0_st_thsd_otmp140_d1mr_END (5)
#define PMIC_STATUS0_st_thsd_otmp125_d1mr_START (6)
#define PMIC_STATUS0_st_thsd_otmp125_d1mr_END (6)
#define PMIC_STATUS0_st_pwron_d20m_START (7)
#define PMIC_STATUS0_st_pwron_d20m_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char st_dcxo_clk_sel : 1;
        unsigned char st_sim0_hpd_d540u : 1;
        unsigned char st_sim1_hpd_d540u : 1;
        unsigned char st_avdd_osc_vld_d20nf : 1;
        unsigned char st_nfc_pwr_en_d5m : 1;
        unsigned char st_ufs_sel_d20nr : 1;
        unsigned char st_freq_sel_d20nr : 1;
        unsigned char st_lpddr_sel_d20nr : 1;
    } reg;
} PMIC_STATUS1_UNION;
#endif
#define PMIC_STATUS1_st_dcxo_clk_sel_START (0)
#define PMIC_STATUS1_st_dcxo_clk_sel_END (0)
#define PMIC_STATUS1_st_sim0_hpd_d540u_START (1)
#define PMIC_STATUS1_st_sim0_hpd_d540u_END (1)
#define PMIC_STATUS1_st_sim1_hpd_d540u_START (2)
#define PMIC_STATUS1_st_sim1_hpd_d540u_END (2)
#define PMIC_STATUS1_st_avdd_osc_vld_d20nf_START (3)
#define PMIC_STATUS1_st_avdd_osc_vld_d20nf_END (3)
#define PMIC_STATUS1_st_nfc_pwr_en_d5m_START (4)
#define PMIC_STATUS1_st_nfc_pwr_en_d5m_END (4)
#define PMIC_STATUS1_st_ufs_sel_d20nr_START (5)
#define PMIC_STATUS1_st_ufs_sel_d20nr_END (5)
#define PMIC_STATUS1_st_freq_sel_d20nr_START (6)
#define PMIC_STATUS1_st_freq_sel_d20nr_END (6)
#define PMIC_STATUS1_st_lpddr_sel_d20nr_START (7)
#define PMIC_STATUS1_st_lpddr_sel_d20nr_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char st_vsys_ovp_surge_d20nr : 1;
        unsigned char st_voice_restart1_d120u : 1;
        unsigned char st_voice_restart2_d120u : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_STATUS2_UNION;
#endif
#define PMIC_STATUS2_st_vsys_ovp_surge_d20nr_START (0)
#define PMIC_STATUS2_st_vsys_ovp_surge_d20nr_END (0)
#define PMIC_STATUS2_st_voice_restart1_d120u_START (1)
#define PMIC_STATUS2_st_voice_restart1_d120u_END (1)
#define PMIC_STATUS2_st_voice_restart2_d120u_START (2)
#define PMIC_STATUS2_st_voice_restart2_d120u_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_ocp_state : 1;
        unsigned char b2_ocp_state : 1;
        unsigned char b3_ocp_state : 1;
        unsigned char b5_ocp_state : 1;
        unsigned char b9_ocp_state : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_STATUS3_UNION;
#endif
#define PMIC_STATUS3_b1_ocp_state_START (0)
#define PMIC_STATUS3_b1_ocp_state_END (0)
#define PMIC_STATUS3_b2_ocp_state_START (1)
#define PMIC_STATUS3_b2_ocp_state_END (1)
#define PMIC_STATUS3_b3_ocp_state_START (2)
#define PMIC_STATUS3_b3_ocp_state_END (2)
#define PMIC_STATUS3_b5_ocp_state_START (3)
#define PMIC_STATUS3_b5_ocp_state_END (3)
#define PMIC_STATUS3_b9_ocp_state_START (4)
#define PMIC_STATUS3_b9_ocp_state_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_ccm_state : 1;
        unsigned char b2_ccm_state : 1;
        unsigned char b3_ccm_state : 1;
        unsigned char b5_ccm_state : 1;
        unsigned char b9_ccm_state : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_STATUS4_UNION;
#endif
#define PMIC_STATUS4_b1_ccm_state_START (0)
#define PMIC_STATUS4_b1_ccm_state_END (0)
#define PMIC_STATUS4_b2_ccm_state_START (1)
#define PMIC_STATUS4_b2_ccm_state_END (1)
#define PMIC_STATUS4_b3_ccm_state_START (2)
#define PMIC_STATUS4_b3_ccm_state_END (2)
#define PMIC_STATUS4_b5_ccm_state_START (3)
#define PMIC_STATUS4_b5_ccm_state_END (3)
#define PMIC_STATUS4_b9_ccm_state_START (4)
#define PMIC_STATUS4_b9_ccm_state_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck401_dcm_ccm_state : 2;
        unsigned char buck42_dcm_ccm_state : 2;
        unsigned char buck70_dcm_ccm_state : 2;
        unsigned char buck71_dcm_ccm_state : 2;
    } reg;
} PMIC_STATUS5_UNION;
#endif
#define PMIC_STATUS5_buck401_dcm_ccm_state_START (0)
#define PMIC_STATUS5_buck401_dcm_ccm_state_END (1)
#define PMIC_STATUS5_buck42_dcm_ccm_state_START (2)
#define PMIC_STATUS5_buck42_dcm_ccm_state_END (3)
#define PMIC_STATUS5_buck70_dcm_ccm_state_START (4)
#define PMIC_STATUS5_buck70_dcm_ccm_state_END (5)
#define PMIC_STATUS5_buck71_dcm_ccm_state_START (6)
#define PMIC_STATUS5_buck71_dcm_ccm_state_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck401_pg : 1;
        unsigned char buck42_pg : 1;
        unsigned char buck70_pg : 1;
        unsigned char buck71_pg : 1;
        unsigned char buck401_cur_det_deb : 1;
        unsigned char buck42_cur_det_deb : 1;
        unsigned char buck70_cur_det_deb : 1;
        unsigned char buck71_cur_det_deb : 1;
    } reg;
} PMIC_STATUS6_UNION;
#endif
#define PMIC_STATUS6_buck401_pg_START (0)
#define PMIC_STATUS6_buck401_pg_END (0)
#define PMIC_STATUS6_buck42_pg_START (1)
#define PMIC_STATUS6_buck42_pg_END (1)
#define PMIC_STATUS6_buck70_pg_START (2)
#define PMIC_STATUS6_buck70_pg_END (2)
#define PMIC_STATUS6_buck71_pg_START (3)
#define PMIC_STATUS6_buck71_pg_END (3)
#define PMIC_STATUS6_buck401_cur_det_deb_START (4)
#define PMIC_STATUS6_buck401_cur_det_deb_END (4)
#define PMIC_STATUS6_buck42_cur_det_deb_START (5)
#define PMIC_STATUS6_buck42_cur_det_deb_END (5)
#define PMIC_STATUS6_buck70_cur_det_deb_START (6)
#define PMIC_STATUS6_buck70_cur_det_deb_END (6)
#define PMIC_STATUS6_buck71_cur_det_deb_START (7)
#define PMIC_STATUS6_buck71_cur_det_deb_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck1_en : 1;
        unsigned char st_buck1_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck1_eco_en : 1;
        unsigned char st_buck1_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_BUCK1_ONOFF_ECO_UNION;
#endif
#define PMIC_BUCK1_ONOFF_ECO_reg_buck1_en_START (0)
#define PMIC_BUCK1_ONOFF_ECO_reg_buck1_en_END (0)
#define PMIC_BUCK1_ONOFF_ECO_st_buck1_en_START (1)
#define PMIC_BUCK1_ONOFF_ECO_st_buck1_en_END (1)
#define PMIC_BUCK1_ONOFF_ECO_reg_buck1_eco_en_START (4)
#define PMIC_BUCK1_ONOFF_ECO_reg_buck1_eco_en_END (4)
#define PMIC_BUCK1_ONOFF_ECO_st_buck1_eco_en_START (5)
#define PMIC_BUCK1_ONOFF_ECO_st_buck1_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck2_en : 1;
        unsigned char st_buck2_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck2_eco_en : 1;
        unsigned char st_buck2_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_BUCK2_ONOFF_ECO_UNION;
#endif
#define PMIC_BUCK2_ONOFF_ECO_reg_buck2_en_START (0)
#define PMIC_BUCK2_ONOFF_ECO_reg_buck2_en_END (0)
#define PMIC_BUCK2_ONOFF_ECO_st_buck2_en_START (1)
#define PMIC_BUCK2_ONOFF_ECO_st_buck2_en_END (1)
#define PMIC_BUCK2_ONOFF_ECO_reg_buck2_eco_en_START (4)
#define PMIC_BUCK2_ONOFF_ECO_reg_buck2_eco_en_END (4)
#define PMIC_BUCK2_ONOFF_ECO_st_buck2_eco_en_START (5)
#define PMIC_BUCK2_ONOFF_ECO_st_buck2_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck3_en : 1;
        unsigned char st_buck3_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck3_eco_en : 1;
        unsigned char st_buck3_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_BUCK3_ONOFF_ECO_UNION;
#endif
#define PMIC_BUCK3_ONOFF_ECO_reg_buck3_en_START (0)
#define PMIC_BUCK3_ONOFF_ECO_reg_buck3_en_END (0)
#define PMIC_BUCK3_ONOFF_ECO_st_buck3_en_START (1)
#define PMIC_BUCK3_ONOFF_ECO_st_buck3_en_END (1)
#define PMIC_BUCK3_ONOFF_ECO_reg_buck3_eco_en_START (4)
#define PMIC_BUCK3_ONOFF_ECO_reg_buck3_eco_en_END (4)
#define PMIC_BUCK3_ONOFF_ECO_st_buck3_eco_en_START (5)
#define PMIC_BUCK3_ONOFF_ECO_st_buck3_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck40_en : 1;
        unsigned char st_buck40_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck40_eco_en : 1;
        unsigned char st_buck40_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_BUCK40_ONOFF_ECO_UNION;
#endif
#define PMIC_BUCK40_ONOFF_ECO_reg_buck40_en_START (0)
#define PMIC_BUCK40_ONOFF_ECO_reg_buck40_en_END (0)
#define PMIC_BUCK40_ONOFF_ECO_st_buck40_en_START (1)
#define PMIC_BUCK40_ONOFF_ECO_st_buck40_en_END (1)
#define PMIC_BUCK40_ONOFF_ECO_reg_buck40_eco_en_START (4)
#define PMIC_BUCK40_ONOFF_ECO_reg_buck40_eco_en_END (4)
#define PMIC_BUCK40_ONOFF_ECO_st_buck40_eco_en_START (5)
#define PMIC_BUCK40_ONOFF_ECO_st_buck40_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_BUCK41_ONOFF_ECO_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck42_en : 1;
        unsigned char st_buck42_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck42_eco_en : 1;
        unsigned char st_buck42_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_BUCK42_ONOFF_ECO_UNION;
#endif
#define PMIC_BUCK42_ONOFF_ECO_reg_buck42_en_START (0)
#define PMIC_BUCK42_ONOFF_ECO_reg_buck42_en_END (0)
#define PMIC_BUCK42_ONOFF_ECO_st_buck42_en_START (1)
#define PMIC_BUCK42_ONOFF_ECO_st_buck42_en_END (1)
#define PMIC_BUCK42_ONOFF_ECO_reg_buck42_eco_en_START (4)
#define PMIC_BUCK42_ONOFF_ECO_reg_buck42_eco_en_END (4)
#define PMIC_BUCK42_ONOFF_ECO_st_buck42_eco_en_START (5)
#define PMIC_BUCK42_ONOFF_ECO_st_buck42_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck5_en : 1;
        unsigned char st_buck5_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck5_eco_en : 1;
        unsigned char st_buck5_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_BUCK5_ONOFF_ECO_UNION;
#endif
#define PMIC_BUCK5_ONOFF_ECO_reg_buck5_en_START (0)
#define PMIC_BUCK5_ONOFF_ECO_reg_buck5_en_END (0)
#define PMIC_BUCK5_ONOFF_ECO_st_buck5_en_START (1)
#define PMIC_BUCK5_ONOFF_ECO_st_buck5_en_END (1)
#define PMIC_BUCK5_ONOFF_ECO_reg_buck5_eco_en_START (4)
#define PMIC_BUCK5_ONOFF_ECO_reg_buck5_eco_en_END (4)
#define PMIC_BUCK5_ONOFF_ECO_st_buck5_eco_en_START (5)
#define PMIC_BUCK5_ONOFF_ECO_st_buck5_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck70_en_a : 1;
        unsigned char st_buck70_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck70_eco_en : 1;
        unsigned char st_buck70_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_BUCK70_ONOFF1_ECO_UNION;
#endif
#define PMIC_BUCK70_ONOFF1_ECO_reg_buck70_en_a_START (0)
#define PMIC_BUCK70_ONOFF1_ECO_reg_buck70_en_a_END (0)
#define PMIC_BUCK70_ONOFF1_ECO_st_buck70_en_START (1)
#define PMIC_BUCK70_ONOFF1_ECO_st_buck70_en_END (1)
#define PMIC_BUCK70_ONOFF1_ECO_reg_buck70_eco_en_START (4)
#define PMIC_BUCK70_ONOFF1_ECO_reg_buck70_eco_en_END (4)
#define PMIC_BUCK70_ONOFF1_ECO_st_buck70_eco_en_START (5)
#define PMIC_BUCK70_ONOFF1_ECO_st_buck70_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck70_en_b : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_BUCK70_ONOFF2_UNION;
#endif
#define PMIC_BUCK70_ONOFF2_reg_buck70_en_b_START (0)
#define PMIC_BUCK70_ONOFF2_reg_buck70_en_b_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck70_en_c : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_BUCK70_ONOFF3_UNION;
#endif
#define PMIC_BUCK70_ONOFF3_reg_buck70_en_c_START (0)
#define PMIC_BUCK70_ONOFF3_reg_buck70_en_c_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck71_en : 1;
        unsigned char st_buck71_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck71_eco_en : 1;
        unsigned char st_buck71_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_BUCK71_ONOFF_ECO_UNION;
#endif
#define PMIC_BUCK71_ONOFF_ECO_reg_buck71_en_START (0)
#define PMIC_BUCK71_ONOFF_ECO_reg_buck71_en_END (0)
#define PMIC_BUCK71_ONOFF_ECO_st_buck71_en_START (1)
#define PMIC_BUCK71_ONOFF_ECO_st_buck71_en_END (1)
#define PMIC_BUCK71_ONOFF_ECO_reg_buck71_eco_en_START (4)
#define PMIC_BUCK71_ONOFF_ECO_reg_buck71_eco_en_END (4)
#define PMIC_BUCK71_ONOFF_ECO_st_buck71_eco_en_START (5)
#define PMIC_BUCK71_ONOFF_ECO_st_buck71_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_buck9_en : 1;
        unsigned char st_buck9_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_buck9_eco_en : 1;
        unsigned char st_buck9_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_BUCK9_ONOFF_ECO_UNION;
#endif
#define PMIC_BUCK9_ONOFF_ECO_reg_buck9_en_START (0)
#define PMIC_BUCK9_ONOFF_ECO_reg_buck9_en_END (0)
#define PMIC_BUCK9_ONOFF_ECO_st_buck9_en_START (1)
#define PMIC_BUCK9_ONOFF_ECO_st_buck9_en_END (1)
#define PMIC_BUCK9_ONOFF_ECO_reg_buck9_eco_en_START (4)
#define PMIC_BUCK9_ONOFF_ECO_reg_buck9_eco_en_END (4)
#define PMIC_BUCK9_ONOFF_ECO_st_buck9_eco_en_START (5)
#define PMIC_BUCK9_ONOFF_ECO_st_buck9_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo0_en : 1;
        unsigned char st_ldo0_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo0_eco_en : 1;
        unsigned char st_ldo0_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO0_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO0_ONOFF_ECO_reg_ldo0_en_START (0)
#define PMIC_LDO0_ONOFF_ECO_reg_ldo0_en_END (0)
#define PMIC_LDO0_ONOFF_ECO_st_ldo0_en_START (1)
#define PMIC_LDO0_ONOFF_ECO_st_ldo0_en_END (1)
#define PMIC_LDO0_ONOFF_ECO_reg_ldo0_eco_en_START (4)
#define PMIC_LDO0_ONOFF_ECO_reg_ldo0_eco_en_END (4)
#define PMIC_LDO0_ONOFF_ECO_st_ldo0_eco_en_START (5)
#define PMIC_LDO0_ONOFF_ECO_st_ldo0_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo1_en : 1;
        unsigned char st_ldo1_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo1_eco_en : 1;
        unsigned char st_ldo1_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO1_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO1_ONOFF_ECO_reg_ldo1_en_START (0)
#define PMIC_LDO1_ONOFF_ECO_reg_ldo1_en_END (0)
#define PMIC_LDO1_ONOFF_ECO_st_ldo1_en_START (1)
#define PMIC_LDO1_ONOFF_ECO_st_ldo1_en_END (1)
#define PMIC_LDO1_ONOFF_ECO_reg_ldo1_eco_en_START (4)
#define PMIC_LDO1_ONOFF_ECO_reg_ldo1_eco_en_END (4)
#define PMIC_LDO1_ONOFF_ECO_st_ldo1_eco_en_START (5)
#define PMIC_LDO1_ONOFF_ECO_st_ldo1_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo3_en : 1;
        unsigned char st_ldo3_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo3_eco_en : 1;
        unsigned char st_ldo3_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO3_ONOFF_UNION;
#endif
#define PMIC_LDO3_ONOFF_reg_ldo3_en_START (0)
#define PMIC_LDO3_ONOFF_reg_ldo3_en_END (0)
#define PMIC_LDO3_ONOFF_st_ldo3_en_START (1)
#define PMIC_LDO3_ONOFF_st_ldo3_en_END (1)
#define PMIC_LDO3_ONOFF_reg_ldo3_eco_en_START (4)
#define PMIC_LDO3_ONOFF_reg_ldo3_eco_en_END (4)
#define PMIC_LDO3_ONOFF_st_ldo3_eco_en_START (5)
#define PMIC_LDO3_ONOFF_st_ldo3_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo4_en : 1;
        unsigned char st_ldo4_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo4_eco_en : 1;
        unsigned char st_ldo4_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO4_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO4_ONOFF_ECO_reg_ldo4_en_START (0)
#define PMIC_LDO4_ONOFF_ECO_reg_ldo4_en_END (0)
#define PMIC_LDO4_ONOFF_ECO_st_ldo4_en_START (1)
#define PMIC_LDO4_ONOFF_ECO_st_ldo4_en_END (1)
#define PMIC_LDO4_ONOFF_ECO_reg_ldo4_eco_en_START (4)
#define PMIC_LDO4_ONOFF_ECO_reg_ldo4_eco_en_END (4)
#define PMIC_LDO4_ONOFF_ECO_st_ldo4_eco_en_START (5)
#define PMIC_LDO4_ONOFF_ECO_st_ldo4_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo6_en : 1;
        unsigned char st_ldo6_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo6_eco_en : 1;
        unsigned char st_ldo6_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO6_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO6_ONOFF_ECO_reg_ldo6_en_START (0)
#define PMIC_LDO6_ONOFF_ECO_reg_ldo6_en_END (0)
#define PMIC_LDO6_ONOFF_ECO_st_ldo6_en_START (1)
#define PMIC_LDO6_ONOFF_ECO_st_ldo6_en_END (1)
#define PMIC_LDO6_ONOFF_ECO_reg_ldo6_eco_en_START (4)
#define PMIC_LDO6_ONOFF_ECO_reg_ldo6_eco_en_END (4)
#define PMIC_LDO6_ONOFF_ECO_st_ldo6_eco_en_START (5)
#define PMIC_LDO6_ONOFF_ECO_st_ldo6_eco_en_END (5)
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
} PMIC_LDO8_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO8_ONOFF_ECO_reg_ldo8_en_START (0)
#define PMIC_LDO8_ONOFF_ECO_reg_ldo8_en_END (0)
#define PMIC_LDO8_ONOFF_ECO_st_ldo8_en_START (1)
#define PMIC_LDO8_ONOFF_ECO_st_ldo8_en_END (1)
#define PMIC_LDO8_ONOFF_ECO_reg_ldo8_eco_en_START (4)
#define PMIC_LDO8_ONOFF_ECO_reg_ldo8_eco_en_END (4)
#define PMIC_LDO8_ONOFF_ECO_st_ldo8_eco_en_START (5)
#define PMIC_LDO8_ONOFF_ECO_st_ldo8_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo11_en : 1;
        unsigned char st_ldo11_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo11_eco_en : 1;
        unsigned char st_ldo11_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO11_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO11_ONOFF_ECO_reg_ldo11_en_START (0)
#define PMIC_LDO11_ONOFF_ECO_reg_ldo11_en_END (0)
#define PMIC_LDO11_ONOFF_ECO_st_ldo11_en_START (1)
#define PMIC_LDO11_ONOFF_ECO_st_ldo11_en_END (1)
#define PMIC_LDO11_ONOFF_ECO_reg_ldo11_eco_en_START (4)
#define PMIC_LDO11_ONOFF_ECO_reg_ldo11_eco_en_END (4)
#define PMIC_LDO11_ONOFF_ECO_st_ldo11_eco_en_START (5)
#define PMIC_LDO11_ONOFF_ECO_st_ldo11_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo12_en : 1;
        unsigned char st_ldo12_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo12_eco_en : 1;
        unsigned char st_ldo12_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO12_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO12_ONOFF_ECO_reg_ldo12_en_START (0)
#define PMIC_LDO12_ONOFF_ECO_reg_ldo12_en_END (0)
#define PMIC_LDO12_ONOFF_ECO_st_ldo12_en_START (1)
#define PMIC_LDO12_ONOFF_ECO_st_ldo12_en_END (1)
#define PMIC_LDO12_ONOFF_ECO_reg_ldo12_eco_en_START (4)
#define PMIC_LDO12_ONOFF_ECO_reg_ldo12_eco_en_END (4)
#define PMIC_LDO12_ONOFF_ECO_st_ldo12_eco_en_START (5)
#define PMIC_LDO12_ONOFF_ECO_st_ldo12_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo14_en : 1;
        unsigned char st_ldo14_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo14_eco_en : 1;
        unsigned char st_ldo14_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO14_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO14_ONOFF_ECO_reg_ldo14_en_START (0)
#define PMIC_LDO14_ONOFF_ECO_reg_ldo14_en_END (0)
#define PMIC_LDO14_ONOFF_ECO_st_ldo14_en_START (1)
#define PMIC_LDO14_ONOFF_ECO_st_ldo14_en_END (1)
#define PMIC_LDO14_ONOFF_ECO_reg_ldo14_eco_en_START (4)
#define PMIC_LDO14_ONOFF_ECO_reg_ldo14_eco_en_END (4)
#define PMIC_LDO14_ONOFF_ECO_st_ldo14_eco_en_START (5)
#define PMIC_LDO14_ONOFF_ECO_st_ldo14_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo15_en : 1;
        unsigned char st_ldo15_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo15_eco_en : 1;
        unsigned char st_ldo15_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO15_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO15_ONOFF_ECO_reg_ldo15_en_START (0)
#define PMIC_LDO15_ONOFF_ECO_reg_ldo15_en_END (0)
#define PMIC_LDO15_ONOFF_ECO_st_ldo15_en_START (1)
#define PMIC_LDO15_ONOFF_ECO_st_ldo15_en_END (1)
#define PMIC_LDO15_ONOFF_ECO_reg_ldo15_eco_en_START (4)
#define PMIC_LDO15_ONOFF_ECO_reg_ldo15_eco_en_END (4)
#define PMIC_LDO15_ONOFF_ECO_st_ldo15_eco_en_START (5)
#define PMIC_LDO15_ONOFF_ECO_st_ldo15_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo16_en : 1;
        unsigned char st_ldo16_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo16_eco_en : 1;
        unsigned char st_ldo16_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO16_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO16_ONOFF_ECO_reg_ldo16_en_START (0)
#define PMIC_LDO16_ONOFF_ECO_reg_ldo16_en_END (0)
#define PMIC_LDO16_ONOFF_ECO_st_ldo16_en_START (1)
#define PMIC_LDO16_ONOFF_ECO_st_ldo16_en_END (1)
#define PMIC_LDO16_ONOFF_ECO_reg_ldo16_eco_en_START (4)
#define PMIC_LDO16_ONOFF_ECO_reg_ldo16_eco_en_END (4)
#define PMIC_LDO16_ONOFF_ECO_st_ldo16_eco_en_START (5)
#define PMIC_LDO16_ONOFF_ECO_st_ldo16_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo17_en : 1;
        unsigned char st_ldo17_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo17_eco_en : 1;
        unsigned char st_ldo17_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO17_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO17_ONOFF_ECO_reg_ldo17_en_START (0)
#define PMIC_LDO17_ONOFF_ECO_reg_ldo17_en_END (0)
#define PMIC_LDO17_ONOFF_ECO_st_ldo17_en_START (1)
#define PMIC_LDO17_ONOFF_ECO_st_ldo17_en_END (1)
#define PMIC_LDO17_ONOFF_ECO_reg_ldo17_eco_en_START (4)
#define PMIC_LDO17_ONOFF_ECO_reg_ldo17_eco_en_END (4)
#define PMIC_LDO17_ONOFF_ECO_st_ldo17_eco_en_START (5)
#define PMIC_LDO17_ONOFF_ECO_st_ldo17_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo18_en : 1;
        unsigned char st_ldo18_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo18_eco_en : 1;
        unsigned char st_ldo18_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO18_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO18_ONOFF_ECO_reg_ldo18_en_START (0)
#define PMIC_LDO18_ONOFF_ECO_reg_ldo18_en_END (0)
#define PMIC_LDO18_ONOFF_ECO_st_ldo18_en_START (1)
#define PMIC_LDO18_ONOFF_ECO_st_ldo18_en_END (1)
#define PMIC_LDO18_ONOFF_ECO_reg_ldo18_eco_en_START (4)
#define PMIC_LDO18_ONOFF_ECO_reg_ldo18_eco_en_END (4)
#define PMIC_LDO18_ONOFF_ECO_st_ldo18_eco_en_START (5)
#define PMIC_LDO18_ONOFF_ECO_st_ldo18_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo21_en : 1;
        unsigned char st_ldo21_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo21_eco_en : 1;
        unsigned char st_ldo21_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO21_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO21_ONOFF_ECO_reg_ldo21_en_START (0)
#define PMIC_LDO21_ONOFF_ECO_reg_ldo21_en_END (0)
#define PMIC_LDO21_ONOFF_ECO_st_ldo21_en_START (1)
#define PMIC_LDO21_ONOFF_ECO_st_ldo21_en_END (1)
#define PMIC_LDO21_ONOFF_ECO_reg_ldo21_eco_en_START (4)
#define PMIC_LDO21_ONOFF_ECO_reg_ldo21_eco_en_END (4)
#define PMIC_LDO21_ONOFF_ECO_st_ldo21_eco_en_START (5)
#define PMIC_LDO21_ONOFF_ECO_st_ldo21_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo22_en : 1;
        unsigned char st_ldo22_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_LDO22_ONOFF_UNION;
#endif
#define PMIC_LDO22_ONOFF_reg_ldo22_en_START (0)
#define PMIC_LDO22_ONOFF_reg_ldo22_en_END (0)
#define PMIC_LDO22_ONOFF_st_ldo22_en_START (1)
#define PMIC_LDO22_ONOFF_st_ldo22_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo23_en : 1;
        unsigned char st_ldo23_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo23_eco_en : 1;
        unsigned char st_ldo23_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO23_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO23_ONOFF_ECO_reg_ldo23_en_START (0)
#define PMIC_LDO23_ONOFF_ECO_reg_ldo23_en_END (0)
#define PMIC_LDO23_ONOFF_ECO_st_ldo23_en_START (1)
#define PMIC_LDO23_ONOFF_ECO_st_ldo23_en_END (1)
#define PMIC_LDO23_ONOFF_ECO_reg_ldo23_eco_en_START (4)
#define PMIC_LDO23_ONOFF_ECO_reg_ldo23_eco_en_END (4)
#define PMIC_LDO23_ONOFF_ECO_st_ldo23_eco_en_START (5)
#define PMIC_LDO23_ONOFF_ECO_st_ldo23_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo24_en : 1;
        unsigned char st_ldo24_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo24_eco_en : 1;
        unsigned char st_ldo24_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO24_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO24_ONOFF_ECO_reg_ldo24_en_START (0)
#define PMIC_LDO24_ONOFF_ECO_reg_ldo24_en_END (0)
#define PMIC_LDO24_ONOFF_ECO_st_ldo24_en_START (1)
#define PMIC_LDO24_ONOFF_ECO_st_ldo24_en_END (1)
#define PMIC_LDO24_ONOFF_ECO_reg_ldo24_eco_en_START (4)
#define PMIC_LDO24_ONOFF_ECO_reg_ldo24_eco_en_END (4)
#define PMIC_LDO24_ONOFF_ECO_st_ldo24_eco_en_START (5)
#define PMIC_LDO24_ONOFF_ECO_st_ldo24_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo25_en : 1;
        unsigned char st_ldo25_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_LDO25_ONOFF_UNION;
#endif
#define PMIC_LDO25_ONOFF_reg_ldo25_en_START (0)
#define PMIC_LDO25_ONOFF_reg_ldo25_en_END (0)
#define PMIC_LDO25_ONOFF_st_ldo25_en_START (1)
#define PMIC_LDO25_ONOFF_st_ldo25_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo26_en : 1;
        unsigned char st_ldo26_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo26_eco_en : 1;
        unsigned char st_ldo26_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO26_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO26_ONOFF_ECO_reg_ldo26_en_START (0)
#define PMIC_LDO26_ONOFF_ECO_reg_ldo26_en_END (0)
#define PMIC_LDO26_ONOFF_ECO_st_ldo26_en_START (1)
#define PMIC_LDO26_ONOFF_ECO_st_ldo26_en_END (1)
#define PMIC_LDO26_ONOFF_ECO_reg_ldo26_eco_en_START (4)
#define PMIC_LDO26_ONOFF_ECO_reg_ldo26_eco_en_END (4)
#define PMIC_LDO26_ONOFF_ECO_st_ldo26_eco_en_START (5)
#define PMIC_LDO26_ONOFF_ECO_st_ldo26_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo27_en : 1;
        unsigned char st_ldo27_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo27_eco_en : 1;
        unsigned char st_ldo27_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO27_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO27_ONOFF_ECO_reg_ldo27_en_START (0)
#define PMIC_LDO27_ONOFF_ECO_reg_ldo27_en_END (0)
#define PMIC_LDO27_ONOFF_ECO_st_ldo27_en_START (1)
#define PMIC_LDO27_ONOFF_ECO_st_ldo27_en_END (1)
#define PMIC_LDO27_ONOFF_ECO_reg_ldo27_eco_en_START (4)
#define PMIC_LDO27_ONOFF_ECO_reg_ldo27_eco_en_END (4)
#define PMIC_LDO27_ONOFF_ECO_st_ldo27_eco_en_START (5)
#define PMIC_LDO27_ONOFF_ECO_st_ldo27_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo28_en : 1;
        unsigned char st_ldo28_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo28_eco_en : 1;
        unsigned char st_ldo28_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO28_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO28_ONOFF_ECO_reg_ldo28_en_START (0)
#define PMIC_LDO28_ONOFF_ECO_reg_ldo28_en_END (0)
#define PMIC_LDO28_ONOFF_ECO_st_ldo28_en_START (1)
#define PMIC_LDO28_ONOFF_ECO_st_ldo28_en_END (1)
#define PMIC_LDO28_ONOFF_ECO_reg_ldo28_eco_en_START (4)
#define PMIC_LDO28_ONOFF_ECO_reg_ldo28_eco_en_END (4)
#define PMIC_LDO28_ONOFF_ECO_st_ldo28_eco_en_START (5)
#define PMIC_LDO28_ONOFF_ECO_st_ldo28_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo29_en : 1;
        unsigned char st_ldo29_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo29_eco_en : 1;
        unsigned char st_ldo29_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO29_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO29_ONOFF_ECO_reg_ldo29_en_START (0)
#define PMIC_LDO29_ONOFF_ECO_reg_ldo29_en_END (0)
#define PMIC_LDO29_ONOFF_ECO_st_ldo29_en_START (1)
#define PMIC_LDO29_ONOFF_ECO_st_ldo29_en_END (1)
#define PMIC_LDO29_ONOFF_ECO_reg_ldo29_eco_en_START (4)
#define PMIC_LDO29_ONOFF_ECO_reg_ldo29_eco_en_END (4)
#define PMIC_LDO29_ONOFF_ECO_st_ldo29_eco_en_START (5)
#define PMIC_LDO29_ONOFF_ECO_st_ldo29_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo30_en : 1;
        unsigned char st_ldo30_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo30_eco_en : 1;
        unsigned char st_ldo30_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO30_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO30_ONOFF_ECO_reg_ldo30_en_START (0)
#define PMIC_LDO30_ONOFF_ECO_reg_ldo30_en_END (0)
#define PMIC_LDO30_ONOFF_ECO_st_ldo30_en_START (1)
#define PMIC_LDO30_ONOFF_ECO_st_ldo30_en_END (1)
#define PMIC_LDO30_ONOFF_ECO_reg_ldo30_eco_en_START (4)
#define PMIC_LDO30_ONOFF_ECO_reg_ldo30_eco_en_END (4)
#define PMIC_LDO30_ONOFF_ECO_st_ldo30_eco_en_START (5)
#define PMIC_LDO30_ONOFF_ECO_st_ldo30_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo32_en : 1;
        unsigned char st_ldo32_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo32_eco_en : 1;
        unsigned char st_ldo32_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO32_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO32_ONOFF_ECO_reg_ldo32_en_START (0)
#define PMIC_LDO32_ONOFF_ECO_reg_ldo32_en_END (0)
#define PMIC_LDO32_ONOFF_ECO_st_ldo32_en_START (1)
#define PMIC_LDO32_ONOFF_ECO_st_ldo32_en_END (1)
#define PMIC_LDO32_ONOFF_ECO_reg_ldo32_eco_en_START (4)
#define PMIC_LDO32_ONOFF_ECO_reg_ldo32_eco_en_END (4)
#define PMIC_LDO32_ONOFF_ECO_st_ldo32_eco_en_START (5)
#define PMIC_LDO32_ONOFF_ECO_st_ldo32_eco_en_END (5)
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
} PMIC_PMUH_ONOFF_ECO_UNION;
#endif
#define PMIC_PMUH_ONOFF_ECO_reg_pmuh_en_START (0)
#define PMIC_PMUH_ONOFF_ECO_reg_pmuh_en_END (0)
#define PMIC_PMUH_ONOFF_ECO_st_pmuh_en_START (1)
#define PMIC_PMUH_ONOFF_ECO_st_pmuh_en_END (1)
#define PMIC_PMUH_ONOFF_ECO_reg_pmuh_eco_en_START (4)
#define PMIC_PMUH_ONOFF_ECO_reg_pmuh_eco_en_END (4)
#define PMIC_PMUH_ONOFF_ECO_st_pmuh_eco_en_START (5)
#define PMIC_PMUH_ONOFF_ECO_st_pmuh_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo36_en : 1;
        unsigned char st_ldo36_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo36_eco_en : 1;
        unsigned char st_ldo36_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO36_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO36_ONOFF_ECO_reg_ldo36_en_START (0)
#define PMIC_LDO36_ONOFF_ECO_reg_ldo36_en_END (0)
#define PMIC_LDO36_ONOFF_ECO_st_ldo36_en_START (1)
#define PMIC_LDO36_ONOFF_ECO_st_ldo36_en_END (1)
#define PMIC_LDO36_ONOFF_ECO_reg_ldo36_eco_en_START (4)
#define PMIC_LDO36_ONOFF_ECO_reg_ldo36_eco_en_END (4)
#define PMIC_LDO36_ONOFF_ECO_st_ldo36_eco_en_START (5)
#define PMIC_LDO36_ONOFF_ECO_st_ldo36_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo37_en : 1;
        unsigned char st_ldo37_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo37_eco_en : 1;
        unsigned char st_ldo37_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO37_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO37_ONOFF_ECO_reg_ldo37_en_START (0)
#define PMIC_LDO37_ONOFF_ECO_reg_ldo37_en_END (0)
#define PMIC_LDO37_ONOFF_ECO_st_ldo37_en_START (1)
#define PMIC_LDO37_ONOFF_ECO_st_ldo37_en_END (1)
#define PMIC_LDO37_ONOFF_ECO_reg_ldo37_eco_en_START (4)
#define PMIC_LDO37_ONOFF_ECO_reg_ldo37_eco_en_END (4)
#define PMIC_LDO37_ONOFF_ECO_st_ldo37_eco_en_START (5)
#define PMIC_LDO37_ONOFF_ECO_st_ldo37_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo38_en : 1;
        unsigned char st_ldo38_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo38_eco_en : 1;
        unsigned char st_ldo38_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO38_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO38_ONOFF_ECO_reg_ldo38_en_START (0)
#define PMIC_LDO38_ONOFF_ECO_reg_ldo38_en_END (0)
#define PMIC_LDO38_ONOFF_ECO_st_ldo38_en_START (1)
#define PMIC_LDO38_ONOFF_ECO_st_ldo38_en_END (1)
#define PMIC_LDO38_ONOFF_ECO_reg_ldo38_eco_en_START (4)
#define PMIC_LDO38_ONOFF_ECO_reg_ldo38_eco_en_END (4)
#define PMIC_LDO38_ONOFF_ECO_st_ldo38_eco_en_START (5)
#define PMIC_LDO38_ONOFF_ECO_st_ldo38_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_LDO40_ONOFF_ECO_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo41_en : 1;
        unsigned char st_ldo41_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo41_eco_en : 1;
        unsigned char st_ldo41_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO41_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO41_ONOFF_ECO_reg_ldo41_en_START (0)
#define PMIC_LDO41_ONOFF_ECO_reg_ldo41_en_END (0)
#define PMIC_LDO41_ONOFF_ECO_st_ldo41_en_START (1)
#define PMIC_LDO41_ONOFF_ECO_st_ldo41_en_END (1)
#define PMIC_LDO41_ONOFF_ECO_reg_ldo41_eco_en_START (4)
#define PMIC_LDO41_ONOFF_ECO_reg_ldo41_eco_en_END (4)
#define PMIC_LDO41_ONOFF_ECO_st_ldo41_eco_en_START (5)
#define PMIC_LDO41_ONOFF_ECO_st_ldo41_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo42_en : 1;
        unsigned char st_ldo42_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo42_eco_en : 1;
        unsigned char st_ldo42_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO42_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO42_ONOFF_ECO_reg_ldo42_en_START (0)
#define PMIC_LDO42_ONOFF_ECO_reg_ldo42_en_END (0)
#define PMIC_LDO42_ONOFF_ECO_st_ldo42_en_START (1)
#define PMIC_LDO42_ONOFF_ECO_st_ldo42_en_END (1)
#define PMIC_LDO42_ONOFF_ECO_reg_ldo42_eco_en_START (4)
#define PMIC_LDO42_ONOFF_ECO_reg_ldo42_eco_en_END (4)
#define PMIC_LDO42_ONOFF_ECO_st_ldo42_eco_en_START (5)
#define PMIC_LDO42_ONOFF_ECO_st_ldo42_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo43_en : 1;
        unsigned char st_ldo43_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo43_eco_en : 1;
        unsigned char st_ldo43_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO43_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO43_ONOFF_ECO_reg_ldo43_en_START (0)
#define PMIC_LDO43_ONOFF_ECO_reg_ldo43_en_END (0)
#define PMIC_LDO43_ONOFF_ECO_st_ldo43_en_START (1)
#define PMIC_LDO43_ONOFF_ECO_st_ldo43_en_END (1)
#define PMIC_LDO43_ONOFF_ECO_reg_ldo43_eco_en_START (4)
#define PMIC_LDO43_ONOFF_ECO_reg_ldo43_eco_en_END (4)
#define PMIC_LDO43_ONOFF_ECO_st_ldo43_eco_en_START (5)
#define PMIC_LDO43_ONOFF_ECO_st_ldo43_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo44_en : 1;
        unsigned char st_ldo44_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo44_eco_en : 1;
        unsigned char st_ldo44_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO44_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO44_ONOFF_ECO_reg_ldo44_en_START (0)
#define PMIC_LDO44_ONOFF_ECO_reg_ldo44_en_END (0)
#define PMIC_LDO44_ONOFF_ECO_st_ldo44_en_START (1)
#define PMIC_LDO44_ONOFF_ECO_st_ldo44_en_END (1)
#define PMIC_LDO44_ONOFF_ECO_reg_ldo44_eco_en_START (4)
#define PMIC_LDO44_ONOFF_ECO_reg_ldo44_eco_en_END (4)
#define PMIC_LDO44_ONOFF_ECO_st_ldo44_eco_en_START (5)
#define PMIC_LDO44_ONOFF_ECO_st_ldo44_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo45_en : 1;
        unsigned char st_ldo45_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo45_eco_en : 1;
        unsigned char st_ldo45_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO45_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO45_ONOFF_ECO_reg_ldo45_en_START (0)
#define PMIC_LDO45_ONOFF_ECO_reg_ldo45_en_END (0)
#define PMIC_LDO45_ONOFF_ECO_st_ldo45_en_START (1)
#define PMIC_LDO45_ONOFF_ECO_st_ldo45_en_END (1)
#define PMIC_LDO45_ONOFF_ECO_reg_ldo45_eco_en_START (4)
#define PMIC_LDO45_ONOFF_ECO_reg_ldo45_eco_en_END (4)
#define PMIC_LDO45_ONOFF_ECO_st_ldo45_eco_en_START (5)
#define PMIC_LDO45_ONOFF_ECO_st_ldo45_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo46_en : 1;
        unsigned char st_ldo46_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo46_eco_en : 1;
        unsigned char st_ldo46_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO46_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO46_ONOFF_ECO_reg_ldo46_en_START (0)
#define PMIC_LDO46_ONOFF_ECO_reg_ldo46_en_END (0)
#define PMIC_LDO46_ONOFF_ECO_st_ldo46_en_START (1)
#define PMIC_LDO46_ONOFF_ECO_st_ldo46_en_END (1)
#define PMIC_LDO46_ONOFF_ECO_reg_ldo46_eco_en_START (4)
#define PMIC_LDO46_ONOFF_ECO_reg_ldo46_eco_en_END (4)
#define PMIC_LDO46_ONOFF_ECO_st_ldo46_eco_en_START (5)
#define PMIC_LDO46_ONOFF_ECO_st_ldo46_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_sw1_n50_en : 1;
        unsigned char st_sw1_n50_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_SW1_ONOFF_UNION;
#endif
#define PMIC_SW1_ONOFF_reg_sw1_n50_en_START (0)
#define PMIC_SW1_ONOFF_reg_sw1_n50_en_END (0)
#define PMIC_SW1_ONOFF_st_sw1_n50_en_START (1)
#define PMIC_SW1_ONOFF_st_sw1_n50_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo_sink_en : 1;
        unsigned char st_ldo_sink_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo_sink_eco_en : 1;
        unsigned char st_ldo_sink_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO_SINK_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO_SINK_ONOFF_ECO_reg_ldo_sink_en_START (0)
#define PMIC_LDO_SINK_ONOFF_ECO_reg_ldo_sink_en_END (0)
#define PMIC_LDO_SINK_ONOFF_ECO_st_ldo_sink_en_START (1)
#define PMIC_LDO_SINK_ONOFF_ECO_st_ldo_sink_en_END (1)
#define PMIC_LDO_SINK_ONOFF_ECO_reg_ldo_sink_eco_en_START (4)
#define PMIC_LDO_SINK_ONOFF_ECO_reg_ldo_sink_eco_en_END (4)
#define PMIC_LDO_SINK_ONOFF_ECO_st_ldo_sink_eco_en_START (5)
#define PMIC_LDO_SINK_ONOFF_ECO_st_ldo_sink_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char st_ref_en : 1;
        unsigned char reserved : 3;
        unsigned char reg_ref_eco_en : 1;
        unsigned char st_ref_eco_en : 1;
        unsigned char reg_thsd_eco_en : 1;
        unsigned char st_thsd_eco_en : 1;
    } reg;
} PMIC_REF_ECO_UNION;
#endif
#define PMIC_REF_ECO_st_ref_en_START (0)
#define PMIC_REF_ECO_st_ref_en_END (0)
#define PMIC_REF_ECO_reg_ref_eco_en_START (4)
#define PMIC_REF_ECO_reg_ref_eco_en_END (4)
#define PMIC_REF_ECO_st_ref_eco_en_START (5)
#define PMIC_REF_ECO_st_ref_eco_en_END (5)
#define PMIC_REF_ECO_reg_thsd_eco_en_START (6)
#define PMIC_REF_ECO_reg_thsd_eco_en_END (6)
#define PMIC_REF_ECO_st_thsd_eco_en_START (7)
#define PMIC_REF_ECO_st_thsd_eco_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_bb_en : 1;
        unsigned char st_bb_en : 1;
        unsigned char st_ext_pmu_en : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_EXT_PMU_ONOFF_UNION;
#endif
#define PMIC_EXT_PMU_ONOFF_reg_bb_en_START (0)
#define PMIC_EXT_PMU_ONOFF_reg_bb_en_END (0)
#define PMIC_EXT_PMU_ONOFF_st_bb_en_START (1)
#define PMIC_EXT_PMU_ONOFF_st_bb_en_END (1)
#define PMIC_EXT_PMU_ONOFF_st_ext_pmu_en_START (2)
#define PMIC_EXT_PMU_ONOFF_st_ext_pmu_en_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_serdes_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_CLK_SERDES_EN_UNION;
#endif
#define PMIC_CLK_SERDES_EN_reg_xo_serdes_en_START (0)
#define PMIC_CLK_SERDES_EN_reg_xo_serdes_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_wifi_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_CLK_WIFI_EN_UNION;
#endif
#define PMIC_CLK_WIFI_EN_reg_xo_wifi_en_START (0)
#define PMIC_CLK_WIFI_EN_reg_xo_wifi_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_nfc_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_CLK_NFC_EN_UNION;
#endif
#define PMIC_CLK_NFC_EN_reg_xo_nfc_en_START (0)
#define PMIC_CLK_NFC_EN_reg_xo_nfc_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_rf0_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_CLK_RF0_EN_UNION;
#endif
#define PMIC_CLK_RF0_EN_xo_rf0_en_START (0)
#define PMIC_CLK_RF0_EN_xo_rf0_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_rf1_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_CLK_RF1_EN_UNION;
#endif
#define PMIC_CLK_RF1_EN_xo_rf1_en_START (0)
#define PMIC_CLK_RF1_EN_xo_rf1_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_sys_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_CLK_SYS_EN_UNION;
#endif
#define PMIC_CLK_SYS_EN_reg_xo_sys_en_START (0)
#define PMIC_CLK_SYS_EN_reg_xo_sys_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_codec_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_CLK_CODEC_EN_UNION;
#endif
#define PMIC_CLK_CODEC_EN_reg_xo_codec_en_START (0)
#define PMIC_CLK_CODEC_EN_reg_xo_codec_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_ufs_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_CLK_UFS_EN_UNION;
#endif
#define PMIC_CLK_UFS_EN_reg_xo_ufs_en_START (0)
#define PMIC_CLK_UFS_EN_reg_xo_ufs_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_xo_eusb_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_CLK_EUSB_EN_UNION;
#endif
#define PMIC_CLK_EUSB_EN_reg_xo_eusb_en_START (0)
#define PMIC_CLK_EUSB_EN_reg_xo_eusb_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char en_32k_gps : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_OSC32K_GPS_ONOFF_CTRL_UNION;
#endif
#define PMIC_OSC32K_GPS_ONOFF_CTRL_en_32k_gps_START (0)
#define PMIC_OSC32K_GPS_ONOFF_CTRL_en_32k_gps_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char en_32k_bt : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_OSC32K_BT_ONOFF_CTRL_UNION;
#endif
#define PMIC_OSC32K_BT_ONOFF_CTRL_en_32k_bt_START (0)
#define PMIC_OSC32K_BT_ONOFF_CTRL_en_32k_bt_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char en_32k_sys : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_OSC32K_SYS_ONOFF_CTRL_UNION;
#endif
#define PMIC_OSC32K_SYS_ONOFF_CTRL_en_32k_sys_START (0)
#define PMIC_OSC32K_SYS_ONOFF_CTRL_en_32k_sys_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_32k_pmu_en0 : 8;
    } reg;
} PMIC_CLK_32K_PMU_CTRL0_UNION;
#endif
#define PMIC_CLK_32K_PMU_CTRL0_reg_32k_pmu_en0_START (0)
#define PMIC_CLK_32K_PMU_CTRL0_reg_32k_pmu_en0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_32k_pmu_en1 : 8;
    } reg;
} PMIC_CLK_32K_PMU_CTRL1_UNION;
#endif
#define PMIC_CLK_32K_PMU_CTRL1_reg_32k_pmu_en1_START (0)
#define PMIC_CLK_32K_PMU_CTRL1_reg_32k_pmu_en1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_lra_clk_en_a : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_LRA_CLK_ONOFF1_UNION;
#endif
#define PMIC_LRA_CLK_ONOFF1_reg_lra_clk_en_a_START (0)
#define PMIC_LRA_CLK_ONOFF1_reg_lra_clk_en_a_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_lra_clk_en_b : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_LRA_CLK_ONOFF2_UNION;
#endif
#define PMIC_LRA_CLK_ONOFF2_reg_lra_clk_en_b_START (0)
#define PMIC_LRA_CLK_ONOFF2_reg_lra_clk_en_b_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck1_vset : 7;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK1_VSET_UNION;
#endif
#define PMIC_BUCK1_VSET_buck1_vset_START (0)
#define PMIC_BUCK1_VSET_buck1_vset_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck2_vset_cfg : 7;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK2_VSET_UNION;
#endif
#define PMIC_BUCK2_VSET_buck2_vset_cfg_START (0)
#define PMIC_BUCK2_VSET_buck2_vset_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck2_vset_eco : 7;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK2_VSET_ECO_UNION;
#endif
#define PMIC_BUCK2_VSET_ECO_buck2_vset_eco_START (0)
#define PMIC_BUCK2_VSET_ECO_buck2_vset_eco_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck3_vset_cfg : 7;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK3_VSET_UNION;
#endif
#define PMIC_BUCK3_VSET_buck3_vset_cfg_START (0)
#define PMIC_BUCK3_VSET_buck3_vset_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck3_vset_eco : 7;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK3_VSET_ECO_UNION;
#endif
#define PMIC_BUCK3_VSET_ECO_buck3_vset_eco_START (0)
#define PMIC_BUCK3_VSET_ECO_buck3_vset_eco_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck40_vset_cfg : 8;
    } reg;
} PMIC_BUCK40_VSET_UNION;
#endif
#define PMIC_BUCK40_VSET_buck40_vset_cfg_START (0)
#define PMIC_BUCK40_VSET_buck40_vset_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck42_vset_cfg : 8;
    } reg;
} PMIC_BUCK42_VSET_UNION;
#endif
#define PMIC_BUCK42_VSET_buck42_vset_cfg_START (0)
#define PMIC_BUCK42_VSET_buck42_vset_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck5_vset_cfg : 8;
    } reg;
} PMIC_BUCK5_VSET_UNION;
#endif
#define PMIC_BUCK5_VSET_buck5_vset_cfg_START (0)
#define PMIC_BUCK5_VSET_buck5_vset_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck5_vset_eco : 8;
    } reg;
} PMIC_BUCK5_VSET_ECO_UNION;
#endif
#define PMIC_BUCK5_VSET_ECO_buck5_vset_eco_START (0)
#define PMIC_BUCK5_VSET_ECO_buck5_vset_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck70_vset_cfg : 8;
    } reg;
} PMIC_BUCK70_VSET_UNION;
#endif
#define PMIC_BUCK70_VSET_buck70_vset_cfg_START (0)
#define PMIC_BUCK70_VSET_buck70_vset_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck71_vset_cfg : 8;
    } reg;
} PMIC_BUCK71_VSET_UNION;
#endif
#define PMIC_BUCK71_VSET_buck71_vset_cfg_START (0)
#define PMIC_BUCK71_VSET_buck71_vset_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck9_vset_cfg : 8;
    } reg;
} PMIC_BUCK9_VSET_UNION;
#endif
#define PMIC_BUCK9_VSET_buck9_vset_cfg_START (0)
#define PMIC_BUCK9_VSET_buck9_vset_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck9_vset_eco : 8;
    } reg;
} PMIC_BUCK9_VSET_ECO_UNION;
#endif
#define PMIC_BUCK9_VSET_ECO_buck9_vset_eco_START (0)
#define PMIC_BUCK9_VSET_ECO_buck9_vset_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo0_vset_cfg : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO0_VSET_UNION;
#endif
#define PMIC_LDO0_VSET_ldo0_vset_cfg_START (0)
#define PMIC_LDO0_VSET_ldo0_vset_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo0_vset_eco : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO0_VSET_ECO_UNION;
#endif
#define PMIC_LDO0_VSET_ECO_ldo0_vset_eco_START (0)
#define PMIC_LDO0_VSET_ECO_ldo0_vset_eco_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo1_vset : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_LDO1_VSET_UNION;
#endif
#define PMIC_LDO1_VSET_ldo1_vset_START (0)
#define PMIC_LDO1_VSET_ldo1_vset_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo3_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO3_VSET_UNION;
#endif
#define PMIC_LDO3_VSET_ldo3_vset_START (0)
#define PMIC_LDO3_VSET_ldo3_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo4_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO4_VSET_UNION;
#endif
#define PMIC_LDO4_VSET_ldo4_vset_START (0)
#define PMIC_LDO4_VSET_ldo4_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo6_vset : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO6_VSET_UNION;
#endif
#define PMIC_LDO6_VSET_ldo6_vset_START (0)
#define PMIC_LDO6_VSET_ldo6_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo8_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO8_VSET_UNION;
#endif
#define PMIC_LDO8_VSET_ldo8_vset_START (0)
#define PMIC_LDO8_VSET_ldo8_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo11_vset : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO11_VSET_UNION;
#endif
#define PMIC_LDO11_VSET_ldo11_vset_START (0)
#define PMIC_LDO11_VSET_ldo11_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo12_vset : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO12_VSET_UNION;
#endif
#define PMIC_LDO12_VSET_ldo12_vset_START (0)
#define PMIC_LDO12_VSET_ldo12_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo14_vset : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO14_VSET_UNION;
#endif
#define PMIC_LDO14_VSET_ldo14_vset_START (0)
#define PMIC_LDO14_VSET_ldo14_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo15_vset : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO15_VSET_UNION;
#endif
#define PMIC_LDO15_VSET_ldo15_vset_START (0)
#define PMIC_LDO15_VSET_ldo15_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo16_vset : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO16_VSET_UNION;
#endif
#define PMIC_LDO16_VSET_ldo16_vset_START (0)
#define PMIC_LDO16_VSET_ldo16_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo17_vset : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO17_VSET_UNION;
#endif
#define PMIC_LDO17_VSET_ldo17_vset_START (0)
#define PMIC_LDO17_VSET_ldo17_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo18_vset : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_LDO18_VSET_UNION;
#endif
#define PMIC_LDO18_VSET_ldo18_vset_START (0)
#define PMIC_LDO18_VSET_ldo18_vset_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo21_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO21_VSET_UNION;
#endif
#define PMIC_LDO21_VSET_ldo21_vset_START (0)
#define PMIC_LDO21_VSET_ldo21_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo22_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO22_VSET_UNION;
#endif
#define PMIC_LDO22_VSET_ldo22_vset_START (0)
#define PMIC_LDO22_VSET_ldo22_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo23_vset : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO23_VSET_UNION;
#endif
#define PMIC_LDO23_VSET_ldo23_vset_START (0)
#define PMIC_LDO23_VSET_ldo23_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo24_vset : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO24_VSET_UNION;
#endif
#define PMIC_LDO24_VSET_ldo24_vset_START (0)
#define PMIC_LDO24_VSET_ldo24_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo25_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO25_VSET_UNION;
#endif
#define PMIC_LDO25_VSET_ldo25_vset_START (0)
#define PMIC_LDO25_VSET_ldo25_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo26_vset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO26_VSET_UNION;
#endif
#define PMIC_LDO26_VSET_ldo26_vset_START (0)
#define PMIC_LDO26_VSET_ldo26_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo27_vset : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_LDO27_VSET_UNION;
#endif
#define PMIC_LDO27_VSET_ldo27_vset_START (0)
#define PMIC_LDO27_VSET_ldo27_vset_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo28_vset : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_LDO28_VSET_UNION;
#endif
#define PMIC_LDO28_VSET_ldo28_vset_START (0)
#define PMIC_LDO28_VSET_ldo28_vset_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo29_vset : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_LDO29_VSET_UNION;
#endif
#define PMIC_LDO29_VSET_ldo29_vset_START (0)
#define PMIC_LDO29_VSET_ldo29_vset_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo30_vset_cfg : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO30_VSET_UNION;
#endif
#define PMIC_LDO30_VSET_ldo30_vset_cfg_START (0)
#define PMIC_LDO30_VSET_ldo30_vset_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo30_vset_eco : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO30_VSET_ECO_UNION;
#endif
#define PMIC_LDO30_VSET_ECO_ldo30_vset_eco_START (0)
#define PMIC_LDO30_VSET_ECO_ldo30_vset_eco_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo32_vset : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_LDO32_VSET_UNION;
#endif
#define PMIC_LDO32_VSET_ldo32_vset_START (0)
#define PMIC_LDO32_VSET_ldo32_vset_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pmuh_vset : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_PMUH_VSET_UNION;
#endif
#define PMIC_PMUH_VSET_pmuh_vset_START (0)
#define PMIC_PMUH_VSET_pmuh_vset_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo36_vset : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_LDO36_VSET_UNION;
#endif
#define PMIC_LDO36_VSET_ldo36_vset_START (0)
#define PMIC_LDO36_VSET_ldo36_vset_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo37_vset : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_LDO37_VSET_UNION;
#endif
#define PMIC_LDO37_VSET_ldo37_vset_START (0)
#define PMIC_LDO37_VSET_ldo37_vset_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo38_vset : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_LDO38_VSET_UNION;
#endif
#define PMIC_LDO38_VSET_ldo38_vset_START (0)
#define PMIC_LDO38_VSET_ldo38_vset_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_LDO40_VSET_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_LDO40_VSET_ECO_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo41_vset : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_LDO41_VSET_UNION;
#endif
#define PMIC_LDO41_VSET_ldo41_vset_START (0)
#define PMIC_LDO41_VSET_ldo41_vset_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo42_vset : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO42_VSET_UNION;
#endif
#define PMIC_LDO42_VSET_ldo42_vset_START (0)
#define PMIC_LDO42_VSET_ldo42_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo43_vset : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO43_VSET_UNION;
#endif
#define PMIC_LDO43_VSET_ldo43_vset_START (0)
#define PMIC_LDO43_VSET_ldo43_vset_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo44_vset_cfg : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO44_VSET_UNION;
#endif
#define PMIC_LDO44_VSET_ldo44_vset_cfg_START (0)
#define PMIC_LDO44_VSET_ldo44_vset_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo44_vset_eco : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO44_VSET_ECO_UNION;
#endif
#define PMIC_LDO44_VSET_ECO_ldo44_vset_eco_START (0)
#define PMIC_LDO44_VSET_ECO_ldo44_vset_eco_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo45_vset_cfg : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO45_VSET_UNION;
#endif
#define PMIC_LDO45_VSET_ldo45_vset_cfg_START (0)
#define PMIC_LDO45_VSET_ldo45_vset_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo46_vset : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_LDO46_VSET_UNION;
#endif
#define PMIC_LDO46_VSET_ldo46_vset_START (0)
#define PMIC_LDO46_VSET_ldo46_vset_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_sink_vset : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_LDO_SINK_VSET_UNION;
#endif
#define PMIC_LDO_SINK_VSET_ldo_sink_vset_START (0)
#define PMIC_LDO_SINK_VSET_ldo_sink_vset_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_buf_vset : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_LDO_BUF_VSET_UNION;
#endif
#define PMIC_LDO_BUF_VSET_ldo_buf_vset_START (0)
#define PMIC_LDO_BUF_VSET_ldo_buf_vset_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pmud_vset : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_PMUD_VSET_UNION;
#endif
#define PMIC_PMUD_VSET_pmud_vset_START (0)
#define PMIC_PMUD_VSET_pmud_vset_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck40_ocp_auto_stop : 2;
        unsigned char buck3_ocp_auto_stop : 2;
        unsigned char buck2_ocp_auto_stop : 2;
        unsigned char buck1_ocp_auto_stop : 2;
    } reg;
} PMIC_BUCK1_40_OCP_CTRL_UNION;
#endif
#define PMIC_BUCK1_40_OCP_CTRL_buck40_ocp_auto_stop_START (0)
#define PMIC_BUCK1_40_OCP_CTRL_buck40_ocp_auto_stop_END (1)
#define PMIC_BUCK1_40_OCP_CTRL_buck3_ocp_auto_stop_START (2)
#define PMIC_BUCK1_40_OCP_CTRL_buck3_ocp_auto_stop_END (3)
#define PMIC_BUCK1_40_OCP_CTRL_buck2_ocp_auto_stop_START (4)
#define PMIC_BUCK1_40_OCP_CTRL_buck2_ocp_auto_stop_END (5)
#define PMIC_BUCK1_40_OCP_CTRL_buck1_ocp_auto_stop_START (6)
#define PMIC_BUCK1_40_OCP_CTRL_buck1_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck71_ocp_auto_stop : 2;
        unsigned char buck70_ocp_auto_stop : 2;
        unsigned char buck5_ocp_auto_stop : 2;
        unsigned char buck42_ocp_auto_stop : 2;
    } reg;
} PMIC_BUCK42_71_OCP_CTRL_UNION;
#endif
#define PMIC_BUCK42_71_OCP_CTRL_buck71_ocp_auto_stop_START (0)
#define PMIC_BUCK42_71_OCP_CTRL_buck71_ocp_auto_stop_END (1)
#define PMIC_BUCK42_71_OCP_CTRL_buck70_ocp_auto_stop_START (2)
#define PMIC_BUCK42_71_OCP_CTRL_buck70_ocp_auto_stop_END (3)
#define PMIC_BUCK42_71_OCP_CTRL_buck5_ocp_auto_stop_START (4)
#define PMIC_BUCK42_71_OCP_CTRL_buck5_ocp_auto_stop_END (5)
#define PMIC_BUCK42_71_OCP_CTRL_buck42_ocp_auto_stop_START (6)
#define PMIC_BUCK42_71_OCP_CTRL_buck42_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck9_ocp_auto_stop : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_BUCK9_OCP_CTRL_UNION;
#endif
#define PMIC_BUCK9_OCP_CTRL_buck9_ocp_auto_stop_START (0)
#define PMIC_BUCK9_OCP_CTRL_buck9_ocp_auto_stop_END (1)
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
} PMIC_LDO0_3_OCP_CTRL_UNION;
#endif
#define PMIC_LDO0_3_OCP_CTRL_ldo3_ocp_auto_stop_START (0)
#define PMIC_LDO0_3_OCP_CTRL_ldo3_ocp_auto_stop_END (1)
#define PMIC_LDO0_3_OCP_CTRL_ldo2_ocp_auto_stop_START (2)
#define PMIC_LDO0_3_OCP_CTRL_ldo2_ocp_auto_stop_END (3)
#define PMIC_LDO0_3_OCP_CTRL_ldo1_ocp_auto_stop_START (4)
#define PMIC_LDO0_3_OCP_CTRL_ldo1_ocp_auto_stop_END (5)
#define PMIC_LDO0_3_OCP_CTRL_ldo0_ocp_auto_stop_START (6)
#define PMIC_LDO0_3_OCP_CTRL_ldo0_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo11_ocp_auto_stop : 2;
        unsigned char ldo8_ocp_auto_stop : 2;
        unsigned char ldo6_ocp_auto_stop : 2;
        unsigned char ldo4_ocp_auto_stop : 2;
    } reg;
} PMIC_LDO4_11_OCP_CTRL_UNION;
#endif
#define PMIC_LDO4_11_OCP_CTRL_ldo11_ocp_auto_stop_START (0)
#define PMIC_LDO4_11_OCP_CTRL_ldo11_ocp_auto_stop_END (1)
#define PMIC_LDO4_11_OCP_CTRL_ldo8_ocp_auto_stop_START (2)
#define PMIC_LDO4_11_OCP_CTRL_ldo8_ocp_auto_stop_END (3)
#define PMIC_LDO4_11_OCP_CTRL_ldo6_ocp_auto_stop_START (4)
#define PMIC_LDO4_11_OCP_CTRL_ldo6_ocp_auto_stop_END (5)
#define PMIC_LDO4_11_OCP_CTRL_ldo4_ocp_auto_stop_START (6)
#define PMIC_LDO4_11_OCP_CTRL_ldo4_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo16_ocp_auto_stop : 2;
        unsigned char ldo15_ocp_auto_stop : 2;
        unsigned char ldo14_ocp_auto_stop : 2;
        unsigned char ldo12_ocp_auto_stop : 2;
    } reg;
} PMIC_LDO12_16_OCP_CTRL_UNION;
#endif
#define PMIC_LDO12_16_OCP_CTRL_ldo16_ocp_auto_stop_START (0)
#define PMIC_LDO12_16_OCP_CTRL_ldo16_ocp_auto_stop_END (1)
#define PMIC_LDO12_16_OCP_CTRL_ldo15_ocp_auto_stop_START (2)
#define PMIC_LDO12_16_OCP_CTRL_ldo15_ocp_auto_stop_END (3)
#define PMIC_LDO12_16_OCP_CTRL_ldo14_ocp_auto_stop_START (4)
#define PMIC_LDO12_16_OCP_CTRL_ldo14_ocp_auto_stop_END (5)
#define PMIC_LDO12_16_OCP_CTRL_ldo12_ocp_auto_stop_START (6)
#define PMIC_LDO12_16_OCP_CTRL_ldo12_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo22_ocp_auto_stop : 2;
        unsigned char ldo21_ocp_auto_stop : 2;
        unsigned char ldo18_ocp_auto_stop : 2;
        unsigned char ldo17_ocp_auto_stop : 2;
    } reg;
} PMIC_LDO17_22_OCP_CTRL_UNION;
#endif
#define PMIC_LDO17_22_OCP_CTRL_ldo22_ocp_auto_stop_START (0)
#define PMIC_LDO17_22_OCP_CTRL_ldo22_ocp_auto_stop_END (1)
#define PMIC_LDO17_22_OCP_CTRL_ldo21_ocp_auto_stop_START (2)
#define PMIC_LDO17_22_OCP_CTRL_ldo21_ocp_auto_stop_END (3)
#define PMIC_LDO17_22_OCP_CTRL_ldo18_ocp_auto_stop_START (4)
#define PMIC_LDO17_22_OCP_CTRL_ldo18_ocp_auto_stop_END (5)
#define PMIC_LDO17_22_OCP_CTRL_ldo17_ocp_auto_stop_START (6)
#define PMIC_LDO17_22_OCP_CTRL_ldo17_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo26_ocp_auto_stop : 2;
        unsigned char ldo25_ocp_auto_stop : 2;
        unsigned char ldo24_ocp_auto_stop : 2;
        unsigned char ldo23_ocp_auto_stop : 2;
    } reg;
} PMIC_LDO23_26_OCP_CTRL_UNION;
#endif
#define PMIC_LDO23_26_OCP_CTRL_ldo26_ocp_auto_stop_START (0)
#define PMIC_LDO23_26_OCP_CTRL_ldo26_ocp_auto_stop_END (1)
#define PMIC_LDO23_26_OCP_CTRL_ldo25_ocp_auto_stop_START (2)
#define PMIC_LDO23_26_OCP_CTRL_ldo25_ocp_auto_stop_END (3)
#define PMIC_LDO23_26_OCP_CTRL_ldo24_ocp_auto_stop_START (4)
#define PMIC_LDO23_26_OCP_CTRL_ldo24_ocp_auto_stop_END (5)
#define PMIC_LDO23_26_OCP_CTRL_ldo23_ocp_auto_stop_START (6)
#define PMIC_LDO23_26_OCP_CTRL_ldo23_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo30_ocp_auto_stop : 2;
        unsigned char ldo29_ocp_auto_stop : 2;
        unsigned char ldo28_ocp_auto_stop : 2;
        unsigned char ldo27_ocp_auto_stop : 2;
    } reg;
} PMIC_LDO27_30_OCP_CTRL_UNION;
#endif
#define PMIC_LDO27_30_OCP_CTRL_ldo30_ocp_auto_stop_START (0)
#define PMIC_LDO27_30_OCP_CTRL_ldo30_ocp_auto_stop_END (1)
#define PMIC_LDO27_30_OCP_CTRL_ldo29_ocp_auto_stop_START (2)
#define PMIC_LDO27_30_OCP_CTRL_ldo29_ocp_auto_stop_END (3)
#define PMIC_LDO27_30_OCP_CTRL_ldo28_ocp_auto_stop_START (4)
#define PMIC_LDO27_30_OCP_CTRL_ldo28_ocp_auto_stop_END (5)
#define PMIC_LDO27_30_OCP_CTRL_ldo27_ocp_auto_stop_START (6)
#define PMIC_LDO27_30_OCP_CTRL_ldo27_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo38_ocp_auto_stop : 2;
        unsigned char ldo37_ocp_auto_stop : 2;
        unsigned char ldo36_ocp_auto_stop : 2;
        unsigned char ldo32_ocp_auto_stop : 2;
    } reg;
} PMIC_LDO32_38_OCP_CTRL_UNION;
#endif
#define PMIC_LDO32_38_OCP_CTRL_ldo38_ocp_auto_stop_START (0)
#define PMIC_LDO32_38_OCP_CTRL_ldo38_ocp_auto_stop_END (1)
#define PMIC_LDO32_38_OCP_CTRL_ldo37_ocp_auto_stop_START (2)
#define PMIC_LDO32_38_OCP_CTRL_ldo37_ocp_auto_stop_END (3)
#define PMIC_LDO32_38_OCP_CTRL_ldo36_ocp_auto_stop_START (4)
#define PMIC_LDO32_38_OCP_CTRL_ldo36_ocp_auto_stop_END (5)
#define PMIC_LDO32_38_OCP_CTRL_ldo32_ocp_auto_stop_START (6)
#define PMIC_LDO32_38_OCP_CTRL_ldo32_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo42_ocp_auto_stop : 2;
        unsigned char ldo41_ocp_auto_stop : 2;
        unsigned char reserved : 2;
        unsigned char ldo39_ocp_auto_stop : 2;
    } reg;
} PMIC_LDO39_42_OCP_CTRL_UNION;
#endif
#define PMIC_LDO39_42_OCP_CTRL_ldo42_ocp_auto_stop_START (0)
#define PMIC_LDO39_42_OCP_CTRL_ldo42_ocp_auto_stop_END (1)
#define PMIC_LDO39_42_OCP_CTRL_ldo41_ocp_auto_stop_START (2)
#define PMIC_LDO39_42_OCP_CTRL_ldo41_ocp_auto_stop_END (3)
#define PMIC_LDO39_42_OCP_CTRL_ldo39_ocp_auto_stop_START (6)
#define PMIC_LDO39_42_OCP_CTRL_ldo39_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo46_ocp_auto_stop : 2;
        unsigned char ldo45_ocp_auto_stop : 2;
        unsigned char ldo44_ocp_auto_stop : 2;
        unsigned char ldo43_ocp_auto_stop : 2;
    } reg;
} PMIC_LDO43_46_OCP_CTRL_UNION;
#endif
#define PMIC_LDO43_46_OCP_CTRL_ldo46_ocp_auto_stop_START (0)
#define PMIC_LDO43_46_OCP_CTRL_ldo46_ocp_auto_stop_END (1)
#define PMIC_LDO43_46_OCP_CTRL_ldo45_ocp_auto_stop_START (2)
#define PMIC_LDO43_46_OCP_CTRL_ldo45_ocp_auto_stop_END (3)
#define PMIC_LDO43_46_OCP_CTRL_ldo44_ocp_auto_stop_START (4)
#define PMIC_LDO43_46_OCP_CTRL_ldo44_ocp_auto_stop_END (5)
#define PMIC_LDO43_46_OCP_CTRL_ldo43_ocp_auto_stop_START (6)
#define PMIC_LDO43_46_OCP_CTRL_ldo43_ocp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_buff_scp_auto_stop : 2;
        unsigned char ldo_sink_ocp_auto_stop : 2;
        unsigned char sw1_ocp_auto_stop : 2;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO_BUFF_SCP_CTRL_UNION;
#endif
#define PMIC_LDO_BUFF_SCP_CTRL_ldo_buff_scp_auto_stop_START (0)
#define PMIC_LDO_BUFF_SCP_CTRL_ldo_buff_scp_auto_stop_END (1)
#define PMIC_LDO_BUFF_SCP_CTRL_ldo_sink_ocp_auto_stop_START (2)
#define PMIC_LDO_BUFF_SCP_CTRL_ldo_sink_ocp_auto_stop_END (3)
#define PMIC_LDO_BUFF_SCP_CTRL_sw1_ocp_auto_stop_START (4)
#define PMIC_LDO_BUFF_SCP_CTRL_sw1_ocp_auto_stop_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck40_scp_auto_stop : 2;
        unsigned char buck3_scp_auto_stop : 2;
        unsigned char buck2_scp_auto_stop : 2;
        unsigned char buck1_scp_auto_stop : 2;
    } reg;
} PMIC_BUCK1_40_SCP_CTRL_UNION;
#endif
#define PMIC_BUCK1_40_SCP_CTRL_buck40_scp_auto_stop_START (0)
#define PMIC_BUCK1_40_SCP_CTRL_buck40_scp_auto_stop_END (1)
#define PMIC_BUCK1_40_SCP_CTRL_buck3_scp_auto_stop_START (2)
#define PMIC_BUCK1_40_SCP_CTRL_buck3_scp_auto_stop_END (3)
#define PMIC_BUCK1_40_SCP_CTRL_buck2_scp_auto_stop_START (4)
#define PMIC_BUCK1_40_SCP_CTRL_buck2_scp_auto_stop_END (5)
#define PMIC_BUCK1_40_SCP_CTRL_buck1_scp_auto_stop_START (6)
#define PMIC_BUCK1_40_SCP_CTRL_buck1_scp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck71_scp_auto_stop : 2;
        unsigned char buck70_scp_auto_stop : 2;
        unsigned char buck5_scp_auto_stop : 2;
        unsigned char buck42_scp_auto_stop : 2;
    } reg;
} PMIC_BUCK42_71_SCP_CTRL_UNION;
#endif
#define PMIC_BUCK42_71_SCP_CTRL_buck71_scp_auto_stop_START (0)
#define PMIC_BUCK42_71_SCP_CTRL_buck71_scp_auto_stop_END (1)
#define PMIC_BUCK42_71_SCP_CTRL_buck70_scp_auto_stop_START (2)
#define PMIC_BUCK42_71_SCP_CTRL_buck70_scp_auto_stop_END (3)
#define PMIC_BUCK42_71_SCP_CTRL_buck5_scp_auto_stop_START (4)
#define PMIC_BUCK42_71_SCP_CTRL_buck5_scp_auto_stop_END (5)
#define PMIC_BUCK42_71_SCP_CTRL_buck42_scp_auto_stop_START (6)
#define PMIC_BUCK42_71_SCP_CTRL_buck42_scp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck9_scp_auto_stop : 2;
        unsigned char ldo_sink_scp_auto_stop : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK9_SCP_CTRL_UNION;
#endif
#define PMIC_BUCK9_SCP_CTRL_buck9_scp_auto_stop_START (0)
#define PMIC_BUCK9_SCP_CTRL_buck9_scp_auto_stop_END (1)
#define PMIC_BUCK9_SCP_CTRL_ldo_sink_scp_auto_stop_START (2)
#define PMIC_BUCK9_SCP_CTRL_ldo_sink_scp_auto_stop_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck71_ovp_auto_stop : 2;
        unsigned char buck70_ovp_auto_stop : 2;
        unsigned char buck42_ovp_auto_stop : 2;
        unsigned char buck40_ovp_auto_stop : 2;
    } reg;
} PMIC_BUCK40_71_OVP_CTRL_UNION;
#endif
#define PMIC_BUCK40_71_OVP_CTRL_buck71_ovp_auto_stop_START (0)
#define PMIC_BUCK40_71_OVP_CTRL_buck71_ovp_auto_stop_END (1)
#define PMIC_BUCK40_71_OVP_CTRL_buck70_ovp_auto_stop_START (2)
#define PMIC_BUCK40_71_OVP_CTRL_buck70_ovp_auto_stop_END (3)
#define PMIC_BUCK40_71_OVP_CTRL_buck42_ovp_auto_stop_START (4)
#define PMIC_BUCK40_71_OVP_CTRL_buck42_ovp_auto_stop_END (5)
#define PMIC_BUCK40_71_OVP_CTRL_buck40_ovp_auto_stop_START (6)
#define PMIC_BUCK40_71_OVP_CTRL_buck40_ovp_auto_stop_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck40_ocp_sel : 1;
        unsigned char buck70_ocp_sel : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_BUCK40_70_OCP_CTRL_UNION;
#endif
#define PMIC_BUCK40_70_OCP_CTRL_buck40_ocp_sel_START (0)
#define PMIC_BUCK40_70_OCP_CTRL_buck40_ocp_sel_END (0)
#define PMIC_BUCK40_70_OCP_CTRL_buck70_ocp_sel_START (1)
#define PMIC_BUCK40_70_OCP_CTRL_buck70_ocp_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_ctrl_reserve : 8;
    } reg;
} PMIC_SYS_CTRL_RESERVE_UNION;
#endif
#define PMIC_SYS_CTRL_RESERVE_sys_ctrl_reserve_START (0)
#define PMIC_SYS_CTRL_RESERVE_sys_ctrl_reserve_END (7)
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
} PMIC_OCP_DEB_CTRL0_UNION;
#endif
#define PMIC_OCP_DEB_CTRL0_ldo_ocp_deb_sel_START (0)
#define PMIC_OCP_DEB_CTRL0_ldo_ocp_deb_sel_END (1)
#define PMIC_OCP_DEB_CTRL0_buck_scp_deb_sel_START (2)
#define PMIC_OCP_DEB_CTRL0_buck_scp_deb_sel_END (3)
#define PMIC_OCP_DEB_CTRL0_buck_ocp_deb_sel_START (4)
#define PMIC_OCP_DEB_CTRL0_buck_ocp_deb_sel_END (5)
#define PMIC_OCP_DEB_CTRL0_buck_ovp_deb_sel_START (6)
#define PMIC_OCP_DEB_CTRL0_buck_ovp_deb_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char vin_ldoh_deb_sel : 2;
        unsigned char ldo_buff_scp_deb_sel : 2;
        unsigned char acr_ocp_deb_sel : 2;
        unsigned char buck_cur_det_deb_sel : 2;
    } reg;
} PMIC_OCP_DEB_CTRL1_UNION;
#endif
#define PMIC_OCP_DEB_CTRL1_vin_ldoh_deb_sel_START (0)
#define PMIC_OCP_DEB_CTRL1_vin_ldoh_deb_sel_END (1)
#define PMIC_OCP_DEB_CTRL1_ldo_buff_scp_deb_sel_START (2)
#define PMIC_OCP_DEB_CTRL1_ldo_buff_scp_deb_sel_END (3)
#define PMIC_OCP_DEB_CTRL1_acr_ocp_deb_sel_START (4)
#define PMIC_OCP_DEB_CTRL1_acr_ocp_deb_sel_END (5)
#define PMIC_OCP_DEB_CTRL1_buck_cur_det_deb_sel_START (6)
#define PMIC_OCP_DEB_CTRL1_buck_cur_det_deb_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char vsys_pwroff_deb_sel : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_PWROFF_DEB_CTRL_UNION;
#endif
#define PMIC_PWROFF_DEB_CTRL_vsys_pwroff_deb_sel_START (0)
#define PMIC_PWROFF_DEB_CTRL_vsys_pwroff_deb_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char vsys_drop_deb_sel : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_VSYS_DROP_DEB_CTRL_UNION;
#endif
#define PMIC_VSYS_DROP_DEB_CTRL_vsys_drop_deb_sel_START (0)
#define PMIC_VSYS_DROP_DEB_CTRL_vsys_drop_deb_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char npu_cur_del_r : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NPU_CUR_DEL_CTRL0_UNION;
#endif
#define PMIC_NPU_CUR_DEL_CTRL0_npu_cur_del_r_START (0)
#define PMIC_NPU_CUR_DEL_CTRL0_npu_cur_del_r_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char npu_cur_del_f : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NPU_CUR_DEL_CTRL1_UNION;
#endif
#define PMIC_NPU_CUR_DEL_CTRL1_npu_cur_del_f_START (0)
#define PMIC_NPU_CUR_DEL_CTRL1_npu_cur_del_f_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char en_vin_ldoh_deb : 1;
        unsigned char en_ldo_buff_scp_deb : 1;
        unsigned char en_ldo_ocp_deb : 1;
        unsigned char en_buck_scp_deb : 1;
        unsigned char en_buck_ocp_deb : 1;
        unsigned char en_acr_ocp_deb : 1;
        unsigned char en_buck_ovp_deb : 1;
        unsigned char en_buck_cur_det_deb : 1;
    } reg;
} PMIC_OCP_SCP_ONOFF_UNION;
#endif
#define PMIC_OCP_SCP_ONOFF_en_vin_ldoh_deb_START (0)
#define PMIC_OCP_SCP_ONOFF_en_vin_ldoh_deb_END (0)
#define PMIC_OCP_SCP_ONOFF_en_ldo_buff_scp_deb_START (1)
#define PMIC_OCP_SCP_ONOFF_en_ldo_buff_scp_deb_END (1)
#define PMIC_OCP_SCP_ONOFF_en_ldo_ocp_deb_START (2)
#define PMIC_OCP_SCP_ONOFF_en_ldo_ocp_deb_END (2)
#define PMIC_OCP_SCP_ONOFF_en_buck_scp_deb_START (3)
#define PMIC_OCP_SCP_ONOFF_en_buck_scp_deb_END (3)
#define PMIC_OCP_SCP_ONOFF_en_buck_ocp_deb_START (4)
#define PMIC_OCP_SCP_ONOFF_en_buck_ocp_deb_END (4)
#define PMIC_OCP_SCP_ONOFF_en_acr_ocp_deb_START (5)
#define PMIC_OCP_SCP_ONOFF_en_acr_ocp_deb_END (5)
#define PMIC_OCP_SCP_ONOFF_en_buck_ovp_deb_START (6)
#define PMIC_OCP_SCP_ONOFF_en_buck_ovp_deb_END (6)
#define PMIC_OCP_SCP_ONOFF_en_buck_cur_det_deb_START (7)
#define PMIC_OCP_SCP_ONOFF_en_buck_cur_det_deb_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_serdes_drv : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_CLK_SERDES_CTRL0_UNION;
#endif
#define PMIC_CLK_SERDES_CTRL0_xo_serdes_drv_START (0)
#define PMIC_CLK_SERDES_CTRL0_xo_serdes_drv_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_wifi_drv : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_CLK_WIFI_CTRL0_UNION;
#endif
#define PMIC_CLK_WIFI_CTRL0_xo_wifi_drv_START (0)
#define PMIC_CLK_WIFI_CTRL0_xo_wifi_drv_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_wifi_delay_sel : 2;
        unsigned char xo_wifi_freq_sel : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_CLK_WIFI_CTRL1_UNION;
#endif
#define PMIC_CLK_WIFI_CTRL1_xo_wifi_delay_sel_START (0)
#define PMIC_CLK_WIFI_CTRL1_xo_wifi_delay_sel_END (1)
#define PMIC_CLK_WIFI_CTRL1_xo_wifi_freq_sel_START (2)
#define PMIC_CLK_WIFI_CTRL1_xo_wifi_freq_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_nfc_drv : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_CLK_NFC_CTRL0_UNION;
#endif
#define PMIC_CLK_NFC_CTRL0_xo_nfc_drv_START (0)
#define PMIC_CLK_NFC_CTRL0_xo_nfc_drv_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_nfc_delay_sel : 2;
        unsigned char xo_nfc_freq_sel : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_CLK_NFC_CTRL1_UNION;
#endif
#define PMIC_CLK_NFC_CTRL1_xo_nfc_delay_sel_START (0)
#define PMIC_CLK_NFC_CTRL1_xo_nfc_delay_sel_END (1)
#define PMIC_CLK_NFC_CTRL1_xo_nfc_freq_sel_START (2)
#define PMIC_CLK_NFC_CTRL1_xo_nfc_freq_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_rf0_drv : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_CLK_RF0_CTRL0_UNION;
#endif
#define PMIC_CLK_RF0_CTRL0_xo_rf0_drv_START (0)
#define PMIC_CLK_RF0_CTRL0_xo_rf0_drv_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_rf0_delay_sel : 2;
        unsigned char xo_rf0_freq_sel : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_CLK_RF0_CTRL1_UNION;
#endif
#define PMIC_CLK_RF0_CTRL1_xo_rf0_delay_sel_START (0)
#define PMIC_CLK_RF0_CTRL1_xo_rf0_delay_sel_END (1)
#define PMIC_CLK_RF0_CTRL1_xo_rf0_freq_sel_START (2)
#define PMIC_CLK_RF0_CTRL1_xo_rf0_freq_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_rf1_drv : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_CLK_RF1_CTRL0_UNION;
#endif
#define PMIC_CLK_RF1_CTRL0_xo_rf1_drv_START (0)
#define PMIC_CLK_RF1_CTRL0_xo_rf1_drv_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_rf1_delay_sel : 2;
        unsigned char xo_rf1_freq_sel : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_CLK_RF1_CTRL1_UNION;
#endif
#define PMIC_CLK_RF1_CTRL1_xo_rf1_delay_sel_START (0)
#define PMIC_CLK_RF1_CTRL1_xo_rf1_delay_sel_END (1)
#define PMIC_CLK_RF1_CTRL1_xo_rf1_freq_sel_START (2)
#define PMIC_CLK_RF1_CTRL1_xo_rf1_freq_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_sys_drv : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_CLK_SYS_CTRL_UNION;
#endif
#define PMIC_CLK_SYS_CTRL_xo_sys_drv_START (0)
#define PMIC_CLK_SYS_CTRL_xo_sys_drv_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_codec_drv : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_CLK_CODEC_CTRL0_UNION;
#endif
#define PMIC_CLK_CODEC_CTRL0_xo_codec_drv_START (0)
#define PMIC_CLK_CODEC_CTRL0_xo_codec_drv_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_codec_delay_sel : 2;
        unsigned char xo_codec_freq_sel : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_CLK_CODEC_CTRL1_UNION;
#endif
#define PMIC_CLK_CODEC_CTRL1_xo_codec_delay_sel_START (0)
#define PMIC_CLK_CODEC_CTRL1_xo_codec_delay_sel_END (1)
#define PMIC_CLK_CODEC_CTRL1_xo_codec_freq_sel_START (2)
#define PMIC_CLK_CODEC_CTRL1_xo_codec_freq_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_ufs_drv : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_CLK_UFS_CTRL_UNION;
#endif
#define PMIC_CLK_UFS_CTRL_xo_ufs_drv_START (0)
#define PMIC_CLK_UFS_CTRL_xo_ufs_drv_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_eusb_drv : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_CLK_EUSB_CTRL_UNION;
#endif
#define PMIC_CLK_EUSB_CTRL_xo_eusb_drv_START (0)
#define PMIC_CLK_EUSB_CTRL_xo_eusb_drv_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_ton_eco_shield : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_TON_ECO_SHIELD_CTRL_UNION;
#endif
#define PMIC_TON_ECO_SHIELD_CTRL_xo_ton_eco_shield_START (0)
#define PMIC_TON_ECO_SHIELD_CTRL_xo_ton_eco_shield_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_rf1_ph_sel : 1;
        unsigned char xo_tri_cap : 1;
        unsigned char reserved : 2;
        unsigned char xo_core_curr_sel : 2;
        unsigned char xo_eco_force_en : 1;
        unsigned char xo_eco_en_shield : 1;
    } reg;
} PMIC_CLK_TOP_CTRL0_UNION;
#endif
#define PMIC_CLK_TOP_CTRL0_xo_rf1_ph_sel_START (0)
#define PMIC_CLK_TOP_CTRL0_xo_rf1_ph_sel_END (0)
#define PMIC_CLK_TOP_CTRL0_xo_tri_cap_START (1)
#define PMIC_CLK_TOP_CTRL0_xo_tri_cap_END (1)
#define PMIC_CLK_TOP_CTRL0_xo_core_curr_sel_START (4)
#define PMIC_CLK_TOP_CTRL0_xo_core_curr_sel_END (5)
#define PMIC_CLK_TOP_CTRL0_xo_eco_force_en_START (6)
#define PMIC_CLK_TOP_CTRL0_xo_eco_force_en_END (6)
#define PMIC_CLK_TOP_CTRL0_xo_eco_en_shield_START (7)
#define PMIC_CLK_TOP_CTRL0_xo_eco_en_shield_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_reserve : 8;
    } reg;
} PMIC_CLK_TOP_CTRL1_UNION;
#endif
#define PMIC_CLK_TOP_CTRL1_xo_reserve_START (0)
#define PMIC_CLK_TOP_CTRL1_xo_reserve_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_CLK_TOP_CTRL2_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char thsd_tmp_set : 2;
        unsigned char reg_thsd_en : 1;
        unsigned char ref_out_sel : 2;
        unsigned char ref_ibias_trim_en : 2;
        unsigned char reserved : 1;
    } reg;
} PMIC_BG_THSD_CTRL0_UNION;
#endif
#define PMIC_BG_THSD_CTRL0_thsd_tmp_set_START (0)
#define PMIC_BG_THSD_CTRL0_thsd_tmp_set_END (1)
#define PMIC_BG_THSD_CTRL0_reg_thsd_en_START (2)
#define PMIC_BG_THSD_CTRL0_reg_thsd_en_END (2)
#define PMIC_BG_THSD_CTRL0_ref_out_sel_START (3)
#define PMIC_BG_THSD_CTRL0_ref_out_sel_END (4)
#define PMIC_BG_THSD_CTRL0_ref_ibias_trim_en_START (5)
#define PMIC_BG_THSD_CTRL0_ref_ibias_trim_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ref_res_sel : 1;
        unsigned char ref_chop_clk_sel : 2;
        unsigned char ref_chop_sel : 1;
        unsigned char ref_ibias_sel : 1;
        unsigned char ref_eco_shield : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BG_THSD_CTRL1_UNION;
#endif
#define PMIC_BG_THSD_CTRL1_ref_res_sel_START (0)
#define PMIC_BG_THSD_CTRL1_ref_res_sel_END (0)
#define PMIC_BG_THSD_CTRL1_ref_chop_clk_sel_START (1)
#define PMIC_BG_THSD_CTRL1_ref_chop_clk_sel_END (2)
#define PMIC_BG_THSD_CTRL1_ref_chop_sel_START (3)
#define PMIC_BG_THSD_CTRL1_ref_chop_sel_END (3)
#define PMIC_BG_THSD_CTRL1_ref_ibias_sel_START (4)
#define PMIC_BG_THSD_CTRL1_ref_ibias_sel_END (4)
#define PMIC_BG_THSD_CTRL1_ref_eco_shield_START (5)
#define PMIC_BG_THSD_CTRL1_ref_eco_shield_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ref_reserve1 : 8;
    } reg;
} PMIC_BG_TEST_UNION;
#endif
#define PMIC_BG_TEST_ref_reserve1_START (0)
#define PMIC_BG_TEST_ref_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ref_reserve2 : 8;
    } reg;
} PMIC_REF_RESERVE2_UNION;
#endif
#define PMIC_REF_RESERVE2_ref_reserve2_START (0)
#define PMIC_REF_RESERVE2_ref_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char tsensor_en : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_TSENSOR_CTRL0_UNION;
#endif
#define PMIC_TSENSOR_CTRL0_tsensor_en_START (0)
#define PMIC_TSENSOR_CTRL0_tsensor_en_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_TSENSOR_RESERVE0_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_TSENSOR_RESERVE1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_TSENSOR_RESERVE2_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_TSENSOR_RESERVE3_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo26_hd_mask : 1;
        unsigned char reg_ldo_buf_hd_mask : 1;
        unsigned char reg_eco_in_hd_mask : 1;
        unsigned char reg_sys_clk_hd_mask : 1;
        unsigned char reg_serdes_clk_hd_mask : 1;
        unsigned char reg_wifi_clk_hd_mask : 1;
        unsigned char reg_nfc_clk_hd_mask : 1;
        unsigned char reg_eusb_clk_hd_mask : 1;
    } reg;
} PMIC_HARDWIRE_CTRL0_UNION;
#endif
#define PMIC_HARDWIRE_CTRL0_reg_ldo26_hd_mask_START (0)
#define PMIC_HARDWIRE_CTRL0_reg_ldo26_hd_mask_END (0)
#define PMIC_HARDWIRE_CTRL0_reg_ldo_buf_hd_mask_START (1)
#define PMIC_HARDWIRE_CTRL0_reg_ldo_buf_hd_mask_END (1)
#define PMIC_HARDWIRE_CTRL0_reg_eco_in_hd_mask_START (2)
#define PMIC_HARDWIRE_CTRL0_reg_eco_in_hd_mask_END (2)
#define PMIC_HARDWIRE_CTRL0_reg_sys_clk_hd_mask_START (3)
#define PMIC_HARDWIRE_CTRL0_reg_sys_clk_hd_mask_END (3)
#define PMIC_HARDWIRE_CTRL0_reg_serdes_clk_hd_mask_START (4)
#define PMIC_HARDWIRE_CTRL0_reg_serdes_clk_hd_mask_END (4)
#define PMIC_HARDWIRE_CTRL0_reg_wifi_clk_hd_mask_START (5)
#define PMIC_HARDWIRE_CTRL0_reg_wifi_clk_hd_mask_END (5)
#define PMIC_HARDWIRE_CTRL0_reg_nfc_clk_hd_mask_START (6)
#define PMIC_HARDWIRE_CTRL0_reg_nfc_clk_hd_mask_END (6)
#define PMIC_HARDWIRE_CTRL0_reg_eusb_clk_hd_mask_START (7)
#define PMIC_HARDWIRE_CTRL0_reg_eusb_clk_hd_mask_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_idle_hd_mask : 1;
        unsigned char reg_eco_in_ao_mask : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_HARDWIRE_CTRL1_UNION;
#endif
#define PMIC_HARDWIRE_CTRL1_reg_idle_hd_mask_START (0)
#define PMIC_HARDWIRE_CTRL1_reg_idle_hd_mask_END (0)
#define PMIC_HARDWIRE_CTRL1_reg_eco_in_ao_mask_START (1)
#define PMIC_HARDWIRE_CTRL1_reg_eco_in_ao_mask_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pmu_idle_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_LOW_POWER_CFG_UNION;
#endif
#define PMIC_LOW_POWER_CFG_pmu_idle_en_START (0)
#define PMIC_LOW_POWER_CFG_pmu_idle_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_buck71_on : 1;
        unsigned char peri_en_buck70_on : 1;
        unsigned char peri_en_buck5_on : 1;
        unsigned char peri_en_buck42_on : 1;
        unsigned char peri_en_buck40_on : 1;
        unsigned char peri_en_buck3_on : 1;
        unsigned char peri_en_buck2_on : 1;
        unsigned char peri_en_buck1_on : 1;
    } reg;
} PMIC_PERI_CTRL0_UNION;
#endif
#define PMIC_PERI_CTRL0_peri_en_buck71_on_START (0)
#define PMIC_PERI_CTRL0_peri_en_buck71_on_END (0)
#define PMIC_PERI_CTRL0_peri_en_buck70_on_START (1)
#define PMIC_PERI_CTRL0_peri_en_buck70_on_END (1)
#define PMIC_PERI_CTRL0_peri_en_buck5_on_START (2)
#define PMIC_PERI_CTRL0_peri_en_buck5_on_END (2)
#define PMIC_PERI_CTRL0_peri_en_buck42_on_START (3)
#define PMIC_PERI_CTRL0_peri_en_buck42_on_END (3)
#define PMIC_PERI_CTRL0_peri_en_buck40_on_START (4)
#define PMIC_PERI_CTRL0_peri_en_buck40_on_END (4)
#define PMIC_PERI_CTRL0_peri_en_buck3_on_START (5)
#define PMIC_PERI_CTRL0_peri_en_buck3_on_END (5)
#define PMIC_PERI_CTRL0_peri_en_buck2_on_START (6)
#define PMIC_PERI_CTRL0_peri_en_buck2_on_END (6)
#define PMIC_PERI_CTRL0_peri_en_buck1_on_START (7)
#define PMIC_PERI_CTRL0_peri_en_buck1_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_ldo18_on : 1;
        unsigned char peri_en_ldo17_on : 1;
        unsigned char peri_en_ldo15_on : 1;
        unsigned char peri_en_ldo8_on : 1;
        unsigned char peri_en_ldo6_on : 1;
        unsigned char peri_en_ldo4_on : 1;
        unsigned char peri_en_ldo0_on : 1;
        unsigned char peri_en_buck9_on : 1;
    } reg;
} PMIC_PERI_CTRL1_UNION;
#endif
#define PMIC_PERI_CTRL1_peri_en_ldo18_on_START (0)
#define PMIC_PERI_CTRL1_peri_en_ldo18_on_END (0)
#define PMIC_PERI_CTRL1_peri_en_ldo17_on_START (1)
#define PMIC_PERI_CTRL1_peri_en_ldo17_on_END (1)
#define PMIC_PERI_CTRL1_peri_en_ldo15_on_START (2)
#define PMIC_PERI_CTRL1_peri_en_ldo15_on_END (2)
#define PMIC_PERI_CTRL1_peri_en_ldo8_on_START (3)
#define PMIC_PERI_CTRL1_peri_en_ldo8_on_END (3)
#define PMIC_PERI_CTRL1_peri_en_ldo6_on_START (4)
#define PMIC_PERI_CTRL1_peri_en_ldo6_on_END (4)
#define PMIC_PERI_CTRL1_peri_en_ldo4_on_START (5)
#define PMIC_PERI_CTRL1_peri_en_ldo4_on_END (5)
#define PMIC_PERI_CTRL1_peri_en_ldo0_on_START (6)
#define PMIC_PERI_CTRL1_peri_en_ldo0_on_END (6)
#define PMIC_PERI_CTRL1_peri_en_buck9_on_START (7)
#define PMIC_PERI_CTRL1_peri_en_buck9_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_ldo36_on : 1;
        unsigned char peri_en_pmuh_on : 1;
        unsigned char peri_en_ldo32_on : 1;
        unsigned char peri_en_ldo30_on : 1;
        unsigned char peri_en_ldo28_on : 1;
        unsigned char peri_en_ldo27_on : 1;
        unsigned char peri_en_ldo24_on : 1;
        unsigned char peri_en_ldo23_on : 1;
    } reg;
} PMIC_PERI_CTRL2_UNION;
#endif
#define PMIC_PERI_CTRL2_peri_en_ldo36_on_START (0)
#define PMIC_PERI_CTRL2_peri_en_ldo36_on_END (0)
#define PMIC_PERI_CTRL2_peri_en_pmuh_on_START (1)
#define PMIC_PERI_CTRL2_peri_en_pmuh_on_END (1)
#define PMIC_PERI_CTRL2_peri_en_ldo32_on_START (2)
#define PMIC_PERI_CTRL2_peri_en_ldo32_on_END (2)
#define PMIC_PERI_CTRL2_peri_en_ldo30_on_START (3)
#define PMIC_PERI_CTRL2_peri_en_ldo30_on_END (3)
#define PMIC_PERI_CTRL2_peri_en_ldo28_on_START (4)
#define PMIC_PERI_CTRL2_peri_en_ldo28_on_END (4)
#define PMIC_PERI_CTRL2_peri_en_ldo27_on_START (5)
#define PMIC_PERI_CTRL2_peri_en_ldo27_on_END (5)
#define PMIC_PERI_CTRL2_peri_en_ldo24_on_START (6)
#define PMIC_PERI_CTRL2_peri_en_ldo24_on_END (6)
#define PMIC_PERI_CTRL2_peri_en_ldo23_on_START (7)
#define PMIC_PERI_CTRL2_peri_en_ldo23_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_ldo44_on : 1;
        unsigned char peri_en_ldo43_on : 1;
        unsigned char peri_en_ldo42_on : 1;
        unsigned char peri_en_ldo41_on : 1;
        unsigned char reserved : 1;
        unsigned char peri_en_ldo39_on : 1;
        unsigned char peri_en_ldo38_on : 1;
        unsigned char peri_en_ldo37_on : 1;
    } reg;
} PMIC_PERI_CTRL3_UNION;
#endif
#define PMIC_PERI_CTRL3_peri_en_ldo44_on_START (0)
#define PMIC_PERI_CTRL3_peri_en_ldo44_on_END (0)
#define PMIC_PERI_CTRL3_peri_en_ldo43_on_START (1)
#define PMIC_PERI_CTRL3_peri_en_ldo43_on_END (1)
#define PMIC_PERI_CTRL3_peri_en_ldo42_on_START (2)
#define PMIC_PERI_CTRL3_peri_en_ldo42_on_END (2)
#define PMIC_PERI_CTRL3_peri_en_ldo41_on_START (3)
#define PMIC_PERI_CTRL3_peri_en_ldo41_on_END (3)
#define PMIC_PERI_CTRL3_peri_en_ldo39_on_START (5)
#define PMIC_PERI_CTRL3_peri_en_ldo39_on_END (5)
#define PMIC_PERI_CTRL3_peri_en_ldo38_on_START (6)
#define PMIC_PERI_CTRL3_peri_en_ldo38_on_END (6)
#define PMIC_PERI_CTRL3_peri_en_ldo37_on_START (7)
#define PMIC_PERI_CTRL3_peri_en_ldo37_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_ldo45_on : 1;
        unsigned char peri_en_ldo46_on : 1;
        unsigned char peri_en_sw1_on : 1;
        unsigned char peri_en_ref_on : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_PERI_CTRL4_UNION;
#endif
#define PMIC_PERI_CTRL4_peri_en_ldo45_on_START (0)
#define PMIC_PERI_CTRL4_peri_en_ldo45_on_END (0)
#define PMIC_PERI_CTRL4_peri_en_ldo46_on_START (1)
#define PMIC_PERI_CTRL4_peri_en_ldo46_on_END (1)
#define PMIC_PERI_CTRL4_peri_en_sw1_on_START (2)
#define PMIC_PERI_CTRL4_peri_en_sw1_on_END (2)
#define PMIC_PERI_CTRL4_peri_en_ref_on_START (3)
#define PMIC_PERI_CTRL4_peri_en_ref_on_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_buck71_eco : 1;
        unsigned char peri_en_buck70_eco : 1;
        unsigned char peri_en_buck5_eco : 1;
        unsigned char peri_en_buck42_eco : 1;
        unsigned char peri_en_buck40_eco : 1;
        unsigned char peri_en_buck3_eco : 1;
        unsigned char peri_en_buck2_eco : 1;
        unsigned char peri_en_buck1_eco : 1;
    } reg;
} PMIC_PERI_CTRL5_UNION;
#endif
#define PMIC_PERI_CTRL5_peri_en_buck71_eco_START (0)
#define PMIC_PERI_CTRL5_peri_en_buck71_eco_END (0)
#define PMIC_PERI_CTRL5_peri_en_buck70_eco_START (1)
#define PMIC_PERI_CTRL5_peri_en_buck70_eco_END (1)
#define PMIC_PERI_CTRL5_peri_en_buck5_eco_START (2)
#define PMIC_PERI_CTRL5_peri_en_buck5_eco_END (2)
#define PMIC_PERI_CTRL5_peri_en_buck42_eco_START (3)
#define PMIC_PERI_CTRL5_peri_en_buck42_eco_END (3)
#define PMIC_PERI_CTRL5_peri_en_buck40_eco_START (4)
#define PMIC_PERI_CTRL5_peri_en_buck40_eco_END (4)
#define PMIC_PERI_CTRL5_peri_en_buck3_eco_START (5)
#define PMIC_PERI_CTRL5_peri_en_buck3_eco_END (5)
#define PMIC_PERI_CTRL5_peri_en_buck2_eco_START (6)
#define PMIC_PERI_CTRL5_peri_en_buck2_eco_END (6)
#define PMIC_PERI_CTRL5_peri_en_buck1_eco_START (7)
#define PMIC_PERI_CTRL5_peri_en_buck1_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_ldo15_eco : 1;
        unsigned char peri_en_ldo12_eco : 1;
        unsigned char peri_en_ldo11_eco : 1;
        unsigned char peri_en_ldo8_eco : 1;
        unsigned char peri_en_ldo6_eco : 1;
        unsigned char peri_en_ldo4_eco : 1;
        unsigned char peri_en_ldo0_eco : 1;
        unsigned char peri_en_buck9_eco : 1;
    } reg;
} PMIC_PERI_CTRL6_UNION;
#endif
#define PMIC_PERI_CTRL6_peri_en_ldo15_eco_START (0)
#define PMIC_PERI_CTRL6_peri_en_ldo15_eco_END (0)
#define PMIC_PERI_CTRL6_peri_en_ldo12_eco_START (1)
#define PMIC_PERI_CTRL6_peri_en_ldo12_eco_END (1)
#define PMIC_PERI_CTRL6_peri_en_ldo11_eco_START (2)
#define PMIC_PERI_CTRL6_peri_en_ldo11_eco_END (2)
#define PMIC_PERI_CTRL6_peri_en_ldo8_eco_START (3)
#define PMIC_PERI_CTRL6_peri_en_ldo8_eco_END (3)
#define PMIC_PERI_CTRL6_peri_en_ldo6_eco_START (4)
#define PMIC_PERI_CTRL6_peri_en_ldo6_eco_END (4)
#define PMIC_PERI_CTRL6_peri_en_ldo4_eco_START (5)
#define PMIC_PERI_CTRL6_peri_en_ldo4_eco_END (5)
#define PMIC_PERI_CTRL6_peri_en_ldo0_eco_START (6)
#define PMIC_PERI_CTRL6_peri_en_ldo0_eco_END (6)
#define PMIC_PERI_CTRL6_peri_en_buck9_eco_START (7)
#define PMIC_PERI_CTRL6_peri_en_buck9_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_ldo32_eco : 1;
        unsigned char peri_en_ldo30_eco : 1;
        unsigned char peri_en_ldo28_eco : 1;
        unsigned char peri_en_ldo27_eco : 1;
        unsigned char peri_en_ldo24_eco : 1;
        unsigned char peri_en_ldo23_eco : 1;
        unsigned char peri_en_ldo18_eco : 1;
        unsigned char peri_en_ldo17_eco : 1;
    } reg;
} PMIC_PERI_CTRL7_UNION;
#endif
#define PMIC_PERI_CTRL7_peri_en_ldo32_eco_START (0)
#define PMIC_PERI_CTRL7_peri_en_ldo32_eco_END (0)
#define PMIC_PERI_CTRL7_peri_en_ldo30_eco_START (1)
#define PMIC_PERI_CTRL7_peri_en_ldo30_eco_END (1)
#define PMIC_PERI_CTRL7_peri_en_ldo28_eco_START (2)
#define PMIC_PERI_CTRL7_peri_en_ldo28_eco_END (2)
#define PMIC_PERI_CTRL7_peri_en_ldo27_eco_START (3)
#define PMIC_PERI_CTRL7_peri_en_ldo27_eco_END (3)
#define PMIC_PERI_CTRL7_peri_en_ldo24_eco_START (4)
#define PMIC_PERI_CTRL7_peri_en_ldo24_eco_END (4)
#define PMIC_PERI_CTRL7_peri_en_ldo23_eco_START (5)
#define PMIC_PERI_CTRL7_peri_en_ldo23_eco_END (5)
#define PMIC_PERI_CTRL7_peri_en_ldo18_eco_START (6)
#define PMIC_PERI_CTRL7_peri_en_ldo18_eco_END (6)
#define PMIC_PERI_CTRL7_peri_en_ldo17_eco_START (7)
#define PMIC_PERI_CTRL7_peri_en_ldo17_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_ldo43_eco : 1;
        unsigned char peri_en_ldo42_eco : 1;
        unsigned char peri_en_ldo41_eco : 1;
        unsigned char peri_en_ldo39_eco : 1;
        unsigned char peri_en_ldo38_eco : 1;
        unsigned char peri_en_ldo37_eco : 1;
        unsigned char peri_en_ldo36_eco : 1;
        unsigned char peri_en_pmuh_eco : 1;
    } reg;
} PMIC_PERI_CTRL8_UNION;
#endif
#define PMIC_PERI_CTRL8_peri_en_ldo43_eco_START (0)
#define PMIC_PERI_CTRL8_peri_en_ldo43_eco_END (0)
#define PMIC_PERI_CTRL8_peri_en_ldo42_eco_START (1)
#define PMIC_PERI_CTRL8_peri_en_ldo42_eco_END (1)
#define PMIC_PERI_CTRL8_peri_en_ldo41_eco_START (2)
#define PMIC_PERI_CTRL8_peri_en_ldo41_eco_END (2)
#define PMIC_PERI_CTRL8_peri_en_ldo39_eco_START (3)
#define PMIC_PERI_CTRL8_peri_en_ldo39_eco_END (3)
#define PMIC_PERI_CTRL8_peri_en_ldo38_eco_START (4)
#define PMIC_PERI_CTRL8_peri_en_ldo38_eco_END (4)
#define PMIC_PERI_CTRL8_peri_en_ldo37_eco_START (5)
#define PMIC_PERI_CTRL8_peri_en_ldo37_eco_END (5)
#define PMIC_PERI_CTRL8_peri_en_ldo36_eco_START (6)
#define PMIC_PERI_CTRL8_peri_en_ldo36_eco_END (6)
#define PMIC_PERI_CTRL8_peri_en_pmuh_eco_START (7)
#define PMIC_PERI_CTRL8_peri_en_pmuh_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_ldo44_eco : 1;
        unsigned char peri_en_ldo45_eco : 1;
        unsigned char peri_en_ldo46_eco : 1;
        unsigned char peri_en_ldo_sink_eco : 1;
        unsigned char peri_en_ref_eco : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL9_UNION;
#endif
#define PMIC_PERI_CTRL9_peri_en_ldo44_eco_START (0)
#define PMIC_PERI_CTRL9_peri_en_ldo44_eco_END (0)
#define PMIC_PERI_CTRL9_peri_en_ldo45_eco_START (1)
#define PMIC_PERI_CTRL9_peri_en_ldo45_eco_END (1)
#define PMIC_PERI_CTRL9_peri_en_ldo46_eco_START (2)
#define PMIC_PERI_CTRL9_peri_en_ldo46_eco_END (2)
#define PMIC_PERI_CTRL9_peri_en_ldo_sink_eco_START (3)
#define PMIC_PERI_CTRL9_peri_en_ldo_sink_eco_END (3)
#define PMIC_PERI_CTRL9_peri_en_ref_eco_START (4)
#define PMIC_PERI_CTRL9_peri_en_ref_eco_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_cnt_cfg : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CNT_CFG_UNION;
#endif
#define PMIC_PERI_CNT_CFG_peri_cnt_cfg_START (0)
#define PMIC_PERI_CNT_CFG_peri_cnt_cfg_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL10_UNION;
#endif
#define PMIC_PERI_CTRL10_peri_buck1_sel_START (0)
#define PMIC_PERI_CTRL10_peri_buck1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck2_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL11_UNION;
#endif
#define PMIC_PERI_CTRL11_peri_buck2_sel_START (0)
#define PMIC_PERI_CTRL11_peri_buck2_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck3_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL12_UNION;
#endif
#define PMIC_PERI_CTRL12_peri_buck3_sel_START (0)
#define PMIC_PERI_CTRL12_peri_buck3_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck40_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL13_UNION;
#endif
#define PMIC_PERI_CTRL13_peri_buck40_sel_START (0)
#define PMIC_PERI_CTRL13_peri_buck40_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck42_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL14_UNION;
#endif
#define PMIC_PERI_CTRL14_peri_buck42_sel_START (0)
#define PMIC_PERI_CTRL14_peri_buck42_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck5_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL15_UNION;
#endif
#define PMIC_PERI_CTRL15_peri_buck5_sel_START (0)
#define PMIC_PERI_CTRL15_peri_buck5_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck70_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL16_UNION;
#endif
#define PMIC_PERI_CTRL16_peri_buck70_sel_START (0)
#define PMIC_PERI_CTRL16_peri_buck70_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck71_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL17_UNION;
#endif
#define PMIC_PERI_CTRL17_peri_buck71_sel_START (0)
#define PMIC_PERI_CTRL17_peri_buck71_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_buck9_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL18_UNION;
#endif
#define PMIC_PERI_CTRL18_peri_buck9_sel_START (0)
#define PMIC_PERI_CTRL18_peri_buck9_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo0_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL19_UNION;
#endif
#define PMIC_PERI_CTRL19_peri_ldo0_sel_START (0)
#define PMIC_PERI_CTRL19_peri_ldo0_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo4_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL20_UNION;
#endif
#define PMIC_PERI_CTRL20_peri_ldo4_sel_START (0)
#define PMIC_PERI_CTRL20_peri_ldo4_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo6_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL21_UNION;
#endif
#define PMIC_PERI_CTRL21_peri_ldo6_sel_START (0)
#define PMIC_PERI_CTRL21_peri_ldo6_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo8_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL22_UNION;
#endif
#define PMIC_PERI_CTRL22_peri_ldo8_sel_START (0)
#define PMIC_PERI_CTRL22_peri_ldo8_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo15_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL23_UNION;
#endif
#define PMIC_PERI_CTRL23_peri_ldo15_sel_START (0)
#define PMIC_PERI_CTRL23_peri_ldo15_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo17_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL24_UNION;
#endif
#define PMIC_PERI_CTRL24_peri_ldo17_sel_START (0)
#define PMIC_PERI_CTRL24_peri_ldo17_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo18_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL25_UNION;
#endif
#define PMIC_PERI_CTRL25_peri_ldo18_sel_START (0)
#define PMIC_PERI_CTRL25_peri_ldo18_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo23_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL26_UNION;
#endif
#define PMIC_PERI_CTRL26_peri_ldo23_sel_START (0)
#define PMIC_PERI_CTRL26_peri_ldo23_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo24_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL27_UNION;
#endif
#define PMIC_PERI_CTRL27_peri_ldo24_sel_START (0)
#define PMIC_PERI_CTRL27_peri_ldo24_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo27_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL28_UNION;
#endif
#define PMIC_PERI_CTRL28_peri_ldo27_sel_START (0)
#define PMIC_PERI_CTRL28_peri_ldo27_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo28_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL29_UNION;
#endif
#define PMIC_PERI_CTRL29_peri_ldo28_sel_START (0)
#define PMIC_PERI_CTRL29_peri_ldo28_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo30_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL30_UNION;
#endif
#define PMIC_PERI_CTRL30_peri_ldo30_sel_START (0)
#define PMIC_PERI_CTRL30_peri_ldo30_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo32_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL31_UNION;
#endif
#define PMIC_PERI_CTRL31_peri_ldo32_sel_START (0)
#define PMIC_PERI_CTRL31_peri_ldo32_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo36_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL32_UNION;
#endif
#define PMIC_PERI_CTRL32_peri_ldo36_sel_START (0)
#define PMIC_PERI_CTRL32_peri_ldo36_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo37_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL33_UNION;
#endif
#define PMIC_PERI_CTRL33_peri_ldo37_sel_START (0)
#define PMIC_PERI_CTRL33_peri_ldo37_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo38_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL34_UNION;
#endif
#define PMIC_PERI_CTRL34_peri_ldo38_sel_START (0)
#define PMIC_PERI_CTRL34_peri_ldo38_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo39_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL35_UNION;
#endif
#define PMIC_PERI_CTRL35_peri_ldo39_sel_START (0)
#define PMIC_PERI_CTRL35_peri_ldo39_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_PERI_CTRL36_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo41_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL37_UNION;
#endif
#define PMIC_PERI_CTRL37_peri_ldo41_sel_START (0)
#define PMIC_PERI_CTRL37_peri_ldo41_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo42_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL38_UNION;
#endif
#define PMIC_PERI_CTRL38_peri_ldo42_sel_START (0)
#define PMIC_PERI_CTRL38_peri_ldo42_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo43_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL39_UNION;
#endif
#define PMIC_PERI_CTRL39_peri_ldo43_sel_START (0)
#define PMIC_PERI_CTRL39_peri_ldo43_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo44_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL40_UNION;
#endif
#define PMIC_PERI_CTRL40_peri_ldo44_sel_START (0)
#define PMIC_PERI_CTRL40_peri_ldo44_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo45_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL41_UNION;
#endif
#define PMIC_PERI_CTRL41_peri_ldo45_sel_START (0)
#define PMIC_PERI_CTRL41_peri_ldo45_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ldo46_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL42_UNION;
#endif
#define PMIC_PERI_CTRL42_peri_ldo46_sel_START (0)
#define PMIC_PERI_CTRL42_peri_ldo46_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_sw1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL43_UNION;
#endif
#define PMIC_PERI_CTRL43_peri_sw1_sel_START (0)
#define PMIC_PERI_CTRL43_peri_sw1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_pmuh_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL44_UNION;
#endif
#define PMIC_PERI_CTRL44_peri_pmuh_sel_START (0)
#define PMIC_PERI_CTRL44_peri_pmuh_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_ref_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_PERI_CTRL45_UNION;
#endif
#define PMIC_PERI_CTRL45_peri_ref_sel_START (0)
#define PMIC_PERI_CTRL45_peri_ref_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char wifi_en_ldo27_eco : 1;
        unsigned char wifi_en_buck3_eco : 1;
        unsigned char peri_en_thsd_eco : 1;
        unsigned char idle_en_thsd_eco : 1;
        unsigned char soh_thsd_en : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_WIFI_CTRL_UNION;
#endif
#define PMIC_WIFI_CTRL_wifi_en_ldo27_eco_START (0)
#define PMIC_WIFI_CTRL_wifi_en_ldo27_eco_END (0)
#define PMIC_WIFI_CTRL_wifi_en_buck3_eco_START (1)
#define PMIC_WIFI_CTRL_wifi_en_buck3_eco_END (1)
#define PMIC_WIFI_CTRL_peri_en_thsd_eco_START (2)
#define PMIC_WIFI_CTRL_peri_en_thsd_eco_END (2)
#define PMIC_WIFI_CTRL_idle_en_thsd_eco_START (3)
#define PMIC_WIFI_CTRL_idle_en_thsd_eco_END (3)
#define PMIC_WIFI_CTRL_soh_thsd_en_START (4)
#define PMIC_WIFI_CTRL_soh_thsd_en_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char peri_en_buck9_vset : 1;
        unsigned char peri_en_buck5_vset : 1;
        unsigned char peri_en_buck3_vset : 1;
        unsigned char peri_en_buck2_vset : 1;
        unsigned char peri_en_ldo44_vset : 1;
        unsigned char reserved : 1;
        unsigned char peri_en_ldo30_vset : 1;
        unsigned char peri_en_ldo0_vset : 1;
    } reg;
} PMIC_PERI_VSET_CTRL_UNION;
#endif
#define PMIC_PERI_VSET_CTRL_peri_en_buck9_vset_START (0)
#define PMIC_PERI_VSET_CTRL_peri_en_buck9_vset_END (0)
#define PMIC_PERI_VSET_CTRL_peri_en_buck5_vset_START (1)
#define PMIC_PERI_VSET_CTRL_peri_en_buck5_vset_END (1)
#define PMIC_PERI_VSET_CTRL_peri_en_buck3_vset_START (2)
#define PMIC_PERI_VSET_CTRL_peri_en_buck3_vset_END (2)
#define PMIC_PERI_VSET_CTRL_peri_en_buck2_vset_START (3)
#define PMIC_PERI_VSET_CTRL_peri_en_buck2_vset_END (3)
#define PMIC_PERI_VSET_CTRL_peri_en_ldo44_vset_START (4)
#define PMIC_PERI_VSET_CTRL_peri_en_ldo44_vset_END (4)
#define PMIC_PERI_VSET_CTRL_peri_en_ldo30_vset_START (6)
#define PMIC_PERI_VSET_CTRL_peri_en_ldo30_vset_END (6)
#define PMIC_PERI_VSET_CTRL_peri_en_ldo0_vset_START (7)
#define PMIC_PERI_VSET_CTRL_peri_en_ldo0_vset_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_en_buck71_on : 1;
        unsigned char idle_en_buck70_on : 1;
        unsigned char idle_en_buck5_on : 1;
        unsigned char idle_en_buck42_on : 1;
        unsigned char idle_en_buck40_on : 1;
        unsigned char idle_en_buck3_on : 1;
        unsigned char idle_en_buck2_on : 1;
        unsigned char idle_en_buck1_on : 1;
    } reg;
} PMIC_IDLE_CTRL0_UNION;
#endif
#define PMIC_IDLE_CTRL0_idle_en_buck71_on_START (0)
#define PMIC_IDLE_CTRL0_idle_en_buck71_on_END (0)
#define PMIC_IDLE_CTRL0_idle_en_buck70_on_START (1)
#define PMIC_IDLE_CTRL0_idle_en_buck70_on_END (1)
#define PMIC_IDLE_CTRL0_idle_en_buck5_on_START (2)
#define PMIC_IDLE_CTRL0_idle_en_buck5_on_END (2)
#define PMIC_IDLE_CTRL0_idle_en_buck42_on_START (3)
#define PMIC_IDLE_CTRL0_idle_en_buck42_on_END (3)
#define PMIC_IDLE_CTRL0_idle_en_buck40_on_START (4)
#define PMIC_IDLE_CTRL0_idle_en_buck40_on_END (4)
#define PMIC_IDLE_CTRL0_idle_en_buck3_on_START (5)
#define PMIC_IDLE_CTRL0_idle_en_buck3_on_END (5)
#define PMIC_IDLE_CTRL0_idle_en_buck2_on_START (6)
#define PMIC_IDLE_CTRL0_idle_en_buck2_on_END (6)
#define PMIC_IDLE_CTRL0_idle_en_buck1_on_START (7)
#define PMIC_IDLE_CTRL0_idle_en_buck1_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_en_ldo18_on : 1;
        unsigned char idle_en_ldo17_on : 1;
        unsigned char idle_en_ldo15_on : 1;
        unsigned char idle_en_ldo8_on : 1;
        unsigned char idle_en_ldo6_on : 1;
        unsigned char idle_en_ldo4_on : 1;
        unsigned char idle_en_ldo0_on : 1;
        unsigned char idle_en_buck9_on : 1;
    } reg;
} PMIC_IDLE_CTRL1_UNION;
#endif
#define PMIC_IDLE_CTRL1_idle_en_ldo18_on_START (0)
#define PMIC_IDLE_CTRL1_idle_en_ldo18_on_END (0)
#define PMIC_IDLE_CTRL1_idle_en_ldo17_on_START (1)
#define PMIC_IDLE_CTRL1_idle_en_ldo17_on_END (1)
#define PMIC_IDLE_CTRL1_idle_en_ldo15_on_START (2)
#define PMIC_IDLE_CTRL1_idle_en_ldo15_on_END (2)
#define PMIC_IDLE_CTRL1_idle_en_ldo8_on_START (3)
#define PMIC_IDLE_CTRL1_idle_en_ldo8_on_END (3)
#define PMIC_IDLE_CTRL1_idle_en_ldo6_on_START (4)
#define PMIC_IDLE_CTRL1_idle_en_ldo6_on_END (4)
#define PMIC_IDLE_CTRL1_idle_en_ldo4_on_START (5)
#define PMIC_IDLE_CTRL1_idle_en_ldo4_on_END (5)
#define PMIC_IDLE_CTRL1_idle_en_ldo0_on_START (6)
#define PMIC_IDLE_CTRL1_idle_en_ldo0_on_END (6)
#define PMIC_IDLE_CTRL1_idle_en_buck9_on_START (7)
#define PMIC_IDLE_CTRL1_idle_en_buck9_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_en_ldo36_on : 1;
        unsigned char idle_en_pmuh_on : 1;
        unsigned char idle_en_ldo32_on : 1;
        unsigned char idle_en_ldo30_on : 1;
        unsigned char idle_en_ldo28_on : 1;
        unsigned char idle_en_ldo27_on : 1;
        unsigned char idle_en_ldo24_on : 1;
        unsigned char idle_en_ldo23_on : 1;
    } reg;
} PMIC_IDLE_CTRL2_UNION;
#endif
#define PMIC_IDLE_CTRL2_idle_en_ldo36_on_START (0)
#define PMIC_IDLE_CTRL2_idle_en_ldo36_on_END (0)
#define PMIC_IDLE_CTRL2_idle_en_pmuh_on_START (1)
#define PMIC_IDLE_CTRL2_idle_en_pmuh_on_END (1)
#define PMIC_IDLE_CTRL2_idle_en_ldo32_on_START (2)
#define PMIC_IDLE_CTRL2_idle_en_ldo32_on_END (2)
#define PMIC_IDLE_CTRL2_idle_en_ldo30_on_START (3)
#define PMIC_IDLE_CTRL2_idle_en_ldo30_on_END (3)
#define PMIC_IDLE_CTRL2_idle_en_ldo28_on_START (4)
#define PMIC_IDLE_CTRL2_idle_en_ldo28_on_END (4)
#define PMIC_IDLE_CTRL2_idle_en_ldo27_on_START (5)
#define PMIC_IDLE_CTRL2_idle_en_ldo27_on_END (5)
#define PMIC_IDLE_CTRL2_idle_en_ldo24_on_START (6)
#define PMIC_IDLE_CTRL2_idle_en_ldo24_on_END (6)
#define PMIC_IDLE_CTRL2_idle_en_ldo23_on_START (7)
#define PMIC_IDLE_CTRL2_idle_en_ldo23_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_en_ldo44_on : 1;
        unsigned char idle_en_ldo43_on : 1;
        unsigned char idle_en_ldo42_on : 1;
        unsigned char idle_en_ldo41_on : 1;
        unsigned char reserved : 1;
        unsigned char idle_en_ldo39_on : 1;
        unsigned char idle_en_ldo38_on : 1;
        unsigned char idle_en_ldo37_on : 1;
    } reg;
} PMIC_IDLE_CTRL3_UNION;
#endif
#define PMIC_IDLE_CTRL3_idle_en_ldo44_on_START (0)
#define PMIC_IDLE_CTRL3_idle_en_ldo44_on_END (0)
#define PMIC_IDLE_CTRL3_idle_en_ldo43_on_START (1)
#define PMIC_IDLE_CTRL3_idle_en_ldo43_on_END (1)
#define PMIC_IDLE_CTRL3_idle_en_ldo42_on_START (2)
#define PMIC_IDLE_CTRL3_idle_en_ldo42_on_END (2)
#define PMIC_IDLE_CTRL3_idle_en_ldo41_on_START (3)
#define PMIC_IDLE_CTRL3_idle_en_ldo41_on_END (3)
#define PMIC_IDLE_CTRL3_idle_en_ldo39_on_START (5)
#define PMIC_IDLE_CTRL3_idle_en_ldo39_on_END (5)
#define PMIC_IDLE_CTRL3_idle_en_ldo38_on_START (6)
#define PMIC_IDLE_CTRL3_idle_en_ldo38_on_END (6)
#define PMIC_IDLE_CTRL3_idle_en_ldo37_on_START (7)
#define PMIC_IDLE_CTRL3_idle_en_ldo37_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_en_ldo45_on : 1;
        unsigned char idle_en_ldo46_on : 1;
        unsigned char idle_en_sw1_on : 1;
        unsigned char idle_en_ref_on : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_IDLE_CTRL4_UNION;
#endif
#define PMIC_IDLE_CTRL4_idle_en_ldo45_on_START (0)
#define PMIC_IDLE_CTRL4_idle_en_ldo45_on_END (0)
#define PMIC_IDLE_CTRL4_idle_en_ldo46_on_START (1)
#define PMIC_IDLE_CTRL4_idle_en_ldo46_on_END (1)
#define PMIC_IDLE_CTRL4_idle_en_sw1_on_START (2)
#define PMIC_IDLE_CTRL4_idle_en_sw1_on_END (2)
#define PMIC_IDLE_CTRL4_idle_en_ref_on_START (3)
#define PMIC_IDLE_CTRL4_idle_en_ref_on_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_en_buck71_eco : 1;
        unsigned char idle_en_buck70_eco : 1;
        unsigned char idle_en_buck5_eco : 1;
        unsigned char idle_en_buck42_eco : 1;
        unsigned char idle_en_buck40_eco : 1;
        unsigned char idle_en_buck3_eco : 1;
        unsigned char idle_en_buck2_eco : 1;
        unsigned char idle_en_buck1_eco : 1;
    } reg;
} PMIC_IDLE_CTRL5_UNION;
#endif
#define PMIC_IDLE_CTRL5_idle_en_buck71_eco_START (0)
#define PMIC_IDLE_CTRL5_idle_en_buck71_eco_END (0)
#define PMIC_IDLE_CTRL5_idle_en_buck70_eco_START (1)
#define PMIC_IDLE_CTRL5_idle_en_buck70_eco_END (1)
#define PMIC_IDLE_CTRL5_idle_en_buck5_eco_START (2)
#define PMIC_IDLE_CTRL5_idle_en_buck5_eco_END (2)
#define PMIC_IDLE_CTRL5_idle_en_buck42_eco_START (3)
#define PMIC_IDLE_CTRL5_idle_en_buck42_eco_END (3)
#define PMIC_IDLE_CTRL5_idle_en_buck40_eco_START (4)
#define PMIC_IDLE_CTRL5_idle_en_buck40_eco_END (4)
#define PMIC_IDLE_CTRL5_idle_en_buck3_eco_START (5)
#define PMIC_IDLE_CTRL5_idle_en_buck3_eco_END (5)
#define PMIC_IDLE_CTRL5_idle_en_buck2_eco_START (6)
#define PMIC_IDLE_CTRL5_idle_en_buck2_eco_END (6)
#define PMIC_IDLE_CTRL5_idle_en_buck1_eco_START (7)
#define PMIC_IDLE_CTRL5_idle_en_buck1_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_en_ldo15_eco : 1;
        unsigned char idle_en_ldo12_eco : 1;
        unsigned char idle_en_ldo11_eco : 1;
        unsigned char idle_en_ldo8_eco : 1;
        unsigned char idle_en_ldo6_eco : 1;
        unsigned char idle_en_ldo4_eco : 1;
        unsigned char idle_en_ldo0_eco : 1;
        unsigned char idle_en_buck9_eco : 1;
    } reg;
} PMIC_IDLE_CTRL6_UNION;
#endif
#define PMIC_IDLE_CTRL6_idle_en_ldo15_eco_START (0)
#define PMIC_IDLE_CTRL6_idle_en_ldo15_eco_END (0)
#define PMIC_IDLE_CTRL6_idle_en_ldo12_eco_START (1)
#define PMIC_IDLE_CTRL6_idle_en_ldo12_eco_END (1)
#define PMIC_IDLE_CTRL6_idle_en_ldo11_eco_START (2)
#define PMIC_IDLE_CTRL6_idle_en_ldo11_eco_END (2)
#define PMIC_IDLE_CTRL6_idle_en_ldo8_eco_START (3)
#define PMIC_IDLE_CTRL6_idle_en_ldo8_eco_END (3)
#define PMIC_IDLE_CTRL6_idle_en_ldo6_eco_START (4)
#define PMIC_IDLE_CTRL6_idle_en_ldo6_eco_END (4)
#define PMIC_IDLE_CTRL6_idle_en_ldo4_eco_START (5)
#define PMIC_IDLE_CTRL6_idle_en_ldo4_eco_END (5)
#define PMIC_IDLE_CTRL6_idle_en_ldo0_eco_START (6)
#define PMIC_IDLE_CTRL6_idle_en_ldo0_eco_END (6)
#define PMIC_IDLE_CTRL6_idle_en_buck9_eco_START (7)
#define PMIC_IDLE_CTRL6_idle_en_buck9_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_en_pmuh_eco : 1;
        unsigned char idle_en_ldo32_eco : 1;
        unsigned char idle_en_ldo30_eco : 1;
        unsigned char idle_en_ldo27_eco : 1;
        unsigned char idle_en_ldo24_eco : 1;
        unsigned char idle_en_ldo23_eco : 1;
        unsigned char idle_en_ldo18_eco : 1;
        unsigned char idle_en_ldo17_eco : 1;
    } reg;
} PMIC_IDLE_CTRL7_UNION;
#endif
#define PMIC_IDLE_CTRL7_idle_en_pmuh_eco_START (0)
#define PMIC_IDLE_CTRL7_idle_en_pmuh_eco_END (0)
#define PMIC_IDLE_CTRL7_idle_en_ldo32_eco_START (1)
#define PMIC_IDLE_CTRL7_idle_en_ldo32_eco_END (1)
#define PMIC_IDLE_CTRL7_idle_en_ldo30_eco_START (2)
#define PMIC_IDLE_CTRL7_idle_en_ldo30_eco_END (2)
#define PMIC_IDLE_CTRL7_idle_en_ldo27_eco_START (3)
#define PMIC_IDLE_CTRL7_idle_en_ldo27_eco_END (3)
#define PMIC_IDLE_CTRL7_idle_en_ldo24_eco_START (4)
#define PMIC_IDLE_CTRL7_idle_en_ldo24_eco_END (4)
#define PMIC_IDLE_CTRL7_idle_en_ldo23_eco_START (5)
#define PMIC_IDLE_CTRL7_idle_en_ldo23_eco_END (5)
#define PMIC_IDLE_CTRL7_idle_en_ldo18_eco_START (6)
#define PMIC_IDLE_CTRL7_idle_en_ldo18_eco_END (6)
#define PMIC_IDLE_CTRL7_idle_en_ldo17_eco_START (7)
#define PMIC_IDLE_CTRL7_idle_en_ldo17_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_en_ldo43_eco : 1;
        unsigned char idle_en_ldo42_eco : 1;
        unsigned char idle_en_ldo41_eco : 1;
        unsigned char reserved : 1;
        unsigned char idle_en_ldo39_eco : 1;
        unsigned char idle_en_ldo38_eco : 1;
        unsigned char idle_en_ldo37_eco : 1;
        unsigned char idle_en_ldo36_eco : 1;
    } reg;
} PMIC_IDLE_CTRL8_UNION;
#endif
#define PMIC_IDLE_CTRL8_idle_en_ldo43_eco_START (0)
#define PMIC_IDLE_CTRL8_idle_en_ldo43_eco_END (0)
#define PMIC_IDLE_CTRL8_idle_en_ldo42_eco_START (1)
#define PMIC_IDLE_CTRL8_idle_en_ldo42_eco_END (1)
#define PMIC_IDLE_CTRL8_idle_en_ldo41_eco_START (2)
#define PMIC_IDLE_CTRL8_idle_en_ldo41_eco_END (2)
#define PMIC_IDLE_CTRL8_idle_en_ldo39_eco_START (4)
#define PMIC_IDLE_CTRL8_idle_en_ldo39_eco_END (4)
#define PMIC_IDLE_CTRL8_idle_en_ldo38_eco_START (5)
#define PMIC_IDLE_CTRL8_idle_en_ldo38_eco_END (5)
#define PMIC_IDLE_CTRL8_idle_en_ldo37_eco_START (6)
#define PMIC_IDLE_CTRL8_idle_en_ldo37_eco_END (6)
#define PMIC_IDLE_CTRL8_idle_en_ldo36_eco_START (7)
#define PMIC_IDLE_CTRL8_idle_en_ldo36_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_en_ldo44_eco : 1;
        unsigned char idle_en_ldo45_eco : 1;
        unsigned char idle_en_ldo46_eco : 1;
        unsigned char idle_en_ref_eco : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_IDLE_CTRL9_UNION;
#endif
#define PMIC_IDLE_CTRL9_idle_en_ldo44_eco_START (0)
#define PMIC_IDLE_CTRL9_idle_en_ldo44_eco_END (0)
#define PMIC_IDLE_CTRL9_idle_en_ldo45_eco_START (1)
#define PMIC_IDLE_CTRL9_idle_en_ldo45_eco_END (1)
#define PMIC_IDLE_CTRL9_idle_en_ldo46_eco_START (2)
#define PMIC_IDLE_CTRL9_idle_en_ldo46_eco_END (2)
#define PMIC_IDLE_CTRL9_idle_en_ref_eco_START (3)
#define PMIC_IDLE_CTRL9_idle_en_ref_eco_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_cnt_cfg : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CNT_CFG_UNION;
#endif
#define PMIC_IDLE_CNT_CFG_idle_cnt_cfg_START (0)
#define PMIC_IDLE_CNT_CFG_idle_cnt_cfg_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL10_UNION;
#endif
#define PMIC_IDLE_CTRL10_idle_buck1_sel_START (0)
#define PMIC_IDLE_CTRL10_idle_buck1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck2_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL11_UNION;
#endif
#define PMIC_IDLE_CTRL11_idle_buck2_sel_START (0)
#define PMIC_IDLE_CTRL11_idle_buck2_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck3_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL12_UNION;
#endif
#define PMIC_IDLE_CTRL12_idle_buck3_sel_START (0)
#define PMIC_IDLE_CTRL12_idle_buck3_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck40_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL13_UNION;
#endif
#define PMIC_IDLE_CTRL13_idle_buck40_sel_START (0)
#define PMIC_IDLE_CTRL13_idle_buck40_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck42_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL14_UNION;
#endif
#define PMIC_IDLE_CTRL14_idle_buck42_sel_START (0)
#define PMIC_IDLE_CTRL14_idle_buck42_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck5_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL15_UNION;
#endif
#define PMIC_IDLE_CTRL15_idle_buck5_sel_START (0)
#define PMIC_IDLE_CTRL15_idle_buck5_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck70_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL16_UNION;
#endif
#define PMIC_IDLE_CTRL16_idle_buck70_sel_START (0)
#define PMIC_IDLE_CTRL16_idle_buck70_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck71_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL17_UNION;
#endif
#define PMIC_IDLE_CTRL17_idle_buck71_sel_START (0)
#define PMIC_IDLE_CTRL17_idle_buck71_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_buck9_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL18_UNION;
#endif
#define PMIC_IDLE_CTRL18_idle_buck9_sel_START (0)
#define PMIC_IDLE_CTRL18_idle_buck9_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo0_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL19_UNION;
#endif
#define PMIC_IDLE_CTRL19_idle_ldo0_sel_START (0)
#define PMIC_IDLE_CTRL19_idle_ldo0_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo4_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL20_UNION;
#endif
#define PMIC_IDLE_CTRL20_idle_ldo4_sel_START (0)
#define PMIC_IDLE_CTRL20_idle_ldo4_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo6_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL21_UNION;
#endif
#define PMIC_IDLE_CTRL21_idle_ldo6_sel_START (0)
#define PMIC_IDLE_CTRL21_idle_ldo6_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo8_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL22_UNION;
#endif
#define PMIC_IDLE_CTRL22_idle_ldo8_sel_START (0)
#define PMIC_IDLE_CTRL22_idle_ldo8_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo15_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL23_UNION;
#endif
#define PMIC_IDLE_CTRL23_idle_ldo15_sel_START (0)
#define PMIC_IDLE_CTRL23_idle_ldo15_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo17_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL24_UNION;
#endif
#define PMIC_IDLE_CTRL24_idle_ldo17_sel_START (0)
#define PMIC_IDLE_CTRL24_idle_ldo17_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo18_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL25_UNION;
#endif
#define PMIC_IDLE_CTRL25_idle_ldo18_sel_START (0)
#define PMIC_IDLE_CTRL25_idle_ldo18_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo23_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL26_UNION;
#endif
#define PMIC_IDLE_CTRL26_idle_ldo23_sel_START (0)
#define PMIC_IDLE_CTRL26_idle_ldo23_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo24_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL27_UNION;
#endif
#define PMIC_IDLE_CTRL27_idle_ldo24_sel_START (0)
#define PMIC_IDLE_CTRL27_idle_ldo24_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo27_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL28_UNION;
#endif
#define PMIC_IDLE_CTRL28_idle_ldo27_sel_START (0)
#define PMIC_IDLE_CTRL28_idle_ldo27_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo28_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL29_UNION;
#endif
#define PMIC_IDLE_CTRL29_idle_ldo28_sel_START (0)
#define PMIC_IDLE_CTRL29_idle_ldo28_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo30_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL30_UNION;
#endif
#define PMIC_IDLE_CTRL30_idle_ldo30_sel_START (0)
#define PMIC_IDLE_CTRL30_idle_ldo30_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo32_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL31_UNION;
#endif
#define PMIC_IDLE_CTRL31_idle_ldo32_sel_START (0)
#define PMIC_IDLE_CTRL31_idle_ldo32_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo36_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL32_UNION;
#endif
#define PMIC_IDLE_CTRL32_idle_ldo36_sel_START (0)
#define PMIC_IDLE_CTRL32_idle_ldo36_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo37_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL33_UNION;
#endif
#define PMIC_IDLE_CTRL33_idle_ldo37_sel_START (0)
#define PMIC_IDLE_CTRL33_idle_ldo37_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo38_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL34_UNION;
#endif
#define PMIC_IDLE_CTRL34_idle_ldo38_sel_START (0)
#define PMIC_IDLE_CTRL34_idle_ldo38_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo39_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL35_UNION;
#endif
#define PMIC_IDLE_CTRL35_idle_ldo39_sel_START (0)
#define PMIC_IDLE_CTRL35_idle_ldo39_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_IDLE_CTRL36_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo41_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL37_UNION;
#endif
#define PMIC_IDLE_CTRL37_idle_ldo41_sel_START (0)
#define PMIC_IDLE_CTRL37_idle_ldo41_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo42_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL38_UNION;
#endif
#define PMIC_IDLE_CTRL38_idle_ldo42_sel_START (0)
#define PMIC_IDLE_CTRL38_idle_ldo42_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo43_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL39_UNION;
#endif
#define PMIC_IDLE_CTRL39_idle_ldo43_sel_START (0)
#define PMIC_IDLE_CTRL39_idle_ldo43_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo44_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL40_UNION;
#endif
#define PMIC_IDLE_CTRL40_idle_ldo44_sel_START (0)
#define PMIC_IDLE_CTRL40_idle_ldo44_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo45_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL41_UNION;
#endif
#define PMIC_IDLE_CTRL41_idle_ldo45_sel_START (0)
#define PMIC_IDLE_CTRL41_idle_ldo45_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ldo46_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL42_UNION;
#endif
#define PMIC_IDLE_CTRL42_idle_ldo46_sel_START (0)
#define PMIC_IDLE_CTRL42_idle_ldo46_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_sw1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL43_UNION;
#endif
#define PMIC_IDLE_CTRL43_idle_sw1_sel_START (0)
#define PMIC_IDLE_CTRL43_idle_sw1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_pmuh_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL44_UNION;
#endif
#define PMIC_IDLE_CTRL44_idle_pmuh_sel_START (0)
#define PMIC_IDLE_CTRL44_idle_pmuh_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char idle_ref_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_IDLE_CTRL45_UNION;
#endif
#define PMIC_IDLE_CTRL45_idle_ref_sel_START (0)
#define PMIC_IDLE_CTRL45_idle_ref_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo2_on : 1;
        unsigned char sys_en_ldo0_on : 1;
        unsigned char sys_en_buck71_on : 1;
        unsigned char sys_en_buck70_on : 1;
        unsigned char sys_en_buck5_on : 1;
        unsigned char sys_en_buck42_on : 1;
        unsigned char sys_en_buck40_on : 1;
        unsigned char sys_en_buck1_on : 1;
    } reg;
} PMIC_SYS_EN_CTRL0_UNION;
#endif
#define PMIC_SYS_EN_CTRL0_sys_en_ldo2_on_START (0)
#define PMIC_SYS_EN_CTRL0_sys_en_ldo2_on_END (0)
#define PMIC_SYS_EN_CTRL0_sys_en_ldo0_on_START (1)
#define PMIC_SYS_EN_CTRL0_sys_en_ldo0_on_END (1)
#define PMIC_SYS_EN_CTRL0_sys_en_buck71_on_START (2)
#define PMIC_SYS_EN_CTRL0_sys_en_buck71_on_END (2)
#define PMIC_SYS_EN_CTRL0_sys_en_buck70_on_START (3)
#define PMIC_SYS_EN_CTRL0_sys_en_buck70_on_END (3)
#define PMIC_SYS_EN_CTRL0_sys_en_buck5_on_START (4)
#define PMIC_SYS_EN_CTRL0_sys_en_buck5_on_END (4)
#define PMIC_SYS_EN_CTRL0_sys_en_buck42_on_START (5)
#define PMIC_SYS_EN_CTRL0_sys_en_buck42_on_END (5)
#define PMIC_SYS_EN_CTRL0_sys_en_buck40_on_START (6)
#define PMIC_SYS_EN_CTRL0_sys_en_buck40_on_END (6)
#define PMIC_SYS_EN_CTRL0_sys_en_buck1_on_START (7)
#define PMIC_SYS_EN_CTRL0_sys_en_buck1_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo43_on : 1;
        unsigned char reserved : 1;
        unsigned char sys_en_ldo39_on : 1;
        unsigned char sys_en_ldo38_on : 1;
        unsigned char sys_en_ldo37_on : 1;
        unsigned char sys_en_ldo36_on : 1;
        unsigned char sys_en_ldo30_on : 1;
        unsigned char sys_en_ldo6_on : 1;
    } reg;
} PMIC_SYS_EN_CTRL1_UNION;
#endif
#define PMIC_SYS_EN_CTRL1_sys_en_ldo43_on_START (0)
#define PMIC_SYS_EN_CTRL1_sys_en_ldo43_on_END (0)
#define PMIC_SYS_EN_CTRL1_sys_en_ldo39_on_START (2)
#define PMIC_SYS_EN_CTRL1_sys_en_ldo39_on_END (2)
#define PMIC_SYS_EN_CTRL1_sys_en_ldo38_on_START (3)
#define PMIC_SYS_EN_CTRL1_sys_en_ldo38_on_END (3)
#define PMIC_SYS_EN_CTRL1_sys_en_ldo37_on_START (4)
#define PMIC_SYS_EN_CTRL1_sys_en_ldo37_on_END (4)
#define PMIC_SYS_EN_CTRL1_sys_en_ldo36_on_START (5)
#define PMIC_SYS_EN_CTRL1_sys_en_ldo36_on_END (5)
#define PMIC_SYS_EN_CTRL1_sys_en_ldo30_on_START (6)
#define PMIC_SYS_EN_CTRL1_sys_en_ldo30_on_END (6)
#define PMIC_SYS_EN_CTRL1_sys_en_ldo6_on_START (7)
#define PMIC_SYS_EN_CTRL1_sys_en_ldo6_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo44_on : 1;
        unsigned char sys_en_ldo45_on : 1;
        unsigned char sys_en_ldo46_on : 1;
        unsigned char sys_en_sw1_on : 1;
        unsigned char sys_en_ref_on : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL2_UNION;
#endif
#define PMIC_SYS_EN_CTRL2_sys_en_ldo44_on_START (0)
#define PMIC_SYS_EN_CTRL2_sys_en_ldo44_on_END (0)
#define PMIC_SYS_EN_CTRL2_sys_en_ldo45_on_START (1)
#define PMIC_SYS_EN_CTRL2_sys_en_ldo45_on_END (1)
#define PMIC_SYS_EN_CTRL2_sys_en_ldo46_on_START (2)
#define PMIC_SYS_EN_CTRL2_sys_en_ldo46_on_END (2)
#define PMIC_SYS_EN_CTRL2_sys_en_sw1_on_START (3)
#define PMIC_SYS_EN_CTRL2_sys_en_sw1_on_END (3)
#define PMIC_SYS_EN_CTRL2_sys_en_ref_on_START (4)
#define PMIC_SYS_EN_CTRL2_sys_en_ref_on_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo2_eco : 1;
        unsigned char sys_en_ldo0_eco : 1;
        unsigned char sys_en_buck71_eco : 1;
        unsigned char sys_en_buck70_eco : 1;
        unsigned char sys_en_buck5_eco : 1;
        unsigned char sys_en_buck42_eco : 1;
        unsigned char sys_en_buck40_eco : 1;
        unsigned char sys_en_buck1_eco : 1;
    } reg;
} PMIC_SYS_EN_CTRL3_UNION;
#endif
#define PMIC_SYS_EN_CTRL3_sys_en_ldo2_eco_START (0)
#define PMIC_SYS_EN_CTRL3_sys_en_ldo2_eco_END (0)
#define PMIC_SYS_EN_CTRL3_sys_en_ldo0_eco_START (1)
#define PMIC_SYS_EN_CTRL3_sys_en_ldo0_eco_END (1)
#define PMIC_SYS_EN_CTRL3_sys_en_buck71_eco_START (2)
#define PMIC_SYS_EN_CTRL3_sys_en_buck71_eco_END (2)
#define PMIC_SYS_EN_CTRL3_sys_en_buck70_eco_START (3)
#define PMIC_SYS_EN_CTRL3_sys_en_buck70_eco_END (3)
#define PMIC_SYS_EN_CTRL3_sys_en_buck5_eco_START (4)
#define PMIC_SYS_EN_CTRL3_sys_en_buck5_eco_END (4)
#define PMIC_SYS_EN_CTRL3_sys_en_buck42_eco_START (5)
#define PMIC_SYS_EN_CTRL3_sys_en_buck42_eco_END (5)
#define PMIC_SYS_EN_CTRL3_sys_en_buck40_eco_START (6)
#define PMIC_SYS_EN_CTRL3_sys_en_buck40_eco_END (6)
#define PMIC_SYS_EN_CTRL3_sys_en_buck1_eco_START (7)
#define PMIC_SYS_EN_CTRL3_sys_en_buck1_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo43_eco : 1;
        unsigned char reserved : 1;
        unsigned char sys_en_ldo39_eco : 1;
        unsigned char sys_en_ldo38_eco : 1;
        unsigned char sys_en_ldo37_eco : 1;
        unsigned char sys_en_ldo36_eco : 1;
        unsigned char sys_en_ldo30_eco : 1;
        unsigned char sys_en_ldo6_eco : 1;
    } reg;
} PMIC_SYS_EN_CTRL4_UNION;
#endif
#define PMIC_SYS_EN_CTRL4_sys_en_ldo43_eco_START (0)
#define PMIC_SYS_EN_CTRL4_sys_en_ldo43_eco_END (0)
#define PMIC_SYS_EN_CTRL4_sys_en_ldo39_eco_START (2)
#define PMIC_SYS_EN_CTRL4_sys_en_ldo39_eco_END (2)
#define PMIC_SYS_EN_CTRL4_sys_en_ldo38_eco_START (3)
#define PMIC_SYS_EN_CTRL4_sys_en_ldo38_eco_END (3)
#define PMIC_SYS_EN_CTRL4_sys_en_ldo37_eco_START (4)
#define PMIC_SYS_EN_CTRL4_sys_en_ldo37_eco_END (4)
#define PMIC_SYS_EN_CTRL4_sys_en_ldo36_eco_START (5)
#define PMIC_SYS_EN_CTRL4_sys_en_ldo36_eco_END (5)
#define PMIC_SYS_EN_CTRL4_sys_en_ldo30_eco_START (6)
#define PMIC_SYS_EN_CTRL4_sys_en_ldo30_eco_END (6)
#define PMIC_SYS_EN_CTRL4_sys_en_ldo6_eco_START (7)
#define PMIC_SYS_EN_CTRL4_sys_en_ldo6_eco_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo44_eco : 1;
        unsigned char sys_en_ldo45_eco : 1;
        unsigned char sys_en_ldo46_eco : 1;
        unsigned char sys_en_ldo_sink_eco : 1;
        unsigned char sys_en_ref_eco : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL5_UNION;
#endif
#define PMIC_SYS_EN_CTRL5_sys_en_ldo44_eco_START (0)
#define PMIC_SYS_EN_CTRL5_sys_en_ldo44_eco_END (0)
#define PMIC_SYS_EN_CTRL5_sys_en_ldo45_eco_START (1)
#define PMIC_SYS_EN_CTRL5_sys_en_ldo45_eco_END (1)
#define PMIC_SYS_EN_CTRL5_sys_en_ldo46_eco_START (2)
#define PMIC_SYS_EN_CTRL5_sys_en_ldo46_eco_END (2)
#define PMIC_SYS_EN_CTRL5_sys_en_ldo_sink_eco_START (3)
#define PMIC_SYS_EN_CTRL5_sys_en_ldo_sink_eco_END (3)
#define PMIC_SYS_EN_CTRL5_sys_en_ref_eco_START (4)
#define PMIC_SYS_EN_CTRL5_sys_en_ref_eco_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_cnt_cfg : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CNT_CFG_UNION;
#endif
#define PMIC_SYS_EN_CNT_CFG_sys_en_cnt_cfg_START (0)
#define PMIC_SYS_EN_CNT_CFG_sys_en_cnt_cfg_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_buck1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL6_UNION;
#endif
#define PMIC_SYS_EN_CTRL6_sys_en_buck1_sel_START (0)
#define PMIC_SYS_EN_CTRL6_sys_en_buck1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_buck40_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL7_UNION;
#endif
#define PMIC_SYS_EN_CTRL7_sys_en_buck40_sel_START (0)
#define PMIC_SYS_EN_CTRL7_sys_en_buck40_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_buck42_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL8_UNION;
#endif
#define PMIC_SYS_EN_CTRL8_sys_en_buck42_sel_START (0)
#define PMIC_SYS_EN_CTRL8_sys_en_buck42_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_buck5_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL9_UNION;
#endif
#define PMIC_SYS_EN_CTRL9_sys_en_buck5_sel_START (0)
#define PMIC_SYS_EN_CTRL9_sys_en_buck5_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_buck70_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL10_UNION;
#endif
#define PMIC_SYS_EN_CTRL10_sys_en_buck70_sel_START (0)
#define PMIC_SYS_EN_CTRL10_sys_en_buck70_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_buck71_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL11_UNION;
#endif
#define PMIC_SYS_EN_CTRL11_sys_en_buck71_sel_START (0)
#define PMIC_SYS_EN_CTRL11_sys_en_buck71_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo0_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL12_UNION;
#endif
#define PMIC_SYS_EN_CTRL12_sys_en_ldo0_sel_START (0)
#define PMIC_SYS_EN_CTRL12_sys_en_ldo0_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo2_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL13_UNION;
#endif
#define PMIC_SYS_EN_CTRL13_sys_en_ldo2_sel_START (0)
#define PMIC_SYS_EN_CTRL13_sys_en_ldo2_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo6_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL14_UNION;
#endif
#define PMIC_SYS_EN_CTRL14_sys_en_ldo6_sel_START (0)
#define PMIC_SYS_EN_CTRL14_sys_en_ldo6_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo30_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL15_UNION;
#endif
#define PMIC_SYS_EN_CTRL15_sys_en_ldo30_sel_START (0)
#define PMIC_SYS_EN_CTRL15_sys_en_ldo30_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo36_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL16_UNION;
#endif
#define PMIC_SYS_EN_CTRL16_sys_en_ldo36_sel_START (0)
#define PMIC_SYS_EN_CTRL16_sys_en_ldo36_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo37_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL17_UNION;
#endif
#define PMIC_SYS_EN_CTRL17_sys_en_ldo37_sel_START (0)
#define PMIC_SYS_EN_CTRL17_sys_en_ldo37_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo38_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL18_UNION;
#endif
#define PMIC_SYS_EN_CTRL18_sys_en_ldo38_sel_START (0)
#define PMIC_SYS_EN_CTRL18_sys_en_ldo38_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo39_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL19_UNION;
#endif
#define PMIC_SYS_EN_CTRL19_sys_en_ldo39_sel_START (0)
#define PMIC_SYS_EN_CTRL19_sys_en_ldo39_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_SYS_EN_CTRL20_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo43_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL21_UNION;
#endif
#define PMIC_SYS_EN_CTRL21_sys_en_ldo43_sel_START (0)
#define PMIC_SYS_EN_CTRL21_sys_en_ldo43_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo44_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL22_UNION;
#endif
#define PMIC_SYS_EN_CTRL22_sys_en_ldo44_sel_START (0)
#define PMIC_SYS_EN_CTRL22_sys_en_ldo44_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo45_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL23_UNION;
#endif
#define PMIC_SYS_EN_CTRL23_sys_en_ldo45_sel_START (0)
#define PMIC_SYS_EN_CTRL23_sys_en_ldo45_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ldo46_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL24_UNION;
#endif
#define PMIC_SYS_EN_CTRL24_sys_en_ldo46_sel_START (0)
#define PMIC_SYS_EN_CTRL24_sys_en_ldo46_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_sw1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL25_UNION;
#endif
#define PMIC_SYS_EN_CTRL25_sys_en_sw1_sel_START (0)
#define PMIC_SYS_EN_CTRL25_sys_en_sw1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_ref_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SYS_EN_CTRL26_UNION;
#endif
#define PMIC_SYS_EN_CTRL26_sys_en_ref_sel_START (0)
#define PMIC_SYS_EN_CTRL26_sys_en_ref_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sys_en_buck9_vset : 1;
        unsigned char sys_en_buck5_vset : 1;
        unsigned char sys_en_buck3_vset : 1;
        unsigned char sys_en_buck2_vset : 1;
        unsigned char sys_en_ldo44_vset : 1;
        unsigned char reserved : 1;
        unsigned char sys_en_ldo30_vset : 1;
        unsigned char sys_en_ldo0_vset : 1;
    } reg;
} PMIC_SYS_EN_VSET_CTRL_UNION;
#endif
#define PMIC_SYS_EN_VSET_CTRL_sys_en_buck9_vset_START (0)
#define PMIC_SYS_EN_VSET_CTRL_sys_en_buck9_vset_END (0)
#define PMIC_SYS_EN_VSET_CTRL_sys_en_buck5_vset_START (1)
#define PMIC_SYS_EN_VSET_CTRL_sys_en_buck5_vset_END (1)
#define PMIC_SYS_EN_VSET_CTRL_sys_en_buck3_vset_START (2)
#define PMIC_SYS_EN_VSET_CTRL_sys_en_buck3_vset_END (2)
#define PMIC_SYS_EN_VSET_CTRL_sys_en_buck2_vset_START (3)
#define PMIC_SYS_EN_VSET_CTRL_sys_en_buck2_vset_END (3)
#define PMIC_SYS_EN_VSET_CTRL_sys_en_ldo44_vset_START (4)
#define PMIC_SYS_EN_VSET_CTRL_sys_en_ldo44_vset_END (4)
#define PMIC_SYS_EN_VSET_CTRL_sys_en_ldo30_vset_START (6)
#define PMIC_SYS_EN_VSET_CTRL_sys_en_ldo30_vset_END (6)
#define PMIC_SYS_EN_VSET_CTRL_sys_en_ldo0_vset_START (7)
#define PMIC_SYS_EN_VSET_CTRL_sys_en_ldo0_vset_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ao_mode : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_AO_MODE_UNION;
#endif
#define PMIC_AO_MODE_ao_mode_START (0)
#define PMIC_AO_MODE_ao_mode_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_in_eusb_mask : 1;
        unsigned char idle_en_eusb_mask : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_AO_EUSB_MASK_UNION;
#endif
#define PMIC_AO_EUSB_MASK_eco_in_eusb_mask_START (0)
#define PMIC_AO_EUSB_MASK_eco_in_eusb_mask_END (0)
#define PMIC_AO_EUSB_MASK_idle_en_eusb_mask_START (1)
#define PMIC_AO_EUSB_MASK_idle_en_eusb_mask_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char coul_codec_clk_en_mask : 1;
        unsigned char coul_nfc_clk_en_mask : 1;
        unsigned char coul_wifi_clk_en_mask : 1;
        unsigned char coul_sys_clk_en_mask : 1;
        unsigned char reg_coul_idle_mask : 1;
        unsigned char reg_coul_eco_mask : 1;
        unsigned char coul_eusb_en_mask : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_COUL_ECO_MASK_UNION;
#endif
#define PMIC_COUL_ECO_MASK_coul_codec_clk_en_mask_START (0)
#define PMIC_COUL_ECO_MASK_coul_codec_clk_en_mask_END (0)
#define PMIC_COUL_ECO_MASK_coul_nfc_clk_en_mask_START (1)
#define PMIC_COUL_ECO_MASK_coul_nfc_clk_en_mask_END (1)
#define PMIC_COUL_ECO_MASK_coul_wifi_clk_en_mask_START (2)
#define PMIC_COUL_ECO_MASK_coul_wifi_clk_en_mask_END (2)
#define PMIC_COUL_ECO_MASK_coul_sys_clk_en_mask_START (3)
#define PMIC_COUL_ECO_MASK_coul_sys_clk_en_mask_END (3)
#define PMIC_COUL_ECO_MASK_reg_coul_idle_mask_START (4)
#define PMIC_COUL_ECO_MASK_reg_coul_idle_mask_END (4)
#define PMIC_COUL_ECO_MASK_reg_coul_eco_mask_START (5)
#define PMIC_COUL_ECO_MASK_reg_coul_eco_mask_END (5)
#define PMIC_COUL_ECO_MASK_coul_eusb_en_mask_START (6)
#define PMIC_COUL_ECO_MASK_coul_eusb_en_mask_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char adcr_codec_clk_en_mask : 1;
        unsigned char adcr_nfc_clk_en_mask : 1;
        unsigned char adcr_wifi_clk_en_mask : 1;
        unsigned char adcr_sys_clk_en_mask : 1;
        unsigned char adcr_eusb_en_mask : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_DCR_ECO_MASK_UNION;
#endif
#define PMIC_DCR_ECO_MASK_adcr_codec_clk_en_mask_START (0)
#define PMIC_DCR_ECO_MASK_adcr_codec_clk_en_mask_END (0)
#define PMIC_DCR_ECO_MASK_adcr_nfc_clk_en_mask_START (1)
#define PMIC_DCR_ECO_MASK_adcr_nfc_clk_en_mask_END (1)
#define PMIC_DCR_ECO_MASK_adcr_wifi_clk_en_mask_START (2)
#define PMIC_DCR_ECO_MASK_adcr_wifi_clk_en_mask_END (2)
#define PMIC_DCR_ECO_MASK_adcr_sys_clk_en_mask_START (3)
#define PMIC_DCR_ECO_MASK_adcr_sys_clk_en_mask_END (3)
#define PMIC_DCR_ECO_MASK_adcr_eusb_en_mask_START (4)
#define PMIC_DCR_ECO_MASK_adcr_eusb_en_mask_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char soft_rst_n : 8;
    } reg;
} PMIC_PMU_SOFT_RST_UNION;
#endif
#define PMIC_PMU_SOFT_RST_soft_rst_n_START (0)
#define PMIC_PMU_SOFT_RST_soft_rst_n_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char lock : 8;
    } reg;
} PMIC_LOCK_UNION;
#endif
#define PMIC_LOCK_lock_START (0)
#define PMIC_LOCK_lock_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char en_dr1_int : 1;
        unsigned char en_dr2_int : 1;
        unsigned char en_dr3_int : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_DR_EN_123_UNION;
#endif
#define PMIC_DR_EN_123_en_dr1_int_START (0)
#define PMIC_DR_EN_123_en_dr1_int_END (0)
#define PMIC_DR_EN_123_en_dr2_int_START (1)
#define PMIC_DR_EN_123_en_dr2_int_END (1)
#define PMIC_DR_EN_123_en_dr3_int_START (2)
#define PMIC_DR_EN_123_en_dr3_int_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr1_mode : 1;
        unsigned char dr2_mode : 1;
        unsigned char dr3_mode : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_DR_EN_MODE_123_UNION;
#endif
#define PMIC_DR_EN_MODE_123_dr1_mode_START (0)
#define PMIC_DR_EN_MODE_123_dr1_mode_END (0)
#define PMIC_DR_EN_MODE_123_dr2_mode_START (1)
#define PMIC_DR_EN_MODE_123_dr2_mode_END (1)
#define PMIC_DR_EN_MODE_123_dr3_mode_START (2)
#define PMIC_DR_EN_MODE_123_dr3_mode_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char flash_period_dr1 : 8;
    } reg;
} PMIC_FLASH_PERIOD_DR1_UNION;
#endif
#define PMIC_FLASH_PERIOD_DR1_flash_period_dr1_START (0)
#define PMIC_FLASH_PERIOD_DR1_flash_period_dr1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char flash_on_dr1 : 8;
    } reg;
} PMIC_FLASH_ON_DR1_UNION;
#endif
#define PMIC_FLASH_ON_DR1_flash_on_dr1_START (0)
#define PMIC_FLASH_ON_DR1_flash_on_dr1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char flash_period_dr2 : 8;
    } reg;
} PMIC_FLASH_PERIOD_DR2_UNION;
#endif
#define PMIC_FLASH_PERIOD_DR2_flash_period_dr2_START (0)
#define PMIC_FLASH_PERIOD_DR2_flash_period_dr2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char flash_on_dr2 : 8;
    } reg;
} PMIC_FLASH_ON_DR2_UNION;
#endif
#define PMIC_FLASH_ON_DR2_flash_on_dr2_START (0)
#define PMIC_FLASH_ON_DR2_flash_on_dr2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char flash_period_dr3 : 8;
    } reg;
} PMIC_FLASH_PERIOD_DR3_UNION;
#endif
#define PMIC_FLASH_PERIOD_DR3_flash_period_dr3_START (0)
#define PMIC_FLASH_PERIOD_DR3_flash_period_dr3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char flash_on_dr3 : 8;
    } reg;
} PMIC_FLASH_ON_DR3_UNION;
#endif
#define PMIC_FLASH_ON_DR3_flash_on_dr3_START (0)
#define PMIC_FLASH_ON_DR3_flash_on_dr3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr1_mode_sel : 1;
        unsigned char dr2_mode_sel : 1;
        unsigned char dr3_mode_sel : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_DR_MODE_SEL_UNION;
#endif
#define PMIC_DR_MODE_SEL_dr1_mode_sel_START (0)
#define PMIC_DR_MODE_SEL_dr1_mode_sel_END (0)
#define PMIC_DR_MODE_SEL_dr2_mode_sel_START (1)
#define PMIC_DR_MODE_SEL_dr2_mode_sel_END (1)
#define PMIC_DR_MODE_SEL_dr3_mode_sel_START (2)
#define PMIC_DR_MODE_SEL_dr3_mode_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_dr1_en : 1;
        unsigned char reg_dr2_en : 1;
        unsigned char reg_dr3_en : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_DR_BRE_CTRL_UNION;
#endif
#define PMIC_DR_BRE_CTRL_reg_dr1_en_START (0)
#define PMIC_DR_BRE_CTRL_reg_dr1_en_END (0)
#define PMIC_DR_BRE_CTRL_reg_dr2_en_START (1)
#define PMIC_DR_BRE_CTRL_reg_dr2_en_END (1)
#define PMIC_DR_BRE_CTRL_reg_dr3_en_START (2)
#define PMIC_DR_BRE_CTRL_reg_dr3_en_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr1_iset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_DR1_ISET_UNION;
#endif
#define PMIC_DR1_ISET_dr1_iset_START (0)
#define PMIC_DR1_ISET_dr1_iset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr2_iset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_DR2_ISET_UNION;
#endif
#define PMIC_DR2_ISET_dr2_iset_START (0)
#define PMIC_DR2_ISET_dr2_iset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr3_iset : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_DR3_ISET_UNION;
#endif
#define PMIC_DR3_ISET_dr3_iset_START (0)
#define PMIC_DR3_ISET_dr3_iset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr1_out_ctrl : 2;
        unsigned char dr2_out_ctrl : 2;
        unsigned char dr3_out_ctrl : 2;
        unsigned char reserved : 2;
    } reg;
} PMIC_DR_OUT_CTRL_UNION;
#endif
#define PMIC_DR_OUT_CTRL_dr1_out_ctrl_START (0)
#define PMIC_DR_OUT_CTRL_dr1_out_ctrl_END (1)
#define PMIC_DR_OUT_CTRL_dr2_out_ctrl_START (2)
#define PMIC_DR_OUT_CTRL_dr2_out_ctrl_END (3)
#define PMIC_DR_OUT_CTRL_dr3_out_ctrl_START (4)
#define PMIC_DR_OUT_CTRL_dr3_out_ctrl_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr1_start_delay : 8;
    } reg;
} PMIC_DR1_START_DEL_UNION;
#endif
#define PMIC_DR1_START_DEL_dr1_start_delay_START (0)
#define PMIC_DR1_START_DEL_dr1_start_delay_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr2_start_delay : 8;
    } reg;
} PMIC_DR2_START_DEL_UNION;
#endif
#define PMIC_DR2_START_DEL_dr2_start_delay_START (0)
#define PMIC_DR2_START_DEL_dr2_start_delay_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr3_start_delay : 8;
    } reg;
} PMIC_DR3_START_DEL_UNION;
#endif
#define PMIC_DR3_START_DEL_dr3_start_delay_START (0)
#define PMIC_DR3_START_DEL_dr3_start_delay_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr1_t_off : 4;
        unsigned char dr1_t_on : 4;
    } reg;
} PMIC_DR1_TIM_CONF0_UNION;
#endif
#define PMIC_DR1_TIM_CONF0_dr1_t_off_START (0)
#define PMIC_DR1_TIM_CONF0_dr1_t_off_END (3)
#define PMIC_DR1_TIM_CONF0_dr1_t_on_START (4)
#define PMIC_DR1_TIM_CONF0_dr1_t_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr1_t_rise : 3;
        unsigned char reserved_0 : 1;
        unsigned char dr1_t_fall : 3;
        unsigned char reserved_1 : 1;
    } reg;
} PMIC_DR1_TIM_CONF1_UNION;
#endif
#define PMIC_DR1_TIM_CONF1_dr1_t_rise_START (0)
#define PMIC_DR1_TIM_CONF1_dr1_t_rise_END (2)
#define PMIC_DR1_TIM_CONF1_dr1_t_fall_START (4)
#define PMIC_DR1_TIM_CONF1_dr1_t_fall_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr2_t_off : 4;
        unsigned char dr2_t_on : 4;
    } reg;
} PMIC_DR2_TIM_CONF0_UNION;
#endif
#define PMIC_DR2_TIM_CONF0_dr2_t_off_START (0)
#define PMIC_DR2_TIM_CONF0_dr2_t_off_END (3)
#define PMIC_DR2_TIM_CONF0_dr2_t_on_START (4)
#define PMIC_DR2_TIM_CONF0_dr2_t_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr2_t_rise : 3;
        unsigned char reserved_0 : 1;
        unsigned char dr2_t_fall : 3;
        unsigned char reserved_1 : 1;
    } reg;
} PMIC_DR2_TIM_CONF1_UNION;
#endif
#define PMIC_DR2_TIM_CONF1_dr2_t_rise_START (0)
#define PMIC_DR2_TIM_CONF1_dr2_t_rise_END (2)
#define PMIC_DR2_TIM_CONF1_dr2_t_fall_START (4)
#define PMIC_DR2_TIM_CONF1_dr2_t_fall_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr3_t_off : 4;
        unsigned char dr3_t_on : 4;
    } reg;
} PMIC_DR3_TIM_CONF0_UNION;
#endif
#define PMIC_DR3_TIM_CONF0_dr3_t_off_START (0)
#define PMIC_DR3_TIM_CONF0_dr3_t_off_END (3)
#define PMIC_DR3_TIM_CONF0_dr3_t_on_START (4)
#define PMIC_DR3_TIM_CONF0_dr3_t_on_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dr3_t_rise : 3;
        unsigned char reserved_0 : 1;
        unsigned char dr3_t_fall : 3;
        unsigned char reserved_1 : 1;
    } reg;
} PMIC_DR3_TIM_CONF1_UNION;
#endif
#define PMIC_DR3_TIM_CONF1_dr3_t_rise_START (0)
#define PMIC_DR3_TIM_CONF1_dr3_t_rise_END (2)
#define PMIC_DR3_TIM_CONF1_dr3_t_fall_START (4)
#define PMIC_DR3_TIM_CONF1_dr3_t_fall_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_DR_CTRLRESERVE8_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_DR_CTRLRESERVE9_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob0 : 8;
    } reg;
} PMIC_OTP0_0_UNION;
#endif
#define PMIC_OTP0_0_otp0_pdob0_START (0)
#define PMIC_OTP0_0_otp0_pdob0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob1 : 8;
    } reg;
} PMIC_OTP0_1_UNION;
#endif
#define PMIC_OTP0_1_otp0_pdob1_START (0)
#define PMIC_OTP0_1_otp0_pdob1_END (7)
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
} PMIC_OTP0_CTRL0_UNION;
#endif
#define PMIC_OTP0_CTRL0_otp0_pwe_int_START (0)
#define PMIC_OTP0_CTRL0_otp0_pwe_int_END (0)
#define PMIC_OTP0_CTRL0_otp0_pwe_pulse_START (1)
#define PMIC_OTP0_CTRL0_otp0_pwe_pulse_END (1)
#define PMIC_OTP0_CTRL0_otp0_por_int_START (2)
#define PMIC_OTP0_CTRL0_otp0_por_int_END (2)
#define PMIC_OTP0_CTRL0_otp0_por_pulse_START (3)
#define PMIC_OTP0_CTRL0_otp0_por_pulse_END (3)
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
} PMIC_OTP0_CTRL1_UNION;
#endif
#define PMIC_OTP0_CTRL1_otp0_pprog_START (0)
#define PMIC_OTP0_CTRL1_otp0_pprog_END (0)
#define PMIC_OTP0_CTRL1_otp0_inctrl_sel_START (1)
#define PMIC_OTP0_CTRL1_otp0_inctrl_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pa_cfg : 6;
        unsigned char otp0_ptm : 2;
    } reg;
} PMIC_OTP0_CTRL2_UNION;
#endif
#define PMIC_OTP0_CTRL2_otp0_pa_cfg_START (0)
#define PMIC_OTP0_CTRL2_otp0_pa_cfg_END (5)
#define PMIC_OTP0_CTRL2_otp0_ptm_START (6)
#define PMIC_OTP0_CTRL2_otp0_ptm_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdin : 8;
    } reg;
} PMIC_OTP0_WDATA_UNION;
#endif
#define PMIC_OTP0_WDATA_otp0_pdin_START (0)
#define PMIC_OTP0_WDATA_otp0_pdin_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob0_w : 8;
    } reg;
} PMIC_OTP0_0_W_UNION;
#endif
#define PMIC_OTP0_0_W_otp0_pdob0_w_START (0)
#define PMIC_OTP0_0_W_otp0_pdob0_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob1_w : 8;
    } reg;
} PMIC_OTP0_1_W_UNION;
#endif
#define PMIC_OTP0_1_W_otp0_pdob1_w_START (0)
#define PMIC_OTP0_1_W_otp0_pdob1_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob2_w : 8;
    } reg;
} PMIC_OTP0_2_W_UNION;
#endif
#define PMIC_OTP0_2_W_otp0_pdob2_w_START (0)
#define PMIC_OTP0_2_W_otp0_pdob2_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob3_w : 8;
    } reg;
} PMIC_OTP0_3_W_UNION;
#endif
#define PMIC_OTP0_3_W_otp0_pdob3_w_START (0)
#define PMIC_OTP0_3_W_otp0_pdob3_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob4_w : 8;
    } reg;
} PMIC_OTP0_4_W_UNION;
#endif
#define PMIC_OTP0_4_W_otp0_pdob4_w_START (0)
#define PMIC_OTP0_4_W_otp0_pdob4_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob5_w : 8;
    } reg;
} PMIC_OTP0_5_W_UNION;
#endif
#define PMIC_OTP0_5_W_otp0_pdob5_w_START (0)
#define PMIC_OTP0_5_W_otp0_pdob5_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob6_w : 8;
    } reg;
} PMIC_OTP0_6_W_UNION;
#endif
#define PMIC_OTP0_6_W_otp0_pdob6_w_START (0)
#define PMIC_OTP0_6_W_otp0_pdob6_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob7_w : 8;
    } reg;
} PMIC_OTP0_7_W_UNION;
#endif
#define PMIC_OTP0_7_W_otp0_pdob7_w_START (0)
#define PMIC_OTP0_7_W_otp0_pdob7_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob8_w : 8;
    } reg;
} PMIC_OTP0_8_W_UNION;
#endif
#define PMIC_OTP0_8_W_otp0_pdob8_w_START (0)
#define PMIC_OTP0_8_W_otp0_pdob8_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob9_w : 8;
    } reg;
} PMIC_OTP0_9_W_UNION;
#endif
#define PMIC_OTP0_9_W_otp0_pdob9_w_START (0)
#define PMIC_OTP0_9_W_otp0_pdob9_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob10_w : 8;
    } reg;
} PMIC_OTP0_10_W_UNION;
#endif
#define PMIC_OTP0_10_W_otp0_pdob10_w_START (0)
#define PMIC_OTP0_10_W_otp0_pdob10_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob11_w : 8;
    } reg;
} PMIC_OTP0_11_W_UNION;
#endif
#define PMIC_OTP0_11_W_otp0_pdob11_w_START (0)
#define PMIC_OTP0_11_W_otp0_pdob11_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob12_w : 8;
    } reg;
} PMIC_OTP0_12_W_UNION;
#endif
#define PMIC_OTP0_12_W_otp0_pdob12_w_START (0)
#define PMIC_OTP0_12_W_otp0_pdob12_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob13_w : 8;
    } reg;
} PMIC_OTP0_13_W_UNION;
#endif
#define PMIC_OTP0_13_W_otp0_pdob13_w_START (0)
#define PMIC_OTP0_13_W_otp0_pdob13_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob14_w : 8;
    } reg;
} PMIC_OTP0_14_W_UNION;
#endif
#define PMIC_OTP0_14_W_otp0_pdob14_w_START (0)
#define PMIC_OTP0_14_W_otp0_pdob14_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob15_w : 8;
    } reg;
} PMIC_OTP0_15_W_UNION;
#endif
#define PMIC_OTP0_15_W_otp0_pdob15_w_START (0)
#define PMIC_OTP0_15_W_otp0_pdob15_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob16_w : 8;
    } reg;
} PMIC_OTP0_16_W_UNION;
#endif
#define PMIC_OTP0_16_W_otp0_pdob16_w_START (0)
#define PMIC_OTP0_16_W_otp0_pdob16_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob17_w : 8;
    } reg;
} PMIC_OTP0_17_W_UNION;
#endif
#define PMIC_OTP0_17_W_otp0_pdob17_w_START (0)
#define PMIC_OTP0_17_W_otp0_pdob17_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob18_w : 8;
    } reg;
} PMIC_OTP0_18_W_UNION;
#endif
#define PMIC_OTP0_18_W_otp0_pdob18_w_START (0)
#define PMIC_OTP0_18_W_otp0_pdob18_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob19_w : 8;
    } reg;
} PMIC_OTP0_19_W_UNION;
#endif
#define PMIC_OTP0_19_W_otp0_pdob19_w_START (0)
#define PMIC_OTP0_19_W_otp0_pdob19_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob20_w : 8;
    } reg;
} PMIC_OTP0_20_W_UNION;
#endif
#define PMIC_OTP0_20_W_otp0_pdob20_w_START (0)
#define PMIC_OTP0_20_W_otp0_pdob20_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob21_w : 8;
    } reg;
} PMIC_OTP0_21_W_UNION;
#endif
#define PMIC_OTP0_21_W_otp0_pdob21_w_START (0)
#define PMIC_OTP0_21_W_otp0_pdob21_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob22_w : 8;
    } reg;
} PMIC_OTP0_22_W_UNION;
#endif
#define PMIC_OTP0_22_W_otp0_pdob22_w_START (0)
#define PMIC_OTP0_22_W_otp0_pdob22_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob23_w : 8;
    } reg;
} PMIC_OTP0_23_W_UNION;
#endif
#define PMIC_OTP0_23_W_otp0_pdob23_w_START (0)
#define PMIC_OTP0_23_W_otp0_pdob23_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob24_w : 8;
    } reg;
} PMIC_OTP0_24_W_UNION;
#endif
#define PMIC_OTP0_24_W_otp0_pdob24_w_START (0)
#define PMIC_OTP0_24_W_otp0_pdob24_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob25_w : 8;
    } reg;
} PMIC_OTP0_25_W_UNION;
#endif
#define PMIC_OTP0_25_W_otp0_pdob25_w_START (0)
#define PMIC_OTP0_25_W_otp0_pdob25_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob26_w : 8;
    } reg;
} PMIC_OTP0_26_W_UNION;
#endif
#define PMIC_OTP0_26_W_otp0_pdob26_w_START (0)
#define PMIC_OTP0_26_W_otp0_pdob26_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob27_w : 8;
    } reg;
} PMIC_OTP0_27_W_UNION;
#endif
#define PMIC_OTP0_27_W_otp0_pdob27_w_START (0)
#define PMIC_OTP0_27_W_otp0_pdob27_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob28_w : 8;
    } reg;
} PMIC_OTP0_28_W_UNION;
#endif
#define PMIC_OTP0_28_W_otp0_pdob28_w_START (0)
#define PMIC_OTP0_28_W_otp0_pdob28_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob29_w : 8;
    } reg;
} PMIC_OTP0_29_W_UNION;
#endif
#define PMIC_OTP0_29_W_otp0_pdob29_w_START (0)
#define PMIC_OTP0_29_W_otp0_pdob29_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob30_w : 8;
    } reg;
} PMIC_OTP0_30_W_UNION;
#endif
#define PMIC_OTP0_30_W_otp0_pdob30_w_START (0)
#define PMIC_OTP0_30_W_otp0_pdob30_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob31_w : 8;
    } reg;
} PMIC_OTP0_31_W_UNION;
#endif
#define PMIC_OTP0_31_W_otp0_pdob31_w_START (0)
#define PMIC_OTP0_31_W_otp0_pdob31_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob32_w : 8;
    } reg;
} PMIC_OTP0_32_W_UNION;
#endif
#define PMIC_OTP0_32_W_otp0_pdob32_w_START (0)
#define PMIC_OTP0_32_W_otp0_pdob32_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob33_w : 8;
    } reg;
} PMIC_OTP0_33_W_UNION;
#endif
#define PMIC_OTP0_33_W_otp0_pdob33_w_START (0)
#define PMIC_OTP0_33_W_otp0_pdob33_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob34_w : 8;
    } reg;
} PMIC_OTP0_34_W_UNION;
#endif
#define PMIC_OTP0_34_W_otp0_pdob34_w_START (0)
#define PMIC_OTP0_34_W_otp0_pdob34_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob35_w : 8;
    } reg;
} PMIC_OTP0_35_W_UNION;
#endif
#define PMIC_OTP0_35_W_otp0_pdob35_w_START (0)
#define PMIC_OTP0_35_W_otp0_pdob35_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob36_w : 8;
    } reg;
} PMIC_OTP0_36_W_UNION;
#endif
#define PMIC_OTP0_36_W_otp0_pdob36_w_START (0)
#define PMIC_OTP0_36_W_otp0_pdob36_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob37_w : 8;
    } reg;
} PMIC_OTP0_37_W_UNION;
#endif
#define PMIC_OTP0_37_W_otp0_pdob37_w_START (0)
#define PMIC_OTP0_37_W_otp0_pdob37_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob38_w : 8;
    } reg;
} PMIC_OTP0_38_W_UNION;
#endif
#define PMIC_OTP0_38_W_otp0_pdob38_w_START (0)
#define PMIC_OTP0_38_W_otp0_pdob38_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob39_w : 8;
    } reg;
} PMIC_OTP0_39_W_UNION;
#endif
#define PMIC_OTP0_39_W_otp0_pdob39_w_START (0)
#define PMIC_OTP0_39_W_otp0_pdob39_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob40_w : 8;
    } reg;
} PMIC_OTP0_40_W_UNION;
#endif
#define PMIC_OTP0_40_W_otp0_pdob40_w_START (0)
#define PMIC_OTP0_40_W_otp0_pdob40_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob41_w : 8;
    } reg;
} PMIC_OTP0_41_W_UNION;
#endif
#define PMIC_OTP0_41_W_otp0_pdob41_w_START (0)
#define PMIC_OTP0_41_W_otp0_pdob41_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob42_w : 8;
    } reg;
} PMIC_OTP0_42_W_UNION;
#endif
#define PMIC_OTP0_42_W_otp0_pdob42_w_START (0)
#define PMIC_OTP0_42_W_otp0_pdob42_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob43_w : 8;
    } reg;
} PMIC_OTP0_43_W_UNION;
#endif
#define PMIC_OTP0_43_W_otp0_pdob43_w_START (0)
#define PMIC_OTP0_43_W_otp0_pdob43_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob44_w : 8;
    } reg;
} PMIC_OTP0_44_W_UNION;
#endif
#define PMIC_OTP0_44_W_otp0_pdob44_w_START (0)
#define PMIC_OTP0_44_W_otp0_pdob44_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob45_w : 8;
    } reg;
} PMIC_OTP0_45_W_UNION;
#endif
#define PMIC_OTP0_45_W_otp0_pdob45_w_START (0)
#define PMIC_OTP0_45_W_otp0_pdob45_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob46_w : 8;
    } reg;
} PMIC_OTP0_46_W_UNION;
#endif
#define PMIC_OTP0_46_W_otp0_pdob46_w_START (0)
#define PMIC_OTP0_46_W_otp0_pdob46_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob47_w : 8;
    } reg;
} PMIC_OTP0_47_W_UNION;
#endif
#define PMIC_OTP0_47_W_otp0_pdob47_w_START (0)
#define PMIC_OTP0_47_W_otp0_pdob47_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob48_w : 8;
    } reg;
} PMIC_OTP0_48_W_UNION;
#endif
#define PMIC_OTP0_48_W_otp0_pdob48_w_START (0)
#define PMIC_OTP0_48_W_otp0_pdob48_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob49_w : 8;
    } reg;
} PMIC_OTP0_49_W_UNION;
#endif
#define PMIC_OTP0_49_W_otp0_pdob49_w_START (0)
#define PMIC_OTP0_49_W_otp0_pdob49_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob50_w : 8;
    } reg;
} PMIC_OTP0_50_W_UNION;
#endif
#define PMIC_OTP0_50_W_otp0_pdob50_w_START (0)
#define PMIC_OTP0_50_W_otp0_pdob50_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob51_w : 8;
    } reg;
} PMIC_OTP0_51_W_UNION;
#endif
#define PMIC_OTP0_51_W_otp0_pdob51_w_START (0)
#define PMIC_OTP0_51_W_otp0_pdob51_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob52_w : 8;
    } reg;
} PMIC_OTP0_52_W_UNION;
#endif
#define PMIC_OTP0_52_W_otp0_pdob52_w_START (0)
#define PMIC_OTP0_52_W_otp0_pdob52_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob53_w : 8;
    } reg;
} PMIC_OTP0_53_W_UNION;
#endif
#define PMIC_OTP0_53_W_otp0_pdob53_w_START (0)
#define PMIC_OTP0_53_W_otp0_pdob53_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob54_w : 8;
    } reg;
} PMIC_OTP0_54_W_UNION;
#endif
#define PMIC_OTP0_54_W_otp0_pdob54_w_START (0)
#define PMIC_OTP0_54_W_otp0_pdob54_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob55_w : 8;
    } reg;
} PMIC_OTP0_55_W_UNION;
#endif
#define PMIC_OTP0_55_W_otp0_pdob55_w_START (0)
#define PMIC_OTP0_55_W_otp0_pdob55_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob56_w : 8;
    } reg;
} PMIC_OTP0_56_W_UNION;
#endif
#define PMIC_OTP0_56_W_otp0_pdob56_w_START (0)
#define PMIC_OTP0_56_W_otp0_pdob56_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob57_w : 8;
    } reg;
} PMIC_OTP0_57_W_UNION;
#endif
#define PMIC_OTP0_57_W_otp0_pdob57_w_START (0)
#define PMIC_OTP0_57_W_otp0_pdob57_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob58_w : 8;
    } reg;
} PMIC_OTP0_58_W_UNION;
#endif
#define PMIC_OTP0_58_W_otp0_pdob58_w_START (0)
#define PMIC_OTP0_58_W_otp0_pdob58_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob59_w : 8;
    } reg;
} PMIC_OTP0_59_W_UNION;
#endif
#define PMIC_OTP0_59_W_otp0_pdob59_w_START (0)
#define PMIC_OTP0_59_W_otp0_pdob59_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob60_w : 8;
    } reg;
} PMIC_OTP0_60_W_UNION;
#endif
#define PMIC_OTP0_60_W_otp0_pdob60_w_START (0)
#define PMIC_OTP0_60_W_otp0_pdob60_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob61_w : 8;
    } reg;
} PMIC_OTP0_61_W_UNION;
#endif
#define PMIC_OTP0_61_W_otp0_pdob61_w_START (0)
#define PMIC_OTP0_61_W_otp0_pdob61_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob62_w : 8;
    } reg;
} PMIC_OTP0_62_W_UNION;
#endif
#define PMIC_OTP0_62_W_otp0_pdob62_w_START (0)
#define PMIC_OTP0_62_W_otp0_pdob62_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp0_pdob63_w : 8;
    } reg;
} PMIC_OTP0_63_W_UNION;
#endif
#define PMIC_OTP0_63_W_otp0_pdob63_w_START (0)
#define PMIC_OTP0_63_W_otp0_pdob63_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob0 : 8;
    } reg;
} PMIC_OTP1_0_UNION;
#endif
#define PMIC_OTP1_0_otp1_pdob0_START (0)
#define PMIC_OTP1_0_otp1_pdob0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob1 : 8;
    } reg;
} PMIC_OTP1_1_UNION;
#endif
#define PMIC_OTP1_1_otp1_pdob1_START (0)
#define PMIC_OTP1_1_otp1_pdob1_END (7)
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
} PMIC_OTP1_CTRL0_UNION;
#endif
#define PMIC_OTP1_CTRL0_otp1_pwe_int_START (0)
#define PMIC_OTP1_CTRL0_otp1_pwe_int_END (0)
#define PMIC_OTP1_CTRL0_otp1_pwe_pulse_START (1)
#define PMIC_OTP1_CTRL0_otp1_pwe_pulse_END (1)
#define PMIC_OTP1_CTRL0_otp1_por_int_START (2)
#define PMIC_OTP1_CTRL0_otp1_por_int_END (2)
#define PMIC_OTP1_CTRL0_otp1_por_pulse_START (3)
#define PMIC_OTP1_CTRL0_otp1_por_pulse_END (3)
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
} PMIC_OTP1_CTRL1_UNION;
#endif
#define PMIC_OTP1_CTRL1_otp1_pprog_START (0)
#define PMIC_OTP1_CTRL1_otp1_pprog_END (0)
#define PMIC_OTP1_CTRL1_otp1_inctrl_sel_START (1)
#define PMIC_OTP1_CTRL1_otp1_inctrl_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pa_cfg : 6;
        unsigned char otp1_ptm : 2;
    } reg;
} PMIC_OTP1_CTRL2_UNION;
#endif
#define PMIC_OTP1_CTRL2_otp1_pa_cfg_START (0)
#define PMIC_OTP1_CTRL2_otp1_pa_cfg_END (5)
#define PMIC_OTP1_CTRL2_otp1_ptm_START (6)
#define PMIC_OTP1_CTRL2_otp1_ptm_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdin : 8;
    } reg;
} PMIC_OTP1_WDATA_UNION;
#endif
#define PMIC_OTP1_WDATA_otp1_pdin_START (0)
#define PMIC_OTP1_WDATA_otp1_pdin_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob0_w : 8;
    } reg;
} PMIC_OTP1_0_W_UNION;
#endif
#define PMIC_OTP1_0_W_otp1_pdob0_w_START (0)
#define PMIC_OTP1_0_W_otp1_pdob0_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob1_w : 8;
    } reg;
} PMIC_OTP1_1_W_UNION;
#endif
#define PMIC_OTP1_1_W_otp1_pdob1_w_START (0)
#define PMIC_OTP1_1_W_otp1_pdob1_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob2_w : 8;
    } reg;
} PMIC_OTP1_2_W_UNION;
#endif
#define PMIC_OTP1_2_W_otp1_pdob2_w_START (0)
#define PMIC_OTP1_2_W_otp1_pdob2_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob3_w : 8;
    } reg;
} PMIC_OTP1_3_W_UNION;
#endif
#define PMIC_OTP1_3_W_otp1_pdob3_w_START (0)
#define PMIC_OTP1_3_W_otp1_pdob3_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob4_w : 8;
    } reg;
} PMIC_OTP1_4_W_UNION;
#endif
#define PMIC_OTP1_4_W_otp1_pdob4_w_START (0)
#define PMIC_OTP1_4_W_otp1_pdob4_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob5_w : 8;
    } reg;
} PMIC_OTP1_5_W_UNION;
#endif
#define PMIC_OTP1_5_W_otp1_pdob5_w_START (0)
#define PMIC_OTP1_5_W_otp1_pdob5_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char otp1_pdob6_w : 8;
    } reg;
} PMIC_OTP1_6_W_UNION;
#endif
#define PMIC_OTP1_6_W_otp1_pdob6_w_START (0)
#define PMIC_OTP1_6_W_otp1_pdob6_w_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim0_hpd_r_pd_en : 1;
        unsigned char sim0_hpd_f_pd_en : 1;
        unsigned char sim1_hpd_r_pd_en : 1;
        unsigned char sim1_hpd_f_pd_en : 1;
        unsigned char sim0_hpd_pd_ldo12_en : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_SIM_CTRL0_UNION;
#endif
#define PMIC_SIM_CTRL0_sim0_hpd_r_pd_en_START (0)
#define PMIC_SIM_CTRL0_sim0_hpd_r_pd_en_END (0)
#define PMIC_SIM_CTRL0_sim0_hpd_f_pd_en_START (1)
#define PMIC_SIM_CTRL0_sim0_hpd_f_pd_en_END (1)
#define PMIC_SIM_CTRL0_sim1_hpd_r_pd_en_START (2)
#define PMIC_SIM_CTRL0_sim1_hpd_r_pd_en_END (2)
#define PMIC_SIM_CTRL0_sim1_hpd_f_pd_en_START (3)
#define PMIC_SIM_CTRL0_sim1_hpd_f_pd_en_END (3)
#define PMIC_SIM_CTRL0_sim0_hpd_pd_ldo12_en_START (4)
#define PMIC_SIM_CTRL0_sim0_hpd_pd_ldo12_en_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim0_hpd_ldo16_en3 : 1;
        unsigned char sim0_hpd_ldo16_en2 : 1;
        unsigned char sim1_hpd_ldo16_en1 : 1;
        unsigned char sim1_hpd_ldo16_en0 : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_SIM_CTRL1_UNION;
#endif
#define PMIC_SIM_CTRL1_sim0_hpd_ldo16_en3_START (0)
#define PMIC_SIM_CTRL1_sim0_hpd_ldo16_en3_END (0)
#define PMIC_SIM_CTRL1_sim0_hpd_ldo16_en2_START (1)
#define PMIC_SIM_CTRL1_sim0_hpd_ldo16_en2_END (1)
#define PMIC_SIM_CTRL1_sim1_hpd_ldo16_en1_START (2)
#define PMIC_SIM_CTRL1_sim1_hpd_ldo16_en1_END (2)
#define PMIC_SIM_CTRL1_sim1_hpd_ldo16_en0_START (3)
#define PMIC_SIM_CTRL1_sim1_hpd_ldo16_en0_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim_hpd_deb_sel : 5;
        unsigned char sim_del_sel0 : 3;
    } reg;
} PMIC_SIM_DEB_CTRL1_UNION;
#endif
#define PMIC_SIM_DEB_CTRL1_sim_hpd_deb_sel_START (0)
#define PMIC_SIM_DEB_CTRL1_sim_hpd_deb_sel_END (4)
#define PMIC_SIM_DEB_CTRL1_sim_del_sel0_START (5)
#define PMIC_SIM_DEB_CTRL1_sim_del_sel0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim_del_sel : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_SIM_DEB_CTRL2_UNION;
#endif
#define PMIC_SIM_DEB_CTRL2_sim_del_sel_START (0)
#define PMIC_SIM_DEB_CTRL2_sim_del_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_data : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO0DATA_UNION;
#endif
#define PMIC_GPIO0DATA_gpio0_data_START (0)
#define PMIC_GPIO0DATA_gpio0_data_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_GPIO0_DATA_RESERVE_UNION;
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
} PMIC_GPIO0DIR_UNION;
#endif
#define PMIC_GPIO0DIR_gpio0_dir_START (0)
#define PMIC_GPIO0DIR_gpio0_dir_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_GPIO0_DIR_RESERVE_UNION;
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
} PMIC_GPIO0IS_UNION;
#endif
#define PMIC_GPIO0IS_gpio0_int_sense_START (0)
#define PMIC_GPIO0IS_gpio0_int_sense_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_int_edge : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO0IBE_UNION;
#endif
#define PMIC_GPIO0IBE_gpio0_int_edge_START (0)
#define PMIC_GPIO0IBE_gpio0_int_edge_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_int_f : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO0IEV_UNION;
#endif
#define PMIC_GPIO0IEV_gpio0_int_f_START (0)
#define PMIC_GPIO0IEV_gpio0_int_f_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_mode_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO0AFSEL_UNION;
#endif
#define PMIC_GPIO0AFSEL_gpio0_mode_ctrl_START (0)
#define PMIC_GPIO0AFSEL_gpio0_mode_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_out_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO0DSSEL_UNION;
#endif
#define PMIC_GPIO0DSSEL_gpio0_out_ctrl_START (0)
#define PMIC_GPIO0DSSEL_gpio0_out_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_pullup_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO0PUSEL_UNION;
#endif
#define PMIC_GPIO0PUSEL_gpio0_pullup_ctrl_START (0)
#define PMIC_GPIO0PUSEL_gpio0_pullup_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_pulldown_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO0PDSEL_UNION;
#endif
#define PMIC_GPIO0PDSEL_gpio0_pulldown_ctrl_START (0)
#define PMIC_GPIO0PDSEL_gpio0_pulldown_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_del_sel : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_GPIO0DEBSEL_UNION;
#endif
#define PMIC_GPIO0DEBSEL_gpio0_del_sel_START (0)
#define PMIC_GPIO0DEBSEL_gpio0_del_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim0_pullup_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_SIM0PUSEL_UNION;
#endif
#define PMIC_SIM0PUSEL_sim0_pullup_ctrl_START (0)
#define PMIC_SIM0PUSEL_sim0_pullup_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim0_pulldown_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_SIM0PDSEL_UNION;
#endif
#define PMIC_SIM0PDSEL_sim0_pulldown_ctrl_START (0)
#define PMIC_SIM0PDSEL_sim0_pulldown_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio1_data : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO1DATA_UNION;
#endif
#define PMIC_GPIO1DATA_gpio1_data_START (0)
#define PMIC_GPIO1DATA_gpio1_data_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_GPIO1_DATA_RESERVE_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio1_dir : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO1DIR_UNION;
#endif
#define PMIC_GPIO1DIR_gpio1_dir_START (0)
#define PMIC_GPIO1DIR_gpio1_dir_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_GPIO1_DIR_RESERVE_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio1_int_sense : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO1IS_UNION;
#endif
#define PMIC_GPIO1IS_gpio1_int_sense_START (0)
#define PMIC_GPIO1IS_gpio1_int_sense_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio1_int_edge : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO1IBE_UNION;
#endif
#define PMIC_GPIO1IBE_gpio1_int_edge_START (0)
#define PMIC_GPIO1IBE_gpio1_int_edge_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio1_int_f : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO1IEV_UNION;
#endif
#define PMIC_GPIO1IEV_gpio1_int_f_START (0)
#define PMIC_GPIO1IEV_gpio1_int_f_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio1_mode_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO1AFSEL_UNION;
#endif
#define PMIC_GPIO1AFSEL_gpio1_mode_ctrl_START (0)
#define PMIC_GPIO1AFSEL_gpio1_mode_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio1_out_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO1DSSEL_UNION;
#endif
#define PMIC_GPIO1DSSEL_gpio1_out_ctrl_START (0)
#define PMIC_GPIO1DSSEL_gpio1_out_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio1_pullup_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO1PUSEL_UNION;
#endif
#define PMIC_GPIO1PUSEL_gpio1_pullup_ctrl_START (0)
#define PMIC_GPIO1PUSEL_gpio1_pullup_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio1_pulldown_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_GPIO1PDSEL_UNION;
#endif
#define PMIC_GPIO1PDSEL_gpio1_pulldown_ctrl_START (0)
#define PMIC_GPIO1PDSEL_gpio1_pulldown_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio1_del_sel : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_GPIO1DEBSEL_UNION;
#endif
#define PMIC_GPIO1DEBSEL_gpio1_del_sel_START (0)
#define PMIC_GPIO1DEBSEL_gpio1_del_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim1_pullup_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_SIM1PUSEL_UNION;
#endif
#define PMIC_SIM1PUSEL_sim1_pullup_ctrl_START (0)
#define PMIC_SIM1PUSEL_sim1_pullup_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim1_pulldown_ctrl : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_SIM1PDSEL_UNION;
#endif
#define PMIC_SIM1PDSEL_sim1_pulldown_ctrl_START (0)
#define PMIC_SIM1PDSEL_sim1_pulldown_ctrl_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sim1_hpd_mode_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_SIM1_RST_MODE_DEL_UNION;
#endif
#define PMIC_SIM1_RST_MODE_DEL_sim1_hpd_mode_sel_START (0)
#define PMIC_SIM1_RST_MODE_DEL_sim1_hpd_mode_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck40_rampdown_ctrl : 3;
        unsigned char buck40_rampup_ctrl : 3;
        unsigned char buck40_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_RAMP_BUCK40_CTRL0_UNION;
#endif
#define PMIC_RAMP_BUCK40_CTRL0_buck40_rampdown_ctrl_START (0)
#define PMIC_RAMP_BUCK40_CTRL0_buck40_rampdown_ctrl_END (2)
#define PMIC_RAMP_BUCK40_CTRL0_buck40_rampup_ctrl_START (3)
#define PMIC_RAMP_BUCK40_CTRL0_buck40_rampup_ctrl_END (5)
#define PMIC_RAMP_BUCK40_CTRL0_buck40_ramp_en_cfg_START (6)
#define PMIC_RAMP_BUCK40_CTRL0_buck40_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck40_pull_down_off_cfg : 3;
        unsigned char buck40_pull_down_on_cfg : 3;
        unsigned char buck40_pull_down_cfg : 1;
        unsigned char buck40_pull_down_mode : 1;
    } reg;
} PMIC_RAMP_BUCK40_CTRL1_UNION;
#endif
#define PMIC_RAMP_BUCK40_CTRL1_buck40_pull_down_off_cfg_START (0)
#define PMIC_RAMP_BUCK40_CTRL1_buck40_pull_down_off_cfg_END (2)
#define PMIC_RAMP_BUCK40_CTRL1_buck40_pull_down_on_cfg_START (3)
#define PMIC_RAMP_BUCK40_CTRL1_buck40_pull_down_on_cfg_END (5)
#define PMIC_RAMP_BUCK40_CTRL1_buck40_pull_down_cfg_START (6)
#define PMIC_RAMP_BUCK40_CTRL1_buck40_pull_down_cfg_END (6)
#define PMIC_RAMP_BUCK40_CTRL1_buck40_pull_down_mode_START (7)
#define PMIC_RAMP_BUCK40_CTRL1_buck40_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck40_dly_ru_off_cfg : 3;
        unsigned char buck40_dly_ru_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK40_RAMPUP_STATE_UNION;
#endif
#define PMIC_BUCK40_RAMPUP_STATE_buck40_dly_ru_off_cfg_START (0)
#define PMIC_BUCK40_RAMPUP_STATE_buck40_dly_ru_off_cfg_END (2)
#define PMIC_BUCK40_RAMPUP_STATE_buck40_dly_ru_on_cfg_START (3)
#define PMIC_BUCK40_RAMPUP_STATE_buck40_dly_ru_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck40_dly_rd_pd_off_cfg : 3;
        unsigned char buck40_dly_rd_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK40_RAMPDOWN_STATE_UNION;
#endif
#define PMIC_BUCK40_RAMPDOWN_STATE_buck40_dly_rd_pd_off_cfg_START (0)
#define PMIC_BUCK40_RAMPDOWN_STATE_buck40_dly_rd_pd_off_cfg_END (2)
#define PMIC_BUCK40_RAMPDOWN_STATE_buck40_dly_rd_on_cfg_START (3)
#define PMIC_BUCK40_RAMPDOWN_STATE_buck40_dly_rd_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck40_rampup_chg_rate : 2;
        unsigned char buck40_rampup_chg_vset : 3;
        unsigned char buck40_ru_nochg_rate_sel : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK40_RAMP_CHG_CTRL0_UNION;
#endif
#define PMIC_BUCK40_RAMP_CHG_CTRL0_buck40_rampup_chg_rate_START (0)
#define PMIC_BUCK40_RAMP_CHG_CTRL0_buck40_rampup_chg_rate_END (1)
#define PMIC_BUCK40_RAMP_CHG_CTRL0_buck40_rampup_chg_vset_START (2)
#define PMIC_BUCK40_RAMP_CHG_CTRL0_buck40_rampup_chg_vset_END (4)
#define PMIC_BUCK40_RAMP_CHG_CTRL0_buck40_ru_nochg_rate_sel_START (5)
#define PMIC_BUCK40_RAMP_CHG_CTRL0_buck40_ru_nochg_rate_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck40_rampdown_chg_rate : 2;
        unsigned char buck40_rampdown_chg_vset : 3;
        unsigned char buck40_rd_nochg_rate_sel : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK40_RAMP_CHG_CTRL1_UNION;
#endif
#define PMIC_BUCK40_RAMP_CHG_CTRL1_buck40_rampdown_chg_rate_START (0)
#define PMIC_BUCK40_RAMP_CHG_CTRL1_buck40_rampdown_chg_rate_END (1)
#define PMIC_BUCK40_RAMP_CHG_CTRL1_buck40_rampdown_chg_vset_START (2)
#define PMIC_BUCK40_RAMP_CHG_CTRL1_buck40_rampdown_chg_vset_END (4)
#define PMIC_BUCK40_RAMP_CHG_CTRL1_buck40_rd_nochg_rate_sel_START (5)
#define PMIC_BUCK40_RAMP_CHG_CTRL1_buck40_rd_nochg_rate_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck42_rampdown_ctrl : 3;
        unsigned char buck42_rampup_ctrl : 3;
        unsigned char buck42_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_RAMP_BUCK42_CTRL0_UNION;
#endif
#define PMIC_RAMP_BUCK42_CTRL0_buck42_rampdown_ctrl_START (0)
#define PMIC_RAMP_BUCK42_CTRL0_buck42_rampdown_ctrl_END (2)
#define PMIC_RAMP_BUCK42_CTRL0_buck42_rampup_ctrl_START (3)
#define PMIC_RAMP_BUCK42_CTRL0_buck42_rampup_ctrl_END (5)
#define PMIC_RAMP_BUCK42_CTRL0_buck42_ramp_en_cfg_START (6)
#define PMIC_RAMP_BUCK42_CTRL0_buck42_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck42_pull_down_off_cfg : 3;
        unsigned char buck42_pull_down_on_cfg : 3;
        unsigned char buck42_pull_down_cfg : 1;
        unsigned char buck42_pull_down_mode : 1;
    } reg;
} PMIC_RAMP_BUCK42_CTRL1_UNION;
#endif
#define PMIC_RAMP_BUCK42_CTRL1_buck42_pull_down_off_cfg_START (0)
#define PMIC_RAMP_BUCK42_CTRL1_buck42_pull_down_off_cfg_END (2)
#define PMIC_RAMP_BUCK42_CTRL1_buck42_pull_down_on_cfg_START (3)
#define PMIC_RAMP_BUCK42_CTRL1_buck42_pull_down_on_cfg_END (5)
#define PMIC_RAMP_BUCK42_CTRL1_buck42_pull_down_cfg_START (6)
#define PMIC_RAMP_BUCK42_CTRL1_buck42_pull_down_cfg_END (6)
#define PMIC_RAMP_BUCK42_CTRL1_buck42_pull_down_mode_START (7)
#define PMIC_RAMP_BUCK42_CTRL1_buck42_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck42_dly_ru_off_cfg : 3;
        unsigned char buck42_dly_ru_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK42_RAMPUP_STATE_UNION;
#endif
#define PMIC_BUCK42_RAMPUP_STATE_buck42_dly_ru_off_cfg_START (0)
#define PMIC_BUCK42_RAMPUP_STATE_buck42_dly_ru_off_cfg_END (2)
#define PMIC_BUCK42_RAMPUP_STATE_buck42_dly_ru_on_cfg_START (3)
#define PMIC_BUCK42_RAMPUP_STATE_buck42_dly_ru_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck42_dly_rd_pd_off_cfg : 3;
        unsigned char buck42_dly_rd_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK42_RAMPDOWN_STATE_UNION;
#endif
#define PMIC_BUCK42_RAMPDOWN_STATE_buck42_dly_rd_pd_off_cfg_START (0)
#define PMIC_BUCK42_RAMPDOWN_STATE_buck42_dly_rd_pd_off_cfg_END (2)
#define PMIC_BUCK42_RAMPDOWN_STATE_buck42_dly_rd_on_cfg_START (3)
#define PMIC_BUCK42_RAMPDOWN_STATE_buck42_dly_rd_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck42_rampup_chg_rate : 2;
        unsigned char buck42_rampup_chg_vset : 3;
        unsigned char buck42_ru_nochg_rate_sel : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK42_RAMP_CHG_CTRL0_UNION;
#endif
#define PMIC_BUCK42_RAMP_CHG_CTRL0_buck42_rampup_chg_rate_START (0)
#define PMIC_BUCK42_RAMP_CHG_CTRL0_buck42_rampup_chg_rate_END (1)
#define PMIC_BUCK42_RAMP_CHG_CTRL0_buck42_rampup_chg_vset_START (2)
#define PMIC_BUCK42_RAMP_CHG_CTRL0_buck42_rampup_chg_vset_END (4)
#define PMIC_BUCK42_RAMP_CHG_CTRL0_buck42_ru_nochg_rate_sel_START (5)
#define PMIC_BUCK42_RAMP_CHG_CTRL0_buck42_ru_nochg_rate_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck42_rampdown_chg_rate : 2;
        unsigned char buck42_rampdown_chg_vset : 3;
        unsigned char buck42_rd_nochg_rate_sel : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK42_RAMP_CHG_CTRL1_UNION;
#endif
#define PMIC_BUCK42_RAMP_CHG_CTRL1_buck42_rampdown_chg_rate_START (0)
#define PMIC_BUCK42_RAMP_CHG_CTRL1_buck42_rampdown_chg_rate_END (1)
#define PMIC_BUCK42_RAMP_CHG_CTRL1_buck42_rampdown_chg_vset_START (2)
#define PMIC_BUCK42_RAMP_CHG_CTRL1_buck42_rampdown_chg_vset_END (4)
#define PMIC_BUCK42_RAMP_CHG_CTRL1_buck42_rd_nochg_rate_sel_START (5)
#define PMIC_BUCK42_RAMP_CHG_CTRL1_buck42_rd_nochg_rate_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck5_rampdown_ctrl : 3;
        unsigned char buck5_rampup_ctrl : 3;
        unsigned char buck5_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_RAMP_BUCK5_CTRL0_UNION;
#endif
#define PMIC_RAMP_BUCK5_CTRL0_buck5_rampdown_ctrl_START (0)
#define PMIC_RAMP_BUCK5_CTRL0_buck5_rampdown_ctrl_END (2)
#define PMIC_RAMP_BUCK5_CTRL0_buck5_rampup_ctrl_START (3)
#define PMIC_RAMP_BUCK5_CTRL0_buck5_rampup_ctrl_END (5)
#define PMIC_RAMP_BUCK5_CTRL0_buck5_ramp_en_cfg_START (6)
#define PMIC_RAMP_BUCK5_CTRL0_buck5_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck5_pull_down_off_cfg : 3;
        unsigned char buck5_pull_down_on_cfg : 3;
        unsigned char buck5_pull_down_cfg : 1;
        unsigned char buck5_pull_down_mode : 1;
    } reg;
} PMIC_RAMP_BUCK5_CTRL1_UNION;
#endif
#define PMIC_RAMP_BUCK5_CTRL1_buck5_pull_down_off_cfg_START (0)
#define PMIC_RAMP_BUCK5_CTRL1_buck5_pull_down_off_cfg_END (2)
#define PMIC_RAMP_BUCK5_CTRL1_buck5_pull_down_on_cfg_START (3)
#define PMIC_RAMP_BUCK5_CTRL1_buck5_pull_down_on_cfg_END (5)
#define PMIC_RAMP_BUCK5_CTRL1_buck5_pull_down_cfg_START (6)
#define PMIC_RAMP_BUCK5_CTRL1_buck5_pull_down_cfg_END (6)
#define PMIC_RAMP_BUCK5_CTRL1_buck5_pull_down_mode_START (7)
#define PMIC_RAMP_BUCK5_CTRL1_buck5_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck5_dly_ru_off_cfg : 3;
        unsigned char buck5_dly_ru_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK5_RAMPUP_STATE_UNION;
#endif
#define PMIC_BUCK5_RAMPUP_STATE_buck5_dly_ru_off_cfg_START (0)
#define PMIC_BUCK5_RAMPUP_STATE_buck5_dly_ru_off_cfg_END (2)
#define PMIC_BUCK5_RAMPUP_STATE_buck5_dly_ru_on_cfg_START (3)
#define PMIC_BUCK5_RAMPUP_STATE_buck5_dly_ru_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck5_dly_rd_pd_off_cfg : 3;
        unsigned char buck5_dly_rd_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK5_RAMPDOWN_STATE_UNION;
#endif
#define PMIC_BUCK5_RAMPDOWN_STATE_buck5_dly_rd_pd_off_cfg_START (0)
#define PMIC_BUCK5_RAMPDOWN_STATE_buck5_dly_rd_pd_off_cfg_END (2)
#define PMIC_BUCK5_RAMPDOWN_STATE_buck5_dly_rd_on_cfg_START (3)
#define PMIC_BUCK5_RAMPDOWN_STATE_buck5_dly_rd_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck5_rampup_chg_rate : 2;
        unsigned char buck5_rampup_chg_vset : 3;
        unsigned char buck5_ru_nochg_rate_sel : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK5_RAMP_CHG_CTRL0_UNION;
#endif
#define PMIC_BUCK5_RAMP_CHG_CTRL0_buck5_rampup_chg_rate_START (0)
#define PMIC_BUCK5_RAMP_CHG_CTRL0_buck5_rampup_chg_rate_END (1)
#define PMIC_BUCK5_RAMP_CHG_CTRL0_buck5_rampup_chg_vset_START (2)
#define PMIC_BUCK5_RAMP_CHG_CTRL0_buck5_rampup_chg_vset_END (4)
#define PMIC_BUCK5_RAMP_CHG_CTRL0_buck5_ru_nochg_rate_sel_START (5)
#define PMIC_BUCK5_RAMP_CHG_CTRL0_buck5_ru_nochg_rate_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck5_rampdown_chg_rate : 2;
        unsigned char buck5_rampdown_chg_vset : 3;
        unsigned char buck5_rd_nochg_rate_sel : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK5_RAMP_CHG_CTRL1_UNION;
#endif
#define PMIC_BUCK5_RAMP_CHG_CTRL1_buck5_rampdown_chg_rate_START (0)
#define PMIC_BUCK5_RAMP_CHG_CTRL1_buck5_rampdown_chg_rate_END (1)
#define PMIC_BUCK5_RAMP_CHG_CTRL1_buck5_rampdown_chg_vset_START (2)
#define PMIC_BUCK5_RAMP_CHG_CTRL1_buck5_rampdown_chg_vset_END (4)
#define PMIC_BUCK5_RAMP_CHG_CTRL1_buck5_rd_nochg_rate_sel_START (5)
#define PMIC_BUCK5_RAMP_CHG_CTRL1_buck5_rd_nochg_rate_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck70_rampdown_ctrl : 3;
        unsigned char buck70_rampup_ctrl : 3;
        unsigned char buck70_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_RAMP_BUCK70_CTRL0_UNION;
#endif
#define PMIC_RAMP_BUCK70_CTRL0_buck70_rampdown_ctrl_START (0)
#define PMIC_RAMP_BUCK70_CTRL0_buck70_rampdown_ctrl_END (2)
#define PMIC_RAMP_BUCK70_CTRL0_buck70_rampup_ctrl_START (3)
#define PMIC_RAMP_BUCK70_CTRL0_buck70_rampup_ctrl_END (5)
#define PMIC_RAMP_BUCK70_CTRL0_buck70_ramp_en_cfg_START (6)
#define PMIC_RAMP_BUCK70_CTRL0_buck70_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck70_pull_down_off_cfg : 3;
        unsigned char buck70_pull_down_on_cfg : 3;
        unsigned char buck70_pull_down_cfg : 1;
        unsigned char buck70_pull_down_mode : 1;
    } reg;
} PMIC_RAMP_BUCK70_CTRL1_UNION;
#endif
#define PMIC_RAMP_BUCK70_CTRL1_buck70_pull_down_off_cfg_START (0)
#define PMIC_RAMP_BUCK70_CTRL1_buck70_pull_down_off_cfg_END (2)
#define PMIC_RAMP_BUCK70_CTRL1_buck70_pull_down_on_cfg_START (3)
#define PMIC_RAMP_BUCK70_CTRL1_buck70_pull_down_on_cfg_END (5)
#define PMIC_RAMP_BUCK70_CTRL1_buck70_pull_down_cfg_START (6)
#define PMIC_RAMP_BUCK70_CTRL1_buck70_pull_down_cfg_END (6)
#define PMIC_RAMP_BUCK70_CTRL1_buck70_pull_down_mode_START (7)
#define PMIC_RAMP_BUCK70_CTRL1_buck70_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck70_dly_ru_off_cfg : 3;
        unsigned char buck70_dly_ru_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK70_RAMPUP_STATE_UNION;
#endif
#define PMIC_BUCK70_RAMPUP_STATE_buck70_dly_ru_off_cfg_START (0)
#define PMIC_BUCK70_RAMPUP_STATE_buck70_dly_ru_off_cfg_END (2)
#define PMIC_BUCK70_RAMPUP_STATE_buck70_dly_ru_on_cfg_START (3)
#define PMIC_BUCK70_RAMPUP_STATE_buck70_dly_ru_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck70_dly_rd_pd_off_cfg : 3;
        unsigned char buck70_dly_rd_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK70_RAMPDOWN_STATE_UNION;
#endif
#define PMIC_BUCK70_RAMPDOWN_STATE_buck70_dly_rd_pd_off_cfg_START (0)
#define PMIC_BUCK70_RAMPDOWN_STATE_buck70_dly_rd_pd_off_cfg_END (2)
#define PMIC_BUCK70_RAMPDOWN_STATE_buck70_dly_rd_on_cfg_START (3)
#define PMIC_BUCK70_RAMPDOWN_STATE_buck70_dly_rd_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck70_rampup_chg_rate : 2;
        unsigned char buck70_rampup_chg_vset : 3;
        unsigned char buck70_ru_nochg_rate_sel : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK70_RAMP_CHG_CTRL0_UNION;
#endif
#define PMIC_BUCK70_RAMP_CHG_CTRL0_buck70_rampup_chg_rate_START (0)
#define PMIC_BUCK70_RAMP_CHG_CTRL0_buck70_rampup_chg_rate_END (1)
#define PMIC_BUCK70_RAMP_CHG_CTRL0_buck70_rampup_chg_vset_START (2)
#define PMIC_BUCK70_RAMP_CHG_CTRL0_buck70_rampup_chg_vset_END (4)
#define PMIC_BUCK70_RAMP_CHG_CTRL0_buck70_ru_nochg_rate_sel_START (5)
#define PMIC_BUCK70_RAMP_CHG_CTRL0_buck70_ru_nochg_rate_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck70_rampdown_chg_rate : 2;
        unsigned char buck70_rampdown_chg_vset : 3;
        unsigned char buck70_rd_nochg_rate_sel : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK70_RAMP_CHG_CTRL1_UNION;
#endif
#define PMIC_BUCK70_RAMP_CHG_CTRL1_buck70_rampdown_chg_rate_START (0)
#define PMIC_BUCK70_RAMP_CHG_CTRL1_buck70_rampdown_chg_rate_END (1)
#define PMIC_BUCK70_RAMP_CHG_CTRL1_buck70_rampdown_chg_vset_START (2)
#define PMIC_BUCK70_RAMP_CHG_CTRL1_buck70_rampdown_chg_vset_END (4)
#define PMIC_BUCK70_RAMP_CHG_CTRL1_buck70_rd_nochg_rate_sel_START (5)
#define PMIC_BUCK70_RAMP_CHG_CTRL1_buck70_rd_nochg_rate_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck71_rampdown_ctrl : 3;
        unsigned char buck71_rampup_ctrl : 3;
        unsigned char buck71_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_RAMP_BUCK71_CTRL0_UNION;
#endif
#define PMIC_RAMP_BUCK71_CTRL0_buck71_rampdown_ctrl_START (0)
#define PMIC_RAMP_BUCK71_CTRL0_buck71_rampdown_ctrl_END (2)
#define PMIC_RAMP_BUCK71_CTRL0_buck71_rampup_ctrl_START (3)
#define PMIC_RAMP_BUCK71_CTRL0_buck71_rampup_ctrl_END (5)
#define PMIC_RAMP_BUCK71_CTRL0_buck71_ramp_en_cfg_START (6)
#define PMIC_RAMP_BUCK71_CTRL0_buck71_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck71_pull_down_off_cfg : 3;
        unsigned char buck71_pull_down_on_cfg : 3;
        unsigned char buck71_pull_down_cfg : 1;
        unsigned char buck71_pull_down_mode : 1;
    } reg;
} PMIC_RAMP_BUCK71_CTRL1_UNION;
#endif
#define PMIC_RAMP_BUCK71_CTRL1_buck71_pull_down_off_cfg_START (0)
#define PMIC_RAMP_BUCK71_CTRL1_buck71_pull_down_off_cfg_END (2)
#define PMIC_RAMP_BUCK71_CTRL1_buck71_pull_down_on_cfg_START (3)
#define PMIC_RAMP_BUCK71_CTRL1_buck71_pull_down_on_cfg_END (5)
#define PMIC_RAMP_BUCK71_CTRL1_buck71_pull_down_cfg_START (6)
#define PMIC_RAMP_BUCK71_CTRL1_buck71_pull_down_cfg_END (6)
#define PMIC_RAMP_BUCK71_CTRL1_buck71_pull_down_mode_START (7)
#define PMIC_RAMP_BUCK71_CTRL1_buck71_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck71_dly_ru_off_cfg : 3;
        unsigned char buck71_dly_ru_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK71_RAMPUP_STATE_UNION;
#endif
#define PMIC_BUCK71_RAMPUP_STATE_buck71_dly_ru_off_cfg_START (0)
#define PMIC_BUCK71_RAMPUP_STATE_buck71_dly_ru_off_cfg_END (2)
#define PMIC_BUCK71_RAMPUP_STATE_buck71_dly_ru_on_cfg_START (3)
#define PMIC_BUCK71_RAMPUP_STATE_buck71_dly_ru_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck71_dly_rd_pd_off_cfg : 3;
        unsigned char buck71_dly_rd_on_cfg : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK71_RAMPDOWN_STATE_UNION;
#endif
#define PMIC_BUCK71_RAMPDOWN_STATE_buck71_dly_rd_pd_off_cfg_START (0)
#define PMIC_BUCK71_RAMPDOWN_STATE_buck71_dly_rd_pd_off_cfg_END (2)
#define PMIC_BUCK71_RAMPDOWN_STATE_buck71_dly_rd_on_cfg_START (3)
#define PMIC_BUCK71_RAMPDOWN_STATE_buck71_dly_rd_on_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck71_rampup_chg_rate : 2;
        unsigned char buck71_rampup_chg_vset : 3;
        unsigned char buck71_ru_nochg_rate_sel : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK71_RAMP_CHG_CTRL0_UNION;
#endif
#define PMIC_BUCK71_RAMP_CHG_CTRL0_buck71_rampup_chg_rate_START (0)
#define PMIC_BUCK71_RAMP_CHG_CTRL0_buck71_rampup_chg_rate_END (1)
#define PMIC_BUCK71_RAMP_CHG_CTRL0_buck71_rampup_chg_vset_START (2)
#define PMIC_BUCK71_RAMP_CHG_CTRL0_buck71_rampup_chg_vset_END (4)
#define PMIC_BUCK71_RAMP_CHG_CTRL0_buck71_ru_nochg_rate_sel_START (5)
#define PMIC_BUCK71_RAMP_CHG_CTRL0_buck71_ru_nochg_rate_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck71_rampdown_chg_rate : 2;
        unsigned char buck71_rampdown_chg_vset : 3;
        unsigned char buck71_rd_nochg_rate_sel : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK71_RAMP_CHG_CTRL1_UNION;
#endif
#define PMIC_BUCK71_RAMP_CHG_CTRL1_buck71_rampdown_chg_rate_START (0)
#define PMIC_BUCK71_RAMP_CHG_CTRL1_buck71_rampdown_chg_rate_END (1)
#define PMIC_BUCK71_RAMP_CHG_CTRL1_buck71_rampdown_chg_vset_START (2)
#define PMIC_BUCK71_RAMP_CHG_CTRL1_buck71_rampdown_chg_vset_END (4)
#define PMIC_BUCK71_RAMP_CHG_CTRL1_buck71_rd_nochg_rate_sel_START (5)
#define PMIC_BUCK71_RAMP_CHG_CTRL1_buck71_rd_nochg_rate_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo0_rampdown_ctrl : 3;
        unsigned char ldo0_rampup_ctrl : 3;
        unsigned char ldo0_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_RAMP_LDO0_CTRL0_UNION;
#endif
#define PMIC_RAMP_LDO0_CTRL0_ldo0_rampdown_ctrl_START (0)
#define PMIC_RAMP_LDO0_CTRL0_ldo0_rampdown_ctrl_END (2)
#define PMIC_RAMP_LDO0_CTRL0_ldo0_rampup_ctrl_START (3)
#define PMIC_RAMP_LDO0_CTRL0_ldo0_rampup_ctrl_END (5)
#define PMIC_RAMP_LDO0_CTRL0_ldo0_ramp_en_cfg_START (6)
#define PMIC_RAMP_LDO0_CTRL0_ldo0_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo0_pull_down_off_cfg : 3;
        unsigned char ldo0_pull_down_on_cfg : 3;
        unsigned char ldo0_pull_down_cfg : 1;
        unsigned char ldo0_pull_down_mode : 1;
    } reg;
} PMIC_RAMP_LDO0_CTRL1_UNION;
#endif
#define PMIC_RAMP_LDO0_CTRL1_ldo0_pull_down_off_cfg_START (0)
#define PMIC_RAMP_LDO0_CTRL1_ldo0_pull_down_off_cfg_END (2)
#define PMIC_RAMP_LDO0_CTRL1_ldo0_pull_down_on_cfg_START (3)
#define PMIC_RAMP_LDO0_CTRL1_ldo0_pull_down_on_cfg_END (5)
#define PMIC_RAMP_LDO0_CTRL1_ldo0_pull_down_cfg_START (6)
#define PMIC_RAMP_LDO0_CTRL1_ldo0_pull_down_cfg_END (6)
#define PMIC_RAMP_LDO0_CTRL1_ldo0_pull_down_mode_START (7)
#define PMIC_RAMP_LDO0_CTRL1_ldo0_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo44_rampdown_ctrl : 3;
        unsigned char ldo44_rampup_ctrl : 3;
        unsigned char ldo44_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_RAMP_LDO44_CTRL0_UNION;
#endif
#define PMIC_RAMP_LDO44_CTRL0_ldo44_rampdown_ctrl_START (0)
#define PMIC_RAMP_LDO44_CTRL0_ldo44_rampdown_ctrl_END (2)
#define PMIC_RAMP_LDO44_CTRL0_ldo44_rampup_ctrl_START (3)
#define PMIC_RAMP_LDO44_CTRL0_ldo44_rampup_ctrl_END (5)
#define PMIC_RAMP_LDO44_CTRL0_ldo44_ramp_en_cfg_START (6)
#define PMIC_RAMP_LDO44_CTRL0_ldo44_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo44_pull_down_off_cfg : 3;
        unsigned char ldo44_pull_down_on_cfg : 3;
        unsigned char ldo44_pull_down_cfg : 1;
        unsigned char ldo44_pull_down_mode : 1;
    } reg;
} PMIC_RAMP_LDO44_CTRL1_UNION;
#endif
#define PMIC_RAMP_LDO44_CTRL1_ldo44_pull_down_off_cfg_START (0)
#define PMIC_RAMP_LDO44_CTRL1_ldo44_pull_down_off_cfg_END (2)
#define PMIC_RAMP_LDO44_CTRL1_ldo44_pull_down_on_cfg_START (3)
#define PMIC_RAMP_LDO44_CTRL1_ldo44_pull_down_on_cfg_END (5)
#define PMIC_RAMP_LDO44_CTRL1_ldo44_pull_down_cfg_START (6)
#define PMIC_RAMP_LDO44_CTRL1_ldo44_pull_down_cfg_END (6)
#define PMIC_RAMP_LDO44_CTRL1_ldo44_pull_down_mode_START (7)
#define PMIC_RAMP_LDO44_CTRL1_ldo44_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo45_rampdown_ctrl : 3;
        unsigned char ldo45_rampup_ctrl : 3;
        unsigned char ldo45_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_RAMP_LDO45_CTRL0_UNION;
#endif
#define PMIC_RAMP_LDO45_CTRL0_ldo45_rampdown_ctrl_START (0)
#define PMIC_RAMP_LDO45_CTRL0_ldo45_rampdown_ctrl_END (2)
#define PMIC_RAMP_LDO45_CTRL0_ldo45_rampup_ctrl_START (3)
#define PMIC_RAMP_LDO45_CTRL0_ldo45_rampup_ctrl_END (5)
#define PMIC_RAMP_LDO45_CTRL0_ldo45_ramp_en_cfg_START (6)
#define PMIC_RAMP_LDO45_CTRL0_ldo45_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo45_pull_down_off_cfg : 3;
        unsigned char ldo45_pull_down_on_cfg : 3;
        unsigned char ldo45_pull_down_cfg : 1;
        unsigned char ldo45_pull_down_mode : 1;
    } reg;
} PMIC_RAMP_LDO45_CTRL1_UNION;
#endif
#define PMIC_RAMP_LDO45_CTRL1_ldo45_pull_down_off_cfg_START (0)
#define PMIC_RAMP_LDO45_CTRL1_ldo45_pull_down_off_cfg_END (2)
#define PMIC_RAMP_LDO45_CTRL1_ldo45_pull_down_on_cfg_START (3)
#define PMIC_RAMP_LDO45_CTRL1_ldo45_pull_down_on_cfg_END (5)
#define PMIC_RAMP_LDO45_CTRL1_ldo45_pull_down_cfg_START (6)
#define PMIC_RAMP_LDO45_CTRL1_ldo45_pull_down_cfg_END (6)
#define PMIC_RAMP_LDO45_CTRL1_ldo45_pull_down_mode_START (7)
#define PMIC_RAMP_LDO45_CTRL1_ldo45_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_eco_gt_bypass : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_SPMI_ECO_GT_BYPASS_UNION;
#endif
#define PMIC_SPMI_ECO_GT_BYPASS_spmi_eco_gt_bypass_START (0)
#define PMIC_SPMI_ECO_GT_BYPASS_spmi_eco_gt_bypass_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xoadc_debug : 1;
        unsigned char pwronn_8s_hreset_mode : 1;
        unsigned char ramp_gt_debug : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_PWRONN_8S_XOADC_DEBUG_CTRL_UNION;
#endif
#define PMIC_PWRONN_8S_XOADC_DEBUG_CTRL_xoadc_debug_START (0)
#define PMIC_PWRONN_8S_XOADC_DEBUG_CTRL_xoadc_debug_END (0)
#define PMIC_PWRONN_8S_XOADC_DEBUG_CTRL_pwronn_8s_hreset_mode_START (1)
#define PMIC_PWRONN_8S_XOADC_DEBUG_CTRL_pwronn_8s_hreset_mode_END (1)
#define PMIC_PWRONN_8S_XOADC_DEBUG_CTRL_ramp_gt_debug_START (2)
#define PMIC_PWRONN_8S_XOADC_DEBUG_CTRL_ramp_gt_debug_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dis_19m2_hreset_mode : 1;
        unsigned char dis_19m2_hreset_mask : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_DIS_19M2_CTRL_UNION;
#endif
#define PMIC_DIS_19M2_CTRL_dis_19m2_hreset_mode_START (0)
#define PMIC_DIS_19M2_CTRL_dis_19m2_hreset_mode_END (0)
#define PMIC_DIS_19M2_CTRL_dis_19m2_hreset_mask_START (1)
#define PMIC_DIS_19M2_CTRL_dis_19m2_hreset_mask_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char fault_s_n_mask : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_FAULT_S_N_CTRL_UNION;
#endif
#define PMIC_FAULT_S_N_CTRL_fault_s_n_mask_START (0)
#define PMIC_FAULT_S_N_CTRL_fault_s_n_mask_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pwronn_cnt : 8;
    } reg;
} PMIC_PWRONN_CNT_UNION;
#endif
#define PMIC_PWRONN_CNT_pwronn_cnt_START (0)
#define PMIC_PWRONN_CNT_pwronn_cnt_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pwronn_cnt_mask : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_PWRONN_CNT_MASK_UNION;
#endif
#define PMIC_PWRONN_CNT_MASK_pwronn_cnt_mask_START (0)
#define PMIC_PWRONN_CNT_MASK_pwronn_cnt_mask_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_0_0 : 1;
        unsigned char irq_mask_0_1 : 1;
        unsigned char irq_mask_0_2 : 1;
        unsigned char irq_mask_0_3 : 1;
        unsigned char irq_mask_0_4 : 1;
        unsigned char irq_mask_0_5 : 1;
        unsigned char irq_mask_0_6 : 1;
        unsigned char irq_mask_0_7 : 1;
    } reg;
} PMIC_IRQ_MASK_0_UNION;
#endif
#define PMIC_IRQ_MASK_0_irq_mask_0_0_START (0)
#define PMIC_IRQ_MASK_0_irq_mask_0_0_END (0)
#define PMIC_IRQ_MASK_0_irq_mask_0_1_START (1)
#define PMIC_IRQ_MASK_0_irq_mask_0_1_END (1)
#define PMIC_IRQ_MASK_0_irq_mask_0_2_START (2)
#define PMIC_IRQ_MASK_0_irq_mask_0_2_END (2)
#define PMIC_IRQ_MASK_0_irq_mask_0_3_START (3)
#define PMIC_IRQ_MASK_0_irq_mask_0_3_END (3)
#define PMIC_IRQ_MASK_0_irq_mask_0_4_START (4)
#define PMIC_IRQ_MASK_0_irq_mask_0_4_END (4)
#define PMIC_IRQ_MASK_0_irq_mask_0_5_START (5)
#define PMIC_IRQ_MASK_0_irq_mask_0_5_END (5)
#define PMIC_IRQ_MASK_0_irq_mask_0_6_START (6)
#define PMIC_IRQ_MASK_0_irq_mask_0_6_END (6)
#define PMIC_IRQ_MASK_0_irq_mask_0_7_START (7)
#define PMIC_IRQ_MASK_0_irq_mask_0_7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_1_0 : 1;
        unsigned char irq_mask_1_1 : 1;
        unsigned char irq_mask_1_2 : 1;
        unsigned char irq_mask_1_3 : 1;
        unsigned char irq_mask_1_4 : 1;
        unsigned char irq_mask_1_5 : 1;
        unsigned char irq_mask_1_6 : 1;
        unsigned char irq_mask_1_7 : 1;
    } reg;
} PMIC_IRQ_MASK_1_UNION;
#endif
#define PMIC_IRQ_MASK_1_irq_mask_1_0_START (0)
#define PMIC_IRQ_MASK_1_irq_mask_1_0_END (0)
#define PMIC_IRQ_MASK_1_irq_mask_1_1_START (1)
#define PMIC_IRQ_MASK_1_irq_mask_1_1_END (1)
#define PMIC_IRQ_MASK_1_irq_mask_1_2_START (2)
#define PMIC_IRQ_MASK_1_irq_mask_1_2_END (2)
#define PMIC_IRQ_MASK_1_irq_mask_1_3_START (3)
#define PMIC_IRQ_MASK_1_irq_mask_1_3_END (3)
#define PMIC_IRQ_MASK_1_irq_mask_1_4_START (4)
#define PMIC_IRQ_MASK_1_irq_mask_1_4_END (4)
#define PMIC_IRQ_MASK_1_irq_mask_1_5_START (5)
#define PMIC_IRQ_MASK_1_irq_mask_1_5_END (5)
#define PMIC_IRQ_MASK_1_irq_mask_1_6_START (6)
#define PMIC_IRQ_MASK_1_irq_mask_1_6_END (6)
#define PMIC_IRQ_MASK_1_irq_mask_1_7_START (7)
#define PMIC_IRQ_MASK_1_irq_mask_1_7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_2_0 : 1;
        unsigned char irq_mask_2_1 : 1;
        unsigned char irq_mask_2_2 : 1;
        unsigned char irq_mask_2_3 : 1;
        unsigned char irq_mask_2_4 : 1;
        unsigned char irq_mask_2_5 : 1;
        unsigned char irq_mask_2_6 : 1;
        unsigned char irq_mask_2_7 : 1;
    } reg;
} PMIC_IRQ_MASK_2_UNION;
#endif
#define PMIC_IRQ_MASK_2_irq_mask_2_0_START (0)
#define PMIC_IRQ_MASK_2_irq_mask_2_0_END (0)
#define PMIC_IRQ_MASK_2_irq_mask_2_1_START (1)
#define PMIC_IRQ_MASK_2_irq_mask_2_1_END (1)
#define PMIC_IRQ_MASK_2_irq_mask_2_2_START (2)
#define PMIC_IRQ_MASK_2_irq_mask_2_2_END (2)
#define PMIC_IRQ_MASK_2_irq_mask_2_3_START (3)
#define PMIC_IRQ_MASK_2_irq_mask_2_3_END (3)
#define PMIC_IRQ_MASK_2_irq_mask_2_4_START (4)
#define PMIC_IRQ_MASK_2_irq_mask_2_4_END (4)
#define PMIC_IRQ_MASK_2_irq_mask_2_5_START (5)
#define PMIC_IRQ_MASK_2_irq_mask_2_5_END (5)
#define PMIC_IRQ_MASK_2_irq_mask_2_6_START (6)
#define PMIC_IRQ_MASK_2_irq_mask_2_6_END (6)
#define PMIC_IRQ_MASK_2_irq_mask_2_7_START (7)
#define PMIC_IRQ_MASK_2_irq_mask_2_7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_3_0 : 1;
        unsigned char irq_mask_3_1 : 1;
        unsigned char irq_mask_3_2 : 1;
        unsigned char irq_mask_3_3 : 1;
        unsigned char irq_mask_3_4 : 1;
        unsigned char irq_mask_3_5 : 1;
        unsigned char irq_mask_3_6 : 1;
        unsigned char irq_mask_3_7 : 1;
    } reg;
} PMIC_IRQ_MASK_3_UNION;
#endif
#define PMIC_IRQ_MASK_3_irq_mask_3_0_START (0)
#define PMIC_IRQ_MASK_3_irq_mask_3_0_END (0)
#define PMIC_IRQ_MASK_3_irq_mask_3_1_START (1)
#define PMIC_IRQ_MASK_3_irq_mask_3_1_END (1)
#define PMIC_IRQ_MASK_3_irq_mask_3_2_START (2)
#define PMIC_IRQ_MASK_3_irq_mask_3_2_END (2)
#define PMIC_IRQ_MASK_3_irq_mask_3_3_START (3)
#define PMIC_IRQ_MASK_3_irq_mask_3_3_END (3)
#define PMIC_IRQ_MASK_3_irq_mask_3_4_START (4)
#define PMIC_IRQ_MASK_3_irq_mask_3_4_END (4)
#define PMIC_IRQ_MASK_3_irq_mask_3_5_START (5)
#define PMIC_IRQ_MASK_3_irq_mask_3_5_END (5)
#define PMIC_IRQ_MASK_3_irq_mask_3_6_START (6)
#define PMIC_IRQ_MASK_3_irq_mask_3_6_END (6)
#define PMIC_IRQ_MASK_3_irq_mask_3_7_START (7)
#define PMIC_IRQ_MASK_3_irq_mask_3_7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_4_0 : 1;
        unsigned char irq_mask_4_1 : 1;
        unsigned char irq_mask_4_2 : 1;
        unsigned char irq_mask_4_3 : 1;
        unsigned char irq_mask_4_4 : 1;
        unsigned char irq_mask_4_5 : 1;
        unsigned char irq_mask_4_6 : 1;
        unsigned char irq_mask_4_7 : 1;
    } reg;
} PMIC_IRQ_MASK_4_UNION;
#endif
#define PMIC_IRQ_MASK_4_irq_mask_4_0_START (0)
#define PMIC_IRQ_MASK_4_irq_mask_4_0_END (0)
#define PMIC_IRQ_MASK_4_irq_mask_4_1_START (1)
#define PMIC_IRQ_MASK_4_irq_mask_4_1_END (1)
#define PMIC_IRQ_MASK_4_irq_mask_4_2_START (2)
#define PMIC_IRQ_MASK_4_irq_mask_4_2_END (2)
#define PMIC_IRQ_MASK_4_irq_mask_4_3_START (3)
#define PMIC_IRQ_MASK_4_irq_mask_4_3_END (3)
#define PMIC_IRQ_MASK_4_irq_mask_4_4_START (4)
#define PMIC_IRQ_MASK_4_irq_mask_4_4_END (4)
#define PMIC_IRQ_MASK_4_irq_mask_4_5_START (5)
#define PMIC_IRQ_MASK_4_irq_mask_4_5_END (5)
#define PMIC_IRQ_MASK_4_irq_mask_4_6_START (6)
#define PMIC_IRQ_MASK_4_irq_mask_4_6_END (6)
#define PMIC_IRQ_MASK_4_irq_mask_4_7_START (7)
#define PMIC_IRQ_MASK_4_irq_mask_4_7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_5_0 : 1;
        unsigned char irq_mask_5_1 : 1;
        unsigned char irq_mask_5_2 : 1;
        unsigned char irq_mask_5_3 : 1;
        unsigned char irq_mask_5_4 : 1;
        unsigned char irq_mask_5_5 : 1;
        unsigned char irq_mask_5_6 : 1;
        unsigned char irq_mask_5_7 : 1;
    } reg;
} PMIC_IRQ_MASK_5_UNION;
#endif
#define PMIC_IRQ_MASK_5_irq_mask_5_0_START (0)
#define PMIC_IRQ_MASK_5_irq_mask_5_0_END (0)
#define PMIC_IRQ_MASK_5_irq_mask_5_1_START (1)
#define PMIC_IRQ_MASK_5_irq_mask_5_1_END (1)
#define PMIC_IRQ_MASK_5_irq_mask_5_2_START (2)
#define PMIC_IRQ_MASK_5_irq_mask_5_2_END (2)
#define PMIC_IRQ_MASK_5_irq_mask_5_3_START (3)
#define PMIC_IRQ_MASK_5_irq_mask_5_3_END (3)
#define PMIC_IRQ_MASK_5_irq_mask_5_4_START (4)
#define PMIC_IRQ_MASK_5_irq_mask_5_4_END (4)
#define PMIC_IRQ_MASK_5_irq_mask_5_5_START (5)
#define PMIC_IRQ_MASK_5_irq_mask_5_5_END (5)
#define PMIC_IRQ_MASK_5_irq_mask_5_6_START (6)
#define PMIC_IRQ_MASK_5_irq_mask_5_6_END (6)
#define PMIC_IRQ_MASK_5_irq_mask_5_7_START (7)
#define PMIC_IRQ_MASK_5_irq_mask_5_7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_6_0 : 1;
        unsigned char irq_mask_6_1 : 1;
        unsigned char irq_mask_6_2 : 1;
        unsigned char irq_mask_6_3 : 1;
        unsigned char irq_mask_6_4 : 1;
        unsigned char irq_mask_6_5 : 1;
        unsigned char irq_mask_6_6 : 1;
        unsigned char irq_mask_6_7 : 1;
    } reg;
} PMIC_IRQ_MASK_6_UNION;
#endif
#define PMIC_IRQ_MASK_6_irq_mask_6_0_START (0)
#define PMIC_IRQ_MASK_6_irq_mask_6_0_END (0)
#define PMIC_IRQ_MASK_6_irq_mask_6_1_START (1)
#define PMIC_IRQ_MASK_6_irq_mask_6_1_END (1)
#define PMIC_IRQ_MASK_6_irq_mask_6_2_START (2)
#define PMIC_IRQ_MASK_6_irq_mask_6_2_END (2)
#define PMIC_IRQ_MASK_6_irq_mask_6_3_START (3)
#define PMIC_IRQ_MASK_6_irq_mask_6_3_END (3)
#define PMIC_IRQ_MASK_6_irq_mask_6_4_START (4)
#define PMIC_IRQ_MASK_6_irq_mask_6_4_END (4)
#define PMIC_IRQ_MASK_6_irq_mask_6_5_START (5)
#define PMIC_IRQ_MASK_6_irq_mask_6_5_END (5)
#define PMIC_IRQ_MASK_6_irq_mask_6_6_START (6)
#define PMIC_IRQ_MASK_6_irq_mask_6_6_END (6)
#define PMIC_IRQ_MASK_6_irq_mask_6_7_START (7)
#define PMIC_IRQ_MASK_6_irq_mask_6_7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_7_0 : 1;
        unsigned char irq_mask_7_1 : 1;
        unsigned char irq_mask_7_2 : 1;
        unsigned char irq_mask_7_3 : 1;
        unsigned char irq_mask_7_4 : 1;
        unsigned char irq_mask_7_5 : 1;
        unsigned char irq_mask_7_6 : 1;
        unsigned char irq_mask_7_7 : 1;
    } reg;
} PMIC_IRQ_MASK_7_UNION;
#endif
#define PMIC_IRQ_MASK_7_irq_mask_7_0_START (0)
#define PMIC_IRQ_MASK_7_irq_mask_7_0_END (0)
#define PMIC_IRQ_MASK_7_irq_mask_7_1_START (1)
#define PMIC_IRQ_MASK_7_irq_mask_7_1_END (1)
#define PMIC_IRQ_MASK_7_irq_mask_7_2_START (2)
#define PMIC_IRQ_MASK_7_irq_mask_7_2_END (2)
#define PMIC_IRQ_MASK_7_irq_mask_7_3_START (3)
#define PMIC_IRQ_MASK_7_irq_mask_7_3_END (3)
#define PMIC_IRQ_MASK_7_irq_mask_7_4_START (4)
#define PMIC_IRQ_MASK_7_irq_mask_7_4_END (4)
#define PMIC_IRQ_MASK_7_irq_mask_7_5_START (5)
#define PMIC_IRQ_MASK_7_irq_mask_7_5_END (5)
#define PMIC_IRQ_MASK_7_irq_mask_7_6_START (6)
#define PMIC_IRQ_MASK_7_irq_mask_7_6_END (6)
#define PMIC_IRQ_MASK_7_irq_mask_7_7_START (7)
#define PMIC_IRQ_MASK_7_irq_mask_7_7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_8_0 : 1;
        unsigned char irq_mask_8_7_1 : 7;
    } reg;
} PMIC_IRQ_MASK_8_UNION;
#endif
#define PMIC_IRQ_MASK_8_irq_mask_8_0_START (0)
#define PMIC_IRQ_MASK_8_irq_mask_8_0_END (0)
#define PMIC_IRQ_MASK_8_irq_mask_8_7_1_START (1)
#define PMIC_IRQ_MASK_8_irq_mask_8_7_1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_9_0 : 1;
        unsigned char irq_mask_9_1 : 1;
        unsigned char irq_mask_9_2 : 1;
        unsigned char irq_mask_9_3 : 1;
        unsigned char irq_mask_9_4 : 1;
        unsigned char irq_mask_9_5 : 1;
        unsigned char irq_mask_9_6 : 1;
        unsigned char irq_mask_9_7 : 1;
    } reg;
} PMIC_IRQ_MASK_9_UNION;
#endif
#define PMIC_IRQ_MASK_9_irq_mask_9_0_START (0)
#define PMIC_IRQ_MASK_9_irq_mask_9_0_END (0)
#define PMIC_IRQ_MASK_9_irq_mask_9_1_START (1)
#define PMIC_IRQ_MASK_9_irq_mask_9_1_END (1)
#define PMIC_IRQ_MASK_9_irq_mask_9_2_START (2)
#define PMIC_IRQ_MASK_9_irq_mask_9_2_END (2)
#define PMIC_IRQ_MASK_9_irq_mask_9_3_START (3)
#define PMIC_IRQ_MASK_9_irq_mask_9_3_END (3)
#define PMIC_IRQ_MASK_9_irq_mask_9_4_START (4)
#define PMIC_IRQ_MASK_9_irq_mask_9_4_END (4)
#define PMIC_IRQ_MASK_9_irq_mask_9_5_START (5)
#define PMIC_IRQ_MASK_9_irq_mask_9_5_END (5)
#define PMIC_IRQ_MASK_9_irq_mask_9_6_START (6)
#define PMIC_IRQ_MASK_9_irq_mask_9_6_END (6)
#define PMIC_IRQ_MASK_9_irq_mask_9_7_START (7)
#define PMIC_IRQ_MASK_9_irq_mask_9_7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_10_0 : 1;
        unsigned char irq_mask_10_1 : 1;
        unsigned char irq_mask_10_2 : 1;
        unsigned char irq_mask_10_3 : 1;
        unsigned char irq_mask_10_7_4 : 4;
    } reg;
} PMIC_IRQ_MASK_10_UNION;
#endif
#define PMIC_IRQ_MASK_10_irq_mask_10_0_START (0)
#define PMIC_IRQ_MASK_10_irq_mask_10_0_END (0)
#define PMIC_IRQ_MASK_10_irq_mask_10_1_START (1)
#define PMIC_IRQ_MASK_10_irq_mask_10_1_END (1)
#define PMIC_IRQ_MASK_10_irq_mask_10_2_START (2)
#define PMIC_IRQ_MASK_10_irq_mask_10_2_END (2)
#define PMIC_IRQ_MASK_10_irq_mask_10_3_START (3)
#define PMIC_IRQ_MASK_10_irq_mask_10_3_END (3)
#define PMIC_IRQ_MASK_10_irq_mask_10_7_4_START (4)
#define PMIC_IRQ_MASK_10_irq_mask_10_7_4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_11_0 : 1;
        unsigned char irq_mask_11_1 : 1;
        unsigned char irq_mask_11_2 : 1;
        unsigned char irq_mask_11_3 : 1;
        unsigned char irq_mask_11_7_4 : 4;
    } reg;
} PMIC_IRQ_MASK_11_UNION;
#endif
#define PMIC_IRQ_MASK_11_irq_mask_11_0_START (0)
#define PMIC_IRQ_MASK_11_irq_mask_11_0_END (0)
#define PMIC_IRQ_MASK_11_irq_mask_11_1_START (1)
#define PMIC_IRQ_MASK_11_irq_mask_11_1_END (1)
#define PMIC_IRQ_MASK_11_irq_mask_11_2_START (2)
#define PMIC_IRQ_MASK_11_irq_mask_11_2_END (2)
#define PMIC_IRQ_MASK_11_irq_mask_11_3_START (3)
#define PMIC_IRQ_MASK_11_irq_mask_11_3_END (3)
#define PMIC_IRQ_MASK_11_irq_mask_11_7_4_START (4)
#define PMIC_IRQ_MASK_11_irq_mask_11_7_4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_12_0 : 1;
        unsigned char irq_mask_12_1 : 1;
        unsigned char irq_mask_12_2 : 1;
        unsigned char irq_mask_12_3 : 1;
        unsigned char irq_mask_12_7_4 : 4;
    } reg;
} PMIC_IRQ_MASK_12_UNION;
#endif
#define PMIC_IRQ_MASK_12_irq_mask_12_0_START (0)
#define PMIC_IRQ_MASK_12_irq_mask_12_0_END (0)
#define PMIC_IRQ_MASK_12_irq_mask_12_1_START (1)
#define PMIC_IRQ_MASK_12_irq_mask_12_1_END (1)
#define PMIC_IRQ_MASK_12_irq_mask_12_2_START (2)
#define PMIC_IRQ_MASK_12_irq_mask_12_2_END (2)
#define PMIC_IRQ_MASK_12_irq_mask_12_3_START (3)
#define PMIC_IRQ_MASK_12_irq_mask_12_3_END (3)
#define PMIC_IRQ_MASK_12_irq_mask_12_7_4_START (4)
#define PMIC_IRQ_MASK_12_irq_mask_12_7_4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_gpio_0 : 1;
        unsigned char irq_mask_gpio_1 : 1;
        unsigned char irq_mask_gpio_7_2 : 6;
    } reg;
} PMIC_IRQ_MASK_13_UNION;
#endif
#define PMIC_IRQ_MASK_13_irq_mask_gpio_0_START (0)
#define PMIC_IRQ_MASK_13_irq_mask_gpio_0_END (0)
#define PMIC_IRQ_MASK_13_irq_mask_gpio_1_START (1)
#define PMIC_IRQ_MASK_13_irq_mask_gpio_1_END (1)
#define PMIC_IRQ_MASK_13_irq_mask_gpio_7_2_START (2)
#define PMIC_IRQ_MASK_13_irq_mask_gpio_7_2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_voice_restart_0 : 1;
        unsigned char irq_mask_voice_restart_1 : 1;
        unsigned char irq_mask_voice_restart_2 : 1;
        unsigned char irq_mask_voice_restart_3 : 1;
        unsigned char irq_mask_voice_restart_7_4 : 4;
    } reg;
} PMIC_IRQ_MASK_14_UNION;
#endif
#define PMIC_IRQ_MASK_14_irq_mask_voice_restart_0_START (0)
#define PMIC_IRQ_MASK_14_irq_mask_voice_restart_0_END (0)
#define PMIC_IRQ_MASK_14_irq_mask_voice_restart_1_START (1)
#define PMIC_IRQ_MASK_14_irq_mask_voice_restart_1_END (1)
#define PMIC_IRQ_MASK_14_irq_mask_voice_restart_2_START (2)
#define PMIC_IRQ_MASK_14_irq_mask_voice_restart_2_END (2)
#define PMIC_IRQ_MASK_14_irq_mask_voice_restart_3_START (3)
#define PMIC_IRQ_MASK_14_irq_mask_voice_restart_3_END (3)
#define PMIC_IRQ_MASK_14_irq_mask_voice_restart_7_4_START (4)
#define PMIC_IRQ_MASK_14_irq_mask_voice_restart_7_4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_soh_0_0 : 1;
        unsigned char irq_mask_soh_0_1 : 1;
        unsigned char irq_mask_soh_0_2 : 1;
        unsigned char irq_mask_soh_0_3 : 1;
        unsigned char irq_mask_soh_0_4 : 1;
        unsigned char irq_mask_soh_0_5 : 1;
        unsigned char irq_mask_soh_0_6 : 1;
        unsigned char irq_mask_soh_0_7 : 1;
    } reg;
} PMIC_IRQ_MASK_15_UNION;
#endif
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_0_START (0)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_0_END (0)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_1_START (1)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_1_END (1)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_2_START (2)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_2_END (2)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_3_START (3)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_3_END (3)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_4_START (4)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_4_END (4)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_5_START (5)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_5_END (5)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_6_START (6)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_6_END (6)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_7_START (7)
#define PMIC_IRQ_MASK_15_irq_mask_soh_0_7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_dcr_0 : 1;
        unsigned char irq_mask_dcr_1 : 1;
        unsigned char irq_mask_dcr_2 : 1;
        unsigned char irq_mask_dcr_7_3 : 5;
    } reg;
} PMIC_IRQ_MASK_16_UNION;
#endif
#define PMIC_IRQ_MASK_16_irq_mask_dcr_0_START (0)
#define PMIC_IRQ_MASK_16_irq_mask_dcr_0_END (0)
#define PMIC_IRQ_MASK_16_irq_mask_dcr_1_START (1)
#define PMIC_IRQ_MASK_16_irq_mask_dcr_1_END (1)
#define PMIC_IRQ_MASK_16_irq_mask_dcr_2_START (2)
#define PMIC_IRQ_MASK_16_irq_mask_dcr_2_END (2)
#define PMIC_IRQ_MASK_16_irq_mask_dcr_7_3_START (3)
#define PMIC_IRQ_MASK_16_irq_mask_dcr_7_3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_eis_0 : 1;
        unsigned char irq_mask_eis_1 : 1;
        unsigned char irq_mask_eis_2 : 1;
        unsigned char irq_mask_eis_3 : 1;
        unsigned char irq_mask_eis_4 : 1;
        unsigned char irq_mask_eis_5 : 1;
        unsigned char irq_mask_eis_6 : 1;
        unsigned char irq_mask_eis_7 : 1;
    } reg;
} PMIC_IRQ_MASK_17_UNION;
#endif
#define PMIC_IRQ_MASK_17_irq_mask_eis_0_START (0)
#define PMIC_IRQ_MASK_17_irq_mask_eis_0_END (0)
#define PMIC_IRQ_MASK_17_irq_mask_eis_1_START (1)
#define PMIC_IRQ_MASK_17_irq_mask_eis_1_END (1)
#define PMIC_IRQ_MASK_17_irq_mask_eis_2_START (2)
#define PMIC_IRQ_MASK_17_irq_mask_eis_2_END (2)
#define PMIC_IRQ_MASK_17_irq_mask_eis_3_START (3)
#define PMIC_IRQ_MASK_17_irq_mask_eis_3_END (3)
#define PMIC_IRQ_MASK_17_irq_mask_eis_4_START (4)
#define PMIC_IRQ_MASK_17_irq_mask_eis_4_END (4)
#define PMIC_IRQ_MASK_17_irq_mask_eis_5_START (5)
#define PMIC_IRQ_MASK_17_irq_mask_eis_5_END (5)
#define PMIC_IRQ_MASK_17_irq_mask_eis_6_START (6)
#define PMIC_IRQ_MASK_17_irq_mask_eis_6_END (6)
#define PMIC_IRQ_MASK_17_irq_mask_eis_7_START (7)
#define PMIC_IRQ_MASK_17_irq_mask_eis_7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_ramp_0_0 : 1;
        unsigned char irq_mask_ramp_0_1 : 1;
        unsigned char irq_mask_ramp_0_2 : 1;
        unsigned char irq_mask_ramp_0_3 : 1;
        unsigned char irq_mask_ramp_0_4 : 1;
        unsigned char irq_mask_ramp_0_7_5 : 3;
    } reg;
} PMIC_IRQ_MASK_18_UNION;
#endif
#define PMIC_IRQ_MASK_18_irq_mask_ramp_0_0_START (0)
#define PMIC_IRQ_MASK_18_irq_mask_ramp_0_0_END (0)
#define PMIC_IRQ_MASK_18_irq_mask_ramp_0_1_START (1)
#define PMIC_IRQ_MASK_18_irq_mask_ramp_0_1_END (1)
#define PMIC_IRQ_MASK_18_irq_mask_ramp_0_2_START (2)
#define PMIC_IRQ_MASK_18_irq_mask_ramp_0_2_END (2)
#define PMIC_IRQ_MASK_18_irq_mask_ramp_0_3_START (3)
#define PMIC_IRQ_MASK_18_irq_mask_ramp_0_3_END (3)
#define PMIC_IRQ_MASK_18_irq_mask_ramp_0_4_START (4)
#define PMIC_IRQ_MASK_18_irq_mask_ramp_0_4_END (4)
#define PMIC_IRQ_MASK_18_irq_mask_ramp_0_7_5_START (5)
#define PMIC_IRQ_MASK_18_irq_mask_ramp_0_7_5_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_ramp_1_0 : 1;
        unsigned char irq_mask_ramp_1_1 : 1;
        unsigned char irq_mask_ramp_1_2 : 1;
        unsigned char irq_mask_ramp_1_3 : 1;
        unsigned char irq_mask_ramp_1_7_4 : 4;
    } reg;
} PMIC_IRQ_MASK_19_UNION;
#endif
#define PMIC_IRQ_MASK_19_irq_mask_ramp_1_0_START (0)
#define PMIC_IRQ_MASK_19_irq_mask_ramp_1_0_END (0)
#define PMIC_IRQ_MASK_19_irq_mask_ramp_1_1_START (1)
#define PMIC_IRQ_MASK_19_irq_mask_ramp_1_1_END (1)
#define PMIC_IRQ_MASK_19_irq_mask_ramp_1_2_START (2)
#define PMIC_IRQ_MASK_19_irq_mask_ramp_1_2_END (2)
#define PMIC_IRQ_MASK_19_irq_mask_ramp_1_3_START (3)
#define PMIC_IRQ_MASK_19_irq_mask_ramp_1_3_END (3)
#define PMIC_IRQ_MASK_19_irq_mask_ramp_1_7_4_START (4)
#define PMIC_IRQ_MASK_19_irq_mask_ramp_1_7_4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char irq_mask_vsys_drop_0 : 1;
        unsigned char irq_mask_vsys_drop_7_1 : 7;
    } reg;
} PMIC_IRQ_MASK_20_UNION;
#endif
#define PMIC_IRQ_MASK_20_irq_mask_vsys_drop_0_START (0)
#define PMIC_IRQ_MASK_20_irq_mask_vsys_drop_0_END (0)
#define PMIC_IRQ_MASK_20_irq_mask_vsys_drop_7_1_START (1)
#define PMIC_IRQ_MASK_20_irq_mask_vsys_drop_7_1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char thsd_otmp125_d1mr : 1;
        unsigned char vbus_det_insert_d20mr : 1;
        unsigned char vbus_det_insert_d20mf : 1;
        unsigned char alarmon_r : 1;
        unsigned char pwronn_d6sf : 1;
        unsigned char pwronn_d1sf : 1;
        unsigned char pwronn_d20mr : 1;
        unsigned char pwronn_d20mf : 1;
    } reg;
} PMIC_IRQ0_UNION;
#endif
#define PMIC_IRQ0_thsd_otmp125_d1mr_START (0)
#define PMIC_IRQ0_thsd_otmp125_d1mr_END (0)
#define PMIC_IRQ0_vbus_det_insert_d20mr_START (1)
#define PMIC_IRQ0_vbus_det_insert_d20mr_END (1)
#define PMIC_IRQ0_vbus_det_insert_d20mf_START (2)
#define PMIC_IRQ0_vbus_det_insert_d20mf_END (2)
#define PMIC_IRQ0_alarmon_r_START (3)
#define PMIC_IRQ0_alarmon_r_END (3)
#define PMIC_IRQ0_pwronn_d6sf_START (4)
#define PMIC_IRQ0_pwronn_d6sf_END (4)
#define PMIC_IRQ0_pwronn_d1sf_START (5)
#define PMIC_IRQ0_pwronn_d1sf_END (5)
#define PMIC_IRQ0_pwronn_d20mr_START (6)
#define PMIC_IRQ0_pwronn_d20mr_END (6)
#define PMIC_IRQ0_pwronn_d20mf_START (7)
#define PMIC_IRQ0_pwronn_d20mf_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ocp_scp_ovp_r : 1;
        unsigned char coul_r : 1;
        unsigned char sim0_hpd_r : 1;
        unsigned char sim0_hpd_f : 1;
        unsigned char sim1_hpd_r : 1;
        unsigned char sim1_hpd_f : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_IRQ1_UNION;
#endif
#define PMIC_IRQ1_ocp_scp_ovp_r_START (0)
#define PMIC_IRQ1_ocp_scp_ovp_r_END (0)
#define PMIC_IRQ1_coul_r_START (1)
#define PMIC_IRQ1_coul_r_END (1)
#define PMIC_IRQ1_sim0_hpd_r_START (2)
#define PMIC_IRQ1_sim0_hpd_r_END (2)
#define PMIC_IRQ1_sim0_hpd_f_START (3)
#define PMIC_IRQ1_sim0_hpd_f_END (3)
#define PMIC_IRQ1_sim1_hpd_r_START (4)
#define PMIC_IRQ1_sim1_hpd_r_END (4)
#define PMIC_IRQ1_sim1_hpd_f_START (5)
#define PMIC_IRQ1_sim1_hpd_f_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ocpbuck1 : 1;
        unsigned char ocpbuck2 : 1;
        unsigned char ocpbuck3 : 1;
        unsigned char ocpbuck40 : 1;
        unsigned char ocpbuck41 : 1;
        unsigned char ocpbuck42 : 1;
        unsigned char ocpbuck70 : 1;
        unsigned char ocpbuck71 : 1;
    } reg;
} PMIC_OCP_IRQ0_UNION;
#endif
#define PMIC_OCP_IRQ0_ocpbuck1_START (0)
#define PMIC_OCP_IRQ0_ocpbuck1_END (0)
#define PMIC_OCP_IRQ0_ocpbuck2_START (1)
#define PMIC_OCP_IRQ0_ocpbuck2_END (1)
#define PMIC_OCP_IRQ0_ocpbuck3_START (2)
#define PMIC_OCP_IRQ0_ocpbuck3_END (2)
#define PMIC_OCP_IRQ0_ocpbuck40_START (3)
#define PMIC_OCP_IRQ0_ocpbuck40_END (3)
#define PMIC_OCP_IRQ0_ocpbuck41_START (4)
#define PMIC_OCP_IRQ0_ocpbuck41_END (4)
#define PMIC_OCP_IRQ0_ocpbuck42_START (5)
#define PMIC_OCP_IRQ0_ocpbuck42_END (5)
#define PMIC_OCP_IRQ0_ocpbuck70_START (6)
#define PMIC_OCP_IRQ0_ocpbuck70_END (6)
#define PMIC_OCP_IRQ0_ocpbuck71_START (7)
#define PMIC_OCP_IRQ0_ocpbuck71_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ocpldo6 : 1;
        unsigned char ocpldo4 : 1;
        unsigned char ocpldo3 : 1;
        unsigned char ocpldo2 : 1;
        unsigned char ocpldo1 : 1;
        unsigned char ocpldo0 : 1;
        unsigned char ocpbuck5 : 1;
        unsigned char ocpbuck9 : 1;
    } reg;
} PMIC_OCP_IRQ1_UNION;
#endif
#define PMIC_OCP_IRQ1_ocpldo6_START (0)
#define PMIC_OCP_IRQ1_ocpldo6_END (0)
#define PMIC_OCP_IRQ1_ocpldo4_START (1)
#define PMIC_OCP_IRQ1_ocpldo4_END (1)
#define PMIC_OCP_IRQ1_ocpldo3_START (2)
#define PMIC_OCP_IRQ1_ocpldo3_END (2)
#define PMIC_OCP_IRQ1_ocpldo2_START (3)
#define PMIC_OCP_IRQ1_ocpldo2_END (3)
#define PMIC_OCP_IRQ1_ocpldo1_START (4)
#define PMIC_OCP_IRQ1_ocpldo1_END (4)
#define PMIC_OCP_IRQ1_ocpldo0_START (5)
#define PMIC_OCP_IRQ1_ocpldo0_END (5)
#define PMIC_OCP_IRQ1_ocpbuck5_START (6)
#define PMIC_OCP_IRQ1_ocpbuck5_END (6)
#define PMIC_OCP_IRQ1_ocpbuck9_START (7)
#define PMIC_OCP_IRQ1_ocpbuck9_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ocpldo18 : 1;
        unsigned char ocpldo17 : 1;
        unsigned char ocpldo16 : 1;
        unsigned char ocpldo15 : 1;
        unsigned char ocpldo14 : 1;
        unsigned char ocpldo12 : 1;
        unsigned char ocpldo11 : 1;
        unsigned char ocpldo8 : 1;
    } reg;
} PMIC_OCP_IRQ2_UNION;
#endif
#define PMIC_OCP_IRQ2_ocpldo18_START (0)
#define PMIC_OCP_IRQ2_ocpldo18_END (0)
#define PMIC_OCP_IRQ2_ocpldo17_START (1)
#define PMIC_OCP_IRQ2_ocpldo17_END (1)
#define PMIC_OCP_IRQ2_ocpldo16_START (2)
#define PMIC_OCP_IRQ2_ocpldo16_END (2)
#define PMIC_OCP_IRQ2_ocpldo15_START (3)
#define PMIC_OCP_IRQ2_ocpldo15_END (3)
#define PMIC_OCP_IRQ2_ocpldo14_START (4)
#define PMIC_OCP_IRQ2_ocpldo14_END (4)
#define PMIC_OCP_IRQ2_ocpldo12_START (5)
#define PMIC_OCP_IRQ2_ocpldo12_END (5)
#define PMIC_OCP_IRQ2_ocpldo11_START (6)
#define PMIC_OCP_IRQ2_ocpldo11_END (6)
#define PMIC_OCP_IRQ2_ocpldo8_START (7)
#define PMIC_OCP_IRQ2_ocpldo8_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ocpldo28 : 1;
        unsigned char ocpldo27 : 1;
        unsigned char ocpldo26 : 1;
        unsigned char ocpldo25 : 1;
        unsigned char ocpldo24 : 1;
        unsigned char ocpldo23 : 1;
        unsigned char ocpldo22 : 1;
        unsigned char ocpldo21 : 1;
    } reg;
} PMIC_OCP_IRQ3_UNION;
#endif
#define PMIC_OCP_IRQ3_ocpldo28_START (0)
#define PMIC_OCP_IRQ3_ocpldo28_END (0)
#define PMIC_OCP_IRQ3_ocpldo27_START (1)
#define PMIC_OCP_IRQ3_ocpldo27_END (1)
#define PMIC_OCP_IRQ3_ocpldo26_START (2)
#define PMIC_OCP_IRQ3_ocpldo26_END (2)
#define PMIC_OCP_IRQ3_ocpldo25_START (3)
#define PMIC_OCP_IRQ3_ocpldo25_END (3)
#define PMIC_OCP_IRQ3_ocpldo24_START (4)
#define PMIC_OCP_IRQ3_ocpldo24_END (4)
#define PMIC_OCP_IRQ3_ocpldo23_START (5)
#define PMIC_OCP_IRQ3_ocpldo23_END (5)
#define PMIC_OCP_IRQ3_ocpldo22_START (6)
#define PMIC_OCP_IRQ3_ocpldo22_END (6)
#define PMIC_OCP_IRQ3_ocpldo21_START (7)
#define PMIC_OCP_IRQ3_ocpldo21_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ocpldo39 : 1;
        unsigned char ocpldo38 : 1;
        unsigned char ocpldo37 : 1;
        unsigned char ocpldo36 : 1;
        unsigned char pmuh_ocp : 1;
        unsigned char ocpldo32 : 1;
        unsigned char ocpldo30 : 1;
        unsigned char ocpldo29 : 1;
    } reg;
} PMIC_OCP_IRQ4_UNION;
#endif
#define PMIC_OCP_IRQ4_ocpldo39_START (0)
#define PMIC_OCP_IRQ4_ocpldo39_END (0)
#define PMIC_OCP_IRQ4_ocpldo38_START (1)
#define PMIC_OCP_IRQ4_ocpldo38_END (1)
#define PMIC_OCP_IRQ4_ocpldo37_START (2)
#define PMIC_OCP_IRQ4_ocpldo37_END (2)
#define PMIC_OCP_IRQ4_ocpldo36_START (3)
#define PMIC_OCP_IRQ4_ocpldo36_END (3)
#define PMIC_OCP_IRQ4_pmuh_ocp_START (4)
#define PMIC_OCP_IRQ4_pmuh_ocp_END (4)
#define PMIC_OCP_IRQ4_ocpldo32_START (5)
#define PMIC_OCP_IRQ4_ocpldo32_END (5)
#define PMIC_OCP_IRQ4_ocpldo30_START (6)
#define PMIC_OCP_IRQ4_ocpldo30_END (6)
#define PMIC_OCP_IRQ4_ocpldo29_START (7)
#define PMIC_OCP_IRQ4_ocpldo29_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sw1_ocp : 1;
        unsigned char ocpldo46 : 1;
        unsigned char ocpldo45 : 1;
        unsigned char ocpldo44 : 1;
        unsigned char ocpldo43 : 1;
        unsigned char ocpldo42 : 1;
        unsigned char ocpldo41 : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_OCP_IRQ5_UNION;
#endif
#define PMIC_OCP_IRQ5_sw1_ocp_START (0)
#define PMIC_OCP_IRQ5_sw1_ocp_END (0)
#define PMIC_OCP_IRQ5_ocpldo46_START (1)
#define PMIC_OCP_IRQ5_ocpldo46_END (1)
#define PMIC_OCP_IRQ5_ocpldo45_START (2)
#define PMIC_OCP_IRQ5_ocpldo45_END (2)
#define PMIC_OCP_IRQ5_ocpldo44_START (3)
#define PMIC_OCP_IRQ5_ocpldo44_END (3)
#define PMIC_OCP_IRQ5_ocpldo43_START (4)
#define PMIC_OCP_IRQ5_ocpldo43_END (4)
#define PMIC_OCP_IRQ5_ocpldo42_START (5)
#define PMIC_OCP_IRQ5_ocpldo42_END (5)
#define PMIC_OCP_IRQ5_ocpldo41_START (6)
#define PMIC_OCP_IRQ5_ocpldo41_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_sink_ocp : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_OCP_IRQ6_UNION;
#endif
#define PMIC_OCP_IRQ6_ldo_sink_ocp_START (0)
#define PMIC_OCP_IRQ6_ldo_sink_ocp_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck1_scp : 1;
        unsigned char buck2_scp : 1;
        unsigned char buck3_scp : 1;
        unsigned char buck401_scp : 1;
        unsigned char buck42_scp : 1;
        unsigned char buck5_scp : 1;
        unsigned char buck70_scp : 1;
        unsigned char buck71_scp : 1;
    } reg;
} PMIC_SCP_IRQ0_UNION;
#endif
#define PMIC_SCP_IRQ0_buck1_scp_START (0)
#define PMIC_SCP_IRQ0_buck1_scp_END (0)
#define PMIC_SCP_IRQ0_buck2_scp_START (1)
#define PMIC_SCP_IRQ0_buck2_scp_END (1)
#define PMIC_SCP_IRQ0_buck3_scp_START (2)
#define PMIC_SCP_IRQ0_buck3_scp_END (2)
#define PMIC_SCP_IRQ0_buck401_scp_START (3)
#define PMIC_SCP_IRQ0_buck401_scp_END (3)
#define PMIC_SCP_IRQ0_buck42_scp_START (4)
#define PMIC_SCP_IRQ0_buck42_scp_END (4)
#define PMIC_SCP_IRQ0_buck5_scp_START (5)
#define PMIC_SCP_IRQ0_buck5_scp_END (5)
#define PMIC_SCP_IRQ0_buck70_scp_START (6)
#define PMIC_SCP_IRQ0_buck70_scp_END (6)
#define PMIC_SCP_IRQ0_buck71_scp_START (7)
#define PMIC_SCP_IRQ0_buck71_scp_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck9_scp : 1;
        unsigned char ldo_buff_scp : 1;
        unsigned char discharge_ocp : 1;
        unsigned char ldo_sink_scp : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_SCP_IRQ1_UNION;
#endif
#define PMIC_SCP_IRQ1_buck9_scp_START (0)
#define PMIC_SCP_IRQ1_buck9_scp_END (0)
#define PMIC_SCP_IRQ1_ldo_buff_scp_START (1)
#define PMIC_SCP_IRQ1_ldo_buff_scp_END (1)
#define PMIC_SCP_IRQ1_discharge_ocp_START (2)
#define PMIC_SCP_IRQ1_discharge_ocp_END (2)
#define PMIC_SCP_IRQ1_ldo_sink_scp_START (3)
#define PMIC_SCP_IRQ1_ldo_sink_scp_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck401_ovp : 1;
        unsigned char buck42_ovp : 1;
        unsigned char buck70_ovp : 1;
        unsigned char buck71_ovp : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_OVP_IRQ0_UNION;
#endif
#define PMIC_OVP_IRQ0_buck401_ovp_START (0)
#define PMIC_OVP_IRQ0_buck401_ovp_END (0)
#define PMIC_OVP_IRQ0_buck42_ovp_START (1)
#define PMIC_OVP_IRQ0_buck42_ovp_END (1)
#define PMIC_OVP_IRQ0_buck70_ovp_START (2)
#define PMIC_OVP_IRQ0_buck70_ovp_END (2)
#define PMIC_OVP_IRQ0_buck71_ovp_START (3)
#define PMIC_OVP_IRQ0_buck71_ovp_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck401_cur_det : 1;
        unsigned char buck42_cur_det : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_CUR_DET_IRQ0_UNION;
#endif
#define PMIC_CUR_DET_IRQ0_buck401_cur_det_START (0)
#define PMIC_CUR_DET_IRQ0_buck401_cur_det_END (0)
#define PMIC_CUR_DET_IRQ0_buck42_cur_det_START (1)
#define PMIC_CUR_DET_IRQ0_buck42_cur_det_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char gpio0_int_status : 1;
        unsigned char gpio1_int_status : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_GPIO_IRQ_UNION;
#endif
#define PMIC_GPIO_IRQ_gpio0_int_status_START (0)
#define PMIC_GPIO_IRQ_gpio0_int_status_END (0)
#define PMIC_GPIO_IRQ_gpio1_int_status_START (1)
#define PMIC_GPIO_IRQ_gpio1_int_status_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char voice_restart1_r : 1;
        unsigned char voice_restart1_f : 1;
        unsigned char voice_restart2_r : 1;
        unsigned char voice_restart2_f : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_VIOCE_RESTART_IRQ_UNION;
#endif
#define PMIC_VIOCE_RESTART_IRQ_voice_restart1_r_START (0)
#define PMIC_VIOCE_RESTART_IRQ_voice_restart1_r_END (0)
#define PMIC_VIOCE_RESTART_IRQ_voice_restart1_f_START (1)
#define PMIC_VIOCE_RESTART_IRQ_voice_restart1_f_END (1)
#define PMIC_VIOCE_RESTART_IRQ_voice_restart2_r_START (2)
#define PMIC_VIOCE_RESTART_IRQ_voice_restart2_r_END (2)
#define PMIC_VIOCE_RESTART_IRQ_voice_restart2_f_START (3)
#define PMIC_VIOCE_RESTART_IRQ_voice_restart2_f_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_flag : 1;
        unsigned char soh_ovp_end : 1;
        unsigned char soh_tbat_tmp_ov : 1;
        unsigned char soh_mode_abn : 1;
        unsigned char soh_vbat_uv : 1;
        unsigned char soh_otmp125 : 1;
        unsigned char soh_30min_timeout : 1;
        unsigned char soh_ovp : 1;
    } reg;
} PMIC_ACR_SOH_IRQ_UNION;
#endif
#define PMIC_ACR_SOH_IRQ_acr_flag_START (0)
#define PMIC_ACR_SOH_IRQ_acr_flag_END (0)
#define PMIC_ACR_SOH_IRQ_soh_ovp_end_START (1)
#define PMIC_ACR_SOH_IRQ_soh_ovp_end_END (1)
#define PMIC_ACR_SOH_IRQ_soh_tbat_tmp_ov_START (2)
#define PMIC_ACR_SOH_IRQ_soh_tbat_tmp_ov_END (2)
#define PMIC_ACR_SOH_IRQ_soh_mode_abn_START (3)
#define PMIC_ACR_SOH_IRQ_soh_mode_abn_END (3)
#define PMIC_ACR_SOH_IRQ_soh_vbat_uv_START (4)
#define PMIC_ACR_SOH_IRQ_soh_vbat_uv_END (4)
#define PMIC_ACR_SOH_IRQ_soh_otmp125_START (5)
#define PMIC_ACR_SOH_IRQ_soh_otmp125_END (5)
#define PMIC_ACR_SOH_IRQ_soh_30min_timeout_START (6)
#define PMIC_ACR_SOH_IRQ_soh_30min_timeout_END (6)
#define PMIC_ACR_SOH_IRQ_soh_ovp_START (7)
#define PMIC_ACR_SOH_IRQ_soh_ovp_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dcr_end : 1;
        unsigned char dcr_done2 : 1;
        unsigned char dcr_done1 : 1;
        unsigned char dcr_err2 : 1;
        unsigned char dcr_err1 : 1;
        unsigned char dcr_tout2 : 1;
        unsigned char dcr_tout1 : 1;
        unsigned char dcr_tout0 : 1;
    } reg;
} PMIC_DCR_IRQ_UNION;
#endif
#define PMIC_DCR_IRQ_dcr_end_START (0)
#define PMIC_DCR_IRQ_dcr_end_END (0)
#define PMIC_DCR_IRQ_dcr_done2_START (1)
#define PMIC_DCR_IRQ_dcr_done2_END (1)
#define PMIC_DCR_IRQ_dcr_done1_START (2)
#define PMIC_DCR_IRQ_dcr_done1_END (2)
#define PMIC_DCR_IRQ_dcr_err2_START (3)
#define PMIC_DCR_IRQ_dcr_err2_END (3)
#define PMIC_DCR_IRQ_dcr_err1_START (4)
#define PMIC_DCR_IRQ_dcr_err1_END (4)
#define PMIC_DCR_IRQ_dcr_tout2_START (5)
#define PMIC_DCR_IRQ_dcr_tout2_END (5)
#define PMIC_DCR_IRQ_dcr_tout1_START (6)
#define PMIC_DCR_IRQ_dcr_tout1_END (6)
#define PMIC_DCR_IRQ_dcr_tout0_START (7)
#define PMIC_DCR_IRQ_dcr_tout0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_start : 1;
        unsigned char eis_end : 1;
        unsigned char eis_int1 : 1;
        unsigned char eis_int2 : 1;
        unsigned char eis_int3 : 1;
        unsigned char eis_int4 : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_EIS_IRQ_UNION;
#endif
#define PMIC_EIS_IRQ_eis_start_START (0)
#define PMIC_EIS_IRQ_eis_start_END (0)
#define PMIC_EIS_IRQ_eis_end_START (1)
#define PMIC_EIS_IRQ_eis_end_END (1)
#define PMIC_EIS_IRQ_eis_int1_START (2)
#define PMIC_EIS_IRQ_eis_int1_END (2)
#define PMIC_EIS_IRQ_eis_int2_START (3)
#define PMIC_EIS_IRQ_eis_int2_END (3)
#define PMIC_EIS_IRQ_eis_int3_START (4)
#define PMIC_EIS_IRQ_eis_int3_END (4)
#define PMIC_EIS_IRQ_eis_int4_START (5)
#define PMIC_EIS_IRQ_eis_int4_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_ramp_irq : 1;
        unsigned char b42_ramp_irq : 1;
        unsigned char b5_ramp_irq : 1;
        unsigned char b70_ramp_irq : 1;
        unsigned char b71_ramp_irq : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_RAMP_IRQ0_UNION;
#endif
#define PMIC_RAMP_IRQ0_b401_ramp_irq_START (0)
#define PMIC_RAMP_IRQ0_b401_ramp_irq_END (0)
#define PMIC_RAMP_IRQ0_b42_ramp_irq_START (1)
#define PMIC_RAMP_IRQ0_b42_ramp_irq_END (1)
#define PMIC_RAMP_IRQ0_b5_ramp_irq_START (2)
#define PMIC_RAMP_IRQ0_b5_ramp_irq_END (2)
#define PMIC_RAMP_IRQ0_b70_ramp_irq_START (3)
#define PMIC_RAMP_IRQ0_b70_ramp_irq_END (3)
#define PMIC_RAMP_IRQ0_b71_ramp_irq_START (4)
#define PMIC_RAMP_IRQ0_b71_ramp_irq_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo0_ramp_irq : 1;
        unsigned char ldo39_ramp_irq : 1;
        unsigned char ldo44_ramp_irq : 1;
        unsigned char ldo45_ramp_irq : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_RAMP_IRQ1_UNION;
#endif
#define PMIC_RAMP_IRQ1_ldo0_ramp_irq_START (0)
#define PMIC_RAMP_IRQ1_ldo0_ramp_irq_END (0)
#define PMIC_RAMP_IRQ1_ldo39_ramp_irq_START (1)
#define PMIC_RAMP_IRQ1_ldo39_ramp_irq_END (1)
#define PMIC_RAMP_IRQ1_ldo44_ramp_irq_START (2)
#define PMIC_RAMP_IRQ1_ldo44_ramp_irq_END (2)
#define PMIC_RAMP_IRQ1_ldo45_ramp_irq_START (3)
#define PMIC_RAMP_IRQ1_ldo45_ramp_irq_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char vsys_drop_r : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_VSYS_DROP_IRQ_UNION;
#endif
#define PMIC_VSYS_DROP_IRQ_vsys_drop_r_START (0)
#define PMIC_VSYS_DROP_IRQ_vsys_drop_r_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_vsys_ov_d200ur : 1;
        unsigned char np_vsys_pwroff_abs_d20nr : 1;
        unsigned char np_vsys_pwroff_deb_d80mr : 1;
        unsigned char np_thsd_otmp140_d1mr : 1;
        unsigned char np_thsd_otmp125_d1mr : 1;
        unsigned char np_hresetn_d90uf : 1;
        unsigned char np_avdd_osc_vld_d20nf : 1;
        unsigned char np_19m2_dis : 1;
    } reg;
} PMIC_NP_RECORD0_UNION;
#endif
#define PMIC_NP_RECORD0_np_vsys_ov_d200ur_START (0)
#define PMIC_NP_RECORD0_np_vsys_ov_d200ur_END (0)
#define PMIC_NP_RECORD0_np_vsys_pwroff_abs_d20nr_START (1)
#define PMIC_NP_RECORD0_np_vsys_pwroff_abs_d20nr_END (1)
#define PMIC_NP_RECORD0_np_vsys_pwroff_deb_d80mr_START (2)
#define PMIC_NP_RECORD0_np_vsys_pwroff_deb_d80mr_END (2)
#define PMIC_NP_RECORD0_np_thsd_otmp140_d1mr_START (3)
#define PMIC_NP_RECORD0_np_thsd_otmp140_d1mr_END (3)
#define PMIC_NP_RECORD0_np_thsd_otmp125_d1mr_START (4)
#define PMIC_NP_RECORD0_np_thsd_otmp125_d1mr_END (4)
#define PMIC_NP_RECORD0_np_hresetn_d90uf_START (5)
#define PMIC_NP_RECORD0_np_hresetn_d90uf_END (5)
#define PMIC_NP_RECORD0_np_avdd_osc_vld_d20nf_START (6)
#define PMIC_NP_RECORD0_np_avdd_osc_vld_d20nf_END (6)
#define PMIC_NP_RECORD0_np_19m2_dis_START (7)
#define PMIC_NP_RECORD0_np_19m2_dis_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_pwronn_restart : 1;
        unsigned char np_pwrhold_shutdown : 1;
        unsigned char np_pwronn_shutdown : 1;
        unsigned char np_pwrhold_pwrup : 1;
        unsigned char np_alarmon_pwrup : 1;
        unsigned char np_vbus_pwrup : 1;
        unsigned char np_pwronn_pwrup : 1;
        unsigned char np_fast_pwrup : 1;
    } reg;
} PMIC_NP_RECORD1_UNION;
#endif
#define PMIC_NP_RECORD1_np_pwronn_restart_START (0)
#define PMIC_NP_RECORD1_np_pwronn_restart_END (0)
#define PMIC_NP_RECORD1_np_pwrhold_shutdown_START (1)
#define PMIC_NP_RECORD1_np_pwrhold_shutdown_END (1)
#define PMIC_NP_RECORD1_np_pwronn_shutdown_START (2)
#define PMIC_NP_RECORD1_np_pwronn_shutdown_END (2)
#define PMIC_NP_RECORD1_np_pwrhold_pwrup_START (3)
#define PMIC_NP_RECORD1_np_pwrhold_pwrup_END (3)
#define PMIC_NP_RECORD1_np_alarmon_pwrup_START (4)
#define PMIC_NP_RECORD1_np_alarmon_pwrup_END (4)
#define PMIC_NP_RECORD1_np_vbus_pwrup_START (5)
#define PMIC_NP_RECORD1_np_vbus_pwrup_END (5)
#define PMIC_NP_RECORD1_np_pwronn_pwrup_START (6)
#define PMIC_NP_RECORD1_np_pwronn_pwrup_END (6)
#define PMIC_NP_RECORD1_np_fast_pwrup_START (7)
#define PMIC_NP_RECORD1_np_fast_pwrup_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_dcxo_clk_sel_r : 1;
        unsigned char np_dcxo_clk_sel_f : 1;
        unsigned char np_vsys_vcoin_sel : 1;
        unsigned char np_smpl : 1;
        unsigned char np_core_io18_vld_f : 1;
        unsigned char np_pwrhold_4s : 1;
        unsigned char np_pwron_n_hrst : 1;
        unsigned char np_nfc_pwrup : 1;
    } reg;
} PMIC_NP_RECORD2_UNION;
#endif
#define PMIC_NP_RECORD2_np_dcxo_clk_sel_r_START (0)
#define PMIC_NP_RECORD2_np_dcxo_clk_sel_r_END (0)
#define PMIC_NP_RECORD2_np_dcxo_clk_sel_f_START (1)
#define PMIC_NP_RECORD2_np_dcxo_clk_sel_f_END (1)
#define PMIC_NP_RECORD2_np_vsys_vcoin_sel_START (2)
#define PMIC_NP_RECORD2_np_vsys_vcoin_sel_END (2)
#define PMIC_NP_RECORD2_np_smpl_START (3)
#define PMIC_NP_RECORD2_np_smpl_END (3)
#define PMIC_NP_RECORD2_np_core_io18_vld_f_START (4)
#define PMIC_NP_RECORD2_np_core_io18_vld_f_END (4)
#define PMIC_NP_RECORD2_np_pwrhold_4s_START (5)
#define PMIC_NP_RECORD2_np_pwrhold_4s_END (5)
#define PMIC_NP_RECORD2_np_pwron_n_hrst_START (6)
#define PMIC_NP_RECORD2_np_pwron_n_hrst_END (6)
#define PMIC_NP_RECORD2_np_nfc_pwrup_START (7)
#define PMIC_NP_RECORD2_np_nfc_pwrup_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_pmud_short_f : 1;
        unsigned char np_pmuh_short_f : 1;
        unsigned char np_vin_ldoh_shutdown : 1;
        unsigned char np_vsys_pwron_shutdown : 1;
        unsigned char np_nfc_shutdown : 1;
        unsigned char np_core_io12_18_vld_f : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_NP_RECORD3_UNION;
#endif
#define PMIC_NP_RECORD3_np_pmud_short_f_START (0)
#define PMIC_NP_RECORD3_np_pmud_short_f_END (0)
#define PMIC_NP_RECORD3_np_pmuh_short_f_START (1)
#define PMIC_NP_RECORD3_np_pmuh_short_f_END (1)
#define PMIC_NP_RECORD3_np_vin_ldoh_shutdown_START (2)
#define PMIC_NP_RECORD3_np_vin_ldoh_shutdown_END (2)
#define PMIC_NP_RECORD3_np_vsys_pwron_shutdown_START (3)
#define PMIC_NP_RECORD3_np_vsys_pwron_shutdown_END (3)
#define PMIC_NP_RECORD3_np_nfc_shutdown_START (4)
#define PMIC_NP_RECORD3_np_nfc_shutdown_END (4)
#define PMIC_NP_RECORD3_np_core_io12_18_vld_f_START (5)
#define PMIC_NP_RECORD3_np_core_io12_18_vld_f_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_cali_pmuh_ocp : 1;
        unsigned char np_cali_ldo26_ocp : 1;
        unsigned char np_cali_buck2_scp : 1;
        unsigned char np_cali_buck2_ocp : 1;
        unsigned char np_cali_pmuh_short : 1;
        unsigned char np_cali_pmud_short : 1;
        unsigned char np_cali_vsys_pwroff_deb : 1;
        unsigned char np_cali_vsys_pwroff_abs : 1;
    } reg;
} PMIC_NP_RECORD4_UNION;
#endif
#define PMIC_NP_RECORD4_np_cali_pmuh_ocp_START (0)
#define PMIC_NP_RECORD4_np_cali_pmuh_ocp_END (0)
#define PMIC_NP_RECORD4_np_cali_ldo26_ocp_START (1)
#define PMIC_NP_RECORD4_np_cali_ldo26_ocp_END (1)
#define PMIC_NP_RECORD4_np_cali_buck2_scp_START (2)
#define PMIC_NP_RECORD4_np_cali_buck2_scp_END (2)
#define PMIC_NP_RECORD4_np_cali_buck2_ocp_START (3)
#define PMIC_NP_RECORD4_np_cali_buck2_ocp_END (3)
#define PMIC_NP_RECORD4_np_cali_pmuh_short_START (4)
#define PMIC_NP_RECORD4_np_cali_pmuh_short_END (4)
#define PMIC_NP_RECORD4_np_cali_pmud_short_START (5)
#define PMIC_NP_RECORD4_np_cali_pmud_short_END (5)
#define PMIC_NP_RECORD4_np_cali_vsys_pwroff_deb_START (6)
#define PMIC_NP_RECORD4_np_cali_vsys_pwroff_deb_END (6)
#define PMIC_NP_RECORD4_np_cali_vsys_pwroff_abs_START (7)
#define PMIC_NP_RECORD4_np_cali_vsys_pwroff_abs_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_cali_avdd_osc_vld : 1;
        unsigned char np_cali_thsd_otmp140 : 1;
        unsigned char np_cali_thsd_otmp125 : 1;
        unsigned char np_cali_vsys_ov : 1;
        unsigned char np_cali_19m2_dis : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_RECORD5_UNION;
#endif
#define PMIC_NP_RECORD5_np_cali_avdd_osc_vld_START (0)
#define PMIC_NP_RECORD5_np_cali_avdd_osc_vld_END (0)
#define PMIC_NP_RECORD5_np_cali_thsd_otmp140_START (1)
#define PMIC_NP_RECORD5_np_cali_thsd_otmp140_END (1)
#define PMIC_NP_RECORD5_np_cali_thsd_otmp125_START (2)
#define PMIC_NP_RECORD5_np_cali_thsd_otmp125_END (2)
#define PMIC_NP_RECORD5_np_cali_vsys_ov_START (3)
#define PMIC_NP_RECORD5_np_cali_vsys_ov_END (3)
#define PMIC_NP_RECORD5_np_cali_19m2_dis_START (4)
#define PMIC_NP_RECORD5_np_cali_19m2_dis_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ocpbuck1 : 1;
        unsigned char np_ocpbuck2 : 1;
        unsigned char np_ocpbuck3 : 1;
        unsigned char np_ocpbuck40 : 1;
        unsigned char np_ocpbuck41 : 1;
        unsigned char np_ocpbuck42 : 1;
        unsigned char np_ocpbuck70 : 1;
        unsigned char np_ocpbuck71 : 1;
    } reg;
} PMIC_NP_OCP0_UNION;
#endif
#define PMIC_NP_OCP0_np_ocpbuck1_START (0)
#define PMIC_NP_OCP0_np_ocpbuck1_END (0)
#define PMIC_NP_OCP0_np_ocpbuck2_START (1)
#define PMIC_NP_OCP0_np_ocpbuck2_END (1)
#define PMIC_NP_OCP0_np_ocpbuck3_START (2)
#define PMIC_NP_OCP0_np_ocpbuck3_END (2)
#define PMIC_NP_OCP0_np_ocpbuck40_START (3)
#define PMIC_NP_OCP0_np_ocpbuck40_END (3)
#define PMIC_NP_OCP0_np_ocpbuck41_START (4)
#define PMIC_NP_OCP0_np_ocpbuck41_END (4)
#define PMIC_NP_OCP0_np_ocpbuck42_START (5)
#define PMIC_NP_OCP0_np_ocpbuck42_END (5)
#define PMIC_NP_OCP0_np_ocpbuck70_START (6)
#define PMIC_NP_OCP0_np_ocpbuck70_END (6)
#define PMIC_NP_OCP0_np_ocpbuck71_START (7)
#define PMIC_NP_OCP0_np_ocpbuck71_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ocpldo6 : 1;
        unsigned char np_ocpldo4 : 1;
        unsigned char np_ocpldo3 : 1;
        unsigned char np_ocpldo2 : 1;
        unsigned char np_ocpldo1 : 1;
        unsigned char np_ocpldo0 : 1;
        unsigned char np_ocpbuck5 : 1;
        unsigned char np_ocpbuck9 : 1;
    } reg;
} PMIC_NP_OCP1_UNION;
#endif
#define PMIC_NP_OCP1_np_ocpldo6_START (0)
#define PMIC_NP_OCP1_np_ocpldo6_END (0)
#define PMIC_NP_OCP1_np_ocpldo4_START (1)
#define PMIC_NP_OCP1_np_ocpldo4_END (1)
#define PMIC_NP_OCP1_np_ocpldo3_START (2)
#define PMIC_NP_OCP1_np_ocpldo3_END (2)
#define PMIC_NP_OCP1_np_ocpldo2_START (3)
#define PMIC_NP_OCP1_np_ocpldo2_END (3)
#define PMIC_NP_OCP1_np_ocpldo1_START (4)
#define PMIC_NP_OCP1_np_ocpldo1_END (4)
#define PMIC_NP_OCP1_np_ocpldo0_START (5)
#define PMIC_NP_OCP1_np_ocpldo0_END (5)
#define PMIC_NP_OCP1_np_ocpbuck5_START (6)
#define PMIC_NP_OCP1_np_ocpbuck5_END (6)
#define PMIC_NP_OCP1_np_ocpbuck9_START (7)
#define PMIC_NP_OCP1_np_ocpbuck9_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ocpldo18 : 1;
        unsigned char np_ocpldo17 : 1;
        unsigned char np_ocpldo16 : 1;
        unsigned char np_ocpldo15 : 1;
        unsigned char np_ocpldo14 : 1;
        unsigned char np_ocpldo12 : 1;
        unsigned char np_ocpldo11 : 1;
        unsigned char np_ocpldo8 : 1;
    } reg;
} PMIC_NP_OCP2_UNION;
#endif
#define PMIC_NP_OCP2_np_ocpldo18_START (0)
#define PMIC_NP_OCP2_np_ocpldo18_END (0)
#define PMIC_NP_OCP2_np_ocpldo17_START (1)
#define PMIC_NP_OCP2_np_ocpldo17_END (1)
#define PMIC_NP_OCP2_np_ocpldo16_START (2)
#define PMIC_NP_OCP2_np_ocpldo16_END (2)
#define PMIC_NP_OCP2_np_ocpldo15_START (3)
#define PMIC_NP_OCP2_np_ocpldo15_END (3)
#define PMIC_NP_OCP2_np_ocpldo14_START (4)
#define PMIC_NP_OCP2_np_ocpldo14_END (4)
#define PMIC_NP_OCP2_np_ocpldo12_START (5)
#define PMIC_NP_OCP2_np_ocpldo12_END (5)
#define PMIC_NP_OCP2_np_ocpldo11_START (6)
#define PMIC_NP_OCP2_np_ocpldo11_END (6)
#define PMIC_NP_OCP2_np_ocpldo8_START (7)
#define PMIC_NP_OCP2_np_ocpldo8_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ocpldo28 : 1;
        unsigned char np_ocpldo27 : 1;
        unsigned char np_ocpldo26 : 1;
        unsigned char np_ocpldo25 : 1;
        unsigned char np_ocpldo24 : 1;
        unsigned char np_ocpldo23 : 1;
        unsigned char np_ocpldo22 : 1;
        unsigned char np_ocpldo21 : 1;
    } reg;
} PMIC_NP_OCP3_UNION;
#endif
#define PMIC_NP_OCP3_np_ocpldo28_START (0)
#define PMIC_NP_OCP3_np_ocpldo28_END (0)
#define PMIC_NP_OCP3_np_ocpldo27_START (1)
#define PMIC_NP_OCP3_np_ocpldo27_END (1)
#define PMIC_NP_OCP3_np_ocpldo26_START (2)
#define PMIC_NP_OCP3_np_ocpldo26_END (2)
#define PMIC_NP_OCP3_np_ocpldo25_START (3)
#define PMIC_NP_OCP3_np_ocpldo25_END (3)
#define PMIC_NP_OCP3_np_ocpldo24_START (4)
#define PMIC_NP_OCP3_np_ocpldo24_END (4)
#define PMIC_NP_OCP3_np_ocpldo23_START (5)
#define PMIC_NP_OCP3_np_ocpldo23_END (5)
#define PMIC_NP_OCP3_np_ocpldo22_START (6)
#define PMIC_NP_OCP3_np_ocpldo22_END (6)
#define PMIC_NP_OCP3_np_ocpldo21_START (7)
#define PMIC_NP_OCP3_np_ocpldo21_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ocpldo39 : 1;
        unsigned char np_ocpldo38 : 1;
        unsigned char np_ocpldo37 : 1;
        unsigned char np_ocpldo36 : 1;
        unsigned char np_pmuh_ocp : 1;
        unsigned char np_ocpldo32 : 1;
        unsigned char np_ocpldo30 : 1;
        unsigned char np_ocpldo29 : 1;
    } reg;
} PMIC_NP_OCP4_UNION;
#endif
#define PMIC_NP_OCP4_np_ocpldo39_START (0)
#define PMIC_NP_OCP4_np_ocpldo39_END (0)
#define PMIC_NP_OCP4_np_ocpldo38_START (1)
#define PMIC_NP_OCP4_np_ocpldo38_END (1)
#define PMIC_NP_OCP4_np_ocpldo37_START (2)
#define PMIC_NP_OCP4_np_ocpldo37_END (2)
#define PMIC_NP_OCP4_np_ocpldo36_START (3)
#define PMIC_NP_OCP4_np_ocpldo36_END (3)
#define PMIC_NP_OCP4_np_pmuh_ocp_START (4)
#define PMIC_NP_OCP4_np_pmuh_ocp_END (4)
#define PMIC_NP_OCP4_np_ocpldo32_START (5)
#define PMIC_NP_OCP4_np_ocpldo32_END (5)
#define PMIC_NP_OCP4_np_ocpldo30_START (6)
#define PMIC_NP_OCP4_np_ocpldo30_END (6)
#define PMIC_NP_OCP4_np_ocpldo29_START (7)
#define PMIC_NP_OCP4_np_ocpldo29_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sw1_ocp : 1;
        unsigned char np_ocpldo46 : 1;
        unsigned char np_ocpldo45 : 1;
        unsigned char np_ocpldo44 : 1;
        unsigned char np_ocpldo43 : 1;
        unsigned char np_ocpldo42 : 1;
        unsigned char np_ocpldo41 : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_NP_OCP5_UNION;
#endif
#define PMIC_NP_OCP5_np_sw1_ocp_START (0)
#define PMIC_NP_OCP5_np_sw1_ocp_END (0)
#define PMIC_NP_OCP5_np_ocpldo46_START (1)
#define PMIC_NP_OCP5_np_ocpldo46_END (1)
#define PMIC_NP_OCP5_np_ocpldo45_START (2)
#define PMIC_NP_OCP5_np_ocpldo45_END (2)
#define PMIC_NP_OCP5_np_ocpldo44_START (3)
#define PMIC_NP_OCP5_np_ocpldo44_END (3)
#define PMIC_NP_OCP5_np_ocpldo43_START (4)
#define PMIC_NP_OCP5_np_ocpldo43_END (4)
#define PMIC_NP_OCP5_np_ocpldo42_START (5)
#define PMIC_NP_OCP5_np_ocpldo42_END (5)
#define PMIC_NP_OCP5_np_ocpldo41_START (6)
#define PMIC_NP_OCP5_np_ocpldo41_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo_sink_ocp : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_NP_OCP6_UNION;
#endif
#define PMIC_NP_OCP6_np_ldo_sink_ocp_START (0)
#define PMIC_NP_OCP6_np_ldo_sink_ocp_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck1_scp : 1;
        unsigned char np_buck2_scp : 1;
        unsigned char np_buck3_scp : 1;
        unsigned char np_buck401_scp : 1;
        unsigned char np_buck42_scp : 1;
        unsigned char np_buck5_scp : 1;
        unsigned char np_buck70_scp : 1;
        unsigned char np_buck71_scp : 1;
    } reg;
} PMIC_NP_SCP0_UNION;
#endif
#define PMIC_NP_SCP0_np_buck1_scp_START (0)
#define PMIC_NP_SCP0_np_buck1_scp_END (0)
#define PMIC_NP_SCP0_np_buck2_scp_START (1)
#define PMIC_NP_SCP0_np_buck2_scp_END (1)
#define PMIC_NP_SCP0_np_buck3_scp_START (2)
#define PMIC_NP_SCP0_np_buck3_scp_END (2)
#define PMIC_NP_SCP0_np_buck401_scp_START (3)
#define PMIC_NP_SCP0_np_buck401_scp_END (3)
#define PMIC_NP_SCP0_np_buck42_scp_START (4)
#define PMIC_NP_SCP0_np_buck42_scp_END (4)
#define PMIC_NP_SCP0_np_buck5_scp_START (5)
#define PMIC_NP_SCP0_np_buck5_scp_END (5)
#define PMIC_NP_SCP0_np_buck70_scp_START (6)
#define PMIC_NP_SCP0_np_buck70_scp_END (6)
#define PMIC_NP_SCP0_np_buck71_scp_START (7)
#define PMIC_NP_SCP0_np_buck71_scp_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck9_scp : 1;
        unsigned char np_ldo_buff_scp : 1;
        unsigned char np_acr_discharge_ocp : 1;
        unsigned char np_ldo_sink_scp : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_NP_SCP1_UNION;
#endif
#define PMIC_NP_SCP1_np_buck9_scp_START (0)
#define PMIC_NP_SCP1_np_buck9_scp_END (0)
#define PMIC_NP_SCP1_np_ldo_buff_scp_START (1)
#define PMIC_NP_SCP1_np_ldo_buff_scp_END (1)
#define PMIC_NP_SCP1_np_acr_discharge_ocp_START (2)
#define PMIC_NP_SCP1_np_acr_discharge_ocp_END (2)
#define PMIC_NP_SCP1_np_ldo_sink_scp_START (3)
#define PMIC_NP_SCP1_np_ldo_sink_scp_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck401_ovp : 1;
        unsigned char np_buck42_ovp : 1;
        unsigned char np_buck70_ovp : 1;
        unsigned char np_buck71_ovp : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_NP_OVP0_UNION;
#endif
#define PMIC_NP_OVP0_np_buck401_ovp_START (0)
#define PMIC_NP_OVP0_np_buck401_ovp_END (0)
#define PMIC_NP_OVP0_np_buck42_ovp_START (1)
#define PMIC_NP_OVP0_np_buck42_ovp_END (1)
#define PMIC_NP_OVP0_np_buck70_ovp_START (2)
#define PMIC_NP_OVP0_np_buck70_ovp_END (2)
#define PMIC_NP_OVP0_np_buck71_ovp_START (3)
#define PMIC_NP_OVP0_np_buck71_ovp_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck401_cur_det : 1;
        unsigned char np_buck42_cur_det : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_NP_CUR_DET0_UNION;
#endif
#define PMIC_NP_CUR_DET0_np_buck401_cur_det_START (0)
#define PMIC_NP_CUR_DET0_np_buck401_cur_det_END (0)
#define PMIC_NP_CUR_DET0_np_buck42_cur_det_START (1)
#define PMIC_NP_CUR_DET0_np_buck42_cur_det_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck40_ramp_chg : 1;
        unsigned char np_buck42_ramp_chg : 1;
        unsigned char np_buck5_ramp_chg : 1;
        unsigned char np_buck70_ramp_chg : 1;
        unsigned char np_buck71_ramp_chg : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_RAMP_EVENT0_UNION;
#endif
#define PMIC_RAMP_EVENT0_np_buck40_ramp_chg_START (0)
#define PMIC_RAMP_EVENT0_np_buck40_ramp_chg_END (0)
#define PMIC_RAMP_EVENT0_np_buck42_ramp_chg_START (1)
#define PMIC_RAMP_EVENT0_np_buck42_ramp_chg_END (1)
#define PMIC_RAMP_EVENT0_np_buck5_ramp_chg_START (2)
#define PMIC_RAMP_EVENT0_np_buck5_ramp_chg_END (2)
#define PMIC_RAMP_EVENT0_np_buck70_ramp_chg_START (3)
#define PMIC_RAMP_EVENT0_np_buck70_ramp_chg_END (3)
#define PMIC_RAMP_EVENT0_np_buck71_ramp_chg_START (4)
#define PMIC_RAMP_EVENT0_np_buck71_ramp_chg_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo0_ramp_chg : 1;
        unsigned char np_ldo39_ramp_chg : 1;
        unsigned char np_ldo44_ramp_chg : 1;
        unsigned char np_ldo45_ramp_chg : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_RAMP_EVENT1_UNION;
#endif
#define PMIC_RAMP_EVENT1_np_ldo0_ramp_chg_START (0)
#define PMIC_RAMP_EVENT1_np_ldo0_ramp_chg_END (0)
#define PMIC_RAMP_EVENT1_np_ldo39_ramp_chg_START (1)
#define PMIC_RAMP_EVENT1_np_ldo39_ramp_chg_END (1)
#define PMIC_RAMP_EVENT1_np_ldo44_ramp_chg_START (2)
#define PMIC_RAMP_EVENT1_np_ldo44_ramp_chg_END (2)
#define PMIC_RAMP_EVENT1_np_ldo45_ramp_chg_START (3)
#define PMIC_RAMP_EVENT1_np_ldo45_ramp_chg_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck40_ramp_abnor : 1;
        unsigned char np_buck42_ramp_abnor : 1;
        unsigned char np_buck5_ramp_abnor : 1;
        unsigned char np_buck70_ramp_abnor : 1;
        unsigned char np_buck71_ramp_abnor : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_RAMP_EVENT2_UNION;
#endif
#define PMIC_RAMP_EVENT2_np_buck40_ramp_abnor_START (0)
#define PMIC_RAMP_EVENT2_np_buck40_ramp_abnor_END (0)
#define PMIC_RAMP_EVENT2_np_buck42_ramp_abnor_START (1)
#define PMIC_RAMP_EVENT2_np_buck42_ramp_abnor_END (1)
#define PMIC_RAMP_EVENT2_np_buck5_ramp_abnor_START (2)
#define PMIC_RAMP_EVENT2_np_buck5_ramp_abnor_END (2)
#define PMIC_RAMP_EVENT2_np_buck70_ramp_abnor_START (3)
#define PMIC_RAMP_EVENT2_np_buck70_ramp_abnor_END (3)
#define PMIC_RAMP_EVENT2_np_buck71_ramp_abnor_START (4)
#define PMIC_RAMP_EVENT2_np_buck71_ramp_abnor_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo0_ramp_abnor : 1;
        unsigned char np_ldo39_ramp_abnor : 1;
        unsigned char np_ldo44_ramp_abnor : 1;
        unsigned char np_ldo45_ramp_abnor : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_RAMP_EVENT3_UNION;
#endif
#define PMIC_RAMP_EVENT3_np_ldo0_ramp_abnor_START (0)
#define PMIC_RAMP_EVENT3_np_ldo0_ramp_abnor_END (0)
#define PMIC_RAMP_EVENT3_np_ldo39_ramp_abnor_START (1)
#define PMIC_RAMP_EVENT3_np_ldo39_ramp_abnor_END (1)
#define PMIC_RAMP_EVENT3_np_ldo44_ramp_abnor_START (2)
#define PMIC_RAMP_EVENT3_np_ldo44_ramp_abnor_END (2)
#define PMIC_RAMP_EVENT3_np_ldo45_ramp_abnor_START (3)
#define PMIC_RAMP_EVENT3_np_ldo45_ramp_abnor_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_soh_tbat_tmp_ov : 1;
        unsigned char np_soh_mode_abn : 1;
        unsigned char np_soh_vbat_uv : 1;
        unsigned char np_soh_otmp125 : 1;
        unsigned char np_soh_30min_timeout : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_SOH_RECORD_UNION;
#endif
#define PMIC_SOH_RECORD_np_soh_tbat_tmp_ov_START (0)
#define PMIC_SOH_RECORD_np_soh_tbat_tmp_ov_END (0)
#define PMIC_SOH_RECORD_np_soh_mode_abn_START (1)
#define PMIC_SOH_RECORD_np_soh_mode_abn_END (1)
#define PMIC_SOH_RECORD_np_soh_vbat_uv_START (2)
#define PMIC_SOH_RECORD_np_soh_vbat_uv_END (2)
#define PMIC_SOH_RECORD_np_soh_otmp125_START (3)
#define PMIC_SOH_RECORD_np_soh_otmp125_END (3)
#define PMIC_SOH_RECORD_np_soh_30min_timeout_START (4)
#define PMIC_SOH_RECORD_np_soh_30min_timeout_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_acr_flag : 1;
        unsigned char np_acr_dischg_ocp : 1;
        unsigned char np_dcr_dischg_ocp : 1;
        unsigned char np_ovp_dischg_ocp : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACR_SOH_RECORD_UNION;
#endif
#define PMIC_ACR_SOH_RECORD_np_acr_flag_START (0)
#define PMIC_ACR_SOH_RECORD_np_acr_flag_END (0)
#define PMIC_ACR_SOH_RECORD_np_acr_dischg_ocp_START (1)
#define PMIC_ACR_SOH_RECORD_np_acr_dischg_ocp_END (1)
#define PMIC_ACR_SOH_RECORD_np_dcr_dischg_ocp_START (2)
#define PMIC_ACR_SOH_RECORD_np_dcr_dischg_ocp_END (2)
#define PMIC_ACR_SOH_RECORD_np_ovp_dischg_ocp_START (3)
#define PMIC_ACR_SOH_RECORD_np_ovp_dischg_ocp_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo26_ocp_ocv : 1;
        unsigned char np_buck3_scp_ocv : 1;
        unsigned char np_buck3_ocp_ocv : 1;
        unsigned char np_pmud_short_ocv : 1;
        unsigned char np_pmuh_short_ocv : 1;
        unsigned char np_vsys_pwroff_abs_d20nr_ocv : 1;
        unsigned char np_vsys_pwroff_deb_d80mr_ocv : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_COUL_RECORD0_UNION;
#endif
#define PMIC_COUL_RECORD0_np_ldo26_ocp_ocv_START (0)
#define PMIC_COUL_RECORD0_np_ldo26_ocp_ocv_END (0)
#define PMIC_COUL_RECORD0_np_buck3_scp_ocv_START (1)
#define PMIC_COUL_RECORD0_np_buck3_scp_ocv_END (1)
#define PMIC_COUL_RECORD0_np_buck3_ocp_ocv_START (2)
#define PMIC_COUL_RECORD0_np_buck3_ocp_ocv_END (2)
#define PMIC_COUL_RECORD0_np_pmud_short_ocv_START (3)
#define PMIC_COUL_RECORD0_np_pmud_short_ocv_END (3)
#define PMIC_COUL_RECORD0_np_pmuh_short_ocv_START (4)
#define PMIC_COUL_RECORD0_np_pmuh_short_ocv_END (4)
#define PMIC_COUL_RECORD0_np_vsys_pwroff_abs_d20nr_ocv_START (5)
#define PMIC_COUL_RECORD0_np_vsys_pwroff_abs_d20nr_ocv_END (5)
#define PMIC_COUL_RECORD0_np_vsys_pwroff_deb_d80mr_ocv_START (6)
#define PMIC_COUL_RECORD0_np_vsys_pwroff_deb_d80mr_ocv_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo27_ocp_ocv : 1;
        unsigned char np_pmuh_ocp_ocv : 1;
        unsigned char np_avdd_osc_vld_ocv : 1;
        unsigned char np_thsd_otmp140_ocv : 1;
        unsigned char np_thsd_otmp125_ocv : 1;
        unsigned char np_vsys_ov_ocv : 1;
        unsigned char np_buck2_scp_ocv : 1;
        unsigned char np_buck2_ocp_ocv : 1;
    } reg;
} PMIC_COUL_RECORD1_UNION;
#endif
#define PMIC_COUL_RECORD1_np_ldo27_ocp_ocv_START (0)
#define PMIC_COUL_RECORD1_np_ldo27_ocp_ocv_END (0)
#define PMIC_COUL_RECORD1_np_pmuh_ocp_ocv_START (1)
#define PMIC_COUL_RECORD1_np_pmuh_ocp_ocv_END (1)
#define PMIC_COUL_RECORD1_np_avdd_osc_vld_ocv_START (2)
#define PMIC_COUL_RECORD1_np_avdd_osc_vld_ocv_END (2)
#define PMIC_COUL_RECORD1_np_thsd_otmp140_ocv_START (3)
#define PMIC_COUL_RECORD1_np_thsd_otmp140_ocv_END (3)
#define PMIC_COUL_RECORD1_np_thsd_otmp125_ocv_START (4)
#define PMIC_COUL_RECORD1_np_thsd_otmp125_ocv_END (4)
#define PMIC_COUL_RECORD1_np_vsys_ov_ocv_START (5)
#define PMIC_COUL_RECORD1_np_vsys_ov_ocv_END (5)
#define PMIC_COUL_RECORD1_np_buck2_scp_ocv_START (6)
#define PMIC_COUL_RECORD1_np_buck2_scp_ocv_END (6)
#define PMIC_COUL_RECORD1_np_buck2_ocp_ocv_START (7)
#define PMIC_COUL_RECORD1_np_buck2_ocp_ocv_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_fault_s_n : 1;
        unsigned char np_fault_m_n : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_NP_FAULT_RECORD_UNION;
#endif
#define PMIC_NP_FAULT_RECORD_np_fault_s_n_START (0)
#define PMIC_NP_FAULT_RECORD_np_fault_s_n_END (0)
#define PMIC_NP_FAULT_RECORD_np_fault_m_n_START (1)
#define PMIC_NP_FAULT_RECORD_np_fault_m_n_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_vsys_surge_d20nr : 1;
        unsigned char np_cur_alert : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_NP_VSYS_SURGE_RECORD_UNION;
#endif
#define PMIC_NP_VSYS_SURGE_RECORD_np_vsys_surge_d20nr_START (0)
#define PMIC_NP_VSYS_SURGE_RECORD_np_vsys_surge_d20nr_END (0)
#define PMIC_NP_VSYS_SURGE_RECORD_np_cur_alert_START (1)
#define PMIC_NP_VSYS_SURGE_RECORD_np_cur_alert_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_vsys_drop_r : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_NP_VSYS_DROP_RECORD_UNION;
#endif
#define PMIC_NP_VSYS_DROP_RECORD_np_vsys_drop_r_START (0)
#define PMIC_NP_VSYS_DROP_RECORD_np_vsys_drop_r_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pwrup_cali_end : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_PWRUP_CALI_END_UNION;
#endif
#define PMIC_PWRUP_CALI_END_pwrup_cali_end_START (0)
#define PMIC_PWRUP_CALI_END_pwrup_cali_end_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_autocali_ave0_l : 8;
    } reg;
} PMIC_XOADC_AUTOCALI_AVE0_UNION;
#endif
#define PMIC_XOADC_AUTOCALI_AVE0_xo_autocali_ave0_l_START (0)
#define PMIC_XOADC_AUTOCALI_AVE0_xo_autocali_ave0_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_autocali_ave0_h : 8;
    } reg;
} PMIC_XOADC_AUTOCALI_AVE1_UNION;
#endif
#define PMIC_XOADC_AUTOCALI_AVE1_xo_autocali_ave0_h_START (0)
#define PMIC_XOADC_AUTOCALI_AVE1_xo_autocali_ave0_h_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_autocali_ave1_l : 8;
    } reg;
} PMIC_XOADC_AUTOCALI_AVE2_UNION;
#endif
#define PMIC_XOADC_AUTOCALI_AVE2_xo_autocali_ave1_l_START (0)
#define PMIC_XOADC_AUTOCALI_AVE2_xo_autocali_ave1_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_autocali_ave1_h : 8;
    } reg;
} PMIC_XOADC_AUTOCALI_AVE3_UNION;
#endif
#define PMIC_XOADC_AUTOCALI_AVE3_xo_autocali_ave1_h_START (0)
#define PMIC_XOADC_AUTOCALI_AVE3_xo_autocali_ave1_h_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xoadc_data_rate_sel : 3;
        unsigned char xoadc_sdm_clk_sel : 1;
        unsigned char xo_chop_fre_sel : 2;
        unsigned char xo_chop_en : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_XOADC_CTRL_UNION;
#endif
#define PMIC_XOADC_CTRL_xoadc_data_rate_sel_START (0)
#define PMIC_XOADC_CTRL_xoadc_data_rate_sel_END (2)
#define PMIC_XOADC_CTRL_xoadc_sdm_clk_sel_START (3)
#define PMIC_XOADC_CTRL_xoadc_sdm_clk_sel_END (3)
#define PMIC_XOADC_CTRL_xo_chop_fre_sel_START (4)
#define PMIC_XOADC_CTRL_xo_chop_fre_sel_END (5)
#define PMIC_XOADC_CTRL_xo_chop_en_START (6)
#define PMIC_XOADC_CTRL_xo_chop_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xoadc_samp_phase_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_XOADC_SAMP_PHASE_UNION;
#endif
#define PMIC_XOADC_SAMP_PHASE_xoadc_samp_phase_sel_START (0)
#define PMIC_XOADC_SAMP_PHASE_xoadc_samp_phase_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xoadc_opt_0 : 8;
    } reg;
} PMIC_XOADC_OPT_0_UNION;
#endif
#define PMIC_XOADC_OPT_0_xoadc_opt_0_START (0)
#define PMIC_XOADC_OPT_0_xoadc_opt_0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xoadc_opt_1 : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_XOADC_OPT_1_UNION;
#endif
#define PMIC_XOADC_OPT_1_xoadc_opt_1_START (0)
#define PMIC_XOADC_OPT_1_xoadc_opt_1_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xoadc_ain_sel : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_XOADC_AIN_SEL_UNION;
#endif
#define PMIC_XOADC_AIN_SEL_xoadc_ain_sel_START (0)
#define PMIC_XOADC_AIN_SEL_xoadc_ain_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char wifi_xoadc_ana_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_XOADC_WIFI_ANA_EN_UNION;
#endif
#define PMIC_XOADC_WIFI_ANA_EN_wifi_xoadc_ana_en_START (0)
#define PMIC_XOADC_WIFI_ANA_EN_wifi_xoadc_ana_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char soc_initial : 1;
        unsigned char soc_xoadc_ana_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_XOADC_SOC_ANA_EN_UNION;
#endif
#define PMIC_XOADC_SOC_ANA_EN_soc_initial_START (0)
#define PMIC_XOADC_SOC_ANA_EN_soc_initial_END (0)
#define PMIC_XOADC_SOC_ANA_EN_soc_xoadc_ana_en_START (1)
#define PMIC_XOADC_SOC_ANA_EN_soc_xoadc_ana_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char data_valid : 1;
        unsigned char soc_initial_ro : 1;
        unsigned char soc_xoadc_ana_en_ro : 1;
        unsigned char wifi_xoadc_ana_en_ro : 1;
        unsigned char soc_xo_cfg_en_ro : 2;
        unsigned char wifi_xo_cfg_en_ro : 2;
    } reg;
} PMIC_XOADC_STATE_UNION;
#endif
#define PMIC_XOADC_STATE_data_valid_START (0)
#define PMIC_XOADC_STATE_data_valid_END (0)
#define PMIC_XOADC_STATE_soc_initial_ro_START (1)
#define PMIC_XOADC_STATE_soc_initial_ro_END (1)
#define PMIC_XOADC_STATE_soc_xoadc_ana_en_ro_START (2)
#define PMIC_XOADC_STATE_soc_xoadc_ana_en_ro_END (2)
#define PMIC_XOADC_STATE_wifi_xoadc_ana_en_ro_START (3)
#define PMIC_XOADC_STATE_wifi_xoadc_ana_en_ro_END (3)
#define PMIC_XOADC_STATE_soc_xo_cfg_en_ro_START (4)
#define PMIC_XOADC_STATE_soc_xo_cfg_en_ro_END (5)
#define PMIC_XOADC_STATE_wifi_xo_cfg_en_ro_START (6)
#define PMIC_XOADC_STATE_wifi_xo_cfg_en_ro_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_data_0 : 8;
    } reg;
} PMIC_XOADC_DATA0_UNION;
#endif
#define PMIC_XOADC_DATA0_xo_data_0_START (0)
#define PMIC_XOADC_DATA0_xo_data_0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_data_1 : 8;
    } reg;
} PMIC_XOADC_DATA1_UNION;
#endif
#define PMIC_XOADC_DATA1_xo_data_1_START (0)
#define PMIC_XOADC_DATA1_xo_data_1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_cali_data_0 : 8;
    } reg;
} PMIC_XOADC_CALI_DATA0_UNION;
#endif
#define PMIC_XOADC_CALI_DATA0_xo_cali_data_0_START (0)
#define PMIC_XOADC_CALI_DATA0_xo_cali_data_0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xo_cali_data_1 : 8;
    } reg;
} PMIC_XOADC_CALI_DATA1_UNION;
#endif
#define PMIC_XOADC_CALI_DATA1_xo_cali_data_1_START (0)
#define PMIC_XOADC_CALI_DATA1_xo_cali_data_1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char soc_xo_cfg_en : 2;
        unsigned char wifi_xo_cfg_en : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_XOADC_CFG_EN_UNION;
#endif
#define PMIC_XOADC_CFG_EN_soc_xo_cfg_en_START (0)
#define PMIC_XOADC_CFG_EN_soc_xo_cfg_en_END (1)
#define PMIC_XOADC_CFG_EN_wifi_xo_cfg_en_START (2)
#define PMIC_XOADC_CFG_EN_wifi_xo_cfg_en_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char abnm_cfg : 2;
        unsigned char data_valid_ori : 1;
        unsigned char abnm_info : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_XOADC_ARB_DEBUG_UNION;
#endif
#define PMIC_XOADC_ARB_DEBUG_abnm_cfg_START (0)
#define PMIC_XOADC_ARB_DEBUG_abnm_cfg_END (1)
#define PMIC_XOADC_ARB_DEBUG_data_valid_ori_START (2)
#define PMIC_XOADC_ARB_DEBUG_data_valid_ori_END (2)
#define PMIC_XOADC_ARB_DEBUG_abnm_info_START (3)
#define PMIC_XOADC_ARB_DEBUG_abnm_info_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xoadc_data_rate_sel_s : 3;
        unsigned char xoadc_sdm_clk_sel_s : 1;
        unsigned char xo_chop_fre_sel_s : 2;
        unsigned char xo_chop_en_s : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_XOADC_CTRL_S_UNION;
#endif
#define PMIC_XOADC_CTRL_S_xoadc_data_rate_sel_s_START (0)
#define PMIC_XOADC_CTRL_S_xoadc_data_rate_sel_s_END (2)
#define PMIC_XOADC_CTRL_S_xoadc_sdm_clk_sel_s_START (3)
#define PMIC_XOADC_CTRL_S_xoadc_sdm_clk_sel_s_END (3)
#define PMIC_XOADC_CTRL_S_xo_chop_fre_sel_s_START (4)
#define PMIC_XOADC_CTRL_S_xo_chop_fre_sel_s_END (5)
#define PMIC_XOADC_CTRL_S_xo_chop_en_s_START (6)
#define PMIC_XOADC_CTRL_S_xo_chop_en_s_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xoadc_samp_phase_sel_s : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_XOADC_SAMP_PHASE_S_UNION;
#endif
#define PMIC_XOADC_SAMP_PHASE_S_xoadc_samp_phase_sel_s_START (0)
#define PMIC_XOADC_SAMP_PHASE_S_xoadc_samp_phase_sel_s_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xoadc_opt_0_s : 8;
    } reg;
} PMIC_XOADC_OPT_0_S_UNION;
#endif
#define PMIC_XOADC_OPT_0_S_xoadc_opt_0_s_START (0)
#define PMIC_XOADC_OPT_0_S_xoadc_opt_0_s_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xoadc_opt_1_s : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_XOADC_OPT_1_S_UNION;
#endif
#define PMIC_XOADC_OPT_1_S_xoadc_opt_1_s_START (0)
#define PMIC_XOADC_OPT_1_S_xoadc_opt_1_s_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xoadc_ain_sel_s : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_XOADC_AIN_SEL_S_UNION;
#endif
#define PMIC_XOADC_AIN_SEL_S_xoadc_ain_sel_s_START (0)
#define PMIC_XOADC_AIN_SEL_S_xoadc_ain_sel_s_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xoadc_ana_en_s : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_XOADC_ANA_EN_S_UNION;
#endif
#define PMIC_XOADC_ANA_EN_S_xoadc_ana_en_s_START (0)
#define PMIC_XOADC_ANA_EN_S_xoadc_ana_en_s_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char soft_cfg0 : 8;
    } reg;
} PMIC_XOADC_SOFT_CFG0_UNION;
#endif
#define PMIC_XOADC_SOFT_CFG0_soft_cfg0_START (0)
#define PMIC_XOADC_SOFT_CFG0_soft_cfg0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char soft_cfg1 : 8;
    } reg;
} PMIC_XOADC_SOFT_CFG1_UNION;
#endif
#define PMIC_XOADC_SOFT_CFG1_soft_cfg1_START (0)
#define PMIC_XOADC_SOFT_CFG1_soft_cfg1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char soft_cfg2 : 8;
    } reg;
} PMIC_XOADC_SOFT_CFG2_UNION;
#endif
#define PMIC_XOADC_SOFT_CFG2_soft_cfg2_START (0)
#define PMIC_XOADC_SOFT_CFG2_soft_cfg2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char soft_cfg3 : 8;
    } reg;
} PMIC_XOADC_SOFT_CFG3_UNION;
#endif
#define PMIC_XOADC_SOFT_CFG3_soft_cfg3_START (0)
#define PMIC_XOADC_SOFT_CFG3_soft_cfg3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char soft_cfg4 : 8;
    } reg;
} PMIC_XOADC_SOFT_CFG4_UNION;
#endif
#define PMIC_XOADC_SOFT_CFG4_soft_cfg4_START (0)
#define PMIC_XOADC_SOFT_CFG4_soft_cfg4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char soft_cfg5 : 8;
    } reg;
} PMIC_XOADC_SOFT_CFG5_UNION;
#endif
#define PMIC_XOADC_SOFT_CFG5_soft_cfg5_START (0)
#define PMIC_XOADC_SOFT_CFG5_soft_cfg5_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char soft_cfg6 : 8;
    } reg;
} PMIC_XOADC_SOFT_CFG6_UNION;
#endif
#define PMIC_XOADC_SOFT_CFG6_soft_cfg6_START (0)
#define PMIC_XOADC_SOFT_CFG6_soft_cfg6_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char soft_cfg7 : 8;
    } reg;
} PMIC_XOADC_SOFT_CFG7_UNION;
#endif
#define PMIC_XOADC_SOFT_CFG7_soft_cfg7_START (0)
#define PMIC_XOADC_SOFT_CFG7_soft_cfg7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char xoadc_reserve : 8;
    } reg;
} PMIC_XOADC_RESERVE_UNION;
#endif
#define PMIC_XOADC_RESERVE_xoadc_reserve_START (0)
#define PMIC_XOADC_RESERVE_xoadc_reserve_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hi1103_rdata_out0 : 8;
    } reg;
} PMIC_HI1103_RDATA_OUT0_UNION;
#endif
#define PMIC_HI1103_RDATA_OUT0_hi1103_rdata_out0_START (0)
#define PMIC_HI1103_RDATA_OUT0_hi1103_rdata_out0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hi1103_rdata_out1 : 8;
    } reg;
} PMIC_HI1103_RDATA_OUT1_UNION;
#endif
#define PMIC_HI1103_RDATA_OUT1_hi1103_rdata_out1_START (0)
#define PMIC_HI1103_RDATA_OUT1_hi1103_rdata_out1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hi1103_rdata_out2 : 8;
    } reg;
} PMIC_HI1103_RDATA_OUT2_UNION;
#endif
#define PMIC_HI1103_RDATA_OUT2_hi1103_rdata_out2_START (0)
#define PMIC_HI1103_RDATA_OUT2_hi1103_rdata_out2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hi1103_rdata_out3 : 8;
    } reg;
} PMIC_HI1103_RDATA_OUT3_UNION;
#endif
#define PMIC_HI1103_RDATA_OUT3_hi1103_rdata_out3_START (0)
#define PMIC_HI1103_RDATA_OUT3_hi1103_rdata_out3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hi1103_rdata_out4 : 8;
    } reg;
} PMIC_HI1103_RDATA_OUT4_UNION;
#endif
#define PMIC_HI1103_RDATA_OUT4_hi1103_rdata_out4_START (0)
#define PMIC_HI1103_RDATA_OUT4_hi1103_rdata_out4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hi1103_rdata_out5 : 8;
    } reg;
} PMIC_HI1103_RDATA_OUT5_UNION;
#endif
#define PMIC_HI1103_RDATA_OUT5_hi1103_rdata_out5_START (0)
#define PMIC_HI1103_RDATA_OUT5_hi1103_rdata_out5_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hi1103_rdata_out6 : 8;
    } reg;
} PMIC_HI1103_RDATA_OUT6_UNION;
#endif
#define PMIC_HI1103_RDATA_OUT6_hi1103_rdata_out6_START (0)
#define PMIC_HI1103_RDATA_OUT6_hi1103_rdata_out6_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hi1103_rdata_out7 : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_HI1103_RDATA_OUT7_UNION;
#endif
#define PMIC_HI1103_RDATA_OUT7_hi1103_rdata_out7_START (0)
#define PMIC_HI1103_RDATA_OUT7_hi1103_rdata_out7_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtc_load_flag : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_RTC_LOAD_FLAG_UNION;
#endif
#define PMIC_RTC_LOAD_FLAG_rtc_load_flag_START (0)
#define PMIC_RTC_LOAD_FLAG_rtc_load_flag_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hi1103_refresh_data : 8;
    } reg;
} PMIC_HI1103_REFRESH_LOCK_UNION;
#endif
#define PMIC_HI1103_REFRESH_LOCK_hi1103_refresh_data_START (0)
#define PMIC_HI1103_REFRESH_LOCK_hi1103_refresh_data_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug0 : 8;
    } reg;
} PMIC_SPMI_DEBUG0_UNION;
#endif
#define PMIC_SPMI_DEBUG0_spmi_debug0_START (0)
#define PMIC_SPMI_DEBUG0_spmi_debug0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug1 : 8;
    } reg;
} PMIC_SPMI_DEBUG1_UNION;
#endif
#define PMIC_SPMI_DEBUG1_spmi_debug1_START (0)
#define PMIC_SPMI_DEBUG1_spmi_debug1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug2 : 8;
    } reg;
} PMIC_SPMI_DEBUG2_UNION;
#endif
#define PMIC_SPMI_DEBUG2_spmi_debug2_START (0)
#define PMIC_SPMI_DEBUG2_spmi_debug2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug3 : 8;
    } reg;
} PMIC_SPMI_DEBUG3_UNION;
#endif
#define PMIC_SPMI_DEBUG3_spmi_debug3_START (0)
#define PMIC_SPMI_DEBUG3_spmi_debug3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug4 : 8;
    } reg;
} PMIC_SPMI_DEBUG4_UNION;
#endif
#define PMIC_SPMI_DEBUG4_spmi_debug4_START (0)
#define PMIC_SPMI_DEBUG4_spmi_debug4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug5 : 8;
    } reg;
} PMIC_SPMI_DEBUG5_UNION;
#endif
#define PMIC_SPMI_DEBUG5_spmi_debug5_START (0)
#define PMIC_SPMI_DEBUG5_spmi_debug5_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug6 : 8;
    } reg;
} PMIC_SPMI_DEBUG6_UNION;
#endif
#define PMIC_SPMI_DEBUG6_spmi_debug6_START (0)
#define PMIC_SPMI_DEBUG6_spmi_debug6_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_debug7 : 8;
    } reg;
} PMIC_SPMI_DEBUG7_UNION;
#endif
#define PMIC_SPMI_DEBUG7_spmi_debug7_START (0)
#define PMIC_SPMI_DEBUG7_spmi_debug7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_clk32_bt_ds : 2;
        unsigned char np_clk32_sys_ds : 2;
        unsigned char np_spmi_data_ds : 2;
        unsigned char np_sidekey_irq_ds : 2;
    } reg;
} PMIC_DIG_IO_DS_CFG0_UNION;
#endif
#define PMIC_DIG_IO_DS_CFG0_np_clk32_bt_ds_START (0)
#define PMIC_DIG_IO_DS_CFG0_np_clk32_bt_ds_END (1)
#define PMIC_DIG_IO_DS_CFG0_np_clk32_sys_ds_START (2)
#define PMIC_DIG_IO_DS_CFG0_np_clk32_sys_ds_END (3)
#define PMIC_DIG_IO_DS_CFG0_np_spmi_data_ds_START (4)
#define PMIC_DIG_IO_DS_CFG0_np_spmi_data_ds_END (5)
#define PMIC_DIG_IO_DS_CFG0_np_sidekey_irq_ds_START (6)
#define PMIC_DIG_IO_DS_CFG0_np_sidekey_irq_ds_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_pmu0_irq_n_ds : 2;
        unsigned char np_sys_rst_n_ds : 2;
        unsigned char np_clk32_gps_ds : 2;
        unsigned char np_xoadc_ssi_ds : 2;
    } reg;
} PMIC_DIG_IO_DS_CFG1_UNION;
#endif
#define PMIC_DIG_IO_DS_CFG1_np_pmu0_irq_n_ds_START (0)
#define PMIC_DIG_IO_DS_CFG1_np_pmu0_irq_n_ds_END (1)
#define PMIC_DIG_IO_DS_CFG1_np_sys_rst_n_ds_START (2)
#define PMIC_DIG_IO_DS_CFG1_np_sys_rst_n_ds_END (3)
#define PMIC_DIG_IO_DS_CFG1_np_clk32_gps_ds_START (4)
#define PMIC_DIG_IO_DS_CFG1_np_clk32_gps_ds_END (5)
#define PMIC_DIG_IO_DS_CFG1_np_xoadc_ssi_ds_START (6)
#define PMIC_DIG_IO_DS_CFG1_np_xoadc_ssi_ds_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_vbat_drop_irq_ds : 2;
        unsigned char np_lra_clk_ds : 2;
        unsigned char np_cur_alert_ds : 2;
        unsigned char reserved : 2;
    } reg;
} PMIC_DIG_IO_DS_CFG2_UNION;
#endif
#define PMIC_DIG_IO_DS_CFG2_np_vbat_drop_irq_ds_START (0)
#define PMIC_DIG_IO_DS_CFG2_np_vbat_drop_irq_ds_END (1)
#define PMIC_DIG_IO_DS_CFG2_np_lra_clk_ds_START (2)
#define PMIC_DIG_IO_DS_CFG2_np_lra_clk_ds_END (3)
#define PMIC_DIG_IO_DS_CFG2_np_cur_alert_ds_START (4)
#define PMIC_DIG_IO_DS_CFG2_np_cur_alert_ds_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_dummy_xoadc_ssi : 4;
        unsigned char st_dummy_xoadc_ssi : 4;
    } reg;
} PMIC_DUMMY_XOADC_SSI_UNION;
#endif
#define PMIC_DUMMY_XOADC_SSI_sc_dummy_xoadc_ssi_START (0)
#define PMIC_DUMMY_XOADC_SSI_sc_dummy_xoadc_ssi_END (3)
#define PMIC_DUMMY_XOADC_SSI_st_dummy_xoadc_ssi_START (4)
#define PMIC_DUMMY_XOADC_SSI_st_dummy_xoadc_ssi_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_pwron_8s_sel : 1;
        unsigned char np_pwron_time_sel : 2;
        unsigned char reserved : 5;
    } reg;
} PMIC_NP_SYS_CTRL0_UNION;
#endif
#define PMIC_NP_SYS_CTRL0_np_pwron_8s_sel_START (0)
#define PMIC_NP_SYS_CTRL0_np_pwron_8s_sel_END (0)
#define PMIC_NP_SYS_CTRL0_np_pwron_time_sel_START (1)
#define PMIC_NP_SYS_CTRL0_np_pwron_time_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_nfc_pwron_mask : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_NP_SYS_CTRL1_UNION;
#endif
#define PMIC_NP_SYS_CTRL1_np_nfc_pwron_mask_START (0)
#define PMIC_NP_SYS_CTRL1_np_nfc_pwron_mask_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_wifi_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_NP_CLK_WIFI_CTRL_UNION;
#endif
#define PMIC_NP_CLK_WIFI_CTRL_np_xo_dig_wifi_sel_START (0)
#define PMIC_NP_CLK_WIFI_CTRL_np_xo_dig_wifi_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_nfc_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_NP_CLK_NFC_CTRL_UNION;
#endif
#define PMIC_NP_CLK_NFC_CTRL_np_xo_dig_nfc_sel_START (0)
#define PMIC_NP_CLK_NFC_CTRL_np_xo_dig_nfc_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_rf0_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_NP_CLK_RF0_CTRL_UNION;
#endif
#define PMIC_NP_CLK_RF0_CTRL_np_xo_dig_rf0_sel_START (0)
#define PMIC_NP_CLK_RF0_CTRL_np_xo_dig_rf0_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_rf1_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_NP_CLK_RF1_CTRL_UNION;
#endif
#define PMIC_NP_CLK_RF1_CTRL_np_xo_dig_rf1_sel_START (0)
#define PMIC_NP_CLK_RF1_CTRL_np_xo_dig_rf1_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_dig_codec_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_NP_CLK_CODEC_CTRL_UNION;
#endif
#define PMIC_NP_CLK_CODEC_CTRL_np_xo_dig_codec_sel_START (0)
#define PMIC_NP_CLK_CODEC_CTRL_np_xo_dig_codec_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_trim_c2fix : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_NP_CLK_TOP_CTRL0_UNION;
#endif
#define PMIC_NP_CLK_TOP_CTRL0_np_xo_trim_c2fix_START (0)
#define PMIC_NP_CLK_TOP_CTRL0_np_xo_trim_c2fix_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_trim_c1fix : 8;
    } reg;
} PMIC_NP_CLK_TOP_CTRL1_UNION;
#endif
#define PMIC_NP_CLK_TOP_CTRL1_np_xo_trim_c1fix_START (0)
#define PMIC_NP_CLK_TOP_CTRL1_np_xo_trim_c1fix_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_reg_256k_en0 : 8;
    } reg;
} PMIC_NP_CLK_256K_CTRL0_UNION;
#endif
#define PMIC_NP_CLK_256K_CTRL0_np_reg_256k_en0_START (0)
#define PMIC_NP_CLK_256K_CTRL0_np_reg_256k_en0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_reg_256k_en1 : 8;
    } reg;
} PMIC_NP_CLK_256K_CTRL1_UNION;
#endif
#define PMIC_NP_CLK_256K_CTRL1_np_reg_256k_en1_START (0)
#define PMIC_NP_CLK_256K_CTRL1_np_reg_256k_en1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_xo_ufs_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_NP_CLK_UFS_FRE_CTRL_UNION;
#endif
#define PMIC_NP_CLK_UFS_FRE_CTRL_np_xo_ufs_sel_START (0)
#define PMIC_NP_CLK_UFS_FRE_CTRL_np_xo_ufs_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_vsys_pwroff_deb_set : 2;
        unsigned char np_vsys_pwroff_abs_set : 2;
        unsigned char np_vsys_pwron_set : 2;
        unsigned char np_surge_sel : 2;
    } reg;
} PMIC_NP_VSYS_LOW_SET_UNION;
#endif
#define PMIC_NP_VSYS_LOW_SET_np_vsys_pwroff_deb_set_START (0)
#define PMIC_NP_VSYS_LOW_SET_np_vsys_pwroff_deb_set_END (1)
#define PMIC_NP_VSYS_LOW_SET_np_vsys_pwroff_abs_set_START (2)
#define PMIC_NP_VSYS_LOW_SET_np_vsys_pwroff_abs_set_END (3)
#define PMIC_NP_VSYS_LOW_SET_np_vsys_pwron_set_START (4)
#define PMIC_NP_VSYS_LOW_SET_np_vsys_pwron_set_END (5)
#define PMIC_NP_VSYS_LOW_SET_np_surge_sel_START (6)
#define PMIC_NP_VSYS_LOW_SET_np_surge_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_vsys_drop_set : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_NP_VSYS_DROP_SET_UNION;
#endif
#define PMIC_NP_VSYS_DROP_SET_np_vsys_drop_set_START (0)
#define PMIC_NP_VSYS_DROP_SET_np_vsys_drop_set_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_nfc_on_d2a : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_NP_NFC_ON_CTRL_UNION;
#endif
#define PMIC_NP_NFC_ON_CTRL_np_nfc_on_d2a_START (0)
#define PMIC_NP_NFC_ON_CTRL_np_nfc_on_d2a_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hreset_mode : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_NP_HRESET_PWRDOWN_CTRL_UNION;
#endif
#define PMIC_NP_HRESET_PWRDOWN_CTRL_np_hreset_mode_START (0)
#define PMIC_NP_HRESET_PWRDOWN_CTRL_np_hreset_mode_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_smpl_open_en : 1;
        unsigned char np_smpl_time_sel : 2;
        unsigned char reserved : 5;
    } reg;
} PMIC_NP_SMPL_CTRL_UNION;
#endif
#define PMIC_NP_SMPL_CTRL_np_smpl_open_en_START (0)
#define PMIC_NP_SMPL_CTRL_np_smpl_open_en_END (0)
#define PMIC_NP_SMPL_CTRL_np_smpl_time_sel_START (1)
#define PMIC_NP_SMPL_CTRL_np_smpl_time_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_debug_lock : 8;
    } reg;
} PMIC_NP_DEBUG_LOCK_UNION;
#endif
#define PMIC_NP_DEBUG_LOCK_np_debug_lock_START (0)
#define PMIC_NP_DEBUG_LOCK_np_debug_lock_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sys_debug0 : 8;
    } reg;
} PMIC_NP_SYS_DEBUG0_UNION;
#endif
#define PMIC_NP_SYS_DEBUG0_np_sys_debug0_START (0)
#define PMIC_NP_SYS_DEBUG0_np_sys_debug0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sys_debug1 : 8;
    } reg;
} PMIC_NP_SYS_DEBUG1_UNION;
#endif
#define PMIC_NP_SYS_DEBUG1_np_sys_debug1_START (0)
#define PMIC_NP_SYS_DEBUG1_np_sys_debug1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_reg_rc_debug : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_NP_SYS_DEBUG2_UNION;
#endif
#define PMIC_NP_SYS_DEBUG2_np_reg_rc_debug_START (0)
#define PMIC_NP_SYS_DEBUG2_np_reg_rc_debug_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_vsys_pwroff_abs_pd_mask : 1;
        unsigned char np_vsys_pwroff_deb_pd_mask : 1;
        unsigned char np_thsd_otmp140_pd_mask : 1;
        unsigned char np_vsys_ov_pd_mask : 1;
        unsigned char np_vin_ldoh_vld_pd_mask : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_SYS_DEBUG3_UNION;
#endif
#define PMIC_NP_SYS_DEBUG3_np_vsys_pwroff_abs_pd_mask_START (0)
#define PMIC_NP_SYS_DEBUG3_np_vsys_pwroff_abs_pd_mask_END (0)
#define PMIC_NP_SYS_DEBUG3_np_vsys_pwroff_deb_pd_mask_START (1)
#define PMIC_NP_SYS_DEBUG3_np_vsys_pwroff_deb_pd_mask_END (1)
#define PMIC_NP_SYS_DEBUG3_np_thsd_otmp140_pd_mask_START (2)
#define PMIC_NP_SYS_DEBUG3_np_thsd_otmp140_pd_mask_END (2)
#define PMIC_NP_SYS_DEBUG3_np_vsys_ov_pd_mask_START (3)
#define PMIC_NP_SYS_DEBUG3_np_vsys_ov_pd_mask_END (3)
#define PMIC_NP_SYS_DEBUG3_np_vin_ldoh_vld_pd_mask_START (4)
#define PMIC_NP_SYS_DEBUG3_np_vin_ldoh_vld_pd_mask_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sys_debug2 : 8;
    } reg;
} PMIC_NP_SYS_DEBUG4_UNION;
#endif
#define PMIC_NP_SYS_DEBUG4_np_sys_debug2_START (0)
#define PMIC_NP_SYS_DEBUG4_np_sys_debug2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sys_debug3 : 8;
    } reg;
} PMIC_NP_SYS_DEBUG5_UNION;
#endif
#define PMIC_NP_SYS_DEBUG5_np_sys_debug3_START (0)
#define PMIC_NP_SYS_DEBUG5_np_sys_debug3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_chg_en : 1;
        unsigned char np_chg_bypass : 1;
        unsigned char np_chg_vset : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_NP_BACKUP_CHG_UNION;
#endif
#define PMIC_NP_BACKUP_CHG_np_chg_en_START (0)
#define PMIC_NP_BACKUP_CHG_np_chg_en_END (0)
#define PMIC_NP_BACKUP_CHG_np_chg_bypass_START (1)
#define PMIC_NP_BACKUP_CHG_np_chg_bypass_END (1)
#define PMIC_NP_BACKUP_CHG_np_chg_vset_START (2)
#define PMIC_NP_BACKUP_CHG_np_chg_vset_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_rtc_cali_ctrl : 1;
        unsigned char np_hreset_d_sel : 1;
        unsigned char np_b4_mode_sel : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_NP_CTRL_UNION;
#endif
#define PMIC_NP_CTRL_np_rtc_cali_ctrl_START (0)
#define PMIC_NP_CTRL_np_rtc_cali_ctrl_END (0)
#define PMIC_NP_CTRL_np_hreset_d_sel_START (1)
#define PMIC_NP_CTRL_np_hreset_d_sel_END (1)
#define PMIC_NP_CTRL_np_b4_mode_sel_START (2)
#define PMIC_NP_CTRL_np_b4_mode_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_nfc_clk_shield_res : 1;
        unsigned char np_wifi_clk_shield_res : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_NP_SHIELD_RES_UNION;
#endif
#define PMIC_NP_SHIELD_RES_np_nfc_clk_shield_res_START (0)
#define PMIC_NP_SHIELD_RES_np_nfc_clk_shield_res_END (0)
#define PMIC_NP_SHIELD_RES_np_wifi_clk_shield_res_START (1)
#define PMIC_NP_SHIELD_RES_np_wifi_clk_shield_res_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_d2a_reserve0 : 8;
    } reg;
} PMIC_NP_D2A_RES0_UNION;
#endif
#define PMIC_NP_D2A_RES0_np_d2a_reserve0_START (0)
#define PMIC_NP_D2A_RES0_np_d2a_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_d2a_reserve1 : 8;
    } reg;
} PMIC_NP_D2A_RES1_UNION;
#endif
#define PMIC_NP_D2A_RES1_np_d2a_reserve1_START (0)
#define PMIC_NP_D2A_RES1_np_d2a_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg0 : 8;
    } reg;
} PMIC_HRST_REG0_UNION;
#endif
#define PMIC_HRST_REG0_np_hrst_reg0_START (0)
#define PMIC_HRST_REG0_np_hrst_reg0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg1 : 8;
    } reg;
} PMIC_HRST_REG1_UNION;
#endif
#define PMIC_HRST_REG1_np_hrst_reg1_START (0)
#define PMIC_HRST_REG1_np_hrst_reg1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg2 : 8;
    } reg;
} PMIC_HRST_REG2_UNION;
#endif
#define PMIC_HRST_REG2_np_hrst_reg2_START (0)
#define PMIC_HRST_REG2_np_hrst_reg2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg3 : 8;
    } reg;
} PMIC_HRST_REG3_UNION;
#endif
#define PMIC_HRST_REG3_np_hrst_reg3_START (0)
#define PMIC_HRST_REG3_np_hrst_reg3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg4 : 8;
    } reg;
} PMIC_HRST_REG4_UNION;
#endif
#define PMIC_HRST_REG4_np_hrst_reg4_START (0)
#define PMIC_HRST_REG4_np_hrst_reg4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg5 : 8;
    } reg;
} PMIC_HRST_REG5_UNION;
#endif
#define PMIC_HRST_REG5_np_hrst_reg5_START (0)
#define PMIC_HRST_REG5_np_hrst_reg5_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg6 : 8;
    } reg;
} PMIC_HRST_REG6_UNION;
#endif
#define PMIC_HRST_REG6_np_hrst_reg6_START (0)
#define PMIC_HRST_REG6_np_hrst_reg6_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg7 : 8;
    } reg;
} PMIC_HRST_REG7_UNION;
#endif
#define PMIC_HRST_REG7_np_hrst_reg7_START (0)
#define PMIC_HRST_REG7_np_hrst_reg7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg8 : 8;
    } reg;
} PMIC_HRST_REG8_UNION;
#endif
#define PMIC_HRST_REG8_np_hrst_reg8_START (0)
#define PMIC_HRST_REG8_np_hrst_reg8_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg9 : 8;
    } reg;
} PMIC_HRST_REG9_UNION;
#endif
#define PMIC_HRST_REG9_np_hrst_reg9_START (0)
#define PMIC_HRST_REG9_np_hrst_reg9_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg10 : 8;
    } reg;
} PMIC_HRST_REG10_UNION;
#endif
#define PMIC_HRST_REG10_np_hrst_reg10_START (0)
#define PMIC_HRST_REG10_np_hrst_reg10_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg11 : 8;
    } reg;
} PMIC_HRST_REG11_UNION;
#endif
#define PMIC_HRST_REG11_np_hrst_reg11_START (0)
#define PMIC_HRST_REG11_np_hrst_reg11_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg12 : 8;
    } reg;
} PMIC_HRST_REG12_UNION;
#endif
#define PMIC_HRST_REG12_np_hrst_reg12_START (0)
#define PMIC_HRST_REG12_np_hrst_reg12_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg13 : 8;
    } reg;
} PMIC_HRST_REG13_UNION;
#endif
#define PMIC_HRST_REG13_np_hrst_reg13_START (0)
#define PMIC_HRST_REG13_np_hrst_reg13_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg14 : 8;
    } reg;
} PMIC_HRST_REG14_UNION;
#endif
#define PMIC_HRST_REG14_np_hrst_reg14_START (0)
#define PMIC_HRST_REG14_np_hrst_reg14_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg15 : 8;
    } reg;
} PMIC_HRST_REG15_UNION;
#endif
#define PMIC_HRST_REG15_np_hrst_reg15_START (0)
#define PMIC_HRST_REG15_np_hrst_reg15_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg16 : 8;
    } reg;
} PMIC_HRST_REG16_UNION;
#endif
#define PMIC_HRST_REG16_np_hrst_reg16_START (0)
#define PMIC_HRST_REG16_np_hrst_reg16_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg17 : 8;
    } reg;
} PMIC_HRST_REG17_UNION;
#endif
#define PMIC_HRST_REG17_np_hrst_reg17_START (0)
#define PMIC_HRST_REG17_np_hrst_reg17_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg18 : 8;
    } reg;
} PMIC_HRST_REG18_UNION;
#endif
#define PMIC_HRST_REG18_np_hrst_reg18_START (0)
#define PMIC_HRST_REG18_np_hrst_reg18_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg19 : 8;
    } reg;
} PMIC_HRST_REG19_UNION;
#endif
#define PMIC_HRST_REG19_np_hrst_reg19_START (0)
#define PMIC_HRST_REG19_np_hrst_reg19_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg20 : 8;
    } reg;
} PMIC_HRST_REG20_UNION;
#endif
#define PMIC_HRST_REG20_np_hrst_reg20_START (0)
#define PMIC_HRST_REG20_np_hrst_reg20_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg21 : 8;
    } reg;
} PMIC_HRST_REG21_UNION;
#endif
#define PMIC_HRST_REG21_np_hrst_reg21_START (0)
#define PMIC_HRST_REG21_np_hrst_reg21_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg22 : 8;
    } reg;
} PMIC_HRST_REG22_UNION;
#endif
#define PMIC_HRST_REG22_np_hrst_reg22_START (0)
#define PMIC_HRST_REG22_np_hrst_reg22_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg23 : 8;
    } reg;
} PMIC_HRST_REG23_UNION;
#endif
#define PMIC_HRST_REG23_np_hrst_reg23_START (0)
#define PMIC_HRST_REG23_np_hrst_reg23_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg24 : 8;
    } reg;
} PMIC_HRST_REG24_UNION;
#endif
#define PMIC_HRST_REG24_np_hrst_reg24_START (0)
#define PMIC_HRST_REG24_np_hrst_reg24_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg25 : 8;
    } reg;
} PMIC_HRST_REG25_UNION;
#endif
#define PMIC_HRST_REG25_np_hrst_reg25_START (0)
#define PMIC_HRST_REG25_np_hrst_reg25_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg26 : 8;
    } reg;
} PMIC_HRST_REG26_UNION;
#endif
#define PMIC_HRST_REG26_np_hrst_reg26_START (0)
#define PMIC_HRST_REG26_np_hrst_reg26_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg27 : 8;
    } reg;
} PMIC_HRST_REG27_UNION;
#endif
#define PMIC_HRST_REG27_np_hrst_reg27_START (0)
#define PMIC_HRST_REG27_np_hrst_reg27_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg28 : 8;
    } reg;
} PMIC_HRST_REG28_UNION;
#endif
#define PMIC_HRST_REG28_np_hrst_reg28_START (0)
#define PMIC_HRST_REG28_np_hrst_reg28_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg29 : 8;
    } reg;
} PMIC_HRST_REG29_UNION;
#endif
#define PMIC_HRST_REG29_np_hrst_reg29_START (0)
#define PMIC_HRST_REG29_np_hrst_reg29_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg30 : 8;
    } reg;
} PMIC_HRST_REG30_UNION;
#endif
#define PMIC_HRST_REG30_np_hrst_reg30_START (0)
#define PMIC_HRST_REG30_np_hrst_reg30_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_hrst_reg31 : 8;
    } reg;
} PMIC_HRST_REG31_UNION;
#endif
#define PMIC_HRST_REG31_np_hrst_reg31_START (0)
#define PMIC_HRST_REG31_np_hrst_reg31_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob0_d2a : 8;
    } reg;
} PMIC_NP_OTP0_0_R_UNION;
#endif
#define PMIC_NP_OTP0_0_R_np_otp0_pdob0_d2a_START (0)
#define PMIC_NP_OTP0_0_R_np_otp0_pdob0_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob1_d2a : 8;
    } reg;
} PMIC_NP_OTP0_1_R_UNION;
#endif
#define PMIC_NP_OTP0_1_R_np_otp0_pdob1_d2a_START (0)
#define PMIC_NP_OTP0_1_R_np_otp0_pdob1_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob2_d2a : 8;
    } reg;
} PMIC_NP_OTP0_2_R_UNION;
#endif
#define PMIC_NP_OTP0_2_R_np_otp0_pdob2_d2a_START (0)
#define PMIC_NP_OTP0_2_R_np_otp0_pdob2_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob3_d2a : 8;
    } reg;
} PMIC_NP_OTP0_3_R_UNION;
#endif
#define PMIC_NP_OTP0_3_R_np_otp0_pdob3_d2a_START (0)
#define PMIC_NP_OTP0_3_R_np_otp0_pdob3_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob4_d2a : 8;
    } reg;
} PMIC_NP_OTP0_4_R_UNION;
#endif
#define PMIC_NP_OTP0_4_R_np_otp0_pdob4_d2a_START (0)
#define PMIC_NP_OTP0_4_R_np_otp0_pdob4_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob5_d2a : 8;
    } reg;
} PMIC_NP_OTP0_5_R_UNION;
#endif
#define PMIC_NP_OTP0_5_R_np_otp0_pdob5_d2a_START (0)
#define PMIC_NP_OTP0_5_R_np_otp0_pdob5_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob6_d2a : 8;
    } reg;
} PMIC_NP_OTP0_6_R_UNION;
#endif
#define PMIC_NP_OTP0_6_R_np_otp0_pdob6_d2a_START (0)
#define PMIC_NP_OTP0_6_R_np_otp0_pdob6_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob7_d2a : 8;
    } reg;
} PMIC_NP_OTP0_7_R_UNION;
#endif
#define PMIC_NP_OTP0_7_R_np_otp0_pdob7_d2a_START (0)
#define PMIC_NP_OTP0_7_R_np_otp0_pdob7_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob8_d2a : 8;
    } reg;
} PMIC_NP_OTP0_8_R_UNION;
#endif
#define PMIC_NP_OTP0_8_R_np_otp0_pdob8_d2a_START (0)
#define PMIC_NP_OTP0_8_R_np_otp0_pdob8_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob9_d2a : 8;
    } reg;
} PMIC_NP_OTP0_9_R_UNION;
#endif
#define PMIC_NP_OTP0_9_R_np_otp0_pdob9_d2a_START (0)
#define PMIC_NP_OTP0_9_R_np_otp0_pdob9_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob10_d2a : 8;
    } reg;
} PMIC_NP_OTP0_10_R_UNION;
#endif
#define PMIC_NP_OTP0_10_R_np_otp0_pdob10_d2a_START (0)
#define PMIC_NP_OTP0_10_R_np_otp0_pdob10_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob11_d2a : 8;
    } reg;
} PMIC_NP_OTP0_11_R_UNION;
#endif
#define PMIC_NP_OTP0_11_R_np_otp0_pdob11_d2a_START (0)
#define PMIC_NP_OTP0_11_R_np_otp0_pdob11_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob12_d2a : 8;
    } reg;
} PMIC_NP_OTP0_12_R_UNION;
#endif
#define PMIC_NP_OTP0_12_R_np_otp0_pdob12_d2a_START (0)
#define PMIC_NP_OTP0_12_R_np_otp0_pdob12_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob13_d2a : 8;
    } reg;
} PMIC_NP_OTP0_13_R_UNION;
#endif
#define PMIC_NP_OTP0_13_R_np_otp0_pdob13_d2a_START (0)
#define PMIC_NP_OTP0_13_R_np_otp0_pdob13_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob14_d2a : 8;
    } reg;
} PMIC_NP_OTP0_14_R_UNION;
#endif
#define PMIC_NP_OTP0_14_R_np_otp0_pdob14_d2a_START (0)
#define PMIC_NP_OTP0_14_R_np_otp0_pdob14_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob15_d2a : 8;
    } reg;
} PMIC_NP_OTP0_15_R_UNION;
#endif
#define PMIC_NP_OTP0_15_R_np_otp0_pdob15_d2a_START (0)
#define PMIC_NP_OTP0_15_R_np_otp0_pdob15_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob16_d2a : 8;
    } reg;
} PMIC_NP_OTP0_16_R_UNION;
#endif
#define PMIC_NP_OTP0_16_R_np_otp0_pdob16_d2a_START (0)
#define PMIC_NP_OTP0_16_R_np_otp0_pdob16_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob17_d2a : 8;
    } reg;
} PMIC_NP_OTP0_17_R_UNION;
#endif
#define PMIC_NP_OTP0_17_R_np_otp0_pdob17_d2a_START (0)
#define PMIC_NP_OTP0_17_R_np_otp0_pdob17_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob18_d2a : 8;
    } reg;
} PMIC_NP_OTP0_18_R_UNION;
#endif
#define PMIC_NP_OTP0_18_R_np_otp0_pdob18_d2a_START (0)
#define PMIC_NP_OTP0_18_R_np_otp0_pdob18_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob19_d2a : 8;
    } reg;
} PMIC_NP_OTP0_19_R_UNION;
#endif
#define PMIC_NP_OTP0_19_R_np_otp0_pdob19_d2a_START (0)
#define PMIC_NP_OTP0_19_R_np_otp0_pdob19_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob20_d2a : 8;
    } reg;
} PMIC_NP_OTP0_20_R_UNION;
#endif
#define PMIC_NP_OTP0_20_R_np_otp0_pdob20_d2a_START (0)
#define PMIC_NP_OTP0_20_R_np_otp0_pdob20_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob21_d2a : 8;
    } reg;
} PMIC_NP_OTP0_21_R_UNION;
#endif
#define PMIC_NP_OTP0_21_R_np_otp0_pdob21_d2a_START (0)
#define PMIC_NP_OTP0_21_R_np_otp0_pdob21_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob22_d2a : 8;
    } reg;
} PMIC_NP_OTP0_22_R_UNION;
#endif
#define PMIC_NP_OTP0_22_R_np_otp0_pdob22_d2a_START (0)
#define PMIC_NP_OTP0_22_R_np_otp0_pdob22_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob23_d2a : 8;
    } reg;
} PMIC_NP_OTP0_23_R_UNION;
#endif
#define PMIC_NP_OTP0_23_R_np_otp0_pdob23_d2a_START (0)
#define PMIC_NP_OTP0_23_R_np_otp0_pdob23_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob24_d2a : 8;
    } reg;
} PMIC_NP_OTP0_24_R_UNION;
#endif
#define PMIC_NP_OTP0_24_R_np_otp0_pdob24_d2a_START (0)
#define PMIC_NP_OTP0_24_R_np_otp0_pdob24_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob25_d2a : 8;
    } reg;
} PMIC_NP_OTP0_25_R_UNION;
#endif
#define PMIC_NP_OTP0_25_R_np_otp0_pdob25_d2a_START (0)
#define PMIC_NP_OTP0_25_R_np_otp0_pdob25_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob26_d2a : 8;
    } reg;
} PMIC_NP_OTP0_26_R_UNION;
#endif
#define PMIC_NP_OTP0_26_R_np_otp0_pdob26_d2a_START (0)
#define PMIC_NP_OTP0_26_R_np_otp0_pdob26_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob27_d2a : 8;
    } reg;
} PMIC_NP_OTP0_27_R_UNION;
#endif
#define PMIC_NP_OTP0_27_R_np_otp0_pdob27_d2a_START (0)
#define PMIC_NP_OTP0_27_R_np_otp0_pdob27_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob28_d2a : 8;
    } reg;
} PMIC_NP_OTP0_28_R_UNION;
#endif
#define PMIC_NP_OTP0_28_R_np_otp0_pdob28_d2a_START (0)
#define PMIC_NP_OTP0_28_R_np_otp0_pdob28_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob29_d2a : 8;
    } reg;
} PMIC_NP_OTP0_29_R_UNION;
#endif
#define PMIC_NP_OTP0_29_R_np_otp0_pdob29_d2a_START (0)
#define PMIC_NP_OTP0_29_R_np_otp0_pdob29_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob30_d2a : 8;
    } reg;
} PMIC_NP_OTP0_30_R_UNION;
#endif
#define PMIC_NP_OTP0_30_R_np_otp0_pdob30_d2a_START (0)
#define PMIC_NP_OTP0_30_R_np_otp0_pdob30_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob31_d2a : 8;
    } reg;
} PMIC_NP_OTP0_31_R_UNION;
#endif
#define PMIC_NP_OTP0_31_R_np_otp0_pdob31_d2a_START (0)
#define PMIC_NP_OTP0_31_R_np_otp0_pdob31_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob32_d2a : 8;
    } reg;
} PMIC_NP_OTP0_32_R_UNION;
#endif
#define PMIC_NP_OTP0_32_R_np_otp0_pdob32_d2a_START (0)
#define PMIC_NP_OTP0_32_R_np_otp0_pdob32_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob33_d2a : 8;
    } reg;
} PMIC_NP_OTP0_33_R_UNION;
#endif
#define PMIC_NP_OTP0_33_R_np_otp0_pdob33_d2a_START (0)
#define PMIC_NP_OTP0_33_R_np_otp0_pdob33_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob34_d2a : 8;
    } reg;
} PMIC_NP_OTP0_34_R_UNION;
#endif
#define PMIC_NP_OTP0_34_R_np_otp0_pdob34_d2a_START (0)
#define PMIC_NP_OTP0_34_R_np_otp0_pdob34_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob35_d2a : 8;
    } reg;
} PMIC_NP_OTP0_35_R_UNION;
#endif
#define PMIC_NP_OTP0_35_R_np_otp0_pdob35_d2a_START (0)
#define PMIC_NP_OTP0_35_R_np_otp0_pdob35_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob36_d2a : 8;
    } reg;
} PMIC_NP_OTP0_36_R_UNION;
#endif
#define PMIC_NP_OTP0_36_R_np_otp0_pdob36_d2a_START (0)
#define PMIC_NP_OTP0_36_R_np_otp0_pdob36_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob37_d2a : 8;
    } reg;
} PMIC_NP_OTP0_37_R_UNION;
#endif
#define PMIC_NP_OTP0_37_R_np_otp0_pdob37_d2a_START (0)
#define PMIC_NP_OTP0_37_R_np_otp0_pdob37_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob38_d2a : 8;
    } reg;
} PMIC_NP_OTP0_38_R_UNION;
#endif
#define PMIC_NP_OTP0_38_R_np_otp0_pdob38_d2a_START (0)
#define PMIC_NP_OTP0_38_R_np_otp0_pdob38_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob39_d2a : 8;
    } reg;
} PMIC_NP_OTP0_39_R_UNION;
#endif
#define PMIC_NP_OTP0_39_R_np_otp0_pdob39_d2a_START (0)
#define PMIC_NP_OTP0_39_R_np_otp0_pdob39_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob40_d2a : 8;
    } reg;
} PMIC_NP_OTP0_40_R_UNION;
#endif
#define PMIC_NP_OTP0_40_R_np_otp0_pdob40_d2a_START (0)
#define PMIC_NP_OTP0_40_R_np_otp0_pdob40_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob41_d2a : 8;
    } reg;
} PMIC_NP_OTP0_41_R_UNION;
#endif
#define PMIC_NP_OTP0_41_R_np_otp0_pdob41_d2a_START (0)
#define PMIC_NP_OTP0_41_R_np_otp0_pdob41_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob42_d2a : 8;
    } reg;
} PMIC_NP_OTP0_42_R_UNION;
#endif
#define PMIC_NP_OTP0_42_R_np_otp0_pdob42_d2a_START (0)
#define PMIC_NP_OTP0_42_R_np_otp0_pdob42_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob43_d2a : 8;
    } reg;
} PMIC_NP_OTP0_43_R_UNION;
#endif
#define PMIC_NP_OTP0_43_R_np_otp0_pdob43_d2a_START (0)
#define PMIC_NP_OTP0_43_R_np_otp0_pdob43_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob44_d2a : 8;
    } reg;
} PMIC_NP_OTP0_44_R_UNION;
#endif
#define PMIC_NP_OTP0_44_R_np_otp0_pdob44_d2a_START (0)
#define PMIC_NP_OTP0_44_R_np_otp0_pdob44_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob45_d2a : 8;
    } reg;
} PMIC_NP_OTP0_45_R_UNION;
#endif
#define PMIC_NP_OTP0_45_R_np_otp0_pdob45_d2a_START (0)
#define PMIC_NP_OTP0_45_R_np_otp0_pdob45_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob46_d2a : 8;
    } reg;
} PMIC_NP_OTP0_46_R_UNION;
#endif
#define PMIC_NP_OTP0_46_R_np_otp0_pdob46_d2a_START (0)
#define PMIC_NP_OTP0_46_R_np_otp0_pdob46_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob47_d2a : 8;
    } reg;
} PMIC_NP_OTP0_47_R_UNION;
#endif
#define PMIC_NP_OTP0_47_R_np_otp0_pdob47_d2a_START (0)
#define PMIC_NP_OTP0_47_R_np_otp0_pdob47_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob48_d2a : 8;
    } reg;
} PMIC_NP_OTP0_48_R_UNION;
#endif
#define PMIC_NP_OTP0_48_R_np_otp0_pdob48_d2a_START (0)
#define PMIC_NP_OTP0_48_R_np_otp0_pdob48_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob49_d2a : 8;
    } reg;
} PMIC_NP_OTP0_49_R_UNION;
#endif
#define PMIC_NP_OTP0_49_R_np_otp0_pdob49_d2a_START (0)
#define PMIC_NP_OTP0_49_R_np_otp0_pdob49_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob50_d2a : 8;
    } reg;
} PMIC_NP_OTP0_50_R_UNION;
#endif
#define PMIC_NP_OTP0_50_R_np_otp0_pdob50_d2a_START (0)
#define PMIC_NP_OTP0_50_R_np_otp0_pdob50_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob51_d2a : 8;
    } reg;
} PMIC_NP_OTP0_51_R_UNION;
#endif
#define PMIC_NP_OTP0_51_R_np_otp0_pdob51_d2a_START (0)
#define PMIC_NP_OTP0_51_R_np_otp0_pdob51_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob52_d2a : 8;
    } reg;
} PMIC_NP_OTP0_52_R_UNION;
#endif
#define PMIC_NP_OTP0_52_R_np_otp0_pdob52_d2a_START (0)
#define PMIC_NP_OTP0_52_R_np_otp0_pdob52_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob53_d2a : 8;
    } reg;
} PMIC_NP_OTP0_53_R_UNION;
#endif
#define PMIC_NP_OTP0_53_R_np_otp0_pdob53_d2a_START (0)
#define PMIC_NP_OTP0_53_R_np_otp0_pdob53_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob54_d2a : 8;
    } reg;
} PMIC_NP_OTP0_54_R_UNION;
#endif
#define PMIC_NP_OTP0_54_R_np_otp0_pdob54_d2a_START (0)
#define PMIC_NP_OTP0_54_R_np_otp0_pdob54_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob55_d2a : 8;
    } reg;
} PMIC_NP_OTP0_55_R_UNION;
#endif
#define PMIC_NP_OTP0_55_R_np_otp0_pdob55_d2a_START (0)
#define PMIC_NP_OTP0_55_R_np_otp0_pdob55_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob56_d2a : 8;
    } reg;
} PMIC_NP_OTP0_56_R_UNION;
#endif
#define PMIC_NP_OTP0_56_R_np_otp0_pdob56_d2a_START (0)
#define PMIC_NP_OTP0_56_R_np_otp0_pdob56_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob57_d2a : 8;
    } reg;
} PMIC_NP_OTP0_57_R_UNION;
#endif
#define PMIC_NP_OTP0_57_R_np_otp0_pdob57_d2a_START (0)
#define PMIC_NP_OTP0_57_R_np_otp0_pdob57_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob58_d2a : 8;
    } reg;
} PMIC_NP_OTP0_58_R_UNION;
#endif
#define PMIC_NP_OTP0_58_R_np_otp0_pdob58_d2a_START (0)
#define PMIC_NP_OTP0_58_R_np_otp0_pdob58_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob59_d2a : 8;
    } reg;
} PMIC_NP_OTP0_59_R_UNION;
#endif
#define PMIC_NP_OTP0_59_R_np_otp0_pdob59_d2a_START (0)
#define PMIC_NP_OTP0_59_R_np_otp0_pdob59_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob60_d2a : 8;
    } reg;
} PMIC_NP_OTP0_60_R_UNION;
#endif
#define PMIC_NP_OTP0_60_R_np_otp0_pdob60_d2a_START (0)
#define PMIC_NP_OTP0_60_R_np_otp0_pdob60_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob61_d2a : 8;
    } reg;
} PMIC_NP_OTP0_61_R_UNION;
#endif
#define PMIC_NP_OTP0_61_R_np_otp0_pdob61_d2a_START (0)
#define PMIC_NP_OTP0_61_R_np_otp0_pdob61_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob62_d2a : 8;
    } reg;
} PMIC_NP_OTP0_62_R_UNION;
#endif
#define PMIC_NP_OTP0_62_R_np_otp0_pdob62_d2a_START (0)
#define PMIC_NP_OTP0_62_R_np_otp0_pdob62_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp0_pdob63_d2a : 8;
    } reg;
} PMIC_NP_OTP0_63_R_UNION;
#endif
#define PMIC_NP_OTP0_63_R_np_otp0_pdob63_d2a_START (0)
#define PMIC_NP_OTP0_63_R_np_otp0_pdob63_d2a_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob0 : 8;
    } reg;
} PMIC_NP_OTP1_0_R_UNION;
#endif
#define PMIC_NP_OTP1_0_R_np_otp1_pdob0_START (0)
#define PMIC_NP_OTP1_0_R_np_otp1_pdob0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob1 : 8;
    } reg;
} PMIC_NP_OTP1_1_R_UNION;
#endif
#define PMIC_NP_OTP1_1_R_np_otp1_pdob1_START (0)
#define PMIC_NP_OTP1_1_R_np_otp1_pdob1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob2 : 8;
    } reg;
} PMIC_NP_OTP1_2_R_UNION;
#endif
#define PMIC_NP_OTP1_2_R_np_otp1_pdob2_START (0)
#define PMIC_NP_OTP1_2_R_np_otp1_pdob2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob3 : 8;
    } reg;
} PMIC_NP_OTP1_3_R_UNION;
#endif
#define PMIC_NP_OTP1_3_R_np_otp1_pdob3_START (0)
#define PMIC_NP_OTP1_3_R_np_otp1_pdob3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob4 : 8;
    } reg;
} PMIC_NP_OTP1_4_R_UNION;
#endif
#define PMIC_NP_OTP1_4_R_np_otp1_pdob4_START (0)
#define PMIC_NP_OTP1_4_R_np_otp1_pdob4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob5 : 8;
    } reg;
} PMIC_NP_OTP1_5_R_UNION;
#endif
#define PMIC_NP_OTP1_5_R_np_otp1_pdob5_START (0)
#define PMIC_NP_OTP1_5_R_np_otp1_pdob5_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob6 : 8;
    } reg;
} PMIC_NP_OTP1_6_R_UNION;
#endif
#define PMIC_NP_OTP1_6_R_np_otp1_pdob6_START (0)
#define PMIC_NP_OTP1_6_R_np_otp1_pdob6_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob7 : 8;
    } reg;
} PMIC_NP_OTP1_7_R_UNION;
#endif
#define PMIC_NP_OTP1_7_R_np_otp1_pdob7_START (0)
#define PMIC_NP_OTP1_7_R_np_otp1_pdob7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob8 : 8;
    } reg;
} PMIC_NP_OTP1_8_R_UNION;
#endif
#define PMIC_NP_OTP1_8_R_np_otp1_pdob8_START (0)
#define PMIC_NP_OTP1_8_R_np_otp1_pdob8_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob9 : 8;
    } reg;
} PMIC_NP_OTP1_9_R_UNION;
#endif
#define PMIC_NP_OTP1_9_R_np_otp1_pdob9_START (0)
#define PMIC_NP_OTP1_9_R_np_otp1_pdob9_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob10 : 8;
    } reg;
} PMIC_NP_OTP1_10_R_UNION;
#endif
#define PMIC_NP_OTP1_10_R_np_otp1_pdob10_START (0)
#define PMIC_NP_OTP1_10_R_np_otp1_pdob10_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob11 : 8;
    } reg;
} PMIC_NP_OTP1_11_R_UNION;
#endif
#define PMIC_NP_OTP1_11_R_np_otp1_pdob11_START (0)
#define PMIC_NP_OTP1_11_R_np_otp1_pdob11_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob12 : 8;
    } reg;
} PMIC_NP_OTP1_12_R_UNION;
#endif
#define PMIC_NP_OTP1_12_R_np_otp1_pdob12_START (0)
#define PMIC_NP_OTP1_12_R_np_otp1_pdob12_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob13 : 8;
    } reg;
} PMIC_NP_OTP1_13_R_UNION;
#endif
#define PMIC_NP_OTP1_13_R_np_otp1_pdob13_START (0)
#define PMIC_NP_OTP1_13_R_np_otp1_pdob13_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob14 : 8;
    } reg;
} PMIC_NP_OTP1_14_R_UNION;
#endif
#define PMIC_NP_OTP1_14_R_np_otp1_pdob14_START (0)
#define PMIC_NP_OTP1_14_R_np_otp1_pdob14_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob15 : 8;
    } reg;
} PMIC_NP_OTP1_15_R_UNION;
#endif
#define PMIC_NP_OTP1_15_R_np_otp1_pdob15_START (0)
#define PMIC_NP_OTP1_15_R_np_otp1_pdob15_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob16 : 8;
    } reg;
} PMIC_NP_OTP1_16_R_UNION;
#endif
#define PMIC_NP_OTP1_16_R_np_otp1_pdob16_START (0)
#define PMIC_NP_OTP1_16_R_np_otp1_pdob16_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob17 : 8;
    } reg;
} PMIC_NP_OTP1_17_R_UNION;
#endif
#define PMIC_NP_OTP1_17_R_np_otp1_pdob17_START (0)
#define PMIC_NP_OTP1_17_R_np_otp1_pdob17_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob18 : 8;
    } reg;
} PMIC_NP_OTP1_18_R_UNION;
#endif
#define PMIC_NP_OTP1_18_R_np_otp1_pdob18_START (0)
#define PMIC_NP_OTP1_18_R_np_otp1_pdob18_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob19 : 8;
    } reg;
} PMIC_NP_OTP1_19_R_UNION;
#endif
#define PMIC_NP_OTP1_19_R_np_otp1_pdob19_START (0)
#define PMIC_NP_OTP1_19_R_np_otp1_pdob19_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob20 : 8;
    } reg;
} PMIC_NP_OTP1_20_R_UNION;
#endif
#define PMIC_NP_OTP1_20_R_np_otp1_pdob20_START (0)
#define PMIC_NP_OTP1_20_R_np_otp1_pdob20_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob21 : 8;
    } reg;
} PMIC_NP_OTP1_21_R_UNION;
#endif
#define PMIC_NP_OTP1_21_R_np_otp1_pdob21_START (0)
#define PMIC_NP_OTP1_21_R_np_otp1_pdob21_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob22 : 8;
    } reg;
} PMIC_NP_OTP1_22_R_UNION;
#endif
#define PMIC_NP_OTP1_22_R_np_otp1_pdob22_START (0)
#define PMIC_NP_OTP1_22_R_np_otp1_pdob22_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob23 : 8;
    } reg;
} PMIC_NP_OTP1_23_R_UNION;
#endif
#define PMIC_NP_OTP1_23_R_np_otp1_pdob23_START (0)
#define PMIC_NP_OTP1_23_R_np_otp1_pdob23_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob24 : 8;
    } reg;
} PMIC_NP_OTP1_24_R_UNION;
#endif
#define PMIC_NP_OTP1_24_R_np_otp1_pdob24_START (0)
#define PMIC_NP_OTP1_24_R_np_otp1_pdob24_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob25 : 8;
    } reg;
} PMIC_NP_OTP1_25_R_UNION;
#endif
#define PMIC_NP_OTP1_25_R_np_otp1_pdob25_START (0)
#define PMIC_NP_OTP1_25_R_np_otp1_pdob25_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob26 : 8;
    } reg;
} PMIC_NP_OTP1_26_R_UNION;
#endif
#define PMIC_NP_OTP1_26_R_np_otp1_pdob26_START (0)
#define PMIC_NP_OTP1_26_R_np_otp1_pdob26_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob27 : 8;
    } reg;
} PMIC_NP_OTP1_27_R_UNION;
#endif
#define PMIC_NP_OTP1_27_R_np_otp1_pdob27_START (0)
#define PMIC_NP_OTP1_27_R_np_otp1_pdob27_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob28 : 8;
    } reg;
} PMIC_NP_OTP1_28_R_UNION;
#endif
#define PMIC_NP_OTP1_28_R_np_otp1_pdob28_START (0)
#define PMIC_NP_OTP1_28_R_np_otp1_pdob28_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob29 : 8;
    } reg;
} PMIC_NP_OTP1_29_R_UNION;
#endif
#define PMIC_NP_OTP1_29_R_np_otp1_pdob29_START (0)
#define PMIC_NP_OTP1_29_R_np_otp1_pdob29_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob30 : 8;
    } reg;
} PMIC_NP_OTP1_30_R_UNION;
#endif
#define PMIC_NP_OTP1_30_R_np_otp1_pdob30_START (0)
#define PMIC_NP_OTP1_30_R_np_otp1_pdob30_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob31 : 8;
    } reg;
} PMIC_NP_OTP1_31_R_UNION;
#endif
#define PMIC_NP_OTP1_31_R_np_otp1_pdob31_START (0)
#define PMIC_NP_OTP1_31_R_np_otp1_pdob31_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob32 : 8;
    } reg;
} PMIC_NP_OTP1_32_R_UNION;
#endif
#define PMIC_NP_OTP1_32_R_np_otp1_pdob32_START (0)
#define PMIC_NP_OTP1_32_R_np_otp1_pdob32_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob33 : 8;
    } reg;
} PMIC_NP_OTP1_33_R_UNION;
#endif
#define PMIC_NP_OTP1_33_R_np_otp1_pdob33_START (0)
#define PMIC_NP_OTP1_33_R_np_otp1_pdob33_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob34 : 8;
    } reg;
} PMIC_NP_OTP1_34_R_UNION;
#endif
#define PMIC_NP_OTP1_34_R_np_otp1_pdob34_START (0)
#define PMIC_NP_OTP1_34_R_np_otp1_pdob34_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob35 : 8;
    } reg;
} PMIC_NP_OTP1_35_R_UNION;
#endif
#define PMIC_NP_OTP1_35_R_np_otp1_pdob35_START (0)
#define PMIC_NP_OTP1_35_R_np_otp1_pdob35_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob36 : 8;
    } reg;
} PMIC_NP_OTP1_36_R_UNION;
#endif
#define PMIC_NP_OTP1_36_R_np_otp1_pdob36_START (0)
#define PMIC_NP_OTP1_36_R_np_otp1_pdob36_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob37 : 8;
    } reg;
} PMIC_NP_OTP1_37_R_UNION;
#endif
#define PMIC_NP_OTP1_37_R_np_otp1_pdob37_START (0)
#define PMIC_NP_OTP1_37_R_np_otp1_pdob37_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob38 : 8;
    } reg;
} PMIC_NP_OTP1_38_R_UNION;
#endif
#define PMIC_NP_OTP1_38_R_np_otp1_pdob38_START (0)
#define PMIC_NP_OTP1_38_R_np_otp1_pdob38_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob39 : 8;
    } reg;
} PMIC_NP_OTP1_39_R_UNION;
#endif
#define PMIC_NP_OTP1_39_R_np_otp1_pdob39_START (0)
#define PMIC_NP_OTP1_39_R_np_otp1_pdob39_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob40 : 8;
    } reg;
} PMIC_NP_OTP1_40_R_UNION;
#endif
#define PMIC_NP_OTP1_40_R_np_otp1_pdob40_START (0)
#define PMIC_NP_OTP1_40_R_np_otp1_pdob40_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob41 : 8;
    } reg;
} PMIC_NP_OTP1_41_R_UNION;
#endif
#define PMIC_NP_OTP1_41_R_np_otp1_pdob41_START (0)
#define PMIC_NP_OTP1_41_R_np_otp1_pdob41_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob42 : 8;
    } reg;
} PMIC_NP_OTP1_42_R_UNION;
#endif
#define PMIC_NP_OTP1_42_R_np_otp1_pdob42_START (0)
#define PMIC_NP_OTP1_42_R_np_otp1_pdob42_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob43 : 8;
    } reg;
} PMIC_NP_OTP1_43_R_UNION;
#endif
#define PMIC_NP_OTP1_43_R_np_otp1_pdob43_START (0)
#define PMIC_NP_OTP1_43_R_np_otp1_pdob43_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob44 : 8;
    } reg;
} PMIC_NP_OTP1_44_R_UNION;
#endif
#define PMIC_NP_OTP1_44_R_np_otp1_pdob44_START (0)
#define PMIC_NP_OTP1_44_R_np_otp1_pdob44_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob45 : 8;
    } reg;
} PMIC_NP_OTP1_45_R_UNION;
#endif
#define PMIC_NP_OTP1_45_R_np_otp1_pdob45_START (0)
#define PMIC_NP_OTP1_45_R_np_otp1_pdob45_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob46 : 8;
    } reg;
} PMIC_NP_OTP1_46_R_UNION;
#endif
#define PMIC_NP_OTP1_46_R_np_otp1_pdob46_START (0)
#define PMIC_NP_OTP1_46_R_np_otp1_pdob46_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob47 : 8;
    } reg;
} PMIC_NP_OTP1_47_R_UNION;
#endif
#define PMIC_NP_OTP1_47_R_np_otp1_pdob47_START (0)
#define PMIC_NP_OTP1_47_R_np_otp1_pdob47_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob48 : 8;
    } reg;
} PMIC_NP_OTP1_48_R_UNION;
#endif
#define PMIC_NP_OTP1_48_R_np_otp1_pdob48_START (0)
#define PMIC_NP_OTP1_48_R_np_otp1_pdob48_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob49 : 8;
    } reg;
} PMIC_NP_OTP1_49_R_UNION;
#endif
#define PMIC_NP_OTP1_49_R_np_otp1_pdob49_START (0)
#define PMIC_NP_OTP1_49_R_np_otp1_pdob49_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob50 : 8;
    } reg;
} PMIC_NP_OTP1_50_R_UNION;
#endif
#define PMIC_NP_OTP1_50_R_np_otp1_pdob50_START (0)
#define PMIC_NP_OTP1_50_R_np_otp1_pdob50_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob51 : 8;
    } reg;
} PMIC_NP_OTP1_51_R_UNION;
#endif
#define PMIC_NP_OTP1_51_R_np_otp1_pdob51_START (0)
#define PMIC_NP_OTP1_51_R_np_otp1_pdob51_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob52 : 8;
    } reg;
} PMIC_NP_OTP1_52_R_UNION;
#endif
#define PMIC_NP_OTP1_52_R_np_otp1_pdob52_START (0)
#define PMIC_NP_OTP1_52_R_np_otp1_pdob52_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob53 : 8;
    } reg;
} PMIC_NP_OTP1_53_R_UNION;
#endif
#define PMIC_NP_OTP1_53_R_np_otp1_pdob53_START (0)
#define PMIC_NP_OTP1_53_R_np_otp1_pdob53_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob54 : 8;
    } reg;
} PMIC_NP_OTP1_54_R_UNION;
#endif
#define PMIC_NP_OTP1_54_R_np_otp1_pdob54_START (0)
#define PMIC_NP_OTP1_54_R_np_otp1_pdob54_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob55 : 8;
    } reg;
} PMIC_NP_OTP1_55_R_UNION;
#endif
#define PMIC_NP_OTP1_55_R_np_otp1_pdob55_START (0)
#define PMIC_NP_OTP1_55_R_np_otp1_pdob55_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob56 : 8;
    } reg;
} PMIC_NP_OTP1_56_R_UNION;
#endif
#define PMIC_NP_OTP1_56_R_np_otp1_pdob56_START (0)
#define PMIC_NP_OTP1_56_R_np_otp1_pdob56_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob57 : 8;
    } reg;
} PMIC_NP_OTP1_57_R_UNION;
#endif
#define PMIC_NP_OTP1_57_R_np_otp1_pdob57_START (0)
#define PMIC_NP_OTP1_57_R_np_otp1_pdob57_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob58 : 8;
    } reg;
} PMIC_NP_OTP1_58_R_UNION;
#endif
#define PMIC_NP_OTP1_58_R_np_otp1_pdob58_START (0)
#define PMIC_NP_OTP1_58_R_np_otp1_pdob58_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob59 : 8;
    } reg;
} PMIC_NP_OTP1_59_R_UNION;
#endif
#define PMIC_NP_OTP1_59_R_np_otp1_pdob59_START (0)
#define PMIC_NP_OTP1_59_R_np_otp1_pdob59_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob60 : 8;
    } reg;
} PMIC_NP_OTP1_60_R_UNION;
#endif
#define PMIC_NP_OTP1_60_R_np_otp1_pdob60_START (0)
#define PMIC_NP_OTP1_60_R_np_otp1_pdob60_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob61 : 8;
    } reg;
} PMIC_NP_OTP1_61_R_UNION;
#endif
#define PMIC_NP_OTP1_61_R_np_otp1_pdob61_START (0)
#define PMIC_NP_OTP1_61_R_np_otp1_pdob61_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob62 : 8;
    } reg;
} PMIC_NP_OTP1_62_R_UNION;
#endif
#define PMIC_NP_OTP1_62_R_np_otp1_pdob62_START (0)
#define PMIC_NP_OTP1_62_R_np_otp1_pdob62_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_otp1_pdob63 : 8;
    } reg;
} PMIC_NP_OTP1_63_R_UNION;
#endif
#define PMIC_NP_OTP1_63_R_np_otp1_pdob63_START (0)
#define PMIC_NP_OTP1_63_R_np_otp1_pdob63_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_pmud_buck_en_cfg : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_NP_EN_PMUD_UNION;
#endif
#define PMIC_NP_EN_PMUD_np_pmud_buck_en_cfg_START (0)
#define PMIC_NP_EN_PMUD_np_pmud_buck_en_cfg_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_nfc_pwron_time_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_NP_NFC_PWRON_TIME_UNION;
#endif
#define PMIC_NP_NFC_PWRON_TIME_np_nfc_pwron_time_sel_START (0)
#define PMIC_NP_NFC_PWRON_TIME_np_nfc_pwron_time_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_voice_resart1_mask : 1;
        unsigned char np_voice_resart2_mask : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_NP_VIOCE_RESTART_CTRL_UNION;
#endif
#define PMIC_NP_VIOCE_RESTART_CTRL_np_voice_resart1_mask_START (0)
#define PMIC_NP_VIOCE_RESTART_CTRL_np_voice_resart1_mask_END (0)
#define PMIC_NP_VIOCE_RESTART_CTRL_np_voice_resart2_mask_START (1)
#define PMIC_NP_VIOCE_RESTART_CTRL_np_voice_resart2_mask_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_pwr_onoff_place : 8;
    } reg;
} PMIC_NP_PWR_ONOFF_SEQ_CTRL_UNION;
#endif
#define PMIC_NP_PWR_ONOFF_SEQ_CTRL_np_pwr_onoff_place_START (0)
#define PMIC_NP_PWR_ONOFF_SEQ_CTRL_np_pwr_onoff_place_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck1_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_BUCK1_ON_PLACE_UNION;
#endif
#define PMIC_NP_BUCK1_ON_PLACE_np_buck1_on_place_START (0)
#define PMIC_NP_BUCK1_ON_PLACE_np_buck1_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck40_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_BUCK40_ON_PLACE_UNION;
#endif
#define PMIC_NP_BUCK40_ON_PLACE_np_buck40_on_place_START (0)
#define PMIC_NP_BUCK40_ON_PLACE_np_buck40_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck42_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_BUCK42_ON_PLACE_UNION;
#endif
#define PMIC_NP_BUCK42_ON_PLACE_np_buck42_on_place_START (0)
#define PMIC_NP_BUCK42_ON_PLACE_np_buck42_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck5_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_BUCK5_ON_PLACE_UNION;
#endif
#define PMIC_NP_BUCK5_ON_PLACE_np_buck5_on_place_START (0)
#define PMIC_NP_BUCK5_ON_PLACE_np_buck5_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck70_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_BUCK70_ON_PLACE_UNION;
#endif
#define PMIC_NP_BUCK70_ON_PLACE_np_buck70_on_place_START (0)
#define PMIC_NP_BUCK70_ON_PLACE_np_buck70_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck71_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_BUCK71_ON_PLACE_UNION;
#endif
#define PMIC_NP_BUCK71_ON_PLACE_np_buck71_on_place_START (0)
#define PMIC_NP_BUCK71_ON_PLACE_np_buck71_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck9_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_BUCK9_ON_PLACE_UNION;
#endif
#define PMIC_NP_BUCK9_ON_PLACE_np_buck9_on_place_START (0)
#define PMIC_NP_BUCK9_ON_PLACE_np_buck9_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo0_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO0_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO0_ON_PLACE_np_ldo0_on_place_START (0)
#define PMIC_NP_LDO0_ON_PLACE_np_ldo0_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo4_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO4_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO4_ON_PLACE_np_ldo4_on_place_START (0)
#define PMIC_NP_LDO4_ON_PLACE_np_ldo4_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo6_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO6_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO6_ON_PLACE_np_ldo6_on_place_START (0)
#define PMIC_NP_LDO6_ON_PLACE_np_ldo6_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo8_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO8_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO8_ON_PLACE_np_ldo8_on_place_START (0)
#define PMIC_NP_LDO8_ON_PLACE_np_ldo8_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo15_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO15_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO15_ON_PLACE_np_ldo15_on_place_START (0)
#define PMIC_NP_LDO15_ON_PLACE_np_ldo15_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo17_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO17_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO17_ON_PLACE_np_ldo17_on_place_START (0)
#define PMIC_NP_LDO17_ON_PLACE_np_ldo17_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo18_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO18_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO18_ON_PLACE_np_ldo18_on_place_START (0)
#define PMIC_NP_LDO18_ON_PLACE_np_ldo18_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo23_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO23_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO23_ON_PLACE_np_ldo23_on_place_START (0)
#define PMIC_NP_LDO23_ON_PLACE_np_ldo23_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo24_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO24_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO24_ON_PLACE_np_ldo24_on_place_START (0)
#define PMIC_NP_LDO24_ON_PLACE_np_ldo24_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo28_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO28_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO28_ON_PLACE_np_ldo28_on_place_START (0)
#define PMIC_NP_LDO28_ON_PLACE_np_ldo28_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo30_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO30_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO30_ON_PLACE_np_ldo30_on_place_START (0)
#define PMIC_NP_LDO30_ON_PLACE_np_ldo30_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo32_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO32_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO32_ON_PLACE_np_ldo32_on_place_START (0)
#define PMIC_NP_LDO32_ON_PLACE_np_ldo32_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo36_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO36_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO36_ON_PLACE_np_ldo36_on_place_START (0)
#define PMIC_NP_LDO36_ON_PLACE_np_ldo36_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo37_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO37_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO37_ON_PLACE_np_ldo37_on_place_START (0)
#define PMIC_NP_LDO37_ON_PLACE_np_ldo37_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo38_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO38_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO38_ON_PLACE_np_ldo38_on_place_START (0)
#define PMIC_NP_LDO38_ON_PLACE_np_ldo38_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo39_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO39_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO39_ON_PLACE_np_ldo39_on_place_START (0)
#define PMIC_NP_LDO39_ON_PLACE_np_ldo39_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_NP_LDO40_ON_PLACE_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo42_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO42_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO42_ON_PLACE_np_ldo42_on_place_START (0)
#define PMIC_NP_LDO42_ON_PLACE_np_ldo42_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo43_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO43_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO43_ON_PLACE_np_ldo43_on_place_START (0)
#define PMIC_NP_LDO43_ON_PLACE_np_ldo43_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo44_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO44_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO44_ON_PLACE_np_ldo44_on_place_START (0)
#define PMIC_NP_LDO44_ON_PLACE_np_ldo44_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo45_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO45_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO45_ON_PLACE_np_ldo45_on_place_START (0)
#define PMIC_NP_LDO45_ON_PLACE_np_ldo45_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo46_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO46_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO46_ON_PLACE_np_ldo46_on_place_START (0)
#define PMIC_NP_LDO46_ON_PLACE_np_ldo46_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo_buf_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO_BUF_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO_BUF_ON_PLACE_np_ldo_buf_on_place_START (0)
#define PMIC_NP_LDO_BUF_ON_PLACE_np_ldo_buf_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo_sink_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO_SINK_ON_PLACE_UNION;
#endif
#define PMIC_NP_LDO_SINK_ON_PLACE_np_ldo_sink_on_place_START (0)
#define PMIC_NP_LDO_SINK_ON_PLACE_np_ldo_sink_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sw1_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_SW1_ON_PLACE_UNION;
#endif
#define PMIC_NP_SW1_ON_PLACE_np_sw1_on_place_START (0)
#define PMIC_NP_SW1_ON_PLACE_np_sw1_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_pmu_en_on_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_PMU_EN_ON_PLACE_UNION;
#endif
#define PMIC_NP_PMU_EN_ON_PLACE_np_pmu_en_on_place_START (0)
#define PMIC_NP_PMU_EN_ON_PLACE_np_pmu_en_on_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck1_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_BUCK1_OFF_PLACE_UNION;
#endif
#define PMIC_NP_BUCK1_OFF_PLACE_np_buck1_off_place_START (0)
#define PMIC_NP_BUCK1_OFF_PLACE_np_buck1_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck40_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_BUCK40_OFF_PLACE_UNION;
#endif
#define PMIC_NP_BUCK40_OFF_PLACE_np_buck40_off_place_START (0)
#define PMIC_NP_BUCK40_OFF_PLACE_np_buck40_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck42_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_BUCK42_OFF_PLACE_UNION;
#endif
#define PMIC_NP_BUCK42_OFF_PLACE_np_buck42_off_place_START (0)
#define PMIC_NP_BUCK42_OFF_PLACE_np_buck42_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck5_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_BUCK5_OFF_PLACE_UNION;
#endif
#define PMIC_NP_BUCK5_OFF_PLACE_np_buck5_off_place_START (0)
#define PMIC_NP_BUCK5_OFF_PLACE_np_buck5_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck70_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_BUCK70_OFF_PLACE_UNION;
#endif
#define PMIC_NP_BUCK70_OFF_PLACE_np_buck70_off_place_START (0)
#define PMIC_NP_BUCK70_OFF_PLACE_np_buck70_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck71_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_BUCK71_OFF_PLACE_UNION;
#endif
#define PMIC_NP_BUCK71_OFF_PLACE_np_buck71_off_place_START (0)
#define PMIC_NP_BUCK71_OFF_PLACE_np_buck71_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_buck9_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_BUCK9_OFF_PLACE_UNION;
#endif
#define PMIC_NP_BUCK9_OFF_PLACE_np_buck9_off_place_START (0)
#define PMIC_NP_BUCK9_OFF_PLACE_np_buck9_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo0_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO0_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO0_OFF_PLACE_np_ldo0_off_place_START (0)
#define PMIC_NP_LDO0_OFF_PLACE_np_ldo0_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo4_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO4_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO4_OFF_PLACE_np_ldo4_off_place_START (0)
#define PMIC_NP_LDO4_OFF_PLACE_np_ldo4_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo6_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO6_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO6_OFF_PLACE_np_ldo6_off_place_START (0)
#define PMIC_NP_LDO6_OFF_PLACE_np_ldo6_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo8_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO8_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO8_OFF_PLACE_np_ldo8_off_place_START (0)
#define PMIC_NP_LDO8_OFF_PLACE_np_ldo8_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo15_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO15_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO15_OFF_PLACE_np_ldo15_off_place_START (0)
#define PMIC_NP_LDO15_OFF_PLACE_np_ldo15_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo17_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO17_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO17_OFF_PLACE_np_ldo17_off_place_START (0)
#define PMIC_NP_LDO17_OFF_PLACE_np_ldo17_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo18_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO18_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO18_OFF_PLACE_np_ldo18_off_place_START (0)
#define PMIC_NP_LDO18_OFF_PLACE_np_ldo18_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo23_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO23_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO23_OFF_PLACE_np_ldo23_off_place_START (0)
#define PMIC_NP_LDO23_OFF_PLACE_np_ldo23_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo24_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO24_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO24_OFF_PLACE_np_ldo24_off_place_START (0)
#define PMIC_NP_LDO24_OFF_PLACE_np_ldo24_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo28_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO28_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO28_OFF_PLACE_np_ldo28_off_place_START (0)
#define PMIC_NP_LDO28_OFF_PLACE_np_ldo28_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo30_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO30_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO30_OFF_PLACE_np_ldo30_off_place_START (0)
#define PMIC_NP_LDO30_OFF_PLACE_np_ldo30_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo32_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO32_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO32_OFF_PLACE_np_ldo32_off_place_START (0)
#define PMIC_NP_LDO32_OFF_PLACE_np_ldo32_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo36_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO36_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO36_OFF_PLACE_np_ldo36_off_place_START (0)
#define PMIC_NP_LDO36_OFF_PLACE_np_ldo36_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo37_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO37_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO37_OFF_PLACE_np_ldo37_off_place_START (0)
#define PMIC_NP_LDO37_OFF_PLACE_np_ldo37_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo38_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO38_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO38_OFF_PLACE_np_ldo38_off_place_START (0)
#define PMIC_NP_LDO38_OFF_PLACE_np_ldo38_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo39_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO39_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO39_OFF_PLACE_np_ldo39_off_place_START (0)
#define PMIC_NP_LDO39_OFF_PLACE_np_ldo39_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 8;
    } reg;
} PMIC_NP_LDO40_OFF_PLACE_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo42_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO42_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO42_OFF_PLACE_np_ldo42_off_place_START (0)
#define PMIC_NP_LDO42_OFF_PLACE_np_ldo42_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo43_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO43_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO43_OFF_PLACE_np_ldo43_off_place_START (0)
#define PMIC_NP_LDO43_OFF_PLACE_np_ldo43_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo44_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO44_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO44_OFF_PLACE_np_ldo44_off_place_START (0)
#define PMIC_NP_LDO44_OFF_PLACE_np_ldo44_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo45_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO45_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO45_OFF_PLACE_np_ldo45_off_place_START (0)
#define PMIC_NP_LDO45_OFF_PLACE_np_ldo45_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo46_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO46_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO46_OFF_PLACE_np_ldo46_off_place_START (0)
#define PMIC_NP_LDO46_OFF_PLACE_np_ldo46_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo_buf_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO_BUF_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO_BUF_OFF_PLACE_np_ldo_buf_off_place_START (0)
#define PMIC_NP_LDO_BUF_OFF_PLACE_np_ldo_buf_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_ldo_sink_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_LDO_SINK_OFF_PLACE_UNION;
#endif
#define PMIC_NP_LDO_SINK_OFF_PLACE_np_ldo_sink_off_place_START (0)
#define PMIC_NP_LDO_SINK_OFF_PLACE_np_ldo_sink_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_sw1_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_SW1_OFF_PLACE_UNION;
#endif
#define PMIC_NP_SW1_OFF_PLACE_np_sw1_off_place_START (0)
#define PMIC_NP_SW1_OFF_PLACE_np_sw1_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_pmu_en_off_place : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_NP_PMU_EN_OFF_PLACE_UNION;
#endif
#define PMIC_NP_PMU_EN_OFF_PLACE_np_pmu_en_off_place_START (0)
#define PMIC_NP_PMU_EN_OFF_PLACE_np_pmu_en_off_place_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_dummy_spmi : 4;
        unsigned char st_dummy_spmi : 4;
    } reg;
} PMIC_DUMMY_SPMI_UNION;
#endif
#define PMIC_DUMMY_SPMI_sc_dummy_spmi_START (0)
#define PMIC_DUMMY_SPMI_sc_dummy_spmi_END (3)
#define PMIC_DUMMY_SPMI_st_dummy_spmi_START (4)
#define PMIC_DUMMY_SPMI_st_dummy_spmi_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_cmp_ibias : 4;
        unsigned char b1_adj_rlx : 4;
    } reg;
} PMIC_BUCK1_CTRL0_UNION;
#endif
#define PMIC_BUCK1_CTRL0_b1_cmp_ibias_START (0)
#define PMIC_BUCK1_CTRL0_b1_cmp_ibias_END (3)
#define PMIC_BUCK1_CTRL0_b1_adj_rlx_START (4)
#define PMIC_BUCK1_CTRL0_b1_adj_rlx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_ptn_dt_ctrl : 1;
        unsigned char b1_ntp_dt_ctrl : 1;
        unsigned char reserved : 2;
        unsigned char b1_dt_sel : 1;
        unsigned char b1_ocp_sel : 3;
    } reg;
} PMIC_BUCK1_CTRL1_UNION;
#endif
#define PMIC_BUCK1_CTRL1_b1_ptn_dt_ctrl_START (0)
#define PMIC_BUCK1_CTRL1_b1_ptn_dt_ctrl_END (0)
#define PMIC_BUCK1_CTRL1_b1_ntp_dt_ctrl_START (1)
#define PMIC_BUCK1_CTRL1_b1_ntp_dt_ctrl_END (1)
#define PMIC_BUCK1_CTRL1_b1_dt_sel_START (4)
#define PMIC_BUCK1_CTRL1_b1_dt_sel_END (4)
#define PMIC_BUCK1_CTRL1_b1_ocp_sel_START (5)
#define PMIC_BUCK1_CTRL1_b1_ocp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_pg_n_sel : 3;
        unsigned char b1_pg_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK1_CTRL2_UNION;
#endif
#define PMIC_BUCK1_CTRL2_b1_pg_n_sel_START (0)
#define PMIC_BUCK1_CTRL2_b1_pg_n_sel_END (2)
#define PMIC_BUCK1_CTRL2_b1_pg_p_sel_START (3)
#define PMIC_BUCK1_CTRL2_b1_pg_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_ng_n_sel : 3;
        unsigned char b1_ng_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK1_CTRL3_UNION;
#endif
#define PMIC_BUCK1_CTRL3_b1_ng_n_sel_START (0)
#define PMIC_BUCK1_CTRL3_b1_ng_n_sel_END (2)
#define PMIC_BUCK1_CTRL3_b1_ng_p_sel_START (3)
#define PMIC_BUCK1_CTRL3_b1_ng_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_reg_r : 2;
        unsigned char reserved_0 : 2;
        unsigned char b1_reg_en : 1;
        unsigned char b1_adj_clx : 2;
        unsigned char reserved_1 : 1;
    } reg;
} PMIC_BUCK1_CTRL4_UNION;
#endif
#define PMIC_BUCK1_CTRL4_b1_reg_r_START (0)
#define PMIC_BUCK1_CTRL4_b1_reg_r_END (1)
#define PMIC_BUCK1_CTRL4_b1_reg_en_START (4)
#define PMIC_BUCK1_CTRL4_b1_reg_en_END (4)
#define PMIC_BUCK1_CTRL4_b1_adj_clx_START (5)
#define PMIC_BUCK1_CTRL4_b1_adj_clx_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_min_ton : 3;
        unsigned char b1_reg_ibias : 1;
        unsigned char b1_reg_op_c : 1;
        unsigned char b1_reg_dr : 3;
    } reg;
} PMIC_BUCK1_CTRL5_UNION;
#endif
#define PMIC_BUCK1_CTRL5_b1_min_ton_START (0)
#define PMIC_BUCK1_CTRL5_b1_min_ton_END (2)
#define PMIC_BUCK1_CTRL5_b1_reg_ibias_START (3)
#define PMIC_BUCK1_CTRL5_b1_reg_ibias_END (3)
#define PMIC_BUCK1_CTRL5_b1_reg_op_c_START (4)
#define PMIC_BUCK1_CTRL5_b1_reg_op_c_END (4)
#define PMIC_BUCK1_CTRL5_b1_reg_dr_START (5)
#define PMIC_BUCK1_CTRL5_b1_reg_dr_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_dmd_ton : 3;
        unsigned char b1_dmd_shield_ton : 1;
        unsigned char b1_dmd_sel : 4;
    } reg;
} PMIC_BUCK1_CTRL6_UNION;
#endif
#define PMIC_BUCK1_CTRL6_b1_dmd_ton_START (0)
#define PMIC_BUCK1_CTRL6_b1_dmd_ton_END (2)
#define PMIC_BUCK1_CTRL6_b1_dmd_shield_ton_START (3)
#define PMIC_BUCK1_CTRL6_b1_dmd_shield_ton_END (3)
#define PMIC_BUCK1_CTRL6_b1_dmd_sel_START (4)
#define PMIC_BUCK1_CTRL6_b1_dmd_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_sense_ctrl : 1;
        unsigned char b1_sense_filter : 4;
        unsigned char reserved : 2;
        unsigned char b1_sense_ibias_dmd : 1;
    } reg;
} PMIC_BUCK1_CTRL7_UNION;
#endif
#define PMIC_BUCK1_CTRL7_b1_sense_ctrl_START (0)
#define PMIC_BUCK1_CTRL7_b1_sense_ctrl_END (0)
#define PMIC_BUCK1_CTRL7_b1_sense_filter_START (1)
#define PMIC_BUCK1_CTRL7_b1_sense_filter_END (4)
#define PMIC_BUCK1_CTRL7_b1_sense_ibias_dmd_START (7)
#define PMIC_BUCK1_CTRL7_b1_sense_ibias_dmd_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_ocp_toff : 2;
        unsigned char b1_ocp_delay : 1;
        unsigned char b1_sense_sel : 1;
        unsigned char b1_sense_rev0 : 4;
    } reg;
} PMIC_BUCK1_CTRL8_UNION;
#endif
#define PMIC_BUCK1_CTRL8_b1_ocp_toff_START (0)
#define PMIC_BUCK1_CTRL8_b1_ocp_toff_END (1)
#define PMIC_BUCK1_CTRL8_b1_ocp_delay_START (2)
#define PMIC_BUCK1_CTRL8_b1_ocp_delay_END (2)
#define PMIC_BUCK1_CTRL8_b1_sense_sel_START (3)
#define PMIC_BUCK1_CTRL8_b1_sense_sel_END (3)
#define PMIC_BUCK1_CTRL8_b1_sense_rev0_START (4)
#define PMIC_BUCK1_CTRL8_b1_sense_rev0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_fb_cap_sel : 1;
        unsigned char b1_nonlinear_driver : 1;
        unsigned char b1_lx_dt : 1;
        unsigned char b1_ptn_dt_sel : 2;
        unsigned char b1_ntp_dt_sel : 2;
        unsigned char b1_cot_rlx : 1;
    } reg;
} PMIC_BUCK1_CTRL9_UNION;
#endif
#define PMIC_BUCK1_CTRL9_b1_fb_cap_sel_START (0)
#define PMIC_BUCK1_CTRL9_b1_fb_cap_sel_END (0)
#define PMIC_BUCK1_CTRL9_b1_nonlinear_driver_START (1)
#define PMIC_BUCK1_CTRL9_b1_nonlinear_driver_END (1)
#define PMIC_BUCK1_CTRL9_b1_lx_dt_START (2)
#define PMIC_BUCK1_CTRL9_b1_lx_dt_END (2)
#define PMIC_BUCK1_CTRL9_b1_ptn_dt_sel_START (3)
#define PMIC_BUCK1_CTRL9_b1_ptn_dt_sel_END (4)
#define PMIC_BUCK1_CTRL9_b1_ntp_dt_sel_START (5)
#define PMIC_BUCK1_CTRL9_b1_ntp_dt_sel_END (6)
#define PMIC_BUCK1_CTRL9_b1_cot_rlx_START (7)
#define PMIC_BUCK1_CTRL9_b1_cot_rlx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_dmd_sel_eco : 4;
        unsigned char b1_ocp_cmp_ibias : 1;
        unsigned char b1_ocpibias_sel : 2;
        unsigned char b1_ramp_ton_ctrl : 1;
    } reg;
} PMIC_BUCK1_CTRL10_UNION;
#endif
#define PMIC_BUCK1_CTRL10_b1_dmd_sel_eco_START (0)
#define PMIC_BUCK1_CTRL10_b1_dmd_sel_eco_END (3)
#define PMIC_BUCK1_CTRL10_b1_ocp_cmp_ibias_START (4)
#define PMIC_BUCK1_CTRL10_b1_ocp_cmp_ibias_END (4)
#define PMIC_BUCK1_CTRL10_b1_ocpibias_sel_START (5)
#define PMIC_BUCK1_CTRL10_b1_ocpibias_sel_END (6)
#define PMIC_BUCK1_CTRL10_b1_ramp_ton_ctrl_START (7)
#define PMIC_BUCK1_CTRL10_b1_ramp_ton_ctrl_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_offres_sel : 1;
        unsigned char b1_offres_shut : 1;
        unsigned char b1_ramp_prebias : 1;
        unsigned char b1_softime_sel : 2;
        unsigned char b1_fix_ton : 1;
        unsigned char b1_dmd_blanktime_sel : 1;
        unsigned char b1_dmd_type_sel : 1;
    } reg;
} PMIC_BUCK1_CTRL11_UNION;
#endif
#define PMIC_BUCK1_CTRL11_b1_offres_sel_START (0)
#define PMIC_BUCK1_CTRL11_b1_offres_sel_END (0)
#define PMIC_BUCK1_CTRL11_b1_offres_shut_START (1)
#define PMIC_BUCK1_CTRL11_b1_offres_shut_END (1)
#define PMIC_BUCK1_CTRL11_b1_ramp_prebias_START (2)
#define PMIC_BUCK1_CTRL11_b1_ramp_prebias_END (2)
#define PMIC_BUCK1_CTRL11_b1_softime_sel_START (3)
#define PMIC_BUCK1_CTRL11_b1_softime_sel_END (4)
#define PMIC_BUCK1_CTRL11_b1_fix_ton_START (5)
#define PMIC_BUCK1_CTRL11_b1_fix_ton_END (5)
#define PMIC_BUCK1_CTRL11_b1_dmd_blanktime_sel_START (6)
#define PMIC_BUCK1_CTRL11_b1_dmd_blanktime_sel_END (6)
#define PMIC_BUCK1_CTRL11_b1_dmd_type_sel_START (7)
#define PMIC_BUCK1_CTRL11_b1_dmd_type_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_ea_ibias_sel : 1;
        unsigned char b1_ibias_dmd_ctrl : 2;
        unsigned char b1_eco_ibias_sel : 1;
        unsigned char b1_ea_clamp_eco_dis : 1;
        unsigned char b1_ea_eco_dis : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK1_CTRL12_UNION;
#endif
#define PMIC_BUCK1_CTRL12_b1_ea_ibias_sel_START (0)
#define PMIC_BUCK1_CTRL12_b1_ea_ibias_sel_END (0)
#define PMIC_BUCK1_CTRL12_b1_ibias_dmd_ctrl_START (1)
#define PMIC_BUCK1_CTRL12_b1_ibias_dmd_ctrl_END (2)
#define PMIC_BUCK1_CTRL12_b1_eco_ibias_sel_START (3)
#define PMIC_BUCK1_CTRL12_b1_eco_ibias_sel_END (3)
#define PMIC_BUCK1_CTRL12_b1_ea_clamp_eco_dis_START (4)
#define PMIC_BUCK1_CTRL12_b1_ea_clamp_eco_dis_END (4)
#define PMIC_BUCK1_CTRL12_b1_ea_eco_dis_START (5)
#define PMIC_BUCK1_CTRL12_b1_ea_eco_dis_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_reserve0 : 8;
    } reg;
} PMIC_BUCK1_CTRL13_UNION;
#endif
#define PMIC_BUCK1_CTRL13_b1_reserve0_START (0)
#define PMIC_BUCK1_CTRL13_b1_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b1_reserve1 : 8;
    } reg;
} PMIC_BUCK1_CTRL14_UNION;
#endif
#define PMIC_BUCK1_CTRL14_b1_reserve1_START (0)
#define PMIC_BUCK1_CTRL14_b1_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b2_cmp_ibias : 4;
        unsigned char b2_adj_rlx : 4;
    } reg;
} PMIC_BUCK2_CTRL0_UNION;
#endif
#define PMIC_BUCK2_CTRL0_b2_cmp_ibias_START (0)
#define PMIC_BUCK2_CTRL0_b2_cmp_ibias_END (3)
#define PMIC_BUCK2_CTRL0_b2_adj_rlx_START (4)
#define PMIC_BUCK2_CTRL0_b2_adj_rlx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b2_ptn_dt_ctrl : 1;
        unsigned char b2_ntp_dt_ctrl : 1;
        unsigned char reserved : 2;
        unsigned char b2_dt_sel : 1;
        unsigned char b2_ocp_sel : 3;
    } reg;
} PMIC_BUCK2_CTRL1_UNION;
#endif
#define PMIC_BUCK2_CTRL1_b2_ptn_dt_ctrl_START (0)
#define PMIC_BUCK2_CTRL1_b2_ptn_dt_ctrl_END (0)
#define PMIC_BUCK2_CTRL1_b2_ntp_dt_ctrl_START (1)
#define PMIC_BUCK2_CTRL1_b2_ntp_dt_ctrl_END (1)
#define PMIC_BUCK2_CTRL1_b2_dt_sel_START (4)
#define PMIC_BUCK2_CTRL1_b2_dt_sel_END (4)
#define PMIC_BUCK2_CTRL1_b2_ocp_sel_START (5)
#define PMIC_BUCK2_CTRL1_b2_ocp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b2_pg_n_sel : 3;
        unsigned char b2_pg_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK2_CTRL2_UNION;
#endif
#define PMIC_BUCK2_CTRL2_b2_pg_n_sel_START (0)
#define PMIC_BUCK2_CTRL2_b2_pg_n_sel_END (2)
#define PMIC_BUCK2_CTRL2_b2_pg_p_sel_START (3)
#define PMIC_BUCK2_CTRL2_b2_pg_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b2_ng_n_sel : 3;
        unsigned char b2_ng_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK2_CTRL3_UNION;
#endif
#define PMIC_BUCK2_CTRL3_b2_ng_n_sel_START (0)
#define PMIC_BUCK2_CTRL3_b2_ng_n_sel_END (2)
#define PMIC_BUCK2_CTRL3_b2_ng_p_sel_START (3)
#define PMIC_BUCK2_CTRL3_b2_ng_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b2_reg_r : 2;
        unsigned char reserved_0 : 2;
        unsigned char b2_reg_en : 1;
        unsigned char b2_adj_clx : 2;
        unsigned char reserved_1 : 1;
    } reg;
} PMIC_BUCK2_CTRL4_UNION;
#endif
#define PMIC_BUCK2_CTRL4_b2_reg_r_START (0)
#define PMIC_BUCK2_CTRL4_b2_reg_r_END (1)
#define PMIC_BUCK2_CTRL4_b2_reg_en_START (4)
#define PMIC_BUCK2_CTRL4_b2_reg_en_END (4)
#define PMIC_BUCK2_CTRL4_b2_adj_clx_START (5)
#define PMIC_BUCK2_CTRL4_b2_adj_clx_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b2_min_ton : 3;
        unsigned char b2_reg_ibias : 1;
        unsigned char b2_reg_op_c : 1;
        unsigned char b2_reg_dr : 3;
    } reg;
} PMIC_BUCK2_CTRL5_UNION;
#endif
#define PMIC_BUCK2_CTRL5_b2_min_ton_START (0)
#define PMIC_BUCK2_CTRL5_b2_min_ton_END (2)
#define PMIC_BUCK2_CTRL5_b2_reg_ibias_START (3)
#define PMIC_BUCK2_CTRL5_b2_reg_ibias_END (3)
#define PMIC_BUCK2_CTRL5_b2_reg_op_c_START (4)
#define PMIC_BUCK2_CTRL5_b2_reg_op_c_END (4)
#define PMIC_BUCK2_CTRL5_b2_reg_dr_START (5)
#define PMIC_BUCK2_CTRL5_b2_reg_dr_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b2_dmd_ton : 3;
        unsigned char b2_dmd_shield_ton : 1;
        unsigned char b2_dmd_sel : 4;
    } reg;
} PMIC_BUCK2_CTRL6_UNION;
#endif
#define PMIC_BUCK2_CTRL6_b2_dmd_ton_START (0)
#define PMIC_BUCK2_CTRL6_b2_dmd_ton_END (2)
#define PMIC_BUCK2_CTRL6_b2_dmd_shield_ton_START (3)
#define PMIC_BUCK2_CTRL6_b2_dmd_shield_ton_END (3)
#define PMIC_BUCK2_CTRL6_b2_dmd_sel_START (4)
#define PMIC_BUCK2_CTRL6_b2_dmd_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b2_ocp_toff : 2;
        unsigned char b2_ocp_delay : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_BUCK2_CTRL7_UNION;
#endif
#define PMIC_BUCK2_CTRL7_b2_ocp_toff_START (0)
#define PMIC_BUCK2_CTRL7_b2_ocp_toff_END (1)
#define PMIC_BUCK2_CTRL7_b2_ocp_delay_START (2)
#define PMIC_BUCK2_CTRL7_b2_ocp_delay_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b2_fb_cap_sel : 1;
        unsigned char b2_nonlinear_driver : 1;
        unsigned char b2_lx_dt : 1;
        unsigned char b2_ptn_dt_sel : 2;
        unsigned char b2_ntp_dt_sel : 2;
        unsigned char b2_cot_rlx : 1;
    } reg;
} PMIC_BUCK2_CTRL8_UNION;
#endif
#define PMIC_BUCK2_CTRL8_b2_fb_cap_sel_START (0)
#define PMIC_BUCK2_CTRL8_b2_fb_cap_sel_END (0)
#define PMIC_BUCK2_CTRL8_b2_nonlinear_driver_START (1)
#define PMIC_BUCK2_CTRL8_b2_nonlinear_driver_END (1)
#define PMIC_BUCK2_CTRL8_b2_lx_dt_START (2)
#define PMIC_BUCK2_CTRL8_b2_lx_dt_END (2)
#define PMIC_BUCK2_CTRL8_b2_ptn_dt_sel_START (3)
#define PMIC_BUCK2_CTRL8_b2_ptn_dt_sel_END (4)
#define PMIC_BUCK2_CTRL8_b2_ntp_dt_sel_START (5)
#define PMIC_BUCK2_CTRL8_b2_ntp_dt_sel_END (6)
#define PMIC_BUCK2_CTRL8_b2_cot_rlx_START (7)
#define PMIC_BUCK2_CTRL8_b2_cot_rlx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b2_dmd_sel_eco : 4;
        unsigned char b2_ocp_cmp_ibias : 1;
        unsigned char b2_ocpibias_sel : 2;
        unsigned char b2_ramp_ton_ctrl : 1;
    } reg;
} PMIC_BUCK2_CTRL9_UNION;
#endif
#define PMIC_BUCK2_CTRL9_b2_dmd_sel_eco_START (0)
#define PMIC_BUCK2_CTRL9_b2_dmd_sel_eco_END (3)
#define PMIC_BUCK2_CTRL9_b2_ocp_cmp_ibias_START (4)
#define PMIC_BUCK2_CTRL9_b2_ocp_cmp_ibias_END (4)
#define PMIC_BUCK2_CTRL9_b2_ocpibias_sel_START (5)
#define PMIC_BUCK2_CTRL9_b2_ocpibias_sel_END (6)
#define PMIC_BUCK2_CTRL9_b2_ramp_ton_ctrl_START (7)
#define PMIC_BUCK2_CTRL9_b2_ramp_ton_ctrl_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b2_offres_sel : 1;
        unsigned char b2_offres_shut : 1;
        unsigned char b2_ramp_prebias : 1;
        unsigned char b2_softime_sel : 2;
        unsigned char b2_fix_ton : 1;
        unsigned char b2_dmd_blanktime_sel : 1;
        unsigned char b2_dmd_type_sel : 1;
    } reg;
} PMIC_BUCK2_CTRL10_UNION;
#endif
#define PMIC_BUCK2_CTRL10_b2_offres_sel_START (0)
#define PMIC_BUCK2_CTRL10_b2_offres_sel_END (0)
#define PMIC_BUCK2_CTRL10_b2_offres_shut_START (1)
#define PMIC_BUCK2_CTRL10_b2_offres_shut_END (1)
#define PMIC_BUCK2_CTRL10_b2_ramp_prebias_START (2)
#define PMIC_BUCK2_CTRL10_b2_ramp_prebias_END (2)
#define PMIC_BUCK2_CTRL10_b2_softime_sel_START (3)
#define PMIC_BUCK2_CTRL10_b2_softime_sel_END (4)
#define PMIC_BUCK2_CTRL10_b2_fix_ton_START (5)
#define PMIC_BUCK2_CTRL10_b2_fix_ton_END (5)
#define PMIC_BUCK2_CTRL10_b2_dmd_blanktime_sel_START (6)
#define PMIC_BUCK2_CTRL10_b2_dmd_blanktime_sel_END (6)
#define PMIC_BUCK2_CTRL10_b2_dmd_type_sel_START (7)
#define PMIC_BUCK2_CTRL10_b2_dmd_type_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b2_ea_ibias_sel : 1;
        unsigned char b2_ibias_dmd_ctrl : 2;
        unsigned char b2_eco_ibias_sel : 1;
        unsigned char b2_ea_clamp_eco_dis : 1;
        unsigned char b2_ea_eco_dis : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK2_CTRL11_UNION;
#endif
#define PMIC_BUCK2_CTRL11_b2_ea_ibias_sel_START (0)
#define PMIC_BUCK2_CTRL11_b2_ea_ibias_sel_END (0)
#define PMIC_BUCK2_CTRL11_b2_ibias_dmd_ctrl_START (1)
#define PMIC_BUCK2_CTRL11_b2_ibias_dmd_ctrl_END (2)
#define PMIC_BUCK2_CTRL11_b2_eco_ibias_sel_START (3)
#define PMIC_BUCK2_CTRL11_b2_eco_ibias_sel_END (3)
#define PMIC_BUCK2_CTRL11_b2_ea_clamp_eco_dis_START (4)
#define PMIC_BUCK2_CTRL11_b2_ea_clamp_eco_dis_END (4)
#define PMIC_BUCK2_CTRL11_b2_ea_eco_dis_START (5)
#define PMIC_BUCK2_CTRL11_b2_ea_eco_dis_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b2_reserve0 : 8;
    } reg;
} PMIC_BUCK2_CTRL12_UNION;
#endif
#define PMIC_BUCK2_CTRL12_b2_reserve0_START (0)
#define PMIC_BUCK2_CTRL12_b2_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b2_reserve1 : 8;
    } reg;
} PMIC_BUCK2_CTRL13_UNION;
#endif
#define PMIC_BUCK2_CTRL13_b2_reserve1_START (0)
#define PMIC_BUCK2_CTRL13_b2_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b3_cmp_ibias : 4;
        unsigned char b3_adj_rlx : 4;
    } reg;
} PMIC_BUCK3_CTRL0_UNION;
#endif
#define PMIC_BUCK3_CTRL0_b3_cmp_ibias_START (0)
#define PMIC_BUCK3_CTRL0_b3_cmp_ibias_END (3)
#define PMIC_BUCK3_CTRL0_b3_adj_rlx_START (4)
#define PMIC_BUCK3_CTRL0_b3_adj_rlx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b3_ptn_dt_ctrl : 1;
        unsigned char b3_ntp_dt_ctrl : 1;
        unsigned char reserved : 2;
        unsigned char b3_dt_sel : 1;
        unsigned char b3_ocp_sel : 3;
    } reg;
} PMIC_BUCK3_CTRL1_UNION;
#endif
#define PMIC_BUCK3_CTRL1_b3_ptn_dt_ctrl_START (0)
#define PMIC_BUCK3_CTRL1_b3_ptn_dt_ctrl_END (0)
#define PMIC_BUCK3_CTRL1_b3_ntp_dt_ctrl_START (1)
#define PMIC_BUCK3_CTRL1_b3_ntp_dt_ctrl_END (1)
#define PMIC_BUCK3_CTRL1_b3_dt_sel_START (4)
#define PMIC_BUCK3_CTRL1_b3_dt_sel_END (4)
#define PMIC_BUCK3_CTRL1_b3_ocp_sel_START (5)
#define PMIC_BUCK3_CTRL1_b3_ocp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b3_pg_n_sel : 3;
        unsigned char b3_pg_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK3_CTRL2_UNION;
#endif
#define PMIC_BUCK3_CTRL2_b3_pg_n_sel_START (0)
#define PMIC_BUCK3_CTRL2_b3_pg_n_sel_END (2)
#define PMIC_BUCK3_CTRL2_b3_pg_p_sel_START (3)
#define PMIC_BUCK3_CTRL2_b3_pg_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b3_ng_n_sel : 3;
        unsigned char b3_ng_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK3_CTRL3_UNION;
#endif
#define PMIC_BUCK3_CTRL3_b3_ng_n_sel_START (0)
#define PMIC_BUCK3_CTRL3_b3_ng_n_sel_END (2)
#define PMIC_BUCK3_CTRL3_b3_ng_p_sel_START (3)
#define PMIC_BUCK3_CTRL3_b3_ng_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b3_reg_r : 2;
        unsigned char reserved_0 : 2;
        unsigned char b3_reg_en : 1;
        unsigned char b3_adj_clx : 2;
        unsigned char reserved_1 : 1;
    } reg;
} PMIC_BUCK3_CTRL4_UNION;
#endif
#define PMIC_BUCK3_CTRL4_b3_reg_r_START (0)
#define PMIC_BUCK3_CTRL4_b3_reg_r_END (1)
#define PMIC_BUCK3_CTRL4_b3_reg_en_START (4)
#define PMIC_BUCK3_CTRL4_b3_reg_en_END (4)
#define PMIC_BUCK3_CTRL4_b3_adj_clx_START (5)
#define PMIC_BUCK3_CTRL4_b3_adj_clx_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b3_min_ton : 3;
        unsigned char b3_reg_ibias : 1;
        unsigned char b3_reg_op_c : 1;
        unsigned char b3_reg_dr : 3;
    } reg;
} PMIC_BUCK3_CTRL5_UNION;
#endif
#define PMIC_BUCK3_CTRL5_b3_min_ton_START (0)
#define PMIC_BUCK3_CTRL5_b3_min_ton_END (2)
#define PMIC_BUCK3_CTRL5_b3_reg_ibias_START (3)
#define PMIC_BUCK3_CTRL5_b3_reg_ibias_END (3)
#define PMIC_BUCK3_CTRL5_b3_reg_op_c_START (4)
#define PMIC_BUCK3_CTRL5_b3_reg_op_c_END (4)
#define PMIC_BUCK3_CTRL5_b3_reg_dr_START (5)
#define PMIC_BUCK3_CTRL5_b3_reg_dr_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b3_dmd_ton : 3;
        unsigned char b3_dmd_shield_ton : 1;
        unsigned char b3_dmd_sel : 4;
    } reg;
} PMIC_BUCK3_CTRL6_UNION;
#endif
#define PMIC_BUCK3_CTRL6_b3_dmd_ton_START (0)
#define PMIC_BUCK3_CTRL6_b3_dmd_ton_END (2)
#define PMIC_BUCK3_CTRL6_b3_dmd_shield_ton_START (3)
#define PMIC_BUCK3_CTRL6_b3_dmd_shield_ton_END (3)
#define PMIC_BUCK3_CTRL6_b3_dmd_sel_START (4)
#define PMIC_BUCK3_CTRL6_b3_dmd_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b3_ocp_toff : 2;
        unsigned char b3_ocp_delay : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_BUCK3_CTRL7_UNION;
#endif
#define PMIC_BUCK3_CTRL7_b3_ocp_toff_START (0)
#define PMIC_BUCK3_CTRL7_b3_ocp_toff_END (1)
#define PMIC_BUCK3_CTRL7_b3_ocp_delay_START (2)
#define PMIC_BUCK3_CTRL7_b3_ocp_delay_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b3_fb_cap_sel : 1;
        unsigned char b3_nonlinear_driver : 1;
        unsigned char b3_lx_dt : 1;
        unsigned char b3_ptn_dt_sel : 2;
        unsigned char b3_ntp_dt_sel : 2;
        unsigned char b3_cot_rlx : 1;
    } reg;
} PMIC_BUCK3_CTRL8_UNION;
#endif
#define PMIC_BUCK3_CTRL8_b3_fb_cap_sel_START (0)
#define PMIC_BUCK3_CTRL8_b3_fb_cap_sel_END (0)
#define PMIC_BUCK3_CTRL8_b3_nonlinear_driver_START (1)
#define PMIC_BUCK3_CTRL8_b3_nonlinear_driver_END (1)
#define PMIC_BUCK3_CTRL8_b3_lx_dt_START (2)
#define PMIC_BUCK3_CTRL8_b3_lx_dt_END (2)
#define PMIC_BUCK3_CTRL8_b3_ptn_dt_sel_START (3)
#define PMIC_BUCK3_CTRL8_b3_ptn_dt_sel_END (4)
#define PMIC_BUCK3_CTRL8_b3_ntp_dt_sel_START (5)
#define PMIC_BUCK3_CTRL8_b3_ntp_dt_sel_END (6)
#define PMIC_BUCK3_CTRL8_b3_cot_rlx_START (7)
#define PMIC_BUCK3_CTRL8_b3_cot_rlx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b3_dmd_sel_eco : 4;
        unsigned char b3_ocp_cmp_ibias : 1;
        unsigned char b3_ocpibias_sel : 2;
        unsigned char b3_ramp_ton_ctrl : 1;
    } reg;
} PMIC_BUCK3_CTRL9_UNION;
#endif
#define PMIC_BUCK3_CTRL9_b3_dmd_sel_eco_START (0)
#define PMIC_BUCK3_CTRL9_b3_dmd_sel_eco_END (3)
#define PMIC_BUCK3_CTRL9_b3_ocp_cmp_ibias_START (4)
#define PMIC_BUCK3_CTRL9_b3_ocp_cmp_ibias_END (4)
#define PMIC_BUCK3_CTRL9_b3_ocpibias_sel_START (5)
#define PMIC_BUCK3_CTRL9_b3_ocpibias_sel_END (6)
#define PMIC_BUCK3_CTRL9_b3_ramp_ton_ctrl_START (7)
#define PMIC_BUCK3_CTRL9_b3_ramp_ton_ctrl_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b3_offres_sel : 1;
        unsigned char b3_offres_shut : 1;
        unsigned char b3_ramp_prebias : 1;
        unsigned char b3_softime_sel : 2;
        unsigned char b3_fix_ton : 1;
        unsigned char b3_dmd_blanktime_sel : 1;
        unsigned char b3_dmd_type_sel : 1;
    } reg;
} PMIC_BUCK3_CTRL10_UNION;
#endif
#define PMIC_BUCK3_CTRL10_b3_offres_sel_START (0)
#define PMIC_BUCK3_CTRL10_b3_offres_sel_END (0)
#define PMIC_BUCK3_CTRL10_b3_offres_shut_START (1)
#define PMIC_BUCK3_CTRL10_b3_offres_shut_END (1)
#define PMIC_BUCK3_CTRL10_b3_ramp_prebias_START (2)
#define PMIC_BUCK3_CTRL10_b3_ramp_prebias_END (2)
#define PMIC_BUCK3_CTRL10_b3_softime_sel_START (3)
#define PMIC_BUCK3_CTRL10_b3_softime_sel_END (4)
#define PMIC_BUCK3_CTRL10_b3_fix_ton_START (5)
#define PMIC_BUCK3_CTRL10_b3_fix_ton_END (5)
#define PMIC_BUCK3_CTRL10_b3_dmd_blanktime_sel_START (6)
#define PMIC_BUCK3_CTRL10_b3_dmd_blanktime_sel_END (6)
#define PMIC_BUCK3_CTRL10_b3_dmd_type_sel_START (7)
#define PMIC_BUCK3_CTRL10_b3_dmd_type_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b3_ea_ibias_sel : 1;
        unsigned char b3_ibias_dmd_ctrl : 2;
        unsigned char b3_eco_ibias_sel : 1;
        unsigned char b3_ea_clamp_eco_dis : 1;
        unsigned char b3_ea_eco_dis : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK3_CTRL11_UNION;
#endif
#define PMIC_BUCK3_CTRL11_b3_ea_ibias_sel_START (0)
#define PMIC_BUCK3_CTRL11_b3_ea_ibias_sel_END (0)
#define PMIC_BUCK3_CTRL11_b3_ibias_dmd_ctrl_START (1)
#define PMIC_BUCK3_CTRL11_b3_ibias_dmd_ctrl_END (2)
#define PMIC_BUCK3_CTRL11_b3_eco_ibias_sel_START (3)
#define PMIC_BUCK3_CTRL11_b3_eco_ibias_sel_END (3)
#define PMIC_BUCK3_CTRL11_b3_ea_clamp_eco_dis_START (4)
#define PMIC_BUCK3_CTRL11_b3_ea_clamp_eco_dis_END (4)
#define PMIC_BUCK3_CTRL11_b3_ea_eco_dis_START (5)
#define PMIC_BUCK3_CTRL11_b3_ea_eco_dis_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b3_reserve0 : 8;
    } reg;
} PMIC_BUCK3_CTRL12_UNION;
#endif
#define PMIC_BUCK3_CTRL12_b3_reserve0_START (0)
#define PMIC_BUCK3_CTRL12_b3_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b3_reserve1 : 8;
    } reg;
} PMIC_BUCK3_CTRL13_UNION;
#endif
#define PMIC_BUCK3_CTRL13_b3_reserve1_START (0)
#define PMIC_BUCK3_CTRL13_b3_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_crtshare_clamp_sel : 4;
        unsigned char b401_crtshare_filter_r_sel : 1;
        unsigned char b401_crtshare_bw_sel : 2;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK401_CTRL0_UNION;
#endif
#define PMIC_BUCK401_CTRL0_b401_crtshare_clamp_sel_START (0)
#define PMIC_BUCK401_CTRL0_b401_crtshare_clamp_sel_END (3)
#define PMIC_BUCK401_CTRL0_b401_crtshare_filter_r_sel_START (4)
#define PMIC_BUCK401_CTRL0_b401_crtshare_filter_r_sel_END (4)
#define PMIC_BUCK401_CTRL0_b401_crtshare_bw_sel_START (5)
#define PMIC_BUCK401_CTRL0_b401_crtshare_bw_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_crtshare_atest_sel : 1;
        unsigned char b401_crtshare_trim : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_BUCK401_CTRL1_UNION;
#endif
#define PMIC_BUCK401_CTRL1_b401_crtshare_atest_sel_START (0)
#define PMIC_BUCK401_CTRL1_b401_crtshare_atest_sel_END (0)
#define PMIC_BUCK401_CTRL1_b401_crtshare_trim_START (1)
#define PMIC_BUCK401_CTRL1_b401_crtshare_trim_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_dmd_auto_eco : 1;
        unsigned char b401_dmd_auto_ccm : 1;
        unsigned char b401_dmd_auto_sel : 1;
        unsigned char b401_dmd_ngc_disch : 1;
        unsigned char b401_dmd_ngc_ramp : 1;
        unsigned char b401_dmd_clp_sel : 1;
        unsigned char b401_dmd_blk_sel : 1;
        unsigned char b401_dmd_eco_sel : 1;
    } reg;
} PMIC_BUCK401_CTRL2_UNION;
#endif
#define PMIC_BUCK401_CTRL2_b401_dmd_auto_eco_START (0)
#define PMIC_BUCK401_CTRL2_b401_dmd_auto_eco_END (0)
#define PMIC_BUCK401_CTRL2_b401_dmd_auto_ccm_START (1)
#define PMIC_BUCK401_CTRL2_b401_dmd_auto_ccm_END (1)
#define PMIC_BUCK401_CTRL2_b401_dmd_auto_sel_START (2)
#define PMIC_BUCK401_CTRL2_b401_dmd_auto_sel_END (2)
#define PMIC_BUCK401_CTRL2_b401_dmd_ngc_disch_START (3)
#define PMIC_BUCK401_CTRL2_b401_dmd_ngc_disch_END (3)
#define PMIC_BUCK401_CTRL2_b401_dmd_ngc_ramp_START (4)
#define PMIC_BUCK401_CTRL2_b401_dmd_ngc_ramp_END (4)
#define PMIC_BUCK401_CTRL2_b401_dmd_clp_sel_START (5)
#define PMIC_BUCK401_CTRL2_b401_dmd_clp_sel_END (5)
#define PMIC_BUCK401_CTRL2_b401_dmd_blk_sel_START (6)
#define PMIC_BUCK401_CTRL2_b401_dmd_blk_sel_END (6)
#define PMIC_BUCK401_CTRL2_b401_dmd_eco_sel_START (7)
#define PMIC_BUCK401_CTRL2_b401_dmd_eco_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_dmd_auto_vos : 3;
        unsigned char b401_dmd_auto_cmpib : 2;
        unsigned char b401_dmd_auto_sampt : 2;
        unsigned char b401_dmd_auto_vbc : 1;
    } reg;
} PMIC_BUCK401_CTRL3_UNION;
#endif
#define PMIC_BUCK401_CTRL3_b401_dmd_auto_vos_START (0)
#define PMIC_BUCK401_CTRL3_b401_dmd_auto_vos_END (2)
#define PMIC_BUCK401_CTRL3_b401_dmd_auto_cmpib_START (3)
#define PMIC_BUCK401_CTRL3_b401_dmd_auto_cmpib_END (4)
#define PMIC_BUCK401_CTRL3_b401_dmd_auto_sampt_START (5)
#define PMIC_BUCK401_CTRL3_b401_dmd_auto_sampt_END (6)
#define PMIC_BUCK401_CTRL3_b401_dmd_auto_vbc_START (7)
#define PMIC_BUCK401_CTRL3_b401_dmd_auto_vbc_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_nocp_delay_sel : 2;
        unsigned char b401_pocp_eco_sel : 1;
        unsigned char b401_pocp_delay_sel : 2;
        unsigned char b401_pocp_sel : 2;
        unsigned char b401_pocp_en : 1;
    } reg;
} PMIC_BUCK401_CTRL4_UNION;
#endif
#define PMIC_BUCK401_CTRL4_b401_nocp_delay_sel_START (0)
#define PMIC_BUCK401_CTRL4_b401_nocp_delay_sel_END (1)
#define PMIC_BUCK401_CTRL4_b401_pocp_eco_sel_START (2)
#define PMIC_BUCK401_CTRL4_b401_pocp_eco_sel_END (2)
#define PMIC_BUCK401_CTRL4_b401_pocp_delay_sel_START (3)
#define PMIC_BUCK401_CTRL4_b401_pocp_delay_sel_END (4)
#define PMIC_BUCK401_CTRL4_b401_pocp_sel_START (5)
#define PMIC_BUCK401_CTRL4_b401_pocp_sel_END (6)
#define PMIC_BUCK401_CTRL4_b401_pocp_en_START (7)
#define PMIC_BUCK401_CTRL4_b401_pocp_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_nocp_low_delay_sel : 2;
        unsigned char b401_nocp_low_sel : 2;
        unsigned char b401_nocp_low_eco_sel : 1;
        unsigned char b401_nocp_sel : 2;
        unsigned char b401_nocp_en : 1;
    } reg;
} PMIC_BUCK401_CTRL5_UNION;
#endif
#define PMIC_BUCK401_CTRL5_b401_nocp_low_delay_sel_START (0)
#define PMIC_BUCK401_CTRL5_b401_nocp_low_delay_sel_END (1)
#define PMIC_BUCK401_CTRL5_b401_nocp_low_sel_START (2)
#define PMIC_BUCK401_CTRL5_b401_nocp_low_sel_END (3)
#define PMIC_BUCK401_CTRL5_b401_nocp_low_eco_sel_START (4)
#define PMIC_BUCK401_CTRL5_b401_nocp_low_eco_sel_END (4)
#define PMIC_BUCK401_CTRL5_b401_nocp_sel_START (5)
#define PMIC_BUCK401_CTRL5_b401_nocp_sel_END (6)
#define PMIC_BUCK401_CTRL5_b401_nocp_en_START (7)
#define PMIC_BUCK401_CTRL5_b401_nocp_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_pocp_sw_delay_sel : 2;
        unsigned char b401_pocp_sw_sel : 3;
        unsigned char b401_pocp_sw_eco_cmp_sel : 1;
        unsigned char b401_pocp_sw_ccm_cmp_sel : 1;
        unsigned char b401_ocp_2us_delay_sel : 1;
    } reg;
} PMIC_BUCK401_CTRL6_UNION;
#endif
#define PMIC_BUCK401_CTRL6_b401_pocp_sw_delay_sel_START (0)
#define PMIC_BUCK401_CTRL6_b401_pocp_sw_delay_sel_END (1)
#define PMIC_BUCK401_CTRL6_b401_pocp_sw_sel_START (2)
#define PMIC_BUCK401_CTRL6_b401_pocp_sw_sel_END (4)
#define PMIC_BUCK401_CTRL6_b401_pocp_sw_eco_cmp_sel_START (5)
#define PMIC_BUCK401_CTRL6_b401_pocp_sw_eco_cmp_sel_END (5)
#define PMIC_BUCK401_CTRL6_b401_pocp_sw_ccm_cmp_sel_START (6)
#define PMIC_BUCK401_CTRL6_b401_pocp_sw_ccm_cmp_sel_END (6)
#define PMIC_BUCK401_CTRL6_b401_ocp_2us_delay_sel_START (7)
#define PMIC_BUCK401_CTRL6_b401_ocp_2us_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_buffer_bias : 2;
        unsigned char b401_ramp_sel : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_BUCK401_CTRL7_UNION;
#endif
#define PMIC_BUCK401_CTRL7_b401_buffer_bias_START (0)
#define PMIC_BUCK401_CTRL7_b401_buffer_bias_END (1)
#define PMIC_BUCK401_CTRL7_b401_ramp_sel_START (2)
#define PMIC_BUCK401_CTRL7_b401_ramp_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_clk9m6_pulse_sel : 2;
        unsigned char b401_vref_test_sel : 2;
        unsigned char b401_vref_vo_cap_sel : 2;
        unsigned char b401_rtr_cap_sel : 2;
    } reg;
} PMIC_BUCK401_CTRL8_UNION;
#endif
#define PMIC_BUCK401_CTRL8_b401_clk9m6_pulse_sel_START (0)
#define PMIC_BUCK401_CTRL8_b401_clk9m6_pulse_sel_END (1)
#define PMIC_BUCK401_CTRL8_b401_vref_test_sel_START (2)
#define PMIC_BUCK401_CTRL8_b401_vref_test_sel_END (3)
#define PMIC_BUCK401_CTRL8_b401_vref_vo_cap_sel_START (4)
#define PMIC_BUCK401_CTRL8_b401_vref_vo_cap_sel_END (5)
#define PMIC_BUCK401_CTRL8_b401_rtr_cap_sel_START (6)
#define PMIC_BUCK401_CTRL8_b401_rtr_cap_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_ton_det : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK401_CTRL9_UNION;
#endif
#define PMIC_BUCK401_CTRL9_b401_ton_det_START (0)
#define PMIC_BUCK401_CTRL9_b401_ton_det_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_load_state_sel : 3;
        unsigned char b401_state_sw_ocp_shield : 1;
        unsigned char b401_state_sw_ocp_sel : 2;
        unsigned char b401_ocp_shield : 1;
        unsigned char b401_nmos_off : 1;
    } reg;
} PMIC_BUCK401_CTRL10_UNION;
#endif
#define PMIC_BUCK401_CTRL10_b401_load_state_sel_START (0)
#define PMIC_BUCK401_CTRL10_b401_load_state_sel_END (2)
#define PMIC_BUCK401_CTRL10_b401_state_sw_ocp_shield_START (3)
#define PMIC_BUCK401_CTRL10_b401_state_sw_ocp_shield_END (3)
#define PMIC_BUCK401_CTRL10_b401_state_sw_ocp_sel_START (4)
#define PMIC_BUCK401_CTRL10_b401_state_sw_ocp_sel_END (5)
#define PMIC_BUCK401_CTRL10_b401_ocp_shield_START (6)
#define PMIC_BUCK401_CTRL10_b401_ocp_shield_END (6)
#define PMIC_BUCK401_CTRL10_b401_nmos_off_START (7)
#define PMIC_BUCK401_CTRL10_b401_nmos_off_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_fastnmos_off : 1;
        unsigned char b401_slave_sel : 1;
        unsigned char b401_oneph_sel : 1;
        unsigned char b401_negdmdton_add : 1;
        unsigned char b401_negdmd_ton_sel : 2;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK401_CTRL11_UNION;
#endif
#define PMIC_BUCK401_CTRL11_b401_fastnmos_off_START (0)
#define PMIC_BUCK401_CTRL11_b401_fastnmos_off_END (0)
#define PMIC_BUCK401_CTRL11_b401_slave_sel_START (1)
#define PMIC_BUCK401_CTRL11_b401_slave_sel_END (1)
#define PMIC_BUCK401_CTRL11_b401_oneph_sel_START (2)
#define PMIC_BUCK401_CTRL11_b401_oneph_sel_END (2)
#define PMIC_BUCK401_CTRL11_b401_negdmdton_add_START (3)
#define PMIC_BUCK401_CTRL11_b401_negdmdton_add_END (3)
#define PMIC_BUCK401_CTRL11_b401_negdmd_ton_sel_START (4)
#define PMIC_BUCK401_CTRL11_b401_negdmd_ton_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_cot_cmphys : 1;
        unsigned char b401_cot_ibst_sel : 1;
        unsigned char b401_cot_sche_neco : 1;
        unsigned char b401_cot_sche_eco : 1;
        unsigned char b401_cot_sel : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK401_CTRL12_UNION;
#endif
#define PMIC_BUCK401_CTRL12_b401_cot_cmphys_START (0)
#define PMIC_BUCK401_CTRL12_b401_cot_cmphys_END (0)
#define PMIC_BUCK401_CTRL12_b401_cot_ibst_sel_START (1)
#define PMIC_BUCK401_CTRL12_b401_cot_ibst_sel_END (1)
#define PMIC_BUCK401_CTRL12_b401_cot_sche_neco_START (2)
#define PMIC_BUCK401_CTRL12_b401_cot_sche_neco_END (2)
#define PMIC_BUCK401_CTRL12_b401_cot_sche_eco_START (3)
#define PMIC_BUCK401_CTRL12_b401_cot_sche_eco_END (3)
#define PMIC_BUCK401_CTRL12_b401_cot_sel_START (4)
#define PMIC_BUCK401_CTRL12_b401_cot_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_c1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK401_CTRL13_UNION;
#endif
#define PMIC_BUCK401_CTRL13_b401_c1_sel_START (0)
#define PMIC_BUCK401_CTRL13_b401_c1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_c2_sel : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK401_CTRL14_UNION;
#endif
#define PMIC_BUCK401_CTRL14_b401_c2_sel_START (0)
#define PMIC_BUCK401_CTRL14_b401_c2_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_c3_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK401_CTRL15_UNION;
#endif
#define PMIC_BUCK401_CTRL15_b401_c3_sel_START (0)
#define PMIC_BUCK401_CTRL15_b401_c3_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_r1_sel : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK401_CTRL16_UNION;
#endif
#define PMIC_BUCK401_CTRL16_b401_r1_sel_START (0)
#define PMIC_BUCK401_CTRL16_b401_r1_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_r2_sel : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK401_CTRL17_UNION;
#endif
#define PMIC_BUCK401_CTRL17_b401_r2_sel_START (0)
#define PMIC_BUCK401_CTRL17_b401_r2_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_r3_sel : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK401_CTRL18_UNION;
#endif
#define PMIC_BUCK401_CTRL18_b401_r3_sel_START (0)
#define PMIC_BUCK401_CTRL18_b401_r3_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_reg_idrp : 3;
        unsigned char b401_reg_idrn : 3;
        unsigned char b401_eco_shield : 1;
        unsigned char b401_clamp_ref_sel : 1;
    } reg;
} PMIC_BUCK401_CTRL19_UNION;
#endif
#define PMIC_BUCK401_CTRL19_b401_reg_idrp_START (0)
#define PMIC_BUCK401_CTRL19_b401_reg_idrp_END (2)
#define PMIC_BUCK401_CTRL19_b401_reg_idrn_START (3)
#define PMIC_BUCK401_CTRL19_b401_reg_idrn_END (5)
#define PMIC_BUCK401_CTRL19_b401_eco_shield_START (6)
#define PMIC_BUCK401_CTRL19_b401_eco_shield_END (6)
#define PMIC_BUCK401_CTRL19_b401_clamp_ref_sel_START (7)
#define PMIC_BUCK401_CTRL19_b401_clamp_ref_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_ramp_valley_sel : 1;
        unsigned char b401_eaamp_clamp : 2;
        unsigned char b401_eacomp_ecocontrol_shield : 1;
        unsigned char b401_bw_sel : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK401_CTRL20_UNION;
#endif
#define PMIC_BUCK401_CTRL20_b401_ramp_valley_sel_START (0)
#define PMIC_BUCK401_CTRL20_b401_ramp_valley_sel_END (0)
#define PMIC_BUCK401_CTRL20_b401_eaamp_clamp_START (1)
#define PMIC_BUCK401_CTRL20_b401_eaamp_clamp_END (2)
#define PMIC_BUCK401_CTRL20_b401_eacomp_ecocontrol_shield_START (3)
#define PMIC_BUCK401_CTRL20_b401_eacomp_ecocontrol_shield_END (3)
#define PMIC_BUCK401_CTRL20_b401_bw_sel_START (4)
#define PMIC_BUCK401_CTRL20_b401_bw_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_eaclamp_srgap_sel : 4;
        unsigned char b401_eaclamp_brgap_sel : 4;
    } reg;
} PMIC_BUCK401_CTRL21_UNION;
#endif
#define PMIC_BUCK401_CTRL21_b401_eaclamp_srgap_sel_START (0)
#define PMIC_BUCK401_CTRL21_b401_eaclamp_srgap_sel_END (3)
#define PMIC_BUCK401_CTRL21_b401_eaclamp_brgap_sel_START (4)
#define PMIC_BUCK401_CTRL21_b401_eaclamp_brgap_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_eaclamp_ecogap_sel : 2;
        unsigned char b401_trim2 : 1;
        unsigned char b401_trim1 : 1;
        unsigned char b401_eaclamp_gap_sel : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK401_CTRL22_UNION;
#endif
#define PMIC_BUCK401_CTRL22_b401_eaclamp_ecogap_sel_START (0)
#define PMIC_BUCK401_CTRL22_b401_eaclamp_ecogap_sel_END (1)
#define PMIC_BUCK401_CTRL22_b401_trim2_START (2)
#define PMIC_BUCK401_CTRL22_b401_trim2_END (2)
#define PMIC_BUCK401_CTRL22_b401_trim1_START (3)
#define PMIC_BUCK401_CTRL22_b401_trim1_END (3)
#define PMIC_BUCK401_CTRL22_b401_eaclamp_gap_sel_START (4)
#define PMIC_BUCK401_CTRL22_b401_eaclamp_gap_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_eabias_sel : 1;
        unsigned char b401_buck401_ea_r : 3;
        unsigned char b401_ea_ecocur_sel : 3;
        unsigned char b401_eaamp_clamp_sel : 1;
    } reg;
} PMIC_BUCK401_CTRL23_UNION;
#endif
#define PMIC_BUCK401_CTRL23_b401_eabias_sel_START (0)
#define PMIC_BUCK401_CTRL23_b401_eabias_sel_END (0)
#define PMIC_BUCK401_CTRL23_b401_buck401_ea_r_START (1)
#define PMIC_BUCK401_CTRL23_b401_buck401_ea_r_END (3)
#define PMIC_BUCK401_CTRL23_b401_ea_ecocur_sel_START (4)
#define PMIC_BUCK401_CTRL23_b401_ea_ecocur_sel_END (6)
#define PMIC_BUCK401_CTRL23_b401_eaamp_clamp_sel_START (7)
#define PMIC_BUCK401_CTRL23_b401_eaamp_clamp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_clampeco_bias_sel : 4;
        unsigned char b401_clamp_reg_idrp : 4;
    } reg;
} PMIC_BUCK401_CTRL24_UNION;
#endif
#define PMIC_BUCK401_CTRL24_b401_clampeco_bias_sel_START (0)
#define PMIC_BUCK401_CTRL24_b401_clampeco_bias_sel_END (3)
#define PMIC_BUCK401_CTRL24_b401_clamp_reg_idrp_START (4)
#define PMIC_BUCK401_CTRL24_b401_clamp_reg_idrp_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_ovp_shield : 1;
        unsigned char b401_ovp_sel : 2;
        unsigned char b401_short_shield : 1;
        unsigned char b401_sft_sel : 1;
        unsigned char b401_ocp_short_en : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK401_CTRL25_UNION;
#endif
#define PMIC_BUCK401_CTRL25_b401_ovp_shield_START (0)
#define PMIC_BUCK401_CTRL25_b401_ovp_shield_END (0)
#define PMIC_BUCK401_CTRL25_b401_ovp_sel_START (1)
#define PMIC_BUCK401_CTRL25_b401_ovp_sel_END (2)
#define PMIC_BUCK401_CTRL25_b401_short_shield_START (3)
#define PMIC_BUCK401_CTRL25_b401_short_shield_END (3)
#define PMIC_BUCK401_CTRL25_b401_sft_sel_START (4)
#define PMIC_BUCK401_CTRL25_b401_sft_sel_END (4)
#define PMIC_BUCK401_CTRL25_b401_ocp_short_en_START (5)
#define PMIC_BUCK401_CTRL25_b401_ocp_short_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_dbias_eco : 3;
        unsigned char b401_dbias : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK401_CTRL26_UNION;
#endif
#define PMIC_BUCK401_CTRL26_b401_dbias_eco_START (0)
#define PMIC_BUCK401_CTRL26_b401_dbias_eco_END (2)
#define PMIC_BUCK401_CTRL26_b401_dbias_START (3)
#define PMIC_BUCK401_CTRL26_b401_dbias_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_ramptrim_sel : 1;
        unsigned char b401_forcenormal : 1;
        unsigned char b401_normaltrim_shield : 1;
        unsigned char b401_ecotrim_shield : 1;
        unsigned char b401_forceeco : 1;
        unsigned char b401_eco_shield_cmp : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK401_CTRL27_UNION;
#endif
#define PMIC_BUCK401_CTRL27_b401_ramptrim_sel_START (0)
#define PMIC_BUCK401_CTRL27_b401_ramptrim_sel_END (0)
#define PMIC_BUCK401_CTRL27_b401_forcenormal_START (1)
#define PMIC_BUCK401_CTRL27_b401_forcenormal_END (1)
#define PMIC_BUCK401_CTRL27_b401_normaltrim_shield_START (2)
#define PMIC_BUCK401_CTRL27_b401_normaltrim_shield_END (2)
#define PMIC_BUCK401_CTRL27_b401_ecotrim_shield_START (3)
#define PMIC_BUCK401_CTRL27_b401_ecotrim_shield_END (3)
#define PMIC_BUCK401_CTRL27_b401_forceeco_START (4)
#define PMIC_BUCK401_CTRL27_b401_forceeco_END (4)
#define PMIC_BUCK401_CTRL27_b401_eco_shield_cmp_START (5)
#define PMIC_BUCK401_CTRL27_b401_eco_shield_cmp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_loadline_set : 5;
        unsigned char b401_loadline_fon : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK401_CTRL28_UNION;
#endif
#define PMIC_BUCK401_CTRL28_b401_loadline_set_START (0)
#define PMIC_BUCK401_CTRL28_b401_loadline_set_END (4)
#define PMIC_BUCK401_CTRL28_b401_loadline_fon_START (5)
#define PMIC_BUCK401_CTRL28_b401_loadline_fon_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_stsw_fstsch_neco : 3;
        unsigned char b401_stsw_fstsch_eco : 3;
        unsigned char b401_stsw_rpup_ccm : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK401_CTRL29_UNION;
#endif
#define PMIC_BUCK401_CTRL29_b401_stsw_fstsch_neco_START (0)
#define PMIC_BUCK401_CTRL29_b401_stsw_fstsch_neco_END (2)
#define PMIC_BUCK401_CTRL29_b401_stsw_fstsch_eco_START (3)
#define PMIC_BUCK401_CTRL29_b401_stsw_fstsch_eco_END (5)
#define PMIC_BUCK401_CTRL29_b401_stsw_rpup_ccm_START (6)
#define PMIC_BUCK401_CTRL29_b401_stsw_rpup_ccm_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_stsw_slwsch_neco : 2;
        unsigned char b401_stsw_slwsch_eco : 2;
        unsigned char b401_stsw_fstps_ith : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK401_CTRL30_UNION;
#endif
#define PMIC_BUCK401_CTRL30_b401_stsw_slwsch_neco_START (0)
#define PMIC_BUCK401_CTRL30_b401_stsw_slwsch_neco_END (1)
#define PMIC_BUCK401_CTRL30_b401_stsw_slwsch_eco_START (2)
#define PMIC_BUCK401_CTRL30_b401_stsw_slwsch_eco_END (3)
#define PMIC_BUCK401_CTRL30_b401_stsw_fstps_ith_START (4)
#define PMIC_BUCK401_CTRL30_b401_stsw_fstps_ith_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_stsw_slw_nohys : 1;
        unsigned char b401_stsw_slw_ct2cm_ith : 3;
        unsigned char b401_stsw_slw_ct2ec_hys : 1;
        unsigned char b401_stsw_slw_ct2ec_ith : 2;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK401_CTRL31_UNION;
#endif
#define PMIC_BUCK401_CTRL31_b401_stsw_slw_nohys_START (0)
#define PMIC_BUCK401_CTRL31_b401_stsw_slw_nohys_END (0)
#define PMIC_BUCK401_CTRL31_b401_stsw_slw_ct2cm_ith_START (1)
#define PMIC_BUCK401_CTRL31_b401_stsw_slw_ct2cm_ith_END (3)
#define PMIC_BUCK401_CTRL31_b401_stsw_slw_ct2ec_hys_START (4)
#define PMIC_BUCK401_CTRL31_b401_stsw_slw_ct2ec_hys_END (4)
#define PMIC_BUCK401_CTRL31_b401_stsw_slw_ct2ec_ith_START (5)
#define PMIC_BUCK401_CTRL31_b401_stsw_slw_ct2ec_ith_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_stsw_st_set : 2;
        unsigned char b401_stsw_slw_dn_td : 2;
        unsigned char b401_stsw_slw_up_td : 2;
        unsigned char b401_stsw_slw_ct2cm_hys : 2;
    } reg;
} PMIC_BUCK401_CTRL32_UNION;
#endif
#define PMIC_BUCK401_CTRL32_b401_stsw_st_set_START (0)
#define PMIC_BUCK401_CTRL32_b401_stsw_st_set_END (1)
#define PMIC_BUCK401_CTRL32_b401_stsw_slw_dn_td_START (2)
#define PMIC_BUCK401_CTRL32_b401_stsw_slw_dn_td_END (3)
#define PMIC_BUCK401_CTRL32_b401_stsw_slw_up_td_START (4)
#define PMIC_BUCK401_CTRL32_b401_stsw_slw_up_td_END (5)
#define PMIC_BUCK401_CTRL32_b401_stsw_slw_ct2cm_hys_START (6)
#define PMIC_BUCK401_CTRL32_b401_stsw_slw_ct2cm_hys_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_ecdt_ct2cm_ft : 2;
        unsigned char b401_ecdt_ec2ct_ref : 3;
        unsigned char b401_ecdt_ct2cm_ref : 3;
    } reg;
} PMIC_BUCK401_CTRL33_UNION;
#endif
#define PMIC_BUCK401_CTRL33_b401_ecdt_ct2cm_ft_START (0)
#define PMIC_BUCK401_CTRL33_b401_ecdt_ct2cm_ft_END (1)
#define PMIC_BUCK401_CTRL33_b401_ecdt_ec2ct_ref_START (2)
#define PMIC_BUCK401_CTRL33_b401_ecdt_ec2ct_ref_END (4)
#define PMIC_BUCK401_CTRL33_b401_ecdt_ct2cm_ref_START (5)
#define PMIC_BUCK401_CTRL33_b401_ecdt_ct2cm_ref_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_ecdt_fltrc : 2;
        unsigned char b401_ecdt_ct2ec_ft : 2;
        unsigned char b401_ecdt_cmphys : 1;
        unsigned char b401_ecdt_ct2ec_ref : 3;
    } reg;
} PMIC_BUCK401_CTRL34_UNION;
#endif
#define PMIC_BUCK401_CTRL34_b401_ecdt_fltrc_START (0)
#define PMIC_BUCK401_CTRL34_b401_ecdt_fltrc_END (1)
#define PMIC_BUCK401_CTRL34_b401_ecdt_ct2ec_ft_START (2)
#define PMIC_BUCK401_CTRL34_b401_ecdt_ct2ec_ft_END (3)
#define PMIC_BUCK401_CTRL34_b401_ecdt_cmphys_START (4)
#define PMIC_BUCK401_CTRL34_b401_ecdt_cmphys_END (4)
#define PMIC_BUCK401_CTRL34_b401_ecdt_ct2ec_ref_START (5)
#define PMIC_BUCK401_CTRL34_b401_ecdt_ct2ec_ref_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_min_toff_sel : 2;
        unsigned char b401_min_ton_sel : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK401_CTRL35_UNION;
#endif
#define PMIC_BUCK401_CTRL35_b401_min_toff_sel_START (0)
#define PMIC_BUCK401_CTRL35_b401_min_toff_sel_END (1)
#define PMIC_BUCK401_CTRL35_b401_min_ton_sel_START (2)
#define PMIC_BUCK401_CTRL35_b401_min_ton_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_ctlogic_crson_td : 2;
        unsigned char b401_ctlogic_modsw : 1;
        unsigned char b401_ctlogic_sgph : 1;
        unsigned char b401_ctlogic_smph : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK401_CTRL36_UNION;
#endif
#define PMIC_BUCK401_CTRL36_b401_ctlogic_crson_td_START (0)
#define PMIC_BUCK401_CTRL36_b401_ctlogic_crson_td_END (1)
#define PMIC_BUCK401_CTRL36_b401_ctlogic_modsw_START (2)
#define PMIC_BUCK401_CTRL36_b401_ctlogic_modsw_END (2)
#define PMIC_BUCK401_CTRL36_b401_ctlogic_sgph_START (3)
#define PMIC_BUCK401_CTRL36_b401_ctlogic_sgph_END (3)
#define PMIC_BUCK401_CTRL36_b401_ctlogic_smph_START (4)
#define PMIC_BUCK401_CTRL36_b401_ctlogic_smph_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_ramp_big_sel : 1;
        unsigned char b401_ramp_mid_sel : 1;
        unsigned char buck401_ramp_line_sel : 1;
        unsigned char buck401_ramp_ccm_outsel : 1;
        unsigned char buck401_ramp_cot_outsel : 1;
        unsigned char buck401_ramp_c : 2;
        unsigned char buck401_ramp_clk_sel : 1;
    } reg;
} PMIC_BUCK401_CTRL37_UNION;
#endif
#define PMIC_BUCK401_CTRL37_b401_ramp_big_sel_START (0)
#define PMIC_BUCK401_CTRL37_b401_ramp_big_sel_END (0)
#define PMIC_BUCK401_CTRL37_b401_ramp_mid_sel_START (1)
#define PMIC_BUCK401_CTRL37_b401_ramp_mid_sel_END (1)
#define PMIC_BUCK401_CTRL37_buck401_ramp_line_sel_START (2)
#define PMIC_BUCK401_CTRL37_buck401_ramp_line_sel_END (2)
#define PMIC_BUCK401_CTRL37_buck401_ramp_ccm_outsel_START (3)
#define PMIC_BUCK401_CTRL37_buck401_ramp_ccm_outsel_END (3)
#define PMIC_BUCK401_CTRL37_buck401_ramp_cot_outsel_START (4)
#define PMIC_BUCK401_CTRL37_buck401_ramp_cot_outsel_END (4)
#define PMIC_BUCK401_CTRL37_buck401_ramp_c_START (5)
#define PMIC_BUCK401_CTRL37_buck401_ramp_c_END (6)
#define PMIC_BUCK401_CTRL37_buck401_ramp_clk_sel_START (7)
#define PMIC_BUCK401_CTRL37_buck401_ramp_clk_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_ramp_r_eco : 4;
        unsigned char b401_ramp_counter : 2;
        unsigned char b401_ramp_sw_sel : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK401_CTRL38_UNION;
#endif
#define PMIC_BUCK401_CTRL38_b401_ramp_r_eco_START (0)
#define PMIC_BUCK401_CTRL38_b401_ramp_r_eco_END (3)
#define PMIC_BUCK401_CTRL38_b401_ramp_counter_START (4)
#define PMIC_BUCK401_CTRL38_b401_ramp_counter_END (5)
#define PMIC_BUCK401_CTRL38_b401_ramp_sw_sel_START (6)
#define PMIC_BUCK401_CTRL38_b401_ramp_sw_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_ramp_r_ccm : 4;
        unsigned char b401_ramp_r_cot : 4;
    } reg;
} PMIC_BUCK401_CTRL39_UNION;
#endif
#define PMIC_BUCK401_CTRL39_b401_ramp_r_ccm_START (0)
#define PMIC_BUCK401_CTRL39_b401_ramp_r_ccm_END (3)
#define PMIC_BUCK401_CTRL39_b401_ramp_r_cot_START (4)
#define PMIC_BUCK401_CTRL39_b401_ramp_r_cot_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_ramp_fast_sel : 1;
        unsigned char b401_ramp_freq_sel : 1;
        unsigned char b401_ramp_res_test : 1;
        unsigned char b401_ramp_change_mode : 5;
    } reg;
} PMIC_BUCK401_CTRL40_UNION;
#endif
#define PMIC_BUCK401_CTRL40_b401_ramp_fast_sel_START (0)
#define PMIC_BUCK401_CTRL40_b401_ramp_fast_sel_END (0)
#define PMIC_BUCK401_CTRL40_b401_ramp_freq_sel_START (1)
#define PMIC_BUCK401_CTRL40_b401_ramp_freq_sel_END (1)
#define PMIC_BUCK401_CTRL40_b401_ramp_res_test_START (2)
#define PMIC_BUCK401_CTRL40_b401_ramp_res_test_END (2)
#define PMIC_BUCK401_CTRL40_b401_ramp_change_mode_START (3)
#define PMIC_BUCK401_CTRL40_b401_ramp_change_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b401_ramp_dblclk_sel : 1;
        unsigned char b401_ramp_force_eco_test : 1;
        unsigned char b401_ramp_buff_trim_test : 1;
        unsigned char b401_ramp_buff_trim : 1;
        unsigned char b401_ramp_buff_bias : 2;
        unsigned char b401_ramp_eco_buffer : 1;
        unsigned char buck401_ramp_trim_sel : 1;
    } reg;
} PMIC_BUCK401_CTRL41_UNION;
#endif
#define PMIC_BUCK401_CTRL41_b401_ramp_dblclk_sel_START (0)
#define PMIC_BUCK401_CTRL41_b401_ramp_dblclk_sel_END (0)
#define PMIC_BUCK401_CTRL41_b401_ramp_force_eco_test_START (1)
#define PMIC_BUCK401_CTRL41_b401_ramp_force_eco_test_END (1)
#define PMIC_BUCK401_CTRL41_b401_ramp_buff_trim_test_START (2)
#define PMIC_BUCK401_CTRL41_b401_ramp_buff_trim_test_END (2)
#define PMIC_BUCK401_CTRL41_b401_ramp_buff_trim_START (3)
#define PMIC_BUCK401_CTRL41_b401_ramp_buff_trim_END (3)
#define PMIC_BUCK401_CTRL41_b401_ramp_buff_bias_START (4)
#define PMIC_BUCK401_CTRL41_b401_ramp_buff_bias_END (5)
#define PMIC_BUCK401_CTRL41_b401_ramp_eco_buffer_START (6)
#define PMIC_BUCK401_CTRL41_b401_ramp_eco_buffer_END (6)
#define PMIC_BUCK401_CTRL41_buck401_ramp_trim_sel_START (7)
#define PMIC_BUCK401_CTRL41_buck401_ramp_trim_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b4012clk_chopper_sel : 2;
        unsigned char b4012en_chopper : 1;
        unsigned char b4012en_nsense : 1;
        unsigned char b4012en_psense : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK4012_CTRL0_UNION;
#endif
#define PMIC_BUCK4012_CTRL0_b4012clk_chopper_sel_START (0)
#define PMIC_BUCK4012_CTRL0_b4012clk_chopper_sel_END (1)
#define PMIC_BUCK4012_CTRL0_b4012en_chopper_START (2)
#define PMIC_BUCK4012_CTRL0_b4012en_chopper_END (2)
#define PMIC_BUCK4012_CTRL0_b4012en_nsense_START (3)
#define PMIC_BUCK4012_CTRL0_b4012en_nsense_END (3)
#define PMIC_BUCK4012_CTRL0_b4012en_psense_START (4)
#define PMIC_BUCK4012_CTRL0_b4012en_psense_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b4012reg_sense_res_sel : 3;
        unsigned char b4012nsense_sel : 3;
        unsigned char b4012sense_ratio_sel : 2;
    } reg;
} PMIC_BUCK4012_CTRL1_UNION;
#endif
#define PMIC_BUCK4012_CTRL1_b4012reg_sense_res_sel_START (0)
#define PMIC_BUCK4012_CTRL1_b4012reg_sense_res_sel_END (2)
#define PMIC_BUCK4012_CTRL1_b4012nsense_sel_START (3)
#define PMIC_BUCK4012_CTRL1_b4012nsense_sel_END (5)
#define PMIC_BUCK4012_CTRL1_b4012sense_ratio_sel_START (6)
#define PMIC_BUCK4012_CTRL1_b4012sense_ratio_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b4012en_vo : 1;
        unsigned char b4012ibalance_gm_sel : 2;
        unsigned char b4012en_test : 1;
        unsigned char b4012cs_test_sel : 1;
        unsigned char b4012avg_curr_filter_sel : 2;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK4012_CTRL2_UNION;
#endif
#define PMIC_BUCK4012_CTRL2_b4012en_vo_START (0)
#define PMIC_BUCK4012_CTRL2_b4012en_vo_END (0)
#define PMIC_BUCK4012_CTRL2_b4012ibalance_gm_sel_START (1)
#define PMIC_BUCK4012_CTRL2_b4012ibalance_gm_sel_END (2)
#define PMIC_BUCK4012_CTRL2_b4012en_test_START (3)
#define PMIC_BUCK4012_CTRL2_b4012en_test_END (3)
#define PMIC_BUCK4012_CTRL2_b4012cs_test_sel_START (4)
#define PMIC_BUCK4012_CTRL2_b4012cs_test_sel_END (4)
#define PMIC_BUCK4012_CTRL2_b4012avg_curr_filter_sel_START (5)
#define PMIC_BUCK4012_CTRL2_b4012avg_curr_filter_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b4012vo_ton_res_sel : 3;
        unsigned char b4012cot_res_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK4012_CTRL3_UNION;
#endif
#define PMIC_BUCK4012_CTRL3_b4012vo_ton_res_sel_START (0)
#define PMIC_BUCK4012_CTRL3_b4012vo_ton_res_sel_END (2)
#define PMIC_BUCK4012_CTRL3_b4012cot_res_sel_START (3)
#define PMIC_BUCK4012_CTRL3_b4012cot_res_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b4012load_det_sel : 2;
        unsigned char b4012cot_ontime_isen_hys_sel : 2;
        unsigned char b4012cot_ontime_isen_bias_sel : 2;
        unsigned char b4012pvdd_ton_sel : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK4012_CTRL4_UNION;
#endif
#define PMIC_BUCK4012_CTRL4_b4012load_det_sel_START (0)
#define PMIC_BUCK4012_CTRL4_b4012load_det_sel_END (1)
#define PMIC_BUCK4012_CTRL4_b4012cot_ontime_isen_hys_sel_START (2)
#define PMIC_BUCK4012_CTRL4_b4012cot_ontime_isen_hys_sel_END (3)
#define PMIC_BUCK4012_CTRL4_b4012cot_ontime_isen_bias_sel_START (4)
#define PMIC_BUCK4012_CTRL4_b4012cot_ontime_isen_bias_sel_END (5)
#define PMIC_BUCK4012_CTRL4_b4012pvdd_ton_sel_START (6)
#define PMIC_BUCK4012_CTRL4_b4012pvdd_ton_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck4012_reserve0 : 8;
    } reg;
} PMIC_BUCK4012_RESERVE0_UNION;
#endif
#define PMIC_BUCK4012_RESERVE0_buck4012_reserve0_START (0)
#define PMIC_BUCK4012_RESERVE0_buck4012_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck4012_reserve1 : 8;
    } reg;
} PMIC_BUCK4012_RESERVE1_UNION;
#endif
#define PMIC_BUCK4012_RESERVE1_buck4012_reserve1_START (0)
#define PMIC_BUCK4012_RESERVE1_buck4012_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck4012_reserve2 : 8;
    } reg;
} PMIC_BUCK4012_RESERVE2_UNION;
#endif
#define PMIC_BUCK4012_RESERVE2_buck4012_reserve2_START (0)
#define PMIC_BUCK4012_RESERVE2_buck4012_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck4012_reserve3 : 8;
    } reg;
} PMIC_BUCK4012_RESERVE3_UNION;
#endif
#define PMIC_BUCK4012_RESERVE3_buck4012_reserve3_START (0)
#define PMIC_BUCK4012_RESERVE3_buck4012_reserve3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b40_n_sel : 4;
        unsigned char b40_p_sel : 4;
    } reg;
} PMIC_BUCK40_CTRL0_UNION;
#endif
#define PMIC_BUCK40_CTRL0_b40_n_sel_START (0)
#define PMIC_BUCK40_CTRL0_b40_n_sel_END (3)
#define PMIC_BUCK40_CTRL0_b40_p_sel_START (4)
#define PMIC_BUCK40_CTRL0_b40_p_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b40_buck401_dt_sel : 8;
    } reg;
} PMIC_BUCK40_CTRL1_UNION;
#endif
#define PMIC_BUCK40_CTRL1_b40_buck401_dt_sel_START (0)
#define PMIC_BUCK40_CTRL1_b40_buck401_dt_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b40_ng_dt_sel : 3;
        unsigned char b40_pg_dt_sel : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK40_CTRL2_UNION;
#endif
#define PMIC_BUCK40_CTRL2_b40_ng_dt_sel_START (0)
#define PMIC_BUCK40_CTRL2_b40_ng_dt_sel_END (2)
#define PMIC_BUCK40_CTRL2_b40_pg_dt_sel_START (3)
#define PMIC_BUCK40_CTRL2_b40_pg_dt_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b40_pg_n_sel : 3;
        unsigned char b40_pg_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK40_CTRL3_UNION;
#endif
#define PMIC_BUCK40_CTRL3_b40_pg_n_sel_START (0)
#define PMIC_BUCK40_CTRL3_b40_pg_n_sel_END (2)
#define PMIC_BUCK40_CTRL3_b40_pg_p_sel_START (3)
#define PMIC_BUCK40_CTRL3_b40_pg_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b40_ng_n_sel : 3;
        unsigned char b40_ng_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK40_CTRL4_UNION;
#endif
#define PMIC_BUCK40_CTRL4_b40_ng_n_sel_START (0)
#define PMIC_BUCK40_CTRL4_b40_ng_n_sel_END (2)
#define PMIC_BUCK40_CTRL4_b40_ng_p_sel_START (3)
#define PMIC_BUCK40_CTRL4_b40_ng_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b40_ron_test_sel : 3;
        unsigned char b40_drv_sw_sel : 1;
        unsigned char b40_drv_dt_sel : 4;
    } reg;
} PMIC_BUCK40_CTRL5_UNION;
#endif
#define PMIC_BUCK40_CTRL5_b40_ron_test_sel_START (0)
#define PMIC_BUCK40_CTRL5_b40_ron_test_sel_END (2)
#define PMIC_BUCK40_CTRL5_b40_drv_sw_sel_START (3)
#define PMIC_BUCK40_CTRL5_b40_drv_sw_sel_END (3)
#define PMIC_BUCK40_CTRL5_b40_drv_dt_sel_START (4)
#define PMIC_BUCK40_CTRL5_b40_drv_dt_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b40_stsw_atest : 1;
        unsigned char b40_ph_sel : 1;
        unsigned char b40_eatrim_en : 1;
        unsigned char b40_en_test : 1;
        unsigned char b40_pocp_sw_atest_sel : 1;
        unsigned char b40_npocp_atest_sel : 1;
        unsigned char b40_cmp_atest_sel : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK40_CTRL6_UNION;
#endif
#define PMIC_BUCK40_CTRL6_b40_stsw_atest_START (0)
#define PMIC_BUCK40_CTRL6_b40_stsw_atest_END (0)
#define PMIC_BUCK40_CTRL6_b40_ph_sel_START (1)
#define PMIC_BUCK40_CTRL6_b40_ph_sel_END (1)
#define PMIC_BUCK40_CTRL6_b40_eatrim_en_START (2)
#define PMIC_BUCK40_CTRL6_b40_eatrim_en_END (2)
#define PMIC_BUCK40_CTRL6_b40_en_test_START (3)
#define PMIC_BUCK40_CTRL6_b40_en_test_END (3)
#define PMIC_BUCK40_CTRL6_b40_pocp_sw_atest_sel_START (4)
#define PMIC_BUCK40_CTRL6_b40_pocp_sw_atest_sel_END (4)
#define PMIC_BUCK40_CTRL6_b40_npocp_atest_sel_START (5)
#define PMIC_BUCK40_CTRL6_b40_npocp_atest_sel_END (5)
#define PMIC_BUCK40_CTRL6_b40_cmp_atest_sel_START (6)
#define PMIC_BUCK40_CTRL6_b40_cmp_atest_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b40_dmd_atest : 1;
        unsigned char b40_dmd_ngc_set : 2;
        unsigned char b40_dmd_man_set : 4;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK40_CTRL7_UNION;
#endif
#define PMIC_BUCK40_CTRL7_b40_dmd_atest_START (0)
#define PMIC_BUCK40_CTRL7_b40_dmd_atest_END (0)
#define PMIC_BUCK40_CTRL7_b40_dmd_ngc_set_START (1)
#define PMIC_BUCK40_CTRL7_b40_dmd_ngc_set_END (2)
#define PMIC_BUCK40_CTRL7_b40_dmd_man_set_START (3)
#define PMIC_BUCK40_CTRL7_b40_dmd_man_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b40_cot_ont_ecoa : 8;
    } reg;
} PMIC_BUCK40_CTRL8_UNION;
#endif
#define PMIC_BUCK40_CTRL8_b40_cot_ont_ecoa_START (0)
#define PMIC_BUCK40_CTRL8_b40_cot_ont_ecoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b40_cot_atest : 1;
        unsigned char b40_cot_ont_necob : 3;
        unsigned char b40_cot_ont_ecob : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK40_CTRL9_UNION;
#endif
#define PMIC_BUCK40_CTRL9_b40_cot_atest_START (0)
#define PMIC_BUCK40_CTRL9_b40_cot_atest_END (0)
#define PMIC_BUCK40_CTRL9_b40_cot_ont_necob_START (1)
#define PMIC_BUCK40_CTRL9_b40_cot_ont_necob_END (3)
#define PMIC_BUCK40_CTRL9_b40_cot_ont_ecob_START (4)
#define PMIC_BUCK40_CTRL9_b40_cot_ont_ecob_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b40_cot_ont_necoa : 8;
    } reg;
} PMIC_BUCK40_CTRL10_UNION;
#endif
#define PMIC_BUCK40_CTRL10_b40_cot_ont_necoa_START (0)
#define PMIC_BUCK40_CTRL10_b40_cot_ont_necoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b41_n_sel : 4;
        unsigned char b41_p_sel : 4;
    } reg;
} PMIC_BUCK41_CTRL0_UNION;
#endif
#define PMIC_BUCK41_CTRL0_b41_n_sel_START (0)
#define PMIC_BUCK41_CTRL0_b41_n_sel_END (3)
#define PMIC_BUCK41_CTRL0_b41_p_sel_START (4)
#define PMIC_BUCK41_CTRL0_b41_p_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b41_buck401_dt_sel : 8;
    } reg;
} PMIC_BUCK41_CTRL1_UNION;
#endif
#define PMIC_BUCK41_CTRL1_b41_buck401_dt_sel_START (0)
#define PMIC_BUCK41_CTRL1_b41_buck401_dt_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b41_ng_dt_sel : 3;
        unsigned char b41_pg_dt_sel : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK41_CTRL2_UNION;
#endif
#define PMIC_BUCK41_CTRL2_b41_ng_dt_sel_START (0)
#define PMIC_BUCK41_CTRL2_b41_ng_dt_sel_END (2)
#define PMIC_BUCK41_CTRL2_b41_pg_dt_sel_START (3)
#define PMIC_BUCK41_CTRL2_b41_pg_dt_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b41_pg_n_sel : 3;
        unsigned char b41_pg_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK41_CTRL3_UNION;
#endif
#define PMIC_BUCK41_CTRL3_b41_pg_n_sel_START (0)
#define PMIC_BUCK41_CTRL3_b41_pg_n_sel_END (2)
#define PMIC_BUCK41_CTRL3_b41_pg_p_sel_START (3)
#define PMIC_BUCK41_CTRL3_b41_pg_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b41_ng_n_sel : 3;
        unsigned char b41_ng_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK41_CTRL4_UNION;
#endif
#define PMIC_BUCK41_CTRL4_b41_ng_n_sel_START (0)
#define PMIC_BUCK41_CTRL4_b41_ng_n_sel_END (2)
#define PMIC_BUCK41_CTRL4_b41_ng_p_sel_START (3)
#define PMIC_BUCK41_CTRL4_b41_ng_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b41_ron_test_sel : 3;
        unsigned char b41_drv_sw_sel : 1;
        unsigned char b41_drv_dt_sel : 4;
    } reg;
} PMIC_BUCK41_CTRL5_UNION;
#endif
#define PMIC_BUCK41_CTRL5_b41_ron_test_sel_START (0)
#define PMIC_BUCK41_CTRL5_b41_ron_test_sel_END (2)
#define PMIC_BUCK41_CTRL5_b41_drv_sw_sel_START (3)
#define PMIC_BUCK41_CTRL5_b41_drv_sw_sel_END (3)
#define PMIC_BUCK41_CTRL5_b41_drv_dt_sel_START (4)
#define PMIC_BUCK41_CTRL5_b41_drv_dt_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b41_stsw_atest : 1;
        unsigned char b41_ph_sel : 1;
        unsigned char b41_eatrim_en : 1;
        unsigned char b41_en_test : 1;
        unsigned char b41_pocp_sw_atest_sel : 1;
        unsigned char b41_npocp_atest_sel : 1;
        unsigned char b41_cmp_atest_sel : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK41_CTRL6_UNION;
#endif
#define PMIC_BUCK41_CTRL6_b41_stsw_atest_START (0)
#define PMIC_BUCK41_CTRL6_b41_stsw_atest_END (0)
#define PMIC_BUCK41_CTRL6_b41_ph_sel_START (1)
#define PMIC_BUCK41_CTRL6_b41_ph_sel_END (1)
#define PMIC_BUCK41_CTRL6_b41_eatrim_en_START (2)
#define PMIC_BUCK41_CTRL6_b41_eatrim_en_END (2)
#define PMIC_BUCK41_CTRL6_b41_en_test_START (3)
#define PMIC_BUCK41_CTRL6_b41_en_test_END (3)
#define PMIC_BUCK41_CTRL6_b41_pocp_sw_atest_sel_START (4)
#define PMIC_BUCK41_CTRL6_b41_pocp_sw_atest_sel_END (4)
#define PMIC_BUCK41_CTRL6_b41_npocp_atest_sel_START (5)
#define PMIC_BUCK41_CTRL6_b41_npocp_atest_sel_END (5)
#define PMIC_BUCK41_CTRL6_b41_cmp_atest_sel_START (6)
#define PMIC_BUCK41_CTRL6_b41_cmp_atest_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b41_dmd_atest : 1;
        unsigned char b41_dmd_ngc_set : 2;
        unsigned char b41_dmd_man_set : 4;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK41_CTRL7_UNION;
#endif
#define PMIC_BUCK41_CTRL7_b41_dmd_atest_START (0)
#define PMIC_BUCK41_CTRL7_b41_dmd_atest_END (0)
#define PMIC_BUCK41_CTRL7_b41_dmd_ngc_set_START (1)
#define PMIC_BUCK41_CTRL7_b41_dmd_ngc_set_END (2)
#define PMIC_BUCK41_CTRL7_b41_dmd_man_set_START (3)
#define PMIC_BUCK41_CTRL7_b41_dmd_man_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b41_cot_ont_ecoa : 8;
    } reg;
} PMIC_BUCK41_CTRL8_UNION;
#endif
#define PMIC_BUCK41_CTRL8_b41_cot_ont_ecoa_START (0)
#define PMIC_BUCK41_CTRL8_b41_cot_ont_ecoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b41_cot_atest : 1;
        unsigned char b41_cot_ont_necob : 3;
        unsigned char b41_cot_ont_ecob : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK41_CTRL9_UNION;
#endif
#define PMIC_BUCK41_CTRL9_b41_cot_atest_START (0)
#define PMIC_BUCK41_CTRL9_b41_cot_atest_END (0)
#define PMIC_BUCK41_CTRL9_b41_cot_ont_necob_START (1)
#define PMIC_BUCK41_CTRL9_b41_cot_ont_necob_END (3)
#define PMIC_BUCK41_CTRL9_b41_cot_ont_ecob_START (4)
#define PMIC_BUCK41_CTRL9_b41_cot_ont_ecob_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b41_cot_ont_necoa : 8;
    } reg;
} PMIC_BUCK41_CTRL10_UNION;
#endif
#define PMIC_BUCK41_CTRL10_b41_cot_ont_necoa_START (0)
#define PMIC_BUCK41_CTRL10_b41_cot_ont_necoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_n_sel : 4;
        unsigned char b42_p_sel : 4;
    } reg;
} PMIC_BUCK42_CTRL0_UNION;
#endif
#define PMIC_BUCK42_CTRL0_b42_n_sel_START (0)
#define PMIC_BUCK42_CTRL0_b42_n_sel_END (3)
#define PMIC_BUCK42_CTRL0_b42_p_sel_START (4)
#define PMIC_BUCK42_CTRL0_b42_p_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_buck401_dt_sel : 8;
    } reg;
} PMIC_BUCK42_CTRL1_UNION;
#endif
#define PMIC_BUCK42_CTRL1_b42_buck401_dt_sel_START (0)
#define PMIC_BUCK42_CTRL1_b42_buck401_dt_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ng_dt_sel : 3;
        unsigned char b42_pg_dt_sel : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK42_CTRL2_UNION;
#endif
#define PMIC_BUCK42_CTRL2_b42_ng_dt_sel_START (0)
#define PMIC_BUCK42_CTRL2_b42_ng_dt_sel_END (2)
#define PMIC_BUCK42_CTRL2_b42_pg_dt_sel_START (3)
#define PMIC_BUCK42_CTRL2_b42_pg_dt_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_pg_n_sel : 3;
        unsigned char b42_pg_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK42_CTRL3_UNION;
#endif
#define PMIC_BUCK42_CTRL3_b42_pg_n_sel_START (0)
#define PMIC_BUCK42_CTRL3_b42_pg_n_sel_END (2)
#define PMIC_BUCK42_CTRL3_b42_pg_p_sel_START (3)
#define PMIC_BUCK42_CTRL3_b42_pg_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ng_n_sel : 3;
        unsigned char b42_ng_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK42_CTRL4_UNION;
#endif
#define PMIC_BUCK42_CTRL4_b42_ng_n_sel_START (0)
#define PMIC_BUCK42_CTRL4_b42_ng_n_sel_END (2)
#define PMIC_BUCK42_CTRL4_b42_ng_p_sel_START (3)
#define PMIC_BUCK42_CTRL4_b42_ng_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ron_test_sel : 3;
        unsigned char b42_drv_sw_sel : 1;
        unsigned char b42_drv_dt_sel : 4;
    } reg;
} PMIC_BUCK42_CTRL5_UNION;
#endif
#define PMIC_BUCK42_CTRL5_b42_ron_test_sel_START (0)
#define PMIC_BUCK42_CTRL5_b42_ron_test_sel_END (2)
#define PMIC_BUCK42_CTRL5_b42_drv_sw_sel_START (3)
#define PMIC_BUCK42_CTRL5_b42_drv_sw_sel_END (3)
#define PMIC_BUCK42_CTRL5_b42_drv_dt_sel_START (4)
#define PMIC_BUCK42_CTRL5_b42_drv_dt_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_stsw_atest : 1;
        unsigned char b42_ph_sel : 1;
        unsigned char b42_eatrim_en : 1;
        unsigned char b42_en_test : 1;
        unsigned char b42_pocp_sw_atest_sel : 1;
        unsigned char b42_npocp_atest_sel : 1;
        unsigned char b42_cmp_atest_sel : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK42_CTRL6_UNION;
#endif
#define PMIC_BUCK42_CTRL6_b42_stsw_atest_START (0)
#define PMIC_BUCK42_CTRL6_b42_stsw_atest_END (0)
#define PMIC_BUCK42_CTRL6_b42_ph_sel_START (1)
#define PMIC_BUCK42_CTRL6_b42_ph_sel_END (1)
#define PMIC_BUCK42_CTRL6_b42_eatrim_en_START (2)
#define PMIC_BUCK42_CTRL6_b42_eatrim_en_END (2)
#define PMIC_BUCK42_CTRL6_b42_en_test_START (3)
#define PMIC_BUCK42_CTRL6_b42_en_test_END (3)
#define PMIC_BUCK42_CTRL6_b42_pocp_sw_atest_sel_START (4)
#define PMIC_BUCK42_CTRL6_b42_pocp_sw_atest_sel_END (4)
#define PMIC_BUCK42_CTRL6_b42_npocp_atest_sel_START (5)
#define PMIC_BUCK42_CTRL6_b42_npocp_atest_sel_END (5)
#define PMIC_BUCK42_CTRL6_b42_cmp_atest_sel_START (6)
#define PMIC_BUCK42_CTRL6_b42_cmp_atest_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_dmd_atest : 1;
        unsigned char b42_dmd_ngc_set : 2;
        unsigned char b42_dmd_man_set : 4;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK42_CTRL7_UNION;
#endif
#define PMIC_BUCK42_CTRL7_b42_dmd_atest_START (0)
#define PMIC_BUCK42_CTRL7_b42_dmd_atest_END (0)
#define PMIC_BUCK42_CTRL7_b42_dmd_ngc_set_START (1)
#define PMIC_BUCK42_CTRL7_b42_dmd_ngc_set_END (2)
#define PMIC_BUCK42_CTRL7_b42_dmd_man_set_START (3)
#define PMIC_BUCK42_CTRL7_b42_dmd_man_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_cot_ont_ecoa : 8;
    } reg;
} PMIC_BUCK42_CTRL8_UNION;
#endif
#define PMIC_BUCK42_CTRL8_b42_cot_ont_ecoa_START (0)
#define PMIC_BUCK42_CTRL8_b42_cot_ont_ecoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_cot_atest : 1;
        unsigned char b42_cot_ont_necob : 3;
        unsigned char b42_cot_ont_ecob : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK42_CTRL9_UNION;
#endif
#define PMIC_BUCK42_CTRL9_b42_cot_atest_START (0)
#define PMIC_BUCK42_CTRL9_b42_cot_atest_END (0)
#define PMIC_BUCK42_CTRL9_b42_cot_ont_necob_START (1)
#define PMIC_BUCK42_CTRL9_b42_cot_ont_necob_END (3)
#define PMIC_BUCK42_CTRL9_b42_cot_ont_ecob_START (4)
#define PMIC_BUCK42_CTRL9_b42_cot_ont_ecob_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_cot_ont_necoa : 8;
    } reg;
} PMIC_BUCK42_CTRL10_UNION;
#endif
#define PMIC_BUCK42_CTRL10_b42_cot_ont_necoa_START (0)
#define PMIC_BUCK42_CTRL10_b42_cot_ont_necoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_buffer_bias : 2;
        unsigned char b42_ramp_sel : 1;
        unsigned char b42_code_mode_sel : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK42_CTRL11_UNION;
#endif
#define PMIC_BUCK42_CTRL11_b42_buffer_bias_START (0)
#define PMIC_BUCK42_CTRL11_b42_buffer_bias_END (1)
#define PMIC_BUCK42_CTRL11_b42_ramp_sel_START (2)
#define PMIC_BUCK42_CTRL11_b42_ramp_sel_END (2)
#define PMIC_BUCK42_CTRL11_b42_code_mode_sel_START (3)
#define PMIC_BUCK42_CTRL11_b42_code_mode_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_clk9m6_pulse_sel : 2;
        unsigned char b42_vref_test_sel : 2;
        unsigned char b42_vref_vo_cap_sel : 2;
        unsigned char b42_rtr_cap_sel : 2;
    } reg;
} PMIC_BUCK42_CTRL12_UNION;
#endif
#define PMIC_BUCK42_CTRL12_b42_clk9m6_pulse_sel_START (0)
#define PMIC_BUCK42_CTRL12_b42_clk9m6_pulse_sel_END (1)
#define PMIC_BUCK42_CTRL12_b42_vref_test_sel_START (2)
#define PMIC_BUCK42_CTRL12_b42_vref_test_sel_END (3)
#define PMIC_BUCK42_CTRL12_b42_vref_vo_cap_sel_START (4)
#define PMIC_BUCK42_CTRL12_b42_vref_vo_cap_sel_END (5)
#define PMIC_BUCK42_CTRL12_b42_rtr_cap_sel_START (6)
#define PMIC_BUCK42_CTRL12_b42_rtr_cap_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ton_det : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK42_CTRL13_UNION;
#endif
#define PMIC_BUCK42_CTRL13_b42_ton_det_START (0)
#define PMIC_BUCK42_CTRL13_b42_ton_det_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_cot_cmphys : 1;
        unsigned char b42_cot_ibst_sel : 1;
        unsigned char b42_cot_sche_neco : 1;
        unsigned char b42_cot_sche_eco : 1;
        unsigned char b42_cot_sel : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK42_CTRL14_UNION;
#endif
#define PMIC_BUCK42_CTRL14_b42_cot_cmphys_START (0)
#define PMIC_BUCK42_CTRL14_b42_cot_cmphys_END (0)
#define PMIC_BUCK42_CTRL14_b42_cot_ibst_sel_START (1)
#define PMIC_BUCK42_CTRL14_b42_cot_ibst_sel_END (1)
#define PMIC_BUCK42_CTRL14_b42_cot_sche_neco_START (2)
#define PMIC_BUCK42_CTRL14_b42_cot_sche_neco_END (2)
#define PMIC_BUCK42_CTRL14_b42_cot_sche_eco_START (3)
#define PMIC_BUCK42_CTRL14_b42_cot_sche_eco_END (3)
#define PMIC_BUCK42_CTRL14_b42_cot_sel_START (4)
#define PMIC_BUCK42_CTRL14_b42_cot_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_c1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK42_CTRL15_UNION;
#endif
#define PMIC_BUCK42_CTRL15_b42_c1_sel_START (0)
#define PMIC_BUCK42_CTRL15_b42_c1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_c2_sel : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK42_CTRL16_UNION;
#endif
#define PMIC_BUCK42_CTRL16_b42_c2_sel_START (0)
#define PMIC_BUCK42_CTRL16_b42_c2_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_c3_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK42_CTRL17_UNION;
#endif
#define PMIC_BUCK42_CTRL17_b42_c3_sel_START (0)
#define PMIC_BUCK42_CTRL17_b42_c3_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_r1_sel : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK42_CTRL18_UNION;
#endif
#define PMIC_BUCK42_CTRL18_b42_r1_sel_START (0)
#define PMIC_BUCK42_CTRL18_b42_r1_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_r2_sel : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK42_CTRL19_UNION;
#endif
#define PMIC_BUCK42_CTRL19_b42_r2_sel_START (0)
#define PMIC_BUCK42_CTRL19_b42_r2_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_r3_sel : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK42_CTRL20_UNION;
#endif
#define PMIC_BUCK42_CTRL20_b42_r3_sel_START (0)
#define PMIC_BUCK42_CTRL20_b42_r3_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_reg_idrp : 3;
        unsigned char b42_reg_idrn : 3;
        unsigned char b42_eco_shield : 1;
        unsigned char b42_clamp_ref_sel : 1;
    } reg;
} PMIC_BUCK42_CTRL21_UNION;
#endif
#define PMIC_BUCK42_CTRL21_b42_reg_idrp_START (0)
#define PMIC_BUCK42_CTRL21_b42_reg_idrp_END (2)
#define PMIC_BUCK42_CTRL21_b42_reg_idrn_START (3)
#define PMIC_BUCK42_CTRL21_b42_reg_idrn_END (5)
#define PMIC_BUCK42_CTRL21_b42_eco_shield_START (6)
#define PMIC_BUCK42_CTRL21_b42_eco_shield_END (6)
#define PMIC_BUCK42_CTRL21_b42_clamp_ref_sel_START (7)
#define PMIC_BUCK42_CTRL21_b42_clamp_ref_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ramp_valley_sel : 1;
        unsigned char b42_eaamp_clamp : 2;
        unsigned char b42_eacomp_ecocontrol_shield : 1;
        unsigned char b42_bw_sel : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK42_CTRL22_UNION;
#endif
#define PMIC_BUCK42_CTRL22_b42_ramp_valley_sel_START (0)
#define PMIC_BUCK42_CTRL22_b42_ramp_valley_sel_END (0)
#define PMIC_BUCK42_CTRL22_b42_eaamp_clamp_START (1)
#define PMIC_BUCK42_CTRL22_b42_eaamp_clamp_END (2)
#define PMIC_BUCK42_CTRL22_b42_eacomp_ecocontrol_shield_START (3)
#define PMIC_BUCK42_CTRL22_b42_eacomp_ecocontrol_shield_END (3)
#define PMIC_BUCK42_CTRL22_b42_bw_sel_START (4)
#define PMIC_BUCK42_CTRL22_b42_bw_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_eaclamp_srgap_sel : 4;
        unsigned char b42_eaclamp_brgap_sel : 4;
    } reg;
} PMIC_BUCK42_CTRL23_UNION;
#endif
#define PMIC_BUCK42_CTRL23_b42_eaclamp_srgap_sel_START (0)
#define PMIC_BUCK42_CTRL23_b42_eaclamp_srgap_sel_END (3)
#define PMIC_BUCK42_CTRL23_b42_eaclamp_brgap_sel_START (4)
#define PMIC_BUCK42_CTRL23_b42_eaclamp_brgap_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_eaclamp_ecogap_sel : 2;
        unsigned char b42_trim2 : 1;
        unsigned char b42_trim1 : 1;
        unsigned char b42_eaclamp_gap_sel : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK42_CTRL24_UNION;
#endif
#define PMIC_BUCK42_CTRL24_b42_eaclamp_ecogap_sel_START (0)
#define PMIC_BUCK42_CTRL24_b42_eaclamp_ecogap_sel_END (1)
#define PMIC_BUCK42_CTRL24_b42_trim2_START (2)
#define PMIC_BUCK42_CTRL24_b42_trim2_END (2)
#define PMIC_BUCK42_CTRL24_b42_trim1_START (3)
#define PMIC_BUCK42_CTRL24_b42_trim1_END (3)
#define PMIC_BUCK42_CTRL24_b42_eaclamp_gap_sel_START (4)
#define PMIC_BUCK42_CTRL24_b42_eaclamp_gap_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_eabias_sel : 1;
        unsigned char b42_buck42_ea_r : 3;
        unsigned char b42_ea_ecocur_sel : 3;
        unsigned char b42_eaamp_clamp_sel : 1;
    } reg;
} PMIC_BUCK42_CTRL25_UNION;
#endif
#define PMIC_BUCK42_CTRL25_b42_eabias_sel_START (0)
#define PMIC_BUCK42_CTRL25_b42_eabias_sel_END (0)
#define PMIC_BUCK42_CTRL25_b42_buck42_ea_r_START (1)
#define PMIC_BUCK42_CTRL25_b42_buck42_ea_r_END (3)
#define PMIC_BUCK42_CTRL25_b42_ea_ecocur_sel_START (4)
#define PMIC_BUCK42_CTRL25_b42_ea_ecocur_sel_END (6)
#define PMIC_BUCK42_CTRL25_b42_eaamp_clamp_sel_START (7)
#define PMIC_BUCK42_CTRL25_b42_eaamp_clamp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_clampeco_bias_sel : 4;
        unsigned char b42_clamp_reg_idrp : 4;
    } reg;
} PMIC_BUCK42_CTRL26_UNION;
#endif
#define PMIC_BUCK42_CTRL26_b42_clampeco_bias_sel_START (0)
#define PMIC_BUCK42_CTRL26_b42_clampeco_bias_sel_END (3)
#define PMIC_BUCK42_CTRL26_b42_clamp_reg_idrp_START (4)
#define PMIC_BUCK42_CTRL26_b42_clamp_reg_idrp_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ovp_shield : 1;
        unsigned char b42_ovp_sel : 2;
        unsigned char b42_short_shield : 1;
        unsigned char b42_sft_sel : 1;
        unsigned char b42_ocp_short_en : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK42_CTRL27_UNION;
#endif
#define PMIC_BUCK42_CTRL27_b42_ovp_shield_START (0)
#define PMIC_BUCK42_CTRL27_b42_ovp_shield_END (0)
#define PMIC_BUCK42_CTRL27_b42_ovp_sel_START (1)
#define PMIC_BUCK42_CTRL27_b42_ovp_sel_END (2)
#define PMIC_BUCK42_CTRL27_b42_short_shield_START (3)
#define PMIC_BUCK42_CTRL27_b42_short_shield_END (3)
#define PMIC_BUCK42_CTRL27_b42_sft_sel_START (4)
#define PMIC_BUCK42_CTRL27_b42_sft_sel_END (4)
#define PMIC_BUCK42_CTRL27_b42_ocp_short_en_START (5)
#define PMIC_BUCK42_CTRL27_b42_ocp_short_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_dbias_eco : 3;
        unsigned char b42_dbias : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK42_CTRL28_UNION;
#endif
#define PMIC_BUCK42_CTRL28_b42_dbias_eco_START (0)
#define PMIC_BUCK42_CTRL28_b42_dbias_eco_END (2)
#define PMIC_BUCK42_CTRL28_b42_dbias_START (3)
#define PMIC_BUCK42_CTRL28_b42_dbias_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ramptrim_sel : 1;
        unsigned char b42_forcenormal : 1;
        unsigned char b42_normaltrim_shield : 1;
        unsigned char b42_ecotrim_shield : 1;
        unsigned char b42_forceeco : 1;
        unsigned char b42_eco_shield_cmp : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK42_CTRL29_UNION;
#endif
#define PMIC_BUCK42_CTRL29_b42_ramptrim_sel_START (0)
#define PMIC_BUCK42_CTRL29_b42_ramptrim_sel_END (0)
#define PMIC_BUCK42_CTRL29_b42_forcenormal_START (1)
#define PMIC_BUCK42_CTRL29_b42_forcenormal_END (1)
#define PMIC_BUCK42_CTRL29_b42_normaltrim_shield_START (2)
#define PMIC_BUCK42_CTRL29_b42_normaltrim_shield_END (2)
#define PMIC_BUCK42_CTRL29_b42_ecotrim_shield_START (3)
#define PMIC_BUCK42_CTRL29_b42_ecotrim_shield_END (3)
#define PMIC_BUCK42_CTRL29_b42_forceeco_START (4)
#define PMIC_BUCK42_CTRL29_b42_forceeco_END (4)
#define PMIC_BUCK42_CTRL29_b42_eco_shield_cmp_START (5)
#define PMIC_BUCK42_CTRL29_b42_eco_shield_cmp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_loadline_set : 5;
        unsigned char b42_loadline_fon : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK42_CTRL30_UNION;
#endif
#define PMIC_BUCK42_CTRL30_b42_loadline_set_START (0)
#define PMIC_BUCK42_CTRL30_b42_loadline_set_END (4)
#define PMIC_BUCK42_CTRL30_b42_loadline_fon_START (5)
#define PMIC_BUCK42_CTRL30_b42_loadline_fon_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_stsw_fstsch_neco : 3;
        unsigned char b42_stsw_fstsch_eco : 3;
        unsigned char b42_stsw_rpup_ccm : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK42_CTRL31_UNION;
#endif
#define PMIC_BUCK42_CTRL31_b42_stsw_fstsch_neco_START (0)
#define PMIC_BUCK42_CTRL31_b42_stsw_fstsch_neco_END (2)
#define PMIC_BUCK42_CTRL31_b42_stsw_fstsch_eco_START (3)
#define PMIC_BUCK42_CTRL31_b42_stsw_fstsch_eco_END (5)
#define PMIC_BUCK42_CTRL31_b42_stsw_rpup_ccm_START (6)
#define PMIC_BUCK42_CTRL31_b42_stsw_rpup_ccm_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_stsw_slwsch_neco : 2;
        unsigned char b42_stsw_slwsch_eco : 2;
        unsigned char b42_stsw_fstps_ith : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK42_CTRL32_UNION;
#endif
#define PMIC_BUCK42_CTRL32_b42_stsw_slwsch_neco_START (0)
#define PMIC_BUCK42_CTRL32_b42_stsw_slwsch_neco_END (1)
#define PMIC_BUCK42_CTRL32_b42_stsw_slwsch_eco_START (2)
#define PMIC_BUCK42_CTRL32_b42_stsw_slwsch_eco_END (3)
#define PMIC_BUCK42_CTRL32_b42_stsw_fstps_ith_START (4)
#define PMIC_BUCK42_CTRL32_b42_stsw_fstps_ith_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_stsw_slw_nohys : 1;
        unsigned char b42_stsw_slw_ct2cm_ith : 3;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK42_CTRL33_UNION;
#endif
#define PMIC_BUCK42_CTRL33_b42_stsw_slw_nohys_START (0)
#define PMIC_BUCK42_CTRL33_b42_stsw_slw_nohys_END (0)
#define PMIC_BUCK42_CTRL33_b42_stsw_slw_ct2cm_ith_START (1)
#define PMIC_BUCK42_CTRL33_b42_stsw_slw_ct2cm_ith_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_stsw_st_set : 2;
        unsigned char b42_stsw_slw_dn_td : 2;
        unsigned char b42_stsw_slw_up_td : 2;
        unsigned char b42_stsw_slw_ct2cm_hys : 2;
    } reg;
} PMIC_BUCK42_CTRL34_UNION;
#endif
#define PMIC_BUCK42_CTRL34_b42_stsw_st_set_START (0)
#define PMIC_BUCK42_CTRL34_b42_stsw_st_set_END (1)
#define PMIC_BUCK42_CTRL34_b42_stsw_slw_dn_td_START (2)
#define PMIC_BUCK42_CTRL34_b42_stsw_slw_dn_td_END (3)
#define PMIC_BUCK42_CTRL34_b42_stsw_slw_up_td_START (4)
#define PMIC_BUCK42_CTRL34_b42_stsw_slw_up_td_END (5)
#define PMIC_BUCK42_CTRL34_b42_stsw_slw_ct2cm_hys_START (6)
#define PMIC_BUCK42_CTRL34_b42_stsw_slw_ct2cm_hys_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ecdt_ct2cm_ft : 2;
        unsigned char b42_ecdt_ec2ct_ref : 3;
        unsigned char b42_ecdt_ct2cm_ref : 3;
    } reg;
} PMIC_BUCK42_CTRL35_UNION;
#endif
#define PMIC_BUCK42_CTRL35_b42_ecdt_ct2cm_ft_START (0)
#define PMIC_BUCK42_CTRL35_b42_ecdt_ct2cm_ft_END (1)
#define PMIC_BUCK42_CTRL35_b42_ecdt_ec2ct_ref_START (2)
#define PMIC_BUCK42_CTRL35_b42_ecdt_ec2ct_ref_END (4)
#define PMIC_BUCK42_CTRL35_b42_ecdt_ct2cm_ref_START (5)
#define PMIC_BUCK42_CTRL35_b42_ecdt_ct2cm_ref_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ecdt_fltrc : 2;
        unsigned char b42_ecdt_ct2ec_ft : 2;
        unsigned char b42_ecdt_cmphys : 1;
        unsigned char b42_ecdt_ct2ec_ref : 3;
    } reg;
} PMIC_BUCK42_CTRL36_UNION;
#endif
#define PMIC_BUCK42_CTRL36_b42_ecdt_fltrc_START (0)
#define PMIC_BUCK42_CTRL36_b42_ecdt_fltrc_END (1)
#define PMIC_BUCK42_CTRL36_b42_ecdt_ct2ec_ft_START (2)
#define PMIC_BUCK42_CTRL36_b42_ecdt_ct2ec_ft_END (3)
#define PMIC_BUCK42_CTRL36_b42_ecdt_cmphys_START (4)
#define PMIC_BUCK42_CTRL36_b42_ecdt_cmphys_END (4)
#define PMIC_BUCK42_CTRL36_b42_ecdt_ct2ec_ref_START (5)
#define PMIC_BUCK42_CTRL36_b42_ecdt_ct2ec_ref_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_min_toff_sel : 2;
        unsigned char b42_min_ton_sel : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK42_CTRL37_UNION;
#endif
#define PMIC_BUCK42_CTRL37_b42_min_toff_sel_START (0)
#define PMIC_BUCK42_CTRL37_b42_min_toff_sel_END (1)
#define PMIC_BUCK42_CTRL37_b42_min_ton_sel_START (2)
#define PMIC_BUCK42_CTRL37_b42_min_ton_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ctlogic_crson_td : 2;
        unsigned char b42_ctlogic_modsw : 1;
        unsigned char b42_ctlogic_sgph : 1;
        unsigned char b42_ctlogic_smph : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK42_CTRL38_UNION;
#endif
#define PMIC_BUCK42_CTRL38_b42_ctlogic_crson_td_START (0)
#define PMIC_BUCK42_CTRL38_b42_ctlogic_crson_td_END (1)
#define PMIC_BUCK42_CTRL38_b42_ctlogic_modsw_START (2)
#define PMIC_BUCK42_CTRL38_b42_ctlogic_modsw_END (2)
#define PMIC_BUCK42_CTRL38_b42_ctlogic_sgph_START (3)
#define PMIC_BUCK42_CTRL38_b42_ctlogic_sgph_END (3)
#define PMIC_BUCK42_CTRL38_b42_ctlogic_smph_START (4)
#define PMIC_BUCK42_CTRL38_b42_ctlogic_smph_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ramp_big_sel : 1;
        unsigned char b42_ramp_mid_sel : 1;
        unsigned char buck42_ramp_line_sel : 1;
        unsigned char buck42_ramp_ccm_outsel : 1;
        unsigned char buck42_ramp_cot_outsel : 1;
        unsigned char buck42_ramp_c : 2;
        unsigned char buck42_ramp_clk_sel : 1;
    } reg;
} PMIC_BUCK42_CTRL39_UNION;
#endif
#define PMIC_BUCK42_CTRL39_b42_ramp_big_sel_START (0)
#define PMIC_BUCK42_CTRL39_b42_ramp_big_sel_END (0)
#define PMIC_BUCK42_CTRL39_b42_ramp_mid_sel_START (1)
#define PMIC_BUCK42_CTRL39_b42_ramp_mid_sel_END (1)
#define PMIC_BUCK42_CTRL39_buck42_ramp_line_sel_START (2)
#define PMIC_BUCK42_CTRL39_buck42_ramp_line_sel_END (2)
#define PMIC_BUCK42_CTRL39_buck42_ramp_ccm_outsel_START (3)
#define PMIC_BUCK42_CTRL39_buck42_ramp_ccm_outsel_END (3)
#define PMIC_BUCK42_CTRL39_buck42_ramp_cot_outsel_START (4)
#define PMIC_BUCK42_CTRL39_buck42_ramp_cot_outsel_END (4)
#define PMIC_BUCK42_CTRL39_buck42_ramp_c_START (5)
#define PMIC_BUCK42_CTRL39_buck42_ramp_c_END (6)
#define PMIC_BUCK42_CTRL39_buck42_ramp_clk_sel_START (7)
#define PMIC_BUCK42_CTRL39_buck42_ramp_clk_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ramp_r_eco : 4;
        unsigned char b42_ramp_counter : 2;
        unsigned char b42_ramp_sw_sel : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK42_CTRL40_UNION;
#endif
#define PMIC_BUCK42_CTRL40_b42_ramp_r_eco_START (0)
#define PMIC_BUCK42_CTRL40_b42_ramp_r_eco_END (3)
#define PMIC_BUCK42_CTRL40_b42_ramp_counter_START (4)
#define PMIC_BUCK42_CTRL40_b42_ramp_counter_END (5)
#define PMIC_BUCK42_CTRL40_b42_ramp_sw_sel_START (6)
#define PMIC_BUCK42_CTRL40_b42_ramp_sw_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ramp_r_ccm : 4;
        unsigned char b42_ramp_r_cot : 4;
    } reg;
} PMIC_BUCK42_CTRL41_UNION;
#endif
#define PMIC_BUCK42_CTRL41_b42_ramp_r_ccm_START (0)
#define PMIC_BUCK42_CTRL41_b42_ramp_r_ccm_END (3)
#define PMIC_BUCK42_CTRL41_b42_ramp_r_cot_START (4)
#define PMIC_BUCK42_CTRL41_b42_ramp_r_cot_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ramp_fast_sel : 1;
        unsigned char b42_ramp_freq_sel : 1;
        unsigned char b42_ramp_res_test : 1;
        unsigned char b42_ramp_change_mode : 5;
    } reg;
} PMIC_BUCK42_CTRL42_UNION;
#endif
#define PMIC_BUCK42_CTRL42_b42_ramp_fast_sel_START (0)
#define PMIC_BUCK42_CTRL42_b42_ramp_fast_sel_END (0)
#define PMIC_BUCK42_CTRL42_b42_ramp_freq_sel_START (1)
#define PMIC_BUCK42_CTRL42_b42_ramp_freq_sel_END (1)
#define PMIC_BUCK42_CTRL42_b42_ramp_res_test_START (2)
#define PMIC_BUCK42_CTRL42_b42_ramp_res_test_END (2)
#define PMIC_BUCK42_CTRL42_b42_ramp_change_mode_START (3)
#define PMIC_BUCK42_CTRL42_b42_ramp_change_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_ramp_dblclk_sel : 1;
        unsigned char b42_ramp_force_eco_test : 1;
        unsigned char b42_ramp_buff_trim_test : 1;
        unsigned char b42_ramp_buff_trim : 1;
        unsigned char b42_ramp_buff_bias : 2;
        unsigned char b42_ramp_eco_buffer : 1;
        unsigned char buck42_ramp_trim_sel : 1;
    } reg;
} PMIC_BUCK42_CTRL43_UNION;
#endif
#define PMIC_BUCK42_CTRL43_b42_ramp_dblclk_sel_START (0)
#define PMIC_BUCK42_CTRL43_b42_ramp_dblclk_sel_END (0)
#define PMIC_BUCK42_CTRL43_b42_ramp_force_eco_test_START (1)
#define PMIC_BUCK42_CTRL43_b42_ramp_force_eco_test_END (1)
#define PMIC_BUCK42_CTRL43_b42_ramp_buff_trim_test_START (2)
#define PMIC_BUCK42_CTRL43_b42_ramp_buff_trim_test_END (2)
#define PMIC_BUCK42_CTRL43_b42_ramp_buff_trim_START (3)
#define PMIC_BUCK42_CTRL43_b42_ramp_buff_trim_END (3)
#define PMIC_BUCK42_CTRL43_b42_ramp_buff_bias_START (4)
#define PMIC_BUCK42_CTRL43_b42_ramp_buff_bias_END (5)
#define PMIC_BUCK42_CTRL43_b42_ramp_eco_buffer_START (6)
#define PMIC_BUCK42_CTRL43_b42_ramp_eco_buffer_END (6)
#define PMIC_BUCK42_CTRL43_buck42_ramp_trim_sel_START (7)
#define PMIC_BUCK42_CTRL43_buck42_ramp_trim_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b42_pulldn_pd_en : 1;
        unsigned char b42_pulldn_pd_sel : 1;
        unsigned char b42_pulldn_sel : 3;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK42_CTRL44_UNION;
#endif
#define PMIC_BUCK42_CTRL44_b42_pulldn_pd_en_START (0)
#define PMIC_BUCK42_CTRL44_b42_pulldn_pd_en_END (0)
#define PMIC_BUCK42_CTRL44_b42_pulldn_pd_sel_START (1)
#define PMIC_BUCK42_CTRL44_b42_pulldn_pd_sel_END (1)
#define PMIC_BUCK42_CTRL44_b42_pulldn_sel_START (2)
#define PMIC_BUCK42_CTRL44_b42_pulldn_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_cmp_ibias : 4;
        unsigned char b5_adj_rlx : 4;
    } reg;
} PMIC_BUCK5_CTRL0_UNION;
#endif
#define PMIC_BUCK5_CTRL0_b5_cmp_ibias_START (0)
#define PMIC_BUCK5_CTRL0_b5_cmp_ibias_END (3)
#define PMIC_BUCK5_CTRL0_b5_adj_rlx_START (4)
#define PMIC_BUCK5_CTRL0_b5_adj_rlx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_ptn_dt_ctrl : 1;
        unsigned char b5_ntp_dt_ctrl : 1;
        unsigned char reserved : 2;
        unsigned char b5_dt_sel : 1;
        unsigned char b5_ocp_sel : 3;
    } reg;
} PMIC_BUCK5_CTRL1_UNION;
#endif
#define PMIC_BUCK5_CTRL1_b5_ptn_dt_ctrl_START (0)
#define PMIC_BUCK5_CTRL1_b5_ptn_dt_ctrl_END (0)
#define PMIC_BUCK5_CTRL1_b5_ntp_dt_ctrl_START (1)
#define PMIC_BUCK5_CTRL1_b5_ntp_dt_ctrl_END (1)
#define PMIC_BUCK5_CTRL1_b5_dt_sel_START (4)
#define PMIC_BUCK5_CTRL1_b5_dt_sel_END (4)
#define PMIC_BUCK5_CTRL1_b5_ocp_sel_START (5)
#define PMIC_BUCK5_CTRL1_b5_ocp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_pg_n_sel : 3;
        unsigned char b5_pg_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK5_CTRL2_UNION;
#endif
#define PMIC_BUCK5_CTRL2_b5_pg_n_sel_START (0)
#define PMIC_BUCK5_CTRL2_b5_pg_n_sel_END (2)
#define PMIC_BUCK5_CTRL2_b5_pg_p_sel_START (3)
#define PMIC_BUCK5_CTRL2_b5_pg_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_ng_n_sel : 3;
        unsigned char b5_ng_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK5_CTRL3_UNION;
#endif
#define PMIC_BUCK5_CTRL3_b5_ng_n_sel_START (0)
#define PMIC_BUCK5_CTRL3_b5_ng_n_sel_END (2)
#define PMIC_BUCK5_CTRL3_b5_ng_p_sel_START (3)
#define PMIC_BUCK5_CTRL3_b5_ng_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_reg_r : 2;
        unsigned char reserved_0 : 2;
        unsigned char b5_reg_en : 1;
        unsigned char b5_adj_clx : 2;
        unsigned char reserved_1 : 1;
    } reg;
} PMIC_BUCK5_CTRL4_UNION;
#endif
#define PMIC_BUCK5_CTRL4_b5_reg_r_START (0)
#define PMIC_BUCK5_CTRL4_b5_reg_r_END (1)
#define PMIC_BUCK5_CTRL4_b5_reg_en_START (4)
#define PMIC_BUCK5_CTRL4_b5_reg_en_END (4)
#define PMIC_BUCK5_CTRL4_b5_adj_clx_START (5)
#define PMIC_BUCK5_CTRL4_b5_adj_clx_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_min_ton : 3;
        unsigned char b5_reg_ibias : 1;
        unsigned char b5_reg_op_c : 1;
        unsigned char b5_reg_dr : 3;
    } reg;
} PMIC_BUCK5_CTRL5_UNION;
#endif
#define PMIC_BUCK5_CTRL5_b5_min_ton_START (0)
#define PMIC_BUCK5_CTRL5_b5_min_ton_END (2)
#define PMIC_BUCK5_CTRL5_b5_reg_ibias_START (3)
#define PMIC_BUCK5_CTRL5_b5_reg_ibias_END (3)
#define PMIC_BUCK5_CTRL5_b5_reg_op_c_START (4)
#define PMIC_BUCK5_CTRL5_b5_reg_op_c_END (4)
#define PMIC_BUCK5_CTRL5_b5_reg_dr_START (5)
#define PMIC_BUCK5_CTRL5_b5_reg_dr_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_dmd_ton : 3;
        unsigned char b5_dmd_shield_ton : 1;
        unsigned char b5_dmd_sel : 4;
    } reg;
} PMIC_BUCK5_CTRL6_UNION;
#endif
#define PMIC_BUCK5_CTRL6_b5_dmd_ton_START (0)
#define PMIC_BUCK5_CTRL6_b5_dmd_ton_END (2)
#define PMIC_BUCK5_CTRL6_b5_dmd_shield_ton_START (3)
#define PMIC_BUCK5_CTRL6_b5_dmd_shield_ton_END (3)
#define PMIC_BUCK5_CTRL6_b5_dmd_sel_START (4)
#define PMIC_BUCK5_CTRL6_b5_dmd_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_ocp_toff : 2;
        unsigned char b5_ocp_delay : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_BUCK5_CTRL7_UNION;
#endif
#define PMIC_BUCK5_CTRL7_b5_ocp_toff_START (0)
#define PMIC_BUCK5_CTRL7_b5_ocp_toff_END (1)
#define PMIC_BUCK5_CTRL7_b5_ocp_delay_START (2)
#define PMIC_BUCK5_CTRL7_b5_ocp_delay_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_fb_cap_sel : 1;
        unsigned char b5_nonlinear_driver : 1;
        unsigned char b5_lx_dt : 1;
        unsigned char b5_ptn_dt_sel : 2;
        unsigned char b5_ntp_dt_sel : 2;
        unsigned char b5_cot_rlx : 1;
    } reg;
} PMIC_BUCK5_CTRL8_UNION;
#endif
#define PMIC_BUCK5_CTRL8_b5_fb_cap_sel_START (0)
#define PMIC_BUCK5_CTRL8_b5_fb_cap_sel_END (0)
#define PMIC_BUCK5_CTRL8_b5_nonlinear_driver_START (1)
#define PMIC_BUCK5_CTRL8_b5_nonlinear_driver_END (1)
#define PMIC_BUCK5_CTRL8_b5_lx_dt_START (2)
#define PMIC_BUCK5_CTRL8_b5_lx_dt_END (2)
#define PMIC_BUCK5_CTRL8_b5_ptn_dt_sel_START (3)
#define PMIC_BUCK5_CTRL8_b5_ptn_dt_sel_END (4)
#define PMIC_BUCK5_CTRL8_b5_ntp_dt_sel_START (5)
#define PMIC_BUCK5_CTRL8_b5_ntp_dt_sel_END (6)
#define PMIC_BUCK5_CTRL8_b5_cot_rlx_START (7)
#define PMIC_BUCK5_CTRL8_b5_cot_rlx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_dmd_sel_eco : 4;
        unsigned char b5_ocp_cmp_ibias : 1;
        unsigned char b5_ocpibias_sel : 2;
        unsigned char b5_ramp_ton_ctrl : 1;
    } reg;
} PMIC_BUCK5_CTRL9_UNION;
#endif
#define PMIC_BUCK5_CTRL9_b5_dmd_sel_eco_START (0)
#define PMIC_BUCK5_CTRL9_b5_dmd_sel_eco_END (3)
#define PMIC_BUCK5_CTRL9_b5_ocp_cmp_ibias_START (4)
#define PMIC_BUCK5_CTRL9_b5_ocp_cmp_ibias_END (4)
#define PMIC_BUCK5_CTRL9_b5_ocpibias_sel_START (5)
#define PMIC_BUCK5_CTRL9_b5_ocpibias_sel_END (6)
#define PMIC_BUCK5_CTRL9_b5_ramp_ton_ctrl_START (7)
#define PMIC_BUCK5_CTRL9_b5_ramp_ton_ctrl_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_offres_sel : 1;
        unsigned char b5_offres_shut : 1;
        unsigned char b5_ramp_prebias : 1;
        unsigned char b5_softime_sel : 2;
        unsigned char b5_fix_ton : 1;
        unsigned char b5_dmd_blanktime_sel : 1;
        unsigned char b5_dmd_type_sel : 1;
    } reg;
} PMIC_BUCK5_CTRL10_UNION;
#endif
#define PMIC_BUCK5_CTRL10_b5_offres_sel_START (0)
#define PMIC_BUCK5_CTRL10_b5_offres_sel_END (0)
#define PMIC_BUCK5_CTRL10_b5_offres_shut_START (1)
#define PMIC_BUCK5_CTRL10_b5_offres_shut_END (1)
#define PMIC_BUCK5_CTRL10_b5_ramp_prebias_START (2)
#define PMIC_BUCK5_CTRL10_b5_ramp_prebias_END (2)
#define PMIC_BUCK5_CTRL10_b5_softime_sel_START (3)
#define PMIC_BUCK5_CTRL10_b5_softime_sel_END (4)
#define PMIC_BUCK5_CTRL10_b5_fix_ton_START (5)
#define PMIC_BUCK5_CTRL10_b5_fix_ton_END (5)
#define PMIC_BUCK5_CTRL10_b5_dmd_blanktime_sel_START (6)
#define PMIC_BUCK5_CTRL10_b5_dmd_blanktime_sel_END (6)
#define PMIC_BUCK5_CTRL10_b5_dmd_type_sel_START (7)
#define PMIC_BUCK5_CTRL10_b5_dmd_type_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_ea_ibias_sel : 1;
        unsigned char b5_ibias_dmd_ctrl : 2;
        unsigned char b5_eco_ibias_sel : 1;
        unsigned char b5_ea_clamp_eco_dis : 1;
        unsigned char b5_ea_eco_dis : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK5_CTRL11_UNION;
#endif
#define PMIC_BUCK5_CTRL11_b5_ea_ibias_sel_START (0)
#define PMIC_BUCK5_CTRL11_b5_ea_ibias_sel_END (0)
#define PMIC_BUCK5_CTRL11_b5_ibias_dmd_ctrl_START (1)
#define PMIC_BUCK5_CTRL11_b5_ibias_dmd_ctrl_END (2)
#define PMIC_BUCK5_CTRL11_b5_eco_ibias_sel_START (3)
#define PMIC_BUCK5_CTRL11_b5_eco_ibias_sel_END (3)
#define PMIC_BUCK5_CTRL11_b5_ea_clamp_eco_dis_START (4)
#define PMIC_BUCK5_CTRL11_b5_ea_clamp_eco_dis_END (4)
#define PMIC_BUCK5_CTRL11_b5_ea_eco_dis_START (5)
#define PMIC_BUCK5_CTRL11_b5_ea_eco_dis_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_fastramp_ctrl : 4;
        unsigned char b5_rampdown_ndmd : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK5_CTRL12_UNION;
#endif
#define PMIC_BUCK5_CTRL12_b5_fastramp_ctrl_START (0)
#define PMIC_BUCK5_CTRL12_b5_fastramp_ctrl_END (3)
#define PMIC_BUCK5_CTRL12_b5_rampdown_ndmd_START (4)
#define PMIC_BUCK5_CTRL12_b5_rampdown_ndmd_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_reserve0 : 8;
    } reg;
} PMIC_BUCK5_CTRL13_UNION;
#endif
#define PMIC_BUCK5_CTRL13_b5_reserve0_START (0)
#define PMIC_BUCK5_CTRL13_b5_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b5_reserve1 : 8;
    } reg;
} PMIC_BUCK5_CTRL14_UNION;
#endif
#define PMIC_BUCK5_CTRL14_b5_reserve1_START (0)
#define PMIC_BUCK5_CTRL14_b5_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_clock_sel : 7;
        unsigned char b70_clock_en_4phase : 1;
    } reg;
} PMIC_BUCK70_CTRL0_UNION;
#endif
#define PMIC_BUCK70_CTRL0_b70_clock_sel_START (0)
#define PMIC_BUCK70_CTRL0_b70_clock_sel_END (6)
#define PMIC_BUCK70_CTRL0_b70_clock_en_4phase_START (7)
#define PMIC_BUCK70_CTRL0_b70_clock_en_4phase_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_clock_atest : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_BUCK70_CTRL1_UNION;
#endif
#define PMIC_BUCK70_CTRL1_b70_clock_atest_START (0)
#define PMIC_BUCK70_CTRL1_b70_clock_atest_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_crtshare_clamp_sel : 4;
        unsigned char b70_crtshare_filter_r_sel : 1;
        unsigned char b70_crtshare_bw_sel : 2;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK70_CTRL2_UNION;
#endif
#define PMIC_BUCK70_CTRL2_b70_crtshare_clamp_sel_START (0)
#define PMIC_BUCK70_CTRL2_b70_crtshare_clamp_sel_END (3)
#define PMIC_BUCK70_CTRL2_b70_crtshare_filter_r_sel_START (4)
#define PMIC_BUCK70_CTRL2_b70_crtshare_filter_r_sel_END (4)
#define PMIC_BUCK70_CTRL2_b70_crtshare_bw_sel_START (5)
#define PMIC_BUCK70_CTRL2_b70_crtshare_bw_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_crtshare_atest_sel : 1;
        unsigned char b70_crtshare_trim : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_BUCK70_CTRL3_UNION;
#endif
#define PMIC_BUCK70_CTRL3_b70_crtshare_atest_sel_START (0)
#define PMIC_BUCK70_CTRL3_b70_crtshare_atest_sel_END (0)
#define PMIC_BUCK70_CTRL3_b70_crtshare_trim_START (1)
#define PMIC_BUCK70_CTRL3_b70_crtshare_trim_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_dmd_auto_eco : 1;
        unsigned char b70_dmd_auto_ccm : 1;
        unsigned char b70_dmd_auto_sel : 1;
        unsigned char b70_dmd_ngc_disch : 1;
        unsigned char b70_dmd_ngc_ramp : 1;
        unsigned char b70_dmd_clp_sel : 1;
        unsigned char b70_dmd_blk_sel : 1;
        unsigned char b70_dmd_eco_sel : 1;
    } reg;
} PMIC_BUCK70_CTRL4_UNION;
#endif
#define PMIC_BUCK70_CTRL4_b70_dmd_auto_eco_START (0)
#define PMIC_BUCK70_CTRL4_b70_dmd_auto_eco_END (0)
#define PMIC_BUCK70_CTRL4_b70_dmd_auto_ccm_START (1)
#define PMIC_BUCK70_CTRL4_b70_dmd_auto_ccm_END (1)
#define PMIC_BUCK70_CTRL4_b70_dmd_auto_sel_START (2)
#define PMIC_BUCK70_CTRL4_b70_dmd_auto_sel_END (2)
#define PMIC_BUCK70_CTRL4_b70_dmd_ngc_disch_START (3)
#define PMIC_BUCK70_CTRL4_b70_dmd_ngc_disch_END (3)
#define PMIC_BUCK70_CTRL4_b70_dmd_ngc_ramp_START (4)
#define PMIC_BUCK70_CTRL4_b70_dmd_ngc_ramp_END (4)
#define PMIC_BUCK70_CTRL4_b70_dmd_clp_sel_START (5)
#define PMIC_BUCK70_CTRL4_b70_dmd_clp_sel_END (5)
#define PMIC_BUCK70_CTRL4_b70_dmd_blk_sel_START (6)
#define PMIC_BUCK70_CTRL4_b70_dmd_blk_sel_END (6)
#define PMIC_BUCK70_CTRL4_b70_dmd_eco_sel_START (7)
#define PMIC_BUCK70_CTRL4_b70_dmd_eco_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_dmd_auto_vos : 3;
        unsigned char b70_dmd_auto_cmpib : 2;
        unsigned char b70_dmd_auto_sampt : 2;
        unsigned char b70_dmd_auto_vbc : 1;
    } reg;
} PMIC_BUCK70_CTRL5_UNION;
#endif
#define PMIC_BUCK70_CTRL5_b70_dmd_auto_vos_START (0)
#define PMIC_BUCK70_CTRL5_b70_dmd_auto_vos_END (2)
#define PMIC_BUCK70_CTRL5_b70_dmd_auto_cmpib_START (3)
#define PMIC_BUCK70_CTRL5_b70_dmd_auto_cmpib_END (4)
#define PMIC_BUCK70_CTRL5_b70_dmd_auto_sampt_START (5)
#define PMIC_BUCK70_CTRL5_b70_dmd_auto_sampt_END (6)
#define PMIC_BUCK70_CTRL5_b70_dmd_auto_vbc_START (7)
#define PMIC_BUCK70_CTRL5_b70_dmd_auto_vbc_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_nocp_delay_sel : 2;
        unsigned char b70_pocp_eco_sel : 1;
        unsigned char b70_pocp_delay_sel : 2;
        unsigned char b70_pocp_sel : 2;
        unsigned char b70_pocp_en : 1;
    } reg;
} PMIC_BUCK70_CTRL6_UNION;
#endif
#define PMIC_BUCK70_CTRL6_b70_nocp_delay_sel_START (0)
#define PMIC_BUCK70_CTRL6_b70_nocp_delay_sel_END (1)
#define PMIC_BUCK70_CTRL6_b70_pocp_eco_sel_START (2)
#define PMIC_BUCK70_CTRL6_b70_pocp_eco_sel_END (2)
#define PMIC_BUCK70_CTRL6_b70_pocp_delay_sel_START (3)
#define PMIC_BUCK70_CTRL6_b70_pocp_delay_sel_END (4)
#define PMIC_BUCK70_CTRL6_b70_pocp_sel_START (5)
#define PMIC_BUCK70_CTRL6_b70_pocp_sel_END (6)
#define PMIC_BUCK70_CTRL6_b70_pocp_en_START (7)
#define PMIC_BUCK70_CTRL6_b70_pocp_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_nocp_low_delay_sel : 2;
        unsigned char b70_nocp_low_sel : 2;
        unsigned char b70_nocp_low_eco_sel : 1;
        unsigned char b70_nocp_sel : 2;
        unsigned char b70_nocp_en : 1;
    } reg;
} PMIC_BUCK70_CTRL7_UNION;
#endif
#define PMIC_BUCK70_CTRL7_b70_nocp_low_delay_sel_START (0)
#define PMIC_BUCK70_CTRL7_b70_nocp_low_delay_sel_END (1)
#define PMIC_BUCK70_CTRL7_b70_nocp_low_sel_START (2)
#define PMIC_BUCK70_CTRL7_b70_nocp_low_sel_END (3)
#define PMIC_BUCK70_CTRL7_b70_nocp_low_eco_sel_START (4)
#define PMIC_BUCK70_CTRL7_b70_nocp_low_eco_sel_END (4)
#define PMIC_BUCK70_CTRL7_b70_nocp_sel_START (5)
#define PMIC_BUCK70_CTRL7_b70_nocp_sel_END (6)
#define PMIC_BUCK70_CTRL7_b70_nocp_en_START (7)
#define PMIC_BUCK70_CTRL7_b70_nocp_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_pocp_sw_delay_sel : 2;
        unsigned char b70_pocp_sw_sel : 3;
        unsigned char b70_pocp_sw_eco_cmp_sel : 1;
        unsigned char b70_pocp_sw_ccm_cmp_sel : 1;
        unsigned char b70_ocp_2us_delay_sel : 1;
    } reg;
} PMIC_BUCK70_CTRL8_UNION;
#endif
#define PMIC_BUCK70_CTRL8_b70_pocp_sw_delay_sel_START (0)
#define PMIC_BUCK70_CTRL8_b70_pocp_sw_delay_sel_END (1)
#define PMIC_BUCK70_CTRL8_b70_pocp_sw_sel_START (2)
#define PMIC_BUCK70_CTRL8_b70_pocp_sw_sel_END (4)
#define PMIC_BUCK70_CTRL8_b70_pocp_sw_eco_cmp_sel_START (5)
#define PMIC_BUCK70_CTRL8_b70_pocp_sw_eco_cmp_sel_END (5)
#define PMIC_BUCK70_CTRL8_b70_pocp_sw_ccm_cmp_sel_START (6)
#define PMIC_BUCK70_CTRL8_b70_pocp_sw_ccm_cmp_sel_END (6)
#define PMIC_BUCK70_CTRL8_b70_ocp_2us_delay_sel_START (7)
#define PMIC_BUCK70_CTRL8_b70_ocp_2us_delay_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_buffer_bias : 2;
        unsigned char b70_ramp_sel : 1;
        unsigned char b70_code_mode_sel : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK70_CTRL9_UNION;
#endif
#define PMIC_BUCK70_CTRL9_b70_buffer_bias_START (0)
#define PMIC_BUCK70_CTRL9_b70_buffer_bias_END (1)
#define PMIC_BUCK70_CTRL9_b70_ramp_sel_START (2)
#define PMIC_BUCK70_CTRL9_b70_ramp_sel_END (2)
#define PMIC_BUCK70_CTRL9_b70_code_mode_sel_START (3)
#define PMIC_BUCK70_CTRL9_b70_code_mode_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_clk9m6_pulse_sel : 2;
        unsigned char b70_vref_test_sel : 2;
        unsigned char b70_vref_vo_cap_sel : 2;
        unsigned char b70_rtr_cap_sel : 2;
    } reg;
} PMIC_BUCK70_CTRL10_UNION;
#endif
#define PMIC_BUCK70_CTRL10_b70_clk9m6_pulse_sel_START (0)
#define PMIC_BUCK70_CTRL10_b70_clk9m6_pulse_sel_END (1)
#define PMIC_BUCK70_CTRL10_b70_vref_test_sel_START (2)
#define PMIC_BUCK70_CTRL10_b70_vref_test_sel_END (3)
#define PMIC_BUCK70_CTRL10_b70_vref_vo_cap_sel_START (4)
#define PMIC_BUCK70_CTRL10_b70_vref_vo_cap_sel_END (5)
#define PMIC_BUCK70_CTRL10_b70_rtr_cap_sel_START (6)
#define PMIC_BUCK70_CTRL10_b70_rtr_cap_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ton_det : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK70_CTRL11_UNION;
#endif
#define PMIC_BUCK70_CTRL11_b70_ton_det_START (0)
#define PMIC_BUCK70_CTRL11_b70_ton_det_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_load_state_sel : 3;
        unsigned char b70_state_sw_ocp_shield : 1;
        unsigned char b70_state_sw_ocp_sel : 2;
        unsigned char b70_ocp_shield : 1;
        unsigned char b70_nmos_off : 1;
    } reg;
} PMIC_BUCK70_CTRL12_UNION;
#endif
#define PMIC_BUCK70_CTRL12_b70_load_state_sel_START (0)
#define PMIC_BUCK70_CTRL12_b70_load_state_sel_END (2)
#define PMIC_BUCK70_CTRL12_b70_state_sw_ocp_shield_START (3)
#define PMIC_BUCK70_CTRL12_b70_state_sw_ocp_shield_END (3)
#define PMIC_BUCK70_CTRL12_b70_state_sw_ocp_sel_START (4)
#define PMIC_BUCK70_CTRL12_b70_state_sw_ocp_sel_END (5)
#define PMIC_BUCK70_CTRL12_b70_ocp_shield_START (6)
#define PMIC_BUCK70_CTRL12_b70_ocp_shield_END (6)
#define PMIC_BUCK70_CTRL12_b70_nmos_off_START (7)
#define PMIC_BUCK70_CTRL12_b70_nmos_off_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_fastnmos_off : 1;
        unsigned char b70_slave_sel : 1;
        unsigned char b70_oneph_sel : 1;
        unsigned char b70_negdmdton_add : 1;
        unsigned char b70_negdmd_ton_sel : 2;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK70_CTRL13_UNION;
#endif
#define PMIC_BUCK70_CTRL13_b70_fastnmos_off_START (0)
#define PMIC_BUCK70_CTRL13_b70_fastnmos_off_END (0)
#define PMIC_BUCK70_CTRL13_b70_slave_sel_START (1)
#define PMIC_BUCK70_CTRL13_b70_slave_sel_END (1)
#define PMIC_BUCK70_CTRL13_b70_oneph_sel_START (2)
#define PMIC_BUCK70_CTRL13_b70_oneph_sel_END (2)
#define PMIC_BUCK70_CTRL13_b70_negdmdton_add_START (3)
#define PMIC_BUCK70_CTRL13_b70_negdmdton_add_END (3)
#define PMIC_BUCK70_CTRL13_b70_negdmd_ton_sel_START (4)
#define PMIC_BUCK70_CTRL13_b70_negdmd_ton_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_cot_cmphys : 1;
        unsigned char b70_cot_ibst_sel : 1;
        unsigned char b70_cot_sche_neco : 1;
        unsigned char b70_cot_sche_eco : 1;
        unsigned char b70_cot_sel : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK70_CTRL14_UNION;
#endif
#define PMIC_BUCK70_CTRL14_b70_cot_cmphys_START (0)
#define PMIC_BUCK70_CTRL14_b70_cot_cmphys_END (0)
#define PMIC_BUCK70_CTRL14_b70_cot_ibst_sel_START (1)
#define PMIC_BUCK70_CTRL14_b70_cot_ibst_sel_END (1)
#define PMIC_BUCK70_CTRL14_b70_cot_sche_neco_START (2)
#define PMIC_BUCK70_CTRL14_b70_cot_sche_neco_END (2)
#define PMIC_BUCK70_CTRL14_b70_cot_sche_eco_START (3)
#define PMIC_BUCK70_CTRL14_b70_cot_sche_eco_END (3)
#define PMIC_BUCK70_CTRL14_b70_cot_sel_START (4)
#define PMIC_BUCK70_CTRL14_b70_cot_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_c1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK70_CTRL15_UNION;
#endif
#define PMIC_BUCK70_CTRL15_b70_c1_sel_START (0)
#define PMIC_BUCK70_CTRL15_b70_c1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_c2_sel : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK70_CTRL16_UNION;
#endif
#define PMIC_BUCK70_CTRL16_b70_c2_sel_START (0)
#define PMIC_BUCK70_CTRL16_b70_c2_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_c3_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK70_CTRL17_UNION;
#endif
#define PMIC_BUCK70_CTRL17_b70_c3_sel_START (0)
#define PMIC_BUCK70_CTRL17_b70_c3_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_r1_sel : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK70_CTRL18_UNION;
#endif
#define PMIC_BUCK70_CTRL18_b70_r1_sel_START (0)
#define PMIC_BUCK70_CTRL18_b70_r1_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_r2_sel : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK70_CTRL19_UNION;
#endif
#define PMIC_BUCK70_CTRL19_b70_r2_sel_START (0)
#define PMIC_BUCK70_CTRL19_b70_r2_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_r3_sel : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK70_CTRL20_UNION;
#endif
#define PMIC_BUCK70_CTRL20_b70_r3_sel_START (0)
#define PMIC_BUCK70_CTRL20_b70_r3_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_reg_idrp : 3;
        unsigned char b70_reg_idrn : 3;
        unsigned char b70_eco_shield : 1;
        unsigned char b70_clamp_ref_sel : 1;
    } reg;
} PMIC_BUCK70_CTRL21_UNION;
#endif
#define PMIC_BUCK70_CTRL21_b70_reg_idrp_START (0)
#define PMIC_BUCK70_CTRL21_b70_reg_idrp_END (2)
#define PMIC_BUCK70_CTRL21_b70_reg_idrn_START (3)
#define PMIC_BUCK70_CTRL21_b70_reg_idrn_END (5)
#define PMIC_BUCK70_CTRL21_b70_eco_shield_START (6)
#define PMIC_BUCK70_CTRL21_b70_eco_shield_END (6)
#define PMIC_BUCK70_CTRL21_b70_clamp_ref_sel_START (7)
#define PMIC_BUCK70_CTRL21_b70_clamp_ref_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ramp_valley_sel : 1;
        unsigned char b70_eaamp_clamp : 2;
        unsigned char b70_eacomp_ecocontrol_shield : 1;
        unsigned char b70_bw_sel : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK70_CTRL22_UNION;
#endif
#define PMIC_BUCK70_CTRL22_b70_ramp_valley_sel_START (0)
#define PMIC_BUCK70_CTRL22_b70_ramp_valley_sel_END (0)
#define PMIC_BUCK70_CTRL22_b70_eaamp_clamp_START (1)
#define PMIC_BUCK70_CTRL22_b70_eaamp_clamp_END (2)
#define PMIC_BUCK70_CTRL22_b70_eacomp_ecocontrol_shield_START (3)
#define PMIC_BUCK70_CTRL22_b70_eacomp_ecocontrol_shield_END (3)
#define PMIC_BUCK70_CTRL22_b70_bw_sel_START (4)
#define PMIC_BUCK70_CTRL22_b70_bw_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_eaclamp_srgap_sel : 4;
        unsigned char b70_eaclamp_brgap_sel : 4;
    } reg;
} PMIC_BUCK70_CTRL23_UNION;
#endif
#define PMIC_BUCK70_CTRL23_b70_eaclamp_srgap_sel_START (0)
#define PMIC_BUCK70_CTRL23_b70_eaclamp_srgap_sel_END (3)
#define PMIC_BUCK70_CTRL23_b70_eaclamp_brgap_sel_START (4)
#define PMIC_BUCK70_CTRL23_b70_eaclamp_brgap_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_eaclamp_ecogap_sel : 2;
        unsigned char b70_trim2 : 1;
        unsigned char b70_trim1 : 1;
        unsigned char b70_eaclamp_gap_sel : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK70_CTRL24_UNION;
#endif
#define PMIC_BUCK70_CTRL24_b70_eaclamp_ecogap_sel_START (0)
#define PMIC_BUCK70_CTRL24_b70_eaclamp_ecogap_sel_END (1)
#define PMIC_BUCK70_CTRL24_b70_trim2_START (2)
#define PMIC_BUCK70_CTRL24_b70_trim2_END (2)
#define PMIC_BUCK70_CTRL24_b70_trim1_START (3)
#define PMIC_BUCK70_CTRL24_b70_trim1_END (3)
#define PMIC_BUCK70_CTRL24_b70_eaclamp_gap_sel_START (4)
#define PMIC_BUCK70_CTRL24_b70_eaclamp_gap_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_eabias_sel : 1;
        unsigned char b70_buck70_ea_r : 3;
        unsigned char b70_ea_ecocur_sel : 3;
        unsigned char b70_eaamp_clamp_sel : 1;
    } reg;
} PMIC_BUCK70_CTRL25_UNION;
#endif
#define PMIC_BUCK70_CTRL25_b70_eabias_sel_START (0)
#define PMIC_BUCK70_CTRL25_b70_eabias_sel_END (0)
#define PMIC_BUCK70_CTRL25_b70_buck70_ea_r_START (1)
#define PMIC_BUCK70_CTRL25_b70_buck70_ea_r_END (3)
#define PMIC_BUCK70_CTRL25_b70_ea_ecocur_sel_START (4)
#define PMIC_BUCK70_CTRL25_b70_ea_ecocur_sel_END (6)
#define PMIC_BUCK70_CTRL25_b70_eaamp_clamp_sel_START (7)
#define PMIC_BUCK70_CTRL25_b70_eaamp_clamp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_clampeco_bias_sel : 4;
        unsigned char b70_clamp_reg_idrp : 4;
    } reg;
} PMIC_BUCK70_CTRL26_UNION;
#endif
#define PMIC_BUCK70_CTRL26_b70_clampeco_bias_sel_START (0)
#define PMIC_BUCK70_CTRL26_b70_clampeco_bias_sel_END (3)
#define PMIC_BUCK70_CTRL26_b70_clamp_reg_idrp_START (4)
#define PMIC_BUCK70_CTRL26_b70_clamp_reg_idrp_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ovp_shield : 1;
        unsigned char b70_ovp_sel : 2;
        unsigned char b70_short_shield : 1;
        unsigned char b70_sft_sel : 1;
        unsigned char b70_ocp_short_en : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK70_CTRL27_UNION;
#endif
#define PMIC_BUCK70_CTRL27_b70_ovp_shield_START (0)
#define PMIC_BUCK70_CTRL27_b70_ovp_shield_END (0)
#define PMIC_BUCK70_CTRL27_b70_ovp_sel_START (1)
#define PMIC_BUCK70_CTRL27_b70_ovp_sel_END (2)
#define PMIC_BUCK70_CTRL27_b70_short_shield_START (3)
#define PMIC_BUCK70_CTRL27_b70_short_shield_END (3)
#define PMIC_BUCK70_CTRL27_b70_sft_sel_START (4)
#define PMIC_BUCK70_CTRL27_b70_sft_sel_END (4)
#define PMIC_BUCK70_CTRL27_b70_ocp_short_en_START (5)
#define PMIC_BUCK70_CTRL27_b70_ocp_short_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_dbias_eco : 3;
        unsigned char b70_dbias : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK70_CTRL28_UNION;
#endif
#define PMIC_BUCK70_CTRL28_b70_dbias_eco_START (0)
#define PMIC_BUCK70_CTRL28_b70_dbias_eco_END (2)
#define PMIC_BUCK70_CTRL28_b70_dbias_START (3)
#define PMIC_BUCK70_CTRL28_b70_dbias_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ramptrim_sel : 1;
        unsigned char b70_forcenormal : 1;
        unsigned char b70_normaltrim_shield : 1;
        unsigned char b70_ecotrim_shield : 1;
        unsigned char b70_forceeco : 1;
        unsigned char b70_eco_shield_cmp : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK70_CTRL29_UNION;
#endif
#define PMIC_BUCK70_CTRL29_b70_ramptrim_sel_START (0)
#define PMIC_BUCK70_CTRL29_b70_ramptrim_sel_END (0)
#define PMIC_BUCK70_CTRL29_b70_forcenormal_START (1)
#define PMIC_BUCK70_CTRL29_b70_forcenormal_END (1)
#define PMIC_BUCK70_CTRL29_b70_normaltrim_shield_START (2)
#define PMIC_BUCK70_CTRL29_b70_normaltrim_shield_END (2)
#define PMIC_BUCK70_CTRL29_b70_ecotrim_shield_START (3)
#define PMIC_BUCK70_CTRL29_b70_ecotrim_shield_END (3)
#define PMIC_BUCK70_CTRL29_b70_forceeco_START (4)
#define PMIC_BUCK70_CTRL29_b70_forceeco_END (4)
#define PMIC_BUCK70_CTRL29_b70_eco_shield_cmp_START (5)
#define PMIC_BUCK70_CTRL29_b70_eco_shield_cmp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_loadline_set : 5;
        unsigned char b70_loadline_fon : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK70_CTRL30_UNION;
#endif
#define PMIC_BUCK70_CTRL30_b70_loadline_set_START (0)
#define PMIC_BUCK70_CTRL30_b70_loadline_set_END (4)
#define PMIC_BUCK70_CTRL30_b70_loadline_fon_START (5)
#define PMIC_BUCK70_CTRL30_b70_loadline_fon_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_stsw_fstsch_neco : 3;
        unsigned char b70_stsw_fstsch_eco : 3;
        unsigned char b70_stsw_rpup_ccm : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK70_CTRL31_UNION;
#endif
#define PMIC_BUCK70_CTRL31_b70_stsw_fstsch_neco_START (0)
#define PMIC_BUCK70_CTRL31_b70_stsw_fstsch_neco_END (2)
#define PMIC_BUCK70_CTRL31_b70_stsw_fstsch_eco_START (3)
#define PMIC_BUCK70_CTRL31_b70_stsw_fstsch_eco_END (5)
#define PMIC_BUCK70_CTRL31_b70_stsw_rpup_ccm_START (6)
#define PMIC_BUCK70_CTRL31_b70_stsw_rpup_ccm_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_stsw_slwsch_neco : 2;
        unsigned char b70_stsw_slwsch_eco : 2;
        unsigned char b70_stsw_fstps_ith : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK70_CTRL32_UNION;
#endif
#define PMIC_BUCK70_CTRL32_b70_stsw_slwsch_neco_START (0)
#define PMIC_BUCK70_CTRL32_b70_stsw_slwsch_neco_END (1)
#define PMIC_BUCK70_CTRL32_b70_stsw_slwsch_eco_START (2)
#define PMIC_BUCK70_CTRL32_b70_stsw_slwsch_eco_END (3)
#define PMIC_BUCK70_CTRL32_b70_stsw_fstps_ith_START (4)
#define PMIC_BUCK70_CTRL32_b70_stsw_fstps_ith_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_stsw_slw_nohys : 1;
        unsigned char b70_stsw_slw_ct2cm_ith : 3;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK70_CTRL33_UNION;
#endif
#define PMIC_BUCK70_CTRL33_b70_stsw_slw_nohys_START (0)
#define PMIC_BUCK70_CTRL33_b70_stsw_slw_nohys_END (0)
#define PMIC_BUCK70_CTRL33_b70_stsw_slw_ct2cm_ith_START (1)
#define PMIC_BUCK70_CTRL33_b70_stsw_slw_ct2cm_ith_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_stsw_st_set : 2;
        unsigned char b70_stsw_slw_dn_td : 2;
        unsigned char b70_stsw_slw_up_td : 2;
        unsigned char b70_stsw_slw_ct2cm_hys : 2;
    } reg;
} PMIC_BUCK70_CTRL34_UNION;
#endif
#define PMIC_BUCK70_CTRL34_b70_stsw_st_set_START (0)
#define PMIC_BUCK70_CTRL34_b70_stsw_st_set_END (1)
#define PMIC_BUCK70_CTRL34_b70_stsw_slw_dn_td_START (2)
#define PMIC_BUCK70_CTRL34_b70_stsw_slw_dn_td_END (3)
#define PMIC_BUCK70_CTRL34_b70_stsw_slw_up_td_START (4)
#define PMIC_BUCK70_CTRL34_b70_stsw_slw_up_td_END (5)
#define PMIC_BUCK70_CTRL34_b70_stsw_slw_ct2cm_hys_START (6)
#define PMIC_BUCK70_CTRL34_b70_stsw_slw_ct2cm_hys_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ecdt_ct2cm_ft : 2;
        unsigned char b70_ecdt_ec2ct_ref : 3;
        unsigned char b70_ecdt_ct2cm_ref : 3;
    } reg;
} PMIC_BUCK70_CTRL35_UNION;
#endif
#define PMIC_BUCK70_CTRL35_b70_ecdt_ct2cm_ft_START (0)
#define PMIC_BUCK70_CTRL35_b70_ecdt_ct2cm_ft_END (1)
#define PMIC_BUCK70_CTRL35_b70_ecdt_ec2ct_ref_START (2)
#define PMIC_BUCK70_CTRL35_b70_ecdt_ec2ct_ref_END (4)
#define PMIC_BUCK70_CTRL35_b70_ecdt_ct2cm_ref_START (5)
#define PMIC_BUCK70_CTRL35_b70_ecdt_ct2cm_ref_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ecdt_fltrc : 2;
        unsigned char b70_ecdt_ct2ec_ft : 2;
        unsigned char b70_ecdt_cmphys : 1;
        unsigned char b70_ecdt_ct2ec_ref : 3;
    } reg;
} PMIC_BUCK70_CTRL36_UNION;
#endif
#define PMIC_BUCK70_CTRL36_b70_ecdt_fltrc_START (0)
#define PMIC_BUCK70_CTRL36_b70_ecdt_fltrc_END (1)
#define PMIC_BUCK70_CTRL36_b70_ecdt_ct2ec_ft_START (2)
#define PMIC_BUCK70_CTRL36_b70_ecdt_ct2ec_ft_END (3)
#define PMIC_BUCK70_CTRL36_b70_ecdt_cmphys_START (4)
#define PMIC_BUCK70_CTRL36_b70_ecdt_cmphys_END (4)
#define PMIC_BUCK70_CTRL36_b70_ecdt_ct2ec_ref_START (5)
#define PMIC_BUCK70_CTRL36_b70_ecdt_ct2ec_ref_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_min_toff_sel : 2;
        unsigned char b70_min_ton_sel : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK70_CTRL37_UNION;
#endif
#define PMIC_BUCK70_CTRL37_b70_min_toff_sel_START (0)
#define PMIC_BUCK70_CTRL37_b70_min_toff_sel_END (1)
#define PMIC_BUCK70_CTRL37_b70_min_ton_sel_START (2)
#define PMIC_BUCK70_CTRL37_b70_min_ton_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ctlogic_crson_td : 2;
        unsigned char b70_ctlogic_modsw : 1;
        unsigned char b70_ctlogic_sgph : 1;
        unsigned char b70_ctlogic_smph : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK70_CTRL38_UNION;
#endif
#define PMIC_BUCK70_CTRL38_b70_ctlogic_crson_td_START (0)
#define PMIC_BUCK70_CTRL38_b70_ctlogic_crson_td_END (1)
#define PMIC_BUCK70_CTRL38_b70_ctlogic_modsw_START (2)
#define PMIC_BUCK70_CTRL38_b70_ctlogic_modsw_END (2)
#define PMIC_BUCK70_CTRL38_b70_ctlogic_sgph_START (3)
#define PMIC_BUCK70_CTRL38_b70_ctlogic_sgph_END (3)
#define PMIC_BUCK70_CTRL38_b70_ctlogic_smph_START (4)
#define PMIC_BUCK70_CTRL38_b70_ctlogic_smph_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ramp_big_sel : 1;
        unsigned char b70_ramp_mid_sel : 1;
        unsigned char buck70_ramp_line_sel : 1;
        unsigned char buck70_ramp_ccm_outsel : 1;
        unsigned char buck70_ramp_cot_outsel : 1;
        unsigned char buck70_ramp_c : 2;
        unsigned char buck70_ramp_clk_sel : 1;
    } reg;
} PMIC_BUCK70_CTRL39_UNION;
#endif
#define PMIC_BUCK70_CTRL39_b70_ramp_big_sel_START (0)
#define PMIC_BUCK70_CTRL39_b70_ramp_big_sel_END (0)
#define PMIC_BUCK70_CTRL39_b70_ramp_mid_sel_START (1)
#define PMIC_BUCK70_CTRL39_b70_ramp_mid_sel_END (1)
#define PMIC_BUCK70_CTRL39_buck70_ramp_line_sel_START (2)
#define PMIC_BUCK70_CTRL39_buck70_ramp_line_sel_END (2)
#define PMIC_BUCK70_CTRL39_buck70_ramp_ccm_outsel_START (3)
#define PMIC_BUCK70_CTRL39_buck70_ramp_ccm_outsel_END (3)
#define PMIC_BUCK70_CTRL39_buck70_ramp_cot_outsel_START (4)
#define PMIC_BUCK70_CTRL39_buck70_ramp_cot_outsel_END (4)
#define PMIC_BUCK70_CTRL39_buck70_ramp_c_START (5)
#define PMIC_BUCK70_CTRL39_buck70_ramp_c_END (6)
#define PMIC_BUCK70_CTRL39_buck70_ramp_clk_sel_START (7)
#define PMIC_BUCK70_CTRL39_buck70_ramp_clk_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ramp_r_eco : 4;
        unsigned char b70_ramp_counter : 2;
        unsigned char b70_ramp_sw_sel : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK70_CTRL40_UNION;
#endif
#define PMIC_BUCK70_CTRL40_b70_ramp_r_eco_START (0)
#define PMIC_BUCK70_CTRL40_b70_ramp_r_eco_END (3)
#define PMIC_BUCK70_CTRL40_b70_ramp_counter_START (4)
#define PMIC_BUCK70_CTRL40_b70_ramp_counter_END (5)
#define PMIC_BUCK70_CTRL40_b70_ramp_sw_sel_START (6)
#define PMIC_BUCK70_CTRL40_b70_ramp_sw_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ramp_r_ccm : 4;
        unsigned char b70_ramp_r_cot : 4;
    } reg;
} PMIC_BUCK70_CTRL41_UNION;
#endif
#define PMIC_BUCK70_CTRL41_b70_ramp_r_ccm_START (0)
#define PMIC_BUCK70_CTRL41_b70_ramp_r_ccm_END (3)
#define PMIC_BUCK70_CTRL41_b70_ramp_r_cot_START (4)
#define PMIC_BUCK70_CTRL41_b70_ramp_r_cot_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ramp_fast_sel : 1;
        unsigned char b70_ramp_freq_sel : 1;
        unsigned char b70_ramp_res_test : 1;
        unsigned char b70_ramp_change_mode : 5;
    } reg;
} PMIC_BUCK70_CTRL42_UNION;
#endif
#define PMIC_BUCK70_CTRL42_b70_ramp_fast_sel_START (0)
#define PMIC_BUCK70_CTRL42_b70_ramp_fast_sel_END (0)
#define PMIC_BUCK70_CTRL42_b70_ramp_freq_sel_START (1)
#define PMIC_BUCK70_CTRL42_b70_ramp_freq_sel_END (1)
#define PMIC_BUCK70_CTRL42_b70_ramp_res_test_START (2)
#define PMIC_BUCK70_CTRL42_b70_ramp_res_test_END (2)
#define PMIC_BUCK70_CTRL42_b70_ramp_change_mode_START (3)
#define PMIC_BUCK70_CTRL42_b70_ramp_change_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ramp_dblclk_sel : 1;
        unsigned char b70_ramp_force_eco_test : 1;
        unsigned char b70_ramp_buff_trim_test : 1;
        unsigned char b70_ramp_buff_trim : 1;
        unsigned char b70_ramp_buff_bias : 2;
        unsigned char b70_ramp_eco_buffer : 1;
        unsigned char buck70_ramp_trim_sel : 1;
    } reg;
} PMIC_BUCK70_CTRL43_UNION;
#endif
#define PMIC_BUCK70_CTRL43_b70_ramp_dblclk_sel_START (0)
#define PMIC_BUCK70_CTRL43_b70_ramp_dblclk_sel_END (0)
#define PMIC_BUCK70_CTRL43_b70_ramp_force_eco_test_START (1)
#define PMIC_BUCK70_CTRL43_b70_ramp_force_eco_test_END (1)
#define PMIC_BUCK70_CTRL43_b70_ramp_buff_trim_test_START (2)
#define PMIC_BUCK70_CTRL43_b70_ramp_buff_trim_test_END (2)
#define PMIC_BUCK70_CTRL43_b70_ramp_buff_trim_START (3)
#define PMIC_BUCK70_CTRL43_b70_ramp_buff_trim_END (3)
#define PMIC_BUCK70_CTRL43_b70_ramp_buff_bias_START (4)
#define PMIC_BUCK70_CTRL43_b70_ramp_buff_bias_END (5)
#define PMIC_BUCK70_CTRL43_b70_ramp_eco_buffer_START (6)
#define PMIC_BUCK70_CTRL43_b70_ramp_eco_buffer_END (6)
#define PMIC_BUCK70_CTRL43_buck70_ramp_trim_sel_START (7)
#define PMIC_BUCK70_CTRL43_buck70_ramp_trim_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_n_sel : 2;
        unsigned char b70_p_sel : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK70_CTRL44_UNION;
#endif
#define PMIC_BUCK70_CTRL44_b70_n_sel_START (0)
#define PMIC_BUCK70_CTRL44_b70_n_sel_END (1)
#define PMIC_BUCK70_CTRL44_b70_p_sel_START (2)
#define PMIC_BUCK70_CTRL44_b70_p_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_buck70_dt_sel : 7;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK70_CTRL45_UNION;
#endif
#define PMIC_BUCK70_CTRL45_b70_buck70_dt_sel_START (0)
#define PMIC_BUCK70_CTRL45_b70_buck70_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ng_dt_sel : 3;
        unsigned char b70_pg_dt_sel : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK70_CTRL46_UNION;
#endif
#define PMIC_BUCK70_CTRL46_b70_ng_dt_sel_START (0)
#define PMIC_BUCK70_CTRL46_b70_ng_dt_sel_END (2)
#define PMIC_BUCK70_CTRL46_b70_pg_dt_sel_START (3)
#define PMIC_BUCK70_CTRL46_b70_pg_dt_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_pg_n_sel : 3;
        unsigned char b70_pg_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK70_CTRL47_UNION;
#endif
#define PMIC_BUCK70_CTRL47_b70_pg_n_sel_START (0)
#define PMIC_BUCK70_CTRL47_b70_pg_n_sel_END (2)
#define PMIC_BUCK70_CTRL47_b70_pg_p_sel_START (3)
#define PMIC_BUCK70_CTRL47_b70_pg_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ng_n_sel : 3;
        unsigned char b70_ng_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK70_CTRL48_UNION;
#endif
#define PMIC_BUCK70_CTRL48_b70_ng_n_sel_START (0)
#define PMIC_BUCK70_CTRL48_b70_ng_n_sel_END (2)
#define PMIC_BUCK70_CTRL48_b70_ng_p_sel_START (3)
#define PMIC_BUCK70_CTRL48_b70_ng_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_ron_test_sel : 3;
        unsigned char b70_drv_sw_sel : 1;
        unsigned char b70_drv_dt_sel : 4;
    } reg;
} PMIC_BUCK70_CTRL49_UNION;
#endif
#define PMIC_BUCK70_CTRL49_b70_ron_test_sel_START (0)
#define PMIC_BUCK70_CTRL49_b70_ron_test_sel_END (2)
#define PMIC_BUCK70_CTRL49_b70_drv_sw_sel_START (3)
#define PMIC_BUCK70_CTRL49_b70_drv_sw_sel_END (3)
#define PMIC_BUCK70_CTRL49_b70_drv_dt_sel_START (4)
#define PMIC_BUCK70_CTRL49_b70_drv_dt_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_stsw_atest : 1;
        unsigned char b70_ph_sel : 1;
        unsigned char b70_eatrim_en : 1;
        unsigned char reserved_0 : 1;
        unsigned char b70_pocp_sw_atest_sel : 1;
        unsigned char b70_npocp_atest_sel : 1;
        unsigned char b70_cmp_atest_sel : 1;
        unsigned char reserved_1 : 1;
    } reg;
} PMIC_BUCK70_CTRL50_UNION;
#endif
#define PMIC_BUCK70_CTRL50_b70_stsw_atest_START (0)
#define PMIC_BUCK70_CTRL50_b70_stsw_atest_END (0)
#define PMIC_BUCK70_CTRL50_b70_ph_sel_START (1)
#define PMIC_BUCK70_CTRL50_b70_ph_sel_END (1)
#define PMIC_BUCK70_CTRL50_b70_eatrim_en_START (2)
#define PMIC_BUCK70_CTRL50_b70_eatrim_en_END (2)
#define PMIC_BUCK70_CTRL50_b70_pocp_sw_atest_sel_START (4)
#define PMIC_BUCK70_CTRL50_b70_pocp_sw_atest_sel_END (4)
#define PMIC_BUCK70_CTRL50_b70_npocp_atest_sel_START (5)
#define PMIC_BUCK70_CTRL50_b70_npocp_atest_sel_END (5)
#define PMIC_BUCK70_CTRL50_b70_cmp_atest_sel_START (6)
#define PMIC_BUCK70_CTRL50_b70_cmp_atest_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_dmd_atest : 1;
        unsigned char b70_dmd_ngc_set : 2;
        unsigned char b70_dmd_man_set : 4;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK70_CTRL51_UNION;
#endif
#define PMIC_BUCK70_CTRL51_b70_dmd_atest_START (0)
#define PMIC_BUCK70_CTRL51_b70_dmd_atest_END (0)
#define PMIC_BUCK70_CTRL51_b70_dmd_ngc_set_START (1)
#define PMIC_BUCK70_CTRL51_b70_dmd_ngc_set_END (2)
#define PMIC_BUCK70_CTRL51_b70_dmd_man_set_START (3)
#define PMIC_BUCK70_CTRL51_b70_dmd_man_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_cot_ont_ecoa : 8;
    } reg;
} PMIC_BUCK70_CTRL52_UNION;
#endif
#define PMIC_BUCK70_CTRL52_b70_cot_ont_ecoa_START (0)
#define PMIC_BUCK70_CTRL52_b70_cot_ont_ecoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_cot_atest : 1;
        unsigned char b70_cot_ont_necob : 3;
        unsigned char b70_cot_ont_ecob : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK70_CTRL53_UNION;
#endif
#define PMIC_BUCK70_CTRL53_b70_cot_atest_START (0)
#define PMIC_BUCK70_CTRL53_b70_cot_atest_END (0)
#define PMIC_BUCK70_CTRL53_b70_cot_ont_necob_START (1)
#define PMIC_BUCK70_CTRL53_b70_cot_ont_necob_END (3)
#define PMIC_BUCK70_CTRL53_b70_cot_ont_ecob_START (4)
#define PMIC_BUCK70_CTRL53_b70_cot_ont_ecob_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b70_cot_ont_necoa : 8;
    } reg;
} PMIC_BUCK70_CTRL54_UNION;
#endif
#define PMIC_BUCK70_CTRL54_b70_cot_ont_necoa_START (0)
#define PMIC_BUCK70_CTRL54_b70_cot_ont_necoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b701clk_chopper_sel : 2;
        unsigned char b701en_chopper : 1;
        unsigned char b701en_nsense : 1;
        unsigned char b701en_psense : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK701_CTRL0_UNION;
#endif
#define PMIC_BUCK701_CTRL0_b701clk_chopper_sel_START (0)
#define PMIC_BUCK701_CTRL0_b701clk_chopper_sel_END (1)
#define PMIC_BUCK701_CTRL0_b701en_chopper_START (2)
#define PMIC_BUCK701_CTRL0_b701en_chopper_END (2)
#define PMIC_BUCK701_CTRL0_b701en_nsense_START (3)
#define PMIC_BUCK701_CTRL0_b701en_nsense_END (3)
#define PMIC_BUCK701_CTRL0_b701en_psense_START (4)
#define PMIC_BUCK701_CTRL0_b701en_psense_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b701reg_sense_res_sel : 3;
        unsigned char b701nsense_sel : 3;
        unsigned char b701sense_ratio_sel : 2;
    } reg;
} PMIC_BUCK701_CTRL1_UNION;
#endif
#define PMIC_BUCK701_CTRL1_b701reg_sense_res_sel_START (0)
#define PMIC_BUCK701_CTRL1_b701reg_sense_res_sel_END (2)
#define PMIC_BUCK701_CTRL1_b701nsense_sel_START (3)
#define PMIC_BUCK701_CTRL1_b701nsense_sel_END (5)
#define PMIC_BUCK701_CTRL1_b701sense_ratio_sel_START (6)
#define PMIC_BUCK701_CTRL1_b701sense_ratio_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b701en_vo : 1;
        unsigned char b701ibalance_gm_sel : 2;
        unsigned char b701en_test : 1;
        unsigned char b701cs_test_sel : 1;
        unsigned char b701avg_curr_filter_sel : 2;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK701_CTRL2_UNION;
#endif
#define PMIC_BUCK701_CTRL2_b701en_vo_START (0)
#define PMIC_BUCK701_CTRL2_b701en_vo_END (0)
#define PMIC_BUCK701_CTRL2_b701ibalance_gm_sel_START (1)
#define PMIC_BUCK701_CTRL2_b701ibalance_gm_sel_END (2)
#define PMIC_BUCK701_CTRL2_b701en_test_START (3)
#define PMIC_BUCK701_CTRL2_b701en_test_END (3)
#define PMIC_BUCK701_CTRL2_b701cs_test_sel_START (4)
#define PMIC_BUCK701_CTRL2_b701cs_test_sel_END (4)
#define PMIC_BUCK701_CTRL2_b701avg_curr_filter_sel_START (5)
#define PMIC_BUCK701_CTRL2_b701avg_curr_filter_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b701vo_ton_res_sel : 3;
        unsigned char b701cot_res_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK701_CTRL3_UNION;
#endif
#define PMIC_BUCK701_CTRL3_b701vo_ton_res_sel_START (0)
#define PMIC_BUCK701_CTRL3_b701vo_ton_res_sel_END (2)
#define PMIC_BUCK701_CTRL3_b701cot_res_sel_START (3)
#define PMIC_BUCK701_CTRL3_b701cot_res_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b701load_det_sel : 3;
        unsigned char b701cot_ontime_isen_hys_sel : 2;
        unsigned char b701cot_ontime_isen_bias_sel : 2;
        unsigned char b701pvdd_ton_sel : 1;
    } reg;
} PMIC_BUCK701_CTRL4_UNION;
#endif
#define PMIC_BUCK701_CTRL4_b701load_det_sel_START (0)
#define PMIC_BUCK701_CTRL4_b701load_det_sel_END (2)
#define PMIC_BUCK701_CTRL4_b701cot_ontime_isen_hys_sel_START (3)
#define PMIC_BUCK701_CTRL4_b701cot_ontime_isen_hys_sel_END (4)
#define PMIC_BUCK701_CTRL4_b701cot_ontime_isen_bias_sel_START (5)
#define PMIC_BUCK701_CTRL4_b701cot_ontime_isen_bias_sel_END (6)
#define PMIC_BUCK701_CTRL4_b701pvdd_ton_sel_START (7)
#define PMIC_BUCK701_CTRL4_b701pvdd_ton_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_n_sel : 2;
        unsigned char b71_p_sel : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK71_CTRL0_UNION;
#endif
#define PMIC_BUCK71_CTRL0_b71_n_sel_START (0)
#define PMIC_BUCK71_CTRL0_b71_n_sel_END (1)
#define PMIC_BUCK71_CTRL0_b71_p_sel_START (2)
#define PMIC_BUCK71_CTRL0_b71_p_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_buck70_dt_sel : 7;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK71_CTRL1_UNION;
#endif
#define PMIC_BUCK71_CTRL1_b71_buck70_dt_sel_START (0)
#define PMIC_BUCK71_CTRL1_b71_buck70_dt_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ng_dt_sel : 3;
        unsigned char b71_pg_dt_sel : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK71_CTRL2_UNION;
#endif
#define PMIC_BUCK71_CTRL2_b71_ng_dt_sel_START (0)
#define PMIC_BUCK71_CTRL2_b71_ng_dt_sel_END (2)
#define PMIC_BUCK71_CTRL2_b71_pg_dt_sel_START (3)
#define PMIC_BUCK71_CTRL2_b71_pg_dt_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_pg_n_sel : 3;
        unsigned char b71_pg_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK71_CTRL3_UNION;
#endif
#define PMIC_BUCK71_CTRL3_b71_pg_n_sel_START (0)
#define PMIC_BUCK71_CTRL3_b71_pg_n_sel_END (2)
#define PMIC_BUCK71_CTRL3_b71_pg_p_sel_START (3)
#define PMIC_BUCK71_CTRL3_b71_pg_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ng_n_sel : 3;
        unsigned char b71_ng_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK71_CTRL4_UNION;
#endif
#define PMIC_BUCK71_CTRL4_b71_ng_n_sel_START (0)
#define PMIC_BUCK71_CTRL4_b71_ng_n_sel_END (2)
#define PMIC_BUCK71_CTRL4_b71_ng_p_sel_START (3)
#define PMIC_BUCK71_CTRL4_b71_ng_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ron_test_sel : 3;
        unsigned char b71_drv_sw_sel : 1;
        unsigned char b71_drv_dt_sel : 4;
    } reg;
} PMIC_BUCK71_CTRL5_UNION;
#endif
#define PMIC_BUCK71_CTRL5_b71_ron_test_sel_START (0)
#define PMIC_BUCK71_CTRL5_b71_ron_test_sel_END (2)
#define PMIC_BUCK71_CTRL5_b71_drv_sw_sel_START (3)
#define PMIC_BUCK71_CTRL5_b71_drv_sw_sel_END (3)
#define PMIC_BUCK71_CTRL5_b71_drv_dt_sel_START (4)
#define PMIC_BUCK71_CTRL5_b71_drv_dt_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_stsw_atest : 1;
        unsigned char b71_ph_sel : 1;
        unsigned char b71_eatrim_en : 1;
        unsigned char reserved_0 : 1;
        unsigned char b71_pocp_sw_atest_sel : 1;
        unsigned char b71_npocp_atest_sel : 1;
        unsigned char b71_cmp_atest_sel : 1;
        unsigned char reserved_1 : 1;
    } reg;
} PMIC_BUCK71_CTRL6_UNION;
#endif
#define PMIC_BUCK71_CTRL6_b71_stsw_atest_START (0)
#define PMIC_BUCK71_CTRL6_b71_stsw_atest_END (0)
#define PMIC_BUCK71_CTRL6_b71_ph_sel_START (1)
#define PMIC_BUCK71_CTRL6_b71_ph_sel_END (1)
#define PMIC_BUCK71_CTRL6_b71_eatrim_en_START (2)
#define PMIC_BUCK71_CTRL6_b71_eatrim_en_END (2)
#define PMIC_BUCK71_CTRL6_b71_pocp_sw_atest_sel_START (4)
#define PMIC_BUCK71_CTRL6_b71_pocp_sw_atest_sel_END (4)
#define PMIC_BUCK71_CTRL6_b71_npocp_atest_sel_START (5)
#define PMIC_BUCK71_CTRL6_b71_npocp_atest_sel_END (5)
#define PMIC_BUCK71_CTRL6_b71_cmp_atest_sel_START (6)
#define PMIC_BUCK71_CTRL6_b71_cmp_atest_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_dmd_atest : 1;
        unsigned char b71_dmd_ngc_set : 2;
        unsigned char b71_dmd_man_set : 4;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK71_CTRL7_UNION;
#endif
#define PMIC_BUCK71_CTRL7_b71_dmd_atest_START (0)
#define PMIC_BUCK71_CTRL7_b71_dmd_atest_END (0)
#define PMIC_BUCK71_CTRL7_b71_dmd_ngc_set_START (1)
#define PMIC_BUCK71_CTRL7_b71_dmd_ngc_set_END (2)
#define PMIC_BUCK71_CTRL7_b71_dmd_man_set_START (3)
#define PMIC_BUCK71_CTRL7_b71_dmd_man_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_cot_ont_ecoa : 8;
    } reg;
} PMIC_BUCK71_CTRL8_UNION;
#endif
#define PMIC_BUCK71_CTRL8_b71_cot_ont_ecoa_START (0)
#define PMIC_BUCK71_CTRL8_b71_cot_ont_ecoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_cot_atest : 1;
        unsigned char b71_cot_ont_necob : 3;
        unsigned char b71_cot_ont_ecob : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK71_CTRL9_UNION;
#endif
#define PMIC_BUCK71_CTRL9_b71_cot_atest_START (0)
#define PMIC_BUCK71_CTRL9_b71_cot_atest_END (0)
#define PMIC_BUCK71_CTRL9_b71_cot_ont_necob_START (1)
#define PMIC_BUCK71_CTRL9_b71_cot_ont_necob_END (3)
#define PMIC_BUCK71_CTRL9_b71_cot_ont_ecob_START (4)
#define PMIC_BUCK71_CTRL9_b71_cot_ont_ecob_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_cot_ont_necoa : 8;
    } reg;
} PMIC_BUCK71_CTRL10_UNION;
#endif
#define PMIC_BUCK71_CTRL10_b71_cot_ont_necoa_START (0)
#define PMIC_BUCK71_CTRL10_b71_cot_ont_necoa_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_buffer_bias : 2;
        unsigned char b71_ramp_sel : 1;
        unsigned char b71_code_mode_sel : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK71_CTRL11_UNION;
#endif
#define PMIC_BUCK71_CTRL11_b71_buffer_bias_START (0)
#define PMIC_BUCK71_CTRL11_b71_buffer_bias_END (1)
#define PMIC_BUCK71_CTRL11_b71_ramp_sel_START (2)
#define PMIC_BUCK71_CTRL11_b71_ramp_sel_END (2)
#define PMIC_BUCK71_CTRL11_b71_code_mode_sel_START (3)
#define PMIC_BUCK71_CTRL11_b71_code_mode_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_clk9m6_pulse_sel : 2;
        unsigned char b71_vref_test_sel : 2;
        unsigned char b71_vref_vo_cap_sel : 2;
        unsigned char b71_rtr_cap_sel : 2;
    } reg;
} PMIC_BUCK71_CTRL12_UNION;
#endif
#define PMIC_BUCK71_CTRL12_b71_clk9m6_pulse_sel_START (0)
#define PMIC_BUCK71_CTRL12_b71_clk9m6_pulse_sel_END (1)
#define PMIC_BUCK71_CTRL12_b71_vref_test_sel_START (2)
#define PMIC_BUCK71_CTRL12_b71_vref_test_sel_END (3)
#define PMIC_BUCK71_CTRL12_b71_vref_vo_cap_sel_START (4)
#define PMIC_BUCK71_CTRL12_b71_vref_vo_cap_sel_END (5)
#define PMIC_BUCK71_CTRL12_b71_rtr_cap_sel_START (6)
#define PMIC_BUCK71_CTRL12_b71_rtr_cap_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ton_det : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK71_CTRL13_UNION;
#endif
#define PMIC_BUCK71_CTRL13_b71_ton_det_START (0)
#define PMIC_BUCK71_CTRL13_b71_ton_det_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_cot_cmphys : 1;
        unsigned char b71_cot_ibst_sel : 1;
        unsigned char b71_cot_sche_neco : 1;
        unsigned char b71_cot_sche_eco : 1;
        unsigned char b71_cot_sel : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK71_CTRL14_UNION;
#endif
#define PMIC_BUCK71_CTRL14_b71_cot_cmphys_START (0)
#define PMIC_BUCK71_CTRL14_b71_cot_cmphys_END (0)
#define PMIC_BUCK71_CTRL14_b71_cot_ibst_sel_START (1)
#define PMIC_BUCK71_CTRL14_b71_cot_ibst_sel_END (1)
#define PMIC_BUCK71_CTRL14_b71_cot_sche_neco_START (2)
#define PMIC_BUCK71_CTRL14_b71_cot_sche_neco_END (2)
#define PMIC_BUCK71_CTRL14_b71_cot_sche_eco_START (3)
#define PMIC_BUCK71_CTRL14_b71_cot_sche_eco_END (3)
#define PMIC_BUCK71_CTRL14_b71_cot_sel_START (4)
#define PMIC_BUCK71_CTRL14_b71_cot_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_c1_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK71_CTRL15_UNION;
#endif
#define PMIC_BUCK71_CTRL15_b71_c1_sel_START (0)
#define PMIC_BUCK71_CTRL15_b71_c1_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_c2_sel : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK71_CTRL16_UNION;
#endif
#define PMIC_BUCK71_CTRL16_b71_c2_sel_START (0)
#define PMIC_BUCK71_CTRL16_b71_c2_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_c3_sel : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK71_CTRL17_UNION;
#endif
#define PMIC_BUCK71_CTRL17_b71_c3_sel_START (0)
#define PMIC_BUCK71_CTRL17_b71_c3_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_r1_sel : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK71_CTRL18_UNION;
#endif
#define PMIC_BUCK71_CTRL18_b71_r1_sel_START (0)
#define PMIC_BUCK71_CTRL18_b71_r1_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_r2_sel : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK71_CTRL19_UNION;
#endif
#define PMIC_BUCK71_CTRL19_b71_r2_sel_START (0)
#define PMIC_BUCK71_CTRL19_b71_r2_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_r3_sel : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK71_CTRL20_UNION;
#endif
#define PMIC_BUCK71_CTRL20_b71_r3_sel_START (0)
#define PMIC_BUCK71_CTRL20_b71_r3_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_reg_idrp : 3;
        unsigned char b71_reg_idrn : 3;
        unsigned char b71_eco_shield : 1;
        unsigned char b71_clamp_ref_sel : 1;
    } reg;
} PMIC_BUCK71_CTRL21_UNION;
#endif
#define PMIC_BUCK71_CTRL21_b71_reg_idrp_START (0)
#define PMIC_BUCK71_CTRL21_b71_reg_idrp_END (2)
#define PMIC_BUCK71_CTRL21_b71_reg_idrn_START (3)
#define PMIC_BUCK71_CTRL21_b71_reg_idrn_END (5)
#define PMIC_BUCK71_CTRL21_b71_eco_shield_START (6)
#define PMIC_BUCK71_CTRL21_b71_eco_shield_END (6)
#define PMIC_BUCK71_CTRL21_b71_clamp_ref_sel_START (7)
#define PMIC_BUCK71_CTRL21_b71_clamp_ref_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ramp_valley_sel : 1;
        unsigned char b71_eaamp_clamp : 2;
        unsigned char b71_eacomp_ecocontrol_shield : 1;
        unsigned char b71_bw_sel : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK71_CTRL22_UNION;
#endif
#define PMIC_BUCK71_CTRL22_b71_ramp_valley_sel_START (0)
#define PMIC_BUCK71_CTRL22_b71_ramp_valley_sel_END (0)
#define PMIC_BUCK71_CTRL22_b71_eaamp_clamp_START (1)
#define PMIC_BUCK71_CTRL22_b71_eaamp_clamp_END (2)
#define PMIC_BUCK71_CTRL22_b71_eacomp_ecocontrol_shield_START (3)
#define PMIC_BUCK71_CTRL22_b71_eacomp_ecocontrol_shield_END (3)
#define PMIC_BUCK71_CTRL22_b71_bw_sel_START (4)
#define PMIC_BUCK71_CTRL22_b71_bw_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_eaclamp_srgap_sel : 4;
        unsigned char b71_eaclamp_brgap_sel : 4;
    } reg;
} PMIC_BUCK71_CTRL23_UNION;
#endif
#define PMIC_BUCK71_CTRL23_b71_eaclamp_srgap_sel_START (0)
#define PMIC_BUCK71_CTRL23_b71_eaclamp_srgap_sel_END (3)
#define PMIC_BUCK71_CTRL23_b71_eaclamp_brgap_sel_START (4)
#define PMIC_BUCK71_CTRL23_b71_eaclamp_brgap_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_eaclamp_ecogap_sel : 2;
        unsigned char b71_trim2 : 1;
        unsigned char b71_trim1 : 1;
        unsigned char b71_eaclamp_gap_sel : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK71_CTRL24_UNION;
#endif
#define PMIC_BUCK71_CTRL24_b71_eaclamp_ecogap_sel_START (0)
#define PMIC_BUCK71_CTRL24_b71_eaclamp_ecogap_sel_END (1)
#define PMIC_BUCK71_CTRL24_b71_trim2_START (2)
#define PMIC_BUCK71_CTRL24_b71_trim2_END (2)
#define PMIC_BUCK71_CTRL24_b71_trim1_START (3)
#define PMIC_BUCK71_CTRL24_b71_trim1_END (3)
#define PMIC_BUCK71_CTRL24_b71_eaclamp_gap_sel_START (4)
#define PMIC_BUCK71_CTRL24_b71_eaclamp_gap_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_eabias_sel : 1;
        unsigned char b71_buck71_ea_r : 3;
        unsigned char b71_ea_ecocur_sel : 3;
        unsigned char b71_eaamp_clamp_sel : 1;
    } reg;
} PMIC_BUCK71_CTRL25_UNION;
#endif
#define PMIC_BUCK71_CTRL25_b71_eabias_sel_START (0)
#define PMIC_BUCK71_CTRL25_b71_eabias_sel_END (0)
#define PMIC_BUCK71_CTRL25_b71_buck71_ea_r_START (1)
#define PMIC_BUCK71_CTRL25_b71_buck71_ea_r_END (3)
#define PMIC_BUCK71_CTRL25_b71_ea_ecocur_sel_START (4)
#define PMIC_BUCK71_CTRL25_b71_ea_ecocur_sel_END (6)
#define PMIC_BUCK71_CTRL25_b71_eaamp_clamp_sel_START (7)
#define PMIC_BUCK71_CTRL25_b71_eaamp_clamp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_clampeco_bias_sel : 4;
        unsigned char b71_clamp_reg_idrp : 4;
    } reg;
} PMIC_BUCK71_CTRL26_UNION;
#endif
#define PMIC_BUCK71_CTRL26_b71_clampeco_bias_sel_START (0)
#define PMIC_BUCK71_CTRL26_b71_clampeco_bias_sel_END (3)
#define PMIC_BUCK71_CTRL26_b71_clamp_reg_idrp_START (4)
#define PMIC_BUCK71_CTRL26_b71_clamp_reg_idrp_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ovp_shield : 1;
        unsigned char b71_ovp_sel : 2;
        unsigned char b71_short_shield : 1;
        unsigned char b71_sft_sel : 1;
        unsigned char b71_ocp_short_en : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK71_CTRL27_UNION;
#endif
#define PMIC_BUCK71_CTRL27_b71_ovp_shield_START (0)
#define PMIC_BUCK71_CTRL27_b71_ovp_shield_END (0)
#define PMIC_BUCK71_CTRL27_b71_ovp_sel_START (1)
#define PMIC_BUCK71_CTRL27_b71_ovp_sel_END (2)
#define PMIC_BUCK71_CTRL27_b71_short_shield_START (3)
#define PMIC_BUCK71_CTRL27_b71_short_shield_END (3)
#define PMIC_BUCK71_CTRL27_b71_sft_sel_START (4)
#define PMIC_BUCK71_CTRL27_b71_sft_sel_END (4)
#define PMIC_BUCK71_CTRL27_b71_ocp_short_en_START (5)
#define PMIC_BUCK71_CTRL27_b71_ocp_short_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_dbias_eco : 3;
        unsigned char b71_dbias : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK71_CTRL28_UNION;
#endif
#define PMIC_BUCK71_CTRL28_b71_dbias_eco_START (0)
#define PMIC_BUCK71_CTRL28_b71_dbias_eco_END (2)
#define PMIC_BUCK71_CTRL28_b71_dbias_START (3)
#define PMIC_BUCK71_CTRL28_b71_dbias_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ramptrim_sel : 1;
        unsigned char b71_forcenormal : 1;
        unsigned char b71_normaltrim_shield : 1;
        unsigned char b71_ecotrim_shield : 1;
        unsigned char b71_forceeco : 1;
        unsigned char b71_eco_shield_cmp : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK71_CTRL29_UNION;
#endif
#define PMIC_BUCK71_CTRL29_b71_ramptrim_sel_START (0)
#define PMIC_BUCK71_CTRL29_b71_ramptrim_sel_END (0)
#define PMIC_BUCK71_CTRL29_b71_forcenormal_START (1)
#define PMIC_BUCK71_CTRL29_b71_forcenormal_END (1)
#define PMIC_BUCK71_CTRL29_b71_normaltrim_shield_START (2)
#define PMIC_BUCK71_CTRL29_b71_normaltrim_shield_END (2)
#define PMIC_BUCK71_CTRL29_b71_ecotrim_shield_START (3)
#define PMIC_BUCK71_CTRL29_b71_ecotrim_shield_END (3)
#define PMIC_BUCK71_CTRL29_b71_forceeco_START (4)
#define PMIC_BUCK71_CTRL29_b71_forceeco_END (4)
#define PMIC_BUCK71_CTRL29_b71_eco_shield_cmp_START (5)
#define PMIC_BUCK71_CTRL29_b71_eco_shield_cmp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_loadline_set : 5;
        unsigned char b71_loadline_fon : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK71_CTRL30_UNION;
#endif
#define PMIC_BUCK71_CTRL30_b71_loadline_set_START (0)
#define PMIC_BUCK71_CTRL30_b71_loadline_set_END (4)
#define PMIC_BUCK71_CTRL30_b71_loadline_fon_START (5)
#define PMIC_BUCK71_CTRL30_b71_loadline_fon_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_stsw_fstsch_neco : 3;
        unsigned char b71_stsw_fstsch_eco : 3;
        unsigned char b71_stsw_rpup_ccm : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK71_CTRL31_UNION;
#endif
#define PMIC_BUCK71_CTRL31_b71_stsw_fstsch_neco_START (0)
#define PMIC_BUCK71_CTRL31_b71_stsw_fstsch_neco_END (2)
#define PMIC_BUCK71_CTRL31_b71_stsw_fstsch_eco_START (3)
#define PMIC_BUCK71_CTRL31_b71_stsw_fstsch_eco_END (5)
#define PMIC_BUCK71_CTRL31_b71_stsw_rpup_ccm_START (6)
#define PMIC_BUCK71_CTRL31_b71_stsw_rpup_ccm_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_stsw_slwsch_neco : 2;
        unsigned char b71_stsw_slwsch_eco : 2;
        unsigned char b71_stsw_fstps_ith : 3;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK71_CTRL32_UNION;
#endif
#define PMIC_BUCK71_CTRL32_b71_stsw_slwsch_neco_START (0)
#define PMIC_BUCK71_CTRL32_b71_stsw_slwsch_neco_END (1)
#define PMIC_BUCK71_CTRL32_b71_stsw_slwsch_eco_START (2)
#define PMIC_BUCK71_CTRL32_b71_stsw_slwsch_eco_END (3)
#define PMIC_BUCK71_CTRL32_b71_stsw_fstps_ith_START (4)
#define PMIC_BUCK71_CTRL32_b71_stsw_fstps_ith_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_stsw_slw_nohys : 1;
        unsigned char b71_stsw_slw_ct2cm_ith : 3;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK71_CTRL33_UNION;
#endif
#define PMIC_BUCK71_CTRL33_b71_stsw_slw_nohys_START (0)
#define PMIC_BUCK71_CTRL33_b71_stsw_slw_nohys_END (0)
#define PMIC_BUCK71_CTRL33_b71_stsw_slw_ct2cm_ith_START (1)
#define PMIC_BUCK71_CTRL33_b71_stsw_slw_ct2cm_ith_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_stsw_st_set : 2;
        unsigned char b71_stsw_slw_dn_td : 2;
        unsigned char b71_stsw_slw_up_td : 2;
        unsigned char b71_stsw_slw_ct2cm_hys : 2;
    } reg;
} PMIC_BUCK71_CTRL34_UNION;
#endif
#define PMIC_BUCK71_CTRL34_b71_stsw_st_set_START (0)
#define PMIC_BUCK71_CTRL34_b71_stsw_st_set_END (1)
#define PMIC_BUCK71_CTRL34_b71_stsw_slw_dn_td_START (2)
#define PMIC_BUCK71_CTRL34_b71_stsw_slw_dn_td_END (3)
#define PMIC_BUCK71_CTRL34_b71_stsw_slw_up_td_START (4)
#define PMIC_BUCK71_CTRL34_b71_stsw_slw_up_td_END (5)
#define PMIC_BUCK71_CTRL34_b71_stsw_slw_ct2cm_hys_START (6)
#define PMIC_BUCK71_CTRL34_b71_stsw_slw_ct2cm_hys_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ecdt_ct2cm_ft : 2;
        unsigned char b71_ecdt_ec2ct_ref : 3;
        unsigned char b71_ecdt_ct2cm_ref : 3;
    } reg;
} PMIC_BUCK71_CTRL35_UNION;
#endif
#define PMIC_BUCK71_CTRL35_b71_ecdt_ct2cm_ft_START (0)
#define PMIC_BUCK71_CTRL35_b71_ecdt_ct2cm_ft_END (1)
#define PMIC_BUCK71_CTRL35_b71_ecdt_ec2ct_ref_START (2)
#define PMIC_BUCK71_CTRL35_b71_ecdt_ec2ct_ref_END (4)
#define PMIC_BUCK71_CTRL35_b71_ecdt_ct2cm_ref_START (5)
#define PMIC_BUCK71_CTRL35_b71_ecdt_ct2cm_ref_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ecdt_fltrc : 2;
        unsigned char b71_ecdt_ct2ec_ft : 2;
        unsigned char b71_ecdt_cmphys : 1;
        unsigned char b71_ecdt_ct2ec_ref : 3;
    } reg;
} PMIC_BUCK71_CTRL36_UNION;
#endif
#define PMIC_BUCK71_CTRL36_b71_ecdt_fltrc_START (0)
#define PMIC_BUCK71_CTRL36_b71_ecdt_fltrc_END (1)
#define PMIC_BUCK71_CTRL36_b71_ecdt_ct2ec_ft_START (2)
#define PMIC_BUCK71_CTRL36_b71_ecdt_ct2ec_ft_END (3)
#define PMIC_BUCK71_CTRL36_b71_ecdt_cmphys_START (4)
#define PMIC_BUCK71_CTRL36_b71_ecdt_cmphys_END (4)
#define PMIC_BUCK71_CTRL36_b71_ecdt_ct2ec_ref_START (5)
#define PMIC_BUCK71_CTRL36_b71_ecdt_ct2ec_ref_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_min_toff_sel : 2;
        unsigned char b71_min_ton_sel : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK71_CTRL37_UNION;
#endif
#define PMIC_BUCK71_CTRL37_b71_min_toff_sel_START (0)
#define PMIC_BUCK71_CTRL37_b71_min_toff_sel_END (1)
#define PMIC_BUCK71_CTRL37_b71_min_ton_sel_START (2)
#define PMIC_BUCK71_CTRL37_b71_min_ton_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ctlogic_crson_td : 2;
        unsigned char b71_ctlogic_modsw : 1;
        unsigned char b71_ctlogic_sgph : 1;
        unsigned char b71_ctlogic_smph : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK71_CTRL38_UNION;
#endif
#define PMIC_BUCK71_CTRL38_b71_ctlogic_crson_td_START (0)
#define PMIC_BUCK71_CTRL38_b71_ctlogic_crson_td_END (1)
#define PMIC_BUCK71_CTRL38_b71_ctlogic_modsw_START (2)
#define PMIC_BUCK71_CTRL38_b71_ctlogic_modsw_END (2)
#define PMIC_BUCK71_CTRL38_b71_ctlogic_sgph_START (3)
#define PMIC_BUCK71_CTRL38_b71_ctlogic_sgph_END (3)
#define PMIC_BUCK71_CTRL38_b71_ctlogic_smph_START (4)
#define PMIC_BUCK71_CTRL38_b71_ctlogic_smph_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ramp_big_sel : 1;
        unsigned char b71_ramp_mid_sel : 1;
        unsigned char buck71_ramp_line_sel : 1;
        unsigned char buck71_ramp_ccm_outsel : 1;
        unsigned char buck71_ramp_cot_outsel : 1;
        unsigned char buck71_ramp_c : 2;
        unsigned char buck71_ramp_clk_sel : 1;
    } reg;
} PMIC_BUCK71_CTRL39_UNION;
#endif
#define PMIC_BUCK71_CTRL39_b71_ramp_big_sel_START (0)
#define PMIC_BUCK71_CTRL39_b71_ramp_big_sel_END (0)
#define PMIC_BUCK71_CTRL39_b71_ramp_mid_sel_START (1)
#define PMIC_BUCK71_CTRL39_b71_ramp_mid_sel_END (1)
#define PMIC_BUCK71_CTRL39_buck71_ramp_line_sel_START (2)
#define PMIC_BUCK71_CTRL39_buck71_ramp_line_sel_END (2)
#define PMIC_BUCK71_CTRL39_buck71_ramp_ccm_outsel_START (3)
#define PMIC_BUCK71_CTRL39_buck71_ramp_ccm_outsel_END (3)
#define PMIC_BUCK71_CTRL39_buck71_ramp_cot_outsel_START (4)
#define PMIC_BUCK71_CTRL39_buck71_ramp_cot_outsel_END (4)
#define PMIC_BUCK71_CTRL39_buck71_ramp_c_START (5)
#define PMIC_BUCK71_CTRL39_buck71_ramp_c_END (6)
#define PMIC_BUCK71_CTRL39_buck71_ramp_clk_sel_START (7)
#define PMIC_BUCK71_CTRL39_buck71_ramp_clk_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ramp_r_eco : 4;
        unsigned char b71_ramp_counter : 2;
        unsigned char b71_ramp_sw_sel : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK71_CTRL40_UNION;
#endif
#define PMIC_BUCK71_CTRL40_b71_ramp_r_eco_START (0)
#define PMIC_BUCK71_CTRL40_b71_ramp_r_eco_END (3)
#define PMIC_BUCK71_CTRL40_b71_ramp_counter_START (4)
#define PMIC_BUCK71_CTRL40_b71_ramp_counter_END (5)
#define PMIC_BUCK71_CTRL40_b71_ramp_sw_sel_START (6)
#define PMIC_BUCK71_CTRL40_b71_ramp_sw_sel_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ramp_r_ccm : 4;
        unsigned char b71_ramp_r_cot : 4;
    } reg;
} PMIC_BUCK71_CTRL41_UNION;
#endif
#define PMIC_BUCK71_CTRL41_b71_ramp_r_ccm_START (0)
#define PMIC_BUCK71_CTRL41_b71_ramp_r_ccm_END (3)
#define PMIC_BUCK71_CTRL41_b71_ramp_r_cot_START (4)
#define PMIC_BUCK71_CTRL41_b71_ramp_r_cot_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ramp_fast_sel : 1;
        unsigned char b71_ramp_freq_sel : 1;
        unsigned char b71_ramp_res_test : 1;
        unsigned char b71_ramp_change_mode : 5;
    } reg;
} PMIC_BUCK71_CTRL42_UNION;
#endif
#define PMIC_BUCK71_CTRL42_b71_ramp_fast_sel_START (0)
#define PMIC_BUCK71_CTRL42_b71_ramp_fast_sel_END (0)
#define PMIC_BUCK71_CTRL42_b71_ramp_freq_sel_START (1)
#define PMIC_BUCK71_CTRL42_b71_ramp_freq_sel_END (1)
#define PMIC_BUCK71_CTRL42_b71_ramp_res_test_START (2)
#define PMIC_BUCK71_CTRL42_b71_ramp_res_test_END (2)
#define PMIC_BUCK71_CTRL42_b71_ramp_change_mode_START (3)
#define PMIC_BUCK71_CTRL42_b71_ramp_change_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_ramp_dblclk_sel : 1;
        unsigned char b71_ramp_force_eco_test : 1;
        unsigned char b71_ramp_buff_trim_test : 1;
        unsigned char b71_ramp_buff_trim : 1;
        unsigned char b71_ramp_buff_bias : 2;
        unsigned char b71_ramp_eco_buffer : 1;
        unsigned char buck71_ramp_trim_sel : 1;
    } reg;
} PMIC_BUCK71_CTRL43_UNION;
#endif
#define PMIC_BUCK71_CTRL43_b71_ramp_dblclk_sel_START (0)
#define PMIC_BUCK71_CTRL43_b71_ramp_dblclk_sel_END (0)
#define PMIC_BUCK71_CTRL43_b71_ramp_force_eco_test_START (1)
#define PMIC_BUCK71_CTRL43_b71_ramp_force_eco_test_END (1)
#define PMIC_BUCK71_CTRL43_b71_ramp_buff_trim_test_START (2)
#define PMIC_BUCK71_CTRL43_b71_ramp_buff_trim_test_END (2)
#define PMIC_BUCK71_CTRL43_b71_ramp_buff_trim_START (3)
#define PMIC_BUCK71_CTRL43_b71_ramp_buff_trim_END (3)
#define PMIC_BUCK71_CTRL43_b71_ramp_buff_bias_START (4)
#define PMIC_BUCK71_CTRL43_b71_ramp_buff_bias_END (5)
#define PMIC_BUCK71_CTRL43_b71_ramp_eco_buffer_START (6)
#define PMIC_BUCK71_CTRL43_b71_ramp_eco_buffer_END (6)
#define PMIC_BUCK71_CTRL43_buck71_ramp_trim_sel_START (7)
#define PMIC_BUCK71_CTRL43_buck71_ramp_trim_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b71_pulldn_pd_en : 1;
        unsigned char b71_pulldn_pd_sel : 1;
        unsigned char b71_pulldn_sel : 3;
        unsigned char reserved : 3;
    } reg;
} PMIC_BUCK71_CTRL44_UNION;
#endif
#define PMIC_BUCK71_CTRL44_b71_pulldn_pd_en_START (0)
#define PMIC_BUCK71_CTRL44_b71_pulldn_pd_en_END (0)
#define PMIC_BUCK71_CTRL44_b71_pulldn_pd_sel_START (1)
#define PMIC_BUCK71_CTRL44_b71_pulldn_pd_sel_END (1)
#define PMIC_BUCK71_CTRL44_b71_pulldn_sel_START (2)
#define PMIC_BUCK71_CTRL44_b71_pulldn_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck701_reserve0 : 8;
    } reg;
} PMIC_BUCK701_RESERVE0_UNION;
#endif
#define PMIC_BUCK701_RESERVE0_buck701_reserve0_START (0)
#define PMIC_BUCK701_RESERVE0_buck701_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck701_reserve1 : 8;
    } reg;
} PMIC_BUCK701_RESERVE1_UNION;
#endif
#define PMIC_BUCK701_RESERVE1_buck701_reserve1_START (0)
#define PMIC_BUCK701_RESERVE1_buck701_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck701_reserve2 : 8;
    } reg;
} PMIC_BUCK701_RESERVE2_UNION;
#endif
#define PMIC_BUCK701_RESERVE2_buck701_reserve2_START (0)
#define PMIC_BUCK701_RESERVE2_buck701_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck701_reserve3 : 8;
    } reg;
} PMIC_BUCK701_RESERVE3_UNION;
#endif
#define PMIC_BUCK701_RESERVE3_buck701_reserve3_START (0)
#define PMIC_BUCK701_RESERVE3_buck701_reserve3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_cmp_ibias : 4;
        unsigned char b9_adj_rlx : 4;
    } reg;
} PMIC_BUCK9_CTRL0_UNION;
#endif
#define PMIC_BUCK9_CTRL0_b9_cmp_ibias_START (0)
#define PMIC_BUCK9_CTRL0_b9_cmp_ibias_END (3)
#define PMIC_BUCK9_CTRL0_b9_adj_rlx_START (4)
#define PMIC_BUCK9_CTRL0_b9_adj_rlx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_ptn_dt_ctrl : 1;
        unsigned char b9_ntp_dt_ctrl : 1;
        unsigned char reserved : 2;
        unsigned char b9_dt_sel : 1;
        unsigned char b9_ocp_sel : 3;
    } reg;
} PMIC_BUCK9_CTRL1_UNION;
#endif
#define PMIC_BUCK9_CTRL1_b9_ptn_dt_ctrl_START (0)
#define PMIC_BUCK9_CTRL1_b9_ptn_dt_ctrl_END (0)
#define PMIC_BUCK9_CTRL1_b9_ntp_dt_ctrl_START (1)
#define PMIC_BUCK9_CTRL1_b9_ntp_dt_ctrl_END (1)
#define PMIC_BUCK9_CTRL1_b9_dt_sel_START (4)
#define PMIC_BUCK9_CTRL1_b9_dt_sel_END (4)
#define PMIC_BUCK9_CTRL1_b9_ocp_sel_START (5)
#define PMIC_BUCK9_CTRL1_b9_ocp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_pg_n_sel : 3;
        unsigned char b9_pg_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK9_CTRL2_UNION;
#endif
#define PMIC_BUCK9_CTRL2_b9_pg_n_sel_START (0)
#define PMIC_BUCK9_CTRL2_b9_pg_n_sel_END (2)
#define PMIC_BUCK9_CTRL2_b9_pg_p_sel_START (3)
#define PMIC_BUCK9_CTRL2_b9_pg_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_ng_n_sel : 3;
        unsigned char b9_ng_p_sel : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK9_CTRL3_UNION;
#endif
#define PMIC_BUCK9_CTRL3_b9_ng_n_sel_START (0)
#define PMIC_BUCK9_CTRL3_b9_ng_n_sel_END (2)
#define PMIC_BUCK9_CTRL3_b9_ng_p_sel_START (3)
#define PMIC_BUCK9_CTRL3_b9_ng_p_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_reg_r : 2;
        unsigned char reserved_0 : 2;
        unsigned char b9_reg_en : 1;
        unsigned char b9_adj_clx : 2;
        unsigned char reserved_1 : 1;
    } reg;
} PMIC_BUCK9_CTRL4_UNION;
#endif
#define PMIC_BUCK9_CTRL4_b9_reg_r_START (0)
#define PMIC_BUCK9_CTRL4_b9_reg_r_END (1)
#define PMIC_BUCK9_CTRL4_b9_reg_en_START (4)
#define PMIC_BUCK9_CTRL4_b9_reg_en_END (4)
#define PMIC_BUCK9_CTRL4_b9_adj_clx_START (5)
#define PMIC_BUCK9_CTRL4_b9_adj_clx_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_min_ton : 3;
        unsigned char b9_reg_ibias : 1;
        unsigned char b9_reg_op_c : 1;
        unsigned char b9_reg_dr : 3;
    } reg;
} PMIC_BUCK9_CTRL5_UNION;
#endif
#define PMIC_BUCK9_CTRL5_b9_min_ton_START (0)
#define PMIC_BUCK9_CTRL5_b9_min_ton_END (2)
#define PMIC_BUCK9_CTRL5_b9_reg_ibias_START (3)
#define PMIC_BUCK9_CTRL5_b9_reg_ibias_END (3)
#define PMIC_BUCK9_CTRL5_b9_reg_op_c_START (4)
#define PMIC_BUCK9_CTRL5_b9_reg_op_c_END (4)
#define PMIC_BUCK9_CTRL5_b9_reg_dr_START (5)
#define PMIC_BUCK9_CTRL5_b9_reg_dr_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_dmd_ton : 3;
        unsigned char b9_dmd_shield_ton : 1;
        unsigned char b9_dmd_sel : 4;
    } reg;
} PMIC_BUCK9_CTRL6_UNION;
#endif
#define PMIC_BUCK9_CTRL6_b9_dmd_ton_START (0)
#define PMIC_BUCK9_CTRL6_b9_dmd_ton_END (2)
#define PMIC_BUCK9_CTRL6_b9_dmd_shield_ton_START (3)
#define PMIC_BUCK9_CTRL6_b9_dmd_shield_ton_END (3)
#define PMIC_BUCK9_CTRL6_b9_dmd_sel_START (4)
#define PMIC_BUCK9_CTRL6_b9_dmd_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_sense_ctrl : 1;
        unsigned char b9_sense_filter : 4;
        unsigned char reserved : 2;
        unsigned char b9_sense_ibias_dmd : 1;
    } reg;
} PMIC_BUCK9_CTRL7_UNION;
#endif
#define PMIC_BUCK9_CTRL7_b9_sense_ctrl_START (0)
#define PMIC_BUCK9_CTRL7_b9_sense_ctrl_END (0)
#define PMIC_BUCK9_CTRL7_b9_sense_filter_START (1)
#define PMIC_BUCK9_CTRL7_b9_sense_filter_END (4)
#define PMIC_BUCK9_CTRL7_b9_sense_ibias_dmd_START (7)
#define PMIC_BUCK9_CTRL7_b9_sense_ibias_dmd_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_ocp_toff : 2;
        unsigned char b9_ocp_delay : 1;
        unsigned char b9_sense_sel : 1;
        unsigned char b9_sense_rev0 : 4;
    } reg;
} PMIC_BUCK9_CTRL8_UNION;
#endif
#define PMIC_BUCK9_CTRL8_b9_ocp_toff_START (0)
#define PMIC_BUCK9_CTRL8_b9_ocp_toff_END (1)
#define PMIC_BUCK9_CTRL8_b9_ocp_delay_START (2)
#define PMIC_BUCK9_CTRL8_b9_ocp_delay_END (2)
#define PMIC_BUCK9_CTRL8_b9_sense_sel_START (3)
#define PMIC_BUCK9_CTRL8_b9_sense_sel_END (3)
#define PMIC_BUCK9_CTRL8_b9_sense_rev0_START (4)
#define PMIC_BUCK9_CTRL8_b9_sense_rev0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_fb_cap_sel : 1;
        unsigned char b9_nonlinear_driver : 1;
        unsigned char b9_lx_dt : 1;
        unsigned char b9_ptn_dt_sel : 2;
        unsigned char b9_ntp_dt_sel : 2;
        unsigned char b9_cot_rlx : 1;
    } reg;
} PMIC_BUCK9_CTRL9_UNION;
#endif
#define PMIC_BUCK9_CTRL9_b9_fb_cap_sel_START (0)
#define PMIC_BUCK9_CTRL9_b9_fb_cap_sel_END (0)
#define PMIC_BUCK9_CTRL9_b9_nonlinear_driver_START (1)
#define PMIC_BUCK9_CTRL9_b9_nonlinear_driver_END (1)
#define PMIC_BUCK9_CTRL9_b9_lx_dt_START (2)
#define PMIC_BUCK9_CTRL9_b9_lx_dt_END (2)
#define PMIC_BUCK9_CTRL9_b9_ptn_dt_sel_START (3)
#define PMIC_BUCK9_CTRL9_b9_ptn_dt_sel_END (4)
#define PMIC_BUCK9_CTRL9_b9_ntp_dt_sel_START (5)
#define PMIC_BUCK9_CTRL9_b9_ntp_dt_sel_END (6)
#define PMIC_BUCK9_CTRL9_b9_cot_rlx_START (7)
#define PMIC_BUCK9_CTRL9_b9_cot_rlx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_dmd_sel_eco : 4;
        unsigned char b9_ocp_cmp_ibias : 1;
        unsigned char b9_ocpibias_sel : 2;
        unsigned char b9_ramp_ton_ctrl : 1;
    } reg;
} PMIC_BUCK9_CTRL10_UNION;
#endif
#define PMIC_BUCK9_CTRL10_b9_dmd_sel_eco_START (0)
#define PMIC_BUCK9_CTRL10_b9_dmd_sel_eco_END (3)
#define PMIC_BUCK9_CTRL10_b9_ocp_cmp_ibias_START (4)
#define PMIC_BUCK9_CTRL10_b9_ocp_cmp_ibias_END (4)
#define PMIC_BUCK9_CTRL10_b9_ocpibias_sel_START (5)
#define PMIC_BUCK9_CTRL10_b9_ocpibias_sel_END (6)
#define PMIC_BUCK9_CTRL10_b9_ramp_ton_ctrl_START (7)
#define PMIC_BUCK9_CTRL10_b9_ramp_ton_ctrl_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_offres_sel : 1;
        unsigned char b9_offres_shut : 1;
        unsigned char b9_ramp_prebias : 1;
        unsigned char b9_softime_sel : 2;
        unsigned char b9_fix_ton : 1;
        unsigned char b9_dmd_blanktime_sel : 1;
        unsigned char b9_dmd_type_sel : 1;
    } reg;
} PMIC_BUCK9_CTRL11_UNION;
#endif
#define PMIC_BUCK9_CTRL11_b9_offres_sel_START (0)
#define PMIC_BUCK9_CTRL11_b9_offres_sel_END (0)
#define PMIC_BUCK9_CTRL11_b9_offres_shut_START (1)
#define PMIC_BUCK9_CTRL11_b9_offres_shut_END (1)
#define PMIC_BUCK9_CTRL11_b9_ramp_prebias_START (2)
#define PMIC_BUCK9_CTRL11_b9_ramp_prebias_END (2)
#define PMIC_BUCK9_CTRL11_b9_softime_sel_START (3)
#define PMIC_BUCK9_CTRL11_b9_softime_sel_END (4)
#define PMIC_BUCK9_CTRL11_b9_fix_ton_START (5)
#define PMIC_BUCK9_CTRL11_b9_fix_ton_END (5)
#define PMIC_BUCK9_CTRL11_b9_dmd_blanktime_sel_START (6)
#define PMIC_BUCK9_CTRL11_b9_dmd_blanktime_sel_END (6)
#define PMIC_BUCK9_CTRL11_b9_dmd_type_sel_START (7)
#define PMIC_BUCK9_CTRL11_b9_dmd_type_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_ea_ibias_sel : 1;
        unsigned char b9_ibias_dmd_ctrl : 2;
        unsigned char b9_eco_ibias_sel : 1;
        unsigned char b9_ea_clamp_eco_dis : 1;
        unsigned char b9_ea_eco_dis : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK9_CTRL12_UNION;
#endif
#define PMIC_BUCK9_CTRL12_b9_ea_ibias_sel_START (0)
#define PMIC_BUCK9_CTRL12_b9_ea_ibias_sel_END (0)
#define PMIC_BUCK9_CTRL12_b9_ibias_dmd_ctrl_START (1)
#define PMIC_BUCK9_CTRL12_b9_ibias_dmd_ctrl_END (2)
#define PMIC_BUCK9_CTRL12_b9_eco_ibias_sel_START (3)
#define PMIC_BUCK9_CTRL12_b9_eco_ibias_sel_END (3)
#define PMIC_BUCK9_CTRL12_b9_ea_clamp_eco_dis_START (4)
#define PMIC_BUCK9_CTRL12_b9_ea_clamp_eco_dis_END (4)
#define PMIC_BUCK9_CTRL12_b9_ea_eco_dis_START (5)
#define PMIC_BUCK9_CTRL12_b9_ea_eco_dis_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_reserve0 : 8;
    } reg;
} PMIC_BUCK9_CTRL13_UNION;
#endif
#define PMIC_BUCK9_CTRL13_b9_reserve0_START (0)
#define PMIC_BUCK9_CTRL13_b9_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char b9_reserve1 : 8;
    } reg;
} PMIC_BUCK9_CTRL14_UNION;
#endif
#define PMIC_BUCK9_CTRL14_b9_reserve1_START (0)
#define PMIC_BUCK9_CTRL14_b9_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck_cmp_filter : 1;
        unsigned char buck_filter_ton : 2;
        unsigned char buck_dmd_clamp : 1;
        unsigned char buck_regop_clamp : 1;
        unsigned char buck_short_pdp : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_BUCK_CTRL0_UNION;
#endif
#define PMIC_BUCK_CTRL0_buck_cmp_filter_START (0)
#define PMIC_BUCK_CTRL0_buck_cmp_filter_END (0)
#define PMIC_BUCK_CTRL0_buck_filter_ton_START (1)
#define PMIC_BUCK_CTRL0_buck_filter_ton_END (2)
#define PMIC_BUCK_CTRL0_buck_dmd_clamp_START (3)
#define PMIC_BUCK_CTRL0_buck_dmd_clamp_END (3)
#define PMIC_BUCK_CTRL0_buck_regop_clamp_START (4)
#define PMIC_BUCK_CTRL0_buck_regop_clamp_END (4)
#define PMIC_BUCK_CTRL0_buck_short_pdp_START (5)
#define PMIC_BUCK_CTRL0_buck_short_pdp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck_reg_ss : 1;
        unsigned char buck_ocp_dis : 1;
        unsigned char buck_ocptrim_ctrl : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_BUCK_CTRL1_UNION;
#endif
#define PMIC_BUCK_CTRL1_buck_reg_ss_START (0)
#define PMIC_BUCK_CTRL1_buck_reg_ss_END (0)
#define PMIC_BUCK_CTRL1_buck_ocp_dis_START (1)
#define PMIC_BUCK_CTRL1_buck_ocp_dis_END (1)
#define PMIC_BUCK_CTRL1_buck_ocptrim_ctrl_START (2)
#define PMIC_BUCK_CTRL1_buck_ocptrim_ctrl_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck_lx_trim_ctrl : 1;
        unsigned char buck_lx_trim_sel : 4;
        unsigned char buck_ton_trim_ctrl : 1;
        unsigned char buck_ron_test : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_BUCK_CTRL2_UNION;
#endif
#define PMIC_BUCK_CTRL2_buck_lx_trim_ctrl_START (0)
#define PMIC_BUCK_CTRL2_buck_lx_trim_ctrl_END (0)
#define PMIC_BUCK_CTRL2_buck_lx_trim_sel_START (1)
#define PMIC_BUCK_CTRL2_buck_lx_trim_sel_END (4)
#define PMIC_BUCK_CTRL2_buck_ton_trim_ctrl_START (5)
#define PMIC_BUCK_CTRL2_buck_ton_trim_ctrl_END (5)
#define PMIC_BUCK_CTRL2_buck_ron_test_START (6)
#define PMIC_BUCK_CTRL2_buck_ron_test_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck_reserve0 : 8;
    } reg;
} PMIC_BUCK_RESERVE0_UNION;
#endif
#define PMIC_BUCK_RESERVE0_buck_reserve0_START (0)
#define PMIC_BUCK_RESERVE0_buck_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck_reserve1 : 8;
    } reg;
} PMIC_BUCK_RESERVE1_UNION;
#endif
#define PMIC_BUCK_RESERVE1_buck_reserve1_START (0)
#define PMIC_BUCK_RESERVE1_buck_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char buck_reserve2 : 8;
    } reg;
} PMIC_BUCK_RESERVE2_UNION;
#endif
#define PMIC_BUCK_RESERVE2_buck_reserve2_START (0)
#define PMIC_BUCK_RESERVE2_buck_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo0_vrset : 3;
        unsigned char ldo0_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO0_CTRL0_UNION;
#endif
#define PMIC_LDO0_CTRL0_ldo0_vrset_START (0)
#define PMIC_LDO0_CTRL0_ldo0_vrset_END (2)
#define PMIC_LDO0_CTRL0_ldo0_ocp_enn_START (3)
#define PMIC_LDO0_CTRL0_ldo0_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo0_ocps_en : 1;
        unsigned char ldo0_ocpd_vos_set : 1;
        unsigned char reserved : 1;
        unsigned char ldo0_vgpr_en : 1;
        unsigned char ldo0_pd_set : 1;
        unsigned char ldo0_comp : 2;
        unsigned char ldo0_bypass_ctrl_enn : 1;
    } reg;
} PMIC_LDO0_CTRL1_UNION;
#endif
#define PMIC_LDO0_CTRL1_ldo0_ocps_en_START (0)
#define PMIC_LDO0_CTRL1_ldo0_ocps_en_END (0)
#define PMIC_LDO0_CTRL1_ldo0_ocpd_vos_set_START (1)
#define PMIC_LDO0_CTRL1_ldo0_ocpd_vos_set_END (1)
#define PMIC_LDO0_CTRL1_ldo0_vgpr_en_START (3)
#define PMIC_LDO0_CTRL1_ldo0_vgpr_en_END (3)
#define PMIC_LDO0_CTRL1_ldo0_pd_set_START (4)
#define PMIC_LDO0_CTRL1_ldo0_pd_set_END (4)
#define PMIC_LDO0_CTRL1_ldo0_comp_START (5)
#define PMIC_LDO0_CTRL1_ldo0_comp_END (6)
#define PMIC_LDO0_CTRL1_ldo0_bypass_ctrl_enn_START (7)
#define PMIC_LDO0_CTRL1_ldo0_bypass_ctrl_enn_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo0_ocpd_set : 2;
        unsigned char ldo0_ocps_set : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO0_CTRL2_UNION;
#endif
#define PMIC_LDO0_CTRL2_ldo0_ocpd_set_START (0)
#define PMIC_LDO0_CTRL2_ldo0_ocpd_set_END (1)
#define PMIC_LDO0_CTRL2_ldo0_ocps_set_START (2)
#define PMIC_LDO0_CTRL2_ldo0_ocps_set_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo1_vrset : 3;
        unsigned char ldo1_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO1_CTRL0_UNION;
#endif
#define PMIC_LDO1_CTRL0_ldo1_vrset_START (0)
#define PMIC_LDO1_CTRL0_ldo1_vrset_END (2)
#define PMIC_LDO1_CTRL0_ldo1_ocp_enn_START (3)
#define PMIC_LDO1_CTRL0_ldo1_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo1_psrr2_set : 3;
        unsigned char ldo1_psrr1_set : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO1_CTRL1_UNION;
#endif
#define PMIC_LDO1_CTRL1_ldo1_psrr2_set_START (0)
#define PMIC_LDO1_CTRL1_ldo1_psrr2_set_END (2)
#define PMIC_LDO1_CTRL1_ldo1_psrr1_set_START (3)
#define PMIC_LDO1_CTRL1_ldo1_psrr1_set_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo1_ocps_en : 1;
        unsigned char ldo1_bw_en : 1;
        unsigned char ldo1_vgpr_en : 1;
        unsigned char ldo1_comp : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO1_CTRL2_UNION;
#endif
#define PMIC_LDO1_CTRL2_ldo1_ocps_en_START (0)
#define PMIC_LDO1_CTRL2_ldo1_ocps_en_END (0)
#define PMIC_LDO1_CTRL2_ldo1_bw_en_START (1)
#define PMIC_LDO1_CTRL2_ldo1_bw_en_END (1)
#define PMIC_LDO1_CTRL2_ldo1_vgpr_en_START (2)
#define PMIC_LDO1_CTRL2_ldo1_vgpr_en_END (2)
#define PMIC_LDO1_CTRL2_ldo1_comp_START (3)
#define PMIC_LDO1_CTRL2_ldo1_comp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo1_ocpd_set : 2;
        unsigned char ldo1_ocps_set : 2;
        unsigned char ldo1_ocps_bw_set : 1;
        unsigned char ldo1_ocpd_vos_set : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO1_CTRL3_UNION;
#endif
#define PMIC_LDO1_CTRL3_ldo1_ocpd_set_START (0)
#define PMIC_LDO1_CTRL3_ldo1_ocpd_set_END (1)
#define PMIC_LDO1_CTRL3_ldo1_ocps_set_START (2)
#define PMIC_LDO1_CTRL3_ldo1_ocps_set_END (3)
#define PMIC_LDO1_CTRL3_ldo1_ocps_bw_set_START (4)
#define PMIC_LDO1_CTRL3_ldo1_ocps_bw_set_END (4)
#define PMIC_LDO1_CTRL3_ldo1_ocpd_vos_set_START (5)
#define PMIC_LDO1_CTRL3_ldo1_ocpd_vos_set_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo3_vrset : 3;
        unsigned char ldo3_ocp_enn : 1;
        unsigned char ldo3_ocp_set : 2;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO3_CTRL0_UNION;
#endif
#define PMIC_LDO3_CTRL0_ldo3_vrset_START (0)
#define PMIC_LDO3_CTRL0_ldo3_vrset_END (2)
#define PMIC_LDO3_CTRL0_ldo3_ocp_enn_START (3)
#define PMIC_LDO3_CTRL0_ldo3_ocp_enn_END (3)
#define PMIC_LDO3_CTRL0_ldo3_ocp_set_START (4)
#define PMIC_LDO3_CTRL0_ldo3_ocp_set_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo3_psrr2_set : 3;
        unsigned char ldo3_psrr1_set : 3;
        unsigned char ldo3_psrr_en : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_LDO3_CTRL1_UNION;
#endif
#define PMIC_LDO3_CTRL1_ldo3_psrr2_set_START (0)
#define PMIC_LDO3_CTRL1_ldo3_psrr2_set_END (2)
#define PMIC_LDO3_CTRL1_ldo3_psrr1_set_START (3)
#define PMIC_LDO3_CTRL1_ldo3_psrr1_set_END (5)
#define PMIC_LDO3_CTRL1_ldo3_psrr_en_START (6)
#define PMIC_LDO3_CTRL1_ldo3_psrr_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo4_vrset : 3;
        unsigned char ldo4_ocp_enn : 1;
        unsigned char ldo4_ocp_set : 2;
        unsigned char ldo4_iss_en : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_LDO4_CTRL0_UNION;
#endif
#define PMIC_LDO4_CTRL0_ldo4_vrset_START (0)
#define PMIC_LDO4_CTRL0_ldo4_vrset_END (2)
#define PMIC_LDO4_CTRL0_ldo4_ocp_enn_START (3)
#define PMIC_LDO4_CTRL0_ldo4_ocp_enn_END (3)
#define PMIC_LDO4_CTRL0_ldo4_ocp_set_START (4)
#define PMIC_LDO4_CTRL0_ldo4_ocp_set_END (5)
#define PMIC_LDO4_CTRL0_ldo4_iss_en_START (6)
#define PMIC_LDO4_CTRL0_ldo4_iss_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo4_psrr2_set : 3;
        unsigned char ldo4_psrr1_set : 3;
        unsigned char ldo4_psrr_en : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_LDO4_CTRL1_UNION;
#endif
#define PMIC_LDO4_CTRL1_ldo4_psrr2_set_START (0)
#define PMIC_LDO4_CTRL1_ldo4_psrr2_set_END (2)
#define PMIC_LDO4_CTRL1_ldo4_psrr1_set_START (3)
#define PMIC_LDO4_CTRL1_ldo4_psrr1_set_END (5)
#define PMIC_LDO4_CTRL1_ldo4_psrr_en_START (6)
#define PMIC_LDO4_CTRL1_ldo4_psrr_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo6_vrset : 3;
        unsigned char ldo6_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO6_CTRL0_UNION;
#endif
#define PMIC_LDO6_CTRL0_ldo6_vrset_START (0)
#define PMIC_LDO6_CTRL0_ldo6_vrset_END (2)
#define PMIC_LDO6_CTRL0_ldo6_ocp_enn_START (3)
#define PMIC_LDO6_CTRL0_ldo6_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo6_ocps_en : 1;
        unsigned char ldo6_ocpd_vos_set : 1;
        unsigned char reserved_0 : 1;
        unsigned char ldo6_vgpr_en : 1;
        unsigned char ldo6_pd_set : 1;
        unsigned char reserved_1 : 2;
        unsigned char ldo6_bypass_ctrl_enn : 1;
    } reg;
} PMIC_LDO6_CTRL1_UNION;
#endif
#define PMIC_LDO6_CTRL1_ldo6_ocps_en_START (0)
#define PMIC_LDO6_CTRL1_ldo6_ocps_en_END (0)
#define PMIC_LDO6_CTRL1_ldo6_ocpd_vos_set_START (1)
#define PMIC_LDO6_CTRL1_ldo6_ocpd_vos_set_END (1)
#define PMIC_LDO6_CTRL1_ldo6_vgpr_en_START (3)
#define PMIC_LDO6_CTRL1_ldo6_vgpr_en_END (3)
#define PMIC_LDO6_CTRL1_ldo6_pd_set_START (4)
#define PMIC_LDO6_CTRL1_ldo6_pd_set_END (4)
#define PMIC_LDO6_CTRL1_ldo6_bypass_ctrl_enn_START (7)
#define PMIC_LDO6_CTRL1_ldo6_bypass_ctrl_enn_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo6_ocpd_set : 2;
        unsigned char ldo6_ocps_set : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO6_CTRL2_UNION;
#endif
#define PMIC_LDO6_CTRL2_ldo6_ocpd_set_START (0)
#define PMIC_LDO6_CTRL2_ldo6_ocpd_set_END (1)
#define PMIC_LDO6_CTRL2_ldo6_ocps_set_START (2)
#define PMIC_LDO6_CTRL2_ldo6_ocps_set_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo8_vrset : 3;
        unsigned char ldo8_ocp_enn : 1;
        unsigned char ldo8_eco_set : 1;
        unsigned char ldo8_ocp_set : 1;
        unsigned char ldo8_esr_set : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_LDO8_CTRL_UNION;
#endif
#define PMIC_LDO8_CTRL_ldo8_vrset_START (0)
#define PMIC_LDO8_CTRL_ldo8_vrset_END (2)
#define PMIC_LDO8_CTRL_ldo8_ocp_enn_START (3)
#define PMIC_LDO8_CTRL_ldo8_ocp_enn_END (3)
#define PMIC_LDO8_CTRL_ldo8_eco_set_START (4)
#define PMIC_LDO8_CTRL_ldo8_eco_set_END (4)
#define PMIC_LDO8_CTRL_ldo8_ocp_set_START (5)
#define PMIC_LDO8_CTRL_ldo8_ocp_set_END (5)
#define PMIC_LDO8_CTRL_ldo8_esr_set_START (6)
#define PMIC_LDO8_CTRL_ldo8_esr_set_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo11_vrset : 2;
        unsigned char ldo11_ocp_enn : 1;
        unsigned char ldo11_eco_config : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO11_CTRL_UNION;
#endif
#define PMIC_LDO11_CTRL_ldo11_vrset_START (0)
#define PMIC_LDO11_CTRL_ldo11_vrset_END (1)
#define PMIC_LDO11_CTRL_ldo11_ocp_enn_START (2)
#define PMIC_LDO11_CTRL_ldo11_ocp_enn_END (2)
#define PMIC_LDO11_CTRL_ldo11_eco_config_START (3)
#define PMIC_LDO11_CTRL_ldo11_eco_config_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo12_vrset : 2;
        unsigned char ldo12_ocp_enn : 1;
        unsigned char ldo12_eco_config : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO12_CTRL_UNION;
#endif
#define PMIC_LDO12_CTRL_ldo12_vrset_START (0)
#define PMIC_LDO12_CTRL_ldo12_vrset_END (1)
#define PMIC_LDO12_CTRL_ldo12_ocp_enn_START (2)
#define PMIC_LDO12_CTRL_ldo12_ocp_enn_END (2)
#define PMIC_LDO12_CTRL_ldo12_eco_config_START (3)
#define PMIC_LDO12_CTRL_ldo12_eco_config_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo14_vrset : 2;
        unsigned char ldo14_ocp_enn : 1;
        unsigned char ldo14_eco_config : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO14_CTRL_UNION;
#endif
#define PMIC_LDO14_CTRL_ldo14_vrset_START (0)
#define PMIC_LDO14_CTRL_ldo14_vrset_END (1)
#define PMIC_LDO14_CTRL_ldo14_ocp_enn_START (2)
#define PMIC_LDO14_CTRL_ldo14_ocp_enn_END (2)
#define PMIC_LDO14_CTRL_ldo14_eco_config_START (3)
#define PMIC_LDO14_CTRL_ldo14_eco_config_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo15_vrset : 2;
        unsigned char ldo15_ocp_enn : 1;
        unsigned char ldo15_ocp_set : 2;
        unsigned char ldo15_eco_config : 1;
        unsigned char ldo15_eco_set_enn : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_LDO15_CTRL_UNION;
#endif
#define PMIC_LDO15_CTRL_ldo15_vrset_START (0)
#define PMIC_LDO15_CTRL_ldo15_vrset_END (1)
#define PMIC_LDO15_CTRL_ldo15_ocp_enn_START (2)
#define PMIC_LDO15_CTRL_ldo15_ocp_enn_END (2)
#define PMIC_LDO15_CTRL_ldo15_ocp_set_START (3)
#define PMIC_LDO15_CTRL_ldo15_ocp_set_END (4)
#define PMIC_LDO15_CTRL_ldo15_eco_config_START (5)
#define PMIC_LDO15_CTRL_ldo15_eco_config_END (5)
#define PMIC_LDO15_CTRL_ldo15_eco_set_enn_START (6)
#define PMIC_LDO15_CTRL_ldo15_eco_set_enn_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo16_vrset : 2;
        unsigned char ldo16_ocp_enn : 1;
        unsigned char ldo16_ocp_set : 2;
        unsigned char ldo16_eco_config : 1;
        unsigned char ldo16_eco_set_enn : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_LDO16_CTRL_UNION;
#endif
#define PMIC_LDO16_CTRL_ldo16_vrset_START (0)
#define PMIC_LDO16_CTRL_ldo16_vrset_END (1)
#define PMIC_LDO16_CTRL_ldo16_ocp_enn_START (2)
#define PMIC_LDO16_CTRL_ldo16_ocp_enn_END (2)
#define PMIC_LDO16_CTRL_ldo16_ocp_set_START (3)
#define PMIC_LDO16_CTRL_ldo16_ocp_set_END (4)
#define PMIC_LDO16_CTRL_ldo16_eco_config_START (5)
#define PMIC_LDO16_CTRL_ldo16_eco_config_END (5)
#define PMIC_LDO16_CTRL_ldo16_eco_set_enn_START (6)
#define PMIC_LDO16_CTRL_ldo16_eco_set_enn_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo17_vrset : 2;
        unsigned char ldo17_ocp_enn : 1;
        unsigned char ldo17_iss_en : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO17_CTRL_UNION;
#endif
#define PMIC_LDO17_CTRL_ldo17_vrset_START (0)
#define PMIC_LDO17_CTRL_ldo17_vrset_END (1)
#define PMIC_LDO17_CTRL_ldo17_ocp_enn_START (2)
#define PMIC_LDO17_CTRL_ldo17_ocp_enn_END (2)
#define PMIC_LDO17_CTRL_ldo17_iss_en_START (3)
#define PMIC_LDO17_CTRL_ldo17_iss_en_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo18_vrset : 3;
        unsigned char ldo18_ocp_set : 2;
        unsigned char ldo18_stb_set : 2;
        unsigned char ldo18_eco_set : 1;
    } reg;
} PMIC_LDO18_CTRL_UNION;
#endif
#define PMIC_LDO18_CTRL_ldo18_vrset_START (0)
#define PMIC_LDO18_CTRL_ldo18_vrset_END (2)
#define PMIC_LDO18_CTRL_ldo18_ocp_set_START (3)
#define PMIC_LDO18_CTRL_ldo18_ocp_set_END (4)
#define PMIC_LDO18_CTRL_ldo18_stb_set_START (5)
#define PMIC_LDO18_CTRL_ldo18_stb_set_END (6)
#define PMIC_LDO18_CTRL_ldo18_eco_set_START (7)
#define PMIC_LDO18_CTRL_ldo18_eco_set_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo21_vrset : 3;
        unsigned char ldo21_ocp_enn : 1;
        unsigned char ldo21_ocp_set : 2;
        unsigned char ldo21_iss_en : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_LDO21_CTRL0_UNION;
#endif
#define PMIC_LDO21_CTRL0_ldo21_vrset_START (0)
#define PMIC_LDO21_CTRL0_ldo21_vrset_END (2)
#define PMIC_LDO21_CTRL0_ldo21_ocp_enn_START (3)
#define PMIC_LDO21_CTRL0_ldo21_ocp_enn_END (3)
#define PMIC_LDO21_CTRL0_ldo21_ocp_set_START (4)
#define PMIC_LDO21_CTRL0_ldo21_ocp_set_END (5)
#define PMIC_LDO21_CTRL0_ldo21_iss_en_START (6)
#define PMIC_LDO21_CTRL0_ldo21_iss_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo21_psrr2_set : 3;
        unsigned char ldo21_psrr1_set : 3;
        unsigned char ldo21_psrr_en : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_LDO21_CTRL1_UNION;
#endif
#define PMIC_LDO21_CTRL1_ldo21_psrr2_set_START (0)
#define PMIC_LDO21_CTRL1_ldo21_psrr2_set_END (2)
#define PMIC_LDO21_CTRL1_ldo21_psrr1_set_START (3)
#define PMIC_LDO21_CTRL1_ldo21_psrr1_set_END (5)
#define PMIC_LDO21_CTRL1_ldo21_psrr_en_START (6)
#define PMIC_LDO21_CTRL1_ldo21_psrr_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo22_vrset : 3;
        unsigned char ldo22_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO22_CTRL0_UNION;
#endif
#define PMIC_LDO22_CTRL0_ldo22_vrset_START (0)
#define PMIC_LDO22_CTRL0_ldo22_vrset_END (2)
#define PMIC_LDO22_CTRL0_ldo22_ocp_enn_START (3)
#define PMIC_LDO22_CTRL0_ldo22_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo22_psrr2_set : 3;
        unsigned char ldo22_psrr1_set : 3;
        unsigned char ldo22_psrr_en : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_LDO22_CTRL1_UNION;
#endif
#define PMIC_LDO22_CTRL1_ldo22_psrr2_set_START (0)
#define PMIC_LDO22_CTRL1_ldo22_psrr2_set_END (2)
#define PMIC_LDO22_CTRL1_ldo22_psrr1_set_START (3)
#define PMIC_LDO22_CTRL1_ldo22_psrr1_set_END (5)
#define PMIC_LDO22_CTRL1_ldo22_psrr_en_START (6)
#define PMIC_LDO22_CTRL1_ldo22_psrr_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo22_ocps_en : 1;
        unsigned char ldo22_bw_en : 1;
        unsigned char ldo22_vgpr_en : 1;
        unsigned char ldo22_comp : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO22_CTRL2_UNION;
#endif
#define PMIC_LDO22_CTRL2_ldo22_ocps_en_START (0)
#define PMIC_LDO22_CTRL2_ldo22_ocps_en_END (0)
#define PMIC_LDO22_CTRL2_ldo22_bw_en_START (1)
#define PMIC_LDO22_CTRL2_ldo22_bw_en_END (1)
#define PMIC_LDO22_CTRL2_ldo22_vgpr_en_START (2)
#define PMIC_LDO22_CTRL2_ldo22_vgpr_en_END (2)
#define PMIC_LDO22_CTRL2_ldo22_comp_START (3)
#define PMIC_LDO22_CTRL2_ldo22_comp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo22_ocpd_set : 2;
        unsigned char ldo22_ocps_set : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO22_CTRL3_UNION;
#endif
#define PMIC_LDO22_CTRL3_ldo22_ocpd_set_START (0)
#define PMIC_LDO22_CTRL3_ldo22_ocpd_set_END (1)
#define PMIC_LDO22_CTRL3_ldo22_ocps_set_START (2)
#define PMIC_LDO22_CTRL3_ldo22_ocps_set_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo23_vrset : 2;
        unsigned char ldo23_ocp_enn : 1;
        unsigned char ldo23_eco_config : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO23_CTRL_UNION;
#endif
#define PMIC_LDO23_CTRL_ldo23_vrset_START (0)
#define PMIC_LDO23_CTRL_ldo23_vrset_END (1)
#define PMIC_LDO23_CTRL_ldo23_ocp_enn_START (2)
#define PMIC_LDO23_CTRL_ldo23_ocp_enn_END (2)
#define PMIC_LDO23_CTRL_ldo23_eco_config_START (3)
#define PMIC_LDO23_CTRL_ldo23_eco_config_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo24_vrset : 2;
        unsigned char ldo24_ocp_enn : 1;
        unsigned char reserved : 5;
    } reg;
} PMIC_LDO24_CTRL_UNION;
#endif
#define PMIC_LDO24_CTRL_ldo24_vrset_START (0)
#define PMIC_LDO24_CTRL_ldo24_vrset_END (1)
#define PMIC_LDO24_CTRL_ldo24_ocp_enn_START (2)
#define PMIC_LDO24_CTRL_ldo24_ocp_enn_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo25_vrset : 3;
        unsigned char ldo25_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO25_CTRL0_UNION;
#endif
#define PMIC_LDO25_CTRL0_ldo25_vrset_START (0)
#define PMIC_LDO25_CTRL0_ldo25_vrset_END (2)
#define PMIC_LDO25_CTRL0_ldo25_ocp_enn_START (3)
#define PMIC_LDO25_CTRL0_ldo25_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo25_psrr2_set : 3;
        unsigned char ldo25_psrr1_set : 3;
        unsigned char ldo25_psrr_en : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_LDO25_CTRL1_UNION;
#endif
#define PMIC_LDO25_CTRL1_ldo25_psrr2_set_START (0)
#define PMIC_LDO25_CTRL1_ldo25_psrr2_set_END (2)
#define PMIC_LDO25_CTRL1_ldo25_psrr1_set_START (3)
#define PMIC_LDO25_CTRL1_ldo25_psrr1_set_END (5)
#define PMIC_LDO25_CTRL1_ldo25_psrr_en_START (6)
#define PMIC_LDO25_CTRL1_ldo25_psrr_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo25_ocps_en : 1;
        unsigned char ldo25_bw_en : 1;
        unsigned char ldo25_vgpr_en : 1;
        unsigned char ldo25_comp : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO25_CTRL2_UNION;
#endif
#define PMIC_LDO25_CTRL2_ldo25_ocps_en_START (0)
#define PMIC_LDO25_CTRL2_ldo25_ocps_en_END (0)
#define PMIC_LDO25_CTRL2_ldo25_bw_en_START (1)
#define PMIC_LDO25_CTRL2_ldo25_bw_en_END (1)
#define PMIC_LDO25_CTRL2_ldo25_vgpr_en_START (2)
#define PMIC_LDO25_CTRL2_ldo25_vgpr_en_END (2)
#define PMIC_LDO25_CTRL2_ldo25_comp_START (3)
#define PMIC_LDO25_CTRL2_ldo25_comp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo25_ocpd_set : 2;
        unsigned char ldo25_ocps_set : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO25_CTRL3_UNION;
#endif
#define PMIC_LDO25_CTRL3_ldo25_ocpd_set_START (0)
#define PMIC_LDO25_CTRL3_ldo25_ocpd_set_END (1)
#define PMIC_LDO25_CTRL3_ldo25_ocps_set_START (2)
#define PMIC_LDO25_CTRL3_ldo25_ocps_set_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo26_vgpr_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_LDO26_CTRL_UNION;
#endif
#define PMIC_LDO26_CTRL_ldo26_vgpr_en_START (0)
#define PMIC_LDO26_CTRL_ldo26_vgpr_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo27_ocp_enn : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_LDO27_CTRL_UNION;
#endif
#define PMIC_LDO27_CTRL_ldo27_ocp_enn_START (0)
#define PMIC_LDO27_CTRL_ldo27_ocp_enn_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo28_vrset : 3;
        unsigned char ldo28_ocp_set : 2;
        unsigned char ldo28_stb_set : 2;
        unsigned char ldo28_eco_set : 1;
    } reg;
} PMIC_LDO28_CTRL_UNION;
#endif
#define PMIC_LDO28_CTRL_ldo28_vrset_START (0)
#define PMIC_LDO28_CTRL_ldo28_vrset_END (2)
#define PMIC_LDO28_CTRL_ldo28_ocp_set_START (3)
#define PMIC_LDO28_CTRL_ldo28_ocp_set_END (4)
#define PMIC_LDO28_CTRL_ldo28_stb_set_START (5)
#define PMIC_LDO28_CTRL_ldo28_stb_set_END (6)
#define PMIC_LDO28_CTRL_ldo28_eco_set_START (7)
#define PMIC_LDO28_CTRL_ldo28_eco_set_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo29_vrset : 3;
        unsigned char ldo29_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO29_CTRL0_UNION;
#endif
#define PMIC_LDO29_CTRL0_ldo29_vrset_START (0)
#define PMIC_LDO29_CTRL0_ldo29_vrset_END (2)
#define PMIC_LDO29_CTRL0_ldo29_ocp_enn_START (3)
#define PMIC_LDO29_CTRL0_ldo29_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo29_psrr2_set : 3;
        unsigned char ldo29_psrr1_set : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO29_CTRL1_UNION;
#endif
#define PMIC_LDO29_CTRL1_ldo29_psrr2_set_START (0)
#define PMIC_LDO29_CTRL1_ldo29_psrr2_set_END (2)
#define PMIC_LDO29_CTRL1_ldo29_psrr1_set_START (3)
#define PMIC_LDO29_CTRL1_ldo29_psrr1_set_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo29_ocps_en : 1;
        unsigned char ldo29_bw_en : 1;
        unsigned char ldo29_vgpr_en : 1;
        unsigned char ldo29_comp : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO29_CTRL2_UNION;
#endif
#define PMIC_LDO29_CTRL2_ldo29_ocps_en_START (0)
#define PMIC_LDO29_CTRL2_ldo29_ocps_en_END (0)
#define PMIC_LDO29_CTRL2_ldo29_bw_en_START (1)
#define PMIC_LDO29_CTRL2_ldo29_bw_en_END (1)
#define PMIC_LDO29_CTRL2_ldo29_vgpr_en_START (2)
#define PMIC_LDO29_CTRL2_ldo29_vgpr_en_END (2)
#define PMIC_LDO29_CTRL2_ldo29_comp_START (3)
#define PMIC_LDO29_CTRL2_ldo29_comp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo29_ocpd_set : 2;
        unsigned char ldo29_ocps_set : 2;
        unsigned char ldo29_ocps_bw_set : 1;
        unsigned char ldo29_ocpd_vos_set : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO29_CTRL3_UNION;
#endif
#define PMIC_LDO29_CTRL3_ldo29_ocpd_set_START (0)
#define PMIC_LDO29_CTRL3_ldo29_ocpd_set_END (1)
#define PMIC_LDO29_CTRL3_ldo29_ocps_set_START (2)
#define PMIC_LDO29_CTRL3_ldo29_ocps_set_END (3)
#define PMIC_LDO29_CTRL3_ldo29_ocps_bw_set_START (4)
#define PMIC_LDO29_CTRL3_ldo29_ocps_bw_set_END (4)
#define PMIC_LDO29_CTRL3_ldo29_ocpd_vos_set_START (5)
#define PMIC_LDO29_CTRL3_ldo29_ocpd_vos_set_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo30_vrset : 3;
        unsigned char ldo30_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO30_CTRL0_UNION;
#endif
#define PMIC_LDO30_CTRL0_ldo30_vrset_START (0)
#define PMIC_LDO30_CTRL0_ldo30_vrset_END (2)
#define PMIC_LDO30_CTRL0_ldo30_ocp_enn_START (3)
#define PMIC_LDO30_CTRL0_ldo30_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo30_ocps_en : 1;
        unsigned char ldo30_ocpd_vos_set : 1;
        unsigned char ldo30_bw_en : 1;
        unsigned char ldo30_vgpr_en : 1;
        unsigned char ldo30_pd_set : 1;
        unsigned char ldo30_comp : 2;
        unsigned char ldo30_bypass_ctrl_enn : 1;
    } reg;
} PMIC_LDO30_CTRL1_UNION;
#endif
#define PMIC_LDO30_CTRL1_ldo30_ocps_en_START (0)
#define PMIC_LDO30_CTRL1_ldo30_ocps_en_END (0)
#define PMIC_LDO30_CTRL1_ldo30_ocpd_vos_set_START (1)
#define PMIC_LDO30_CTRL1_ldo30_ocpd_vos_set_END (1)
#define PMIC_LDO30_CTRL1_ldo30_bw_en_START (2)
#define PMIC_LDO30_CTRL1_ldo30_bw_en_END (2)
#define PMIC_LDO30_CTRL1_ldo30_vgpr_en_START (3)
#define PMIC_LDO30_CTRL1_ldo30_vgpr_en_END (3)
#define PMIC_LDO30_CTRL1_ldo30_pd_set_START (4)
#define PMIC_LDO30_CTRL1_ldo30_pd_set_END (4)
#define PMIC_LDO30_CTRL1_ldo30_comp_START (5)
#define PMIC_LDO30_CTRL1_ldo30_comp_END (6)
#define PMIC_LDO30_CTRL1_ldo30_bypass_ctrl_enn_START (7)
#define PMIC_LDO30_CTRL1_ldo30_bypass_ctrl_enn_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo30_ocpd_set : 2;
        unsigned char ldo30_ocps_set : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO30_CTRL2_UNION;
#endif
#define PMIC_LDO30_CTRL2_ldo30_ocpd_set_START (0)
#define PMIC_LDO30_CTRL2_ldo30_ocpd_set_END (1)
#define PMIC_LDO30_CTRL2_ldo30_ocps_set_START (2)
#define PMIC_LDO30_CTRL2_ldo30_ocps_set_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo32_vrset : 3;
        unsigned char ldo32_ocp_enn : 1;
        unsigned char ldo32_iss_en : 1;
        unsigned char ldo32_i_config : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO32_CTRL0_UNION;
#endif
#define PMIC_LDO32_CTRL0_ldo32_vrset_START (0)
#define PMIC_LDO32_CTRL0_ldo32_vrset_END (2)
#define PMIC_LDO32_CTRL0_ldo32_ocp_enn_START (3)
#define PMIC_LDO32_CTRL0_ldo32_ocp_enn_END (3)
#define PMIC_LDO32_CTRL0_ldo32_iss_en_START (4)
#define PMIC_LDO32_CTRL0_ldo32_iss_en_END (4)
#define PMIC_LDO32_CTRL0_ldo32_i_config_START (5)
#define PMIC_LDO32_CTRL0_ldo32_i_config_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo32_psrr2_set : 3;
        unsigned char ldo32_psrr1_set : 3;
        unsigned char ldo32_psrr_en : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_LDO32_CTRL1_UNION;
#endif
#define PMIC_LDO32_CTRL1_ldo32_psrr2_set_START (0)
#define PMIC_LDO32_CTRL1_ldo32_psrr2_set_END (2)
#define PMIC_LDO32_CTRL1_ldo32_psrr1_set_START (3)
#define PMIC_LDO32_CTRL1_ldo32_psrr1_set_END (5)
#define PMIC_LDO32_CTRL1_ldo32_psrr_en_START (6)
#define PMIC_LDO32_CTRL1_ldo32_psrr_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo32_ocps_en : 1;
        unsigned char ldo32_bw_en : 1;
        unsigned char ldo32_vgpr_en : 1;
        unsigned char ldo32_comp : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO32_CTRL2_UNION;
#endif
#define PMIC_LDO32_CTRL2_ldo32_ocps_en_START (0)
#define PMIC_LDO32_CTRL2_ldo32_ocps_en_END (0)
#define PMIC_LDO32_CTRL2_ldo32_bw_en_START (1)
#define PMIC_LDO32_CTRL2_ldo32_bw_en_END (1)
#define PMIC_LDO32_CTRL2_ldo32_vgpr_en_START (2)
#define PMIC_LDO32_CTRL2_ldo32_vgpr_en_END (2)
#define PMIC_LDO32_CTRL2_ldo32_comp_START (3)
#define PMIC_LDO32_CTRL2_ldo32_comp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo32_ocpd_set : 3;
        unsigned char ldo32_ocps_set : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO32_CTRL3_UNION;
#endif
#define PMIC_LDO32_CTRL3_ldo32_ocpd_set_START (0)
#define PMIC_LDO32_CTRL3_ldo32_ocpd_set_END (2)
#define PMIC_LDO32_CTRL3_ldo32_ocps_set_START (3)
#define PMIC_LDO32_CTRL3_ldo32_ocps_set_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pmuh_vrset : 3;
        unsigned char pmuh_ocp_enn : 1;
        unsigned char pmuh_stb_set : 4;
    } reg;
} PMIC_PMUH_CTRL_UNION;
#endif
#define PMIC_PMUH_CTRL_pmuh_vrset_START (0)
#define PMIC_PMUH_CTRL_pmuh_vrset_END (2)
#define PMIC_PMUH_CTRL_pmuh_ocp_enn_START (3)
#define PMIC_PMUH_CTRL_pmuh_ocp_enn_END (3)
#define PMIC_PMUH_CTRL_pmuh_stb_set_START (4)
#define PMIC_PMUH_CTRL_pmuh_stb_set_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo36_vrset : 3;
        unsigned char ldo36_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO36_CTRL0_UNION;
#endif
#define PMIC_LDO36_CTRL0_ldo36_vrset_START (0)
#define PMIC_LDO36_CTRL0_ldo36_vrset_END (2)
#define PMIC_LDO36_CTRL0_ldo36_ocp_enn_START (3)
#define PMIC_LDO36_CTRL0_ldo36_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo36_ocpd_set : 2;
        unsigned char ldo36_ocps_set : 2;
        unsigned char ldo36_ocps_en : 1;
        unsigned char ldo36_bw_en : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO36_CTRL1_UNION;
#endif
#define PMIC_LDO36_CTRL1_ldo36_ocpd_set_START (0)
#define PMIC_LDO36_CTRL1_ldo36_ocpd_set_END (1)
#define PMIC_LDO36_CTRL1_ldo36_ocps_set_START (2)
#define PMIC_LDO36_CTRL1_ldo36_ocps_set_END (3)
#define PMIC_LDO36_CTRL1_ldo36_ocps_en_START (4)
#define PMIC_LDO36_CTRL1_ldo36_ocps_en_END (4)
#define PMIC_LDO36_CTRL1_ldo36_bw_en_START (5)
#define PMIC_LDO36_CTRL1_ldo36_bw_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo37_vrset : 3;
        unsigned char ldo37_ocp_enn : 1;
        unsigned char ldo37_eco_set : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_LDO37_CTRL0_UNION;
#endif
#define PMIC_LDO37_CTRL0_ldo37_vrset_START (0)
#define PMIC_LDO37_CTRL0_ldo37_vrset_END (2)
#define PMIC_LDO37_CTRL0_ldo37_ocp_enn_START (3)
#define PMIC_LDO37_CTRL0_ldo37_ocp_enn_END (3)
#define PMIC_LDO37_CTRL0_ldo37_eco_set_START (4)
#define PMIC_LDO37_CTRL0_ldo37_eco_set_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo37_ocpd_set : 2;
        unsigned char ldo37_ocps_set : 2;
        unsigned char ldo37_ocps_en : 1;
        unsigned char ldo37_bw_en : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO37_CTRL1_UNION;
#endif
#define PMIC_LDO37_CTRL1_ldo37_ocpd_set_START (0)
#define PMIC_LDO37_CTRL1_ldo37_ocpd_set_END (1)
#define PMIC_LDO37_CTRL1_ldo37_ocps_set_START (2)
#define PMIC_LDO37_CTRL1_ldo37_ocps_set_END (3)
#define PMIC_LDO37_CTRL1_ldo37_ocps_en_START (4)
#define PMIC_LDO37_CTRL1_ldo37_ocps_en_END (4)
#define PMIC_LDO37_CTRL1_ldo37_bw_en_START (5)
#define PMIC_LDO37_CTRL1_ldo37_bw_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo38_vrset : 3;
        unsigned char ldo38_ocp_enn : 1;
        unsigned char ldo38_iss_en : 1;
        unsigned char ldo38_eco_set : 1;
        unsigned char ldo38_i_config : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_LDO38_CTRL0_UNION;
#endif
#define PMIC_LDO38_CTRL0_ldo38_vrset_START (0)
#define PMIC_LDO38_CTRL0_ldo38_vrset_END (2)
#define PMIC_LDO38_CTRL0_ldo38_ocp_enn_START (3)
#define PMIC_LDO38_CTRL0_ldo38_ocp_enn_END (3)
#define PMIC_LDO38_CTRL0_ldo38_iss_en_START (4)
#define PMIC_LDO38_CTRL0_ldo38_iss_en_END (4)
#define PMIC_LDO38_CTRL0_ldo38_eco_set_START (5)
#define PMIC_LDO38_CTRL0_ldo38_eco_set_END (5)
#define PMIC_LDO38_CTRL0_ldo38_i_config_START (6)
#define PMIC_LDO38_CTRL0_ldo38_i_config_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo38_psrr2_set : 3;
        unsigned char ldo38_psrr1_set : 3;
        unsigned char ldo38_psrr_en : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_LDO38_CTRL1_UNION;
#endif
#define PMIC_LDO38_CTRL1_ldo38_psrr2_set_START (0)
#define PMIC_LDO38_CTRL1_ldo38_psrr2_set_END (2)
#define PMIC_LDO38_CTRL1_ldo38_psrr1_set_START (3)
#define PMIC_LDO38_CTRL1_ldo38_psrr1_set_END (5)
#define PMIC_LDO38_CTRL1_ldo38_psrr_en_START (6)
#define PMIC_LDO38_CTRL1_ldo38_psrr_en_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo38_ocps_en : 1;
        unsigned char ldo38_bw_en : 1;
        unsigned char ldo38_vgpr_en : 1;
        unsigned char ldo38_comp : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO38_CTRL2_UNION;
#endif
#define PMIC_LDO38_CTRL2_ldo38_ocps_en_START (0)
#define PMIC_LDO38_CTRL2_ldo38_ocps_en_END (0)
#define PMIC_LDO38_CTRL2_ldo38_bw_en_START (1)
#define PMIC_LDO38_CTRL2_ldo38_bw_en_END (1)
#define PMIC_LDO38_CTRL2_ldo38_vgpr_en_START (2)
#define PMIC_LDO38_CTRL2_ldo38_vgpr_en_END (2)
#define PMIC_LDO38_CTRL2_ldo38_comp_START (3)
#define PMIC_LDO38_CTRL2_ldo38_comp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo38_ocpd_set : 3;
        unsigned char ldo38_ocps_set : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO38_CTRL3_UNION;
#endif
#define PMIC_LDO38_CTRL3_ldo38_ocpd_set_START (0)
#define PMIC_LDO38_CTRL3_ldo38_ocpd_set_END (2)
#define PMIC_LDO38_CTRL3_ldo38_ocps_set_START (3)
#define PMIC_LDO38_CTRL3_ldo38_ocps_set_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo41_vrset : 3;
        unsigned char ldo41_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO41_CTRL0_UNION;
#endif
#define PMIC_LDO41_CTRL0_ldo41_vrset_START (0)
#define PMIC_LDO41_CTRL0_ldo41_vrset_END (2)
#define PMIC_LDO41_CTRL0_ldo41_ocp_enn_START (3)
#define PMIC_LDO41_CTRL0_ldo41_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo41_psrr2_set : 3;
        unsigned char ldo41_psrr1_set : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO41_CTRL1_UNION;
#endif
#define PMIC_LDO41_CTRL1_ldo41_psrr2_set_START (0)
#define PMIC_LDO41_CTRL1_ldo41_psrr2_set_END (2)
#define PMIC_LDO41_CTRL1_ldo41_psrr1_set_START (3)
#define PMIC_LDO41_CTRL1_ldo41_psrr1_set_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo41_ocps_en : 1;
        unsigned char ldo41_bw_en : 1;
        unsigned char ldo41_vgpr_en : 1;
        unsigned char ldo41_comp : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO41_CTRL2_UNION;
#endif
#define PMIC_LDO41_CTRL2_ldo41_ocps_en_START (0)
#define PMIC_LDO41_CTRL2_ldo41_ocps_en_END (0)
#define PMIC_LDO41_CTRL2_ldo41_bw_en_START (1)
#define PMIC_LDO41_CTRL2_ldo41_bw_en_END (1)
#define PMIC_LDO41_CTRL2_ldo41_vgpr_en_START (2)
#define PMIC_LDO41_CTRL2_ldo41_vgpr_en_END (2)
#define PMIC_LDO41_CTRL2_ldo41_comp_START (3)
#define PMIC_LDO41_CTRL2_ldo41_comp_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo41_ocpd_set : 2;
        unsigned char ldo41_ocps_set : 2;
        unsigned char ldo41_ocps_bw_set : 1;
        unsigned char ldo41_ocpd_vos_set : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO41_CTRL3_UNION;
#endif
#define PMIC_LDO41_CTRL3_ldo41_ocpd_set_START (0)
#define PMIC_LDO41_CTRL3_ldo41_ocpd_set_END (1)
#define PMIC_LDO41_CTRL3_ldo41_ocps_set_START (2)
#define PMIC_LDO41_CTRL3_ldo41_ocps_set_END (3)
#define PMIC_LDO41_CTRL3_ldo41_ocps_bw_set_START (4)
#define PMIC_LDO41_CTRL3_ldo41_ocps_bw_set_END (4)
#define PMIC_LDO41_CTRL3_ldo41_ocpd_vos_set_START (5)
#define PMIC_LDO41_CTRL3_ldo41_ocpd_vos_set_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo42_vrset : 3;
        unsigned char ldo42_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO42_CTRL0_UNION;
#endif
#define PMIC_LDO42_CTRL0_ldo42_vrset_START (0)
#define PMIC_LDO42_CTRL0_ldo42_vrset_END (2)
#define PMIC_LDO42_CTRL0_ldo42_ocp_enn_START (3)
#define PMIC_LDO42_CTRL0_ldo42_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo42_ocps_en : 1;
        unsigned char ldo42_ocpd_vos_set : 1;
        unsigned char ldo42_bw_en : 1;
        unsigned char ldo42_vgpr_en : 1;
        unsigned char ldo42_pd_set : 1;
        unsigned char ldo42_comp : 2;
        unsigned char ldo42_bypass_ctrl_enn : 1;
    } reg;
} PMIC_LDO42_CTRL1_UNION;
#endif
#define PMIC_LDO42_CTRL1_ldo42_ocps_en_START (0)
#define PMIC_LDO42_CTRL1_ldo42_ocps_en_END (0)
#define PMIC_LDO42_CTRL1_ldo42_ocpd_vos_set_START (1)
#define PMIC_LDO42_CTRL1_ldo42_ocpd_vos_set_END (1)
#define PMIC_LDO42_CTRL1_ldo42_bw_en_START (2)
#define PMIC_LDO42_CTRL1_ldo42_bw_en_END (2)
#define PMIC_LDO42_CTRL1_ldo42_vgpr_en_START (3)
#define PMIC_LDO42_CTRL1_ldo42_vgpr_en_END (3)
#define PMIC_LDO42_CTRL1_ldo42_pd_set_START (4)
#define PMIC_LDO42_CTRL1_ldo42_pd_set_END (4)
#define PMIC_LDO42_CTRL1_ldo42_comp_START (5)
#define PMIC_LDO42_CTRL1_ldo42_comp_END (6)
#define PMIC_LDO42_CTRL1_ldo42_bypass_ctrl_enn_START (7)
#define PMIC_LDO42_CTRL1_ldo42_bypass_ctrl_enn_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo42_ocpd_set : 2;
        unsigned char ldo42_ocps_set : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO42_CTRL2_UNION;
#endif
#define PMIC_LDO42_CTRL2_ldo42_ocpd_set_START (0)
#define PMIC_LDO42_CTRL2_ldo42_ocpd_set_END (1)
#define PMIC_LDO42_CTRL2_ldo42_ocps_set_START (2)
#define PMIC_LDO42_CTRL2_ldo42_ocps_set_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo43_vrset : 3;
        unsigned char ldo43_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO43_CTRL0_UNION;
#endif
#define PMIC_LDO43_CTRL0_ldo43_vrset_START (0)
#define PMIC_LDO43_CTRL0_ldo43_vrset_END (2)
#define PMIC_LDO43_CTRL0_ldo43_ocp_enn_START (3)
#define PMIC_LDO43_CTRL0_ldo43_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo43_ocps_en : 1;
        unsigned char ldo43_ocpd_vos_set : 1;
        unsigned char ldo43_bw_en : 1;
        unsigned char ldo43_vgpr_en : 1;
        unsigned char ldo43_pd_set : 1;
        unsigned char ldo43_comp : 2;
        unsigned char ldo43_bypass_ctrl_enn : 1;
    } reg;
} PMIC_LDO43_CTRL1_UNION;
#endif
#define PMIC_LDO43_CTRL1_ldo43_ocps_en_START (0)
#define PMIC_LDO43_CTRL1_ldo43_ocps_en_END (0)
#define PMIC_LDO43_CTRL1_ldo43_ocpd_vos_set_START (1)
#define PMIC_LDO43_CTRL1_ldo43_ocpd_vos_set_END (1)
#define PMIC_LDO43_CTRL1_ldo43_bw_en_START (2)
#define PMIC_LDO43_CTRL1_ldo43_bw_en_END (2)
#define PMIC_LDO43_CTRL1_ldo43_vgpr_en_START (3)
#define PMIC_LDO43_CTRL1_ldo43_vgpr_en_END (3)
#define PMIC_LDO43_CTRL1_ldo43_pd_set_START (4)
#define PMIC_LDO43_CTRL1_ldo43_pd_set_END (4)
#define PMIC_LDO43_CTRL1_ldo43_comp_START (5)
#define PMIC_LDO43_CTRL1_ldo43_comp_END (6)
#define PMIC_LDO43_CTRL1_ldo43_bypass_ctrl_enn_START (7)
#define PMIC_LDO43_CTRL1_ldo43_bypass_ctrl_enn_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo43_ocpd_set : 2;
        unsigned char ldo43_ocps_set : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO43_CTRL2_UNION;
#endif
#define PMIC_LDO43_CTRL2_ldo43_ocpd_set_START (0)
#define PMIC_LDO43_CTRL2_ldo43_ocpd_set_END (1)
#define PMIC_LDO43_CTRL2_ldo43_ocps_set_START (2)
#define PMIC_LDO43_CTRL2_ldo43_ocps_set_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo44_vrset : 3;
        unsigned char ldo44_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO44_CTRL0_UNION;
#endif
#define PMIC_LDO44_CTRL0_ldo44_vrset_START (0)
#define PMIC_LDO44_CTRL0_ldo44_vrset_END (2)
#define PMIC_LDO44_CTRL0_ldo44_ocp_enn_START (3)
#define PMIC_LDO44_CTRL0_ldo44_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo44_ocps_en : 1;
        unsigned char ldo44_ocpd_vos_set : 1;
        unsigned char ldo44_bw_en : 1;
        unsigned char ldo44_vgpr_en : 1;
        unsigned char ldo44_pd_set : 1;
        unsigned char ldo44_comp : 2;
        unsigned char ldo44_bypass_ctrl_enn : 1;
    } reg;
} PMIC_LDO44_CTRL1_UNION;
#endif
#define PMIC_LDO44_CTRL1_ldo44_ocps_en_START (0)
#define PMIC_LDO44_CTRL1_ldo44_ocps_en_END (0)
#define PMIC_LDO44_CTRL1_ldo44_ocpd_vos_set_START (1)
#define PMIC_LDO44_CTRL1_ldo44_ocpd_vos_set_END (1)
#define PMIC_LDO44_CTRL1_ldo44_bw_en_START (2)
#define PMIC_LDO44_CTRL1_ldo44_bw_en_END (2)
#define PMIC_LDO44_CTRL1_ldo44_vgpr_en_START (3)
#define PMIC_LDO44_CTRL1_ldo44_vgpr_en_END (3)
#define PMIC_LDO44_CTRL1_ldo44_pd_set_START (4)
#define PMIC_LDO44_CTRL1_ldo44_pd_set_END (4)
#define PMIC_LDO44_CTRL1_ldo44_comp_START (5)
#define PMIC_LDO44_CTRL1_ldo44_comp_END (6)
#define PMIC_LDO44_CTRL1_ldo44_bypass_ctrl_enn_START (7)
#define PMIC_LDO44_CTRL1_ldo44_bypass_ctrl_enn_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo44_ocpd_set : 2;
        unsigned char ldo44_ocps_set : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO44_CTRL2_UNION;
#endif
#define PMIC_LDO44_CTRL2_ldo44_ocpd_set_START (0)
#define PMIC_LDO44_CTRL2_ldo44_ocpd_set_END (1)
#define PMIC_LDO44_CTRL2_ldo44_ocps_set_START (2)
#define PMIC_LDO44_CTRL2_ldo44_ocps_set_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo45_vrset : 3;
        unsigned char ldo45_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO45_CTRL0_UNION;
#endif
#define PMIC_LDO45_CTRL0_ldo45_vrset_START (0)
#define PMIC_LDO45_CTRL0_ldo45_vrset_END (2)
#define PMIC_LDO45_CTRL0_ldo45_ocp_enn_START (3)
#define PMIC_LDO45_CTRL0_ldo45_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo45_ocps_en : 1;
        unsigned char ldo45_ocpd_vos_set : 1;
        unsigned char ldo45_bw_en : 1;
        unsigned char ldo45_vgpr_en : 1;
        unsigned char ldo45_pd_set : 1;
        unsigned char ldo45_comp : 2;
        unsigned char ldo45_bypass_ctrl_enn : 1;
    } reg;
} PMIC_LDO45_CTRL1_UNION;
#endif
#define PMIC_LDO45_CTRL1_ldo45_ocps_en_START (0)
#define PMIC_LDO45_CTRL1_ldo45_ocps_en_END (0)
#define PMIC_LDO45_CTRL1_ldo45_ocpd_vos_set_START (1)
#define PMIC_LDO45_CTRL1_ldo45_ocpd_vos_set_END (1)
#define PMIC_LDO45_CTRL1_ldo45_bw_en_START (2)
#define PMIC_LDO45_CTRL1_ldo45_bw_en_END (2)
#define PMIC_LDO45_CTRL1_ldo45_vgpr_en_START (3)
#define PMIC_LDO45_CTRL1_ldo45_vgpr_en_END (3)
#define PMIC_LDO45_CTRL1_ldo45_pd_set_START (4)
#define PMIC_LDO45_CTRL1_ldo45_pd_set_END (4)
#define PMIC_LDO45_CTRL1_ldo45_comp_START (5)
#define PMIC_LDO45_CTRL1_ldo45_comp_END (6)
#define PMIC_LDO45_CTRL1_ldo45_bypass_ctrl_enn_START (7)
#define PMIC_LDO45_CTRL1_ldo45_bypass_ctrl_enn_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo45_ocpd_set : 2;
        unsigned char ldo45_ocps_set : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO45_CTRL2_UNION;
#endif
#define PMIC_LDO45_CTRL2_ldo45_ocpd_set_START (0)
#define PMIC_LDO45_CTRL2_ldo45_ocpd_set_END (1)
#define PMIC_LDO45_CTRL2_ldo45_ocps_set_START (2)
#define PMIC_LDO45_CTRL2_ldo45_ocps_set_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo46_vrset : 3;
        unsigned char ldo46_ocp_enn : 1;
        unsigned char ldo46_eco_set : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_LDO46_CTRL0_UNION;
#endif
#define PMIC_LDO46_CTRL0_ldo46_vrset_START (0)
#define PMIC_LDO46_CTRL0_ldo46_vrset_END (2)
#define PMIC_LDO46_CTRL0_ldo46_ocp_enn_START (3)
#define PMIC_LDO46_CTRL0_ldo46_ocp_enn_END (3)
#define PMIC_LDO46_CTRL0_ldo46_eco_set_START (4)
#define PMIC_LDO46_CTRL0_ldo46_eco_set_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo46_ocpd_set : 2;
        unsigned char ldo46_ocps_set : 2;
        unsigned char ldo46_ocps_en : 1;
        unsigned char ldo46_bw_en : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO46_CTRL1_UNION;
#endif
#define PMIC_LDO46_CTRL1_ldo46_ocpd_set_START (0)
#define PMIC_LDO46_CTRL1_ldo46_ocpd_set_END (1)
#define PMIC_LDO46_CTRL1_ldo46_ocps_set_START (2)
#define PMIC_LDO46_CTRL1_ldo46_ocps_set_END (3)
#define PMIC_LDO46_CTRL1_ldo46_ocps_en_START (4)
#define PMIC_LDO46_CTRL1_ldo46_ocps_en_END (4)
#define PMIC_LDO46_CTRL1_ldo46_bw_en_START (5)
#define PMIC_LDO46_CTRL1_ldo46_bw_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sw1_ocpd_set : 2;
        unsigned char sw1_ocps_set : 2;
        unsigned char sw1_ocps_en : 1;
        unsigned char sw1_ocps_curr_set : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_SW1_CTRL_UNION;
#endif
#define PMIC_SW1_CTRL_sw1_ocpd_set_START (0)
#define PMIC_SW1_CTRL_sw1_ocpd_set_END (1)
#define PMIC_SW1_CTRL_sw1_ocps_set_START (2)
#define PMIC_SW1_CTRL_sw1_ocps_set_END (3)
#define PMIC_SW1_CTRL_sw1_ocps_en_START (4)
#define PMIC_SW1_CTRL_sw1_ocps_en_END (4)
#define PMIC_SW1_CTRL_sw1_ocps_curr_set_START (5)
#define PMIC_SW1_CTRL_sw1_ocps_curr_set_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_sink_ocpd_set : 2;
        unsigned char ldo_sink_ocp_enn : 1;
        unsigned char ldo_sink_offres_sel : 1;
        unsigned char ldo_sink_config : 4;
    } reg;
} PMIC_LDO_SINK_CTRL_UNION;
#endif
#define PMIC_LDO_SINK_CTRL_ldo_sink_ocpd_set_START (0)
#define PMIC_LDO_SINK_CTRL_ldo_sink_ocpd_set_END (1)
#define PMIC_LDO_SINK_CTRL_ldo_sink_ocp_enn_START (2)
#define PMIC_LDO_SINK_CTRL_ldo_sink_ocp_enn_END (2)
#define PMIC_LDO_SINK_CTRL_ldo_sink_offres_sel_START (3)
#define PMIC_LDO_SINK_CTRL_ldo_sink_offres_sel_END (3)
#define PMIC_LDO_SINK_CTRL_ldo_sink_config_START (4)
#define PMIC_LDO_SINK_CTRL_ldo_sink_config_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_buff_curr_sel : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_LDO_BUF_REF_CTRL_UNION;
#endif
#define PMIC_LDO_BUF_REF_CTRL_ldo_buff_curr_sel_START (0)
#define PMIC_LDO_BUF_REF_CTRL_ldo_buff_curr_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve0 : 8;
    } reg;
} PMIC_LDO_RESERVE0_UNION;
#endif
#define PMIC_LDO_RESERVE0_ldo_reserve0_START (0)
#define PMIC_LDO_RESERVE0_ldo_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve1 : 8;
    } reg;
} PMIC_LDO_RESERVE1_UNION;
#endif
#define PMIC_LDO_RESERVE1_ldo_reserve1_START (0)
#define PMIC_LDO_RESERVE1_ldo_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve2 : 8;
    } reg;
} PMIC_LDO_RESERVE2_UNION;
#endif
#define PMIC_LDO_RESERVE2_ldo_reserve2_START (0)
#define PMIC_LDO_RESERVE2_ldo_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve3 : 8;
    } reg;
} PMIC_LDO_RESERVE3_UNION;
#endif
#define PMIC_LDO_RESERVE3_ldo_reserve3_START (0)
#define PMIC_LDO_RESERVE3_ldo_reserve3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve4 : 8;
    } reg;
} PMIC_LDO_RESERVE4_UNION;
#endif
#define PMIC_LDO_RESERVE4_ldo_reserve4_START (0)
#define PMIC_LDO_RESERVE4_ldo_reserve4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve5 : 8;
    } reg;
} PMIC_LDO_RESERVE5_UNION;
#endif
#define PMIC_LDO_RESERVE5_ldo_reserve5_START (0)
#define PMIC_LDO_RESERVE5_ldo_reserve5_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve6 : 8;
    } reg;
} PMIC_LDO_RESERVE6_UNION;
#endif
#define PMIC_LDO_RESERVE6_ldo_reserve6_START (0)
#define PMIC_LDO_RESERVE6_ldo_reserve6_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve7 : 8;
    } reg;
} PMIC_LDO_RESERVE7_UNION;
#endif
#define PMIC_LDO_RESERVE7_ldo_reserve7_START (0)
#define PMIC_LDO_RESERVE7_ldo_reserve7_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve8 : 8;
    } reg;
} PMIC_LDO_RESERVE8_UNION;
#endif
#define PMIC_LDO_RESERVE8_ldo_reserve8_START (0)
#define PMIC_LDO_RESERVE8_ldo_reserve8_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo_reserve9 : 8;
    } reg;
} PMIC_LDO_RESERVE9_UNION;
#endif
#define PMIC_LDO_RESERVE9_ldo_reserve9_START (0)
#define PMIC_LDO_RESERVE9_ldo_reserve9_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char d2a_reserve0 : 8;
    } reg;
} PMIC_D2A_RES0_UNION;
#endif
#define PMIC_D2A_RES0_d2a_reserve0_START (0)
#define PMIC_D2A_RES0_d2a_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char d2a_reserve1 : 8;
    } reg;
} PMIC_D2A_RES1_UNION;
#endif
#define PMIC_D2A_RES1_d2a_reserve1_START (0)
#define PMIC_D2A_RES1_d2a_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char a2d_reserve0 : 8;
    } reg;
} PMIC_A2D_RES0_UNION;
#endif
#define PMIC_A2D_RES0_a2d_reserve0_START (0)
#define PMIC_A2D_RES0_a2d_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtcdr0 : 8;
    } reg;
} PMIC_SER_RTCDR0_UNION;
#endif
#define PMIC_SER_RTCDR0_ser_rtcdr0_START (0)
#define PMIC_SER_RTCDR0_ser_rtcdr0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtcdr1 : 8;
    } reg;
} PMIC_SER_RTCDR1_UNION;
#endif
#define PMIC_SER_RTCDR1_ser_rtcdr1_START (0)
#define PMIC_SER_RTCDR1_ser_rtcdr1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtcdr2 : 8;
    } reg;
} PMIC_SER_RTCDR2_UNION;
#endif
#define PMIC_SER_RTCDR2_ser_rtcdr2_START (0)
#define PMIC_SER_RTCDR2_ser_rtcdr2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtcdr3 : 8;
    } reg;
} PMIC_SER_RTCDR3_UNION;
#endif
#define PMIC_SER_RTCDR3_ser_rtcdr3_START (0)
#define PMIC_SER_RTCDR3_ser_rtcdr3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtcmr0 : 8;
    } reg;
} PMIC_SER_RTCMR0_UNION;
#endif
#define PMIC_SER_RTCMR0_ser_rtcmr0_START (0)
#define PMIC_SER_RTCMR0_ser_rtcmr0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtcmr1 : 8;
    } reg;
} PMIC_SER_RTCMR1_UNION;
#endif
#define PMIC_SER_RTCMR1_ser_rtcmr1_START (0)
#define PMIC_SER_RTCMR1_ser_rtcmr1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtcmr2 : 8;
    } reg;
} PMIC_SER_RTCMR2_UNION;
#endif
#define PMIC_SER_RTCMR2_ser_rtcmr2_START (0)
#define PMIC_SER_RTCMR2_ser_rtcmr2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtcmr3 : 8;
    } reg;
} PMIC_SER_RTCMR3_UNION;
#endif
#define PMIC_SER_RTCMR3_ser_rtcmr3_START (0)
#define PMIC_SER_RTCMR3_ser_rtcmr3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtcclr0 : 8;
    } reg;
} PMIC_SER_RTCLR0_UNION;
#endif
#define PMIC_SER_RTCLR0_ser_rtcclr0_START (0)
#define PMIC_SER_RTCLR0_ser_rtcclr0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtcclr1 : 8;
    } reg;
} PMIC_SER_RTCLR1_UNION;
#endif
#define PMIC_SER_RTCLR1_ser_rtcclr1_START (0)
#define PMIC_SER_RTCLR1_ser_rtcclr1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtcclr2 : 8;
    } reg;
} PMIC_SER_RTCLR2_UNION;
#endif
#define PMIC_SER_RTCLR2_ser_rtcclr2_START (0)
#define PMIC_SER_RTCLR2_ser_rtcclr2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtcclr3 : 8;
    } reg;
} PMIC_SER_RTCLR3_UNION;
#endif
#define PMIC_SER_RTCLR3_ser_rtcclr3_START (0)
#define PMIC_SER_RTCLR3_ser_rtcclr3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtccr : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_SER_RTCCTRL_UNION;
#endif
#define PMIC_SER_RTCCTRL_ser_rtccr_START (0)
#define PMIC_SER_RTCCTRL_ser_rtccr_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_xo_cali_thresold_low : 8;
    } reg;
} PMIC_SER_XO_THRESOLD0_UNION;
#endif
#define PMIC_SER_XO_THRESOLD0_ser_xo_cali_thresold_low_START (0)
#define PMIC_SER_XO_THRESOLD0_ser_xo_cali_thresold_low_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_xo_cali_thresold_high : 8;
    } reg;
} PMIC_SER_XO_THRESOLD1_UNION;
#endif
#define PMIC_SER_XO_THRESOLD1_ser_xo_cali_thresold_high_START (0)
#define PMIC_SER_XO_THRESOLD1_ser_xo_cali_thresold_high_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_crc_value0 : 8;
    } reg;
} PMIC_SER_CRC_VAULE0_UNION;
#endif
#define PMIC_SER_CRC_VAULE0_ser_crc_value0_START (0)
#define PMIC_SER_CRC_VAULE0_ser_crc_value0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_crc_value1 : 8;
    } reg;
} PMIC_SER_CRC_VAULE1_UNION;
#endif
#define PMIC_SER_CRC_VAULE1_ser_crc_value1_START (0)
#define PMIC_SER_CRC_VAULE1_ser_crc_value1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_crc_value2 : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_SER_CRC_VAULE2_UNION;
#endif
#define PMIC_SER_CRC_VAULE2_ser_crc_value2_START (0)
#define PMIC_SER_CRC_VAULE2_ser_crc_value2_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtc_pwrup_timer0 : 8;
    } reg;
} PMIC_SER_RTC_PWRUP_TIMER0_UNION;
#endif
#define PMIC_SER_RTC_PWRUP_TIMER0_ser_rtc_pwrup_timer0_START (0)
#define PMIC_SER_RTC_PWRUP_TIMER0_ser_rtc_pwrup_timer0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtc_pwrup_timer1 : 8;
    } reg;
} PMIC_SER_RTC_PWRUP_TIMER1_UNION;
#endif
#define PMIC_SER_RTC_PWRUP_TIMER1_ser_rtc_pwrup_timer1_START (0)
#define PMIC_SER_RTC_PWRUP_TIMER1_ser_rtc_pwrup_timer1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtc_pwrup_timer2 : 8;
    } reg;
} PMIC_SER_RTC_PWRUP_TIMER2_UNION;
#endif
#define PMIC_SER_RTC_PWRUP_TIMER2_ser_rtc_pwrup_timer2_START (0)
#define PMIC_SER_RTC_PWRUP_TIMER2_ser_rtc_pwrup_timer2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtc_pwrup_timer3 : 8;
    } reg;
} PMIC_SER_RTC_PWRUP_TIMER3_UNION;
#endif
#define PMIC_SER_RTC_PWRUP_TIMER3_ser_rtc_pwrup_timer3_START (0)
#define PMIC_SER_RTC_PWRUP_TIMER3_ser_rtc_pwrup_timer3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtc_pwrdown_timer0 : 8;
    } reg;
} PMIC_SER_RTC_PWRDOWN_TIMER0_UNION;
#endif
#define PMIC_SER_RTC_PWRDOWN_TIMER0_ser_rtc_pwrdown_timer0_START (0)
#define PMIC_SER_RTC_PWRDOWN_TIMER0_ser_rtc_pwrdown_timer0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtc_pwrdown_timer1 : 8;
    } reg;
} PMIC_SER_RTC_PWRDOWN_TIMER1_UNION;
#endif
#define PMIC_SER_RTC_PWRDOWN_TIMER1_ser_rtc_pwrdown_timer1_START (0)
#define PMIC_SER_RTC_PWRDOWN_TIMER1_ser_rtc_pwrdown_timer1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtc_pwrdown_timer2 : 8;
    } reg;
} PMIC_SER_RTC_PWRDOWN_TIMER2_UNION;
#endif
#define PMIC_SER_RTC_PWRDOWN_TIMER2_ser_rtc_pwrdown_timer2_START (0)
#define PMIC_SER_RTC_PWRDOWN_TIMER2_ser_rtc_pwrdown_timer2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_rtc_pwrdown_timer3 : 8;
    } reg;
} PMIC_SER_RTC_PWRDOWN_TIMER3_UNION;
#endif
#define PMIC_SER_RTC_PWRDOWN_TIMER3_ser_rtc_pwrdown_timer3_START (0)
#define PMIC_SER_RTC_PWRDOWN_TIMER3_ser_rtc_pwrdown_timer3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo2_en_a : 1;
        unsigned char st_ldo2_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo2_eco_en : 1;
        unsigned char st_ldo2_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO2_ONOFF1_ECO_UNION;
#endif
#define PMIC_LDO2_ONOFF1_ECO_reg_ldo2_en_a_START (0)
#define PMIC_LDO2_ONOFF1_ECO_reg_ldo2_en_a_END (0)
#define PMIC_LDO2_ONOFF1_ECO_st_ldo2_en_START (1)
#define PMIC_LDO2_ONOFF1_ECO_st_ldo2_en_END (1)
#define PMIC_LDO2_ONOFF1_ECO_reg_ldo2_eco_en_START (4)
#define PMIC_LDO2_ONOFF1_ECO_reg_ldo2_eco_en_END (4)
#define PMIC_LDO2_ONOFF1_ECO_st_ldo2_eco_en_START (5)
#define PMIC_LDO2_ONOFF1_ECO_st_ldo2_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo2_en_b : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_LDO2_ONOFF2_UNION;
#endif
#define PMIC_LDO2_ONOFF2_reg_ldo2_en_b_START (0)
#define PMIC_LDO2_ONOFF2_reg_ldo2_en_b_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo2_vset : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_LDO2_VSET_UNION;
#endif
#define PMIC_LDO2_VSET_ldo2_vset_START (0)
#define PMIC_LDO2_VSET_ldo2_vset_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo2_vrset : 3;
        unsigned char ldo2_ocp_enn : 1;
        unsigned char ldo2_eco_set : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_LDO2_CTRL_UNION;
#endif
#define PMIC_LDO2_CTRL_ldo2_vrset_START (0)
#define PMIC_LDO2_CTRL_ldo2_vrset_END (2)
#define PMIC_LDO2_CTRL_ldo2_ocp_enn_START (3)
#define PMIC_LDO2_CTRL_ldo2_ocp_enn_END (3)
#define PMIC_LDO2_CTRL_ldo2_eco_set_START (4)
#define PMIC_LDO2_CTRL_ldo2_eco_set_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo2_enable_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_LDO2_EN_CTRL_UNION;
#endif
#define PMIC_LDO2_EN_CTRL_ldo2_enable_sel_START (0)
#define PMIC_LDO2_EN_CTRL_ldo2_enable_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_ldo39_en : 1;
        unsigned char st_ldo39_en : 1;
        unsigned char reserved_0 : 2;
        unsigned char reg_ldo39_eco_en : 1;
        unsigned char st_ldo39_eco_en : 1;
        unsigned char reserved_1 : 2;
    } reg;
} PMIC_LDO39_ONOFF_ECO_UNION;
#endif
#define PMIC_LDO39_ONOFF_ECO_reg_ldo39_en_START (0)
#define PMIC_LDO39_ONOFF_ECO_reg_ldo39_en_END (0)
#define PMIC_LDO39_ONOFF_ECO_st_ldo39_en_START (1)
#define PMIC_LDO39_ONOFF_ECO_st_ldo39_en_END (1)
#define PMIC_LDO39_ONOFF_ECO_reg_ldo39_eco_en_START (4)
#define PMIC_LDO39_ONOFF_ECO_reg_ldo39_eco_en_END (4)
#define PMIC_LDO39_ONOFF_ECO_st_ldo39_eco_en_START (5)
#define PMIC_LDO39_ONOFF_ECO_st_ldo39_eco_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo39_vset_cfg : 6;
        unsigned char reserved : 2;
    } reg;
} PMIC_LDO39_VSET_UNION;
#endif
#define PMIC_LDO39_VSET_ldo39_vset_cfg_START (0)
#define PMIC_LDO39_VSET_ldo39_vset_cfg_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo39_vrset : 3;
        unsigned char ldo39_ocp_enn : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO39_CTRL0_UNION;
#endif
#define PMIC_LDO39_CTRL0_ldo39_vrset_START (0)
#define PMIC_LDO39_CTRL0_ldo39_vrset_END (2)
#define PMIC_LDO39_CTRL0_ldo39_ocp_enn_START (3)
#define PMIC_LDO39_CTRL0_ldo39_ocp_enn_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo39_ocps_en : 1;
        unsigned char ldo39_ocpd_vos_set : 1;
        unsigned char ldo39_bw_en : 1;
        unsigned char ldo39_vgpr_en : 1;
        unsigned char ldo39_pd_set : 1;
        unsigned char ldo39_comp : 2;
        unsigned char ldo39_bypass_ctrl_enn : 1;
    } reg;
} PMIC_LDO39_CTRL1_UNION;
#endif
#define PMIC_LDO39_CTRL1_ldo39_ocps_en_START (0)
#define PMIC_LDO39_CTRL1_ldo39_ocps_en_END (0)
#define PMIC_LDO39_CTRL1_ldo39_ocpd_vos_set_START (1)
#define PMIC_LDO39_CTRL1_ldo39_ocpd_vos_set_END (1)
#define PMIC_LDO39_CTRL1_ldo39_bw_en_START (2)
#define PMIC_LDO39_CTRL1_ldo39_bw_en_END (2)
#define PMIC_LDO39_CTRL1_ldo39_vgpr_en_START (3)
#define PMIC_LDO39_CTRL1_ldo39_vgpr_en_END (3)
#define PMIC_LDO39_CTRL1_ldo39_pd_set_START (4)
#define PMIC_LDO39_CTRL1_ldo39_pd_set_END (4)
#define PMIC_LDO39_CTRL1_ldo39_comp_START (5)
#define PMIC_LDO39_CTRL1_ldo39_comp_END (6)
#define PMIC_LDO39_CTRL1_ldo39_bypass_ctrl_enn_START (7)
#define PMIC_LDO39_CTRL1_ldo39_bypass_ctrl_enn_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo39_ocpd_set : 2;
        unsigned char ldo39_ocps_set : 2;
        unsigned char reserved : 4;
    } reg;
} PMIC_LDO39_CTRL2_UNION;
#endif
#define PMIC_LDO39_CTRL2_ldo39_ocpd_set_START (0)
#define PMIC_LDO39_CTRL2_ldo39_ocpd_set_END (1)
#define PMIC_LDO39_CTRL2_ldo39_ocps_set_START (2)
#define PMIC_LDO39_CTRL2_ldo39_ocps_set_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo39_rampdown_ctrl : 3;
        unsigned char ldo39_rampup_ctrl : 3;
        unsigned char ldo39_ramp_en_cfg : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_RAMP_LDO39_CTRL0_UNION;
#endif
#define PMIC_RAMP_LDO39_CTRL0_ldo39_rampdown_ctrl_START (0)
#define PMIC_RAMP_LDO39_CTRL0_ldo39_rampdown_ctrl_END (2)
#define PMIC_RAMP_LDO39_CTRL0_ldo39_rampup_ctrl_START (3)
#define PMIC_RAMP_LDO39_CTRL0_ldo39_rampup_ctrl_END (5)
#define PMIC_RAMP_LDO39_CTRL0_ldo39_ramp_en_cfg_START (6)
#define PMIC_RAMP_LDO39_CTRL0_ldo39_ramp_en_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ldo39_pull_down_off_cfg : 3;
        unsigned char ldo39_pull_down_on_cfg : 3;
        unsigned char ldo39_pull_down_cfg : 1;
        unsigned char ldo39_pull_down_mode : 1;
    } reg;
} PMIC_RAMP_LDO39_CTRL1_UNION;
#endif
#define PMIC_RAMP_LDO39_CTRL1_ldo39_pull_down_off_cfg_START (0)
#define PMIC_RAMP_LDO39_CTRL1_ldo39_pull_down_off_cfg_END (2)
#define PMIC_RAMP_LDO39_CTRL1_ldo39_pull_down_on_cfg_START (3)
#define PMIC_RAMP_LDO39_CTRL1_ldo39_pull_down_on_cfg_END (5)
#define PMIC_RAMP_LDO39_CTRL1_ldo39_pull_down_cfg_START (6)
#define PMIC_RAMP_LDO39_CTRL1_ldo39_pull_down_cfg_END (6)
#define PMIC_RAMP_LDO39_CTRL1_ldo39_pull_down_mode_START (7)
#define PMIC_RAMP_LDO39_CTRL1_ldo39_pull_down_mode_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ser_sidekey_mode_sel : 1;
        unsigned char ser_sidekey_time : 2;
        unsigned char reserved : 5;
    } reg;
} PMIC_SER_SIDEKEY_MODE_UNION;
#endif
#define PMIC_SER_SIDEKEY_MODE_ser_sidekey_mode_sel_START (0)
#define PMIC_SER_SIDEKEY_MODE_ser_sidekey_mode_sel_END (0)
#define PMIC_SER_SIDEKEY_MODE_ser_sidekey_time_START (1)
#define PMIC_SER_SIDEKEY_MODE_ser_sidekey_time_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc_chanl_sel : 5;
        unsigned char reserved : 2;
        unsigned char hkadc_bypass : 1;
    } reg;
} PMIC_ADC_CTRL_UNION;
#endif
#define PMIC_ADC_CTRL_hkadc_chanl_sel_START (0)
#define PMIC_ADC_CTRL_hkadc_chanl_sel_END (4)
#define PMIC_ADC_CTRL_hkadc_bypass_START (7)
#define PMIC_ADC_CTRL_hkadc_bypass_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc_start : 1;
        unsigned char hkadc_reserve : 7;
    } reg;
} PMIC_ADC_START_UNION;
#endif
#define PMIC_ADC_START_hkadc_start_START (0)
#define PMIC_ADC_START_hkadc_start_END (0)
#define PMIC_ADC_START_hkadc_reserve_START (1)
#define PMIC_ADC_START_hkadc_reserve_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc_valid : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_CONV_STATUS_UNION;
#endif
#define PMIC_CONV_STATUS_hkadc_valid_START (0)
#define PMIC_CONV_STATUS_hkadc_valid_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc_data11_4 : 8;
    } reg;
} PMIC_ADC_DATA1_UNION;
#endif
#define PMIC_ADC_DATA1_hkadc_data11_4_START (0)
#define PMIC_ADC_DATA1_hkadc_data11_4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 4;
        unsigned char hkadc_data3_0 : 4;
    } reg;
} PMIC_ADC_DATA0_UNION;
#endif
#define PMIC_ADC_DATA0_hkadc_data3_0_START (4)
#define PMIC_ADC_DATA0_hkadc_data3_0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc_buffer_sel : 1;
        unsigned char hkadc_config : 7;
    } reg;
} PMIC_ADC_CONV_UNION;
#endif
#define PMIC_ADC_CONV_hkadc_buffer_sel_START (0)
#define PMIC_ADC_CONV_hkadc_buffer_sel_END (0)
#define PMIC_ADC_CONV_hkadc_config_START (1)
#define PMIC_ADC_CONV_hkadc_config_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc_ibias_sel : 8;
    } reg;
} PMIC_ADC_CURRENT_UNION;
#endif
#define PMIC_ADC_CURRENT_hkadc_ibias_sel_START (0)
#define PMIC_ADC_CURRENT_hkadc_ibias_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc_cali_en : 1;
        unsigned char hkadc_cali_sel : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_ADC_CALI_CTRL_UNION;
#endif
#define PMIC_ADC_CALI_CTRL_hkadc_cali_en_START (0)
#define PMIC_ADC_CALI_CTRL_hkadc_cali_en_END (0)
#define PMIC_ADC_CALI_CTRL_hkadc_cali_sel_START (1)
#define PMIC_ADC_CALI_CTRL_hkadc_cali_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc_cali_data : 8;
    } reg;
} PMIC_ADC_CALI_VALUE_UNION;
#endif
#define PMIC_ADC_CALI_VALUE_hkadc_cali_data_START (0)
#define PMIC_ADC_CALI_VALUE_hkadc_cali_data_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc_cali_cfg : 8;
    } reg;
} PMIC_ADC_CALI_CFG_UNION;
#endif
#define PMIC_ADC_CALI_CFG_hkadc_cali_cfg_START (0)
#define PMIC_ADC_CALI_CFG_hkadc_cali_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc_chopper_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_ADC_MODE_CFG_UNION;
#endif
#define PMIC_ADC_MODE_CFG_hkadc_chopper_en_START (0)
#define PMIC_ADC_MODE_CFG_hkadc_chopper_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc_chopper_1st_data11_4 : 8;
    } reg;
} PMIC_ADC_CHOPPER_1ST_DATA1_UNION;
#endif
#define PMIC_ADC_CHOPPER_1ST_DATA1_hkadc_chopper_1st_data11_4_START (0)
#define PMIC_ADC_CHOPPER_1ST_DATA1_hkadc_chopper_1st_data11_4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 4;
        unsigned char hkadc_chopper_1st_data3_0 : 4;
    } reg;
} PMIC_ADC_CHOPPER_1ST_DATA2_UNION;
#endif
#define PMIC_ADC_CHOPPER_1ST_DATA2_hkadc_chopper_1st_data3_0_START (4)
#define PMIC_ADC_CHOPPER_1ST_DATA2_hkadc_chopper_1st_data3_0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc_chopper_2nd_data11_4 : 8;
    } reg;
} PMIC_ADC_CHOPPER_2ND_DATA1_UNION;
#endif
#define PMIC_ADC_CHOPPER_2ND_DATA1_hkadc_chopper_2nd_data11_4_START (0)
#define PMIC_ADC_CHOPPER_2ND_DATA1_hkadc_chopper_2nd_data11_4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 4;
        unsigned char hkadc_chopper_2nd_data3_0 : 4;
    } reg;
} PMIC_ADC_CHOPPER_2ND_DATA2_UNION;
#endif
#define PMIC_ADC_CHOPPER_2ND_DATA2_hkadc_chopper_2nd_data3_0_START (4)
#define PMIC_ADC_CHOPPER_2ND_DATA2_hkadc_chopper_2nd_data3_0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc_cali_offset_cfg11_4 : 8;
    } reg;
} PMIC_ADC_CALIVALUE_CFG1_UNION;
#endif
#define PMIC_ADC_CALIVALUE_CFG1_hkadc_cali_offset_cfg11_4_START (0)
#define PMIC_ADC_CALIVALUE_CFG1_hkadc_cali_offset_cfg11_4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 4;
        unsigned char hkadc_cali_offset_cfg3_0 : 4;
    } reg;
} PMIC_ADC_CALIVALUE_CFG2_UNION;
#endif
#define PMIC_ADC_CALIVALUE_CFG2_hkadc_cali_offset_cfg3_0_START (4)
#define PMIC_ADC_CALIVALUE_CFG2_hkadc_cali_offset_cfg3_0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_gate_int : 1;
        unsigned char cl_out_int : 1;
        unsigned char cl_in_int : 1;
        unsigned char v_gate_int : 1;
        unsigned char i_in_int : 1;
        unsigned char i_out_int : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_COUL_IRQ_UNION;
#endif
#define PMIC_COUL_IRQ_cl_gate_int_START (0)
#define PMIC_COUL_IRQ_cl_gate_int_END (0)
#define PMIC_COUL_IRQ_cl_out_int_START (1)
#define PMIC_COUL_IRQ_cl_out_int_END (1)
#define PMIC_COUL_IRQ_cl_in_int_START (2)
#define PMIC_COUL_IRQ_cl_in_int_END (2)
#define PMIC_COUL_IRQ_v_gate_int_START (3)
#define PMIC_COUL_IRQ_v_gate_int_END (3)
#define PMIC_COUL_IRQ_i_in_int_START (4)
#define PMIC_COUL_IRQ_i_in_int_END (4)
#define PMIC_COUL_IRQ_i_out_int_START (5)
#define PMIC_COUL_IRQ_i_out_int_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_gate_int_mk : 1;
        unsigned char cl_out_int_mk : 1;
        unsigned char cl_in_int_mk : 1;
        unsigned char v_gate_int_mk : 1;
        unsigned char i_in_int_mk : 1;
        unsigned char i_out_int_mk : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_COUL_IRQ_MASK_UNION;
#endif
#define PMIC_COUL_IRQ_MASK_cl_gate_int_mk_START (0)
#define PMIC_COUL_IRQ_MASK_cl_gate_int_mk_END (0)
#define PMIC_COUL_IRQ_MASK_cl_out_int_mk_START (1)
#define PMIC_COUL_IRQ_MASK_cl_out_int_mk_END (1)
#define PMIC_COUL_IRQ_MASK_cl_in_int_mk_START (2)
#define PMIC_COUL_IRQ_MASK_cl_in_int_mk_END (2)
#define PMIC_COUL_IRQ_MASK_v_gate_int_mk_START (3)
#define PMIC_COUL_IRQ_MASK_v_gate_int_mk_END (3)
#define PMIC_COUL_IRQ_MASK_i_in_int_mk_START (4)
#define PMIC_COUL_IRQ_MASK_i_in_int_mk_END (4)
#define PMIC_COUL_IRQ_MASK_i_out_int_mk_START (5)
#define PMIC_COUL_IRQ_MASK_i_out_int_mk_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char np_i_in_s : 1;
        unsigned char np_i_out_s : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_COUL_IRQ_NP_UNION;
#endif
#define PMIC_COUL_IRQ_NP_np_i_in_s_START (0)
#define PMIC_COUL_IRQ_NP_np_i_in_s_END (0)
#define PMIC_COUL_IRQ_NP_np_i_out_s_START (1)
#define PMIC_COUL_IRQ_NP_np_i_out_s_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_ctrl : 3;
        unsigned char reflash_value_ctrl : 1;
        unsigned char eco_filter_time : 2;
        unsigned char reserved : 1;
        unsigned char calibration_ctrl : 1;
    } reg;
} PMIC_CLJ_CTRL_REG_UNION;
#endif
#define PMIC_CLJ_CTRL_REG_eco_ctrl_START (0)
#define PMIC_CLJ_CTRL_REG_eco_ctrl_END (2)
#define PMIC_CLJ_CTRL_REG_reflash_value_ctrl_START (3)
#define PMIC_CLJ_CTRL_REG_reflash_value_ctrl_END (3)
#define PMIC_CLJ_CTRL_REG_eco_filter_time_START (4)
#define PMIC_CLJ_CTRL_REG_eco_filter_time_END (5)
#define PMIC_CLJ_CTRL_REG_calibration_ctrl_START (7)
#define PMIC_CLJ_CTRL_REG_calibration_ctrl_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char pd_ocv_i_onoff : 1;
        unsigned char reg_data_clr : 1;
        unsigned char cali_auto_time : 2;
        unsigned char cali_auto_onoff_ctrl : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_CLJ_CTRL_REGS2_UNION;
#endif
#define PMIC_CLJ_CTRL_REGS2_pd_ocv_i_onoff_START (0)
#define PMIC_CLJ_CTRL_REGS2_pd_ocv_i_onoff_END (0)
#define PMIC_CLJ_CTRL_REGS2_reg_data_clr_START (1)
#define PMIC_CLJ_CTRL_REGS2_reg_data_clr_END (1)
#define PMIC_CLJ_CTRL_REGS2_cali_auto_time_START (2)
#define PMIC_CLJ_CTRL_REGS2_cali_auto_time_END (3)
#define PMIC_CLJ_CTRL_REGS2_cali_auto_onoff_ctrl_START (4)
#define PMIC_CLJ_CTRL_REGS2_cali_auto_onoff_ctrl_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_delay_en : 1;
        unsigned char coul_eco_dly_sel : 2;
        unsigned char wait_comp_en : 1;
        unsigned char wait_comp_sel : 2;
        unsigned char reserved : 2;
    } reg;
} PMIC_CLJ_CTRL_REGS3_UNION;
#endif
#define PMIC_CLJ_CTRL_REGS3_eco_delay_en_START (0)
#define PMIC_CLJ_CTRL_REGS3_eco_delay_en_END (0)
#define PMIC_CLJ_CTRL_REGS3_coul_eco_dly_sel_START (1)
#define PMIC_CLJ_CTRL_REGS3_coul_eco_dly_sel_END (2)
#define PMIC_CLJ_CTRL_REGS3_wait_comp_en_START (3)
#define PMIC_CLJ_CTRL_REGS3_wait_comp_en_END (3)
#define PMIC_CLJ_CTRL_REGS3_wait_comp_sel_START (4)
#define PMIC_CLJ_CTRL_REGS3_wait_comp_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char data_eco_en : 1;
        unsigned char eco_data_clr : 1;
        unsigned char coul_charge_en : 1;
        unsigned char coul_charge_flag : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_CLJ_CTRL_REGS4_UNION;
#endif
#define PMIC_CLJ_CTRL_REGS4_data_eco_en_START (0)
#define PMIC_CLJ_CTRL_REGS4_data_eco_en_END (0)
#define PMIC_CLJ_CTRL_REGS4_eco_data_clr_START (1)
#define PMIC_CLJ_CTRL_REGS4_eco_data_clr_END (1)
#define PMIC_CLJ_CTRL_REGS4_coul_charge_en_START (2)
#define PMIC_CLJ_CTRL_REGS4_coul_charge_en_END (2)
#define PMIC_CLJ_CTRL_REGS4_coul_charge_flag_START (3)
#define PMIC_CLJ_CTRL_REGS4_coul_charge_flag_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_en : 1;
        unsigned char slide_refresh_en : 1;
        unsigned char slide_cnt_cfg : 2;
        unsigned char slide_ready : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_CLJ_CTRL_REGS5_UNION;
#endif
#define PMIC_CLJ_CTRL_REGS5_slide_en_START (0)
#define PMIC_CLJ_CTRL_REGS5_slide_en_END (0)
#define PMIC_CLJ_CTRL_REGS5_slide_refresh_en_START (1)
#define PMIC_CLJ_CTRL_REGS5_slide_refresh_en_END (1)
#define PMIC_CLJ_CTRL_REGS5_slide_cnt_cfg_START (2)
#define PMIC_CLJ_CTRL_REGS5_slide_cnt_cfg_END (3)
#define PMIC_CLJ_CTRL_REGS5_slide_ready_START (4)
#define PMIC_CLJ_CTRL_REGS5_slide_ready_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ocv_average_sel : 1;
        unsigned char coul_average_sel : 1;
        unsigned char coul_data_rate_sel : 1;
        unsigned char debug_refresh_en : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_CIC_CTRL_UNION;
#endif
#define PMIC_CIC_CTRL_ocv_average_sel_START (0)
#define PMIC_CIC_CTRL_ocv_average_sel_END (0)
#define PMIC_CIC_CTRL_coul_average_sel_START (1)
#define PMIC_CIC_CTRL_coul_average_sel_END (1)
#define PMIC_CIC_CTRL_coul_data_rate_sel_START (2)
#define PMIC_CIC_CTRL_coul_data_rate_sel_END (2)
#define PMIC_CIC_CTRL_debug_refresh_en_START (3)
#define PMIC_CIC_CTRL_debug_refresh_en_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char temp_en : 1;
        unsigned char temp_rdy : 1;
        unsigned char vout_rdy : 1;
        unsigned char coul_vin_sel_st : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_COUL_TEMP_CTRL_UNION;
#endif
#define PMIC_COUL_TEMP_CTRL_temp_en_START (0)
#define PMIC_COUL_TEMP_CTRL_temp_en_END (0)
#define PMIC_COUL_TEMP_CTRL_temp_rdy_START (1)
#define PMIC_COUL_TEMP_CTRL_temp_rdy_END (1)
#define PMIC_COUL_TEMP_CTRL_vout_rdy_START (2)
#define PMIC_COUL_TEMP_CTRL_vout_rdy_END (2)
#define PMIC_COUL_TEMP_CTRL_coul_vin_sel_st_START (3)
#define PMIC_COUL_TEMP_CTRL_coul_vin_sel_st_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char coul_v_decode : 2;
        unsigned char coul_i_decode : 2;
        unsigned char coul_v_pga_gain : 1;
        unsigned char coul_i_pga_gain : 2;
        unsigned char reserved : 1;
    } reg;
} PMIC_COUL_VI_CTRL0_UNION;
#endif
#define PMIC_COUL_VI_CTRL0_coul_v_decode_START (0)
#define PMIC_COUL_VI_CTRL0_coul_v_decode_END (1)
#define PMIC_COUL_VI_CTRL0_coul_i_decode_START (2)
#define PMIC_COUL_VI_CTRL0_coul_i_decode_END (3)
#define PMIC_COUL_VI_CTRL0_coul_v_pga_gain_START (4)
#define PMIC_COUL_VI_CTRL0_coul_v_pga_gain_END (4)
#define PMIC_COUL_VI_CTRL0_coul_i_pga_gain_START (5)
#define PMIC_COUL_VI_CTRL0_coul_i_pga_gain_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char coul_v_chop_enb : 2;
        unsigned char coul_i_chop_enb : 2;
        unsigned char coul_v_alias_en : 1;
        unsigned char coul_i_alias_en : 1;
        unsigned char coul_v_comp_sel : 1;
        unsigned char coul_i_comp_sel : 1;
    } reg;
} PMIC_COUL_VI_CTRL1_UNION;
#endif
#define PMIC_COUL_VI_CTRL1_coul_v_chop_enb_START (0)
#define PMIC_COUL_VI_CTRL1_coul_v_chop_enb_END (1)
#define PMIC_COUL_VI_CTRL1_coul_i_chop_enb_START (2)
#define PMIC_COUL_VI_CTRL1_coul_i_chop_enb_END (3)
#define PMIC_COUL_VI_CTRL1_coul_v_alias_en_START (4)
#define PMIC_COUL_VI_CTRL1_coul_v_alias_en_END (4)
#define PMIC_COUL_VI_CTRL1_coul_i_alias_en_START (5)
#define PMIC_COUL_VI_CTRL1_coul_i_alias_en_END (5)
#define PMIC_COUL_VI_CTRL1_coul_v_comp_sel_START (6)
#define PMIC_COUL_VI_CTRL1_coul_v_comp_sel_END (6)
#define PMIC_COUL_VI_CTRL1_coul_i_comp_sel_START (7)
#define PMIC_COUL_VI_CTRL1_coul_i_comp_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char coul_v_chop_freq_sel : 4;
        unsigned char coul_i_chop_freq_sel : 4;
    } reg;
} PMIC_COUL_VI_CTRL2_UNION;
#endif
#define PMIC_COUL_VI_CTRL2_coul_v_chop_freq_sel_START (0)
#define PMIC_COUL_VI_CTRL2_coul_v_chop_freq_sel_END (3)
#define PMIC_COUL_VI_CTRL2_coul_i_chop_freq_sel_START (4)
#define PMIC_COUL_VI_CTRL2_coul_i_chop_freq_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char coul_i_system_chop_enb : 1;
        unsigned char coul_v_bias_buff : 2;
        unsigned char coul_i_bias_buff : 2;
        unsigned char coul_dt_ctrl : 2;
        unsigned char reserved : 1;
    } reg;
} PMIC_COUL_VI_CTRL3_UNION;
#endif
#define PMIC_COUL_VI_CTRL3_coul_i_system_chop_enb_START (0)
#define PMIC_COUL_VI_CTRL3_coul_i_system_chop_enb_END (0)
#define PMIC_COUL_VI_CTRL3_coul_v_bias_buff_START (1)
#define PMIC_COUL_VI_CTRL3_coul_v_bias_buff_END (2)
#define PMIC_COUL_VI_CTRL3_coul_i_bias_buff_START (3)
#define PMIC_COUL_VI_CTRL3_coul_i_bias_buff_END (4)
#define PMIC_COUL_VI_CTRL3_coul_dt_ctrl_START (5)
#define PMIC_COUL_VI_CTRL3_coul_dt_ctrl_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char coul_i_bias : 8;
    } reg;
} PMIC_COUL_VI_CTRL4_UNION;
#endif
#define PMIC_COUL_VI_CTRL4_coul_i_bias_START (0)
#define PMIC_COUL_VI_CTRL4_coul_i_bias_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char coul_v_bias : 8;
    } reg;
} PMIC_COUL_VI_CTRL5_UNION;
#endif
#define PMIC_COUL_VI_CTRL5_coul_v_bias_START (0)
#define PMIC_COUL_VI_CTRL5_coul_v_bias_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_out_s0 : 8;
    } reg;
} PMIC_CL_OUT0_UNION;
#endif
#define PMIC_CL_OUT0_cl_out_s0_START (0)
#define PMIC_CL_OUT0_cl_out_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_out_s1 : 8;
    } reg;
} PMIC_CL_OUT1_UNION;
#endif
#define PMIC_CL_OUT1_cl_out_s1_START (0)
#define PMIC_CL_OUT1_cl_out_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_out_s2 : 8;
    } reg;
} PMIC_CL_OUT2_UNION;
#endif
#define PMIC_CL_OUT2_cl_out_s2_START (0)
#define PMIC_CL_OUT2_cl_out_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_out_s3 : 8;
    } reg;
} PMIC_CL_OUT3_UNION;
#endif
#define PMIC_CL_OUT3_cl_out_s3_START (0)
#define PMIC_CL_OUT3_cl_out_s3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_out_s4 : 8;
    } reg;
} PMIC_CL_OUT4_UNION;
#endif
#define PMIC_CL_OUT4_cl_out_s4_START (0)
#define PMIC_CL_OUT4_cl_out_s4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_in_s0 : 8;
    } reg;
} PMIC_CL_IN0_UNION;
#endif
#define PMIC_CL_IN0_cl_in_s0_START (0)
#define PMIC_CL_IN0_cl_in_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_in_s1 : 8;
    } reg;
} PMIC_CL_IN1_UNION;
#endif
#define PMIC_CL_IN1_cl_in_s1_START (0)
#define PMIC_CL_IN1_cl_in_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_in_s2 : 8;
    } reg;
} PMIC_CL_IN2_UNION;
#endif
#define PMIC_CL_IN2_cl_in_s2_START (0)
#define PMIC_CL_IN2_cl_in_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_in_s3 : 8;
    } reg;
} PMIC_CL_IN3_UNION;
#endif
#define PMIC_CL_IN3_cl_in_s3_START (0)
#define PMIC_CL_IN3_cl_in_s3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_in_s4 : 8;
    } reg;
} PMIC_CL_IN4_UNION;
#endif
#define PMIC_CL_IN4_cl_in_s4_START (0)
#define PMIC_CL_IN4_cl_in_s4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char chg_timer_s0 : 8;
    } reg;
} PMIC_CHG_TIMER0_UNION;
#endif
#define PMIC_CHG_TIMER0_chg_timer_s0_START (0)
#define PMIC_CHG_TIMER0_chg_timer_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char chg_timer_s1 : 8;
    } reg;
} PMIC_CHG_TIMER1_UNION;
#endif
#define PMIC_CHG_TIMER1_chg_timer_s1_START (0)
#define PMIC_CHG_TIMER1_chg_timer_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char chg_timer_s2 : 8;
    } reg;
} PMIC_CHG_TIMER2_UNION;
#endif
#define PMIC_CHG_TIMER2_chg_timer_s2_START (0)
#define PMIC_CHG_TIMER2_chg_timer_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char chg_timer_s3 : 8;
    } reg;
} PMIC_CHG_TIMER3_UNION;
#endif
#define PMIC_CHG_TIMER3_chg_timer_s3_START (0)
#define PMIC_CHG_TIMER3_chg_timer_s3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char load_timer_s0 : 8;
    } reg;
} PMIC_LOAD_TIMER0_UNION;
#endif
#define PMIC_LOAD_TIMER0_load_timer_s0_START (0)
#define PMIC_LOAD_TIMER0_load_timer_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char load_timer_s1 : 8;
    } reg;
} PMIC_LOAD_TIMER1_UNION;
#endif
#define PMIC_LOAD_TIMER1_load_timer_s1_START (0)
#define PMIC_LOAD_TIMER1_load_timer_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char load_timer_s2 : 8;
    } reg;
} PMIC_LOAD_TIMER2_UNION;
#endif
#define PMIC_LOAD_TIMER2_load_timer_s2_START (0)
#define PMIC_LOAD_TIMER2_load_timer_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char load_timer_s3 : 8;
    } reg;
} PMIC_LOAD_TIMER3_UNION;
#endif
#define PMIC_LOAD_TIMER3_load_timer_s3_START (0)
#define PMIC_LOAD_TIMER3_load_timer_s3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_int_s0 : 8;
    } reg;
} PMIC_CL_INT0_UNION;
#endif
#define PMIC_CL_INT0_cl_int_s0_START (0)
#define PMIC_CL_INT0_cl_int_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_int_s1 : 8;
    } reg;
} PMIC_CL_INT1_UNION;
#endif
#define PMIC_CL_INT1_cl_int_s1_START (0)
#define PMIC_CL_INT1_cl_int_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_int_s2 : 8;
    } reg;
} PMIC_CL_INT2_UNION;
#endif
#define PMIC_CL_INT2_cl_int_s2_START (0)
#define PMIC_CL_INT2_cl_int_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_int_s3 : 8;
    } reg;
} PMIC_CL_INT3_UNION;
#endif
#define PMIC_CL_INT3_cl_int_s3_START (0)
#define PMIC_CL_INT3_cl_int_s3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char cl_int_s4 : 8;
    } reg;
} PMIC_CL_INT4_UNION;
#endif
#define PMIC_CL_INT4_cl_int_s4_START (0)
#define PMIC_CL_INT4_cl_int_s4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_int_s0 : 8;
    } reg;
} PMIC_V_INT0_UNION;
#endif
#define PMIC_V_INT0_v_int_s0_START (0)
#define PMIC_V_INT0_v_int_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_int_s1 : 8;
    } reg;
} PMIC_V_INT1_UNION;
#endif
#define PMIC_V_INT1_v_int_s1_START (0)
#define PMIC_V_INT1_v_int_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_int_s2 : 8;
    } reg;
} PMIC_V_INT2_UNION;
#endif
#define PMIC_V_INT2_v_int_s2_START (0)
#define PMIC_V_INT2_v_int_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_out_gate_s0 : 8;
    } reg;
} PMIC_I_OUT_GATE0_UNION;
#endif
#define PMIC_I_OUT_GATE0_i_out_gate_s0_START (0)
#define PMIC_I_OUT_GATE0_i_out_gate_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_out_gate_s1 : 8;
    } reg;
} PMIC_I_OUT_GATE1_UNION;
#endif
#define PMIC_I_OUT_GATE1_i_out_gate_s1_START (0)
#define PMIC_I_OUT_GATE1_i_out_gate_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_out_gate_s2 : 8;
    } reg;
} PMIC_I_OUT_GATE2_UNION;
#endif
#define PMIC_I_OUT_GATE2_i_out_gate_s2_START (0)
#define PMIC_I_OUT_GATE2_i_out_gate_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_in_gate_s0 : 8;
    } reg;
} PMIC_I_IN_GATE0_UNION;
#endif
#define PMIC_I_IN_GATE0_i_in_gate_s0_START (0)
#define PMIC_I_IN_GATE0_i_in_gate_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_in_gate_s1 : 8;
    } reg;
} PMIC_I_IN_GATE1_UNION;
#endif
#define PMIC_I_IN_GATE1_i_in_gate_s1_START (0)
#define PMIC_I_IN_GATE1_i_in_gate_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_in_gate_s2 : 8;
    } reg;
} PMIC_I_IN_GATE2_UNION;
#endif
#define PMIC_I_IN_GATE2_i_in_gate_s2_START (0)
#define PMIC_I_IN_GATE2_i_in_gate_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char offset_current_mod_s0 : 8;
    } reg;
} PMIC_OFFSET_CURRENT_MOD_0_UNION;
#endif
#define PMIC_OFFSET_CURRENT_MOD_0_offset_current_mod_s0_START (0)
#define PMIC_OFFSET_CURRENT_MOD_0_offset_current_mod_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char offset_current_mod_s1 : 8;
    } reg;
} PMIC_OFFSET_CURRENT_MOD_1_UNION;
#endif
#define PMIC_OFFSET_CURRENT_MOD_1_offset_current_mod_s1_START (0)
#define PMIC_OFFSET_CURRENT_MOD_1_offset_current_mod_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char offset_current_mod_s2 : 8;
    } reg;
} PMIC_OFFSET_CURRENT_MOD_2_UNION;
#endif
#define PMIC_OFFSET_CURRENT_MOD_2_offset_current_mod_s2_START (0)
#define PMIC_OFFSET_CURRENT_MOD_2_offset_current_mod_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char offset_voltage_mod_s0 : 8;
    } reg;
} PMIC_OFFSET_VOLTAGE_MOD_0_UNION;
#endif
#define PMIC_OFFSET_VOLTAGE_MOD_0_offset_voltage_mod_s0_START (0)
#define PMIC_OFFSET_VOLTAGE_MOD_0_offset_voltage_mod_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char offset_voltage_mod_s1 : 8;
    } reg;
} PMIC_OFFSET_VOLTAGE_MOD_1_UNION;
#endif
#define PMIC_OFFSET_VOLTAGE_MOD_1_offset_voltage_mod_s1_START (0)
#define PMIC_OFFSET_VOLTAGE_MOD_1_offset_voltage_mod_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char offset_voltage_mod_s2 : 8;
    } reg;
} PMIC_OFFSET_VOLTAGE_MOD_2_UNION;
#endif
#define PMIC_OFFSET_VOLTAGE_MOD_2_offset_voltage_mod_s2_START (0)
#define PMIC_OFFSET_VOLTAGE_MOD_2_offset_voltage_mod_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char mstate : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_STATE_TEST_UNION;
#endif
#define PMIC_STATE_TEST_mstate_START (0)
#define PMIC_STATE_TEST_mstate_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_s0 : 8;
    } reg;
} PMIC_CURRENT_0_UNION;
#endif
#define PMIC_CURRENT_0_current_s0_START (0)
#define PMIC_CURRENT_0_current_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_s1 : 8;
    } reg;
} PMIC_CURRENT_1_UNION;
#endif
#define PMIC_CURRENT_1_current_s1_START (0)
#define PMIC_CURRENT_1_current_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_s2 : 8;
    } reg;
} PMIC_CURRENT_2_UNION;
#endif
#define PMIC_CURRENT_2_current_s2_START (0)
#define PMIC_CURRENT_2_current_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_s0 : 8;
    } reg;
} PMIC_V_OUT_0_UNION;
#endif
#define PMIC_V_OUT_0_v_out_s0_START (0)
#define PMIC_V_OUT_0_v_out_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_s1 : 8;
    } reg;
} PMIC_V_OUT_1_UNION;
#endif
#define PMIC_V_OUT_1_v_out_s1_START (0)
#define PMIC_V_OUT_1_v_out_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_s2 : 8;
    } reg;
} PMIC_V_OUT_2_UNION;
#endif
#define PMIC_V_OUT_2_v_out_s2_START (0)
#define PMIC_V_OUT_2_v_out_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char offset_current_s0 : 8;
    } reg;
} PMIC_OFFSET_CURRENT0_UNION;
#endif
#define PMIC_OFFSET_CURRENT0_offset_current_s0_START (0)
#define PMIC_OFFSET_CURRENT0_offset_current_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char offset_current_s1 : 8;
    } reg;
} PMIC_OFFSET_CURRENT1_UNION;
#endif
#define PMIC_OFFSET_CURRENT1_offset_current_s1_START (0)
#define PMIC_OFFSET_CURRENT1_offset_current_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char offset_current_s2 : 8;
    } reg;
} PMIC_OFFSET_CURRENT2_UNION;
#endif
#define PMIC_OFFSET_CURRENT2_offset_current_s2_START (0)
#define PMIC_OFFSET_CURRENT2_offset_current_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char offset_voltage_s0 : 8;
    } reg;
} PMIC_OFFSET_VOLTAGE0_UNION;
#endif
#define PMIC_OFFSET_VOLTAGE0_offset_voltage_s0_START (0)
#define PMIC_OFFSET_VOLTAGE0_offset_voltage_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char offset_voltage_s1 : 8;
    } reg;
} PMIC_OFFSET_VOLTAGE1_UNION;
#endif
#define PMIC_OFFSET_VOLTAGE1_offset_voltage_s1_START (0)
#define PMIC_OFFSET_VOLTAGE1_offset_voltage_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char offset_voltage_s2 : 8;
    } reg;
} PMIC_OFFSET_VOLTAGE2_UNION;
#endif
#define PMIC_OFFSET_VOLTAGE2_offset_voltage_s2_START (0)
#define PMIC_OFFSET_VOLTAGE2_offset_voltage_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_data_s0 : 8;
    } reg;
} PMIC_OCV_VOLTAGE0_UNION;
#endif
#define PMIC_OCV_VOLTAGE0_v_ocv_data_s0_START (0)
#define PMIC_OCV_VOLTAGE0_v_ocv_data_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_data_s1 : 8;
    } reg;
} PMIC_OCV_VOLTAGE1_UNION;
#endif
#define PMIC_OCV_VOLTAGE1_v_ocv_data_s1_START (0)
#define PMIC_OCV_VOLTAGE1_v_ocv_data_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_data_s2 : 8;
    } reg;
} PMIC_OCV_VOLTAGE2_UNION;
#endif
#define PMIC_OCV_VOLTAGE2_v_ocv_data_s2_START (0)
#define PMIC_OCV_VOLTAGE2_v_ocv_data_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_data_s0 : 8;
    } reg;
} PMIC_OCV_CURRENT0_UNION;
#endif
#define PMIC_OCV_CURRENT0_i_ocv_data_s0_START (0)
#define PMIC_OCV_CURRENT0_i_ocv_data_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_data_s1 : 8;
    } reg;
} PMIC_OCV_CURRENT1_UNION;
#endif
#define PMIC_OCV_CURRENT1_i_ocv_data_s1_START (0)
#define PMIC_OCV_CURRENT1_i_ocv_data_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_data_s2 : 8;
    } reg;
} PMIC_OCV_CURRENT2_UNION;
#endif
#define PMIC_OCV_CURRENT2_i_ocv_data_s2_START (0)
#define PMIC_OCV_CURRENT2_i_ocv_data_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char t_ocv_data_s0 : 8;
    } reg;
} PMIC_OCV_TEMP0_UNION;
#endif
#define PMIC_OCV_TEMP0_t_ocv_data_s0_START (0)
#define PMIC_OCV_TEMP0_t_ocv_data_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char t_ocv_data_s1 : 8;
    } reg;
} PMIC_OCV_TEMP1_UNION;
#endif
#define PMIC_OCV_TEMP1_t_ocv_data_s1_START (0)
#define PMIC_OCV_TEMP1_t_ocv_data_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char t_ocv_data_s2 : 8;
    } reg;
} PMIC_OCV_TEMP2_UNION;
#endif
#define PMIC_OCV_TEMP2_t_ocv_data_s2_START (0)
#define PMIC_OCV_TEMP2_t_ocv_data_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_out_clin_s0 : 8;
    } reg;
} PMIC_ECO_OUT_CLIN_0_UNION;
#endif
#define PMIC_ECO_OUT_CLIN_0_eco_out_clin_s0_START (0)
#define PMIC_ECO_OUT_CLIN_0_eco_out_clin_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_out_clin_s1 : 8;
    } reg;
} PMIC_ECO_OUT_CLIN_1_UNION;
#endif
#define PMIC_ECO_OUT_CLIN_1_eco_out_clin_s1_START (0)
#define PMIC_ECO_OUT_CLIN_1_eco_out_clin_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_out_clin_s2 : 8;
    } reg;
} PMIC_ECO_OUT_CLIN_2_UNION;
#endif
#define PMIC_ECO_OUT_CLIN_2_eco_out_clin_s2_START (0)
#define PMIC_ECO_OUT_CLIN_2_eco_out_clin_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_out_clin_s3 : 8;
    } reg;
} PMIC_ECO_OUT_CLIN_3_UNION;
#endif
#define PMIC_ECO_OUT_CLIN_3_eco_out_clin_s3_START (0)
#define PMIC_ECO_OUT_CLIN_3_eco_out_clin_s3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_out_clin_s4 : 8;
    } reg;
} PMIC_ECO_OUT_CLIN_4_UNION;
#endif
#define PMIC_ECO_OUT_CLIN_4_eco_out_clin_s4_START (0)
#define PMIC_ECO_OUT_CLIN_4_eco_out_clin_s4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_out_clout_s0 : 8;
    } reg;
} PMIC_ECO_OUT_CLOUT_0_UNION;
#endif
#define PMIC_ECO_OUT_CLOUT_0_eco_out_clout_s0_START (0)
#define PMIC_ECO_OUT_CLOUT_0_eco_out_clout_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_out_clout_s1 : 8;
    } reg;
} PMIC_ECO_OUT_CLOUT_1_UNION;
#endif
#define PMIC_ECO_OUT_CLOUT_1_eco_out_clout_s1_START (0)
#define PMIC_ECO_OUT_CLOUT_1_eco_out_clout_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_out_clout_s2 : 8;
    } reg;
} PMIC_ECO_OUT_CLOUT_2_UNION;
#endif
#define PMIC_ECO_OUT_CLOUT_2_eco_out_clout_s2_START (0)
#define PMIC_ECO_OUT_CLOUT_2_eco_out_clout_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_out_clout_s3 : 8;
    } reg;
} PMIC_ECO_OUT_CLOUT_3_UNION;
#endif
#define PMIC_ECO_OUT_CLOUT_3_eco_out_clout_s3_START (0)
#define PMIC_ECO_OUT_CLOUT_3_eco_out_clout_s3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_out_clout_s4 : 8;
    } reg;
} PMIC_ECO_OUT_CLOUT_4_UNION;
#endif
#define PMIC_ECO_OUT_CLOUT_4_eco_out_clout_s4_START (0)
#define PMIC_ECO_OUT_CLOUT_4_eco_out_clout_s4_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_out_temp_s0 : 8;
    } reg;
} PMIC_ECO_OUT_TEMP_0_UNION;
#endif
#define PMIC_ECO_OUT_TEMP_0_eco_out_temp_s0_START (0)
#define PMIC_ECO_OUT_TEMP_0_eco_out_temp_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_out_temp_s1 : 8;
    } reg;
} PMIC_ECO_OUT_TEMP_1_UNION;
#endif
#define PMIC_ECO_OUT_TEMP_1_eco_out_temp_s1_START (0)
#define PMIC_ECO_OUT_TEMP_1_eco_out_temp_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eco_out_temp_s2 : 8;
    } reg;
} PMIC_ECO_OUT_TEMP_2_UNION;
#endif
#define PMIC_ECO_OUT_TEMP_2_eco_out_temp_s2_START (0)
#define PMIC_ECO_OUT_TEMP_2_eco_out_temp_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char temp_rdata_s0 : 8;
    } reg;
} PMIC_TEMP0_RDATA_UNION;
#endif
#define PMIC_TEMP0_RDATA_temp_rdata_s0_START (0)
#define PMIC_TEMP0_RDATA_temp_rdata_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char temp_rdata_s1 : 8;
    } reg;
} PMIC_TEMP1_RDATA_UNION;
#endif
#define PMIC_TEMP1_RDATA_temp_rdata_s1_START (0)
#define PMIC_TEMP1_RDATA_temp_rdata_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char temp_rdata_s2 : 8;
    } reg;
} PMIC_TEMP2_RDATA_UNION;
#endif
#define PMIC_TEMP2_RDATA_temp_rdata_s2_START (0)
#define PMIC_TEMP2_RDATA_temp_rdata_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre0_s0 : 8;
    } reg;
} PMIC_V_PRE0_OUT0_UNION;
#endif
#define PMIC_V_PRE0_OUT0_v_out_pre0_s0_START (0)
#define PMIC_V_PRE0_OUT0_v_out_pre0_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre0_s1 : 8;
    } reg;
} PMIC_V_PRE0_OUT1_UNION;
#endif
#define PMIC_V_PRE0_OUT1_v_out_pre0_s1_START (0)
#define PMIC_V_PRE0_OUT1_v_out_pre0_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre0_s2 : 8;
    } reg;
} PMIC_V_PRE0_OUT2_UNION;
#endif
#define PMIC_V_PRE0_OUT2_v_out_pre0_s2_START (0)
#define PMIC_V_PRE0_OUT2_v_out_pre0_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre1_s0 : 8;
    } reg;
} PMIC_V_PRE1_OUT0_UNION;
#endif
#define PMIC_V_PRE1_OUT0_v_out_pre1_s0_START (0)
#define PMIC_V_PRE1_OUT0_v_out_pre1_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre1_s1 : 8;
    } reg;
} PMIC_V_PRE1_OUT1_UNION;
#endif
#define PMIC_V_PRE1_OUT1_v_out_pre1_s1_START (0)
#define PMIC_V_PRE1_OUT1_v_out_pre1_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre1_s2 : 8;
    } reg;
} PMIC_V_PRE1_OUT2_UNION;
#endif
#define PMIC_V_PRE1_OUT2_v_out_pre1_s2_START (0)
#define PMIC_V_PRE1_OUT2_v_out_pre1_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre2_s0 : 8;
    } reg;
} PMIC_V_PRE2_OUT0_UNION;
#endif
#define PMIC_V_PRE2_OUT0_v_out_pre2_s0_START (0)
#define PMIC_V_PRE2_OUT0_v_out_pre2_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre2_s1 : 8;
    } reg;
} PMIC_V_PRE2_OUT1_UNION;
#endif
#define PMIC_V_PRE2_OUT1_v_out_pre2_s1_START (0)
#define PMIC_V_PRE2_OUT1_v_out_pre2_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre2_s2 : 8;
    } reg;
} PMIC_V_PRE2_OUT2_UNION;
#endif
#define PMIC_V_PRE2_OUT2_v_out_pre2_s2_START (0)
#define PMIC_V_PRE2_OUT2_v_out_pre2_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre3_s0 : 8;
    } reg;
} PMIC_V_PRE3_OUT0_UNION;
#endif
#define PMIC_V_PRE3_OUT0_v_out_pre3_s0_START (0)
#define PMIC_V_PRE3_OUT0_v_out_pre3_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre3_s1 : 8;
    } reg;
} PMIC_V_PRE3_OUT1_UNION;
#endif
#define PMIC_V_PRE3_OUT1_v_out_pre3_s1_START (0)
#define PMIC_V_PRE3_OUT1_v_out_pre3_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre3_s2 : 8;
    } reg;
} PMIC_V_PRE3_OUT2_UNION;
#endif
#define PMIC_V_PRE3_OUT2_v_out_pre3_s2_START (0)
#define PMIC_V_PRE3_OUT2_v_out_pre3_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre4_s0 : 8;
    } reg;
} PMIC_V_PRE4_OUT0_UNION;
#endif
#define PMIC_V_PRE4_OUT0_v_out_pre4_s0_START (0)
#define PMIC_V_PRE4_OUT0_v_out_pre4_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre4_s1 : 8;
    } reg;
} PMIC_V_PRE4_OUT1_UNION;
#endif
#define PMIC_V_PRE4_OUT1_v_out_pre4_s1_START (0)
#define PMIC_V_PRE4_OUT1_v_out_pre4_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre4_s2 : 8;
    } reg;
} PMIC_V_PRE4_OUT2_UNION;
#endif
#define PMIC_V_PRE4_OUT2_v_out_pre4_s2_START (0)
#define PMIC_V_PRE4_OUT2_v_out_pre4_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre5_s0 : 8;
    } reg;
} PMIC_V_PRE5_OUT0_UNION;
#endif
#define PMIC_V_PRE5_OUT0_v_out_pre5_s0_START (0)
#define PMIC_V_PRE5_OUT0_v_out_pre5_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre5_s1 : 8;
    } reg;
} PMIC_V_PRE5_OUT1_UNION;
#endif
#define PMIC_V_PRE5_OUT1_v_out_pre5_s1_START (0)
#define PMIC_V_PRE5_OUT1_v_out_pre5_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre5_s2 : 8;
    } reg;
} PMIC_V_PRE5_OUT2_UNION;
#endif
#define PMIC_V_PRE5_OUT2_v_out_pre5_s2_START (0)
#define PMIC_V_PRE5_OUT2_v_out_pre5_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre6_s0 : 8;
    } reg;
} PMIC_V_PRE6_OUT0_UNION;
#endif
#define PMIC_V_PRE6_OUT0_v_out_pre6_s0_START (0)
#define PMIC_V_PRE6_OUT0_v_out_pre6_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre6_s1 : 8;
    } reg;
} PMIC_V_PRE6_OUT1_UNION;
#endif
#define PMIC_V_PRE6_OUT1_v_out_pre6_s1_START (0)
#define PMIC_V_PRE6_OUT1_v_out_pre6_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre6_s2 : 8;
    } reg;
} PMIC_V_PRE6_OUT2_UNION;
#endif
#define PMIC_V_PRE6_OUT2_v_out_pre6_s2_START (0)
#define PMIC_V_PRE6_OUT2_v_out_pre6_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre7_s0 : 8;
    } reg;
} PMIC_V_PRE7_OUT0_UNION;
#endif
#define PMIC_V_PRE7_OUT0_v_out_pre7_s0_START (0)
#define PMIC_V_PRE7_OUT0_v_out_pre7_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre7_s1 : 8;
    } reg;
} PMIC_V_PRE7_OUT1_UNION;
#endif
#define PMIC_V_PRE7_OUT1_v_out_pre7_s1_START (0)
#define PMIC_V_PRE7_OUT1_v_out_pre7_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_out_pre7_s2 : 8;
    } reg;
} PMIC_V_PRE7_OUT2_UNION;
#endif
#define PMIC_V_PRE7_OUT2_v_out_pre7_s2_START (0)
#define PMIC_V_PRE7_OUT2_v_out_pre7_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre0_s0 : 8;
    } reg;
} PMIC_CURRENT_PRE0_OUT0_UNION;
#endif
#define PMIC_CURRENT_PRE0_OUT0_current_pre0_s0_START (0)
#define PMIC_CURRENT_PRE0_OUT0_current_pre0_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre0_s1 : 8;
    } reg;
} PMIC_CURRENT_PRE0_OUT1_UNION;
#endif
#define PMIC_CURRENT_PRE0_OUT1_current_pre0_s1_START (0)
#define PMIC_CURRENT_PRE0_OUT1_current_pre0_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre0_s2 : 8;
    } reg;
} PMIC_CURRENT_PRE0_OUT2_UNION;
#endif
#define PMIC_CURRENT_PRE0_OUT2_current_pre0_s2_START (0)
#define PMIC_CURRENT_PRE0_OUT2_current_pre0_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre1_s0 : 8;
    } reg;
} PMIC_CURRENT_PRE1_OUT0_UNION;
#endif
#define PMIC_CURRENT_PRE1_OUT0_current_pre1_s0_START (0)
#define PMIC_CURRENT_PRE1_OUT0_current_pre1_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre1_s1 : 8;
    } reg;
} PMIC_CURRENT_PRE1_OUT1_UNION;
#endif
#define PMIC_CURRENT_PRE1_OUT1_current_pre1_s1_START (0)
#define PMIC_CURRENT_PRE1_OUT1_current_pre1_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre1_s2 : 8;
    } reg;
} PMIC_CURRENT_PRE1_OUT2_UNION;
#endif
#define PMIC_CURRENT_PRE1_OUT2_current_pre1_s2_START (0)
#define PMIC_CURRENT_PRE1_OUT2_current_pre1_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre2_s0 : 8;
    } reg;
} PMIC_CURRENT_PRE2_OUT0_UNION;
#endif
#define PMIC_CURRENT_PRE2_OUT0_current_pre2_s0_START (0)
#define PMIC_CURRENT_PRE2_OUT0_current_pre2_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre2_s1 : 8;
    } reg;
} PMIC_CURRENT_PRE2_OUT1_UNION;
#endif
#define PMIC_CURRENT_PRE2_OUT1_current_pre2_s1_START (0)
#define PMIC_CURRENT_PRE2_OUT1_current_pre2_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre2_s2 : 8;
    } reg;
} PMIC_CURRENT_PRE2_OUT2_UNION;
#endif
#define PMIC_CURRENT_PRE2_OUT2_current_pre2_s2_START (0)
#define PMIC_CURRENT_PRE2_OUT2_current_pre2_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre3_s0 : 8;
    } reg;
} PMIC_CURRENT_PRE3_OUT0_UNION;
#endif
#define PMIC_CURRENT_PRE3_OUT0_current_pre3_s0_START (0)
#define PMIC_CURRENT_PRE3_OUT0_current_pre3_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre3_s1 : 8;
    } reg;
} PMIC_CURRENT_PRE3_OUT1_UNION;
#endif
#define PMIC_CURRENT_PRE3_OUT1_current_pre3_s1_START (0)
#define PMIC_CURRENT_PRE3_OUT1_current_pre3_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre3_s2 : 8;
    } reg;
} PMIC_CURRENT_PRE3_OUT2_UNION;
#endif
#define PMIC_CURRENT_PRE3_OUT2_current_pre3_s2_START (0)
#define PMIC_CURRENT_PRE3_OUT2_current_pre3_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre4_s0 : 8;
    } reg;
} PMIC_CURRENT_PRE4_OUT0_UNION;
#endif
#define PMIC_CURRENT_PRE4_OUT0_current_pre4_s0_START (0)
#define PMIC_CURRENT_PRE4_OUT0_current_pre4_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre4_s1 : 8;
    } reg;
} PMIC_CURRENT_PRE4_OUT1_UNION;
#endif
#define PMIC_CURRENT_PRE4_OUT1_current_pre4_s1_START (0)
#define PMIC_CURRENT_PRE4_OUT1_current_pre4_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre4_s2 : 8;
    } reg;
} PMIC_CURRENT_PRE4_OUT2_UNION;
#endif
#define PMIC_CURRENT_PRE4_OUT2_current_pre4_s2_START (0)
#define PMIC_CURRENT_PRE4_OUT2_current_pre4_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre5_s0 : 8;
    } reg;
} PMIC_CURRENT_PRE5_OUT0_UNION;
#endif
#define PMIC_CURRENT_PRE5_OUT0_current_pre5_s0_START (0)
#define PMIC_CURRENT_PRE5_OUT0_current_pre5_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre5_s1 : 8;
    } reg;
} PMIC_CURRENT_PRE5_OUT1_UNION;
#endif
#define PMIC_CURRENT_PRE5_OUT1_current_pre5_s1_START (0)
#define PMIC_CURRENT_PRE5_OUT1_current_pre5_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre5_s2 : 8;
    } reg;
} PMIC_CURRENT_PRE5_OUT2_UNION;
#endif
#define PMIC_CURRENT_PRE5_OUT2_current_pre5_s2_START (0)
#define PMIC_CURRENT_PRE5_OUT2_current_pre5_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre6_s0 : 8;
    } reg;
} PMIC_CURRENT_PRE6_OUT0_UNION;
#endif
#define PMIC_CURRENT_PRE6_OUT0_current_pre6_s0_START (0)
#define PMIC_CURRENT_PRE6_OUT0_current_pre6_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre6_s1 : 8;
    } reg;
} PMIC_CURRENT_PRE6_OUT1_UNION;
#endif
#define PMIC_CURRENT_PRE6_OUT1_current_pre6_s1_START (0)
#define PMIC_CURRENT_PRE6_OUT1_current_pre6_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre6_s2 : 8;
    } reg;
} PMIC_CURRENT_PRE6_OUT2_UNION;
#endif
#define PMIC_CURRENT_PRE6_OUT2_current_pre6_s2_START (0)
#define PMIC_CURRENT_PRE6_OUT2_current_pre6_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre7_s0 : 8;
    } reg;
} PMIC_CURRENT_PRE7_OUT0_UNION;
#endif
#define PMIC_CURRENT_PRE7_OUT0_current_pre7_s0_START (0)
#define PMIC_CURRENT_PRE7_OUT0_current_pre7_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre7_s1 : 8;
    } reg;
} PMIC_CURRENT_PRE7_OUT1_UNION;
#endif
#define PMIC_CURRENT_PRE7_OUT1_current_pre7_s1_START (0)
#define PMIC_CURRENT_PRE7_OUT1_current_pre7_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_pre7_s2 : 8;
    } reg;
} PMIC_CURRENT_PRE7_OUT2_UNION;
#endif
#define PMIC_CURRENT_PRE7_OUT2_current_pre7_s2_START (0)
#define PMIC_CURRENT_PRE7_OUT2_current_pre7_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_out_pre1_s0 : 8;
    } reg;
} PMIC_V_OCV_PRE1_OUT0_UNION;
#endif
#define PMIC_V_OCV_PRE1_OUT0_v_ocv_out_pre1_s0_START (0)
#define PMIC_V_OCV_PRE1_OUT0_v_ocv_out_pre1_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_out_pre1_s1 : 8;
    } reg;
} PMIC_V_OCV_PRE1_OUT1_UNION;
#endif
#define PMIC_V_OCV_PRE1_OUT1_v_ocv_out_pre1_s1_START (0)
#define PMIC_V_OCV_PRE1_OUT1_v_ocv_out_pre1_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_out_pre1_s2 : 8;
    } reg;
} PMIC_V_OCV_PRE1_OUT2_UNION;
#endif
#define PMIC_V_OCV_PRE1_OUT2_v_ocv_out_pre1_s2_START (0)
#define PMIC_V_OCV_PRE1_OUT2_v_ocv_out_pre1_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_out_pre2_s0 : 8;
    } reg;
} PMIC_V_OCV_PRE2_OUT0_UNION;
#endif
#define PMIC_V_OCV_PRE2_OUT0_v_ocv_out_pre2_s0_START (0)
#define PMIC_V_OCV_PRE2_OUT0_v_ocv_out_pre2_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_out_pre2_s1 : 8;
    } reg;
} PMIC_V_OCV_PRE2_OUT1_UNION;
#endif
#define PMIC_V_OCV_PRE2_OUT1_v_ocv_out_pre2_s1_START (0)
#define PMIC_V_OCV_PRE2_OUT1_v_ocv_out_pre2_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_out_pre2_s2 : 8;
    } reg;
} PMIC_V_OCV_PRE2_OUT2_UNION;
#endif
#define PMIC_V_OCV_PRE2_OUT2_v_ocv_out_pre2_s2_START (0)
#define PMIC_V_OCV_PRE2_OUT2_v_ocv_out_pre2_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_out_pre3_s0 : 8;
    } reg;
} PMIC_V_OCV_PRE3_OUT0_UNION;
#endif
#define PMIC_V_OCV_PRE3_OUT0_v_ocv_out_pre3_s0_START (0)
#define PMIC_V_OCV_PRE3_OUT0_v_ocv_out_pre3_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_out_pre3_s1 : 8;
    } reg;
} PMIC_V_OCV_PRE3_OUT1_UNION;
#endif
#define PMIC_V_OCV_PRE3_OUT1_v_ocv_out_pre3_s1_START (0)
#define PMIC_V_OCV_PRE3_OUT1_v_ocv_out_pre3_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_out_pre3_s2 : 8;
    } reg;
} PMIC_V_OCV_PRE3_OUT2_UNION;
#endif
#define PMIC_V_OCV_PRE3_OUT2_v_ocv_out_pre3_s2_START (0)
#define PMIC_V_OCV_PRE3_OUT2_v_ocv_out_pre3_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_out_pre4_s0 : 8;
    } reg;
} PMIC_V_OCV_PRE4_OUT0_UNION;
#endif
#define PMIC_V_OCV_PRE4_OUT0_v_ocv_out_pre4_s0_START (0)
#define PMIC_V_OCV_PRE4_OUT0_v_ocv_out_pre4_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_out_pre4_s1 : 8;
    } reg;
} PMIC_V_OCV_PRE4_OUT1_UNION;
#endif
#define PMIC_V_OCV_PRE4_OUT1_v_ocv_out_pre4_s1_START (0)
#define PMIC_V_OCV_PRE4_OUT1_v_ocv_out_pre4_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char v_ocv_out_pre4_s2 : 8;
    } reg;
} PMIC_V_OCV_PRE4_OUT2_UNION;
#endif
#define PMIC_V_OCV_PRE4_OUT2_v_ocv_out_pre4_s2_START (0)
#define PMIC_V_OCV_PRE4_OUT2_v_ocv_out_pre4_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_out_pre1_s0 : 8;
    } reg;
} PMIC_I_OCV_PRE1_OUT0_UNION;
#endif
#define PMIC_I_OCV_PRE1_OUT0_i_ocv_out_pre1_s0_START (0)
#define PMIC_I_OCV_PRE1_OUT0_i_ocv_out_pre1_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_out_pre1_s1 : 8;
    } reg;
} PMIC_I_OCV_PRE1_OUT1_UNION;
#endif
#define PMIC_I_OCV_PRE1_OUT1_i_ocv_out_pre1_s1_START (0)
#define PMIC_I_OCV_PRE1_OUT1_i_ocv_out_pre1_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_out_pre1_s2 : 8;
    } reg;
} PMIC_I_OCV_PRE1_OUT2_UNION;
#endif
#define PMIC_I_OCV_PRE1_OUT2_i_ocv_out_pre1_s2_START (0)
#define PMIC_I_OCV_PRE1_OUT2_i_ocv_out_pre1_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_out_pre2_s0 : 8;
    } reg;
} PMIC_I_OCV_PRE2_OUT0_UNION;
#endif
#define PMIC_I_OCV_PRE2_OUT0_i_ocv_out_pre2_s0_START (0)
#define PMIC_I_OCV_PRE2_OUT0_i_ocv_out_pre2_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_out_pre2_s1 : 8;
    } reg;
} PMIC_I_OCV_PRE2_OUT1_UNION;
#endif
#define PMIC_I_OCV_PRE2_OUT1_i_ocv_out_pre2_s1_START (0)
#define PMIC_I_OCV_PRE2_OUT1_i_ocv_out_pre2_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_out_pre2_s2 : 8;
    } reg;
} PMIC_I_OCV_PRE2_OUT2_UNION;
#endif
#define PMIC_I_OCV_PRE2_OUT2_i_ocv_out_pre2_s2_START (0)
#define PMIC_I_OCV_PRE2_OUT2_i_ocv_out_pre2_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_out_pre3_s0 : 8;
    } reg;
} PMIC_I_OCV_PRE3_OUT0_UNION;
#endif
#define PMIC_I_OCV_PRE3_OUT0_i_ocv_out_pre3_s0_START (0)
#define PMIC_I_OCV_PRE3_OUT0_i_ocv_out_pre3_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_out_pre3_s1 : 8;
    } reg;
} PMIC_I_OCV_PRE3_OUT1_UNION;
#endif
#define PMIC_I_OCV_PRE3_OUT1_i_ocv_out_pre3_s1_START (0)
#define PMIC_I_OCV_PRE3_OUT1_i_ocv_out_pre3_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_out_pre3_s2 : 8;
    } reg;
} PMIC_I_OCV_PRE3_OUT2_UNION;
#endif
#define PMIC_I_OCV_PRE3_OUT2_i_ocv_out_pre3_s2_START (0)
#define PMIC_I_OCV_PRE3_OUT2_i_ocv_out_pre3_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_out_pre4_s0 : 8;
    } reg;
} PMIC_I_OCV_PRE4_OUT0_UNION;
#endif
#define PMIC_I_OCV_PRE4_OUT0_i_ocv_out_pre4_s0_START (0)
#define PMIC_I_OCV_PRE4_OUT0_i_ocv_out_pre4_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_out_pre4_s1 : 8;
    } reg;
} PMIC_I_OCV_PRE4_OUT1_UNION;
#endif
#define PMIC_I_OCV_PRE4_OUT1_i_ocv_out_pre4_s1_START (0)
#define PMIC_I_OCV_PRE4_OUT1_i_ocv_out_pre4_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char i_ocv_out_pre4_s2 : 8;
    } reg;
} PMIC_I_OCV_PRE4_OUT2_UNION;
#endif
#define PMIC_I_OCV_PRE4_OUT2_i_ocv_out_pre4_s2_START (0)
#define PMIC_I_OCV_PRE4_OUT2_i_ocv_out_pre4_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char t_ocv_out_pre1_s0 : 8;
    } reg;
} PMIC_T_OCV_PRE1_OUT0_UNION;
#endif
#define PMIC_T_OCV_PRE1_OUT0_t_ocv_out_pre1_s0_START (0)
#define PMIC_T_OCV_PRE1_OUT0_t_ocv_out_pre1_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char t_ocv_out_pre1_s1 : 8;
    } reg;
} PMIC_T_OCV_PRE1_OUT1_UNION;
#endif
#define PMIC_T_OCV_PRE1_OUT1_t_ocv_out_pre1_s1_START (0)
#define PMIC_T_OCV_PRE1_OUT1_t_ocv_out_pre1_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char t_ocv_out_pre2_s0 : 8;
    } reg;
} PMIC_T_OCV_PRE2_OUT0_UNION;
#endif
#define PMIC_T_OCV_PRE2_OUT0_t_ocv_out_pre2_s0_START (0)
#define PMIC_T_OCV_PRE2_OUT0_t_ocv_out_pre2_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char t_ocv_out_pre2_s1 : 8;
    } reg;
} PMIC_T_OCV_PRE2_OUT1_UNION;
#endif
#define PMIC_T_OCV_PRE2_OUT1_t_ocv_out_pre2_s1_START (0)
#define PMIC_T_OCV_PRE2_OUT1_t_ocv_out_pre2_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char t_ocv_out_pre3_s0 : 8;
    } reg;
} PMIC_T_OCV_PRE3_OUT0_UNION;
#endif
#define PMIC_T_OCV_PRE3_OUT0_t_ocv_out_pre3_s0_START (0)
#define PMIC_T_OCV_PRE3_OUT0_t_ocv_out_pre3_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char t_ocv_out_pre3_s1 : 8;
    } reg;
} PMIC_T_OCV_PRE3_OUT1_UNION;
#endif
#define PMIC_T_OCV_PRE3_OUT1_t_ocv_out_pre3_s1_START (0)
#define PMIC_T_OCV_PRE3_OUT1_t_ocv_out_pre3_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char t_ocv_out_pre4_s0 : 8;
    } reg;
} PMIC_T_OCV_PRE4_OUT0_UNION;
#endif
#define PMIC_T_OCV_PRE4_OUT0_t_ocv_out_pre4_s0_START (0)
#define PMIC_T_OCV_PRE4_OUT0_t_ocv_out_pre4_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char t_ocv_out_pre4_s1 : 8;
    } reg;
} PMIC_T_OCV_PRE4_OUT1_UNION;
#endif
#define PMIC_T_OCV_PRE4_OUT1_t_ocv_out_pre4_s1_START (0)
#define PMIC_T_OCV_PRE4_OUT1_t_ocv_out_pre4_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtc_ocv_out_pre4 : 2;
        unsigned char rtc_ocv_out_pre3 : 2;
        unsigned char rtc_ocv_out_pre2 : 2;
        unsigned char rtc_ocv_out_pre1 : 2;
    } reg;
} PMIC_RTC_OCV_OUT_UNION;
#endif
#define PMIC_RTC_OCV_OUT_rtc_ocv_out_pre4_START (0)
#define PMIC_RTC_OCV_OUT_rtc_ocv_out_pre4_END (1)
#define PMIC_RTC_OCV_OUT_rtc_ocv_out_pre3_START (2)
#define PMIC_RTC_OCV_OUT_rtc_ocv_out_pre3_END (3)
#define PMIC_RTC_OCV_OUT_rtc_ocv_out_pre2_START (4)
#define PMIC_RTC_OCV_OUT_rtc_ocv_out_pre2_END (5)
#define PMIC_RTC_OCV_OUT_rtc_ocv_out_pre1_START (6)
#define PMIC_RTC_OCV_OUT_rtc_ocv_out_pre1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_i_data0_out0 : 8;
    } reg;
} PMIC_SLIDE_I_DATA0_OUT0_UNION;
#endif
#define PMIC_SLIDE_I_DATA0_OUT0_slide_i_data0_out0_START (0)
#define PMIC_SLIDE_I_DATA0_OUT0_slide_i_data0_out0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_i_data0_out1 : 8;
    } reg;
} PMIC_SLIDE_I_DATA0_OUT1_UNION;
#endif
#define PMIC_SLIDE_I_DATA0_OUT1_slide_i_data0_out1_START (0)
#define PMIC_SLIDE_I_DATA0_OUT1_slide_i_data0_out1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_i_data0_out2 : 8;
    } reg;
} PMIC_SLIDE_I_DATA0_OUT2_UNION;
#endif
#define PMIC_SLIDE_I_DATA0_OUT2_slide_i_data0_out2_START (0)
#define PMIC_SLIDE_I_DATA0_OUT2_slide_i_data0_out2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_i_data1_out0 : 8;
    } reg;
} PMIC_SLIDE_I_DATA1_OUT0_UNION;
#endif
#define PMIC_SLIDE_I_DATA1_OUT0_slide_i_data1_out0_START (0)
#define PMIC_SLIDE_I_DATA1_OUT0_slide_i_data1_out0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_i_data1_out1 : 8;
    } reg;
} PMIC_SLIDE_I_DATA1_OUT1_UNION;
#endif
#define PMIC_SLIDE_I_DATA1_OUT1_slide_i_data1_out1_START (0)
#define PMIC_SLIDE_I_DATA1_OUT1_slide_i_data1_out1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_i_data1_out2 : 8;
    } reg;
} PMIC_SLIDE_I_DATA1_OUT2_UNION;
#endif
#define PMIC_SLIDE_I_DATA1_OUT2_slide_i_data1_out2_START (0)
#define PMIC_SLIDE_I_DATA1_OUT2_slide_i_data1_out2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_i_data2_out0 : 8;
    } reg;
} PMIC_SLIDE_I_DATA2_OUT0_UNION;
#endif
#define PMIC_SLIDE_I_DATA2_OUT0_slide_i_data2_out0_START (0)
#define PMIC_SLIDE_I_DATA2_OUT0_slide_i_data2_out0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_i_data2_out1 : 8;
    } reg;
} PMIC_SLIDE_I_DATA2_OUT1_UNION;
#endif
#define PMIC_SLIDE_I_DATA2_OUT1_slide_i_data2_out1_START (0)
#define PMIC_SLIDE_I_DATA2_OUT1_slide_i_data2_out1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_i_data2_out2 : 8;
    } reg;
} PMIC_SLIDE_I_DATA2_OUT2_UNION;
#endif
#define PMIC_SLIDE_I_DATA2_OUT2_slide_i_data2_out2_START (0)
#define PMIC_SLIDE_I_DATA2_OUT2_slide_i_data2_out2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_i_data3_out0 : 8;
    } reg;
} PMIC_SLIDE_I_DATA3_OUT0_UNION;
#endif
#define PMIC_SLIDE_I_DATA3_OUT0_slide_i_data3_out0_START (0)
#define PMIC_SLIDE_I_DATA3_OUT0_slide_i_data3_out0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_i_data3_out1 : 8;
    } reg;
} PMIC_SLIDE_I_DATA3_OUT1_UNION;
#endif
#define PMIC_SLIDE_I_DATA3_OUT1_slide_i_data3_out1_START (0)
#define PMIC_SLIDE_I_DATA3_OUT1_slide_i_data3_out1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_i_data3_out2 : 8;
    } reg;
} PMIC_SLIDE_I_DATA3_OUT2_UNION;
#endif
#define PMIC_SLIDE_I_DATA3_OUT2_slide_i_data3_out2_START (0)
#define PMIC_SLIDE_I_DATA3_OUT2_slide_i_data3_out2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_v_data0_out0 : 8;
    } reg;
} PMIC_SLIDE_V_DATA0_OUT0_UNION;
#endif
#define PMIC_SLIDE_V_DATA0_OUT0_slide_v_data0_out0_START (0)
#define PMIC_SLIDE_V_DATA0_OUT0_slide_v_data0_out0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_v_data0_out1 : 8;
    } reg;
} PMIC_SLIDE_V_DATA0_OUT1_UNION;
#endif
#define PMIC_SLIDE_V_DATA0_OUT1_slide_v_data0_out1_START (0)
#define PMIC_SLIDE_V_DATA0_OUT1_slide_v_data0_out1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_v_data0_out2 : 8;
    } reg;
} PMIC_SLIDE_V_DATA0_OUT2_UNION;
#endif
#define PMIC_SLIDE_V_DATA0_OUT2_slide_v_data0_out2_START (0)
#define PMIC_SLIDE_V_DATA0_OUT2_slide_v_data0_out2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_v_data1_out0 : 8;
    } reg;
} PMIC_SLIDE_V_DATA1_OUT0_UNION;
#endif
#define PMIC_SLIDE_V_DATA1_OUT0_slide_v_data1_out0_START (0)
#define PMIC_SLIDE_V_DATA1_OUT0_slide_v_data1_out0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_v_data1_out1 : 8;
    } reg;
} PMIC_SLIDE_V_DATA1_OUT1_UNION;
#endif
#define PMIC_SLIDE_V_DATA1_OUT1_slide_v_data1_out1_START (0)
#define PMIC_SLIDE_V_DATA1_OUT1_slide_v_data1_out1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_v_data1_out2 : 8;
    } reg;
} PMIC_SLIDE_V_DATA1_OUT2_UNION;
#endif
#define PMIC_SLIDE_V_DATA1_OUT2_slide_v_data1_out2_START (0)
#define PMIC_SLIDE_V_DATA1_OUT2_slide_v_data1_out2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_v_data2_out0 : 8;
    } reg;
} PMIC_SLIDE_V_DATA2_OUT0_UNION;
#endif
#define PMIC_SLIDE_V_DATA2_OUT0_slide_v_data2_out0_START (0)
#define PMIC_SLIDE_V_DATA2_OUT0_slide_v_data2_out0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_v_data2_out1 : 8;
    } reg;
} PMIC_SLIDE_V_DATA2_OUT1_UNION;
#endif
#define PMIC_SLIDE_V_DATA2_OUT1_slide_v_data2_out1_START (0)
#define PMIC_SLIDE_V_DATA2_OUT1_slide_v_data2_out1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_v_data2_out2 : 8;
    } reg;
} PMIC_SLIDE_V_DATA2_OUT2_UNION;
#endif
#define PMIC_SLIDE_V_DATA2_OUT2_slide_v_data2_out2_START (0)
#define PMIC_SLIDE_V_DATA2_OUT2_slide_v_data2_out2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_v_data3_out0 : 8;
    } reg;
} PMIC_SLIDE_V_DATA3_OUT0_UNION;
#endif
#define PMIC_SLIDE_V_DATA3_OUT0_slide_v_data3_out0_START (0)
#define PMIC_SLIDE_V_DATA3_OUT0_slide_v_data3_out0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_v_data3_out1 : 8;
    } reg;
} PMIC_SLIDE_V_DATA3_OUT1_UNION;
#endif
#define PMIC_SLIDE_V_DATA3_OUT1_slide_v_data3_out1_START (0)
#define PMIC_SLIDE_V_DATA3_OUT1_slide_v_data3_out1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_v_data3_out2 : 8;
    } reg;
} PMIC_SLIDE_V_DATA3_OUT2_UNION;
#endif
#define PMIC_SLIDE_V_DATA3_OUT2_slide_v_data3_out2_START (0)
#define PMIC_SLIDE_V_DATA3_OUT2_slide_v_data3_out2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_cnt_data_out0 : 8;
    } reg;
} PMIC_SLIDE_CNT_DATA_OUT0_UNION;
#endif
#define PMIC_SLIDE_CNT_DATA_OUT0_slide_cnt_data_out0_START (0)
#define PMIC_SLIDE_CNT_DATA_OUT0_slide_cnt_data_out0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_cnt_data_out1 : 8;
    } reg;
} PMIC_SLIDE_CNT_DATA_OUT1_UNION;
#endif
#define PMIC_SLIDE_CNT_DATA_OUT1_slide_cnt_data_out1_START (0)
#define PMIC_SLIDE_CNT_DATA_OUT1_slide_cnt_data_out1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char slide_cnt_data_out2 : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_SLIDE_CNT_DATA_OUT2_UNION;
#endif
#define PMIC_SLIDE_CNT_DATA_OUT2_slide_cnt_data_out2_START (0)
#define PMIC_SLIDE_CNT_DATA_OUT2_slide_cnt_data_out2_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char coul_reserve0 : 8;
    } reg;
} PMIC_COUL_RESERVE0_UNION;
#endif
#define PMIC_COUL_RESERVE0_coul_reserve0_START (0)
#define PMIC_COUL_RESERVE0_coul_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char coul_reserve1 : 8;
    } reg;
} PMIC_COUL_RESERVE1_UNION;
#endif
#define PMIC_COUL_RESERVE1_coul_reserve1_START (0)
#define PMIC_COUL_RESERVE1_coul_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char clj_debug0_0 : 1;
        unsigned char clj_debug0_1 : 1;
        unsigned char clj_debug0_2 : 1;
        unsigned char clj_debug0_3 : 1;
        unsigned char cali_en_i : 1;
        unsigned char cali_en_i_force : 1;
        unsigned char cali_en_v_force : 1;
        unsigned char cali_en_v : 1;
    } reg;
} PMIC_CLJ_DEBUG0_UNION;
#endif
#define PMIC_CLJ_DEBUG0_clj_debug0_0_START (0)
#define PMIC_CLJ_DEBUG0_clj_debug0_0_END (0)
#define PMIC_CLJ_DEBUG0_clj_debug0_1_START (1)
#define PMIC_CLJ_DEBUG0_clj_debug0_1_END (1)
#define PMIC_CLJ_DEBUG0_clj_debug0_2_START (2)
#define PMIC_CLJ_DEBUG0_clj_debug0_2_END (2)
#define PMIC_CLJ_DEBUG0_clj_debug0_3_START (3)
#define PMIC_CLJ_DEBUG0_clj_debug0_3_END (3)
#define PMIC_CLJ_DEBUG0_cali_en_i_START (4)
#define PMIC_CLJ_DEBUG0_cali_en_i_END (4)
#define PMIC_CLJ_DEBUG0_cali_en_i_force_START (5)
#define PMIC_CLJ_DEBUG0_cali_en_i_force_END (5)
#define PMIC_CLJ_DEBUG0_cali_en_v_force_START (6)
#define PMIC_CLJ_DEBUG0_cali_en_v_force_END (6)
#define PMIC_CLJ_DEBUG0_cali_en_v_START (7)
#define PMIC_CLJ_DEBUG0_cali_en_v_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char current_coul_always_off : 1;
        unsigned char voltage_coul_always_off : 1;
        unsigned char clj_debug_1_2 : 1;
        unsigned char coul_ctrl_onoff : 1;
        unsigned char coul_ocv_onoff : 1;
        unsigned char coul_debug_data_sel : 3;
    } reg;
} PMIC_CLJ_DEBUG1_UNION;
#endif
#define PMIC_CLJ_DEBUG1_current_coul_always_off_START (0)
#define PMIC_CLJ_DEBUG1_current_coul_always_off_END (0)
#define PMIC_CLJ_DEBUG1_voltage_coul_always_off_START (1)
#define PMIC_CLJ_DEBUG1_voltage_coul_always_off_END (1)
#define PMIC_CLJ_DEBUG1_clj_debug_1_2_START (2)
#define PMIC_CLJ_DEBUG1_clj_debug_1_2_END (2)
#define PMIC_CLJ_DEBUG1_coul_ctrl_onoff_START (3)
#define PMIC_CLJ_DEBUG1_coul_ctrl_onoff_END (3)
#define PMIC_CLJ_DEBUG1_coul_ocv_onoff_START (4)
#define PMIC_CLJ_DEBUG1_coul_ocv_onoff_END (4)
#define PMIC_CLJ_DEBUG1_coul_debug_data_sel_START (5)
#define PMIC_CLJ_DEBUG1_coul_debug_data_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char debug_cic_i_s0 : 8;
    } reg;
} PMIC_DEBUG_CIC_I0_UNION;
#endif
#define PMIC_DEBUG_CIC_I0_debug_cic_i_s0_START (0)
#define PMIC_DEBUG_CIC_I0_debug_cic_i_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char debug_cic_i_s1 : 8;
    } reg;
} PMIC_DEBUG_CIC_I1_UNION;
#endif
#define PMIC_DEBUG_CIC_I1_debug_cic_i_s1_START (0)
#define PMIC_DEBUG_CIC_I1_debug_cic_i_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char debug_cic_i_s2 : 8;
    } reg;
} PMIC_DEBUG_CIC_I2_UNION;
#endif
#define PMIC_DEBUG_CIC_I2_debug_cic_i_s2_START (0)
#define PMIC_DEBUG_CIC_I2_debug_cic_i_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char debug_cic_v_s0 : 8;
    } reg;
} PMIC_DEBUG_CIC_V0_UNION;
#endif
#define PMIC_DEBUG_CIC_V0_debug_cic_v_s0_START (0)
#define PMIC_DEBUG_CIC_V0_debug_cic_v_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char debug_cic_v_s1 : 8;
    } reg;
} PMIC_DEBUG_CIC_V1_UNION;
#endif
#define PMIC_DEBUG_CIC_V1_debug_cic_v_s1_START (0)
#define PMIC_DEBUG_CIC_V1_debug_cic_v_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char debug_cic_v_s2 : 8;
    } reg;
} PMIC_DEBUG_CIC_V2_UNION;
#endif
#define PMIC_DEBUG_CIC_V2_debug_cic_v_s2_START (0)
#define PMIC_DEBUG_CIC_V2_debug_cic_v_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char debug_write_pro : 8;
    } reg;
} PMIC_DEBUG_WRITE_PRO_UNION;
#endif
#define PMIC_DEBUG_WRITE_PRO_debug_write_pro_START (0)
#define PMIC_DEBUG_WRITE_PRO_debug_write_pro_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtcdr0 : 8;
    } reg;
} PMIC_RTCDR0_UNION;
#endif
#define PMIC_RTCDR0_rtcdr0_START (0)
#define PMIC_RTCDR0_rtcdr0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtcdr1 : 8;
    } reg;
} PMIC_RTCDR1_UNION;
#endif
#define PMIC_RTCDR1_rtcdr1_START (0)
#define PMIC_RTCDR1_rtcdr1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtcdr2 : 8;
    } reg;
} PMIC_RTCDR2_UNION;
#endif
#define PMIC_RTCDR2_rtcdr2_START (0)
#define PMIC_RTCDR2_rtcdr2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtcdr3 : 8;
    } reg;
} PMIC_RTCDR3_UNION;
#endif
#define PMIC_RTCDR3_rtcdr3_START (0)
#define PMIC_RTCDR3_rtcdr3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtcmr0 : 8;
    } reg;
} PMIC_RTCMR0_UNION;
#endif
#define PMIC_RTCMR0_rtcmr0_START (0)
#define PMIC_RTCMR0_rtcmr0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtcmr1 : 8;
    } reg;
} PMIC_RTCMR1_UNION;
#endif
#define PMIC_RTCMR1_rtcmr1_START (0)
#define PMIC_RTCMR1_rtcmr1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtcmr2 : 8;
    } reg;
} PMIC_RTCMR2_UNION;
#endif
#define PMIC_RTCMR2_rtcmr2_START (0)
#define PMIC_RTCMR2_rtcmr2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtcmr3 : 8;
    } reg;
} PMIC_RTCMR3_UNION;
#endif
#define PMIC_RTCMR3_rtcmr3_START (0)
#define PMIC_RTCMR3_rtcmr3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtcclr0 : 8;
    } reg;
} PMIC_RTCLR0_UNION;
#endif
#define PMIC_RTCLR0_rtcclr0_START (0)
#define PMIC_RTCLR0_rtcclr0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtcclr1 : 8;
    } reg;
} PMIC_RTCLR1_UNION;
#endif
#define PMIC_RTCLR1_rtcclr1_START (0)
#define PMIC_RTCLR1_rtcclr1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtcclr2 : 8;
    } reg;
} PMIC_RTCLR2_UNION;
#endif
#define PMIC_RTCLR2_rtcclr2_START (0)
#define PMIC_RTCLR2_rtcclr2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtcclr3 : 8;
    } reg;
} PMIC_RTCLR3_UNION;
#endif
#define PMIC_RTCLR3_rtcclr3_START (0)
#define PMIC_RTCLR3_rtcclr3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtccr : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_RTCCTRL_UNION;
#endif
#define PMIC_RTCCTRL_rtccr_START (0)
#define PMIC_RTCCTRL_rtccr_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char crc_value0 : 8;
    } reg;
} PMIC_CRC_VAULE0_UNION;
#endif
#define PMIC_CRC_VAULE0_crc_value0_START (0)
#define PMIC_CRC_VAULE0_crc_value0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char crc_value1 : 8;
    } reg;
} PMIC_CRC_VAULE1_UNION;
#endif
#define PMIC_CRC_VAULE1_crc_value1_START (0)
#define PMIC_CRC_VAULE1_crc_value1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char crc_value2 : 5;
        unsigned char reserved : 3;
    } reg;
} PMIC_CRC_VAULE2_UNION;
#endif
#define PMIC_CRC_VAULE2_crc_value2_START (0)
#define PMIC_CRC_VAULE2_crc_value2_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtc_pwrup_timer0 : 8;
    } reg;
} PMIC_RTC_PWRUP_TIMER0_UNION;
#endif
#define PMIC_RTC_PWRUP_TIMER0_rtc_pwrup_timer0_START (0)
#define PMIC_RTC_PWRUP_TIMER0_rtc_pwrup_timer0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtc_pwrup_timer1 : 8;
    } reg;
} PMIC_RTC_PWRUP_TIMER1_UNION;
#endif
#define PMIC_RTC_PWRUP_TIMER1_rtc_pwrup_timer1_START (0)
#define PMIC_RTC_PWRUP_TIMER1_rtc_pwrup_timer1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtc_pwrup_timer2 : 8;
    } reg;
} PMIC_RTC_PWRUP_TIMER2_UNION;
#endif
#define PMIC_RTC_PWRUP_TIMER2_rtc_pwrup_timer2_START (0)
#define PMIC_RTC_PWRUP_TIMER2_rtc_pwrup_timer2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtc_pwrup_timer3 : 8;
    } reg;
} PMIC_RTC_PWRUP_TIMER3_UNION;
#endif
#define PMIC_RTC_PWRUP_TIMER3_rtc_pwrup_timer3_START (0)
#define PMIC_RTC_PWRUP_TIMER3_rtc_pwrup_timer3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtc_pwrdown_timer0 : 8;
    } reg;
} PMIC_RTC_PWRDOWN_TIMER0_UNION;
#endif
#define PMIC_RTC_PWRDOWN_TIMER0_rtc_pwrdown_timer0_START (0)
#define PMIC_RTC_PWRDOWN_TIMER0_rtc_pwrdown_timer0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtc_pwrdown_timer1 : 8;
    } reg;
} PMIC_RTC_PWRDOWN_TIMER1_UNION;
#endif
#define PMIC_RTC_PWRDOWN_TIMER1_rtc_pwrdown_timer1_START (0)
#define PMIC_RTC_PWRDOWN_TIMER1_rtc_pwrdown_timer1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtc_pwrdown_timer2 : 8;
    } reg;
} PMIC_RTC_PWRDOWN_TIMER2_UNION;
#endif
#define PMIC_RTC_PWRDOWN_TIMER2_rtc_pwrdown_timer2_START (0)
#define PMIC_RTC_PWRDOWN_TIMER2_rtc_pwrdown_timer2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char rtc_pwrdown_timer3 : 8;
    } reg;
} PMIC_RTC_PWRDOWN_TIMER3_UNION;
#endif
#define PMIC_RTC_PWRDOWN_TIMER3_rtc_pwrdown_timer3_START (0)
#define PMIC_RTC_PWRDOWN_TIMER3_rtc_pwrdown_timer3_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ovp_idis_sel : 4;
        unsigned char acr_cap_sel : 2;
        unsigned char acr_mul_sel : 2;
    } reg;
} PMIC_ACR_CFG0_UNION;
#endif
#define PMIC_ACR_CFG0_ovp_idis_sel_START (0)
#define PMIC_ACR_CFG0_ovp_idis_sel_END (3)
#define PMIC_ACR_CFG0_acr_cap_sel_START (4)
#define PMIC_ACR_CFG0_acr_cap_sel_END (5)
#define PMIC_ACR_CFG0_acr_mul_sel_START (6)
#define PMIC_ACR_CFG0_acr_mul_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_testmode : 1;
        unsigned char acr_iref_sel : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_ACR_CFG1_UNION;
#endif
#define PMIC_ACR_CFG1_acr_testmode_START (0)
#define PMIC_ACR_CFG1_acr_testmode_END (0)
#define PMIC_ACR_CFG1_acr_iref_sel_START (1)
#define PMIC_ACR_CFG1_acr_iref_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_reserve : 8;
    } reg;
} PMIC_ACR_RESERVE_CFG_UNION;
#endif
#define PMIC_ACR_RESERVE_CFG_acr_reserve_START (0)
#define PMIC_ACR_RESERVE_CFG_acr_reserve_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_chanl_sel : 5;
        unsigned char reserved : 2;
        unsigned char hkadc2_bypass : 1;
    } reg;
} PMIC_ACRADC_CTRL_UNION;
#endif
#define PMIC_ACRADC_CTRL_hkadc2_chanl_sel_START (0)
#define PMIC_ACRADC_CTRL_hkadc2_chanl_sel_END (4)
#define PMIC_ACRADC_CTRL_hkadc2_bypass_START (7)
#define PMIC_ACRADC_CTRL_hkadc2_bypass_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_start_soft : 1;
        unsigned char hkadc2_reserve : 7;
    } reg;
} PMIC_ACRADC_START_UNION;
#endif
#define PMIC_ACRADC_START_hkadc2_start_soft_START (0)
#define PMIC_ACRADC_START_hkadc2_start_soft_END (0)
#define PMIC_ACRADC_START_hkadc2_reserve_START (1)
#define PMIC_ACRADC_START_hkadc2_reserve_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_valid : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_ACRCONV_STATUS_UNION;
#endif
#define PMIC_ACRCONV_STATUS_hkadc2_valid_START (0)
#define PMIC_ACRCONV_STATUS_hkadc2_valid_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_data_l : 8;
    } reg;
} PMIC_ACRADC_DATA_L_UNION;
#endif
#define PMIC_ACRADC_DATA_L_hkadc2_data_l_START (0)
#define PMIC_ACRADC_DATA_L_hkadc2_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACRADC_DATA_H_UNION;
#endif
#define PMIC_ACRADC_DATA_H_hkadc2_data_h_START (0)
#define PMIC_ACRADC_DATA_H_hkadc2_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_tdie_data_l : 8;
    } reg;
} PMIC_ACRADC_TDIE_DATA_L_UNION;
#endif
#define PMIC_ACRADC_TDIE_DATA_L_hkadc2_tdie_data_l_START (0)
#define PMIC_ACRADC_TDIE_DATA_L_hkadc2_tdie_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_tdie_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACRADC_TDIE_DATA_H_UNION;
#endif
#define PMIC_ACRADC_TDIE_DATA_H_hkadc2_tdie_data_h_START (0)
#define PMIC_ACRADC_TDIE_DATA_H_hkadc2_tdie_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_tbat_data_l : 8;
    } reg;
} PMIC_ACRADC_TBAT_DATA_L_UNION;
#endif
#define PMIC_ACRADC_TBAT_DATA_L_hkadc2_tbat_data_l_START (0)
#define PMIC_ACRADC_TBAT_DATA_L_hkadc2_tbat_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_tbat_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACRADC_TBAT_DATA_H_UNION;
#endif
#define PMIC_ACRADC_TBAT_DATA_H_hkadc2_tbat_data_h_START (0)
#define PMIC_ACRADC_TBAT_DATA_H_hkadc2_tbat_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_buffer_sel : 1;
        unsigned char hkadc2_config : 7;
    } reg;
} PMIC_ACRADC_CONV_UNION;
#endif
#define PMIC_ACRADC_CONV_hkadc2_buffer_sel_START (0)
#define PMIC_ACRADC_CONV_hkadc2_buffer_sel_END (0)
#define PMIC_ACRADC_CONV_hkadc2_config_START (1)
#define PMIC_ACRADC_CONV_hkadc2_config_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_ibias_sel : 8;
    } reg;
} PMIC_ACRADC_CURRENT_UNION;
#endif
#define PMIC_ACRADC_CURRENT_hkadc2_ibias_sel_START (0)
#define PMIC_ACRADC_CURRENT_hkadc2_ibias_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_cali_en : 1;
        unsigned char hkadc2_cali_sel : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_ACRADC_CALI_CTRL_UNION;
#endif
#define PMIC_ACRADC_CALI_CTRL_hkadc2_cali_en_START (0)
#define PMIC_ACRADC_CALI_CTRL_hkadc2_cali_en_END (0)
#define PMIC_ACRADC_CALI_CTRL_hkadc2_cali_sel_START (1)
#define PMIC_ACRADC_CALI_CTRL_hkadc2_cali_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_cali_data : 8;
    } reg;
} PMIC_ACRADC_CALI_VALUE_UNION;
#endif
#define PMIC_ACRADC_CALI_VALUE_hkadc2_cali_data_START (0)
#define PMIC_ACRADC_CALI_VALUE_hkadc2_cali_data_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_cali_cfg : 8;
    } reg;
} PMIC_ACRADC_CALI_CFG_UNION;
#endif
#define PMIC_ACRADC_CALI_CFG_hkadc2_cali_cfg_START (0)
#define PMIC_ACRADC_CALI_CFG_hkadc2_cali_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_tdie_cali_data : 8;
    } reg;
} PMIC_ACRADC_TDIE_CALI_DATA_UNION;
#endif
#define PMIC_ACRADC_TDIE_CALI_DATA_hkadc2_tdie_cali_data_START (0)
#define PMIC_ACRADC_TDIE_CALI_DATA_hkadc2_tdie_cali_data_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_tbat_cali_data : 8;
    } reg;
} PMIC_ACRADC_TBAT_CALI_DATA_UNION;
#endif
#define PMIC_ACRADC_TBAT_CALI_DATA_hkadc2_tbat_cali_data_START (0)
#define PMIC_ACRADC_TBAT_CALI_DATA_hkadc2_tbat_cali_data_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_tdie_cali_cfg : 8;
    } reg;
} PMIC_ACRADC_TDIE_CALI_CFG_UNION;
#endif
#define PMIC_ACRADC_TDIE_CALI_CFG_hkadc2_tdie_cali_cfg_START (0)
#define PMIC_ACRADC_TDIE_CALI_CFG_hkadc2_tdie_cali_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_tbat_cali_cfg : 8;
    } reg;
} PMIC_ACRADC_TBAT_CALI_CFG_UNION;
#endif
#define PMIC_ACRADC_TBAT_CALI_CFG_hkadc2_tbat_cali_cfg_START (0)
#define PMIC_ACRADC_TBAT_CALI_CFG_hkadc2_tbat_cali_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_chopper_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_ACRADC_MODE_CFG_UNION;
#endif
#define PMIC_ACRADC_MODE_CFG_hkadc2_chopper_en_START (0)
#define PMIC_ACRADC_MODE_CFG_hkadc2_chopper_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_cali_offset_l : 8;
    } reg;
} PMIC_ACRADC_CALIVALUE_CFG1_UNION;
#endif
#define PMIC_ACRADC_CALIVALUE_CFG1_hkadc2_cali_offset_l_START (0)
#define PMIC_ACRADC_CALIVALUE_CFG1_hkadc2_cali_offset_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_cali_offset_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACRADC_CALIVALUE_CFG2_UNION;
#endif
#define PMIC_ACRADC_CALIVALUE_CFG2_hkadc2_cali_offset_h_START (0)
#define PMIC_ACRADC_CALIVALUE_CFG2_hkadc2_cali_offset_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_tdie_offset_l : 8;
    } reg;
} PMIC_ACRADC_TDIE_OFFSET_CFGL_UNION;
#endif
#define PMIC_ACRADC_TDIE_OFFSET_CFGL_hkadc2_tdie_offset_l_START (0)
#define PMIC_ACRADC_TDIE_OFFSET_CFGL_hkadc2_tdie_offset_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_tdie_offset_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACRADC_TDIE_OFFSET_CFGH_UNION;
#endif
#define PMIC_ACRADC_TDIE_OFFSET_CFGH_hkadc2_tdie_offset_h_START (0)
#define PMIC_ACRADC_TDIE_OFFSET_CFGH_hkadc2_tdie_offset_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_tbat_offset_l : 8;
    } reg;
} PMIC_ACRADC_TBAT_OFFSET_CFGL_UNION;
#endif
#define PMIC_ACRADC_TBAT_OFFSET_CFGL_hkadc2_tbat_offset_l_START (0)
#define PMIC_ACRADC_TBAT_OFFSET_CFGL_hkadc2_tbat_offset_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char hkadc2_tbat_offset_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACRADC_TBAT_OFFSET_CFGH_UNION;
#endif
#define PMIC_ACRADC_TBAT_OFFSET_CFGH_hkadc2_tbat_offset_h_START (0)
#define PMIC_ACRADC_TBAT_OFFSET_CFGH_hkadc2_tbat_offset_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_soh_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_SOH_EN_UNION;
#endif
#define PMIC_SOH_EN_sc_soh_en_START (0)
#define PMIC_SOH_EN_sc_soh_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_soh_det_timer : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_SOH_DET_TIMER_UNION;
#endif
#define PMIC_SOH_DET_TIMER_sc_soh_det_timer_START (0)
#define PMIC_SOH_DET_TIMER_sc_soh_det_timer_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_soh_tbat_ovh_l : 8;
    } reg;
} PMIC_SOH_TBAT_OVH_L_UNION;
#endif
#define PMIC_SOH_TBAT_OVH_L_sc_soh_tbat_ovh_l_START (0)
#define PMIC_SOH_TBAT_OVH_L_sc_soh_tbat_ovh_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_soh_tbat_ovh_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_SOH_TBAT_OVH_H_UNION;
#endif
#define PMIC_SOH_TBAT_OVH_H_sc_soh_tbat_ovh_h_START (0)
#define PMIC_SOH_TBAT_OVH_H_sc_soh_tbat_ovh_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_soh_vbat_uvp_l : 8;
    } reg;
} PMIC_SOH_VBAT_UVP_L_UNION;
#endif
#define PMIC_SOH_VBAT_UVP_L_sc_soh_vbat_uvp_l_START (0)
#define PMIC_SOH_VBAT_UVP_L_sc_soh_vbat_uvp_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_soh_vbat_uvp_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_SOH_VBAT_UVP_H_UNION;
#endif
#define PMIC_SOH_VBAT_UVP_H_sc_soh_vbat_uvp_h_START (0)
#define PMIC_SOH_VBAT_UVP_H_sc_soh_vbat_uvp_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char soh_mode : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_SOH_MODE_UNION;
#endif
#define PMIC_SOH_MODE_soh_mode_START (0)
#define PMIC_SOH_MODE_soh_mode_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_acr_pulse_num : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_ACR_PULSE_NUM_UNION;
#endif
#define PMIC_ACR_PULSE_NUM_sc_acr_pulse_num_START (0)
#define PMIC_ACR_PULSE_NUM_sc_acr_pulse_num_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_acr_sample_t2 : 3;
        unsigned char sc_acr_sample_t1 : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_ACR_SAMPLE_TIME_H_UNION;
#endif
#define PMIC_ACR_SAMPLE_TIME_H_sc_acr_sample_t2_START (0)
#define PMIC_ACR_SAMPLE_TIME_H_sc_acr_sample_t2_END (2)
#define PMIC_ACR_SAMPLE_TIME_H_sc_acr_sample_t1_START (3)
#define PMIC_ACR_SAMPLE_TIME_H_sc_acr_sample_t1_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_acr_sample_t4 : 3;
        unsigned char sc_acr_sample_t3 : 3;
        unsigned char reserved : 2;
    } reg;
} PMIC_ACR_SAMPLE_TIME_L_UNION;
#endif
#define PMIC_ACR_SAMPLE_TIME_L_sc_acr_sample_t4_START (0)
#define PMIC_ACR_SAMPLE_TIME_L_sc_acr_sample_t4_END (2)
#define PMIC_ACR_SAMPLE_TIME_L_sc_acr_sample_t3_START (3)
#define PMIC_ACR_SAMPLE_TIME_L_sc_acr_sample_t3_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data0_l : 8;
    } reg;
} PMIC_ACR_DATA0_L_UNION;
#endif
#define PMIC_ACR_DATA0_L_acr_data0_l_START (0)
#define PMIC_ACR_DATA0_L_acr_data0_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data0_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACR_DATA0_H_UNION;
#endif
#define PMIC_ACR_DATA0_H_acr_data0_h_START (0)
#define PMIC_ACR_DATA0_H_acr_data0_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data1_l : 8;
    } reg;
} PMIC_ACR_DATA1_L_UNION;
#endif
#define PMIC_ACR_DATA1_L_acr_data1_l_START (0)
#define PMIC_ACR_DATA1_L_acr_data1_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data1_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACR_DATA1_H_UNION;
#endif
#define PMIC_ACR_DATA1_H_acr_data1_h_START (0)
#define PMIC_ACR_DATA1_H_acr_data1_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data2_l : 8;
    } reg;
} PMIC_ACR_DATA2_L_UNION;
#endif
#define PMIC_ACR_DATA2_L_acr_data2_l_START (0)
#define PMIC_ACR_DATA2_L_acr_data2_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data2_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACR_DATA2_H_UNION;
#endif
#define PMIC_ACR_DATA2_H_acr_data2_h_START (0)
#define PMIC_ACR_DATA2_H_acr_data2_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data3_l : 8;
    } reg;
} PMIC_ACR_DATA3_L_UNION;
#endif
#define PMIC_ACR_DATA3_L_acr_data3_l_START (0)
#define PMIC_ACR_DATA3_L_acr_data3_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data3_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACR_DATA3_H_UNION;
#endif
#define PMIC_ACR_DATA3_H_acr_data3_h_START (0)
#define PMIC_ACR_DATA3_H_acr_data3_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data4_l : 8;
    } reg;
} PMIC_ACR_DATA4_L_UNION;
#endif
#define PMIC_ACR_DATA4_L_acr_data4_l_START (0)
#define PMIC_ACR_DATA4_L_acr_data4_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data4_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACR_DATA4_H_UNION;
#endif
#define PMIC_ACR_DATA4_H_acr_data4_h_START (0)
#define PMIC_ACR_DATA4_H_acr_data4_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data5_l : 8;
    } reg;
} PMIC_ACR_DATA5_L_UNION;
#endif
#define PMIC_ACR_DATA5_L_acr_data5_l_START (0)
#define PMIC_ACR_DATA5_L_acr_data5_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data5_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACR_DATA5_H_UNION;
#endif
#define PMIC_ACR_DATA5_H_acr_data5_h_START (0)
#define PMIC_ACR_DATA5_H_acr_data5_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data6_l : 8;
    } reg;
} PMIC_ACR_DATA6_L_UNION;
#endif
#define PMIC_ACR_DATA6_L_acr_data6_l_START (0)
#define PMIC_ACR_DATA6_L_acr_data6_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data6_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACR_DATA6_H_UNION;
#endif
#define PMIC_ACR_DATA6_H_acr_data6_h_START (0)
#define PMIC_ACR_DATA6_H_acr_data6_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data7_l : 8;
    } reg;
} PMIC_ACR_DATA7_L_UNION;
#endif
#define PMIC_ACR_DATA7_L_acr_data7_l_START (0)
#define PMIC_ACR_DATA7_L_acr_data7_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_data7_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_ACR_DATA7_H_UNION;
#endif
#define PMIC_ACR_DATA7_H_acr_data7_h_START (0)
#define PMIC_ACR_DATA7_H_acr_data7_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_ovp_timer : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_OVP_CTRL_UNION;
#endif
#define PMIC_OVP_CTRL_sc_ovp_timer_START (0)
#define PMIC_OVP_CTRL_sc_ovp_timer_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_ovp_vbat_th0_l : 8;
    } reg;
} PMIC_OVP_VBAT_OVH_TH0_L_UNION;
#endif
#define PMIC_OVP_VBAT_OVH_TH0_L_sc_ovp_vbat_th0_l_START (0)
#define PMIC_OVP_VBAT_OVH_TH0_L_sc_ovp_vbat_th0_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_ovp_vbat_th0_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_OVP_VBAT_OVH_TH0_H_UNION;
#endif
#define PMIC_OVP_VBAT_OVH_TH0_H_sc_ovp_vbat_th0_h_START (0)
#define PMIC_OVP_VBAT_OVH_TH0_H_sc_ovp_vbat_th0_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_ovp_tbat_th0_l : 8;
    } reg;
} PMIC_OVP_TBAT_OVH_TH0_L_UNION;
#endif
#define PMIC_OVP_TBAT_OVH_TH0_L_sc_ovp_tbat_th0_l_START (0)
#define PMIC_OVP_TBAT_OVH_TH0_L_sc_ovp_tbat_th0_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_ovp_tbat_th0_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_OVP_TBAT_OVH_TH0_H_UNION;
#endif
#define PMIC_OVP_TBAT_OVH_TH0_H_sc_ovp_tbat_th0_h_START (0)
#define PMIC_OVP_TBAT_OVH_TH0_H_sc_ovp_tbat_th0_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_ovp_vbat_th1_l : 8;
    } reg;
} PMIC_OVP_VBAT_OVH_TH1_L_UNION;
#endif
#define PMIC_OVP_VBAT_OVH_TH1_L_sc_ovp_vbat_th1_l_START (0)
#define PMIC_OVP_VBAT_OVH_TH1_L_sc_ovp_vbat_th1_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_ovp_vbat_th1_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_OVP_VBAT_OVH_TH1_H_UNION;
#endif
#define PMIC_OVP_VBAT_OVH_TH1_H_sc_ovp_vbat_th1_h_START (0)
#define PMIC_OVP_VBAT_OVH_TH1_H_sc_ovp_vbat_th1_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_ovp_tbat_th1_l : 8;
    } reg;
} PMIC_OVP_TBAT_OVH_TH1_L_UNION;
#endif
#define PMIC_OVP_TBAT_OVH_TH1_L_sc_ovp_tbat_th1_l_START (0)
#define PMIC_OVP_TBAT_OVH_TH1_L_sc_ovp_tbat_th1_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_ovp_tbat_th1_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_OVP_TBAT_OVH_TH1_H_UNION;
#endif
#define PMIC_OVP_TBAT_OVH_TH1_H_sc_ovp_tbat_th1_h_START (0)
#define PMIC_OVP_TBAT_OVH_TH1_H_sc_ovp_tbat_th1_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_ovp_vbat_th2_l : 8;
    } reg;
} PMIC_OVP_VBAT_OVH_TH2_L_UNION;
#endif
#define PMIC_OVP_VBAT_OVH_TH2_L_sc_ovp_vbat_th2_l_START (0)
#define PMIC_OVP_VBAT_OVH_TH2_L_sc_ovp_vbat_th2_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_ovp_vbat_th2_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_OVP_VBAT_OVH_TH2_H_UNION;
#endif
#define PMIC_OVP_VBAT_OVH_TH2_H_sc_ovp_vbat_th2_h_START (0)
#define PMIC_OVP_VBAT_OVH_TH2_H_sc_ovp_vbat_th2_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_ovp_tbat_th2_l : 8;
    } reg;
} PMIC_OVP_TBAT_OVH_TH2_L_UNION;
#endif
#define PMIC_OVP_TBAT_OVH_TH2_L_sc_ovp_tbat_th2_l_START (0)
#define PMIC_OVP_TBAT_OVH_TH2_L_sc_ovp_tbat_th2_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_ovp_tbat_th2_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_OVP_TBAT_OVH_TH2_H_UNION;
#endif
#define PMIC_OVP_TBAT_OVH_TH2_H_sc_ovp_tbat_th2_h_START (0)
#define PMIC_OVP_TBAT_OVH_TH2_H_sc_ovp_tbat_th2_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char st_soh_ovp : 1;
        unsigned char st_tmp_ovh_2 : 1;
        unsigned char st_soh_ovh_2 : 1;
        unsigned char st_tmp_ovh_1 : 1;
        unsigned char st_soh_ovh_1 : 1;
        unsigned char st_tmp_ovh_0 : 1;
        unsigned char st_soh_ovh_0 : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_SOH_OVP_REAL_UNION;
#endif
#define PMIC_SOH_OVP_REAL_st_soh_ovp_START (0)
#define PMIC_SOH_OVP_REAL_st_soh_ovp_END (0)
#define PMIC_SOH_OVP_REAL_st_tmp_ovh_2_START (1)
#define PMIC_SOH_OVP_REAL_st_tmp_ovh_2_END (1)
#define PMIC_SOH_OVP_REAL_st_soh_ovh_2_START (2)
#define PMIC_SOH_OVP_REAL_st_soh_ovh_2_END (2)
#define PMIC_SOH_OVP_REAL_st_tmp_ovh_1_START (3)
#define PMIC_SOH_OVP_REAL_st_tmp_ovh_1_END (3)
#define PMIC_SOH_OVP_REAL_st_soh_ovh_1_START (4)
#define PMIC_SOH_OVP_REAL_st_soh_ovh_1_END (4)
#define PMIC_SOH_OVP_REAL_st_tmp_ovh_0_START (5)
#define PMIC_SOH_OVP_REAL_st_tmp_ovh_0_END (5)
#define PMIC_SOH_OVP_REAL_st_soh_ovh_0_START (6)
#define PMIC_SOH_OVP_REAL_st_soh_ovh_0_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ovp_discharge_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_OVP_DISCHARGE_CTRL_UNION;
#endif
#define PMIC_OVP_DISCHARGE_CTRL_ovp_discharge_sel_START (0)
#define PMIC_OVP_DISCHARGE_CTRL_ovp_discharge_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char discharge_sel : 1;
        unsigned char eco_sel : 1;
        unsigned char twp_sel : 1;
        unsigned char cic_sel : 1;
        unsigned char reserved : 4;
    } reg;
} PMIC_DCR_CONFIG_UNION;
#endif
#define PMIC_DCR_CONFIG_discharge_sel_START (0)
#define PMIC_DCR_CONFIG_discharge_sel_END (0)
#define PMIC_DCR_CONFIG_eco_sel_START (1)
#define PMIC_DCR_CONFIG_eco_sel_END (1)
#define PMIC_DCR_CONFIG_twp_sel_START (2)
#define PMIC_DCR_CONFIG_twp_sel_END (2)
#define PMIC_DCR_CONFIG_cic_sel_START (3)
#define PMIC_DCR_CONFIG_cic_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char timer_set2 : 4;
        unsigned char timer_set1 : 4;
    } reg;
} PMIC_DCR_TIMER_CONFIG0_UNION;
#endif
#define PMIC_DCR_TIMER_CONFIG0_timer_set2_START (0)
#define PMIC_DCR_TIMER_CONFIG0_timer_set2_END (3)
#define PMIC_DCR_TIMER_CONFIG0_timer_set1_START (4)
#define PMIC_DCR_TIMER_CONFIG0_timer_set1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char timer_set3 : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_DCR_TIMER_CONFIG1_UNION;
#endif
#define PMIC_DCR_TIMER_CONFIG1_timer_set3_START (0)
#define PMIC_DCR_TIMER_CONFIG1_timer_set3_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char count : 8;
    } reg;
} PMIC_DCR_COUNT_CONFIG_UNION;
#endif
#define PMIC_DCR_COUNT_CONFIG_count_START (0)
#define PMIC_DCR_COUNT_CONFIG_count_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ibat1_s0 : 8;
    } reg;
} PMIC_IBAT1_0_UNION;
#endif
#define PMIC_IBAT1_0_ibat1_s0_START (0)
#define PMIC_IBAT1_0_ibat1_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ibat1_s1 : 8;
    } reg;
} PMIC_IBAT1_1_UNION;
#endif
#define PMIC_IBAT1_1_ibat1_s1_START (0)
#define PMIC_IBAT1_1_ibat1_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ibat1_s2 : 8;
    } reg;
} PMIC_IBAT1_2_UNION;
#endif
#define PMIC_IBAT1_2_ibat1_s2_START (0)
#define PMIC_IBAT1_2_ibat1_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char vbat1_s0 : 8;
    } reg;
} PMIC_VBAT1_0_UNION;
#endif
#define PMIC_VBAT1_0_vbat1_s0_START (0)
#define PMIC_VBAT1_0_vbat1_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char vbat1_s1 : 8;
    } reg;
} PMIC_VBAT1_1_UNION;
#endif
#define PMIC_VBAT1_1_vbat1_s1_START (0)
#define PMIC_VBAT1_1_vbat1_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char vbat1_s2 : 8;
    } reg;
} PMIC_VBAT1_2_UNION;
#endif
#define PMIC_VBAT1_2_vbat1_s2_START (0)
#define PMIC_VBAT1_2_vbat1_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char tmr1_s0 : 8;
    } reg;
} PMIC_TMR1_0_UNION;
#endif
#define PMIC_TMR1_0_tmr1_s0_START (0)
#define PMIC_TMR1_0_tmr1_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char tmr1_s1 : 8;
    } reg;
} PMIC_TMR1_1_UNION;
#endif
#define PMIC_TMR1_1_tmr1_s1_START (0)
#define PMIC_TMR1_1_tmr1_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ibat2_s0 : 8;
    } reg;
} PMIC_IBAT2_0_UNION;
#endif
#define PMIC_IBAT2_0_ibat2_s0_START (0)
#define PMIC_IBAT2_0_ibat2_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ibat2_s1 : 8;
    } reg;
} PMIC_IBAT2_1_UNION;
#endif
#define PMIC_IBAT2_1_ibat2_s1_START (0)
#define PMIC_IBAT2_1_ibat2_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ibat2_s2 : 8;
    } reg;
} PMIC_IBAT2_2_UNION;
#endif
#define PMIC_IBAT2_2_ibat2_s2_START (0)
#define PMIC_IBAT2_2_ibat2_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char vbat2_s0 : 8;
    } reg;
} PMIC_VBAT2_0_UNION;
#endif
#define PMIC_VBAT2_0_vbat2_s0_START (0)
#define PMIC_VBAT2_0_vbat2_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char vbat2_s1 : 8;
    } reg;
} PMIC_VBAT2_1_UNION;
#endif
#define PMIC_VBAT2_1_vbat2_s1_START (0)
#define PMIC_VBAT2_1_vbat2_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char vbat2_s2 : 8;
    } reg;
} PMIC_VBAT2_2_UNION;
#endif
#define PMIC_VBAT2_2_vbat2_s2_START (0)
#define PMIC_VBAT2_2_vbat2_s2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char tmr2_s0 : 8;
    } reg;
} PMIC_TMR2_0_UNION;
#endif
#define PMIC_TMR2_0_tmr2_s0_START (0)
#define PMIC_TMR2_0_tmr2_s0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char tmr2_s1 : 8;
    } reg;
} PMIC_TMR2_1_UNION;
#endif
#define PMIC_TMR2_1_tmr2_s1_START (0)
#define PMIC_TMR2_1_tmr2_s1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_soh_soft_rst : 8;
    } reg;
} PMIC_SOH_SOFT_RST_UNION;
#endif
#define PMIC_SOH_SOFT_RST_sc_soh_soft_rst_START (0)
#define PMIC_SOH_SOFT_RST_sc_soh_soft_rst_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_acr_tb_sel : 4;
        unsigned char sc_acr_tb_en : 1;
        unsigned char cic_sample_cfg : 2;
        unsigned char reserved : 1;
    } reg;
} PMIC_TEST_BUS_SEL_UNION;
#endif
#define PMIC_TEST_BUS_SEL_sc_acr_tb_sel_START (0)
#define PMIC_TEST_BUS_SEL_sc_acr_tb_sel_END (3)
#define PMIC_TEST_BUS_SEL_sc_acr_tb_en_START (4)
#define PMIC_TEST_BUS_SEL_sc_acr_tb_en_END (4)
#define PMIC_TEST_BUS_SEL_cic_sample_cfg_START (5)
#define PMIC_TEST_BUS_SEL_cic_sample_cfg_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_tb_bus_0 : 8;
    } reg;
} PMIC_ACR_TB_BUS_0_UNION;
#endif
#define PMIC_ACR_TB_BUS_0_acr_tb_bus_0_START (0)
#define PMIC_ACR_TB_BUS_0_acr_tb_bus_0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char acr_tb_bus_1 : 8;
    } reg;
} PMIC_ACR_TB_BUS_1_UNION;
#endif
#define PMIC_ACR_TB_BUS_1_acr_tb_bus_1_START (0)
#define PMIC_ACR_TB_BUS_1_acr_tb_bus_1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char sc_debug_acr : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_ACR_CLK_GT_EN_UNION;
#endif
#define PMIC_ACR_CLK_GT_EN_sc_debug_acr_START (0)
#define PMIC_ACR_CLK_GT_EN_sc_debug_acr_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_disen_curr_chanel : 2;
        unsigned char eis_disen_volt_chanel : 2;
        unsigned char eis_chopper2_en : 1;
        unsigned char eis_chopper1_en : 1;
        unsigned char reserved : 2;
    } reg;
} PMIC_EIS_ANA_CTRL0_UNION;
#endif
#define PMIC_EIS_ANA_CTRL0_eis_disen_curr_chanel_START (0)
#define PMIC_EIS_ANA_CTRL0_eis_disen_curr_chanel_END (1)
#define PMIC_EIS_ANA_CTRL0_eis_disen_volt_chanel_START (2)
#define PMIC_EIS_ANA_CTRL0_eis_disen_volt_chanel_END (3)
#define PMIC_EIS_ANA_CTRL0_eis_chopper2_en_START (4)
#define PMIC_EIS_ANA_CTRL0_eis_chopper2_en_END (4)
#define PMIC_EIS_ANA_CTRL0_eis_chopper1_en_START (5)
#define PMIC_EIS_ANA_CTRL0_eis_chopper1_en_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_curr_gain_set : 3;
        unsigned char eis_volt_gain_set : 3;
        unsigned char eis_disc_curr_set : 2;
    } reg;
} PMIC_EIS_ANA_CTRL1_UNION;
#endif
#define PMIC_EIS_ANA_CTRL1_eis_curr_gain_set_START (0)
#define PMIC_EIS_ANA_CTRL1_eis_curr_gain_set_END (2)
#define PMIC_EIS_ANA_CTRL1_eis_volt_gain_set_START (3)
#define PMIC_EIS_ANA_CTRL1_eis_volt_gain_set_END (5)
#define PMIC_EIS_ANA_CTRL1_eis_disc_curr_set_START (6)
#define PMIC_EIS_ANA_CTRL1_eis_disc_curr_set_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_cur_ampcap_sel : 2;
        unsigned char eis_cur_rescap_sel : 2;
        unsigned char eis_volt_cap_sel : 2;
        unsigned char eis_volt_rescap_sel : 2;
    } reg;
} PMIC_EIS_ANA_CTRL2_UNION;
#endif
#define PMIC_EIS_ANA_CTRL2_eis_cur_ampcap_sel_START (0)
#define PMIC_EIS_ANA_CTRL2_eis_cur_ampcap_sel_END (1)
#define PMIC_EIS_ANA_CTRL2_eis_cur_rescap_sel_START (2)
#define PMIC_EIS_ANA_CTRL2_eis_cur_rescap_sel_END (3)
#define PMIC_EIS_ANA_CTRL2_eis_volt_cap_sel_START (4)
#define PMIC_EIS_ANA_CTRL2_eis_volt_cap_sel_END (5)
#define PMIC_EIS_ANA_CTRL2_eis_volt_rescap_sel_START (6)
#define PMIC_EIS_ANA_CTRL2_eis_volt_rescap_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_ibias_set_0 : 8;
    } reg;
} PMIC_EIS_IBIAS_SET_0_UNION;
#endif
#define PMIC_EIS_IBIAS_SET_0_eis_ibias_set_0_START (0)
#define PMIC_EIS_IBIAS_SET_0_eis_ibias_set_0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_ibias_set_1 : 8;
    } reg;
} PMIC_EIS_IBIAS_SET_1_UNION;
#endif
#define PMIC_EIS_IBIAS_SET_1_eis_ibias_set_1_START (0)
#define PMIC_EIS_IBIAS_SET_1_eis_ibias_set_1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_testmode : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_EIS_TEST_CTRL_UNION;
#endif
#define PMIC_EIS_TEST_CTRL_eis_testmode_START (0)
#define PMIC_EIS_TEST_CTRL_eis_testmode_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_ana_reserve0 : 8;
    } reg;
} PMIC_EIS_RESERVE0_UNION;
#endif
#define PMIC_EIS_RESERVE0_eis_ana_reserve0_START (0)
#define PMIC_EIS_RESERVE0_eis_ana_reserve0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_ana_reserve1 : 8;
    } reg;
} PMIC_EIS_RESERVE1_UNION;
#endif
#define PMIC_EIS_RESERVE1_eis_ana_reserve1_START (0)
#define PMIC_EIS_RESERVE1_eis_ana_reserve1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_ana_reserve2 : 8;
    } reg;
} PMIC_EIS_RESERVE2_UNION;
#endif
#define PMIC_EIS_RESERVE2_eis_ana_reserve2_START (0)
#define PMIC_EIS_RESERVE2_eis_ana_reserve2_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_EIS_CTRL0_UNION;
#endif
#define PMIC_EIS_CTRL0_reg_eis_en_START (0)
#define PMIC_EIS_CTRL0_reg_eis_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_dischg_mode : 1;
        unsigned char reg_eis_dischg_en : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_EIS_CTRL1_UNION;
#endif
#define PMIC_EIS_CTRL1_reg_eis_dischg_mode_START (0)
#define PMIC_EIS_CTRL1_reg_eis_dischg_mode_END (0)
#define PMIC_EIS_CTRL1_reg_eis_dischg_en_START (1)
#define PMIC_EIS_CTRL1_reg_eis_dischg_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_tw : 2;
        unsigned char reg_eis_init_chanl2start : 2;
        unsigned char reg_eis_init_adc_average : 2;
        unsigned char reg_eis_init_start : 1;
        unsigned char reserved : 1;
    } reg;
} PMIC_EIS_CTRL2_UNION;
#endif
#define PMIC_EIS_CTRL2_reg_eis_tw_START (0)
#define PMIC_EIS_CTRL2_reg_eis_tw_END (1)
#define PMIC_EIS_CTRL2_reg_eis_init_chanl2start_START (2)
#define PMIC_EIS_CTRL2_reg_eis_init_chanl2start_END (3)
#define PMIC_EIS_CTRL2_reg_eis_init_adc_average_START (4)
#define PMIC_EIS_CTRL2_reg_eis_init_adc_average_END (5)
#define PMIC_EIS_CTRL2_reg_eis_init_start_START (6)
#define PMIC_EIS_CTRL2_reg_eis_init_start_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eisadc_init_v_offset_data_l : 8;
    } reg;
} PMIC_EISADC_INIT_V_OFFSET_DATA_L_UNION;
#endif
#define PMIC_EISADC_INIT_V_OFFSET_DATA_L_eisadc_init_v_offset_data_l_START (0)
#define PMIC_EISADC_INIT_V_OFFSET_DATA_L_eisadc_init_v_offset_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eisadc_init_v_offset_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EISADC_INIT_V_OFFSET_DATA_H_UNION;
#endif
#define PMIC_EISADC_INIT_V_OFFSET_DATA_H_eisadc_init_v_offset_data_h_START (0)
#define PMIC_EISADC_INIT_V_OFFSET_DATA_H_eisadc_init_v_offset_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eisadc_init_i_offset_data_l : 8;
    } reg;
} PMIC_EISADC_INIT_I_OFFSET_DATA_L_UNION;
#endif
#define PMIC_EISADC_INIT_I_OFFSET_DATA_L_eisadc_init_i_offset_data_l_START (0)
#define PMIC_EISADC_INIT_I_OFFSET_DATA_L_eisadc_init_i_offset_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eisadc_init_i_offset_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EISADC_INIT_I_OFFSET_DATA_H_UNION;
#endif
#define PMIC_EISADC_INIT_I_OFFSET_DATA_H_eisadc_init_i_offset_data_h_START (0)
#define PMIC_EISADC_INIT_I_OFFSET_DATA_H_eisadc_init_i_offset_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eisadc_init_i_data_l : 8;
    } reg;
} PMIC_EISADC_INIT_I_DATA_L_UNION;
#endif
#define PMIC_EISADC_INIT_I_DATA_L_eisadc_init_i_data_l_START (0)
#define PMIC_EISADC_INIT_I_DATA_L_eisadc_init_i_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eisadc_init_i_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EISADC_INIT_I_DATA_H_UNION;
#endif
#define PMIC_EISADC_INIT_I_DATA_H_eisadc_init_i_data_h_START (0)
#define PMIC_EISADC_INIT_I_DATA_H_eisadc_init_i_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eisadc_t_data_l : 8;
    } reg;
} PMIC_EISADC_T_DATA_L_UNION;
#endif
#define PMIC_EISADC_T_DATA_L_eisadc_t_data_l_START (0)
#define PMIC_EISADC_T_DATA_L_eisadc_t_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eisadc_t_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EISADC_T_DATA_H_UNION;
#endif
#define PMIC_EISADC_T_DATA_H_eisadc_t_data_h_START (0)
#define PMIC_EISADC_T_DATA_H_eisadc_t_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eisadc_init_v_abs_data_l : 8;
    } reg;
} PMIC_EISADC_INIT_V_ABS_DATA_L_UNION;
#endif
#define PMIC_EISADC_INIT_V_ABS_DATA_L_eisadc_init_v_abs_data_l_START (0)
#define PMIC_EISADC_INIT_V_ABS_DATA_L_eisadc_init_v_abs_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eisadc_init_v_abs_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EISADC_INIT_V_ABS_DATA_H_UNION;
#endif
#define PMIC_EISADC_INIT_V_ABS_DATA_H_eisadc_init_v_abs_data_h_START (0)
#define PMIC_EISADC_INIT_V_ABS_DATA_H_eisadc_init_v_abs_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eisadc_init_v_rela_data_l : 8;
    } reg;
} PMIC_EISADC_INIT_V_RELA_DATA_L_UNION;
#endif
#define PMIC_EISADC_INIT_V_RELA_DATA_L_eisadc_init_v_rela_data_l_START (0)
#define PMIC_EISADC_INIT_V_RELA_DATA_L_eisadc_init_v_rela_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eisadc_init_v_rela_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EISADC_INIT_V_RELA_DATA_H_UNION;
#endif
#define PMIC_EISADC_INIT_V_RELA_DATA_H_eisadc_init_v_rela_data_h_START (0)
#define PMIC_EISADC_INIT_V_RELA_DATA_H_eisadc_init_v_rela_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eisadc_init_vbat0_data_l : 8;
    } reg;
} PMIC_EISADC_INIT_VBAT0_DATA_L_UNION;
#endif
#define PMIC_EISADC_INIT_VBAT0_DATA_L_eisadc_init_vbat0_data_l_START (0)
#define PMIC_EISADC_INIT_VBAT0_DATA_L_eisadc_init_vbat0_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eisadc_init_vbat0_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EISADC_INIT_VBAT0_DATA_H_UNION;
#endif
#define PMIC_EISADC_INIT_VBAT0_DATA_H_eisadc_init_vbat0_data_h_START (0)
#define PMIC_EISADC_INIT_VBAT0_DATA_H_eisadc_init_vbat0_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_det_timer : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EIS_DET_TIMER_UNION;
#endif
#define PMIC_EIS_DET_TIMER_reg_eis_det_timer_START (0)
#define PMIC_EIS_DET_TIMER_reg_eis_det_timer_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_det_timer_n : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EIS_DET_TIMER_N_UNION;
#endif
#define PMIC_EIS_DET_TIMER_N_reg_eis_det_timer_n_START (0)
#define PMIC_EIS_DET_TIMER_N_reg_eis_det_timer_n_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_det_timer_m_l : 8;
    } reg;
} PMIC_EIS_DET_TIMER_M_L_UNION;
#endif
#define PMIC_EIS_DET_TIMER_M_L_reg_eis_det_timer_m_l_START (0)
#define PMIC_EIS_DET_TIMER_M_L_reg_eis_det_timer_m_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_det_timer_m_h : 2;
        unsigned char reserved : 6;
    } reg;
} PMIC_EIS_DET_TIMER_M_H_UNION;
#endif
#define PMIC_EIS_DET_TIMER_M_H_reg_eis_det_timer_m_h_START (0)
#define PMIC_EIS_DET_TIMER_M_H_reg_eis_det_timer_m_h_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_ibat_timer_k : 3;
        unsigned char reg_eis_ibat_timer_j : 2;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_IBAT_DET_CTRL_UNION;
#endif
#define PMIC_EIS_IBAT_DET_CTRL_reg_eis_ibat_timer_k_START (0)
#define PMIC_EIS_IBAT_DET_CTRL_reg_eis_ibat_timer_k_END (2)
#define PMIC_EIS_IBAT_DET_CTRL_reg_eis_ibat_timer_j_START (3)
#define PMIC_EIS_IBAT_DET_CTRL_reg_eis_ibat_timer_j_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_i_average : 1;
        unsigned char reg_eis_v_average : 1;
        unsigned char reserved : 6;
    } reg;
} PMIC_EIS_ADC_AVERAGE_UNION;
#endif
#define PMIC_EIS_ADC_AVERAGE_reg_eis_i_average_START (0)
#define PMIC_EIS_ADC_AVERAGE_reg_eis_i_average_END (0)
#define PMIC_EIS_ADC_AVERAGE_reg_eis_v_average_START (1)
#define PMIC_EIS_ADC_AVERAGE_reg_eis_v_average_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_vbat0_offset_l : 8;
    } reg;
} PMIC_EIS_VBAT0_OFFSET_L_UNION;
#endif
#define PMIC_EIS_VBAT0_OFFSET_L_reg_eis_vbat0_offset_l_START (0)
#define PMIC_EIS_VBAT0_OFFSET_L_reg_eis_vbat0_offset_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_vbat0_offset_h : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_EIS_VBAT0_OFFSET_H_UNION;
#endif
#define PMIC_EIS_VBAT0_OFFSET_H_reg_eis_vbat0_offset_h_START (0)
#define PMIC_EIS_VBAT0_OFFSET_H_reg_eis_vbat0_offset_h_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_vbat0_offset_sel : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_EIS_VBAT0_OFFSET_SEL_UNION;
#endif
#define PMIC_EIS_VBAT0_OFFSET_SEL_reg_eis_vbat0_offset_sel_START (0)
#define PMIC_EIS_VBAT0_OFFSET_SEL_reg_eis_vbat0_offset_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_hthreshold_err : 8;
    } reg;
} PMIC_EIS_HTHRESHOLD_ERROR_UNION;
#endif
#define PMIC_EIS_HTHRESHOLD_ERROR_reg_eis_hthreshold_err_START (0)
#define PMIC_EIS_HTHRESHOLD_ERROR_reg_eis_hthreshold_err_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_icomp_hthreshold_l : 8;
    } reg;
} PMIC_EIS_ICOMP_HTHRESHOLD_L_UNION;
#endif
#define PMIC_EIS_ICOMP_HTHRESHOLD_L_reg_eis_icomp_hthreshold_l_START (0)
#define PMIC_EIS_ICOMP_HTHRESHOLD_L_reg_eis_icomp_hthreshold_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_icomp_hthreshold_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EIS_ICOMP_HTHRESHOLD_H_UNION;
#endif
#define PMIC_EIS_ICOMP_HTHRESHOLD_H_reg_eis_icomp_hthreshold_h_START (0)
#define PMIC_EIS_ICOMP_HTHRESHOLD_H_reg_eis_icomp_hthreshold_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_lthreshold_err : 8;
    } reg;
} PMIC_EIS_LTHRESHOLD_ERROR_UNION;
#endif
#define PMIC_EIS_LTHRESHOLD_ERROR_reg_eis_lthreshold_err_START (0)
#define PMIC_EIS_LTHRESHOLD_ERROR_reg_eis_lthreshold_err_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_icomp_lthreshold_l : 8;
    } reg;
} PMIC_EIS_ICOMP_LTHRESHOLD_L_UNION;
#endif
#define PMIC_EIS_ICOMP_LTHRESHOLD_L_reg_eis_icomp_lthreshold_l_START (0)
#define PMIC_EIS_ICOMP_LTHRESHOLD_L_reg_eis_icomp_lthreshold_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_icomp_lthreshold_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EIS_ICOMP_LTHRESHOLD_H_UNION;
#endif
#define PMIC_EIS_ICOMP_LTHRESHOLD_H_reg_eis_icomp_lthreshold_h_START (0)
#define PMIC_EIS_ICOMP_LTHRESHOLD_H_reg_eis_icomp_lthreshold_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reg_eis_icomp_state_clr : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_EIS_ICOMP_STATE_CLR_UNION;
#endif
#define PMIC_EIS_ICOMP_STATE_CLR_reg_eis_icomp_state_clr_START (0)
#define PMIC_EIS_ICOMP_STATE_CLR_reg_eis_icomp_state_clr_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_i_curr_data_l : 8;
    } reg;
} PMIC_EIS_I_CURRENT_L_UNION;
#endif
#define PMIC_EIS_I_CURRENT_L_eis_i_curr_data_l_START (0)
#define PMIC_EIS_I_CURRENT_L_eis_i_curr_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_i_curr_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EIS_I_CURRENT_H_UNION;
#endif
#define PMIC_EIS_I_CURRENT_H_eis_i_curr_data_h_START (0)
#define PMIC_EIS_I_CURRENT_H_eis_i_curr_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d1_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D1_L_UNION;
#endif
#define PMIC_EIS_VBAT_D1_L_eis_vbat_d1_data_l_START (0)
#define PMIC_EIS_VBAT_D1_L_eis_vbat_d1_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d1_data_h : 4;
        unsigned char eis_i_accu_state_d1 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D1_H_UNION;
#endif
#define PMIC_EIS_VBAT_D1_H_eis_vbat_d1_data_h_START (0)
#define PMIC_EIS_VBAT_D1_H_eis_vbat_d1_data_h_END (3)
#define PMIC_EIS_VBAT_D1_H_eis_i_accu_state_d1_START (4)
#define PMIC_EIS_VBAT_D1_H_eis_i_accu_state_d1_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d2_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D2_L_UNION;
#endif
#define PMIC_EIS_VBAT_D2_L_eis_vbat_d2_data_l_START (0)
#define PMIC_EIS_VBAT_D2_L_eis_vbat_d2_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d2_data_h : 4;
        unsigned char eis_i_accu_state_d2 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D2_H_UNION;
#endif
#define PMIC_EIS_VBAT_D2_H_eis_vbat_d2_data_h_START (0)
#define PMIC_EIS_VBAT_D2_H_eis_vbat_d2_data_h_END (3)
#define PMIC_EIS_VBAT_D2_H_eis_i_accu_state_d2_START (4)
#define PMIC_EIS_VBAT_D2_H_eis_i_accu_state_d2_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d3_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D3_L_UNION;
#endif
#define PMIC_EIS_VBAT_D3_L_eis_vbat_d3_data_l_START (0)
#define PMIC_EIS_VBAT_D3_L_eis_vbat_d3_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d3_data_h : 4;
        unsigned char eis_i_accu_state_d3 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D3_H_UNION;
#endif
#define PMIC_EIS_VBAT_D3_H_eis_vbat_d3_data_h_START (0)
#define PMIC_EIS_VBAT_D3_H_eis_vbat_d3_data_h_END (3)
#define PMIC_EIS_VBAT_D3_H_eis_i_accu_state_d3_START (4)
#define PMIC_EIS_VBAT_D3_H_eis_i_accu_state_d3_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d4_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D4_L_UNION;
#endif
#define PMIC_EIS_VBAT_D4_L_eis_vbat_d4_data_l_START (0)
#define PMIC_EIS_VBAT_D4_L_eis_vbat_d4_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d4_data_h : 4;
        unsigned char eis_i_accu_state_d4 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D4_H_UNION;
#endif
#define PMIC_EIS_VBAT_D4_H_eis_vbat_d4_data_h_START (0)
#define PMIC_EIS_VBAT_D4_H_eis_vbat_d4_data_h_END (3)
#define PMIC_EIS_VBAT_D4_H_eis_i_accu_state_d4_START (4)
#define PMIC_EIS_VBAT_D4_H_eis_i_accu_state_d4_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d5_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D5_L_UNION;
#endif
#define PMIC_EIS_VBAT_D5_L_eis_vbat_d5_data_l_START (0)
#define PMIC_EIS_VBAT_D5_L_eis_vbat_d5_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d5_data_h : 4;
        unsigned char eis_i_accu_state_d5 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D5_H_UNION;
#endif
#define PMIC_EIS_VBAT_D5_H_eis_vbat_d5_data_h_START (0)
#define PMIC_EIS_VBAT_D5_H_eis_vbat_d5_data_h_END (3)
#define PMIC_EIS_VBAT_D5_H_eis_i_accu_state_d5_START (4)
#define PMIC_EIS_VBAT_D5_H_eis_i_accu_state_d5_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d6_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D6_L_UNION;
#endif
#define PMIC_EIS_VBAT_D6_L_eis_vbat_d6_data_l_START (0)
#define PMIC_EIS_VBAT_D6_L_eis_vbat_d6_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d6_data_h : 4;
        unsigned char eis_i_accu_state_d6 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D6_H_UNION;
#endif
#define PMIC_EIS_VBAT_D6_H_eis_vbat_d6_data_h_START (0)
#define PMIC_EIS_VBAT_D6_H_eis_vbat_d6_data_h_END (3)
#define PMIC_EIS_VBAT_D6_H_eis_i_accu_state_d6_START (4)
#define PMIC_EIS_VBAT_D6_H_eis_i_accu_state_d6_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d7_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D7_L_UNION;
#endif
#define PMIC_EIS_VBAT_D7_L_eis_vbat_d7_data_l_START (0)
#define PMIC_EIS_VBAT_D7_L_eis_vbat_d7_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d7_data_h : 4;
        unsigned char eis_i_accu_state_d7 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D7_H_UNION;
#endif
#define PMIC_EIS_VBAT_D7_H_eis_vbat_d7_data_h_START (0)
#define PMIC_EIS_VBAT_D7_H_eis_vbat_d7_data_h_END (3)
#define PMIC_EIS_VBAT_D7_H_eis_i_accu_state_d7_START (4)
#define PMIC_EIS_VBAT_D7_H_eis_i_accu_state_d7_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d8_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D8_L_UNION;
#endif
#define PMIC_EIS_VBAT_D8_L_eis_vbat_d8_data_l_START (0)
#define PMIC_EIS_VBAT_D8_L_eis_vbat_d8_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d8_data_h : 4;
        unsigned char eis_i_accu_state_d8 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D8_H_UNION;
#endif
#define PMIC_EIS_VBAT_D8_H_eis_vbat_d8_data_h_START (0)
#define PMIC_EIS_VBAT_D8_H_eis_vbat_d8_data_h_END (3)
#define PMIC_EIS_VBAT_D8_H_eis_i_accu_state_d8_START (4)
#define PMIC_EIS_VBAT_D8_H_eis_i_accu_state_d8_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d9_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D9_L_UNION;
#endif
#define PMIC_EIS_VBAT_D9_L_eis_vbat_d9_data_l_START (0)
#define PMIC_EIS_VBAT_D9_L_eis_vbat_d9_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d9_data_h : 4;
        unsigned char eis_i_accu_state_d9 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D9_H_UNION;
#endif
#define PMIC_EIS_VBAT_D9_H_eis_vbat_d9_data_h_START (0)
#define PMIC_EIS_VBAT_D9_H_eis_vbat_d9_data_h_END (3)
#define PMIC_EIS_VBAT_D9_H_eis_i_accu_state_d9_START (4)
#define PMIC_EIS_VBAT_D9_H_eis_i_accu_state_d9_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d10_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D10_L_UNION;
#endif
#define PMIC_EIS_VBAT_D10_L_eis_vbat_d10_data_l_START (0)
#define PMIC_EIS_VBAT_D10_L_eis_vbat_d10_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d10_data_h : 4;
        unsigned char eis_i_accu_state_d10 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D10_H_UNION;
#endif
#define PMIC_EIS_VBAT_D10_H_eis_vbat_d10_data_h_START (0)
#define PMIC_EIS_VBAT_D10_H_eis_vbat_d10_data_h_END (3)
#define PMIC_EIS_VBAT_D10_H_eis_i_accu_state_d10_START (4)
#define PMIC_EIS_VBAT_D10_H_eis_i_accu_state_d10_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d11_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D11_L_UNION;
#endif
#define PMIC_EIS_VBAT_D11_L_eis_vbat_d11_data_l_START (0)
#define PMIC_EIS_VBAT_D11_L_eis_vbat_d11_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d11_data_h : 4;
        unsigned char eis_i_accu_state_d11 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D11_H_UNION;
#endif
#define PMIC_EIS_VBAT_D11_H_eis_vbat_d11_data_h_START (0)
#define PMIC_EIS_VBAT_D11_H_eis_vbat_d11_data_h_END (3)
#define PMIC_EIS_VBAT_D11_H_eis_i_accu_state_d11_START (4)
#define PMIC_EIS_VBAT_D11_H_eis_i_accu_state_d11_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d12_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D12_L_UNION;
#endif
#define PMIC_EIS_VBAT_D12_L_eis_vbat_d12_data_l_START (0)
#define PMIC_EIS_VBAT_D12_L_eis_vbat_d12_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d12_data_h : 4;
        unsigned char eis_i_accu_state_d12 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D12_H_UNION;
#endif
#define PMIC_EIS_VBAT_D12_H_eis_vbat_d12_data_h_START (0)
#define PMIC_EIS_VBAT_D12_H_eis_vbat_d12_data_h_END (3)
#define PMIC_EIS_VBAT_D12_H_eis_i_accu_state_d12_START (4)
#define PMIC_EIS_VBAT_D12_H_eis_i_accu_state_d12_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d13_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D13_L_UNION;
#endif
#define PMIC_EIS_VBAT_D13_L_eis_vbat_d13_data_l_START (0)
#define PMIC_EIS_VBAT_D13_L_eis_vbat_d13_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d13_data_h : 4;
        unsigned char eis_i_accu_state_d13 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D13_H_UNION;
#endif
#define PMIC_EIS_VBAT_D13_H_eis_vbat_d13_data_h_START (0)
#define PMIC_EIS_VBAT_D13_H_eis_vbat_d13_data_h_END (3)
#define PMIC_EIS_VBAT_D13_H_eis_i_accu_state_d13_START (4)
#define PMIC_EIS_VBAT_D13_H_eis_i_accu_state_d13_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d14_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D14_L_UNION;
#endif
#define PMIC_EIS_VBAT_D14_L_eis_vbat_d14_data_l_START (0)
#define PMIC_EIS_VBAT_D14_L_eis_vbat_d14_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d14_data_h : 4;
        unsigned char eis_i_accu_state_d14 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D14_H_UNION;
#endif
#define PMIC_EIS_VBAT_D14_H_eis_vbat_d14_data_h_START (0)
#define PMIC_EIS_VBAT_D14_H_eis_vbat_d14_data_h_END (3)
#define PMIC_EIS_VBAT_D14_H_eis_i_accu_state_d14_START (4)
#define PMIC_EIS_VBAT_D14_H_eis_i_accu_state_d14_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d15_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D15_L_UNION;
#endif
#define PMIC_EIS_VBAT_D15_L_eis_vbat_d15_data_l_START (0)
#define PMIC_EIS_VBAT_D15_L_eis_vbat_d15_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d15_data_h : 4;
        unsigned char eis_i_accu_state_d15 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D15_H_UNION;
#endif
#define PMIC_EIS_VBAT_D15_H_eis_vbat_d15_data_h_START (0)
#define PMIC_EIS_VBAT_D15_H_eis_vbat_d15_data_h_END (3)
#define PMIC_EIS_VBAT_D15_H_eis_i_accu_state_d15_START (4)
#define PMIC_EIS_VBAT_D15_H_eis_i_accu_state_d15_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d16_data_l : 8;
    } reg;
} PMIC_EIS_VBAT_D16_L_UNION;
#endif
#define PMIC_EIS_VBAT_D16_L_eis_vbat_d16_data_l_START (0)
#define PMIC_EIS_VBAT_D16_L_eis_vbat_d16_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_d16_data_h : 4;
        unsigned char eis_i_accu_state_d16 : 1;
        unsigned char reserved : 3;
    } reg;
} PMIC_EIS_VBAT_D16_H_UNION;
#endif
#define PMIC_EIS_VBAT_D16_H_eis_vbat_d16_data_h_START (0)
#define PMIC_EIS_VBAT_D16_H_eis_vbat_d16_data_h_END (3)
#define PMIC_EIS_VBAT_D16_H_eis_i_accu_state_d16_START (4)
#define PMIC_EIS_VBAT_D16_H_eis_i_accu_state_d16_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_ibat_p_d4_data_l : 8;
    } reg;
} PMIC_EIS_IBAT_P_D4_L_UNION;
#endif
#define PMIC_EIS_IBAT_P_D4_L_eis_ibat_p_d4_data_l_START (0)
#define PMIC_EIS_IBAT_P_D4_L_eis_ibat_p_d4_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_ibat_p_d4_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EIS_IBAT_P_D4_H_UNION;
#endif
#define PMIC_EIS_IBAT_P_D4_H_eis_ibat_p_d4_data_h_START (0)
#define PMIC_EIS_IBAT_P_D4_H_eis_ibat_p_d4_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_ibat_p_d8_data_l : 8;
    } reg;
} PMIC_EIS_IBAT_P_D8_L_UNION;
#endif
#define PMIC_EIS_IBAT_P_D8_L_eis_ibat_p_d8_data_l_START (0)
#define PMIC_EIS_IBAT_P_D8_L_eis_ibat_p_d8_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_ibat_p_d8_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EIS_IBAT_P_D8_H_UNION;
#endif
#define PMIC_EIS_IBAT_P_D8_H_eis_ibat_p_d8_data_h_START (0)
#define PMIC_EIS_IBAT_P_D8_H_eis_ibat_p_d8_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_ibat_p_d12_data_l : 8;
    } reg;
} PMIC_EIS_IBAT_P_D12_L_UNION;
#endif
#define PMIC_EIS_IBAT_P_D12_L_eis_ibat_p_d12_data_l_START (0)
#define PMIC_EIS_IBAT_P_D12_L_eis_ibat_p_d12_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_ibat_p_d12_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EIS_IBAT_P_D12_H_UNION;
#endif
#define PMIC_EIS_IBAT_P_D12_H_eis_ibat_p_d12_data_h_START (0)
#define PMIC_EIS_IBAT_P_D12_H_eis_ibat_p_d12_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_ibat_p_d16_data_l : 8;
    } reg;
} PMIC_EIS_IBAT_P_D16_L_UNION;
#endif
#define PMIC_EIS_IBAT_P_D16_L_eis_ibat_p_d16_data_l_START (0)
#define PMIC_EIS_IBAT_P_D16_L_eis_ibat_p_d16_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_ibat_p_d16_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EIS_IBAT_P_D16_H_UNION;
#endif
#define PMIC_EIS_IBAT_P_D16_H_eis_ibat_p_d16_data_h_START (0)
#define PMIC_EIS_IBAT_P_D16_H_eis_ibat_p_d16_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_det_t_num_l : 8;
    } reg;
} PMIC_EIS_DET_T_NUM_L_UNION;
#endif
#define PMIC_EIS_DET_T_NUM_L_eis_det_t_num_l_START (0)
#define PMIC_EIS_DET_T_NUM_L_eis_det_t_num_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_det_t_num_h : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_EIS_DET_T_NUM_H_UNION;
#endif
#define PMIC_EIS_DET_T_NUM_H_eis_det_t_num_h_START (0)
#define PMIC_EIS_DET_T_NUM_H_eis_det_t_num_h_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_vbat_sample_num : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EIS_VBAT_DET_NUM_UNION;
#endif
#define PMIC_EIS_VBAT_DET_NUM_eis_vbat_sample_num_START (0)
#define PMIC_EIS_VBAT_DET_NUM_eis_vbat_sample_num_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_flag : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_EIS_DET_FLAG_UNION;
#endif
#define PMIC_EIS_DET_FLAG_eis_flag_START (0)
#define PMIC_EIS_DET_FLAG_eis_flag_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_initial_end_flag : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_EIS_INIT_END_FLAG_UNION;
#endif
#define PMIC_EIS_INIT_END_FLAG_eis_initial_end_flag_START (0)
#define PMIC_EIS_INIT_END_FLAG_eis_initial_end_flag_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char reserved : 1;
        unsigned char eis_adc_reserve : 7;
    } reg;
} PMIC_EISADC_RESERVE_UNION;
#endif
#define PMIC_EISADC_RESERVE_eis_adc_reserve_START (1)
#define PMIC_EISADC_RESERVE_eis_adc_reserve_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_adc_valid : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_EISADC_CONV_STATUS_UNION;
#endif
#define PMIC_EISADC_CONV_STATUS_eis_adc_valid_START (0)
#define PMIC_EISADC_CONV_STATUS_eis_adc_valid_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_adc_data_l : 8;
    } reg;
} PMIC_EISADC_DATA_L_UNION;
#endif
#define PMIC_EISADC_DATA_L_eis_adc_data_l_START (0)
#define PMIC_EISADC_DATA_L_eis_adc_data_l_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_adc_data_h : 4;
        unsigned char reserved : 4;
    } reg;
} PMIC_EISADC_DATA_H_UNION;
#endif
#define PMIC_EISADC_DATA_H_eis_adc_data_h_START (0)
#define PMIC_EISADC_DATA_H_eis_adc_data_h_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_adc_buffer_sel : 1;
        unsigned char eis_adc_config : 7;
    } reg;
} PMIC_EISADC_CONV_UNION;
#endif
#define PMIC_EISADC_CONV_eis_adc_buffer_sel_START (0)
#define PMIC_EISADC_CONV_eis_adc_buffer_sel_END (0)
#define PMIC_EISADC_CONV_eis_adc_config_START (1)
#define PMIC_EISADC_CONV_eis_adc_config_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_adc_ibias_sel : 8;
    } reg;
} PMIC_EIS_ADC_CURRENT_UNION;
#endif
#define PMIC_EIS_ADC_CURRENT_eis_adc_ibias_sel_START (0)
#define PMIC_EIS_ADC_CURRENT_eis_adc_ibias_sel_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_soft_rst_cfg : 8;
    } reg;
} PMIC_EIS_SOFT_RST_UNION;
#endif
#define PMIC_EIS_SOFT_RST_eis_soft_rst_cfg_START (0)
#define PMIC_EIS_SOFT_RST_eis_soft_rst_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_clk_gt_bypass : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_EIS_CLK_GT_CTRL_UNION;
#endif
#define PMIC_EIS_CLK_GT_CTRL_eis_clk_gt_bypass_START (0)
#define PMIC_EIS_CLK_GT_CTRL_eis_clk_gt_bypass_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_debug_en : 8;
    } reg;
} PMIC_EIS_DEBUG_EN_UNION;
#endif
#define PMIC_EIS_DEBUG_EN_eis_debug_en_START (0)
#define PMIC_EIS_DEBUG_EN_eis_debug_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dbg_eis_ana_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_DEBUG_EIS_EN_UNION;
#endif
#define PMIC_DEBUG_EIS_EN_dbg_eis_ana_en_START (0)
#define PMIC_DEBUG_EIS_EN_dbg_eis_ana_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dbg_eis_dischg_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_DEBUG_EIS_DISCHG_EN_UNION;
#endif
#define PMIC_DEBUG_EIS_DISCHG_EN_dbg_eis_dischg_en_START (0)
#define PMIC_DEBUG_EIS_DISCHG_EN_dbg_eis_dischg_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dbg_eis_chanl_sel : 3;
        unsigned char reserved : 5;
    } reg;
} PMIC_DEBUG_EIS_CHANL_UNION;
#endif
#define PMIC_DEBUG_EIS_CHANL_dbg_eis_chanl_sel_START (0)
#define PMIC_DEBUG_EIS_CHANL_dbg_eis_chanl_sel_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dbg_eisadc_chanl_sel : 5;
        unsigned char reserved_0 : 2;
        unsigned char reserved_1 : 1;
    } reg;
} PMIC_DEBUG_EISADC_CTRL_UNION;
#endif
#define PMIC_DEBUG_EISADC_CTRL_dbg_eisadc_chanl_sel_START (0)
#define PMIC_DEBUG_EISADC_CTRL_dbg_eisadc_chanl_sel_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char dbg_eis_adc_start : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_DEBUG_EISADC_START_UNION;
#endif
#define PMIC_DEBUG_EISADC_START_dbg_eis_adc_start_START (0)
#define PMIC_DEBUG_EISADC_START_dbg_eis_adc_start_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char eis_adc_chopper_en : 1;
        unsigned char reserved : 7;
    } reg;
} PMIC_EISADC_MODE_CFG_UNION;
#endif
#define PMIC_EISADC_MODE_CFG_eis_adc_chopper_en_START (0)
#define PMIC_EISADC_MODE_CFG_eis_adc_chopper_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_test0 : 8;
    } reg;
} PMIC_SPMI_TEST0_UNION;
#endif
#define PMIC_SPMI_TEST0_spmi_test0_START (0)
#define PMIC_SPMI_TEST0_spmi_test0_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char spmi_test1 : 8;
    } reg;
} PMIC_SPMI_TEST1_UNION;
#endif
#define PMIC_SPMI_TEST1_spmi_test1_START (0)
#define PMIC_SPMI_TEST1_spmi_test1_END (7)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
