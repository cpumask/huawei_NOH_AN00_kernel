
#ifndef SN65DSIX6_BRIDGE_H
#define SN65DSIX6_BRIDGE_H

#define  SN65DSIX6_I2C_ADDR		0x2C
#define  SN_DEVICE_REV_REG			0x08
#define  SN_DPPLL_SRC_REG			0x0A
#define  DPPLL_CLK_SRC_DSICLK			BIT(0)
#define  REFCLK_FREQ_MASK			GENMASK(3, 1)
#define  REFCLK_FREQ(x)				((x) << 1)
#define  DPPLL_SRC_DP_PLL_LOCK			BIT(7)
#define  SN_PLL_ENABLE_REG			0x0D
#define  SN_DSI_LANES_REG			0x10
#define  CHA_DSI_LANES_MASK			GENMASK(4, 3)
#define  CHA_DSI_LANES(x)			((x) << 3)
#define  SN_DSIA_CLK_FREQ_REG			0x12
#define  SN_CHA_ACTIVE_LINE_LENGTH_LOW_REG	0x20
#define  SN_CHA_VERTICAL_DISPLAY_SIZE_LOW_REG	0x24
#define  SN_CHA_HSYNC_PULSE_WIDTH_LOW_REG	0x2C
#define  SN_CHA_HSYNC_PULSE_WIDTH_HIGH_REG	0x2D
#define  CHA_HSYNC_POLARITY			BIT(7)
#define  SN_CHA_VSYNC_PULSE_WIDTH_LOW_REG	0x30
#define  SN_CHA_VSYNC_PULSE_WIDTH_HIGH_REG	0x31
#define  CHA_VSYNC_POLARITY			BIT(7)
#define  SN_CHA_HORIZONTAL_BACK_PORCH_REG	0x34
#define  SN_CHA_VERTICAL_BACK_PORCH_REG		0x36
#define  SN_CHA_HORIZONTAL_FRONT_PORCH_REG	0x38
#define  SN_CHA_VERTICAL_FRONT_PORCH_REG		0x3A
#define  SN_ENH_FRAME_REG			0x5A
#define  VSTREAM_ENABLE				BIT(3)
#define  SN_DATA_FORMAT_REG			0x5B
#define  SN_HPD_DISABLE_REG			0x5C
#define  HPD_DISABLE				BIT(0)
#define  SN_AUX_WDATA_REG(x)			(0x64 + (x))
#define  SN_AUX_ADDR_19_16_REG			0x74
#define  SN_AUX_ADDR_15_8_REG			0x75
#define  SN_AUX_ADDR_7_0_REG			0x76
#define  SN_AUX_LENGTH_REG			0x77
#define  SN_AUX_CMD_REG				0x78
#define  AUX_CMD_SEND				BIT(0)
#define  AUX_CMD_REQ(x)				((x) << 4)
#define  SN_AUX_RDATA_REG(x)			(0x79 + (x))
#define  SN_SSC_CONFIG_REG			0x93
#define  DP_NUM_LANES_MASK			GENMASK(5, 4)
#define  DP_NUM_LANES(x)			((x) << 4)
#define  SN_DATARATE_CONFIG_REG			0x94
#define  DP_DATARATE_MASK			GENMASK(7, 5)
#define  DP_DATARATE(x)				((x) << 5)
#define  SN_ML_TX_MODE_REG			0x96
#define  ML_TX_MAIN_LINK_OFF			0
#define  ML_TX_NORMAL_MODE			BIT(0)
#define  SN_AUX_CMD_STATUS_REG			0xF4
#define  AUX_IRQ_STATUS_AUX_RPLY_TOUT		BIT(3)
#define  AUX_IRQ_STATUS_AUX_SHORT		BIT(5)
#define  AUX_IRQ_STATUS_NAT_I2C_FAIL		BIT(6)

/* Matches DP_AUX_MAX_PAYLOAD_BYTES (for now) */
#define SN_AUX_MAX_PAYLOAD_BYTES  16

static const struct regmap_config sn65dsix6_regmap = {
	.reg_bits = 8,
	.val_bits = 8,
	.reg_stride = 1,
};

int is_sn65dsix6_onboard(struct mipi2edp *pdata);
void sn65dsi86_pre_enable(struct mipi2edp *pdata);
int sn65dsi86_enable(struct mipi2edp *pdata);
void sn65dsi86_disable(struct mipi2edp *pdata);
#endif
