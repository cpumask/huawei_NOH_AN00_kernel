
#ifndef __VENC_REGULATOR_H__
#define __VENC_REGULATOR_H__

#include "drv_common.h"

#define MAX_NAME_LEN 20
#define VENC_CLK_RATE         "enc_clk_rate"
#define VENC_CLOCK_NAME       "clk_venc"
#define VENC_REGULATOR_NAME   "ldo_venc"
#define MEDIA_REGULATOR_NAME  "ldo_media"

/* clock */
struct venc_clock {
	struct clk *venc_clk;
	venc_clk_t curr_clk_type;
};

/* regulator */
struct venc_regulator {
	struct regulator *media_regulator;
	struct regulator *venc_regulator[MAX_SUPPORT_CORE_NUM];
};

/* config */
struct venc_config_common {
	HI_U32 core_num;
	HI_U32 fpga_flag;
	HI_U32 qos_mode;
	HI_U32 clk_rate[VENC_CLK_BUTT];
	HI_U32 power_off_clk_rate;
	HI_U32 transit_clk_rate;
	HI_U32 valid_core_id; /* only use in m536 signle core board */
	bool single_core_mode; /* only use in m536 signle core board */
};

struct venc_config_priv {
	HI_U32 reg_base_addr;
	HI_U32 reg_range;
};

struct venc_config {
	struct venc_config_common venc_conf_com;
	struct venc_config_priv venc_conf_priv[MAX_SUPPORT_CORE_NUM];
#ifdef VENC_DPM_ENABLE
	struct venc_config_priv venc_conf_dpm[MAX_INNER_MODULE];
#endif
};

#ifdef SUPPORT_VENC_FREQ_CHANGE
extern HI_U32 g_venc_freq;
extern struct mutex g_venc_freq_mutex;
#endif

HI_S32 get_dts_config_info(struct platform_device *pdev);
HI_S32 get_regulator_info(struct platform_device *pdev);
HI_S32  venc_regulator_enable(void);
HI_S32  venc_regulator_disable(void);
HI_S32  venc_regulator_select_idle_core(vedu_osal_event_t *event);
HI_S32  venc_regulator_wait_hardware_idle(vedu_osal_event_t *event);
HI_S32  venc_regulator_update(const struct clock_info *clock_info);
HI_S32  venc_regulator_reset(void);
HI_BOOL venc_regulator_is_fpga(void);
HI_U64  venc_get_smmu_ttb(HI_VOID);
HI_BOOL is_support_power_control_per_frame(HI_VOID);
HI_U32 venc_get_reg_base_addr(HI_S32 core_id);

#endif

