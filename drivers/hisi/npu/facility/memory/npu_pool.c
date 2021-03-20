/*
 * npu_pool.c
 *
 * about npu pool
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
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
#include "npu_pool.h"

#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/vmalloc.h>
#include <linux/bitmap.h>
#include <securec.h>

#include "npu_log.h"

struct devdrv_pool_info {
	u8 is_created;
	vir_addr_t virt_addr;
	unsigned long iova;
	u32 used_num;
	u32 entity_num;
} ;
/*
Pool allocation principle: You can only allocate pools in order from 0 to n, that is, when expanding,
		it will expand backwards, and when it shrinks, it will shrink from the back.
		Even if only the nth pool is used, the front is empty, and the previous pool will not be released first.
Id assignment principle: assign the currently available minimum id each time
*/
struct devdrv_pool_manager {
	struct mutex lock;
	u32 start_id;
	u32 entity_num;
	u32 entity_size;
	u32 pool_num;
	alloc_pool_func alloc_pool;
	free_pool_func free_pool;
	struct devdrv_pool_info *pool;
	u32 pool_created;
	struct devdrv_entity_info *entity;
	unsigned long *entity_status_bitmap; // 1.unused, 0.used
};

#define	DEVDRV_MEM_POOL_USE_THRESHOLD(size) ((size) * 4 / 5)

static struct devdrv_pool_manager g_pool_manager[NPU_DEV_NUM][DEVDRV_MEM_POOL_TYPE_MAX];

static int devdrv_pool_manager_init_entity_info(struct devdrv_pool_manager *pool_manager)
{
	u32 entity_num;
	u32 i;

	COND_RETURN_ERROR(pool_manager == NULL, -1, "illegal pool_manager\n");

	entity_num = pool_manager->entity_num;

	NPU_DRV_INFO("alloc entity:size=%lu * %u\n", sizeof(struct devdrv_entity_info), entity_num);
	// 1. entity
	pool_manager->entity = (struct devdrv_entity_info *)vmalloc(sizeof(struct devdrv_entity_info) * entity_num);
	COND_RETURN_ERROR(pool_manager->entity == NULL, -ENOMEM, "no mem to alloc entity info table\n");
	memset_s(pool_manager->entity, sizeof(struct devdrv_entity_info) * entity_num, 0x0,
		sizeof(struct devdrv_entity_info) * entity_num);
	for (i = 0; i < entity_num; i++) {
		pool_manager->entity[i].index = i;
	}

	// 2. entity_status_bitmap
	pool_manager->entity_status_bitmap= (unsigned long *)kmalloc(
		sizeof(unsigned long) * BITS_TO_LONGS(entity_num), GFP_KERNEL);
	if(pool_manager->entity_status_bitmap == NULL) {
		vfree(pool_manager->entity);
		NPU_DRV_ERR("no mem to alloc entity status table\n");
		return -ENOMEM;
	}
	bitmap_fill(pool_manager->entity_status_bitmap, entity_num);
	return 0;
}

static int devdrv_pool_manager_destroy_entity_info(struct devdrv_pool_manager *pool_manager)
{
	COND_RETURN_ERROR(pool_manager == NULL, -1, "illegal pool_manager\n");
	if(pool_manager->entity) {
		vfree(pool_manager->entity);
		pool_manager->entity = NULL;
		if (pool_manager->entity_status_bitmap) {
			kfree(pool_manager->entity_status_bitmap);
			pool_manager->entity_status_bitmap = NULL;
		}
	}
	return 0;
}

static int devdrv_pool_manager_init_pool_info(struct devdrv_pool_manager *pool_manager)
{
	u32 entity_num;
	u32 entity_size;
	u8 pool_num;
	int i;

	COND_RETURN_ERROR(pool_manager == NULL, -1, "illegal pool_manager\n");
	entity_num = pool_manager->entity_num;
	entity_size = pool_manager->entity_size;
	pool_num = pool_manager->pool_num;

	NPU_DRV_INFO("devdrv_pool_regist to alloc pools:size=%lu * %u\n", sizeof(struct devdrv_pool_info), pool_num);
	pool_manager->pool = (struct devdrv_pool_info *)kmalloc(sizeof(struct devdrv_pool_info) * pool_num, GFP_KERNEL);
	if (pool_manager->pool == NULL) {
		NPU_DRV_ERR("no mem to alloc pool info table\n");
		return -ENOMEM;
	}
	memset_s(pool_manager->pool, sizeof(struct devdrv_pool_info) * pool_num, 0x0,
		sizeof(struct devdrv_pool_info) * pool_num);

	for (i = 0; i < pool_num; i++)
		pool_manager->pool[i].entity_num = entity_num / pool_num;

	// alloc the first pool
	if (pool_manager->alloc_pool(pool_manager->pool[0].entity_num * entity_size,
		&pool_manager->pool[0].virt_addr, &pool_manager->pool[0].iova) != 0) {
		NPU_DRV_ERR("no mem to alloc the first pool\n");
		kfree(pool_manager->pool);
		return -ENOMEM;
	}
	pool_manager->pool[0].is_created = 1;
	pool_manager->pool_created = 1;
	return 0;
}
static int devdrv_pool_manager_destroy_pool_info(struct devdrv_pool_manager *pool_manager)
{
	u32 i;
	COND_RETURN_ERROR(pool_manager == NULL, -1, "illegal pool_manager\n");
	// 1. free pool
	for (i = 0; i < pool_manager->pool_created; i++)
		pool_manager->free_pool(pool_manager->pool[i].virt_addr,
			pool_manager->pool[i].iova);

	pool_manager->pool_created = 0;

	// 2. free pool info table
	kfree(pool_manager->pool);
	return 0;
}
int devdrv_pool_regist(u8 dev_id, tag_devdrv_mem_pool_type type, u32 start_id, u32 entity_num, u32 entity_size, u8 pool_num,
                       alloc_pool_func alloc_pool, free_pool_func free_pool)
{
	struct devdrv_pool_manager *pool_manager = NULL;
	int ret = 0;
	COND_RETURN_ERROR(dev_id >= NPU_DEV_NUM, -1, "illegal npu dev id: %d\n", dev_id);
	COND_RETURN_ERROR(type >= DEVDRV_MEM_POOL_TYPE_MAX, -1, "illegal type: %d\n", type);
	COND_RETURN_ERROR(pool_num < 1 || entity_num < pool_num || entity_num % pool_num != 0, -1
		, "illegal info:entity_num=%d, entity_size=%d, pool_num=%d\n", entity_num, entity_size, pool_num);
	COND_RETURN_ERROR(alloc_pool == NULL || free_pool == NULL, -1, "illegal func:alloc_pool=%pK, free_pool=%pK\n"
		, alloc_pool, free_pool);

	NPU_DRV_INFO("devdrv_pool_regist  enter:entity_num=%d, entity_size=%d, pool_num=%d\n",
		entity_num, entity_size, pool_num);
	pool_manager = &(g_pool_manager[dev_id][type]);

	mutex_init(&pool_manager->lock);
	pool_manager->start_id = start_id;
	pool_manager->entity_num = entity_num;
	pool_manager->entity_size = entity_size;
	pool_manager->pool_num = pool_num;
	pool_manager->alloc_pool = alloc_pool;
	pool_manager->free_pool = free_pool;
	// 1. init id list
	ret = devdrv_pool_manager_init_entity_info(pool_manager);
	COND_RETURN_ERROR(ret != 0, ret, "devdrv_pool_manager_init_id fail: %d\n", ret);

	// 2. init pool info table
	ret = devdrv_pool_manager_init_pool_info(pool_manager);
	if(ret != 0) {
		NPU_DRV_ERR("devdrv_pool_manager_init_pool_info fail: %d\n", ret);
		devdrv_pool_manager_destroy_entity_info(pool_manager);
	}
	return ret;
}
int devdrv_pool_unregist(u8 dev_id, tag_devdrv_mem_pool_type type)
{
	struct devdrv_pool_manager *pool_manager = NULL;

	NPU_DRV_DEBUG("enter cur dev %d type %d\n", dev_id, type);

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id: %d\n", dev_id);
		return -1;
	}

	if (type >= DEVDRV_MEM_POOL_TYPE_MAX) {
		NPU_DRV_ERR("illegal type:%d\n", type);
		return -1;
	}

	pool_manager = &g_pool_manager[dev_id][type];
	mutex_lock(&pool_manager->lock);
	// 1. free pool
	devdrv_pool_manager_destroy_pool_info(pool_manager);

	// 2. free entity info table
	devdrv_pool_manager_destroy_entity_info(pool_manager);

	memset_s(&g_pool_manager[dev_id][type], sizeof(struct devdrv_pool_manager), 0, sizeof(struct devdrv_pool_manager));
	mutex_unlock(&pool_manager->lock);
	mutex_destroy(&pool_manager->lock);
	return 0;
}

static int devdrv_pool_manager_check_extend_pool(struct devdrv_pool_manager *pool_manager, u32 pool_index)
{
	u32 pool_size;
	int ret;
	COND_RETURN_ERROR(pool_manager == NULL, -1, "illegal pool_manager\n");

	if (pool_manager->pool_created < pool_manager->pool_num && pool_index >= pool_manager->pool_created - 1) {
		// there is any more pool to apply, and pool_index is the id of the last pool current.
		// Note: theoretically '>' is impossible, unless there was an expansion failure before
		if (pool_manager->pool[pool_index].used_num >
			DEVDRV_MEM_POOL_USE_THRESHOLD(pool_manager->pool[pool_index].entity_num)) {
			pool_size = pool_manager->entity_size * pool_manager->pool[0].entity_num;
			ret = pool_manager->alloc_pool(pool_size,
				&pool_manager->pool[pool_index + 1].virt_addr,
				&pool_manager->pool[pool_index + 1].iova);
			if (ret != 0) {
				NPU_DRV_ERR("no mem to alloc the %dth pool\n", pool_index + 1);
				return -1;
			}
			pool_manager->pool[pool_index + 1].is_created = 1;
			pool_manager->pool[pool_index + 1].used_num = 0;
			pool_manager->pool_created++;
			NPU_DRV_WARN("extend pool:current pool num=%d\n", pool_manager->pool_created);
		}
	}
	return 0;
}
static int devdrv_pool_manager_check_shink_pool(struct devdrv_pool_manager *pool_manager)
{
	while (pool_manager->pool_created > 1) {
		if (pool_manager->pool[pool_manager->pool_created - 1].used_num != 0 ||
			pool_manager->pool[pool_manager->pool_created - 2].used_num >
			DEVDRV_MEM_POOL_USE_THRESHOLD(pool_manager->pool[0].entity_num))
			// the last pool is not empty, or the second last pool's usage rate is over *
			break;

		pool_manager->free_pool(pool_manager->pool[pool_manager->pool_created - 1].virt_addr,
			pool_manager->pool[pool_manager->pool_created - 1].iova);
		pool_manager->pool[pool_manager->pool_created - 1].virt_addr = 0;
		pool_manager->pool[pool_manager->pool_created - 1].iova = 0;
		pool_manager->pool[pool_manager->pool_created - 1].is_created = 0;
		pool_manager->pool_created--;
		NPU_DRV_WARN("shrink pool:current pool num=%d\n", pool_manager->pool_created);
	}
	return 0;
}
struct devdrv_entity_info *devdrv_alloc_entity(u8 dev_id, tag_devdrv_mem_pool_type type)
{
	struct devdrv_pool_manager *pool_manager = NULL;
	struct devdrv_entity_info *entity_info = NULL;
	u32 pool_index;
	unsigned long index_offset;
	unsigned long index;

	COND_RETURN_ERROR(dev_id >= NPU_DEV_NUM, NULL, "illegal npu dev id: %d\n", dev_id);
	COND_RETURN_ERROR(type >= DEVDRV_MEM_POOL_TYPE_MAX, NULL, "illegal type:%d\n", type);

	pool_manager = &g_pool_manager[dev_id][type];
	COND_RETURN_ERROR(pool_manager == NULL || pool_manager->entity_num == 0, NULL,
		"devdrv_alloc_entity failed, dev=%d type=%d has not registed!\n", dev_id, type);

	mutex_lock(&pool_manager->lock);
	// 1. select the first entity info node from entity info list
	index = find_first_bit(pool_manager->entity_status_bitmap, pool_manager->entity_num);
	COND_GOTO_ERROR(index >= pool_manager->entity_num, out, entity_info, NULL,
		"cur dev %d type %d available entity list empty!!!\n", dev_id, type);
	entity_info = &pool_manager->entity[index];
	NPU_DRV_INFO("first index = %lu", index);
	COND_RETURN_ERROR(entity_info == NULL, NULL, "entity_info = NULL!\n");

	/* There is no case where the pool to which the id belongs is not applied,
	  because the usage rate of the last pool exceeds 80%, it is necessary to apply for a new pool.
	*/
	pool_index = entity_info->index / pool_manager->pool[0].entity_num;

	// 2.  Check if you need to extend the pool
	COND_GOTO_ERROR(devdrv_pool_manager_check_extend_pool(pool_manager, pool_index) != 0, out,
		entity_info, NULL, "devdrv_pool_manager_check_extend_pool fail:poolindex=%u\n", pool_index);

	// 3. Fill in the address information of the entity_info
	bitmap_clear(pool_manager->entity_status_bitmap, index, 1);

	pool_manager->pool[pool_index].used_num++;
	index_offset = (entity_info->index % pool_manager->pool[pool_index].entity_num) *
		((u64)pool_manager->entity_size);
	entity_info->vir_addr = pool_manager->pool[pool_index].virt_addr + index_offset;
	entity_info->iova = pool_manager->pool[pool_index].iova + index_offset;
	entity_info->index += pool_manager->start_id;

	NPU_DRV_DEBUG("cur dev %d type %d pool %d used %d entity\n", dev_id, type, pool_index,
		pool_manager->pool[pool_index].used_num);
	NPU_DRV_DEBUG("pool %d virt=%llx, iova=%llx\n", pool_index, pool_manager->pool[pool_index].virt_addr,
		pool_manager->pool[pool_index].iova);
	NPU_DRV_DEBUG("pool entity num=%d, entity size=%d\n", pool_manager->pool[pool_index].entity_num,
		pool_manager->entity_size);
	NPU_DRV_DEBUG("alloc entity=%d, vir_addr=%llx, iova=%llx\n", entity_info->index,
		entity_info->vir_addr, entity_info->iova);
out:
	mutex_unlock(&pool_manager->lock);
	return entity_info;
}

int devdrv_free_entity(u8 dev_id, tag_devdrv_mem_pool_type type, u32 entity_id)
{
	struct devdrv_pool_manager *pool_manager = NULL;
	struct devdrv_entity_info *entity_info = NULL;
	int pool_index;
	int ret = 0;
	u32 index = 0;

	NPU_DRV_INFO("enter cur dev %d type %d entity %d\n", dev_id, type, index);
	COND_RETURN_ERROR(dev_id >= NPU_DEV_NUM, -1, "illegal npu dev id: %d\n", dev_id);
	COND_RETURN_ERROR(type >= DEVDRV_MEM_POOL_TYPE_MAX, -1, "illegal type:%d\n", type);

	pool_manager = &g_pool_manager[dev_id][type];
	COND_RETURN_ERROR(pool_manager == NULL || pool_manager->entity_num == 0, -1,
		"devdrv_free_entity failed, dev=%d type=%d has not registed!\n", dev_id, type);
	COND_RETURN_ERROR(entity_id < pool_manager->start_id, -1, "illegal index:%d, start id:%d\n", index, pool_manager->start_id);

	mutex_lock(&pool_manager->lock);
	index = entity_id - pool_manager->start_id;
	COND_GOTO_ERROR(index >= pool_manager->entity_num, out, ret, -1,
		"illegal entity %d\n", index);

	entity_info = &pool_manager->entity[index];
	entity_info->index = index;

// 1. clear the entity
	COND_GOTO_ERROR(test_and_set_bit(index, pool_manager->entity_status_bitmap), out, ret, -1,
		"the entity has not be alloced, index=%d", index);

// 2. update pool info
	pool_index = index / pool_manager->pool[0].entity_num;
	pool_manager->pool[pool_index].used_num--;

// 3. Check if you want to shrink the pool
	devdrv_pool_manager_check_shink_pool(pool_manager);

	NPU_DRV_DEBUG("cur dev %d type %d pool %d used %d entity\n", dev_id, type, pool_index,
		pool_manager->pool[pool_index].used_num);
out:
	mutex_unlock(&pool_manager->lock);
	return ret;
}

