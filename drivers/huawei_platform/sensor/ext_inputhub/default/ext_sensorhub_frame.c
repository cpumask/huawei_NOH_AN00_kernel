/*
 * ext_sensorhub_frame.c
 *
 * code for make external sensorhub frame
 *
 * Copyright (c) 2020-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "ext_sensorhub_frame.h"

#include <asm/memory.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/log/hwlog_kernel.h>

#include "securec.h"
#include "crc16.h"
#include "ext_sensorhub_route.h"
#include "ext_sensorhub_commu.h"

#define HWLOG_TAG ext_sensorhub_frame
HWLOG_REGIST();
#define SENSOR_SERVICE_ID  0x15
#define SENSOR_DATA_COMMAND_ID 0x81
#define SENSOR_TYPE_INDEX 2

struct dubai_data {
	bool record_dubai;
	unsigned char service_id;
	unsigned char command_id;
	unsigned char sensor_type;
};

#define FRAME_RETRY_TIME 2

static const int time_factor = 1000;
static const int one_byte_bit_count = 8;
static struct work_struct handshake_work;
static bool g_handshake_flag;
static struct work_struct dubai_work;
static struct dubai_data g_dubai_data;

static struct frame_decode_data g_frame_recv;
/* recode fsn when control not 0 */
static unsigned char g_expected_fsn;

#define RTC_SID     0x01
#define RTC_CID     0x06

static void convert_16_data_b2l(unsigned char *ptr_data_in,
				unsigned short *ptr_data_out)
{
	*ptr_data_out = ((unsigned short)(*ptr_data_in++) <<
	one_byte_bit_count);
	*ptr_data_out |= (unsigned short)(*ptr_data_in);
}

static void calc_frame_length(struct frame_request *request,
			      unsigned char *control,
			      unsigned short frame_index, int payload_bufsz)
{
	if (request->frame_count == 1) {
		/* none FSN */
		request->frames[frame_index].frame_length =
			FRAME_CTRL_HEADER_LEN  +
			FRAME_OPCODE_HEADER_LEN + payload_bufsz;
		*control = FRAME_CTL_NONE; /* none FSN */
	} else if ((frame_index + 1) == request->frame_count) {
		/* with FSN, left buffer size + frame header - tranid & crc */
		request->frames[frame_index].frame_length = payload_bufsz -
			SPLIT_FRAME_MAX_PAYLOAD_LEN * frame_index +
			FRAME_OPCODE_HEADER_LEN + FRAME_CTRL_HEADER_LEN +
			FRAME_FSN_HEADER_LEN;
		*control = FRAME_CTL_END; /* end FSN */
	} else {
		request->frames[frame_index].frame_length =
		FRAME_LEN_BODY_MAX_LEN;
		if (frame_index == 0)
			*control = FRAME_CTL_FIRST; /* first FSN */
		else
			*control = FRAME_CTL_DOING; /* doing FSN */
	}
	request->frames[frame_index].frame_total_length =
		request->frames[frame_index].frame_length +
		FRAME_TRAN_HEADER_LEN + FRAME_CRC16_HEADER_LEN;
}

static int encode_frame_crc(struct frame_request *request,
			    unsigned short offset, int i)
{
	unsigned short crc;
	unsigned short frame_crc = 0;
	int ret;

	crc = crc16_calc(request->frames[i].frame_body + FRAME_TRAN_ID_LEN,
			 request->frames[i].frame_total_length -
			 FRAME_TRAN_ID_LEN - FRAME_CRC16_HEADER_LEN);
	convert_16_data_b2l((unsigned char *)&crc, &frame_crc);
	ret = memcpy_s(request->frames[i].frame_body + offset,
		       FRAME_BUF_LEN - offset,
		       &frame_crc, sizeof(frame_crc));

	return ret;
}

static int encode_frame_header(unsigned char tid,
			       struct frame_request *request,
			       unsigned short *offset,
			       int payload_bufsz, int i)
{
	unsigned char control = 0;
	unsigned short frame_len = 0;
	int ret;

	/* 1. Fill tran id */
	request->frames[i].frame_body[*offset] = tid;
	*offset = *offset + 1;

	/* 1. Fill SOF */
	request->frames[i].frame_body[*offset] = START_OF_FRAME;
	*offset = *offset + 1;

	/* 2. Fill frame length:contain control, fsn(or not), opcode, payload */
	calc_frame_length(request, &control, i, payload_bufsz);

	convert_16_data_b2l((unsigned char *)&request->frames[i].frame_length,
			    &frame_len);
	ret = memcpy_s(request->frames[i].frame_body + *offset,
		       FRAME_BUF_LEN - *offset,
		       &frame_len, sizeof(unsigned short));
	if (ret < 0)
		return ret;

	*offset = *offset + sizeof(unsigned short);

	/* 3. Fill control and FSN */
	request->frames[i].frame_body[*offset] = control;
	*offset = *offset + 1;
	/* If have FSN, fill it; range[0 - 255], but current max is 4 */
	if (control != 0x00) {
		/* fsn */
		request->frames[i].frame_body[*offset] = i;
		*offset = *offset + 1;
	}
	/* 4. Fill OPCode, only first frame */
	if (i == 0) {
		request->frames[i].frame_body[*offset] = request->service_id;
		*offset = *offset + 1;
		request->frames[i].frame_body[*offset] = request->command_id;
		*offset = *offset + 1;
	}

	return 0;
}

static int encode_req_frame_body(unsigned char tid,
				 struct frame_request *request,
				 unsigned char *payload_buf, int payload_bufsz)
{
	unsigned short i = 0;
	unsigned short offset = 0;
	unsigned short payload_offset = 0;
	int ret = 0;

	for (i = 0; i < request->frame_count; i++) {
		/* memset to 0x00 */
		memset_s(request->frames[i].frame_body, FRAME_BUF_LEN,
			 0, FRAME_BUF_LEN);

		offset = 0;
		ret = encode_frame_header(
			tid, request, &offset, payload_bufsz, i);
		if (ret < 0)
			return ret;
		/* Fill payload */
		if (request->frame_count == 1) {
			ret = memcpy_s(request->frames[i].frame_body + offset,
				       FRAME_BUF_LEN - offset,
				       payload_buf, payload_bufsz);
			offset += payload_bufsz;
		} else if ((i + 1) == request->frame_count) {
			ret = memcpy_s(request->frames[i].frame_body + offset,
				       FRAME_BUF_LEN - offset,
				       payload_buf + payload_offset,
				       /* first - FRAME_OPCODE_HEADER_LEN */
				       payload_bufsz -
				       SPLIT_FRAME_MAX_PAYLOAD_LEN * i +
				       FRAME_OPCODE_HEADER_LEN);

			offset += (payload_bufsz -
				SPLIT_FRAME_MAX_PAYLOAD_LEN * i +
				FRAME_OPCODE_HEADER_LEN);
		} else if (i == 0) {
			/* first frame need contain opcode */
			ret = memcpy_s(request->frames[i].frame_body + offset,
				       FRAME_BUF_LEN - offset,
				       payload_buf + payload_offset,
				       SPLIT_FRAME_MAX_PAYLOAD_LEN -
				       FRAME_OPCODE_HEADER_LEN);
			payload_offset += SPLIT_FRAME_MAX_PAYLOAD_LEN -
			FRAME_OPCODE_HEADER_LEN;
			offset += SPLIT_FRAME_MAX_PAYLOAD_LEN -
			FRAME_OPCODE_HEADER_LEN;
		} else {
			/* frame except first, will not contain opcode */
			ret = memcpy_s(request->frames[i].frame_body + offset,
				       FRAME_BUF_LEN - offset,
				       payload_buf + payload_offset,
				       SPLIT_FRAME_MAX_PAYLOAD_LEN);
			payload_offset += SPLIT_FRAME_MAX_PAYLOAD_LEN;
			offset += SPLIT_FRAME_MAX_PAYLOAD_LEN;
		}
		if (ret < 0)
			return ret;

		/* Fill crc, from sof to payload, ie: exclude tran id and crc */
		ret = encode_frame_crc(request, offset, i);
		if (ret < 0)
			return ret;
	}

	return 0;
}

static int send_frame_data_retry(const unsigned char *buf, size_t len)
{
	int ret;
	int retry_count = 0;

	ret = commu_write(buf, len);
	while (ret < 0) {
		if (retry_count >= FRAME_RETRY_TIME)
			break;
		ret = commu_write(buf, len);
		retry_count++;
	}

	return ret;
}

int send_frame_data(unsigned char tid,
		    unsigned char service_id, unsigned char command_id,
		    unsigned char *payload_buf, int payload_bufsz)
{
	struct frame_request request;
	int ret;
	int i;
	unsigned char frame_count;

	request.frames = NULL;
	if (!g_handshake_flag && (service_id != SERVICE_ID_HANDSHAKE ||
				  command_id != SERVICE_MCU_HANDSHAKE_REQ)) {
		hwlog_err("%s handshake has not done", __func__);
		return -EREMOTEIO;
	}

	hwlog_debug("%s start in service_id = %d, command_id = %d\n", __func__,
		    service_id, command_id);
	request.service_id = service_id;
	request.command_id = command_id;
	/* div into frame, may multiple frames, if one frame, without fsn */
	if (payload_bufsz <= FRAME_BUF_LEN - FRAME_HEADER_WITHOUT_FSN_LEN) {
		frame_count = 1;
	} else {
		/* calculate frame count, only first frame need opcode */
		frame_count = (payload_bufsz + FRAME_OPCODE_HEADER_LEN - 1) /
		SPLIT_FRAME_MAX_PAYLOAD_LEN + 1;
		/* check max count */
		if (frame_count > MAX_FRAME_COUNT)
			return -EINVAL;
	}

	request.frame_count = frame_count;
	request.frames = kmalloc_array(request.frame_count,
				       sizeof(struct frame_content),
				       GFP_KERNEL);
	if (!request.frames)
		return -ENOMEM;

	ret = encode_req_frame_body(tid, &request, payload_buf, payload_bufsz);
	if (ret < 0) {
		hwlog_err("%s, encode request frame error, ret:%d",
			  __func__, ret);
		return ret;
	}

	hwlog_debug("%s frame count: %d, service_id = %d, command_id = %d\n",
		    __func__, request.frame_count, service_id, command_id);
	/* send request to external sensorhub */
	for (i = 0; i < request.frame_count; ++i) {
		ret = send_frame_data_retry(
			request.frames[i].frame_body,
			request.frames[i].frame_total_length);
		if (ret < 0) {
			hwlog_err("%s commu write error, ret:%d",
				  __func__, ret);
			break;
		}
	}

	kfree(request.frames);
	request.frames = NULL;
	return ret;
}

int send_debug_frame(unsigned char *payload_buf, int payload_bufsz)
{
	int ret;
	unsigned short crc;
	unsigned short frame_crc = 0;
	unsigned char *frame_buffer = NULL;

	if (payload_bufsz <= 0 ||
	    payload_bufsz + FRAME_CRC16_HEADER_LEN > FRAME_BUF_LEN)
		return -EINVAL;

	frame_buffer = kmalloc(FRAME_BUF_LEN, GFP_KERNEL);
	memset_s(frame_buffer, FRAME_BUF_LEN, 0, FRAME_BUF_LEN);
	if (!frame_buffer)
		return -ENOMEM;

	/* Fill crc, from sof to payload, ie: exclude tran id and crc */
	crc = crc16_calc(payload_buf + FRAME_TRAN_ID_LEN,
			 payload_bufsz - FRAME_TRAN_ID_LEN);
	convert_16_data_b2l((unsigned char *)&crc, &frame_crc);

	ret = memcpy_s(frame_buffer, FRAME_BUF_LEN, payload_buf, payload_bufsz);
	if (ret < 0)
		goto err;
	ret = memcpy_s(frame_buffer + payload_bufsz,
		       FRAME_BUF_LEN - payload_bufsz,
		       &frame_crc, sizeof(frame_crc));
	if (ret < 0)
		goto err;

	ret = commu_write(frame_buffer, payload_bufsz + FRAME_CRC16_HEADER_LEN);
	if (ret < 0)
		hwlog_err("%s commu write error, ret:%d", __func__, ret);

err:
	kfree(frame_buffer);
	frame_buffer = NULL;
	return ret;
}

static int handle_resp_tran_header(unsigned char *read_buf,
				   struct frame_decode_data *decode_data)
{
	int ret = -1;
	/* contian:control [fsn] + opcode + payload */
	unsigned short frame_length = 0;
	struct frame_tran_header *tran_header = NULL;

	do {
		if (!read_buf || !decode_data) {
			hwlog_err("%s parameter is invalid", __func__);
			break;
		}

		/* check transaction_id, sof */
		tran_header = (struct frame_tran_header *)(read_buf);
		if (!tran_header) {
			hwlog_err("%s, ptran header is NULL", __func__);
			break;
		}

		/* check dst:get bit4 - bit7, if 0x1x, is AP; and check SOF */
		if (((tran_header->tran_id & 0xF0) !=
		     TRANSACTION_ID_AP_IS_DST) ||
		    tran_header->sof != START_OF_FRAME) {
			hwlog_err("%s, dst isn't AP or SOF is error. %02x",
				  __func__, tran_header->tran_id);
			break;
		}

		/* convert frame length */
		convert_16_data_b2l(&tran_header->frame_length_msb,
				    &frame_length);

		/* frame total length > FRAME_BUF_LEN, is invalid */
		if ((frame_length + FRAME_TRAN_HEADER_LEN +
		     FRAME_CRC16_HEADER_LEN) > FRAME_BUF_LEN) {
			hwlog_err("%s,frame_length is error, %d",
				  __func__, frame_length);
			break;
		}
		decode_data->frame_length = frame_length;

		ret = 0;
	} while (0);

	return ret;
}

static int handle_resp_crc_header(unsigned char *read_buf,
				  unsigned short frame_length)
{
	int ret = -1;
	unsigned short crc_offset = 0;
	unsigned short crc_value = 0;
	unsigned short crc_check = 0;
	struct frame_crc16_header *crc_header_ptr = NULL;

	do {
		if (!read_buf)
			break;

		crc_offset = frame_length + FRAME_TRAN_HEADER_LEN;

		/* get CRC value */
		crc_header_ptr = (struct frame_crc16_header *)(read_buf +
			crc_offset);

		convert_16_data_b2l(&crc_header_ptr->crc16_msb, &crc_value);

		/* check CRC, read_buf contain tansaction_id, shift 1 byte */
		crc_check = crc16_calc(read_buf + FRAME_TRAN_ID_LEN,
				       (frame_length + FRAME_TRAN_HEADER_LEN -
					FRAME_TRAN_ID_LEN));
		if (crc_check != crc_value)
			break;

		/* CRC pass */
		ret = 0;
	} while (0);

	return ret;
}

static int handle_resp_opcode_header(unsigned char *read_buf,
				     unsigned short offset,
				     unsigned char *command_id,
				     unsigned char *service_id)
{
	int ret = -1;
	struct frame_opcode_header *opcode_ptr = NULL;

	do {
		if (!read_buf || !command_id || !service_id)
			break;

		opcode_ptr = (struct frame_opcode_header *)(read_buf + offset);
		*service_id = opcode_ptr->service_id;
		*command_id = opcode_ptr->command_id;

		ret = 0;
	} while (0);

	return ret;
}

static int handle_resp_control_none(unsigned char *read_buf,
				    struct frame_decode_data *raw_data)
{
	int ret = -1;
	unsigned char command_id = 0;
	unsigned char service_id = 0;
	unsigned short opcode_offset = 0;
	unsigned short payload_offset = 0;
	unsigned char *payload_ptr = NULL;

	if (!read_buf || !raw_data) {
		hwlog_err("%s, parameter is invalid", __func__);
		return ret;
	}

	/* handle opcode */
	opcode_offset = FRAME_TRAN_HEADER_LEN + FRAME_CTRL_HEADER_LEN;
	if (handle_resp_opcode_header(read_buf, opcode_offset,
				      &command_id, &service_id) != 0) {
		hwlog_err("%s, opcode_offset is error", __func__);
		return ret;
	}

	raw_data->service_id = service_id;
	raw_data->command_id = command_id;
	raw_data->payload_length =
		(raw_data->frame_length) - FRAME_CTRL_HEADER_LEN -
		FRAME_OPCODE_HEADER_LEN;

	/* copy payload, according to payload_length */
	payload_offset =
		FRAME_TRAN_HEADER_LEN + FRAME_CTRL_HEADER_LEN +
		FRAME_OPCODE_HEADER_LEN;
	payload_ptr = (unsigned char *)(read_buf + payload_offset);
	if (!payload_ptr) {
		hwlog_err("%s, payload_ptr is error", __func__);
		return ret;
	}
	ret = memcpy_s(raw_data->payload, MAX_PAYLOAD_DATA_SIZE,
		       payload_ptr,
		       ext_sensorhub_get_min(raw_data->payload_length,
					     MAX_PAYLOAD_DATA_SIZE));

	return ret;
}

static int handle_resp_fsn_header(unsigned char *read_buf)
{
	int ret = -1;
	int fsn_offset = 0;
	unsigned char fsn_value = 0;
	struct frame_fsn_header *fsn_header_ptr = NULL;

	if (!read_buf)
		return ret;

	fsn_offset = FRAME_TRAN_HEADER_LEN + FRAME_CTRL_HEADER_LEN;

	fsn_header_ptr = (struct frame_fsn_header *)(read_buf +
		fsn_offset);

	ret = memcpy_s(&fsn_value, sizeof(fsn_value),
		       fsn_header_ptr, sizeof(fsn_value));

	/* current frame max connt is 5, so FSN (0 - 4) */
	if (ret < 0 || g_expected_fsn != fsn_value ||
	    fsn_value >= MAX_FRAME_COUNT)
		hwlog_err("%s fsn value invalid, fsn=%d",
			  __func__, fsn_value);

	return ret;
}

static int handle_resp_control_first(unsigned char *read_buf,
				     struct frame_decode_data *raw_data)
{
	int ret = -1;
	unsigned char command_id = 0;
	unsigned short offset = 0;
	unsigned char *payload_ptr = NULL;

	if (!read_buf || !raw_data)
		return ret;

	g_expected_fsn = 0;

	/* check FSN */
	if (handle_resp_fsn_header(read_buf) != 0)
		return ret;

	/* hand opcode */
	offset = FRAME_TRAN_HEADER_LEN + FRAME_CTRL_HEADER_LEN +
	FRAME_FSN_HEADER_LEN;
	if (handle_resp_opcode_header(read_buf, offset,
				      &command_id,
				      &raw_data->service_id) != 0)
		return ret;

	raw_data->command_id = command_id;

	/* copy payload to raw_data->payload */
	offset = FRAME_HEADER_LEN - FRAME_CRC16_HEADER_LEN;
	raw_data->payload_length = (raw_data->frame_length) -
	FRAME_CTRL_HEADER_LEN - FRAME_FSN_HEADER_LEN -
	FRAME_OPCODE_HEADER_LEN;
	payload_ptr = (unsigned char *)(read_buf + offset);

	ret = memcpy_s(raw_data->payload, MAX_PAYLOAD_DATA_SIZE,
		       payload_ptr,
		       ext_sensorhub_get_min(
			       raw_data->payload_length,
			       MAX_PAYLOAD_DATA_SIZE));

	if (ret < 0)
		return ret;

	g_expected_fsn++;
	return 0;
}

static int handle_resp_control_doing(unsigned char *read_buf,
				     struct frame_decode_data *raw_data)
{
	int ret = -1;
	unsigned short offset = 0;
	unsigned short payload_len = 0;
	unsigned char *payload_ptr = NULL;

	if (!read_buf || !raw_data)
		return ret;

	/* check FSN */
	if (handle_resp_fsn_header(read_buf) != 0)
		return ret;

	/* hand opcode */
	offset = FRAME_TRAN_HEADER_LEN + FRAME_CTRL_HEADER_LEN +
	FRAME_FSN_HEADER_LEN;

	/* copy payload to raw_data->payload, doing or end frame,
	 * payload only raw data, no opcode
	 */
	payload_len = raw_data->frame_length -
	FRAME_CTRL_HEADER_LEN - FRAME_FSN_HEADER_LEN;
	payload_ptr = (unsigned char *)(read_buf +
		(FRAME_TRAN_HEADER_LEN + FRAME_CTRL_HEADER_LEN +
			FRAME_FSN_HEADER_LEN));

	offset = raw_data->payload_length;
	ret = memcpy_s(raw_data->payload + offset,
		       MAX_PAYLOAD_DATA_SIZE - offset, payload_ptr,
		       ext_sensorhub_get_min(
			       payload_len, MAX_PAYLOAD_DATA_SIZE - offset));

	if (ret < 0)
		return ret;
	/* change raw_data->payload_length */
	raw_data->payload_length += payload_len;
	g_expected_fsn++;
	return 0;
}

static int handle_resp_control_end(unsigned char *read_buf,
				   struct frame_decode_data *raw_data)
{
	int ret;

	ret = handle_resp_control_doing(read_buf, raw_data);
	if (ret == 0) {
		if (raw_data->payload_length > MAX_PAYLOAD_DATA_SIZE)
			ret = -1;
	}

	return ret;
}

static int handle_resp_frame_data(unsigned char *read_buf,
				  struct frame_decode_data *raw_data)
{
	int ret = -1;
	unsigned char control = 0;
	struct frame_ctrl_header *ctrl_header_ptr = NULL;

	do {
		if (!read_buf || !raw_data) {
			hwlog_err("%s, parameter is invalid", __func__);
			break;
		}

		/* Get control */
		ctrl_header_ptr =
		   (struct frame_ctrl_header *)(read_buf +
			FRAME_TRAN_HEADER_LEN);
		if (!ctrl_header_ptr) {
			hwlog_err("%s, ctrl_header_ptr is null", __func__);
			break;
		}

		control = (ctrl_header_ptr->control) & FRAME_FSN_CTL;
		raw_data->control = control;

		/* handle control */
		switch (raw_data->control) {
		case FRAME_CTL_NONE:
			ret = handle_resp_control_none(read_buf, raw_data);
			break;

		case FRAME_CTL_FIRST:
			ret = handle_resp_control_first(read_buf, raw_data);
			break;

		case FRAME_CTL_DOING:
			ret = handle_resp_control_doing(read_buf, raw_data);
			break;

		case FRAME_CTL_END:
			ret = handle_resp_control_end(read_buf, raw_data);
			break;

		default:
			ret = -1;
			break;
		}

	} while (0);

	return ret;
}

static void record_dubai_data(struct frame_decode_data *raw_data)
{
	g_dubai_data.record_dubai = false;
	g_dubai_data.service_id = raw_data->service_id;
	g_dubai_data.command_id = raw_data->command_id;

	// check if sensor data and record sensor type
	if (raw_data->service_id == SENSOR_SERVICE_ID &&
	    raw_data->command_id == SENSOR_DATA_COMMAND_ID &&
	    raw_data->payload_length > SENSOR_TYPE_INDEX) {
		g_dubai_data.sensor_type = raw_data->payload[SENSOR_TYPE_INDEX];
		hwlog_info("%s get sensor:0x%02x data to record dubai",
			   __func__, g_dubai_data.sensor_type);
	} else {
		g_dubai_data.sensor_type = -1;
	}

	schedule_work(&dubai_work);
}

void notify_commu_wakeup(bool is_wakeup)
{
	g_dubai_data.record_dubai = is_wakeup;
}

static int malloc_resp(struct frame_decode_data *raw_data)
{
	struct ext_sensorhub_buf_list *resp_list = NULL;

	if (!raw_data)
		return -EINVAL;

	/* record dubai */
	if (g_dubai_data.record_dubai) {
		hwlog_debug("%s will record dubai", __func__);
		record_dubai_data(raw_data);
	}

	/* check if is a handshake command */
	if (raw_data->service_id == SERVICE_ID_HANDSHAKE &&
	    raw_data->command_id == SERVICE_MCU_HANDSHAKE_REQ) {
		hwlog_debug("%s get handshake from mcu", __func__);
		schedule_work(&handshake_work);
		return 0;
	}

	/* resp_list free in ext_sensorhub_route_append after used */
	resp_list = kmalloc(sizeof(*resp_list), GFP_KERNEL);
	if (!resp_list)
		return -ENOMEM;

	resp_list->service_id = raw_data->service_id;
	resp_list->command_id = raw_data->command_id;
	resp_list->read_cnt = raw_data->payload_length;
	resp_list->buffer = kmalloc(resp_list->read_cnt, GFP_KERNEL);

	if (memcpy_s(resp_list->buffer, resp_list->read_cnt,
		     raw_data->payload, resp_list->read_cnt) < 0)
		return -EFAULT;

	hwlog_debug("resp_list sid: %d, cid: %d",
		    resp_list->service_id, resp_list->command_id);

	ext_sensorhub_route_append(resp_list);

	return 0;
}

int decode_frame_resp(unsigned char *read_buf, unsigned short read_bufsz)
{
	int ret;

	if (read_bufsz <= 0)
		return -EINVAL;

	ret = handle_resp_tran_header(read_buf, &g_frame_recv);
	if (ret < 0) {
		memset_s(&g_frame_recv, sizeof(struct frame_decode_data), 0,
			 sizeof(struct frame_decode_data));
		hwlog_err("check tran id error %s", __func__);
		return ret;
	}

	ret = handle_resp_crc_header(read_buf, g_frame_recv.frame_length);
	if (ret < 0) {
		memset_s(&g_frame_recv, sizeof(struct frame_decode_data), 0,
			 sizeof(struct frame_decode_data));
		hwlog_err("check crc error %s", __func__);
		return ret;
	}

	ret = handle_resp_frame_data(read_buf, &g_frame_recv);
	if (ret < 0) {
		memset_s(&g_frame_recv, sizeof(struct frame_decode_data), 0,
			 sizeof(struct frame_decode_data));
		hwlog_err("check frame data error %s", __func__);
		return ret;
	}

	/* a complete frame reveived,copy and append to data list */
	if (g_frame_recv.control == FRAME_CTL_NONE ||
	    g_frame_recv.control == FRAME_CTL_END) {
		malloc_resp(&g_frame_recv);
		hwlog_debug("get one frame %s", __func__);
		/* reset global revice */
		memset_s(&g_frame_recv, sizeof(struct frame_decode_data), 0,
			 sizeof(struct frame_decode_data));
	}

	return 0;
}

static void convert_64_l2b(char *ptr_data_in, char *ptr_data_out)
{
	int i;

	for (i = 0; i < one_byte_bit_count; ++i)
		*ptr_data_out++ = *(ptr_data_in + (one_byte_bit_count - 1 - i));
}

static int get_rtc_time(void)
{
	unsigned char buff[4] = {0x01, 0x00, 0x02, 0x00};

	return send_frame_data(TRANSACTION_ID_AP_TO_MCU,
			      RTC_SID, RTC_CID,
			      &buff, sizeof(buff));
}

static void handshake_resp_work(struct work_struct *work)
{
	unsigned char service_id = SERVICE_ID_HANDSHAKE;
	unsigned char command_id = SERVICE_MCU_HANDSHAKE_REQ;
	int ret;
	long long int msec;
	long long int big_value = 0;
	struct timeval tv;

	hwlog_info("%s begin to send handshake resp", __func__);
	do_gettimeofday(&tv);
	msec = (long long int)tv.tv_sec * time_factor +
	tv.tv_usec / time_factor;

	convert_64_l2b((char *)(&msec), (char *)(&big_value));

	/* send handshake resp */
	ret = send_frame_data(TRANSACTION_ID_AP_TO_MCU,
			      service_id, command_id,
			      (unsigned char *)&big_value, sizeof(big_value));
	if (ret < 0) {
		hwlog_err("%s send handshake error, ret: %d", __func__, ret);
	} else {
		hwlog_info("%s has handshake with mcu success", __func__);
		g_handshake_flag = true;
	}
	/* get init RTC from MCU */
	if (get_rtc_time() < 0) {
		hwlog_err("%s get init rtc from mcu error, ret: %d",
			  __func__, ret);
	} else {
		hwlog_info("%s get init rtc from mcu success", __func__);
	}
}

static void record_dubai_work(struct work_struct *work)
{
	hwlog_info("%s begin to record dubai log, sid:0x%02x, cid:0x%02x, sensor:0x%02x",
		   __func__, g_dubai_data.service_id, g_dubai_data.command_id,
		   g_dubai_data.sensor_type);
	HWDUBAI_LOGE("DUBAI_TAG_WATCH_MCU_WAKEUP",
		     "serviceId=%d commandId=%d type=%d",
		     g_dubai_data.service_id, g_dubai_data.command_id,
		     g_dubai_data.sensor_type);
	hwlog_info("%s record dubai log end", __func__);
}

int ext_sensorhub_frame_init(void)
{
	INIT_WORK(&handshake_work, handshake_resp_work);

	INIT_WORK(&dubai_work, record_dubai_work);
	/* commu probe init */
	return commu_probe_init();
}

void ext_sensorhub_frame_exit(void)
{
	/* stop communication */
	commu_exit();

	cancel_work_sync(&handshake_work);

	cancel_work_sync(&dubai_work);
}

void notify_mcu_reboot(void)
{
	g_handshake_flag = false;
}
