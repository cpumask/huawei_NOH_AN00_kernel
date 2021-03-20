#ifndef __SOC_CRGPERIPH_INTERFACE_H__
#define __SOC_CRGPERIPH_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_CRGPERIPH_PEREN0_ADDR(base) ((base) + (0x000UL))
#define SOC_CRGPERIPH_PERDIS0_ADDR(base) ((base) + (0x004UL))
#define SOC_CRGPERIPH_PERCLKEN0_ADDR(base) ((base) + (0x008UL))
#define SOC_CRGPERIPH_PERSTAT0_ADDR(base) ((base) + (0x00CUL))
#define SOC_CRGPERIPH_PEREN1_ADDR(base) ((base) + (0x010UL))
#define SOC_CRGPERIPH_PERDIS1_ADDR(base) ((base) + (0x014UL))
#define SOC_CRGPERIPH_PERCLKEN1_ADDR(base) ((base) + (0x018UL))
#define SOC_CRGPERIPH_PERSTAT1_ADDR(base) ((base) + (0x01CUL))
#define SOC_CRGPERIPH_PEREN2_ADDR(base) ((base) + (0x020UL))
#define SOC_CRGPERIPH_PERDIS2_ADDR(base) ((base) + (0x024UL))
#define SOC_CRGPERIPH_PERCLKEN2_ADDR(base) ((base) + (0x028UL))
#define SOC_CRGPERIPH_PERSTAT2_ADDR(base) ((base) + (0x02CUL))
#define SOC_CRGPERIPH_PEREN3_ADDR(base) ((base) + (0x030UL))
#define SOC_CRGPERIPH_PERDIS3_ADDR(base) ((base) + (0x034UL))
#define SOC_CRGPERIPH_PERCLKEN3_ADDR(base) ((base) + (0x038UL))
#define SOC_CRGPERIPH_PERSTAT3_ADDR(base) ((base) + (0x03CUL))
#define SOC_CRGPERIPH_PEREN4_ADDR(base) ((base) + (0x040UL))
#define SOC_CRGPERIPH_PERDIS4_ADDR(base) ((base) + (0x044UL))
#define SOC_CRGPERIPH_PERCLKEN4_ADDR(base) ((base) + (0x048UL))
#define SOC_CRGPERIPH_PERSTAT4_ADDR(base) ((base) + (0x04CUL))
#define SOC_CRGPERIPH_PEREN5_ADDR(base) ((base) + (0x050UL))
#define SOC_CRGPERIPH_PERDIS5_ADDR(base) ((base) + (0x054UL))
#define SOC_CRGPERIPH_PERCLKEN5_ADDR(base) ((base) + (0x058UL))
#define SOC_CRGPERIPH_PERSTAT5_ADDR(base) ((base) + (0x05CUL))
#define SOC_CRGPERIPH_PERRSTEN0_ADDR(base) ((base) + (0x060UL))
#define SOC_CRGPERIPH_PERRSTDIS0_ADDR(base) ((base) + (0x064UL))
#define SOC_CRGPERIPH_PERRSTSTAT0_ADDR(base) ((base) + (0x068UL))
#define SOC_CRGPERIPH_PERRSTEN1_ADDR(base) ((base) + (0x06CUL))
#define SOC_CRGPERIPH_PERRSTDIS1_ADDR(base) ((base) + (0x070UL))
#define SOC_CRGPERIPH_PERRSTSTAT1_ADDR(base) ((base) + (0x074UL))
#define SOC_CRGPERIPH_PERRSTEN2_ADDR(base) ((base) + (0x078UL))
#define SOC_CRGPERIPH_PERRSTDIS2_ADDR(base) ((base) + (0x07CUL))
#define SOC_CRGPERIPH_PERRSTSTAT2_ADDR(base) ((base) + (0x080UL))
#define SOC_CRGPERIPH_PERRSTEN3_ADDR(base) ((base) + (0x084UL))
#define SOC_CRGPERIPH_PERRSTDIS3_ADDR(base) ((base) + (0x088UL))
#define SOC_CRGPERIPH_PERRSTSTAT3_ADDR(base) ((base) + (0x08CUL))
#define SOC_CRGPERIPH_PERRSTEN4_ADDR(base) ((base) + (0x090UL))
#define SOC_CRGPERIPH_PERRSTDIS4_ADDR(base) ((base) + (0x094UL))
#define SOC_CRGPERIPH_PERRSTSTAT4_ADDR(base) ((base) + (0x098UL))
#define SOC_CRGPERIPH_PERRSTEN5_ADDR(base) ((base) + (0x09CUL))
#define SOC_CRGPERIPH_PERRSTDIS5_ADDR(base) ((base) + (0x0A0UL))
#define SOC_CRGPERIPH_PERRSTSTAT5_ADDR(base) ((base) + (0x0A4UL))
#define SOC_CRGPERIPH_CLKDIV0_ADDR(base) ((base) + (0x0A8UL))
#define SOC_CRGPERIPH_CLKDIV1_ADDR(base) ((base) + (0x0ACUL))
#define SOC_CRGPERIPH_CLKDIV2_ADDR(base) ((base) + (0x0B0UL))
#define SOC_CRGPERIPH_CLKDIV3_ADDR(base) ((base) + (0x0B4UL))
#define SOC_CRGPERIPH_CLKDIV4_ADDR(base) ((base) + (0x0B8UL))
#define SOC_CRGPERIPH_CLKDIV5_ADDR(base) ((base) + (0x0BCUL))
#define SOC_CRGPERIPH_CLKDIV6_ADDR(base) ((base) + (0x0C0UL))
#define SOC_CRGPERIPH_CLKDIV7_ADDR(base) ((base) + (0x0C4UL))
#define SOC_CRGPERIPH_CLKDIV8_ADDR(base) ((base) + (0x0C8UL))
#define SOC_CRGPERIPH_CLKDIV9_ADDR(base) ((base) + (0x0CCUL))
#define SOC_CRGPERIPH_CLKDIV10_ADDR(base) ((base) + (0x0D0UL))
#define SOC_CRGPERIPH_CLKDIV11_ADDR(base) ((base) + (0x0D4UL))
#define SOC_CRGPERIPH_CLKDIV12_ADDR(base) ((base) + (0x0D8UL))
#define SOC_CRGPERIPH_CLKDIV13_ADDR(base) ((base) + (0x0DCUL))
#define SOC_CRGPERIPH_CLKDIV14_ADDR(base) ((base) + (0x0E0UL))
#define SOC_CRGPERIPH_CLKDIV15_ADDR(base) ((base) + (0x0E4UL))
#define SOC_CRGPERIPH_CLKDIV16_ADDR(base) ((base) + (0x0E8UL))
#define SOC_CRGPERIPH_CLKDIV17_ADDR(base) ((base) + (0x0ECUL))
#define SOC_CRGPERIPH_CLKDIV18_ADDR(base) ((base) + (0x0F0UL))
#define SOC_CRGPERIPH_CLKDIV19_ADDR(base) ((base) + (0x0F4UL))
#define SOC_CRGPERIPH_CLKDIV20_ADDR(base) ((base) + (0x0F8UL))
#define SOC_CRGPERIPH_CLKDIV21_ADDR(base) ((base) + (0x0FCUL))
#define SOC_CRGPERIPH_CLKDIV22_ADDR(base) ((base) + (0x100UL))
#define SOC_CRGPERIPH_CLKDIV23_ADDR(base) ((base) + (0x104UL))
#define SOC_CRGPERIPH_CLKDIV24_ADDR(base) ((base) + (0x108UL))
#define SOC_CRGPERIPH_CLKDIV25_ADDR(base) ((base) + (0x10CUL))
#define SOC_CRGPERIPH_PERI_STAT0_ADDR(base) ((base) + (0x110UL))
#define SOC_CRGPERIPH_PERI_STAT1_ADDR(base) ((base) + (0x114UL))
#define SOC_CRGPERIPH_PERI_STAT2_ADDR(base) ((base) + (0x118UL))
#define SOC_CRGPERIPH_PERI_STAT3_ADDR(base) ((base) + (0x11CUL))
#define SOC_CRGPERIPH_PERI_CTRL0_ADDR(base) ((base) + (0x120UL))
#define SOC_CRGPERIPH_PERI_CTRL1_ADDR(base) ((base) + (0x124UL))
#define SOC_CRGPERIPH_PERI_CTRL3_ADDR(base) ((base) + (0x12CUL))
#define SOC_CRGPERIPH_ISOEN1_ADDR(base) ((base) + (0x130UL))
#define SOC_CRGPERIPH_ISODIS1_ADDR(base) ((base) + (0x134UL))
#define SOC_CRGPERIPH_ISOSTAT1_ADDR(base) ((base) + (0x138UL))
#define SOC_CRGPERIPH_PERTIMECTRL_ADDR(base) ((base) + (0x140UL))
#define SOC_CRGPERIPH_ISOEN_ADDR(base) ((base) + (0x144UL))
#define SOC_CRGPERIPH_ISODIS_ADDR(base) ((base) + (0x148UL))
#define SOC_CRGPERIPH_ISOSTAT_ADDR(base) ((base) + (0x14CUL))
#define SOC_CRGPERIPH_PERPWREN_ADDR(base) ((base) + (0x150UL))
#define SOC_CRGPERIPH_PERPWRDIS_ADDR(base) ((base) + (0x154UL))
#define SOC_CRGPERIPH_PERPWRSTAT_ADDR(base) ((base) + (0x158UL))
#define SOC_CRGPERIPH_PERPWRACK_ADDR(base) ((base) + (0x15CUL))
#define SOC_CRGPERIPH_FCM_CLKEN_ADDR(base) ((base) + (0x160UL))
#define SOC_CRGPERIPH_FCM_RSTEN_ADDR(base) ((base) + (0x164UL))
#define SOC_CRGPERIPH_FCM_RSTDIS_ADDR(base) ((base) + (0x168UL))
#define SOC_CRGPERIPH_FCM_RSTSTAT_ADDR(base) ((base) + (0x16CUL))
#define SOC_CRGPERIPH_FCM0_ADBLPSTAT_ADDR(base) ((base) + (0x174UL))
#define SOC_CRGPERIPH_FCM_DBGPWRDUP_ADDR(base) ((base) + (0x178UL))
#define SOC_CRGPERIPH_A53_OCLDOVSET_ADDR(base) ((base) + (0x184UL))
#define SOC_CRGPERIPH_A53_OCLDO_STAT_ADDR(base) ((base) + (0x198UL))
#define SOC_CRGPERIPH_PERI_CTRL2_L_ADDR(base) ((base) + (0x1A0UL))
#define SOC_CRGPERIPH_PERI_CTRL2_H_ADDR(base) ((base) + (0x1A4UL))
#define SOC_CRGPERIPH_FCM1_ADBLPSTAT_ADDR(base) ((base) + (0x1D4UL))
#define SOC_CRGPERIPH_FCM_PERIPH_ADBLPSTAT_ADDR(base) ((base) + (0x1D8UL))
#define SOC_CRGPERIPH_FCM_ACP_ADBLPSTAT_ADDR(base) ((base) + (0x1DCUL))
#define SOC_CRGPERIPH_MAIA_OCLDOVSET_ADDR(base) ((base) + (0x1E4UL))
#define SOC_CRGPERIPH_FCM_STAT0_ADDR(base) ((base) + (0x1F4UL))
#define SOC_CRGPERIPH_FCM_STAT1_ADDR(base) ((base) + (0x1F8UL))
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_ADDR(base) ((base) + (0x1FCUL))
#define SOC_CRGPERIPH_CORESIGHTLPSTAT_ADDR(base) ((base) + (0x204UL))
#define SOC_CRGPERIPH_CORESIGHT_CTRL_ADDR(base) ((base) + (0x208UL))
#define SOC_CRGPERIPH_CORESIGHTETFINT_ADDR(base) ((base) + (0x20CUL))
#define SOC_CRGPERIPH_CORESIGHTETRINT_ADDR(base) ((base) + (0x210UL))
#define SOC_CRGPERIPH_ADB_GT_STATUS_ADDR(base) ((base) + (0x21CUL))
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_ADDR(base) ((base) + (0x220UL))
#define SOC_CRGPERIPH_G3D_0_ADBLPSTAT_ADDR(base) ((base) + (0x224UL))
#define SOC_CRGPERIPH_G3D_1_ADBLPSTAT_ADDR(base) ((base) + (0x228UL))
#define SOC_CRGPERIPH_G3D_2_ADBLPSTAT_ADDR(base) ((base) + (0x22CUL))
#define SOC_CRGPERIPH_G3D_3_ADBLPSTAT_ADDR(base) ((base) + (0x230UL))
#define SOC_CRGPERIPH_NPU_0_ADBLPSTAT_ADDR(base) ((base) + (0x234UL))
#define SOC_CRGPERIPH_NPU_1_ADBLPSTAT_ADDR(base) ((base) + (0x238UL))
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_ADDR(base) ((base) + (0x23CUL))
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_ADDR(base) ((base) + (0x240UL))
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_ADDR(base) ((base) + (0x244UL))
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_ADDR(base) ((base) + (0x248UL))
#define SOC_CRGPERIPH_NPU_2_ADBLPSTAT_ADDR(base) ((base) + (0x24CUL))
#define SOC_CRGPERIPH_NPU_3_ADBLPSTAT_ADDR(base) ((base) + (0x250UL))
#define SOC_CRGPERIPH_FCM_PDCEN_ADDR(base) ((base) + (0x260UL))
#define SOC_CRGPERIPH_A53_COREPWRINTEN_ADDR(base) ((base) + (0x264UL))
#define SOC_CRGPERIPH_A53_COREPWRINTSTAT_ADDR(base) ((base) + (0x268UL))
#define SOC_CRGPERIPH_A53_COREGICMASK_ADDR(base) ((base) + (0x26CUL))
#define SOC_CRGPERIPH_A53_COREPOWERUP_ADDR(base) ((base) + (0x270UL))
#define SOC_CRGPERIPH_A53_COREPOWERDN_ADDR(base) ((base) + (0x274UL))
#define SOC_CRGPERIPH_A53_COREPOWERSTAT_ADDR(base) ((base) + (0x278UL))
#define SOC_CRGPERIPH_A53_COREPWRUPTIME_ADDR(base) ((base) + (0x27CUL))
#define SOC_CRGPERIPH_A53_COREPWRDNTIME_ADDR(base) ((base) + (0x280UL))
#define SOC_CRGPERIPH_A53_COREISOTIME_ADDR(base) ((base) + (0x284UL))
#define SOC_CRGPERIPH_A53_COREDBGTIME_ADDR(base) ((base) + (0x288UL))
#define SOC_CRGPERIPH_A53_CORERSTTIME_ADDR(base) ((base) + (0x28CUL))
#define SOC_CRGPERIPH_A53_COREURSTTIME_ADDR(base) ((base) + (0x290UL))
#define SOC_CRGPERIPH_A53_COREOCLDOTIME0_ADDR(base) ((base) + (0x294UL))
#define SOC_CRGPERIPH_A53_COREOCLDOTIME1_ADDR(base) ((base) + (0x298UL))
#define SOC_CRGPERIPH_A53_COREOCLDOSTAT_ADDR(base) ((base) + (0x29CUL))
#define SOC_CRGPERIPH_MAIA_COREPWRINTEN_ADDR(base) ((base) + (0x304UL))
#define SOC_CRGPERIPH_MAIA_COREPWRINTSTAT_ADDR(base) ((base) + (0x308UL))
#define SOC_CRGPERIPH_MAIA_COREGICMASK_ADDR(base) ((base) + (0x30CUL))
#define SOC_CRGPERIPH_MAIA_COREPOWERUP_ADDR(base) ((base) + (0x310UL))
#define SOC_CRGPERIPH_MAIA_COREPOWERDN_ADDR(base) ((base) + (0x314UL))
#define SOC_CRGPERIPH_MAIA_COREPOWERSTAT_ADDR(base) ((base) + (0x318UL))
#define SOC_CRGPERIPH_MAIA_COREPWRUPTIME_ADDR(base) ((base) + (0x31CUL))
#define SOC_CRGPERIPH_MAIA_COREPWRDNTIME_ADDR(base) ((base) + (0x320UL))
#define SOC_CRGPERIPH_MAIA_COREISOTIME_ADDR(base) ((base) + (0x324UL))
#define SOC_CRGPERIPH_MAIA_COREDBGTIME_ADDR(base) ((base) + (0x328UL))
#define SOC_CRGPERIPH_MAIA_CORERSTTIME_ADDR(base) ((base) + (0x32CUL))
#define SOC_CRGPERIPH_MAIA_COREURSTTIME_ADDR(base) ((base) + (0x330UL))
#define SOC_CRGPERIPH_MAIA_COREOCLDOTIME0_ADDR(base) ((base) + (0x334UL))
#define SOC_CRGPERIPH_MAIA_COREOCLDOTIME1_ADDR(base) ((base) + (0x338UL))
#define SOC_CRGPERIPH_MAIA_COREOCLDOSTAT_ADDR(base) ((base) + (0x33CUL))
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_ADDR(base) ((base) + (0x340UL))
#define SOC_CRGPERIPH_GPU_LV_CTRL_ADDR(base) ((base) + (0x344UL))
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_ADDR(base) ((base) + (0x354UL))
#define SOC_CRGPERIPH_DMC_PERI_AUTOGT_CTRL_ADDR(base) ((base) + (0x358UL))
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ADDR(base) ((base) + (0x35CUL))
#define SOC_CRGPERIPH_PERI_AUTODIV0_ADDR(base) ((base) + (0x360UL))
#define SOC_CRGPERIPH_PERI_AUTODIV1_ADDR(base) ((base) + (0x364UL))
#define SOC_CRGPERIPH_PERI_AUTODIV2_ADDR(base) ((base) + (0x368UL))
#define SOC_CRGPERIPH_PERI_AUTODIV3_ADDR(base) ((base) + (0x36CUL))
#define SOC_CRGPERIPH_PERI_AUTODIV4_ADDR(base) ((base) + (0x370UL))
#define SOC_CRGPERIPH_PERI_AUTODIV5_ADDR(base) ((base) + (0x374UL))
#define SOC_CRGPERIPH_PERI_AUTODIV6_ADDR(base) ((base) + (0x378UL))
#define SOC_CRGPERIPH_PERI_AUTODIV7_ADDR(base) ((base) + (0x37CUL))
#define SOC_CRGPERIPH_PERI_AUTODIV8_ADDR(base) ((base) + (0x380UL))
#define SOC_CRGPERIPH_PERI_AUTODIV9_ADDR(base) ((base) + (0x384UL))
#define SOC_CRGPERIPH_PERI_AUTODIV10_ADDR(base) ((base) + (0x388UL))
#define SOC_CRGPERIPH_PERI_AUTODIV11_ADDR(base) ((base) + (0x38CUL))
#define SOC_CRGPERIPH_PERI_AUTODIV12_ADDR(base) ((base) + (0x390UL))
#define SOC_CRGPERIPH_PERI_AUTODIV13_ADDR(base) ((base) + (0x394UL))
#define SOC_CRGPERIPH_PERI_AUTODIV14_ADDR(base) ((base) + (0x398UL))
#define SOC_CRGPERIPH_PERI_AUTODIV15_ADDR(base) ((base) + (0x39CUL))
#define SOC_CRGPERIPH_PERI_AUTODIV16_ADDR(base) ((base) + (0x400UL))
#define SOC_CRGPERIPH_PERI_AUTODIV_ACPU_ADDR(base) ((base) + (0x404UL))
#define SOC_CRGPERIPH_PERI_AUTODIV_MCPU_ADDR(base) ((base) + (0x408UL))
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_ADDR(base) ((base) + (0x40CUL))
#define SOC_CRGPERIPH_PEREN6_ADDR(base) ((base) + (0x410UL))
#define SOC_CRGPERIPH_PERDIS6_ADDR(base) ((base) + (0x414UL))
#define SOC_CRGPERIPH_PERCLKEN6_ADDR(base) ((base) + (0x418UL))
#define SOC_CRGPERIPH_PERSTAT6_ADDR(base) ((base) + (0x41CUL))
#define SOC_CRGPERIPH_PEREN7_ADDR(base) ((base) + (0x420UL))
#define SOC_CRGPERIPH_PERDIS7_ADDR(base) ((base) + (0x424UL))
#define SOC_CRGPERIPH_PERCLKEN7_ADDR(base) ((base) + (0x428UL))
#define SOC_CRGPERIPH_PERSTAT7_ADDR(base) ((base) + (0x42CUL))
#define SOC_CRGPERIPH_PEREN8_ADDR(base) ((base) + (0x430UL))
#define SOC_CRGPERIPH_PERDIS8_ADDR(base) ((base) + (0x434UL))
#define SOC_CRGPERIPH_PERCLKEN8_ADDR(base) ((base) + (0x438UL))
#define SOC_CRGPERIPH_PERSTAT8_ADDR(base) ((base) + (0x43CUL))
#define SOC_CRGPERIPH_PEREN9_ADDR(base) ((base) + (0x440UL))
#define SOC_CRGPERIPH_PERDIS9_ADDR(base) ((base) + (0x444UL))
#define SOC_CRGPERIPH_PERCLKEN9_ADDR(base) ((base) + (0x448UL))
#define SOC_CRGPERIPH_PERSTAT9_ADDR(base) ((base) + (0x44CUL))
#define SOC_CRGPERIPH_PEREN10_ADDR(base) ((base) + (0x450UL))
#define SOC_CRGPERIPH_PERDIS10_ADDR(base) ((base) + (0x454UL))
#define SOC_CRGPERIPH_PERCLKEN10_ADDR(base) ((base) + (0x458UL))
#define SOC_CRGPERIPH_PERSTAT10_ADDR(base) ((base) + (0x45CUL))
#define SOC_CRGPERIPH_PEREN11_ADDR(base) ((base) + (0x460UL))
#define SOC_CRGPERIPH_PERDIS11_ADDR(base) ((base) + (0x464UL))
#define SOC_CRGPERIPH_PERCLKEN11_ADDR(base) ((base) + (0x468UL))
#define SOC_CRGPERIPH_PERSTAT11_ADDR(base) ((base) + (0x46CUL))
#define SOC_CRGPERIPH_PEREN12_ADDR(base) ((base) + (0x470UL))
#define SOC_CRGPERIPH_PERDIS12_ADDR(base) ((base) + (0x474UL))
#define SOC_CRGPERIPH_PERCLKEN12_ADDR(base) ((base) + (0x478UL))
#define SOC_CRGPERIPH_PERSTAT12_ADDR(base) ((base) + (0x47CUL))
#define SOC_CRGPERIPH_PEREN13_ADDR(base) ((base) + (0x480UL))
#define SOC_CRGPERIPH_PERDIS13_ADDR(base) ((base) + (0x484UL))
#define SOC_CRGPERIPH_PERCLKEN13_ADDR(base) ((base) + (0x488UL))
#define SOC_CRGPERIPH_PERSTAT13_ADDR(base) ((base) + (0x48CUL))
#define SOC_CRGPERIPH_PERI_STAT4_ADDR(base) ((base) + (0x500UL))
#define SOC_CRGPERIPH_PERI_STAT5_ADDR(base) ((base) + (0x504UL))
#define SOC_CRGPERIPH_PERI_STAT6_ADDR(base) ((base) + (0x508UL))
#define SOC_CRGPERIPH_PERI_STAT7_ADDR(base) ((base) + (0x50CUL))
#define SOC_CRGPERIPH_PERPWRACK1_ADDR(base) ((base) + (0x510UL))
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x520UL))
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x524UL))
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x528UL))
#define SOC_CRGPERIPH_PEREN14_ADDR(base) ((base) + (0x530UL))
#define SOC_CRGPERIPH_PERDIS14_ADDR(base) ((base) + (0x534UL))
#define SOC_CRGPERIPH_PERCLKEN14_ADDR(base) ((base) + (0x538UL))
#define SOC_CRGPERIPH_PERSTAT14_ADDR(base) ((base) + (0x53CUL))
#define SOC_CRGPERIPH_DFT_FUNC_GLOBAL_CLK_EN_ADDR(base) ((base) + (0x5FCUL))
#define SOC_CRGPERIPH_PERI_STAT8_ADDR(base) ((base) + (0x600UL))
#define SOC_CRGPERIPH_PERI_STAT9_ADDR(base) ((base) + (0x604UL))
#define SOC_CRGPERIPH_PERRSTEN_EHC_ADDR(base) ((base) + (0x650UL))
#define SOC_CRGPERIPH_PERRSTDIS_EHC_ADDR(base) ((base) + (0x654UL))
#define SOC_CRGPERIPH_PERRSTSTAT_EHC_ADDR(base) ((base) + (0x658UL))
#define SOC_CRGPERIPH_PERRSTEN6_ADDR(base) ((base) + (0x65CUL))
#define SOC_CRGPERIPH_PERRSTDIS6_ADDR(base) ((base) + (0x660UL))
#define SOC_CRGPERIPH_PERRSTSTAT6_ADDR(base) ((base) + (0x664UL))
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_ADDR(base) ((base) + (0x668UL))
#define SOC_CRGPERIPH_CLKDIV26_ADDR(base) ((base) + (0x700UL))
#define SOC_CRGPERIPH_CLKDIV27_ADDR(base) ((base) + (0x704UL))
#define SOC_CRGPERIPH_CLKDIV28_ADDR(base) ((base) + (0x708UL))
#define SOC_CRGPERIPH_CLKDIV29_ADDR(base) ((base) + (0x70CUL))
#define SOC_CRGPERIPH_CLKDIV30_ADDR(base) ((base) + (0x710UL))
#define SOC_CRGPERIPH_CLKDIV31_ADDR(base) ((base) + (0x714UL))
#define SOC_CRGPERIPH_CLKDIV32_ADDR(base) ((base) + (0x718UL))
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_ADDR(base) ((base) + (0x720UL))
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ADDR(base) ((base) + (0x724UL))
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ADDR(base) ((base) + (0x728UL))
#define SOC_CRGPERIPH_PERI_COMMON_CTRL3_ADDR(base) ((base) + (0x72CUL))
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL0_ADDR(base) ((base) + (0x730UL))
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL1_ADDR(base) ((base) + (0x734UL))
#define SOC_CRGPERIPH_PERI_AVS_CTRL0_ADDR(base) ((base) + (0x740UL))
#define SOC_CRGPERIPH_PERI_AVS_CTRL1_ADDR(base) ((base) + (0x744UL))
#define SOC_CRGPERIPH_PERI_AVS_CTRL2_ADDR(base) ((base) + (0x748UL))
#define SOC_CRGPERIPH_PERI_AVS_CTRL3_ADDR(base) ((base) + (0x74CUL))
#define SOC_CRGPERIPH_PPLL0CTRL0_ADDR(base) ((base) + (0x800UL))
#define SOC_CRGPERIPH_PPLL0CTRL1_ADDR(base) ((base) + (0x804UL))
#define SOC_CRGPERIPH_PPLL1CTRL0_ADDR(base) ((base) + (0x808UL))
#define SOC_CRGPERIPH_PPLL1CTRL1_ADDR(base) ((base) + (0x80CUL))
#define SOC_CRGPERIPH_PPLL2CTRL0_ADDR(base) ((base) + (0x810UL))
#define SOC_CRGPERIPH_PPLL2CTRL1_ADDR(base) ((base) + (0x814UL))
#define SOC_CRGPERIPH_PPLL3CTRL0_ADDR(base) ((base) + (0x818UL))
#define SOC_CRGPERIPH_PPLL3CTRL1_ADDR(base) ((base) + (0x81CUL))
#define SOC_CRGPERIPH_PPLL7CTRL0_ADDR(base) ((base) + (0x838UL))
#define SOC_CRGPERIPH_PPLL7CTRL1_ADDR(base) ((base) + (0x83CUL))
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ADDR(base) ((base) + (0x840UL))
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ADDR(base) ((base) + (0x844UL))
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ADDR(base) ((base) + (0x848UL))
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ADDR(base) ((base) + (0x84CUL))
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ADDR(base) ((base) + (0x85CUL))
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ADDR(base) ((base) + (0x864UL))
#define SOC_CRGPERIPH_PPLL2_B_CTRL1_ADDR(base) ((base) + (0x868UL))
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ADDR(base) ((base) + (0x86CUL))
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_ADDR(base) ((base) + (0x87CUL))
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_ADDR(base) ((base) + (0x880UL))
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_ADDR(base) ((base) + (0x884UL))
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_ADDR(base) ((base) + (0x888UL))
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_ADDR(base) ((base) + (0x88CUL))
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_ADDR(base) ((base) + (0x890UL))
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_ADDR(base) ((base) + (0x894UL))
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_ADDR(base) ((base) + (0x8A0UL))
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_ADDR(base) ((base) + (0x8A4UL))
#define SOC_CRGPERIPH_GPU_ADBLPCTRL_ADDR(base) ((base) + (0x900UL))
#define SOC_CRGPERIPH_CPU_ADBLPCTRL_ADDR(base) ((base) + (0x904UL))
#define SOC_CRGPERIPH_MDM_ADBLPCTRL_ADDR(base) ((base) + (0x908UL))
#define SOC_CRGPERIPH_NPU_ADBLPCTRL_ADDR(base) ((base) + (0x90CUL))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_CLEAR_ADDR(base) ((base) + (0x918UL))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ADDR(base) ((base) + (0x91CUL))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_CTRL_ADDR(base) ((base) + (0x920UL))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ADDR(base) ((base) + (0x924UL))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ADDR(base) ((base) + (0x928UL))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_ADDR(base) ((base) + (0x92CUL))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_ADDR(base) ((base) + (0x930UL))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_ADDR(base) ((base) + (0x934UL))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT3_ADDR(base) ((base) + (0x938UL))
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ADDR(base) ((base) + (0x950UL))
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL0_ADDR(base) ((base) + (0x954UL))
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL0_ADDR(base) ((base) + (0x958UL))
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL0_ADDR(base) ((base) + (0x95CUL))
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_ADDR(base) ((base) + (0x960UL))
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL1_ADDR(base) ((base) + (0x964UL))
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL1_ADDR(base) ((base) + (0x968UL))
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_ADDR(base) ((base) + (0x96CUL))
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL2_ADDR(base) ((base) + (0x970UL))
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL2_ADDR(base) ((base) + (0x974UL))
#define SOC_CRGPERIPH_PERI_FNPLL_STAT_SEL_ADDR(base) ((base) + (0x978UL))
#define SOC_CRGPERIPH_FNPLL_CFG0_ADDR(base) ((base) + (0xa00UL))
#define SOC_CRGPERIPH_FNPLL_CFG1_ADDR(base) ((base) + (0xa04UL))
#define SOC_CRGPERIPH_FNPLL_CFG2_ADDR(base) ((base) + (0xa08UL))
#define SOC_CRGPERIPH_FNPLL_CFG3_ADDR(base) ((base) + (0xa0cUL))
#define SOC_CRGPERIPH_FNPLL_CFG4_ADDR(base) ((base) + (0xa10UL))
#define SOC_CRGPERIPH_FNPLL_CFG5_ADDR(base) ((base) + (0xa14UL))
#define SOC_CRGPERIPH_FNPLL_STATE0_ADDR(base) ((base) + (0xa18UL))
#define SOC_CRGPERIPH_FNPLL_STATE1_ADDR(base) ((base) + (0xa1cUL))
#define SOC_CRGPERIPH_RSTEN_HISEE_SEC_ADDR(base) ((base) + (0xC00UL))
#define SOC_CRGPERIPH_RSTDIS_HISEE_SEC_ADDR(base) ((base) + (0xC04UL))
#define SOC_CRGPERIPH_RSTSTAT_HISEE_SEC_ADDR(base) ((base) + (0xC08UL))
#define SOC_CRGPERIPH_PEREN_HISEE_SEC_ADDR(base) ((base) + (0xC10UL))
#define SOC_CRGPERIPH_PERDIS_HISEE_SEC_ADDR(base) ((base) + (0xC14UL))
#define SOC_CRGPERIPH_PERCLKEN_HISEE_SEC_ADDR(base) ((base) + (0xC18UL))
#define SOC_CRGPERIPH_PERSTAT_HISEE_SEC_ADDR(base) ((base) + (0xC1CUL))
#define SOC_CRGPERIPH_CLKDIV_HISEE_SEC_ADDR(base) ((base) + (0xC30UL))
#define SOC_CRGPERIPH_PEREN_HISEE_FLASH_ADDR(base) ((base) + (0xC80UL))
#define SOC_CRGPERIPH_PERDIS_HISEE_FLASH_ADDR(base) ((base) + (0xC84UL))
#define SOC_CRGPERIPH_PERCLKEN_HISEE_FLASH_ADDR(base) ((base) + (0xC88UL))
#define SOC_CRGPERIPH_PERSTAT_HISEE_FLASH_ADDR(base) ((base) + (0xC8CUL))
#define SOC_CRGPERIPH_CLKDIV0_HISEE_FLASH_ADDR(base) ((base) + (0xC90UL))
#define SOC_CRGPERIPH_CLKDIV1_HISEE_FLASH_ADDR(base) ((base) + (0xC94UL))
#define SOC_CRGPERIPH_CLKDIV2_HISEE_FLASH_ADDR(base) ((base) + (0xC98UL))
#define SOC_CRGPERIPH_MDM_SEC_RSTEN_ADDR(base) ((base) + (0xD00UL))
#define SOC_CRGPERIPH_MDM_SEC_RSTDIS_ADDR(base) ((base) + (0xD04UL))
#define SOC_CRGPERIPH_MDM_SEC_RSTSTAT_ADDR(base) ((base) + (0xD08UL))
#define SOC_CRGPERIPH_GENERAL_SEC_RSTEN_ADDR(base) ((base) + (0xE20UL))
#define SOC_CRGPERIPH_GENERAL_SEC_RSTDIS_ADDR(base) ((base) + (0xE24UL))
#define SOC_CRGPERIPH_GENERAL_SEC_RSTSTAT_ADDR(base) ((base) + (0xE28UL))
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_ADDR(base) ((base) + (0xE2CUL))
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_ADDR(base) ((base) + (0xE30UL))
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_ADDR(base) ((base) + (0xE34UL))
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_ADDR(base) ((base) + (0xE38UL))
#define SOC_CRGPERIPH_SEC_IPCLKRST_BYPASS_ADDR(base) ((base) + (0xE80UL))
#define SOC_CRGPERIPH_GENERAL_SEC_CLKDIV0_ADDR(base) ((base) + (0xE90UL))
#define SOC_CRGPERIPH_GENERAL_SEC_CTRL0_ADDR(base) ((base) + (0xF00UL))
#else
#define SOC_CRGPERIPH_PEREN0_ADDR(base) ((base) + (0x000))
#define SOC_CRGPERIPH_PERDIS0_ADDR(base) ((base) + (0x004))
#define SOC_CRGPERIPH_PERCLKEN0_ADDR(base) ((base) + (0x008))
#define SOC_CRGPERIPH_PERSTAT0_ADDR(base) ((base) + (0x00C))
#define SOC_CRGPERIPH_PEREN1_ADDR(base) ((base) + (0x010))
#define SOC_CRGPERIPH_PERDIS1_ADDR(base) ((base) + (0x014))
#define SOC_CRGPERIPH_PERCLKEN1_ADDR(base) ((base) + (0x018))
#define SOC_CRGPERIPH_PERSTAT1_ADDR(base) ((base) + (0x01C))
#define SOC_CRGPERIPH_PEREN2_ADDR(base) ((base) + (0x020))
#define SOC_CRGPERIPH_PERDIS2_ADDR(base) ((base) + (0x024))
#define SOC_CRGPERIPH_PERCLKEN2_ADDR(base) ((base) + (0x028))
#define SOC_CRGPERIPH_PERSTAT2_ADDR(base) ((base) + (0x02C))
#define SOC_CRGPERIPH_PEREN3_ADDR(base) ((base) + (0x030))
#define SOC_CRGPERIPH_PERDIS3_ADDR(base) ((base) + (0x034))
#define SOC_CRGPERIPH_PERCLKEN3_ADDR(base) ((base) + (0x038))
#define SOC_CRGPERIPH_PERSTAT3_ADDR(base) ((base) + (0x03C))
#define SOC_CRGPERIPH_PEREN4_ADDR(base) ((base) + (0x040))
#define SOC_CRGPERIPH_PERDIS4_ADDR(base) ((base) + (0x044))
#define SOC_CRGPERIPH_PERCLKEN4_ADDR(base) ((base) + (0x048))
#define SOC_CRGPERIPH_PERSTAT4_ADDR(base) ((base) + (0x04C))
#define SOC_CRGPERIPH_PEREN5_ADDR(base) ((base) + (0x050))
#define SOC_CRGPERIPH_PERDIS5_ADDR(base) ((base) + (0x054))
#define SOC_CRGPERIPH_PERCLKEN5_ADDR(base) ((base) + (0x058))
#define SOC_CRGPERIPH_PERSTAT5_ADDR(base) ((base) + (0x05C))
#define SOC_CRGPERIPH_PERRSTEN0_ADDR(base) ((base) + (0x060))
#define SOC_CRGPERIPH_PERRSTDIS0_ADDR(base) ((base) + (0x064))
#define SOC_CRGPERIPH_PERRSTSTAT0_ADDR(base) ((base) + (0x068))
#define SOC_CRGPERIPH_PERRSTEN1_ADDR(base) ((base) + (0x06C))
#define SOC_CRGPERIPH_PERRSTDIS1_ADDR(base) ((base) + (0x070))
#define SOC_CRGPERIPH_PERRSTSTAT1_ADDR(base) ((base) + (0x074))
#define SOC_CRGPERIPH_PERRSTEN2_ADDR(base) ((base) + (0x078))
#define SOC_CRGPERIPH_PERRSTDIS2_ADDR(base) ((base) + (0x07C))
#define SOC_CRGPERIPH_PERRSTSTAT2_ADDR(base) ((base) + (0x080))
#define SOC_CRGPERIPH_PERRSTEN3_ADDR(base) ((base) + (0x084))
#define SOC_CRGPERIPH_PERRSTDIS3_ADDR(base) ((base) + (0x088))
#define SOC_CRGPERIPH_PERRSTSTAT3_ADDR(base) ((base) + (0x08C))
#define SOC_CRGPERIPH_PERRSTEN4_ADDR(base) ((base) + (0x090))
#define SOC_CRGPERIPH_PERRSTDIS4_ADDR(base) ((base) + (0x094))
#define SOC_CRGPERIPH_PERRSTSTAT4_ADDR(base) ((base) + (0x098))
#define SOC_CRGPERIPH_PERRSTEN5_ADDR(base) ((base) + (0x09C))
#define SOC_CRGPERIPH_PERRSTDIS5_ADDR(base) ((base) + (0x0A0))
#define SOC_CRGPERIPH_PERRSTSTAT5_ADDR(base) ((base) + (0x0A4))
#define SOC_CRGPERIPH_CLKDIV0_ADDR(base) ((base) + (0x0A8))
#define SOC_CRGPERIPH_CLKDIV1_ADDR(base) ((base) + (0x0AC))
#define SOC_CRGPERIPH_CLKDIV2_ADDR(base) ((base) + (0x0B0))
#define SOC_CRGPERIPH_CLKDIV3_ADDR(base) ((base) + (0x0B4))
#define SOC_CRGPERIPH_CLKDIV4_ADDR(base) ((base) + (0x0B8))
#define SOC_CRGPERIPH_CLKDIV5_ADDR(base) ((base) + (0x0BC))
#define SOC_CRGPERIPH_CLKDIV6_ADDR(base) ((base) + (0x0C0))
#define SOC_CRGPERIPH_CLKDIV7_ADDR(base) ((base) + (0x0C4))
#define SOC_CRGPERIPH_CLKDIV8_ADDR(base) ((base) + (0x0C8))
#define SOC_CRGPERIPH_CLKDIV9_ADDR(base) ((base) + (0x0CC))
#define SOC_CRGPERIPH_CLKDIV10_ADDR(base) ((base) + (0x0D0))
#define SOC_CRGPERIPH_CLKDIV11_ADDR(base) ((base) + (0x0D4))
#define SOC_CRGPERIPH_CLKDIV12_ADDR(base) ((base) + (0x0D8))
#define SOC_CRGPERIPH_CLKDIV13_ADDR(base) ((base) + (0x0DC))
#define SOC_CRGPERIPH_CLKDIV14_ADDR(base) ((base) + (0x0E0))
#define SOC_CRGPERIPH_CLKDIV15_ADDR(base) ((base) + (0x0E4))
#define SOC_CRGPERIPH_CLKDIV16_ADDR(base) ((base) + (0x0E8))
#define SOC_CRGPERIPH_CLKDIV17_ADDR(base) ((base) + (0x0EC))
#define SOC_CRGPERIPH_CLKDIV18_ADDR(base) ((base) + (0x0F0))
#define SOC_CRGPERIPH_CLKDIV19_ADDR(base) ((base) + (0x0F4))
#define SOC_CRGPERIPH_CLKDIV20_ADDR(base) ((base) + (0x0F8))
#define SOC_CRGPERIPH_CLKDIV21_ADDR(base) ((base) + (0x0FC))
#define SOC_CRGPERIPH_CLKDIV22_ADDR(base) ((base) + (0x100))
#define SOC_CRGPERIPH_CLKDIV23_ADDR(base) ((base) + (0x104))
#define SOC_CRGPERIPH_CLKDIV24_ADDR(base) ((base) + (0x108))
#define SOC_CRGPERIPH_CLKDIV25_ADDR(base) ((base) + (0x10C))
#define SOC_CRGPERIPH_PERI_STAT0_ADDR(base) ((base) + (0x110))
#define SOC_CRGPERIPH_PERI_STAT1_ADDR(base) ((base) + (0x114))
#define SOC_CRGPERIPH_PERI_STAT2_ADDR(base) ((base) + (0x118))
#define SOC_CRGPERIPH_PERI_STAT3_ADDR(base) ((base) + (0x11C))
#define SOC_CRGPERIPH_PERI_CTRL0_ADDR(base) ((base) + (0x120))
#define SOC_CRGPERIPH_PERI_CTRL1_ADDR(base) ((base) + (0x124))
#define SOC_CRGPERIPH_PERI_CTRL3_ADDR(base) ((base) + (0x12C))
#define SOC_CRGPERIPH_ISOEN1_ADDR(base) ((base) + (0x130))
#define SOC_CRGPERIPH_ISODIS1_ADDR(base) ((base) + (0x134))
#define SOC_CRGPERIPH_ISOSTAT1_ADDR(base) ((base) + (0x138))
#define SOC_CRGPERIPH_PERTIMECTRL_ADDR(base) ((base) + (0x140))
#define SOC_CRGPERIPH_ISOEN_ADDR(base) ((base) + (0x144))
#define SOC_CRGPERIPH_ISODIS_ADDR(base) ((base) + (0x148))
#define SOC_CRGPERIPH_ISOSTAT_ADDR(base) ((base) + (0x14C))
#define SOC_CRGPERIPH_PERPWREN_ADDR(base) ((base) + (0x150))
#define SOC_CRGPERIPH_PERPWRDIS_ADDR(base) ((base) + (0x154))
#define SOC_CRGPERIPH_PERPWRSTAT_ADDR(base) ((base) + (0x158))
#define SOC_CRGPERIPH_PERPWRACK_ADDR(base) ((base) + (0x15C))
#define SOC_CRGPERIPH_FCM_CLKEN_ADDR(base) ((base) + (0x160))
#define SOC_CRGPERIPH_FCM_RSTEN_ADDR(base) ((base) + (0x164))
#define SOC_CRGPERIPH_FCM_RSTDIS_ADDR(base) ((base) + (0x168))
#define SOC_CRGPERIPH_FCM_RSTSTAT_ADDR(base) ((base) + (0x16C))
#define SOC_CRGPERIPH_FCM0_ADBLPSTAT_ADDR(base) ((base) + (0x174))
#define SOC_CRGPERIPH_FCM_DBGPWRDUP_ADDR(base) ((base) + (0x178))
#define SOC_CRGPERIPH_A53_OCLDOVSET_ADDR(base) ((base) + (0x184))
#define SOC_CRGPERIPH_A53_OCLDO_STAT_ADDR(base) ((base) + (0x198))
#define SOC_CRGPERIPH_PERI_CTRL2_L_ADDR(base) ((base) + (0x1A0))
#define SOC_CRGPERIPH_PERI_CTRL2_H_ADDR(base) ((base) + (0x1A4))
#define SOC_CRGPERIPH_FCM1_ADBLPSTAT_ADDR(base) ((base) + (0x1D4))
#define SOC_CRGPERIPH_FCM_PERIPH_ADBLPSTAT_ADDR(base) ((base) + (0x1D8))
#define SOC_CRGPERIPH_FCM_ACP_ADBLPSTAT_ADDR(base) ((base) + (0x1DC))
#define SOC_CRGPERIPH_MAIA_OCLDOVSET_ADDR(base) ((base) + (0x1E4))
#define SOC_CRGPERIPH_FCM_STAT0_ADDR(base) ((base) + (0x1F4))
#define SOC_CRGPERIPH_FCM_STAT1_ADDR(base) ((base) + (0x1F8))
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_ADDR(base) ((base) + (0x1FC))
#define SOC_CRGPERIPH_CORESIGHTLPSTAT_ADDR(base) ((base) + (0x204))
#define SOC_CRGPERIPH_CORESIGHT_CTRL_ADDR(base) ((base) + (0x208))
#define SOC_CRGPERIPH_CORESIGHTETFINT_ADDR(base) ((base) + (0x20C))
#define SOC_CRGPERIPH_CORESIGHTETRINT_ADDR(base) ((base) + (0x210))
#define SOC_CRGPERIPH_ADB_GT_STATUS_ADDR(base) ((base) + (0x21C))
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_ADDR(base) ((base) + (0x220))
#define SOC_CRGPERIPH_G3D_0_ADBLPSTAT_ADDR(base) ((base) + (0x224))
#define SOC_CRGPERIPH_G3D_1_ADBLPSTAT_ADDR(base) ((base) + (0x228))
#define SOC_CRGPERIPH_G3D_2_ADBLPSTAT_ADDR(base) ((base) + (0x22C))
#define SOC_CRGPERIPH_G3D_3_ADBLPSTAT_ADDR(base) ((base) + (0x230))
#define SOC_CRGPERIPH_NPU_0_ADBLPSTAT_ADDR(base) ((base) + (0x234))
#define SOC_CRGPERIPH_NPU_1_ADBLPSTAT_ADDR(base) ((base) + (0x238))
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_ADDR(base) ((base) + (0x23C))
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_ADDR(base) ((base) + (0x240))
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_ADDR(base) ((base) + (0x244))
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_ADDR(base) ((base) + (0x248))
#define SOC_CRGPERIPH_NPU_2_ADBLPSTAT_ADDR(base) ((base) + (0x24C))
#define SOC_CRGPERIPH_NPU_3_ADBLPSTAT_ADDR(base) ((base) + (0x250))
#define SOC_CRGPERIPH_FCM_PDCEN_ADDR(base) ((base) + (0x260))
#define SOC_CRGPERIPH_A53_COREPWRINTEN_ADDR(base) ((base) + (0x264))
#define SOC_CRGPERIPH_A53_COREPWRINTSTAT_ADDR(base) ((base) + (0x268))
#define SOC_CRGPERIPH_A53_COREGICMASK_ADDR(base) ((base) + (0x26C))
#define SOC_CRGPERIPH_A53_COREPOWERUP_ADDR(base) ((base) + (0x270))
#define SOC_CRGPERIPH_A53_COREPOWERDN_ADDR(base) ((base) + (0x274))
#define SOC_CRGPERIPH_A53_COREPOWERSTAT_ADDR(base) ((base) + (0x278))
#define SOC_CRGPERIPH_A53_COREPWRUPTIME_ADDR(base) ((base) + (0x27C))
#define SOC_CRGPERIPH_A53_COREPWRDNTIME_ADDR(base) ((base) + (0x280))
#define SOC_CRGPERIPH_A53_COREISOTIME_ADDR(base) ((base) + (0x284))
#define SOC_CRGPERIPH_A53_COREDBGTIME_ADDR(base) ((base) + (0x288))
#define SOC_CRGPERIPH_A53_CORERSTTIME_ADDR(base) ((base) + (0x28C))
#define SOC_CRGPERIPH_A53_COREURSTTIME_ADDR(base) ((base) + (0x290))
#define SOC_CRGPERIPH_A53_COREOCLDOTIME0_ADDR(base) ((base) + (0x294))
#define SOC_CRGPERIPH_A53_COREOCLDOTIME1_ADDR(base) ((base) + (0x298))
#define SOC_CRGPERIPH_A53_COREOCLDOSTAT_ADDR(base) ((base) + (0x29C))
#define SOC_CRGPERIPH_MAIA_COREPWRINTEN_ADDR(base) ((base) + (0x304))
#define SOC_CRGPERIPH_MAIA_COREPWRINTSTAT_ADDR(base) ((base) + (0x308))
#define SOC_CRGPERIPH_MAIA_COREGICMASK_ADDR(base) ((base) + (0x30C))
#define SOC_CRGPERIPH_MAIA_COREPOWERUP_ADDR(base) ((base) + (0x310))
#define SOC_CRGPERIPH_MAIA_COREPOWERDN_ADDR(base) ((base) + (0x314))
#define SOC_CRGPERIPH_MAIA_COREPOWERSTAT_ADDR(base) ((base) + (0x318))
#define SOC_CRGPERIPH_MAIA_COREPWRUPTIME_ADDR(base) ((base) + (0x31C))
#define SOC_CRGPERIPH_MAIA_COREPWRDNTIME_ADDR(base) ((base) + (0x320))
#define SOC_CRGPERIPH_MAIA_COREISOTIME_ADDR(base) ((base) + (0x324))
#define SOC_CRGPERIPH_MAIA_COREDBGTIME_ADDR(base) ((base) + (0x328))
#define SOC_CRGPERIPH_MAIA_CORERSTTIME_ADDR(base) ((base) + (0x32C))
#define SOC_CRGPERIPH_MAIA_COREURSTTIME_ADDR(base) ((base) + (0x330))
#define SOC_CRGPERIPH_MAIA_COREOCLDOTIME0_ADDR(base) ((base) + (0x334))
#define SOC_CRGPERIPH_MAIA_COREOCLDOTIME1_ADDR(base) ((base) + (0x338))
#define SOC_CRGPERIPH_MAIA_COREOCLDOSTAT_ADDR(base) ((base) + (0x33C))
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_ADDR(base) ((base) + (0x340))
#define SOC_CRGPERIPH_GPU_LV_CTRL_ADDR(base) ((base) + (0x344))
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_ADDR(base) ((base) + (0x354))
#define SOC_CRGPERIPH_DMC_PERI_AUTOGT_CTRL_ADDR(base) ((base) + (0x358))
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ADDR(base) ((base) + (0x35C))
#define SOC_CRGPERIPH_PERI_AUTODIV0_ADDR(base) ((base) + (0x360))
#define SOC_CRGPERIPH_PERI_AUTODIV1_ADDR(base) ((base) + (0x364))
#define SOC_CRGPERIPH_PERI_AUTODIV2_ADDR(base) ((base) + (0x368))
#define SOC_CRGPERIPH_PERI_AUTODIV3_ADDR(base) ((base) + (0x36C))
#define SOC_CRGPERIPH_PERI_AUTODIV4_ADDR(base) ((base) + (0x370))
#define SOC_CRGPERIPH_PERI_AUTODIV5_ADDR(base) ((base) + (0x374))
#define SOC_CRGPERIPH_PERI_AUTODIV6_ADDR(base) ((base) + (0x378))
#define SOC_CRGPERIPH_PERI_AUTODIV7_ADDR(base) ((base) + (0x37C))
#define SOC_CRGPERIPH_PERI_AUTODIV8_ADDR(base) ((base) + (0x380))
#define SOC_CRGPERIPH_PERI_AUTODIV9_ADDR(base) ((base) + (0x384))
#define SOC_CRGPERIPH_PERI_AUTODIV10_ADDR(base) ((base) + (0x388))
#define SOC_CRGPERIPH_PERI_AUTODIV11_ADDR(base) ((base) + (0x38C))
#define SOC_CRGPERIPH_PERI_AUTODIV12_ADDR(base) ((base) + (0x390))
#define SOC_CRGPERIPH_PERI_AUTODIV13_ADDR(base) ((base) + (0x394))
#define SOC_CRGPERIPH_PERI_AUTODIV14_ADDR(base) ((base) + (0x398))
#define SOC_CRGPERIPH_PERI_AUTODIV15_ADDR(base) ((base) + (0x39C))
#define SOC_CRGPERIPH_PERI_AUTODIV16_ADDR(base) ((base) + (0x400))
#define SOC_CRGPERIPH_PERI_AUTODIV_ACPU_ADDR(base) ((base) + (0x404))
#define SOC_CRGPERIPH_PERI_AUTODIV_MCPU_ADDR(base) ((base) + (0x408))
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_ADDR(base) ((base) + (0x40C))
#define SOC_CRGPERIPH_PEREN6_ADDR(base) ((base) + (0x410))
#define SOC_CRGPERIPH_PERDIS6_ADDR(base) ((base) + (0x414))
#define SOC_CRGPERIPH_PERCLKEN6_ADDR(base) ((base) + (0x418))
#define SOC_CRGPERIPH_PERSTAT6_ADDR(base) ((base) + (0x41C))
#define SOC_CRGPERIPH_PEREN7_ADDR(base) ((base) + (0x420))
#define SOC_CRGPERIPH_PERDIS7_ADDR(base) ((base) + (0x424))
#define SOC_CRGPERIPH_PERCLKEN7_ADDR(base) ((base) + (0x428))
#define SOC_CRGPERIPH_PERSTAT7_ADDR(base) ((base) + (0x42C))
#define SOC_CRGPERIPH_PEREN8_ADDR(base) ((base) + (0x430))
#define SOC_CRGPERIPH_PERDIS8_ADDR(base) ((base) + (0x434))
#define SOC_CRGPERIPH_PERCLKEN8_ADDR(base) ((base) + (0x438))
#define SOC_CRGPERIPH_PERSTAT8_ADDR(base) ((base) + (0x43C))
#define SOC_CRGPERIPH_PEREN9_ADDR(base) ((base) + (0x440))
#define SOC_CRGPERIPH_PERDIS9_ADDR(base) ((base) + (0x444))
#define SOC_CRGPERIPH_PERCLKEN9_ADDR(base) ((base) + (0x448))
#define SOC_CRGPERIPH_PERSTAT9_ADDR(base) ((base) + (0x44C))
#define SOC_CRGPERIPH_PEREN10_ADDR(base) ((base) + (0x450))
#define SOC_CRGPERIPH_PERDIS10_ADDR(base) ((base) + (0x454))
#define SOC_CRGPERIPH_PERCLKEN10_ADDR(base) ((base) + (0x458))
#define SOC_CRGPERIPH_PERSTAT10_ADDR(base) ((base) + (0x45C))
#define SOC_CRGPERIPH_PEREN11_ADDR(base) ((base) + (0x460))
#define SOC_CRGPERIPH_PERDIS11_ADDR(base) ((base) + (0x464))
#define SOC_CRGPERIPH_PERCLKEN11_ADDR(base) ((base) + (0x468))
#define SOC_CRGPERIPH_PERSTAT11_ADDR(base) ((base) + (0x46C))
#define SOC_CRGPERIPH_PEREN12_ADDR(base) ((base) + (0x470))
#define SOC_CRGPERIPH_PERDIS12_ADDR(base) ((base) + (0x474))
#define SOC_CRGPERIPH_PERCLKEN12_ADDR(base) ((base) + (0x478))
#define SOC_CRGPERIPH_PERSTAT12_ADDR(base) ((base) + (0x47C))
#define SOC_CRGPERIPH_PEREN13_ADDR(base) ((base) + (0x480))
#define SOC_CRGPERIPH_PERDIS13_ADDR(base) ((base) + (0x484))
#define SOC_CRGPERIPH_PERCLKEN13_ADDR(base) ((base) + (0x488))
#define SOC_CRGPERIPH_PERSTAT13_ADDR(base) ((base) + (0x48C))
#define SOC_CRGPERIPH_PERI_STAT4_ADDR(base) ((base) + (0x500))
#define SOC_CRGPERIPH_PERI_STAT5_ADDR(base) ((base) + (0x504))
#define SOC_CRGPERIPH_PERI_STAT6_ADDR(base) ((base) + (0x508))
#define SOC_CRGPERIPH_PERI_STAT7_ADDR(base) ((base) + (0x50C))
#define SOC_CRGPERIPH_PERPWRACK1_ADDR(base) ((base) + (0x510))
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x520))
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x524))
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x528))
#define SOC_CRGPERIPH_PEREN14_ADDR(base) ((base) + (0x530))
#define SOC_CRGPERIPH_PERDIS14_ADDR(base) ((base) + (0x534))
#define SOC_CRGPERIPH_PERCLKEN14_ADDR(base) ((base) + (0x538))
#define SOC_CRGPERIPH_PERSTAT14_ADDR(base) ((base) + (0x53C))
#define SOC_CRGPERIPH_DFT_FUNC_GLOBAL_CLK_EN_ADDR(base) ((base) + (0x5FC))
#define SOC_CRGPERIPH_PERI_STAT8_ADDR(base) ((base) + (0x600))
#define SOC_CRGPERIPH_PERI_STAT9_ADDR(base) ((base) + (0x604))
#define SOC_CRGPERIPH_PERRSTEN_EHC_ADDR(base) ((base) + (0x650))
#define SOC_CRGPERIPH_PERRSTDIS_EHC_ADDR(base) ((base) + (0x654))
#define SOC_CRGPERIPH_PERRSTSTAT_EHC_ADDR(base) ((base) + (0x658))
#define SOC_CRGPERIPH_PERRSTEN6_ADDR(base) ((base) + (0x65C))
#define SOC_CRGPERIPH_PERRSTDIS6_ADDR(base) ((base) + (0x660))
#define SOC_CRGPERIPH_PERRSTSTAT6_ADDR(base) ((base) + (0x664))
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_ADDR(base) ((base) + (0x668))
#define SOC_CRGPERIPH_CLKDIV26_ADDR(base) ((base) + (0x700))
#define SOC_CRGPERIPH_CLKDIV27_ADDR(base) ((base) + (0x704))
#define SOC_CRGPERIPH_CLKDIV28_ADDR(base) ((base) + (0x708))
#define SOC_CRGPERIPH_CLKDIV29_ADDR(base) ((base) + (0x70C))
#define SOC_CRGPERIPH_CLKDIV30_ADDR(base) ((base) + (0x710))
#define SOC_CRGPERIPH_CLKDIV31_ADDR(base) ((base) + (0x714))
#define SOC_CRGPERIPH_CLKDIV32_ADDR(base) ((base) + (0x718))
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_ADDR(base) ((base) + (0x720))
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ADDR(base) ((base) + (0x724))
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ADDR(base) ((base) + (0x728))
#define SOC_CRGPERIPH_PERI_COMMON_CTRL3_ADDR(base) ((base) + (0x72C))
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL0_ADDR(base) ((base) + (0x730))
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL1_ADDR(base) ((base) + (0x734))
#define SOC_CRGPERIPH_PERI_AVS_CTRL0_ADDR(base) ((base) + (0x740))
#define SOC_CRGPERIPH_PERI_AVS_CTRL1_ADDR(base) ((base) + (0x744))
#define SOC_CRGPERIPH_PERI_AVS_CTRL2_ADDR(base) ((base) + (0x748))
#define SOC_CRGPERIPH_PERI_AVS_CTRL3_ADDR(base) ((base) + (0x74C))
#define SOC_CRGPERIPH_PPLL0CTRL0_ADDR(base) ((base) + (0x800))
#define SOC_CRGPERIPH_PPLL0CTRL1_ADDR(base) ((base) + (0x804))
#define SOC_CRGPERIPH_PPLL1CTRL0_ADDR(base) ((base) + (0x808))
#define SOC_CRGPERIPH_PPLL1CTRL1_ADDR(base) ((base) + (0x80C))
#define SOC_CRGPERIPH_PPLL2CTRL0_ADDR(base) ((base) + (0x810))
#define SOC_CRGPERIPH_PPLL2CTRL1_ADDR(base) ((base) + (0x814))
#define SOC_CRGPERIPH_PPLL3CTRL0_ADDR(base) ((base) + (0x818))
#define SOC_CRGPERIPH_PPLL3CTRL1_ADDR(base) ((base) + (0x81C))
#define SOC_CRGPERIPH_PPLL7CTRL0_ADDR(base) ((base) + (0x838))
#define SOC_CRGPERIPH_PPLL7CTRL1_ADDR(base) ((base) + (0x83C))
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ADDR(base) ((base) + (0x840))
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ADDR(base) ((base) + (0x844))
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ADDR(base) ((base) + (0x848))
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ADDR(base) ((base) + (0x84C))
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ADDR(base) ((base) + (0x85C))
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ADDR(base) ((base) + (0x864))
#define SOC_CRGPERIPH_PPLL2_B_CTRL1_ADDR(base) ((base) + (0x868))
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ADDR(base) ((base) + (0x86C))
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_ADDR(base) ((base) + (0x87C))
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_ADDR(base) ((base) + (0x880))
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_ADDR(base) ((base) + (0x884))
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_ADDR(base) ((base) + (0x888))
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_ADDR(base) ((base) + (0x88C))
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_ADDR(base) ((base) + (0x890))
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_ADDR(base) ((base) + (0x894))
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_ADDR(base) ((base) + (0x8A0))
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_ADDR(base) ((base) + (0x8A4))
#define SOC_CRGPERIPH_GPU_ADBLPCTRL_ADDR(base) ((base) + (0x900))
#define SOC_CRGPERIPH_CPU_ADBLPCTRL_ADDR(base) ((base) + (0x904))
#define SOC_CRGPERIPH_MDM_ADBLPCTRL_ADDR(base) ((base) + (0x908))
#define SOC_CRGPERIPH_NPU_ADBLPCTRL_ADDR(base) ((base) + (0x90C))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_CLEAR_ADDR(base) ((base) + (0x918))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ADDR(base) ((base) + (0x91C))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_CTRL_ADDR(base) ((base) + (0x920))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ADDR(base) ((base) + (0x924))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ADDR(base) ((base) + (0x928))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_ADDR(base) ((base) + (0x92C))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_ADDR(base) ((base) + (0x930))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_ADDR(base) ((base) + (0x934))
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT3_ADDR(base) ((base) + (0x938))
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ADDR(base) ((base) + (0x950))
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL0_ADDR(base) ((base) + (0x954))
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL0_ADDR(base) ((base) + (0x958))
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL0_ADDR(base) ((base) + (0x95C))
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_ADDR(base) ((base) + (0x960))
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL1_ADDR(base) ((base) + (0x964))
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL1_ADDR(base) ((base) + (0x968))
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_ADDR(base) ((base) + (0x96C))
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL2_ADDR(base) ((base) + (0x970))
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL2_ADDR(base) ((base) + (0x974))
#define SOC_CRGPERIPH_PERI_FNPLL_STAT_SEL_ADDR(base) ((base) + (0x978))
#define SOC_CRGPERIPH_FNPLL_CFG0_ADDR(base) ((base) + (0xa00))
#define SOC_CRGPERIPH_FNPLL_CFG1_ADDR(base) ((base) + (0xa04))
#define SOC_CRGPERIPH_FNPLL_CFG2_ADDR(base) ((base) + (0xa08))
#define SOC_CRGPERIPH_FNPLL_CFG3_ADDR(base) ((base) + (0xa0c))
#define SOC_CRGPERIPH_FNPLL_CFG4_ADDR(base) ((base) + (0xa10))
#define SOC_CRGPERIPH_FNPLL_CFG5_ADDR(base) ((base) + (0xa14))
#define SOC_CRGPERIPH_FNPLL_STATE0_ADDR(base) ((base) + (0xa18))
#define SOC_CRGPERIPH_FNPLL_STATE1_ADDR(base) ((base) + (0xa1c))
#define SOC_CRGPERIPH_RSTEN_HISEE_SEC_ADDR(base) ((base) + (0xC00))
#define SOC_CRGPERIPH_RSTDIS_HISEE_SEC_ADDR(base) ((base) + (0xC04))
#define SOC_CRGPERIPH_RSTSTAT_HISEE_SEC_ADDR(base) ((base) + (0xC08))
#define SOC_CRGPERIPH_PEREN_HISEE_SEC_ADDR(base) ((base) + (0xC10))
#define SOC_CRGPERIPH_PERDIS_HISEE_SEC_ADDR(base) ((base) + (0xC14))
#define SOC_CRGPERIPH_PERCLKEN_HISEE_SEC_ADDR(base) ((base) + (0xC18))
#define SOC_CRGPERIPH_PERSTAT_HISEE_SEC_ADDR(base) ((base) + (0xC1C))
#define SOC_CRGPERIPH_CLKDIV_HISEE_SEC_ADDR(base) ((base) + (0xC30))
#define SOC_CRGPERIPH_PEREN_HISEE_FLASH_ADDR(base) ((base) + (0xC80))
#define SOC_CRGPERIPH_PERDIS_HISEE_FLASH_ADDR(base) ((base) + (0xC84))
#define SOC_CRGPERIPH_PERCLKEN_HISEE_FLASH_ADDR(base) ((base) + (0xC88))
#define SOC_CRGPERIPH_PERSTAT_HISEE_FLASH_ADDR(base) ((base) + (0xC8C))
#define SOC_CRGPERIPH_CLKDIV0_HISEE_FLASH_ADDR(base) ((base) + (0xC90))
#define SOC_CRGPERIPH_CLKDIV1_HISEE_FLASH_ADDR(base) ((base) + (0xC94))
#define SOC_CRGPERIPH_CLKDIV2_HISEE_FLASH_ADDR(base) ((base) + (0xC98))
#define SOC_CRGPERIPH_MDM_SEC_RSTEN_ADDR(base) ((base) + (0xD00))
#define SOC_CRGPERIPH_MDM_SEC_RSTDIS_ADDR(base) ((base) + (0xD04))
#define SOC_CRGPERIPH_MDM_SEC_RSTSTAT_ADDR(base) ((base) + (0xD08))
#define SOC_CRGPERIPH_GENERAL_SEC_RSTEN_ADDR(base) ((base) + (0xE20))
#define SOC_CRGPERIPH_GENERAL_SEC_RSTDIS_ADDR(base) ((base) + (0xE24))
#define SOC_CRGPERIPH_GENERAL_SEC_RSTSTAT_ADDR(base) ((base) + (0xE28))
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_ADDR(base) ((base) + (0xE2C))
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_ADDR(base) ((base) + (0xE30))
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_ADDR(base) ((base) + (0xE34))
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_ADDR(base) ((base) + (0xE38))
#define SOC_CRGPERIPH_SEC_IPCLKRST_BYPASS_ADDR(base) ((base) + (0xE80))
#define SOC_CRGPERIPH_GENERAL_SEC_CLKDIV0_ADDR(base) ((base) + (0xE90))
#define SOC_CRGPERIPH_GENERAL_SEC_CTRL0_ADDR(base) ((base) + (0xF00))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_pclk_hsdt_crg : 1;
        unsigned int gt_clk_hsdtbus : 1;
        unsigned int gt_aclk_ddrc : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_pclk_hsdt1_crg : 1;
        unsigned int gt_aclk_dmss2dmc_mux : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_clk_ipf : 1;
        unsigned int gt_clk_tiny_core_peri : 1;
        unsigned int gt_clk_sysbus : 1;
        unsigned int gt_clk_cfgbus : 1;
        unsigned int gt_clk_sys2cfgbus : 1;
        unsigned int gt_clk_noc_cfg2fcm : 1;
        unsigned int gt_clk_fcm_nic_m : 1;
        unsigned int gt_clk_hsdt1_subsys : 1;
        unsigned int gt_clk_sysbus_r : 1;
        unsigned int gt_clk_sysbus_l : 1;
        unsigned int gt_clk_vcodec2cfgbus : 1;
        unsigned int gt_clk_sysbus2hsdt1 : 1;
        unsigned int gt_aclk_dmss_free : 1;
        unsigned int gt_clk_lpmcu2cfgbus : 1;
        unsigned int gt_clk_hsdt1bus : 1;
        unsigned int gt_clk_dmabus : 1;
        unsigned int gt_clk_dma2sysbus : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_clk_adb_mst_npu : 1;
        unsigned int gt_clk_ao_asp : 1;
        unsigned int gt_clk_npu_tcxo : 1;
        unsigned int gt_clk_npu_gic : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int gt_clk_noise_print : 1;
    } reg;
} SOC_CRGPERIPH_PEREN0_UNION;
#endif
#define SOC_CRGPERIPH_PEREN0_gt_pclk_hsdt_crg_START (0)
#define SOC_CRGPERIPH_PEREN0_gt_pclk_hsdt_crg_END (0)
#define SOC_CRGPERIPH_PEREN0_gt_clk_hsdtbus_START (1)
#define SOC_CRGPERIPH_PEREN0_gt_clk_hsdtbus_END (1)
#define SOC_CRGPERIPH_PEREN0_gt_aclk_ddrc_START (2)
#define SOC_CRGPERIPH_PEREN0_gt_aclk_ddrc_END (2)
#define SOC_CRGPERIPH_PEREN0_gt_pclk_hsdt1_crg_START (4)
#define SOC_CRGPERIPH_PEREN0_gt_pclk_hsdt1_crg_END (4)
#define SOC_CRGPERIPH_PEREN0_gt_aclk_dmss2dmc_mux_START (5)
#define SOC_CRGPERIPH_PEREN0_gt_aclk_dmss2dmc_mux_END (5)
#define SOC_CRGPERIPH_PEREN0_gt_clk_ipf_START (7)
#define SOC_CRGPERIPH_PEREN0_gt_clk_ipf_END (7)
#define SOC_CRGPERIPH_PEREN0_gt_clk_tiny_core_peri_START (8)
#define SOC_CRGPERIPH_PEREN0_gt_clk_tiny_core_peri_END (8)
#define SOC_CRGPERIPH_PEREN0_gt_clk_sysbus_START (9)
#define SOC_CRGPERIPH_PEREN0_gt_clk_sysbus_END (9)
#define SOC_CRGPERIPH_PEREN0_gt_clk_cfgbus_START (10)
#define SOC_CRGPERIPH_PEREN0_gt_clk_cfgbus_END (10)
#define SOC_CRGPERIPH_PEREN0_gt_clk_sys2cfgbus_START (11)
#define SOC_CRGPERIPH_PEREN0_gt_clk_sys2cfgbus_END (11)
#define SOC_CRGPERIPH_PEREN0_gt_clk_noc_cfg2fcm_START (12)
#define SOC_CRGPERIPH_PEREN0_gt_clk_noc_cfg2fcm_END (12)
#define SOC_CRGPERIPH_PEREN0_gt_clk_fcm_nic_m_START (13)
#define SOC_CRGPERIPH_PEREN0_gt_clk_fcm_nic_m_END (13)
#define SOC_CRGPERIPH_PEREN0_gt_clk_hsdt1_subsys_START (14)
#define SOC_CRGPERIPH_PEREN0_gt_clk_hsdt1_subsys_END (14)
#define SOC_CRGPERIPH_PEREN0_gt_clk_sysbus_r_START (15)
#define SOC_CRGPERIPH_PEREN0_gt_clk_sysbus_r_END (15)
#define SOC_CRGPERIPH_PEREN0_gt_clk_sysbus_l_START (16)
#define SOC_CRGPERIPH_PEREN0_gt_clk_sysbus_l_END (16)
#define SOC_CRGPERIPH_PEREN0_gt_clk_vcodec2cfgbus_START (17)
#define SOC_CRGPERIPH_PEREN0_gt_clk_vcodec2cfgbus_END (17)
#define SOC_CRGPERIPH_PEREN0_gt_clk_sysbus2hsdt1_START (18)
#define SOC_CRGPERIPH_PEREN0_gt_clk_sysbus2hsdt1_END (18)
#define SOC_CRGPERIPH_PEREN0_gt_aclk_dmss_free_START (19)
#define SOC_CRGPERIPH_PEREN0_gt_aclk_dmss_free_END (19)
#define SOC_CRGPERIPH_PEREN0_gt_clk_lpmcu2cfgbus_START (20)
#define SOC_CRGPERIPH_PEREN0_gt_clk_lpmcu2cfgbus_END (20)
#define SOC_CRGPERIPH_PEREN0_gt_clk_hsdt1bus_START (21)
#define SOC_CRGPERIPH_PEREN0_gt_clk_hsdt1bus_END (21)
#define SOC_CRGPERIPH_PEREN0_gt_clk_dmabus_START (22)
#define SOC_CRGPERIPH_PEREN0_gt_clk_dmabus_END (22)
#define SOC_CRGPERIPH_PEREN0_gt_clk_dma2sysbus_START (23)
#define SOC_CRGPERIPH_PEREN0_gt_clk_dma2sysbus_END (23)
#define SOC_CRGPERIPH_PEREN0_gt_clk_adb_mst_npu_START (25)
#define SOC_CRGPERIPH_PEREN0_gt_clk_adb_mst_npu_END (25)
#define SOC_CRGPERIPH_PEREN0_gt_clk_ao_asp_START (26)
#define SOC_CRGPERIPH_PEREN0_gt_clk_ao_asp_END (26)
#define SOC_CRGPERIPH_PEREN0_gt_clk_npu_tcxo_START (27)
#define SOC_CRGPERIPH_PEREN0_gt_clk_npu_tcxo_END (27)
#define SOC_CRGPERIPH_PEREN0_gt_clk_npu_gic_START (28)
#define SOC_CRGPERIPH_PEREN0_gt_clk_npu_gic_END (28)
#define SOC_CRGPERIPH_PEREN0_gt_clk_noise_print_START (31)
#define SOC_CRGPERIPH_PEREN0_gt_clk_noise_print_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_pclk_hsdt_crg : 1;
        unsigned int gt_clk_hsdtbus : 1;
        unsigned int gt_aclk_ddrc : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_pclk_hsdt1_crg : 1;
        unsigned int gt_aclk_dmss2dmc_mux : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_clk_ipf : 1;
        unsigned int gt_clk_tiny_core_peri : 1;
        unsigned int gt_clk_sysbus : 1;
        unsigned int gt_clk_cfgbus : 1;
        unsigned int gt_clk_sys2cfgbus : 1;
        unsigned int gt_clk_noc_cfg2fcm : 1;
        unsigned int gt_clk_fcm_nic_m : 1;
        unsigned int gt_clk_hsdt1_subsys : 1;
        unsigned int gt_clk_sysbus_r : 1;
        unsigned int gt_clk_sysbus_l : 1;
        unsigned int gt_clk_vcodec2cfgbus : 1;
        unsigned int gt_clk_sysbus2hsdt1 : 1;
        unsigned int gt_aclk_dmss_free : 1;
        unsigned int gt_clk_lpmcu2cfgbus : 1;
        unsigned int gt_clk_hsdt1bus : 1;
        unsigned int gt_clk_dmabus : 1;
        unsigned int gt_clk_dma2sysbus : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_clk_adb_mst_npu : 1;
        unsigned int gt_clk_ao_asp : 1;
        unsigned int gt_clk_npu_tcxo : 1;
        unsigned int gt_clk_npu_gic : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int gt_clk_noise_print : 1;
    } reg;
} SOC_CRGPERIPH_PERDIS0_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS0_gt_pclk_hsdt_crg_START (0)
#define SOC_CRGPERIPH_PERDIS0_gt_pclk_hsdt_crg_END (0)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_hsdtbus_START (1)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_hsdtbus_END (1)
#define SOC_CRGPERIPH_PERDIS0_gt_aclk_ddrc_START (2)
#define SOC_CRGPERIPH_PERDIS0_gt_aclk_ddrc_END (2)
#define SOC_CRGPERIPH_PERDIS0_gt_pclk_hsdt1_crg_START (4)
#define SOC_CRGPERIPH_PERDIS0_gt_pclk_hsdt1_crg_END (4)
#define SOC_CRGPERIPH_PERDIS0_gt_aclk_dmss2dmc_mux_START (5)
#define SOC_CRGPERIPH_PERDIS0_gt_aclk_dmss2dmc_mux_END (5)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_ipf_START (7)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_ipf_END (7)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_tiny_core_peri_START (8)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_tiny_core_peri_END (8)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_sysbus_START (9)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_sysbus_END (9)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_cfgbus_START (10)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_cfgbus_END (10)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_sys2cfgbus_START (11)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_sys2cfgbus_END (11)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_noc_cfg2fcm_START (12)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_noc_cfg2fcm_END (12)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_fcm_nic_m_START (13)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_fcm_nic_m_END (13)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_hsdt1_subsys_START (14)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_hsdt1_subsys_END (14)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_sysbus_r_START (15)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_sysbus_r_END (15)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_sysbus_l_START (16)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_sysbus_l_END (16)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_vcodec2cfgbus_START (17)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_vcodec2cfgbus_END (17)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_sysbus2hsdt1_START (18)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_sysbus2hsdt1_END (18)
#define SOC_CRGPERIPH_PERDIS0_gt_aclk_dmss_free_START (19)
#define SOC_CRGPERIPH_PERDIS0_gt_aclk_dmss_free_END (19)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_lpmcu2cfgbus_START (20)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_lpmcu2cfgbus_END (20)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_hsdt1bus_START (21)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_hsdt1bus_END (21)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_dmabus_START (22)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_dmabus_END (22)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_dma2sysbus_START (23)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_dma2sysbus_END (23)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_adb_mst_npu_START (25)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_adb_mst_npu_END (25)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_ao_asp_START (26)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_ao_asp_END (26)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_npu_tcxo_START (27)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_npu_tcxo_END (27)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_npu_gic_START (28)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_npu_gic_END (28)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_noise_print_START (31)
#define SOC_CRGPERIPH_PERDIS0_gt_clk_noise_print_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_pclk_hsdt_crg : 1;
        unsigned int gt_clk_hsdtbus : 1;
        unsigned int gt_aclk_ddrc : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_pclk_hsdt1_crg : 1;
        unsigned int gt_aclk_dmss2dmc_mux : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_clk_ipf : 1;
        unsigned int gt_clk_tiny_core_peri : 1;
        unsigned int gt_clk_sysbus : 1;
        unsigned int gt_clk_cfgbus : 1;
        unsigned int gt_clk_sys2cfgbus : 1;
        unsigned int gt_clk_noc_cfg2fcm : 1;
        unsigned int gt_clk_fcm_nic_m : 1;
        unsigned int gt_clk_hsdt1_subsys : 1;
        unsigned int gt_clk_sysbus_r : 1;
        unsigned int gt_clk_sysbus_l : 1;
        unsigned int gt_clk_vcodec2cfgbus : 1;
        unsigned int gt_clk_sysbus2hsdt1 : 1;
        unsigned int gt_aclk_dmss_free : 1;
        unsigned int gt_clk_lpmcu2cfgbus : 1;
        unsigned int gt_clk_hsdt1bus : 1;
        unsigned int gt_clk_dmabus : 1;
        unsigned int gt_clk_dma2sysbus : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_clk_adb_mst_npu : 1;
        unsigned int gt_clk_ao_asp : 1;
        unsigned int gt_clk_npu_tcxo : 1;
        unsigned int gt_clk_npu_gic : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int gt_clk_noise_print : 1;
    } reg;
} SOC_CRGPERIPH_PERCLKEN0_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN0_gt_pclk_hsdt_crg_START (0)
#define SOC_CRGPERIPH_PERCLKEN0_gt_pclk_hsdt_crg_END (0)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_hsdtbus_START (1)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_hsdtbus_END (1)
#define SOC_CRGPERIPH_PERCLKEN0_gt_aclk_ddrc_START (2)
#define SOC_CRGPERIPH_PERCLKEN0_gt_aclk_ddrc_END (2)
#define SOC_CRGPERIPH_PERCLKEN0_gt_pclk_hsdt1_crg_START (4)
#define SOC_CRGPERIPH_PERCLKEN0_gt_pclk_hsdt1_crg_END (4)
#define SOC_CRGPERIPH_PERCLKEN0_gt_aclk_dmss2dmc_mux_START (5)
#define SOC_CRGPERIPH_PERCLKEN0_gt_aclk_dmss2dmc_mux_END (5)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_ipf_START (7)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_ipf_END (7)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_tiny_core_peri_START (8)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_tiny_core_peri_END (8)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_sysbus_START (9)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_sysbus_END (9)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_cfgbus_START (10)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_cfgbus_END (10)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_sys2cfgbus_START (11)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_sys2cfgbus_END (11)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_noc_cfg2fcm_START (12)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_noc_cfg2fcm_END (12)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_fcm_nic_m_START (13)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_fcm_nic_m_END (13)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_hsdt1_subsys_START (14)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_hsdt1_subsys_END (14)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_sysbus_r_START (15)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_sysbus_r_END (15)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_sysbus_l_START (16)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_sysbus_l_END (16)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_vcodec2cfgbus_START (17)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_vcodec2cfgbus_END (17)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_sysbus2hsdt1_START (18)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_sysbus2hsdt1_END (18)
#define SOC_CRGPERIPH_PERCLKEN0_gt_aclk_dmss_free_START (19)
#define SOC_CRGPERIPH_PERCLKEN0_gt_aclk_dmss_free_END (19)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_lpmcu2cfgbus_START (20)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_lpmcu2cfgbus_END (20)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_hsdt1bus_START (21)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_hsdt1bus_END (21)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_dmabus_START (22)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_dmabus_END (22)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_dma2sysbus_START (23)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_dma2sysbus_END (23)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_adb_mst_npu_START (25)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_adb_mst_npu_END (25)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_ao_asp_START (26)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_ao_asp_END (26)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_npu_tcxo_START (27)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_npu_tcxo_END (27)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_npu_gic_START (28)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_npu_gic_END (28)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_noise_print_START (31)
#define SOC_CRGPERIPH_PERCLKEN0_gt_clk_noise_print_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_pclk_hsdt_crg : 1;
        unsigned int st_clk_hsdtbus : 1;
        unsigned int st_aclk_ddrc : 1;
        unsigned int st_clk_ddrcfg : 1;
        unsigned int st_pclk_hsdt1_crg : 1;
        unsigned int st_aclk_dmss2dmc_mux : 1;
        unsigned int reserved_0 : 1;
        unsigned int st_clk_ipf : 1;
        unsigned int st_clk_tiny_core_peri : 1;
        unsigned int st_clk_sysbus : 1;
        unsigned int st_clk_cfgbus : 1;
        unsigned int st_clk_sys2cfgbus : 1;
        unsigned int st_clk_noc_cfg2fcm : 1;
        unsigned int st_clk_fcm_nic_m : 1;
        unsigned int st_clk_hsdt1_subsys : 1;
        unsigned int st_clk_sysbus_r : 1;
        unsigned int st_clk_sysbus_l : 1;
        unsigned int st_clk_vcodec2cfgbus : 1;
        unsigned int st_clk_sysbus2hsdt1 : 1;
        unsigned int st_aclk_dmss_free : 1;
        unsigned int st_clk_lpmcu2cfgbus : 1;
        unsigned int st_clk_hsdt1bus : 1;
        unsigned int st_clk_dmabus : 1;
        unsigned int st_clk_dma2sysbus : 1;
        unsigned int reserved_1 : 1;
        unsigned int st_clk_adb_mst_npu : 1;
        unsigned int st_clk_ao_asp : 1;
        unsigned int st_clk_npu_tcxo : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int st_clk_noise_print : 1;
    } reg;
} SOC_CRGPERIPH_PERSTAT0_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT0_st_pclk_hsdt_crg_START (0)
#define SOC_CRGPERIPH_PERSTAT0_st_pclk_hsdt_crg_END (0)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_hsdtbus_START (1)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_hsdtbus_END (1)
#define SOC_CRGPERIPH_PERSTAT0_st_aclk_ddrc_START (2)
#define SOC_CRGPERIPH_PERSTAT0_st_aclk_ddrc_END (2)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_ddrcfg_START (3)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_ddrcfg_END (3)
#define SOC_CRGPERIPH_PERSTAT0_st_pclk_hsdt1_crg_START (4)
#define SOC_CRGPERIPH_PERSTAT0_st_pclk_hsdt1_crg_END (4)
#define SOC_CRGPERIPH_PERSTAT0_st_aclk_dmss2dmc_mux_START (5)
#define SOC_CRGPERIPH_PERSTAT0_st_aclk_dmss2dmc_mux_END (5)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_ipf_START (7)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_ipf_END (7)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_tiny_core_peri_START (8)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_tiny_core_peri_END (8)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_sysbus_START (9)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_sysbus_END (9)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_cfgbus_START (10)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_cfgbus_END (10)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_sys2cfgbus_START (11)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_sys2cfgbus_END (11)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_noc_cfg2fcm_START (12)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_noc_cfg2fcm_END (12)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_fcm_nic_m_START (13)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_fcm_nic_m_END (13)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_hsdt1_subsys_START (14)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_hsdt1_subsys_END (14)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_sysbus_r_START (15)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_sysbus_r_END (15)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_sysbus_l_START (16)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_sysbus_l_END (16)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_vcodec2cfgbus_START (17)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_vcodec2cfgbus_END (17)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_sysbus2hsdt1_START (18)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_sysbus2hsdt1_END (18)
#define SOC_CRGPERIPH_PERSTAT0_st_aclk_dmss_free_START (19)
#define SOC_CRGPERIPH_PERSTAT0_st_aclk_dmss_free_END (19)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_lpmcu2cfgbus_START (20)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_lpmcu2cfgbus_END (20)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_hsdt1bus_START (21)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_hsdt1bus_END (21)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_dmabus_START (22)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_dmabus_END (22)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_dma2sysbus_START (23)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_dma2sysbus_END (23)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_adb_mst_npu_START (25)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_adb_mst_npu_END (25)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_ao_asp_START (26)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_ao_asp_END (26)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_npu_tcxo_START (27)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_npu_tcxo_END (27)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_noise_print_START (31)
#define SOC_CRGPERIPH_PERSTAT0_st_clk_noise_print_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_pclk_gpio0 : 1;
        unsigned int gt_pclk_gpio1 : 1;
        unsigned int gt_pclk_gpio2 : 1;
        unsigned int gt_pclk_gpio3 : 1;
        unsigned int gt_pclk_gpio4 : 1;
        unsigned int gt_pclk_gpio5 : 1;
        unsigned int gt_pclk_gpio6 : 1;
        unsigned int gt_pclk_gpio7 : 1;
        unsigned int gt_pclk_gpio8 : 1;
        unsigned int gt_pclk_gpio9 : 1;
        unsigned int gt_pclk_gpio10 : 1;
        unsigned int gt_pclk_gpio11 : 1;
        unsigned int gt_pclk_gpio12 : 1;
        unsigned int gt_pclk_gpio13 : 1;
        unsigned int gt_pclk_gpio14 : 1;
        unsigned int gt_pclk_gpio15 : 1;
        unsigned int gt_pclk_gpio16 : 1;
        unsigned int gt_pclk_gpio17 : 1;
        unsigned int gt_pclk_gpio18 : 1;
        unsigned int gt_pclk_gpio19 : 1;
        unsigned int gt_clk_socp_mcpu_5g : 1;
        unsigned int reserved : 1;
        unsigned int gt_pclk_timer9 : 1;
        unsigned int gt_pclk_timer10 : 1;
        unsigned int gt_pclk_timer11 : 1;
        unsigned int gt_pclk_timer12 : 1;
        unsigned int gt_clk_socp_lpm3 : 1;
        unsigned int gt_clk_djtag : 1;
        unsigned int gt_clk_socp_acpu : 1;
        unsigned int gt_clk_socp_mcpu_4g : 1;
        unsigned int gt_clk_i2c6 : 1;
        unsigned int gt_clk_i2c7 : 1;
    } reg;
} SOC_CRGPERIPH_PEREN1_UNION;
#endif
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio0_START (0)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio0_END (0)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio1_START (1)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio1_END (1)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio2_START (2)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio2_END (2)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio3_START (3)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio3_END (3)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio4_START (4)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio4_END (4)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio5_START (5)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio5_END (5)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio6_START (6)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio6_END (6)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio7_START (7)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio7_END (7)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio8_START (8)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio8_END (8)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio9_START (9)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio9_END (9)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio10_START (10)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio10_END (10)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio11_START (11)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio11_END (11)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio12_START (12)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio12_END (12)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio13_START (13)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio13_END (13)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio14_START (14)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio14_END (14)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio15_START (15)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio15_END (15)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio16_START (16)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio16_END (16)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio17_START (17)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio17_END (17)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio18_START (18)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio18_END (18)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio19_START (19)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_gpio19_END (19)
#define SOC_CRGPERIPH_PEREN1_gt_clk_socp_mcpu_5g_START (20)
#define SOC_CRGPERIPH_PEREN1_gt_clk_socp_mcpu_5g_END (20)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_timer9_START (22)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_timer9_END (22)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_timer10_START (23)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_timer10_END (23)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_timer11_START (24)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_timer11_END (24)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_timer12_START (25)
#define SOC_CRGPERIPH_PEREN1_gt_pclk_timer12_END (25)
#define SOC_CRGPERIPH_PEREN1_gt_clk_socp_lpm3_START (26)
#define SOC_CRGPERIPH_PEREN1_gt_clk_socp_lpm3_END (26)
#define SOC_CRGPERIPH_PEREN1_gt_clk_djtag_START (27)
#define SOC_CRGPERIPH_PEREN1_gt_clk_djtag_END (27)
#define SOC_CRGPERIPH_PEREN1_gt_clk_socp_acpu_START (28)
#define SOC_CRGPERIPH_PEREN1_gt_clk_socp_acpu_END (28)
#define SOC_CRGPERIPH_PEREN1_gt_clk_socp_mcpu_4g_START (29)
#define SOC_CRGPERIPH_PEREN1_gt_clk_socp_mcpu_4g_END (29)
#define SOC_CRGPERIPH_PEREN1_gt_clk_i2c6_START (30)
#define SOC_CRGPERIPH_PEREN1_gt_clk_i2c6_END (30)
#define SOC_CRGPERIPH_PEREN1_gt_clk_i2c7_START (31)
#define SOC_CRGPERIPH_PEREN1_gt_clk_i2c7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_pclk_gpio0 : 1;
        unsigned int gt_pclk_gpio1 : 1;
        unsigned int gt_pclk_gpio2 : 1;
        unsigned int gt_pclk_gpio3 : 1;
        unsigned int gt_pclk_gpio4 : 1;
        unsigned int gt_pclk_gpio5 : 1;
        unsigned int gt_pclk_gpio6 : 1;
        unsigned int gt_pclk_gpio7 : 1;
        unsigned int gt_pclk_gpio8 : 1;
        unsigned int gt_pclk_gpio9 : 1;
        unsigned int gt_pclk_gpio10 : 1;
        unsigned int gt_pclk_gpio11 : 1;
        unsigned int gt_pclk_gpio12 : 1;
        unsigned int gt_pclk_gpio13 : 1;
        unsigned int gt_pclk_gpio14 : 1;
        unsigned int gt_pclk_gpio15 : 1;
        unsigned int gt_pclk_gpio16 : 1;
        unsigned int gt_pclk_gpio17 : 1;
        unsigned int gt_pclk_gpio18 : 1;
        unsigned int gt_pclk_gpio19 : 1;
        unsigned int gt_clk_socp_mcpu_5g : 1;
        unsigned int reserved : 1;
        unsigned int gt_pclk_timer9 : 1;
        unsigned int gt_pclk_timer10 : 1;
        unsigned int gt_pclk_timer11 : 1;
        unsigned int gt_pclk_timer12 : 1;
        unsigned int gt_clk_socp_lpm3 : 1;
        unsigned int gt_clk_djtag : 1;
        unsigned int gt_clk_socp_acpu : 1;
        unsigned int gt_clk_socp_mcpu_4g : 1;
        unsigned int gt_clk_i2c6 : 1;
        unsigned int gt_clk_i2c7 : 1;
    } reg;
} SOC_CRGPERIPH_PERDIS1_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio0_START (0)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio0_END (0)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio1_START (1)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio1_END (1)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio2_START (2)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio2_END (2)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio3_START (3)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio3_END (3)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio4_START (4)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio4_END (4)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio5_START (5)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio5_END (5)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio6_START (6)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio6_END (6)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio7_START (7)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio7_END (7)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio8_START (8)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio8_END (8)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio9_START (9)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio9_END (9)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio10_START (10)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio10_END (10)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio11_START (11)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio11_END (11)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio12_START (12)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio12_END (12)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio13_START (13)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio13_END (13)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio14_START (14)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio14_END (14)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio15_START (15)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio15_END (15)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio16_START (16)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio16_END (16)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio17_START (17)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio17_END (17)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio18_START (18)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio18_END (18)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio19_START (19)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_gpio19_END (19)
#define SOC_CRGPERIPH_PERDIS1_gt_clk_socp_mcpu_5g_START (20)
#define SOC_CRGPERIPH_PERDIS1_gt_clk_socp_mcpu_5g_END (20)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_timer9_START (22)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_timer9_END (22)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_timer10_START (23)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_timer10_END (23)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_timer11_START (24)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_timer11_END (24)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_timer12_START (25)
#define SOC_CRGPERIPH_PERDIS1_gt_pclk_timer12_END (25)
#define SOC_CRGPERIPH_PERDIS1_gt_clk_socp_lpm3_START (26)
#define SOC_CRGPERIPH_PERDIS1_gt_clk_socp_lpm3_END (26)
#define SOC_CRGPERIPH_PERDIS1_gt_clk_djtag_START (27)
#define SOC_CRGPERIPH_PERDIS1_gt_clk_djtag_END (27)
#define SOC_CRGPERIPH_PERDIS1_gt_clk_socp_acpu_START (28)
#define SOC_CRGPERIPH_PERDIS1_gt_clk_socp_acpu_END (28)
#define SOC_CRGPERIPH_PERDIS1_gt_clk_socp_mcpu_4g_START (29)
#define SOC_CRGPERIPH_PERDIS1_gt_clk_socp_mcpu_4g_END (29)
#define SOC_CRGPERIPH_PERDIS1_gt_clk_i2c6_START (30)
#define SOC_CRGPERIPH_PERDIS1_gt_clk_i2c6_END (30)
#define SOC_CRGPERIPH_PERDIS1_gt_clk_i2c7_START (31)
#define SOC_CRGPERIPH_PERDIS1_gt_clk_i2c7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_pclk_gpio0 : 1;
        unsigned int gt_pclk_gpio1 : 1;
        unsigned int gt_pclk_gpio2 : 1;
        unsigned int gt_pclk_gpio3 : 1;
        unsigned int gt_pclk_gpio4 : 1;
        unsigned int gt_pclk_gpio5 : 1;
        unsigned int gt_pclk_gpio6 : 1;
        unsigned int gt_pclk_gpio7 : 1;
        unsigned int gt_pclk_gpio8 : 1;
        unsigned int gt_pclk_gpio9 : 1;
        unsigned int gt_pclk_gpio10 : 1;
        unsigned int gt_pclk_gpio11 : 1;
        unsigned int gt_pclk_gpio12 : 1;
        unsigned int gt_pclk_gpio13 : 1;
        unsigned int gt_pclk_gpio14 : 1;
        unsigned int gt_pclk_gpio15 : 1;
        unsigned int gt_pclk_gpio16 : 1;
        unsigned int gt_pclk_gpio17 : 1;
        unsigned int gt_pclk_gpio18 : 1;
        unsigned int gt_pclk_gpio19 : 1;
        unsigned int gt_clk_socp_mcpu_5g : 1;
        unsigned int reserved : 1;
        unsigned int gt_pclk_timer9 : 1;
        unsigned int gt_pclk_timer10 : 1;
        unsigned int gt_pclk_timer11 : 1;
        unsigned int gt_pclk_timer12 : 1;
        unsigned int gt_clk_socp_lpm3 : 1;
        unsigned int gt_clk_djtag : 1;
        unsigned int gt_clk_socp_acpu : 1;
        unsigned int gt_clk_socp_mcpu_4g : 1;
        unsigned int gt_clk_i2c6 : 1;
        unsigned int gt_clk_i2c7 : 1;
    } reg;
} SOC_CRGPERIPH_PERCLKEN1_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio0_START (0)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio0_END (0)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio1_START (1)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio1_END (1)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio2_START (2)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio2_END (2)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio3_START (3)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio3_END (3)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio4_START (4)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio4_END (4)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio5_START (5)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio5_END (5)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio6_START (6)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio6_END (6)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio7_START (7)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio7_END (7)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio8_START (8)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio8_END (8)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio9_START (9)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio9_END (9)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio10_START (10)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio10_END (10)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio11_START (11)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio11_END (11)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio12_START (12)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio12_END (12)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio13_START (13)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio13_END (13)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio14_START (14)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio14_END (14)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio15_START (15)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio15_END (15)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio16_START (16)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio16_END (16)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio17_START (17)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio17_END (17)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio18_START (18)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio18_END (18)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio19_START (19)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_gpio19_END (19)
#define SOC_CRGPERIPH_PERCLKEN1_gt_clk_socp_mcpu_5g_START (20)
#define SOC_CRGPERIPH_PERCLKEN1_gt_clk_socp_mcpu_5g_END (20)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_timer9_START (22)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_timer9_END (22)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_timer10_START (23)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_timer10_END (23)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_timer11_START (24)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_timer11_END (24)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_timer12_START (25)
#define SOC_CRGPERIPH_PERCLKEN1_gt_pclk_timer12_END (25)
#define SOC_CRGPERIPH_PERCLKEN1_gt_clk_socp_lpm3_START (26)
#define SOC_CRGPERIPH_PERCLKEN1_gt_clk_socp_lpm3_END (26)
#define SOC_CRGPERIPH_PERCLKEN1_gt_clk_djtag_START (27)
#define SOC_CRGPERIPH_PERCLKEN1_gt_clk_djtag_END (27)
#define SOC_CRGPERIPH_PERCLKEN1_gt_clk_socp_acpu_START (28)
#define SOC_CRGPERIPH_PERCLKEN1_gt_clk_socp_acpu_END (28)
#define SOC_CRGPERIPH_PERCLKEN1_gt_clk_socp_mcpu_4g_START (29)
#define SOC_CRGPERIPH_PERCLKEN1_gt_clk_socp_mcpu_4g_END (29)
#define SOC_CRGPERIPH_PERCLKEN1_gt_clk_i2c6_START (30)
#define SOC_CRGPERIPH_PERCLKEN1_gt_clk_i2c6_END (30)
#define SOC_CRGPERIPH_PERCLKEN1_gt_clk_i2c7_START (31)
#define SOC_CRGPERIPH_PERCLKEN1_gt_clk_i2c7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_pclk_gpio0 : 1;
        unsigned int st_pclk_gpio1 : 1;
        unsigned int st_pclk_gpio2 : 1;
        unsigned int st_pclk_gpio3 : 1;
        unsigned int st_pclk_gpio4 : 1;
        unsigned int st_pclk_gpio5 : 1;
        unsigned int st_pclk_gpio6 : 1;
        unsigned int st_pclk_gpio7 : 1;
        unsigned int st_pclk_gpio8 : 1;
        unsigned int st_pclk_gpio9 : 1;
        unsigned int st_pclk_gpio10 : 1;
        unsigned int st_pclk_gpio11 : 1;
        unsigned int st_pclk_gpio12 : 1;
        unsigned int st_pclk_gpio13 : 1;
        unsigned int st_pclk_gpio14 : 1;
        unsigned int st_pclk_gpio15 : 1;
        unsigned int st_pclk_gpio16 : 1;
        unsigned int st_pclk_gpio17 : 1;
        unsigned int st_pclk_gpio18 : 1;
        unsigned int st_pclk_gpio19 : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int st_pclk_timer9 : 1;
        unsigned int st_pclk_timer10 : 1;
        unsigned int st_pclk_timer11 : 1;
        unsigned int st_pclk_timer12 : 1;
        unsigned int st_clk_socp : 1;
        unsigned int st_clk_djtag : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int st_clk_i2c6 : 1;
        unsigned int st_clk_i2c7 : 1;
    } reg;
} SOC_CRGPERIPH_PERSTAT1_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio0_START (0)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio0_END (0)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio1_START (1)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio1_END (1)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio2_START (2)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio2_END (2)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio3_START (3)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio3_END (3)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio4_START (4)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio4_END (4)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio5_START (5)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio5_END (5)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio6_START (6)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio6_END (6)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio7_START (7)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio7_END (7)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio8_START (8)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio8_END (8)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio9_START (9)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio9_END (9)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio10_START (10)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio10_END (10)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio11_START (11)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio11_END (11)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio12_START (12)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio12_END (12)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio13_START (13)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio13_END (13)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio14_START (14)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio14_END (14)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio15_START (15)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio15_END (15)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio16_START (16)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio16_END (16)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio17_START (17)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio17_END (17)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio18_START (18)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio18_END (18)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio19_START (19)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_gpio19_END (19)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_timer9_START (22)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_timer9_END (22)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_timer10_START (23)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_timer10_END (23)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_timer11_START (24)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_timer11_END (24)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_timer12_START (25)
#define SOC_CRGPERIPH_PERSTAT1_st_pclk_timer12_END (25)
#define SOC_CRGPERIPH_PERSTAT1_st_clk_socp_START (26)
#define SOC_CRGPERIPH_PERSTAT1_st_clk_socp_END (26)
#define SOC_CRGPERIPH_PERSTAT1_st_clk_djtag_START (27)
#define SOC_CRGPERIPH_PERSTAT1_st_clk_djtag_END (27)
#define SOC_CRGPERIPH_PERSTAT1_st_clk_i2c6_START (30)
#define SOC_CRGPERIPH_PERSTAT1_st_clk_i2c6_END (30)
#define SOC_CRGPERIPH_PERSTAT1_st_clk_i2c7_START (31)
#define SOC_CRGPERIPH_PERSTAT1_st_clk_i2c7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_pclk_pwm : 1;
        unsigned int gt_clk_i2c2 : 1;
        unsigned int gt_pclk_ipc0 : 1;
        unsigned int gt_pclk_ipc1 : 1;
        unsigned int reserved : 1;
        unsigned int gt_clk_loadmonitor : 1;
        unsigned int gt_pclk_loadmonitor : 1;
        unsigned int gt_clk_i2c3 : 1;
        unsigned int gt_pclk_loadmonitor_lpmcu : 1;
        unsigned int gt_clk_spi1 : 1;
        unsigned int gt_clk_uart0 : 1;
        unsigned int gt_clk_uart1 : 1;
        unsigned int gt_clk_uart2 : 1;
        unsigned int gt_clk_adb_mst_mdm : 1;
        unsigned int gt_clk_uart4 : 1;
        unsigned int gt_clk_uart5 : 1;
        unsigned int gt_pclk_wd0 : 1;
        unsigned int gt_pclk_wd1 : 1;
        unsigned int gt_pclk_tzpc : 1;
        unsigned int gt_clk_out1_pre : 1;
        unsigned int gt_pclk_ipc_mdm : 1;
        unsigned int gt_clk_adb_mst_fcm0 : 1;
        unsigned int gt_clk_adb_mst_fcm1 : 1;
        unsigned int gt_aclk_gic : 1;
        unsigned int gt_clk_adb_slv_acp : 1;
        unsigned int gt_pclk_ioc : 1;
        unsigned int gt_clk_codecssi : 1;
        unsigned int gt_clk_i2c4 : 1;
        unsigned int gt_clk_loadmonitor_1 : 1;
        unsigned int gt_pclk_loadmonitor_1 : 1;
        unsigned int gt_pclk_loadmonitor_1_lpmcu : 1;
        unsigned int gt_pclk_pctrl : 1;
    } reg;
} SOC_CRGPERIPH_PEREN2_UNION;
#endif
#define SOC_CRGPERIPH_PEREN2_gt_pclk_pwm_START (0)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_pwm_END (0)
#define SOC_CRGPERIPH_PEREN2_gt_clk_i2c2_START (1)
#define SOC_CRGPERIPH_PEREN2_gt_clk_i2c2_END (1)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_ipc0_START (2)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_ipc0_END (2)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_ipc1_START (3)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_ipc1_END (3)
#define SOC_CRGPERIPH_PEREN2_gt_clk_loadmonitor_START (5)
#define SOC_CRGPERIPH_PEREN2_gt_clk_loadmonitor_END (5)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_loadmonitor_START (6)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_loadmonitor_END (6)
#define SOC_CRGPERIPH_PEREN2_gt_clk_i2c3_START (7)
#define SOC_CRGPERIPH_PEREN2_gt_clk_i2c3_END (7)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_loadmonitor_lpmcu_START (8)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_loadmonitor_lpmcu_END (8)
#define SOC_CRGPERIPH_PEREN2_gt_clk_spi1_START (9)
#define SOC_CRGPERIPH_PEREN2_gt_clk_spi1_END (9)
#define SOC_CRGPERIPH_PEREN2_gt_clk_uart0_START (10)
#define SOC_CRGPERIPH_PEREN2_gt_clk_uart0_END (10)
#define SOC_CRGPERIPH_PEREN2_gt_clk_uart1_START (11)
#define SOC_CRGPERIPH_PEREN2_gt_clk_uart1_END (11)
#define SOC_CRGPERIPH_PEREN2_gt_clk_uart2_START (12)
#define SOC_CRGPERIPH_PEREN2_gt_clk_uart2_END (12)
#define SOC_CRGPERIPH_PEREN2_gt_clk_adb_mst_mdm_START (13)
#define SOC_CRGPERIPH_PEREN2_gt_clk_adb_mst_mdm_END (13)
#define SOC_CRGPERIPH_PEREN2_gt_clk_uart4_START (14)
#define SOC_CRGPERIPH_PEREN2_gt_clk_uart4_END (14)
#define SOC_CRGPERIPH_PEREN2_gt_clk_uart5_START (15)
#define SOC_CRGPERIPH_PEREN2_gt_clk_uart5_END (15)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_wd0_START (16)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_wd0_END (16)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_wd1_START (17)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_wd1_END (17)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_tzpc_START (18)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_tzpc_END (18)
#define SOC_CRGPERIPH_PEREN2_gt_clk_out1_pre_START (19)
#define SOC_CRGPERIPH_PEREN2_gt_clk_out1_pre_END (19)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_ipc_mdm_START (20)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_ipc_mdm_END (20)
#define SOC_CRGPERIPH_PEREN2_gt_clk_adb_mst_fcm0_START (21)
#define SOC_CRGPERIPH_PEREN2_gt_clk_adb_mst_fcm0_END (21)
#define SOC_CRGPERIPH_PEREN2_gt_clk_adb_mst_fcm1_START (22)
#define SOC_CRGPERIPH_PEREN2_gt_clk_adb_mst_fcm1_END (22)
#define SOC_CRGPERIPH_PEREN2_gt_aclk_gic_START (23)
#define SOC_CRGPERIPH_PEREN2_gt_aclk_gic_END (23)
#define SOC_CRGPERIPH_PEREN2_gt_clk_adb_slv_acp_START (24)
#define SOC_CRGPERIPH_PEREN2_gt_clk_adb_slv_acp_END (24)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_ioc_START (25)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_ioc_END (25)
#define SOC_CRGPERIPH_PEREN2_gt_clk_codecssi_START (26)
#define SOC_CRGPERIPH_PEREN2_gt_clk_codecssi_END (26)
#define SOC_CRGPERIPH_PEREN2_gt_clk_i2c4_START (27)
#define SOC_CRGPERIPH_PEREN2_gt_clk_i2c4_END (27)
#define SOC_CRGPERIPH_PEREN2_gt_clk_loadmonitor_1_START (28)
#define SOC_CRGPERIPH_PEREN2_gt_clk_loadmonitor_1_END (28)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_loadmonitor_1_START (29)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_loadmonitor_1_END (29)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_loadmonitor_1_lpmcu_START (30)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_loadmonitor_1_lpmcu_END (30)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_pctrl_START (31)
#define SOC_CRGPERIPH_PEREN2_gt_pclk_pctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_pclk_pwm : 1;
        unsigned int gt_clk_i2c2 : 1;
        unsigned int gt_pclk_ipc0 : 1;
        unsigned int gt_pclk_ipc1 : 1;
        unsigned int reserved : 1;
        unsigned int gt_clk_loadmonitor : 1;
        unsigned int gt_pclk_loadmonitor : 1;
        unsigned int gt_clk_i2c3 : 1;
        unsigned int gt_pclk_loadmonitor_lpmcu : 1;
        unsigned int gt_clk_spi1 : 1;
        unsigned int gt_clk_uart0 : 1;
        unsigned int gt_clk_uart1 : 1;
        unsigned int gt_clk_uart2 : 1;
        unsigned int gt_clk_adb_mst_mdm : 1;
        unsigned int gt_clk_uart4 : 1;
        unsigned int gt_clk_uart5 : 1;
        unsigned int gt_pclk_wd0 : 1;
        unsigned int gt_pclk_wd1 : 1;
        unsigned int gt_pclk_tzpc : 1;
        unsigned int gt_clk_out1_pre : 1;
        unsigned int gt_pclk_ipc_mdm : 1;
        unsigned int gt_clk_adb_mst_fcm0 : 1;
        unsigned int gt_clk_adb_mst_fcm1 : 1;
        unsigned int gt_aclk_gic : 1;
        unsigned int gt_clk_adb_slv_acp : 1;
        unsigned int gt_pclk_ioc : 1;
        unsigned int gt_clk_codecssi : 1;
        unsigned int gt_clk_i2c4 : 1;
        unsigned int gt_clk_loadmonitor_1 : 1;
        unsigned int gt_pclk_loadmonitor_1 : 1;
        unsigned int gt_pclk_loadmonitor_1_lpmcu : 1;
        unsigned int gt_pclk_pctrl : 1;
    } reg;
} SOC_CRGPERIPH_PERDIS2_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_pwm_START (0)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_pwm_END (0)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_i2c2_START (1)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_i2c2_END (1)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_ipc0_START (2)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_ipc0_END (2)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_ipc1_START (3)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_ipc1_END (3)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_loadmonitor_START (5)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_loadmonitor_END (5)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_loadmonitor_START (6)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_loadmonitor_END (6)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_i2c3_START (7)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_i2c3_END (7)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_loadmonitor_lpmcu_START (8)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_loadmonitor_lpmcu_END (8)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_spi1_START (9)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_spi1_END (9)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_uart0_START (10)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_uart0_END (10)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_uart1_START (11)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_uart1_END (11)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_uart2_START (12)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_uart2_END (12)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_adb_mst_mdm_START (13)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_adb_mst_mdm_END (13)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_uart4_START (14)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_uart4_END (14)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_uart5_START (15)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_uart5_END (15)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_wd0_START (16)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_wd0_END (16)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_wd1_START (17)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_wd1_END (17)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_tzpc_START (18)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_tzpc_END (18)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_out1_pre_START (19)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_out1_pre_END (19)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_ipc_mdm_START (20)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_ipc_mdm_END (20)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_adb_mst_fcm0_START (21)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_adb_mst_fcm0_END (21)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_adb_mst_fcm1_START (22)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_adb_mst_fcm1_END (22)
#define SOC_CRGPERIPH_PERDIS2_gt_aclk_gic_START (23)
#define SOC_CRGPERIPH_PERDIS2_gt_aclk_gic_END (23)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_adb_slv_acp_START (24)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_adb_slv_acp_END (24)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_ioc_START (25)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_ioc_END (25)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_codecssi_START (26)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_codecssi_END (26)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_i2c4_START (27)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_i2c4_END (27)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_loadmonitor_1_START (28)
#define SOC_CRGPERIPH_PERDIS2_gt_clk_loadmonitor_1_END (28)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_loadmonitor_1_START (29)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_loadmonitor_1_END (29)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_loadmonitor_1_lpmcu_START (30)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_loadmonitor_1_lpmcu_END (30)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_pctrl_START (31)
#define SOC_CRGPERIPH_PERDIS2_gt_pclk_pctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_pclk_pwm : 1;
        unsigned int gt_clk_i2c2 : 1;
        unsigned int gt_pclk_ipc0 : 1;
        unsigned int gt_pclk_ipc1 : 1;
        unsigned int reserved : 1;
        unsigned int gt_clk_loadmonitor : 1;
        unsigned int gt_pclk_loadmonitor : 1;
        unsigned int gt_clk_i2c3 : 1;
        unsigned int gt_pclk_loadmonitor_lpmcu : 1;
        unsigned int gt_clk_spi1 : 1;
        unsigned int gt_clk_uart0 : 1;
        unsigned int gt_clk_uart1 : 1;
        unsigned int gt_clk_uart2 : 1;
        unsigned int gt_clk_adb_mst_mdm : 1;
        unsigned int gt_clk_uart4 : 1;
        unsigned int gt_clk_uart5 : 1;
        unsigned int gt_pclk_wd0 : 1;
        unsigned int gt_pclk_wd1 : 1;
        unsigned int gt_pclk_tzpc : 1;
        unsigned int gt_clk_out1_pre : 1;
        unsigned int gt_pclk_ipc_mdm : 1;
        unsigned int gt_clk_adb_mst_fcm0 : 1;
        unsigned int gt_clk_adb_mst_fcm1 : 1;
        unsigned int gt_aclk_gic : 1;
        unsigned int gt_clk_adb_slv_acp : 1;
        unsigned int gt_pclk_ioc : 1;
        unsigned int gt_clk_codecssi : 1;
        unsigned int gt_clk_i2c4 : 1;
        unsigned int gt_clk_loadmonitor_1 : 1;
        unsigned int gt_pclk_loadmonitor_1 : 1;
        unsigned int gt_pclk_loadmonitor_1_lpmcu : 1;
        unsigned int gt_pclk_pctrl : 1;
    } reg;
} SOC_CRGPERIPH_PERCLKEN2_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_pwm_START (0)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_pwm_END (0)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_i2c2_START (1)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_i2c2_END (1)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_ipc0_START (2)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_ipc0_END (2)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_ipc1_START (3)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_ipc1_END (3)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_loadmonitor_START (5)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_loadmonitor_END (5)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_loadmonitor_START (6)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_loadmonitor_END (6)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_i2c3_START (7)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_i2c3_END (7)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_loadmonitor_lpmcu_START (8)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_loadmonitor_lpmcu_END (8)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_spi1_START (9)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_spi1_END (9)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_uart0_START (10)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_uart0_END (10)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_uart1_START (11)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_uart1_END (11)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_uart2_START (12)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_uart2_END (12)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_adb_mst_mdm_START (13)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_adb_mst_mdm_END (13)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_uart4_START (14)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_uart4_END (14)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_uart5_START (15)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_uart5_END (15)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_wd0_START (16)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_wd0_END (16)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_wd1_START (17)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_wd1_END (17)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_tzpc_START (18)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_tzpc_END (18)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_out1_pre_START (19)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_out1_pre_END (19)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_ipc_mdm_START (20)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_ipc_mdm_END (20)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_adb_mst_fcm0_START (21)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_adb_mst_fcm0_END (21)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_adb_mst_fcm1_START (22)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_adb_mst_fcm1_END (22)
#define SOC_CRGPERIPH_PERCLKEN2_gt_aclk_gic_START (23)
#define SOC_CRGPERIPH_PERCLKEN2_gt_aclk_gic_END (23)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_adb_slv_acp_START (24)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_adb_slv_acp_END (24)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_ioc_START (25)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_ioc_END (25)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_codecssi_START (26)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_codecssi_END (26)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_i2c4_START (27)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_i2c4_END (27)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_loadmonitor_1_START (28)
#define SOC_CRGPERIPH_PERCLKEN2_gt_clk_loadmonitor_1_END (28)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_loadmonitor_1_START (29)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_loadmonitor_1_END (29)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_loadmonitor_1_lpmcu_START (30)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_loadmonitor_1_lpmcu_END (30)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_pctrl_START (31)
#define SOC_CRGPERIPH_PERCLKEN2_gt_pclk_pctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_pclk_pwm : 1;
        unsigned int st_clk_i2c2 : 1;
        unsigned int st_pclk_ipc0 : 1;
        unsigned int st_pclk_ipc1 : 1;
        unsigned int reserved_0 : 1;
        unsigned int st_clk_loadmonitor : 1;
        unsigned int st_pclk_loadmonitor : 1;
        unsigned int st_clk_i2c3 : 1;
        unsigned int reserved_1 : 1;
        unsigned int st_clk_spi1 : 1;
        unsigned int st_clk_uart0 : 1;
        unsigned int st_clk_uart1 : 1;
        unsigned int st_clk_uart2 : 1;
        unsigned int st_clk_adb_mst_mdm : 1;
        unsigned int st_clk_uart4 : 1;
        unsigned int st_clk_uart5 : 1;
        unsigned int st_pclk_wd0 : 1;
        unsigned int st_pclk_wd1 : 1;
        unsigned int st_pclk_tzpc : 1;
        unsigned int st_clk_out1_pre : 1;
        unsigned int st_pclk_ipc_mdm : 1;
        unsigned int st_clk_adb_mst_fcm0 : 1;
        unsigned int st_clk_adb_mst_fcm1 : 1;
        unsigned int st_aclk_gic : 1;
        unsigned int st_clk_adb_slv_acp : 1;
        unsigned int st_pclk_ioc : 1;
        unsigned int st_clk_codecssi : 1;
        unsigned int st_clk_i2c4 : 1;
        unsigned int st_clk_loadmonitor_1 : 1;
        unsigned int st_pclk_loadmonitor_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int st_pclk_pctrl : 1;
    } reg;
} SOC_CRGPERIPH_PERSTAT2_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_pwm_START (0)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_pwm_END (0)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_i2c2_START (1)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_i2c2_END (1)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_ipc0_START (2)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_ipc0_END (2)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_ipc1_START (3)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_ipc1_END (3)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_loadmonitor_START (5)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_loadmonitor_END (5)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_loadmonitor_START (6)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_loadmonitor_END (6)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_i2c3_START (7)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_i2c3_END (7)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_spi1_START (9)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_spi1_END (9)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_uart0_START (10)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_uart0_END (10)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_uart1_START (11)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_uart1_END (11)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_uart2_START (12)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_uart2_END (12)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_adb_mst_mdm_START (13)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_adb_mst_mdm_END (13)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_uart4_START (14)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_uart4_END (14)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_uart5_START (15)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_uart5_END (15)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_wd0_START (16)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_wd0_END (16)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_wd1_START (17)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_wd1_END (17)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_tzpc_START (18)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_tzpc_END (18)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_out1_pre_START (19)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_out1_pre_END (19)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_ipc_mdm_START (20)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_ipc_mdm_END (20)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_adb_mst_fcm0_START (21)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_adb_mst_fcm0_END (21)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_adb_mst_fcm1_START (22)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_adb_mst_fcm1_END (22)
#define SOC_CRGPERIPH_PERSTAT2_st_aclk_gic_START (23)
#define SOC_CRGPERIPH_PERSTAT2_st_aclk_gic_END (23)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_adb_slv_acp_START (24)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_adb_slv_acp_END (24)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_ioc_START (25)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_ioc_END (25)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_codecssi_START (26)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_codecssi_END (26)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_i2c4_START (27)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_i2c4_END (27)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_loadmonitor_1_START (28)
#define SOC_CRGPERIPH_PERSTAT2_st_clk_loadmonitor_1_END (28)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_loadmonitor_1_START (29)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_loadmonitor_1_END (29)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_pctrl_START (31)
#define SOC_CRGPERIPH_PERSTAT2_st_pclk_pctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_aclk_dmac_lpm3 : 1;
        unsigned int gt_aclk_dmac_acpu : 1;
        unsigned int gt_aclk_dmac_mcpu : 1;
        unsigned int gt_aclk_g3d_cfg : 1;
        unsigned int gt_clk_g3dmt : 1;
        unsigned int gt_aclk_g3d_core : 1;
        unsigned int gt_clk_loadmonitor_2 : 1;
        unsigned int gt_pclk_loadmonitor_2 : 1;
        unsigned int gt_pclk_loadmonitor_2_lpmcu : 1;
        unsigned int gt_pclk_hsdt1_pcie : 1;
        unsigned int gt_clk_noc_mdm_mst : 1;
        unsigned int gt_clk_noc_mdm_cfg : 1;
        unsigned int gt_pclk_hsdt0_pcie : 1;
        unsigned int gt_clk_pll_unlock_detect : 1;
        unsigned int gt_clk_adb_mst_mdm2sys : 1;
        unsigned int gt_clk_sysbus2mdm : 1;
        unsigned int gt_clk_pll_unlock_detect_acpu : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_aclk_g3d_top_plus : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_clk_isp_i2c_media : 1;
        unsigned int gt_clk_dcdrbus : 1;
        unsigned int gt_clk_txdphy0_cfg : 1;
        unsigned int gt_clk_txdphy0_ref : 1;
        unsigned int gt_clk_txdphy1_cfg : 1;
        unsigned int gt_clk_txdphy1_ref : 1;
    } reg;
} SOC_CRGPERIPH_PEREN3_UNION;
#endif
#define SOC_CRGPERIPH_PEREN3_gt_aclk_dmac_lpm3_START (0)
#define SOC_CRGPERIPH_PEREN3_gt_aclk_dmac_lpm3_END (0)
#define SOC_CRGPERIPH_PEREN3_gt_aclk_dmac_acpu_START (1)
#define SOC_CRGPERIPH_PEREN3_gt_aclk_dmac_acpu_END (1)
#define SOC_CRGPERIPH_PEREN3_gt_aclk_dmac_mcpu_START (2)
#define SOC_CRGPERIPH_PEREN3_gt_aclk_dmac_mcpu_END (2)
#define SOC_CRGPERIPH_PEREN3_gt_aclk_g3d_cfg_START (3)
#define SOC_CRGPERIPH_PEREN3_gt_aclk_g3d_cfg_END (3)
#define SOC_CRGPERIPH_PEREN3_gt_clk_g3dmt_START (4)
#define SOC_CRGPERIPH_PEREN3_gt_clk_g3dmt_END (4)
#define SOC_CRGPERIPH_PEREN3_gt_aclk_g3d_core_START (5)
#define SOC_CRGPERIPH_PEREN3_gt_aclk_g3d_core_END (5)
#define SOC_CRGPERIPH_PEREN3_gt_clk_loadmonitor_2_START (6)
#define SOC_CRGPERIPH_PEREN3_gt_clk_loadmonitor_2_END (6)
#define SOC_CRGPERIPH_PEREN3_gt_pclk_loadmonitor_2_START (7)
#define SOC_CRGPERIPH_PEREN3_gt_pclk_loadmonitor_2_END (7)
#define SOC_CRGPERIPH_PEREN3_gt_pclk_loadmonitor_2_lpmcu_START (8)
#define SOC_CRGPERIPH_PEREN3_gt_pclk_loadmonitor_2_lpmcu_END (8)
#define SOC_CRGPERIPH_PEREN3_gt_pclk_hsdt1_pcie_START (9)
#define SOC_CRGPERIPH_PEREN3_gt_pclk_hsdt1_pcie_END (9)
#define SOC_CRGPERIPH_PEREN3_gt_clk_noc_mdm_mst_START (10)
#define SOC_CRGPERIPH_PEREN3_gt_clk_noc_mdm_mst_END (10)
#define SOC_CRGPERIPH_PEREN3_gt_clk_noc_mdm_cfg_START (11)
#define SOC_CRGPERIPH_PEREN3_gt_clk_noc_mdm_cfg_END (11)
#define SOC_CRGPERIPH_PEREN3_gt_pclk_hsdt0_pcie_START (12)
#define SOC_CRGPERIPH_PEREN3_gt_pclk_hsdt0_pcie_END (12)
#define SOC_CRGPERIPH_PEREN3_gt_clk_pll_unlock_detect_START (13)
#define SOC_CRGPERIPH_PEREN3_gt_clk_pll_unlock_detect_END (13)
#define SOC_CRGPERIPH_PEREN3_gt_clk_adb_mst_mdm2sys_START (14)
#define SOC_CRGPERIPH_PEREN3_gt_clk_adb_mst_mdm2sys_END (14)
#define SOC_CRGPERIPH_PEREN3_gt_clk_sysbus2mdm_START (15)
#define SOC_CRGPERIPH_PEREN3_gt_clk_sysbus2mdm_END (15)
#define SOC_CRGPERIPH_PEREN3_gt_clk_pll_unlock_detect_acpu_START (16)
#define SOC_CRGPERIPH_PEREN3_gt_clk_pll_unlock_detect_acpu_END (16)
#define SOC_CRGPERIPH_PEREN3_gt_aclk_g3d_top_plus_START (18)
#define SOC_CRGPERIPH_PEREN3_gt_aclk_g3d_top_plus_END (18)
#define SOC_CRGPERIPH_PEREN3_gt_clk_isp_i2c_media_START (26)
#define SOC_CRGPERIPH_PEREN3_gt_clk_isp_i2c_media_END (26)
#define SOC_CRGPERIPH_PEREN3_gt_clk_dcdrbus_START (27)
#define SOC_CRGPERIPH_PEREN3_gt_clk_dcdrbus_END (27)
#define SOC_CRGPERIPH_PEREN3_gt_clk_txdphy0_cfg_START (28)
#define SOC_CRGPERIPH_PEREN3_gt_clk_txdphy0_cfg_END (28)
#define SOC_CRGPERIPH_PEREN3_gt_clk_txdphy0_ref_START (29)
#define SOC_CRGPERIPH_PEREN3_gt_clk_txdphy0_ref_END (29)
#define SOC_CRGPERIPH_PEREN3_gt_clk_txdphy1_cfg_START (30)
#define SOC_CRGPERIPH_PEREN3_gt_clk_txdphy1_cfg_END (30)
#define SOC_CRGPERIPH_PEREN3_gt_clk_txdphy1_ref_START (31)
#define SOC_CRGPERIPH_PEREN3_gt_clk_txdphy1_ref_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_aclk_dmac_lpm3 : 1;
        unsigned int gt_aclk_dmac_acpu : 1;
        unsigned int gt_aclk_dmac_mcpu : 1;
        unsigned int gt_aclk_g3d_cfg : 1;
        unsigned int gt_clk_g3dmt : 1;
        unsigned int gt_aclk_g3d_core : 1;
        unsigned int gt_clk_loadmonitor_2 : 1;
        unsigned int gt_pclk_loadmonitor_2 : 1;
        unsigned int gt_pclk_loadmonitor_2_lpmcu : 1;
        unsigned int gt_pclk_hsdt1_pcie : 1;
        unsigned int gt_clk_noc_mdm_mst : 1;
        unsigned int gt_clk_noc_mdm_cfg : 1;
        unsigned int gt_pclk_hsdt0_pcie : 1;
        unsigned int gt_clk_pll_unlock_detect : 1;
        unsigned int gt_clk_adb_mst_mdm2sys : 1;
        unsigned int gt_clk_sysbus2mdm : 1;
        unsigned int gt_clk_pll_unlock_detect_acpu : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_aclk_g3d_top_plus : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_clk_isp_i2c_media : 1;
        unsigned int gt_clk_dcdrbus : 1;
        unsigned int gt_clk_txdphy0_cfg : 1;
        unsigned int gt_clk_txdphy0_ref : 1;
        unsigned int gt_clk_txdphy1_cfg : 1;
        unsigned int gt_clk_txdphy1_ref : 1;
    } reg;
} SOC_CRGPERIPH_PERDIS3_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS3_gt_aclk_dmac_lpm3_START (0)
#define SOC_CRGPERIPH_PERDIS3_gt_aclk_dmac_lpm3_END (0)
#define SOC_CRGPERIPH_PERDIS3_gt_aclk_dmac_acpu_START (1)
#define SOC_CRGPERIPH_PERDIS3_gt_aclk_dmac_acpu_END (1)
#define SOC_CRGPERIPH_PERDIS3_gt_aclk_dmac_mcpu_START (2)
#define SOC_CRGPERIPH_PERDIS3_gt_aclk_dmac_mcpu_END (2)
#define SOC_CRGPERIPH_PERDIS3_gt_aclk_g3d_cfg_START (3)
#define SOC_CRGPERIPH_PERDIS3_gt_aclk_g3d_cfg_END (3)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_g3dmt_START (4)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_g3dmt_END (4)
#define SOC_CRGPERIPH_PERDIS3_gt_aclk_g3d_core_START (5)
#define SOC_CRGPERIPH_PERDIS3_gt_aclk_g3d_core_END (5)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_loadmonitor_2_START (6)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_loadmonitor_2_END (6)
#define SOC_CRGPERIPH_PERDIS3_gt_pclk_loadmonitor_2_START (7)
#define SOC_CRGPERIPH_PERDIS3_gt_pclk_loadmonitor_2_END (7)
#define SOC_CRGPERIPH_PERDIS3_gt_pclk_loadmonitor_2_lpmcu_START (8)
#define SOC_CRGPERIPH_PERDIS3_gt_pclk_loadmonitor_2_lpmcu_END (8)
#define SOC_CRGPERIPH_PERDIS3_gt_pclk_hsdt1_pcie_START (9)
#define SOC_CRGPERIPH_PERDIS3_gt_pclk_hsdt1_pcie_END (9)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_noc_mdm_mst_START (10)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_noc_mdm_mst_END (10)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_noc_mdm_cfg_START (11)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_noc_mdm_cfg_END (11)
#define SOC_CRGPERIPH_PERDIS3_gt_pclk_hsdt0_pcie_START (12)
#define SOC_CRGPERIPH_PERDIS3_gt_pclk_hsdt0_pcie_END (12)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_pll_unlock_detect_START (13)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_pll_unlock_detect_END (13)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_adb_mst_mdm2sys_START (14)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_adb_mst_mdm2sys_END (14)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_sysbus2mdm_START (15)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_sysbus2mdm_END (15)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_pll_unlock_detect_acpu_START (16)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_pll_unlock_detect_acpu_END (16)
#define SOC_CRGPERIPH_PERDIS3_gt_aclk_g3d_top_plus_START (18)
#define SOC_CRGPERIPH_PERDIS3_gt_aclk_g3d_top_plus_END (18)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_isp_i2c_media_START (26)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_isp_i2c_media_END (26)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_dcdrbus_START (27)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_dcdrbus_END (27)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_txdphy0_cfg_START (28)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_txdphy0_cfg_END (28)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_txdphy0_ref_START (29)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_txdphy0_ref_END (29)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_txdphy1_cfg_START (30)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_txdphy1_cfg_END (30)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_txdphy1_ref_START (31)
#define SOC_CRGPERIPH_PERDIS3_gt_clk_txdphy1_ref_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_aclk_dmac_lpm3 : 1;
        unsigned int gt_aclk_dmac_acpu : 1;
        unsigned int gt_aclk_dmac_mcpu : 1;
        unsigned int gt_aclk_g3d_cfg : 1;
        unsigned int gt_clk_g3dmt : 1;
        unsigned int gt_aclk_g3d_core : 1;
        unsigned int gt_clk_loadmonitor_2 : 1;
        unsigned int gt_pclk_loadmonitor_2 : 1;
        unsigned int gt_pclk_loadmonitor_2_lpmcu : 1;
        unsigned int gt_pclk_hsdt1_pcie : 1;
        unsigned int gt_clk_noc_mdm_mst : 1;
        unsigned int gt_clk_noc_mdm_cfg : 1;
        unsigned int gt_pclk_hsdt0_pcie : 1;
        unsigned int gt_clk_pll_unlock_detect : 1;
        unsigned int gt_clk_adb_mst_mdm2sys : 1;
        unsigned int gt_clk_sysbus2mdm : 1;
        unsigned int gt_clk_pll_unlock_detect_acpu : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_aclk_g3d_top_plus : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_clk_isp_i2c_media : 1;
        unsigned int gt_clk_dcdrbus : 1;
        unsigned int gt_clk_txdphy0_cfg : 1;
        unsigned int gt_clk_txdphy0_ref : 1;
        unsigned int gt_clk_txdphy1_cfg : 1;
        unsigned int gt_clk_txdphy1_ref : 1;
    } reg;
} SOC_CRGPERIPH_PERCLKEN3_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN3_gt_aclk_dmac_lpm3_START (0)
#define SOC_CRGPERIPH_PERCLKEN3_gt_aclk_dmac_lpm3_END (0)
#define SOC_CRGPERIPH_PERCLKEN3_gt_aclk_dmac_acpu_START (1)
#define SOC_CRGPERIPH_PERCLKEN3_gt_aclk_dmac_acpu_END (1)
#define SOC_CRGPERIPH_PERCLKEN3_gt_aclk_dmac_mcpu_START (2)
#define SOC_CRGPERIPH_PERCLKEN3_gt_aclk_dmac_mcpu_END (2)
#define SOC_CRGPERIPH_PERCLKEN3_gt_aclk_g3d_cfg_START (3)
#define SOC_CRGPERIPH_PERCLKEN3_gt_aclk_g3d_cfg_END (3)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_g3dmt_START (4)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_g3dmt_END (4)
#define SOC_CRGPERIPH_PERCLKEN3_gt_aclk_g3d_core_START (5)
#define SOC_CRGPERIPH_PERCLKEN3_gt_aclk_g3d_core_END (5)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_loadmonitor_2_START (6)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_loadmonitor_2_END (6)
#define SOC_CRGPERIPH_PERCLKEN3_gt_pclk_loadmonitor_2_START (7)
#define SOC_CRGPERIPH_PERCLKEN3_gt_pclk_loadmonitor_2_END (7)
#define SOC_CRGPERIPH_PERCLKEN3_gt_pclk_loadmonitor_2_lpmcu_START (8)
#define SOC_CRGPERIPH_PERCLKEN3_gt_pclk_loadmonitor_2_lpmcu_END (8)
#define SOC_CRGPERIPH_PERCLKEN3_gt_pclk_hsdt1_pcie_START (9)
#define SOC_CRGPERIPH_PERCLKEN3_gt_pclk_hsdt1_pcie_END (9)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_noc_mdm_mst_START (10)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_noc_mdm_mst_END (10)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_noc_mdm_cfg_START (11)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_noc_mdm_cfg_END (11)
#define SOC_CRGPERIPH_PERCLKEN3_gt_pclk_hsdt0_pcie_START (12)
#define SOC_CRGPERIPH_PERCLKEN3_gt_pclk_hsdt0_pcie_END (12)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_pll_unlock_detect_START (13)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_pll_unlock_detect_END (13)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_adb_mst_mdm2sys_START (14)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_adb_mst_mdm2sys_END (14)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_sysbus2mdm_START (15)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_sysbus2mdm_END (15)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_pll_unlock_detect_acpu_START (16)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_pll_unlock_detect_acpu_END (16)
#define SOC_CRGPERIPH_PERCLKEN3_gt_aclk_g3d_top_plus_START (18)
#define SOC_CRGPERIPH_PERCLKEN3_gt_aclk_g3d_top_plus_END (18)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_isp_i2c_media_START (26)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_isp_i2c_media_END (26)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_dcdrbus_START (27)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_dcdrbus_END (27)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_txdphy0_cfg_START (28)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_txdphy0_cfg_END (28)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_txdphy0_ref_START (29)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_txdphy0_ref_END (29)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_txdphy1_cfg_START (30)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_txdphy1_cfg_END (30)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_txdphy1_ref_START (31)
#define SOC_CRGPERIPH_PERCLKEN3_gt_clk_txdphy1_ref_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_dmac0 : 1;
        unsigned int st_clk_dmac1 : 1;
        unsigned int st_aclk_dmac : 1;
        unsigned int st_aclk_g3d_cfg : 1;
        unsigned int st_clk_g3dmt : 1;
        unsigned int st_aclk_g3d_core : 1;
        unsigned int st_clk_loadmonitor_2 : 1;
        unsigned int st_pclk_loadmonitor_2 : 1;
        unsigned int reserved_0 : 1;
        unsigned int st_pclk_hsdt1_pcie : 1;
        unsigned int st_clk_noc_mdm_mst : 1;
        unsigned int st_clk_noc_mdm_cfg : 1;
        unsigned int st_pclk_hsdt0_pcie : 1;
        unsigned int st_clk_pll_unlock_detect : 1;
        unsigned int st_clk_adb_mst_mdm2sys : 1;
        unsigned int st_clk_sysbus2mdm : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int st_aclk_g3d_top_plus : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int st_clk_isp_i2c_media : 1;
        unsigned int st_clk_dcdrbus : 1;
        unsigned int st_clk_txdphy0_cfg : 1;
        unsigned int st_clk_txdphy0_ref : 1;
        unsigned int st_clk_txdphy1_cfg : 1;
        unsigned int st_clk_txdphy1_ref : 1;
    } reg;
} SOC_CRGPERIPH_PERSTAT3_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT3_st_clk_dmac0_START (0)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_dmac0_END (0)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_dmac1_START (1)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_dmac1_END (1)
#define SOC_CRGPERIPH_PERSTAT3_st_aclk_dmac_START (2)
#define SOC_CRGPERIPH_PERSTAT3_st_aclk_dmac_END (2)
#define SOC_CRGPERIPH_PERSTAT3_st_aclk_g3d_cfg_START (3)
#define SOC_CRGPERIPH_PERSTAT3_st_aclk_g3d_cfg_END (3)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_g3dmt_START (4)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_g3dmt_END (4)
#define SOC_CRGPERIPH_PERSTAT3_st_aclk_g3d_core_START (5)
#define SOC_CRGPERIPH_PERSTAT3_st_aclk_g3d_core_END (5)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_loadmonitor_2_START (6)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_loadmonitor_2_END (6)
#define SOC_CRGPERIPH_PERSTAT3_st_pclk_loadmonitor_2_START (7)
#define SOC_CRGPERIPH_PERSTAT3_st_pclk_loadmonitor_2_END (7)
#define SOC_CRGPERIPH_PERSTAT3_st_pclk_hsdt1_pcie_START (9)
#define SOC_CRGPERIPH_PERSTAT3_st_pclk_hsdt1_pcie_END (9)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_noc_mdm_mst_START (10)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_noc_mdm_mst_END (10)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_noc_mdm_cfg_START (11)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_noc_mdm_cfg_END (11)
#define SOC_CRGPERIPH_PERSTAT3_st_pclk_hsdt0_pcie_START (12)
#define SOC_CRGPERIPH_PERSTAT3_st_pclk_hsdt0_pcie_END (12)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_pll_unlock_detect_START (13)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_pll_unlock_detect_END (13)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_adb_mst_mdm2sys_START (14)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_adb_mst_mdm2sys_END (14)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_sysbus2mdm_START (15)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_sysbus2mdm_END (15)
#define SOC_CRGPERIPH_PERSTAT3_st_aclk_g3d_top_plus_START (18)
#define SOC_CRGPERIPH_PERSTAT3_st_aclk_g3d_top_plus_END (18)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_isp_i2c_media_START (26)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_isp_i2c_media_END (26)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_dcdrbus_START (27)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_dcdrbus_END (27)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_txdphy0_cfg_START (28)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_txdphy0_cfg_END (28)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_txdphy0_ref_START (29)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_txdphy0_ref_END (29)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_txdphy1_cfg_START (30)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_txdphy1_cfg_END (30)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_txdphy1_ref_START (31)
#define SOC_CRGPERIPH_PERSTAT3_st_clk_txdphy1_ref_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_npu_monitor_sensor : 1;
        unsigned int gt_clk_hsdt1_usbdp : 1;
        unsigned int gt_clk_isp_snclk3 : 1;
        unsigned int gt_aclk_hsdt1_pcie1 : 1;
        unsigned int gt_clk_spi4 : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_media_tcxo : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_clk_perf_stat : 1;
        unsigned int gt_pclk_perf_stat : 1;
        unsigned int gt_aclk_perf_stat : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_clk_x2j_func_mbist : 1;
        unsigned int gt_pclk_ipc_fast0 : 1;
        unsigned int gt_pclk_ipc_fast1 : 1;
        unsigned int gt_clk_socp_hifi : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_sd : 1;
        unsigned int reserved_4 : 1;
        unsigned int gt_clk_ppll0_sscg : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_clk_ace0_mst_g3d : 1;
        unsigned int gt_clk_apll7_sscg : 1;
        unsigned int gt_clk_apll2_sscg : 1;
        unsigned int gt_clk_ppll1_sscg : 1;
        unsigned int gt_clk_ppll2_sscg : 1;
        unsigned int gt_clk_ppll3_sscg : 1;
        unsigned int gt_clk_ppll2_b_sscg : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
    } reg;
} SOC_CRGPERIPH_PEREN4_UNION;
#endif
#define SOC_CRGPERIPH_PEREN4_gt_clk_npu_monitor_sensor_START (0)
#define SOC_CRGPERIPH_PEREN4_gt_clk_npu_monitor_sensor_END (0)
#define SOC_CRGPERIPH_PEREN4_gt_clk_hsdt1_usbdp_START (1)
#define SOC_CRGPERIPH_PEREN4_gt_clk_hsdt1_usbdp_END (1)
#define SOC_CRGPERIPH_PEREN4_gt_clk_isp_snclk3_START (2)
#define SOC_CRGPERIPH_PEREN4_gt_clk_isp_snclk3_END (2)
#define SOC_CRGPERIPH_PEREN4_gt_aclk_hsdt1_pcie1_START (3)
#define SOC_CRGPERIPH_PEREN4_gt_aclk_hsdt1_pcie1_END (3)
#define SOC_CRGPERIPH_PEREN4_gt_clk_spi4_START (4)
#define SOC_CRGPERIPH_PEREN4_gt_clk_spi4_END (4)
#define SOC_CRGPERIPH_PEREN4_gt_clk_media_tcxo_START (6)
#define SOC_CRGPERIPH_PEREN4_gt_clk_media_tcxo_END (6)
#define SOC_CRGPERIPH_PEREN4_gt_clk_perf_stat_START (8)
#define SOC_CRGPERIPH_PEREN4_gt_clk_perf_stat_END (8)
#define SOC_CRGPERIPH_PEREN4_gt_pclk_perf_stat_START (9)
#define SOC_CRGPERIPH_PEREN4_gt_pclk_perf_stat_END (9)
#define SOC_CRGPERIPH_PEREN4_gt_aclk_perf_stat_START (10)
#define SOC_CRGPERIPH_PEREN4_gt_aclk_perf_stat_END (10)
#define SOC_CRGPERIPH_PEREN4_gt_clk_x2j_func_mbist_START (12)
#define SOC_CRGPERIPH_PEREN4_gt_clk_x2j_func_mbist_END (12)
#define SOC_CRGPERIPH_PEREN4_gt_pclk_ipc_fast0_START (13)
#define SOC_CRGPERIPH_PEREN4_gt_pclk_ipc_fast0_END (13)
#define SOC_CRGPERIPH_PEREN4_gt_pclk_ipc_fast1_START (14)
#define SOC_CRGPERIPH_PEREN4_gt_pclk_ipc_fast1_END (14)
#define SOC_CRGPERIPH_PEREN4_gt_clk_socp_hifi_START (15)
#define SOC_CRGPERIPH_PEREN4_gt_clk_socp_hifi_END (15)
#define SOC_CRGPERIPH_PEREN4_gt_clk_sd_START (17)
#define SOC_CRGPERIPH_PEREN4_gt_clk_sd_END (17)
#define SOC_CRGPERIPH_PEREN4_gt_clk_ppll0_sscg_START (19)
#define SOC_CRGPERIPH_PEREN4_gt_clk_ppll0_sscg_END (19)
#define SOC_CRGPERIPH_PEREN4_gt_clk_ace0_mst_g3d_START (23)
#define SOC_CRGPERIPH_PEREN4_gt_clk_ace0_mst_g3d_END (23)
#define SOC_CRGPERIPH_PEREN4_gt_clk_apll7_sscg_START (24)
#define SOC_CRGPERIPH_PEREN4_gt_clk_apll7_sscg_END (24)
#define SOC_CRGPERIPH_PEREN4_gt_clk_apll2_sscg_START (25)
#define SOC_CRGPERIPH_PEREN4_gt_clk_apll2_sscg_END (25)
#define SOC_CRGPERIPH_PEREN4_gt_clk_ppll1_sscg_START (26)
#define SOC_CRGPERIPH_PEREN4_gt_clk_ppll1_sscg_END (26)
#define SOC_CRGPERIPH_PEREN4_gt_clk_ppll2_sscg_START (27)
#define SOC_CRGPERIPH_PEREN4_gt_clk_ppll2_sscg_END (27)
#define SOC_CRGPERIPH_PEREN4_gt_clk_ppll3_sscg_START (28)
#define SOC_CRGPERIPH_PEREN4_gt_clk_ppll3_sscg_END (28)
#define SOC_CRGPERIPH_PEREN4_gt_clk_ppll2_b_sscg_START (29)
#define SOC_CRGPERIPH_PEREN4_gt_clk_ppll2_b_sscg_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_npu_monitor_sensor : 1;
        unsigned int gt_clk_hsdt1_usbdp : 1;
        unsigned int gt_clk_isp_snclk3 : 1;
        unsigned int gt_aclk_hsdt1_pcie1 : 1;
        unsigned int gt_clk_spi4 : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_media_tcxo : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_clk_perf_stat : 1;
        unsigned int gt_pclk_perf_stat : 1;
        unsigned int gt_aclk_perf_stat : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_clk_x2j_func_mbist : 1;
        unsigned int gt_pclk_ipc_fast0 : 1;
        unsigned int gt_pclk_ipc_fast1 : 1;
        unsigned int gt_clk_socp_hifi : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_sd : 1;
        unsigned int reserved_4 : 1;
        unsigned int gt_clk_ppll0_sscg : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_clk_ace0_mst_g3d : 1;
        unsigned int gt_clk_apll7_sscg : 1;
        unsigned int gt_clk_apll2_sscg : 1;
        unsigned int gt_clk_ppll1_sscg : 1;
        unsigned int gt_clk_ppll2_sscg : 1;
        unsigned int gt_clk_ppll3_sscg : 1;
        unsigned int gt_clk_ppll2_b_sscg : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
    } reg;
} SOC_CRGPERIPH_PERDIS4_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS4_gt_clk_npu_monitor_sensor_START (0)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_npu_monitor_sensor_END (0)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_hsdt1_usbdp_START (1)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_hsdt1_usbdp_END (1)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_isp_snclk3_START (2)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_isp_snclk3_END (2)
#define SOC_CRGPERIPH_PERDIS4_gt_aclk_hsdt1_pcie1_START (3)
#define SOC_CRGPERIPH_PERDIS4_gt_aclk_hsdt1_pcie1_END (3)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_spi4_START (4)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_spi4_END (4)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_media_tcxo_START (6)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_media_tcxo_END (6)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_perf_stat_START (8)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_perf_stat_END (8)
#define SOC_CRGPERIPH_PERDIS4_gt_pclk_perf_stat_START (9)
#define SOC_CRGPERIPH_PERDIS4_gt_pclk_perf_stat_END (9)
#define SOC_CRGPERIPH_PERDIS4_gt_aclk_perf_stat_START (10)
#define SOC_CRGPERIPH_PERDIS4_gt_aclk_perf_stat_END (10)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_x2j_func_mbist_START (12)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_x2j_func_mbist_END (12)
#define SOC_CRGPERIPH_PERDIS4_gt_pclk_ipc_fast0_START (13)
#define SOC_CRGPERIPH_PERDIS4_gt_pclk_ipc_fast0_END (13)
#define SOC_CRGPERIPH_PERDIS4_gt_pclk_ipc_fast1_START (14)
#define SOC_CRGPERIPH_PERDIS4_gt_pclk_ipc_fast1_END (14)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_socp_hifi_START (15)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_socp_hifi_END (15)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_sd_START (17)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_sd_END (17)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_ppll0_sscg_START (19)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_ppll0_sscg_END (19)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_ace0_mst_g3d_START (23)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_ace0_mst_g3d_END (23)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_apll7_sscg_START (24)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_apll7_sscg_END (24)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_apll2_sscg_START (25)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_apll2_sscg_END (25)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_ppll1_sscg_START (26)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_ppll1_sscg_END (26)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_ppll2_sscg_START (27)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_ppll2_sscg_END (27)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_ppll3_sscg_START (28)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_ppll3_sscg_END (28)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_ppll2_b_sscg_START (29)
#define SOC_CRGPERIPH_PERDIS4_gt_clk_ppll2_b_sscg_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_npu_monitor_sensor : 1;
        unsigned int gt_clk_hsdt1_usbdp : 1;
        unsigned int gt_clk_isp_snclk3 : 1;
        unsigned int gt_aclk_hsdt1_pcie1 : 1;
        unsigned int gt_clk_spi4 : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_media_tcxo : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_clk_perf_stat : 1;
        unsigned int gt_pclk_perf_stat : 1;
        unsigned int gt_aclk_perf_stat : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_clk_x2j_func_mbist : 1;
        unsigned int gt_pclk_ipc_fast0 : 1;
        unsigned int gt_pclk_ipc_fast1 : 1;
        unsigned int gt_clk_socp_hifi : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_sd : 1;
        unsigned int reserved_4 : 1;
        unsigned int gt_clk_ppll0_sscg : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_clk_ace0_mst_g3d : 1;
        unsigned int gt_clk_apll7_sscg : 1;
        unsigned int gt_clk_apll2_sscg : 1;
        unsigned int gt_clk_ppll1_sscg : 1;
        unsigned int gt_clk_ppll2_sscg : 1;
        unsigned int gt_clk_ppll3_sscg : 1;
        unsigned int gt_clk_ppll2_b_sscg : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
    } reg;
} SOC_CRGPERIPH_PERCLKEN4_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_npu_monitor_sensor_START (0)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_npu_monitor_sensor_END (0)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_hsdt1_usbdp_START (1)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_hsdt1_usbdp_END (1)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_isp_snclk3_START (2)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_isp_snclk3_END (2)
#define SOC_CRGPERIPH_PERCLKEN4_gt_aclk_hsdt1_pcie1_START (3)
#define SOC_CRGPERIPH_PERCLKEN4_gt_aclk_hsdt1_pcie1_END (3)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_spi4_START (4)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_spi4_END (4)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_media_tcxo_START (6)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_media_tcxo_END (6)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_perf_stat_START (8)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_perf_stat_END (8)
#define SOC_CRGPERIPH_PERCLKEN4_gt_pclk_perf_stat_START (9)
#define SOC_CRGPERIPH_PERCLKEN4_gt_pclk_perf_stat_END (9)
#define SOC_CRGPERIPH_PERCLKEN4_gt_aclk_perf_stat_START (10)
#define SOC_CRGPERIPH_PERCLKEN4_gt_aclk_perf_stat_END (10)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_x2j_func_mbist_START (12)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_x2j_func_mbist_END (12)
#define SOC_CRGPERIPH_PERCLKEN4_gt_pclk_ipc_fast0_START (13)
#define SOC_CRGPERIPH_PERCLKEN4_gt_pclk_ipc_fast0_END (13)
#define SOC_CRGPERIPH_PERCLKEN4_gt_pclk_ipc_fast1_START (14)
#define SOC_CRGPERIPH_PERCLKEN4_gt_pclk_ipc_fast1_END (14)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_socp_hifi_START (15)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_socp_hifi_END (15)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_sd_START (17)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_sd_END (17)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_ppll0_sscg_START (19)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_ppll0_sscg_END (19)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_ace0_mst_g3d_START (23)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_ace0_mst_g3d_END (23)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_apll7_sscg_START (24)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_apll7_sscg_END (24)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_apll2_sscg_START (25)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_apll2_sscg_END (25)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_ppll1_sscg_START (26)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_ppll1_sscg_END (26)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_ppll2_sscg_START (27)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_ppll2_sscg_END (27)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_ppll3_sscg_START (28)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_ppll3_sscg_END (28)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_ppll2_b_sscg_START (29)
#define SOC_CRGPERIPH_PERCLKEN4_gt_clk_ppll2_b_sscg_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_npu_monitor_sensor : 1;
        unsigned int st_clk_hsdt1_usbdp : 1;
        unsigned int st_clk_isp_snclk3 : 1;
        unsigned int st_aclk_hsdt1_pcie1 : 1;
        unsigned int st_clk_spi4 : 1;
        unsigned int reserved_0 : 1;
        unsigned int st_clk_media_tcxo : 1;
        unsigned int reserved_1 : 1;
        unsigned int st_clk_perf_stat : 1;
        unsigned int st_pclk_perf_stat : 1;
        unsigned int st_aclk_perf_stat : 1;
        unsigned int reserved_2 : 1;
        unsigned int st_clk_x2j_func_mbist : 1;
        unsigned int st_pclk_ipc_fast0 : 1;
        unsigned int st_pclk_ipc_fast1 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int st_clk_sd : 1;
        unsigned int st_clk_ace0_mst_g3d_rs_core : 1;
        unsigned int st_clk_ppll0_sscg : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int st_clk_ace0_mst_g3d : 1;
        unsigned int st_clk_apll7_sscg : 1;
        unsigned int st_clk_apll2_sscg : 1;
        unsigned int st_clk_ppll1_sscg : 1;
        unsigned int st_clk_ppll2_sscg : 1;
        unsigned int st_clk_ppll3_sscg : 1;
        unsigned int st_clk_ppll2_b_sscg : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
    } reg;
} SOC_CRGPERIPH_PERSTAT4_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT4_st_clk_npu_monitor_sensor_START (0)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_npu_monitor_sensor_END (0)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_hsdt1_usbdp_START (1)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_hsdt1_usbdp_END (1)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_isp_snclk3_START (2)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_isp_snclk3_END (2)
#define SOC_CRGPERIPH_PERSTAT4_st_aclk_hsdt1_pcie1_START (3)
#define SOC_CRGPERIPH_PERSTAT4_st_aclk_hsdt1_pcie1_END (3)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_spi4_START (4)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_spi4_END (4)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_media_tcxo_START (6)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_media_tcxo_END (6)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_perf_stat_START (8)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_perf_stat_END (8)
#define SOC_CRGPERIPH_PERSTAT4_st_pclk_perf_stat_START (9)
#define SOC_CRGPERIPH_PERSTAT4_st_pclk_perf_stat_END (9)
#define SOC_CRGPERIPH_PERSTAT4_st_aclk_perf_stat_START (10)
#define SOC_CRGPERIPH_PERSTAT4_st_aclk_perf_stat_END (10)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_x2j_func_mbist_START (12)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_x2j_func_mbist_END (12)
#define SOC_CRGPERIPH_PERSTAT4_st_pclk_ipc_fast0_START (13)
#define SOC_CRGPERIPH_PERSTAT4_st_pclk_ipc_fast0_END (13)
#define SOC_CRGPERIPH_PERSTAT4_st_pclk_ipc_fast1_START (14)
#define SOC_CRGPERIPH_PERSTAT4_st_pclk_ipc_fast1_END (14)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_sd_START (17)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_sd_END (17)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_ace0_mst_g3d_rs_core_START (18)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_ace0_mst_g3d_rs_core_END (18)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_ppll0_sscg_START (19)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_ppll0_sscg_END (19)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_ace0_mst_g3d_START (23)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_ace0_mst_g3d_END (23)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_apll7_sscg_START (24)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_apll7_sscg_END (24)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_apll2_sscg_START (25)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_apll2_sscg_END (25)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_ppll1_sscg_START (26)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_ppll1_sscg_END (26)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_ppll2_sscg_START (27)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_ppll2_sscg_END (27)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_ppll3_sscg_START (28)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_ppll3_sscg_END (28)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_ppll2_b_sscg_START (29)
#define SOC_CRGPERIPH_PERSTAT4_st_clk_ppll2_b_sscg_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_modem_tcxo : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_clk_core_crg_apb : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int gt_clk_lpmcu : 1;
        unsigned int gt_clk_ao_hifd : 1;
        unsigned int gt_clk_usb3otg_ref : 1;
        unsigned int gt_clk_aohpm : 1;
        unsigned int gt_clk_perihpm : 1;
        unsigned int gt_clk_fcm_monitor_sensor : 1;
        unsigned int gt_aclk_fcm : 1;
        unsigned int gt_clk_gpuhpm : 1;
        unsigned int gt_clk_isp_snclk0 : 1;
        unsigned int gt_clk_isp_snclk1 : 1;
        unsigned int gt_clk_isp_snclk2 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int gt_pclk_atgc1 : 1;
        unsigned int gt_pclk_atg_npu : 1;
        unsigned int gt_pclk_atgc : 1;
        unsigned int gt_pclk_atg_media : 1;
        unsigned int gt_pclk_atgs_mdm : 1;
        unsigned int gt_clk_system_cache_peri_cfg : 1;
        unsigned int gt_clk_system_cache_peri_cmo : 1;
        unsigned int gt_clk_vcodec_tcxo : 1;
        unsigned int gt_clk_latency_monitor : 1;
    } reg;
} SOC_CRGPERIPH_PEREN5_UNION;
#endif
#define SOC_CRGPERIPH_PEREN5_gt_clk_modem_tcxo_START (1)
#define SOC_CRGPERIPH_PEREN5_gt_clk_modem_tcxo_END (1)
#define SOC_CRGPERIPH_PEREN5_gt_clk_core_crg_apb_START (4)
#define SOC_CRGPERIPH_PEREN5_gt_clk_core_crg_apb_END (4)
#define SOC_CRGPERIPH_PEREN5_gt_clk_lpmcu_START (8)
#define SOC_CRGPERIPH_PEREN5_gt_clk_lpmcu_END (8)
#define SOC_CRGPERIPH_PEREN5_gt_clk_ao_hifd_START (9)
#define SOC_CRGPERIPH_PEREN5_gt_clk_ao_hifd_END (9)
#define SOC_CRGPERIPH_PEREN5_gt_clk_usb3otg_ref_START (10)
#define SOC_CRGPERIPH_PEREN5_gt_clk_usb3otg_ref_END (10)
#define SOC_CRGPERIPH_PEREN5_gt_clk_aohpm_START (11)
#define SOC_CRGPERIPH_PEREN5_gt_clk_aohpm_END (11)
#define SOC_CRGPERIPH_PEREN5_gt_clk_perihpm_START (12)
#define SOC_CRGPERIPH_PEREN5_gt_clk_perihpm_END (12)
#define SOC_CRGPERIPH_PEREN5_gt_clk_fcm_monitor_sensor_START (13)
#define SOC_CRGPERIPH_PEREN5_gt_clk_fcm_monitor_sensor_END (13)
#define SOC_CRGPERIPH_PEREN5_gt_aclk_fcm_START (14)
#define SOC_CRGPERIPH_PEREN5_gt_aclk_fcm_END (14)
#define SOC_CRGPERIPH_PEREN5_gt_clk_gpuhpm_START (15)
#define SOC_CRGPERIPH_PEREN5_gt_clk_gpuhpm_END (15)
#define SOC_CRGPERIPH_PEREN5_gt_clk_isp_snclk0_START (16)
#define SOC_CRGPERIPH_PEREN5_gt_clk_isp_snclk0_END (16)
#define SOC_CRGPERIPH_PEREN5_gt_clk_isp_snclk1_START (17)
#define SOC_CRGPERIPH_PEREN5_gt_clk_isp_snclk1_END (17)
#define SOC_CRGPERIPH_PEREN5_gt_clk_isp_snclk2_START (18)
#define SOC_CRGPERIPH_PEREN5_gt_clk_isp_snclk2_END (18)
#define SOC_CRGPERIPH_PEREN5_gt_pclk_atgc1_START (23)
#define SOC_CRGPERIPH_PEREN5_gt_pclk_atgc1_END (23)
#define SOC_CRGPERIPH_PEREN5_gt_pclk_atg_npu_START (24)
#define SOC_CRGPERIPH_PEREN5_gt_pclk_atg_npu_END (24)
#define SOC_CRGPERIPH_PEREN5_gt_pclk_atgc_START (25)
#define SOC_CRGPERIPH_PEREN5_gt_pclk_atgc_END (25)
#define SOC_CRGPERIPH_PEREN5_gt_pclk_atg_media_START (26)
#define SOC_CRGPERIPH_PEREN5_gt_pclk_atg_media_END (26)
#define SOC_CRGPERIPH_PEREN5_gt_pclk_atgs_mdm_START (27)
#define SOC_CRGPERIPH_PEREN5_gt_pclk_atgs_mdm_END (27)
#define SOC_CRGPERIPH_PEREN5_gt_clk_system_cache_peri_cfg_START (28)
#define SOC_CRGPERIPH_PEREN5_gt_clk_system_cache_peri_cfg_END (28)
#define SOC_CRGPERIPH_PEREN5_gt_clk_system_cache_peri_cmo_START (29)
#define SOC_CRGPERIPH_PEREN5_gt_clk_system_cache_peri_cmo_END (29)
#define SOC_CRGPERIPH_PEREN5_gt_clk_vcodec_tcxo_START (30)
#define SOC_CRGPERIPH_PEREN5_gt_clk_vcodec_tcxo_END (30)
#define SOC_CRGPERIPH_PEREN5_gt_clk_latency_monitor_START (31)
#define SOC_CRGPERIPH_PEREN5_gt_clk_latency_monitor_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_modem_tcxo : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_clk_core_crg_apb : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int gt_clk_lpmcu : 1;
        unsigned int gt_clk_ao_hifd : 1;
        unsigned int gt_clk_usb3otg_ref : 1;
        unsigned int gt_clk_aohpm : 1;
        unsigned int gt_clk_perihpm : 1;
        unsigned int gt_clk_fcm_monitor_sensor : 1;
        unsigned int gt_aclk_fcm : 1;
        unsigned int gt_clk_gpuhpm : 1;
        unsigned int gt_clk_isp_snclk0 : 1;
        unsigned int gt_clk_isp_snclk1 : 1;
        unsigned int gt_clk_isp_snclk2 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int gt_pclk_atgc1 : 1;
        unsigned int gt_pclk_atg_npu : 1;
        unsigned int gt_pclk_atgc : 1;
        unsigned int gt_pclk_atg_media : 1;
        unsigned int gt_pclk_atgs_mdm : 1;
        unsigned int gt_clk_system_cache_peri_cfg : 1;
        unsigned int gt_clk_system_cache_peri_cmo : 1;
        unsigned int gt_clk_vcodec_tcxo : 1;
        unsigned int gt_clk_latency_monitor : 1;
    } reg;
} SOC_CRGPERIPH_PERDIS5_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS5_gt_clk_modem_tcxo_START (1)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_modem_tcxo_END (1)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_core_crg_apb_START (4)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_core_crg_apb_END (4)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_lpmcu_START (8)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_lpmcu_END (8)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_ao_hifd_START (9)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_ao_hifd_END (9)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_usb3otg_ref_START (10)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_usb3otg_ref_END (10)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_aohpm_START (11)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_aohpm_END (11)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_perihpm_START (12)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_perihpm_END (12)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_fcm_monitor_sensor_START (13)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_fcm_monitor_sensor_END (13)
#define SOC_CRGPERIPH_PERDIS5_gt_aclk_fcm_START (14)
#define SOC_CRGPERIPH_PERDIS5_gt_aclk_fcm_END (14)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_gpuhpm_START (15)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_gpuhpm_END (15)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_isp_snclk0_START (16)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_isp_snclk0_END (16)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_isp_snclk1_START (17)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_isp_snclk1_END (17)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_isp_snclk2_START (18)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_isp_snclk2_END (18)
#define SOC_CRGPERIPH_PERDIS5_gt_pclk_atgc1_START (23)
#define SOC_CRGPERIPH_PERDIS5_gt_pclk_atgc1_END (23)
#define SOC_CRGPERIPH_PERDIS5_gt_pclk_atg_npu_START (24)
#define SOC_CRGPERIPH_PERDIS5_gt_pclk_atg_npu_END (24)
#define SOC_CRGPERIPH_PERDIS5_gt_pclk_atgc_START (25)
#define SOC_CRGPERIPH_PERDIS5_gt_pclk_atgc_END (25)
#define SOC_CRGPERIPH_PERDIS5_gt_pclk_atg_media_START (26)
#define SOC_CRGPERIPH_PERDIS5_gt_pclk_atg_media_END (26)
#define SOC_CRGPERIPH_PERDIS5_gt_pclk_atgs_mdm_START (27)
#define SOC_CRGPERIPH_PERDIS5_gt_pclk_atgs_mdm_END (27)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_system_cache_peri_cfg_START (28)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_system_cache_peri_cfg_END (28)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_system_cache_peri_cmo_START (29)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_system_cache_peri_cmo_END (29)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_vcodec_tcxo_START (30)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_vcodec_tcxo_END (30)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_latency_monitor_START (31)
#define SOC_CRGPERIPH_PERDIS5_gt_clk_latency_monitor_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_modem_tcxo : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_clk_core_crg_apb : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int gt_clk_lpmcu : 1;
        unsigned int gt_clk_ao_hifd : 1;
        unsigned int gt_clk_usb3otg_ref : 1;
        unsigned int gt_clk_aohpm : 1;
        unsigned int gt_clk_perihpm : 1;
        unsigned int gt_clk_fcm_monitor_sensor : 1;
        unsigned int gt_aclk_fcm : 1;
        unsigned int gt_clk_gpuhpm : 1;
        unsigned int gt_clk_isp_snclk0 : 1;
        unsigned int gt_clk_isp_snclk1 : 1;
        unsigned int gt_clk_isp_snclk2 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int gt_pclk_atgc1 : 1;
        unsigned int gt_pclk_atg_npu : 1;
        unsigned int gt_pclk_atgc : 1;
        unsigned int gt_pclk_atg_media : 1;
        unsigned int gt_pclk_atgs_mdm : 1;
        unsigned int gt_clk_system_cache_peri_cfg : 1;
        unsigned int gt_clk_system_cache_peri_cmo : 1;
        unsigned int gt_clk_vcodec_tcxo : 1;
        unsigned int gt_clk_latency_monitor : 1;
    } reg;
} SOC_CRGPERIPH_PERCLKEN5_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_modem_tcxo_START (1)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_modem_tcxo_END (1)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_core_crg_apb_START (4)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_core_crg_apb_END (4)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_lpmcu_START (8)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_lpmcu_END (8)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_ao_hifd_START (9)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_ao_hifd_END (9)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_usb3otg_ref_START (10)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_usb3otg_ref_END (10)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_aohpm_START (11)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_aohpm_END (11)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_perihpm_START (12)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_perihpm_END (12)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_fcm_monitor_sensor_START (13)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_fcm_monitor_sensor_END (13)
#define SOC_CRGPERIPH_PERCLKEN5_gt_aclk_fcm_START (14)
#define SOC_CRGPERIPH_PERCLKEN5_gt_aclk_fcm_END (14)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_gpuhpm_START (15)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_gpuhpm_END (15)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_isp_snclk0_START (16)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_isp_snclk0_END (16)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_isp_snclk1_START (17)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_isp_snclk1_END (17)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_isp_snclk2_START (18)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_isp_snclk2_END (18)
#define SOC_CRGPERIPH_PERCLKEN5_gt_pclk_atgc1_START (23)
#define SOC_CRGPERIPH_PERCLKEN5_gt_pclk_atgc1_END (23)
#define SOC_CRGPERIPH_PERCLKEN5_gt_pclk_atg_npu_START (24)
#define SOC_CRGPERIPH_PERCLKEN5_gt_pclk_atg_npu_END (24)
#define SOC_CRGPERIPH_PERCLKEN5_gt_pclk_atgc_START (25)
#define SOC_CRGPERIPH_PERCLKEN5_gt_pclk_atgc_END (25)
#define SOC_CRGPERIPH_PERCLKEN5_gt_pclk_atg_media_START (26)
#define SOC_CRGPERIPH_PERCLKEN5_gt_pclk_atg_media_END (26)
#define SOC_CRGPERIPH_PERCLKEN5_gt_pclk_atgs_mdm_START (27)
#define SOC_CRGPERIPH_PERCLKEN5_gt_pclk_atgs_mdm_END (27)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_system_cache_peri_cfg_START (28)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_system_cache_peri_cfg_END (28)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_system_cache_peri_cmo_START (29)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_system_cache_peri_cmo_END (29)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_vcodec_tcxo_START (30)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_vcodec_tcxo_END (30)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_latency_monitor_START (31)
#define SOC_CRGPERIPH_PERCLKEN5_gt_clk_latency_monitor_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int st_clk_modem_tcxo : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int st_clk_core_crg_apb : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int st_clk_lpmcu : 1;
        unsigned int st_clk_ao_hifd : 1;
        unsigned int st_clk_usb3otg_ref : 1;
        unsigned int st_clk_aohpm : 1;
        unsigned int st_clk_perihpm : 1;
        unsigned int st_clk_fcm_monitor_sensor : 1;
        unsigned int reserved_6 : 1;
        unsigned int st_clk_gpuhpm : 1;
        unsigned int st_clk_isp_snclk0 : 1;
        unsigned int st_clk_isp_snclk1 : 1;
        unsigned int st_clk_isp_snclk2 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int st_pclk_atgc1 : 1;
        unsigned int st_pclk_atg_npu : 1;
        unsigned int st_pclk_atgc : 1;
        unsigned int st_pclk_atg_media : 1;
        unsigned int st_pclk_atgs_mdm : 1;
        unsigned int st_clk_system_cache_peri_cfg : 1;
        unsigned int st_clk_system_cache_peri_cmo : 1;
        unsigned int st_clk_vcodec_tcxo : 1;
        unsigned int st_clk_latency_monitor : 1;
    } reg;
} SOC_CRGPERIPH_PERSTAT5_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT5_st_clk_modem_tcxo_START (1)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_modem_tcxo_END (1)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_core_crg_apb_START (4)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_core_crg_apb_END (4)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_lpmcu_START (8)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_lpmcu_END (8)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_ao_hifd_START (9)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_ao_hifd_END (9)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_usb3otg_ref_START (10)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_usb3otg_ref_END (10)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_aohpm_START (11)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_aohpm_END (11)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_perihpm_START (12)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_perihpm_END (12)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_fcm_monitor_sensor_START (13)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_fcm_monitor_sensor_END (13)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_gpuhpm_START (15)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_gpuhpm_END (15)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_isp_snclk0_START (16)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_isp_snclk0_END (16)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_isp_snclk1_START (17)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_isp_snclk1_END (17)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_isp_snclk2_START (18)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_isp_snclk2_END (18)
#define SOC_CRGPERIPH_PERSTAT5_st_pclk_atgc1_START (23)
#define SOC_CRGPERIPH_PERSTAT5_st_pclk_atgc1_END (23)
#define SOC_CRGPERIPH_PERSTAT5_st_pclk_atg_npu_START (24)
#define SOC_CRGPERIPH_PERSTAT5_st_pclk_atg_npu_END (24)
#define SOC_CRGPERIPH_PERSTAT5_st_pclk_atgc_START (25)
#define SOC_CRGPERIPH_PERSTAT5_st_pclk_atgc_END (25)
#define SOC_CRGPERIPH_PERSTAT5_st_pclk_atg_media_START (26)
#define SOC_CRGPERIPH_PERSTAT5_st_pclk_atg_media_END (26)
#define SOC_CRGPERIPH_PERSTAT5_st_pclk_atgs_mdm_START (27)
#define SOC_CRGPERIPH_PERSTAT5_st_pclk_atgs_mdm_END (27)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_system_cache_peri_cfg_START (28)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_system_cache_peri_cfg_END (28)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_system_cache_peri_cmo_START (29)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_system_cache_peri_cmo_END (29)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_vcodec_tcxo_START (30)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_vcodec_tcxo_END (30)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_latency_monitor_START (31)
#define SOC_CRGPERIPH_PERSTAT5_st_clk_latency_monitor_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_noc_sysbus2ddrc : 1;
        unsigned int ip_rst_exmbist : 1;
        unsigned int ip_rst_adb_mst_mdm2sys : 1;
        unsigned int ip_rst_hsdtbus : 1;
        unsigned int ip_rst_peri_dmc : 1;
        unsigned int ip_rst_peri_dmc_crg : 1;
        unsigned int ip_rst_sysbus : 1;
        unsigned int ip_rst_cfgbus : 1;
        unsigned int ip_rst_timerperi : 1;
        unsigned int ip_rst_hsdt_crg : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_gpu_crg : 1;
        unsigned int ip_rst_mddrc_crg : 1;
        unsigned int ip_rst_i2c7 : 1;
        unsigned int ip_rst_spi4 : 1;
        unsigned int ip_rst_system_cache_core : 1;
        unsigned int ip_rst_syscache_crg : 1;
        unsigned int ip_rst_perf_stat : 1;
        unsigned int ip_rst_i2c6 : 1;
        unsigned int ip_rst_lpmcu2cfgbus : 1;
        unsigned int ip_rst_noc_dmabus : 1;
        unsigned int ip_rst_noc_hsdt1bus : 1;
        unsigned int ip_rst_noc_mdm_cfg : 1;
        unsigned int ip_rst_noc_mdm_mst : 1;
        unsigned int ip_rst_svfd_cpm_l3 : 1;
        unsigned int ip_rst_svfd_cpm_middle0 : 1;
        unsigned int ip_rst_svfd_ffs_l3 : 1;
        unsigned int ip_rst_svfd_ffs_middle0 : 1;
        unsigned int ip_rst_adb0_mst_npu : 1;
        unsigned int ip_rst_system_cache_cfg : 1;
        unsigned int ip_rst_noise_print : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTEN0_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_noc_sysbus2ddrc_START (1)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_noc_sysbus2ddrc_END (1)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_exmbist_START (2)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_exmbist_END (2)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_adb_mst_mdm2sys_START (3)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_adb_mst_mdm2sys_END (3)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_hsdtbus_START (4)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_hsdtbus_END (4)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_peri_dmc_START (5)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_peri_dmc_END (5)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_peri_dmc_crg_START (6)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_peri_dmc_crg_END (6)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_sysbus_START (7)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_sysbus_END (7)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_cfgbus_START (8)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_cfgbus_END (8)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_timerperi_START (9)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_timerperi_END (9)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_hsdt_crg_START (10)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_hsdt_crg_END (10)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_gpu_crg_START (12)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_gpu_crg_END (12)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_mddrc_crg_START (13)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_mddrc_crg_END (13)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_i2c7_START (14)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_i2c7_END (14)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_spi4_START (15)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_spi4_END (15)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_system_cache_core_START (16)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_system_cache_core_END (16)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_syscache_crg_START (17)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_syscache_crg_END (17)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_perf_stat_START (18)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_perf_stat_END (18)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_i2c6_START (19)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_i2c6_END (19)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_lpmcu2cfgbus_START (20)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_lpmcu2cfgbus_END (20)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_noc_dmabus_START (21)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_noc_dmabus_END (21)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_noc_hsdt1bus_START (22)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_noc_hsdt1bus_END (22)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_noc_mdm_cfg_START (23)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_noc_mdm_cfg_END (23)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_noc_mdm_mst_START (24)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_noc_mdm_mst_END (24)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_svfd_cpm_l3_START (25)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_svfd_cpm_l3_END (25)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_svfd_cpm_middle0_START (26)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_svfd_cpm_middle0_END (26)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_svfd_ffs_l3_START (27)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_svfd_ffs_l3_END (27)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_svfd_ffs_middle0_START (28)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_svfd_ffs_middle0_END (28)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_adb0_mst_npu_START (29)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_adb0_mst_npu_END (29)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_system_cache_cfg_START (30)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_system_cache_cfg_END (30)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_noise_print_START (31)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_noise_print_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_noc_sysbus2ddrc : 1;
        unsigned int ip_rst_exmbist : 1;
        unsigned int ip_rst_adb_mst_mdm2sys : 1;
        unsigned int ip_rst_hsdtbus : 1;
        unsigned int ip_rst_peri_dmc : 1;
        unsigned int ip_rst_peri_dmc_crg : 1;
        unsigned int ip_rst_sysbus : 1;
        unsigned int ip_rst_cfgbus : 1;
        unsigned int ip_rst_timerperi : 1;
        unsigned int ip_rst_hsdt_crg : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_gpu_crg : 1;
        unsigned int ip_rst_mddrc_crg : 1;
        unsigned int ip_rst_i2c7 : 1;
        unsigned int ip_rst_spi4 : 1;
        unsigned int ip_rst_system_cache_core : 1;
        unsigned int ip_rst_syscache_crg : 1;
        unsigned int ip_rst_perf_stat : 1;
        unsigned int ip_rst_i2c6 : 1;
        unsigned int ip_rst_lpmcu2cfgbus : 1;
        unsigned int ip_rst_noc_dmabus : 1;
        unsigned int ip_rst_noc_hsdt1bus : 1;
        unsigned int ip_rst_noc_mdm_cfg : 1;
        unsigned int ip_rst_noc_mdm_mst : 1;
        unsigned int ip_rst_svfd_cpm_l3 : 1;
        unsigned int ip_rst_svfd_cpm_middle0 : 1;
        unsigned int ip_rst_svfd_ffs_l3 : 1;
        unsigned int ip_rst_svfd_ffs_middle0 : 1;
        unsigned int ip_rst_adb0_mst_npu : 1;
        unsigned int ip_rst_system_cache_cfg : 1;
        unsigned int ip_rst_noise_print : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTDIS0_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_noc_sysbus2ddrc_START (1)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_noc_sysbus2ddrc_END (1)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_exmbist_START (2)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_exmbist_END (2)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_adb_mst_mdm2sys_START (3)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_adb_mst_mdm2sys_END (3)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_hsdtbus_START (4)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_hsdtbus_END (4)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_peri_dmc_START (5)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_peri_dmc_END (5)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_peri_dmc_crg_START (6)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_peri_dmc_crg_END (6)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_sysbus_START (7)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_sysbus_END (7)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_cfgbus_START (8)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_cfgbus_END (8)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_timerperi_START (9)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_timerperi_END (9)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_hsdt_crg_START (10)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_hsdt_crg_END (10)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_gpu_crg_START (12)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_gpu_crg_END (12)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_mddrc_crg_START (13)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_mddrc_crg_END (13)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_i2c7_START (14)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_i2c7_END (14)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_spi4_START (15)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_spi4_END (15)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_system_cache_core_START (16)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_system_cache_core_END (16)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_syscache_crg_START (17)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_syscache_crg_END (17)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_perf_stat_START (18)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_perf_stat_END (18)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_i2c6_START (19)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_i2c6_END (19)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_lpmcu2cfgbus_START (20)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_lpmcu2cfgbus_END (20)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_noc_dmabus_START (21)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_noc_dmabus_END (21)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_noc_hsdt1bus_START (22)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_noc_hsdt1bus_END (22)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_noc_mdm_cfg_START (23)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_noc_mdm_cfg_END (23)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_noc_mdm_mst_START (24)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_noc_mdm_mst_END (24)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_svfd_cpm_l3_START (25)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_svfd_cpm_l3_END (25)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_svfd_cpm_middle0_START (26)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_svfd_cpm_middle0_END (26)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_svfd_ffs_l3_START (27)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_svfd_ffs_l3_END (27)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_svfd_ffs_middle0_START (28)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_svfd_ffs_middle0_END (28)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_adb0_mst_npu_START (29)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_adb0_mst_npu_END (29)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_system_cache_cfg_START (30)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_system_cache_cfg_END (30)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_noise_print_START (31)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_noise_print_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_noc_sysbus2ddrc : 1;
        unsigned int ip_rst_exmbist : 1;
        unsigned int ip_rst_adb_mst_mdm2sys : 1;
        unsigned int ip_rst_hsdtbus : 1;
        unsigned int ip_rst_peri_dmc : 1;
        unsigned int ip_rst_peri_dmc_crg : 1;
        unsigned int ip_rst_sysbus : 1;
        unsigned int ip_rst_cfgbus : 1;
        unsigned int ip_rst_timerperi : 1;
        unsigned int ip_rst_hsdt_crg : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_gpu_crg : 1;
        unsigned int ip_rst_mddrc_crg : 1;
        unsigned int ip_rst_i2c7 : 1;
        unsigned int ip_rst_spi4 : 1;
        unsigned int ip_rst_system_cache_core : 1;
        unsigned int ip_rst_syscache_crg : 1;
        unsigned int ip_rst_perf_stat : 1;
        unsigned int ip_rst_i2c6 : 1;
        unsigned int ip_rst_lpmcu2cfgbus : 1;
        unsigned int ip_rst_noc_dmabus : 1;
        unsigned int ip_rst_noc_hsdt1bus : 1;
        unsigned int ip_rst_noc_mdm_cfg : 1;
        unsigned int ip_rst_noc_mdm_mst : 1;
        unsigned int ip_rst_svfd_cpm_l3 : 1;
        unsigned int ip_rst_svfd_cpm_middle0 : 1;
        unsigned int ip_rst_svfd_ffs_l3 : 1;
        unsigned int ip_rst_svfd_ffs_middle0 : 1;
        unsigned int ip_rst_adb0_mst_npu : 1;
        unsigned int ip_rst_system_cache_cfg : 1;
        unsigned int ip_rst_noise_print : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTSTAT0_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_noc_sysbus2ddrc_START (1)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_noc_sysbus2ddrc_END (1)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_exmbist_START (2)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_exmbist_END (2)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_adb_mst_mdm2sys_START (3)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_adb_mst_mdm2sys_END (3)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_hsdtbus_START (4)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_hsdtbus_END (4)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_peri_dmc_START (5)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_peri_dmc_END (5)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_peri_dmc_crg_START (6)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_peri_dmc_crg_END (6)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_sysbus_START (7)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_sysbus_END (7)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_cfgbus_START (8)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_cfgbus_END (8)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_timerperi_START (9)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_timerperi_END (9)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_hsdt_crg_START (10)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_hsdt_crg_END (10)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_gpu_crg_START (12)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_gpu_crg_END (12)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_mddrc_crg_START (13)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_mddrc_crg_END (13)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_i2c7_START (14)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_i2c7_END (14)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_spi4_START (15)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_spi4_END (15)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_system_cache_core_START (16)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_system_cache_core_END (16)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_syscache_crg_START (17)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_syscache_crg_END (17)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_perf_stat_START (18)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_perf_stat_END (18)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_i2c6_START (19)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_i2c6_END (19)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_lpmcu2cfgbus_START (20)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_lpmcu2cfgbus_END (20)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_noc_dmabus_START (21)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_noc_dmabus_END (21)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_noc_hsdt1bus_START (22)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_noc_hsdt1bus_END (22)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_noc_mdm_cfg_START (23)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_noc_mdm_cfg_END (23)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_noc_mdm_mst_START (24)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_noc_mdm_mst_END (24)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_svfd_cpm_l3_START (25)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_svfd_cpm_l3_END (25)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_svfd_cpm_middle0_START (26)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_svfd_cpm_middle0_END (26)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_svfd_ffs_l3_START (27)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_svfd_ffs_l3_END (27)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_svfd_ffs_middle0_START (28)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_svfd_ffs_middle0_END (28)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_adb0_mst_npu_START (29)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_adb0_mst_npu_END (29)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_system_cache_cfg_START (30)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_system_cache_cfg_END (30)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_noise_print_START (31)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_noise_print_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_prst_gpio0 : 1;
        unsigned int ip_prst_gpio1 : 1;
        unsigned int ip_prst_gpio2 : 1;
        unsigned int ip_prst_gpio3 : 1;
        unsigned int ip_prst_gpio4 : 1;
        unsigned int ip_prst_gpio5 : 1;
        unsigned int ip_prst_gpio6 : 1;
        unsigned int ip_prst_gpio7 : 1;
        unsigned int ip_prst_gpio8 : 1;
        unsigned int ip_prst_gpio9 : 1;
        unsigned int ip_prst_gpio10 : 1;
        unsigned int ip_prst_gpio11 : 1;
        unsigned int ip_prst_gpio12 : 1;
        unsigned int ip_prst_gpio13 : 1;
        unsigned int ip_prst_gpio14 : 1;
        unsigned int ip_prst_gpio15 : 1;
        unsigned int ip_prst_gpio16 : 1;
        unsigned int ip_prst_gpio17 : 1;
        unsigned int ip_rst_vsensorc_l3 : 1;
        unsigned int ip_rst_vsensorc_middle0 : 1;
        unsigned int ip_prst_gpio18 : 1;
        unsigned int ip_prst_gpio19 : 1;
        unsigned int ip_prst_timer9 : 1;
        unsigned int ip_prst_timer10 : 1;
        unsigned int ip_prst_timer11 : 1;
        unsigned int ip_prst_timer12 : 1;
        unsigned int ip_rst_socp : 1;
        unsigned int ip_rst_djtag : 1;
        unsigned int ip_rst_vsensorc_big0 : 1;
        unsigned int ip_rst_vsensorc_little : 1;
        unsigned int ip_rst_vsensorc_gpu_core : 1;
        unsigned int reserved : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTEN1_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio0_START (0)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio0_END (0)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio1_START (1)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio1_END (1)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio2_START (2)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio2_END (2)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio3_START (3)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio3_END (3)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio4_START (4)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio4_END (4)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio5_START (5)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio5_END (5)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio6_START (6)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio6_END (6)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio7_START (7)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio7_END (7)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio8_START (8)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio8_END (8)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio9_START (9)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio9_END (9)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio10_START (10)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio10_END (10)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio11_START (11)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio11_END (11)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio12_START (12)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio12_END (12)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio13_START (13)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio13_END (13)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio14_START (14)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio14_END (14)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio15_START (15)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio15_END (15)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio16_START (16)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio16_END (16)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio17_START (17)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio17_END (17)
#define SOC_CRGPERIPH_PERRSTEN1_ip_rst_vsensorc_l3_START (18)
#define SOC_CRGPERIPH_PERRSTEN1_ip_rst_vsensorc_l3_END (18)
#define SOC_CRGPERIPH_PERRSTEN1_ip_rst_vsensorc_middle0_START (19)
#define SOC_CRGPERIPH_PERRSTEN1_ip_rst_vsensorc_middle0_END (19)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio18_START (20)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio18_END (20)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio19_START (21)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_gpio19_END (21)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_timer9_START (22)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_timer9_END (22)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_timer10_START (23)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_timer10_END (23)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_timer11_START (24)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_timer11_END (24)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_timer12_START (25)
#define SOC_CRGPERIPH_PERRSTEN1_ip_prst_timer12_END (25)
#define SOC_CRGPERIPH_PERRSTEN1_ip_rst_socp_START (26)
#define SOC_CRGPERIPH_PERRSTEN1_ip_rst_socp_END (26)
#define SOC_CRGPERIPH_PERRSTEN1_ip_rst_djtag_START (27)
#define SOC_CRGPERIPH_PERRSTEN1_ip_rst_djtag_END (27)
#define SOC_CRGPERIPH_PERRSTEN1_ip_rst_vsensorc_big0_START (28)
#define SOC_CRGPERIPH_PERRSTEN1_ip_rst_vsensorc_big0_END (28)
#define SOC_CRGPERIPH_PERRSTEN1_ip_rst_vsensorc_little_START (29)
#define SOC_CRGPERIPH_PERRSTEN1_ip_rst_vsensorc_little_END (29)
#define SOC_CRGPERIPH_PERRSTEN1_ip_rst_vsensorc_gpu_core_START (30)
#define SOC_CRGPERIPH_PERRSTEN1_ip_rst_vsensorc_gpu_core_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_prst_gpio0 : 1;
        unsigned int ip_prst_gpio1 : 1;
        unsigned int ip_prst_gpio2 : 1;
        unsigned int ip_prst_gpio3 : 1;
        unsigned int ip_prst_gpio4 : 1;
        unsigned int ip_prst_gpio5 : 1;
        unsigned int ip_prst_gpio6 : 1;
        unsigned int ip_prst_gpio7 : 1;
        unsigned int ip_prst_gpio8 : 1;
        unsigned int ip_prst_gpio9 : 1;
        unsigned int ip_prst_gpio10 : 1;
        unsigned int ip_prst_gpio11 : 1;
        unsigned int ip_prst_gpio12 : 1;
        unsigned int ip_prst_gpio13 : 1;
        unsigned int ip_prst_gpio14 : 1;
        unsigned int ip_prst_gpio15 : 1;
        unsigned int ip_prst_gpio16 : 1;
        unsigned int ip_prst_gpio17 : 1;
        unsigned int ip_rst_vsensorc_l3 : 1;
        unsigned int ip_rst_vsensorc_middle0 : 1;
        unsigned int ip_prst_gpio18 : 1;
        unsigned int ip_prst_gpio19 : 1;
        unsigned int ip_prst_timer9 : 1;
        unsigned int ip_prst_timer10 : 1;
        unsigned int ip_prst_timer11 : 1;
        unsigned int ip_prst_timer12 : 1;
        unsigned int ip_rst_socp : 1;
        unsigned int ip_rst_djtag : 1;
        unsigned int ip_rst_vsensorc_big0 : 1;
        unsigned int ip_rst_vsensorc_little : 1;
        unsigned int ip_rst_vsensorc_gpu_core : 1;
        unsigned int reserved : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTDIS1_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio0_START (0)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio0_END (0)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio1_START (1)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio1_END (1)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio2_START (2)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio2_END (2)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio3_START (3)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio3_END (3)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio4_START (4)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio4_END (4)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio5_START (5)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio5_END (5)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio6_START (6)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio6_END (6)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio7_START (7)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio7_END (7)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio8_START (8)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio8_END (8)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio9_START (9)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio9_END (9)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio10_START (10)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio10_END (10)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio11_START (11)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio11_END (11)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio12_START (12)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio12_END (12)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio13_START (13)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio13_END (13)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio14_START (14)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio14_END (14)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio15_START (15)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio15_END (15)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio16_START (16)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio16_END (16)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio17_START (17)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio17_END (17)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_rst_vsensorc_l3_START (18)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_rst_vsensorc_l3_END (18)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_rst_vsensorc_middle0_START (19)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_rst_vsensorc_middle0_END (19)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio18_START (20)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio18_END (20)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio19_START (21)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_gpio19_END (21)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_timer9_START (22)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_timer9_END (22)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_timer10_START (23)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_timer10_END (23)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_timer11_START (24)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_timer11_END (24)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_timer12_START (25)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_prst_timer12_END (25)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_rst_socp_START (26)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_rst_socp_END (26)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_rst_djtag_START (27)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_rst_djtag_END (27)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_rst_vsensorc_big0_START (28)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_rst_vsensorc_big0_END (28)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_rst_vsensorc_little_START (29)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_rst_vsensorc_little_END (29)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_rst_vsensorc_gpu_core_START (30)
#define SOC_CRGPERIPH_PERRSTDIS1_ip_rst_vsensorc_gpu_core_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_prst_gpio0 : 1;
        unsigned int ip_prst_gpio1 : 1;
        unsigned int ip_prst_gpio2 : 1;
        unsigned int ip_prst_gpio3 : 1;
        unsigned int ip_prst_gpio4 : 1;
        unsigned int ip_prst_gpio5 : 1;
        unsigned int ip_prst_gpio6 : 1;
        unsigned int ip_prst_gpio7 : 1;
        unsigned int ip_prst_gpio8 : 1;
        unsigned int ip_prst_gpio9 : 1;
        unsigned int ip_prst_gpio10 : 1;
        unsigned int ip_prst_gpio11 : 1;
        unsigned int ip_prst_gpio12 : 1;
        unsigned int ip_prst_gpio13 : 1;
        unsigned int ip_prst_gpio14 : 1;
        unsigned int ip_prst_gpio15 : 1;
        unsigned int ip_prst_gpio16 : 1;
        unsigned int ip_prst_gpio17 : 1;
        unsigned int ip_rst_vsensorc_l3 : 1;
        unsigned int ip_rst_vsensorc_middle0 : 1;
        unsigned int ip_prst_gpio18 : 1;
        unsigned int ip_prst_gpio19 : 1;
        unsigned int ip_prst_timer9 : 1;
        unsigned int ip_prst_timer10 : 1;
        unsigned int ip_prst_timer11 : 1;
        unsigned int ip_prst_timer12 : 1;
        unsigned int ip_rst_socp : 1;
        unsigned int ip_rst_djtag : 1;
        unsigned int ip_rst_vsensorc_big0 : 1;
        unsigned int ip_rst_vsensorc_little : 1;
        unsigned int ip_rst_vsensorc_gpu_core : 1;
        unsigned int reserved : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTSTAT1_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio0_START (0)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio0_END (0)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio1_START (1)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio1_END (1)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio2_START (2)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio2_END (2)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio3_START (3)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio3_END (3)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio4_START (4)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio4_END (4)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio5_START (5)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio5_END (5)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio6_START (6)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio6_END (6)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio7_START (7)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio7_END (7)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio8_START (8)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio8_END (8)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio9_START (9)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio9_END (9)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio10_START (10)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio10_END (10)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio11_START (11)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio11_END (11)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio12_START (12)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio12_END (12)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio13_START (13)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio13_END (13)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio14_START (14)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio14_END (14)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio15_START (15)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio15_END (15)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio16_START (16)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio16_END (16)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio17_START (17)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio17_END (17)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_rst_vsensorc_l3_START (18)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_rst_vsensorc_l3_END (18)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_rst_vsensorc_middle0_START (19)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_rst_vsensorc_middle0_END (19)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio18_START (20)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio18_END (20)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio19_START (21)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_gpio19_END (21)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_timer9_START (22)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_timer9_END (22)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_timer10_START (23)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_timer10_END (23)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_timer11_START (24)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_timer11_END (24)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_timer12_START (25)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_prst_timer12_END (25)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_rst_socp_START (26)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_rst_socp_END (26)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_rst_djtag_START (27)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_rst_djtag_END (27)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_rst_vsensorc_big0_START (28)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_rst_vsensorc_big0_END (28)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_rst_vsensorc_little_START (29)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_rst_vsensorc_little_END (29)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_rst_vsensorc_gpu_core_START (30)
#define SOC_CRGPERIPH_PERRSTSTAT1_ip_rst_vsensorc_gpu_core_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_prst_pwm : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_prst_ipc0 : 1;
        unsigned int ip_prst_ipc1 : 1;
        unsigned int ip_rst_peri_pd : 1;
        unsigned int ip_rst_pericrg_pd : 1;
        unsigned int ip_rst_i2c2 : 1;
        unsigned int ip_rst_i2c3 : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_spi1 : 1;
        unsigned int ip_prst_uart0 : 1;
        unsigned int ip_prst_uart1 : 1;
        unsigned int ip_prst_uart2 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_prst_uart4 : 1;
        unsigned int ip_prst_uart5 : 1;
        unsigned int ip_prst_tzpc : 1;
        unsigned int ip_rst_peripcr : 1;
        unsigned int ip_prst_peripcr : 1;
        unsigned int ip_rst_adb_slv_acp : 1;
        unsigned int ip_prst_ipc_mdm : 1;
        unsigned int ip_rst_adb_mst_fcm0 : 1;
        unsigned int ip_rst_adb_mst_fcm1 : 1;
        unsigned int ip_rst_gic : 1;
        unsigned int reserved_3 : 1;
        unsigned int ip_prst_ioc : 1;
        unsigned int ip_rst_codecssi : 1;
        unsigned int ip_rst_i2c4 : 1;
        unsigned int ip_rst_adb_mst_mdm : 1;
        unsigned int ip_rst_hsdt1_subsys_crg : 1;
        unsigned int ip_rst_hsdt1_subsys : 1;
        unsigned int ip_prst_pctrl : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTEN2_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_pwm_START (0)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_pwm_END (0)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_ipc0_START (2)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_ipc0_END (2)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_ipc1_START (3)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_ipc1_END (3)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_peri_pd_START (4)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_peri_pd_END (4)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_pericrg_pd_START (5)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_pericrg_pd_END (5)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_i2c2_START (6)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_i2c2_END (6)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_i2c3_START (7)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_i2c3_END (7)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_spi1_START (9)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_spi1_END (9)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_uart0_START (10)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_uart0_END (10)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_uart1_START (11)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_uart1_END (11)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_uart2_START (12)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_uart2_END (12)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_uart4_START (14)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_uart4_END (14)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_uart5_START (15)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_uart5_END (15)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_tzpc_START (16)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_tzpc_END (16)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_peripcr_START (17)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_peripcr_END (17)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_peripcr_START (18)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_peripcr_END (18)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_adb_slv_acp_START (19)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_adb_slv_acp_END (19)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_ipc_mdm_START (20)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_ipc_mdm_END (20)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_adb_mst_fcm0_START (21)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_adb_mst_fcm0_END (21)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_adb_mst_fcm1_START (22)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_adb_mst_fcm1_END (22)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_gic_START (23)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_gic_END (23)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_ioc_START (25)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_ioc_END (25)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_codecssi_START (26)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_codecssi_END (26)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_i2c4_START (27)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_i2c4_END (27)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_adb_mst_mdm_START (28)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_adb_mst_mdm_END (28)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_hsdt1_subsys_crg_START (29)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_hsdt1_subsys_crg_END (29)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_hsdt1_subsys_START (30)
#define SOC_CRGPERIPH_PERRSTEN2_ip_rst_hsdt1_subsys_END (30)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_pctrl_START (31)
#define SOC_CRGPERIPH_PERRSTEN2_ip_prst_pctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_prst_pwm : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_prst_ipc0 : 1;
        unsigned int ip_prst_ipc1 : 1;
        unsigned int ip_rst_peri_pd : 1;
        unsigned int ip_rst_pericrg_pd : 1;
        unsigned int ip_rst_i2c2 : 1;
        unsigned int ip_rst_i2c3 : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_spi1 : 1;
        unsigned int ip_prst_uart0 : 1;
        unsigned int ip_prst_uart1 : 1;
        unsigned int ip_prst_uart2 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_prst_uart4 : 1;
        unsigned int ip_prst_uart5 : 1;
        unsigned int ip_prst_tzpc : 1;
        unsigned int ip_rst_peripcr : 1;
        unsigned int ip_prst_peripcr : 1;
        unsigned int ip_rst_adb_slv_acp : 1;
        unsigned int ip_prst_ipc_mdm : 1;
        unsigned int ip_rst_adb_mst_fcm0 : 1;
        unsigned int ip_rst_adb_mst_fcm1 : 1;
        unsigned int ip_rst_gic : 1;
        unsigned int reserved_3 : 1;
        unsigned int ip_prst_ioc : 1;
        unsigned int ip_rst_codecssi : 1;
        unsigned int ip_rst_i2c4 : 1;
        unsigned int ip_rst_adb_mst_mdm : 1;
        unsigned int ip_rst_hsdt1_subsys_crg : 1;
        unsigned int ip_rst_hsdt1_subsys : 1;
        unsigned int ip_prst_pctrl : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTDIS2_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_pwm_START (0)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_pwm_END (0)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_ipc0_START (2)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_ipc0_END (2)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_ipc1_START (3)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_ipc1_END (3)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_peri_pd_START (4)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_peri_pd_END (4)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_pericrg_pd_START (5)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_pericrg_pd_END (5)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_i2c2_START (6)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_i2c2_END (6)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_i2c3_START (7)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_i2c3_END (7)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_spi1_START (9)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_spi1_END (9)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_uart0_START (10)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_uart0_END (10)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_uart1_START (11)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_uart1_END (11)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_uart2_START (12)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_uart2_END (12)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_uart4_START (14)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_uart4_END (14)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_uart5_START (15)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_uart5_END (15)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_tzpc_START (16)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_tzpc_END (16)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_peripcr_START (17)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_peripcr_END (17)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_peripcr_START (18)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_peripcr_END (18)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_adb_slv_acp_START (19)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_adb_slv_acp_END (19)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_ipc_mdm_START (20)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_ipc_mdm_END (20)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_adb_mst_fcm0_START (21)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_adb_mst_fcm0_END (21)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_adb_mst_fcm1_START (22)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_adb_mst_fcm1_END (22)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_gic_START (23)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_gic_END (23)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_ioc_START (25)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_ioc_END (25)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_codecssi_START (26)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_codecssi_END (26)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_i2c4_START (27)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_i2c4_END (27)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_adb_mst_mdm_START (28)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_adb_mst_mdm_END (28)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_hsdt1_subsys_crg_START (29)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_hsdt1_subsys_crg_END (29)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_hsdt1_subsys_START (30)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_rst_hsdt1_subsys_END (30)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_pctrl_START (31)
#define SOC_CRGPERIPH_PERRSTDIS2_ip_prst_pctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_prst_pwm : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_prst_ipc0 : 1;
        unsigned int ip_prst_ipc1 : 1;
        unsigned int ip_rst_peri_pd : 1;
        unsigned int ip_rst_pericrg_pd : 1;
        unsigned int ip_rst_i2c2 : 1;
        unsigned int ip_rst_i2c3 : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_spi1 : 1;
        unsigned int ip_prst_uart0 : 1;
        unsigned int ip_prst_uart1 : 1;
        unsigned int ip_prst_uart2 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_prst_uart4 : 1;
        unsigned int ip_prst_uart5 : 1;
        unsigned int ip_prst_tzpc : 1;
        unsigned int ip_rst_peripcr : 1;
        unsigned int ip_prst_peripcr : 1;
        unsigned int ip_rst_adb_slv_acp : 1;
        unsigned int ip_prst_ipc_mdm : 1;
        unsigned int ip_rst_adb_mst_fcm0 : 1;
        unsigned int ip_rst_adb_mst_fcm1 : 1;
        unsigned int ip_rst_gic : 1;
        unsigned int reserved_3 : 1;
        unsigned int ip_prst_ioc : 1;
        unsigned int ip_rst_codecssi : 1;
        unsigned int ip_rst_i2c4 : 1;
        unsigned int ip_rst_adb_mst_mdm : 1;
        unsigned int ip_rst_hsdt1_subsys_crg : 1;
        unsigned int ip_rst_hsdt1_subsys : 1;
        unsigned int ip_prst_pctrl : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTSTAT2_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_pwm_START (0)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_pwm_END (0)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_ipc0_START (2)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_ipc0_END (2)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_ipc1_START (3)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_ipc1_END (3)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_peri_pd_START (4)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_peri_pd_END (4)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_pericrg_pd_START (5)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_pericrg_pd_END (5)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_i2c2_START (6)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_i2c2_END (6)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_i2c3_START (7)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_i2c3_END (7)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_spi1_START (9)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_spi1_END (9)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_uart0_START (10)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_uart0_END (10)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_uart1_START (11)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_uart1_END (11)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_uart2_START (12)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_uart2_END (12)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_uart4_START (14)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_uart4_END (14)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_uart5_START (15)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_uart5_END (15)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_tzpc_START (16)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_tzpc_END (16)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_peripcr_START (17)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_peripcr_END (17)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_peripcr_START (18)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_peripcr_END (18)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_adb_slv_acp_START (19)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_adb_slv_acp_END (19)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_ipc_mdm_START (20)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_ipc_mdm_END (20)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_adb_mst_fcm0_START (21)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_adb_mst_fcm0_END (21)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_adb_mst_fcm1_START (22)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_adb_mst_fcm1_END (22)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_gic_START (23)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_gic_END (23)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_ioc_START (25)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_ioc_END (25)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_codecssi_START (26)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_codecssi_END (26)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_i2c4_START (27)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_i2c4_END (27)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_adb_mst_mdm_START (28)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_adb_mst_mdm_END (28)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_hsdt1_subsys_crg_START (29)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_hsdt1_subsys_crg_END (29)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_hsdt1_subsys_START (30)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_rst_hsdt1_subsys_END (30)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_pctrl_START (31)
#define SOC_CRGPERIPH_PERRSTSTAT2_ip_prst_pctrl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_arst_dmac : 1;
        unsigned int ip_prst_loadmonitor : 1;
        unsigned int ip_arst_g3d_cfg : 1;
        unsigned int ip_arst_g3d : 1;
        unsigned int ip_rst_g3dmt : 1;
        unsigned int ip_arst_g3d_top_plus : 1;
        unsigned int ip_prst_loadmonitor_1 : 1;
        unsigned int ip_prst_loadmonitor_2 : 1;
        unsigned int ip_rst_g3dshpm15 : 1;
        unsigned int ip_rst_g3dshpm14 : 1;
        unsigned int ip_rst_g3dshpm13 : 1;
        unsigned int ip_rst_g3dshpm12 : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_g3dshpm8 : 1;
        unsigned int ip_rst_g3dshpm7 : 1;
        unsigned int ip_rst_g3dshpm6 : 1;
        unsigned int ip_rst_g3dshpm5 : 1;
        unsigned int ip_rst_g3dshpm4 : 1;
        unsigned int ip_rst_g3dshpm3 : 1;
        unsigned int ip_rst_g3dshpm2 : 1;
        unsigned int ip_rst_g3dshpm1 : 1;
        unsigned int ip_rst_g3dshpm0 : 1;
        unsigned int ip_rst_g3dghpm : 1;
        unsigned int ip_rst_perihpm : 1;
        unsigned int ip_rst_aohpm : 1;
        unsigned int ip_rst_ipf : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_prst_dsi0 : 1;
        unsigned int ip_prst_dsi1 : 1;
        unsigned int ip_rst_fcmpcr : 1;
        unsigned int reserved_3 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTEN3_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTEN3_ip_arst_dmac_START (0)
#define SOC_CRGPERIPH_PERRSTEN3_ip_arst_dmac_END (0)
#define SOC_CRGPERIPH_PERRSTEN3_ip_prst_loadmonitor_START (1)
#define SOC_CRGPERIPH_PERRSTEN3_ip_prst_loadmonitor_END (1)
#define SOC_CRGPERIPH_PERRSTEN3_ip_arst_g3d_cfg_START (2)
#define SOC_CRGPERIPH_PERRSTEN3_ip_arst_g3d_cfg_END (2)
#define SOC_CRGPERIPH_PERRSTEN3_ip_arst_g3d_START (3)
#define SOC_CRGPERIPH_PERRSTEN3_ip_arst_g3d_END (3)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dmt_START (4)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dmt_END (4)
#define SOC_CRGPERIPH_PERRSTEN3_ip_arst_g3d_top_plus_START (5)
#define SOC_CRGPERIPH_PERRSTEN3_ip_arst_g3d_top_plus_END (5)
#define SOC_CRGPERIPH_PERRSTEN3_ip_prst_loadmonitor_1_START (6)
#define SOC_CRGPERIPH_PERRSTEN3_ip_prst_loadmonitor_1_END (6)
#define SOC_CRGPERIPH_PERRSTEN3_ip_prst_loadmonitor_2_START (7)
#define SOC_CRGPERIPH_PERRSTEN3_ip_prst_loadmonitor_2_END (7)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm15_START (8)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm15_END (8)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm14_START (9)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm14_END (9)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm13_START (10)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm13_END (10)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm12_START (11)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm12_END (11)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm8_START (13)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm8_END (13)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm7_START (14)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm7_END (14)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm6_START (15)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm6_END (15)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm5_START (16)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm5_END (16)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm4_START (17)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm4_END (17)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm3_START (18)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm3_END (18)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm2_START (19)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm2_END (19)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm1_START (20)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm1_END (20)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm0_START (21)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dshpm0_END (21)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dghpm_START (22)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_g3dghpm_END (22)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_perihpm_START (23)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_perihpm_END (23)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_aohpm_START (24)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_aohpm_END (24)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_ipf_START (25)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_ipf_END (25)
#define SOC_CRGPERIPH_PERRSTEN3_ip_prst_dsi0_START (28)
#define SOC_CRGPERIPH_PERRSTEN3_ip_prst_dsi0_END (28)
#define SOC_CRGPERIPH_PERRSTEN3_ip_prst_dsi1_START (29)
#define SOC_CRGPERIPH_PERRSTEN3_ip_prst_dsi1_END (29)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_fcmpcr_START (30)
#define SOC_CRGPERIPH_PERRSTEN3_ip_rst_fcmpcr_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_arst_dmac : 1;
        unsigned int ip_prst_loadmonitor : 1;
        unsigned int ip_arst_g3d_cfg : 1;
        unsigned int ip_arst_g3d : 1;
        unsigned int ip_rst_g3dmt : 1;
        unsigned int ip_arst_g3d_top_plus : 1;
        unsigned int ip_prst_loadmonitor_1 : 1;
        unsigned int ip_prst_loadmonitor_2 : 1;
        unsigned int ip_rst_g3dshpm15 : 1;
        unsigned int ip_rst_g3dshpm14 : 1;
        unsigned int ip_rst_g3dshpm13 : 1;
        unsigned int ip_rst_g3dshpm12 : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_g3dshpm8 : 1;
        unsigned int ip_rst_g3dshpm7 : 1;
        unsigned int ip_rst_g3dshpm6 : 1;
        unsigned int ip_rst_g3dshpm5 : 1;
        unsigned int ip_rst_g3dshpm4 : 1;
        unsigned int ip_rst_g3dshpm3 : 1;
        unsigned int ip_rst_g3dshpm2 : 1;
        unsigned int ip_rst_g3dshpm1 : 1;
        unsigned int ip_rst_g3dshpm0 : 1;
        unsigned int ip_rst_g3dghpm : 1;
        unsigned int ip_rst_perihpm : 1;
        unsigned int ip_rst_aohpm : 1;
        unsigned int ip_rst_ipf : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_prst_dsi0 : 1;
        unsigned int ip_prst_dsi1 : 1;
        unsigned int ip_rst_fcmpcr : 1;
        unsigned int reserved_3 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTDIS3_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTDIS3_ip_arst_dmac_START (0)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_arst_dmac_END (0)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_prst_loadmonitor_START (1)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_prst_loadmonitor_END (1)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_arst_g3d_cfg_START (2)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_arst_g3d_cfg_END (2)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_arst_g3d_START (3)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_arst_g3d_END (3)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dmt_START (4)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dmt_END (4)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_arst_g3d_top_plus_START (5)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_arst_g3d_top_plus_END (5)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_prst_loadmonitor_1_START (6)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_prst_loadmonitor_1_END (6)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_prst_loadmonitor_2_START (7)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_prst_loadmonitor_2_END (7)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm15_START (8)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm15_END (8)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm14_START (9)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm14_END (9)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm13_START (10)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm13_END (10)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm12_START (11)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm12_END (11)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm8_START (13)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm8_END (13)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm7_START (14)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm7_END (14)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm6_START (15)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm6_END (15)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm5_START (16)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm5_END (16)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm4_START (17)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm4_END (17)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm3_START (18)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm3_END (18)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm2_START (19)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm2_END (19)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm1_START (20)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm1_END (20)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm0_START (21)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dshpm0_END (21)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dghpm_START (22)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_g3dghpm_END (22)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_perihpm_START (23)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_perihpm_END (23)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_aohpm_START (24)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_aohpm_END (24)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_ipf_START (25)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_ipf_END (25)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_prst_dsi0_START (28)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_prst_dsi0_END (28)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_prst_dsi1_START (29)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_prst_dsi1_END (29)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_fcmpcr_START (30)
#define SOC_CRGPERIPH_PERRSTDIS3_ip_rst_fcmpcr_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_arst_dmac : 1;
        unsigned int ip_prst_loadmonitor : 1;
        unsigned int ip_arst_g3d_cfg : 1;
        unsigned int ip_arst_g3d : 1;
        unsigned int ip_rst_g3dmt : 1;
        unsigned int ip_arst_g3d_top_plus : 1;
        unsigned int ip_prst_loadmonitor_1 : 1;
        unsigned int ip_prst_loadmonitor_2 : 1;
        unsigned int ip_rst_g3dshpm15 : 1;
        unsigned int ip_rst_g3dshpm14 : 1;
        unsigned int ip_rst_g3dshpm13 : 1;
        unsigned int ip_rst_g3dshpm12 : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_g3dshpm8 : 1;
        unsigned int ip_rst_g3dshpm7 : 1;
        unsigned int ip_rst_g3dshpm6 : 1;
        unsigned int ip_rst_g3dshpm5 : 1;
        unsigned int ip_rst_g3dshpm4 : 1;
        unsigned int ip_rst_g3dshpm3 : 1;
        unsigned int ip_rst_g3dshpm2 : 1;
        unsigned int ip_rst_g3dshpm1 : 1;
        unsigned int ip_rst_g3dshpm0 : 1;
        unsigned int ip_rst_g3dghpm : 1;
        unsigned int ip_rst_perihpm : 1;
        unsigned int ip_rst_aohpm : 1;
        unsigned int ip_rst_ipf : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_prst_dsi0 : 1;
        unsigned int ip_prst_dsi1 : 1;
        unsigned int ip_rst_fcmpcr : 1;
        unsigned int reserved_3 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTSTAT3_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_arst_dmac_START (0)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_arst_dmac_END (0)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_prst_loadmonitor_START (1)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_prst_loadmonitor_END (1)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_arst_g3d_cfg_START (2)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_arst_g3d_cfg_END (2)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_arst_g3d_START (3)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_arst_g3d_END (3)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dmt_START (4)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dmt_END (4)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_arst_g3d_top_plus_START (5)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_arst_g3d_top_plus_END (5)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_prst_loadmonitor_1_START (6)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_prst_loadmonitor_1_END (6)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_prst_loadmonitor_2_START (7)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_prst_loadmonitor_2_END (7)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm15_START (8)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm15_END (8)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm14_START (9)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm14_END (9)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm13_START (10)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm13_END (10)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm12_START (11)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm12_END (11)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm8_START (13)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm8_END (13)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm7_START (14)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm7_END (14)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm6_START (15)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm6_END (15)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm5_START (16)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm5_END (16)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm4_START (17)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm4_END (17)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm3_START (18)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm3_END (18)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm2_START (19)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm2_END (19)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm1_START (20)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm1_END (20)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm0_START (21)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dshpm0_END (21)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dghpm_START (22)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_g3dghpm_END (22)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_perihpm_START (23)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_perihpm_END (23)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_aohpm_START (24)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_aohpm_END (24)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_ipf_START (25)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_ipf_END (25)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_prst_dsi0_START (28)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_prst_dsi0_END (28)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_prst_dsi1_START (29)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_prst_dsi1_END (29)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_fcmpcr_START (30)
#define SOC_CRGPERIPH_PERRSTSTAT3_ip_rst_fcmpcr_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_media2 : 1;
        unsigned int ip_rst_media2_crg : 1;
        unsigned int ip_rst_lpm_cpu_big : 1;
        unsigned int ip_rst_lpm_cpu_middle : 1;
        unsigned int ip_rst_lpm_cpu_little : 1;
        unsigned int ip_rst_tidm_core : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_noc_mdm5g_cfg : 1;
        unsigned int ip_rst_pa_gpu : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_tidm_gpu : 1;
        unsigned int ip_prst_atgc : 1;
        unsigned int ip_rst_x2j_func_mbist : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_svfd_ffs_big0_mdm : 1;
        unsigned int ip_rst_svfd_ffs_middle0_mdm : 1;
        unsigned int ip_rst_svfd_cpm_big0_mdm : 1;
        unsigned int ip_rst_svfd_cpm_middle0_mdm : 1;
        unsigned int ip_prst_atgc1 : 1;
        unsigned int ip_prst_atgs_sys : 1;
        unsigned int ip_hrst_spe : 1;
        unsigned int ip_rst_spe : 1;
        unsigned int ip_rst_core_l_l3_crg : 1;
        unsigned int ip_rst_core_b_m_crg : 1;
        unsigned int ip_rst_i3c4 : 1;
        unsigned int ip_arst_axi_mem : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int ip_rst_lpmcu : 1;
        unsigned int ip_arst_maa : 1;
        unsigned int ip_rst_latency_monitor : 1;
        unsigned int reserved_5 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTEN4_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_media2_START (0)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_media2_END (0)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_media2_crg_START (1)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_media2_crg_END (1)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_lpm_cpu_big_START (2)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_lpm_cpu_big_END (2)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_lpm_cpu_middle_START (3)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_lpm_cpu_middle_END (3)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_lpm_cpu_little_START (4)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_lpm_cpu_little_END (4)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_tidm_core_START (5)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_tidm_core_END (5)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_noc_mdm5g_cfg_START (7)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_noc_mdm5g_cfg_END (7)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_pa_gpu_START (8)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_pa_gpu_END (8)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_tidm_gpu_START (10)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_tidm_gpu_END (10)
#define SOC_CRGPERIPH_PERRSTEN4_ip_prst_atgc_START (11)
#define SOC_CRGPERIPH_PERRSTEN4_ip_prst_atgc_END (11)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_x2j_func_mbist_START (12)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_x2j_func_mbist_END (12)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_svfd_ffs_big0_mdm_START (14)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_svfd_ffs_big0_mdm_END (14)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_svfd_ffs_middle0_mdm_START (15)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_svfd_ffs_middle0_mdm_END (15)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_svfd_cpm_big0_mdm_START (16)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_svfd_cpm_big0_mdm_END (16)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_svfd_cpm_middle0_mdm_START (17)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_svfd_cpm_middle0_mdm_END (17)
#define SOC_CRGPERIPH_PERRSTEN4_ip_prst_atgc1_START (18)
#define SOC_CRGPERIPH_PERRSTEN4_ip_prst_atgc1_END (18)
#define SOC_CRGPERIPH_PERRSTEN4_ip_prst_atgs_sys_START (19)
#define SOC_CRGPERIPH_PERRSTEN4_ip_prst_atgs_sys_END (19)
#define SOC_CRGPERIPH_PERRSTEN4_ip_hrst_spe_START (20)
#define SOC_CRGPERIPH_PERRSTEN4_ip_hrst_spe_END (20)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_spe_START (21)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_spe_END (21)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_core_l_l3_crg_START (22)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_core_l_l3_crg_END (22)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_core_b_m_crg_START (23)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_core_b_m_crg_END (23)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_i3c4_START (24)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_i3c4_END (24)
#define SOC_CRGPERIPH_PERRSTEN4_ip_arst_axi_mem_START (25)
#define SOC_CRGPERIPH_PERRSTEN4_ip_arst_axi_mem_END (25)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_lpmcu_START (28)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_lpmcu_END (28)
#define SOC_CRGPERIPH_PERRSTEN4_ip_arst_maa_START (29)
#define SOC_CRGPERIPH_PERRSTEN4_ip_arst_maa_END (29)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_latency_monitor_START (30)
#define SOC_CRGPERIPH_PERRSTEN4_ip_rst_latency_monitor_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_media2 : 1;
        unsigned int ip_rst_media2_crg : 1;
        unsigned int ip_rst_lpm_cpu_big : 1;
        unsigned int ip_rst_lpm_cpu_middle : 1;
        unsigned int ip_rst_lpm_cpu_little : 1;
        unsigned int ip_rst_tidm_core : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_noc_mdm5g_cfg : 1;
        unsigned int ip_rst_pa_gpu : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_tidm_gpu : 1;
        unsigned int ip_prst_atgc : 1;
        unsigned int ip_rst_x2j_func_mbist : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_svfd_ffs_big0_mdm : 1;
        unsigned int ip_rst_svfd_ffs_middle0_mdm : 1;
        unsigned int ip_rst_svfd_cpm_big0_mdm : 1;
        unsigned int ip_rst_svfd_cpm_middle0_mdm : 1;
        unsigned int ip_prst_atgc1 : 1;
        unsigned int ip_prst_atgs_sys : 1;
        unsigned int ip_hrst_spe : 1;
        unsigned int ip_rst_spe : 1;
        unsigned int ip_rst_core_l_l3_crg : 1;
        unsigned int ip_rst_core_b_m_crg : 1;
        unsigned int ip_rst_i3c4 : 1;
        unsigned int ip_arst_axi_mem : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int ip_rst_lpmcu : 1;
        unsigned int ip_arst_maa : 1;
        unsigned int ip_rst_latency_monitor : 1;
        unsigned int reserved_5 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTDIS4_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_media2_START (0)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_media2_END (0)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_media2_crg_START (1)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_media2_crg_END (1)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_lpm_cpu_big_START (2)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_lpm_cpu_big_END (2)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_lpm_cpu_middle_START (3)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_lpm_cpu_middle_END (3)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_lpm_cpu_little_START (4)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_lpm_cpu_little_END (4)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_tidm_core_START (5)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_tidm_core_END (5)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_noc_mdm5g_cfg_START (7)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_noc_mdm5g_cfg_END (7)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_pa_gpu_START (8)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_pa_gpu_END (8)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_tidm_gpu_START (10)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_tidm_gpu_END (10)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_prst_atgc_START (11)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_prst_atgc_END (11)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_x2j_func_mbist_START (12)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_x2j_func_mbist_END (12)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_svfd_ffs_big0_mdm_START (14)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_svfd_ffs_big0_mdm_END (14)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_svfd_ffs_middle0_mdm_START (15)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_svfd_ffs_middle0_mdm_END (15)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_svfd_cpm_big0_mdm_START (16)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_svfd_cpm_big0_mdm_END (16)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_svfd_cpm_middle0_mdm_START (17)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_svfd_cpm_middle0_mdm_END (17)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_prst_atgc1_START (18)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_prst_atgc1_END (18)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_prst_atgs_sys_START (19)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_prst_atgs_sys_END (19)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_hrst_spe_START (20)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_hrst_spe_END (20)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_spe_START (21)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_spe_END (21)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_core_l_l3_crg_START (22)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_core_l_l3_crg_END (22)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_core_b_m_crg_START (23)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_core_b_m_crg_END (23)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_i3c4_START (24)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_i3c4_END (24)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_arst_axi_mem_START (25)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_arst_axi_mem_END (25)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_lpmcu_START (28)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_lpmcu_END (28)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_arst_maa_START (29)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_arst_maa_END (29)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_latency_monitor_START (30)
#define SOC_CRGPERIPH_PERRSTDIS4_ip_rst_latency_monitor_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_media2 : 1;
        unsigned int ip_rst_media2_crg : 1;
        unsigned int ip_rst_lpm_cpu_big : 1;
        unsigned int ip_rst_lpm_cpu_middle : 1;
        unsigned int ip_rst_lpm_cpu_little : 1;
        unsigned int ip_rst_tidm_core : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_noc_mdm5g_cfg : 1;
        unsigned int ip_rst_pa_gpu : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_tidm_gpu : 1;
        unsigned int ip_prst_atgc : 1;
        unsigned int ip_rst_x2j_func_mbist : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_svfd_ffs_big0_mdm : 1;
        unsigned int ip_rst_svfd_ffs_middle0_mdm : 1;
        unsigned int ip_rst_svfd_cpm_big0_mdm : 1;
        unsigned int ip_rst_svfd_cpm_middle0_mdm : 1;
        unsigned int ip_prst_atgc1 : 1;
        unsigned int ip_prst_atgs_sys : 1;
        unsigned int ip_hrst_spe : 1;
        unsigned int ip_rst_spe : 1;
        unsigned int ip_rst_core_l_l3_crg : 1;
        unsigned int ip_rst_core_b_m_crg : 1;
        unsigned int ip_rst_i3c4 : 1;
        unsigned int ip_arst_axi_mem : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int ip_rst_lpmcu : 1;
        unsigned int ip_arst_maa : 1;
        unsigned int ip_rst_latency_monitor : 1;
        unsigned int reserved_5 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTSTAT4_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_media2_START (0)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_media2_END (0)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_media2_crg_START (1)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_media2_crg_END (1)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_lpm_cpu_big_START (2)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_lpm_cpu_big_END (2)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_lpm_cpu_middle_START (3)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_lpm_cpu_middle_END (3)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_lpm_cpu_little_START (4)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_lpm_cpu_little_END (4)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_tidm_core_START (5)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_tidm_core_END (5)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_noc_mdm5g_cfg_START (7)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_noc_mdm5g_cfg_END (7)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_pa_gpu_START (8)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_pa_gpu_END (8)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_tidm_gpu_START (10)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_tidm_gpu_END (10)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_prst_atgc_START (11)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_prst_atgc_END (11)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_x2j_func_mbist_START (12)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_x2j_func_mbist_END (12)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_svfd_ffs_big0_mdm_START (14)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_svfd_ffs_big0_mdm_END (14)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_svfd_ffs_middle0_mdm_START (15)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_svfd_ffs_middle0_mdm_END (15)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_svfd_cpm_big0_mdm_START (16)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_svfd_cpm_big0_mdm_END (16)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_svfd_cpm_middle0_mdm_START (17)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_svfd_cpm_middle0_mdm_END (17)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_prst_atgc1_START (18)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_prst_atgc1_END (18)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_prst_atgs_sys_START (19)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_prst_atgs_sys_END (19)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_hrst_spe_START (20)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_hrst_spe_END (20)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_spe_START (21)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_spe_END (21)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_core_l_l3_crg_START (22)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_core_l_l3_crg_END (22)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_core_b_m_crg_START (23)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_core_b_m_crg_END (23)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_i3c4_START (24)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_i3c4_END (24)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_arst_axi_mem_START (25)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_arst_axi_mem_END (25)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_lpmcu_START (28)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_lpmcu_END (28)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_arst_maa_START (29)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_arst_maa_END (29)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_latency_monitor_START (30)
#define SOC_CRGPERIPH_PERRSTSTAT4_ip_rst_latency_monitor_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_hrst_ddrc_dmux : 1;
        unsigned int ip_rst_noc_dmc_a : 1;
        unsigned int ip_rst_noc_dmc_b : 1;
        unsigned int ip_rst_noc_dmc_c : 1;
        unsigned int ip_rst_noc_dmc_d : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_core_crg_apb : 1;
        unsigned int ip_rst_cpu_gic : 1;
        unsigned int ip_rst_svfd_ffs_gpu_core : 1;
        unsigned int ip_rst_svfd_ffs_big0 : 1;
        unsigned int ip_rst_svfd_ffs_little : 1;
        unsigned int ip_rst_svfd_cpm_gpu_core : 1;
        unsigned int ip_rst_svfd_cpm_big0 : 1;
        unsigned int ip_rst_svfd_cpm_little : 1;
        unsigned int ip_rst_pll_unlock_detect : 1;
        unsigned int ip_rst_media : 1;
        unsigned int ip_rst_media_crg : 1;
        unsigned int ip_rst_g3dshpm9 : 1;
        unsigned int ip_rst_g3dshpm10 : 1;
        unsigned int ip_rst_g3dshpm11 : 1;
        unsigned int ip_rst_process_monitor_gpu : 1;
        unsigned int ip_rst_process_monitor_mdm0 : 1;
        unsigned int reserved_3 : 1;
        unsigned int ip_rst_process_monitor_ao : 1;
        unsigned int ip_rst_process_monitor_cpu : 1;
        unsigned int ip_rst_process_monitor_vivo : 1;
        unsigned int ip_arst_gpupcr : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTEN5_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTEN5_ip_hrst_ddrc_dmux_START (0)
#define SOC_CRGPERIPH_PERRSTEN5_ip_hrst_ddrc_dmux_END (0)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_noc_dmc_a_START (1)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_noc_dmc_a_END (1)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_noc_dmc_b_START (2)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_noc_dmc_b_END (2)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_noc_dmc_c_START (3)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_noc_dmc_c_END (3)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_noc_dmc_d_START (4)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_noc_dmc_d_END (4)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_core_crg_apb_START (8)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_core_crg_apb_END (8)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_cpu_gic_START (9)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_cpu_gic_END (9)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_svfd_ffs_gpu_core_START (10)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_svfd_ffs_gpu_core_END (10)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_svfd_ffs_big0_START (11)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_svfd_ffs_big0_END (11)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_svfd_ffs_little_START (12)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_svfd_ffs_little_END (12)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_svfd_cpm_gpu_core_START (13)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_svfd_cpm_gpu_core_END (13)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_svfd_cpm_big0_START (14)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_svfd_cpm_big0_END (14)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_svfd_cpm_little_START (15)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_svfd_cpm_little_END (15)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_pll_unlock_detect_START (16)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_pll_unlock_detect_END (16)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_media_START (17)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_media_END (17)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_media_crg_START (18)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_media_crg_END (18)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_g3dshpm9_START (19)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_g3dshpm9_END (19)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_g3dshpm10_START (20)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_g3dshpm10_END (20)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_g3dshpm11_START (21)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_g3dshpm11_END (21)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_process_monitor_gpu_START (22)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_process_monitor_gpu_END (22)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_process_monitor_mdm0_START (23)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_process_monitor_mdm0_END (23)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_process_monitor_ao_START (25)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_process_monitor_ao_END (25)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_process_monitor_cpu_START (26)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_process_monitor_cpu_END (26)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_process_monitor_vivo_START (27)
#define SOC_CRGPERIPH_PERRSTEN5_ip_rst_process_monitor_vivo_END (27)
#define SOC_CRGPERIPH_PERRSTEN5_ip_arst_gpupcr_START (28)
#define SOC_CRGPERIPH_PERRSTEN5_ip_arst_gpupcr_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_hrst_ddrc_dmux : 1;
        unsigned int ip_rst_noc_dmc_a : 1;
        unsigned int ip_rst_noc_dmc_b : 1;
        unsigned int ip_rst_noc_dmc_c : 1;
        unsigned int ip_rst_noc_dmc_d : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_core_crg_apb : 1;
        unsigned int ip_rst_cpu_gic : 1;
        unsigned int ip_rst_svfd_ffs_gpu_core : 1;
        unsigned int ip_rst_svfd_ffs_big0 : 1;
        unsigned int ip_rst_svfd_ffs_little : 1;
        unsigned int ip_rst_svfd_cpm_gpu_core : 1;
        unsigned int ip_rst_svfd_cpm_big0 : 1;
        unsigned int ip_rst_svfd_cpm_little : 1;
        unsigned int ip_rst_pll_unlock_detect : 1;
        unsigned int ip_rst_media : 1;
        unsigned int ip_rst_media_crg : 1;
        unsigned int ip_rst_g3dshpm9 : 1;
        unsigned int ip_rst_g3dshpm10 : 1;
        unsigned int ip_rst_g3dshpm11 : 1;
        unsigned int ip_rst_process_monitor_gpu : 1;
        unsigned int ip_rst_process_monitor_mdm0 : 1;
        unsigned int reserved_3 : 1;
        unsigned int ip_rst_process_monitor_ao : 1;
        unsigned int ip_rst_process_monitor_cpu : 1;
        unsigned int ip_rst_process_monitor_vivo : 1;
        unsigned int ip_arst_gpupcr : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTDIS5_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTDIS5_ip_hrst_ddrc_dmux_START (0)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_hrst_ddrc_dmux_END (0)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_noc_dmc_a_START (1)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_noc_dmc_a_END (1)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_noc_dmc_b_START (2)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_noc_dmc_b_END (2)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_noc_dmc_c_START (3)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_noc_dmc_c_END (3)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_noc_dmc_d_START (4)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_noc_dmc_d_END (4)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_core_crg_apb_START (8)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_core_crg_apb_END (8)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_cpu_gic_START (9)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_cpu_gic_END (9)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_svfd_ffs_gpu_core_START (10)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_svfd_ffs_gpu_core_END (10)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_svfd_ffs_big0_START (11)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_svfd_ffs_big0_END (11)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_svfd_ffs_little_START (12)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_svfd_ffs_little_END (12)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_svfd_cpm_gpu_core_START (13)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_svfd_cpm_gpu_core_END (13)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_svfd_cpm_big0_START (14)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_svfd_cpm_big0_END (14)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_svfd_cpm_little_START (15)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_svfd_cpm_little_END (15)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_pll_unlock_detect_START (16)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_pll_unlock_detect_END (16)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_media_START (17)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_media_END (17)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_media_crg_START (18)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_media_crg_END (18)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_g3dshpm9_START (19)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_g3dshpm9_END (19)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_g3dshpm10_START (20)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_g3dshpm10_END (20)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_g3dshpm11_START (21)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_g3dshpm11_END (21)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_process_monitor_gpu_START (22)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_process_monitor_gpu_END (22)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_process_monitor_mdm0_START (23)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_process_monitor_mdm0_END (23)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_process_monitor_ao_START (25)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_process_monitor_ao_END (25)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_process_monitor_cpu_START (26)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_process_monitor_cpu_END (26)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_process_monitor_vivo_START (27)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_rst_process_monitor_vivo_END (27)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_arst_gpupcr_START (28)
#define SOC_CRGPERIPH_PERRSTDIS5_ip_arst_gpupcr_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_hrst_ddrc_dmux : 1;
        unsigned int ip_rst_noc_dmc_a : 1;
        unsigned int ip_rst_noc_dmc_b : 1;
        unsigned int ip_rst_noc_dmc_c : 1;
        unsigned int ip_rst_noc_dmc_d : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_core_crg_apb : 1;
        unsigned int ip_rst_cpu_gic : 1;
        unsigned int ip_rst_svfd_ffs_gpu_core : 1;
        unsigned int ip_rst_svfd_ffs_big0 : 1;
        unsigned int ip_rst_svfd_ffs_little : 1;
        unsigned int ip_rst_svfd_cpm_gpu_core : 1;
        unsigned int ip_rst_svfd_cpm_big0 : 1;
        unsigned int ip_rst_svfd_cpm_little : 1;
        unsigned int ip_rst_pll_unlock_detect : 1;
        unsigned int ip_rst_media : 1;
        unsigned int ip_rst_media_crg : 1;
        unsigned int ip_rst_g3dshpm9 : 1;
        unsigned int ip_rst_g3dshpm10 : 1;
        unsigned int ip_rst_g3dshpm11 : 1;
        unsigned int ip_rst_process_monitor_gpu : 1;
        unsigned int ip_rst_process_monitor_mdm0 : 1;
        unsigned int reserved_3 : 1;
        unsigned int ip_rst_process_monitor_ao : 1;
        unsigned int ip_rst_process_monitor_cpu : 1;
        unsigned int ip_rst_process_monitor_vivo : 1;
        unsigned int ip_arst_gpupcr : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTSTAT5_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_hrst_ddrc_dmux_START (0)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_hrst_ddrc_dmux_END (0)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_noc_dmc_a_START (1)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_noc_dmc_a_END (1)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_noc_dmc_b_START (2)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_noc_dmc_b_END (2)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_noc_dmc_c_START (3)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_noc_dmc_c_END (3)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_noc_dmc_d_START (4)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_noc_dmc_d_END (4)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_core_crg_apb_START (8)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_core_crg_apb_END (8)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_cpu_gic_START (9)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_cpu_gic_END (9)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_svfd_ffs_gpu_core_START (10)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_svfd_ffs_gpu_core_END (10)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_svfd_ffs_big0_START (11)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_svfd_ffs_big0_END (11)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_svfd_ffs_little_START (12)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_svfd_ffs_little_END (12)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_svfd_cpm_gpu_core_START (13)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_svfd_cpm_gpu_core_END (13)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_svfd_cpm_big0_START (14)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_svfd_cpm_big0_END (14)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_svfd_cpm_little_START (15)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_svfd_cpm_little_END (15)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_pll_unlock_detect_START (16)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_pll_unlock_detect_END (16)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_media_START (17)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_media_END (17)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_media_crg_START (18)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_media_crg_END (18)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_g3dshpm9_START (19)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_g3dshpm9_END (19)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_g3dshpm10_START (20)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_g3dshpm10_END (20)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_g3dshpm11_START (21)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_g3dshpm11_END (21)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_process_monitor_gpu_START (22)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_process_monitor_gpu_END (22)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_process_monitor_mdm0_START (23)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_process_monitor_mdm0_END (23)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_process_monitor_ao_START (25)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_process_monitor_ao_END (25)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_process_monitor_cpu_START (26)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_process_monitor_cpu_END (26)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_process_monitor_vivo_START (27)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_rst_process_monitor_vivo_END (27)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_arst_gpupcr_START (28)
#define SOC_CRGPERIPH_PERRSTSTAT5_ip_arst_gpupcr_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_sysbus_pll : 5;
        unsigned int div_lpmcu : 5;
        unsigned int div_x2j : 4;
        unsigned int sel_clk_out1 : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV0_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV0_div_sysbus_pll_START (0)
#define SOC_CRGPERIPH_CLKDIV0_div_sysbus_pll_END (4)
#define SOC_CRGPERIPH_CLKDIV0_div_lpmcu_START (5)
#define SOC_CRGPERIPH_CLKDIV0_div_lpmcu_END (9)
#define SOC_CRGPERIPH_CLKDIV0_div_x2j_START (10)
#define SOC_CRGPERIPH_CLKDIV0_div_x2j_END (13)
#define SOC_CRGPERIPH_CLKDIV0_sel_clk_out1_START (14)
#define SOC_CRGPERIPH_CLKDIV0_sel_clk_out1_END (15)
#define SOC_CRGPERIPH_CLKDIV0_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 2;
        unsigned int sel_uart0 : 1;
        unsigned int sel_uartl : 1;
        unsigned int sel_uarth : 1;
        unsigned int reserved_1 : 2;
        unsigned int sel_codeccssi_sys_src : 1;
        unsigned int sel_spi : 1;
        unsigned int lpmcu_clk_sw_req_cfg : 2;
        unsigned int lpmcu_invar_clk_sw_req_cfg : 2;
        unsigned int sel_i2c : 1;
        unsigned int sysbus_clk_sw_req_cfg : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV1_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV1_sel_uart0_START (2)
#define SOC_CRGPERIPH_CLKDIV1_sel_uart0_END (2)
#define SOC_CRGPERIPH_CLKDIV1_sel_uartl_START (3)
#define SOC_CRGPERIPH_CLKDIV1_sel_uartl_END (3)
#define SOC_CRGPERIPH_CLKDIV1_sel_uarth_START (4)
#define SOC_CRGPERIPH_CLKDIV1_sel_uarth_END (4)
#define SOC_CRGPERIPH_CLKDIV1_sel_codeccssi_sys_src_START (7)
#define SOC_CRGPERIPH_CLKDIV1_sel_codeccssi_sys_src_END (7)
#define SOC_CRGPERIPH_CLKDIV1_sel_spi_START (8)
#define SOC_CRGPERIPH_CLKDIV1_sel_spi_END (8)
#define SOC_CRGPERIPH_CLKDIV1_lpmcu_clk_sw_req_cfg_START (9)
#define SOC_CRGPERIPH_CLKDIV1_lpmcu_clk_sw_req_cfg_END (10)
#define SOC_CRGPERIPH_CLKDIV1_lpmcu_invar_clk_sw_req_cfg_START (11)
#define SOC_CRGPERIPH_CLKDIV1_lpmcu_invar_clk_sw_req_cfg_END (12)
#define SOC_CRGPERIPH_CLKDIV1_sel_i2c_START (13)
#define SOC_CRGPERIPH_CLKDIV1_sel_i2c_END (13)
#define SOC_CRGPERIPH_CLKDIV1_sysbus_clk_sw_req_cfg_START (14)
#define SOC_CRGPERIPH_CLKDIV1_sysbus_clk_sw_req_cfg_END (15)
#define SOC_CRGPERIPH_CLKDIV1_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV1_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sel_tiny_core_peri_pll : 4;
        unsigned int div_uart0 : 4;
        unsigned int div_uartl : 4;
        unsigned int div_uarth : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV2_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV2_sel_tiny_core_peri_pll_START (0)
#define SOC_CRGPERIPH_CLKDIV2_sel_tiny_core_peri_pll_END (3)
#define SOC_CRGPERIPH_CLKDIV2_div_uart0_START (4)
#define SOC_CRGPERIPH_CLKDIV2_div_uart0_END (7)
#define SOC_CRGPERIPH_CLKDIV2_div_uartl_START (8)
#define SOC_CRGPERIPH_CLKDIV2_div_uartl_END (11)
#define SOC_CRGPERIPH_CLKDIV2_div_uarth_START (12)
#define SOC_CRGPERIPH_CLKDIV2_div_uarth_END (15)
#define SOC_CRGPERIPH_CLKDIV2_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV2_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 6;
        unsigned int div_pclk_pcie : 4;
        unsigned int div_fcm_pcr : 2;
        unsigned int reserved_1 : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV3_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV3_div_pclk_pcie_START (6)
#define SOC_CRGPERIPH_CLKDIV3_div_pclk_pcie_END (9)
#define SOC_CRGPERIPH_CLKDIV3_div_fcm_pcr_START (10)
#define SOC_CRGPERIPH_CLKDIV3_div_fcm_pcr_END (11)
#define SOC_CRGPERIPH_CLKDIV3_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV3_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_sd : 4;
        unsigned int sel_sd_pll : 3;
        unsigned int div_isp_i2c : 4;
        unsigned int div_clkout1_tcxo : 3;
        unsigned int div_loadmonitor : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV4_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV4_div_sd_START (0)
#define SOC_CRGPERIPH_CLKDIV4_div_sd_END (3)
#define SOC_CRGPERIPH_CLKDIV4_sel_sd_pll_START (4)
#define SOC_CRGPERIPH_CLKDIV4_sel_sd_pll_END (6)
#define SOC_CRGPERIPH_CLKDIV4_div_isp_i2c_START (7)
#define SOC_CRGPERIPH_CLKDIV4_div_isp_i2c_END (10)
#define SOC_CRGPERIPH_CLKDIV4_div_clkout1_tcxo_START (11)
#define SOC_CRGPERIPH_CLKDIV4_div_clkout1_tcxo_END (13)
#define SOC_CRGPERIPH_CLKDIV4_div_loadmonitor_START (14)
#define SOC_CRGPERIPH_CLKDIV4_div_loadmonitor_END (15)
#define SOC_CRGPERIPH_CLKDIV4_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV4_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 6;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 4;
        unsigned int div_bba : 4;
        unsigned int div_cssysdbg : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV5_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV5_div_bba_START (11)
#define SOC_CRGPERIPH_CLKDIV5_div_bba_END (14)
#define SOC_CRGPERIPH_CLKDIV5_div_cssysdbg_START (15)
#define SOC_CRGPERIPH_CLKDIV5_div_cssysdbg_END (15)
#define SOC_CRGPERIPH_CLKDIV5_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV5_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 6;
        unsigned int reserved_1 : 2;
        unsigned int reserved_2 : 2;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 2;
        unsigned int div_clk_noc_npubus_cfg : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV6_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV6_div_clk_noc_npubus_cfg_START (14)
#define SOC_CRGPERIPH_CLKDIV6_div_clk_noc_npubus_cfg_END (15)
#define SOC_CRGPERIPH_CLKDIV6_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV6_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 6;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int sc_gt_clk_tiny_core_peri : 1;
        unsigned int reserved_4 : 2;
        unsigned int div_spi : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV7_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV7_sc_gt_clk_tiny_core_peri_START (9)
#define SOC_CRGPERIPH_CLKDIV7_sc_gt_clk_tiny_core_peri_END (9)
#define SOC_CRGPERIPH_CLKDIV7_div_spi_START (12)
#define SOC_CRGPERIPH_CLKDIV7_div_spi_END (15)
#define SOC_CRGPERIPH_CLKDIV7_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV7_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 4;
        unsigned int reserved_1 : 3;
        unsigned int gt_aclk_g3d_core_pcr_in_bypass : 1;
        unsigned int div_clk_gpu_tp : 6;
        unsigned int sc_gt_clk_gpu_tp_mux : 1;
        unsigned int sel_clk_gpu_tp : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV8_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV8_gt_aclk_g3d_core_pcr_in_bypass_START (7)
#define SOC_CRGPERIPH_CLKDIV8_gt_aclk_g3d_core_pcr_in_bypass_END (7)
#define SOC_CRGPERIPH_CLKDIV8_div_clk_gpu_tp_START (8)
#define SOC_CRGPERIPH_CLKDIV8_div_clk_gpu_tp_END (13)
#define SOC_CRGPERIPH_CLKDIV8_sc_gt_clk_gpu_tp_mux_START (14)
#define SOC_CRGPERIPH_CLKDIV8_sc_gt_clk_gpu_tp_mux_END (14)
#define SOC_CRGPERIPH_CLKDIV8_sel_clk_gpu_tp_START (15)
#define SOC_CRGPERIPH_CLKDIV8_sel_clk_gpu_tp_END (15)
#define SOC_CRGPERIPH_CLKDIV8_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV8_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_mdm4g_slow : 4;
        unsigned int reserved_0 : 1;
        unsigned int div_npu_pcr : 4;
        unsigned int sel_clk_out1_core : 3;
        unsigned int reserved_1 : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV9_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV9_div_mdm4g_slow_START (0)
#define SOC_CRGPERIPH_CLKDIV9_div_mdm4g_slow_END (3)
#define SOC_CRGPERIPH_CLKDIV9_div_npu_pcr_START (5)
#define SOC_CRGPERIPH_CLKDIV9_div_npu_pcr_END (8)
#define SOC_CRGPERIPH_CLKDIV9_sel_clk_out1_core_START (9)
#define SOC_CRGPERIPH_CLKDIV9_sel_clk_out1_core_END (11)
#define SOC_CRGPERIPH_CLKDIV9_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV9_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 3;
        unsigned int reserved_1 : 4;
        unsigned int reserved_2 : 4;
        unsigned int div_tsensor : 1;
        unsigned int div_perf_stat : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV10_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV10_div_tsensor_START (11)
#define SOC_CRGPERIPH_CLKDIV10_div_tsensor_END (11)
#define SOC_CRGPERIPH_CLKDIV10_div_perf_stat_START (12)
#define SOC_CRGPERIPH_CLKDIV10_div_perf_stat_END (15)
#define SOC_CRGPERIPH_CLKDIV10_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV10_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_hsdt1_pcie1 : 4;
        unsigned int sc_gt_aclk_hsdt1_pcie1 : 1;
        unsigned int reserved_0 : 3;
        unsigned int sel_pll_dmc_sw_a : 1;
        unsigned int sel_a53hpm_pll : 1;
        unsigned int sel_pll_dmc_sw_b : 1;
        unsigned int sel_pll_dmc_sw_c : 1;
        unsigned int sel_pll_dmc_sw_d : 1;
        unsigned int reserved_1 : 1;
        unsigned int div_clk_dcdrbus : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV11_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV11_div_hsdt1_pcie1_START (0)
#define SOC_CRGPERIPH_CLKDIV11_div_hsdt1_pcie1_END (3)
#define SOC_CRGPERIPH_CLKDIV11_sc_gt_aclk_hsdt1_pcie1_START (4)
#define SOC_CRGPERIPH_CLKDIV11_sc_gt_aclk_hsdt1_pcie1_END (4)
#define SOC_CRGPERIPH_CLKDIV11_sel_pll_dmc_sw_a_START (8)
#define SOC_CRGPERIPH_CLKDIV11_sel_pll_dmc_sw_a_END (8)
#define SOC_CRGPERIPH_CLKDIV11_sel_a53hpm_pll_START (9)
#define SOC_CRGPERIPH_CLKDIV11_sel_a53hpm_pll_END (9)
#define SOC_CRGPERIPH_CLKDIV11_sel_pll_dmc_sw_b_START (10)
#define SOC_CRGPERIPH_CLKDIV11_sel_pll_dmc_sw_b_END (10)
#define SOC_CRGPERIPH_CLKDIV11_sel_pll_dmc_sw_c_START (11)
#define SOC_CRGPERIPH_CLKDIV11_sel_pll_dmc_sw_c_END (11)
#define SOC_CRGPERIPH_CLKDIV11_sel_pll_dmc_sw_d_START (12)
#define SOC_CRGPERIPH_CLKDIV11_sel_pll_dmc_sw_d_END (12)
#define SOC_CRGPERIPH_CLKDIV11_div_clk_dcdrbus_START (14)
#define SOC_CRGPERIPH_CLKDIV11_div_clk_dcdrbus_END (15)
#define SOC_CRGPERIPH_CLKDIV11_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV11_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_ptp : 4;
        unsigned int sel_clk_pll_test_src : 4;
        unsigned int div_pll_src_tp : 5;
        unsigned int div_a53hpm : 3;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV12_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV12_div_ptp_START (0)
#define SOC_CRGPERIPH_CLKDIV12_div_ptp_END (3)
#define SOC_CRGPERIPH_CLKDIV12_sel_clk_pll_test_src_START (4)
#define SOC_CRGPERIPH_CLKDIV12_sel_clk_pll_test_src_END (7)
#define SOC_CRGPERIPH_CLKDIV12_div_pll_src_tp_START (8)
#define SOC_CRGPERIPH_CLKDIV12_div_pll_src_tp_END (12)
#define SOC_CRGPERIPH_CLKDIV12_div_a53hpm_START (13)
#define SOC_CRGPERIPH_CLKDIV12_div_a53hpm_END (15)
#define SOC_CRGPERIPH_CLKDIV12_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV12_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sel_npubus_pre_pll : 4;
        unsigned int div_npubus_pre : 4;
        unsigned int reserved : 6;
        unsigned int div_mdm_553m : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV13_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV13_sel_npubus_pre_pll_START (0)
#define SOC_CRGPERIPH_CLKDIV13_sel_npubus_pre_pll_END (3)
#define SOC_CRGPERIPH_CLKDIV13_div_npubus_pre_START (4)
#define SOC_CRGPERIPH_CLKDIV13_div_npubus_pre_END (7)
#define SOC_CRGPERIPH_CLKDIV13_div_mdm_553m_START (14)
#define SOC_CRGPERIPH_CLKDIV13_div_mdm_553m_END (15)
#define SOC_CRGPERIPH_CLKDIV13_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV13_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_process_monitor : 4;
        unsigned int div_clkout1_pll : 6;
        unsigned int reserved : 6;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV14_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV14_div_process_monitor_START (0)
#define SOC_CRGPERIPH_CLKDIV14_div_process_monitor_END (3)
#define SOC_CRGPERIPH_CLKDIV14_div_clkout1_pll_START (4)
#define SOC_CRGPERIPH_CLKDIV14_div_clkout1_pll_END (9)
#define SOC_CRGPERIPH_CLKDIV14_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV14_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_tiny_core_peri : 4;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 4;
        unsigned int reserved_2 : 6;
        unsigned int go2pwr_bypass : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV15_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV15_div_tiny_core_peri_START (0)
#define SOC_CRGPERIPH_CLKDIV15_div_tiny_core_peri_END (3)
#define SOC_CRGPERIPH_CLKDIV15_go2pwr_bypass_START (15)
#define SOC_CRGPERIPH_CLKDIV15_go2pwr_bypass_END (15)
#define SOC_CRGPERIPH_CLKDIV15_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV15_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 4;
        unsigned int div_i2c : 4;
        unsigned int sel_ddrcpll : 4;
        unsigned int ddrc_clk_sw_req_cfg : 2;
        unsigned int reserved_1 : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV16_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV16_div_i2c_START (4)
#define SOC_CRGPERIPH_CLKDIV16_div_i2c_END (7)
#define SOC_CRGPERIPH_CLKDIV16_sel_ddrcpll_START (8)
#define SOC_CRGPERIPH_CLKDIV16_sel_ddrcpll_END (11)
#define SOC_CRGPERIPH_CLKDIV16_ddrc_clk_sw_req_cfg_START (12)
#define SOC_CRGPERIPH_CLKDIV16_ddrc_clk_sw_req_cfg_END (13)
#define SOC_CRGPERIPH_CLKDIV16_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV16_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_cfgbus : 2;
        unsigned int reserved_0 : 1;
        unsigned int req_g3d_core_sram_lv : 1;
        unsigned int reserved_1 : 2;
        unsigned int gt_aclk_g3d_core_sram_lv_bypass : 1;
        unsigned int div_cpu_gic : 6;
        unsigned int req_g3d_core_sram_lv_msk : 1;
        unsigned int div_hsdt1bus : 1;
        unsigned int div_dmabus : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV17_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV17_div_cfgbus_START (0)
#define SOC_CRGPERIPH_CLKDIV17_div_cfgbus_END (1)
#define SOC_CRGPERIPH_CLKDIV17_req_g3d_core_sram_lv_START (3)
#define SOC_CRGPERIPH_CLKDIV17_req_g3d_core_sram_lv_END (3)
#define SOC_CRGPERIPH_CLKDIV17_gt_aclk_g3d_core_sram_lv_bypass_START (6)
#define SOC_CRGPERIPH_CLKDIV17_gt_aclk_g3d_core_sram_lv_bypass_END (6)
#define SOC_CRGPERIPH_CLKDIV17_div_cpu_gic_START (7)
#define SOC_CRGPERIPH_CLKDIV17_div_cpu_gic_END (12)
#define SOC_CRGPERIPH_CLKDIV17_req_g3d_core_sram_lv_msk_START (13)
#define SOC_CRGPERIPH_CLKDIV17_req_g3d_core_sram_lv_msk_END (13)
#define SOC_CRGPERIPH_CLKDIV17_div_hsdt1bus_START (14)
#define SOC_CRGPERIPH_CLKDIV17_div_hsdt1bus_END (14)
#define SOC_CRGPERIPH_CLKDIV17_div_dmabus_START (15)
#define SOC_CRGPERIPH_CLKDIV17_div_dmabus_END (15)
#define SOC_CRGPERIPH_CLKDIV17_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV17_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int sc_gt_clk_time_stamp_div : 1;
        unsigned int sc_gt_clk_ddrc_pll : 1;
        unsigned int sc_gt_clk_perf_stat_div : 1;
        unsigned int sc_gt_clk_lpmcu_pll : 1;
        unsigned int sc_gt_clk_loadmonitor : 1;
        unsigned int sc_gt_clk_sysbus_pll : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int sc_gt_clk_out1 : 1;
        unsigned int sc_gt_clk_fcm_pcr : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int sc_gt_clk_sysbus_sys : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV18_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_time_stamp_div_START (1)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_time_stamp_div_END (1)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_ddrc_pll_START (2)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_ddrc_pll_END (2)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_perf_stat_div_START (3)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_perf_stat_div_END (3)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_lpmcu_pll_START (4)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_lpmcu_pll_END (4)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_loadmonitor_START (5)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_loadmonitor_END (5)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_sysbus_pll_START (6)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_sysbus_pll_END (6)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_out1_START (10)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_out1_END (10)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_fcm_pcr_START (11)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_fcm_pcr_END (11)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_sysbus_sys_START (15)
#define SOC_CRGPERIPH_CLKDIV18_sc_gt_clk_sysbus_sys_END (15)
#define SOC_CRGPERIPH_CLKDIV18_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV18_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_gt_clk_npubus_pre : 1;
        unsigned int sc_gt_clk_cpul_ppll0_div : 1;
        unsigned int sc_gt_clk_l3_ppll0_div : 1;
        unsigned int sc_gt_clk_sd : 1;
        unsigned int sc_gt_clk_ao_asp_pll : 1;
        unsigned int sc_gt_clk_sd_sys : 1;
        unsigned int reserved_0 : 1;
        unsigned int sc_gt_clk_a53hpm : 1;
        unsigned int sc_gt_clk_ts_cpu : 1;
        unsigned int sc_gt_clk_uart0 : 1;
        unsigned int sc_gt_clk_uartl : 1;
        unsigned int sc_gt_clk_uarth : 1;
        unsigned int reserved_1 : 1;
        unsigned int sc_gt_clk_spi : 1;
        unsigned int sc_gt_clk_mdm_553m : 1;
        unsigned int sc_gt_aclk_mdm4g_slow : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV19_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_npubus_pre_START (0)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_npubus_pre_END (0)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_cpul_ppll0_div_START (1)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_cpul_ppll0_div_END (1)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_l3_ppll0_div_START (2)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_l3_ppll0_div_END (2)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_sd_START (3)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_sd_END (3)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_ao_asp_pll_START (4)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_ao_asp_pll_END (4)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_sd_sys_START (5)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_sd_sys_END (5)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_a53hpm_START (7)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_a53hpm_END (7)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_ts_cpu_START (8)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_ts_cpu_END (8)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_uart0_START (9)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_uart0_END (9)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_uartl_START (10)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_uartl_END (10)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_uarth_START (11)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_uarth_END (11)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_spi_START (13)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_spi_END (13)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_mdm_553m_START (14)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_clk_mdm_553m_END (14)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_aclk_mdm4g_slow_START (15)
#define SOC_CRGPERIPH_CLKDIV19_sc_gt_aclk_mdm4g_slow_END (15)
#define SOC_CRGPERIPH_CLKDIV19_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV19_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_gt_clk_i2c : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int sc_gt_clk_cfgbus : 1;
        unsigned int sc_gt_clk_isp_i2c : 1;
        unsigned int sc_gt_clk_ptp : 1;
        unsigned int reserved_2 : 1;
        unsigned int sc_gt_clk_npu_pcr : 1;
        unsigned int reserved_3 : 1;
        unsigned int sc_gt_clk_noc_mdm_mst : 1;
        unsigned int sc_gt_clk_320m_pll : 1;
        unsigned int sc_gt_clk_pll_test_src : 1;
        unsigned int sc_gt_clk_atgc : 1;
        unsigned int sc_gt_pclk_pcie : 1;
        unsigned int sc_gt_clk_hsdt1bus : 1;
        unsigned int reserved_4 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV20_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_i2c_START (0)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_i2c_END (0)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_cfgbus_START (3)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_cfgbus_END (3)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_isp_i2c_START (4)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_isp_i2c_END (4)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_ptp_START (5)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_ptp_END (5)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_npu_pcr_START (7)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_npu_pcr_END (7)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_noc_mdm_mst_START (9)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_noc_mdm_mst_END (9)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_320m_pll_START (10)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_320m_pll_END (10)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_pll_test_src_START (11)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_pll_test_src_END (11)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_atgc_START (12)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_atgc_END (12)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_pclk_pcie_START (13)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_pclk_pcie_END (13)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_hsdt1bus_START (14)
#define SOC_CRGPERIPH_CLKDIV20_sc_gt_clk_hsdt1bus_END (14)
#define SOC_CRGPERIPH_CLKDIV20_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV20_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_gt_clk_core_crg_apb : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int sc_gt_clk_cpu_gic : 1;
        unsigned int reserved_3 : 1;
        unsigned int sc_gt_clk_cssysdbg_div : 1;
        unsigned int sc_gt_clk_lpmcu_invar_fll : 1;
        unsigned int sc_gt_clk_dcdrbus : 1;
        unsigned int sc_gt_clk_socp_deflate : 1;
        unsigned int reserved_4 : 1;
        unsigned int sc_gt_clk_dmabus : 1;
        unsigned int sc_gt_clk_dpctrl_16m : 1;
        unsigned int sc_gt_clk_process_monitor : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV21_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_core_crg_apb_START (0)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_core_crg_apb_END (0)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_cpu_gic_START (4)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_cpu_gic_END (4)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_cssysdbg_div_START (6)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_cssysdbg_div_END (6)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_lpmcu_invar_fll_START (7)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_lpmcu_invar_fll_END (7)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_dcdrbus_START (8)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_dcdrbus_END (8)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_socp_deflate_START (9)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_socp_deflate_END (9)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_dmabus_START (11)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_dmabus_END (11)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_dpctrl_16m_START (12)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_dpctrl_16m_END (12)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_process_monitor_START (13)
#define SOC_CRGPERIPH_CLKDIV21_sc_gt_clk_process_monitor_END (13)
#define SOC_CRGPERIPH_CLKDIV21_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV21_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sel_320m_pll : 1;
        unsigned int div_320m : 3;
        unsigned int sel_ao_asp_pll : 2;
        unsigned int sel_clk_ao_hifd : 1;
        unsigned int sel_isp_snclk3 : 1;
        unsigned int div_isp_snclk3 : 2;
        unsigned int sc_gt_clk_ao_hifd : 1;
        unsigned int div_clk_sys_ddr : 2;
        unsigned int div_clk_sys_sysbus : 2;
        unsigned int sc_gt_clk_latency_monitor : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV22_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV22_sel_320m_pll_START (0)
#define SOC_CRGPERIPH_CLKDIV22_sel_320m_pll_END (0)
#define SOC_CRGPERIPH_CLKDIV22_div_320m_START (1)
#define SOC_CRGPERIPH_CLKDIV22_div_320m_END (3)
#define SOC_CRGPERIPH_CLKDIV22_sel_ao_asp_pll_START (4)
#define SOC_CRGPERIPH_CLKDIV22_sel_ao_asp_pll_END (5)
#define SOC_CRGPERIPH_CLKDIV22_sel_clk_ao_hifd_START (6)
#define SOC_CRGPERIPH_CLKDIV22_sel_clk_ao_hifd_END (6)
#define SOC_CRGPERIPH_CLKDIV22_sel_isp_snclk3_START (7)
#define SOC_CRGPERIPH_CLKDIV22_sel_isp_snclk3_END (7)
#define SOC_CRGPERIPH_CLKDIV22_div_isp_snclk3_START (8)
#define SOC_CRGPERIPH_CLKDIV22_div_isp_snclk3_END (9)
#define SOC_CRGPERIPH_CLKDIV22_sc_gt_clk_ao_hifd_START (10)
#define SOC_CRGPERIPH_CLKDIV22_sc_gt_clk_ao_hifd_END (10)
#define SOC_CRGPERIPH_CLKDIV22_div_clk_sys_ddr_START (11)
#define SOC_CRGPERIPH_CLKDIV22_div_clk_sys_ddr_END (12)
#define SOC_CRGPERIPH_CLKDIV22_div_clk_sys_sysbus_START (13)
#define SOC_CRGPERIPH_CLKDIV22_div_clk_sys_sysbus_END (14)
#define SOC_CRGPERIPH_CLKDIV22_sc_gt_clk_latency_monitor_START (15)
#define SOC_CRGPERIPH_CLKDIV22_sc_gt_clk_latency_monitor_END (15)
#define SOC_CRGPERIPH_CLKDIV22_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV22_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_ddrc_pll : 5;
        unsigned int div_clk_ddrcfg : 2;
        unsigned int sc_gt_clk_ddrcfg : 1;
        unsigned int sc_gt_clk_ddrc_sys : 1;
        unsigned int sc_gt_clk_ddrsys_ao : 1;
        unsigned int div_clk_ddrsys : 2;
        unsigned int sc_gt_clk_ddrsys_noc : 1;
        unsigned int sc_gt_clk_ddrc_sw : 1;
        unsigned int div_ao_hifd : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV23_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV23_div_ddrc_pll_START (0)
#define SOC_CRGPERIPH_CLKDIV23_div_ddrc_pll_END (4)
#define SOC_CRGPERIPH_CLKDIV23_div_clk_ddrcfg_START (5)
#define SOC_CRGPERIPH_CLKDIV23_div_clk_ddrcfg_END (6)
#define SOC_CRGPERIPH_CLKDIV23_sc_gt_clk_ddrcfg_START (7)
#define SOC_CRGPERIPH_CLKDIV23_sc_gt_clk_ddrcfg_END (7)
#define SOC_CRGPERIPH_CLKDIV23_sc_gt_clk_ddrc_sys_START (8)
#define SOC_CRGPERIPH_CLKDIV23_sc_gt_clk_ddrc_sys_END (8)
#define SOC_CRGPERIPH_CLKDIV23_sc_gt_clk_ddrsys_ao_START (9)
#define SOC_CRGPERIPH_CLKDIV23_sc_gt_clk_ddrsys_ao_END (9)
#define SOC_CRGPERIPH_CLKDIV23_div_clk_ddrsys_START (10)
#define SOC_CRGPERIPH_CLKDIV23_div_clk_ddrsys_END (11)
#define SOC_CRGPERIPH_CLKDIV23_sc_gt_clk_ddrsys_noc_START (12)
#define SOC_CRGPERIPH_CLKDIV23_sc_gt_clk_ddrsys_noc_END (12)
#define SOC_CRGPERIPH_CLKDIV23_sc_gt_clk_ddrc_sw_START (13)
#define SOC_CRGPERIPH_CLKDIV23_sc_gt_clk_ddrc_sw_END (13)
#define SOC_CRGPERIPH_CLKDIV23_div_ao_hifd_START (14)
#define SOC_CRGPERIPH_CLKDIV23_div_ao_hifd_END (15)
#define SOC_CRGPERIPH_CLKDIV23_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV23_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_isp_snclk0 : 2;
        unsigned int sc_gt_clk_isp_snclk : 1;
        unsigned int sel_isp_snclk0 : 1;
        unsigned int atclk_to_ispa7_clkoff_sys : 1;
        unsigned int pclkdbg_to_ispa7_clkoff_sys : 1;
        unsigned int div_ao_asp : 4;
        unsigned int reserved_0 : 2;
        unsigned int reserved_1 : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV24_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV24_div_isp_snclk0_START (0)
#define SOC_CRGPERIPH_CLKDIV24_div_isp_snclk0_END (1)
#define SOC_CRGPERIPH_CLKDIV24_sc_gt_clk_isp_snclk_START (2)
#define SOC_CRGPERIPH_CLKDIV24_sc_gt_clk_isp_snclk_END (2)
#define SOC_CRGPERIPH_CLKDIV24_sel_isp_snclk0_START (3)
#define SOC_CRGPERIPH_CLKDIV24_sel_isp_snclk0_END (3)
#define SOC_CRGPERIPH_CLKDIV24_atclk_to_ispa7_clkoff_sys_START (4)
#define SOC_CRGPERIPH_CLKDIV24_atclk_to_ispa7_clkoff_sys_END (4)
#define SOC_CRGPERIPH_CLKDIV24_pclkdbg_to_ispa7_clkoff_sys_START (5)
#define SOC_CRGPERIPH_CLKDIV24_pclkdbg_to_ispa7_clkoff_sys_END (5)
#define SOC_CRGPERIPH_CLKDIV24_div_ao_asp_START (6)
#define SOC_CRGPERIPH_CLKDIV24_div_ao_asp_END (9)
#define SOC_CRGPERIPH_CLKDIV24_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV24_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 2;
        unsigned int reserved_1 : 2;
        unsigned int reserved_2 : 1;
        unsigned int sel_hsdt1_usbdp : 1;
        unsigned int reserved_3 : 2;
        unsigned int reserved_4 : 2;
        unsigned int sel_isp_snclk2 : 1;
        unsigned int div_isp_snclk2 : 2;
        unsigned int sel_isp_snclk1 : 1;
        unsigned int div_isp_snclk1 : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV25_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV25_sel_hsdt1_usbdp_START (5)
#define SOC_CRGPERIPH_CLKDIV25_sel_hsdt1_usbdp_END (5)
#define SOC_CRGPERIPH_CLKDIV25_sel_isp_snclk2_START (10)
#define SOC_CRGPERIPH_CLKDIV25_sel_isp_snclk2_END (10)
#define SOC_CRGPERIPH_CLKDIV25_div_isp_snclk2_START (11)
#define SOC_CRGPERIPH_CLKDIV25_div_isp_snclk2_END (12)
#define SOC_CRGPERIPH_CLKDIV25_sel_isp_snclk1_START (13)
#define SOC_CRGPERIPH_CLKDIV25_sel_isp_snclk1_END (13)
#define SOC_CRGPERIPH_CLKDIV25_div_isp_snclk1_START (14)
#define SOC_CRGPERIPH_CLKDIV25_div_isp_snclk1_END (15)
#define SOC_CRGPERIPH_CLKDIV25_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV25_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sysbus_idle_req : 1;
        unsigned int cfgbus_idle_req : 1;
        unsigned int dmabus_idle_req : 1;
        unsigned int hsdtbus_idle_req : 1;
        unsigned int hsdt1bus_idle_req : 1;
        unsigned int mdmbus_idle_req : 1;
        unsigned int hiseebus_idle_req : 1;
        unsigned int npubus_idle_req : 1;
        unsigned int mdm5gbus_idle_req : 1;
        unsigned int reserved_0 : 1;
        unsigned int sysbus_idle_ack : 1;
        unsigned int cfgbus_idle_ack : 1;
        unsigned int dmabus_idle_ack : 1;
        unsigned int hsdtbus_idle_ack : 1;
        unsigned int hsdt1bus_idle_ack : 1;
        unsigned int mdmbus_idle_ack : 1;
        unsigned int hiseebus_idle_ack : 1;
        unsigned int npubus_idle_ack : 1;
        unsigned int mdm5gbus_idle_ack : 1;
        unsigned int reserved_1 : 1;
        unsigned int sysbus_idle_stat : 1;
        unsigned int cfgbus_idle_stat : 1;
        unsigned int dmabus_idle_stat : 1;
        unsigned int hsdtbus_idle_stat : 1;
        unsigned int hsdt1bus_idle_stat : 1;
        unsigned int mdmbus_idle_stat : 1;
        unsigned int hiseebus_idle_stat : 1;
        unsigned int npubus_idle_stat : 1;
        unsigned int mdm5gbus_idle_stat : 1;
        unsigned int reserved_2 : 3;
    } reg;
} SOC_CRGPERIPH_PERI_STAT0_UNION;
#endif
#define SOC_CRGPERIPH_PERI_STAT0_sysbus_idle_req_START (0)
#define SOC_CRGPERIPH_PERI_STAT0_sysbus_idle_req_END (0)
#define SOC_CRGPERIPH_PERI_STAT0_cfgbus_idle_req_START (1)
#define SOC_CRGPERIPH_PERI_STAT0_cfgbus_idle_req_END (1)
#define SOC_CRGPERIPH_PERI_STAT0_dmabus_idle_req_START (2)
#define SOC_CRGPERIPH_PERI_STAT0_dmabus_idle_req_END (2)
#define SOC_CRGPERIPH_PERI_STAT0_hsdtbus_idle_req_START (3)
#define SOC_CRGPERIPH_PERI_STAT0_hsdtbus_idle_req_END (3)
#define SOC_CRGPERIPH_PERI_STAT0_hsdt1bus_idle_req_START (4)
#define SOC_CRGPERIPH_PERI_STAT0_hsdt1bus_idle_req_END (4)
#define SOC_CRGPERIPH_PERI_STAT0_mdmbus_idle_req_START (5)
#define SOC_CRGPERIPH_PERI_STAT0_mdmbus_idle_req_END (5)
#define SOC_CRGPERIPH_PERI_STAT0_hiseebus_idle_req_START (6)
#define SOC_CRGPERIPH_PERI_STAT0_hiseebus_idle_req_END (6)
#define SOC_CRGPERIPH_PERI_STAT0_npubus_idle_req_START (7)
#define SOC_CRGPERIPH_PERI_STAT0_npubus_idle_req_END (7)
#define SOC_CRGPERIPH_PERI_STAT0_mdm5gbus_idle_req_START (8)
#define SOC_CRGPERIPH_PERI_STAT0_mdm5gbus_idle_req_END (8)
#define SOC_CRGPERIPH_PERI_STAT0_sysbus_idle_ack_START (10)
#define SOC_CRGPERIPH_PERI_STAT0_sysbus_idle_ack_END (10)
#define SOC_CRGPERIPH_PERI_STAT0_cfgbus_idle_ack_START (11)
#define SOC_CRGPERIPH_PERI_STAT0_cfgbus_idle_ack_END (11)
#define SOC_CRGPERIPH_PERI_STAT0_dmabus_idle_ack_START (12)
#define SOC_CRGPERIPH_PERI_STAT0_dmabus_idle_ack_END (12)
#define SOC_CRGPERIPH_PERI_STAT0_hsdtbus_idle_ack_START (13)
#define SOC_CRGPERIPH_PERI_STAT0_hsdtbus_idle_ack_END (13)
#define SOC_CRGPERIPH_PERI_STAT0_hsdt1bus_idle_ack_START (14)
#define SOC_CRGPERIPH_PERI_STAT0_hsdt1bus_idle_ack_END (14)
#define SOC_CRGPERIPH_PERI_STAT0_mdmbus_idle_ack_START (15)
#define SOC_CRGPERIPH_PERI_STAT0_mdmbus_idle_ack_END (15)
#define SOC_CRGPERIPH_PERI_STAT0_hiseebus_idle_ack_START (16)
#define SOC_CRGPERIPH_PERI_STAT0_hiseebus_idle_ack_END (16)
#define SOC_CRGPERIPH_PERI_STAT0_npubus_idle_ack_START (17)
#define SOC_CRGPERIPH_PERI_STAT0_npubus_idle_ack_END (17)
#define SOC_CRGPERIPH_PERI_STAT0_mdm5gbus_idle_ack_START (18)
#define SOC_CRGPERIPH_PERI_STAT0_mdm5gbus_idle_ack_END (18)
#define SOC_CRGPERIPH_PERI_STAT0_sysbus_idle_stat_START (20)
#define SOC_CRGPERIPH_PERI_STAT0_sysbus_idle_stat_END (20)
#define SOC_CRGPERIPH_PERI_STAT0_cfgbus_idle_stat_START (21)
#define SOC_CRGPERIPH_PERI_STAT0_cfgbus_idle_stat_END (21)
#define SOC_CRGPERIPH_PERI_STAT0_dmabus_idle_stat_START (22)
#define SOC_CRGPERIPH_PERI_STAT0_dmabus_idle_stat_END (22)
#define SOC_CRGPERIPH_PERI_STAT0_hsdtbus_idle_stat_START (23)
#define SOC_CRGPERIPH_PERI_STAT0_hsdtbus_idle_stat_END (23)
#define SOC_CRGPERIPH_PERI_STAT0_hsdt1bus_idle_stat_START (24)
#define SOC_CRGPERIPH_PERI_STAT0_hsdt1bus_idle_stat_END (24)
#define SOC_CRGPERIPH_PERI_STAT0_mdmbus_idle_stat_START (25)
#define SOC_CRGPERIPH_PERI_STAT0_mdmbus_idle_stat_END (25)
#define SOC_CRGPERIPH_PERI_STAT0_hiseebus_idle_stat_START (26)
#define SOC_CRGPERIPH_PERI_STAT0_hiseebus_idle_stat_END (26)
#define SOC_CRGPERIPH_PERI_STAT0_npubus_idle_stat_START (27)
#define SOC_CRGPERIPH_PERI_STAT0_npubus_idle_stat_END (27)
#define SOC_CRGPERIPH_PERI_STAT0_mdm5gbus_idle_stat_START (28)
#define SOC_CRGPERIPH_PERI_STAT0_mdm5gbus_idle_stat_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bootmode : 2;
        unsigned int sc_noc_aobus_idle_flag : 1;
        unsigned int isp_to_ddrc_dfs_en : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 24;
    } reg;
} SOC_CRGPERIPH_PERI_STAT1_UNION;
#endif
#define SOC_CRGPERIPH_PERI_STAT1_bootmode_START (0)
#define SOC_CRGPERIPH_PERI_STAT1_bootmode_END (1)
#define SOC_CRGPERIPH_PERI_STAT1_sc_noc_aobus_idle_flag_START (2)
#define SOC_CRGPERIPH_PERI_STAT1_sc_noc_aobus_idle_flag_END (2)
#define SOC_CRGPERIPH_PERI_STAT1_isp_to_ddrc_dfs_en_START (3)
#define SOC_CRGPERIPH_PERI_STAT1_isp_to_ddrc_dfs_en_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int swdone_clk_ddrc_pll_div : 1;
        unsigned int swdone_clk_sysbus_pll_div : 1;
        unsigned int swdone_clk_lpmcu_pll_div : 1;
        unsigned int swdone_clk_cpu_gic_div : 1;
        unsigned int swdone_clk_isp_snclk3_div : 1;
        unsigned int swdone_clk_ddrcfg : 1;
        unsigned int reserved_0 : 1;
        unsigned int swdone_clk_sys_tsensor_div : 1;
        unsigned int swdone_clk_noc_npubus_div : 1;
        unsigned int swdone_clk_ddrsys_noc : 1;
        unsigned int swdone_pclk_pcie : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int swdone_clk_isp_snclk2_div : 1;
        unsigned int swdone_clk_cfgbus_div : 1;
        unsigned int reserved_4 : 1;
        unsigned int swdone_clk_npubus_pre_div : 1;
        unsigned int reserved_5 : 1;
        unsigned int swdone_clk_dmabus_div : 1;
        unsigned int reserved_6 : 1;
        unsigned int swdone_clk_a53hpm_div : 1;
        unsigned int swdone_clk_fcm_pcr_div : 1;
        unsigned int reserved_7 : 1;
        unsigned int swdone_clk_isp_snclk0_div : 1;
        unsigned int swdone_clk_ptp_div : 1;
        unsigned int swdone_clk_isp_snclk1_div : 1;
        unsigned int swdone_clk_spi_div : 1;
        unsigned int swdone_clk_i2c_div : 1;
        unsigned int swdone_aclk_g3d_vdm_aft_div : 1;
        unsigned int reserved_8 : 1;
        unsigned int swdone_clk_out1_div : 1;
    } reg;
} SOC_CRGPERIPH_PERI_STAT2_UNION;
#endif
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_ddrc_pll_div_START (0)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_ddrc_pll_div_END (0)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_sysbus_pll_div_START (1)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_sysbus_pll_div_END (1)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_lpmcu_pll_div_START (2)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_lpmcu_pll_div_END (2)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_cpu_gic_div_START (3)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_cpu_gic_div_END (3)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_isp_snclk3_div_START (4)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_isp_snclk3_div_END (4)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_ddrcfg_START (5)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_ddrcfg_END (5)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_sys_tsensor_div_START (7)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_sys_tsensor_div_END (7)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_noc_npubus_div_START (8)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_noc_npubus_div_END (8)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_ddrsys_noc_START (9)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_ddrsys_noc_END (9)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_pclk_pcie_START (10)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_pclk_pcie_END (10)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_isp_snclk2_div_START (14)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_isp_snclk2_div_END (14)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_cfgbus_div_START (15)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_cfgbus_div_END (15)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_npubus_pre_div_START (17)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_npubus_pre_div_END (17)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_dmabus_div_START (19)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_dmabus_div_END (19)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_a53hpm_div_START (21)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_a53hpm_div_END (21)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_fcm_pcr_div_START (22)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_fcm_pcr_div_END (22)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_isp_snclk0_div_START (24)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_isp_snclk0_div_END (24)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_ptp_div_START (25)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_ptp_div_END (25)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_isp_snclk1_div_START (26)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_isp_snclk1_div_END (26)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_spi_div_START (27)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_spi_div_END (27)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_i2c_div_START (28)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_i2c_div_END (28)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_aclk_g3d_vdm_aft_div_START (29)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_aclk_g3d_vdm_aft_div_END (29)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_out1_div_START (31)
#define SOC_CRGPERIPH_PERI_STAT2_swdone_clk_out1_div_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int peri_avs_vol_up_timeout : 1;
        unsigned int peri_avs_vol_down_timeout : 1;
        unsigned int peri_avs_up_req : 1;
        unsigned int peri_avs_down_req : 1;
        unsigned int peri_avs_ddr_lp_en : 1;
        unsigned int sw_ack_clk_npubus_pre_sw : 4;
        unsigned int reserved_0 : 2;
        unsigned int swdone_clk_ts_cpu_div : 1;
        unsigned int sw_ack_clk_ddrc_pll_sw : 4;
        unsigned int reserved_1 : 2;
        unsigned int reserved_2 : 2;
        unsigned int swdone_clk_npu_pcr_div : 1;
        unsigned int swdone_aclk_mdm4g_slow_div : 1;
        unsigned int swdone_clk_cpul_ppll0_div : 1;
        unsigned int swdone_clk_l3_ppll0_div : 1;
        unsigned int ddrclksw2sys : 1;
        unsigned int ddrclksw2pll : 1;
        unsigned int sysbusclksw2sys : 1;
        unsigned int sysbusclksw2pll : 1;
        unsigned int lpmcuclksw2sys : 1;
        unsigned int lpmcuclksw2pll : 1;
        unsigned int reserved_3 : 2;
    } reg;
} SOC_CRGPERIPH_PERI_STAT3_UNION;
#endif
#define SOC_CRGPERIPH_PERI_STAT3_peri_avs_vol_up_timeout_START (0)
#define SOC_CRGPERIPH_PERI_STAT3_peri_avs_vol_up_timeout_END (0)
#define SOC_CRGPERIPH_PERI_STAT3_peri_avs_vol_down_timeout_START (1)
#define SOC_CRGPERIPH_PERI_STAT3_peri_avs_vol_down_timeout_END (1)
#define SOC_CRGPERIPH_PERI_STAT3_peri_avs_up_req_START (2)
#define SOC_CRGPERIPH_PERI_STAT3_peri_avs_up_req_END (2)
#define SOC_CRGPERIPH_PERI_STAT3_peri_avs_down_req_START (3)
#define SOC_CRGPERIPH_PERI_STAT3_peri_avs_down_req_END (3)
#define SOC_CRGPERIPH_PERI_STAT3_peri_avs_ddr_lp_en_START (4)
#define SOC_CRGPERIPH_PERI_STAT3_peri_avs_ddr_lp_en_END (4)
#define SOC_CRGPERIPH_PERI_STAT3_sw_ack_clk_npubus_pre_sw_START (5)
#define SOC_CRGPERIPH_PERI_STAT3_sw_ack_clk_npubus_pre_sw_END (8)
#define SOC_CRGPERIPH_PERI_STAT3_swdone_clk_ts_cpu_div_START (11)
#define SOC_CRGPERIPH_PERI_STAT3_swdone_clk_ts_cpu_div_END (11)
#define SOC_CRGPERIPH_PERI_STAT3_sw_ack_clk_ddrc_pll_sw_START (12)
#define SOC_CRGPERIPH_PERI_STAT3_sw_ack_clk_ddrc_pll_sw_END (15)
#define SOC_CRGPERIPH_PERI_STAT3_swdone_clk_npu_pcr_div_START (20)
#define SOC_CRGPERIPH_PERI_STAT3_swdone_clk_npu_pcr_div_END (20)
#define SOC_CRGPERIPH_PERI_STAT3_swdone_aclk_mdm4g_slow_div_START (21)
#define SOC_CRGPERIPH_PERI_STAT3_swdone_aclk_mdm4g_slow_div_END (21)
#define SOC_CRGPERIPH_PERI_STAT3_swdone_clk_cpul_ppll0_div_START (22)
#define SOC_CRGPERIPH_PERI_STAT3_swdone_clk_cpul_ppll0_div_END (22)
#define SOC_CRGPERIPH_PERI_STAT3_swdone_clk_l3_ppll0_div_START (23)
#define SOC_CRGPERIPH_PERI_STAT3_swdone_clk_l3_ppll0_div_END (23)
#define SOC_CRGPERIPH_PERI_STAT3_ddrclksw2sys_START (24)
#define SOC_CRGPERIPH_PERI_STAT3_ddrclksw2sys_END (24)
#define SOC_CRGPERIPH_PERI_STAT3_ddrclksw2pll_START (25)
#define SOC_CRGPERIPH_PERI_STAT3_ddrclksw2pll_END (25)
#define SOC_CRGPERIPH_PERI_STAT3_sysbusclksw2sys_START (26)
#define SOC_CRGPERIPH_PERI_STAT3_sysbusclksw2sys_END (26)
#define SOC_CRGPERIPH_PERI_STAT3_sysbusclksw2pll_START (27)
#define SOC_CRGPERIPH_PERI_STAT3_sysbusclksw2pll_END (27)
#define SOC_CRGPERIPH_PERI_STAT3_lpmcuclksw2sys_START (28)
#define SOC_CRGPERIPH_PERI_STAT3_lpmcuclksw2sys_END (28)
#define SOC_CRGPERIPH_PERI_STAT3_lpmcuclksw2pll_START (29)
#define SOC_CRGPERIPH_PERI_STAT3_lpmcuclksw2pll_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sel_pll_ctrl_tp_peri : 5;
        unsigned int reserved : 11;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PERI_CTRL0_UNION;
#endif
#define SOC_CRGPERIPH_PERI_CTRL0_sel_pll_ctrl_tp_peri_START (0)
#define SOC_CRGPERIPH_PERI_CTRL0_sel_pll_ctrl_tp_peri_END (4)
#define SOC_CRGPERIPH_PERI_CTRL0_bitmasken_START (16)
#define SOC_CRGPERIPH_PERI_CTRL0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 5;
        unsigned int a53_0_ocldo_iso_byppass : 1;
        unsigned int a53_1_ocldo_iso_byppass : 1;
        unsigned int reserved_1 : 9;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PERI_CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PERI_CTRL1_a53_0_ocldo_iso_byppass_START (5)
#define SOC_CRGPERIPH_PERI_CTRL1_a53_0_ocldo_iso_byppass_END (5)
#define SOC_CRGPERIPH_PERI_CTRL1_a53_1_ocldo_iso_byppass_START (6)
#define SOC_CRGPERIPH_PERI_CTRL1_a53_1_ocldo_iso_byppass_END (6)
#define SOC_CRGPERIPH_PERI_CTRL1_bitmasken_START (16)
#define SOC_CRGPERIPH_PERI_CTRL1_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int noc_timeout_en : 1;
        unsigned int ccpu5g_ccpu_fig_n : 2;
        unsigned int l2hac_ccpu_fig_n : 2;
        unsigned int reserved_0 : 1;
        unsigned int mem_ctrl_pgt_lpm3 : 6;
        unsigned int mcpu_fiq_n : 2;
        unsigned int reserved_1 : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PERI_CTRL3_UNION;
#endif
#define SOC_CRGPERIPH_PERI_CTRL3_noc_timeout_en_START (0)
#define SOC_CRGPERIPH_PERI_CTRL3_noc_timeout_en_END (0)
#define SOC_CRGPERIPH_PERI_CTRL3_ccpu5g_ccpu_fig_n_START (1)
#define SOC_CRGPERIPH_PERI_CTRL3_ccpu5g_ccpu_fig_n_END (2)
#define SOC_CRGPERIPH_PERI_CTRL3_l2hac_ccpu_fig_n_START (3)
#define SOC_CRGPERIPH_PERI_CTRL3_l2hac_ccpu_fig_n_END (4)
#define SOC_CRGPERIPH_PERI_CTRL3_mem_ctrl_pgt_lpm3_START (6)
#define SOC_CRGPERIPH_PERI_CTRL3_mem_ctrl_pgt_lpm3_END (11)
#define SOC_CRGPERIPH_PERI_CTRL3_mcpu_fiq_n_START (12)
#define SOC_CRGPERIPH_PERI_CTRL3_mcpu_fiq_n_END (13)
#define SOC_CRGPERIPH_PERI_CTRL3_bitmasken_START (16)
#define SOC_CRGPERIPH_PERI_CTRL3_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 1;
        unsigned int reserved_1: 1;
        unsigned int reserved_2: 1;
        unsigned int reserved_3: 1;
        unsigned int reserved_4: 1;
        unsigned int reserved_5: 1;
        unsigned int reserved_6: 1;
        unsigned int reserved_7: 1;
        unsigned int reserved_8: 1;
        unsigned int reserved_9: 1;
        unsigned int reserved_10: 1;
        unsigned int reserved_11: 1;
        unsigned int reserved_12: 1;
        unsigned int reserved_13: 1;
        unsigned int reserved_14: 1;
        unsigned int reserved_15: 1;
        unsigned int reserved_16: 1;
        unsigned int reserved_17: 1;
        unsigned int reserved_18: 1;
        unsigned int reserved_19: 1;
        unsigned int reserved_20: 1;
        unsigned int reserved_21: 1;
        unsigned int reserved_22: 1;
        unsigned int reserved_23: 1;
        unsigned int reserved_24: 1;
        unsigned int reserved_25: 1;
        unsigned int reserved_26: 1;
        unsigned int reserved_27: 1;
        unsigned int reserved_28: 1;
        unsigned int reserved_29: 1;
        unsigned int reserved_30: 1;
        unsigned int reserved_31: 1;
    } reg;
} SOC_CRGPERIPH_ISOEN1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 1;
        unsigned int reserved_1: 1;
        unsigned int reserved_2: 1;
        unsigned int reserved_3: 1;
        unsigned int reserved_4: 1;
        unsigned int reserved_5: 1;
        unsigned int reserved_6: 1;
        unsigned int reserved_7: 1;
        unsigned int reserved_8: 1;
        unsigned int reserved_9: 1;
        unsigned int reserved_10: 1;
        unsigned int reserved_11: 1;
        unsigned int reserved_12: 1;
        unsigned int reserved_13: 1;
        unsigned int reserved_14: 1;
        unsigned int reserved_15: 1;
        unsigned int reserved_16: 1;
        unsigned int reserved_17: 1;
        unsigned int reserved_18: 1;
        unsigned int reserved_19: 1;
        unsigned int reserved_20: 1;
        unsigned int reserved_21: 1;
        unsigned int reserved_22: 1;
        unsigned int reserved_23: 1;
        unsigned int reserved_24: 1;
        unsigned int reserved_25: 1;
        unsigned int reserved_26: 1;
        unsigned int reserved_27: 1;
        unsigned int reserved_28: 1;
        unsigned int reserved_29: 1;
        unsigned int reserved_30: 1;
        unsigned int reserved_31: 1;
    } reg;
} SOC_CRGPERIPH_ISODIS1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 1;
        unsigned int reserved_1: 1;
        unsigned int reserved_2: 1;
        unsigned int reserved_3: 1;
        unsigned int reserved_4: 1;
        unsigned int reserved_5: 1;
        unsigned int reserved_6: 1;
        unsigned int reserved_7: 1;
        unsigned int reserved_8: 1;
        unsigned int reserved_9: 1;
        unsigned int reserved_10: 1;
        unsigned int reserved_11: 1;
        unsigned int reserved_12: 1;
        unsigned int reserved_13: 1;
        unsigned int reserved_14: 1;
        unsigned int reserved_15: 1;
        unsigned int reserved_16: 1;
        unsigned int reserved_17: 1;
        unsigned int reserved_18: 1;
        unsigned int reserved_19: 1;
        unsigned int reserved_20: 1;
        unsigned int reserved_21: 1;
        unsigned int reserved_22: 1;
        unsigned int reserved_23: 1;
        unsigned int reserved_24: 1;
        unsigned int reserved_25: 1;
        unsigned int reserved_26: 1;
        unsigned int reserved_27: 1;
        unsigned int reserved_28: 1;
        unsigned int reserved_29: 1;
        unsigned int reserved_30: 1;
        unsigned int reserved_31: 1;
    } reg;
} SOC_CRGPERIPH_ISOSTAT1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int timer9_A_en_sel : 1;
        unsigned int timer9_A_en_ov : 1;
        unsigned int timer9_B_en_sel : 1;
        unsigned int timer9_B_en_ov : 1;
        unsigned int timer10_A_en_sel : 1;
        unsigned int timer10_A_en_ov : 1;
        unsigned int timer10_B_en_sel : 1;
        unsigned int timer10_B_en_ov : 1;
        unsigned int timer11_A_en_sel : 1;
        unsigned int timer11_A_en_ov : 1;
        unsigned int timer11_B_en_sel : 1;
        unsigned int timer11_B_en_ov : 1;
        unsigned int timer12_A_en_sel : 1;
        unsigned int timer12_A_en_ov : 1;
        unsigned int timer12_B_en_sel : 1;
        unsigned int timer12_B_en_ov : 1;
        unsigned int wdog1enov : 1;
        unsigned int wdog0enov : 1;
        unsigned int wd0_dbgack_byp : 1;
        unsigned int wd1_dbgack_byp : 1;
        unsigned int reserved : 12;
    } reg;
} SOC_CRGPERIPH_PERTIMECTRL_UNION;
#endif
#define SOC_CRGPERIPH_PERTIMECTRL_timer9_A_en_sel_START (0)
#define SOC_CRGPERIPH_PERTIMECTRL_timer9_A_en_sel_END (0)
#define SOC_CRGPERIPH_PERTIMECTRL_timer9_A_en_ov_START (1)
#define SOC_CRGPERIPH_PERTIMECTRL_timer9_A_en_ov_END (1)
#define SOC_CRGPERIPH_PERTIMECTRL_timer9_B_en_sel_START (2)
#define SOC_CRGPERIPH_PERTIMECTRL_timer9_B_en_sel_END (2)
#define SOC_CRGPERIPH_PERTIMECTRL_timer9_B_en_ov_START (3)
#define SOC_CRGPERIPH_PERTIMECTRL_timer9_B_en_ov_END (3)
#define SOC_CRGPERIPH_PERTIMECTRL_timer10_A_en_sel_START (4)
#define SOC_CRGPERIPH_PERTIMECTRL_timer10_A_en_sel_END (4)
#define SOC_CRGPERIPH_PERTIMECTRL_timer10_A_en_ov_START (5)
#define SOC_CRGPERIPH_PERTIMECTRL_timer10_A_en_ov_END (5)
#define SOC_CRGPERIPH_PERTIMECTRL_timer10_B_en_sel_START (6)
#define SOC_CRGPERIPH_PERTIMECTRL_timer10_B_en_sel_END (6)
#define SOC_CRGPERIPH_PERTIMECTRL_timer10_B_en_ov_START (7)
#define SOC_CRGPERIPH_PERTIMECTRL_timer10_B_en_ov_END (7)
#define SOC_CRGPERIPH_PERTIMECTRL_timer11_A_en_sel_START (8)
#define SOC_CRGPERIPH_PERTIMECTRL_timer11_A_en_sel_END (8)
#define SOC_CRGPERIPH_PERTIMECTRL_timer11_A_en_ov_START (9)
#define SOC_CRGPERIPH_PERTIMECTRL_timer11_A_en_ov_END (9)
#define SOC_CRGPERIPH_PERTIMECTRL_timer11_B_en_sel_START (10)
#define SOC_CRGPERIPH_PERTIMECTRL_timer11_B_en_sel_END (10)
#define SOC_CRGPERIPH_PERTIMECTRL_timer11_B_en_ov_START (11)
#define SOC_CRGPERIPH_PERTIMECTRL_timer11_B_en_ov_END (11)
#define SOC_CRGPERIPH_PERTIMECTRL_timer12_A_en_sel_START (12)
#define SOC_CRGPERIPH_PERTIMECTRL_timer12_A_en_sel_END (12)
#define SOC_CRGPERIPH_PERTIMECTRL_timer12_A_en_ov_START (13)
#define SOC_CRGPERIPH_PERTIMECTRL_timer12_A_en_ov_END (13)
#define SOC_CRGPERIPH_PERTIMECTRL_timer12_B_en_sel_START (14)
#define SOC_CRGPERIPH_PERTIMECTRL_timer12_B_en_sel_END (14)
#define SOC_CRGPERIPH_PERTIMECTRL_timer12_B_en_ov_START (15)
#define SOC_CRGPERIPH_PERTIMECTRL_timer12_B_en_ov_END (15)
#define SOC_CRGPERIPH_PERTIMECTRL_wdog1enov_START (16)
#define SOC_CRGPERIPH_PERTIMECTRL_wdog1enov_END (16)
#define SOC_CRGPERIPH_PERTIMECTRL_wdog0enov_START (17)
#define SOC_CRGPERIPH_PERTIMECTRL_wdog0enov_END (17)
#define SOC_CRGPERIPH_PERTIMECTRL_wd0_dbgack_byp_START (18)
#define SOC_CRGPERIPH_PERTIMECTRL_wd0_dbgack_byp_END (18)
#define SOC_CRGPERIPH_PERTIMECTRL_wd1_dbgack_byp_START (19)
#define SOC_CRGPERIPH_PERTIMECTRL_wd1_dbgack_byp_END (19)
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
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int fcm_core_0_iso_en : 4;
        unsigned int fcm_cluster_iso_en : 1;
        unsigned int fcm_core_1_iso_en : 4;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
    } reg;
} SOC_CRGPERIPH_ISOEN_UNION;
#endif
#define SOC_CRGPERIPH_ISOEN_fcm_core_0_iso_en_START (12)
#define SOC_CRGPERIPH_ISOEN_fcm_core_0_iso_en_END (15)
#define SOC_CRGPERIPH_ISOEN_fcm_cluster_iso_en_START (16)
#define SOC_CRGPERIPH_ISOEN_fcm_cluster_iso_en_END (16)
#define SOC_CRGPERIPH_ISOEN_fcm_core_1_iso_en_START (17)
#define SOC_CRGPERIPH_ISOEN_fcm_core_1_iso_en_END (20)
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
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int fcm_core_0_iso_en : 4;
        unsigned int fcm_cluster_iso_en : 1;
        unsigned int fcm_core_1_iso_en : 4;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
    } reg;
} SOC_CRGPERIPH_ISODIS_UNION;
#endif
#define SOC_CRGPERIPH_ISODIS_fcm_core_0_iso_en_START (12)
#define SOC_CRGPERIPH_ISODIS_fcm_core_0_iso_en_END (15)
#define SOC_CRGPERIPH_ISODIS_fcm_cluster_iso_en_START (16)
#define SOC_CRGPERIPH_ISODIS_fcm_cluster_iso_en_END (16)
#define SOC_CRGPERIPH_ISODIS_fcm_core_1_iso_en_START (17)
#define SOC_CRGPERIPH_ISODIS_fcm_core_1_iso_en_END (20)
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
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int fcm_core_0_iso_en : 4;
        unsigned int fcm_cluster_iso_en : 1;
        unsigned int fcm_core_1_iso_en : 4;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
    } reg;
} SOC_CRGPERIPH_ISOSTAT_UNION;
#endif
#define SOC_CRGPERIPH_ISOSTAT_fcm_core_0_iso_en_START (12)
#define SOC_CRGPERIPH_ISOSTAT_fcm_core_0_iso_en_END (15)
#define SOC_CRGPERIPH_ISOSTAT_fcm_cluster_iso_en_START (16)
#define SOC_CRGPERIPH_ISOSTAT_fcm_cluster_iso_en_END (16)
#define SOC_CRGPERIPH_ISOSTAT_fcm_core_1_iso_en_START (17)
#define SOC_CRGPERIPH_ISOSTAT_fcm_core_1_iso_en_END (20)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 1;
        unsigned int reserved_1: 1;
        unsigned int reserved_2: 1;
        unsigned int reserved_3: 1;
        unsigned int reserved_4: 1;
        unsigned int reserved_5: 1;
        unsigned int reserved_6: 1;
        unsigned int reserved_7: 1;
        unsigned int reserved_8: 1;
        unsigned int reserved_9: 1;
        unsigned int reserved_10: 1;
        unsigned int fcm_pwren : 8;
        unsigned int reserved_11: 1;
        unsigned int reserved_12: 1;
        unsigned int reserved_13: 1;
        unsigned int reserved_14: 1;
        unsigned int reserved_15: 1;
        unsigned int reserved_16: 1;
        unsigned int reserved_17: 1;
        unsigned int reserved_18: 1;
        unsigned int reserved_19: 1;
        unsigned int reserved_20: 4;
    } reg;
} SOC_CRGPERIPH_PERPWREN_UNION;
#endif
#define SOC_CRGPERIPH_PERPWREN_fcm_pwren_START (11)
#define SOC_CRGPERIPH_PERPWREN_fcm_pwren_END (18)
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
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10: 1;
        unsigned int fcm_pwrdis : 8;
        unsigned int reserved_11: 1;
        unsigned int reserved_12: 1;
        unsigned int reserved_13: 1;
        unsigned int reserved_14: 1;
        unsigned int reserved_15: 1;
        unsigned int reserved_16: 1;
        unsigned int reserved_17: 1;
        unsigned int reserved_18: 1;
        unsigned int reserved_19: 1;
        unsigned int reserved_20: 4;
    } reg;
} SOC_CRGPERIPH_PERPWRDIS_UNION;
#endif
#define SOC_CRGPERIPH_PERPWRDIS_fcm_pwrdis_START (11)
#define SOC_CRGPERIPH_PERPWRDIS_fcm_pwrdis_END (18)
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
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int fcm_pwrstat : 8;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 4;
    } reg;
} SOC_CRGPERIPH_PERPWRSTAT_UNION;
#endif
#define SOC_CRGPERIPH_PERPWRSTAT_fcm_pwrstat_START (11)
#define SOC_CRGPERIPH_PERPWRSTAT_fcm_pwrstat_END (18)
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
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int fcm_little_core0_pwrack : 1;
        unsigned int fcm_little_core1_pwrack : 1;
        unsigned int fcm_little_core2_pwrack : 1;
        unsigned int fcm_little_core3_pwrack : 1;
        unsigned int fcm_middle_core0_pwrack : 1;
        unsigned int fcm_middle_core1_pwrack : 1;
        unsigned int fcm_big_core0_pwrack : 1;
        unsigned int fcm_big_core1_pwrack : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 1;
    } reg;
} SOC_CRGPERIPH_PERPWRACK_UNION;
#endif
#define SOC_CRGPERIPH_PERPWRACK_fcm_little_core0_pwrack_START (11)
#define SOC_CRGPERIPH_PERPWRACK_fcm_little_core0_pwrack_END (11)
#define SOC_CRGPERIPH_PERPWRACK_fcm_little_core1_pwrack_START (12)
#define SOC_CRGPERIPH_PERPWRACK_fcm_little_core1_pwrack_END (12)
#define SOC_CRGPERIPH_PERPWRACK_fcm_little_core2_pwrack_START (13)
#define SOC_CRGPERIPH_PERPWRACK_fcm_little_core2_pwrack_END (13)
#define SOC_CRGPERIPH_PERPWRACK_fcm_little_core3_pwrack_START (14)
#define SOC_CRGPERIPH_PERPWRACK_fcm_little_core3_pwrack_END (14)
#define SOC_CRGPERIPH_PERPWRACK_fcm_middle_core0_pwrack_START (15)
#define SOC_CRGPERIPH_PERPWRACK_fcm_middle_core0_pwrack_END (15)
#define SOC_CRGPERIPH_PERPWRACK_fcm_middle_core1_pwrack_START (16)
#define SOC_CRGPERIPH_PERPWRACK_fcm_middle_core1_pwrack_END (16)
#define SOC_CRGPERIPH_PERPWRACK_fcm_big_core0_pwrack_START (17)
#define SOC_CRGPERIPH_PERPWRACK_fcm_big_core0_pwrack_END (17)
#define SOC_CRGPERIPH_PERPWRACK_fcm_big_core1_pwrack_START (18)
#define SOC_CRGPERIPH_PERPWRACK_fcm_big_core1_pwrack_END (18)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int clkon_pclkdbg : 1;
        unsigned int clkon_pclkdbg_top : 1;
        unsigned int reserved_0 : 8;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_FCM_CLKEN_UNION;
#endif
#define SOC_CRGPERIPH_FCM_CLKEN_clkon_pclkdbg_START (0)
#define SOC_CRGPERIPH_FCM_CLKEN_clkon_pclkdbg_END (0)
#define SOC_CRGPERIPH_FCM_CLKEN_clkon_pclkdbg_top_START (1)
#define SOC_CRGPERIPH_FCM_CLKEN_clkon_pclkdbg_top_END (1)
#define SOC_CRGPERIPH_FCM_CLKEN_bitmasken_START (16)
#define SOC_CRGPERIPH_FCM_CLKEN_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int core_por_rst_req_little : 4;
        unsigned int core_por_rst_req_middle : 3;
        unsigned int core_por_rst_req_big : 1;
        unsigned int presetdbg_rst_req : 1;
        unsigned int coresight_soft_rst_req : 1;
        unsigned int cluster_soft_rst_req : 1;
        unsigned int scu_soft_rst_req : 1;
        unsigned int reserved : 20;
    } reg;
} SOC_CRGPERIPH_FCM_RSTEN_UNION;
#endif
#define SOC_CRGPERIPH_FCM_RSTEN_core_por_rst_req_little_START (0)
#define SOC_CRGPERIPH_FCM_RSTEN_core_por_rst_req_little_END (3)
#define SOC_CRGPERIPH_FCM_RSTEN_core_por_rst_req_middle_START (4)
#define SOC_CRGPERIPH_FCM_RSTEN_core_por_rst_req_middle_END (6)
#define SOC_CRGPERIPH_FCM_RSTEN_core_por_rst_req_big_START (7)
#define SOC_CRGPERIPH_FCM_RSTEN_core_por_rst_req_big_END (7)
#define SOC_CRGPERIPH_FCM_RSTEN_presetdbg_rst_req_START (8)
#define SOC_CRGPERIPH_FCM_RSTEN_presetdbg_rst_req_END (8)
#define SOC_CRGPERIPH_FCM_RSTEN_coresight_soft_rst_req_START (9)
#define SOC_CRGPERIPH_FCM_RSTEN_coresight_soft_rst_req_END (9)
#define SOC_CRGPERIPH_FCM_RSTEN_cluster_soft_rst_req_START (10)
#define SOC_CRGPERIPH_FCM_RSTEN_cluster_soft_rst_req_END (10)
#define SOC_CRGPERIPH_FCM_RSTEN_scu_soft_rst_req_START (11)
#define SOC_CRGPERIPH_FCM_RSTEN_scu_soft_rst_req_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int core_por_rst_req_little : 4;
        unsigned int core_por_rst_req_middle : 3;
        unsigned int core_por_rst_req_big : 1;
        unsigned int presetdbg_rst_req : 1;
        unsigned int coresight_soft_rst_req : 1;
        unsigned int cluster_soft_rst_req : 1;
        unsigned int scu_soft_rst_req : 1;
        unsigned int reserved : 20;
    } reg;
} SOC_CRGPERIPH_FCM_RSTDIS_UNION;
#endif
#define SOC_CRGPERIPH_FCM_RSTDIS_core_por_rst_req_little_START (0)
#define SOC_CRGPERIPH_FCM_RSTDIS_core_por_rst_req_little_END (3)
#define SOC_CRGPERIPH_FCM_RSTDIS_core_por_rst_req_middle_START (4)
#define SOC_CRGPERIPH_FCM_RSTDIS_core_por_rst_req_middle_END (6)
#define SOC_CRGPERIPH_FCM_RSTDIS_core_por_rst_req_big_START (7)
#define SOC_CRGPERIPH_FCM_RSTDIS_core_por_rst_req_big_END (7)
#define SOC_CRGPERIPH_FCM_RSTDIS_presetdbg_rst_req_START (8)
#define SOC_CRGPERIPH_FCM_RSTDIS_presetdbg_rst_req_END (8)
#define SOC_CRGPERIPH_FCM_RSTDIS_coresight_soft_rst_req_START (9)
#define SOC_CRGPERIPH_FCM_RSTDIS_coresight_soft_rst_req_END (9)
#define SOC_CRGPERIPH_FCM_RSTDIS_cluster_soft_rst_req_START (10)
#define SOC_CRGPERIPH_FCM_RSTDIS_cluster_soft_rst_req_END (10)
#define SOC_CRGPERIPH_FCM_RSTDIS_scu_soft_rst_req_START (11)
#define SOC_CRGPERIPH_FCM_RSTDIS_scu_soft_rst_req_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int core_por_rst_req_little : 4;
        unsigned int core_por_rst_req_middle : 3;
        unsigned int core_por_rst_req_big : 1;
        unsigned int presetdbg_rst_req : 1;
        unsigned int coresight_soft_rst_req : 1;
        unsigned int cluster_soft_rst_req : 1;
        unsigned int scu_soft_rst_req : 1;
        unsigned int reserved : 20;
    } reg;
} SOC_CRGPERIPH_FCM_RSTSTAT_UNION;
#endif
#define SOC_CRGPERIPH_FCM_RSTSTAT_core_por_rst_req_little_START (0)
#define SOC_CRGPERIPH_FCM_RSTSTAT_core_por_rst_req_little_END (3)
#define SOC_CRGPERIPH_FCM_RSTSTAT_core_por_rst_req_middle_START (4)
#define SOC_CRGPERIPH_FCM_RSTSTAT_core_por_rst_req_middle_END (6)
#define SOC_CRGPERIPH_FCM_RSTSTAT_core_por_rst_req_big_START (7)
#define SOC_CRGPERIPH_FCM_RSTSTAT_core_por_rst_req_big_END (7)
#define SOC_CRGPERIPH_FCM_RSTSTAT_presetdbg_rst_req_START (8)
#define SOC_CRGPERIPH_FCM_RSTSTAT_presetdbg_rst_req_END (8)
#define SOC_CRGPERIPH_FCM_RSTSTAT_coresight_soft_rst_req_START (9)
#define SOC_CRGPERIPH_FCM_RSTSTAT_coresight_soft_rst_req_END (9)
#define SOC_CRGPERIPH_FCM_RSTSTAT_cluster_soft_rst_req_START (10)
#define SOC_CRGPERIPH_FCM_RSTSTAT_cluster_soft_rst_req_END (10)
#define SOC_CRGPERIPH_FCM_RSTSTAT_scu_soft_rst_req_START (11)
#define SOC_CRGPERIPH_FCM_RSTSTAT_scu_soft_rst_req_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csysreq_adb_fcm0 : 1;
        unsigned int reserved_0 : 1;
        unsigned int cactive_adb_fcm0 : 1;
        unsigned int csysaccept_adb_fcm0 : 1;
        unsigned int csysdeney_adb_fcm0 : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_FCM0_ADBLPSTAT_UNION;
#endif
#define SOC_CRGPERIPH_FCM0_ADBLPSTAT_csysreq_adb_fcm0_START (0)
#define SOC_CRGPERIPH_FCM0_ADBLPSTAT_csysreq_adb_fcm0_END (0)
#define SOC_CRGPERIPH_FCM0_ADBLPSTAT_cactive_adb_fcm0_START (2)
#define SOC_CRGPERIPH_FCM0_ADBLPSTAT_cactive_adb_fcm0_END (2)
#define SOC_CRGPERIPH_FCM0_ADBLPSTAT_csysaccept_adb_fcm0_START (3)
#define SOC_CRGPERIPH_FCM0_ADBLPSTAT_csysaccept_adb_fcm0_END (3)
#define SOC_CRGPERIPH_FCM0_ADBLPSTAT_csysdeney_adb_fcm0_START (4)
#define SOC_CRGPERIPH_FCM0_ADBLPSTAT_csysdeney_adb_fcm0_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_little_core_dbgpwrdup : 4;
        unsigned int cpu_middle_core_dbgpwrdup : 3;
        unsigned int cpu_big_core_dbgpwrdup : 1;
        unsigned int reserved : 24;
    } reg;
} SOC_CRGPERIPH_FCM_DBGPWRDUP_UNION;
#endif
#define SOC_CRGPERIPH_FCM_DBGPWRDUP_cpu_little_core_dbgpwrdup_START (0)
#define SOC_CRGPERIPH_FCM_DBGPWRDUP_cpu_little_core_dbgpwrdup_END (3)
#define SOC_CRGPERIPH_FCM_DBGPWRDUP_cpu_middle_core_dbgpwrdup_START (4)
#define SOC_CRGPERIPH_FCM_DBGPWRDUP_cpu_middle_core_dbgpwrdup_END (6)
#define SOC_CRGPERIPH_FCM_DBGPWRDUP_cpu_big_core_dbgpwrdup_START (7)
#define SOC_CRGPERIPH_FCM_DBGPWRDUP_cpu_big_core_dbgpwrdup_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 3;
        unsigned int reserved_1: 3;
        unsigned int reserved_2: 3;
        unsigned int reserved_3: 3;
        unsigned int reserved_4: 4;
        unsigned int reserved_5: 7;
        unsigned int reserved_6: 1;
        unsigned int reserved_7: 8;
    } reg;
} SOC_CRGPERIPH_A53_OCLDOVSET_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a53_ocldo_req : 4;
        unsigned int a53_core0_ocldo_stat : 3;
        unsigned int a53_core0_mtcmos_stat : 1;
        unsigned int a53_core1_ocldo_stat : 3;
        unsigned int a53_core1_mtcmos_stat : 1;
        unsigned int a53_core2_ocldo_stat : 3;
        unsigned int a53_core2_mtcmos_stat : 1;
        unsigned int a53_core3_ocldo_stat : 3;
        unsigned int a53_core3_mtcmos_stat : 1;
        unsigned int reserved_0 : 10;
        unsigned int reserved_1 : 2;
    } reg;
} SOC_CRGPERIPH_A53_OCLDO_STAT_UNION;
#endif
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_ocldo_req_START (0)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_ocldo_req_END (3)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core0_ocldo_stat_START (4)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core0_ocldo_stat_END (6)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core0_mtcmos_stat_START (7)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core0_mtcmos_stat_END (7)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core1_ocldo_stat_START (8)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core1_ocldo_stat_END (10)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core1_mtcmos_stat_START (11)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core1_mtcmos_stat_END (11)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core2_ocldo_stat_START (12)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core2_ocldo_stat_END (14)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core2_mtcmos_stat_START (15)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core2_mtcmos_stat_END (15)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core3_ocldo_stat_START (16)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core3_ocldo_stat_END (18)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core3_mtcmos_stat_START (19)
#define SOC_CRGPERIPH_A53_OCLDO_STAT_a53_core3_mtcmos_stat_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pclkdbg_sel_iomcu : 1;
        unsigned int pclkdbg_clkoff_sys : 1;
        unsigned int pclkdbg_to_a53cfg_clkoff_sys_0 : 1;
        unsigned int pclkdbg_to_a53cfg_clkoff_sys_1 : 1;
        unsigned int pclkdbg_to_iomcu_clkoff_sys : 1;
        unsigned int pclkdbg_to_lpmcu_clkoff_sys : 1;
        unsigned int traceclkin_clkoff_sys : 1;
        unsigned int atclkoff_sys : 1;
        unsigned int atclk_to_iomcu_clkoff_sys : 1;
        unsigned int atclk_to_a53crg_clkoff_sys_0 : 1;
        unsigned int atclk_to_a53crg_clkoff_sys_1 : 1;
        unsigned int cs_soft_rst_req : 1;
        unsigned int traceclkin_sel : 2;
        unsigned int atclk_to_npucrg_clkoff_sys : 1;
        unsigned int cs_cssys_rst_req : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PERI_CTRL2_L_UNION;
#endif
#define SOC_CRGPERIPH_PERI_CTRL2_L_pclkdbg_sel_iomcu_START (0)
#define SOC_CRGPERIPH_PERI_CTRL2_L_pclkdbg_sel_iomcu_END (0)
#define SOC_CRGPERIPH_PERI_CTRL2_L_pclkdbg_clkoff_sys_START (1)
#define SOC_CRGPERIPH_PERI_CTRL2_L_pclkdbg_clkoff_sys_END (1)
#define SOC_CRGPERIPH_PERI_CTRL2_L_pclkdbg_to_a53cfg_clkoff_sys_0_START (2)
#define SOC_CRGPERIPH_PERI_CTRL2_L_pclkdbg_to_a53cfg_clkoff_sys_0_END (2)
#define SOC_CRGPERIPH_PERI_CTRL2_L_pclkdbg_to_a53cfg_clkoff_sys_1_START (3)
#define SOC_CRGPERIPH_PERI_CTRL2_L_pclkdbg_to_a53cfg_clkoff_sys_1_END (3)
#define SOC_CRGPERIPH_PERI_CTRL2_L_pclkdbg_to_iomcu_clkoff_sys_START (4)
#define SOC_CRGPERIPH_PERI_CTRL2_L_pclkdbg_to_iomcu_clkoff_sys_END (4)
#define SOC_CRGPERIPH_PERI_CTRL2_L_pclkdbg_to_lpmcu_clkoff_sys_START (5)
#define SOC_CRGPERIPH_PERI_CTRL2_L_pclkdbg_to_lpmcu_clkoff_sys_END (5)
#define SOC_CRGPERIPH_PERI_CTRL2_L_traceclkin_clkoff_sys_START (6)
#define SOC_CRGPERIPH_PERI_CTRL2_L_traceclkin_clkoff_sys_END (6)
#define SOC_CRGPERIPH_PERI_CTRL2_L_atclkoff_sys_START (7)
#define SOC_CRGPERIPH_PERI_CTRL2_L_atclkoff_sys_END (7)
#define SOC_CRGPERIPH_PERI_CTRL2_L_atclk_to_iomcu_clkoff_sys_START (8)
#define SOC_CRGPERIPH_PERI_CTRL2_L_atclk_to_iomcu_clkoff_sys_END (8)
#define SOC_CRGPERIPH_PERI_CTRL2_L_atclk_to_a53crg_clkoff_sys_0_START (9)
#define SOC_CRGPERIPH_PERI_CTRL2_L_atclk_to_a53crg_clkoff_sys_0_END (9)
#define SOC_CRGPERIPH_PERI_CTRL2_L_atclk_to_a53crg_clkoff_sys_1_START (10)
#define SOC_CRGPERIPH_PERI_CTRL2_L_atclk_to_a53crg_clkoff_sys_1_END (10)
#define SOC_CRGPERIPH_PERI_CTRL2_L_cs_soft_rst_req_START (11)
#define SOC_CRGPERIPH_PERI_CTRL2_L_cs_soft_rst_req_END (11)
#define SOC_CRGPERIPH_PERI_CTRL2_L_traceclkin_sel_START (12)
#define SOC_CRGPERIPH_PERI_CTRL2_L_traceclkin_sel_END (13)
#define SOC_CRGPERIPH_PERI_CTRL2_L_atclk_to_npucrg_clkoff_sys_START (14)
#define SOC_CRGPERIPH_PERI_CTRL2_L_atclk_to_npucrg_clkoff_sys_END (14)
#define SOC_CRGPERIPH_PERI_CTRL2_L_cs_cssys_rst_req_START (15)
#define SOC_CRGPERIPH_PERI_CTRL2_L_cs_cssys_rst_req_END (15)
#define SOC_CRGPERIPH_PERI_CTRL2_L_bitmasken_START (16)
#define SOC_CRGPERIPH_PERI_CTRL2_L_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int atclk_to_modem_clkoff_sys : 1;
        unsigned int pclkdbg_to_modem_clkoff_sys : 1;
        unsigned int modem_cssys_rst_req : 1;
        unsigned int pclkdbg_to_npucrg_clkoff_sys : 1;
        unsigned int atclk_to_vcodec_clkoff_sys : 1;
        unsigned int pclkdbg_to_vcodec_clkoff_sys : 1;
        unsigned int div_time_stamp : 3;
        unsigned int gt_clk_cssys_atclk : 1;
        unsigned int ip_rst_cssys_atclk : 1;
        unsigned int gt_clk_time_stamp : 1;
        unsigned int ip_rst_time_stamp : 1;
        unsigned int div_pclkdbg_sel : 2;
        unsigned int reserved : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PERI_CTRL2_H_UNION;
#endif
#define SOC_CRGPERIPH_PERI_CTRL2_H_atclk_to_modem_clkoff_sys_START (0)
#define SOC_CRGPERIPH_PERI_CTRL2_H_atclk_to_modem_clkoff_sys_END (0)
#define SOC_CRGPERIPH_PERI_CTRL2_H_pclkdbg_to_modem_clkoff_sys_START (1)
#define SOC_CRGPERIPH_PERI_CTRL2_H_pclkdbg_to_modem_clkoff_sys_END (1)
#define SOC_CRGPERIPH_PERI_CTRL2_H_modem_cssys_rst_req_START (2)
#define SOC_CRGPERIPH_PERI_CTRL2_H_modem_cssys_rst_req_END (2)
#define SOC_CRGPERIPH_PERI_CTRL2_H_pclkdbg_to_npucrg_clkoff_sys_START (3)
#define SOC_CRGPERIPH_PERI_CTRL2_H_pclkdbg_to_npucrg_clkoff_sys_END (3)
#define SOC_CRGPERIPH_PERI_CTRL2_H_atclk_to_vcodec_clkoff_sys_START (4)
#define SOC_CRGPERIPH_PERI_CTRL2_H_atclk_to_vcodec_clkoff_sys_END (4)
#define SOC_CRGPERIPH_PERI_CTRL2_H_pclkdbg_to_vcodec_clkoff_sys_START (5)
#define SOC_CRGPERIPH_PERI_CTRL2_H_pclkdbg_to_vcodec_clkoff_sys_END (5)
#define SOC_CRGPERIPH_PERI_CTRL2_H_div_time_stamp_START (6)
#define SOC_CRGPERIPH_PERI_CTRL2_H_div_time_stamp_END (8)
#define SOC_CRGPERIPH_PERI_CTRL2_H_gt_clk_cssys_atclk_START (9)
#define SOC_CRGPERIPH_PERI_CTRL2_H_gt_clk_cssys_atclk_END (9)
#define SOC_CRGPERIPH_PERI_CTRL2_H_ip_rst_cssys_atclk_START (10)
#define SOC_CRGPERIPH_PERI_CTRL2_H_ip_rst_cssys_atclk_END (10)
#define SOC_CRGPERIPH_PERI_CTRL2_H_gt_clk_time_stamp_START (11)
#define SOC_CRGPERIPH_PERI_CTRL2_H_gt_clk_time_stamp_END (11)
#define SOC_CRGPERIPH_PERI_CTRL2_H_ip_rst_time_stamp_START (12)
#define SOC_CRGPERIPH_PERI_CTRL2_H_ip_rst_time_stamp_END (12)
#define SOC_CRGPERIPH_PERI_CTRL2_H_div_pclkdbg_sel_START (13)
#define SOC_CRGPERIPH_PERI_CTRL2_H_div_pclkdbg_sel_END (14)
#define SOC_CRGPERIPH_PERI_CTRL2_H_bitmasken_START (16)
#define SOC_CRGPERIPH_PERI_CTRL2_H_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csysreq_adb_fcm1 : 1;
        unsigned int reserved_0 : 1;
        unsigned int cactive_adb_fcm1 : 1;
        unsigned int csysaccept_adb_fcm1 : 1;
        unsigned int csysdeney_adb_fcm1 : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_FCM1_ADBLPSTAT_UNION;
#endif
#define SOC_CRGPERIPH_FCM1_ADBLPSTAT_csysreq_adb_fcm1_START (0)
#define SOC_CRGPERIPH_FCM1_ADBLPSTAT_csysreq_adb_fcm1_END (0)
#define SOC_CRGPERIPH_FCM1_ADBLPSTAT_cactive_adb_fcm1_START (2)
#define SOC_CRGPERIPH_FCM1_ADBLPSTAT_cactive_adb_fcm1_END (2)
#define SOC_CRGPERIPH_FCM1_ADBLPSTAT_csysaccept_adb_fcm1_START (3)
#define SOC_CRGPERIPH_FCM1_ADBLPSTAT_csysaccept_adb_fcm1_END (3)
#define SOC_CRGPERIPH_FCM1_ADBLPSTAT_csysdeney_adb_fcm1_START (4)
#define SOC_CRGPERIPH_FCM1_ADBLPSTAT_csysdeney_adb_fcm1_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csysreq_adb_fcm_periph : 1;
        unsigned int reserved_0 : 1;
        unsigned int cactive_adb_fcm_periph : 1;
        unsigned int csysaccept_adb_fcm_periph : 1;
        unsigned int csysdeney_adb_fcm_periph : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_FCM_PERIPH_ADBLPSTAT_UNION;
#endif
#define SOC_CRGPERIPH_FCM_PERIPH_ADBLPSTAT_csysreq_adb_fcm_periph_START (0)
#define SOC_CRGPERIPH_FCM_PERIPH_ADBLPSTAT_csysreq_adb_fcm_periph_END (0)
#define SOC_CRGPERIPH_FCM_PERIPH_ADBLPSTAT_cactive_adb_fcm_periph_START (2)
#define SOC_CRGPERIPH_FCM_PERIPH_ADBLPSTAT_cactive_adb_fcm_periph_END (2)
#define SOC_CRGPERIPH_FCM_PERIPH_ADBLPSTAT_csysaccept_adb_fcm_periph_START (3)
#define SOC_CRGPERIPH_FCM_PERIPH_ADBLPSTAT_csysaccept_adb_fcm_periph_END (3)
#define SOC_CRGPERIPH_FCM_PERIPH_ADBLPSTAT_csysdeney_adb_fcm_periph_START (4)
#define SOC_CRGPERIPH_FCM_PERIPH_ADBLPSTAT_csysdeney_adb_fcm_periph_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csysreq_adb_fcm_acp : 1;
        unsigned int reserved_0 : 1;
        unsigned int cactive_adb_fcm_acp : 1;
        unsigned int csysaccept_adb_fcm_acp : 1;
        unsigned int csysdeney_adb_fcm_acp : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_FCM_ACP_ADBLPSTAT_UNION;
#endif
#define SOC_CRGPERIPH_FCM_ACP_ADBLPSTAT_csysreq_adb_fcm_acp_START (0)
#define SOC_CRGPERIPH_FCM_ACP_ADBLPSTAT_csysreq_adb_fcm_acp_END (0)
#define SOC_CRGPERIPH_FCM_ACP_ADBLPSTAT_cactive_adb_fcm_acp_START (2)
#define SOC_CRGPERIPH_FCM_ACP_ADBLPSTAT_cactive_adb_fcm_acp_END (2)
#define SOC_CRGPERIPH_FCM_ACP_ADBLPSTAT_csysaccept_adb_fcm_acp_START (3)
#define SOC_CRGPERIPH_FCM_ACP_ADBLPSTAT_csysaccept_adb_fcm_acp_END (3)
#define SOC_CRGPERIPH_FCM_ACP_ADBLPSTAT_csysdeney_adb_fcm_acp_START (4)
#define SOC_CRGPERIPH_FCM_ACP_ADBLPSTAT_csysdeney_adb_fcm_acp_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a57_core0_ocldo_vset : 3;
        unsigned int a57_core1_ocldo_vset : 3;
        unsigned int a57_core2_ocldo_vset : 3;
        unsigned int a57_core3_ocldo_vset : 3;
        unsigned int a57_ocldo_bypass : 4;
        unsigned int ocldo_a_sel : 4;
        unsigned int reserved_0 : 4;
        unsigned int reserved_1 : 8;
    } reg;
} SOC_CRGPERIPH_MAIA_OCLDOVSET_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_OCLDOVSET_a57_core0_ocldo_vset_START (0)
#define SOC_CRGPERIPH_MAIA_OCLDOVSET_a57_core0_ocldo_vset_END (2)
#define SOC_CRGPERIPH_MAIA_OCLDOVSET_a57_core1_ocldo_vset_START (3)
#define SOC_CRGPERIPH_MAIA_OCLDOVSET_a57_core1_ocldo_vset_END (5)
#define SOC_CRGPERIPH_MAIA_OCLDOVSET_a57_core2_ocldo_vset_START (6)
#define SOC_CRGPERIPH_MAIA_OCLDOVSET_a57_core2_ocldo_vset_END (8)
#define SOC_CRGPERIPH_MAIA_OCLDOVSET_a57_core3_ocldo_vset_START (9)
#define SOC_CRGPERIPH_MAIA_OCLDOVSET_a57_core3_ocldo_vset_END (11)
#define SOC_CRGPERIPH_MAIA_OCLDOVSET_a57_ocldo_bypass_START (12)
#define SOC_CRGPERIPH_MAIA_OCLDOVSET_a57_ocldo_bypass_END (15)
#define SOC_CRGPERIPH_MAIA_OCLDOVSET_ocldo_a_sel_START (16)
#define SOC_CRGPERIPH_MAIA_OCLDOVSET_ocldo_a_sel_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int eco_pm_ccnt_q_63bit : 8;
        unsigned int l2_idle_big : 1;
        unsigned int l2_idle_little : 1;
        unsigned int l2_idle_middle : 1;
        unsigned int l3_idle : 1;
        unsigned int cpu_load_big : 1;
        unsigned int cpu_load_little : 1;
        unsigned int cpu_load_middle : 1;
        unsigned int reserved : 1;
        unsigned int core_standbywfe : 8;
        unsigned int core_standbywfi : 8;
    } reg;
} SOC_CRGPERIPH_FCM_STAT0_UNION;
#endif
#define SOC_CRGPERIPH_FCM_STAT0_eco_pm_ccnt_q_63bit_START (0)
#define SOC_CRGPERIPH_FCM_STAT0_eco_pm_ccnt_q_63bit_END (7)
#define SOC_CRGPERIPH_FCM_STAT0_l2_idle_big_START (8)
#define SOC_CRGPERIPH_FCM_STAT0_l2_idle_big_END (8)
#define SOC_CRGPERIPH_FCM_STAT0_l2_idle_little_START (9)
#define SOC_CRGPERIPH_FCM_STAT0_l2_idle_little_END (9)
#define SOC_CRGPERIPH_FCM_STAT0_l2_idle_middle_START (10)
#define SOC_CRGPERIPH_FCM_STAT0_l2_idle_middle_END (10)
#define SOC_CRGPERIPH_FCM_STAT0_l3_idle_START (11)
#define SOC_CRGPERIPH_FCM_STAT0_l3_idle_END (11)
#define SOC_CRGPERIPH_FCM_STAT0_cpu_load_big_START (12)
#define SOC_CRGPERIPH_FCM_STAT0_cpu_load_big_END (12)
#define SOC_CRGPERIPH_FCM_STAT0_cpu_load_little_START (13)
#define SOC_CRGPERIPH_FCM_STAT0_cpu_load_little_END (13)
#define SOC_CRGPERIPH_FCM_STAT0_cpu_load_middle_START (14)
#define SOC_CRGPERIPH_FCM_STAT0_cpu_load_middle_END (14)
#define SOC_CRGPERIPH_FCM_STAT0_core_standbywfe_START (16)
#define SOC_CRGPERIPH_FCM_STAT0_core_standbywfe_END (23)
#define SOC_CRGPERIPH_FCM_STAT0_core_standbywfi_START (24)
#define SOC_CRGPERIPH_FCM_STAT0_core_standbywfi_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fcm_standbywfe : 1;
        unsigned int fcm_dbgack : 1;
        unsigned int etf_fcm_full : 1;
        unsigned int fcm_standbywfi : 1;
        unsigned int vdm_standbywfi_core : 8;
        unsigned int vdm_standbywfi_fcm : 1;
        unsigned int reserved : 19;
    } reg;
} SOC_CRGPERIPH_FCM_STAT1_UNION;
#endif
#define SOC_CRGPERIPH_FCM_STAT1_fcm_standbywfe_START (0)
#define SOC_CRGPERIPH_FCM_STAT1_fcm_standbywfe_END (0)
#define SOC_CRGPERIPH_FCM_STAT1_fcm_dbgack_START (1)
#define SOC_CRGPERIPH_FCM_STAT1_fcm_dbgack_END (1)
#define SOC_CRGPERIPH_FCM_STAT1_etf_fcm_full_START (2)
#define SOC_CRGPERIPH_FCM_STAT1_etf_fcm_full_END (2)
#define SOC_CRGPERIPH_FCM_STAT1_fcm_standbywfi_START (3)
#define SOC_CRGPERIPH_FCM_STAT1_fcm_standbywfi_END (3)
#define SOC_CRGPERIPH_FCM_STAT1_vdm_standbywfi_core_START (4)
#define SOC_CRGPERIPH_FCM_STAT1_vdm_standbywfi_core_END (11)
#define SOC_CRGPERIPH_FCM_STAT1_vdm_standbywfi_fcm_START (12)
#define SOC_CRGPERIPH_FCM_STAT1_vdm_standbywfi_fcm_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a57_ocldo_req : 4;
        unsigned int a57_core0_retention_stat : 3;
        unsigned int a57_core0_ocldo_req : 1;
        unsigned int a57_core0_ocldo_ack : 1;
        unsigned int a57_core0_qreq_n : 1;
        unsigned int a57_core0_qaccept_n : 1;
        unsigned int a57_core1_retention_stat : 3;
        unsigned int a57_core1_ocldo_req : 1;
        unsigned int a57_core1_ocldo_ack : 1;
        unsigned int a57_core1_qreq_n : 1;
        unsigned int a57_core1_qaccept_n : 1;
        unsigned int a57_core2_retention_stat : 3;
        unsigned int a57_core2_ocldo_req : 1;
        unsigned int a57_core2_ocldo_ack : 1;
        unsigned int a57_core2_qreq_n : 1;
        unsigned int a57_core2_qaccept_n : 1;
        unsigned int a57_core3_retention_stat : 3;
        unsigned int a57_core3_ocldo_req : 1;
        unsigned int a57_core3_ocldo_ack : 1;
        unsigned int a57_core3_qreq_n : 1;
        unsigned int a57_core3_qaccept_n : 1;
    } reg;
} SOC_CRGPERIPH_MAIA_OCLDO_STAT_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_ocldo_req_START (0)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_ocldo_req_END (3)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core0_retention_stat_START (4)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core0_retention_stat_END (6)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core0_ocldo_req_START (7)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core0_ocldo_req_END (7)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core0_ocldo_ack_START (8)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core0_ocldo_ack_END (8)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core0_qreq_n_START (9)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core0_qreq_n_END (9)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core0_qaccept_n_START (10)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core0_qaccept_n_END (10)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core1_retention_stat_START (11)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core1_retention_stat_END (13)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core1_ocldo_req_START (14)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core1_ocldo_req_END (14)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core1_ocldo_ack_START (15)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core1_ocldo_ack_END (15)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core1_qreq_n_START (16)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core1_qreq_n_END (16)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core1_qaccept_n_START (17)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core1_qaccept_n_END (17)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core2_retention_stat_START (18)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core2_retention_stat_END (20)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core2_ocldo_req_START (21)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core2_ocldo_req_END (21)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core2_ocldo_ack_START (22)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core2_ocldo_ack_END (22)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core2_qreq_n_START (23)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core2_qreq_n_END (23)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core2_qaccept_n_START (24)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core2_qaccept_n_END (24)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core3_retention_stat_START (25)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core3_retention_stat_END (27)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core3_ocldo_req_START (28)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core3_ocldo_req_END (28)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core3_ocldo_ack_START (29)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core3_ocldo_ack_END (29)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core3_qreq_n_START (30)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core3_qreq_n_END (30)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core3_qaccept_n_START (31)
#define SOC_CRGPERIPH_MAIA_OCLDO_STAT_a57_core3_qaccept_n_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csysreq_etr_lpi : 1;
        unsigned int csysack_etr_lpi : 1;
        unsigned int cactive_etr_lpi : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_CRGPERIPH_CORESIGHTLPSTAT_UNION;
#endif
#define SOC_CRGPERIPH_CORESIGHTLPSTAT_csysreq_etr_lpi_START (0)
#define SOC_CRGPERIPH_CORESIGHTLPSTAT_csysreq_etr_lpi_END (0)
#define SOC_CRGPERIPH_CORESIGHTLPSTAT_csysack_etr_lpi_START (1)
#define SOC_CRGPERIPH_CORESIGHTLPSTAT_csysack_etr_lpi_END (1)
#define SOC_CRGPERIPH_CORESIGHTLPSTAT_cactive_etr_lpi_START (2)
#define SOC_CRGPERIPH_CORESIGHTLPSTAT_cactive_etr_lpi_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mem_ctrl_s_cssys : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_CRGPERIPH_CORESIGHT_CTRL_UNION;
#endif
#define SOC_CRGPERIPH_CORESIGHT_CTRL_mem_ctrl_s_cssys_START (0)
#define SOC_CRGPERIPH_CORESIGHT_CTRL_mem_ctrl_s_cssys_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int etf_a53_1_full : 1;
        unsigned int etf_a53_0_full : 1;
        unsigned int etf_top_full : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_CRGPERIPH_CORESIGHTETFINT_UNION;
#endif
#define SOC_CRGPERIPH_CORESIGHTETFINT_etf_a53_1_full_START (0)
#define SOC_CRGPERIPH_CORESIGHTETFINT_etf_a53_1_full_END (0)
#define SOC_CRGPERIPH_CORESIGHTETFINT_etf_a53_0_full_START (1)
#define SOC_CRGPERIPH_CORESIGHTETFINT_etf_a53_0_full_END (1)
#define SOC_CRGPERIPH_CORESIGHTETFINT_etf_top_full_START (2)
#define SOC_CRGPERIPH_CORESIGHTETFINT_etf_top_full_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int etr_top_full : 1;
        unsigned int intr_a53_0_core0_gic : 1;
        unsigned int intr_a53_0_core1_gic : 1;
        unsigned int intr_a53_0_core2_gic : 1;
        unsigned int intr_a53_0_core3_gic : 1;
        unsigned int intr_a53_1_core0_gic : 1;
        unsigned int intr_a53_1_core1_gic : 1;
        unsigned int intr_a53_1_core2_gic : 1;
        unsigned int intr_a53_1_core3_gic : 1;
        unsigned int reserved : 23;
    } reg;
} SOC_CRGPERIPH_CORESIGHTETRINT_UNION;
#endif
#define SOC_CRGPERIPH_CORESIGHTETRINT_etr_top_full_START (0)
#define SOC_CRGPERIPH_CORESIGHTETRINT_etr_top_full_END (0)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_0_core0_gic_START (1)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_0_core0_gic_END (1)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_0_core1_gic_START (2)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_0_core1_gic_END (2)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_0_core2_gic_START (3)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_0_core2_gic_END (3)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_0_core3_gic_START (4)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_0_core3_gic_END (4)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_1_core0_gic_START (5)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_1_core0_gic_END (5)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_1_core1_gic_START (6)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_1_core1_gic_END (6)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_1_core2_gic_START (7)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_1_core2_gic_END (7)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_1_core3_gic_START (8)
#define SOC_CRGPERIPH_CORESIGHTETRINT_intr_a53_1_core3_gic_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int adb_gt_status_fcm_acp_slv : 1;
        unsigned int adb_gt_status_fcm_acp_mst : 1;
        unsigned int adb_gt_status_npudmss3_slv : 1;
        unsigned int adb_gt_status_npudmss3_mst : 1;
        unsigned int adb_gt_status_npudmss2_slv : 1;
        unsigned int adb_gt_status_npudmss2_mst : 1;
        unsigned int adb_gt_status_npudmss1_slv : 1;
        unsigned int adb_gt_status_npudmss1_mst : 1;
        unsigned int adb_gt_status_npudmss0_slv : 1;
        unsigned int adb_gt_status_npudmss0_mst : 1;
        unsigned int adb_gt_status_gpu3_slv : 1;
        unsigned int adb_gt_status_gpu3_mst : 1;
        unsigned int adb_gt_status_gpu2_slv : 1;
        unsigned int adb_gt_status_gpu2_mst : 1;
        unsigned int adb_gt_status_gpu1_slv : 1;
        unsigned int adb_gt_status_gpu1_mst : 1;
        unsigned int adb_gt_status_gpu0_slv : 1;
        unsigned int adb_gt_status_gpu0_mst : 1;
        unsigned int adb_gt_status_cpu1_slv : 1;
        unsigned int adb_gt_status_cpu1_mst : 1;
        unsigned int adb_gt_status_cpu0_slv : 1;
        unsigned int adb_gt_status_cpu0_mst : 1;
        unsigned int adb_gt_status_mdm4g2ddr_slv : 1;
        unsigned int adb_gt_status_mdm4g2ddr_mst : 1;
        unsigned int reserved : 8;
    } reg;
} SOC_CRGPERIPH_ADB_GT_STATUS_UNION;
#endif
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_fcm_acp_slv_START (0)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_fcm_acp_slv_END (0)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_fcm_acp_mst_START (1)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_fcm_acp_mst_END (1)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss3_slv_START (2)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss3_slv_END (2)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss3_mst_START (3)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss3_mst_END (3)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss2_slv_START (4)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss2_slv_END (4)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss2_mst_START (5)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss2_mst_END (5)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss1_slv_START (6)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss1_slv_END (6)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss1_mst_START (7)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss1_mst_END (7)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss0_slv_START (8)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss0_slv_END (8)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss0_mst_START (9)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_npudmss0_mst_END (9)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu3_slv_START (10)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu3_slv_END (10)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu3_mst_START (11)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu3_mst_END (11)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu2_slv_START (12)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu2_slv_END (12)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu2_mst_START (13)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu2_mst_END (13)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu1_slv_START (14)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu1_slv_END (14)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu1_mst_START (15)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu1_mst_END (15)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu0_slv_START (16)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu0_slv_END (16)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu0_mst_START (17)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_gpu0_mst_END (17)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_cpu1_slv_START (18)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_cpu1_slv_END (18)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_cpu1_mst_START (19)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_cpu1_mst_END (19)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_cpu0_slv_START (20)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_cpu0_slv_END (20)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_cpu0_mst_START (21)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_cpu0_mst_END (21)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_mdm4g2ddr_slv_START (22)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_mdm4g2ddr_slv_END (22)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_mdm4g2ddr_mst_START (23)
#define SOC_CRGPERIPH_ADB_GT_STATUS_adb_gt_status_mdm4g2ddr_mst_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int adb_wakeup_fcm_acp_slv : 1;
        unsigned int adb_wakeup_fcm_acp_mst : 1;
        unsigned int adb_wakeup_npudmss3_slv : 1;
        unsigned int adb_wakeup_npudmss3_mst : 1;
        unsigned int adb_wakeup_npudmss2_slv : 1;
        unsigned int adb_wakeup_npudmss2_mst : 1;
        unsigned int adb_wakeup_npudmss1_slv : 1;
        unsigned int adb_wakeup_npudmss1_mst : 1;
        unsigned int adb_wakeup_npudmss0_slv : 1;
        unsigned int adb_wakeup_npudmss0_mst : 1;
        unsigned int adb_wakeup_gpu3_slv : 1;
        unsigned int adb_wakeup_gpu3_mst : 1;
        unsigned int adb_wakeup_gpu2_slv : 1;
        unsigned int adb_wakeup_gpu2_mst : 1;
        unsigned int adb_wakeup_gpu1_slv : 1;
        unsigned int adb_wakeup_gpu1_mst : 1;
        unsigned int adb_wakeup_gpu0_slv : 1;
        unsigned int adb_wakeup_gpu0_mst : 1;
        unsigned int adb_wakeup_cpu1_slv : 1;
        unsigned int adb_wakeup_cpu1_mst : 1;
        unsigned int adb_wakeup_cpu0_slv : 1;
        unsigned int adb_wakeup_cpu0_mst : 1;
        unsigned int adb_wakeup_mdm4g2ddr_slv : 1;
        unsigned int adb_wakeup_mdm4g2ddr_mst : 1;
        unsigned int reserved : 8;
    } reg;
} SOC_CRGPERIPH_ADB_WAKEUP_STATUS_UNION;
#endif
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_fcm_acp_slv_START (0)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_fcm_acp_slv_END (0)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_fcm_acp_mst_START (1)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_fcm_acp_mst_END (1)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss3_slv_START (2)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss3_slv_END (2)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss3_mst_START (3)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss3_mst_END (3)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss2_slv_START (4)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss2_slv_END (4)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss2_mst_START (5)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss2_mst_END (5)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss1_slv_START (6)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss1_slv_END (6)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss1_mst_START (7)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss1_mst_END (7)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss0_slv_START (8)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss0_slv_END (8)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss0_mst_START (9)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_npudmss0_mst_END (9)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu3_slv_START (10)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu3_slv_END (10)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu3_mst_START (11)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu3_mst_END (11)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu2_slv_START (12)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu2_slv_END (12)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu2_mst_START (13)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu2_mst_END (13)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu1_slv_START (14)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu1_slv_END (14)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu1_mst_START (15)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu1_mst_END (15)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu0_slv_START (16)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu0_slv_END (16)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu0_mst_START (17)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_gpu0_mst_END (17)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_cpu1_slv_START (18)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_cpu1_slv_END (18)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_cpu1_mst_START (19)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_cpu1_mst_END (19)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_cpu0_slv_START (20)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_cpu0_slv_END (20)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_cpu0_mst_START (21)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_cpu0_mst_END (21)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_mdm4g2ddr_slv_START (22)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_mdm4g2ddr_slv_END (22)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_mdm4g2ddr_mst_START (23)
#define SOC_CRGPERIPH_ADB_WAKEUP_STATUS_adb_wakeup_mdm4g2ddr_mst_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csysreq_adb_g3d_0 : 1;
        unsigned int reserved_0 : 1;
        unsigned int cactive_adb_g3d_0 : 1;
        unsigned int csysaccept_adb_g3d_0 : 1;
        unsigned int csysdeney_adb_g3d_0 : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_G3D_0_ADBLPSTAT_UNION;
#endif
#define SOC_CRGPERIPH_G3D_0_ADBLPSTAT_csysreq_adb_g3d_0_START (0)
#define SOC_CRGPERIPH_G3D_0_ADBLPSTAT_csysreq_adb_g3d_0_END (0)
#define SOC_CRGPERIPH_G3D_0_ADBLPSTAT_cactive_adb_g3d_0_START (2)
#define SOC_CRGPERIPH_G3D_0_ADBLPSTAT_cactive_adb_g3d_0_END (2)
#define SOC_CRGPERIPH_G3D_0_ADBLPSTAT_csysaccept_adb_g3d_0_START (3)
#define SOC_CRGPERIPH_G3D_0_ADBLPSTAT_csysaccept_adb_g3d_0_END (3)
#define SOC_CRGPERIPH_G3D_0_ADBLPSTAT_csysdeney_adb_g3d_0_START (4)
#define SOC_CRGPERIPH_G3D_0_ADBLPSTAT_csysdeney_adb_g3d_0_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csysreq_adb_g3d_1 : 1;
        unsigned int reserved_0 : 1;
        unsigned int cactive_adb_g3d_1 : 1;
        unsigned int csysaccept_adb_g3d_1 : 1;
        unsigned int csysdeney_adb_g3d_1 : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_G3D_1_ADBLPSTAT_UNION;
#endif
#define SOC_CRGPERIPH_G3D_1_ADBLPSTAT_csysreq_adb_g3d_1_START (0)
#define SOC_CRGPERIPH_G3D_1_ADBLPSTAT_csysreq_adb_g3d_1_END (0)
#define SOC_CRGPERIPH_G3D_1_ADBLPSTAT_cactive_adb_g3d_1_START (2)
#define SOC_CRGPERIPH_G3D_1_ADBLPSTAT_cactive_adb_g3d_1_END (2)
#define SOC_CRGPERIPH_G3D_1_ADBLPSTAT_csysaccept_adb_g3d_1_START (3)
#define SOC_CRGPERIPH_G3D_1_ADBLPSTAT_csysaccept_adb_g3d_1_END (3)
#define SOC_CRGPERIPH_G3D_1_ADBLPSTAT_csysdeney_adb_g3d_1_START (4)
#define SOC_CRGPERIPH_G3D_1_ADBLPSTAT_csysdeney_adb_g3d_1_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csysreq_adb_g3d_2 : 1;
        unsigned int reserved_0 : 1;
        unsigned int cactive_adb_g3d_2 : 1;
        unsigned int csysaccept_adb_g3d_2 : 1;
        unsigned int csysdeney_adb_g3d_2 : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_G3D_2_ADBLPSTAT_UNION;
#endif
#define SOC_CRGPERIPH_G3D_2_ADBLPSTAT_csysreq_adb_g3d_2_START (0)
#define SOC_CRGPERIPH_G3D_2_ADBLPSTAT_csysreq_adb_g3d_2_END (0)
#define SOC_CRGPERIPH_G3D_2_ADBLPSTAT_cactive_adb_g3d_2_START (2)
#define SOC_CRGPERIPH_G3D_2_ADBLPSTAT_cactive_adb_g3d_2_END (2)
#define SOC_CRGPERIPH_G3D_2_ADBLPSTAT_csysaccept_adb_g3d_2_START (3)
#define SOC_CRGPERIPH_G3D_2_ADBLPSTAT_csysaccept_adb_g3d_2_END (3)
#define SOC_CRGPERIPH_G3D_2_ADBLPSTAT_csysdeney_adb_g3d_2_START (4)
#define SOC_CRGPERIPH_G3D_2_ADBLPSTAT_csysdeney_adb_g3d_2_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csysreq_adb_g3d_3 : 1;
        unsigned int reserved_0 : 1;
        unsigned int cactive_adb_g3d_3 : 1;
        unsigned int csysaccept_adb_g3d_3 : 1;
        unsigned int csysdeney_adb_g3d_3 : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_G3D_3_ADBLPSTAT_UNION;
#endif
#define SOC_CRGPERIPH_G3D_3_ADBLPSTAT_csysreq_adb_g3d_3_START (0)
#define SOC_CRGPERIPH_G3D_3_ADBLPSTAT_csysreq_adb_g3d_3_END (0)
#define SOC_CRGPERIPH_G3D_3_ADBLPSTAT_cactive_adb_g3d_3_START (2)
#define SOC_CRGPERIPH_G3D_3_ADBLPSTAT_cactive_adb_g3d_3_END (2)
#define SOC_CRGPERIPH_G3D_3_ADBLPSTAT_csysaccept_adb_g3d_3_START (3)
#define SOC_CRGPERIPH_G3D_3_ADBLPSTAT_csysaccept_adb_g3d_3_END (3)
#define SOC_CRGPERIPH_G3D_3_ADBLPSTAT_csysdeney_adb_g3d_3_START (4)
#define SOC_CRGPERIPH_G3D_3_ADBLPSTAT_csysdeney_adb_g3d_3_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csysreq_adb_npu_0 : 1;
        unsigned int reserved_0 : 1;
        unsigned int cactive_adb_npu_0 : 1;
        unsigned int csysaccept_adb_npu_0 : 1;
        unsigned int csysdeney_adb_npu_0 : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_NPU_0_ADBLPSTAT_UNION;
#endif
#define SOC_CRGPERIPH_NPU_0_ADBLPSTAT_csysreq_adb_npu_0_START (0)
#define SOC_CRGPERIPH_NPU_0_ADBLPSTAT_csysreq_adb_npu_0_END (0)
#define SOC_CRGPERIPH_NPU_0_ADBLPSTAT_cactive_adb_npu_0_START (2)
#define SOC_CRGPERIPH_NPU_0_ADBLPSTAT_cactive_adb_npu_0_END (2)
#define SOC_CRGPERIPH_NPU_0_ADBLPSTAT_csysaccept_adb_npu_0_START (3)
#define SOC_CRGPERIPH_NPU_0_ADBLPSTAT_csysaccept_adb_npu_0_END (3)
#define SOC_CRGPERIPH_NPU_0_ADBLPSTAT_csysdeney_adb_npu_0_START (4)
#define SOC_CRGPERIPH_NPU_0_ADBLPSTAT_csysdeney_adb_npu_0_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csysreq_adb_npu_1 : 1;
        unsigned int reserved_0 : 1;
        unsigned int cactive_adb_npu_1 : 1;
        unsigned int csysaccept_adb_npu_1 : 1;
        unsigned int csysdeney_adb_npu_1 : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_NPU_1_ADBLPSTAT_UNION;
#endif
#define SOC_CRGPERIPH_NPU_1_ADBLPSTAT_csysreq_adb_npu_1_START (0)
#define SOC_CRGPERIPH_NPU_1_ADBLPSTAT_csysreq_adb_npu_1_END (0)
#define SOC_CRGPERIPH_NPU_1_ADBLPSTAT_cactive_adb_npu_1_START (2)
#define SOC_CRGPERIPH_NPU_1_ADBLPSTAT_cactive_adb_npu_1_END (2)
#define SOC_CRGPERIPH_NPU_1_ADBLPSTAT_csysaccept_adb_npu_1_START (3)
#define SOC_CRGPERIPH_NPU_1_ADBLPSTAT_csysaccept_adb_npu_1_END (3)
#define SOC_CRGPERIPH_NPU_1_ADBLPSTAT_csysdeney_adb_npu_1_START (4)
#define SOC_CRGPERIPH_NPU_1_ADBLPSTAT_csysdeney_adb_npu_1_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int adb_qhanske_enable_fcm_acp_slv : 1;
        unsigned int adb_qhanske_enable_fcm_acp_mst : 1;
        unsigned int adb_qhanske_enable_npudmss3_slv : 1;
        unsigned int adb_qhanske_enable_npudmss3_mst : 1;
        unsigned int adb_qhanske_enable_npudmss2_slv : 1;
        unsigned int adb_qhanske_enable_npudmss2_mst : 1;
        unsigned int adb_qhanske_enable_npudmss1_slv : 1;
        unsigned int adb_qhanske_enable_npudmss1_mst : 1;
        unsigned int adb_qhanske_enable_npudmss0_slv : 1;
        unsigned int adb_qhanske_enable_npudmss0_mst : 1;
        unsigned int adb_qhanske_enable_gpu3_slv : 1;
        unsigned int adb_qhanske_enable_gpu3_mst : 1;
        unsigned int adb_qhanske_enable_gpu2_slv : 1;
        unsigned int adb_qhanske_enable_gpu2_mst : 1;
        unsigned int adb_qhanske_enable_gpu1_slv : 1;
        unsigned int adb_qhanske_enable_gpu1_mst : 1;
        unsigned int adb_qhanske_enable_gpu0_slv : 1;
        unsigned int adb_qhanske_enable_gpu0_mst : 1;
        unsigned int adb_qhanske_enable_cpu1_slv : 1;
        unsigned int adb_qhanske_enable_cpu1_mst : 1;
        unsigned int adb_qhanske_enable_cpu0_slv : 1;
        unsigned int adb_qhanske_enable_cpu0_mst : 1;
        unsigned int adb_qhanske_enable_mdm4g2ddr_slv : 1;
        unsigned int adb_qhanske_enable_mdm4g2ddr_mst : 1;
        unsigned int reserved : 8;
    } reg;
} SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_UNION;
#endif
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_fcm_acp_slv_START (0)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_fcm_acp_slv_END (0)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_fcm_acp_mst_START (1)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_fcm_acp_mst_END (1)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss3_slv_START (2)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss3_slv_END (2)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss3_mst_START (3)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss3_mst_END (3)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss2_slv_START (4)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss2_slv_END (4)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss2_mst_START (5)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss2_mst_END (5)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss1_slv_START (6)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss1_slv_END (6)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss1_mst_START (7)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss1_mst_END (7)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss0_slv_START (8)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss0_slv_END (8)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss0_mst_START (9)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_npudmss0_mst_END (9)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu3_slv_START (10)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu3_slv_END (10)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu3_mst_START (11)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu3_mst_END (11)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu2_slv_START (12)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu2_slv_END (12)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu2_mst_START (13)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu2_mst_END (13)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu1_slv_START (14)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu1_slv_END (14)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu1_mst_START (15)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu1_mst_END (15)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu0_slv_START (16)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu0_slv_END (16)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu0_mst_START (17)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_gpu0_mst_END (17)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_cpu1_slv_START (18)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_cpu1_slv_END (18)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_cpu1_mst_START (19)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_cpu1_mst_END (19)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_cpu0_slv_START (20)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_cpu0_slv_END (20)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_cpu0_mst_START (21)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_cpu0_mst_END (21)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_mdm4g2ddr_slv_START (22)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_mdm4g2ddr_slv_END (22)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_mdm4g2ddr_mst_START (23)
#define SOC_CRGPERIPH_ADB_QHANSKE_ENABLE_STATUS_adb_qhanske_enable_mdm4g2ddr_mst_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fcm_noc_clkrst_bypass : 1;
        unsigned int pctrl_clkrst_bypass : 1;
        unsigned int pwm_clkrst_bypass : 1;
        unsigned int reserved : 1;
        unsigned int wd0_clkrst_bypass : 1;
        unsigned int wd1_clkrst_bypass : 1;
        unsigned int gpio0_clkrst_bypass : 1;
        unsigned int gpio1_clkrst_bypass : 1;
        unsigned int gpio2_clkrst_bypass : 1;
        unsigned int gpio3_clkrst_bypass : 1;
        unsigned int gpio4_clkrst_bypass : 1;
        unsigned int gpio5_clkrst_bypass : 1;
        unsigned int gpio6_clkrst_bypass : 1;
        unsigned int gpio7_clkrst_bypass : 1;
        unsigned int gpio8_clkrst_bypass : 1;
        unsigned int gpio9_clkrst_bypass : 1;
        unsigned int gpio10_clkrst_bypass : 1;
        unsigned int gpio11_clkrst_bypass : 1;
        unsigned int gpio12_clkrst_bypass : 1;
        unsigned int gpio13_clkrst_bypass : 1;
        unsigned int gpio14_clkrst_bypass : 1;
        unsigned int gpio15_clkrst_bypass : 1;
        unsigned int gpio16_clkrst_bypass : 1;
        unsigned int gpio17_clkrst_bypass : 1;
        unsigned int gpio18_clkrst_bypass : 1;
        unsigned int gpio19_clkrst_bypass : 1;
        unsigned int gpio20_clkrst_bypass : 1;
        unsigned int gpio21_clkrst_bypass : 1;
        unsigned int timer9_clkrst_bypass : 1;
        unsigned int timer10_clkrst_bypass : 1;
        unsigned int timer11_clkrst_bypass : 1;
        unsigned int timer12_clkrst_bypass : 1;
    } reg;
} SOC_CRGPERIPH_IPCLKRST_BYPASS0_UNION;
#endif
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_fcm_noc_clkrst_bypass_START (0)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_fcm_noc_clkrst_bypass_END (0)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_pctrl_clkrst_bypass_START (1)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_pctrl_clkrst_bypass_END (1)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_pwm_clkrst_bypass_START (2)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_pwm_clkrst_bypass_END (2)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_wd0_clkrst_bypass_START (4)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_wd0_clkrst_bypass_END (4)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_wd1_clkrst_bypass_START (5)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_wd1_clkrst_bypass_END (5)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio0_clkrst_bypass_START (6)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio0_clkrst_bypass_END (6)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio1_clkrst_bypass_START (7)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio1_clkrst_bypass_END (7)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio2_clkrst_bypass_START (8)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio2_clkrst_bypass_END (8)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio3_clkrst_bypass_START (9)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio3_clkrst_bypass_END (9)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio4_clkrst_bypass_START (10)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio4_clkrst_bypass_END (10)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio5_clkrst_bypass_START (11)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio5_clkrst_bypass_END (11)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio6_clkrst_bypass_START (12)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio6_clkrst_bypass_END (12)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio7_clkrst_bypass_START (13)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio7_clkrst_bypass_END (13)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio8_clkrst_bypass_START (14)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio8_clkrst_bypass_END (14)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio9_clkrst_bypass_START (15)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio9_clkrst_bypass_END (15)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio10_clkrst_bypass_START (16)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio10_clkrst_bypass_END (16)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio11_clkrst_bypass_START (17)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio11_clkrst_bypass_END (17)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio12_clkrst_bypass_START (18)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio12_clkrst_bypass_END (18)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio13_clkrst_bypass_START (19)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio13_clkrst_bypass_END (19)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio14_clkrst_bypass_START (20)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio14_clkrst_bypass_END (20)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio15_clkrst_bypass_START (21)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio15_clkrst_bypass_END (21)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio16_clkrst_bypass_START (22)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio16_clkrst_bypass_END (22)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio17_clkrst_bypass_START (23)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio17_clkrst_bypass_END (23)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio18_clkrst_bypass_START (24)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio18_clkrst_bypass_END (24)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio19_clkrst_bypass_START (25)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio19_clkrst_bypass_END (25)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio20_clkrst_bypass_START (26)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio20_clkrst_bypass_END (26)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio21_clkrst_bypass_START (27)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_gpio21_clkrst_bypass_END (27)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_timer9_clkrst_bypass_START (28)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_timer9_clkrst_bypass_END (28)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_timer10_clkrst_bypass_START (29)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_timer10_clkrst_bypass_END (29)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_timer11_clkrst_bypass_START (30)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_timer11_clkrst_bypass_END (30)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_timer12_clkrst_bypass_START (31)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS0_timer12_clkrst_bypass_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tzpc_clkrst_bypass : 1;
        unsigned int ipc0_clkrst_bypass : 1;
        unsigned int ipc1_clkrst_bypass : 1;
        unsigned int ioc_clkrst_bypass : 1;
        unsigned int codecssi_clkrst_bypass : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int ipc_mdm_clkrst_bypass : 1;
        unsigned int uart0_clkrst_bypass : 1;
        unsigned int uart1_clkrst_bypass : 1;
        unsigned int uart2_clkrst_bypass : 1;
        unsigned int reserved_2 : 1;
        unsigned int uart4_clkrst_bypass : 1;
        unsigned int uart5_clkrst_bypass : 1;
        unsigned int reserved_3 : 1;
        unsigned int spi1_clkrst_bypass : 1;
        unsigned int spi4_clkrst_bypass : 1;
        unsigned int i2c2_clkrst_bypass : 1;
        unsigned int i2c6_clkrst_bypass : 1;
        unsigned int i2c3_clkrst_bypass : 1;
        unsigned int i2c4_clkrst_bypass : 1;
        unsigned int dmac_clkrst_bypass : 1;
        unsigned int ipc_fast0_clkrst_bypass : 1;
        unsigned int ipc_fast1_clkrst_bypass : 1;
        unsigned int peri_pcr_clkrst_bypass : 1;
        unsigned int i3c4_clkrst_bypass : 1;
        unsigned int socp_clkrst_bypass : 1;
        unsigned int hsdt1_crg_clkrst_bypass : 1;
        unsigned int media_crg_clkrst_bypass : 1;
        unsigned int media2_crg_clkrst_bypass : 1;
        unsigned int latmon_clkrst_bypass : 1;
        unsigned int reserved_4 : 1;
    } reg;
} SOC_CRGPERIPH_IPCLKRST_BYPASS1_UNION;
#endif
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_tzpc_clkrst_bypass_START (0)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_tzpc_clkrst_bypass_END (0)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_ipc0_clkrst_bypass_START (1)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_ipc0_clkrst_bypass_END (1)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_ipc1_clkrst_bypass_START (2)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_ipc1_clkrst_bypass_END (2)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_ioc_clkrst_bypass_START (3)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_ioc_clkrst_bypass_END (3)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_codecssi_clkrst_bypass_START (4)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_codecssi_clkrst_bypass_END (4)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_ipc_mdm_clkrst_bypass_START (7)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_ipc_mdm_clkrst_bypass_END (7)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_uart0_clkrst_bypass_START (8)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_uart0_clkrst_bypass_END (8)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_uart1_clkrst_bypass_START (9)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_uart1_clkrst_bypass_END (9)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_uart2_clkrst_bypass_START (10)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_uart2_clkrst_bypass_END (10)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_uart4_clkrst_bypass_START (12)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_uart4_clkrst_bypass_END (12)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_uart5_clkrst_bypass_START (13)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_uart5_clkrst_bypass_END (13)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_spi1_clkrst_bypass_START (15)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_spi1_clkrst_bypass_END (15)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_spi4_clkrst_bypass_START (16)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_spi4_clkrst_bypass_END (16)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_i2c2_clkrst_bypass_START (17)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_i2c2_clkrst_bypass_END (17)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_i2c6_clkrst_bypass_START (18)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_i2c6_clkrst_bypass_END (18)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_i2c3_clkrst_bypass_START (19)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_i2c3_clkrst_bypass_END (19)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_i2c4_clkrst_bypass_START (20)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_i2c4_clkrst_bypass_END (20)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_dmac_clkrst_bypass_START (21)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_dmac_clkrst_bypass_END (21)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_ipc_fast0_clkrst_bypass_START (22)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_ipc_fast0_clkrst_bypass_END (22)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_ipc_fast1_clkrst_bypass_START (23)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_ipc_fast1_clkrst_bypass_END (23)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_peri_pcr_clkrst_bypass_START (24)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_peri_pcr_clkrst_bypass_END (24)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_i3c4_clkrst_bypass_START (25)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_i3c4_clkrst_bypass_END (25)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_socp_clkrst_bypass_START (26)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_socp_clkrst_bypass_END (26)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_hsdt1_crg_clkrst_bypass_START (27)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_hsdt1_crg_clkrst_bypass_END (27)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_media_crg_clkrst_bypass_START (28)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_media_crg_clkrst_bypass_END (28)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_media2_crg_clkrst_bypass_START (29)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_media2_crg_clkrst_bypass_END (29)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_latmon_clkrst_bypass_START (30)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS1_latmon_clkrst_bypass_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_crg_clkrst_bypass : 1;
        unsigned int hsdt_crg_clkrst_bypass : 1;
        unsigned int core_crg_clkrst_bypass : 1;
        unsigned int reserved_0 : 1;
        unsigned int cfg2gpu_apb_clkrst_bypass : 1;
        unsigned int pimon_clkrst_bypass : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int perf_clkrst_bypass : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int ipf_mdm_clkrst_bypass : 1;
        unsigned int loadmonitor_clkrst_bypass : 1;
        unsigned int fcm_clkrst_bypass : 1;
        unsigned int i2c7_clkrst_bypass : 1;
        unsigned int loadmonitor_1_clkrst_bypass : 1;
        unsigned int loadmonitor_2_clkrst_bypass : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int atgc_clkrst_bypass : 1;
        unsigned int atgc1_clkrst_bypass : 1;
        unsigned int spe_clkrst_bypass : 1;
        unsigned int bba_clkrst_bypass : 1;
        unsigned int cfg2media2_apb_clkrst_bypass : 1;
        unsigned int cfg2media1_apb_clkrst_bypass : 1;
        unsigned int dmss_clkrst_bypass : 1;
        unsigned int dmc_a_clkrst_bypass : 1;
        unsigned int dmc_b_clkrst_bypass : 1;
        unsigned int dmc_c_clkrst_bypass : 1;
        unsigned int dmc_d_clkrst_bypass : 1;
    } reg;
} SOC_CRGPERIPH_IPCLKRST_BYPASS2_UNION;
#endif
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_npu_crg_clkrst_bypass_START (0)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_npu_crg_clkrst_bypass_END (0)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_hsdt_crg_clkrst_bypass_START (1)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_hsdt_crg_clkrst_bypass_END (1)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_core_crg_clkrst_bypass_START (2)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_core_crg_clkrst_bypass_END (2)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_cfg2gpu_apb_clkrst_bypass_START (4)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_cfg2gpu_apb_clkrst_bypass_END (4)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_pimon_clkrst_bypass_START (5)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_pimon_clkrst_bypass_END (5)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_perf_clkrst_bypass_START (9)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_perf_clkrst_bypass_END (9)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_ipf_mdm_clkrst_bypass_START (13)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_ipf_mdm_clkrst_bypass_END (13)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_loadmonitor_clkrst_bypass_START (14)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_loadmonitor_clkrst_bypass_END (14)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_fcm_clkrst_bypass_START (15)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_fcm_clkrst_bypass_END (15)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_i2c7_clkrst_bypass_START (16)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_i2c7_clkrst_bypass_END (16)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_loadmonitor_1_clkrst_bypass_START (17)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_loadmonitor_1_clkrst_bypass_END (17)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_loadmonitor_2_clkrst_bypass_START (18)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_loadmonitor_2_clkrst_bypass_END (18)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_atgc_clkrst_bypass_START (21)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_atgc_clkrst_bypass_END (21)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_atgc1_clkrst_bypass_START (22)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_atgc1_clkrst_bypass_END (22)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_spe_clkrst_bypass_START (23)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_spe_clkrst_bypass_END (23)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_bba_clkrst_bypass_START (24)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_bba_clkrst_bypass_END (24)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_cfg2media2_apb_clkrst_bypass_START (25)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_cfg2media2_apb_clkrst_bypass_END (25)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_cfg2media1_apb_clkrst_bypass_START (26)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_cfg2media1_apb_clkrst_bypass_END (26)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_dmss_clkrst_bypass_START (27)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_dmss_clkrst_bypass_END (27)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_dmc_a_clkrst_bypass_START (28)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_dmc_a_clkrst_bypass_END (28)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_dmc_b_clkrst_bypass_START (29)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_dmc_b_clkrst_bypass_END (29)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_dmc_c_clkrst_bypass_START (30)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_dmc_c_clkrst_bypass_END (30)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_dmc_d_clkrst_bypass_START (31)
#define SOC_CRGPERIPH_IPCLKRST_BYPASS2_dmc_d_clkrst_bypass_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csysreq_adb_npu_2 : 1;
        unsigned int reserved_0 : 1;
        unsigned int cactive_adb_npu_2 : 1;
        unsigned int csysaccept_adb_npu_2 : 1;
        unsigned int csysdeney_adb_npu_2 : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_NPU_2_ADBLPSTAT_UNION;
#endif
#define SOC_CRGPERIPH_NPU_2_ADBLPSTAT_csysreq_adb_npu_2_START (0)
#define SOC_CRGPERIPH_NPU_2_ADBLPSTAT_csysreq_adb_npu_2_END (0)
#define SOC_CRGPERIPH_NPU_2_ADBLPSTAT_cactive_adb_npu_2_START (2)
#define SOC_CRGPERIPH_NPU_2_ADBLPSTAT_cactive_adb_npu_2_END (2)
#define SOC_CRGPERIPH_NPU_2_ADBLPSTAT_csysaccept_adb_npu_2_START (3)
#define SOC_CRGPERIPH_NPU_2_ADBLPSTAT_csysaccept_adb_npu_2_END (3)
#define SOC_CRGPERIPH_NPU_2_ADBLPSTAT_csysdeney_adb_npu_2_START (4)
#define SOC_CRGPERIPH_NPU_2_ADBLPSTAT_csysdeney_adb_npu_2_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csysreq_adb_npu_3 : 1;
        unsigned int reserved_0 : 1;
        unsigned int cactive_adb_npu_3 : 1;
        unsigned int csysaccept_adb_npu_3 : 1;
        unsigned int csysdeney_adb_npu_3 : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_NPU_3_ADBLPSTAT_UNION;
#endif
#define SOC_CRGPERIPH_NPU_3_ADBLPSTAT_csysreq_adb_npu_3_START (0)
#define SOC_CRGPERIPH_NPU_3_ADBLPSTAT_csysreq_adb_npu_3_END (0)
#define SOC_CRGPERIPH_NPU_3_ADBLPSTAT_cactive_adb_npu_3_START (2)
#define SOC_CRGPERIPH_NPU_3_ADBLPSTAT_cactive_adb_npu_3_END (2)
#define SOC_CRGPERIPH_NPU_3_ADBLPSTAT_csysaccept_adb_npu_3_START (3)
#define SOC_CRGPERIPH_NPU_3_ADBLPSTAT_csysaccept_adb_npu_3_END (3)
#define SOC_CRGPERIPH_NPU_3_ADBLPSTAT_csysdeney_adb_npu_3_START (4)
#define SOC_CRGPERIPH_NPU_3_ADBLPSTAT_csysdeney_adb_npu_3_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_little_pdc_en : 1;
        unsigned int fcm_middle_pdc_en : 1;
        unsigned int fcm_big_pdc_en : 1;
        unsigned int reserved : 13;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_FCM_PDCEN_UNION;
#endif
#define SOC_CRGPERIPH_FCM_PDCEN_cpu_little_pdc_en_START (0)
#define SOC_CRGPERIPH_FCM_PDCEN_cpu_little_pdc_en_END (0)
#define SOC_CRGPERIPH_FCM_PDCEN_fcm_middle_pdc_en_START (1)
#define SOC_CRGPERIPH_FCM_PDCEN_fcm_middle_pdc_en_END (1)
#define SOC_CRGPERIPH_FCM_PDCEN_fcm_big_pdc_en_START (2)
#define SOC_CRGPERIPH_FCM_PDCEN_fcm_big_pdc_en_END (2)
#define SOC_CRGPERIPH_FCM_PDCEN_bitmasken_START (16)
#define SOC_CRGPERIPH_FCM_PDCEN_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_cpu_little_core0_pwr_en : 1;
        unsigned int intr_cpu_little_core1_pwr_en : 1;
        unsigned int intr_cpu_little_core2_pwr_en : 1;
        unsigned int intr_cpu_little_core3_pwr_en : 1;
        unsigned int reserved : 12;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_A53_COREPWRINTEN_UNION;
#endif
#define SOC_CRGPERIPH_A53_COREPWRINTEN_intr_cpu_little_core0_pwr_en_START (0)
#define SOC_CRGPERIPH_A53_COREPWRINTEN_intr_cpu_little_core0_pwr_en_END (0)
#define SOC_CRGPERIPH_A53_COREPWRINTEN_intr_cpu_little_core1_pwr_en_START (1)
#define SOC_CRGPERIPH_A53_COREPWRINTEN_intr_cpu_little_core1_pwr_en_END (1)
#define SOC_CRGPERIPH_A53_COREPWRINTEN_intr_cpu_little_core2_pwr_en_START (2)
#define SOC_CRGPERIPH_A53_COREPWRINTEN_intr_cpu_little_core2_pwr_en_END (2)
#define SOC_CRGPERIPH_A53_COREPWRINTEN_intr_cpu_little_core3_pwr_en_START (3)
#define SOC_CRGPERIPH_A53_COREPWRINTEN_intr_cpu_little_core3_pwr_en_END (3)
#define SOC_CRGPERIPH_A53_COREPWRINTEN_bitmasken_START (16)
#define SOC_CRGPERIPH_A53_COREPWRINTEN_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_cpu_little_core0_pwr_stat : 1;
        unsigned int intr_cpu_little_core1_pwr_stat : 1;
        unsigned int intr_cpu_little_core2_pwr_stat : 1;
        unsigned int intr_cpu_little_core3_pwr_stat : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_A53_COREPWRINTSTAT_UNION;
#endif
#define SOC_CRGPERIPH_A53_COREPWRINTSTAT_intr_cpu_little_core0_pwr_stat_START (0)
#define SOC_CRGPERIPH_A53_COREPWRINTSTAT_intr_cpu_little_core0_pwr_stat_END (0)
#define SOC_CRGPERIPH_A53_COREPWRINTSTAT_intr_cpu_little_core1_pwr_stat_START (1)
#define SOC_CRGPERIPH_A53_COREPWRINTSTAT_intr_cpu_little_core1_pwr_stat_END (1)
#define SOC_CRGPERIPH_A53_COREPWRINTSTAT_intr_cpu_little_core2_pwr_stat_START (2)
#define SOC_CRGPERIPH_A53_COREPWRINTSTAT_intr_cpu_little_core2_pwr_stat_END (2)
#define SOC_CRGPERIPH_A53_COREPWRINTSTAT_intr_cpu_little_core3_pwr_stat_START (3)
#define SOC_CRGPERIPH_A53_COREPWRINTSTAT_intr_cpu_little_core3_pwr_stat_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_cpu_little_core0_gic_mask : 1;
        unsigned int intr_cpu_little_core1_gic_mask : 1;
        unsigned int intr_cpu_little_core2_gic_mask : 1;
        unsigned int intr_cpu_little_core3_gic_mask : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_A53_COREGICMASK_UNION;
#endif
#define SOC_CRGPERIPH_A53_COREGICMASK_intr_cpu_little_core0_gic_mask_START (0)
#define SOC_CRGPERIPH_A53_COREGICMASK_intr_cpu_little_core0_gic_mask_END (0)
#define SOC_CRGPERIPH_A53_COREGICMASK_intr_cpu_little_core1_gic_mask_START (1)
#define SOC_CRGPERIPH_A53_COREGICMASK_intr_cpu_little_core1_gic_mask_END (1)
#define SOC_CRGPERIPH_A53_COREGICMASK_intr_cpu_little_core2_gic_mask_START (2)
#define SOC_CRGPERIPH_A53_COREGICMASK_intr_cpu_little_core2_gic_mask_END (2)
#define SOC_CRGPERIPH_A53_COREGICMASK_intr_cpu_little_core3_gic_mask_START (3)
#define SOC_CRGPERIPH_A53_COREGICMASK_intr_cpu_little_core3_gic_mask_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_little_core0_pwrup_req : 1;
        unsigned int cpu_little_core1_pwrup_req : 1;
        unsigned int cpu_little_core2_pwrup_req : 1;
        unsigned int cpu_little_core3_pwrup_req : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_A53_COREPOWERUP_UNION;
#endif
#define SOC_CRGPERIPH_A53_COREPOWERUP_cpu_little_core0_pwrup_req_START (0)
#define SOC_CRGPERIPH_A53_COREPOWERUP_cpu_little_core0_pwrup_req_END (0)
#define SOC_CRGPERIPH_A53_COREPOWERUP_cpu_little_core1_pwrup_req_START (1)
#define SOC_CRGPERIPH_A53_COREPOWERUP_cpu_little_core1_pwrup_req_END (1)
#define SOC_CRGPERIPH_A53_COREPOWERUP_cpu_little_core2_pwrup_req_START (2)
#define SOC_CRGPERIPH_A53_COREPOWERUP_cpu_little_core2_pwrup_req_END (2)
#define SOC_CRGPERIPH_A53_COREPOWERUP_cpu_little_core3_pwrup_req_START (3)
#define SOC_CRGPERIPH_A53_COREPOWERUP_cpu_little_core3_pwrup_req_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_little_core0_pwrdn_req : 1;
        unsigned int cpu_little_core1_pwrdn_req : 1;
        unsigned int cpu_little_core2_pwrdn_req : 1;
        unsigned int cpu_little_core3_pwrdn_req : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_A53_COREPOWERDN_UNION;
#endif
#define SOC_CRGPERIPH_A53_COREPOWERDN_cpu_little_core0_pwrdn_req_START (0)
#define SOC_CRGPERIPH_A53_COREPOWERDN_cpu_little_core0_pwrdn_req_END (0)
#define SOC_CRGPERIPH_A53_COREPOWERDN_cpu_little_core1_pwrdn_req_START (1)
#define SOC_CRGPERIPH_A53_COREPOWERDN_cpu_little_core1_pwrdn_req_END (1)
#define SOC_CRGPERIPH_A53_COREPOWERDN_cpu_little_core2_pwrdn_req_START (2)
#define SOC_CRGPERIPH_A53_COREPOWERDN_cpu_little_core2_pwrdn_req_END (2)
#define SOC_CRGPERIPH_A53_COREPOWERDN_cpu_little_core3_pwrdn_req_START (3)
#define SOC_CRGPERIPH_A53_COREPOWERDN_cpu_little_core3_pwrdn_req_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_little_core0_pwr_stat : 4;
        unsigned int cpu_little_core1_pwr_stat : 4;
        unsigned int cpu_little_core2_pwr_stat : 4;
        unsigned int cpu_little_core3_pwr_stat : 4;
        unsigned int reserved : 16;
    } reg;
} SOC_CRGPERIPH_A53_COREPOWERSTAT_UNION;
#endif
#define SOC_CRGPERIPH_A53_COREPOWERSTAT_cpu_little_core0_pwr_stat_START (0)
#define SOC_CRGPERIPH_A53_COREPOWERSTAT_cpu_little_core0_pwr_stat_END (3)
#define SOC_CRGPERIPH_A53_COREPOWERSTAT_cpu_little_core1_pwr_stat_START (4)
#define SOC_CRGPERIPH_A53_COREPOWERSTAT_cpu_little_core1_pwr_stat_END (7)
#define SOC_CRGPERIPH_A53_COREPOWERSTAT_cpu_little_core2_pwr_stat_START (8)
#define SOC_CRGPERIPH_A53_COREPOWERSTAT_cpu_little_core2_pwr_stat_END (11)
#define SOC_CRGPERIPH_A53_COREPOWERSTAT_cpu_little_core3_pwr_stat_START (12)
#define SOC_CRGPERIPH_A53_COREPOWERSTAT_cpu_little_core3_pwr_stat_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_little_core_pwrup_time : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_CRGPERIPH_A53_COREPWRUPTIME_UNION;
#endif
#define SOC_CRGPERIPH_A53_COREPWRUPTIME_cpu_little_core_pwrup_time_START (0)
#define SOC_CRGPERIPH_A53_COREPWRUPTIME_cpu_little_core_pwrup_time_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_little_core_pwrdn_time : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_CRGPERIPH_A53_COREPWRDNTIME_UNION;
#endif
#define SOC_CRGPERIPH_A53_COREPWRDNTIME_cpu_little_core_pwrdn_time_START (0)
#define SOC_CRGPERIPH_A53_COREPWRDNTIME_cpu_little_core_pwrdn_time_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_little_core_iso_time : 5;
        unsigned int reserved : 27;
    } reg;
} SOC_CRGPERIPH_A53_COREISOTIME_UNION;
#endif
#define SOC_CRGPERIPH_A53_COREISOTIME_cpu_little_core_iso_time_START (0)
#define SOC_CRGPERIPH_A53_COREISOTIME_cpu_little_core_iso_time_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_little_core_dbg_time : 4;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_A53_COREDBGTIME_UNION;
#endif
#define SOC_CRGPERIPH_A53_COREDBGTIME_cpu_little_core_dbg_time_START (0)
#define SOC_CRGPERIPH_A53_COREDBGTIME_cpu_little_core_dbg_time_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_little_core_rst_time : 3;
        unsigned int reserved : 29;
    } reg;
} SOC_CRGPERIPH_A53_CORERSTTIME_UNION;
#endif
#define SOC_CRGPERIPH_A53_CORERSTTIME_cpu_little_core_rst_time_START (0)
#define SOC_CRGPERIPH_A53_CORERSTTIME_cpu_little_core_rst_time_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_little_core_urst_time : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_CRGPERIPH_A53_COREURSTTIME_UNION;
#endif
#define SOC_CRGPERIPH_A53_COREURSTTIME_cpu_little_core_urst_time_START (0)
#define SOC_CRGPERIPH_A53_COREURSTTIME_cpu_little_core_urst_time_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_little_core_ocldo_dis_time : 16;
        unsigned int cpu_little_core_ocldo_en_time : 16;
    } reg;
} SOC_CRGPERIPH_A53_COREOCLDOTIME0_UNION;
#endif
#define SOC_CRGPERIPH_A53_COREOCLDOTIME0_cpu_little_core_ocldo_dis_time_START (0)
#define SOC_CRGPERIPH_A53_COREOCLDOTIME0_cpu_little_core_ocldo_dis_time_END (15)
#define SOC_CRGPERIPH_A53_COREOCLDOTIME0_cpu_little_core_ocldo_en_time_START (16)
#define SOC_CRGPERIPH_A53_COREOCLDOTIME0_cpu_little_core_ocldo_en_time_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_little_core_ocldo_pwrdn_time : 16;
        unsigned int cpu_little_core_ocldo_pwrup_time : 16;
    } reg;
} SOC_CRGPERIPH_A53_COREOCLDOTIME1_UNION;
#endif
#define SOC_CRGPERIPH_A53_COREOCLDOTIME1_cpu_little_core_ocldo_pwrdn_time_START (0)
#define SOC_CRGPERIPH_A53_COREOCLDOTIME1_cpu_little_core_ocldo_pwrdn_time_END (15)
#define SOC_CRGPERIPH_A53_COREOCLDOTIME1_cpu_little_core_ocldo_pwrup_time_START (16)
#define SOC_CRGPERIPH_A53_COREOCLDOTIME1_cpu_little_core_ocldo_pwrup_time_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a53_0_core0_ocdlo_stat : 3;
        unsigned int a53_0_core1_ocdlo_stat : 3;
        unsigned int a53_0_core2_ocdlo_stat : 3;
        unsigned int a53_0_core3_ocdlo_stat : 3;
        unsigned int reserved : 20;
    } reg;
} SOC_CRGPERIPH_A53_COREOCLDOSTAT_UNION;
#endif
#define SOC_CRGPERIPH_A53_COREOCLDOSTAT_a53_0_core0_ocdlo_stat_START (0)
#define SOC_CRGPERIPH_A53_COREOCLDOSTAT_a53_0_core0_ocdlo_stat_END (2)
#define SOC_CRGPERIPH_A53_COREOCLDOSTAT_a53_0_core1_ocdlo_stat_START (3)
#define SOC_CRGPERIPH_A53_COREOCLDOSTAT_a53_0_core1_ocdlo_stat_END (5)
#define SOC_CRGPERIPH_A53_COREOCLDOSTAT_a53_0_core2_ocdlo_stat_START (6)
#define SOC_CRGPERIPH_A53_COREOCLDOSTAT_a53_0_core2_ocdlo_stat_END (8)
#define SOC_CRGPERIPH_A53_COREOCLDOSTAT_a53_0_core3_ocdlo_stat_START (9)
#define SOC_CRGPERIPH_A53_COREOCLDOSTAT_a53_0_core3_ocdlo_stat_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_a53_1_core0_pwr_en : 1;
        unsigned int intr_a53_1_core1_pwr_en : 1;
        unsigned int intr_a53_1_core2_pwr_en : 1;
        unsigned int intr_a53_1_core3_pwr_en : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 11;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_MAIA_COREPWRINTEN_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_COREPWRINTEN_intr_a53_1_core0_pwr_en_START (0)
#define SOC_CRGPERIPH_MAIA_COREPWRINTEN_intr_a53_1_core0_pwr_en_END (0)
#define SOC_CRGPERIPH_MAIA_COREPWRINTEN_intr_a53_1_core1_pwr_en_START (1)
#define SOC_CRGPERIPH_MAIA_COREPWRINTEN_intr_a53_1_core1_pwr_en_END (1)
#define SOC_CRGPERIPH_MAIA_COREPWRINTEN_intr_a53_1_core2_pwr_en_START (2)
#define SOC_CRGPERIPH_MAIA_COREPWRINTEN_intr_a53_1_core2_pwr_en_END (2)
#define SOC_CRGPERIPH_MAIA_COREPWRINTEN_intr_a53_1_core3_pwr_en_START (3)
#define SOC_CRGPERIPH_MAIA_COREPWRINTEN_intr_a53_1_core3_pwr_en_END (3)
#define SOC_CRGPERIPH_MAIA_COREPWRINTEN_bitmasken_START (16)
#define SOC_CRGPERIPH_MAIA_COREPWRINTEN_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_a53_1_core0_pwr_stat : 1;
        unsigned int intr_a53_1_core1_pwr_stat : 1;
        unsigned int intr_a53_1_core2_pwr_stat : 1;
        unsigned int intr_a53_1_core3_pwr_stat : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_MAIA_COREPWRINTSTAT_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_COREPWRINTSTAT_intr_a53_1_core0_pwr_stat_START (0)
#define SOC_CRGPERIPH_MAIA_COREPWRINTSTAT_intr_a53_1_core0_pwr_stat_END (0)
#define SOC_CRGPERIPH_MAIA_COREPWRINTSTAT_intr_a53_1_core1_pwr_stat_START (1)
#define SOC_CRGPERIPH_MAIA_COREPWRINTSTAT_intr_a53_1_core1_pwr_stat_END (1)
#define SOC_CRGPERIPH_MAIA_COREPWRINTSTAT_intr_a53_1_core2_pwr_stat_START (2)
#define SOC_CRGPERIPH_MAIA_COREPWRINTSTAT_intr_a53_1_core2_pwr_stat_END (2)
#define SOC_CRGPERIPH_MAIA_COREPWRINTSTAT_intr_a53_1_core3_pwr_stat_START (3)
#define SOC_CRGPERIPH_MAIA_COREPWRINTSTAT_intr_a53_1_core3_pwr_stat_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_a53_1_core0_gic_mask : 1;
        unsigned int intr_a53_1_core1_gic_mask : 1;
        unsigned int intr_a53_1_core2_gic_mask : 1;
        unsigned int intr_a53_1_core3_gic_mask : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_MAIA_COREGICMASK_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_COREGICMASK_intr_a53_1_core0_gic_mask_START (0)
#define SOC_CRGPERIPH_MAIA_COREGICMASK_intr_a53_1_core0_gic_mask_END (0)
#define SOC_CRGPERIPH_MAIA_COREGICMASK_intr_a53_1_core1_gic_mask_START (1)
#define SOC_CRGPERIPH_MAIA_COREGICMASK_intr_a53_1_core1_gic_mask_END (1)
#define SOC_CRGPERIPH_MAIA_COREGICMASK_intr_a53_1_core2_gic_mask_START (2)
#define SOC_CRGPERIPH_MAIA_COREGICMASK_intr_a53_1_core2_gic_mask_END (2)
#define SOC_CRGPERIPH_MAIA_COREGICMASK_intr_a53_1_core3_gic_mask_START (3)
#define SOC_CRGPERIPH_MAIA_COREGICMASK_intr_a53_1_core3_gic_mask_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_middle_core0_pwrup_req : 1;
        unsigned int cpu_middle_core1_pwrup_req : 1;
        unsigned int cpu_middle_core2_pwrup_req : 1;
        unsigned int cpu_big_core0_pwrup_req : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_MAIA_COREPOWERUP_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_COREPOWERUP_cpu_middle_core0_pwrup_req_START (0)
#define SOC_CRGPERIPH_MAIA_COREPOWERUP_cpu_middle_core0_pwrup_req_END (0)
#define SOC_CRGPERIPH_MAIA_COREPOWERUP_cpu_middle_core1_pwrup_req_START (1)
#define SOC_CRGPERIPH_MAIA_COREPOWERUP_cpu_middle_core1_pwrup_req_END (1)
#define SOC_CRGPERIPH_MAIA_COREPOWERUP_cpu_middle_core2_pwrup_req_START (2)
#define SOC_CRGPERIPH_MAIA_COREPOWERUP_cpu_middle_core2_pwrup_req_END (2)
#define SOC_CRGPERIPH_MAIA_COREPOWERUP_cpu_big_core0_pwrup_req_START (3)
#define SOC_CRGPERIPH_MAIA_COREPOWERUP_cpu_big_core0_pwrup_req_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_middle_core0_pwrdn_req : 1;
        unsigned int cpu_middle_core1_pwrdn_req : 1;
        unsigned int cpu_middle_core2_pwrdn_req : 1;
        unsigned int cpu_big_core0_pwrdn_req : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_MAIA_COREPOWERDN_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_COREPOWERDN_cpu_middle_core0_pwrdn_req_START (0)
#define SOC_CRGPERIPH_MAIA_COREPOWERDN_cpu_middle_core0_pwrdn_req_END (0)
#define SOC_CRGPERIPH_MAIA_COREPOWERDN_cpu_middle_core1_pwrdn_req_START (1)
#define SOC_CRGPERIPH_MAIA_COREPOWERDN_cpu_middle_core1_pwrdn_req_END (1)
#define SOC_CRGPERIPH_MAIA_COREPOWERDN_cpu_middle_core2_pwrdn_req_START (2)
#define SOC_CRGPERIPH_MAIA_COREPOWERDN_cpu_middle_core2_pwrdn_req_END (2)
#define SOC_CRGPERIPH_MAIA_COREPOWERDN_cpu_big_core0_pwrdn_req_START (3)
#define SOC_CRGPERIPH_MAIA_COREPOWERDN_cpu_big_core0_pwrdn_req_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a53_1_core0_pwr_stat : 4;
        unsigned int a53_1_core1_pwr_stat : 4;
        unsigned int a53_1_core2_pwr_stat : 4;
        unsigned int a53_1_core3_pwr_stat : 4;
        unsigned int reserved : 16;
    } reg;
} SOC_CRGPERIPH_MAIA_COREPOWERSTAT_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_COREPOWERSTAT_a53_1_core0_pwr_stat_START (0)
#define SOC_CRGPERIPH_MAIA_COREPOWERSTAT_a53_1_core0_pwr_stat_END (3)
#define SOC_CRGPERIPH_MAIA_COREPOWERSTAT_a53_1_core1_pwr_stat_START (4)
#define SOC_CRGPERIPH_MAIA_COREPOWERSTAT_a53_1_core1_pwr_stat_END (7)
#define SOC_CRGPERIPH_MAIA_COREPOWERSTAT_a53_1_core2_pwr_stat_START (8)
#define SOC_CRGPERIPH_MAIA_COREPOWERSTAT_a53_1_core2_pwr_stat_END (11)
#define SOC_CRGPERIPH_MAIA_COREPOWERSTAT_a53_1_core3_pwr_stat_START (12)
#define SOC_CRGPERIPH_MAIA_COREPOWERSTAT_a53_1_core3_pwr_stat_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a53_1_core_pwrup_time : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_CRGPERIPH_MAIA_COREPWRUPTIME_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_COREPWRUPTIME_a53_1_core_pwrup_time_START (0)
#define SOC_CRGPERIPH_MAIA_COREPWRUPTIME_a53_1_core_pwrup_time_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a53_1_core_pwrdn_time : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_CRGPERIPH_MAIA_COREPWRDNTIME_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_COREPWRDNTIME_a53_1_core_pwrdn_time_START (0)
#define SOC_CRGPERIPH_MAIA_COREPWRDNTIME_a53_1_core_pwrdn_time_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a53_1_core_iso_time : 5;
        unsigned int reserved : 27;
    } reg;
} SOC_CRGPERIPH_MAIA_COREISOTIME_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_COREISOTIME_a53_1_core_iso_time_START (0)
#define SOC_CRGPERIPH_MAIA_COREISOTIME_a53_1_core_iso_time_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a53_1_core_dbg_time : 4;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_MAIA_COREDBGTIME_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_COREDBGTIME_a53_1_core_dbg_time_START (0)
#define SOC_CRGPERIPH_MAIA_COREDBGTIME_a53_1_core_dbg_time_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a53_1_core_rst_time : 3;
        unsigned int reserved : 29;
    } reg;
} SOC_CRGPERIPH_MAIA_CORERSTTIME_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_CORERSTTIME_a53_1_core_rst_time_START (0)
#define SOC_CRGPERIPH_MAIA_CORERSTTIME_a53_1_core_rst_time_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a53_1_core_urst_time : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_CRGPERIPH_MAIA_COREURSTTIME_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_COREURSTTIME_a53_1_core_urst_time_START (0)
#define SOC_CRGPERIPH_MAIA_COREURSTTIME_a53_1_core_urst_time_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a53_1_core_ocldo_dis_time : 16;
        unsigned int a53_1_core_ocldo_en_time : 16;
    } reg;
} SOC_CRGPERIPH_MAIA_COREOCLDOTIME0_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_COREOCLDOTIME0_a53_1_core_ocldo_dis_time_START (0)
#define SOC_CRGPERIPH_MAIA_COREOCLDOTIME0_a53_1_core_ocldo_dis_time_END (15)
#define SOC_CRGPERIPH_MAIA_COREOCLDOTIME0_a53_1_core_ocldo_en_time_START (16)
#define SOC_CRGPERIPH_MAIA_COREOCLDOTIME0_a53_1_core_ocldo_en_time_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a53_1_core_ocldo_pwrdn_time : 16;
        unsigned int a53_1_core_ocldo_pwrup_time : 16;
    } reg;
} SOC_CRGPERIPH_MAIA_COREOCLDOTIME1_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_COREOCLDOTIME1_a53_1_core_ocldo_pwrdn_time_START (0)
#define SOC_CRGPERIPH_MAIA_COREOCLDOTIME1_a53_1_core_ocldo_pwrdn_time_END (15)
#define SOC_CRGPERIPH_MAIA_COREOCLDOTIME1_a53_1_core_ocldo_pwrup_time_START (16)
#define SOC_CRGPERIPH_MAIA_COREOCLDOTIME1_a53_1_core_ocldo_pwrup_time_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a53_1_core0_ocdlo_stat : 3;
        unsigned int a53_1_core1_ocdlo_stat : 3;
        unsigned int a53_1_core2_ocdlo_stat : 3;
        unsigned int a53_1_core3_ocdlo_stat : 3;
        unsigned int reserved : 20;
    } reg;
} SOC_CRGPERIPH_MAIA_COREOCLDOSTAT_UNION;
#endif
#define SOC_CRGPERIPH_MAIA_COREOCLDOSTAT_a53_1_core0_ocdlo_stat_START (0)
#define SOC_CRGPERIPH_MAIA_COREOCLDOSTAT_a53_1_core0_ocdlo_stat_END (2)
#define SOC_CRGPERIPH_MAIA_COREOCLDOSTAT_a53_1_core1_ocdlo_stat_START (3)
#define SOC_CRGPERIPH_MAIA_COREOCLDOSTAT_a53_1_core1_ocdlo_stat_END (5)
#define SOC_CRGPERIPH_MAIA_COREOCLDOSTAT_a53_1_core2_ocdlo_stat_START (6)
#define SOC_CRGPERIPH_MAIA_COREOCLDOSTAT_a53_1_core2_ocdlo_stat_END (8)
#define SOC_CRGPERIPH_MAIA_COREOCLDOSTAT_a53_1_core3_ocdlo_stat_START (9)
#define SOC_CRGPERIPH_MAIA_COREOCLDOSTAT_a53_1_core3_ocdlo_stat_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int big_core0_dvfs_dis_req_bypass : 1;
        unsigned int big_core1_dvfs_dis_req_bypass : 1;
        unsigned int big_core2_dvfs_dis_req_bypass : 1;
        unsigned int big_core3_dvfs_dis_req_bypass : 1;
        unsigned int big_core0_sw2ocldo_req_bypass : 1;
        unsigned int big_core1_sw2ocldo_req_bypass : 1;
        unsigned int big_core2_sw2ocldo_req_bypass : 1;
        unsigned int big_core3_sw2ocldo_req_bypass : 1;
        unsigned int big_core0_ocldo_dis_req_bypass : 1;
        unsigned int big_core1_ocldo_dis_req_bypass : 1;
        unsigned int big_core2_ocldo_dis_req_bypass : 1;
        unsigned int big_core3_ocldo_dis_req_bypass : 1;
        unsigned int reserved : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_UNION;
#endif
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core0_dvfs_dis_req_bypass_START (0)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core0_dvfs_dis_req_bypass_END (0)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core1_dvfs_dis_req_bypass_START (1)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core1_dvfs_dis_req_bypass_END (1)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core2_dvfs_dis_req_bypass_START (2)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core2_dvfs_dis_req_bypass_END (2)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core3_dvfs_dis_req_bypass_START (3)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core3_dvfs_dis_req_bypass_END (3)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core0_sw2ocldo_req_bypass_START (4)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core0_sw2ocldo_req_bypass_END (4)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core1_sw2ocldo_req_bypass_START (5)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core1_sw2ocldo_req_bypass_END (5)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core2_sw2ocldo_req_bypass_START (6)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core2_sw2ocldo_req_bypass_END (6)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core3_sw2ocldo_req_bypass_START (7)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core3_sw2ocldo_req_bypass_END (7)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core0_ocldo_dis_req_bypass_START (8)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core0_ocldo_dis_req_bypass_END (8)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core1_ocldo_dis_req_bypass_START (9)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core1_ocldo_dis_req_bypass_END (9)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core2_ocldo_dis_req_bypass_START (10)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core2_ocldo_dis_req_bypass_END (10)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core3_ocldo_dis_req_bypass_START (11)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_big_core3_ocldo_dis_req_bypass_END (11)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_bitmasken_START (16)
#define SOC_CRGPERIPH_BIG_CORE_PDC_PMC_CTRL_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gpu_lv_num_in : 5;
        unsigned int gpu_lv_num_out : 5;
        unsigned int reserved : 22;
    } reg;
} SOC_CRGPERIPH_GPU_LV_CTRL_UNION;
#endif
#define SOC_CRGPERIPH_GPU_LV_CTRL_gpu_lv_num_in_START (0)
#define SOC_CRGPERIPH_GPU_LV_CTRL_gpu_lv_num_in_END (4)
#define SOC_CRGPERIPH_GPU_LV_CTRL_gpu_lv_num_out_START (5)
#define SOC_CRGPERIPH_GPU_LV_CTRL_gpu_lv_num_out_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int adb_debounce_num_out : 8;
        unsigned int clkon_req_latmon_dmss_bypass : 1;
        unsigned int adb_qhanske_enable_acp_mst_bypass : 1;
        unsigned int adb_qhanske_enable_acp_slv_bypass : 1;
        unsigned int reserved_0 : 1;
        unsigned int ddrc_buf_ddr_busy_bypass : 1;
        unsigned int ddrc_buf_busy_bypass : 1;
        unsigned int dmss2dmc_mux_autogt_bypass : 1;
        unsigned int ddrc_top_idle_bypass : 1;
        unsigned int reserved_1 : 6;
        unsigned int reserved_2 : 6;
        unsigned int reserved_3 : 1;
        unsigned int adb_qhanske_enable_mst_bypass : 1;
        unsigned int adb_qhanske_enable_slv_bypass : 1;
        unsigned int ddrc_core_up_auto_gt_bypass : 1;
    } reg;
} SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_adb_debounce_num_out_START (0)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_adb_debounce_num_out_END (7)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_clkon_req_latmon_dmss_bypass_START (8)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_clkon_req_latmon_dmss_bypass_END (8)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_adb_qhanske_enable_acp_mst_bypass_START (9)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_adb_qhanske_enable_acp_mst_bypass_END (9)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_adb_qhanske_enable_acp_slv_bypass_START (10)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_adb_qhanske_enable_acp_slv_bypass_END (10)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_ddrc_buf_ddr_busy_bypass_START (12)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_ddrc_buf_ddr_busy_bypass_END (12)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_ddrc_buf_busy_bypass_START (13)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_ddrc_buf_busy_bypass_END (13)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_dmss2dmc_mux_autogt_bypass_START (14)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_dmss2dmc_mux_autogt_bypass_END (14)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_ddrc_top_idle_bypass_START (15)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_ddrc_top_idle_bypass_END (15)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_adb_qhanske_enable_mst_bypass_START (29)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_adb_qhanske_enable_mst_bypass_END (29)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_adb_qhanske_enable_slv_bypass_START (30)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_adb_qhanske_enable_slv_bypass_END (30)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_ddrc_core_up_auto_gt_bypass_START (31)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL1_ddrc_core_up_auto_gt_bypass_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmc_peri_autogt_mode : 1;
        unsigned int dmc_peri_autogt_cnt_cfg : 6;
        unsigned int dmc_peri_autogt_bypass : 1;
        unsigned int reserved : 8;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_DMC_PERI_AUTOGT_CTRL_UNION;
#endif
#define SOC_CRGPERIPH_DMC_PERI_AUTOGT_CTRL_dmc_peri_autogt_mode_START (0)
#define SOC_CRGPERIPH_DMC_PERI_AUTOGT_CTRL_dmc_peri_autogt_mode_END (0)
#define SOC_CRGPERIPH_DMC_PERI_AUTOGT_CTRL_dmc_peri_autogt_cnt_cfg_START (1)
#define SOC_CRGPERIPH_DMC_PERI_AUTOGT_CTRL_dmc_peri_autogt_cnt_cfg_END (6)
#define SOC_CRGPERIPH_DMC_PERI_AUTOGT_CTRL_dmc_peri_autogt_bypass_START (7)
#define SOC_CRGPERIPH_DMC_PERI_AUTOGT_CTRL_dmc_peri_autogt_bypass_END (7)
#define SOC_CRGPERIPH_DMC_PERI_AUTOGT_CTRL_bitmasken_START (16)
#define SOC_CRGPERIPH_DMC_PERI_AUTOGT_CTRL_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ddrc_core_r_auto_gt_bypass : 1;
        unsigned int ddrc_lp_waitcfg_in : 10;
        unsigned int ddrc_lp_waitcfg_out : 10;
        unsigned int ddrc_autogt_mode : 1;
        unsigned int ddrc_autogt_cnt_cfg : 6;
        unsigned int mddrc_idle_bypass : 1;
        unsigned int ddrc_atgs_mon_bypass : 1;
        unsigned int ddrc_sys_clk_auto_gt_bypass : 1;
        unsigned int ddrc_core_l_auto_gt_bypass : 1;
    } reg;
} SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_UNION;
#endif
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_core_r_auto_gt_bypass_START (0)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_core_r_auto_gt_bypass_END (0)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_lp_waitcfg_in_START (1)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_lp_waitcfg_in_END (10)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_lp_waitcfg_out_START (11)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_lp_waitcfg_out_END (20)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_autogt_mode_START (21)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_autogt_mode_END (21)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_autogt_cnt_cfg_START (22)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_autogt_cnt_cfg_END (27)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_mddrc_idle_bypass_START (28)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_mddrc_idle_bypass_END (28)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_atgs_mon_bypass_START (29)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_atgs_mon_bypass_END (29)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_sys_clk_auto_gt_bypass_START (30)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_sys_clk_auto_gt_bypass_END (30)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_core_l_auto_gt_bypass_START (31)
#define SOC_CRGPERIPH_DDRC_AUTOGT_CTRL_ddrc_core_l_auto_gt_bypass_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ivp_div_auto_reduce_bypass : 1;
        unsigned int reserved_0 : 10;
        unsigned int reserved_1 : 9;
        unsigned int reserved_2 : 1;
        unsigned int ivp_div_auto_cfg : 6;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV0_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV0_ivp_div_auto_reduce_bypass_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV0_ivp_div_auto_reduce_bypass_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV0_ivp_div_auto_cfg_START (21)
#define SOC_CRGPERIPH_PERI_AUTODIV0_ivp_div_auto_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int venc2_div_auto_reduce_bypass : 1;
        unsigned int reserved_0 : 10;
        unsigned int reserved_1 : 10;
        unsigned int venc2_div_auto_cfg : 6;
        unsigned int reserved_2 : 5;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV1_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV1_venc2_div_auto_reduce_bypass_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV1_venc2_div_auto_reduce_bypass_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV1_venc2_div_auto_cfg_START (21)
#define SOC_CRGPERIPH_PERI_AUTODIV1_venc2_div_auto_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int venc_div_auto_reduce_bypass : 1;
        unsigned int reserved_0 : 10;
        unsigned int reserved_1 : 10;
        unsigned int venc_div_auto_cfg : 6;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 4;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV2_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV2_venc_div_auto_reduce_bypass_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV2_venc_div_auto_reduce_bypass_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV2_venc_div_auto_cfg_START (21)
#define SOC_CRGPERIPH_PERI_AUTODIV2_venc_div_auto_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vdec_div_auto_reduce_bypass : 1;
        unsigned int reserved_0 : 10;
        unsigned int reserved_1 : 10;
        unsigned int vdec_div_auto_cfg : 6;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 4;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV3_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV3_vdec_div_auto_reduce_bypass_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV3_vdec_div_auto_reduce_bypass_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV3_vdec_div_auto_cfg_START (21)
#define SOC_CRGPERIPH_PERI_AUTODIV3_vdec_div_auto_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hiface_div_auto_reduce_bypass : 1;
        unsigned int reserved_0 : 10;
        unsigned int reserved_1 : 8;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int hiface_div_auto_cfg : 6;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV4_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV4_hiface_div_auto_reduce_bypass_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV4_hiface_div_auto_reduce_bypass_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV4_hiface_div_auto_cfg_START (21)
#define SOC_CRGPERIPH_PERI_AUTODIV4_hiface_div_auto_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vcodecbus_div_auto_reduce_bypass : 1;
        unsigned int reserved_0 : 10;
        unsigned int reserved_1 : 10;
        unsigned int vcodecbus_div_auto_cfg : 6;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV5_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV5_vcodecbus_div_auto_reduce_bypass_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV5_vcodecbus_div_auto_reduce_bypass_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV5_vcodecbus_div_auto_cfg_START (21)
#define SOC_CRGPERIPH_PERI_AUTODIV5_vcodecbus_div_auto_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sysbus_div_auto_reduce_bypass_lpm3 : 1;
        unsigned int sysbus_auto_waitcfg_in : 10;
        unsigned int sysbus_auto_waitcfg_out : 10;
        unsigned int sysbus_div_auto_cfg : 6;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int sysbus_pcie1_bypass : 1;
        unsigned int sysbus_latmon_bypass : 1;
        unsigned int sysbus_dpctrl_bypass : 1;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV6_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV6_sysbus_div_auto_reduce_bypass_lpm3_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV6_sysbus_div_auto_reduce_bypass_lpm3_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV6_sysbus_auto_waitcfg_in_START (1)
#define SOC_CRGPERIPH_PERI_AUTODIV6_sysbus_auto_waitcfg_in_END (10)
#define SOC_CRGPERIPH_PERI_AUTODIV6_sysbus_auto_waitcfg_out_START (11)
#define SOC_CRGPERIPH_PERI_AUTODIV6_sysbus_auto_waitcfg_out_END (20)
#define SOC_CRGPERIPH_PERI_AUTODIV6_sysbus_div_auto_cfg_START (21)
#define SOC_CRGPERIPH_PERI_AUTODIV6_sysbus_div_auto_cfg_END (26)
#define SOC_CRGPERIPH_PERI_AUTODIV6_sysbus_pcie1_bypass_START (29)
#define SOC_CRGPERIPH_PERI_AUTODIV6_sysbus_pcie1_bypass_END (29)
#define SOC_CRGPERIPH_PERI_AUTODIV6_sysbus_latmon_bypass_START (30)
#define SOC_CRGPERIPH_PERI_AUTODIV6_sysbus_latmon_bypass_END (30)
#define SOC_CRGPERIPH_PERI_AUTODIV6_sysbus_dpctrl_bypass_START (31)
#define SOC_CRGPERIPH_PERI_AUTODIV6_sysbus_dpctrl_bypass_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sysbus_aobus_bypass : 1;
        unsigned int sysbus_top_cssys_bypass : 1;
        unsigned int sysbus_perf_stat_bypass : 1;
        unsigned int sysbus_usb3_bypass : 1;
        unsigned int sysbus_vivo_cfg_bypass : 1;
        unsigned int sysbus_sec_bypass : 1;
        unsigned int sysbus_socp_bypass : 1;
        unsigned int sysbus_dmac_mst_bypass : 1;
        unsigned int sysbus_ivp32_core1_mst_bypass : 1;
        unsigned int sysbus_pcie_bypass : 1;
        unsigned int sysbus_sd3_bypass : 1;
        unsigned int sysbus_ipf_bypass : 1;
        unsigned int sysbus_cci2sysbus_bypass : 1;
        unsigned int sysbus_modem_mst_bypass : 1;
        unsigned int sysbus_ufs_bypass : 1;
        unsigned int sysbus_djtag_mst_bypass : 1;
        unsigned int sysbus_lpm3_mst_bypass : 1;
        unsigned int sysbus_eps_cfg_bypass : 1;
        unsigned int sysbus_ivp32_core0_mst_bypass : 1;
        unsigned int dmabus_latmon_bypass : 1;
        unsigned int dmabus_perf_stat_bypass : 1;
        unsigned int dmabus_ipf_bypass : 1;
        unsigned int cfgbus_lpmcu_slv_t_bypass : 1;
        unsigned int cfgbus_hkadc_ssi_t_bypass : 1;
        unsigned int dmabus_socp_bypass : 1;
        unsigned int dmabus_top_cssys_bypass : 1;
        unsigned int cfgbus_gpu_cfg_t_bypass : 1;
        unsigned int sysbus_vcodec_cfg_bypass : 1;
        unsigned int sysbus_ispcpu_cfg_bypass : 1;
        unsigned int cfgbus_djtag_mst_bypass : 1;
        unsigned int cfgbus_lpm3_mst_bypass : 1;
        unsigned int cfgbus_peri1_cfg_t_bypass : 1;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV7_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_aobus_bypass_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_aobus_bypass_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_top_cssys_bypass_START (1)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_top_cssys_bypass_END (1)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_perf_stat_bypass_START (2)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_perf_stat_bypass_END (2)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_usb3_bypass_START (3)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_usb3_bypass_END (3)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_vivo_cfg_bypass_START (4)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_vivo_cfg_bypass_END (4)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_sec_bypass_START (5)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_sec_bypass_END (5)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_socp_bypass_START (6)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_socp_bypass_END (6)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_dmac_mst_bypass_START (7)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_dmac_mst_bypass_END (7)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_ivp32_core1_mst_bypass_START (8)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_ivp32_core1_mst_bypass_END (8)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_pcie_bypass_START (9)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_pcie_bypass_END (9)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_sd3_bypass_START (10)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_sd3_bypass_END (10)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_ipf_bypass_START (11)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_ipf_bypass_END (11)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_cci2sysbus_bypass_START (12)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_cci2sysbus_bypass_END (12)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_modem_mst_bypass_START (13)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_modem_mst_bypass_END (13)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_ufs_bypass_START (14)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_ufs_bypass_END (14)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_djtag_mst_bypass_START (15)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_djtag_mst_bypass_END (15)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_lpm3_mst_bypass_START (16)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_lpm3_mst_bypass_END (16)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_eps_cfg_bypass_START (17)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_eps_cfg_bypass_END (17)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_ivp32_core0_mst_bypass_START (18)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_ivp32_core0_mst_bypass_END (18)
#define SOC_CRGPERIPH_PERI_AUTODIV7_dmabus_latmon_bypass_START (19)
#define SOC_CRGPERIPH_PERI_AUTODIV7_dmabus_latmon_bypass_END (19)
#define SOC_CRGPERIPH_PERI_AUTODIV7_dmabus_perf_stat_bypass_START (20)
#define SOC_CRGPERIPH_PERI_AUTODIV7_dmabus_perf_stat_bypass_END (20)
#define SOC_CRGPERIPH_PERI_AUTODIV7_dmabus_ipf_bypass_START (21)
#define SOC_CRGPERIPH_PERI_AUTODIV7_dmabus_ipf_bypass_END (21)
#define SOC_CRGPERIPH_PERI_AUTODIV7_cfgbus_lpmcu_slv_t_bypass_START (22)
#define SOC_CRGPERIPH_PERI_AUTODIV7_cfgbus_lpmcu_slv_t_bypass_END (22)
#define SOC_CRGPERIPH_PERI_AUTODIV7_cfgbus_hkadc_ssi_t_bypass_START (23)
#define SOC_CRGPERIPH_PERI_AUTODIV7_cfgbus_hkadc_ssi_t_bypass_END (23)
#define SOC_CRGPERIPH_PERI_AUTODIV7_dmabus_socp_bypass_START (24)
#define SOC_CRGPERIPH_PERI_AUTODIV7_dmabus_socp_bypass_END (24)
#define SOC_CRGPERIPH_PERI_AUTODIV7_dmabus_top_cssys_bypass_START (25)
#define SOC_CRGPERIPH_PERI_AUTODIV7_dmabus_top_cssys_bypass_END (25)
#define SOC_CRGPERIPH_PERI_AUTODIV7_cfgbus_gpu_cfg_t_bypass_START (26)
#define SOC_CRGPERIPH_PERI_AUTODIV7_cfgbus_gpu_cfg_t_bypass_END (26)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_vcodec_cfg_bypass_START (27)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_vcodec_cfg_bypass_END (27)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_ispcpu_cfg_bypass_START (28)
#define SOC_CRGPERIPH_PERI_AUTODIV7_sysbus_ispcpu_cfg_bypass_END (28)
#define SOC_CRGPERIPH_PERI_AUTODIV7_cfgbus_djtag_mst_bypass_START (29)
#define SOC_CRGPERIPH_PERI_AUTODIV7_cfgbus_djtag_mst_bypass_END (29)
#define SOC_CRGPERIPH_PERI_AUTODIV7_cfgbus_lpm3_mst_bypass_START (30)
#define SOC_CRGPERIPH_PERI_AUTODIV7_cfgbus_lpm3_mst_bypass_END (30)
#define SOC_CRGPERIPH_PERI_AUTODIV7_cfgbus_peri1_cfg_t_bypass_START (31)
#define SOC_CRGPERIPH_PERI_AUTODIV7_cfgbus_peri1_cfg_t_bypass_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cfgbus_div_auto_reduce_bypass_lpm3 : 1;
        unsigned int cfgbus_auto_waitcfg_in : 10;
        unsigned int cfgbus_auto_waitcfg_out : 10;
        unsigned int cfgbus_div_auto_cfg : 6;
        unsigned int cfgbus_sysbus_relate_auto_reduce_bypass : 1;
        unsigned int cfgbus_vivo_cfg_bypass : 1;
        unsigned int cfgbus_vcodec_cfg_bypass : 1;
        unsigned int sysbus_fcm2sys_t_bypass : 1;
        unsigned int cfgbus_peri0_cfg_t_bypass : 1;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV8_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_div_auto_reduce_bypass_lpm3_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_div_auto_reduce_bypass_lpm3_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_auto_waitcfg_in_START (1)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_auto_waitcfg_in_END (10)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_auto_waitcfg_out_START (11)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_auto_waitcfg_out_END (20)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_div_auto_cfg_START (21)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_div_auto_cfg_END (26)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_sysbus_relate_auto_reduce_bypass_START (27)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_sysbus_relate_auto_reduce_bypass_END (27)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_vivo_cfg_bypass_START (28)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_vivo_cfg_bypass_END (28)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_vcodec_cfg_bypass_START (29)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_vcodec_cfg_bypass_END (29)
#define SOC_CRGPERIPH_PERI_AUTODIV8_sysbus_fcm2sys_t_bypass_START (30)
#define SOC_CRGPERIPH_PERI_AUTODIV8_sysbus_fcm2sys_t_bypass_END (30)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_peri0_cfg_t_bypass_START (31)
#define SOC_CRGPERIPH_PERI_AUTODIV8_cfgbus_peri0_cfg_t_bypass_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmabus_div_auto_reduce_bypass_lpm3 : 1;
        unsigned int dmabus_auto_waitcfg_in : 10;
        unsigned int dmabus_auto_waitcfg_out : 10;
        unsigned int dmabus_div_auto_cfg : 6;
        unsigned int dmabus_dmac_mst_bypass : 1;
        unsigned int dmabus_sysbus_relate_auto_reduce_bypass : 1;
        unsigned int dmabus_hisee_cfg_t_bypass : 1;
        unsigned int dmabus_dmac_cfg_t_bypass : 1;
        unsigned int dmabus_cssys_slv_t_bypass : 1;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV9_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_div_auto_reduce_bypass_lpm3_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_div_auto_reduce_bypass_lpm3_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_auto_waitcfg_in_START (1)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_auto_waitcfg_in_END (10)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_auto_waitcfg_out_START (11)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_auto_waitcfg_out_END (20)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_div_auto_cfg_START (21)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_div_auto_cfg_END (26)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_dmac_mst_bypass_START (27)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_dmac_mst_bypass_END (27)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_sysbus_relate_auto_reduce_bypass_START (28)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_sysbus_relate_auto_reduce_bypass_END (28)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_hisee_cfg_t_bypass_START (29)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_hisee_cfg_t_bypass_END (29)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_dmac_cfg_t_bypass_START (30)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_dmac_cfg_t_bypass_END (30)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_cssys_slv_t_bypass_START (31)
#define SOC_CRGPERIPH_PERI_AUTODIV9_dmabus_cssys_slv_t_bypass_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tscpu_div_auto_reduce_bypass : 1;
        unsigned int tscpu_auto_waitcfg_in : 10;
        unsigned int tscpu_auto_waitcfg_out : 10;
        unsigned int tscpu_div_auto_cfg : 6;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 4;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV10_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV10_tscpu_div_auto_reduce_bypass_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV10_tscpu_div_auto_reduce_bypass_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV10_tscpu_auto_waitcfg_in_START (1)
#define SOC_CRGPERIPH_PERI_AUTODIV10_tscpu_auto_waitcfg_in_END (10)
#define SOC_CRGPERIPH_PERI_AUTODIV10_tscpu_auto_waitcfg_out_START (11)
#define SOC_CRGPERIPH_PERI_AUTODIV10_tscpu_auto_waitcfg_out_END (20)
#define SOC_CRGPERIPH_PERI_AUTODIV10_tscpu_div_auto_cfg_START (21)
#define SOC_CRGPERIPH_PERI_AUTODIV10_tscpu_div_auto_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int lpmcu_fll_div_auto_cfg : 4;
        unsigned int sysbus_fll_div_auto_cfg : 4;
        unsigned int reserved_0 : 13;
        unsigned int reserved_1 : 6;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV11_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV11_lpmcu_fll_div_auto_cfg_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV11_lpmcu_fll_div_auto_cfg_END (3)
#define SOC_CRGPERIPH_PERI_AUTODIV11_sysbus_fll_div_auto_cfg_START (4)
#define SOC_CRGPERIPH_PERI_AUTODIV11_sysbus_fll_div_auto_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hsdt1bus_div_auto_reduce_bypass_lpm3 : 1;
        unsigned int hsdt1bus_auto_waitcfg_in : 10;
        unsigned int hsdt1bus_auto_waitcfg_out : 10;
        unsigned int hsdt1bus_div_auto_cfg : 6;
        unsigned int hsdt1bus_sysbus_relate_auto_reduce_bypass : 1;
        unsigned int hsdt1bus_usb3_bypass : 1;
        unsigned int hsdt1bus_dpctrl_bypass : 1;
        unsigned int hsdt1bus_sd_bypass : 1;
        unsigned int hsdt1bus_usb3_cfg_t_bypass : 1;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV12_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_div_auto_reduce_bypass_lpm3_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_div_auto_reduce_bypass_lpm3_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_auto_waitcfg_in_START (1)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_auto_waitcfg_in_END (10)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_auto_waitcfg_out_START (11)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_auto_waitcfg_out_END (20)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_div_auto_cfg_START (21)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_div_auto_cfg_END (26)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_sysbus_relate_auto_reduce_bypass_START (27)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_sysbus_relate_auto_reduce_bypass_END (27)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_usb3_bypass_START (28)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_usb3_bypass_END (28)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_dpctrl_bypass_START (29)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_dpctrl_bypass_END (29)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_sd_bypass_START (30)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_sd_bypass_END (30)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_usb3_cfg_t_bypass_START (31)
#define SOC_CRGPERIPH_PERI_AUTODIV12_hsdt1bus_usb3_cfg_t_bypass_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int lpmcu_div_auto_reduce_bypass : 1;
        unsigned int lpmcu_auto_waitcfg_in : 10;
        unsigned int lpmcu_auto_waitcfg_out : 10;
        unsigned int lpmcu_div_auto_cfg : 6;
        unsigned int lpmcu_idle_bypass : 1;
        unsigned int lpmcu_bus_slv_idle_bypass : 1;
        unsigned int reserved : 3;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV13_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV13_lpmcu_div_auto_reduce_bypass_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV13_lpmcu_div_auto_reduce_bypass_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV13_lpmcu_auto_waitcfg_in_START (1)
#define SOC_CRGPERIPH_PERI_AUTODIV13_lpmcu_auto_waitcfg_in_END (10)
#define SOC_CRGPERIPH_PERI_AUTODIV13_lpmcu_auto_waitcfg_out_START (11)
#define SOC_CRGPERIPH_PERI_AUTODIV13_lpmcu_auto_waitcfg_out_END (20)
#define SOC_CRGPERIPH_PERI_AUTODIV13_lpmcu_div_auto_cfg_START (21)
#define SOC_CRGPERIPH_PERI_AUTODIV13_lpmcu_div_auto_cfg_END (26)
#define SOC_CRGPERIPH_PERI_AUTODIV13_lpmcu_idle_bypass_START (27)
#define SOC_CRGPERIPH_PERI_AUTODIV13_lpmcu_idle_bypass_END (27)
#define SOC_CRGPERIPH_PERI_AUTODIV13_lpmcu_bus_slv_idle_bypass_START (28)
#define SOC_CRGPERIPH_PERI_AUTODIV13_lpmcu_bus_slv_idle_bypass_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a57_auto_swtotcxo_bypass : 1;
        unsigned int a57_auto_waitcfg_in : 10;
        unsigned int a57_auto_waitcfg_out : 10;
        unsigned int reserved : 11;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV14_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV14_a57_auto_swtotcxo_bypass_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV14_a57_auto_swtotcxo_bypass_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV14_a57_auto_waitcfg_in_START (1)
#define SOC_CRGPERIPH_PERI_AUTODIV14_a57_auto_waitcfg_in_END (10)
#define SOC_CRGPERIPH_PERI_AUTODIV14_a57_auto_waitcfg_out_START (11)
#define SOC_CRGPERIPH_PERI_AUTODIV14_a57_auto_waitcfg_out_END (20)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int a53_auto_swtotcxo_bypass : 1;
        unsigned int a53_auto_waitcfg_in : 10;
        unsigned int a53_auto_waitcfg_out : 10;
        unsigned int reserved : 11;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV15_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV15_a53_auto_swtotcxo_bypass_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV15_a53_auto_swtotcxo_bypass_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV15_a53_auto_waitcfg_in_START (1)
#define SOC_CRGPERIPH_PERI_AUTODIV15_a53_auto_waitcfg_in_END (10)
#define SOC_CRGPERIPH_PERI_AUTODIV15_a53_auto_waitcfg_out_START (11)
#define SOC_CRGPERIPH_PERI_AUTODIV15_a53_auto_waitcfg_out_END (20)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmabus_peri2_cfg_t_bypass : 1;
        unsigned int dmabus_peri3_cfg_t_bypass : 1;
        unsigned int hsdt1bus_apb_cfg_t_bypass : 1;
        unsigned int dmabus_spe_bypass : 1;
        unsigned int cfgbus_bba_cfg_t_bypass : 1;
        unsigned int sysbus_modem5g_mst_bypass : 1;
        unsigned int sysbus_maa_bypass : 1;
        unsigned int sysbus_maa_idle_bypass : 1;
        unsigned int sysbus_spe_bypass : 1;
        unsigned int sysbus_spe_idle_bypass : 1;
        unsigned int dmabus_spe_idle_bypass : 1;
        unsigned int dmabus_socp_idle_bypass : 1;
        unsigned int reserved : 1;
        unsigned int sysbus_socp_idle_bypass : 1;
        unsigned int sysbus_ipf_idle_bypass : 1;
        unsigned int sysbus_trace1_bypass : 1;
        unsigned int sysbus_pcie1_tcu_bypass : 1;
        unsigned int sysbus_ipp_cfg_bypass : 1;
        unsigned int sysbus_pcie0_tcu_bypass : 1;
        unsigned int sysbus_trace0_bypass : 1;
        unsigned int sysbus_hibernate_freqdonw_bypass : 1;
        unsigned int sysbus_npu2sys_bypass : 1;
        unsigned int sysbus_npu2sys_t_bypass : 1;
        unsigned int sysbus_arpp_cfg_bypass : 1;
        unsigned int cfgbus_qic_cfg_t_bypass : 1;
        unsigned int hsdt1bus_pcie1_bypass : 1;
        unsigned int hsdt1bus_pcie1_tcu_bypass : 1;
        unsigned int hsdt1bus_trace1_bypass : 1;
        unsigned int hsdt1bus_pcie1_cfg_t_bypass : 1;
        unsigned int dmabus_ipf_idle_bypass : 1;
        unsigned int dmabus_perf_idle_bypass : 1;
        unsigned int sysbus2ddrc_perf_idle_bypass : 1;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV16_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV16_dmabus_peri2_cfg_t_bypass_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV16_dmabus_peri2_cfg_t_bypass_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV16_dmabus_peri3_cfg_t_bypass_START (1)
#define SOC_CRGPERIPH_PERI_AUTODIV16_dmabus_peri3_cfg_t_bypass_END (1)
#define SOC_CRGPERIPH_PERI_AUTODIV16_hsdt1bus_apb_cfg_t_bypass_START (2)
#define SOC_CRGPERIPH_PERI_AUTODIV16_hsdt1bus_apb_cfg_t_bypass_END (2)
#define SOC_CRGPERIPH_PERI_AUTODIV16_dmabus_spe_bypass_START (3)
#define SOC_CRGPERIPH_PERI_AUTODIV16_dmabus_spe_bypass_END (3)
#define SOC_CRGPERIPH_PERI_AUTODIV16_cfgbus_bba_cfg_t_bypass_START (4)
#define SOC_CRGPERIPH_PERI_AUTODIV16_cfgbus_bba_cfg_t_bypass_END (4)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_modem5g_mst_bypass_START (5)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_modem5g_mst_bypass_END (5)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_maa_bypass_START (6)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_maa_bypass_END (6)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_maa_idle_bypass_START (7)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_maa_idle_bypass_END (7)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_spe_bypass_START (8)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_spe_bypass_END (8)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_spe_idle_bypass_START (9)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_spe_idle_bypass_END (9)
#define SOC_CRGPERIPH_PERI_AUTODIV16_dmabus_spe_idle_bypass_START (10)
#define SOC_CRGPERIPH_PERI_AUTODIV16_dmabus_spe_idle_bypass_END (10)
#define SOC_CRGPERIPH_PERI_AUTODIV16_dmabus_socp_idle_bypass_START (11)
#define SOC_CRGPERIPH_PERI_AUTODIV16_dmabus_socp_idle_bypass_END (11)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_socp_idle_bypass_START (13)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_socp_idle_bypass_END (13)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_ipf_idle_bypass_START (14)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_ipf_idle_bypass_END (14)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_trace1_bypass_START (15)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_trace1_bypass_END (15)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_pcie1_tcu_bypass_START (16)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_pcie1_tcu_bypass_END (16)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_ipp_cfg_bypass_START (17)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_ipp_cfg_bypass_END (17)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_pcie0_tcu_bypass_START (18)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_pcie0_tcu_bypass_END (18)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_trace0_bypass_START (19)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_trace0_bypass_END (19)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_hibernate_freqdonw_bypass_START (20)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_hibernate_freqdonw_bypass_END (20)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_npu2sys_bypass_START (21)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_npu2sys_bypass_END (21)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_npu2sys_t_bypass_START (22)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_npu2sys_t_bypass_END (22)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_arpp_cfg_bypass_START (23)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus_arpp_cfg_bypass_END (23)
#define SOC_CRGPERIPH_PERI_AUTODIV16_cfgbus_qic_cfg_t_bypass_START (24)
#define SOC_CRGPERIPH_PERI_AUTODIV16_cfgbus_qic_cfg_t_bypass_END (24)
#define SOC_CRGPERIPH_PERI_AUTODIV16_hsdt1bus_pcie1_bypass_START (25)
#define SOC_CRGPERIPH_PERI_AUTODIV16_hsdt1bus_pcie1_bypass_END (25)
#define SOC_CRGPERIPH_PERI_AUTODIV16_hsdt1bus_pcie1_tcu_bypass_START (26)
#define SOC_CRGPERIPH_PERI_AUTODIV16_hsdt1bus_pcie1_tcu_bypass_END (26)
#define SOC_CRGPERIPH_PERI_AUTODIV16_hsdt1bus_trace1_bypass_START (27)
#define SOC_CRGPERIPH_PERI_AUTODIV16_hsdt1bus_trace1_bypass_END (27)
#define SOC_CRGPERIPH_PERI_AUTODIV16_hsdt1bus_pcie1_cfg_t_bypass_START (28)
#define SOC_CRGPERIPH_PERI_AUTODIV16_hsdt1bus_pcie1_cfg_t_bypass_END (28)
#define SOC_CRGPERIPH_PERI_AUTODIV16_dmabus_ipf_idle_bypass_START (29)
#define SOC_CRGPERIPH_PERI_AUTODIV16_dmabus_ipf_idle_bypass_END (29)
#define SOC_CRGPERIPH_PERI_AUTODIV16_dmabus_perf_idle_bypass_START (30)
#define SOC_CRGPERIPH_PERI_AUTODIV16_dmabus_perf_idle_bypass_END (30)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus2ddrc_perf_idle_bypass_START (31)
#define SOC_CRGPERIPH_PERI_AUTODIV16_sysbus2ddrc_perf_idle_bypass_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int hsdt1bus_div_auto_reduce_bypass_acpu : 1;
        unsigned int reserved_1 : 1;
        unsigned int dmabus_div_auto_reduce_bypass_acpu : 1;
        unsigned int cfgbus_div_auto_reduce_bypass_acpu : 1;
        unsigned int sysbus_div_auto_reduce_bypass_acpu : 1;
        unsigned int reserved_2 : 2;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV_ACPU_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV_ACPU_hsdt1bus_div_auto_reduce_bypass_acpu_START (1)
#define SOC_CRGPERIPH_PERI_AUTODIV_ACPU_hsdt1bus_div_auto_reduce_bypass_acpu_END (1)
#define SOC_CRGPERIPH_PERI_AUTODIV_ACPU_dmabus_div_auto_reduce_bypass_acpu_START (3)
#define SOC_CRGPERIPH_PERI_AUTODIV_ACPU_dmabus_div_auto_reduce_bypass_acpu_END (3)
#define SOC_CRGPERIPH_PERI_AUTODIV_ACPU_cfgbus_div_auto_reduce_bypass_acpu_START (4)
#define SOC_CRGPERIPH_PERI_AUTODIV_ACPU_cfgbus_div_auto_reduce_bypass_acpu_END (4)
#define SOC_CRGPERIPH_PERI_AUTODIV_ACPU_sysbus_div_auto_reduce_bypass_acpu_START (5)
#define SOC_CRGPERIPH_PERI_AUTODIV_ACPU_sysbus_div_auto_reduce_bypass_acpu_END (5)
#define SOC_CRGPERIPH_PERI_AUTODIV_ACPU_bitmasken_START (16)
#define SOC_CRGPERIPH_PERI_AUTODIV_ACPU_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int hsdt1bus_div_auto_reduce_bypass_mcpu : 1;
        unsigned int reserved_1 : 1;
        unsigned int dmabus_div_auto_reduce_bypass_mcpu : 1;
        unsigned int cfgbus_div_auto_reduce_bypass_mcpu : 1;
        unsigned int sysbus_div_auto_reduce_bypass_mcpu : 1;
        unsigned int reserved_2 : 2;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV_MCPU_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV_MCPU_hsdt1bus_div_auto_reduce_bypass_mcpu_START (1)
#define SOC_CRGPERIPH_PERI_AUTODIV_MCPU_hsdt1bus_div_auto_reduce_bypass_mcpu_END (1)
#define SOC_CRGPERIPH_PERI_AUTODIV_MCPU_dmabus_div_auto_reduce_bypass_mcpu_START (3)
#define SOC_CRGPERIPH_PERI_AUTODIV_MCPU_dmabus_div_auto_reduce_bypass_mcpu_END (3)
#define SOC_CRGPERIPH_PERI_AUTODIV_MCPU_cfgbus_div_auto_reduce_bypass_mcpu_START (4)
#define SOC_CRGPERIPH_PERI_AUTODIV_MCPU_cfgbus_div_auto_reduce_bypass_mcpu_END (4)
#define SOC_CRGPERIPH_PERI_AUTODIV_MCPU_sysbus_div_auto_reduce_bypass_mcpu_START (5)
#define SOC_CRGPERIPH_PERI_AUTODIV_MCPU_sysbus_div_auto_reduce_bypass_mcpu_END (5)
#define SOC_CRGPERIPH_PERI_AUTODIV_MCPU_bitmasken_START (16)
#define SOC_CRGPERIPH_PERI_AUTODIV_MCPU_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int autodiv_sysbus_stat : 1;
        unsigned int autodiv_lpmcu_stat : 1;
        unsigned int autodiv_ics_stat : 1;
        unsigned int autodiv_sysbus_fll_stat : 1;
        unsigned int autodiv_lpmcu_fll_stat : 1;
        unsigned int autodiv_cfgbus_stat : 1;
        unsigned int autodiv_dmabus_stat : 1;
        unsigned int reserved_0 : 1;
        unsigned int autogt_dmc_peri_stat : 1;
        unsigned int dmss_perf_idle : 3;
        unsigned int autogt_ddrc_stat : 1;
        unsigned int mdm5gnoc_pwr_ind : 1;
        unsigned int reserved_1 : 1;
        unsigned int autodiv_hsdt1_peribus_stat : 1;
        unsigned int qic_nopendingtrans_ib_comb_media1 : 1;
        unsigned int qic_nopendingtrans_ib_comb_media2 : 1;
        unsigned int reserved_2 : 14;
    } reg;
} SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_sysbus_stat_START (0)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_sysbus_stat_END (0)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_lpmcu_stat_START (1)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_lpmcu_stat_END (1)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_ics_stat_START (2)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_ics_stat_END (2)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_sysbus_fll_stat_START (3)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_sysbus_fll_stat_END (3)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_lpmcu_fll_stat_START (4)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_lpmcu_fll_stat_END (4)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_cfgbus_stat_START (5)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_cfgbus_stat_END (5)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_dmabus_stat_START (6)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_dmabus_stat_END (6)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autogt_dmc_peri_stat_START (8)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autogt_dmc_peri_stat_END (8)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_dmss_perf_idle_START (9)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_dmss_perf_idle_END (11)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autogt_ddrc_stat_START (12)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autogt_ddrc_stat_END (12)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_mdm5gnoc_pwr_ind_START (13)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_mdm5gnoc_pwr_ind_END (13)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_hsdt1_peribus_stat_START (15)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_autodiv_hsdt1_peribus_stat_END (15)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_qic_nopendingtrans_ib_comb_media1_START (16)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_qic_nopendingtrans_ib_comb_media1_END (16)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_qic_nopendingtrans_ib_comb_media2_START (17)
#define SOC_CRGPERIPH_PERI_AUTODIV_INUSE_STAT_qic_nopendingtrans_ib_comb_media2_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_peri_avs_mon : 1;
        unsigned int gt_clk_autofs_ddrsys_mon : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_vivo2cfgbus : 1;
        unsigned int gt_clk_autogt_dmc_peri : 1;
        unsigned int gt_pclk_media_crg : 1;
        unsigned int gt_pclk_autodiv_sysbus : 1;
        unsigned int gt_pclk_autodiv_cfgbus : 1;
        unsigned int gt_pclk_autodiv_dmabus : 1;
        unsigned int gt_clk_cfgbus_media2 : 1;
        unsigned int gt_pclk_autodiv_hsdt1bus : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_pclk_autodiv_lpmcu : 1;
        unsigned int gt_clk_cfgbus_media : 1;
        unsigned int gt_clk_atgs_mon_ddrc : 1;
        unsigned int gt_clk_autogt_ddrc : 1;
        unsigned int gt_pclk_media2_crg : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int gt_clk_ppll7_sscg : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_pclk_autodiv_tscpu : 1;
        unsigned int gt_pclk_autodiv_aicpu : 1;
        unsigned int reserved_8 : 1;
        unsigned int gt_clk_ppll2_media : 1;
        unsigned int gt_clk_ppll3_media : 1;
        unsigned int gt_clk_ppll7_media : 1;
        unsigned int gt_clk_ppll0_media : 1;
        unsigned int gt_clk_ppll2_b_media : 1;
    } reg;
} SOC_CRGPERIPH_PEREN6_UNION;
#endif
#define SOC_CRGPERIPH_PEREN6_gt_clk_peri_avs_mon_START (0)
#define SOC_CRGPERIPH_PEREN6_gt_clk_peri_avs_mon_END (0)
#define SOC_CRGPERIPH_PEREN6_gt_clk_autofs_ddrsys_mon_START (1)
#define SOC_CRGPERIPH_PEREN6_gt_clk_autofs_ddrsys_mon_END (1)
#define SOC_CRGPERIPH_PEREN6_gt_clk_vivo2cfgbus_START (3)
#define SOC_CRGPERIPH_PEREN6_gt_clk_vivo2cfgbus_END (3)
#define SOC_CRGPERIPH_PEREN6_gt_clk_autogt_dmc_peri_START (4)
#define SOC_CRGPERIPH_PEREN6_gt_clk_autogt_dmc_peri_END (4)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_media_crg_START (5)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_media_crg_END (5)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_autodiv_sysbus_START (6)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_autodiv_sysbus_END (6)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_autodiv_cfgbus_START (7)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_autodiv_cfgbus_END (7)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_autodiv_dmabus_START (8)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_autodiv_dmabus_END (8)
#define SOC_CRGPERIPH_PEREN6_gt_clk_cfgbus_media2_START (9)
#define SOC_CRGPERIPH_PEREN6_gt_clk_cfgbus_media2_END (9)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_autodiv_hsdt1bus_START (10)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_autodiv_hsdt1bus_END (10)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_autodiv_lpmcu_START (12)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_autodiv_lpmcu_END (12)
#define SOC_CRGPERIPH_PEREN6_gt_clk_cfgbus_media_START (13)
#define SOC_CRGPERIPH_PEREN6_gt_clk_cfgbus_media_END (13)
#define SOC_CRGPERIPH_PEREN6_gt_clk_atgs_mon_ddrc_START (14)
#define SOC_CRGPERIPH_PEREN6_gt_clk_atgs_mon_ddrc_END (14)
#define SOC_CRGPERIPH_PEREN6_gt_clk_autogt_ddrc_START (15)
#define SOC_CRGPERIPH_PEREN6_gt_clk_autogt_ddrc_END (15)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_media2_crg_START (16)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_media2_crg_END (16)
#define SOC_CRGPERIPH_PEREN6_gt_clk_ppll7_sscg_START (21)
#define SOC_CRGPERIPH_PEREN6_gt_clk_ppll7_sscg_END (21)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_autodiv_tscpu_START (24)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_autodiv_tscpu_END (24)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_autodiv_aicpu_START (25)
#define SOC_CRGPERIPH_PEREN6_gt_pclk_autodiv_aicpu_END (25)
#define SOC_CRGPERIPH_PEREN6_gt_clk_ppll2_media_START (27)
#define SOC_CRGPERIPH_PEREN6_gt_clk_ppll2_media_END (27)
#define SOC_CRGPERIPH_PEREN6_gt_clk_ppll3_media_START (28)
#define SOC_CRGPERIPH_PEREN6_gt_clk_ppll3_media_END (28)
#define SOC_CRGPERIPH_PEREN6_gt_clk_ppll7_media_START (29)
#define SOC_CRGPERIPH_PEREN6_gt_clk_ppll7_media_END (29)
#define SOC_CRGPERIPH_PEREN6_gt_clk_ppll0_media_START (30)
#define SOC_CRGPERIPH_PEREN6_gt_clk_ppll0_media_END (30)
#define SOC_CRGPERIPH_PEREN6_gt_clk_ppll2_b_media_START (31)
#define SOC_CRGPERIPH_PEREN6_gt_clk_ppll2_b_media_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_peri_avs_mon : 1;
        unsigned int gt_clk_autofs_ddrsys_mon : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_vivo2cfgbus : 1;
        unsigned int gt_clk_autogt_dmc_peri : 1;
        unsigned int gt_pclk_media_crg : 1;
        unsigned int gt_pclk_autodiv_sysbus : 1;
        unsigned int gt_pclk_autodiv_cfgbus : 1;
        unsigned int gt_pclk_autodiv_dmabus : 1;
        unsigned int gt_clk_cfgbus_media2 : 1;
        unsigned int gt_pclk_autodiv_hsdt1bus : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_pclk_autodiv_lpmcu : 1;
        unsigned int gt_clk_cfgbus_media : 1;
        unsigned int gt_clk_atgs_mon_ddrc : 1;
        unsigned int gt_clk_autogt_ddrc : 1;
        unsigned int gt_pclk_media2_crg : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int gt_clk_ppll7_sscg : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_pclk_autodiv_tscpu : 1;
        unsigned int gt_pclk_autodiv_aicpu : 1;
        unsigned int reserved_8 : 1;
        unsigned int gt_clk_ppll2_media : 1;
        unsigned int gt_clk_ppll3_media : 1;
        unsigned int gt_clk_ppll7_media : 1;
        unsigned int gt_clk_ppll0_media : 1;
        unsigned int gt_clk_ppll2_b_media : 1;
    } reg;
} SOC_CRGPERIPH_PERDIS6_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS6_gt_clk_peri_avs_mon_START (0)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_peri_avs_mon_END (0)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_autofs_ddrsys_mon_START (1)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_autofs_ddrsys_mon_END (1)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_vivo2cfgbus_START (3)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_vivo2cfgbus_END (3)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_autogt_dmc_peri_START (4)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_autogt_dmc_peri_END (4)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_media_crg_START (5)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_media_crg_END (5)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_autodiv_sysbus_START (6)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_autodiv_sysbus_END (6)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_autodiv_cfgbus_START (7)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_autodiv_cfgbus_END (7)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_autodiv_dmabus_START (8)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_autodiv_dmabus_END (8)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_cfgbus_media2_START (9)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_cfgbus_media2_END (9)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_autodiv_hsdt1bus_START (10)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_autodiv_hsdt1bus_END (10)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_autodiv_lpmcu_START (12)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_autodiv_lpmcu_END (12)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_cfgbus_media_START (13)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_cfgbus_media_END (13)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_atgs_mon_ddrc_START (14)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_atgs_mon_ddrc_END (14)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_autogt_ddrc_START (15)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_autogt_ddrc_END (15)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_media2_crg_START (16)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_media2_crg_END (16)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_ppll7_sscg_START (21)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_ppll7_sscg_END (21)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_autodiv_tscpu_START (24)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_autodiv_tscpu_END (24)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_autodiv_aicpu_START (25)
#define SOC_CRGPERIPH_PERDIS6_gt_pclk_autodiv_aicpu_END (25)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_ppll2_media_START (27)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_ppll2_media_END (27)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_ppll3_media_START (28)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_ppll3_media_END (28)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_ppll7_media_START (29)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_ppll7_media_END (29)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_ppll0_media_START (30)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_ppll0_media_END (30)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_ppll2_b_media_START (31)
#define SOC_CRGPERIPH_PERDIS6_gt_clk_ppll2_b_media_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_peri_avs_mon : 1;
        unsigned int gt_clk_autofs_ddrsys_mon : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_vivo2cfgbus : 1;
        unsigned int gt_clk_autogt_dmc_peri : 1;
        unsigned int gt_pclk_media_crg : 1;
        unsigned int gt_pclk_autodiv_sysbus : 1;
        unsigned int gt_pclk_autodiv_cfgbus : 1;
        unsigned int gt_pclk_autodiv_dmabus : 1;
        unsigned int gt_clk_cfgbus_media2 : 1;
        unsigned int gt_pclk_autodiv_hsdt1bus : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_pclk_autodiv_lpmcu : 1;
        unsigned int gt_clk_cfgbus_media : 1;
        unsigned int gt_clk_atgs_mon_ddrc : 1;
        unsigned int gt_clk_autogt_ddrc : 1;
        unsigned int gt_pclk_media2_crg : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int gt_clk_ppll7_sscg : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_pclk_autodiv_tscpu : 1;
        unsigned int gt_pclk_autodiv_aicpu : 1;
        unsigned int reserved_8 : 1;
        unsigned int gt_clk_ppll2_media : 1;
        unsigned int gt_clk_ppll3_media : 1;
        unsigned int gt_clk_ppll7_media : 1;
        unsigned int gt_clk_ppll0_media : 1;
        unsigned int gt_clk_ppll2_b_media : 1;
    } reg;
} SOC_CRGPERIPH_PERCLKEN6_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_peri_avs_mon_START (0)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_peri_avs_mon_END (0)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_autofs_ddrsys_mon_START (1)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_autofs_ddrsys_mon_END (1)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_vivo2cfgbus_START (3)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_vivo2cfgbus_END (3)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_autogt_dmc_peri_START (4)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_autogt_dmc_peri_END (4)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_media_crg_START (5)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_media_crg_END (5)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_autodiv_sysbus_START (6)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_autodiv_sysbus_END (6)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_autodiv_cfgbus_START (7)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_autodiv_cfgbus_END (7)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_autodiv_dmabus_START (8)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_autodiv_dmabus_END (8)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_cfgbus_media2_START (9)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_cfgbus_media2_END (9)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_autodiv_hsdt1bus_START (10)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_autodiv_hsdt1bus_END (10)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_autodiv_lpmcu_START (12)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_autodiv_lpmcu_END (12)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_cfgbus_media_START (13)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_cfgbus_media_END (13)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_atgs_mon_ddrc_START (14)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_atgs_mon_ddrc_END (14)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_autogt_ddrc_START (15)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_autogt_ddrc_END (15)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_media2_crg_START (16)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_media2_crg_END (16)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_ppll7_sscg_START (21)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_ppll7_sscg_END (21)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_autodiv_tscpu_START (24)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_autodiv_tscpu_END (24)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_autodiv_aicpu_START (25)
#define SOC_CRGPERIPH_PERCLKEN6_gt_pclk_autodiv_aicpu_END (25)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_ppll2_media_START (27)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_ppll2_media_END (27)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_ppll3_media_START (28)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_ppll3_media_END (28)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_ppll7_media_START (29)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_ppll7_media_END (29)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_ppll0_media_START (30)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_ppll0_media_END (30)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_ppll2_b_media_START (31)
#define SOC_CRGPERIPH_PERCLKEN6_gt_clk_ppll2_b_media_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_peri_avs_mon : 1;
        unsigned int st_clk_autofs_ddrsys_mon : 1;
        unsigned int reserved_0 : 1;
        unsigned int st_clk_vivo2cfgbus : 1;
        unsigned int st_clk_autogt_dmc_peri : 1;
        unsigned int st_pclk_media_crg : 1;
        unsigned int st_pclk_autodiv_sysbus : 1;
        unsigned int st_pclk_autodiv_cfgbus : 1;
        unsigned int st_pclk_autodiv_dmabus : 1;
        unsigned int st_clk_cfgbus_media2 : 1;
        unsigned int st_pclk_autodiv_hsdt1bus : 1;
        unsigned int reserved_1 : 1;
        unsigned int st_pclk_autodiv_lpmcu : 1;
        unsigned int st_clk_cfgbus_media : 1;
        unsigned int st_clk_atgs_mon_ddrc : 1;
        unsigned int st_clk_autogt_ddrc : 1;
        unsigned int st_pclk_media2_crg : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int st_clk_ppll7_sscg : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int st_pclk_autodiv_tscpu : 1;
        unsigned int st_pclk_autodiv_aicpu : 1;
        unsigned int reserved_8 : 1;
        unsigned int st_clk_ppll2_media : 1;
        unsigned int st_clk_ppll3_media : 1;
        unsigned int st_clk_ppll7_media : 1;
        unsigned int st_clk_ppll0_media : 1;
        unsigned int st_clk_ppll2_b_media : 1;
    } reg;
} SOC_CRGPERIPH_PERSTAT6_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT6_st_clk_peri_avs_mon_START (0)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_peri_avs_mon_END (0)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_autofs_ddrsys_mon_START (1)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_autofs_ddrsys_mon_END (1)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_vivo2cfgbus_START (3)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_vivo2cfgbus_END (3)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_autogt_dmc_peri_START (4)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_autogt_dmc_peri_END (4)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_media_crg_START (5)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_media_crg_END (5)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_autodiv_sysbus_START (6)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_autodiv_sysbus_END (6)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_autodiv_cfgbus_START (7)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_autodiv_cfgbus_END (7)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_autodiv_dmabus_START (8)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_autodiv_dmabus_END (8)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_cfgbus_media2_START (9)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_cfgbus_media2_END (9)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_autodiv_hsdt1bus_START (10)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_autodiv_hsdt1bus_END (10)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_autodiv_lpmcu_START (12)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_autodiv_lpmcu_END (12)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_cfgbus_media_START (13)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_cfgbus_media_END (13)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_atgs_mon_ddrc_START (14)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_atgs_mon_ddrc_END (14)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_autogt_ddrc_START (15)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_autogt_ddrc_END (15)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_media2_crg_START (16)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_media2_crg_END (16)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_ppll7_sscg_START (21)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_ppll7_sscg_END (21)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_autodiv_tscpu_START (24)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_autodiv_tscpu_END (24)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_autodiv_aicpu_START (25)
#define SOC_CRGPERIPH_PERSTAT6_st_pclk_autodiv_aicpu_END (25)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_ppll2_media_START (27)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_ppll2_media_END (27)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_ppll3_media_START (28)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_ppll3_media_END (28)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_ppll7_media_START (29)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_ppll7_media_END (29)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_ppll0_media_START (30)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_ppll0_media_END (30)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_ppll2_b_media_START (31)
#define SOC_CRGPERIPH_PERSTAT6_st_clk_ppll2_b_media_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_ddrc_acp_rs_peri : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_noc_aicore2npu : 1;
        unsigned int gt_pclk_gpu_apb : 1;
        unsigned int gt_clk_noc_npubus_cfg : 1;
        unsigned int gt_clk_dmss_to_vcodec_rs : 1;
        unsigned int gt_clk_dmss_to_vivo_rs : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_clk_noc_npubus : 1;
        unsigned int gt_pclk_npu_crg : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_pclk_latency_monitor : 1;
        unsigned int gt_aclk_latency_monitor : 1;
        unsigned int gt_clk_fcm_pcr : 1;
        unsigned int gt_clk_npu_pcr : 1;
        unsigned int gt_clk_cpu_gic : 1;
        unsigned int gt_clk_gpu_pcr : 1;
        unsigned int gt_clk_peri_pcr : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_sysbus2npu : 1;
        unsigned int gt_pclk_peri_pcr : 1;
        unsigned int reserved_4 : 1;
        unsigned int gt_clk_fll_peri2npu : 1;
        unsigned int gt_clk_ts_cpu : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int gt_clk_npubus_pre : 1;
        unsigned int reserved_7 : 5;
    } reg;
} SOC_CRGPERIPH_PEREN7_UNION;
#endif
#define SOC_CRGPERIPH_PEREN7_gt_clk_ddrc_acp_rs_peri_START (0)
#define SOC_CRGPERIPH_PEREN7_gt_clk_ddrc_acp_rs_peri_END (0)
#define SOC_CRGPERIPH_PEREN7_gt_clk_noc_aicore2npu_START (2)
#define SOC_CRGPERIPH_PEREN7_gt_clk_noc_aicore2npu_END (2)
#define SOC_CRGPERIPH_PEREN7_gt_pclk_gpu_apb_START (3)
#define SOC_CRGPERIPH_PEREN7_gt_pclk_gpu_apb_END (3)
#define SOC_CRGPERIPH_PEREN7_gt_clk_noc_npubus_cfg_START (4)
#define SOC_CRGPERIPH_PEREN7_gt_clk_noc_npubus_cfg_END (4)
#define SOC_CRGPERIPH_PEREN7_gt_clk_dmss_to_vcodec_rs_START (5)
#define SOC_CRGPERIPH_PEREN7_gt_clk_dmss_to_vcodec_rs_END (5)
#define SOC_CRGPERIPH_PEREN7_gt_clk_dmss_to_vivo_rs_START (6)
#define SOC_CRGPERIPH_PEREN7_gt_clk_dmss_to_vivo_rs_END (6)
#define SOC_CRGPERIPH_PEREN7_gt_clk_noc_npubus_START (8)
#define SOC_CRGPERIPH_PEREN7_gt_clk_noc_npubus_END (8)
#define SOC_CRGPERIPH_PEREN7_gt_pclk_npu_crg_START (9)
#define SOC_CRGPERIPH_PEREN7_gt_pclk_npu_crg_END (9)
#define SOC_CRGPERIPH_PEREN7_gt_pclk_latency_monitor_START (11)
#define SOC_CRGPERIPH_PEREN7_gt_pclk_latency_monitor_END (11)
#define SOC_CRGPERIPH_PEREN7_gt_aclk_latency_monitor_START (12)
#define SOC_CRGPERIPH_PEREN7_gt_aclk_latency_monitor_END (12)
#define SOC_CRGPERIPH_PEREN7_gt_clk_fcm_pcr_START (13)
#define SOC_CRGPERIPH_PEREN7_gt_clk_fcm_pcr_END (13)
#define SOC_CRGPERIPH_PEREN7_gt_clk_npu_pcr_START (14)
#define SOC_CRGPERIPH_PEREN7_gt_clk_npu_pcr_END (14)
#define SOC_CRGPERIPH_PEREN7_gt_clk_cpu_gic_START (15)
#define SOC_CRGPERIPH_PEREN7_gt_clk_cpu_gic_END (15)
#define SOC_CRGPERIPH_PEREN7_gt_clk_gpu_pcr_START (16)
#define SOC_CRGPERIPH_PEREN7_gt_clk_gpu_pcr_END (16)
#define SOC_CRGPERIPH_PEREN7_gt_clk_peri_pcr_START (17)
#define SOC_CRGPERIPH_PEREN7_gt_clk_peri_pcr_END (17)
#define SOC_CRGPERIPH_PEREN7_gt_clk_sysbus2npu_START (19)
#define SOC_CRGPERIPH_PEREN7_gt_clk_sysbus2npu_END (19)
#define SOC_CRGPERIPH_PEREN7_gt_pclk_peri_pcr_START (20)
#define SOC_CRGPERIPH_PEREN7_gt_pclk_peri_pcr_END (20)
#define SOC_CRGPERIPH_PEREN7_gt_clk_fll_peri2npu_START (22)
#define SOC_CRGPERIPH_PEREN7_gt_clk_fll_peri2npu_END (22)
#define SOC_CRGPERIPH_PEREN7_gt_clk_ts_cpu_START (23)
#define SOC_CRGPERIPH_PEREN7_gt_clk_ts_cpu_END (23)
#define SOC_CRGPERIPH_PEREN7_gt_clk_npubus_pre_START (26)
#define SOC_CRGPERIPH_PEREN7_gt_clk_npubus_pre_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_ddrc_acp_rs_peri : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_noc_aicore2npu : 1;
        unsigned int gt_pclk_gpu_apb : 1;
        unsigned int gt_clk_noc_npubus_cfg : 1;
        unsigned int gt_clk_dmss_to_vcodec_rs : 1;
        unsigned int gt_clk_dmss_to_vivo_rs : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_clk_noc_npubus : 1;
        unsigned int gt_pclk_npu_crg : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_pclk_latency_monitor : 1;
        unsigned int gt_aclk_latency_monitor : 1;
        unsigned int gt_clk_fcm_pcr : 1;
        unsigned int gt_clk_npu_pcr : 1;
        unsigned int gt_clk_cpu_gic : 1;
        unsigned int gt_clk_gpu_pcr : 1;
        unsigned int gt_clk_peri_pcr : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_sysbus2npu : 1;
        unsigned int gt_pclk_peri_pcr : 1;
        unsigned int reserved_4 : 1;
        unsigned int gt_clk_fll_peri2npu : 1;
        unsigned int gt_clk_ts_cpu : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int gt_clk_npubus_pre : 1;
        unsigned int reserved_7 : 5;
    } reg;
} SOC_CRGPERIPH_PERDIS7_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS7_gt_clk_ddrc_acp_rs_peri_START (0)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_ddrc_acp_rs_peri_END (0)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_noc_aicore2npu_START (2)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_noc_aicore2npu_END (2)
#define SOC_CRGPERIPH_PERDIS7_gt_pclk_gpu_apb_START (3)
#define SOC_CRGPERIPH_PERDIS7_gt_pclk_gpu_apb_END (3)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_noc_npubus_cfg_START (4)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_noc_npubus_cfg_END (4)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_dmss_to_vcodec_rs_START (5)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_dmss_to_vcodec_rs_END (5)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_dmss_to_vivo_rs_START (6)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_dmss_to_vivo_rs_END (6)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_noc_npubus_START (8)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_noc_npubus_END (8)
#define SOC_CRGPERIPH_PERDIS7_gt_pclk_npu_crg_START (9)
#define SOC_CRGPERIPH_PERDIS7_gt_pclk_npu_crg_END (9)
#define SOC_CRGPERIPH_PERDIS7_gt_pclk_latency_monitor_START (11)
#define SOC_CRGPERIPH_PERDIS7_gt_pclk_latency_monitor_END (11)
#define SOC_CRGPERIPH_PERDIS7_gt_aclk_latency_monitor_START (12)
#define SOC_CRGPERIPH_PERDIS7_gt_aclk_latency_monitor_END (12)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_fcm_pcr_START (13)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_fcm_pcr_END (13)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_npu_pcr_START (14)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_npu_pcr_END (14)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_cpu_gic_START (15)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_cpu_gic_END (15)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_gpu_pcr_START (16)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_gpu_pcr_END (16)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_peri_pcr_START (17)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_peri_pcr_END (17)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_sysbus2npu_START (19)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_sysbus2npu_END (19)
#define SOC_CRGPERIPH_PERDIS7_gt_pclk_peri_pcr_START (20)
#define SOC_CRGPERIPH_PERDIS7_gt_pclk_peri_pcr_END (20)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_fll_peri2npu_START (22)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_fll_peri2npu_END (22)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_ts_cpu_START (23)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_ts_cpu_END (23)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_npubus_pre_START (26)
#define SOC_CRGPERIPH_PERDIS7_gt_clk_npubus_pre_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_ddrc_acp_rs_peri : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_noc_aicore2npu : 1;
        unsigned int gt_pclk_gpu_apb : 1;
        unsigned int gt_clk_noc_npubus_cfg : 1;
        unsigned int gt_clk_dmss_to_vcodec_rs : 1;
        unsigned int gt_clk_dmss_to_vivo_rs : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_clk_noc_npubus : 1;
        unsigned int gt_pclk_npu_crg : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_pclk_latency_monitor : 1;
        unsigned int gt_aclk_latency_monitor : 1;
        unsigned int gt_clk_fcm_pcr : 1;
        unsigned int gt_clk_npu_pcr : 1;
        unsigned int gt_clk_cpu_gic : 1;
        unsigned int gt_clk_gpu_pcr : 1;
        unsigned int gt_clk_peri_pcr : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_sysbus2npu : 1;
        unsigned int gt_pclk_peri_pcr : 1;
        unsigned int reserved_4 : 1;
        unsigned int gt_clk_fll_peri2npu : 1;
        unsigned int gt_clk_ts_cpu : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int gt_clk_npubus_pre : 1;
        unsigned int reserved_7 : 5;
    } reg;
} SOC_CRGPERIPH_PERCLKEN7_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_ddrc_acp_rs_peri_START (0)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_ddrc_acp_rs_peri_END (0)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_noc_aicore2npu_START (2)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_noc_aicore2npu_END (2)
#define SOC_CRGPERIPH_PERCLKEN7_gt_pclk_gpu_apb_START (3)
#define SOC_CRGPERIPH_PERCLKEN7_gt_pclk_gpu_apb_END (3)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_noc_npubus_cfg_START (4)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_noc_npubus_cfg_END (4)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_dmss_to_vcodec_rs_START (5)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_dmss_to_vcodec_rs_END (5)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_dmss_to_vivo_rs_START (6)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_dmss_to_vivo_rs_END (6)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_noc_npubus_START (8)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_noc_npubus_END (8)
#define SOC_CRGPERIPH_PERCLKEN7_gt_pclk_npu_crg_START (9)
#define SOC_CRGPERIPH_PERCLKEN7_gt_pclk_npu_crg_END (9)
#define SOC_CRGPERIPH_PERCLKEN7_gt_pclk_latency_monitor_START (11)
#define SOC_CRGPERIPH_PERCLKEN7_gt_pclk_latency_monitor_END (11)
#define SOC_CRGPERIPH_PERCLKEN7_gt_aclk_latency_monitor_START (12)
#define SOC_CRGPERIPH_PERCLKEN7_gt_aclk_latency_monitor_END (12)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_fcm_pcr_START (13)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_fcm_pcr_END (13)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_npu_pcr_START (14)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_npu_pcr_END (14)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_cpu_gic_START (15)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_cpu_gic_END (15)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_gpu_pcr_START (16)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_gpu_pcr_END (16)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_peri_pcr_START (17)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_peri_pcr_END (17)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_sysbus2npu_START (19)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_sysbus2npu_END (19)
#define SOC_CRGPERIPH_PERCLKEN7_gt_pclk_peri_pcr_START (20)
#define SOC_CRGPERIPH_PERCLKEN7_gt_pclk_peri_pcr_END (20)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_fll_peri2npu_START (22)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_fll_peri2npu_END (22)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_ts_cpu_START (23)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_ts_cpu_END (23)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_npubus_pre_START (26)
#define SOC_CRGPERIPH_PERCLKEN7_gt_clk_npubus_pre_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_ddrc_acp_rs_peri : 1;
        unsigned int reserved_0 : 1;
        unsigned int st_clk_noc_aicore2npu : 1;
        unsigned int st_pclk_gpu_apb : 1;
        unsigned int st_clk_noc_npubus_cfg : 1;
        unsigned int st_clk_dmss_to_vcodec_rs : 1;
        unsigned int st_clk_dmss_to_vivo_rs : 1;
        unsigned int reserved_1 : 1;
        unsigned int st_clk_noc_npubus : 1;
        unsigned int st_pclk_npu_crg : 1;
        unsigned int reserved_2 : 1;
        unsigned int st_pclk_latency_monitor : 1;
        unsigned int st_aclk_latency_monitor : 1;
        unsigned int st_clk_fcm_pcr : 1;
        unsigned int st_clk_npu_pcr : 1;
        unsigned int st_clk_cpu_gic : 1;
        unsigned int st_clk_gpu_pcr : 1;
        unsigned int st_clk_peri_pcr : 1;
        unsigned int reserved_3 : 1;
        unsigned int st_clk_sysbus2npu : 1;
        unsigned int st_pclk_peri_pcr : 1;
        unsigned int reserved_4 : 1;
        unsigned int st_clk_fll_peri2npu : 1;
        unsigned int st_clk_ts_cpu : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int st_clk_npubus_pre : 1;
        unsigned int reserved_7 : 5;
    } reg;
} SOC_CRGPERIPH_PERSTAT7_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT7_st_clk_ddrc_acp_rs_peri_START (0)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_ddrc_acp_rs_peri_END (0)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_noc_aicore2npu_START (2)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_noc_aicore2npu_END (2)
#define SOC_CRGPERIPH_PERSTAT7_st_pclk_gpu_apb_START (3)
#define SOC_CRGPERIPH_PERSTAT7_st_pclk_gpu_apb_END (3)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_noc_npubus_cfg_START (4)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_noc_npubus_cfg_END (4)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_dmss_to_vcodec_rs_START (5)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_dmss_to_vcodec_rs_END (5)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_dmss_to_vivo_rs_START (6)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_dmss_to_vivo_rs_END (6)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_noc_npubus_START (8)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_noc_npubus_END (8)
#define SOC_CRGPERIPH_PERSTAT7_st_pclk_npu_crg_START (9)
#define SOC_CRGPERIPH_PERSTAT7_st_pclk_npu_crg_END (9)
#define SOC_CRGPERIPH_PERSTAT7_st_pclk_latency_monitor_START (11)
#define SOC_CRGPERIPH_PERSTAT7_st_pclk_latency_monitor_END (11)
#define SOC_CRGPERIPH_PERSTAT7_st_aclk_latency_monitor_START (12)
#define SOC_CRGPERIPH_PERSTAT7_st_aclk_latency_monitor_END (12)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_fcm_pcr_START (13)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_fcm_pcr_END (13)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_npu_pcr_START (14)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_npu_pcr_END (14)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_cpu_gic_START (15)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_cpu_gic_END (15)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_gpu_pcr_START (16)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_gpu_pcr_END (16)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_peri_pcr_START (17)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_peri_pcr_END (17)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_sysbus2npu_START (19)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_sysbus2npu_END (19)
#define SOC_CRGPERIPH_PERSTAT7_st_pclk_peri_pcr_START (20)
#define SOC_CRGPERIPH_PERSTAT7_st_pclk_peri_pcr_END (20)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_fll_peri2npu_START (22)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_fll_peri2npu_END (22)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_ts_cpu_START (23)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_ts_cpu_END (23)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_npubus_pre_START (26)
#define SOC_CRGPERIPH_PERSTAT7_st_clk_npubus_pre_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_aclk_ddrphy_dfi_r : 1;
        unsigned int gt_aclk_ddrphy_dfi_l : 1;
        unsigned int gt_clk_autogt_dmc_d : 1;
        unsigned int gt_clk_autogt_dmc_c : 1;
        unsigned int gt_clk_autogt_dmc_b : 1;
        unsigned int gt_clk_autogt_dmc_a : 1;
        unsigned int gt_clk_ddrphy_pll_d : 1;
        unsigned int gt_clk_ddrphy_pll_c : 1;
        unsigned int gt_clk_ddrphy_pll_b : 1;
        unsigned int gt_clk_ddrphy_pll_a : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_hclk_dmc : 1;
        unsigned int gt_aclk_ddrphy_dfi_up : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int gt_clk_ddrc_a_sys : 1;
        unsigned int gt_clk_ddrc_b_sys : 1;
        unsigned int gt_clk_ddrc_c_sys : 1;
        unsigned int gt_clk_ddrc_d_sys : 1;
        unsigned int gt_clk_ppll2_media2 : 1;
        unsigned int gt_clk_ppll3_media2 : 1;
        unsigned int reserved_11 : 1;
        unsigned int gt_clk_ppll0_media2 : 1;
        unsigned int gt_clk_ppll2_b_media2 : 1;
    } reg;
} SOC_CRGPERIPH_PEREN8_UNION;
#endif
#define SOC_CRGPERIPH_PEREN8_gt_aclk_ddrphy_dfi_r_START (0)
#define SOC_CRGPERIPH_PEREN8_gt_aclk_ddrphy_dfi_r_END (0)
#define SOC_CRGPERIPH_PEREN8_gt_aclk_ddrphy_dfi_l_START (1)
#define SOC_CRGPERIPH_PEREN8_gt_aclk_ddrphy_dfi_l_END (1)
#define SOC_CRGPERIPH_PEREN8_gt_clk_autogt_dmc_d_START (2)
#define SOC_CRGPERIPH_PEREN8_gt_clk_autogt_dmc_d_END (2)
#define SOC_CRGPERIPH_PEREN8_gt_clk_autogt_dmc_c_START (3)
#define SOC_CRGPERIPH_PEREN8_gt_clk_autogt_dmc_c_END (3)
#define SOC_CRGPERIPH_PEREN8_gt_clk_autogt_dmc_b_START (4)
#define SOC_CRGPERIPH_PEREN8_gt_clk_autogt_dmc_b_END (4)
#define SOC_CRGPERIPH_PEREN8_gt_clk_autogt_dmc_a_START (5)
#define SOC_CRGPERIPH_PEREN8_gt_clk_autogt_dmc_a_END (5)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrphy_pll_d_START (6)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrphy_pll_d_END (6)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrphy_pll_c_START (7)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrphy_pll_c_END (7)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrphy_pll_b_START (8)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrphy_pll_b_END (8)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrphy_pll_a_START (9)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrphy_pll_a_END (9)
#define SOC_CRGPERIPH_PEREN8_gt_hclk_dmc_START (18)
#define SOC_CRGPERIPH_PEREN8_gt_hclk_dmc_END (18)
#define SOC_CRGPERIPH_PEREN8_gt_aclk_ddrphy_dfi_up_START (19)
#define SOC_CRGPERIPH_PEREN8_gt_aclk_ddrphy_dfi_up_END (19)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrc_a_sys_START (23)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrc_a_sys_END (23)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrc_b_sys_START (24)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrc_b_sys_END (24)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrc_c_sys_START (25)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrc_c_sys_END (25)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrc_d_sys_START (26)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ddrc_d_sys_END (26)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ppll2_media2_START (27)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ppll2_media2_END (27)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ppll3_media2_START (28)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ppll3_media2_END (28)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ppll0_media2_START (30)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ppll0_media2_END (30)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ppll2_b_media2_START (31)
#define SOC_CRGPERIPH_PEREN8_gt_clk_ppll2_b_media2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_aclk_ddrphy_dfi_r : 1;
        unsigned int gt_aclk_ddrphy_dfi_l : 1;
        unsigned int gt_clk_autogt_dmc_d : 1;
        unsigned int gt_clk_autogt_dmc_c : 1;
        unsigned int gt_clk_autogt_dmc_b : 1;
        unsigned int gt_clk_autogt_dmc_a : 1;
        unsigned int gt_clk_ddrphy_pll_d : 1;
        unsigned int gt_clk_ddrphy_pll_c : 1;
        unsigned int gt_clk_ddrphy_pll_b : 1;
        unsigned int gt_clk_ddrphy_pll_a : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_hclk_dmc : 1;
        unsigned int gt_aclk_ddrphy_dfi_up : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int gt_clk_ddrc_a_sys : 1;
        unsigned int gt_clk_ddrc_b_sys : 1;
        unsigned int gt_clk_ddrc_c_sys : 1;
        unsigned int gt_clk_ddrc_d_sys : 1;
        unsigned int gt_clk_ppll2_media2 : 1;
        unsigned int gt_clk_ppll3_media2 : 1;
        unsigned int reserved_11 : 1;
        unsigned int gt_clk_ppll0_media2 : 1;
        unsigned int gt_clk_ppll2_b_media2 : 1;
    } reg;
} SOC_CRGPERIPH_PERDIS8_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS8_gt_aclk_ddrphy_dfi_r_START (0)
#define SOC_CRGPERIPH_PERDIS8_gt_aclk_ddrphy_dfi_r_END (0)
#define SOC_CRGPERIPH_PERDIS8_gt_aclk_ddrphy_dfi_l_START (1)
#define SOC_CRGPERIPH_PERDIS8_gt_aclk_ddrphy_dfi_l_END (1)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_autogt_dmc_d_START (2)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_autogt_dmc_d_END (2)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_autogt_dmc_c_START (3)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_autogt_dmc_c_END (3)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_autogt_dmc_b_START (4)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_autogt_dmc_b_END (4)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_autogt_dmc_a_START (5)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_autogt_dmc_a_END (5)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrphy_pll_d_START (6)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrphy_pll_d_END (6)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrphy_pll_c_START (7)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrphy_pll_c_END (7)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrphy_pll_b_START (8)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrphy_pll_b_END (8)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrphy_pll_a_START (9)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrphy_pll_a_END (9)
#define SOC_CRGPERIPH_PERDIS8_gt_hclk_dmc_START (18)
#define SOC_CRGPERIPH_PERDIS8_gt_hclk_dmc_END (18)
#define SOC_CRGPERIPH_PERDIS8_gt_aclk_ddrphy_dfi_up_START (19)
#define SOC_CRGPERIPH_PERDIS8_gt_aclk_ddrphy_dfi_up_END (19)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrc_a_sys_START (23)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrc_a_sys_END (23)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrc_b_sys_START (24)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrc_b_sys_END (24)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrc_c_sys_START (25)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrc_c_sys_END (25)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrc_d_sys_START (26)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ddrc_d_sys_END (26)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ppll2_media2_START (27)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ppll2_media2_END (27)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ppll3_media2_START (28)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ppll3_media2_END (28)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ppll0_media2_START (30)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ppll0_media2_END (30)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ppll2_b_media2_START (31)
#define SOC_CRGPERIPH_PERDIS8_gt_clk_ppll2_b_media2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_aclk_ddrphy_dfi_r : 1;
        unsigned int gt_aclk_ddrphy_dfi_l : 1;
        unsigned int gt_clk_autogt_dmc_d : 1;
        unsigned int gt_clk_autogt_dmc_c : 1;
        unsigned int gt_clk_autogt_dmc_b : 1;
        unsigned int gt_clk_autogt_dmc_a : 1;
        unsigned int gt_clk_ddrphy_pll_d : 1;
        unsigned int gt_clk_ddrphy_pll_c : 1;
        unsigned int gt_clk_ddrphy_pll_b : 1;
        unsigned int gt_clk_ddrphy_pll_a : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_hclk_dmc : 1;
        unsigned int gt_aclk_ddrphy_dfi_up : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int gt_clk_ddrc_a_sys : 1;
        unsigned int gt_clk_ddrc_b_sys : 1;
        unsigned int gt_clk_ddrc_c_sys : 1;
        unsigned int gt_clk_ddrc_d_sys : 1;
        unsigned int gt_clk_ppll2_media2 : 1;
        unsigned int gt_clk_ppll3_media2 : 1;
        unsigned int reserved_11 : 1;
        unsigned int gt_clk_ppll0_media2 : 1;
        unsigned int gt_clk_ppll2_b_media2 : 1;
    } reg;
} SOC_CRGPERIPH_PERCLKEN8_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN8_gt_aclk_ddrphy_dfi_r_START (0)
#define SOC_CRGPERIPH_PERCLKEN8_gt_aclk_ddrphy_dfi_r_END (0)
#define SOC_CRGPERIPH_PERCLKEN8_gt_aclk_ddrphy_dfi_l_START (1)
#define SOC_CRGPERIPH_PERCLKEN8_gt_aclk_ddrphy_dfi_l_END (1)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_autogt_dmc_d_START (2)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_autogt_dmc_d_END (2)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_autogt_dmc_c_START (3)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_autogt_dmc_c_END (3)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_autogt_dmc_b_START (4)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_autogt_dmc_b_END (4)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_autogt_dmc_a_START (5)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_autogt_dmc_a_END (5)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrphy_pll_d_START (6)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrphy_pll_d_END (6)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrphy_pll_c_START (7)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrphy_pll_c_END (7)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrphy_pll_b_START (8)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrphy_pll_b_END (8)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrphy_pll_a_START (9)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrphy_pll_a_END (9)
#define SOC_CRGPERIPH_PERCLKEN8_gt_hclk_dmc_START (18)
#define SOC_CRGPERIPH_PERCLKEN8_gt_hclk_dmc_END (18)
#define SOC_CRGPERIPH_PERCLKEN8_gt_aclk_ddrphy_dfi_up_START (19)
#define SOC_CRGPERIPH_PERCLKEN8_gt_aclk_ddrphy_dfi_up_END (19)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrc_a_sys_START (23)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrc_a_sys_END (23)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrc_b_sys_START (24)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrc_b_sys_END (24)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrc_c_sys_START (25)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrc_c_sys_END (25)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrc_d_sys_START (26)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ddrc_d_sys_END (26)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ppll2_media2_START (27)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ppll2_media2_END (27)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ppll3_media2_START (28)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ppll3_media2_END (28)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ppll0_media2_START (30)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ppll0_media2_END (30)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ppll2_b_media2_START (31)
#define SOC_CRGPERIPH_PERCLKEN8_gt_clk_ppll2_b_media2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_aclk_ddrphy_dfi_r : 1;
        unsigned int st_aclk_ddrphy_dfi_l : 1;
        unsigned int st_clk_autogt_dmc_d : 1;
        unsigned int st_clk_autogt_dmc_c : 1;
        unsigned int st_clk_autogt_dmc_b : 1;
        unsigned int st_clk_autogt_dmc_a : 1;
        unsigned int st_clk_ddrphy_pll_autogt_d : 1;
        unsigned int st_clk_ddrphy_pll_autogt_c : 1;
        unsigned int st_clk_ddrphy_pll_autogt_b : 1;
        unsigned int st_clk_ddrphy_pll_autogt_a : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int st_hclk_dmc : 1;
        unsigned int st_aclk_ddrphy_dfi_up : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int st_clk_ddrc_a_sys : 1;
        unsigned int st_clk_ddrc_b_sys : 1;
        unsigned int st_clk_ddrc_c_sys : 1;
        unsigned int st_clk_ddrc_d_sys : 1;
        unsigned int st_clk_ppll2_media2 : 1;
        unsigned int st_clk_ppll3_media2 : 1;
        unsigned int reserved_11 : 1;
        unsigned int st_clk_ppll0_media2 : 1;
        unsigned int st_clk_ppll2_b_media2 : 1;
    } reg;
} SOC_CRGPERIPH_PERSTAT8_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT8_st_aclk_ddrphy_dfi_r_START (0)
#define SOC_CRGPERIPH_PERSTAT8_st_aclk_ddrphy_dfi_r_END (0)
#define SOC_CRGPERIPH_PERSTAT8_st_aclk_ddrphy_dfi_l_START (1)
#define SOC_CRGPERIPH_PERSTAT8_st_aclk_ddrphy_dfi_l_END (1)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_autogt_dmc_d_START (2)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_autogt_dmc_d_END (2)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_autogt_dmc_c_START (3)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_autogt_dmc_c_END (3)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_autogt_dmc_b_START (4)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_autogt_dmc_b_END (4)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_autogt_dmc_a_START (5)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_autogt_dmc_a_END (5)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrphy_pll_autogt_d_START (6)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrphy_pll_autogt_d_END (6)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrphy_pll_autogt_c_START (7)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrphy_pll_autogt_c_END (7)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrphy_pll_autogt_b_START (8)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrphy_pll_autogt_b_END (8)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrphy_pll_autogt_a_START (9)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrphy_pll_autogt_a_END (9)
#define SOC_CRGPERIPH_PERSTAT8_st_hclk_dmc_START (18)
#define SOC_CRGPERIPH_PERSTAT8_st_hclk_dmc_END (18)
#define SOC_CRGPERIPH_PERSTAT8_st_aclk_ddrphy_dfi_up_START (19)
#define SOC_CRGPERIPH_PERSTAT8_st_aclk_ddrphy_dfi_up_END (19)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrc_a_sys_START (23)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrc_a_sys_END (23)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrc_b_sys_START (24)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrc_b_sys_END (24)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrc_c_sys_START (25)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrc_c_sys_END (25)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrc_d_sys_START (26)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ddrc_d_sys_END (26)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ppll2_media2_START (27)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ppll2_media2_END (27)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ppll3_media2_START (28)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ppll3_media2_END (28)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ppll0_media2_START (30)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ppll0_media2_END (30)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ppll2_b_media2_START (31)
#define SOC_CRGPERIPH_PERSTAT8_st_clk_ppll2_b_media2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_p2p_m_a : 1;
        unsigned int gt_clk_dmss2dmc_mux_d : 1;
        unsigned int gt_clk_dmss2dmc_mux_c : 1;
        unsigned int gt_clk_dmss2dmc_mux_b : 1;
        unsigned int gt_clk_dmss2dmc_mux_a : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int gt_clk_uce_subsys_a : 1;
        unsigned int gt_clk_ddrc_a_ref : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int gt_clk_p2p_m_b : 1;
        unsigned int reserved_9 : 1;
        unsigned int gt_clk_ddrc_pipereg_rd_d_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_rd_c_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_rd_b_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_rd_a_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_wr_d_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_wr_c_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_wr_b_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_wr_a_abrg : 1;
        unsigned int reserved_10 : 1;
        unsigned int gt_clk_uce_subsys_b : 1;
        unsigned int gt_clk_ddrc_b_ref : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
    } reg;
} SOC_CRGPERIPH_PEREN9_UNION;
#endif
#define SOC_CRGPERIPH_PEREN9_gt_clk_p2p_m_a_START (0)
#define SOC_CRGPERIPH_PEREN9_gt_clk_p2p_m_a_END (0)
#define SOC_CRGPERIPH_PEREN9_gt_clk_dmss2dmc_mux_d_START (1)
#define SOC_CRGPERIPH_PEREN9_gt_clk_dmss2dmc_mux_d_END (1)
#define SOC_CRGPERIPH_PEREN9_gt_clk_dmss2dmc_mux_c_START (2)
#define SOC_CRGPERIPH_PEREN9_gt_clk_dmss2dmc_mux_c_END (2)
#define SOC_CRGPERIPH_PEREN9_gt_clk_dmss2dmc_mux_b_START (3)
#define SOC_CRGPERIPH_PEREN9_gt_clk_dmss2dmc_mux_b_END (3)
#define SOC_CRGPERIPH_PEREN9_gt_clk_dmss2dmc_mux_a_START (4)
#define SOC_CRGPERIPH_PEREN9_gt_clk_dmss2dmc_mux_a_END (4)
#define SOC_CRGPERIPH_PEREN9_gt_clk_uce_subsys_a_START (11)
#define SOC_CRGPERIPH_PEREN9_gt_clk_uce_subsys_a_END (11)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_a_ref_START (12)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_a_ref_END (12)
#define SOC_CRGPERIPH_PEREN9_gt_clk_p2p_m_b_START (16)
#define SOC_CRGPERIPH_PEREN9_gt_clk_p2p_m_b_END (16)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_rd_d_abrg_START (18)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_rd_d_abrg_END (18)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_rd_c_abrg_START (19)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_rd_c_abrg_END (19)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_rd_b_abrg_START (20)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_rd_b_abrg_END (20)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_rd_a_abrg_START (21)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_rd_a_abrg_END (21)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_wr_d_abrg_START (22)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_wr_d_abrg_END (22)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_wr_c_abrg_START (23)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_wr_c_abrg_END (23)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_wr_b_abrg_START (24)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_wr_b_abrg_END (24)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_wr_a_abrg_START (25)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_pipereg_wr_a_abrg_END (25)
#define SOC_CRGPERIPH_PEREN9_gt_clk_uce_subsys_b_START (27)
#define SOC_CRGPERIPH_PEREN9_gt_clk_uce_subsys_b_END (27)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_b_ref_START (28)
#define SOC_CRGPERIPH_PEREN9_gt_clk_ddrc_b_ref_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_p2p_m_a : 1;
        unsigned int gt_clk_dmss2dmc_mux_d : 1;
        unsigned int gt_clk_dmss2dmc_mux_c : 1;
        unsigned int gt_clk_dmss2dmc_mux_b : 1;
        unsigned int gt_clk_dmss2dmc_mux_a : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int gt_clk_uce_subsys_a : 1;
        unsigned int gt_clk_ddrc_a_ref : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int gt_clk_p2p_m_b : 1;
        unsigned int reserved_9 : 1;
        unsigned int gt_clk_ddrc_pipereg_rd_d_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_rd_c_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_rd_b_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_rd_a_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_wr_d_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_wr_c_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_wr_b_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_wr_a_abrg : 1;
        unsigned int reserved_10 : 1;
        unsigned int gt_clk_uce_subsys_b : 1;
        unsigned int gt_clk_ddrc_b_ref : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
    } reg;
} SOC_CRGPERIPH_PERDIS9_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS9_gt_clk_p2p_m_a_START (0)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_p2p_m_a_END (0)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_dmss2dmc_mux_d_START (1)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_dmss2dmc_mux_d_END (1)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_dmss2dmc_mux_c_START (2)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_dmss2dmc_mux_c_END (2)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_dmss2dmc_mux_b_START (3)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_dmss2dmc_mux_b_END (3)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_dmss2dmc_mux_a_START (4)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_dmss2dmc_mux_a_END (4)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_uce_subsys_a_START (11)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_uce_subsys_a_END (11)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_a_ref_START (12)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_a_ref_END (12)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_p2p_m_b_START (16)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_p2p_m_b_END (16)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_rd_d_abrg_START (18)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_rd_d_abrg_END (18)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_rd_c_abrg_START (19)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_rd_c_abrg_END (19)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_rd_b_abrg_START (20)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_rd_b_abrg_END (20)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_rd_a_abrg_START (21)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_rd_a_abrg_END (21)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_wr_d_abrg_START (22)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_wr_d_abrg_END (22)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_wr_c_abrg_START (23)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_wr_c_abrg_END (23)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_wr_b_abrg_START (24)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_wr_b_abrg_END (24)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_wr_a_abrg_START (25)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_pipereg_wr_a_abrg_END (25)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_uce_subsys_b_START (27)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_uce_subsys_b_END (27)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_b_ref_START (28)
#define SOC_CRGPERIPH_PERDIS9_gt_clk_ddrc_b_ref_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_p2p_m_a : 1;
        unsigned int gt_clk_dmss2dmc_mux_d : 1;
        unsigned int gt_clk_dmss2dmc_mux_c : 1;
        unsigned int gt_clk_dmss2dmc_mux_b : 1;
        unsigned int gt_clk_dmss2dmc_mux_a : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int gt_clk_uce_subsys_a : 1;
        unsigned int gt_clk_ddrc_a_ref : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int gt_clk_p2p_m_b : 1;
        unsigned int reserved_9 : 1;
        unsigned int gt_clk_ddrc_pipereg_rd_d_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_rd_c_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_rd_b_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_rd_a_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_wr_d_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_wr_c_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_wr_b_abrg : 1;
        unsigned int gt_clk_ddrc_pipereg_wr_a_abrg : 1;
        unsigned int reserved_10 : 1;
        unsigned int gt_clk_uce_subsys_b : 1;
        unsigned int gt_clk_ddrc_b_ref : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
    } reg;
} SOC_CRGPERIPH_PERCLKEN9_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_p2p_m_a_START (0)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_p2p_m_a_END (0)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_dmss2dmc_mux_d_START (1)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_dmss2dmc_mux_d_END (1)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_dmss2dmc_mux_c_START (2)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_dmss2dmc_mux_c_END (2)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_dmss2dmc_mux_b_START (3)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_dmss2dmc_mux_b_END (3)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_dmss2dmc_mux_a_START (4)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_dmss2dmc_mux_a_END (4)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_uce_subsys_a_START (11)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_uce_subsys_a_END (11)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_a_ref_START (12)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_a_ref_END (12)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_p2p_m_b_START (16)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_p2p_m_b_END (16)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_rd_d_abrg_START (18)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_rd_d_abrg_END (18)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_rd_c_abrg_START (19)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_rd_c_abrg_END (19)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_rd_b_abrg_START (20)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_rd_b_abrg_END (20)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_rd_a_abrg_START (21)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_rd_a_abrg_END (21)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_wr_d_abrg_START (22)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_wr_d_abrg_END (22)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_wr_c_abrg_START (23)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_wr_c_abrg_END (23)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_wr_b_abrg_START (24)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_wr_b_abrg_END (24)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_wr_a_abrg_START (25)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_pipereg_wr_a_abrg_END (25)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_uce_subsys_b_START (27)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_uce_subsys_b_END (27)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_b_ref_START (28)
#define SOC_CRGPERIPH_PERCLKEN9_gt_clk_ddrc_b_ref_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_p2p_m_a : 1;
        unsigned int st_clk_dmss2dmc_mux_d : 1;
        unsigned int st_clk_dmss2dmc_mux_c : 1;
        unsigned int st_clk_dmss2dmc_mux_b : 1;
        unsigned int st_clk_dmss2dmc_mux_a : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int st_clk_uce_subsys_a : 1;
        unsigned int st_clk_ddrc_a_ref : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int st_clk_p2p_m_b : 1;
        unsigned int reserved_9 : 1;
        unsigned int st_clk_ddrc_pipereg_rd_d_abrg : 1;
        unsigned int st_clk_ddrc_pipereg_rd_c_abrg : 1;
        unsigned int st_clk_ddrc_pipereg_rd_b_abrg : 1;
        unsigned int st_clk_ddrc_pipereg_rd_a_abrg : 1;
        unsigned int st_clk_ddrc_pipereg_wr_d_abrg : 1;
        unsigned int st_clk_ddrc_pipereg_wr_c_abrg : 1;
        unsigned int st_clk_ddrc_pipereg_wr_b_abrg : 1;
        unsigned int st_clk_ddrc_pipereg_wr_a_abrg : 1;
        unsigned int reserved_10 : 1;
        unsigned int st_clk_uce_subsys_b : 1;
        unsigned int st_clk_ddrc_b_ref : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
    } reg;
} SOC_CRGPERIPH_PERSTAT9_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT9_st_clk_p2p_m_a_START (0)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_p2p_m_a_END (0)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_dmss2dmc_mux_d_START (1)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_dmss2dmc_mux_d_END (1)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_dmss2dmc_mux_c_START (2)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_dmss2dmc_mux_c_END (2)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_dmss2dmc_mux_b_START (3)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_dmss2dmc_mux_b_END (3)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_dmss2dmc_mux_a_START (4)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_dmss2dmc_mux_a_END (4)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_uce_subsys_a_START (11)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_uce_subsys_a_END (11)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_a_ref_START (12)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_a_ref_END (12)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_p2p_m_b_START (16)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_p2p_m_b_END (16)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_rd_d_abrg_START (18)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_rd_d_abrg_END (18)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_rd_c_abrg_START (19)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_rd_c_abrg_END (19)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_rd_b_abrg_START (20)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_rd_b_abrg_END (20)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_rd_a_abrg_START (21)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_rd_a_abrg_END (21)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_wr_d_abrg_START (22)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_wr_d_abrg_END (22)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_wr_c_abrg_START (23)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_wr_c_abrg_END (23)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_wr_b_abrg_START (24)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_wr_b_abrg_END (24)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_wr_a_abrg_START (25)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_pipereg_wr_a_abrg_END (25)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_uce_subsys_b_START (27)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_uce_subsys_b_END (27)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_b_ref_START (28)
#define SOC_CRGPERIPH_PERSTAT9_st_clk_ddrc_b_ref_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_p2p_m_c : 1;
        unsigned int atgs_lp_ack_bypass_sysbus : 1;
        unsigned int atgs_lp_ack_bypass_mdm : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_clk_uce_subsys_c : 1;
        unsigned int gt_clk_ddrc_c_ref : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int gt_clk_p2p_m_d : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int gt_clk_uce_subsys_d : 1;
        unsigned int gt_clk_ddrc_d_ref : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 1;
    } reg;
} SOC_CRGPERIPH_PEREN10_UNION;
#endif
#define SOC_CRGPERIPH_PEREN10_gt_clk_p2p_m_c_START (0)
#define SOC_CRGPERIPH_PEREN10_gt_clk_p2p_m_c_END (0)
#define SOC_CRGPERIPH_PEREN10_atgs_lp_ack_bypass_sysbus_START (1)
#define SOC_CRGPERIPH_PEREN10_atgs_lp_ack_bypass_sysbus_END (1)
#define SOC_CRGPERIPH_PEREN10_atgs_lp_ack_bypass_mdm_START (2)
#define SOC_CRGPERIPH_PEREN10_atgs_lp_ack_bypass_mdm_END (2)
#define SOC_CRGPERIPH_PEREN10_gt_clk_uce_subsys_c_START (11)
#define SOC_CRGPERIPH_PEREN10_gt_clk_uce_subsys_c_END (11)
#define SOC_CRGPERIPH_PEREN10_gt_clk_ddrc_c_ref_START (12)
#define SOC_CRGPERIPH_PEREN10_gt_clk_ddrc_c_ref_END (12)
#define SOC_CRGPERIPH_PEREN10_gt_clk_p2p_m_d_START (16)
#define SOC_CRGPERIPH_PEREN10_gt_clk_p2p_m_d_END (16)
#define SOC_CRGPERIPH_PEREN10_gt_clk_uce_subsys_d_START (27)
#define SOC_CRGPERIPH_PEREN10_gt_clk_uce_subsys_d_END (27)
#define SOC_CRGPERIPH_PEREN10_gt_clk_ddrc_d_ref_START (28)
#define SOC_CRGPERIPH_PEREN10_gt_clk_ddrc_d_ref_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_p2p_m_c : 1;
        unsigned int atgs_lp_ack_bypass_sysbus : 1;
        unsigned int atgs_lp_ack_bypass_mdm : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_clk_uce_subsys_c : 1;
        unsigned int gt_clk_ddrc_c_ref : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int gt_clk_p2p_m_d : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int gt_clk_uce_subsys_d : 1;
        unsigned int gt_clk_ddrc_d_ref : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 1;
    } reg;
} SOC_CRGPERIPH_PERDIS10_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS10_gt_clk_p2p_m_c_START (0)
#define SOC_CRGPERIPH_PERDIS10_gt_clk_p2p_m_c_END (0)
#define SOC_CRGPERIPH_PERDIS10_atgs_lp_ack_bypass_sysbus_START (1)
#define SOC_CRGPERIPH_PERDIS10_atgs_lp_ack_bypass_sysbus_END (1)
#define SOC_CRGPERIPH_PERDIS10_atgs_lp_ack_bypass_mdm_START (2)
#define SOC_CRGPERIPH_PERDIS10_atgs_lp_ack_bypass_mdm_END (2)
#define SOC_CRGPERIPH_PERDIS10_gt_clk_uce_subsys_c_START (11)
#define SOC_CRGPERIPH_PERDIS10_gt_clk_uce_subsys_c_END (11)
#define SOC_CRGPERIPH_PERDIS10_gt_clk_ddrc_c_ref_START (12)
#define SOC_CRGPERIPH_PERDIS10_gt_clk_ddrc_c_ref_END (12)
#define SOC_CRGPERIPH_PERDIS10_gt_clk_p2p_m_d_START (16)
#define SOC_CRGPERIPH_PERDIS10_gt_clk_p2p_m_d_END (16)
#define SOC_CRGPERIPH_PERDIS10_gt_clk_uce_subsys_d_START (27)
#define SOC_CRGPERIPH_PERDIS10_gt_clk_uce_subsys_d_END (27)
#define SOC_CRGPERIPH_PERDIS10_gt_clk_ddrc_d_ref_START (28)
#define SOC_CRGPERIPH_PERDIS10_gt_clk_ddrc_d_ref_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_p2p_m_c : 1;
        unsigned int atgs_lp_ack_bypass_sysbus : 1;
        unsigned int atgs_lp_ack_bypass_mdm : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_clk_uce_subsys_c : 1;
        unsigned int gt_clk_ddrc_c_ref : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int gt_clk_p2p_m_d : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int gt_clk_uce_subsys_d : 1;
        unsigned int gt_clk_ddrc_d_ref : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 1;
    } reg;
} SOC_CRGPERIPH_PERCLKEN10_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN10_gt_clk_p2p_m_c_START (0)
#define SOC_CRGPERIPH_PERCLKEN10_gt_clk_p2p_m_c_END (0)
#define SOC_CRGPERIPH_PERCLKEN10_atgs_lp_ack_bypass_sysbus_START (1)
#define SOC_CRGPERIPH_PERCLKEN10_atgs_lp_ack_bypass_sysbus_END (1)
#define SOC_CRGPERIPH_PERCLKEN10_atgs_lp_ack_bypass_mdm_START (2)
#define SOC_CRGPERIPH_PERCLKEN10_atgs_lp_ack_bypass_mdm_END (2)
#define SOC_CRGPERIPH_PERCLKEN10_gt_clk_uce_subsys_c_START (11)
#define SOC_CRGPERIPH_PERCLKEN10_gt_clk_uce_subsys_c_END (11)
#define SOC_CRGPERIPH_PERCLKEN10_gt_clk_ddrc_c_ref_START (12)
#define SOC_CRGPERIPH_PERCLKEN10_gt_clk_ddrc_c_ref_END (12)
#define SOC_CRGPERIPH_PERCLKEN10_gt_clk_p2p_m_d_START (16)
#define SOC_CRGPERIPH_PERCLKEN10_gt_clk_p2p_m_d_END (16)
#define SOC_CRGPERIPH_PERCLKEN10_gt_clk_uce_subsys_d_START (27)
#define SOC_CRGPERIPH_PERCLKEN10_gt_clk_uce_subsys_d_END (27)
#define SOC_CRGPERIPH_PERCLKEN10_gt_clk_ddrc_d_ref_START (28)
#define SOC_CRGPERIPH_PERCLKEN10_gt_clk_ddrc_d_ref_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_p2p_m_c : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int st_clk_uce_subsys_c : 1;
        unsigned int st_clk_ddrc_c_ref : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int st_clk_p2p_m_d : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int st_clk_uce_subsys_d : 1;
        unsigned int st_clk_ddrc_d_ref : 1;
        unsigned int reserved_23 : 1;
        unsigned int reserved_24 : 1;
        unsigned int reserved_25 : 1;
    } reg;
} SOC_CRGPERIPH_PERSTAT10_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT10_st_clk_p2p_m_c_START (0)
#define SOC_CRGPERIPH_PERSTAT10_st_clk_p2p_m_c_END (0)
#define SOC_CRGPERIPH_PERSTAT10_st_clk_uce_subsys_c_START (11)
#define SOC_CRGPERIPH_PERSTAT10_st_clk_uce_subsys_c_END (11)
#define SOC_CRGPERIPH_PERSTAT10_st_clk_ddrc_c_ref_START (12)
#define SOC_CRGPERIPH_PERSTAT10_st_clk_ddrc_c_ref_END (12)
#define SOC_CRGPERIPH_PERSTAT10_st_clk_p2p_m_d_START (16)
#define SOC_CRGPERIPH_PERSTAT10_st_clk_p2p_m_d_END (16)
#define SOC_CRGPERIPH_PERSTAT10_st_clk_uce_subsys_d_START (27)
#define SOC_CRGPERIPH_PERSTAT10_st_clk_uce_subsys_d_END (27)
#define SOC_CRGPERIPH_PERSTAT10_st_clk_ddrc_d_ref_START (28)
#define SOC_CRGPERIPH_PERSTAT10_st_clk_ddrc_d_ref_END (28)
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
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 1;
        unsigned int reserved_24 : 1;
        unsigned int reserved_25 : 1;
        unsigned int peri_auto_bypass_a : 1;
        unsigned int peri_auto_bypass_b : 1;
        unsigned int peri_auto_bypass_c : 1;
        unsigned int peri_auto_bypass_d : 1;
        unsigned int ddrc_autogt_bypass : 1;
        unsigned int reserved_26 : 1;
    } reg;
} SOC_CRGPERIPH_PEREN11_UNION;
#endif
#define SOC_CRGPERIPH_PEREN11_peri_auto_bypass_a_START (26)
#define SOC_CRGPERIPH_PEREN11_peri_auto_bypass_a_END (26)
#define SOC_CRGPERIPH_PEREN11_peri_auto_bypass_b_START (27)
#define SOC_CRGPERIPH_PEREN11_peri_auto_bypass_b_END (27)
#define SOC_CRGPERIPH_PEREN11_peri_auto_bypass_c_START (28)
#define SOC_CRGPERIPH_PEREN11_peri_auto_bypass_c_END (28)
#define SOC_CRGPERIPH_PEREN11_peri_auto_bypass_d_START (29)
#define SOC_CRGPERIPH_PEREN11_peri_auto_bypass_d_END (29)
#define SOC_CRGPERIPH_PEREN11_ddrc_autogt_bypass_START (30)
#define SOC_CRGPERIPH_PEREN11_ddrc_autogt_bypass_END (30)
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
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 1;
        unsigned int reserved_24 : 1;
        unsigned int reserved_25 : 1;
        unsigned int peri_auto_bypass_a : 1;
        unsigned int peri_auto_bypass_b : 1;
        unsigned int peri_auto_bypass_c : 1;
        unsigned int peri_auto_bypass_d : 1;
        unsigned int ddrc_autogt_bypass : 1;
        unsigned int reserved_26 : 1;
    } reg;
} SOC_CRGPERIPH_PERDIS11_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS11_peri_auto_bypass_a_START (26)
#define SOC_CRGPERIPH_PERDIS11_peri_auto_bypass_a_END (26)
#define SOC_CRGPERIPH_PERDIS11_peri_auto_bypass_b_START (27)
#define SOC_CRGPERIPH_PERDIS11_peri_auto_bypass_b_END (27)
#define SOC_CRGPERIPH_PERDIS11_peri_auto_bypass_c_START (28)
#define SOC_CRGPERIPH_PERDIS11_peri_auto_bypass_c_END (28)
#define SOC_CRGPERIPH_PERDIS11_peri_auto_bypass_d_START (29)
#define SOC_CRGPERIPH_PERDIS11_peri_auto_bypass_d_END (29)
#define SOC_CRGPERIPH_PERDIS11_ddrc_autogt_bypass_START (30)
#define SOC_CRGPERIPH_PERDIS11_ddrc_autogt_bypass_END (30)
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
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 1;
        unsigned int reserved_24 : 1;
        unsigned int reserved_25 : 1;
        unsigned int peri_auto_bypass_a : 1;
        unsigned int peri_auto_bypass_b : 1;
        unsigned int peri_auto_bypass_c : 1;
        unsigned int peri_auto_bypass_d : 1;
        unsigned int ddrc_autogt_bypass : 1;
        unsigned int reserved_26 : 1;
    } reg;
} SOC_CRGPERIPH_PERCLKEN11_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN11_peri_auto_bypass_a_START (26)
#define SOC_CRGPERIPH_PERCLKEN11_peri_auto_bypass_a_END (26)
#define SOC_CRGPERIPH_PERCLKEN11_peri_auto_bypass_b_START (27)
#define SOC_CRGPERIPH_PERCLKEN11_peri_auto_bypass_b_END (27)
#define SOC_CRGPERIPH_PERCLKEN11_peri_auto_bypass_c_START (28)
#define SOC_CRGPERIPH_PERCLKEN11_peri_auto_bypass_c_END (28)
#define SOC_CRGPERIPH_PERCLKEN11_peri_auto_bypass_d_START (29)
#define SOC_CRGPERIPH_PERCLKEN11_peri_auto_bypass_d_END (29)
#define SOC_CRGPERIPH_PERCLKEN11_ddrc_autogt_bypass_START (30)
#define SOC_CRGPERIPH_PERCLKEN11_ddrc_autogt_bypass_END (30)
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
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int ppll2_b_gt_stat : 1;
        unsigned int ppll1_gt_stat : 1;
        unsigned int ppll2_gt_stat : 1;
        unsigned int ppll3_gt_stat : 1;
        unsigned int ppll1_en_stat : 1;
        unsigned int ppll2_en_stat : 1;
        unsigned int ppll3_en_stat : 1;
        unsigned int ppll2_b_en_stat : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 1;
    } reg;
} SOC_CRGPERIPH_PERSTAT11_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT11_ppll2_b_gt_stat_START (15)
#define SOC_CRGPERIPH_PERSTAT11_ppll2_b_gt_stat_END (15)
#define SOC_CRGPERIPH_PERSTAT11_ppll1_gt_stat_START (16)
#define SOC_CRGPERIPH_PERSTAT11_ppll1_gt_stat_END (16)
#define SOC_CRGPERIPH_PERSTAT11_ppll2_gt_stat_START (17)
#define SOC_CRGPERIPH_PERSTAT11_ppll2_gt_stat_END (17)
#define SOC_CRGPERIPH_PERSTAT11_ppll3_gt_stat_START (18)
#define SOC_CRGPERIPH_PERSTAT11_ppll3_gt_stat_END (18)
#define SOC_CRGPERIPH_PERSTAT11_ppll1_en_stat_START (19)
#define SOC_CRGPERIPH_PERSTAT11_ppll1_en_stat_END (19)
#define SOC_CRGPERIPH_PERSTAT11_ppll2_en_stat_START (20)
#define SOC_CRGPERIPH_PERSTAT11_ppll2_en_stat_END (20)
#define SOC_CRGPERIPH_PERSTAT11_ppll3_en_stat_START (21)
#define SOC_CRGPERIPH_PERSTAT11_ppll3_en_stat_END (21)
#define SOC_CRGPERIPH_PERSTAT11_ppll2_b_en_stat_START (22)
#define SOC_CRGPERIPH_PERSTAT11_ppll2_b_en_stat_END (22)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_i3c4 : 1;
        unsigned int gt_clk_sysbus2mdm5g : 1;
        unsigned int gt_clk_noc_mdm5g_cfg : 1;
        unsigned int gt_pclk_atgs_sys : 1;
        unsigned int gt_clk_mdm_spi2apb : 1;
        unsigned int gt_clk_mdm_nttp2axi : 1;
        unsigned int gt_clk_axi_mem_gs : 1;
        unsigned int gt_clk_axi_mem : 1;
        unsigned int gt_clk_mdm5g_553m : 1;
        unsigned int gt_clk_mdm4g_553m : 1;
        unsigned int gt_clk_mdm_553m : 1;
        unsigned int gt_clk_mdm5g_830m : 1;
        unsigned int gt_clk_mdm4g_830m : 1;
        unsigned int gt_clk_mdm_830m : 1;
        unsigned int gt_pclk_bba : 1;
        unsigned int gt_clk_spe_ref : 1;
        unsigned int gt_hclk_spe : 1;
        unsigned int gt_clk_spe : 1;
        unsigned int gt_aclk_axi_mem : 1;
        unsigned int gt_clk_maa_ref : 1;
        unsigned int gt_aclk_maa : 1;
        unsigned int gt_clk_dpctrl_16m : 1;
        unsigned int gt_aclk_mdm4g_slow : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 8;
    } reg;
} SOC_CRGPERIPH_PEREN12_UNION;
#endif
#define SOC_CRGPERIPH_PEREN12_gt_clk_i3c4_START (0)
#define SOC_CRGPERIPH_PEREN12_gt_clk_i3c4_END (0)
#define SOC_CRGPERIPH_PEREN12_gt_clk_sysbus2mdm5g_START (1)
#define SOC_CRGPERIPH_PEREN12_gt_clk_sysbus2mdm5g_END (1)
#define SOC_CRGPERIPH_PEREN12_gt_clk_noc_mdm5g_cfg_START (2)
#define SOC_CRGPERIPH_PEREN12_gt_clk_noc_mdm5g_cfg_END (2)
#define SOC_CRGPERIPH_PEREN12_gt_pclk_atgs_sys_START (3)
#define SOC_CRGPERIPH_PEREN12_gt_pclk_atgs_sys_END (3)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm_spi2apb_START (4)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm_spi2apb_END (4)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm_nttp2axi_START (5)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm_nttp2axi_END (5)
#define SOC_CRGPERIPH_PEREN12_gt_clk_axi_mem_gs_START (6)
#define SOC_CRGPERIPH_PEREN12_gt_clk_axi_mem_gs_END (6)
#define SOC_CRGPERIPH_PEREN12_gt_clk_axi_mem_START (7)
#define SOC_CRGPERIPH_PEREN12_gt_clk_axi_mem_END (7)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm5g_553m_START (8)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm5g_553m_END (8)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm4g_553m_START (9)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm4g_553m_END (9)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm_553m_START (10)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm_553m_END (10)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm5g_830m_START (11)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm5g_830m_END (11)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm4g_830m_START (12)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm4g_830m_END (12)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm_830m_START (13)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm_830m_END (13)
#define SOC_CRGPERIPH_PEREN12_gt_pclk_bba_START (14)
#define SOC_CRGPERIPH_PEREN12_gt_pclk_bba_END (14)
#define SOC_CRGPERIPH_PEREN12_gt_clk_spe_ref_START (15)
#define SOC_CRGPERIPH_PEREN12_gt_clk_spe_ref_END (15)
#define SOC_CRGPERIPH_PEREN12_gt_hclk_spe_START (16)
#define SOC_CRGPERIPH_PEREN12_gt_hclk_spe_END (16)
#define SOC_CRGPERIPH_PEREN12_gt_clk_spe_START (17)
#define SOC_CRGPERIPH_PEREN12_gt_clk_spe_END (17)
#define SOC_CRGPERIPH_PEREN12_gt_aclk_axi_mem_START (18)
#define SOC_CRGPERIPH_PEREN12_gt_aclk_axi_mem_END (18)
#define SOC_CRGPERIPH_PEREN12_gt_clk_maa_ref_START (19)
#define SOC_CRGPERIPH_PEREN12_gt_clk_maa_ref_END (19)
#define SOC_CRGPERIPH_PEREN12_gt_aclk_maa_START (20)
#define SOC_CRGPERIPH_PEREN12_gt_aclk_maa_END (20)
#define SOC_CRGPERIPH_PEREN12_gt_clk_dpctrl_16m_START (21)
#define SOC_CRGPERIPH_PEREN12_gt_clk_dpctrl_16m_END (21)
#define SOC_CRGPERIPH_PEREN12_gt_aclk_mdm4g_slow_START (22)
#define SOC_CRGPERIPH_PEREN12_gt_aclk_mdm4g_slow_END (22)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_i3c4 : 1;
        unsigned int gt_clk_sysbus2mdm5g : 1;
        unsigned int gt_clk_noc_mdm5g_cfg : 1;
        unsigned int gt_pclk_atgs_sys : 1;
        unsigned int gt_clk_mdm_spi2apb : 1;
        unsigned int gt_clk_mdm_nttp2axi : 1;
        unsigned int gt_clk_axi_mem_gs : 1;
        unsigned int gt_clk_axi_mem : 1;
        unsigned int gt_clk_mdm5g_553m : 1;
        unsigned int gt_clk_mdm4g_553m : 1;
        unsigned int gt_clk_mdm_553m : 1;
        unsigned int gt_clk_mdm5g_830m : 1;
        unsigned int gt_clk_mdm4g_830m : 1;
        unsigned int gt_clk_mdm_830m : 1;
        unsigned int gt_pclk_bba : 1;
        unsigned int gt_clk_spe_ref : 1;
        unsigned int gt_hclk_spe : 1;
        unsigned int gt_clk_spe : 1;
        unsigned int gt_aclk_axi_mem : 1;
        unsigned int gt_clk_maa_ref : 1;
        unsigned int gt_aclk_maa : 1;
        unsigned int gt_clk_dpctrl_16m : 1;
        unsigned int gt_aclk_mdm4g_slow : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 8;
    } reg;
} SOC_CRGPERIPH_PERDIS12_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS12_gt_clk_i3c4_START (0)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_i3c4_END (0)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_sysbus2mdm5g_START (1)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_sysbus2mdm5g_END (1)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_noc_mdm5g_cfg_START (2)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_noc_mdm5g_cfg_END (2)
#define SOC_CRGPERIPH_PERDIS12_gt_pclk_atgs_sys_START (3)
#define SOC_CRGPERIPH_PERDIS12_gt_pclk_atgs_sys_END (3)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm_spi2apb_START (4)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm_spi2apb_END (4)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm_nttp2axi_START (5)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm_nttp2axi_END (5)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_axi_mem_gs_START (6)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_axi_mem_gs_END (6)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_axi_mem_START (7)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_axi_mem_END (7)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm5g_553m_START (8)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm5g_553m_END (8)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm4g_553m_START (9)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm4g_553m_END (9)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm_553m_START (10)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm_553m_END (10)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm5g_830m_START (11)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm5g_830m_END (11)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm4g_830m_START (12)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm4g_830m_END (12)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm_830m_START (13)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm_830m_END (13)
#define SOC_CRGPERIPH_PERDIS12_gt_pclk_bba_START (14)
#define SOC_CRGPERIPH_PERDIS12_gt_pclk_bba_END (14)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_spe_ref_START (15)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_spe_ref_END (15)
#define SOC_CRGPERIPH_PERDIS12_gt_hclk_spe_START (16)
#define SOC_CRGPERIPH_PERDIS12_gt_hclk_spe_END (16)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_spe_START (17)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_spe_END (17)
#define SOC_CRGPERIPH_PERDIS12_gt_aclk_axi_mem_START (18)
#define SOC_CRGPERIPH_PERDIS12_gt_aclk_axi_mem_END (18)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_maa_ref_START (19)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_maa_ref_END (19)
#define SOC_CRGPERIPH_PERDIS12_gt_aclk_maa_START (20)
#define SOC_CRGPERIPH_PERDIS12_gt_aclk_maa_END (20)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_dpctrl_16m_START (21)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_dpctrl_16m_END (21)
#define SOC_CRGPERIPH_PERDIS12_gt_aclk_mdm4g_slow_START (22)
#define SOC_CRGPERIPH_PERDIS12_gt_aclk_mdm4g_slow_END (22)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_i3c4 : 1;
        unsigned int gt_clk_sysbus2mdm5g : 1;
        unsigned int gt_clk_noc_mdm5g_cfg : 1;
        unsigned int gt_pclk_atgs_sys : 1;
        unsigned int gt_clk_mdm_spi2apb : 1;
        unsigned int gt_clk_mdm_nttp2axi : 1;
        unsigned int gt_clk_axi_mem_gs : 1;
        unsigned int gt_clk_axi_mem : 1;
        unsigned int gt_clk_mdm5g_553m : 1;
        unsigned int gt_clk_mdm4g_553m : 1;
        unsigned int gt_clk_mdm_553m : 1;
        unsigned int gt_clk_mdm5g_830m : 1;
        unsigned int gt_clk_mdm4g_830m : 1;
        unsigned int gt_clk_mdm_830m : 1;
        unsigned int gt_pclk_bba : 1;
        unsigned int gt_clk_spe_ref : 1;
        unsigned int gt_hclk_spe : 1;
        unsigned int gt_clk_spe : 1;
        unsigned int gt_aclk_axi_mem : 1;
        unsigned int gt_clk_maa_ref : 1;
        unsigned int gt_aclk_maa : 1;
        unsigned int gt_clk_dpctrl_16m : 1;
        unsigned int gt_aclk_mdm4g_slow : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 8;
    } reg;
} SOC_CRGPERIPH_PERCLKEN12_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_i3c4_START (0)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_i3c4_END (0)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_sysbus2mdm5g_START (1)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_sysbus2mdm5g_END (1)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_noc_mdm5g_cfg_START (2)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_noc_mdm5g_cfg_END (2)
#define SOC_CRGPERIPH_PERCLKEN12_gt_pclk_atgs_sys_START (3)
#define SOC_CRGPERIPH_PERCLKEN12_gt_pclk_atgs_sys_END (3)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm_spi2apb_START (4)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm_spi2apb_END (4)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm_nttp2axi_START (5)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm_nttp2axi_END (5)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_axi_mem_gs_START (6)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_axi_mem_gs_END (6)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_axi_mem_START (7)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_axi_mem_END (7)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm5g_553m_START (8)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm5g_553m_END (8)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm4g_553m_START (9)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm4g_553m_END (9)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm_553m_START (10)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm_553m_END (10)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm5g_830m_START (11)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm5g_830m_END (11)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm4g_830m_START (12)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm4g_830m_END (12)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm_830m_START (13)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_mdm_830m_END (13)
#define SOC_CRGPERIPH_PERCLKEN12_gt_pclk_bba_START (14)
#define SOC_CRGPERIPH_PERCLKEN12_gt_pclk_bba_END (14)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_spe_ref_START (15)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_spe_ref_END (15)
#define SOC_CRGPERIPH_PERCLKEN12_gt_hclk_spe_START (16)
#define SOC_CRGPERIPH_PERCLKEN12_gt_hclk_spe_END (16)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_spe_START (17)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_spe_END (17)
#define SOC_CRGPERIPH_PERCLKEN12_gt_aclk_axi_mem_START (18)
#define SOC_CRGPERIPH_PERCLKEN12_gt_aclk_axi_mem_END (18)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_maa_ref_START (19)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_maa_ref_END (19)
#define SOC_CRGPERIPH_PERCLKEN12_gt_aclk_maa_START (20)
#define SOC_CRGPERIPH_PERCLKEN12_gt_aclk_maa_END (20)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_dpctrl_16m_START (21)
#define SOC_CRGPERIPH_PERCLKEN12_gt_clk_dpctrl_16m_END (21)
#define SOC_CRGPERIPH_PERCLKEN12_gt_aclk_mdm4g_slow_START (22)
#define SOC_CRGPERIPH_PERCLKEN12_gt_aclk_mdm4g_slow_END (22)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_i3c4 : 1;
        unsigned int st_clk_sysbus2mdm5g : 1;
        unsigned int st_clk_noc_mdm5g_cfg : 1;
        unsigned int st_pclk_atgs_sys : 1;
        unsigned int st_clk_mdm_spi2apb : 1;
        unsigned int st_clk_mdm_nttp2axi : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int st_clk_mdm5g_553m : 1;
        unsigned int st_clk_mdm4g_553m : 1;
        unsigned int st_clk_mdm_553m : 1;
        unsigned int st_clk_mdm5g_830m : 1;
        unsigned int st_clk_mdm4g_830m : 1;
        unsigned int st_clk_mdm_830m : 1;
        unsigned int st_pclk_bba : 1;
        unsigned int st_clk_spe_ref : 1;
        unsigned int st_hclk_spe : 1;
        unsigned int st_clk_spe : 1;
        unsigned int st_aclk_axi_mem : 1;
        unsigned int st_clk_maa_ref : 1;
        unsigned int st_aclk_maa : 1;
        unsigned int st_clk_dpctrl_16m : 1;
        unsigned int st_aclk_mdm4g_slow : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 8;
    } reg;
} SOC_CRGPERIPH_PERSTAT12_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT12_st_clk_i3c4_START (0)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_i3c4_END (0)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_sysbus2mdm5g_START (1)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_sysbus2mdm5g_END (1)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_noc_mdm5g_cfg_START (2)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_noc_mdm5g_cfg_END (2)
#define SOC_CRGPERIPH_PERSTAT12_st_pclk_atgs_sys_START (3)
#define SOC_CRGPERIPH_PERSTAT12_st_pclk_atgs_sys_END (3)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm_spi2apb_START (4)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm_spi2apb_END (4)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm_nttp2axi_START (5)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm_nttp2axi_END (5)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm5g_553m_START (8)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm5g_553m_END (8)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm4g_553m_START (9)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm4g_553m_END (9)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm_553m_START (10)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm_553m_END (10)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm5g_830m_START (11)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm5g_830m_END (11)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm4g_830m_START (12)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm4g_830m_END (12)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm_830m_START (13)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_mdm_830m_END (13)
#define SOC_CRGPERIPH_PERSTAT12_st_pclk_bba_START (14)
#define SOC_CRGPERIPH_PERSTAT12_st_pclk_bba_END (14)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_spe_ref_START (15)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_spe_ref_END (15)
#define SOC_CRGPERIPH_PERSTAT12_st_hclk_spe_START (16)
#define SOC_CRGPERIPH_PERSTAT12_st_hclk_spe_END (16)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_spe_START (17)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_spe_END (17)
#define SOC_CRGPERIPH_PERSTAT12_st_aclk_axi_mem_START (18)
#define SOC_CRGPERIPH_PERSTAT12_st_aclk_axi_mem_END (18)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_maa_ref_START (19)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_maa_ref_END (19)
#define SOC_CRGPERIPH_PERSTAT12_st_aclk_maa_START (20)
#define SOC_CRGPERIPH_PERSTAT12_st_aclk_maa_END (20)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_dpctrl_16m_START (21)
#define SOC_CRGPERIPH_PERSTAT12_st_clk_dpctrl_16m_END (21)
#define SOC_CRGPERIPH_PERSTAT12_st_aclk_mdm4g_slow_START (22)
#define SOC_CRGPERIPH_PERSTAT12_st_aclk_mdm4g_slow_END (22)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_process_monitor_gpu : 1;
        unsigned int gt_clk_process_monitor_mdm0 : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_process_monitor_ao : 1;
        unsigned int gt_clk_process_monitor_cpu : 1;
        unsigned int gt_clk_process_monitor_vivo : 1;
        unsigned int gt_clk_tidm_core : 1;
        unsigned int gt_clk_dmc_320m : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_lpm_cpu_big : 1;
        unsigned int gt_clk_lpm_cpu_middle : 1;
        unsigned int gt_clk_lpm_cpu_little : 1;
        unsigned int gt_clk_tidm_gpu : 1;
        unsigned int gt_clk_pa_gpu : 1;
        unsigned int reserved_4 : 16;
    } reg;
} SOC_CRGPERIPH_PEREN13_UNION;
#endif
#define SOC_CRGPERIPH_PEREN13_gt_clk_process_monitor_gpu_START (0)
#define SOC_CRGPERIPH_PEREN13_gt_clk_process_monitor_gpu_END (0)
#define SOC_CRGPERIPH_PEREN13_gt_clk_process_monitor_mdm0_START (1)
#define SOC_CRGPERIPH_PEREN13_gt_clk_process_monitor_mdm0_END (1)
#define SOC_CRGPERIPH_PEREN13_gt_clk_process_monitor_ao_START (3)
#define SOC_CRGPERIPH_PEREN13_gt_clk_process_monitor_ao_END (3)
#define SOC_CRGPERIPH_PEREN13_gt_clk_process_monitor_cpu_START (4)
#define SOC_CRGPERIPH_PEREN13_gt_clk_process_monitor_cpu_END (4)
#define SOC_CRGPERIPH_PEREN13_gt_clk_process_monitor_vivo_START (5)
#define SOC_CRGPERIPH_PEREN13_gt_clk_process_monitor_vivo_END (5)
#define SOC_CRGPERIPH_PEREN13_gt_clk_tidm_core_START (6)
#define SOC_CRGPERIPH_PEREN13_gt_clk_tidm_core_END (6)
#define SOC_CRGPERIPH_PEREN13_gt_clk_dmc_320m_START (7)
#define SOC_CRGPERIPH_PEREN13_gt_clk_dmc_320m_END (7)
#define SOC_CRGPERIPH_PEREN13_gt_clk_lpm_cpu_big_START (11)
#define SOC_CRGPERIPH_PEREN13_gt_clk_lpm_cpu_big_END (11)
#define SOC_CRGPERIPH_PEREN13_gt_clk_lpm_cpu_middle_START (12)
#define SOC_CRGPERIPH_PEREN13_gt_clk_lpm_cpu_middle_END (12)
#define SOC_CRGPERIPH_PEREN13_gt_clk_lpm_cpu_little_START (13)
#define SOC_CRGPERIPH_PEREN13_gt_clk_lpm_cpu_little_END (13)
#define SOC_CRGPERIPH_PEREN13_gt_clk_tidm_gpu_START (14)
#define SOC_CRGPERIPH_PEREN13_gt_clk_tidm_gpu_END (14)
#define SOC_CRGPERIPH_PEREN13_gt_clk_pa_gpu_START (15)
#define SOC_CRGPERIPH_PEREN13_gt_clk_pa_gpu_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_process_monitor_gpu : 1;
        unsigned int gt_clk_process_monitor_mdm0 : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_process_monitor_ao : 1;
        unsigned int gt_clk_process_monitor_cpu : 1;
        unsigned int gt_clk_process_monitor_vivo : 1;
        unsigned int gt_clk_tidm_core : 1;
        unsigned int gt_clk_dmc_320m : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_lpm_cpu_big : 1;
        unsigned int gt_clk_lpm_cpu_middle : 1;
        unsigned int gt_clk_lpm_cpu_little : 1;
        unsigned int gt_clk_tidm_gpu : 1;
        unsigned int gt_clk_pa_gpu : 1;
        unsigned int reserved_4 : 16;
    } reg;
} SOC_CRGPERIPH_PERDIS13_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS13_gt_clk_process_monitor_gpu_START (0)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_process_monitor_gpu_END (0)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_process_monitor_mdm0_START (1)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_process_monitor_mdm0_END (1)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_process_monitor_ao_START (3)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_process_monitor_ao_END (3)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_process_monitor_cpu_START (4)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_process_monitor_cpu_END (4)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_process_monitor_vivo_START (5)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_process_monitor_vivo_END (5)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_tidm_core_START (6)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_tidm_core_END (6)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_dmc_320m_START (7)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_dmc_320m_END (7)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_lpm_cpu_big_START (11)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_lpm_cpu_big_END (11)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_lpm_cpu_middle_START (12)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_lpm_cpu_middle_END (12)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_lpm_cpu_little_START (13)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_lpm_cpu_little_END (13)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_tidm_gpu_START (14)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_tidm_gpu_END (14)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_pa_gpu_START (15)
#define SOC_CRGPERIPH_PERDIS13_gt_clk_pa_gpu_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_process_monitor_gpu : 1;
        unsigned int gt_clk_process_monitor_mdm0 : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_process_monitor_ao : 1;
        unsigned int gt_clk_process_monitor_cpu : 1;
        unsigned int gt_clk_process_monitor_vivo : 1;
        unsigned int gt_clk_tidm_core : 1;
        unsigned int gt_clk_dmc_320m : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_lpm_cpu_big : 1;
        unsigned int gt_clk_lpm_cpu_middle : 1;
        unsigned int gt_clk_lpm_cpu_little : 1;
        unsigned int gt_clk_tidm_gpu : 1;
        unsigned int gt_clk_pa_gpu : 1;
        unsigned int reserved_4 : 16;
    } reg;
} SOC_CRGPERIPH_PERCLKEN13_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_process_monitor_gpu_START (0)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_process_monitor_gpu_END (0)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_process_monitor_mdm0_START (1)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_process_monitor_mdm0_END (1)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_process_monitor_ao_START (3)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_process_monitor_ao_END (3)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_process_monitor_cpu_START (4)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_process_monitor_cpu_END (4)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_process_monitor_vivo_START (5)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_process_monitor_vivo_END (5)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_tidm_core_START (6)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_tidm_core_END (6)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_dmc_320m_START (7)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_dmc_320m_END (7)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_lpm_cpu_big_START (11)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_lpm_cpu_big_END (11)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_lpm_cpu_middle_START (12)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_lpm_cpu_middle_END (12)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_lpm_cpu_little_START (13)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_lpm_cpu_little_END (13)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_tidm_gpu_START (14)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_tidm_gpu_END (14)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_pa_gpu_START (15)
#define SOC_CRGPERIPH_PERCLKEN13_gt_clk_pa_gpu_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_process_monitor_gpu : 1;
        unsigned int st_clk_process_monitor_mdm0 : 1;
        unsigned int reserved_0 : 1;
        unsigned int st_clk_process_monitor_ao : 1;
        unsigned int st_clk_process_monitor_cpu : 1;
        unsigned int st_clk_process_monitor_vivo : 1;
        unsigned int st_clk_tidm_core : 1;
        unsigned int st_clk_dmc_320m : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int st_clk_lpm_cpu_big : 1;
        unsigned int st_clk_lpm_cpu_middle : 1;
        unsigned int st_clk_lpm_cpu_little : 1;
        unsigned int st_clk_tidm_gpu : 1;
        unsigned int st_clk_pa_gpu : 1;
        unsigned int reserved_4 : 16;
    } reg;
} SOC_CRGPERIPH_PERSTAT13_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT13_st_clk_process_monitor_gpu_START (0)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_process_monitor_gpu_END (0)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_process_monitor_mdm0_START (1)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_process_monitor_mdm0_END (1)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_process_monitor_ao_START (3)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_process_monitor_ao_END (3)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_process_monitor_cpu_START (4)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_process_monitor_cpu_END (4)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_process_monitor_vivo_START (5)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_process_monitor_vivo_END (5)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_tidm_core_START (6)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_tidm_core_END (6)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_dmc_320m_START (7)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_dmc_320m_END (7)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_lpm_cpu_big_START (11)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_lpm_cpu_big_END (11)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_lpm_cpu_middle_START (12)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_lpm_cpu_middle_END (12)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_lpm_cpu_little_START (13)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_lpm_cpu_little_END (13)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_tidm_gpu_START (14)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_tidm_gpu_END (14)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_pa_gpu_START (15)
#define SOC_CRGPERIPH_PERSTAT13_st_clk_pa_gpu_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int swdone_clk_hsdt1bus_div : 1;
        unsigned int reserved_2 : 1;
        unsigned int swdone_clk_uart0_div : 1;
        unsigned int swdone_clk_uartl_div : 1;
        unsigned int swdone_clk_uarth_div : 1;
        unsigned int sw_ack_clk_pll_dmc_sw_d : 2;
        unsigned int sw_ack_clk_pll_dmc_sw_c : 2;
        unsigned int swdone_clk_noc_npubus_cfg_div : 1;
        unsigned int swdone_clk_sd_div : 1;
        unsigned int swdone_clk_ddrc_sys_div : 1;
        unsigned int swdone_clk_sysbus_sys_div : 1;
        unsigned int swdone_clk_hisee_pll_div : 1;
        unsigned int swdone_clk_ao_asp : 1;
        unsigned int swdone_clk_socp_defalte_div : 1;
        unsigned int reserved_3 : 1;
        unsigned int swdone_clk_hisee_flash_div : 1;
        unsigned int reserved_4 : 2;
        unsigned int sw_ack_clk_pll_dmc_sw_b : 2;
        unsigned int st_clk_cfgbus_media_core : 1;
        unsigned int reserved_5 : 1;
        unsigned int swdone_clk_fll_peri2npu_div : 1;
        unsigned int swdone_clk_dcdrbus_div : 1;
        unsigned int swdone_clk_core_crg_apb_div : 1;
        unsigned int swdone_clk_i3c4_div : 1;
        unsigned int swdone_clk_lpmcu_fll_div : 1;
        unsigned int swdone_clk_sysbus_fll_div : 1;
    } reg;
} SOC_CRGPERIPH_PERI_STAT4_UNION;
#endif
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_hsdt1bus_div_START (2)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_hsdt1bus_div_END (2)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_uart0_div_START (4)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_uart0_div_END (4)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_uartl_div_START (5)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_uartl_div_END (5)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_uarth_div_START (6)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_uarth_div_END (6)
#define SOC_CRGPERIPH_PERI_STAT4_sw_ack_clk_pll_dmc_sw_d_START (7)
#define SOC_CRGPERIPH_PERI_STAT4_sw_ack_clk_pll_dmc_sw_d_END (8)
#define SOC_CRGPERIPH_PERI_STAT4_sw_ack_clk_pll_dmc_sw_c_START (9)
#define SOC_CRGPERIPH_PERI_STAT4_sw_ack_clk_pll_dmc_sw_c_END (10)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_noc_npubus_cfg_div_START (11)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_noc_npubus_cfg_div_END (11)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_sd_div_START (12)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_sd_div_END (12)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_ddrc_sys_div_START (13)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_ddrc_sys_div_END (13)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_sysbus_sys_div_START (14)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_sysbus_sys_div_END (14)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_hisee_pll_div_START (15)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_hisee_pll_div_END (15)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_ao_asp_START (16)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_ao_asp_END (16)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_socp_defalte_div_START (17)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_socp_defalte_div_END (17)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_hisee_flash_div_START (19)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_hisee_flash_div_END (19)
#define SOC_CRGPERIPH_PERI_STAT4_sw_ack_clk_pll_dmc_sw_b_START (22)
#define SOC_CRGPERIPH_PERI_STAT4_sw_ack_clk_pll_dmc_sw_b_END (23)
#define SOC_CRGPERIPH_PERI_STAT4_st_clk_cfgbus_media_core_START (24)
#define SOC_CRGPERIPH_PERI_STAT4_st_clk_cfgbus_media_core_END (24)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_fll_peri2npu_div_START (26)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_fll_peri2npu_div_END (26)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_dcdrbus_div_START (27)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_dcdrbus_div_END (27)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_core_crg_apb_div_START (28)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_core_crg_apb_div_END (28)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_i3c4_div_START (29)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_i3c4_div_END (29)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_lpmcu_fll_div_START (30)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_lpmcu_fll_div_END (30)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_sysbus_fll_div_START (31)
#define SOC_CRGPERIPH_PERI_STAT4_swdone_clk_sysbus_fll_div_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int swdone_clk_latency_monitor_div : 1;
        unsigned int swdone_clk_ao_hifd_div : 1;
        unsigned int swdone_clk_hsdt1_usbdp_div : 1;
        unsigned int sw_ack_clk_tiny_core_peri_sw : 4;
        unsigned int sw_ack_clk_cpu_gic_sw : 3;
        unsigned int lpmcu_invar_sw_ack : 2;
        unsigned int sw_ack_clk_pll_dmc_sw_a : 2;
        unsigned int reserved_0 : 4;
        unsigned int sw_ack_clk_320m_mux : 2;
        unsigned int sw_ack_clk_core_crg_apb_sw : 3;
        unsigned int st_clk_ddrsys_noc : 1;
        unsigned int st_clk_ddrsys_ao : 1;
        unsigned int swdone_clk_hsdt1_usbdp_ulppll_div : 1;
        unsigned int st_clk_cpul_ppll0_div : 1;
        unsigned int st_clk_l3_ppll0_div : 1;
        unsigned int sw_ack_clk_hsdt1_usbdp_sw : 2;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
    } reg;
} SOC_CRGPERIPH_PERI_STAT5_UNION;
#endif
#define SOC_CRGPERIPH_PERI_STAT5_swdone_clk_latency_monitor_div_START (0)
#define SOC_CRGPERIPH_PERI_STAT5_swdone_clk_latency_monitor_div_END (0)
#define SOC_CRGPERIPH_PERI_STAT5_swdone_clk_ao_hifd_div_START (1)
#define SOC_CRGPERIPH_PERI_STAT5_swdone_clk_ao_hifd_div_END (1)
#define SOC_CRGPERIPH_PERI_STAT5_swdone_clk_hsdt1_usbdp_div_START (2)
#define SOC_CRGPERIPH_PERI_STAT5_swdone_clk_hsdt1_usbdp_div_END (2)
#define SOC_CRGPERIPH_PERI_STAT5_sw_ack_clk_tiny_core_peri_sw_START (3)
#define SOC_CRGPERIPH_PERI_STAT5_sw_ack_clk_tiny_core_peri_sw_END (6)
#define SOC_CRGPERIPH_PERI_STAT5_sw_ack_clk_cpu_gic_sw_START (7)
#define SOC_CRGPERIPH_PERI_STAT5_sw_ack_clk_cpu_gic_sw_END (9)
#define SOC_CRGPERIPH_PERI_STAT5_lpmcu_invar_sw_ack_START (10)
#define SOC_CRGPERIPH_PERI_STAT5_lpmcu_invar_sw_ack_END (11)
#define SOC_CRGPERIPH_PERI_STAT5_sw_ack_clk_pll_dmc_sw_a_START (12)
#define SOC_CRGPERIPH_PERI_STAT5_sw_ack_clk_pll_dmc_sw_a_END (13)
#define SOC_CRGPERIPH_PERI_STAT5_sw_ack_clk_320m_mux_START (18)
#define SOC_CRGPERIPH_PERI_STAT5_sw_ack_clk_320m_mux_END (19)
#define SOC_CRGPERIPH_PERI_STAT5_sw_ack_clk_core_crg_apb_sw_START (20)
#define SOC_CRGPERIPH_PERI_STAT5_sw_ack_clk_core_crg_apb_sw_END (22)
#define SOC_CRGPERIPH_PERI_STAT5_st_clk_ddrsys_noc_START (23)
#define SOC_CRGPERIPH_PERI_STAT5_st_clk_ddrsys_noc_END (23)
#define SOC_CRGPERIPH_PERI_STAT5_st_clk_ddrsys_ao_START (24)
#define SOC_CRGPERIPH_PERI_STAT5_st_clk_ddrsys_ao_END (24)
#define SOC_CRGPERIPH_PERI_STAT5_swdone_clk_hsdt1_usbdp_ulppll_div_START (25)
#define SOC_CRGPERIPH_PERI_STAT5_swdone_clk_hsdt1_usbdp_ulppll_div_END (25)
#define SOC_CRGPERIPH_PERI_STAT5_st_clk_cpul_ppll0_div_START (26)
#define SOC_CRGPERIPH_PERI_STAT5_st_clk_cpul_ppll0_div_END (26)
#define SOC_CRGPERIPH_PERI_STAT5_st_clk_l3_ppll0_div_START (27)
#define SOC_CRGPERIPH_PERI_STAT5_st_clk_l3_ppll0_div_END (27)
#define SOC_CRGPERIPH_PERI_STAT5_sw_ack_clk_hsdt1_usbdp_sw_START (28)
#define SOC_CRGPERIPH_PERI_STAT5_sw_ack_clk_hsdt1_usbdp_sw_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_pclk_noc_bba_asyncbrg : 1;
        unsigned int st_clk_noc_spe_asyncbrg : 1;
        unsigned int reserved_0 : 4;
        unsigned int st_pclk_uart25_bridge : 1;
        unsigned int st_pclk_uart14_bridge : 1;
        unsigned int st_pclk_spi_bridge : 1;
        unsigned int st_pclk_i2c_bridge : 1;
        unsigned int st_clk_cssys_atclk : 1;
        unsigned int reserved_1 : 1;
        unsigned int st_clk_fcm_rs : 1;
        unsigned int st_pclk_codecssi : 1;
        unsigned int reserved_2 : 4;
        unsigned int st_tclk_socp : 1;
        unsigned int st_clk_socp_deflate : 1;
        unsigned int st_clk_dmss_to_vcodec_rs_core : 1;
        unsigned int swdone_clk_tiny_core_div : 1;
        unsigned int st_clk_noc_mdm5g_cfg_rs : 1;
        unsigned int st_clk_adb_mst_npu_rs : 1;
        unsigned int reserved_3 : 4;
        unsigned int swdone_aclk_hsdt1_pcie1_div : 1;
        unsigned int st_clk_noc_system_cache_cfg : 1;
        unsigned int st_clk_time_stamp : 1;
        unsigned int reserved_4 : 1;
    } reg;
} SOC_CRGPERIPH_PERI_STAT6_UNION;
#endif
#define SOC_CRGPERIPH_PERI_STAT6_st_pclk_noc_bba_asyncbrg_START (0)
#define SOC_CRGPERIPH_PERI_STAT6_st_pclk_noc_bba_asyncbrg_END (0)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_noc_spe_asyncbrg_START (1)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_noc_spe_asyncbrg_END (1)
#define SOC_CRGPERIPH_PERI_STAT6_st_pclk_uart25_bridge_START (6)
#define SOC_CRGPERIPH_PERI_STAT6_st_pclk_uart25_bridge_END (6)
#define SOC_CRGPERIPH_PERI_STAT6_st_pclk_uart14_bridge_START (7)
#define SOC_CRGPERIPH_PERI_STAT6_st_pclk_uart14_bridge_END (7)
#define SOC_CRGPERIPH_PERI_STAT6_st_pclk_spi_bridge_START (8)
#define SOC_CRGPERIPH_PERI_STAT6_st_pclk_spi_bridge_END (8)
#define SOC_CRGPERIPH_PERI_STAT6_st_pclk_i2c_bridge_START (9)
#define SOC_CRGPERIPH_PERI_STAT6_st_pclk_i2c_bridge_END (9)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_cssys_atclk_START (10)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_cssys_atclk_END (10)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_fcm_rs_START (12)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_fcm_rs_END (12)
#define SOC_CRGPERIPH_PERI_STAT6_st_pclk_codecssi_START (13)
#define SOC_CRGPERIPH_PERI_STAT6_st_pclk_codecssi_END (13)
#define SOC_CRGPERIPH_PERI_STAT6_st_tclk_socp_START (18)
#define SOC_CRGPERIPH_PERI_STAT6_st_tclk_socp_END (18)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_socp_deflate_START (19)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_socp_deflate_END (19)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_dmss_to_vcodec_rs_core_START (20)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_dmss_to_vcodec_rs_core_END (20)
#define SOC_CRGPERIPH_PERI_STAT6_swdone_clk_tiny_core_div_START (21)
#define SOC_CRGPERIPH_PERI_STAT6_swdone_clk_tiny_core_div_END (21)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_noc_mdm5g_cfg_rs_START (22)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_noc_mdm5g_cfg_rs_END (22)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_adb_mst_npu_rs_START (23)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_adb_mst_npu_rs_END (23)
#define SOC_CRGPERIPH_PERI_STAT6_swdone_aclk_hsdt1_pcie1_div_START (28)
#define SOC_CRGPERIPH_PERI_STAT6_swdone_aclk_hsdt1_pcie1_div_END (28)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_noc_system_cache_cfg_START (29)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_noc_system_cache_cfg_END (29)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_time_stamp_START (30)
#define SOC_CRGPERIPH_PERI_STAT6_st_clk_time_stamp_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 4;
        unsigned int reserved_1 : 4;
        unsigned int reserved_2 : 4;
        unsigned int reserved_3 : 4;
        unsigned int reserved_4 : 3;
        unsigned int sw_ack_clk_lpmcu_fll_sw : 2;
        unsigned int reserved_5 : 2;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 2;
    } reg;
} SOC_CRGPERIPH_PERI_STAT7_UNION;
#endif
#define SOC_CRGPERIPH_PERI_STAT7_sw_ack_clk_lpmcu_fll_sw_START (19)
#define SOC_CRGPERIPH_PERI_STAT7_sw_ack_clk_lpmcu_fll_sw_END (20)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 1;
        unsigned int reserved_1: 1;
        unsigned int reserved_2: 1;
        unsigned int reserved_3: 1;
        unsigned int reserved_4: 1;
        unsigned int reserved_5: 1;
        unsigned int reserved_6: 1;
        unsigned int reserved_7: 1;
        unsigned int reserved_8: 1;
        unsigned int reserved_9: 1;
        unsigned int reserved_10: 1;
        unsigned int reserved_11: 1;
        unsigned int reserved_12: 20;
    } reg;
} SOC_CRGPERIPH_PERPWRACK1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_mask_sysbus_nonidle_pend : 1;
        unsigned int intr_mask_cfgbus_nonidle_pend : 1;
        unsigned int intr_mask_dmabus_nonidle_pend : 1;
        unsigned int intr_mask_hsdt1bus_nonidle_pend : 1;
        unsigned int intr_mask_hsdtbus_nonidle_pend : 1;
        unsigned int intr_mask_mdmbus_nonidle_pend : 1;
        unsigned int intr_mask_hiseebus_nonidle_pend : 1;
        unsigned int intr_mask_npubus_nonidle_pend : 1;
        unsigned int intr_mask_mdm5gbus_nonidle_pend : 1;
        unsigned int reserved : 7;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_sysbus_nonidle_pend_START (0)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_sysbus_nonidle_pend_END (0)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_cfgbus_nonidle_pend_START (1)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_cfgbus_nonidle_pend_END (1)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_dmabus_nonidle_pend_START (2)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_dmabus_nonidle_pend_END (2)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_hsdt1bus_nonidle_pend_START (3)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_hsdt1bus_nonidle_pend_END (3)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_hsdtbus_nonidle_pend_START (4)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_hsdtbus_nonidle_pend_END (4)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_mdmbus_nonidle_pend_START (5)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_mdmbus_nonidle_pend_END (5)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_hiseebus_nonidle_pend_START (6)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_hiseebus_nonidle_pend_END (6)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_npubus_nonidle_pend_START (7)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_npubus_nonidle_pend_END (7)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_mdm5gbus_nonidle_pend_START (8)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_mdm5gbus_nonidle_pend_END (8)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_bitmasken_START (16)
#define SOC_CRGPERIPH_INTR_MASK_NOCBUS_NONIDLE_PEND_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_clr_sysbus_nonidle_pend : 1;
        unsigned int intr_clr_cfgbus_nonidle_pend : 1;
        unsigned int intr_clr_dmabus_nonidle_pend : 1;
        unsigned int intr_clr_hsdt1bus_nonidle_pend : 1;
        unsigned int intr_clr_hsdtbus_nonidle_pend : 1;
        unsigned int intr_clr_mdmbus_nonidle_pend : 1;
        unsigned int intr_clr_hiseebus_nonidle_pend : 1;
        unsigned int intr_clr_npubus_nonidle_pend : 1;
        unsigned int intr_clr_mdm5gbus_nonidle_pend : 1;
        unsigned int reserved : 7;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_sysbus_nonidle_pend_START (0)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_sysbus_nonidle_pend_END (0)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_cfgbus_nonidle_pend_START (1)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_cfgbus_nonidle_pend_END (1)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_dmabus_nonidle_pend_START (2)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_dmabus_nonidle_pend_END (2)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_hsdt1bus_nonidle_pend_START (3)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_hsdt1bus_nonidle_pend_END (3)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_hsdtbus_nonidle_pend_START (4)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_hsdtbus_nonidle_pend_END (4)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_mdmbus_nonidle_pend_START (5)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_mdmbus_nonidle_pend_END (5)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_hiseebus_nonidle_pend_START (6)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_hiseebus_nonidle_pend_END (6)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_npubus_nonidle_pend_START (7)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_npubus_nonidle_pend_END (7)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_mdm5gbus_nonidle_pend_START (8)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_mdm5gbus_nonidle_pend_END (8)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_bitmasken_START (16)
#define SOC_CRGPERIPH_INTR_CLR_NOCBUS_NONIDLE_PEND_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_stat_sysbus_nonidle_pend : 1;
        unsigned int intr_stat_cfgbus_nonidle_pend : 1;
        unsigned int intr_stat_dmabus_nonidle_pend : 1;
        unsigned int intr_stat_hsdt1bus_nonidle_pend : 1;
        unsigned int intr_stat_hsdtbus_nonidle_pend : 1;
        unsigned int intr_stat_mediabus_nonidle_pend : 1;
        unsigned int intr_stat_media2bus_nonidle_pend : 1;
        unsigned int intr_stat_mdmbus_nonidle_pend : 1;
        unsigned int intr_stat_hiseebus_nonidle_pend : 1;
        unsigned int intr_stat_ao_nocbus_nonidle_pend : 1;
        unsigned int intr_stat_npubus_nonidle_pend : 1;
        unsigned int intr_stat_npuip_nonidle_pend : 1;
        unsigned int intr_stat_hsdt1ip_nonidle_pend : 1;
        unsigned int intr_stat_hsdtip_nonidle_pend : 1;
        unsigned int intr_stat_mdm5gbus_nonidle_pend : 1;
        unsigned int reserved : 17;
    } reg;
} SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_sysbus_nonidle_pend_START (0)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_sysbus_nonidle_pend_END (0)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_cfgbus_nonidle_pend_START (1)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_cfgbus_nonidle_pend_END (1)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_dmabus_nonidle_pend_START (2)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_dmabus_nonidle_pend_END (2)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_hsdt1bus_nonidle_pend_START (3)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_hsdt1bus_nonidle_pend_END (3)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_hsdtbus_nonidle_pend_START (4)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_hsdtbus_nonidle_pend_END (4)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_mediabus_nonidle_pend_START (5)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_mediabus_nonidle_pend_END (5)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_media2bus_nonidle_pend_START (6)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_media2bus_nonidle_pend_END (6)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_mdmbus_nonidle_pend_START (7)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_mdmbus_nonidle_pend_END (7)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_hiseebus_nonidle_pend_START (8)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_hiseebus_nonidle_pend_END (8)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_ao_nocbus_nonidle_pend_START (9)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_ao_nocbus_nonidle_pend_END (9)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_npubus_nonidle_pend_START (10)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_npubus_nonidle_pend_END (10)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_npuip_nonidle_pend_START (11)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_npuip_nonidle_pend_END (11)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_hsdt1ip_nonidle_pend_START (12)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_hsdt1ip_nonidle_pend_END (12)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_hsdtip_nonidle_pend_START (13)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_hsdtip_nonidle_pend_END (13)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_mdm5gbus_nonidle_pend_START (14)
#define SOC_CRGPERIPH_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_mdm5gbus_nonidle_pend_END (14)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_system_cache_l : 1;
        unsigned int gt_clk_ddrc_buf_l : 1;
        unsigned int gt_clk_dmss2dmc_mux_l : 1;
        unsigned int gt_clk_system_cache_cfg_l : 1;
        unsigned int gt_clk_ddrc_buf_cfg_l : 1;
        unsigned int gt_clk_system_cache_r : 1;
        unsigned int gt_clk_ddrc_buf_r : 1;
        unsigned int gt_clk_dmss2dmc_mux_r : 1;
        unsigned int gt_clk_system_cache_cfg_r : 1;
        unsigned int gt_clk_ddrc_buf_cfg_r : 1;
        unsigned int reserved : 22;
    } reg;
} SOC_CRGPERIPH_PEREN14_UNION;
#endif
#define SOC_CRGPERIPH_PEREN14_gt_clk_system_cache_l_START (0)
#define SOC_CRGPERIPH_PEREN14_gt_clk_system_cache_l_END (0)
#define SOC_CRGPERIPH_PEREN14_gt_clk_ddrc_buf_l_START (1)
#define SOC_CRGPERIPH_PEREN14_gt_clk_ddrc_buf_l_END (1)
#define SOC_CRGPERIPH_PEREN14_gt_clk_dmss2dmc_mux_l_START (2)
#define SOC_CRGPERIPH_PEREN14_gt_clk_dmss2dmc_mux_l_END (2)
#define SOC_CRGPERIPH_PEREN14_gt_clk_system_cache_cfg_l_START (3)
#define SOC_CRGPERIPH_PEREN14_gt_clk_system_cache_cfg_l_END (3)
#define SOC_CRGPERIPH_PEREN14_gt_clk_ddrc_buf_cfg_l_START (4)
#define SOC_CRGPERIPH_PEREN14_gt_clk_ddrc_buf_cfg_l_END (4)
#define SOC_CRGPERIPH_PEREN14_gt_clk_system_cache_r_START (5)
#define SOC_CRGPERIPH_PEREN14_gt_clk_system_cache_r_END (5)
#define SOC_CRGPERIPH_PEREN14_gt_clk_ddrc_buf_r_START (6)
#define SOC_CRGPERIPH_PEREN14_gt_clk_ddrc_buf_r_END (6)
#define SOC_CRGPERIPH_PEREN14_gt_clk_dmss2dmc_mux_r_START (7)
#define SOC_CRGPERIPH_PEREN14_gt_clk_dmss2dmc_mux_r_END (7)
#define SOC_CRGPERIPH_PEREN14_gt_clk_system_cache_cfg_r_START (8)
#define SOC_CRGPERIPH_PEREN14_gt_clk_system_cache_cfg_r_END (8)
#define SOC_CRGPERIPH_PEREN14_gt_clk_ddrc_buf_cfg_r_START (9)
#define SOC_CRGPERIPH_PEREN14_gt_clk_ddrc_buf_cfg_r_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_system_cache_l : 1;
        unsigned int gt_clk_ddrc_buf_l : 1;
        unsigned int gt_clk_dmss2dmc_mux_l : 1;
        unsigned int gt_clk_system_cache_cfg_l : 1;
        unsigned int gt_clk_ddrc_buf_cfg_l : 1;
        unsigned int gt_clk_system_cache_r : 1;
        unsigned int gt_clk_ddrc_buf_r : 1;
        unsigned int gt_clk_dmss2dmc_mux_r : 1;
        unsigned int gt_clk_system_cache_cfg_r : 1;
        unsigned int gt_clk_ddrc_buf_cfg_r : 1;
        unsigned int reserved : 22;
    } reg;
} SOC_CRGPERIPH_PERDIS14_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS14_gt_clk_system_cache_l_START (0)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_system_cache_l_END (0)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_ddrc_buf_l_START (1)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_ddrc_buf_l_END (1)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_dmss2dmc_mux_l_START (2)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_dmss2dmc_mux_l_END (2)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_system_cache_cfg_l_START (3)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_system_cache_cfg_l_END (3)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_ddrc_buf_cfg_l_START (4)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_ddrc_buf_cfg_l_END (4)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_system_cache_r_START (5)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_system_cache_r_END (5)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_ddrc_buf_r_START (6)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_ddrc_buf_r_END (6)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_dmss2dmc_mux_r_START (7)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_dmss2dmc_mux_r_END (7)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_system_cache_cfg_r_START (8)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_system_cache_cfg_r_END (8)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_ddrc_buf_cfg_r_START (9)
#define SOC_CRGPERIPH_PERDIS14_gt_clk_ddrc_buf_cfg_r_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_system_cache_l : 1;
        unsigned int gt_clk_ddrc_buf_l : 1;
        unsigned int gt_clk_dmss2dmc_mux_l : 1;
        unsigned int gt_clk_system_cache_cfg_l : 1;
        unsigned int gt_clk_ddrc_buf_cfg_l : 1;
        unsigned int gt_clk_system_cache_r : 1;
        unsigned int gt_clk_ddrc_buf_r : 1;
        unsigned int gt_clk_dmss2dmc_mux_r : 1;
        unsigned int gt_clk_system_cache_cfg_r : 1;
        unsigned int gt_clk_ddrc_buf_cfg_r : 1;
        unsigned int reserved : 22;
    } reg;
} SOC_CRGPERIPH_PERCLKEN14_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_system_cache_l_START (0)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_system_cache_l_END (0)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_ddrc_buf_l_START (1)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_ddrc_buf_l_END (1)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_dmss2dmc_mux_l_START (2)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_dmss2dmc_mux_l_END (2)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_system_cache_cfg_l_START (3)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_system_cache_cfg_l_END (3)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_ddrc_buf_cfg_l_START (4)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_ddrc_buf_cfg_l_END (4)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_system_cache_r_START (5)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_system_cache_r_END (5)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_ddrc_buf_r_START (6)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_ddrc_buf_r_END (6)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_dmss2dmc_mux_r_START (7)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_dmss2dmc_mux_r_END (7)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_system_cache_cfg_r_START (8)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_system_cache_cfg_r_END (8)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_ddrc_buf_cfg_r_START (9)
#define SOC_CRGPERIPH_PERCLKEN14_gt_clk_ddrc_buf_cfg_r_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_system_cache_l : 1;
        unsigned int st_clk_ddrc_buf_l : 1;
        unsigned int st_clk_dmss2dmc_mux_l : 1;
        unsigned int st_clk_system_cache_cfg_l : 1;
        unsigned int st_clk_ddrc_buf_cfg_l : 1;
        unsigned int st_clk_system_cache_r : 1;
        unsigned int st_clk_ddrc_buf_r : 1;
        unsigned int st_clk_dmss2dmc_mux_r : 1;
        unsigned int st_clk_system_cache_cfg_r : 1;
        unsigned int st_clk_ddrc_buf_cfg_r : 1;
        unsigned int reserved : 22;
    } reg;
} SOC_CRGPERIPH_PERSTAT14_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT14_st_clk_system_cache_l_START (0)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_system_cache_l_END (0)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_ddrc_buf_l_START (1)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_ddrc_buf_l_END (1)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_dmss2dmc_mux_l_START (2)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_dmss2dmc_mux_l_END (2)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_system_cache_cfg_l_START (3)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_system_cache_cfg_l_END (3)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_ddrc_buf_cfg_l_START (4)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_ddrc_buf_cfg_l_END (4)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_system_cache_r_START (5)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_system_cache_r_END (5)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_ddrc_buf_r_START (6)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_ddrc_buf_r_END (6)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_dmss2dmc_mux_r_START (7)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_dmss2dmc_mux_r_END (7)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_system_cache_cfg_r_START (8)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_system_cache_cfg_r_END (8)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_ddrc_buf_cfg_r_START (9)
#define SOC_CRGPERIPH_PERSTAT14_st_clk_ddrc_buf_cfg_r_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int func_global_clk_en : 32;
    } reg;
} SOC_CRGPERIPH_DFT_FUNC_GLOBAL_CLK_EN_UNION;
#endif
#define SOC_CRGPERIPH_DFT_FUNC_GLOBAL_CLK_EN_func_global_clk_en_START (0)
#define SOC_CRGPERIPH_DFT_FUNC_GLOBAL_CLK_EN_func_global_clk_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 4;
        unsigned int reserved_1 : 4;
        unsigned int sw_ack_clk_ai_core_sw : 2;
        unsigned int sw_ack_clk_fll_peri2npu_sw : 2;
        unsigned int sw_ack_clk_ts_cpu_sw : 4;
        unsigned int reserved_2 : 3;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 4;
        unsigned int sw_ack_clk_gpio_sw : 3;
        unsigned int sw_ack_clk_sysbus_fll_sw : 2;
        unsigned int swdone_clk_ddrc_fll_div : 1;
        unsigned int sw_ack_clk_ddrc_fll_sw : 2;
    } reg;
} SOC_CRGPERIPH_PERI_STAT8_UNION;
#endif
#define SOC_CRGPERIPH_PERI_STAT8_sw_ack_clk_ai_core_sw_START (8)
#define SOC_CRGPERIPH_PERI_STAT8_sw_ack_clk_ai_core_sw_END (9)
#define SOC_CRGPERIPH_PERI_STAT8_sw_ack_clk_fll_peri2npu_sw_START (10)
#define SOC_CRGPERIPH_PERI_STAT8_sw_ack_clk_fll_peri2npu_sw_END (11)
#define SOC_CRGPERIPH_PERI_STAT8_sw_ack_clk_ts_cpu_sw_START (12)
#define SOC_CRGPERIPH_PERI_STAT8_sw_ack_clk_ts_cpu_sw_END (15)
#define SOC_CRGPERIPH_PERI_STAT8_sw_ack_clk_gpio_sw_START (24)
#define SOC_CRGPERIPH_PERI_STAT8_sw_ack_clk_gpio_sw_END (26)
#define SOC_CRGPERIPH_PERI_STAT8_sw_ack_clk_sysbus_fll_sw_START (27)
#define SOC_CRGPERIPH_PERI_STAT8_sw_ack_clk_sysbus_fll_sw_END (28)
#define SOC_CRGPERIPH_PERI_STAT8_swdone_clk_ddrc_fll_div_START (29)
#define SOC_CRGPERIPH_PERI_STAT8_swdone_clk_ddrc_fll_div_END (29)
#define SOC_CRGPERIPH_PERI_STAT8_sw_ack_clk_ddrc_fll_sw_START (30)
#define SOC_CRGPERIPH_PERI_STAT8_sw_ack_clk_ddrc_fll_sw_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 4;
        unsigned int reserved_1 : 4;
        unsigned int reserved_2 : 4;
        unsigned int reserved_3 : 4;
        unsigned int swdone_clk_spe_div : 1;
        unsigned int swdone_clk_hpm_h_div : 1;
        unsigned int swdone_clk_hpm_l_div : 1;
        unsigned int swdone_clk_cssysdbg_div : 1;
        unsigned int reserved_4 : 4;
        unsigned int reserved_5 : 1;
        unsigned int sw_ack_clk_ddrc_sys_sw : 2;
        unsigned int sw_ack_clk_lpmcu_invar_fll_sw : 2;
        unsigned int swdone_clk_lpmcu_invar_fll_div : 1;
        unsigned int reserved_6 : 1;
        unsigned int swdone_pclk_bba_div : 1;
    } reg;
} SOC_CRGPERIPH_PERI_STAT9_UNION;
#endif
#define SOC_CRGPERIPH_PERI_STAT9_swdone_clk_spe_div_START (16)
#define SOC_CRGPERIPH_PERI_STAT9_swdone_clk_spe_div_END (16)
#define SOC_CRGPERIPH_PERI_STAT9_swdone_clk_hpm_h_div_START (17)
#define SOC_CRGPERIPH_PERI_STAT9_swdone_clk_hpm_h_div_END (17)
#define SOC_CRGPERIPH_PERI_STAT9_swdone_clk_hpm_l_div_START (18)
#define SOC_CRGPERIPH_PERI_STAT9_swdone_clk_hpm_l_div_END (18)
#define SOC_CRGPERIPH_PERI_STAT9_swdone_clk_cssysdbg_div_START (19)
#define SOC_CRGPERIPH_PERI_STAT9_swdone_clk_cssysdbg_div_END (19)
#define SOC_CRGPERIPH_PERI_STAT9_sw_ack_clk_ddrc_sys_sw_START (25)
#define SOC_CRGPERIPH_PERI_STAT9_sw_ack_clk_ddrc_sys_sw_END (26)
#define SOC_CRGPERIPH_PERI_STAT9_sw_ack_clk_lpmcu_invar_fll_sw_START (27)
#define SOC_CRGPERIPH_PERI_STAT9_sw_ack_clk_lpmcu_invar_fll_sw_END (28)
#define SOC_CRGPERIPH_PERI_STAT9_swdone_clk_lpmcu_invar_fll_div_START (29)
#define SOC_CRGPERIPH_PERI_STAT9_swdone_clk_lpmcu_invar_fll_div_END (29)
#define SOC_CRGPERIPH_PERI_STAT9_swdone_pclk_bba_div_START (31)
#define SOC_CRGPERIPH_PERI_STAT9_swdone_pclk_bba_div_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_hifdul_enable : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int ip_rst_noc_aicore2npu : 1;
        unsigned int ip_rst_npu_subsys_crg : 1;
        unsigned int ip_rst_npu_subsys : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 1;
        unsigned int reserved_24 : 1;
        unsigned int reserved_25 : 1;
        unsigned int reserved_26 : 1;
        unsigned int reserved_27 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTEN_EHC_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTEN_EHC_ip_rst_hifdul_enable_START (3)
#define SOC_CRGPERIPH_PERRSTEN_EHC_ip_rst_hifdul_enable_END (3)
#define SOC_CRGPERIPH_PERRSTEN_EHC_ip_rst_noc_aicore2npu_START (11)
#define SOC_CRGPERIPH_PERRSTEN_EHC_ip_rst_noc_aicore2npu_END (11)
#define SOC_CRGPERIPH_PERRSTEN_EHC_ip_rst_npu_subsys_crg_START (12)
#define SOC_CRGPERIPH_PERRSTEN_EHC_ip_rst_npu_subsys_crg_END (12)
#define SOC_CRGPERIPH_PERRSTEN_EHC_ip_rst_npu_subsys_START (13)
#define SOC_CRGPERIPH_PERRSTEN_EHC_ip_rst_npu_subsys_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_hifdul_enable : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int ip_rst_noc_aicore2npu : 1;
        unsigned int ip_rst_npu_subsys_crg : 1;
        unsigned int ip_rst_npu_subsys : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 1;
        unsigned int reserved_24 : 1;
        unsigned int reserved_25 : 1;
        unsigned int reserved_26 : 1;
        unsigned int reserved_27 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTDIS_EHC_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTDIS_EHC_ip_rst_hifdul_enable_START (3)
#define SOC_CRGPERIPH_PERRSTDIS_EHC_ip_rst_hifdul_enable_END (3)
#define SOC_CRGPERIPH_PERRSTDIS_EHC_ip_rst_noc_aicore2npu_START (11)
#define SOC_CRGPERIPH_PERRSTDIS_EHC_ip_rst_noc_aicore2npu_END (11)
#define SOC_CRGPERIPH_PERRSTDIS_EHC_ip_rst_npu_subsys_crg_START (12)
#define SOC_CRGPERIPH_PERRSTDIS_EHC_ip_rst_npu_subsys_crg_END (12)
#define SOC_CRGPERIPH_PERRSTDIS_EHC_ip_rst_npu_subsys_START (13)
#define SOC_CRGPERIPH_PERRSTDIS_EHC_ip_rst_npu_subsys_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_hifdul_enable : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int ip_rst_noc_aicore2npu : 1;
        unsigned int ip_rst_npu_subsys_crg : 1;
        unsigned int ip_rst_npu_subsys : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 1;
        unsigned int reserved_24 : 1;
        unsigned int reserved_25 : 1;
        unsigned int reserved_26 : 1;
        unsigned int reserved_27 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTSTAT_EHC_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTSTAT_EHC_ip_rst_hifdul_enable_START (3)
#define SOC_CRGPERIPH_PERRSTSTAT_EHC_ip_rst_hifdul_enable_END (3)
#define SOC_CRGPERIPH_PERRSTSTAT_EHC_ip_rst_noc_aicore2npu_START (11)
#define SOC_CRGPERIPH_PERRSTSTAT_EHC_ip_rst_noc_aicore2npu_END (11)
#define SOC_CRGPERIPH_PERRSTSTAT_EHC_ip_rst_npu_subsys_crg_START (12)
#define SOC_CRGPERIPH_PERRSTSTAT_EHC_ip_rst_npu_subsys_crg_END (12)
#define SOC_CRGPERIPH_PERRSTSTAT_EHC_ip_rst_npu_subsys_START (13)
#define SOC_CRGPERIPH_PERRSTSTAT_EHC_ip_rst_npu_subsys_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_prst_ipc_fast0 : 1;
        unsigned int ip_prst_ipc_fast1 : 1;
        unsigned int ip_prst_gpu_apb : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 1;
        unsigned int reserved_24 : 1;
        unsigned int reserved_25 : 1;
        unsigned int reserved_26 : 1;
        unsigned int reserved_27 : 1;
        unsigned int reserved_28 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTEN6_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTEN6_ip_prst_ipc_fast0_START (0)
#define SOC_CRGPERIPH_PERRSTEN6_ip_prst_ipc_fast0_END (0)
#define SOC_CRGPERIPH_PERRSTEN6_ip_prst_ipc_fast1_START (1)
#define SOC_CRGPERIPH_PERRSTEN6_ip_prst_ipc_fast1_END (1)
#define SOC_CRGPERIPH_PERRSTEN6_ip_prst_gpu_apb_START (2)
#define SOC_CRGPERIPH_PERRSTEN6_ip_prst_gpu_apb_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_prst_ipc_fast0 : 1;
        unsigned int ip_prst_ipc_fast1 : 1;
        unsigned int ip_prst_gpu_apb : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 1;
        unsigned int reserved_24 : 1;
        unsigned int reserved_25 : 1;
        unsigned int reserved_26 : 1;
        unsigned int reserved_27 : 1;
        unsigned int reserved_28 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTDIS6_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTDIS6_ip_prst_ipc_fast0_START (0)
#define SOC_CRGPERIPH_PERRSTDIS6_ip_prst_ipc_fast0_END (0)
#define SOC_CRGPERIPH_PERRSTDIS6_ip_prst_ipc_fast1_START (1)
#define SOC_CRGPERIPH_PERRSTDIS6_ip_prst_ipc_fast1_END (1)
#define SOC_CRGPERIPH_PERRSTDIS6_ip_prst_gpu_apb_START (2)
#define SOC_CRGPERIPH_PERRSTDIS6_ip_prst_gpu_apb_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_prst_ipc_fast0 : 1;
        unsigned int ip_prst_ipc_fast1 : 1;
        unsigned int ip_prst_gpu_apb : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 1;
        unsigned int reserved_24 : 1;
        unsigned int reserved_25 : 1;
        unsigned int reserved_26 : 1;
        unsigned int reserved_27 : 1;
        unsigned int reserved_28 : 1;
    } reg;
} SOC_CRGPERIPH_PERRSTSTAT6_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTSTAT6_ip_prst_ipc_fast0_START (0)
#define SOC_CRGPERIPH_PERRSTSTAT6_ip_prst_ipc_fast0_END (0)
#define SOC_CRGPERIPH_PERRSTSTAT6_ip_prst_ipc_fast1_START (1)
#define SOC_CRGPERIPH_PERRSTSTAT6_ip_prst_ipc_fast1_END (1)
#define SOC_CRGPERIPH_PERRSTSTAT6_ip_prst_gpu_apb_START (2)
#define SOC_CRGPERIPH_PERRSTSTAT6_ip_prst_gpu_apb_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_ppll2_media_iomcu : 1;
        unsigned int gt_clk_ppll3_media_iomcu : 1;
        unsigned int gt_clk_ppll7_media_iomcu : 1;
        unsigned int gt_clk_ppll0_media_iomcu : 1;
        unsigned int gt_clk_ppll2_b_media_iomcu : 1;
        unsigned int gt_clk_ppll2_media2_iomcu : 1;
        unsigned int gt_clk_ppll3_media2_iomcu : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_ppll0_media2_iomcu : 1;
        unsigned int gt_clk_ppll2_b_media2_iomcu : 1;
        unsigned int gt_clk_txdphy0_cfg_iomcu : 1;
        unsigned int gt_clk_txdphy0_ref_iomcu : 1;
        unsigned int gt_clk_txdphy1_cfg_iomcu : 1;
        unsigned int gt_clk_txdphy1_ref_iomcu : 1;
        unsigned int gt_clk_cfgbus_media_iomcu : 1;
        unsigned int reserved_1 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_UNION;
#endif
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll2_media_iomcu_START (0)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll2_media_iomcu_END (0)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll3_media_iomcu_START (1)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll3_media_iomcu_END (1)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll7_media_iomcu_START (2)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll7_media_iomcu_END (2)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll0_media_iomcu_START (3)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll0_media_iomcu_END (3)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll2_b_media_iomcu_START (4)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll2_b_media_iomcu_END (4)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll2_media2_iomcu_START (5)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll2_media2_iomcu_END (5)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll3_media2_iomcu_START (6)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll3_media2_iomcu_END (6)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll0_media2_iomcu_START (8)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll0_media2_iomcu_END (8)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll2_b_media2_iomcu_START (9)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_ppll2_b_media2_iomcu_END (9)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_txdphy0_cfg_iomcu_START (10)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_txdphy0_cfg_iomcu_END (10)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_txdphy0_ref_iomcu_START (11)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_txdphy0_ref_iomcu_END (11)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_txdphy1_cfg_iomcu_START (12)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_txdphy1_cfg_iomcu_END (12)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_txdphy1_ref_iomcu_START (13)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_txdphy1_ref_iomcu_END (13)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_cfgbus_media_iomcu_START (14)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_gt_clk_cfgbus_media_iomcu_END (14)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_bitmasken_START (16)
#define SOC_CRGPERIPH_AOD_DSS_PERI_CLK_GT_VOTE_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_atgc : 4;
        unsigned int sel_ddrcsys : 1;
        unsigned int sel_ts_cpu_pll : 4;
        unsigned int div_socp_deflate : 5;
        unsigned int div_dpctrl_16m : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV26_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV26_div_atgc_START (0)
#define SOC_CRGPERIPH_CLKDIV26_div_atgc_END (3)
#define SOC_CRGPERIPH_CLKDIV26_sel_ddrcsys_START (4)
#define SOC_CRGPERIPH_CLKDIV26_sel_ddrcsys_END (4)
#define SOC_CRGPERIPH_CLKDIV26_sel_ts_cpu_pll_START (5)
#define SOC_CRGPERIPH_CLKDIV26_sel_ts_cpu_pll_END (8)
#define SOC_CRGPERIPH_CLKDIV26_div_socp_deflate_START (9)
#define SOC_CRGPERIPH_CLKDIV26_div_socp_deflate_END (13)
#define SOC_CRGPERIPH_CLKDIV26_div_dpctrl_16m_START (14)
#define SOC_CRGPERIPH_CLKDIV26_div_dpctrl_16m_END (15)
#define SOC_CRGPERIPH_CLKDIV26_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV26_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sel_fll_peri2npu : 1;
        unsigned int div_fll_peri2npu : 2;
        unsigned int sc_gt_clk_fll_peri2npu : 1;
        unsigned int reserved : 12;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV27_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV27_sel_fll_peri2npu_START (0)
#define SOC_CRGPERIPH_CLKDIV27_sel_fll_peri2npu_END (0)
#define SOC_CRGPERIPH_CLKDIV27_div_fll_peri2npu_START (1)
#define SOC_CRGPERIPH_CLKDIV27_div_fll_peri2npu_END (2)
#define SOC_CRGPERIPH_CLKDIV27_sc_gt_clk_fll_peri2npu_START (3)
#define SOC_CRGPERIPH_CLKDIV27_sc_gt_clk_fll_peri2npu_END (3)
#define SOC_CRGPERIPH_CLKDIV27_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV27_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_ts_cpu : 4;
        unsigned int div_lpmcu_invar_fll : 4;
        unsigned int div_core_crg_apb : 4;
        unsigned int reserved : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV28_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV28_div_ts_cpu_START (0)
#define SOC_CRGPERIPH_CLKDIV28_div_ts_cpu_END (3)
#define SOC_CRGPERIPH_CLKDIV28_div_lpmcu_invar_fll_START (4)
#define SOC_CRGPERIPH_CLKDIV28_div_lpmcu_invar_fll_END (7)
#define SOC_CRGPERIPH_CLKDIV28_div_core_crg_apb_START (8)
#define SOC_CRGPERIPH_CLKDIV28_div_core_crg_apb_END (11)
#define SOC_CRGPERIPH_CLKDIV28_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV28_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int sc_gt_clk_hsdt1_usbdp : 1;
        unsigned int sc_gt_clk_hpm_h : 1;
        unsigned int sc_gt_clk_lpmcu_fll : 1;
        unsigned int sc_gt_clk_i3c4 : 1;
        unsigned int sc_gt_clk_sysbus_fll : 1;
        unsigned int sc_gt_clk_ddrc_fll : 1;
        unsigned int sc_gt_clk_hpm_l : 1;
        unsigned int sc_gt_clk_spe : 1;
        unsigned int sc_gt_pclk_bba : 1;
        unsigned int sc_gt_clk_mdm_830m : 1;
        unsigned int sc_gt_clk_hsdt1_usbdp_ulppll : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV29_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_hsdt1_usbdp_START (2)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_hsdt1_usbdp_END (2)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_hpm_h_START (3)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_hpm_h_END (3)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_lpmcu_fll_START (4)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_lpmcu_fll_END (4)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_i3c4_START (5)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_i3c4_END (5)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_sysbus_fll_START (6)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_sysbus_fll_END (6)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_ddrc_fll_START (7)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_ddrc_fll_END (7)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_hpm_l_START (8)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_hpm_l_END (8)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_spe_START (9)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_spe_END (9)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_pclk_bba_START (10)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_pclk_bba_END (10)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_mdm_830m_START (11)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_mdm_830m_END (11)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_hsdt1_usbdp_ulppll_START (12)
#define SOC_CRGPERIPH_CLKDIV29_sc_gt_clk_hsdt1_usbdp_ulppll_END (12)
#define SOC_CRGPERIPH_CLKDIV29_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV29_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_usbdp : 4;
        unsigned int div_spe : 4;
        unsigned int div_usbdp_ulppll : 2;
        unsigned int div_i3c4 : 2;
        unsigned int div_latency_monitor : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV30_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV30_div_usbdp_START (0)
#define SOC_CRGPERIPH_CLKDIV30_div_usbdp_END (3)
#define SOC_CRGPERIPH_CLKDIV30_div_spe_START (4)
#define SOC_CRGPERIPH_CLKDIV30_div_spe_END (7)
#define SOC_CRGPERIPH_CLKDIV30_div_usbdp_ulppll_START (8)
#define SOC_CRGPERIPH_CLKDIV30_div_usbdp_ulppll_END (9)
#define SOC_CRGPERIPH_CLKDIV30_div_i3c4_START (10)
#define SOC_CRGPERIPH_CLKDIV30_div_i3c4_END (11)
#define SOC_CRGPERIPH_CLKDIV30_div_latency_monitor_START (12)
#define SOC_CRGPERIPH_CLKDIV30_div_latency_monitor_END (15)
#define SOC_CRGPERIPH_CLKDIV30_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV30_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_lpmcu_fll : 4;
        unsigned int div_sysbus_fll : 4;
        unsigned int div_ddrc_fll : 4;
        unsigned int div_hpm_h : 2;
        unsigned int div_hpm_l : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV31_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV31_div_lpmcu_fll_START (0)
#define SOC_CRGPERIPH_CLKDIV31_div_lpmcu_fll_END (3)
#define SOC_CRGPERIPH_CLKDIV31_div_sysbus_fll_START (4)
#define SOC_CRGPERIPH_CLKDIV31_div_sysbus_fll_END (7)
#define SOC_CRGPERIPH_CLKDIV31_div_ddrc_fll_START (8)
#define SOC_CRGPERIPH_CLKDIV31_div_ddrc_fll_END (11)
#define SOC_CRGPERIPH_CLKDIV31_div_hpm_h_START (12)
#define SOC_CRGPERIPH_CLKDIV31_div_hpm_h_END (13)
#define SOC_CRGPERIPH_CLKDIV31_div_hpm_l_START (14)
#define SOC_CRGPERIPH_CLKDIV31_div_hpm_l_END (15)
#define SOC_CRGPERIPH_CLKDIV31_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV31_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 2;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 5;
        unsigned int sel_ai_core_sw : 1;
        unsigned int autogt_clk_ddrphy_pll_bypass : 1;
        unsigned int autogt_clk_ddrphy_bypass : 1;
        unsigned int sel_ddrc_from_32kpll : 1;
        unsigned int sel_ddrc_cache_div1 : 1;
        unsigned int sel_ddrc_occ_cgdiv_pipe : 1;
        unsigned int reserved_3 : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV32_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV32_sel_ai_core_sw_START (8)
#define SOC_CRGPERIPH_CLKDIV32_sel_ai_core_sw_END (8)
#define SOC_CRGPERIPH_CLKDIV32_autogt_clk_ddrphy_pll_bypass_START (9)
#define SOC_CRGPERIPH_CLKDIV32_autogt_clk_ddrphy_pll_bypass_END (9)
#define SOC_CRGPERIPH_CLKDIV32_autogt_clk_ddrphy_bypass_START (10)
#define SOC_CRGPERIPH_CLKDIV32_autogt_clk_ddrphy_bypass_END (10)
#define SOC_CRGPERIPH_CLKDIV32_sel_ddrc_from_32kpll_START (11)
#define SOC_CRGPERIPH_CLKDIV32_sel_ddrc_from_32kpll_END (11)
#define SOC_CRGPERIPH_CLKDIV32_sel_ddrc_cache_div1_START (12)
#define SOC_CRGPERIPH_CLKDIV32_sel_ddrc_cache_div1_END (12)
#define SOC_CRGPERIPH_CLKDIV32_sel_ddrc_occ_cgdiv_pipe_START (13)
#define SOC_CRGPERIPH_CLKDIV32_sel_ddrc_occ_cgdiv_pipe_END (13)
#define SOC_CRGPERIPH_CLKDIV32_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV32_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int dvfs_gate_ctrl_cpul_bypass : 1;
        unsigned int dvfs_gate_ctrl_g3d_bypass : 1;
        unsigned int ics_dbat_drop_freq_down_bp : 1;
        unsigned int ics_dbat_drop_freq_down_div : 5;
        unsigned int dvfs_gate_ctrl_l3_bypass : 1;
        unsigned int icfg_ckg_bypass_syscache : 1;
        unsigned int dvfs_gate_ctrl_g3d_top_bypass : 1;
        unsigned int reserved_2 : 3;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PERI_COMMON_CTRL0_UNION;
#endif
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_dvfs_gate_ctrl_cpul_bypass_START (2)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_dvfs_gate_ctrl_cpul_bypass_END (2)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_dvfs_gate_ctrl_g3d_bypass_START (3)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_dvfs_gate_ctrl_g3d_bypass_END (3)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_ics_dbat_drop_freq_down_bp_START (4)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_ics_dbat_drop_freq_down_bp_END (4)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_ics_dbat_drop_freq_down_div_START (5)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_ics_dbat_drop_freq_down_div_END (9)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_dvfs_gate_ctrl_l3_bypass_START (10)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_dvfs_gate_ctrl_l3_bypass_END (10)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_icfg_ckg_bypass_syscache_START (11)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_icfg_ckg_bypass_syscache_END (11)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_dvfs_gate_ctrl_g3d_top_bypass_START (12)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_dvfs_gate_ctrl_g3d_top_bypass_END (12)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_bitmasken_START (16)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ddrc_autogt_bypass_lpmcu : 1;
        unsigned int ddrc_autogt_bypass_acpu_l_0 : 1;
        unsigned int ddrc_autogt_bypass_acpu_l_1 : 1;
        unsigned int ddrc_autogt_bypass_acpu_l_2 : 1;
        unsigned int ddrc_autogt_bypass_acpu_l_3 : 1;
        unsigned int ddrc_autogt_bypass_acpu_b_0 : 1;
        unsigned int ddrc_autogt_bypass_acpu_b_1 : 1;
        unsigned int ddrc_autogt_bypass_acpu_b_2 : 1;
        unsigned int ddrc_autogt_bypass_acpu_b_3 : 1;
        unsigned int ddrc_autogt_bypass_ccpu : 1;
        unsigned int ddrc_autogt_bypass_mdm_bbe0 : 1;
        unsigned int ddrc_autogt_bypass_mdm_bbe1 : 1;
        unsigned int ddrc_autogt_bypass_mdm_bbe2 : 1;
        unsigned int ddrc_autogt_bypass_other_cpu0 : 1;
        unsigned int ddrc_autogt_bypass_other_cpu1 : 1;
        unsigned int ddrc_autogt_bypass_other_cpu2 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PERI_COMMON_CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_lpmcu_START (0)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_lpmcu_END (0)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_l_0_START (1)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_l_0_END (1)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_l_1_START (2)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_l_1_END (2)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_l_2_START (3)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_l_2_END (3)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_l_3_START (4)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_l_3_END (4)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_b_0_START (5)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_b_0_END (5)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_b_1_START (6)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_b_1_END (6)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_b_2_START (7)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_b_2_END (7)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_b_3_START (8)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_acpu_b_3_END (8)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_ccpu_START (9)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_ccpu_END (9)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_mdm_bbe0_START (10)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_mdm_bbe0_END (10)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_mdm_bbe1_START (11)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_mdm_bbe1_END (11)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_mdm_bbe2_START (12)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_mdm_bbe2_END (12)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_other_cpu0_START (13)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_other_cpu0_END (13)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_other_cpu1_START (14)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_other_cpu1_END (14)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_other_cpu2_START (15)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_ddrc_autogt_bypass_other_cpu2_END (15)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_bitmasken_START (16)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL1_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ddrsys_autogt_bypass_lpmcu : 1;
        unsigned int ddrsys_autogt_bypass_acpu_l_0 : 1;
        unsigned int ddrsys_autogt_bypass_acpu_l_1 : 1;
        unsigned int ddrsys_autogt_bypass_acpu_l_2 : 1;
        unsigned int ddrsys_autogt_bypass_acpu_l_3 : 1;
        unsigned int ddrsys_autogt_bypass_acpu_b_0 : 1;
        unsigned int ddrsys_autogt_bypass_acpu_b_1 : 1;
        unsigned int ddrsys_autogt_bypass_acpu_b_2 : 1;
        unsigned int ddrsys_autogt_bypass_acpu_b_3 : 1;
        unsigned int ddrsys_autogt_bypass_ccpu : 1;
        unsigned int ddrsys_autogt_bypass_mdm_bbe0 : 1;
        unsigned int ddrsys_autogt_bypass_mdm_bbe1 : 1;
        unsigned int ddrsys_autogt_bypass_mdm_bbe2 : 1;
        unsigned int ddrsys_autogt_bypass_other_cpu0 : 1;
        unsigned int ddrsys_autogt_bypass_other_cpu1 : 1;
        unsigned int ddrsys_autogt_bypass_other_cpu2 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PERI_COMMON_CTRL2_UNION;
#endif
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_lpmcu_START (0)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_lpmcu_END (0)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_l_0_START (1)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_l_0_END (1)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_l_1_START (2)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_l_1_END (2)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_l_2_START (3)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_l_2_END (3)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_l_3_START (4)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_l_3_END (4)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_b_0_START (5)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_b_0_END (5)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_b_1_START (6)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_b_1_END (6)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_b_2_START (7)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_b_2_END (7)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_b_3_START (8)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_acpu_b_3_END (8)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_ccpu_START (9)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_ccpu_END (9)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_mdm_bbe0_START (10)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_mdm_bbe0_END (10)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_mdm_bbe1_START (11)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_mdm_bbe1_END (11)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_mdm_bbe2_START (12)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_mdm_bbe2_END (12)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_other_cpu0_START (13)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_other_cpu0_END (13)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_other_cpu1_START (14)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_other_cpu1_END (14)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_other_cpu2_START (15)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_ddrsys_autogt_bypass_other_cpu2_END (15)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_bitmasken_START (16)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL2_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ddrc_autogt_bypass_other_mst : 16;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PERI_COMMON_CTRL3_UNION;
#endif
#define SOC_CRGPERIPH_PERI_COMMON_CTRL3_ddrc_autogt_bypass_other_mst_START (0)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL3_ddrc_autogt_bypass_other_mst_END (15)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL3_bitmasken_START (16)
#define SOC_CRGPERIPH_PERI_COMMON_CTRL3_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int ddrsys_idle_waitcfg_in : 10;
        unsigned int ddrsys_idle_waitcfg_out : 10;
        unsigned int ddrsys_autofs_cnt_cfg : 6;
        unsigned int reserved_1 : 5;
    } reg;
} SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL0_UNION;
#endif
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL0_ddrsys_idle_waitcfg_in_START (1)
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL0_ddrsys_idle_waitcfg_in_END (10)
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL0_ddrsys_idle_waitcfg_out_START (11)
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL0_ddrsys_idle_waitcfg_out_END (20)
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL0_ddrsys_autofs_cnt_cfg_START (21)
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL0_ddrsys_autofs_cnt_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ddrsys_atgs_asi8_bypass : 1;
        unsigned int ddrsys_aonoc_bypass : 1;
        unsigned int ddrsys_sysbus_bypass : 1;
        unsigned int ddrsys_t_bypass : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int ddrsys_modem_axi_bypass : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 19;
    } reg;
} SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL1_ddrsys_atgs_asi8_bypass_START (0)
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL1_ddrsys_atgs_asi8_bypass_END (0)
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL1_ddrsys_aonoc_bypass_START (1)
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL1_ddrsys_aonoc_bypass_END (1)
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL1_ddrsys_sysbus_bypass_START (2)
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL1_ddrsys_sysbus_bypass_END (2)
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL1_ddrsys_t_bypass_START (3)
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL1_ddrsys_t_bypass_END (3)
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL1_ddrsys_modem_axi_bypass_START (10)
#define SOC_CRGPERIPH_DDRSYS_AUTOFS_CTRL1_ddrsys_modem_axi_bypass_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int peri_avs_disable_vote : 11;
        unsigned int peri_avs_pmu_sel : 1;
        unsigned int peri_avs_vol_up_wait_down_finish_bypass : 1;
        unsigned int peri_avs_vol_down_wait_up_finish_bypass : 1;
        unsigned int reserved : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PERI_AVS_CTRL0_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AVS_CTRL0_peri_avs_disable_vote_START (0)
#define SOC_CRGPERIPH_PERI_AVS_CTRL0_peri_avs_disable_vote_END (10)
#define SOC_CRGPERIPH_PERI_AVS_CTRL0_peri_avs_pmu_sel_START (11)
#define SOC_CRGPERIPH_PERI_AVS_CTRL0_peri_avs_pmu_sel_END (11)
#define SOC_CRGPERIPH_PERI_AVS_CTRL0_peri_avs_vol_up_wait_down_finish_bypass_START (12)
#define SOC_CRGPERIPH_PERI_AVS_CTRL0_peri_avs_vol_up_wait_down_finish_bypass_END (12)
#define SOC_CRGPERIPH_PERI_AVS_CTRL0_peri_avs_vol_down_wait_up_finish_bypass_START (13)
#define SOC_CRGPERIPH_PERI_AVS_CTRL0_peri_avs_vol_down_wait_up_finish_bypass_END (13)
#define SOC_CRGPERIPH_PERI_AVS_CTRL0_bitmasken_START (16)
#define SOC_CRGPERIPH_PERI_AVS_CTRL0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int peri_avs_vol_down_timeout_cfg : 12;
        unsigned int peri_avs_vol_up_timeout_cfg : 12;
        unsigned int reserved : 8;
    } reg;
} SOC_CRGPERIPH_PERI_AVS_CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AVS_CTRL1_peri_avs_vol_down_timeout_cfg_START (0)
#define SOC_CRGPERIPH_PERI_AVS_CTRL1_peri_avs_vol_down_timeout_cfg_END (11)
#define SOC_CRGPERIPH_PERI_AVS_CTRL1_peri_avs_vol_up_timeout_cfg_START (12)
#define SOC_CRGPERIPH_PERI_AVS_CTRL1_peri_avs_vol_up_timeout_cfg_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int peri_avs_vol_down_idx : 8;
        unsigned int peri_avs_vol_up_idx : 8;
        unsigned int reserved : 16;
    } reg;
} SOC_CRGPERIPH_PERI_AVS_CTRL2_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AVS_CTRL2_peri_avs_vol_down_idx_START (0)
#define SOC_CRGPERIPH_PERI_AVS_CTRL2_peri_avs_vol_down_idx_END (7)
#define SOC_CRGPERIPH_PERI_AVS_CTRL2_peri_avs_vol_up_idx_START (8)
#define SOC_CRGPERIPH_PERI_AVS_CTRL2_peri_avs_vol_up_idx_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ddrc_lp_en_waitcfg_in : 10;
        unsigned int ddrc_lp_en_waitcfg_out : 10;
        unsigned int reserved : 12;
    } reg;
} SOC_CRGPERIPH_PERI_AVS_CTRL3_UNION;
#endif
#define SOC_CRGPERIPH_PERI_AVS_CTRL3_ddrc_lp_en_waitcfg_in_START (0)
#define SOC_CRGPERIPH_PERI_AVS_CTRL3_ddrc_lp_en_waitcfg_in_END (9)
#define SOC_CRGPERIPH_PERI_AVS_CTRL3_ddrc_lp_en_waitcfg_out_START (10)
#define SOC_CRGPERIPH_PERI_AVS_CTRL3_ddrc_lp_en_waitcfg_out_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll0_en : 1;
        unsigned int ppll0_bp : 1;
        unsigned int ppll0_refdiv : 6;
        unsigned int ppll0_fbdiv : 12;
        unsigned int ppll0_postdiv1 : 3;
        unsigned int ppll0_postdiv2 : 3;
        unsigned int ppll0_lock : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_CRGPERIPH_PPLL0CTRL0_UNION;
#endif
#define SOC_CRGPERIPH_PPLL0CTRL0_ppll0_en_START (0)
#define SOC_CRGPERIPH_PPLL0CTRL0_ppll0_en_END (0)
#define SOC_CRGPERIPH_PPLL0CTRL0_ppll0_bp_START (1)
#define SOC_CRGPERIPH_PPLL0CTRL0_ppll0_bp_END (1)
#define SOC_CRGPERIPH_PPLL0CTRL0_ppll0_refdiv_START (2)
#define SOC_CRGPERIPH_PPLL0CTRL0_ppll0_refdiv_END (7)
#define SOC_CRGPERIPH_PPLL0CTRL0_ppll0_fbdiv_START (8)
#define SOC_CRGPERIPH_PPLL0CTRL0_ppll0_fbdiv_END (19)
#define SOC_CRGPERIPH_PPLL0CTRL0_ppll0_postdiv1_START (20)
#define SOC_CRGPERIPH_PPLL0CTRL0_ppll0_postdiv1_END (22)
#define SOC_CRGPERIPH_PPLL0CTRL0_ppll0_postdiv2_START (23)
#define SOC_CRGPERIPH_PPLL0CTRL0_ppll0_postdiv2_END (25)
#define SOC_CRGPERIPH_PPLL0CTRL0_ppll0_lock_START (26)
#define SOC_CRGPERIPH_PPLL0CTRL0_ppll0_lock_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll0_fracdiv : 24;
        unsigned int ppll0_int_mod : 1;
        unsigned int ppll0_cfg_vld : 1;
        unsigned int gt_clk_ppll0 : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_CRGPERIPH_PPLL0CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PPLL0CTRL1_ppll0_fracdiv_START (0)
#define SOC_CRGPERIPH_PPLL0CTRL1_ppll0_fracdiv_END (23)
#define SOC_CRGPERIPH_PPLL0CTRL1_ppll0_int_mod_START (24)
#define SOC_CRGPERIPH_PPLL0CTRL1_ppll0_int_mod_END (24)
#define SOC_CRGPERIPH_PPLL0CTRL1_ppll0_cfg_vld_START (25)
#define SOC_CRGPERIPH_PPLL0CTRL1_ppll0_cfg_vld_END (25)
#define SOC_CRGPERIPH_PPLL0CTRL1_gt_clk_ppll0_START (26)
#define SOC_CRGPERIPH_PPLL0CTRL1_gt_clk_ppll0_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll1_en : 1;
        unsigned int ppll1_bp : 1;
        unsigned int ppll1_refdiv : 6;
        unsigned int ppll1_fbdiv : 12;
        unsigned int ppll1_postdiv1 : 3;
        unsigned int ppll1_postdiv2 : 3;
        unsigned int ppll1_lock : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_CRGPERIPH_PPLL1CTRL0_UNION;
#endif
#define SOC_CRGPERIPH_PPLL1CTRL0_ppll1_en_START (0)
#define SOC_CRGPERIPH_PPLL1CTRL0_ppll1_en_END (0)
#define SOC_CRGPERIPH_PPLL1CTRL0_ppll1_bp_START (1)
#define SOC_CRGPERIPH_PPLL1CTRL0_ppll1_bp_END (1)
#define SOC_CRGPERIPH_PPLL1CTRL0_ppll1_refdiv_START (2)
#define SOC_CRGPERIPH_PPLL1CTRL0_ppll1_refdiv_END (7)
#define SOC_CRGPERIPH_PPLL1CTRL0_ppll1_fbdiv_START (8)
#define SOC_CRGPERIPH_PPLL1CTRL0_ppll1_fbdiv_END (19)
#define SOC_CRGPERIPH_PPLL1CTRL0_ppll1_postdiv1_START (20)
#define SOC_CRGPERIPH_PPLL1CTRL0_ppll1_postdiv1_END (22)
#define SOC_CRGPERIPH_PPLL1CTRL0_ppll1_postdiv2_START (23)
#define SOC_CRGPERIPH_PPLL1CTRL0_ppll1_postdiv2_END (25)
#define SOC_CRGPERIPH_PPLL1CTRL0_ppll1_lock_START (26)
#define SOC_CRGPERIPH_PPLL1CTRL0_ppll1_lock_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll1_fracdiv : 24;
        unsigned int ppll1_int_mod : 1;
        unsigned int ppll1_cfg_vld : 1;
        unsigned int gt_clk_ppll1 : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_CRGPERIPH_PPLL1CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PPLL1CTRL1_ppll1_fracdiv_START (0)
#define SOC_CRGPERIPH_PPLL1CTRL1_ppll1_fracdiv_END (23)
#define SOC_CRGPERIPH_PPLL1CTRL1_ppll1_int_mod_START (24)
#define SOC_CRGPERIPH_PPLL1CTRL1_ppll1_int_mod_END (24)
#define SOC_CRGPERIPH_PPLL1CTRL1_ppll1_cfg_vld_START (25)
#define SOC_CRGPERIPH_PPLL1CTRL1_ppll1_cfg_vld_END (25)
#define SOC_CRGPERIPH_PPLL1CTRL1_gt_clk_ppll1_START (26)
#define SOC_CRGPERIPH_PPLL1CTRL1_gt_clk_ppll1_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll2_en : 1;
        unsigned int ppll2_bp : 1;
        unsigned int ppll2_refdiv : 6;
        unsigned int ppll2_fbdiv : 12;
        unsigned int ppll2_postdiv1 : 3;
        unsigned int ppll2_postdiv2 : 3;
        unsigned int ppll2_lock : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_CRGPERIPH_PPLL2CTRL0_UNION;
#endif
#define SOC_CRGPERIPH_PPLL2CTRL0_ppll2_en_START (0)
#define SOC_CRGPERIPH_PPLL2CTRL0_ppll2_en_END (0)
#define SOC_CRGPERIPH_PPLL2CTRL0_ppll2_bp_START (1)
#define SOC_CRGPERIPH_PPLL2CTRL0_ppll2_bp_END (1)
#define SOC_CRGPERIPH_PPLL2CTRL0_ppll2_refdiv_START (2)
#define SOC_CRGPERIPH_PPLL2CTRL0_ppll2_refdiv_END (7)
#define SOC_CRGPERIPH_PPLL2CTRL0_ppll2_fbdiv_START (8)
#define SOC_CRGPERIPH_PPLL2CTRL0_ppll2_fbdiv_END (19)
#define SOC_CRGPERIPH_PPLL2CTRL0_ppll2_postdiv1_START (20)
#define SOC_CRGPERIPH_PPLL2CTRL0_ppll2_postdiv1_END (22)
#define SOC_CRGPERIPH_PPLL2CTRL0_ppll2_postdiv2_START (23)
#define SOC_CRGPERIPH_PPLL2CTRL0_ppll2_postdiv2_END (25)
#define SOC_CRGPERIPH_PPLL2CTRL0_ppll2_lock_START (26)
#define SOC_CRGPERIPH_PPLL2CTRL0_ppll2_lock_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll2_fracdiv : 24;
        unsigned int ppll2_int_mod : 1;
        unsigned int ppll2_cfg_vld : 1;
        unsigned int gt_clk_ppll2 : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_CRGPERIPH_PPLL2CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PPLL2CTRL1_ppll2_fracdiv_START (0)
#define SOC_CRGPERIPH_PPLL2CTRL1_ppll2_fracdiv_END (23)
#define SOC_CRGPERIPH_PPLL2CTRL1_ppll2_int_mod_START (24)
#define SOC_CRGPERIPH_PPLL2CTRL1_ppll2_int_mod_END (24)
#define SOC_CRGPERIPH_PPLL2CTRL1_ppll2_cfg_vld_START (25)
#define SOC_CRGPERIPH_PPLL2CTRL1_ppll2_cfg_vld_END (25)
#define SOC_CRGPERIPH_PPLL2CTRL1_gt_clk_ppll2_START (26)
#define SOC_CRGPERIPH_PPLL2CTRL1_gt_clk_ppll2_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll3_en : 1;
        unsigned int ppll3_bp : 1;
        unsigned int ppll3_refdiv : 6;
        unsigned int ppll3_fbdiv : 12;
        unsigned int ppll3_postdiv1 : 3;
        unsigned int ppll3_postdiv2 : 3;
        unsigned int ppll3_lock : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_CRGPERIPH_PPLL3CTRL0_UNION;
#endif
#define SOC_CRGPERIPH_PPLL3CTRL0_ppll3_en_START (0)
#define SOC_CRGPERIPH_PPLL3CTRL0_ppll3_en_END (0)
#define SOC_CRGPERIPH_PPLL3CTRL0_ppll3_bp_START (1)
#define SOC_CRGPERIPH_PPLL3CTRL0_ppll3_bp_END (1)
#define SOC_CRGPERIPH_PPLL3CTRL0_ppll3_refdiv_START (2)
#define SOC_CRGPERIPH_PPLL3CTRL0_ppll3_refdiv_END (7)
#define SOC_CRGPERIPH_PPLL3CTRL0_ppll3_fbdiv_START (8)
#define SOC_CRGPERIPH_PPLL3CTRL0_ppll3_fbdiv_END (19)
#define SOC_CRGPERIPH_PPLL3CTRL0_ppll3_postdiv1_START (20)
#define SOC_CRGPERIPH_PPLL3CTRL0_ppll3_postdiv1_END (22)
#define SOC_CRGPERIPH_PPLL3CTRL0_ppll3_postdiv2_START (23)
#define SOC_CRGPERIPH_PPLL3CTRL0_ppll3_postdiv2_END (25)
#define SOC_CRGPERIPH_PPLL3CTRL0_ppll3_lock_START (26)
#define SOC_CRGPERIPH_PPLL3CTRL0_ppll3_lock_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll3_fracdiv : 24;
        unsigned int ppll3_int_mod : 1;
        unsigned int ppll3_cfg_vld : 1;
        unsigned int gt_clk_ppll3 : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_CRGPERIPH_PPLL3CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PPLL3CTRL1_ppll3_fracdiv_START (0)
#define SOC_CRGPERIPH_PPLL3CTRL1_ppll3_fracdiv_END (23)
#define SOC_CRGPERIPH_PPLL3CTRL1_ppll3_int_mod_START (24)
#define SOC_CRGPERIPH_PPLL3CTRL1_ppll3_int_mod_END (24)
#define SOC_CRGPERIPH_PPLL3CTRL1_ppll3_cfg_vld_START (25)
#define SOC_CRGPERIPH_PPLL3CTRL1_ppll3_cfg_vld_END (25)
#define SOC_CRGPERIPH_PPLL3CTRL1_gt_clk_ppll3_START (26)
#define SOC_CRGPERIPH_PPLL3CTRL1_gt_clk_ppll3_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll7_en : 1;
        unsigned int ppll7_bp : 1;
        unsigned int ppll7_refdiv : 6;
        unsigned int ppll7_fbdiv : 12;
        unsigned int ppll7_postdiv1 : 3;
        unsigned int ppll7_postdiv2 : 3;
        unsigned int ppll7_lock : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_CRGPERIPH_PPLL7CTRL0_UNION;
#endif
#define SOC_CRGPERIPH_PPLL7CTRL0_ppll7_en_START (0)
#define SOC_CRGPERIPH_PPLL7CTRL0_ppll7_en_END (0)
#define SOC_CRGPERIPH_PPLL7CTRL0_ppll7_bp_START (1)
#define SOC_CRGPERIPH_PPLL7CTRL0_ppll7_bp_END (1)
#define SOC_CRGPERIPH_PPLL7CTRL0_ppll7_refdiv_START (2)
#define SOC_CRGPERIPH_PPLL7CTRL0_ppll7_refdiv_END (7)
#define SOC_CRGPERIPH_PPLL7CTRL0_ppll7_fbdiv_START (8)
#define SOC_CRGPERIPH_PPLL7CTRL0_ppll7_fbdiv_END (19)
#define SOC_CRGPERIPH_PPLL7CTRL0_ppll7_postdiv1_START (20)
#define SOC_CRGPERIPH_PPLL7CTRL0_ppll7_postdiv1_END (22)
#define SOC_CRGPERIPH_PPLL7CTRL0_ppll7_postdiv2_START (23)
#define SOC_CRGPERIPH_PPLL7CTRL0_ppll7_postdiv2_END (25)
#define SOC_CRGPERIPH_PPLL7CTRL0_ppll7_lock_START (26)
#define SOC_CRGPERIPH_PPLL7CTRL0_ppll7_lock_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll7_fracdiv : 24;
        unsigned int ppll7_int_mod : 1;
        unsigned int ppll7_cfg_vld : 1;
        unsigned int gt_clk_ppll7 : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_CRGPERIPH_PPLL7CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PPLL7CTRL1_ppll7_fracdiv_START (0)
#define SOC_CRGPERIPH_PPLL7CTRL1_ppll7_fracdiv_END (23)
#define SOC_CRGPERIPH_PPLL7CTRL1_ppll7_int_mod_START (24)
#define SOC_CRGPERIPH_PPLL7CTRL1_ppll7_int_mod_END (24)
#define SOC_CRGPERIPH_PPLL7CTRL1_ppll7_cfg_vld_START (25)
#define SOC_CRGPERIPH_PPLL7CTRL1_ppll7_cfg_vld_END (25)
#define SOC_CRGPERIPH_PPLL7CTRL1_gt_clk_ppll7_START (26)
#define SOC_CRGPERIPH_PPLL7CTRL1_gt_clk_ppll7_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll0_ssc_reset : 1;
        unsigned int ppll0_ssc_disable : 1;
        unsigned int ppll0_ssc_downspread : 1;
        unsigned int ppll0_ssc_spread : 3;
        unsigned int reserved_0 : 2;
        unsigned int ppll0_ssc_divval : 4;
        unsigned int reserved_1 : 2;
        unsigned int ppll0_ssmod_rstnptr : 1;
        unsigned int ppll0_ssc_self_vld : 1;
        unsigned int reserved_2 : 16;
    } reg;
} SOC_CRGPERIPH_PPLL0SSCCTRL_UNION;
#endif
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ppll0_ssc_reset_START (0)
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ppll0_ssc_reset_END (0)
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ppll0_ssc_disable_START (1)
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ppll0_ssc_disable_END (1)
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ppll0_ssc_downspread_START (2)
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ppll0_ssc_downspread_END (2)
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ppll0_ssc_spread_START (3)
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ppll0_ssc_spread_END (5)
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ppll0_ssc_divval_START (8)
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ppll0_ssc_divval_END (11)
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ppll0_ssmod_rstnptr_START (14)
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ppll0_ssmod_rstnptr_END (14)
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ppll0_ssc_self_vld_START (15)
#define SOC_CRGPERIPH_PPLL0SSCCTRL_ppll0_ssc_self_vld_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll1_ssc_reset : 1;
        unsigned int ppll1_ssc_disable : 1;
        unsigned int ppll1_ssc_downspread : 1;
        unsigned int ppll1_ssc_spread : 3;
        unsigned int reserved_0 : 2;
        unsigned int ppll1_ssc_divval : 4;
        unsigned int reserved_1 : 2;
        unsigned int ppll1_ssmod_rstnptr : 1;
        unsigned int ppll1_ssc_self_vld : 1;
        unsigned int reserved_2 : 16;
    } reg;
} SOC_CRGPERIPH_PPLL1SSCCTRL_UNION;
#endif
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ppll1_ssc_reset_START (0)
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ppll1_ssc_reset_END (0)
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ppll1_ssc_disable_START (1)
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ppll1_ssc_disable_END (1)
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ppll1_ssc_downspread_START (2)
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ppll1_ssc_downspread_END (2)
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ppll1_ssc_spread_START (3)
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ppll1_ssc_spread_END (5)
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ppll1_ssc_divval_START (8)
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ppll1_ssc_divval_END (11)
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ppll1_ssmod_rstnptr_START (14)
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ppll1_ssmod_rstnptr_END (14)
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ppll1_ssc_self_vld_START (15)
#define SOC_CRGPERIPH_PPLL1SSCCTRL_ppll1_ssc_self_vld_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll2_ssc_reset : 1;
        unsigned int ppll2_ssc_disable : 1;
        unsigned int ppll2_ssc_downspread : 1;
        unsigned int ppll2_ssc_spread : 3;
        unsigned int reserved_0 : 2;
        unsigned int ppll2_ssc_divval : 4;
        unsigned int reserved_1 : 2;
        unsigned int ppll2_ssmod_rstnptr : 1;
        unsigned int ppll2_ssc_self_vld : 1;
        unsigned int reserved_2 : 16;
    } reg;
} SOC_CRGPERIPH_PPLL2SSCCTRL_UNION;
#endif
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ppll2_ssc_reset_START (0)
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ppll2_ssc_reset_END (0)
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ppll2_ssc_disable_START (1)
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ppll2_ssc_disable_END (1)
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ppll2_ssc_downspread_START (2)
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ppll2_ssc_downspread_END (2)
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ppll2_ssc_spread_START (3)
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ppll2_ssc_spread_END (5)
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ppll2_ssc_divval_START (8)
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ppll2_ssc_divval_END (11)
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ppll2_ssmod_rstnptr_START (14)
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ppll2_ssmod_rstnptr_END (14)
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ppll2_ssc_self_vld_START (15)
#define SOC_CRGPERIPH_PPLL2SSCCTRL_ppll2_ssc_self_vld_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll3_ssc_reset : 1;
        unsigned int ppll3_ssc_disable : 1;
        unsigned int ppll3_ssc_downspread : 1;
        unsigned int ppll3_ssc_spread : 3;
        unsigned int reserved_0 : 2;
        unsigned int ppll3_ssc_divval : 4;
        unsigned int reserved_1 : 2;
        unsigned int ppll3_ssmod_rstnptr : 1;
        unsigned int ppll3_ssc_self_vld : 1;
        unsigned int reserved_2 : 16;
    } reg;
} SOC_CRGPERIPH_PPLL3SSCCTRL_UNION;
#endif
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ppll3_ssc_reset_START (0)
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ppll3_ssc_reset_END (0)
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ppll3_ssc_disable_START (1)
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ppll3_ssc_disable_END (1)
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ppll3_ssc_downspread_START (2)
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ppll3_ssc_downspread_END (2)
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ppll3_ssc_spread_START (3)
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ppll3_ssc_spread_END (5)
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ppll3_ssc_divval_START (8)
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ppll3_ssc_divval_END (11)
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ppll3_ssmod_rstnptr_START (14)
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ppll3_ssmod_rstnptr_END (14)
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ppll3_ssc_self_vld_START (15)
#define SOC_CRGPERIPH_PPLL3SSCCTRL_ppll3_ssc_self_vld_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll7_ssc_reset : 1;
        unsigned int ppll7_ssc_disable : 1;
        unsigned int ppll7_ssc_downspread : 1;
        unsigned int ppll7_ssc_spread : 3;
        unsigned int reserved_0 : 2;
        unsigned int ppll7_ssc_divval : 4;
        unsigned int reserved_1 : 2;
        unsigned int ppll7_ssmod_rstnptr : 1;
        unsigned int ppll7_ssc_self_vld : 1;
        unsigned int reserved_2 : 16;
    } reg;
} SOC_CRGPERIPH_PPLL7SSCCTRL_UNION;
#endif
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ppll7_ssc_reset_START (0)
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ppll7_ssc_reset_END (0)
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ppll7_ssc_disable_START (1)
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ppll7_ssc_disable_END (1)
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ppll7_ssc_downspread_START (2)
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ppll7_ssc_downspread_END (2)
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ppll7_ssc_spread_START (3)
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ppll7_ssc_spread_END (5)
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ppll7_ssc_divval_START (8)
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ppll7_ssc_divval_END (11)
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ppll7_ssmod_rstnptr_START (14)
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ppll7_ssmod_rstnptr_END (14)
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ppll7_ssc_self_vld_START (15)
#define SOC_CRGPERIPH_PPLL7SSCCTRL_ppll7_ssc_self_vld_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll2_b_en : 1;
        unsigned int ppll2_b_bp : 1;
        unsigned int ppll2_b_refdiv : 6;
        unsigned int ppll2_b_fbdiv : 12;
        unsigned int ppll2_b_postdiv1 : 3;
        unsigned int ppll2_b_postdiv2 : 3;
        unsigned int ppll2_b_lock : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_CRGPERIPH_PPLL2_B_CTRL0_UNION;
#endif
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ppll2_b_en_START (0)
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ppll2_b_en_END (0)
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ppll2_b_bp_START (1)
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ppll2_b_bp_END (1)
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ppll2_b_refdiv_START (2)
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ppll2_b_refdiv_END (7)
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ppll2_b_fbdiv_START (8)
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ppll2_b_fbdiv_END (19)
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ppll2_b_postdiv1_START (20)
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ppll2_b_postdiv1_END (22)
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ppll2_b_postdiv2_START (23)
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ppll2_b_postdiv2_END (25)
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ppll2_b_lock_START (26)
#define SOC_CRGPERIPH_PPLL2_B_CTRL0_ppll2_b_lock_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll2_b_fracdiv : 24;
        unsigned int ppll2_b_int_mod : 1;
        unsigned int ppll2_b_cfg_vld : 1;
        unsigned int gt_clk_ppll2_b : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_CRGPERIPH_PPLL2_B_CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PPLL2_B_CTRL1_ppll2_b_fracdiv_START (0)
#define SOC_CRGPERIPH_PPLL2_B_CTRL1_ppll2_b_fracdiv_END (23)
#define SOC_CRGPERIPH_PPLL2_B_CTRL1_ppll2_b_int_mod_START (24)
#define SOC_CRGPERIPH_PPLL2_B_CTRL1_ppll2_b_int_mod_END (24)
#define SOC_CRGPERIPH_PPLL2_B_CTRL1_ppll2_b_cfg_vld_START (25)
#define SOC_CRGPERIPH_PPLL2_B_CTRL1_ppll2_b_cfg_vld_END (25)
#define SOC_CRGPERIPH_PPLL2_B_CTRL1_gt_clk_ppll2_b_START (26)
#define SOC_CRGPERIPH_PPLL2_B_CTRL1_gt_clk_ppll2_b_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll2_b_ssc_reset : 1;
        unsigned int ppll2_b_ssc_disable : 1;
        unsigned int ppll2_b_ssc_downspread : 1;
        unsigned int ppll2_b_ssc_spread : 3;
        unsigned int reserved_0 : 2;
        unsigned int ppll2_b_ssc_divval : 4;
        unsigned int reserved_1 : 2;
        unsigned int ppll2_b_ssmod_rstnptr : 1;
        unsigned int ppll2_b_ssc_self_vld : 1;
        unsigned int reserved_2 : 16;
    } reg;
} SOC_CRGPERIPH_PPLL2_B_SSCCTRL_UNION;
#endif
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ppll2_b_ssc_reset_START (0)
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ppll2_b_ssc_reset_END (0)
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ppll2_b_ssc_disable_START (1)
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ppll2_b_ssc_disable_END (1)
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ppll2_b_ssc_downspread_START (2)
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ppll2_b_ssc_downspread_END (2)
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ppll2_b_ssc_spread_START (3)
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ppll2_b_ssc_spread_END (5)
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ppll2_b_ssc_divval_START (8)
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ppll2_b_ssc_divval_END (11)
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ppll2_b_ssmod_rstnptr_START (14)
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ppll2_b_ssmod_rstnptr_END (14)
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ppll2_b_ssc_self_vld_START (15)
#define SOC_CRGPERIPH_PPLL2_B_SSCCTRL_ppll2_b_ssc_self_vld_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fn_sc_ppll7_sel : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int fn_sc_ppll3_sel : 1;
        unsigned int fn_sc_ppll2_b_sel : 1;
        unsigned int fn_sc_ppll2_sel : 1;
        unsigned int fn_sc_ppll1_sel : 1;
        unsigned int fn_sc_ppll0_sel : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 6;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PLL_FN_SC_SEL_UNION;
#endif
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_fn_sc_ppll7_sel_START (0)
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_fn_sc_ppll7_sel_END (0)
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_fn_sc_ppll3_sel_START (3)
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_fn_sc_ppll3_sel_END (3)
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_fn_sc_ppll2_b_sel_START (4)
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_fn_sc_ppll2_b_sel_END (4)
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_fn_sc_ppll2_sel_START (5)
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_fn_sc_ppll2_sel_END (5)
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_fn_sc_ppll1_sel_START (6)
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_fn_sc_ppll1_sel_END (6)
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_fn_sc_ppll0_sel_START (7)
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_fn_sc_ppll0_sel_END (7)
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_bitmasken_START (16)
#define SOC_CRGPERIPH_PLL_FN_SC_SEL_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_bbdrv : 3;
        unsigned int pll_bben : 1;
        unsigned int pll_bbenoverride : 1;
        unsigned int pll_bbpw : 3;
        unsigned int pll_bwprog : 3;
        unsigned int pll_bwprogen : 1;
        unsigned int pll_foutvco_en : 1;
        unsigned int pll_offsetcalbyp : 1;
        unsigned int pll_offsetcalcnt : 3;
        unsigned int pll_offsetcalen : 1;
        unsigned int pll_offsetcalin : 12;
        unsigned int pll_offsetfastcal : 1;
        unsigned int reserved : 1;
    } reg;
} SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_UNION;
#endif
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_bbdrv_START (0)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_bbdrv_END (2)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_bben_START (3)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_bben_END (3)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_bbenoverride_START (4)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_bbenoverride_END (4)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_bbpw_START (5)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_bbpw_END (7)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_bwprog_START (8)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_bwprog_END (10)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_bwprogen_START (11)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_bwprogen_END (11)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_foutvco_en_START (12)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_foutvco_en_END (12)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_offsetcalbyp_START (13)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_offsetcalbyp_END (13)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_offsetcalcnt_START (14)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_offsetcalcnt_END (16)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_offsetcalen_START (17)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_offsetcalen_END (17)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_offsetcalin_START (18)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_offsetcalin_END (29)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_offsetfastcal_START (30)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL0_pll_offsetfastcal_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_vcosel_ppll0 : 2;
        unsigned int pll_qpiprog : 3;
        unsigned int pll_qpiprogen : 1;
        unsigned int pll_qpiskewprog : 3;
        unsigned int pll_qpiskewprogen : 1;
        unsigned int pll_fprog : 3;
        unsigned int pll_fprogen : 1;
        unsigned int reserved : 2;
        unsigned int pll_reserved : 16;
    } reg;
} SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_UNION;
#endif
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_vcosel_ppll0_START (0)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_vcosel_ppll0_END (1)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_qpiprog_START (2)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_qpiprog_END (4)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_qpiprogen_START (5)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_qpiprogen_END (5)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_qpiskewprog_START (6)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_qpiskewprog_END (8)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_qpiskewprogen_START (9)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_qpiskewprogen_END (9)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_fprog_START (10)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_fprog_END (12)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_fprogen_START (13)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_fprogen_END (13)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_reserved_START (16)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL0_pll_reserved_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_bbdrv : 3;
        unsigned int pll_bben : 1;
        unsigned int pll_bbenoverride : 1;
        unsigned int pll_bbpw : 3;
        unsigned int pll_bwprog : 3;
        unsigned int pll_bwprogen : 1;
        unsigned int pll_foutvco_en : 1;
        unsigned int pll_offsetcalbyp : 1;
        unsigned int pll_offsetcalcnt : 3;
        unsigned int pll_offsetcalen : 1;
        unsigned int pll_offsetcalin : 12;
        unsigned int pll_offsetfastcal : 1;
        unsigned int reserved : 1;
    } reg;
} SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_UNION;
#endif
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_bbdrv_START (0)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_bbdrv_END (2)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_bben_START (3)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_bben_END (3)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_bbenoverride_START (4)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_bbenoverride_END (4)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_bbpw_START (5)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_bbpw_END (7)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_bwprog_START (8)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_bwprog_END (10)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_bwprogen_START (11)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_bwprogen_END (11)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_foutvco_en_START (12)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_foutvco_en_END (12)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_offsetcalbyp_START (13)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_offsetcalbyp_END (13)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_offsetcalcnt_START (14)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_offsetcalcnt_END (16)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_offsetcalen_START (17)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_offsetcalen_END (17)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_offsetcalin_START (18)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_offsetcalin_END (29)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_offsetfastcal_START (30)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL1_pll_offsetfastcal_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_vcosel_ppll1 : 2;
        unsigned int pll_qpiprog : 3;
        unsigned int pll_qpiprogen : 1;
        unsigned int pll_qpiskewprog : 3;
        unsigned int pll_qpiskewprogen : 1;
        unsigned int pll_fprog : 3;
        unsigned int pll_fprogen : 1;
        unsigned int reserved : 2;
        unsigned int pll_reserved : 16;
    } reg;
} SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_UNION;
#endif
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_vcosel_ppll1_START (0)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_vcosel_ppll1_END (1)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_qpiprog_START (2)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_qpiprog_END (4)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_qpiprogen_START (5)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_qpiprogen_END (5)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_qpiskewprog_START (6)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_qpiskewprog_END (8)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_qpiskewprogen_START (9)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_qpiskewprogen_END (9)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_fprog_START (10)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_fprog_END (12)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_fprogen_START (13)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_fprogen_END (13)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_reserved_START (16)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL1_pll_reserved_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_bbdrv : 3;
        unsigned int pll_bben : 1;
        unsigned int pll_bbenoverride : 1;
        unsigned int pll_bbpw : 3;
        unsigned int pll_bwprog : 3;
        unsigned int pll_bwprogen : 1;
        unsigned int pll_foutvco_en : 1;
        unsigned int pll_offsetcalbyp : 1;
        unsigned int pll_offsetcalcnt : 3;
        unsigned int pll_offsetcalen : 1;
        unsigned int pll_offsetcalin : 12;
        unsigned int pll_offsetfastcal : 1;
        unsigned int reserved : 1;
    } reg;
} SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_UNION;
#endif
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_bbdrv_START (0)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_bbdrv_END (2)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_bben_START (3)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_bben_END (3)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_bbenoverride_START (4)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_bbenoverride_END (4)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_bbpw_START (5)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_bbpw_END (7)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_bwprog_START (8)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_bwprog_END (10)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_bwprogen_START (11)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_bwprogen_END (11)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_foutvco_en_START (12)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_foutvco_en_END (12)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_offsetcalbyp_START (13)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_offsetcalbyp_END (13)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_offsetcalcnt_START (14)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_offsetcalcnt_END (16)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_offsetcalen_START (17)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_offsetcalen_END (17)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_offsetcalin_START (18)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_offsetcalin_END (29)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_offsetfastcal_START (30)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL2_pll_offsetfastcal_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_vcosel_ppll2 : 2;
        unsigned int pll_qpiprog : 3;
        unsigned int pll_qpiprogen : 1;
        unsigned int pll_qpiskewprog : 3;
        unsigned int pll_qpiskewprogen : 1;
        unsigned int pll_fprog : 3;
        unsigned int pll_fprogen : 1;
        unsigned int reserved : 2;
        unsigned int pll_reserved : 16;
    } reg;
} SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_UNION;
#endif
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_vcosel_ppll2_START (0)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_vcosel_ppll2_END (1)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_qpiprog_START (2)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_qpiprog_END (4)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_qpiprogen_START (5)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_qpiprogen_END (5)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_qpiskewprog_START (6)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_qpiskewprog_END (8)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_qpiskewprogen_START (9)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_qpiskewprogen_END (9)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_fprog_START (10)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_fprog_END (12)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_fprogen_START (13)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_fprogen_END (13)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_reserved_START (16)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL2_pll_reserved_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_bbdrv : 3;
        unsigned int pll_bben : 1;
        unsigned int pll_bbenoverride : 1;
        unsigned int pll_bbpw : 3;
        unsigned int pll_bwprog : 3;
        unsigned int pll_bwprogen : 1;
        unsigned int pll_foutvco_en : 1;
        unsigned int pll_offsetcalbyp : 1;
        unsigned int pll_offsetcalcnt : 3;
        unsigned int pll_offsetcalen : 1;
        unsigned int pll_offsetcalin : 12;
        unsigned int pll_offsetfastcal : 1;
        unsigned int reserved : 1;
    } reg;
} SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_UNION;
#endif
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_bbdrv_START (0)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_bbdrv_END (2)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_bben_START (3)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_bben_END (3)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_bbenoverride_START (4)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_bbenoverride_END (4)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_bbpw_START (5)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_bbpw_END (7)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_bwprog_START (8)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_bwprog_END (10)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_bwprogen_START (11)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_bwprogen_END (11)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_foutvco_en_START (12)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_foutvco_en_END (12)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_offsetcalbyp_START (13)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_offsetcalbyp_END (13)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_offsetcalcnt_START (14)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_offsetcalcnt_END (16)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_offsetcalen_START (17)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_offsetcalen_END (17)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_offsetcalin_START (18)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_offsetcalin_END (29)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_offsetfastcal_START (30)
#define SOC_CRGPERIPH_PLL_CTRL_A_PPLL3_pll_offsetfastcal_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_vcosel_ppll3 : 2;
        unsigned int pll_qpiprog : 3;
        unsigned int pll_qpiprogen : 1;
        unsigned int pll_qpiskewprog : 3;
        unsigned int pll_qpiskewprogen : 1;
        unsigned int pll_fprog : 3;
        unsigned int pll_fprogen : 1;
        unsigned int reserved : 2;
        unsigned int pll_reserved : 16;
    } reg;
} SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_UNION;
#endif
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_vcosel_ppll3_START (0)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_vcosel_ppll3_END (1)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_qpiprog_START (2)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_qpiprog_END (4)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_qpiprogen_START (5)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_qpiprogen_END (5)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_qpiskewprog_START (6)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_qpiskewprog_END (8)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_qpiskewprogen_START (9)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_qpiskewprogen_END (9)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_fprog_START (10)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_fprog_END (12)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_fprogen_START (13)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_fprogen_END (13)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_reserved_START (16)
#define SOC_CRGPERIPH_PLL_CTRL_B_PPLL3_pll_reserved_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qactive_cfgcnt_gpu : 7;
        unsigned int gt_clk_bypass_gpu : 1;
        unsigned int reserved : 24;
    } reg;
} SOC_CRGPERIPH_GPU_ADBLPCTRL_UNION;
#endif
#define SOC_CRGPERIPH_GPU_ADBLPCTRL_qactive_cfgcnt_gpu_START (0)
#define SOC_CRGPERIPH_GPU_ADBLPCTRL_qactive_cfgcnt_gpu_END (6)
#define SOC_CRGPERIPH_GPU_ADBLPCTRL_gt_clk_bypass_gpu_START (7)
#define SOC_CRGPERIPH_GPU_ADBLPCTRL_gt_clk_bypass_gpu_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qactive_cfgcnt_cpu : 7;
        unsigned int gt_clk_bypass_cpu : 1;
        unsigned int reserved : 24;
    } reg;
} SOC_CRGPERIPH_CPU_ADBLPCTRL_UNION;
#endif
#define SOC_CRGPERIPH_CPU_ADBLPCTRL_qactive_cfgcnt_cpu_START (0)
#define SOC_CRGPERIPH_CPU_ADBLPCTRL_qactive_cfgcnt_cpu_END (6)
#define SOC_CRGPERIPH_CPU_ADBLPCTRL_gt_clk_bypass_cpu_START (7)
#define SOC_CRGPERIPH_CPU_ADBLPCTRL_gt_clk_bypass_cpu_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qactive_cfgcnt_mst_mdm : 7;
        unsigned int gt_clk_bypass_mst_mdm : 1;
        unsigned int reserved : 24;
    } reg;
} SOC_CRGPERIPH_MDM_ADBLPCTRL_UNION;
#endif
#define SOC_CRGPERIPH_MDM_ADBLPCTRL_qactive_cfgcnt_mst_mdm_START (0)
#define SOC_CRGPERIPH_MDM_ADBLPCTRL_qactive_cfgcnt_mst_mdm_END (6)
#define SOC_CRGPERIPH_MDM_ADBLPCTRL_gt_clk_bypass_mst_mdm_START (7)
#define SOC_CRGPERIPH_MDM_ADBLPCTRL_gt_clk_bypass_mst_mdm_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qactive_cfgcnt_npu : 7;
        unsigned int gt_clk_bypass_npu : 1;
        unsigned int reserved : 24;
    } reg;
} SOC_CRGPERIPH_NPU_ADBLPCTRL_UNION;
#endif
#define SOC_CRGPERIPH_NPU_ADBLPCTRL_qactive_cfgcnt_npu_START (0)
#define SOC_CRGPERIPH_NPU_ADBLPCTRL_qactive_cfgcnt_npu_END (6)
#define SOC_CRGPERIPH_NPU_ADBLPCTRL_gt_clk_bypass_npu_START (7)
#define SOC_CRGPERIPH_NPU_ADBLPCTRL_gt_clk_bypass_npu_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_unlock_intr_clr : 1;
        unsigned int pll_unlock_intr_clr_acpu : 1;
        unsigned int pll_unlock_en : 1;
        unsigned int pll_unlock_en_acpu : 1;
        unsigned int reserved : 12;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_CLEAR_UNION;
#endif
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_CLEAR_pll_unlock_intr_clr_START (0)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_CLEAR_pll_unlock_intr_clr_END (0)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_CLEAR_pll_unlock_intr_clr_acpu_START (1)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_CLEAR_pll_unlock_intr_clr_acpu_END (1)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_CLEAR_pll_unlock_en_START (2)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_CLEAR_pll_unlock_en_END (2)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_CLEAR_pll_unlock_en_acpu_START (3)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_CLEAR_pll_unlock_en_acpu_END (3)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_CLEAR_bitmasken_START (16)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_CLEAR_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ulppll_unlock_intr_bypass : 1;
        unsigned int ulpppll_1_unlock_intr_bypass : 1;
        unsigned int ppll0_unlock_intr_bypass : 1;
        unsigned int apll0_unlock_intr_bypass : 1;
        unsigned int apll1_unlock_intr_bypass : 1;
        unsigned int apll2_unlock_intr_bypass : 1;
        unsigned int apll3_unlock_intr_bypass : 1;
        unsigned int scpll_s_media_unlock_intr_bypass : 1;
        unsigned int apll5_unlock_intr_bypass : 1;
        unsigned int ppll2_b_unlock_intr_bypass : 1;
        unsigned int ppll1_unlock_intr_bypass : 1;
        unsigned int ppll2_unlock_intr_bypass : 1;
        unsigned int ppll3_unlock_intr_bypass : 1;
        unsigned int apll7_unlock_intr_bypass : 1;
        unsigned int pcie1pll_fn_unlock_intr_bypass : 1;
        unsigned int pcie1pll_sc_unlock_intr_bypass : 1;
        unsigned int pcie_fnpll_unlock_intr_bypass : 1;
        unsigned int ppll6_unlock_intr_bypass : 1;
        unsigned int ppll7_unlock_intr_bypass : 1;
        unsigned int pcieppll_unlock_intr_bypass : 1;
        unsigned int spll_unlock_intr_bypass : 1;
        unsigned int lbintjpll_unlock_intr_bypass : 1;
        unsigned int ppll5_unlock_intr_bypass : 1;
        unsigned int aupll_unlock_intr_bypass : 1;
        unsigned int fnplls_media1_unlock_intr_bypass : 1;
        unsigned int reserved : 7;
    } reg;
} SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_UNION;
#endif
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ulppll_unlock_intr_bypass_START (0)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ulppll_unlock_intr_bypass_END (0)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ulpppll_1_unlock_intr_bypass_START (1)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ulpppll_1_unlock_intr_bypass_END (1)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll0_unlock_intr_bypass_START (2)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll0_unlock_intr_bypass_END (2)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_apll0_unlock_intr_bypass_START (3)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_apll0_unlock_intr_bypass_END (3)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_apll1_unlock_intr_bypass_START (4)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_apll1_unlock_intr_bypass_END (4)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_apll2_unlock_intr_bypass_START (5)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_apll2_unlock_intr_bypass_END (5)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_apll3_unlock_intr_bypass_START (6)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_apll3_unlock_intr_bypass_END (6)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_scpll_s_media_unlock_intr_bypass_START (7)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_scpll_s_media_unlock_intr_bypass_END (7)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_apll5_unlock_intr_bypass_START (8)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_apll5_unlock_intr_bypass_END (8)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll2_b_unlock_intr_bypass_START (9)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll2_b_unlock_intr_bypass_END (9)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll1_unlock_intr_bypass_START (10)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll1_unlock_intr_bypass_END (10)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll2_unlock_intr_bypass_START (11)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll2_unlock_intr_bypass_END (11)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll3_unlock_intr_bypass_START (12)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll3_unlock_intr_bypass_END (12)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_apll7_unlock_intr_bypass_START (13)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_apll7_unlock_intr_bypass_END (13)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_pcie1pll_fn_unlock_intr_bypass_START (14)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_pcie1pll_fn_unlock_intr_bypass_END (14)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_pcie1pll_sc_unlock_intr_bypass_START (15)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_pcie1pll_sc_unlock_intr_bypass_END (15)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_pcie_fnpll_unlock_intr_bypass_START (16)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_pcie_fnpll_unlock_intr_bypass_END (16)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll6_unlock_intr_bypass_START (17)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll6_unlock_intr_bypass_END (17)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll7_unlock_intr_bypass_START (18)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll7_unlock_intr_bypass_END (18)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_pcieppll_unlock_intr_bypass_START (19)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_pcieppll_unlock_intr_bypass_END (19)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_spll_unlock_intr_bypass_START (20)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_spll_unlock_intr_bypass_END (20)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_lbintjpll_unlock_intr_bypass_START (21)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_lbintjpll_unlock_intr_bypass_END (21)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll5_unlock_intr_bypass_START (22)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_ppll5_unlock_intr_bypass_END (22)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_aupll_unlock_intr_bypass_START (23)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_aupll_unlock_intr_bypass_END (23)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_fnplls_media1_unlock_intr_bypass_START (24)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_ACPU_fnplls_media1_unlock_intr_bypass_END (24)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_lock_debounce_cfg : 10;
        unsigned int pll_unlock_debounce_cfg : 10;
        unsigned int pll_en_posedge_delay_enable : 1;
        unsigned int reserved : 11;
    } reg;
} SOC_CRGPERIPH_PLL_UNLOCK_DETECT_CTRL_UNION;
#endif
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_CTRL_pll_lock_debounce_cfg_START (0)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_CTRL_pll_lock_debounce_cfg_END (9)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_CTRL_pll_unlock_debounce_cfg_START (10)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_CTRL_pll_unlock_debounce_cfg_END (19)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_CTRL_pll_en_posedge_delay_enable_START (20)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_CTRL_pll_en_posedge_delay_enable_END (20)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ulppll_unlock_intr_bypass : 1;
        unsigned int ulpppll_1_unlock_intr_bypass : 1;
        unsigned int ppll0_unlock_intr_bypass : 1;
        unsigned int apll0_unlock_intr_bypass : 1;
        unsigned int apll1_unlock_intr_bypass : 1;
        unsigned int apll2_unlock_intr_bypass : 1;
        unsigned int apll3_unlock_intr_bypass : 1;
        unsigned int scpll_s_media_unlock_intr_bypass : 1;
        unsigned int apll5_unlock_intr_bypass : 1;
        unsigned int ppll2_b_unlock_intr_bypass : 1;
        unsigned int ppll1_unlock_intr_bypass : 1;
        unsigned int ppll2_unlock_intr_bypass : 1;
        unsigned int ppll3_unlock_intr_bypass : 1;
        unsigned int apll7_unlock_intr_bypass : 1;
        unsigned int pcie1pll_fn_unlock_intr_bypass : 1;
        unsigned int pcie1pll_sc_unlock_intr_bypass : 1;
        unsigned int pcie_fnpll_unlock_intr_bypass : 1;
        unsigned int ppll6_unlock_intr_bypass : 1;
        unsigned int ppll7_unlock_intr_bypass : 1;
        unsigned int pcieppll_unlock_intr_bypass : 1;
        unsigned int spll_unlock_intr_bypass : 1;
        unsigned int lbintjpll_unlock_intr_bypass : 1;
        unsigned int ppll5_unlock_intr_bypass : 1;
        unsigned int aupll_unlock_intr_bypass : 1;
        unsigned int fnpll_s_media1_unlock_intr_bypass : 1;
        unsigned int reserved : 7;
    } reg;
} SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_UNION;
#endif
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ulppll_unlock_intr_bypass_START (0)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ulppll_unlock_intr_bypass_END (0)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ulpppll_1_unlock_intr_bypass_START (1)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ulpppll_1_unlock_intr_bypass_END (1)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll0_unlock_intr_bypass_START (2)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll0_unlock_intr_bypass_END (2)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_apll0_unlock_intr_bypass_START (3)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_apll0_unlock_intr_bypass_END (3)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_apll1_unlock_intr_bypass_START (4)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_apll1_unlock_intr_bypass_END (4)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_apll2_unlock_intr_bypass_START (5)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_apll2_unlock_intr_bypass_END (5)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_apll3_unlock_intr_bypass_START (6)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_apll3_unlock_intr_bypass_END (6)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_scpll_s_media_unlock_intr_bypass_START (7)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_scpll_s_media_unlock_intr_bypass_END (7)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_apll5_unlock_intr_bypass_START (8)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_apll5_unlock_intr_bypass_END (8)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll2_b_unlock_intr_bypass_START (9)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll2_b_unlock_intr_bypass_END (9)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll1_unlock_intr_bypass_START (10)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll1_unlock_intr_bypass_END (10)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll2_unlock_intr_bypass_START (11)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll2_unlock_intr_bypass_END (11)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll3_unlock_intr_bypass_START (12)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll3_unlock_intr_bypass_END (12)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_apll7_unlock_intr_bypass_START (13)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_apll7_unlock_intr_bypass_END (13)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_pcie1pll_fn_unlock_intr_bypass_START (14)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_pcie1pll_fn_unlock_intr_bypass_END (14)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_pcie1pll_sc_unlock_intr_bypass_START (15)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_pcie1pll_sc_unlock_intr_bypass_END (15)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_pcie_fnpll_unlock_intr_bypass_START (16)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_pcie_fnpll_unlock_intr_bypass_END (16)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll6_unlock_intr_bypass_START (17)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll6_unlock_intr_bypass_END (17)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll7_unlock_intr_bypass_START (18)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll7_unlock_intr_bypass_END (18)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_pcieppll_unlock_intr_bypass_START (19)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_pcieppll_unlock_intr_bypass_END (19)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_spll_unlock_intr_bypass_START (20)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_spll_unlock_intr_bypass_END (20)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_lbintjpll_unlock_intr_bypass_START (21)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_lbintjpll_unlock_intr_bypass_END (21)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll5_unlock_intr_bypass_START (22)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_ppll5_unlock_intr_bypass_END (22)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_aupll_unlock_intr_bypass_START (23)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_aupll_unlock_intr_bypass_END (23)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_fnpll_s_media1_unlock_intr_bypass_START (24)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_BYPSS_LPM3_fnpll_s_media1_unlock_intr_bypass_END (24)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ulppll_unlock_intr_stat : 1;
        unsigned int ulppll_1_unlock_intr_stat : 1;
        unsigned int ppll0_unlock_intr_stat : 1;
        unsigned int apll0_unlock_intr_stat : 1;
        unsigned int apll1_unlock_intr_stat : 1;
        unsigned int apll2_unlock_intr_stat : 1;
        unsigned int apll3_unlock_intr_stat : 1;
        unsigned int scpll_s_media_unlock_intr_stat : 1;
        unsigned int apll5_unlock_intr_stat : 1;
        unsigned int ppll2_b_unlock_intr_stat : 1;
        unsigned int ppll1_unlock_intr_stat : 1;
        unsigned int ppll2_unlock_intr_stat : 1;
        unsigned int ppll3_unlock_intr_stat : 1;
        unsigned int apll7_unlock_intr_stat : 1;
        unsigned int pcie1pll_fn_unlock_intr_stat : 1;
        unsigned int pcie1pll_sc_unlock_intr_stat : 1;
        unsigned int pcie_fnpll_unlock_intr_stat : 1;
        unsigned int ppll6_unlock_intr_stat : 1;
        unsigned int ppll7_unlock_intr_stat : 1;
        unsigned int pcieppll_unlock_intr_stat : 1;
        unsigned int spll_unlock_intr_stat : 1;
        unsigned int lbintjpll_unlock_intr_stat : 1;
        unsigned int ppll5_unlock_intr_stat : 1;
        unsigned int aupll_unlock_intr_stat : 1;
        unsigned int fnpll_s_media1_unlock_intr_stat : 1;
        unsigned int reserved : 7;
    } reg;
} SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_UNION;
#endif
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ulppll_unlock_intr_stat_START (0)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ulppll_unlock_intr_stat_END (0)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ulppll_1_unlock_intr_stat_START (1)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ulppll_1_unlock_intr_stat_END (1)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll0_unlock_intr_stat_START (2)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll0_unlock_intr_stat_END (2)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_apll0_unlock_intr_stat_START (3)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_apll0_unlock_intr_stat_END (3)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_apll1_unlock_intr_stat_START (4)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_apll1_unlock_intr_stat_END (4)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_apll2_unlock_intr_stat_START (5)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_apll2_unlock_intr_stat_END (5)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_apll3_unlock_intr_stat_START (6)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_apll3_unlock_intr_stat_END (6)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_scpll_s_media_unlock_intr_stat_START (7)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_scpll_s_media_unlock_intr_stat_END (7)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_apll5_unlock_intr_stat_START (8)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_apll5_unlock_intr_stat_END (8)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll2_b_unlock_intr_stat_START (9)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll2_b_unlock_intr_stat_END (9)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll1_unlock_intr_stat_START (10)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll1_unlock_intr_stat_END (10)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll2_unlock_intr_stat_START (11)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll2_unlock_intr_stat_END (11)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll3_unlock_intr_stat_START (12)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll3_unlock_intr_stat_END (12)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_apll7_unlock_intr_stat_START (13)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_apll7_unlock_intr_stat_END (13)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_pcie1pll_fn_unlock_intr_stat_START (14)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_pcie1pll_fn_unlock_intr_stat_END (14)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_pcie1pll_sc_unlock_intr_stat_START (15)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_pcie1pll_sc_unlock_intr_stat_END (15)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_pcie_fnpll_unlock_intr_stat_START (16)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_pcie_fnpll_unlock_intr_stat_END (16)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll6_unlock_intr_stat_START (17)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll6_unlock_intr_stat_END (17)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll7_unlock_intr_stat_START (18)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll7_unlock_intr_stat_END (18)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_pcieppll_unlock_intr_stat_START (19)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_pcieppll_unlock_intr_stat_END (19)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_spll_unlock_intr_stat_START (20)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_spll_unlock_intr_stat_END (20)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_lbintjpll_unlock_intr_stat_START (21)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_lbintjpll_unlock_intr_stat_END (21)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll5_unlock_intr_stat_START (22)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_ppll5_unlock_intr_stat_END (22)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_aupll_unlock_intr_stat_START (23)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_aupll_unlock_intr_stat_END (23)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_fnpll_s_media1_unlock_intr_stat_START (24)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECT_INTR_STAT_fnpll_s_media1_unlock_intr_stat_END (24)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ulppll_unlock_intr_cnt : 4;
        unsigned int ulppll_1_unlock_intr_cnt : 4;
        unsigned int ppll0_unlock_intr_cnt : 4;
        unsigned int apll0_unlock_intr_cnt : 4;
        unsigned int apll1_unlock_intr_cnt : 4;
        unsigned int apll2_unlock_intr_cnt : 4;
        unsigned int apll3_unlock_intr_cnt : 4;
        unsigned int scpll_s_media_unlock_intr_cnt : 4;
    } reg;
} SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_UNION;
#endif
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_ulppll_unlock_intr_cnt_START (0)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_ulppll_unlock_intr_cnt_END (3)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_ulppll_1_unlock_intr_cnt_START (4)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_ulppll_1_unlock_intr_cnt_END (7)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_ppll0_unlock_intr_cnt_START (8)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_ppll0_unlock_intr_cnt_END (11)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_apll0_unlock_intr_cnt_START (12)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_apll0_unlock_intr_cnt_END (15)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_apll1_unlock_intr_cnt_START (16)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_apll1_unlock_intr_cnt_END (19)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_apll2_unlock_intr_cnt_START (20)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_apll2_unlock_intr_cnt_END (23)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_apll3_unlock_intr_cnt_START (24)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_apll3_unlock_intr_cnt_END (27)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_scpll_s_media_unlock_intr_cnt_START (28)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT0_scpll_s_media_unlock_intr_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int apll5_unlock_intr_cnt : 4;
        unsigned int ppll2_b_unlock_intr_cnt : 4;
        unsigned int ppll1_unlock_intr_cnt : 4;
        unsigned int ppll2_unlock_intr_cnt : 4;
        unsigned int ppll3_unlock_intr_cnt : 4;
        unsigned int apll7_unlock_intr_cnt : 4;
        unsigned int pcie1pll_fn_unlock_intr_cnt : 4;
        unsigned int pcie1pll_sc_unlock_intr_cnt : 4;
    } reg;
} SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_UNION;
#endif
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_apll5_unlock_intr_cnt_START (0)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_apll5_unlock_intr_cnt_END (3)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_ppll2_b_unlock_intr_cnt_START (4)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_ppll2_b_unlock_intr_cnt_END (7)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_ppll1_unlock_intr_cnt_START (8)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_ppll1_unlock_intr_cnt_END (11)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_ppll2_unlock_intr_cnt_START (12)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_ppll2_unlock_intr_cnt_END (15)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_ppll3_unlock_intr_cnt_START (16)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_ppll3_unlock_intr_cnt_END (19)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_apll7_unlock_intr_cnt_START (20)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_apll7_unlock_intr_cnt_END (23)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_pcie1pll_fn_unlock_intr_cnt_START (24)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_pcie1pll_fn_unlock_intr_cnt_END (27)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_pcie1pll_sc_unlock_intr_cnt_START (28)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT1_pcie1pll_sc_unlock_intr_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcie_fnpll_unlock_intr_cnt : 4;
        unsigned int ppll6_unlock_intr_cnt : 4;
        unsigned int ppll7_unlock_intr_cnt : 4;
        unsigned int pcieppll_unlock_intr_cnt : 4;
        unsigned int spll_unlock_intr_cnt : 4;
        unsigned int lbintjpll_unlock_intr_cnt : 4;
        unsigned int ppll5_unlock_intr_cnt : 4;
        unsigned int aupll_unlock_intr_cnt : 4;
    } reg;
} SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_UNION;
#endif
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_pcie_fnpll_unlock_intr_cnt_START (0)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_pcie_fnpll_unlock_intr_cnt_END (3)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_ppll6_unlock_intr_cnt_START (4)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_ppll6_unlock_intr_cnt_END (7)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_ppll7_unlock_intr_cnt_START (8)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_ppll7_unlock_intr_cnt_END (11)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_pcieppll_unlock_intr_cnt_START (12)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_pcieppll_unlock_intr_cnt_END (15)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_spll_unlock_intr_cnt_START (16)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_spll_unlock_intr_cnt_END (19)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_lbintjpll_unlock_intr_cnt_START (20)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_lbintjpll_unlock_intr_cnt_END (23)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_ppll5_unlock_intr_cnt_START (24)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_ppll5_unlock_intr_cnt_END (27)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_aupll_unlock_intr_cnt_START (28)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT2_aupll_unlock_intr_cnt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fnpll_s_media1_unlock_intr_cnt : 4;
        unsigned int reserved_0 : 4;
        unsigned int reserved_1 : 4;
        unsigned int reserved_2 : 4;
        unsigned int reserved_3 : 4;
        unsigned int reserved_4 : 4;
        unsigned int reserved_5 : 4;
        unsigned int reserved_6 : 4;
    } reg;
} SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT3_UNION;
#endif
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT3_fnpll_s_media1_unlock_intr_cnt_START (0)
#define SOC_CRGPERIPH_PLL_UNLOCK_DETECTCNT3_fnpll_s_media1_unlock_intr_cnt_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll0_gt_stat : 1;
        unsigned int ppll1_gt_stat : 1;
        unsigned int ppll2_gt_stat : 1;
        unsigned int ppll2_b_gt_stat : 1;
        unsigned int ppll3_gt_stat : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ppll7_gt_stat : 1;
        unsigned int ppll0_bypass : 1;
        unsigned int ppll1_bypass : 1;
        unsigned int ppll2_bypass : 1;
        unsigned int ppll2_b_bypass : 1;
        unsigned int ppll3_bypass : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int ppll7_bypass : 1;
        unsigned int ppll0_en_stat : 1;
        unsigned int ppll1_en_stat : 1;
        unsigned int ppll2_en_stat : 1;
        unsigned int ppll2_b_en_stat : 1;
        unsigned int ppll3_en_stat : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int ppll7_en_stat : 1;
        unsigned int reserved_9 : 5;
    } reg;
} SOC_CRGPERIPH_PLL_VOTE_STAT0_UNION;
#endif
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll0_gt_stat_START (0)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll0_gt_stat_END (0)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll1_gt_stat_START (1)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll1_gt_stat_END (1)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_gt_stat_START (2)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_gt_stat_END (2)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_b_gt_stat_START (3)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_b_gt_stat_END (3)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll3_gt_stat_START (4)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll3_gt_stat_END (4)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll7_gt_stat_START (8)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll7_gt_stat_END (8)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll0_bypass_START (9)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll0_bypass_END (9)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll1_bypass_START (10)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll1_bypass_END (10)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_bypass_START (11)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_bypass_END (11)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_b_bypass_START (12)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_b_bypass_END (12)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll3_bypass_START (13)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll3_bypass_END (13)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll7_bypass_START (17)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll7_bypass_END (17)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll0_en_stat_START (18)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll0_en_stat_END (18)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll1_en_stat_START (19)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll1_en_stat_END (19)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_en_stat_START (20)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_en_stat_END (20)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_b_en_stat_START (21)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_b_en_stat_END (21)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll3_en_stat_START (22)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll3_en_stat_END (22)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll7_en_stat_START (26)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll7_en_stat_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll1_en_vote : 5;
        unsigned int ppll2_en_vote : 5;
        unsigned int ppll3_en_vote : 5;
        unsigned int reserved : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PLL_EN_VOTE_CTRL0_UNION;
#endif
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL0_ppll1_en_vote_START (0)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL0_ppll1_en_vote_END (4)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL0_ppll2_en_vote_START (5)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL0_ppll2_en_vote_END (9)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL0_ppll3_en_vote_START (10)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL0_ppll3_en_vote_END (14)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL0_bitmasken_START (16)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll1_gt_vote : 5;
        unsigned int ppll2_gt_vote : 5;
        unsigned int ppll3_gt_vote : 5;
        unsigned int reserved : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PLL_GT_VOTE_CTRL0_UNION;
#endif
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL0_ppll1_gt_vote_START (0)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL0_ppll1_gt_vote_END (4)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL0_ppll2_gt_vote_START (5)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL0_ppll2_gt_vote_END (9)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL0_ppll3_gt_vote_START (10)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL0_ppll3_gt_vote_END (14)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL0_bitmasken_START (16)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll1_bypass_vote : 5;
        unsigned int ppll2_bypass_vote : 5;
        unsigned int ppll3_bypass_vote : 5;
        unsigned int reserved : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL0_UNION;
#endif
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL0_ppll1_bypass_vote_START (0)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL0_ppll1_bypass_vote_END (4)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL0_ppll2_bypass_vote_START (5)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL0_ppll2_bypass_vote_END (9)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL0_ppll3_bypass_vote_START (10)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL0_ppll3_bypass_vote_END (14)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL0_bitmasken_START (16)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 5;
        unsigned int reserved_1 : 5;
        unsigned int ppll7_en_vote : 5;
        unsigned int reserved_2 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_ppll7_en_vote_START (10)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_ppll7_en_vote_END (14)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_bitmasken_START (16)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 5;
        unsigned int reserved_1 : 5;
        unsigned int ppll7_gt_vote : 5;
        unsigned int reserved_2 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PLL_GT_VOTE_CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL1_ppll7_gt_vote_START (10)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL1_ppll7_gt_vote_END (14)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL1_bitmasken_START (16)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL1_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 5;
        unsigned int reserved_1 : 5;
        unsigned int ppll7_bypass_vote : 5;
        unsigned int reserved_2 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL1_ppll7_bypass_vote_START (10)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL1_ppll7_bypass_vote_END (14)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL1_bitmasken_START (16)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL1_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll0_en_vote : 5;
        unsigned int reserved_0 : 5;
        unsigned int ppll2_b_en_vote : 5;
        unsigned int reserved_1 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_UNION;
#endif
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_ppll0_en_vote_START (0)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_ppll0_en_vote_END (4)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_ppll2_b_en_vote_START (10)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_ppll2_b_en_vote_END (14)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_bitmasken_START (16)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll0_gt_vote : 5;
        unsigned int reserved_0 : 5;
        unsigned int ppll2_b_gt_vote : 5;
        unsigned int reserved_1 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PLL_GT_VOTE_CTRL2_UNION;
#endif
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL2_ppll0_gt_vote_START (0)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL2_ppll0_gt_vote_END (4)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL2_ppll2_b_gt_vote_START (10)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL2_ppll2_b_gt_vote_END (14)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL2_bitmasken_START (16)
#define SOC_CRGPERIPH_PLL_GT_VOTE_CTRL2_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll0_bypass_vote : 5;
        unsigned int reserved_0 : 5;
        unsigned int ppll2_b_bypass_vote : 5;
        unsigned int reserved_1 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL2_UNION;
#endif
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL2_ppll0_bypass_vote_START (0)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL2_ppll0_bypass_vote_END (4)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL2_ppll2_b_bypass_vote_START (10)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL2_ppll2_b_bypass_vote_END (14)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL2_bitmasken_START (16)
#define SOC_CRGPERIPH_PLL_BYPASS_VOTE_CTRL2_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int peri_pll_stat_sel : 4;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 11;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_PERI_FNPLL_STAT_SEL_UNION;
#endif
#define SOC_CRGPERIPH_PERI_FNPLL_STAT_SEL_peri_pll_stat_sel_START (0)
#define SOC_CRGPERIPH_PERI_FNPLL_STAT_SEL_peri_pll_stat_sel_END (3)
#define SOC_CRGPERIPH_PERI_FNPLL_STAT_SEL_bitmasken_START (16)
#define SOC_CRGPERIPH_PERI_FNPLL_STAT_SEL_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int phe_sel_en_fnpll_1v2 : 1;
        unsigned int dtc_test_fnpll_1v2 : 1;
        unsigned int sw_dc_buf_en : 1;
        unsigned int sw_ac_buf_en : 1;
        unsigned int phe_sel_in_fnpll_1v2 : 2;
        unsigned int unlock_clear : 1;
        unsigned int dc_ac_clk_en : 1;
        unsigned int dtc_m_cfg_fnpll_1v2 : 6;
        unsigned int ref_lost_clear_fnpll_1v2 : 1;
        unsigned int reserved : 1;
        unsigned int spread_en_fnpll_1v2 : 1;
        unsigned int downspread_fnpll_1v2 : 1;
        unsigned int sel_extwave_fnpll_1v2 : 1;
        unsigned int divval_fnpll_1v2 : 4;
        unsigned int spread_fnpll_1v2 : 3;
        unsigned int freq_threshold : 6;
    } reg;
} SOC_CRGPERIPH_FNPLL_CFG0_UNION;
#endif
#define SOC_CRGPERIPH_FNPLL_CFG0_phe_sel_en_fnpll_1v2_START (0)
#define SOC_CRGPERIPH_FNPLL_CFG0_phe_sel_en_fnpll_1v2_END (0)
#define SOC_CRGPERIPH_FNPLL_CFG0_dtc_test_fnpll_1v2_START (1)
#define SOC_CRGPERIPH_FNPLL_CFG0_dtc_test_fnpll_1v2_END (1)
#define SOC_CRGPERIPH_FNPLL_CFG0_sw_dc_buf_en_START (2)
#define SOC_CRGPERIPH_FNPLL_CFG0_sw_dc_buf_en_END (2)
#define SOC_CRGPERIPH_FNPLL_CFG0_sw_ac_buf_en_START (3)
#define SOC_CRGPERIPH_FNPLL_CFG0_sw_ac_buf_en_END (3)
#define SOC_CRGPERIPH_FNPLL_CFG0_phe_sel_in_fnpll_1v2_START (4)
#define SOC_CRGPERIPH_FNPLL_CFG0_phe_sel_in_fnpll_1v2_END (5)
#define SOC_CRGPERIPH_FNPLL_CFG0_unlock_clear_START (6)
#define SOC_CRGPERIPH_FNPLL_CFG0_unlock_clear_END (6)
#define SOC_CRGPERIPH_FNPLL_CFG0_dc_ac_clk_en_START (7)
#define SOC_CRGPERIPH_FNPLL_CFG0_dc_ac_clk_en_END (7)
#define SOC_CRGPERIPH_FNPLL_CFG0_dtc_m_cfg_fnpll_1v2_START (8)
#define SOC_CRGPERIPH_FNPLL_CFG0_dtc_m_cfg_fnpll_1v2_END (13)
#define SOC_CRGPERIPH_FNPLL_CFG0_ref_lost_clear_fnpll_1v2_START (14)
#define SOC_CRGPERIPH_FNPLL_CFG0_ref_lost_clear_fnpll_1v2_END (14)
#define SOC_CRGPERIPH_FNPLL_CFG0_spread_en_fnpll_1v2_START (16)
#define SOC_CRGPERIPH_FNPLL_CFG0_spread_en_fnpll_1v2_END (16)
#define SOC_CRGPERIPH_FNPLL_CFG0_downspread_fnpll_1v2_START (17)
#define SOC_CRGPERIPH_FNPLL_CFG0_downspread_fnpll_1v2_END (17)
#define SOC_CRGPERIPH_FNPLL_CFG0_sel_extwave_fnpll_1v2_START (18)
#define SOC_CRGPERIPH_FNPLL_CFG0_sel_extwave_fnpll_1v2_END (18)
#define SOC_CRGPERIPH_FNPLL_CFG0_divval_fnpll_1v2_START (19)
#define SOC_CRGPERIPH_FNPLL_CFG0_divval_fnpll_1v2_END (22)
#define SOC_CRGPERIPH_FNPLL_CFG0_spread_fnpll_1v2_START (23)
#define SOC_CRGPERIPH_FNPLL_CFG0_spread_fnpll_1v2_END (25)
#define SOC_CRGPERIPH_FNPLL_CFG0_freq_threshold_START (26)
#define SOC_CRGPERIPH_FNPLL_CFG0_freq_threshold_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ext_maxaddr_fnpll_1v2 : 8;
        unsigned int extwaveval_fnpll_1v2 : 8;
        unsigned int input_cfg_en_fnpll_1v2 : 1;
        unsigned int test_mode_fnpll_1v2 : 1;
        unsigned int lock_thr_fnpll_1v2 : 2;
        unsigned int refdiv_rst_n_fnpll_1v2 : 1;
        unsigned int fbdiv_rst_n_fnpll_1v2 : 1;
        unsigned int div2_pd_fnpll_1v2 : 1;
        unsigned int lock_en_fnpll_1v2 : 1;
        unsigned int fbdiv_delay_num_fnpll_1v2 : 7;
        unsigned int reserved : 1;
    } reg;
} SOC_CRGPERIPH_FNPLL_CFG1_UNION;
#endif
#define SOC_CRGPERIPH_FNPLL_CFG1_ext_maxaddr_fnpll_1v2_START (0)
#define SOC_CRGPERIPH_FNPLL_CFG1_ext_maxaddr_fnpll_1v2_END (7)
#define SOC_CRGPERIPH_FNPLL_CFG1_extwaveval_fnpll_1v2_START (8)
#define SOC_CRGPERIPH_FNPLL_CFG1_extwaveval_fnpll_1v2_END (15)
#define SOC_CRGPERIPH_FNPLL_CFG1_input_cfg_en_fnpll_1v2_START (16)
#define SOC_CRGPERIPH_FNPLL_CFG1_input_cfg_en_fnpll_1v2_END (16)
#define SOC_CRGPERIPH_FNPLL_CFG1_test_mode_fnpll_1v2_START (17)
#define SOC_CRGPERIPH_FNPLL_CFG1_test_mode_fnpll_1v2_END (17)
#define SOC_CRGPERIPH_FNPLL_CFG1_lock_thr_fnpll_1v2_START (18)
#define SOC_CRGPERIPH_FNPLL_CFG1_lock_thr_fnpll_1v2_END (19)
#define SOC_CRGPERIPH_FNPLL_CFG1_refdiv_rst_n_fnpll_1v2_START (20)
#define SOC_CRGPERIPH_FNPLL_CFG1_refdiv_rst_n_fnpll_1v2_END (20)
#define SOC_CRGPERIPH_FNPLL_CFG1_fbdiv_rst_n_fnpll_1v2_START (21)
#define SOC_CRGPERIPH_FNPLL_CFG1_fbdiv_rst_n_fnpll_1v2_END (21)
#define SOC_CRGPERIPH_FNPLL_CFG1_div2_pd_fnpll_1v2_START (22)
#define SOC_CRGPERIPH_FNPLL_CFG1_div2_pd_fnpll_1v2_END (22)
#define SOC_CRGPERIPH_FNPLL_CFG1_lock_en_fnpll_1v2_START (23)
#define SOC_CRGPERIPH_FNPLL_CFG1_lock_en_fnpll_1v2_END (23)
#define SOC_CRGPERIPH_FNPLL_CFG1_fbdiv_delay_num_fnpll_1v2_START (24)
#define SOC_CRGPERIPH_FNPLL_CFG1_fbdiv_delay_num_fnpll_1v2_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bbpd_calib_byp_fnpll_1v2 : 1;
        unsigned int dtc_ctrl_inv_fnpll_1v2 : 1;
        unsigned int dll_force_en_fnpll_1v2 : 1;
        unsigned int DLL_EN_FNPLL_1V2 : 1;
        unsigned int phe_code_a_fnpll_1v2 : 2;
        unsigned int phe_code_b_fnpll_1v2 : 2;
        unsigned int ctinue_lock_num_fnpll_1v2 : 6;
        unsigned int reserved_0 : 2;
        unsigned int read_en_fnpll_1v2 : 1;
        unsigned int k_gain_cfg_en_fnpll_1v2 : 1;
        unsigned int reserved_1 : 2;
        unsigned int k_gain_av_thr_fnpll_1v2 : 3;
        unsigned int pll_unlock_clr_fnpll_1v2 : 1;
        unsigned int k_gain_cfg_fnpll_1v2 : 6;
        unsigned int reserved_2 : 2;
    } reg;
} SOC_CRGPERIPH_FNPLL_CFG2_UNION;
#endif
#define SOC_CRGPERIPH_FNPLL_CFG2_bbpd_calib_byp_fnpll_1v2_START (0)
#define SOC_CRGPERIPH_FNPLL_CFG2_bbpd_calib_byp_fnpll_1v2_END (0)
#define SOC_CRGPERIPH_FNPLL_CFG2_dtc_ctrl_inv_fnpll_1v2_START (1)
#define SOC_CRGPERIPH_FNPLL_CFG2_dtc_ctrl_inv_fnpll_1v2_END (1)
#define SOC_CRGPERIPH_FNPLL_CFG2_dll_force_en_fnpll_1v2_START (2)
#define SOC_CRGPERIPH_FNPLL_CFG2_dll_force_en_fnpll_1v2_END (2)
#define SOC_CRGPERIPH_FNPLL_CFG2_DLL_EN_FNPLL_1V2_START (3)
#define SOC_CRGPERIPH_FNPLL_CFG2_DLL_EN_FNPLL_1V2_END (3)
#define SOC_CRGPERIPH_FNPLL_CFG2_phe_code_a_fnpll_1v2_START (4)
#define SOC_CRGPERIPH_FNPLL_CFG2_phe_code_a_fnpll_1v2_END (5)
#define SOC_CRGPERIPH_FNPLL_CFG2_phe_code_b_fnpll_1v2_START (6)
#define SOC_CRGPERIPH_FNPLL_CFG2_phe_code_b_fnpll_1v2_END (7)
#define SOC_CRGPERIPH_FNPLL_CFG2_ctinue_lock_num_fnpll_1v2_START (8)
#define SOC_CRGPERIPH_FNPLL_CFG2_ctinue_lock_num_fnpll_1v2_END (13)
#define SOC_CRGPERIPH_FNPLL_CFG2_read_en_fnpll_1v2_START (16)
#define SOC_CRGPERIPH_FNPLL_CFG2_read_en_fnpll_1v2_END (16)
#define SOC_CRGPERIPH_FNPLL_CFG2_k_gain_cfg_en_fnpll_1v2_START (17)
#define SOC_CRGPERIPH_FNPLL_CFG2_k_gain_cfg_en_fnpll_1v2_END (17)
#define SOC_CRGPERIPH_FNPLL_CFG2_k_gain_av_thr_fnpll_1v2_START (20)
#define SOC_CRGPERIPH_FNPLL_CFG2_k_gain_av_thr_fnpll_1v2_END (22)
#define SOC_CRGPERIPH_FNPLL_CFG2_pll_unlock_clr_fnpll_1v2_START (23)
#define SOC_CRGPERIPH_FNPLL_CFG2_pll_unlock_clr_fnpll_1v2_END (23)
#define SOC_CRGPERIPH_FNPLL_CFG2_k_gain_cfg_fnpll_1v2_START (24)
#define SOC_CRGPERIPH_FNPLL_CFG2_k_gain_cfg_fnpll_1v2_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dtc_o_cfg_fnpll_1v2 : 6;
        unsigned int reserved_0 : 2;
        unsigned int bbpd_lock_num_fnpll_1v2 : 8;
        unsigned int lpf_bw_fnpll_1v2 : 3;
        unsigned int reserved_1 : 1;
        unsigned int lpf_bw1_fnpll_1v2 : 3;
        unsigned int reserved_2 : 1;
        unsigned int calib_lpf_bw_fnpll_1v2 : 3;
        unsigned int reserved_3 : 1;
        unsigned int calib_lpf_bw1_fnpll_1v2 : 3;
        unsigned int reserved_4 : 1;
    } reg;
} SOC_CRGPERIPH_FNPLL_CFG3_UNION;
#endif
#define SOC_CRGPERIPH_FNPLL_CFG3_dtc_o_cfg_fnpll_1v2_START (0)
#define SOC_CRGPERIPH_FNPLL_CFG3_dtc_o_cfg_fnpll_1v2_END (5)
#define SOC_CRGPERIPH_FNPLL_CFG3_bbpd_lock_num_fnpll_1v2_START (8)
#define SOC_CRGPERIPH_FNPLL_CFG3_bbpd_lock_num_fnpll_1v2_END (15)
#define SOC_CRGPERIPH_FNPLL_CFG3_lpf_bw_fnpll_1v2_START (16)
#define SOC_CRGPERIPH_FNPLL_CFG3_lpf_bw_fnpll_1v2_END (18)
#define SOC_CRGPERIPH_FNPLL_CFG3_lpf_bw1_fnpll_1v2_START (20)
#define SOC_CRGPERIPH_FNPLL_CFG3_lpf_bw1_fnpll_1v2_END (22)
#define SOC_CRGPERIPH_FNPLL_CFG3_calib_lpf_bw_fnpll_1v2_START (24)
#define SOC_CRGPERIPH_FNPLL_CFG3_calib_lpf_bw_fnpll_1v2_END (26)
#define SOC_CRGPERIPH_FNPLL_CFG3_calib_lpf_bw1_fnpll_1v2_START (28)
#define SOC_CRGPERIPH_FNPLL_CFG3_calib_lpf_bw1_fnpll_1v2_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int max_k_gain_fnpll_1v2 : 6;
        unsigned int reserved_0 : 2;
        unsigned int min_k_gain_fnpll_1v2 : 6;
        unsigned int reserved_1 : 2;
        unsigned int en_dac_test_fnpll_1v2 : 1;
        unsigned int updn_sel_fnpll_1v2 : 1;
        unsigned int icp_ctrl_fnpll_1v2 : 1;
        unsigned int reserved_2 : 1;
        unsigned int vdc_sel_fnpll_1v2 : 4;
        unsigned int reserved_3 : 4;
        unsigned int test_data_sel_fnpll_1v2 : 4;
    } reg;
} SOC_CRGPERIPH_FNPLL_CFG4_UNION;
#endif
#define SOC_CRGPERIPH_FNPLL_CFG4_max_k_gain_fnpll_1v2_START (0)
#define SOC_CRGPERIPH_FNPLL_CFG4_max_k_gain_fnpll_1v2_END (5)
#define SOC_CRGPERIPH_FNPLL_CFG4_min_k_gain_fnpll_1v2_START (8)
#define SOC_CRGPERIPH_FNPLL_CFG4_min_k_gain_fnpll_1v2_END (13)
#define SOC_CRGPERIPH_FNPLL_CFG4_en_dac_test_fnpll_1v2_START (16)
#define SOC_CRGPERIPH_FNPLL_CFG4_en_dac_test_fnpll_1v2_END (16)
#define SOC_CRGPERIPH_FNPLL_CFG4_updn_sel_fnpll_1v2_START (17)
#define SOC_CRGPERIPH_FNPLL_CFG4_updn_sel_fnpll_1v2_END (17)
#define SOC_CRGPERIPH_FNPLL_CFG4_icp_ctrl_fnpll_1v2_START (18)
#define SOC_CRGPERIPH_FNPLL_CFG4_icp_ctrl_fnpll_1v2_END (18)
#define SOC_CRGPERIPH_FNPLL_CFG4_vdc_sel_fnpll_1v2_START (20)
#define SOC_CRGPERIPH_FNPLL_CFG4_vdc_sel_fnpll_1v2_END (23)
#define SOC_CRGPERIPH_FNPLL_CFG4_test_data_sel_fnpll_1v2_START (28)
#define SOC_CRGPERIPH_FNPLL_CFG4_test_data_sel_fnpll_1v2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ref_lost_thr_fnpll_1v2 : 8;
        unsigned int pfd_div_ratio : 4;
        unsigned int reserved : 20;
    } reg;
} SOC_CRGPERIPH_FNPLL_CFG5_UNION;
#endif
#define SOC_CRGPERIPH_FNPLL_CFG5_ref_lost_thr_fnpll_1v2_START (0)
#define SOC_CRGPERIPH_FNPLL_CFG5_ref_lost_thr_fnpll_1v2_END (7)
#define SOC_CRGPERIPH_FNPLL_CFG5_pfd_div_ratio_START (8)
#define SOC_CRGPERIPH_FNPLL_CFG5_pfd_div_ratio_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 11;
        unsigned int div2_pd_test_fnpll_1v2 : 1;
        unsigned int fbdiv_rst_n_test_fnpll_1v2 : 1;
        unsigned int refdiv_rst_n_test_fnpll_1v2 : 1;
        unsigned int ac_buf_pd : 1;
        unsigned int dc_buf_pd : 1;
        unsigned int pll_lock_fnpll_1v2 : 1;
        unsigned int pll_lock_ate_fnpll_1v2 : 1;
        unsigned int bbpd_calib_done_fnpll_1v2 : 1;
        unsigned int bbpd_calib_fail_fnpll_1v2 : 1;
        unsigned int bbpd_calib_success_fnpll_1v2 : 1;
        unsigned int pll_unlock_fnpll_1v2 : 1;
        unsigned int clock_lost_fnpll_1v2 : 1;
        unsigned int ref_lost_flag_fnpll_1v2 : 1;
        unsigned int test_data_fnpll_1v2 : 8;
    } reg;
} SOC_CRGPERIPH_FNPLL_STATE0_UNION;
#endif
#define SOC_CRGPERIPH_FNPLL_STATE0_div2_pd_test_fnpll_1v2_START (11)
#define SOC_CRGPERIPH_FNPLL_STATE0_div2_pd_test_fnpll_1v2_END (11)
#define SOC_CRGPERIPH_FNPLL_STATE0_fbdiv_rst_n_test_fnpll_1v2_START (12)
#define SOC_CRGPERIPH_FNPLL_STATE0_fbdiv_rst_n_test_fnpll_1v2_END (12)
#define SOC_CRGPERIPH_FNPLL_STATE0_refdiv_rst_n_test_fnpll_1v2_START (13)
#define SOC_CRGPERIPH_FNPLL_STATE0_refdiv_rst_n_test_fnpll_1v2_END (13)
#define SOC_CRGPERIPH_FNPLL_STATE0_ac_buf_pd_START (14)
#define SOC_CRGPERIPH_FNPLL_STATE0_ac_buf_pd_END (14)
#define SOC_CRGPERIPH_FNPLL_STATE0_dc_buf_pd_START (15)
#define SOC_CRGPERIPH_FNPLL_STATE0_dc_buf_pd_END (15)
#define SOC_CRGPERIPH_FNPLL_STATE0_pll_lock_fnpll_1v2_START (16)
#define SOC_CRGPERIPH_FNPLL_STATE0_pll_lock_fnpll_1v2_END (16)
#define SOC_CRGPERIPH_FNPLL_STATE0_pll_lock_ate_fnpll_1v2_START (17)
#define SOC_CRGPERIPH_FNPLL_STATE0_pll_lock_ate_fnpll_1v2_END (17)
#define SOC_CRGPERIPH_FNPLL_STATE0_bbpd_calib_done_fnpll_1v2_START (18)
#define SOC_CRGPERIPH_FNPLL_STATE0_bbpd_calib_done_fnpll_1v2_END (18)
#define SOC_CRGPERIPH_FNPLL_STATE0_bbpd_calib_fail_fnpll_1v2_START (19)
#define SOC_CRGPERIPH_FNPLL_STATE0_bbpd_calib_fail_fnpll_1v2_END (19)
#define SOC_CRGPERIPH_FNPLL_STATE0_bbpd_calib_success_fnpll_1v2_START (20)
#define SOC_CRGPERIPH_FNPLL_STATE0_bbpd_calib_success_fnpll_1v2_END (20)
#define SOC_CRGPERIPH_FNPLL_STATE0_pll_unlock_fnpll_1v2_START (21)
#define SOC_CRGPERIPH_FNPLL_STATE0_pll_unlock_fnpll_1v2_END (21)
#define SOC_CRGPERIPH_FNPLL_STATE0_clock_lost_fnpll_1v2_START (22)
#define SOC_CRGPERIPH_FNPLL_STATE0_clock_lost_fnpll_1v2_END (22)
#define SOC_CRGPERIPH_FNPLL_STATE0_ref_lost_flag_fnpll_1v2_START (23)
#define SOC_CRGPERIPH_FNPLL_STATE0_ref_lost_flag_fnpll_1v2_END (23)
#define SOC_CRGPERIPH_FNPLL_STATE0_test_data_fnpll_1v2_START (24)
#define SOC_CRGPERIPH_FNPLL_STATE0_test_data_fnpll_1v2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 5;
        unsigned int unlock_count_fnpll_1v2 : 4;
        unsigned int unlock_flag_fnpll_1v2 : 1;
        unsigned int reserved_1 : 2;
        unsigned int lock_counter_fnpll_1v2 : 15;
        unsigned int reserved_2 : 5;
    } reg;
} SOC_CRGPERIPH_FNPLL_STATE1_UNION;
#endif
#define SOC_CRGPERIPH_FNPLL_STATE1_unlock_count_fnpll_1v2_START (5)
#define SOC_CRGPERIPH_FNPLL_STATE1_unlock_count_fnpll_1v2_END (8)
#define SOC_CRGPERIPH_FNPLL_STATE1_unlock_flag_fnpll_1v2_START (9)
#define SOC_CRGPERIPH_FNPLL_STATE1_unlock_flag_fnpll_1v2_END (9)
#define SOC_CRGPERIPH_FNPLL_STATE1_lock_counter_fnpll_1v2_START (12)
#define SOC_CRGPERIPH_FNPLL_STATE1_lock_counter_fnpll_1v2_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_hisee : 1;
        unsigned int ip_rst_hsdt : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_CRGPERIPH_RSTEN_HISEE_SEC_UNION;
#endif
#define SOC_CRGPERIPH_RSTEN_HISEE_SEC_ip_rst_hisee_START (0)
#define SOC_CRGPERIPH_RSTEN_HISEE_SEC_ip_rst_hisee_END (0)
#define SOC_CRGPERIPH_RSTEN_HISEE_SEC_ip_rst_hsdt_START (1)
#define SOC_CRGPERIPH_RSTEN_HISEE_SEC_ip_rst_hsdt_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_hisee : 1;
        unsigned int ip_rst_hsdt : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_CRGPERIPH_RSTDIS_HISEE_SEC_UNION;
#endif
#define SOC_CRGPERIPH_RSTDIS_HISEE_SEC_ip_rst_hisee_START (0)
#define SOC_CRGPERIPH_RSTDIS_HISEE_SEC_ip_rst_hisee_END (0)
#define SOC_CRGPERIPH_RSTDIS_HISEE_SEC_ip_rst_hsdt_START (1)
#define SOC_CRGPERIPH_RSTDIS_HISEE_SEC_ip_rst_hsdt_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_hisee : 1;
        unsigned int ip_rst_hsdt : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_CRGPERIPH_RSTSTAT_HISEE_SEC_UNION;
#endif
#define SOC_CRGPERIPH_RSTSTAT_HISEE_SEC_ip_rst_hisee_START (0)
#define SOC_CRGPERIPH_RSTSTAT_HISEE_SEC_ip_rst_hisee_END (0)
#define SOC_CRGPERIPH_RSTSTAT_HISEE_SEC_ip_rst_hsdt_START (1)
#define SOC_CRGPERIPH_RSTSTAT_HISEE_SEC_ip_rst_hsdt_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_hisee_pll : 1;
        unsigned int gt_clk_hisee_sys : 1;
        unsigned int gt_clk_noc_hiseebus : 1;
        unsigned int gt_clk_hisee_ref : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_PEREN_HISEE_SEC_UNION;
#endif
#define SOC_CRGPERIPH_PEREN_HISEE_SEC_gt_clk_hisee_pll_START (0)
#define SOC_CRGPERIPH_PEREN_HISEE_SEC_gt_clk_hisee_pll_END (0)
#define SOC_CRGPERIPH_PEREN_HISEE_SEC_gt_clk_hisee_sys_START (1)
#define SOC_CRGPERIPH_PEREN_HISEE_SEC_gt_clk_hisee_sys_END (1)
#define SOC_CRGPERIPH_PEREN_HISEE_SEC_gt_clk_noc_hiseebus_START (2)
#define SOC_CRGPERIPH_PEREN_HISEE_SEC_gt_clk_noc_hiseebus_END (2)
#define SOC_CRGPERIPH_PEREN_HISEE_SEC_gt_clk_hisee_ref_START (3)
#define SOC_CRGPERIPH_PEREN_HISEE_SEC_gt_clk_hisee_ref_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_hisee_pll : 1;
        unsigned int gt_clk_hisee_sys : 1;
        unsigned int gt_clk_noc_hiseebus : 1;
        unsigned int gt_clk_hisee_ref : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_PERDIS_HISEE_SEC_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS_HISEE_SEC_gt_clk_hisee_pll_START (0)
#define SOC_CRGPERIPH_PERDIS_HISEE_SEC_gt_clk_hisee_pll_END (0)
#define SOC_CRGPERIPH_PERDIS_HISEE_SEC_gt_clk_hisee_sys_START (1)
#define SOC_CRGPERIPH_PERDIS_HISEE_SEC_gt_clk_hisee_sys_END (1)
#define SOC_CRGPERIPH_PERDIS_HISEE_SEC_gt_clk_noc_hiseebus_START (2)
#define SOC_CRGPERIPH_PERDIS_HISEE_SEC_gt_clk_noc_hiseebus_END (2)
#define SOC_CRGPERIPH_PERDIS_HISEE_SEC_gt_clk_hisee_ref_START (3)
#define SOC_CRGPERIPH_PERDIS_HISEE_SEC_gt_clk_hisee_ref_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_hisee_pll : 1;
        unsigned int gt_clk_hisee_sys : 1;
        unsigned int gt_clk_noc_hiseebus : 1;
        unsigned int gt_clk_hisee_ref : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_PERCLKEN_HISEE_SEC_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN_HISEE_SEC_gt_clk_hisee_pll_START (0)
#define SOC_CRGPERIPH_PERCLKEN_HISEE_SEC_gt_clk_hisee_pll_END (0)
#define SOC_CRGPERIPH_PERCLKEN_HISEE_SEC_gt_clk_hisee_sys_START (1)
#define SOC_CRGPERIPH_PERCLKEN_HISEE_SEC_gt_clk_hisee_sys_END (1)
#define SOC_CRGPERIPH_PERCLKEN_HISEE_SEC_gt_clk_noc_hiseebus_START (2)
#define SOC_CRGPERIPH_PERCLKEN_HISEE_SEC_gt_clk_noc_hiseebus_END (2)
#define SOC_CRGPERIPH_PERCLKEN_HISEE_SEC_gt_clk_hisee_ref_START (3)
#define SOC_CRGPERIPH_PERCLKEN_HISEE_SEC_gt_clk_hisee_ref_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_hisee_pll : 1;
        unsigned int st_clk_hisee_sys : 1;
        unsigned int st_clk_noc_hiseebus : 1;
        unsigned int st_clk_hisee_ref : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_PERSTAT_HISEE_SEC_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT_HISEE_SEC_st_clk_hisee_pll_START (0)
#define SOC_CRGPERIPH_PERSTAT_HISEE_SEC_st_clk_hisee_pll_END (0)
#define SOC_CRGPERIPH_PERSTAT_HISEE_SEC_st_clk_hisee_sys_START (1)
#define SOC_CRGPERIPH_PERSTAT_HISEE_SEC_st_clk_hisee_sys_END (1)
#define SOC_CRGPERIPH_PERSTAT_HISEE_SEC_st_clk_noc_hiseebus_START (2)
#define SOC_CRGPERIPH_PERSTAT_HISEE_SEC_st_clk_noc_hiseebus_END (2)
#define SOC_CRGPERIPH_PERSTAT_HISEE_SEC_st_clk_hisee_ref_START (3)
#define SOC_CRGPERIPH_PERSTAT_HISEE_SEC_st_clk_hisee_ref_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_hisee_pll : 6;
        unsigned int sc_gt_clk_hisee_pll : 1;
        unsigned int sel_hisee_pll : 1;
        unsigned int reserved_0 : 6;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV_HISEE_SEC_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV_HISEE_SEC_div_hisee_pll_START (0)
#define SOC_CRGPERIPH_CLKDIV_HISEE_SEC_div_hisee_pll_END (5)
#define SOC_CRGPERIPH_CLKDIV_HISEE_SEC_sc_gt_clk_hisee_pll_START (6)
#define SOC_CRGPERIPH_CLKDIV_HISEE_SEC_sc_gt_clk_hisee_pll_END (6)
#define SOC_CRGPERIPH_CLKDIV_HISEE_SEC_sel_hisee_pll_START (7)
#define SOC_CRGPERIPH_CLKDIV_HISEE_SEC_sel_hisee_pll_END (7)
#define SOC_CRGPERIPH_CLKDIV_HISEE_SEC_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV_HISEE_SEC_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_hisee_flash : 1;
        unsigned int gt_clk_out1 : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 26;
    } reg;
} SOC_CRGPERIPH_PEREN_HISEE_FLASH_UNION;
#endif
#define SOC_CRGPERIPH_PEREN_HISEE_FLASH_gt_clk_hisee_flash_START (0)
#define SOC_CRGPERIPH_PEREN_HISEE_FLASH_gt_clk_hisee_flash_END (0)
#define SOC_CRGPERIPH_PEREN_HISEE_FLASH_gt_clk_out1_START (1)
#define SOC_CRGPERIPH_PEREN_HISEE_FLASH_gt_clk_out1_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_hisee_flash : 1;
        unsigned int gt_clk_out1 : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 26;
    } reg;
} SOC_CRGPERIPH_PERDIS_HISEE_FLASH_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS_HISEE_FLASH_gt_clk_hisee_flash_START (0)
#define SOC_CRGPERIPH_PERDIS_HISEE_FLASH_gt_clk_hisee_flash_END (0)
#define SOC_CRGPERIPH_PERDIS_HISEE_FLASH_gt_clk_out1_START (1)
#define SOC_CRGPERIPH_PERDIS_HISEE_FLASH_gt_clk_out1_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_hisee_flash : 1;
        unsigned int gt_clk_out1 : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 26;
    } reg;
} SOC_CRGPERIPH_PERCLKEN_HISEE_FLASH_UNION;
#endif
#define SOC_CRGPERIPH_PERCLKEN_HISEE_FLASH_gt_clk_hisee_flash_START (0)
#define SOC_CRGPERIPH_PERCLKEN_HISEE_FLASH_gt_clk_hisee_flash_END (0)
#define SOC_CRGPERIPH_PERCLKEN_HISEE_FLASH_gt_clk_out1_START (1)
#define SOC_CRGPERIPH_PERCLKEN_HISEE_FLASH_gt_clk_out1_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_hisee_flash : 1;
        unsigned int st_clk_out1 : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 26;
    } reg;
} SOC_CRGPERIPH_PERSTAT_HISEE_FLASH_UNION;
#endif
#define SOC_CRGPERIPH_PERSTAT_HISEE_FLASH_st_clk_hisee_flash_START (0)
#define SOC_CRGPERIPH_PERSTAT_HISEE_FLASH_st_clk_hisee_flash_END (0)
#define SOC_CRGPERIPH_PERSTAT_HISEE_FLASH_st_clk_out1_START (1)
#define SOC_CRGPERIPH_PERSTAT_HISEE_FLASH_st_clk_out1_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_hisee_flash : 6;
        unsigned int sc_gt_clk_hisee_flash : 1;
        unsigned int reserved : 9;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV0_HISEE_FLASH_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV0_HISEE_FLASH_div_hisee_flash_START (0)
#define SOC_CRGPERIPH_CLKDIV0_HISEE_FLASH_div_hisee_flash_END (5)
#define SOC_CRGPERIPH_CLKDIV0_HISEE_FLASH_sc_gt_clk_hisee_flash_START (6)
#define SOC_CRGPERIPH_CLKDIV0_HISEE_FLASH_sc_gt_clk_hisee_flash_END (6)
#define SOC_CRGPERIPH_CLKDIV0_HISEE_FLASH_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV0_HISEE_FLASH_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sel_clk_hisee_flash : 1;
        unsigned int reserved_0 : 4;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 6;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 3;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV1_HISEE_FLASH_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV1_HISEE_FLASH_sel_clk_hisee_flash_START (0)
#define SOC_CRGPERIPH_CLKDIV1_HISEE_FLASH_sel_clk_hisee_flash_END (0)
#define SOC_CRGPERIPH_CLKDIV1_HISEE_FLASH_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV1_HISEE_FLASH_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 4;
        unsigned int reserved_1: 4;
        unsigned int reserved_2: 8;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_CLKDIV2_HISEE_FLASH_UNION;
#endif
#define SOC_CRGPERIPH_CLKDIV2_HISEE_FLASH_bitmasken_START (16)
#define SOC_CRGPERIPH_CLKDIV2_HISEE_FLASH_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_modem : 1;
        unsigned int ip_rst_modem_5g : 1;
        unsigned int ip_rst_bba : 1;
        unsigned int ip_rst_mdm_nttp2axi : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_MDM_SEC_RSTEN_UNION;
#endif
#define SOC_CRGPERIPH_MDM_SEC_RSTEN_ip_rst_modem_START (0)
#define SOC_CRGPERIPH_MDM_SEC_RSTEN_ip_rst_modem_END (0)
#define SOC_CRGPERIPH_MDM_SEC_RSTEN_ip_rst_modem_5g_START (1)
#define SOC_CRGPERIPH_MDM_SEC_RSTEN_ip_rst_modem_5g_END (1)
#define SOC_CRGPERIPH_MDM_SEC_RSTEN_ip_rst_bba_START (2)
#define SOC_CRGPERIPH_MDM_SEC_RSTEN_ip_rst_bba_END (2)
#define SOC_CRGPERIPH_MDM_SEC_RSTEN_ip_rst_mdm_nttp2axi_START (3)
#define SOC_CRGPERIPH_MDM_SEC_RSTEN_ip_rst_mdm_nttp2axi_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_modem : 1;
        unsigned int ip_rst_modem_5g : 1;
        unsigned int ip_rst_bba : 1;
        unsigned int ip_rst_mdm_nttp2axi : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_MDM_SEC_RSTDIS_UNION;
#endif
#define SOC_CRGPERIPH_MDM_SEC_RSTDIS_ip_rst_modem_START (0)
#define SOC_CRGPERIPH_MDM_SEC_RSTDIS_ip_rst_modem_END (0)
#define SOC_CRGPERIPH_MDM_SEC_RSTDIS_ip_rst_modem_5g_START (1)
#define SOC_CRGPERIPH_MDM_SEC_RSTDIS_ip_rst_modem_5g_END (1)
#define SOC_CRGPERIPH_MDM_SEC_RSTDIS_ip_rst_bba_START (2)
#define SOC_CRGPERIPH_MDM_SEC_RSTDIS_ip_rst_bba_END (2)
#define SOC_CRGPERIPH_MDM_SEC_RSTDIS_ip_rst_mdm_nttp2axi_START (3)
#define SOC_CRGPERIPH_MDM_SEC_RSTDIS_ip_rst_mdm_nttp2axi_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_modem : 1;
        unsigned int ip_rst_modem_5g : 1;
        unsigned int ip_rst_bba : 1;
        unsigned int ip_rst_mdm_nttp2axi : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_CRGPERIPH_MDM_SEC_RSTSTAT_UNION;
#endif
#define SOC_CRGPERIPH_MDM_SEC_RSTSTAT_ip_rst_modem_START (0)
#define SOC_CRGPERIPH_MDM_SEC_RSTSTAT_ip_rst_modem_END (0)
#define SOC_CRGPERIPH_MDM_SEC_RSTSTAT_ip_rst_modem_5g_START (1)
#define SOC_CRGPERIPH_MDM_SEC_RSTSTAT_ip_rst_modem_5g_END (1)
#define SOC_CRGPERIPH_MDM_SEC_RSTSTAT_ip_rst_bba_START (2)
#define SOC_CRGPERIPH_MDM_SEC_RSTSTAT_ip_rst_bba_END (2)
#define SOC_CRGPERIPH_MDM_SEC_RSTSTAT_ip_rst_mdm_nttp2axi_START (3)
#define SOC_CRGPERIPH_MDM_SEC_RSTSTAT_ip_rst_mdm_nttp2axi_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ddrc_buf : 1;
        unsigned int ip_prst_ipc_mdm_sec : 1;
        unsigned int ip_prst_gpio0_se : 1;
        unsigned int ip_arst_ddrc : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_GENERAL_SEC_RSTEN_UNION;
#endif
#define SOC_CRGPERIPH_GENERAL_SEC_RSTEN_ip_rst_ddrc_buf_START (0)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTEN_ip_rst_ddrc_buf_END (0)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTEN_ip_prst_ipc_mdm_sec_START (1)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTEN_ip_prst_ipc_mdm_sec_END (1)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTEN_ip_prst_gpio0_se_START (2)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTEN_ip_prst_gpio0_se_END (2)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTEN_ip_arst_ddrc_START (3)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTEN_ip_arst_ddrc_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ddrc_buf : 1;
        unsigned int ip_prst_ipc_mdm_sec : 1;
        unsigned int ip_prst_gpio0_se : 1;
        unsigned int ip_arst_ddrc : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_GENERAL_SEC_RSTDIS_UNION;
#endif
#define SOC_CRGPERIPH_GENERAL_SEC_RSTDIS_ip_rst_ddrc_buf_START (0)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTDIS_ip_rst_ddrc_buf_END (0)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTDIS_ip_prst_ipc_mdm_sec_START (1)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTDIS_ip_prst_ipc_mdm_sec_END (1)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTDIS_ip_prst_gpio0_se_START (2)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTDIS_ip_prst_gpio0_se_END (2)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTDIS_ip_arst_ddrc_START (3)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTDIS_ip_arst_ddrc_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ddrc_buf : 1;
        unsigned int ip_prst_ipc_mdm_sec : 1;
        unsigned int ip_prst_gpio0_se : 1;
        unsigned int ip_arst_ddrc : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 27;
    } reg;
} SOC_CRGPERIPH_GENERAL_SEC_RSTSTAT_UNION;
#endif
#define SOC_CRGPERIPH_GENERAL_SEC_RSTSTAT_ip_rst_ddrc_buf_START (0)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTSTAT_ip_rst_ddrc_buf_END (0)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTSTAT_ip_prst_ipc_mdm_sec_START (1)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTSTAT_ip_prst_ipc_mdm_sec_END (1)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTSTAT_ip_prst_gpio0_se_START (2)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTSTAT_ip_prst_gpio0_se_END (2)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTSTAT_ip_arst_ddrc_START (3)
#define SOC_CRGPERIPH_GENERAL_SEC_RSTSTAT_ip_arst_ddrc_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_pclk_ipc_mdm_sec : 1;
        unsigned int gt_pclk_gpio0_se : 1;
        unsigned int gt_clk_modem0_tsensor : 1;
        unsigned int gt_clk_a53_tsensor : 1;
        unsigned int gt_clk_g3d_tsensor : 1;
        unsigned int gt_clk_modem_tsensor : 1;
        unsigned int gt_clk_dig_tsensor1 : 1;
        unsigned int gt_clk_dig_tsensor2 : 1;
        unsigned int gt_clk_dig_tsensor3 : 1;
        unsigned int gt_clk_dig_tsensor4 : 1;
        unsigned int gt_clk_dig_tsensor5 : 1;
        unsigned int gt_clk_dig_tsensor6 : 1;
        unsigned int gt_clk_dig_tsensor7 : 1;
        unsigned int gt_clk_dig_tsensor8 : 1;
        unsigned int gt_clk_dig_tsensor9 : 1;
        unsigned int gt_clk_dig_tsensor10 : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_CRGPERIPH_GENERAL_SEC_PEREN_UNION;
#endif
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_pclk_ipc_mdm_sec_START (0)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_pclk_ipc_mdm_sec_END (0)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_pclk_gpio0_se_START (1)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_pclk_gpio0_se_END (1)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_modem0_tsensor_START (2)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_modem0_tsensor_END (2)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_a53_tsensor_START (3)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_a53_tsensor_END (3)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_g3d_tsensor_START (4)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_g3d_tsensor_END (4)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_modem_tsensor_START (5)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_modem_tsensor_END (5)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor1_START (6)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor1_END (6)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor2_START (7)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor2_END (7)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor3_START (8)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor3_END (8)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor4_START (9)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor4_END (9)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor5_START (10)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor5_END (10)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor6_START (11)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor6_END (11)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor7_START (12)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor7_END (12)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor8_START (13)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor8_END (13)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor9_START (14)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor9_END (14)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor10_START (15)
#define SOC_CRGPERIPH_GENERAL_SEC_PEREN_gt_clk_dig_tsensor10_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_pclk_ipc_mdm_sec : 1;
        unsigned int gt_pclk_gpio0_se : 1;
        unsigned int gt_clk_modem0_tsensor : 1;
        unsigned int gt_clk_a53_tsensor : 1;
        unsigned int gt_clk_g3d_tsensor : 1;
        unsigned int gt_clk_modem_tsensor : 1;
        unsigned int gt_clk_dig_tsensor1 : 1;
        unsigned int gt_clk_dig_tsensor2 : 1;
        unsigned int gt_clk_dig_tsensor3 : 1;
        unsigned int gt_clk_dig_tsensor4 : 1;
        unsigned int gt_clk_dig_tsensor5 : 1;
        unsigned int gt_clk_dig_tsensor6 : 1;
        unsigned int gt_clk_dig_tsensor7 : 1;
        unsigned int gt_clk_dig_tsensor8 : 1;
        unsigned int gt_clk_dig_tsensor9 : 1;
        unsigned int gt_clk_dig_tsensor10 : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_CRGPERIPH_GENERAL_SEC_PERDIS_UNION;
#endif
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_pclk_ipc_mdm_sec_START (0)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_pclk_ipc_mdm_sec_END (0)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_pclk_gpio0_se_START (1)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_pclk_gpio0_se_END (1)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_modem0_tsensor_START (2)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_modem0_tsensor_END (2)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_a53_tsensor_START (3)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_a53_tsensor_END (3)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_g3d_tsensor_START (4)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_g3d_tsensor_END (4)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_modem_tsensor_START (5)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_modem_tsensor_END (5)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor1_START (6)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor1_END (6)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor2_START (7)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor2_END (7)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor3_START (8)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor3_END (8)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor4_START (9)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor4_END (9)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor5_START (10)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor5_END (10)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor6_START (11)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor6_END (11)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor7_START (12)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor7_END (12)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor8_START (13)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor8_END (13)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor9_START (14)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor9_END (14)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor10_START (15)
#define SOC_CRGPERIPH_GENERAL_SEC_PERDIS_gt_clk_dig_tsensor10_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_pclk_ipc_mdm_sec : 1;
        unsigned int gt_pclk_gpio0_se : 1;
        unsigned int gt_clk_modem0_tsensor : 1;
        unsigned int gt_clk_a53_tsensor : 1;
        unsigned int gt_clk_g3d_tsensor : 1;
        unsigned int gt_clk_modem_tsensor : 1;
        unsigned int gt_clk_dig_tsensor1 : 1;
        unsigned int gt_clk_dig_tsensor2 : 1;
        unsigned int gt_clk_dig_tsensor3 : 1;
        unsigned int gt_clk_dig_tsensor4 : 1;
        unsigned int gt_clk_dig_tsensor5 : 1;
        unsigned int gt_clk_dig_tsensor6 : 1;
        unsigned int gt_clk_dig_tsensor7 : 1;
        unsigned int gt_clk_dig_tsensor8 : 1;
        unsigned int gt_clk_dig_tsensor9 : 1;
        unsigned int gt_clk_dig_tsensor10 : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_UNION;
#endif
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_pclk_ipc_mdm_sec_START (0)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_pclk_ipc_mdm_sec_END (0)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_pclk_gpio0_se_START (1)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_pclk_gpio0_se_END (1)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_modem0_tsensor_START (2)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_modem0_tsensor_END (2)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_a53_tsensor_START (3)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_a53_tsensor_END (3)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_g3d_tsensor_START (4)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_g3d_tsensor_END (4)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_modem_tsensor_START (5)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_modem_tsensor_END (5)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor1_START (6)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor1_END (6)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor2_START (7)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor2_END (7)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor3_START (8)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor3_END (8)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor4_START (9)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor4_END (9)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor5_START (10)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor5_END (10)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor6_START (11)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor6_END (11)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor7_START (12)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor7_END (12)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor8_START (13)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor8_END (13)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor9_START (14)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor9_END (14)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor10_START (15)
#define SOC_CRGPERIPH_GENERAL_SEC_PERCLKEN_gt_clk_dig_tsensor10_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_pclk_ipc_mdm_sec : 1;
        unsigned int st_pclk_gpio0_se : 1;
        unsigned int st_clk_modem0_tsensor : 1;
        unsigned int st_clk_a53_tsensor : 1;
        unsigned int st_clk_g3d_tsensor : 1;
        unsigned int st_clk_modem_tsensor : 1;
        unsigned int st_clk_dig_tsensor1 : 1;
        unsigned int st_clk_dig_tsensor2 : 1;
        unsigned int st_clk_dig_tsensor3 : 1;
        unsigned int st_clk_dig_tsensor4 : 1;
        unsigned int st_clk_dig_tsensor5 : 1;
        unsigned int st_clk_dig_tsensor6 : 1;
        unsigned int st_clk_dig_tsensor7 : 1;
        unsigned int st_clk_dig_tsensor8 : 1;
        unsigned int st_clk_dig_tsensor9 : 1;
        unsigned int st_clk_dig_tsensor10 : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_UNION;
#endif
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_pclk_ipc_mdm_sec_START (0)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_pclk_ipc_mdm_sec_END (0)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_pclk_gpio0_se_START (1)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_pclk_gpio0_se_END (1)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_modem0_tsensor_START (2)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_modem0_tsensor_END (2)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_a53_tsensor_START (3)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_a53_tsensor_END (3)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_g3d_tsensor_START (4)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_g3d_tsensor_END (4)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_modem_tsensor_START (5)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_modem_tsensor_END (5)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor1_START (6)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor1_END (6)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor2_START (7)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor2_END (7)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor3_START (8)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor3_END (8)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor4_START (9)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor4_END (9)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor5_START (10)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor5_END (10)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor6_START (11)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor6_END (11)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor7_START (12)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor7_END (12)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor8_START (13)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor8_END (13)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor9_START (14)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor9_END (14)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor10_START (15)
#define SOC_CRGPERIPH_GENERAL_SEC_PERSTAT_st_clk_dig_tsensor10_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ipc_mdm_sec_clkrst_bypass : 1;
        unsigned int gpio0_se_clkrst_bypass : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 29;
    } reg;
} SOC_CRGPERIPH_SEC_IPCLKRST_BYPASS_UNION;
#endif
#define SOC_CRGPERIPH_SEC_IPCLKRST_BYPASS_ipc_mdm_sec_clkrst_bypass_START (0)
#define SOC_CRGPERIPH_SEC_IPCLKRST_BYPASS_ipc_mdm_sec_clkrst_bypass_END (0)
#define SOC_CRGPERIPH_SEC_IPCLKRST_BYPASS_gpio0_se_clkrst_bypass_START (1)
#define SOC_CRGPERIPH_SEC_IPCLKRST_BYPASS_gpio0_se_clkrst_bypass_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_peri_io_ret_from_crg : 1;
        unsigned int reserved : 15;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_GENERAL_SEC_CLKDIV0_UNION;
#endif
#define SOC_CRGPERIPH_GENERAL_SEC_CLKDIV0_sc_peri_io_ret_from_crg_START (0)
#define SOC_CRGPERIPH_GENERAL_SEC_CLKDIV0_sc_peri_io_ret_from_crg_END (0)
#define SOC_CRGPERIPH_GENERAL_SEC_CLKDIV0_bitmasken_START (16)
#define SOC_CRGPERIPH_GENERAL_SEC_CLKDIV0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int peri_powerup_cnt : 4;
        unsigned int reserved_0 : 4;
        unsigned int reserved_1 : 4;
        unsigned int reserved_2 : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CRGPERIPH_GENERAL_SEC_CTRL0_UNION;
#endif
#define SOC_CRGPERIPH_GENERAL_SEC_CTRL0_peri_powerup_cnt_START (0)
#define SOC_CRGPERIPH_GENERAL_SEC_CTRL0_peri_powerup_cnt_END (3)
#define SOC_CRGPERIPH_GENERAL_SEC_CTRL0_bitmasken_START (16)
#define SOC_CRGPERIPH_GENERAL_SEC_CTRL0_bitmasken_END (31)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
