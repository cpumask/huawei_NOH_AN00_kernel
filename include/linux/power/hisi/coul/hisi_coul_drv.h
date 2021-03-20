/*
 *
 *
 * Copyright (C) 2013 Huawei Inc.
 */


#ifndef __HISI_COUL_DRV_H
#define __HISI_COUL_DRV_H

#define	BAT_VOL_3500						(3500)
#define	BAT_VOL_3600						(3600)
#define	BAT_VOL_3700						(3700)
#define	BAT_VOL_3800						(3800)
#define	BAT_VOL_3900						(3900)

#define BASP_TAG "[BASP] " /* Battery Aging Safe Policy LOG TAG */

#define VTERM_MAX_DEFAULT_MV (4400)
#define MAX_BATT_CHARGE_CUR_RATIO (70) /* 0.7C */
#define COUL_IC_GOOD 0
#define COUL_IC_BAD   1

#define COMPENSATION_PARA_LEVEL   8
#define COMPENSATION_THRESHOLD    200 /* 20c */

#define INVALID_TEMP_VAL                (-999)
#define INVALID_VOLTAGE_VAL             (-999)
#define BATTERY_NORMAL_CUTOFF_VOL 3150
#define DEFAULT_RPCB 10000 /*uohm*/
#define POLAR_CALC_INTERVAL (200) /*ms*/
#define TEMP_POLAR_REPORT     (5)
#define CURR_AVG_DEFAULT      (19999)//MA
/* ma = ua/1000 = uas/s/1000 = uah*3600/s/1000 = uah*18/(s*5) */
#define CC_UAS2MA(cc,time) (((cc) * 18) / ((time) * 5))
enum HISI_COULOMETER_TYPE {
	COUL_HISI = 0,
	COUL_BQ27510,
	COUL_BQ28Z610,
	COUL_UNKNOW,
};

/* ntc_temp_compensation_para*/
enum ntc_temp_compensation_para_info {
	NTC_COMPENSATION_PARA_ICHG = 0,
	NTC_COMPENSATION_PARA_VALUE,
	NTC_COMPENSATION_PARA_TOTAL,
};
struct ntc_temp_compensation_para_data {
	int ntc_compensation_ichg;
	int ntc_compensation_value;
};

struct polar_calc_info {
	long vol;
	int curr_5s;
	int curr_peak;
	int ocv;
	int ocv_old;
	int ori_vol;
	int ori_cur;
	int err_a;
	int sr_polar_vol1;
	int sr_polar_vol0;
	int sr_polar_err_a;
	int polar_ocv;
	int polar_ocv_time;
	int last_calc_temp;
};

struct bat_ocv_info  {
	unsigned int eco_vbat_reg;
	unsigned int eco_ibat_reg;
	int last_sample_time;
	int now_sample_time;
	s64 eco_cc_reg;
};

struct hisi_coul_ops {
	int (*is_coul_ready)(void);
	int (*is_battery_exist)(void);
	int (*is_battery_reach_threshold)(void);
	char *(*battery_brand)(void);
	int (* battery_id_voltage)(void);
	int (*battery_voltage)(void);
	int (*battery_voltage_uv)(void);
	int (*battery_current)(void);
	int (*battery_resistance)(void);
	int (*fifo_avg_current)(void);
	int (*battery_current_avg)(void);
	int (*battery_unfiltered_capacity)(void);
	int (*battery_capacity)(void);
	int (*battery_temperature)(void);
	int (*chip_temperature)(void);
	int (*battery_temperature_for_charger)(void);
	int (*battery_rm)(void);
	int (*battery_fcc)(void);
	int (*battery_fcc_design)(void);
	int (*battery_tte)(void);
	int (*battery_ttf)(void);
	int (*battery_health)(void);
	int (*battery_capacity_level)(void);
	int (*battery_technology)(void);
	struct chrg_para_lut *(*battery_charge_params)(void);
	int (*battery_vbat_max)(void);
	int (*battery_ifull)(void);
	int (*charger_event_rcv)(unsigned int);
	int (*battery_cycle_count)(void);
	int (*get_battery_limit_fcc)(void);
	int (*coul_is_fcc_debounce)(void);
	int (*set_hltherm_flag)(int);
	int (*get_hltherm_flag)(void);
	int (*dev_check)(void);
	int (*get_soc_vary_flag)(int monitor_flag, int *deta_soc);
	int (*coul_low_temp_opt)(void);
	int (*battery_cc)(void);
	int (*battery_fifo_curr)(unsigned int);
	int (*battery_fifo_depth)(void);
	int (*battery_ufcapacity_tenth)(void);
	int (*convert_regval2ua)(unsigned int reg_val);
	int (*convert_regval2uv)(unsigned int reg_val);
	int (*convert_regval2temp)(unsigned int reg_val);
	int (*convert_mv2regval)(int vol_mv);
	int (*cal_uah_by_ocv)(int ocv_uv, int *ocv_soc_uAh);
	int (*convert_temp_to_adc)(int temp);
	int (*get_coul_calibration_status)(void);
	int (*battery_removed_before_boot)(void);
	int (*get_qmax)(void);
	int (*get_ocv_by_soc)(int temp, int soc);
	int (*get_soc_by_ocv)(int temp, int ocv);
	int (*get_ocv)(void);
	int (*get_polar_avg)(void);
	int (*get_polar_peak)(void);
	int (*update_basp_policy)(unsigned int level,
				  unsigned int nondc_volt_dec);
	int (*get_record_fcc)(unsigned int size, unsigned int *records);
	int (*get_last_powerdown_soc)(void);
	int (*get_battery_temperature_raw)(void);
	int (*get_battery_temp_permille)(void);
	void (*get_c_calibration)(int *cur_offset_a, int *cur_offset_b);
};

extern enum HISI_COULOMETER_TYPE hisi_coulometer_type(void);
extern void hisi_coul_charger_event_rcv(unsigned int event);
extern int is_hisi_coul_ready(void);
extern int is_hisi_battery_exist(void);
extern int is_hisi_battery_reach_threshold(void);
extern int hisi_battery_voltage(void);
extern char* hisi_battery_brand(void);
extern int hisi_battery_id_voltage(void);
extern int hisi_battery_voltage_uv(void);
extern int hisi_battery_current(void);
extern int hisi_battery_resistance(void);
extern int hisi_get_coul_calibration_status(void);
extern int hisi_coul_fifo_avg_current(void);
extern int hisi_battery_current_avg(void);
extern int hisi_battery_unfiltered_capacity(void);
extern int hisi_battery_capacity(void);
extern int hisi_battery_temperature(void);
extern int hisi_battery_rm(void);
extern int hisi_battery_fcc(void);
extern int hisi_battery_fcc_design(void);
extern int hisi_battery_tte(void);
extern int hisi_battery_ttf(void);
extern int hisi_battery_health(void);
extern int hisi_battery_capacity_level(void);
extern int hisi_battery_technology(void);
extern struct chrg_para_lut *hisi_battery_charge_params(void);
extern int hisi_battery_ifull(void);
extern int hisi_battery_vbat_max(void);
extern int hisi_battery_cycle_count(void);
extern int hisi_battery_get_limit_fcc(void);
extern int hisi_coul_ops_register(struct hisi_coul_ops *coul_ops, enum HISI_COULOMETER_TYPE coul_type);
extern int hisi_coul_ops_unregister(struct hisi_coul_ops *coul_ops);
extern int hisi_power_supply_voltage(void);
extern int is_hisi_fcc_debounce(void);
extern int hisi_coul_set_hltherm_flag(int temp_flag);
extern int hisi_coul_get_hltherm_flag(void);
extern int hisi_battery_soc_vary_flag(int monitor_flag, int *deta_soc);
extern int hisi_battery_temperature_for_charger(void);
extern int hisi_coul_low_temp_opt(void);
extern int hisi_battery_cc(void);
extern int hisi_coul_battery_fifo_depth(void);
extern int hisi_coul_battery_ufcapacity_tenth(void);
extern int hisi_coul_battery_fifo_curr(unsigned int index);
extern int hisi_coul_convert_regval2ua(unsigned int reg_val);
extern int hisi_coul_convert_regval2uv(unsigned int reg_val);
extern int hisi_coul_convert_regval2temp(unsigned int reg_val);
extern int hisi_coul_convert_mv2regval(int vol_mv);
extern int hisi_coul_cal_uah_by_ocv(int ocv_uv, int *ocv_soc_uAh);
extern int hisi_coul_convert_temp_to_adc(int temp);
extern int hisi_coul_chip_temperature(void);
extern int hisi_battery_cc_uah(void);
extern int hisi_battery_removed_before_boot(void);
extern int hisi_battery_get_qmax(void);
int hisi_battery_temperature_raw(int *raw_temp);
int hisi_battery_temperature_permille(int *raw_temp);
extern int hisi_coul_get_ocv_by_soc(int temp, int soc);
extern int hisi_coul_get_soc_by_ocv(int temp, int ocv);
extern int hisi_coul_get_ocv(void);

extern int hisi_get_polar_avg(void);
extern int hisi_get_polar_peak(void);
extern int hisi_battery_update_basp_policy(unsigned int level,
		unsigned int nondc_volt_dec);
extern int hisi_battery_get_recrod_fcc(unsigned int size,
				       unsigned int *records);
extern int hisi_coul_get_last_powerdown_soc(void);
int hisi_get_cur_calibration(int *cur_offset_a, int *cur_offset_b);
/* depend on coul cmdline add */
extern unsigned long nv_info_addr;
#endif
