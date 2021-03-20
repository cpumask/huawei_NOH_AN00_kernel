/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 */

#include "socp_hal_common.h"
#include <of.h>
#include "bsp_socp.h"
#include "bsp_print.h"


struct device_node * socp_hal_get_dev_info(void)
{
    return g_socp_hal_info.dev;
}

u8 *socp_hal_get_socp_base_addr(void)
{
    return g_socp_hal_info.base_addr;
}

void socp_hal_set_timeout_threshold(u32 mode, u32 timeout)
{
    u32 temp;

    temp = SOCP_REG_READ(SOCP_REG_INTTIMEOUT);

    if (mode == SOCP_TIMEOUT_TFR_LONG) {
        timeout = (timeout << 8) | (temp & 0xff);
    } else {
        timeout = (temp & 0xffffff00) | timeout;
    }

    SOCP_REG_WRITE(SOCP_REG_INTTIMEOUT, timeout);

    return;
}

void socp_hal_global_reset(void)
{
    SOCP_REG_SETBITS(SOCP_REG_GBLRST, 0, 1, 0x1);
}

u32 socp_hal_get_global_reset_state(void)
{
    return SOCP_REG_GETBITS(SOCP_REG_GBLRST, 0, 1);
}

u32 socp_hal_get_timeout_mode(void)
{
    return SOCP_REG_GETBITS(SOCP_REG_GBLRST, 4, 1);
}

void socp_hal_set_single_chan_timeout_mode(u32 chan_id, u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_ENCDEST_BUFMODE_CFG(chan_id), 3, 1, value);
}

void socp_hal_set_timeout_mode(u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_GBLRST, 4, 1, value);
}

u32 socp_hal_get_src_debug_cfg(u32 chan_id)
{
    return SOCP_REG_GETBITS(SOCP_REG_ENCSRC_BUFCFG(chan_id), 31, 1);
}


u32 socp_hal_get_tfr_modeswt_int_state(void)
{
    return SOCP_REG_READ(SOCP_REG_ENC_CORE0_INT0);
}

u32 socp_hal_get_tfr_modeswt_int_mask(void)
{
    return SOCP_REG_READ(SOCP_REG_ENC_CORE0_MASK0);
}

void socp_hal_set_tfr_int_mask(u32 value)
{
    SOCP_REG_WRITE(SOCP_REG_ENC_CORE0_MASK0, value);
}

u32 socp_hal_get_overflow_int_mask(void)
{
    return SOCP_REG_READ(SOCP_REG_ENC_CORE0_MASK2);
}

void socp_hal_set_overflow_int_mask(u32 value)
{
    SOCP_REG_WRITE(SOCP_REG_ENC_CORE0_MASK2, value);
}

void socp_hal_clear_tfr_raw_int(u32 value)
{
    SOCP_REG_WRITE(SOCP_REG_ENC_RAWINT0, value);
}

u32 socp_hal_get_overflow_int_state(void)
{
    return SOCP_REG_READ(SOCP_REG_ENC_CORE0_INTSTAT2);
}

void socp_hal_clear_overflow_raw_int(u32 value)
{
    SOCP_REG_WRITE(SOCP_REG_ENC_RAWINT2, value);
}

void socp_hal_set_modeswt_int_mask(u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_ENC_CORE0_MASK0, 16, 7, value);
}

void socp_hal_clear_modeswt_raw_int(u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_ENC_RAWINT0, 16, 7, value);
}

void socp_hal_clear_single_chan_tfr_raw_int(u32 chan_id)
{
    SOCP_REG_SETBITS(SOCP_REG_ENC_RAWINT0, chan_id, 1, 1);
}

void socp_hal_set_single_chan_tfr_mask(u32 chan_id, u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_ENC_CORE0_MASK0, chan_id, 1, value);
}

void socp_hal_clear_single_chan_ovf_raw_int(u32 chan_id)
{
    SOCP_REG_SETBITS(SOCP_REG_ENC_RAWINT2, chan_id, 1, 1);
}

void socp_hal_set_single_chan_ovf_mask(u32 chan_id, u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_ENC_CORE0_MASK2, chan_id, 1, value);
}

void socp_hal_clear_single_chan_thr_ovf_raw_int(u32 chan_id)
{
    SOCP_REG_SETBITS(SOCP_REG_ENC_RAWINT2, chan_id + 16, 1, 1);
}

void socp_hal_set_single_chan_thr_ovf_mask(u32 chan_id, u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_ENC_CORE0_MASK2, chan_id + 16, 1, value);
}

u32 socp_hal_get_dst_chan_mode(u32 chan_id)
{
    return SOCP_REG_GETBITS(SOCP_REG_ENCDEST_BUFMODE_CFG(chan_id), 2, 1);
}

void socp_hal_set_dst_chan_mode(u32 chan_id, u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_ENCDEST_BUFMODE_CFG(chan_id), 1, 1, value);
}

void socp_hal_set_clk_ctrl(u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_CLKCTRL, 0, 1, value);
}


void socp_hal_set_dst_buffer_addr(u32 chan_id, u32 value1, u32 value2)
{
    SOCP_REG_WRITE(SOCP_REG_ENCDEST_BUFADDR_L(chan_id), value1);
    SOCP_REG_WRITE(SOCP_REG_ENCDEST_BUFADDR_H(chan_id), value2);
}
void socp_hal_set_dst_write_ptr(u32 chan_id, u32 value)
{
    SOCP_REG_WRITE(SOCP_REG_ENCDEST_BUFWPTR(chan_id), value);
}

void socp_hal_set_dst_read_ptr(u32 chan_id, u32 value)
{
    SOCP_REG_WRITE(SOCP_REG_ENCDEST_BUFRPTR(chan_id), value);
}

void socp_hal_set_dst_buffer_length(u32 chan_id, u32 value)
{
    SOCP_REG_WRITE(SOCP_REG_ENCDEST_BUFDEPTH(chan_id), value);
}

u32 socp_hal_get_dst_buffer_length(u32 chan_id)
{
    return SOCP_REG_READ(SOCP_REG_ENCDEST_BUFDEPTH(chan_id));
}

void socp_hal_set_dst_threshold_length(u32 chan_id, u32 value)
{
    SOCP_REG_WRITE(SOCP_REG_ENCDEST_BUFTHRESHOLD(chan_id), value);
}
void socp_hal_set_dst_arbitrate_length(u32 chan_id, u32 value)
{
    SOCP_REG_WRITE(SOCP_REG_ENCDEST_BUFTHRH(chan_id), value);
}

void socp_hal_set_dst_chan_start_stop(u32 chan_id, u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_ENCDEST_BUFMODE_CFG(chan_id), 0, 1, value);
}

void socp_hal_set_src_buffer_addr(u32 chan_id, u32 addr_low, u32 addr_high)
{
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFADDR_L(chan_id), addr_low);
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFADDR_H(chan_id), addr_high);
}

u32 socp_hal_get_dst_read_ptr(u32 chan_id)
{
    return SOCP_REG_READ(SOCP_REG_ENCDEST_BUFRPTR(chan_id));
}

u32 socp_hal_get_dst_write_ptr(u32 chan_id)
{
    return SOCP_REG_READ(SOCP_REG_ENCDEST_BUFWPTR(chan_id));
}

void socp_hal_set_src_rd_buffer_addr(u32 chan_id, u32 addr_low, u32 addr_high)
{
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQADDR_L(chan_id), addr_low);
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQADDR_H(chan_id), addr_high);
}

void socp_hal_set_src_write_ptr(u32 chan_id, u32 value)
{
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFWPTR(chan_id), value);
}

void socp_hal_set_src_read_ptr(u32 chan_id, u32 value)
{
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFRPTR(chan_id), value);
}

void socp_hal_set_src_rd_write_ptr(u32 chan_id, u32 value)
{
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQWPTR(chan_id), value);
}

void socp_hal_set_src_rd_read_ptr(u32 chan_id, u32 value)
{
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQRPTR(chan_id), value);
}


void socp_hal_set_dst_chan_id(u32 chan_id, u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG(chan_id), 4, 2, value);
}

void socp_hal_set_src_buffer_length(u32 chan_id, u32 value)
{
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFDEPTH(chan_id), value);
}

void socp_hal_set_src_rd_buffer_length(u32 chan_id, u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_RDQCFG(chan_id), 0, 16, value);
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_RDQCFG(chan_id), 16, 16, 0);
}


void socp_hal_set_src_rd_wptr_addr(u32 chan_id, u32 addr_low, u32 addr_high)
{
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQ_WPTR_ADDR_L(chan_id), addr_low);
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQ_WPTR_ADDR_H(chan_id), addr_high);
}

void socp_hal_set_src_chan_start_stop(u32 chan_id, u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG(chan_id), 0, 1, value);
}


u32 socp_hal_get_src_write_ptr(u32 chan_id)
{
    return SOCP_REG_READ(SOCP_REG_ENCSRC_BUFWPTR(chan_id));
}

u32 socp_hal_get_src_read_ptr(u32 chan_id)
{
    return SOCP_REG_READ(SOCP_REG_ENCSRC_BUFRPTR(chan_id));
}

u32 socp_hal_get_src_rd_write_ptr(u32 chan_id)
{
    return SOCP_REG_READ(SOCP_REG_ENCSRC_RDQWPTR(chan_id));
}

u32 socp_hal_get_src_start_stop_state(u32 chan_id)
{
    return SOCP_REG_GETBITS(SOCP_REG_ENCSRC_BUFCFG(chan_id), 0, 1);
}

u32 socp_hal_get_global_int_state(void)
{
    return SOCP_REG_READ(SOCP_REG_GBL_INTSTAT);
}

void socp_hal_set_global_halt(u32 value)
{
    SOCP_REG_SETBITS(SOCP_REG_GBLRST, 16, 1, value);
}



