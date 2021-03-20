#ifndef __HI_COMMON_H__
#define __HI_COMMON_H__ 
#include "bsp_memmap.h"
#define CCPU_INIT_PREPARE_REG_NUM 40
static __inline__ void prepare_ccpu_a9(void)
{
}
static __inline__ void start_ccpu_a9(unsigned int addr)
{
    unsigned int ret = 0;
    ret = (*(volatile unsigned *) (HI_IO_ADDRESS(0xC0200400 + 0x2C)));
    ret &=~ 0x300;
    (*(volatile unsigned *) (HI_IO_ADDRESS(0xC0200400 + 0x2C))) = ret;
    ret = (*(volatile unsigned *) (HI_IO_ADDRESS(0xC4001000 + 0x810)));
    ret = (ret&0x03)|((addr>>13)<<2);
    ret &= ~(0x3<<21);
    (*(volatile unsigned *) (HI_IO_ADDRESS(0xC4001000 + 0x810))) = ret;
    asm volatile ( "dsb;" );
    (*(volatile unsigned *) (HI_IO_ADDRESS(0xC0200400 + 0x818))) = 0x1;
    while(1)
    {
        ret = (*(volatile unsigned *) (HI_IO_ADDRESS(0xC0200400 + 0xa04)));
        if((ret & 0x1) != 0x0)
        {
            break;
        }
    }
    (*(volatile unsigned *) (HI_IO_ADDRESS(0xC0200000 + 0x70))) = (0x2667);
    (*(volatile unsigned *) (HI_IO_ADDRESS(0xC0200400 + 0x810))) = 0x1;
    (*(volatile unsigned *) (HI_IO_ADDRESS(0xC0200000 + 0x24))) = (0xBFC8);
    ret = (*(volatile unsigned *) (HI_IO_ADDRESS(0xC0200400 + 0x2C)));
    ret |= 0x300;
    (*(volatile unsigned *) (HI_IO_ADDRESS(0xC0200400 + 0x2C))) = ret;
}
#endif
