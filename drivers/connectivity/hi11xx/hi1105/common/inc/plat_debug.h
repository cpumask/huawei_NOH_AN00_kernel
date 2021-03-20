

#ifndef __PLAT_DEBUG_H__
#define __PLAT_DEBUG_H__

/* ����ͷ�ļ����� */
#include "bfgx_user_ctrl.h"

/* �궨�� */
#define PS_PRINT_FUNCTION_NAME                                   \
    do {                                                         \
        if (g_hi110x_loglevel >= PLAT_LOG_DEBUG) {               \
            printk(KERN_INFO KBUILD_MODNAME ":D]%s]", __func__); \
        }                                                        \
    } while (0)

#define ps_print_dbg(s, args...)                                           \
    do {                                                                   \
        if (g_hi110x_loglevel >= PLAT_LOG_DEBUG) {                         \
            printk(KERN_INFO KBUILD_MODNAME ":D]%s]" s, __func__, ##args); \
        }                                                                  \
    } while (0)

#ifdef _PRE_UART_PRINT_LOG
/* logs will print from UART, reduce print */
#define ps_print_info(s, args...)                                           \
    do {                                                                    \
        if (g_hi110x_loglevel >= PLAT_LOG_INFO) {                           \
            printk(KERN_DEBUG KBUILD_MODNAME ":I]%s]" s, __func__, ##args); \
        }                                                                   \
    } while (0)
#else
#define ps_print_info(s, args...)                                          \
    do {                                                                   \
        if (g_hi110x_loglevel >= PLAT_LOG_INFO) {                          \
            printk(KERN_INFO KBUILD_MODNAME ":I]%s]" s, __func__, ##args); \
        }                                                                  \
    } while (0)
#endif

#define ps_print_suc(s, args...)                                           \
    do {                                                                   \
        if (g_hi110x_loglevel >= PLAT_LOG_INFO) {                          \
            printk(KERN_INFO KBUILD_MODNAME ":S]%s]" s, __func__, ##args); \
        }                                                                  \
    } while (0)

#define ps_print_warning(s, args...)                                          \
    do {                                                                      \
        if (g_hi110x_loglevel >= PLAT_LOG_WARNING) {                          \
            printk(KERN_WARNING KBUILD_MODNAME ":W]%s]" s, __func__, ##args); \
        }                                                                     \
    } while (0)

#define ps_print_err(s, args...)                                          \
    do {                                                                  \
        if (g_hi110x_loglevel >= PLAT_LOG_ERR) {                          \
            printk(KERN_ERR KBUILD_MODNAME ":E]%s]" s, __func__, ##args); \
        }                                                                 \
    } while (0)

#endif /* PLAT_DEBUG_H */
