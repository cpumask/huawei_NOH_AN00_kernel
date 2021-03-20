

#ifndef __PLATFORM_COMMON_CLK_H__
#define __PLATFORM_COMMON_CLK_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "oal_types.h"
#include "plat_type.h"
#ifdef HISI_NVRAM_SUPPORT
#include <linux/mtd/hisi_nve_interface.h>
#endif

#define hi_static_assert(cond_, name_) typedef char assert_failed_##name_[(cond_) ? 1 : -1]

#define DCXO_DEVICE_MAX_BUF_LEN 64
#define DCXO_CALI_DATA_BUF_LEN  (sizeof(dcxo_dl_para_stru))

/* ������DCXO ��ȡ��NV�еĲ��� */
#define MALLOC_LEN            104
#define NV_GNSS_ID            236
#define NV_GNSS_SIZE          32
#define NV_GNSS_NAME          "XOCOE"
#define NV_FAC_ID             235
#define NV_FAC_SIZE           88
#define NV_FAC_NAME           "XOA1A0"
#define PLAT_DEF_ID           (-1)
#define PLAT_DEF_NAME         "PLAT_DEF_COEFF"
#define NVRAM_READ            1
#define NV_CHK_PAIR           2
#define RSV_NUM               2
#define DCXO_NV_CHK_OK        1
#define T0_BIT_WIDTH_10       0
#define T0_WIDTH10_TO_WIDTH16 (1 << 6)
#define DCXO_PARA_READ_OK     0x01
#define DCXO_PARA_T0_30       0x1E0000
#define FAC_NV_CRC_MASK       0xFFFF

#define PLAT_MAX_REPORT_CNT       10
#define PLAT_CHR_ERRNO_DCXO_NV_RD 910800034

/** �������� **/
#define INI_BUF_SIZE   16
#define INI_TCXO_STR   "tcxo"
#define INI_DCXO_STR   "dcxo"
#define TCXO_DCXO_MODE "tcxo_dcxo_mode"

// Ĭ���·���device�Ĳ���
#define DEFAULT_T0       0x1D0000    // 29*2^16
#define DEFAULT_A0       0
#define DEFAULT_A1       0xFFFc0000  // -0.25*2^20
#define DEFAULT_A2       0
#define DEFAULT_A3       0x68db8b  // 0.0001*2^36
#define DEFAULT_MODE     0
#define DCXO_CHECK_MAGIC 0xA0A0A0A0

// Ĭ��a0~t0��min max����
#define DEF_A0_MIN (-3145728)  // -12*2^18
#define DEF_A0_MAX 3145728     // 12*2^18
#define DEF_A1_MIN (-419430)   // -0.4*2^20
#define DEF_A1_MAX (-104857)   // -0.1*2^20
#define DEF_A2_MIN (-30923764) // -0.00045*2^36
#define DEF_A2_MAX 30923764    // 0.00045*2^36
#define DEF_A3_MIN 5841155     // 0.000085*2^36
#define DEF_A3_MAX 7902739     // 0.000115*2^36
#define DEF_T0_MIN 1638400     // 25*2^16
#define DEF_T0_MAX 2293760     // 35*2^16

// a0~t0 min max �쳣��Χ������
#define A0_EXCEP_PROT_MIN (-13107200)   // -50*2^18
#define A0_EXCEP_PROT_MAX 13107200      // 50*2^18
#define A1_EXCEP_PROT_MIN (-1048576)    // -1*2^20
#define A1_EXCEP_PROT_MAX 0             // 0*2^20
#define A2_EXCEP_PROT_MIN (-1374389535) // -0.02*2^36
#define A2_EXCEP_PROT_MAX 1374389535    // 0.02*2^36
#define A3_EXCEP_PROT_MIN 0             // 0*2^36
#define A3_EXCEP_PROT_MAX 1374389535    // 0.02*2^36
#define T0_EXCEP_PROT_MIN 1310720       // 20*2^16
#define T0_EXCEP_PROT_MAX 2621440       // 40*2^16

/* PMU �������� */
#define INI_PMU_CHIP_TYPE "pmu_version"
#define INI_6555V300_STR  "Hi6555V300"
#define INI_6421V700_STR  "Hi6421V700"

// PMU 6555V300
#define PMU_6555V300_CALI_END  0x3A0
#define PMU_6555V300_CALI_AVE0 0x3A1
#define PMU_6555V300_CALI_AVE1 0x3A2
#define PMU_6555V300_CALI_AVE2 0x3A3
#define PMU_6555V300_CALI_AVE3 0x3A4
#define PMU_6555V300_ANA_EN    0x3AA
#define PMU_6555V300_STATE     0x3AC
#define PMU_6555V300_LOW_BYTE  0x3AD
#define PMU_6555V300_HIGH_BYTE 0x3AE

// PMU 6421V700
#define PMU_6421V700_CALI_END  0x3A0
#define PMU_6421V700_CALI_AVE0 0x3A1
#define PMU_6421V700_CALI_AVE1 0x3A2
#define PMU_6421V700_CALI_AVE2 0x3A3
#define PMU_6421V700_CALI_AVE3 0x3A4
#define PMU_6421V700_ANA_EN    0x3AA
#define PMU_6421V700_STATE     0x3AC
#define PMU_6421V700_LOW_BYTE  0x3AD
#define PMU_6421V700_HIGH_BYTE 0x3AE

enum coeff_index_enum {
    COEFF_A0 = 0,
    COEFF_A1 = 1,
    COEFF_A2 = 2,
    COEFF_A3 = 3,
    COEFF_T0 = 4,
    COEFF_NUM,
};

enum pmu_type_enum {
    PMU_HI655V300 = 0,
    PMU_HI6421V700 = 1,
    PMU_TYPE_BUFF,
};

enum coeff_type_enum {
    INIT_TYPE_MIN = 0,
    INIT_TYPE_MAX = 1,
    COEFF_TYPE_BUFF,
};

enum dcxo_tcxo_enum {
    INI_TCXO_MODE = 0,
    INI_DCXO_MODE = 1,
    INIT_MODE_BUFF,
};

typedef struct {
    oal_int32 valid;                            // �жϵ���nv�Ƿ���Ч�� 1:��Ч 0:��Ч
    oal_int32 coeff_para[COEFF_NUM];            // ���a0, a1, a2, a3, t0
    oal_uint32 coeff_time;                      // gnss��ز�����ƽ̨δʹ��
    oal_int32 reserve0;                         // ����
    oal_int32 coeff_th[COEFF_NUM][NV_CHK_PAIR]; // a0~t0��ֵ��Χ�ŵ�ini�ļ������ã��˴��Ѿ�����ʹ��
    oal_int32 t0_bit_width_fix;                 // 0: t0�洢����10λ����Ҫ����2^6����, 1:t0�洢����16λ������Ҫ����
    oal_int32 tempr_expand;                     // gnss��ز�����ƽ̨δʹ��
    oal_int32 reserve1;                         // ����
    oal_uint32 crc_relt;                        // crcУ����
} dcxo_nv_info_stru;

typedef struct {
    oal_uint16 cali_end;
    oal_uint16 auto_cali_ave0;
    oal_uint16 auto_cali_ave1;
    oal_uint16 auto_cali_ave2;
    oal_uint16 auto_cali_ave3;
    oal_uint16 wifi_ana_en;
    oal_uint16 xoadc_state;
    oal_uint16 reg_addr_low;
    oal_uint16 reg_addr_high;
    oal_uint16 pmu_type;
} dcxo_pmu_addr_stru;

typedef struct {
    oal_int32 coeff_para[COEFF_NUM]; // ���a0, a1, a2, a3, t0
    oal_int32 reserve2[RSV_NUM];     // δʹ��
    oal_uint32 tcxo_dcxo_flag;
    dcxo_pmu_addr_stru pmu_addr;
    oal_uint32 check_data;
} dcxo_dl_para_stru;

// ��nv�ж�ȡ�Ĳ����쳣
typedef struct {
    oal_int32 l_struct_len;       // �ṹ��Ĵ�С
    oal_int32 l_para[COEFF_NUM];  // �洢����Ϊ a0, a1, a2, a3, t0
} plat_chr_nv_rd_excep_stru;

// ����devcie�����Ĵ洢�ռ����ƣ�������Ҫ���ж�
hi_static_assert(((sizeof(dcxo_dl_para_stru)) < DCXO_DEVICE_MAX_BUF_LEN), device_mem_must_big_than_host);

typedef struct {
    oal_uint32 use_part_id;
    oal_uint32 chr_nv_read_err;
    int64 nv_init_flag;
    oal_uint32 dcxo_tcxo_mode;
    oal_uint32 pmu_chip_type;
    oal_int32 check_value[COEFF_NUM][NV_CHK_PAIR];  // ���ֵ/��Сֵ�ж�
} dcxo_manage_stru;

extern dcxo_manage_stru g_dcxo_info;
extern oal_int32 dcxo_data_buf_malloc(oal_void);
extern void dcxo_data_buf_free(void);
extern oal_void *get_dcxo_data_buf_addr(oal_void);
extern void read_tcxo_dcxo_ini_file(void);
extern oal_void update_dcxo_coeff(oal_int32 *coeff, oal_uint32 coeff_cnt);
extern oal_int32 read_dcxo_cali_data(oal_void);

#endif /* end of platform_common_clk.h */
