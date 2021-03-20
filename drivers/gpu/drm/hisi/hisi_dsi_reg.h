/*
 * Copyright (c) 2016 Linaro Limited.
 * Copyright (c) 2014-2016 Hisilicon Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef HISI_DSI_REG_H
#define HISI_DSI_REG_H

#define MASK(x) (BIT(x) - 1)
#define DEFAULT_MAX_TX_ESC_CLK (10 * 1000000UL)
#define DEFAULT_MIPI_CLK_RATE  (192 * 100000L)
#define DEFAULT_PCLK_DSI_RATE  (120 * 1000000L)

/*
 * regs
 */
#define PWR_UP                  0x04 /* Core power-up */
#define RESET                   0
#define POWERUP                 BIT(0)
#define PHY_IF_CFG              0xA4 /* D-PHY interface configuration */
#define CLKMGR_CFG              0x08 /* the internal clock dividers */
#define PHY_RSTZ                0xA0 /* D-PHY reset control */
#define PHY_ENABLECLK           BIT(2)
#define PHY_UNRSTZ              BIT(1)
#define PHY_UNSHUTDOWNZ         BIT(0)
#define PHY_TST_CTRL0           0xB4 /* D-PHY test interface control 0 */
#define PHY_TST_CTRL1           0xB8 /* D-PHY test interface control 1 */
#define CLK_TLPX                0x10
#define CLK_THS_PREPARE         0x11
#define CLK_THS_ZERO            0x12
#define CLK_THS_TRAIL           0x13
#define CLK_TWAKEUP             0x14
#define DATA_TLPX(x)            (0x20 + ((x) << 4))
#define DATA_THS_PREPARE(x)     (0x21 + ((x) << 4))
#define DATA_THS_ZERO(x)        (0x22 + ((x) << 4))
#define DATA_THS_TRAIL(x)       (0x23 + ((x) << 4))
#define DATA_TTA_GO(x)          (0x24 + ((x) << 4))
#define DATA_TTA_GET(x)         (0x25 + ((x) << 4))
#define DATA_TWAKEUP(x)         (0x26 + ((x) << 4))
#define PHY_CFG_I               0x60
#define PHY_CFG_PLL_I           0x63
#define PHY_CFG_PLL_II          0x64
#define PHY_CFG_PLL_III         0x65
#define PHY_CFG_PLL_IV          0x66
#define PHY_CFG_PLL_V           0x67
#define DPI_COLOR_CODING        0x10 /* DPI color coding */
#define DPI_CFG_POL             0x14 /* DPI polarity configuration */
#define VID_HSA_TIME            0x48 /* Horizontal Sync Active time */
#define VID_HBP_TIME            0x4C /* Horizontal Back Porch time */
#define VID_HLINE_TIME          0x50 /* Line time */
#define VID_VSA_LINES           0x54 /* Vertical Sync Active period */
#define VID_VBP_LINES           0x58 /* Vertical Back Porch period */
#define VID_VFP_LINES           0x5C /* Vertical Front Porch period */
#define VID_VACTIVE_LINES       0x60 /* Vertical resolution */
#define VID_PKT_SIZE            0x3C /* Video packet size */
#define VID_MODE_CFG            0x38 /* Video mode configuration */
#define GEN_HDR             0x6c
#define GEN_HDATA(data)     (((data) & 0xffff) << 8)
#define GEN_HDATA_MASK      (0xffff << 8)
#define GEN_HTYPE(type)     (((type) & 0xff) << 0)
#define GEN_HTYPE_MASK      0xff
#define GEN_PLD_DATA        0x70
#define CMD_PKT_STATUS      0x74
#define GEN_CMD_EMPTY       BIT(0)
#define GEN_CMD_FULL        BIT(1)
#define GEN_PLD_W_EMPTY     BIT(2)
#define GEN_PLD_W_FULL      BIT(3)
#define GEN_PLD_R_EMPTY     BIT(4)
#define GEN_PLD_R_FULL      BIT(5)
#define GEN_RD_CMD_BUSY     BIT(6)
#define CMD_MODE_CFG        0x68
#define MAX_RD_PKT_SIZE_LP  BIT(24)
#define DCS_LW_TX_LP        BIT(19)
#define DCS_SR_0P_TX_LP     BIT(18)
#define DCS_SW_1P_TX_LP     BIT(17)
#define DCS_SW_0P_TX_LP     BIT(16)
#define GEN_LW_TX_LP        BIT(14)
#define GEN_SR_2P_TX_LP     BIT(13)
#define GEN_SR_1P_TX_LP     BIT(12)
#define GEN_SR_0P_TX_LP     BIT(11)
#define GEN_SW_2P_TX_LP     BIT(10)
#define GEN_SW_1P_TX_LP     BIT(9)
#define GEN_SW_0P_TX_LP     BIT(8)
#define EN_ACK_RQST         BIT(1)
#define EN_TEAR_FX          BIT(0)
#define CMD_MODE_ALL_LP (MAX_RD_PKT_SIZE_LP | \
				 DCS_LW_TX_LP | \
				 DCS_SR_0P_TX_LP | \
				 DCS_SW_1P_TX_LP | \
				 DCS_SW_0P_TX_LP | \
				 GEN_LW_TX_LP | \
				 GEN_SR_2P_TX_LP | \
				 GEN_SR_1P_TX_LP | \
				 GEN_SR_0P_TX_LP | \
				 GEN_SW_2P_TX_LP | \
				 GEN_SW_1P_TX_LP | \
				 GEN_SW_0P_TX_LP)
#define PHY_TMR_CFG             0x9C /* Data lanes timing configuration */
#define BTA_TO_CNT              0x8C /* Response timeout definition */
#define PHY_TMR_LPCLK_CFG       0x98 /* clock lane timing configuration */
#define CLK_DATA_TMR_CFG        0xCC
#define LPCLK_CTRL              0x94 /* Low-power in clock lane */
#define PHY_TXREQUESTCLKHS      BIT(0)
#define MODE_CFG                0x34 /* Video or Command mode selection */
#define PHY_STATUS              0xB0 /* D-PHY PPI status interface */

#define	PHY_STOP_WAIT_TIME      0x30
#define CMD_PKT_STATUS_TIMEOUT_US 20000

#endif /* HISI_DSI_REG_H */
