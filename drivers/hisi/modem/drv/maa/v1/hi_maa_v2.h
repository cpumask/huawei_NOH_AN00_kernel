
#define MAA_DDR_RLS_INT_MASK BIT(16)
#define MAA_MDM_DDR_RLS_INT_MASK BIT(18)
#define MAA_RLS_FULL_WR_INT_MASK BIT(17)
#define MAA_MDM_RLS_FULL_WR_INT_MASK BIT(19)
#define MAA_SAFE_CHK_INT_MASK BIT(20)

#define MAA_GLB_CTRL (0x0)             /* ȫ�ֿ��ƼĴ��� */
#define MAA_AXI_BUS_CFG (0x4)          /* AXI_MST OS���üĴ��� */
#define MAA_VERSION (0x8)              /* �汾�Ĵ��� */
#define MAA_LP_EN (0xC)                /* �͹���ʹ�ܼĴ��� */
#define MAA_LP_STAT (0x10)             /* �͹���״̬�Ĵ��� */
#define MAA_AUTOCLK_RESP_BYPASS (0x14) /* �͹���״̬�Ĵ��� */
#define MAA_OPIPE_INT_MSK (0x18)       /* MAA�ж����μĴ���������ACORE */
#define MAA_OPIPE_INT_STAT (0x1C)      /* MAA�ж����μĴ���������ACORE */
#define MAA_OPIPE_INT_RAW (0x20)       /* MAA�ж����μĴ���������ACORE */
#define MAA_INT_MSK_DISPATCH (0x24)    /* MAA�ж����μĴ���������ACORE */
#define MAA_INT_STAT_DISPATCH (0x28)   /* MAA�ж����μĴ���������ACORE */
#define MAA_IPIPE_INT_RAW0 (0x30)      /* MAA�ж�ԭʼ״̬�Ĵ���������ACORE */
#define MAA_IPIPE_INT_MSK0 (0x34)      /* MAA�ж�ԭʼ״̬�Ĵ���������ACORE */
#define MAA_IPIPE_INT_STAT0 (0x38)     /* MAA�ж�ԭʼ״̬�Ĵ���������ACORE */
#define MAA_IPIPE_INT_RAW1 (0x3C)      /* MAA�ж�ԭʼ״̬�Ĵ���������ACORE */
#define MAA_IPIPE_INT_MSK1 (0x40)      /* MAA�ж�ԭʼ״̬�Ĵ���������ACORE */
#define MAA_IPIPE_INT_STAT1 (0x44)     /* MAA�ж�ԭʼ״̬�Ĵ���������ACORE */
#define MAA_IPIPE_INT_RAW2 (0x48)      /* MAA�ж�ԭʼ״̬�Ĵ���������L2HAC */
#define MAA_IPIPE_INT_MSK2 (0x4C)      /* MAA�ж�ԭʼ״̬�Ĵ���������L2HAC */
#define MAA_IPIPE_INT_STAT2 (0x50)     /* MAA�ж�ԭʼ״̬�Ĵ���������L2HAC */
#define MAA_IPIPE_INT_RAW3 (0x54)      /* MAA�ж�ԭʼ״̬�Ĵ���������4G CCORE */
#define MAA_IPIPE_INT_MSK3 (0x58)      /* MAA�ж�ԭʼ״̬�Ĵ���������4G CCORE */
#define MAA_IPIPE_INT_STAT3 (0x5C)     /* MAA�ж�ԭʼ״̬�Ĵ���������4G CCORE */
#define MAA_IPIPE_INT_RAW4 (0x60)      /* MAA�ж�ԭʼ״̬�Ĵ���������5G CCORE */
#define MAA_IPIPE_INT_MSK4 (0x64)      /* MAA�ж�ԭʼ״̬�Ĵ���������5G CCORE */
#define MAA_IPIPE_INT_STAT4 (0x68)     /* MAA�ж�ԭʼ״̬�Ĵ���������5G CCORE */
#define MAA_IPIPE_INT_RAW5 (0x6C)      /* MAA�ж�ԭʼ״̬�Ĵ���������HL1C */
#define MAA_IPIPE_INT_MSK5 (0x70)      /* MAA�ж�ԭʼ״̬�Ĵ���������HL1C */
#define MAA_IPIPE_INT_STAT5 (0x74)     /* MAA�ж�ԭʼ״̬�Ĵ���������HL1C */
#define MAA_IPIPE_INT_RAW6 (0x78)      /* MAA�ж�ԭʼ״̬�Ĵ���������HL1C */
#define MAA_IPIPE_INT_MSK6 (0x7C)      /* MAA�ж�ԭʼ״̬�Ĵ���������HL1C */
#define MAA_IPIPE_INT_STAT6 (0x80)     /* MAA�ж�ԭʼ״̬�Ĵ���������HL1C */
#define MAA_IPIPE_INT_RAW7 (0x84)      /* MAA�ж�ԭʼ״̬�Ĵ���������HL1C */
#define MAA_IPIPE_INT_MSK7 (0x88)      /* MAA�ж�ԭʼ״̬�Ĵ���������HL1C */
#define MAA_IPIPE_INT_STAT7 (0x8C)     /* MAA�ж�ԭʼ״̬�Ĵ���������HL1C */
#define MAA_HAC_GIF_128SPLIT (0x180)   /* MAA HAC_GIFͨ����д128��ֿ��� */
#define MAA_HAC_GIF_MAX_LEN (0x184)    /* MAA HAC_GIFͨ��DMA MAX_LEN */
#define MAA_HAC_GIF_PRIOR (0x188)      /* MAA HAC_GIFͨ�����ȼ� */
#define MAA_TIMER_EN (0x190)           /* MAAʹ��fifo��ʱtimer */
#define MAA_TIMER_WATCHCNT (0x194)     /* MAA ����TIMER��������ֵ */
#define MAA_ALLOC_ADDR_L(opipes, src_ptrs) (0x200 + 0x80 * opipes + 0x8 * src_ptrs)
#define MAA_ALLOC_ADDR_H(opipes, src_ptrs) (0x204 + 0x80 * opipes + 0x8 * src_ptrs)
#define MAA_RLS_POOL_ADDR_L (0x800)   /* �ͷ�Ŀ���ڴ�ػ���ַ��λ�Ĵ��� */
#define MAA_RLS_POOL_ADDR_H (0x804)   /* �ͷ�Ŀ���ڴ�ػ���ַ��λ�Ĵ��� */
#define MAA_RLS_POOL_DEPTH (0x808)    /* �ͷ�Ŀ���ڴ����ȼĴ��� */
#define MAA_RLS_POOL_WPTR (0x80C)     /* �ͷ�Ŀ���ڴ��дָ��Ĵ��� */
#define MAA_RLS_POOL_RPTR (0x810)     /* �ͷ�Ŀ���ڴ�ض�ָ��Ĵ��� */
#define MAA_RLS_POOL_UP_THRH (0x814)  /* �ͷ�Ŀ���ڴ����ˮ�߼Ĵ��� */
#define MAA_RLS1_POOL_ADDR_L (0x820)  /* �ͷ�Ŀ���ڴ�ػ���ַ��λ�Ĵ��� */
#define MAA_RLS1_POOL_ADDR_H (0x824)  /* �ͷ�Ŀ���ڴ�ػ���ַ��λ�Ĵ��� */
#define MAA_RLS1POOL_DEPTH (0x828)    /* �ͷ�Ŀ���ڴ����ȼĴ��� */
#define MAA_RLS1_POOL_WPTR (0x82C)    /* �ͷ�Ŀ���ڴ��дָ��Ĵ��� */
#define MAA_RLS1_POOL_RPTR (0x830)    /* �ͷ�Ŀ���ڴ�ض�ָ��Ĵ��� */
#define MAA_RLS1_POOL_UP_THRH (0x834) /* �ͷ�Ŀ���ڴ����ˮ�߼Ĵ��� */

#define MAA_ACORE_ALLOC_CNT(levels) (0x900 + 0x80 * levels)
#define MAA_ALLOC_FIFO_CNT(levels) (0x904 + 0x80 * levels)
#define MAA_ALLOC_MAA_CNT(levels) (0x90C + 0x80 * levels)
#define MAA_ALLOC_DESTN_CNT(destns, levels) (0x910 + 0x4 * destns + 0x80 * levels)
#define MAA_DESTN_ALLOC_CNT(destns, levels) (0x930 + 0x4 * destns + 0x80 * levels)
#define MAA_RLS_CNT(levels) (0x960 + 0x80 * levels)

#define MAA_RLS0_ABANDON_CNT (0xEF8)       /* RELEASE POOL��дʱ������ָ����� */
#define MAA_RLS1_ABANDON_CNT (0xEFC)       /* RELEASE POOL��дʱ������ָ����� */
#define MAA_CNT_CLK_EN (0xF00)             /* ά�������ʱ���ſ�ʹ�ܼĴ��� */
#define MAA_OPIPE_STAT (0xF04)             /* OPIPE״̬�Ĵ��� */
#define MAA_IPIPE_STAT0 (0xF08)            /* IPIPE״̬�Ĵ��� */
#define MAA_IPIPE_STAT1 (0xF0C)            /* IPIPE״̬�Ĵ��� */
#define MAA_IPIPE_STAT2 (0xF10)            /* IPIPE״̬�Ĵ��� */
#define MAA_AP_MDM_OPIPE_NUM (0xF20)       /* �ڴ��������üĴ����� */
#define MAA_HAC_BP_DBG (0x1000)            /* MAA BP debug�Ĵ��� */
#define MAA_HAC_DATA_DBG (0x1004)          /* MAA AXI_MST DATA debug�Ĵ��� */
#define MAA_HAC_PUSH_DBG (0x1008)          /* MAA AXI_MST PUSH debug�Ĵ��� */
#define MAA_HAC_DBG (0x100C)               /* MAA AXI_MST debug�Ĵ��� */
#define MAA_GS_DBG (0x1010)                /* MAA AXI_GS debug�Ĵ��� */
#define MAA_HAC_BP_INVALID_DATA_L (0x1018) /* HAC_BP_INVALID_DATA_L�Ĵ��� */
#define MAA_HAC_BP_INVALID_DATA_H (0x101C) /* HAC_BP_INVALID_DATA_H�Ĵ��� */

#define MAA_ALLOC_ADDR_RD_L(opipes, src_ptrs) (0x1100 + 0x100 * opipes + 0x8 * src_ptrs)
#define MAA_ALLOC_ADDR_RD_H(opipes, src_ptrs) (0x1104 + 0x100 * opipes + 0x8 * src_ptrs)

#define MAA_OPIPE_SECCTRL (0x3000)  /* opipe��ȫ���üĴ��� */
#define MAA_IPIPE_SECCTRL0 (0x3008) /* ipipe��ȫ���üĴ��� */
#define MAA_IPIPE_SECCTRL1 (0x300C) /* ipipe��ȫ���üĴ��� */
#define MAA_IPIPE_SECCTRL2 (0x3010) /* ipipe��ȫ���üĴ��� */
#define MAA_IPIPE_SECCTRL3 (0x3014) /* ipipe��ȫ���üĴ��� */
#define MAA_IPIPE_SECCTRL4 (0x3018) /* ipipe��ȫ���üĴ��� */

#define MAA_ALLOC_ADDR_STR_L(sec_rgns) (0x3020 + 0x10 * sec_rgns)
#define MAA_ALLOC_ADDR_STR_H(sec_rgns) (0x3024 + 0x10 * sec_rgns)
#define MAA_ALLOC_ADDR_END_L(sec_rgns) (0x3028 + 0x10 * sec_rgns)
#define MAA_ALLOC_ADDR_END_H(sec_rgns) (0x302C + 0x10 * sec_rgns)

#define MAA_ALLOC_ADDR_PT_CTRL (0x3100) /* ��ַָ�밲ȫ��Χ���ƼĴ����� */
#define MAA_PTR_SIZE0 (0x3104)          /* �ڴ��������üĴ����� */
#define MAA_PTR_SIZE1 (0x3108)          /* �ڴ��������üĴ����� */
#define MAA_PTR_SIZE2 (0x310C)          /* �ڴ��������üĴ����� */
#define MAA_PTR_SIZE3 (0x3110)          /* �ڴ��������üĴ����� */
#define MAA_PTR_SIZE4 (0x3114)          /* �ڴ��������üĴ����� */
#define MAA_PTR_SIZE5 (0x3118)          /* �ڴ��������üĴ����� */
#define MAA_PTR_SIZE6 (0x311C)          /* �ڴ��������üĴ����� */
#define MAA_PTR_SIZE7 (0x3120)          /* �ڴ��������üĴ����� */
#define MAA_PTR_SIZE8 (0x3124)          /* �ڴ��������üĴ����� */
#define MAA_PTR_SIZE9 (0x3128)          /* �ڴ��������üĴ����� */
#define MAA_AP_MID (0x3200)             /* OPIPE��MID�Ĵ��� */
#define MAA_MDM_MID (0x3204)            /* OPIPE��MID�Ĵ��� */
#define MAA_IPIPE_MID0 (0x3208)         /* IPIPE��MID�Ĵ��� */
#define MAA_IPIPE_MID1 (0x320C)         /* IPIPE��MID�Ĵ��� */

#define MAA_OPIPE_BASE_ADDR_L(opipes) (0x4000 + 0x40 * opipes)
#define MAA_OPIPE_BASE_ADDR_H(opipes) (0x4004 + 0x40 * opipes)
#define MAA_OPIPE_DEPTH(opipes) (0x4008 + 0x40 * opipes)
#define MAA_OPIPE_UP_THRH(opipes) (0x400c + 0x40 * opipes)
#define MAA_OPIPE_DN_THRH(opipes) (0x4010 + 0x40 * opipes)
#define MAA_OPIPE_RPTR(opipes) (0x4014 + 0x40 * opipes)
#define MAA_OPIPE_WPTR(opipes) (0x4018 + 0x40 * opipes)
#define MAA_OPIPE_SPACE(opipes) (0x401c + 0x40 * opipes)
#define MAA_OPIPE_EN(opipes) (0x4020 + 0x40 * opipes)
#define MAA_OPIPE_CFG_DONE(opipes) (0x4024 + 0x40 * opipes)
#define MAA_OPIPE_DBG1(opipes) (0x4028 + 0x40 * opipes)
#define MAA_OPIPE_DBG0(opipes) (0x402c + 0x40 * opipes)
#define MAA_IPIPE_BASE_ADDR_L(ipipes) (0x6000 + 0x40 * ipipes)
#define MAA_IPIPE_BASE_ADDR_H(ipipes) (0x6004 + 0x40 * ipipes)
#define MAA_IPIPE_DEPTH(ipipes) (0x6008 + 0x40 * ipipes)
#define MAA_IPIPE_WPTR(ipipes) (0x600C + 0x40 * ipipes)
#define MAA_IPIPE_RPTR(ipipes) (0x6010 + 0x40 * ipipes)
#define MAA_IPIPE_PTR_ADDR_L(ipipes) (0x6018 + 0x40 * ipipes)
#define MAA_IPIPE_PTR_ADDR_H(ipipes) (0x601C + 0x40 * ipipes)
#define MAA_IPIPE_UP_THRH(ipipes) (0x6020 + 0x40 * ipipes)
#define MAA_IPIPE_DN_THRH(ipipes) (0x6024 + 0x40 * ipipes)

#define MAA_IPIPE_CTRL(ipipes) (0x6028 + 0x40 * ipipes)
#define MAA_IPIPE_DBG1(ipipes) (0x602C + 0x40 * ipipes)