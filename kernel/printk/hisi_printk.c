/*
 * Hisilicon Printk.
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2016-2019. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include "hisi_printk.h"

#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <securec.h>
#include <linux/version.h>
#ifdef CONFIG_HUAWEI_PRINTK_CTRL
#include <linux/hisi/hw_cmdline_parse.h> /* for runmode_is_factory */
#include <log/log_usertype.h>
#endif

#define SCBBPD_RX_STAT1 0x534
#define SCBBPD_RX_STAT2 0x538

#define HAS_INITED_TIME_ADDR 1
#define HAS_NOT_INITED_TIME_ADDR 0
#define TRUE 1
#define SECONDS_OF_MINUTE 60
#define BEGIN_YEAR 1900
#define REG_NUMS 3

#define FPGA_SECOND_TICK_CNT 19200000
#define SECOND_TICK_CNT 32764

static unsigned int scbbpdrxstat1;
static unsigned int scbbpdrxstat2;
static unsigned int fpga_flag;
static void __iomem *addr;
static int init_time_addr;

u64 hisi_getcurtime(void)
{
	u64 timervalue[4]; /* there are 4 time registers */
	u64 pcurtime;
	u64 ts;

	if (!init_time_addr)
		return 0;

	timervalue[0] = readl(addr + scbbpdrxstat1);
	timervalue[1] = readl(addr + scbbpdrxstat2);
	timervalue[2] = readl(addr + scbbpdrxstat1);
	timervalue[3] = readl(addr + scbbpdrxstat2);

	/* make up current time value */
	if (timervalue[2] < timervalue[0])
		pcurtime = (((timervalue[3] - 1) << 32) | timervalue[0]);
	else
		pcurtime = ((timervalue[1] << 32) | timervalue[0]);

	/* convert system tick count to ns */
	if (fpga_flag == FPGA_CONFIGED) {
		ts = do_div(pcurtime, FPGA_SECOND_TICK_CNT);
		
		ts = ts * 625;
		do_div(ts, 12);
		pcurtime = pcurtime * NS_INTERVAL + ts;
	} else {
		ts = do_div(pcurtime, SECOND_TICK_CNT);
		
		ts = ts * 1953125;
		do_div(ts, 64);
		pcurtime = pcurtime * NS_INTERVAL + ts;
	}

	return pcurtime;
}

size_t print_time(u64 ts, char *buf)
{
	int temp;
	int ret;
	char ts_buf[TS_BUF_SIZE] = {0};
	unsigned long rem_nsec;

	rem_nsec = do_div(ts, NS_INTERVAL);

	if (!printk_time)
		return 0;

	if (!buf) {
		ret = snprintf_s(ts_buf, TS_BUF_SIZE, TS_BUF_SIZE - 1,
			"[%5lu.000000s]", (unsigned long)ts
			);
		if (ret >= 0)
			return (unsigned int)ret;
		else
			return 0;
	}

	/* cppcheck-suppress */
	temp = sprintf_s(buf, TIME_LOG_SIZE, "[%5lu.%06lus]", (unsigned long)ts,
		rem_nsec / 1000); /* convert to us */
	if (temp >= 0)
		return (unsigned int)temp;
	else
		return 0;
}

static int __init uniformity_timer_init(void)
{
	struct device_node *np = NULL;
	int ret;

	np = of_find_compatible_node(NULL, NULL, "hisilicon,prktimer");
	if (!np) {
		pr_err("NOT FOUND device node 'hisilicon,prktimer'!\n");
		return -ENXIO;
	}

	if (addr) {
		init_time_addr = HAS_NOT_INITED_TIME_ADDR;
		early_iounmap(addr, SZ_4K);
		addr = NULL;
	}

	addr = of_iomap(np, 0);
	if (!addr) {
		pr_err("get prktimer resource fail, lisc addr init fail\n");
		return -ENXIO;
	}

	ret = of_property_read_u32(np, "fpga_flag", &fpga_flag);
	if (ret) {
		pr_err("failed to get fpga_flag resource\n");
		return -ENXIO;
	}

	if (fpga_flag == FPGA_CONFIGED) {
		scbbpdrxstat1 = FPGA_SCBBPD_RX_STAT1;
		scbbpdrxstat2 = FPGA_SCBBPD_RX_STAT2;
	} else {
		scbbpdrxstat1 = SCBBPD_RX_STAT1;
		scbbpdrxstat2 = SCBBPD_RX_STAT2;
	}
	init_time_addr = HAS_INITED_TIME_ADDR;

	return 0;
}

#ifdef CONFIG_HISI_APANIC
static void panic_print_msg(struct printk_log *msg)
{
	char *text = (char *)msg + sizeof(struct printk_log);
	size_t text_size = msg->text_len;
	char time_log[TIME_LOG_SIZE] = "";
	size_t tlen = 0;
	char *ptime_log = NULL;

	ptime_log = time_log;

	do {
		const char *next = memchr(text, '\n', text_size);
		size_t text_len;

		if (next) {
			text_len = next - text;
			next++;
			text_size -= next - text;
		} else {
			text_len = text_size;
		}
		tlen = print_time(msg->ts_nsec, ptime_log);
		apanic_console_write(ptime_log, tlen);
		apanic_console_write(text, text_len);
		apanic_console_write("\n", 1);

		text = (char *)next;
	} while (text);
}
#endif

void hisi_log_store_add_time(
	char *hisi_char, u32 sizeof_hisi_char, u16 *hisi_len)
{
	struct tm tm_rtc;
	unsigned long cur_secs;
	static unsigned long prev_jffy;
	static unsigned int prejf_init_flag;
	int ret = 0;

	if (!prejf_init_flag) {
		prejf_init_flag = TRUE;
		prev_jffy = jiffies;
	}
	cur_secs = get_seconds();
	cur_secs -= sys_tz.tz_minuteswest * SECONDS_OF_MINUTE;
	time_to_tm(cur_secs, 0, &tm_rtc);
	if (time_after(jiffies, prev_jffy + 1 * HZ)) {
		prev_jffy = jiffies;
		ret = snprintf_s(
			hisi_char, sizeof_hisi_char, sizeof_hisi_char - 1,
			"[%lu:%.2d:%.2d %.2d:%.2d:%.2d]",
			BEGIN_YEAR + tm_rtc.tm_year,
			tm_rtc.tm_mon + 1, tm_rtc.tm_mday, tm_rtc.tm_hour,
			tm_rtc.tm_min, tm_rtc.tm_sec);
		if (ret < 0) {
			pr_err("%s: snprintf_s failed\n", __func__);
			return;
		}
		*hisi_len += ret;
	}

#if defined(CONFIG_HISI_PRINTK_EXTENSION)
	ret = snprintf_s(hisi_char + *hisi_len, sizeof_hisi_char - *hisi_len,
		sizeof_hisi_char - *hisi_len - 1,
		"[pid:%d,cpu%u,%s]", current->pid,
		smp_processor_id(), in_irq() ? "in irq" : current->comm);
#endif
	if (ret < 0) {
		pr_err("%s: snprintf_s failed\n", __func__);
		return;
	}
	*hisi_len += ret;
}
pure_initcall(uniformity_timer_init);

#ifdef CONFIG_HISI_AMBA_PL011
int get_console_index(void)
{
	if ((preferred_console != -1) &&
		(preferred_console < MAX_CMDLINECONSOLES))
		return console_cmdline[preferred_console].index;

	return -1;
}

int get_console_name(char *name, int name_buf_len)
{
	int ret = -1;
	u32 strcount = 0;

	if (!name) {
		pr_err("%s:name is NULL\n", __func__);
		return -1;
	}

	if (name_buf_len <= 0) {
		pr_err("%s:name_buf_len is invalid\n", __func__);
		return -1;
	}

	if ((preferred_console != -1) &&
		(preferred_console < MAX_CMDLINECONSOLES)) {
		strcount = (u32)min(
			(int)(sizeof(console_cmdline[preferred_console].name)),
			name_buf_len - 1);
		ret = strncpy_s(name, name_buf_len,
			console_cmdline[preferred_console].name,
			strcount);
		if (ret) {
			pr_err("%s: strncpy_s failed\n", __func__);
			return -1;
		}
	}

	return 0;
}
#endif

#ifdef CONFIG_HUAWEI_PRINTK_CTRL
int printk_level = LOGLEVEL_DEBUG;
int sysctl_printk_level = LOGLEVEL_DEBUG;
/*
 * if loglevel > level, the log will not be saved to memory in no log load
 * log load and factory mode load will not be affected
 */
void printk_level_setup(int level)
{
	if (runmode_is_factory())
		return;

	pr_alert("%s: %d\n", __func__, level);
	raw_spin_lock(g_logbuf_level_lock_ex);
	if (level >= LOGLEVEL_EMERG && level <= LOGLEVEL_DEBUG)
		printk_level = level;
	raw_spin_unlock(g_logbuf_level_lock_ex);
}
#endif

static int __init early_printk_timer_setup(char *str)
{
	int i = 0;
	int ret;
	char *token = NULL;
	unsigned long long regs[REG_NUMS] = {};

	while ((token = strsep(&str, ","))) {
		token = strim(token);
		if (!*token)
			continue;

		if (i >= REG_NUMS)
			goto fmt_err;

		ret = kstrtoull(token, 0, &regs[i++]);
		if (ret < 0)
			goto fmt_err;
	}

	if (i != REG_NUMS)
		goto fmt_err;

	if (!regs[0])
		goto fmt_err;

	addr = early_ioremap(regs[0], SZ_4K);
	if (!addr) {
		pr_err("%s fail to ioremap\n", __func__);
		return -ENOMEM;
	}

	scbbpdrxstat1 = regs[1];
	scbbpdrxstat2 = regs[2];
	init_time_addr = HAS_INITED_TIME_ADDR;

	return 0;

fmt_err:
	pr_err("should be printktimer=<base>,<offset>,<offset>\n");
	return -EINVAL;
}

early_param("printktimer", early_printk_timer_setup);
