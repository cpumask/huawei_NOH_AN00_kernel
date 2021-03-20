/*
 * npu_id_allocator.c
 *
 * about npu id allocator
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
 *
 */
#include <linux/vmalloc.h>
#include <securec.h>
#include <linux/fs.h>
#include "npu_id_allocator.h"
#include "npu_log.h"
#include "npu_common.h"

#define data_ceil(data, size)  (((((data) - 1) / (size)) + 1) * (size))
#define DATA_ALIGN        8

int npu_id_allocator_create(struct npu_id_allocator *id_allocator, u32 start_id, u32 id_num, u32 data_size)
{
	char *tmp = NULL;
	u32 size;
	u32 i;

	COND_RETURN_ERROR(id_num == 0, -EINVAL, "invalid id_num\n");
	COND_RETURN_ERROR(id_allocator == NULL, -EINVAL, "id allocator handel is null\n");

	/* 1. alloc id entity table */
	id_allocator->start_id = start_id;
	id_allocator->entity_num = id_num;
	id_allocator->entity_size = data_ceil(sizeof(struct npu_id_entity) + data_size, DATA_ALIGN);

	size = (long)(unsigned)id_allocator->entity_size * id_num;
	id_allocator->id_entity_base_addr = vmalloc(size);
	COND_RETURN_ERROR(id_allocator->id_entity_base_addr == NULL, -ENOMEM, "no mem to alloc entity info table\n");
	memset_s(id_allocator->id_entity_base_addr, size, 0x0, size);

	/* 2. init lock, available list */
	INIT_LIST_HEAD(&id_allocator->available_list);
	COND_RETURN_ERROR(!list_empty_careful(&id_allocator->available_list), -EEXIST, "available list not empty\n");

	tmp = (char *)id_allocator->id_entity_base_addr;
	for (i = 0; i < id_num; i++) {
		struct npu_id_entity *id_entity = (struct npu_id_entity *)tmp;
		id_entity->id = i + start_id;
		list_add_tail(&id_entity->list, &id_allocator->available_list);
		id_entity->is_allocated = 0;
		id_allocator->available_id_num++;
		tmp += id_allocator->entity_size;
	}
	mutex_init(&id_allocator->lock);

	return 0;
}
int npu_id_allocator_destroy(struct npu_id_allocator *id_allocator)
{
	COND_RETURN_ERROR(id_allocator == NULL, -EINVAL, "id allocator handel is null\n");
	COND_RETURN_ERROR(id_allocator->id_entity_base_addr == NULL, 0, "the id allocator has not created\n");

	/* 1. free id entity table */
	mutex_lock(&id_allocator->lock);
	vfree(id_allocator->id_entity_base_addr);
	id_allocator->id_entity_base_addr = NULL;
	id_allocator->available_id_num = 0;
	id_allocator->entity_size = 0;
	id_allocator->entity_num = 0;
	INIT_LIST_HEAD(&id_allocator->available_list);
	mutex_unlock(&id_allocator->lock);

	/* 2. destroy lock */
	mutex_destroy(&id_allocator->lock);
	return 0;
}
struct npu_id_entity *npu_id_allocator_alloc(struct npu_id_allocator *id_allocator)
{
	struct npu_id_entity *id_entity = NULL;

	COND_RETURN_ERROR(id_allocator == NULL, NULL, "id allocator handel is null\n");
	COND_RETURN_ERROR(id_allocator->id_entity_base_addr == NULL, NULL, "the id allocator %pK has not created\n", id_allocator);
	COND_RETURN_ERROR(id_allocator->available_id_num == 0, NULL, "no id available, id allocator %pK\n", id_allocator);

	mutex_lock(&id_allocator->lock);
	if (!list_empty_careful(&id_allocator->available_list)) {
		id_entity = list_first_entry(&id_allocator->available_list, struct npu_id_entity, list);
		list_del_init(&id_entity->list);
		id_allocator->available_id_num--;
		id_entity->is_allocated = 1;
	}
	mutex_unlock(&id_allocator->lock);

	COND_RETURN_ERROR(id_entity == NULL, NULL, "get one id failed, id allocator %pK\n", id_allocator);
	NPU_DRV_INFO("id allocator %pK has %u left", id_allocator, id_allocator->available_id_num);
	return id_entity;
}
int npu_id_allocator_free(struct npu_id_allocator *id_allocator, u32 index)
{
	struct npu_id_entity *id_entity = NULL;
	int ret = 0;

	COND_RETURN_ERROR(id_allocator == NULL, -EINVAL, "id allocator handel is null\n");
	COND_RETURN_ERROR(id_allocator->id_entity_base_addr == NULL, -ENODATA, "the id allocator has not registed\n");

	mutex_lock(&id_allocator->lock);
	if ((index < id_allocator->start_id) ||
		(index >= (id_allocator->entity_num + id_allocator->start_id)) ||
		(id_allocator->available_id_num == id_allocator->entity_num)) {
		NPU_DRV_ERR("fatal error, invalid id = %u \n", index);
		ret = -ENODATA;
		goto out;
	}

	id_entity = (struct npu_id_entity *)((uintptr_t)id_allocator->id_entity_base_addr + id_allocator->entity_size * (index - id_allocator->start_id));
	if (id_entity == NULL || id_entity->id != index) {
		NPU_DRV_ERR("fatal error, can not find id entity id = %u\n", index);
		ret = -ENODATA;
		goto out;
	}

	if (id_entity->is_allocated != 1) {
		NPU_DRV_ERR("fatal error, id not allocated id = %u\n", index);
		ret = -ENODATA;
		goto out;
	}

	list_del_init(&id_entity->list);
	list_add(&id_entity->list, &id_allocator->available_list);
	id_entity->is_allocated = 0;
	id_allocator->available_id_num++;

	NPU_DRV_INFO("id allocator %pK has %u available", id_allocator, id_allocator->available_id_num);
out:
	mutex_unlock(&id_allocator->lock);
	return ret;
}
