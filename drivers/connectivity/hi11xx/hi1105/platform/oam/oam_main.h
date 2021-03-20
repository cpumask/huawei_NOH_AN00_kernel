

#ifndef __OAM_MAIN_H__
#define __OAM_MAIN_H__

#include "oal_types.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"

#define OAM_BEACON_HDR_LEN 24
#define OAM_TIMER_MAX_LEN  36
#define OAM_PRINT_CRLF_NUM 20 /* ������з��ĸ��� */

#define OAM_FEATURE_NAME_ABBR_LEN 12 /* ����������д��󳤶� */

/* ��ӡ��ʽ���� */
typedef oal_uint32 (*oal_print_func)(const char *pc_string);

/* ������ݸ�ʽ���� */
typedef oal_int8 *oam_va_list;

/* STRUCT���� */
/* ��־�����б�ṹ�嶨�� */
typedef struct {
    oam_feature_enum_uint8 en_feature_id;
    oal_uint8 auc_feature_name_abbr[OAM_FEATURE_NAME_ABBR_LEN]; /* ��0���� */
} oam_software_feature_stru;

extern oam_software_feature_stru g_oam_feature_list[];

extern oal_int32 oam_main_init(oal_void);
extern oal_void oam_main_exit(oal_void);
extern oal_uint32 oam_set_file_path(oal_int8 *pc_file_path, oal_uint32 ul_length);
extern oal_uint32 oam_set_output_type(oam_output_type_enum_uint8 en_output_type);
extern oal_uint32 oam_print_to_file(const char *pc_string);
extern oal_uint32 oam_print_to_sdt(const char *pc_string);
extern oal_uint32 oam_print_to_console(const char *pc_string);
extern oal_uint32 oam_upload_log_to_sdt(oal_int8 *pc_string);
extern oal_void oam_dump_buff_by_hex(oal_uint8 *puc_buff, oal_int32 l_len, oal_int32 l_num);

#endif /* end of oam_main */
