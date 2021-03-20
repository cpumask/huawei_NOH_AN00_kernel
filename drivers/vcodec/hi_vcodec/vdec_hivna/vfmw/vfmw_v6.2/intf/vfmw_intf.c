/*
 * vfmw_intf.c
 *
 * This is for vfmw_intf.
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
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

#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/atomic.h>
#include "vfmw_intf.h"
#include "vfmw_intf_check.h"
#include "vfmw_osal.h"
#include "vfmw_pdt.h"
#include "vfmw_proc.h"
#include "dec_dev.h"
#include "stm_dev.h"
#include "scd_hal.h"
#include "dbg.h"
#include "hisi_vcodec_vdec.h"

#define VFMW_OK 0
#define VFMW_ERR (-1)

/* only dev_id[0->VDH; 1->MDMA] enabled */
const hi_u32 g_irq_handled;
hi_u32 g_irq_num_norm;

#ifdef HI_TVP_SUPPORT
#define WAIT_TASK_EXIT_TIMEOUT  20 // ms
#define WAIT_TASK_EXIT_INER     1  // ms

typedef struct {
	struct task_struct *task;
	wait_queue_head_t waitq;
	bool suspend;
	bool resume;
	hi_u32 suspend_event;
	hi_u32 resume_event;
} vdec_tvp_info;

typedef enum {
	DEVICE_SUSPEND = 0,
	DEVICE_RESUME,
	DEVICE_UNKNOW
} device_state;

static vdec_tvp_info g_vdec_tvp;
#endif

typedef hi_s32 (*vctrl_handler)(void *, void *, void *);

typedef struct {
	vfmw_cmd_type cmd;
	vctrl_handler handler;
} vctrl_case;

static irqreturn_t vfmw_isr(int irq, void *id)
{
	hi_u16 dev_id = *((hi_s32 *)id);
	vdec_entry *vdec = vdec_get_entry();
	unsigned long flag;

	if (dev_id > DEC_DEV_NUM)
		return IRQ_NONE;

	spin_lock_irqsave(&vdec->power_state_spin_lock, flag);
	if (!vdec->power_state) {
		dprint(PRN_ERROR, "vdec now power off, irq should exit\n");
		goto exit;
	}

	if (scd_hal_isr_state() == STM_OK)
		stm_dev_isr();

	if (dec_dev_isr_state(dev_id) == DEC_OK)
		dec_dev_isr(dev_id);
exit:
	spin_unlock_irqrestore(&vdec->power_state_spin_lock, flag);
	return IRQ_HANDLED;
}

static hi_s32 vfmw_request_irq(hi_u32 irq_num_norm)
{
	vfmw_check_return(irq_num_norm != 0, VFMW_ERR);

	if (OS_REQUEST_IRQ(irq_num_norm, (vfmw_irq_handler_t)vfmw_isr,
		IRQF_SHARED, "vdec_norm_irq", (void *)&g_irq_handled) != 0) {
		dprint(PRN_FATAL, "request vdec norm irq %u failed\n", irq_num_norm);
		return VFMW_ERR;
	}
	g_irq_num_norm = irq_num_norm;

	return VFMW_OK;
}

static void vfmw_free_irq(hi_u32 irq_num_norm)
{
	if (irq_num_norm == 0) {
		dprint(PRN_ERROR, "irq num invalid.\n");
		return;
	}
	OS_FREE_IRQ(irq_num_norm, (void *)&g_irq_handled);
	g_irq_num_norm = 0;
}

#ifdef HI_TVP_SUPPORT
static int vdec_wait_suspend(void)
{
	return tvp_vdec_suspend();
}

static int vdec_wait_resume(void)
{
	return tvp_vdec_resume();
}

hi_s32 vdec_device_proc(hi_u32 *device_stat)
{
	dprint(PRN_DBG, "resume: %d, suspend: %d, device_stat: %p",
		g_vdec_tvp.resume, g_vdec_tvp.suspend, device_stat);
	*device_stat = DEVICE_UNKNOW;

	if (g_vdec_tvp.resume) {
		*device_stat = DEVICE_RESUME;
		OS_EVENT_GIVE(g_vdec_tvp.resume_event);
		return HI_SUCCESS;
	}

	if (g_vdec_tvp.suspend) {
		*device_stat = DEVICE_SUSPEND;
		OS_EVENT_GIVE(g_vdec_tvp.suspend_event);
		return HI_SUCCESS;
	}

	return HI_SUCCESS;
}

static int vdec_tvp_task(void *data)
{
	int ret;

	dprint(PRN_ALWS, "enter vdec tvp task\n");

	while (!kthread_should_stop()) {
		dprint(PRN_ALWS, "before wait event interruptible\n");
		ret = wait_event_interruptible(g_vdec_tvp.waitq,
			(g_vdec_tvp.resume || g_vdec_tvp.suspend ||
			(kthread_should_stop())));
		if (ret) {
			dprint(PRN_FATAL, "wait event interruptible failed\n");
			continue;
		}

		if (tvp_vdec_secure_init() != VDEC_OK)
			dprint(PRN_FATAL, "tvp vdec init failed\n");

		dprint(PRN_ALWS,
			"after wait event interruptible, resume:%d, suspend:%d\n",
			g_vdec_tvp.resume, g_vdec_tvp.suspend);

		if (g_vdec_tvp.resume) {
			dprint(PRN_ALWS, "tvp vdec resume\n");
			if (vdec_wait_resume() != VDEC_OK)
				dprint(PRN_FATAL, "tvp vdec resume failed\n");
			g_vdec_tvp.resume = false;
		}

		if (g_vdec_tvp.suspend) {
			dprint(PRN_ALWS, "tvp vdec supend\n");
			if (vdec_wait_suspend() != VDEC_OK)
				dprint(PRN_FATAL, "tvp vdec suspend failed\n");
			g_vdec_tvp.suspend = false;
		}

		tvp_vdec_secure_exit();
	}

	dprint(PRN_ALWS, "exit vdec tvp task\n");

	return HI_SUCCESS;
}
#endif

hi_s32 vfmw_init(void *args)
{
	hi_s32 ret;
	vdec_dts *dts_info = (vdec_dts *)args;

	vfmw_vdec_addr_init();
	ret = vfmw_request_irq(dts_info->irq_norm);
	if (ret != VFMW_OK) {
		dprint(PRN_FATAL, "vfmw_request_irq failed\n");
		goto request_irq_failure;
	}

	ret = pdt_init(&dts_info->module_reg[CRG_MOUDLE], dts_info->is_fpga,
		(void *)dts_info->dev);
	if (ret) {
		dprint(PRN_FATAL, "glb reset failed\n");
		goto pdt_init_failure;
	}

	ret = stm_dev_init(&dts_info->module_reg[SCD_MOUDLE]);
	if (ret != STM_OK) {
		dprint(PRN_FATAL, "stm_dev init failed\n");
		goto stm_dev_init_failure;
	}

	ret = dec_dev_init(&dts_info->module_reg[MFDE_MODULE]);
	if (ret != DEC_OK) {
		dprint(PRN_FATAL, "dec_dev init failed\n");
		goto dec_dev_init_failure;
	}
#ifdef HI_TVP_SUPPORT
	if (!g_vdec_tvp.task) {
		ret = memset_s(&g_vdec_tvp, sizeof(g_vdec_tvp), 0, sizeof(g_vdec_tvp));
		if (ret != EOK) {
			dprint(PRN_FATAL, "memset_s err in function\n");
			goto exit;
		}
		init_waitqueue_head(&g_vdec_tvp.waitq);

		OS_EVENT_INIT(&g_vdec_tvp.resume_event, 0);
		OS_EVENT_INIT(&g_vdec_tvp.suspend_event, 0);

		g_vdec_tvp.task = kthread_run(vdec_tvp_task, NULL, "vdec tvp task");
		if (IS_ERR(g_vdec_tvp.task)) {
			/* needn't return fail for normal video play */
			dprint(PRN_FATAL, "creat vdec tvp task failed\n");
			OS_EVENT_EXIT(g_vdec_tvp.resume_event);
			OS_EVENT_EXIT(g_vdec_tvp.suspend_event);
			ret = memset_s(&g_vdec_tvp, sizeof(g_vdec_tvp), 0,
				sizeof(g_vdec_tvp));
			if (ret != EOK) {
				dprint(PRN_FATAL, "memset_s err in function\n");
				goto exit;
			}
		}
	}
#endif
	return ret;

exit:
dec_dev_init_failure:
	stm_dev_deinit();
stm_dev_init_failure:
	pdt_deinit();
pdt_init_failure:
	vfmw_free_irq(g_irq_num_norm);
request_irq_failure:
	vfmw_vdec_addr_clear();

	return ret;
}

void vfmw_deinit(void)
{
	vfmw_vdec_addr_clear();

	dec_dev_deinit();

	stm_dev_deinit();

	pdt_deinit();

	vfmw_free_irq(g_irq_num_norm);
#ifdef HI_TVP_SUPPORT
	if (g_vdec_tvp.task) {
		kthread_stop(g_vdec_tvp.task);
		OS_EVENT_EXIT(g_vdec_tvp.resume_event);
		OS_EVENT_EXIT(g_vdec_tvp.suspend_event);
		(void)memset_s(&g_vdec_tvp, sizeof(g_vdec_tvp), 0,
			sizeof(g_vdec_tvp));
	}
#endif
}

static hi_s32 vfmw_stm_process(void *fd, void *stm_cfg, void *scd_state_reg)
{
	hi_s32 ret;

	vfmw_check_return(vfmw_stm_param_check(fd, stm_cfg) == VFMW_OK,
		 VFMW_ERR);
	ret = stm_dev_config_reg(stm_cfg, scd_state_reg);
	vfmw_update_pre_sb(fd, scd_state_reg);

	return ret;
}

static hi_s32 vfmw_get_cur_active_reg(void *fd, void *dev_cfg,
	void *vdh_state_reg)
{
	return get_cur_active_reg(dev_cfg);
}

static hi_s32 vfmw_dec_process(void *fd, void *dev_cfg, void *vdh_state_reg)
{
	vfmw_check_return(vfmw_dec_param_check(dev_cfg) == VFMW_OK, VFMW_ERR);
	vfmw_check_return(vfmw_reset_vdh(dev_cfg) == VFMW_OK, VFMW_ERR);
	vfmw_check_return(vfmw_dec_check_reg_id(dev_cfg) == VFMW_OK, VFMW_ERR);

	return dec_dev_config_reg(dev_cfg);
}

hi_s32 vfmw_query_image(void *dev_cfg, void *read_backup)
{
	vfmw_check_return(vfmw_dec_param_check(dev_cfg) == VFMW_OK, VFMW_ERR);

	return dec_dev_get_backup(dev_cfg, read_backup);
}

static vctrl_case g_vctrl_case_table[] = {
	{ VFMW_CID_STM_PROCESS, vfmw_stm_process },
	{ VFMW_CID_GET_ACTIVE_REG, vfmw_get_cur_active_reg },
	{ VFMW_CID_DEC_PROCESS, vfmw_dec_process },
	{ VFMW_CID_MAX_PROCESS, HI_NULL },
};

hi_s32 vfmw_control(struct file *file, vfmw_cmd_type cmd, void *arg_in,
	void *arg_out)
{
	hi_s32 ret = VFMW_ERR;
	hi_u32 index;
	hi_u32 case_num;
	vctrl_handler target_handler = HI_NULL;

	vfmw_check_return(cmd < VFMW_CID_MAX_PROCESS, VFMW_ERR);

	case_num = sizeof(g_vctrl_case_table) / sizeof(vctrl_case);
	for (index = 0; index < case_num; index++) {
		if (cmd == g_vctrl_case_table[index].cmd) {
			target_handler = g_vctrl_case_table[index].handler;
			break;
		}
	}

	if (target_handler) {
		ret = target_handler((void *)file, arg_in, arg_out);
	} else {
		dprint(PRN_FATAL, "invalid cmd id %d\n", cmd);
		ret = VFMW_ERR;
	}

	return ret;
}

#ifdef HI_TVP_SUPPORT
inline void notify_and_wait_tvp_process(bool *notify_flag)
{
	hi_s32 sleep_count = 0;
	*notify_flag = true;
	wake_up_interruptible(&g_vdec_tvp.waitq);
	do {
		OS_MSLEEP(WAIT_TASK_EXIT_INER);
		sleep_count++;
	} while (sleep_count < (WAIT_TASK_EXIT_TIMEOUT / WAIT_TASK_EXIT_INER));

	dprint(PRN_ALWS, "notify flag %d, sleep count %d\n", *notify_flag, sleep_count);
}
#endif

void vfmw_suspend(bool sec_instance_exist)
{
	stm_dev_suspend();
	dec_dev_suspend();
#ifdef HI_TVP_SUPPORT
	if (sec_instance_exist)
		notify_and_wait_tvp_process(&g_vdec_tvp.suspend);
#endif
}

void vfmw_resume(bool sec_instance_exist)
{
	stm_dev_resume();
	dec_dev_resume();
#ifdef HI_TVP_SUPPORT
	if (sec_instance_exist)
		notify_and_wait_tvp_process(&g_vdec_tvp.resume);
#endif
}

hi_bool vfmw_scene_ident(void)
{
	hi_u32 ident_info = dec_dev_read_store_info();
	if (ident_info != 0 && ident_info == current->tgid) {
	    return HI_TRUE;
	}

	return HI_FALSE;
}

static hi_s32 __init vfmw_mod_init(void)
{
	os_intf_init();
	stm_dev_init_entry();
	dec_dev_init_entry();

#ifdef VFMW_PROC_SUPPORT
	(void)vfmw_create_proc();
#endif
#ifdef HI_TVP_SUPPORT
	(void)memset_s(&g_vdec_tvp, sizeof(g_vdec_tvp), 0, sizeof(g_vdec_tvp));
#endif
	return 0;
}

static void __exit vfmw_mod_exit(void)
{
#ifdef VFMW_PROC_SUPPORT
	vfmw_destroy_proc();
#endif
	dec_dev_deinit_entry();
	stm_dev_deinit_entry();
	os_intf_exit();
}

module_init(vfmw_mod_init);
module_exit(vfmw_mod_exit);
