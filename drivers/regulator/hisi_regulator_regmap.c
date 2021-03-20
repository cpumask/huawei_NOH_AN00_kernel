/*
 * hisi_regulator_regmap.c
 *
 * Device driver for regulators in HISI PMIC IC
 *
 * Copyright (c) 2020 Huawei Technologies Co., Ltd.
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

#include <linux/debugfs.h>
#include <hisi_regulator_debug.h>
#include <linux/regulator/of_regulator.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/regmap.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <securec.h>
#include <linux/version.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/hisi/hisi_log.h>

#define HISI_LOG_TAG HISI_PMIC_REGULATOR_TAG
#define BRAND_DEBUG(args...)	pr_err(args)

#define TIME_CHANGE_US_MS 1000

#define HISI_ECO_MODE_ENABLE		1
#define HISI_ECO_MODE_DISABLE		0
#define READ_SIZE 2

struct hisi_regulator_register_info {
	u32 ctrl_reg;
	u32 enable_mask;
	u32 eco_mode_mask;
	u32 vset_reg;
	u32 vset_mask;
};

struct hisi_regulator_opt_reg_info {
	int opt_reg_num;
	u16 *opt_regs;
	u8 *reg_val;
};

struct hisi_regulator {
	const char *name;
	struct hisi_regulator_register_info register_info;
	struct timeval last_off_time;
	u32 off_on_delay;
	u32 eco_uA;
	struct regulator_desc rdesc;
	int (*dt_parse)(struct hisi_regulator *, struct platform_device *);
};
static DEFINE_MUTEX(enable_mutex);

/*
 * helper function to ensure when it returns it is at least 'delay_us'
 * microseconds after 'since'.
 */
static void ensured_time_after(struct timeval since, u32 delay_us)
{
	struct timeval now;
	u64 elapsed_ns64, delay_ns64;
	u32 actual_us32;

	delay_ns64 = delay_us * NSEC_PER_USEC;
	do_gettimeofday(&now);
	elapsed_ns64 = timeval_to_ns(&now) - timeval_to_ns(&since);
	if (delay_ns64 > elapsed_ns64) {
		actual_us32 = ((u32)(delay_ns64 - elapsed_ns64) /
							NSEC_PER_USEC);
		if (actual_us32 >= TIME_CHANGE_US_MS) {
			mdelay((u32)(actual_us32 / TIME_CHANGE_US_MS));
			udelay(actual_us32 % TIME_CHANGE_US_MS);
		} else if (actual_us32 > 0) {
			udelay(actual_us32);
		}
	}
}

static int hisi_regulator_is_enabled(struct regulator_dev *dev)
{
	struct hisi_regulator *sreg = rdev_get_drvdata(dev);
	struct regmap *regmap = dev->regmap;
	u32 reg_val;
	int ret;

	if (!sreg || !regmap)
		return 0;

	ret = regmap_read(regmap, sreg->register_info.ctrl_reg, &reg_val);
	if (ret < 0) {
		pr_err("%s read ctrl_reg 0x%x failed\n", __func__,
			sreg->register_info.ctrl_reg);
		return ret;
	}

	BRAND_DEBUG("<[%s]: ctrl_reg=0x%x,enable_state=%d>\n", __func__,
			sreg->register_info.ctrl_reg,
			(reg_val & sreg->register_info.enable_mask));
	return ((reg_val & sreg->register_info.enable_mask) != 0);
}

static int hisi_regulator_enable(struct regulator_dev *dev)
{
	struct hisi_regulator *sreg = rdev_get_drvdata(dev);
	struct regmap *regmap = dev->regmap;
	int ret;

	if (!sreg || !regmap)
		return -EINVAL;

	/* keep a distance of off_on_delay from last time disabled */
	ensured_time_after(sreg->last_off_time, sreg->off_on_delay);

	BRAND_DEBUG("<[%s]: off_on_delay = %d us>\n", __func__, sreg->off_on_delay);

	/* cannot enable more than one regulator at one time */
	mutex_lock(&enable_mutex);

	/* set enable register */
	ret = regmap_write_bits(regmap, sreg->register_info.ctrl_reg,
				sreg->register_info.enable_mask,
				sreg->register_info.enable_mask);
	if (ret < 0) {
		pr_err("%s regmap_write_bits ctrl_reg 0x%x failed\n", __func__,
			sreg->register_info.ctrl_reg);
		return ret;
	}

	BRAND_DEBUG("<[%s]: ctrl_reg=0x%x,enable_mask=0x%x>\n",
			__func__, sreg->register_info.ctrl_reg,
			sreg->register_info.enable_mask);

	mutex_unlock(&enable_mutex);

	return 0;
}

static int hisi_regulator_disable(struct regulator_dev *dev)
{
	struct hisi_regulator *sreg = rdev_get_drvdata(dev);
	struct regmap *regmap = dev->regmap;
	int ret;

	if (!sreg || !regmap)
		return -EINVAL;
	/* set enable register to 0 */
	ret = regmap_write_bits(regmap, sreg->register_info.ctrl_reg,
				sreg->register_info.enable_mask, 0);
	if (ret < 0) {
		pr_err("%s regmap_write_bits ctrl_reg 0x%x failed\n", __func__,
			sreg->register_info.ctrl_reg);
		return ret;
	}

	do_gettimeofday(&sreg->last_off_time);

	return 0;
}

static int hisi_regulator_get_voltage(struct regulator_dev *dev)
{
	struct hisi_regulator *sreg = rdev_get_drvdata(dev);
	struct regmap *regmap = dev->regmap;
	u32 reg_val;
	u32 selector;
	int ret;
	int volt;

	if (!sreg || !regmap)
		return -EINVAL;

	/* get voltage selector */
	ret = regmap_read(regmap, sreg->register_info.vset_reg, &reg_val);
	if (ret < 0) {
		pr_err("%s read vset_reg 0x%x failed\n", __func__,
			sreg->register_info.vset_reg);
		return ret;
	}

	selector = (reg_val & sreg->register_info.vset_mask) >>
			((unsigned int)ffs(sreg->register_info.vset_mask) - 1);

	volt = sreg->rdesc.ops->list_voltage(dev, selector);

	BRAND_DEBUG("<[%s]: vset_reg 0x%x = 0x%x, volt %d>\n", __func__,
				sreg->register_info.vset_reg, reg_val, volt);

	return volt;
}

static int hisi_regulator_set_voltage(struct regulator_dev *dev,
				int min_uv, int max_uv, unsigned int *selector)
{
	struct hisi_regulator *sreg = rdev_get_drvdata(dev);
	struct regmap *regmap = dev->regmap;
	u32 vsel;
	int uv;
	int ret;

	if (!sreg || !regmap)
		return -EINVAL;

	for (vsel = 0; vsel < sreg->rdesc.n_voltages; vsel++) {
		uv = sreg->rdesc.volt_table[vsel];
		/* Break at the first in-range value */
		if (min_uv <= uv && uv <= max_uv)
			break;
	}

	/* unlikely to happen. sanity test done by regulator core */
	if (unlikely(vsel == sreg->rdesc.n_voltages))
		return -EINVAL;

	*selector = vsel;
	/* set voltage selector */
	ret = regmap_write_bits(regmap, sreg->register_info.vset_reg,
		sreg->register_info.vset_mask,
		vsel << ((unsigned int)ffs(sreg->register_info.vset_mask) - 1));
	if (ret < 0) {
		pr_err("%s regmap_write_bits vset reg 0x%x failed\n", __func__,
			sreg->register_info.vset_reg);
		return ret;
	}

	BRAND_DEBUG("<[%s]: vset_reg=0x%x, vset_mask=0x%x, value=0x%x>\n",
			__func__, sreg->register_info.vset_reg,
			sreg->register_info.vset_mask,
			vsel << (ffs(sreg->register_info.vset_mask) - 1));

	return 0;
}

static unsigned int hisi_regulator_get_mode(struct regulator_dev *dev)
{
	struct hisi_regulator *sreg = rdev_get_drvdata(dev);
	struct regmap *regmap = dev->regmap;
	u32 reg_val;
	int ret;

	if (!sreg || !regmap)
		return 0;

	ret = regmap_read(regmap, sreg->register_info.ctrl_reg, &reg_val);
	if (ret < 0) {
		pr_err("%s regmap read ctrl reg 0x%x failed\n", __func__,
			sreg->register_info.ctrl_reg);
		return 0;
	}

	BRAND_DEBUG("<[%s]: reg_val=%d, ctrl_reg=0x%x, eco_mode_mask=0x%x>\n",
		__func__, reg_val,
		sreg->register_info.ctrl_reg,
		sreg->register_info.eco_mode_mask);

	if (reg_val & sreg->register_info.eco_mode_mask)
		return REGULATOR_MODE_IDLE;
	else
		return REGULATOR_MODE_NORMAL;
}

static int hisi_regulator_set_mode(struct regulator_dev *dev,
						unsigned int mode)
{
	struct hisi_regulator *sreg = rdev_get_drvdata(dev);
	struct regmap *regmap = dev->regmap;
	u32 eco_mode;
	int ret;

	if (!sreg || !regmap)
		return -EINVAL;

	switch (mode) {
	case REGULATOR_MODE_NORMAL:
		eco_mode = HISI_ECO_MODE_DISABLE;
		break;
	case REGULATOR_MODE_IDLE:
		eco_mode = HISI_ECO_MODE_ENABLE;
		break;
	default:
		return -EINVAL;
	}

	/* set mode */
	ret = regmap_write_bits(regmap, sreg->register_info.ctrl_reg,
		sreg->register_info.eco_mode_mask,
		eco_mode << ((unsigned int)ffs(sreg->register_info.eco_mode_mask) - 1));
	if (ret < 0) {
		pr_err("%s regmap write_bits ctrl reg 0x%x failed\n", __func__,
			sreg->register_info.ctrl_reg);
		return ret;
	}

	BRAND_DEBUG("<[%s]: ctrl_reg=0x%x, eco_mode_mask=0x%x, value=0x%x>\n",
		__func__, sreg->register_info.ctrl_reg,
		sreg->register_info.eco_mode_mask,
		eco_mode << (ffs(sreg->register_info.eco_mode_mask) - 1));
	return 0;
}


static unsigned int hisi_regulator_get_optimum_mode(struct regulator_dev *dev,
			int input_uV, int output_uV, int load_uA)
{
	struct hisi_regulator *sreg = rdev_get_drvdata(dev);

	if (!sreg)
		return 0;

	if ((load_uA == 0) || ((unsigned int)load_uA > sreg->eco_uA))
		return REGULATOR_MODE_NORMAL;
	else
		return REGULATOR_MODE_IDLE;
}

static int hisi_dt_parse_common(struct hisi_regulator *sreg,
					struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct regulator_desc *rdesc = &sreg->rdesc;
	unsigned int register_info[3] = {0}; /* the length is determined by dts parameter */
	int ret;

	/* parse .register_info.ctrl_reg */
	ret = of_property_read_u32_array(np, "hisilicon,hisi-ctrl",
						register_info, READ_SIZE);
	if (ret) {
		dev_err(dev, "no hisilicon,hisi-ctrl property set\n");
		return ret;
	}
	sreg->register_info.ctrl_reg = register_info[0];
	sreg->register_info.enable_mask = register_info[1];
	sreg->register_info.eco_mode_mask = register_info[2];

	/* parse .register_info.vset_reg */
	ret = of_property_read_u32_array(np, "hisilicon,hisi-vset",
						register_info, READ_SIZE);
	if (ret) {
		dev_err(dev, "no hisilicon,hisi-vset property set\n");
		return ret;
	}
	sreg->register_info.vset_reg = register_info[0];
	sreg->register_info.vset_mask = register_info[1];

	/* parse .off-on-delay */
	ret = of_property_read_u32(np, "hisilicon,hisi-off-on-delay-us",
						&sreg->off_on_delay);
	if (ret) {
		dev_err(dev, "no hisilicon,hisi-off-on-delay-us property set\n");
		return ret;
	}

	/* parse .enable_time */
	ret = of_property_read_u32(np, "hisilicon,hisi-enable-time-us",
				   &rdesc->enable_time);
	if (ret) {
		dev_err(dev, "no hisilicon,hisi-enable-time-us property set\n");
		return ret;
	}

	/* parse .eco_uA */
	ret = of_property_read_u32(np, "hisilicon,hisi-eco-microamp",
				   &sreg->eco_uA);
	if (ret) {
		sreg->eco_uA = 0;
		ret = 0;
	}
	return ret;
}
static int fake_of_get_regulator_constraint(struct regulation_constraints *constraints,
						struct device_node *np)
{
	unsigned int temp_modes = 0;
	int ret;

	/* hisi regulator supports two modes */
	ret = of_property_read_u32_array(np, "hisilicon,valid-modes-mask",
					&(constraints->valid_modes_mask), 1);
	if (ret) {
		pr_err("no hisilicon,valid-modes-mask property set\n");
		ret = -ENODEV;
		return ret;
	}
	ret = of_property_read_u32_array(np, "hisilicon,valid-idle-mask",
						&temp_modes, 1);
	if (ret) {
		pr_err("no hisilicon,valid-idle-mask property set\n");
		ret = -ENODEV;
		return ret;
	}
	constraints->valid_ops_mask |= temp_modes;
	return ret;
}
static int hisi_dt_parse_ldo(struct hisi_regulator *sreg,
				struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct regulator_desc *rdesc = &sreg->rdesc;
	unsigned int *v_table = NULL;
	int ret;

	/* parse .n_voltages, and .volt_table */
	ret = of_property_read_u32(np, "hisilicon,hisi-n-voltages",
				   &rdesc->n_voltages);
	if (ret) {
		dev_err(dev, "no hisilicon,hisi-n-voltages property set\n");
		return ret;
	}
	/* alloc space for .volt_table */
	v_table = devm_kzalloc(
		dev, sizeof(unsigned int) * rdesc->n_voltages, GFP_KERNEL);
	if (!v_table) {
		ret = -ENOMEM;
		dev_err(dev, "no memory for .volt_table\n");
		return ret;
	}
	ret = of_property_read_u32_array(np, "hisilicon,hisi-vset-table",
						v_table, rdesc->n_voltages);
	if (ret) {
		dev_err(dev, "no hisilicon,hisi-vset-table property set\n");
		devm_kfree(dev, v_table);
		return ret;
	}
	rdesc->volt_table = v_table;

	/* parse hisi regulator's dt common part */
	ret = hisi_dt_parse_common(sreg, pdev);
	if (ret) {
		dev_err(dev, "failure in hisi_dt_parse_common\n");
		devm_kfree(dev, v_table);
		return ret;
	}
	return ret;
}

static struct regulator_ops hisi_ldo_rops = {
	.is_enabled = hisi_regulator_is_enabled,
	.enable = hisi_regulator_enable,
	.disable = hisi_regulator_disable,
	.list_voltage = regulator_list_voltage_table,
	.get_voltage = hisi_regulator_get_voltage,
	.set_voltage = hisi_regulator_set_voltage,
	.get_mode = hisi_regulator_get_mode,
	.set_mode = hisi_regulator_set_mode,
	.get_optimum_mode = hisi_regulator_get_optimum_mode,
};

static const struct hisi_regulator hisi_regulator_ldo = {
	.rdesc = {
	.ops = &hisi_ldo_rops,
		.type = REGULATOR_VOLTAGE,
		.owner = THIS_MODULE,
	},
	.dt_parse = hisi_dt_parse_ldo,
};

static const struct of_device_id of_hisi_regulator_match_tbl[] = {
	{
		.compatible = "hisilicon-hisi-ldo",
		.data = &hisi_regulator_ldo,
	},
	{ /* end */ }
};

static struct hisi_regulator *regulator_rdesc_prepare(
										struct platform_device *pdev,
										struct regulator_init_data *initdata)
{
	struct device *dev = &pdev->dev;
	const struct of_device_id *match = NULL;
	struct hisi_regulator *sreg = NULL;
	const struct hisi_regulator *template = NULL;
	struct regulator_desc *rdesc = NULL;
	int ret;

	/* to check which type of regulator this is */
	match = of_match_device(of_hisi_regulator_match_tbl, &pdev->dev);
	if (!match) {
		dev_err(dev, "get hisi regulator fail\n");
		return NULL;
	}
	template = match->data;

	sreg = kmemdup(template, sizeof(*sreg), GFP_KERNEL);
	if (!sreg) {
		pr_err("template kememdup is fail\n");
		return NULL;
	}

	sreg->name = initdata->constraints.name;
	rdesc = &sreg->rdesc;
	rdesc->name = sreg->name;
	rdesc->min_uV = initdata->constraints.min_uV;

	/* to parse device tree data for regulator specific */
	ret = sreg->dt_parse(sreg, pdev);
	if (ret) {
		dev_err(dev, "device tree parameter parse error!\n");
		kfree(sreg);
		return NULL;
	}

	return sreg;
}

static struct regulator_init_data *regulator_initdata_prepare(
								struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct regulation_constraints *constraint = NULL;
	struct regulator_init_data *initdata = NULL;
	const char *supplyname = NULL;
	int ret;

	initdata = of_get_regulator_init_data(dev, np, NULL);
	if (!initdata) {
		dev_err(dev, "%s get regulator init data error\n", __func__);
		return NULL;
	}

	supplyname = of_get_property(np, "hisilicon,supply_name", NULL);
	if (supplyname)
		initdata->supply_regulator = supplyname;

	/* hisi regulator supports two modes */
	constraint = &initdata->constraints;
	ret = fake_of_get_regulator_constraint(constraint, np);
	if (ret) {
		dev_err(dev, "%s get regulator constraint fail %d\n", __func__, ret);
		return NULL;
	}

	return initdata;
}

static int hisi_regulator_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct regulator_dev *rdev = NULL;
	struct regulator_init_data *initdata = NULL;
	struct hisi_regulator *sreg = NULL;
	struct regulator_config config;
	int ret;

	ret = memset_s(&config, sizeof(struct regulator_config),
				0, sizeof(struct regulator_config));
	if (ret != EOK) {
		dev_err(dev, "%s memset_s config fail\n", __func__);
		return ret;
	}

	initdata = regulator_initdata_prepare(pdev);
	if (!initdata) {
		dev_err(dev, "%s initdata prepare fail %d\n", __func__, ret);
		return -1;
	}

	sreg = regulator_rdesc_prepare(pdev, initdata);
	if (!sreg) {
		dev_err(dev, "%s initdata prepare fail %d\n", __func__, ret);
		return -1;
	}

	config.dev = &pdev->dev;
	config.init_data = initdata;
	config.driver_data = sreg;
	config.of_node = pdev->dev.of_node;

	/* register regulator */
	rdev = regulator_register(&sreg->rdesc, &config);
	if (IS_ERR(rdev)) {
		dev_err(dev, "failed to register %s\n",	sreg->rdesc.name);
		ret = PTR_ERR(rdev);
		goto hisi_probe_end;
	}

	dev_set_drvdata(dev, rdev);
#if defined CONFIG_HISI_PMIC_DEBUG
	ret = hisi_regulator_debugfs_create(dev);
	if (ret)
		regulator_unregister(rdev);
#endif

hisi_probe_end:
	if (ret)
		kfree(sreg);

	return ret;
}

static int hisi_regulator_remove(struct platform_device *pdev)
{
	struct regulator_dev *rdev = NULL;
	struct hisi_regulator *sreg = NULL;

	rdev = dev_get_drvdata(&pdev->dev);
	if (!rdev) {
		pr_err("%s:rdev is NULL\n", __func__);
		return -ENOMEM;
	}

	sreg = rdev_get_drvdata(rdev);
	if (!sreg) {
		pr_err("%s:sreg is NULL\n", __func__);
		return -ENOMEM;
	}

	regulator_unregister(rdev);

	if (sreg->rdesc.volt_table)
		devm_kfree(&pdev->dev, (unsigned int *)sreg->rdesc.volt_table);

	kfree(sreg);
	return 0;
}

static const struct platform_device_id regulator_id_tbl[] = {
	{ "hisilicon-hisi-ldo", 0 },
	{},
};

static struct platform_driver hisi_pmic_driver = {
	.driver = {
		.name	= "hisi_regulator",
		.owner  = THIS_MODULE,
		.of_match_table = of_hisi_regulator_match_tbl,
	},
	.id_table = regulator_id_tbl,
	.probe	= hisi_regulator_probe,
	.remove	= hisi_regulator_remove,
};

static int __init hisi_regulator_init(void)
{
	return platform_driver_register(&hisi_pmic_driver);
}

static void __exit hisi_regulator_exit(void)
{
	platform_driver_unregister(&hisi_pmic_driver);
}

fs_initcall(hisi_regulator_init);
module_exit(hisi_regulator_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Hisi regulator regmap driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
