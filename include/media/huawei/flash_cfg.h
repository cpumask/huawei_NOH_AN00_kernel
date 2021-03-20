


#ifndef __HW_ALAN_KERNEL_HWCAM_FLASH_CFG_H__
#define __HW_ALAN_KERNEL_HWCAM_FLASH_CFG_H__

#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/videodev2.h>
#include <media/huawei/camera.h>

/***************************** cfg type define *******************************/
#define CFG_FLASH_TURN_ON		0
#define CFG_FLASH_TURN_OFF		1
#define CFG_FLASH_GET_FLASH_NAME	2
#define CFG_FLASH_GET_FLASH_STATE	3
#define CFG_FLASH_GET_FLASH_TYPE	5

typedef enum {
	STANDBY_MODE=0,
	FLASH_MODE,
	TORCH_MODE,
	TORCH_LEFT_MODE,
	TORCH_RIGHT_MODE,
	PREFLASH_MODE,
	FLASH_STROBE_MODE, // strobe is a trigger method of FLASH mode
	MAX_MODE,
} flash_mode;

typedef enum {
	LED_FLASH = 0,
	XEON_FLASH =1,
	IR_FLASH = 2,
} flash_type;

struct flash_i2c_reg {
	unsigned int address;
	unsigned int value;
};

typedef enum {
	HWFLASH_POSITION_REAR = 0,
	HWFLASH_POSITION_FORE = 1,
} flash_position_t;

struct hw_flash_cfg_data {
	int cfgtype;
	flash_mode mode;
	int data;
	int position;

	union {
	char name[32];
	} cfg;
};

/********************** v4l2 subdev ioctl case id define **********************/
#define VIDIOC_HISI_FLASH_CFG	\
	_IOWR('V', BASE_VIDIOC_PRIVATE + 41, struct hw_flash_cfg_data)

#endif // __HW_ALAN_KERNEL_HWCAM_FLASH_CFG_H__

