/*
 * hifi om.
 *
 * Copyright (c) 2013 Hisilicon Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __HIFI_OM_H__
#define __HIFI_OM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define UNUSED_PARAMETER(x) (void)(x)
#define LOG_PATH_HISI_LOGS "/data/hisi_logs/"
#define LOG_PATH_RUNNING_TRACE "/data/hisi_logs/running_trace/"
#define LOG_PATH_HIFI_LOG "/data/hisi_logs/running_trace/hifi_log/"
#define LOG_PATH_HIFI_REQUEST_LOG "/data/hisi_logs/running_trace/hifi_log/hifi_requset/"
#define LOG_PATH_BSD_LOG "/data/hisi_logs/running_trace/hifi_log/bsd_log/"
#define HIFI_OM_DATA_PTAH "/data/hisi_logs/om_data/"
#define FILE_NAME_DUMP_DSP_LOG		 "hifi.log"
#define FILE_NAME_DUMP_DSP_BIN		 "hifi.bin"
#define FILE_NAME_DUMP_DSP_PANIC_LOG "hifi_panic.log"
#define FILE_NAME_DUMP_DSP_PANIC_BIN "hifi_panic.bin"
#define FILE_NAME_DUMP_DSP_OCRAM_BIN "hifi_ocram.bin"
#define FILE_NAME_DUMP_DSP_TCM_BIN   "hifi_tcm.bin"

#define HIFIDEBUG_PATH                                "hifidebug"
#define HIFIDEBUG_LEVEL_PROC_FILE                     "debuglevel"
#define HIFIDEBUG_DSPDUMPLOG_PROC_FILE                "dspdumplog"
#define HIFIDEBUG_FAULTINJECT_PROC_FILE               "dspfaultinject"
#define HIFIDEBUG_MISCPROC_PROC_FILE                  "miscproc"

#ifndef LOG_TAG
#define LOG_TAG "hifi_misc "
#endif

#define HIFI_DUMP_FILE_NAME_MAX_LEN 256
#define DSP_DUMP_MAX_EFFECTS_CNT        (10)/*与HIFI  中dps_dump.h 中的定义保持一致*/
#define ROOT_UID     0
#define SYSTEM_GID   1000
#define HIFI_OM_DIR_LIMIT		0750
#define HIFI_OM_FILE_LIMIT		0640
#define HIFI_OM_LOG_SIZE_MAX	0x400000 /* 4*1024*1024 = 4M */
#define HIFI_OM_FILE_BUFFER_SIZE_MAX	(1024)
#define HIFI_SEC_MAX_NUM 100
#define VOICE_BIGDATA_NOISESIZE     16
#define VOICE_BIGDATA_VOICESIZE     16
#define VOICE_BIGDATA_CHARACTSIZE   32
#define VOICE_BIGDATA_NOISE_VOICE_SIZE  (VOICE_BIGDATA_NOISESIZE+VOICE_BIGDATA_VOICESIZE)
#define AUXHEAR_BIGDATA_MODE       1
#define AUXHEAR_BIGDATA_DEVICE     4
#define AUXHEAR_BIGDATA_ERROR      2

typedef enum {
	DUMP_DSP_LOG,
	DUMP_DSP_BIN
}DUMP_DSP_TYPE;

typedef enum {
	DSP_NORMAL,
	DSP_PANIC,
	DSP_LOG_BUF_FULL
}DSP_ERROR_TYPE;

typedef enum {
	NORMAL_LOG = 0,
	NORMAL_BIN,
	PANIC_LOG,
	PANIC_BIN,
	OCRAM_BIN,
	TCM_BIN
}DUMP_DSP_INDEX;

enum HIFI_CPU_LOAD_INFO_ENUM
{
	HIFI_CPU_LOAD_REPORT   = 0,              /* 定时上报cpu频率 */
	HIFI_CPU_LOAD_VOTE_UP,                   /* 投票ddr上调 */
	HIFI_CPU_LOAD_VOTE_DOWN,                 /* 投票ddr下调 */
	HIFI_CPU_LOAD_LACK_PERFORMANCE,          /* 性能不足 */
	HIFI_CPU_LOAD_INFO_BUTT
};

enum HIFI_CPU_OM_INFO_ENUM
{
	HIFI_CPU_OM_LOAD_INFO= 0,     /*DDR 调时hifi发消息类型*/
	HIFI_CPU_OM_ALGO_MCPS_INFO,   /*DDR调频到顶或播放卡顿hifi发送算法占用mcps信息*/
	HIFI_CPU_OM_UPDATE_BUFF_DELAY_INFO, /* audio pcm play or capture update buff delay */
	HIFI_CPU_OM_DUMP_LOG_INFO,
	HIFI_CPU_OM_INFO_BUTT
};

enum HIFI_VOICE_3A_INFO_ENUM
{
	HIFI_3A_INFO_MSG= 0,
	HIFI_3A_INFO_MSG_BUTT
};

enum EFFECT_ALGO_ENUM                          /*算法列表*/
{
	ID_EFFECT_ALGO_START                =0,
	ID_EFFECT_ALGO_FORMATER,
	ID_EFFECT_ALGO_FORTE_VOICE_SPKOUT,
	ID_EFFECT_ALGO_FORTE_VOICE_MICIN,
	ID_EFFECT_ALGO_FORTE_VOICE_SPKOUT_BWE,
	ID_EFFECT_ALGO_FORTE_VOIP_MICIN,
	ID_EFFECT_ALGO_FORTE_VOIP_SPKOUT,
	ID_EFFECT_ALGO_IN_CONVERT_I2S_GENERAL,
	ID_EFFECT_ALGO_IN_CONVERT_I2S_HI363X,
	ID_EFFECT_ALGO_INTERLACE,
	ID_EFFECT_ALGO_OUT_CONVERT_I2S_GENERAL,
	ID_EFFECT_ALGO_OUT_CONVERT_I2S_HI363X,
	ID_EFFECT_ALGO_SWAP,
	ID_EFFECT_ALGO_IMEDIA_WNR_MICIN,
	ID_EFFECT_ALGO_IMEDIA_WNR_SPKOUT,
	ID_EFFECT_ALGO_SWS_INTERFACE,
	ID_EFFECT_ALGO_DTS,
	ID_EFFECT_ALGO_DRE,
	ID_EFFECT_ALGO_CHC,
	ID_EFFECT_ALGO_SRC,
	ID_EFFECT_ALGO_TTY,
	ID_EFFECT_ALGO_KARAOKE_RECORD,
	ID_EFFECT_ALGO_KARAOKE_PLAY,
	ID_EFFECT_ALGO_MLIB_CS_VOICE_CALL_MICIN,
	ID_EFFECT_ALGO_MLIB_CS_VOICE_CALL_SPKOUT,
	ID_EFFECT_ALGO_MLIB_VOIP_CALL_MICIN,
	ID_EFFECT_ALGO_MLIB_VOIP_CALL_SPKOUT,
	ID_EFFECT_ALGO_MLIB_AUDIO_PLAY,
	ID_EFFECT_ALGO_MLIB_AUDIO_RECORD,
	ID_EFFECT_ALGO_MLIB_SIRI_MICIN,
	ID_EFFECT_ALGO_MLIB_SIRI_SPKOUT,
	ID_EFFECT_ALGO_EQ,
	ID_EFFECT_ALGO_MBDRC6402,
	ID_EFFECT_ALGO_IMEDIA_VOIP_MICIN,
	ID_EFFECT_ALGO_IMEDIA_VOIP_SPKOUT,
	ID_EFFECT_ALGO_IMEDIA_VOICE_CALL_MICIN,
	ID_EFFECT_ALGO_IMEDIA_VOICE_CALL_SPKOUT,
	ID_EFFECT_ALGO_IMEDIA_VOICE_CALL_SPKOUT_BWE,
	ID_EFFECT_ALGO_BUTT
};

enum EFFECT_STREAM_ID{
	AUDIO_STREAM_PCM_OUTPUT         = 0,
	AUDIO_STREAM_PLAYER_OUTPUT,
	AUDIO_STREAM_DIRECT_OUTPUT,
	AUDIO_STREAM_LOWLATENCY_OUTPUT,
	AUDIO_STREAM_MMAP_OUTPUT,
	AUDIO_STREAM_MIXER_OUTPUT,
	AUDIO_STREAM_VOICE_OUTPUT,
	AUDIO_STREAM_VOICEPP_OUTPUT,
	AUDIO_STREAM_OUTPUT_CNT,

	AUDIO_STREAM_PCM_INPUT          = 0x10,
	AUDIO_STREAM_LOWLATENCY_INPUT,
	AUDIO_STREAM_MMAP_INPUT,
	AUDIO_STREAM_VOICE_INPUT,
	AUDIO_STREAM_VOICEPP_INPUT,
	AUDIO_STREAM_INPUT_CNT,
};

enum DRV_HIFI_IMAGE_SEC_LOAD_ENUM {
	DRV_HIFI_IMAGE_SEC_LOAD_STATIC = 0,
	DRV_HIFI_IMAGE_SEC_LOAD_DYNAMIC,
	DRV_HIFI_IMAGE_SEC_UNLOAD,
	DRV_HIFI_IMAGE_SEC_UNINIT,
	DRV_HIFI_IMAGE_SEC_LOAD_BUTT,
};
typedef unsigned char DRV_HIFI_IMAGE_SEC_LOAD_ENUM_UINT8;

enum DRV_HIFI_IMAGE_SEC_TYPE_ENUM {
	DRV_HIFI_IMAGE_SEC_TYPE_CODE = 0,
	DRV_HIFI_IMAGE_SEC_TYPE_DATA,
	DRV_HIFI_IMAGE_SEC_TYPE_BSS,
	DRV_HIFI_IMAGE_SEC_TYPE_BUTT,
};
typedef unsigned char DRV_HIFI_IMAGE_SEC_TYPE_ENUM_UINT8;

enum MLIB_DEVICE_ENUM
{
	MLIB_DEVICE_HANDSET = 0,              /* handset mode */
	MLIB_DEVICE_HANDFREE,                 /* handfree mode */
	MLIB_DEVICE_CARFREE,                  /* carfree mode */
	MLIB_DEVICE_HEADSET,                  /* headset mode */
	MLIB_DEVICE_BLUETOOTH,                /* buletooth mode */
	MLIB_DEVICE_PCVOICE,                  /* PC-VOICE mode */
	MLIB_DEVICE_HEADPHONE,                /* Three segment headphone mode */
	MLIB_DEVICE_USBVOICE,                 /* VR USB VOICE call*/
	MLIB_DEVICE_USBHEADSET,               /* TypeC HIFI USB VOICE call (UP & DOWN LINK)*/
	MLIB_DEVICE_USBHEADPHONE,             /* TypeC HIFI USB VOICE call (JUST DOWN LINK)*/
	MLIB_DEVICE_BUTT,
};

enum audio_hook_msg_type {
	AUDIO_HOOK_MSG_START = 0,
	AUDIO_HOOK_MSG_STOP = 1,
	AUDIO_HOOK_MSG_DATA_TRANS = 2,
	AUDIO_HOOK_MSG_BUTT
};

struct drv_hifi_image_sec {
	unsigned short sn;
	DRV_HIFI_IMAGE_SEC_TYPE_ENUM_UINT8 type;
	DRV_HIFI_IMAGE_SEC_LOAD_ENUM_UINT8 load_attib;
	unsigned int src_offset;
	unsigned int des_addr;
	unsigned int size;
};

struct drv_hifi_image_head {
	char time_stamp[24];
	unsigned int image_size;
	unsigned int sections_num;
	struct drv_hifi_image_sec sections[HIFI_SEC_MAX_NUM];
};

struct image_partition_table{
	unsigned long phy_addr_start;
	unsigned long phy_addr_end;
	unsigned int size;
	unsigned long remap_addr;
};

struct hifi_om_s {
	struct task_struct* kdumpdsp_task;
	struct semaphore	dsp_dump_sema;

	unsigned int	debug_level;
	unsigned int	dsp_debug_level;
	unsigned int*	dsp_debug_level_addr;

	unsigned int	pre_dsp_dump_timestamp;
	unsigned int*	dsp_time_stamp;
	unsigned int	pre_exception_no;
	unsigned int*	dsp_exception_no;

	unsigned int*	dsp_panic_mark;

	unsigned int*	dsp_log_cur_addr;
	char*			dsp_log_addr;
	char*			dsp_bin_addr;
	char			cur_dump_time[HIFI_DUMP_FILE_NAME_MAX_LEN];
	bool			first_dump_log;
	bool			force_dump_log;
	DSP_ERROR_TYPE  dsp_error_type;

	bool			dsp_loaded;
	bool			reset_system;
	unsigned int	dsp_loaded_sign;

	unsigned int*	dsp_debug_kill_addr;
	unsigned int*	dsp_stack_addr;
	unsigned int*	dsp_loaded_indicate_addr;
	struct drv_fama_config *dsp_fama_config;

	struct device	*dev;

};

struct hifi_dsp_dump_info{
	DSP_ERROR_TYPE error_type;
	DUMP_DSP_TYPE dump_type;
	char* file_name;
	char* data_addr;
	unsigned int data_len;
};

struct hifi_data_hook_ctrl_info {
	char file_path[HIFI_DUMP_FILE_NAME_MAX_LEN];
	uint32_t hook_data_enable;
	unsigned char *priv_data_base;
};

typedef struct
{
	unsigned short	effect_stream_id;
	unsigned short	effect_algo_id;
	unsigned int	effect_algo_mcps;
}hifi_effect_mcps_stru;

typedef struct
{
	unsigned int	cpu_load;
	unsigned int	avg_cpu_Load;
	unsigned int	ddr_freq;
}hifi_cpu_load_info_stru;

struct hifi_om_load_info_stru
{
	unsigned int	recv_msg_type;            /* CPU负载上报消息类型 */
	hifi_cpu_load_info_stru	cpu_load_info;
	unsigned int	info_type;
	unsigned int	report_interval;
};

struct hifi_om_effect_mcps_stru
{
	unsigned int	recv_msg_type;
	hifi_cpu_load_info_stru	cpu_load_info;
	hifi_effect_mcps_stru	effect_mcps_info[DSP_DUMP_MAX_EFFECTS_CNT];
};

struct hifi_om_update_buff_delay_info
{
	unsigned int	recv_msg_type;
	unsigned short	pcm_mode;
	unsigned short	pcm_device;
};

struct hifi_om_dump_log_info
{
	unsigned int	recv_msg_type;
	unsigned int	value;
};

struct voice_3a_om_stru
{
	unsigned int	recv_msg_type;
	unsigned short	recv_msg;
	unsigned short	nvid;
	unsigned short	data1;
	unsigned short	data2;
};

typedef struct {
	unsigned char	iMedia_Voice_bigdata_device :4;
	unsigned char	iMedia_Voice_bigdata_flag:4;
	unsigned char	iMedia_Voice_bigdata_noise  :4;
	unsigned char	iMedia_Voice_bigdata_voice  :4;
	unsigned char	iMedia_Voice_bigdata_miccheck;
	unsigned char	iMedia_Voice_bigdata_reserve2;
	unsigned int	iMedia_Voice_bigdata_charact ;
} imedia_voice_bigdata;

typedef struct {
	char			imedia_auxhear_mode;
	short			imedia_auxhear_error;
	unsigned int	imedia_auxhear_device;
} imedia_auxhear_bigdata;

typedef struct {
	char	noise[VOICE_BIGDATA_NOISESIZE];              /*0-15 noise level*/
	char	voice[VOICE_BIGDATA_VOICESIZE];              /*0-15 voice level*/
	char	charact[VOICE_BIGDATA_CHARACTSIZE];         /*32 voice charact, such as whisper, ave and so on*/
} imedia_voice_bigdata_to_imonitor;

typedef struct {
	char	mode[AUXHEAR_BIGDATA_MODE];
	char	device[AUXHEAR_BIGDATA_DEVICE];
	char	error[AUXHEAR_BIGDATA_ERROR];
} imedia_auxhear_bigdata_to_imonitor;

extern struct hifi_om_s g_om_data;

typedef struct {
	char level_char;
	unsigned int level_num;
} debug_level_com;

typedef struct _hifi_str_cmd {
	unsigned short msg_id;
	unsigned short str_len; /*length of string, include \0 */
	char           str[0];
} hifi_str_cmd;

struct hifi_effect_info_stru {
	unsigned int	effect_id;
	char		effect_name[64];
};

enum hifi_om_work_id {
	HIFI_OM_WORK_VOICE_BSD = 0,
	HIFI_OM_WORK_AUDIO_OM_DETECTION,
	HIFI_OM_WORK_VOICE_3A,
	HIFI_OM_WORK_VOICE_BIGDATA,
	HIFI_OM_WORK_SMARTPA_DFT,
	HIFI_OM_WORK_AUDIO_DB,
	HIFI_OM_WORK_AUXHEAR_BIGDATA,
#ifdef ENABLE_HIFI_DEBUG
	HIFI_OM_WORK_DATA_HOOK,
#endif
	HIFI_OM_WORK_MAX,
};

struct hifi_om_work_ctl {
	struct workqueue_struct *wq;
	struct work_struct work;
	spinlock_t lock;
	struct list_head list;
};

struct hifi_om_work_info {
	int work_id;
	char *work_name;
	work_func_t func;
	struct hifi_om_work_ctl ctl;
};

struct hifi_om_ap_data {
	unsigned short msg_id;
	unsigned int data_len;
	unsigned char data[0];
};

struct hifi_om_work {
	struct list_head om_node;
	unsigned int data_len;
	unsigned char data[0];
};

struct hifi_om_send_hook_to_ap {
	uint32_t msg_type;
	uint32_t data_pos;
	uint32_t data_addr;
	uint32_t data_length;
};

#define HIFI_STAMP (unsigned int)readl(g_om_data.dsp_time_stamp)

#ifdef ENABLE_HIFI_DEBUG
#define can_reset_system() \
do {\
	if (g_om_data.reset_system) {\
		printk("soc hifi reset, reset all system by reset option");\
		BUG_ON(true);\
	}\
} while(0);

#else
#define can_reset_system()
#endif


#define logd(fmt, ...) /*lint !e683 */ \
do {\
	if (g_om_data.debug_level >= 3) {\
		printk(LOG_TAG"[D][%u]:%s:%d: "fmt, HIFI_STAMP, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	}\
} while(0);

#define logi(fmt, ...) /*lint !e683 */ \
do {\
	if (g_om_data.debug_level >= 2) {\
		printk(LOG_TAG"[I][%u]:%s:%d: "fmt, HIFI_STAMP, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	}\
} while(0);


#define logw(fmt, ...) \
do {\
	if (g_om_data.debug_level >= 1) {\
		printk(LOG_TAG"[W][%u]:%s:%d: "fmt, HIFI_STAMP, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	}\
} while(0);

#define loge(fmt, ...) /*lint !e683 */ \
do {\
		printk(LOG_TAG"[E][%u]:%s:%d: "fmt, HIFI_STAMP, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
} while(0);

#define IN_FUNCTION   logd("begin.\n");
#define OUT_FUNCTION  logd("end.\n");

void hifi_om_init(struct platform_device *dev, unsigned char* hifi_priv_base_virt, unsigned char* hifi_priv_base_phy);
void hifi_om_deinit(struct platform_device *dev);

int hifi_dsp_dump_hifi(const void __user *arg);
void hifi_dump_panic_log(void);

bool is_hifi_loaded(void);

void hifi_om_effect_mcps_info_show(struct hifi_om_effect_mcps_stru *hifi_mcps_info);
void hifi_om_cpu_load_info_show(struct hifi_om_load_info_stru *hifi_om_info);
void hifi_om_update_buff_delay_info_show(struct hifi_om_update_buff_delay_info *info);

int hifi_get_dmesg(const void __user *arg);
int hifi_om_get_voice_bsd_param(const void __user * uaddr);
void hifi_om_rev_data_handle(int type, const unsigned char *addr, unsigned int len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

