/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: this file define fd maps
 * Author: donghaixu
 * Create: 2020-05-25
 */

#include "ivp_map.h"
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/ion.h>
#include <linux/dma-buf.h>
#include <linux/hisi-iommu.h>
#include <linux/hisi/hisi_ion.h>
#include "ivp_log.h"
#ifdef SEC_IVP_V300
#include "ivp_ca.h"
#endif

#ifndef EOK
#define EOK 0
#endif

#define DMA_BUF_REF_MAX 20

enum DMA_BUF_STATUS {
	FREE = 0,
	WORK
};

struct ion_buffer {
	u64 magic;
	union {
		struct rb_node node;
		struct list_head list;
	};
	struct ion_device *dev;
	struct ion_heap *heap;
	unsigned long flags;
	unsigned long private_flags;
	size_t size;
	void *priv_virt;
	struct mutex lock;
	int kmap_cnt;
	void *vaddr;
	struct sg_table *sg_table;
	struct list_head attachments;
	char task_comm[TASK_COMM_LEN];
	pid_t pid;
	unsigned int id;
#ifdef CONFIG_HISI_LB
	unsigned int plc_id;
	unsigned long offset;
	size_t lb_size;
#endif
};

struct dma_buf_ref {
	int fd;
	int status;
	struct dma_buf *dmabuf;
};

struct dma_buf_ref g_dma_ref[DMA_BUF_REF_MAX];

int ivp_map_hidl_fd(struct device *dev, struct ivp_map_info *map_buf)
{
	struct dma_buf *buf = NULL;

	if (!dev || !map_buf) {
		ivp_err("invalid input");
		return -EINVAL;
	}

	if (map_buf->fd < 0) {
		ivp_err("fd is invalid");
		return -EINVAL;
	}

	buf = dma_buf_get(map_buf->fd);
	if (IS_ERR(buf)) {
		ivp_err("invalid file shared_fd = %d", map_buf->fd);
		return -EINVAL;
	}

	map_buf->iova = hisi_iommu_map_dmabuf(dev, buf,
		map_buf->fd_prot, &(map_buf->mapped_size));
	if (map_buf->iova == 0) {
		ivp_err("fail to map iommu iova");
		dma_buf_put(buf);
		return -ENOMEM;
	}

	dma_buf_put(buf);
	return EOK;
}

int ivp_unmap_hidl_fd(struct device *dev,
		struct ivp_map_info *map_buf)
{
	int ret = EOK;
	struct dma_buf *buf = NULL;

	if (!dev || !map_buf) {
		ivp_err("invalid input");
		return -EINVAL;
	}

	if (map_buf->fd < 0) {
		ivp_err("fd is invalid");
		return -EINVAL;
	}

	buf = dma_buf_get(map_buf->fd);
	if (IS_ERR(buf)) {
		ivp_err("fail to get dma dmabuf, fd = %d", map_buf->fd);
		return -EINVAL;
	}

	if (hisi_iommu_unmap_dmabuf(dev, buf, map_buf->iova) < 0) {
		ivp_err("fail to unmap fd = %d", map_buf->fd);
		ret = -ENOMEM;
	}

	map_buf->iova = 0;
	dma_buf_put(buf);
	return ret;
}

#ifdef SEC_IVP_V300
static int ivp_get_sfd(int fd, unsigned int *sfd)
{
	struct dma_buf *dmabuf = NULL;
	struct ion_buffer *buffer = NULL;

	if (fd < 0 || !sfd) {
		ivp_err("invalid input");
		return -EINVAL;
	}

	dmabuf = dma_buf_get(fd);
	if (IS_ERR(dmabuf)) {
		ivp_err("dma_buf_get fail, fd=%d", fd);
		return -EINVAL;
	}

	buffer = dmabuf->priv;
	if (!buffer) {
		ivp_err("dmabuf has no ION buffer");
		dma_buf_put(dmabuf);
		return -EINVAL;
	}

	*sfd = buffer->id;
	dma_buf_put(dmabuf);

	return EOK;
}

static int ivp_get_sgl_info(int fd, struct scatterlist **sgl,
		unsigned int *nents)
{
	struct dma_buf *dmabuf = NULL;
	struct ion_buffer *buffer = NULL;
	struct sg_table *table = NULL;

	if (fd < 0 || !sgl || !nents) {
		ivp_err("invalid input");
		return -EINVAL;
	}

	dmabuf = dma_buf_get(fd);
	if (IS_ERR(dmabuf)) {
		ivp_err("dma_buf_get fail, fd=%d", fd);
		return -EINVAL;
	}

	if (!is_ion_dma_buf(dmabuf)) {
		ivp_err("invalid dmabuf");
		dma_buf_put(dmabuf);
		return -EINVAL;
	}

	buffer = dmabuf->priv;
	if (!buffer) {
		ivp_err("dmabuf has no ION buffer");
		dma_buf_put(dmabuf);
		return -EINVAL;
	}

	table = buffer->sg_table;
	if (!table) {
		ivp_err("ION buffer's sg_table is NULL");
		dma_buf_put(dmabuf);
		return -EINVAL;
	}

	if (!table->sgl) {
		ivp_err("sgl is null");
		dma_buf_put(dmabuf);
		return -EINVAL;
	}

	*sgl = table->sgl;
	*nents = table->nents;
	dma_buf_put(dmabuf);

	return EOK;
}

static int ivp_get_mmu_info(int fd, struct sglist **sglist)
{
	struct scatterlist *sg = NULL;
	struct scatterlist *sgl = NULL;
	struct page *page = NULL;
	struct sglist *tmp_sgl = NULL;
	unsigned int nents = 0;
	unsigned int i = 0;
	unsigned int sgl_size;

	if (fd < 0 || !sglist) {
		ivp_err("invalid input");
		return -EINVAL;
	}

	if (ivp_get_sgl_info(fd, &sgl, &nents)) {
		ivp_err("fail ivp_get_sgl_info");
		return -EFAULT;
	}

	sgl_size = sizeof(struct sglist) + nents * sizeof(struct ion_page_info);
	tmp_sgl = (struct sglist *)kzalloc(sgl_size, GFP_KERNEL);
	if (!tmp_sgl) {
		ivp_err("kzalloc tmp_sgl mem failed");
		return -ENOMEM;
	}

	tmp_sgl->sglist_size = sgl_size;
	tmp_sgl->info_length = nents;

	for_each_sg(sgl, sg, tmp_sgl->info_length, i) {
		page = sg_page(sg);
		tmp_sgl->page_info[i].phys_addr = page_to_phys(page);
		tmp_sgl->page_info[i].npages = sg->length / PAGE_SIZE;
	}

	*sglist = tmp_sgl;

	return EOK;
}

int ivp_add_dma_ref(int fd)
{
	unsigned int i;

	if (fd < 0) {
		ivp_err("invalid input");
		return -EINVAL;
	}

	ivp_info("enter fd = %d", fd);
	for (i = 0; i < DMA_BUF_REF_MAX; i++) {
		if (g_dma_ref[i].status == FREE) {
			g_dma_ref[i].dmabuf = dma_buf_get(fd);
			if (IS_ERR(g_dma_ref[i].dmabuf)) {
				ivp_err("dma_buf_get fail, fd = %d", fd);
				g_dma_ref[i].dmabuf = NULL;
				return -EINVAL;
			}
			g_dma_ref[i].fd = fd;
			g_dma_ref[i].status = WORK;
			return EOK;
		}
	}

	ivp_err("no dma ref node");
	return -ENOMEM;
}

static void ivp_check_clear_ta(void)
{
	unsigned int i;
	ivp_info("enter");

	for (i = 0; i < DMA_BUF_REF_MAX; i++) {
		if (g_dma_ref[i].status == WORK) {
			ivp_warn("clear last dma info");
			teek_secivp_clear();
			break;
		}
	}
}

static void ivp_clear_dma_ref(void)
{
	unsigned int i;
	ivp_info("enter");

	ivp_check_clear_ta();
	for (i = 0; i < DMA_BUF_REF_MAX; i++) {
		if (g_dma_ref[i].status == WORK) {
			dma_buf_put(g_dma_ref[i].dmabuf);
			g_dma_ref[i].fd = 0;
			g_dma_ref[i].status = FREE;
			g_dma_ref[i].dmabuf = NULL;
		}
	}
	return;
}

int ivp_init_map_info(void)
{
	ivp_info("enter");

	if (teek_secivp_ca_probe()) {
		ivp_err("ca probe failed");
		return -EPERM;
	}
	ivp_clear_dma_ref();
	return EOK;
}

void ivp_clear_map_info(void)
{
	ivp_info("enter");

	ivp_clear_dma_ref();
	teek_secivp_ca_remove();
	return;
}

int ivp_init_sec_fd(struct ivp_fd_info *info)
{
	unsigned int sfd;
	struct sglist *sgl = NULL;

	if (!info || info->fd < 0) {
		ivp_err("invalid input");
		return -EINVAL;
	}

	info->iova = 0;
	if (info->sec_flag == 0) {
		if (teek_secivp_sec_cfg(info->fd, info->size) != EOK) {
			ivp_err("sec cfg failed");
			return -EPERM;
		}
		/* secure memory need do fd to sfd */
		if (ivp_get_sfd(info->fd, &sfd)) {
			teek_secivp_sec_uncfg(info->fd, info->size);
			ivp_err("ivp get sfd failed");
			return -EPERM;
		}
		info->iova = teek_secivp_secmem_map(sfd, info->size);
		if (info->iova == 0) {
			ivp_err("secmem map failed");
			teek_secivp_sec_uncfg(info->fd, info->size);
			return -EPERM;
		}
	} else {
		/* non-secure memory need do fd to sg_list */
		if (ivp_get_mmu_info(info->fd, &sgl)) {
			ivp_err("get mmu info fail");
			return -EPERM;
		}
		info->iova = teek_secivp_nonsecmem_map(info->fd,
			info->size, sgl);
		kfree(sgl);
		if (info->iova == 0) {
			ivp_err("nonsecmem map failed");
			return -EPERM;
		}
	}

	return ivp_add_dma_ref(info->fd);
}

int ivp_deinit_sec_fd(struct ivp_fd_info *info)
{
	int ret = EOK;
	unsigned int sfd;

	if (!info || info->fd < 0) {
		ivp_err("invalid input");
		return -EINVAL;
	}

	if (info->sec_flag == 0) {
		if (!ivp_get_sfd(info->fd, &sfd)) {
			if (teek_secivp_secmem_unmap(sfd, info->size) != EOK) {
				ivp_err("secmem unmap failed");
				ret = -EPERM;
			}
		} else {
			ivp_err("get sfd failed");
			ret = -EPERM;
		}
		if (teek_secivp_sec_uncfg(info->fd, info->size) != EOK) {
			ivp_err("sec uncfg failed");
			ret = -EPERM;
		}
	} else {
		if (teek_secivp_nonsecmem_unmap(info->fd, info->size) != EOK) {
			ivp_err("nonsecmem unmap failed");
			ret = -EPERM;
		}
	}

	return ret;
}
#endif

