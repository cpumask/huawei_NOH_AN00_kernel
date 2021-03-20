/*
 * hisilicon driver, hisp_vdev.c
 *
 * Copyright (c) 2013- Hisilicon Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/platform_data/remoteproc_hisi.h>
#include <linux/scatterlist.h>
#include <linux/hisi-iommu.h>


/*
 * hisi viring alloc struct
 */
#ifdef CONFIG_HISI_REMOTEPROC_DMAALLOC_DEBUG
enum hisi_isp_vring_e {
	VRING0          = 0x0,
	VRING1,
	VIRTQUEUE,
};
struct hisi_isp_vring_s {
	u64 paddr;
	void *virt_addr;
	size_t size;
};

struct hisp_virtio_dev {
	struct hisi_isp_vring_s hisi_isp_vring[HISI_ISP_VRING_NUM];
};

static struct hisp_virtio_dev g_hisp_vdev;
#endif

unsigned long hisp_sg2virtio(struct virtqueue *vq, struct scatterlist *sg)
{
	struct virtio_device *vdev = NULL;
	struct rproc_vdev *rvdev = NULL;
	struct rproc *rproc = NULL;
	dma_addr_t dma;

	if (!vq || !sg) {
		pr_err("%s: vq or sg is NULL!\n", __func__);
		return 0;
	}

	vdev = vq->vdev;
	rvdev = container_of(vdev, struct rproc_vdev, vdev);
	rproc = rvdev->rproc;

	dma = sg_dma_address(sg);
	return ((dma - vq->dma_base) + rproc->ipc_addr);
}

/* rpmsg init msg dma and device addr */
/*lint -e548 */
void virtqueue_sg_init(struct scatterlist *sg,
			const void *va, dma_addr_t dma, int size)
{
	if (is_vmalloc_addr(va)) {
		sg_init_table(sg, 1);
		sg_set_page(sg, vmalloc_to_page(va), size, offset_in_page(va));
	} else {
		WARN_ON(!virt_addr_valid(va)); /*lint !e146 !e665*/
		sg_init_one(sg, va, size);
	}
	sg_dma_address(sg) = dma;
	sg_dma_len(sg) = size;
}
/*lint +e548 */

/*lint -save -e429*/
static int rpmsg_vdev_map_resource_sec(struct rproc_vdev *rvdev, dma_addr_t dma)
{
	int i;
	struct rproc_vring *rvring = NULL;
	unsigned int vr_size;
	int size;
	int index;
	int ret;

	if (rvdev == NULL) {
		pr_err("%s: rvdev is NULL\n", __func__);
		return -EINVAL;
	}

	if (!sec_process_use_ca_ta()) {
		/* map vring */
		for (i = 0; i < RVDEV_NUM_VRINGS; i++) {
			rvring = &rvdev->vring[i];
			index = A7VRING0 + i;
			vr_size = vring_size(rvring->len, rvring->align);
			size = PAGE_ALIGN(vr_size);/*lint !e666 */
			ret = hisp_mem_type_pa_init(index, rvring->dma);
			if (ret < 0) {
				pr_err("[%s] Failed : hisp_meminit.%d.(0x%x)\n",
					__func__, ret, index);
				return ret;
			}
		}

		/* map virtqueue */
		index = A7VQ;
		ret = hisp_mem_type_pa_init(index, dma);
		if (ret < 0) {
			pr_err("[%s] Failed : hisp_meminit.%d.(0x%x)\n",
				__func__, ret, index);
			return ret;
		}
	}
	return 0;
}

static int rpmsg_vdev_map_vring(struct rproc_mem_entry **vringmapping,
	unsigned int map_size, struct rproc_vdev *rvdev)
{
	unsigned int i;
	int prot;
	struct rproc_vring *rvring = NULL;
	unsigned int vr_size;
	int size;
	int ret;
	int unmaped;
	struct device *subdev = NULL;

	if ((vringmapping == NULL) || (rvdev == NULL)) {
		pr_err("%s: vringmapping or rvdev is NULL\n", __func__);
		return -1;
	}

	if (map_size != RVDEV_NUM_VRINGS) {
		pr_err("%s: error.map_size.%d\n", __func__, map_size);
		return -1;
	}

	if ((rvdev->rproc == NULL) || (rvdev->rproc->domain == NULL)) {
		pr_err("[%s] Failed : domain don't exist!\n", __func__);
		return -EINVAL;
	}

	subdev = get_isp_device();
	if (subdev == NULL) {
		pr_err("%s: platform dev is NULL!\n", __func__);
		return -1;
	}

	for (i = 0; i < map_size; i++) {
		prot = IOMMU_READ | IOMMU_WRITE;
		rvring = &rvdev->vring[i];
		vr_size = vring_size(rvring->len, rvring->align);
		size = PAGE_ALIGN(vr_size);/*lint !e666 */

		ret = iommu_map(rvdev->rproc->domain, rvring->da,
				rvring->dma, size, prot);
		if (ret) {
			pr_err("[%s] Failed : iommu_map.%d\n", __func__, ret);
			return ret;
		}

		vringmapping[i] = kzalloc(sizeof(**vringmapping), GFP_KERNEL);
		if (!vringmapping[i]) {
			if (hisp_get_smmuc3_flag() == 1) {
				unmaped = hisi_iommu_unmap_fast(subdev, rvring->da, size);
			} else {
				unmaped = iommu_unmap(rvdev->rproc->domain,
							rvring->da, size);
			}
			if (unmaped != size)
				pr_err("[%s] Failed : size.%u, unmaped.%u\n",
					__func__, size, unmaped);
			ret = -ENOMEM;
			return ret;
		}

		vringmapping[i]->da = rvring->da;
		vringmapping[i]->len = size;
		list_add_tail(&vringmapping[i]->node, &rvdev->rproc->mappings);
	}

	return 0;
}

static int rpmsg_vdev_map_vq(struct rproc_vdev *rvdev,
		dma_addr_t dma, int total_space)
{
	int ret;
	int prot;
	int unmaped;
	struct rproc_mem_entry *vqmapping = NULL;
	struct device *subdev = NULL;

	if ((rvdev->rproc == NULL) || (rvdev->rproc->domain == NULL)) {
		pr_err("[%s] Failed : domain don't exist!\n", __func__);
		return -EINVAL;
	}

	subdev = get_isp_device();
	if (subdev == NULL) {
		pr_err("%s: platform dev is NULL!\n", __func__);
		return -EINVAL;
	}

	prot = IOMMU_READ | IOMMU_WRITE;
	ret = iommu_map(rvdev->rproc->domain, rvdev->rproc->ipc_addr,
		(phys_addr_t)(dma), total_space, prot);
	if (ret) {
		pr_err("[%s] Failed : iommu_map.%d\n", __func__, ret);
		return -ENOMEM;
	}

	vqmapping = kzalloc(sizeof(struct rproc_mem_entry), GFP_KERNEL);
	if (vqmapping == NULL) {
		if (hisp_get_smmuc3_flag() == 1) {
			unmaped = hisi_iommu_unmap_fast(subdev, rvdev->rproc->ipc_addr, total_space);
		} else {
			unmaped = iommu_unmap(rvdev->rproc->domain,
					rvdev->rproc->ipc_addr, total_space);
		}
		if (unmaped != total_space)
			pr_err("[%s] Failed :iommu_unmap size.%u, unmaped.%u\n",
				__func__, total_space, unmaped);
		return -ENOMEM;
	}
	vqmapping->da = rvdev->rproc->ipc_addr;
	vqmapping->len = total_space;
	list_add_tail(&vqmapping->node, &rvdev->rproc->mappings);

	return 0;
}

static void rpmsg_vdev_unmap_vring(struct rproc_mem_entry **vringmapping,
	unsigned int map_size, struct rproc_vdev *rvdev)
{
	int size;
	unsigned int j;
	unsigned int vr_size;
	struct rproc_vring *rvring = NULL;
	int unmaped;
	struct device *subdev = NULL;

	if ((vringmapping == NULL) || (rvdev == NULL)) {
		pr_err("%s: vringmapping or rvdev is NULL\n", __func__);
		return;
	}

	if (map_size != RVDEV_NUM_VRINGS) {
		pr_err("%s: error.map_size.%d\n", __func__, map_size);
		return;
	}

	if ((rvdev->rproc == NULL) || (rvdev->rproc->domain == NULL)) {
		pr_err("[%s] Failed : domain don't exist!\n", __func__);
		return;
	}

	subdev = get_isp_device();
	if (subdev == NULL) {
		pr_err("%s: platform dev is NULL!\n", __func__);
		return;
	}

	for (j = 0; j < map_size; j++) {
		if (vringmapping[j]) {
			list_del(&vringmapping[j]->node);
			kfree(vringmapping[j]);
			rvring = &rvdev->vring[j];
			vr_size = vring_size(rvring->len, rvring->align);
			size = PAGE_ALIGN(vr_size);/*lint !e666 */
			if (hisp_get_smmuc3_flag() == 1) {
				unmaped = hisi_iommu_unmap_fast(subdev, rvring->da, size);
			} else {
				unmaped = iommu_unmap(rvdev->rproc->domain,
						rvring->da, size);
			}
			if (unmaped != size)
				pr_err("[%s] Failed:unmap size.%u,unmaped.%u\n",
					__func__, size, unmaped);
		}
	}
}

int rpmsg_vdev_map_resource(struct virtio_device *vdev,
				dma_addr_t dma, int total_space)
{
	struct rproc_vdev *rvdev = NULL;
	struct rproc_mem_entry *vringmapping[RVDEV_NUM_VRINGS] = {NULL, NULL};
	int ret = 0;

	pr_info("[%s] +\n", __func__);
	if (vdev == NULL) {
		pr_err("%s: vdev is NULL\n", __func__);
		return -EINVAL;
	}

	rvdev = container_of(vdev, struct rproc_vdev, vdev);

	if (use_sec_isp())
		return rpmsg_vdev_map_resource_sec(rvdev, dma);

	/* map vring */
	ret = rpmsg_vdev_map_vring(vringmapping, RVDEV_NUM_VRINGS, rvdev);
	if (ret < 0) {
		pr_err("[%s] Failed: rpmsg_vdev_map_vring.%d\n", __func__, ret);
		goto exit_vdevmap;
	}

	/* map virtqueue*/
	ret = rpmsg_vdev_map_vq(rvdev, dma, total_space);
	if (ret < 0) {
		pr_err("[%s] Failed : rpmsg_vdev_map_vq.%d\n", __func__, ret);
		goto exit_vdevmap;
	}

	return 0;

exit_vdevmap:
	rpmsg_vdev_unmap_vring(vringmapping, RVDEV_NUM_VRINGS, rvdev);
	return ret;
}
/*lint -restore */

#ifdef CONFIG_HISI_REMOTEPROC_DMAALLOC_DEBUG
void *get_vring_dma_addr(u64 *dma_handle, size_t size, unsigned int index)
{
	struct hisp_virtio_dev *rproc_dev = &g_hisp_vdev;
	struct hisi_isp_vring_s *hisi_isp_vring = NULL;

	pr_info("[%s] : +\n", __func__);
	if (rproc_dev == NULL) {
		pr_err("%s: rproc_boot_device in NULL\n", __func__);
		return NULL;
	}

	hisi_isp_vring = &rproc_dev->hisi_isp_vring[index];
	if (hisi_isp_vring == NULL) {
		pr_err("%s: hisi_isp_vring is NULL\n", __func__);
		return NULL;
	}

	if (hisi_isp_vring->size != size) {
		pr_err("%s: hisi_isp_vring size not same: 0x%lx --> 0x%lx\n",
				__func__, hisi_isp_vring->size, size);
		return NULL;
	}

	*dma_handle = hisi_isp_vring->paddr;
	pr_info("[%s] : -\n", __func__);
	return hisi_isp_vring->virt_addr;
}


int get_vring_dma_addr_probe(struct platform_device *pdev)
{
	struct hisp_virtio_dev *rproc_dev = &g_hisp_vdev;
	struct hisi_isp_vring_s *hisi_isp_vring = NULL;
	int ret = 0;
	int i = 0;

	if (pdev == NULL) {
		pr_err("%s: rproc_boot_device in NULL\n", __func__);
		return -ENOMEM;
	}

	for (i = 0; i < HISI_ISP_VRING_NUM; i++) {
		hisi_isp_vring = &rproc_dev->hisi_isp_vring[i];
		if (hisi_isp_vring == NULL) {
			pr_err("%s: hisi_isp_vring is NULL\n", __func__);
			ret = -ENOMEM;
			goto out;
		}

		if (i < (HISI_ISP_VRING_NUM - 1))
			hisi_isp_vring->size =  HISI_ISP_VRING_SIEZ;
		else
			hisi_isp_vring->size =  HISI_ISP_VQUEUE_SIEZ;

		hisi_isp_vring->virt_addr = dma_alloc_coherent(&pdev->dev,
			hisi_isp_vring->size, &hisi_isp_vring->paddr,
			GFP_KERNEL);
		if (!hisi_isp_vring->virt_addr) {
			pr_err("%s: hisi_isp_vring is NULL\n", __func__);
			ret = -ENOMEM;
			goto out;
		}
	}
	return 0;

out:
	for (; i >= 0; i--) {
		if (i >= HISI_ISP_VRING_NUM)
			continue;
		hisi_isp_vring = &rproc_dev->hisi_isp_vring[i];
		if (hisi_isp_vring == NULL)
			continue;

		dma_free_coherent(&pdev->dev, hisi_isp_vring->size,
			hisi_isp_vring->virt_addr, hisi_isp_vring->paddr);
	}

	return ret;
}

int get_vring_dma_addr_remove(struct platform_device *pdev)
{
	struct hisp_virtio_dev *rproc_dev = &g_hisp_vdev;
	struct hisi_isp_vring_s *hisi_isp_vring = NULL;
	unsigned int i = 0;

	for (i = 0; i < HISI_ISP_VRING_NUM; i++) {
		hisi_isp_vring = &rproc_dev->hisi_isp_vring[i];
		if (hisi_isp_vring == NULL) {
			pr_err("%s: hisi_isp_vring is NULL\n", __func__);
			return -ENOMEM;
		}

		dma_free_coherent(&pdev->dev, hisi_isp_vring->size,
			hisi_isp_vring->virt_addr, hisi_isp_vring->paddr);
	}
	return 0;
}
#endif

