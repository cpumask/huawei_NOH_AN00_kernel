 /*
 * hisi_thermal.h
 *
 * head file for thermal
 *
 * Copyright (c) 2017-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifdef CONFIG_HISI_PERF_CTRL
#include <linux/hisi/thermal_perf_ctrl.h>
#endif
#ifdef CONFIG_HISI_ITS
#include <soc_its_para.h>
#endif

typedef u32 (*get_power_t)(void);
u32 get_camera_power(void);
u32 get_backlight_power(void);
u32 get_charger_power(void);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvisibility"
void board_cooling_unregister(struct thermal_cooling_device *cdev);
#pragma GCC diagnostic pop
struct thermal_cooling_device *board_power_cooling_register(struct device_node *np,
							    get_power_t get_power);
int ipa_get_tsensor_id(const char *name);
int ipa_get_sensor_value(u32 sensor, int *val);
int ipa_get_periph_id(const char *name);
int ipa_get_periph_value(u32 sensor, int *val);
int get_ipa_status(struct ipa_stat *status);
struct cpufreq_frequency_table *cpufreq_frequency_get_table(unsigned int cpu);
#ifdef CONFIG_HISI_THERMAL_SHELL
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvisibility"
int hisi_get_shell_temp(struct thermal_zone_device *thermal, int *temp);
#pragma GCC diagnostic pop
#endif
#ifdef CONFIG_HISI_THERMAL_GPU_HOTPLUG
extern void hisi_gpu_thermal_cores_control(u64 cores);
#endif
extern int devdrv_npu_ctrl_core(u32 dev_id, u32 core_num);
extern int coul_get_battery_cc(void);
extern int cpufreq_update_policies(void);
#ifdef CONFIG_HISI_ITS
extern int reset_power_result(void);
extern int get_its_power_result(its_cpu_power_t *result);
extern int get_its_core_dynamic_power(int core, unsigned long long *power);
extern int get_its_core_leakage_power(int core, unsigned long long *power);
extern bool check_its_enabled(void);
extern int get_gpu_leakage_power(unsigned long long *power);
extern int get_gpu_dynamic_power(unsigned long long *power);
#endif
#ifdef CONFIG_HISI_ITS_IPA
#define ITS_NORMALIZED_RATIO	1000000000UL
#define HZ_TO_MHZ_DIVISOR	1000000
#endif
