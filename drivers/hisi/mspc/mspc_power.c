

#include <mspc_power.h>
#include <mspc.h>
#include <mspc_errno.h>
#include <mspc_upgrade.h>
#include <mspc_smc.h>
#include <hisee_mntn.h>
#include <securec.h>

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#define MSPC_POWER_PARAM_LEN              3
#define BLANK_INDEX                       0
#define COSID_INDEX                       1
#define VOTEID_INDEX                      2

#define MSPC_UPGRADE_PARAM_LEN            2
#define UPGRADE_TYPE_INDEX                1

#define MSPC_FORCE_OFF_TIMES              5
#define MSPC_WAIT_STATE_UNIT              20 /* ms */
#define MSPC_WAIT_STATE_MAX_TIME          1000000 /* 1000s */

#define MSPC_NATIVE_COS_READY             "NativeCos ready"
#define MSPC_NATIVE_COS_UNREADY           "NativeCos unready"
#define MSPC_READY_LEN                    3

static struct mutex g_power_mutex;


static union mspc_power_vote_status g_mspc_vote_status;
static uint32_t g_curr_cos_id = MSPC_MAX_COS_ID;
static uint32_t mspc_get_curr_cos_id(void)
{
	return g_curr_cos_id;
}

static void mspc_set_curr_cos_id(uint32_t cos_id)
{
	if (cos_id != MSPC_NATIVE_COS_ID && cos_id != MSPC_FLASH_COS_ID &&
	    cos_id != MSPC_MAX_COS_ID) {
		pr_err("%s:invalid cos_id:%u\n", __func__, cos_id);
		return;
	}
	g_curr_cos_id = cos_id;
}

static int32_t mspc_power_ctrl(uint32_t cos_id,
			       enum mspc_power_operation op_type,
			       enum mspc_power_cmd power_cmd)
{
	int32_t ret;
	uint64_t cmd_to_atf;
	uint64_t func_id = (uint64_t)MSPC_FN_MAIN_SERVICE_CMD;

	if (power_cmd == MSPC_POWER_CMD_ON)
		cmd_to_atf = (uint64_t)MSPC_SMC_POWER_ON;
	else
		cmd_to_atf = (uint64_t)MSPC_SMC_POWER_OFF;

	/* Send power command to atf */
	ret = mspc_send_atf_smc(func_id, cmd_to_atf, (uint64_t)op_type, cos_id);
	if (ret != MSPC_OK)
		pr_err("%s(): power_cmd=0x%x to atf failed! ret=%d\n",
		       __func__, power_cmd, ret);

	mspc_record_errno(ret);
	return ret;
}

static void mspc_set_vote_status(uint32_t vote_id, enum mspc_power_cmd cmd)
{
	uint32_t vote_unit;
	uint32_t vote_shift;

	vote_shift = (uint32_t)(MSPC_VOTE_UNIT_SIZE * vote_id);
	g_mspc_vote_status.value &= ~((uint64_t)MSPC_VOTE_MASK << vote_shift);

	if (cmd == MSPC_POWER_CMD_ON)
		vote_unit = (uint64_t)MSPC_VOTE_ON;
	else
		vote_unit = (uint64_t)MSPC_VOTE_OFF;

	g_mspc_vote_status.value |= vote_unit << vote_shift;
	pr_err("%s:vote status:0x%x\n", __func__, g_mspc_vote_status.value);
}

enum mspc_vote_status mspc_get_vote_status(void)
{
	if (g_mspc_vote_status.value != MSPC_VOTE_OFF_STATUS)
		return MSPC_VOTE_STATUS_ON;

	return MSPC_VOTE_STATUS_OFF;
}

static void mspc_clear_vote_status(void)
{
	g_mspc_vote_status.value = MSPC_VOTE_OFF_STATUS;
}

static int32_t mspc_power_vote(uint32_t cos_id, uint32_t vote_id,
			       enum mspc_power_operation op_type,
			       enum mspc_power_cmd cmd)
{
	int32_t ret = MSPC_OK;
	enum mspc_vote_status vote_status;

	vote_status = mspc_get_vote_status();

	if (vote_status == MSPC_VOTE_STATUS_OFF &&
	    cmd == MSPC_POWER_CMD_ON) {
		/* From the off status to on status. */
		ret = mspc_power_ctrl(cos_id, op_type, cmd);
		if (ret != MSPC_OK) {
			pr_err("%s: power on mspc failed!\n", __func__);
		} else {
			mspc_set_vote_status(vote_id, cmd);
			mspc_set_curr_cos_id(cos_id);
		}
	} else if (vote_status == MSPC_VOTE_STATUS_ON &&
		   cmd == MSPC_POWER_CMD_ON) {
		/* Mspc is already on, so just vote. */
		mspc_set_vote_status(vote_id, cmd);
	} else if (vote_status == MSPC_VOTE_STATUS_ON &&
		   cmd == MSPC_POWER_CMD_OFF) {
		/*
		 * Vote firstly, then check whether the vote_status is off,
		 * If the status is off, then power_off the mspc.
		 */
		mspc_set_vote_status(vote_id, cmd);

		vote_status = mspc_get_vote_status();
		if (vote_status == MSPC_VOTE_STATUS_OFF) {
			ret = mspc_power_ctrl(cos_id, op_type, cmd);
			if (ret != MSPC_OK) {
				/* Recover the vote if power off failed. */
				mspc_set_vote_status(vote_id,
						     MSPC_POWER_CMD_ON);
				pr_err("%s: power off mspc failed!\n",
				       __func__);
			}
			mspc_set_curr_cos_id(MSPC_MAX_COS_ID);
		}
	} else if (vote_status == MSPC_VOTE_STATUS_OFF &&
		   cmd == MSPC_POWER_CMD_OFF) {
		/* Mspc is already off */
		pr_err("%s: mspc is already off.\n",  __func__);
		ret = MSPC_OK;
	} else {
		pr_err("%s: power status(0x%x) or cmd(0x%x) error!\n",
		       __func__, vote_status, cmd);
		ret = MSPC_INVALID_PARAM_ERROR;
	}

	mspc_record_errno(ret);
	return ret;
}

static int32_t mspc_get_power_param(const int8_t *buff, uint32_t len,
				    uint32_t *cos_id, uint32_t *vote_id)
{
	const int8_t *argv = buff;

	if (!cos_id || !vote_id) {
		pr_err("%s:Null pointer!\n", __func__);
		return MSPC_INVALID_PARAM_ERROR;
	}

	if (!buff || *argv == MSPC_CHAR_NEWLINE || *argv == '\0') {
		/* If no args, use default id. */
		*cos_id = MSPC_NATIVE_COS_ID;
		*vote_id = MSPC_CHIP_TEST;
		return MSPC_OK;
	}

	/*
	 * Otherwise, the input must be "Bxx", the 'B' is blank,
	 * the first 'x' is the cos id and the last 'x' is the
	 * vote id. That is, argv[0] must be a blank, argv[1] is
	 * cos id and argv[2] is vote id. We Ignore the redundant
	 * chars after the "Bxx" if it exist.
	 */
	if (len >= MSPC_POWER_PARAM_LEN &&
	    argv[BLANK_INDEX] == MSPC_CHAR_SPACE &&
	    argv[COSID_INDEX] >= MSPC_CHAR_ZERO &&
	    argv[COSID_INDEX] < MSPC_CHAR_ZERO + MSPC_MAX_COS_ID &&
	    argv[VOTEID_INDEX] >= MSPC_CHAR_ZERO &&
	    argv[VOTEID_INDEX] < MSPC_CHAR_ZERO + MSPC_MAX_VOTE_ID) {
		*cos_id = argv[COSID_INDEX] - MSPC_CHAR_ZERO;
		*vote_id = argv[VOTEID_INDEX] - MSPC_CHAR_ZERO;
		pr_err("%s: cos id is %d, vote id is %d\n",
		       __func__, *cos_id, *vote_id);
		return MSPC_OK;
	}
	pr_err("mspc:Invalid input!Correct is:1 blank,1 cosid and 1 voteid!\n");
	return	MSPC_INVALID_PARAM_ERROR;
}

/* the param op_type is sent to lpm3 through ATF. */
int32_t mspc_power_func(const int8_t *buf, int32_t len,
			enum mspc_power_operation op_type,
			enum mspc_power_cmd cmd)
{
	uint32_t cos_id = 0;
	uint32_t vote_id = 0;
	int32_t ret;
	uint32_t cur_cos_id;

	mutex_lock(&g_power_mutex);

	ret = mspc_get_power_param(buf, len, &cos_id, &vote_id);
	if (ret != MSPC_OK) {
		pr_err("%s: get power parameter failed!\n", __func__);
		goto exit;
	}

	/*
	 * MSP core only allow power nativieCos(cos_id = 1)
	 * and flashCos(cos_id = 5) in kernel.
	 */
	if ((cos_id != MSPC_NATIVE_COS_ID && cos_id != MSPC_FLASH_COS_ID) ||
	    vote_id >= MSPC_MAX_VOTE_ID) {
		ret = MSPC_INVALID_PARAM_ERROR;
		pr_err("%s:Invalid param, cos_id:%u, vote_id:%u\n",
		       __func__, cos_id, vote_id);
		goto exit;
	}

	/*
	 * if some cos_id has power on, when the other one want to power on,
	 * it need wait cur cos_id power off.
	 */
	cur_cos_id = mspc_get_curr_cos_id();
	if (cur_cos_id != MSPC_MAX_COS_ID && cos_id != cur_cos_id) {
		ret = MSPC_INVALID_PARAM_ERROR;
		pr_err("%s:cos_id:%u has power on, cos_id:%u need wait\n",
		       __func__, cur_cos_id, cos_id);
		goto exit;
	}

	ret = mspc_power_vote(cos_id, vote_id, op_type, cmd);
	if (ret != MSPC_OK)
		pr_err("%s: power vote failed!\n", __func__);

exit:
	mutex_unlock(&g_power_mutex);
	mspc_record_errno(ret);
	return ret;
}


static void mspc_force_off_in_suspend(void)
{
	int32_t ret;
	uint8_t retry = (uint8_t)MSPC_FORCE_OFF_TIMES;
	uint32_t cos_id = mspc_get_curr_cos_id();

	do {
		ret = mspc_power_ctrl(cos_id, MSPC_POWER_OFF,
				      MSPC_POWER_CMD_OFF);
		if (ret == MSPC_OK)
			break;
		retry--;
	} while (retry > 0);

	if (retry == 0)
		pr_err("mspc error! mspc cannot been powered off.\n");

	pr_err("mspc force off successful!\n");
}

int32_t mspc_suspend(struct platform_device *pdev, struct pm_message state)
{
	pr_err("hisi_mspc_suspend: +\n");

	mutex_lock(&g_power_mutex);

	if (mspc_get_vote_status() != MSPC_VOTE_STATUS_OFF) {
		pr_err("Error:mspc hasnot been power off, vote status:0x%x\n",
		       g_mspc_vote_status.value);
		mspc_clear_vote_status();
		mspc_force_off_in_suspend();
	}

	mutex_unlock(&g_power_mutex);

	pr_err("hisi_mspc_suspend: -\n");
	return OK;
}


ssize_t mspc_check_ready_show(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	enum mspc_state state;
	int32_t ret;
	uint64_t func_id = (uint64_t)MSPC_FN_MAIN_SERVICE_CMD;
	uint64_t smc_cmd = (uint64_t)MSPC_SMC_GET_STATE;
	const int8_t *result = NULL;
	uint64_t num;

	if (!buf) {
		pr_err("%s: NULL pointer!\n", __func__);
		return MSPC_INVALID_PARAM_ERROR;
	}

	state = (enum mspc_state)mspc_send_atf_smc(func_id,
		 smc_cmd, (uint64_t)0, (uint64_t)MSPC_NATIVE_COS_ID);

	if (state == MSPC_STATE_NATIVE_READY) {
		num = 0; /* 0 indicates ready. */
		result = MSPC_NATIVE_COS_READY;
	} else {
		num = 1; /* 1 indicates unready. */
		result = MSPC_NATIVE_COS_UNREADY;
	}

	ret = snprintf_s(buf, MSPC_BUF_SHOW_LEN, MSPC_READY_LEN, "%d,", num);
	if (ret == MSPC_SECLIB_ERROR) {
		pr_err("%s: snprintf err.\n", __func__);
		return ret;
	}

	ret = strncat_s(buf, MSPC_BUF_SHOW_LEN, result, strlen(result));
	if (ret != EOK) {
		pr_err("%s: strncat err.\n", __func__);
		return ret;
	}

	pr_err("%s: state=%d, %s!\n", __func__, state, buf);
	return (ssize_t)strlen(buf);
}

/**
 * @brief      : mspc_powerctrl_show : Show the power status of MSP core.
 *		 Include kernel status, teeos status, ATF status and lpmcu
 *		 status.
 *
 * @param[in]  : dev  : Devices pointer.
 * @param[in]  : attr : Devices attribution.
 * @param[out] : buf  : The buffer to store out data.
 *
 * @return     : >= 0: The size of output data, Others: failed.
 */
ssize_t mspc_powerctrl_show(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	uint32_t vote_lpm3;
	uint32_t vote_atf;
	uint64_t vote_tee;
	uint32_t vote_kernel;
	int32_t ret;

	if (!buf) {
		pr_err("%s: NULL pointer!\n", __func__);
		return MSPC_INVALID_PARAM_ERROR;
	}

	hisee_mntn_collect_vote_value_cmd();
	vote_lpm3 = hisee_mntn_get_vote_val_lpm3();
	vote_atf = hisee_mntn_get_vote_val_atf();
	vote_tee = mspc_mntn_get_vote_val_tee();
	vote_kernel = g_mspc_vote_status.value;

	ret = snprintf_s(buf, MSPC_BUF_SHOW_LEN, MSPC_BUF_SHOW_LEN - 1,
			 "lpm3 0x%08x, atf 0x%08x, tee 0x%llx, kernel 0x%08x!",
			 vote_lpm3, vote_atf, vote_tee, vote_kernel);
	if (ret == MSPC_SECLIB_ERROR) {
		pr_err("%s snprintf err.\n", __func__);
		return ret;
	}

	pr_err("mspc vote: %s\n", buf);
	return strlen(buf);
}

/**
 * @brief      : mspc_wait_state : Wait for the specify state in the
 *		 specify time.
 *
 * @param[in]  : expected_state : The state to wait.
 * @param[in]  : ms : The time to wait.
 *
 * @return     : MSPC_OK: successful, Others: failed.
 */
int32_t mspc_wait_state(enum mspc_state expected_state, uint32_t ms)
{
	uint64_t func_id = (uint64_t)MSPC_FN_MAIN_SERVICE_CMD;
	uint64_t smc_cmd = (uint64_t)MSPC_SMC_GET_STATE;
	enum mspc_state state = 0;
	uint32_t unit = (uint32_t)MSPC_WAIT_STATE_UNIT;
	uint32_t timeout = 0;

	if (ms >= MSPC_WAIT_STATE_MAX_TIME) {
		pr_err("%s:Invalid time:%u\n", __func__, ms);
		return MSPC_INVALID_PARAM_ERROR;
	}

	/* At least wait a unit(20ms) time. */
	ms = ms < unit ? unit : ms;
	do {
		state = (enum mspc_state)mspc_send_atf_smc(func_id,
			smc_cmd, (uint64_t)0, (uint64_t)MSPC_NATIVE_COS_ID);
		if (state == expected_state)
			return MSPC_OK;
		msleep(unit);
		/* timeout max is MSPC_WAIT_STATE_MAX_TIME + 20. */
		timeout += unit;
	} while (timeout <= ms);

	return MSPC_WAIT_READY_TIMEOUT_ERROR;
}

void mspc_init_power(void)
{
	mutex_init(&g_power_mutex);
	g_mspc_vote_status.value = MSPC_VOTE_OFF_STATUS;
}
