

#include <mspc_factory.h>
#include <mspc_errno.h>
#include <mspc.h>
#include <mspc_power.h>
#include <mspc_smc.h>
#include <mspc_fs.h>
#include <mspc_flash.h>
#include <mspc_upgrade.h>
#include <hisi_rpmb.h>
#include <hisee_mntn.h>
#include <linux/printk.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <securec.h>
#ifdef CONFIG_HUAWEI_DSM
#include <dsm/dsm_pub.h>
#endif

#define MSPC_WAIT_COS_READY_TIME         10000 /* 10s */
#define MSPC_WAIT_COS_DOWN_TIME          10000 /* 10s */
#define MSPC_STATE_FLASH_READY           MSPC_STATE_NATIVE_READY

#define MSPC_POLL_WAIT_TIME               100    /* 100ms */
#define MSPC_ENTER_FACTORY_MODE_WAIT_TIME 20000  /* 20s */

static struct mutex g_mspc_otp_mutex;

/* flag to indicate running status of flash otp1 */
enum otp1_status {
	NO_NEED = 0,
	PREPARED,
	RUNING,
	FINISH,
};

/* flag to indicate running status of flash otp1 */
static uint32_t g_mspc_flash_otp1_status;

static atomic_t g_mspc_secdebug_disable;

/* check otp1 write work is running */
/* flash_otp_task may not being created by set/get efuse _securitydebug_value */
bool mspc_chiptest_otp1_is_running(void)
{
	pr_info("mspc otp1 work status %x\n", g_mspc_flash_otp1_status);
	if (g_mspc_flash_otp1_status == RUNING) {
		return true;
	}

	if (g_mspc_flash_otp1_status == PREPARED &&
	    mspc_flash_is_task_started()) {
		return true;
	}

	return false;
}

void mspc_chiptest_set_otp1_status(enum otp1_status status)
{
	g_mspc_flash_otp1_status = status;
	pr_err("mspc set otp1 state:%x\n", g_mspc_flash_otp1_status);
}

void mspc_set_secdebug_status(bool is_debug_disable)
{
	atomic_set(&g_mspc_secdebug_disable, (int)is_debug_disable);
}

bool mspc_get_secdebug_status(void)
{
	return (bool)atomic_read(&g_mspc_secdebug_disable);
}

int32_t mspc_send_smc(uint32_t cmd_type)
{
	int32_t ret;
	int8_t *buff_virt = NULL;
	phys_addr_t buff_phy = 0;
	struct mspc_atf_msg_header *p_msg_header = NULL;
	uint32_t img_size = 0;
	struct device *device = mspc_get_device();

	if (!device) {
		pr_err("%s:Get mspc device failed!\n", __func__);
		return MSPC_INVALID_PARAM_ERROR;
	}

	buff_virt = (int8_t *)dma_alloc_coherent(device, SIZE_4K,
					       &buff_phy, GFP_KERNEL);
	if (!buff_virt) {
		pr_err("%s dma_alloc_coherent failed\n", __func__);
		return MSPC_NO_RESOURCES_ERROR;
	}
	(void)memset_s(buff_virt, SIZE_4K, 0, SIZE_4K);
	p_msg_header = (struct mspc_atf_msg_header *)buff_virt;
	mspc_set_atf_msg_header(p_msg_header, cmd_type);
	p_msg_header->cos_id = MSPC_FLASH_COS_ID;
	p_msg_header->result_addr = (uint32_t)buff_phy +
				    MSPC_ATF_MESSAGE_HEADER_LEN;
	p_msg_header->result_size = sizeof(uint32_t);
	img_size = MSPC_ATF_MESSAGE_HEADER_LEN + sizeof(uint32_t);
	ret = mspc_send_smc_process(p_msg_header, buff_phy, img_size,
	       MSPC_ATF_SMC_TIMEOUT, cmd_type);
	pr_err("%s result %x\n", __func__,
	       *(uint32_t *)(buff_virt + MSPC_ATF_MESSAGE_HEADER_LEN));
	dma_free_coherent(device, SIZE_4K, buff_virt, buff_phy);
	pr_err("%s exit\n", __func__);
	return ret;
}

static int32_t mspc_manufacture_check_lcs(void)
{
	int32_t ret;
	uint32_t mspc_lcs_mode = 0;

	ret = mspc_get_lcs_mode(&mspc_lcs_mode);
	if (ret != MSPC_OK) {
		pr_err("%s:get mspc lcs failed!\n", __func__);
		return ret;
	}

	if (mspc_lcs_mode != MSPC_SM_MODE_MAGIC) {
		pr_err("%s: mspc lcs isnot sm!\n", __func__);
		return MSPC_LCS_MODE_ERROR;
	}

	return MSPC_OK;
}

/* function to process for flash cos when securedebug disabled */
static int32_t mspc_flash_debug_flash_cos_fn(void)
{
	int32_t ret;

	ret = mspc_write_otp_image();
	if (ret != MSPC_OK) {
		pr_err("%s:write otp1 image failed\n", __func__);
		return ret;
	}
	/* delete flashcos image */
	ret = mspc_remove_flash_cos();
	if (ret != MSPC_OK) {
		pr_err("%s:remove flash cos failed\n", __func__);
		return ret;
	}

	return MSPC_OK;
}

/* function to process for flash cos when pinstall */
static int32_t mspc_pinstall_flash_cos_fn(void)
{
	int32_t ret;
	bool is_debug_disable = false;

	/* prepare secflash initial key for set urs */
	ret = mspc_send_smc(MSPC_SMC_SECFLASH_INITKEY);
	if (ret != MSPC_OK) {
		pr_err("%s:prepare secflash initkey failed\n", __func__);
		return ret;
	}

	is_debug_disable = mspc_get_secdebug_status();
	/* If secdebug is DebugDisable, write otp1 */
	if (is_debug_disable) {
		ret = mspc_write_otp_image();
		if (ret != MSPC_OK) {
			pr_err("%s:write otp1 image failed %x\n", __func__, ret);
			return ret;
		}
		pr_err("%s: flash debug disable\n", __func__);
	}
	return MSPC_OK;
}

/* power on flash cos, run fn, then power off */
int32_t mspc_run_flash_cos(int32_t (*fn)(void))
{
	int32_t ret, ret1;
	uint32_t time = (uint32_t)MSPC_WAIT_COS_READY_TIME;
	const int8_t *param = MSPC_FLASH_COS_POWER_PARAM;

	/* Power on flashCOS, then process fn */
	ret = mspc_power_func(param, strlen(param),
			      MSPC_POWER_ON_BOOTING, MSPC_POWER_CMD_ON);
	if (ret != MSPC_OK) {
		pr_err("%s:power on flash cos failed! ret=%d\n", __func__, ret);
		return ret;
	}

	ret = mspc_wait_state(MSPC_STATE_FLASH_READY, time);
	if (ret != MSPC_OK)
		pr_err("%s:wait flash cos ready timeout!\n", __func__);
	else
		ret = (fn) ? fn() : MSPC_OK;

	if (ret != MSPC_OK)
		hisee_mntn_debug_dump();

	/* power off flashCOS */
	ret1 = mspc_power_func(param, strlen(param),
			       MSPC_POWER_OFF, MSPC_POWER_CMD_OFF);
	if (ret1 != MSPC_OK) {
		pr_err("%s:power off flash cos failed!\n", __func__);
		goto exit;
	}

	/* Wait for powering down msp core. */
	time = (uint32_t)MSPC_WAIT_COS_DOWN_TIME;
	ret1 = mspc_wait_state(MSPC_STATE_POWER_DOWN, time);
	if (ret1 != MSPC_OK)
		pr_err("%s:wait flash cos down tiemout!\n", __func__);
exit:
	return (ret == MSPC_OK) ? ret1 : ret;
}

/* set secflash to urs mode */
static int32_t mspc_set_secflash_urs(void)
{
	int32_t ret, ret1;
	uint32_t time = (uint32_t)MSPC_WAIT_COS_READY_TIME;
	const int8_t *param = MSPC_NATIVE_COS_POWER_PARAM;

	/* Power on nativeCOS */
	ret = mspc_power_func(param, strlen(param),
			      MSPC_POWER_ON_BOOTING_SECFLASH, MSPC_POWER_CMD_ON);
	if (ret != MSPC_OK) {
		pr_err("%s:power on native cos failed! ret=%x\n", __func__, ret);
		return ret;
	}

	/* Wait for secflash ready */
	ret = mspc_wait_state(MSPC_STATE_SECFLASH, time);
	if (ret != MSPC_OK)
		pr_err("%s:wait secflash ready timeout\n", __func__);
	else
		ret = mspc_send_smc(MSPC_SMC_SECFLASH_SETURS);
	pr_err("%s:ret %x\n", __func__, ret);

	if (ret != MSPC_OK)
		hisee_mntn_debug_dump();

	/* power off nativeCOS */
	ret1 = mspc_power_func(param, strlen(param),
			       MSPC_POWER_OFF, MSPC_POWER_CMD_OFF);
	if (ret1 != MSPC_OK) {
		pr_err("%s:power off native cos failed!\n", __func__);
		goto exit;
	}

	/* Wait for powering down msp core. */
	time = (uint32_t)MSPC_WAIT_COS_DOWN_TIME;
	ret1 = mspc_wait_state(MSPC_STATE_POWER_DOWN, time);
	if (ret1 != MSPC_OK)
		pr_err("%s:wait native cos down tiemout!\n", __func__);
exit:
	return (ret == MSPC_OK) ? ret1 : ret;
}

int32_t mspc_enter_factory_mode(void)
{
	const int8_t *param = MSPC_NATIVE_COS_POWER_PARAM;
	int32_t wait_time = MSPC_ENTER_FACTORY_MODE_WAIT_TIME;
	int32_t vote_atf, vote_tee;
	int32_t ret;

	ret = mspc_power_func(param, strlen(param), MSPC_POWER_OFF, MSPC_POWER_CMD_OFF);
	if (ret != MSPC_OK) {
		pr_err("%s power off native cos failed\n", __func__);
		return ret;
	}

	while (wait_time > 0) {
		ret = mspc_send_atf_smc((uint64_t)MSPC_FN_MAIN_SERVICE_CMD,
					(uint64_t)MSPC_SMC_ENTER_FACTORY_MODE,
					0, (uint64_t)MSPC_NATIVE_COS_ID);
		if (ret == MSPC_OK)
			return ret;
		msleep(MSPC_POLL_WAIT_TIME);
		wait_time -= MSPC_POLL_WAIT_TIME;
	}
	pr_err("%s err %d\n", __func__, ret);

	vote_atf = mspc_send_atf_smc((uint64_t)HISEE_MNTN_ID,
				     (uint64_t)HISEE_SMC_GET_ATF_VOTE, 0, 0);
	vote_tee = mspc_send_atf_smc((uint64_t)HISEE_MNTN_ID,
				     (uint64_t)HISEE_SMC_GET_TEE_VOTE, 0, 0);
	pr_err("vote vote atf[0x%x] vote TEE[0x%x]\n", vote_atf, vote_tee);

	return MSPC_ENTER_FACTROY_MODE_ERROR;
}

int32_t mspc_exit_factory_mode(void)
{
	int32_t ret;

	ret = mspc_send_atf_smc((uint64_t)MSPC_FN_MAIN_SERVICE_CMD,
				(uint64_t)MSPC_SMC_EXIT_FACTORY_MODE,
				0, (uint64_t)MSPC_NATIVE_COS_ID);
	if (ret != MSPC_OK) {
		pr_err("%s err %d\n", __func__, ret);
		return MSPC_EXIT_FACTROY_MODE_ERROR;
	}

	return MSPC_OK;
}

/* do some check before pinstall test */
static int32_t mspc_pinstall_pre_check(void)
{
	int32_t ret;
	bool is_new_cos = false;

	ret = mspc_manufacture_check_lcs();
	if (ret != MSPC_OK)
		return ret;

	/* check rpmb key, should be ready */
	if (get_rpmb_key_status() != KEY_READY) {
		pr_err("%s rpmb key not ready\n", __func__);
		return MSPC_RPMB_KEY_UNREADY_ERROR;
	}

	ret = mspc_enter_factory_mode();
	if (ret != MSPC_OK) {
		pr_err("%s enter factory mode fail\n", __func__);
		return ret;
	}

	/* switch storage media to rpmb */
	ret = mspc_send_smc(MSPC_SMC_SWITCH_RPMB);
	if (ret != MSPC_OK) {
		pr_err("%s switch to rpmb fail\n", __func__);
		return ret;
	}

	/* check hisee_fs is erased */
	ret = mspc_check_hisee_fs_empty();
	if (ret != MSPC_OK) {
		pr_err("%s hisee_fs check fail\n", __func__);
		return ret;
	}

	/* check whether new image is upgraded */
	ret = mspc_check_new_image(&is_new_cos);
	if (ret != MSPC_OK || is_new_cos) {
		pr_err("%s:check boot upgrade fail\n", __func__);
		return MSPC_BOOT_UPGRADE_CHK_ERROR;
	}

	return MSPC_OK;
}

/* check whether secdebug disabled by efuse */
static int32_t mspc_check_secdebug_disable(void)
{
	int32_t ret;
	bool is_debug_disable = false;

	ret = efuse_check_secdebug_disable(&is_debug_disable);
	if (ret != OK) {
		pr_err("%s:check secdebug failed\n", __func__);
		return MSPC_CHECK_SECDEBUG_ERROR;
	}

	mspc_set_secdebug_status(is_debug_disable);
	return MSPC_OK;
}

/* things to do when secdebug is disabled */
static int32_t mspc_pinstall_secdebug_disabled(void)
{
	int32_t ret;
	bool is_debug_disable = mspc_get_secdebug_status();

	/* nothing to do if secdebug not disabled */
	if (!is_debug_disable)
		return MSPC_OK;

	/* set secflash to urs mode */
	ret = mspc_set_secflash_urs();
	if (ret != MSPC_OK) {
		pr_err("%s:set secflash urs err %d\n", __func__, ret);
		return ret;
	}

	/* delete flashcos image */
	ret = mspc_remove_flash_cos();
	if (ret != MSPC_OK) {
		pr_err("%s:remove flash cos err %d\n", __func__, ret);
		return ret;
	}

	return MSPC_OK;
}

/* function run when AT^MSPC=PINSTALL */
int32_t mspc_total_manufacture_func(void)
{
	int32_t ret;
	uint32_t state = 0;

	mspc_reinit_flash_complete();
	ret = mspc_pinstall_pre_check();
	if (ret != MSPC_OK) {
		pr_err("%s:pre_check failed\n", __func__);
		goto exit;
	}

	ret = mspc_check_flash_cos_file(&state);
	if (ret != MSPC_OK || state == FLASH_COS_EMPTY) {
		pr_err("%s:check flash cos failed\n", __func__);
		ret = MSPC_FLASH_COS_ERROR;
		goto exit;
	}

	/* Test already, exit directly. */
	if (state == FLASH_COS_ERASE) {
		pr_err("%s:flash cos is erased\n", __func__);
		goto exit;
	}

	/* check secdebug before run flashcos */
	ret = mspc_check_secdebug_disable();
	if (ret != MSPC_OK) {
		pr_err("%s:check secdebug failed\n", __func__);
		goto exit;
	}

	ret = mspc_run_flash_cos(mspc_pinstall_flash_cos_fn);
	if (ret != MSPC_OK) {
		pr_err("%s:run flash cos failed\n", __func__);
		goto exit;
	}

	ret = mspc_pinstall_secdebug_disabled();
exit:
	if (ret == MSPC_OK) {
		mspc_chiptest_set_otp1_status(PREPARED);
		/* sync signal for flash_otp_task */
		mspc_release_flash_complete();
	}

	mspc_record_errno(ret);
	return ret;
}


/* function run when send AT^SECUREDEBUG=, to rm debug switch in mspc */
int32_t mspc_flash_debug_switchs(void)
{
	int32_t ret;
	uint32_t state = 0;

	mutex_lock(&g_mspc_otp_mutex);
	mspc_chiptest_set_otp1_status(RUNING);

	ret = mspc_check_flash_cos_file(&state);
	if (ret != MSPC_OK || state == FLASH_COS_EMPTY) {
		pr_err("%s:check flash cos failed!\n", __func__);
		ret = MSPC_FLASH_COS_ERROR;
		goto exit;
	}

	/* Test already, exit directly. */
	if (state == FLASH_COS_ERASE) {
		pr_err("%s:flash cos is erased!\n", __func__);
		goto exit;
	}

	/* set secflash urs before erase flashcos */
	ret = mspc_set_secflash_urs();
	if (ret != MSPC_OK) {
		pr_err("%s:set secflash urs err, ret %d\n", __func__, ret);
		goto exit;
	}

	ret = mspc_run_flash_cos(mspc_flash_debug_flash_cos_fn);
	pr_err("%s: run flash cos ret %d\n", __func__, ret);

exit:
	mspc_chiptest_set_otp1_status(FINISH);
	mutex_unlock(&g_mspc_otp_mutex);

	mspc_record_errno(ret);
	return ret;
}

int32_t mspc_check_boot_upgrade(void)
{
	int32_t ret;
	bool is_new_cos = false;

	ret = mspc_manufacture_check_lcs();
	if (ret != MSPC_OK)
		return ret;

	/* check whether new image is upgraded */
	ret = mspc_check_new_image(&is_new_cos);
	if (ret != MSPC_OK) {
		pr_err("%s:check boot upgrade result failed!\n", __func__);
		return ret;
	}
	if (is_new_cos) {
		pr_err("%s:fail to upgrade to new img!\n", __func__);
#ifdef CONFIG_HUAWEI_DSM
		if (hisee_mntn_record_dmd_info(DSM_HISEE_POWER_ON_OFF_ERROR_NO,
					      "boot upgrade fail"))
			pr_err("%s:dmd info failed!\n", __func__);
#endif
		return MSPC_BOOT_UPGRADE_CHK_ERROR;
	}

	return MSPC_OK;
}

void mspc_factory_init(void)
{
	mutex_init(&g_mspc_otp_mutex);
}
