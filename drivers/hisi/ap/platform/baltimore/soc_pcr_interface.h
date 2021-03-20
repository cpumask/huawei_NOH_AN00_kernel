#ifndef __SOC_PCR_INTERFACE_H__
#define __SOC_PCR_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_PCR_CFG_BYPASS_ADDR(base) ((base) + (0x040UL))
#define SOC_PCR_THRESHOLD_BUDGET0_ADDR(base) ((base) + (0x044UL))
#define SOC_PCR_THRESHOLD_BUDGET1_ADDR(base) ((base) + (0x048UL))
#define SOC_PCR_THRESHOLD_BUDGET2_ADDR(base) ((base) + (0x04CUL))
#define SOC_PCR_THRESHOLD_BUDGET3_ADDR(base) ((base) + (0x050UL))
#define SOC_PCR_THRESHOLD_DIDT0_ADDR(base) ((base) + (0x054UL))
#define SOC_PCR_THRESHOLD_DIDT1_ADDR(base) ((base) + (0x058UL))
#define SOC_PCR_THRESHOLD_DIDT2_ADDR(base) ((base) + (0x05CUL))
#define SOC_PCR_THRESHOLD_DIDT3_ADDR(base) ((base) + (0x060UL))
#define SOC_PCR_POWER_VALUE0_ADDR(base) ((base) + (0x064UL))
#define SOC_PCR_POWER_VALUE1_ADDR(base) ((base) + (0x068UL))
#define SOC_PCR_TIMER_COUNTER0_ADDR(base) ((base) + (0x06CUL))
#define SOC_PCR_TIMER_COUNTER1_ADDR(base) ((base) + (0x070UL))
#define SOC_PCR_CTRL_ADDR(base) ((base) + (0x074UL))
#define SOC_PCR_CFG_PERIOD0_ADDR(base) ((base) + (0x078UL))
#define SOC_PCR_CFG_PERIOD1_ADDR(base) ((base) + (0x07CUL))
#define SOC_PCR_THRESHOLD_BUDGET_SVFD0_ADDR(base) ((base) + (0x080UL))
#define SOC_PCR_THRESHOLD_BUDGET_SVFD1_ADDR(base) ((base) + (0x084UL))
#define SOC_PCR_SELECTED_MSB_ADDR(base) ((base) + (0x088UL))
#define SOC_PCR_SVFD_COUNTER_ADDR(base) ((base) + (0x10CUL))
#define SOC_PCR_AVG_ST_COUNTER_ADDR(base) ((base) + (0x110UL))
#define SOC_PCR_AVG_LT_COUNTER_ADDR(base) ((base) + (0x114UL))
#define SOC_PCR_DIDT_COUNTER_ADDR(base) ((base) + (0x118UL))
#define SOC_PCR_SRAM_RADDR_ADDR(base) ((base) + (0x11CUL))
#define SOC_PCR_SRAM_WADDR_ADDR(base) ((base) + (0x120UL))
#define SOC_PCR_RDATA_ST_ADDR(base) ((base) + (0x124UL))
#define SOC_PCR_RDATA_LT_ADDR(base) ((base) + (0x128UL))
#define SOC_PCR_CLK_SW_ADDR(base) ((base) + (0x12CUL))
#define SOC_PCR_POWER_SUM_ADDR(base) ((base) + (0x130UL))
#define SOC_PCR_INTR_STAT_ADDR(base) ((base) + (0x134UL))
#define SOC_PCR_INTR_RAW_STAT_ADDR(base) ((base) + (0x138UL))
#define SOC_PCR_INTR_EN_ADDR(base) ((base) + (0x13CUL))
#define SOC_PCR_INTR_CLR_ADDR(base) ((base) + (0x140UL))
#else
#define SOC_PCR_CFG_BYPASS_ADDR(base) ((base) + (0x040))
#define SOC_PCR_THRESHOLD_BUDGET0_ADDR(base) ((base) + (0x044))
#define SOC_PCR_THRESHOLD_BUDGET1_ADDR(base) ((base) + (0x048))
#define SOC_PCR_THRESHOLD_BUDGET2_ADDR(base) ((base) + (0x04C))
#define SOC_PCR_THRESHOLD_BUDGET3_ADDR(base) ((base) + (0x050))
#define SOC_PCR_THRESHOLD_DIDT0_ADDR(base) ((base) + (0x054))
#define SOC_PCR_THRESHOLD_DIDT1_ADDR(base) ((base) + (0x058))
#define SOC_PCR_THRESHOLD_DIDT2_ADDR(base) ((base) + (0x05C))
#define SOC_PCR_THRESHOLD_DIDT3_ADDR(base) ((base) + (0x060))
#define SOC_PCR_POWER_VALUE0_ADDR(base) ((base) + (0x064))
#define SOC_PCR_POWER_VALUE1_ADDR(base) ((base) + (0x068))
#define SOC_PCR_TIMER_COUNTER0_ADDR(base) ((base) + (0x06C))
#define SOC_PCR_TIMER_COUNTER1_ADDR(base) ((base) + (0x070))
#define SOC_PCR_CTRL_ADDR(base) ((base) + (0x074))
#define SOC_PCR_CFG_PERIOD0_ADDR(base) ((base) + (0x078))
#define SOC_PCR_CFG_PERIOD1_ADDR(base) ((base) + (0x07C))
#define SOC_PCR_THRESHOLD_BUDGET_SVFD0_ADDR(base) ((base) + (0x080))
#define SOC_PCR_THRESHOLD_BUDGET_SVFD1_ADDR(base) ((base) + (0x084))
#define SOC_PCR_SELECTED_MSB_ADDR(base) ((base) + (0x088))
#define SOC_PCR_SVFD_COUNTER_ADDR(base) ((base) + (0x10C))
#define SOC_PCR_AVG_ST_COUNTER_ADDR(base) ((base) + (0x110))
#define SOC_PCR_AVG_LT_COUNTER_ADDR(base) ((base) + (0x114))
#define SOC_PCR_DIDT_COUNTER_ADDR(base) ((base) + (0x118))
#define SOC_PCR_SRAM_RADDR_ADDR(base) ((base) + (0x11C))
#define SOC_PCR_SRAM_WADDR_ADDR(base) ((base) + (0x120))
#define SOC_PCR_RDATA_ST_ADDR(base) ((base) + (0x124))
#define SOC_PCR_RDATA_LT_ADDR(base) ((base) + (0x128))
#define SOC_PCR_CLK_SW_ADDR(base) ((base) + (0x12C))
#define SOC_PCR_POWER_SUM_ADDR(base) ((base) + (0x130))
#define SOC_PCR_INTR_STAT_ADDR(base) ((base) + (0x134))
#define SOC_PCR_INTR_RAW_STAT_ADDR(base) ((base) + (0x138))
#define SOC_PCR_INTR_EN_ADDR(base) ((base) + (0x13C))
#define SOC_PCR_INTR_CLR_ADDR(base) ((base) + (0x140))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ff_en_bypass : 1;
        unsigned int clk_sw_bypass : 1;
        unsigned int svfd_bypass : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_PCR_CFG_BYPASS_UNION;
#endif
#define SOC_PCR_CFG_BYPASS_ff_en_bypass_START (0)
#define SOC_PCR_CFG_BYPASS_ff_en_bypass_END (0)
#define SOC_PCR_CFG_BYPASS_clk_sw_bypass_START (1)
#define SOC_PCR_CFG_BYPASS_clk_sw_bypass_END (1)
#define SOC_PCR_CFG_BYPASS_svfd_bypass_START (2)
#define SOC_PCR_CFG_BYPASS_svfd_bypass_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int threshold_budget0 : 32;
    } reg;
} SOC_PCR_THRESHOLD_BUDGET0_UNION;
#endif
#define SOC_PCR_THRESHOLD_BUDGET0_threshold_budget0_START (0)
#define SOC_PCR_THRESHOLD_BUDGET0_threshold_budget0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int threshold_budget1 : 32;
    } reg;
} SOC_PCR_THRESHOLD_BUDGET1_UNION;
#endif
#define SOC_PCR_THRESHOLD_BUDGET1_threshold_budget1_START (0)
#define SOC_PCR_THRESHOLD_BUDGET1_threshold_budget1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int threshold_budget2 : 32;
    } reg;
} SOC_PCR_THRESHOLD_BUDGET2_UNION;
#endif
#define SOC_PCR_THRESHOLD_BUDGET2_threshold_budget2_START (0)
#define SOC_PCR_THRESHOLD_BUDGET2_threshold_budget2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int threshold_budget3 : 32;
    } reg;
} SOC_PCR_THRESHOLD_BUDGET3_UNION;
#endif
#define SOC_PCR_THRESHOLD_BUDGET3_threshold_budget3_START (0)
#define SOC_PCR_THRESHOLD_BUDGET3_threshold_budget3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int threshold_didt0 : 32;
    } reg;
} SOC_PCR_THRESHOLD_DIDT0_UNION;
#endif
#define SOC_PCR_THRESHOLD_DIDT0_threshold_didt0_START (0)
#define SOC_PCR_THRESHOLD_DIDT0_threshold_didt0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int threshold_didt1 : 32;
    } reg;
} SOC_PCR_THRESHOLD_DIDT1_UNION;
#endif
#define SOC_PCR_THRESHOLD_DIDT1_threshold_didt1_START (0)
#define SOC_PCR_THRESHOLD_DIDT1_threshold_didt1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int threshold_didt2 : 32;
    } reg;
} SOC_PCR_THRESHOLD_DIDT2_UNION;
#endif
#define SOC_PCR_THRESHOLD_DIDT2_threshold_didt2_START (0)
#define SOC_PCR_THRESHOLD_DIDT2_threshold_didt2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int threshold_didt3 : 32;
    } reg;
} SOC_PCR_THRESHOLD_DIDT3_UNION;
#endif
#define SOC_PCR_THRESHOLD_DIDT3_threshold_didt3_START (0)
#define SOC_PCR_THRESHOLD_DIDT3_threshold_didt3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int power_value0 : 32;
    } reg;
} SOC_PCR_POWER_VALUE0_UNION;
#endif
#define SOC_PCR_POWER_VALUE0_power_value0_START (0)
#define SOC_PCR_POWER_VALUE0_power_value0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int power_value1 : 32;
    } reg;
} SOC_PCR_POWER_VALUE1_UNION;
#endif
#define SOC_PCR_POWER_VALUE1_power_value1_START (0)
#define SOC_PCR_POWER_VALUE1_power_value1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int timer_counter0 : 32;
    } reg;
} SOC_PCR_TIMER_COUNTER0_UNION;
#endif
#define SOC_PCR_TIMER_COUNTER0_timer_counter0_START (0)
#define SOC_PCR_TIMER_COUNTER0_timer_counter0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int timer_counter1 : 32;
    } reg;
} SOC_PCR_TIMER_COUNTER1_UNION;
#endif
#define SOC_PCR_TIMER_COUNTER1_timer_counter1_START (0)
#define SOC_PCR_TIMER_COUNTER1_timer_counter1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctrl_mode_didt : 2;
        unsigned int ctrl_mode_budget : 2;
        unsigned int enable : 1;
        unsigned int snapshot : 1;
        unsigned int reserved : 26;
    } reg;
} SOC_PCR_CTRL_UNION;
#endif
#define SOC_PCR_CTRL_ctrl_mode_didt_START (0)
#define SOC_PCR_CTRL_ctrl_mode_didt_END (1)
#define SOC_PCR_CTRL_ctrl_mode_budget_START (2)
#define SOC_PCR_CTRL_ctrl_mode_budget_END (3)
#define SOC_PCR_CTRL_enable_START (4)
#define SOC_PCR_CTRL_enable_END (4)
#define SOC_PCR_CTRL_snapshot_START (5)
#define SOC_PCR_CTRL_snapshot_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int saving_period : 10;
        unsigned int loading_period_st : 10;
        unsigned int accumulation_period : 10;
        unsigned int reserved : 2;
    } reg;
} SOC_PCR_CFG_PERIOD0_UNION;
#endif
#define SOC_PCR_CFG_PERIOD0_saving_period_START (0)
#define SOC_PCR_CFG_PERIOD0_saving_period_END (9)
#define SOC_PCR_CFG_PERIOD0_loading_period_st_START (10)
#define SOC_PCR_CFG_PERIOD0_loading_period_st_END (19)
#define SOC_PCR_CFG_PERIOD0_accumulation_period_START (20)
#define SOC_PCR_CFG_PERIOD0_accumulation_period_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int loading_period_st_rec : 11;
        unsigned int loading_period_lt : 10;
        unsigned int loading_period_lt_rec : 11;
    } reg;
} SOC_PCR_CFG_PERIOD1_UNION;
#endif
#define SOC_PCR_CFG_PERIOD1_loading_period_st_rec_START (0)
#define SOC_PCR_CFG_PERIOD1_loading_period_st_rec_END (10)
#define SOC_PCR_CFG_PERIOD1_loading_period_lt_START (11)
#define SOC_PCR_CFG_PERIOD1_loading_period_lt_END (20)
#define SOC_PCR_CFG_PERIOD1_loading_period_lt_rec_START (21)
#define SOC_PCR_CFG_PERIOD1_loading_period_lt_rec_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int threshold_budget_svfd0 : 32;
    } reg;
} SOC_PCR_THRESHOLD_BUDGET_SVFD0_UNION;
#endif
#define SOC_PCR_THRESHOLD_BUDGET_SVFD0_threshold_budget_svfd0_START (0)
#define SOC_PCR_THRESHOLD_BUDGET_SVFD0_threshold_budget_svfd0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int threshold_budget_svfd1 : 32;
    } reg;
} SOC_PCR_THRESHOLD_BUDGET_SVFD1_UNION;
#endif
#define SOC_PCR_THRESHOLD_BUDGET_SVFD1_threshold_budget_svfd1_START (0)
#define SOC_PCR_THRESHOLD_BUDGET_SVFD1_threshold_budget_svfd1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int selected_msb_lt : 6;
        unsigned int selected_msb_st : 6;
        unsigned int reserved : 20;
    } reg;
} SOC_PCR_SELECTED_MSB_UNION;
#endif
#define SOC_PCR_SELECTED_MSB_selected_msb_lt_START (0)
#define SOC_PCR_SELECTED_MSB_selected_msb_lt_END (5)
#define SOC_PCR_SELECTED_MSB_selected_msb_st_START (6)
#define SOC_PCR_SELECTED_MSB_selected_msb_st_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_counter : 32;
    } reg;
} SOC_PCR_SVFD_COUNTER_UNION;
#endif
#define SOC_PCR_SVFD_COUNTER_svfd_counter_START (0)
#define SOC_PCR_SVFD_COUNTER_svfd_counter_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int avg_st_counter : 32;
    } reg;
} SOC_PCR_AVG_ST_COUNTER_UNION;
#endif
#define SOC_PCR_AVG_ST_COUNTER_avg_st_counter_START (0)
#define SOC_PCR_AVG_ST_COUNTER_avg_st_counter_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int avg_lt_counter : 32;
    } reg;
} SOC_PCR_AVG_LT_COUNTER_UNION;
#endif
#define SOC_PCR_AVG_LT_COUNTER_avg_lt_counter_START (0)
#define SOC_PCR_AVG_LT_COUNTER_avg_lt_counter_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int didt_counter : 32;
    } reg;
} SOC_PCR_DIDT_COUNTER_UNION;
#endif
#define SOC_PCR_DIDT_COUNTER_didt_counter_START (0)
#define SOC_PCR_DIDT_COUNTER_didt_counter_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int apb_raddr_lt : 10;
        unsigned int apb_raddr_st : 10;
        unsigned int test_mode : 1;
        unsigned int reserved : 11;
    } reg;
} SOC_PCR_SRAM_RADDR_UNION;
#endif
#define SOC_PCR_SRAM_RADDR_apb_raddr_lt_START (0)
#define SOC_PCR_SRAM_RADDR_apb_raddr_lt_END (9)
#define SOC_PCR_SRAM_RADDR_apb_raddr_st_START (10)
#define SOC_PCR_SRAM_RADDR_apb_raddr_st_END (19)
#define SOC_PCR_SRAM_RADDR_test_mode_START (20)
#define SOC_PCR_SRAM_RADDR_test_mode_END (20)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int apb_waddr_lt : 10;
        unsigned int apb_waddr_st : 10;
        unsigned int reserved : 12;
    } reg;
} SOC_PCR_SRAM_WADDR_UNION;
#endif
#define SOC_PCR_SRAM_WADDR_apb_waddr_lt_START (0)
#define SOC_PCR_SRAM_WADDR_apb_waddr_lt_END (9)
#define SOC_PCR_SRAM_WADDR_apb_waddr_st_START (10)
#define SOC_PCR_SRAM_WADDR_apb_waddr_st_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int apb_rdata_st : 32;
    } reg;
} SOC_PCR_RDATA_ST_UNION;
#endif
#define SOC_PCR_RDATA_ST_apb_rdata_st_START (0)
#define SOC_PCR_RDATA_ST_apb_rdata_st_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int apb_rdata_lt : 32;
    } reg;
} SOC_PCR_RDATA_LT_UNION;
#endif
#define SOC_PCR_RDATA_LT_apb_rdata_lt_START (0)
#define SOC_PCR_RDATA_LT_apb_rdata_lt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_clk_sw : 3;
        unsigned int reserved : 29;
    } reg;
} SOC_PCR_CLK_SW_UNION;
#endif
#define SOC_PCR_CLK_SW_pcr_clk_sw_START (0)
#define SOC_PCR_CLK_SW_pcr_clk_sw_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int energy_sum : 32;
    } reg;
} SOC_PCR_POWER_SUM_UNION;
#endif
#define SOC_PCR_POWER_SUM_energy_sum_START (0)
#define SOC_PCR_POWER_SUM_energy_sum_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_intr_stat : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_PCR_INTR_STAT_UNION;
#endif
#define SOC_PCR_INTR_STAT_pcr_intr_stat_START (0)
#define SOC_PCR_INTR_STAT_pcr_intr_stat_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_intr_raw_stat : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_PCR_INTR_RAW_STAT_UNION;
#endif
#define SOC_PCR_INTR_RAW_STAT_pcr_intr_raw_stat_START (0)
#define SOC_PCR_INTR_RAW_STAT_pcr_intr_raw_stat_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_intr_en : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_PCR_INTR_EN_UNION;
#endif
#define SOC_PCR_INTR_EN_pcr_intr_en_START (0)
#define SOC_PCR_INTR_EN_pcr_intr_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_intr_clr : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_PCR_INTR_CLR_UNION;
#endif
#define SOC_PCR_INTR_CLR_pcr_intr_clr_START (0)
#define SOC_PCR_INTR_CLR_pcr_intr_clr_END (0)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
