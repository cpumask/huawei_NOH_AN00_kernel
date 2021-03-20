/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "deflate.h"
#include <linux/clk.h>
#include <linux/module.h>
#include <of.h>
#include <osl_thread.h>



#undef THIS_MODU
#define THIS_MODU mod_deflate

struct deflate_ctrl_info g_deflate_ctrl;
struct deflate_debug_info g_deflate_debug;
struct deflate_abort_info g_strDeflateAbort;
u32 g_deflate_enable_state = 0;
struct socp_enc_dst_log_cfg g_deflate_dst_buf_log_cfg;

DRV_DEFLATE_CFG_STRU g_deflate_nv_ctrl;
#define DEFFLATE_TASK_PRO 81
#define DEFLATE_TASK_STACK_SIZE 4096

/*
 * 函 数 名: deflate_get_data_buffer
 * 功能描述: 获取空闲缓冲区的数据
 * 输入参数:  ring_buffer       待查询的环形buffer
 *                 p_rw_buff         输出的环形buffer
 * 输出参数: 无
 * 返 回 值:  无
 */
void deflate_get_data_buffer(deflate_ring_buf_s *ring_buffer, deflate_buffer_rw_s *p_rw_buff)
{
    if (ring_buffer->read <= ring_buffer->write) {
        /* 写指针大于读指针，直接计算 */
        p_rw_buff->buffer = (char *)(uintptr_t)(ring_buffer->start + (u32)ring_buffer->read);
        p_rw_buff->size = (u32)(ring_buffer->write - ring_buffer->read);
        p_rw_buff->rb_buffer = (char *)BSP_NULL;
        p_rw_buff->rb_size = 0;
    } else {
        /* 读指针大于写指针，需要考虑回卷 */
        p_rw_buff->buffer = (char *)(uintptr_t)(ring_buffer->start + (u32)ring_buffer->read);
        p_rw_buff->size = ((u32)((u64)ring_buffer->end - ((u64)ring_buffer->start + ring_buffer->read)) + 1);
        p_rw_buff->rb_buffer = (char *)(uintptr_t)ring_buffer->start;
        p_rw_buff->rb_size = ring_buffer->write;
    }
}
/*
 * 函 数 名: deflate_read_done
 * 功能描述: 更新缓冲区的读指针
 * 输入参数:  ring_buffer       待更新的环形buffer
 *                 size          更新的数据长度
 * 输出参数: 无
 * 返 回 值:  无
 */
void deflate_read_done(deflate_ring_buf_s *ring_buffer, u32 size)
{
    ring_buffer->read += size;
    if (ring_buffer->read > (u32)(ring_buffer->end - ring_buffer->start)) {
        ring_buffer->read -= ring_buffer->length;
    }
}
/*
 * 函 数 名: deflate_debug
 * 功能描述:读取寄存器
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 */
void deflate_debug(void)
{
    /*lint -save -e732*/
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_GLOBALCTRL, g_strDeflateAbort.deflate_global);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_IBUFTIMEOUTCFG, g_strDeflateAbort.ibuf_timeout_config);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_RAWINT, g_strDeflateAbort.raw_int);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_INT, g_strDeflateAbort.int_state);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_INTMASK, g_strDeflateAbort.int_mask);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_TFRTIMEOUTCFG, g_strDeflateAbort.thf_timeout_config);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_STATE, g_strDeflateAbort.deflate_time);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_ABORTSTATERECORD, g_strDeflateAbort.abort_state_record);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEBUG_CH, g_strDeflateAbort.debug_chan);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFREMAINTHCFG, g_strDeflateAbort.obuf_thrh);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFRPTR, g_strDeflateAbort.read_addr);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFWPTR, g_strDeflateAbort.write_addr);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDST_BUFADDR_L, g_strDeflateAbort.start_low_addr);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDST_BUFADDR_H, g_strDeflateAbort.start_high_addr);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFDEPTH, g_strDeflateAbort.buff_size);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFTHRH, g_strDeflateAbort.int_thrh);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFOVFTIMEOUT, g_strDeflateAbort.over_timeout_en);
    DEFLATE_REG_READ(SOCP_REG_SOCP_MAX_PKG_BYTE_CFG, g_strDeflateAbort.pkg_config);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_OBUF_DEBUG, g_strDeflateAbort.obuf_debug);
    /*lint -restore +e732*/
}
/*
 * 函 数 名: deflate_set
 * 功能描述:deflate配置接口，当配置socp目的端时候调用
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 配置成功与否的标识码
 */
u32 deflate_set(u32 dst_chan_id, deflate_chan_config_s *deflate_attr)
{
    unsigned long start;
    unsigned long end;
    u32 buf_thrhold;
    u32 buf_length;
    u32 chan_id;
    u32 threshold;

    struct deflate_ctrl_info *p_chan = NULL;
    /* 判断是否已经初始化 */
    if (!g_deflate_ctrl.init_flag) {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }
    g_deflate_debug.deflate_dst_set_cnt++;
    if (deflate_attr == NULL) {
        socp_error("the parameter is NULL!\n");
        return DEFLATE_ERR_NULL;
    }

    /* 判断参数有效性 */
    chan_id = DEFLATE_REAL_CHAN_ID(dst_chan_id);
    if (chan_id > DEFLATE_MAX_ENCDST_CHN) {
        socp_error("the chan id(0x%x) is invalid!\n", chan_id);
        return DEFLATE_ERR_INVALID_PARA;
    }
    start = (uintptr_t)deflate_attr->coder_dst_setbuf.output_start;
    end = (uintptr_t)deflate_attr->coder_dst_setbuf.output_end;
    buf_thrhold = deflate_attr->coder_dst_setbuf.threshold;  // socp寄存器是kbyte为单位,deflate是byte为单位
    threshold = 32 * 1024;                                               // 阈值溢出,芯片建议该值在32KBytes以上

    buf_length = (u32)((end - start) + 1);
    if ((buf_length % 8) != 0) {
        socp_error("the parameter is not 8 bytes aligned!\n");
        return DEFLATE_ERR_NOT_8BYTESALIGN;
    }

    /* 如果经过配置则不能再次配置 */
    p_chan = &g_deflate_ctrl;

    if (!p_chan->set_state) {
        /* 写入起始地址到目的buffer起始地址寄存器 */
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDST_BUFADDR_L, (u32)start);
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDST_BUFADDR_H, (u32)((u64)start >> 32));
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFRPTR, 0);
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFRPTR, 0);
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFDEPTH, buf_length);
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFREMAINTHCFG, threshold);  // 阈值溢出
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFTHRH, buf_thrhold);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 22, 4, chan_id);

        /* 在g_strDeflateStat中保存参数 */
        p_chan->chan_id = dst_chan_id;
        p_chan->threshold = threshold;
        p_chan->deflate_dst_chan.start = start;
        p_chan->deflate_dst_chan.end = end;
        p_chan->deflate_dst_chan.write = 0;
        p_chan->deflate_dst_chan.read = 0;
        p_chan->deflate_dst_chan.length = buf_length;

        /* 表明该通道已经配置 */
        p_chan->set_state = DEFLATE_CHN_SET;
        socp_crit("Deflate set finished!\n");
    } else {
        socp_crit("Deflate have been set!\n");
    }
    return DEFLATE_OK;
}
/*
 * 函 数 名: deflate_ctrl_clear
 * 功能描述: 全局变量g_strDeflateCtrl清0
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 成功标识符
 */
u32 deflate_ctrl_clear(u32 dst_chan_id)
{
    u32 chan_id;
    chan_id = DEFLATE_REAL_CHAN_ID(dst_chan_id);
    if (chan_id > DEFLATE_MAX_ENCDST_CHN) {
        socp_error("the chan id is invalid!\n");
        return DEFLATE_ERR_INVALID_PARA;
    }
    /* deflate中断标志初始化 */
    g_deflate_ctrl.deflate_int_dst_ovf = 0;
    g_deflate_ctrl.deflate_int_dst_tfr = 0;
    g_deflate_ctrl.deflate_int_dst_thrh_ovf = 0;
    g_deflate_ctrl.deflate_int_dst_work_abort = 0;

    /* deflate目的通道属性初始化 */
    g_deflate_ctrl.set_state = 0;
    g_deflate_ctrl.threshold = 0;

    return DEFLATE_OK;
}
/*
 * 函 数 名: deflate_enable
 * 功能描述:deflate使能接口,供SOCP非压缩转压缩调用
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 成功标识符
 */
u32 deflate_enable(u32 dst_chan_id)
{
    u32 chan_id;

    chan_id = DEFLATE_REAL_CHAN_ID(dst_chan_id);
    if (chan_id > DEFLATE_MAX_ENCDST_CHN) {
        socp_error("the chan id is invalid!\n");
        return DEFLATE_ERR_INVALID_PARA;
    }
    /* 判断是否已经初始化 */
    if (!g_deflate_ctrl.init_flag) {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }
    if (g_deflate_ctrl.set_state == DEFLATE_CHN_SET) {
        /* 清中断，开中断 */
        bsp_deflate_data_send_manager(COMPRESS_ENABLE_STATE);
        
        /* 使能deflate */
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 21, 1, 1);
        socp_crit("Deflate is enabled!\n");
    } else {
        socp_error("Deflate Enable failed!\n");
        return DEFLATE_ERR_SET_INVALID;
    }
    return DEFLATE_OK;
}
/*
 * 函 数 名: deflate_unable
 * 功能描述:deflate不使能接口,供SOCP压缩转非压缩场景调用
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 成功与否标识码
 */
u32 deflate_disable(u32 dst_chan_id)
{
    u32 deflate_idle_state;
    u32 chan_id;

    chan_id = DEFLATE_REAL_CHAN_ID(dst_chan_id);
    if (chan_id > DEFLATE_MAX_ENCDST_CHN) {
        socp_error("the chan id is invalid!\n");
        return DEFLATE_ERR_INVALID_PARA;
    }
    /* 判断是否已经初始化 */
    if (!g_deflate_ctrl.init_flag) {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }

    if (g_deflate_ctrl.set_state == DEFLATE_CHN_SET) {
        /* 立即压缩 */
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 20, 1, 1);

        /*lint -save -e732*/
        /* 读deflate全局状态寄存器，获取deflate工作状态 */
        DEFLATE_REG_READ(SOCP_REG_DEFLATE_GLOBALCTRL, deflate_idle_state);
        while (!(deflate_idle_state & DEFLATE_WORK_STATE)) {
            DEFLATE_REG_READ(SOCP_REG_DEFLATE_GLOBALCTRL, deflate_idle_state);
            msleep(1);
        }
        /*lint -save -e845*/
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 21, 1, 0);
        /*lint -restore +e845*/
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFRPTR, 0);
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFWPTR, 0);

        /* 清压缩中断状态，屏蔽中断状态 */
        bsp_deflate_data_send_manager(COMPRESS_DISABLE_STATE);
        socp_crit("Deflate is disabled!\n");
    } else {
        socp_error("Deflate disable failed!\n");
        return DEFLATE_ERR_SET_INVALID;
    }
    return DEFLATE_OK;
}
/*
 * 函 数 名: deflate_register_read_cb
 * 功能描述: deflate 注册读回调接口
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值:成功与否标识码
 */
u32 deflate_register_read_cb(deflate_read_cb read_cb)
{
    /* 判断是否已经初始化 */
    if (!g_deflate_ctrl.init_flag) {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }

    /* deflate是否已经配置 */
    if (g_deflate_ctrl.set_state == DEFLATE_CHN_SET) {
        /* 设置对应通道的回调函数 */
        g_deflate_ctrl.read_cb = read_cb;
        g_deflate_debug.deflate_reg_readcb_cnt++;
    } else {
        socp_error("Deflate readCB  failed!\n");
        return DEFLATE_ERR_SET_INVALID;
    }
    return DEFLATE_OK;
}
/*
 * 函 数 名: deflate_register_event_cb
 * 功能描述:deflate 注册event回调接口
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 成功与否标识码
 */
u32 deflate_register_event_cb(deflate_event_cb event_cb)
{
    /* 判断是否已经初始化 */
    if (!g_deflate_ctrl.init_flag) {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }

    /* deflate是否已经配置 */
    if (g_deflate_ctrl.set_state == DEFLATE_CHN_SET) {
        /* 设置对应通道的回调函数 */
        g_deflate_ctrl.event_cb = event_cb;
        g_deflate_debug.deflate_reg_eventcb_cnt++;
    } else {
        socp_error("Deflate RegiseEventCB  failed!\n");
        return DEFLATE_ERR_SET_INVALID;
    }
    return DEFLATE_OK;
}

/*
 * 函 数 名: deflate_read_data_done
 * 功能描述: deflate read done接口
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 成功与否标识码
 */
u32 deflate_read_data_done(u32 data_len)
{
    /*lint -save -e438*/
    deflate_buffer_rw_s rw_buff;
    u32 phy_addr;
    u32 cur_mode;
    unsigned long lock_flag;

    /* 判断是否已经初始化 */
    if (!g_deflate_ctrl.init_flag) {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }
    cur_mode = DEFLATE_REG_GETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 27, 1);
    if (cur_mode == 1) {
        socp_crit("deflate ind delay mode is cycle 0x%x!\n", cur_mode);
        return BSP_OK;
    }

    if (g_deflate_ctrl.set_state == DEFLATE_CHN_SET) {
        g_deflate_debug.deflate_readdone_etr_cnt++;
        if (data_len == 0) {
            g_deflate_debug.deflate_readdone_zero_cnt++;
        } else {
            g_deflate_debug.deflate_readdone_valid_cnt++;
        }
        DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFRPTR, phy_addr);
        g_deflate_ctrl.deflate_dst_chan.read = phy_addr;
        DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFWPTR, phy_addr);
        /*lint -restore +e732*/
        g_deflate_ctrl.deflate_dst_chan.write = phy_addr;
        deflate_get_data_buffer(&g_deflate_ctrl.deflate_dst_chan, &rw_buff);
        if (rw_buff.size + rw_buff.rb_size < data_len) {
            socp_error("rw_buff is not enough, data_len=0x%x!\n", data_len);

            /*lint -save -e550 -e845*/
            spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
            DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 2, 1, 1);
            DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 2, 1, 0);

            DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 6, 3, 0x07);
            DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 6, 1, 0);
            spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);

            g_deflate_debug.deflate_readdone_fail_cnt++;
            return DEFLATE_ERR_INVALID_PARA;
        }
        /* 更新读指针 */
        deflate_read_done(&g_deflate_ctrl.deflate_dst_chan, data_len);
        /* 写入读指针到读指针寄存器 */
        phy_addr = g_deflate_ctrl.deflate_dst_chan.read;
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFRPTR, phy_addr);

        spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
        bsp_deflate_data_send_continue();
        spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
        /*lint -restore +e550 +e845*/
        g_deflate_debug.deflate_readdone_suc_cnt++;
    } else {
        socp_error("Readdatadone failed!\n");
        return DEFLATE_ERR_SET_INVALID;
    }

    return DEFLATE_OK;
    /*lint -restore +e438*/
}
/*
 * 函 数 名: deflate_get_read_buffer
 * 功能描述: 获取deflate数据空间
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值:成功与否标识码
 */
u32 deflate_get_read_buffer(deflate_buffer_rw_s *p_rw_buff)
{
    u32 phy_addr;
    /* 判断参数有效性 */
    if (p_rw_buff == NULL) {
        socp_error("the parameter is NULL!\n");
        return DEFLATE_ERR_NULL;
    }
    /* 判断是否已经初始化 */
    if (!g_deflate_ctrl.init_flag) {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }

    if (g_deflate_ctrl.set_state == DEFLATE_CHN_SET) {
        g_deflate_debug.deflate_get_readbuf_etr_cnt++;
        /* 根据读写指针获取buffer */
        DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFRPTR, phy_addr);
        g_deflate_ctrl.deflate_dst_chan.read = phy_addr;

        DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFWPTR, phy_addr);
        g_deflate_ctrl.deflate_dst_chan.write = phy_addr;

        deflate_get_data_buffer(&g_deflate_ctrl.deflate_dst_chan, p_rw_buff);

        g_deflate_debug.deflate_get_readbuf_suc_cnt++;
    } else {
        socp_error("deflate set failed!\n");
        return DEFLATE_ERR_SET_INVALID;
    }

    return DEFLATE_OK;
}

/*
 * 函 数 名: deflate_int_handler
 * 功能描述:deflate中断处理接口
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 */
u32 deflate_int_handler(void)
{
    /*lint -save -e438*/
    u32 is_handle = false;
    u32 init_state = 0;
    unsigned long lock_flag;
    u32 mask;
    if (g_deflate_ctrl.set_state == DEFLATE_CHN_SET) {
        DEFLATE_REG_READ(SOCP_REG_DEFLATE_INT, init_state);
        /* 阈值传输中断 */
        if (init_state & DEFLATE_TFR_MASK) {
            spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
            DEFLATE_REG_READ(SOCP_REG_DEFLATE_INTMASK, mask);
            DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_INTMASK, init_state | mask);
            DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_RAWINT, init_state);
            g_deflate_ctrl.deflate_int_dst_tfr |= init_state;
            spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
            is_handle = TRUE;
        } else if (init_state & DEFLATE_THROVF_MASK) { /* buffer阈值溢出 */
            spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
            DEFLATE_REG_READ(SOCP_REG_DEFLATE_INTMASK, mask);
            DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_INTMASK, init_state | mask);
            DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_RAWINT, init_state);
            g_deflate_ctrl.deflate_int_dst_thrh_ovf |= init_state;
            spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
            is_handle = TRUE;
        } else if (init_state & DEFLATE_OVF_MASK) { /* buffer上溢中断 */
            spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
            DEFLATE_REG_READ(SOCP_REG_DEFLATE_INTMASK, mask);
            DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_INTMASK, init_state | mask);
            DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_RAWINT, init_state);
            g_deflate_ctrl.deflate_int_dst_ovf |= init_state;
            spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
            is_handle = TRUE;
        } else if (init_state & DEFLATE_WORK_ABORT_MASK) { /* 异常中断 */
            spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
            DEFLATE_REG_READ(SOCP_REG_DEFLATE_INTMASK, mask);
            DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_INTMASK, init_state | mask);
            DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_RAWINT, init_state);
            g_deflate_ctrl.deflate_int_dst_work_abort |= init_state;
            spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
            is_handle = TRUE;
        } else if (init_state & DEFLATE_CYCLE_MASK) { /* 循环模式中断 */
            spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
            DEFLATE_REG_READ(SOCP_REG_DEFLATE_INTMASK, mask);
            DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_INTMASK, init_state | mask);
            DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_RAWINT, init_state);
            g_deflate_ctrl.int_deflate_cycle |= init_state;
            spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
            is_handle = TRUE;
        } else if (init_state & DEFLATE_NOCYCLE_MASK) { /* 阻塞模式中断 */
            spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
            DEFLATE_REG_READ(SOCP_REG_DEFLATE_INTMASK, mask);
            DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_INTMASK, init_state | mask);
            DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_RAWINT, init_state);
            g_deflate_ctrl.int_deflate_no_cycle |= init_state;
            spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
            is_handle = TRUE;
        } else {
            is_handle = FALSE;
        }
    }
    if (is_handle) {
        osl_sem_up(&g_deflate_ctrl.task_sem);
    }
    /*lint -restore +e438*/
    return DEFLATE_OK;
}
/*
 * 函 数 名: deflate_cycle
 * 功能描述: deflate 循环模式处理
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 */
void deflate_cycle(void)
{
    unsigned long lock_flag;
    /* 检测通道是否配置 */
    if (g_deflate_ctrl.set_state == DEFLATE_CHN_SET) {
        /* 屏蔽别的中断 */
        spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 2, 1, 1);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 2, 1, 1);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 6, 3, 0x07);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 6, 3, 0x07);
        /* 打开阻塞中断 */
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 10, 1, 1);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 10, 1, 0);
        spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
        if (g_deflate_ctrl.event_cb) {
            (void)g_deflate_ctrl.event_cb(g_deflate_ctrl.chan_id, DEFLATE_EVENT_CYCLE, 0);
        }
    }
    return;
}
void deflate_nocycle(void)
{
    unsigned long lock_flag;

    /* 检测通道是否配置 */
    if (g_deflate_ctrl.set_state == DEFLATE_CHN_SET) {
        /* 打开别的中断 */
        spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 2, 1, 1);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 2, 1, 0);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 6, 3, 0x07);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 6, 3, 0);
        /* 打开循环模式中断 */
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 9, 1, 1);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 9, 1, 0);
        spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
        if (g_deflate_ctrl.event_cb) {
            (void)g_deflate_ctrl.event_cb(g_deflate_ctrl.chan_id, DEFLATE_EVENT_NOCYCLE, 0);
        }
    }
    return;
}
void deflate_thresholdovf(void)
{
    /* 检测通道是否配置 */
    if (g_deflate_ctrl.set_state == DEFLATE_CHN_SET) {
        if (g_deflate_ctrl.event_cb) {
            g_deflate_debug.deflate_task_thrh_ovf_cb_ori_cnt++;
            (void)g_deflate_ctrl.event_cb(g_deflate_ctrl.chan_id, DEFLATE_EVENT_THRESHOLD_OVERFLOW, 0);
            g_deflate_debug.deflate_task_thrh_ovf_cb_cnt++;
        }
        if (g_deflate_ctrl.read_cb) {
            g_deflate_debug.deflate_task_thrh_ovf_cb_ori_cnt++;
            (void)g_deflate_ctrl.read_cb(g_deflate_ctrl.chan_id);
            g_deflate_debug.deflate_task_thrh_ovf_cb_cnt++;
        }
    }

    return;
}
void deflate_ovf(void)
{
    /* 检测通道是否配置 */
    if (g_deflate_ctrl.set_state == DEFLATE_CHN_SET) {
        if (g_deflate_ctrl.event_cb) {
            g_deflate_debug.deflate_task_ovf_cb_ori_cnt++;
            (void)g_deflate_ctrl.event_cb(g_deflate_ctrl.chan_id, DEFLATE_EVENT_OVERFLOW, 0);
            g_deflate_debug.deflate_task_ovf_cb_cnt++;
        }
        
        if (g_deflate_ctrl.read_cb) {
            g_deflate_debug.deflate_task_ovf_cb_ori_cnt++;
            (void)g_deflate_ctrl.read_cb(g_deflate_ctrl.chan_id);
            g_deflate_debug.deflate_task_trf_cb_cnt++;
        }
    }
    return;
}
/*
 * 函 数 名: deflate_task
 * 功能描述: deflate 任务处理接口
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 */
int deflate_task(void *para)
{
    u32 int_trf_state = 0;
    u32 int_ovf_state = 0;
    u32 int_thrh_ovf_state = 0;
    u32 int_cycle_state = 0;
    u32 int_no_cycle_state = 0;
    u32 int_work_abort_state = 0;
    unsigned long lock_flag;

    while (1) {
        osl_sem_down(&g_deflate_ctrl.task_sem);
        /*lint -save -e550*/
        spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);

        int_trf_state = g_deflate_ctrl.deflate_int_dst_tfr;
        g_deflate_ctrl.deflate_int_dst_tfr = 0;

        int_thrh_ovf_state = g_deflate_ctrl.deflate_int_dst_thrh_ovf;
        g_deflate_ctrl.deflate_int_dst_thrh_ovf = 0;

        int_ovf_state = g_deflate_ctrl.deflate_int_dst_ovf;
        g_deflate_ctrl.deflate_int_dst_ovf = 0;

        int_cycle_state = g_deflate_ctrl.int_deflate_cycle;
        g_deflate_ctrl.int_deflate_cycle = 0;

        int_no_cycle_state = g_deflate_ctrl.int_deflate_no_cycle;
        g_deflate_ctrl.int_deflate_no_cycle = 0;

        int_work_abort_state = g_deflate_ctrl.deflate_int_dst_work_abort;
        g_deflate_ctrl.deflate_int_dst_work_abort = 0;

        spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
        /*lint -restore +e550*/
        /* 处理传输中断 */
        if (int_trf_state) {
            /* 检测通道是否配置 */
            if (g_deflate_ctrl.set_state == DEFLATE_CHN_SET) {
                if (g_deflate_ctrl.read_cb) {
                    g_deflate_debug.deflate_task_trf_cb_ori_cnt++;
                    (void)g_deflate_ctrl.read_cb(g_deflate_ctrl.chan_id);
                    g_deflate_debug.deflate_task_trf_cb_cnt++;
                }
            }
        }
        /* 处理目的 buffer 阈值溢出中断 */
        if (int_thrh_ovf_state) {
            deflate_thresholdovf();
        }
        /* 处理buffer上溢中断 */
        if (int_ovf_state) {
            deflate_ovf();
        }
        if (int_cycle_state) {
            deflate_cycle();
        }
        if (int_no_cycle_state) {
            deflate_nocycle();
        }
        /* 处理异常中断 */
        if (int_work_abort_state) {
            /* 检测通道是否配置 */
            if (g_deflate_ctrl.set_state == DEFLATE_CHN_SET) {
                if (g_deflate_ctrl.event_cb) {
                    g_deflate_debug.deflate_task_int_workabort_cb_ori_cnt++;
                    (void)g_deflate_ctrl.event_cb(g_deflate_ctrl.chan_id, DEFLATE_EVENT_WORK_ABORT, 0);
                    deflate_debug();
                    g_deflate_debug.deflate_task_int_workabort_cb_cnt++;
                }
            }
        }
    }

    return 0;
}

/*
 * 函 数 名: deflate_set_dst_threshold
 * 功能描述:deflate传输阈值上报接口，用于立即上报、延时上报
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 */
void deflate_set_dst_threshold(bool mode)
{
    u32 buf_len;
    u32 thrh;

    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFDEPTH, buf_len);
    if (mode == true) { /* true为需要打开延时上报的场景 */
        thrh = (buf_len >> 2) * 3;
    } else {
        thrh = 0x1000;
    }

    DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFTHRH, thrh);

    socp_crit(" set encdst thrh success! 0x%x\n", thrh);

    return;
}

/*
 * 函 数 名: deflate_set_cycle_mode
 * 功能描述: SOCP循环模式设置
 * 输入参数: 通道号、模式
 * 输出参数: 无
 * 返 回 值: 无
 */
s32 deflate_set_cycle_mode(u32 cycle)
{
    u32 mode_state;
    u32 waittime = 10000;
    unsigned long lock_flag;

    mode_state = DEFLATE_REG_GETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 27, 1);
    if ((cycle == SOCP_IND_MODE_DIRECT || cycle == SOCP_IND_MODE_DELAY) && mode_state) {
        u32 i;
        spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 26, 1, 0);
        spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
        for (i = 0; i < waittime; i++) {
            mode_state = DEFLATE_REG_GETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 27, 1);
            if (mode_state == 0) {
                break;
            }
        }

        if (waittime == i) {
            socp_error("set encdst cycle off timeout!\n");
            return DEFLATE_ERR_GET_CYCLE;
        }
        bsp_deflate_data_send_manager(COMPRESS_ENABLE_STATE);
        return DEFLATE_OK;
    } else if ((cycle == SOCP_IND_MODE_CYCLE) && (!mode_state)) {
        u32 i;

        bsp_deflate_data_send_manager(COMPRESS_DISABLE_STATE);
        spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 26, 1, 1);
        spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
        for (i = 0; i < waittime; i++) {
            mode_state = DEFLATE_REG_GETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 27, 1);
            if (mode_state == 1) {
                break;
            }
        }

        if (waittime == i) {
            socp_error("set encdst cycle on timeout!\n");
            return DEFLATE_ERR_GET_CYCLE;
        }
    }

    return BSP_OK;
}
/*
 * 函 数 名: deflate_set_time
 * 功能描述: DEFLATE时间设置
 * 输入参数: 通道号、模式
 * 输出参数: 无
 * 返 回 值: 无
 */
s32 deflate_set_time(u32 mode)
{
    if ((mode != SOCP_IND_MODE_DIRECT) && (mode != SOCP_IND_MODE_DELAY) && (mode != SOCP_IND_MODE_CYCLE)) {
        socp_error("mode error !\n");
        return DEFLATE_ERR_IND_MODE;
    }
    
    if (mode == SOCP_IND_MODE_DIRECT) {
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_TFRTIMEOUTCFG, DEFLATE_DIRECT_COMPRESS_TIMEOUT);
        return DEFLATE_OK;
    } else if (mode == SOCP_IND_MODE_DELAY) {
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_TFRTIMEOUTCFG, DEFLATE_DELAY_COMPRESS_TIMEOUT);
        return DEFLATE_OK;
    } else {
        socp_crit("cycle mode need not set time !\n");
        return DEFLATE_OK;
    }
}
/*
 * 函 数 名: bsp_deflate_data_send_manager
 * 功能描述: deflate编码目的端上报数据
 * 输入参数: enc_dst_chan_id: 编码目的端通道号
 *           b_enable: 中断使能
 * 输出参数: 无
 * 返 回 值: 无
 */
void bsp_deflate_data_send_manager(u32 b_enable)
{
    unsigned long lock_flag;

    if ((b_enable == COMPRESS_DISABLE_STATE) &&
        (g_deflate_enable_state == COMPRESS_ENABLE_STATE)) {
        spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 2, 1, 1);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 2, 1, 1);

        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 6, 3, 0x07);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 6, 3, 0x07);

        g_deflate_enable_state = COMPRESS_DISABLE_STATE;
        spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
    } else if ((b_enable == COMPRESS_ENABLE_STATE) &&
        (g_deflate_enable_state == COMPRESS_DISABLE_STATE)) {
        spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 2, 1, 1);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 2, 1, 0);

        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 6, 3, 0x07);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 6, 3, 0);
        g_deflate_enable_state = COMPRESS_ENABLE_STATE;

        spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
    } else {
        socp_crit(" deflate has enabled.\n");
    }
}
/*
 * 函 数 名: bsp_deflate_data_send_continue
 * 功能描述: deflate目的端数据上报使能
 * 注    意: 该函数调用时，需要调用者保证同步
 * 输入参数:        b_enable: 中断使能
 * 输出参数: 无
 * 返 回 值: 无
 */
void bsp_deflate_data_send_continue(void)
{
    if (g_deflate_enable_state == COMPRESS_ENABLE_STATE) {
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 2, 1, 1);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 2, 1, 0);

        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 6, 3, 0x07);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 6, 3, 0);
    } else {
        socp_crit("deflate g_socp_enable_state = %d\n", g_deflate_enable_state);
    }
}

static void deflate_global_ctrl_init(void)
{
    spin_lock_init(&g_deflate_ctrl.int_spin_lock);
    osl_sem_init(0, &g_deflate_ctrl.task_sem);

    /* deflate中断标志初始化 */
    g_deflate_ctrl.deflate_int_dst_ovf = 0;
    g_deflate_ctrl.deflate_int_dst_tfr = 0;
    g_deflate_ctrl.deflate_int_dst_thrh_ovf = 0;
    g_deflate_ctrl.deflate_int_dst_work_abort = 0;

    /* deflate目的通道属性初始化 */
    g_deflate_ctrl.set_state = 0;
    g_deflate_ctrl.threshold = 0;
}

static void deflate_global_reset(void)
{
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 1, 1, 1);
    while (DEFLATE_REG_GETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 1, 1) != 0) {
        socp_error("deflate is soft resetting\n");
    }
}

/*
 * 函 数 名: deflate_init
 * 功能描述: deflate初始化函数
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 */
s32 deflate_init(void)
{
    s32 ret;
    struct clk *cDeflate = NULL;
    struct device_node *dev = NULL;
    socp_compress_ops_s ops;
    unsigned long lock_flag;

    ret = bsp_nvm_read(NV_ID_DRV_DEFLATE, (u8 *)&g_deflate_nv_ctrl, sizeof(DRV_DEFLATE_CFG_STRU));
    if (ret) {
        g_deflate_nv_ctrl.deflate_enable = 0;
        deflate_error("deflate read nv fail!\n");
        return ret;
    }
    if (!g_deflate_nv_ctrl.deflate_enable) {
        deflate_crit("deflate_init:deflate is diabled!\n");
        return DEFLATE_OK;
    }

    deflate_crit("[init]start\n");

    if (g_deflate_ctrl.init_flag) {
        deflate_error("deflate init already!\n");
        return DEFLATE_OK;
    }

    /* 解析dts,获取deflate基地址，并映射 */
    dev = of_find_compatible_node(NULL, NULL, "hisilicon,deflate_balong_app");
    if (dev == NULL) {
        deflate_error("deflate dev find failed!\n");
        return DEFLATE_ERROR;
    }
    g_deflate_ctrl.base_addr = (void *)of_iomap(dev, 0);
    if (g_deflate_ctrl.base_addr == NULL) {
        deflate_error("deflate base addr is error!\n");
        return DEFLATE_ERROR;
    }

    deflate_global_ctrl_init(); /* deflate控制块初始化 */

    /* 打开defalte时钟 */
    cDeflate = clk_get(NULL, "clk_socp_deflat");
    if (IS_ERR(cDeflate)) {
        deflate_error("deflate prepare clk fail!\n");
        return DEFLATE_ERROR;
    }
    clk_prepare(cDeflate);
    if (BSP_OK != clk_enable(cDeflate)) {
        deflate_error("deflate clk enable failed!\n");
        return DEFLATE_ERROR;
    }

    deflate_global_reset(); /* deflate全局软复位 */

    /* 创建deflate任务 */
    ret = osl_task_init("deflateProc", DEFFLATE_TASK_PRO, DEFLATE_TASK_STACK_SIZE,
                        (OSL_TASK_FUNC)deflate_task, NULL, &g_deflate_ctrl.taskid);
    if (ret) {
        deflate_error("create task failed!\n");
        return DEFLATE_ERROR;
    }

    /*lint -save -e845*/
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 10, 8, 0x5); /* 5*socp编码包压缩作为触发压缩条件之一 */
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 18, 1, 0);   /* 编码包阈值条件默认不使能 */
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 0, 1, 0);    /* bypass模式不使能 */

    DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_TFRTIMEOUTCFG, DEFLATE_DIRECT_COMPRESS_TIMEOUT); /* 默认超时寄存器设置为立即上报=10ms */
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATEDEST_BUFOVFTIMEOUT, 31, 1, 0);           /* 设置buffer溢出不丢数 */
    DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_IBUFTIMEOUTCFG, DEFLATE_DIRECT_COMPRESS_TIMEOUT); /* ibuf超时配置 */
    DEFLATE_REG_WRITE(SOCP_REG_SOCP_MAX_PKG_BYTE_CFG, 0x2000); /* socp最大包长设置为8k,满足socp源端数据包为4k情况下极限编码场景 */

    spin_lock_irqsave(&g_deflate_ctrl.int_spin_lock, lock_flag);
    /* 清原始中断 */
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 1, 2, 0x03);
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 4, 1, 1);
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 6, 5, 0x1f);
    /* 屏蔽中断 */
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 1, 2, 0x03);
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 4, 1, 1);
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 6, 3, 0x07);
    spin_unlock_irqrestore(&g_deflate_ctrl.int_spin_lock, lock_flag);
    /*lint -restore +e845*/
    ops.isr = deflate_int_handler;
    ops.register_event_cb = deflate_register_event_cb;
    ops.register_read_cb = deflate_register_read_cb;
    ops.enable = deflate_enable;
    ops.disable = deflate_disable;
    ops.set = deflate_set;
    ops.getbuffer = deflate_get_read_buffer;
    ops.readdone = deflate_read_data_done;
    ops.clear = deflate_ctrl_clear;
    deflate_register_compress(&ops);

    /* 设置初始化状态 */
    g_deflate_ctrl.init_flag = BSP_TRUE;
    deflate_crit("[init]ok\n");
    return DEFLATE_OK;
}

s32 deflate_stop(u32 dst_chan_id)
{
    u32 real_chan_id;
    u32 chan_type;

    /* 判断是否已经初始化 */
    if (!g_deflate_ctrl.init_flag) {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }

    /* 判断通道ID是否有效 */
    real_chan_id = DEFLATE_REAL_CHAN_ID(dst_chan_id);
    chan_type = DEFLATE_REAL_CHAN_TYPE(dst_chan_id);
    /* 编码通道 */
    if (chan_type == DEFLATE_CODER_DEST_CHAN) {
        if (real_chan_id < DEFLATE_MAX_ENCDST_CHN) {
            if (g_deflate_ctrl.set_state != DEFLATE_CHN_SET) {
                socp_error("encoder src not allocated!\n");
                return DEFLATE_ERR_INVALID_CHAN;
            }
        } else {
            socp_error("enc dst 0x%x is valid!\n", dst_chan_id);
            return DEFLATE_ERROR;
        }

        /* 停止deflate */
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 21, 1, 0);
        /*lint -restore +e845*/
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 1, 1, 1);
    }
    return DEFLATE_OK;
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
#ifdef CONFIG_DEFLATE
module_init(deflate_init);
#endif
#endif

/*
 * 函 数 名: deflate_compress_enable
 * 功能描述: deflate 压缩使能
 * 输入参数: 压缩目的通道ID
 * 输出参数: 无
 * 返 回 值:压缩成功与否标志
 */
s32 deflate_compress_enable(u32 dst_chan_id)
{
    u32 chan_id = SOCP_REAL_CHAN_ID(dst_chan_id);
    socp_enc_dst_info_s *dst_chan = socp_get_dst_chan_info(chan_id);
    u32 cnt = 500;
    socp_dst_chan_cfg_s attr;

    if (dst_chan->cps_info.bcompress == DEFLATE_IND_COMPRESS) {
        socp_error("socp_compress_enable already!\n");
        return BSP_OK;
    }

    socp_hal_set_global_halt(1); /* SOCP全局halt */
    while (socp_hal_get_src_state() && cnt) {
        msleep(1);
        cnt--;
    }

    /* 读写指针重置，当前数据丢弃 */
    socp_hal_set_dst_read_ptr(chan_id, 0);
    socp_hal_set_dst_write_ptr(chan_id, 0);
    dst_chan->enc_dst_buff.read = 0;
    socp_hal_set_dst_chan_mode(chan_id, 0);

    attr.encdst_thrh = dst_chan->threshold;
    attr.coder_dst_setbuf.output_start = (u8 *)(uintptr_t)(dst_chan->enc_dst_buff.start);
    attr.coder_dst_setbuf.output_end = (u8 *)(uintptr_t)(dst_chan->enc_dst_buff.end);
    attr.coder_dst_setbuf.threshold = dst_chan->buf_thrhold;

    dst_chan->cps_info.ops.set(dst_chan_id, &attr);
    dst_chan->cps_info.ops.register_read_cb(dst_chan->read_cb);
    /* 压缩使能 */
    dst_chan->cps_info.ops.enable(dst_chan_id);
    /* 清非压缩通道原始中断，屏蔽中断状态 */
    bsp_socp_data_send_manager(dst_chan_id, SOCP_DEST_DSM_DISABLE);

    socp_hal_set_global_halt(0); /* SOCP全局使能 */
    dst_chan->cps_info.bcompress = DEFLATE_IND_COMPRESS;

    return BSP_OK;
}
/*
 * 函 数 名: deflate_compress_disable
 * 功能描述: deflate 压缩停止
 * 输入参数: 压缩目的通道ID
 * 输出参数: 无
 * 返 回 值:压缩停止成功与否标志
 */
s32 deflate_compress_disable(u32 dst_chan_id)
{
    u32 chan_id = SOCP_REAL_CHAN_ID(dst_chan_id);
    u32 cnt = 500;
    socp_enc_dst_info_s *dst_chan = socp_get_dst_chan_info(chan_id);

    if (dst_chan->cps_info.bcompress == DEFLATE_IND_NO_COMPRESS) {
        socp_error("already socp compress disabled\n");
        return BSP_ERROR;
    }

    socp_hal_set_global_halt(1); /* SOCP全局halt */

    while (socp_hal_get_src_state() && cnt) {
        msleep(1);
        cnt--;
    }

    dst_chan->cps_info.ops.disable(dst_chan_id);
    dst_chan->cps_info.ops.clear(dst_chan_id);

    bsp_socp_data_send_manager(dst_chan_id, SOCP_DEST_DSM_ENABLE);

    socp_hal_set_dst_chan_mode(chan_id, 1);
    socp_hal_set_global_halt(0); /* SOCP全局使能 */
    
    dst_chan->cps_info.bcompress = DEFLATE_IND_NO_COMPRESS;
    return BSP_OK;
}

/*
 * 函 数 名: deflate_register_compress
 * 功能描述: 压缩函数注册
 * 输入参数: 注册函数结构体
 * 输出参数: 无
 * 返 回 值: 无
 */
void deflate_register_compress(socp_compress_ops_s *ops)
{
    int i;
    socp_int_manager_info_s *socp_int_info = NULL;
    socp_enc_dst_info_s *dst_chan = socp_get_dst_chan_info(0);

    socp_int_info = socp_get_int_info();
    socp_int_info->compress_isr = ops->isr;

    for (i = 0; i < SOCP_MAX_ENCDST_CHN; i++) {
        dst_chan[i].cps_info.ops.register_event_cb = ops->register_event_cb;
        dst_chan[i].cps_info.ops.register_read_cb = ops->register_read_cb;
        dst_chan[i].cps_info.ops.enable = ops->enable;
        dst_chan[i].cps_info.ops.disable = ops->disable;
        dst_chan[i].cps_info.ops.set = ops->set;
        dst_chan[i].cps_info.ops.getbuffer = ops->getbuffer;
        dst_chan[i].cps_info.ops.readdone = ops->readdone;
        dst_chan[i].cps_info.ops.clear = ops->clear;
    }
    
    return;
}

void deflate_set_rsv_buffer_info(unsigned long phy_addr, u8 *virt_addr, u32 buf_len, u32 flag)
{
    g_deflate_dst_buf_log_cfg.phy_addr = phy_addr;
    g_deflate_dst_buf_log_cfg.virt_addr = virt_addr;
    g_deflate_dst_buf_log_cfg.buff_size = buf_len;
    g_deflate_dst_buf_log_cfg.log_on_flag = flag;

    return;
}

u32 deflate_get_mem_log_on_flag(void)
{
    return g_deflate_dst_buf_log_cfg.log_on_flag;
}

u32 deflate_get_init_state(void)
{
    return g_deflate_ctrl.init_flag;
}

/*
 * 函 数 名: deflate_help
 * 功能描述:deflate 打印信息
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 */
void deflate_help(void)
{
    socp_crit("deflate_show_debug_gbl: check global cnt info(src channel,config and interrupt)\n");
    socp_crit("deflate_show_dest_chan_cur: compress dst chan info\n");
}
void deflate_show_debug_gbl(void)
{
    socp_crit(" DEFLATE global status\n");
    socp_crit(" g_deflate_debug.deflate_dst_set_cnt: 0x%x\n", (s32)g_deflate_debug.deflate_dst_set_cnt);
    socp_crit(" g_deflate_debug.deflate_dst_set_suc_cnt : 0x%x\n", (s32)g_deflate_debug.deflate_dst_set_suc_cnt);
    socp_crit(" g_deflate_debug.deflate_reg_readcb_cnt: 0x%x\n", (s32)g_deflate_debug.deflate_reg_readcb_cnt);
    socp_crit(" g_deflate_debug.deflate_reg_eventcb_cnt: 0x%x\n", (s32)g_deflate_debug.deflate_reg_eventcb_cnt);
    socp_crit(" g_deflate_debug.deflate_get_readbuf_etr_cnt: 0x%x\n",
              (s32)g_deflate_debug.deflate_get_readbuf_etr_cnt);
    socp_crit(" g_deflate_debug.deflate_get_readbuf_suc_cnt: 0x%x\n",
              (s32)g_deflate_debug.deflate_get_readbuf_suc_cnt);
    socp_crit(" g_deflate_debug.deflate_readdone_etr_cnt: 0x%x\n", (s32)g_deflate_debug.deflate_readdone_etr_cnt);
    socp_crit(" g_deflate_debug.deflate_readdone_zero_cnt: 0x%x\n",
              (s32)g_deflate_debug.deflate_readdone_zero_cnt);
    socp_crit(" g_deflate_debug.deflate_readdone_valid_cnt: 0x%x\n",
              (s32)g_deflate_debug.deflate_readdone_valid_cnt);
    socp_crit(" g_deflate_debug.deflate_readdone_fail_cnt: 0x%x\n",
              (s32)g_deflate_debug.deflate_readdone_fail_cnt);
    socp_crit(" g_deflate_debug.deflate_readdone_suc_cnt: 0x%x\n", (s32)g_deflate_debug.deflate_readdone_suc_cnt);
    socp_crit(" g_deflate_debug.deflate_task_trf_cb_ori_cnt: 0x%x\n", (s32)g_deflate_debug.deflate_task_trf_cb_ori_cnt);
    socp_crit(" g_deflate_debug.deflate_task_trf_cb_cnt: 0x%x\n", (s32)g_deflate_debug.deflate_task_trf_cb_cnt);
    socp_crit(" g_deflate_debug.deflate_task_ovf_cb_ori_cnt: 0x%x\n", (s32)g_deflate_debug.deflate_task_ovf_cb_ori_cnt);
    socp_crit(" g_deflate_debug.deflate_task_ovf_cb_cnt: 0x%x\n", (s32)g_deflate_debug.deflate_task_ovf_cb_cnt);
    socp_crit(" g_deflate_debug.deflate_task_thrh_ovf_cb_ori_cnt: 0x%x\n",
              (s32)g_deflate_debug.deflate_task_thrh_ovf_cb_ori_cnt);
    socp_crit(" g_deflate_debug.deflate_task_thrh_ovf_cb_cnt: 0x%x\n",
              (s32)g_deflate_debug.deflate_task_thrh_ovf_cb_cnt);
    socp_crit(" g_deflate_debug.deflate_task_int_workabort_cb_ori_cnt: 0x%x\n",
              (s32)g_deflate_debug.deflate_task_int_workabort_cb_ori_cnt);
    socp_crit(" g_deflate_debug.deflate_task_int_workabort_cb_cnt: 0x%x\n",
              (s32)g_deflate_debug.deflate_task_int_workabort_cb_cnt);
}
u32 deflate_show_dest_chan_cur(void)
{
    socp_crit("g_deflate_ctrl.init_flag:0x%x\n", g_deflate_ctrl.init_flag);
    socp_crit("g_deflate_ctrl.chan_id:0x%x\n", g_deflate_ctrl.chan_id);
    socp_crit("g_deflate_ctrl.set_state:%d\n", g_deflate_ctrl.set_state);
    socp_crit("g_deflate_ctrl.deflate_dst_chan.read:0x%x\n", g_deflate_ctrl.deflate_dst_chan.read);
    socp_crit("g_deflate_ctrl.deflate_dst_chan.write:0x%x\n", g_deflate_ctrl.deflate_dst_chan.write);
    socp_crit("g_deflate_ctrl.deflate_dst_chan.length:0x%x\n", g_deflate_ctrl.deflate_dst_chan.length);
    return BSP_OK;
}





