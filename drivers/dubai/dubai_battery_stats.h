#ifndef DUBAI_BATTERY_STATS_H
#define DUBAI_BATTERY_STATS_H

#define BATTERY_BRAND_MAX_LEN 	32

union dubai_battery_prop_value {
	int prop;
	int value;
	char brand[BATTERY_BRAND_MAX_LEN];
} __packed;

int dubai_get_battery_prop(void __user *argp);

#endif // DUBAI_BATTERY_STATS_H
