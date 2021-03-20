/*
 * static_ion_mem.c
 *
 * memory init, register for mailbox pool.
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "static_ion_mem.h"
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/mempool.h>
#include <linux/vmalloc.h>
#include <linux/of_reserved_mem.h>
#include "smc_smp.h"
#include "teek_ns_client.h"
#include "mailbox_mempool.h"
#include "tc_ns_log.h"

enum static_mem_tag {
	MEM_TAG_MIN = 0,
	PP_MEM_TAG = 1,
	PRI_PP_MEM_TAG = 2,
	PT_MEM_TAG = 3,
	MEM_TAG_MAX,
};

static u64 g_ion_mem_addr;
static u64 g_ion_mem_size;

static int supersonic_reserve_tee_mem(struct reserved_mem *rmem)
{
	if (rmem != NULL) {
		g_ion_mem_addr = rmem->base;
		g_ion_mem_size = rmem->size;
	} else {
		tloge("rmem is NULL\n");
	}

	return 0;
}

RESERVEDMEM_OF_DECLARE(supersonic, "hisi-supersonic",
	supersonic_reserve_tee_mem);

static u64 g_secfacedetect_mem_addr;
static u64 g_secfacedetect_mem_size;

static int secfacedetect_reserve_tee_mem(struct reserved_mem *rmem)
{
	if (rmem != NULL) {
		g_secfacedetect_mem_addr = rmem->base;
		g_secfacedetect_mem_size = rmem->size;
	} else {
		tloge("secfacedetect_reserve_tee_mem mem is NULL\n");
	}
	return 0;
}

RESERVEDMEM_OF_DECLARE(secfacedetect, "hisi-secfacedetect",
	secfacedetect_reserve_tee_mem);

static u64 g_hiai_pt_addr = 0;
static u64 g_hiai_pt_size = 0;

static int hiai_reserve_pt_mem(struct reserved_mem *rmem)
{
	if (rmem != NULL) {
		g_hiai_pt_addr = rmem->base;
		g_hiai_pt_size = rmem->size;
		tloge("reserve pp mem is not NULL.\n");
	} else {
		tloge("reserve pt mem is NULL.\n");
	}
	return 0;
}

RESERVEDMEM_OF_DECLARE(hiai_pagetable, "hisi-hiai-pagetable", hiai_reserve_pt_mem);

static u64 g_hiai_pp_addr = 0;
static u64 g_hiai_pp_size = 0;

static int hiai_reserve_pp_mem(struct reserved_mem *rmem)
{
	if (rmem != NULL) {
		g_hiai_pp_addr = rmem->base;
		g_hiai_pp_size = rmem->size;
		tloge("reserve pp mem is not NULL.\n");
	} else {
		tloge("reserve pp mem is NULL.\n");
	}
	return 0;
}

RESERVEDMEM_OF_DECLARE(hiai_running, "hisi-hiai-running", hiai_reserve_pp_mem);

static u64 g_voiceid_addr;
static u64 g_voiceid_size;

static int voiceid_reserve_tee_mem(struct reserved_mem *rmem)
{
	if (rmem != NULL) {
		g_voiceid_addr = rmem->base;
		g_voiceid_size = rmem->size;
	} else {
		tloge("voiceid_reserve_tee_mem  mem is NULL.\n");
	}
	return 0;
}

RESERVEDMEM_OF_DECLARE(voiceid, "hisi-voiceid",
	voiceid_reserve_tee_mem);

static u64 g_secos_ex_addr;
static u64 g_secos_ex_size;

static int secos_reserve_tee_mem(struct reserved_mem *rmem)
{
	if (rmem != NULL) {
		g_secos_ex_addr = rmem->base;
		g_secos_ex_size = rmem->size;
	} else {
		tloge("secos_reserve_tee_mem  mem is NULL.\n");
	}
	return 0;
}

RESERVEDMEM_OF_DECLARE(secos_ex, "hisi-secos-ex",
	secos_reserve_tee_mem);

static u64 g_ion_ex_mem_addr;
static u64 g_ion_ex_mem_size;

static int supersonic_ex_reserve_tee_mem(struct reserved_mem *rmem)
{
	if (rmem != NULL) {
		g_ion_ex_mem_addr = rmem->base;
		g_ion_ex_mem_size = rmem->size;
	} else {
		tloge("rmem is NULL\n");
	}

	return 0;
}

RESERVEDMEM_OF_DECLARE(supersonic_ex, "hisi-supersonic-ex",
	supersonic_ex_reserve_tee_mem);

#define ION_MEM_MAX_SIZE 10
struct register_ion_mem_tag {
	uint32_t size;
	uint64_t memaddr[ION_MEM_MAX_SIZE];
	uint32_t memsize[ION_MEM_MAX_SIZE];
	uint32_t memtag[ION_MEM_MAX_SIZE];
};

static void set_ion_mem_info(struct register_ion_mem_tag *memtag)
{
	uint32_t pos = 0;

	if (g_ion_mem_addr != (u64)0 && g_ion_mem_size  != (u64)0) {
		memtag->memaddr[pos] = g_ion_mem_addr;
		memtag->memsize[pos] = g_ion_mem_size;
		memtag->memtag[pos] = PP_MEM_TAG;
		pos++; // pos is 1
	}
	if (g_secfacedetect_mem_addr != (u64)0 &&
		g_secfacedetect_mem_size != (u64)0) {
		memtag->memaddr[pos] = g_secfacedetect_mem_addr;
		memtag->memsize[pos] = g_secfacedetect_mem_size;
		memtag->memtag[pos] = PP_MEM_TAG;
		pos++; // pos is 2
	}
	if (g_voiceid_addr != (u64)0 && g_voiceid_size != (u64)0) {
		memtag->memaddr[pos] = g_voiceid_addr;
		memtag->memsize[pos] = g_voiceid_size;
		memtag->memtag[pos] = PP_MEM_TAG;
		pos++; // pos is 3
	}
	if (g_secos_ex_addr != (u64)0 && g_secos_ex_size != (u64)0) {
		memtag->memaddr[pos] = g_secos_ex_addr;
		memtag->memsize[pos] = g_secos_ex_size;
		memtag->memtag[pos] = PP_MEM_TAG;
		pos++; // pos is 4
	}
	if (g_hiai_pt_addr != (u64)0 && g_hiai_pt_size != (u64)0) {
		memtag->memaddr[pos] = g_hiai_pt_addr;
		memtag->memsize[pos] = g_hiai_pt_size;
		memtag->memtag[pos] = PT_MEM_TAG;
		pos++; // pos is 5
	}
	if (g_hiai_pp_addr != (u64)0 && g_hiai_pp_size != (u64)0) {
		memtag->memaddr[pos] = g_hiai_pp_addr;
		memtag->memsize[pos] = g_hiai_pp_size;
		memtag->memtag[pos] = PRI_PP_MEM_TAG;
		pos++; // pos is 6
	}
	if (g_ion_ex_mem_addr != (u64)0 && g_ion_ex_mem_size != (u64)0) {
		memtag->memaddr[pos] = g_ion_ex_mem_addr;
		memtag->memsize[pos] = g_ion_ex_mem_size;
		memtag->memtag[pos] = PP_MEM_TAG;
		pos++; // pos is 7
	}
	/* here pos max is 7, memaddr[] has 10 positions, just 3 free */
	memtag->size = pos;
	return;
}

/* send the ion static memory to tee */
int tc_ns_register_ion_mem(void)
{
	struct tc_ns_smc_cmd smc_cmd = { {0}, 0 };
	int ret = 0;
	struct mb_cmd_pack *mb_pack = NULL;
	struct register_ion_mem_tag *memtag = NULL;

	tloge("ion mem static reserved for tee face=%d,finger=%d,voiceid=%d,\
secos=%d,finger-ex=%d,hiai-pt=%d,hiai_pp=%d\n",
		(uint32_t)g_secfacedetect_mem_size, (uint32_t)g_ion_mem_size,
		(uint32_t)g_voiceid_size, (uint32_t)g_secos_ex_size,
		(uint32_t)g_ion_ex_mem_size, (uint32_t)g_hiai_pt_size,
		(uint32_t)g_hiai_pp_size);
	mb_pack = mailbox_alloc_cmd_pack();
	if (mb_pack == NULL) {
		tloge("mailbox alloc failed\n");
		return -ENOMEM;
	}
	memtag = mailbox_alloc(sizeof(*memtag), 0);
	if (memtag == NULL) {
		mailbox_free(mb_pack);
		return -ENOMEM;
	}
	set_ion_mem_info(memtag);
	smc_cmd.global_cmd = true;
	smc_cmd.cmd_id = GLOBAL_CMD_ID_REGISTER_ION_MEM;

	mb_pack->operation.paramtypes = TEE_PARAM_TYPE_MEMREF_INPUT;
	mb_pack->operation.params[0].memref.buffer =
		virt_to_phys((void *)memtag);
	mb_pack->operation.buffer_h_addr[0] =
		virt_to_phys((void *)memtag) >> ADDR_TRANS_NUM;
	mb_pack->operation.params[0].memref.size = sizeof(*memtag);

	smc_cmd.operation_phys = virt_to_phys(&mb_pack->operation);
	smc_cmd.operation_h_phys = virt_to_phys(&mb_pack->operation) >> 32;

	ret = tc_ns_smc(&smc_cmd);
	mailbox_free(mb_pack);
	mailbox_free(memtag);
	if (ret)
		tloge("Send ion mem info failed.\n");

	return ret;
}
