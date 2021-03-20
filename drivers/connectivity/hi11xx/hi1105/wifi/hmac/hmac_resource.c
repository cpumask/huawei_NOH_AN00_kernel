
/* 1 ͷ�ļ����� */
#include "hmac_resource.h"
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RESOURCE_C

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* �洢hmac res��Դ�ṹ�� */
typedef struct {
    hmac_res_device_stru st_hmac_dev_res;
} hmac_res_stru;

OAL_STATIC hmac_res_stru g_st_hmac_res;

/*
 * ��������  : ��ȡһ��HMAC DEV��Դ
 */
uint32_t hmac_res_alloc_mac_dev(uint32_t ul_dev_idx)
{
    if (oal_unlikely(ul_dev_idx >= MAC_RES_MAX_DEV_NUM)) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_res_alloc_hmac_dev::invalid ul_dev_idx[%d].}", ul_dev_idx);

        return OAL_FAIL;
    }

    (g_st_hmac_res.st_hmac_dev_res.auc_user_cnt[ul_dev_idx])++;

    return OAL_SUCC;
}


uint32_t hmac_res_free_mac_dev(uint32_t ul_dev_idx)
{
    if (oal_unlikely(ul_dev_idx >= MAC_RES_MAX_DEV_NUM)) {
        oam_error_log1(0, OAM_SF_ANY, "{mac_res_free_dev::invalid ul_dev_idx[%d].}", ul_dev_idx);

        return OAL_FAIL;
    }

    (g_st_hmac_res.st_hmac_dev_res.auc_user_cnt[ul_dev_idx])--;

    if (g_st_hmac_res.st_hmac_dev_res.auc_user_cnt[ul_dev_idx] != 0) {
        return OAL_SUCC;
    }

    /* �������ֵ��Ҫ��1���� */
    oal_queue_enqueue (&(g_st_hmac_res.st_hmac_dev_res.st_queue),
                       (void *)(uintptr_t)((unsigned long) ul_dev_idx + 1));

    return OAL_SUCC;
}


hmac_device_stru *hmac_res_get_mac_dev(uint32_t ul_dev_idx)
{
    if (oal_unlikely(ul_dev_idx >= MAC_RES_MAX_DEV_NUM)) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_res_get_hmac_dev::invalid ul_dev_idx[%d].}", ul_dev_idx);

        return OAL_PTR_NULL;
    }

    return &(g_st_hmac_res.st_hmac_dev_res.ast_hmac_dev_info[ul_dev_idx]);
}


mac_chip_stru *hmac_res_get_mac_chip(uint32_t ul_chip_idx)
{
    return mac_res_get_mac_chip(ul_chip_idx);
}


uint32_t hmac_res_init(void)
{
    uint32_t ul_loop;

    memset_s(&g_st_hmac_res, OAL_SIZEOF(g_st_hmac_res), 0, OAL_SIZEOF(g_st_hmac_res));

    /***************************************************************************
            ��ʼ��HMAC DEV����Դ��������
    ***************************************************************************/
    oal_queue_set(&(g_st_hmac_res.st_hmac_dev_res.st_queue),
                  g_st_hmac_res.st_hmac_dev_res.aul_idx,
                  MAC_RES_MAX_DEV_NUM);

    for (ul_loop = 0; ul_loop < MAC_RES_MAX_DEV_NUM; ul_loop++) {
        /* ��ʼֵ������Ƕ�Ӧ�����±�ֵ��1 */
        oal_queue_enqueue(&(g_st_hmac_res.st_hmac_dev_res.st_queue),
                          (void *)(uintptr_t)((unsigned long)ul_loop + 1));

        /* ��ʼ����Ӧ�����ü���ֵΪ0 */
        g_st_hmac_res.st_hmac_dev_res.auc_user_cnt[ul_loop] = 0;
    }

    return OAL_SUCC;
}

/*
 * ��������  : ��Դ���˳����ͷŶ�̬������ڴ�
 */
uint32_t hmac_res_exit(mac_board_stru *pst_hmac_board)
{
    uint8_t uc_chip;
    uint8_t uc_device_max;
    uint8_t uc_device;
    uint16_t ul_chip_max_num;
    mac_chip_stru *pst_chip = OAL_PTR_NULL;

    /* chip֧�ֵ��������PCIe���ߴ����ṩ; */
    ul_chip_max_num = oal_bus_get_chip_num();

    for (uc_chip = 0; uc_chip < ul_chip_max_num; uc_chip++) {
        pst_chip = &pst_hmac_board->ast_chip[uc_chip];

        /* OAL�ӿڻ�ȡ֧��device���� */
        uc_device_max = oal_chip_get_device_num(pst_chip->ul_chip_ver);

        for (uc_device = 0; uc_device < uc_device_max; uc_device++) {
            /* �ͷ�hmac res��Դ */
            hmac_res_free_mac_dev(pst_chip->auc_device_id[uc_device]);
        }
    }
    return OAL_SUCC;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

