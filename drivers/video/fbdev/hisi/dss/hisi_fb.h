/* Copyright (c) 2013-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef HISI_FB_H
#define HISI_FB_H

#include "hisi_fb_struct.h"
/*
 * FUNCTIONS PROTOTYPES
 */
extern int g_primary_lcd_xres;
extern int g_primary_lcd_yres;
extern uint64_t g_pxl_clk_rate;

extern uint8_t g_prefix_ce_support;
extern uint8_t g_prefix_sharpness1d_support;
extern uint8_t g_prefix_sharpness2d_support;

extern int delta_bl_delayed;
extern bool blc_enable_delayed;
extern bool dc_switch_xcc_updated;

extern uint32_t g_online_cmdlist_idxs;
extern uint32_t g_offline_cmdlist_idxs;

extern uint32_t g_fpga_flag;
extern uint32_t g_fastboot_enable_flag;
extern uint32_t g_fake_lcd_flag;
extern uint32_t g_dss_version_tag;
extern uint32_t g_dss_module_resource_initialized;
extern uint32_t g_logo_buffer_base;
extern uint32_t g_logo_buffer_size;
extern uint32_t g_underflow_stop_perf_stat;
extern uint32_t g_mipi_dphy_version; /* C30:0 C50:1 */
extern uint32_t g_mipi_dphy_opt;
extern uint32_t g_chip_id;

extern uint32_t g_fastboot_already_set;

extern int g_debug_online_vactive;
void de_open_update_te(void);
void de_close_update_te(struct work_struct *work);
extern struct fb_info *fbi_list[HISI_FB_MAX_FBI_LIST];
extern struct hisi_fb_data_type *hisifd_list[HISI_FB_MAX_FBI_LIST];
extern struct semaphore g_hisifb_dss_clk_vote_sem;
extern struct mutex g_rgbw_lock;

#if defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600) || \
	defined(CONFIG_HISI_FB_V350) || defined(CONFIG_HISI_FB_V345) || \
	defined(CONFIG_HISI_FB_V346)
extern uint32_t g_post_xcc_table_temp[12];
extern u32 g_gmp_lut_table_low32bit_init[4913];
extern u32 g_gmp_lut_table_high4bit_init[4913];
#endif

#ifdef CONFIG_HISI_FB_V600
extern char __iomem *hisifd_soc_acpu_ao_ioc_base;
#endif
#ifdef CONFIG_REPORT_VSYNC
extern void mali_kbase_pm_report_vsync(int);
#endif
extern int mipi_dsi_ulps_cfg(struct hisi_fb_data_type *hisifd, int enable);
extern bool hisi_dss_check_reg_reload_status(struct hisi_fb_data_type *hisifd);
unsigned int get_boot_into_recovery_flag(void);
uint32_t get_panel_xres(struct hisi_fb_data_type *hisifd);
uint32_t get_panel_yres(struct hisi_fb_data_type *hisifd);

bool is_fastboot_display_enable(void);
bool is_dss_idle_enable(void);

/* mediacomm channel manager */
int hisi_mdc_resource_init(struct hisi_fb_data_type *hisifd, unsigned int platform);
int hisi_mdc_chn_request(struct fb_info *info, void __user *argp);
int hisi_mdc_chn_release(struct fb_info *info, const void __user *argp);

/* dss secure */
void hisifb_atf_config_security(uint32_t master_op_type, uint32_t channel, uint32_t mode);
void hisifb_secure_register(struct platform_device *pdev);
void hisifb_secure_unregister(struct platform_device *pdev);
int hisi_fb_blank_sub(int blank_mode, struct fb_info *info);

/* backlight */
void hisifb_backlight_update(struct hisi_fb_data_type *hisifd);
void hisifb_backlight_cancel(struct hisi_fb_data_type *hisifd);
void hisifb_backlight_register(struct platform_device *pdev);
void hisifb_backlight_unregister(struct platform_device *pdev);
void hisifb_set_backlight(struct hisi_fb_data_type *hisifd, uint32_t bkl_lvl, bool enforce);
int update_cabc_pwm(struct hisi_fb_data_type *hisifd);

/* backlight flicker detector */
void bl_flicker_detector_init(display_engine_flicker_detector_config_t config);
void bl_flicker_detector_collect_upper_bl(int level);
void bl_flicker_detector_collect_algo_delta_bl(int level);
void bl_flicker_detector_collect_device_bl(int level);

/* vsync */
void hisifb_frame_updated(struct hisi_fb_data_type *hisifd);
void hisifb_set_vsync_activate_state(struct hisi_fb_data_type *hisifd, bool infinite);
void hisifb_activate_vsync(struct hisi_fb_data_type *hisifd);
void hisifb_deactivate_vsync(struct hisi_fb_data_type *hisifd);
int hisifb_vsync_ctrl(struct fb_info *info, const void __user *argp);
int hisifb_vsync_resume(struct hisi_fb_data_type *hisifd);
int hisifb_vsync_suspend(struct hisi_fb_data_type *hisifd);
void hisifb_vsync_isr_handler(struct hisi_fb_data_type *hisifd);
void hisifb_vsync_register(struct platform_device *pdev);
void hisifb_vsync_unregister(struct platform_device *pdev);
void hisifb_vsync_disable_enter_idle(struct hisi_fb_data_type *hisifd, bool disable);
void hisifb_video_idle_ctrl_register(struct platform_device *pdev);
void hisifb_video_idle_ctrl_unregister(struct platform_device *pdev);
void hisifb_esd_register(struct platform_device *pdev);
void hisifb_esd_unregister(struct platform_device *pdev);
void hisifb_masklayer_backlight_flag_config(struct hisi_fb_data_type *hisifd,
	bool masklayer_backlight_flag);

/* buffer sync */
int hisifb_layerbuf_lock(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, struct list_head *lock_list);
void hisifb_layerbuf_flush(struct hisi_fb_data_type *hisifd,
	struct list_head *lock_list);
void hisifb_layerbuf_unlock(struct hisi_fb_data_type *hisifd,
	struct list_head *pfree_list);
void hisifb_layerbuf_lock_exception(struct hisi_fb_data_type *hisifd,
	struct list_head *lock_list);
int hisifb_offline_layerbuf_lock(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, struct list_head *plock_list);

void hisifb_buf_sync_wait_async(struct hisifb_buf_sync *buf_sync);
int hisifb_buf_sync_wait(int fence_fd);
int hisifb_buf_sync_handle(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req);
void hisifb_buf_sync_signal(struct hisi_fb_data_type *hisifd);
void hisifb_buf_sync_suspend(struct hisi_fb_data_type *hisifd);
int hisifb_offline_create_fence(struct hisi_fb_data_type *hisifd);
int hisifb_buf_sync_create_fence(struct hisi_fb_data_type *hisifd, int32_t *release_fence, int32_t *retire_fence);
void hisifb_buf_sync_close_fence(int32_t *release_fence, int32_t *retire_fence);
void hisifb_buf_sync_register(struct platform_device *pdev);
void hisifb_buf_sync_unregister(struct platform_device *pdev);

/* control */
int hisifb_ctrl_fastboot(struct hisi_fb_data_type *hisifd);
int hisifb_ctrl_on(struct hisi_fb_data_type *hisifd);
int hisifb_ctrl_off(struct hisi_fb_data_type *hisifd);
int hisifb_ctrl_lp(struct hisi_fb_data_type *hisifd, bool lp_enter);
int hisifb_ctrl_sbl(struct fb_info *info, uint32_t value);
int hisifb_ctrl_dss_voltage_get(struct fb_info *info, void __user *argp);
int hisifb_ctrl_dss_voltage_set(struct fb_info *info, void __user *argp);
int hisifb_ctrl_dss_vote_cmd_set(struct fb_info *info, const void __user *argp);
int hisifb_fps_upt_isr_handler(struct hisi_fb_data_type *hisifd);
int hisifb_ctrl_esd(struct hisi_fb_data_type *hisifd);
void hisifb_sysfs_attrs_add(struct hisi_fb_data_type *hisifd);
void set_reg(char __iomem *addr, uint32_t val, uint8_t bw, uint8_t bs);
uint32_t set_bits32(uint32_t old_val, uint32_t val, uint8_t bw, uint8_t bs);
void hisifb_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *addr, uint32_t val, uint8_t bw, uint8_t bs);
uint32_t hisifb_line_length(int index, uint32_t xres, int bpp);
void hisifb_get_timestamp(struct timeval *tv);
uint32_t hisifb_timestamp_diff(struct timeval *lasttime, struct timeval *curtime);
void hisifb_save_file(char *filename, const char *buf, uint32_t buf_len);

struct platform_device *hisi_fb_device_alloc(struct hisi_fb_panel_data *pdata,
	uint32_t type, uint32_t id);
struct platform_device *hisi_fb_add_device(struct platform_device *pdev);

#ifdef CONFIG_HUAWEI_OCP
int hisi_lcd_ocp_recover(struct notifier_block *nb,
		unsigned long event, void *data);
#endif

#if CONFIG_SH_AOD_ENABLE
/* sensorhub aod */
bool hisi_sensorhub_aod_hw_lock(struct hisi_fb_data_type *hisifd);
bool hisi_sensorhub_aod_hw_unlock(struct hisi_fb_data_type *hisifd);
int hisi_sensorhub_aod_unblank(uint32_t msg_no);
int hisi_sensorhub_aod_blank(uint32_t msg_no);
bool hisi_fb_request_aod_hw_lock(struct hisi_fb_data_type *hisifd);
void hisi_fb_release_aod_hw_lock(struct hisi_fb_data_type *hisifd, bool locked);
int hisi_fb_aod_blank(struct hisi_fb_data_type *hisifd, int blank_mode);
bool hisi_fb_get_aod_lock_status(void);
bool hisi_aod_get_aod_status(void);
int hisi_send_aod_stop(void);
uint32_t hisi_aod_get_panel_id(void);
bool hisi_aod_need_fast_unblank(void);
void hisi_aod_wait_stop_nolock(void);
#else
static inline bool hisi_fb_request_aod_hw_lock(struct hisi_fb_data_type *hisifd) {return false; };
static inline void hisi_fb_release_aod_hw_lock(struct hisi_fb_data_type *hisifd, bool locked) {};
static inline int hisi_fb_aod_blank(struct hisi_fb_data_type *hisifd, int blank_mode) {return 0; }
static inline bool hisi_fb_get_aod_lock_status(void) { return true; }
static inline bool hisi_aod_get_aod_status(void) {return false; };
static inline int hisi_send_aod_stop(void) {return 0; };
static inline uint32_t hisi_aod_get_panel_id(void) {return 0; };
static inline bool hisi_aod_need_fast_unblank(void) {return false; };
static inline void hisi_aod_wait_stop_nolock(void) {};
#endif
void wait_for_aod_stop(struct hisi_fb_data_type *hisifd);
void wait_for_fast_unblank_end(struct hisi_fb_data_type *hisifd);
uint32_t hisi_get_panel_product_type(void);

int hisi_fb_open_sub(struct fb_info *info);
int hisi_fb_release_sub(struct fb_info *info);

void hisi_fb_frame_refresh(struct hisi_fb_data_type *hisifd, char *trigger);
int hisi_dss_alloc_cmdlist_buffer(struct hisi_fb_data_type *hisifd);
void hisi_dss_free_cmdlist_buffer(struct hisi_fb_data_type *hisifd);
unsigned long hisifb_alloc_fb_buffer(struct hisi_fb_data_type *hisifd);
void hisifb_free_fb_buffer(struct hisi_fb_data_type *hisifd);

int hisifb_create_buffer_client(struct hisi_fb_data_type *hisifd);
void hisifb_destroy_buffer_client(struct hisi_fb_data_type *hisifd);
void hisifb_free_logo_buffer(struct hisi_fb_data_type *hisifd);

void hisi_fb_unblank_wq_handle(struct work_struct *work);
int hisi_fb_blank_device(struct hisi_fb_data_type *hisifd, int blank_mode, struct fb_info *info);
int hisi_fb_blank_panel_power_on(struct hisi_fb_data_type *hisifd);
int hisi_fb_blank_panel_power_off(struct hisi_fb_data_type *hisifd);
int hisi_fb_blank_update_tui_status(struct hisi_fb_data_type *hisifd);
void hisi_fb_pm_runtime_register(struct hisi_fb_data_type *hisifd);
void hisi_fb_fnc_register_base(struct hisi_fb_data_type *hisifd);
void hisi_fb_sdp_fnc_register(struct hisi_fb_data_type *hisifd);
void hisi_fb_mdc_fnc_register(struct hisi_fb_data_type *hisifd);
void hisi_fb_aux_fnc_register(struct hisi_fb_data_type *hisifd);
void hisi_fb_common_register(struct hisi_fb_data_type *hisifd);
void hisi_fb_init_screeninfo_base(struct fb_fix_screeninfo *fix, struct fb_var_screeninfo *var);
bool hisi_fb_img_type_valid(uint32_t fb_imgType);
void hisi_fb_init_sreeninfo_by_img_type(struct fb_fix_screeninfo *fix,
	struct fb_var_screeninfo *var, uint32_t fb_imgType, int *bpp);
void hisi_fb_init_sreeninfo_by_panel_info(struct fb_var_screeninfo *var,
	struct hisi_panel_info *panel_info, uint32_t fb_num, int bpp);
void hisi_fb_data_init(struct hisi_fb_data_type *hisifd);
void hisi_fb_init_sema(struct hisi_fb_data_type *hisifd);
void hisi_fb_init_spin_lock(struct hisi_fb_data_type *hisifd);
int hisi_fb_registe_callback(struct hisi_fb_data_type *hisifd,
	struct fb_var_screeninfo *var, struct hisi_panel_info *panel_info);
void hisi_fb_offlinecomposer_init(struct fb_var_screeninfo *var, struct hisi_panel_info *panel_info);
void hisi_fb_pdp_fnc_register(struct hisi_fb_data_type *hisifd);
int hisifb_check_ldi_porch(struct hisi_panel_info *pinfo);

int bl_lvl_map(int level);

void hisifb_set_panel_power_status(struct hisi_fb_data_type *hisifd, bool power_on);
#endif /* HISI_FB_H */
