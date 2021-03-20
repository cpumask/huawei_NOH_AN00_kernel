#ifndef __SOC_DPMONITOR_INTERFACE_H__
#define __SOC_DPMONITOR_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_DPMONITOR_CTRL_EN_ADDR(base) ((base) + (0x000UL))
#define SOC_DPMONITOR_CFG_TIME_ADDR(base) ((base) + (0x004UL))
#define SOC_DPMONITOR_SENSOR_EN_ADDR(base) ((base) + (0x008UL))
#define SOC_DPMONITOR_SENSOR_DIS_ADDR(base) ((base) + (0x00CUL))
#define SOC_DPMONITOR_DPM_SENSOR_STAT_ADDR(base) ((base) + (0x010UL))
#define SOC_DPMONITOR_BUSY_CNT0_ADDR(base) ((base) + (0x014UL))
#define SOC_DPMONITOR_BUSY_CNT1_ADDR(base) ((base) + (0x018UL))
#define SOC_DPMONITOR_BUSY_CNT2_ADDR(base) ((base) + (0x01CUL))
#define SOC_DPMONITOR_BUSY_CNT3_ADDR(base) ((base) + (0x020UL))
#define SOC_DPMONITOR_BUSY_CNT4_ADDR(base) ((base) + (0x024UL))
#define SOC_DPMONITOR_BUSY_CNT5_ADDR(base) ((base) + (0x028UL))
#define SOC_DPMONITOR_BUSY_CNT6_ADDR(base) ((base) + (0x02CUL))
#define SOC_DPMONITOR_BUSY_CNT7_ADDR(base) ((base) + (0x030UL))
#define SOC_DPMONITOR_BUSY_CNT8_ADDR(base) ((base) + (0x034UL))
#define SOC_DPMONITOR_BUSY_CNT9_ADDR(base) ((base) + (0x038UL))
#define SOC_DPMONITOR_BUSY_CNT10_ADDR(base) ((base) + (0x03CUL))
#define SOC_DPMONITOR_BUSY_CNT11_ADDR(base) ((base) + (0x040UL))
#define SOC_DPMONITOR_BUSY_CNT12_ADDR(base) ((base) + (0x044UL))
#define SOC_DPMONITOR_BUSY_CNT13_ADDR(base) ((base) + (0x048UL))
#define SOC_DPMONITOR_BUSY_CNT14_ADDR(base) ((base) + (0x04CUL))
#define SOC_DPMONITOR_BUSY_CNT15_ADDR(base) ((base) + (0x050UL))
#define SOC_DPMONITOR_ACC_ENERGY_ADDR(base) ((base) + (0x054UL))
#define SOC_DPMONITOR_PCLK_LOAD_ADDR(base) ((base) + (0x058UL))
#define SOC_DPMONITOR_VOLT_FREQ_SEL_ADDR(base) ((base) + (0x05CUL))
#define SOC_DPMONITOR_POWER_PARAM0_ADDR(base) ((base) + (0x060UL))
#define SOC_DPMONITOR_POWER_PARAM1_ADDR(base) ((base) + (0x064UL))
#define SOC_DPMONITOR_POWER_PARAM2_ADDR(base) ((base) + (0x068UL))
#define SOC_DPMONITOR_POWER_PARAM3_ADDR(base) ((base) + (0x06CUL))
#define SOC_DPMONITOR_POWER_PARAM4_ADDR(base) ((base) + (0x070UL))
#define SOC_DPMONITOR_POWER_PARAM5_ADDR(base) ((base) + (0x074UL))
#define SOC_DPMONITOR_POWER_PARAM6_ADDR(base) ((base) + (0x078UL))
#define SOC_DPMONITOR_POWER_PARAM7_ADDR(base) ((base) + (0x07CUL))
#define SOC_DPMONITOR_POWER_PARAM8_ADDR(base) ((base) + (0x080UL))
#define SOC_DPMONITOR_POWER_PARAM9_ADDR(base) ((base) + (0x084UL))
#define SOC_DPMONITOR_POWER_PARAM10_ADDR(base) ((base) + (0x088UL))
#define SOC_DPMONITOR_POWER_PARAM11_ADDR(base) ((base) + (0x08CUL))
#define SOC_DPMONITOR_POWER_PARAM12_ADDR(base) ((base) + (0x090UL))
#define SOC_DPMONITOR_POWER_PARAM13_ADDR(base) ((base) + (0x094UL))
#define SOC_DPMONITOR_POWER_PARAM14_ADDR(base) ((base) + (0x098UL))
#define SOC_DPMONITOR_POWER_PARAM15_ADDR(base) ((base) + (0x09CUL))
#define SOC_DPMONITOR_CONST_POWER_PARAM_ADDR(base) ((base) + (0x0A0UL))
#define SOC_DPMONITOR_VOTE_PARAM0_ADDR(base) ((base) + (0x0A4UL))
#define SOC_DPMONITOR_VOTE_PARAM1_ADDR(base) ((base) + (0x0A8UL))
#define SOC_DPMONITOR_VOTE_PARAM2_ADDR(base) ((base) + (0x0ACUL))
#define SOC_DPMONITOR_VOTE_PARAM3_ADDR(base) ((base) + (0x0B0UL))
#define SOC_DPMONITOR_VOTE_PARAM4_ADDR(base) ((base) + (0x0B4UL))
#define SOC_DPMONITOR_VOTE_PARAM5_ADDR(base) ((base) + (0x0B8UL))
#define SOC_DPMONITOR_VOTE_PARAM6_ADDR(base) ((base) + (0x0BCUL))
#define SOC_DPMONITOR_VOTE_PARAM7_ADDR(base) ((base) + (0x0C0UL))
#define SOC_DPMONITOR_FREQ_PARAM0_ADDR(base) ((base) + (0x0C4UL))
#define SOC_DPMONITOR_FREQ_PARAM1_ADDR(base) ((base) + (0x0C8UL))
#define SOC_DPMONITOR_FREQ_PARAM2_ADDR(base) ((base) + (0x0CCUL))
#define SOC_DPMONITOR_FREQ_PARAM3_ADDR(base) ((base) + (0x0D0UL))
#define SOC_DPMONITOR_FREQ_PARAM4_ADDR(base) ((base) + (0x0D4UL))
#define SOC_DPMONITOR_FREQ_PARAM5_ADDR(base) ((base) + (0x0D8UL))
#define SOC_DPMONITOR_FREQ_PARAM6_ADDR(base) ((base) + (0x0DCUL))
#define SOC_DPMONITOR_FREQ_PARAM7_ADDR(base) ((base) + (0x0E0UL))
#define SOC_DPMONITOR_SIGNAL_LEVEL_MODE_ADDR(base) ((base) + (0x0E4UL))
#define SOC_DPMONITOR_SOFT_PULSE_ADDR(base) ((base) + (0x0E8UL))
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_ADDR(base) ((base) + (0x0ECUL))
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_ADDR(base) ((base) + (0x0F0UL))
#define SOC_DPMONITOR_SIG_INV_EN_ADDR(base) ((base) + (0x0F4UL))
#define SOC_DPMONITOR_CLK_INVERSE_EN_ADDR(base) ((base) + (0x0F8UL))
#define SOC_DPMONITOR_CLK_INVERSE_DIS_ADDR(base) ((base) + (0x0FCUL))
#define SOC_DPMONITOR_CLK_INV_EN_ADDR(base) ((base) + (0x100UL))
#else
#define SOC_DPMONITOR_CTRL_EN_ADDR(base) ((base) + (0x000))
#define SOC_DPMONITOR_CFG_TIME_ADDR(base) ((base) + (0x004))
#define SOC_DPMONITOR_SENSOR_EN_ADDR(base) ((base) + (0x008))
#define SOC_DPMONITOR_SENSOR_DIS_ADDR(base) ((base) + (0x00C))
#define SOC_DPMONITOR_DPM_SENSOR_STAT_ADDR(base) ((base) + (0x010))
#define SOC_DPMONITOR_BUSY_CNT0_ADDR(base) ((base) + (0x014))
#define SOC_DPMONITOR_BUSY_CNT1_ADDR(base) ((base) + (0x018))
#define SOC_DPMONITOR_BUSY_CNT2_ADDR(base) ((base) + (0x01C))
#define SOC_DPMONITOR_BUSY_CNT3_ADDR(base) ((base) + (0x020))
#define SOC_DPMONITOR_BUSY_CNT4_ADDR(base) ((base) + (0x024))
#define SOC_DPMONITOR_BUSY_CNT5_ADDR(base) ((base) + (0x028))
#define SOC_DPMONITOR_BUSY_CNT6_ADDR(base) ((base) + (0x02C))
#define SOC_DPMONITOR_BUSY_CNT7_ADDR(base) ((base) + (0x030))
#define SOC_DPMONITOR_BUSY_CNT8_ADDR(base) ((base) + (0x034))
#define SOC_DPMONITOR_BUSY_CNT9_ADDR(base) ((base) + (0x038))
#define SOC_DPMONITOR_BUSY_CNT10_ADDR(base) ((base) + (0x03C))
#define SOC_DPMONITOR_BUSY_CNT11_ADDR(base) ((base) + (0x040))
#define SOC_DPMONITOR_BUSY_CNT12_ADDR(base) ((base) + (0x044))
#define SOC_DPMONITOR_BUSY_CNT13_ADDR(base) ((base) + (0x048))
#define SOC_DPMONITOR_BUSY_CNT14_ADDR(base) ((base) + (0x04C))
#define SOC_DPMONITOR_BUSY_CNT15_ADDR(base) ((base) + (0x050))
#define SOC_DPMONITOR_ACC_ENERGY_ADDR(base) ((base) + (0x054))
#define SOC_DPMONITOR_PCLK_LOAD_ADDR(base) ((base) + (0x058))
#define SOC_DPMONITOR_VOLT_FREQ_SEL_ADDR(base) ((base) + (0x05C))
#define SOC_DPMONITOR_POWER_PARAM0_ADDR(base) ((base) + (0x060))
#define SOC_DPMONITOR_POWER_PARAM1_ADDR(base) ((base) + (0x064))
#define SOC_DPMONITOR_POWER_PARAM2_ADDR(base) ((base) + (0x068))
#define SOC_DPMONITOR_POWER_PARAM3_ADDR(base) ((base) + (0x06C))
#define SOC_DPMONITOR_POWER_PARAM4_ADDR(base) ((base) + (0x070))
#define SOC_DPMONITOR_POWER_PARAM5_ADDR(base) ((base) + (0x074))
#define SOC_DPMONITOR_POWER_PARAM6_ADDR(base) ((base) + (0x078))
#define SOC_DPMONITOR_POWER_PARAM7_ADDR(base) ((base) + (0x07C))
#define SOC_DPMONITOR_POWER_PARAM8_ADDR(base) ((base) + (0x080))
#define SOC_DPMONITOR_POWER_PARAM9_ADDR(base) ((base) + (0x084))
#define SOC_DPMONITOR_POWER_PARAM10_ADDR(base) ((base) + (0x088))
#define SOC_DPMONITOR_POWER_PARAM11_ADDR(base) ((base) + (0x08C))
#define SOC_DPMONITOR_POWER_PARAM12_ADDR(base) ((base) + (0x090))
#define SOC_DPMONITOR_POWER_PARAM13_ADDR(base) ((base) + (0x094))
#define SOC_DPMONITOR_POWER_PARAM14_ADDR(base) ((base) + (0x098))
#define SOC_DPMONITOR_POWER_PARAM15_ADDR(base) ((base) + (0x09C))
#define SOC_DPMONITOR_CONST_POWER_PARAM_ADDR(base) ((base) + (0x0A0))
#define SOC_DPMONITOR_VOTE_PARAM0_ADDR(base) ((base) + (0x0A4))
#define SOC_DPMONITOR_VOTE_PARAM1_ADDR(base) ((base) + (0x0A8))
#define SOC_DPMONITOR_VOTE_PARAM2_ADDR(base) ((base) + (0x0AC))
#define SOC_DPMONITOR_VOTE_PARAM3_ADDR(base) ((base) + (0x0B0))
#define SOC_DPMONITOR_VOTE_PARAM4_ADDR(base) ((base) + (0x0B4))
#define SOC_DPMONITOR_VOTE_PARAM5_ADDR(base) ((base) + (0x0B8))
#define SOC_DPMONITOR_VOTE_PARAM6_ADDR(base) ((base) + (0x0BC))
#define SOC_DPMONITOR_VOTE_PARAM7_ADDR(base) ((base) + (0x0C0))
#define SOC_DPMONITOR_FREQ_PARAM0_ADDR(base) ((base) + (0x0C4))
#define SOC_DPMONITOR_FREQ_PARAM1_ADDR(base) ((base) + (0x0C8))
#define SOC_DPMONITOR_FREQ_PARAM2_ADDR(base) ((base) + (0x0CC))
#define SOC_DPMONITOR_FREQ_PARAM3_ADDR(base) ((base) + (0x0D0))
#define SOC_DPMONITOR_FREQ_PARAM4_ADDR(base) ((base) + (0x0D4))
#define SOC_DPMONITOR_FREQ_PARAM5_ADDR(base) ((base) + (0x0D8))
#define SOC_DPMONITOR_FREQ_PARAM6_ADDR(base) ((base) + (0x0DC))
#define SOC_DPMONITOR_FREQ_PARAM7_ADDR(base) ((base) + (0x0E0))
#define SOC_DPMONITOR_SIGNAL_LEVEL_MODE_ADDR(base) ((base) + (0x0E4))
#define SOC_DPMONITOR_SOFT_PULSE_ADDR(base) ((base) + (0x0E8))
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_ADDR(base) ((base) + (0x0EC))
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_ADDR(base) ((base) + (0x0F0))
#define SOC_DPMONITOR_SIG_INV_EN_ADDR(base) ((base) + (0x0F4))
#define SOC_DPMONITOR_CLK_INVERSE_EN_ADDR(base) ((base) + (0x0F8))
#define SOC_DPMONITOR_CLK_INVERSE_DIS_ADDR(base) ((base) + (0x0FC))
#define SOC_DPMONITOR_CLK_INV_EN_ADDR(base) ((base) + (0x100))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int monitor_ctrl_en : 1;
        unsigned int sync_cg_off : 1;
        unsigned int reserved_0 : 14;
        unsigned int reserved_1 : 16;
    } reg;
} SOC_DPMONITOR_CTRL_EN_UNION;
#endif
#define SOC_DPMONITOR_CTRL_EN_monitor_ctrl_en_START (0)
#define SOC_DPMONITOR_CTRL_EN_monitor_ctrl_en_END (0)
#define SOC_DPMONITOR_CTRL_EN_sync_cg_off_START (1)
#define SOC_DPMONITOR_CTRL_EN_sync_cg_off_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int monitor_cfg_time : 32;
    } reg;
} SOC_DPMONITOR_CFG_TIME_UNION;
#endif
#define SOC_DPMONITOR_CFG_TIME_monitor_cfg_time_START (0)
#define SOC_DPMONITOR_CFG_TIME_monitor_cfg_time_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int monitor_sensor0_en : 1;
        unsigned int monitor_sensor1_en : 1;
        unsigned int monitor_sensor2_en : 1;
        unsigned int monitor_sensor3_en : 1;
        unsigned int monitor_sensor4_en : 1;
        unsigned int monitor_sensor5_en : 1;
        unsigned int monitor_sensor6_en : 1;
        unsigned int monitor_sensor7_en : 1;
        unsigned int monitor_sensor8_en : 1;
        unsigned int monitor_sensor9_en : 1;
        unsigned int monitor_sensor10_en : 1;
        unsigned int monitor_sensor11_en : 1;
        unsigned int monitor_sensor12_en : 1;
        unsigned int monitor_sensor13_en : 1;
        unsigned int monitor_sensor14_en : 1;
        unsigned int monitor_sensor15_en : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_DPMONITOR_SENSOR_EN_UNION;
#endif
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor0_en_START (0)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor0_en_END (0)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor1_en_START (1)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor1_en_END (1)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor2_en_START (2)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor2_en_END (2)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor3_en_START (3)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor3_en_END (3)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor4_en_START (4)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor4_en_END (4)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor5_en_START (5)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor5_en_END (5)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor6_en_START (6)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor6_en_END (6)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor7_en_START (7)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor7_en_END (7)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor8_en_START (8)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor8_en_END (8)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor9_en_START (9)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor9_en_END (9)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor10_en_START (10)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor10_en_END (10)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor11_en_START (11)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor11_en_END (11)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor12_en_START (12)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor12_en_END (12)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor13_en_START (13)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor13_en_END (13)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor14_en_START (14)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor14_en_END (14)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor15_en_START (15)
#define SOC_DPMONITOR_SENSOR_EN_monitor_sensor15_en_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int monitor_sensor0_dis : 1;
        unsigned int monitor_sensor1_dis : 1;
        unsigned int monitor_sensor2_dis : 1;
        unsigned int monitor_sensor3_dis : 1;
        unsigned int monitor_sensor4_dis : 1;
        unsigned int monitor_sensor5_dis : 1;
        unsigned int monitor_sensor6_dis : 1;
        unsigned int monitor_sensor7_dis : 1;
        unsigned int monitor_sensor8_dis : 1;
        unsigned int monitor_sensor9_dis : 1;
        unsigned int monitor_sensor10_dis : 1;
        unsigned int monitor_sensor11_dis : 1;
        unsigned int monitor_sensor12_dis : 1;
        unsigned int monitor_sensor13_dis : 1;
        unsigned int monitor_sensor14_dis : 1;
        unsigned int monitor_sensor15_dis : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_DPMONITOR_SENSOR_DIS_UNION;
#endif
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor0_dis_START (0)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor0_dis_END (0)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor1_dis_START (1)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor1_dis_END (1)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor2_dis_START (2)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor2_dis_END (2)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor3_dis_START (3)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor3_dis_END (3)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor4_dis_START (4)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor4_dis_END (4)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor5_dis_START (5)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor5_dis_END (5)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor6_dis_START (6)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor6_dis_END (6)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor7_dis_START (7)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor7_dis_END (7)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor8_dis_START (8)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor8_dis_END (8)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor9_dis_START (9)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor9_dis_END (9)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor10_dis_START (10)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor10_dis_END (10)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor11_dis_START (11)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor11_dis_END (11)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor12_dis_START (12)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor12_dis_END (12)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor13_dis_START (13)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor13_dis_END (13)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor14_dis_START (14)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor14_dis_END (14)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor15_dis_START (15)
#define SOC_DPMONITOR_SENSOR_DIS_monitor_sensor15_dis_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int monitor_sensor0_stat : 1;
        unsigned int monitor_sensor1_stat : 1;
        unsigned int monitor_sensor2_stat : 1;
        unsigned int monitor_sensor3_stat : 1;
        unsigned int monitor_sensor4_stat : 1;
        unsigned int monitor_sensor5_stat : 1;
        unsigned int monitor_sensor6_stat : 1;
        unsigned int monitor_sensor7_stat : 1;
        unsigned int monitor_sensor8_stat : 1;
        unsigned int monitor_sensor9_stat : 1;
        unsigned int monitor_sensor10_stat : 1;
        unsigned int monitor_sensor11_stat : 1;
        unsigned int monitor_sensor12_stat : 1;
        unsigned int monitor_sensor13_stat : 1;
        unsigned int monitor_sensor14_stat : 1;
        unsigned int monitor_sensor15_stat : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_DPMONITOR_DPM_SENSOR_STAT_UNION;
#endif
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor0_stat_START (0)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor0_stat_END (0)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor1_stat_START (1)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor1_stat_END (1)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor2_stat_START (2)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor2_stat_END (2)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor3_stat_START (3)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor3_stat_END (3)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor4_stat_START (4)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor4_stat_END (4)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor5_stat_START (5)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor5_stat_END (5)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor6_stat_START (6)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor6_stat_END (6)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor7_stat_START (7)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor7_stat_END (7)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor8_stat_START (8)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor8_stat_END (8)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor9_stat_START (9)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor9_stat_END (9)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor10_stat_START (10)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor10_stat_END (10)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor11_stat_START (11)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor11_stat_END (11)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor12_stat_START (12)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor12_stat_END (12)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor13_stat_START (13)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor13_stat_END (13)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor14_stat_START (14)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor14_stat_END (14)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor15_stat_START (15)
#define SOC_DPMONITOR_DPM_SENSOR_STAT_monitor_sensor15_stat_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig0_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT0_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT0_sig0_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT0_sig0_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig1_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT1_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT1_sig1_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT1_sig1_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig2_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT2_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT2_sig2_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT2_sig2_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig3_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT3_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT3_sig3_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT3_sig3_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig4_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT4_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT4_sig4_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT4_sig4_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig5_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT5_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT5_sig5_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT5_sig5_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig6_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT6_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT6_sig6_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT6_sig6_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig7_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT7_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT7_sig7_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT7_sig7_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig8_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT8_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT8_sig8_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT8_sig8_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig9_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT9_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT9_sig9_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT9_sig9_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig10_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT10_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT10_sig10_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT10_sig10_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig11_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT11_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT11_sig11_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT11_sig11_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig12_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT12_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT12_sig12_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT12_sig12_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig13_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT13_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT13_sig13_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT13_sig13_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig14_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT14_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT14_sig14_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT14_sig14_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig15_busy_cnt : 32;
    } reg;
} SOC_DPMONITOR_BUSY_CNT15_UNION;
#endif
#define SOC_DPMONITOR_BUSY_CNT15_sig15_busy_cnt_START (0)
#define SOC_DPMONITOR_BUSY_CNT15_sig15_busy_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int acc_energy : 32;
    } reg;
} SOC_DPMONITOR_ACC_ENERGY_UNION;
#endif
#define SOC_DPMONITOR_ACC_ENERGY_acc_energy_START (0)
#define SOC_DPMONITOR_ACC_ENERGY_acc_energy_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pclk_cnt_time : 32;
    } reg;
} SOC_DPMONITOR_PCLK_LOAD_UNION;
#endif
#define SOC_DPMONITOR_PCLK_LOAD_pclk_cnt_time_START (0)
#define SOC_DPMONITOR_PCLK_LOAD_pclk_cnt_time_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int volt_sel : 3;
        unsigned int freq_sel : 3;
        unsigned int reserved : 26;
    } reg;
} SOC_DPMONITOR_VOLT_FREQ_SEL_UNION;
#endif
#define SOC_DPMONITOR_VOLT_FREQ_SEL_volt_sel_START (0)
#define SOC_DPMONITOR_VOLT_FREQ_SEL_volt_sel_END (2)
#define SOC_DPMONITOR_VOLT_FREQ_SEL_freq_sel_START (3)
#define SOC_DPMONITOR_VOLT_FREQ_SEL_freq_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig0_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM0_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM0_sig0_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM0_sig0_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig1_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM1_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM1_sig1_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM1_sig1_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig2_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM2_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM2_sig2_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM2_sig2_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig3_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM3_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM3_sig3_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM3_sig3_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig4_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM4_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM4_sig4_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM4_sig4_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig5_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM5_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM5_sig5_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM5_sig5_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig6_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM6_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM6_sig6_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM6_sig6_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig7_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM7_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM7_sig7_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM7_sig7_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig8_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM8_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM8_sig8_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM8_sig8_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig9_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM9_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM9_sig9_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM9_sig9_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig10_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM10_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM10_sig10_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM10_sig10_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig11_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM11_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM11_sig11_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM11_sig11_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig12_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM12_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM12_sig12_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM12_sig12_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig13_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM13_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM13_sig13_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM13_sig13_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig14_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM14_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM14_sig14_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM14_sig14_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig15_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_POWER_PARAM15_UNION;
#endif
#define SOC_DPMONITOR_POWER_PARAM15_sig15_power_param_START (0)
#define SOC_DPMONITOR_POWER_PARAM15_sig15_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int const_power_param : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_DPMONITOR_CONST_POWER_PARAM_UNION;
#endif
#define SOC_DPMONITOR_CONST_POWER_PARAM_const_power_param_START (0)
#define SOC_DPMONITOR_CONST_POWER_PARAM_const_power_param_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int volt0_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_VOTE_PARAM0_UNION;
#endif
#define SOC_DPMONITOR_VOTE_PARAM0_volt0_param_START (0)
#define SOC_DPMONITOR_VOTE_PARAM0_volt0_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int volt1_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_VOTE_PARAM1_UNION;
#endif
#define SOC_DPMONITOR_VOTE_PARAM1_volt1_param_START (0)
#define SOC_DPMONITOR_VOTE_PARAM1_volt1_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int volt2_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_VOTE_PARAM2_UNION;
#endif
#define SOC_DPMONITOR_VOTE_PARAM2_volt2_param_START (0)
#define SOC_DPMONITOR_VOTE_PARAM2_volt2_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int volt3_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_VOTE_PARAM3_UNION;
#endif
#define SOC_DPMONITOR_VOTE_PARAM3_volt3_param_START (0)
#define SOC_DPMONITOR_VOTE_PARAM3_volt3_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int volt4_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_VOTE_PARAM4_UNION;
#endif
#define SOC_DPMONITOR_VOTE_PARAM4_volt4_param_START (0)
#define SOC_DPMONITOR_VOTE_PARAM4_volt4_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int volt5_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_VOTE_PARAM5_UNION;
#endif
#define SOC_DPMONITOR_VOTE_PARAM5_volt5_param_START (0)
#define SOC_DPMONITOR_VOTE_PARAM5_volt5_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int volt6_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_VOTE_PARAM6_UNION;
#endif
#define SOC_DPMONITOR_VOTE_PARAM6_volt6_param_START (0)
#define SOC_DPMONITOR_VOTE_PARAM6_volt6_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int volt7_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_VOTE_PARAM7_UNION;
#endif
#define SOC_DPMONITOR_VOTE_PARAM7_volt7_param_START (0)
#define SOC_DPMONITOR_VOTE_PARAM7_volt7_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int freq0_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_FREQ_PARAM0_UNION;
#endif
#define SOC_DPMONITOR_FREQ_PARAM0_freq0_param_START (0)
#define SOC_DPMONITOR_FREQ_PARAM0_freq0_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int freq1_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_FREQ_PARAM1_UNION;
#endif
#define SOC_DPMONITOR_FREQ_PARAM1_freq1_param_START (0)
#define SOC_DPMONITOR_FREQ_PARAM1_freq1_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int freq2_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_FREQ_PARAM2_UNION;
#endif
#define SOC_DPMONITOR_FREQ_PARAM2_freq2_param_START (0)
#define SOC_DPMONITOR_FREQ_PARAM2_freq2_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int freq3_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_FREQ_PARAM3_UNION;
#endif
#define SOC_DPMONITOR_FREQ_PARAM3_freq3_param_START (0)
#define SOC_DPMONITOR_FREQ_PARAM3_freq3_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int freq4_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_FREQ_PARAM4_UNION;
#endif
#define SOC_DPMONITOR_FREQ_PARAM4_freq4_param_START (0)
#define SOC_DPMONITOR_FREQ_PARAM4_freq4_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int freq5_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_FREQ_PARAM5_UNION;
#endif
#define SOC_DPMONITOR_FREQ_PARAM5_freq5_param_START (0)
#define SOC_DPMONITOR_FREQ_PARAM5_freq5_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int freq6_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_FREQ_PARAM6_UNION;
#endif
#define SOC_DPMONITOR_FREQ_PARAM6_freq6_param_START (0)
#define SOC_DPMONITOR_FREQ_PARAM6_freq6_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int freq7_param : 10;
        unsigned int reserved : 22;
    } reg;
} SOC_DPMONITOR_FREQ_PARAM7_UNION;
#endif
#define SOC_DPMONITOR_FREQ_PARAM7_freq7_param_START (0)
#define SOC_DPMONITOR_FREQ_PARAM7_freq7_param_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig_lev_mode : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_DPMONITOR_SIGNAL_LEVEL_MODE_UNION;
#endif
#define SOC_DPMONITOR_SIGNAL_LEVEL_MODE_sig_lev_mode_START (0)
#define SOC_DPMONITOR_SIGNAL_LEVEL_MODE_sig_lev_mode_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int soft_pulse : 2;
        unsigned int reserved : 30;
    } reg;
} SOC_DPMONITOR_SOFT_PULSE_UNION;
#endif
#define SOC_DPMONITOR_SOFT_PULSE_soft_pulse_START (0)
#define SOC_DPMONITOR_SOFT_PULSE_soft_pulse_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig0_inv_en : 1;
        unsigned int sig1_inv_en : 1;
        unsigned int sig2_inv_en : 1;
        unsigned int sig3_inv_en : 1;
        unsigned int sig4_inv_en : 1;
        unsigned int sig5_inv_en : 1;
        unsigned int sig6_inv_en : 1;
        unsigned int sig7_inv_en : 1;
        unsigned int sig8_inv_en : 1;
        unsigned int sig9_inv_en : 1;
        unsigned int sig10_inv_en : 1;
        unsigned int sig11_inv_en : 1;
        unsigned int sig12_inv_en : 1;
        unsigned int sig13_inv_en : 1;
        unsigned int sig14_inv_en : 1;
        unsigned int sig15_inv_en : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_DPMONITOR_SIGNAL_INVERSE_EN_UNION;
#endif
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig0_inv_en_START (0)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig0_inv_en_END (0)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig1_inv_en_START (1)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig1_inv_en_END (1)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig2_inv_en_START (2)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig2_inv_en_END (2)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig3_inv_en_START (3)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig3_inv_en_END (3)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig4_inv_en_START (4)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig4_inv_en_END (4)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig5_inv_en_START (5)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig5_inv_en_END (5)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig6_inv_en_START (6)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig6_inv_en_END (6)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig7_inv_en_START (7)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig7_inv_en_END (7)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig8_inv_en_START (8)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig8_inv_en_END (8)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig9_inv_en_START (9)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig9_inv_en_END (9)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig10_inv_en_START (10)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig10_inv_en_END (10)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig11_inv_en_START (11)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig11_inv_en_END (11)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig12_inv_en_START (12)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig12_inv_en_END (12)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig13_inv_en_START (13)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig13_inv_en_END (13)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig14_inv_en_START (14)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig14_inv_en_END (14)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig15_inv_en_START (15)
#define SOC_DPMONITOR_SIGNAL_INVERSE_EN_sig15_inv_en_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig0_inv_dis : 1;
        unsigned int sig1_inv_dis : 1;
        unsigned int sig2_inv_dis : 1;
        unsigned int sig3_inv_dis : 1;
        unsigned int sig4_inv_dis : 1;
        unsigned int sig5_inv_dis : 1;
        unsigned int sig6_inv_dis : 1;
        unsigned int sig7_inv_dis : 1;
        unsigned int sig8_inv_dis : 1;
        unsigned int sig9_inv_dis : 1;
        unsigned int sig10_inv_dis : 1;
        unsigned int sig11_inv_dis : 1;
        unsigned int sig12_inv_dis : 1;
        unsigned int sig13_inv_dis : 1;
        unsigned int sig14_inv_dis : 1;
        unsigned int sig15_inv_dis : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_DPMONITOR_SIGNAL_INVERSE_DIS_UNION;
#endif
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig0_inv_dis_START (0)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig0_inv_dis_END (0)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig1_inv_dis_START (1)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig1_inv_dis_END (1)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig2_inv_dis_START (2)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig2_inv_dis_END (2)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig3_inv_dis_START (3)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig3_inv_dis_END (3)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig4_inv_dis_START (4)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig4_inv_dis_END (4)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig5_inv_dis_START (5)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig5_inv_dis_END (5)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig6_inv_dis_START (6)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig6_inv_dis_END (6)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig7_inv_dis_START (7)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig7_inv_dis_END (7)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig8_inv_dis_START (8)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig8_inv_dis_END (8)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig9_inv_dis_START (9)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig9_inv_dis_END (9)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig10_inv_dis_START (10)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig10_inv_dis_END (10)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig11_inv_dis_START (11)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig11_inv_dis_END (11)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig12_inv_dis_START (12)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig12_inv_dis_END (12)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig13_inv_dis_START (13)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig13_inv_dis_END (13)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig14_inv_dis_START (14)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig14_inv_dis_END (14)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig15_inv_dis_START (15)
#define SOC_DPMONITOR_SIGNAL_INVERSE_DIS_sig15_inv_dis_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sig0_inv_stat : 1;
        unsigned int sig1_inv_stat : 1;
        unsigned int sig2_inv_stat : 1;
        unsigned int sig3_inv_stat : 1;
        unsigned int sig4_inv_stat : 1;
        unsigned int sig5_inv_stat : 1;
        unsigned int sig6_inv_stat : 1;
        unsigned int sig7_inv_stat : 1;
        unsigned int sig8_inv_stat : 1;
        unsigned int sig9_inv_stat : 1;
        unsigned int sig10_inv_stat : 1;
        unsigned int sig11_inv_stat : 1;
        unsigned int sig12_inv_stat : 1;
        unsigned int sig13_inv_stat : 1;
        unsigned int sig14_inv_stat : 1;
        unsigned int sig15_inv_stat : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_DPMONITOR_SIG_INV_EN_UNION;
#endif
#define SOC_DPMONITOR_SIG_INV_EN_sig0_inv_stat_START (0)
#define SOC_DPMONITOR_SIG_INV_EN_sig0_inv_stat_END (0)
#define SOC_DPMONITOR_SIG_INV_EN_sig1_inv_stat_START (1)
#define SOC_DPMONITOR_SIG_INV_EN_sig1_inv_stat_END (1)
#define SOC_DPMONITOR_SIG_INV_EN_sig2_inv_stat_START (2)
#define SOC_DPMONITOR_SIG_INV_EN_sig2_inv_stat_END (2)
#define SOC_DPMONITOR_SIG_INV_EN_sig3_inv_stat_START (3)
#define SOC_DPMONITOR_SIG_INV_EN_sig3_inv_stat_END (3)
#define SOC_DPMONITOR_SIG_INV_EN_sig4_inv_stat_START (4)
#define SOC_DPMONITOR_SIG_INV_EN_sig4_inv_stat_END (4)
#define SOC_DPMONITOR_SIG_INV_EN_sig5_inv_stat_START (5)
#define SOC_DPMONITOR_SIG_INV_EN_sig5_inv_stat_END (5)
#define SOC_DPMONITOR_SIG_INV_EN_sig6_inv_stat_START (6)
#define SOC_DPMONITOR_SIG_INV_EN_sig6_inv_stat_END (6)
#define SOC_DPMONITOR_SIG_INV_EN_sig7_inv_stat_START (7)
#define SOC_DPMONITOR_SIG_INV_EN_sig7_inv_stat_END (7)
#define SOC_DPMONITOR_SIG_INV_EN_sig8_inv_stat_START (8)
#define SOC_DPMONITOR_SIG_INV_EN_sig8_inv_stat_END (8)
#define SOC_DPMONITOR_SIG_INV_EN_sig9_inv_stat_START (9)
#define SOC_DPMONITOR_SIG_INV_EN_sig9_inv_stat_END (9)
#define SOC_DPMONITOR_SIG_INV_EN_sig10_inv_stat_START (10)
#define SOC_DPMONITOR_SIG_INV_EN_sig10_inv_stat_END (10)
#define SOC_DPMONITOR_SIG_INV_EN_sig11_inv_stat_START (11)
#define SOC_DPMONITOR_SIG_INV_EN_sig11_inv_stat_END (11)
#define SOC_DPMONITOR_SIG_INV_EN_sig12_inv_stat_START (12)
#define SOC_DPMONITOR_SIG_INV_EN_sig12_inv_stat_END (12)
#define SOC_DPMONITOR_SIG_INV_EN_sig13_inv_stat_START (13)
#define SOC_DPMONITOR_SIG_INV_EN_sig13_inv_stat_END (13)
#define SOC_DPMONITOR_SIG_INV_EN_sig14_inv_stat_START (14)
#define SOC_DPMONITOR_SIG_INV_EN_sig14_inv_stat_END (14)
#define SOC_DPMONITOR_SIG_INV_EN_sig15_inv_stat_START (15)
#define SOC_DPMONITOR_SIG_INV_EN_sig15_inv_stat_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int clk0_inv_en : 1;
        unsigned int clk1_inv_en : 1;
        unsigned int clk2_inv_en : 1;
        unsigned int clk3_inv_en : 1;
        unsigned int clk4_inv_en : 1;
        unsigned int clk5_inv_en : 1;
        unsigned int clk6_inv_en : 1;
        unsigned int clk7_inv_en : 1;
        unsigned int clk8_inv_en : 1;
        unsigned int clk9_inv_en : 1;
        unsigned int clk10_inv_en : 1;
        unsigned int clk11_inv_en : 1;
        unsigned int clk12_inv_en : 1;
        unsigned int clk13_inv_en : 1;
        unsigned int clk14_inv_en : 1;
        unsigned int clk15_inv_en : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_DPMONITOR_CLK_INVERSE_EN_UNION;
#endif
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk0_inv_en_START (0)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk0_inv_en_END (0)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk1_inv_en_START (1)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk1_inv_en_END (1)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk2_inv_en_START (2)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk2_inv_en_END (2)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk3_inv_en_START (3)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk3_inv_en_END (3)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk4_inv_en_START (4)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk4_inv_en_END (4)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk5_inv_en_START (5)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk5_inv_en_END (5)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk6_inv_en_START (6)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk6_inv_en_END (6)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk7_inv_en_START (7)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk7_inv_en_END (7)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk8_inv_en_START (8)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk8_inv_en_END (8)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk9_inv_en_START (9)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk9_inv_en_END (9)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk10_inv_en_START (10)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk10_inv_en_END (10)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk11_inv_en_START (11)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk11_inv_en_END (11)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk12_inv_en_START (12)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk12_inv_en_END (12)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk13_inv_en_START (13)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk13_inv_en_END (13)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk14_inv_en_START (14)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk14_inv_en_END (14)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk15_inv_en_START (15)
#define SOC_DPMONITOR_CLK_INVERSE_EN_clk15_inv_en_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int clk0_inv_dis : 1;
        unsigned int clk1_inv_dis : 1;
        unsigned int clk2_inv_dis : 1;
        unsigned int clk3_inv_dis : 1;
        unsigned int clk4_inv_dis : 1;
        unsigned int clk5_inv_dis : 1;
        unsigned int clk6_inv_dis : 1;
        unsigned int clk7_inv_dis : 1;
        unsigned int clk8_inv_dis : 1;
        unsigned int clk9_inv_dis : 1;
        unsigned int clk10_inv_dis : 1;
        unsigned int clk11_inv_dis : 1;
        unsigned int clk12_inv_dis : 1;
        unsigned int clk13_inv_dis : 1;
        unsigned int clk14_inv_dis : 1;
        unsigned int clk15_inv_dis : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_DPMONITOR_CLK_INVERSE_DIS_UNION;
#endif
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk0_inv_dis_START (0)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk0_inv_dis_END (0)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk1_inv_dis_START (1)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk1_inv_dis_END (1)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk2_inv_dis_START (2)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk2_inv_dis_END (2)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk3_inv_dis_START (3)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk3_inv_dis_END (3)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk4_inv_dis_START (4)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk4_inv_dis_END (4)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk5_inv_dis_START (5)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk5_inv_dis_END (5)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk6_inv_dis_START (6)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk6_inv_dis_END (6)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk7_inv_dis_START (7)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk7_inv_dis_END (7)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk8_inv_dis_START (8)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk8_inv_dis_END (8)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk9_inv_dis_START (9)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk9_inv_dis_END (9)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk10_inv_dis_START (10)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk10_inv_dis_END (10)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk11_inv_dis_START (11)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk11_inv_dis_END (11)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk12_inv_dis_START (12)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk12_inv_dis_END (12)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk13_inv_dis_START (13)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk13_inv_dis_END (13)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk14_inv_dis_START (14)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk14_inv_dis_END (14)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk15_inv_dis_START (15)
#define SOC_DPMONITOR_CLK_INVERSE_DIS_clk15_inv_dis_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int clk0_inv_stat : 1;
        unsigned int clk1_inv_stat : 1;
        unsigned int clk2_inv_stat : 1;
        unsigned int clk3_inv_stat : 1;
        unsigned int clk4_inv_stat : 1;
        unsigned int clk5_inv_stat : 1;
        unsigned int clk6_inv_stat : 1;
        unsigned int clk7_inv_stat : 1;
        unsigned int clk8_inv_stat : 1;
        unsigned int clk9_inv_stat : 1;
        unsigned int clk10_inv_stat : 1;
        unsigned int clk11_inv_stat : 1;
        unsigned int clk12_inv_stat : 1;
        unsigned int clk13_inv_stat : 1;
        unsigned int clk14_inv_stat : 1;
        unsigned int clk15_inv_stat : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_DPMONITOR_CLK_INV_EN_UNION;
#endif
#define SOC_DPMONITOR_CLK_INV_EN_clk0_inv_stat_START (0)
#define SOC_DPMONITOR_CLK_INV_EN_clk0_inv_stat_END (0)
#define SOC_DPMONITOR_CLK_INV_EN_clk1_inv_stat_START (1)
#define SOC_DPMONITOR_CLK_INV_EN_clk1_inv_stat_END (1)
#define SOC_DPMONITOR_CLK_INV_EN_clk2_inv_stat_START (2)
#define SOC_DPMONITOR_CLK_INV_EN_clk2_inv_stat_END (2)
#define SOC_DPMONITOR_CLK_INV_EN_clk3_inv_stat_START (3)
#define SOC_DPMONITOR_CLK_INV_EN_clk3_inv_stat_END (3)
#define SOC_DPMONITOR_CLK_INV_EN_clk4_inv_stat_START (4)
#define SOC_DPMONITOR_CLK_INV_EN_clk4_inv_stat_END (4)
#define SOC_DPMONITOR_CLK_INV_EN_clk5_inv_stat_START (5)
#define SOC_DPMONITOR_CLK_INV_EN_clk5_inv_stat_END (5)
#define SOC_DPMONITOR_CLK_INV_EN_clk6_inv_stat_START (6)
#define SOC_DPMONITOR_CLK_INV_EN_clk6_inv_stat_END (6)
#define SOC_DPMONITOR_CLK_INV_EN_clk7_inv_stat_START (7)
#define SOC_DPMONITOR_CLK_INV_EN_clk7_inv_stat_END (7)
#define SOC_DPMONITOR_CLK_INV_EN_clk8_inv_stat_START (8)
#define SOC_DPMONITOR_CLK_INV_EN_clk8_inv_stat_END (8)
#define SOC_DPMONITOR_CLK_INV_EN_clk9_inv_stat_START (9)
#define SOC_DPMONITOR_CLK_INV_EN_clk9_inv_stat_END (9)
#define SOC_DPMONITOR_CLK_INV_EN_clk10_inv_stat_START (10)
#define SOC_DPMONITOR_CLK_INV_EN_clk10_inv_stat_END (10)
#define SOC_DPMONITOR_CLK_INV_EN_clk11_inv_stat_START (11)
#define SOC_DPMONITOR_CLK_INV_EN_clk11_inv_stat_END (11)
#define SOC_DPMONITOR_CLK_INV_EN_clk12_inv_stat_START (12)
#define SOC_DPMONITOR_CLK_INV_EN_clk12_inv_stat_END (12)
#define SOC_DPMONITOR_CLK_INV_EN_clk13_inv_stat_START (13)
#define SOC_DPMONITOR_CLK_INV_EN_clk13_inv_stat_END (13)
#define SOC_DPMONITOR_CLK_INV_EN_clk14_inv_stat_START (14)
#define SOC_DPMONITOR_CLK_INV_EN_clk14_inv_stat_END (14)
#define SOC_DPMONITOR_CLK_INV_EN_clk15_inv_stat_START (15)
#define SOC_DPMONITOR_CLK_INV_EN_clk15_inv_stat_END (15)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
