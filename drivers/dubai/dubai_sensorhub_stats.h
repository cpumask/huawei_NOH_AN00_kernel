#ifndef DUBAI_SENSORHUB_STATS_H
#define DUBAI_SENSORHUB_STATS_H

#include <linux/types.h>

#define PHYSICAL_SENSOR_TYPE_MAX			40

enum {
	TINY_CORE_MODEL_FD = 0,
	TINY_CORE_MODEL_SD,
	TINY_CORE_MODEL_FR,
	TINY_CORE_MODEL_AD,
	TINY_CORE_MODEL_FP,
	TINY_CORE_MODEL_HD,
	TINY_CORE_MODEL_WD,
	TINY_CORE_MODEL_WE,
	TINY_CORE_MODEL_WC,
	TINY_CORE_MODEL_MAX,
};

struct swing_data {
	uint64_t active_time;
	uint64_t software_standby_time;
	uint64_t als_time;
	uint64_t fill_light_time;
	uint32_t capture_cnt;
	uint32_t fill_light_cnt;
	uint32_t tiny_core_model_cnt[TINY_CORE_MODEL_MAX];
} __packed;

struct tp_duration {
	uint64_t active_time;
	uint64_t idle_time;
} __packed;

struct sensor_time {
	uint32_t type;
	uint32_t time;
} __packed;

struct sensor_time_transmit {
	int32_t count;
	struct sensor_time data[PHYSICAL_SENSOR_TYPE_MAX];
} __packed;

#ifdef CONFIG_INPUTHUB_20
int dubai_get_aod_duration(void __user *argp);
int dubai_get_tp_duration(void __user *argp);
int dubai_get_sensorhub_type_list(void __user * argp);
int dubai_get_all_sensor_stats(void __user *argp);
int dubai_get_fp_icon_stats(void __user *argp);
int dubai_get_swing_data(void __user *argp);
#else /* !CONFIG_INPUTHUB_20 */
static inline int dubai_get_aod_duration(void __user __always_unused *argp) { return -EOPNOTSUPP; }
static inline int dubai_get_tp_duration(void __user __always_unused *argp) { return -EOPNOTSUPP; }
static inline int dubai_get_sensorhub_type_list(void __user __always_unused *argp) { return -EOPNOTSUPP; }
static inline int dubai_get_all_sensor_stats(void __user __always_unused *argp) { return -EOPNOTSUPP; }
static inline int dubai_get_fp_icon_stats(void __user __always_unused *argp) { return -EOPNOTSUPP; }
static inline int dubai_get_swing_data(void __user __always_unused *argp) { return -EOPNOTSUPP; }
#endif /* !CONFIG_INPUTHUB_20 */

#endif // DUBAI_SENSORHUB_STATS_H
