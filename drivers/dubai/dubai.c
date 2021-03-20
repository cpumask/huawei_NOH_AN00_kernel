#include <linux/init.h>
#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "buffered_log_sender.h"
#include "dubai_battery_stats.h"
#include "dubai_cpu_stats.h"
#include "dubai_display_stats.h"
#include "dubai_gpu_stats.h"
#include "dubai_sensorhub_stats.h"
#include "dubai_misc_stats.h"
#include "dubai_sr_stats.h"
#include "dubai_utils.h"

#define DUBAI_MAGIC 'k'
#define IOCTL_GPU_ENABLE _IOW(DUBAI_MAGIC, 1, bool)
#define IOCTL_GPU_INFO_GET _IOR(DUBAI_MAGIC, 2, struct gpu_store)
#define IOCTL_PROC_CPUTIME_REQUEST _IOW(DUBAI_MAGIC, 3, long long)
#define IOCTL_PROC_CMDLINE_GET _IOWR(DUBAI_MAGIC, 4, struct proc_cmdline)
#define IOCTL_LOG_STATS_ENABLE _IOW(DUBAI_MAGIC, 5, bool)
#define IOCTL_KWORKER_INFO_REQUEST _IOW(DUBAI_MAGIC, 6, long long)
#define IOCTL_UEVENT_INFO_REQUEST _IOW(DUBAI_MAGIC, 7, long long)
#define IOCTL_BRIGHTNESS_ENABLE _IOW(DUBAI_MAGIC, 8, bool)
#define IOCTL_BRIGHTNESS_GET _IOR(DUBAI_MAGIC, 9, uint32_t)
#define IOCTL_PROC_CPUTIME_ENABLE _IOW(DUBAI_MAGIC, 10, bool)
#define IOCTL_BINDER_STATS_ENABLE _IOW(DUBAI_MAGIC, 11, bool)
#define IOCTL_BINDER_STATS_REQUEST _IOW(DUBAI_MAGIC, 13, long long)
#define IOCTL_TASK_CPUPOWER_ENABLE _IOR(DUBAI_MAGIC, 14, bool)
#define IOCTL_AOD_GET_DURATION _IOR(DUBAI_MAGIC, 15, uint64_t)
#define IOCTL_BATTERY_PROP _IOWR(DUBAI_MAGIC, 16, union dubai_battery_prop_value)
#define IOCTL_WAKEUP_SOURCE_NAME_REQUEST _IOW(DUBAI_MAGIC, 17, long long)
#define IOCTL_PROC_DECOMPOSE_SET _IOW(DUBAI_MAGIC, 18, struct dev_transmit_t)
#define IOCTL_SENSORHUB_TYPE_LIST _IOW(DUBAI_MAGIC, 19, long long)
#define IOCTL_SENSOR_TIME_GET _IOWR(DUBAI_MAGIC, 20, struct sensor_time_transmit)
#define IOCTL_FP_ICON_STATS _IOR(DUBAI_MAGIC, 21, uint32_t)
#define IOCTL_SWING_GET _IOWR(DUBAI_MAGIC, 22, struct swing_data)
#define IOCTL_RSS_GET _IOR(DUBAI_MAGIC, 23, long long)
#define IOCTL_TP_GET _IOR(DUBAI_MAGIC, 24, struct tp_duration)
#define IOCTL_RGB_ENABLE _IOW(DUBAI_MAGIC, 25, bool)
#define IOCTL_RGB_GET _IOR(DUBAI_MAGIC, 26, rgb_data_t)
#define IOCTL_PIDS_GET _IOWR(DUBAI_MAGIC, 27, struct uid_name_to_pid)

static char *version = "000.4.0.1";

static long dubai_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int rc;
	void __user *argp = (void __user *)arg;

	rc = 0;
	switch (cmd) {
	case IOCTL_GPU_ENABLE:
		rc = dubai_set_gpu_enable(argp);
		break;
	case IOCTL_GPU_INFO_GET:
		rc = dubai_get_gpu_info(argp);
		break;
	case IOCTL_PROC_CPUTIME_ENABLE:
		rc = dubai_proc_cputime_enable(argp);
		break;
	case IOCTL_PROC_CPUTIME_REQUEST:
		rc = dubai_get_proc_cputime(argp);
		break;
	case IOCTL_PROC_CMDLINE_GET:
		rc = dubai_get_proc_cmdline(argp);
		break;
	case IOCTL_LOG_STATS_ENABLE:
		rc = dubai_log_stats_enable(argp);
		break;
	case IOCTL_KWORKER_INFO_REQUEST:
		rc = dubai_get_kworker_info(argp);
		break;
	case IOCTL_UEVENT_INFO_REQUEST:
		rc = dubai_get_uevent_info(argp);
		break;
	case IOCTL_BRIGHTNESS_ENABLE:
		rc = dubai_set_brightness_enable(argp);
		break;
	case IOCTL_BRIGHTNESS_GET:
		rc = dubai_get_brightness_info(argp);
		break;
	case IOCTL_BINDER_STATS_ENABLE:
		rc = dubai_binder_stats_enable(argp);
		break;
	case IOCTL_BINDER_STATS_REQUEST:
		rc = dubai_get_binder_stats(argp);
		break;
	case IOCTL_TASK_CPUPOWER_ENABLE:
		rc = dubai_get_task_cpupower_enable(argp);
		break;
	case IOCTL_AOD_GET_DURATION:
		rc = dubai_get_aod_duration(argp);
		break;
	case IOCTL_WAKEUP_SOURCE_NAME_REQUEST:
		rc = dubai_get_ws_lasting_name(argp);
		break;
	case IOCTL_PROC_DECOMPOSE_SET:
		rc = dubai_set_proc_decompose(argp);
		break;
	case IOCTL_BATTERY_PROP:
		rc = dubai_get_battery_prop(argp);
		break;
	case IOCTL_SENSORHUB_TYPE_LIST:
		rc = dubai_get_sensorhub_type_list(argp);
		break;
	case IOCTL_SENSOR_TIME_GET:
		rc = dubai_get_all_sensor_stats(argp);
		break;
	case IOCTL_FP_ICON_STATS:
		rc = dubai_get_fp_icon_stats(argp);
		break;
	case IOCTL_SWING_GET:
		rc = dubai_get_swing_data(argp);
		break;
	case IOCTL_RSS_GET:
		rc = dubai_get_rss(argp);
		break;
	case IOCTL_TP_GET:
		rc = dubai_get_tp_duration(argp);
		break;
	case IOCTL_RGB_ENABLE:
		rc = dubai_set_rgb_enable(argp);
		break;
	case IOCTL_RGB_GET:
		rc = dubai_get_rgb_info(argp);
		break;
	case IOCTL_PIDS_GET:
		rc = dubai_get_pids_by_uid_name(argp);
		break;
	default:
		rc = -EINVAL;
		break;
	}

	return rc;
}

#ifdef CONFIG_COMPAT
static long dubai_compat_ioctl(struct file *filp,
			unsigned int cmd, unsigned long arg)
{
	return dubai_ioctl(filp, cmd, (unsigned long) compat_ptr(arg));
}
#endif

static int dubai_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int dubai_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static const struct file_operations dubai_device_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = dubai_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= dubai_compat_ioctl,
#endif
	.open = dubai_open,
	.release = dubai_release,
};

static struct miscdevice dubai_device = {
	.name = "dubai",
	.fops = &dubai_device_fops,
	.minor = MISC_DYNAMIC_MINOR,
};

static int __init dubai_init(void)
{
	int ret = 0;

	dubai_display_stats_init();
	dubai_proc_cputime_init();
	dubai_sr_stats_init();
	dubai_misc_stats_init();

	ret = misc_register(&dubai_device);
	if (ret) {
		dubai_err("Failed to register dubai device");
		goto out;
	}
	dubai_info("DUBAI module initialize success: %s", version);

out:
	return ret;
}

static void __exit dubai_exit(void)
{
	dubai_proc_cputime_exit();
	dubai_sr_stats_exit();
	dubai_misc_stats_exit();
	buffered_log_release();
}

late_initcall(dubai_init);
module_exit(dubai_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yu Peng, <pengyu7@huawei.com>");
MODULE_DESCRIPTION("Huawei Device Usage Big-data Analytics Initiative Driver");
