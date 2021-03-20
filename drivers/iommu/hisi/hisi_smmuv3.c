/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 *
 * Description:
 * IOMMU API for Hisilicon SVM driver implementations based on ARM architected
 * SMMUv3.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: HISI_DRV1
 * Create: 2019-1-22
 */
#define pr_fmt(fmt) "smmuv3_svm:" fmt

#include <linux/device.h>
#include <linux/err.h>
#include <linux/iommu.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/slab.h>
#include <linux/sched/mm.h>
#include <linux/sched/task.h>
#include <asm/pgtable.h>
#include <asm/cacheflush.h>

#include <linux/hisi/hisi_bbox_diaginfo.h>
#include <linux/hisi/rdr_hisi_platform.h>
#include "hisi_bl31_smc.h"
#include "hisi_smmuv3.h"
#include "hisi_svm.h"
#include "hisi-svm-private.h"

static int svm_debug_level = svm_info;
module_param_named(level, svm_debug_level, int, 0444);

#define hisi_svm_print(level, x...)                                        \
	do { if (svm_debug_level >= (level)) pr_err(x); } while (0)

static struct mutex hisi_svm_mutex;

int hisi_smmu_poweron(struct device *dev)
{
	int ret;

	if (!dev) {
		pr_err("%s, dev is null\n", __func__);
		return -ENODEV;
	}

	mutex_init(&hisi_svm_mutex);
	ret = arm_smmu_poweron(dev);
	if (ret)
		dev_err(dev, "%s, smmu power on failed\n", __func__);
	return ret;
}

int hisi_smmu_poweroff(struct device *dev)
{
	int ret;

	if (!dev) {
		pr_err("%s, dev is null\n", __func__);
		return -ENODEV;
	}

	ret = arm_smmu_poweroff(dev);
	if (ret)
		dev_err(dev, "%s, smmu power off failed\n", __func__);
	return ret;
}

int hisi_svm_get_ssid(struct hisi_svm *svm, u16 *ssid, u64 *ttbr, u64 *tcr)
{
	int ret;

	if (!svm || !ssid || !ttbr || !tcr)
		return -EINVAL;

	if (!svm->dom)
		return -EINVAL;
	ret = arm_smmu_svm_get_ssid(svm->dom, ssid, ttbr, tcr);
	if (ret)
		hisi_svm_print(svm_error, "%s get ssid error!\n", __func__);

	return ret;
}

void hisi_svm_dump_pagetable(pgd_t *base_pgd, unsigned long addr, size_t size)
{
	size_t step;
	pgd_t *pgd = NULL;
	pud_t *pud = NULL;
	pmd_t *pmd = NULL;
	pte_t *pte = NULL;

	do {
		step = SZ_4K;
		pgd = pgd_offset_raw(base_pgd, addr);
		pr_err("[0x%08lx], *pgd=0x%016llx", addr, pgd_val(*pgd));
		if (pgd_none(*pgd) || pgd_bad(*pgd)) {
			step = SZ_1G;
			continue;
		}

		pud = pud_offset(pgd, addr);
		pr_err(", *pud=0x%016llx", pud_val(*pud));
		if (pud_none(*pud) || pud_bad(*pud)) {
			step = SZ_1G;
			continue;
		}

		pmd = pmd_offset(pud, addr);
		pr_err(", *pmd=0x%016llx", pmd_val(*pmd));
		if (pmd_none(*pmd) || pmd_bad(*pmd)) {
			step = SZ_2M;
			continue;
		}

		pte = pte_offset_map(pmd, addr);
		pr_err(", *pte=0x%016llx", pte_val(*pte));
		pte_unmap(pte);
		pr_err("\n");
	} while (size >= step && (addr += step, size -= step));
}

void hisi_svm_show_pte(struct hisi_svm *svm, unsigned long addr, size_t size)
{
	struct mm_struct *mm = NULL;
	struct task_struct *task = NULL;
	struct pid *bindpid = NULL;

	if (!svm || !svm->mm || !size)
		return;

	bindpid = find_get_pid(svm->pid);
	if (!bindpid) {
		pr_err("task exit,%d", svm->pid);
		return;
	}

	task = get_pid_task(bindpid, PIDTYPE_PID);
	if (!task) {
		pr_err("task exit,%d", svm->pid);
		put_pid(bindpid);
		return;
	}

	if (task->mm != svm->mm) {
		pr_err("task exit,%d, %pK, %pK", svm->pid, task->mm, svm->mm);
		put_pid(bindpid);
		put_task_struct(task);
		return;
	}

	put_pid(bindpid);
	put_task_struct(task);

	spin_lock(&svm->mm->page_table_lock);

	mm = svm->mm;
	pr_alert("cached pgd = %pK\n", mm->pgd);

	hisi_svm_dump_pagetable(mm->pgd, addr, size);

	spin_unlock(&svm->mm->page_table_lock);
}

int hisi_svm_flag_set(struct task_struct *task, u32 flag)
{
	struct mm_struct *mm = NULL;

	if (!task) {
		hisi_svm_print(svm_error, "%s param invalid!\n", __func__);
		return -EINVAL;
	}

	mm = get_task_mm(task);
	if (!mm) {
		hisi_svm_print(svm_error, "%s get_task_mm failed!\n", __func__);
		return -EINVAL;
	}
	if (flag)
		set_bit(MMF_SVM, &mm->flags);
	else
		clear_bit(MMF_SVM, &mm->flags);

	mmput(mm);
	hisi_svm_print(svm_info, "into %s, pid:%d,name:%s,pgd:%pK,flag:%d\n",
		__func__, task->pid, task->comm, mm->pgd, flag);
	return 0;
}

static int hisi_svm_set_task_info(struct hisi_svm *svm,
				struct task_struct *task)
{
	struct mm_struct *mm = NULL;

	svm->name = task->comm;
	mm = get_task_mm(task);
	if (!mm) {
		hisi_svm_print(svm_error, "%s get_task_mm err!\n", __func__);
		return -EINVAL;
	}
	svm->mm = mm;
	svm->pid = task->pid;
	/*
	 * Drop the reference to the mm_struct here. We rely on the
	 * mmu_notifier release call-back to inform us when the mm
	 * is going away.
	 */
	mmput(mm);

	return 0;
}

static int hisi_svm_domain_set_pgd(struct device *dev,
				struct iommu_domain *dom,
				struct task_struct *task)
{
	int ret;
	struct iommu_fwspec *fwspec = NULL;

	fwspec = dev->iommu_fwspec;
	if (!fwspec) {
		hisi_svm_print(svm_error, "%s fwspec is null\n", __func__);
		return -ENODEV;
	}

	if (!fwspec->ops) {
		hisi_svm_print(svm_error, "%s fwspec ops is null\n", __func__);
		return -ENODEV;
	}

	if (!fwspec->ops->domain_set_attr) {
		hisi_svm_print(svm_error, "%s domain_set_attr is null\n",
			__func__);
		return -ENODEV;
	}

	ret = fwspec->ops->domain_set_attr(dom, DOMAIN_ATTR_PGD, task);
	if (ret)
		hisi_svm_print(svm_error, "%s domain_set_attr pgd err %d\n",
			__func__, ret);

	return ret;
}

struct hisi_svm *hisi_svm_bind_task(struct device *dev,
				struct task_struct *task)
{
	struct hisi_svm *svm = NULL;
	struct iommu_domain *dom = NULL;
	struct iommu_group *group = NULL;

	hisi_svm_print(svm_info, "into %s\n", __func__);
	if (!dev || !task) {
		hisi_svm_print(svm_error, "%s param invalid!\n", __func__);
		return NULL;
	}

	mutex_lock(&hisi_svm_mutex);
	svm = kzalloc(sizeof(*svm), GFP_KERNEL);
	if (!svm) {
		hisi_svm_print(svm_error, "%s kzalloc failed!\n", __func__);
		goto out;
	}

	dom = iommu_domain_alloc(dev->bus);
	if (!dom) {
		hisi_svm_print(svm_error, "%s domain alloc err!\n", __func__);
		goto out_free;
	}
	svm->dom = dom;

	group = dev->iommu_group;
	if (hisi_svm_domain_set_pgd(dev, dom, task)) {
		hisi_svm_print(svm_error, "%s set task info err!\n", __func__);
		goto dom_free;
	}

	dev->iommu_group = NULL;
	if (iommu_attach_device(dom, dev)) {
		hisi_svm_print(svm_error, "%s attach device err!\n", __func__);
		goto dom_free;
	}
	svm->dev = dev;

	if (hisi_svm_set_task_info(svm, task)) {
		hisi_svm_print(svm_error, "%s set task info err!\n", __func__);
		goto iommu_attach;
	}

	dev->iommu_group = group;
	hisi_svm_print(svm_info, "out %s\n", __func__);
	mutex_unlock(&hisi_svm_mutex);
	return svm;

iommu_attach:
	iommu_detach_device(svm->dom, svm->dev);

dom_free:
	dev->iommu_group = group;
	iommu_domain_free(dom);

out_free:
	kfree(svm);

out:
	mutex_unlock(&hisi_svm_mutex);
	return NULL;
}

void hisi_svm_unbind_task(struct hisi_svm *svm)
{
	struct iommu_group *group = NULL;

	hisi_svm_print(svm_info, "in %s\n", __func__);
	if (!svm) {
		hisi_svm_print(svm_error, "%s:svm NULL\n", __func__);
		return;
	}

	mutex_lock(&hisi_svm_mutex);

	group = svm->dev->iommu_group;
	svm->dev->iommu_group = NULL;
	iommu_detach_device(svm->dom, svm->dev);
	svm->dev->iommu_group = group;

	iommu_domain_free(svm->dom);
	kfree(svm);

	mutex_unlock(&hisi_svm_mutex);
	hisi_svm_print(svm_info, "out %s\n", __func__);
}

static void pte_flush_range(pmd_t *pmd, unsigned long addr, unsigned long end)
{
	pte_t *pte = NULL;
	pte_t *pte4k = NULL;

	pte = pte_offset_map(pmd, addr);

	pte4k = (pte_t *)round_down((u64)pte, PAGE_SIZE);
	__flush_dcache_area(pte4k, PAGE_SIZE);

	pte_unmap(pte);
}

static void pmd_flush_range(pud_t *pud, unsigned long addr, unsigned long end)
{
	pmd_t *pmd = NULL;
	pmd_t *pmd4k = NULL;
	unsigned long next;

	pmd = pmd_offset(pud, addr);
	pmd4k = (pmd_t *)round_down((u64)pmd, PAGE_SIZE);

	do {
		next = pmd_addr_end(addr, end);
		pte_flush_range(pmd, addr, next);
		pmd++;
		addr = next;
	} while (addr != end);

	__flush_dcache_area(pmd4k, PAGE_SIZE);
}

static void pud_flush_range(pgd_t *pgd, unsigned long addr, unsigned long end)
{
	pud_t *pud = NULL;
#if CONFIG_PGTABLE_LEVELS > 3
	pud_t *pud4k = NULL;
#endif
	unsigned long next;

	pud = pud_offset(pgd, addr);
#if CONFIG_PGTABLE_LEVELS > 3
	pud4k = (pud_t *)round_down((u64)pud, PAGE_SIZE);
#endif

	do {
		next = pud_addr_end(addr, end);
		pmd_flush_range(pud, addr, next);
		pud++;
		addr = next;
	} while (addr != end);

#if CONFIG_PGTABLE_LEVELS > 3
	__flush_dcache_area(pud4k, PAGE_SIZE);
#endif
}

int hisi_svm_flush_cache(struct mm_struct *mm, unsigned long addr, size_t size)
{
	pgd_t *pgd = NULL;
	pgd_t *pgd4k = NULL;
	unsigned long next;
	unsigned long end = addr + PAGE_ALIGN(size);

	hisi_svm_print(svm_trace, "tlb-cache:map user addr[0x%8lx ~0x%8lx]\n",
		       addr, end);

	pgd = pgd_offset(mm, addr);
	pgd4k = (pgd_t *)round_down((u64)pgd, PAGE_SIZE);

	do {
		next = pgd_addr_end(addr, end);
		pud_flush_range(pgd, addr, next);
		pgd++;
		addr = next;
	} while (addr != end);

	__flush_dcache_area(pgd4k, PAGE_SIZE);

	arm_smmu_svm_tlb_inv_context(mm); /* flush tlb when mmap */

	return 0;
}

int hisi_smmu_evt_register_notify(struct device *dev, struct notifier_block *n)
{
	return arm_smmu_evt_register_notify(dev, n);
}

int hisi_smmu_evt_unregister_notify(struct device *dev,
				struct notifier_block *n)
{
	return arm_smmu_evt_unregister_notify(dev, n);
}

static noinline u64 arm_smmu_sec_config(u64 fid, int a1, int a2, int a3)
{
	register u64 x0 asm("x0") = fid;
	register u64 x1 asm("x1") = a1;
	register u64 x2 asm("x2") = a2;
	register u64 x3 asm("x3") = a3;

	asm volatile (
		__asmeq("%0", "x0")
		__asmeq("%1", "x1")
		__asmeq("%2", "x2")
		__asmeq("%3", "x3")

		"smc    #0\n"
		: "+r" (x0)
		: "r" (x1), "r" (x2), "r"(x3));

	return x0;
}

int hisi_smmu_nonsec_tcuctl(int smmuid)
{
	u64 ret;

	ret = arm_smmu_sec_config(HISI_SMMU_FID_TCUCTL_UNSEC, smmuid, 0, 0);
	pr_info("%s, smmuid:%d, ret:0x%llx\n", __func__, smmuid, ret);
	return 0;
}

int hisi_smmu_tcu_node_status(int smmuid)
{
	u64 ret;

	ret = arm_smmu_sec_config(HISI_SMMU_FID_NODE_STATUS, smmuid, 0, 0);
	pr_info("%s, smmuid:%d, ret:0x%llx\n", __func__, smmuid, ret);
	return 0;
}

int hisi_smmu_dmss_status(int smmuid)
{
	u64 ret;

	ret = arm_smmu_sec_config(HISI_SMMU_DUMP_DMSS_STATUS, smmuid, 0, 0);
	pr_info("%s, smmuid:%d, ret:0x%llx\n", __func__, smmuid, ret);
	return 0;
}

void hisi_smmu_dump_tbu_status(struct device *dev)
{
	if (!dev) {
		pr_err("%s input invalid\n", __func__);
		return;
	}
	arm_smmu_tbu_status_print(dev);
}

#ifdef CONFIG_HISI_IOMMU_DMD_REPORT
#define MAX_NAME_LENGTH		20
#define MAX_DMD_REG_INFO	2
static DEFINE_SPINLOCK(g_smmu_dmd_lock);
struct dmd_info {
	bool is_init;
	unsigned int err_id;
	char err_name[MAX_NAME_LENGTH];
	unsigned int reg_info[MAX_DMD_REG_INFO];
	void __iomem *reg_base[MAX_DMD_REG_INFO];
};

static struct dmd_info g_smmu_dmd_info[SMMU_INDEX_MAX] = {
	/* PCTRL PERI_CTRL2, ACTRL AO_MEM_CTRL10 */
	{false, SMMU_NPU_NOC_INFO, {"NPU_TCU"}, {0xFEC3E00C, 0xFA894848}, {NULL, NULL}},
};

void hisi_smmu_noc_dmd_upload(enum smmu_index index)
{
	int ret;
	unsigned long flags;
	unsigned int stat0;
	unsigned int stat1;

	if (index >= SMMU_INDEX_MAX) {
		pr_err("%s err input index %d\n", __func__, index);
		return;
	}

	spin_lock_irqsave(&g_smmu_dmd_lock, flags);

	if (g_smmu_dmd_info[index].is_init == false) {
		pr_err("%s %d dmd info is uninit\n", __func__, index);
		spin_unlock_irqrestore(&g_smmu_dmd_lock, flags);
		return;
	}
	/* reg_base has 2 members */
	stat0 = readl(g_smmu_dmd_info[index].reg_base[0]);
	stat1 = readl(g_smmu_dmd_info[index].reg_base[1]);

	spin_unlock_irqrestore(&g_smmu_dmd_lock, flags);

	ret = bbox_diaginfo_record(g_smmu_dmd_info[index].err_id, NULL,
		"%s [0x%x][0x%x]",
		g_smmu_dmd_info[index].err_name, stat0, stat1);
	if (ret)
		pr_err("%s diaginfo record err\n", __func__);

}

static int __init hisi_smmu_dmd_init(void)
{
	unsigned long flags;
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(g_smmu_dmd_info); i++) {
		 /* reg_base has 2 members, first init reg_base[0] */
		g_smmu_dmd_info[i].reg_base[0] = ioremap(g_smmu_dmd_info[i].reg_info[0], sizeof(u32));/*lint !e446*/
		if (!g_smmu_dmd_info[i].reg_base[0]) {
			pr_err("%s %d reg[0] ioremap failed\n", __func__, i);
			continue;
		}
		/* init reg_base[1] */
		g_smmu_dmd_info[i].reg_base[1] = ioremap(g_smmu_dmd_info[i].reg_info[1], sizeof(u32));/*lint !e446*/
		if (!g_smmu_dmd_info[i].reg_base[1]) {
			pr_err("%s %d reg[1] ioremap failed\n", __func__, i);
			iounmap(g_smmu_dmd_info[i].reg_base[0]);
			continue;
		}

		spin_lock_irqsave(&g_smmu_dmd_lock, flags);
		g_smmu_dmd_info[i].is_init = true;
		spin_unlock_irqrestore(&g_smmu_dmd_lock, flags);
	}

	return 0;
}

static void __exit hisi_smmu_dmd_exit(void)
{
	unsigned long flags;
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(g_smmu_dmd_info); i++) {
		if (g_smmu_dmd_info[i].is_init == true) {
			spin_lock_irqsave(&g_smmu_dmd_lock, flags);
			g_smmu_dmd_info[i].is_init = false;
			spin_unlock_irqrestore(&g_smmu_dmd_lock, flags);

			/* reg_base has 2 members, they need be iounmap when exit */
			iounmap(g_smmu_dmd_info[i].reg_base[0]);
			iounmap(g_smmu_dmd_info[i].reg_base[1]);
		}
	}
}

module_init(hisi_smmu_dmd_init);
module_exit(hisi_smmu_dmd_exit);
#endif

MODULE_DESCRIPTION(
	"Hisilicon SVM Base on ARM architected SMMUv3 implementations");
MODULE_LICENSE("GPL v2");
