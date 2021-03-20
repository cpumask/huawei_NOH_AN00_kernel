

#include <mspc_smc.h>
#include <mspc.h>
#include <mspc_errno.h>
#include <mspc_upgrade.h>
#include <linux/sched.h>
#include <linux/arm-smccc.h>
#include <linux/mutex.h>
#include <linux/completion.h>
#include <linux/jiffies.h>
#include <linux/printk.h>

#define MSPC_ATF_MSG_ACK_ERROR       0xdeadbeef

static struct mutex g_smc_mutex;
static struct completion g_smc_completion;
static bool g_smc_cmd_run = false;

noinline int32_t mspc_send_atf_smc(uint64_t _function_id, uint64_t _arg0,
				   uint64_t _arg1, uint64_t _arg2)
{
	register uint64_t function_id asm("x0") = _function_id;
	register uint64_t arg0 asm("x1") = _arg0;
	register uint64_t arg1 asm("x2") = _arg1;
	register uint64_t arg2 asm("x3") = _arg2;

	asm volatile(
		__asmeq("%0", "x0")
		__asmeq("%1", "x1")
		__asmeq("%2", "x2")
		__asmeq("%3", "x3")
		"smc    #0\n"
		: "+r" (function_id)
		: "r" (arg0), "r" (arg1), "r" (arg2));

	return (int32_t)function_id;
}

int32_t mspc_send_smc_process(struct mspc_atf_msg_header *p_msg_header,
			      uint64_t phy_addr, uint64_t size,
			      uint32_t timeout, uint64_t smc_cmd)
{
	int32_t ret;
	uint64_t local_jiffies;
	uint64_t func_id = MSPC_FN_MAIN_SERVICE_CMD;

	if (!p_msg_header) {
		pr_err("%s: msg_header is NULL\n", __func__);
		return MSPC_INVALID_PARAMS_ERROR;
	}
	if (smc_cmd == MSPC_SMC_CHANNEL_AUTOTEST)
		func_id = MSPC_FN_CHANNEL_TEST_CMD;

	mutex_lock(&g_smc_mutex);
	g_smc_cmd_run = true;
	ret = mspc_send_atf_smc(func_id, (uint64_t)smc_cmd, phy_addr, size);
	if (ret != MSPC_OK) {
		pr_err("%s: send smc to ATF failed, ret=%d\n", __func__, ret);
		ret = MSPC_FIRST_SMC_CMD_ERROR;
		goto exit;
	}

	local_jiffies = msecs_to_jiffies(timeout);
	if (smc_cmd == MSPC_SMC_CHANNEL_AUTOTEST)
		local_jiffies = MAX_SCHEDULE_TIMEOUT;

	ret = wait_for_completion_timeout(&g_smc_completion, local_jiffies);
	if (ret == -ETIME) {
		pr_err("%s:wait seamphore timeout!\n", __func__);
		ret = MSPC_SMC_CMD_TIMEOUT_ERROR;
		goto exit;
	}

	if (p_msg_header->cmd != smc_cmd ||
	    p_msg_header->ack != MSPC_ATF_ACK_OK) {
		ret = MSPC_SMC_CMD_PROCESS_ERROR;
		pr_err("mspc:cmd=%llu %u, ack=%x, smc process error\n",
		       smc_cmd, p_msg_header->cmd, p_msg_header->ack);
	} else {
		ret = MSPC_OK;
		pr_err("%s: smc process successful!\n", __func__);
	}

exit:
	g_smc_cmd_run = false;
	mutex_unlock(&g_smc_mutex);
	mspc_record_errno(ret);
	return ret;
}

void mspc_set_atf_msg_header(struct mspc_atf_msg_header *header,
			     uint32_t cmd_type)
{
	if (!header) {
		pr_err("%s:NULL pointer! cmd:%x\n", __func__, cmd_type);
		return;
	}

	header->ack = MSPC_ATF_ACK_ERROR;
	header->cmd = cmd_type;
	header->result_size = 0;
	header->result_addr = 0;
}

void hisi_hisee_active(void)
{
	if (g_smc_cmd_run)
		complete(&g_smc_completion);
	else
		pr_err("%s:Receive an unexpected IPI from ATF", __func__);
}
EXPORT_SYMBOL(hisi_hisee_active);

void mspc_init_smc(void)
{
	mutex_init(&g_smc_mutex);
	init_completion(&g_smc_completion);
}
