#ifndef __NPU_IOVA_H
#define __NPU_IOVA_H

int devdrv_hisi_npu_iova_alloc(unsigned long long *virt_addr, unsigned long size);
void devdrv_hisi_npu_iova_free(unsigned long long virt_addr);
int devdrv_hisi_npu_iova_map(unsigned long long virt_addr, unsigned long *iova);
int devdrv_hisi_npu_iova_unmap(unsigned long long virt_addr, unsigned long iova);

int devdrv_hisi_npu_iova_map_ion(int fd, int prot, unsigned long *iova);
int devdrv_hisi_npu_iova_unmap_ion(int fd, unsigned long iova);

#endif
