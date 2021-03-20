#ifndef __HISI_NPU_PM_H__
#define __HISI_NPU_PM_H__


int hisi_npu_power_on(void);
int hisi_npu_power_off(void);

/* return mv */
int hisi_npu_get_voltage(void);

#endif /* __HISI_NPU_PM_H__ */
