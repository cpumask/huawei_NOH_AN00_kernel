﻿/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#ifndef __NV_PARTITION_UPGRADE_H__
#define __NV_PARTITION_UPGRADE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct _nv_file_map_s {
    u32 magic_num;                     /* file valid flag */ /* 标识文件   0x766e(nv): 有 */
    u32 off;                           /* file offset in flash */
    u32 len;                           /* length of the data in this section */
} nv_file_map_s;
/* ****************************NV非压缩文件描述************************************************* */
typedef struct _xnv_map_file_s {
    nv_file_map_s stXnvFile;  /* xnv.xml文件信息 */
    nv_file_map_s stCustFile; /* cust.xml文件信息 */
    nv_file_map_s stMapFile;  /* xnv.xml map文件信息 */
} xnv_map_file_s;

/* NV升级包包头结构 */
typedef struct _nv_dload_packet_head_s {
    nv_file_map_s nv_bin;       /* nv.bin文件信息 */
    nv_file_map_s xnv_xml[2];   /* 双卡xnv.xml文件信息 */
    nv_file_map_s cust_xml[2];  /* 双卡cust.xml文件信息 */
    nv_file_map_s xnv_map[2];   /* 双卡xnv.xml map文件信息 */
    u32 ulSimNumEx;             /* NV支持的总modem数量减2 */
    xnv_map_file_s xnv_file[0]; /* 除双卡外所支持的卡的信息，ulSimNumEx -2个 */
} nv_dload_packet_head_s;
/* ****************************NV压缩文件描述************************************************* */
#define XNV_SEC_MAX (0x100000)

typedef struct _xnv_sec_info {
    u32 offset;
    u32 length;
    u32 src_length;
    u32 reserved;
} xnv_sec_info;

typedef struct _xnv_sec_data {
    u32 cnt;
    xnv_sec_info map[0];
} xnv_sec_data;

/* CAUTION! section map is variable, can't get cate_cnt/prdt_cnt from member of this struct. */
typedef struct _xnv_prdt_map {
    xnv_sec_data class_map;
    xnv_sec_data cate_map;
    xnv_sec_data prdt_map;
} xnv_prdt_map;

typedef struct _xnv_prdt_info {
    u32 prdt_id;         /* product id */
    u32 map_data_offset; /* 标识product id对应xnv_prdt_info数据偏移 */
    u32 map_data_len;    /* 标识product id对应xnv_prdt_info数据长度 */
} xnv_prdt_info;

/* CAUTION! product info is variable, can't get product map from member of this struct. */
typedef struct _xnv_map_file {
    u32 magic_num;
    u32 prdt_num;
    u8 reserved[16];
    xnv_prdt_info prdt_info[0];
    xnv_prdt_map prdt_map[0];
} xnv_map_file;

/*************************************************************************************/
/*                              XNV BIN文件格式                                      */
/*************************************************************************************/
/*               |--------------------------------------------------------|
                 |magic_num| seclist offset| seclist num| prdt stru offset|
                 |--------------------------------------------------------|
                 |                      section list                      |
                 |--------------------------------------------------------|
                 |                      section data[0]                   |
                 |--------------------------------------------------------|
                 |                      section data[...]                 |
                 |--------------------------------------------------------|
                 |                      section data[n]                   |
                 |--------------------------------------------------------|
                 |                      product_struct                    |
                 |--------------------------------------------------------|
Description:
1. section list也是一个数组，数组的维度从section list... ；
2. cust.map, cust.bin文件格式和xnv格式完全相同；
3. product structure的作用和数据结构不关心;

                 |******************* section list文件结构 ***************|

                 |--------------------------------------------------------|
                 |                      section list[0]                   |
                 |    id     |    offset    |    length     |  src_length |
                 |--------------------------------------------------------|
                 |                      section list[1]                   |
                 |--------------------------------------------------------|
                 |                      section list[...]                 |
                 |--------------------------------------------------------|
                 |                      section list[n]                   |
                 |--------------------------------------------------------|

                 |******************* section data文件结构 ***************|

                 |--------------------------------------------------------|
                 |      file name[64]       |  nv_num   |   reserved[2]   |
                 |--------------------------------------------------------|
                 |                      xnv_item[0]                       |
                 |--------------------------------------------------------|
                 |                      xnv_item[1]                       |
                 |--------------------------------------------------------|
                 |                      xnv_item[...]                     |
                 |--------------------------------------------------------|
                 |                      xnv_item[n]                       |
                 |--------------------------------------------------------|

                 |*******************   xnv item 文件结构  ***************|

                 |--------------------------------------------------------|
                 |    magic_num   |   id     |   length    |  modem_num   |
                 |--------------------------------------------------------|
                 |                      reserved[5]                       |
                 |--------------------------------------------------------|
                 |                      modem_data[0]                     |
                 |    modem_id    | has_priority  |  priority  |  data    |
                 |--------------------------------------------------------|
                 |                      modem_data[1]                     |
                 |--------------------------------------------------------|
                 |                      modem_data[...]                   |
                 |--------------------------------------------------------|
                 |                      modem_data[n]                     |
                 |--------------------------------------------------------|

 */
/*************************************************************************************/
#define XNV_ITEM_MAGIC (0x766e)

typedef struct _xnv_sec_list {
    u32 id;
    u32 offset;
    u32 length;
    u32 src_length;
} xnv_sec_list;

typedef struct _xnv_modem {
    u8 modem_id;
    u8 reserved[3];
    u8 data[0];
} xnv_modem;

typedef struct _xnv_item {
    u16 magic_num;
    u16 id;
    u16 length;
    u8 has_priority;
    u8 priority;
    u8 modem_num;
    u8 reserved[7];
    xnv_modem modem_data[0];
} xnv_item;

#define XNV_FILE_NAME_LEN 64
typedef struct _xnv_sec {
    u8 file_name[XNV_FILE_NAME_LEN];
    u32 nv_num;
    u32 reserved[2];
    xnv_item item[0];
} xnv_sec;

typedef struct _xnv_bin_file {
    u32 magic_num;
    u32 seclist_offset;
    u32 seclist_num;
    u32 sec_offset;
    u32 reserved[12];
    xnv_sec_list seclist[0];
    xnv_sec sec[0];
    u32 prdt_stru[0];
} xnv_bin_file;

/*************************************************************************************/
/*                              NV 升级包格式                                        */
/*************************************************************************************/
/*                         |--------------|
                           |   header     |
                           |--------------|
                           |   ctrl.bin   |
                           |--------------|
                           |   xnv.map    |
                           |--------------|
                           |   xnv.bin    |
                           |--------------|
                           |   cust.map   |
                           |--------------|
                           |   cust.bin   |
                           |--------------|
Description:
1. cust.map, cust.bin文件为可选，不一定存在；
2. cust.map, cust.bin文件格式和xnv格式完全相同；
3.
 */
/*************************************************************************************/
typedef struct _nv_dload_head {
    nv_file_map_s nv_bin;   /* nv.bin文件信息 */
    nv_file_map_s xnv_map;  /* 双卡cust.xml文件信息 */
    nv_file_map_s xnv;      /* 双卡xnv.xml文件信息 */
    nv_file_map_s cust_map; /* 双卡xnv.xml map文件信息 */
    nv_file_map_s cust;     /* 双卡xnv.xml map文件信息 */
    u32 reserved[16];       /* NV支持的总modem数量减2 */
} nv_dload_head;

typedef struct _nv_dload_tail {
    u32 reserve;
    u32 upgrade_flag;
} nv_dload_tail;

typedef struct _nv_dload_file {
    nv_dload_head header;
    xnv_map_file xnv_map;
    xnv_bin_file xnv_bin;
    xnv_map_file cust_map;
    xnv_bin_file cust_bin;
    nv_dload_tail tail;
} nv_dload_file;

u32 nv_upgrade_file_updata(void);
u32 nv_upgrade_set_flag(bool flag);
#ifdef BSP_CONFIG_PHONE_TYPE
u32 nv_set_coldpatch_upgrade_flag(bool flag);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __NV_PARTITION_UPGRADE_H__ */
