#ifndef DUBAI_GPU_STATS_H
#define DUBAI_GPU_STATS_H

#define DUBAI_GPU_FREQ_MAX_SIZE		20

struct gpu_info {
	unsigned long freq;
	unsigned long run_time;
	unsigned long idle_time;
} __packed;

struct gpu_store {
	int num;
	struct gpu_info array[DUBAI_GPU_FREQ_MAX_SIZE];
} __packed;

int dubai_set_gpu_enable(void __user *argp);
int dubai_get_gpu_info(void __user *argp);

#endif // DUBAI_GPU_STATS_H
