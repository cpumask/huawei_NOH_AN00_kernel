#ifndef _HI_MLOADER_H_
#define _HI_MLOADER_H_ 
#include <product_config.h>
#include <bsp_sysctrl.h>
#include <soc_memmap.h>
#include <osl_bio.h>
#include <osl_types.h>
#define MODEM_IMAGE_PATH "/vendor/modem/modem_fw/"
#define MLOADER_COLD_PATCH_PATH "/patch_hw/hot_modem_fw/"
#define MLOADER_LR_CCPU_IMG "modem_lr_ccpu.bin"
static inline void mloader_ccpu_reset(u32 ccpu_no)
{
    writel(0xfb58, bsp_sysctrl_addr_get((void *) 0xe0200020));
}
static inline void mloader_ccpu_unreset(void* run_addr ,u32 ccpu_no)
{
    u32 value = 0;
    void *addr = NULL;
    if (ccpu_no == 0) {
        addr = bsp_sysctrl_addr_get((void *) 0xE0200024);
        if (addr == NULL) {
            return;
        }
        writel(0x2040, addr);
        addr = bsp_sysctrl_addr_get((void *) 0xE020042c);
        if (addr == NULL) {
            return;
        }
        value = readl(addr);
        value |= 0x300;
        writel(value, addr);
    } else if (ccpu_no == 1) {
        addr = bsp_sysctrl_addr_get((void *) 0xF4000094);
        if (addr == NULL) {
            return;
        }
        writel(0x30, addr);
        addr = bsp_sysctrl_addr_get((void *) 0xF5201010);
        if (addr == NULL) {
            return;
        }
        value = readl(addr);
        value |= 0x300;
        writel(value, addr);
    }
}
static inline void mloader_l2hac_reset(void)
{
    writel(0xFFFFFFFF, (void *)(0xF8000000 + 0x78));
}
static inline void mloader_l2hac_unreset(void* run_addr)
{
    unsigned int sys_ctrl = 0;
    sys_ctrl = readl((void *)0xF8801000);
    writel(((sys_ctrl & 0xFFF) | (HAC_LLRAM_ADDR)), ((void *)0xF8801000));
    writel(0xFFFFFFFF, (void *)(0xF8000000 + 0x7C));
    sys_ctrl = readl((void *)0xF8801010);
    sys_ctrl |= 0x300;
    writel(sys_ctrl, (void *)0xF8801010);
}
static inline unsigned int mloader_l2hac_stat(void)
{
    unsigned int stat= 0;
    stat = readl((void *)0xF8000080);
    return stat;
}
#endif
