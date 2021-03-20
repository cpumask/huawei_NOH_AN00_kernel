#ifndef __SOC_NPU_HW_EXP_IRQ_INTERFACE_H__
#define __SOC_NPU_HW_EXP_IRQ_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_NPU_HW_EXP_IRQ_SET_ADDR(base) ((base) + (0x000UL))
#define SOC_NPU_HW_EXP_IRQ_CLR_ADDR(base) ((base) + (0x004UL))
#define SOC_NPU_HW_EXP_IRQ_STAT_ADDR(base) ((base) + (0x008UL))
#define SOC_NPU_HW_EXP_IRQ_SET_P_ADDR(base) ((base) + (0x00CUL))
#define SOC_NPU_HW_EXP_IRQ_CFG_ADDR(base) ((base) + (0x100UL))
#else
#define SOC_NPU_HW_EXP_IRQ_SET_ADDR(base) ((base) + (0x000))
#define SOC_NPU_HW_EXP_IRQ_CLR_ADDR(base) ((base) + (0x004))
#define SOC_NPU_HW_EXP_IRQ_STAT_ADDR(base) ((base) + (0x008))
#define SOC_NPU_HW_EXP_IRQ_SET_P_ADDR(base) ((base) + (0x00C))
#define SOC_NPU_HW_EXP_IRQ_CFG_ADDR(base) ((base) + (0x100))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_hw_exp_irq_set : 8;
        unsigned int reserved : 24;
    } reg;
} SOC_NPU_HW_EXP_IRQ_SET_UNION;
#endif
#define SOC_NPU_HW_EXP_IRQ_SET_npu_hw_exp_irq_set_START (0)
#define SOC_NPU_HW_EXP_IRQ_SET_npu_hw_exp_irq_set_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_hw_exp_irq_clr : 8;
        unsigned int reserved : 24;
    } reg;
} SOC_NPU_HW_EXP_IRQ_CLR_UNION;
#endif
#define SOC_NPU_HW_EXP_IRQ_CLR_npu_hw_exp_irq_clr_START (0)
#define SOC_NPU_HW_EXP_IRQ_CLR_npu_hw_exp_irq_clr_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_hw_exp_irq_stat : 8;
        unsigned int reserved : 24;
    } reg;
} SOC_NPU_HW_EXP_IRQ_STAT_UNION;
#endif
#define SOC_NPU_HW_EXP_IRQ_STAT_npu_hw_exp_irq_stat_START (0)
#define SOC_NPU_HW_EXP_IRQ_STAT_npu_hw_exp_irq_stat_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_hw_exp_irq_set_p : 8;
        unsigned int reserved : 24;
    } reg;
} SOC_NPU_HW_EXP_IRQ_SET_P_UNION;
#endif
#define SOC_NPU_HW_EXP_IRQ_SET_P_npu_hw_exp_irq_set_p_START (0)
#define SOC_NPU_HW_EXP_IRQ_SET_P_npu_hw_exp_irq_set_p_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_hw_exp_irq_cfg : 8;
        unsigned int reserved : 24;
    } reg;
} SOC_NPU_HW_EXP_IRQ_CFG_UNION;
#endif
#define SOC_NPU_HW_EXP_IRQ_CFG_npu_hw_exp_irq_cfg_START (0)
#define SOC_NPU_HW_EXP_IRQ_CFG_npu_hw_exp_irq_cfg_END (7)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
