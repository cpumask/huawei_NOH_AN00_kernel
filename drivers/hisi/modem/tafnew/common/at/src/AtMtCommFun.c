/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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
#include "AtMtCommFun.h"
#include "securec.h"
#include "taf_drv_agent.h"
#include "AtParse.h"
#include "AtMtMsgProc.h"
#include "ATCmdProc.h"
#include "AtInputProc.h"
#include "mn_comm_api.h"
#include "at_mdrv_interface.h" /* AT_IsMiscSupport */


/*
 * Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
 */
#define THIS_FILE_ID PS_FILE_ID_AT_MT_COMM_FUN_C
#define AT_FWAVE_WAVEFORM_TYPE 0
#define AT_FWAVE_WAVEFORM_POWER 1
#define AT_FWAVE_GSM_TX_SLOT_TYPE 2
#define AT_CMD_MIPIWR_SECONDARY_ADDR 1
#define AT_CMD_MIPIWR_REG_ADDR 2
#define AT_CMD_MIPIWR_REG_DATA 3
#define AT_CMD_MIPIWR_MIPI_CHANNEL 4
#define AT_SSIWR_PARA_REG_ADDR 2
#define AT_SSIWR_PARA_DATA 3
#define AT_SSI_RD_PARA_CHANNEL_NO 1
#define AT_SSI_RD_PARA_FIC_REG 2
#define AT_MIPIRD_SECONDARY_ADDR 2
#define AT_MIPIRD_REG_PARA 3
#define AT_MIPI_READ_PARA_READ_TYPE 0
#define AT_MIPI_READ_PARA_MIPI_ID 1
#define AT_MIPI_READ_PARA_SECONDARY_ID 2
#define AT_MIPI_READ_PARA_REG_ADDR 3
#define AT_MIPI_READ_PARA_SPEED_TYPE 4
#define AT_MIPI_READ_PARA_READ_BIT_MASK 5
#define AT_MIPI_READ_PARA_REAERVED 6
#define AT_PHYMIPIWRITE_WR_TYPE 0
#define AT_PHYMIPIWRITE_MIPI_ID 1
#define AT_PHYMIPIWRITE_SECONDARY_ID 2
#define AT_PHYMIPIWRITE_REG_ADDR 3
#define AT_PHYMIPIWRITE_MIPI_DATA 4
#define AT_PHYMIPIWRITE_RESERVAED 5
#define AT_PDM_CTRL_REG_VALUE 0
#define AT_PDM_CTRL_BIAS_1 1
#define AT_PDM_CTRL_BIAS_2 2
#define AT_PDM_CTRL_BIAS_3 3
#define AT_SSIRD_CHANNEL_NO 1
#define AT_SSIRD_RFIC_REG 2
#define AT_FCHAN_PARA_MODE 0
#define AT_FCHAN_PARA_BAND 1
#define AT_FCHAN_PARA_CHANNEL_NO 2
#define AT_FCHAN_PARA_UL_CHANNEL 3
#define AT_FCHAN_PARA_DL_CHANNEL 4
#define AT_MIPIOPERATE_MIPI_PORT_SEL 1
#define AT_MIPIOPERATE_SECONDARY_ID 2
#define AT_MIPIOPERATE_REG_ADDR 3
#define AT_MIPIOPERATE_BYTE_CNT 4
#define AT_MIPIOPERATE_DATA_BITBYTE0_VALUE 5
#define AT_TSELRF_TX_OR_RX_PATH 0
#define AT_TSELRF_TX_OR_RX 1
#define AT_TSELRF_PARA_TYPE 2
#define AT_TSELRF_PARA_ANT_NUM 3
#define AT_FPA_LEVEL 0
#define AT_DPDT_RAT_MODE 0
#define AT_DPDT_VALUE 1
#define AT_DPDT_WORK_TYPE 2
#define AT_DPDT_QRY_RAT_MODE 0
#define AT_DPDT_QRY_WORK_TYPE 1
#define AT_FPLL_STATUS_TX 0
#define AT_TRXTAS_RAT_MODE 1
#define AT_TRXTAS_MODE 0
#define AT_TRXTAS_CMD 2
#define AT_TRXTAS_DPDT_VALUE 3
#define AT_TRXTAS_QRY_MODE 0
#define AT_TRXTAS_QRY_RAT_MODE 1
#define AT_TFDPDT_QRY_RAT_MODE 0
#define AT_TFDPDT_RAT_MODE 0
#define AT_TFDPDT_DPDT_VALUE 1
#define AT_FCHAN_PARA_NUM 3
#define AT_FWAVE_CMD_ARRAY_LEN 200
#define AT_FWAVE_PARA_NUM 2
#define AT_DPDT_PARA_NUM 2
#define AT_DCXOTEMPCOMP_PARA_NUM 1
#define AT_MIPIWR_PARA_NUM 5
#define AT_SSIWR_PARA_NUM 4
#define AT_SSIRD_PARA_NUM 3
#define AT_MIPIRD_PARA_NUM 4
#define AT_MIPIREAD_PARA_NUM 7
#define AT_PHYMIPIWRITE_PARA_NUM 6
#define AT_PDMCTRL_PARA_NUM 4
#define AT_FCHAN_NR_PARA_NUM 4
#define AT_MIPIOPERATE_PARA_NUM 5
#define AT_TSELRF_PARA_NUM 2
#define AT_DPDTQRY_PARA_NUM 2
#define AT_FTEMPRPT_PARA_NUM 2
#define AT_DPDT_TO_BBIC_PARA_NUM 3
#define AT_TRXTAS_PARA_NUM 3
#define AT_TRXTASQRY_PARA_NUM 2
#define AT_FSERDESRT_PARA_NUM 7
#define AT_TFDPDT_PARA_NUM 2
#define AT_RAT_MODE_GSM_BASE_FREQ 66700
#define AT_RAT_MODE_WCDMA_BASE_FREQ 480000
#define AT_RAT_MODE_CDMA_BASE_FREQ 153600
#define AT_RAT_MODE_LTE_BASE_FREQ 480000
#define AT_RAT_MODE_NR_BASE_FREQ 480000
#define AT_RAT_MODE_DEAFAULT_BASE_FREQ 480000
#define AT_FRBINFO_PARA_NUM        2
#define AT_FRBINFO_PARA_RBNUM      0 /* FRBINFO�ĵ�һ������RBNUM */
#define AT_FRBINFO_PARA_RBSTARTCFG 1 /* FRBINFO�ĵڶ�������RBSTARTCFG */
#define AT_FWAVE_WAVEFORM_TYPE_SUPPORT_NUM 7
#define AT_FWAVE_WAVEFORM_CONTINUES_WAVE 8 /* ���䲨������8Ϊ���� */
#define AT_SERDESTESTASYNC_PARA_MIN_NUM   2
#define AT_SERDESTESTASYNC_PARA_MAX_NUM   22
#define AT_SERDESTESTASYNC_PARA_MODE    0 /* SERDESTESTASYNC�ĵ�һ������MODE */
#define AT_SERDESTESTASYNC_PARA_CMDTYPE 1 /* SERDESTESTASYNC�ĵڶ�������CMDTYPE */
#define AT_SERDESTESTASYNC_PARA_RPTFLG  2 /* SERDESTESTASYNC�ĵ���������RPTFLG */
#define AT_TRXTAS_CMD_SET 1
#define AT_FWAVE_WAVEFORM_BPSK 0 /* ����Ĳ�������ΪBPSK���� */

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

static const AT_BAND_WidthInfo g_bandWidthTable[] = {
    { AT_BAND_WIDTH_200K, BANDWIDTH_200K, AT_BAND_WIDTH_VALUE_200K },
    { AT_BAND_WIDTH_1M2288, BANDWIDTH_1M2288, AT_BAND_WIDTH_VALUE_1M2288 },
    { AT_BAND_WIDTH_1M28, BANDWIDTH_1M28, AT_BAND_WIDTH_VALUE_1M28 },
    { AT_BAND_WIDTH_1M4, BANDWIDTH_1M4, AT_BAND_WIDTH_VALUE_1M4 },
    { AT_BAND_WIDTH_3M, BANDWIDTH_3M, AT_BAND_WIDTH_VALUE_3M },
    { AT_BAND_WIDTH_5M, BANDWIDTH_5M, AT_BAND_WIDTH_VALUE_5M },
    { AT_BAND_WIDTH_10M, BANDWIDTH_10M, AT_BAND_WIDTH_VALUE_10M },
    { AT_BAND_WIDTH_15M, BANDWIDTH_15M, AT_BAND_WIDTH_VALUE_15M },
    { AT_BAND_WIDTH_20M, BANDWIDTH_20M, AT_BAND_WIDTH_VALUE_20M },
    { AT_BAND_WIDTH_25M, BANDWIDTH_25M, AT_BAND_WIDTH_VALUE_25M },
    { AT_BAND_WIDTH_30M, BANDWIDTH_30M, AT_BAND_WIDTH_VALUE_30M },
    { AT_BAND_WIDTH_40M, BANDWIDTH_40M, AT_BAND_WIDTH_VALUE_40M },
    { AT_BAND_WIDTH_50M, BANDWIDTH_50M, AT_BAND_WIDTH_VALUE_50M },
    { AT_BAND_WIDTH_60M, BANDWIDTH_60M, AT_BAND_WIDTH_VALUE_60M },
    { AT_BAND_WIDTH_80M, BANDWIDTH_80M, AT_BAND_WIDTH_VALUE_80M },
    { AT_BAND_WIDTH_90M, BANDWIDTH_90M, AT_BAND_WIDTH_VALUE_90M },
    { AT_BAND_WIDTH_100M, BANDWIDTH_100M, AT_BAND_WIDTH_VALUE_100M },
    { AT_BAND_WIDTH_200M, BANDWIDTH_200M, AT_BAND_WIDTH_VALUE_200M },
    { AT_BAND_WIDTH_400M, BANDWIDTH_400M, AT_BAND_WIDTH_VALUE_400M },
    { AT_BAND_WIDTH_800M, BANDWIDTH_800M, AT_BAND_WIDTH_VALUE_800M },
    { AT_BAND_WIDTH_1G, BANDWIDTH_1G, AT_BAND_WIDTH_VALUE_1G },
};

static const AT_PATH_ToAntType g_path2AntTypeTable[] = {
    { AT_TSELRF_PATH_GSM, AT_ANT_TYPE_PRI },           { AT_TSELRF_PATH_WCDMA_PRI, AT_ANT_TYPE_PRI },
    { AT_TSELRF_PATH_WCDMA_DIV, AT_ANT_TYPE_DIV },     { AT_TSELRF_PATH_CDMA_PRI, AT_ANT_TYPE_PRI },
    { AT_TSELRF_PATH_CDMA_DIV, AT_ANT_TYPE_DIV },      { AT_TSELRF_PATH_FDD_LTE_PRI, AT_ANT_TYPE_PRI },
    { AT_TSELRF_PATH_FDD_LTE_DIV, AT_ANT_TYPE_DIV },   { AT_TSELRF_PATH_FDD_LTE_MIMO, AT_ANT_TYPE_MIMO },
    { AT_TSELRF_PATH_TDD_LTE_PRI, AT_ANT_TYPE_PRI },   { AT_TSELRF_PATH_TDD_LTE_DIV, AT_ANT_TYPE_DIV },
    { AT_TSELRF_PATH_TDD_LTE_MIMO, AT_ANT_TYPE_MIMO }, { AT_TSELRF_PATH_NR_PRI, AT_ANT_TYPE_PRI },
    { AT_TSELRF_PATH_NR_DIV, AT_ANT_TYPE_DIV },        { AT_TSELRF_PATH_NR_MIMO, AT_ANT_TYPE_MIMO },
};


/*
 * Ƶ�ʵļ��㷽��:FREF = FREF-Offs + ��FGlobal (NREF �C NREF-Offs)
 * Frequency     ��FGlobal     FREF-Offs    NREF-Offs     Range of NREF
 * range [MHz]     [kHz]        [MHz]
 * 0 �C 3000        5            0            0            0 �C 599999
 * 3000 �C 24250   15            3000        600000        600000 �C 2016666
 * 24250 �C 100000 60           24250.08      2016667      2016667 �C 3279165
 */

static const AT_NR_FreqOffsetTable g_atNrFreqOffsetTable[] = {
    {{ 0, 3000 * FREQ_UNIT_MHZ_TO_KHZ }, 5, 0, 0, { 0, 599999 }},
    {{ 3000 * FREQ_UNIT_MHZ_TO_KHZ, 24250 * FREQ_UNIT_MHZ_TO_KHZ }, 15, 3000 * FREQ_UNIT_MHZ_TO_KHZ, 600000, { 600000, 2016666 }},
    {{ 24250 * FREQ_UNIT_MHZ_TO_KHZ, 100000 * FREQ_UNIT_MHZ_TO_KHZ }, 60, 24250080, 2016667, { 2016667, 3279165 }},
};


static const AT_NR_BandInfo g_atNrBandInfoTable[] = {
    { 1, AT_DUPLEX_MODE_FDD, { 384000, 396000 }, { 422000, 434000 }, { 1920000, 1980000 }, { 2110000, 2170000 }},
    { 2, AT_DUPLEX_MODE_FDD, { 370000, 382000 }, { 386000, 398000 }, { 1850000, 1910000 }, { 1930000, 1990000 }},
    { 3, AT_DUPLEX_MODE_FDD, { 342000, 357000 }, { 361000, 376000 }, { 1710000, 1785000 }, { 1805000, 1880000 }},
    { 5, AT_DUPLEX_MODE_FDD, { 164800, 169800 }, { 173800, 178800 }, { 824000, 849000 }, { 869000, 894000 }},
    { 7, AT_DUPLEX_MODE_FDD, { 500000, 514000 }, { 524000, 538000 }, { 2500000, 2570000 }, { 2620000, 2690000 }},
    { 8, AT_DUPLEX_MODE_FDD, { 176000, 183000 }, { 185000, 192000 }, { 880000, 915000 }, { 925000, 960000 }},
    { 12, AT_DUPLEX_MODE_FDD, { 139800, 143200 }, { 145800, 149200 }, { 699000, 716000 }, { 729000, 746000 }},
    { 20, AT_DUPLEX_MODE_FDD, { 166400, 172400 }, { 158200, 164200 }, { 832000, 862000 }, { 791000, 821000 }},
    { 25, AT_DUPLEX_MODE_FDD, { 370000, 383000 }, { 386000, 399000 }, { 1850000, 1915000 }, { 1930000, 1995000 }},
    { 28, AT_DUPLEX_MODE_FDD, { 140600, 149600 }, { 151600, 160600 }, { 703000, 748000 }, { 758000, 803000 }},
    { 34, AT_DUPLEX_MODE_TDD, { 402000, 405000 }, { 402000, 405000 }, { 2010000, 2025000 }, { 2010000, 2025000 }},
    { 38, AT_DUPLEX_MODE_TDD, { 514000, 524000 }, { 514000, 524000 }, { 2570000, 2620000 }, { 2570000, 2620000 }},
    { 39, AT_DUPLEX_MODE_TDD, { 376000, 384000 }, { 376000, 384000 }, { 1880000, 1920000 }, { 1880000, 1920000 }},
    { 40, AT_DUPLEX_MODE_TDD, { 460000, 480000 }, { 460000, 480000 }, { 2300000, 2400000 }, { 2300000, 2400000 }},
    { 41, AT_DUPLEX_MODE_TDD, { 499200, 537999 }, { 499200, 537999 }, { 2496000, 2690000 }, { 2496000, 2690000 }},
    { 51, AT_DUPLEX_MODE_TDD, { 285400, 286400 }, { 285400, 286400 }, { 1427000, 1432000 }, { 1427000, 1432000 }},
    /* ������Ƶ�㲻�Գ� */
    { 66, AT_DUPLEX_MODE_FDD, { 342000, 356000 }, { 422000, 440000 }, { 1710000, 1780000 }, { 2110000, 2200000 }},
    /* ������Ƶ�㲻�Գ� */
    { 70, AT_DUPLEX_MODE_FDD, { 339000, 342000 }, { 399000, 404000 }, { 1695000, 1710000 }, { 1995000, 2020000 }},
    { 71, AT_DUPLEX_MODE_FDD, { 132600, 139600 }, { 123400, 130400 }, { 663000, 698000 }, { 617000, 652000 }},
    /* ������Ƶ�㲻�Գ� */
    { 75, AT_DUPLEX_MODE_SDL, { 0, 0 }, { 286400, 303400 }, { 0, 0 }, { 1432000, 1517000 }},
    /* ������Ƶ�㲻�Գ� */
    { 76, AT_DUPLEX_MODE_SDL, { 0, 0 }, { 285400, 286400 }, { 0, 0 }, { 1427000, 1432000 }},
    { 77, AT_DUPLEX_MODE_TDD, { 620000, 680000 }, { 620000, 680000 }, { 3300000, 4200000 }, { 3300000, 4200000 }},
    { 78, AT_DUPLEX_MODE_TDD, { 620000, 653333 }, { 620000, 653333 }, { 3300000, 3800000 }, { 3300000, 3800000 }},
    { 79, AT_DUPLEX_MODE_TDD, { 693334, 733333 }, { 693334, 733333 }, { 4400000, 5000000 }, { 4400000, 5000000 }},
    { 80, AT_DUPLEX_MODE_SUL, { 342000, 357000 }, { 0, 0 }, { 1710000, 1785000 }, { 0, 0 }},
    { 81, AT_DUPLEX_MODE_SUL, { 176000, 183000 }, { 0, 0 }, { 880000, 915000 }, { 0, 0 }},
    { 82, AT_DUPLEX_MODE_SUL, { 166400, 172400 }, { 0, 0 }, { 832000, 862000 }, { 0, 0 }},
    { 83, AT_DUPLEX_MODE_SUL, { 140600, 149600 }, { 0, 0 }, { 703000, 748000 }, { 0, 0 }},
    { 84, AT_DUPLEX_MODE_SUL, { 384000, 396000 }, { 0, 0 }, { 1920000, 1980000 }, { 0, 0 }},
    { 86, AT_DUPLEX_MODE_SUL, { 342000, 356000 }, { 0, 0 }, { 1710000, 1780000 }, { 0, 0 }},
    { 257, AT_DUPLEX_MODE_TDD, { 2054166, 2104165 }, { 2054166, 2104165 }, { 26500000, 29500000 }, { 26500000, 29500000 }},
    { 258, AT_DUPLEX_MODE_TDD, { 2016667, 2070832 }, { 2016667, 2070832 }, { 24250000, 27500000 }, { 24250000, 27500000 }},
    { 260, AT_DUPLEX_MODE_TDD, { 2229166, 2279165 }, { 2229166, 2279165 }, { 37000000, 40000000 }, { 37000000, 40000000 }},
    { 261, AT_DUPLEX_MODE_TDD, { 2070833, 2087497 }, { 2070833, 2087497 }, { 27500000, 28500000 }, { 27500000, 28350000 }},
};


static const AT_LTE_BandInfo g_atLteBandInfoTable[] = {
    { 1, 0, 2110000, 0, { 0, 599 }, 1920000, 18000, { 18000, 18599 }},
    { 2, 0, 1930000, 600, { 600, 1199 }, 1850000, 18600, { 18600, 19199 }},
    { 3, 0, 1805000, 1200, { 1200, 1949 }, 1710000, 19200, { 19200, 19949 }},
    { 4, 0, 2110000, 1950, { 1950, 2399 }, 1710000, 19950, { 19950, 20399 }},
    { 5, 0, 869000, 2400, { 2400, 2649 }, 824000, 20400, { 20400, 20649 }},
    { 6, 0, 875000, 2650, { 2650, 2749 }, 830000, 20650, { 20650, 20749 }},
    { 7, 0, 2620000, 2750, { 2750, 3449 }, 2500000, 20750, { 20750, 21449 }},
    { 8, 0, 925000, 3450, { 3450, 3799 }, 880000, 21450, { 21450, 21799 }},
    { 9, 0, 1844900, 3800, { 3800, 4149 }, 1749900, 21800, { 21800, 22149 }},
    { 10, 0, 2110000, 4150, { 4150, 4749 }, 1710000, 22150, { 22150, 22749 }},
    { 11, 0, 1475900, 4750, { 4750, 4949 }, 1427900, 22750, { 22750, 22949 }},
    { 12, 0, 729000, 5010, { 5010, 5179 }, 699000, 23010, { 23010, 23179 }},
    { 13, 0, 746000, 5180, { 5180, 5279 }, 777000, 23180, { 23180, 23279 }},
    { 14, 0, 758000, 5280, { 5280, 5379 }, 788000, 23280, { 23280, 23379 }},
    { 17, 0, 734000, 5730, { 5730, 5849 }, 704000, 23730, { 23730, 23849 }},
    { 18, 0, 860000, 5850, { 5850, 5999 }, 815000, 23850, { 23850, 23999 }},
    { 19, 0, 875000, 6000, { 6000, 6149 }, 830000, 24000, { 24000, 24149 }},
    { 20, 0, 791000, 6150, { 6150, 6449 }, 832000, 24150, { 24150, 24449 }},
    { 21, 0, 1495900, 6450, { 6450, 6599 }, 1447900, 24450, { 24450, 24599 }},
    { 22, 0, 3510000, 6600, { 6600, 7399 }, 3410000, 24600, { 24600, 25399 }},
    { 23, 0, 2180000, 7500, { 7500, 7699 }, 2000000, 25500, { 25500, 25699 }},
    { 24, 0, 1525000, 7700, { 7700, 8039 }, 1626500, 25700, { 25700, 26039 }},
    { 25, 0, 1930000, 8040, { 8040, 8689 }, 1850000, 26040, { 26040, 26689 }},
    { 26, 0, 859000, 8690, { 8690, 9039 }, 814000, 26690, { 26690, 27039 }},
    { 27, 0, 852000, 9040, { 9040, 9209 }, 807000, 27040, { 27040, 27209 }},
    { 28, 0, 758000, 9210, { 9210, 9659 }, 703000, 27210, { 27210, 27659 }},
    { 29, 0, 717000, 9660, { 9660, 9769 }, 0, 0, { 0, 0 }},
    { 30, 0, 2350000, 9770, { 9770, 9869 }, 2305000, 27660, { 27660, 27759 }},
    { 31, 0, 462500, 9870, { 9870, 9919 }, 452500, 27760, { 27760, 27809 }},
    { 32, 0, 1452000, 9920, { 9920, 10359 }, 0, 0, { 0, 0, }},
    { 33, 0, 1900000, 36000, { 36000, 36199 }, 1900000, 36000, { 36000, 36199 }},
    { 34, 0, 2010000, 36200, { 36200, 36349 }, 2010000, 36200, { 36200, 36349 }},
    { 35, 0, 1850000, 36350, { 36350, 36949 }, 1850000, 36350, { 36350, 36949 }},
    { 36, 0, 1930000, 36950, { 36950, 37549 }, 1930000, 36950, { 36950, 37549 }},
    { 37, 0, 1910000, 37550, { 37550, 37749 }, 1910000, 37550, { 37550, 37749 }},
    { 38, 0, 2570000, 37750, { 37750, 38249 }, 2570000, 37750, { 37750, 38249 }},
    { 39, 0, 1880000, 38250, { 38250, 38649 }, 1880000, 38250, { 38250, 38649 }},
    { 40, 0, 2300000, 38650, { 38650, 39649 }, 2300000, 38650, { 38650, 39649 }},
    { 41, 0, 2496000, 39650, { 39650, 41589 }, 2496000, 39650, { 39650, 41589 }},
    { 42, 0, 3400000, 41590, { 41590, 43589 }, 3400000, 41590, { 41590, 43589 }},
    { 43, 0, 3600000, 43590, { 43590, 45589 }, 3600000, 43590, { 43590, 45589 }},
    { 44, 0, 703000, 45590, { 45590, 46589 }, 703000, 45590, { 45590, 46589 }},
    { 45, 0, 1447000, 46590, { 46590, 46789 }, 1447000, 46590, { 46590, 46789 }},
    { 46, 0, 5150000, 46790, { 46790, 54539 }, 5150000, 46790, { 46790, 54539 }},
    { 47, 0, 5855000, 54540, { 54540, 55239 }, 5855000, 54540, { 54540, 55239 }},
    { 48, 0, 3550000, 55240, { 55240, 56739 }, 3550000, 55240, { 55240, 56739 }},
    { 49, 0, 3550000, 56740, { 56740, 58239 }, 3550000, 56740, { 56740, 58239 }},
    { 50, 0, 1432000, 58240, { 58240, 59089 }, 1432000, 58240, { 58240, 59089 }},
    { 51, 0, 1427000, 59090, { 59090, 59139 }, 1427000, 59090, { 59090, 59139 }},
    { 52, 0, 3300000, 59140, { 59140, 60139 }, 3300000, 59140, { 59140, 60139 }},
    { 65, 0, 2110000, 65536, { 65536, 66435 }, 1920000, 131072, { 131072, 131971 }},
    /* ���band���������ŵ��ǲ��ԳƵ� */
    { 66, 0, 2110000, 66436, { 66436, 67335 }, 1710000, 131972, { 131972, 132671 }},
    { 67, 0, 738000, 67336, { 67336, 67535 }, 0, 0, { 0, 0 }},
    { 68, 0, 753000, 67536, { 67536, 67835 }, 698000, 132672, { 132672, 132971 }},
    { 69, 0, 2570000, 67836, { 67836, 68335 }, 0, 0, { 0, 0 }},
    /* ���band���������ŵ��ǲ��ԳƵ� */
    { 70, 0, 1995000, 68336, { 68336, 68585 }, 1695000, 132972, { 132972, 133121 }},
    { 71, 0, 617000, 68586, { 68586, 68935 }, 663000, 133122, { 133122, 133471 }},
    { 72, 0, 461000, 68936, { 68936, 68985 }, 451000, 133472, { 133472, 133521 }},
    { 73, 0, 460000, 68986, { 68986, 69035 }, 450000, 133522, { 133522, 133571 }},
    { 74, 0, 1475000, 69036, { 69036, 69465 }, 1427000, 133572, { 133572, 134001 }},
    { 75, 0, 1432000, 69466, { 69466, 70315 }, 0, 0, { 0, 0 }},
    { 76, 0, 1427000, 70316, { 70316, 70365 }, 0, 0, { 0, 0 }},
    { 85, 0, 728000, 70366, { 70366, 70545 }, 698000, 134002, { 134002, 134181 }},
    { 128, 0, 780500, 9435, { 9435, 9659 }, 725500, 27435, { 27435, 27659 }},
    { 140, 0, 2300000, 38650, { 38650, 39250 }, 2300000, 38650, { 38650, 39250 }},
};


static const AT_W_BandInfo g_atWBandInfoTable[] = {
    { 1, 0, 0, { 9612, 9888 }, { 1922400, 1977600 }, 0, { 10562, 10838 }, { 2112400, 2167600 }},
    { 2, 0, 0, { 9262, 9538 }, { 1852400, 1907600 }, 0, { 9662, 9938 }, { 1932400, 1987600 }},
    { 3, 0, 1525000, { 937, 1288 }, { 1712400, 1782600 }, 1575000, { 1162, 1513 }, { 1807400, 1877600 }},
    { 4, 0, 1450000, { 1312, 1513 }, { 1712400, 1752600 }, 1805000, { 1537, 1738 }, { 2112400, 2152600 }},
    { 5, 0, 0, { 4132, 4233 }, { 826400, 846600 }, 0, { 4357, 4458 }, { 871400, 891600 }},
    { 6, 0, 0, { 4162, 4188 }, { 832400, 837600 }, 0, { 4387, 4413 }, { 877400, 882600 }},
    { 7, 0, 2100000, { 2012, 2338 }, { 2502400, 2567600 }, 2175000, { 2237, 2563 }, { 2622400, 2687600 }},
    { 8, 0, 340000, { 2712, 2863 }, { 882400, 912600 }, 340000, { 2937, 3088 }, { 927400, 957600 }},
    { 9, 0, 0, { 8762, 8912 }, { 1752400, 1782400 }, 0, { 9237, 9387 }, { 1847400, 1877400 }},
    { 11, 0, 733000, { 3487, 3562 }, { 1430400, 1445400 }, 736000, { 3712, 3787 }, { 1478400, 1493400 }},
    { 19, 0, 770000, { 312, 363 }, { 832400, 842600 }, 735000, { 712, 763 }, { 877400, 887600 }},
};


static const AT_G_BandInfo g_atGBandInfoTable[] = {
    { 2, 3, 80000, { 512, 810 }}, /* GSM1900 */
    { 3, 2, 95000, { 512, 885 }}, /* GSM1800 */
    { 5, 0, 45000, { 128, 251 }}, /* GSM850 */
    { 8, 1, 45000, { 0, 1023 }},  /* GSM900 */
};
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)

VOS_UINT32 At_CheckSupportFChanRat(VOS_VOID)
{
    if ((g_atParaList[0].paraValue != AT_RAT_MODE_GSM) && (g_atParaList[0].paraValue != AT_RAT_MODE_EDGE) &&
        (g_atParaList[0].paraValue != AT_RAT_MODE_WCDMA) && (g_atParaList[0].paraValue != AT_RAT_MODE_AWS)
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
        && (g_atParaList[0].paraValue != AT_RAT_MODE_CDMA)
#endif
    ) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 At_CheckWifiFChanPara(VOS_VOID)
{
    if (((g_atParaList[0].paraValue == AT_RAT_MODE_WIFI) && (g_atParaList[1].paraValue != AT_BAND_WIFI)) ||
        ((g_atParaList[1].paraValue == AT_BAND_WIFI) && (g_atParaList[0].paraValue != AT_RAT_MODE_WIFI))) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 At_SetFChanPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl    *atDevCmdCtrl = VOS_NULL_PTR;
    DRV_AGENT_FchanSetReq fchanSetReq;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* ���� LTE ģ�Ľӿڷ�֧ */
#if (FEATURE_LTE == FEATURE_ON)

    if ((g_atParaList[0].paraValue == AT_RAT_MODE_FDD_LTE) ||
#if (FEATURE_LTEV == FEATURE_ON)
        (g_atParaList[0].paraValue == AT_RAT_MODE_LTEV) ||
#endif
        (g_atParaList[0].paraValue == AT_RAT_MODE_TDD_LTE)) {

        atDevCmdCtrl->deviceRatMode = (AT_DEVICE_CmdRatModeUint8)(g_atParaList[0].paraValue);

        return atSetFCHANPara(indexNum);
    }

#endif

#if (FEATURE_UE_MODE_TDS == FEATURE_ON)
    if (g_atParaList[0].paraValue == AT_RAT_MODE_TDSCDMA) {
        atDevCmdCtrl->deviceRatMode = (AT_DEVICE_CmdRatModeUint8)(g_atParaList[0].paraValue);
        return atSetFCHANPara(indexNum);
    }
#endif

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_FCHAN_OTHER_ERR;
    }
    /* ����������Ҫ�� */
    if (g_atParaIndex != AT_FCHAN_PARA_NUM) {
        return AT_FCHAN_OTHER_ERR;
    }

    /* ���WIFIģʽ */
    /* WIFI�ĵ�һ����������Ϊ8���ڶ�����������Ϊ15 */
    if (At_CheckWifiFChanPara() == VOS_FALSE) {
        return AT_CHANNEL_NOT_SET;
    }

    /* WIFI ��֧ */
    if (g_atParaList[0].paraValue == AT_RAT_MODE_WIFI) {
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
        if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_SUPPORT) {
            /* WIFIδEnableֱ�ӷ���ʧ�� */
            if ((VOS_UINT32)AT_GetWifiStatus() == VOS_FALSE) {
                return AT_FCHAN_OTHER_ERR;
            }

            atDevCmdCtrl->deviceRatMode = AT_RAT_MODE_WIFI;

            return AT_OK;
        }
#else
        return AT_ERROR;
#endif
    }

    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ���FCHAN �Ľ���ģʽ�Ƿ�֧�� */
    if (At_CheckSupportFChanRat() == VOS_FALSE) {
        return AT_DEVICE_MODE_ERROR;
    }

    if (g_atParaList[1].paraValue >= AT_BAND_BUTT) {
        return AT_FCHAN_BAND_NOT_MATCH;
    }

    (VOS_VOID)memset_s(&fchanSetReq, sizeof(fchanSetReq), 0x00, sizeof(DRV_AGENT_FchanSetReq));

    fchanSetReq.loadDspMode    = At_GetDspLoadMode(g_atParaList[0].paraValue);
    fchanSetReq.currentDspMode = At_GetDspLoadMode(atDevCmdCtrl->deviceRatMode); /* ��ǰ����ģʽ */
    fchanSetReq.dspLoadFlag    = atDevCmdCtrl->dspLoadFlag;
    fchanSetReq.deviceRatMode  = (VOS_UINT8)g_atParaList[0].paraValue;
    fchanSetReq.deviceAtBand   = (VOS_UINT8)g_atParaList[1].paraValue;
    fchanSetReq.channelNo      = (VOS_UINT16)g_atParaList[AT_FCHAN_CHANNEL_NO].paraValue;

    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, DRV_AGENT_FCHAN_SET_REQ,
                               (VOS_UINT8 *)&fchanSetReq, sizeof(fchanSetReq), I0_WUEPS_PID_DRV_AGENT) == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FCHAN_SET; /* ���õ�ǰ����ģʽ */
        return AT_WAIT_ASYNC_RETURN;                              /* �ȴ��첽�¼����� */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetFwavePara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) ||
#if (FEATURE_LTEV == FEATURE_ON)
        (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_LTEV) ||
#endif
        (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return atSetFWAVEPara(indexNum);
    }
#endif

#if (FEATURE_UE_MODE_TDS == FEATURE_ON)
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDSCDMA) {
        return atSetFWAVEPara(indexNum);
    }
#endif

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    /* ������������ȷ����������������ͺͲ��ι��� */
    if (g_atParaIndex != AT_FWAVE_PARA_NUM) {
        return AT_ERROR;
    }

    /* Ŀǰ��������ֻ֧�����õ��� */
    if (g_atParaList[AT_FWAVE_WAVEFORM_TYPE].paraValue > AT_FWAVE_WAVEFORM_TYPE_SUPPORT_NUM) {
        return AT_ERROR;
    }

    /* ���������ڷ�����ģʽ��ʹ�� */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_ERROR;
    }

    /* WIFI */
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_WIFI) {
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
        return AT_SetWifiFwavePara();
#else
        return AT_ERROR;
#endif
    }

    /*
     * �����ñ����ڱ��ر���
     * AT^FDAC���õ�FDACֵ��AT^FWAVE���õ�powerֵ��ʾ�ĺ�����ͬ��ȡ�����õ�ֵ
     * ����ֵ��0.01Ϊ��λ������DSP��ֵ���10������AT����Ҫ����ֵ�ٳ�10
     */
    atDevCmdCtrl->power     = (VOS_UINT16)(g_atParaList[AT_FWAVE_WAVEFORM_POWER].paraValue / 10);
    atDevCmdCtrl->powerFlag = VOS_TRUE;
    atDevCmdCtrl->fdacFlag  = VOS_FALSE;
    /* ��¼��type��Ϣ����ת��ΪG�����ʹ�õ�TxMode����������㷢��ID_AT_GHPA_RF_TX_CFG_REQʱЯ�� */
    if (g_atParaList[AT_FWAVE_WAVEFORM_TYPE].paraValue == AT_FWAVE_WAVEFORM_BPSK) {
        atDevCmdCtrl->txMode = AT_FWAVE_WAVEFORM_CONTINUES_WAVE;
    } else {
        atDevCmdCtrl->txMode = (VOS_UINT16)g_atParaList[AT_FWAVE_WAVEFORM_TYPE].paraValue;
    }

    /* WCDMA */
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_WCDMA) {
        /* ��WDSP���Ϳ��ص����źŵ�ԭ������ */
        if (At_SendContinuesWaveOnToHPA(atDevCmdCtrl->power, indexNum) == AT_FAILURE) {
            return AT_ERROR;
        }

        /* ���õ�ǰ�������� */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SET_FWAVE;
        atDevCmdCtrl->index                   = indexNum;

        return AT_WAIT_ASYNC_RETURN; /* ������������״̬ */
    }

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_CDMA) {
        /* ��X DSP���Ϳ��ص����źŵ�ԭ������ */
        if (At_SendContinuesWaveOnToCHPA(WDSP_CTRL_TX_ONE_TONE, atDevCmdCtrl->power) == AT_FAILURE) {
            return AT_ERROR;
        }

        /* ���õ�ǰ�������� */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SET_FWAVE;
        atDevCmdCtrl->index                   = indexNum;

        return AT_WAIT_ASYNC_RETURN; /* ������������״̬ */
    }
#endif

    return AT_OK;
}


VOS_UINT32 At_CheckFTxonPara(VOS_UINT8 switchStatus)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    if (switchStatus > AT_DSP_RF_SWITCH_ON) {
        return AT_FTXON_OTHER_ERR;
    }

    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    if (atDevCmdCtrl->dspLoadFlag == VOS_FALSE) {
        return AT_CHANNEL_NOT_SET;
    }

    return AT_OK;
}


VOS_UINT32 At_SetFTxonPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    TAF_UINT8          switchStatus;
    VOS_UINT32         result;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* ��� LTE ģ�Ľӿڷ�֧ */
#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) ||
#if (FEATURE_LTEV == FEATURE_ON)
        (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_LTEV) ||
#endif
        (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return atSetFTXONPara(indexNum);
    }
#endif

#if (FEATURE_UE_MODE_TDS == FEATURE_ON)
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDSCDMA) {
        return atSetFTXONPara(indexNum);
    }

#endif
    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_FTXON_OTHER_ERR;
    }
    /* ����������Ҫ�� */
    if (g_atParaIndex != 1) {
        return AT_FTXON_OTHER_ERR;
    }

    switchStatus                  = (VOS_UINT8)g_atParaList[0].paraValue;
    atDevCmdCtrl->tempRxorTxOnOff = switchStatus;
    result                        = At_CheckFTxonPara(switchStatus);
    if (result != AT_OK) {
        return result;
    }

    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_WCDMA) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_AWS)) {
        if (At_SendTxOnOffToHPA(switchStatus, indexNum) == AT_FAILURE) {
            return AT_FTXON_SET_FAIL;
        }
    }
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    else if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_CDMA) {
        if (At_SendTxOnOffToCHPA(switchStatus) == AT_FAILURE) {
            return AT_FTXON_SET_FAIL;
        }
    }
#endif
    else {
        if (At_SendTxOnOffToGHPA(indexNum, switchStatus) == AT_FAILURE) {
            return AT_FTXON_SET_FAIL;
        }
    }
    /* ���õ�ǰ�������� */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SET_FTXON;
    atDevCmdCtrl->index                   = indexNum;

    return AT_WAIT_ASYNC_RETURN; /* ������������״̬ */
}


VOS_UINT32 At_SetFRxonPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         switchStatus;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* ���� LTE ģ�Ľӿڷ�֧ */
#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) ||
#if (FEATURE_LTEV == FEATURE_ON)
        (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_LTEV) ||
#endif
        (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return atSetFRXONPara(indexNum);
    }
#endif

#if (FEATURE_UE_MODE_TDS == FEATURE_ON)
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDSCDMA) {
        return atSetFRXONPara(indexNum);
    }
#endif

    /* ����״̬��� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_FRXON_OTHER_ERR;
    }

    /* ����������Ҫ�� */
    if (g_atParaIndex != 1) {
        return AT_FRXON_OTHER_ERR;
    }

    /* ��AT������AT^TMODE=1������ģʽ��ʹ�� */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ��AT������Ҫ��AT^FCHAN���÷������ŵ�����ִ�гɹ���ʹ�� */
    if (atDevCmdCtrl->dspLoadFlag == VOS_FALSE) {
        return AT_CHANNEL_NOT_SET;
    }

    switchStatus                  = g_atParaList[0].paraValue;
    atDevCmdCtrl->tempRxorTxOnOff = (AT_DSP_RfOnOffUint8)switchStatus;

    /* �ѿ��ؽ��ջ����󷢸�W����� */
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_WCDMA) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_AWS)) {
        if (At_SendRxOnOffToHPA(switchStatus, indexNum) == AT_FAILURE) {
            return AT_FRXON_SET_FAIL;
        }
    }
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    else if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_CDMA) {
        if (At_SendRxOnOffToCHPA(switchStatus) == AT_FAILURE) {
            return AT_FRXON_SET_FAIL;
        }
    }
#endif
    else {
        /* �ѿ��ؽ��ջ����󷢸�G����� */
        if (At_SendRxOnOffToGHPA(indexNum, switchStatus) == AT_FAILURE) {
            return AT_FRXON_SET_FAIL;
        }
    }

    /* ���õ�ǰ�������� */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SET_FRXON;
    atDevCmdCtrl->index                   = indexNum;

    /* ������������״̬ */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_ProcTSelRfWDivPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    if (atDevCmdCtrl->rxOnOff != DRV_AGENT_DSP_RF_SWITCH_ON) {
        atDevCmdCtrl->priOrDiv = AT_RX_DIV_ON;
        return AT_OK;
    }
    if (At_SendRfCfgAntSelToHPA(AT_RX_DIV_ON, indexNum) == AT_FAILURE) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_TSELRF_SET;
    atDevCmdCtrl->index                   = indexNum;

    /* ������������״̬ */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_ProcTSelRfWPriPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    if (atDevCmdCtrl->rxOnOff != DRV_AGENT_DSP_RF_SWITCH_ON) {
        atDevCmdCtrl->priOrDiv = AT_RX_PRI_ON;
        return AT_OK;
    }

    if (At_SendRfCfgAntSelToHPA(AT_RX_PRI_ON, indexNum) == AT_FAILURE) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_TSELRF_SET;
    atDevCmdCtrl->index                   = indexNum;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetTSelRfPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl     *atDevCmdCtrl = VOS_NULL_PTR;
    DRV_AGENT_TselrfSetReq tseLrf;
    VOS_BOOL               bLoadDsp;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    /* ����������Ҫ�� */
    if ((g_atParaIndex > AT_TSELRF_PARA_MAX_NUM) || (g_atParaList[AT_TSELRF_PATH_ID].paraLen == 0)) {
        return AT_ERROR;
    }

    if (g_atParaList[AT_TSELRF_PATH_ID].paraValue == AT_TSELRF_PATH_WIFI) {
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
        return AT_ProcTSelRfWifiPara();
#else
        return AT_ERROR;
#endif
    }

#if (FEATURE_UE_MODE_TDS == FEATURE_ON)
    if (g_atParaList[AT_TSELRF_PATH_ID].paraValue == AT_TSELRF_PATH_TD) {
        return atSetTselrfPara(indexNum);
    }

#endif

#if (FEATURE_LTE == FEATURE_ON)
    if ((g_atParaList[AT_TSELRF_PATH_ID].paraValue != AT_TSELRF_PATH_WCDMA_PRI) &&
        (g_atParaList[AT_TSELRF_PATH_ID].paraValue != AT_TSELRF_PATH_WCDMA_DIV) &&
        (g_atParaList[AT_TSELRF_PATH_ID].paraValue != AT_TSELRF_PATH_GSM)) {
        return atSetTselrfPara(indexNum);
    }
#endif

    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_ERROR;
    }

    /* �򿪷ּ�������FRXON֮�󣬲ο�RXDIVʵ�� */
    if (g_atParaList[AT_TSELRF_PATH_ID].paraValue == AT_TSELRF_PATH_WCDMA_DIV) {
        return AT_ProcTSelRfWDivPara(indexNum);
    }

    if (g_atParaList[AT_TSELRF_PATH_ID].paraValue == AT_TSELRF_PATH_WCDMA_PRI) {
        return AT_ProcTSelRfWPriPara(indexNum);
    }

    /* �˴��ж��Ƿ���Ҫ���¼���DSP: ��Ҫ������C�˼���DSP������ֱ�ӷ���OK */
    bLoadDsp = AT_GetTseLrfLoadDspInfo(g_atParaList[AT_TSELRF_PATH_ID].paraValue, &tseLrf);
    if (bLoadDsp == VOS_TRUE) {
        if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                   DRV_AGENT_TSELRF_SET_REQ, (VOS_UINT8 *)&tseLrf, sizeof(tseLrf),
                                   I0_WUEPS_PID_DRV_AGENT) == TAF_SUCCESS) {
            g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_TSELRF_SET; /* ���õ�ǰ����ģʽ */
            return AT_WAIT_ASYNC_RETURN;                               /* �ȴ��첽�¼����� */
        } else {
            return AT_ERROR;
        }
    }

    return AT_OK;
}


VOS_UINT32 At_SetFpaPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* ����LTE ģ�Ľӿڷ�֧ */
#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return AT_CMD_NOT_SUPPORT;
    }
#endif
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDSCDMA) {
        return AT_CMD_NOT_SUPPORT;
    }
    /* ����״̬��� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_FPA_OTHER_ERR;
    }

    /* �������� */
    if (g_atParaIndex > 1) {
        return AT_FPA_OTHER_ERR;
    }

    /* ���������ڷ�����ģʽ��ʹ�� */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* �������������÷������ŵ���ʹ��,��^FCHAN�ɹ�ִ�к� */
    if (atDevCmdCtrl->dspLoadFlag == VOS_FALSE) {
        return AT_CHANNEL_NOT_SET;
    }

    /* �����ñ����ڱ��ر��� */
    atDevCmdCtrl->paLevel = (VOS_UINT8)g_atParaList[0].paraValue;

    return AT_OK;
}


VOS_UINT32 AT_SetFAgcgainPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* ���� LTE ģ�Ľӿڷ�֧ */
#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) ||
#if (FEATURE_LTEV == FEATURE_ON)
        (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_LTEV) ||
#endif
        (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return atSetFLNAPara(indexNum);
    }
#endif

#if (FEATURE_UE_MODE_TDS == FEATURE_ON)
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDSCDMA) {
        return atSetFLNAPara(indexNum);
    }
#endif

    /* ����״̬��� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_FLNA_OTHER_ERR;
    }

    /* �������� */
    if (g_atParaIndex > 1) {
        return AT_FLNA_OTHER_ERR;
    }

    /* ���������ڷ�����ģʽ��ʹ�� */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* �������������÷������ŵ���ʹ�� */
    if (atDevCmdCtrl->dspLoadFlag == VOS_FALSE) {
        return AT_CHANNEL_NOT_SET;
    }

    /* ����LNA�ȼ�ȡֵ��Χ��� */
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_WCDMA) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_AWS)) {
        /* WDSP LNA�ȼ�ȡֵΪ0-2 */
        if (g_atParaList[0].paraValue > DSP_LNA_HIGH_GAIN_MODE) {
            return AT_FLNA_OTHER_ERR;
        }
    }

    /* �����ñ����ڱ��ر��� */
    atDevCmdCtrl->lnaLevel = (VOS_UINT8)g_atParaList[0].paraValue;

    return AT_OK;
}


VOS_UINT32 At_SetDpdtPara(VOS_UINT8 indexNum)
{
    AT_MTA_SetDpdtValueReq atCmd;
    VOS_UINT32             rst;

    /* ������� */
    if (g_atParaIndex != AT_DPDT_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* AT������MTA����Ϣ�ṹ��ֵ */
    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(AT_MTA_SetDpdtValueReq));
    atCmd.ratMode   = (AT_MTA_CmdRatmodeUint8)g_atParaList[0].paraValue;
    atCmd.dpdtValue = g_atParaList[1].paraValue;

    /* ������Ϣ��C�˴��� */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_SET_DPDT_VALUE_REQ, (VOS_UINT8 *)&atCmd,
                                 sizeof(AT_MTA_SetDpdtValueReq), I0_UEPS_PID_MTA);

    if (rst == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DPDT_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_SetQryDpdtPara(VOS_UINT8 indexNum)
{
    AT_MTA_QryDpdtValueReq atCmd;
    VOS_UINT32             rst;

    /* ������� */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* AT������MTA����Ϣ�ṹ��ֵ */
    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(AT_MTA_QryDpdtValueReq));
    atCmd.ratMode = (AT_MTA_CmdRatmodeUint8)g_atParaList[0].paraValue;

    /* ������Ϣ��C�˴��� */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_QRY_DPDT_VALUE_REQ, (VOS_UINT8 *)&atCmd,
                                 sizeof(AT_MTA_QryDpdtValueReq), I0_UEPS_PID_MTA);

    if (rst == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DPDTQRY_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

/*
 * ��������: ^DCXOTEMPCOMP�����������
 */
VOS_UINT32 AT_SetDcxotempcompPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����������� */
    if (g_atParaIndex != AT_DCXOTEMPCOMP_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    /*  ���Ƿ�����ģʽ�·����ش��� */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    atDevCmdCtrl->dcxoTempCompEnableFlg = (AT_DCXOTEMPCOMP_EnableUint8)g_atParaList[0].paraValue;

    return AT_OK;
}


VOS_UINT32 AT_SetFDac(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    TAF_UINT16         dac;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* ���� LTE ģ�Ľӿڷ�֧ */
#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return AT_CMD_NOT_SUPPORT;
    }
#endif
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDSCDMA) {
        return AT_CMD_NOT_SUPPORT;
    }
    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_FDAC_SET_FAIL;
    }
    /* ����������Ҫ�� */
    if (g_atParaIndex != 1) {
        return AT_FDAC_SET_FAIL;
    }
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    if (atDevCmdCtrl->dspLoadFlag == VOS_FALSE) {
        return AT_CHANNEL_NOT_SET;
    }

    dac = (VOS_UINT16)g_atParaList[0].paraValue;

    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_WCDMA) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_AWS)) {
        if (dac > WDSP_MAX_TX_AGC) {
            return AT_FDAC_SET_FAIL;
        } else {
            atDevCmdCtrl->fdac = (VOS_UINT16)g_atParaList[0].paraValue;
        }
    } else {
        if (dac > GDSP_MAX_TX_VPA) {
            return AT_FDAC_SET_FAIL;
        } else {
            atDevCmdCtrl->fdac = (VOS_UINT16)g_atParaList[0].paraValue;
        }
    }

    /* AT^FDAC���õ�FDACֵ��AT^FWAVE���õ�powerֵ��ʾ�ĺ�����ͬ��ȡ�����õ�ֵ */
    atDevCmdCtrl->fdacFlag  = VOS_TRUE;
    atDevCmdCtrl->powerFlag = VOS_FALSE;

    return AT_OK; /* ������������״̬ */
}


VOS_UINT32 AT_SetMipiWrPara(VOS_UINT8 indexNum)
{
    AT_HPA_MipiWrReq  *msg          = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* �������� */
    if (g_atParaIndex != AT_MIPIWR_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    length = sizeof(AT_HPA_MipiWrReq) - VOS_MSG_HEAD_LENGTH;
    msg    = (AT_HPA_MipiWrReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_SetMipiWrPara: alloc msg fail!");
        return AT_ERROR;
    }

    if (g_atParaList[0].paraValue == AT_RAT_MODE_GSM) {
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_GPHY), length);
    } else {
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_WPHY), length);
    }

    msg->msgId         = ID_AT_HPA_MIPI_WR_REQ;
    msg->secondaryAddr = (VOS_UINT16)g_atParaList[AT_CMD_MIPIWR_SECONDARY_ADDR].paraValue;
    msg->regAddr       = (VOS_UINT16)g_atParaList[AT_CMD_MIPIWR_REG_ADDR].paraValue;
    msg->regData       = (VOS_UINT16)g_atParaList[AT_CMD_MIPIWR_REG_DATA].paraValue;
    msg->mipiChannel   = (VOS_UINT16)g_atParaList[AT_CMD_MIPIWR_MIPI_CHANNEL].paraValue;

    /* GSM and UMTS share the same PID */
    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("AT_SetMipiWrPara: Send msg fail!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MIPI_WR; /* ���õ�ǰ����ģʽ */
    atDevCmdCtrl->index                   = indexNum;

    return AT_WAIT_ASYNC_RETURN; /* �ȴ��첽�¼����� */
}


VOS_UINT32 AT_SetSSIWrPara(VOS_UINT8 indexNum)
{
    AT_HPA_SsiWrReq   *msg          = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* �������� */
    if (g_atParaIndex != AT_SSIWR_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    length = sizeof(AT_HPA_SsiWrReq) - VOS_MSG_HEAD_LENGTH;
    msg    = (AT_HPA_SsiWrReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_SetSSIWrPara: alloc msg fail!");
        return AT_ERROR;
    }

    if (g_atParaList[0].paraValue == AT_RAT_MODE_GSM) {
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_GPHY), length);
    } else {
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_WPHY), length);
    }

    msg->msgId   = ID_AT_HPA_SSI_WR_REQ;
    msg->rficSsi = (VOS_UINT16)g_atParaList[1].paraValue;
    msg->regAddr = (VOS_UINT16)g_atParaList[AT_SSIWR_PARA_REG_ADDR].paraValue;
    msg->data    = (VOS_UINT16)g_atParaList[AT_SSIWR_PARA_DATA].paraValue;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("AT_SetSSIWrPara: Send msg fail!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SSI_WR; /* ���õ�ǰ����ģʽ */
    atDevCmdCtrl->index                   = indexNum;

    return AT_WAIT_ASYNC_RETURN; /* �ȴ��첽�¼����� */
}


VOS_UINT32 AT_SetSSIRdPara(VOS_UINT8 indexNum)
{
    AT_HPA_SsiRdReq   *msg          = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* �������� */
    if (g_atParaIndex != AT_SSIRD_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    if (g_atParaList[0].paraValue == AT_RAT_MODE_FDD_LTE || g_atParaList[0].paraValue == AT_RAT_MODE_TDD_LTE ||
        g_atParaList[0].paraValue == AT_RAT_MODE_TDSCDMA) {
        /* ����TL�������� */
        return AT_SetTlRficSsiRdPara(indexNum);
    }

    length = sizeof(AT_HPA_SsiRdReq) - VOS_MSG_HEAD_LENGTH;
    msg    = (AT_HPA_SsiRdReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_SetMipiRdPara: alloc msg fail!");
        return AT_ERROR;
    }

    if (g_atParaList[0].paraValue == AT_RAT_MODE_GSM) {
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_GPHY), length);
    } else {
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_WPHY), length);
    }

    msg->msgId     = ID_AT_HPA_SSI_RD_REQ;
    msg->channelNo = (VOS_UINT16)g_atParaList[AT_SSI_RD_PARA_CHANNEL_NO].paraValue;
    msg->rficReg   = g_atParaList[AT_SSI_RD_PARA_FIC_REG].paraValue;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("AT_SetMipiRdPara: Send msg fail!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SSI_RD; /* ���õ�ǰ����ģʽ */
    atDevCmdCtrl->index                   = indexNum;

    return AT_WAIT_ASYNC_RETURN; /* �ȴ��첽�¼����� */
}


VOS_UINT32 AT_SetMipiRdPara(VOS_UINT8 indexNum)
{
    AT_HPA_MipiRdReq  *msg          = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* �������� */
    if (g_atParaIndex != AT_MIPIRD_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    if (g_atParaList[0].paraValue == AT_RAT_MODE_FDD_LTE || g_atParaList[0].paraValue == AT_RAT_MODE_TDD_LTE) {
        return AT_ERROR;
    }

    length = sizeof(AT_HPA_MipiRdReq) - VOS_MSG_HEAD_LENGTH;
    msg    = (AT_HPA_MipiRdReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_SetMipiRdPara: alloc msg fail!");
        return AT_ERROR;
    }

    if (g_atParaList[0].paraValue == AT_RAT_MODE_GSM) {
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_GPHY), length);
    } else {
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_WPHY), length);
    }

    msg->msgId         = ID_AT_HPA_MIPI_RD_REQ;
    msg->channel       = (VOS_UINT16)g_atParaList[1].paraValue;
    msg->secondaryAddr = (VOS_UINT16)g_atParaList[AT_MIPIRD_SECONDARY_ADDR].paraValue;
    msg->reg           = (VOS_UINT16)g_atParaList[AT_MIPIRD_REG_PARA].paraValue;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("AT_SetMipiRdPara: Send msg fail!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MIPI_RD; /* ���õ�ǰ����ģʽ */
    atDevCmdCtrl->index                   = indexNum;

    return AT_WAIT_ASYNC_RETURN; /* �ȴ��첽�¼����� */
}


VOS_UINT32 AT_SetMipiReadPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    AT_MTA_MipiReadReq mipiReadReq;
    VOS_UINT32         result;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* �ֲ�������ʼ�� */
    (VOS_VOID)memset_s(&mipiReadReq, sizeof(mipiReadReq), 0x00, sizeof(AT_MTA_MipiReadReq));

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ������Ŀ����ȷ */
    if (g_atParaIndex != AT_MIPIREAD_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���Ƿ�����ģʽ */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* �������ȼ�� */
    if ((g_atParaList[AT_MIPI_READ_PARA_READ_TYPE].paraLen == 0) ||
        (g_atParaList[AT_MIPI_READ_PARA_MIPI_ID].paraLen == 0) ||
        (g_atParaList[AT_MIPI_READ_PARA_SECONDARY_ID].paraLen == 0) ||
        (g_atParaList[AT_MIPI_READ_PARA_REG_ADDR].paraLen == 0) ||
        (g_atParaList[AT_MIPI_READ_PARA_SPEED_TYPE].paraLen == 0) ||
        (g_atParaList[AT_MIPI_READ_PARA_READ_BIT_MASK].paraLen == 0) ||
        (g_atParaList[AT_MIPI_READ_PARA_REAERVED].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��д��Ϣ���� */
    mipiReadReq.readType    = (VOS_UINT16)g_atParaList[AT_MIPI_READ_PARA_READ_TYPE].paraValue;
    mipiReadReq.mipiId      = (VOS_UINT16)g_atParaList[AT_MIPI_READ_PARA_MIPI_ID].paraValue;
    mipiReadReq.secondaryId = (VOS_UINT16)g_atParaList[AT_MIPI_READ_PARA_SECONDARY_ID].paraValue;
    mipiReadReq.regAddr     = (VOS_UINT16)g_atParaList[AT_MIPI_READ_PARA_REG_ADDR].paraValue;
    mipiReadReq.speedType   = (VOS_UINT16)g_atParaList[AT_MIPI_READ_PARA_SPEED_TYPE].paraValue;
    mipiReadReq.readBitMask = (VOS_UINT16)g_atParaList[AT_MIPI_READ_PARA_READ_BIT_MASK].paraValue;
    mipiReadReq.reserved1   = (VOS_UINT16)g_atParaList[AT_MIPI_READ_PARA_REAERVED].paraValue;

    /* ������Ϣ��MTA */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_MIPIREAD_SET_REQ, (VOS_UINT8 *)&mipiReadReq,
                                    (VOS_SIZE_T)sizeof(mipiReadReq), I0_UEPS_PID_MTA);
    /* ����ʧ�� */
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetMipiReadPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* ���ͳɹ������õ�ǰ����ģʽ */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MIPIREAD_SET;

    /* �ȴ��첽����ʱ�䷵�� */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetPhyMipiWritePara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl     *atDevCmdCtrl = VOS_NULL_PTR;
    AT_MTA_PhyMipiWriteReq phyMipiWriteReq;
    VOS_UINT32             result;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* �ֲ�������ʼ�� */
    (VOS_VOID)memset_s(&phyMipiWriteReq, sizeof(phyMipiWriteReq), 0x00, sizeof(AT_MTA_PhyMipiWriteReq));

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ������Ŀ����ȷ */
    if (g_atParaIndex != AT_PHYMIPIWRITE_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���Ƿ�����ģʽ */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* �������ȼ�� */
    if ((g_atParaList[AT_PHYMIPIWRITE_WR_TYPE].paraLen == 0) ||
        (g_atParaList[AT_PHYMIPIWRITE_MIPI_ID].paraLen == 0) ||
        (g_atParaList[AT_PHYMIPIWRITE_SECONDARY_ID].paraLen == 0) ||
        (g_atParaList[AT_PHYMIPIWRITE_REG_ADDR].paraLen == 0) ||
        (g_atParaList[AT_PHYMIPIWRITE_MIPI_DATA].paraLen == 0) ||
        (g_atParaList[AT_PHYMIPIWRITE_RESERVAED].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��д��Ϣ���� */
    phyMipiWriteReq.writeType   = (VOS_UINT16)g_atParaList[AT_PHYMIPIWRITE_WR_TYPE].paraValue;
    phyMipiWriteReq.mipiId      = (VOS_UINT16)g_atParaList[AT_PHYMIPIWRITE_MIPI_ID].paraValue;
    phyMipiWriteReq.secondaryId = (VOS_UINT16)g_atParaList[AT_PHYMIPIWRITE_SECONDARY_ID].paraValue;
    phyMipiWriteReq.regAddr     = (VOS_UINT16)g_atParaList[AT_PHYMIPIWRITE_REG_ADDR].paraValue;
    phyMipiWriteReq.mipiData    = (VOS_UINT16)g_atParaList[AT_PHYMIPIWRITE_MIPI_DATA].paraValue;
    phyMipiWriteReq.reserved1   = (VOS_UINT16)g_atParaList[AT_PHYMIPIWRITE_RESERVAED].paraValue;

    /* ������Ϣ��MTA */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_PHYMIPIWRITE_SET_REQ, (VOS_UINT8 *)&phyMipiWriteReq,
                                    (VOS_SIZE_T)sizeof(phyMipiWriteReq), I0_UEPS_PID_MTA);
    /* ����ʧ�� */
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetPhyMipiWritePara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* ���ͳɹ������õ�ǰ����ģʽ */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PHYMIPIWRITE_SET;

    /* �ȴ��첽����ʱ�䷵�� */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_SetCltPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* ״̬��� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��������������Ҫ�� */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /*  ���Ƿ�����ģʽ�·����ش��� */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    atDevCmdCtrl->cltEnableFlg = (AT_DSP_CltEnableUint8)g_atParaList[0].paraValue;

    return AT_OK; /* �����������ɹ� */
}


VOS_UINT32 AT_SetPdmCtrlPara(VOS_UINT8 indexNum)
{
    AT_HPA_PdmCtrlReq *msg          = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* �������� */
    if (g_atParaIndex != AT_PDMCTRL_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if ((g_atParaList[AT_PDM_CTRL_REG_VALUE].paraLen == 0) || (g_atParaList[AT_PDM_CTRL_BIAS_1].paraLen == 0) ||
        (g_atParaList[AT_PDM_CTRL_BIAS_2].paraLen == 0) || (g_atParaList[AT_PDM_CTRL_BIAS_3].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }
    /*lint -save -e516 */
    msg = (AT_HPA_PdmCtrlReq *)AT_ALLOC_MSG_WITH_HDR(sizeof(AT_HPA_PdmCtrlReq));
    /*lint -restore */
    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_SetPdmCtrlPara: alloc msg fail!");
        return AT_ERROR;
    }

    /* ��ʼ����Ϣ */
    (VOS_VOID)memset_s((VOS_CHAR *)msg + VOS_MSG_HEAD_LENGTH,
                       sizeof(AT_HPA_PdmCtrlReq) - VOS_MSG_HEAD_LENGTH, 0x00,
                       sizeof(AT_HPA_PdmCtrlReq) - VOS_MSG_HEAD_LENGTH);

    /* ��д��Ϣͷ */
    AT_CFG_MSG_HDR(msg, DSP_PID_WPHY, ID_AT_HPA_PDM_CTRL_REQ);

    msg->msgId       = ID_AT_HPA_PDM_CTRL_REQ;
    msg->rsv         = 0;
    msg->pdmRegValue = (VOS_UINT16)g_atParaList[AT_PDM_CTRL_REG_VALUE].paraValue;
    msg->paVbias     = (VOS_UINT16)g_atParaList[AT_PDM_CTRL_BIAS_1].paraValue;
    msg->paVbias2    = (VOS_UINT16)g_atParaList[AT_PDM_CTRL_BIAS_2].paraValue;
    msg->paVbias3    = (VOS_UINT16)g_atParaList[AT_PDM_CTRL_BIAS_3].paraValue;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("AT_SetPdmCtrlPara: Send msg fail!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PDM_CTRL; /* ���õ�ǰ����ģʽ */
    atDevCmdCtrl->index                   = indexNum;

    return AT_WAIT_ASYNC_RETURN; /* �ȴ��첽�¼����� */
}


VOS_UINT32 At_QryFChanPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    TAF_UINT16         length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) ||
#if (FEATURE_LTEV == FEATURE_ON)
        (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_LTEV) ||
#endif
        (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return atQryFCHANPara(indexNum);
    }
#endif

    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ��ѯ��ǰFCHAN������ */
    length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d,%d,%d,%d", atDevCmdCtrl->deviceRatMode,
        atDevCmdCtrl->deviceAtBand, atDevCmdCtrl->dspBandArfcn.ulArfcn, atDevCmdCtrl->dspBandArfcn.dlArfcn);
    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 At_QryFTxonPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    TAF_UINT16         length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* ��� LTE ģ�Ľӿڷ�֧ */
#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return atQryFTXONPara(indexNum);
    }
#endif

    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }
    /* ��ѯ��ǰDAC������ */
    length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d", atDevCmdCtrl->txOnOff);
    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 At_QryFRxonPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT16         length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* ���LTE ģ�Ľӿڷ�֧ */
#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return atQryFRXONPara(indexNum);
    }
#endif

    /* ��ǰ��Ϊ������ģʽ */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ��ѯ��ǰ���ջ�����״̬ */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, atDevCmdCtrl->rxOnOff);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_QryFpowdetTPara(VOS_UINT8 indexNum)
{
    AT_PHY_PowerDetReq *msg          = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl  *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32          length;
    VOS_UINT32          result;
    VOS_UINT8           isLteFlg;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    isLteFlg     = VOS_FALSE;

    /* �жϵ�ǰ����ģʽ��ֻ֧��W */
    if ((atDevCmdCtrl->deviceRatMode != AT_RAT_MODE_WCDMA)
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
        && (atDevCmdCtrl->deviceRatMode != AT_RAT_MODE_CDMA)
#endif
        && (atDevCmdCtrl->deviceRatMode != AT_RAT_MODE_GSM)
        && (atDevCmdCtrl->deviceRatMode != AT_RAT_MODE_FDD_LTE) &&
        (atDevCmdCtrl->deviceRatMode != AT_RAT_MODE_TDD_LTE)) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ����AT_PHY_PowerDetReq��Ϣ */
    length = sizeof(AT_PHY_PowerDetReq) - VOS_MSG_HEAD_LENGTH;
    /*lint -save -e516 */
    msg = (AT_PHY_PowerDetReq *)PS_ALLOC_MSG(WUEPS_PID_AT, length);
    /*lint -restore */
    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_QryFpowdetTPara: Alloc msg fail!");
        return AT_ERROR;
    }

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    /* CDMA�Ļ������͸�UPHY_PID_CSDR_1X_CM */
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_CDMA) {
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, UPHY_PID_CSDR_1X_CM, length);
    }
    else
#endif
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_GSM) {
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_GPHY), length);
    }
    else if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_WCDMA) {
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_WPHY), length);
    } else {
        isLteFlg = VOS_TRUE;
    }

    if (isLteFlg == VOS_FALSE) {
        msg->msgId = ID_AT_PHY_POWER_DET_REQ;
        msg->rsv   = 0;

        /* ���ӦPHY������Ϣ */
        result = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    } else {
        /*lint --e{516,830} */
        PS_FREE_MSG(WUEPS_PID_AT, msg);
        msg = VOS_NULL_PTR;
        result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                        ID_AT_MTA_POWER_DET_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);
    }

    if (result != VOS_OK) {
        AT_WARN_LOG("AT_QryFpowdetTPara: Send msg fail!");
        return AT_ERROR;
    }

    /* ���õ�ǰ�������� */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FPOWDET_QRY;
    atDevCmdCtrl->index                   = indexNum;

    /* ������������״̬ */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryFPllStatusPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl     *atDevCmdCtrl = VOS_NULL_PTR;
    AT_PHY_RfPllStatusReq *msg          = VOS_NULL_PTR;
    VOS_UINT32             receiverPid;
    VOS_UINT32             length;
    VOS_UINT16             msgId;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return atQryFPllStatusPara(indexNum);
    }
#endif
    /* �жϵ�ǰ����ģʽ��ֻ֧��G/W */
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_WCDMA) {
        receiverPid = AT_GetDestPid(indexNum, I0_DSP_PID_WPHY);
        msgId       = ID_AT_WPHY_RF_PLL_STATUS_REQ;
    } else if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_GSM) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_EDGE)) {
        receiverPid = AT_GetDestPid(indexNum, I0_DSP_PID_GPHY);
        msgId       = ID_AT_GPHY_RF_PLL_STATUS_REQ;
    } else {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ����AT_PHY_RfPllStatusReq��Ϣ */
    length = sizeof(AT_PHY_RfPllStatusReq) - VOS_MSG_HEAD_LENGTH;
    /*lint -save -e516 */
    msg = (AT_PHY_RfPllStatusReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);
    /*lint -restore */
    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_QryFPllStatusPara: Alloc msg fail!");
        return AT_ERROR;
    }

    /* �����Ϣ */
    TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, receiverPid, length);
    msg->msgId   = msgId;
    msg->rsv1    = 0;
    msg->dspBand = atDevCmdCtrl->dspBandArfcn.dspBand;
    msg->rsv2    = 0;

    /* ���ӦPHY������Ϣ */
    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("AT_QryFPllStatusPara: Send msg fail!");
        return AT_ERROR;
    }

    /* ���õ�ǰ�������� */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FPLLSTATUS_QRY;
    atDevCmdCtrl->index                   = indexNum;

    /* ������������״̬ */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QryFrssiPara(VOS_UINT8 indexNum)
{
    AT_HPA_RfRxRssiReq *msg          = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl  *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32          length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) ||
#if (FEATURE_LTEV == FEATURE_ON)
        (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_LTEV) ||
#endif
        (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return atQryFRSSIPara(indexNum);
    }
#endif

#if (FEATURE_UE_MODE_TDS == FEATURE_ON)
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDSCDMA) {
        return atQryFRSSIPara(indexNum);
    }

#endif

    /* ���������ڷ�����ģʽ��ʹ�� */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* �������������÷������ŵ���ʹ�� */
    if (atDevCmdCtrl->dspLoadFlag == VOS_FALSE) {
        return AT_CHANNEL_NOT_SET;
    }

    /* ��������Ҫ�ڴ򿪽��ջ���ʹ�� */
    if (atDevCmdCtrl->rxOnOff == AT_DSP_RF_SWITCH_OFF) {
        return AT_FRSSI_RX_NOT_OPEN;
    }

    /*
     * GDSP LOAD������²�֧�ֽ��ջ��ͷ����ͬʱ�򿪣���Ҫ�ж����һ��ִ�е��Ǵ򿪽��ջ�����
     * ���Ǵ򿪷��������������Ǵ򿪷������������ֱ�ӷ��س��������GDSP ����
     */
    if ((atDevCmdCtrl->rxonOrTxon == AT_TXON_OPEN) &&
        ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_GSM) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_EDGE))) {
        return AT_FRSSI_OTHER_ERR;
    }

    /* ����AT_HPA_RfRxRssiReq��Ϣ */
    length = sizeof(AT_HPA_RfRxRssiReq) - VOS_MSG_HEAD_LENGTH;
    /*lint -save -e830 */
    msg = (AT_HPA_RfRxRssiReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);
    /*lint -restore */
    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_QryFrssiPara: alloc msg fail!");
        return AT_FRSSI_OTHER_ERR;
    }

    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_GSM) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_EDGE)) {
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_GPHY), length);
        msg->msgId = ID_AT_GHPA_RF_RX_RSSI_REQ;
    } else {
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_WPHY), length);
        msg->msgId = ID_AT_HPA_RF_RX_RSSI_REQ;
    }

    msg->measNum  = AT_DSP_RSSI_MEASURE_NUM;
    msg->interval = AT_DSP_RSSI_MEASURE_INTERVAL;
    msg->rsv      = 0;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("At_QryFrssiPara: Send msg fail!");
        return AT_FRSSI_OTHER_ERR;
    }

    /* ���õ�ǰ�������� */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_QUERY_RSSI;
    atDevCmdCtrl->index                   = indexNum;

    return AT_WAIT_ASYNC_RETURN; /* ������������״̬ */
}


VOS_UINT32 AT_QryTSelRfPara(VOS_UINT8 indexNum)
{
#if (FEATURE_LTE == FEATURE_ON)
    return atQryTselrfPara(indexNum);
#else
    VOS_UINT16 length;

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%d%s", g_parseContext[indexNum].cmdElement->cmdName, AT_TSELRF_PATH_TOTAL,
        g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_GSM, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_WCDMA_PRI, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_WCDMA_DIV, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_WIFI);

    g_atSendDataBuff.usBufLen = length;

    return AT_OK;
#endif
}


VOS_UINT32 At_QryFpaPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT16         length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* ��ǰ��Ϊ������ģʽ */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ��ѯ��ǰ�����PA�ȼ������� */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, atDevCmdCtrl->paLevel);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_QryFAgcgainPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT16         length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* ��� LTE ģ�Ľӿڷ�֧ */
#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return atQryFLNAPara(indexNum);
    }
#endif

    /* ��ǰ��Ϊ������ģʽ */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ��ѯ��ǰ�����PA�ȼ������� */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, atDevCmdCtrl->lnaLevel);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

/*
 * ��������: ^DCXOTEMPCOMP�Ĳ�ѯ�������
 */
VOS_UINT32 AT_QryDcxotempcompPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }
    /*  ���Ƿ�����ģʽ�·����ش��� */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            atDevCmdCtrl->dcxoTempCompEnableFlg);

    return AT_OK;
}


VOS_UINT32 AT_QryFDac(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    TAF_UINT16         length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ��ѯ��ǰDAC������ */
    length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d", atDevCmdCtrl->fdac);
    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 At_QryCltInfo(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT16         length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    length       = 0;

    /* �����¼��Ч�����ϱ���ѯ��� */
    if (atDevCmdCtrl->cltInfo.infoAvailableFlg == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length,
            "%s%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            "^CLTINFO: ", atDevCmdCtrl->cltInfo.gammaReal, /* ����ϵ��ʵ�� */
            atDevCmdCtrl->cltInfo.gammaImag,               /* ����ϵ���鲿 */
            atDevCmdCtrl->cltInfo.gammaAmpUc0,             /* פ����ⳡ��0����ϵ������ */
            atDevCmdCtrl->cltInfo.gammaAmpUc1,             /* פ����ⳡ��1����ϵ������ */
            atDevCmdCtrl->cltInfo.gammaAmpUc2,             /* פ����ⳡ��2����ϵ������ */
            atDevCmdCtrl->cltInfo.gammaAntCoarseTune,      /* �ֵ����λ�� */
            atDevCmdCtrl->cltInfo.ulwFomcoarseTune,        /* �ֵ�FOMֵ */
            atDevCmdCtrl->cltInfo.cltAlgState,             /* �ջ��㷨����״̬ */
            atDevCmdCtrl->cltInfo.cltDetectCount,          /* �ջ������ܲ��� */
            atDevCmdCtrl->cltInfo.dac0,                    /* DAC0 */
            atDevCmdCtrl->cltInfo.dac1,                    /* DAC1 */
            atDevCmdCtrl->cltInfo.dac2,                    /* DAC2 */
            atDevCmdCtrl->cltInfo.dac3);                   /* DAC3 */

        /* �ϱ��󱾵ؼ�¼����Ч */
        atDevCmdCtrl->cltInfo.infoAvailableFlg = VOS_FALSE;
    }

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 At_TestFdacPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT16         length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    length       = 0;

    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_WCDMA) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_AWS)) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: (0-2047)", g_parseContext[indexNum].cmdElement->cmdName);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: (0-1023)", g_parseContext[indexNum].cmdElement->cmdName);
    }
    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT8 At_GetDspLoadMode(VOS_UINT32 ratMode)
{
    if ((ratMode == AT_RAT_MODE_WCDMA) || (ratMode == AT_RAT_MODE_AWS)) {
        return VOS_RATMODE_WCDMA;
    } else if ((ratMode == AT_RAT_MODE_GSM) || (ratMode == AT_RAT_MODE_EDGE)) {
        return VOS_RATMODE_GSM;
    }
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    else if (ratMode == AT_RAT_MODE_CDMA) {
        return VOS_RATMODE_1X;
    }
#endif
    else {
        return VOS_RATMODE_BUTT;
    }
}


VOS_BOOL AT_GetTseLrfLoadDspInfo(AT_TSELRF_PathUint32 path, DRV_AGENT_TselrfSetReq *tseLrf)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_BOOL           loadDsp;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* ^TSELRF�������õ���Ƶͨ·���ΪGSM�ҵ�ǰ�Ѿ�LOAD�˸�ͨ·������LOAD */
    if (path == AT_TSELRF_PATH_GSM) {
        if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_GSM) && (atDevCmdCtrl->dspLoadFlag == VOS_TRUE)) {
            loadDsp = VOS_FALSE;
        } else {
            tseLrf->loadDspMode   = VOS_RATMODE_GSM;
            tseLrf->deviceRatMode = AT_RAT_MODE_GSM;
            loadDsp               = VOS_TRUE;
        }
        return loadDsp;
    }

    /* ^TSELRF�������õ���Ƶͨ·���ΪWCDMA�����ҵ�ǰ�Ѿ�LOAD�˸�ͨ·������LOAD */
    if (path == AT_TSELRF_PATH_WCDMA_PRI) {
        if (((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_WCDMA) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_AWS)) &&
            (atDevCmdCtrl->dspLoadFlag == VOS_TRUE)) {
            loadDsp = VOS_FALSE;
        } else {
            tseLrf->loadDspMode   = VOS_RATMODE_WCDMA;
            tseLrf->deviceRatMode = AT_RAT_MODE_WCDMA;
            loadDsp               = VOS_TRUE;
        }
        return loadDsp;
    }

    loadDsp = VOS_FALSE;

    AT_WARN_LOG("AT_GetTseLrfLoadDspInfo: enPath only support GSM or WCDMA primary.");

    return loadDsp;
}


VOS_UINT32 AT_SetGlobalFchan(VOS_UINT8 rATMode)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl                = AT_GetDevCmdCtrl();
    atDevCmdCtrl->deviceRatMode = rATMode;

    return VOS_OK;
}


VOS_UINT32 AT_SetTlRficSsiRdPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl  *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32          result;
    AT_MTA_RficssirdReq rficSsiRdReq;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* ͨ����� */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        return AT_ERROR;
    }

    /* ��ʼ�� */
    (VOS_VOID)memset_s(&rficSsiRdReq, sizeof(rficSsiRdReq), 0x00, sizeof(rficSsiRdReq));

    rficSsiRdReq.channelNo = (VOS_UINT16)g_atParaList[AT_SSIRD_CHANNEL_NO].paraValue;
    rficSsiRdReq.rficReg   = (VOS_UINT16)g_atParaList[AT_SSIRD_RFIC_REG].paraValue;

    /* ������Ϣ */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_RFICSSIRD_QRY_REQ, (VOS_UINT8 *)&rficSsiRdReq, sizeof(rficSsiRdReq),
                                    I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetTlRficSsiRdPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* ����ATģ��ʵ���״̬Ϊ�ȴ��첽���� */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RFICSSIRD_SET;
    atDevCmdCtrl->index                   = indexNum;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetFPllStatusQryPara(VOS_UINT8 indexNum)
{
    return AT_CME_OPERATION_NOT_SUPPORTED;
}

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)

VOS_UINT32 AT_SetWifiFwavePara(VOS_VOID)
{
    VOS_CHAR acCmd[200] = {0};
    VOS_INT32 bufLen;

    /* WIFIδEnableֱ�ӷ���ʧ�� */
    if ((VOS_UINT32)AT_GetWifiStatus() == VOS_FALSE) {
        return AT_ERROR;
    }

    /* ��WIFI���͵��������ź�����,���䲨�εĹ��ʴ�С,G��0.01dBmΪ��λ,��Ҫ�����ʴ�С��100 */
    bufLen = snprintf_s(acCmd, sizeof(acCmd), sizeof(acCmd) - 1, "athtestcmd -ieth0 --tx sine --txpwr %d",
        g_atParaList[1].paraValue / 100);
    TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(acCmd), sizeof(acCmd) - 1);

    AT_WIFI_TEST_CMD_CALL(acCmd);

    return AT_OK;
}


VOS_UINT32 AT_ProcTSelRfWifiPara(VOS_VOID)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_SUPPORT) {
        /* WIFIδEnableֱ�ӷ���ʧ�� */
        if ((VOS_UINT32)AT_GetWifiStatus() == VOS_FALSE) {
            return AT_ERROR;
        }

        atDevCmdCtrl->deviceRatMode = AT_RAT_MODE_WIFI;

        return AT_OK;
    } else {
        return AT_ERROR;
    }
}
#endif

#else

VOS_VOID AT_SetUartTestState(AT_UartTestStateUint8 uartTestState)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx                            = AT_GetMtInfoCtx();
    atMtInfoCtx->cifTestInfo.uartTestState = uartTestState;
}


AT_UartTestStateUint8 AT_GetUartTestState(VOS_VOID)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();
    return atMtInfoCtx->cifTestInfo.uartTestState;
}


VOS_VOID AT_SetI2sTestState(AT_I2sTestStateUint8 i2sTestState)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx                           = AT_GetMtInfoCtx();
    atMtInfoCtx->cifTestInfo.i2sTestState = i2sTestState;
}


AT_I2sTestStateUint8 AT_GetI2sTestState(VOS_VOID)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();
    return atMtInfoCtx->cifTestInfo.i2sTestState;
}


VOS_UINT32 At_SetFChanPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_SetFChanPara Enter");

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����������ÿ��band���Եĵ�һ�����������ǰ����ģʽ�����Ϣ��� */
    (VOS_VOID)memset_s(&atMtInfoCtx->atInfo, sizeof(atMtInfoCtx->atInfo), 0, sizeof(AT_MT_AtInfo));
    (VOS_VOID)memset_s(&atMtInfoCtx->bbicInfo, sizeof(atMtInfoCtx->bbicInfo), 0, sizeof(AT_MT_BbicInfo));

    /* WIFIģʽ */
    if (g_atParaList[AT_FCHAN_PARA_MODE].paraValue == AT_RAT_MODE_WIFI) {
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
        return At_SetFChanWifiProc(g_atParaList[AT_FCHAN_PARA_BAND].paraValue);
#else
        return AT_ERROR;
#endif
    }

    /* ��ǰģʽ�ж� */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /*
     * ����������Ҫ��,֧��NR�İ汾�У�һ��Ҫ����Band width,
     * Ŀǰ���������ش�����ģʽ��Band, �ŵ��źʹ����һ����ѡ�������ز����
     */
    if (g_atParaIndex < AT_FCHAN_NR_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[AT_FCHAN_PARA_BAND].paraValue >= AT_PROTOCOL_BAND_BUTT) {
        return AT_FCHAN_BAND_NOT_MATCH;
    }

    /* ��¼band��Ϣ, Ҫ�󹤾��·��Ͱ�Э��band���·� */
    atMtInfoCtx->atInfo.bandArfcn.dspBand = (AT_PROTOCOL_BandUint16)g_atParaList[AT_FCHAN_PARA_BAND].paraValue;

    /* ��¼���Խ��뼼��ģʽ��Ϣ */
    if (At_CovertAtModeToBbicCal((AT_DEVICE_CmdRatModeUint8)g_atParaList[AT_FCHAN_PARA_MODE].paraValue,
                                 &atMtInfoCtx->bbicInfo.currtRatMode) == VOS_FALSE) {
        return AT_FCHAN_RAT_ERR;
    }

    atMtInfoCtx->atInfo.ratMode = (AT_DEVICE_CmdRatModeUint8)g_atParaList[AT_FCHAN_PARA_MODE].paraValue;

    /* ��ȡƵ����Ϣ */
    if (AT_GetFreq(g_atParaList[AT_FCHAN_PARA_CHANNEL_NO].paraValue) == VOS_FALSE) {
        return AT_DEVICE_CHAN_BAND_CHAN_NOT_MAP;
    }

    /* ��¼������Ϣ */
    if (At_CovertAtBandWidthToBbicCal((AT_BAND_WidthUint16)g_atParaList[AT_FCHAN_PARA_UL_CHANNEL].paraValue,
                                      &atMtInfoCtx->bbicInfo.bandWidth) == VOS_FALSE) {
        return AT_FCHAN_BAND_WIDTH_ERR;
    }

    atMtInfoCtx->atInfo.bandWidth = (AT_BAND_WidthUint16)g_atParaList[AT_FCHAN_PARA_UL_CHANNEL].paraValue;

    /* ��AT�Ĵ���ת����������ֵ��¼���� */
    At_CovertAtBandWidthToValue((AT_BAND_WidthUint16)g_atParaList[AT_FCHAN_PARA_UL_CHANNEL].paraValue,
                                &atMtInfoCtx->bbicInfo.bandWidthValue);

    if (atMtInfoCtx->atInfo.ratMode == AT_RAT_MODE_NR) {
        if (g_atParaList[AT_FCHAN_PARA_DL_CHANNEL].paraLen == 0) {
            return AT_FCHAN_NO_SCS;
        }

        if (At_CovertAtScsToBbicCal((AT_SubCarrierSpacingUint8)g_atParaList[AT_FCHAN_PARA_DL_CHANNEL].paraValue,
                                    (VOS_UINT8 *)&atMtInfoCtx->bbicInfo.bbicScs) == VOS_FALSE) {
            return AT_FCHAN_SCS_ERR;
        }

        atMtInfoCtx->atInfo.bbicScs = (AT_SubCarrierSpacingUint8)g_atParaList[AT_FCHAN_PARA_DL_CHANNEL].paraValue;
    }

    /* ��¼��ǰclient */
    atMtInfoCtx->atInfo.indexNum = indexNum;

    /* ��CBT����Ϣ��֪ͨ�����PHY */
    if (At_LoadPhy() == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FCHAN_SET;

        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_SND_MSG_FAIL;
}


VOS_UINT32 AT_SetFwavePara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    VOS_CHAR    acCmd[AT_FWAVE_CMD_ARRAY_LEN]  = {0};
    VOS_INT32   bufLen;
#endif

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("AT_SetFwavePara Enter");

    /* ״̬��� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ������������ȷ����������������ͺͲ��ι��� */
    if (g_atParaIndex < AT_FWAVE_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���������ڷ�����ģʽ��ʹ�� */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* WIFI */
    if (atMtInfoCtx->atInfo.ratMode == AT_RAT_MODE_WIFI) {
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
        /* WIFIδEnableֱ�ӷ���ʧ�� */
        if ((VOS_UINT32)AT_GetWifiStatus() == VOS_FALSE) {
            return AT_WIFI_NOT_ENABLE;
        }

        /* ��WIFI���͵��������ź�����,���䲨�εĹ��ʴ�С,G��0.01dBmΪ��λ,��Ҫ�����ʴ�С��100 */
        bufLen = snprintf_s(acCmd, sizeof(acCmd), sizeof(acCmd) - 1, "athtestcmd -ieth0 --tx sine --txpwr %d",
                            g_atParaList[AT_FWAVE_WAVEFORM_POWER].paraValue / 100);
        TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(acCmd), sizeof(acCmd) - 1);

        AT_WIFI_TEST_CMD_CALL(acCmd);

        return AT_OK;
#else
        return AT_ERROR;
#endif
    }

    /* �Ƿ������DSP */
    if (atMtInfoCtx->atInfo.dspLoadFlag != VOS_TRUE) {
        return AT_NOT_LOAD_DSP;
    }

    /* ���ݽ��뼼���жϲ��������Ƿ�֧�� */
    if (AT_CheckFwaveTypePara(g_atParaList[AT_FWAVE_WAVEFORM_TYPE].paraValue) == VOS_FALSE) {
        return AT_FWAVE_TYPE_ERR;
    }

    atMtInfoCtx->atInfo.faveType = (AT_FWAVE_TypeUint8)g_atParaList[AT_FWAVE_WAVEFORM_TYPE].paraValue;

    /* ת��TYPE ���� */
    AT_CovertAtFwaveTypeToBbicCal((AT_FWAVE_TypeUint8)g_atParaList[AT_FWAVE_WAVEFORM_TYPE].paraValue,
                                  &atMtInfoCtx->bbicInfo.fwaveType);

    /* MT��λ�����·��ĵ�λ��0.01dB,  GUC�����ʹ�õĵ�λ��0.1db,LTE��NRʹ�õ���0.125dB(X / 10 * (4 / 5)) */
    if ((atMtInfoCtx->bbicInfo.currtRatMode == RAT_MODE_LTE) ||
        (atMtInfoCtx->bbicInfo.currtRatMode == RAT_MODE_NR)) {
        atMtInfoCtx->bbicInfo.fwavePower    = (VOS_INT16)(g_atParaList[AT_FWAVE_WAVEFORM_POWER].paraValue * 2 / 25);
    } else {
        /* GUC�����ʹ�õĵ�λ��0.1db,��Ҫ�����ʴ�С��10 */
        atMtInfoCtx->bbicInfo.fwavePower = (VOS_INT16)(g_atParaList[AT_FWAVE_WAVEFORM_POWER].paraValue / 10);
    }

    /* ���ǲ���GSM�ĵ����źţ�����Ҫ������������ */
    if ((atMtInfoCtx->bbicInfo.currtRatMode == RAT_MODE_GSM) &&
        (atMtInfoCtx->atInfo.faveType != AT_FWAVE_TYPE_CONTINUE)) {
        if (g_atParaList[AT_FWAVE_GSM_TX_SLOT_TYPE].paraValue >= AT_GSM_TX_SLOT_BUTT) {
            return AT_CME_INCORRECT_PARAMETERS;
        } else {
            atMtInfoCtx->atInfo.gsmTxSlotType =
                (AT_GsmTxSlotTypeUint8)g_atParaList[AT_FWAVE_GSM_TX_SLOT_TYPE].paraValue;
        }
    }

    return AT_OK;
}


VOS_UINT32 At_SetFTxonPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_SetFTxonPara Enter");

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_FTXON_OTHER_ERR;
    }
    /* ����������Ҫ�� */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraValue >= AT_DSP_RF_SWITCH_BUTT) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��ǰ�Ƿ���FTMģʽ */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ����Ƿ������DSP */
    if (atMtInfoCtx->atInfo.dspLoadFlag == VOS_FALSE) {
        return AT_NOT_LOAD_DSP;
    }

    /* ����Ƿ�������path */
    if (atMtInfoCtx->atInfo.setTxTselrfFlag == VOS_FALSE) {
        return AT_NOT_SET_PATH;
    }

    /* ��¼ȫ�ֱ��� */
    atMtInfoCtx->atInfo.tempRxorTxOnOff = (AT_DSP_RfOnOffUint8)g_atParaList[0].paraValue;

    /* ��BBIC����Ϣ */
    if ((atMtInfoCtx->bbicInfo.currtRatMode == RAT_MODE_GSM) &&
        (atMtInfoCtx->atInfo.faveType != AT_FWAVE_TYPE_CONTINUE)) {
        if (At_SndGsmTxOnOffReq_ModulatedWave(VOS_FALSE) != VOS_TRUE) {
            return AT_SND_MSG_FAIL;
        }
    } else {
        if (At_SndTxOnOffReq(VOS_FALSE) != VOS_TRUE) {
            return AT_SND_MSG_FAIL;
        }
    }

    /* ���õ�ǰ�������� */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SET_FTXON;
    atMtInfoCtx->atInfo.indexNum          = indexNum;

    return AT_WAIT_ASYNC_RETURN; /* ������������״̬ */
}


VOS_UINT32 At_SetFRxonPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_SetFRxonPara Enter");

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����������Ҫ�� */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��AT������AT^TMODE=1������ģʽ��ʹ�� */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ��AT������Ҫ��AT^FCHAN���÷������ŵ�����ִ�гɹ���ʹ�� */
    if (atMtInfoCtx->atInfo.dspLoadFlag == VOS_FALSE) {
        return AT_CHANNEL_NOT_SET;
    }

    /* ��AT������Ҫ��AT^TSELRF���÷������ŵ�����ִ�гɹ���ʹ�� */
    if (atMtInfoCtx->atInfo.setRxTselrfFlag == VOS_FALSE) {
        return AT_NOT_SET_PATH;
    }

    atMtInfoCtx->atInfo.tempRxorTxOnOff = (AT_DSP_RfOnOffUint8)g_atParaList[0].paraValue;

    /* ��BBIC CAL����Ϣ */
    if (At_SndRxOnOffReq() == VOS_FALSE) {
        return AT_SND_MSG_FAIL;
    }

    /* ���õ�ǰ�������� */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SET_FRXON;
    atMtInfoCtx->atInfo.indexNum          = indexNum;

    /* ������������״̬ */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetMipiOpeRatePara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  result;
    VOS_UINT32  readSpeed;

    atMtInfoCtx = AT_GetMtInfoCtx();
    result      = AT_FAILURE;
    readSpeed   = 0;

    AT_PR_LOGH("AT_SetMipiOpeRatePara Enter");

    /* ״̬��� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��������������Ҫ�� */
    if (g_atParaIndex < AT_MIPIOPERATE_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraValue == 0) {
        if (g_atParaList[AT_MIPIOPERATE_DATA_BITBYTE0_VALUE].paraLen != 0) {
            readSpeed = g_atParaList[AT_MIPIOPERATE_DATA_BITBYTE0_VALUE].paraValue;
        }

        if (g_atParaList[AT_MIPIOPERATE_BYTE_CNT].paraValue <= AT_MT_MIPI_READ_MAX_BYTE) {
            /* ���ڵײ㲻֧�����ö�ȡ�ٶȣ������һ������������ */
            result = AT_SndBbicCalMipiReadReq(g_atParaList[AT_MIPIOPERATE_MIPI_PORT_SEL].paraValue,
                                              g_atParaList[AT_MIPIOPERATE_SECONDARY_ID].paraValue,
                                              g_atParaList[AT_MIPIOPERATE_REG_ADDR].paraValue,
                                              g_atParaList[AT_MIPIOPERATE_BYTE_CNT].paraValue, readSpeed);
        }
    } else {
        if (g_atParaList[AT_MIPIOPERATE_DATA_BITBYTE0_VALUE].paraLen == 0) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* MIPIд��ʱ��Ĭ��ֻдһ��ֵ */
        g_atParaList[AT_MIPIOPERATE_BYTE_CNT].paraValue = 1;
        result = AT_SndBbicCalMipiWriteReq(g_atParaList[AT_MIPIOPERATE_MIPI_PORT_SEL].paraValue,
                                           g_atParaList[AT_MIPIOPERATE_SECONDARY_ID].paraValue,
                                           g_atParaList[AT_MIPIOPERATE_REG_ADDR].paraValue,
                                           g_atParaList[AT_MIPIOPERATE_BYTE_CNT].paraValue,
                                           g_atParaList[AT_MIPIOPERATE_DATA_BITBYTE0_VALUE].paraValue);
    }

    /* ����ʧ�� */
    if (result != AT_SUCCESS) {
        AT_ERR_LOG("AT_SetMipiOpeRatePara: AT Snd ReqMsg fail.");
        return AT_SND_MSG_FAIL;
    }

    /* ���ͳɹ������õ�ǰ����ģʽ */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MIPIOPERATE_SET;
    atMtInfoCtx->atInfo.indexNum          = indexNum;

    /* �ȴ��첽����ʱ�䷵�� */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetFAgcgainPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("AT_SetFAgcgainPara Enter");

    /* ״̬��� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /*  ���Ƿ�����ģʽ�·����ش��� */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* �������������÷������ŵ���ʹ�� */
    if (atMtInfoCtx->atInfo.dspLoadFlag == VOS_FALSE) {
        return AT_NOT_LOAD_DSP;
    }

    /* ������� */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    atMtInfoCtx->atInfo.agcGainLevel = (VOS_UINT8)g_atParaList[0].paraValue;

    return AT_OK;
}


VOS_UINT32 AT_SetFRbInfoPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("AT_SetFRbInfoPara Enter");

    /* ״̬��� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /*  ���Ƿ�����ģʽ�·����ش��� */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* �������������÷������ŵ���ʹ�� */
    if (atMtInfoCtx->atInfo.dspLoadFlag == VOS_FALSE) {
        return AT_NOT_LOAD_DSP;
    }

    /* ������� */
    if (g_atParaIndex != AT_FRBINFO_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    atMtInfoCtx->atInfo.rbNum      = (VOS_UINT16)g_atParaList[AT_FRBINFO_PARA_RBNUM].paraValue;
    atMtInfoCtx->atInfo.rbStartCfg = (VOS_UINT16)g_atParaList[AT_FRBINFO_PARA_RBSTARTCFG].paraValue;
    atMtInfoCtx->atInfo.rbEnable   = VOS_TRUE;

    return AT_OK;
}


VOS_UINT32 AT_CheckMimoPara(VOS_VOID)
{
    if (g_atParaList[AT_TSELRF_TX_OR_RX].paraValue == 0) {
        /* ˫TXͬʱ����ʱ������group������3 */
        if ((g_atParaList[AT_TSELRF_PARA_TYPE].paraValue != AT_MIMO_TYPE_2) ||
            (g_atParaList[AT_TSELRF_PARA_ANT_NUM].paraValue > AT_MIMO_ANT_NUM_3)) {
            AT_PR_LOGH("AT_CheckMimoPara TX MIMO ERR");
            return AT_FAILURE;
        }

        return AT_SUCCESS;
    } else {
        if ((g_atParaList[AT_TSELRF_PARA_TYPE].paraValue == AT_MIMO_TYPE_2) &&
            (g_atParaList[AT_TSELRF_PARA_ANT_NUM].paraValue > AT_MIMO_ANT_NUM_2)) {
            AT_PR_LOGH("AT_CheckMimoPara TX MIMO_2 ERR");
            return AT_FAILURE;
        }

        if ((g_atParaList[AT_TSELRF_PARA_TYPE].paraValue == AT_MIMO_TYPE_4) &&
            (g_atParaList[AT_TSELRF_PARA_ANT_NUM].paraValue > AT_MIMO_ANT_NUM_4)) {
            AT_PR_LOGH("AT_CheckMimoPara TX MIMO_4 ERR");
            return AT_FAILURE;
        }

        if ((g_atParaList[AT_TSELRF_PARA_TYPE].paraValue == AT_MIMO_TYPE_8) &&
            (g_atParaList[AT_TSELRF_PARA_ANT_NUM].paraValue > AT_MIMO_ANT_NUM_8)) {
            AT_PR_LOGH("AT_CheckMimoPara TX MIMO_8 ERR");
            return AT_FAILURE;
        }

        return AT_SUCCESS;
    }
}


VOS_UINT32 AT_ProcTSelRfPara(AT_AntTypeUint8 antType)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    if (g_atParaList[AT_TSELRF_TX_OR_RX].paraValue == 0) {
        /* ��ʼ�� */
        atMtInfoCtx->atInfo.txMimoType     = AT_MIMO_TYPE_BUTT;
        atMtInfoCtx->atInfo.txMimoAntNum   = AT_MIMO_ANT_NUM_BUTT;
        atMtInfoCtx->bbicInfo.txMimoType   = 0xFF;
        atMtInfoCtx->bbicInfo.txMimoAntNum = 0xFF;

        if (antType == AT_ANT_TYPE_MIMO) {
            if (AT_CheckMimoPara() == AT_FAILURE) {
                return AT_MIMO_PARA_ERR;
            }

            atMtInfoCtx->atInfo.txMimoType   = (AT_MIMO_TYPE_UINT8)g_atParaList[AT_TSELRF_PARA_TYPE].paraValue;
            atMtInfoCtx->atInfo.txMimoAntNum = (AT_MIMO_AntNumUint8 )g_atParaList[AT_TSELRF_PARA_ANT_NUM].paraValue;
            /* BBIC����BIT��ʾ */
            atMtInfoCtx->bbicInfo.txMimoType   = (VOS_UINT16)AT_SET_BIT32(atMtInfoCtx->atInfo.txMimoType);
            atMtInfoCtx->bbicInfo.txMimoAntNum = (VOS_UINT16)AT_SET_BIT32(atMtInfoCtx->atInfo.txMimoAntNum - 1);

            /* ˫TX���� */
            if (atMtInfoCtx->atInfo.txMimoAntNum == AT_MIMO_ANT_NUM_3) {
                atMtInfoCtx->bbicInfo.txMimoAntNum = AT_MIMO_ANT_NUM_3;
            }
        }

        atMtInfoCtx->bbicInfo.txAntType     = antType;
        atMtInfoCtx->atInfo.tseLrfTxPath    = g_atParaList[AT_TSELRF_TX_OR_RX_PATH].paraValue;
        atMtInfoCtx->atInfo.setTxTselrfFlag = VOS_TRUE;
    } else if (g_atParaList[AT_TSELRF_TX_OR_RX].paraValue == 1) {
        /* ��ʼ�� */
        atMtInfoCtx->atInfo.rxMimoType     = AT_MIMO_TYPE_BUTT;
        atMtInfoCtx->atInfo.rxMimoAntNum   = AT_MIMO_ANT_NUM_BUTT;
        atMtInfoCtx->bbicInfo.rxMimoType   = 0xFF;
        atMtInfoCtx->bbicInfo.rxMimoAntNum = 0xFF;

        if (antType == AT_ANT_TYPE_MIMO) {
            if (AT_CheckMimoPara() == AT_FAILURE) {
                return AT_MIMO_PARA_ERR;
            }

            atMtInfoCtx->atInfo.rxMimoType   = (AT_MIMO_TYPE_UINT8)g_atParaList[AT_TSELRF_PARA_TYPE].paraValue;
            atMtInfoCtx->atInfo.rxMimoAntNum = (AT_MIMO_AntNumUint8 )g_atParaList[AT_TSELRF_PARA_ANT_NUM].paraValue;
            /* BBIC����BIT��ʾ */
            atMtInfoCtx->bbicInfo.rxMimoType   = (VOS_UINT16)AT_SET_BIT32(atMtInfoCtx->atInfo.rxMimoType);
            atMtInfoCtx->bbicInfo.rxMimoAntNum = (VOS_UINT16)AT_SET_BIT32(atMtInfoCtx->atInfo.rxMimoAntNum - 1);
        }

        atMtInfoCtx->bbicInfo.rxAntType     = antType;
        atMtInfoCtx->atInfo.tseLrfRxPath    = g_atParaList[AT_TSELRF_TX_OR_RX_PATH].paraValue;
        atMtInfoCtx->atInfo.setRxTselrfFlag = VOS_TRUE;
    } else {
        return AT_ESELRF_TX_OR_RX_ERR;
    }

    return AT_OK;
}


VOS_UINT32 AT_SetTSelRfPara(VOS_UINT8 indexNum)
{
    AT_MT_Info     *atMtInfoCtx = VOS_NULL_PTR;
    AT_AntTypeUint8 antType;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("AT_SetTSelRfPara Enter");

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[AT_TSELRF_TX_OR_RX_PATH].paraValue >= AT_TSELRF_PATH_BUTT) {
        return AT_TSELRF_PATH_ERR;
    }

    if (g_atParaList[AT_TSELRF_TX_OR_RX_PATH].paraValue == AT_TSELRF_PATH_WIFI) {
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
        return AT_ProcTSelRfWifiPara();
#else
        return AT_ERROR;
#endif
    }

    /* ����������Ҫ��,�������������� */
    if (g_atParaIndex < AT_TSELRF_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    if (AT_CoverTselPathToPriOrDiv(g_atParaList[AT_TSELRF_TX_OR_RX_PATH].paraValue, &antType) != VOS_TRUE) {
        return AT_TSELRF_PATH_ERR;
    }

    /* ���MIMO�Ĳ����Ƿ������ */
    if ((antType == AT_ANT_TYPE_MIMO) &&
        ((g_atParaList[AT_TSELRF_PARA_TYPE].paraLen == 0) || (g_atParaList[AT_TSELRF_PARA_ANT_NUM].paraLen == 0))) {
        return AT_MIMO_PARA_ERR;
    }

    return AT_ProcTSelRfPara(antType);
}


VOS_UINT32 At_SetFpaPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_SetFpaPara Enter");

    /* ����״̬��� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_FPA_OTHER_ERR;
    }

    /* �������� */
    if (g_atParaIndex > 1) {
        return AT_FPA_OTHER_ERR;
    }

    /* ���������ڷ�����ģʽ��ʹ�� */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* �������������÷������ŵ���ʹ��,��^FCHAN�ɹ�ִ�к� */
    if (atMtInfoCtx->atInfo.dspLoadFlag == VOS_FALSE) {
        return AT_NOT_LOAD_DSP;
    }

    if (At_CovertAtPaLevelToBbicCal((AT_CmdPalevelUint8)g_atParaList[AT_FPA_LEVEL].paraValue,
                                    &atMtInfoCtx->bbicInfo.paLevel) == VOS_FALSE) {
        return AT_FPA_LEVEL_ERR;
    }
    /* �����ñ����ڱ��ر��� */
    atMtInfoCtx->atInfo.paLevel = (AT_CmdPalevelUint8)g_atParaList[AT_FPA_LEVEL].paraValue;

    return AT_OK;
}


VOS_UINT32 AT_SetDcxotempcompPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("AT_SetDcxotempcompPara Enter");

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����������� */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /*  ���Ƿ�����ģʽ�·����ش��� */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    if (g_atParaList[0].paraValue == 0) {
        atMtInfoCtx->bbicInfo.dcxoTempCompEnableFlg = BBIC_DCXO_SET_DISABLE;
    } else {
        atMtInfoCtx->bbicInfo.dcxoTempCompEnableFlg = BBIC_DCXO_SET_ENABLE;
    }

    if (At_SndDcxoReq() == VOS_FALSE) {
        return AT_SND_MSG_FAIL;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DCXOTEMPCOMP_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_SetDpdtPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  rst;

    atMtInfoCtx = AT_GetMtInfoCtx();
    rst         = AT_ERROR;

    AT_PR_LOGH("At_SetDpdtPara Enter");

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��ǰ��Ϊ������ģʽ */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ����������� */
    if (g_atParaIndex != AT_DPDT_TO_BBIC_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��¼���Խ��뼼��ģʽ��Ϣ */
    if (At_CovertRatModeToBbicCal((AT_CmdRatmodeUint8)g_atParaList[AT_DPDT_RAT_MODE].paraValue,
                                   &atMtInfoCtx->bbicInfo.dpdtRatMode) == VOS_FALSE) {
        return AT_DPDT_RAT_ERR;
    }

    /* AT����DPDT������Ϣ��BBIC���� */

    rst = AT_SndBbicCalSetDpdtReq(BBIC_DPDT_OPERTYPE_SET, g_atParaList[AT_DPDT_VALUE].paraValue,
                                  g_atParaList[AT_DPDT_WORK_TYPE].paraValue);

    if (rst == AT_SUCCESS) {
        atMtInfoCtx->atInfo.indexNum          = indexNum;
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DPDT_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_SetQryDpdtPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  rst;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_SetQryDpdtPara Enter");

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��ǰ��Ϊ������ģʽ */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ����������� */
    if (g_atParaIndex != AT_DPDTQRY_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��¼���Խ��뼼��ģʽ��Ϣ */
    if (At_CovertRatModeToBbicCal((AT_CmdRatmodeUint8)g_atParaList[AT_DPDT_QRY_RAT_MODE].paraValue,
                                   &atMtInfoCtx->bbicInfo.dpdtRatMode) == VOS_FALSE) {
        return AT_DPDT_RAT_ERR;
    }

    /* AT����DPDT��ѯ��Ϣ��BBIC���� */
    rst = AT_SndBbicCalSetDpdtReq(BBIC_DPDT_OPERTYPE_GET, 0, g_atParaList[AT_DPDT_QRY_WORK_TYPE].paraValue);

    if (rst == AT_SUCCESS) {
        atMtInfoCtx->atInfo.indexNum          = indexNum;
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DPDTQRY_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_QryFChanPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT16  length;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_QryFChanPara Enter");

    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ��ѯ��ǰFCHAN������ */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d,%d,%d,%d,%d", atMtInfoCtx->atInfo.ratMode,
        atMtInfoCtx->atInfo.bandArfcn.dspBand, atMtInfoCtx->atInfo.bandArfcn.ulChanNo,
        atMtInfoCtx->atInfo.bandArfcn.dlChanNo, atMtInfoCtx->atInfo.bandWidth);

    if (atMtInfoCtx->atInfo.ratMode == AT_RAT_MODE_NR) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", atMtInfoCtx->atInfo.bbicScs);
    }

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 At_QryFTxonPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT16  length;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_QryFTxonPara Enter");

    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        atMtInfoCtx->atInfo.txOnOff);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 At_QryFRxonPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT16  length;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_QryFRxonPara Enter");

    /* ��ǰ��Ϊ������ģʽ */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ��ѯ��ǰ���ջ�����״̬ */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        atMtInfoCtx->atInfo.rxOnOff);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_QryFpowdetTPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("AT_QryFpowdetTPara Enter");

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��ǰ��Ϊ������ģʽ */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ��ǰ�Ƿ������������ý��뼼�� */
    if (atMtInfoCtx->bbicInfo.currtRatMode >= RAT_MODE_BUTT) {
        return AT_ERROR;
    }

    /* ��BBIC����Ϣ */
    if ((atMtInfoCtx->bbicInfo.currtRatMode == RAT_MODE_GSM) &&
        (atMtInfoCtx->atInfo.faveType != AT_FWAVE_TYPE_CONTINUE)) {
        if (At_SndGsmTxOnOffReq_ModulatedWave(VOS_TRUE) != VOS_TRUE) {
            return AT_SND_MSG_FAIL;
        }
    } else {
        if (At_SndTxOnOffReq(VOS_TRUE) != VOS_TRUE) {
            return AT_SND_MSG_FAIL;
        }
    }

    /* ���õ�ǰ�������� */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FPOWDET_QRY;
    atMtInfoCtx->atInfo.indexNum          = indexNum;

    /* ������������״̬ */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryFAgcgainPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("AT_QryFAgcgainPara Enter");

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /*  ���Ƿ�����ģʽ�·����ش��� */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            atMtInfoCtx->atInfo.agcGainLevel);

    return AT_OK;
}


VOS_UINT32 AT_QryFPllStatusPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  result;

    atMtInfoCtx = AT_GetMtInfoCtx();
    result      = AT_FAILURE;

    AT_PR_LOGH("AT_QryFPllStatusPara Enter");

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (atMtInfoCtx->atInfo.dspLoadFlag != VOS_TRUE) {
        return AT_ERROR;
    }

    result = AT_SndBbicPllStatusReq();

    /* ����ʧ�� */
    if (result != AT_SUCCESS) {
        AT_ERR_LOG("AT_QryFPllStatusPara: AT Snd ReqMsg fail.");
        return AT_ERROR;
    }

    /* ���õ�ǰ�������� */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FPLLSTATUS_QRY;
    atMtInfoCtx->atInfo.indexNum          = indexNum;

    /* ������������״̬ */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetFPllStatusQryPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  result;

    atMtInfoCtx = AT_GetMtInfoCtx();
    result      = AT_FAILURE;

    AT_PR_LOGH("AT_SetFPllStatusQryPara Enter");

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����������� */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (atMtInfoCtx->atInfo.dspLoadFlag != VOS_TRUE) {
        return AT_NOT_LOAD_DSP;
    }

    result = AT_SndBbicPllStatusReq();

    /* ����ʧ�� */
    if (result != AT_SUCCESS) {
        AT_ERR_LOG("AT_SetFPllStatusQryPara: AT Snd ReqMsg fail.");
        return AT_ERROR;
    }

    atMtInfoCtx->atInfo.antType = (AT_MT_AntTypeUint8)g_atParaList[AT_FPLL_STATUS_TX].paraValue;

    /* ���õ�ǰ�������� */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FPLLSTATUS_SET;
    atMtInfoCtx->atInfo.indexNum          = indexNum;

    /* ������������״̬ */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QryFrssiPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  result;

    atMtInfoCtx = AT_GetMtInfoCtx();
    result      = AT_FAILURE;

    AT_PR_LOGH("At_QryFrssiPara Enter");

    /* ���������ڷ�����ģʽ��ʹ�� */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* �������������÷������ŵ���ʹ�� */
    if (atMtInfoCtx->atInfo.dspLoadFlag == VOS_FALSE) {
        return AT_CHANNEL_NOT_SET;
    }

    /* ��������Ҫ�ڴ򿪽��ջ���ʹ�� */
    if (atMtInfoCtx->atInfo.rxOnOff != AT_DSP_RF_SWITCH_ON) {
        return AT_FRSSI_RX_NOT_OPEN;
    }

    result = AT_SndBbicRssiReq();

    /* ����ʧ�� */
    if (result != AT_SUCCESS) {
        AT_ERR_LOG("At_QryFrssiPara: AT Snd ReqMsg fail.");
        return AT_FRSSI_OTHER_ERR;
    }

    atMtInfoCtx->atInfo.indexNum          = indexNum;
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_QUERY_RSSI;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryTSelRfPara(VOS_UINT8 indexNum)
{
    VOS_UINT16 length;

    length = 0;

    AT_PR_LOGH("AT_QryTSelRfPara Enter");

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_TOTAL_MT, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_GSM, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_WCDMA_PRI, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_WCDMA_DIV, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_CDMA_PRI, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_CDMA_DIV, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_TD, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_WIFI, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_FDD_LTE_PRI, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_FDD_LTE_DIV, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_FDD_LTE_MIMO, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_TDD_LTE_PRI, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_TDD_LTE_DIV, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_TDD_LTE_MIMO, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_NR_PRI, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%s", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_NR_DIV, g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        AT_TSELRF_PATH_NR_MIMO);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 At_QryFpaPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT16  length;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_QryFpaPara Enter");

    /* ��ǰ��Ϊ������ģʽ */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ��ѯ��ǰ�����PA�ȼ������� */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        atMtInfoCtx->atInfo.paLevel);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_QryDcxotempcompPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("AT_QryDcxotempcompPara Enter");

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /*  ���Ƿ�����ģʽ�·����ش��� */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            atMtInfoCtx->atInfo.dcxoTempCompEnableFlg);

    return AT_OK;
}


VOS_UINT32 AT_QryFtemprptPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  rst;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("AT_QryFtemprptPara Enter");

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����������� */
    if (g_atParaIndex != AT_FTEMPRPT_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��¼ͨ��������Ϣ */
    if (At_CovertChannelTypeToBbicCal((AT_TEMP_ChannelTypeUint16)g_atParaList[0].paraValue,
                                      &atMtInfoCtx->bbicInfo.currentChannelType) == VOS_FALSE) {
        return AT_ERROR;
    }

    /* AT����Ftemprpt��ѯ��Ϣ��BBIC���� */
    rst = AT_SndBbicCalQryFtemprptReq((INT16)g_atParaList[1].paraValue);

    if (rst == AT_SUCCESS) {
        atMtInfoCtx->atInfo.indexNum          = indexNum;
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FTEMPRPT_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_CovertAtModeToBbicCal(AT_DEVICE_CmdRatModeUint8 atMode, RAT_MODE_ENUM_UINT16 *bbicMode)
{
    VOS_UINT32 result;

    result = VOS_TRUE;

    switch (atMode) {
        case AT_RAT_MODE_WCDMA:
        case AT_RAT_MODE_AWS:
            *bbicMode = RAT_MODE_WCDMA;
            break;

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
        case AT_RAT_MODE_CDMA:
            *bbicMode = RAT_MODE_CDMA;
            break;
#endif

        case AT_RAT_MODE_GSM:
        case AT_RAT_MODE_EDGE:
            *bbicMode = RAT_MODE_GSM;
            break;

#if (FEATURE_LTE == FEATURE_ON)
        case AT_RAT_MODE_FDD_LTE:
        case AT_RAT_MODE_TDD_LTE:
#if (FEATURE_LTEV == FEATURE_ON)
        case AT_RAT_MODE_LTEV:
#endif
            *bbicMode = RAT_MODE_LTE;
            break;
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
        case AT_RAT_MODE_NR:
            *bbicMode = RAT_MODE_NR;
            break;
#endif

        case AT_RAT_MODE_TDSCDMA: /* Ŀǰ��NR��оƬ��֧��TD */
        case AT_RAT_MODE_WIFI:
        default:
            *bbicMode = RAT_MODE_BUTT;
            result    = VOS_FALSE;
            break;
    }

    return result;
}


VOS_UINT32 At_CovertAtBandWidthToBbicCal(AT_BAND_WidthUint16 atBandWidth, BANDWIDTH_ENUM_UINT16 *bbicBandWidth)
{
    VOS_UINT32 indexNum;
    VOS_UINT32 bandWidthNo;

    bandWidthNo    = sizeof(g_bandWidthTable) / sizeof(g_bandWidthTable[0]);
    *bbicBandWidth = BANDWIDTH_BUTT;

    for (indexNum = 0; indexNum < bandWidthNo; indexNum++) {
        if (atBandWidth == g_bandWidthTable[indexNum].atBandWidth) {
            *bbicBandWidth = g_bandWidthTable[indexNum].bbicBandWidth;

            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 At_CovertAtBandWidthToValue(AT_BAND_WidthUint16 atBandWidth, AT_BAND_WidthValueUint32 *bandWidthValue)
{
    VOS_UINT32 indexNum;
    VOS_UINT32 bandWidthNo;

    bandWidthNo     = sizeof(g_bandWidthTable) / sizeof(g_bandWidthTable[0]);
    *bandWidthValue = AT_BAND_WIDTH_VALUE_BUTT;

    for (indexNum = 0; indexNum < bandWidthNo; indexNum++) {
        if (atBandWidth == g_bandWidthTable[indexNum].atBandWidth) {
            *bandWidthValue = g_bandWidthTable[indexNum].atBandWidthValue;

            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 At_CovertAtScsToBbicCal(AT_SubCarrierSpacingUint8 atScs, NR_SCS_TYPE_COMM_ENUM_UINT8 *bbicScs)
{
    VOS_UINT32 result;

    result = VOS_TRUE;

    switch (atScs) {
        case AT_SUB_CARRIER_SPACING_15K:
            *bbicScs = NR_SCS_TYPE_COMM_15;
            break;
        case AT_SUB_CARRIER_SPACING_30K:
            *bbicScs = NR_SCS_TYPE_COMM_30;
            break;
        case AT_SUB_CARRIER_SPACING_60K:
            *bbicScs = NR_SCS_TYPE_COMM_60;
            break;
        case AT_SUB_CARRIER_SPACING_120K:
            *bbicScs = NR_SCS_TYPE_COMM_120;
            break;
        case AT_SUB_CARRIER_SPACING_240K:
            *bbicScs = NR_SCS_TYPE_COMM_240;
            break;
        default:
            *bbicScs = NR_SCS_TYPE_COMM_BUTT;
            result   = VOS_FALSE;
            break;
    }

    return result;
}


VOS_UINT32 At_CovertRatModeToBbicCal(AT_CmdRatmodeUint8 ratMode, RAT_MODE_ENUM_UINT16 *bbicMode)
{
    VOS_UINT32 result;

    result = VOS_TRUE;

    switch (ratMode) {
        case AT_CMD_RATMODE_GSM:
            *bbicMode = RAT_MODE_GSM;
            break;

        case AT_CMD_RATMODE_WCDMA:
            *bbicMode = RAT_MODE_WCDMA;
            break;

        case AT_CMD_RATMODE_LTE:
            *bbicMode = RAT_MODE_LTE;
            break;

        case AT_CMD_RATMODE_CDMA:
            *bbicMode = RAT_MODE_CDMA;
            break;

        case AT_CMD_RATMODE_NR:
            *bbicMode = RAT_MODE_NR;
            break;

        case AT_CMD_RATMODE_TD: /* Ŀǰ��NR��оƬ��֧��TD */
        default:
            *bbicMode = RAT_MODE_BUTT;
            result    = VOS_FALSE;
            break;
    }

    return result;
}


VOS_UINT32 At_CovertChannelTypeToBbicCal(AT_TEMP_ChannelTypeUint16           channelType,
                                         BBIC_TEMP_CHANNEL_TYPE_ENUM_UINT16 *bbicChannelType)
{
    VOS_UINT32 result;

    result = VOS_TRUE;

    switch (channelType) {
        case AT_TEMP_CHANNEL_TYPE_LOGIC:
            *bbicChannelType = BBIC_TEMP_CHANNEL_TYPE_LOGIC;
            break;

        case AT_TEMP_CHANNEL_TYPE_PHY:
            *bbicChannelType = BBIC_TEMP_CHANNEL_TYPE_PHY;
            break;

        default:
            *bbicChannelType = BBIC_TEMP_CHANNEL_TYPE_BUTT;
            result           = VOS_FALSE;
            break;
    }

    return result;
}


VOS_UINT32 At_CovertAtPaLevelToBbicCal(AT_CmdPalevelUint8 atPaLevel, BBIC_CAL_PA_MODE_ENUM_UINT8 *bbicPaLevel)
{
    VOS_UINT32 result;

    result = VOS_TRUE;

    switch (atPaLevel) {
        case AT_CMD_PALEVEL_HIGH:
            *bbicPaLevel = BBIC_CAL_PA_GAIN_MODE_HIGH;
            break;

        case AT_CMD_PALEVEL_MID:
            *bbicPaLevel = BBIC_CAL_PA_GAIN_MODE_MID;
            break;

        case AT_CMD_PALEVEL_LOW:
            *bbicPaLevel = BBIC_CAL_PA_GAIN_MODE_LOW;
            break;

        case AT_CMD_PALEVEL_ULTRALOW:
            *bbicPaLevel = BBIC_CAL_PA_GAIN_MODE_ULTRA_LOW;
            break;

        default:
            *bbicPaLevel = BBIC_CAL_PA_GAIN_MODE_BUTT;
            result       = VOS_FALSE;
            break;
    }

    return result;
}


VOS_UINT32 AT_GetNrFreqOffset(VOS_UINT32 channelNo, AT_NR_FreqOffsetTable *nrFreqOffset)
{
    VOS_UINT32 indexNum;
    VOS_UINT32 freqOffsetNo;
    errno_t    memResult;

    freqOffsetNo = sizeof(g_atNrFreqOffsetTable) / sizeof(AT_NR_FreqOffsetTable);

    for (indexNum = 0; indexNum < freqOffsetNo; indexNum++) {
        if ((channelNo >= g_atNrFreqOffsetTable[indexNum].channelRange.channelMin) &&
            (channelNo <= g_atNrFreqOffsetTable[indexNum].channelRange.channelMax)) {
            break;
        }
    }

    if (indexNum == freqOffsetNo) {
        return VOS_FALSE;
    }

    memResult = memcpy_s(nrFreqOffset, sizeof(AT_NR_FreqOffsetTable), &g_atNrFreqOffsetTable[indexNum],
                         sizeof(g_atNrFreqOffsetTable[indexNum]));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_NR_FreqOffsetTable), sizeof(g_atNrFreqOffsetTable[indexNum]));

    return VOS_TRUE;
}


VOS_VOID AT_GetNrFreqFromUlChannelNo(VOS_UINT32 ulChannelNo, AT_NR_FreqOffsetTable *nrFreqOffset,
                                     AT_DSP_BandFreq *dspBandFreq, const AT_NR_BandInfo *bandInfo)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_INT32   detlFreq;
    VOS_UINT32  ulChanNoRange;
    VOS_UINT32  dlChanNoRange;

    atMtInfoCtx = AT_GetMtInfoCtx();

    /* ����Ƶ�� */
    dspBandFreq->ulFreq = nrFreqOffset->freqOffset +
                          nrFreqOffset->freqGlobal * (ulChannelNo - nrFreqOffset->offsetChannel);

    atMtInfoCtx->atInfo.bandArfcn.ulChanNo = ulChannelNo;

    ulChanNoRange = bandInfo->ulChannelRange.channelMax - bandInfo->ulChannelRange.channelMin;

    dlChanNoRange = bandInfo->dlChannelRange.channelMax - bandInfo->dlChannelRange.channelMin;

    /* �ж��������ŵ��Ƿ�Գƣ�������ԳƲ��ܼ�������Ƶ�� */
    if (ulChanNoRange != dlChanNoRange) {
        return;
    }

    /* �����е�Ƶ��� */
    detlFreq = (VOS_INT32)(bandInfo->dlFreqRange.freqMin - bandInfo->ulFreqRange.freqMin);

    /* ����Ƶ�� */
    dspBandFreq->dlFreq = dspBandFreq->ulFreq + detlFreq;

    atMtInfoCtx->atInfo.bandArfcn.dlChanNo = bandInfo->dlChannelRange.channelMin + ulChannelNo -
                                             bandInfo->ulChannelRange.channelMin;
    return;
}


VOS_VOID AT_GetNrFreqFromDlChannelNo(VOS_UINT32 dlChannelNo, AT_NR_FreqOffsetTable *nrFreqOffset,
                                     AT_DSP_BandFreq *dspBandFreq, const AT_NR_BandInfo *bandInfo)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_INT32   detlFreq;
    VOS_UINT32  ulChanNoRange;
    VOS_UINT32  dlChanNoRange;

    atMtInfoCtx = AT_GetMtInfoCtx();

    /* ����Ƶ�� */
    dspBandFreq->dlFreq = nrFreqOffset->freqOffset +
                          nrFreqOffset->freqGlobal * (dlChannelNo - nrFreqOffset->offsetChannel);
    atMtInfoCtx->atInfo.bandArfcn.dlChanNo = dlChannelNo;

    ulChanNoRange = bandInfo->ulChannelRange.channelMax - bandInfo->ulChannelRange.channelMin;

    dlChanNoRange = bandInfo->dlChannelRange.channelMax - bandInfo->dlChannelRange.channelMin;

    /* �ж��������ŵ��Ƿ�Գƣ�������ԳƲ��ܼ�������Ƶ�� */
    if (ulChanNoRange != dlChanNoRange) {
        return;
    }

    /* ��ͬ�ŵ��ϵ�Ƶ��� */
    detlFreq = (VOS_INT32)(bandInfo->dlFreqRange.freqMin - bandInfo->ulFreqRange.freqMin);

    /* ����Ƶ�� */
    dspBandFreq->ulFreq = dspBandFreq->dlFreq - detlFreq;

    atMtInfoCtx->atInfo.bandArfcn.ulChanNo = bandInfo->ulChannelRange.channelMin + dlChannelNo -
                                             bandInfo->dlChannelRange.channelMin;
    return;
}


VOS_UINT32 AT_GetNrFreq(VOS_UINT32 channelNo)
{
    AT_DSP_BandFreq      *dspBandFreq = VOS_NULL_PTR;
    AT_MT_Info           *atMtInfoCtx = VOS_NULL_PTR;
    AT_NR_FreqOffsetTable nrFreqOffset;
    VOS_UINT32            indexNum;
    VOS_UINT32            supportBandNo;

    atMtInfoCtx   = AT_GetMtInfoCtx();
    dspBandFreq   = &atMtInfoCtx->bbicInfo.dspBandFreq;
    supportBandNo = sizeof(g_atNrBandInfoTable) / sizeof(AT_NR_BandInfo);

    for (indexNum = 0; indexNum < supportBandNo; indexNum++) {
        if (g_atNrBandInfoTable[indexNum].bandNo == atMtInfoCtx->atInfo.bandArfcn.dspBand) {
            break;
        }
    }

    /* Band��ƥ�� */
    if (indexNum == supportBandNo) {
        return VOS_FALSE;
    }

    dspBandFreq->dspBand = atMtInfoCtx->atInfo.bandArfcn.dspBand;

    /* ��ȡƵ�ʵ�ƫ����,��λKHZ */
    if (AT_GetNrFreqOffset(channelNo, &nrFreqOffset) == VOS_FALSE) {
        return VOS_FALSE;
    }

    /* �жϹ����·����Ƿ��������ŵ��� */
    if ((channelNo >= g_atNrBandInfoTable[indexNum].ulChannelRange.channelMin) &&
        (channelNo <= g_atNrBandInfoTable[indexNum].ulChannelRange.channelMax)) {
        AT_GetNrFreqFromUlChannelNo(channelNo, &nrFreqOffset, dspBandFreq, &g_atNrBandInfoTable[indexNum]);
    }
    /* �жϹ����·����Ƿ��������ŵ��� */
    else if ((channelNo >= g_atNrBandInfoTable[indexNum].dlChannelRange.channelMin) &&
             (channelNo <= g_atNrBandInfoTable[indexNum].dlChannelRange.channelMax)) {
        AT_GetNrFreqFromDlChannelNo(channelNo, &nrFreqOffset, dspBandFreq, &g_atNrBandInfoTable[indexNum]);
    } else {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_VOID AT_GetLteFreqFromUlChannelNo(VOS_UINT32 ulChannelNo, AT_DSP_BandFreq *dspBandFreq,
                                      const AT_LTE_BandInfo *bandInfo)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  dlChannelNo;
    VOS_UINT32  ulChanNoRange;
    VOS_UINT32  dlChanNoRange;

    atMtInfoCtx = AT_GetMtInfoCtx();

    dspBandFreq->ulFreq = bandInfo->ulLowFreq + ulChannelNo * LTE_CHANNEL_TO_FREQ_UNIT -
                          bandInfo->ulChannelOffset * LTE_CHANNEL_TO_FREQ_UNIT;
    atMtInfoCtx->atInfo.bandArfcn.ulChanNo = ulChannelNo;

    ulChanNoRange = bandInfo->ulChannelRange.channelMax - bandInfo->ulChannelRange.channelMin;

    dlChanNoRange = bandInfo->dlChannelRange.channelMax - bandInfo->dlChannelRange.channelMin;

    /* �ж��������ŵ��Ƿ�Գƣ�������ԳƲ��ܼ�������Ƶ�� */
    if (ulChanNoRange != dlChanNoRange) {
        return;
    }

    dlChannelNo = bandInfo->dlChannelRange.channelMin + ulChannelNo - bandInfo->ulChannelOffset;

    dspBandFreq->dlFreq = bandInfo->dlLowFreq + dlChannelNo * LTE_CHANNEL_TO_FREQ_UNIT -
                          bandInfo->dlChannelOffset * LTE_CHANNEL_TO_FREQ_UNIT;

    atMtInfoCtx->atInfo.bandArfcn.dlChanNo = dlChannelNo;

    return;
}


VOS_VOID AT_GetLteFreqFromDlChannelNo(VOS_UINT32 dlChannelNo, AT_DSP_BandFreq *dspBandFreq,
                                      const AT_LTE_BandInfo *bandInfo)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  ulChanNoRange;
    VOS_UINT32  dlChanNoRange;
    VOS_UINT32  ulChannelNo;

    atMtInfoCtx = AT_GetMtInfoCtx();

    dspBandFreq->dlFreq = bandInfo->dlLowFreq + dlChannelNo * LTE_CHANNEL_TO_FREQ_UNIT -
                          bandInfo->dlChannelOffset * LTE_CHANNEL_TO_FREQ_UNIT;
    atMtInfoCtx->atInfo.bandArfcn.dlChanNo = dlChannelNo;

    ulChanNoRange = bandInfo->ulChannelRange.channelMax - bandInfo->ulChannelRange.channelMin;

    dlChanNoRange = bandInfo->dlChannelRange.channelMax - bandInfo->dlChannelRange.channelMin;

    /* �ж��������ŵ��Ƿ�Գƣ�������ԳƲ��ܼ�������Ƶ�� */
    if (ulChanNoRange != dlChanNoRange) {
        return;
    }

    ulChannelNo = bandInfo->ulChannelRange.channelMin + dlChannelNo - bandInfo->dlChannelOffset;

    dspBandFreq->ulFreq = bandInfo->ulLowFreq + ulChannelNo * LTE_CHANNEL_TO_FREQ_UNIT -
                          bandInfo->ulChannelOffset * LTE_CHANNEL_TO_FREQ_UNIT;

    atMtInfoCtx->atInfo.bandArfcn.ulChanNo = ulChannelNo;

    return;
}


VOS_UINT32 AT_GetLteFreq(VOS_UINT32 channelNo)
{
    AT_DSP_BandFreq *dspBandFreq = VOS_NULL_PTR;
    AT_MT_Info      *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32       indexNum;
    VOS_UINT32       supportBandNo;
    VOS_UINT32       result;

    atMtInfoCtx   = AT_GetMtInfoCtx();
    result        = VOS_TRUE;
    dspBandFreq   = &atMtInfoCtx->bbicInfo.dspBandFreq;
    supportBandNo = sizeof(g_atLteBandInfoTable) / sizeof(AT_LTE_BandInfo);

    /* Band�Ų�ѯ */
    for (indexNum = 0; indexNum < supportBandNo; indexNum++) {
        if (g_atLteBandInfoTable[indexNum].bandNo == atMtInfoCtx->atInfo.bandArfcn.dspBand) {
            break;
        }
    }

    if (indexNum == supportBandNo) {
        return VOS_FALSE;
    }

    dspBandFreq->dspBand = atMtInfoCtx->atInfo.bandArfcn.dspBand;

    /* �жϹ����·����Ƿ��������ŵ��� */
    if ((channelNo >= g_atLteBandInfoTable[indexNum].ulChannelRange.channelMin) &&
        (channelNo <= g_atLteBandInfoTable[indexNum].ulChannelRange.channelMax)) {
        AT_GetLteFreqFromUlChannelNo(channelNo, dspBandFreq, &g_atLteBandInfoTable[indexNum]);
    }
    /* �жϹ����·����Ƿ��������ŵ��� */
    else if ((channelNo >= g_atLteBandInfoTable[indexNum].dlChannelRange.channelMin) &&
             (channelNo <= g_atLteBandInfoTable[indexNum].dlChannelRange.channelMax)) {
        AT_GetLteFreqFromDlChannelNo(channelNo, dspBandFreq, &g_atLteBandInfoTable[indexNum]);
    } else {
        result = VOS_FALSE;
    }

    return result;
}


VOS_UINT32 AT_GetWFreqFromUlChannelNo(VOS_UINT32 ulChannelNo, AT_DSP_BandFreq *dspBandFreq,
                                      const AT_W_BandInfo *bandInfo)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    dspBandFreq->ulFreq = ulChannelNo * FREQ_UNIT_MHZ_TO_KHZ / W_CHANNEL_FREQ_TIMES + bandInfo->ulFreqOffset;

    dspBandFreq->dlFreq = dspBandFreq->ulFreq + bandInfo->dlFreqRange.freqMin - bandInfo->ulFreqRange.freqMin;

    atMtInfoCtx->atInfo.bandArfcn.ulChanNo = ulChannelNo;
    atMtInfoCtx->atInfo.bandArfcn.dlChanNo = bandInfo->dlChannelRange.channelMin + ulChannelNo -
                                             bandInfo->ulChannelRange.channelMin;

    return VOS_TRUE;
}


VOS_UINT32 AT_GetWFreqFromDlChannelNo(VOS_UINT32 dlChannelNo, AT_DSP_BandFreq *dspBandFreq,
                                      const AT_W_BandInfo *bandInfo)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    dspBandFreq->dlFreq = dlChannelNo * FREQ_UNIT_MHZ_TO_KHZ / W_CHANNEL_FREQ_TIMES + bandInfo->dlFreqOffset;

    dspBandFreq->ulFreq = dspBandFreq->dlFreq + bandInfo->ulFreqRange.freqMin - bandInfo->dlFreqRange.freqMin;

    atMtInfoCtx->atInfo.bandArfcn.dlChanNo = dlChannelNo;
    atMtInfoCtx->atInfo.bandArfcn.ulChanNo = bandInfo->ulChannelRange.channelMin + dlChannelNo -
                                             bandInfo->dlChannelRange.channelMin;

    return VOS_TRUE;
}


VOS_UINT32 AT_GetWFreq(VOS_UINT32 channelNo)
{
    AT_DSP_BandFreq *dspBandFreq = VOS_NULL_PTR;
    AT_MT_Info      *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32       indexNum;
    VOS_UINT32       supportBandNo;
    VOS_UINT32       result;

    atMtInfoCtx   = AT_GetMtInfoCtx();
    result        = VOS_TRUE;
    dspBandFreq   = &atMtInfoCtx->bbicInfo.dspBandFreq;
    supportBandNo = sizeof(g_atWBandInfoTable) / sizeof(AT_W_BandInfo);

    /*  �жϵ�ǰband�Ƿ���֧�ֵ�band */
    for (indexNum = 0; indexNum < supportBandNo; indexNum++) {
        if (atMtInfoCtx->atInfo.bandArfcn.dspBand == g_atWBandInfoTable[indexNum].bandNo) {
            break;
        }
    }

    if (indexNum == supportBandNo) {
        return VOS_FALSE;
    }

    dspBandFreq->dspBand = atMtInfoCtx->atInfo.bandArfcn.dspBand;

    /* �жϹ����·����Ƿ��������ŵ��� */
    if ((channelNo >= g_atWBandInfoTable[indexNum].ulChannelRange.channelMin) &&
        (channelNo <= g_atWBandInfoTable[indexNum].ulChannelRange.channelMax)) {
        AT_GetWFreqFromUlChannelNo(channelNo, dspBandFreq, &g_atWBandInfoTable[indexNum]);
    }
    /* �жϹ����·����Ƿ��������ŵ��� */
    else if ((channelNo >= g_atWBandInfoTable[indexNum].dlChannelRange.channelMin) &&
             (channelNo <= g_atWBandInfoTable[indexNum].dlChannelRange.channelMax)) {
        AT_GetWFreqFromDlChannelNo(channelNo, dspBandFreq, &g_atWBandInfoTable[indexNum]);
    } else {
        result = VOS_FALSE;
    }

    return result;
}


VOS_UINT32 AT_GetGFreq(VOS_UINT32 channelNo)
{
    AT_DSP_BandFreq *dspBandFreq = VOS_NULL_PTR;
    AT_MT_Info      *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32       indexNum;
    VOS_UINT32       supportBandNo;

    atMtInfoCtx = AT_GetMtInfoCtx();
    dspBandFreq = &atMtInfoCtx->bbicInfo.dspBandFreq;

    supportBandNo = sizeof(g_atGBandInfoTable) / sizeof(AT_G_BandInfo);

    /*  �жϵ�ǰband�Ƿ���֧�ֵ�band */
    for (indexNum = 0; indexNum < supportBandNo; indexNum++) {
        if (atMtInfoCtx->atInfo.bandArfcn.dspBand == g_atGBandInfoTable[indexNum].bandNo) {
            break;
        }
    }

    if (indexNum == supportBandNo) {
        return VOS_FALSE;
    }

    dspBandFreq->dspBand = atMtInfoCtx->atInfo.bandArfcn.dspBand;

    /*  �ж��ŵ��Ƿ��ڷ�Χ�� */
    if ((channelNo < g_atGBandInfoTable[indexNum].channelRange.channelMin) ||
        (channelNo > g_atGBandInfoTable[indexNum].channelRange.channelMax)) {
        return VOS_FALSE;
    }

    /* GSM 900���ŵ���Χ�Ƚ϶� */
    if (dspBandFreq->dspBand == AT_PROTOCOL_BAND_8) {
        /* Fu����890000 + 200*N */
        if (channelNo <= G_CHANNEL_NO_124) {
            dspBandFreq->ulFreq = 890000 + 200 * channelNo;
            dspBandFreq->dlFreq = dspBandFreq->ulFreq + g_atGBandInfoTable[indexNum].freqOffset;

            atMtInfoCtx->atInfo.bandArfcn.dlChanNo = channelNo;
            atMtInfoCtx->atInfo.bandArfcn.ulChanNo = channelNo;

            return VOS_TRUE;
        }

        /* Fu����890000 + 200*(N - 1024) */
        if ((channelNo >= G_CHANNEL_NO_955) && (channelNo <= G_CHANNEL_NO_1023)) {
            dspBandFreq->ulFreq = 890000 + 200 * channelNo - 200 * 1024;
            dspBandFreq->dlFreq = dspBandFreq->ulFreq + g_atGBandInfoTable[indexNum].freqOffset;

            atMtInfoCtx->atInfo.bandArfcn.dlChanNo = channelNo;
            atMtInfoCtx->atInfo.bandArfcn.ulChanNo = channelNo;

            return VOS_TRUE;
        }
    }

    /* GSM 1800���ŵ���Χ�Ƚ϶� */
    if (dspBandFreq->dspBand == AT_PROTOCOL_BAND_3) {
        /* Fu����1710200 + 200*(N - 512) */
        dspBandFreq->ulFreq = 1710200 + 200 * channelNo - 200 * 512;
        dspBandFreq->dlFreq = dspBandFreq->ulFreq + g_atGBandInfoTable[indexNum].freqOffset;

        atMtInfoCtx->atInfo.bandArfcn.dlChanNo = channelNo;
        atMtInfoCtx->atInfo.bandArfcn.ulChanNo = channelNo;
        return VOS_TRUE;
    }

    /* GSM 1900���ŵ���Χ�Ƚ϶� */
    if (dspBandFreq->dspBand == AT_PROTOCOL_BAND_2) {
        /* Fu����1850200 + 200*(N - 512) */
        dspBandFreq->ulFreq                    = 1850200 + 200 * channelNo - 200 * 512;
        dspBandFreq->dlFreq                    = dspBandFreq->ulFreq + g_atGBandInfoTable[indexNum].freqOffset;
        atMtInfoCtx->atInfo.bandArfcn.dlChanNo = channelNo;
        atMtInfoCtx->atInfo.bandArfcn.ulChanNo = channelNo;
        return VOS_TRUE;
    }

    /* GSM 850���ŵ���Χ�Ƚ϶� */
    if (dspBandFreq->dspBand == AT_PROTOCOL_BAND_5) {
        /* Fu����824200 + 200*(N - 128) */
        dspBandFreq->ulFreq                    = 824200 + 200 * channelNo - 200 * 128;
        dspBandFreq->dlFreq                    = dspBandFreq->ulFreq + g_atGBandInfoTable[indexNum].freqOffset;
        atMtInfoCtx->atInfo.bandArfcn.dlChanNo = channelNo;
        atMtInfoCtx->atInfo.bandArfcn.ulChanNo = channelNo;
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 AT_GetCFreq(VOS_UINT32 channelNo)
{
    AT_DSP_BandFreq *dspBandFreq = VOS_NULL_PTR;
    AT_MT_Info      *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();
    dspBandFreq = &atMtInfoCtx->bbicInfo.dspBandFreq;

    /* ��ǰֻ֧��Band Class 0,Band5 */
    if (atMtInfoCtx->atInfo.bandArfcn.dspBand != AT_PROTOCOL_BAND_5) {
        return VOS_FALSE;
    }

    dspBandFreq->dspBand = atMtInfoCtx->atInfo.bandArfcn.dspBand;

    if ((channelNo >= C_CHANNEL_NO_1) && (channelNo <= C_CHANNEL_NO_799)) {
        /* ���㹫ʽ: Fu = 30N + 825000 */
        dspBandFreq->ulFreq = 30 * channelNo + 825000;

        /* ���㹫ʽ: Fd = 30N + 870000 */
        dspBandFreq->dlFreq = 30 * channelNo + 870000;

        atMtInfoCtx->atInfo.bandArfcn.dlChanNo = channelNo;
        atMtInfoCtx->atInfo.bandArfcn.ulChanNo = channelNo;

        return VOS_TRUE;
    }

    if ((channelNo >= C_CHANNEL_NO_991) && (channelNo <= C_CHANNEL_NO_1023)) {
        /* ���㹫ʽ: Fu = 30(N - 1023) + 825000 */
        dspBandFreq->ulFreq = 825000 - 30 * (1023 - channelNo);

        /* ���㹫ʽ: Fd = 30(N -1023) + 870000 */
        dspBandFreq->dlFreq = 870000 - 30 * (1023 - channelNo);

        atMtInfoCtx->atInfo.bandArfcn.dlChanNo = channelNo;
        atMtInfoCtx->atInfo.bandArfcn.ulChanNo = channelNo;

        return VOS_TRUE;
    }

    if ((channelNo >= C_CHANNEL_NO_1024) && (channelNo <= C_CHANNEL_NO_1323)) {
        /* ���㹫ʽ: Fu = 30(N - 1024) + 815040 */
        dspBandFreq->ulFreq = 30 * (channelNo - 1024) + 815040;

        /* ���㹫ʽ: Fd = 30(N -1024) + 860040 */
        dspBandFreq->dlFreq = 30 * (channelNo - 1024) + 860040;

        atMtInfoCtx->atInfo.bandArfcn.dlChanNo = channelNo;
        atMtInfoCtx->atInfo.bandArfcn.ulChanNo = channelNo;

        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 AT_GetFreq(VOS_UINT32 channelNo)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  result;

    atMtInfoCtx = AT_GetMtInfoCtx();
    result      = VOS_FALSE;

    switch (atMtInfoCtx->bbicInfo.currtRatMode) {
        case RAT_MODE_GSM:
            result = AT_GetGFreq(channelNo);
            break;

        case RAT_MODE_WCDMA:
            result = AT_GetWFreq(channelNo);
            break;
        case RAT_MODE_CDMA:
            result = AT_GetCFreq(channelNo);
            break;

        case RAT_MODE_LTE:
            result = AT_GetLteFreq(channelNo);
            break;

        case RAT_MODE_NR:
            result = AT_GetNrFreq(channelNo);
            break;

        default:
            result = VOS_FALSE;
            break;
    }

    return result;
}


VOS_UINT32 AT_CheckNrFwaveTypePara(VOS_UINT32 para)
{
    VOS_UINT32 result;

    result = VOS_TRUE;
    switch (para) {
        case AT_FWAVE_TYPE_BPSK:
        case AT_FWAVE_TYPE_PI2_BPSK:
        case AT_FWAVE_TYPE_QPSK:
        case AT_FWAVE_TYPE_16QAM:
        case AT_FWAVE_TYPE_64QAM:
        case AT_FWAVE_TYPE_256QAM:
        case AT_FWAVE_TYPE_CONTINUE:
            result = VOS_TRUE;
            break;
        default:
            result = VOS_FALSE;
            break;
    }

    return result;
}


VOS_UINT32 AT_CheckLteFwaveTypePara(VOS_UINT32 para)
{
    VOS_UINT32 result;

    result = VOS_TRUE;
    switch (para) {
        case AT_FWAVE_TYPE_QPSK:
        case AT_FWAVE_TYPE_16QAM:
        case AT_FWAVE_TYPE_64QAM:
        case AT_FWAVE_TYPE_256QAM:
        case AT_FWAVE_TYPE_CONTINUE:
            result = VOS_TRUE;
            break;
        default:
            result = VOS_FALSE;
            break;
    }

    return result;
}


VOS_UINT32 AT_CheckCFwaveTypePara(VOS_UINT32 para)
{
    VOS_UINT32 result;

    result = VOS_TRUE;
    switch (para) {
        case AT_FWAVE_TYPE_QPSK:
        case AT_FWAVE_TYPE_CONTINUE:
            result = VOS_TRUE;
            break;
        default:
            result = VOS_FALSE;
            break;
    }

    return result;
}


VOS_UINT32 AT_CheckWFwaveTypePara(VOS_UINT32 para)
{
    VOS_UINT32 result;

    result = VOS_TRUE;
    switch (para) {
        case AT_FWAVE_TYPE_QPSK:
        case AT_FWAVE_TYPE_CONTINUE:
            result = VOS_TRUE;
            break;
        default:
            result = VOS_FALSE;
            break;
    }

    return result;
}


VOS_UINT32 AT_CheckGFwaveTypePara(VOS_UINT32 para)
{
    VOS_UINT32 result;

    result = VOS_TRUE;
    switch (para) {
        case AT_FWAVE_TYPE_GMSK:
        case AT_FWAVE_TYPE_8PSK:
        case AT_FWAVE_TYPE_CONTINUE:
            result = VOS_TRUE;
            break;
        default:
            result = VOS_FALSE;
            break;
    }

    return result;
}


VOS_UINT32 AT_CheckFwaveTypePara(VOS_UINT32 para)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  result;

    atMtInfoCtx = AT_GetMtInfoCtx();
    result      = VOS_TRUE;
    switch (atMtInfoCtx->bbicInfo.currtRatMode) {
        case RAT_MODE_GSM:
            result = AT_CheckGFwaveTypePara(para);
            break;
        case RAT_MODE_WCDMA:
            result = AT_CheckWFwaveTypePara(para);
            break;
        case RAT_MODE_CDMA:
            result = AT_CheckCFwaveTypePara(para);
            break;
        case RAT_MODE_LTE:
            result = AT_CheckLteFwaveTypePara(para);
            break;
        case RAT_MODE_NR:
            result = AT_CheckNrFwaveTypePara(para);
            break;
        default:
            result = VOS_FALSE;
            break;
    }

    return result;
}


VOS_UINT32 AT_CovertAtFwaveTypeToBbicCal(AT_FWAVE_TypeUint8 typeIn, MODU_TYPE_ENUM_UINT16 *typeOut)
{
    VOS_UINT32 result;

    result = VOS_TRUE;
    switch (typeIn) {
        case AT_FWAVE_TYPE_BPSK:
            *typeOut = MODU_TYPE_LTE_BPSK;
            break;
        case AT_FWAVE_TYPE_PI2_BPSK:
            *typeOut = MODU_TYPE_LTE_PI2_BPSK;
            break;
        case AT_FWAVE_TYPE_QPSK:
            *typeOut = MODU_TYPE_LTE_QPSK;
            break;
        case AT_FWAVE_TYPE_16QAM:
            *typeOut = MODU_TYPE_LTE_16QAM;
            break;
        case AT_FWAVE_TYPE_64QAM:
            *typeOut = MODU_TYPE_LTE_64QAM;
            break;
        case AT_FWAVE_TYPE_256QAM:
            *typeOut = MODU_TYPE_LTE_256QAM;
            break;
        case AT_FWAVE_TYPE_GMSK:
            *typeOut = MODU_TYPE_GMSK;
            break;
        case AT_FWAVE_TYPE_8PSK:
            *typeOut = MODU_TYPE_8PSK;
            break;
        /* BBICû�е������壬���Ը�ֵΪBUTT */
        case AT_FWAVE_TYPE_CONTINUE:
            *typeOut = MODU_TYPE_BUTT;
            break;
        default:
            result = VOS_FALSE;
            break;
    }

    return result;
}


VOS_UINT32 AT_CoverTselPathToPriOrDiv(AT_TSELRF_PathUint32 tselPath, AT_AntTypeUint8 *antType)
{
    VOS_UINT32 pathNo;
    VOS_UINT32 indexNum;

    pathNo = sizeof(g_path2AntTypeTable) / sizeof(AT_PATH_ToAntType);

    for (indexNum = 0; indexNum < pathNo; indexNum++) {
        if (tselPath == g_path2AntTypeTable[indexNum].tselPath) {
            *antType = g_path2AntTypeTable[indexNum].antType;

            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 At_GetBaseFreq(RAT_MODE_ENUM_UINT16 ratMode)
{
    VOS_UINT32 baseFreq;
    switch (ratMode) {
        case RAT_MODE_GSM:
            baseFreq = AT_RAT_MODE_GSM_BASE_FREQ;
            break;

        case RAT_MODE_WCDMA:
            baseFreq = AT_RAT_MODE_WCDMA_BASE_FREQ;
            break;

        case RAT_MODE_CDMA:
            baseFreq = AT_RAT_MODE_CDMA_BASE_FREQ;
            break;

        case RAT_MODE_LTE:
            baseFreq = AT_RAT_MODE_LTE_BASE_FREQ;
            break;

        case RAT_MODE_NR:
            baseFreq = AT_RAT_MODE_NR_BASE_FREQ;
            break;

        default:
            baseFreq = AT_RAT_MODE_DEAFAULT_BASE_FREQ;
            break;
    }

    return baseFreq;
}


VOS_UINT32 At_CheckTrxTasFtmPara(VOS_VOID)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();
    if (atMtInfoCtx->atInfo.txOnOff != AT_DSP_RF_SWITCH_ON) {
        return AT_TRXTAS_TX_NOT_SWITCH_ON;
    }

    /* ��������TRXTASֻ������ */
    if (g_atParaList[AT_TRXTAS_CMD].paraValue != AT_TRXTAS_CMD_SET) {
        return AT_TRXTAS_CMD_PARA_ERR;
    }

    /* ��¼���Խ��뼼��ģʽ��Ϣ */
    if (At_CovertRatModeToBbicCal((AT_CmdRatmodeUint8)g_atParaList[AT_TRXTAS_RAT_MODE].paraValue,
                                  &atMtInfoCtx->bbicInfo.trxTasRatMode) == VOS_FALSE) {
        return AT_TRXTAS_RAT_ERR;
    }

    /* �ж�^TRXTAS������^Fchanʱ�����RAT�Ƿ�һ�� */
    if (atMtInfoCtx->bbicInfo.trxTasRatMode != atMtInfoCtx->bbicInfo.currtRatMode) {
        return AT_TRXTAS_FCHAN_RAT_NOT_MATCH;
    }

    return AT_SUCCESS;
}


VOS_UINT32 At_SetTrxTasPara(VOS_UINT8 indexNum)
{
    AT_MT_Info         *atMtInfoCtx = VOS_NULL_PTR;
    AT_MTA_SetTrxTasReq setTrxTasCmd;
    VOS_UINT32          rst;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_SetTrxTasPara Enter");

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ������� */
    if (g_atParaIndex < AT_TRXTAS_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���������������Ҫ�·����ĸ����� */
    if ((g_atParaList[AT_TRXTAS_CMD].paraValue == 1) && (g_atParaList[AT_TRXTAS_DPDT_VALUE].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����ģʽ */
    if (g_atParaList[AT_TRXTAS_MODE].paraValue == AT_MTA_CMD_SIGNALING_MODE) {
        /* AT������MTA����Ϣ�ṹ��ֵ */
        (VOS_VOID)memset_s(&setTrxTasCmd, sizeof(setTrxTasCmd), 0x00, sizeof(setTrxTasCmd));
        setTrxTasCmd.ratMode     = (AT_MTA_CmdRatmodeUint8 )g_atParaList[AT_TRXTAS_RAT_MODE].paraValue;
        setTrxTasCmd.cmd         = (AT_MTA_TrxTasCmdUint8 )g_atParaList[AT_TRXTAS_CMD].paraValue;
        setTrxTasCmd.mode        = (AT_MTA_CmdSignalingUint8 )g_atParaList[AT_TRXTAS_MODE].paraValue;
        setTrxTasCmd.trxTasValue = g_atParaList[AT_TRXTAS_DPDT_VALUE].paraValue;

        /* ������Ϣ��C�˴��� */
        rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_SET_TRX_TAS_REQ,
                                     (VOS_UINT8 *)&setTrxTasCmd, sizeof(setTrxTasCmd), I0_UEPS_PID_MTA);
    }
    /* ������ģʽ */
    else {
        rst = At_CheckTrxTasFtmPara();
        if (rst != AT_SUCCESS) {
            return rst;
        }
        atMtInfoCtx->atInfo.indexNum = indexNum;
        rst = At_SndBbicCalSetTrxTasReq((VOS_UINT16)g_atParaList[AT_TRXTAS_DPDT_VALUE].paraValue);
    }

    if (rst == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_TRX_TAS_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_QryTrxTasPara(VOS_UINT8 indexNum)
{
    AT_MTA_QryTrxTasReq qryTrxTas;
    VOS_UINT32          rst;

    AT_PR_LOGH("At_SetQryTrxTasPara Enter");

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ������� */
    if (g_atParaIndex != AT_TRXTASQRY_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����ģʽ */
    if (g_atParaList[AT_TRXTAS_QRY_MODE].paraValue == AT_MTA_CMD_SIGNALING_MODE) {
        /* AT������MTA����Ϣ�ṹ��ֵ */
        (VOS_VOID)memset_s(&qryTrxTas, sizeof(qryTrxTas), 0x00, sizeof(qryTrxTas));
        qryTrxTas.mode    = (AT_MTA_CmdSignalingUint8)g_atParaList[AT_TRXTAS_QRY_MODE].paraValue;
        qryTrxTas.ratMode = (AT_MTA_CmdRatmodeUint8)g_atParaList[AT_TRXTAS_QRY_RAT_MODE].paraValue;

        /* ������Ϣ��C�˴��� */
        rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_QRY_TRX_TAS_REQ,
                                     (VOS_UINT8 *)&qryTrxTas, sizeof(qryTrxTas), I0_UEPS_PID_MTA);
    }
    /* ������ģʽ */
    else {
        return AT_CME_OPERATION_NOT_SUPPORTED;
    }

    if (rst == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_TRX_TAS_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT16 AT_GetGsmUlPathByBandNo(VOS_UINT16 bandNo)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  totalNum;
    VOS_UINT32  indexNum;
    VOS_UINT16  ulPath;

    atMtInfoCtx = AT_GetMtInfoCtx();
    ulPath      = 0;
    totalNum    = sizeof(g_atGBandInfoTable) / sizeof(AT_G_BandInfo);
    for (indexNum = 0; indexNum < totalNum; indexNum++) {
        if (g_atGBandInfoTable[indexNum].bandNo == atMtInfoCtx->bbicInfo.dspBandFreq.dspBand) {
            ulPath = g_atGBandInfoTable[indexNum].ulPath;
        }
    }

    return ulPath;
}

VOS_UINT32 At_SetRfIcMemTest(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  result;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_SetRfIcMemTest Enter");

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (atMtInfoCtx->atInfo.rficTestResult == AT_RFIC_MEM_TEST_RUNNING) {
        return AT_ERROR;
    }

    if (atMtInfoCtx->atInfo.dspLoadFlag != VOS_TRUE) {
        return AT_NOT_LOAD_DSP;
    }

    /* ��ʼ��ȫ�ֱ��� */
    atMtInfoCtx->atInfo.rficTestResult = AT_RFIC_MEM_TEST_NOT_START;

    /* ֱ�Ӹ�BBIC������Ϣ */
    result = At_SndUeCbtRfIcMemTestReq();
    if (result == AT_SUCCESS) {
        atMtInfoCtx->atInfo.rficTestResult = AT_RFIC_MEM_TEST_RUNNING;
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_QryRfIcMemTest(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_QryRfIcMemTest Enter");

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            atMtInfoCtx->atInfo.rficTestResult);
    /* ���ײ��Ѿ����أ����ѯ����Ҫ������ */
    if (atMtInfoCtx->atInfo.rficTestResult != AT_RFIC_MEM_TEST_RUNNING) {
        atMtInfoCtx->atInfo.rficTestResult = AT_RFIC_MEM_TEST_NOT_START;
    }

    return AT_OK;
}


VOS_UINT32 At_SetFSerdesRt(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  result;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_SetFSerdesRt Enter");

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (atMtInfoCtx->rserTestResult == (VOS_INT32)AT_SERDES_TEST_RUNNING) {
        AT_PR_LOGE("At_SetFSerdesRt:The Running");
        return AT_ERROR;
    }

    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        AT_PR_LOGE("At_SetFSerdesRt:Model Error");
        return AT_DEVICE_MODE_ERROR;
    }

    if (g_atParaIndex < AT_FSERDESRT_PARA_NUM) {
        AT_PR_LOGE("At_SetFSerdesRt:Parameter Error");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    atMtInfoCtx->rserTestResult = (VOS_INT32)AT_SERDES_TEST_NOT_START;

    /* ֱ�Ӹ�BBIC������Ϣ */
    result = At_SndDspIdleSerdesRtReq();
    if (result == AT_SUCCESS) {
        atMtInfoCtx->rserTestResult = (VOS_INT32)AT_SERDES_TEST_RUNNING;
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_QryFSerdesRt(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_QryFSerdesRt Enter");

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            atMtInfoCtx->rserTestResult);

    return AT_OK;
}


VOS_UINT32 At_SetSerdesAsyncTest(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  result;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_SetSerdesAsyncTest Enter");

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex < AT_SERDESTESTASYNC_PARA_MIN_NUM || g_atParaIndex > AT_SERDESTESTASYNC_PARA_MAX_NUM) {
        AT_PR_LOGE("At_SetSerdesAsyncTest:Parameter Error");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[AT_SERDESTESTASYNC_PARA_MODE].paraValue == 0) {
        /* �������� */
        if (g_atParaList[AT_SERDESTESTASYNC_PARA_CMDTYPE].paraValue == 0) {
            /* �����ϱ��������� */
            if (g_atParaList[AT_SERDESTESTASYNC_PARA_RPTFLG].paraLen == 0) {
                return AT_CME_INCORRECT_PARAMETERS;
            } else {
                atMtInfoCtx->serdesTestAsyncInfo.rptFlg = g_atParaList[AT_SERDESTESTASYNC_PARA_RPTFLG].paraValue;
                atMtInfoCtx->serdesTestAsyncInfo.result = (VOS_INT16)AT_SERDES_TEST_RST_NOT_START;
            }
        } else {
            /* ��������������� */
            if (atMtInfoCtx->serdesTestAsyncInfo.result == (VOS_INT16)AT_SERDES_TEST_RST_RUNNING) {
                AT_PR_LOGE("At_SetSerdesAsyncTest:The Running");
                return AT_ERROR;
            }
            atMtInfoCtx->serdesTestAsyncInfo.result  = (VOS_INT16)AT_SERDES_TEST_RST_NOT_START;
            atMtInfoCtx->serdesTestAsyncInfo.cmdType =
                (VOS_UINT8)(g_atParaList[AT_SERDESTESTASYNC_PARA_CMDTYPE].paraValue);
        }

        result = At_SndDspIdleSerdesTestAsyncReq();
        if (result == AT_SUCCESS) {
            atMtInfoCtx->serdesTestAsyncInfo.result = (VOS_INT16)AT_SERDES_TEST_RST_RUNNING;
            return AT_OK;
        } else {
            return AT_ERROR;
        }
    } else {
        /* ��ѯ���� */
        if (atMtInfoCtx->serdesTestAsyncInfo.cmdType ==
            (VOS_INT8)g_atParaList[AT_SERDESTESTASYNC_PARA_CMDTYPE].paraValue) {
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                atMtInfoCtx->serdesTestAsyncInfo.result);
        } else {
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                AT_SERDES_TEST_RST_NOT_MATCH);
        }
    }
    return AT_OK;
}


VOS_UINT32 AT_QryRficDieIDExPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32  rst;

    atMtInfoCtx = AT_GetMtInfoCtx();

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* ֪ͨCCPU_PID_PAM_MFG��ѯRFIC IDE ID */
    rst = At_SndUeCbtRfIcIdExQryReq();
    if (rst != AT_SUCCESS) {
        AT_WARN_LOG("AT_QryRficDieIDExPara: Snd fail.");
        return AT_ERROR;
    }

    atMtInfoCtx->atInfo.indexNum          = indexNum;
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RFIC_DIE_ID_EX_QRY;
    return AT_WAIT_ASYNC_RETURN;
}

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)

VOS_UINT32 AT_ProcTSelRfWifiPara(VOS_VOID)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();
    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_SUPPORT) {
        /* WIFIδEnableֱ�ӷ���ʧ�� */
        if ((VOS_UINT32)AT_GetWifiStatus() == VOS_FALSE) {
            return AT_WIFI_NOT_ENABLE;
        }

        atMtInfoCtx->atInfo.ratMode = AT_RAT_MODE_WIFI;

        return AT_OK;
    } else {
        return AT_NOT_SUPPORT_WIFI;
    }
}


VOS_UINT32 At_SetFChanWifiProc(VOS_UINT32 band)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    /* WIFI�ĵ�һ����������Ϊ8���ڶ�����������Ϊ15 */
    if (band != AT_BAND_WIFI) {
        return AT_FCHAN_WIFI_BAND_ERR;
    }

    /* WIFI ��֧ */
    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_SUPPORT) {
        /* WIFIδEnableֱ�ӷ���ʧ�� */
        if ((VOS_UINT32)AT_GetWifiStatus() == VOS_FALSE) {
            return AT_WIFI_NOT_ENABLE;
        }

        atMtInfoCtx->atInfo.ratMode = AT_RAT_MODE_WIFI;

        return AT_OK;
    }

    return AT_NOT_SUPPORT_WIFI;
}
#endif
#endif


VOS_UINT32 At_SetTfDpdtPara(VOS_UINT8 indexNum)
{
    AT_MTA_SetDpdtValueReq atCmd;
    VOS_UINT32             rst;

    /* ������� */
    if (g_atParaIndex != AT_TFDPDT_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* AT������MTA����Ϣ�ṹ��ֵ */
    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(AT_MTA_SetDpdtValueReq));
    atCmd.ratMode   = (AT_MTA_CmdRatmodeUint8)g_atParaList[AT_TFDPDT_RAT_MODE].paraValue;
    atCmd.dpdtValue = g_atParaList[AT_TFDPDT_DPDT_VALUE].paraValue;

    /* ������Ϣ��C�˴��� */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_SET_DPDT_VALUE_REQ, (VOS_UINT8 *)&atCmd,
                                 sizeof(AT_MTA_SetDpdtValueReq), I0_UEPS_PID_MTA);

    if (rst == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_TFDPDT_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_SetQryTfDpdtPara(VOS_UINT8 indexNum)
{
    AT_MTA_QryDpdtValueReq atCmd;
    VOS_UINT32             rst;

    /* ������� */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* AT������MTA����Ϣ�ṹ��ֵ */
    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(AT_MTA_QryDpdtValueReq));
    atCmd.ratMode = (AT_MTA_CmdRatmodeUint8)g_atParaList[AT_TFDPDT_QRY_RAT_MODE].paraValue;

    /* ������Ϣ��C�˴��� */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_QRY_DPDT_VALUE_REQ, (VOS_UINT8 *)&atCmd,
                                 sizeof(AT_MTA_QryDpdtValueReq), I0_UEPS_PID_MTA);

    if (rst == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_TFDPDTQRY_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_TestFPllStatusPara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (0,1),(0,1)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}

