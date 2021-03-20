#include "dubai_gpu_stats.h"

#include <linux/time64.h>

#include "dubai_utils.h"

static struct gpu_store store;
static DEFINE_MUTEX(gpu_lock);
static atomic_t gpu_enable = ATOMIC_INIT(0);

static struct gpu_info *dubai_gpu_lookup_add(unsigned long freq)
{
	int i, num = store.num;

	if (!freq)
		return NULL;

	for (i = 0; i < num; i++) {
		if (freq == store.array[i].freq)
			return &store.array[i];
	}

	if (num < DUBAI_GPU_FREQ_MAX_SIZE - 1) {
		store.array[num].freq = freq;
		store.num++;
		return &store.array[num];
	}

	return NULL;
}

void dubai_update_gpu_info(unsigned long freq, unsigned long busy_time,
	unsigned long total_time, unsigned long cycle_ms)
{
	struct gpu_info *info = NULL;
	unsigned long temp;

	if (!atomic_read(&gpu_enable) || !freq || !total_time)
		return;

	if (!mutex_trylock(&gpu_lock))
		return;

	info = dubai_gpu_lookup_add(freq);
	if (info) {
		info->freq = freq;
		temp = cycle_ms * USEC_PER_MSEC * busy_time / total_time;
		info->run_time += temp;
		info->idle_time += (cycle_ms * USEC_PER_MSEC - temp);
	} else {
		dubai_err("gpu num exceed %d, abort freq: %ld", DUBAI_GPU_FREQ_MAX_SIZE, freq);
	}
	mutex_unlock(&gpu_lock);
}
EXPORT_SYMBOL(dubai_update_gpu_info);

int dubai_set_gpu_enable(void __user *argp)
{
	int ret;
	bool enable = false;

	ret = get_enable_value(argp, &enable);
	if (!ret)
		atomic_set(&gpu_enable, enable ? 1 : 0);
	dubai_info("gpu stats enable: %d", enable ? 1 : 0);

	return ret;
}

int dubai_get_gpu_info(void __user *argp)
{
	int rc = 0;

	if (!atomic_read(&gpu_enable))
		return -EPERM;

	mutex_lock(&gpu_lock);
	if ((store.num < 0) || (store.num > DUBAI_GPU_FREQ_MAX_SIZE)) {
		dubai_err("invalid gpu store num: %d", store.num);
		rc = -EINVAL;
		goto exit;
	}

	if (!store.num)
		goto exit;

	if (copy_to_user(argp, &store, sizeof(struct gpu_store)))
		rc = -EFAULT;

exit:
	memset(&store, 0, sizeof(struct gpu_store));
	mutex_unlock(&gpu_lock);

	return rc;
}
