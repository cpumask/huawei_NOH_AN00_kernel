/*
 *  linux/drivers/thermal/cpu_cooling.c
 *
 *  Copyright (C) 2012	Samsung Electronics Co., Ltd(http://www.samsung.com)
 *  Copyright (C) 2012  Amit Daniel <amit.kachhap@linaro.org>
 *
 *  Copyright (C) 2014  Viresh Kumar <viresh.kumar@linaro.org>
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include <linux/module.h>
#include <linux/thermal.h>
#include <linux/cpufreq.h>
#include <linux/err.h>
#include <linux/idr.h>
#include <linux/pm_opp.h>
#include <linux/slab.h>
#include <linux/cpu.h>
#include <linux/cpu_cooling.h>

#include <trace/events/thermal.h>

#ifdef CONFIG_HISI_DRG
#include <linux/hisi/hisi_drg.h>
#endif
#ifdef CONFIG_HISI_ITS_IPA
#include <linux/hisi/hisi_thermal.h>
#endif
#ifdef CONFIG_HISI_IPA_THERMAL
#include <trace/events/thermal_power_allocator.h>
#ifdef CONFIG_HISI_THERMAL_SPM
extern unsigned int get_powerhal_profile(int actor);
extern unsigned int get_minfreq_profile(int actor);
extern bool is_spm_mode_enabled(void);

u32 profile_freq[CAPACITY_OF_ARRAY];
int hisi_calc_static_power(const struct cpumask *cpumask, int temp,
				unsigned long u_volt, u32 *static_power);
#endif
extern unsigned int g_ipa_freq_limit[];
extern unsigned int g_ipa_soc_freq_limit[];
extern unsigned int g_ipa_board_freq_limit[];
extern unsigned int g_ipa_board_state[];
extern unsigned int g_ipa_soc_state[];
#endif

/*
 * Cooling state <-> CPUFreq frequency
 *
 * Cooling states are translated to frequencies throughout this driver and this
 * is the relation between them.
 *
 * Highest cooling state corresponds to lowest possible frequency.
 *
 * i.e.
 *	level 0 --> 1st Max Freq
 *	level 1 --> 2nd Max Freq
 *	...
 */

/**
 * struct freq_table - frequency table along with power entries
 * @frequency:	frequency in KHz
 * @power:	power in mW
 *
 * This structure is built when the cooling device registers and helps
 * in translating frequency to power and vice versa.
 */
struct freq_table {
	u32 frequency;
	u32 power;
};

/**
 * struct time_in_idle - Idle time stats
 * @time: previous reading of the absolute time that this cpu was idle
 * @timestamp: wall time of the last invocation of get_cpu_idle_time_us()
 */
struct time_in_idle {
	u64 time;
	u64 timestamp;
};

/**
 * struct cpufreq_cooling_device - data for cooling device with cpufreq
 * @id: unique integer value corresponding to each cpufreq_cooling_device
 *	registered.
 * @last_load: load measured by the latest call to cpufreq_get_requested_power()
 * @cpufreq_state: integer value representing the current state of cpufreq
 *	cooling	devices.
 * @clipped_freq: integer value representing the absolute value of the clipped
 *	frequency.
 * @max_level: maximum cooling level. One less than total number of valid
 *	cpufreq frequencies.
 * @freq_table: Freq table in descending order of frequencies
 * @cdev: thermal_cooling_device pointer to keep track of the
 *	registered cooling device.
 * @policy: cpufreq policy.
 * @node: list_head to link all cpufreq_cooling_device together.
 * @idle_time: idle time stats
 * @plat_get_static_power: callback to calculate the static power
 *
 * This structure is required for keeping information of each registered
 * cpufreq_cooling_device.
 */
struct cpufreq_cooling_device {
	int id;
	u32 last_load;
	unsigned int cpufreq_state;
	unsigned int clipped_freq;
	unsigned int max_level;
	struct freq_table *freq_table;	/* In descending order */
	struct thermal_cooling_device *cdev;
	struct cpufreq_policy *policy;
	struct list_head node;
	struct time_in_idle *idle_time;
	get_static_t plat_get_static_power;
#ifdef CONFIG_HISI_ITS_IPA
	unsigned long normalized_powerdata;
#endif
};

static DEFINE_IDA(cpufreq_ida);
static DEFINE_MUTEX(cooling_cpufreq_lock);
static DEFINE_MUTEX(cooling_list_lock);
static LIST_HEAD(cpufreq_cdev_list);

/* Below code defines functions to be used for cpufreq as cooling device */

/**
 * get_level: Find the level for a particular frequency
 * @cpufreq_cdev: cpufreq_cdev for which the property is required
 * @freq: Frequency
 *
 * Return: level corresponding to the frequency.
 */
static unsigned long get_level(struct cpufreq_cooling_device *cpufreq_cdev,
			       unsigned int freq)
{
	struct freq_table *freq_table = cpufreq_cdev->freq_table;
	unsigned long level;

	for (level = 1; level <= cpufreq_cdev->max_level; level++)
		if (freq > freq_table[level].frequency)
			break;

	return level - 1;
}

/**
 * cpufreq_thermal_notifier - notifier callback for cpufreq policy change.
 * @nb:	struct notifier_block * with callback info.
 * @event: value showing cpufreq event for which this function invoked.
 * @data: callback-specific data
 *
 * Callback to hijack the notification on cpufreq policy transition.
 * Every time there is a change in policy, we will intercept and
 * update the cpufreq policy with thermal constraints.
 *
 * Return: 0 (success)
 */
static int cpufreq_thermal_notifier(struct notifier_block *nb,
				    unsigned long event, void *data)
{
	struct cpufreq_policy *policy = data;
	unsigned long clipped_freq;
	struct cpufreq_cooling_device *cpufreq_cdev;
#ifdef CONFIG_HISI_THERMAL_SPM
	int actor;
	unsigned int min_freq = 0, freq = 0;
#endif

	if (event != CPUFREQ_ADJUST)
		return NOTIFY_DONE;

	mutex_lock(&cooling_list_lock);
	list_for_each_entry(cpufreq_cdev, &cpufreq_cdev_list, node) {
		/*
		 * A new copy of the policy is sent to the notifier and can't
		 * compare that directly.
		 */
		if (policy->cpu != cpufreq_cdev->policy->cpu)
			continue;

		/*
		 * policy->max is the maximum allowed frequency defined by user
		 * and clipped_freq is the maximum that thermal constraints
		 * allow.
		 *
		 * If clipped_freq is lower than policy->max, then we need to
		 * readjust policy->max.
		 *
		 * But, if clipped_freq is greater than policy->max, we don't
		 * need to do anything.
		 */
		clipped_freq = cpufreq_cdev->clipped_freq;
#ifdef CONFIG_HISI_FREQ_LIMIT_COUNTER
		update_cpu_ipa_max_freq(policy, clipped_freq);
#endif

#ifndef CONFIG_HISI_THERMAL_SPM
		if (policy->max > clipped_freq)
			cpufreq_verify_within_limits(policy, 0, clipped_freq);
#else
		if (is_spm_mode_enabled()) {
			actor = topology_physical_package_id(policy->cpu);
			freq = get_powerhal_profile(actor);
			min_freq = get_minfreq_profile(actor);
			cpufreq_verify_within_limits(policy, min_freq, freq);
		} else {
			if (policy->max != clipped_freq)
				cpufreq_verify_within_limits(policy, 0, clipped_freq);
		}
#endif
		break;
	}
	mutex_unlock(&cooling_list_lock);

	return NOTIFY_OK;
}

/**
 * update_freq_table() - Update the freq table with power numbers
 * @cpufreq_cdev:	the cpufreq cooling device in which to update the table
 * @capacitance: dynamic power coefficient for these cpus
 *
 * Update the freq table with power numbers.  This table will be used in
 * cpu_power_to_freq() and cpu_freq_to_power() to convert between power and
 * frequency efficiently.  Power is stored in mW, frequency in KHz.  The
 * resulting table is in descending order.
 *
 * Return: 0 on success, -EINVAL if there are no OPPs for any CPUs,
 * or -ENOMEM if we run out of memory.
 */
static int update_freq_table(struct cpufreq_cooling_device *cpufreq_cdev,
			     u32 capacitance)
{
	struct freq_table *freq_table = cpufreq_cdev->freq_table;
	struct dev_pm_opp *opp;
	struct device *dev = NULL;
	int num_opps = 0, cpu = cpufreq_cdev->policy->cpu, i;
#ifdef CONFIG_HISI_IPA_THERMAL
	u32 static_power;
	int nr_cpus;
#endif

	dev = get_cpu_device(cpu);
	if (unlikely(!dev)) {
		dev_warn(&cpufreq_cdev->cdev->device,
			 "No cpu device for cpu %d\n", cpu);
		return -ENODEV;
	}

	num_opps = dev_pm_opp_get_opp_count(dev);
	if (num_opps < 0)
		return num_opps;

	/*
	 * The cpufreq table is also built from the OPP table and so the count
	 * should match.
	 */
	if (num_opps != cpufreq_cdev->max_level + 1) {
		dev_warn(dev, "Number of OPPs not matching with max_levels\n");
		return -EINVAL;
	}

	for (i = 0; i <= cpufreq_cdev->max_level; i++) { /*lint !e574*/
		unsigned long freq = freq_table[i].frequency * 1000; /*lint !e647*/
		u32 freq_mhz = freq_table[i].frequency / 1000;
		u64 power;
		u32 voltage_mv;

		/*
		 * Find ceil frequency as 'freq' may be slightly lower than OPP
		 * freq due to truncation while converting to kHz.
		 */
		opp = dev_pm_opp_find_freq_ceil(dev, &freq);
		if (IS_ERR(opp)) {
			dev_err(dev, "failed to get opp for %lu frequency\n",
				freq);
			return -EINVAL;
		}

		voltage_mv = dev_pm_opp_get_voltage(opp) / 1000;
		dev_pm_opp_put(opp);

		/*
		 * Do the multiplication with MHz and millivolt so as
		 * to not overflow.
		 */
		power = (u64)capacitance * freq_mhz * voltage_mv * voltage_mv;
		do_div(power, 1000000000);

		/* power is stored in mW */
		freq_table[i].power = power;
#ifdef CONFIG_HISI_IPA_THERMAL
		nr_cpus = (int)cpumask_weight(cpufreq_cdev->policy->related_cpus);
		if (0 == nr_cpus)
			nr_cpus = 1;
		cpufreq_cdev->plat_get_static_power(cpufreq_cdev->policy->related_cpus, 0, (unsigned long)(voltage_mv * 1000), &static_power); /*lint !e647*/

		/* hisi static_power givern in cluster */
		static_power = static_power / (u32)nr_cpus;

		pr_err("  %u MHz @ %u mV :  %u + %u = %u mW\n",
			freq_mhz, voltage_mv, freq_table[i].power, static_power, freq_table[i].power+static_power);
#endif
	}

	return 0;
}

static u32 cpu_freq_to_power(struct cpufreq_cooling_device *cpufreq_cdev,
			     u32 freq)
{
	int i;
	struct freq_table *freq_table = cpufreq_cdev->freq_table;

	for (i = 1; i <= cpufreq_cdev->max_level; i++) /*lint !e574*/
		if (freq > freq_table[i].frequency)
			break;

	return freq_table[i - 1].power;
}

static u32 cpu_power_to_freq(struct cpufreq_cooling_device *cpufreq_cdev,
			     u32 power)
{
	int i;
	struct freq_table *freq_table = cpufreq_cdev->freq_table;

	for (i = 1; i <= cpufreq_cdev->max_level; i++) /*lint !e574*/
		if (power > freq_table[i].power)
			break;

	return freq_table[i - 1].frequency;
}

/**
 * get_load() - get load for a cpu since last updated
 * @cpufreq_cdev:	&struct cpufreq_cooling_device for this cpu
 * @cpu:	cpu number
 * @cpu_idx:	index of the cpu in time_in_idle*
 *
 * Return: The average load of cpu @cpu in percentage since this
 * function was last called.
 */
static u32 get_load(struct cpufreq_cooling_device *cpufreq_cdev, int cpu,
		    int cpu_idx)
{
	u32 load;
	u64 now, now_idle, delta_time, delta_idle;
	struct time_in_idle *idle_time = &cpufreq_cdev->idle_time[cpu_idx];

	now_idle = get_cpu_idle_time(cpu, &now, 0);
	delta_idle = now_idle - idle_time->time;
	delta_time = now - idle_time->timestamp;

	if (delta_time <= delta_idle)
		load = 0;
	else
		load = div64_u64(100 * (delta_time - delta_idle), delta_time);

	idle_time->time = now_idle;
	idle_time->timestamp = now;

	return load;
}

/**
 * get_static_power() - calculate the static power consumed by the cpus
 * @cpufreq_cdev:	struct &cpufreq_cooling_device for this cpu cdev
 * @tz:		thermal zone device in which we're operating
 * @freq:	frequency in KHz
 * @power:	pointer in which to store the calculated static power
 *
 * Calculate the static power consumed by the cpus described by
 * @cpu_actor running at frequency @freq.  This function relies on a
 * platform specific function that should have been provided when the
 * actor was registered.  If it wasn't, the static power is assumed to
 * be negligible.  The calculated static power is stored in @power.
 *
 * Return: 0 on success, -E* on failure.
 */
static int get_static_power(struct cpufreq_cooling_device *cpufreq_cdev,
			    struct thermal_zone_device *tz, unsigned long freq,
			    u32 *power)
{
	struct dev_pm_opp *opp;
	unsigned long voltage;
	struct cpufreq_policy *policy = cpufreq_cdev->policy;
	struct cpumask *cpumask = policy->related_cpus;
	unsigned long freq_hz = freq * 1000;
	struct device *dev;

#ifdef CONFIG_HISI_ITS_IPA
	int cpu, ret;
	unsigned long long temp_power = 0;

	if (check_its_enabled()) {
		*power = 0;
		for_each_cpu(cpu, policy->related_cpus) { /*lint !e574*/
			ret = get_its_core_leakage_power(cpu, &temp_power);
			if (ret == 0)
				*power += temp_power;
		}
		return 0;
	}
#endif

	if (!cpufreq_cdev->plat_get_static_power) {
		*power = 0;
		return 0;
	}

	dev = get_cpu_device(policy->cpu);
	WARN_ON(!dev); /*lint !e146 !e665*/

#ifdef CONFIG_HISI_IPA_THERMAL
	if (dev == NULL) {
		*power = 0;
		return 0;
	}

	device_lock(dev);
	if (dev->offline == true) {
		*power = 0;
		device_unlock(dev);
		return 0;
	}
	device_unlock(dev);
#endif
	opp = dev_pm_opp_find_freq_exact(dev, freq_hz, true);
	if (IS_ERR(opp)) {
		dev_warn_ratelimited(dev, "Failed to find OPP for frequency %lu: %ld\n",
				     freq_hz, PTR_ERR(opp));
		return -EINVAL;
	}

	voltage = dev_pm_opp_get_voltage(opp);
	dev_pm_opp_put(opp);

	if (voltage == 0) {
		dev_err_ratelimited(dev, "Failed to get voltage for frequency %lu\n",
				    freq_hz);
		return -EINVAL;
	}

	return cpufreq_cdev->plat_get_static_power(cpumask, tz->passive_delay,
						  voltage, power);
}

/**
 * get_dynamic_power() - calculate the dynamic power
 * @cpufreq_cdev:	&cpufreq_cooling_device for this cdev
 * @freq:	current frequency
 *
 * Return: the dynamic power consumed by the cpus described by
 * @cpufreq_cdev.
 */
static u32 get_dynamic_power(struct cpufreq_cooling_device *cpufreq_cdev,
			     unsigned long freq)
{
	u32 raw_cpu_power;

#ifdef CONFIG_HISI_ITS_IPA
	struct cpufreq_policy *policy = cpufreq_cdev->policy;
	int cpu, ret;
	unsigned long long temp_power = 0;
	unsigned long long total_power = 0;

	if (check_its_enabled()) {
		for_each_cpu(cpu, policy->related_cpus) { /*lint !e574*/
			ret = get_its_core_dynamic_power(cpu, &temp_power);
			if (ret == 0)
				total_power += temp_power;
		}
		return (u32)total_power;
	}
#endif

	raw_cpu_power = cpu_freq_to_power(cpufreq_cdev, freq);
	return (raw_cpu_power * cpufreq_cdev->last_load) / 100;
}

/* cpufreq cooling device callback functions are defined below */

/**
 * cpufreq_get_max_state - callback function to get the max cooling state.
 * @cdev: thermal cooling device pointer.
 * @state: fill this variable with the max cooling state.
 *
 * Callback for the thermal cooling device to return the cpufreq
 * max cooling state.
 *
 * Return: 0 on success, an error code otherwise.
 */
static int cpufreq_get_max_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{
	struct cpufreq_cooling_device *cpufreq_cdev = cdev->devdata;

	*state = cpufreq_cdev->max_level;
	return 0;
}

/**
 * cpufreq_get_cur_state - callback function to get the current cooling state.
 * @cdev: thermal cooling device pointer.
 * @state: fill this variable with the current cooling state.
 *
 * Callback for the thermal cooling device to return the cpufreq
 * current cooling state.
 *
 * Return: 0 on success, an error code otherwise.
 */
static int cpufreq_get_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long *state)
{
	struct cpufreq_cooling_device *cpufreq_cdev = cdev->devdata;

	*state = cpufreq_cdev->cpufreq_state;

	return 0;
}

/**
 * cpufreq_set_cur_state - callback function to set the current cooling state.
 * @cdev: thermal cooling device pointer.
 * @state: set this variable to the current cooling state.
 *
 * Callback for the thermal cooling device to change the cpufreq
 * current cooling state.
 *
 * Return: 0 on success, an error code otherwise.
 */
static int cpufreq_set_cur_state(struct thermal_cooling_device *cdev,
				 unsigned long state)
{
	struct cpufreq_cooling_device *cpufreq_cdev = cdev->devdata;
	unsigned int clip_freq;
#ifdef CONFIG_HISI_IPA_THERMAL
	unsigned int cpu = cpumask_any(cpufreq_cdev->policy->related_cpus);
	unsigned int cur_cluster;
	unsigned long limit_state;
#endif

	/* Request state should be less than max_level */
	if (WARN_ON(state > cpufreq_cdev->max_level)) /*lint !e146 !e665*/
		return -EINVAL;

#ifdef CONFIG_HISI_IPA_THERMAL
	cur_cluster = (unsigned int)topology_physical_package_id(cpu);

	if (g_ipa_soc_state[cur_cluster] <= cpufreq_cdev->max_level)
		g_ipa_soc_freq_limit[cur_cluster] = cpufreq_cdev->freq_table[g_ipa_soc_state[cur_cluster]].frequency;

	if (g_ipa_board_state[cur_cluster] <= cpufreq_cdev->max_level)
		g_ipa_board_freq_limit[cur_cluster] = cpufreq_cdev->freq_table[g_ipa_board_state[cur_cluster]].frequency;

	limit_state = max(g_ipa_soc_state[cur_cluster], g_ipa_board_state[cur_cluster]);/*lint !e1058*/

	/* only change new state when limit_state less than max_level */
	if (!WARN_ON(limit_state > cpufreq_cdev->max_level)) /*lint !e146 !e665*/
		state = max(state, limit_state);
#endif
	/* Check if the old cooling action is same as new cooling action */
	if (cpufreq_cdev->cpufreq_state == state)
		return 0;

	clip_freq = cpufreq_cdev->freq_table[state].frequency;
	cpufreq_cdev->cpufreq_state = state;
	cpufreq_cdev->clipped_freq = clip_freq;

#ifdef CONFIG_HISI_IPA_THERMAL
	g_ipa_freq_limit[cur_cluster] = clip_freq;
#endif
#ifdef CONFIG_HISI_DRG
	drg_cpufreq_cooling_update(cpu, clip_freq);
#endif
	cpufreq_update_policy(cpufreq_cdev->policy->cpu);

	return 0;
}

#ifdef CONFIG_HISI_ITS_IPA
static unsigned long get_voltage(struct cpufreq_cooling_device *cpufreq_cdev,
				unsigned long freq)
{
	struct device *dev = NULL;
	struct dev_pm_opp *opp = NULL;
	struct cpufreq_policy *policy = cpufreq_cdev->policy;
	unsigned long freq_hz = freq * 1000;
	unsigned long voltage;

	dev = get_cpu_device(policy->cpu);

	if (dev == NULL)
		return 0;

	if (dev->offline == true)
		return 0;

	opp = dev_pm_opp_find_freq_exact(dev, freq_hz, true);

	if (IS_ERR(opp)) {
		dev_warn_ratelimited(dev,
				"Failed to find OPP for frequency %lu:%ld\n",
				freq_hz, PTR_ERR(opp));
		return 0;
	}

	voltage = dev_pm_opp_get_voltage(opp);
	dev_pm_opp_put(opp);
	if (voltage == 0) {
		dev_err_ratelimited(dev,
				"Failed to get voltage for frequency %lu\n",
				freq_hz);
		return 0;
	}

	return voltage;
}

unsigned long normalize_its_power(struct cpufreq_cooling_device *cpufreq_cdev,
		unsigned int dynamic_power, unsigned long freq)
{
	unsigned long voltage, freq_mhz;
	unsigned long long normalized_data = 0;

	/* get_voltage() will get uV, so div 100 for mV */
	voltage = get_voltage(cpufreq_cdev, freq) / 1000;
	freq_mhz = freq / 1000;

	if (freq_mhz != 0 && voltage != 0)
		normalized_data = ITS_NORMALIZED_RATIO * dynamic_power /
					(voltage * voltage * freq_mhz);

	trace_IPA_actor_cpu_normalize_power(normalized_data, dynamic_power,
					    freq_mhz, voltage);
	return (unsigned long)normalized_data;
}
#endif

/**
 * cpufreq_get_requested_power() - get the current power
 * @cdev:	&thermal_cooling_device pointer
 * @tz:		a valid thermal zone device pointer
 * @power:	pointer in which to store the resulting power
 *
 * Calculate the current power consumption of the cpus in milliwatts
 * and store it in @power.  This function should actually calculate
 * the requested power, but it's hard to get the frequency that
 * cpufreq would have assigned if there were no thermal limits.
 * Instead, we calculate the current power on the assumption that the
 * immediate future will look like the immediate past.
 *
 * We use the current frequency and the average load since this
 * function was last called.  In reality, there could have been
 * multiple opps since this function was last called and that affects
 * the load calculation.  While it's not perfectly accurate, this
 * simplification is good enough and works.  REVISIT this, as more
 * complex code may be needed if experiments show that it's not
 * accurate enough.
 *
 * Return: 0 on success, -E* if getting the static power failed.
 */
static int cpufreq_get_requested_power(struct thermal_cooling_device *cdev,
				       struct thermal_zone_device *tz,
				       u32 *power)
{
	unsigned long freq;
	int i = 0, cpu, ret;
	u32 static_power, dynamic_power, total_load = 0;
	struct cpufreq_cooling_device *cpufreq_cdev = cdev->devdata;
	struct cpufreq_policy *policy = cpufreq_cdev->policy;
	u32 *load_cpu = NULL;
#ifdef CONFIG_HISI_IPA_THERMAL
	u32 max_load = 0;
#endif

	freq = cpufreq_quick_get(policy->cpu);
#ifdef CONFIG_HISI_IPA_THERMAL
	/* policy->cur equals 0, means the policy data of this cpu was NULL,
	*  return early to avoid find voltage of freq(0) in the opp
	*/
	if (!freq) {
		*power = 0;
		return 0;
	}
#endif

#ifdef CONFIG_HISI_IPA_THERMAL
	if (1) { /*lint !e774*/
#else
	if (trace_thermal_power_cpu_get_power_enabled()) {
#endif
		u32 ncpus = cpumask_weight(policy->related_cpus);

		load_cpu = kcalloc(ncpus, sizeof(*load_cpu), GFP_KERNEL);
	}

	for_each_cpu(cpu, policy->related_cpus) { /*lint !e574*/
		u32 load;

		if (cpu_online(cpu))
			load = get_load(cpufreq_cdev, cpu, i);
		else
			load = 0;

		total_load += load;
#ifdef CONFIG_HISI_IPA_THERMAL
		if (load > max_load)
			max_load = load;
#endif
#ifdef CONFIG_HISI_IPA_THERMAL
		if (load_cpu != NULL)
#else
		if (trace_thermal_power_cpu_limit_enabled() && load_cpu)
#endif
			load_cpu[i] = load;

		i++;
	}

	cpufreq_cdev->last_load = total_load;

	dynamic_power = get_dynamic_power(cpufreq_cdev, freq);
	ret = get_static_power(cpufreq_cdev, tz, freq, &static_power);
	if (ret) {
		kfree(load_cpu); /*lint !e668*/
		return ret;
	}
#ifdef CONFIG_HISI_ITS_IPA
	cpufreq_cdev->normalized_powerdata = normalize_its_power(cpufreq_cdev,
								dynamic_power,
								freq);
#endif
	if (load_cpu) {
		trace_thermal_power_cpu_get_power(policy->related_cpus, freq,
						  load_cpu, i, dynamic_power,
						  static_power);

#ifdef CONFIG_HISI_IPA_THERMAL
		if (tz->is_soc_thermal) {
			trace_IPA_actor_cpu_get_power(policy->related_cpus, freq,
				load_cpu, (unsigned long)((long)i), dynamic_power,
				static_power, (static_power + dynamic_power));
		}
#endif

		kfree(load_cpu);
	}

	*power = static_power + dynamic_power;

#ifdef CONFIG_HISI_IPA_THERMAL
	cdev->current_freq = freq;
	if (load_cpu != NULL)
		cdev->current_load = max_load;
#endif

	return 0;
}

/**
 * cpufreq_state2power() - convert a cpu cdev state to power consumed
 * @cdev:	&thermal_cooling_device pointer
 * @tz:		a valid thermal zone device pointer
 * @state:	cooling device state to be converted
 * @power:	pointer in which to store the resulting power
 *
 * Convert cooling device state @state into power consumption in
 * milliwatts assuming 100% load.  Store the calculated power in
 * @power.
 *
 * Return: 0 on success, -EINVAL if the cooling device state could not
 * be converted into a frequency or other -E* if there was an error
 * when calculating the static power.
 */
static int cpufreq_state2power(struct thermal_cooling_device *cdev,
			       struct thermal_zone_device *tz,
			       unsigned long state, u32 *power)
{
	unsigned int freq, num_cpus;
	u32 static_power, dynamic_power;
	int ret;
	struct cpufreq_cooling_device *cpufreq_cdev = cdev->devdata;

	/* Request state should be less than max_level */
	if (WARN_ON(state > cpufreq_cdev->max_level)) /*lint !e146 !e665*/
		return -EINVAL;

	num_cpus = cpumask_weight(cpufreq_cdev->policy->cpus);

	freq = cpufreq_cdev->freq_table[state].frequency;
	dynamic_power = cpu_freq_to_power(cpufreq_cdev, freq) * num_cpus;
	ret = get_static_power(cpufreq_cdev, tz, freq, &static_power);
	if (ret)
		return ret;

	*power = static_power + dynamic_power;
	return ret;
}

#ifdef CONFIG_HISI_ITS_IPA
unsigned int
its_normalized_data_to_freq(struct cpufreq_cooling_device *cpufreq_cdev,
			    unsigned long power)
{
	struct freq_table *freq_table = cpufreq_cdev->freq_table;
	struct cpufreq_policy *policy = cpufreq_cdev->policy;
	unsigned long long calc_power = 0;
	unsigned int freq, freq_mhz, voltage, level;
	unsigned int ncpus = cpumask_weight(policy->related_cpus);

	for (level = 0; level < cpufreq_cdev->max_level; level++) {
		freq = freq_table[level].frequency;
		freq_mhz = freq / 1000;
		voltage = get_voltage(cpufreq_cdev, freq) / 1000;
		if (freq != 0)
			calc_power = ((unsigned long long)
				       cpufreq_cdev->normalized_powerdata *
				       freq_mhz * voltage * voltage) /
				       ITS_NORMALIZED_RATIO;

		trace_IPA_actor_cpu_pdata_to_freq(level, freq_mhz,
					voltage, calc_power,
					freq_table[level].power * ncpus,
					cpufreq_cdev->normalized_powerdata);
		if ((unsigned long long)power >= calc_power ||
		    power >= freq_table[level].power * ncpus)
			break;
	}
	return freq_table[level].frequency;
};
#endif

/**
 * cpufreq_power2state() - convert power to a cooling device state
 * @cdev:	&thermal_cooling_device pointer
 * @tz:		a valid thermal zone device pointer
 * @power:	power in milliwatts to be converted
 * @state:	pointer in which to store the resulting state
 *
 * Calculate a cooling device state for the cpus described by @cdev
 * that would allow them to consume at most @power mW and store it in
 * @state.  Note that this calculation depends on external factors
 * such as the cpu load or the current static power.  Calling this
 * function with the same power as input can yield different cooling
 * device states depending on those external factors.
 *
 * Return: 0 on success, -ENODEV if no cpus are online or -EINVAL if
 * the calculated frequency could not be converted to a valid state.
 * The latter should not happen unless the frequencies available to
 * cpufreq have changed since the initialization of the cpu cooling
 * device.
 */
static int cpufreq_power2state(struct thermal_cooling_device *cdev,
			       struct thermal_zone_device *tz, u32 power,
			       unsigned long *state)
{
	unsigned int cur_freq, target_freq;
	int ret;
	s32 dyn_power;
	u32 last_load, normalised_power, static_power;
	struct cpufreq_cooling_device *cpufreq_cdev = cdev->devdata;
	struct cpufreq_policy *policy = cpufreq_cdev->policy;

	cur_freq = cpufreq_quick_get(policy->cpu);
#ifdef CONFIG_HISI_IPA_THERMAL
	if (!cur_freq)
		return -EINVAL;
#endif
	ret = get_static_power(cpufreq_cdev, tz, cur_freq, &static_power);
	if (ret)
		return ret;

	dyn_power = power - static_power;
	dyn_power = dyn_power > 0 ? dyn_power : 0;
#ifdef CONFIG_HISI_ITS_IPA
	if (check_its_enabled()) {
		target_freq = its_normalized_data_to_freq(cpufreq_cdev,
							  dyn_power);
	} else {
#endif
		last_load = cpufreq_cdev->last_load ?: 1;
		normalised_power = (dyn_power * 100) / last_load;/*lint !e573*/
		target_freq = cpu_power_to_freq(cpufreq_cdev, normalised_power);
#ifdef CONFIG_HISI_ITS_IPA
	}
#endif
	*state = get_level(cpufreq_cdev, target_freq);
	trace_thermal_power_cpu_limit(policy->related_cpus, target_freq, *state,
				      power);
#ifdef CONFIG_HISI_IPA_THERMAL
	trace_IPA_actor_cpu_limit(policy->related_cpus, target_freq,
					*state, power);
#endif
	return 0;
}

/* Bind cpufreq callbacks to thermal cooling device ops */

static struct thermal_cooling_device_ops cpufreq_cooling_ops = {
	.get_max_state = cpufreq_get_max_state,
	.get_cur_state = cpufreq_get_cur_state,
	.set_cur_state = cpufreq_set_cur_state,
};

static struct thermal_cooling_device_ops cpufreq_power_cooling_ops = {
	.get_max_state		= cpufreq_get_max_state,
	.get_cur_state		= cpufreq_get_cur_state,
	.set_cur_state		= cpufreq_set_cur_state,
	.get_requested_power	= cpufreq_get_requested_power,
	.state2power		= cpufreq_state2power,
	.power2state		= cpufreq_power2state,
};

/* Notifier for cpufreq policy change */
static struct notifier_block thermal_cpufreq_notifier_block = {
	.notifier_call = cpufreq_thermal_notifier,
};

static unsigned int find_next_max(struct cpufreq_frequency_table *table,
				  unsigned int prev_max)
{
	struct cpufreq_frequency_table *pos;
	unsigned int max = 0;

	cpufreq_for_each_valid_entry(pos, table) {
		if (pos->frequency > max && pos->frequency < prev_max)
			max = pos->frequency;
	}

	return max;
}

#ifdef CONFIG_HISI_THERMAL_SPM
/*lint -e64 -e826 -e771 -esym(64,826,771,*)*/
int cpufreq_update_policies(void)
{
	struct cpufreq_cooling_device *cpufreq_cdev;
	unsigned int cpus[g_cluster_num];
	int i, num = 0;

	mutex_lock(&cooling_cpufreq_lock);
	list_for_each_entry(cpufreq_cdev, &cpufreq_cdev_list, node) {
		if (num >= (int)g_cluster_num)
			break;
		cpus[num] = cpumask_any(cpufreq_cdev->policy->related_cpus);
		num++;
	}
	mutex_unlock(&cooling_cpufreq_lock);

	for (i = 0; i < num; i++)
		cpufreq_update_policy(cpus[i]);

	return 0;
}
/*lint -e64 -e826 -e771 +esym(64,826,771,*)*/
EXPORT_SYMBOL(cpufreq_update_policies);
#endif

/**
 * __cpufreq_cooling_register - helper function to create cpufreq cooling device
 * @np: a valid struct device_node to the cooling device device tree node
 * @policy: cpufreq policy
 * Normally this should be same as cpufreq policy->related_cpus.
 * @capacitance: dynamic power coefficient for these cpus
 * @plat_static_func: function to calculate the static power consumed by these
 *                    cpus (optional)
 *
 * This interface function registers the cpufreq cooling device with the name
 * "thermal-cpufreq-%x". This api can support multiple instances of cpufreq
 * cooling devices. It also gives the opportunity to link the cooling device
 * with a device tree node, in order to bind it via the thermal DT code.
 *
 * Return: a valid struct thermal_cooling_device pointer on success,
 * on failure, it returns a corresponding ERR_PTR().
 */
static struct thermal_cooling_device *
__cpufreq_cooling_register(struct device_node *np,
			struct cpufreq_policy *policy, u32 capacitance,
			get_static_t plat_static_func)
{
	struct thermal_cooling_device *cdev;
	struct cpufreq_cooling_device *cpufreq_cdev;
	char dev_name[THERMAL_NAME_LENGTH]; /*lint !e578*/
	unsigned int freq, i, num_cpus;
	int ret;
	struct thermal_cooling_device_ops *cooling_ops;
	bool first;

	if (IS_ERR_OR_NULL(policy)) {
		pr_err("%s: cpufreq policy isn't valid: %p", __func__, policy);
		return ERR_PTR(-EINVAL);
	}

	i = cpufreq_table_count_valid_entries(policy);
	if (!i) {
		pr_debug("%s: CPUFreq table not found or has no valid entries\n",
			 __func__);
		return ERR_PTR(-ENODEV);
	}
	cpufreq_cdev = kzalloc(sizeof(*cpufreq_cdev), GFP_KERNEL);
	if (!cpufreq_cdev)
		return ERR_PTR(-ENOMEM);

	cpufreq_cdev->policy = policy;
	num_cpus = cpumask_weight(policy->related_cpus);
	cpufreq_cdev->idle_time = kcalloc(num_cpus,
					 sizeof(*cpufreq_cdev->idle_time),
					 GFP_KERNEL);
	if (!cpufreq_cdev->idle_time) {
		cdev = ERR_PTR(-ENOMEM);
		goto free_cdev;
	}

	/* max_level is an index, not a counter */
	cpufreq_cdev->max_level = i - 1;

	cpufreq_cdev->freq_table = kmalloc_array(i,
					sizeof(*cpufreq_cdev->freq_table),
					GFP_KERNEL);
	if (!cpufreq_cdev->freq_table) {
		cdev = ERR_PTR(-ENOMEM);
		goto free_idle_time;
	}

	ret = ida_simple_get(&cpufreq_ida, 0, 0, GFP_KERNEL);
	if (ret < 0) {
		cdev = ERR_PTR(ret);
		goto free_table;
	}
	cpufreq_cdev->id = ret;

	snprintf(dev_name, sizeof(dev_name), "thermal-cpufreq-%d",
		 cpufreq_cdev->id);

	/* Fill freq-table in descending order of frequencies */
	for (i = 0, freq = -1; i <= cpufreq_cdev->max_level; i++) { /*lint !e570*/
		freq = find_next_max(policy->freq_table, freq);
		cpufreq_cdev->freq_table[i].frequency = freq;

		/* Warn for duplicate entries */
		if (!freq)
			pr_warn("%s: table has duplicate entries\n", __func__);
		else
			pr_debug("%s: freq:%u KHz\n", __func__, freq);
	}

	if (capacitance) {
		cpufreq_cdev->plat_get_static_power = plat_static_func;

		ret = update_freq_table(cpufreq_cdev, capacitance);
		if (ret) {
			cdev = ERR_PTR(ret);
			goto remove_ida;
		}

		cooling_ops = &cpufreq_power_cooling_ops;
	} else {
		cooling_ops = &cpufreq_cooling_ops;
	}

	cdev = thermal_of_cooling_device_register(np, dev_name, cpufreq_cdev,
						  cooling_ops);
	if (IS_ERR(cdev))
		goto remove_ida;

	cpufreq_cdev->clipped_freq = cpufreq_cdev->freq_table[0].frequency;
	cpufreq_cdev->cdev = cdev;

	mutex_lock(&cooling_list_lock);
	/* Register the notifier for first cpufreq cooling device */
	first = list_empty(&cpufreq_cdev_list);
	list_add(&cpufreq_cdev->node, &cpufreq_cdev_list);
	mutex_unlock(&cooling_list_lock);

	if (first)
		cpufreq_register_notifier(&thermal_cpufreq_notifier_block,
					  CPUFREQ_POLICY_NOTIFIER);

	return cdev; /*lint !e593*/

remove_ida:
	ida_simple_remove(&cpufreq_ida, cpufreq_cdev->id);
free_table:
	kfree(cpufreq_cdev->freq_table);
free_idle_time:
	kfree(cpufreq_cdev->idle_time);
free_cdev:
	kfree(cpufreq_cdev);
	return cdev;
}

/**
 * cpufreq_cooling_register - function to create cpufreq cooling device.
 * @policy: cpufreq policy
 *
 * This interface function registers the cpufreq cooling device with the name
 * "thermal-cpufreq-%x". This api can support multiple instances of cpufreq
 * cooling devices.
 *
 * Return: a valid struct thermal_cooling_device pointer on success,
 * on failure, it returns a corresponding ERR_PTR().
 */
struct thermal_cooling_device *
cpufreq_cooling_register(struct cpufreq_policy *policy)
{
	return __cpufreq_cooling_register(NULL, policy, 0, NULL);
}
EXPORT_SYMBOL_GPL(cpufreq_cooling_register);

/**
 * of_cpufreq_cooling_register - function to create cpufreq cooling device.
 * @np: a valid struct device_node to the cooling device device tree node
 * @policy: cpufreq policy
 *
 * This interface function registers the cpufreq cooling device with the name
 * "thermal-cpufreq-%x". This api can support multiple instances of cpufreq
 * cooling devices. Using this API, the cpufreq cooling device will be
 * linked to the device tree node provided.
 *
 * Return: a valid struct thermal_cooling_device pointer on success,
 * on failure, it returns a corresponding ERR_PTR().
 */
struct thermal_cooling_device *
of_cpufreq_cooling_register(struct device_node *np,
			    struct cpufreq_policy *policy)
{
	if (!np)
		return ERR_PTR(-EINVAL);

	return __cpufreq_cooling_register(np, policy, 0, NULL);
}
EXPORT_SYMBOL_GPL(of_cpufreq_cooling_register);

/**
 * cpufreq_power_cooling_register() - create cpufreq cooling device with power extensions
 * @policy:		cpufreq policy
 * @capacitance:	dynamic power coefficient for these cpus
 * @plat_static_func:	function to calculate the static power consumed by these
 *			cpus (optional)
 *
 * This interface function registers the cpufreq cooling device with
 * the name "thermal-cpufreq-%x".  This api can support multiple
 * instances of cpufreq cooling devices.  Using this function, the
 * cooling device will implement the power extensions by using a
 * simple cpu power model.  The cpus must have registered their OPPs
 * using the OPP library.
 *
 * An optional @plat_static_func may be provided to calculate the
 * static power consumed by these cpus.  If the platform's static
 * power consumption is unknown or negligible, make it NULL.
 *
 * Return: a valid struct thermal_cooling_device pointer on success,
 * on failure, it returns a corresponding ERR_PTR().
 */
struct thermal_cooling_device *
cpufreq_power_cooling_register(struct cpufreq_policy *policy, u32 capacitance,
			       get_static_t plat_static_func)
{
	return __cpufreq_cooling_register(NULL, policy, capacitance,
				plat_static_func);
}
EXPORT_SYMBOL(cpufreq_power_cooling_register);

/**
 * of_cpufreq_power_cooling_register() - create cpufreq cooling device with power extensions
 * @np:	a valid struct device_node to the cooling device device tree node
 * @policy: cpufreq policy
 * @capacitance:	dynamic power coefficient for these cpus
 * @plat_static_func:	function to calculate the static power consumed by these
 *			cpus (optional)
 *
 * This interface function registers the cpufreq cooling device with
 * the name "thermal-cpufreq-%x".  This api can support multiple
 * instances of cpufreq cooling devices.  Using this API, the cpufreq
 * cooling device will be linked to the device tree node provided.
 * Using this function, the cooling device will implement the power
 * extensions by using a simple cpu power model.  The cpus must have
 * registered their OPPs using the OPP library.
 *
 * An optional @plat_static_func may be provided to calculate the
 * static power consumed by these cpus.  If the platform's static
 * power consumption is unknown or negligible, make it NULL.
 *
 * Return: a valid struct thermal_cooling_device pointer on success,
 * on failure, it returns a corresponding ERR_PTR().
 */
struct thermal_cooling_device *
of_cpufreq_power_cooling_register(struct device_node *np,
				  struct cpufreq_policy *policy,
				  u32 capacitance,
				  get_static_t plat_static_func)
{
	if (!np)
		return ERR_PTR(-EINVAL);

	return __cpufreq_cooling_register(np, policy, capacitance,
				plat_static_func);
}
EXPORT_SYMBOL(of_cpufreq_power_cooling_register);

/**
 * cpufreq_cooling_unregister - function to remove cpufreq cooling device.
 * @cdev: thermal cooling device pointer.
 *
 * This interface function unregisters the "thermal-cpufreq-%x" cooling device.
 */
void cpufreq_cooling_unregister(struct thermal_cooling_device *cdev)
{
	struct cpufreq_cooling_device *cpufreq_cdev;
	bool last;

	if (!cdev)
		return;

	cpufreq_cdev = cdev->devdata;

	mutex_lock(&cooling_list_lock);
	list_del(&cpufreq_cdev->node);
	/* Unregister the notifier for the last cpufreq cooling device */
	last = list_empty(&cpufreq_cdev_list);
	mutex_unlock(&cooling_list_lock);

	if (last)
		cpufreq_unregister_notifier(&thermal_cpufreq_notifier_block,
					    CPUFREQ_POLICY_NOTIFIER);

	thermal_cooling_device_unregister(cpufreq_cdev->cdev);
	ida_simple_remove(&cpufreq_ida, cpufreq_cdev->id);
	kfree(cpufreq_cdev->idle_time);
	kfree(cpufreq_cdev->freq_table);
	kfree(cpufreq_cdev);
}
EXPORT_SYMBOL_GPL(cpufreq_cooling_unregister);
