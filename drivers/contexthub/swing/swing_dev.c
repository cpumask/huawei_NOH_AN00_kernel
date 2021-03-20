/*
 * Copyright (C) Huawei Tech. Co. Ltd. 2017-2019. All rights reserved.
 * Description: dev drvier to communicate with sensorhub swing app
 * Author: Huawei
 * Create: 2017.12.05
 */
#include "swing_dev.h"
#include <linux/version.h>
#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/kfifo.h>
#include <linux/debugfs.h>
#include <linux/io.h>
#include <linux/syscalls.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/completion.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/hisi/hisi_ion.h>
#include <securec.h>
#include <protocol.h>
#include "hisi_lb.h"
#include "../inputhub_api.h"
#include "../common.h"
#include "../shmem.h"
#include "hisi_bbox_diaginfo.h"
#include "bbox_diaginfo_id_def.h"
#ifdef CONFIG_CONTEXTHUB_SWING_20
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <linux/fb.h>
#endif
#ifdef CONFIG_INPUTHUB_30
#include <linux/hisi/contexthub/iomcu_boot.h>
#endif

#define swing_log_info(msg...) pr_info("[I/SWING]" msg)
#define swing_log_err(msg...) pr_err("[E/SWING]" msg)
#define swing_log_warn(msg...) pr_warn("[W/SWING]" msg)

#ifdef __LLT_UT__
#define STATIC
#else
#define STATIC static
#endif

typedef struct {
	unsigned int recv_len;
	void *p_recv;
} swing_read_data_t;

typedef struct {
	struct device *self;            // self device.
	struct completion swing_wait;
	struct completion read_wait;
	struct mutex read_mutex;        // Used to protect ops on read
	struct mutex swing_mutex;       // Used to protect ops on ioctl & open
	struct kfifo read_kfifo;
	swing_fusion_en_resp_t en_resp;
	swing_fusion_set_resp_t set_resp;
	int ref_cnt;
	int sh_recover_flag;
#ifdef CONFIG_CONTEXTHUB_SWING_20
	swing_fusion_gets_resp_t gets_resp;
	swing_fusion_sets_resp_t sets_resp;
	struct wakeup_source swing_wklock;
	struct mutex wklock_mutex;       // Used to protect swing_wklock op
	struct list_head ion_buf_list;
#endif
	int send_ipc_to_shb;
} swing_priv_t;

#ifdef CONFIG_CONTEXTHUB_SWING_20
struct swing_ion_node {
        struct list_head list;
        swing_ion_info_t ion_info;
};
#endif

typedef struct {
	unsigned int dmd_id;
	unsigned int dmd_para_num;
	const char *dmd_msg;
} swing_dmd_log_t;

#define SWING_READ_CACHE_COUNT          (5)
#define SWING_SYSCAHCE_ID               (8)

#define SWING_IOCTL_WAIT_TMOUT          5000 // ms

#define SWING_RESET_NOTIFY              (0xFFFF)

#define SWING_DMD_CASE_DETAIL           0xFF
#define SWING_DMD_INFO_NUM_MAX          5
#define SWING_DEV_MAX_UPLOAD_LEN        (0x1000)
#define SWING_DEV_MAX_DATA_LEN          (16000)
#define SWING_DMA_MASK                  (0xFFFFFFFFFFFFFFFF)

#define SWING_SEND_SUSPEND_TO_SHB       (1)
#define SWING_SEND_RESUME_TO_SHB        (2)
#define SWING_SCREEN_OFF                (0x5A)
#define SWING_SCREEN_ON                 (0x0)

static unsigned long long g_swing_dmamask = SWING_DMA_MASK;

static swing_dmd_log_t dmd_log[] = {
	{SWING_DMD_FDUL_PW_ON,          3, "FDUL PowerOn Failed: step %u, register 0x %x, value 0x %x."},
	{SWING_DMD_FDUL_PW_OFF,         3, "FDUL PowerOff Failed: step %u, register 0x %x, value 0x %x."},
	{SWING_DMD_HWTS_PW_ON,          4, "HWTS PowerOn Failed: step %u, register 0x %x, value 0x %x %x."},
	{SWING_DMD_HWTS_PW_OFF,         4, "HWTS PowerOff Failed: step %u, register 0x %x, value 0x %x %x."},
	{SWING_DMD_AIC_PW_ON,           3, "TinyCore PowerOn Failed: step %u, register 0x %x, value 0x %x."},
	{SWING_DMD_AIC_PW_OFF,          3, "TinyCore PowerOff Failed: step %u, register 0x %x, value 0x %x."},
	{SWING_DMD_CAM_PW_ON,           2, "Swing cam pw on timeout: cam_state: %d, ao_cam_status: %d."},
	{SWING_DMD_CAM_PW_OFF,          2, "Swing cam pw off timeout: cam_state: %d, ao_cam_status: %d."},
	{SWING_DMD_CAM_IR_PW,           3, "Swing IR pw fail: onoff: %d, ir_status: %d, pw_err_type: %d."},
	{SWING_DMD_CAM_TIMEOUT,         0, "Camera timeout."},
	{SWING_DMD_SLEEP_FUSION,        3, "Fusions open when suspend: num %u, bitmap 0x %x %x."},
	{SWING_DMD_NPU_PLL_RETRY,       3, "NPU_PLL_RETRY: PLL_ID %d Total %d Retry %d."},
};

static swing_priv_t g_swing_priv = { 0 };

static unsigned int *g_swing_avs_data;
static unsigned int *g_swing_pa_margin;
static unsigned int *g_swing_svfd_data;
static unsigned int *g_swing_svfd_para;

static int g_avs_data_num;
static int g_pa_margin_num;
static int g_svfd_data_num;
static int g_svfd_para_num;

STATIC bool get_npu_data_ptr(void)
{
#ifdef CONFIG_INPUTHUB_30
	if(g_smplat_scfg != NULL) {
		g_swing_avs_data = g_smplat_scfg->npu_data.avs_data;
		g_swing_pa_margin = g_smplat_scfg->npu_data.pa_margin;
		g_swing_svfd_data = g_smplat_scfg->npu_data.svfd_data;
		g_swing_svfd_para = g_smplat_scfg->npu_data.svfd_para;
		g_avs_data_num = NPU_AVS_DATA_NUN;
		g_pa_margin_num = PA_MARGIN_NUM;
		g_svfd_data_num = NPU_SVFD_DATA_NUN;
		g_svfd_para_num = NPU_SVFD_PARA_NUN;
		return true;
	}
	swing_log_err("get_npu_data_ptr: g_smplat_scfg NULL\n");
	return false;
#else
	return false;
#endif
}

STATIC void swing_dev_wait_init(struct completion *p_wait)
{
	if (p_wait == NULL) {
		swing_log_err("swing_dev_wait_init: wait NULL\n");
		return;
	}

	init_completion(p_wait);
}

STATIC int swing_dev_wait_completion(struct completion *p_wait, unsigned int tm_out)
{
	if (p_wait == NULL) {
		swing_log_err("swing_dev_wait_completion: wait NULL\n");
		return -EFAULT;
	}

	swing_log_info("swing_dev_wait_completion: waitting\n");
	if (tm_out != 0) {
		if (!wait_for_completion_interruptible_timeout(
		    p_wait, msecs_to_jiffies(tm_out))) {
			swing_log_warn("swing_dev_wait_completion: wait timeout\n");
			return -ETIMEOUT;
		}
	} else {
		if (wait_for_completion_interruptible(p_wait)) {
			swing_log_warn("swing_dev_wait_completion: wait interrupted.\n");
			return -EFAULT;
		}
	}

	return 0;
}

STATIC void swing_dev_complete(struct completion *p_wait)
{
	if (p_wait == NULL) {
		swing_log_err("swing_dev_complete: wait NULL\n");
		return;
	}

	complete(p_wait);
}

STATIC int swing_dev_ioctl_fusion_en(struct file *file, unsigned int cmd, unsigned long arg)
{
	swing_fusion_en_param_t fe = { {0} };
	int ret;

	if (arg == 0) {
		swing_log_err("[%s] arg NULL.\n", __func__);
		return -EFAULT;
	}

	if (copy_from_user((void *)&fe, (void *)((uintptr_t)arg), sizeof(swing_fusion_en_param_t))) {
		swing_log_err("[%s]copy_from_user error\n", __func__);
		return -EFAULT;
	}

	if (send_cmd_from_kernel(TAG_SWING, CMD_CMN_CONFIG_REQ, SUB_CMD_SWING_FUSION_EN,
				 (char *)(&fe.en), sizeof(swing_fusion_en_t))) {
		swing_log_err("[%s]send cmd error\n", __func__);
		return -EFAULT;
	}

	ret = swing_dev_wait_completion(&g_swing_priv.swing_wait, SWING_IOCTL_WAIT_TMOUT);
	if (ret)
		return ret;

	ret = memcpy_s((void *)(&fe.en_resp), sizeof(swing_fusion_en_resp_t),
		       (void *)(&g_swing_priv.en_resp), sizeof(swing_fusion_en_resp_t));
	if (ret != EOK)
		pr_err("%s memcpy buffer fail, ret[%d]\n", __func__, ret);

	if (copy_to_user((char *)((uintptr_t)arg), &fe, sizeof(swing_fusion_en_param_t))) {
		swing_log_err("%s failed to copy to user\n", __func__);
		return -EFAULT;
	}
	return 0;
}

STATIC int swing_dev_ioctl_fusion_set(struct file *file, unsigned int cmd, unsigned long arg)
{
	swing_fusion_set_param_t fs = { {0} };
	int ret;

	if (arg == 0) {
		swing_log_err("[%s] arg NULL.\n", __func__);
		return -EFAULT;
	}

	if (copy_from_user((void *)&fs, (void *)((uintptr_t)arg), sizeof(swing_fusion_set_param_t))) {
		swing_log_err("[%s]copy_from_user error\n", __func__);
		return -EFAULT;
	}

	if (send_cmd_from_kernel(TAG_SWING, CMD_CMN_CONFIG_REQ, SUB_CMD_SWING_FUSION_SET,
				 (char *)(&fs.set), sizeof(swing_fusion_set_t))) {
		swing_log_err("[%s]send cmd error\n", __func__);
		return -EFAULT;
	}

	ret = swing_dev_wait_completion(&g_swing_priv.swing_wait, SWING_IOCTL_WAIT_TMOUT);
	if (ret)
		return ret;

	ret = memcpy_s((void *)(&fs.set_resp), sizeof(swing_fusion_set_resp_t),
		       (void *)(&g_swing_priv.set_resp), sizeof(swing_fusion_set_resp_t));
	if (ret != EOK)
		pr_err("%s memcpy buffer fail, ret[%d]\n", __func__, ret);

	if (copy_to_user((void *)((uintptr_t)arg), &fs, sizeof(swing_fusion_set_param_t))) {
		swing_log_err("%s failed to copy to user\n", __func__);
		return -EFAULT;
	}

	return 0;
}

#ifdef CONFIG_CONTEXTHUB_SWING_20
STATIC int swing_dev_ioctl_fusion_gets(struct file *file, unsigned int cmd, unsigned long arg)
{
	swing_fusion_gets_param_t fs_gets = { {0} };
	int ret;

	if (arg == 0) {
		swing_log_err("[%s] arg NULL.\n", __func__);
		return -EFAULT;
	}

	if (copy_from_user((void *)&fs_gets, (void *)((uintptr_t)arg), sizeof(swing_fusion_gets_param_t))) {
		swing_log_err("[%s]copy_from_user error\n", __func__);
		return -EFAULT;
	}

	if (send_cmd_from_kernel(TAG_SWING, CMD_CMN_CONFIG_REQ, SUB_CMD_SWING_FUSION_GETS,
				 (char *)(&fs_gets.gets_param), sizeof(swing_fusion_gets_t))) {
		swing_log_err("[%s]send cmd error\n", __func__);
		return -EFAULT;
	}

	ret = swing_dev_wait_completion(&g_swing_priv.swing_wait, SWING_IOCTL_WAIT_TMOUT);
	if (ret != 0)
		goto do_ret;

	fs_gets.gets_resp.ret_code = g_swing_priv.gets_resp.ret_code;
	fs_gets.gets_resp.fusion_id = g_swing_priv.gets_resp.fusion_id;
	fs_gets.gets_resp.data_type = g_swing_priv.gets_resp.data_type;
	fs_gets.gets_resp.data_len = g_swing_priv.gets_resp.data_len;
	fs_gets.gets_resp.data_addr = fs_gets.gets_param.data_addr;

	if (fs_gets.gets_resp.data_addr == 0 || g_swing_priv.gets_resp.data_addr == 0) {
		swing_log_err("%s: addr NULL\n", __func__);
		ret = -EFAULT;
		goto do_ret;
	}
	// copy get data to get buffer.
	if (copy_to_user((void *)((uintptr_t)fs_gets.gets_resp.data_addr), (void *)((uintptr_t)g_swing_priv.gets_resp.data_addr), g_swing_priv.gets_resp.data_len)) {
		swing_log_err("%s failed to copy to user\n", __func__);
		ret = -EFAULT;
		goto do_ret;
	}

	if (copy_to_user((void *)((uintptr_t)arg), &fs_gets, sizeof(swing_fusion_gets_param_t))) {
		swing_log_err("%s failed to copy to user\n", __func__);
		ret = -EFAULT;
		goto do_ret;
	}

do_ret:
	// try to free buffer, alloc in get_resp.
	if ((void *)((uintptr_t)g_swing_priv.gets_resp.data_addr) != NULL) {
		kfree((void *)((uintptr_t)g_swing_priv.gets_resp.data_addr));
		g_swing_priv.gets_resp.data_addr = 0;
	}
	return ret;
}

STATIC int swing_dev_ioctl_fusion_sets(struct file *file, unsigned int cmd, unsigned long arg)
{
	swing_fusion_sets_param_t fss = { {0} };
	swing_fusion_sets_t *p_fss = NULL;
	struct write_info wr;
	int ret;

	if (arg == 0) {
		swing_log_err("[%s] arg NULL.\n", __func__);
		return -EFAULT;
	}

	if (copy_from_user((void *)&fss, (void *)((uintptr_t)arg), sizeof(swing_fusion_sets_param_t))) {
		swing_log_err("[%s]copy_from_user error\n", __func__);
		return -EFAULT;
	}

	if (fss.sets.data_len == 0 || fss.sets.data_addr == 0 || fss.sets.data_len >= SWING_DEV_MAX_DATA_LEN) {
		swing_log_err("[%s]para error\n", __func__);
		return -EFAULT;
	}

	p_fss = (swing_fusion_sets_t *)kzalloc(sizeof(swing_fusion_sets_t) + fss.sets.data_len, GFP_ATOMIC);
	if (p_fss == NULL) {
		swing_log_err("[%s]all fail\n", __func__);
		return -EFAULT;
	}

	ret = memcpy_s((void *)p_fss, sizeof(swing_fusion_sets_t) + fss.sets.data_len, (void *)(&fss), sizeof(swing_fusion_sets_t));
	if (ret != EOK) {
		swing_log_err("%s memcpy buffer fail, ret[%d]\n", __func__, ret);
		kfree(p_fss);
		return ret;
	}

	swing_log_info("[%s] copy data ++\n", __func__);
	if (copy_from_user((void *)(p_fss + 1), (void *)(uintptr_t)(p_fss->data_addr), p_fss->data_len)) {
		swing_log_err("[%s]copy_from_user error\n", __func__);
		kfree(p_fss);
		return -EFAULT;
	}
	swing_log_info("[%s] copy data --\n", __func__);

	wr.tag = TAG_SWING;
	wr.wr_buf = (void *)p_fss;
	wr.wr_len = sizeof(swing_fusion_sets_t) + fss.sets.data_len;
#ifdef CONFIG_INPUTHUB_30
	if (write_customize_cmd(&wr, NULL, false) != 0) {
		swing_log_err("[%s]send cmd error\n", __func__);
		kfree(p_fss);
		return -EFAULT;
	}
#else
	if (shmem_send(wr.tag, wr.wr_buf, wr.wr_len) != 0) {
		swing_log_err("[%s]send cmd error\n", __func__);
		kfree(p_fss);
		return -EFAULT;
	}
#endif
	ret = swing_dev_wait_completion(&g_swing_priv.swing_wait, SWING_IOCTL_WAIT_TMOUT);
	if (ret) {
		kfree(p_fss);
		return ret;
	}
	ret = memcpy_s((void *)(&fss.sets_resp), sizeof(swing_fusion_sets_resp_t),
		       (void *)(&g_swing_priv.sets_resp), sizeof(swing_fusion_sets_resp_t));
	if (ret != EOK) {
		swing_log_err("%s memcpy buffer fail, ret[%d]\n", __func__, ret);
		kfree(p_fss);
		return ret;
	}

	if (copy_to_user((void *)((uintptr_t)arg), &fss, sizeof(swing_fusion_sets_param_t))) {
		swing_log_err("%s failed to copy to user\n", __func__);
		kfree(p_fss);
		return -EFAULT;
	}

	kfree(p_fss);
	return 0;
}

static long swing_dev_ioctl_ion_attach(struct file *file, unsigned int cmd, unsigned long arg)
{
	swing_ion_info_t ion_info;
	struct swing_ion_node *node = NULL;
	struct sg_table *table = NULL;
	enum SEC_SVC type = 0;
	int ret;

	if (arg == 0) {
		swing_log_err("[%s]: arg NULL...\n", __func__);
		return -EFAULT;
	}

	if (copy_from_user((void *)&ion_info, (void *)((uintptr_t)arg), sizeof(swing_ion_info_t))) {
		swing_log_err("[%s]: copy_from_user failed\n", __func__);
		return -EFAULT;
	}

	if (ion_info.fd < 0) {
		swing_log_err("[%s] : invalid fd...\n", __func__);
		return -EFAULT;
	}

	ion_info.buf = dma_buf_get(ion_info.fd);
	if (IS_ERR(ion_info.buf)) {
		swing_log_err("[%s] Failed : dma_buf_get\n", __func__);
		return -EFAULT;
	}

	ret = secmem_get_buffer(ion_info.fd, &table, &ion_info.id, &type);
	if (ret != 0) {
		swing_log_err("[%s] : secmem_get_buffer failed.\n", __func__);
		return -EFAULT;
	}

	if (copy_to_user((void *)((uintptr_t)arg), &ion_info, sizeof(swing_ion_info_t))) {
		swing_log_err("%s failed to copy to user\n", __func__);
		return -EFAULT;
	}

	node = kzalloc(sizeof(struct swing_ion_node), GFP_ATOMIC);
	if (node == NULL) {
		swing_log_err("[%s] : Failed to alloc memory for ion node...\n", __func__);
		return -EFAULT;
	}

	node->ion_info.fd = ion_info.fd;
	node->ion_info.id = ion_info.id;
	node->ion_info.buf = ion_info.buf;
	list_add(&node->list, &g_swing_priv.ion_buf_list);
	swing_log_info("[%s]....done, %x, %llx\n", __func__, ion_info.fd, ion_info.id);

	return 0;
}

static long swing_dev_ioctl_ion_detach(struct file *file, unsigned int cmd, unsigned long arg)
{
	swing_ion_info_t ion_info;
	struct swing_ion_node *cur = NULL;
	struct swing_ion_node *next = NULL;

	if (arg == 0) {
		swing_log_err("[%s]: arg NULL...\n", __func__);
		return -EFAULT;
	}

	if (copy_from_user((void *)&ion_info, (void *)((uintptr_t)arg), sizeof(swing_ion_info_t))) {
		swing_log_err("[%s]: copy_from_user failed\n", __func__);
		return -EFAULT;
	}


	if (ion_info.buf == NULL) {
		swing_log_err("[%s]: dma buf is NULL...\n", __func__);
		return -EFAULT;
	}

	list_for_each_entry_safe(cur, next, &g_swing_priv.ion_buf_list, list) {
		if (cur->ion_info.buf == ion_info.buf) {
			swing_log_info("[%s]....done, %x\n", __func__, cur->ion_info.fd);

			dma_buf_put(ion_info.buf);

			list_del(&(cur->list));
			kfree(cur);
			return 0;
		}
	}

	swing_log_info("[%s]....done\n", __func__);

	return 0;
}
#endif

STATIC int swing_dev_ioctl_timeout_handler(void)
{
	swing_read_data_t read_data = {0};
	int ret = 0;

	swing_log_info("enter [%s]\n", __func__);
	mutex_lock(&g_swing_priv.read_mutex);

	if (kfifo_avail(&g_swing_priv.read_kfifo) < sizeof(swing_read_data_t)) {
		swing_log_err("%s read_kfifo is full, drop upload data.\n", __func__);
		ret = -EFAULT;
		goto ERR;
	}

	read_data.recv_len = sizeof(u32);
	read_data.p_recv = kzalloc(sizeof(u32), GFP_ATOMIC);
	if (read_data.p_recv == NULL) {
		swing_log_err("Failed to alloc memory for sensorhub reset message...\n");
		ret = -EFAULT;
		goto ERR;
	}

	*(u32 *)read_data.p_recv = SWING_RESET_NOTIFY;

	ret = kfifo_in(&g_swing_priv.read_kfifo, (unsigned char *)&read_data, sizeof(swing_read_data_t));
	if (ret <= 0) {
		swing_log_err("%s: kfifo_in failed\n", __func__);
		ret = -EFAULT;
		goto ERR;
	}

	mutex_unlock(&g_swing_priv.read_mutex);

	g_swing_priv.sh_recover_flag = 1;

	swing_dev_complete(&g_swing_priv.read_wait);

	return 0;
ERR:
	if (read_data.p_recv != NULL)
		kfree(read_data.p_recv);

	mutex_unlock(&g_swing_priv.read_mutex);

	return ret;
}

STATIC long swing_dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret;

	swing_log_info("%s cmd: [0x%x]\n", __func__, cmd);

	mutex_lock(&g_swing_priv.swing_mutex);

	if (g_swing_priv.sh_recover_flag == 1) {
		mutex_unlock(&g_swing_priv.swing_mutex);
		swing_log_info("%s sensorhub in recover mode\n", __func__);
		return -EBUSY;
	}

	reinit_completion(&g_swing_priv.swing_wait);

	swing_log_info("%s reinit completion\n", __func__);
	switch (cmd) {
	case SWING_IOCTL_FUSION_EN:
		ret = swing_dev_ioctl_fusion_en(file, cmd, arg);
		break;

	case SWING_IOCTL_FUSION_SET:
		ret = swing_dev_ioctl_fusion_set(file, cmd, arg);
		break;
#ifdef CONFIG_CONTEXTHUB_SWING_20
	case SWING_IOCTL_FUSION_GETS:
		ret = swing_dev_ioctl_fusion_gets(file, cmd, arg);
		break;

	case SWING_IOCTL_FUSION_SETS:
		ret = swing_dev_ioctl_fusion_sets(file, cmd, arg);
		break;

	case SWING_IOCTL_ION_REF_ATTACH:
		ret = swing_dev_ioctl_ion_attach(file, cmd, arg);
		break;
	case SWING_IOCTL_ION_REF_DETACH:
		ret = swing_dev_ioctl_ion_detach(file, cmd, arg);
		break;
#endif
	default:
		mutex_unlock(&g_swing_priv.swing_mutex);
		swing_log_err("%s unknown cmd : %d\n", __func__, cmd);
		return -ENOTTY;
	}

	mutex_unlock(&g_swing_priv.swing_mutex);

	if (ret == -ETIMEOUT) {
		if (swing_dev_ioctl_timeout_handler())
			swing_log_err("%s swing_dev_ioctl_timeout_handler err\n", __func__);
	}

	return ret;
}

STATIC int swing_get_resp_para_check(const pkt_header_t *head)
{
	pkt_subcmd_resp_t *p_resp = NULL;
	swing_upload_t *p_upload = NULL;
#ifdef CONFIG_CONTEXTHUB_SWING_20
	swing_fusion_gets_resp_t *p_fs_gets = NULL;
#endif
	unsigned int valid_len;

	p_resp = (pkt_subcmd_resp_t *)(head);

	if (p_resp == NULL) {
		swing_log_err("%s: p_resp is null\n", __func__);
		return -EFAULT;
	}

	if (p_resp->hd.tag != TAG_SWING) {
		swing_log_err("%s: invalid tag [0x%x]\n", __func__, p_resp->hd.tag);
		return -EFAULT;
	}

	switch (p_resp->subcmd) {
	case SUB_CMD_SWING_FUSION_EN:
		valid_len = sizeof(swing_fusion_en_resp_t);
		break;
	case SUB_CMD_SWING_FUSION_SET:
		valid_len = sizeof(swing_fusion_set_resp_t);
		break;
#ifdef CONFIG_CONTEXTHUB_SWING_20
	case SUB_CMD_SWING_FUSION_SETS:
		valid_len = sizeof(swing_fusion_sets_resp_t);
		break;
	case SUB_CMD_SWING_FUSION_GETS:
		p_fs_gets = (swing_fusion_gets_resp_t *)(p_resp + 1);
		valid_len = sizeof(swing_fusion_gets_resp_t) + p_fs_gets->data_len;
		break;
#endif
	case SUB_CMD_SWING_FUSION_UPLOAD:
		p_upload = (swing_upload_t *)(p_resp + 1);
		valid_len = sizeof(swing_upload_t) + p_upload->notify_len;
		break;

	case SUB_CMD_SWING_SYSCACHE_DISABLE:
	case SUB_CMD_SWING_SYSCACHE_ENABLE:
#ifdef CONFIG_CONTEXTHUB_SWING_20
	case SUB_CMD_SWING_SCREEN_ON:
	case SUB_CMD_SWING_SCREEN_OFF:
	case SUB_CMD_SWING_GET_WAKEUP_LOCK:
	case SUB_CMD_SWING_RELEASE_WAKEUP_LOCK:
#endif
		swing_log_info("%s: receive len:%u\n", __func__, head->length);
		valid_len = 0;
		break;

	default:
		swing_log_warn("%s: unhandled cmd, tag[%d], sub_cmd[%d]\n",
			       __func__, p_resp->hd.tag, p_resp->subcmd);
		return -EFAULT;
	}

	if (head->length != (valid_len + 8)) {
		swing_log_err("%s: invalid payload length: tag[%d], sub_cmd[%d], length[0x%x]\n",
			      __func__, p_resp->hd.tag, p_resp->subcmd, head->length);
		return -EFAULT;
	}

	return 0;
}

STATIC int swing_get_resp_upload(pkt_subcmd_resp_t *p_resp)
{
	swing_upload_t *p_upload = NULL;
	swing_read_data_t read_data = {0};
	int ret = 0;

	mutex_lock(&g_swing_priv.read_mutex);

	if (kfifo_avail(&g_swing_priv.read_kfifo) < sizeof(swing_read_data_t)) {
		swing_log_err("%s read_kfifo is full, drop upload data.\n", __func__);
		ret = -EFAULT;
		goto RET_ERR;
	}

	p_upload = (swing_upload_t *)(p_resp + 1);

	if (p_upload->notify_len > SWING_DEV_MAX_UPLOAD_LEN) {
		swing_log_err("%s upload length err 0x%x.\n", __func__, p_upload->notify_len);
		ret = -EFAULT;
		goto RET_ERR;
	}

	read_data.recv_len = sizeof(swing_upload_t) + p_upload->notify_len;
	read_data.p_recv = kzalloc(read_data.recv_len, GFP_ATOMIC);
	if (read_data.p_recv == NULL) {
		swing_log_err("Failed to alloc memory to save upload resp...\n");
		ret = -EFAULT;
		goto RET_ERR;
	}

	ret = memcpy_s(read_data.p_recv, read_data.recv_len, p_upload, read_data.recv_len);
	if (ret != 0) {
		swing_log_err("%s memcpy_s failed...\n", __func__);
		ret = -EFAULT;
		goto RET_ERR;
	}

	ret = kfifo_in(&g_swing_priv.read_kfifo, (unsigned char *)&read_data, sizeof(swing_read_data_t));
	if (ret <= 0) {
		swing_log_err("%s: kfifo_in failed\n", __func__);
		ret = -EFAULT;
		goto RET_ERR;
	}

	mutex_unlock(&g_swing_priv.read_mutex);

	swing_log_info("Fusion Upload Received, [0x%x].\n", p_upload->fusion_id);
	swing_dev_complete(&g_swing_priv.read_wait);

	return 0;

 RET_ERR:
	if (read_data.p_recv != NULL)
		kfree(read_data.p_recv);

	mutex_unlock(&g_swing_priv.read_mutex);

	return ret;
}

#ifdef CONFIG_CONTEXTHUB_SWING_20
STATIC int swing_get_resp_fusion_gets(pkt_subcmd_resp_t *p_resp)
{
	swing_fusion_gets_resp_t *p_gets_resp = NULL;
	int ret = 0;

	p_gets_resp = (swing_fusion_gets_resp_t *)(p_resp + 1);
	ret = memcpy_s(&g_swing_priv.gets_resp, sizeof(swing_fusion_gets_resp_t),
		       p_gets_resp, sizeof(swing_fusion_gets_resp_t));
	if (ret != EOK){
		swing_log_err("%s memcpy buffer fail, ret[%d]\n", __func__, ret);
		goto RET_ERR;
	}

	if (g_swing_priv.gets_resp.data_len != 0 && g_swing_priv.gets_resp.data_len < SWING_DEV_MAX_DATA_LEN) {
		g_swing_priv.gets_resp.data_addr = (u64)((uintptr_t)kzalloc(g_swing_priv.gets_resp.data_len, GFP_ATOMIC));
		if ((void *)((uintptr_t)g_swing_priv.gets_resp.data_addr) == NULL) {
			swing_log_err("[%s]kzalloc error\n", __func__);
			ret = -EFAULT;
			goto RET_ERR;
		}
		ret = memcpy_s((void *)((uintptr_t)g_swing_priv.gets_resp.data_addr), g_swing_priv.gets_resp.data_len,
			       (void *)((u8 *)p_gets_resp + sizeof(swing_fusion_gets_resp_t)), g_swing_priv.gets_resp.data_len);
		if (ret != EOK){
			swing_log_err("%s memcpy buffer fail, ret[%d]\n", __func__, ret);
			goto RET_ERR;
		}
	}
	swing_log_info("Fusion [0x%x] Gets Resp ret[0x%x], type[%d].\n",
		       p_gets_resp->fusion_id, p_gets_resp->ret_code, p_gets_resp->data_type);

RET_ERR:
	swing_dev_complete(&g_swing_priv.swing_wait);
	return ret;
}

STATIC int swing_get_resp_fusion_sets(pkt_subcmd_resp_t *p_resp)
{
	swing_fusion_sets_resp_t *p_sets_resp = NULL;
	int ret = 0;

	p_sets_resp = (swing_fusion_sets_resp_t *)(p_resp + 1);
	ret = memcpy_s(&g_swing_priv.sets_resp, sizeof(swing_fusion_sets_resp_t),
		       p_sets_resp, sizeof(swing_fusion_sets_resp_t));
	if (ret != EOK)
		swing_log_err("%s memcpy buffer fail, ret[%d]\n", __func__, ret);

	swing_log_info("Fusion [0x%x] Sets Resp ret[0x%x], type[%d].\n",
		       p_sets_resp->fusion_id, p_sets_resp->ret_code, p_sets_resp->data_type);
	swing_dev_complete(&g_swing_priv.swing_wait);

	return ret;
}

STATIC int swing_get_resp_syscache_disable(void)
{
	int ret = 0;

	swing_log_info("%s: receive shb resume rsp msg, sc_flag = %u.\n", __func__, g_config_on_ddr->sc_flag);
	mutex_lock(&g_swing_priv.swing_mutex);
	if (g_swing_priv.send_ipc_to_shb == SWING_SEND_RESUME_TO_SHB) {
		if (g_config_on_ddr->sc_flag == 1) {
			swing_log_info("release quota.\n");
			ret = lb_release_quota(SWING_SYSCAHCE_ID);
			if (ret != 0)
				swing_log_err("%s lb_release_quota fail, ret[%d]\n", __func__, ret);
		}

		g_swing_priv.send_ipc_to_shb = 0;
	}
	mutex_unlock(&g_swing_priv.swing_mutex);

	return ret;
}
#endif

STATIC int swing_get_resp_fusion_en(pkt_subcmd_resp_t *p_resp)
{
	swing_fusion_en_resp_t *p_en_resp = NULL;
	int ret;

	p_en_resp = (swing_fusion_en_resp_t *)(p_resp + 1);
	ret = memcpy_s(&g_swing_priv.en_resp, sizeof(swing_fusion_en_resp_t), p_en_resp, sizeof(swing_fusion_en_resp_t));
	if (ret != EOK)
		swing_log_err("%s memcpy buffer fail, ret[%d]\n", __func__, ret);

	swing_log_info("Fusion [0x%x] En Resp ret[0x%x] .\n",
		       p_en_resp->fusion_id, p_en_resp->ret_code);
	swing_dev_complete(&g_swing_priv.swing_wait);

	return ret;
}

STATIC int swing_get_resp_fusion_set(pkt_subcmd_resp_t *p_resp)
{
	swing_fusion_set_resp_t *p_set_resp = NULL;
	int ret;

	p_set_resp = (swing_fusion_set_resp_t *)(p_resp + 1);
	ret = memcpy_s(&g_swing_priv.set_resp, sizeof(swing_fusion_set_resp_t),
		       p_set_resp, sizeof(swing_fusion_set_resp_t));
	if (ret != EOK)
		swing_log_err("%s memcpy buffer fail, ret[%d]\n", __func__, ret);

	swing_log_info("Fusion [0x%x] Set Resp ret[0x%x] .\n",
		       p_set_resp->fusion_id, p_set_resp->ret_code);
	swing_dev_complete(&g_swing_priv.swing_wait);

	return ret;
}

STATIC int swing_get_resp(const pkt_header_t *head)
{
	pkt_subcmd_resp_t *p_resp = NULL;
	int ret;

	ret = swing_get_resp_para_check(head);
	if (ret != 0) {
		swing_log_warn("%s: para check failed.\n", __func__);
		return ret;
	}

	p_resp = (pkt_subcmd_resp_t *)(head);

	swing_log_info("%s: cmd[%d], length[%d], tag[%d], sub_cmd[%d]\n",
		       __func__, p_resp->hd.cmd, p_resp->hd.length, p_resp->hd.tag, p_resp->subcmd);

	switch (p_resp->subcmd) {
	case SUB_CMD_SWING_FUSION_EN:
		ret = swing_get_resp_fusion_en(p_resp);
		break;
	case SUB_CMD_SWING_FUSION_SET:
		ret = swing_get_resp_fusion_set(p_resp);
		break;

#ifdef CONFIG_CONTEXTHUB_SWING_20
	case SUB_CMD_SWING_FUSION_GETS:
		ret = swing_get_resp_fusion_gets(p_resp);
		break;

	case SUB_CMD_SWING_FUSION_SETS:
		ret = swing_get_resp_fusion_sets(p_resp);
		break;
	case SUB_CMD_SWING_SYSCACHE_DISABLE:
		ret = swing_get_resp_syscache_disable();
		break;
#endif
	case SUB_CMD_SWING_FUSION_UPLOAD:
		swing_log_info("Fusion Upload 0x%x.\n", p_resp->subcmd);

		ret = swing_get_resp_upload(p_resp);
		break;

#ifdef CONFIG_CONTEXTHUB_SWING_20
	case SUB_CMD_SWING_SYSCACHE_ENABLE:
	case SUB_CMD_SWING_SCREEN_ON:
		// do nothing.
		break;
	case SUB_CMD_SWING_GET_WAKEUP_LOCK:
		// release wake-lock when got screen-off ack.
		mutex_lock(&g_swing_priv.wklock_mutex);
		swing_log_info("%s: get wake msg.\n", __func__);
		if (!g_swing_priv.swing_wklock.active) {
			swing_log_info("%s: wake unlock.\n", __func__);
			__pm_wakeup_event(&g_swing_priv.swing_wklock, SWING_IOCTL_WAIT_TMOUT * 2);
		}
		mutex_unlock(&g_swing_priv.wklock_mutex);
		break;
	case SUB_CMD_SWING_RELEASE_WAKEUP_LOCK:
	case SUB_CMD_SWING_SCREEN_OFF:
		mutex_lock(&g_swing_priv.wklock_mutex);
		// release wake-lock when got screen-off ack.
		swing_log_info("%s: receive msg [%x].\n", __func__, p_resp->subcmd);
		if (g_swing_priv.swing_wklock.active) {
			swing_log_info("%s: wake unlock.\n", __func__);
			__pm_relax(&g_swing_priv.swing_wklock);
		}
		mutex_unlock(&g_swing_priv.wklock_mutex);
		break;
#endif
	default:
		swing_log_warn("unhandled cmd: tag[%d], sub_cmd[%d]\n", p_resp->hd.tag, p_resp->subcmd);
		break;
	}

	return ret;
}

STATIC u32 swing_get_dmd_log_index(u32 dmd_id)
{
	u32 index;
	size_t log_arry_size = ARRAY_SIZE(dmd_log);

	for (index = 0; index < log_arry_size; index++) {
		if (dmd_log[index].dmd_id == dmd_id)
			break;
	}
	return index;
}

STATIC int swing_get_shb_dmd_report(const pkt_header_t *head)
{
	int ret;
	u32 index;
	size_t hd_size = sizeof(pkt_header_t);
	size_t req_size = sizeof(pkt_dmd_log_report_req_t);
	size_t log_arry_size = ARRAY_SIZE(dmd_log);

	pkt_dmd_log_report_req_t *req = (pkt_dmd_log_report_req_t *)(head);

	if (req == NULL) {
		swing_log_err("swing_get_shb_dmd_report: req is null!!\n");
		return -EFAULT;
	}

	if (req->hd.tag != TAG_SWING) {
		swing_log_err("swing_get_shb_dmd_report: tag[0x%x] not match TAG_SWING!!\n", req->hd.tag);
		return -EFAULT;
	}

	if (req->hd.cmd != CMD_LOG_REPORT_REQ) {
		swing_log_err("swing_get_shb_dmd_report: cmd[0x%x] not match CMD_LOG_REPORT_REQ!!\n", req->hd.cmd);
		return -EFAULT;
	}

	if (hd_size + req->hd.length != req_size) {
		swing_log_err("swing_get_shb_dmd_report: req length check failed!! hd_size[%u], payload size[%u], total[%u]!!\n",
			      (u32)hd_size, req->hd.length, (u32)req_size);
		return -EFAULT;
	}
	swing_log_info("swing_get_shb_dmd_report: dmd_id[%u], dmd_case[0x%x], length[%u], tranid[%u], para_num [%u]\n",
		       req->dmd_id, req->dmd_case, req->hd.length, req->hd.tranid, req->resv1);

	if (req->dmd_case != SWING_DMD_CASE_DETAIL) {
		swing_log_err("swing_get_shb_dmd_report: dmd_id[%u] err!! dmd_case [0x%x]\n", req->dmd_id, req->dmd_case);
		return -EFAULT;
	}

	index = swing_get_dmd_log_index(req->dmd_id);
	if (index >= log_arry_size) {
		swing_log_err("swing_get_shb_dmd_report: dmd_id[%u] not found!!\n", req->dmd_id);
		return -EFAULT;
	}

	if (req->resv1 != dmd_log[index].dmd_para_num) {
		swing_log_err("swing_get_shb_dmd_report: resv1[%u] not equal to dmd_para_num[%u]!! req->dmd_id[%u].\n",
			      req->resv1, dmd_log[index].dmd_para_num, req->dmd_id);
		return -EFAULT;
	}

	// req->resv1: info[] arry size
	switch (req->resv1) {
	case 0:
		ret = bbox_diaginfo_record(req->dmd_id, NULL, dmd_log[index].dmd_msg);
		break;

	case 1:
		ret = bbox_diaginfo_record(req->dmd_id, NULL, dmd_log[index].dmd_msg, req->info[0]);
		break;

	case 2:
		ret = bbox_diaginfo_record(req->dmd_id, NULL, dmd_log[index].dmd_msg, req->info[0], req->info[1]);
		break;

	case 3:
		ret = bbox_diaginfo_record(req->dmd_id, NULL, dmd_log[index].dmd_msg, req->info[0], req->info[1], req->info[2]);
		break;

	case 4:
		ret = bbox_diaginfo_record(req->dmd_id, NULL, dmd_log[index].dmd_msg, req->info[0], req->info[1], req->info[2], req->info[3]);
		break;

	case 5:
		ret = bbox_diaginfo_record(req->dmd_id, NULL, dmd_log[index].dmd_msg, req->info[0], req->info[1], req->info[2], req->info[3], req->info[4]);
		break;

	default:
		swing_log_err("swing_get_shb_dmd_report: info arry size[%u] error!! dmd_id[%u].\n", req->resv1, req->dmd_id);
		return -EFAULT;
	}

	return ret;
}

STATIC int swing_sensorhub_reset_handler(void)
{
	swing_read_data_t read_data = {0};
	int ret = 0;

	swing_log_info("enter [%s]\n", __func__);
	mutex_lock(&g_swing_priv.read_mutex);

	if (kfifo_avail(&g_swing_priv.read_kfifo) < sizeof(swing_read_data_t)) {
		swing_log_err("%s read_kfifo is full, drop upload data.\n", __func__);
		ret = -EFAULT;
		goto ERR;
	}

	read_data.recv_len = sizeof(u32);
	read_data.p_recv = kzalloc(sizeof(u32), GFP_ATOMIC);
	if (read_data.p_recv == NULL) {
		swing_log_err("Failed to alloc memory for sensorhub reset message...\n");
		ret = -EFAULT;
		goto ERR;
	}

	*(u32 *)read_data.p_recv = SWING_RESET_NOTIFY;

	ret = kfifo_in(&g_swing_priv.read_kfifo, (unsigned char *)&read_data, sizeof(swing_read_data_t));
	if (ret <= 0) {
		swing_log_err("%s: kfifo_in failed\n", __func__);
		ret = -EFAULT;
		goto ERR;
	}

	mutex_unlock(&g_swing_priv.read_mutex);

	g_swing_priv.sh_recover_flag = 1;

#ifdef CONFIG_CONTEXTHUB_SWING_20
	if (g_swing_priv.send_ipc_to_shb != 0) {
		swing_log_info("%s: sc_flag = %u.\n", __func__, g_config_on_ddr->sc_flag);
		if (g_config_on_ddr->sc_flag == 1) {
			ret = lb_release_quota(SWING_SYSCAHCE_ID);
			if (ret != 0)
				swing_log_err("%s: lb_release_quota failed[%d].\n", __func__, ret);
		}

		g_swing_priv.send_ipc_to_shb = 0;
	}
#endif

	swing_dev_complete(&g_swing_priv.read_wait);
	swing_dev_complete(&g_swing_priv.swing_wait);
	return 0;

ERR:
	if (read_data.p_recv != NULL)
		kfree(read_data.p_recv);

	mutex_unlock(&g_swing_priv.read_mutex);

	return ret;
}

STATIC int swing_sensorhub_reset_notifier(struct notifier_block *nb, unsigned long action, void *data)
{
	int ret = 0;

	switch (action) {
	case IOM3_RECOVERY_IDLE:
		ret = swing_sensorhub_reset_handler();
		break;
	default:
		break;
	}

	return ret;
}

static struct notifier_block swing_reboot_notify = {
	.notifier_call = swing_sensorhub_reset_notifier,
	.priority = -1,
};

STATIC ssize_t swing_dev_read(struct file *file,
			      char __user *buf, size_t count, loff_t *pos)
{
	swing_read_data_t read_data = {0};
	u32 error = 0;
	u32 length;
	int ret = 0;

	swing_log_info("[%s]\n", __func__);
	if (buf == NULL || count == 0)
		goto ERR;

	error = swing_dev_wait_completion(&g_swing_priv.read_wait, 0);
	if (error != 0) {
		error = 0;
		goto ERR;
	}

	mutex_lock(&g_swing_priv.read_mutex);
	if (kfifo_len(&g_swing_priv.read_kfifo) < sizeof(swing_read_data_t)) {
		swing_log_err("%s: read data failed.\n", __func__);
		mutex_unlock(&g_swing_priv.read_mutex);
		goto ERR;
	}

	ret = kfifo_out(&g_swing_priv.read_kfifo, (unsigned char *)&read_data, sizeof(swing_read_data_t));
	if (ret < 0) {
		swing_log_err("%s: kfifo out failed.\n", __func__);
		mutex_unlock(&g_swing_priv.read_mutex);
		goto ERR;
	}

	if (count < read_data.recv_len) {
		length = count;
		swing_log_err("%s user buffer is too small\n", __func__);
	} else {
		length = read_data.recv_len;
	}

	swing_log_info("[%s] copy len[0x%x], count[0x%x]\n", __func__, read_data.recv_len, (u32)count);

	error = length;
	/* copy to user */
	if (copy_to_user(buf, read_data.p_recv, length)) {
		swing_log_err("%s failed to copy to user\n", __func__);
		error = 0;
	}

	mutex_unlock(&g_swing_priv.read_mutex);

ERR:
	if (read_data.p_recv != NULL) {
		kfree(read_data.p_recv);
		read_data.p_recv = NULL;
		read_data.recv_len = 0;
	}

	return error;
}

STATIC ssize_t swing_dev_write(struct file *file, const char __user *data,
			       size_t len, loff_t *ppos)
{
	swing_log_info("%s need to do...\n", __func__);
	return len;
}

STATIC int swing_dev_open(struct inode *inode, struct file *file)
{
	int ret = 0;
	swing_log_info("enter %s\n", __func__);

	mutex_lock(&g_swing_priv.swing_mutex);

	if (g_swing_priv.ref_cnt != 0) {
		swing_log_warn("%s duplicate open.\n", __func__);
		mutex_unlock(&g_swing_priv.swing_mutex);
		return -EFAULT;
	}

	if (g_swing_priv.ref_cnt == 0)
		ret = send_cmd_from_kernel(TAG_SWING, CMD_CMN_OPEN_REQ, 0, NULL, (size_t)0);

	file->private_data = &g_swing_priv;

	if (ret == 0)
		g_swing_priv.ref_cnt++;

	g_swing_priv.sh_recover_flag = 0;
	g_swing_priv.send_ipc_to_shb = 0;

	mutex_unlock(&g_swing_priv.swing_mutex);
	return ret;
}

STATIC int swing_dev_release(struct inode *inode, struct file *file)
{
	struct read_info rd;
#ifdef CONFIG_CONTEXTHUB_SWING_20
	struct swing_ion_node *cur = NULL;
	struct swing_ion_node *next = NULL;
#endif

	swing_log_info("enter %s\n", __func__);

	mutex_lock(&g_swing_priv.swing_mutex);
	if (g_swing_priv.ref_cnt == 0) {
		swing_log_err("%s: ref cnt is 0.\n", __func__);
		mutex_unlock(&g_swing_priv.swing_mutex);
		return -EFAULT;
	}

	g_swing_priv.ref_cnt--;

	if (g_swing_priv.ref_cnt == 0) {
		(void)memset_s((void *)&rd, sizeof(struct read_info), 0, sizeof(struct read_info));

		send_cmd_from_kernel_response(TAG_SWING, CMD_CMN_CLOSE_REQ, 0, NULL, (size_t)0, &rd);
		swing_log_info("%s: got close resp\n", __func__);
	}

#ifdef CONFIG_CONTEXTHUB_SWING_20
	list_for_each_entry_safe(cur, next, &g_swing_priv.ion_buf_list, list) {
		swing_log_info("%s: detach %x\n", __func__, cur->ion_info.fd);

		dma_buf_put(cur->ion_info.buf);
		list_del(&(cur->list));
		kfree(cur);
	}
#endif
	mutex_unlock(&g_swing_priv.swing_mutex);

	return 0;
}

static int npu_get_avs_volt(struct device_node *np)
{
	const struct property *prop = NULL;
	int nr, i;
	const char *npu_avs_name = "npu-avs-volt";
	const char *margin_code_name = "pa-code-margin";

	if (g_swing_pa_margin == NULL || g_swing_avs_data == NULL ||
	    g_pa_margin_num == 0 || g_avs_data_num == 0) {
		swing_log_err("%s: invalid g_smplat_scfg\n", __func__);
		return -EINVAL;
	}
	prop = of_find_property(np, npu_avs_name, NULL);
	if (prop == NULL) {
		swing_log_warn("%s: no property:%s\n", __func__, npu_avs_name);
		return 0;
	}
	if (!prop->value) {
		swing_log_err("%s: property:%s value err\n", __func__, npu_avs_name);
		return -ENODATA;
	}

	nr = prop->length / sizeof(u32);
	if (nr > g_avs_data_num) {
		swing_log_err("%s: property:%s value num err, nr=%d\n", __func__, npu_avs_name, nr);
		return -EINVAL;
	}

	if (of_property_read_u32_array(np, npu_avs_name, g_swing_avs_data, nr)) {
		swing_log_err("%s: property:%s array err\n", __func__, npu_avs_name);
		return -EINVAL;
	}

	for (i = 0; i < nr / 2; i++) { /* 2 int for 1 profile avs data */
		if (i * 2 + 1 < g_avs_data_num)
			swing_log_info("[%s]: npu_avs_volt[%d]=0x%x %x\n", __func__, i,
				       g_swing_avs_data[i * 2 + 1], /* 2 int for 1 profile avs data */
				       g_swing_avs_data[i * 2]);
	}

	/* read margin */
	prop = of_find_property(np, margin_code_name, NULL);
	if (prop == NULL) {
		swing_log_warn("%s: no property:%s\n", __func__, margin_code_name);
		return 0;
	}
	if (!prop->value) {
		swing_log_err("%s: property:%s value err\n", __func__, margin_code_name);
		return -ENODATA;
	}

	nr = prop->length / sizeof(u32);
	if (nr > g_pa_margin_num) {
		swing_log_err("%s: property:%s value num err, nr=%d\n", __func__, margin_code_name, nr);
		return -EINVAL;
	}

	if (of_property_read_u32_array(np, margin_code_name, g_swing_pa_margin, nr)) {
		swing_log_err("%s: property:%s array err\n", __func__, margin_code_name);
		return -EINVAL;
	}
	swing_log_info("[%s]: pa_margin=0x%x, 0x%x\n", __func__, g_swing_pa_margin[0],
		       g_swing_pa_margin[1]);
	return 0;
}

static int get_npu_svfd_code(struct device_node *np)
{
	const struct property *prop = NULL;
	int nr;
	const char *svfd_code_name = "npu-svfd-code";
	const char *svfd_para_name = "npu-svfd-para";

	if (g_swing_svfd_data == NULL || g_swing_svfd_para == NULL ||
	    g_svfd_data_num == 0 || g_svfd_para_num == 0) {
		swing_log_err("%s: invalid g_smplat_scfg\n", __func__);
		return -EINVAL;
	}

	prop = of_find_property(np, svfd_code_name, NULL);
	if (prop == NULL) {
		swing_log_warn("%s: find property:npu-svfd-code fail\n", __func__);
		return 0;
	}
	if (!prop->value) {
		swing_log_err("%s: property:npu-svfd-code value err\n", __func__);
		return -ENODATA;
	}

	nr = prop->length / sizeof(u32);
	if (nr > g_svfd_data_num) {
		swing_log_err("%s: property:npu-svfd-code value num err, nr=%d\n", __func__, nr);
		return -EINVAL;
	}

	if (of_property_read_u32_array(np, svfd_code_name, g_swing_svfd_data, nr)) {
		swing_log_err("%s: property:%s array err\n", __func__, svfd_code_name);
		return -EINVAL;
	}

	/* npu_svfd_para */
	prop = of_find_property(np, svfd_para_name, NULL);
	if (prop == NULL) {
		swing_log_warn("%s: find property:npu-svfd-para fail\n", __func__);
		return 0;
	}
	if (!prop->value) {
		swing_log_err("%s: property:npu-svfd-para value err\n", __func__);
		return -ENODATA;
	}

	nr = prop->length / sizeof(u32);
	if (nr > g_svfd_para_num) {
		swing_log_err("%s: property:npu-svfd-para value num err, nr=%d\n", __func__, nr);
		return -EINVAL;
	}

	if (of_property_read_u32_array(np, svfd_para_name, g_swing_svfd_para, nr)) {
		swing_log_err("%s: property:%s array err\n", __func__, svfd_para_name);
		return -EINVAL;
	}

	return 0;
}

STATIC int get_swing_dev_dts_status(void)
{
	struct device_node *node = NULL;
	int temp = 0;

	node = of_find_compatible_node(NULL, NULL, "hisilicon,swing-dev");
	if (node == NULL) {
		pr_warn("[%s] : no swing dev..\n", __func__);
		return -ENODEV;
	}

	if (!of_device_is_available(node)) {
		pr_warn("[%s] swing disabled..\n", __func__);
		return -ENODATA;
	}

	if (of_property_read_u32(node, "is_pll_on", &temp))
		swing_log_warn("[%s]:read product_id_value fail\n", __func__);
	else
		g_config_on_ddr->is_pll_on = (u32)temp;

	swing_log_info("[%s] pid 0x%x..\n", __func__, temp);

	if (get_npu_data_ptr()) {
		if (npu_get_avs_volt(node) != 0) {
			swing_log_err("[%s] fail..\n", __func__);
			return -ENODATA;
		}
		if (get_npu_svfd_code(node) != 0) {
			swing_log_err("[%s] fail..\n", __func__);
			return -ENODATA;
		}
	}
	pr_info("[%s][enabled]\n", __func__);

	return 0;
}

#ifdef CONFIG_CONTEXTHUB_SWING_20
static int swing_notifier(struct notifier_block *nb, unsigned long action, void *data)
{
	int ret;

	if (!data)
		return NOTIFY_OK;

	switch (action) {
	case FB_EVENT_BLANK: /* change finished */
	{
		struct fb_event *event = data;
		int *blank = event->data;

		if (registered_fb[0] !=
			event->info) { /* only main screen on/off info send to hub */
			swing_log_err("%s, not main screen info, just return\n", __func__);
			return NOTIFY_OK;
		}
		switch (*blank) {
		case FB_BLANK_UNBLANK:
			mutex_lock(&g_swing_priv.wklock_mutex);
			/* screen on */
			swing_log_info("[%s] screen on\n", __func__);
			if (g_swing_priv.swing_wklock.active)
				__pm_relax(&g_swing_priv.swing_wklock);
			ret = send_cmd_from_kernel(TAG_SWING, CMD_CMN_CONFIG_REQ,
						   SUB_CMD_SWING_SCREEN_ON, NULL, (size_t)0);
			if (ret != 0)
				swing_log_err("[%s]send cmd error[%d]\n", __func__, ret);

			g_config_on_ddr->screen_sts = SWING_SCREEN_ON;
			mutex_unlock(&g_swing_priv.wklock_mutex);
			break;

		case FB_BLANK_POWERDOWN:
			mutex_lock(&g_swing_priv.wklock_mutex);
			/* screen off, wake-lock, util receive ack from sensorhub */
			swing_log_info("[%s] screen off\n", __func__);
			if (!g_swing_priv.swing_wklock.active) {
				swing_log_info("[%s] wake lock.\n", __func__);
				__pm_wakeup_event(&g_swing_priv.swing_wklock, SWING_IOCTL_WAIT_TMOUT * 2);
			}

			ret = send_cmd_from_kernel(TAG_SWING, CMD_CMN_CONFIG_REQ,
						   SUB_CMD_SWING_SCREEN_OFF, NULL, (size_t)0);
			if (ret != 0) {
				swing_log_err("[%s]send cmd error[%d]\n", __func__, ret);
				/* send ipc failed, release wake-lock */
				__pm_relax(&g_swing_priv.swing_wklock);
			}

			g_config_on_ddr->screen_sts = SWING_SCREEN_OFF;
			mutex_unlock(&g_swing_priv.wklock_mutex);
			break;

		default:
			swing_log_err("unknown---> lcd unknown in %s\n", __func__);
			break;
		}
		break;
	}
	default:
		break;
	}

	return NOTIFY_OK;
}

static struct notifier_block swing_fb_notify = {
	.notifier_call = swing_notifier,
};
#endif

static const struct file_operations swing_dev_fops = {
	.owner             = THIS_MODULE,
	.llseek            = no_llseek,
	.unlocked_ioctl    = swing_dev_ioctl,
	.open              = swing_dev_open,
	.release           = swing_dev_release,
	.read              = swing_dev_read,
	.write             = swing_dev_write,
};

static struct miscdevice swing_miscdev = {
	.minor =    MISC_DYNAMIC_MINOR,
	.name =     "swing_dev",
	.fops =     &swing_dev_fops,
};

STATIC int __init swing_dev_init(struct platform_device *pdev)
{
	int ret = 0;

	if (is_sensorhub_disabled()) {
		swing_log_err("sensorhub disabled....\n");
		return -EFAULT;
	}

	ret = get_swing_dev_dts_status();
	if (ret != 0) {
		swing_log_warn("%s swing is disabled\n", __func__);
		return ret;
	}

	if (g_config_on_ddr->swing_hardware_bypass != 0) {
		swing_log_warn("%s: swing hardware bypass!\n", __func__);
		return -ENODEV;
	}

	ret = misc_register(&swing_miscdev);
	if (ret != 0) {
		swing_log_err("%s cannot register miscdev err=%d\n", __func__, ret);
		return ret;
	}
#ifdef CONFIG_CONTEXTHUB_SWING_20
	ret = fb_register_client(&swing_fb_notify);
	if (ret != 0) {
		swing_log_err("%s: unable to register fb_notifier: %d", __func__, ret);
		misc_deregister(&swing_miscdev);
		return ret;
	}

	INIT_LIST_HEAD(&g_swing_priv.ion_buf_list);
#endif
	mutex_init(&g_swing_priv.read_mutex);
	mutex_init(&g_swing_priv.swing_mutex);

	swing_dev_wait_init(&g_swing_priv.swing_wait);
	swing_dev_wait_init(&g_swing_priv.read_wait);
#ifdef CONFIG_CONTEXTHUB_SWING_20
	mutex_init(&g_swing_priv.wklock_mutex);
	wakeup_source_init(&g_swing_priv.swing_wklock, "swing-wklock");
#endif
	ret = register_mcu_event_notifier(TAG_SWING, CMD_CMN_CONFIG_RESP, swing_get_resp);
	if (ret != 0) {
		swing_log_err("[%s]: register CMD_CMN_CONFIG_RESP notifier failed. [%d]\n", __func__, ret);
		goto ERR1;
	}

	ret = register_mcu_event_notifier(TAG_SWING, CMD_LOG_REPORT_REQ, swing_get_shb_dmd_report);
	if (ret != 0) {
		swing_log_err("[%s]: register DMD CMD_LOG_REPORT_REQ notifier failed. [%d]\n", __func__, ret);
		goto ERR2;
	}

	ret = register_iom3_recovery_notifier(&swing_reboot_notify);
	if (ret < 0) {
		swing_log_err("[%s]register_iom3_recovery_notifier fail\n", __func__);
		goto ERR3;
	}

	if (kfifo_alloc(&g_swing_priv.read_kfifo,
			sizeof(swing_read_data_t) * SWING_READ_CACHE_COUNT, GFP_KERNEL)) {
		swing_log_err("%s kfifo alloc failed.\n", __func__);
		ret = -ENOMEM;
		goto ERR4;
	}

	g_swing_priv.ref_cnt = 0;
	g_swing_priv.self = &(pdev->dev);
	g_swing_priv.send_ipc_to_shb = 0;

	g_config_on_ddr->screen_sts = SWING_SCREEN_ON;

	pdev->dev.dma_mask = &g_swing_dmamask;
	pdev->dev.coherent_dma_mask = SWING_DMA_MASK;

	return 0;

ERR4:
	// unregister_iom3_recovery_notifier
ERR3:
	unregister_mcu_event_notifier(TAG_SWING, CMD_LOG_REPORT_REQ, swing_get_shb_dmd_report);
ERR2:
	unregister_mcu_event_notifier(TAG_SWING, CMD_CMN_CONFIG_RESP, swing_get_resp);
ERR1:
#ifdef CONFIG_CONTEXTHUB_SWING_20
	wakeup_source_trash(&g_swing_priv.swing_wklock);
	fb_unregister_client(&swing_fb_notify);
#endif
	misc_deregister(&swing_miscdev);

	swing_log_err("%s : init failed....\n", __func__);

	return ret;
}

STATIC void __exit swing_dev_exit(void)
{
	swing_log_info("%s : enter....\n", __func__);

	kfifo_free(&g_swing_priv.read_kfifo);

	unregister_mcu_event_notifier(TAG_SWING, CMD_CMN_CONFIG_RESP, swing_get_resp);
	unregister_mcu_event_notifier(TAG_SWING, CMD_LOG_REPORT_REQ, swing_get_shb_dmd_report);

#ifdef CONFIG_CONTEXTHUB_SWING_20
	if (g_swing_priv.swing_wklock.active)
		__pm_relax(&g_swing_priv.swing_wklock);

	wakeup_source_trash(&g_swing_priv.swing_wklock);
	fb_unregister_client(&swing_fb_notify);
#endif
	misc_deregister(&swing_miscdev);
}

/* probe() function for platform driver */
static int swing_probe(struct platform_device *pdev)
{
	if (pdev == NULL) {
		swing_log_err("swing_probe: pdev is NULL\n");
		return -EFAULT;
	}

	swing_log_info("swing_probe...\n");

	return swing_dev_init(pdev);
}

/* remove() function for platform driver */
static int __exit swing_remove(struct platform_device *pdev)
{
	swing_log_info("swing_remove...\n");

	swing_dev_exit();

	return 0;
}

#define SWING_DRV_NAME "hisilicon,swing"

/*
 * suspend
 */
static int swing_pm_suspend(struct device *dev)
{
	int ret;

#ifndef CONFIG_CONTEXTHUB_SWING_20
	// just return, if swing20 is off.
	return 0;
#endif

	mutex_lock(&g_swing_priv.swing_mutex);

	if (g_swing_priv.ref_cnt == 0) {
		swing_log_info("[%s] swing is not open.\n", __func__);
		mutex_unlock(&g_swing_priv.swing_mutex);
		return 0;
	}

	if (g_swing_priv.send_ipc_to_shb != 0) {
		swing_log_err("[%s]not send suspend to shb. g_swing_priv.send_ipc_to_shb:[%d]\n",
				 __func__, g_swing_priv.send_ipc_to_shb);
		mutex_unlock(&g_swing_priv.swing_mutex);
		return 0;
	}
	swing_log_info("[%s]sc_flag = %u.\n", __func__, g_config_on_ddr->sc_flag);
	if (g_config_on_ddr->sc_flag == 1) {
		ret = lb_request_quota(SWING_SYSCAHCE_ID);
		if (ret != 0) {
			swing_log_err("[%s]lb_request_quota failed[%d]\n", __func__, ret);
			goto suspend_err;
		}
	}

	g_swing_priv.send_ipc_to_shb = SWING_SEND_SUSPEND_TO_SHB;

	ret = send_cmd_from_kernel(TAG_SWING, CMD_CMN_CONFIG_REQ, SUB_CMD_SWING_SYSCACHE_ENABLE,
				   NULL, (size_t)0);
	if (ret != 0) {
		swing_log_err("[%s]send cmd error[%d]\n", __func__, ret);
		goto suspend_err;
	}
	mutex_unlock(&g_swing_priv.swing_mutex);
	return 0 ;

suspend_err:
	mutex_unlock(&g_swing_priv.swing_mutex);
	return ret;
}

/*
 * resume
 */
static int swing_pm_resume(struct device *dev)
{
	int ret;

#ifndef CONFIG_CONTEXTHUB_SWING_20
	// just return, if swing20 is off.
	return 0;
#endif

	mutex_lock(&g_swing_priv.swing_mutex);

	if (g_swing_priv.ref_cnt == 0) {
		swing_log_info("[%s] swing is not open.\n", __func__);
		mutex_unlock(&g_swing_priv.swing_mutex);
		return 0;
	}

	if (g_swing_priv.send_ipc_to_shb != SWING_SEND_SUSPEND_TO_SHB) {
		swing_log_info("[%s] shb condition not match, send_ipc_to_shb: %d\n", __func__, g_swing_priv.send_ipc_to_shb);
	}

	if (g_swing_priv.send_ipc_to_shb == 0) {
		swing_log_info("[%s]not send resume to shb: g_swing_priv.send_ipc_to_shb %d\n",
				__func__, g_swing_priv.send_ipc_to_shb);
		mutex_unlock(&g_swing_priv.swing_mutex);
		return 0;
	}

	ret = send_cmd_from_kernel(TAG_SWING, CMD_CMN_CONFIG_REQ, SUB_CMD_SWING_SYSCACHE_DISABLE,
				   NULL, (size_t)0);
	if (ret != 0) {
		swing_log_err("[%s]send cmd error[%d]\n", __func__, ret);
		mutex_unlock(&g_swing_priv.swing_mutex);
		return ret;
	}

	g_swing_priv.send_ipc_to_shb = SWING_SEND_RESUME_TO_SHB;
	mutex_unlock(&g_swing_priv.swing_mutex);

	return ret;
}

const struct dev_pm_ops swing_pm_ops = {
	.suspend = swing_pm_suspend,
	.resume  = swing_pm_resume,
};

static const struct of_device_id swing_match_table[] = {
	{ .compatible = SWING_DRV_NAME, },
	{},
};

static struct platform_driver swing_platdev = {
	.driver = {
		.name = "swing_dev",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(swing_match_table),
		.pm = &swing_pm_ops,
	},
	.probe  = swing_probe,
	.remove = swing_remove,
};

static int __init swing_main_init(void)
{
	return platform_driver_register(&swing_platdev);
}

static void __exit swing_main_exit(void)
{
	platform_driver_unregister(&swing_platdev);
}

late_initcall_sync(swing_main_init);
module_exit(swing_main_exit);
