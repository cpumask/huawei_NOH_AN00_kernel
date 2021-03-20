/*
 * tui.c
 *
 * tui agent for tui display and interact
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
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
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/sched/types.h>
#include <linux/sched/rt.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/debugfs.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/atomic.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/version.h>
#include <linux/ion.h>
#include <linux/cma.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/sysfs.h>
/* add for CMA malloc framebuffer */
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_reserved_mem.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <asm/tlbflush.h>
#include <asm/cacheflush.h>
#include <linux/kmemleak.h>
#include <linux/hisi/hisi_ion.h>
#include <securec.h>

#include "teek_client_constants.h"
#include "agent.h"
#include "mem.h"
#include "teek_ns_client.h"
#include "tui.h"
#include "smc_smp.h"
#include "tc_ns_client.h"
#include "hisi_fb.h"
#include "tc_ns_log.h"
#include "mailbox_mempool.h"
#include <linux/hisi/hisi_powerkey_event.h>
#include "dynamic_ion_mem.h"

#define MAX_SCREEN_RESOLUTION 8192

static void tui_poweroff_work_func(struct work_struct *work);
static ssize_t tui_status_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static void tui_msg_del(const char *name);
static DECLARE_DELAYED_WORK(tui_poweroff_work, tui_poweroff_work_func);

static struct kobject *g_tui_kobj = NULL;
static struct kobj_attribute tui_attribute =
	__ATTR(c_state, 0440, tui_status_show, NULL);
static struct attribute *attrs[] = {
	&tui_attribute.attr,
	NULL,
};

static struct attribute_group tui_attr_group = {
	.attrs = attrs,
};

static struct task_struct *g_tui_task = NULL;
static struct tui_ctl_shm *g_tui_ctl = NULL;

static spinlock_t tui_msg_lock;
static struct list_head tui_msg_head;

static atomic_t tui_usage = ATOMIC_INIT(0);
static atomic_t tui_state = ATOMIC_INIT(TUI_STATE_UNUSED);

DEFINE_MUTEX(tui_drv_lock);
static struct list_head tui_drv_head = LIST_HEAD_INIT(tui_drv_head);

static atomic_t g_tui_attached_device = ATOMIC_INIT(TUI_PID_CLEAR);
static atomic_t g_tui_pid = ATOMIC_INIT(TUI_PID_CLEAR);

static wait_queue_head_t tui_state_wq;
static int tui_state_flag;
static bool normal_load_flag = false;

static wait_queue_head_t tui_msg_wq;
static int tui_msg_flag;
static struct hisi_fb_data_type *dss_fd;

#define TUI_DSS_NAME "DSS"
#define TUI_GPIO_NAME "fff0d000.gpio"
#define TUI_TP_NAME "tp"
#define TUI_FP_NAME "fp"

#define TTF_NORMAL_BUFF_SIZE (10 * 1024 * 1024)
#define TTF_NORMAL_FILE_PATH "/system/fonts/DroidSansChinese.ttf"

/* 2M memory size is 2^21 */
#define ALIGN_SIZE 21
#define ALIGN_M  (1<<21)

/* dss and tp couple mode: 0 is init dss and tp; 1 is only init dss; 2 is only init tp */
#define DSS_TP_COUPLE_MODE 0

#define NORMAL_MODE 0 /* init all driver */
#define ONLY_INIT_DSS 1 /* only init dss */
#define ONLY_INIT_TP 2 /* only init tp */

/* do fp init(disable fp irq) before gpio init in order not response
 * sensor in normal world(when gpio secure status is set) */
#if ONLY_INIT_DSS == DSS_TP_COUPLE_MODE
#define DRIVER_NUM 1
static char *g_init_driver[DRIVER_NUM] = {TUI_DSS_NAME};
static char *g_deinit_driver[DRIVER_NUM] = {TUI_DSS_NAME};
#endif

#if ONLY_INIT_TP == DSS_TP_COUPLE_MODE
#define DRIVER_NUM 3
static char *g_init_driver[DRIVER_NUM] = {TUI_TP_NAME, TUI_FP_NAME, TUI_GPIO_NAME};
static char *g_deinit_driver[DRIVER_NUM] = {TUI_TP_NAME, TUI_FP_NAME, TUI_GPIO_NAME};
#endif

#if NORMAL_MODE == DSS_TP_COUPLE_MODE
#define DRIVER_NUM 4
static char *g_init_driver[DRIVER_NUM] = {TUI_DSS_NAME, TUI_TP_NAME, TUI_FP_NAME, TUI_GPIO_NAME};
static char *g_deinit_driver[DRIVER_NUM] = {TUI_DSS_NAME, TUI_TP_NAME, TUI_FP_NAME, TUI_GPIO_NAME};
#endif

#define TIME_OUT_FOWER_ON 100

#define DOWN_VAL 22 // 4M
#define UP_VAL 27 // 64M
#define COLOR_TYPE 4 /* ARGB */
#define BUFFER_NUM 2

/*tui-need-memory is calculated dynamically according to the screen resolution*/
struct tui_mem {
	unsigned int tui_addr_size;
	unsigned int tui_addr;
	unsigned int tui_addr_h;
	struct device *tui_dev;
	char *tui_virt;
};

struct ttf_mem {
	unsigned int ttf_addr_h;
	unsigned int ttf_addr_l;
	char *ttf_buff_virt;
	unsigned int ttf_file_size;
};


typedef struct tui_memory {
	phys_addr_t tui_ion_phys_addr;
	void *tui_ion_virt_addr;
	size_t len;
	uint32_t size;
	uint32_t configid;
	struct sg_table *tui_sg_table;
	phys_addr_t fb_phys_addr;
	uint32_t npages;
	uint64_t info_length;
} tui_ion_mem;

static tui_ion_mem tui_display_mem;
static tui_ion_mem normal_font_mem;

unsigned int get_frame_size(unsigned int num)
{
	if (num % ALIGN_M)
		return (((num >> ALIGN_SIZE) + 1) << ALIGN_SIZE);
	else
		return num;
}

unsigned int get_tui_size(unsigned int num)
{
	unsigned int i;
	for (i = DOWN_VAL; i < UP_VAL; i++)
		if (!(num >> i))
			break;
	return (unsigned int)1 << i;
}

/* alloc order: 4M-font, framebuffer, 20M-unusualfont */
/* 1.4M alloc when boot so from ION_TUI_HEAP_ID */
/* 2.20M and frambuffer alloc when tui init so from ION_MISC_HEAP_ID */
static size_t get_tui_font_file_size(void)
{
	int ret;
	struct kstat ttf_file_stat;
	mm_segment_t old_fs;

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	/*get ttf file size*/
	ret = vfs_stat(TTF_NORMAL_FILE_PATH, &ttf_file_stat);
	if (ret < 0) {
		tloge("Failed to get ttf extend file size, ret is %d\n", ret);
		set_fs(old_fs);
		return 0;
	}
	set_fs(old_fs);

	return ttf_file_stat.size;
}
static int get_tui_ion_sglist(tui_ion_mem *tui_mem)
{
	struct sglist *tmp_tui_sglist = NULL;
	struct scatterlist *tui_sg = NULL;
	struct page *tui_page = NULL;
	uint32_t tui_sglist_size;
	uint32_t i = 0;
	struct sg_table *tui_ion_sg_table = NULL;

	if (tui_mem == NULL) {
		tloge("invalid input params\n");
		return -1;
	}

	tui_ion_sg_table = tui_mem->tui_sg_table;
	if (tui_ion_sg_table == NULL) {
		tloge("invalid sgtable\n");
		return -1;
	}

	tui_sglist_size = sizeof(struct ion_page_info) * tui_ion_sg_table->nents +
		sizeof(struct sglist);
	tmp_tui_sglist = (struct sglist *)mailbox_alloc(tui_sglist_size, MB_FLAG_ZERO);

	if (tmp_tui_sglist == NULL) {
		tloge("in %s err: mailbox_alloc failed\n", __func__);
		return -1;
	}

	tmp_tui_sglist->sglist_size = (uint64_t)tui_sglist_size;
	tmp_tui_sglist->ion_size = (uint64_t)tui_mem->len;
	tmp_tui_sglist->info_length = (uint64_t)tui_ion_sg_table->nents;
	tui_mem->info_length = (uint64_t)tui_ion_sg_table->nents;

	/* nent must be 1, because ion addr for tui is continuous */
	if (tui_ion_sg_table->nents != 1) {
		tloge("nent is invalid\n");
		mailbox_free(tmp_tui_sglist);
		return -1;
	}
	/* get tui_sg to fetch ion for tui */
	for_each_sg(tui_ion_sg_table->sgl, tui_sg, tui_ion_sg_table->nents, i) {
		if (tui_sg == NULL) {
			tloge("tui sg is NULL");
			mailbox_free(tmp_tui_sglist);
			return -1;
		}
		tui_page = sg_page(tui_sg);
		tmp_tui_sglist->page_info[0].phys_addr = page_to_phys(tui_page);
		tmp_tui_sglist->page_info[0].npages = tui_sg->length / PAGE_SIZE;
		tui_mem->npages = tmp_tui_sglist->page_info[0].npages;
		tui_mem->tui_ion_virt_addr = phys_to_virt(tmp_tui_sglist->page_info[0].phys_addr);
		tui_mem->fb_phys_addr = tmp_tui_sglist->page_info[0].phys_addr;
	}

	tui_mem->tui_ion_phys_addr = virt_to_phys((void *)tmp_tui_sglist); // sglist phys-addr
	if (tui_mem->tui_ion_phys_addr == 0) {
		tloge("Failed to get tmp_tui_sglist physaddr, configid=%d\n",
		tui_mem->configid);
		mailbox_free(tmp_tui_sglist);
		return -1;
	}
	tui_mem->size = tui_sglist_size;
	return 0;
}

static int alloc_ion_mem(tui_ion_mem *tui_mem)
{
	struct sg_table *tui_ion_sg_table = NULL;
	if (tui_mem == NULL) {
		tloge("bad input params\n");
		return -1;
	}

	tui_ion_sg_table = hisi_secmem_alloc(SEC_TUI, tui_mem->len);
	if (tui_ion_sg_table == NULL) {
		tloge("failed to get ion page for tui,configid is %d\n", tui_mem->configid);
		return -1;
	}
	tui_mem->tui_sg_table = tui_ion_sg_table;
	return 0;
}

static void free_ion_mem(tui_ion_mem *tui_mem)
{
	if (tui_mem->tui_sg_table == NULL || tui_mem->tui_ion_phys_addr == 0) {
		tloge("bad input params\n");
		return;
	}
	hisi_secmem_free(SEC_TUI, tui_mem->tui_sg_table);
	tui_mem->tui_ion_phys_addr = 0;
	return;
}

static void free_tui_font_mem(void)
{
	free_ion_mem(&normal_font_mem);
	normal_load_flag = false;
	tloge("normal tui font file freeed\n");
}

static int get_tui_font_mem(tui_ion_mem *tui_font_mem)
{
	int ret;

	ret = alloc_ion_mem(tui_font_mem);
	if (ret < 0) {
		tloge("Failed to alloc cma mem for tui font lib\n");
		return -ENOMEM;
	}

	return 0;
}

/* size is calculated dynamically according to the screen resolution */
static phys_addr_t get_frame_addr(void)
{
	int screen_r;
	int ret;
	bool check_params = false;
	if(dss_fd == NULL){
		return 0;
	}
	check_params = (dss_fd->panel_info.xres > MAX_SCREEN_RESOLUTION) ||
		(dss_fd->panel_info.yres > MAX_SCREEN_RESOLUTION);
	if (check_params) {
		tloge("Horizontal resolution or Vertical resolution is too large\n");
		return 0;
	}
	screen_r = dss_fd->panel_info.xres * dss_fd->panel_info.yres * COLOR_TYPE * BUFFER_NUM;
	tui_display_mem.len = get_frame_size(screen_r);
	ret = alloc_ion_mem(&tui_display_mem);
	if (ret) {
		tloge("Failed to alloc mem for tui display\n");
		return 0;
	}

	if (get_tui_ion_sglist(&tui_display_mem)) {
		tloge("get sglist failed\n");
		free_ion_mem(&tui_display_mem);
		return 0;
	}

	return tui_display_mem.fb_phys_addr;
}

void free_frame_addr(void)
{
	mailbox_free(phys_to_virt(tui_display_mem.tui_ion_phys_addr));
	free_ion_mem(&tui_display_mem);
	return;
}
static int32_t tc_ns_register_tui_font_mem(tui_ion_mem *tui_font_mem,
	size_t font_file_size)
{
	struct tc_ns_smc_cmd smc_cmd = { {0}, 0 };
	int ret = 0;
	struct mb_cmd_pack *mb_pack = NULL;


	mb_pack = mailbox_alloc_cmd_pack();
	if (mb_pack == NULL) {
		tloge("alloc cmd pack failed\n");
		return -ENOMEM;
	}

	smc_cmd.global_cmd = true;
	smc_cmd.cmd_id = GLOBAL_CMD_ID_REGISTER_TTF_MEM;

	mb_pack->operation.paramtypes = teec_param_types(
		TEEC_MEMREF_TEMP_INPUT,
		TEEC_VALUE_INPUT,
		TEEC_NONE,
		TEEC_NONE
	);

	mb_pack->operation.params[0].memref.size = (uint32_t)(tui_font_mem->size);
	mb_pack->operation.params[0].memref.buffer = (uint32_t)(tui_font_mem->tui_ion_phys_addr & 0xFFFFFFFF);
	mb_pack->operation.buffer_h_addr[0] = tui_font_mem->tui_ion_phys_addr >> 32;
	mb_pack->operation.params[1].value.a = font_file_size;


	smc_cmd.operation_phys = (unsigned int)virt_to_phys(&mb_pack->operation);
	smc_cmd.operation_h_phys = virt_to_phys(&mb_pack->operation) >> 32;
	if (tc_ns_smc(&smc_cmd)) {
		ret = -EPERM;
		tloge("send ttf mem info failed. ret = 0x%x\n", smc_cmd.ret_val);
	}
	mailbox_free(mb_pack);

	return ret;
}

static int32_t copy_tui_font_file(size_t font_file_size, const void *font_virt_addr)
{
	struct file *filep = NULL;
	mm_segment_t old_fs;
	loff_t pos = 0;
	unsigned int count;
	int ret = 0;

	if (font_virt_addr == NULL) {
		return -1;
	}

	filep = filp_open(TTF_NORMAL_FILE_PATH, O_RDONLY, 0);
	if (IS_ERR(filep) || filep == NULL) {
		tloge("Failed to open ttf file\n");
		return -1;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	count = (unsigned int)vfs_read(filep, (char __user *)font_virt_addr, font_file_size, &pos);

	if (font_file_size != count) {
		tloge("read ttf file failed\n");
		ret = -1;
	}

	set_fs(old_fs);
	filp_close(filep, 0);
	filep = NULL;
	return ret;
}

static int32_t send_ttf_mem(tui_ion_mem *tui_ttf_mem)
{
	int ret;
	size_t tui_font_file_size;
	bool check_params = false;

	tui_font_file_size = get_tui_font_file_size();
	check_params = (tui_font_file_size == 0) || (tui_font_file_size > tui_ttf_mem->len);
	if (check_params) {
		tloge("Failed to get the tui font file size or the tui_font_file_size is too big\n");
		return -1;
	}

	ret = copy_tui_font_file(tui_font_file_size, tui_ttf_mem->tui_ion_virt_addr);
	if (ret < 0) {
		tloge("Failed to do ttf file copy\n");
		return -1;
	}

	__dma_map_area(tui_ttf_mem->tui_ion_virt_addr, tui_ttf_mem->len, DMA_BIDIRECTIONAL);
	__dma_map_area(tui_ttf_mem->tui_ion_virt_addr, tui_ttf_mem->len, DMA_FROM_DEVICE);

	ret = tc_ns_register_tui_font_mem(tui_ttf_mem, tui_font_file_size);
	if (ret != 0) {
		tloge("Failed to do ttf file register ret is 0x%x\n", ret);
		return -1;
	}

	return 0;
}

static int32_t load_tui_font_file(void)
{
	int ret = 0;
	tui_ion_mem *tui_ttf_mem = NULL;

	tloge("====load ttf start =====\n");

	mutex_lock(&tui_drv_lock);
	if (normal_load_flag) {
		tloge("normal tui font file has been loaded\n");
		mutex_unlock(&tui_drv_lock);
		return 0;
	}

	normal_font_mem.len = TTF_NORMAL_BUFF_SIZE;
	ret = get_tui_font_mem(&normal_font_mem);
	tui_ttf_mem = &normal_font_mem;
	if (ret) {
		tloge("Failed to get tui font memory\n");
		mutex_unlock(&tui_drv_lock);
		return -1;
	}

	if (get_tui_ion_sglist(tui_ttf_mem) != 0) {
		tloge("get tui sglist failed\n");
		free_tui_font_mem();
		mutex_unlock(&tui_drv_lock);
		return -1;
	}

	ret = send_ttf_mem(tui_ttf_mem);
	if (ret != 0) {
		mailbox_free(phys_to_virt(tui_ttf_mem->tui_ion_phys_addr));
		free_tui_font_mem();
		mutex_unlock(&tui_drv_lock);
		return -1;
	}

	tloge("normal tui font file loaded\n");
	normal_load_flag = true;
	mutex_unlock(&tui_drv_lock);

	mailbox_free(phys_to_virt(tui_ttf_mem->tui_ion_phys_addr));
	tloge("=====load ttf end=====\n");
	return ret;
}

int register_tui_driver(tui_drv_init fun, const char *name,
			void *pdata)
{
	struct tui_drv_node *tui_drv = NULL;
	struct tui_drv_node *pos = NULL;
	int rc;
	int sz_drv_node;

	/* Return error if name is invalid */
	if (name == NULL || fun == NULL) {
		tloge("name or func is null");
		return -EINVAL;
	}

	if (!strncmp(name, TUI_DSS_NAME, (size_t)TUI_DRV_NAME_MAX)) {
		if (pdata == NULL) {
			return -1;
		} else {
			dss_fd = (struct hisi_fb_data_type *)pdata;
		}
	}

	if (!strncmp(name, TUI_TP_NAME, (size_t)TUI_DRV_NAME_MAX) && NULL == pdata)
		return -1;

	mutex_lock(&tui_drv_lock);

	/* name should not have been registered */
	list_for_each_entry(pos, &tui_drv_head, list) {
		if (!strncmp(pos->name, name, TUI_DRV_NAME_MAX - 1)) {
			tloge("this drv(%s) have registered\n", name);
			mutex_unlock(&tui_drv_lock);
			return -EINVAL;
		}
	}
	mutex_unlock(&tui_drv_lock);

	/* Allocate memory for tui_drv */
	tui_drv = kzalloc(sizeof(struct tui_drv_node), GFP_KERNEL);
	if (tui_drv == NULL) {
		return -ENOMEM;
	}

	sz_drv_node = sizeof(struct tui_drv_node);
	if (memset_s(tui_drv, sz_drv_node, 0, sz_drv_node)) {
		tloge("tui_drv memset failed");
		kfree(tui_drv);
		return -1;
	}

	/* Assign content for tui_drv */
	tui_drv->init_func = fun;
	tui_drv->pdata = pdata;

	rc = strncpy_s(tui_drv->name, TUI_DRV_NAME_MAX, name, TUI_DRV_NAME_MAX - 1);
	if (rc != 0) {
		tloge("strncpy_s error\n");
		kfree(tui_drv);
		return -1;
	}

	INIT_LIST_HEAD(&tui_drv->list);

	/* Link the new tui_drv to the list */
	mutex_lock(&tui_drv_lock);
	list_add_tail(&tui_drv->list, &tui_drv_head);
	mutex_unlock(&tui_drv_lock);

	return 0;
}
EXPORT_SYMBOL(register_tui_driver);

void unregister_tui_driver(const char *name)
{
	struct tui_drv_node *pos = NULL, *tmp = NULL;

	/* Return error if name is invalid */
	if (name == NULL) {
		tloge("name is null");
		return;
	}

	mutex_lock(&tui_drv_lock);
	list_for_each_entry_safe(pos, tmp, &tui_drv_head, list) {
		if (!strncmp(pos->name, name, TUI_DRV_NAME_MAX)) {
			list_del(&pos->list);
			kfree(pos);
			break;
		}
	}
	mutex_unlock(&tui_drv_lock);
}
EXPORT_SYMBOL(unregister_tui_driver);

static int add_tui_msg(int type, int val, void *data)
{
	struct tui_msg_node *tui_msg = NULL;
	unsigned long flags;

	/* Return error if pdata is invalid */
	if (data == NULL) {
		tloge("data is null");
		return -EINVAL;
	}

	/* Allocate memory for tui_msg */
	tui_msg = kzalloc(sizeof(*tui_msg), GFP_KERNEL);
	if (tui_msg == NULL) {
		return -ENOMEM;
	}

	if (memset_s(tui_msg, sizeof(*tui_msg), 0, sizeof(*tui_msg))){
		tloge("tui_msg memset failed");
		kfree(tui_msg);
		return -1;
	}

	/* Assign the content of tui_msg */
	tui_msg->type = type;
	tui_msg->val = val;
	tui_msg->data = data;
	INIT_LIST_HEAD(&tui_msg->list);

	/* Link the new tui_msg to the list */
	spin_lock_irqsave(&tui_msg_lock, flags);
	list_add_tail(&tui_msg->list, &tui_msg_head);
	tui_msg_flag = 1;
	spin_unlock_irqrestore(&tui_msg_lock, flags);

	return 0;
}
/*WARNING: Too many leading tabs - consider code refactoring*/
/* secure : 0-unsecure, 1-secure */
static int init_tui_driver(int secure)
{
	struct tui_drv_node *pos = NULL;
	char *drv_name = NULL;
	char **drv_array = g_deinit_driver;
	int count = 0;
	int i = 0;
	if (dss_fd == NULL) {
		return -1;
	}
	if (secure) {
		drv_array = g_init_driver;
	}
	while (i < DRIVER_NUM) {
		drv_name = drv_array[i];
		tlogd("drv-name is %s\n", drv_name);
		i++;
		mutex_lock(&tui_drv_lock);

		/* Search all the tui_drv in their list */
		list_for_each_entry(pos, &tui_drv_head, list) {
			if (!strncmp(drv_name, pos->name, TUI_DRV_NAME_MAX)) {
				/* The names match. */

				// cppcheck-suppress *
				if (!strncmp(TUI_TP_NAME, pos->name, TUI_DRV_NAME_MAX)) {
					/* If the name is "tp", assign pos->pdata to g_tui_ctl */
					g_tui_ctl->n2s.tp_info = (int)virt_to_phys(pos->pdata);
					g_tui_ctl->n2s.tp_info_h_addr = virt_to_phys(pos->pdata) >> 32;
				}
				if (pos->init_func) {
					// cppcheck-suppress *
					if (!strncmp(TUI_DSS_NAME, pos->name,
						     TUI_DRV_NAME_MAX) && secure) {
						tloge("init_tui_driver wait power on status---\n");
						while (!dss_fd->panel_power_on &&
						       count < TIME_OUT_FOWER_ON) {
							count++;
							msleep(20);
						}
						if (TIME_OUT_FOWER_ON == count) {
							/* Time out. So return error. */
							mutex_unlock(&tui_drv_lock);
							tloge("wait status time out\n");
							return -1;
						}
						spin_lock(&tui_msg_lock);
						tui_msg_del(TUI_DSS_NAME);
						spin_unlock(&tui_msg_lock);
					}
					if (!secure) {
						tlogd("drv(%s) state=%d,%d\n",
							pos->name, secure, pos->state);
						if (0 == pos->state)
							continue;
						if (pos->init_func(pos->pdata, secure)) {
							/* Process init_func() fail */
							pos->state = -1;
						}
						/* set secure state will be proceed in tui msg */
						pos->state = 0;
					} else {
						tlogd("init tui drv(%s) state=%d\n",
							pos->name, secure);
						/*when init, tp and dss should be async*/
						if (pos->init_func(pos->pdata, secure)) {
							pos->state = -1;
							mutex_unlock(&tui_drv_lock);
							return -1;
						} else {
							// cppcheck-suppress *
							if (strncmp(TUI_DSS_NAME, pos->name, TUI_DRV_NAME_MAX))
								pos->state = 1;
						}
					}
				}
			}
		}
		mutex_unlock(&tui_drv_lock);
	}

	return 0;
}

/* Only after all drivers cfg ok or some one failed, it need
 * to add_tui_msg.
 * ret val:  1 - all cfg ok
 *			 0 - cfg is not complete, or have done
 *			-1 - cfg failed
 *			-2 - invalid name
 */
static int tui_cfg_filter(const char *name, bool ok)
{
	struct tui_drv_node *pos = NULL;
	char find = 0;
	int lock_flag = 0;

	/* Return error if name is invalid */
	if (name == NULL) {
		tloge("name is null");
		return -2;
	}

	/* some drivers may call send_tui_msg_config at the end
	 * of drv_init_func which had got the lock.
	 */
	if (mutex_is_locked(&tui_drv_lock))
		lock_flag = 1;
	if (!lock_flag)
		mutex_lock(&tui_drv_lock);
	list_for_each_entry(pos, &tui_drv_head, list) {
		if (!strncmp(pos->name, name, TUI_DRV_NAME_MAX)) {
			find = 1;
			if (ok)
				pos->state = 1;
			else {
				if (!lock_flag)
					mutex_unlock(&tui_drv_lock);
				return -1;
			}
		}
	}
	if (!lock_flag)
		mutex_unlock(&tui_drv_lock);

	if (!find)
		return -2;

	return 1;
}

enum poll_class {
	CLASS_POLL_CONFIG,
	CLASS_POLL_RUNNING,
	CLASS_POLL_COMMON,
	CLASS_POLL_INVALID
};

static inline enum poll_class tui_poll_class(int event_type)
{
	enum poll_class class = CLASS_POLL_INVALID;

	switch (event_type) {
	case TUI_POLL_CFG_OK:
	case TUI_POLL_CFG_FAIL:
	case TUI_POLL_RESUME_TUI:
		class = CLASS_POLL_CONFIG;
		break;
	case TUI_POLL_TP:
	case TUI_POLL_TICK:
	case TUI_POLL_DELAYED_WORK:
		class = CLASS_POLL_RUNNING;
		break;
	case TUI_POLL_CANCEL:
		class = CLASS_POLL_COMMON;
		break;
	default:
		break;
	}
	return class;
}

int send_tui_msg_config(int type, int val, void *data)
{
	int ret;

	if (type >= TUI_POLL_MAX  || type < 0 || data == NULL) {
		tloge("invalid tui event type\n");
		return -EINVAL;
	}

	/* The tui_state should be CONFIG */
	if (atomic_read(&tui_state) != TUI_STATE_CONFIG) {
		tloge("failed to send tui msg(%s)\n", poll_event_type_name[type]);
		return -EINVAL;
	}

	if (CLASS_POLL_RUNNING == tui_poll_class(type)) {
		tloge("invalid tui event type(%s) in config state\n", poll_event_type_name[type]);
		return -EINVAL;
	}

	tlogd("send config event type %s(%s)\n", poll_event_type_name[type], (char *)data);

	if (TUI_POLL_CFG_OK == type || TUI_POLL_CFG_FAIL == type) {
		int cfg_ret;

		cfg_ret = tui_cfg_filter((const char *)data,
					 TUI_POLL_CFG_OK == type);
		tlogd("tui driver(%s) cfg ret = %d\n", (char *)data, cfg_ret);
		if (-2 == cfg_ret) {
			tloge("tui cfg filter failed, cfg_ret = %d\n", cfg_ret);
			return -EINVAL;
		}
	}

	ret = add_tui_msg(type, val, data);
	if (ret) {
		tloge("add tui msg ret=%d\n", ret);
		return ret;
	}

	tlogd("add config msg type %s\n", poll_event_type_name[type]);

	/* wake up tui kthread */
	wake_up(&tui_msg_wq);

	return 0;
}

#define MAKE32(high, low) ((((uint32_t)(high)) << 16) | (uint16_t)(low))

static bool package_notch_msg(struct mb_cmd_pack *mb_pack, uint8_t **buf_to_tee,
			      struct teec_tui_parameter *tui_param)
{
	uint32_t buf_len = sizeof(*tui_param) - sizeof(tui_param->event_type);
	*buf_to_tee = mailbox_alloc(buf_len, 0);
	if (*buf_to_tee == NULL) {
		tloge("failed to alloc memory!\n");
		return false;
	}
	if (memcpy_s(*buf_to_tee, buf_len, &tui_param->value,
		     sizeof(*tui_param) - sizeof(tui_param->event_type)) !=
	    EOK) {
		tloge("copy notch data failed");
		mailbox_free(*buf_to_tee);
		return false;
	}
	mb_pack->operation.paramtypes =
		TEE_PARAM_TYPE_VALUE_INPUT |
		(TEE_PARAM_TYPE_VALUE_INPUT << TEE_PARAM_NUM);
	mb_pack->operation.params[0].value.a =
		(uint32_t)virt_to_phys(*buf_to_tee);
	mb_pack->operation.params[0].value.b =
		(uint32_t)(virt_to_phys(*buf_to_tee) >> ADDR_TRANS_NUM);
	mb_pack->operation.params[1].value.a = buf_len;
	return true;
}
/* Send tui event by smc_cmd */
int tui_send_event(int event, struct teec_tui_parameter *tui_param)
{
	int status_temp;
	bool check_value = false;
	uint8_t *buf_to_tee = NULL;
	if (dss_fd == NULL || tui_param == NULL) {
		return -1;
	}
	status_temp = atomic_read(&tui_state);
	check_value = (status_temp != TUI_STATE_UNUSED && dss_fd->panel_power_on) || TUI_POLL_NOTCH == event || TUI_POLL_FOLD == event;
	if (check_value == true) {
		struct tc_ns_smc_cmd smc_cmd = { {0}, 0 };
		uint32_t uid;
		struct mb_cmd_pack *mb_pack = NULL;
		int ret = 0;
		unsigned int smc_ret = 0;
		kuid_t kuid;

		kuid = current_uid();
		uid = kuid.val;

		if (uid > 1000 || ((TUI_POLL_CANCEL != event) && (TUI_POLL_NOTCH != event) && (TUI_POLL_FOLD != event))) {
			tloge("no permission to send msg\n");
			return -1;
		}

		mb_pack = mailbox_alloc_cmd_pack();
		if (mb_pack == NULL) {
			tloge("alloc cmd pack failed\n");
			return -1;
		}


		smc_cmd.global_cmd = true;
		smc_cmd.operation_phys = virt_to_phys(&mb_pack->operation);
		smc_cmd.operation_h_phys = virt_to_phys(&mb_pack->operation) >> 32;
		switch(event){
			case TUI_POLL_CANCEL:
				smc_cmd.cmd_id = GLOBAL_CMD_ID_TUI_EXCEPTION;
				break;
			case TUI_POLL_NOTCH:
				if (!package_notch_msg(mb_pack, &buf_to_tee,
				    tui_param)) {
					mailbox_free(mb_pack);
					return -1;
				}
				smc_cmd.cmd_id = GLOBAL_CMD_ID_TUI_NOTCH;
				break;
			case TUI_POLL_FOLD:
				mb_pack->operation.paramtypes =
					teec_param_types(
						TEE_PARAM_TYPE_VALUE_INPUT,
						TEE_PARAM_TYPE_VALUE_INPUT,
						TEE_PARAM_TYPE_VALUE_INPUT,
						TEE_PARAM_TYPE_VALUE_INPUT);
				mb_pack->operation.params[0].value.a =
					tui_param->notch;
				mb_pack->operation.params[0].value.b =
					MAKE32(dss_fd->panel_info.xres,
						dss_fd->panel_info.yres);
				mb_pack->operation.params[1].value.a = tui_param->phy_width;
				mb_pack->operation.params[1].value.b = tui_param->phy_height;
				mb_pack->operation.params[2].value.a = tui_param->width;
				mb_pack->operation.params[2].value.b = tui_param->height;
				mb_pack->operation.params[3].value.a = tui_param->fold_state;
				mb_pack->operation.params[3].value.b = tui_param->display_state;
				smc_cmd.cmd_id = GLOBAL_CMD_ID_TUI_FOLD;
				break;
			default:
				tloge("invalid event type : %d\n",event);
				break;
		}
		smc_cmd.agent_id = event;
		smc_cmd.uid = uid;

		smc_ret = (unsigned int)tc_ns_smc(&smc_cmd);
		if (smc_ret != 0) {
			tloge("call smc failed: 0x%x\n", smc_ret);
			ret = -1;
		}
		mailbox_free(mb_pack);
		mailbox_free(buf_to_tee);

		return ret;
	} else {
		tlogd("tui unused no need send tui event!\n");
		return 0;
	}
}

static void tui_poweroff_work_func(struct work_struct *work)
{
	struct teec_tui_parameter tui_param = {0};
	tui_send_event(TUI_POLL_CANCEL, &tui_param);
}

void tui_poweroff_work_start(void)
{
	tlogd("tui_poweroff_work_start----------\n");
	if (dss_fd == NULL) {
		return;
	}
	if (atomic_read(&tui_state) != TUI_STATE_UNUSED
		&& dss_fd->panel_power_on) {
		tlogd("come in tui_poweroff_work_start state=%d--\n",
		atomic_read(&tui_state));
		queue_work(system_wq, &tui_poweroff_work.work);
	}
}

static void wait_tui_msg(void)
{
	if (wait_event_interruptible(tui_msg_wq, tui_msg_flag))
		tloge("get tui state is interrupted\n");
}

static int valid_msg(int msg_type)
{
	switch (msg_type) {
	case TUI_POLL_RESUME_TUI:
		if (TUI_STATE_RUNNING == atomic_read(&tui_state))
			return 0;
		break;
	case TUI_POLL_CANCEL:
		if (TUI_STATE_UNUSED == atomic_read(&tui_state))
			return 0;
		break;
	default:
		break;
	}

	return 1;
}
/*
 * 1: init ok
 * 0: still do init
 * -1: init failed
 */
static int get_cfg_state(const char *name)
{
	const struct tui_msg_node *tui_msg = NULL;

	/* Return error if name is invalid */
	if (name == NULL) {
		tloge("name is null");
		return -1;
	}

	list_for_each_entry(tui_msg, &tui_msg_head, list) {
		/* Names match */
		if (!strncmp(tui_msg->data, name, TUI_DRV_NAME_MAX)) {
			if (TUI_POLL_CFG_OK == tui_msg->type) {
				return 1;
			} else if (TUI_POLL_CFG_FAIL == tui_msg->type) {
				return -1;
			} else {
				tloge("other state\n");
			}
		}
	}

	return 0;
}
static void tui_msg_del(const char *name)
{
	struct tui_msg_node *tui_msg = NULL, *tmp = NULL;

	/* Return error if name is invalid */
	if (name == NULL) {
		tloge("name is null");
		return;
	}

	list_for_each_entry_safe(tui_msg, tmp, &tui_msg_head, list) {
		/* Names match */
		if (!strncmp(tui_msg->data, name, TUI_DRV_NAME_MAX)) {
			list_del(&tui_msg->list);
			kfree(tui_msg);
		}
	}
}
#define DSS_CONFIG_INDEX (1)
#define TP_CONFIG_INDEX (2)
static void process_tui_msg(void)
{
	int val = 0;
	int type = TUI_POLL_CFG_OK;

fetch_msg:
	spin_lock(&tui_msg_lock);
	if (DSS_CONFIG_INDEX == g_tui_ctl->s2n.value) {
#if ONLY_INIT_TP != DSS_TP_COUPLE_MODE
		/* Wait, until DSS init finishs */
		while (0 == get_cfg_state(TUI_DSS_NAME)) {
			tlogd("waiting for dss tui msg\n");
			tui_msg_flag = 0;
			spin_unlock(&tui_msg_lock);
			wait_tui_msg();
			tlogd("get dss init ok tui msg\n");
			spin_lock(&tui_msg_lock);
		}
		if (-1 == get_cfg_state(TUI_DSS_NAME)) {
			tloge("dss init failed\n");
			type = TUI_POLL_CFG_FAIL;
		}
		/* Delete DSS msg from tui_msg_head */
		tui_msg_del(TUI_DSS_NAME);
#endif

	} else if (TP_CONFIG_INDEX == g_tui_ctl->s2n.value) {
#if ONLY_INIT_DSS != DSS_TP_COUPLE_MODE
		while (0 == get_cfg_state(TUI_TP_NAME)) {
			tlogd("waiting for tp tui msg\n");
			tui_msg_flag = 0;
			spin_unlock(&tui_msg_lock);
			wait_tui_msg();
			tlogd("get tp init ok tui msg\n");
			spin_lock(&tui_msg_lock);
		}
		if (-1 == get_cfg_state(TUI_TP_NAME)) {
			tloge("tp failed to do init\n");
			type = TUI_POLL_CFG_FAIL;
			tui_msg_del(TUI_TP_NAME);
			goto next;
		}
		tui_msg_del(TUI_TP_NAME);
		tlogd("tp/gpio/fp is config result:type = 0x%x\n", type);
#endif
	} else {
		tloge("wait others dev\n");
	}
next:
	spin_unlock(&tui_msg_lock);

	/* pre-process tui poll event if needed */
	switch (type) {
	case TUI_POLL_CFG_OK:
		if (DSS_CONFIG_INDEX == g_tui_ctl->s2n.value) {


			phys_addr_t tui_addr_t;
			tui_addr_t = get_frame_addr();
			if (0 == tui_addr_t) {
				tloge("get frame addr error\n");
			}
			g_tui_ctl->n2s.addr = (unsigned int)tui_addr_t;
			g_tui_ctl->n2s.addr_h = tui_addr_t >> 32;
			g_tui_ctl->n2s.npages = tui_display_mem.npages;
			g_tui_ctl->n2s.info_length = tui_display_mem.info_length;
			g_tui_ctl->n2s.x = tui_display_mem.len;
			if (0 == g_tui_ctl->n2s.addr)
				val = -1;
		}
		break;
	default:
		break;
	}

	g_tui_ctl->n2s.event_type = type;
	g_tui_ctl->n2s.value = val;

	if (!valid_msg(g_tui_ctl->n2s.event_type)) {
		tlogd("refetch tui msg\n");
		goto fetch_msg;
	}
}

static int init_tui_agent(void)
{
	int ret;

	ret = tc_ns_register_agent(NULL, TEE_TUI_AGENT_ID, SZ_4K, (void **)(&g_tui_ctl), false);
	if (ret) {
		tloge("register tui agent failed, ret = 0x%x\n", ret);
		g_tui_ctl = NULL;
		return -EFAULT;
	}

	return 0;
}

static void exit_tui_agent(void)
{
	if (tc_ns_unregister_agent(TEE_TUI_AGENT_ID))
		tloge("unregister tui agent failed\n");

	g_tui_ctl = NULL;
}

static void set_tui_state(int state)
{
	if (state < TUI_STATE_UNUSED || state > TUI_STATE_ERROR) {
		tloge("state=%d is invalid\n",state);
		return;
	}
	if (atomic_read(&tui_state) != state) {
		atomic_set(&tui_state, state);
		tloge("set ree tui state is %d, 0: unused, 1:config , 2:running\n", state);
		tui_state_flag = 1;
		wake_up(&tui_state_wq);
	}
}

int is_tui_in_use(int pid_value)
{
	if (pid_value == atomic_read(&g_tui_pid))
		return 1;
	return 0;
}

void free_tui_caller_info()
{
	atomic_set(&g_tui_attached_device, TUI_PID_CLEAR);
	atomic_set(&g_tui_pid, TUI_PID_CLEAR);
}

static int agent_process_work_tui(void)
{
	struct smc_event_data *event_data = NULL;

	event_data = find_event_control(TEE_TUI_AGENT_ID);
	if (event_data == NULL || atomic_read(&event_data->agent_ready) == AGENT_CRASHED) {
		/* if return, the pending task in S can't be resumed!! */
		tloge("tui agent is not exist\n");
		put_agent_event(event_data);
		return TEEC_ERROR_GENERIC;
	}

	isb();
	wmb();
	event_data->ret_flag = 1;
	/* Wake up tui agent that will process the command */
	wake_up(&event_data->wait_event_wq);

	tlogi("agent 0x%x request, goto sleep, pe->run=%d\n",
		TEE_TUI_AGENT_ID, atomic_read(&event_data->ca_run));
	wait_event(event_data->ca_pending_wq, atomic_read(&event_data->ca_run));
	atomic_set(&event_data->ca_run, 0);
	put_agent_event(event_data);

	return TEEC_SUCCESS;
}

void do_ns_tui_release(void)
{
	if (atomic_read(&tui_state) != TUI_STATE_UNUSED) {
		g_tui_ctl->s2n.command = TUI_CMD_EXIT;
		g_tui_ctl->s2n.ret = -1;
		tloge("exec tui do_ns_tui_release\n");
		if (agent_process_work_tui())
			tloge("wake up tui agent error\n");
	}
}
static int32_t do_tui_ttf_work(void)
{
	int ret = 0;
	switch (g_tui_ctl->s2n.command) {
	case TUI_CMD_LOAD_TTF:
		ret = load_tui_font_file();
		if (ret == 0) {
			tlogd("=======suceed to load ttf\n");
			g_tui_ctl->n2s.event_type = TUI_POLL_CFG_OK;
		} else {
			tloge("Failed to load normal ttf ret is 0x%x\n", ret);
			g_tui_ctl->n2s.event_type = TUI_POLL_CFG_FAIL;
		}
		break;
	case TUI_CMD_EXIT:
		if (atomic_read(&tui_state) != TUI_STATE_UNUSED &&
		    atomic_dec_and_test(&tui_usage)) {
			tlogd("tui disable\n");
			(void)init_tui_driver(0);
			free_frame_addr();
			free_tui_font_mem();
			free_tui_caller_info();
			set_tui_state(TUI_STATE_UNUSED);
		}
		break;
	case TUI_CMD_FREE_TTF_MEM:
		free_tui_font_mem();
		ret = 0;
		break;
	default:
		ret = -EINVAL;
		tloge("get error ttf tui command 0x%x\n", g_tui_ctl->s2n.command);
		break;
	}
	return ret;
}
static int do_tui_config_work(void)
{
	int ret = 0;

	switch (g_tui_ctl->s2n.command) {
	case TUI_CMD_ENABLE:
		if (atomic_read(&tui_state) != TUI_STATE_CONFIG) {
			tlogd("tui enable\n");
			set_tui_state(TUI_STATE_CONFIG);
			/*do dss and tp init*/
			if (init_tui_driver(1)) {
				g_tui_ctl->s2n.ret = -1;
				set_tui_state(TUI_STATE_ERROR);
				(void)init_tui_driver(0);
				free_tui_caller_info();
				set_tui_state(TUI_STATE_UNUSED);
				break;
			}
			atomic_inc(&tui_usage);
		}
		break;
	case TUI_CMD_DISABLE:
		if (atomic_read(&tui_state) != TUI_STATE_UNUSED &&
			atomic_dec_and_test(&tui_usage)) {
			tlogd("tui disable\n");
			(void)init_tui_driver(0);
			free_frame_addr();
			free_tui_caller_info();
			set_tui_state(TUI_STATE_UNUSED);
		}
		break;
	case TUI_CMD_PAUSE:
		if (atomic_read(&tui_state) != TUI_STATE_UNUSED) {
			tlogd("tui pause\n");
			(void)init_tui_driver(0);
			set_tui_state(TUI_STATE_CONFIG);
		}
		break;
	case TUI_CMD_POLL:
		process_tui_msg();
		break;
	case TUI_CMD_DO_SYNC:
		tlogd("enable tp irq cmd\n");
		break;
	case TUI_CMD_SET_STATE:
		tlogd("tui set state %d\n", g_tui_ctl->s2n.value);
		set_tui_state(g_tui_ctl->s2n.value);
		break;
	case TUI_CMD_START_DELAY_WORK:
		tlogd("start delay work\n");
		break;
	case TUI_CMD_CANCEL_DELAY_WORK:
		tlogd("cancel delay work\n");
		break;
	default:
		ret = -EINVAL;
		tloge("get error config tui command(0x%x)\n", g_tui_ctl->s2n.command);
		break;
	}
	return ret;
}
static int do_tui_work(void)
{
	int ret = 0;

	/* clear s2n cmd ret */
	g_tui_ctl->s2n.ret = 0;
	switch (g_tui_ctl->s2n.command) {
	case TUI_CMD_ENABLE:
	case TUI_CMD_DISABLE:
	case TUI_CMD_PAUSE:
	case TUI_CMD_POLL:
	case TUI_CMD_DO_SYNC:
	case TUI_CMD_SET_STATE:
	case TUI_CMD_START_DELAY_WORK:
	case TUI_CMD_CANCEL_DELAY_WORK:
		ret = do_tui_config_work();
		break;
	case TUI_CMD_LOAD_TTF:
	case TUI_CMD_EXIT:
	case TUI_CMD_FREE_TTF_MEM:
		ret = do_tui_ttf_work();
		break;
	default:
		ret = -EINVAL;
		tloge("get error tui command\n");
		break;
	}
	return ret;
}

void set_tui_caller_info(unsigned int devid, int pid)
{
	atomic_set(&g_tui_attached_device, (int)devid);
	atomic_set(&g_tui_pid, pid);
}

unsigned int tui_attach_device(void)
{
	return (unsigned int)atomic_read(&g_tui_attached_device);
}

static int tui_kthread_work_fn(void *data)
{
	int ret;
	ret = init_tui_agent();
	if (ret) {
		tloge("init tui agent error, ret = %d\n", ret);
		return ret;
	}

	while (1) {
		tc_ns_wait_event(TEE_TUI_AGENT_ID);

		if (kthread_should_stop())
			break;

		do_tui_work();

		if (tc_ns_send_event_response(TEE_TUI_AGENT_ID))
			tloge("send event response error\n");
	}

	exit_tui_agent();

	return 0;
}

#define READ_BUF 128
static ssize_t tui_dbg_state_read(struct file *filp,char __user *ubuf,
				  size_t cnt, loff_t *ppos)
{
	char buf[READ_BUF] = {0};
	unsigned int r;
	int ret;
	struct tui_drv_node *pos = NULL;

	if (filp == NULL || ubuf == NULL || ppos == NULL)
		return -EINVAL;

	ret = snprintf_s(buf, READ_BUF, READ_BUF - 1, "tui state:%s\n",
			 state_name[atomic_read(&tui_state)]);
	if (ret < 0) {
		tloge("tui dbg state read 1 snprintf is faile, ret = 0x%x\n", ret);
		return -EINVAL;
	}
	r = (unsigned int)ret;

	ret = snprintf_s(buf + r, READ_BUF - r, READ_BUF - r - 1, "%s", "drv config state:");
	if (ret < 0) {
		tloge("tui dbg state read 2 snprintf is faile, ret = 0x%x\n", ret);
		return -EINVAL;
	}
	r += (unsigned int)ret;

	mutex_lock(&tui_drv_lock);
	list_for_each_entry(pos, &tui_drv_head, list) {
		ret = snprintf_s(buf + r, READ_BUF - r, READ_BUF - r - 1, "%s-%s,", pos->name, 1 == pos->state ? "ok" : "no ok");
		if (ret < 0) {
			tloge("tui dbg state read 3 snprintf is faile, ret = 0x%x\n", ret);
			mutex_unlock(&tui_drv_lock);
			return -EINVAL;
		}
		r += (unsigned int)ret;
	}
	mutex_unlock(&tui_drv_lock);
	if (r < READ_BUF)
		buf[r-1]='\n';

	return simple_read_from_buffer(ubuf, cnt, ppos, buf, r);
}

static const struct file_operations tui_dbg_state_fops = {
	.owner = THIS_MODULE,
	.read = tui_dbg_state_read,
};

static ssize_t tui_status_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	int r;
	size_t buf_len = 0;
	if (kobj == NULL || attr == NULL || buf == NULL) {
		return -EINVAL;
	}
	tui_state_flag = 0;
	r = wait_event_interruptible(tui_state_wq, tui_state_flag);
	if (r) {
		tloge("get tui state is interrupted\n");
		return r;
	}
	buf_len = 32;
	r = snprintf_s(buf, buf_len, buf_len - 1, "%s", state_name[atomic_read(&tui_state)]);
	if (r < 0) {
		tloge("tui statue show snprintf is faile, ret = 0x%x\n", r);
		return -1;
	}

	return r;
}

#define MSG_BUF 512
static ssize_t tui_dbg_msg_read(struct file *filp, char __user *ubuf,
				size_t cnt, loff_t *ppos)
{
	char buf[MSG_BUF] = {0};
	unsigned int r;
	int ret;
	int i;
	struct tui_drv_node *pos = NULL;

	if (filp == NULL || ubuf == NULL || ppos == NULL) {
		return -EINVAL;
	}

	ret = snprintf_s(buf, MSG_BUF, MSG_BUF - 1, "%s", "event format: event_type:val\n"
			 "event type:\n");
	if (ret < 0) {
		tloge("tui db msg read 1 snprint is error, ret = 0x%x\n", ret);
		return -EINVAL;
	}
	r = (unsigned int)ret;

	/* event type list */
	for (i = 0; i < TUI_POLL_MAX - 1; i++) {
		ret = snprintf_s(buf + r, MSG_BUF - r, MSG_BUF - r - 1, "%s, ",
				 poll_event_type_name[i]);
		if (ret < 0) {
			tloge("tui db msg read 2 snprint is error, ret = 0x%x\n", ret);
			return -EINVAL;
		}
		r += (unsigned int)ret;
	}
	// cppcheck-suppress *
	ret = snprintf_s(buf + r, MSG_BUF - r, MSG_BUF - r - 1, "%s\n", poll_event_type_name[i]);
	if (ret < 0) {
		tloge("tui db msg read 3 snprint is error, ret = 0x%x\n", ret);
		return -EINVAL;
	}
	r += (unsigned int)ret;

	/* cfg drv type list */
	ret = snprintf_s(buf + r, MSG_BUF - r, MSG_BUF - r - 1, "val type for %s or %s:\n",
			poll_event_type_name[TUI_POLL_CFG_OK],
			poll_event_type_name[TUI_POLL_CFG_FAIL]);
	if (ret < 0) {
		tloge("tui db msg read 4 snprint is error, ret = 0x%x\n", ret);
		return -EINVAL;
	}
	r += (unsigned int)ret;

	mutex_lock(&tui_drv_lock);
	list_for_each_entry(pos, &tui_drv_head, list) {
		ret = snprintf_s(buf + r, MSG_BUF - r, MSG_BUF - r - 1, "%s,", pos->name);
		if (ret < 0) {
			tloge("tui db msg read 5 snprint is error, ret = 0x%x\n", ret);
			mutex_unlock(&tui_drv_lock);
			return -EINVAL;
		}
		r += (unsigned int)ret;
	}
	mutex_unlock(&tui_drv_lock);
	if (r < MSG_BUF)
		buf[r - 1] = '\n';

	return simple_read_from_buffer(ubuf, cnt, ppos, buf, r);
}

static ssize_t tui_dbg_msg_write(struct file *filp,
				const char __user *ubuf, size_t cnt,
				loff_t *ppos)
{
	char buf[64];
	int i;
	int event_type = -1;
	char *tokens = NULL, *begins = NULL;
	int ret;
	struct teec_tui_parameter tui_param = {0};

	if (ubuf == NULL || filp == NULL || ppos == NULL) {
		return -EINVAL;
	}

	if (cnt >= sizeof(buf)/sizeof(char))
		return -EINVAL;

	if (copy_from_user(buf, ubuf, cnt))
		return -EFAULT;

	buf[cnt] = 0;

	begins = buf;

	/* event type */
	tokens = strsep(&begins, ":");
	if (tokens == NULL) {
		return -EFAULT;
	}

	tlogd("1: tokens:%s\n", tokens);
	for (i = 0; i < TUI_POLL_MAX; i++) {
		// cppcheck-suppress *
		if (!strncmp(tokens, poll_event_type_name[i], strlen(poll_event_type_name[i]))) {
			event_type = i;
			break;
		}
	}

	/* only for tp and cancel  */
	if (event_type != TUI_POLL_TP  && event_type != TUI_POLL_CANCEL)
		return -EFAULT;
	/* drv type */
	tokens = strsep(&begins, ":");
	if (tokens == NULL) {
		return -EFAULT;
	}
	tlogd("2: tokens:%s\n", tokens);
	if (TUI_POLL_TP == event_type) {
		long value = 0;
		int base = 10;

		/* simple_strtol is obsolete, use kstrtol instead*/
		ret = kstrtol(tokens, base, &value);
		if (ret)
			return -EFAULT;
		g_tui_ctl->n2s.status = (int)value;

		tokens = strsep(&begins, ":");
		if (tokens == NULL) {
			return -EFAULT;
		}
		ret = kstrtol(tokens, base, &value);
		if (ret)
			return -EFAULT;
		g_tui_ctl->n2s.x = (int)value;

		tokens = strsep(&begins, ":");
		if (tokens == NULL) {
			return -EFAULT;
		}
		ret = kstrtol(tokens, base, &value);
		if (ret)
			return -EFAULT;
		g_tui_ctl->n2s.y = (int)value;
	}
	tlogd("status=%d x=%d y=%d\n", g_tui_ctl->n2s.status, g_tui_ctl->n2s.x, g_tui_ctl->n2s.y);

	if (tui_send_event(event_type, &tui_param))
		return -EFAULT;

	*ppos += cnt;

	return cnt;
}

static const struct file_operations tui_dbg_msg_fops = {
	.owner = THIS_MODULE,
	.read = tui_dbg_msg_read,
	.write = tui_dbg_msg_write,
};

static struct dentry *g_dbg_dentry = NULL;


static int tui_powerkey_notifier_call(struct notifier_block *powerkey_nb, unsigned long event, void *data)
{
	if (event == HISI_PRESS_KEY_DOWN) {
		tui_poweroff_work_start();
	}else if (event == HISI_PRESS_KEY_UP) {
	}else if (event == HISI_PRESS_KEY_1S) {
	}else if (event == HISI_PRESS_KEY_6S) {
	} else if (event == HISI_PRESS_KEY_8S) {
	} else if (event == HISI_PRESS_KEY_10S) {
	} else {
		tloge("[%s]invalid event %ld !\n", __func__, event);
	}
	return 0;
}
static struct notifier_block tui_powerkey_nb;
int register_tui_powerkeyListener(void)
{
	tui_powerkey_nb.notifier_call = tui_powerkey_notifier_call;
	return hisi_powerkey_register_notifier(&tui_powerkey_nb);
}
int unregister_tui_powerkeyListener(void)
{
	tui_powerkey_nb.notifier_call = tui_powerkey_notifier_call;
	return hisi_powerkey_unregister_notifier(&tui_powerkey_nb);
}



int __init init_tui(const struct device *class_dev)
{
	int retval;
	struct sched_param param;
	param.sched_priority = MAX_RT_PRIO - 1;

	if (class_dev == NULL) {
		return -1;
	}

	g_tui_task = kthread_create(tui_kthread_work_fn, NULL, "tuid");
	if (IS_ERR_OR_NULL(g_tui_task)) {
		tloge("kthread create is error\n");
		return PTR_ERR(g_tui_task);
	}

	sched_setscheduler_nocheck(g_tui_task, SCHED_FIFO, &param);
	get_task_struct(g_tui_task);

	wake_up_process(g_tui_task);

	INIT_LIST_HEAD(&tui_msg_head);
	spin_lock_init(&tui_msg_lock);

	init_waitqueue_head(&tui_state_wq);
	init_waitqueue_head(&tui_msg_wq);
	g_dbg_dentry = debugfs_create_dir("tui", NULL);
	debugfs_create_file("d_state", 0440, g_dbg_dentry, NULL, &tui_dbg_state_fops);
	g_tui_kobj = kobject_create_and_add("tui", kernel_kobj);
	if (g_tui_kobj == NULL) {
		tloge("tui kobj create error\n");
		retval =  -ENOMEM;
		goto error2;
	}
	retval = sysfs_create_group(g_tui_kobj, &tui_attr_group);

	if (retval) {
		tloge("sysfs_create_group error, retval = 0x%x\n", retval);
		goto error1;
	}

	retval = register_tui_powerkeyListener();
	if (retval != 0) {
		tloge("tui register failed, retval = 0x%x\n", retval);
		goto error1;
	}
	return 0;
error1:
	kobject_put(g_tui_kobj);
error2:
	kthread_stop(g_tui_task);
	return retval;

}

void tui_exit(void)
{
	if(unregister_tui_powerkeyListener() < 0)
		tloge("tui power key unregister failed\n");
	kthread_stop(g_tui_task);
	put_task_struct(g_tui_task);
	debugfs_remove(g_dbg_dentry);
	sysfs_remove_group(g_tui_kobj, &tui_attr_group);
	kobject_put(g_tui_kobj);
}
