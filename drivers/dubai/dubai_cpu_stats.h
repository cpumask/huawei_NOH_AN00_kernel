#ifndef DUBAI_CPU_STATS_H
#define DUBAI_CPU_STATS_H

void dubai_proc_cputime_init(void);
void dubai_proc_cputime_exit(void);
int dubai_proc_cputime_enable(void __user *argp);
int dubai_get_task_cpupower_enable(void __user *argp);
int dubai_get_proc_cputime(void __user *argp);
int dubai_set_proc_decompose(const void __user *argp);

#endif // DUBAI_CPU_STATS_H
