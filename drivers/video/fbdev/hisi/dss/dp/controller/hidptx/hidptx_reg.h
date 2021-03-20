/* Copyright (c) 2020, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __HIDPTX_REG_H__
#define __HIDPTX_REG_H__

#define DPTX_CFG_AUX_STATUS_ACK						0x0

#define DPTX_CFG_AUX_STATUS_AUX_NACK				0x10

#define DPTX_CFG_AUX_STATUS_AUX_DEFER				0x20

#define DPTX_DSI_COLOR_BAR							0x28

#define DPTX_MST_CTRL								0x30
#define DPTX_MST_VCP_TABLE_REG_N(n)	(0x10 + (n) * 4)
#define DPTX_CFG_SOFT_ACT_TRG						BIT(16)
#define DPTX_CFG_STREAM_ENABLE0						BIT(17)
#define DPTX_CFG_STREAM_ENABLE1						BIT(18)

#define DPTX_CFG_AUX_STATUS_I2C_NACK				0x40

#define DPTX_AUX_CMD_ADDR							0x50
#define DPTX_AUX_CMD_REQ_LEN_SHIFT					0
#define DPTX_AUX_CMD_I2C_ADDR_ONLY					BIT(4)
#define DPTX_AUX_CMD_ADDR_SHIFT						8
#define DPTX_AUX_CMD_TYPE_SHIFT						28
#define DPTX_AUX_CMD_TYPE_WRITE						0x0
#define DPTX_AUX_CMD_TYPE_READ						0x1
#define DPTX_AUX_CMD_TYPE_MOT						0x4
#define DPTX_AUX_CMD_TYPE_NATIVE					0x8


#define DPTX_AUX_WR_DATA0							0x54
#define DPTX_AUX_WR_DATA1							0x58
#define DPTX_AUX_WR_DATA2							0x5c
#define DPTX_AUX_WR_DATA3							0x60

#define DPTX_AUX_RD_DATA0							0x64
#define DPTX_AUX_RD_DATA1							0x68
#define DPTX_AUX_RD_DATA2							0x6c
#define DPTX_AUX_RD_DATA3							0x70

#define DPTX_AUX_REQ								0x74
#define DPTX_CFG_AUX_REQ							BIT(0)

#define DPTX_AUX_STATUS								0x78
#define DPTX_CFG_AUX_STATUS_MASK					GENMASK(11, 4)
#define DPTX_CFG_AUX_STATUS_SHIFT					4
#define DPTX_CFG_AUX_REPLY_ERR_DETECTED_MASK		GENMASK(3, 1)
#define DPTX_CFG_AUX_REPLY_ERR_DETECTED_SHIFT		1
#define DPTX_CFG_AUX_TIMEOUT						BIT(0)
#define DPTX_VIDEO_HACTIVE_SHIFT					16
#define DPTX_VIDEO_HBLANK_SHIFT						0
#define DPTX_CFG_AUX_READY_DATA_BYTE_MASK			GENMASK(16, 12)
#define DPTX_CFG_AUX_READY_DATA_BYTE_SHIFT			12

#define DPTX_CFG_AUX_STATUS_I2C_DEFER				0x80
#define DPTX_VIDEO_CONFIG0_STREAM(n)				(0x104 + 0x200 * (n))
#define DPTX_VIDEO_VACTIVE_SHIFT					16
#define DPTX_VIDEO_VBLANK_SHIFT						0

#define DPTX_HPD_STATUS								0x98
#define CFG_SINK_HPD_STATUS							GENMASK(7, 0)

#define DPTX_PHYIF_CTRL0							0xa0
#define DPTX_LANE_DATA_EN_SHIFT						8
#define DPTX_LANE_DATA_EN_MASK						GENMASK(11, 8)
#define DPTX_PATTERN_SEL_SHIFT						4
#define DPTX_PATTERN_SEL_MASK						GENMASK(7, 4)
#define DPTX_LANE_NUN_SHIFT							1
#define DPTX_LANE_NUN_MASK							GENMASK(2, 1)
#define DPTX_PHYIF_CTRL_TPS_SYM_ERM					10
#define DPTX_PHYIF_CTRL_TPS_PRBS7					5
#define DPTX_PHYIF_CTRL_TPS_CUSTOM80				6
#define DPTX_PHYIF_CTRL_TPS_CP2520_1				7
#define DPTX_PHYIF_CTRL_TPS_CP2520_2				8

#define DPTX_PHYIF_CTRL1							0xa4
#define CFG_PHY_LANES(lane_num)						GENMASK((lane_num - 1), 0)

#define DPTX_PHYIF_CTRL2							0xa8
#define DPTX_LANE_RATE_MASK						GENMASK(2, 0)

#define DPTX_PHYIF_CTRL3							0xac
#define LANE_POWER_MODE_P0							0
#define LANE_POWER_MODE_P1							1
#define LANE_POWER_MODE_P2							2
#define LANE_POWER_MODE_P3							3

#define DPTX_PHYIF_CUSTOM80B_0						0xb0

#define DPTX_PHYIF_CUSTOM80B_1						0xb4

#define DPTX_PHYIF_CUSTOM80B_2						0xb8

#define DPTX_PHYIF_STATUS0							0xc0
#define DPTX_LANE0_PHY_STATUS_SHIFT					0
#define DPTX_LANE0_PHY_STATUS_MASK					GENMASK(3, 0)
#define DPTX_LANE1_PHY_STATUS_SHIFT					4
#define DPTX_LANE1_PHY_STATUS_MASK					GENMASK(7, 4)
#define DPTX_LANE2_PHY_STATUS_SHIFT					8
#define DPTX_LANE2_PHY_STATUS_MASK					GENMASK(11, 8)
#define DPTX_LANE3_PHY_STATUS_SHIFT					12
#define DPTX_LANE3_PHY_STATUS_MASK					GENMASK(15, 12)
#define DPTX_PHYIF_CTRL_BUSY(lane)					BIT(lane)

#define DPTX_VIDEO_CTRL_STREAM(n)					(0x100 + 0x200 * (n))
#define DPTX_VIDEO_CTRL_VMAP_BPC_MASK				GENMASK(5, 2)
#define DPTX_VIDEO_CTRL_VMAP_BPC_SHIFT				2
#define DPTX_POL_CTRL_SYNC_POL_MASK					GENMASK(8, 7)
#define DPTX_POL_CTRL_H_SYNC_POL_SHIFT				7
#define DPTX_POL_CTRL_V_SYNC_POL_SHIFT				8
#define DPTX_VIDEO_CTRL_STREAM_EN					BIT(0)
#define DPTX_STREAM_FRAME_MODE						BIT(6)
#define DPTX_CFG_STREAM_DSC_ENABLE					BIT(9)
#define DPTX_CFG_STREAM_DSC_CRC_ENABLE				BIT(10)

#define DPTX_VIDEO_CONFIG1_STREAM(n)				(0x108 + 0x200 * (n))
#define DPTX_VIDEO_HFRONT_PORCH						16
#define DPTX_VIDEO_HSYNC_WIDTH						0

#define DPTX_VIDEO_CONFIG2_STREAM(n)				(0x10c + 0x200 * (n))
#define DPTX_VIDEO_VFRONT_PORCH						16
#define DPTX_VIDEO_VSYNC_WIDTH						0
#define DPTX_VIDEO_CONFIG3_STREAM(n)				(0x110 + 0x200 * (n))

#define DPTX_VIDEO_PACKET_STREAM(n)					(0x114 + 0x200 * (n))
#define DPTX_VIDEO_PACKET_TU_MASK					GENMASK(5, 0)
#define DPTX_VIDEO_PACKET_TU_SHIFT					0
#define DPTX_VIDEO_PACKET_TU_FRAC_MASK_SST			GENMASK(9, 6)
#define DPTX_VIDEO_PACKET_TU_FRAC_SHIFT_SST			6
#define DPTX_VIDEO_PACKET_INIT_THRESHOLD_MASK		GENMASK(19, 10)
#define DPTX_VIDEO_PACKET_INIT_THRESHOLD_SHIFT		10
#define DPTX_VIDEO_PACKET_DEFAULT_THRESHOLD			0xf

#define DPTX_VIDEO_MSA0_STREAM(n)					(0x118 + 0x200 * (n))
#define DPTX_VIDEO_MSA0_HSTART_SHIFT				0
#define DPTX_VIDEO_MSA0_VSTART_SHIFT				16

#define DPTX_VIDEO_MSA1_STREAM(n)					(0x11c + 0x200 * (n))
#define DPTX_VIDEO_VMSA1_COL_MASK					GENMASK(4, 1)
#define DPTX_VIDEO_VMSA1_COL_SHIFT					1
#define DPTX_VIDEO_VMSA1_BPC_MASK					GENMASK(7, 5)
#define DPTX_VIDEO_VMSA1_BPC_SHIFT					5

#define DPTX_VIDEO_MSA2_STREAM(n)					(0x120 + 0x200 * (n))
#define DPTX_VIDEO_VMSA2_PIX_ENC_MASK				GENMASK(7, 6)
#define DPTX_VIDEO_VMSA2_PIX_ENC_YCBCR420_SHIFT		6 /* ignore MSA */
#define DPTX_VIDEO_VMSA2_PIX_ENC_SHIFT				7

#define DPTX_VIDEO_HORIZONTAL_SIZE(n)				(0x124 + 0x200 * (n))
#define DPTX_VIDEO_HORIZONTAL_SIZE_HTOTAL_OFFSET 16

#define DPTX_SDP_CTRL(n)                            (0x12c + 0x200 * (n))
#define DPTX_CFG_STREAM_AUDIO_SDP_TX_MODE           BIT(0)
#define DPTX_CFG_STREAM_PPS_SDP_TX_MODE             BIT(1)
#define DPTX_CFG_STREAM_INFORMFRAME0_SDP_TX_MODE    BIT(2)
#define DPTX_CFG_STREAM_PPS_SDP_ENABLE              BIT(10)
#define DPTX_TIMESTAMP_SDP_EN                       BIT(11)
#define DPTX_STREAM_SDP_EN                          BIT(12)
#define DPTX_SDP_SPLIT_EN                           BIT(21)
#define DPTX_INFOFRAME_SDP_TX_MODE_SHIFT	2
#define DPTX_INFOFRAME_SDP_ENABLE_SHIFT	13
#define DPTX_INFOFRAME_SDP_TX_MODE_MASK	GENMASK(9, 2)
#define DPTX_INFOFRAME_SDP_ENABLE_MASK	GENMASK(20, 13)

#define DPTX_STREAM_AUDIO_INFOFRAME_SDP(m, n)		(0x134 + 0x24 * (m) + 0x200 * (n))

#define DPTX_STREAM_DSC_CRC_CTRL(n)					(0x25c + 0x200 * (n))
#define CFG_STREAM_CHUNK_SIZE						GENMASK(15, 0)
#define CFG_STREAM_TIMESTAMP_UNIT					GENMASK(23, 19)


#define DPTX_DP_COLOR_BAR_STREAM(n)					(0x260 + 0x200 * (n))
#define DPTX_COLOR_BAR_STREAM_EN					BIT(0)
#define DPTX_COLOR_BAR_STREAM_DYNAMIC_EN			GENMASK(8, 0)

#define DPTX_DSC_PPS(stream, i)				((0x500 + (stream - 1) * 0x80) + (i * 0x4))
#define DPTX_SST_MODE						1
#define DPTX_BITS_PER_PIXEL					8 /* For Interop testing only RGB 8bpp 8bpc */

/* PPS SDPs */
#define DPTX_DSC_VER_MAJ_SHIFT 4
#define DPTX_DSC_BUF_DEPTH_SHIFT 24
#define DPTX_DSC_BLOCK_PRED_SHIFT 5
#define DPTX_DSC_BPP_HIGH_MASK GENMASK(9, 8)
#define DPTX_DSC_BPP_LOW_MASK GENMASK(7, 0)
#define DPTX_DSC_BPP_LOW_MASK1 GENMASK(15, 8)
#define DPTX_DSC_BPC_SHIFT 28

#define DPTX_RST_CTRL								0x700
#define DPTX_SST0_RST_N								BIT(0)
#define DPTX_SST1_RST_N								BIT(1)
#define DPTX_MST_RST_N								BIT(2)
#define DPTX_HDCP_RST_N								BIT(3)
#define DPTX_AUX_RST_N								BIT(4)
#define DPTX_PHY_RST_N								BIT(5)
#define DPTX_GOBAL_RST_N							BIT(6)
#define DPTX_GOBAL_RST_N_MASK						GENMASK(6, 0)
#define DPTX_CFG_GOBAL_RST_N						BIT(6)
#define DPTX_RST_CTRL_ALL				(DPTX_SST0_RST_N | \
										 DPTX_SST1_RST_N | \
										 DPTX_MST_RST_N | \
										 DPTX_HDCP_RST_N | \
										 DPTX_AUX_RST_N | \
										 DPTX_PHY_RST_N)

#define DPTX_CLK_CTRL								0x704
#define DPTX_CFG_MST_SST1_CLK_EN					BIT(1)

#define DPTX_GCTL0									0x708
#define DPTX_CFG_MST_ENABLE							BIT(0)
#define DPTX_CFG_PHY_FEC_ENABLE						BIT(3)

#define DPTX_INTR_ENABLE							0x720

#define DPTX_SDP_INTR_ENABLE						0x730

#define DPTX_INTR_ORIGINAL_STATUS					0x728
#define DPTX_IRQ_STREAM0_FRAME						BIT(0)
#define DPTX_IRQ_HPD_PLUG							BIT(2)
#define DPTX_IRQ_HPD_UNPLUG							BIT(3)
#define DPTX_IRQ_HPD_SHORT							BIT(4)
#define DPTX_IRQ_AUX_REPLY							BIT(6)
#define DPTX_IRQ_STREAM0_SDP						BIT(9)
#define DPTX_IRQ_VIDEO_FIFO_FULL					BIT(10)
#define DPTX_IRQ_AUDIO_FIFO_FULL					BIT(11)
#define DPTX_IRQ_STREAM1_SDP						BIT(13)
#define DPTX_IRQ_SDP_INTR				(DPTX_IRQ_STREAM0_SDP | \
										 DPTX_IRQ_STREAM1_SDP)
#define DPTX_IRQ_ALL_INTR				(DPTX_IRQ_STREAM0_FRAME | \
										 DPTX_IRQ_HPD_PLUG | \
										 DPTX_IRQ_HPD_UNPLUG | \
										 DPTX_IRQ_HPD_SHORT | \
										 DPTX_IRQ_AUX_REPLY | \
										 DPTX_IRQ_STREAM0_SDP | \
										 DPTX_IRQ_VIDEO_FIFO_FULL | \
										 DPTX_IRQ_AUDIO_FIFO_FULL)

/*
 * Default value set
 */
#define DPTX_MP_DUAL_PIXEL		1
/* The max rate and lanes supported by the core */
#define DPTX_MAX_LINK_RATE		DPTX_PHYIF_CTRL_RATE_HBR2
#define DPTX_MAX_LINK_LANES	(4)
/* The default rate and lanes to use for link training */
#define DPTX_DEFAULT_LINK_RATE DPTX_MAX_LINK_RATE
#define DPTX_DEFAULT_LINK_LANES DPTX_MAX_LINK_LANES

/*
 * Audio Registers
 */
#define DPTX_AUDIO_CTRL_STREAM(n)				(0x128 + 0x200 * (n))
#define DPTX_AUDIO_VERSION_NUM_MASK				GENMASK(24, 19)
#define DPTX_AUDIO_VERSION_NUM_SHIFT			19
#define DPTX_AUDIO_HB0_MASK						GENMASK(18, 11)
#define DPTX_AUDIO_HB0_SHIFT					11
#define DPTX_AUDIO_MUTE_EN						BIT(10)
#define DPTX_AUDIO_CH_NUM_MASK					GENMASK(9, 7)
#define DPTX_AUDIO_CH_NUM_SHIFT					7
#define DPTX_AUDIO_HBR_MODE_EN					BIT(6)
#define DPTX_AUDIO_DATA_WIDTH_SHIFT				5
#define DPTX_AUDIO_IN_ENABLE_MASK				GENMASK(4, 1)
#define DPTX_AUDIO_IN_ENABLE_SHIFT				1
#define DPTX_AUDIO_IN_EN						BIT(1)
#define DPTX_AUDIO_IF_SEL						BIT(0)

/*
 * bit[23:0]=0;
 * bit[31:24]=1;
 */
#define DPTX_AUDIO_TEST_CTRL0_STREAM(n)					(0x264 + 0x200 * (n))
#define DPTX_AUDIO_TEST_CTRL1_STREAM(n)					(0x268  + 0x200 * (n))
#define DPTX_COLOR_BAR_AUDIO_TEST_EN					BIT(9)
#define DPTX_COLOR_BAR_AUDIO_VUC_IND_MASK				GENMASK(12, 10)

/* Data Byte 5 contains Level Shift Information, a Down-mix Inhibit Flag, and LFE playback level information. */
#define DPTX_AUDIO_LSV_0DB		0x0
#define DPTX_AUDIO_LSV_1DB		0x1
#define DPTX_AUDIO_LSV_2DB		0x2
#define DPTX_AUDIO_LSV_3DB		0x3
#define DPTX_AUDIO_LSV_4DB		0x4
#define DPTX_AUDIO_LSV_5DB		0x5
#define DPTX_AUDIO_LSV_6DB		0x6
#define DPTX_AUDIO_LSV_7DB		0x7
#define DPTX_AUDIO_LSV_8DB		0x8
#define DPTX_AUDIO_LSV_9DB		0x9
#define DPTX_AUDIO_LSV_10DB		0xA
#define DPTX_AUDIO_LSV_11DB		0xB
#define DPTX_AUDIO_LSV_12DB		0xC
#define DPTX_AUDIO_LSV_13DB		0xD
#define DPTX_AUDIO_LSV_14DB		0xE
#define DPTX_AUDIO_LSV_15DB		0xF
#define DPTX_AUDIO_LSV_SHIFT	3
#define DPTX_AUDIO_LSV_MASK		GENMASK(6, 3)

#define DPTX_AUDIO_DOWN_MIX_PERMITETED	0x0 /* Permitted or no information about any assertion of this */
#define DPTX_AUDIO_DOWN_MIX_PROHIBITED	0x1
#define DPTX_AUDIO_DOWN_MIX_SHIFT		7
/*
 * HDCP Registers
 */
#define DPTX_HDCP_API_INT_MSK		0xE10
#define DPTX_HDCP_REG_RMLCTL		0x3614
#define DPTX_HDCP_CFG				0x600
#define DPTX_HDCP_REG_RMLSTS		0x3618
#define DPTX_HDCP_REG_DPK_CRC		0x3630
#define DPTX_HDCP_REG_DPK0		0x3620
#define DPTX_HDCP_REG_DPK1		0x3624
#define DPTX_HDCP_REG_SEED		0x361C
#define DPTX_HDCP_INT_STS			0xE0C
#define DPTX_HDCP_INT_CLR			0xE08
#define DPTX_HDCP_OBS				0xE04

#define DPTX_HDCP_CFG_DPCD_PLUS_SHIFT			7
#define DPTX_HDCP_CFG_DPCD_PLUS_MASK			BIT(7)
#define DPTX_HDCP_KSV_ACCESS_INT			BIT(0)
#define DPTX_HDCP_AUX_RESP_TIMEOUT			BIT(3)
#define DPTX_HDCP_KSV_SHA1				BIT(5)
#define DPTX_HDCP_FAILED				BIT(6)
#define DPTX_HDCP_ENGAGED				BIT(7)
#define DPTX_HDCP22_GPIOINT				BIT(8)
#define DPTX_REVOC_LIST_MASK				GENMASK(31, 24)
#define DPTX_REVOC_SIZE_SHIFT				23
#define DPTX_REVOC_SIZE_MASK				GENMASK(23, 8)
#define DPTX_ODPK_DECRYPT_ENABLE_SHIFT			0
#define DPTX_ODPK_DECRYPT_ENABLE_MASK			BIT(0)
#define DPTX_IDPK_DATA_INDEX_SHIFT			0
#define DPTX_IDPK_DATA_INDEX_MASK			GENMASK(5, 0)
#define DPTX_IDPK_WR_OK_STS_SHIFT			6
#define DPTX_IDPK_WR_OK_STS_MASK			BIT(6)
#define DPTX_CFG_EN_HDCP13				BIT(2)
#define DPTX_CFG_CP_IRQ					BIT(6)
#define DPTX_CFG_EN_HDCP				BIT(1)

#define DPTX_HDCP22_BOOTED				BIT(23)
#define DPTX_HDCP22_STATE				(BIT(24)|BIT(25)|BIT(26))
#define DPTX_HDCP22_CAP_CHECK_COMPLETE	BIT(27)
#define DPTX_HDCP22_CAPABLE_SINK		BIT(28)
#define DPTX_HDCP22_AUTH_SUCCESS		BIT(29)
#define DPTX_HDCP22_AUTH_FAILED			BIT(30)
#define DPTX_HDCP22_RE_AUTH_REQ			BIT(31)

/* HDR Bitfields */
#define DPTX_EN_HDR_INFOFRAME_SDP BIT(3)
#define DPTX_FIXED_PRIORITY_ARBITRATION BIT(31)

/* LowPower Mode Registers */
#define LOW_POWER_LANE_RESET 0x42588
#define LOW_POWER_LANE_RESET_MASK GENMASK(3, 0)
#define LOW_POWER_LANE_RESET_SEL_MASK GENMASK(7, 4)
#define HIDPC_MEMORY 0x42200
#define HIDPC_MEMORY_SHUTDOWN_BIT BIT(2)
/* n = 0~3 */
#define HIDPC_DSC_MEMORY_CONFIG_NUM 4
#define HIDPC_DSC_MEMORY_N(n) (0x42204 + 0x4 * (n))
#define HIDPC_DSC_MEMORY_SHUTDOWN_BITS_N(n) ((n == 3) ? (BIT(2)) : (BIT(2) | BIT(18)))
#define HIDPC_HPI_CLK 0x42580
#define HIDPC_HPI_CLK_SHUTDOWN_MASK GENMASK(1, 0)
#define HIDPC_TRNG_CLK 0x42584
#define HIDPC_TRNG_CLK_SHUTDOWN_MASK GENMASK(1, 0)
#define HIDPC_SOFT_RESET_CTRL 0x204
#define HIDPC_PHY_SOFT_RESET_BIT BIT(1)

#endif /* HIDPTX_REG_H */
