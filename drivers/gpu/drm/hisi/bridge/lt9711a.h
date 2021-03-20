
#ifndef LT9711A_BRIDGE_H
#define LT9711A_BRIDGE_H

#define	LT9711A_I2C_ADDR		0x41
#define	CHIP_ID_REG_0			0x00
#define	CHIP_ID_REG_1			0x01
#define	CHIP_MODE_REG_0		0xFF
#define	CHIP_MODE_REG_1		0xFE
#define	DOWNLOAD_MODE		0x80
#define	MODE_CHANGING		0x01
#define	WORKING_MODE			0x81


static const struct regmap_config lt9711a_regmap = {
	.reg_bits = 8,
	.val_bits = 8,
	.reg_stride = 1,
};

int is_lt9711a_onboard(struct mipi2edp *pdata);
int lt9711a_get_edid(char *edid, unsigned int len);
int check_edid(char *edid, unsigned int len);

#endif
