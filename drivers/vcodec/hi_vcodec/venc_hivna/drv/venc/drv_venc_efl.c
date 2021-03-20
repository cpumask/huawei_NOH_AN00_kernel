/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: venc drv
 * Author: liushaoping liushaoping@huawei.com
 * Create: 2009/12/17
 */
#include "drv_venc_efl.h"
#include "drv_mem.h"
#include "venc_regulator.h"

#define D_VENC_CHECK_CFG_REG_ADDR_RET(map_addr, cfg_reg_addr, map_buffer_size) \
	do { \
		if (((cfg_reg_addr) < (map_addr)) || ((cfg_reg_addr) > ((map_addr) + (map_buffer_size)))) \
			return HI_FAILURE; \
	} while (0)

#define D_VENC_CHECK_CFG_REG_ENDADDR_RET(map_addr, cfg_reg_addr, map_buffer_size) \
	do { \
		if (((cfg_reg_addr) < ((map_addr) -1)) || ((cfg_reg_addr) > ((map_addr) + (map_buffer_size)))) \
			return HI_FAILURE; \
	} while (0)

#define D_VENC_CHECK_CFG_REG_STREAMADDR_RET(map_addr, cfg_reg_addr, map_buffer_size, shared_fd) \
	do { \
		if ((((cfg_reg_addr) < (map_addr))  || ((cfg_reg_addr) > ((map_addr) + (map_buffer_size)))) && \
			(((shared_fd) != 0) && (((cfg_reg_addr) != 0) && ((cfg_reg_addr) != 0xffffffff)))) \
			return HI_FAILURE; \
	} while (0)

#define D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(map_addr, cfg_reg_addr, map_buffer_size, shared_fd) \
	do { \
		if ((((cfg_reg_addr) < ((map_addr) - 1))  || ((cfg_reg_addr) > ((map_addr) + (map_buffer_size)))) && \
			(((shared_fd) != 0) && (((cfg_reg_addr) != 0) && ((cfg_reg_addr) != 0xffffffff)))) \
			return HI_FAILURE; \
	} while (0)

#define D_VENC_CHECK_SMRX_REG_ADDR_RET(map_addr, cfg_reg_addr, map_buffer_size) \
	do { \
	if (((cfg_reg_addr) < ((map_addr) >> 15)) || ((cfg_reg_addr) > (((map_addr) + (map_buffer_size)) >> 15))) \
		return HI_FAILURE; \
	} while (0)

#define D_VENC_CHECK_SMRX_REG_ENDADDR_RET(map_addr, cfg_reg_addr, map_buffer_size)\
	do { \
		if (((cfg_reg_addr) < (((map_addr) - 1) >> 15)) || ((cfg_reg_addr) > (((map_addr) + (map_buffer_size)) >> 15))) \
			return HI_FAILURE; \
	} while (0)

static struct venc_fifo_buffer *g_encode_done_info[MAX_OPEN_COUNT];

static HI_S32 wait_buffer_filled(struct venc_fifo_buffer *buffer)
{
	HI_S32 ret;
	HI_U32 timeout = venc_regulator_is_fpga() ? FPGA_WAIT_EVENT_TIME_MS : ENCODE_DONE_TIMEOUT_MS;

	ret = OSAL_WAIT_EVENT_TIMEOUT(&buffer->event, !queue_is_empty(buffer), timeout); /*lint !e578 !e666*/

	if (ret != HI_SUCCESS) {
		HI_ERR_VENC("wait buffer filled timeout");
	}

	return ret;
}

static HI_S32 get_mem_info_by_fd(HI_S32 fd, mem_buffer_t *buffer)
{
	HI_S32 ret;

	if (fd < 0) {
		HI_FATAL_VENC("invalid param, share fd is NULL");
		return HI_FAILURE;
	}

	ret = drv_mem_get_map_info(fd, buffer);
	if (ret) {
		HI_FATAL_VENC("share fd map failed");
		return HI_FAILURE;
	}

	drv_mem_put_map_info(buffer);

	return HI_SUCCESS;
}

static HI_S32 get_mem_info(venc_fd_info_t *fd_info, venc_buffer_info_t *mem_info)
{
	HI_S32 ret;
	HI_S32 i;

	if (!mem_info) {
		HI_FATAL_VENC("pMemMap parameter error");
		return HI_FAILURE;
	}

	ret = get_mem_info_by_fd(fd_info->interal_shared_fd, &mem_info->internal_buffer);

	if (ret) {
		HI_FATAL_VENC("Interal buffer map failed");
		return HI_FAILURE;
	}

	ret = get_mem_info_by_fd(fd_info->image_shared_fd, &mem_info->image_buffer);

	if (ret) {
		HI_FATAL_VENC("Image buffer map failed");
		return HI_FAILURE;
	}

	ret = get_mem_info_by_fd(fd_info->stream_head_shared_fd, &mem_info->stream_head_buffer);

	if (ret) {
		HI_FATAL_VENC("streamhead buffer map failed");
		return HI_FAILURE;
	}

	for (i = 0; i < MAX_SLICE_NUM; i++) {
		if (fd_info->stream_shared_fd[i] == 0) {
			continue;
		}

		ret = get_mem_info_by_fd(fd_info->stream_shared_fd[i], &mem_info->stream_buffer[i]);

		if (ret) {
			HI_FATAL_VENC("Stream buffer map failed");
			return HI_FAILURE;
		}
	}

	return HI_SUCCESS;
}

static HI_S32 check_internal_buffer_addr(const S_HEVC_AVC_REGS_TYPE_CFG *reg, const mem_buffer_t *buffer)
{
#ifndef HIVCODECV500
	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REC_YADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REC_CADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REC_YH_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REC_CH_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFY_L0_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFC_L0_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFYH_L0_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFCH_L0_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_PMELD_L0_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFY_L1_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFC_L1_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFYH_L1_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFCH_L1_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_PMELD_L1_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_PMEST_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_NBI_MVST_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_NBI_MVLD_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_PMEINFO_ST_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_PMEINFO_LD0_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_PMEINFO_LD1_ADDR, buffer->size);

#ifndef HIVCODEC_PLATFORM_ECONOMIC
	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_QPGLD_INF_ADDR, buffer->size);
#endif

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_NBI_MVST_ADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_NBI_MVST_ADDR_END, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_NBI_MVLD_ADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_NBI_MVLD_ADDR_END, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_PMEST_ADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_PMEST_ADDR_END, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_PMELD_ADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_PMELD_ADDR_END, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_PMEINFOST_ADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_PMEINFOST_ADDR_END, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_PMEINFOLD0_ADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_PMEINFOLD0_ADDR_END, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_PMEINFOLD1_ADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_PMEINFOLD1_ADDR_END, buffer->size);
#ifndef HIVCODEC_PLATFORM_ECONOMIC
	/* QPMAP register, not used in v210 v310 */
	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_QPGLD_ADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_QPGLD_ADDR_END, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REC_YH_ADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REC_YH_ADDR_END, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REC_CH_ADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REC_CH_ADDR_END, buffer->size);
#endif
	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REC_YADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REC_YADDR_END, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REC_CADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REC_CADDR_END, buffer->size);
#ifndef HIVCODEC_PLATFORM_ECONOMIC
	/* QPMAP register, not used in v210 v310 */
	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REF_YH_ADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REF_YH_ADDR_END, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REF_CH_ADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REF_CH_ADDR_END, buffer->size);
#endif
	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REF_YADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REF_YADDR_END, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REF_CADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_REF_CADDR_END, buffer->size);
#ifdef HIVCODEC_PLATFORM_ECONOMIC
	/* secure in v200, not check */
	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[12].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[12].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[13].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[13].bits.va_end, buffer->size);
	/* image addr when not in v210 */
	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[15].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[15].bits.va_end, buffer->size);
#endif
	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[16].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[16].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[17].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[17].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[18].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[18].bits.va_end, buffer->size);
#ifndef HIVCODEC_PLATFORM_ECONOMIC
	/* image addr in v200 */
	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[19].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[19].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[20].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[20].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[21].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[21].bits.va_end, buffer->size);
	/* secure in v210, not check */
	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[22].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[22].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[23].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[23].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[24].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[24].bits.va_end, buffer->size);
#endif
#ifdef HIVCODEC_PLATFORM_ECONOMIC
	/* internal addr in v210 v310 */
	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[29].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[29].bits.va_end, buffer->size);
	/* secure when not in v210 */
	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[30].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[30].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[31].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[31].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[32].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[32].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[33].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[33].bits.va_end, buffer->size);
#endif
#ifndef HIVCODEC_PLATFORM_ECONOMIC
	/* value not setted in v210 v310 */
	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[39].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[39].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[40].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[40].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[41].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[41].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[42].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[42].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[43].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[43].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[44].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[44].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[45].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[45].bits.va_end, buffer->size);
#endif
#else
	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REC_YADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REC_CADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REC_YH_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REC_CH_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFY_L0_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFC_L0_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFYH_L0_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFCH_L0_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_PMELD_L0_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFY_L1_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFC_L1_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFYH_L1_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_REFCH_L1_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_PMELD_L1_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_PMEST_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_NBI_MVST_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_NBI_MVLD_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_PMEINFO_ST_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_PMEINFO_LD0_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_PMEINFO_LD1_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_QPGLD_INF_ADDR_L, buffer->size);

#endif

	return HI_SUCCESS;
}

static HI_S32 check_image_buffer_addr(const S_HEVC_AVC_REGS_TYPE_CFG *reg, const mem_buffer_t *buffer)
{
#ifndef HIVCODECV500
	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_TUNLCELL_ADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_SRC_YADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_SRC_CADDR, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_SRC_YADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_SRC_YADDR_END, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_SRC_CADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_SRC_CADDR_END, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_LOWDLY_ADDR_STR, buffer->size);

	D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_LOWDLY_ADDR_END, buffer->size);
#ifdef HIVCODEC_PLATFORM_ECONOMIC
	/* secure in v200 v310 */
	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[11].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[11].bits.va_end, buffer->size);
#endif
#ifndef HIVCODEC_PLATFORM_ECONOMIC
	/* internal addr in v210 v310 */
	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[15].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[15].bits.va_end, buffer->size);
	/* secure in v210 */
	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[27].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[27].bits.va_end, buffer->size);
	/* value not setted in v210 */
	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[28].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[28].bits.va_end, buffer->size);
#endif
#ifdef HIVCODEC_PLATFORM_ECONOMIC
	/* internal addr in v200, image addr in v210 v310 */
	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[19].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[19].bits.va_end, buffer->size);

	D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[20].bits.va_str, buffer->size);

	D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[20].bits.va_end, buffer->size);
#endif
	if (reg->VEDU_VCPI_STRFMT.bits.vcpi_str_fmt == YUV420_PLANAR ||
		reg->VEDU_VCPI_STRFMT.bits.vcpi_str_fmt == YUV422_PLANAR) {
		D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_SRC_VADDR, buffer->size);

		D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_SRC_VADDR_STR, buffer->size);

		D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_SRC_VADDR_END, buffer->size);
#ifndef HIVCODEC_PLATFORM_ECONOMIC
		/* internal addr in v210 v310 */
		D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[29].bits.va_str, buffer->size);

		D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[29].bits.va_end, buffer->size);
#endif
#ifdef HIVCODEC_PLATFORM_ECONOMIC
		/* VEDU_SRC_VADDR, need in this condition */
		D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[21].bits.va_str, buffer->size);

		D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[21].bits.va_end, buffer->size);
#endif
	}

	if (reg->VEDU_VCPI_STRFMT.bits.vcpi_str_fmt == YUV420_SEMIPLANAR_CMP) {
		D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_YH_ADDR, buffer->size);

		D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_CH_ADDR, buffer->size);

		D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_SRC_YHADDR_STR, buffer->size);

		D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_SRC_YHADDR_END, buffer->size);

		D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_SRC_CHADDR_STR, buffer->size);

		D_VENC_CHECK_CFG_REG_ENDADDR_RET(buffer->start_phys_addr, reg->MMU_PRE_SRC_CHADDR_END, buffer->size);

		D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[25].bits.va_str, buffer->size);

		D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[25].bits.va_end, buffer->size);

		D_VENC_CHECK_SMRX_REG_ADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_1[26].bits.va_str, buffer->size);

		D_VENC_CHECK_SMRX_REG_ENDADDR_RET(buffer->start_phys_addr, reg->SMMU_MSTR_SMRX_2[26].bits.va_end, buffer->size);
	}
#else
	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_TUNLCELL_ADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_SRC_YADDR_L, buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_SRC_CADDR_L, buffer->size);

	if (reg->VEDU_VCPI_STRFMT.bits.vcpi_str_fmt == YUV420_PLANAR ||
		reg->VEDU_VCPI_STRFMT.bits.vcpi_str_fmt == YUV422_PLANAR) {
		D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_SRC_VADDR_L, buffer->size);
	}

	if (reg->VEDU_VCPI_STRFMT.bits.vcpi_str_fmt == YUV420_SEMIPLANAR_CMP) {
		D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_YH_ADDR_L, buffer->size);

		D_VENC_CHECK_CFG_REG_ADDR_RET(buffer->start_phys_addr, reg->VEDU_VCPI_CH_ADDR_L, buffer->size);
	}
#endif
	return HI_SUCCESS;
}

static HI_S32 check_image_stream_addr(const S_HEVC_AVC_REGS_TYPE_CFG *reg,
		const mem_buffer_t *stream_buffer, const mem_buffer_t *head_buffer)
{
#ifndef HIVCODECV500
	if (reg->VEDU_VCPI_STRFMT.bits.vcpi_str_fmt == YUV420_SEMIPLANAR_CMP) {
		D_VENC_CHECK_CFG_REG_STREAMADDR_RET(head_buffer->start_phys_addr,
				reg->MMU_PRE_PPS_ADDR_STR, head_buffer->size, head_buffer->share_fd);
		D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(head_buffer->start_phys_addr,
				reg->MMU_PRE_PPS_ADDR_END, head_buffer->size, head_buffer->share_fd);
	}

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[0].start_phys_addr,
			reg->MMU_PRE_STRMADDR0_STR, stream_buffer[0].size, stream_buffer[0].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[0].start_phys_addr,
			reg->MMU_PRE_STRMADDR0_END, stream_buffer[0].size, stream_buffer[0].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[1].start_phys_addr,
			reg->MMU_PRE_STRMADDR1_STR, stream_buffer[1].size, stream_buffer[1].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[1].start_phys_addr,
			reg->MMU_PRE_STRMADDR1_END, stream_buffer[1].size, stream_buffer[1].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[2].start_phys_addr,
			reg->MMU_PRE_STRMADDR2_STR, stream_buffer[2].size, stream_buffer[2].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[2].start_phys_addr,
			reg->MMU_PRE_STRMADDR2_END, stream_buffer[2].size, stream_buffer[2].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[3].start_phys_addr,
			reg->MMU_PRE_STRMADDR3_STR, stream_buffer[3].size, stream_buffer[3].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[3].start_phys_addr,
			reg->MMU_PRE_STRMADDR3_END, stream_buffer[3].size, stream_buffer[3].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[4].start_phys_addr,
			reg->MMU_PRE_STRMADDR4_STR, stream_buffer[4].size, stream_buffer[4].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[4].start_phys_addr,
			reg->MMU_PRE_STRMADDR4_END, stream_buffer[4].size, stream_buffer[4].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[5].start_phys_addr,
			reg->MMU_PRE_STRMADDR5_STR, stream_buffer[5].size, stream_buffer[5].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[5].start_phys_addr,
			reg->MMU_PRE_STRMADDR5_END, stream_buffer[5].size, stream_buffer[5].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[6].start_phys_addr,
			reg->MMU_PRE_STRMADDR6_STR, stream_buffer[6].size, stream_buffer[6].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[6].start_phys_addr,
			reg->MMU_PRE_STRMADDR6_END, stream_buffer[6].size, stream_buffer[6].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[7].start_phys_addr,
			reg->MMU_PRE_STRMADDR7_STR, stream_buffer[7].size, stream_buffer[7].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[7].start_phys_addr,
			reg->MMU_PRE_STRMADDR7_END, stream_buffer[7].size, stream_buffer[7].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[8].start_phys_addr,
			reg->MMU_PRE_STRMADDR8_STR, stream_buffer[8].size, stream_buffer[8].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[8].start_phys_addr,
			reg->MMU_PRE_STRMADDR8_END, stream_buffer[8].size, stream_buffer[8].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[9].start_phys_addr,
			reg->MMU_PRE_STRMADDR9_STR, stream_buffer[9].size, stream_buffer[9].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[9].start_phys_addr,
			reg->MMU_PRE_STRMADDR9_END, stream_buffer[9].size, stream_buffer[9].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[10].start_phys_addr,
			reg->MMU_PRE_STRMADDR10_STR, stream_buffer[10].size, stream_buffer[10].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[10].start_phys_addr,
			reg->MMU_PRE_STRMADDR10_END, stream_buffer[10].size, stream_buffer[10].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[11].start_phys_addr,
			reg->MMU_PRE_STRMADDR11_STR, stream_buffer[11].size, stream_buffer[11].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[11].start_phys_addr,
			reg->MMU_PRE_STRMADDR11_END, stream_buffer[11].size, stream_buffer[11].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[12].start_phys_addr,
			reg->MMU_PRE_STRMADDR12_STR, stream_buffer[12].size, stream_buffer[12].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[12].start_phys_addr,
			reg->MMU_PRE_STRMADDR12_END, stream_buffer[12].size, stream_buffer[12].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[13].start_phys_addr,
			reg->MMU_PRE_STRMADDR13_STR, stream_buffer[13].size, stream_buffer[13].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[13].start_phys_addr,
			reg->MMU_PRE_STRMADDR13_END, stream_buffer[13].size, stream_buffer[13].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[14].start_phys_addr,
			reg->MMU_PRE_STRMADDR14_STR, stream_buffer[14].size, stream_buffer[14].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[14].start_phys_addr,
			reg->MMU_PRE_STRMADDR14_END, stream_buffer[14].size, stream_buffer[14].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[15].start_phys_addr,
			reg->MMU_PRE_STRMADDR15_STR, stream_buffer[15].size, stream_buffer[15].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMENDADDR_RET(stream_buffer[15].start_phys_addr,
			reg->MMU_PRE_STRMADDR15_END, stream_buffer[15].size, stream_buffer[15].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[0].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR0, stream_buffer[0].size, stream_buffer[0].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[1].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR1, stream_buffer[1].size, stream_buffer[1].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[2].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR2, stream_buffer[2].size, stream_buffer[2].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[3].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR3, stream_buffer[3].size, stream_buffer[3].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[4].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR4, stream_buffer[4].size, stream_buffer[4].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[5].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR5, stream_buffer[5].size, stream_buffer[5].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[6].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR6, stream_buffer[6].size, stream_buffer[6].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[7].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR7, stream_buffer[7].size, stream_buffer[7].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[8].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR8, stream_buffer[8].size, stream_buffer[8].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[9].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR9, stream_buffer[9].size, stream_buffer[9].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[10].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR10, stream_buffer[10].size, stream_buffer[10].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[11].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR11, stream_buffer[11].size, stream_buffer[11].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[12].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR12, stream_buffer[12].size, stream_buffer[12].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[13].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR13, stream_buffer[13].size, stream_buffer[13].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[14].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR14, stream_buffer[14].size, stream_buffer[14].share_fd);

	D_VENC_CHECK_CFG_REG_STREAMADDR_RET(stream_buffer[15].start_phys_addr,
			reg->VEDU_VLCST_STRMADDR15, stream_buffer[15].size, stream_buffer[15].share_fd);
#else

	D_VENC_CHECK_CFG_REG_ADDR_RET(stream_buffer->start_phys_addr, reg->VEDU_VCPI_SWPTRADDR_L, stream_buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(stream_buffer->start_phys_addr, reg->VEDU_VCPI_SRPTRADDR_L, stream_buffer->size);

	D_VENC_CHECK_CFG_REG_ADDR_RET(stream_buffer->start_phys_addr, reg->VEDU_VCPI_STRMADDR_L, stream_buffer->size);

#endif
	return HI_SUCCESS;
}

static HI_S32 check_buffer_addr(const S_HEVC_AVC_REGS_TYPE_CFG *reg, venc_fd_info_t *fd_info)
{
	HI_S32 ret;
	venc_buffer_info_t mem_info;

	(void)memset_s((HI_VOID *)&mem_info, sizeof(mem_info), 0, sizeof(mem_info));

	ret = get_mem_info(fd_info, &mem_info);
	if (ret) {
		HI_FATAL_VENC("mem shared error");
		return HI_FAILURE;
	}

	ret = check_internal_buffer_addr(reg, &mem_info.internal_buffer);
	if (ret) {
		HI_FATAL_VENC("check internal buffer addr failed");
		return HI_FAILURE;
	}

	ret = check_image_buffer_addr(reg, &mem_info.image_buffer);
	if (ret) {
		HI_FATAL_VENC("check image buffer addr failed");
		return HI_FAILURE;
	}

	ret = check_image_stream_addr(reg, &mem_info.stream_buffer[0], &mem_info.stream_head_buffer);
	if (ret) {
		HI_FATAL_VENC("check stream buffer addr failed");
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

HI_S32 venc_drv_create_queue(HI_VOID)
{
	HI_S32 i;
	HI_S32 j;

	for (i = 0; i < MAX_OPEN_COUNT; i++) {
		g_encode_done_info[i] = create_queue(struct venc_fifo_buffer);
		if (g_encode_done_info[i] == NULL)
			break;
	}

	if (i == MAX_OPEN_COUNT)
		return HI_SUCCESS;

	for (j = 0; j < i; j++) {
		destory_queue(g_encode_done_info[j]);
		g_encode_done_info[j] = NULL;
	}

	HI_FATAL_VENC("create queue failed");
	return HI_FAILURE;
}

HI_VOID venc_drv_destroy_queue(HI_VOID)
{
	HI_S32 i;

	for (i = 0; i < MAX_OPEN_COUNT; i++) {
		if (g_encode_done_info[i] != NULL) {
			destory_queue(g_encode_done_info[i]);
			g_encode_done_info[i] = NULL;
		}
	}
}


HI_S32 venc_drv_alloc_encode_done_info_buffer(struct file *file)
{
	HI_S32 i;
	HI_S32 ret;

	if (file == NULL) {
		HI_ERR_VENC("input file is NULL");
		return HI_FAILURE;
	}

	for (i = 0; i < MAX_OPEN_COUNT; i++) {
		if (queue_is_null(g_encode_done_info[i]))
			break;
	}

	if (i == MAX_OPEN_COUNT) {
		HI_FATAL_VENC("failed to find idle kfifo");
		return HI_FAILURE;
	}

	ret = alloc_queue(g_encode_done_info[i], MAX_RING_BUFFER_SIZE);
	if (ret != HI_SUCCESS) {
		HI_FATAL_VENC("failed to allocate kfifo");
		return HI_FAILURE;
	}

	file->private_data = g_encode_done_info[i];

	return HI_SUCCESS;
}

HI_S32 venc_drv_free_encode_done_info_buffer(const struct file *file)
{
	HI_S32 i;

	if (file == NULL) {
		HI_ERR_VENC("input file is NULL");
		return HI_FAILURE;
	}

	for (i = 0; i < MAX_OPEN_COUNT; i++) {
		if (file->private_data == g_encode_done_info[i]) {
			free_queue(g_encode_done_info[i]);
			return HI_SUCCESS;
		}
	}

	HI_ERR_VENC("not find buffer info, free buffer fail");

	return HI_FAILURE;
}

HI_S32 venc_drv_get_encode_done_info(struct venc_fifo_buffer *buffer, struct encode_done_info *encode_done_info)
{
	HI_S32 ret;

	if (buffer == NULL || encode_done_info == NULL) {
		HI_ERR_VENC("input is invalid");
		return HI_FAILURE;
	}

	ret = wait_buffer_filled(buffer);
	if (ret != HI_SUCCESS) {
		HI_ERR_VENC("wait buffer filled failed");
		return HI_FAILURE;
	}

	ret = pop(buffer, encode_done_info);
	if (ret != HI_SUCCESS) {
		HI_ERR_VENC("read encode done info failed");
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

HI_S32 venc_drv_encode(struct encode_info *encode_info, struct venc_fifo_buffer *buffer)
{
	HI_S32 ret;
	venc_entry_t *venc = platform_get_drvdata(venc_get_device());

	if (encode_info == NULL || buffer == NULL) {
		HI_ERR_VENC("input is invalid");
		return HI_FAILURE;
	}

	ret = check_buffer_addr(&encode_info->all_reg, &encode_info->mem_info);
	if (ret) {
		HI_FATAL_VENC("check buffer addr error");
		return HI_FAILURE;
	}

	return venc->ops.encode(encode_info, buffer);
}

irqreturn_t venc_drv_encode_done(HI_S32 irq, HI_VOID *dev_id)
{
	HI_S32 i;
	venc_entry_t *venc = platform_get_drvdata(venc_get_device());

	HI_DBG_VENC("enter %s ()", __func__);

	for (i = 0; i < MAX_SUPPORT_CORE_NUM; i++) {
		if (venc->ctx[i].irq_num_normal == irq)
			break;
	}

	if (i == MAX_SUPPORT_CORE_NUM) {
		HI_FATAL_VENC("isr not register");
		return IRQ_HANDLED;
	}

	HI_DBG_VENC("current cord_id is %d", i);

	if (venc->ctx[i].status != VENC_BUSY)
		HI_FATAL_VENC("cord %d: isr invalid core status, status is %d",
				i, venc->ctx[i].status);

	venc->ops.encode_done(&venc->ctx[i]);

	HI_DBG_VENC("out %s ()", __func__);
	return IRQ_HANDLED;
}

HI_S32 venc_drv_open_vedu(HI_VOID)
{
	venc_entry_t *venc = platform_get_drvdata(venc_get_device());
	HI_S32 ret;

	HI_DBG_VENC("enter %s()", __func__);

	ret = venc->ops.init();

	HI_DBG_VENC("exit %s()", __func__);
	return ret;
}

HI_S32 venc_drv_close_vedu(HI_VOID)
{
	venc_entry_t *venc = platform_get_drvdata(venc_get_device());

	HI_DBG_VENC("enter %s()", __func__);

	venc_regulator_wait_hardware_idle(&venc->event);
	venc->ops.deinit();

	HI_DBG_VENC("exit %s()", __func__);

	return HI_SUCCESS;
}

HI_S32 venc_drv_suspend_vedu(HI_VOID)
{
	venc_entry_t *venc = platform_get_drvdata(venc_get_device());

	HI_INFO_VENC("enter %s()", __func__);

	venc_regulator_wait_hardware_idle(&venc->event);

	HI_INFO_VENC("exit %s()", __func__);

	return HI_SUCCESS;
}

HI_S32 venc_drv_resume_vedu(HI_VOID)
{
	HI_INFO_VENC("enter %s()", __func__);
	HI_INFO_VENC("exit %s()", __func__);
	return HI_SUCCESS;
}

inline HI_S32 venc_check_coreid(HI_S32 core_id)
{
	if (core_id >= MAX_SUPPORT_CORE_NUM || core_id < VENC_CORE_0)
		return HI_FAILURE;

	return HI_SUCCESS;
}

