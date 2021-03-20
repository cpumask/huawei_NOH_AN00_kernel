#ifndef __SOC_PCTRL_INTERFACE_H__
#define __SOC_PCTRL_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_PCTRL_G3D_RASTER_ADDR(base) ((base) + (0x000UL))
#define SOC_PCTRL_PERI_CTRL0_ADDR(base) ((base) + (0x004UL))
#define SOC_PCTRL_PERI_CTRL1_ADDR(base) ((base) + (0x008UL))
#define SOC_PCTRL_PERI_CTRL2_ADDR(base) ((base) + (0x00CUL))
#define SOC_PCTRL_PERI_CTRL3_ADDR(base) ((base) + (0x010UL))
#define SOC_PCTRL_PERI_CTRL4_ADDR(base) ((base) + (0x014UL))
#define SOC_PCTRL_PERI_CTRL98_ADDR(base) ((base) + (0x018UL))
#define SOC_PCTRL_PERI_CTRL99_ADDR(base) ((base) + (0x01CUL))
#define SOC_PCTRL_PERI_CTRL100_ADDR(base) ((base) + (0x020UL))
#define SOC_PCTRL_PERI_CTRL101_ADDR(base) ((base) + (0x024UL))
#define SOC_PCTRL_PERI_CTRL102_ADDR(base) ((base) + (0x028UL))
#define SOC_PCTRL_PERI_CTRL103_ADDR(base) ((base) + (0x02CUL))
#define SOC_PCTRL_PERI_CTRL104_ADDR(base) ((base) + (0x030UL))
#define SOC_PCTRL_PERI_CTRL12_ADDR(base) ((base) + (0x034UL))
#define SOC_PCTRL_PERI_CTRL13_ADDR(base) ((base) + (0x038UL))
#define SOC_PCTRL_PERI_CTRL14_ADDR(base) ((base) + (0x03CUL))
#define SOC_PCTRL_PERI_CTRL15_ADDR(base) ((base) + (0x040UL))
#define SOC_PCTRL_PERI_CTRL16_ADDR(base) ((base) + (0x044UL))
#define SOC_PCTRL_PERI_CTRL17_ADDR(base) ((base) + (0x048UL))
#define SOC_PCTRL_PERI_CTRL18_ADDR(base) ((base) + (0x04CUL))
#define SOC_PCTRL_PERI_CTRL19_ADDR(base) ((base) + (0x050UL))
#define SOC_PCTRL_PERI_CTRL20_ADDR(base) ((base) + (0x054UL))
#define SOC_PCTRL_PERI_CTRL21_ADDR(base) ((base) + (0x058UL))
#define SOC_PCTRL_PERI_CTRL22_ADDR(base) ((base) + (0x05CUL))
#define SOC_PCTRL_PERI_CTRL23_ADDR(base) ((base) + (0x060UL))
#define SOC_PCTRL_PERI_CTRL24_ADDR(base) ((base) + (0x064UL))
#define SOC_PCTRL_PERI_CTRL25_ADDR(base) ((base) + (0x068UL))
#define SOC_PCTRL_PERI_CTRL26_ADDR(base) ((base) + (0x06CUL))
#define SOC_PCTRL_PERI_CTRL27_ADDR(base) ((base) + (0x070UL))
#define SOC_PCTRL_PERI_CTRL28_ADDR(base) ((base) + (0x074UL))
#define SOC_PCTRL_PERI_CTRL29_ADDR(base) ((base) + (0x078UL))
#define SOC_PCTRL_PERI_CTRL30_ADDR(base) ((base) + (0x07CUL))
#define SOC_PCTRL_PERI_CTRL31_ADDR(base) ((base) + (0x080UL))
#define SOC_PCTRL_PERI_CTRL32_ADDR(base) ((base) + (0x084UL))
#define SOC_PCTRL_PERI_CTRL33_ADDR(base) ((base) + (0x088UL))
#define SOC_PCTRL_PERI_STAT0_ADDR(base) ((base) + (0x094UL))
#define SOC_PCTRL_PERI_STAT1_ADDR(base) ((base) + (0x098UL))
#define SOC_PCTRL_PERI_STAT2_ADDR(base) ((base) + (0x09CUL))
#define SOC_PCTRL_PERI_STAT3_ADDR(base) ((base) + (0x0A0UL))
#define SOC_PCTRL_PERI_STAT4_ADDR(base) ((base) + (0x0A4UL))
#define SOC_PCTRL_PERI_STAT5_ADDR(base) ((base) + (0x0A8UL))
#define SOC_PCTRL_PERI_STAT6_ADDR(base) ((base) + (0x0ACUL))
#define SOC_PCTRL_PERI_STAT7_ADDR(base) ((base) + (0x0B0UL))
#define SOC_PCTRL_PERI_STAT8_ADDR(base) ((base) + (0x0B4UL))
#define SOC_PCTRL_PERI_STAT9_ADDR(base) ((base) + (0x0B8UL))
#define SOC_PCTRL_PERI_STAT10_ADDR(base) ((base) + (0x0BCUL))
#define SOC_PCTRL_PERI_STAT11_ADDR(base) ((base) + (0x0C0UL))
#define SOC_PCTRL_PERI_STAT12_ADDR(base) ((base) + (0x0C4UL))
#define SOC_PCTRL_PERI_STAT13_ADDR(base) ((base) + (0x0C8UL))
#define SOC_PCTRL_PERI_STAT14_ADDR(base) ((base) + (0x0CCUL))
#define SOC_PCTRL_PERI_STAT15_ADDR(base) ((base) + (0x0D0UL))
#define SOC_PCTRL_PERI_STAT16_ADDR(base) ((base) + (0x0D4UL))
#define SOC_PCTRL_PERI_STAT17_ADDR(base) ((base) + (0x0D8UL))
#define SOC_PCTRL_PERI_STAT18_ADDR(base) ((base) + (0x0DCUL))
#define SOC_PCTRL_PERI_STAT19_ADDR(base) ((base) + (0x0E0UL))
#define SOC_PCTRL_USB2_HOST_CTRL0_ADDR(base) ((base) + (0x0F0UL))
#define SOC_PCTRL_USB2_HOST_CTRL1_ADDR(base) ((base) + (0x0F4UL))
#define SOC_PCTRL_USB2_HOST_CTRL2_ADDR(base) ((base) + (0x0F8UL))
#define SOC_PCTRL_PERI_STAT29_ADDR(base) ((base) + (0x100UL))
#define SOC_PCTRL_PERI_STAT30_ADDR(base) ((base) + (0x104UL))
#define SOC_PCTRL_PERI_STAT31_ADDR(base) ((base) + (0x108UL))
#define SOC_PCTRL_PERI_STAT32_ADDR(base) ((base) + (0x10CUL))
#define SOC_PCTRL_PERI_STAT33_ADDR(base) ((base) + (0x110UL))
#define SOC_PCTRL_PERI_STAT34_ADDR(base) ((base) + (0x114UL))
#define SOC_PCTRL_PERI_STAT35_ADDR(base) ((base) + (0x118UL))
#define SOC_PCTRL_PERI_STAT36_ADDR(base) ((base) + (0x11CUL))
#define SOC_PCTRL_PERI_STAT37_ADDR(base) ((base) + (0x120UL))
#define SOC_PCTRL_PERI_STAT38_ADDR(base) ((base) + (0x124UL))
#define SOC_PCTRL_PERI_STAT39_ADDR(base) ((base) + (0x128UL))
#define SOC_PCTRL_PERI_STAT40_ADDR(base) ((base) + (0x12CUL))
#define SOC_PCTRL_PERI_STAT41_ADDR(base) ((base) + (0x130UL))
#define SOC_PCTRL_PERI_STAT42_ADDR(base) ((base) + (0x134UL))
#define SOC_PCTRL_PERI_STAT43_ADDR(base) ((base) + (0x138UL))
#define SOC_PCTRL_PERI_STAT44_ADDR(base) ((base) + (0x13CUL))
#define SOC_PCTRL_PERI_STAT45_ADDR(base) ((base) + (0x140UL))
#define SOC_PCTRL_PERI_STAT46_ADDR(base) ((base) + (0x144UL))
#define SOC_PCTRL_PERI_STAT47_ADDR(base) ((base) + (0x148UL))
#define SOC_PCTRL_PERI_STAT48_ADDR(base) ((base) + (0x14CUL))
#define SOC_PCTRL_PERI_STAT49_ADDR(base) ((base) + (0x150UL))
#define SOC_PCTRL_PERI_STAT50_ADDR(base) ((base) + (0x154UL))
#define SOC_PCTRL_PERI_STAT51_ADDR(base) ((base) + (0x158UL))
#define SOC_PCTRL_PERI_STAT52_ADDR(base) ((base) + (0x15CUL))
#define SOC_PCTRL_PERI_STAT53_ADDR(base) ((base) + (0x160UL))
#define SOC_PCTRL_PERI_STAT54_ADDR(base) ((base) + (0x164UL))
#define SOC_PCTRL_PERI_STAT55_ADDR(base) ((base) + (0x168UL))
#define SOC_PCTRL_PERI_STAT56_ADDR(base) ((base) + (0x16CUL))
#define SOC_PCTRL_PERI_STAT57_ADDR(base) ((base) + (0x170UL))
#define SOC_PCTRL_PERI_STAT58_ADDR(base) ((base) + (0x174UL))
#define SOC_PCTRL_PERI_STAT59_ADDR(base) ((base) + (0x178UL))
#define SOC_PCTRL_PERI_STAT60_ADDR(base) ((base) + (0x17CUL))
#define SOC_PCTRL_PERI_STAT61_ADDR(base) ((base) + (0x180UL))
#define SOC_PCTRL_PERI_STAT62_ADDR(base) ((base) + (0x184UL))
#define SOC_PCTRL_PERI_STAT63_ADDR(base) ((base) + (0x188UL))
#define SOC_PCTRL_PERI_STAT64_ADDR(base) ((base) + (0x18CUL))
#define SOC_PCTRL_PERI_STAT66_ADDR(base) ((base) + (0x190UL))
#define SOC_PCTRL_PERI_STAT67_ADDR(base) ((base) + (0x194UL))
#define SOC_PCTRL_PERI_STAT68_ADDR(base) ((base) + (0x198UL))
#define SOC_PCTRL_PERI_STAT69_ADDR(base) ((base) + (0x19CUL))
#define SOC_PCTRL_PERI_STAT70_ADDR(base) ((base) + (0x1A0UL))
#define SOC_PCTRL_PERI_STAT71_ADDR(base) ((base) + (0x1A4UL))
#define SOC_PCTRL_PERI_STAT72_ADDR(base) ((base) + (0x1A8UL))
#define SOC_PCTRL_PERI_STAT73_ADDR(base) ((base) + (0x1ACUL))
#define SOC_PCTRL_PERI_STAT74_ADDR(base) ((base) + (0x1B0UL))
#define SOC_PCTRL_PERI_STAT75_ADDR(base) ((base) + (0x1B4UL))
#define SOC_PCTRL_PERI_STAT76_ADDR(base) ((base) + (0x1B8UL))
#define SOC_PCTRL_PERI_STAT77_ADDR(base) ((base) + (0x1BCUL))
#define SOC_PCTRL_PERI_STAT78_ADDR(base) ((base) + (0x1C0UL))
#define SOC_PCTRL_PERI_STAT79_ADDR(base) ((base) + (0x1C4UL))
#define SOC_PCTRL_PERI_STAT80_ADDR(base) ((base) + (0x1C8UL))
#define SOC_PCTRL_PERI_STAT81_ADDR(base) ((base) + (0x1CCUL))
#define SOC_PCTRL_PERI_STAT82_ADDR(base) ((base) + (0x1D0UL))
#define SOC_PCTRL_PERI_STAT83_ADDR(base) ((base) + (0x1D4UL))
#define SOC_PCTRL_PERI_STAT84_ADDR(base) ((base) + (0x1D8UL))
#define SOC_PCTRL_PERI_STAT85_ADDR(base) ((base) + (0x1DCUL))
#define SOC_PCTRL_PERI_STAT86_ADDR(base) ((base) + (0x1E0UL))
#define SOC_PCTRL_PERI_STAT87_ADDR(base) ((base) + (0x1E4UL))
#define SOC_PCTRL_PERI_STAT88_ADDR(base) ((base) + (0x1E8UL))
#define SOC_PCTRL_PERI_STAT89_ADDR(base) ((base) + (0x1ECUL))
#define SOC_PCTRL_PERI_STAT90_ADDR(base) ((base) + (0x1F0UL))
#define SOC_PCTRL_PERI_STAT91_ADDR(base) ((base) + (0x1F4UL))
#define SOC_PCTRL_PERI_STAT92_ADDR(base) ((base) + (0x1F8UL))
#define SOC_PCTRL_PERI_STAT93_ADDR(base) ((base) + (0x1FCUL))
#define SOC_PCTRL_PERI_CTRL67_ADDR(base) ((base) + (0x200UL))
#define SOC_PCTRL_PERI_CTRL68_ADDR(base) ((base) + (0x204UL))
#define SOC_PCTRL_PERI_CTRL69_ADDR(base) ((base) + (0x208UL))
#define SOC_PCTRL_PERI_CTRL70_ADDR(base) ((base) + (0x20CUL))
#define SOC_PCTRL_PERI_CTRL71_ADDR(base) ((base) + (0x210UL))
#define SOC_PCTRL_PERI_CTRL72_ADDR(base) ((base) + (0x214UL))
#define SOC_PCTRL_PERI_CTRL73_ADDR(base) ((base) + (0x218UL))
#define SOC_PCTRL_PERI_CTRL74_ADDR(base) ((base) + (0x21CUL))
#define SOC_PCTRL_PERI_CTRL88_ADDR(base) ((base) + (0x220UL))
#define SOC_PCTRL_PERI_CTRL89_ADDR(base) ((base) + (0x224UL))
#define SOC_PCTRL_PERI_CTRL90_ADDR(base) ((base) + (0x228UL))
#define SOC_PCTRL_PERI_CTRL91_ADDR(base) ((base) + (0x22CUL))
#define SOC_PCTRL_PERI_CTRL92_ADDR(base) ((base) + (0x230UL))
#define SOC_PCTRL_PERI_CTRL93_ADDR(base) ((base) + (0x234UL))
#define SOC_PCTRL_PERI_CTRL95_ADDR(base) ((base) + (0x238UL))
#define SOC_PCTRL_PERI_CTRL96_ADDR(base) ((base) + (0x23CUL))
#define SOC_PCTRL_PERI_CTRL97_ADDR(base) ((base) + (0x240UL))
#define SOC_PCTRL_PERI_CTRL105_ADDR(base) ((base) + (0x244UL))
#define SOC_PCTRL_PERI_CTRL106_ADDR(base) ((base) + (0x248UL))
#define SOC_PCTRL_PERI_CTRL107_ADDR(base) ((base) + (0x24CUL))
#define SOC_PCTRL_PERI_CTRL108_ADDR(base) ((base) + (0x250UL))
#define SOC_PCTRL_PERI_CTRL109_ADDR(base) ((base) + (0x254UL))
#define SOC_PCTRL_PERI_CTRL110_ADDR(base) ((base) + (0x258UL))
#define SOC_PCTRL_PERI_CTRL111_ADDR(base) ((base) + (0x25CUL))
#define SOC_PCTRL_PERI_CTRL112_ADDR(base) ((base) + (0x260UL))
#define SOC_PCTRL_PERI_CTRL113_ADDR(base) ((base) + (0x264UL))
#define SOC_PCTRL_PERI_CTRL114_ADDR(base) ((base) + (0x268UL))
#define SOC_PCTRL_PERI_CTRL115_ADDR(base) ((base) + (0x26CUL))
#define SOC_PCTRL_PERI_CTRL116_ADDR(base) ((base) + (0x270UL))
#define SOC_PCTRL_PERI_CTRL117_ADDR(base) ((base) + (0x274UL))
#define SOC_PCTRL_PERI_CTRL118_ADDR(base) ((base) + (0x278UL))
#define SOC_PCTRL_PERI_CTRL119_ADDR(base) ((base) + (0x27CUL))
#define SOC_PCTRL_PERI_CTRL120_ADDR(base) ((base) + (0x280UL))
#define SOC_PCTRL_PERI_CTRL121_ADDR(base) ((base) + (0x284UL))
#define SOC_PCTRL_PERI_CTRL122_ADDR(base) ((base) + (0x288UL))
#define SOC_PCTRL_PERI_CTRL123_ADDR(base) ((base) + (0x28CUL))
#define SOC_PCTRL_PERI_CTRL124_ADDR(base) ((base) + (0x290UL))
#define SOC_PCTRL_PERI_CTRL125_ADDR(base) ((base) + (0x294UL))
#define SOC_PCTRL_PERI_CTRL126_ADDR(base) ((base) + (0x298UL))
#define SOC_PCTRL_PERI_CTRL127_ADDR(base) ((base) + (0x29CUL))
#define SOC_PCTRL_PERI_CTRL128_ADDR(base) ((base) + (0x2A0UL))
#define SOC_PCTRL_PERI_CTRL129_ADDR(base) ((base) + (0x2A4UL))
#define SOC_PCTRL_PERI_CTRL130_ADDR(base) ((base) + (0x2A8UL))
#define SOC_PCTRL_PERI_CTRL131_ADDR(base) ((base) + (0x2ACUL))
#define SOC_PCTRL_PERI_CTRL132_ADDR(base) ((base) + (0x2B0UL))
#define SOC_PCTRL_PERI_CTRL133_ADDR(base) ((base) + (0x2B4UL))
#define SOC_PCTRL_PERI_CTRL134_ADDR(base) ((base) + (0x2B8UL))
#define SOC_PCTRL_PERI_CTRL135_ADDR(base) ((base) + (0x2BCUL))
#define SOC_PCTRL_PERI_CTRL136_ADDR(base) ((base) + (0x2C0UL))
#define SOC_PCTRL_PERI_CTRL137_ADDR(base) ((base) + (0x2C4UL))
#define SOC_PCTRL_PERI_CTRL138_ADDR(base) ((base) + (0x2C8UL))
#define SOC_PCTRL_PERI_CTRL139_ADDR(base) ((base) + (0x2CCUL))
#define SOC_PCTRL_PERI_CTRL140_ADDR(base) ((base) + (0x2D0UL))
#define SOC_PCTRL_RESOURCE0_LOCK_ADDR(base) ((base) + (0x400UL))
#define SOC_PCTRL_RESOURCE0_UNLOCK_ADDR(base) ((base) + (0x404UL))
#define SOC_PCTRL_RESOURCE0_LOCK_ST_ADDR(base) ((base) + (0x408UL))
#define SOC_PCTRL_RESOURCE1_LOCK_ADDR(base) ((base) + (0x40CUL))
#define SOC_PCTRL_RESOURCE1_UNLOCK_ADDR(base) ((base) + (0x410UL))
#define SOC_PCTRL_RESOURCE1_LOCK_ST_ADDR(base) ((base) + (0x414UL))
#define SOC_PCTRL_RESOURCE2_LOCK_ADDR(base) ((base) + (0x418UL))
#define SOC_PCTRL_RESOURCE2_UNLOCK_ADDR(base) ((base) + (0x41CUL))
#define SOC_PCTRL_RESOURCE2_LOCK_ST_ADDR(base) ((base) + (0x420UL))
#define SOC_PCTRL_RESOURCE3_LOCK_ADDR(base) ((base) + (0x424UL))
#define SOC_PCTRL_RESOURCE3_UNLOCK_ADDR(base) ((base) + (0x428UL))
#define SOC_PCTRL_RESOURCE3_LOCK_ST_ADDR(base) ((base) + (0x42CUL))
#define SOC_PCTRL_RESOURCE4_LOCK_ADDR(base) ((base) + (0x800UL))
#define SOC_PCTRL_RESOURCE4_UNLOCK_ADDR(base) ((base) + (0x804UL))
#define SOC_PCTRL_RESOURCE4_LOCK_ST_ADDR(base) ((base) + (0x808UL))
#define SOC_PCTRL_RESOURCE5_LOCK_ADDR(base) ((base) + (0x80CUL))
#define SOC_PCTRL_RESOURCE5_UNLOCK_ADDR(base) ((base) + (0x810UL))
#define SOC_PCTRL_RESOURCE5_LOCK_ST_ADDR(base) ((base) + (0x814UL))
#define SOC_PCTRL_RESOURCE6_LOCK_ADDR(base) ((base) + (0x818UL))
#define SOC_PCTRL_RESOURCE6_UNLOCK_ADDR(base) ((base) + (0x81CUL))
#define SOC_PCTRL_RESOURCE6_LOCK_ST_ADDR(base) ((base) + (0x820UL))
#define SOC_PCTRL_RESOURCE7_LOCK_ADDR(base) ((base) + (0x824UL))
#define SOC_PCTRL_RESOURCE7_UNLOCK_ADDR(base) ((base) + (0x828UL))
#define SOC_PCTRL_RESOURCE7_LOCK_ST_ADDR(base) ((base) + (0x82CUL))
#define SOC_PCTRL_PERI_CTRL5_ADDR(base) ((base) + (0xC00UL))
#define SOC_PCTRL_PERI_CTRL6_ADDR(base) ((base) + (0xC04UL))
#define SOC_PCTRL_PERI_CTRL7_ADDR(base) ((base) + (0xC08UL))
#define SOC_PCTRL_PERI_CTRL8_ADDR(base) ((base) + (0xC0CUL))
#define SOC_PCTRL_PERI_CTRL9_ADDR(base) ((base) + (0xC10UL))
#define SOC_PCTRL_PERI_CTRL10_ADDR(base) ((base) + (0xC14UL))
#define SOC_PCTRL_PERI_CTRL11_ADDR(base) ((base) + (0xC18UL))
#define SOC_PCTRL_PERI_CTRL34_ADDR(base) ((base) + (0xC1CUL))
#define SOC_PCTRL_PERI_CTRL35_ADDR(base) ((base) + (0xC20UL))
#define SOC_PCTRL_PERI_CTRL38_ADDR(base) ((base) + (0xC2CUL))
#define SOC_PCTRL_PERI_CTRL39_ADDR(base) ((base) + (0xC30UL))
#define SOC_PCTRL_PERI_CTRL40_ADDR(base) ((base) + (0xC34UL))
#define SOC_PCTRL_PERI_CTRL41_ADDR(base) ((base) + (0xC38UL))
#define SOC_PCTRL_PERI_CTRL42_ADDR(base) ((base) + (0xC3CUL))
#define SOC_PCTRL_PERI_CTRL43_ADDR(base) ((base) + (0xC40UL))
#define SOC_PCTRL_PERI_CTRL44_ADDR(base) ((base) + (0xC44UL))
#define SOC_PCTRL_PERI_CTRL45_ADDR(base) ((base) + (0xC48UL))
#define SOC_PCTRL_PERI_CTRL46_ADDR(base) ((base) + (0xC4CUL))
#define SOC_PCTRL_PERI_CTRL47_ADDR(base) ((base) + (0xC50UL))
#define SOC_PCTRL_PERI_CTRL48_ADDR(base) ((base) + (0xC54UL))
#define SOC_PCTRL_PERI_CTRL49_ADDR(base) ((base) + (0xC58UL))
#define SOC_PCTRL_PERI_CTRL50_ADDR(base) ((base) + (0xC5CUL))
#define SOC_PCTRL_PERI_CTRL51_ADDR(base) ((base) + (0xC60UL))
#define SOC_PCTRL_PERI_CTRL52_ADDR(base) ((base) + (0xC64UL))
#define SOC_PCTRL_PERI_CTRL53_ADDR(base) ((base) + (0xC68UL))
#define SOC_PCTRL_PERI_CTRL54_ADDR(base) ((base) + (0xC6CUL))
#define SOC_PCTRL_PERI_CTRL55_ADDR(base) ((base) + (0xC70UL))
#define SOC_PCTRL_PERI_CTRL56_ADDR(base) ((base) + (0xC74UL))
#define SOC_PCTRL_PERI_CTRL57_ADDR(base) ((base) + (0xC78UL))
#define SOC_PCTRL_PERI_CTRL58_ADDR(base) ((base) + (0xC7CUL))
#define SOC_PCTRL_PERI_CTRL59_ADDR(base) ((base) + (0xC80UL))
#define SOC_PCTRL_PERI_CTRL60_ADDR(base) ((base) + (0xC84UL))
#define SOC_PCTRL_PERI_CTRL61_ADDR(base) ((base) + (0xC88UL))
#define SOC_PCTRL_PERI_CTRL62_ADDR(base) ((base) + (0xC8CUL))
#define SOC_PCTRL_PERI_CTRL63_ADDR(base) ((base) + (0xC90UL))
#define SOC_PCTRL_PERI_CTRL64_ADDR(base) ((base) + (0xC94UL))
#define SOC_PCTRL_PERI_CTRL65_ADDR(base) ((base) + (0xC98UL))
#define SOC_PCTRL_PERI_CTRL66_ADDR(base) ((base) + (0xC9CUL))
#define SOC_PCTRL_PERI_CTRL75_ADDR(base) ((base) + (0xCA0UL))
#define SOC_PCTRL_PERI_CTRL76_ADDR(base) ((base) + (0xCA4UL))
#define SOC_PCTRL_PERI_CTRL77_ADDR(base) ((base) + (0xCA8UL))
#define SOC_PCTRL_PERI_CTRL78_ADDR(base) ((base) + (0xCACUL))
#define SOC_PCTRL_PERI_CTRL79_ADDR(base) ((base) + (0xCB0UL))
#define SOC_PCTRL_PERI_CTRL80_ADDR(base) ((base) + (0xCB4UL))
#define SOC_PCTRL_PERI_CTRL81_ADDR(base) ((base) + (0xCB8UL))
#define SOC_PCTRL_PERI_CTRL82_ADDR(base) ((base) + (0xCBCUL))
#define SOC_PCTRL_PERI_CTRL83_ADDR(base) ((base) + (0xCC0UL))
#define SOC_PCTRL_PERI_CTRL84_ADDR(base) ((base) + (0xCC4UL))
#define SOC_PCTRL_PERI_CTRL85_ADDR(base) ((base) + (0xCC8UL))
#define SOC_PCTRL_PERI_CTRL86_ADDR(base) ((base) + (0xCCCUL))
#define SOC_PCTRL_PERI_CTRL87_ADDR(base) ((base) + (0xCD0UL))
#define SOC_PCTRL_PERI_CTRL94_ADDR(base) ((base) + (0xCD4UL))
#define SOC_PCTRL_PERI_CTRL141_ADDR(base) ((base) + (0xCD8UL))
#define SOC_PCTRL_PERI_CTRL142_ADDR(base) ((base) + (0xCDCUL))
#define SOC_PCTRL_PERI_CTRL143_ADDR(base) ((base) + (0xCE0UL))
#define SOC_PCTRL_PERI_CTRL144_ADDR(base) ((base) + (0xCE4UL))
#define SOC_PCTRL_PERI_CTRL145_ADDR(base) ((base) + (0xCE8UL))
#define SOC_PCTRL_PERI_CTRL146_ADDR(base) ((base) + (0xCECUL))
#define SOC_PCTRL_PERI_CTRL147_ADDR(base) ((base) + (0xCF0UL))
#define SOC_PCTRL_PERI_CTRL148_ADDR(base) ((base) + (0xCF4UL))
#define SOC_PCTRL_PERI_STAT20_ADDR(base) ((base) + (0xE00UL))
#define SOC_PCTRL_PERI_STAT21_ADDR(base) ((base) + (0xE04UL))
#define SOC_PCTRL_PERI_STAT22_ADDR(base) ((base) + (0xE08UL))
#define SOC_PCTRL_PERI_STAT23_ADDR(base) ((base) + (0xE0CUL))
#define SOC_PCTRL_PERI_STAT24_ADDR(base) ((base) + (0xE10UL))
#define SOC_PCTRL_PERI_STAT25_ADDR(base) ((base) + (0xE14UL))
#define SOC_PCTRL_PERI_STAT26_ADDR(base) ((base) + (0xE18UL))
#define SOC_PCTRL_PERI_STAT27_ADDR(base) ((base) + (0xE1CUL))
#define SOC_PCTRL_PERI_STAT28_ADDR(base) ((base) + (0xE20UL))
#define SOC_PCTRL_PERI_STAT65_ADDR(base) ((base) + (0xE24UL))
#else
#define SOC_PCTRL_G3D_RASTER_ADDR(base) ((base) + (0x000))
#define SOC_PCTRL_PERI_CTRL0_ADDR(base) ((base) + (0x004))
#define SOC_PCTRL_PERI_CTRL1_ADDR(base) ((base) + (0x008))
#define SOC_PCTRL_PERI_CTRL2_ADDR(base) ((base) + (0x00C))
#define SOC_PCTRL_PERI_CTRL3_ADDR(base) ((base) + (0x010))
#define SOC_PCTRL_PERI_CTRL4_ADDR(base) ((base) + (0x014))
#define SOC_PCTRL_PERI_CTRL98_ADDR(base) ((base) + (0x018))
#define SOC_PCTRL_PERI_CTRL99_ADDR(base) ((base) + (0x01C))
#define SOC_PCTRL_PERI_CTRL100_ADDR(base) ((base) + (0x020))
#define SOC_PCTRL_PERI_CTRL101_ADDR(base) ((base) + (0x024))
#define SOC_PCTRL_PERI_CTRL102_ADDR(base) ((base) + (0x028))
#define SOC_PCTRL_PERI_CTRL103_ADDR(base) ((base) + (0x02C))
#define SOC_PCTRL_PERI_CTRL104_ADDR(base) ((base) + (0x030))
#define SOC_PCTRL_PERI_CTRL12_ADDR(base) ((base) + (0x034))
#define SOC_PCTRL_PERI_CTRL13_ADDR(base) ((base) + (0x038))
#define SOC_PCTRL_PERI_CTRL14_ADDR(base) ((base) + (0x03C))
#define SOC_PCTRL_PERI_CTRL15_ADDR(base) ((base) + (0x040))
#define SOC_PCTRL_PERI_CTRL16_ADDR(base) ((base) + (0x044))
#define SOC_PCTRL_PERI_CTRL17_ADDR(base) ((base) + (0x048))
#define SOC_PCTRL_PERI_CTRL18_ADDR(base) ((base) + (0x04C))
#define SOC_PCTRL_PERI_CTRL19_ADDR(base) ((base) + (0x050))
#define SOC_PCTRL_PERI_CTRL20_ADDR(base) ((base) + (0x054))
#define SOC_PCTRL_PERI_CTRL21_ADDR(base) ((base) + (0x058))
#define SOC_PCTRL_PERI_CTRL22_ADDR(base) ((base) + (0x05C))
#define SOC_PCTRL_PERI_CTRL23_ADDR(base) ((base) + (0x060))
#define SOC_PCTRL_PERI_CTRL24_ADDR(base) ((base) + (0x064))
#define SOC_PCTRL_PERI_CTRL25_ADDR(base) ((base) + (0x068))
#define SOC_PCTRL_PERI_CTRL26_ADDR(base) ((base) + (0x06C))
#define SOC_PCTRL_PERI_CTRL27_ADDR(base) ((base) + (0x070))
#define SOC_PCTRL_PERI_CTRL28_ADDR(base) ((base) + (0x074))
#define SOC_PCTRL_PERI_CTRL29_ADDR(base) ((base) + (0x078))
#define SOC_PCTRL_PERI_CTRL30_ADDR(base) ((base) + (0x07C))
#define SOC_PCTRL_PERI_CTRL31_ADDR(base) ((base) + (0x080))
#define SOC_PCTRL_PERI_CTRL32_ADDR(base) ((base) + (0x084))
#define SOC_PCTRL_PERI_CTRL33_ADDR(base) ((base) + (0x088))
#define SOC_PCTRL_PERI_STAT0_ADDR(base) ((base) + (0x094))
#define SOC_PCTRL_PERI_STAT1_ADDR(base) ((base) + (0x098))
#define SOC_PCTRL_PERI_STAT2_ADDR(base) ((base) + (0x09C))
#define SOC_PCTRL_PERI_STAT3_ADDR(base) ((base) + (0x0A0))
#define SOC_PCTRL_PERI_STAT4_ADDR(base) ((base) + (0x0A4))
#define SOC_PCTRL_PERI_STAT5_ADDR(base) ((base) + (0x0A8))
#define SOC_PCTRL_PERI_STAT6_ADDR(base) ((base) + (0x0AC))
#define SOC_PCTRL_PERI_STAT7_ADDR(base) ((base) + (0x0B0))
#define SOC_PCTRL_PERI_STAT8_ADDR(base) ((base) + (0x0B4))
#define SOC_PCTRL_PERI_STAT9_ADDR(base) ((base) + (0x0B8))
#define SOC_PCTRL_PERI_STAT10_ADDR(base) ((base) + (0x0BC))
#define SOC_PCTRL_PERI_STAT11_ADDR(base) ((base) + (0x0C0))
#define SOC_PCTRL_PERI_STAT12_ADDR(base) ((base) + (0x0C4))
#define SOC_PCTRL_PERI_STAT13_ADDR(base) ((base) + (0x0C8))
#define SOC_PCTRL_PERI_STAT14_ADDR(base) ((base) + (0x0CC))
#define SOC_PCTRL_PERI_STAT15_ADDR(base) ((base) + (0x0D0))
#define SOC_PCTRL_PERI_STAT16_ADDR(base) ((base) + (0x0D4))
#define SOC_PCTRL_PERI_STAT17_ADDR(base) ((base) + (0x0D8))
#define SOC_PCTRL_PERI_STAT18_ADDR(base) ((base) + (0x0DC))
#define SOC_PCTRL_PERI_STAT19_ADDR(base) ((base) + (0x0E0))
#define SOC_PCTRL_USB2_HOST_CTRL0_ADDR(base) ((base) + (0x0F0))
#define SOC_PCTRL_USB2_HOST_CTRL1_ADDR(base) ((base) + (0x0F4))
#define SOC_PCTRL_USB2_HOST_CTRL2_ADDR(base) ((base) + (0x0F8))
#define SOC_PCTRL_PERI_STAT29_ADDR(base) ((base) + (0x100))
#define SOC_PCTRL_PERI_STAT30_ADDR(base) ((base) + (0x104))
#define SOC_PCTRL_PERI_STAT31_ADDR(base) ((base) + (0x108))
#define SOC_PCTRL_PERI_STAT32_ADDR(base) ((base) + (0x10C))
#define SOC_PCTRL_PERI_STAT33_ADDR(base) ((base) + (0x110))
#define SOC_PCTRL_PERI_STAT34_ADDR(base) ((base) + (0x114))
#define SOC_PCTRL_PERI_STAT35_ADDR(base) ((base) + (0x118))
#define SOC_PCTRL_PERI_STAT36_ADDR(base) ((base) + (0x11C))
#define SOC_PCTRL_PERI_STAT37_ADDR(base) ((base) + (0x120))
#define SOC_PCTRL_PERI_STAT38_ADDR(base) ((base) + (0x124))
#define SOC_PCTRL_PERI_STAT39_ADDR(base) ((base) + (0x128))
#define SOC_PCTRL_PERI_STAT40_ADDR(base) ((base) + (0x12C))
#define SOC_PCTRL_PERI_STAT41_ADDR(base) ((base) + (0x130))
#define SOC_PCTRL_PERI_STAT42_ADDR(base) ((base) + (0x134))
#define SOC_PCTRL_PERI_STAT43_ADDR(base) ((base) + (0x138))
#define SOC_PCTRL_PERI_STAT44_ADDR(base) ((base) + (0x13C))
#define SOC_PCTRL_PERI_STAT45_ADDR(base) ((base) + (0x140))
#define SOC_PCTRL_PERI_STAT46_ADDR(base) ((base) + (0x144))
#define SOC_PCTRL_PERI_STAT47_ADDR(base) ((base) + (0x148))
#define SOC_PCTRL_PERI_STAT48_ADDR(base) ((base) + (0x14C))
#define SOC_PCTRL_PERI_STAT49_ADDR(base) ((base) + (0x150))
#define SOC_PCTRL_PERI_STAT50_ADDR(base) ((base) + (0x154))
#define SOC_PCTRL_PERI_STAT51_ADDR(base) ((base) + (0x158))
#define SOC_PCTRL_PERI_STAT52_ADDR(base) ((base) + (0x15C))
#define SOC_PCTRL_PERI_STAT53_ADDR(base) ((base) + (0x160))
#define SOC_PCTRL_PERI_STAT54_ADDR(base) ((base) + (0x164))
#define SOC_PCTRL_PERI_STAT55_ADDR(base) ((base) + (0x168))
#define SOC_PCTRL_PERI_STAT56_ADDR(base) ((base) + (0x16C))
#define SOC_PCTRL_PERI_STAT57_ADDR(base) ((base) + (0x170))
#define SOC_PCTRL_PERI_STAT58_ADDR(base) ((base) + (0x174))
#define SOC_PCTRL_PERI_STAT59_ADDR(base) ((base) + (0x178))
#define SOC_PCTRL_PERI_STAT60_ADDR(base) ((base) + (0x17C))
#define SOC_PCTRL_PERI_STAT61_ADDR(base) ((base) + (0x180))
#define SOC_PCTRL_PERI_STAT62_ADDR(base) ((base) + (0x184))
#define SOC_PCTRL_PERI_STAT63_ADDR(base) ((base) + (0x188))
#define SOC_PCTRL_PERI_STAT64_ADDR(base) ((base) + (0x18C))
#define SOC_PCTRL_PERI_STAT66_ADDR(base) ((base) + (0x190))
#define SOC_PCTRL_PERI_STAT67_ADDR(base) ((base) + (0x194))
#define SOC_PCTRL_PERI_STAT68_ADDR(base) ((base) + (0x198))
#define SOC_PCTRL_PERI_STAT69_ADDR(base) ((base) + (0x19C))
#define SOC_PCTRL_PERI_STAT70_ADDR(base) ((base) + (0x1A0))
#define SOC_PCTRL_PERI_STAT71_ADDR(base) ((base) + (0x1A4))
#define SOC_PCTRL_PERI_STAT72_ADDR(base) ((base) + (0x1A8))
#define SOC_PCTRL_PERI_STAT73_ADDR(base) ((base) + (0x1AC))
#define SOC_PCTRL_PERI_STAT74_ADDR(base) ((base) + (0x1B0))
#define SOC_PCTRL_PERI_STAT75_ADDR(base) ((base) + (0x1B4))
#define SOC_PCTRL_PERI_STAT76_ADDR(base) ((base) + (0x1B8))
#define SOC_PCTRL_PERI_STAT77_ADDR(base) ((base) + (0x1BC))
#define SOC_PCTRL_PERI_STAT78_ADDR(base) ((base) + (0x1C0))
#define SOC_PCTRL_PERI_STAT79_ADDR(base) ((base) + (0x1C4))
#define SOC_PCTRL_PERI_STAT80_ADDR(base) ((base) + (0x1C8))
#define SOC_PCTRL_PERI_STAT81_ADDR(base) ((base) + (0x1CC))
#define SOC_PCTRL_PERI_STAT82_ADDR(base) ((base) + (0x1D0))
#define SOC_PCTRL_PERI_STAT83_ADDR(base) ((base) + (0x1D4))
#define SOC_PCTRL_PERI_STAT84_ADDR(base) ((base) + (0x1D8))
#define SOC_PCTRL_PERI_STAT85_ADDR(base) ((base) + (0x1DC))
#define SOC_PCTRL_PERI_STAT86_ADDR(base) ((base) + (0x1E0))
#define SOC_PCTRL_PERI_STAT87_ADDR(base) ((base) + (0x1E4))
#define SOC_PCTRL_PERI_STAT88_ADDR(base) ((base) + (0x1E8))
#define SOC_PCTRL_PERI_STAT89_ADDR(base) ((base) + (0x1EC))
#define SOC_PCTRL_PERI_STAT90_ADDR(base) ((base) + (0x1F0))
#define SOC_PCTRL_PERI_STAT91_ADDR(base) ((base) + (0x1F4))
#define SOC_PCTRL_PERI_STAT92_ADDR(base) ((base) + (0x1F8))
#define SOC_PCTRL_PERI_STAT93_ADDR(base) ((base) + (0x1FC))
#define SOC_PCTRL_PERI_CTRL67_ADDR(base) ((base) + (0x200))
#define SOC_PCTRL_PERI_CTRL68_ADDR(base) ((base) + (0x204))
#define SOC_PCTRL_PERI_CTRL69_ADDR(base) ((base) + (0x208))
#define SOC_PCTRL_PERI_CTRL70_ADDR(base) ((base) + (0x20C))
#define SOC_PCTRL_PERI_CTRL71_ADDR(base) ((base) + (0x210))
#define SOC_PCTRL_PERI_CTRL72_ADDR(base) ((base) + (0x214))
#define SOC_PCTRL_PERI_CTRL73_ADDR(base) ((base) + (0x218))
#define SOC_PCTRL_PERI_CTRL74_ADDR(base) ((base) + (0x21C))
#define SOC_PCTRL_PERI_CTRL88_ADDR(base) ((base) + (0x220))
#define SOC_PCTRL_PERI_CTRL89_ADDR(base) ((base) + (0x224))
#define SOC_PCTRL_PERI_CTRL90_ADDR(base) ((base) + (0x228))
#define SOC_PCTRL_PERI_CTRL91_ADDR(base) ((base) + (0x22C))
#define SOC_PCTRL_PERI_CTRL92_ADDR(base) ((base) + (0x230))
#define SOC_PCTRL_PERI_CTRL93_ADDR(base) ((base) + (0x234))
#define SOC_PCTRL_PERI_CTRL95_ADDR(base) ((base) + (0x238))
#define SOC_PCTRL_PERI_CTRL96_ADDR(base) ((base) + (0x23C))
#define SOC_PCTRL_PERI_CTRL97_ADDR(base) ((base) + (0x240))
#define SOC_PCTRL_PERI_CTRL105_ADDR(base) ((base) + (0x244))
#define SOC_PCTRL_PERI_CTRL106_ADDR(base) ((base) + (0x248))
#define SOC_PCTRL_PERI_CTRL107_ADDR(base) ((base) + (0x24C))
#define SOC_PCTRL_PERI_CTRL108_ADDR(base) ((base) + (0x250))
#define SOC_PCTRL_PERI_CTRL109_ADDR(base) ((base) + (0x254))
#define SOC_PCTRL_PERI_CTRL110_ADDR(base) ((base) + (0x258))
#define SOC_PCTRL_PERI_CTRL111_ADDR(base) ((base) + (0x25C))
#define SOC_PCTRL_PERI_CTRL112_ADDR(base) ((base) + (0x260))
#define SOC_PCTRL_PERI_CTRL113_ADDR(base) ((base) + (0x264))
#define SOC_PCTRL_PERI_CTRL114_ADDR(base) ((base) + (0x268))
#define SOC_PCTRL_PERI_CTRL115_ADDR(base) ((base) + (0x26C))
#define SOC_PCTRL_PERI_CTRL116_ADDR(base) ((base) + (0x270))
#define SOC_PCTRL_PERI_CTRL117_ADDR(base) ((base) + (0x274))
#define SOC_PCTRL_PERI_CTRL118_ADDR(base) ((base) + (0x278))
#define SOC_PCTRL_PERI_CTRL119_ADDR(base) ((base) + (0x27C))
#define SOC_PCTRL_PERI_CTRL120_ADDR(base) ((base) + (0x280))
#define SOC_PCTRL_PERI_CTRL121_ADDR(base) ((base) + (0x284))
#define SOC_PCTRL_PERI_CTRL122_ADDR(base) ((base) + (0x288))
#define SOC_PCTRL_PERI_CTRL123_ADDR(base) ((base) + (0x28C))
#define SOC_PCTRL_PERI_CTRL124_ADDR(base) ((base) + (0x290))
#define SOC_PCTRL_PERI_CTRL125_ADDR(base) ((base) + (0x294))
#define SOC_PCTRL_PERI_CTRL126_ADDR(base) ((base) + (0x298))
#define SOC_PCTRL_PERI_CTRL127_ADDR(base) ((base) + (0x29C))
#define SOC_PCTRL_PERI_CTRL128_ADDR(base) ((base) + (0x2A0))
#define SOC_PCTRL_PERI_CTRL129_ADDR(base) ((base) + (0x2A4))
#define SOC_PCTRL_PERI_CTRL130_ADDR(base) ((base) + (0x2A8))
#define SOC_PCTRL_PERI_CTRL131_ADDR(base) ((base) + (0x2AC))
#define SOC_PCTRL_PERI_CTRL132_ADDR(base) ((base) + (0x2B0))
#define SOC_PCTRL_PERI_CTRL133_ADDR(base) ((base) + (0x2B4))
#define SOC_PCTRL_PERI_CTRL134_ADDR(base) ((base) + (0x2B8))
#define SOC_PCTRL_PERI_CTRL135_ADDR(base) ((base) + (0x2BC))
#define SOC_PCTRL_PERI_CTRL136_ADDR(base) ((base) + (0x2C0))
#define SOC_PCTRL_PERI_CTRL137_ADDR(base) ((base) + (0x2C4))
#define SOC_PCTRL_PERI_CTRL138_ADDR(base) ((base) + (0x2C8))
#define SOC_PCTRL_PERI_CTRL139_ADDR(base) ((base) + (0x2CC))
#define SOC_PCTRL_PERI_CTRL140_ADDR(base) ((base) + (0x2D0))
#define SOC_PCTRL_RESOURCE0_LOCK_ADDR(base) ((base) + (0x400))
#define SOC_PCTRL_RESOURCE0_UNLOCK_ADDR(base) ((base) + (0x404))
#define SOC_PCTRL_RESOURCE0_LOCK_ST_ADDR(base) ((base) + (0x408))
#define SOC_PCTRL_RESOURCE1_LOCK_ADDR(base) ((base) + (0x40C))
#define SOC_PCTRL_RESOURCE1_UNLOCK_ADDR(base) ((base) + (0x410))
#define SOC_PCTRL_RESOURCE1_LOCK_ST_ADDR(base) ((base) + (0x414))
#define SOC_PCTRL_RESOURCE2_LOCK_ADDR(base) ((base) + (0x418))
#define SOC_PCTRL_RESOURCE2_UNLOCK_ADDR(base) ((base) + (0x41C))
#define SOC_PCTRL_RESOURCE2_LOCK_ST_ADDR(base) ((base) + (0x420))
#define SOC_PCTRL_RESOURCE3_LOCK_ADDR(base) ((base) + (0x424))
#define SOC_PCTRL_RESOURCE3_UNLOCK_ADDR(base) ((base) + (0x428))
#define SOC_PCTRL_RESOURCE3_LOCK_ST_ADDR(base) ((base) + (0x42C))
#define SOC_PCTRL_RESOURCE4_LOCK_ADDR(base) ((base) + (0x800))
#define SOC_PCTRL_RESOURCE4_UNLOCK_ADDR(base) ((base) + (0x804))
#define SOC_PCTRL_RESOURCE4_LOCK_ST_ADDR(base) ((base) + (0x808))
#define SOC_PCTRL_RESOURCE5_LOCK_ADDR(base) ((base) + (0x80C))
#define SOC_PCTRL_RESOURCE5_UNLOCK_ADDR(base) ((base) + (0x810))
#define SOC_PCTRL_RESOURCE5_LOCK_ST_ADDR(base) ((base) + (0x814))
#define SOC_PCTRL_RESOURCE6_LOCK_ADDR(base) ((base) + (0x818))
#define SOC_PCTRL_RESOURCE6_UNLOCK_ADDR(base) ((base) + (0x81C))
#define SOC_PCTRL_RESOURCE6_LOCK_ST_ADDR(base) ((base) + (0x820))
#define SOC_PCTRL_RESOURCE7_LOCK_ADDR(base) ((base) + (0x824))
#define SOC_PCTRL_RESOURCE7_UNLOCK_ADDR(base) ((base) + (0x828))
#define SOC_PCTRL_RESOURCE7_LOCK_ST_ADDR(base) ((base) + (0x82C))
#define SOC_PCTRL_PERI_CTRL5_ADDR(base) ((base) + (0xC00))
#define SOC_PCTRL_PERI_CTRL6_ADDR(base) ((base) + (0xC04))
#define SOC_PCTRL_PERI_CTRL7_ADDR(base) ((base) + (0xC08))
#define SOC_PCTRL_PERI_CTRL8_ADDR(base) ((base) + (0xC0C))
#define SOC_PCTRL_PERI_CTRL9_ADDR(base) ((base) + (0xC10))
#define SOC_PCTRL_PERI_CTRL10_ADDR(base) ((base) + (0xC14))
#define SOC_PCTRL_PERI_CTRL11_ADDR(base) ((base) + (0xC18))
#define SOC_PCTRL_PERI_CTRL34_ADDR(base) ((base) + (0xC1C))
#define SOC_PCTRL_PERI_CTRL35_ADDR(base) ((base) + (0xC20))
#define SOC_PCTRL_PERI_CTRL38_ADDR(base) ((base) + (0xC2C))
#define SOC_PCTRL_PERI_CTRL39_ADDR(base) ((base) + (0xC30))
#define SOC_PCTRL_PERI_CTRL40_ADDR(base) ((base) + (0xC34))
#define SOC_PCTRL_PERI_CTRL41_ADDR(base) ((base) + (0xC38))
#define SOC_PCTRL_PERI_CTRL42_ADDR(base) ((base) + (0xC3C))
#define SOC_PCTRL_PERI_CTRL43_ADDR(base) ((base) + (0xC40))
#define SOC_PCTRL_PERI_CTRL44_ADDR(base) ((base) + (0xC44))
#define SOC_PCTRL_PERI_CTRL45_ADDR(base) ((base) + (0xC48))
#define SOC_PCTRL_PERI_CTRL46_ADDR(base) ((base) + (0xC4C))
#define SOC_PCTRL_PERI_CTRL47_ADDR(base) ((base) + (0xC50))
#define SOC_PCTRL_PERI_CTRL48_ADDR(base) ((base) + (0xC54))
#define SOC_PCTRL_PERI_CTRL49_ADDR(base) ((base) + (0xC58))
#define SOC_PCTRL_PERI_CTRL50_ADDR(base) ((base) + (0xC5C))
#define SOC_PCTRL_PERI_CTRL51_ADDR(base) ((base) + (0xC60))
#define SOC_PCTRL_PERI_CTRL52_ADDR(base) ((base) + (0xC64))
#define SOC_PCTRL_PERI_CTRL53_ADDR(base) ((base) + (0xC68))
#define SOC_PCTRL_PERI_CTRL54_ADDR(base) ((base) + (0xC6C))
#define SOC_PCTRL_PERI_CTRL55_ADDR(base) ((base) + (0xC70))
#define SOC_PCTRL_PERI_CTRL56_ADDR(base) ((base) + (0xC74))
#define SOC_PCTRL_PERI_CTRL57_ADDR(base) ((base) + (0xC78))
#define SOC_PCTRL_PERI_CTRL58_ADDR(base) ((base) + (0xC7C))
#define SOC_PCTRL_PERI_CTRL59_ADDR(base) ((base) + (0xC80))
#define SOC_PCTRL_PERI_CTRL60_ADDR(base) ((base) + (0xC84))
#define SOC_PCTRL_PERI_CTRL61_ADDR(base) ((base) + (0xC88))
#define SOC_PCTRL_PERI_CTRL62_ADDR(base) ((base) + (0xC8C))
#define SOC_PCTRL_PERI_CTRL63_ADDR(base) ((base) + (0xC90))
#define SOC_PCTRL_PERI_CTRL64_ADDR(base) ((base) + (0xC94))
#define SOC_PCTRL_PERI_CTRL65_ADDR(base) ((base) + (0xC98))
#define SOC_PCTRL_PERI_CTRL66_ADDR(base) ((base) + (0xC9C))
#define SOC_PCTRL_PERI_CTRL75_ADDR(base) ((base) + (0xCA0))
#define SOC_PCTRL_PERI_CTRL76_ADDR(base) ((base) + (0xCA4))
#define SOC_PCTRL_PERI_CTRL77_ADDR(base) ((base) + (0xCA8))
#define SOC_PCTRL_PERI_CTRL78_ADDR(base) ((base) + (0xCAC))
#define SOC_PCTRL_PERI_CTRL79_ADDR(base) ((base) + (0xCB0))
#define SOC_PCTRL_PERI_CTRL80_ADDR(base) ((base) + (0xCB4))
#define SOC_PCTRL_PERI_CTRL81_ADDR(base) ((base) + (0xCB8))
#define SOC_PCTRL_PERI_CTRL82_ADDR(base) ((base) + (0xCBC))
#define SOC_PCTRL_PERI_CTRL83_ADDR(base) ((base) + (0xCC0))
#define SOC_PCTRL_PERI_CTRL84_ADDR(base) ((base) + (0xCC4))
#define SOC_PCTRL_PERI_CTRL85_ADDR(base) ((base) + (0xCC8))
#define SOC_PCTRL_PERI_CTRL86_ADDR(base) ((base) + (0xCCC))
#define SOC_PCTRL_PERI_CTRL87_ADDR(base) ((base) + (0xCD0))
#define SOC_PCTRL_PERI_CTRL94_ADDR(base) ((base) + (0xCD4))
#define SOC_PCTRL_PERI_CTRL141_ADDR(base) ((base) + (0xCD8))
#define SOC_PCTRL_PERI_CTRL142_ADDR(base) ((base) + (0xCDC))
#define SOC_PCTRL_PERI_CTRL143_ADDR(base) ((base) + (0xCE0))
#define SOC_PCTRL_PERI_CTRL144_ADDR(base) ((base) + (0xCE4))
#define SOC_PCTRL_PERI_CTRL145_ADDR(base) ((base) + (0xCE8))
#define SOC_PCTRL_PERI_CTRL146_ADDR(base) ((base) + (0xCEC))
#define SOC_PCTRL_PERI_CTRL147_ADDR(base) ((base) + (0xCF0))
#define SOC_PCTRL_PERI_CTRL148_ADDR(base) ((base) + (0xCF4))
#define SOC_PCTRL_PERI_STAT20_ADDR(base) ((base) + (0xE00))
#define SOC_PCTRL_PERI_STAT21_ADDR(base) ((base) + (0xE04))
#define SOC_PCTRL_PERI_STAT22_ADDR(base) ((base) + (0xE08))
#define SOC_PCTRL_PERI_STAT23_ADDR(base) ((base) + (0xE0C))
#define SOC_PCTRL_PERI_STAT24_ADDR(base) ((base) + (0xE10))
#define SOC_PCTRL_PERI_STAT25_ADDR(base) ((base) + (0xE14))
#define SOC_PCTRL_PERI_STAT26_ADDR(base) ((base) + (0xE18))
#define SOC_PCTRL_PERI_STAT27_ADDR(base) ((base) + (0xE1C))
#define SOC_PCTRL_PERI_STAT28_ADDR(base) ((base) + (0xE20))
#define SOC_PCTRL_PERI_STAT65_ADDR(base) ((base) + (0xE24))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int g3d_div : 10;
        unsigned int portrait_landscape : 1;
        unsigned int lcd_3d_2d : 1;
        unsigned int g3d_raster_en : 1;
        unsigned int lcd_3d_sw_inv : 4;
        unsigned int reserved : 15;
    } reg;
} SOC_PCTRL_G3D_RASTER_UNION;
#endif
#define SOC_PCTRL_G3D_RASTER_g3d_div_START (0)
#define SOC_PCTRL_G3D_RASTER_g3d_div_END (9)
#define SOC_PCTRL_G3D_RASTER_portrait_landscape_START (10)
#define SOC_PCTRL_G3D_RASTER_portrait_landscape_END (10)
#define SOC_PCTRL_G3D_RASTER_lcd_3d_2d_START (11)
#define SOC_PCTRL_G3D_RASTER_lcd_3d_2d_END (11)
#define SOC_PCTRL_G3D_RASTER_g3d_raster_en_START (12)
#define SOC_PCTRL_G3D_RASTER_g3d_raster_en_END (12)
#define SOC_PCTRL_G3D_RASTER_lcd_3d_sw_inv_START (13)
#define SOC_PCTRL_G3D_RASTER_lcd_3d_sw_inv_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tidm_core_hpm_en : 1;
        unsigned int tidm_core_hpm_res_sel : 1;
        unsigned int tidm_core_tidm_mode : 2;
        unsigned int tidm_core_hpm_div : 6;
        unsigned int peri_ctrl0_cmd : 6;
        unsigned int peri_ctrl0_msk : 16;
    } reg;
} SOC_PCTRL_PERI_CTRL0_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL0_tidm_core_hpm_en_START (0)
#define SOC_PCTRL_PERI_CTRL0_tidm_core_hpm_en_END (0)
#define SOC_PCTRL_PERI_CTRL0_tidm_core_hpm_res_sel_START (1)
#define SOC_PCTRL_PERI_CTRL0_tidm_core_hpm_res_sel_END (1)
#define SOC_PCTRL_PERI_CTRL0_tidm_core_tidm_mode_START (2)
#define SOC_PCTRL_PERI_CTRL0_tidm_core_tidm_mode_END (3)
#define SOC_PCTRL_PERI_CTRL0_tidm_core_hpm_div_START (4)
#define SOC_PCTRL_PERI_CTRL0_tidm_core_hpm_div_END (9)
#define SOC_PCTRL_PERI_CTRL0_peri_ctrl0_cmd_START (10)
#define SOC_PCTRL_PERI_CTRL0_peri_ctrl0_cmd_END (15)
#define SOC_PCTRL_PERI_CTRL0_peri_ctrl0_msk_START (16)
#define SOC_PCTRL_PERI_CTRL0_peri_ctrl0_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int test_point_sel : 4;
        unsigned int test_point_ioc_sel : 3;
        unsigned int peri_ctrl1_cmd : 9;
        unsigned int peri_ctrl1_msk : 16;
    } reg;
} SOC_PCTRL_PERI_CTRL1_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL1_test_point_sel_START (0)
#define SOC_PCTRL_PERI_CTRL1_test_point_sel_END (3)
#define SOC_PCTRL_PERI_CTRL1_test_point_ioc_sel_START (4)
#define SOC_PCTRL_PERI_CTRL1_test_point_ioc_sel_END (6)
#define SOC_PCTRL_PERI_CTRL1_peri_ctrl1_cmd_START (7)
#define SOC_PCTRL_PERI_CTRL1_peri_ctrl1_cmd_END (15)
#define SOC_PCTRL_PERI_CTRL1_peri_ctrl1_msk_START (16)
#define SOC_PCTRL_PERI_CTRL1_peri_ctrl1_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pctrl_mem_ctrl_sd_dmc_pack : 4;
        unsigned int pctrl_mem_ctrl_sd_iputop : 1;
        unsigned int pctrl_mem_ctrl_sd_aicore1 : 1;
        unsigned int pctrl_mem_ctrl_sd_tiny : 1;
        unsigned int pctrl_mem_ctrl_sd_aicore0 : 1;
        unsigned int pctrl_mem_ctrl_sd_tscpu : 1;
        unsigned int pctrl_mem_ctrl_sd_pac : 5;
        unsigned int peri_ctrl2_cmd : 2;
        unsigned int peri_ctrl2_msk : 16;
    } reg;
} SOC_PCTRL_PERI_CTRL2_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL2_pctrl_mem_ctrl_sd_dmc_pack_START (0)
#define SOC_PCTRL_PERI_CTRL2_pctrl_mem_ctrl_sd_dmc_pack_END (3)
#define SOC_PCTRL_PERI_CTRL2_pctrl_mem_ctrl_sd_iputop_START (4)
#define SOC_PCTRL_PERI_CTRL2_pctrl_mem_ctrl_sd_iputop_END (4)
#define SOC_PCTRL_PERI_CTRL2_pctrl_mem_ctrl_sd_aicore1_START (5)
#define SOC_PCTRL_PERI_CTRL2_pctrl_mem_ctrl_sd_aicore1_END (5)
#define SOC_PCTRL_PERI_CTRL2_pctrl_mem_ctrl_sd_tiny_START (6)
#define SOC_PCTRL_PERI_CTRL2_pctrl_mem_ctrl_sd_tiny_END (6)
#define SOC_PCTRL_PERI_CTRL2_pctrl_mem_ctrl_sd_aicore0_START (7)
#define SOC_PCTRL_PERI_CTRL2_pctrl_mem_ctrl_sd_aicore0_END (7)
#define SOC_PCTRL_PERI_CTRL2_pctrl_mem_ctrl_sd_tscpu_START (8)
#define SOC_PCTRL_PERI_CTRL2_pctrl_mem_ctrl_sd_tscpu_END (8)
#define SOC_PCTRL_PERI_CTRL2_pctrl_mem_ctrl_sd_pac_START (9)
#define SOC_PCTRL_PERI_CTRL2_pctrl_mem_ctrl_sd_pac_END (13)
#define SOC_PCTRL_PERI_CTRL2_peri_ctrl2_cmd_START (14)
#define SOC_PCTRL_PERI_CTRL2_peri_ctrl2_cmd_END (15)
#define SOC_PCTRL_PERI_CTRL2_peri_ctrl2_msk_START (16)
#define SOC_PCTRL_PERI_CTRL2_peri_ctrl2_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 15;
        unsigned int peri_ctrl3_msk : 16;
    } reg;
} SOC_PCTRL_PERI_CTRL3_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL3_peri_ctrl3_msk_START (16)
#define SOC_PCTRL_PERI_CTRL3_peri_ctrl3_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int g3d_tprf_tselr_lv : 2;
        unsigned int g3d_tprf_tselw_lv : 2;
        unsigned int g3d_tprf_test_lv : 3;
        unsigned int reserved : 25;
    } reg;
} SOC_PCTRL_PERI_CTRL4_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL4_g3d_tprf_tselr_lv_START (0)
#define SOC_PCTRL_PERI_CTRL4_g3d_tprf_tselr_lv_END (1)
#define SOC_PCTRL_PERI_CTRL4_g3d_tprf_tselw_lv_START (2)
#define SOC_PCTRL_PERI_CTRL4_g3d_tprf_tselw_lv_END (3)
#define SOC_PCTRL_PERI_CTRL4_g3d_tprf_test_lv_START (4)
#define SOC_PCTRL_PERI_CTRL4_g3d_tprf_test_lv_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int bisr_spram_ctrl_slp : 1;
        unsigned int bisr_spram_ctrl_dslp : 1;
        unsigned int bisr_spram_ctrl_sd : 1;
        unsigned int ipf_spram_ctrl_slp : 1;
        unsigned int ipf_spram_ctrl_dslp : 1;
        unsigned int i3c_spram_ctrl_slp : 1;
        unsigned int i3c_spram_ctrl_dslp : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int socp_spram_ctrl_slp : 1;
        unsigned int socp_spram_ctrl_dslp : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 8;
    } reg;
} SOC_PCTRL_PERI_CTRL98_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL98_bisr_spram_ctrl_slp_START (6)
#define SOC_PCTRL_PERI_CTRL98_bisr_spram_ctrl_slp_END (6)
#define SOC_PCTRL_PERI_CTRL98_bisr_spram_ctrl_dslp_START (7)
#define SOC_PCTRL_PERI_CTRL98_bisr_spram_ctrl_dslp_END (7)
#define SOC_PCTRL_PERI_CTRL98_bisr_spram_ctrl_sd_START (8)
#define SOC_PCTRL_PERI_CTRL98_bisr_spram_ctrl_sd_END (8)
#define SOC_PCTRL_PERI_CTRL98_ipf_spram_ctrl_slp_START (9)
#define SOC_PCTRL_PERI_CTRL98_ipf_spram_ctrl_slp_END (9)
#define SOC_PCTRL_PERI_CTRL98_ipf_spram_ctrl_dslp_START (10)
#define SOC_PCTRL_PERI_CTRL98_ipf_spram_ctrl_dslp_END (10)
#define SOC_PCTRL_PERI_CTRL98_i3c_spram_ctrl_slp_START (11)
#define SOC_PCTRL_PERI_CTRL98_i3c_spram_ctrl_slp_END (11)
#define SOC_PCTRL_PERI_CTRL98_i3c_spram_ctrl_dslp_START (12)
#define SOC_PCTRL_PERI_CTRL98_i3c_spram_ctrl_dslp_END (12)
#define SOC_PCTRL_PERI_CTRL98_socp_spram_ctrl_slp_START (18)
#define SOC_PCTRL_PERI_CTRL98_socp_spram_ctrl_slp_END (18)
#define SOC_PCTRL_PERI_CTRL98_socp_spram_ctrl_dslp_START (19)
#define SOC_PCTRL_PERI_CTRL98_socp_spram_ctrl_dslp_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ipf_tprf_ctrl_slp : 1;
        unsigned int ipf_tprf_ctrl_dslp : 1;
        unsigned int reserved_0 : 1;
        unsigned int socp_tprf_ctrl_slp : 1;
        unsigned int socp_tprf_ctrl_dslp : 1;
        unsigned int reserved_1 : 1;
        unsigned int peri_edmac_tprf_ctrl_slp : 1;
        unsigned int peri_edmac_tprf_ctrl_dslp : 1;
        unsigned int reserved_2 : 1;
        unsigned int spi_bpram_ctrl_slp : 1;
        unsigned int spi_bpram_ctrl_dslp : 1;
        unsigned int aximem_spram_ctrl_slp : 1;
        unsigned int aximem_spram_ctrl_dslp : 1;
        unsigned int spe_spram_ctrl_slp : 1;
        unsigned int spe_spram_ctrl_dslp : 1;
        unsigned int spe_tpram_ctrl_slp : 1;
        unsigned int spe_tpram_ctrl_dslp : 1;
        unsigned int maa_spram_ctrl_slp : 1;
        unsigned int maa_spram_ctrl_dslp : 1;
        unsigned int maa_tpram_ctrl_slp : 1;
        unsigned int maa_tpram_ctrl_dslp : 1;
        unsigned int reserved_3 : 11;
    } reg;
} SOC_PCTRL_PERI_CTRL99_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL99_ipf_tprf_ctrl_slp_START (0)
#define SOC_PCTRL_PERI_CTRL99_ipf_tprf_ctrl_slp_END (0)
#define SOC_PCTRL_PERI_CTRL99_ipf_tprf_ctrl_dslp_START (1)
#define SOC_PCTRL_PERI_CTRL99_ipf_tprf_ctrl_dslp_END (1)
#define SOC_PCTRL_PERI_CTRL99_socp_tprf_ctrl_slp_START (3)
#define SOC_PCTRL_PERI_CTRL99_socp_tprf_ctrl_slp_END (3)
#define SOC_PCTRL_PERI_CTRL99_socp_tprf_ctrl_dslp_START (4)
#define SOC_PCTRL_PERI_CTRL99_socp_tprf_ctrl_dslp_END (4)
#define SOC_PCTRL_PERI_CTRL99_peri_edmac_tprf_ctrl_slp_START (6)
#define SOC_PCTRL_PERI_CTRL99_peri_edmac_tprf_ctrl_slp_END (6)
#define SOC_PCTRL_PERI_CTRL99_peri_edmac_tprf_ctrl_dslp_START (7)
#define SOC_PCTRL_PERI_CTRL99_peri_edmac_tprf_ctrl_dslp_END (7)
#define SOC_PCTRL_PERI_CTRL99_spi_bpram_ctrl_slp_START (9)
#define SOC_PCTRL_PERI_CTRL99_spi_bpram_ctrl_slp_END (9)
#define SOC_PCTRL_PERI_CTRL99_spi_bpram_ctrl_dslp_START (10)
#define SOC_PCTRL_PERI_CTRL99_spi_bpram_ctrl_dslp_END (10)
#define SOC_PCTRL_PERI_CTRL99_aximem_spram_ctrl_slp_START (11)
#define SOC_PCTRL_PERI_CTRL99_aximem_spram_ctrl_slp_END (11)
#define SOC_PCTRL_PERI_CTRL99_aximem_spram_ctrl_dslp_START (12)
#define SOC_PCTRL_PERI_CTRL99_aximem_spram_ctrl_dslp_END (12)
#define SOC_PCTRL_PERI_CTRL99_spe_spram_ctrl_slp_START (13)
#define SOC_PCTRL_PERI_CTRL99_spe_spram_ctrl_slp_END (13)
#define SOC_PCTRL_PERI_CTRL99_spe_spram_ctrl_dslp_START (14)
#define SOC_PCTRL_PERI_CTRL99_spe_spram_ctrl_dslp_END (14)
#define SOC_PCTRL_PERI_CTRL99_spe_tpram_ctrl_slp_START (15)
#define SOC_PCTRL_PERI_CTRL99_spe_tpram_ctrl_slp_END (15)
#define SOC_PCTRL_PERI_CTRL99_spe_tpram_ctrl_dslp_START (16)
#define SOC_PCTRL_PERI_CTRL99_spe_tpram_ctrl_dslp_END (16)
#define SOC_PCTRL_PERI_CTRL99_maa_spram_ctrl_slp_START (17)
#define SOC_PCTRL_PERI_CTRL99_maa_spram_ctrl_slp_END (17)
#define SOC_PCTRL_PERI_CTRL99_maa_spram_ctrl_dslp_START (18)
#define SOC_PCTRL_PERI_CTRL99_maa_spram_ctrl_dslp_END (18)
#define SOC_PCTRL_PERI_CTRL99_maa_tpram_ctrl_slp_START (19)
#define SOC_PCTRL_PERI_CTRL99_maa_tpram_ctrl_slp_END (19)
#define SOC_PCTRL_PERI_CTRL99_maa_tpram_ctrl_dslp_START (20)
#define SOC_PCTRL_PERI_CTRL99_maa_tpram_ctrl_dslp_END (20)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bootrom_rom_ctrl_dt : 2;
        unsigned int bootrom_rom_ctrl_skp : 2;
        unsigned int bootrom_rom_ctrl_ckle : 1;
        unsigned int bootrom_rom_ctrl_ckhe : 1;
        unsigned int bootrom_rom_ctrl_sd : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 24;
    } reg;
} SOC_PCTRL_PERI_CTRL100_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL100_bootrom_rom_ctrl_dt_START (0)
#define SOC_PCTRL_PERI_CTRL100_bootrom_rom_ctrl_dt_END (1)
#define SOC_PCTRL_PERI_CTRL100_bootrom_rom_ctrl_skp_START (2)
#define SOC_PCTRL_PERI_CTRL100_bootrom_rom_ctrl_skp_END (3)
#define SOC_PCTRL_PERI_CTRL100_bootrom_rom_ctrl_ckle_START (4)
#define SOC_PCTRL_PERI_CTRL100_bootrom_rom_ctrl_ckle_END (4)
#define SOC_PCTRL_PERI_CTRL100_bootrom_rom_ctrl_ckhe_START (5)
#define SOC_PCTRL_PERI_CTRL100_bootrom_rom_ctrl_ckhe_END (5)
#define SOC_PCTRL_PERI_CTRL100_bootrom_rom_ctrl_sd_START (6)
#define SOC_PCTRL_PERI_CTRL100_bootrom_rom_ctrl_sd_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int spsram_tra_peri : 2;
        unsigned int bpram_tra_peri : 2;
        unsigned int reserved_0 : 12;
        unsigned int tprf_tra_peri : 2;
        unsigned int reserved_1 : 14;
    } reg;
} SOC_PCTRL_PERI_CTRL101_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL101_spsram_tra_peri_START (0)
#define SOC_PCTRL_PERI_CTRL101_spsram_tra_peri_END (1)
#define SOC_PCTRL_PERI_CTRL101_bpram_tra_peri_START (2)
#define SOC_PCTRL_PERI_CTRL101_bpram_tra_peri_END (3)
#define SOC_PCTRL_PERI_CTRL101_tprf_tra_peri_START (16)
#define SOC_PCTRL_PERI_CTRL101_tprf_tra_peri_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pctrl_mem_ctrl_sd_peri : 1;
        unsigned int reserved_0 : 1;
        unsigned int pctrl_rom_ctrl_sd_dec : 1;
        unsigned int pctrl_mem_ctrl_sd_dec : 1;
        unsigned int reserved_1 : 1;
        unsigned int pctrl_mem_ctrl_sd_enc : 1;
        unsigned int pctrl_mem_ctrl_sd_ivp1 : 1;
        unsigned int pctrl_mem_ctrl_sd_ivp0 : 1;
        unsigned int pctrl_mem_ctrl_sd_isp_bpecap : 1;
        unsigned int pctrl_mem_ctrl_sd_isp_fe : 1;
        unsigned int reserved_2 : 1;
        unsigned int pctrl_mem_ctrl_sd_isp_noc : 1;
        unsigned int pctrl_mem_ctrl_sd_media1_sub : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int pctrl_mem_ctrl_sd_ipp : 1;
        unsigned int peri_ctrl102_msk : 16;
    } reg;
} SOC_PCTRL_PERI_CTRL102_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_peri_START (0)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_peri_END (0)
#define SOC_PCTRL_PERI_CTRL102_pctrl_rom_ctrl_sd_dec_START (2)
#define SOC_PCTRL_PERI_CTRL102_pctrl_rom_ctrl_sd_dec_END (2)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_dec_START (3)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_dec_END (3)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_enc_START (5)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_enc_END (5)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_ivp1_START (6)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_ivp1_END (6)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_ivp0_START (7)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_ivp0_END (7)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_isp_bpecap_START (8)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_isp_bpecap_END (8)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_isp_fe_START (9)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_isp_fe_END (9)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_isp_noc_START (11)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_isp_noc_END (11)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_media1_sub_START (12)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_media1_sub_END (12)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_ipp_START (15)
#define SOC_PCTRL_PERI_CTRL102_pctrl_mem_ctrl_sd_ipp_END (15)
#define SOC_PCTRL_PERI_CTRL102_peri_ctrl102_msk_START (16)
#define SOC_PCTRL_PERI_CTRL102_peri_ctrl102_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 8;
        unsigned int pctrl_rom_ctrl_sd_dmc_pack_a : 1;
        unsigned int pctrl_rom_ctrl_sd_dmc_pack_b : 1;
        unsigned int pctrl_rom_ctrl_sd_dmc_pack_c : 1;
        unsigned int pctrl_rom_ctrl_sd_dmc_pack_d : 1;
        unsigned int pctrl_mem_ctrl_sd_dmss_sc0 : 1;
        unsigned int pctrl_mem_ctrl_sd_dmss_sc1 : 1;
        unsigned int pctrl_mem_ctrl_sd_mddrc : 1;
        unsigned int reserved_1 : 1;
        unsigned int peri_ctrl103_msk : 16;
    } reg;
} SOC_PCTRL_PERI_CTRL103_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL103_pctrl_rom_ctrl_sd_dmc_pack_a_START (8)
#define SOC_PCTRL_PERI_CTRL103_pctrl_rom_ctrl_sd_dmc_pack_a_END (8)
#define SOC_PCTRL_PERI_CTRL103_pctrl_rom_ctrl_sd_dmc_pack_b_START (9)
#define SOC_PCTRL_PERI_CTRL103_pctrl_rom_ctrl_sd_dmc_pack_b_END (9)
#define SOC_PCTRL_PERI_CTRL103_pctrl_rom_ctrl_sd_dmc_pack_c_START (10)
#define SOC_PCTRL_PERI_CTRL103_pctrl_rom_ctrl_sd_dmc_pack_c_END (10)
#define SOC_PCTRL_PERI_CTRL103_pctrl_rom_ctrl_sd_dmc_pack_d_START (11)
#define SOC_PCTRL_PERI_CTRL103_pctrl_rom_ctrl_sd_dmc_pack_d_END (11)
#define SOC_PCTRL_PERI_CTRL103_pctrl_mem_ctrl_sd_dmss_sc0_START (12)
#define SOC_PCTRL_PERI_CTRL103_pctrl_mem_ctrl_sd_dmss_sc0_END (12)
#define SOC_PCTRL_PERI_CTRL103_pctrl_mem_ctrl_sd_dmss_sc1_START (13)
#define SOC_PCTRL_PERI_CTRL103_pctrl_mem_ctrl_sd_dmss_sc1_END (13)
#define SOC_PCTRL_PERI_CTRL103_pctrl_mem_ctrl_sd_mddrc_START (14)
#define SOC_PCTRL_PERI_CTRL103_pctrl_mem_ctrl_sd_mddrc_END (14)
#define SOC_PCTRL_PERI_CTRL103_peri_ctrl103_msk_START (16)
#define SOC_PCTRL_PERI_CTRL103_peri_ctrl103_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ipf_spram_auto_clk_enable_mask : 1;
        unsigned int socp_spram_auto_clk_enable_mask : 1;
        unsigned int ipf_tpram_auto_clk_enable_mask : 1;
        unsigned int socp_tpram_auto_clk_enable_mask : 1;
        unsigned int aximem_spram_auto_clk_enable_mask : 1;
        unsigned int spe_spram_auto_clk_enable_mask : 1;
        unsigned int spe_tpram_auto_clk_enable_mask : 1;
        unsigned int maa_spram_auto_clk_enable_mask : 1;
        unsigned int maa_tpram_auto_clk_enable_mask : 1;
        unsigned int reserved : 23;
    } reg;
} SOC_PCTRL_PERI_CTRL104_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL104_ipf_spram_auto_clk_enable_mask_START (0)
#define SOC_PCTRL_PERI_CTRL104_ipf_spram_auto_clk_enable_mask_END (0)
#define SOC_PCTRL_PERI_CTRL104_socp_spram_auto_clk_enable_mask_START (1)
#define SOC_PCTRL_PERI_CTRL104_socp_spram_auto_clk_enable_mask_END (1)
#define SOC_PCTRL_PERI_CTRL104_ipf_tpram_auto_clk_enable_mask_START (2)
#define SOC_PCTRL_PERI_CTRL104_ipf_tpram_auto_clk_enable_mask_END (2)
#define SOC_PCTRL_PERI_CTRL104_socp_tpram_auto_clk_enable_mask_START (3)
#define SOC_PCTRL_PERI_CTRL104_socp_tpram_auto_clk_enable_mask_END (3)
#define SOC_PCTRL_PERI_CTRL104_aximem_spram_auto_clk_enable_mask_START (4)
#define SOC_PCTRL_PERI_CTRL104_aximem_spram_auto_clk_enable_mask_END (4)
#define SOC_PCTRL_PERI_CTRL104_spe_spram_auto_clk_enable_mask_START (5)
#define SOC_PCTRL_PERI_CTRL104_spe_spram_auto_clk_enable_mask_END (5)
#define SOC_PCTRL_PERI_CTRL104_spe_tpram_auto_clk_enable_mask_START (6)
#define SOC_PCTRL_PERI_CTRL104_spe_tpram_auto_clk_enable_mask_END (6)
#define SOC_PCTRL_PERI_CTRL104_maa_spram_auto_clk_enable_mask_START (7)
#define SOC_PCTRL_PERI_CTRL104_maa_spram_auto_clk_enable_mask_END (7)
#define SOC_PCTRL_PERI_CTRL104_maa_tpram_auto_clk_enable_mask_START (8)
#define SOC_PCTRL_PERI_CTRL104_maa_tpram_auto_clk_enable_mask_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 3;
        unsigned int sphcsram_rtsel : 3;
        unsigned int sphcsram_wtsel : 2;
        unsigned int spsram_test : 3;
        unsigned int sphdsram_rtsel : 3;
        unsigned int sphdsram_wtsel : 2;
        unsigned int reserved_1 : 3;
        unsigned int reserved_2 : 13;
    } reg;
} SOC_PCTRL_PERI_CTRL12_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL12_sphcsram_rtsel_START (3)
#define SOC_PCTRL_PERI_CTRL12_sphcsram_rtsel_END (5)
#define SOC_PCTRL_PERI_CTRL12_sphcsram_wtsel_START (6)
#define SOC_PCTRL_PERI_CTRL12_sphcsram_wtsel_END (7)
#define SOC_PCTRL_PERI_CTRL12_spsram_test_START (8)
#define SOC_PCTRL_PERI_CTRL12_spsram_test_END (10)
#define SOC_PCTRL_PERI_CTRL12_sphdsram_rtsel_START (11)
#define SOC_PCTRL_PERI_CTRL12_sphdsram_rtsel_END (13)
#define SOC_PCTRL_PERI_CTRL12_sphdsram_wtsel_START (14)
#define SOC_PCTRL_PERI_CTRL12_sphdsram_wtsel_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 4;
        unsigned int tprf_tselr : 2;
        unsigned int tprf_tselw : 2;
        unsigned int tprf_test : 3;
        unsigned int reserved_1 : 21;
    } reg;
} SOC_PCTRL_PERI_CTRL13_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL13_tprf_tselr_START (4)
#define SOC_PCTRL_PERI_CTRL13_tprf_tselr_END (5)
#define SOC_PCTRL_PERI_CTRL13_tprf_tselw_START (6)
#define SOC_PCTRL_PERI_CTRL13_tprf_tselw_END (7)
#define SOC_PCTRL_PERI_CTRL13_tprf_test_START (8)
#define SOC_PCTRL_PERI_CTRL13_tprf_test_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rom_dt : 2;
        unsigned int rom_skp : 2;
        unsigned int rom_ckle : 1;
        unsigned int rom_ckhe : 1;
        unsigned int reserved_0 : 11;
        unsigned int bisr_rom_ctrl_sd : 1;
        unsigned int reserved_1 : 14;
    } reg;
} SOC_PCTRL_PERI_CTRL14_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL14_rom_dt_START (0)
#define SOC_PCTRL_PERI_CTRL14_rom_dt_END (1)
#define SOC_PCTRL_PERI_CTRL14_rom_skp_START (2)
#define SOC_PCTRL_PERI_CTRL14_rom_skp_END (3)
#define SOC_PCTRL_PERI_CTRL14_rom_ckle_START (4)
#define SOC_PCTRL_PERI_CTRL14_rom_ckle_END (4)
#define SOC_PCTRL_PERI_CTRL14_rom_ckhe_START (5)
#define SOC_PCTRL_PERI_CTRL14_rom_ckhe_END (5)
#define SOC_PCTRL_PERI_CTRL14_bisr_rom_ctrl_sd_START (17)
#define SOC_PCTRL_PERI_CTRL14_bisr_rom_ctrl_sd_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int press_push_en : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_PCTRL_PERI_CTRL15_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL15_press_push_en_START (0)
#define SOC_PCTRL_PERI_CTRL15_press_push_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int spi4_cs_sel : 4;
        unsigned int reserved_0 : 4;
        unsigned int reserved_1 : 24;
    } reg;
} SOC_PCTRL_PERI_CTRL16_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL16_spi4_cs_sel_START (0)
#define SOC_PCTRL_PERI_CTRL16_spi4_cs_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ccpu_pd_bypass : 1;
        unsigned int mdm5g_ccpu5g_pd_bypass : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_PCTRL_PERI_CTRL17_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL17_ccpu_pd_bypass_START (0)
#define SOC_PCTRL_PERI_CTRL17_ccpu_pd_bypass_END (0)
#define SOC_PCTRL_PERI_CTRL17_mdm5g_ccpu5g_pd_bypass_START (1)
#define SOC_PCTRL_PERI_CTRL17_mdm5g_ccpu5g_pd_bypass_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pwr_dwn_req_0 : 1;
        unsigned int pwr_dwn_req_1 : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_PCTRL_PERI_CTRL18_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL18_pwr_dwn_req_0_START (0)
#define SOC_PCTRL_PERI_CTRL18_pwr_dwn_req_0_END (0)
#define SOC_PCTRL_PERI_CTRL18_pwr_dwn_req_1_START (1)
#define SOC_PCTRL_PERI_CTRL18_pwr_dwn_req_1_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 4;
        unsigned int g3d_pw_sel : 1;
        unsigned int g3d_slp_sel : 1;
        unsigned int reserved_1 : 3;
        unsigned int gpu_striping_granule : 4;
        unsigned int reserved_2 : 19;
    } reg;
} SOC_PCTRL_PERI_CTRL19_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL19_g3d_pw_sel_START (4)
#define SOC_PCTRL_PERI_CTRL19_g3d_pw_sel_END (4)
#define SOC_PCTRL_PERI_CTRL19_g3d_slp_sel_START (5)
#define SOC_PCTRL_PERI_CTRL19_g3d_slp_sel_END (5)
#define SOC_PCTRL_PERI_CTRL19_gpu_striping_granule_START (9)
#define SOC_PCTRL_PERI_CTRL19_gpu_striping_granule_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int uart0_1wire_en : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_PCTRL_PERI_CTRL20_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL20_uart0_1wire_en_START (0)
#define SOC_PCTRL_PERI_CTRL20_uart0_1wire_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int g3d_mem_sd : 25;
        unsigned int reserved : 7;
    } reg;
} SOC_PCTRL_PERI_CTRL21_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL21_g3d_mem_sd_START (0)
#define SOC_PCTRL_PERI_CTRL21_g3d_mem_sd_END (24)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmss_bus_buff_en : 1;
        unsigned int dmc_bus_buff_en : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_PCTRL_PERI_CTRL22_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL22_dmss_bus_buff_en_START (0)
#define SOC_PCTRL_PERI_CTRL22_dmss_bus_buff_en_END (0)
#define SOC_PCTRL_PERI_CTRL22_dmc_bus_buff_en_START (1)
#define SOC_PCTRL_PERI_CTRL22_dmc_bus_buff_en_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int dmac_ckgt_dis : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 27;
    } reg;
} SOC_PCTRL_PERI_CTRL23_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL23_dmac_ckgt_dis_START (2)
#define SOC_PCTRL_PERI_CTRL23_dmac_ckgt_dis_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 3;
        unsigned int djtag_mst_pstrb : 4;
        unsigned int reserved_1 : 3;
        unsigned int sd3_resp_ctrl : 1;
        unsigned int reserved_2 : 1;
        unsigned int lpmcu_resp_ctrl : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int isp_axi_xdcdr_sel : 1;
        unsigned int reserved_5 : 3;
        unsigned int reserved_6 : 1;
        unsigned int codecssi_mst_cnt : 3;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int clk_usb3phy_2mux1_sel : 1;
        unsigned int reserved_9 : 2;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 3;
    } reg;
} SOC_PCTRL_PERI_CTRL24_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL24_djtag_mst_pstrb_START (3)
#define SOC_PCTRL_PERI_CTRL24_djtag_mst_pstrb_END (6)
#define SOC_PCTRL_PERI_CTRL24_sd3_resp_ctrl_START (10)
#define SOC_PCTRL_PERI_CTRL24_sd3_resp_ctrl_END (10)
#define SOC_PCTRL_PERI_CTRL24_lpmcu_resp_ctrl_START (12)
#define SOC_PCTRL_PERI_CTRL24_lpmcu_resp_ctrl_END (12)
#define SOC_PCTRL_PERI_CTRL24_isp_axi_xdcdr_sel_START (15)
#define SOC_PCTRL_PERI_CTRL24_isp_axi_xdcdr_sel_END (15)
#define SOC_PCTRL_PERI_CTRL24_codecssi_mst_cnt_START (20)
#define SOC_PCTRL_PERI_CTRL24_codecssi_mst_cnt_END (22)
#define SOC_PCTRL_PERI_CTRL24_clk_usb3phy_2mux1_sel_START (25)
#define SOC_PCTRL_PERI_CTRL24_clk_usb3phy_2mux1_sel_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 8;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int g3d_coherency_feature : 2;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int sc_g3d_dw_axi_m1_cg_en : 1;
        unsigned int g3d_scramble_sw : 1;
        unsigned int reserved_8 : 1;
        unsigned int g3d_snoop_bypass : 1;
        unsigned int reserved_9 : 6;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int sc_modem_ipc_auto_clk_gate_en : 1;
        unsigned int sc_modem_ipc_soft_gate_clk_en : 1;
        unsigned int reserved_12 : 1;
    } reg;
} SOC_PCTRL_PERI_CTRL25_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL25_g3d_coherency_feature_START (12)
#define SOC_PCTRL_PERI_CTRL25_g3d_coherency_feature_END (13)
#define SOC_PCTRL_PERI_CTRL25_sc_g3d_dw_axi_m1_cg_en_START (17)
#define SOC_PCTRL_PERI_CTRL25_sc_g3d_dw_axi_m1_cg_en_END (17)
#define SOC_PCTRL_PERI_CTRL25_g3d_scramble_sw_START (18)
#define SOC_PCTRL_PERI_CTRL25_g3d_scramble_sw_END (18)
#define SOC_PCTRL_PERI_CTRL25_g3d_snoop_bypass_START (20)
#define SOC_PCTRL_PERI_CTRL25_g3d_snoop_bypass_END (20)
#define SOC_PCTRL_PERI_CTRL25_sc_modem_ipc_auto_clk_gate_en_START (29)
#define SOC_PCTRL_PERI_CTRL25_sc_modem_ipc_auto_clk_gate_en_END (29)
#define SOC_PCTRL_PERI_CTRL25_sc_modem_ipc_soft_gate_clk_en_START (30)
#define SOC_PCTRL_PERI_CTRL25_sc_modem_ipc_soft_gate_clk_en_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gps_sync_sel : 4;
        unsigned int reserved_0 : 5;
        unsigned int reserved_1 : 5;
        unsigned int isp_dw_axi_gatedclock_en : 1;
        unsigned int ivp32_dw_axi_gatedclock_en : 1;
        unsigned int ivp32_dw_axi_gatedclock_en_ivp1 : 1;
        unsigned int reserved_2 : 15;
    } reg;
} SOC_PCTRL_PERI_CTRL26_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL26_gps_sync_sel_START (0)
#define SOC_PCTRL_PERI_CTRL26_gps_sync_sel_END (3)
#define SOC_PCTRL_PERI_CTRL26_isp_dw_axi_gatedclock_en_START (14)
#define SOC_PCTRL_PERI_CTRL26_isp_dw_axi_gatedclock_en_END (14)
#define SOC_PCTRL_PERI_CTRL26_ivp32_dw_axi_gatedclock_en_START (15)
#define SOC_PCTRL_PERI_CTRL26_ivp32_dw_axi_gatedclock_en_END (15)
#define SOC_PCTRL_PERI_CTRL26_ivp32_dw_axi_gatedclock_en_ivp1_START (16)
#define SOC_PCTRL_PERI_CTRL26_ivp32_dw_axi_gatedclock_en_ivp1_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int isp_sys_ctrl_0 : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL27_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL27_isp_sys_ctrl_0_START (0)
#define SOC_PCTRL_PERI_CTRL27_isp_sys_ctrl_0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int isp_sys_ctrl_1 : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL28_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL28_isp_sys_ctrl_1_START (0)
#define SOC_PCTRL_PERI_CTRL28_isp_sys_ctrl_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_peri_ctrl0 : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL29_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL29_npu_peri_ctrl0_START (0)
#define SOC_PCTRL_PERI_CTRL29_npu_peri_ctrl0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_peri_ctrl1 : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL30_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL30_npu_peri_ctrl1_START (0)
#define SOC_PCTRL_PERI_CTRL30_npu_peri_ctrl1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gpu_spram_mem_ctrl : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL31_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL31_gpu_spram_mem_ctrl_START (0)
#define SOC_PCTRL_PERI_CTRL31_gpu_spram_mem_ctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gpu_tpram_mem_ctrl : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL32_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL32_gpu_tpram_mem_ctrl_START (0)
#define SOC_PCTRL_PERI_CTRL32_gpu_tpram_mem_ctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_tpram_mem_ctrl : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL33_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL33_pcr_tpram_mem_ctrl_START (0)
#define SOC_PCTRL_PERI_CTRL33_pcr_tpram_mem_ctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int int_dmabus_error_probe_observer_mainfault0 : 1;
        unsigned int int_sysbus_error_probe_observer_mainfault0 : 1;
        unsigned int int_aobus_error_probe_observer_mainfault0 : 1;
        unsigned int int_modemcfg_error_probe_observer_mainfault0 : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int int_hsdt1bus_error_probe_observer_mainfault0 : 1;
        unsigned int int_cfgbus_error_probe_observer_mainfault0 : 1;
        unsigned int int_hsdt0bus_error_probe_observer_mainfault0 : 1;
        unsigned int int_npubus_error_probe_observer_mainfault0 : 1;
        unsigned int cfgbus_trace_packet_probe_tracealarm : 1;
        unsigned int intr_aobus_aon_trace_packet_probe_tracealarm : 1;
        unsigned int sysbus_acpu_packet_probe_tracealarm : 1;
        unsigned int int_modem_transaction_probe_mainstatalarm : 1;
        unsigned int reserved_2 : 7;
        unsigned int intr_modem5g_cfg_error_probe_mainstatalarm : 1;
        unsigned int intr_fcmbus_error_probe_mainstatalarm : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int intr_dpctrl_transaction_probe_mainstatalarm : 1;
        unsigned int reserved_5 : 1;
        unsigned int int_sysbusddrc_packet_probe_mainstatalarm : 1;
        unsigned int intr_asp_transaction_probe_mainstatalarm : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
    } reg;
} SOC_PCTRL_PERI_STAT0_UNION;
#endif
#define SOC_PCTRL_PERI_STAT0_int_dmabus_error_probe_observer_mainfault0_START (0)
#define SOC_PCTRL_PERI_STAT0_int_dmabus_error_probe_observer_mainfault0_END (0)
#define SOC_PCTRL_PERI_STAT0_int_sysbus_error_probe_observer_mainfault0_START (1)
#define SOC_PCTRL_PERI_STAT0_int_sysbus_error_probe_observer_mainfault0_END (1)
#define SOC_PCTRL_PERI_STAT0_int_aobus_error_probe_observer_mainfault0_START (2)
#define SOC_PCTRL_PERI_STAT0_int_aobus_error_probe_observer_mainfault0_END (2)
#define SOC_PCTRL_PERI_STAT0_int_modemcfg_error_probe_observer_mainfault0_START (3)
#define SOC_PCTRL_PERI_STAT0_int_modemcfg_error_probe_observer_mainfault0_END (3)
#define SOC_PCTRL_PERI_STAT0_int_hsdt1bus_error_probe_observer_mainfault0_START (6)
#define SOC_PCTRL_PERI_STAT0_int_hsdt1bus_error_probe_observer_mainfault0_END (6)
#define SOC_PCTRL_PERI_STAT0_int_cfgbus_error_probe_observer_mainfault0_START (7)
#define SOC_PCTRL_PERI_STAT0_int_cfgbus_error_probe_observer_mainfault0_END (7)
#define SOC_PCTRL_PERI_STAT0_int_hsdt0bus_error_probe_observer_mainfault0_START (8)
#define SOC_PCTRL_PERI_STAT0_int_hsdt0bus_error_probe_observer_mainfault0_END (8)
#define SOC_PCTRL_PERI_STAT0_int_npubus_error_probe_observer_mainfault0_START (9)
#define SOC_PCTRL_PERI_STAT0_int_npubus_error_probe_observer_mainfault0_END (9)
#define SOC_PCTRL_PERI_STAT0_cfgbus_trace_packet_probe_tracealarm_START (10)
#define SOC_PCTRL_PERI_STAT0_cfgbus_trace_packet_probe_tracealarm_END (10)
#define SOC_PCTRL_PERI_STAT0_intr_aobus_aon_trace_packet_probe_tracealarm_START (11)
#define SOC_PCTRL_PERI_STAT0_intr_aobus_aon_trace_packet_probe_tracealarm_END (11)
#define SOC_PCTRL_PERI_STAT0_sysbus_acpu_packet_probe_tracealarm_START (12)
#define SOC_PCTRL_PERI_STAT0_sysbus_acpu_packet_probe_tracealarm_END (12)
#define SOC_PCTRL_PERI_STAT0_int_modem_transaction_probe_mainstatalarm_START (13)
#define SOC_PCTRL_PERI_STAT0_int_modem_transaction_probe_mainstatalarm_END (13)
#define SOC_PCTRL_PERI_STAT0_intr_modem5g_cfg_error_probe_mainstatalarm_START (21)
#define SOC_PCTRL_PERI_STAT0_intr_modem5g_cfg_error_probe_mainstatalarm_END (21)
#define SOC_PCTRL_PERI_STAT0_intr_fcmbus_error_probe_mainstatalarm_START (22)
#define SOC_PCTRL_PERI_STAT0_intr_fcmbus_error_probe_mainstatalarm_END (22)
#define SOC_PCTRL_PERI_STAT0_intr_dpctrl_transaction_probe_mainstatalarm_START (25)
#define SOC_PCTRL_PERI_STAT0_intr_dpctrl_transaction_probe_mainstatalarm_END (25)
#define SOC_PCTRL_PERI_STAT0_int_sysbusddrc_packet_probe_mainstatalarm_START (27)
#define SOC_PCTRL_PERI_STAT0_int_sysbusddrc_packet_probe_mainstatalarm_END (27)
#define SOC_PCTRL_PERI_STAT0_intr_asp_transaction_probe_mainstatalarm_START (28)
#define SOC_PCTRL_PERI_STAT0_intr_asp_transaction_probe_mainstatalarm_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_peri_stat_0 : 32;
    } reg;
} SOC_PCTRL_PERI_STAT2_UNION;
#endif
#define SOC_PCTRL_PERI_STAT2_npu_peri_stat_0_START (0)
#define SOC_PCTRL_PERI_STAT2_npu_peri_stat_0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_peri_stat_1 : 32;
    } reg;
} SOC_PCTRL_PERI_STAT3_UNION;
#endif
#define SOC_PCTRL_PERI_STAT3_npu_peri_stat_1_START (0)
#define SOC_PCTRL_PERI_STAT3_npu_peri_stat_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int spe_idle : 1;
        unsigned int ipf_idle : 1;
        unsigned int vdec_idle : 1;
        unsigned int reserved_0 : 1;
        unsigned int venc_idle : 1;
        unsigned int maa_idle : 1;
        unsigned int socp_idle : 1;
        unsigned int reserved_1 : 1;
        unsigned int ivp32_dwaxi_dlock_mst : 2;
        unsigned int ivp32_dwaxi_dlock_slv : 3;
        unsigned int ivp32_dwaxi_dlock_id : 6;
        unsigned int ivp32_dwaxi_dlock_wr : 1;
        unsigned int ivp32_dwaxi_dlock_irq : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int mdm_ipc_s_clk_state : 3;
        unsigned int mdm_ipc_ns_clk_state : 3;
        unsigned int reserved_4 : 3;
    } reg;
} SOC_PCTRL_PERI_STAT4_UNION;
#endif
#define SOC_PCTRL_PERI_STAT4_spe_idle_START (0)
#define SOC_PCTRL_PERI_STAT4_spe_idle_END (0)
#define SOC_PCTRL_PERI_STAT4_ipf_idle_START (1)
#define SOC_PCTRL_PERI_STAT4_ipf_idle_END (1)
#define SOC_PCTRL_PERI_STAT4_vdec_idle_START (2)
#define SOC_PCTRL_PERI_STAT4_vdec_idle_END (2)
#define SOC_PCTRL_PERI_STAT4_venc_idle_START (4)
#define SOC_PCTRL_PERI_STAT4_venc_idle_END (4)
#define SOC_PCTRL_PERI_STAT4_maa_idle_START (5)
#define SOC_PCTRL_PERI_STAT4_maa_idle_END (5)
#define SOC_PCTRL_PERI_STAT4_socp_idle_START (6)
#define SOC_PCTRL_PERI_STAT4_socp_idle_END (6)
#define SOC_PCTRL_PERI_STAT4_ivp32_dwaxi_dlock_mst_START (8)
#define SOC_PCTRL_PERI_STAT4_ivp32_dwaxi_dlock_mst_END (9)
#define SOC_PCTRL_PERI_STAT4_ivp32_dwaxi_dlock_slv_START (10)
#define SOC_PCTRL_PERI_STAT4_ivp32_dwaxi_dlock_slv_END (12)
#define SOC_PCTRL_PERI_STAT4_ivp32_dwaxi_dlock_id_START (13)
#define SOC_PCTRL_PERI_STAT4_ivp32_dwaxi_dlock_id_END (18)
#define SOC_PCTRL_PERI_STAT4_ivp32_dwaxi_dlock_wr_START (19)
#define SOC_PCTRL_PERI_STAT4_ivp32_dwaxi_dlock_wr_END (19)
#define SOC_PCTRL_PERI_STAT4_ivp32_dwaxi_dlock_irq_START (20)
#define SOC_PCTRL_PERI_STAT4_ivp32_dwaxi_dlock_irq_END (20)
#define SOC_PCTRL_PERI_STAT4_mdm_ipc_s_clk_state_START (23)
#define SOC_PCTRL_PERI_STAT4_mdm_ipc_s_clk_state_END (25)
#define SOC_PCTRL_PERI_STAT4_mdm_ipc_ns_clk_state_START (26)
#define SOC_PCTRL_PERI_STAT4_mdm_ipc_ns_clk_state_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_mdm2ap_status : 32;
    } reg;
} SOC_PCTRL_PERI_STAT5_UNION;
#endif
#define SOC_PCTRL_PERI_STAT5_sc_mdm2ap_status_START (0)
#define SOC_PCTRL_PERI_STAT5_sc_mdm2ap_status_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tp_s0_grp : 22;
        unsigned int tp_s14_grp : 10;
    } reg;
} SOC_PCTRL_PERI_STAT6_UNION;
#endif
#define SOC_PCTRL_PERI_STAT6_tp_s0_grp_START (0)
#define SOC_PCTRL_PERI_STAT6_tp_s0_grp_END (21)
#define SOC_PCTRL_PERI_STAT6_tp_s14_grp_START (22)
#define SOC_PCTRL_PERI_STAT6_tp_s14_grp_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tp_s1_grp : 22;
        unsigned int tp_s14_grp_1 : 10;
    } reg;
} SOC_PCTRL_PERI_STAT7_UNION;
#endif
#define SOC_PCTRL_PERI_STAT7_tp_s1_grp_START (0)
#define SOC_PCTRL_PERI_STAT7_tp_s1_grp_END (21)
#define SOC_PCTRL_PERI_STAT7_tp_s14_grp_1_START (22)
#define SOC_PCTRL_PERI_STAT7_tp_s14_grp_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tp_s2_grp : 22;
        unsigned int tp_s14_grp_2 : 2;
        unsigned int reserved : 8;
    } reg;
} SOC_PCTRL_PERI_STAT8_UNION;
#endif
#define SOC_PCTRL_PERI_STAT8_tp_s2_grp_START (0)
#define SOC_PCTRL_PERI_STAT8_tp_s2_grp_END (21)
#define SOC_PCTRL_PERI_STAT8_tp_s14_grp_2_START (22)
#define SOC_PCTRL_PERI_STAT8_tp_s14_grp_2_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tp_s3_grp : 22;
        unsigned int tp_s15_grp : 10;
    } reg;
} SOC_PCTRL_PERI_STAT9_UNION;
#endif
#define SOC_PCTRL_PERI_STAT9_tp_s3_grp_START (0)
#define SOC_PCTRL_PERI_STAT9_tp_s3_grp_END (21)
#define SOC_PCTRL_PERI_STAT9_tp_s15_grp_START (22)
#define SOC_PCTRL_PERI_STAT9_tp_s15_grp_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tp_s4_grp : 22;
        unsigned int tp_s15_grp_1 : 10;
    } reg;
} SOC_PCTRL_PERI_STAT10_UNION;
#endif
#define SOC_PCTRL_PERI_STAT10_tp_s4_grp_START (0)
#define SOC_PCTRL_PERI_STAT10_tp_s4_grp_END (21)
#define SOC_PCTRL_PERI_STAT10_tp_s15_grp_1_START (22)
#define SOC_PCTRL_PERI_STAT10_tp_s15_grp_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tp_s5_grp : 22;
        unsigned int tp_s15_grp_2 : 2;
        unsigned int reserved : 8;
    } reg;
} SOC_PCTRL_PERI_STAT11_UNION;
#endif
#define SOC_PCTRL_PERI_STAT11_tp_s5_grp_START (0)
#define SOC_PCTRL_PERI_STAT11_tp_s5_grp_END (21)
#define SOC_PCTRL_PERI_STAT11_tp_s15_grp_2_START (22)
#define SOC_PCTRL_PERI_STAT11_tp_s15_grp_2_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tp_s6_grp : 22;
        unsigned int reserved : 10;
    } reg;
} SOC_PCTRL_PERI_STAT12_UNION;
#endif
#define SOC_PCTRL_PERI_STAT12_tp_s6_grp_START (0)
#define SOC_PCTRL_PERI_STAT12_tp_s6_grp_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tp_s7_grp : 22;
        unsigned int reserved : 10;
    } reg;
} SOC_PCTRL_PERI_STAT13_UNION;
#endif
#define SOC_PCTRL_PERI_STAT13_tp_s7_grp_START (0)
#define SOC_PCTRL_PERI_STAT13_tp_s7_grp_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tp_s8_grp : 22;
        unsigned int reserved : 10;
    } reg;
} SOC_PCTRL_PERI_STAT14_UNION;
#endif
#define SOC_PCTRL_PERI_STAT14_tp_s8_grp_START (0)
#define SOC_PCTRL_PERI_STAT14_tp_s8_grp_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tp_s9_grp : 22;
        unsigned int reserved : 10;
    } reg;
} SOC_PCTRL_PERI_STAT15_UNION;
#endif
#define SOC_PCTRL_PERI_STAT15_tp_s9_grp_START (0)
#define SOC_PCTRL_PERI_STAT15_tp_s9_grp_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tp_s10_grp : 22;
        unsigned int reserved : 10;
    } reg;
} SOC_PCTRL_PERI_STAT16_UNION;
#endif
#define SOC_PCTRL_PERI_STAT16_tp_s10_grp_START (0)
#define SOC_PCTRL_PERI_STAT16_tp_s10_grp_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tp_s11_grp : 22;
        unsigned int reserved : 10;
    } reg;
} SOC_PCTRL_PERI_STAT17_UNION;
#endif
#define SOC_PCTRL_PERI_STAT17_tp_s11_grp_START (0)
#define SOC_PCTRL_PERI_STAT17_tp_s11_grp_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tp_s12_grp : 22;
        unsigned int reserved : 10;
    } reg;
} SOC_PCTRL_PERI_STAT18_UNION;
#endif
#define SOC_PCTRL_PERI_STAT18_tp_s12_grp_START (0)
#define SOC_PCTRL_PERI_STAT18_tp_s12_grp_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tp_s13_grp : 22;
        unsigned int reserved : 10;
    } reg;
} SOC_PCTRL_PERI_STAT19_UNION;
#endif
#define SOC_PCTRL_PERI_STAT19_tp_s13_grp_START (0)
#define SOC_PCTRL_PERI_STAT19_tp_s13_grp_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int usb2_host_ctrl0 : 32;
    } reg;
} SOC_PCTRL_USB2_HOST_CTRL0_UNION;
#endif
#define SOC_PCTRL_USB2_HOST_CTRL0_usb2_host_ctrl0_START (0)
#define SOC_PCTRL_USB2_HOST_CTRL0_usb2_host_ctrl0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int usb2_host_ctrl1 : 32;
    } reg;
} SOC_PCTRL_USB2_HOST_CTRL1_UNION;
#endif
#define SOC_PCTRL_USB2_HOST_CTRL1_usb2_host_ctrl1_START (0)
#define SOC_PCTRL_USB2_HOST_CTRL1_usb2_host_ctrl1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int usb2_host_ctrl2 : 32;
    } reg;
} SOC_PCTRL_USB2_HOST_CTRL2_UNION;
#endif
#define SOC_PCTRL_USB2_HOST_CTRL2_usb2_host_ctrl2_START (0)
#define SOC_PCTRL_USB2_HOST_CTRL2_usb2_host_ctrl2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int axi_isp_dlock : 11;
        unsigned int reserved : 21;
    } reg;
} SOC_PCTRL_PERI_STAT29_UNION;
#endif
#define SOC_PCTRL_PERI_STAT29_axi_isp_dlock_START (0)
#define SOC_PCTRL_PERI_STAT29_axi_isp_dlock_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int g3d_stat_reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT30_UNION;
#endif
#define SOC_PCTRL_PERI_STAT30_g3d_stat_reserved_START (0)
#define SOC_PCTRL_PERI_STAT30_g3d_stat_reserved_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pasensor_cfg_ready : 1;
        unsigned int pasensor_valid : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_PCTRL_PERI_STAT31_UNION;
#endif
#define SOC_PCTRL_PERI_STAT31_pasensor_cfg_ready_START (0)
#define SOC_PCTRL_PERI_STAT31_pasensor_cfg_ready_END (0)
#define SOC_PCTRL_PERI_STAT31_pasensor_valid_START (1)
#define SOC_PCTRL_PERI_STAT31_pasensor_valid_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pasensor_nbti_a_out : 9;
        unsigned int reserved_0 : 7;
        unsigned int pasensor_nbti_o_out : 9;
        unsigned int reserved_1 : 7;
    } reg;
} SOC_PCTRL_PERI_STAT32_UNION;
#endif
#define SOC_PCTRL_PERI_STAT32_pasensor_nbti_a_out_START (0)
#define SOC_PCTRL_PERI_STAT32_pasensor_nbti_a_out_END (8)
#define SOC_PCTRL_PERI_STAT32_pasensor_nbti_o_out_START (16)
#define SOC_PCTRL_PERI_STAT32_pasensor_nbti_o_out_END (24)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pasensor_hci_a_out : 9;
        unsigned int reserved_0 : 7;
        unsigned int pasensor_hci_o_out : 9;
        unsigned int reserved_1 : 7;
    } reg;
} SOC_PCTRL_PERI_STAT33_UNION;
#endif
#define SOC_PCTRL_PERI_STAT33_pasensor_hci_a_out_START (0)
#define SOC_PCTRL_PERI_STAT33_pasensor_hci_a_out_END (8)
#define SOC_PCTRL_PERI_STAT33_pasensor_hci_o_out_START (16)
#define SOC_PCTRL_PERI_STAT33_pasensor_hci_o_out_END (24)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT34_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tidm_core_pc_tidm_org : 10;
        unsigned int reserved_0 : 6;
        unsigned int tidm_core_pc_stdcell_org : 10;
        unsigned int reserved_1 : 5;
        unsigned int tidm_core_hpm_pc_valid : 1;
    } reg;
} SOC_PCTRL_PERI_STAT35_UNION;
#endif
#define SOC_PCTRL_PERI_STAT35_tidm_core_pc_tidm_org_START (0)
#define SOC_PCTRL_PERI_STAT35_tidm_core_pc_tidm_org_END (9)
#define SOC_PCTRL_PERI_STAT35_tidm_core_pc_stdcell_org_START (16)
#define SOC_PCTRL_PERI_STAT35_tidm_core_pc_stdcell_org_END (25)
#define SOC_PCTRL_PERI_STAT35_tidm_core_hpm_pc_valid_START (31)
#define SOC_PCTRL_PERI_STAT35_tidm_core_hpm_pc_valid_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT36_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT37_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_dbg_info : 32;
    } reg;
} SOC_PCTRL_PERI_STAT38_UNION;
#endif
#define SOC_PCTRL_PERI_STAT38_bisr_dbg_info_START (0)
#define SOC_PCTRL_PERI_STAT38_bisr_dbg_info_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_dbg_info_1 : 32;
    } reg;
} SOC_PCTRL_PERI_STAT39_UNION;
#endif
#define SOC_PCTRL_PERI_STAT39_bisr_dbg_info_1_START (0)
#define SOC_PCTRL_PERI_STAT39_bisr_dbg_info_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT40_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pa_valid_g3d : 1;
        unsigned int reserved_0 : 15;
        unsigned int pa_cfg_ready_g3d : 1;
        unsigned int reserved_1 : 15;
    } reg;
} SOC_PCTRL_PERI_STAT41_UNION;
#endif
#define SOC_PCTRL_PERI_STAT41_pa_valid_g3d_START (0)
#define SOC_PCTRL_PERI_STAT41_pa_valid_g3d_END (0)
#define SOC_PCTRL_PERI_STAT41_pa_cfg_ready_g3d_START (16)
#define SOC_PCTRL_PERI_STAT41_pa_cfg_ready_g3d_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pctrl_mem_sta_sd_modem_5g_lowbit : 32;
    } reg;
} SOC_PCTRL_PERI_STAT42_UNION;
#endif
#define SOC_PCTRL_PERI_STAT42_pctrl_mem_sta_sd_modem_5g_lowbit_START (0)
#define SOC_PCTRL_PERI_STAT42_pctrl_mem_sta_sd_modem_5g_lowbit_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pctrl_mem_sta_sd_modem_5g_medbit : 32;
    } reg;
} SOC_PCTRL_PERI_STAT43_UNION;
#endif
#define SOC_PCTRL_PERI_STAT43_pctrl_mem_sta_sd_modem_5g_medbit_START (0)
#define SOC_PCTRL_PERI_STAT43_pctrl_mem_sta_sd_modem_5g_medbit_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pctrl_mem_sta_sd_modem_5g_highbit : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_PCTRL_PERI_STAT44_UNION;
#endif
#define SOC_PCTRL_PERI_STAT44_pctrl_mem_sta_sd_modem_5g_highbit_START (0)
#define SOC_PCTRL_PERI_STAT44_pctrl_mem_sta_sd_modem_5g_highbit_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int lpm_data_big : 5;
        unsigned int reserved_0 : 3;
        unsigned int result_valid_big : 1;
        unsigned int lpm_status_big : 2;
        unsigned int reserved_1 : 21;
    } reg;
} SOC_PCTRL_PERI_STAT45_UNION;
#endif
#define SOC_PCTRL_PERI_STAT45_lpm_data_big_START (0)
#define SOC_PCTRL_PERI_STAT45_lpm_data_big_END (4)
#define SOC_PCTRL_PERI_STAT45_result_valid_big_START (8)
#define SOC_PCTRL_PERI_STAT45_result_valid_big_END (8)
#define SOC_PCTRL_PERI_STAT45_lpm_status_big_START (9)
#define SOC_PCTRL_PERI_STAT45_lpm_status_big_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int lpm_data_middle : 5;
        unsigned int reserved_0 : 3;
        unsigned int result_valid_middle : 1;
        unsigned int lpm_status_middle : 2;
        unsigned int reserved_1 : 21;
    } reg;
} SOC_PCTRL_PERI_STAT46_UNION;
#endif
#define SOC_PCTRL_PERI_STAT46_lpm_data_middle_START (0)
#define SOC_PCTRL_PERI_STAT46_lpm_data_middle_END (4)
#define SOC_PCTRL_PERI_STAT46_result_valid_middle_START (8)
#define SOC_PCTRL_PERI_STAT46_result_valid_middle_END (8)
#define SOC_PCTRL_PERI_STAT46_lpm_status_middle_START (9)
#define SOC_PCTRL_PERI_STAT46_lpm_status_middle_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int lpm_data_little : 5;
        unsigned int reserved_0 : 3;
        unsigned int result_valid_little : 1;
        unsigned int lpm_status_little : 2;
        unsigned int reserved_1 : 21;
    } reg;
} SOC_PCTRL_PERI_STAT47_UNION;
#endif
#define SOC_PCTRL_PERI_STAT47_lpm_data_little_START (0)
#define SOC_PCTRL_PERI_STAT47_lpm_data_little_END (4)
#define SOC_PCTRL_PERI_STAT47_result_valid_little_START (8)
#define SOC_PCTRL_PERI_STAT47_result_valid_little_END (8)
#define SOC_PCTRL_PERI_STAT47_lpm_status_little_START (9)
#define SOC_PCTRL_PERI_STAT47_lpm_status_little_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int pctrl_retmem_sta_sd_ipp : 1;
        unsigned int pctrl_retmem_sta_sd_dmss_sc_buffer0 : 1;
        unsigned int pctrl_retmem_sta_sd_dmss_sc_buffer1 : 1;
        unsigned int pctrl_retmem_sta_sd_arpp0 : 1;
        unsigned int pctrl_retmem_sta_sd_arpp1 : 1;
        unsigned int pctrl_retmem_sta_sd_arpp2 : 1;
        unsigned int pctrl_retmem_sta_sd_arpp3 : 1;
        unsigned int pctrl_retmem_sta_sd_arpp4 : 1;
        unsigned int pctrl_retmem_sta_sd_dmc_pack : 4;
        unsigned int pctrl_retmem_sta_ds_dmc_pack : 4;
        unsigned int pctrl_mem_sta_sd_media1_sub : 1;
        unsigned int pctrl_mem_sta_sd_isp_noc : 1;
        unsigned int reserved_1 : 1;
        unsigned int pctrl_mem_sta_sd_isp_fe : 1;
        unsigned int pctrl_mem_sta_sd_isp_bpecap : 1;
        unsigned int pctrl_mem_sta_sd_ivp1 : 1;
        unsigned int pctrl_mem_sta_sd_ivp0 : 1;
        unsigned int pctrl_mem_sta_sd_dec : 1;
        unsigned int pctrl_rom_sta_sd_dec : 1;
        unsigned int pctrl_mem_sta_sd_enc : 1;
        unsigned int pctrl_mem_sta_sd_peri : 1;
        unsigned int pctrl_mem_sta_sd_dmss_mpu : 1;
        unsigned int pctrl_mem_sta_ds_dmss_mpu : 1;
        unsigned int pctrl_mem_sta_sd_mddrc : 1;
        unsigned int reserved_2 : 1;
    } reg;
} SOC_PCTRL_PERI_STAT48_UNION;
#endif
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_ipp_START (1)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_ipp_END (1)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_dmss_sc_buffer0_START (2)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_dmss_sc_buffer0_END (2)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_dmss_sc_buffer1_START (3)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_dmss_sc_buffer1_END (3)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_arpp0_START (4)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_arpp0_END (4)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_arpp1_START (5)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_arpp1_END (5)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_arpp2_START (6)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_arpp2_END (6)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_arpp3_START (7)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_arpp3_END (7)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_arpp4_START (8)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_arpp4_END (8)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_dmc_pack_START (9)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_sd_dmc_pack_END (12)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_ds_dmc_pack_START (13)
#define SOC_PCTRL_PERI_STAT48_pctrl_retmem_sta_ds_dmc_pack_END (16)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_media1_sub_START (17)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_media1_sub_END (17)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_isp_noc_START (18)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_isp_noc_END (18)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_isp_fe_START (20)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_isp_fe_END (20)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_isp_bpecap_START (21)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_isp_bpecap_END (21)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_ivp1_START (22)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_ivp1_END (22)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_ivp0_START (23)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_ivp0_END (23)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_dec_START (24)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_dec_END (24)
#define SOC_PCTRL_PERI_STAT48_pctrl_rom_sta_sd_dec_START (25)
#define SOC_PCTRL_PERI_STAT48_pctrl_rom_sta_sd_dec_END (25)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_enc_START (26)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_enc_END (26)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_peri_START (27)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_peri_END (27)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_dmss_mpu_START (28)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_dmss_mpu_END (28)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_ds_dmss_mpu_START (29)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_ds_dmss_mpu_END (29)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_mddrc_START (30)
#define SOC_PCTRL_PERI_STAT48_pctrl_mem_sta_sd_mddrc_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pctrl_mem_sta_sd_gpu : 32;
    } reg;
} SOC_PCTRL_PERI_STAT49_UNION;
#endif
#define SOC_PCTRL_PERI_STAT49_pctrl_mem_sta_sd_gpu_START (0)
#define SOC_PCTRL_PERI_STAT49_pctrl_mem_sta_sd_gpu_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pctrl_mem_stat_sd_dmc : 4;
        unsigned int pctrl_mem_stat_sd_iputop : 1;
        unsigned int pctrl_mem_stat_sd_aicore1 : 1;
        unsigned int pctrl_mem_stat_sd_tiny : 1;
        unsigned int pctrl_mem_stat_sd_aicore : 1;
        unsigned int pctrl_mem_stat_sd_tscpu : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int pctrl_mem_sta_sd_modem : 20;
    } reg;
} SOC_PCTRL_PERI_STAT50_UNION;
#endif
#define SOC_PCTRL_PERI_STAT50_pctrl_mem_stat_sd_dmc_START (0)
#define SOC_PCTRL_PERI_STAT50_pctrl_mem_stat_sd_dmc_END (3)
#define SOC_PCTRL_PERI_STAT50_pctrl_mem_stat_sd_iputop_START (4)
#define SOC_PCTRL_PERI_STAT50_pctrl_mem_stat_sd_iputop_END (4)
#define SOC_PCTRL_PERI_STAT50_pctrl_mem_stat_sd_aicore1_START (5)
#define SOC_PCTRL_PERI_STAT50_pctrl_mem_stat_sd_aicore1_END (5)
#define SOC_PCTRL_PERI_STAT50_pctrl_mem_stat_sd_tiny_START (6)
#define SOC_PCTRL_PERI_STAT50_pctrl_mem_stat_sd_tiny_END (6)
#define SOC_PCTRL_PERI_STAT50_pctrl_mem_stat_sd_aicore_START (7)
#define SOC_PCTRL_PERI_STAT50_pctrl_mem_stat_sd_aicore_END (7)
#define SOC_PCTRL_PERI_STAT50_pctrl_mem_stat_sd_tscpu_START (8)
#define SOC_PCTRL_PERI_STAT50_pctrl_mem_stat_sd_tscpu_END (8)
#define SOC_PCTRL_PERI_STAT50_pctrl_mem_sta_sd_modem_START (12)
#define SOC_PCTRL_PERI_STAT50_pctrl_mem_sta_sd_modem_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pctrl_mem_sta_dslp_gpu : 32;
    } reg;
} SOC_PCTRL_PERI_STAT51_UNION;
#endif
#define SOC_PCTRL_PERI_STAT51_pctrl_mem_sta_dslp_gpu_START (0)
#define SOC_PCTRL_PERI_STAT51_pctrl_mem_sta_dslp_gpu_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pctrl_mem_stat_sd_system_cache0 : 9;
        unsigned int pctrl_mem_stat_sd_system_cache1 : 9;
        unsigned int pctrl_retmem_stat_sd_npu_localbuf : 4;
        unsigned int pctrl_retmem_stat_ds_npu_localbuf : 4;
        unsigned int pctrl_retmem_stat_sd_pac : 5;
        unsigned int reserved : 1;
    } reg;
} SOC_PCTRL_PERI_STAT52_UNION;
#endif
#define SOC_PCTRL_PERI_STAT52_pctrl_mem_stat_sd_system_cache0_START (0)
#define SOC_PCTRL_PERI_STAT52_pctrl_mem_stat_sd_system_cache0_END (8)
#define SOC_PCTRL_PERI_STAT52_pctrl_mem_stat_sd_system_cache1_START (9)
#define SOC_PCTRL_PERI_STAT52_pctrl_mem_stat_sd_system_cache1_END (17)
#define SOC_PCTRL_PERI_STAT52_pctrl_retmem_stat_sd_npu_localbuf_START (18)
#define SOC_PCTRL_PERI_STAT52_pctrl_retmem_stat_sd_npu_localbuf_END (21)
#define SOC_PCTRL_PERI_STAT52_pctrl_retmem_stat_ds_npu_localbuf_START (22)
#define SOC_PCTRL_PERI_STAT52_pctrl_retmem_stat_ds_npu_localbuf_END (25)
#define SOC_PCTRL_PERI_STAT52_pctrl_retmem_stat_sd_pac_START (26)
#define SOC_PCTRL_PERI_STAT52_pctrl_retmem_stat_sd_pac_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT53_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT54_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT55_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT56_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT57_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT58_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT59_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT60_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT61_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pwr_dwn_ack_0 : 1;
        unsigned int pwr_dwn_ack_1 : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_PCTRL_PERI_STAT62_UNION;
#endif
#define SOC_PCTRL_PERI_STAT62_pwr_dwn_ack_0_START (0)
#define SOC_PCTRL_PERI_STAT62_pwr_dwn_ack_0_END (0)
#define SOC_PCTRL_PERI_STAT62_pwr_dwn_ack_1_START (1)
#define SOC_PCTRL_PERI_STAT62_pwr_dwn_ack_1_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int media1_qic_nopendingtrans_1 : 32;
    } reg;
} SOC_PCTRL_PERI_STAT63_UNION;
#endif
#define SOC_PCTRL_PERI_STAT63_media1_qic_nopendingtrans_1_START (0)
#define SOC_PCTRL_PERI_STAT63_media1_qic_nopendingtrans_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int media1_qic_nopendingtrans_2 : 32;
    } reg;
} SOC_PCTRL_PERI_STAT64_UNION;
#endif
#define SOC_PCTRL_PERI_STAT64_media1_qic_nopendingtrans_2_START (0)
#define SOC_PCTRL_PERI_STAT64_media1_qic_nopendingtrans_2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int media2_qic_nopendingtrans_1 : 32;
    } reg;
} SOC_PCTRL_PERI_STAT66_UNION;
#endif
#define SOC_PCTRL_PERI_STAT66_media2_qic_nopendingtrans_1_START (0)
#define SOC_PCTRL_PERI_STAT66_media2_qic_nopendingtrans_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int media2_qic_nopendingtrans_2 : 32;
    } reg;
} SOC_PCTRL_PERI_STAT67_UNION;
#endif
#define SOC_PCTRL_PERI_STAT67_media2_qic_nopendingtrans_2_START (0)
#define SOC_PCTRL_PERI_STAT67_media2_qic_nopendingtrans_2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_mdm5g2ap_status : 32;
    } reg;
} SOC_PCTRL_PERI_STAT68_UNION;
#endif
#define SOC_PCTRL_PERI_STAT68_sc_mdm5g2ap_status_START (0)
#define SOC_PCTRL_PERI_STAT68_sc_mdm5g2ap_status_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int intr_repair_err : 1;
        unsigned int pack_is0_err : 1;
        unsigned int repair_start_point_err : 1;
        unsigned int reserved_1 : 28;
    } reg;
} SOC_PCTRL_PERI_STAT69_UNION;
#endif
#define SOC_PCTRL_PERI_STAT69_intr_repair_err_START (1)
#define SOC_PCTRL_PERI_STAT69_intr_repair_err_END (1)
#define SOC_PCTRL_PERI_STAT69_pack_is0_err_START (2)
#define SOC_PCTRL_PERI_STAT69_pack_is0_err_END (2)
#define SOC_PCTRL_PERI_STAT69_repair_start_point_err_START (3)
#define SOC_PCTRL_PERI_STAT69_repair_start_point_err_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_ddrc0_err : 1;
        unsigned int intr_ddrc1_err : 1;
        unsigned int intr_ddrc2_err : 1;
        unsigned int intr_ddrc3_err : 1;
        unsigned int intr_ddrphy : 4;
        unsigned int intr_uce0_wdog : 1;
        unsigned int intr_uce1_wdog : 1;
        unsigned int intr_uce2_wdog : 1;
        unsigned int intr_uce3_wdog : 1;
        unsigned int intr_ddrc_inban_ecc_err : 4;
        unsigned int reserved : 16;
    } reg;
} SOC_PCTRL_PERI_STAT70_UNION;
#endif
#define SOC_PCTRL_PERI_STAT70_intr_ddrc0_err_START (0)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc0_err_END (0)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc1_err_START (1)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc1_err_END (1)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc2_err_START (2)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc2_err_END (2)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc3_err_START (3)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc3_err_END (3)
#define SOC_PCTRL_PERI_STAT70_intr_ddrphy_START (4)
#define SOC_PCTRL_PERI_STAT70_intr_ddrphy_END (7)
#define SOC_PCTRL_PERI_STAT70_intr_uce0_wdog_START (8)
#define SOC_PCTRL_PERI_STAT70_intr_uce0_wdog_END (8)
#define SOC_PCTRL_PERI_STAT70_intr_uce1_wdog_START (9)
#define SOC_PCTRL_PERI_STAT70_intr_uce1_wdog_END (9)
#define SOC_PCTRL_PERI_STAT70_intr_uce2_wdog_START (10)
#define SOC_PCTRL_PERI_STAT70_intr_uce2_wdog_END (10)
#define SOC_PCTRL_PERI_STAT70_intr_uce3_wdog_START (11)
#define SOC_PCTRL_PERI_STAT70_intr_uce3_wdog_END (11)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc_inban_ecc_err_START (12)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc_inban_ecc_err_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int socp_hac_gif_axi_st : 18;
        unsigned int reserved : 14;
    } reg;
} SOC_PCTRL_PERI_STAT71_UNION;
#endif
#define SOC_PCTRL_PERI_STAT71_socp_hac_gif_axi_st_START (0)
#define SOC_PCTRL_PERI_STAT71_socp_hac_gif_axi_st_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ipf_hac_gif_axi_st : 18;
        unsigned int reserved : 14;
    } reg;
} SOC_PCTRL_PERI_STAT72_UNION;
#endif
#define SOC_PCTRL_PERI_STAT72_ipf_hac_gif_axi_st_START (0)
#define SOC_PCTRL_PERI_STAT72_ipf_hac_gif_axi_st_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int spe_hac_gif_axi_st : 18;
        unsigned int reserved : 14;
    } reg;
} SOC_PCTRL_PERI_STAT73_UNION;
#endif
#define SOC_PCTRL_PERI_STAT73_spe_hac_gif_axi_st_START (0)
#define SOC_PCTRL_PERI_STAT73_spe_hac_gif_axi_st_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int maa_hac_gif_axi_st : 23;
        unsigned int reserved : 9;
    } reg;
} SOC_PCTRL_PERI_STAT74_UNION;
#endif
#define SOC_PCTRL_PERI_STAT74_maa_hac_gif_axi_st_START (0)
#define SOC_PCTRL_PERI_STAT74_maa_hac_gif_axi_st_END (22)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT75_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT76_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT77_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ivp32_dwaxi_dlock_mst_ivp1 : 2;
        unsigned int ivp32_dwaxi_dlock_slv_ivp1 : 3;
        unsigned int ivp32_dwaxi_dlock_id_ivp1 : 6;
        unsigned int ivp32_dwaxi_dlock_wr_ivp1 : 1;
        unsigned int ivp32_dwaxi_dlock_irq_ivp1 : 1;
        unsigned int reserved : 19;
    } reg;
} SOC_PCTRL_PERI_STAT78_UNION;
#endif
#define SOC_PCTRL_PERI_STAT78_ivp32_dwaxi_dlock_mst_ivp1_START (0)
#define SOC_PCTRL_PERI_STAT78_ivp32_dwaxi_dlock_mst_ivp1_END (1)
#define SOC_PCTRL_PERI_STAT78_ivp32_dwaxi_dlock_slv_ivp1_START (2)
#define SOC_PCTRL_PERI_STAT78_ivp32_dwaxi_dlock_slv_ivp1_END (4)
#define SOC_PCTRL_PERI_STAT78_ivp32_dwaxi_dlock_id_ivp1_START (5)
#define SOC_PCTRL_PERI_STAT78_ivp32_dwaxi_dlock_id_ivp1_END (10)
#define SOC_PCTRL_PERI_STAT78_ivp32_dwaxi_dlock_wr_ivp1_START (11)
#define SOC_PCTRL_PERI_STAT78_ivp32_dwaxi_dlock_wr_ivp1_END (11)
#define SOC_PCTRL_PERI_STAT78_ivp32_dwaxi_dlock_irq_ivp1_START (12)
#define SOC_PCTRL_PERI_STAT78_ivp32_dwaxi_dlock_irq_ivp1_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT79_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT80_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT81_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT82_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT83_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT84_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT85_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT86_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT87_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT88_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT89_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT90_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT91_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT92_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT93_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 3;
        unsigned int bpram_tselr : 3;
        unsigned int bparm_tselw : 2;
        unsigned int bparm_test : 3;
        unsigned int reserved_1 : 3;
        unsigned int bpram_tselm : 2;
        unsigned int reserved_2 : 16;
    } reg;
} SOC_PCTRL_PERI_CTRL67_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL67_bpram_tselr_START (3)
#define SOC_PCTRL_PERI_CTRL67_bpram_tselr_END (5)
#define SOC_PCTRL_PERI_CTRL67_bparm_tselw_START (6)
#define SOC_PCTRL_PERI_CTRL67_bparm_tselw_END (7)
#define SOC_PCTRL_PERI_CTRL67_bparm_test_START (8)
#define SOC_PCTRL_PERI_CTRL67_bparm_test_END (10)
#define SOC_PCTRL_PERI_CTRL67_bpram_tselm_START (14)
#define SOC_PCTRL_PERI_CTRL67_bpram_tselm_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int peri_pcr_tpram_mem_ctrl : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL68_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL68_peri_pcr_tpram_mem_ctrl_START (0)
#define SOC_PCTRL_PERI_CTRL68_peri_pcr_tpram_mem_ctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int g3d_spram_mem_ctrl_lv : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL69_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL69_g3d_spram_mem_ctrl_lv_START (0)
#define SOC_PCTRL_PERI_CTRL69_g3d_spram_mem_ctrl_lv_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int g3d_tpram_mem_ctrl_lv : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL70_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL70_g3d_tpram_mem_ctrl_lv_START (0)
#define SOC_PCTRL_PERI_CTRL70_g3d_tpram_mem_ctrl_lv_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL71_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hisee_jtag_sel : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_PCTRL_PERI_CTRL72_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL72_hisee_jtag_sel_START (0)
#define SOC_PCTRL_PERI_CTRL72_hisee_jtag_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int lpm_ctrl_big : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_PCTRL_PERI_CTRL73_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL73_lpm_ctrl_big_START (0)
#define SOC_PCTRL_PERI_CTRL73_lpm_ctrl_big_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int lpm_ctrl_middle : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_PCTRL_PERI_CTRL74_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL74_lpm_ctrl_middle_START (0)
#define SOC_PCTRL_PERI_CTRL74_lpm_ctrl_middle_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int lpm_ctrl_little : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_PCTRL_PERI_CTRL88_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL88_lpm_ctrl_little_START (0)
#define SOC_PCTRL_PERI_CTRL88_lpm_ctrl_little_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 14;
        unsigned int intr_repair_err_msk : 1;
        unsigned int bisr_dbg_sel : 4;
        unsigned int reserved_1 : 13;
    } reg;
} SOC_PCTRL_PERI_CTRL89_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL89_intr_repair_err_msk_START (14)
#define SOC_PCTRL_PERI_CTRL89_intr_repair_err_msk_END (14)
#define SOC_PCTRL_PERI_CTRL89_bisr_dbg_sel_START (15)
#define SOC_PCTRL_PERI_CTRL89_bisr_dbg_sel_END (18)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ap2mdm_ctrl : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL90_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL90_ap2mdm_ctrl_START (0)
#define SOC_PCTRL_PERI_CTRL90_ap2mdm_ctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int g3d_ctrl_reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL91_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL91_g3d_ctrl_reserved_START (0)
#define SOC_PCTRL_PERI_CTRL91_g3d_ctrl_reserved_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_g3d_mem_dslp_hisi : 25;
        unsigned int reserved : 7;
    } reg;
} SOC_PCTRL_PERI_CTRL92_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL92_sc_g3d_mem_dslp_hisi_START (0)
#define SOC_PCTRL_PERI_CTRL92_sc_g3d_mem_dslp_hisi_END (24)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_g3d_mem_lp_cfg_hisi : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL93_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL93_sc_g3d_mem_lp_cfg_hisi_START (0)
#define SOC_PCTRL_PERI_CTRL93_sc_g3d_mem_lp_cfg_hisi_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_noc_timeout_en_syscache : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_PCTRL_PERI_CTRL95_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL95_sc_noc_timeout_en_syscache_START (0)
#define SOC_PCTRL_PERI_CTRL95_sc_noc_timeout_en_syscache_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ap2mdm5g_ctrl : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL96_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL96_ap2mdm5g_ctrl_START (0)
#define SOC_PCTRL_PERI_CTRL96_ap2mdm5g_ctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL97_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pasensor_cfg_valid : 1;
        unsigned int pasensor_cmd : 2;
        unsigned int pasensor_en : 1;
        unsigned int pasensor_ctrl : 6;
        unsigned int pasensor_sel : 6;
        unsigned int peri_ctrl105_msk : 16;
    } reg;
} SOC_PCTRL_PERI_CTRL105_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL105_pasensor_cfg_valid_START (0)
#define SOC_PCTRL_PERI_CTRL105_pasensor_cfg_valid_END (0)
#define SOC_PCTRL_PERI_CTRL105_pasensor_cmd_START (1)
#define SOC_PCTRL_PERI_CTRL105_pasensor_cmd_END (2)
#define SOC_PCTRL_PERI_CTRL105_pasensor_en_START (3)
#define SOC_PCTRL_PERI_CTRL105_pasensor_en_END (3)
#define SOC_PCTRL_PERI_CTRL105_pasensor_ctrl_START (4)
#define SOC_PCTRL_PERI_CTRL105_pasensor_ctrl_END (9)
#define SOC_PCTRL_PERI_CTRL105_pasensor_sel_START (10)
#define SOC_PCTRL_PERI_CTRL105_pasensor_sel_END (15)
#define SOC_PCTRL_PERI_CTRL105_peri_ctrl105_msk_START (16)
#define SOC_PCTRL_PERI_CTRL105_peri_ctrl105_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pasensor_msg : 16;
        unsigned int peri_ctrl106_msk : 16;
    } reg;
} SOC_PCTRL_PERI_CTRL106_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL106_pasensor_msg_START (0)
#define SOC_PCTRL_PERI_CTRL106_pasensor_msg_END (15)
#define SOC_PCTRL_PERI_CTRL106_peri_ctrl106_msk_START (16)
#define SOC_PCTRL_PERI_CTRL106_peri_ctrl106_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pctrl_mem_ctrl_sd_arpp0 : 1;
        unsigned int pctrl_mem_ctrl_sd_arpp1 : 1;
        unsigned int pctrl_mem_ctrl_sd_arpp2 : 1;
        unsigned int pctrl_mem_ctrl_sd_arpp3 : 1;
        unsigned int pctrl_mem_ctrl_sd_arpp4 : 1;
        unsigned int peri_ctrl107_cmd : 11;
        unsigned int peri_ctrl107_msk : 16;
    } reg;
} SOC_PCTRL_PERI_CTRL107_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL107_pctrl_mem_ctrl_sd_arpp0_START (0)
#define SOC_PCTRL_PERI_CTRL107_pctrl_mem_ctrl_sd_arpp0_END (0)
#define SOC_PCTRL_PERI_CTRL107_pctrl_mem_ctrl_sd_arpp1_START (1)
#define SOC_PCTRL_PERI_CTRL107_pctrl_mem_ctrl_sd_arpp1_END (1)
#define SOC_PCTRL_PERI_CTRL107_pctrl_mem_ctrl_sd_arpp2_START (2)
#define SOC_PCTRL_PERI_CTRL107_pctrl_mem_ctrl_sd_arpp2_END (2)
#define SOC_PCTRL_PERI_CTRL107_pctrl_mem_ctrl_sd_arpp3_START (3)
#define SOC_PCTRL_PERI_CTRL107_pctrl_mem_ctrl_sd_arpp3_END (3)
#define SOC_PCTRL_PERI_CTRL107_pctrl_mem_ctrl_sd_arpp4_START (4)
#define SOC_PCTRL_PERI_CTRL107_pctrl_mem_ctrl_sd_arpp4_END (4)
#define SOC_PCTRL_PERI_CTRL107_peri_ctrl107_cmd_START (5)
#define SOC_PCTRL_PERI_CTRL107_peri_ctrl107_cmd_END (15)
#define SOC_PCTRL_PERI_CTRL107_peri_ctrl107_msk_START (16)
#define SOC_PCTRL_PERI_CTRL107_peri_ctrl107_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ap2bba_ctrl : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL108_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL108_ap2bba_ctrl_START (0)
#define SOC_PCTRL_PERI_CTRL108_ap2bba_ctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int g3d_bpsram_mem_ctrl : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL109_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL109_g3d_bpsram_mem_ctrl_START (0)
#define SOC_PCTRL_PERI_CTRL109_g3d_bpsram_mem_ctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int g3d_bparam_mem_ctrl : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL110_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL110_g3d_bparam_mem_ctrl_START (0)
#define SOC_PCTRL_PERI_CTRL110_g3d_bparam_mem_ctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dpm_clk_en_gpu : 25;
        unsigned int volt_sel_gpu_core_dpm : 3;
        unsigned int freq_sel_gpu_core_dpm : 3;
        unsigned int reserved : 1;
    } reg;
} SOC_PCTRL_PERI_CTRL111_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL111_dpm_clk_en_gpu_START (0)
#define SOC_PCTRL_PERI_CTRL111_dpm_clk_en_gpu_END (24)
#define SOC_PCTRL_PERI_CTRL111_volt_sel_gpu_core_dpm_START (25)
#define SOC_PCTRL_PERI_CTRL111_volt_sel_gpu_core_dpm_END (27)
#define SOC_PCTRL_PERI_CTRL111_freq_sel_gpu_core_dpm_START (28)
#define SOC_PCTRL_PERI_CTRL111_freq_sel_gpu_core_dpm_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dpm_soft_rst_gpu : 25;
        unsigned int reserved_0 : 3;
        unsigned int freq_sel_gpu_top_dpm : 3;
        unsigned int reserved_1 : 1;
    } reg;
} SOC_PCTRL_PERI_CTRL112_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL112_dpm_soft_rst_gpu_START (0)
#define SOC_PCTRL_PERI_CTRL112_dpm_soft_rst_gpu_END (24)
#define SOC_PCTRL_PERI_CTRL112_freq_sel_gpu_top_dpm_START (28)
#define SOC_PCTRL_PERI_CTRL112_freq_sel_gpu_top_dpm_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int auto_clk_gt_byp_qcr : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_PCTRL_PERI_CTRL113_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL113_auto_clk_gt_byp_qcr_START (0)
#define SOC_PCTRL_PERI_CTRL113_auto_clk_gt_byp_qcr_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL114_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ipf_aw_lmtr_saturation : 14;
        unsigned int ipf_aw_lmtr_bandwidth : 13;
        unsigned int ipf_aw_lmtr_en : 1;
        unsigned int reserved : 4;
    } reg;
} SOC_PCTRL_PERI_CTRL115_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL115_ipf_aw_lmtr_saturation_START (0)
#define SOC_PCTRL_PERI_CTRL115_ipf_aw_lmtr_saturation_END (13)
#define SOC_PCTRL_PERI_CTRL115_ipf_aw_lmtr_bandwidth_START (14)
#define SOC_PCTRL_PERI_CTRL115_ipf_aw_lmtr_bandwidth_END (26)
#define SOC_PCTRL_PERI_CTRL115_ipf_aw_lmtr_en_START (27)
#define SOC_PCTRL_PERI_CTRL115_ipf_aw_lmtr_en_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ipf_ar_lmtr_saturation : 14;
        unsigned int ipf_ar_lmtr_bandwidth : 13;
        unsigned int ipf_ar_lmtr_en : 1;
        unsigned int reserved : 4;
    } reg;
} SOC_PCTRL_PERI_CTRL116_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL116_ipf_ar_lmtr_saturation_START (0)
#define SOC_PCTRL_PERI_CTRL116_ipf_ar_lmtr_saturation_END (13)
#define SOC_PCTRL_PERI_CTRL116_ipf_ar_lmtr_bandwidth_START (14)
#define SOC_PCTRL_PERI_CTRL116_ipf_ar_lmtr_bandwidth_END (26)
#define SOC_PCTRL_PERI_CTRL116_ipf_ar_lmtr_en_START (27)
#define SOC_PCTRL_PERI_CTRL116_ipf_ar_lmtr_en_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmac_aw_lmtr_saturation : 14;
        unsigned int dmac_aw_lmtr_bandwidth : 13;
        unsigned int dmac_aw_lmtr_en : 1;
        unsigned int reserved : 4;
    } reg;
} SOC_PCTRL_PERI_CTRL117_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL117_dmac_aw_lmtr_saturation_START (0)
#define SOC_PCTRL_PERI_CTRL117_dmac_aw_lmtr_saturation_END (13)
#define SOC_PCTRL_PERI_CTRL117_dmac_aw_lmtr_bandwidth_START (14)
#define SOC_PCTRL_PERI_CTRL117_dmac_aw_lmtr_bandwidth_END (26)
#define SOC_PCTRL_PERI_CTRL117_dmac_aw_lmtr_en_START (27)
#define SOC_PCTRL_PERI_CTRL117_dmac_aw_lmtr_en_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmac_ar_lmtr_saturation : 14;
        unsigned int dmac_ar_lmtr_bandwidth : 13;
        unsigned int dmac_ar_lmtr_en : 1;
        unsigned int reserved : 4;
    } reg;
} SOC_PCTRL_PERI_CTRL118_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL118_dmac_ar_lmtr_saturation_START (0)
#define SOC_PCTRL_PERI_CTRL118_dmac_ar_lmtr_saturation_END (13)
#define SOC_PCTRL_PERI_CTRL118_dmac_ar_lmtr_bandwidth_START (14)
#define SOC_PCTRL_PERI_CTRL118_dmac_ar_lmtr_bandwidth_END (26)
#define SOC_PCTRL_PERI_CTRL118_dmac_ar_lmtr_en_START (27)
#define SOC_PCTRL_PERI_CTRL118_dmac_ar_lmtr_en_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int modem_aw_lmtr_saturation : 14;
        unsigned int modem_aw_lmtr_bandwidth : 13;
        unsigned int modem_aw_lmtr_en : 1;
        unsigned int reserved : 4;
    } reg;
} SOC_PCTRL_PERI_CTRL119_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL119_modem_aw_lmtr_saturation_START (0)
#define SOC_PCTRL_PERI_CTRL119_modem_aw_lmtr_saturation_END (13)
#define SOC_PCTRL_PERI_CTRL119_modem_aw_lmtr_bandwidth_START (14)
#define SOC_PCTRL_PERI_CTRL119_modem_aw_lmtr_bandwidth_END (26)
#define SOC_PCTRL_PERI_CTRL119_modem_aw_lmtr_en_START (27)
#define SOC_PCTRL_PERI_CTRL119_modem_aw_lmtr_en_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int modem_ar_lmtr_saturation : 14;
        unsigned int modem_ar_lmtr_bandwidth : 13;
        unsigned int modem_ar_lmtr_en : 1;
        unsigned int reserved : 4;
    } reg;
} SOC_PCTRL_PERI_CTRL120_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL120_modem_ar_lmtr_saturation_START (0)
#define SOC_PCTRL_PERI_CTRL120_modem_ar_lmtr_saturation_END (13)
#define SOC_PCTRL_PERI_CTRL120_modem_ar_lmtr_bandwidth_START (14)
#define SOC_PCTRL_PERI_CTRL120_modem_ar_lmtr_bandwidth_END (26)
#define SOC_PCTRL_PERI_CTRL120_modem_ar_lmtr_en_START (27)
#define SOC_PCTRL_PERI_CTRL120_modem_ar_lmtr_en_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_spe_awhint : 4;
        unsigned int sc_spe_arhint : 4;
        unsigned int reserved : 24;
    } reg;
} SOC_PCTRL_PERI_CTRL121_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL121_sc_spe_awhint_START (0)
#define SOC_PCTRL_PERI_CTRL121_sc_spe_awhint_END (3)
#define SOC_PCTRL_PERI_CTRL121_sc_spe_arhint_START (4)
#define SOC_PCTRL_PERI_CTRL121_sc_spe_arhint_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL122_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL123_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL124_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL125_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL126_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL127_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL128_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL129_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL130_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL131_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL132_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL133_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL134_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL135_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL136_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL137_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL138_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL139_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL140_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource0lock_cmd0 : 1;
        unsigned int resource0lock_id0 : 3;
        unsigned int resource0lock_cmd1 : 1;
        unsigned int resource0lock_id1 : 3;
        unsigned int resource0lock_cmd2 : 1;
        unsigned int resource0lock_id2 : 3;
        unsigned int resource0lock_cmd3 : 1;
        unsigned int resource0lock_id3 : 3;
        unsigned int resource0lock_cmd4 : 1;
        unsigned int resource0lock_id4 : 3;
        unsigned int resource0lock_cmd5 : 1;
        unsigned int resource0lock_id5 : 3;
        unsigned int resource0lock_cmd6 : 1;
        unsigned int resource0lock_id6 : 3;
        unsigned int resource0lock_cmd7 : 1;
        unsigned int resource0lock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE0_LOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id0_START (1)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id0_END (3)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id1_START (5)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id1_END (7)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id2_START (9)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id2_END (11)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id3_START (13)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id3_END (15)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id4_START (17)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id4_END (19)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id5_START (21)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id5_END (23)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id6_START (25)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id6_END (27)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id7_START (29)
#define SOC_PCTRL_RESOURCE0_LOCK_resource0lock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource0unlock_cmd0 : 1;
        unsigned int resource0unlock_id0 : 3;
        unsigned int resource0unlock_cmd1 : 1;
        unsigned int resource0unlock_id1 : 3;
        unsigned int resource0unlock_cmd2 : 1;
        unsigned int resource0unlock_id2 : 3;
        unsigned int resource0unlock_cmd3 : 1;
        unsigned int resource0unlock_id3 : 3;
        unsigned int resource0unlock_cmd4 : 1;
        unsigned int resource0unlock_id4 : 3;
        unsigned int resource0unlock_cmd5 : 1;
        unsigned int resource0unlock_id5 : 3;
        unsigned int resource0unlock_cmd6 : 1;
        unsigned int resource0unlock_id6 : 3;
        unsigned int resource0unlock_cmd7 : 1;
        unsigned int resource0unlock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE0_UNLOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id0_START (1)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id0_END (3)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id1_START (5)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id1_END (7)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id2_START (9)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id2_END (11)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id3_START (13)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id3_END (15)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id4_START (17)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id4_END (19)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id5_START (21)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id5_END (23)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id6_START (25)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id6_END (27)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id7_START (29)
#define SOC_PCTRL_RESOURCE0_UNLOCK_resource0unlock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource0lock_st0 : 1;
        unsigned int resource0lock_st_id0 : 3;
        unsigned int resource0lock_st1 : 1;
        unsigned int resource0lock_st_id1 : 3;
        unsigned int resource0lock_st2 : 1;
        unsigned int resource0lock_st_id2 : 3;
        unsigned int resource0lock_st3 : 1;
        unsigned int resource0lock_st_id3 : 3;
        unsigned int resource0lock_st4 : 1;
        unsigned int resource0lock_st_id4 : 3;
        unsigned int resource0lock_st5 : 1;
        unsigned int resource0lock_st_id5 : 3;
        unsigned int resource0lock_st6 : 1;
        unsigned int resource0lock_st_id6 : 3;
        unsigned int resource0lock_st7 : 1;
        unsigned int resource0lock_st_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE0_LOCK_ST_UNION;
#endif
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st0_START (0)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st0_END (0)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id0_START (1)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id0_END (3)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st1_START (4)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st1_END (4)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id1_START (5)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id1_END (7)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st2_START (8)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st2_END (8)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id2_START (9)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id2_END (11)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st3_START (12)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st3_END (12)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id3_START (13)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id3_END (15)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st4_START (16)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st4_END (16)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id4_START (17)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id4_END (19)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st5_START (20)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st5_END (20)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id5_START (21)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id5_END (23)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st6_START (24)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st6_END (24)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id6_START (25)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id6_END (27)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st7_START (28)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st7_END (28)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id7_START (29)
#define SOC_PCTRL_RESOURCE0_LOCK_ST_resource0lock_st_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource1lock_cmd0 : 1;
        unsigned int resource1lock_id0 : 3;
        unsigned int resource1lock_cmd1 : 1;
        unsigned int resource1lock_id1 : 3;
        unsigned int resource1lock_cmd2 : 1;
        unsigned int resource1lock_id2 : 3;
        unsigned int resource1lock_cmd3 : 1;
        unsigned int resource1lock_id3 : 3;
        unsigned int resource1lock_cmd4 : 1;
        unsigned int resource1lock_id4 : 3;
        unsigned int resource1lock_cmd5 : 1;
        unsigned int resource1lock_id5 : 3;
        unsigned int resource1lock_cmd6 : 1;
        unsigned int resource1lock_id6 : 3;
        unsigned int resource1lock_cmd7 : 1;
        unsigned int resource1lock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE1_LOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id0_START (1)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id0_END (3)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id1_START (5)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id1_END (7)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id2_START (9)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id2_END (11)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id3_START (13)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id3_END (15)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id4_START (17)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id4_END (19)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id5_START (21)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id5_END (23)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id6_START (25)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id6_END (27)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id7_START (29)
#define SOC_PCTRL_RESOURCE1_LOCK_resource1lock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource1unlock_cmd0 : 1;
        unsigned int resource1unlock_id0 : 3;
        unsigned int resource1unlock_cmd1 : 1;
        unsigned int resource1unlock_id1 : 3;
        unsigned int resource1unlock_cmd2 : 1;
        unsigned int resource1unlock_id2 : 3;
        unsigned int resource1unlock_cmd3 : 1;
        unsigned int resource1unlock_id3 : 3;
        unsigned int resource1unlock_cmd4 : 1;
        unsigned int resource1unlock_id4 : 3;
        unsigned int resource1unlock_cmd5 : 1;
        unsigned int resource1unlock_id5 : 3;
        unsigned int resource1unlock_cmd6 : 1;
        unsigned int resource1unlock_id6 : 3;
        unsigned int resource1unlock_cmd7 : 1;
        unsigned int resource1unlock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE1_UNLOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id0_START (1)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id0_END (3)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id1_START (5)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id1_END (7)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id2_START (9)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id2_END (11)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id3_START (13)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id3_END (15)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id4_START (17)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id4_END (19)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id5_START (21)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id5_END (23)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id6_START (25)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id6_END (27)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id7_START (29)
#define SOC_PCTRL_RESOURCE1_UNLOCK_resource1unlock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource1lock_st0 : 1;
        unsigned int resource1lock_st_id0 : 3;
        unsigned int resource1lock_st1 : 1;
        unsigned int resource1lock_st_id1 : 3;
        unsigned int resource1lock_st2 : 1;
        unsigned int resource1lock_st_id2 : 3;
        unsigned int resource1lock_st3 : 1;
        unsigned int resource1lock_st_id3 : 3;
        unsigned int resource1lock_st4 : 1;
        unsigned int resource1lock_st_id4 : 3;
        unsigned int resource1lock_st5 : 1;
        unsigned int resource1lock_st_id5 : 3;
        unsigned int resource1lock_st6 : 1;
        unsigned int resource1lock_st_id6 : 3;
        unsigned int resource1lock_st7 : 1;
        unsigned int resource1lock_st_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE1_LOCK_ST_UNION;
#endif
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st0_START (0)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st0_END (0)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id0_START (1)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id0_END (3)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st1_START (4)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st1_END (4)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id1_START (5)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id1_END (7)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st2_START (8)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st2_END (8)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id2_START (9)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id2_END (11)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st3_START (12)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st3_END (12)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id3_START (13)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id3_END (15)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st4_START (16)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st4_END (16)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id4_START (17)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id4_END (19)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st5_START (20)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st5_END (20)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id5_START (21)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id5_END (23)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st6_START (24)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st6_END (24)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id6_START (25)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id6_END (27)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st7_START (28)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st7_END (28)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id7_START (29)
#define SOC_PCTRL_RESOURCE1_LOCK_ST_resource1lock_st_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource2lock_cmd0 : 1;
        unsigned int resource2lock_id0 : 3;
        unsigned int resource2lock_cmd1 : 1;
        unsigned int resource2lock_id1 : 3;
        unsigned int resource2lock_cmd2 : 1;
        unsigned int resource2lock_id2 : 3;
        unsigned int resource2lock_cmd3 : 1;
        unsigned int resource2lock_id3 : 3;
        unsigned int resource2lock_cmd4 : 1;
        unsigned int resource2lock_id4 : 3;
        unsigned int resource2lock_cmd5 : 1;
        unsigned int resource2lock_id5 : 3;
        unsigned int resource2lock_cmd6 : 1;
        unsigned int resource2lock_id6 : 3;
        unsigned int resource2lock_cmd7 : 1;
        unsigned int resource2lock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE2_LOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id0_START (1)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id0_END (3)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id1_START (5)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id1_END (7)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id2_START (9)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id2_END (11)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id3_START (13)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id3_END (15)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id4_START (17)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id4_END (19)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id5_START (21)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id5_END (23)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id6_START (25)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id6_END (27)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id7_START (29)
#define SOC_PCTRL_RESOURCE2_LOCK_resource2lock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource2unlock_cmd0 : 1;
        unsigned int resource2unlock_id0 : 3;
        unsigned int resource2unlock_cmd1 : 1;
        unsigned int resource2unlock_id1 : 3;
        unsigned int resource2unlock_cmd2 : 1;
        unsigned int resource2unlock_id2 : 3;
        unsigned int resource2unlock_cmd3 : 1;
        unsigned int resource2unlock_id3 : 3;
        unsigned int resource2unlock_cmd4 : 1;
        unsigned int resource2unlock_id4 : 3;
        unsigned int resource2unlock_cmd5 : 1;
        unsigned int resource2unlock_id5 : 3;
        unsigned int resource2unlock_cmd6 : 1;
        unsigned int resource2unlock_id6 : 3;
        unsigned int resource2unlock_cmd7 : 1;
        unsigned int resource2unlock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE2_UNLOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id0_START (1)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id0_END (3)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id1_START (5)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id1_END (7)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id2_START (9)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id2_END (11)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id3_START (13)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id3_END (15)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id4_START (17)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id4_END (19)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id5_START (21)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id5_END (23)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id6_START (25)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id6_END (27)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id7_START (29)
#define SOC_PCTRL_RESOURCE2_UNLOCK_resource2unlock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource2lock_st0 : 1;
        unsigned int resource2lock_st_id0 : 3;
        unsigned int resource2lock_st1 : 1;
        unsigned int resource2lock_st_id1 : 3;
        unsigned int resource2lock_st2 : 1;
        unsigned int resource2lock_st_id2 : 3;
        unsigned int resource2lock_st3 : 1;
        unsigned int resource2lock_st_id3 : 3;
        unsigned int resource2lock_st4 : 1;
        unsigned int resource2lock_st_id4 : 3;
        unsigned int resource2lock_st5 : 1;
        unsigned int resource2lock_st_id5 : 3;
        unsigned int resource2lock_st6 : 1;
        unsigned int resource2lock_st_id6 : 3;
        unsigned int resource2lock_st7 : 1;
        unsigned int resource2lock_st_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE2_LOCK_ST_UNION;
#endif
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st0_START (0)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st0_END (0)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id0_START (1)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id0_END (3)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st1_START (4)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st1_END (4)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id1_START (5)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id1_END (7)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st2_START (8)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st2_END (8)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id2_START (9)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id2_END (11)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st3_START (12)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st3_END (12)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id3_START (13)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id3_END (15)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st4_START (16)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st4_END (16)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id4_START (17)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id4_END (19)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st5_START (20)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st5_END (20)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id5_START (21)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id5_END (23)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st6_START (24)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st6_END (24)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id6_START (25)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id6_END (27)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st7_START (28)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st7_END (28)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id7_START (29)
#define SOC_PCTRL_RESOURCE2_LOCK_ST_resource2lock_st_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource3lock_cmd0 : 1;
        unsigned int resource3lock_id0 : 3;
        unsigned int resource3lock_cmd1 : 1;
        unsigned int resource3lock_id1 : 3;
        unsigned int resource3lock_cmd2 : 1;
        unsigned int resource3lock_id2 : 3;
        unsigned int resource3lock_cmd3 : 1;
        unsigned int resource3lock_id3 : 3;
        unsigned int resource3lock_cmd4 : 1;
        unsigned int resource3lock_id4 : 3;
        unsigned int resource3lock_cmd5 : 1;
        unsigned int resource3lock_id5 : 3;
        unsigned int resource3lock_cmd6 : 1;
        unsigned int resource3lock_id6 : 3;
        unsigned int resource3lock_cmd7 : 1;
        unsigned int resource3lock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE3_LOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id0_START (1)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id0_END (3)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id1_START (5)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id1_END (7)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id2_START (9)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id2_END (11)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id3_START (13)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id3_END (15)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id4_START (17)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id4_END (19)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id5_START (21)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id5_END (23)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id6_START (25)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id6_END (27)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id7_START (29)
#define SOC_PCTRL_RESOURCE3_LOCK_resource3lock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource3unlock_cmd0 : 1;
        unsigned int resource3unlock_id0 : 3;
        unsigned int resource3unlock_cmd1 : 1;
        unsigned int resource3unlock_id1 : 3;
        unsigned int resource3unlock_cmd2 : 1;
        unsigned int resource3unlock_id2 : 3;
        unsigned int resource3unlock_cmd3 : 1;
        unsigned int resource3unlock_id3 : 3;
        unsigned int resource3unlock_cmd4 : 1;
        unsigned int resource3unlock_id4 : 3;
        unsigned int resource3unlock_cmd5 : 1;
        unsigned int resource3unlock_id5 : 3;
        unsigned int resource3unlock_cmd6 : 1;
        unsigned int resource3unlock_id6 : 3;
        unsigned int resource3unlock_cmd7 : 1;
        unsigned int resource3unlock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE3_UNLOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id0_START (1)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id0_END (3)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id1_START (5)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id1_END (7)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id2_START (9)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id2_END (11)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id3_START (13)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id3_END (15)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id4_START (17)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id4_END (19)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id5_START (21)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id5_END (23)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id6_START (25)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id6_END (27)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id7_START (29)
#define SOC_PCTRL_RESOURCE3_UNLOCK_resource3unlock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource3lock_st0 : 1;
        unsigned int resource3lock_st_id0 : 3;
        unsigned int resource3lock_st1 : 1;
        unsigned int resource3lock_st_id1 : 3;
        unsigned int resource3lock_st2 : 1;
        unsigned int resource3lock_st_id2 : 3;
        unsigned int resource3lock_st3 : 1;
        unsigned int resource3lock_st_id3 : 3;
        unsigned int resource3lock_st4 : 1;
        unsigned int resource3lock_st_id4 : 3;
        unsigned int resource3lock_st5 : 1;
        unsigned int resource3lock_st_id5 : 3;
        unsigned int resource3lock_st6 : 1;
        unsigned int resource3lock_st_id6 : 3;
        unsigned int resource3lock_st7 : 1;
        unsigned int resource3lock_st_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE3_LOCK_ST_UNION;
#endif
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st0_START (0)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st0_END (0)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id0_START (1)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id0_END (3)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st1_START (4)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st1_END (4)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id1_START (5)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id1_END (7)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st2_START (8)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st2_END (8)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id2_START (9)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id2_END (11)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st3_START (12)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st3_END (12)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id3_START (13)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id3_END (15)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st4_START (16)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st4_END (16)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id4_START (17)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id4_END (19)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st5_START (20)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st5_END (20)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id5_START (21)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id5_END (23)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st6_START (24)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st6_END (24)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id6_START (25)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id6_END (27)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st7_START (28)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st7_END (28)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id7_START (29)
#define SOC_PCTRL_RESOURCE3_LOCK_ST_resource3lock_st_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource4lock_cmd0 : 1;
        unsigned int resource4lock_id0 : 3;
        unsigned int resource4lock_cmd1 : 1;
        unsigned int resource4lock_id1 : 3;
        unsigned int resource4lock_cmd2 : 1;
        unsigned int resource4lock_id2 : 3;
        unsigned int resource4lock_cmd3 : 1;
        unsigned int resource4lock_id3 : 3;
        unsigned int resource4lock_cmd4 : 1;
        unsigned int resource4lock_id4 : 3;
        unsigned int resource4lock_cmd5 : 1;
        unsigned int resource4lock_id5 : 3;
        unsigned int resource4lock_cmd6 : 1;
        unsigned int resource4lock_id6 : 3;
        unsigned int resource4lock_cmd7 : 1;
        unsigned int resource4lock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE4_LOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id0_START (1)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id0_END (3)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id1_START (5)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id1_END (7)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id2_START (9)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id2_END (11)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id3_START (13)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id3_END (15)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id4_START (17)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id4_END (19)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id5_START (21)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id5_END (23)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id6_START (25)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id6_END (27)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id7_START (29)
#define SOC_PCTRL_RESOURCE4_LOCK_resource4lock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource4unlock_cmd0 : 1;
        unsigned int resource4unlock_id0 : 3;
        unsigned int resource4unlock_cmd1 : 1;
        unsigned int resource4unlock_id1 : 3;
        unsigned int resource4unlock_cmd2 : 1;
        unsigned int resource4unlock_id2 : 3;
        unsigned int resource4unlock_cmd3 : 1;
        unsigned int resource4unlock_id3 : 3;
        unsigned int resource4unlock_cmd4 : 1;
        unsigned int resource4unlock_id4 : 3;
        unsigned int resource4unlock_cmd5 : 1;
        unsigned int resource4unlock_id5 : 3;
        unsigned int resource4unlock_cmd6 : 1;
        unsigned int resource4unlock_id6 : 3;
        unsigned int resource4unlock_cmd7 : 1;
        unsigned int resource4unlock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE4_UNLOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id0_START (1)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id0_END (3)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id1_START (5)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id1_END (7)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id2_START (9)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id2_END (11)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id3_START (13)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id3_END (15)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id4_START (17)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id4_END (19)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id5_START (21)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id5_END (23)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id6_START (25)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id6_END (27)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id7_START (29)
#define SOC_PCTRL_RESOURCE4_UNLOCK_resource4unlock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource4lock_st0 : 1;
        unsigned int resource4lock_st_id0 : 3;
        unsigned int resource4lock_st1 : 1;
        unsigned int resource4lock_st_id1 : 3;
        unsigned int resource4lock_st2 : 1;
        unsigned int resource4lock_st_id2 : 3;
        unsigned int resource4lock_st3 : 1;
        unsigned int resource4lock_st_id3 : 3;
        unsigned int resource4lock_st4 : 1;
        unsigned int resource4lock_st_id4 : 3;
        unsigned int resource4lock_st5 : 1;
        unsigned int resource4lock_st_id5 : 3;
        unsigned int resource4lock_st6 : 1;
        unsigned int resource4lock_st_id6 : 3;
        unsigned int resource4lock_st7 : 1;
        unsigned int resource4lock_st_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE4_LOCK_ST_UNION;
#endif
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st0_START (0)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st0_END (0)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id0_START (1)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id0_END (3)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st1_START (4)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st1_END (4)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id1_START (5)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id1_END (7)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st2_START (8)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st2_END (8)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id2_START (9)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id2_END (11)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st3_START (12)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st3_END (12)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id3_START (13)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id3_END (15)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st4_START (16)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st4_END (16)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id4_START (17)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id4_END (19)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st5_START (20)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st5_END (20)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id5_START (21)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id5_END (23)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st6_START (24)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st6_END (24)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id6_START (25)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id6_END (27)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st7_START (28)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st7_END (28)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id7_START (29)
#define SOC_PCTRL_RESOURCE4_LOCK_ST_resource4lock_st_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource5lock_cmd0 : 1;
        unsigned int resource5lock_id0 : 3;
        unsigned int resource5lock_cmd1 : 1;
        unsigned int resource5lock_id1 : 3;
        unsigned int resource5lock_cmd2 : 1;
        unsigned int resource5lock_id2 : 3;
        unsigned int resource5lock_cmd3 : 1;
        unsigned int resource5lock_id3 : 3;
        unsigned int resource5lock_cmd4 : 1;
        unsigned int resource5lock_id4 : 3;
        unsigned int resource5lock_cmd5 : 1;
        unsigned int resource5lock_id5 : 3;
        unsigned int resource5lock_cmd6 : 1;
        unsigned int resource5lock_id6 : 3;
        unsigned int resource5lock_cmd7 : 1;
        unsigned int resource5lock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE5_LOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id0_START (1)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id0_END (3)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id1_START (5)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id1_END (7)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id2_START (9)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id2_END (11)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id3_START (13)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id3_END (15)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id4_START (17)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id4_END (19)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id5_START (21)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id5_END (23)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id6_START (25)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id6_END (27)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id7_START (29)
#define SOC_PCTRL_RESOURCE5_LOCK_resource5lock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource5unlock_cmd0 : 1;
        unsigned int resource5unlock_id0 : 3;
        unsigned int resource5unlock_cmd1 : 1;
        unsigned int resource5unlock_id1 : 3;
        unsigned int resource5unlock_cmd2 : 1;
        unsigned int resource5unlock_id2 : 3;
        unsigned int resource5unlock_cmd3 : 1;
        unsigned int resource5unlock_id3 : 3;
        unsigned int resource5unlock_cmd4 : 1;
        unsigned int resource5unlock_id4 : 3;
        unsigned int resource5unlock_cmd5 : 1;
        unsigned int resource5unlock_id5 : 3;
        unsigned int resource5unlock_cmd6 : 1;
        unsigned int resource5unlock_id6 : 3;
        unsigned int resource5unlock_cmd7 : 1;
        unsigned int resource5unlock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE5_UNLOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id0_START (1)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id0_END (3)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id1_START (5)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id1_END (7)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id2_START (9)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id2_END (11)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id3_START (13)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id3_END (15)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id4_START (17)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id4_END (19)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id5_START (21)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id5_END (23)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id6_START (25)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id6_END (27)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id7_START (29)
#define SOC_PCTRL_RESOURCE5_UNLOCK_resource5unlock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource5lock_st0 : 1;
        unsigned int resource5lock_st_id0 : 3;
        unsigned int resource5lock_st1 : 1;
        unsigned int resource5lock_st_id1 : 3;
        unsigned int resource5lock_st2 : 1;
        unsigned int resource5lock_st_id2 : 3;
        unsigned int resource5lock_st3 : 1;
        unsigned int resource5lock_st_id3 : 3;
        unsigned int resource5lock_st4 : 1;
        unsigned int resource5lock_st_id4 : 3;
        unsigned int resource5lock_st5 : 1;
        unsigned int resource5lock_st_id5 : 3;
        unsigned int resource5lock_st6 : 1;
        unsigned int resource5lock_st_id6 : 3;
        unsigned int resource5lock_st7 : 1;
        unsigned int resource5lock_st_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE5_LOCK_ST_UNION;
#endif
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st0_START (0)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st0_END (0)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id0_START (1)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id0_END (3)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st1_START (4)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st1_END (4)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id1_START (5)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id1_END (7)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st2_START (8)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st2_END (8)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id2_START (9)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id2_END (11)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st3_START (12)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st3_END (12)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id3_START (13)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id3_END (15)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st4_START (16)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st4_END (16)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id4_START (17)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id4_END (19)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st5_START (20)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st5_END (20)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id5_START (21)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id5_END (23)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st6_START (24)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st6_END (24)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id6_START (25)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id6_END (27)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st7_START (28)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st7_END (28)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id7_START (29)
#define SOC_PCTRL_RESOURCE5_LOCK_ST_resource5lock_st_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource6lock_cmd0 : 1;
        unsigned int resource6lock_id0 : 3;
        unsigned int resource6lock_cmd1 : 1;
        unsigned int resource6lock_id1 : 3;
        unsigned int resource6lock_cmd2 : 1;
        unsigned int resource6lock_id2 : 3;
        unsigned int resource6lock_cmd3 : 1;
        unsigned int resource6lock_id3 : 3;
        unsigned int resource6lock_cmd4 : 1;
        unsigned int resource6lock_id4 : 3;
        unsigned int resource6lock_cmd5 : 1;
        unsigned int resource6lock_id5 : 3;
        unsigned int resource6lock_cmd6 : 1;
        unsigned int resource6lock_id6 : 3;
        unsigned int resource6lock_cmd7 : 1;
        unsigned int resource6lock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE6_LOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id0_START (1)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id0_END (3)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id1_START (5)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id1_END (7)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id2_START (9)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id2_END (11)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id3_START (13)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id3_END (15)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id4_START (17)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id4_END (19)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id5_START (21)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id5_END (23)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id6_START (25)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id6_END (27)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id7_START (29)
#define SOC_PCTRL_RESOURCE6_LOCK_resource6lock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource6unlock_cmd0 : 1;
        unsigned int resource6unlock_id0 : 3;
        unsigned int resource6unlock_cmd1 : 1;
        unsigned int resource6unlock_id1 : 3;
        unsigned int resource6unlock_cmd2 : 1;
        unsigned int resource6unlock_id2 : 3;
        unsigned int resource6unlock_cmd3 : 1;
        unsigned int resource6unlock_id3 : 3;
        unsigned int resource6unlock_cmd4 : 1;
        unsigned int resource6unlock_id4 : 3;
        unsigned int resource6unlock_cmd5 : 1;
        unsigned int resource6unlock_id5 : 3;
        unsigned int resource6unlock_cmd6 : 1;
        unsigned int resource6unlock_id6 : 3;
        unsigned int resource6unlock_cmd7 : 1;
        unsigned int resource6unlock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE6_UNLOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id0_START (1)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id0_END (3)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id1_START (5)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id1_END (7)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id2_START (9)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id2_END (11)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id3_START (13)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id3_END (15)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id4_START (17)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id4_END (19)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id5_START (21)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id5_END (23)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id6_START (25)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id6_END (27)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id7_START (29)
#define SOC_PCTRL_RESOURCE6_UNLOCK_resource6unlock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource6lock_st0 : 1;
        unsigned int resource6lock_st_id0 : 3;
        unsigned int resource6lock_st1 : 1;
        unsigned int resource6lock_st_id1 : 3;
        unsigned int resource6lock_st2 : 1;
        unsigned int resource6lock_st_id2 : 3;
        unsigned int resource6lock_st3 : 1;
        unsigned int resource6lock_st_id3 : 3;
        unsigned int resource6lock_st4 : 1;
        unsigned int resource6lock_st_id4 : 3;
        unsigned int resource6lock_st5 : 1;
        unsigned int resource6lock_st_id5 : 3;
        unsigned int resource6lock_st6 : 1;
        unsigned int resource6lock_st_id6 : 3;
        unsigned int resource6lock_st7 : 1;
        unsigned int resource6lock_st_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE6_LOCK_ST_UNION;
#endif
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st0_START (0)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st0_END (0)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id0_START (1)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id0_END (3)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st1_START (4)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st1_END (4)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id1_START (5)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id1_END (7)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st2_START (8)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st2_END (8)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id2_START (9)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id2_END (11)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st3_START (12)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st3_END (12)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id3_START (13)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id3_END (15)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st4_START (16)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st4_END (16)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id4_START (17)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id4_END (19)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st5_START (20)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st5_END (20)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id5_START (21)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id5_END (23)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st6_START (24)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st6_END (24)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id6_START (25)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id6_END (27)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st7_START (28)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st7_END (28)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id7_START (29)
#define SOC_PCTRL_RESOURCE6_LOCK_ST_resource6lock_st_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource7lock_cmd0 : 1;
        unsigned int resource7lock_id0 : 3;
        unsigned int resource7lock_cmd1 : 1;
        unsigned int resource7lock_id1 : 3;
        unsigned int resource7lock_cmd2 : 1;
        unsigned int resource7lock_id2 : 3;
        unsigned int resource7lock_cmd3 : 1;
        unsigned int resource7lock_id3 : 3;
        unsigned int resource7lock_cmd4 : 1;
        unsigned int resource7lock_id4 : 3;
        unsigned int resource7lock_cmd5 : 1;
        unsigned int resource7lock_id5 : 3;
        unsigned int resource7lock_cmd6 : 1;
        unsigned int resource7lock_id6 : 3;
        unsigned int resource7lock_cmd7 : 1;
        unsigned int resource7lock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE7_LOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id0_START (1)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id0_END (3)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id1_START (5)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id1_END (7)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id2_START (9)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id2_END (11)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id3_START (13)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id3_END (15)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id4_START (17)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id4_END (19)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id5_START (21)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id5_END (23)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id6_START (25)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id6_END (27)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id7_START (29)
#define SOC_PCTRL_RESOURCE7_LOCK_resource7lock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource7unlock_cmd0 : 1;
        unsigned int resource7unlock_id0 : 3;
        unsigned int resource7unlock_cmd1 : 1;
        unsigned int resource7unlock_id1 : 3;
        unsigned int resource7unlock_cmd2 : 1;
        unsigned int resource7unlock_id2 : 3;
        unsigned int resource7unlock_cmd3 : 1;
        unsigned int resource7unlock_id3 : 3;
        unsigned int resource7unlock_cmd4 : 1;
        unsigned int resource7unlock_id4 : 3;
        unsigned int resource7unlock_cmd5 : 1;
        unsigned int resource7unlock_id5 : 3;
        unsigned int resource7unlock_cmd6 : 1;
        unsigned int resource7unlock_id6 : 3;
        unsigned int resource7unlock_cmd7 : 1;
        unsigned int resource7unlock_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE7_UNLOCK_UNION;
#endif
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd0_START (0)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd0_END (0)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id0_START (1)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id0_END (3)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd1_START (4)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd1_END (4)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id1_START (5)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id1_END (7)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd2_START (8)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd2_END (8)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id2_START (9)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id2_END (11)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd3_START (12)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd3_END (12)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id3_START (13)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id3_END (15)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd4_START (16)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd4_END (16)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id4_START (17)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id4_END (19)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd5_START (20)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd5_END (20)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id5_START (21)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id5_END (23)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd6_START (24)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd6_END (24)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id6_START (25)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id6_END (27)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd7_START (28)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_cmd7_END (28)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id7_START (29)
#define SOC_PCTRL_RESOURCE7_UNLOCK_resource7unlock_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int resource7lock_st0 : 1;
        unsigned int resource7lock_st_id0 : 3;
        unsigned int resource7lock_st1 : 1;
        unsigned int resource7lock_st_id1 : 3;
        unsigned int resource7lock_st2 : 1;
        unsigned int resource7lock_st_id2 : 3;
        unsigned int resource7lock_st3 : 1;
        unsigned int resource7lock_st_id3 : 3;
        unsigned int resource7lock_st4 : 1;
        unsigned int resource7lock_st_id4 : 3;
        unsigned int resource7lock_st5 : 1;
        unsigned int resource7lock_st_id5 : 3;
        unsigned int resource7lock_st6 : 1;
        unsigned int resource7lock_st_id6 : 3;
        unsigned int resource7lock_st7 : 1;
        unsigned int resource7lock_st_id7 : 3;
    } reg;
} SOC_PCTRL_RESOURCE7_LOCK_ST_UNION;
#endif
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st0_START (0)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st0_END (0)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id0_START (1)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id0_END (3)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st1_START (4)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st1_END (4)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id1_START (5)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id1_END (7)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st2_START (8)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st2_END (8)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id2_START (9)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id2_END (11)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st3_START (12)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st3_END (12)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id3_START (13)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id3_END (15)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st4_START (16)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st4_END (16)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id4_START (17)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id4_END (19)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st5_START (20)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st5_END (20)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id5_START (21)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id5_END (23)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st6_START (24)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st6_END (24)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id6_START (25)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id6_END (27)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st7_START (28)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st7_END (28)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id7_START (29)
#define SOC_PCTRL_RESOURCE7_LOCK_ST_resource7lock_st_id7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL5_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL6_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL7_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 6;
        unsigned int reserved_1: 1;
        unsigned int reserved_2: 6;
        unsigned int reserved_3: 6;
        unsigned int reserved_4: 1;
        unsigned int g3d_arqos : 4;
        unsigned int reserved_5: 1;
        unsigned int g3d_awqos : 4;
        unsigned int reserved_6: 3;
    } reg;
} SOC_PCTRL_PERI_CTRL8_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL8_g3d_arqos_START (20)
#define SOC_PCTRL_PERI_CTRL8_g3d_arqos_END (23)
#define SOC_PCTRL_PERI_CTRL8_g3d_awqos_START (25)
#define SOC_PCTRL_PERI_CTRL8_g3d_awqos_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL9_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL10_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL11_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hard_volt_low : 1;
        unsigned int hard_volt_high : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_PCTRL_PERI_CTRL34_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL34_hard_volt_low_START (0)
#define SOC_PCTRL_PERI_CTRL34_hard_volt_low_END (0)
#define SOC_PCTRL_PERI_CTRL34_hard_volt_high_START (1)
#define SOC_PCTRL_PERI_CTRL34_hard_volt_high_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL35_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 6;
        unsigned int reserved_1 : 6;
        unsigned int sc_nsaid_cfg1 : 4;
        unsigned int sc_nsaid_cfg0 : 4;
        unsigned int reserved_2 : 12;
    } reg;
} SOC_PCTRL_PERI_CTRL38_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL38_sc_nsaid_cfg1_START (12)
#define SOC_PCTRL_PERI_CTRL38_sc_nsaid_cfg1_END (15)
#define SOC_PCTRL_PERI_CTRL38_sc_nsaid_cfg0_START (16)
#define SOC_PCTRL_PERI_CTRL38_sc_nsaid_cfg0_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int chan_gran_sw_config : 3;
        unsigned int reserved_0 : 3;
        unsigned int reserved_1 : 6;
        unsigned int reserved_2 : 6;
        unsigned int reserved_3 : 6;
        unsigned int reserved_4 : 6;
        unsigned int reserved_5 : 2;
    } reg;
} SOC_PCTRL_PERI_CTRL39_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL39_chan_gran_sw_config_START (0)
#define SOC_PCTRL_PERI_CTRL39_chan_gran_sw_config_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_ipf_awhint : 4;
        unsigned int sc_ipf_arhint : 4;
        unsigned int reserved : 24;
    } reg;
} SOC_PCTRL_PERI_CTRL40_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL40_sc_ipf_awhint_START (0)
#define SOC_PCTRL_PERI_CTRL40_sc_ipf_awhint_END (3)
#define SOC_PCTRL_PERI_CTRL40_sc_ipf_arhint_START (4)
#define SOC_PCTRL_PERI_CTRL40_sc_ipf_arhint_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int psw_test_mux : 4;
        unsigned int ocldo_test_mux : 3;
        unsigned int reserved : 25;
    } reg;
} SOC_PCTRL_PERI_CTRL41_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL41_psw_test_mux_START (0)
#define SOC_PCTRL_PERI_CTRL41_psw_test_mux_END (3)
#define SOC_PCTRL_PERI_CTRL41_ocldo_test_mux_START (4)
#define SOC_PCTRL_PERI_CTRL41_ocldo_test_mux_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL42_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL43_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int djtag_addr_msb : 4;
        unsigned int reserved : 28;
    } reg;
} SOC_PCTRL_PERI_CTRL44_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL44_djtag_addr_msb_START (0)
#define SOC_PCTRL_PERI_CTRL44_djtag_addr_msb_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL45_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL46_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL47_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL48_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL49_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL50_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL51_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL52_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL53_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL54_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL55_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL56_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL57_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL58_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL59_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL60_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL61_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL62_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL63_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL64_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL65_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL66_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL75_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL76_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL77_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL78_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL79_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc0_save : 1;
        unsigned int sc0_nrestore : 1;
        unsigned int sc1_save : 1;
        unsigned int sc1_nrestore : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_PCTRL_PERI_CTRL80_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL80_sc0_save_START (0)
#define SOC_PCTRL_PERI_CTRL80_sc0_save_END (0)
#define SOC_PCTRL_PERI_CTRL80_sc0_nrestore_START (1)
#define SOC_PCTRL_PERI_CTRL80_sc0_nrestore_END (1)
#define SOC_PCTRL_PERI_CTRL80_sc1_save_START (2)
#define SOC_PCTRL_PERI_CTRL80_sc1_save_END (2)
#define SOC_PCTRL_PERI_CTRL80_sc1_nrestore_START (3)
#define SOC_PCTRL_PERI_CTRL80_sc1_nrestore_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL81_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL82_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL83_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int otp_trim_port : 4;
        unsigned int peri2hisec_otp_trim_port : 9;
        unsigned int reserved : 19;
    } reg;
} SOC_PCTRL_PERI_CTRL84_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL84_otp_trim_port_START (0)
#define SOC_PCTRL_PERI_CTRL84_otp_trim_port_END (3)
#define SOC_PCTRL_PERI_CTRL84_peri2hisec_otp_trim_port_START (4)
#define SOC_PCTRL_PERI_CTRL84_peri2hisec_otp_trim_port_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bootram_auth : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_PCTRL_PERI_CTRL85_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL85_bootram_auth_START (0)
#define SOC_PCTRL_PERI_CTRL85_bootram_auth_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int pctrl_mem_ctrl_sd_eps : 1;
        unsigned int pctrl_rom_ctrl_sd_eps : 1;
        unsigned int peri_ctrl86_cmd : 12;
        unsigned int peri_ctrl86_msk : 16;
    } reg;
} SOC_PCTRL_PERI_CTRL86_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL86_pctrl_mem_ctrl_sd_eps_START (2)
#define SOC_PCTRL_PERI_CTRL86_pctrl_mem_ctrl_sd_eps_END (2)
#define SOC_PCTRL_PERI_CTRL86_pctrl_rom_ctrl_sd_eps_START (3)
#define SOC_PCTRL_PERI_CTRL86_pctrl_rom_ctrl_sd_eps_END (3)
#define SOC_PCTRL_PERI_CTRL86_peri_ctrl86_cmd_START (4)
#define SOC_PCTRL_PERI_CTRL86_peri_ctrl86_cmd_END (15)
#define SOC_PCTRL_PERI_CTRL86_peri_ctrl86_msk_START (16)
#define SOC_PCTRL_PERI_CTRL86_peri_ctrl86_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pctrl2hieps_ctrl : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL87_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL87_pctrl2hieps_ctrl_START (0)
#define SOC_PCTRL_PERI_CTRL87_pctrl2hieps_ctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int por_repeater_out : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_PCTRL_PERI_CTRL94_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL94_por_repeater_out_START (0)
#define SOC_PCTRL_PERI_CTRL94_por_repeater_out_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int software_secure_write_read0 : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL141_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL141_software_secure_write_read0_START (0)
#define SOC_PCTRL_PERI_CTRL141_software_secure_write_read0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int software_secure_write_read1 : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL142_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL142_software_secure_write_read1_START (0)
#define SOC_PCTRL_PERI_CTRL142_software_secure_write_read1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int software_secure_write_read2 : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL143_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL143_software_secure_write_read2_START (0)
#define SOC_PCTRL_PERI_CTRL143_software_secure_write_read2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int software_secure_write_read3 : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL144_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL144_software_secure_write_read3_START (0)
#define SOC_PCTRL_PERI_CTRL144_software_secure_write_read3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int software_secure_write_read4 : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL145_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL145_software_secure_write_read4_START (0)
#define SOC_PCTRL_PERI_CTRL145_software_secure_write_read4_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int software_secure_write_read5 : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL146_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL146_software_secure_write_read5_START (0)
#define SOC_PCTRL_PERI_CTRL146_software_secure_write_read5_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int software_secure_write_read6 : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL147_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL147_software_secure_write_read6_START (0)
#define SOC_PCTRL_PERI_CTRL147_software_secure_write_read6_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int software_secure_write_read7 : 32;
    } reg;
} SOC_PCTRL_PERI_CTRL148_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL148_software_secure_write_read7_START (0)
#define SOC_PCTRL_PERI_CTRL148_software_secure_write_read7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT20_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT21_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT22_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT23_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_PCTRL_PERI_STAT24_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps2pctrl_state_l : 32;
    } reg;
} SOC_PCTRL_PERI_STAT25_UNION;
#endif
#define SOC_PCTRL_PERI_STAT25_hieps2pctrl_state_l_START (0)
#define SOC_PCTRL_PERI_STAT25_hieps2pctrl_state_l_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps2pctrl_state_h : 32;
    } reg;
} SOC_PCTRL_PERI_STAT26_UNION;
#endif
#define SOC_PCTRL_PERI_STAT26_hieps2pctrl_state_h_START (0)
#define SOC_PCTRL_PERI_STAT26_hieps2pctrl_state_h_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps2pctrl_qic : 32;
    } reg;
} SOC_PCTRL_PERI_STAT27_UNION;
#endif
#define SOC_PCTRL_PERI_STAT27_hieps2pctrl_qic_START (0)
#define SOC_PCTRL_PERI_STAT27_hieps2pctrl_qic_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pctrl_mem_stat_sd_eps : 1;
        unsigned int pctrl_retmem_stat_sd_eps : 1;
        unsigned int pctrl_retmem_stat_ds_eps : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_PCTRL_PERI_STAT28_UNION;
#endif
#define SOC_PCTRL_PERI_STAT28_pctrl_mem_stat_sd_eps_START (0)
#define SOC_PCTRL_PERI_STAT28_pctrl_mem_stat_sd_eps_END (0)
#define SOC_PCTRL_PERI_STAT28_pctrl_retmem_stat_sd_eps_START (1)
#define SOC_PCTRL_PERI_STAT28_pctrl_retmem_stat_sd_eps_END (1)
#define SOC_PCTRL_PERI_STAT28_pctrl_retmem_stat_ds_eps_START (2)
#define SOC_PCTRL_PERI_STAT28_pctrl_retmem_stat_ds_eps_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bbic2ap_status : 32;
    } reg;
} SOC_PCTRL_PERI_STAT65_UNION;
#endif
#define SOC_PCTRL_PERI_STAT65_bbic2ap_status_START (0)
#define SOC_PCTRL_PERI_STAT65_bbic2ap_status_END (31)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
