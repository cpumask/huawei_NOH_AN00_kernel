#include "dubai_battery_stats.h"

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/power_supply.h>

#include "dubai_utils.h"

enum {
	DUBAI_BATTERY_PROP_GAS_GAUGE = 0,
	DUBAI_BATTERY_PROP_VOLTAGE_NOW = 1,
	DUBAI_BATTERY_PROP_CURRENT_NOW = 2,
	DUBAI_BATTERY_PROP_CYCLE_COUNT = 3,
	DUBAI_BATTERY_PROP_CHARGE_FULL = 4,
	DUBAI_BATTERY_PROP_LEVEL = 5,
	DUBAI_BATTERY_PROP_BRAND = 6,
	DUBAI_BATTERY_PROP_CHARGE_STATUS = 7,
};

#define UV_PER_MV				1000
#ifdef CONFIG_HISI_COUL
#define BATTERY_UNIT			1
#define BATTERY_SUPPLY_NAME 	("Battery")
extern int hisi_battery_rm(void);
#else
#define BATTERY_UNIT 			1000
#define BATTERY_SUPPLY_NAME		("battery")
#endif

static int dubai_get_property(struct power_supply *psy, enum power_supply_property psp, union power_supply_propval *val)
{
	int ret;

	ret = power_supply_get_property(psy, psp, val);
	if (ret != 0) {
		dubai_err("Failed to get power supply property %d %d", psp, ret);
		return -EINVAL;
	}

	return ret;
}

static int dubai_get_charge_full(struct power_supply *psy)
{
	int ret;
	union power_supply_propval propval;

	ret = dubai_get_property(psy, POWER_SUPPLY_PROP_CHARGE_FULL, &propval);
	if (ret == 0)
		return propval.intval / BATTERY_UNIT;

	return ret;
}

static int dubai_get_level(struct power_supply *psy)
{
	int ret;
	union power_supply_propval propval;

	ret = dubai_get_property(psy, POWER_SUPPLY_PROP_CAPACITY, &propval);
	if (ret == 0)
		return propval.intval;

	return ret;
}

#ifdef CONFIG_HUAWEI_WATCH_CHARGER
static int dubai_get_capacity_rm(struct power_supply *psy)
{
	int ret;
	union power_supply_propval propval;

	ret = dubai_get_property(psy, POWER_SUPPLY_PROP_CAPACITY_RM, &propval);
	if (ret == 0)
		return propval.intval;

	return ret;
}
#endif

static int dubai_get_gas_gauge(struct power_supply *psy)
{
#ifdef CONFIG_HUAWEI_WATCH_CHARGER
	return dubai_get_capacity_rm(psy);
#elif CONFIG_HISI_COUL
	return hisi_battery_rm();
#else
	int full, level;

	full = dubai_get_charge_full(psy);
	level = dubai_get_level(psy);

	return (full * level / 100); // 100 is used for normalizing
#endif
}

static int dubai_get_voltage(struct power_supply *psy)
{
	int ret;
	union power_supply_propval propval;

	ret = dubai_get_property(psy, POWER_SUPPLY_PROP_VOLTAGE_NOW, &propval);
	if (ret == 0)
		return propval.intval / UV_PER_MV;

	return ret;
}

static int dubai_get_current(struct power_supply *psy)
{
	int ret = 0;
	union power_supply_propval propval;

	ret = dubai_get_property(psy, POWER_SUPPLY_PROP_CURRENT_NOW, &propval);
	if (ret == 0)
		return propval.intval;

	return ret;
}

static int dubai_get_cycle_count(struct power_supply *psy)
{
	int ret;
	union power_supply_propval propval;

	ret = dubai_get_property(psy, POWER_SUPPLY_PROP_CYCLE_COUNT, &propval);
	if (ret == 0)
		return propval.intval;

	return ret;
}

static int dubai_get_status(struct power_supply *psy)
{
	int ret;
	union power_supply_propval propval;

	ret = dubai_get_property(psy, POWER_SUPPLY_PROP_STATUS, &propval);
	if (ret == 0) {
		dubai_info("Get power supply status: %d", propval.intval);
		if ((propval.intval == POWER_SUPPLY_STATUS_CHARGING) || (propval.intval == POWER_SUPPLY_STATUS_FULL))
			return 1;
		else
			return 0;
	}

	return ret;
}

static void dubai_get_brand(struct power_supply *psy, char *brand, size_t len)
{
	int ret;
	union power_supply_propval propval;

	ret = dubai_get_property(psy, POWER_SUPPLY_PROP_BRAND, &propval);
	if (ret == 0)
		strncpy(brand, propval.strval, len - 1);
	else
		strncpy(brand, "Unknown", len - 1);
}

int dubai_get_battery_prop(void __user *argp) {

	int ret = 0;
	struct power_supply *psy = NULL;
	union dubai_battery_prop_value propval;

	if (copy_from_user(&propval, argp, sizeof(union dubai_battery_prop_value))) {
		dubai_err("Failed to get argp");
		return -EFAULT;
	}

	psy = power_supply_get_by_name(BATTERY_SUPPLY_NAME);
	if (psy == NULL) {
		dubai_err("Failed to get power supply(%s)", BATTERY_SUPPLY_NAME);
		return  -EINVAL;
	}

	switch (propval.prop) {
	case DUBAI_BATTERY_PROP_GAS_GAUGE:
		propval.value = dubai_get_gas_gauge(psy);
		break;
	case DUBAI_BATTERY_PROP_VOLTAGE_NOW:
		propval.value = dubai_get_voltage(psy);
		break;
	case DUBAI_BATTERY_PROP_CURRENT_NOW:
		propval.value = dubai_get_current(psy);
		break;
	case DUBAI_BATTERY_PROP_CYCLE_COUNT:
		propval.value = dubai_get_cycle_count(psy);
		break;
	case DUBAI_BATTERY_PROP_CHARGE_FULL:
		propval.value = dubai_get_charge_full(psy);
		break;
	case DUBAI_BATTERY_PROP_LEVEL:
		propval.value = dubai_get_level(psy);
		break;
	case DUBAI_BATTERY_PROP_BRAND:
		dubai_get_brand(psy, propval.brand, BATTERY_BRAND_MAX_LEN);
		propval.brand[BATTERY_BRAND_MAX_LEN - 1] = '\0';
		break;
	case DUBAI_BATTERY_PROP_CHARGE_STATUS:
		propval.value = dubai_get_status(psy);
		break;
	default:
		dubai_err("Invalid prop %d", propval.prop);
		ret = -EINVAL;
		break;
	}
	if (copy_to_user(argp, &propval, sizeof(union dubai_battery_prop_value))) {
		dubai_err("Failed to set prop: %d", propval.prop);
		return -EINVAL;
	}

	return ret;
}
