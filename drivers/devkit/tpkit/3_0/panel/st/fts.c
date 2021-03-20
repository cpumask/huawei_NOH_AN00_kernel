/*
 * fts.c
 *
 * FTS Capacitive touch screen controller (FingerTipS)
 *
 * Copyright (C) 2016, STMicroelectronics Limited.
 * Authors: AMG(Analog Mems Group)
 *
 * 		marco.cali@st.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/device.h>

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/input/mt.h>
#include <linux/interrupt.h>
#include <linux/hrtimer.h>
#include <linux/delay.h>
#include <linux/firmware.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/completion.h>
#include <linux/pm_wakeup.h>

#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/regulator/consumer.h>

#include <linux/notifier.h>
#include <linux/fb.h>

#include "fts.h"
#include "fts_lib/ftsCompensation.h"
#include "fts_lib/ftsCore.h"
#include "fts_lib/ftsIO.h"
#include "fts_lib/ftsError.h"
#include "fts_lib/ftsFlash.h"
#include "fts_lib/ftsFrame.h"
#include "fts_lib/ftsGesture.h"
#include "fts_lib/ftsTest.h"
#include "fts_lib/ftsTime.h"
#include "fts_lib/ftsTool.h"
#include <linux/vmalloc.h>

#if defined(CONFIG_TEE_TUI)
#include "tui.h"
#endif

struct fts_ts_info *fts_info;

static struct completion st_roi_data_done;
static u8 st_roi_data_staled;
static u8 st_roi_switch_on;
static unsigned char st_roi_data[ROI_DATA_READ_LENGTH];

/*
 * Event installer helpers
 */
#define event_id(_e)		(EVT_ID_##_e >> 4)
#define handler_name(_h)	fts_##_h##_event_handler

#define install_handler(_i, _evt, _hnd) \
do { \
		_i->event_dispatch_table[event_id(_evt)] = handler_name(_hnd); \
} while (0)

#if defined(CONFIG_TEE_TUI)
extern struct ts_tui_data tee_tui_data;
#endif

extern SysInfo systemInfo;
extern TestToDo tests;
extern u8 gesture_mask[GESTURE_MASK_SIZE];
extern struct mutex gestureMask_mutex;
char tag[8] = "[ FTS ]\0";
char fts_ts_phys[64];	/* /< buffer which store the input device name assigned
			 * by the kernel */

//static u32 typeOfComand[CMD_STR_LEN] = { 0 };
// /< buffer used to store the command sent from the MP device file node
//static int numberParameters;	/* /< number of parameter passed through the MP  device file node */

static struct class *fts_cmd_class =NULL;


int fts_mode_handler(struct fts_ts_info *info, int force);
static int fts_enable_reg(struct fts_ts_info *info,bool enable);
int writeLockDownInfo(u8 *data, int size,u8 lock_id);
int read_lock_down_info(u8 *lock_data, u8 lock_id, int size);
static void st_set_fold_status(struct ts_fold_info info, int calibration);
static int fts_set_charger_status(u8 charger_switch);

#define STATUS_GLOVE_MODE 6
#define STATUS_FINGER_MODE 1
#define ST_CALIBRATE_NUM 3
#define MATRIX_DATA_NUM 98
#define FTS_MODE_ACTIVE_SIZE 14
#define ROI_DATA_WAIT_TIMEOUT 30
#define GLOVE_STATUS 0x02
#define FINGER_STATUS 0x01
#define DONT_CALIBRATE 0
#define ST_POWER_ON_DELAY_MS 15
#define ST_IOVDD_DELAY_MS 5
#define ST_VCC_DELAY_MS 10
#define ST_PPWER_DELAY_MS 0

#define CHARGER_CMD_SIZE 3
#define CHARGER_IN 0x01
#define CHARGER_OUT 0x00

int check_mutual_raw_res_gap(void);
struct fts_ts_info* fts_get_info(void)
{
	return fts_info;
}

static int fts_set_info(struct fts_ts_info* info)
{
	fts_info = info;
	return 0;
}

static void st_set_ext_event_to_fingers(struct fts_ts_info *info, int wx, int wy, int xer,
	int yer, int ewx, int ewy, unsigned char touchId, unsigned char touchcount)
{
	struct ts_fingers *f_info = NULL;
	u32 max_fingers = fts_info->chip_data->ts_platform_data->max_fingers;

	if((!info)||(!info->fingers_info)){
		TS_LOG_ERR( "%s:input error!\n", __func__);
		return;
	}

	f_info = info->fingers_info;

	TS_LOG_DEBUG("%s: wx:%d, wy:%d, xer:%d, yer:%d,ewx:%d, ewy:%d,touchId:%u, touchcount:%u\n",
		__func__, wx, wy, xer, yer,ewx,ewy, touchId, touchcount);

	if ((touchId <= 0) || (touchId >= max_fingers))
		touchId = 0;
	f_info->fingers[touchId].wx = wx;
	f_info->fingers[touchId].wy = wy;
	f_info->fingers[touchId].xer= xer;
	f_info->fingers[touchId].yer = yer;
	f_info->fingers[touchId].ewx = ewx;
	f_info->fingers[touchId].ewy = ewy;
	return;
}

static void fts_ext_pointer_event_handler(struct fts_ts_info *info,unsigned char *event)
{
	unsigned char touchId = 0;
	unsigned char touchcount = 0;
	int wx = 0;
	int wy = 0;
	int xer = 0;
	int yer = 0;
	int ewx = 0;
	int ewy = 0;

	TS_LOG_DEBUG("fts_ext_pointer_event_handler\n");
	touchId = (event[1] & 0xF0) >> 4;
	touchcount = ((event[5] & 0xC0) >> 6) | ((event[7] & 0xC0) >> 4);
	__clear_bit(touchId, &info->touch_id);

	wx =  (event[2] & 0x0F);
	wy =  (event[2] & 0xF0)>> 4;
	xer = event[3];
	yer = event[4];
	ewx =(0x1F & event[5]);
	ewy = event[6];

	st_set_ext_event_to_fingers(info, wx,wy, xer,yer, ewx, ewy, touchId, touchcount);
}

static void st_set_event_to_fingers(struct fts_ts_info *info, int x, int y, int major,
	int minor, int pressure, int status, unsigned char touchId, unsigned char touchcount)
{
	struct ts_fingers *f_info = NULL;
	u32 max_fingers = fts_info->chip_data->ts_platform_data->max_fingers;
	f_info = info->fingers_info;

	TS_LOG_DEBUG("%s: z:%d, status:%d, touchId:%u, touchcount:%u\n",
		__func__, pressure, status, touchId, touchcount);

	if ((touchId <= 0) || (touchId >= max_fingers))
		touchId = 0;
	if (NOT_HANDLE_EVENT == f_info->fingers[touchId].pressure) {
		TS_LOG_DEBUG("Not handle pointer event, has handled leave event\n");
	}

	memset(&f_info->fingers[touchId], 0, sizeof(f_info->fingers[touchId]));
	f_info->cur_finger_number = 0;
	f_info->fingers[touchId].x = x;
	f_info->fingers[touchId].y = y;
	f_info->fingers[touchId].pressure = pressure;
	f_info->fingers[touchId].status = status;
	f_info->fingers[touchId].major = major;
	f_info->fingers[touchId].minor = minor;
	f_info->cur_finger_number += touchcount;
	TS_LOG_DEBUG("%s:cur_finger_number %d,touchcount %d,major %d,minor %d\n",
		__func__, f_info->cur_finger_number, touchcount, major, minor);
	return;
}

/*
 * New Interrupt handle implementation
 */
/* TODO: Customer should implement their own actions in respond of a gesture
 * event. This is an example that simply print the gesture received and simulate
 * the click on a different button for each gesture. */
/**
  * Event handler for gesture events (EVT_TYPE_USER_GESTURE)
  * Handle gesture events and simulate the click on a different button for any
  *gesture detected (@link gesture_opt Gesture IDs @endlink)
  */
static void fts_gesture_event_handler(struct fts_ts_info *info, unsigned
				      char *event)
{
	int value = 0;

	if (!info->chip_data->support_gesture_mode ||
		true != info->chip_data->easy_wakeup_info.easy_wakeup_flag) {
		TS_LOG_ERR("%s No support_gesture_mode.\n", __func__);
		return;
	}
	TS_LOG_INFO("%s  gesture event data. +\n", __func__);
	if (!info->fingers_info) {
		TS_LOG_ERR("%s null info return\n", __func__);
		return;
	}
	TS_LOG_INFO(
		 "%s  gesture event data: %02X %02X %02X %02X %02X %02X %02X %02X\n",
		 __func__, event[0], event[1], event[2], event[3], event[4],
		 event[5], event[6], event[7]);

	if ((event[0] == EVT_ID_USER_REPORT) && (event[1] ==
		EVT_TYPE_USER_GESTURE)) {
		switch (event[2]) {
		case GEST_ID_DOUBLE_TAP:
			mutex_lock(&info->wrong_touch_lock);
			if (info->chip_data->easy_wakeup_info.off_motion_on ==
				true) {
				info->chip_data->
					easy_wakeup_info.off_motion_on = false;
				value = TS_DOUBLE_CLICK;
			}
			mutex_unlock(&info->wrong_touch_lock);
			TS_LOG_INFO("%s: double tap value = %d!\n",
				__func__, value);
			break;
		default:
			TS_LOG_INFO("%s:  No valid GestureID!\n", __func__);
			goto gesture_done;
		}

		readGestureCoords(event, info);
		info->fingers_info->gesture_wakeup_value = value;
		TS_LOG_INFO("%s: GestureID! value = %d\n",
				__func__, value);
	} else {
		TS_LOG_INFO("%s : Invalid event passed as argument!\n",
			__func__);
	}
gesture_done:
	return;
}

static void fts_nop_event_handler(struct fts_ts_info *info, unsigned
				  char *event)
{
	TS_LOG_DEBUG( "%s Doing nothing for event = %02X %02X %02X %02X %02X %02X %02X %02X\n",
		 __func__, event[0], event[1], event[2], event[3],event[4],event[5], event[6], event[7]);
	return;
}

static void fts_enter_pointer_event_handler(struct fts_ts_info *info,unsigned char *event)
{
	unsigned char touchId = 0, touchcount = 0;
	int x = 0;
	int y = 0;
	int status = 0;
	int status1 = 0;

	TS_LOG_DEBUG("%s enter\n", __func__);

	if ((info == NULL) || (info->fingers_info == NULL)) {
		TS_LOG_ERR("%s null poniter return\n", __func__);
		return;
	}

	status = event[1] & 0x0F;
	touchId = (event[1] & 0xF0) >> 4;
	__set_bit(touchId, &info->touch_id);

	x = (((int)event[3] & 0x0F) << 8) | (event[2]);
	y = ((int)event[4] << 4) | ((event[3] & 0xF0) >> 4);
	touchcount = ((event[5] & 0xC0) >> 6) | ((event[7] & 0xC0) >> 4);

	if(status == 0x02) {
		status1 = STATUS_GLOVE_MODE;
	}else if(status == 0x01) {
		status1 = STATUS_FINGER_MODE;
	}else{
		TS_LOG_ERR("[%s] status error 0x%x\n", __func__, status);
	}

	TS_LOG_DEBUG("[%s] coor status from 0x%x to 0x%x\n",
		__func__, status, status1);

	if (x == info->chip_data->x_max_mt)
	            x--;

	if (y == info->chip_data->y_max_mt)
	            y--;
	/*
	 * When the finger is pressed,major,minor and pressure
	 * should be set to 1. and when the finger is released,
	 * major,minor,presure should set to 0
	 */
	st_set_event_to_fingers(info, x, y, 1, 1, 1,
			status1, touchId, touchcount);
	if (st_roi_switch_on)
		st_roi_data_staled = 1; /* roi_data Ready to read the new */

	return;

}

/* EventId : 0x04 */
static void fts_leave_pointer_event_handler(struct fts_ts_info *info, unsigned char *event)
{
	unsigned char touchId = 0;
	unsigned char touchcount = 0;
	int status = 0;
	int status1 = 0;

	if((!info)||(!info->fingers_info)){
		TS_LOG_ERR( "%s:input error!\n", __func__);
		return;
	}

	TS_LOG_DEBUG("%s fts_leave_pointer_event_handler\n",__func__);

	touchId = (event[1] & 0xF0) >> 4;
	touchcount = ((event[5] & 0xC0) >> 6) | ((event[7] & 0xC0) >> 4);
	status = (event[1] & 0x0F);

	if ((status == FINGER_STATUS) || (status == GLOVE_STATUS))
		status1 = TP_NONE_FINGER;
	else
		TS_LOG_ERR("[%s] status error 0x%x\n",__func__,status);

	__clear_bit(touchId, &info->touch_id);
	/* When the finger is lifted, the pressure 6th parm is 0 */
	st_set_event_to_fingers(info, 0, 0, 0, 0, 0, status1,
			touchId, touchcount);
	return;
}


/* EventId : 0x05 */
#define fts_motion_pointer_event_handler fts_enter_pointer_event_handler


int fts_mode_handler(struct fts_ts_info *info,int force)
{
	int res = OK;
	int ret = OK;
	u8 settings[4] = { 0 };

	if(!info){
		TS_LOG_ERR( "%s:input error!\n", __func__);
		return ERROR_OP_NOT_ALLOW;
	}
	info->mode = MODE_NOTHING;
	TS_LOG_DEBUG("%s: Mode Handler starting...\n",  __func__);

	switch (info->resume_bit) {
	case 0:
		TS_LOG_INFO("%s: Screen OFF...\n",  __func__);
		ret = setScanMode(SCAN_MODE_ACTIVE, 0x00);
		res |= ret;
		setSystemResetedDown(0);
		break;

	case 1:	/* screen up */
		TS_LOG_INFO("%s:Screen ON...\n", __func__);

		settings[0] = 0xFF;
		TS_LOG_DEBUG( "%s: Sense ON!\n", __func__);
		res |= setScanMode(SCAN_MODE_ACTIVE, settings[0]);
		info->mode |= (SCAN_MODE_ACTIVE << 24);
		MODE_ACTIVE(info->mode, settings[0]);

		setSystemResetedUp(0);
		break;

	default:
		TS_LOG_DEBUG("%s: invalid resume_bit value = %d! ERROR %08X\n",
			 __func__, info->resume_bit, ERROR_OP_NOT_ALLOW);
		res = ERROR_OP_NOT_ALLOW;
	}


	TS_LOG_DEBUG("%s: Mode Handler finished! res = %08X mode = %08X\n",__func__, res, info->mode);
	return res;
}


static void fts_error_event_handler(struct fts_ts_info *info, unsigned
				    char *event)
{
	int error = 0;

	TS_LOG_DEBUG("%s Received event %02X %02X %02X %02X %02X %02X %02X %02X\n",
		 __func__, event[0], event[1], event[2], event[3], event[4], event[5], event[6], event[7]);

	switch (event[1]) {
		case EVT_TYPE_ERROR_ESD:/* esd */
			fts_chip_powercycle(info);

			error = fts_system_reset();
			error |= fts_mode_handler(info, 0);
			error |= fts_enableInterrupt();
			if (error < OK)
				TS_LOG_ERR("%s Cannot restore the device ERROR %08X\n", __func__, error);
			break;
		case EVT_TYPE_ERROR_HARD_FAULT:	/* hard fault */
		case EVT_TYPE_ERROR_WATCHDOG:	/* watch dog timer */
			dumpErrorInfo();
			/* before reset clear all slots */
			error = fts_system_reset();
			error |= fts_mode_handler(info, 0);
			error |= fts_enableInterrupt();
			if (error < OK)
				TS_LOG_ERR("%s Cannot reset the device ERROR %08X\n",__func__, error);
			break;
	}
	return;
}


/* EventId : 0x10 */
static void fts_controller_ready_event_handler(struct fts_ts_info *info,
					       unsigned char *event)
{
	int error = 0;

	TS_LOG_INFO("%s Received event %02X %02X %02X %02X %02X %02X %02X %02X\n",
		__func__, event[0], event[1], event[2], event[3], event[4],event[5],event[6], event[7]);

	setSystemResetedUp(1);
	setSystemResetedDown(1);
	error = fts_mode_handler(info, 0);

	if (error < OK)
		TS_LOG_INFO( "%s Cannot restore the device status ERROR %08X\n",__func__, error);
}

/* EventId : 0x16 */
static void fts_status_event_handler(
		struct fts_ts_info *info, unsigned char *event)
{
	if(!info){
		TS_LOG_ERR( "%s:input error!\n", __func__);
		return ;
	}
	switch (event[1]) {
	case EVT_TYPE_STATUS_ECHO:
		TS_LOG_ERR("%s Echo event of command = %02X %02X %02X %02X %02X %02X\n",
			  __func__, event[2], event[3], event[4], event[5],event[6],event[7]);
		break;

	case EVT_TYPE_STATUS_FORCE_CAL:
		switch (event[2]) {
		case EVT_FORCE_CAL_CONTINUOUS_DROP:
			TS_LOG_ERR("%s Continuous frame drop Force cal = %02X %02X %02X %02X %02X %02X\n",
				  __func__, event[2], event[3], event[4],event[5],event[6], event[7]);
			break;

		case EVT_FORCE_CAL_MUTUAL_NEGATIVE:
			TS_LOG_ERR("%s Mutual negative detect Force cal = %02X %02X %02X %02X %02X %02X\n",
				  __func__, event[2], event[3], event[4],event[5], event[6], event[7]);
			break;

		case EVT_FORCE_CAL_MUTUAL_CALIB:
			TS_LOG_ERR("%s Mutual calib deviation Force cal = %02X %02X %02X %02X %02X %02X\n",
				  __func__, event[2], event[3], event[4],event[5],event[6], event[7]);
			break;

		case EVT_FORCE_CAL_SS_NEGATIVE:
			TS_LOG_ERR("%s SS negative detect Force cal = %02X %02X %02X %02X %02X %02X\n",
				  __func__, event[2], event[3], event[4],event[5],event[6], event[7]);
			break;

		case EVT_FORCE_CAL_SS_NEGATIVE_LOW:
			TS_LOG_ERR("%s SS negative detect Force cal in Low Power mode = %02X %02X %02X %02X %02X %02X\n",
				  __func__, event[2], event[3], event[4],event[5],event[6], event[7]);
			break;

		case EVT_FORCE_CAL_SS_NEGATIVE_IDLE:
			TS_LOG_ERR(" %s SS negative detect Force cal in Idle mode = %02X %02X %02X %02X %02X %02X\n",
				  __func__, event[2], event[3], event[4],event[5],event[6], event[7]);
			break;

		case EVT_FORCE_CAL_SS_INVALID_MUTUAL:
			TS_LOG_ERR("%s SS invalid Mutual Strength soft Force cal = %02X %02X %02X %02X %02X %02X\n",
				__func__, event[2], event[3], event[4], event[5], event[6], event[7]);
			break;

		case EVT_FORCE_CAL_SS_INVALID_SELF_STRENGTH:
			TS_LOG_ERR("%s SS invalid Self Strength soft Force cal = %02X %02X %02X %02X %02X %02X\n",
				 __func__, event[2], event[3], event[4],event[5],event[6], event[7]);
			break;

		case EVT_FORCE_CAL_SS_INVALID_SELF_ISLAND:
			TS_LOG_ERR("%s :SS invalid Self Island soft Force cal = %02X %02X %02X %02X %02X %02X\n",
				 __func__, event[2], event[3], event[4],event[5],event[6], event[7]);
			break;

		case EVT_FORCE_CAL_MS_INVALID_MUTUAL:
			TS_LOG_ERR("%s MS invalid Mutual Strength soft Force cal = %02X %02X %02X %02X %02X %02X\n",
				  __func__, event[2], event[3], event[4], event[5],event[6], event[7]);
			break;

		case EVT_FORCE_CAL_MS_INVALID_SELF:
			TS_LOG_ERR("%s MS invalid Self Strength soft Force cal = %02X %02X %02X %02X %02X %02X\n",
				  __func__, event[2], event[3], event[4],event[5],event[6], event[7]);
			break;

		default:
			TS_LOG_ERR("%s Force cal = %02X %02X %02X %02X %02X %02X\n",__func__,
				event[2], event[3], event[4],event[5],event[6], event[7]);
		}
		break;

	case EVT_TYPE_STATUS_FRAME_DROP:
		switch (event[2]) {
		case EVT_FRAME_DROP_NOISY:
			TS_LOG_ERR("%s Frame drop noisy frame = %02X %02X %02X %02X %02X %02X\n",
				 __func__, event[2], event[3], event[4],event[5],event[6], event[7]);
			break;

		case EVT_FRAME_DROP_BAD:
			TS_LOG_ERR("%s Frame drop bad R = %02X %02X %02X %02X %02X %02X\n",
				 __func__,  event[2], event[3], event[4],event[5],event[6], event[7]);
			break;

		case EVT_FRAME_DROP_INVALID:
			TS_LOG_ERR("%s Frame drop invalid processing state = %02X %02X %02X %02X %02X %02X\n",
				 __func__,  event[2], event[3], event[4],event[5],event[6], event[7]);
			break;

		default:
			TS_LOG_ERR("%s Frame drop = %02X %02X %02X %02X %02X %02X\n",
				 __func__, event[2], event[3], event[4],event[5],event[6], event[7]);
		}
		break;

	case EVT_TYPE_STATUS_SS_RAW_SAT:
		if (event[2] == 1){
			TS_LOG_ERR( "%s SS Raw Saturated = %02X %02X %02X %02X %02X %02X\n",
				 __func__, event[2], event[3], event[4],event[5],event[6], event[7]);
		} else {
			TS_LOG_ERR("%s SS Raw No more Saturated = %02X %02X %02X %02X %02X %02X\n",
				 __func__, event[2], event[3], event[4],event[5],event[6], event[7]);
		}
		break;

	case EVT_TYPE_STATUS_WATER:
		if (event[2] == 1)
			TS_LOG_ERR("%s Enter Water mode = %02X %02X %02X %02X %02X %02X\n",
				 __func__, event[2], event[3], event[4], event[5],event[6], event[7]);
		else
			TS_LOG_ERR("%s Exit Water mode = %02X %02X %02X %02X %02X %02X\n",
				 __func__, event[2], event[3], event[4],event[5],event[6], event[7]);
		break;

	default:
		TS_LOG_ERR("%s Received unhandled status event = %02X %02X %02X %02X %02X %02X %02X %02X\n",
			  __func__, event[0], event[1], event[2], event[3],event[4], event[5], event[6], event[7]);
		break;
	}
	return;
}

/* EventId : 0x05 */
#define fts_motion_pointer_event_handler fts_enter_pointer_event_handler

/*1440*3120
 * This handler is called each time there is at least
 * one new event in the FIFO
 */
static void fts_event_handler(struct fts_ts_info *info,
				int *touch_event_count) {

	int error = 0, count = 0;
	unsigned char regAdd = FIFO_CMD_READALL;
	unsigned char data[FIFO_EVENT_SIZE * FIFO_DEPTH] = { 0 };
	unsigned char eventId =0;
	const unsigned char EVENTS_REMAINING_POS = 7;
	const unsigned char EVENTS_REMAINING_MASK = 0x1F;
	unsigned char events_remaining = 0;
	unsigned char *evt_data = NULL;
	unsigned char event_id = 0;

	event_dispatch_handler_t event_handler;


	error = fts_writeReadU8UX(regAdd, 0, 0, data, FIFO_EVENT_SIZE, DUMMY_FIFO);
	events_remaining = data[EVENTS_REMAINING_POS] & EVENTS_REMAINING_MASK;


	if (error == OK && events_remaining > 0) {
		error = fts_writeReadU8UX(regAdd, 0 , 0, &data[FIFO_EVENT_SIZE],
									FIFO_EVENT_SIZE * events_remaining,
									DUMMY_FIFO);
	}
	if (error != OK) {
		TS_LOG_ERR("%s:Error (%d) while reading from FIFO in fts_event_handler",__func__,error);
	} else {
		for (count = 0; count < events_remaining + 1; count++) {
			evt_data = &data[count * FIFO_EVENT_SIZE];

			if (evt_data[0] == EVT_ID_NOEVENT)
				break;

			eventId = evt_data[0] >> 4;
			event_id  = evt_data[0];

			if (eventId < NUM_EVT_ID) {
				event_handler = info->event_dispatch_table[eventId];
				event_handler(info, (evt_data));
			}
			if ((event_id == EVT_ID_ENTER_POINT) ||
				(event_id == EVT_ID_LEAVE_POINT) ||
				(event_id == EVT_ID_MOTION_POINT) ||
				(evt_data[2] == GEST_ID_DOUBLE_TAP))
				(*touch_event_count)++;
		}
	}
	return;
}

static void fts_user_report_event_handler(struct fts_ts_info *info, unsigned
					  char *event)
{
	if(!info || !event){
		TS_LOG_ERR( "%s:input error!\n", __func__);
		return ;
	}
	switch (event[1]) {
		case EVT_TYPE_USER_PROXIMITY:
			if (event[2] == 0)
				TS_LOG_INFO("%s No proximity!\n", __func__);
			else
				TS_LOG_INFO("%s Proximity Detected!\n",__func__);
			break;
		case EVT_TYPE_USER_GESTURE:
			fts_gesture_event_handler(info, event);
			break;
		default:
			TS_LOG_ERR("%s Received unhandled user report event = %02X %02X %02X %02X %02X %02X %02X %02X\n",
				 __func__, event[0], event[1], event[2], event[3],event[4],event[5], event[6], event[7]);
			break;
	}

	return;
}


static void  generate_fw_name(int force, char *fw_name)
{
	struct fts_ts_info *info = fts_get_info();

	if(!fw_name){
		TS_LOG_ERR( "%s:input error!\n", __func__);
		return ;
	}

	if (force)
		snprintf(fw_name, ST_FW_NAME_MAX_LEN -1 , "%s", ST_FW_PATH_SD);
	else
		snprintf(fw_name, ST_FW_NAME_MAX_LEN -1 , "ts/%s_st_%s.bin",
			info->chip_data->ts_platform_data->product_name,
			info->project_id);
	return ;
}


static int fts_fw_update_auto(struct fts_ts_info *info, int force)
{
	int retval = 0;
	int retval1 = 0;
	int ret = 0;
	int crc_status = 0;
	int keep_cx = 1;
	char fw_name[ST_FW_NAME_MAX_LEN] = {'\0'};

	TS_LOG_INFO("%s :tarting ...\n", __func__);

	generate_fw_name(force, fw_name);
	TS_LOG_INFO("%s:fw name:%s\n", __func__, fw_name);

	ret = fts_crc_check();
	if (ret > OK) {
		TS_LOG_ERR("%s: CRC Error or NO FW!\n", __func__);
		crc_status = ret;
	} else {
		crc_status = 0;
		TS_LOG_ERR( "%s: NO CRC Error or Impossible to read CRC register!\n", __func__);
	}
	if(force == 1)
		crc_status =CRC_CODE;

	retval = flashProcedure(fw_name, crc_status, keep_cx);
	if ((retval & 0xFF000000) == ERROR_FLASH_PROCEDURE) {
		TS_LOG_ERR(" %s: firmware update failed and retry! ERROR %08X\n",__func__, retval);
		fts_chip_powercycle(info);	/* power reset */
		retval1 = flashProcedure(fw_name, crc_status, keep_cx);
		if ((retval1 & 0xFF000000) == ERROR_FLASH_PROCEDURE) {
			TS_LOG_ERR("%s: firmware update failed again!  ERROR %08X\n",__func__, retval1);
		}
	}
	retval |= fts_mode_handler(info, 0);
	retval |= fts_enableInterrupt();
	if (retval < OK) {
		TS_LOG_ERR("%s Cannot initialize the hardware device %08X\n",
						__func__, retval);
	}

	TS_LOG_INFO("%s Fw Auto Update Finished!\n", __func__);
	snprintf(info->chip_data->version_name, MAX_STR_LEN, "%x.%x",
		systemInfo.u16_fwVer, systemInfo.u16_cfgProjectId);
	return retval;
}

static int fts_interrupt_install(struct fts_ts_info *info)
{
	int i = 0, error = 0;
	if(!info){
		TS_LOG_ERR( "%s:input error!\n", __func__);
		return ERROR_OP_NOT_ALLOW;
	}
	info->event_dispatch_table = kzalloc(
			sizeof (event_dispatch_handler_t) * NUM_EVT_ID, GFP_KERNEL);

	if (!info->event_dispatch_table) {
		TS_LOG_ERR("%s OOM allocating event dispatch table\n", __func__);
		return -ENOMEM;
	}

	for (i = 0; i < NUM_EVT_ID; i++)
		info->event_dispatch_table[i] = fts_nop_event_handler;

	install_handler(info, ENTER_POINT, enter_pointer);
	install_handler(info, LEAVE_POINT, leave_pointer);
	install_handler(info, MOTION_POINT, motion_pointer);
	install_handler(info, ERROR, error);
	install_handler(info, CONTROLLER_READY, controller_ready);
	install_handler(info, STATUS_UPDATE, status);
	install_handler(info, USER_REPORT, user_report);
	install_handler(info, EXT_POINTER,ext_pointer);

	/* disable interrupts in any case */
	error = fts_disableInterrupt();

	return error;
}


static struct attribute *fts_attr_attributes[] = {
	NULL,
};
struct attribute_group fts_attr_group = {
	.attrs = fts_attr_attributes,
};
static int fts_init(struct fts_ts_info *info)
{
	int error = 0;
	error = fts_system_reset();
	if (error < OK && isI2cError(error)) {
		TS_LOG_ERR("Cannot reset the device! ERROR %08X\n",error);
		return error;
	} else {
		if (error == (ERROR_TIMEOUT | ERROR_SYSTEM_RESET_FAIL)) {
			TS_LOG_ERR("%s Setting default Sys INFO!\n",__func__);
			defaultSysInfo(0);
			return OK;
		} else {
			error = readSysInfo(0);	/* system reset OK */
			if (error < OK) {
				if (!isI2cError(error))
					error = OK;
				TS_LOG_ERR("%s Cannot read Sys Info! ERROR %08X\n",__func__,error);
			}
		}
	}

	return error;
}

int fts_chip_powercycle(struct fts_ts_info *info)
{
	int error = 0;

	TS_LOG_ERR("%s: Power Cycle Starting...\n", __func__);
	TS_LOG_ERR("%s: Disabling IRQ...\n", __func__);	//if IRQ pin is short with DVDD a call to the ISR will triggered when the regulator is turned off

	if(!info || !info->chip_data || !info->chip_data->ts_platform_data){
		TS_LOG_ERR("%s: invalid input.\n", __func__);	
		return -1;
	}
	disable_irq_nosync(info->chip_data->ts_platform_data->irq_id);
	/* delay 5ms after iovdd off */
	error = ts_kit_power_supply_ctrl(TS_KIT_IOVDD, TS_KIT_POWER_OFF,
		ST_IOVDD_DELAY_MS);
	if (error < 0)
		TS_LOG_ERR("%s: fail to disable iovdd power\n", __func__);
	error = ts_kit_power_supply_ctrl(TS_KIT_VCC, TS_KIT_POWER_OFF,
		ST_PPWER_DELAY_MS);
	if (error < 0)
		TS_LOG_ERR("%s: fail to disable vcc power\n", __func__);

	gpio_set_value(info->chip_data->ts_platform_data->reset_gpio, 0);
	msleep(10);
	/* delay 5ms after iovdd on */
	error = ts_kit_power_supply_ctrl(TS_KIT_IOVDD, TS_KIT_POWER_ON,
		ST_IOVDD_DELAY_MS);
	if (error < 0)
		TS_LOG_ERR("%s: fail to enable iovdd power\n", __func__);
	/* delay 10ms after vcc on */
	error = ts_kit_power_supply_ctrl(TS_KIT_VCC, TS_KIT_POWER_ON,
		ST_VCC_DELAY_MS);
	if (error < 0)
		TS_LOG_ERR("%s: fail to enable vcc power\n", __func__);

	gpio_set_value(info->chip_data->ts_platform_data->reset_gpio, 1);
	mdelay(20);
	TS_LOG_ERR("%s: Enabling IRQ...\n", __func__);

	enable_irq(info->chip_data->ts_platform_data->irq_id);

	TS_LOG_ERR("%s: Power Cycle Finished! CODE = %08x\n", __func__, error);
	setSystemResetedUp(1);
	setSystemResetedDown(1);
	return error;
}

static int st_resume(void)
{
	struct fts_ts_info *info = fts_get_info();
	int retval = NO_ERR;
	struct lcd_kit_ops *tp_ops = lcd_kit_get_ops();
	int tskit_pt_station_flag = 0;
	struct fts_ts_info *st_info = fts_get_info();

	TS_LOG_INFO("%s:enter!\n",__func__);
	st_info->resume_bit = 1;
	if((tp_ops)&&(tp_ops->get_status_by_type)) {
		retval = tp_ops->get_status_by_type(PT_STATION_TYPE, &tskit_pt_station_flag);
		if(retval < 0) {
			TS_LOG_ERR("%s: get tskit_pt_station_flag fail\n", __func__);
			return retval;
		}
	}

	switch (st_info->chip_data->easy_wakeup_info.sleep_mode) {
	case TS_POWER_OFF_MODE:
			if (!tskit_pt_station_flag){
				gpio_set_value(st_info->chip_data->ts_platform_data->reset_gpio, 0);
				retval = fts_enable_reg(st_info, 1);
				gpio_set_value(st_info->chip_data->ts_platform_data->reset_gpio, 1);
				msleep(20);
			}
		break;
	case TS_GESTURE_MODE:
		st_info->chip_data->easy_wakeup_info.easy_wakeup_flag = false;
		break;
	default:
		TS_LOG_ERR("no suspend mode\n");
		return -EINVAL;
	}

	retval |= fts_system_reset();
	retval |= fts_mode_handler(info, 0);
	retval |= fts_enableInterrupt();

	TS_LOG_INFO("%s:out!\n",__func__);
	return retval;
}

static  void fts_gesture_mode_in(void)
{
	struct fts_ts_info *info = fts_get_info();
	int res =0;

       TS_LOG_INFO("%s: enter in gesture mode !\n",  __func__);
        gesture_mask[0] = 0xff;
        gesture_mask[1] = 0xff;
        gesture_mask[2] = 0xff;
        gesture_mask[3] = 0x01;
        res = setFeatures(FEAT_SEL_GESTURE, gesture_mask, 4);
        if (res < OK) {
                   TS_LOG_ERR("%s: Gesture suspend setFeatures Faild\n",__func__);
        }
        res = enterGestureMode(isSystemResettedDown());
        if (res >= OK) {
                    fromIDtoMask(FEAT_SEL_GESTURE, (u8 *)&info->mode, sizeof(info->mode));
                    MODE_LOW_POWER(info->mode, 0);
                   TS_LOG_INFO("%s: enterGestureMode suscessful\n", __func__);
        } else {
                   TS_LOG_ERR("%s: enterGestureMode failed! ERROR %08X recovery in senseOff...\n", __func__, res);
        }
	return;
}
static int st_suspend(void)
{
	int retval = NO_ERR;
	struct lcd_kit_ops *tp_ops = lcd_kit_get_ops();
	int tskit_pt_station_flag = 0;
	struct fts_ts_info *st_info = fts_get_info();

	TS_LOG_INFO("%s:enter!\n",__func__);
	st_info->resume_bit = 0;
	if((tp_ops)&&(tp_ops->get_status_by_type)) {
		retval = tp_ops->get_status_by_type(PT_STATION_TYPE, &tskit_pt_station_flag);
		if(retval < 0) {
			TS_LOG_ERR("%s: get tskit_pt_station_flag fail\n", __func__);
			return retval;
		}
	}

	switch (st_info->chip_data->easy_wakeup_info.sleep_mode) {
	case TS_POWER_OFF_MODE:
		/*for in_cell, tp will power off in suspend. */
		if (!tskit_pt_station_flag){
			retval |= fts_enable_reg(st_info, 0);
			gpio_set_value(st_info->chip_data->ts_platform_data->reset_gpio, 0);
		} else {
			retval |= fts_mode_handler(st_info, 0);	/*goto sleep mode*/
		}
		break;
		/*for gesture wake up mode suspend. */
	case TS_GESTURE_MODE:
		st_info->chip_data->easy_wakeup_info.easy_wakeup_flag = true;
		mutex_lock(&st_info->wrong_touch_lock);
		st_info->chip_data->easy_wakeup_info.off_motion_on = true;
		mutex_unlock(&st_info->wrong_touch_lock);
		fts_gesture_mode_in();
		retval |= fts_enableInterrupt();
		break;
	default:
		TS_LOG_ERR("no suspend mode\n");
		return -EINVAL;
	}
	TS_LOG_INFO("%s:out!\n",__func__);

	return retval;
}

static int fts_get_reg(struct fts_ts_info *info, bool get) 
{
	int retval;

	if (!get)
		goto regulator_put;
	retval = ts_kit_power_supply_get(TS_KIT_IOVDD);
	if (retval < 0)
		TS_LOG_ERR("%s: fail to get iovdd power\n", __func__);
	retval = ts_kit_power_supply_get(TS_KIT_VCC);
	if (retval < 0)
		TS_LOG_ERR("%s: fail to get vcc power\n", __func__);
	return retval;

regulator_put:
	retval = ts_kit_power_supply_put(TS_KIT_IOVDD);
	if (retval < 0)
		TS_LOG_ERR("%s: fail to put iovdd power\n", __func__);
	retval = ts_kit_power_supply_put(TS_KIT_VCC);
	if (retval < 0)
		TS_LOG_ERR("%s: fail to put vcc power\n", __func__);
	return 0;
}

static int fts_enable_reg(struct fts_ts_info *info,bool enable) 
{
	int retval = 0;
	if (!enable) {
		retval = 0;
		goto power_off;
	}
	/* delay 5ms after iovdd on */
	retval = ts_kit_power_supply_ctrl(TS_KIT_IOVDD, TS_KIT_POWER_ON,
		ST_IOVDD_DELAY_MS);
	if (retval < 0)
		TS_LOG_ERR("%s: fail to enable iovdd power\n", __func__);
	retval = ts_kit_power_supply_ctrl(TS_KIT_VCC, TS_KIT_POWER_ON,
		ST_POWER_ON_DELAY_MS);
	if (retval < 0)
		TS_LOG_ERR("%s: fail to enable vcc power\n", __func__);
	return OK;

power_off:
	/* delay 5ms after iovdd off */
	retval = ts_kit_power_supply_ctrl(TS_KIT_IOVDD, TS_KIT_POWER_OFF,
		ST_IOVDD_DELAY_MS);
	if (retval < 0)
		TS_LOG_ERR("%s: fail to disable iovdd power\n", __func__);
	retval = ts_kit_power_supply_ctrl(TS_KIT_VCC, TS_KIT_POWER_OFF, 0);
	if (retval < 0)
		TS_LOG_ERR("%s: fail to disable vcc power\n", __func__);
	return retval;
}

static void st_hw_reset(struct ts_kit_device_data *chip_data)
{
	/* reset chip */
	if (chip_data->ts_platform_data->reset_gpio > 0){
		gpio_direction_output(chip_data->ts_platform_data->reset_gpio, 0);
		mdelay(10); /* ST need more than 15ms to keep reset low in SPEC */
		gpio_direction_output(chip_data->ts_platform_data->reset_gpio, 1);
		mdelay(50);
	}
}

static void fts_set_gpio(struct fts_ts_info *info) {
	st_hw_reset(info->chip_data);
	setResetGpio(info->chip_data->ts_platform_data->reset_gpio);
	setIrqGpio(info->chip_data->ts_platform_data->irq_id);
	return ;
}


static int st_calibrate(void)
{
	int res;
	int reval;
	int i = 0;
	struct fts_ts_info *info = fts_get_info();

	info->check_mutual_raw = FTS_TRUE;
repeat:
	disable_irq(info->chip_data->ts_platform_data->irq_id);
	res = production_test_initialization(SPECIAL_FULL_PANEL_INIT);
	if (res < 0) {
		TS_LOG_ERR("%s Error during initialize TEST! ERROR %08X\n",
			__func__, res);
		if (isI2cError(res)) {
			enable_irq(info->chip_data->ts_platform_data->irq_id);
			TS_LOG_ERR("i2c error %08X\n", res);
			return res;
		}
		goto end;
	} else {
		TS_LOG_ERR("%s:INITIALIZATION TEST OK!\n", __func__);
	}

	res = saveMpFlag(MP_FLAG_BOOT);
	if (res < OK)
		TS_LOG_ERR("%s saving MP FLAG! ERROR %08X\n", __func__, res);
	else
		TS_LOG_ERR("%s MP FLAG saving OK!\n", __func__);

	res |= fts_system_reset();
	res |= senseOn();
	res |= fts_enableInterrupt();
	if (res < 0)
		TS_LOG_ERR("Cannot initialize the hardware device after\n");
end:
	enable_irq(info->chip_data->ts_platform_data->irq_id);
	reval = check_mutual_raw_res_gap();
	if (reval) {
		if (i < ST_CALIBRATE_NUM) {
			i++;
			goto repeat;
		}
	}
	info->check_mutual_raw = FTS_FALSE;
	return res;
}



static int st_oem_info_switch(struct ts_oem_info_param *info)
{
	//struct fts_ts_info *st_info = fts_get_info();
	TS_LOG_INFO("[%s]  enter.\n",__func__);
	return 0;
}

int st_get_debug_data(struct ts_diff_data_info* info, struct ts_cmd_node* out_cmd)
{
	int ret = 0;
	return ret;
}

static int st_get_capacitance_test_type(struct ts_test_type_info *info)
{
	int error = NO_ERR;
	struct fts_ts_info *data = fts_get_info();

	if (!info) {
		TS_LOG_ERR("%s:info=%ld\n", __func__, PTR_ERR(info));
		error = -ENOMEM;
		return error;
	}
	switch (info->op_action) {
	case TS_ACTION_READ:
		memcpy(info->tp_test_type, data->chip_data->tp_test_type,
		       TS_CAP_TEST_TYPE_LEN);
		TS_LOG_INFO("read_chip_get_test_type=%s,\n",
			    info->tp_test_type);
		break;
	case TS_ACTION_WRITE:
		break;
	default:
		TS_LOG_ERR("invalid status: %s", info->tp_test_type);
		error = -EINVAL;
		break;
	}
	return error;
}

static int st_holster_switch(struct ts_holster_info *info)
{
	int retval = NO_ERR;
	u8 settings[2]={0x00};
	TS_LOG_INFO("holster switch(%d) action: %d\n", info->holster_switch, info->op_action);
	if (!info) {
		TS_LOG_ERR("synaptics_holster_switch: info is Null\n");
		retval = -ENOMEM;
		return retval;
	}

	switch (info->op_action) {
		case TS_ACTION_WRITE:
			if(info->holster_switch){
				settings[0] = 1;
			} else {
				settings[0] = 0;
			}
			retval = setFeatures(FEAT_SEL_COVER,settings, 1);
			if (retval < 0) {
				TS_LOG_ERR("set holster switch(%d), failed: %d\n",
					   info->holster_switch, retval);
			}
			break;
		case TS_ACTION_READ:
			TS_LOG_INFO
				("invalid holster switch(%d) action: TS_ACTION_READ\n",
				 info->holster_switch);
			break;
		default:
			TS_LOG_INFO("invalid holster switch(%d) action: %d\n", info->holster_switch, info->op_action);
			retval = -EINVAL;
			break;
	}
	return retval;
}

static int st_glove_switch(struct ts_glove_info *info)
{
	int retval = NO_ERR;
	u8 settings[2]={0x00};

	TS_LOG_INFO("%s +\n", __func__);
	if (!info) {
		TS_LOG_ERR("st_glove_switch: info is Null\n");
		retval = -ENOMEM;
		return retval;
	}
	if (!info->glove_supported) {
		TS_LOG_ERR("%s no supported\n", __func__);
		retval = -ENOMEM;
		return retval;
	}
	switch (info->op_action) {
		case TS_ACTION_READ:
			TS_LOG_INFO("read_glove_switch=%d, 1:on 0:off\n",info->glove_switch);
			break;
		case TS_ACTION_WRITE:
			TS_LOG_INFO("write_glove_switch=%d\n",info->glove_switch);
			if(info->glove_switch){
				settings[0] = 1;
			} else {
				settings[0] = 0;
			}
			retval = setFeatures(FEAT_SEL_GLOVE,settings, 1);
			if (retval < 0) {
				TS_LOG_ERR("set glove switch(%d), failed : %d", info->glove_switch,
					   retval);
			}
			break;
		default:
			TS_LOG_ERR("invalid switch status: %d", info->glove_switch);
			retval = -EINVAL;
			break;
	}
	TS_LOG_INFO("%s -\n", __func__);
	return retval;
}

static int st_roi_switch(struct ts_roi_info *info)
{
	if (info == NULL) {
		TS_LOG_ERR("sec_ts_roi_switch: info is null\n");
		return -ENOMEM;
	}

	switch (info->op_action) {
	case TS_ACTION_WRITE:
		TS_LOG_INFO("%s:set roi switch:%d\n", __func__,
			info->roi_switch);
		st_roi_switch_on = info->roi_switch;
		break;
	case TS_ACTION_READ:
		TS_LOG_INFO("%s: roi switch read not surport\n", __func__);
		break;
	default:
		TS_LOG_INFO("%s: default\n", __func__);
		break;
	}
	return 0;
}

static unsigned char *st_ts_roi_rawdata(void)
{
	if (st_roi_switch_on) {
		/* roi_data may be refreshing now,
		 * wait it for some time(30ms)
		 */
		if (st_roi_data_staled) {
			if (!wait_for_completion_interruptible_timeout(
				&st_roi_data_done, msecs_to_jiffies(
				ROI_DATA_WAIT_TIMEOUT))) {
				TS_LOG_DEBUG("%s:wait roi data timeout\n",
					__func__);
				st_roi_data_staled = 0;
				memset(st_roi_data, 0, sizeof(st_roi_data));
			}
		}
	}
	return st_roi_data;
}


void st_ts_work_after_input_kit(void)
{
	u16 matrix_offset;
	u8 matrix_data[MATRIX_DATA_NUM] = { 0x00 };
	int ret;

	matrix_offset = systemInfo.st_matrix_info_addr;
	if (matrix_offset) {
		TS_LOG_DEBUG("%s: matrix offset is 0x%04X\n", __func__,
			matrix_offset);
	} else {
		TS_LOG_ERR("%s: matrix offset is not right\n", __func__);
		return;
	}

	if (st_roi_switch_on) {
		if (st_roi_data_staled == 0) /* roi_data is up to date now */
			return;
		/*
		 * We are about to refresh roi_data
		 * To avoid stale output, use a completion
		 * to block possible readers
		 */
		reinit_completion(&st_roi_data_done);
		ret = fts_writeReadU8UX(FTS_CMD_FRAMEBUFFER_R, BITS_16,
			matrix_offset, matrix_data, MATRIX_DATA_NUM,
			DUMMY_FRAMEBUFFER);
		if (ret < OK)
			TS_LOG_ERR("%s:read matrix data error %08X\n",
				__func__, ret);
		else
			/* matrix data  to st_roi_data[4], store the data
			 * from the forth bit
			 */
			memcpy(&st_roi_data[4], &matrix_data[0],
				ARRAY_SIZE(matrix_data));
	}

	st_roi_data_staled = 0;
	/* If anyone has been blocked by us, wake it up */
	complete_all(&st_roi_data_done);
	return;
}

static int st_after_resume(void *feature_info)
{
	int retval;
	u8 settings[2];
	struct fts_ts_info *fts_info = fts_get_info();
	struct ts_feature_info *info =
		&fts_info->chip_data->ts_platform_data->feature_info;

	TS_LOG_INFO("%s +\n", __func__);
	if (info->glove_info.glove_supported) {
		if (info->glove_info.glove_switch)
			settings[0] = MODE_OPEN;
		else
			settings[0] = MODE_SHUTDOWN;
		retval = setFeatures(FEAT_SEL_GLOVE, &settings[0], 1);
		if (retval)
			TS_LOG_ERR("retore glove switch %d failed: %d\n",
				info->glove_info.glove_switch, retval);
	}
	if (info->holster_info.holster_supported) {
		if (info->holster_info.holster_switch)
			settings[1] = MODE_OPEN;
		else
			settings[1] = MODE_SHUTDOWN;

		retval = setFeatures(FEAT_SEL_COVER, &settings[1], 1);
		if (retval)
			TS_LOG_ERR("retore holster switch %d, failed: %d\n",
				info->holster_info.holster_switch, retval);
	}
	if (info->charger_info.charger_supported) {
		retval = fts_set_charger_status(
			info->charger_info.charger_switch);
		if (retval < 0)
			TS_LOG_ERR("fts_set_charger_status failed\n");
	}
	st_set_fold_status(info->fold_info, DONT_CALIBRATE);
	TS_LOG_INFO("%s -\n", __func__);
	return OK;
}

static int st_before_suspend(void)
{
	int retval = NO_ERR;

	TS_LOG_INFO("%s +\n", __func__);
	TS_LOG_INFO("%s -\n", __func__);
	return retval;
}

static int st_chip_get_info(struct ts_chip_info_param *info)
{
	int retval = NO_ERR;
	struct fts_ts_info *ts_info = fts_get_info();

	snprintf(info->ic_vendor, sizeof(info->ic_vendor), "st");
	snprintf(info->mod_vendor, sizeof(info->mod_vendor), ts_info->project_id);
	snprintf(info->fw_vendor, sizeof(info->fw_vendor),
		"%x.%x", systemInfo.u16_fwVer, systemInfo.u16_cfgProjectId);

	return retval;
}


static int st_fw_update_sd(void)
{
	struct fts_ts_info *st_info = fts_get_info();
	int ret = 0;

	TS_LOG_INFO("%s: +\n", __func__);
	ret = fts_fw_update_auto(st_info, true);
	return ret;
}

static int st_fw_update_boot(char *file_name)
{
	struct fts_ts_info *st_info = fts_get_info();
	int ret = 0;

	TS_LOG_INFO("%s: enter\n", __func__);
	ret= fts_fw_update_auto(st_info, false);
	TS_LOG_INFO("%s: end\n", __func__);
	return ret;
}

static int st_irq_bottom_half(struct ts_cmd_node *in_cmd, struct ts_cmd_node *out_cmd)
{
	/*
	 * to avoid reading all FIFO, we read the first event and
	 * then check how many events left in the FIFO
	 */

	struct fts_ts_info *info = fts_get_info();
	int touch_evnet_count = 0;
	if(!info || !info->chip_data) {
		TS_LOG_ERR( "%s:input error!\n", __func__);
		return ERROR_OP_NOT_ALLOW;
	}
	TS_LOG_DEBUG("%s: +\n", __func__);

	info->fingers_info = &out_cmd->cmd_param.pub_params.algo_param.info;

	out_cmd->command = TS_INPUT_ALGO;
	out_cmd->cmd_param.pub_params.algo_param.algo_order =
		info->chip_data->algo_id;

	fts_event_handler(info, &touch_evnet_count);

	if(NULL == info->fingers_info){
		TS_LOG_ERR("fingers_info is NULL pointer\n");
		return NO_ERR;
	}

	/*
	 * If no touch report event, shoud not do input report
	 */
	if(!touch_evnet_count) {
		out_cmd->command = TS_INVAILD_CMD;
		return NO_ERR;
	}

	TS_LOG_DEBUG("%s: -\n", __func__);
	return NO_ERR;
}

static void cap_test_parse_dts(struct device_node *device)
{
	int retval;
	struct fts_ts_info *ts = fts_get_info();

	if (!ts) {
		TS_LOG_ERR("%s ts null\n", __func__);
		return;
	}
	retval = of_property_read_u32(device, "self_cap_test_support",
				&ts->chip_data->self_cap_test);
	if (retval) {
		TS_LOG_ERR("get self_cap_test support = 0\n" );
		ts->chip_data->self_cap_test = 0;
	}
	retval = of_property_read_u32(device, "support_self_calibrate_test",
		&ts->chip_data->support_self_calibrate_test);
	if (retval) {
		ts->chip_data->support_self_calibrate_test = 0;
		TS_LOG_ERR("support_self_calibrate_test = 0\n");
	}
	retval = of_property_read_u32(device, "support_ito_cap_test",
		&ts->chip_data->support_ito_cap_test);
	if (retval) {
		ts->chip_data->support_ito_cap_test = 0;
		TS_LOG_ERR("support_ito_cap_test = 0\n");
	}
	retval = of_property_read_u32(device, "support_raw_adjacent_test",
		&ts->support_raw_adjacent_test);
	if (retval) {
		ts->support_raw_adjacent_test = 0;
		TS_LOG_ERR("support_raw_adjacent_test = 0\n");
	}
}

static int st_parse_dts(struct device_node *device, struct ts_kit_device_data *chip_data)
{
	int retval  = NO_ERR;

	TS_LOG_INFO("%s st parse dts start\n", __func__);

	retval = of_property_read_u32(device, ST_IRQ_CFG, &chip_data->irq_config);
	if (retval) {
		TS_LOG_ERR("get irq config failed\n");
		retval = -EINVAL;
		goto err;
	}

	retval = of_property_read_u32(device, "i2c_address", &chip_data->slave_addr);
	if (retval) {
		TS_LOG_ERR("get i2c address failed, use default 0x49\n");
		chip_data->slave_addr = 0x49;
	}

	retval = of_property_read_u32(device, ST_ALGO_ID, &chip_data->algo_id);
	if (retval) {
		TS_LOG_ERR("get algo id failed\n");
		retval = -EINVAL;
		goto err;
	}

	retval = of_property_read_u32(device, ST_X_MAX, &chip_data->x_max);
	if (retval) {
		TS_LOG_ERR("get device x_max failed\n");
		retval = -EINVAL;
		goto err;
	}
	retval = of_property_read_u32(device, ST_Y_MAX, &chip_data->y_max);
	if (retval) {
		TS_LOG_ERR("get device y_max failed\n");
		retval = -EINVAL;
		goto err;
	}
	retval = of_property_read_u32(device, ST_X_MAX_MT, &chip_data->x_max_mt);
	if (retval) {
		TS_LOG_ERR("get device x_max failed\n");
		retval = -EINVAL;
		goto err;
	}
	retval = of_property_read_u32(device, ST_Y_MAX_MT, &chip_data->y_max_mt);
	if (retval) {
		TS_LOG_ERR("get device y_max failed\n");
		retval = -EINVAL;
		goto err;
	}
	retval = of_property_read_u32(device,
		"support_notice_aft_gesture_mode",
		&chip_data->support_notice_aft_gesture_mode);
	if (retval) {
		TS_LOG_INFO("get support_notice_aft_gesture_mode = 0\n");
		chip_data->support_notice_aft_gesture_mode = 0;
	}
	retval = of_property_read_u32(device, "support_extra_key_event_input",
		&chip_data->support_extra_key_event_input);
	if (retval) {
		TS_LOG_INFO("get support_extra_key_event_input = 0\n");
		chip_data->support_extra_key_event_input = 0;
	}
	retval = of_property_read_u32(device, "support_gesture_mode",
		&chip_data->support_gesture_mode);
	if (retval) {
		chip_data->support_gesture_mode = 0;
		TS_LOG_ERR("support_gesture_mode = 0\n");
	}
	cap_test_parse_dts(device);
	TS_LOG_INFO("irq_config=%d, algo_id=%d, x_max=%d, y_max=%d, x_mt=%d,y_mt=%d\n", \
		chip_data->irq_config, chip_data->algo_id,
		chip_data->x_max, chip_data->y_max, chip_data->x_max_mt, chip_data->y_max_mt);

	retval = of_property_read_u32(device, "tui_special_feature_support",
		&chip_data->tui_special_feature_support);
	if (retval)
		chip_data->tui_special_feature_support = 0;
	TS_LOG_INFO("get tui_special_feature_support = %u\n",
		chip_data->tui_special_feature_support);
err:
	return retval;
}

static int st_get_project_id(struct fts_ts_info *fts_info)
{
	int ret = 0;

	fts_info->project_id[ST_PROJECT_ID_LEN] = '\0';
	ret = read_lock_down_info(fts_info->project_id, FTS_LOCK_ID,
		ST_PROJECT_ID_LEN);
	if (ret < OK)
		TS_LOG_ERR("%s: project_id read failed\n", __func__);

	TS_LOG_INFO("%s: project_id=%s\n", __func__, fts_info->project_id);
	return 0;
}



static int st_init_chip(void)
{
	int retval = 0;
	struct fts_ts_info *info = fts_get_info();

	TS_LOG_INFO("%s: in\n", __func__);

	info->resume_bit = 1;
	st_get_project_id(info);

	snprintf(info->chip_data->chip_name, MAX_STR_LEN, "st");
	snprintf(info->chip_data->module_name, MAX_STR_LEN, info->project_id);
	snprintf(info->chip_data->version_name, MAX_STR_LEN, "%x.%x",
		systemInfo.u16_fwVer, systemInfo.u16_cfgProjectId);

	retval |= fts_interrupt_install(info);
	mutex_init(&gestureMask_mutex);
	mutex_init(&info->wrong_touch_lock);
	if (retval < OK)
		TS_LOG_ERR("%s fts_interrupt_install (ERROR = %08X)\n", __func__, retval);
	/*I2C cmd*/
	if (fts_cmd_class == NULL)
		fts_cmd_class = class_create(THIS_MODULE, FTS_TS_DRV_NAME);

	info->i2c_cmd_dev = device_create(fts_cmd_class,NULL, DCHIP_ID_0, info, "fts_i2c");
	if (IS_ERR(info->i2c_cmd_dev)) {
		TS_LOG_ERR("%s: Failed to create device for the sysfs!\n", __func__);
	}

	dev_set_drvdata(info->i2c_cmd_dev, info);

	retval = sysfs_create_group(&info->i2c_cmd_dev->kobj,&fts_attr_group);
	if (retval) {
		TS_LOG_ERR("%s: Failed to create sysfs group!\n", __func__);
	}


	retval |= fts_mode_handler(info, 0);
	retval |= fts_enableInterrupt();
	retval = fts_proc_init();
	if (retval < OK)
		TS_LOG_ERR("%s Error: can not create /proc file!\n", __func__);

#if defined(CONFIG_TEE_TUI)
	strncpy(tee_tui_data.device_name, "new_st", strlen("new_st"));
	tee_tui_data.device_name[strlen("new_st")] = '\0';
	TS_LOG_INFO("%s irq gpio = %d\n", __func__,
		info->chip_data->ts_platform_data->irq_gpio);
	tee_tui_data.tui_special_feature_support =
		info->chip_data->tui_special_feature_support;
	if ((tee_tui_data.tui_special_feature_support & TP_TUI_NEW_IRQ_MASK) ==
		TP_TUI_NEW_IRQ_SUPPORT)
		tee_tui_data.tui_irq_gpio =
			info->chip_data->ts_platform_data->irq_gpio;
#endif

	TS_LOG_INFO("%s: -\n", __func__);
	return 0;
}

static int st_input_config(struct input_dev *input_dev)
{
	struct fts_ts_info *st_info = fts_get_info();

	set_bit(EV_SYN, input_dev->evbit);
	set_bit(EV_KEY, input_dev->evbit);
	set_bit(EV_ABS, input_dev->evbit);
	set_bit(BTN_TOUCH, input_dev->keybit);
	set_bit(BTN_TOOL_FINGER, input_dev->keybit);

	set_bit(TS_DOUBLE_CLICK, input_dev->keybit);
	set_bit(TS_SLIDE_L2R, input_dev->keybit);
	set_bit(TS_SLIDE_R2L, input_dev->keybit);
	set_bit(TS_SLIDE_T2B, input_dev->keybit);
	set_bit(TS_SLIDE_B2T, input_dev->keybit);
	set_bit(TS_CIRCLE_SLIDE, input_dev->keybit);
	set_bit(TS_LETTER_C, input_dev->keybit);
	set_bit(TS_LETTER_E, input_dev->keybit);
	set_bit(TS_LETTER_M, input_dev->keybit);
	set_bit(TS_LETTER_W, input_dev->keybit);
	set_bit(TS_PALM_COVERED, input_dev->keybit);

	set_bit(KEY_FLIP, input_dev->keybit);
	set_bit(TS_TOUCHPLUS_KEY0, input_dev->keybit);
	set_bit(TS_TOUCHPLUS_KEY1, input_dev->keybit);
	set_bit(TS_TOUCHPLUS_KEY2, input_dev->keybit);
	set_bit(TS_TOUCHPLUS_KEY3, input_dev->keybit);
	set_bit(TS_TOUCHPLUS_KEY4, input_dev->keybit);

#ifdef INPUT_PROP_DIRECT
	set_bit(INPUT_PROP_DIRECT, input_dev->propbit);
#endif
	input_set_abs_params(input_dev, ABS_X,
				0, st_info->chip_data->x_max_mt - 1, 0, 0);
	input_set_abs_params(input_dev, ABS_Y,
				0, st_info->chip_data->y_max_mt - 1, 0, 0);
	input_set_abs_params(input_dev, ABS_PRESSURE, 0, 255, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TRACKING_ID, 0, 15, 0, 0);

	input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0,
				st_info->chip_data->x_max_mt - 1, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0,
				st_info->chip_data->y_max_mt - 1, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_PRESSURE, 0, 127, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_WIDTH_MAJOR, 0, 100, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_WIDTH_MINOR, 0, 100, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, 127, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MINOR, 0, 127, 0, 0);

	st_info->input_dev = input_dev;

	return NO_ERR;
}

static int st_irq_top_half(struct ts_cmd_node *cmd)
{
	cmd->command = TS_INT_PROCESS;
	TS_LOG_DEBUG("st irq top half called\n");
	return NO_ERR;
}

static void st_shutdown(void)
{
	struct fts_ts_info *info = fts_get_info();

	TS_LOG_INFO("%s +\n", __func__);
	gpio_set_value(info->chip_data->ts_platform_data->reset_gpio, 0);
	fts_enable_reg(info, false);

	TS_LOG_INFO("%s -\n", __func__);
	return;
}

static int st_chip_detect(struct ts_kit_platform_data *data)
{
	int retval = NO_ERR;
	struct fts_ts_info *ts = fts_get_info();
	TS_LOG_INFO("st chip detect called\n");

	if (!data || !ts || !ts->chip_data || !data->client) {
		TS_LOG_ERR("%s: input is NULL\n", __func__);
		return -ENOMEM;
	}

	ts->chip_data->ts_platform_data = data;
	st_parse_dts(ts->chip_data->cnode, ts->chip_data);
	ts->dev = &data->ts_dev->dev;
	data->ts_dev->dev.of_node = ts->chip_data->cnode;
	data->client->addr = ts->chip_data->slave_addr;

	retval = fts_get_reg(ts, 1);
	if (retval){
		TS_LOG_ERR("st regulator get fail\n");
	}
	gpio_set_value(ts->chip_data->ts_platform_data->reset_gpio, 0);
	fts_enable_reg(ts, 1);
	fts_set_gpio(ts);

	TS_LOG_INFO("Init Core Lib:\n");
	openChannel(data->client);
	initCore(ts);

	retval = fts_init(ts);
	if (retval) {
		TS_LOG_ERR("st chip detect fail\n");
		goto detect_fail;
	}
	if (ts->chip_data->support_gesture_mode) {
		ts->chip_data->easy_wakeup_info.easy_wakeup_flag = false;
		ts->chip_data->easy_wakeup_info.sleep_mode = TS_POWER_OFF_MODE;
		ts->chip_data->easy_wakeup_info.off_motion_on = false;
		ts->chip_data->easy_wakeup_info.easy_wakeup_gesture = false;
	}
	init_completion(&st_roi_data_done);
	if (data->max_fingers > TOUCH_ID_MAX)
		ts->chip_data->fold_fingers_supported = 1; /* use 20 fingers */
	TS_LOG_INFO("st detect success\n");

	return 0;

detect_fail:
	retval |= fts_enable_reg(ts, 0);
	retval |= fts_get_reg(ts, 0);

	TS_LOG_INFO("free chip data\n");
	kfree(ts->chip_data);
	ts->chip_data = NULL;

	if (ts->event_dispatch_table) {
		kfree(ts->event_dispatch_table);
		ts->event_dispatch_table = NULL;
	}
	kfree(ts);

	return retval;
}

static int st_get_testdata(struct ts_rawdata_info *info, struct ts_cmd_node *out_cmd)
{
	int retval = 0;
	struct fts_ts_info *fts_info = fts_get_info();

	TS_LOG_INFO("%s enter.\n", __func__);
	st_get_rawdata_aftertest(info,MP_FLAG_FACTORY);
	retval |= cleanUp(0);					//system reset
	retval |= fts_mode_handler(fts_info, 0);			//enable the features and the sensing
       msleep(500);

	TS_LOG_INFO("%s:MS RAW DATA TESTsleep 500ms\n",__func__);
	retval |= fts_enableInterrupt();
	if (retval<0) {
		TS_LOG_ERR("Cannot initialize the hardware device\n");
		return retval;
	}

	TS_LOG_INFO("%s end.\n", __func__);
	return NO_ERR;
}

static void st_set_tp_ic_cmd(struct tp_ic_command ic_cmd)
{
	struct ts_fold_info fold_info = {0};
	int calibrate = 0;
	int i;

	if ((ic_cmd.data == NULL) || (ic_cmd.length == 0)) {
		TS_LOG_ERR("%s:invalid input\n", __func__);
		return;
	}
	for (i = 0; i < ic_cmd.length; i++)
		TS_LOG_DEBUG("%s:ic cmd is %x\n", __func__, ic_cmd.data[i]);
	switch (ic_cmd.type) {
	case SCREEN_CALIBRATE:
		/* in calibrate, data[3] is fold_status */
		fold_info.fold_status = ic_cmd.data[3];
		calibrate = 1;
		st_set_fold_status(fold_info, calibrate);
		break;
	case SCREEN_FOLD:
	case SCREEN_FLIP:
		/* in fold, data[2] is fold_status, data[3] is flip_switch */
		fold_info.fold_status = ic_cmd.data[2];
		fold_info.flip_switch = ic_cmd.data[3];
		calibrate = 0;
		st_set_fold_status(fold_info, calibrate);
		break;
	default:
		TS_LOG_ERR("%s:invalid cmd type %d\n", __func__,
			ic_cmd.type);
	}
}

static void st_set_fold_status(struct ts_fold_info info, int calibration)
{
	struct fts_ts_info *ts = fts_get_info();
	struct ts_feature_info *feature_info =
		&fts_info->chip_data->ts_platform_data->feature_info;
	int retval;
	/*
	 * In fold_status_cmd array, 0xA4 and 0x0B are the command head of
	 * setting fold status to ic. cmd[2] means fold status of the screen
	 * which can be FULL_SCREEN(0x00), MAIN_SCREEN(0x01) or
	 * MINOR_SCREEN(0x02), cmd[3] means flip switch which can be
	 * ON(0x00) or OFF(0x01)
	 */
	unsigned char fold_status_cmd[4] = { 0xA4, 0x0B, 0x00, 0x00 };
	/*
	 * In force_calibrate_cmd array, 0xA4, 0x02, 0x05 are the command
	 * head of doing force calibration.cmd[3] means fold status of screen
	 * which can be FULL_SCREEN(0x00), MAIN_SCREEN(0x01) or MINOR_
	 * SCREEN(0x02)
	 */
	unsigned char force_calibrate_cmd[4] = { 0xA4, 0x02, 0x05, 0x00 };

	if (!ts->chip_data->fold_status_supported)
		return;
	switch (info.fold_status) {
	/* just valid value can be write to tp ic */
	case FULL_SCREEN:
	case MAIN_SCREEN:
	case MINOR_SCREEN:
		fold_status_cmd[2] = info.fold_status;
		fold_status_cmd[3] = info.flip_switch;
		force_calibrate_cmd[3] = info.fold_status;
		break;
	default:
		return;
	}

	if (!calibration) {
		retval = fts_write(fold_status_cmd, sizeof(fold_status_cmd));
		TS_LOG_INFO("%s:%x, %x, %x, %x\n", __func__,
			fold_status_cmd[0], fold_status_cmd[1],
			fold_status_cmd[2], fold_status_cmd[3]);
		feature_info->fold_info.flip_switch = info.flip_switch;
	} else {
		retval = fts_write(force_calibrate_cmd,
			sizeof(force_calibrate_cmd));
		TS_LOG_INFO("%s:%x, %x, %x, %x\n", __func__,
			force_calibrate_cmd[0], force_calibrate_cmd[1],
			force_calibrate_cmd[2], force_calibrate_cmd[3]);
	}
	feature_info->fold_info.fold_status = info.fold_status;
	if (retval < OK)
		TS_LOG_ERR("%s: write failed error %08X\n", __func__, retval);
}

static void st_set_recovery_window(void)
{
	int retval;
	unsigned char settings;
	struct ts_kit_platform_data *p_data = NULL;
	struct fts_ts_info *ts = fts_get_info();

	/*
	 * In the last step of recovery, the recovery thread will
	 * send suspend and resume cmd. If not set holster switch,
	 * the recovery window will be closed in resume process
	 */
	if (ts && ts->chip_data && ts->chip_data->ts_platform_data) {
		p_data = ts->chip_data->ts_platform_data;
		p_data->feature_info.holster_info.holster_switch = 1;
	}
	/*
	 * settings = 1 means turning on window switch
	 * settings = 0 means turning off window switch
	 */
	settings = 1;
	retval = setFeatures(FEAT_SEL_COVER, &settings, sizeof(settings));
	if (retval < 0)
		TS_LOG_ERR("%s:set features failed: %d\n",
			__func__, retval);
}

int check_mutual_raw_res_gap(void)
{
	int ret;
	struct ts_rawdata_info *info = vmalloc(sizeof(struct ts_rawdata_info));

	if (info == NULL) {
		TS_LOG_ERR("%s:kzalloc failed\n", __func__);
		return -ENOMEM;
	}
	memset(info, 0, sizeof(struct ts_rawdata_info));
	ret = st_get_testdata(info, NULL);
	TS_LOG_INFO("%s: %s\n", __func__, info->result);
	if (strstr(info->result, "-11F")) {
		TS_LOG_ERR("%s: check rawgap failed !\n", __func__);
		ret = -EINVAL;
	}
	vfree(info);
	return ret;
}

static int fts_palm_switch(struct ts_palm_info *info)
{
	return 0;
}

static int fts_set_charger_status(u8 charger_switch)
{
	int ret;
	/* Charger mode ic cmd 0xA2, 0x02, 0x00 */
	unsigned char charger_cmd[CHARGER_CMD_SIZE] = { 0xA2, 0x02, 0x00 };

	/* charger_cmd[2] is charger in or out */
	if (charger_switch)
		charger_cmd[2] = CHARGER_IN;
	else
		charger_cmd[2] = CHARGER_OUT;

	TS_LOG_INFO("%s:%x, %x, %x\n", __func__,
		charger_cmd[0], charger_cmd[1], charger_cmd[2]);
	ret = fts_write(charger_cmd, sizeof(charger_cmd));
	if (ret < 0)
		TS_LOG_ERR("%s: write failed error %08X\n", __func__, ret);
	return ret;
}

static int fts_charger_switch(struct ts_charger_info *info)
{
	int retval;

	if (!info) {
		TS_LOG_ERR("%s: info is null\n", __func__);
		return -EINVAL;
	}
	if (!info->charger_supported) {
		TS_LOG_ERR("%s: no charger supported\n", __func__);
		return -EINVAL;
	}
	switch (info->op_action) {
	case TS_ACTION_WRITE:
		retval = fts_set_charger_status(info->charger_switch);
		if (retval < 0)
			TS_LOG_ERR("%s set charger status %d, failed: %d\n",
				__func__, info->charger_switch, retval);
		break;
	default:
		TS_LOG_ERR("%s invalid cmd\n", __func__);
		retval = -EINVAL;
		break;
	}
	return retval;
}

static int fts_wrong_touch(void)
{
	struct fts_ts_info *ts_info = fts_get_info();

	mutex_lock(&ts_info->wrong_touch_lock);
	ts_info->chip_data->easy_wakeup_info.off_motion_on = true;
	mutex_unlock(&ts_info->wrong_touch_lock);
	TS_LOG_INFO("done\n");
	return 0;
}

struct ts_device_ops ts_kit_st_ops = {
	.chip_detect = st_chip_detect,
	.chip_init = st_init_chip,
	.chip_parse_config = st_parse_dts,
	.chip_input_config = st_input_config,
	.chip_irq_top_half = st_irq_top_half,
	.chip_irq_bottom_half = st_irq_bottom_half,
	.chip_fw_update_boot = st_fw_update_boot,
	.chip_fw_update_sd = st_fw_update_sd,
	.chip_get_info = st_chip_get_info,
	.chip_before_suspend = st_before_suspend,
	.chip_suspend = st_suspend,
	.chip_resume = st_resume,
	.chip_after_resume = st_after_resume,
	.chip_get_rawdata = st_get_testdata,
	.chip_glove_switch = st_glove_switch,
	.chip_shutdown = st_shutdown,
	.chip_holster_switch = st_holster_switch,
	.chip_roi_switch = st_roi_switch,
	.chip_roi_rawdata = st_ts_roi_rawdata,
	.chip_get_capacitance_test_type = st_get_capacitance_test_type,
	.chip_calibrate = st_calibrate,
	.chip_work_after_input = st_ts_work_after_input_kit,
	.oem_info_switch = st_oem_info_switch,
	.chip_get_debug_data = st_get_debug_data,
	.chip_set_tp_ic_cmd = st_set_tp_ic_cmd,
	.chip_set_fold_status = st_set_fold_status,
	.chip_set_recovery_window = st_set_recovery_window,
	.chip_palm_switch = fts_palm_switch,
	.chip_wrong_touch = fts_wrong_touch,
	.chip_charger_switch = fts_charger_switch,
	.chip_reset = NULL,
};

static int __init st_ts_module_init(void)
{
	bool found = false;
	struct device_node *child = NULL;
	struct device_node *root = NULL;
	int error = NO_ERR;
	struct fts_ts_info *st_info = NULL;

	TS_LOG_ERR(" %s: called\n", __func__);

	st_info = kzalloc(sizeof(struct fts_ts_info), GFP_KERNEL);
	if (!st_info) {
		TS_LOG_ERR("st get ts_info mem fail\n");
		return -ENOMEM;
	}

	st_info->chip_data = kzalloc(sizeof(struct ts_kit_device_data), GFP_KERNEL);
	if (!st_info->chip_data) {
		TS_LOG_ERR("Failed to alloc mem for struct chip_data\n");
		error =  -ENOMEM;
		goto out;
	}

	fts_set_info(st_info);

	root = of_find_compatible_node(NULL, NULL, "huawei,ts_kit");
	if (!root) {
		TS_LOG_ERR("huawei_ts, find_compatible_node huawei,ts_kit error\n");
		error = -EINVAL;
		goto out;
	}

	for_each_child_of_node(root, child) {
		if (of_device_is_compatible(child, "st_ts")) {
			found = true;
			break;
		}
	}
	if (!found) {
		TS_LOG_ERR(" not found chip st_ts child node  !\n");
		error = -EINVAL;
		goto out;
	}

	st_info->chip_data->cnode = child;
	st_info->chip_data->ops = &ts_kit_st_ops;

	error = huawei_ts_chip_register(st_info->chip_data);
	if (error) {
		TS_LOG_ERR(" sec_ts chip register fail !\n");
		goto out;
	}
	TS_LOG_ERR("st_ts chip_register! err=%d\n", error);
	return error;
out:
	if (st_info) {
		if (st_info->chip_data){
			kfree(st_info->chip_data);
			st_info->chip_data = NULL;
		}
		kfree(st_info);
	}
	return error;
}

static void __exit st_ts_module_exit(void)
{
	TS_LOG_INFO("st_ts_module_exit called here\n");
	return;
}

late_initcall(st_ts_module_init);
module_exit(st_ts_module_exit);


MODULE_DESCRIPTION("STMicroelectronics MultiTouch IC Driver");
MODULE_AUTHOR("STMicroelectronics, Inc.");
MODULE_LICENSE("GPL");
