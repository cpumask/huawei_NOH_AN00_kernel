#ifndef DUBAI_MISC_STATS_H
#define DUBAI_MISC_STATS_H

void dubai_misc_stats_init(void);
void dubai_misc_stats_exit(void);
int dubai_log_stats_enable(void __user *argp);
int dubai_get_kworker_info(void __user *argp);
int dubai_get_uevent_info(void __user *argp);
int dubai_get_binder_stats(void __user *argp);
int dubai_binder_stats_enable(void __user *argp);

#endif // DUBAI_MISC_STATS_H
