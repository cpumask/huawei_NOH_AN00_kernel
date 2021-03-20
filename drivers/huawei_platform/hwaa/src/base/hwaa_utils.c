
#include "inc/base/hwaa_utils.h"
#include <linux/err.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/limits.h>
#include <linux/mm.h>
#include <linux/path.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/rcupdate.h>
#include <linux/sched/task.h>
#include <linux/sched/mm.h>
#include <securec.h>
#include "inc/base/hwaa_define.h"
#include "huawei_platform/hwaa/hwaa_limits.h"

#define EXE_MAX_ARG_STRLEN 256
#define MAX_JSON_STR_LEN 256
#define HEX_STR_PER_BYTE 2
#define HWAA_PERMISSION_LEN 8
#define PROCESS_INIT_TAG "init="
#define PROCESS_INIT_TAG_LEN 5

s64 hwaa_utils_get_ausn(uid_t uid)
{
	return (s64)(uid / HWAA_PER_USER_RANGE);
}

static s8 hex2char(u8 hex)
{
	s8 c;
	u8 lower = hex & 0x0f;

	if (lower >= 0x0a)
		c = ('a' + lower - 0x0a);
	else
		c = '0' + lower;
	return c;
}

void bytes2string(u8 *bytes, u32 len, s8 *str, u32 str_len)
{
	s32 i;

	if (!bytes || !str || ((HEX_STR_PER_BYTE * len + 1) > str_len))
		return;
	for (i = 0; i < len; i++) {
		str[HEX_STR_PER_BYTE * i] = hex2char((bytes[i] & 0xf0) >> 4);
		str[HEX_STR_PER_BYTE * i + 1] = hex2char(bytes[i] & 0x0f);
	}
	str[HEX_STR_PER_BYTE * len] = '\0';
}

s32 valid_init_user(struct hwaa_init_user_t *usr)
{
	s32 ret = 0;

	if (!usr || !usr->key_desc ||
		(usr->key_desc_len < HWAA_USER_KEY_DESC_MIN) ||
		(usr->key_desc_len > HWAA_USER_KEY_DESC_MAX)) {
		hwaa_pr_err("hwaa_init_user_t was invalid!");
		ret = -EINVAL;
	}
	return ret;
}

static bool hwaa_utils_exe_check_without_check_para(const s8 *exe_path,
	s32 exe_path_len, pid_t pid)
{
	bool is_matched = false;
	s8 *buf = NULL;
	s8 *path = NULL;
	struct file *exe_file = NULL;
	struct mm_struct *mm = NULL;
	struct task_struct *task = NULL;
	rcu_read_lock();
	task = find_task_by_vpid(pid);
	if (!task) {
		rcu_read_unlock();
		hwaa_pr_err("Failed to lookup running task! (pid:%d)", pid);
		return is_matched;
	}
	get_task_struct(task);
	rcu_read_unlock();
	mm = get_task_mm(task);
	if (mm) {
		buf = (s8 *)__get_free_page(GFP_KERNEL);
		if (buf == NULL)
			goto done_mmput;
		exe_file = get_mm_exe_file(mm);
		if (exe_file == NULL)
			goto done_free;
		path = file_path(exe_file, buf, PAGE_SIZE);
		if (path == NULL || IS_ERR(path))
			goto done_put;
		if (strncmp(path, exe_path, exe_path_len) == 0)
			is_matched = true;
		else
			hwaa_pr_err("arg dismatch %s", path);
	} else {
		put_task_struct(task);
		return is_matched;
	}
done_put:
	fput(exe_file);
done_free:
	free_page((unsigned long)buf);
done_mmput:
	mmput(mm); // only when mm is not null would this branch take affect
	put_task_struct(task);
	return is_matched;
}

bool hwaa_utils_exe_check(pid_t pid, const s8 *exe_path, s32 exe_path_len)
{
	if ((exe_path == NULL) || (exe_path_len > EXE_MAX_ARG_STRLEN) ||
		(exe_path_len < 0))
		return false;

	return hwaa_utils_exe_check_without_check_para(exe_path,
		exe_path_len, pid);
}