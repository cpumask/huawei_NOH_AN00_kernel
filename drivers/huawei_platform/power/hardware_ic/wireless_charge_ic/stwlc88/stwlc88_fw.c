/*
 * stwlc88_fw.c
 *
 * stwlc88 ftp, sram driver
 *
 * Copyright (c) 2020-2020 Huawei Technologies Co., Ltd.
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

#include "stwlc88.h"
#include "stwlc88_ftp.h"

#define HWLOG_TAG wireless_stwlc88_fw
HWLOG_REGIST();

static int g_st88_ftp_status;

static int stwlc88_fw_check_dc_power(void)
{
	int ret;
	u8 op_mode = 0;

	ret = stwlc88_get_mode(&op_mode);
	if (ret)
		return -WLC_ERR_I2C_R;

	hwlog_info("[check_dc_power]: op_mode=%02X\n", op_mode);
	if (op_mode != ST88_OP_MODE_SA)
		return -WLC_ERR_MISMATCH;

	return 0;
}

static int stwlc88_fw_reset_system(void)
{
	int ret;
	u8 wr_buff = ST88_RST_SYS;

	ret = stwlc88_hw_write_block(ST88_RST_ADDR, &wr_buff, BYTE_LEN);
	if (ret)
		hwlog_info("[reset_system] ignore i2c failure\n");

	msleep(100); /* delay for system reset */
	return 0;
}

static int stwlc88_fw_cmd_reset_system(void)
{
	int ret;

	ret = stwlc88_write_word_mask(ST88_SYS_CMD_ADDR, ST88_SYS_CMD_FW_RESET,
		ST88_SYS_CMD_FW_RESET_SHIFT, ST88_SYS_CMD_VAL);
	if (ret) {
		hwlog_err("cmd_reset_system: failed\n");
		return ret;
	}

	msleep(100); /* delay for system reset */
	return 0;
}

static int stwlc88_fw_unlock_ftp(void)
{
	int ret;

	ret = stwlc88_write_byte(ST88_FTP_WR_PWD_ADDR, ST88_FTP_WR_UNLOCK_PWD);
	if (ret) {
		hwlog_err("unlock_ftp: failed\n");
		return ret;
	}

	return 0;
}

static int stwlc88_fw_iload_disable(void)
{
	u8 reg_val = ST88_ILOAD_DIS_VAL;

	return stwlc88_hw_write_block(ST88_ILOAD_ADDR, &reg_val, BYTE_LEN);
}

static int stwlc88_fw_write_sys_cmd(u16 mask, u8 shift, u16 val)
{
	int i;
	int ret;
	u16 rd_data = 0;

	ret = stwlc88_write_word_mask(ST88_SYS_CMD_ADDR, mask, shift, val);
	if (ret) {
		hwlog_err("write_sys_cmd: wr failed\n");
		return ret;
	}

	/* 1s=10ms*100 timeout for cmd completion */
	for (i = 0; i < 100; i++) {
		usleep_range(9500, 10500); /* 10ms */
		ret = stwlc88_read_word_mask(ST88_SYS_CMD_ADDR,
			mask, shift, &rd_data);
		if (ret) {
			hwlog_err("write_sys_cmd: rd failed\n");
			return ret;
		}
		if (!rd_data) /* self-cleaned when completed */
			return 0;
	}

	hwlog_err("write_sys_cmd: timeout\n");
	return -WLC_ERR_I2C_W;
}

static int stwlc88_fw_hang_process_prepare(void)
{
	int ret;
	u8 reg_val = ST88_RST_SYS | ST88_RST_M0;
	u32 regs_val = ST88_FTP_STANDBY_WORD_VAL;

	/* system reset and hold m0 */
	(void)stwlc88_hw_write_block(ST88_RST_ADDR, &reg_val, BYTE_LEN);
	msleep(100);
	(void)stwlc88_fw_iload_disable();
	reg_val = ST88_FTP_CTRL_VAL1;
	ret = stwlc88_hw_write_block(ST88_FTP_CTRL_ADDR, &reg_val, BYTE_LEN);
	if (ret) {
		hwlog_err("erase_ftp: system reset and hold m0 failed\n");
		return -1;
	}
	hwlog_info("[fw_hang_process_prepare] system reset and hold m0 succ\n");

	/* unlock ftp */
	reg_val = ST88_FTP_TRIM_LOCK_VAL;
	ret = stwlc88_hw_write_block(ST88_FTP_TRIM_LOCK_NUM_ADDR, &reg_val, BYTE_LEN);
	reg_val = ST88_FTP_USER_LOCK_VAL;
	ret += stwlc88_hw_write_block(ST88_FTP_USER_LOCK_NUM_ADDR, &reg_val, BYTE_LEN);
	if (ret) {
		hwlog_err("erase_ftp: unlock ftp failed\n");
		return -1;
	}
	hwlog_info("[fw_hang_process_prepare] unlock ftp succ\n");

	/* ftp full erase */
	reg_val = ST88_FTP_VDDCP_CTRL_VAL;
	ret = stwlc88_hw_write_block(ST88_FTP_VDDCP_CTRL_ADDR,
		(u8 *)&reg_val, BYTE_LEN);
	ret += stwlc88_hw_write_block(ST88_FTP_STANDBY_WORD_ADDR,
		(u8 *)&regs_val, ST88_FTP_STANDBY_WORD_LEN);
	if (ret) {
		hwlog_err("erase_ftp: ftp full erase failed\n");
		return -1;
	}
	hwlog_info("[fw_hang_process_prepare] ftp full erase succ\n");

	return 0;
}

static int stwlc88_fw_hang_process_soft_program(void)
{
	int i, ret;
	u8 reg_val = 0;
	u32 regs_val = ST88_FTP_SOFT_PROGRAM_VAL1;

	ret = stwlc88_hw_write_block(ST88_FTP_PATCH_ADDR, (u8 *)&regs_val,
		ST88_FTP_SOFT_PROGRAM_LEN);
	if (ret) {
		hwlog_err("fw_hang_process_soft_program: failed\n");
		return -1;
	}

	for (i = 0; i < 100; i++) { /* 100: soft program cnt,total 100*10ms */
		usleep_range(9500, 10500); /* 10ms delay for soft program */
		ret = stwlc88_hw_read_block(ST88_FTP_STAT_ADDR, &reg_val, BYTE_LEN);
		if (!ret && ((reg_val & ST88_FTP_STAT_SOFT_PROGRAM) == 0))
			break;
	}
	if (i >= 100) { /* 100: soft program cnt,total 100*10ms */
		hwlog_err("fw_hang_process_soft_program: status failed\n");
		return -1;
	}

	regs_val = ST88_FTP_STANDBY_WORD_VAL;
	ret = stwlc88_hw_write_block(ST88_FTP_STANDBY_WORD_ADDR, (u8 *)&regs_val,
		ST88_FTP_STANDBY_WORD_LEN);
	regs_val = ST88_FTP_SOFT_PROGRAM_VAL2;
	ret += stwlc88_hw_write_block(ST88_FTP_PATCH_ADDR, (u8 *)&regs_val,
		ST88_FTP_SOFT_PROGRAM_LEN);
	if (ret) {
		hwlog_err("fw_hang_process_soft_program: fail\n");
		return -1;
	}
	for (i = 0; i < 100; i++) { /* 100: soft program cnt,total 100*10ms */
		usleep_range(9500, 10500); /* 10ms delay for soft program */
		ret = stwlc88_hw_read_block(ST88_FTP_STAT_ADDR, &reg_val, BYTE_LEN);
		if (!ret && ((reg_val & ST88_FTP_STAT_SOFT_PROGRAM) == 0))
			break;
	}
	if (i >= 100) { /* 100: soft program cnt,total 100*10ms */
		hwlog_err("fw_hang_process_soft_program: status fail\n");
		return -1;
	}
	hwlog_info("[fw_hang_process] soft program succ\n");

	return 0;
}

static int stwlc88_fw_hang_process(void)
{
	int ret;
	u16 chip_id = 0;

	ret = stwlc88_get_chip_id(&chip_id);
	if (ret) {
		hwlog_err("fw_hang_process: get chip_id failed\n");
		return ret;
	}
	if (chip_id == ST88_CHIP_ID)
		return 0;

	hwlog_info("[fw_hang_process] chip_id=0x%x, need hw ftp erase\n", chip_id);
	ret = stwlc88_fw_hang_process_prepare();
	if (ret)
		goto exit;

	ret = stwlc88_fw_hang_process_soft_program();
	if (ret)
		goto exit;

exit:
	(void)stwlc88_chip_reset();
	msleep(100); /* 100 ms, delay for chip reset */
	return ret;
}

static int stwlc88_check_ftp_match(void)
{
	int ret;
	u16 ftp_patch_id = 0;
	u16 cfg_id = 0;

	g_st88_ftp_status = 0;
	ret = stwlc88_get_ftp_patch_id(&ftp_patch_id);
	if (ret) {
		hwlog_err("is_ftp_match: get ftp_patch_id failed\n");
		return ret;
	}
	hwlog_info("[is_ftp_match] ftp_patch_id=0x%x\n", ftp_patch_id);
	if (ftp_patch_id == ST88_FTP_PATCH_ID)
		g_st88_ftp_status |= ST88_FTP_PATCH_LATEST;

	ret = stwlc88_get_cfg_id(&cfg_id);
	if (ret) {
		hwlog_err("is_ftp_match: get cfg_id failed\n");
		return ret;
	}

	hwlog_info("[is_ftp_match] cfg_id=0x%x\n", cfg_id);
	if (cfg_id == ST88_FTP_CFG_ID)
		g_st88_ftp_status |= ST88_FTP_CFG_LATEST;

	if (g_st88_ftp_status != ST88_FTP_LATEST)
		return -WLC_ERR_MISMATCH;

	return 0;
}

static int stwlc88_fw_program_ftp_pre_check(void)
{
	int ret;

	ret = stwlc88_fw_check_dc_power();
	if (ret) {
		hwlog_err("program_ftp_pre_check: not in DC power\n");
		return ret;
	}

	return 0;
}

static int stwlc88_fw_cmd_full_erase_ftp(void)
{
	int ret;

	ret = stwlc88_fw_write_sys_cmd(ST88_SYS_CMD_FTP_FULL_ERASE,
		ST88_SYS_CMD_FTP_FULL_ERASE_SHIFT, ST88_SYS_CMD_VAL);
	if (ret) {
		hwlog_err("cmd_full_erase_ftp: failed\n");
		return ret;
	}

	return 0;
}

static int stwlc88_fw_full_erase_ftp(void)
{
	int i, ret;
	u16 reg_val = 0;

	/* ftp password */
	ret = stwlc88_fw_unlock_ftp();
	/* ftp full erase command set */
	ret += stwlc88_fw_cmd_full_erase_ftp();
	if (ret) {
		hwlog_err("full_erase_ftp: write regs failed\n");
		return ret;
	}

	for (i = 0; i < 100; i++) { /* full erase timeout: 1000ms(100*10) */
		usleep_range(9500, 10500); /* step: 10ms */
		ret = stwlc88_read_word(ST88_SYS_CMD_ADDR, &reg_val);
		if (!ret && ((reg_val & ST88_SYS_CMD_FTP_FULL_ERASE) == 0))
			return 0;
	}

	hwlog_err("full_erase_ftp: erase timeout\n");
	return -WLC_ERR_I2C_W;
}

static int stwlc88_fw_program_ftp_prepare(void)
{
	int i;
	int ret;

	/* full-erase ftp twice at most if not successful */
	for (i = 0; i < 2; i++) {
		/* fw system reset */
		ret = stwlc88_fw_cmd_reset_system();
		if (ret)
			continue;
		/* disable internal load */
		ret = stwlc88_fw_iload_disable();
		if (ret)
			continue;
		/* fw ftp erase */
		ret = stwlc88_fw_full_erase_ftp();
		if (ret)
			continue;

		hwlog_info("[program_ftp_prepare] succ, cnt=%d\n", i);
		return 0;
	}

	return -WLC_ERR_I2C_W;
}

static int stwlc88_fw_write_ftp_sector(const u8 *data, int len, u8 sector_id)
{
	int ret;
	u8 wr_buff[ST88_FTP_SECTOR_SIZE];

	if (len > ST88_FTP_SECTOR_SIZE) {
		hwlog_err("write_ftp_sector: data_len>%d bytes\n",
			ST88_FTP_SECTOR_SIZE);
		return -WLC_ERR_PARA_WRONG;
	}

	memset(wr_buff, 0, ST88_FTP_SECTOR_SIZE);
	memcpy(wr_buff, data, len);

	ret = stwlc88_write_byte(ST88_AUX_ADDR_ADDR, sector_id);
	if (ret) {
		hwlog_err("write_ftp_sector: write sector_id failed\n");
		return ret;
	}
	ret = stwlc88_write_block(ST88_AUX_DATA_ADDR, wr_buff, len);
	if (ret) {
		hwlog_err("write_ftp_sector: write ftp_data failed\n");
		return ret;
	}

	return stwlc88_fw_write_sys_cmd(ST88_SYS_CMD_FTP_WR,
		ST88_SYS_CMD_FTP_WR_SHIFT, ST88_SYS_CMD_VAL);
}

static int stwlc88_fw_write_ftp_bulk(const u8 *data, int len, u8 sector_id)
{
	int ret;
	int remaining = len;
	int size_to_wr;
	int wr_already = 0;

	while (remaining > 0) {
		size_to_wr = remaining > ST88_FTP_SECTOR_SIZE ?
			ST88_FTP_SECTOR_SIZE : remaining;
		ret = stwlc88_fw_write_ftp_sector(data + wr_already,
			size_to_wr, sector_id);
		if (ret) {
			hwlog_err("write_ftp_bulk: failed, sector_id=%d\n",
				sector_id);
			return ret;
		}

		remaining -= size_to_wr;
		wr_already += size_to_wr;
		sector_id++;
	}

	return 0;
}

static int stwlc88_fw_write_ftp_patch(void)
{
	return stwlc88_fw_write_ftp_bulk(g_st88_ftp_patch,
		ARRAY_SIZE(g_st88_ftp_patch), ST88_FTP_PATCH_START_SECTOR_ID);
}

static int stwlc88_fw_write_ftp_cfg(void)
{
	return stwlc88_fw_write_ftp_bulk(g_st88_ftp_cfg,
		ARRAY_SIZE(g_st88_ftp_cfg), ST88_FTP_CFG_START_SECTOR_ID);
}

static int stwlc88_fw_load_ftp(void)
{
	int ret;

	ret = stwlc88_fw_program_ftp_pre_check();
	if (ret)
		return ret;
	ret = stwlc88_fw_program_ftp_prepare();
	if (ret)
		return ret;
	ret = stwlc88_fw_write_ftp_patch();
	if (ret)
		return ret;
	ret = stwlc88_fw_write_ftp_cfg();
	if (ret)
		return ret;
	ret = stwlc88_fw_reset_system();
	if (ret)
		return ret;
	ret = stwlc88_fw_iload_disable();
	if (ret)
		return ret;

	return 0;
}

static int stwlc88_fw_program_ftp(void)
{
	int ret;

	stwlc88_disable_irq_nosync();
	stwlc88_ps_control(WLPS_PROC_OTP_PWR, WLPS_CTRL_ON);
	stwlc88_chip_enable(RX_EN_ENABLE);
	msleep(100); /* delay for power on */

	ret = stwlc88_fw_hang_process();
	if (ret < 0)
		goto exit;
	ret = stwlc88_check_ftp_match();
	if (!ret) /* ftp latest */
		goto exit;
	ret = stwlc88_fw_load_ftp();
	if (ret)
		goto exit;
	ret = stwlc88_check_ftp_match();

exit:
	stwlc88_enable_irq();
	stwlc88_ps_control(WLPS_PROC_OTP_PWR, WLPS_CTRL_OFF);
	return ret;
}

static int stwlc88_fw_rx_program_ftp(int proc_type)
{
	int ret;
	struct stwlc88_dev_info *di = NULL;

	stwlc88_get_dev_info(&di);
	if (!di)
		return -WLC_ERR_PARA_NULL;

	hwlog_info("program_ftp: type=%d\n", proc_type);

	if (!di->g_val.ftp_chk_complete)
		return -WLC_ERR_I2C_WR;

	di->g_val.ftp_chk_complete = false;
	ret = stwlc88_fw_program_ftp();
	if (!ret)
		hwlog_info("[rx_program_ftp] succ\n");
	di->g_val.ftp_chk_complete = true;

	return ret;
}

static int stwlc88_fw_check_ftp(void)
{
	if (g_st88_ftp_status == ST88_FTP_LATEST)
		return 0;

	stwlc88_ps_control(WLPS_RX_EXT_PWR, WLPS_CTRL_ON);
	wlps_control(WLPS_RX_SW, WLPS_CTRL_OFF);
	charge_pump_chip_enable(CP_TYPE_MAIN, false);
	msleep(100); /* delay for power on */

	if (stwlc88_check_ftp_match())
		goto check_fail;

	stwlc88_ps_control(WLPS_RX_EXT_PWR, WLPS_CTRL_OFF);
	return 0;

check_fail:
	stwlc88_ps_control(WLPS_RX_EXT_PWR, WLPS_CTRL_OFF);
	return -WLC_ERR_CHECK_FAIL;
}

int stwlc88_fw_sram_update(enum wireless_mode sram_mode)
{
	int ret;
	u16 cfg_id = 0;
	u16 patch_id = 0;

	ret = stwlc88_get_cfg_id(&cfg_id);
	ret += stwlc88_get_ftp_patch_id(&patch_id);
	if (!ret)
		hwlog_info("[ftp_version] patch_id=0x%x, cfg_id=0x%x\n",
			patch_id, cfg_id);

	return 0;
}

static int stwlc88_fw_is_ftp_exist(void)
{
	int ret;
	struct stwlc88_dev_info *di = NULL;

	stwlc88_get_dev_info(&di);
	if (!di) {
		hwlog_err("is_ftp_exist: di null\n");
		return WIRELESS_FW_NON_PROGRAMED;
	}

	di->g_val.ftp_chk_complete = false;
	ret = stwlc88_fw_check_ftp();
	if (!ret) {
		di->g_val.ftp_chk_complete = true;
		return WIRELESS_FW_PROGRAMED;
	}
	di->g_val.ftp_chk_complete = true;

	return WIRELESS_FW_NON_PROGRAMED;
}

void stwlc88_fw_ftp_check_work(struct work_struct *work)
{
	int i;
	int ret;
	struct stwlc88_dev_info *di = NULL;

	stwlc88_get_dev_info(&di);
	if (!di) {
		hwlog_err("ftp_check_work: di null\n");
		return;
	}

	di->g_val.ftp_chk_complete = false;
	ret = stwlc88_fw_check_ftp();
	if (!ret) {
		(void)stwlc88_chip_reset();
		hwlog_info("[ftp_check_work] succ\n");
		goto exit;
	}

	/* program for 3 times until it's ok */
	for (i = 0; i < 3; i++) {
		ret = stwlc88_fw_program_ftp();
		if (ret)
			continue;
		hwlog_info("ftp_check_work: update ftp succ, cnt=%d\n", i + 1);
		goto exit;
	}
	hwlog_err("ftp_check_work: update ftp failed\n");

exit:
	di->g_val.ftp_chk_complete = true;
}

static struct wireless_fw_ops g_stwlc88_fw_ops = {
	.program_fw  = stwlc88_fw_rx_program_ftp,
	.is_fw_exist = stwlc88_fw_is_ftp_exist,
	.check_fw    = stwlc88_fw_check_ftp,
};

int stwlc88_fw_ops_register(void)
{
	return wireless_fw_ops_register(&g_stwlc88_fw_ops);
}
