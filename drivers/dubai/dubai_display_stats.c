#include "dubai_display_stats.h"
#include "dubai_utils.h"

#define MAX_BRIGHTNESS			10000

struct dubai_brightness_info {
	atomic_t enable;
	uint64_t last_time;
	uint32_t last_brightness;
	uint64_t sum_time;
	uint64_t sum_brightness_time;
};

static struct dubai_brightness_info dubai_backlight;
static DEFINE_MUTEX(brightness_lock);

int dubai_set_brightness_enable(void __user *argp)
{
	int ret;
	bool enable = false;

	ret = get_enable_value(argp, &enable);
	if (ret == 0) {
		atomic_set(&dubai_backlight.enable, enable ? 1 : 0);
		dubai_info("Dubai brightness enable: %d", enable ? 1 : 0);
	}

	return ret;
}

static int dubai_set_brightness_locked(uint32_t brightness)
{
	uint64_t current_time;
	uint64_t diff_time;

	if (brightness > MAX_BRIGHTNESS) {
		dubai_err("Need valid data! brightness= %d", brightness);
		return -EINVAL;
	}

	current_time = div_u64(ktime_get_ns(), NSEC_PER_MSEC);

	if (dubai_backlight.last_brightness > 0) {
		diff_time = current_time - dubai_backlight.last_time;
		dubai_backlight.sum_time += diff_time;
		dubai_backlight.sum_brightness_time += dubai_backlight.last_brightness * diff_time;
	}

	dubai_backlight.last_time = current_time;
	dubai_backlight.last_brightness = brightness;

	return 0;
}

void dubai_update_brightness(uint32_t brightness)
{
	int ret;

	if (!atomic_read(&dubai_backlight.enable))
		return;

	mutex_lock(&brightness_lock);
	ret = dubai_set_brightness_locked(brightness);
	if (ret < 0)
		dubai_err("Update brightness= %d error!", brightness);
	mutex_unlock(&brightness_lock);
}
EXPORT_SYMBOL(dubai_update_brightness);

int dubai_get_brightness_info(void __user *argp)
{
	int ret;
	uint32_t brightness;

	mutex_lock(&brightness_lock);
	ret = dubai_set_brightness_locked(dubai_backlight.last_brightness);
	if (ret == 0) {
		if (dubai_backlight.sum_time > 0 && dubai_backlight.sum_brightness_time > 0)
			brightness = (uint32_t)div64_u64(dubai_backlight.sum_brightness_time, dubai_backlight.sum_time);
		else
			brightness = 0;

		if (copy_to_user(argp, &brightness, sizeof(uint32_t)))
			ret = -EFAULT;
	}

	dubai_backlight.sum_time = 0;
	dubai_backlight.sum_brightness_time = 0;
	mutex_unlock(&brightness_lock);

	return ret;
}

#ifdef CONFIG_HUAWEI_DUBAI_RGB_STATS
int dubai_set_rgb_enable(void __user *argp)
{
	int ret;
	bool enable = false;

	ret = get_enable_value(argp, &enable);
	if ((ret == 0) && (!hisifb_rgb_set_enable(enable)))
		return -EFAULT;

	return ret;
}

int dubai_get_rgb_info(void __user *argp)
{
	int ret;
	rgb_data_t data;

	ret = hisifb_rgb_get_data(&data);
	if (ret < 0) {
		dubai_err("Read rgb data fail!");
		return -EFAULT;
	}

	if (copy_to_user(argp, &data, sizeof(rgb_data_t)))
		return -EFAULT;

	return 0;
}
#else /* !CONFIG_HUAWEI_DUBAI_RGB_STATS */
int dubai_set_rgb_enable(void __user __always_unused *argp)
{
	return -EOPNOTSUPP;
}

int dubai_get_rgb_info(void __user __always_unused *argp)
{
	return -EOPNOTSUPP;
}
#endif /* !CONFIG_HUAWEI_DUBAI_RGB_STATS */

void dubai_display_stats_init(void)
{
	atomic_set(&dubai_backlight.enable, 0);
	dubai_backlight.last_time = 0;
	dubai_backlight.last_brightness = 0;
	dubai_backlight.sum_time = 0;
	dubai_backlight.sum_brightness_time = 0;
}
