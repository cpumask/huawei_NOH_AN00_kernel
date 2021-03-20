/*
 * hisilicon ISP driver, qos.c
 *
 * Copyright (c) 2018 Hisilicon Technologies CO., Ltd.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/iommu.h>
#include <linux/platform_data/remoteproc_hisi.h>
#include "hisp_internel.h"

#define QOS_MAX_NUM 64

struct {
	unsigned int num;
	unsigned int *offset;
	unsigned int *value;
} qos_dev;

int hisp_qos_dtget(struct device_node *np)
{
	int ret = -1;
	unsigned int qos_num = 0;
	unsigned int *qos_offset = NULL;
	unsigned int *qos_value = NULL;

	pr_info("[%s] +\n", __func__);

	if (np == NULL)
		return -ENOMEM;

	ret = of_property_read_u32(np, "qos-num", &qos_num);
	if (ret < 0) {
		pr_err("[%s] Failed: qos-num.0x%x get from dtsi.%d\n",
				__func__, qos_num, ret);
		return -ENODEV;
	}

	pr_info("[%s] qos-num.%d\n", __func__, qos_num);

	if ((qos_num == 0) || (qos_num > QOS_MAX_NUM)) {
		pr_err("[%s] Failed: QOS_MAX_NUM.%d\n", __func__, QOS_MAX_NUM);
		return -ENODEV;
	}

	qos_offset = kmalloc_array(qos_num, sizeof(unsigned int), GFP_KERNEL);
	if (qos_offset == NULL)
		return -ENOMEM;

	qos_value = kmalloc_array(qos_num, sizeof(unsigned int), GFP_KERNEL);
	if (qos_value == NULL)
		goto free_qos;

	ret = of_property_read_u32_array(np,
			"qos-offset", qos_offset, qos_num);
	if (ret < 0) {
		pr_err("[%s] Failed: qos-offset get from dtsi.%d\n",
				__func__, ret);
		goto free_qos;
	}

	ret = of_property_read_u32_array(np,
			"qos-value", qos_value, qos_num);
	if (ret < 0) {
		pr_err("[%s] Failed: qos-value get from dtsi.%d\n",
				__func__, ret);
		goto free_qos;
	}

	qos_dev.num = qos_num;
	qos_dev.offset = qos_offset;
	qos_dev.value = qos_value;

	return 0;

free_qos:
	if (qos_offset != NULL)
		kfree(qos_offset);
	if (qos_value != NULL)
		kfree(qos_value);
	return -ENOMEM;
}

int ispcpu_qos_cfg(void)
{
	void __iomem *vivobus_va;
	unsigned int i = 0;

	pr_info("[%s] +\n", __func__);

	vivobus_va = get_regaddr_by_pa(VIVOBUS);
	if (vivobus_va == NULL) {
		pr_err("[%s] vivobus_base remap fail\n", __func__);
		return -ENOMEM;
	}
	pr_info("[%s] vivobus_base.%pK, qos_num.%d",
			__func__, vivobus_va, qos_dev.num);

	if ((qos_dev.num == 0) || (qos_dev.num > QOS_MAX_NUM) ||
	    (qos_dev.offset == NULL) || (qos_dev.value == NULL)) {
		pr_err("[%s] Failed: QOS_MAX_NUM.%d, offset.%pK, value.%pK\n",
			__func__, QOS_MAX_NUM, qos_dev.offset, qos_dev.value);
		return -ENODEV;
	}

	for (i = 0; i < qos_dev.num; i++) {
		__raw_writel(qos_dev.value[i], vivobus_va + qos_dev.offset[i]);
	}

	return 0;
}

void hisp_qos_free(void)
{
	if (qos_dev.offset != NULL) {
		kfree(qos_dev.offset);
		qos_dev.offset = NULL;
	}
	if (qos_dev.value != NULL) {
		kfree(qos_dev.value);
		qos_dev.value = NULL;
	}
	qos_dev.num = 0;
}

