/*
 * hisi_pmic_spmi.c
 *
 * Device driver for regulators in HISI PMIC IC
 *
 * Copyright (c) 2011-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/delay.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/hisi-spmi.h>
#include <linux/hisi/hisi_log.h>
#include <linux/mfd/hisi_pmic.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/of_hisi_spmi.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/bitops.h>
#include "hisi_pmic_sim_hpd.h"
#include <securec.h>

#ifndef NO_IRQ
#define NO_IRQ 0
#endif
#ifndef FALSE
#define FALSE 0
#endif
/* 8-bit register offset in PMIC */
#define HISI_LOG_TAG HISI_PMIC_TAG
#define HISI_MASK_STATE 0xff
#define HISI_MASK_FIELD 0xff
#define HISI_BITS 8
#define PMIC_FPGA_FLAG 1

#define HISI_IRQ_KEY_NUM 0
#define HISI_IRQ_KEY_VALUE 0xc0
#define HISI_IRQ_KEY_DOWN 7
#define HISI_IRQ_KEY_UP 6

#define VBUS_IRQ_MASK 0x3
#define VBUS_IRQ_UNMASK 0
#define VBUS_IRQ_CLEAR 0x3
#define VBUS_IRQ_UNCLEAR 0
#define VBUS_IRQ_INFO_MEMBERS_NUMS 3
#define PMIC_IRQ_MASK 0
#define PMIC_IRQ_UNMASK 1
#define ONE_IRQ_GROUP_NUM 8
static struct hisi_pmic *g_pmic;
static struct hisi_pmic *g_mmw_pmic;
static struct hisi_pmic *g_sub_pmic;

static struct bit_info g_pmic_vbus;
static struct vbus_irq_info g_pmic_vbus_mask;
static struct vbus_irq_info g_pmic_vbus_clear;

static u32 _hisi_pmic_read(struct hisi_pmic *pmic, int reg)
{
	u32 ret;
	u8 read_value = 0;
	struct spmi_device *pdev = NULL;

	if (pmic == NULL) {
		pr_err("g_pmic is NULL\n");
		return 0;
	}

	pdev = to_spmi_device(pmic->dev);
	if (pdev == NULL) {
		pr_err("%s:pdev get failed!\n", __func__);
		return 0;
	}

	ret = spmi_ext_register_readl(
		pdev->ctrl, pdev->sid, reg, (unsigned char *)&read_value, 1);
	if (ret) {
		pr_err("%s:spmi_ext_register_readl failed!\n", __func__);
		return 0;
	}
	return (u32)read_value;
}

static void _hisi_pmic_write(struct hisi_pmic *pmic, int reg, u32 val)
{
	u32 ret;
	u8 value;
	struct spmi_device *pdev = NULL;

	if (pmic == NULL) {
		pr_err("g_pmic is NULL\n");
		return;
	}
	pdev = to_spmi_device(pmic->dev);
	if (pdev == NULL) {
		pr_err("%s:sub pdev get failed!\n", __func__);
		return;
	}

	value = val & 0xff;
	ret = spmi_ext_register_writel(
		pdev->ctrl, pdev->sid, reg, &value, 1);
	if (ret) {
		pr_err("%s:spmi_ext_register_writel failed!\n", __func__);
		return;
	}
}

u32 hisi_sub_pmic_read(struct hisi_pmic *pmic, int reg)
{
	return _hisi_pmic_read(g_sub_pmic, reg);
}

void hisi_sub_pmic_write(struct hisi_pmic *pmic, int reg, u32 val)
{
	_hisi_pmic_write(g_sub_pmic, reg, val);
}

u32 hisi_pmic_read(struct hisi_pmic *pmic, int reg)
{
	return _hisi_pmic_read(g_pmic, reg);
}

void hisi_pmic_write(struct hisi_pmic *pmic, int reg, u32 val)
{
	_hisi_pmic_write(g_pmic, reg, val);
}

#if defined(CONFIG_HISI_PMIC_SUB_PMU_SPMI)
unsigned int hisi_mmw_pmic_reg_read(int addr)
{
	return _hisi_pmic_read(g_mmw_pmic, addr);
}

void hisi_mmw_pmic_reg_write(int addr, int val)
{
	_hisi_pmic_write(g_mmw_pmic, addr, val);
}

unsigned int hisi_sub_pmic_reg_read(int addr)
{
	return _hisi_pmic_read(g_sub_pmic, addr);
}
EXPORT_SYMBOL(hisi_sub_pmic_reg_read);

void hisi_sub_pmic_reg_write(int addr, int val)
{
	_hisi_pmic_write(g_sub_pmic, addr, val);
}
EXPORT_SYMBOL(hisi_sub_pmic_reg_write);

#endif

unsigned int hisi_pmic_reg_read(int addr)
{
	return _hisi_pmic_read(g_pmic, addr);
}
EXPORT_SYMBOL(hisi_pmic_reg_read);

void hisi_pmic_reg_write(int addr, int val)
{
	_hisi_pmic_write(g_pmic, addr, val);
}
EXPORT_SYMBOL(hisi_pmic_reg_write);

/* for read dieid of sub pmu before pho */
u32 hisi_pmic_read_sub_pmu(u8 sid, int reg)
{
	u8 read_value = 0;

#ifdef CONFIG_HISI_PMIC_DEBUG
	u32 ret;
	struct spmi_device *pdev = NULL;

	if (g_pmic == NULL) {
		pr_err(" g_pmic is NULL\n");
		return 0;
	}

	pdev = to_spmi_device(g_pmic->dev);
	if (pdev == NULL) {
		pr_err("%s:pdev get failed!\n", __func__);
		return 0;
	}

	ret = spmi_ext_register_readl(
		pdev->ctrl, sid, reg, (unsigned char *)&read_value, 1);
	if (ret) {
		pr_err("%s:spmi_ext_register_readl failed!\n",
			__func__);
		return 0;
	}
#endif
	return (u32)read_value;
}

void hisi_pmic_write_sub_pmu(u8 sid, int reg, u32 val)
{
#ifdef CONFIG_HISI_PMIC_DEBUG
	u32 ret;
	struct spmi_device *pdev = NULL;

	if (g_pmic == NULL) {
		pr_err(" g_pmic is NULL\n");
		return;
	}

	pdev = to_spmi_device(g_pmic->dev);
	if (pdev == NULL) {
		pr_err("%s:pdev get failed!\n", __func__);
		return;
	}

	ret = spmi_ext_register_writel(
		pdev->ctrl, sid, reg, (unsigned char *)&val, 1);
	if (ret) {
		pr_err("%s:spmi_ext_register_writel failed!\n",
			__func__);
		return;
	}
#endif
}

void hisi_pmic_rmw(struct hisi_pmic *pmic, int reg, u32 mask, u32 bits)
{
	u32 data;
	unsigned long flags;

	if (g_pmic == NULL) {
		pr_err(" g_pmic  is NULL\n");
		return;
	}

	spin_lock_irqsave(&g_pmic->lock, flags);
	data = hisi_pmic_read(pmic, reg) & ~mask;
	data |= mask & bits;
	hisi_pmic_write(pmic, reg, data);
	spin_unlock_irqrestore(&g_pmic->lock, flags);
}

void hisi_pmic_reg_write_lock(int addr, int val)
{
	unsigned long flags;

	if (g_pmic == NULL) {
		pr_err(" g_pmic  is NULL\n");
		return;
	}

	spin_lock_irqsave(&g_pmic->lock, flags);
	hisi_pmic_write(
		g_pmic, g_pmic->normal_lock.addr, g_pmic->normal_lock.val);
	hisi_pmic_write(
		g_pmic, g_pmic->debug_lock.addr, g_pmic->debug_lock.val);
	hisi_pmic_write(g_pmic, addr, val);
	hisi_pmic_write(g_pmic, g_pmic->normal_lock.addr, 0);
	hisi_pmic_write(g_pmic, g_pmic->debug_lock.addr, 0);
	spin_unlock_irqrestore(&g_pmic->lock, flags);
}
int hisi_pmic_array_read(int addr, char *buff, unsigned int len)
{
	unsigned int i;

	if ((len > 32) || (buff == NULL)) /* buf maxlen: 32 */
		return -EINVAL;

	/*
	 * Here is a bug in the pmu die.
	 * the coul driver will read 4 bytes,
	 * but the ssi bus only read 1 byte, and the pmu die
	 * will make sampling 1/10669us about vol cur,so the driver
	 * read the data is not the same sampling
	 */
	for (i = 0; i < len; i++)
		*(buff + i) = hisi_pmic_reg_read(addr + i);

	return 0;
}

int hisi_pmic_array_write(int addr, const char *buff, unsigned int len)
{
	unsigned int i;

	if ((len > 32) || (buff == NULL)) /* buf maxlen: 32 */
		return -EINVAL;

	for (i = 0; i < len; i++)
		hisi_pmic_reg_write(addr + i, *(buff + i));

	return 0;
}

/*
 * The PMIC register is only 8-bit.
 * Hisilicon SoC use hardware to map PMIC register into SoC mapping.
 * At here, we are accessing SoC register with 32-bit.
 */

int hisi_get_pmic_irq_byname(unsigned int pmic_irq_list)
{
	if (g_pmic == NULL) {
		pr_err("[%s]g_pmic is NULL\n", __func__);
		return -1;
	}

	if (pmic_irq_list > (unsigned int)g_pmic->irqnum) {
		pr_err("[%s]input pmic irq number is error\n", __func__);
		return -1;
	}
	pr_info("%s:g_pmic->irqs[%u] %u\n", __func__, pmic_irq_list,
		g_pmic->irqs[pmic_irq_list]);
	return (int)g_pmic->irqs[pmic_irq_list];
}

int hisi_pmic_get_vbus_status(void)
{
	if (g_pmic_vbus.addr == 0)
		return -1;

	if (hisi_pmic_reg_read(g_pmic_vbus.addr) &
		BIT((unsigned int)g_pmic_vbus.bit))
		return 1;

	return 0;
}

static void hisi_pmic_vbus_irq(
	struct vbus_irq_info *vbus_info, unsigned int mask)
{
	unsigned int value;
	unsigned int temp;

	if (vbus_info->addr == 0)
		return;

	value = hisi_pmic_reg_read(vbus_info->addr);

	temp = (value & (~(vbus_info->mask << vbus_info->shift))) |
	       (mask << vbus_info->shift);
	hisi_pmic_reg_write(vbus_info->addr, temp);
}

void hisi_pmic_vbus_irq_mask(int enable)
{
	if (enable)
		hisi_pmic_vbus_irq(&g_pmic_vbus_mask, VBUS_IRQ_MASK);
	else
		hisi_pmic_vbus_irq(&g_pmic_vbus_mask, VBUS_IRQ_UNMASK);
}

void hisi_pmic_clear_vbus_irq(int enable)
{
	if (enable)
		hisi_pmic_vbus_irq(&g_pmic_vbus_clear, VBUS_IRQ_CLEAR);
	else
		hisi_pmic_vbus_irq(&g_pmic_vbus_clear, VBUS_IRQ_UNCLEAR);
}

static int _hisi_pmic_get_dieid(
	struct hisi_pmic *pmic, char *dieid, unsigned int len)
{
	int ret;
	unsigned int i;
	unsigned char value;
	unsigned int length;
	char pmu_buf[HISI_PMIC_DIEID_BUF] = {0};
	char buf[HISI_PMIC_DIEID_TEM_SAVE_BUF] = {0};

	if (!dieid || !pmic || !pmic->dieid_name || !pmic->dieid_regs) {
		pr_err("%s dieid g_pmic  is NULL\n", __func__);
		return -ENOMEM;
	}

	ret = snprintf_s(pmu_buf, sizeof(pmu_buf), sizeof(pmu_buf) - 1,
		"%s%s%s", "\r\n", pmic->dieid_name, ":0x");
	if (ret < 0) {
		pr_err("%s read main pmu dieid head fail\n", __func__);
		return ret;
	}

	for (i = 0; i < pmic->dieid_reg_num; i++) {
		value = _hisi_pmic_read(pmic, pmic->dieid_regs[i]);
		ret = snprintf_s(
			buf, sizeof(buf), sizeof(buf) - 1, "%02x", value);
		if (ret < 0) {
			pr_err("%s read main pmu dieid fail\n", __func__);
			return ret;
		}
		ret = strncat_s(pmu_buf, HISI_PMIC_DIEID_BUF, buf, strlen(buf));
		if (ret) {
			pr_err("%s read main pmu dieid buf fail\n", __func__);
			return ret;
		}
	}

	ret = strncat_s(pmu_buf, HISI_PMIC_DIEID_BUF, "\r\n", strlen("\r\n"));
	if (ret) {
		pr_err("%s read main pmu dieid buf fail\n", __func__);
		return ret;
	}

	length = strlen(pmu_buf);
	if (len >= length) {
		ret = strncat_s(dieid, HISI_PMIC_DIEID_BUF, pmu_buf, length);
		if (ret) {
			pr_err("%s read pmu dieid length fail\n", __func__);
			return ret;
		}
	} else {
		pr_err("%s:dieid buf length is not enough!\n", __func__);
		return length;
	}

	return 0;
}


int hisi_subpmu_pmic_get_dieid(char *dieid, unsigned int len)
{
	int ret;

	ret = _hisi_pmic_get_dieid(g_sub_pmic, dieid, len);
	if (ret)
		pr_err("%s:fail!\n", __func__);
	return ret;
}

int hisi_pmic_get_dieid(char *dieid, unsigned int len)
{
	int ret;

	ret = _hisi_pmic_get_dieid(g_pmic, dieid, len);
	if (ret)
		pr_err("%s:fail!\n", __func__);
	return ret;
}

static void _hisi_irq_mask(
	struct irq_data *d, int maskflag)
{
	struct hisi_pmic *pmic = irq_data_get_irq_chip_data(d);
	u32 data, offset;
	unsigned long flags;

	if (pmic == NULL) {
		pr_err(" irq_mask pmic is NULL\n");
		return;
	}
	/* Convert interrupt data to interrupt offset */
	offset = (irqd_to_hwirq(d) / ONE_IRQ_GROUP_NUM);
	offset = pmic->irq_mask_addr_arry[offset];
	spin_lock_irqsave(&pmic->lock, flags);
	data = _hisi_pmic_read(pmic, offset);
	if (maskflag == PMIC_IRQ_MASK)
		data |= (1 << (irqd_to_hwirq(d) & 0x07)); /* offset mask 3 */
	else
		data &= ~(u32)(1 << (irqd_to_hwirq(d) & 0x07)); /* offset mask 3 */

	_hisi_pmic_write(pmic, offset, data);
	spin_unlock_irqrestore(&pmic->lock, flags);
}

static void hisi_irq_mask(struct irq_data *d)
{
	_hisi_irq_mask(d, PMIC_IRQ_MASK);
}

static void hisi_irq_unmask(struct irq_data *d)
{
	_hisi_irq_mask(d, PMIC_IRQ_UNMASK);
}

static int hisi_irq_map(
	struct irq_domain *d, unsigned int virq, irq_hw_number_t hw)
{
	struct hisi_pmic *pmic = d->host_data;
	int ret;

	if (!pmic)
		return -ENOMEM;

	irq_set_chip_and_handler_name(
		virq, &pmic->irq_chip, handle_simple_irq, "hisi");
	irq_set_chip_data(virq, pmic);
	ret = irq_set_irq_type(virq, IRQ_TYPE_NONE);
	if (ret < 0)
		pr_err("irq set type fail\n");

	return 0;
}

static const struct irq_domain_ops hisi_domain_ops = {
	.map = hisi_irq_map,
	.xlate = irq_domain_xlate_twocell,
};

static void get_pmic_device_tree_vbus_data(struct device_node *np)
{
	int ret;

	ret = of_property_read_u32_array(np,
		"hisilicon,hisi-pmic-vbus-mask-addr", (u32 *)&g_pmic_vbus_mask,
		VBUS_IRQ_INFO_MEMBERS_NUMS);
	if (ret)
		pr_err("no hisilicon,hisi-pmic-vbus-mask-addr property\n");

	ret = of_property_read_u32_array(np,
		"hisilicon,hisi-pmic-clear-vbus-irq-addr",
		(u32 *)&g_pmic_vbus_clear, VBUS_IRQ_INFO_MEMBERS_NUMS);
	if (ret)
		pr_err("no hisilicon,hisi-pmic-clear-vbus-irq-addr proprety\n");
}

static int get_pmic_device_tree_data(
	struct spmi_device *pdev, struct hisi_pmic *pmic)
{
	int ret;
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	bool is_process_irq = FALSE;

	is_process_irq = of_property_read_bool(np, "handle_irq");
	if (!is_process_irq) {
		pr_err(" pmu not need process irq\n");
		return 0;
	}

	/* get pmic irq num */
	ret = of_property_read_u32(
		np, "hisilicon,hisi-pmic-irq-num", &(pmic->irqnum));
	if (ret) {
		pr_err("no hisilicon,hisi-pmic-irq-num property set\n");
		return -ENODEV;
	}

	/* get pmic irq array number */
	ret = of_property_read_u32(
		np, "hisilicon,hisi-pmic-irq-array", &(pmic->irqarray));
	if (ret) {
		pr_err("no hisilicon,hisi-pmic-irq-array property set\n");
		return -ENODEV;
	}

	pmic->irq_mask_addr_arry = (int *)devm_kzalloc(
		dev, sizeof(int) * pmic->irqarray, GFP_KERNEL);
	if (!pmic->irq_mask_addr_arry)
		return -ENOMEM;

	/* SOC_PMIC_IRQ_MASK_ADDR */
	ret = of_property_read_u32_array(np,
		"hisilicon,hisi-pmic-irq-mask-addr",
		(int *)pmic->irq_mask_addr_arry, pmic->irqarray); /* width:2 */
	if (ret) {
		pr_err("no hisilicon,hisi-pmic-irq-mask-addr property set\n");
		return -ENODEV;
	}

	/* SOC_PMIC_IRQ_ADDR */
	pmic->irq_addr_arry = (int *)devm_kzalloc(
		dev, sizeof(int) * pmic->irqarray, GFP_KERNEL);
	if (!pmic->irq_addr_arry)
		return -ENOMEM;

	ret = of_property_read_u32_array(np, "hisilicon,hisi-pmic-irq-addr",
		(int *)pmic->irq_addr_arry, pmic->irqarray);
	if (ret) {
		pr_err("no hisilicon,hisi-pmic-irq-addr property set\n");
		return -ENODEV;
	}

	if (of_device_is_compatible(np, HISI_PMIC_COMP)) {
		ret = of_property_read_u32_array(np, "hisilicon,hisi-pmic-vbus",
			(u32 *)&g_pmic_vbus, 2); /* width:2 */
		if (ret) {
			pr_err("no hisilicon,hisi-pmic-vbus property\n");
			return -ENODEV;
		}
		get_pmic_device_tree_vbus_data(np);
		ret = of_property_read_u32_array(np, "hisilicon,hisi-pmic-lock",
			(int *)&pmic->normal_lock, 2); /* width:2 */
		if (ret) {
			pr_err("no hisilicon,hisi-pmic-lock property set\n");
			return -ENODEV;
		}

		/* pmic debug lock */
		ret = of_property_read_u32_array(np, "hisilicon,hisi-pmic-debug-lock",
			(int *)&pmic->debug_lock, 2); /* width:2 */
		if (ret) {
			pr_err("no hisilicon,hisi-pmic-debug-lock property set\n");
			return -ENODEV;
		}
	}
	return ret;
}

static int get_pmic_dieid_tree_data(
	struct spmi_device *pdev, struct hisi_pmic *pmic)
{
	int ret;
	struct device_node *root = NULL;
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;

	root = of_find_compatible_node(np, NULL, "hisilicon-hisi-pmic-dieid");
	if (root == NULL) {
		pr_err("[%s]no hisilicon-hisi-pmic-dieid root node\n",
			__func__);
		return -ENODEV;
	}

	pmic->dieid_name = (char *)devm_kzalloc(
		dev, sizeof(*pmic->dieid_name), GFP_KERNEL);
	if (!pmic->dieid_name)
		return -ENOMEM;

	ret = of_property_read_string(root, "hisilicon,hisi-pmic-dieid-name",
		(const char **)&(pmic->dieid_name));
	if (ret) {
		pr_err("no hisilicon,hisi-pmic-dieid-name property set\n");
		devm_kfree(dev, pmic->dieid_name);
		return -ENODEV;
	}

	ret = of_property_read_u32(root, "hisilicon,hisi-pmic-dieid-reg-num",
		(u32 *)&(pmic->dieid_reg_num));
	if (ret || !pmic->dieid_reg_num) {
		pr_err("no hisilicon,hisi-pmic-dieid-reg-num property set\n");
		devm_kfree(dev, pmic->dieid_name);
		return -ENODEV;
	}

	pmic->dieid_regs = (u32 *)devm_kzalloc(
		dev, sizeof(u32) * pmic->dieid_reg_num, GFP_KERNEL);
	if (!pmic->dieid_regs) {
		devm_kfree(dev, pmic->dieid_name);
		return -ENOMEM;
	}
	ret = of_property_read_u32_array(root, "hisilicon,hisi-pmic-dieid-regs",
		pmic->dieid_regs, pmic->dieid_reg_num);
	if (ret) {
		pr_err("[%s]get hisi-pmic-dieid-regs attribute failed\n",
			__func__);
		devm_kfree(dev, pmic->dieid_name);
		devm_kfree(dev, pmic->dieid_regs);
		return -ENODEV;
	}
	return ret;
}

static void hisi_pmic_irq_prc(struct hisi_pmic *pmic)
{
	int i;
	unsigned int pending;

	for (i = 0; i < pmic->irqarray; i++)
		_hisi_pmic_write(pmic, pmic->irq_mask_addr_arry[i],
			HISI_MASK_STATE);

	for (i = 0; i < pmic->irqarray; i++) {
		pending = _hisi_pmic_read(pmic, pmic->irq_addr_arry[i]);
		pr_debug("PMU IRQ address value:irq[0x%x] = 0x%x\n",
			pmic->irq_addr_arry[i], pending);
		_hisi_pmic_write(
			pmic, pmic->irq_addr_arry[i], HISI_MASK_STATE);
	}
}

static unsigned int hisi_power_key_irq_handler(struct hisi_pmic *pmic,
	int num, unsigned int pending)
{
	if (pmic->powerkey_irq_down_up) {
		if ((num == HISI_IRQ_KEY_NUM) &&
			((pending & HISI_IRQ_KEY_VALUE) ==
				HISI_IRQ_KEY_VALUE)) {
			generic_handle_irq(pmic->irqs[HISI_IRQ_KEY_DOWN]);
			generic_handle_irq(pmic->irqs[HISI_IRQ_KEY_UP]);
			pending &= (~HISI_IRQ_KEY_VALUE);
		}
	}
	return pending;
}

static irqreturn_t hisi_irq_handler(int irq, void *data)
{
	struct hisi_pmic *pmic = (struct hisi_pmic *)data;
	unsigned int pending;
	unsigned long pending_s;
	int i, offset;

	hisi_pmic_sim_hpd_proc();

	for (i = 0; i < pmic->irqarray; i++) {
		pending = _hisi_pmic_read(pmic, pmic->irq_addr_arry[i]);
		pending &= HISI_MASK_FIELD;
		if (pending != 0)
			pr_info("pending[%d]=0x%x\n\r", i, pending);

		_hisi_pmic_write(pmic, pmic->irq_addr_arry[i], pending);
		/* solve powerkey order */
		pending_s = (unsigned long)hisi_power_key_irq_handler(
				pmic, i, pending);
		if (pending_s) {
			for_each_set_bit(offset, &pending_s, HISI_BITS)
			generic_handle_irq(
				pmic->irqs[offset + i * HISI_BITS]);
		}
	}
	return IRQ_HANDLED;
}

static int hisi_pmic_irq_create_mapping(struct hisi_pmic *pmic)
{
	int i;
	unsigned int virq;

	for (i = 0; i < pmic->irqnum; i++) {
		virq = irq_create_mapping(pmic->domain, i);
		if (virq == NO_IRQ) {
			pr_debug("Failed mapping hwirq\n");
			return -ENOSPC;
		}
		pmic->irqs[i] = virq;
		pr_info("[%s]. pmic->irqs[%d] = %u\n", __func__, i,
			pmic->irqs[i]);
	}
	return 0;
}

static int hisi_pmic_irq_register(
	struct spmi_device *pdev, struct hisi_pmic *pmic, const char *name)
{
	int ret;
	enum of_gpio_flags flags;
	struct device *dev = NULL;
	struct device_node *np = NULL;

	if (!pdev || !pmic || !name)
		return -EINVAL;

	dev = &pdev->dev;
	np = dev->of_node;
	pmic->gpio = of_get_gpio_flags(np, 0, &flags);
	if (pmic->gpio < 0)
		return pmic->gpio;

	if (!gpio_is_valid(pmic->gpio))
		return -EINVAL;

	ret = gpio_request_one(pmic->gpio, GPIOF_IN, name);
	if (ret < 0) {
		dev_err(dev, "failed to request gpio %d\n", pmic->gpio);
		return ret;
	}

	pmic->irq = gpio_to_irq(pmic->gpio);

	/* mask && clear IRQ status */
	hisi_pmic_irq_prc(pmic);

	pmic->irqs = (unsigned int *)devm_kmalloc(
		dev, pmic->irqnum * sizeof(int), GFP_KERNEL);
	if (!pmic->irqs)
		return -ENODEV;

	/* Dynamic obtain struct irq_chip */
	pmic->irq_chip.name = pmic->chip_irq_name;
	pmic->irq_chip.irq_mask = hisi_irq_mask;
	pmic->irq_chip.irq_unmask = hisi_irq_unmask;
	pmic->irq_chip.irq_disable = hisi_irq_mask;
	pmic->irq_chip.irq_enable = hisi_irq_unmask;

	pmic->domain = irq_domain_add_simple(
		np, pmic->irqnum, 0, &hisi_domain_ops, pmic);

	if (!pmic->domain) {
		dev_err(dev, "failed irq domain add simple!\n");
		return -ENODEV;
	}

	ret = hisi_pmic_irq_create_mapping(pmic);
	if (ret < 0)
		return ret;

	ret = request_threaded_irq(pmic->irq, hisi_irq_handler, NULL,
		IRQF_TRIGGER_LOW | IRQF_SHARED | IRQF_NO_SUSPEND, name, pmic);
	if (ret < 0) {
		dev_err(dev, "could not claim pmic %d\n", ret);
		return -ENODEV;
	}

	return 0;
}

static int get_pmic_device_info(
	struct spmi_device *pdev, struct hisi_pmic *pmic)
{

	int ret;
	struct device *dev = &pdev->dev;

	/* get pmic dieid dts info */
	ret = get_pmic_dieid_tree_data(pdev, pmic);
	if (ret)
		dev_err(&pdev->dev, "Error reading hisi pmic dieid dts\n");

	/* get pmic dts irq */
	ret = get_pmic_device_tree_data(pdev, pmic);
	if (ret) {
		dev_err(&pdev->dev, "Error reading hisi pmic device dts\n");
		return ret;
	}
	pmic->dev = dev;
	return 0;
}
static int hisi_pmic_get_chip_and_irq_name(struct hisi_pmic *pmic)
{
	int ret;
	struct spmi_device *pdev = NULL;

	if(!pmic || !pmic->dev)
		return -ENOMEM;

	pdev = to_spmi_device(pmic->dev);
	if(!pdev)
		return -ENOMEM;

	ret = snprintf_s(pmic->irq_name, HISI_PMIC_IRQ_NAME_SIZE,
		HISI_PMIC_IRQ_NAME_SIZE - 1, "%s%u%s",
		"pmic-sid", pdev->sid, "-irq");
	if(ret < 0)
		pr_err("snprintf error, ret = %d!\n", ret);

	ret = snprintf_s(pmic->chip_irq_name, HISI_PMIC_IRQ_NAME_SIZE,
		HISI_PMIC_IRQ_NAME_SIZE - 1, "%s%u%s",
		"pmu-sid", pdev->sid, "-chip-irq");
	if(ret < 0)
		pr_err("snprintf error, ret = %d!\n", ret);
	return 0;

}
static int hisi_pmic_probe(struct spmi_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct hisi_pmic *pmic = NULL;
	int ret;
	unsigned int fpga_flag = 0;
	bool is_process_irq = FALSE;

	pmic = devm_kzalloc(dev, sizeof(*pmic), GFP_KERNEL);
	if (!pmic)
		return -ENOMEM;

	ret = get_pmic_device_info(pdev, pmic);
	if (ret)
		return ret;

	spin_lock_init(&pmic->lock);

	/* get spmi sid */
	ret = of_property_read_u32(np, "slave_id", (u32 *)&(pdev->sid));
	if (ret)
		return -ENOMEM;

	is_process_irq = of_property_read_bool(np, "handle_irq");
	if (!is_process_irq) {
		dev_err(&pdev->dev, " not need process irq\n");
		goto success;
	}

	ret = of_property_read_u32_array(
		np, "hisilicon,pmic_fpga_flag", &fpga_flag, 1);
	if (ret)
		pr_err("no hisilicon,pmic_fpga_flag!\n");

	if (fpga_flag == PMIC_FPGA_FLAG)
		goto success;

	if (of_device_is_compatible(np, HISI_PMIC_COMP))
		pmic->powerkey_irq_down_up = 1;

	ret = hisi_pmic_get_chip_and_irq_name(pmic);
	if (ret) {
		pr_err("get chip irq name or irq name fail!\n");
		return ret;
	}

	ret = hisi_pmic_irq_register(pdev, pmic, pmic->irq_name);
	if (ret)
		goto request_theaded_irq;

success:
	if (of_device_is_compatible(np, HISI_MMW_PMIC_COMP))
		g_mmw_pmic = pmic;
	else if (of_device_is_compatible(np, HISI_SUB_PMIC_COMP))
		g_sub_pmic = pmic;
	else
		g_pmic = pmic;

	return 0;

request_theaded_irq:
	gpio_free(pmic->gpio);
	return ret;
}

static int hisi_pmic_remove(struct spmi_device *pdev)
{
	struct hisi_pmic *pmic = dev_get_drvdata(&pdev->dev);

	free_irq(pmic->irq, pmic);
	gpio_free(pmic->gpio);
	devm_kfree(&pdev->dev, pmic);

	return 0;
}

static int hisi_pmic_suspend(struct device *dev, pm_message_t state)
{
	struct hisi_pmic *pmic = dev_get_drvdata(dev);

	if (pmic == NULL) {
		pr_err("%s:pmic is NULL\n", __func__);
		return -ENOMEM;
	}

	pr_info("%s:+\n", __func__);
	pr_info("%s:-\n", __func__);

	return 0;
}

static int hisi_pmic_resume(struct device *dev)
{
	struct hisi_pmic *pmic = dev_get_drvdata(dev);

	if (pmic == NULL) {
		pr_err("%s:pmic is NULL\n", __func__);
		return -ENOMEM;
	}

	pr_info("%s:+\n", __func__);
	pr_info("%s:-\n", __func__);

	return 0;
}

static const struct of_device_id of_hisi_pmic_match_tbl[] = {
	{ .compatible = HISI_PMIC_COMP, },
	{ .compatible = HISI_SUB_PMIC_COMP, },
	{ .compatible = HISI_MMW_PMIC_COMP, },
	{ /* end */ }
};

static const struct spmi_device_id pmic_spmi_id[] = {
	{ "pmic_spmi", 0 },
	{}
};

MODULE_DEVICE_TABLE(spmi, pmic_spmi_id);

static struct spmi_driver hisi_pmic_driver = {
	.driver = {
			.name = "hisi_pmic",
			.owner = THIS_MODULE,
			.of_match_table = of_hisi_pmic_match_tbl,
			.suspend = hisi_pmic_suspend,
			.resume = hisi_pmic_resume,
		},
	.id_table = pmic_spmi_id,
	.probe = hisi_pmic_probe,
	.remove = hisi_pmic_remove,
};

static int __init hisi_pmic_init(void)
{
	return spmi_driver_register(&hisi_pmic_driver);
}

static void __exit hisi_pmic_exit(void)
{
	spmi_driver_unregister(&hisi_pmic_driver);
}

subsys_initcall_sync(hisi_pmic_init);
module_exit(hisi_pmic_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("PMIC driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
