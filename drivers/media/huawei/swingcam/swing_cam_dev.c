/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: swing driver
 * Author: Huawei
 * Create: 2019-03-01
 */

#include <linux/mutex.h>
#include <linux/kfifo.h>
#include <linux/hisi/hisi_ion.h>
#include <linux/hisi-iommu.h>
#include <media/huawei/camera.h>
#include <media/v4l2-event.h>
#include <media/v4l2-subdev.h>
#include <media/videobuf2-core.h>
#include <securec.h>
#include "inputhub_api.h"
#include "common.h"
#include "shmem.h"
#include "swing_cam_dev.h"
#include "hwcam_intf.h"

static struct swingcam_priv g_swing_priv = {0};
static struct miniisp_t g_miniIsp = {
	.name = "miniisp",
};

enum timestamp_state_t {
	TIMESTAMP_UNINITIAL = 0,
	TIMESTAMP_INITIAL,
};

static enum timestamp_state_t s_timestamp_state;
static struct timeval s_timeval;
static struct timespec64 s_ts;

static void swing_cam_init_timestamp(void)
{
	s_timestamp_state = TIMESTAMP_INITIAL;
	get_monotonic_boottime64(&s_ts);
	do_gettimeofday(&s_timeval);
}

static void swing_cam_destroy_timestamp(void)
{
	s_timestamp_state = TIMESTAMP_UNINITIAL;
	memset_s(&s_timeval, sizeof(s_timeval), 0x00, sizeof(s_timeval));
	memset_s(&s_ts, sizeof(s_ts), 0x00, sizeof(s_ts));
}

static void swing_cam_set_timestamp(u32 *timestampH, u32 *timestampL)
{
#define MICROSECOND_PER_SECOND     (1000000)
#define NANOSECOND_PER_MICROSECOND (1000)
	u64 fw_micro_second = 0;
	u64 fw_sys_counter = 0;
	u64 micro_second = 0;

	if (s_timestamp_state == TIMESTAMP_UNINITIAL) {
		swing_cam_log_err("%s wouldn't enter this branch.", __func__);
		swing_cam_init_timestamp();
	}

	if (timestampH == NULL || timestampL == NULL) {
		swing_cam_log_err("%s timestampH or timestampL is null.",
				__func__);
		return;
	}

	if (*timestampH == 0 && *timestampL == 0)
		return;

	fw_sys_counter = ((u64)(*timestampH) << 32) | (u64)(*timestampL);
	micro_second = (fw_sys_counter / NANOSECOND_PER_MICROSECOND) -
			s_ts.tv_sec * MICROSECOND_PER_SECOND -
			s_ts.tv_nsec/NANOSECOND_PER_MICROSECOND;

	fw_micro_second =
		(micro_second / MICROSECOND_PER_SECOND + s_timeval.tv_sec) *
		MICROSECOND_PER_SECOND +
		((micro_second % MICROSECOND_PER_SECOND) + s_timeval.tv_usec);

	*timestampH = (u32)(fw_micro_second >> 32 & 0xFFFFFFFF);
	*timestampL = (u32)(fw_micro_second & 0xFFFFFFFF);
}

static int swing_cam_enqueue(unsigned char *buf,
					unsigned int len,
					enum miniisp_event_kind cur_kind)
{
	struct swingcam_data read_data = {0};
	int ret;
	unsigned int length;
	struct v4l2_event ev;
	struct video_device *vdev = NULL;
	struct miniisp_event *req = NULL;

	length = sizeof(struct swingcam_data);
	mutex_lock(&g_swing_priv.swing_mutex);
	if (g_swing_priv.ref_cnt == 0) {
		swing_cam_log_info("%s: ref cnt is 0.\n", __func__);
		mutex_unlock(&g_swing_priv.swing_mutex);
		return -1;
	}

	if (kfifo_avail(&g_swing_priv.read_kfifo) < length) {
		swing_cam_log_err("%s read_kfifo is full, drop upload data.\n",
			__func__);
		ret = -EFAULT;
		mutex_unlock(&g_swing_priv.swing_mutex);
		goto RET_ERR;
	}

	read_data.recv_len = len;
	read_data.p_recv = kzalloc(read_data.recv_len, GFP_ATOMIC);
	if (read_data.p_recv == NULL) {
		swing_cam_log_err("Failed to alloc memory to save resp...\n");
		ret = -EFAULT;
		mutex_unlock(&g_swing_priv.swing_mutex);
		goto RET_ERR;
	}

	ret = memcpy_s(read_data.p_recv, read_data.recv_len,
					buf, read_data.recv_len);
	if (ret != 0) {
		swing_cam_log_err("%s memcpy_s failed...\n", __func__);
		ret = -EFAULT;
		mutex_unlock(&g_swing_priv.swing_mutex);
		goto RET_ERR;
	}

	/*lint -e712*/
	length = kfifo_in(&g_swing_priv.read_kfifo, (unsigned char *)&read_data,
				sizeof(struct swingcam_data));
	mutex_unlock(&g_swing_priv.swing_mutex);
	/*lint +e712*/
	if (length == 0) {
		swing_cam_log_err("%s: kfifo_in failed\n", __func__);
		ret = -EFAULT;
		goto RET_ERR;
	}

	req = (struct miniisp_event *)ev.u.data;
	memset_s(&ev, sizeof(struct v4l2_event), 0, sizeof(struct v4l2_event));

	vdev = g_miniIsp.subdev.devnode;

	if (vdev == NULL) {
		swing_cam_log_err("%s vdec is null\n", __func__);
		ret = -EFAULT;
		goto RET_ERR;
	}

	ev.type = MINIISP_V4L2_EVENT_TYPE;
	ev.id = MINIISP_HIGH_PRIO_EVENT;
	req->kind = cur_kind;

	v4l2_event_queue(vdev, &ev);

	return 0;

RET_ERR:
	if (read_data.p_recv != NULL)
		kfree(read_data.p_recv);

	return ret;
}

static int swing_cam_get_resp(const pkt_header_t *head)
{
	pkt_subcmd_resp_t *p_resp = NULL;
	int ret = 0;
	struct miniisp_msg_t *p_msg_ack = NULL;

	p_resp = (pkt_subcmd_resp_t *)(head);

	if (p_resp == NULL)
		return -EFAULT;

	if (p_resp->hd.tag != TAG_SWING_CAM)
		return -EFAULT;

	p_msg_ack = (struct miniisp_msg_t *)(p_resp + 1);

	swing_cam_log_info("%s cmd[%d], subcmd[%d] length[%d]\n", __func__,
		p_resp->hd.cmd, p_resp->subcmd, p_resp->hd.length);

	swing_cam_log_info("%s tag[%d], errno[%d], tranid[%d]\n", __func__,
		p_resp->hd.tag, p_resp->hd.errno, p_resp->hd.tranid);

	swing_cam_log_info("%s size[%d], name[%d] id[%d]\n", __func__,
		p_msg_ack->message_size, p_msg_ack->api_name,
		p_msg_ack->message_id);

	if (p_resp->hd.cmd == CMD_CMN_CLOSE_RESP) {
		swing_cam_log_info("Close Ack Received.\n");
		ret = swing_cam_enqueue((unsigned char *)p_msg_ack,
			sizeof(struct miniisp_msg_t), MINIISP_MSG_CB);
		return ret;
	}

	switch (p_resp->subcmd) {
	case SUB_CMD_SWING_CAM_CONFIG:
		swing_cam_log_info("Config Ack Received.\n");
		break;

	case SUB_CMD_SWING_CAM_CAPTURE:
		swing_cam_log_info("Capture Ack Received.\n");
		swing_cam_set_timestamp(
			&(p_msg_ack->u.ack_miniisp_request.timestampH),
			&(p_msg_ack->u.ack_miniisp_request.timestampL));
		break;

	case SUB_CMD_SWING_CAM_MATCH_ID:
		swing_cam_log_info("Received, match id, status:%d\n",
			p_msg_ack->u.ack_query_swing_cam.status);
		break;

	case SUB_CMD_SWING_CAM_ACQUIRE:
		swing_cam_log_info("Received, csi_index:%u init_ret:%d\n",
			p_msg_ack->u.ack_acquire_swing_cam.csi_index,
			p_msg_ack->u.ack_acquire_swing_cam.init_ret);
		break;

	case SUB_CMD_SWING_CAM_EXTEND:
		swing_cam_log_info("%s Received, extend_cmd:%d\n", __func__,
			p_msg_ack->u.ack_extend_swing_set.extend_cmd);
		break;
	case SUB_CMD_SWING_GET_OTP:
		swing_cam_log_info("%s Received, get_otp status:%d\n", __func__,
			p_msg_ack->u.ack_get_ao_otp.status);
		break;

	default:
		swing_cam_log_err("unhandled cmd: tag[%d], sub_cmd[%d]\n",
			p_resp->hd.tag, p_resp->subcmd);
		ret = -EFAULT;
		break;
	}

	if (ret != 0)
		return ret;

	ret = swing_cam_enqueue((unsigned char *)p_msg_ack,
		sizeof(struct miniisp_msg_t), MINIISP_MSG_CB);

	return ret;
}

static void swing_cam_clear_fifo()
{
	int i = 0;
	u32 length = sizeof(struct swingcam_data);
	unsigned int ret = 0;
	struct swingcam_data read_data = {0};

	while (i < SWING_READ_CACHE_COUNT) {
		if (kfifo_len(&g_swing_priv.read_kfifo) < length) {
			swing_cam_log_err("%s: kfifo len is less than swing data i=%d.\n", __func__, i);
			break;
		}

		/*lint -e712*/
		ret = kfifo_out(&g_swing_priv.read_kfifo,
				(unsigned char *)&read_data, length);
		/*lint +e712*/
		if (ret == 0) {
			swing_cam_log_err("%s: kfifo out failed i=%d.\n", __func__, i);
			break;
		}

		if (read_data.p_recv != NULL) {
			kfree(read_data.p_recv);
			read_data.p_recv = NULL;
			read_data.recv_len = 0;
		}

		i++;
	}
	kfifo_free(&g_swing_priv.read_kfifo);

	return;
}

static int swing_cam_dev_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	swing_cam_log_info("enter %s..\n", __func__);

	mutex_lock(&g_swing_priv.swing_mutex);

	if (g_swing_priv.ref_cnt == 0) {
		if (kfifo_alloc(&g_swing_priv.read_kfifo,
			sizeof(struct swingcam_data) * SWING_READ_CACHE_COUNT,
			GFP_KERNEL)) {
			swing_cam_log_err("%s kfifo alloc failed.\n", __func__);
			mutex_unlock(&g_swing_priv.swing_mutex);
			return -ENOMEM;
		}
	}

	g_swing_priv.ref_cnt++;

	mutex_unlock(&g_swing_priv.swing_mutex);

	return 0;
}

static int swing_cam_dev_release(struct v4l2_subdev *sd,
				struct v4l2_subdev_fh *fh)
{
	swing_cam_log_info("enter %s..\n", __func__);

	mutex_lock(&g_swing_priv.swing_mutex);
	if (g_swing_priv.ref_cnt == 0) {
		swing_cam_log_err("%s: ref cnt is 0.\n", __func__);
		mutex_unlock(&g_swing_priv.swing_mutex);
		return -EFAULT;
	}

	g_swing_priv.ref_cnt--;

	if (g_swing_priv.ref_cnt == 0) {
		send_cmd_from_kernel(TAG_SWING_CAM,
				CMD_CMN_CLOSE_REQ, 0, NULL, (size_t)0);
		swing_cam_log_info("enter %s..\n", __func__);
		swing_cam_clear_fifo();
		swing_cam_log_info("finish!\n");
	}

	mutex_unlock(&g_swing_priv.swing_mutex);

	return 0;
}

static int miniisp_reset_notify(void)
{
	int ret = 0;
	struct miniisp_msg_t msg_ack = {0};

	msg_ack.u.event_sent.event_id = EVENT_MINI_RECOVER_CODE;
	msg_ack.api_name = MINI_EVENT_SENT;
	ret = swing_cam_enqueue((unsigned char *)(&msg_ack),
		sizeof(struct miniisp_msg_t), MINIISP_MSG_CB);
	swing_cam_log_info("enter [%s]\n", __func__);
	return ret;
}

static int miniisp_reset_notifier(struct notifier_block *nb,
					unsigned long action, void *data)
{
	int ret = 0;

	switch (action) {
	case IOM3_RECOVERY_IDLE:
		ret = miniisp_reset_notify();
		break;
	default:
		break;
	}

	return ret;
}

static struct notifier_block miniisp_reboot_notify = {
	.notifier_call = miniisp_reset_notifier,
	.priority = -1,
};

static int __init swing_cam_dev_init(struct platform_device *pdev)
{
	int ret = 0;

	swing_cam_log_info("%s enter\n", __func__);

	if (is_sensorhub_disabled()) {
		swing_cam_log_err("sensorhub disabled....\n");
		return -EFAULT;
	}

	mutex_init(&g_swing_priv.swing_mutex);

	ret = register_mcu_event_notifier(TAG_SWING_CAM,
		CMD_CMN_CONFIG_RESP, swing_cam_get_resp);
	if (ret != 0) {
		swing_cam_log_err("reg notifier failed. [%d]\n", ret);
		return -EFAULT;
	}
	ret = register_mcu_event_notifier(TAG_SWING_CAM,
		CMD_CMN_CLOSE_RESP, swing_cam_get_resp);
	if (ret != 0) {
		swing_cam_log_err("reg notifier failed. [%d]\n", ret);
		return -EFAULT;
	}
	ret = register_iom3_recovery_notifier(&miniisp_reboot_notify);
	if (ret < 0) {
		swing_cam_log_err("register_iom3_recovery_notifier fail\n");
		return ret;
	}
	g_swing_priv.ref_cnt = 0;
	g_swing_priv.self = &(pdev->dev);

	swing_cam_log_info("%s exit, success\n", __func__);
	return 0;
}

static void __exit miniisp_dev_exit(void)
{
	unregister_mcu_event_notifier(TAG_SWING_CAM,
		CMD_CMN_CLOSE_RESP, swing_cam_get_resp);
	unregister_mcu_event_notifier(TAG_SWING_CAM,
		CMD_CMN_CONFIG_RESP, swing_cam_get_resp);
}

static int miniisp_recv_rpmsg(void *arg)
{
	struct miniisp_msg_t *resp_msg = (struct miniisp_msg_t *)arg;

	struct swingcam_data read_data = {0};
	u32 length = sizeof(struct swingcam_data);
	unsigned int ret = 0;

	if (arg == NULL) {
		swing_cam_log_err("[%s] arg NULL.\n", __func__);
		return -1;
	}

	/*lint -e737*/
	mutex_lock(&g_swing_priv.swing_mutex);
	if (g_swing_priv.ref_cnt == 0) {
		swing_cam_log_info("%s: ref cnt is 0.\n", __func__);
		mutex_unlock(&g_swing_priv.swing_mutex);
		return -1;
	}

	if (kfifo_len(&g_swing_priv.read_kfifo) < length) {
		swing_cam_log_err("%s: read data failed.\n", __func__);
		mutex_unlock(&g_swing_priv.swing_mutex);
		return -1;
	}
	/*lint +e737*/

	/*lint -e712*/
	ret = kfifo_out(&g_swing_priv.read_kfifo,
				(unsigned char *)&read_data, length);
	/*lint +e712*/
	mutex_unlock(&g_swing_priv.swing_mutex);
	if (ret == 0) {
		swing_cam_log_err("%s: kfifo out failed.\n", __func__);
		return -1;
	}

	if (read_data.recv_len == sizeof(struct miniisp_msg_t)) {
		memcpy_s(resp_msg, sizeof(struct miniisp_msg_t),
			read_data.p_recv, sizeof(struct miniisp_msg_t));
	} else {
		swing_cam_log_err("[%s] copy fail len[0x%x]\n",
			__func__, read_data.recv_len);
	}

	if (read_data.p_recv != NULL) {
		kfree(read_data.p_recv);
		read_data.p_recv = NULL;
		read_data.recv_len = 0;
	}

	return (int)ret;
}

static int swing_cam_acquire_check(struct msg_req_acquire_swing_cam *acq)
{
	if (acq->sensor_spec.sensor_mode >= SENSOR_WORK_MODE_MAX)
		return -EFAULT;

	if (acq->sensor_spec.test_pattern >= SENSOR_TEST_PATTERN_MAX)
		return -EFAULT;

	return 0;
}

static int swing_v4l2_ioctl_cam_acquire(void *arg)
{
	struct miniisp_msg_t *miniReq = (struct miniisp_msg_t *)arg;
	struct msg_req_acquire_swing_cam *acqReq = NULL;

	if (arg == NULL) {
		swing_cam_log_err("[%s] arg NULL.\n", __func__);
		return -EFAULT;
	}

	if (miniReq->api_name != COMMAND_ACQUIRE_SWING_CAM) {
		swing_cam_log_err("[%s] api_name err.\n", __func__);
		return -EFAULT;
	}
	swing_cam_log_info("%s api_name:0x%x id:0x%x size:0x%x\n",
			__func__, miniReq->api_name,
			miniReq->message_id, miniReq->message_size);

	acqReq = &(miniReq->u.req_acquire_swing_cam);

	(acqReq->sensor_name)[sizeof(acqReq->sensor_name)-1] = '\0';
	(acqReq->i2c_bus_type)[sizeof(acqReq->i2c_bus_type)-1] = '\0';
	swing_cam_log_info("i2c_index:%d, i2c_bus_type:%s\n",
			acqReq->i2c_index, acqReq->i2c_bus_type);

	swing_cam_log_info("sensor_mode:%d, test_patt:%d is_master:%d\n",
			acqReq->sensor_spec.sensor_mode,
			acqReq->sensor_spec.test_pattern,
			acqReq->sensor_spec.phy_info.is_master_sensor);

	swing_cam_log_info("phy_id:%d,phy_mode:%d, phy_work_mode:%d\n",
			acqReq->sensor_spec.phy_info.phy_id,
			acqReq->sensor_spec.phy_info.phy_mode,
			acqReq->sensor_spec.phy_info.phy_work_mode);

	if (swing_cam_acquire_check(acqReq)) {
		swing_cam_log_err("[%s] acquire check fail.\n", __func__);
		return -EFAULT;
	}
	if (send_cmd_from_kernel(TAG_SWING_CAM, CMD_CMN_CONFIG_REQ,
			SUB_CMD_SWING_CAM_ACQUIRE,
			(char *)miniReq, sizeof(struct miniisp_msg_t))) {
		swing_cam_log_err("[%s]send cmd error\n", __func__);
		return -EFAULT;
	}

	return 0;
}

static int swing_cam_config_check(struct msg_req_config_swing_cam *cfg)
{
	if (cfg->extension > FULL_SIZE)
		return -EFAULT;

	if (cfg->flow_mode > SWING_CONTINUOUS)
		return -EFAULT;

	if (cfg->process_pattern >= SENSOR_PIXEL_MAX)
		return -EFAULT;

	if (cfg->stream_cfg.format > SWING_PIXEL_FMT_YUV422I)
		return -EFAULT;

	return 0;
}

static int swing_v4l2_ioctl_cam_config(void *arg)
{
	struct miniisp_msg_t *miniReq = (struct miniisp_msg_t *)arg;
	struct msg_req_config_swing_cam *cfgReq = NULL;

	if (arg == 0) {
		swing_cam_log_err("[%s] arg NULL.\n", __func__);
		return -EFAULT;
	}

	if (miniReq->api_name != COMMAND_USECASE_CONFIG_SWING_CAM) {
		swing_cam_log_err("[%s] api_name err.\n", __func__);
		return -EFAULT;
	}
	swing_cam_log_info("api_name:0x%x message_id:0x%x message_size:0x%x\n",
		miniReq->api_name, miniReq->message_id, miniReq->message_size);

	cfgReq = &(miniReq->u.req_usecase_cfg_swing_cam);
	swing_cam_log_info("extension:%d, flow_mode:%d is_secure:%d %d\n",
			cfgReq->extension, cfgReq->flow_mode,
			cfgReq->is_secure[STREAM_MINIISP_OUT_PREVIEW],
			cfgReq->is_secure[STREAM_MINIISP_OUT_META]);

	swing_cam_log_info("%s pattern:%d w:%d, h:%d, stride:%d format:%d\n",
			__func__, cfgReq->process_pattern,
			cfgReq->stream_cfg.width, cfgReq->stream_cfg.height,
			cfgReq->stream_cfg.stride, cfgReq->stream_cfg.format);

	if (swing_cam_config_check(cfgReq)) {
		swing_cam_log_err("[%s] cfg check fail.\n", __func__);
		return -EFAULT;
	}
	if (send_cmd_from_kernel(TAG_SWING_CAM,
			CMD_CMN_CONFIG_REQ, SUB_CMD_SWING_CAM_CONFIG,
			(char *)miniReq, sizeof(struct miniisp_msg_t))) {
		swing_cam_log_err("[%s]send cmd error\n", __func__);
		return -EFAULT;
	}

	return 0;
}

static int swing_v4l2_ioctl_cam_match_id(void *arg)
{
	struct miniisp_msg_t *miniReq = (struct miniisp_msg_t *)arg;
	struct msg_req_query_swing_cam *matchReq = NULL;

	if (arg == 0) {
		swing_cam_log_err("[%s] arg NULL.\n", __func__);
		return -EFAULT;
	}

	if (miniReq->api_name != COMMAND_QUERY_SWING_CAM) {
		swing_cam_log_err("[%s] api_name err.\n", __func__);
		return -EFAULT;
	}
	swing_cam_log_info("%s api_name:0x%x id:0x%x size:0x%x\n", __func__,
		miniReq->api_name, miniReq->message_id, miniReq->message_size);

	matchReq = &(miniReq->u.req_query_swing_cam);

	(matchReq->product_name)[sizeof(matchReq->product_name)-1] = '\0';
	(matchReq->name)[sizeof(matchReq->name)-1] = '\0';
	swing_cam_log_info("pos:%d, i2c:%d, csi:%d\n",
			matchReq->mount_position, matchReq->i2c_index,
			matchReq->csi_index);

	if (send_cmd_from_kernel(TAG_SWING_CAM,
			CMD_CMN_CONFIG_REQ, SUB_CMD_SWING_CAM_MATCH_ID,
			(char *)miniReq, sizeof(struct miniisp_msg_t))) {
		swing_cam_log_err("[%s]send cmd error\n", __func__);
		return -EFAULT;
	}

	return 0;
}

static int swing_v4l2_ioctl_cam_capture(void *arg)
{
	struct miniisp_msg_t *miniReq = (struct miniisp_msg_t *)arg;
	struct miniisp_req_request *capReq = NULL;

	if (arg == 0) {
		swing_cam_log_err("[%s] arg NULL.\n", __func__);
		return -EFAULT;
	}

	if (miniReq->api_name != COMMAND_MINIISP_REQUEST) {
		swing_cam_log_err("[%s] api_name err.\n", __func__);
		return -EFAULT;
	}
	swing_cam_log_info("%s api_name:0x%x id:0x%x size:0x%x\n",
			__func__, miniReq->api_name,
			miniReq->message_id, miniReq->message_size);

	capReq = &(miniReq->u.req_miniisp_request);
	swing_cam_log_info("map_fd:%d, frame:%d\n",
			capReq->map_fd, capReq->frame_num);

	if (send_cmd_from_kernel(TAG_SWING_CAM,
			CMD_CMN_CONFIG_REQ, SUB_CMD_SWING_CAM_CAPTURE,
			(char *)miniReq, sizeof(struct miniisp_msg_t))) {
		swing_cam_log_err("[%s]send cmd error\n", __func__);
		return -EFAULT;
	}

	return 0;
}

static int swing_v4l2_ioctl_cam_extend(void *arg)
{
	struct miniisp_msg_t *miniReq = (struct miniisp_msg_t *)arg;
	struct msg_req_extend_swing_set *extReq = NULL;

	if (arg == 0) {
		swing_cam_log_err("[%s] arg NULL.\n", __func__);
		return -EFAULT;
	}

	if (miniReq->api_name != COMMAND_MINIISP_EXTEND_SET) {
		swing_cam_log_err("[%s] api_name err.\n", __func__);
		return -EFAULT;
	}
	swing_cam_log_info("%s api_name:0x%x id:0x%x size:0x%x\n",
			__func__, miniReq->api_name,
			miniReq->message_id, miniReq->message_size);

	extReq = &(miniReq->u.req_subcmd_swing_cam);
	swing_cam_log_info("%s extend_cmd=%d\n", __func__, extReq->extend_cmd);

	if (send_cmd_from_kernel(TAG_SWING_CAM,
			CMD_CMN_CONFIG_REQ, SUB_CMD_SWING_CAM_EXTEND,
			(char *)miniReq, sizeof(struct miniisp_msg_t))) {
		swing_cam_log_err("[%s]send cmd error\n", __func__);
		return -EFAULT;
	}

	return 0;
}

static int swing_v4l2_ioctl_get_otp(void *arg)
{
	struct miniisp_msg_t *miniReq = (struct miniisp_msg_t *)arg;
	struct msg_req_get_ao_otp_t *otpReq = NULL;

	if (arg == 0) {
		swing_cam_log_err("[%s] arg NULL.\n", __func__);
		return -EFAULT;
	}

	if (miniReq->api_name != COMMAND_GET_SWING_CAM_OTP) {
		swing_cam_log_err("[%s] api_name err.\n", __func__);
		return -EFAULT;
	}
	swing_cam_log_info("%s api_name:0x%x id:0x%x size:0x%x\n",
			__func__, miniReq->api_name,
			miniReq->message_id, miniReq->message_size);

	otpReq = &(miniReq->u.req_get_ao_otp);
	(otpReq->sensor_name)[sizeof(otpReq->sensor_name)-1] = '\0';

	if (send_cmd_from_kernel(TAG_SWING_CAM,
			CMD_CMN_CONFIG_REQ, SUB_CMD_SWING_GET_OTP,
			(char *)miniReq, sizeof(struct miniisp_msg_t))) {
		swing_cam_log_err("[%s]send cmd error\n", __func__);
		return -EFAULT;
	}

	return 0;
}

static long miniisp_vo_subdev_ioctl(struct v4l2_subdev *sd,
						unsigned int cmd, void *arg)
{
	long ret = 0;

	swing_cam_log_info("%s cmd is (0x%x), arg(%pK)!\n", __func__, cmd, arg);

	if ((arg != NULL) &&
		((sizeof(struct miniisp_msg_t) > MAX_SWING_PAYLOAD))) {
		return -EFAULT;
	}

	switch (cmd) {
	case SWING_IOCTL_CAM_OPEN:
		send_cmd_from_kernel(TAG_SWING_CAM,
				CMD_CMN_OPEN_REQ, 0, NULL, (size_t)0);
		swing_cam_init_timestamp();
		break;

	case SWING_IOCTL_CAM_CLOSE:
		swing_cam_destroy_timestamp();
		send_cmd_from_kernel(TAG_SWING_CAM,
				CMD_CMN_CLOSE_REQ, 0, NULL, (size_t)0);
		break;

	case SWING_IOCTL_CAM_CONFIG:
		ret = swing_v4l2_ioctl_cam_config(arg);
		break;

	case SWING_IOCTL_CAM_CAPTURE:
		ret = swing_v4l2_ioctl_cam_capture(arg);
		break;

	case SWING_IOCTL_CAM_MATCH_ID:
		ret = swing_v4l2_ioctl_cam_match_id(arg);
		break;

	case SWING_IOCTL_CAM_RECV_RPMSG:
		ret = miniisp_recv_rpmsg(arg);
		break;

	case SWING_IOCTL_CAM_ACQUIRE:
		ret = swing_v4l2_ioctl_cam_acquire(arg);
		break;

	case SWING_IOCTL_CAM_SET_EXTEND:
		ret = swing_v4l2_ioctl_cam_extend(arg);
		break;

	case SWING_IOCTL_CAM_GET_OTP:
		ret = swing_v4l2_ioctl_get_otp(arg);
		break;

	default:
		swing_cam_log_err("%s unknown cmd : 0x%x\n", __func__, cmd);
		return -ENOTTY;
	}

	return ret;
}

static int miniisp_subdev_subscribe_event(struct v4l2_subdev *sd,
		struct v4l2_fh *fh, struct v4l2_event_subscription *sub)
{
	swing_cam_log_info("enter %s\n", __func__);
	return v4l2_event_subscribe(fh, sub, 128, NULL);
}

static int miniisp_subdev_unsubscribe_event(struct v4l2_subdev *sd,
		struct v4l2_fh *fh, struct v4l2_event_subscription *sub)
{
	swing_cam_log_info("enter %s\n", __func__);
	return v4l2_event_unsubscribe(fh, sub);
}

static struct v4l2_subdev_core_ops s_subdev_core_ops_miniisp = {
	.ioctl = miniisp_vo_subdev_ioctl,
	.subscribe_event = miniisp_subdev_subscribe_event,
	.unsubscribe_event = miniisp_subdev_unsubscribe_event,
};

static struct v4l2_subdev_internal_ops s_subdev_internal_ops_miniisp = {
	.open = swing_cam_dev_open,
	.close = swing_cam_dev_release,
};

static struct v4l2_subdev_ops s_subdev_ops_miniisp = {
	.core = &s_subdev_core_ops_miniisp,
};

static int miniisp_probe(struct platform_device *pdev)
{
	int rc;
	struct v4l2_subdev *subdev;

	subdev = &g_miniIsp.subdev;

	if (pdev == NULL) {
		swing_cam_log_err("%s: pdev is NULL\n", __func__);
		return -EFAULT;
	}

	swing_cam_log_info("%s...\n", __func__);

	g_miniIsp.pdev = pdev;
	mutex_init(&g_miniIsp.lock);
	v4l2_subdev_init(subdev, &s_subdev_ops_miniisp);
	subdev->internal_ops = &s_subdev_internal_ops_miniisp;
	snprintf_s(subdev->name, sizeof(subdev->name),
		sizeof(subdev->name), "hwcam-cfg-miniisp");
	subdev->flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
	subdev->flags |= V4L2_SUBDEV_FL_HAS_EVENTS;

	(void)init_subdev_media_entity(subdev, HWCAM_SUBDEV_SWING);
	(void)hwcam_cfgdev_register_subdev(subdev, HWCAM_SUBDEV_SWING);
	subdev->devnode->lock = &g_miniIsp.lock;
	platform_set_drvdata(pdev, subdev);

	rc = swing_cam_dev_init(pdev);
	swing_cam_log_info("%s exit...\n", __func__);

	return rc;
}

static int __exit miniisp_remove(struct platform_device *pdev)
{
	struct v4l2_subdev *subdev = NULL;

	swing_cam_log_info("%s...\n", __func__);

	subdev = platform_get_drvdata(g_miniIsp.pdev);
	media_entity_cleanup(&subdev->entity);
	hwcam_cfgdev_unregister_subdev(subdev);
	mutex_destroy(&g_miniIsp.lock);

	miniisp_dev_exit();

	return 0;
}

static const struct of_device_id swing_cam_match_table[] = {
	{ .compatible = SWING_CAM_DRV_NAME, },
	{},
};

static struct platform_driver swing_cam_platdev = {
	.driver = {
		.name = "huawei,swing_cam_dev",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(swing_cam_match_table),
	},
	.probe  = miniisp_probe,
	.remove = miniisp_remove,
};

static int __init swing_cam_main_init(void)
{
	swing_cam_log_info("%s enter\n", __func__);
	return platform_driver_register(&swing_cam_platdev);
}

static void __exit swing_cam_main_exit(void)
{
	swing_cam_log_info("%s enter\n", __func__);

	platform_driver_unregister(&swing_cam_platdev);
}

late_initcall_sync(swing_cam_main_init);
module_exit(swing_cam_main_exit);
MODULE_DESCRIPTION("swingcamdev");
MODULE_LICENSE("GPL v2");
