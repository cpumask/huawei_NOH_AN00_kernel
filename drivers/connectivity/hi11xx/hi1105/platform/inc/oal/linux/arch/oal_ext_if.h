

#ifndef __OAL_LINUX_EXT_IF_H__
#define __OAL_LINUX_EXT_IF_H__

#include "platform_oneimage_define.h"

/* �궨�� */
/* �ô˺����εĺ������ں�����ںͳ��ڴ�������������������__cyg_profile_func_enter��__cyg_profile_func_exit�Ĵ��� */
#if defined(_PRE_PC_LINT)
#define ATTR_OAL_NO_FUNC_TRACE
#elif (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#define ATTR_OAL_NO_FUNC_TRACE __attribute__((__no_instrument_function__))
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
#define oal_reinit_completion(x) reinit_completion(&x)
#define IRQF_DISABLED      0
#endif

/* ȫ�ֱ������� */
extern oal_mempool_info_to_sdt_stru g_mempool_info;

/* OTHERS���� */
typedef uint32_t (*oal_stats_info_up_to_sdt)(uint8_t en_pool_id,
                                             uint16_t us_mem_total_cnt,
                                             uint16_t us_mem_used_cnt,
                                             uint8_t uc_subpool_id,
                                             uint16_t us_total_cnt,
                                             uint16_t us_free_cnt);
typedef uint32_t (*oal_memblock_info_up_to_sdt)(uint8_t *puc_origin_data,
                                                uint8_t uc_user_cnt,
                                                oal_mem_pool_id_enum_uint8 en_pool_id,
                                                uint8_t uc_subpool_id,
                                                uint16_t us_len,
                                                uint32_t ul_file_id,
                                                uint32_t ul_alloc_line_num);

/* �������� */
extern uint32_t oal_mempool_info_to_sdt_register(oal_stats_info_up_to_sdt p_up_mempool_info,
                                                 oal_memblock_info_up_to_sdt p_up_memblock_info);

#endif /* end of oal_ext_if.h */
