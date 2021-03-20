#include "dubai_cpu_stats.h"

#include <linux/hashtable.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/net.h>
#include <linux/profile.h>
#include <linux/slab.h>
#include <linux/version.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
#include <linux/sched.h>
#include <linux/sched/cputime.h>
#include <linux/sched/signal.h>
#endif

#include "buffered_log_sender.h"
#include "dubai_utils.h"

#define PROC_HASH_BITS			16
#define BASE_COUNT				1500
#define KERNEL_TGID				0
#define KERNEL_NAME				"kernel"
#define SYSTEM_SERVER			"system_server"
#define DUBAID_NAME				"dubaid"
#define DECOMPOSE_COUNT_MAX		10
#define DECOMPOSE_RETYR_MAX		30
#define PREFIX_LEN				32
#define NAME_LEN				(PREFIX_LEN + TASK_COMM_LEN)
#define RCU_LOCK_BREAK_TIMEOUT	(HZ / 10)

#define dubai_check_entry_state(entry, expected)	((entry)->state == (expected))
#define dubai_check_update_entry_state(entry, expected, updated) \
	({ \
		bool ret = dubai_check_entry_state(entry, expected); \
		if (!ret) \
			(entry)->state = (updated); \
		ret; \
	})

enum {
	CMDLINE_NEED_TO_GET = 0,
	CMDLINE_PROCESS,
	CMDLINE_THREAD,
};

enum {
	PROCESS_STATE_DEAD = 0,
	PROCESS_STATE_ACTIVE,
	PROCESS_STATE_INVALID,
};

enum {
	TASK_STATE_RUNNING = 0,
	TASK_STATE_SLEEPING,
	TASK_STATE_DISK_SLEEP,
	TASK_STATE_STOPPED,
	TASK_STATE_TRACING_STOP,
	TASK_STATE_DEAD,
	TASK_STATE_ZOMBIE,
	TASK_STATE_PARKED,
	TASK_STATE_IDLE,
};

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
// keep sync with fs/proc/array.c
static const int task_state_array[] = {
	TASK_STATE_RUNNING,
	TASK_STATE_SLEEPING,
	TASK_STATE_DISK_SLEEP,
	TASK_STATE_STOPPED,
	TASK_STATE_TRACING_STOP,
	TASK_STATE_DEAD,
	TASK_STATE_ZOMBIE,
	TASK_STATE_PARKED,
	TASK_STATE_IDLE,
};
#else
static const int task_state_array[] = {
	TASK_STATE_RUNNING,
	TASK_STATE_SLEEPING,
	TASK_STATE_DISK_SLEEP,
	TASK_STATE_STOPPED,
	TASK_STATE_TRACING_STOP,
	TASK_STATE_DEAD,
	TASK_STATE_ZOMBIE,
};
#endif

enum {
	DUBAI_STATE_DEAD,
	DUBAI_STATE_ALIVE,
	DUBAI_STATE_UNSETTLED,
};

struct dubai_cputime {
	uid_t uid;
	pid_t pid;
	unsigned long long time;
	unsigned long long power;
	unsigned char cmdline;
	char name[NAME_LEN];
} __packed;

struct dubai_thread_entry {
	pid_t pid;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
	u64 utime;
	u64 stime;
#else
	cputime_t utime;
	cputime_t stime;
#endif
#ifdef CONFIG_HUAWEI_DUBAI_TASK_CPU_POWER
	unsigned long long power;
#endif
	int8_t state;
	char name[NAME_LEN];
	struct list_head node;
};

struct dubai_proc_entry {
	pid_t tgid;
	uid_t uid;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
	u64 utime;
	u64 stime;
	u64 active_utime;
	u64 active_stime;
#else
	cputime_t utime;
	cputime_t stime;
	cputime_t active_utime;
	cputime_t active_stime;
#endif
#ifdef CONFIG_HUAWEI_DUBAI_TASK_CPU_POWER
	unsigned long long active_power;
	unsigned long long power;
#endif
	int8_t state;
	bool cmdline;
	char name[NAME_LEN];
	struct list_head threads;
	struct hlist_node hash;
};

struct dubai_cputime_transmit {
	long long timestamp;
	int type;
	int count;
	unsigned char value[0];
} __packed;

struct dubai_decompose_info {
	uid_t uid;
	char comm[TASK_COMM_LEN];
	char prefix[PREFIX_LEN];
} __packed;

struct dubai_proc_decompose {
	pid_t tgid;
	struct dubai_decompose_info decompose;
	struct list_head node;
};

static DECLARE_HASHTABLE(proc_hash_table, PROC_HASH_BITS);
static DEFINE_MUTEX(dubai_proc_lock);
static atomic_t proc_cputime_enable;
static atomic_t dead_count;
static atomic_t active_count;

static void dubai_decompose_work_fn(struct work_struct *work);
static DECLARE_DELAYED_WORK(dubai_decompose_work, dubai_decompose_work_fn);
static LIST_HEAD(dubai_proc_decompose_list);
static const int decompose_check_delay = HZ;
static atomic_t decompose_count_target;
static atomic_t decompose_count;
static atomic_t decompose_check_retry;
static int decompose_found_cnt;
static bool update_active_succ = true;

#ifdef CONFIG_HUAWEI_DUBAI_TASK_CPU_POWER
static const atomic_t task_power_enable = ATOMIC_INIT(1);
#else
static const atomic_t task_power_enable = ATOMIC_INIT(0);
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
static inline unsigned long long dubai_cputime_to_usecs(u64 time)
{
	return ((unsigned long long)ktime_to_ms(time) * USEC_PER_MSEC);
}
#else
static inline unsigned long long dubai_cputime_to_usecs(cputime_t time)
{
	return ((unsigned long long)
		jiffies_to_msecs(cputime_to_jiffies(time)) * USEC_PER_MSEC);
}
#endif

#ifndef CONFIG_HUAWEI_DUBAI_TASK_CPU_POWER
// keep sync with fs/proc/array.c
static int dubai_get_task_state(struct task_struct *task)
{
	unsigned int state;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
	BUILD_BUG_ON(1 + ilog2(TASK_REPORT_MAX) != ARRAY_SIZE(task_state_array));
	state = __get_task_state(task);
#else
	state = (task->state | task->exit_state) & TASK_REPORT;

	/*
	 * Parked tasks do not run; they sit in __kthread_parkme().
	 * Without this check, we would report them as running, which is
	 * clearly wrong, so we report them as sleeping instead.
	 */
	if (task->state == TASK_PARKED)
		state = TASK_INTERRUPTIBLE;

	BUILD_BUG_ON(1 + ilog2(TASK_REPORT)
			!= ARRAY_SIZE(task_state_array) - 1);
#endif

	return task_state_array[fls(state)];
}
#endif

static bool dubai_task_alive(struct task_struct *task)
{
#ifdef CONFIG_HUAWEI_DUBAI_TASK_CPU_POWER
	/*
	 * if this task is exiting, we have already accounted for the
	 * time and power.
	 */
	if (unlikely(task == NULL) || (task->cpu_power == ULLONG_MAX))
		return false;
#else
	int state;

	if (unlikely(task == NULL)
		|| (task->flags & PF_EXITING)
		|| (task->flags & PF_EXITPIDONE)
		|| (task->flags & PF_SIGNALED))
		return false;

	state = dubai_get_task_state(task);
	if (state == TASK_STATE_DEAD || state == TASK_STATE_ZOMBIE)
		return false;
#endif

	return true;
}

static void dubai_copy_name(char *to, const char *from, size_t len)
{
	char *p = NULL;

	if (strlen(from) <= len) {
		strncpy(to, from, len);
		return;
	}

	p = strrchr(from, '/');
	if (p != NULL && (*(p + 1) != '\0'))
		strncpy(to, p + 1, len);
	else
		strncpy(to, from, len);
}

static bool dubai_check_proc_compose_count(void)
{
	int count = atomic_read(&decompose_count);

	return (count > 0 && count == atomic_read(&decompose_count_target));
}

static void dubai_schedule_decompose_work(void)
{
	if (atomic_read(&decompose_check_retry) < DECOMPOSE_RETYR_MAX) {
		atomic_inc(&decompose_check_retry);
		schedule_delayed_work(&dubai_decompose_work, decompose_check_delay);
	}
}

static struct dubai_proc_decompose *dubai_find_decompose_entry(pid_t tgid)
{
	struct dubai_proc_decompose *entry = NULL;

	list_for_each_entry(entry, &dubai_proc_decompose_list, node) {
		if (entry->tgid == tgid)
			return entry;
	}

	return NULL;
}

static void dubai_remove_proc_decompose(pid_t tgid)
{
	struct dubai_proc_decompose *entry = NULL;

	entry = dubai_find_decompose_entry(tgid);
	if (entry != NULL) {
		dubai_info("Remove decompose proc: %d, %s", tgid, entry->decompose.prefix);
		entry->tgid = -1;
		atomic_dec(&decompose_count);
		decompose_found_cnt--;
		dubai_schedule_decompose_work();
	}
}

static pid_t dubai_find_tgid(uid_t t_uid, const char *t_comm)
{
	pid_t tgid = -1;
	uid_t uid;
	struct task_struct *task = NULL;

	rcu_read_lock();
	for_each_process(task) {
		if ((task->flags & PF_KTHREAD) || !dubai_task_alive(task))
			continue;

		uid = from_kuid_munged(current_user_ns(), task_uid(task));
		if (uid == t_uid && !strncmp(task->comm, t_comm, TASK_COMM_LEN - 1)) {
			dubai_info("Get [%s] tgid: %d", t_comm, task->tgid);
			tgid = task->tgid;
			break;
		}
	}
	rcu_read_unlock();

	return tgid;
}

static void dubai_check_proc_decompose(void)
{
	pid_t tgid;
	struct dubai_proc_decompose *entry = NULL;

	if (dubai_check_proc_compose_count())
		return;

	list_for_each_entry(entry, &dubai_proc_decompose_list, node) {
		if (entry->tgid >= 0)
			continue;

		if (!strncmp(entry->decompose.comm, KERNEL_NAME, TASK_COMM_LEN - 1)) {
			tgid = KERNEL_TGID;
		} else {
			tgid = dubai_find_tgid(entry->decompose.uid, entry->decompose.comm);
		}
		if (tgid >= 0) {
			entry->tgid = tgid;
			atomic_inc(&decompose_count);
			dubai_info("Add decompose proc: %d, %s", tgid, entry->decompose.prefix);
		}
	}
}

static void dubai_decompose_work_fn(struct work_struct *work)
{
	mutex_lock(&dubai_proc_lock);
	dubai_check_proc_decompose();

	if (!dubai_check_proc_compose_count()) {
		dubai_schedule_decompose_work();
	} else {
		atomic_set(&decompose_check_retry, 0);
		dubai_info("Check process to decompose successfully, count: %d", atomic_read(&decompose_count));
	}
	mutex_unlock(&dubai_proc_lock);
}

static void dubai_clear_proc_decompose(void)
{
	struct dubai_proc_decompose *entry = NULL, *tmp = NULL;

	list_for_each_entry_safe(entry, tmp, &dubai_proc_decompose_list, node) {
		list_del_init(&entry->node);
		kfree(entry);
	}
	atomic_set(&decompose_count_target, 0);
	atomic_set(&decompose_count, 0);
	atomic_set(&decompose_check_retry, 0);
	decompose_found_cnt = 0;
}

static int dubai_set_proc_decompose_list(int count, const struct dubai_decompose_info *data)
{
	int ret = 0, i;
	struct dubai_proc_decompose *entry = NULL;

	mutex_lock(&dubai_proc_lock);
	dubai_clear_proc_decompose();

	for (i = 0; i < count; i++) {
		entry = kzalloc(sizeof(struct dubai_proc_decompose), GFP_ATOMIC);
		if (entry == NULL) {
			ret = -ENOMEM;
			goto exit;
		}
		memcpy(&entry->decompose, data, sizeof(struct dubai_decompose_info));
		entry->decompose.comm[TASK_COMM_LEN - 1] = '\0';
		entry->decompose.prefix[PREFIX_LEN - 1] = '\0';
		entry->tgid = -1;
		list_add_tail(&entry->node, &dubai_proc_decompose_list);
		atomic_inc(&decompose_count_target);
		data++;
	}

exit:
	if (atomic_read(&decompose_count_target) > 0)
		dubai_schedule_decompose_work();
	mutex_unlock(&dubai_proc_lock);

	return ret;
}

int dubai_set_proc_decompose(const void __user *argp)
{
	int ret = 0, size, count, length, remain;
	struct dev_transmit_t *transmit = NULL;
	const char *comm = current->comm;

	if (current->group_leader)
		comm = current->group_leader->comm;

	if (!strstr(comm, DUBAID_NAME))
		return -EPERM;

	if (copy_from_user(&length, argp, sizeof(int)))
		return -EFAULT;

	count = length / (int)(sizeof(struct dubai_decompose_info));
	remain = length % (int)(sizeof(struct dubai_decompose_info));
	if ((count <= 0) || (count > DECOMPOSE_COUNT_MAX) || (remain != 0)) {
		dubai_err("Invalid length, length: %d, count: %d, remain: %d", length, count, remain);
		return -EINVAL;
	}

	size = length + sizeof(struct dev_transmit_t);
	transmit = kzalloc(size, GFP_KERNEL);
	if (transmit == NULL)
		return -ENOMEM;

	if (copy_from_user(transmit, argp, size)) {
		ret = -EFAULT;
		goto exit;
	}

	ret = dubai_set_proc_decompose_list(count, (const struct dubai_decompose_info *)transmit->data);
	if (ret < 0)
		dubai_err("Failed to set process decompose list");

exit:
	kfree(transmit);

	return ret;
}

// Create log entry to store process cputime structures
static struct buffered_log_entry *dubai_create_log_entry(long long timestamp, int count, int type)
{
	long long size = 0;
	struct buffered_log_entry *entry = NULL;
	struct dubai_cputime_transmit *transmit = NULL;

	if (unlikely(count < 0)) {
		dubai_err("Invalid parameter, count=%d", count);
		return NULL;
	}
	/*
	 * allocate more space(BASE_COUNT)
	 * size = dubai_cputime_transmit size + dubai_cputime size * count
	 */
	count += BASE_COUNT;
	size = cal_log_entry_len(struct dubai_cputime_transmit, struct dubai_cputime, count);
	entry = create_buffered_log_entry(size, BUFFERED_LOG_MAGIC_PROC_CPUTIME);
	if (entry == NULL) {
		dubai_err("Failed to create buffered log entry");
		return NULL;
	}

	transmit = (struct dubai_cputime_transmit *)(entry->data);
	transmit->timestamp = timestamp;
	transmit->type = type;
	transmit->count = count;

	return entry;
}

void dubai_send_log_entry(struct buffered_log_entry *entry)
{
	int ret;
	struct dubai_cputime_transmit *transmit = NULL;

	transmit = (struct dubai_cputime_transmit *)(entry->data);
	entry->length = cal_log_entry_len(struct dubai_cputime_transmit, struct dubai_cputime, transmit->count);
	ret = send_buffered_log(entry);
	if (ret < 0)
		dubai_err("Failed to send process log entry, type: %d", transmit->type);
}

static void dubai_proc_entry_copy(unsigned char *value, const struct dubai_proc_entry *entry)
{
	struct dubai_cputime stat;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
	u64 total_time;
#else
	cputime_t total_time;
#endif

	total_time = entry->active_utime + entry->active_stime;
	total_time += entry->utime + entry->stime;

	memset(&stat, 0, sizeof(struct dubai_cputime));
	stat.uid = entry->uid;
	stat.pid = entry->tgid;
	stat.time = dubai_cputime_to_usecs(total_time);
#ifdef CONFIG_HUAWEI_DUBAI_TASK_CPU_POWER
	stat.power = entry->active_power + entry->power;
#endif
	stat.cmdline = entry->cmdline? CMDLINE_PROCESS : CMDLINE_NEED_TO_GET;
	dubai_copy_name(stat.name, entry->name, NAME_LEN - 1);

	memcpy(value, &stat, sizeof(struct dubai_cputime));
}

static void dubai_thread_entry_copy(unsigned char *value, uid_t uid, const struct dubai_thread_entry *thread)
{
	struct dubai_cputime stat;

	memset(&stat, 0, sizeof(struct dubai_cputime));
	stat.uid = uid;
	stat.pid = thread->pid;
	stat.time = dubai_cputime_to_usecs(thread->utime + thread->stime);
#ifdef CONFIG_HUAWEI_DUBAI_TASK_CPU_POWER
	stat.power = thread->power;
#endif
	stat.cmdline = CMDLINE_THREAD;
	dubai_copy_name(stat.name, thread->name, NAME_LEN - 1);

	memcpy(value, &stat, sizeof(struct dubai_cputime));
}

static bool dubai_proc_entry_decomposed(struct dubai_proc_entry *entry)
{
	if (!list_empty(&entry->threads))
		return true;
	if ((decompose_found_cnt == atomic_read(&decompose_count)) ||
		!dubai_find_decompose_entry(entry->tgid))
		return false;

	dubai_info("Found decompsed entry: %d, %s", entry->tgid, entry->name);
	decompose_found_cnt++;
	return true;
}

static struct dubai_thread_entry *dubai_find_or_register_thread(pid_t pid,
		const char *comm, struct dubai_proc_entry *entry)
{
	struct dubai_thread_entry *thread = NULL;
	struct dubai_proc_decompose *d_proc = NULL;

	d_proc = dubai_find_decompose_entry(entry->tgid);
	if (d_proc == NULL && list_empty(&entry->threads))
		return NULL;

	list_for_each_entry(thread, &entry->threads, node) {
		if (thread->pid == pid)
			goto update_thread_name;
	}

	thread = kzalloc(sizeof(struct dubai_thread_entry), GFP_ATOMIC);
	if (thread == NULL) {
		dubai_err("Failed to allocate memory");
		return NULL;
	}
	thread->pid = pid;
	thread->state = DUBAI_STATE_ALIVE;
	list_add_tail(&thread->node, &entry->threads);
	atomic_inc(&active_count);

update_thread_name:
	if (d_proc != NULL)
		snprintf(thread->name, NAME_LEN - 1, "%s_%s", d_proc->decompose.prefix, comm);
	else
		snprintf(thread->name, NAME_LEN - 1, "%s_%s", entry->name, comm);

	return thread;
}

static struct dubai_proc_entry *dubai_find_proc_entry(pid_t tgid)
{
	struct dubai_proc_entry *entry = NULL;

	hash_for_each_possible(proc_hash_table, entry, hash, tgid) {
		if (entry->tgid == tgid) {
			return entry;
		}
	}

	return NULL;
}

// combine kernel thread to same entry which tgid is 0
static inline int dubai_get_task_normalized_tgid(struct task_struct *task)
{
	return ((task->flags & PF_KTHREAD) ? KERNEL_TGID : task->tgid);
}

static struct dubai_proc_entry *dubai_find_or_register_proc(pid_t tgid, const struct task_struct *task)
{
	const char *comm = NULL;
	struct dubai_proc_entry *entry = NULL;
	struct task_struct *leader = task->group_leader;

	if (task->flags & PF_KTHREAD)
		comm = KERNEL_NAME;
	else
		comm = leader ? leader->comm : task->comm;

	entry = dubai_find_proc_entry(tgid);
	if (entry != NULL)
		goto update_uid_name;

	entry = kzalloc(sizeof(struct dubai_proc_entry), GFP_ATOMIC);
	if (entry == NULL) {
		dubai_err("Failed to allocate memory");
		return NULL;
	}
	entry->tgid = tgid;
	entry->state = DUBAI_STATE_ALIVE;
	entry->cmdline = (tgid == KERNEL_TGID);
	INIT_LIST_HEAD(&entry->threads);
	hash_add(proc_hash_table, &entry->hash, tgid);
	atomic_inc(&active_count);

update_uid_name:
	entry->uid = from_kuid_munged(current_user_ns(), task_uid(task));
	if (likely(!entry->cmdline) || (strlen(entry->name) == 0))
		strncpy(entry->name, comm, NAME_LEN - 1);

	return entry;
}

/*
 * To avoid extending the RCU grace period for an unbounded amount of time,
 * periodically exit the critical section and enter a new one.
 * For preemptible RCU it is sufficient to call rcu_read_unlock in order
 * to exit the grace period. For classic RCU, a reschedule is required.
 */
static bool dubai_rcu_lock_break(struct task_struct *g, struct task_struct *t)
{
	bool can_cont = true;

	get_task_struct(g);
	get_task_struct(t);
	rcu_read_unlock();
	cond_resched();
	rcu_read_lock();
	can_cont = pid_alive(g) && pid_alive(t);
	put_task_struct(t);
	put_task_struct(g);

	return can_cont;
}

static int dubai_update_proc_cputime(void)
{
	struct dubai_proc_entry *entry = NULL;
	struct dubai_thread_entry *thread = NULL;
	struct task_struct *task = NULL, *temp = NULL;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
	u64 utime;
	u64 stime;
#else
	cputime_t utime;
	cputime_t stime;
#endif
	pid_t tgid;
	unsigned long last_break;
	int ret = 0, proc_cnt = 0;
	u64 cal_time;

	update_active_succ = true;
	rcu_read_lock();
	cal_time = ktime_get_ns();
	last_break = jiffies;
	// update active time from alive task
	do_each_thread(temp, task) {
		/*
		 * check whether task is alive or not
		 * if not, do not record this task's cpu time
		 */
		if (!dubai_task_alive(task))
			continue;

		tgid = dubai_get_task_normalized_tgid(task);
		if (!entry || (entry->tgid != tgid)) {
			proc_cnt++;
			if (((proc_cnt % 10) == 0) &&
				time_is_before_jiffies(last_break + RCU_LOCK_BREAK_TIMEOUT)) {
				dubai_info("Timeout with rcu_read_lock");
				if (!dubai_rcu_lock_break(temp, task)) {
					dubai_err("Cannot continue to update cpu stats ");
					update_active_succ = false;
					goto unlock;
				}
				last_break = jiffies;
			}
			entry = dubai_find_or_register_proc(tgid, task);
		}
		if (entry == NULL) {
			dubai_err("Failed to find the entry for process %d", tgid);
			ret = -ENOMEM;
			goto unlock;
		}
		task_cputime_adjusted(task, &utime, &stime);
		if (!dubai_check_entry_state(entry, DUBAI_STATE_DEAD)) {
			entry->state = DUBAI_STATE_ALIVE;
			entry->active_utime += utime;
			entry->active_stime += stime;
#ifdef CONFIG_HUAWEI_DUBAI_TASK_CPU_POWER
			entry->active_power += task->cpu_power;
#endif
		}
		if (!dubai_proc_entry_decomposed(entry))
			continue;

		thread = dubai_find_or_register_thread(task->pid, task->comm, entry);
		if ((thread != NULL) && !dubai_check_entry_state(thread, DUBAI_STATE_DEAD)) {
			thread->state = DUBAI_STATE_ALIVE;
			thread->utime = utime;
			thread->stime = stime;
#ifdef CONFIG_HUAWEI_DUBAI_TASK_CPU_POWER
			thread->power = task->cpu_power;
#endif
		}
	} while_each_thread(temp, task);
unlock:
	dubai_info("Updating cpu stats with rcu_read_lock takes %lldms", (ktime_get_ns() - cal_time) / NSEC_PER_MSEC);
	rcu_read_unlock();

	return ret;
}

/*
 * initialize hash list
 * report dead process and delete hash node
 */
static void dubai_clear_dead_entry(struct buffered_log_entry *log_entry)
{
	int max;
	bool decompose = false;
	unsigned char *value = NULL;
	unsigned long bkt;
	struct dubai_proc_entry *entry = NULL;
	struct hlist_node *tmp = NULL;
	struct dubai_thread_entry *thread = NULL, *temp = NULL;
	struct dubai_cputime_transmit *transmit = (struct dubai_cputime_transmit *)(log_entry->data);

	value = transmit->value;
	max = transmit->count;
	transmit->count = 0;

	hash_for_each_safe(proc_hash_table, bkt, tmp, entry, hash) {
		decompose = false;
		entry->active_stime = 0;
		entry->active_utime = 0;
#ifdef CONFIG_HUAWEI_DUBAI_TASK_CPU_POWER
		entry->active_power = 0;
#endif

		if (!list_empty(&entry->threads)) {
			decompose= true;
			list_for_each_entry_safe(thread, temp, &entry->threads, node) {
				if (!dubai_check_update_entry_state(thread, DUBAI_STATE_DEAD, DUBAI_STATE_UNSETTLED))
					continue;

				if (transmit->count < max
					&& (thread->utime + thread->stime) > 0) {
					dubai_thread_entry_copy(value, entry->uid, thread);
					value += sizeof(struct dubai_cputime);
					transmit->count++;
				}
				list_del_init(&thread->node);
				kfree(thread);
			}
		}

		if (!dubai_check_update_entry_state(entry, DUBAI_STATE_DEAD, DUBAI_STATE_UNSETTLED))
			continue;

		if (!decompose
			&& transmit->count < max
			&& (entry->utime + entry->stime) > 0) {
			dubai_proc_entry_copy(value, entry);
			value += sizeof(struct dubai_cputime);
			transmit->count++;
		}
		if (list_empty(&entry->threads)) {
			hash_del(&entry->hash);
			kfree(entry);
		}
	}
}

static void dubai_get_active_process(struct buffered_log_entry *log_entry)
{
	int max_count;
	unsigned long bkt;
	struct dubai_proc_entry *entry = NULL;
	struct dubai_thread_entry *thread = NULL;
	struct dubai_cputime_transmit *transmit = (struct dubai_cputime_transmit *)(log_entry->data);
	unsigned char *value = transmit->value;

	max_count = transmit->count;
	transmit->count = 0;
	hash_for_each(proc_hash_table, bkt, entry, hash) {
		if (unlikely(dubai_check_entry_state(entry, DUBAI_STATE_UNSETTLED)))
			dubai_err("Proc stay unsettled: uid=%u, tgid=%d, name=%s",
				entry->uid, entry->tgid, entry->name);

		if ((update_active_succ &&
			!dubai_check_update_entry_state(entry, DUBAI_STATE_ALIVE, DUBAI_STATE_DEAD)) ||
			((entry->active_utime + entry->active_stime) == 0))
			continue;

		if (transmit->count >= max_count)
			break;

		if (list_empty(&entry->threads)) {
			dubai_proc_entry_copy(value, entry);
			value += sizeof(struct dubai_cputime);
			transmit->count++;
			continue;
		}

		list_for_each_entry(thread, &entry->threads, node) {
			if (unlikely(dubai_check_entry_state(thread, DUBAI_STATE_UNSETTLED)))
				dubai_err("Thread stay unsettled: uid=%u, tgid=%d, pid=%d, name=%s",
					entry->uid, entry->tgid, thread->pid, thread->name);

			if ((update_active_succ &&
				!dubai_check_update_entry_state(thread, DUBAI_STATE_ALIVE, DUBAI_STATE_DEAD)) ||
				((thread->utime + thread->stime) == 0))
				continue;

			if (transmit->count >= max_count)
				break;

			dubai_thread_entry_copy(value, entry->uid, thread);
			value += sizeof(struct dubai_cputime);
			transmit->count++;
		}
	}
}

static int dubai_clear_and_update(long long timestamp)
{
	int ret;
	struct buffered_log_entry *dead_entry = NULL;
	struct buffered_log_entry *active_entry = NULL;
	int dead_cnt = atomic_read(&dead_count);
	int total_cnt = dead_cnt + atomic_read(&active_count);

	dead_entry = dubai_create_log_entry(timestamp, dead_cnt, PROCESS_STATE_DEAD);
	if (dead_entry == NULL) {
		dubai_err("Failed to create log entry for dead processes");
		return -ENOMEM;
	}
	// use total_cnt for active process
	active_entry = dubai_create_log_entry(timestamp, total_cnt, PROCESS_STATE_ACTIVE);
	if (active_entry == NULL) {
		dubai_err("Failed to create log entry for active processes");
		free_buffered_log_entry(dead_entry);
		return -ENOMEM;
	}

	dubai_info("get cpu stats enter");
	mutex_lock(&dubai_proc_lock);
	dubai_clear_dead_entry(dead_entry);
	atomic_set(&dead_count, 0);
	dubai_check_proc_decompose();

	ret = dubai_update_proc_cputime();
	if (ret < 0) {
		dubai_err("Failed to update process cpu time");
		mutex_unlock(&dubai_proc_lock);
		goto exit;
	}
	dubai_get_active_process(active_entry);
	mutex_unlock(&dubai_proc_lock);
	dubai_send_log_entry(dead_entry);
	dubai_send_log_entry(active_entry);

exit:
	free_buffered_log_entry(dead_entry);
	free_buffered_log_entry(active_entry);
	dubai_info("get cpu stats exit, active count: %d", atomic_read(&active_count));

	return ret;
}

/*
 * if there are dead processes in the list,
 * we should clear these dead processes
 * in case of pid reused
 */
int dubai_get_proc_cputime(void __user *argp)
{
	int ret = 0;
	long long timestamp;

	if (!atomic_read(&proc_cputime_enable))
		return -EPERM;

	ret = get_timestamp_value(argp, &timestamp);
	if (ret < 0) {
		dubai_err("Failed to get timestamp");
		goto exit;
	}
	dubai_info("get prco cputime: %lld", timestamp);

	ret = dubai_clear_and_update(timestamp);
	if (ret < 0) {
		dubai_err("Failed to clear dead process and update list");
		goto exit;
	}

exit:
	return ret;
}

static bool is_same_process(const struct dubai_proc_entry *entry, struct task_struct *task)
{
	uid_t uid;

	uid = from_kuid_munged(current_user_ns(), task_uid(task));
	// task with same pid has already died before OR it's not in same uid
	if (entry->tgid == task->pid || uid != entry->uid)
		return false;

	if (dubai_task_alive(task->group_leader)
		&& (strstr(entry->name, task->comm) == NULL)
		&& (strstr(entry->name, task->group_leader->comm) == NULL))
		return false;

	return true;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
static void dubai_update_proc_dead(struct dubai_proc_entry *entry, struct task_struct *task,
	u64 utime, u64 stime)
#else
static void dubai_update_proc_dead(struct dubai_proc_entry *entry, struct task_struct *task,
	cputime_t utime, cputime_t stime)
#endif
{
	if (dubai_check_entry_state(entry, DUBAI_STATE_DEAD) && !is_same_process(entry, task))
		return;

	entry->utime += utime;
	entry->stime += stime;
#ifdef CONFIG_HUAWEI_DUBAI_TASK_CPU_POWER
	if (task->cpu_power != ULLONG_MAX)
		entry->power += task->cpu_power;
#endif

	// process has died
	if ((entry->tgid == task->pid) && !dubai_check_entry_state(entry, DUBAI_STATE_DEAD)) {
		entry->state = DUBAI_STATE_DEAD;
		atomic_dec(&active_count);
		atomic_inc(&dead_count);
		if (!list_empty(&entry->threads))
			dubai_remove_proc_decompose(entry->tgid);
	}
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
static void dubai_update_thread_dead(struct dubai_thread_entry *thread, struct task_struct *task,
	u64 utime, u64 stime)
#else
static void dubai_update_thread_dead(struct dubai_thread_entry *thread, struct task_struct *task,
	cputime_t utime, cputime_t stime)
#endif
{
	if ((thread == NULL) || dubai_check_entry_state(thread, DUBAI_STATE_DEAD))
		return;

	thread->utime = utime;
	thread->stime = stime;
#ifdef CONFIG_HUAWEI_DUBAI_TASK_CPU_POWER
	if (task->cpu_power != ULLONG_MAX)
		thread->power = task->cpu_power;
#endif
	thread->state = DUBAI_STATE_DEAD;
	atomic_dec(&active_count);
	atomic_inc(&dead_count);
}

static int dubai_process_notifier(struct notifier_block __always_unused *self,
	unsigned long __always_unused cmd, void *v)
{
	struct task_struct *task = v;
	pid_t tgid;
	struct dubai_proc_entry *entry = NULL;
	struct dubai_thread_entry *thread = NULL;
	char cmdline[CMDLINE_LEN] = {0};
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
	u64 utime, stime;
#else
	cputime_t utime, stime;
#endif

	if (task == NULL || !atomic_read(&proc_cputime_enable))
		return NOTIFY_OK;

	tgid = dubai_get_task_normalized_tgid(task);
	if (tgid == task->pid) {
		if (dubai_get_cmdline(task, cmdline, CMDLINE_LEN) < 0)
			cmdline[0] = '\0';
	}

	mutex_lock(&dubai_proc_lock);
	if (task->tgid == task->pid || dubai_task_alive(task->group_leader)) {
		entry = dubai_find_or_register_proc(tgid, task);
	} else {
		entry = dubai_find_proc_entry(tgid);
	}
	if (entry == NULL)
		goto exit;

	if (strlen(cmdline) > 0) {
		dubai_copy_name(entry->name, cmdline, NAME_LEN - 1);
		entry->cmdline = true;
	}

	task_cputime_adjusted(task, &utime, &stime);
	dubai_update_proc_dead(entry, task, utime, stime);
	thread = dubai_find_or_register_thread(task->pid, task->comm, entry);
	dubai_update_thread_dead(thread, task, utime, stime);

exit:
	mutex_unlock(&dubai_proc_lock);

	return NOTIFY_OK;
}

static struct notifier_block process_notifier_block = {
	.notifier_call	= dubai_process_notifier,
	.priority = INT_MAX,
};

static void dubai_proc_cputime_reset(void)
{
	unsigned long bkt;
	struct dubai_proc_entry *entry = NULL;
	struct hlist_node *tmp = NULL;
	struct dubai_thread_entry *thread = NULL, *temp = NULL;

	mutex_lock(&dubai_proc_lock);

	hash_for_each_safe(proc_hash_table, bkt, tmp, entry, hash) {
		list_for_each_entry_safe(thread, temp, &entry->threads, node) {
			list_del_init(&thread->node);
			kfree(thread);
		}
		hash_del(&entry->hash);
		kfree(entry);
	}
	atomic_set(&dead_count, 0);
	atomic_set(&active_count, 0);
	atomic_set(&proc_cputime_enable, 0);
	hash_init(proc_hash_table);

	mutex_unlock(&dubai_proc_lock);
}

int dubai_proc_cputime_enable(void __user *argp)
{
	int ret;
	bool enable = false;

	ret = get_enable_value(argp, &enable);
	if (ret == 0) {
		dubai_proc_cputime_reset();
		atomic_set(&proc_cputime_enable, enable ? 1 : 0);
		dubai_info("Dubai cpu process stats enable: %d", enable ? 1 : 0);
	}

	return ret;
}

int dubai_get_task_cpupower_enable(void __user *argp) {
	bool enable;

	enable = !!atomic_read(&task_power_enable);
	if (copy_to_user(argp, &enable, sizeof(bool)))
		return -EFAULT;

	return 0;
}

void dubai_proc_cputime_init(void)
{
	dubai_proc_cputime_reset();
	profile_event_register(PROFILE_TASK_EXIT, &process_notifier_block);
}

void dubai_proc_cputime_exit(void)
{
	profile_event_unregister(PROFILE_TASK_EXIT, &process_notifier_block);
	dubai_proc_cputime_reset();
}
