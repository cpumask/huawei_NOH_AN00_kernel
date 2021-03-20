/*
 * tlogger.c
 *
 * TEE Logging Subsystem, read the tee os log from log memory
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "tlogger.h"
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/delay.h>
#include <asm/ioctls.h>
#include <linux/syscalls.h>
#include <securec.h>
#include "smc_smp.h"
#include "mailbox_mempool.h"
#include "teek_client_constants.h"
#include "tc_ns_client.h"
#include "teek_ns_client.h"
#include "log_cfg_api.h"
#include "tc_ns_log.h"
#include "ko_adapt.h"

/* ----------------------------------------------- */
/* for log item ----------------------------------- */
#define LOG_ITEM_MAGIC        0x5A5A
#define LOG_ITEM_LEN_ALIGN    64
#define LOG_ITEM_MAX_LEN      1024
#define LOG_READ_STATUS_ERROR 0x000FFFF

/* =================================================== */
#define LOGGER_LOG_TEEOS               "hisi_teelog" /* tee os log */
#define __TEELOGGERIO                  0xBE /* for ioctl */
/* get tee verison */
#define TEELOGGER_GET_VERSION          _IOR(__TEELOGGERIO, 5, char[256])

/* set the log reader pos to current pos */
#define TEELOGGER_SET_READERPOS_CUR    _IO(__TEELOGGERIO, 6)
#define TEELOGGER_SET_TLOGCAT_STAT _IO(__TEELOGGERIO, 7)
#define TEELOGGER_GET_TLOGCAT_STAT _IO(__TEELOGGERIO, 8)

#ifdef CONFIG_TEE_KTRACE
#define TEELOGGER_GET_EVENT_KTRACE _IOR(__TEELOGGERIO, 9, char *)
#define TEEOS_KEVENT_TRACE_SIZE (8 * 2048)
char *g_event_buffer_start = NULL;
#endif

int g_tlogcat_f = 0;

#ifndef CONFIG_TEE_LOG_ACHIVE_PATH
#define CONFIG_TEE_LOG_ACHIVE_PATH "/data/log/tee/last_teemsg"
#endif

uint32_t g_lastread_offset = 0;

#define NEVER_USED_LEN 32U
#define LOG_ITEM_RESERVED_LEN 3U

typedef struct {
	uint8_t never_used[NEVER_USED_LEN];
	uint16_t magic;
	uint16_t log_buffer_crc;
	uint32_t serial_no;
	int16_t log_real_len;
	uint16_t log_buffer_len;
	uint8_t uuid[UUID_LEN];
	uint8_t reserved[LOG_ITEM_RESERVED_LEN];
	uint8_t new_line;
	uint8_t log_buffer[0];
} LOG_ITEM_T;

/* --- for log mem --------------------------------- */
#define TEMP_LOG_MEM_SIZE   (10*1024)

#define LOG_BUFFER_RESERVED_LEN 11U
#define VERSION_INFO_LEN 156U


typedef struct {
	uint16_t crc;
	uint16_t reserved0;
	uint32_t last_pos;
	uint32_t write_loops;
	uint32_t log_level;
	uint32_t reserved[LOG_BUFFER_RESERVED_LEN];
	uint32_t max_len;
	uint8_t version_info[VERSION_INFO_LEN];
} LOG_BUFFER_FLAG_T;

typedef struct {
	LOG_BUFFER_FLAG_T flag;
	uint8_t buffer_start[0];
} LOG_BUFFER_T;

static LOG_BUFFER_T *g_logbuffer = NULL;

struct tlogger_log_t {
	unsigned char *buffer_info; /* ring buffer info */
	struct mutex mutex_info; /* this mutex protects buffer_info */
	wait_queue_head_t wait_queue_head; /* wait queue head for reader */
	struct list_head logs; /* log channels list */
	struct miscdevice misc_device; /* misc device log */
	struct list_head readers; /* log's readers */
};

static LIST_HEAD(m_log_list);

#define READ_FAIL_MAX_TIMES	3
struct tlogger_reader_t {
	struct tlogger_log_t *log; /* tlogger_log_t info data */
	struct list_head list; /* log entry in tlogger_log_t's list */
	uint32_t r_off; /* Current reading position, start position of next read again */
	uint32_t r_loops;
	uint32_t r_sn;
	uint32_t r_failtimes;
	uint32_t r_from_cur;
	uint32_t r_is_tlogf;
	bool r_all; /* whether this reader can read all entries */
	unsigned int r_ver;
};

static uint32_t g_log_mem_len = 0;
static uint32_t g_tlogcat_count = 0;
static struct tlogger_log_t *g_log;

static inline struct tlogger_log_t *get_reader_log(const struct file *file)
{
	struct tlogger_reader_t *reader = NULL;
	if (file == NULL)
		return NULL;

	reader = (struct tlogger_reader_t *)file->private_data;
	if (reader == NULL)
		return NULL;

	return reader->log;
}

static LOG_ITEM_T *logitem_getnext(const uint8_t *buffer_start, uint32_t max_len, uint32_t readpos, uint32_t scope_len, uint32_t *pos)
{
	uint32_t i = 0;
	LOG_ITEM_T *logitem_next = NULL;
	uint32_t item_max_size = 0;
	bool con = ((buffer_start == NULL) || (pos == NULL));

	if (con)
		return NULL;

	if ((readpos + scope_len) > max_len)
		return NULL;

	while ((i + sizeof(LOG_ITEM_T) + LOG_ITEM_LEN_ALIGN) <= scope_len) {
		*pos = readpos + i;
		logitem_next = (LOG_ITEM_T *)(buffer_start + readpos + i);
		item_max_size = (scope_len - i) > LOG_ITEM_MAX_LEN ? LOG_ITEM_MAX_LEN : (scope_len - i);
		if (logitem_next->magic == LOG_ITEM_MAGIC
			&& logitem_next->log_buffer_len > 0
			&& logitem_next->log_real_len > 0
			&& logitem_next->log_buffer_len % LOG_ITEM_LEN_ALIGN == 0
			&& logitem_next->log_real_len <= logitem_next->log_buffer_len
			&& (logitem_next->log_buffer_len - logitem_next->log_real_len) < LOG_ITEM_LEN_ALIGN
			&& logitem_next->log_buffer_len + sizeof(LOG_ITEM_T) <= item_max_size) {
				break;
		}

		i += LOG_ITEM_LEN_ALIGN;
		logitem_next = NULL;
	}

	return logitem_next;
}

static uint32_t logitem_parse(char __user *buf, size_t count, const uint8_t	*buffer_start, uint32_t max_len,
	uint32_t start_pos, uint32_t end_pos, uint32_t *read_off, uint32_t *userbuffer_left)
{
	LOG_ITEM_T *logitem_next = NULL;
	uint32_t buf_left, buf_written, item_len;
	bool con = false;

	buf_written = 0;
	buf_left = count;

	con = ((userbuffer_left == NULL) || (read_off == NULL) || (buf == NULL) || (buffer_start == NULL));
	if (con)
		return buf_written;

	*userbuffer_left = 1;
	while (start_pos < end_pos) {
		logitem_next = logitem_getnext(buffer_start, max_len, start_pos, end_pos - start_pos, &start_pos);
		if (logitem_next == NULL)
			break;

		/* copy to user */
		item_len = logitem_next->log_buffer_len + sizeof(LOG_ITEM_T);
		if (buf_left < item_len) {
			*userbuffer_left = 0;
			break;
		}

		start_pos += item_len;
		if (copy_to_user(buf + buf_written, (void *)logitem_next, item_len) != 0)
			tloge("copy failed, item_len %u \n", item_len);

		buf_written += item_len;
		buf_left -= item_len;
	}

	*read_off = start_pos;
	return buf_written;
}

static ssize_t process_tlogger_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
	struct tlogger_reader_t *reader = NULL;
	struct tlogger_log_t *log = NULL;
	LOG_BUFFER_T *logbuffer = NULL;
	ssize_t ret = 0;
	uint32_t buf_written, userbuffer_left;
	uint32_t log_last_pos;
	errno_t ret_s;
	uint32_t log_buffer_maxlen = g_log_mem_len - sizeof(LOG_BUFFER_T);
	LOG_BUFFER_FLAG_T bufferflag;

	if (count < LOG_ITEM_MAX_LEN)
		return -EINVAL;

	if (log_buffer_maxlen > 0x100000)
		return -EINVAL;

	if (file == NULL || buf == NULL)
		return -EINVAL;

	reader = file->private_data;
	if (reader == NULL)
		return -EINVAL;

	log = reader->log;
	if (log == NULL)
		return -EINVAL;

	logbuffer = (LOG_BUFFER_T*)log->buffer_info;

	if (logbuffer == NULL)
		return -EINVAL;

	__asm__ volatile ("isb");
	__asm__ volatile ("dsb sy");

	mutex_lock(&log->mutex_info);
	ret_s = memcpy_s(&bufferflag, sizeof(bufferflag), &logbuffer->flag, sizeof(logbuffer->flag));
	mutex_unlock(&log->mutex_info);

	if (ret_s != EOK) {
		tloge("memcpy failed %d\n", ret_s);
		ret = -EAGAIN;
		return ret;
	}

	log_last_pos = bufferflag.last_pos;
	if (log_last_pos == reader->r_off && bufferflag.write_loops == reader->r_loops) {
		ret = 0;
		return ret;
	}

	if (bufferflag.max_len < log_last_pos || bufferflag.max_len > log_buffer_maxlen) {
		tloge("invalid data maxlen %x  pos %x \n", bufferflag.max_len , log_last_pos);
		ret = -EFAULT;
		return ret;
	}

	if (reader->r_off > bufferflag.max_len) {
		tloge("invalid data roff %x maxlen %x\n", reader->r_off , bufferflag.max_len);
		ret = -EFAULT;
		return ret;
	}

	/* 2. 再看上次读到的位置
		如果上次读的位置序列号比当前最小的小， 说明没有读完被覆盖了， 则从当前最小的开始读。
		否则按照上次读的位置接着读。
	*/
	buf_written = 0;
	userbuffer_left = 0;
	tlogd("read start \n");

	if (bufferflag.write_loops == reader->r_loops) {
		buf_written = logitem_parse(buf, count, logbuffer->buffer_start, bufferflag.max_len, reader->r_off, log_last_pos,
			&reader->r_off, &userbuffer_left);
	} else {
		if (bufferflag.write_loops > (reader->r_loops +1)
			|| ((bufferflag.write_loops == (reader->r_loops + 1) ) && (reader->r_off < log_last_pos))) {
			reader->r_off = log_last_pos;
			reader->r_loops = bufferflag.write_loops - 1;
		}

		buf_written = logitem_parse(buf, count, logbuffer->buffer_start, bufferflag.max_len, reader->r_off,
			bufferflag.max_len, &reader->r_off, &userbuffer_left);
		if (count > buf_written && userbuffer_left) {
			buf_written += logitem_parse(buf + buf_written, count - buf_written, logbuffer->buffer_start,
				bufferflag.max_len, 0, log_last_pos, &reader->r_off, &userbuffer_left);
			reader->r_loops = bufferflag.write_loops;
		}
	}

	if (buf_written == 0) {
		ret = LOG_READ_STATUS_ERROR;
	} else {
		ret = buf_written;
		tlogd("read length %u\n", buf_written);
		g_lastread_offset = reader->r_off;
	}

	return ret;
}

void tz_log_write(void)
{
	LOG_BUFFER_T *logbuffer = NULL;

	if (g_log == NULL)
		return;

	logbuffer = (LOG_BUFFER_T*)g_log->buffer_info;
	if (logbuffer == NULL)
		return;

	if (g_lastread_offset != logbuffer->flag.last_pos) {
		tlogd("tz_log_write wake up start \n");
		wake_up_interruptible(&g_log->wait_queue_head);
	}

	return;
}

static struct tlogger_log_t *get_tlogger_log_by_minor(int minor)
{
	struct tlogger_log_t *log = NULL;

	list_for_each_entry(log, &m_log_list, logs) {
		if (log->misc_device.minor == minor)
			return log;
	}
	return NULL;
}

static int process_tlogger_open(struct inode *inode, struct file *file)
{
	struct tlogger_log_t *log = NULL;
	int ret;
	struct tlogger_reader_t *reader = NULL;

	tlogd("open logger_open ++\n");
	/* not support seek */
	ret = nonseekable_open(inode, file);
	if (ret)
		return ret;
	tlogd("Before get_log_from_minor\n");
	log = get_tlogger_log_by_minor(MINOR(inode->i_rdev));
	if (log == NULL)
		return -ENODEV;

	reader = kmalloc(sizeof(struct tlogger_reader_t), GFP_KERNEL);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)reader))
		return -ENOMEM;

	reader->log = log;
	reader->r_all = true;
	reader->r_off = 0;
	reader->r_loops = 0;
	reader->r_sn = 0;
	reader->r_failtimes = 0;
	reader->r_is_tlogf = 0;
	reader->r_from_cur = 0;

	INIT_LIST_HEAD(&reader->list);

	mutex_lock(&log->mutex_info);
	list_add_tail(&reader->list, &log->readers);
	g_tlogcat_count++;
	mutex_unlock(&log->mutex_info);

	file->private_data = reader;
	tlogd("tlogcat count %u\n", g_tlogcat_count);
	return 0;
}

static int process_tlogger_release(struct inode *ignored, struct file *file)
{
	struct tlogger_reader_t *reader = NULL;
	struct tlogger_log_t *log = NULL;

	tlogd("logger_release ++\n");

	reader = file->private_data;
	if (reader == NULL) {
		tloge("reader is null");
		return -1;
	}

	log = reader->log;
	if (log == NULL) {
		tloge("log is null");
		return -1;
	}

	mutex_lock(&log->mutex_info);
	list_del(&reader->list);
	if (g_tlogcat_count >= 1)
		g_tlogcat_count--;
	mutex_unlock(&log->mutex_info);

	tlogi("logger_release r_is_tlogf-%u\n", reader->r_is_tlogf);
	if (reader->r_is_tlogf)
		g_tlogcat_f = 0;

	kfree(reader);
	tlogd("tlogcat count %u\n", g_tlogcat_count);
	return 0;
}

static unsigned int process_tlogger_poll(struct file *file, poll_table *wait)
{
	struct tlogger_reader_t *reader = NULL;
	struct tlogger_log_t *log = NULL;
	LOG_BUFFER_T *logbuffer = NULL;
	unsigned int ret = POLLOUT | POLLWRNORM;

	tlogd("logger_poll ++\n");
	if (file == NULL)
		return (unsigned int)(-1);

	reader = (struct tlogger_reader_t *)file->private_data;
	if (reader == NULL) {
		tloge("the private data is null\n");
		return ret;
	}

	log = reader->log;
	if (log == NULL) {
		tloge("log is null\n");
		return ret;
	}

	logbuffer = (LOG_BUFFER_T*)log->buffer_info;
	if (logbuffer == NULL) {
		tloge("logbuffer is null\n");
		return ret;
	}

	poll_wait(file, &log->wait_queue_head, wait);

	tlogd("poll after w_off_reader=%x reader->r_off=%x\n", logbuffer->flag.last_pos, reader->r_off);
	if (logbuffer->flag.last_pos != reader->r_off)
		ret |= POLLIN | POLLRDNORM;

	return ret;
}

static void tlogger_setreaderpos_cur(struct file *file)
{
	struct tlogger_reader_t *reader = NULL;
	struct tlogger_log_t *log = NULL;
	LOG_BUFFER_T *logbuffer = NULL;

	if (file == NULL)
		return;

	reader = file->private_data;
	if (reader == NULL)
		return;

	log = reader->log;

	if (log == NULL)
		return;

	logbuffer = (LOG_BUFFER_T*)log->buffer_info;

	if (logbuffer == NULL) {
		return;
	}

	reader->r_from_cur = 1;
	reader->r_off = logbuffer->flag.last_pos;
	reader->r_loops = logbuffer->flag.write_loops;
}

static void tlogger_set_tlogcat_f(struct file *file)
{
	struct tlogger_reader_t *reader = NULL;

	if (file == NULL)
		return;

	reader = file->private_data;
	if (reader == NULL)
		return;

	reader->r_is_tlogf = 1;
	g_tlogcat_f = 1;

	tlogi("set tlogcat_f-%u\n", g_tlogcat_f);
	return;
}
static int tlogger_get_tlogcat_f(void)
{
	tlogi("get tlogcat_f-%u\n", g_tlogcat_f);
	return g_tlogcat_f;
}

static int tlogger_get_version(unsigned int cmd, unsigned long arg)
{
	int ret = -EINVAL;
	if (_IOC_DIR(cmd) & _IOC_READ) {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0))
		ret = (int)access_ok(VERIFY_READ,
			(void __user *)(uintptr_t)arg, sizeof(g_logbuffer->flag.version_info));
#else
		ret = (int)access_ok(
			(void __user *)(uintptr_t)arg, sizeof(g_logbuffer->flag.version_info));
#endif

		if (ret) {
			if (copy_to_user((void __user *)(uintptr_t)arg, (void *)g_logbuffer->flag.version_info,
							  sizeof(g_logbuffer->flag.version_info)) != 0) {
				ret = -EINVAL;
				tloge("ver copy failed ret %d\n", ret);
			} else {
				ret = 0;
			}
		} else {
			ret = -EINVAL;
		}
	}
	return ret;
}

#ifdef CONFIG_TEE_KTRACE
static int tlogger_get_event_trace(unsigned int cmd, unsigned long arg)
{
	int ret = -EINVAL;
	if (_IOC_DIR(cmd) & _IOC_READ) {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0))
		ret = (int)access_ok(VERIFY_READ,
			(void __user *)(uintptr_t)arg, TEEOS_KEVENT_TRACE_SIZE);
#else
		ret = (int)access_ok(
			(void __user *)(uintptr_t)arg, TEEOS_KEVENT_TRACE_SIZE);
#endif
		if (ret && g_event_buffer_start != NULL) {
			if (copy_to_user((void __user *)(uintptr_t)arg, (void *)g_event_buffer_start, TEEOS_KEVENT_TRACE_SIZE) != 0) {
				ret = -EINVAL;
				tloge("kernel trace copy failed ret %d\n", ret);
			} else {
				ret = 0;
			}
		} else {
			ret = -EINVAL;
			tloge("invalid event buffer or accesss buffer failed\n");
		}
	}
	return ret;
}
#endif

static long process_tlogger_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct tlogger_log_t *log = NULL;
	long ret = -EINVAL;

	if (file == NULL) {
		return -1;
	}

	log = get_reader_log(file);
	if (g_logbuffer == NULL) {
		tloge("log buffer is null \n");
		return -1;
	}

	if (log == NULL) {
		tloge("log is null \n");
		return -1;
	}

	tlogd("logger_ioctl start ++\n");
	mutex_lock(&log->mutex_info);

	switch (cmd) {
	case TEELOGGER_GET_VERSION:
		if (tlogger_get_version(cmd, arg) == 0) {
			ret = 0;
		}
		break;
	case TEELOGGER_SET_READERPOS_CUR:
		tlogger_setreaderpos_cur(file);
		ret = 0;
		break;
	case TEELOGGER_SET_TLOGCAT_STAT:
		tlogger_set_tlogcat_f(file);
		ret = 0;
		break;
	case TEELOGGER_GET_TLOGCAT_STAT:
		ret = tlogger_get_tlogcat_f();
		break;
#ifdef CONFIG_TEE_KTRACE
	case TEELOGGER_GET_EVENT_KTRACE:
		if (tlogger_get_event_trace(cmd, arg) == 0) {
			ret = 0;
		}
		break;
#endif
	default:
		tloge("ioctl error default\n");
		break;
	}

	mutex_unlock(&log->mutex_info);
	return ret;
}

#ifdef CONFIG_COMPAT
static long process_tlogger_compat_ioctl(struct file *file, unsigned int cmd,
	unsigned long arg)
{
	long ret;

	tlogd("logger_compat_ioctl ++\n");
	arg = (unsigned long)(uintptr_t)compat_ptr(arg);
	ret = process_tlogger_ioctl(file, cmd, arg);
	return ret;
}
#endif

static const struct file_operations g_logger_fops = {
	.owner = THIS_MODULE,
	.read = process_tlogger_read,
	.poll = process_tlogger_poll,
	.unlocked_ioctl = process_tlogger_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = process_tlogger_compat_ioctl,
#endif
	.open = process_tlogger_open,
	.release = process_tlogger_release,
};


static int __init register_device(const char *log_name, uintptr_t addr, int size)
{
	int ret = 0;
	struct tlogger_log_t *log = NULL;
	unsigned char *buffer = NULL;

	buffer = (unsigned char *)addr;
	if (buffer == NULL)
		return -ENOMEM;

	if (log_name == NULL)
		return -EINVAL;

	log = kzalloc(sizeof(struct tlogger_log_t), GFP_KERNEL);
	if (log == NULL) {
		tloge("kzalloc is failed\n");
		return -ENOMEM;
	}
	log->buffer_info = buffer;
	log->misc_device.minor = MISC_DYNAMIC_MINOR;
	log->misc_device.name = kstrdup(log_name, GFP_KERNEL);
	if (log->misc_device.name == NULL) {
		ret = -ENOMEM;
		tloge("kstrdup is failed\n");
		goto out_free_log;
	}
	log->misc_device.fops = &g_logger_fops;
	log->misc_device.parent = NULL;

	init_waitqueue_head(&log->wait_queue_head);
	INIT_LIST_HEAD(&log->readers);
	mutex_init(&log->mutex_info);
	INIT_LIST_HEAD(&log->logs);
	list_add_tail(&log->logs, &m_log_list);

	/* register misc device for this log */
	ret = misc_register(&log->misc_device);
	if (unlikely(ret)) {
		tloge("failed to register misc device for log '%s'!\n", log->misc_device.name);
		goto out_free_log;
	}
	g_log = log;
	return 0;

out_free_log:
	if (log->misc_device.name != NULL) {
		kfree(log->misc_device.name);
	}
	kfree(log);
	return ret;
}

static LOG_ITEM_T *lastmsg_logitem_getnext(const uint8_t *buffer_start, uint32_t readpos, int scope_len, uint32_t max_len)
{
	int i = 0;
	LOG_ITEM_T *logitem_next = NULL;
	uint32_t item_max_size = 0;

	if (buffer_start == NULL)
		return NULL;

	while (i <= scope_len &&
		((readpos + i + sizeof(LOG_ITEM_T)) < max_len)) {

		item_max_size = (uint32_t)((scope_len - i) > LOG_ITEM_MAX_LEN ? LOG_ITEM_MAX_LEN : (scope_len - i));
		logitem_next = (LOG_ITEM_T	*)(buffer_start + readpos + i);

		if (logitem_next != NULL && logitem_next->magic == LOG_ITEM_MAGIC
			&& logitem_next->log_buffer_len > 0
			&& logitem_next->log_real_len > 0
			&& logitem_next->log_buffer_len % LOG_ITEM_LEN_ALIGN == 0
			&& logitem_next->log_real_len <= logitem_next->log_buffer_len
			&& (logitem_next->log_buffer_len - logitem_next->log_real_len) < LOG_ITEM_LEN_ALIGN
			&& logitem_next->log_buffer_len + sizeof(LOG_ITEM_T) < item_max_size) {

			if ((readpos + i + sizeof(LOG_ITEM_T) + logitem_next->log_buffer_len) > max_len) {
				return NULL;
			}

			return logitem_next;
		}

		i += LOG_ITEM_LEN_ALIGN;
		logitem_next = NULL;
	}

	return NULL;
}

#define OPEN_ERROR          (-1)
#define CHOWN_ERROR         (-2)
#define WRITE_VERSION_ERROR (-4)
#define WRITE_LOG_ERROR     (-6)
#define LOG_PATH_ERROR      (-7)

#define OPEN_FILE_MODE       0640U

#define ROOT_UID                0
#define ROOT_GID                0

static int tlogger_chown(void)
{
	uid_t user = ROOT_UID;
	gid_t group = ROOT_GID;
	int ret;

	get_log_chown(&user, &group);

	if (group == ROOT_GID && user == ROOT_UID)
		return 0;

	ret = (int)koadpt_sys_chown(
		(const char __user *)CONFIG_TEE_LOG_ACHIVE_PATH, user, group);
	if (ret != 0) {
		tloge("sys chown for last_teemsg file error\n");
		return CHOWN_ERROR;
	}

	return 0;
}

int tlogger_store_lastmsg(void)
{
	struct file *filep = NULL;
	ssize_t write_len, item_len, total_len;
	uint32_t read_off;
	mm_segment_t old_fs;
	loff_t pos = 0;
	int ret = 0;
	errno_t ret_s;
	LOG_ITEM_T *logitem_next = NULL;
	uint32_t log_buffer_maxlen = g_log_mem_len - sizeof(LOG_BUFFER_T);
	uint8_t *buffer = NULL;

	tlogd("filp_open last_teemsg\n");

	if (!g_tlogcat_count) {
		tlogd("tlogcat count  %u\n", g_tlogcat_count);
		return 0;
	}

	if (g_logbuffer == NULL)
		return 0;

	/* copy logs from log memory, then parse the logs */
	if (log_buffer_maxlen > 0x100000)
		return 0;

	buffer = kmalloc(log_buffer_maxlen, GFP_KERNEL);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)buffer)) {
		ret = -ENOMEM;
		goto out;
	}

	ret_s = memcpy_s(buffer, log_buffer_maxlen,  g_logbuffer->buffer_start, log_buffer_maxlen);
	if (ret_s != EOK) {
		tloge("memcpy failed %d\n", ret_s);
		ret = -EAGAIN;
		goto out;
	}

	/* exception handling, store trustedcore exception info to file */
	filep = filp_open(CONFIG_TEE_LOG_ACHIVE_PATH, O_CREAT | O_RDWR | O_TRUNC,
		OPEN_FILE_MODE);
	if (IS_ERR(filep)) {
		tloge("Failed to filp_open last_teemsg, err %ld\n", PTR_ERR(filep));
		ret = OPEN_ERROR;
		filep = NULL;
		goto out;
	}

	tlogd("Succeed to filp_open last_teemsg\n");

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	ret = tlogger_chown();
	if (ret != 0) {
		tloge("Failed to chown last_teemsg\n");
		goto out2;
	}

	/* first write tee versino info */
	write_len = koadpt_vfs_write(filep, g_logbuffer->flag.version_info,
		strlen(g_logbuffer->flag.version_info), &pos);
	if (write_len < 0) {
		tloge("Failed to write to last_teemsg version\n");
		ret = WRITE_VERSION_ERROR;
		goto out2;
	} else {
		tlogd("Succeed to Write to last_teemsg version, write_len=%ld\n",
			write_len);
	}

	read_off = 0;
	total_len = 0;
	logitem_next = lastmsg_logitem_getnext(buffer, read_off, LOG_ITEM_MAX_LEN, log_buffer_maxlen);

	while (logitem_next != NULL) {
		item_len = logitem_next->log_buffer_len + sizeof(LOG_ITEM_T);
		write_len = koadpt_vfs_write(filep, logitem_next->log_buffer, logitem_next->log_real_len, &pos);
		if (write_len < 0) {
			tloge("Failed to write to last_teemsg %zd\n", write_len);
			ret = WRITE_LOG_ERROR;
			goto out2;
		} else {
			tlogd("Succeed to Write to last_teemsg, write_len=%ld\n",
				write_len);
		}

		total_len += item_len;
		read_off = (uint8_t *)logitem_next - buffer + item_len;
		if (total_len >= log_buffer_maxlen)
			break;

		logitem_next = lastmsg_logitem_getnext(buffer, read_off, LOG_ITEM_MAX_LEN, log_buffer_maxlen);
	}
	pos = 0;
	ret = 0;

out2:
	set_fs(old_fs);
out:
	if (buffer != NULL) {
		kfree(buffer);
		buffer = NULL;
	}

	if (filep != NULL)
		filp_close(filep, 0);

	/* 触发写 teeos_log */
	tz_log_write();
	return ret;
}


int register_mem_to_teeos(uint64_t mem_addr, uint32_t mem_len)
{
	struct tc_ns_smc_cmd smc_cmd = { {0}, 0 };
	struct mb_cmd_pack *mb_pack = NULL;
	int ret;

	mb_pack = mailbox_alloc_cmd_pack();
	if (mb_pack == NULL) {
		tloge("mailbox alloc failed\n");
		return -ENOMEM;
	}

	smc_cmd.global_cmd = true;
	smc_cmd.cmd_id = GLOBAL_CMD_ID_REGISTER_LOG_MEM;
	mb_pack->operation.paramtypes = TEE_PARAM_TYPE_VALUE_INPUT |
		TEE_PARAM_TYPE_VALUE_INPUT << TEE_PARAM_NUM;
	mb_pack->operation.params[0].value.a = mem_addr;
	mb_pack->operation.params[0].value.b = mem_addr >> ADDR_TRANS_NUM;
	mb_pack->operation.params[1].value.a = mem_len;

	smc_cmd.operation_phys = virt_to_phys(&mb_pack->operation);
	smc_cmd.operation_h_phys = virt_to_phys(&mb_pack->operation) >> ADDR_TRANS_NUM;
	ret = tc_ns_smc(&smc_cmd);
	mailbox_free(mb_pack);
	if (ret != 0)
		tloge("Send log mem info failed\n");
	return ret;
}

static int register_mem_cfg(uint64_t *addr, uint32_t *len)
{
	int ret;
	ret = register_log_mem(addr, len);
	if (ret != 0)
		tloge("register log mem failed %x\n", ret);

	ret = register_log_exception();
	if (ret != 0)
		tloge("teeos register exception to log module failed\n");

	return ret;
}

static int __init init_tlogger_service(void)
{
	int ret;
	uint64_t log_mem_addr;

	ret = register_mem_cfg(&log_mem_addr, &g_log_mem_len);
	if (ret != 0)
		return ret;

#ifdef CONFIG_TEE_KTRACE
	if (g_log_mem_len < TEMP_LOG_MEM_SIZE || g_log_mem_len - TEMP_LOG_MEM_SIZE < TEEOS_KEVENT_TRACE_SIZE)  {
#else
	if (g_log_mem_len < TEMP_LOG_MEM_SIZE) {
#endif
		tloge("log mem init failed!!! log len is too small 0x%x\n", g_log_mem_len);
		return -1;
	}

	if (log_mem_addr == 0) {
		tloge("mem init failed!!! addr is 0\n");
		return -1;
	}

	g_logbuffer = (LOG_BUFFER_T *)map_log_mem(log_mem_addr, g_log_mem_len);
	if (g_logbuffer == NULL)
		return -ENOMEM;
#ifdef CONFIG_TEE_KTRACE
	g_log_mem_len = g_log_mem_len - TEEOS_KEVENT_TRACE_SIZE;
	g_event_buffer_start = ((char *)g_logbuffer) + g_log_mem_len;
#endif
	g_logbuffer->flag.max_len = g_log_mem_len - sizeof(LOG_BUFFER_T);

	tloge("tlogcat verison 1.0.0\n");
	tlogd("tlogcat:start=0x%lx\t len=0x%x\n", (unsigned long)g_logbuffer->buffer_start, g_logbuffer->flag.max_len);
	ret = register_device(LOGGER_LOG_TEEOS, (uintptr_t)g_logbuffer, sizeof(LOG_BUFFER_T) + g_logbuffer->flag.max_len);
	if (ret != 0) {
		unmap_log_mem((int *)g_logbuffer);
		g_logbuffer = NULL;
#ifdef CONFIG_TEE_KTRACE
		g_event_buffer_start = NULL;
#endif
		g_log_mem_len = 0;
	}

	return ret;
}

static void unregister_mem_cfg(void)
{
	if (g_logbuffer != NULL)
		unmap_log_mem((int *)g_logbuffer);

	unregister_log_exception();
}

static void __exit exit_tlogger_service(void)
{
	struct tlogger_log_t *current_log = NULL;
	struct tlogger_log_t *next_log = NULL;

	list_for_each_entry_safe(current_log, next_log, &m_log_list, logs) {
		/* we have to delete all the entry inside m_log_list */
		misc_deregister(&current_log->misc_device);
		kfree(current_log->misc_device.name);
		list_del(&current_log->logs);
		kfree(current_log);
	}

	unregister_mem_cfg();
	g_logbuffer = NULL;
	g_log_mem_len = 0;
#ifdef CONFIG_TEE_KTRACE
	g_event_buffer_start = NULL;
#endif
}

#ifdef CONFIG_TZDRIVER
device_initcall(init_tlogger_service);
module_exit(exit_tlogger_service);

MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
MODULE_DESCRIPTION("TrustCore Logger");
MODULE_VERSION("1.00");
#endif
