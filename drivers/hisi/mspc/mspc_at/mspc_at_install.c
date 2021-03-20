
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <mspc.h>
#include <mspc_errno.h>
#include <mspc_factory.h>
#include <mspc_fs.h>

#ifndef UNUSED_PARAM
#define UNUSED_PARAM(param)                (void)(param)
#endif

#define MSPC_AT_INSTALL_RUNNING_RSP_STR    "Running"
#define MSPC_AT_INSTALL_SUCCESS_RSP_STR    "PASS"
#define MSPC_AT_INSTALL_FAIL_RSP_STR       "FAIL"

struct mspc_at_install {
	enum mspc_factory_test_state state;
	int32_t errcode;
};

static DEFINE_MUTEX(g_mspc_at_install_lock);

static struct mspc_at_install g_mspc_at_install = {
	.state = MSPC_FACTORY_TEST_NORUNNING,
	.errcode = MSPC_ERROR,
};

static inline void mspc_at_set_install_state(enum mspc_factory_test_state state)
{
	mutex_lock(&g_mspc_at_install_lock);
	g_mspc_at_install.state = state;
	mutex_unlock(&g_mspc_at_install_lock);
}

static inline enum mspc_factory_test_state mspc_at_get_install_state(void)
{
	return g_mspc_at_install.state;
}

static inline void mspc_at_set_install_errcode(int32_t errcode)
{
	mutex_lock(&g_mspc_at_install_lock);
	g_mspc_at_install.errcode = errcode;
	mutex_unlock(&g_mspc_at_install_lock);
}

static inline int32_t mspc_at_get_install_errcode(void)
{
	return g_mspc_at_install.errcode;
}

static inline void mspc_at_set_install_state_and_errcode(enum mspc_factory_test_state state, int32_t errcode)
{
	mspc_at_set_install_state(state);
	mspc_at_set_install_errcode(errcode);
}

static int32_t mspc_at_install_task(void *arg)
{
	int32_t ret;
	enum mspc_factory_test_state state;

	state = mspc_at_get_install_state();
	if (state != MSPC_FACTORY_TEST_RUNNING) {
		pr_err("%s:install test state error: %x\n", __func__, state);
		mspc_at_set_install_state_and_errcode(MSPC_FACTORY_TEST_FAIL, MSPC_AT_INSTALL_STATE_ERROR);
		return MSPC_AT_INSTALL_STATE_ERROR;
	}

	ret = mspc_total_manufacture_func();
	if (ret != MSPC_OK) {
		pr_err("%s:run failed!\n", __func__);
		mspc_at_set_install_state_and_errcode(MSPC_FACTORY_TEST_FAIL, ret);
	} else {
		pr_info("%s:run successful!\n", __func__);
		mspc_at_set_install_state_and_errcode(MSPC_FACTORY_TEST_SUCCESS, ret);
	}

	return ret;
}

static int32_t mspc_at_install_without_secflash_task(void *arg)
{
	int32_t ret;
	uint32_t state = 0;

	ret = mspc_check_flash_cos_file(&state);
	if (ret != MSPC_OK || state == FLASH_COS_EMPTY) {
		pr_err("%s:check flash cos failed\n", __func__);
		ret = MSPC_FLASH_COS_ERROR;
		goto exit;
	}

	/* Test already, exit directly. */
	if (state == FLASH_COS_ERASE) {
		pr_err("%s:flash cos is erased\n", __func__);
		ret = MSPC_OK;
		goto exit;
	}

	/* delete flashcos image */
	ret = mspc_remove_flash_cos();
	if (ret != MSPC_OK)
		pr_err("%s:remove flash cos err %d\n", __func__, ret);

exit:
	if (ret != MSPC_OK) {
		pr_err("%s:run failed!\n", __func__);
		mspc_at_set_install_state_and_errcode(MSPC_FACTORY_TEST_FAIL, ret);
	} else {
		pr_info("%s:run successful!\n", __func__);
		mspc_at_set_install_state_and_errcode(MSPC_FACTORY_TEST_SUCCESS, ret);
	}

	return ret;
}

bool mspc_at_install_bypass(void)
{
	int32_t ret;
	uint32_t status_result = SECFLASH_IS_ABSENCE_MAGIC;

	ret = mspc_secflash_is_available(&status_result);
	if (ret != MSPC_OK) {
		pr_err("%s: get secflash dts info fail\n", __func__);
		return false;
	}
	if (status_result == SECFLASH_IS_ABSENCE_MAGIC) {
		pr_info("%s: secflash dts is disable, bypass install\n", __func__);
		return true;
	}
	return false;
}

/*
 * @brief: Execute MSPC install test.
 * @param[in]: data - not currently used.
 * @param[in]: resp - MSPC at response callback function, not currently used.
 * @return: if success return MSPC_OK, others are failure.
 */
int32_t mspc_at_execute_install(const char *data, int32_t (*resp)(const char *buf, size_t len))
{
	struct task_struct *install_task = NULL;

	UNUSED_PARAM(data);
	UNUSED_PARAM(resp);

	if (mspc_at_get_install_state() == MSPC_FACTORY_TEST_RUNNING ||
	    mspc_chiptest_otp1_is_running()) {
		pr_err("%s:install test or otp1 test already run!\n", __func__);
		return MSPC_OK;
	}

	mspc_at_set_install_state(MSPC_FACTORY_TEST_RUNNING);
	if (mspc_at_install_bypass()) {
		pr_info("%s: bypass, install without secflash!\n", __func__);
		install_task = kthread_run(mspc_at_install_without_secflash_task, NULL,
					   "mspc_bypass_install_task");
	} else {
		pr_info("%s: install not bypass!\n", __func__);
		install_task = kthread_run(mspc_at_install_task, NULL, "mspc_at_install_task");
	}
	if (!install_task) {
		pr_err("%s:create mspc_at_install_task failed!\n", __func__);
		mspc_at_set_install_state_and_errcode(MSPC_FACTORY_TEST_FAIL, MSPC_THREAD_CREATE_ERROR);
		return MSPC_THREAD_CREATE_ERROR;
	}

	return MSPC_OK;
}

/*
 * @brief: Check MSPC install test result.
 * @param[in]: data - not currently used.
 * @param[in]: resp - MSPC at response callback function, not currently used.
 * @return: if success return MSPC_OK, others are failure.
 */
int32_t mspc_at_check_install(const char *data, int32_t (*resp)(const char *buf, size_t len))
{
	enum mspc_factory_test_state state = mspc_at_get_install_state();
	int32_t errcode = mspc_at_get_install_errcode();
	int32_t ret;

	UNUSED_PARAM(data);

	if (!resp)
		return MSPC_AT_RSP_CALLBACK_NULL_ERROR;

	if (state == MSPC_FACTORY_TEST_NORUNNING)
		return MSPC_AT_CMD_NOTRUNNING_ERROR;

	if (mspc_at_install_bypass()) {
		pr_info("%s: bypass, check result of install without secflash!\n", __func__);
		if (state == MSPC_FACTORY_TEST_RUNNING)
			return resp(MSPC_AT_INSTALL_RUNNING_RSP_STR, sizeof(MSPC_AT_INSTALL_RUNNING_RSP_STR));

		if (state == MSPC_FACTORY_TEST_SUCCESS && errcode == MSPC_OK)
			return resp(MSPC_AT_INSTALL_SUCCESS_RSP_STR, sizeof(MSPC_AT_INSTALL_SUCCESS_RSP_STR));

		pr_err("%s: failed, result = %d\n", __func__, errcode);
		(void)resp(MSPC_AT_INSTALL_FAIL_RSP_STR, sizeof(MSPC_AT_INSTALL_FAIL_RSP_STR));
		return errcode;
	}

	if (state == MSPC_FACTORY_TEST_RUNNING || mspc_chiptest_otp1_is_running())
		return resp(MSPC_AT_INSTALL_RUNNING_RSP_STR, sizeof(MSPC_AT_INSTALL_RUNNING_RSP_STR));

	if (state == MSPC_FACTORY_TEST_SUCCESS && errcode == MSPC_OK) {
		ret = mspc_exit_factory_mode();
		if (ret != MSPC_OK) {
			(void)resp(MSPC_AT_INSTALL_FAIL_RSP_STR, sizeof(MSPC_AT_INSTALL_FAIL_RSP_STR));
			return ret;
		}
		return resp(MSPC_AT_INSTALL_SUCCESS_RSP_STR, sizeof(MSPC_AT_INSTALL_SUCCESS_RSP_STR));
	}

	if (state == MSPC_FACTORY_TEST_FAIL && errcode != MSPC_OK) {
		(void)mspc_exit_factory_mode();
		(void)resp(MSPC_AT_INSTALL_FAIL_RSP_STR, sizeof(MSPC_AT_INSTALL_FAIL_RSP_STR));
		return errcode;
	}

	pr_err("%s: failed, state %d, errcode 0x%x\n", __func__, (int32_t)state, errcode);
	(void)resp(MSPC_AT_INSTALL_FAIL_RSP_STR, sizeof(MSPC_AT_INSTALL_FAIL_RSP_STR));
	return MSPC_AT_INSTALL_STATE_ERROR;
}

