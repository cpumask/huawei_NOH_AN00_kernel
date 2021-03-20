

#ifndef __OAL_LINUX_HARDWARE_H__
#define __OAL_LINUX_HARDWARE_H__

/* ����ͷ�ļ����� */
/*lint -e322*/
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <linux/pci.h>
#include <linux/ioport.h>
#include <linux/pm.h>
#include <linux/version.h>
#if (_PRE_CONFIG_TARGET_PRODUCT == _PRE_TARGET_PRODUCT_TYPE_E5) ||  \
    (_PRE_CONFIG_TARGET_PRODUCT == _PRE_TARGET_PRODUCT_TYPE_CPE)
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/syscore_ops.h>
#include "mdrv_timer.h"
#endif
#include <linux/gpio.h>
/*lint +e322*/
#include "oal_util.h"
#include "hi_drv_gpio.h"

/* �궨�� */
typedef irq_handler_t oal_irq_handler_t;

#define OAL_HI_TIMER_REG_BASE 0x10105000

#define OAL_HI_TIMER_NUM            2
#define OAL_HI_TIMER_ENABLE         1
#define OAL_HI_TIMER_DISABLE        0
#define OAL_HI_TIMER_INT_DISABLE    1
#define OAL_HI_TIMER_INT_CLEAR      0
#define OAL_HI_TIMER_DEFAULT_PERIOD 1

#define OAL_HI_TIMER_IRQ_NO 80 /* 5113 : 5   5115:80 */

#define OAL_HI_TIMER_FREE_MODE   0 /* 1101�������� */
#define OAL_HI_TIMER_CYCLE_MODE  1
#define OAL_HI_TIMER_SIZE_32_BIT 1
#define OAL_HI_TIMER_WRAPPING    0
#define OAL_HI_TIMER_INT_OCCUR   1
#define OAL_HI_TIMER_INT_VALID   0x01
#define OAL_HI_TIMER_NO_DIV_FREQ 0x0

#define OAL_HI_SC_REG_BASE 0x10100000
#define OAL_HI_SC_CTRL     (OAL_HI_SC_REG_BASE + 0x0000)

#define OAL_IRQ_ENABLE    1 /* �����ж� */
#define OAL_IRQ_FORBIDDEN 0 /* ��ֹ�ж� */

/* �������һ����Ա��������saveʱ�� */
/* ���鵹���ڶ�����Ա��������save������ */
#define OAL_TIMER_IRQ_TYPE_MAX_NUM    255
#define OAL_TIMER_IRQ_SAVE_TIME_INDEX (OAL_TIMER_IRQ_TYPE_MAX_NUM - 1)
#define OAL_TIMER_IRQ_SAVE_TYPE_INDEX (OAL_TIMER_IRQ_TYPE_MAX_NUM - 2)
typedef irqreturn_t oal_irqreturn_t;

/* ö�ٶ��� */
typedef enum {
    OAL_5115TIMER_ONE,
    OAL_5115TIMER_SEC,

    OAL_5115TIMER_BUTT
} oal_5115timer_enum;

/* STRUCT���� */
typedef struct cpumask *oal_cpumask;

#define OAL_SA_SHIRQ IRQF_SHARED /* �ж����� */

typedef uint32_t (*oal_irq_intr_func)(void *);
typedef int32_t (*oal_dbac_isr_func)(int);

typedef struct resource oal_resource;

typedef struct {
    volatile uint32_t ul_timerx_load;
    volatile uint32_t ul_timerx_value;
    volatile uint32_t ul_timerx_control;
    volatile uint32_t ul_timerx_intclr;
    volatile uint32_t ul_timerx_ris;
    volatile uint32_t ul_timerx_mis;
    volatile uint32_t ul_timerx_bgload;
    volatile uint32_t ul_reserve;
} oal_hi_timerx_reg_stru;
/* timer���ƼĴ��� */
typedef union {
    volatile uint32_t ul_value;
    struct {
        volatile uint32_t ul_oneshot : 1;    /* ѡ�����ģʽ 0���ؾ���� 1��һ���Լ��� */
        volatile uint32_t ul_timersize : 1;  /* 16bit|32bit��������ģʽ 0��16bit 1��32bit */
        volatile uint32_t ul_timerpre : 2;   /* Ԥ��Ƶ���� 00������Ƶ 01��4����Ƶ 10��8����Ƶ 11��δ���壬�����൱�ڷ�Ƶ����10 */
        volatile uint32_t ul_reserved0 : 1;  /* ����λ */
        volatile uint32_t ul_intenable : 1;  /* �ж�����λ 0������ 1�������� */
        volatile uint32_t ul_timermode : 1;  /* ����ģʽ 0������ģʽ 1������ģʽ */
        volatile uint32_t ul_timeren : 1;    /* ��ʱ��ʹ��λ 0����ֹ 1��ʹ�� */
        volatile uint32_t ul_reserved1 : 24; /* ����λ */
    } bits_stru;
} oal_hi_timer_control_union;

/* timer2_3�Ĵ��� */
typedef struct {
    oal_hi_timerx_reg_stru ast_timer[OAL_5115TIMER_BUTT];
} oal_hi_timer_reg_stru;
typedef struct {
    oal_hi_timer_control_union u_timerx_config;
} oal_hi_timerx_config_stru;

/* PCI������ض��� */
typedef struct pci_driver oal_pci_driver_stru;
typedef struct pci_device_id oal_pci_device_id_stru;
typedef struct pci_dev oal_pci_dev_stru;
typedef struct dev_pm_ops oal_dev_pm_ops_stru;
typedef pm_message_t oal_pm_message_t;
#if (_PRE_CONFIG_TARGET_PRODUCT == _PRE_TARGET_PRODUCT_TYPE_E5)
typedef struct syscore_ops oal_pm_syscore_ops;
#endif

/* �ж��豸�ṹ�� */
typedef struct {
    uint32_t ul_irq;                 /* �жϺ� */
    int32_t l_irq_type;              /* �ж����ͱ�־ */
    void *p_drv_arg;               /* �жϴ��������� */
    char *pc_name;                 /* �ж��豸���� ֻΪ�����Ѻ� */
    oal_irq_intr_func p_irq_intr_func; /* �жϴ�������ַ */
} oal_irq_dev_stru;

/* ȫ�ֱ������� */
extern oal_hi_timer_reg_stru *g_reg_timer;
extern uint32_t g_irq_save_time[][OAL_TIMER_IRQ_TYPE_MAX_NUM];

/* �������� */
/*
 * �� �� ��  : oal_irq_free
 * ��������  : �ͷ��жϴ������
 * �������  : st_osdev: �ж��豸�ṹ��
 */
OAL_STATIC OAL_INLINE void oal_irq_free(oal_irq_dev_stru *st_osdev)
{
    if (oal_unlikely(st_osdev == NULL)) {
        oal_warn_on(1);
        return;
    }
    free_irq(st_osdev->ul_irq, st_osdev);
}

/*
 * �� �� ��  : oal_irq_interrupt
 * ��������  : �жϷ������
 * �������  : l_irq: �жϺ�
 *             p_dev: �ж��豸
 * �� �� ֵ  : IRQ_HANDLED�жϳ��������
 */
OAL_STATIC OAL_INLINE irqreturn_t oal_irq_interrupt(int32_t l_irq, void *p_dev)
{
    oal_irq_dev_stru *st_osdev = (oal_irq_dev_stru *)p_dev;

    st_osdev->p_irq_intr_func((void *)st_osdev);

    return IRQ_HANDLED;
}

/*
 * �� �� ��  : oal_irq_setup
 * ��������  : ע���жϡ�
 * �������  : st_osdev: �ж��豸�ṹ��
 */
OAL_STATIC OAL_INLINE int32_t oal_irq_setup(oal_irq_dev_stru *st_osdev)
{
    int32_t l_err;

    if (oal_unlikely(st_osdev == NULL)) {
        oal_warn_on(1);
        return OAL_FAIL;
    }

    l_err = request_irq(st_osdev->ul_irq, oal_irq_interrupt,
                        st_osdev->l_irq_type, st_osdev->pc_name, (void *)st_osdev);

    return l_err;
}

/*
 * �� �� ��  : oal_irq_trigger
 * ��������  : �������Ŀ��˵�Ӳ���ж�
 */
OAL_STATIC OAL_INLINE void oal_irq_trigger(uint8_t uc_cpuid)
{
}

OAL_STATIC OAL_INLINE int32_t oal_gpio_is_valid(int32_t i_number)
{
    return gpio_is_valid(i_number);
}

OAL_STATIC OAL_INLINE int32_t oal_gpio_request(uint32_t ul_gpio, OAL_CONST char *pc_label)
{
    return gpio_request(ul_gpio, pc_label);
}

OAL_STATIC OAL_INLINE void oal_gpio_free(uint32_t ul_gpio)
{
    gpio_free(ul_gpio);
}

OAL_STATIC OAL_INLINE int32_t oal_gpio_direction_input(uint32_t ul_gpio)
{
#ifdef _PRE_HI_DRV_GPIO
    return hitv_gpio_request(ul_gpio, 1);
#else
    return gpio_direction_input(ul_gpio);
#endif
}

OAL_STATIC OAL_INLINE int oal_gpio_direction_output(uint32_t ul_gpio, int l_level)
{
#ifdef _PRE_HI_DRV_GPIO
    hitv_gpio_request(ul_gpio, 0);
    return hitv_gpio_direction_output(ul_gpio, l_level);
#else
    return gpio_direction_output(ul_gpio, l_level);
#endif
}

OAL_STATIC OAL_INLINE int32_t oal_gpio_to_irq(uint32_t ul_gpio)
{
    return gpio_to_irq(ul_gpio);
}

OAL_STATIC OAL_INLINE int32_t oal_request_irq(uint32_t ul_irq,
                                              oal_irq_handler_t p_handler,
                                              uint32_t ul_flags,
                                              OAL_CONST char *p_name,
                                              void *p_dev)
{
    return request_irq(ul_irq, p_handler, ul_flags, p_name, p_dev);
}

OAL_STATIC OAL_INLINE int32_t oal_gpio_get_value(uint32_t ul_gpio)
{
#ifdef _PRE_HI_DRV_GPIO
    return hitv_gpio_get_value(ul_gpio);
#else
    return gpio_get_value(ul_gpio);
#endif
}

OAL_STATIC OAL_INLINE void oal_gpio_set_value(uint32_t ul_gpio, int32_t value)
{
    gpio_set_value(ul_gpio, value);
}

#define DELAY_TIME_MS 500
OAL_STATIC OAL_INLINE void oal_wifi_reg_on_pull_up(int32_t wifi_gpio_addr)
{
    if (!oal_gpio_is_valid(wifi_gpio_addr)) {
        oal_io_print("wifi_reg_on_pull_up:fail to get wifi gpio!\n");
        return;
    }
    /* ����Ѿ��Ϲ��磬��ֱ�ӷ��� */
    if (oal_gpio_get_value(wifi_gpio_addr) == 1) {
        oal_io_print("wifi_reg_on_pull_up:WL_REG_ON has been pulled up in wifi_reg_on_pull_up!!!\n");
        return;
    }
    oal_mdelay(DELAY_TIME_MS);
    oal_gpio_direction_output(wifi_gpio_addr, 1);
    oal_mdelay(DELAY_TIME_MS);
}

OAL_STATIC OAL_INLINE void oal_wifi_reg_on_pull_down(int32_t wifi_gpio_addr)
{
    if (!oal_gpio_is_valid(wifi_gpio_addr)) {
        oal_io_print("wifi_reg_on_pull_down:fail to get wifi gpio!\n");
        return;
    }
    /* ����Ѿ��¹��磬��ֱ�ӷ��� */
    if (oal_gpio_get_value(wifi_gpio_addr) == 0) {
        oal_io_print("wifi_reg_on_pull_down:WL_REG_ON has been pulled down in wifi_reg_on_pull_down!!!\n");
        return;
    }

    oal_gpio_direction_output(wifi_gpio_addr, 0);
    oal_mdelay(DELAY_TIME_MS);
}

/*
 * �� �� ��  : oal_irq_set_affinity
 * ��������  : ��ָ�����жϺŰ󶨵�ָ����cpu��ִ��
 * �������  : irq: �жϺ�
 *             ul_cpu: cpu����
 */
OAL_STATIC OAL_INLINE int32_t oal_irq_set_affinity(uint32_t irq, uint32_t ul_cpu)
{
    int32_t l_ret;
    struct cpumask mask;

    cpumask_clear(&mask);
    cpumask_set_cpu(ul_cpu, &mask);

    l_ret = irq_set_affinity(irq, &mask);
    if (l_ret != 0) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/* ����һ���µı�ʹ����Դ�� */
#define oal_request_mem_region(start, n, name) request_mem_region(start, n, name)
#define oal_release_mem_region(start, n)       release_mem_region(start, n)

#define oal_ioremap(cookie, size)           ioremap(cookie, size)
#define oal_ioremap_nocache(_cookie, _size) ioremap_nocache(_cookie, _size)
#define oal_iounmap(cookie)                 iounmap(cookie)

#define oal_writel(_ul_val, _ul_addr) writel(_ul_val, (void*)(_ul_addr))

#define oal_readl(_ul_addr) readl((void*)(_ul_addr))

#define oal_writew(_us_val, _ul_addr) writew(_us_val, _ul_addr)

#define oal_readw(_ul_addr) readw(_ul_addr)

/* ��var��[pos, pos + bits-1]��������,  pos��0��ʼ��� e.g oal_clearl_bits(var, 4, 2) ��ʾ��Bit5~4���� */
OAL_STATIC OAL_INLINE void oal_clearl_bits(void *addr, uint32_t pos, uint32_t bits)
{
    uint32_t value;
    if (oal_unlikely(addr == NULL)) {
        oal_warn_on(1);
        return;
    }
    value = oal_readl(addr);
    value &= ~((((uint32_t)1 << (bits)) - 1) << (pos));
    oal_writel(value, (addr));
}

/* ��var��[pos, pos + bits-1]��������Ϊval,  pos��0��ʼ��� e.g oal_setl_bits(var, 4, 2, 2) ��ʾ��Bit5~4����Ϊb'10 */
OAL_STATIC OAL_INLINE void oal_setl_bits(void *addr, uint32_t pos, uint32_t bits, uint32_t val)
{
    uint32_t value;
    if (oal_unlikely(addr == NULL)) {
        oal_warn_on(1);
        return;
    }
    value = oal_readl(addr);
    value &= ~((((uint32_t)1 << (bits)) - 1) << (pos));
    value |= ((uint32_t)((val) & (((uint32_t)1 << (bits)) - 1)) << (pos));
    oal_writel(value, (addr));
}

OAL_STATIC OAL_INLINE void oal_clearl_bit(void *addr, uint32_t pos)
{
    uint32_t value;
    if (oal_unlikely(addr == NULL)) {
        oal_warn_on(1);
        return;
    }
    value = oal_readl(addr);
    value &= ~(1 << (pos));
    oal_writel(value, (addr));
}

OAL_STATIC OAL_INLINE void oal_setl_bit(void *addr, uint32_t pos)
{
    uint32_t value;
    if (oal_unlikely(addr == NULL)) {
        oal_warn_on(1);
        return;
    }
    value = oal_readl(addr);
    value |= (1 << pos);
    oal_writel(value, (addr));
}

OAL_STATIC OAL_INLINE void oal_clearl_mask(void *addr, uint32_t mask)
{
    uint32_t value;
    if (oal_unlikely(addr == NULL)) {
        oal_warn_on(1);
        return;
    }
    value = oal_readl(addr);
    value &= ~(mask);
    oal_writel(value, (addr));
}

OAL_STATIC OAL_INLINE void oal_setl_mask(void *addr, uint32_t mask)
{
    uint32_t value;
    if (oal_unlikely(addr == NULL)) {
        oal_warn_on(1);
        return;
    }
    value = oal_readl(addr);
    value |= (mask);
    oal_writel(value, (addr));
}

OAL_STATIC OAL_INLINE void oal_value_mask(void *addr, uint32_t value, uint32_t mask)
{
    uint32_t reg;
    if (oal_unlikely(addr == NULL)) {
        oal_warn_on(1);
        return;
    }
    reg = oal_readl(addr);
    reg &= ~(mask);
    value &= mask;
    reg |= value;
    oal_writel(reg, (addr));
}

#endif /* end of oal_hardware.h */
