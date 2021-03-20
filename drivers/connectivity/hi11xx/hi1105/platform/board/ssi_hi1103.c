

#ifdef _PRE_CONFIG_GPIO_TO_SSI_DEBUG

/* ͷ�ļ����� */
#include "ssi_hi1103.h"

#include "board.h"
#include "plat_debug.h"
#include "plat_pm.h"
#include "ssi_common.h"


#define TCXO_32K_DET_VALUE 10
#define TCXO_LIMIT_THRESHOLD 5
#define TCXO_GATING_CLK 76800000 /* Ĭ��ʱ�� */
#define TCXO_NOMAL_CKL  38400000

static uint32 g_halt_det_cnt = 0; /* ���soc�쳣���� */
static ssi_cpu_infos g_ssi_cpu_infos;

static char *g_ssi_cpu_st_str[] = {
    "OFF",              /* 0x0 */
    "BOOTING",          /* 0x1 */
    "SLEEPING",         /* 0x2 */
    "WORK",             /* 0x3 */
    "SAVING",           /* 0x4 */
    "PROTECT(ocp/scp)", /* 0x5 */
    "SLEEP",            /* 0x6 */
    "PROTECTING"        /* 0x7 */
};

static ssi_reg_info g_hi1103_glb_ctrl_full = { 0x50000000, 0x1000, SSI_RW_WORD_MOD };
static ssi_reg_info g_hi1103_glb_ctrl_extend1 = { 0x50001400, 0x10,   SSI_RW_WORD_MOD };
static ssi_reg_info g_hi1103_glb_ctrl_extend2 = { 0x50001540, 0xc,    SSI_RW_WORD_MOD };
static ssi_reg_info g_hi1103_glb_ctrl_extend3 = { 0x50001600, 0x4,    SSI_RW_WORD_MOD };
static ssi_reg_info g_hi1103_pmu_cmu_ctrl_full = { 0x50002000, 0xb00,  SSI_RW_WORD_MOD };
static ssi_reg_info g_hi1103_pmu2_cmu_ir_ctrl_full = { 0x50003000, 0xa20,  SSI_RW_WORD_MOD };
static ssi_reg_info g_hi1103_pmu2_cmu_ir_ctrl_tail = { 0x50003a80, 0xc,    SSI_RW_WORD_MOD };
static ssi_reg_info g_hi1103_coex_ctl_part1 = { 0x5000a000, 0x354,  SSI_RW_WORD_MOD }; /* coex ctl part1 */
static ssi_reg_info g_hi1103_coex_ctl_part2 = { 0x5000a500, 0x8,    SSI_RW_WORD_MOD };   /* coex ctl part2 */
static ssi_reg_info g_hi1103_w_ctrl_full = { 0x40000000, 0x408,  SSI_RW_WORD_MOD };
static ssi_reg_info g_hi1103_w_key_mem = { 0x2001e620, 0x80,   SSI_RW_DWORD_MOD };
static ssi_reg_info g_hi1105_w_key_mem = { 0x200268b0, 0x80,   SSI_RW_DWORD_MOD };
static ssi_reg_info g_hi1103_b_except_mem  =    {0x8021ab80, 0x64, SSI_RW_DWORD_MOD};
static ssi_reg_info g_hi1103_b_ctrl_full = { 0x48000000, 0x40c,  SSI_RW_WORD_MOD };
static ssi_reg_info g_hi1103_pcie_ctrl_full = { 0x40007000, 0x4c8,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_hi1103_pcie_dbi_full = { 0x40102000, 0x900,  SSI_RW_DWORD_MOD };         /* û����֮ǰ���ܶ� */
static ssi_reg_info g_hi1103_pcie_pilot_dma_full = { 0x40106000, 0x1000, SSI_RW_DWORD_MOD };  /* 4KB */
static ssi_reg_info g_hi1103_pcie_dma_ctrl_full = { 0x40008000, 0x34,   SSI_RW_DWORD_MOD };
static ssi_reg_info g_hi1103_pcie_sdio_ctrl_full = { 0x40101000, 0x180,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_hi1103_wifi_gpio0_full = {0x50004000, 0x74, SSI_RW_WORD_MOD};
static ssi_reg_info g_hi1103_wifi_gpio1_full = {0x5000c000, 0x74, SSI_RW_WORD_MOD};
static ssi_reg_info g_hi1103_bfgx_gpio0_full = {0x50005000, 0x74, SSI_RW_WORD_MOD};
static ssi_reg_info g_hi1103_bfgx_gpio1_full = {0x5000d000, 0x74, SSI_RW_WORD_MOD};

static ssi_reg_info g_hi1103_rf_w_c0_full = {0x40040000, 0x314, SSI_RW_WORD_MOD};
static ssi_reg_info g_hi1103_rf_w_c1_full = {0x40042000, 0x314, SSI_RW_WORD_MOD};
static ssi_reg_info g_hi1103_rf_bfg_full = {0x4800C000, 0xB24, SSI_RW_WORD_MOD};

static ssi_reg_info g_hi1103_tcxo_detect_reg1 = { 0x50000040, 0x4,  SSI_RW_WORD_MOD };
static ssi_reg_info g_hi1103_tcxo_detect_reg2 = { 0x500000c0, 0x14, SSI_RW_WORD_MOD };
static ssi_reg_info g_hi1103_tcxo_detect_reg3 = { 0x50000700, 0xc,  SSI_RW_WORD_MOD };

static ssi_reg_info *g_hi1103_aon_reg_full[] = {
    &g_hi1103_glb_ctrl_full,
    &g_hi1103_glb_ctrl_extend1,
    &g_hi1103_glb_ctrl_extend2,
    &g_hi1103_glb_ctrl_extend3,
    &g_hi1103_pmu_cmu_ctrl_full,
    &g_hi1103_pmu2_cmu_ir_ctrl_full,
    &g_hi1103_pmu2_cmu_ir_ctrl_tail,
    &g_hi1103_wifi_gpio0_full,
    &g_hi1103_wifi_gpio1_full,
    &g_hi1103_bfgx_gpio0_full,
    &g_hi1103_bfgx_gpio1_full
};

static ssi_reg_info *g_hi1103_coex_ctl_full[] = {
    &g_hi1103_coex_ctl_part1,
    &g_hi1103_coex_ctl_part2,
};

static ssi_reg_info *g_hi1103_tcxo_detect_regs[] = {
    &g_hi1103_tcxo_detect_reg1,
    &g_hi1103_tcxo_detect_reg2,
    &g_hi1103_tcxo_detect_reg3
};

// 0x5000_0000~0x500000FC
static ssi_reg_info g_hi1103_glb_ctrl_cut1 = { 0x50000000, 0xfc, SSI_RW_WORD_MOD };
// 0x5000_0200~0x5000_020C
static ssi_reg_info g_hi1103_glb_ctrl_cut2 = { 0x50000200, 0xc, SSI_RW_WORD_MOD };
// 0x5000_0400~0x5000_043C
static ssi_reg_info g_hi1103_glb_ctrl_cut3 = { 0x50000400, 0x3c, SSI_RW_WORD_MOD };
// 0x5000_0500~0x5000_051C
static ssi_reg_info g_hi1103_glb_ctrl_cut4 = { 0x50000500, 0x1c, SSI_RW_WORD_MOD };
// 0x5000_0700~0x5000_070C
static ssi_reg_info g_hi1103_glb_ctrl_cut5 = { 0x50000700, 0xc, SSI_RW_WORD_MOD };
// 0x5000_0E00~0x5000_0E0C
static ssi_reg_info g_hi1103_glb_ctrl_cut6 = { 0x50000E00, 0xc, SSI_RW_WORD_MOD };
// 0x5000_1400~0x5000_140C
static ssi_reg_info g_hi1103_glb_ctrl_cut7 = { 0x50001400, 0x10, SSI_RW_WORD_MOD };
// 0x5000_1540~0x5000_1548
static ssi_reg_info g_hi1103_glb_ctrl_cut8 = { 0x50001540, 0xc, SSI_RW_WORD_MOD };
// 0x5000_1600~0x5000_1604
static ssi_reg_info g_hi1103_glb_ctrl_cut9 = { 0x50001600, 0x4, SSI_RW_WORD_MOD };

// PMU_CMU_CTRL
// 0x50002080~0x500021AC
static ssi_reg_info g_hi1103_pmu_cmu_ctrl_cut1 = { 0x50002080, 0x12c, SSI_RW_WORD_MOD };
// 0x50002200~0x5000220C
static ssi_reg_info g_hi1103_pmu_cmu_ctrl_cut2 = { 0x50002200, 0xc, SSI_RW_WORD_MOD };
// 0x50002380~0x5000239C
static ssi_reg_info g_hi1103_pmu_cmu_ctrl_cut3 = { 0x50002380, 0x1c, SSI_RW_WORD_MOD };
// 0x50002800~0x5000283C
static ssi_reg_info g_hi1103_pmu_cmu_ctrl_cut4 = { 0x50002800, 0x3c, SSI_RW_WORD_MOD };

// PMU2_CMU_IR_TS_EF_CTL
// 0x50003040~0x5000307C
static ssi_reg_info g_hi1103_pmu2_cmu_ir_ctrl_cut1 = { 0x50003040, 0x3c, SSI_RW_WORD_MOD };
// 0x5000311C
static ssi_reg_info g_hi1103_pmu2_cmu_ir_ctrl_cut2 = { 0x5000311C, 0x4, SSI_RW_WORD_MOD };
// 0x5000313C
static ssi_reg_info g_hi1103_pmu2_cmu_ir_ctrl_cut3 = { 0x5000313C, 0x4, SSI_RW_WORD_MOD };
// 0x5000315C
static ssi_reg_info g_hi1103_pmu2_cmu_ir_ctrl_cut4 = { 0x5000315C, 0x4, SSI_RW_WORD_MOD };
// 0x5000317C
static ssi_reg_info g_hi1103_pmu2_cmu_ir_ctrl_cut5 = { 0x5000317C, 0x4, SSI_RW_WORD_MOD };
// 0x5000319C
static ssi_reg_info g_hi1103_pmu2_cmu_ir_ctrl_cut6 = { 0x5000319C, 0x4, SSI_RW_WORD_MOD };
// 0x50003220~0x5000339C
static ssi_reg_info g_hi1103_pmu2_cmu_ir_ctrl_cut7 = { 0x50003220, 0x17c, SSI_RW_WORD_MOD };
// 0x50003420~0x5000343C
static ssi_reg_info g_hi1103_pmu2_cmu_ir_ctrl_cut8 = { 0x50003420, 0x1c, SSI_RW_WORD_MOD };
// 0x50003780~0x500037FC
static ssi_reg_info g_hi1103_pmu2_cmu_ir_ctrl_cut9 = { 0x50003780, 0x7c, SSI_RW_WORD_MOD };
// 0x50003800~0x500038BF
static ssi_reg_info g_hi1103_pmu2_cmu_ir_ctrl_cut10 = { 0x50003800, 0xc0, SSI_RW_WORD_MOD };
// 0x50003A80~0x50003A8C
static ssi_reg_info g_hi1103_pmu2_cmu_ir_ctrl_cut11 = { 0x50003A80, 0xc, SSI_RW_WORD_MOD };

static ssi_reg_info *g_hi1103_aon_reg_cut[] = {
    &g_hi1103_glb_ctrl_cut1,
    &g_hi1103_glb_ctrl_cut2,
    &g_hi1103_glb_ctrl_cut3,
    &g_hi1103_glb_ctrl_cut4,
    &g_hi1103_glb_ctrl_cut5,
    &g_hi1103_glb_ctrl_cut6,
    &g_hi1103_glb_ctrl_cut7,
    &g_hi1103_glb_ctrl_cut8,
    &g_hi1103_glb_ctrl_cut9,
    &g_hi1103_pmu_cmu_ctrl_cut1,
    &g_hi1103_pmu_cmu_ctrl_cut2,
    &g_hi1103_pmu_cmu_ctrl_cut3,
    &g_hi1103_pmu_cmu_ctrl_cut4,
    &g_hi1103_pmu2_cmu_ir_ctrl_cut1,
    &g_hi1103_pmu2_cmu_ir_ctrl_cut2,
    &g_hi1103_pmu2_cmu_ir_ctrl_cut3,
    &g_hi1103_pmu2_cmu_ir_ctrl_cut4,
    &g_hi1103_pmu2_cmu_ir_ctrl_cut5,
    &g_hi1103_pmu2_cmu_ir_ctrl_cut6,
    &g_hi1103_pmu2_cmu_ir_ctrl_cut7,
    &g_hi1103_pmu2_cmu_ir_ctrl_cut8,
    &g_hi1103_pmu2_cmu_ir_ctrl_cut9,
    &g_hi1103_pmu2_cmu_ir_ctrl_cut10,
    &g_hi1103_pmu2_cmu_ir_ctrl_cut11
};

static ssi_reg_info g_hi1103_pcie_ctrl_cut1 = { 0x40007224, 0x4,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_hi1103_pcie_ctrl_cut2 = { 0x400072d0, 0x4,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_hi1103_pcie_ctrl_cut3 = { 0x40007430, 0x9c, SSI_RW_DWORD_MOD };

static ssi_reg_info *g_hi1103_pcie_cfg_reg_cut[] = {
    &g_hi1103_pcie_ctrl_cut1,
    &g_hi1103_pcie_ctrl_cut2,
    &g_hi1103_pcie_ctrl_cut3
};

static ssi_reg_info *g_hi1103_pcie_cfg_reg_full[] = {
    &g_hi1103_pcie_ctrl_full,
    &g_hi1103_pcie_dma_ctrl_full
};

static ssi_reg_info *g_hi1103_pcie_dbi_pilot_reg_full[] = {
    &g_hi1103_pcie_dbi_full,
    &g_hi1103_pcie_pilot_dma_full,
};

/* gnss_only uart_cfg */
static ssi_file_st g_ssi_file[] = {
#ifdef BFGX_UART_DOWNLOAD_SUPPORT
    /* gnss only */
    { "/system/vendor/firmware/RAM_VECTOR.bin", 0x80100800 },
    { "/system/vendor/firmware/CPU_RAM_SCHED.bin", 0x80004000 },
#else
    /* mpw2 */
    { "/system/vendor/firmware/BCPU_ROM.bin",           0x80000000 },
    { "/system/vendor/firmware/VECTORS.bin",            0x80010000 },
    { "/system/vendor/firmware/RAM_VECTOR.bin",         0x80105c00 },
    { "/system/vendor/firmware/WCPU_ROM.bin",           0x4000 },
    { "/system/vendor/firmware/WL_ITCM.bin",            0x10000 },
    { "/system/vendor/firmware/PLAT_RAM_EXCEPTION.bin", 0x20002800 },
#endif
};

static void ssi_detect_tcxo_handle(uint32 tcxo_det_res_old,
                                   uint32 tcxo_det_res_new,
                                   oal_uint64 base_tcxo_clock,
                                   uint32 tcxo_det_value_target,
                                   oal_uint32 clock_32k)
{
    char *tcxo_str = "";
    int tcxo_is_abnormal;
    oal_uint64 clock_tcxo = 0;
    oal_uint64 div_clock = 0;
    oal_uint64 tcxo_limit_low, tcxo_limit_high, tcxo_tmp;

    /* Ϊ�˼�����Χ */
    tcxo_tmp = div_u64(base_tcxo_clock, 100); /* 100 is Percentage */
    tcxo_limit_low = (tcxo_tmp * (100 - TCXO_LIMIT_THRESHOLD)); /* TCXO Lower threshold 100 - 5 */
    tcxo_limit_high = (tcxo_tmp * (100 + TCXO_LIMIT_THRESHOLD)); /* TCXO Upper threshold 100 + 5 */

    if (tcxo_det_res_new == tcxo_det_res_old) {
        /* tcxo �������Ϊ�򿪴�ʱӦ����ʱ�� */
        ps_print_err("tcxo don't change after detect, tcxo or 32k maybe abnormal, tcxo=0x%x,32k_clock=%u\n",
                     tcxo_det_res_new,
                     clock_32k);
        if (tcxo_det_res_new == 0) {
            tcxo_is_abnormal = 1;
            tcxo_str = "non-tcxo";
        } else {
            /* �����������Ч��̽�⣬Ҫ�����ϸ��־�������˴�DSM���Ըķ�֧�����ϱ� */
            tcxo_is_abnormal = 0;
            tcxo_str = "tcxo-detect-invalid";
        }
    } else {
        /*
         * tcxo_det_res_new read from 16bit width register  <= 0xffff
         * (tcxo_det_res_new * 32768) = (��⵽�ļ��������� * 32kʱ��)
         */
        if (tcxo_det_value_target == 0) {
            ps_print_err("tcxo_det_value_target is zero\n");
            return;
        }
        clock_tcxo = (oal_uint64)((tcxo_det_res_new * 32768) / (tcxo_det_value_target)); /* 32768 is 32.768KHz */
        div_clock = clock_tcxo;
        div_clock = div_u64(div_clock, 1000000); /* 1000000 is unit conversion hz to Mhz */
        if ((clock_tcxo < tcxo_limit_low) || (clock_tcxo > tcxo_limit_high)) {
            /* ʱ��������ֵ */
            tcxo_is_abnormal = 2; /* 2 */
            tcxo_str = "tcxo clock-abnormal";
        } else {
            tcxo_is_abnormal = 0;
            tcxo_str = "tcxo normal";
        }
        ps_print_err("%s real=%llu hz,%llu Mhz[base=%llu][limit:%llu~%llu]\n",
                     tcxo_str, clock_tcxo, div_clock, base_tcxo_clock, tcxo_limit_low, tcxo_limit_high);
    }

    /* tcxo detect abnormal, dmd report */
    if (g_hi11xx_kernel_crash == 0) {
        /* kernel is normal */
        if (tcxo_is_abnormal) {
#ifdef CONFIG_HUAWEI_DSM
            hw_110x_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_1103_TCXO_ERROR,
                "%s: tcxo=%llu[%llu][limit:%llu~%llu] 32k_clock=%lu,det_tick=0x%x value=0x%x\n",
                tcxo_str, clock_tcxo, base_tcxo_clock, tcxo_limit_low, tcxo_limit_high, clock_32k,
                tcxo_det_value_target, tcxo_det_res_new);
                g_halt_det_cnt++;
#endif
        }
    }
}

static int ssi_detect_tcxo_is_normal(void)
{
    /*
     * tcxo detect ����tcxoʱ�ӣ�
     * �����������tcxo �쳣��ôtcxo_32k_det_result Ϊ��ֵ
     * �����������32k�쳣 ��ôsytem_tickΪ��ֵ
     */
    int ret;
    uint32 reg;
    uint32 tcxo_enable;
    uint32 tcxo_det_value_src, tcxo_det_value_target;
    uint32 clock_32k = 0;
    uint32 sys_tick_old, sys_tick_new, pmu2_cmu_abb_sts_3, pmu2_cmu_abb_sts_2;
    uint32 tcxo_det_res_old, tcxo_det_res_new, cmu_reserve1;
    oal_uint64 base_tcxo_clock = TCXO_GATING_CLK;

    declare_time_cost_stru(cost);

    pmu2_cmu_abb_sts_3 = (uint32)ssi_read32(HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_STS_3_REG);
    pmu2_cmu_abb_sts_2 = (uint32)ssi_read32(HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_STS_2_REG);
    cmu_reserve1 = (uint32)ssi_read32(HI1103_PMU2_CMU_IR_CMU_RESERVE1_REG);
    tcxo_det_value_src = (uint32)ssi_read32(HI1103_GLB_CTL_TCXO_32K_DET_CNT_REG);

    if (cmu_reserve1 & (1 << 7)) { /* 0x50003338��bit7��ʾʱ��Ƶ��ѡ�� */
        base_tcxo_clock = TCXO_GATING_CLK;
    } else {
        base_tcxo_clock = TCXO_NOMAL_CKL;
    }

    if ((!(pmu2_cmu_abb_sts_3 & (1 << 7))) /* shift operation 7 bit */
        && (pmu2_cmu_abb_sts_2 & (1 << 14))) { /* shift operation 14 bit  */
        /* tcxo enable */
        tcxo_enable = 1;
    } else {
        /* system maybe sleep, tcxo disable */
        tcxo_enable = 0;
        ps_print_err("tcxo gating normal\n");
    }

    tcxo_det_value_target = TCXO_32K_DET_VALUE;
    if (tcxo_det_value_src == tcxo_det_value_target) {
        /* ������detect,�ı�det_value���۲�ֵ�Ƿ�ı� */
        tcxo_det_value_target = TCXO_32K_DET_VALUE + 2; /* 2 */
    }

    sys_tick_old = (uint32)ssi_read32(HI1103_GLB_CTL_SYS_TICK_VALUE_W_0_REG);
    tcxo_det_res_old = (uint32)ssi_read32(HI1103_GLB_CTL_TCXO_32K_DET_RESULT_REG);

    ssi_write32(HI1103_GLB_CTL_SYS_TICK_CFG_W_REG, 0x2); /* ����w systick */
    oal_get_time_cost_start(cost);

    if (tcxo_enable) {
        ssi_write32(HI1103_GLB_CTL_TCXO_32K_DET_CNT_REG, tcxo_det_value_target); /* ���ü������� */
        ssi_write32(HI1103_GLB_CTL_TCXO_DET_CTL_REG, 0x0);                       /* tcxo_det_en disable */

        /* to tcxo */
        ssi_switch_clk(SSI_AON_CLKSEL_TCXO);

        oal_udelay(150); /* delay 150 us */

        /* to ssi */
        ssi_switch_clk(SSI_AON_CLKSEL_SSI);
        ssi_write32(HI1103_GLB_CTL_TCXO_DET_CTL_REG, 0x1); /* tcxo_det_en enable */
        /* to tcxo */
        ssi_switch_clk(SSI_AON_CLKSEL_TCXO);
        /* delay 31 * 2 * tcxo_det_value_taget us.wait detect done,�������õļ���������  */
        oal_udelay(31 * tcxo_det_value_target * 2);

        /* to ssi */
        ssi_switch_clk(SSI_AON_CLKSEL_SSI);
    } else {
        oal_udelay(300); /* delay 300 us */
    }

    ret = ssi_read_reg_info_arry(g_hi1103_tcxo_detect_regs, sizeof(g_hi1103_tcxo_detect_regs) / sizeof(ssi_reg_info *),
                                 g_ssi_is_logfile);
    if (ret) {
        return ret;
    }

    oal_udelay(1000); /* delay 1000 us,wait 32k count more */

    oal_get_time_cost_end(cost);
    oal_calc_time_cost_sub(cost);

    sys_tick_new = (uint32)ssi_read32(HI1103_GLB_CTL_SYS_TICK_VALUE_W_0_REG);

    reg = (uint32)ssi_read32(HI1103_GLB_CTL_TCXO_DET_CTL_REG);
    tcxo_det_res_new = (uint32)ssi_read32(HI1103_GLB_CTL_TCXO_32K_DET_RESULT_REG);

    /* 32k detect */
    if (sys_tick_new == sys_tick_old) {
        ps_print_err("32k sys_tick don't change after detect, 32k maybe abnormal, sys_tick=0x%x\n", sys_tick_new);
    } else {
        oal_uint64 us_to_s;
        us_to_s = time_cost_var_sub(cost);
        us_to_s += 1446; /* ����ֵ,���1446us */
        clock_32k = (sys_tick_new * 1000) / (oal_uint32)us_to_s; /* 1000 is mean hz to KHz */
        ps_print_err("32k runtime:%llu us , sys_tick:%u\n", us_to_s, sys_tick_new);
        ps_print_err("32k realclock real= %u Khz[base=32768]\n", clock_32k);
    }

    /* tcxo enabled */
    if (tcxo_enable) {
        ssi_detect_tcxo_handle(tcxo_det_res_old, tcxo_det_res_new,
                               base_tcxo_clock, tcxo_det_value_target, clock_32k);
    }

    return ret;
}

int32 ssi_tcxo_mux(uint32 flag)
{
    int ret;

    if ((g_st_board_info.ssi_gpio_clk == 0) || (g_st_board_info.ssi_gpio_data == 0)) {
        ps_print_err("reset aon, gpio ssi don't support\n");
        return -1;
    }

    ret = ssi_request_gpio(g_st_board_info.ssi_gpio_clk, g_st_board_info.ssi_gpio_data);
    if (ret) {
        ps_print_err("ssi_force_reset_aon request failed:%d, data:%d, ret=%d\n",
                     g_st_board_info.ssi_gpio_clk, g_st_board_info.ssi_gpio_data, ret);
        return ret;
    }

    ps_print_info("SSI start set\n");

    ssi_switch_clk(SSI_AON_CLKSEL_SSI);

    if (flag == 1) {
        ssi_write16(gpio_ssi_reg(SSI_SSI_CTRL), 0x0);
        ssi_write16(gpio_ssi_reg(SSI_SEL_CTRL), 0x60);
        ssi_write16(gpio_ssi_reg(SSI_SSI_CTRL), 0x60);
        ssi_write32(HI1103_PMU2_CMU_IR_CMU_RESERVE1_REG, 0x100);
        ps_print_info("SSI set 0x50003338 to 0x100\n");
    } else {
        ssi_write16(gpio_ssi_reg(SSI_SEL_CTRL), 0x0);
    }

    ssi_switch_clk(SSI_AON_CLKSEL_TCXO);

    ps_print_info("SSI set OK\n");

    ssi_free_gpio();

    return 0;
}

static void dsm_cpu_info_dump(void)
{
    int32 i;
    int32 ret;
    int32 count = 0;
    char buf[DSM_CPU_INFO_SIZE];
    /* dsm cpu��Ϣ�ϱ� */
    if (g_halt_det_cnt || g_hi11xx_kernel_crash) {
        ps_print_info("g_halt_det_cnt=%u g_hi11xx_kernel_crash=%d dsm_cpu_info_dump return\n",
                      g_halt_det_cnt, g_hi11xx_kernel_crash);
        return;
    }

    /* û�м�⵽�쳣���ϱ���¼��CPU��Ϣ */
    memset_s((void *)buf, sizeof(buf), 0, sizeof(buf));
    ret = snprintf_s(buf + count, sizeof(buf) - count, sizeof(buf) - count - 1,
                     "wcpu_state=0x%x %s, bcpu_state=0x%x %s ",
                     g_ssi_cpu_infos.wcpu_info.cpu_state,
                     (g_ssi_cpu_st_str[g_ssi_cpu_infos.wcpu_info.cpu_state & 0x7]),
                     g_ssi_cpu_infos.bcpu_info.cpu_state,
                     (g_ssi_cpu_st_str[g_ssi_cpu_infos.bcpu_info.cpu_state & 0x7]));
    if (ret < 0) {
        goto done;
    }
    count += ret;

    for (i = 0; i < SSI_CPU_ARM_REG_DUMP_CNT; i++) {
        if (g_ssi_cpu_infos.wcpu_info.reg_flag[i] == 0) {
            continue;
        }
        ret = snprintf_s(buf + count, sizeof(buf) - count, sizeof(buf) - count - 1,
                         "wcpu[%d] pc:0x%x lr:0x%x sp:0x%x ", i, g_ssi_cpu_infos.wcpu_info.pc[i],
                         g_ssi_cpu_infos.wcpu_info.lr[i], g_ssi_cpu_infos.wcpu_info.sp[i]);
        if (ret < 0) {
            goto done;
        }
        count += ret;
    }

done:
#ifdef CONFIG_HUAWEI_DSM
    hw_110x_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_1103_HALT, "%s\n", buf);
#else
    oal_io_print("log str format err [non-dsm]%s\n", buf);
#endif
}

static void ssi_check_buck_scp_ocp_status(void)
{
    uint32 reg;

    /* buck ocp/acp */
    reg = (uint32)ssi_read32(HI1103_PMU_CMU_CTL_PMU_PROTECT_STATUS_REG);
    if ((reg & (0xFFFFFFFC)) != 0) {
        /* bit 0,1 */
        ps_print_info("buck protect status:0x%x invalid", reg);
        return;
    }

    ps_print_info("buck protect status:0x%x %s %s \n", reg,
                  (reg & 0x1) ? "buck_scp_off" : "", (reg & 0x2) ? "buck_ocp_off" : "");
#ifdef CONFIG_HUAWEI_DSM
    if (reg & 0x3) {
        hw_110x_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_BUCK_PROTECTED, "%s: buck protect status:0x%x %s %s \n",
                                  __FUNCTION__, reg, (reg & 0x1) ? "buck_scp_off" : "",
                                  (reg & 0x2) ? "buck_ocp_off" : "");
        g_halt_det_cnt++;
    }
#endif
}

static int ssi_check_wcpu_is_working(void)
{
    uint32 reg, mask;

    /* pilot */
    reg = (uint32)ssi_read32(HI1103_PMU_CMU_CTL_SYS_STATUS_0_REG);
    mask = reg & 0x7;
    ps_print_info("cpu state=0x%8x, wcpu is %s\n", reg, g_ssi_cpu_st_str[mask]);
    g_ssi_cpu_infos.wcpu_info.cpu_state = mask;
    if (mask == 0x5) {
        ssi_check_buck_scp_ocp_status();
    }
    return (mask == 0x3);
}

static int ssi_check_bcpu_is_working(void)
{
    uint32 reg, mask;

    /* pilot */
    reg = (uint32)ssi_read32(HI1103_PMU_CMU_CTL_SYS_STATUS_0_REG);
    mask = (reg >> 3) & 0x7; /* reg shift right by 3 bits and 0x7 bits */
    ps_print_info("cpu state=0x%8x, bcpu is %s\n", reg, g_ssi_cpu_st_str[mask]);
    g_ssi_cpu_infos.bcpu_info.cpu_state = mask;
    if (mask == 0x5) {
        ssi_check_buck_scp_ocp_status();
    }
    return (mask == 0x3);
}

static int ssi_read_wpcu_pc_lr_sp(int trace_en)
{
    int i;
    uint32 reg_low, reg_high, pc, lr, sp;

    /* read pc twice check whether wcpu is runing */
    for (i = 0; i < SSI_CPU_ARM_REG_DUMP_CNT; i++) {
        ssi_write32(HI1103_GLB_CTL_WCPU_LOAD_REG, 0x1);
        oal_mdelay(1);

        reg_low = (uint32)ssi_read32(HI1103_GLB_CTL_WCPU_PC_L_REG);
        reg_high = (uint32)ssi_read32(HI1103_GLB_CTL_WCPU_PC_H_REG);
        pc = reg_low | (reg_high << 16); /* Shift leftwards by 16 bits */

        reg_low = (uint32)ssi_read32(HI1103_GLB_CTL_WCPU_LR_L_REG);
        reg_high = (uint32)ssi_read32(HI1103_GLB_CTL_WCPU_LR_H_REG);
        lr = reg_low | (reg_high << 16); /* Shift leftwards by 16 bits */

        reg_low = (uint32)ssi_read32(HI1103_GLB_CTL_WCPU_SP_L_REG);
        reg_high = (uint32)ssi_read32(HI1103_GLB_CTL_WCPU_SP_H_REG);
        sp = reg_low | (reg_high << 16); /* Shift leftwards by 16 bits */

        ps_print_info("gpio-ssi:read wcpu[%i], pc:0x%x, lr:0x%x, sp:0x%x \n", i, pc, lr, sp);
        if (!pc && !lr && !sp) {
            ps_print_info("wcpu pc lr sp all zero\n");
            if (trace_en) {
                if (ssi_check_wcpu_is_working()) {
                    ps_print_info("wcpu try to enable trace en\n");
                    ssi_write32(HI1103_WCPU_PATCH_WCPU_CFG_TRACE_EN_REG, 0x1);
                    oal_mdelay(1);
                }
                trace_en = 0;
                i = -1;
            }
        } else {
            if (g_ssi_cpu_infos.wcpu_info.reg_flag[i] == 0) {
                g_ssi_cpu_infos.wcpu_info.reg_flag[i] = 1;
                g_ssi_cpu_infos.wcpu_info.pc[i] = pc;
                g_ssi_cpu_infos.wcpu_info.lr[i] = lr;
                g_ssi_cpu_infos.wcpu_info.sp[i] = sp;
            }
        }
        oal_mdelay(10); /* delay 10 ms */
    }

    return 0;
}

static int ssi_read_bpcu_pc_lr_sp(int trace_en)
{
    int i;
    uint32 reg_low, reg_high, pc, lr, sp;

    /* read pc twice check whether wcpu is runing */
    for (i = 0; i < SSI_CPU_ARM_REG_DUMP_CNT; i++) {
        ssi_write32(HI1103_GLB_CTL_BCPU_LOAD_REG, 0x1);
        oal_mdelay(1);

        reg_low = (uint32)ssi_read32(HI1103_GLB_CTL_BCPU_PC_L_REG);
        reg_high = (uint32)ssi_read32(HI1103_GLB_CTL_BCPU_PC_H_REG);
        pc = reg_low | (reg_high << 16); /* Shift leftwards by 16 bits */

        reg_low = (uint32)ssi_read32(HI1103_GLB_CTL_BCPU_LR_L_REG);
        reg_high = (uint32)ssi_read32(HI1103_GLB_CTL_BCPU_LR_H_REG);
        lr = reg_low | (reg_high << 16); /* Shift leftwards by 16 bits */

        reg_low = (uint32)ssi_read32(HI1103_GLB_CTL_BCPU_SP_L_REG);
        reg_high = (uint32)ssi_read32(HI1103_GLB_CTL_BCPU_SP_H_REG);
        sp = reg_low | (reg_high << 16); /* Shift leftwards by 16 bits */

        ps_print_info("gpio-ssi:read bcpu[%i], pc:0x%x, lr:0x%x, sp:0x%x \n", i, pc, lr, sp);
        if (!pc && !lr && !sp) {
            ps_print_info("bcpu pc lr sp all zero\n");
            if (trace_en) {
                if (ssi_check_bcpu_is_working()) {
                    ps_print_info("bcpu try to enable trace en\n");
                    ssi_write32(HI1103_BCPU_PATCH_BCPU_CFG_TRACE_EN_REG, 0x1);
                    oal_mdelay(1);
                }
                trace_en = 0;
                i = -1;
            }
        } else {
            if (g_ssi_cpu_infos.bcpu_info.reg_flag[i] == 0) {
                g_ssi_cpu_infos.bcpu_info.reg_flag[i] = 1;
                g_ssi_cpu_infos.bcpu_info.pc[i] = pc;
                g_ssi_cpu_infos.bcpu_info.lr[i] = lr;
                g_ssi_cpu_infos.bcpu_info.sp[i] = sp;
            }
        }
        oal_mdelay(10); /* delay 10 ms */
    }

    return 0;
}

int ssi_read_device_arm_register(int trace_en)
{
    int32 ret;

    uint32 reg = (uint32)ssi_read32(HI1103_PMU2_CMU_IR_SYSLDO_WL_C0_ABB_RF_PWR_EN_STS_REG);
    if (reg == 0x3) {
        ps_print_err("0x50003a88 is 0x3, wifi chip maybe enter dft mode , please check!\n");
    }

    ret = ssi_check_wcpu_is_working();
    if (ret < 0) {
        return ret;
    }
    if (ret) {
        ssi_read_wpcu_pc_lr_sp(trace_en);
    }
    bfgx_print_subsys_state();
    ret = ssi_check_bcpu_is_working();
    if (ret < 0) {
        return ret;
    }
    if (ret) {
        ssi_read_bpcu_pc_lr_sp(trace_en);
    }

    return 0;
}

static int ssi_dump_device_aon_regs(unsigned long long module_set)
{
    int ret;
    if (module_set & SSI_MODULE_MASK_AON) {
        ret = ssi_read_reg_info_arry(g_hi1103_aon_reg_full, sizeof(g_hi1103_aon_reg_full) / sizeof(ssi_reg_info *),
                                     g_ssi_is_logfile);
        if (ret) {
            return -OAL_EFAIL;
        }
    }

    if (module_set & SSI_MODULE_MASK_AON_CUT) {
        ret = ssi_read_reg_info_arry(g_hi1103_aon_reg_cut, sizeof(g_hi1103_aon_reg_cut) / sizeof(ssi_reg_info *),
                                     g_ssi_is_logfile);
        if (ret) {
            return -OAL_EFAIL;
        }
    }

    if (module_set & SSI_MODULE_MASK_COEX_CTL) {
        ret = ssi_read_reg_info_arry(g_hi1103_coex_ctl_full, sizeof(g_hi1103_coex_ctl_full) / sizeof(ssi_reg_info *),
                                     g_ssi_is_logfile);
        if (ret) {
            return -OAL_EFAIL;
        }
    }

    return OAL_SUCC;
}

static int ssi_dump_device_wcpu_key_mem(unsigned long long module_set)
{
    int ret;
    int chip_nr;

    if (module_set & SSI_MODULE_MASK_WCPU_KEY_DTCM) {
        if (ssi_check_wcpu_is_working()) {
            chip_nr = get_hi110x_subchip_type();
            if (chip_nr == BOARD_VERSION_HI1103) {
                ret = ssi_read_reg_info(&g_hi1103_w_key_mem, NULL, 0, g_ssi_is_logfile);
            } else if (chip_nr == BOARD_VERSION_HI1105) {
                ret = ssi_read_reg_info(&g_hi1105_w_key_mem, NULL, 0, g_ssi_is_logfile);
            } else {
                ps_print_info("get chip type %d error\n", chip_nr);
                return -OAL_EFAIL;
            }

            if (ret) {
                ps_print_info("wcpu key mem read failed, continue try aon\n");
            }
        } else {
            ps_print_info("wctrl can't dump, wcpu down\n");
        }
    }

    return 0;
}

static int ssi_dump_device_wctrl_regs(unsigned long long module_set)
{
    int ret;

    if (module_set & SSI_MODULE_MASK_WCTRL) {
        if (ssi_check_wcpu_is_working()) {
            ret = ssi_read_reg_info(&g_hi1103_w_ctrl_full, NULL, 0, g_ssi_is_logfile);
            if (ret) {
                goto ssi_fail;
            }

            ret = ssi_read_reg_info(&g_hi1103_rf_w_c0_full, NULL, 0, g_ssi_is_logfile);
            if (ret) {
                goto ssi_fail;
            }

            ret = ssi_read_reg_info(&g_hi1103_rf_w_c1_full, NULL, 0, g_ssi_is_logfile);
            if (ret) {
                goto ssi_fail;
            }
        } else {
            ps_print_info("wctrl can't dump, wcpu down\n");
        }
    }

    return 0;
ssi_fail:
    return ret;
}

static int ssi_dump_device_pcie_regs(unsigned long long module_set)
{
    int ret;

    if (module_set & SSI_MODULE_MASK_PCIE_CFG) {
        if (ssi_check_wcpu_is_working()) {
            ret = ssi_read_reg_info_arry(g_hi1103_pcie_cfg_reg_full,
                                         sizeof(g_hi1103_pcie_cfg_reg_full) / sizeof(ssi_reg_info *),
                                         g_ssi_is_logfile);
            if (ret) {
                goto ssi_fail;
            }
        } else {
            ps_print_info("pcie cfg can't dump, wcpu down\n");
        }
    }

    if (module_set & SSI_MODULE_MASK_PCIE_CUT) {
        if (ssi_check_wcpu_is_working()) {
            ret = ssi_read_reg_info_arry(g_hi1103_pcie_cfg_reg_cut,
                                         sizeof(g_hi1103_pcie_cfg_reg_cut) / sizeof(ssi_reg_info *),
                                         g_ssi_is_logfile);
            if (ret) {
                goto ssi_fail;
            }
        } else {
            ps_print_info("pcie cfg cut can't dump, wcpu down\n");
        }
    }

    if (module_set & SSI_MODULE_MASK_PCIE_DBI) {
        if (ssi_check_wcpu_is_working()) {
            oal_uint32 reg_nums;
            ssi_reg_info **pst_pcie_dbi_reg;
            reg_nums = sizeof(g_hi1103_pcie_dbi_pilot_reg_full) / sizeof(ssi_reg_info *);
            pst_pcie_dbi_reg = g_hi1103_pcie_dbi_pilot_reg_full;

            ret = ssi_read_reg_info_arry(pst_pcie_dbi_reg, reg_nums, g_ssi_is_logfile);
            if (ret) {
                goto ssi_fail;
            }
        } else {
            ps_print_info("pcie dbi can't dump, wcpu down\n");
        }
    }

    return 0;
ssi_fail:
    return ret;
}

static int ssi_dump_device_sdio_regs(unsigned long long module_set)
{
    int ret;

    if (module_set & SSI_MODULE_MASK_SDIO) {
        if (ssi_check_wcpu_is_working()) {
            ret = ssi_read_reg_info(&g_hi1103_pcie_sdio_ctrl_full, NULL, 0, g_ssi_is_logfile);
            if (ret) {
                goto ssi_fail;
            }
        } else {
            ps_print_info("sdio can't dump, wcpu down\n");
        }
    }

    return 0;
ssi_fail:
    return ret;
}

static int ssi_dump_device_bctl_and_exception_regs(unsigned long long module_set)
{
    int ret;

    if (module_set & SSI_MODULE_MASK_BCTRL) {
        if (ssi_check_bcpu_is_working()) {
            ret = ssi_read_reg_info(&g_hi1103_b_ctrl_full, NULL, 0, g_ssi_is_logfile);
            if (ret) {
                goto ssi_fail;
            }

            ret = ssi_read_reg_info(&g_hi1103_rf_bfg_full, NULL, 0, g_ssi_is_logfile);
            if (ret) {
                goto ssi_fail;
            }
        }
    }

    if (module_set & SSI_MODULE_MASK_BCPU_EXCEPT_MEM) {
        if (ssi_check_bcpu_is_working()) {
            ret = ssi_read_reg_info(&g_hi1103_b_except_mem, NULL, 0, g_ssi_is_logfile);
            if (ret) {
                ps_print_info("bcpu key mem read failed\n");
                goto ssi_fail;
            }
        }
    }

    return 0;
ssi_fail:
    return ret;
}

static int ssi_dump_device_arm_regs(unsigned long long module_set, int trace_en)
{
    int ret;

    if (module_set & SSI_MODULE_MASK_ARM_REG) {
        ret = ssi_read_device_arm_register(trace_en);
        if (ret) {
            goto ssi_fail;
        }
    }

    return 0;
ssi_fail:
    return ret;
}

static int ssi_dump_device_tcxo_regs(unsigned long long module_set)
{
    int ret;

    if (module_set & (SSI_MODULE_MASK_AON | SSI_MODULE_MASK_AON_CUT)) {
        ret = ssi_detect_tcxo_is_normal();
        if (ret) {
            ps_print_info("tcxo detect failed, continue dump\n");
        }
    }

    return 0;
}

int ssi_device_regs_dump(unsigned long long module_set)
{
    int ret;

    g_halt_det_cnt = 0;
    memset_s(&g_ssi_cpu_infos, sizeof(g_ssi_cpu_infos), 0, sizeof(g_ssi_cpu_infos));

    ssi_read16(gpio_ssi_reg(SSI_SSI_CTRL));
    ssi_read16(gpio_ssi_reg(SSI_SEL_CTRL));

    ssi_switch_clk(SSI_AON_CLKSEL_SSI);

    ret = ssi_check_device_isalive();
    if (ret) {
        /* try to reset aon */
        ssi_force_reset_reg();
        ssi_switch_clk(SSI_AON_CLKSEL_SSI);
        if (ssi_check_device_isalive()) {
            ps_print_info("after reset aon, ssi still can't work\n");
            goto ssi_fail;
        } else {
            ps_print_info("after reset aon, ssi ok, dump acp/ocp reg\n");
            ssi_check_buck_scp_ocp_status();
            module_set = SSI_MODULE_MASK_COMM;
        }
    } else {
        ssi_check_buck_scp_ocp_status();
    }

    ret = ssi_dump_device_arm_regs(module_set, 0);
    if (ret) {
        goto ssi_fail;
    }

    ret = ssi_check_device_isalive();
    if (ret) {
        goto ssi_fail;
    }

    ret = ssi_dump_device_aon_regs(module_set);
    if (ret != OAL_SUCC) {
        goto ssi_fail;
    }

    ret = ssi_dump_device_arm_regs(module_set, 1);
    if (ret) {
        goto ssi_fail;
    }

    /* detect tcxo clock is normal, trigger */
    ssi_dump_device_tcxo_regs(module_set);

    ret = ssi_dump_device_wcpu_key_mem(module_set);
    if (ret) {
        goto ssi_fail;
    }

    ret = ssi_dump_device_wctrl_regs(module_set);
    if (ret) {
        goto ssi_fail;
    }

    ret = ssi_dump_device_pcie_regs(module_set);
    if (ret) {
        goto ssi_fail;
    }

    ret = ssi_dump_device_sdio_regs(module_set);
    if (ret) {
        goto ssi_fail;
    }

    ret = ssi_dump_device_bctl_and_exception_regs(module_set);
    if (ret) {
        goto ssi_fail;
    }

    ssi_switch_clk(SSI_AON_CLKSEL_TCXO);
    dsm_cpu_info_dump();

    return 0;

ssi_fail:
    ssi_switch_clk(SSI_AON_CLKSEL_TCXO);
    dsm_cpu_info_dump();
    return ret;
}

/*
 * SSI�Ĵ�����ӡ�����ݷ��������ҵ�񳡾���
 * ����SSI��ӡ�ļĴ�����С�ʹ�ӡƵ�ʣ��������ٶ�ҵ���Ӱ��
 * ����Android user/eng ������user beta �� user release�汾
 * ���user releaseӦ�ϸ�����SSI DUMP����
 **/
OAL_STATIC unsigned long long ssi_get_module_comm(ssi_err_type etype)
{
    oal_reference(etype);
    return SSI_MODULE_MASK_COMM;
}

OAL_STATIC int32 ssi_private_comm(ssi_err_type etype)
{
    oal_reference(etype);
    return 0;
}

OAL_STATIC int ssi_get_err_dump_level(void)
{
    int auto_switch = ssi_clk_auto_switch_is_support();
    hi110x_release_vtype vtype = hi110x_get_release_type();

    ps_print_info("get verison type is %d\n", vtype);
    if (vtype == HI110X_VTYPE_RELEASE) {
        return SSI_ERR_LOG_LEVEL_CLOSE;
    } else if (vtype == HI110X_VTYPE_RELEASE_DEBUG) {
        if (auto_switch == OAL_TRUE) {
            return SSI_ERR_LOG_LEVEL_FULL;
        } else {
            return SSI_ERR_LOG_LEVEL_CUT;
        }
    } else if (vtype == HI110X_VTYPE_DEBUG) {
        return SSI_ERR_LOG_LEVEL_FULL;
    } else {
        return SSI_ERR_LOG_LEVEL_CLOSE;
    }
}

/* SSI_ERR_HCC_EXCP_SDIO */
OAL_STATIC unsigned long long ssi_get_module_hcc_excp_sdio(ssi_err_type etype)
{
    unsigned long long flag;

    int err_level = ssi_get_err_dump_level();
    oal_reference(etype);
    if (err_level == SSI_ERR_LOG_LEVEL_CLOSE) {
        if (oal_print_rate_limit(24 * PRINT_RATE_HOUR)) { /* 24Сʱ��ӡһ�� */
            flag = (bfgx_is_shutdown() == true) ? (SSI_MODULE_MASK_ARM_REG | SSI_MODULE_MASK_AON_CUT) : (0);
        } else {
            flag = SSI_MODULE_NON_FLAG;
        }
    } else if (err_level == SSI_ERR_LOG_LEVEL_CUT) {
        flag = (bfgx_is_shutdown() == true) ? (SSI_MODULE_MASK_COMM) : (SSI_MODULE_MASK_ARM_REG);
    } else if (err_level == SSI_ERR_LOG_LEVEL_FULL) {
        flag = SSI_MODULE_MASK_COMM;
    } else {
        flag = SSI_MODULE_NON_FLAG;
    }

    return flag;
}

/* SSI_ERR_HCC_EXCP_PCIE */
OAL_STATIC unsigned long long ssi_get_module_hcc_excp_pcie(ssi_err_type etype)
{
    int err_level;
    unsigned long long cut_flag, full_flag;
    unsigned long long flag;
    oal_reference(etype);

    cut_flag = (SSI_MODULE_MASK_ARM_REG | SSI_MODULE_MASK_AON_CUT | SSI_MODULE_MASK_PCIE_CUT);
    full_flag = (SSI_MODULE_MASK_COMM | SSI_MODULE_MASK_PCIE_CFG | SSI_MODULE_MASK_PCIE_DBI);

    err_level = ssi_get_err_dump_level();
    if (err_level == SSI_ERR_LOG_LEVEL_CLOSE) {
        if (oal_print_rate_limit(24 * PRINT_RATE_HOUR)) { /* 24Сʱ��ӡһ�� */
            flag = (bfgx_is_shutdown() == true) ? cut_flag : 0;
        } else {
            flag = SSI_MODULE_NON_FLAG;
        }
    } else if (err_level == SSI_ERR_LOG_LEVEL_CUT) {
        flag = (bfgx_is_shutdown() == true) ? cut_flag : (SSI_MODULE_MASK_ARM_REG);
    } else if (err_level == SSI_ERR_LOG_LEVEL_FULL) {
        flag = full_flag;
    } else {
        flag = SSI_MODULE_NON_FLAG;
    }

    return flag;
}

OAL_STATIC unsigned long long ssi_get_module_kirin_noc(ssi_err_type etype)
{
    int err_level;
    unsigned long long cut_flag, full_flag;
    unsigned long long flag;

    oal_reference(etype);

    cut_flag = (SSI_MODULE_MASK_ARM_REG);
    full_flag = (SSI_MODULE_MASK_COMM | SSI_MODULE_MASK_PCIE_CFG);

    err_level = ssi_get_err_dump_level();
    if (err_level == SSI_ERR_LOG_LEVEL_CLOSE) {
        flag = cut_flag;
    } else {
        flag = full_flag;
    }

    return flag;
}

OAL_STATIC unsigned long long ssi_get_module_pcie_check_linkfail(ssi_err_type etype)
{
    int err_level;
    unsigned long long cut_flag, full_flag;
    unsigned long long flag;

    oal_reference(etype);

    cut_flag = SSI_MODULE_NON_FLAG;
    full_flag = (SSI_MODULE_MASK_COMM | SSI_MODULE_MASK_PCIE_CFG);

    err_level = ssi_get_err_dump_level();
    if (err_level == SSI_ERR_LOG_LEVEL_CLOSE) {
        flag = cut_flag;
    } else if (err_level == SSI_ERR_LOG_LEVEL_CUT) {
        flag = (bfgx_is_shutdown() == true) ? full_flag : SSI_MODULE_MASK_ARM_REG;
    } else {
        flag = full_flag;
    }

    return flag;
}

OAL_STATIC unsigned long long ssi_get_module_pcie_wait_boot_fail(ssi_err_type etype)
{
    int err_level;
    unsigned long long cut_flag, full_flag;
    unsigned long long flag;

    oal_reference(etype);

    cut_flag = (SSI_MODULE_MASK_ARM_REG);
    full_flag = (SSI_MODULE_MASK_COMM);

    err_level = ssi_get_err_dump_level();
    if (err_level == SSI_ERR_LOG_LEVEL_CLOSE) {
        if (oal_print_rate_limit(1 * PRINT_RATE_HOUR)) { /* 1Сʱ��ӡһ�� */
            flag = cut_flag;
        } else {
            flag = SSI_MODULE_NON_FLAG;
        }
    } else {
        flag = full_flag;
    }

    return flag;
}

OAL_STATIC unsigned long long ssi_get_module_pcie_sr_wakeup_fail(ssi_err_type etype)
{
    unsigned long long flag = SSI_MODULE_NON_FLAG;
    hi110x_release_vtype vtype;
    oal_reference(etype);

    vtype = hi110x_get_release_type();
    if (vtype == HI110X_VTYPE_DEBUG) {
        flag = (SSI_MODULE_MASK_COMM | SSI_MODULE_MASK_PCIE_CFG);
    }

    return flag;
}

OAL_STATIC unsigned long long ssi_get_module_pcie_sr_wakeup_retry_fail(ssi_err_type etype)
{
    int err_level;
    unsigned long long cut_flag, full_flag;
    unsigned long long flag;
    oal_reference(etype);

    cut_flag = (SSI_MODULE_MASK_ARM_REG | SSI_MODULE_MASK_AON_CUT | SSI_MODULE_MASK_PCIE_CUT);
    full_flag = (SSI_MODULE_MASK_COMM | SSI_MODULE_MASK_PCIE_CFG);

    err_level = ssi_get_err_dump_level();
    if (err_level == SSI_ERR_LOG_LEVEL_CLOSE) {
        if (oal_print_rate_limit(1 * PRINT_RATE_HOUR)) { /* 1Сʱ��ӡһ�� */
            flag = (bfgx_is_shutdown() == true) ? cut_flag : 0;
        } else {
            flag = SSI_MODULE_MASK_ARM_REG;
        }
    } else if (err_level == SSI_ERR_LOG_LEVEL_CUT) {
        flag = (bfgx_is_shutdown() == true) ? cut_flag : (SSI_MODULE_MASK_ARM_REG);
    } else if (err_level == SSI_ERR_LOG_LEVEL_FULL) {
        flag = full_flag;
    } else {
        flag = SSI_MODULE_NON_FLAG;
    }

    return flag;
}

OAL_STATIC unsigned long long ssi_get_module_pcie_pwrup_fail(ssi_err_type etype)
{
    int err_level;
    unsigned long long cut_flag, full_flag;
    unsigned long long flag;
    oal_reference(etype);

    cut_flag = SSI_MODULE_MASK_PCIE_CUT_SET;
    full_flag = SSI_MODULE_MASK_PCIE_FULL;

    err_level = ssi_get_err_dump_level();
    if (err_level == SSI_ERR_LOG_LEVEL_CLOSE) {
        if (oal_print_rate_limit(24 * PRINT_RATE_HOUR)) { /* 24Сʱ��ӡһ�� */
            flag = (bfgx_is_shutdown() == true) ? cut_flag : 0;
            ps_print_err("bfgx is shutdown\n");
        } else {
            flag = SSI_MODULE_MASK_ARM_REG;
            ps_print_err("bfgx is working\n");
        }
    } else if (err_level == SSI_ERR_LOG_LEVEL_CUT) {
        flag = (bfgx_is_shutdown() == true) ? cut_flag : (SSI_MODULE_MASK_ARM_REG);
    } else if (err_level == SSI_ERR_LOG_LEVEL_FULL) {
        flag = full_flag;
    } else {
        flag = SSI_MODULE_NON_FLAG;
    }

    return flag;
}

/* PCIE ����һ�εĴ��󣬱���ö�ٴ���͵�һ��WiFi�ػ����� */
OAL_STATIC unsigned long long ssi_get_module_pcie_proc_once_fail(ssi_err_type etype)
{
    int err_level;
    unsigned long long cut_flag, full_flag;
    unsigned long long flag;

    oal_reference(etype);

    cut_flag = (SSI_MODULE_MASK_PCIE_CUT_SET);
    full_flag = (SSI_MODULE_MASK_PCIE_FULL);

    err_level = ssi_get_err_dump_level();
    if (err_level == SSI_ERR_LOG_LEVEL_CLOSE) {
        if (oal_print_rate_limit(1 * PRINT_RATE_MINUTE)) { /* 1���Ӵ�ӡһ�� */
            flag = cut_flag;
        } else {
            flag = SSI_MODULE_NON_FLAG;
        }
    } else {
        flag = full_flag;
    }

    return flag;
}

OAL_STATIC unsigned long long ssi_get_module_sdio_reenum_fail(ssi_err_type etype)
{
    int err_level;
    unsigned long long cut_flag, full_flag;
    unsigned long long flag;

    oal_reference(etype);

    cut_flag = (SSI_MODULE_MASK_ARM_REG | SSI_MODULE_MASK_AON_CUT | SSI_MODULE_MASK_SDIO);
    full_flag = (SSI_MODULE_MASK_COMM | SSI_MODULE_MASK_SDIO);

    err_level = ssi_get_err_dump_level();
    if (err_level == SSI_ERR_LOG_LEVEL_CLOSE) {
        if (oal_print_rate_limit(1 * PRINT_RATE_HOUR)) { /* 1Сʱ��ӡһ�� */
            flag = (bfgx_is_shutdown() == true) ? SSI_MODULE_MASK_ARM_REG : 0;
        } else {
            flag = SSI_MODULE_NON_FLAG;
        }
    } else if (err_level == SSI_ERR_LOG_LEVEL_CUT) {
        flag = (bfgx_is_shutdown() == true) ? cut_flag : (SSI_MODULE_MASK_ARM_REG);
    } else if (err_level == SSI_ERR_LOG_LEVEL_FULL) {
        flag = full_flag;
    } else {
        flag = SSI_MODULE_NON_FLAG;
    }

    return flag;
}

OAL_STATIC unsigned long long ssi_get_module_sdio_probeinit_fail(ssi_err_type etype)
{
    int err_level;
    unsigned long long flag;

    oal_reference(etype);

    err_level = ssi_get_err_dump_level();
    if (err_level == SSI_ERR_LOG_LEVEL_CLOSE) {
        flag = SSI_MODULE_NON_FLAG;
    } else {
        flag = SSI_MODULE_MASK_COMM | SSI_MODULE_MASK_SDIO;
    }

    return flag;
}

OAL_STATIC unsigned long long ssi_get_module_bfg_wkup_fail(ssi_err_type etype)
{
    int err_level;
    unsigned long long full_flag;
    unsigned long long flag;

    oal_reference(etype);

    /* bfg�쳣��ӡSSI �п��ܵ���PCIE�쳣��
       ����PCIE NOC������ֻ��root��beta �汾��ӡ */
    full_flag = (SSI_MODULE_MASK_ARM_REG | SSI_MODULE_MASK_AON | SSI_MODULE_MASK_COEX_CTL | SSI_MODULE_MASK_BCTRL);

    err_level = ssi_get_err_dump_level();
    if (err_level == SSI_ERR_LOG_LEVEL_CLOSE) {
        if (oal_print_rate_limit(1 * PRINT_RATE_HOUR)) { /* 1Сʱ��ӡһ�� */
            flag = ((wlan_is_shutdown() == true) && (g_ssi_dump_en != 0)) ? SSI_MODULE_MASK_ARM_REG : 0;
        } else {
            flag = SSI_MODULE_NON_FLAG;
        }
    } else if (err_level == SSI_ERR_LOG_LEVEL_CUT) {
        flag = ((wlan_is_shutdown() == true) && (g_ssi_dump_en != 0)) ? full_flag : 0;
    } else if (err_level == SSI_ERR_LOG_LEVEL_FULL) {
        flag = full_flag;
    } else {
        flag = SSI_MODULE_NON_FLAG;
    }

    return flag;
}

OAL_STATIC unsigned long long ssi_proc_fw_down_fail(ssi_err_type etype, int32 is_sdio)
{
    hi110x_release_vtype vtype;
    unsigned long long flag = SSI_MODULE_NON_FLAG;

    oal_reference(etype);

    vtype = hi110x_get_release_type();
    if (vtype == HI110X_VTYPE_DEBUG) {
        flag = SSI_MODULE_MASK_COMM;
        if (is_sdio == OAL_TRUE) {
            flag |= SSI_MODULE_MASK_SDIO;
        }
    }

    return flag;
}

OAL_STATIC unsigned long long ssi_get_module_fw_down_fail(ssi_err_type etype)
{
    return ssi_proc_fw_down_fail(etype, 0);
}

OAL_STATIC unsigned long long ssi_get_module_fw_down_sdio_fail(ssi_err_type etype)
{
    return ssi_proc_fw_down_fail(etype, 1);
}

OAL_STATIC unsigned long long ssi_get_module_pcie_gpio_wkup_fail(ssi_err_type etype)
{
    int err_level;
    unsigned long long flag = SSI_MODULE_NON_FLAG;

    oal_reference(etype);

    err_level = ssi_get_err_dump_level();
    if (err_level == SSI_ERR_LOG_LEVEL_CLOSE) {
        ps_print_err("dump when dfr\n");
    } else if (err_level == SSI_ERR_LOG_LEVEL_CUT) {
        flag = (bfgx_is_shutdown() == true) ? (SSI_MODULE_MASK_COMM) : (SSI_MODULE_MASK_ARM_REG);
    } else if (err_level == SSI_ERR_LOG_LEVEL_FULL) {
        flag = SSI_MODULE_MASK_COMM;
    } else {
        flag = SSI_MODULE_NON_FLAG;
    }

    return flag;
}

OAL_STATIC unsigned long long ssi_get_module_sdio_wkup_fail(ssi_err_type etype)
{
    hi110x_release_vtype vtype;
    unsigned long long flag = SSI_MODULE_NON_FLAG;

    oal_reference(etype);

    vtype = hi110x_get_release_type();
    if (vtype == HI110X_VTYPE_DEBUG) {
        if (oal_print_rate_limit(30 * PRINT_RATE_SECOND)) { /* 30s��ӡһ�� */
            flag = SSI_MODULE_MASK_COMM;
        }
    }

    if (flag == 0) {
        ps_print_err("dump when dfr\n");
    }

    return flag;
}

OAL_STATIC unsigned long long ssi_get_module_wlan_pwr_off_fail(ssi_err_type etype)
{
    int err_level;
    unsigned long long flag;

    oal_reference(etype);

    err_level = ssi_get_err_dump_level();
    if (err_level == SSI_ERR_LOG_LEVEL_CLOSE) {
        if (oal_print_rate_limit(24 * PRINT_RATE_HOUR)) { /* 24Сʱ��ӡһ�� */
            flag = (bfgx_is_shutdown() == true) ? SSI_MODULE_MASK_ARM_REG : 0;
        } else {
            flag = SSI_MODULE_NON_FLAG;
        }
    } else if (err_level == SSI_ERR_LOG_LEVEL_CUT) {
        if (oal_print_rate_limit(1 * PRINT_RATE_HOUR)) { /* 1Сʱ��ӡһ�� */
            flag = (bfgx_is_shutdown() == true) ? (SSI_MODULE_MASK_COMM) : (SSI_MODULE_MASK_ARM_REG);
        } else {
            flag = (bfgx_is_shutdown() == true) ? (SSI_MODULE_MASK_ARM_REG) : (0);
        }
    } else if (err_level == SSI_ERR_LOG_LEVEL_FULL) {
        if (oal_print_rate_limit(1 * PRINT_RATE_HOUR)) { /* 1Сʱ��ӡһ�� */
            flag = SSI_MODULE_MASK_COMM;
        } else {
            flag = SSI_MODULE_MASK_ARM_REG;
        }
    } else {
        flag = SSI_MODULE_NON_FLAG;
    }

    return flag;
}

OAL_STATIC unsigned long long ssi_get_module_bfgx_dump_fail(ssi_err_type etype, unsigned long long flag_t)
{
    unsigned long long flag = SSI_MODULE_NON_FLAG;

    int auto_switch = ssi_clk_auto_switch_is_support();
    hi110x_release_vtype vtype = hi110x_get_release_type();

    oal_reference(etype);

    if (auto_switch == OAL_TRUE) {
        if (oal_print_rate_limit(30 * PRINT_RATE_SECOND)) { /* 30 */
            if (vtype != HI110X_VTYPE_RELEASE) {
                ps_print_info("bfgx exception ssi dump starts\n");
                flag = flag_t;
            }
        }
    } else {
        ps_print_warning("subchip type not support bfgx exception ssi dump\n");
    }
    return flag;
}

OAL_STATIC unsigned long long ssi_get_module_bfgx_open_fail(ssi_err_type etype)
{
    return ssi_get_module_bfgx_dump_fail(etype, (SSI_MODULE_MASK_COMM | SSI_MODULE_MASK_BCTRL));
}

OAL_STATIC unsigned long long ssi_get_module_bfgx_hb_timeout_fail(ssi_err_type etype)
{
    return ssi_get_module_bfgx_dump_fail(etype, (SSI_MODULE_MASK_COMM | SSI_MODULE_MASK_BCTRL | SSI_MODULE_MASK_WCTRL));
}

OAL_STATIC ssi_err_stru g_ssi_err_infos[] = {
    declare_ssi_err_info(SSI_ERR_COMM, ssi_get_module_comm, ssi_private_comm), /* 1st fixed */
    declare_ssi_err_info(SSI_ERR_HCC_EXCP_SDIO, ssi_get_module_hcc_excp_sdio, NULL),
    declare_ssi_err_info(SSI_ERR_HCC_EXCP_PCIE, ssi_get_module_hcc_excp_pcie, NULL),
    declare_ssi_err_info(SSI_ERR_PCIE_KIRIN_NOC, ssi_get_module_kirin_noc, NULL),
    declare_ssi_err_info(SSI_ERR_PCIE_CHECK_LINK_FAIL, ssi_get_module_pcie_check_linkfail, NULL),
    declare_ssi_err_info(SSI_ERR_PCIE_WAIT_BOOT_TIMEOUT, ssi_get_module_pcie_wait_boot_fail, NULL),
    declare_ssi_err_info(SSI_ERR_PCIE_SR_WAKEUP_FAIL, ssi_get_module_pcie_sr_wakeup_fail, NULL),
    declare_ssi_err_info(SSI_ERR_PCIE_SR_WAKEUP_RETRY_FAIL, ssi_get_module_pcie_sr_wakeup_retry_fail, NULL),
    declare_ssi_err_info(SSI_ERR_PCIE_POWER_UP_FAIL, ssi_get_module_pcie_pwrup_fail, NULL),
    declare_ssi_err_info(SSI_ERR_PCIE_FST_POWER_OFF_FAIL, ssi_get_module_pcie_proc_once_fail, NULL),
    declare_ssi_err_info(SSI_ERR_PCIE_ENUM_FAIL, ssi_get_module_pcie_proc_once_fail, NULL),
    declare_ssi_err_info(SSI_ERR_SDIO_REENUM_FAIL, ssi_get_module_sdio_reenum_fail, NULL),
    declare_ssi_err_info(SSI_ERR_SDIO_REINIT_FAIL, ssi_get_module_sdio_reenum_fail, NULL),
    declare_ssi_err_info(SSI_ERR_SDIO_PROBE_INIT_FAIL, ssi_get_module_sdio_probeinit_fail, NULL),
    declare_ssi_err_info(SSI_ERR_SDIO_PROBE_FAIL, ssi_get_module_sdio_probeinit_fail, NULL),
    declare_ssi_err_info(SSI_ERR_BFG_WAKE_UP_FAIL, ssi_get_module_bfg_wkup_fail, NULL),
    declare_ssi_err_info(SSI_ERR_FIRMWARE_DOWN_FAIL, ssi_get_module_fw_down_fail, NULL),
    declare_ssi_err_info(SSI_ERR_FIRMWARE_DOWN_SDIO_FAIL, ssi_get_module_fw_down_sdio_fail, NULL),
    declare_ssi_err_info(SSI_ERR_PCIE_GPIO_WAKE_FAIL, ssi_get_module_pcie_gpio_wkup_fail, NULL),
    declare_ssi_err_info(SSI_ERR_SDIO_CMD_WAKE_FAIL, ssi_get_module_sdio_wkup_fail, NULL),
    declare_ssi_err_info(SSI_ERR_SDIO_GPIO_WAKE_FAIL, ssi_get_module_sdio_wkup_fail, NULL),
    declare_ssi_err_info(SSI_ERR_WLAN_POWEROFF_FAIL, ssi_get_module_wlan_pwr_off_fail, NULL),
    declare_ssi_err_info(SSI_ERR_BFGX_OPEN_FAIL, ssi_get_module_bfgx_open_fail, NULL),
    declare_ssi_err_info(SSI_ERR_BFGX_HEART_TIMEOUT, ssi_get_module_bfgx_hb_timeout_fail, NULL),
};

OAL_STATIC ssi_err_stru *ssi_find_idx_by_etype(ssi_err_type etype)
{
    int idx;
    int err_size, i;

    err_size = oal_array_size(g_ssi_err_infos);

    if (oal_warn_on((uint32)etype >= (uint32)SSI_ERR_BUT)) {
        ps_print_warning("unkown ssi etype=%d\n", etype);
        etype = SSI_ERR_COMM;
    }

    for (i = 0; i < err_size; i++) {
        if (g_ssi_err_infos[i].etype == etype) {
            break;
        }
    }

    if (oal_warn_on(i == err_size)) {
        /* can't found */
        ps_print_warning("can't found ssi etype=%d\n", etype);
        idx = 0; /* comm err type */
    } else {
        /* found */
        idx = i;
    }

    return &g_ssi_err_infos[idx];
}

int ssi_dump_err_reg(ssi_err_type etype)
{
    int ret;
    unsigned long long module_set = 0;
    ssi_err_stru *pst_err_info;

    pst_err_info = ssi_find_idx_by_etype(etype);
    if (pst_err_info == NULL) {
        return -OAL_ENODEV;
    }

    /* print err info */
    ps_print_err("etype=%d , name=%s\n", pst_err_info->etype, pst_err_info->type_name);

    if (pst_err_info->ssi_get_module_flag != NULL) {
        module_set = pst_err_info->ssi_get_module_flag(pst_err_info->etype);
    } else {
        ps_print_err("ssi_get_module_flag is null, bypass dump\n");
    }

    ps_print_err("ssi_err_module_flag = 0x%llx\n", module_set);

    /* dump callback before dump ssi err regs */
    oal_warn_on(module_set != 0);

    ret = ssi_dump_device_regs(module_set);

    return ret;
}

static int32 ssi_file_test_pre(void)
{
    // waring: fpga version should set 300801c0 1 to let host control ssi
    /* first set ssi clk ctl */
    if (ssi_switch_clk(SSI_AON_CLKSEL_SSI) != BOARD_SUCC) {
        ps_print_err("set ssi clk fail\n");
        return BOARD_FAIL;
    }
    // env init
#ifdef BFGX_UART_DOWNLOAD_SUPPORT
    /* set bootloader deadbeaf */
    if (ssi_write32(0x8010010c, 0xbeaf) != BOARD_SUCC) {
        ps_print_err("set flag:beaf fail\n");
        return BOARD_FAIL;
    }
    if (ssi_write32(0x8010010e, 0xdead) != BOARD_SUCC) {
        ps_print_err("set flag:dead fail\n");
        return BOARD_FAIL;
    }
#else
    /* set wcpu wait */
    if (ssi_write32(HI1103_GLB_CTL_WCPU_WAIT_CTL_REG, 0x1) != BOARD_SUCC) {
        ps_print_err("set wcpu wait fail\n");
        return BOARD_FAIL;
    }

    /* reset wcpu */
    if (ssi_write32(HI1103_W_CTL_WTOPCRG_SOFT_RESET_REG, 0xfe5e) != BOARD_SUCC) { /* ���帴λ */
    }
    /* boot flag */
    if (ssi_write32(HI1103_GLB_CTL_PWR_ON_LABLE_REG, 0xbeaf) != BOARD_SUCC) {
        ps_print_err("set boot flag fail\n");
        return BOARD_FAIL;
    }
    /* dereset bcpu */
    if (ssi_write32(HI1103_GLB_CTL_SOFT_RST_BCPU_REG, 1) != BOARD_SUCC) {
        ps_print_err("dereset bcpu\n");
        return BOARD_FAIL;
    }
#endif

    return BOARD_SUCC;
}

static int32 ssi_file_test_post(void)
{
    /* let cpu go */
#ifdef BFGX_UART_DOWNLOAD_SUPPORT
    /* reset bcpu */
    if (ssi_write32(HI1103_GLB_CTL_SOFT_RST_BCPU_REG, 0) != BOARD_SUCC) {
        ps_print_err("reset bcpu set 0 fail\n");
        return BOARD_FAIL;
    }
    if (ssi_write32(HI1103_GLB_CTL_SOFT_RST_BCPU_REG, 1) != BOARD_SUCC) {
        ps_print_err("reset bcpu set 1 fail\n");
        return BOARD_FAIL;
    }
#else
    /* clear b wait */
    if (ssi_write32(HI1103_GLB_CTL_BCPU_WAIT_CTL_REG, 0x0) != BOARD_SUCC) {
        ps_print_err("clear b wait\n");
        return BOARD_FAIL;
    }
    /* clear w wait */
    if (ssi_write32(HI1103_GLB_CTL_WCPU_WAIT_CTL_REG, 0x0) != BOARD_SUCC) {
        ps_print_err("clear w wait\n");
        return BOARD_FAIL;
    }
#endif
    /* reset clk */
    if (ssi_switch_clk(SSI_AON_CLKSEL_TCXO) != BOARD_SUCC) {
        ps_print_err("set ssi clk fail\n");
        return BOARD_FAIL;
    }

    return BOARD_SUCC;
}

STATIC int32 ssi_file_test(ssi_trans_test_st *pst_ssi_test)
{
    int32 i;
    int32 ret;

    if (pst_ssi_test == NULL) {
        return BOARD_FAIL;
    }
    pst_ssi_test->trans_len = 0;

#ifndef BFGX_UART_DOWNLOAD_SUPPORT
    hi1103_chip_power_on();
    hi1103_bfgx_enable();
    if (hi1103_wifi_enable()) {
        ps_print_err("hi1103_wifi_enable failed!\n");
        return BOARD_FAIL;
    }
#endif

    ret = ssi_file_test_pre();
    if (ret != BOARD_SUCC) {
        return ret;
    }

    /* file download */
    for (i = 0; i < sizeof(g_ssi_file) / sizeof(ssi_file_st); i++) {
        if (do_ssi_file_test(&g_ssi_file[i], pst_ssi_test) != BOARD_SUCC) {
            ps_print_err("%s write %d error\n", g_ssi_file[i].file_name, g_ssi_file[i].write_addr);
            return BOARD_FAIL;
        }
    }

    ret = ssi_file_test_post();
    if (ret != BOARD_SUCC) {
        return ret;
    }

    return BOARD_SUCC;
}

static int32 do_ssi_mem_test(ssi_trans_test_st *pst_ssi_test)
{
    uint32 i;
    uint32 ul_write_base = 0x0;
    uint32 ul_addr;
    int32 l_ret;
    if (pst_ssi_test == NULL) {
        return BOARD_FAIL;
    }

    for (i = 0; i < pst_ssi_test->trans_len; i++) {
        ul_addr = ul_write_base + 2 * i;  // ��2�ֽڶ�д
        l_ret = ssi_write32(ul_addr, SSI_WRITE_DATA);
        if (l_ret != BOARD_SUCC) {
            ps_print_err(" write data error, ul_addr=0x%x, l_ret=%d\n", ul_addr, l_ret);
            return l_ret;
        }
        l_ret = ssi_read32(ul_addr);
        if (l_ret != SSI_WRITE_DATA) {
            ps_print_err("read write 0x%x error, expect:0x5a5a,actual:0x%x\n", ul_addr, l_ret);
            return l_ret;
        }
    }
    return BOARD_SUCC;
}

int32 ssi_download_test(ssi_trans_test_st *pst_ssi_test)
{
    int32 l_ret = BOARD_FAIL;
    const uint32 ul_test_trans_len = 1024;

    struct timeval stime, etime;

    if (pst_ssi_test == NULL) {
        return BOARD_FAIL;
    }
    pst_ssi_test->trans_len = ul_test_trans_len;
    if (ssi_request_gpio(SSI_CLK_GPIO, SSI_DATA_GPIO) != BOARD_SUCC) {
        ps_print_err("ssi_request_gpio fail\n");
        goto fail_process;
    }

    do_gettimeofday(&stime);
    switch (pst_ssi_test->test_type) {
        case SSI_MEM_TEST:
            l_ret = do_ssi_mem_test(pst_ssi_test);
            break;
        case SSI_FILE_TEST:
            l_ret = ssi_file_test(pst_ssi_test);
            break;
        default:
            ps_print_err("error type=%d\n", pst_ssi_test->test_type);
            break;
    }
    do_gettimeofday(&etime);
    ssi_free_gpio();
    if (l_ret != BOARD_SUCC) {
        goto fail_process;
    }
    /* 1000 is mean uint conversion */
    pst_ssi_test->used_time = (etime.tv_sec - stime.tv_sec) * 1000 + (etime.tv_usec - stime.tv_usec) / 1000;
    pst_ssi_test->send_status = 0;
    return BOARD_SUCC;
fail_process:
    pst_ssi_test->used_time = 0;
    pst_ssi_test->send_status = -1;
    return BOARD_FAIL;
}

#endif /* #ifdef _PRE_CONFIG_GPIO_TO_SSI_DEBUG */

