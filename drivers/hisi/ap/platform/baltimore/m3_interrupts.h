#ifndef __LPMCU_INTERRUPTS_H__
#define __LPMCU_INTERRUPTS_H__ 
#define IRQ_INTR_BIG_PMUIRQ_MIDDLE_LITTLE 0
#define IRQ_RTC0 1
#define IRQ_RTC1 2
#define IRQ_CCPUPLL_DFS_UNLOCK_INT 3
#define IRQ_INTR_IVP32_WATCH_DOG_IVP1 4
#define IRQ_NVIC5_NSRM 5
#define IRQ_NVIC6_NSRM 6
#define IRQ_APWDOG 8
#define IRQ_INTR_WD1 9
#define IRQ_NVIC10_COMBO 10
    #define PIE10_IRQ_INTR_TIMER00 (0U)
    #define PIE10_IRQ_INTR_TIMER01 (1U)
    #define PIE10_IRQ_INTR_TIMER10 (2U)
    #define PIE10_IRQ_INTR_TIMER11 (3U)
    #define PIE10_IRQ_INTR_TIMER20 (4U)
    #define PIE10_IRQ_INTR_TIMER21 (5U)
    #define PIE10_IRQ_INTR_TIMER30 (6U)
    #define PIE10_IRQ_INTR_TIMER31 (7U)
    #define PIE10_IRQ_TIME40 (8U)
    #define PIE10_IRQ_INTR_TIMER41 (9U)
    #define PIE10_IRQ_INTR_TIMER50 (10U)
    #define PIE10_IRQ_INTR_TIMER51 (11U)
    #define PIE10_IRQ_INTR_TIMER60 (12U)
    #define PIE10_IRQ_INTR_TIMER61 (13U)
    #define PIE10_IRQ_INTR_TIMER70 (14U)
    #define PIE10_IRQ_INTR_TIMER71 (15U)
    #define PIE10_IRQ_INTR_TIMER80 (16U)
    #define PIE10_IRQ_INTR_TIMER81 (17U)
    #define PIE10_IRQ_INTR_TIMER130 (18U)
    #define PIE10_IRQ_INTR_TIMER131 (19U)
    #define PIE10_IRQ_INTR_TIMER140 (20U)
    #define PIE10_IRQ_INTR_TIMER141 (21U)
    #define PIE10_IRQ_INTR_TIMER150 (22U)
    #define PIE10_IRQ_INTR_TIMER151 (23U)
    #define PIE10_IRQ_INTR_TIMER160 (24U)
    #define PIE10_IRQ_INTR_TIMER161 (25U)
    #define PIE10_IRQ_INTR_TIMER170 (26U)
    #define PIE10_IRQ_INTR_TIMER171 (27U)
#define IRQ_NVIC11_COMBO 11
    #define PIE11_IRQ_INTR_TIMER00 (0U)
    #define PIE11_IRQ_INTR_TIMER01 (1U)
    #define PIE11_IRQ_INTR_TIMER10 (2U)
    #define PIE11_IRQ_INTR_TIMER11 (3U)
    #define PIE11_IRQ_INTR_TIMER20 (4U)
    #define PIE11_IRQ_INTR_TIMER21 (5U)
    #define PIE11_IRQ_INTR_TIMER30 (6U)
    #define PIE11_IRQ_INTR_TIMER31 (7U)
    #define PIE11_IRQ_INTR_TIMER40 (8U)
    #define PIE11_IRQ_INTR_TIMER41 (9U)
    #define PIE11_IRQ_INTR_TIMER50 (10U)
    #define PIE11_IRQ_INTR_TIMER51 (11U)
    #define PIE11_IRQ_INTR_TIMER60 (12U)
    #define PIE11_IRQ_INTR_TIMER61 (13U)
    #define PIE11_IRQ_INTR_TIMER70 (14U)
    #define PIE11_IRQ_INTR_TIMER71 (15U)
    #define PIE11_IRQ_INTR_TIMER80 (16U)
    #define PIE11_IRQ_INTR_TIMER81 (17U)
    #define PIE11_IRQ_INTR_TIMER130 (18U)
    #define PIE11_IRQ_INTR_TIMER131 (19U)
    #define PIE11_IRQ_INTR_TIMER140 (20U)
    #define PIE11_IRQ_INTR_TIMER141 (21U)
    #define PIE11_IRQ_INTR_TIMER150 (22U)
    #define PIE11_IRQ_INTR_TIMER151 (23U)
    #define PIE11_IRQ_INTR_TIMER160 (24U)
    #define PIE11_IRQ_INTR_TIMER161 (25U)
    #define PIE11_IRQ_INTR_TIMER170 (26U)
    #define PIE11_IRQ_INTR_TIMER171 (27U)
#define IRQ_NVIC12_COMBO 12
    #define PIE12_IRQ_INTR_TIMER00 (0U)
    #define PIE12_IRQ_INTR_TIMER01 (1U)
    #define PIE12_IRQ_INTR_TIMER10 (2U)
    #define PIE12_IRQ_INTR_TIMER11 (3U)
    #define PIE12_IRQ_INTR_TIMER20 (4U)
    #define PIE12_IRQ_INTR_TIMER21 (5U)
    #define PIE12_IRQ_INTR_TIMER30 (6U)
    #define PIE12_IRQ_INTR_TIMER31 (7U)
    #define PIE12_IRQ_INTR_TIMER40 (8U)
    #define PIE12_IRQ_INTR_TIMER41 (9U)
    #define PIE12_IRQ_INTR_TIMER50 (10U)
    #define PIE12_IRQ_INTR_TIMER51 (11U)
    #define PIE12_IRQ_INTR_TIMER60 (12U)
    #define PIE12_IRQ_INTR_TIMER61 (13U)
    #define PIE12_IRQ_INTR_TIMER70 (14U)
    #define PIE12_IRQ_INTR_TIMER71 (15U)
    #define PIE12_IRQ_INTR_TIMER80 (16U)
    #define PIE12_IRQ_INTR_TIMER81 (17U)
    #define PIE12_IRQ_INTR_TIMER130 (18U)
    #define PIE12_IRQ_INTR_TIMER131 (19U)
    #define PIE12_IRQ_INTR_TIMER140 (20U)
    #define PIE12_IRQ_INTR_TIMER141 (21U)
    #define PIE12_IRQ_INTR_TIMER150 (22U)
    #define PIE12_IRQ_INTR_TIMER151 (23U)
    #define PIE12_IRQ_INTR_TIMER160 (24U)
    #define PIE12_IRQ_INTR_TIMER161 (25U)
    #define PIE12_IRQ_INTR_TIMER170 (26U)
    #define PIE12_IRQ_INTR_TIMER171 (27U)
#define IRQ_NVIC13_COMBO 13
    #define PIE13_IRQ_INTR_TIMER00 (0U)
    #define PIE13_IRQ_INTR_TIMER01 (1U)
    #define PIE13_IRQ_INTR_TIMER10 (2U)
    #define PIE13_IRQ_INTR_TIMER11 (3U)
    #define PIE13_IRQ_INTR_TIMER20 (4U)
    #define PIE13_IRQ_INTR_TIMER21 (5U)
    #define PIE13_IRQ_INTR_TIMER30 (6U)
    #define PIE13_IRQ_INTR_TIMER31 (7U)
    #define PIE13_IRQ_INTR_TIMER40 (8U)
    #define PIE13_IRQ_INTR_TIMER41 (9U)
    #define PIE13_IRQ_INTR_TIMER50 (10U)
    #define PIE13_IRQ_INTR_TIMER51 (11U)
    #define PIE13_IRQ_INTR_TIMER60 (12U)
    #define PIE13_IRQ_INTR_TIMER61 (13U)
    #define PIE13_IRQ_INTR_TIMER70 (14U)
    #define PIE13_IRQ_INTR_TIMER71 (15U)
    #define PIE13_IRQ_INTR_TIMER80 (16U)
    #define PIE13_IRQ_INTR_TIMER81 (17U)
    #define PIE13_IRQ_INTR_TIMER130 (18U)
    #define PIE13_IRQ_INTR_TIMER131 (19U)
    #define PIE13_IRQ_INTR_TIMER140 (20U)
    #define PIE13_IRQ_INTR_TIMER141 (21U)
    #define PIE13_IRQ_INTR_TIMER150 (22U)
    #define PIE13_IRQ_INTR_TIMER151 (23U)
    #define PIE13_IRQ_INTR_TIMER160 (24U)
    #define PIE13_IRQ_INTR_TIMER161 (25U)
    #define PIE13_IRQ_INTR_TIMER170 (26U)
    #define PIE13_IRQ_INTR_TIMER171 (27U)
#define IRQ_NVIC14_COMBO 14
    #define PIE14_IRQ_INTR_GPIO_CPU_0 (0U)
    #define PIE14_IRQ_INTR_GPIO_CPU_1 (1U)
    #define PIE14_IRQ_INTR_GPIO_CPU_2 (2U)
    #define PIE14_IRQ_INTR_GPIO_CPU_3 (3U)
    #define PIE14_IRQ_INTR_GPIO_CPU_4 (4U)
    #define PIE14_IRQ_INTR_GPIO_CPU_5 (5U)
    #define PIE14_IRQ_INTR_GPIO_CPU_6 (6U)
    #define PIE14_IRQ_INTR_GPIO_CPU_7 (7U)
    #define PIE14_IRQ_INTR_GPIO_CPU_8 (8U)
    #define PIE14_IRQ_INTR_GPIO_CPU_9 (9U)
    #define PIE14_IRQ_INTR_GPIO_CPU_10 (10U)
    #define PIE14_IRQ_INTR_GPIO_CPU_11 (11U)
    #define PIE14_IRQ_INTR_GPIO_CPU_12 (12U)
    #define PIE14_IRQ_INTR_GPIO_CPU_13 (13U)
    #define PIE14_IRQ_INTR_GPIO_CPU_14 (14U)
    #define PIE14_IRQ_INTR_GPIO_CPU_20 (15U)
    #define PIE14_IRQ_INTR_GPIO_CPU_21 (16U)
    #define PIE14_IRQ_INTR_GPIO_CPU_22 (17U)
    #define PIE14_IRQ_INTR_GPIO_CPU_23 (18U)
    #define PIE14_IRQ_INTR_GPIO_CPU_24 (19U)
    #define PIE14_IRQ_INTR_GPIO_CPU_25 (20U)
    #define PIE14_IRQ_INTR_GPIO_CPU_26 (21U)
    #define PIE14_IRQ_INTR_GPIO_CPU_27 (22U)
    #define PIE14_IRQ_INTR_GPIO_CPU_28 (23U)
    #define PIE14_IRQ_INTR_GPIO_CPU_29 (24U)
    #define PIE14_IRQ_INTR_GPIO_CPU_30 (25U)
    #define PIE14_IRQ_INTR_GPIO_CPU_31 (26U)
    #define PIE14_IRQ_INTR_GPIO_CPU_32 (27U)
    #define PIE14_IRQ_INTR_GPIO_CPU_33 (28U)
    #define PIE14_IRQ_INTR_GPIO_CPU_34 (29U)
    #define PIE14_IRQ_INTR_GPIO_CPU_35 (30U)
    #define PIE14_IRQ_INTR_GPIO_CPU_36 (31U)
#define IRQ_NVIC15_COMBO 15
    #define PIE15_IRQ_INTR_GPIO_LPMCU_0 (0U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_1 (1U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_2 (2U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_3 (3U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_4 (4U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_5 (5U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_6 (6U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_7 (7U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_8 (8U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_9 (9U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_10 (10U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_11 (11U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_12 (12U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_13 (13U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_14 (14U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_20 (15U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_21 (16U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_22 (17U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_23 (18U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_24 (19U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_25 (20U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_26 (21U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_27 (22U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_28 (23U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_29 (24U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_30 (25U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_31 (26U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_32 (27U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_33 (28U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_34 (29U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_35 (30U)
    #define PIE15_IRQ_INTR_GPIO_LPMCU_36 (31U)
#define IRQ_NVIC16_COMBO 16
    #define PIE16_IRQ_INTR_GPIO_MDM_28 (0U)
    #define PIE16_IRQ_INTR_GPIO_MDM_36 (1U)
#define IRQ_NVIC17_COMBO 17
    #define PIE17_IRQ_INTR_GPIO_MDM_0 (0U)
    #define PIE17_IRQ_INTR_GPIO_MDM_1 (1U)
    #define PIE17_IRQ_INTR_GPIO_MDM_2 (2U)
    #define PIE17_IRQ_INTR_GPIO_MDM_3 (3U)
    #define PIE17_IRQ_INTR_GPIO_MDM_4 (4U)
    #define PIE17_IRQ_INTR_GPIO_MDM_5 (5U)
    #define PIE17_IRQ_INTR_GPIO_MDM_6 (6U)
    #define PIE17_IRQ_INTR_GPIO_MDM_7 (7U)
    #define PIE17_IRQ_INTR_GPIO_MDM_8 (8U)
    #define PIE17_IRQ_INTR_GPIO_MDM_9 (9U)
    #define PIE17_IRQ_INTR_GPIO_MDM_10 (10U)
    #define PIE17_IRQ_INTR_GPIO_MDM_11 (11U)
    #define PIE17_IRQ_INTR_GPIO_MDM_12 (12U)
    #define PIE17_IRQ_INTR_GPIO_MDM_13 (13U)
    #define PIE17_IRQ_INTR_GPIO_MDM_14 (14U)
    #define PIE17_IRQ_INTR_GPIO_MDM_20 (15U)
    #define PIE17_IRQ_INTR_GPIO_MDM_21 (16U)
    #define PIE17_IRQ_INTR_GPIO_MDM_22 (17U)
    #define PIE17_IRQ_INTR_GPIO_MDM_23 (18U)
    #define PIE17_IRQ_INTR_GPIO_MDM_24 (19U)
    #define PIE17_IRQ_INTR_GPIO_MDM_25 (20U)
    #define PIE17_IRQ_INTR_GPIO_MDM_26 (21U)
    #define PIE17_IRQ_INTR_GPIO_MDM_27 (22U)
    #define PIE17_IRQ_INTR_GPIO_MDM_29 (24U)
    #define PIE17_IRQ_INTR_GPIO_MDM_30 (25U)
    #define PIE17_IRQ_INTR_GPIO_MDM_31 (26U)
    #define PIE17_IRQ_INTR_GPIO_MDM_32 (27U)
    #define PIE17_IRQ_INTR_GPIO_MDM_33 (28U)
    #define PIE17_IRQ_INTR_GPIO_MDM_34 (29U)
    #define PIE17_IRQ_INTR_GPIO_MDM_35 (30U)
#define IRQ_NVIC18_COMBO 18
    #define PIE18_IRQ_INTR_GPIO_MDM5G_0 (0U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_1 (1U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_2 (2U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_3 (3U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_4 (4U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_5 (5U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_6 (6U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_7 (7U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_8 (8U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_9 (9U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_10 (10U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_11 (11U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_12 (12U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_13 (13U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_14 (14U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_20 (15U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_21 (16U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_22 (17U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_23 (18U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_24 (19U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_25 (20U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_26 (21U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_27 (22U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_29 (24U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_30 (25U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_31 (26U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_32 (27U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_33 (28U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_34 (29U)
    #define PIE18_IRQ_INTR_GPIO_MDM5G_35 (30U)
#define IRQ_NVIC19_COMBO 19
    #define PIE19_IRQ_LTE2_DRX_ARM_TIMES_INT (0U)
    #define PIE19_IRQ_CBBP_INT01_2 (1U)
    #define PIE19_IRQ_W_ARM_INT03 (2U)
    #define PIE19_IRQ_W_ARM_INT02 (3U)
    #define PIE19_IRQ_W_ARM_INT03_2 (4U)
    #define PIE19_IRQ_W_ARM_INT02_2 (5U)
    #define PIE19_IRQ_LTE2_DRX_ARM_WAKEUP_INT (6U)
    #define PIE19_IRQ_G3_INT_GBBP_TO_CPU_ON (7U)
    #define PIE19_IRQ_G2_INT_GBBP_TO_CPU_ON (8U)
    #define PIE19_IRQ_G1_INT_GBBP_TO_CPU_ON (9U)
    #define PIE19_IRQ_TDS_DRX_ARM_TIMES_INT (10U)
    #define PIE19_IRQ_TDS_DRX_ARM_WAKEUP_INT (11U)
    #define PIE19_IRQ_LTE_DRX_ARM_WAKEUP_INT (12U)
    #define PIE19_IRQ_LTE_DRX_ARM_TIMES_INT (13U)
    #define PIE19_IRQ_LTEV_DRX_ARM_WAKEUP_INT (14U)
    #define PIE19_IRQ_LTEV_DRX_ARM_TIMES_INT (15U)
    #define PIE19_IRQ_NR_DRX_ARM_WAKEUP_INT (16U)
    #define PIE19_IRQ_NR_DRX_ARM_TIMES_INT (17U)
    #define PIE19_IRQ_NR_DRX_ARM_4G_WAKEUP_INT (18U)
#define IRQ_NVIC20_COMBO 20
    #define PIE20_IRQ_LTE2_DRX_ARM_TIMES_INT (0U)
    #define PIE20_IRQ_CBBP_INT01_2 (1U)
    #define PIE20_IRQ_W_ARM_INT03 (2U)
    #define PIE20_IRQ_W_ARM_INT02 (3U)
    #define PIE20_IRQ_W_ARM_INT03_2 (4U)
    #define PIE20_IRQ_W_ARM_INT02_2 (5U)
    #define PIE20_IRQ_LTE2_DRX_ARM_WAKEUP_INT (6U)
    #define PIE20_IRQ_G3_INT_GBBP_TO_CPU_ON (7U)
    #define PIE20_IRQ_G2_INT_GBBP_TO_CPU_ON (8U)
    #define PIE20_IRQ_G1_INT_GBBP_TO_CPU_ON (9U)
    #define PIE20_IRQ_TDS_DRX_ARM_TIMES_INT (10U)
    #define PIE20_IRQ_TDS_DRX_ARM_WAKEUP_INT (11U)
    #define PIE20_IRQ_LTE_DRX_ARM_WAKEUP_INT (12U)
    #define PIE20_IRQ_LTE_DRX_ARM_TIMES_INT (13U)
    #define PIE20_IRQ_LTEV_DRX_ARM_WAKEUP_INT (14U)
    #define PIE20_IRQ_LTEV_DRX_ARM_TIMES_INT (15U)
    #define PIE20_IRQ_NR_DRX_ARM_WAKEUP_INT (16U)
    #define PIE20_IRQ_NR_DRX_ARM_TIMES_INT (17U)
    #define PIE20_IRQ_NR_DRX_ARM_4G_WAKEUP_INT (18U)
#define IRQ_NVIC21_COMBO 21
    #define PIE21_IRQ_INTR_HISEE_ALARM0 (0U)
    #define PIE21_IRQ_INTR_HISEE_ALARM1 (1U)
    #define PIE21_IRQ_INTR_HISEE_AS2AP_IRQ (2U)
    #define PIE21_IRQ_INTR_HISEE_DS2AP_IRQ (3U)
    #define PIE21_IRQ_INTR_HISEE_SENC2AP_IRQ0 (4U)
    #define PIE21_IRQ_INTR_HISEE_SENC2AP_IRQ1 (5U)
    #define PIE21_IRQ_INTR_HISEE_TSENSOR0 (6U)
    #define PIE21_IRQ_INTR_HISEE_TSENSOR1 (7U)
    #define PIE21_IRQ_INTR_HISEE_LOCKUP (8U)
#define IRQ_NVIC22_COMBO 22
    #define PIE22_IRQ_INTR_IP0 (0U)
    #define PIE22_IRQ_INTR_IP1 (1U)
    #define PIE22_IRQ_INTR_IP2 (2U)
    #define PIE22_IRQ_INTR_MAA1 (3U)
    #define PIE22_IRQ_MDM4G_WAKEUP_EICC_MERG (4U)
    #define PIE22_IRQ_IPCM_CCPU_INT0 (5U)
    #define PIE22_IRQ_IPCM_CCPU_INT1 (6U)
    #define PIE22_IRQ_MDM_TIMER0_INT (7U)
    #define PIE22_IRQ_MDM_TIMER1_INT (8U)
    #define PIE22_IRQ_MDM_TIMER16_INT (9U)
    #define PIE22_IRQ_MDM_TIMER17_INT (10U)
    #define PIE22_IRQ_MDM_TIMER18_INT (11U)
    #define PIE22_IRQ_MDM_TIMER19_INT (12U)
    #define PIE22_IRQ_MDM_BUS_ERR_INT (13U)
    #define PIE22_IRQ_MDM_IPCM_CDSP_MERG (14U)
    #define PIE22_IRQ_MDM_IPCM_DSP0_MERG (15U)
    #define PIE22_IRQ_CCPU4G_EICC_INT30 (16U)
    #define PIE22_IRQ_LL1C_EICC0_INT_CCPU4G10 (17U)
    #define PIE22_IRQ_LL1C_EICC1_INT_CCPU4G10 (18U)
    #define PIE22_IRQ_L2HAC_EICC0_INT_CCPU4G10 (19U)
    #define PIE22_IRQ_L2HAC_EICC1_INT_CCPU4G10 (20U)
    #define PIE22_IRQ_CCPU5G_EICC0_INT_CCPU4G10 (21U)
    #define PIE22_IRQ_INTR_MAA6 (22U)
    #define PIE22_IRQ_INTR_MAA9 (23U)
    #define PIE22_IRQ_MDM5G_IPC_CCPU4G_INT10 (24U)
    #define PIE22_IRQ_MODEMTOP2AP_WAKEUP_RSV30 (25U)
    #define PIE22_IRQ_CCPU5G_EICC1_INT_CCPU4G10 (26U)
    #define PIE22_IRQ_INTR_MAA11 (27U)
#define IRQ_NVIC23_COMBO 23
    #define PIE23_IRQ_INTR_IP0 (0U)
    #define PIE23_IRQ_INTR_IP1 (1U)
    #define PIE23_IRQ_INTR_IP2 (2U)
    #define PIE23_IRQ_INTR_SOCP2 (3U)
    #define PIE23_IRQ_INTR_MAA1 (4U)
    #define PIE23_IRQ_INTR_MAA3 (5U)
    #define PIE23_IRQ_INTR_MAA4 (6U)
    #define PIE23_IRQ_CCPU5G_TIMER_INTR0 (7U)
    #define PIE23_IRQ_CCPU5G_TIMER_INTR1 (8U)
    #define PIE23_IRQ_CCPU5G_TIMER_INTR2 (9U)
    #define PIE23_IRQ_CCPU5G_TIMER_INTR3 (10U)
    #define PIE23_IRQ_CCPU5G_TIMER_INTR4 (11U)
    #define PIE23_IRQ_CCPU5G_TIMER_INTR5 (12U)
    #define PIE23_IRQ_CCPU5G_TIMER_INTR6 (13U)
    #define PIE23_IRQ_CCPU5G_TIMER_INTR7 (14U)
    #define PIE23_IRQ_CCPU5G_TIMER_INTR8 (15U)
    #define PIE23_IRQ_CCPU5G_TIMER_INTR9 (16U)
    #define PIE23_IRQ_CCPU5G_TIMER_INTR10 (17U)
    #define PIE23_IRQ_CCPU5G_TIMER_INTR11 (18U)
    #define PIE23_IRQ__LL1C_EICC1_INTR10 (19U)
    #define PIE23_IRQ__LL1C_EICC0_INTR10 (20U)
    #define PIE23_IRQ__L2HAC_EICC_INTR10 (21U)
    #define PIE23_IRQ__L2HAC_EICC1_INTR10 (22U)
    #define PIE23_IRQ__CCPU5G_EICC_INTR30 (23U)
    #define PIE23_IRQ_INTR_MAA7 (24U)
    #define PIE23_IRQ_INTR_MAA8 (25U)
    #define PIE23_IRQ_INTR_MAA11 (27U)
#define IRQ_NVIC24_COMBO 24
    #define PIE24_IRQ__CCPU5G_EICC1_INT30 (0U)
    #define PIE24_IRQ__MDM5G_IPC_CCPU5G_INT (1U)
    #define PIE24_IRQ__MDM5G_IPC_L2HAC_INT (2U)
    #define PIE24_IRQ_CCPU5G_CCPU_IRQS_248 (3U)
    #define PIE24_IRQ_CCPU5G_CCPU_IRQS_143 (4U)
    #define PIE24_IRQ_CCPU5G_CCPU_IRQS_249 (5U)
    #define PIE24_IRQ_CCPU5G_CCPU_IRQS_250 (6U)
    #define PIE24_IRQ_MODEM4G_IPC_LL1C_INT (7U)
    #define PIE24_IRQ_MODEM4G_IPC_CCPU5G_INT0 (8U)
    #define PIE24_IRQ_MODEM4G_IPC_CCPU5G_INT1 (9U)
    #define PIE24_IRQ__MODEM4G_EICC_INT_CCPU5G10 (10U)
    #define PIE24_IRQ_CIPHER_INTR1 (11U)
    #define PIE24_IRQ_MODEMTOP2AP_CCPU5GWAKEUP_RSV_INT0 (12U)
    #define PIE24_IRQ_MODEMTOP2AP_CCPU5GWAKEUP_RSV_INT1 (13U)
    #define PIE24_IRQ_MODEMTOP2AP_CCPU5GWAKEUP_RSV_INT2 (14U)
    #define PIE24_IRQ_MODEMTOP2AP_CCPU5GWAKEUP_RSV_INT3 (15U)
    #define PIE24_IRQ_CCPU5G_CCPU_IRQS_135 (16U)
#define IRQ_NVIC25_COMBO 25
    #define PIE25_IRQ_INTR_ANANKE_CPU_DVFS_EVENT_UP (0U)
    #define PIE25_IRQ_INTR_ANANKE_CPU_DVFS_EVENT_DOWN (1U)
    #define PIE25_IRQ_INTR_MID_CPU_DVFS_EVENT_UP (2U)
    #define PIE25_IRQ_INTR_MID_CPU_DVFS_EVENT_DOWN (3U)
    #define PIE25_IRQ_INTR_BIG_CPU_DVFS_EVENT_UP (4U)
    #define PIE25_IRQ_INTR_BIG_CPU_DVFS_EVENT_DOWN (5U)
    #define PIE25_IRQ_INTR_FCM_CPU_DVFS_EVENT_UP (6U)
    #define PIE25_IRQ_INTR_FCM_CPU_DVFS_EVENT_DOWN (7U)
    #define PIE25_IRQ_INTR_ANANKE_L3_DVFS_EVENT_UP (8U)
    #define PIE25_IRQ_INTR_ANANKE_L3_DVFS_EVENT_DOWN (9U)
    #define PIE25_IRQ_INTR_MID_L3_DVFS_EVENT_UP (10U)
    #define PIE25_IRQ_INTR_MID_L3_DVFS_EVENT_DOWN (11U)
    #define PIE25_IRQ_INTR_BIG_L3_DVFS_EVENT_UP (12U)
    #define PIE25_IRQ_INTR_BIG_L3_DVFS_EVENT_DOWN (13U)
    #define PIE25_IRQ_INTR_FCM_L3_DVFS_EVENT_UP (14U)
    #define PIE25_IRQ_INTR_FCM_L3_DVFS_EVENT_DOWN (15U)
    #define PIE25_IRQ_INTR_ANANKE_DDR_DVFS_EVENT_UP (16U)
    #define PIE25_IRQ_INTR_ANANKE_DDR_DVFS_EVENT_DOWN (17U)
    #define PIE25_IRQ_INTR_MID_DDR_DVFS_EVENT_UP (18U)
    #define PIE25_IRQ_INTR_MID_DDR_DVFS_EVENT_DOWN (19U)
    #define PIE25_IRQ_INTR_BIG_DDR_DVFS_EVENT_UP (20U)
    #define PIE25_IRQ_INTR_BIG_DDR_DVFS_EVENT_DOWN (21U)
    #define PIE25_IRQ_INTR_FCM_DDR_DVFS_EVENT_UP (22U)
    #define PIE25_IRQ_INTR_FCM_DDR_DVFS_EVENT_DOWN (23U)
    #define PIE25_IRQ_INTR_HSDT0_PCIE0_TRACE_INT (25U)
    #define PIE25_IRQ_INTR_HSDT1_PCIE1_TRACE_INT (27U)
    #define PIE25_IRQ_INTR_WAKEUP_SWING_ENTER_REQ (30U)
    #define PIE25_IRQ_INTR_WAKEUP_SWING_EXIT_REQ (31U)
#define IRQ_INTR_QIC_TO_GIC_LPMCU_COMB 26
#define IRQ_INTR_PCIE0_L12_WAKE_INT 27
#define IRQ_INTR_I2C2 28
#define IRQ_INTR_PERI_DVS_STAT_PMC2LPM3 29
#define IRQ_INTR_AO_WD1 30
#define IRQ_INTR_IPC_IVP32_IVP1_OR 31
#define IRQ_INTR_IPC_CCPU5G_OR_INTR_IPC_AO_CCPU5G_OR 32
#define IRQ_INTR_ALINK_MST1 33
#define IRQ_INTR_NPU2LPMCU_OVER_CURRENT 34
#define IRQ_INTR_IPC_ARPP2ACPU_OR 35
#define IRQ_INTR_HISEE_IPC_MBX_LPMCU0 36
#define IRQ_INTR_HISEE_IPC_MBX_LPMCU1 37
#define IRQ_UART0 38
#define IRQ_GPIO0_SE_INTR2 39
#define IRQ_GPIO1_SE_INTR2 40
#define IRQ_INTR_SYSCACHE_COMB 41
#define IRQ_INTR_UART5 42
#define IRQ_UART6 43
#define IRQ_INTR_UCE0_IPC 44
#define IRQ_INTR_UCE0_IPC_MBX 45
#define IRQ_INTR_UCE1_IPC 46
#define IRQ_INTR_UCE1_IPC_MBX 47
#define IRQ_INTR_UCE2_IPC 48
#define IRQ_INTR_UCE2_IPC_MBX 49
#define IRQ_INTR_UCE3_IPC 50
#define IRQ_INTR_UCE3_IPC_MBX 51
#define IRQ_INTR_ALINK_MST0 52
#define IRQ_RF0_WDOG_INT 53
#define IRQ_INTR_ATGC1 54
#define IRQ_INTR_REPAIR_ERR_PERI 55
#define IRQ_RF1_WDOG_INT 56
#define IRQ_INTR_HISEE_IPC_MDM5G 57
#define IRQ_INTR_HISEE_IPC_MBX_MDM5G 58
#define IRQ_INTR_PCIE0_100MS_IDLE_TIMEOUT 59
#define IRQ_INTR_UCE0123_WDT 60
#define IRQ_DDRC_INBAND_ECC_ERR_INT_COMB 61
#define IRQ_DDRC_ERR_INT_COMB 62
#define IRQ_DDRPHY_INT_COMB 63
#define IRQ_EDMAC2_MDM_INTR_S_BIT2 64
#define IRQ_EDMAC2_MDM_INTR_NS_BIT2 65
#define IRQ_INTR_TS_WDOG_INT_TS 66
#define IRQ_UCE_RTACTIVE0 67
#define IRQ_INTR_TS_WDOG_RES_TS 68
#define IRQ_DDRC_FATAL_INT_COMB 69
#define IRQ_MDM_CCPU_WDT 76
#define IRQ_IOMCU_WDT 77
#define IRQ_IOMCU_WAKEUP 78
#define IRQ_MDM_IPCM_GIC_COMB 79
#define IRQ_MDM_IPCM_CCPU_COMB 80
#define IRQ_INTR_MODEM_IPC0_S2 81
#define IRQ_INTR_MEDIA1_COMB 82
#define IRQ_MDM_IPCM_HIFI_COMB 83
#define IRQ_MDM_IPCM_CDSP_COMB 84
#define IRQ_INTR_MODEM_IPC1_S2 85
#define IRQ_CCPU5G_WDT_INT0 86
#define IRQ_INTR_HISEE_RST 87
#define IRQ_INTR_CPU_LITTLE_PWR 88
#define IRQ_CCPU5G_WDT_INT1 89
#define IRQ_BUS_DEC_ERR_INT 90
#define IRQ_INTR_HISEE_WDOG 91
#define IRQ_INTR_DDRCA_RESET_COMB 92
#define IRQ_INTR_PCIE1_LINK_DOWN 93
#define IRQ_INTR_PCIE1_RADMA 94
#define IRQ_INTR_PERF_STAT 95
#define IRQ_INTR_ISP_A7_TO_MCU_MBX_INT0 96
#define IRQ_INTR_WAKEUP_SCTRL2M3_STAT 97
#define IRQ_INTR_ISP_A7_TO_MCU_IPC_INT 98
#define IRQ_INTR_ISP_A7_WATCHDOG 99
#define IRQ_INTR_NOC_COMB_ALL 100
#define IRQ_INTR_DMSS 101
#define IRQ_INTR_DMSS_NORMAL 102
#define IRQ_INTR_AO_WD 103
#define IRQ_INTR_PMC 104
#define IRQ_INTR_UFS 105
#define IRQ_INTR_SPMI0 106
#define IRQ_INTR_SPMI10 107
#define IRQ_INTR_SECP 108
#define IRQ_INTR_SECS 109
#define IRQ_INTR_SDIO 110
#define IRQ_INTR_PCIE0_LINK_DOWN 111
#define IRQ_INTR_TSEN 112
#define IRQ_INTR_PCIE0_RADMA 113
#define IRQ_INTR_LPMCU_PCTRL_TIMEOUT 114
#define IRQ_INTR_DMA2 115
#define IRQ_INTR_DMA_NS2 116
#define IRQ_PROBHIT 117
#define IRQ_INTR_HIEPS2AP_ALARM 118
#define IRQ_INTR_HIEPS2AP_WD_RST_REQ 119
#define IRQ_INTR_HIEPS2MDM_IPC_MBOX 120
#define IRQ_INTR_HIEPS2MDM_IPC 121
#define IRQ_INTR_HIEPS2LPMCU_IPC_MBOX 122
#define IRQ_INTR_HIEPS2LPMCU_IPC 123
#define IRQ_INTR_CPU_BIG_PWR 124
#define IRQ_INTR_CPU_MIDDLE_PWR 125
#define IRQ_INTR_IOMCU_TCP_TASK_DONE_IRQ_2_LPM3 126
#define IRQ_INTR_IOMCU_TCP_ERR_IRQ_2_LPM3 127
#define IRQ_INTR_IOMCU_TCP_SOFT_IRQ2_LPM3 128
#define IRQ_INTR_DSS_MCU_PDP 129
#define IRQ_INTR_MEDIA1_TRACE 130
#define IRQ_INTR_MEDIA2_TRACE 131
#define IRQ_INTR_MODEM_IPC02 133
#define IRQ_MDM5G_IPC_CM3_INT0 134
#define IRQ_MDM5G_IPC_CM3_INT1 135
#define IRQ_INTR_MODEM_IPC12 136
#define IRQ_INTR_LAT_MON 137
#define IRQ_MDM_IPC_TLDSP_COMB 138
#define IRQ_MDM_BUS_ERR_INT 139
#define IRQ_INTR_USB30 140
#define IRQ_INTR_USB31 141
#define IRQ_INTR_USB3_OTG 142
#define IRQ_INTR_USB3_PME_GENERATION 143
#define IRQ_INTR_DVFS_STAT_PMC2LPM3_0 144
#define IRQ_LAT_STAT_INT 145
#define IRQ_INTR_DVFS_STAT_PMC2LPM3_1 146
#define IRQ_INTR_ASP_POWERUP 147
#define IRQ_INTR_ASP_POWERDN 148
#define IRQ_VBAT_DROP_PROTECT_DB 149
#define IRQ_INTR_NPU2AP_RST_REQ 150
#define IRQ_INTR_NPU2LPMCU_IPC_NS 151
#define IRQ__INTR_NPU2LPMCU_IPC_NS_MBOX10 152
#define IRQ_WATCHDOG 153
#define IRQ_INTR_NPU2LPMCU_IPC_S 154
#define IRQ_IPC_S_INT3 155
#define IRQ_INTR_IPC4 156
#define IRQ__INTR_NPU2LPMCU_IPC_S_MBOX10 157
#define IRQ_IPC_S_MBX13 158
#define IRQ_IPC_S_MBX14 159
#define IRQ_IPC_S_MBX15 160
#define IRQ_IPC_S_MBX16 161
#define IRQ_IPC_S_MBX17 162
#define IRQ_IPC_S_MBX18 163
#define IRQ_INTR_CPU_APS_DPM_COMB 164
#define IRQ_IPC_S_MBX23 165
#define IRQ_IPC_S_MBX24 166
#define IRQ_IPC_NS_INT3 167
#define IRQ_IPC_NS_INT4 168
#define IRQ_AO_IPC_S_MBX4 169
#define IRQ_AO_IPC_S_MBX5 170
#define IRQ_IPC_NS_MBX13 171
#define IRQ_IPC_NS_MBX14 172
#define IRQ_IPC_NS_MBX15 173
#define IRQ_IPC_NS_MBX16 174
#define IRQ_IPC_NS_MBX17 175
#define IRQ_IPC_NS_MBX18 176
#define IRQ_AO_IPC_S_INT1 177
#define IRQ_IPC_R8_INT_COMB 178
#define IRQ_IPC_NS_MBX27 179
#define IRQ_IPC_NS_MBX28 180
#define IRQ_IPC_GIC_INT_COMB 181
#define IRQ_IPC_IOMCU_INT_COMB 182
#define IRQ_IPC_IVP_INT_COMB 183
#define IRQ_INTR_HISEE_IPC_MBX_MDM0 184
#define IRQ_ASPIPC 185
#define IRQ_AO_IPC_S_ASP_INT_COMB 186
#define IRQ_INTR_HISEE_IPC_MBX_MDM1 187
#define IRQ_NSRM 188
#define IRQ_INTR_I3C4 189
#define IRQ_INTR_SCI0 190
#define IRQ_INTR_SCI1 191
#define IRQ_SOCP0 192
#define IRQ_INTR_SOCP1 193
#define IRQ_INTR_MODEM_IPC0_S7_INTR_MODEM_IPC1_S7 196
#define IRQ_INTR_MODEM_IPC07_INTR_MODEM_IPC17 197
#define IRQ_INTR_SPE_A 198
#define IRQ_INTR_SPE_C 199
#define IRQ_INTR_MAA0 200
#define IRQ_BBIC_PLL_INT_CM3 201
#define IRQ_INTR_MAA2 202
#define IRQ_INTR_APS_DPM 204
#define IRQ_INTR_I2C9 205
#define IRQ_INTR_HIEPS2LPMCU_RSV0 206
#define IRQ_INTR_HIEPS2LPMCU_RSV1 207
#define IRQ_INTR_HIEPS2LPMCU_RSV2 208
#define IRQ_INTR_HIEPS2LPMCU_RSV3 209
#define IRQ_MDM_EDMAC01_INTR2_COMB 210
#define IRQ_GIC_IRQ_FIQ_OUT0 211
#define IRQ_GIC_IRQ_FIQ_OUT1 212
#define IRQ_GIC_IRQ_FIQ_OUT2 213
#define IRQ_GIC_IRQ_FIQ_OUT3 214
#define IRQ_GIC_IRQ_FIQ_OUT4 215
#define IRQ_GIC_IRQ_FIQ_OUT5 216
#define IRQ_GIC_IRQ_FIQ_OUT6 217
#define IRQ_GIC_IRQ_FIQ_OUT7 218
#define IRQ_MDM2AP_ACPU_INTR_IF_LTE_HARQ2AP_INT 219
#define IRQ_INTR_REPAIR_ERR_AO 220
#define IRQ_INTR_IVP32_WATCH_DOG 221
#define IRQ_INTR_IOMCU_ENABLE_SYSCACHE_STAT 222
#define IRQ_INTR_IOMCU_DISABLE_SYSCACHE_STAT 223
#define IRQ_INTR_MAD 224
#define IRQ_CCPU5GPLL_DFS_UNLOCK_INT 225
#define IRQ_ISP_TO_DDRC_DFS_OK 226
#define IRQ_INTR_EXMBIST 227
#define IRQ_INTR_ATGC 228
#define IRQ_DSS_DFS_OK 229
#define IRQ_INTR_VAD 230
#define IRQ_INTR_MCU_DSI0 231
#define IRQ_INTR_MCU_DSI1 232
#define IRQ_PLL_UNLOCK 233
#define IRQ_NOCBUS_NONIDLE_PEND 234
#define IRQ_FREQUENCY_VOTE 235
#define IRQ_LPMCU_TIMER1 236
#define IRQ_LPMCU_TIMER2 237
#define IRQ_CTI2 238
#define IRQ_CTI3 239
#define IRQ_MAX 240
#define FAULT_SVC_ID 11
#define FAULT_PENDSV_ID 14
#define FAULT_SYSTICK_ID 15
#endif