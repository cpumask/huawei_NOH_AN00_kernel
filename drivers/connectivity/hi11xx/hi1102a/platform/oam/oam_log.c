

/* ͷ�ļ����� */
#include "oam_log.h"
#include "securec.h"
#include "chr_user.h"
#ifdef CONFIG_PRINTK
#include <linux/kernel.h>
#endif
#if defined(CONFIG_LOG_EXCEPTION) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
#include <log/log_usertype.h>
#endif

#include "oam_main.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_OAM_LOG_C

/* ȫ�ֱ������� */
#ifdef _PRE_DEBUG_MODE
oam_tx_complete_stat_stru g_ast_tx_complete_stat[WLAN_DEVICE_SUPPORT_MAX_NUM_SPEC];
#endif

#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
#ifdef CONFIG_PRINTK
static char *g_loglevel_string[OAM_LOG_LEVEL_BUTT];
#endif
#endif

#ifdef _PRE_WLAN_REPORT_PRODUCT_LOG
OAL_STATIC oal_bool_enum_uint8 g_en_report_product_log_flag = OAL_TRUE;              // ont log��Ĭ�Ͽ��أ���ʱĬ�ϴ�
OAL_STATIC oal_uint8 g_vapid_to_chipid[WLAN_VAP_MAX_NUM_PER_DEVICE_LIMIT * 2] = {0}; // mac vap id��chip id��ӳ���ϵ
OAL_STATIC oal_uint8 g_featureid_to_eventid[OAM_SOFTWARE_FEATURE_BUTT];              // �����feature id �� ont log event��ӳ���ϵ
OAL_STATIC oam_pdt_log_stru g_oam_product_log;
#endif

/*
 * �� �� ��  : OAL_PRINT2KERNEL
 * ��������  : �Ѵ�ӡ��Ϣ����һ�ݵ��ں���
 */
oal_int32 OAL_PRINT2KERNEL(oal_uint32 ul_para,
                           oal_uint16 us_line_no,
                           oal_int8 *fmt,
                           oal_uint p1, oal_uint p2, oal_uint p3, oal_uint p4)
{
#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
#ifdef CONFIG_PRINTK
    oal_int32 l_ret;
    oal_int8 pc_buf[OAM_LOG_PRINT_DATA_LENGTH];

    /* ��ѹ�����������ɵ������� */
    oal_uint8 uc_vap_id = (ul_para >> 24) & 0xf;
    oal_uint16 us_file_no = ul_para & 0xffff;
    oam_log_level_enum_uint8 clog_level = (ul_para >> 28) & 0xf;

    pc_buf[0] = '\0';
    if (clog_level == OAM_LOG_LEVEL_ERROR) {
        DECLARE_DFT_TRACE_KEY_INFO("oam error log", OAL_DFT_TRACE_OTHER);
    }

    l_ret = snprintf_s(pc_buf, sizeof(pc_buf), sizeof(pc_buf) - 1, KERN_INFO "%s[vap:%d]%s [F:%d][L:%d]\n",
                       g_loglevel_string[clog_level],
                       uc_vap_id,
                       fmt,
                       us_file_no,
                       us_line_no);
    if (l_ret < 0) {
        OAM_IO_PRINTK("l_ret < 0 \r\n");
        return l_ret;
    }

    printk(pc_buf, (oal_int32)p1, (oal_int32)p2, (oal_int32)p3, (oal_int32)p4);
#endif
#endif

    return OAL_SUCC;
}

oal_uint32 oam_log_set_global_switch(oal_switch_enum_uint8 en_log_switch)
{
    if (OAL_UNLIKELY(en_log_switch >= OAL_SWITCH_BUTT)) {
        OAM_IO_PRINTK("invalid en_log_switch[%d]. \r\n", en_log_switch);
        return OAL_FAIL;
    }

    oam_mng_ctx.st_log_ctx.en_global_log_switch = en_log_switch;

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_set_ratelimit_switch
 * ��������  : ������־�����ؿ���
 */
OAL_STATIC oal_uint32 oam_log_set_ratelimit_switch(oam_ratelimit_type_enum_uint8 en_ratelimit_type,
                                                   oal_switch_enum_uint8 en_log_switch)
{
    if (OAL_UNLIKELY(en_ratelimit_type >= OAM_RATELIMIT_TYPE_BUTT)) {
        OAM_IO_PRINTK("invalid en_ratelimit_type[%d]. \r\n", en_ratelimit_type);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    if (en_log_switch >= OAL_SWITCH_BUTT) {
        OAM_IO_PRINTK("invalid en_log_switch[%d]. \r\n", en_log_switch);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    oam_mng_ctx.st_log_ctx.st_ratelimit[en_ratelimit_type].en_ratelimit_switch = en_log_switch;

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_get_ratelimit_switch
 * ��������  : ��ȡ��־�����ؿ���
 */
oal_uint32 oam_log_get_ratelimit_switch(oam_ratelimit_type_enum_uint8 en_ratelimit_type,
                                        oal_switch_enum_uint8 *pen_log_switch)
{
    if (OAL_UNLIKELY(en_ratelimit_type >= OAM_RATELIMIT_TYPE_BUTT)) {
        OAM_IO_PRINTK("invalid en_ratelimit_type[%d]. \r\n", en_ratelimit_type);
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }

    if (OAL_UNLIKELY(pen_log_switch == OAL_PTR_NULL)) {
        OAM_IO_PRINTK("null param. \r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    *pen_log_switch = oam_mng_ctx.st_log_ctx.st_ratelimit[en_ratelimit_type].en_ratelimit_switch;

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_set_ratelimit_intervel
 * ��������  : ����printk����ʱ����������Ϊ��λ
 */
OAL_STATIC oal_uint32 oam_log_set_ratelimit_intervel(oam_ratelimit_type_enum_uint8 en_ratelimit_type,
                                                     oal_uint32 ul_interval)
{
    if (OAL_UNLIKELY(en_ratelimit_type >= OAM_RATELIMIT_TYPE_BUTT)) {
        OAM_IO_PRINTK("invalid en_ratelimit_type[%d]. \r\n", en_ratelimit_type);
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }

    if ((ul_interval < OAM_RATELIMIT_MIN_INTERVAL) ||
        (ul_interval > OAM_RATELIMIT_MAX_INTERVAL)) {
        OAM_IO_PRINTK("ul_interval[%d] must be range[%d~%d]. \r\n",
                      ul_interval, OAM_RATELIMIT_MIN_INTERVAL, OAM_RATELIMIT_MAX_INTERVAL);

        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    oam_mng_ctx.st_log_ctx.st_ratelimit[en_ratelimit_type].ul_interval = ul_interval * OAL_TIME_HZ;

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_set_ratelimit_burst
 * ��������  : ����printk���ص���ֵ
 */
OAL_STATIC oal_uint32 oam_log_set_ratelimit_burst(oam_ratelimit_type_enum_uint8 en_ratelimit_type,
                                                  oal_uint32 ul_burst)
{
    if (OAL_UNLIKELY(en_ratelimit_type >= OAM_RATELIMIT_TYPE_BUTT)) {
        OAM_IO_PRINTK("invalid en_ratelimit_type[%d]. \r\n", en_ratelimit_type);
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }

    if ((ul_burst < OAM_RATELIMIT_MIN_BURST) ||
        (ul_burst > OAM_RATELIMIT_MAX_BURST)) {
        OAM_IO_PRINTK("ul_burst[%d] must be range[%d~%d]. \r\n",
                      ul_burst, OAM_RATELIMIT_MIN_BURST, OAM_RATELIMIT_MAX_BURST);

        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    oam_mng_ctx.st_log_ctx.st_ratelimit[en_ratelimit_type].ul_burst = ul_burst;

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_set_ratelimit_param
 * ��������  : ������־���ز���
 */
oal_uint32 oam_log_set_ratelimit_param(oam_ratelimit_type_enum_uint8 en_ratelimit_type,
                                       oam_ratelimit_stru *pst_printk_ratelimit)
{
    oal_uint32 ul_ret;

    if (OAL_UNLIKELY(pst_printk_ratelimit == NULL)) {
        OAL_WARN_ON(1);
        return OAL_FAIL;
    }

    if (en_ratelimit_type >= OAM_RATELIMIT_TYPE_BUTT) {
        OAM_IO_PRINTK("invalid en_ratelimit_type[%d]. \r\n", en_ratelimit_type);
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }

    ul_ret = oam_log_set_ratelimit_switch(en_ratelimit_type, pst_printk_ratelimit->en_ratelimit_switch);

    ul_ret += oam_log_set_ratelimit_intervel(en_ratelimit_type, pst_printk_ratelimit->ul_interval);

    ul_ret += oam_log_set_ratelimit_burst(en_ratelimit_type, pst_printk_ratelimit->ul_burst);

    return ul_ret;
}

/*
 * �� �� ��  : oam_log_get_ratelimit_param
 * ��������  : ��ȡ��־���ز���
 */
oal_uint32 oam_log_get_ratelimit_param(oam_ratelimit_type_enum_uint8 en_ratelimit_type,
                                       oam_ratelimit_stru *pst_printk_ratelimit)
{
    oal_uint32 ul_ret = OAL_SUCC;

    if (en_ratelimit_type >= OAM_RATELIMIT_TYPE_BUTT) {
        OAM_IO_PRINTK("invalid en_ratelimit_type[%d]. \r\n", en_ratelimit_type);
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }

    if (OAL_UNLIKELY(pst_printk_ratelimit == OAL_PTR_NULL)) {
        OAM_IO_PRINTK("null param. \r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    memcpy_s(pst_printk_ratelimit, sizeof(oam_ratelimit_stru),
             &oam_mng_ctx.st_log_ctx.st_ratelimit[en_ratelimit_type],
             OAL_SIZEOF(oam_ratelimit_stru));

    return ul_ret;
}

/*
 * �� �� ��  : oam_log_ratelimit
 * ��������  : �����Ӧ�����������Ƿ������������
 *             ��ul_intervalʱ�䷶Χ��ֻ����ul_burst����¼�����
 *             ��ul_interval�ڣ��������־�Ѿ������趨ֵ�����������
 *             �ȴ�ul_interval֮�������������ul_begin���¸���ǰֵ
 */
oam_ratelimit_output_enum_uint8 oam_log_ratelimit(oam_ratelimit_type_enum_uint8 en_ratelimit_type)
{
    oal_uint ui_flags;
    oam_ratelimit_stru *pst_ratelimit;
    oam_ratelimit_output_enum_uint8 en_ret = OAM_RATELIMIT_OUTPUT;

    pst_ratelimit = &oam_mng_ctx.st_log_ctx.st_ratelimit[en_ratelimit_type];

    // �ж����ؿ���״̬
    if (pst_ratelimit->en_ratelimit_switch == OAL_SWITCH_OFF) {
        return OAM_RATELIMIT_OUTPUT;
    }

    // �����Ϊ0 ����������
    if (pst_ratelimit->ul_interval == 0) {
        return OAM_RATELIMIT_OUTPUT;
    }

    oal_spin_lock_irq_save(&pst_ratelimit->spin_lock, &ui_flags);

    // ��¼��һ����־�ĵ�ǰʱ��
    if (pst_ratelimit->ul_begin == 0) {
        pst_ratelimit->ul_begin = OAL_TIME_JIFFY;
    }

    // ��ʱʱ��+����ڵ�ǰʱ��֮ǰ���������ʱ���Ѿ���ʱ����Ҫ���¼�����
    if (oal_time_is_before(pst_ratelimit->ul_begin + pst_ratelimit->ul_interval)) {
        pst_ratelimit->ul_begin = 0;
        pst_ratelimit->ul_printed = 0;
        pst_ratelimit->ul_missed = 0;
    }

    /* ��δ��ʱ���жϵ�ǰʱ���������������־�����Ƿ�ﵽ��������� */
    /* δ�ﵽ���Ƶ������־������������� */
    if (pst_ratelimit->ul_burst && (pst_ratelimit->ul_burst > pst_ratelimit->ul_printed)) {
        pst_ratelimit->ul_printed++;
        en_ret = OAM_RATELIMIT_OUTPUT;
    } else { /* �ﵽ���Ƶ������־�����������������һ����������� */
        pst_ratelimit->ul_missed++;
        en_ret = OAM_RATELIMIT_NOT_OUTPUT;
    }

    oal_spin_unlock_irq_restore(&pst_ratelimit->spin_lock, &ui_flags);

    return en_ret;
}

/*
 * �� �� ��  : oam_log_set_vap_switch
 * ��������  : ����VAP��־����
 */
oal_uint32 oam_log_set_vap_switch(oal_uint8 uc_vap_id,
                                  oal_switch_enum_uint8 en_log_switch)
{
    if (OAL_UNLIKELY(uc_vap_id >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT)) {
        OAM_IO_PRINTK("invalid uc_vap_id[%d]. \r\n", uc_vap_id);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    if (OAL_UNLIKELY(en_log_switch >= OAL_SWITCH_BUTT)) {
        OAM_IO_PRINTK("invalid en_log_switch[%d]. \r\n", en_log_switch);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    oam_mng_ctx.st_log_ctx.st_vap_log_info.aen_vap_log_switch[uc_vap_id] = en_log_switch;

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_set_vap_level
 * ��������  : ����VAP������������־����Ϊen_log_level
 *             ���������ó�OAM_LOG_LEVEL_INFO����,����������������ƣ�������������
 */
oal_uint32 oam_log_set_vap_level(oal_uint8 uc_vap_id, oam_log_level_enum_uint8 en_log_level)
{
    oam_feature_enum_uint8 en_feature_idx;

    if (OAL_UNLIKELY(uc_vap_id >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT)) {
        OAM_IO_PRINTK("invalid vap id.[%d] \r\n", uc_vap_id);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    /* ���õ�ǰVAP����־���� */
    oam_mng_ctx.st_log_ctx.st_vap_log_info.aen_vap_log_level[uc_vap_id] = en_log_level;

    /* ͬʱ���õ�ǰVAP������������־���� */
    for (en_feature_idx = 0; en_feature_idx < OAM_SOFTWARE_FEATURE_BUTT; en_feature_idx++) {
        oam_log_set_feature_level(uc_vap_id, en_feature_idx, en_log_level);
    }

    return OAL_SUCC;
}

oal_uint32 oam_log_get_vap_level(oal_uint8 uc_vap_id, oam_log_level_enum_uint8 *pen_log_level)
{
    if (OAL_UNLIKELY(uc_vap_id >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT)) {
        OAM_IO_PRINTK("invalid vap id.[%d] \r\n", uc_vap_id);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    if (OAL_UNLIKELY(pen_log_level == OAL_PTR_NULL)) {
        OAM_IO_PRINTK("null param \r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    *pen_log_level = oam_mng_ctx.st_log_ctx.st_vap_log_info.aen_vap_log_level[uc_vap_id];

    return OAL_SUCC;
}

oal_uint32 oam_log_set_feature_level(oal_uint8 uc_vap_id,
                                     oam_feature_enum_uint8 en_feature_id,
                                     oam_log_level_enum_uint8 en_log_level)
{
    if (OAL_UNLIKELY(uc_vap_id >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT)) {
        OAM_IO_PRINTK("invalid uc_vap_id.[%d] \r\n", uc_vap_id);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    if (OAL_UNLIKELY(en_feature_id >= OAM_SOFTWARE_FEATURE_BUTT)) {
        OAM_IO_PRINTK("invalid en_feature_id.[%d] \r\n", en_feature_id);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    if (OAL_UNLIKELY((en_log_level >= OAM_LOG_LEVEL_BUTT) || (en_log_level < OAM_LOG_LEVEL_ERROR))) {
        OAM_IO_PRINTK("invalid en_log_level.[%d] \r\n", en_log_level);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    oam_mng_ctx.st_log_ctx.st_vap_log_info.aen_feature_log_level[uc_vap_id][en_feature_id] = en_log_level;
    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_get_feature_id
 * ��������  : ���������ƻ�ȡ����ID
 * �������  : puc_feature_id  ��Ӧ�������Ƶ�ID��
 */
oal_uint32 oam_get_feature_id(oal_uint8 *puc_feature_name,
                              oam_feature_enum_uint8 *puc_feature_id)
{
    oam_feature_enum_uint8 en_feature_idx;

    if (OAL_UNLIKELY((puc_feature_name == OAL_PTR_NULL) || (puc_feature_id == OAL_PTR_NULL))) {
        OAM_IO_PRINTK("null param \r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    for (en_feature_idx = 0; en_feature_idx < OAM_SOFTWARE_FEATURE_BUTT; en_feature_idx++) {
        if (oal_strcmp((oal_int8 *)gst_oam_feature_list[en_feature_idx].auc_feature_name_abbr,
                       (oal_int8 *)puc_feature_name) == 0) {
            *puc_feature_id = en_feature_idx;
            return OAL_SUCC;
        }
    }

    return OAL_FAIL;
}

/*
 * �� �� ��  : oam_get_feature_name
 * ��������  : ��ȡָ������ID����������
 */
oal_uint32 oam_get_feature_name(oam_feature_enum_uint8 en_feature_id,
                                oal_uint8 *puc_feature_name,
                                oal_uint8 uc_size)
{
    oal_uint8 uc_feature_len;
    oal_uint8 uc_temp_name_len;

    if (OAL_UNLIKELY(en_feature_id >= OAM_SOFTWARE_FEATURE_BUTT)) {
        OAM_IO_PRINTK("en_feature_id override. %d. \r\n", en_feature_id);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    if (OAL_UNLIKELY(puc_feature_name == OAL_PTR_NULL)) {
        OAM_IO_PRINTK("puc_feature_name is NULL. \r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    uc_temp_name_len = uc_size;
    uc_feature_len = (oal_uint8)OAL_STRLEN((oal_int8 *)gst_oam_feature_list[en_feature_id].auc_feature_name_abbr);
    uc_size = (uc_size > uc_feature_len) ? uc_feature_len : uc_size;

    if (memcpy_s(puc_feature_name, uc_temp_name_len,
                 gst_oam_feature_list[en_feature_id].auc_feature_name_abbr, uc_size) != EOK) {
        OAM_IO_PRINTK("memcpy_s error, destlen=%u, srclen=%u\n ", uc_temp_name_len, uc_size);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_show_feature_list
 * ��������  : ��ʾ������д��ȫ�Ƶİ�����Ϣ
 */
oal_uint32 oam_show_feature_list(oal_void)
{
    oam_feature_enum_uint8 en_feature_id;

    OAL_IO_PRINT("feature_list: \r\n");
    for (en_feature_id = 0; en_feature_id < OAM_SOFTWARE_FEATURE_BUTT; en_feature_id++) {
        OAL_IO_PRINT("%s\r\n", gst_oam_feature_list[en_feature_id].auc_feature_name_abbr);
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_format_string
 * ��������  : ���ݸ�ʽ,����־��Ϣд�뵽ָ�����ļ���
 */
OAL_STATIC oal_uint32 oam_log_format_string(oal_int8 *pac_output_data,
                                            oal_uint16 us_data_len,
                                            oal_uint8 uc_vap_id,
                                            oam_feature_enum_uint8 en_feature_id,
                                            oal_uint16 us_file_id,
                                            oal_uint16 us_line_num,
                                            oam_log_level_enum_uint8 en_log_level,
                                            oal_int8 *pc_string,
                                            oal_uint8 uc_param_cnt,
                                            oal_int32 l_param1,
                                            oal_int32 l_param2,
                                            oal_int32 l_param3,
                                            oal_int32 l_param4)
{
    oal_int8 *pac_print_level_tbl[] = { "OFF", "ERROR", "WARNING", "INFO" };
    oal_uint32 ul_tick;
    oal_uint8 auc_feature_name[OAM_FEATURE_NAME_ABBR_LEN] = {0};
    oal_int8 *pac_print_format[] = {
        "[LOG=%s]:Tick=%lu, FileId=%d, LineNo=%d, VAP=%d, FeatureName=%s, \"%s\", \r\n",
        "[LOG=%s]:Tick=%lu, FileId=%d, LineNo=%d, VAP=%d, FeatureName=%s, \"%s\", %lu \r\n",
        "[LOG=%s]:Tick=%lu, FileId=%d, LineNo=%d, VAP=%d, FeatureName=%s, \"%s\", %lu, %lu \r\n",
        "[LOG=%s]:Tick=%lu, FileId=%d, LineNo=%d, VAP=%d, FeatureName=%s, \"%s\", %lu, %lu, %lu \r\n",
        "[LOG=%s]:Tick=%lu, FileId=%d, LineNo=%d, VAP=%d, FeatureName=%s, \"%s\", %lu, %lu, %lu, %lu \r\n"
    };
    oal_int32 ret;

    /* ��ȡϵͳTICKֵ */
    ul_tick = (oal_uint32)OAL_TIME_GET_STAMP_MS();

    oam_get_feature_name(en_feature_id, auc_feature_name, OAL_SIZEOF(auc_feature_name));

    /* ���ݲ�������,��LOG��Ϣ���浽ac_file_data�� */
    switch (uc_param_cnt) {
        case 0:
            ret = snprintf_s(pac_output_data, us_data_len, us_data_len - 1, pac_print_format[uc_param_cnt],
                             pac_print_level_tbl[en_log_level], ul_tick, us_file_id,
                             us_line_num, uc_vap_id, auc_feature_name, pc_string);
            break;

        case 1:
            ret = snprintf_s(pac_output_data, us_data_len, us_data_len - 1, pac_print_format[uc_param_cnt],
                             pac_print_level_tbl[en_log_level], ul_tick, us_file_id,
                             us_line_num, uc_vap_id, auc_feature_name, pc_string, l_param1);
            break;

        case 2:
            ret = snprintf_s(pac_output_data, us_data_len, us_data_len - 1, pac_print_format[uc_param_cnt],
                             pac_print_level_tbl[en_log_level], ul_tick, us_file_id,
                             us_line_num, uc_vap_id, auc_feature_name, pc_string, l_param1, l_param2);
            break;

        case 3:
            ret = snprintf_s(pac_output_data, us_data_len, us_data_len - 1, pac_print_format[uc_param_cnt],
                             pac_print_level_tbl[en_log_level], ul_tick, us_file_id,
                             us_line_num, uc_vap_id, auc_feature_name, pc_string,
                             l_param1, l_param2, l_param3);
            break;

        case 4:
            ret = snprintf_s(pac_output_data, us_data_len, us_data_len - 1, pac_print_format[uc_param_cnt],
                             pac_print_level_tbl[en_log_level], ul_tick, us_file_id,
                             us_line_num, uc_vap_id, auc_feature_name, pc_string,
                             l_param1, l_param2, l_param3, l_param4);
            break;

        default:
            OAM_IO_PRINTK("invalid uc_param_cnt.[%d] \r\n", uc_param_cnt);
            return OAL_FAIL;
    }

    if (ret < 0) {
        OAM_IO_PRINTK("log format str err\n");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC oal_void oam_set_log_info_stru(oam_log_info_stru *pst_log_info,
                                          oal_uint8 uc_vap_id,
                                          oam_feature_enum_uint8 en_feature_id,
                                          oal_uint16 us_file_id,
                                          oal_uint16 us_line_num,
                                          oam_log_level_enum_uint8 en_log_level,
                                          oal_int32 l_param1,
                                          oal_int32 l_param2,
                                          oal_int32 l_param3,
                                          oal_int32 l_param4)
{
    oal_uint32 ul_tick;

    /* ��ȡϵͳTICKֵ */
    ul_tick = (oal_uint32)OAL_TIME_GET_STAMP_MS();

    /* Ϊ��־�ṹ��������Ա��ֵ */
    pst_log_info->st_vap_log_level.bit_vap_id = uc_vap_id;
    pst_log_info->st_vap_log_level.bit_log_level = en_log_level;
    pst_log_info->us_file_id = us_file_id;
    pst_log_info->us_line_num = us_line_num;
    pst_log_info->en_feature_id = en_feature_id;
    pst_log_info->ul_tick = ul_tick;
    pst_log_info->al_param[0] = l_param1;
    pst_log_info->al_param[1] = l_param2;
    pst_log_info->al_param[2] = l_param3;
    pst_log_info->al_param[3] = l_param4;
}

/* UT��Ҫ���ֽӿڽ��в��� */
#if ((_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION) || (_PRE_OS_VERSION_WIN32_RAW == _PRE_OS_VERSION))
/*
 * �� �� ��  : oam_log_check_param
 * ��������  : ���VAP��š�ģ��ID�ʹ�ӡ�������Ч��
 */
OAL_STATIC oal_uint32 oam_log_check_param(oal_uint8 uc_vap_id,
                                          oam_feature_enum_uint8 en_feature_id,
                                          oam_log_level_enum_uint8 en_log_level)
{
    /* �ж�VAP�Ƿ���� */
    if (OAL_UNLIKELY(uc_vap_id >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT)) {
        OAM_IO_PRINTK("invalid uc_vap_id[%d]. \r\n", uc_vap_id);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    /* �ж�����ID�ĺ����� */
    if (OAL_UNLIKELY(en_feature_id >= OAM_SOFTWARE_FEATURE_BUTT)) {
        OAM_IO_PRINTK("invalid en_feature_id[%d]. \r\n", en_feature_id);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    /* �жϴ�ӡ����ĺ����� */
    if (OAL_UNLIKELY(en_log_level >= OAM_LOG_LEVEL_BUTT)) {
        OAM_IO_PRINTK("invalid en_log_level[%d]. \r\n", en_log_level);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_get_feature_level
 * ��������  : ��ȡVAP������־����
 */
OAL_STATIC oal_uint32 oam_log_get_feature_level(oal_uint8 uc_vap_id,
                                                oam_feature_enum_uint8 en_feature_id,
                                                oam_log_level_enum_uint8 *pen_log_level)
{
    if (OAL_UNLIKELY(uc_vap_id >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT)) {
        OAM_IO_PRINTK("invalid uc_vap_id.[%d] \r\n", uc_vap_id);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    if (OAL_UNLIKELY(en_feature_id >= OAM_SOFTWARE_FEATURE_BUTT)) {
        OAM_IO_PRINTK("invalid en_feature_id.[%d] \r\n", en_feature_id);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    if (OAL_UNLIKELY(pen_log_level == OAL_PTR_NULL)) {
        OAM_IO_PRINTK("null param \r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    *pen_log_level = oam_mng_ctx.st_log_ctx.st_vap_log_info.aen_feature_log_level[uc_vap_id][en_feature_id];

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_get_global_switch
 * ��������  : ��ȡȫ�ֿ���״̬
 */
OAL_STATIC oal_uint32 oam_log_get_global_switch(oal_switch_enum_uint8 *pen_log_switch)
{
    if (OAL_UNLIKELY(pen_log_switch == OAL_PTR_NULL)) {
        OAM_IO_PRINTK("null param. \r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    *pen_log_switch = oam_mng_ctx.st_log_ctx.en_global_log_switch;
    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_get_vap_switch
 * ��������  : ��ȡVAP��־����
 */
OAL_STATIC oal_uint32 oam_log_get_vap_switch(oal_uint8 uc_vap_id,
                                             oal_switch_enum_uint8 *pen_log_switch)
{
    if (OAL_UNLIKELY(uc_vap_id >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT)) {
        OAM_IO_PRINTK("invalid uc_vap_id[%d] \r\n", uc_vap_id);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    if (OAL_UNLIKELY(pen_log_switch == OAL_PTR_NULL)) {
        OAM_IO_PRINTK("null param \r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    *pen_log_switch = oam_mng_ctx.st_log_ctx.st_vap_log_info.aen_vap_log_switch[uc_vap_id];

    return OAL_SUCC;
}
#endif

/*
 * �� �� ��  : oam_log_chr_report
 * ��������  : ����־��Ϣ�ϱ�CHR
 */
OAL_STATIC void oam_log_chr_report(oal_uint16 file_id, oal_uint16 line_num,
    oal_uint8 param_cnt, oam_log_level_enum_uint8 log_level, oal_int32 para1,
    oal_int32 para2, oal_int32 para3, oal_int32 para4)
{
#define FILE_ID_OFFSET 16

    chr_oam_stru chr_oam_info;

#if defined(CONFIG_LOG_EXCEPTION) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
    if (get_logusertype_flag() != BETA_USER) {
        return;
    }
#endif

    memset_s(&chr_oam_info, sizeof(chr_oam_info), 0, sizeof(chr_oam_info));
    if ((log_level == OAM_LOG_LEVEL_ERROR) && (oal_print_rate_limit(1 * PRINT_RATE_SECOND))) {
        chr_oam_info.chr_sys = CHR_SYSTEM_OAM;
        chr_oam_info.file_line = (file_id << FILE_ID_OFFSET) | line_num;
        CHR_EXCEPTION_P(CHR_PLATFORM_EXCEPTION_EVENTID, (oal_uint8 *)(&chr_oam_info), sizeof(chr_oam_stru));
    }
}

/*
 * �� �� ��  : oam_log_print_to_console
 * ��������  : ��־����������ն�
 */
OAL_STATIC oal_uint32 oam_log_print_to_console(oal_uint8 uc_vap_id,
                                               oam_feature_enum_uint8 en_feature_id,
                                               oal_uint16 us_file_id,
                                               oal_uint16 us_line_num,
                                               oam_log_level_enum_uint8 en_log_level,
                                               oal_int8 *pc_string,
                                               oal_uint8 uc_param_cnt,
                                               oal_int32 l_param1,
                                               oal_int32 l_param2,
                                               oal_int32 l_param3,
                                               oal_int32 l_param4)
{
    oal_int8 ac_print_buff[OAM_PRINT_FORMAT_LENGTH]; /* ���ڱ���д�뵽�ļ��еĸ�ʽ */

    oam_log_format_string(ac_print_buff,
                          OAM_PRINT_FORMAT_LENGTH,
                          uc_vap_id,
                          en_feature_id,
                          us_file_id,
                          us_line_num,
                          en_log_level,
                          pc_string,
                          uc_param_cnt,
                          l_param1,
                          l_param2,
                          l_param3,
                          l_param4);

    oam_print_to_console(ac_print_buff);

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_print_to_file
 * ��������  : ����־��Ϣ��ӡ���ļ���
 * �������  : 1) VAP���
 *             2) �ļ�ID
 *             3) �к�
 *             4) ģ��ID
 *             5) ��ӡ����
 *             6) Ҫ��ӡ���ַ���
 *             7) ��ӡ�Ĳ�������
 *             8) ��Ҫ��ӡ�ĵ�һ������
 *             9) ��Ҫ��ӡ�ĵڶ�������
 *             10)��Ҫ��ӡ�ĵ���������
 *             11)��Ҫ��ӡ�ĵ��ĸ�����
 */
oal_uint32 oam_log_print_to_file(oal_uint8 uc_vap_id,
                                 oam_feature_enum_uint8 en_feature_id,
                                 oal_uint16 us_file_id,
                                 oal_uint16 us_line_num,
                                 oam_log_level_enum_uint8 en_log_level,
                                 oal_int8 *pc_string,
                                 oal_uint8 uc_param_cnt,
                                 oal_int32 l_param1,
                                 oal_int32 l_param2,
                                 oal_int32 l_param3,
                                 oal_int32 l_param4)
{
#ifdef _PRE_WIFI_DMT
    oal_int8 ac_output_data[OAM_PRINT_FORMAT_LENGTH]; /* ���ڱ���д�뵽�ļ��еĸ�ʽ */
    oal_uint32 ul_ret;

    oam_log_format_string(ac_output_data,
                          OAM_PRINT_FORMAT_LENGTH,
                          uc_vap_id,
                          en_feature_id,
                          us_file_id,
                          us_line_num,
                          en_log_level,
                          pc_string,
                          uc_param_cnt,
                          l_param1,
                          l_param2,
                          l_param3,
                          l_param4);

    ul_ret = oam_print_to_file(ac_output_data);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }
#endif
    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_print_to_sdt
 * ��������  : ��־�ı�׼�������
 * �������  : 1) VAP���
 *             2) �ļ�ID
 *             3) �к�
 *             4) ģ��ID
 *             5) ��ӡ����
 *             6) Ҫ��ӡ���ַ���
 *             7) ��ӡ�Ĳ�������
 *             8) ��Ҫ��ӡ�ĵ�һ������
 *             9) ��Ҫ��ӡ�ĵڶ�������
 *             10)��Ҫ��ӡ�ĵ���������
 *             11)��Ҫ��ӡ�ĵ��ĸ�����
 */
OAL_STATIC oal_uint32 oam_log_print_to_sdt(oal_uint8 uc_vap_id,
                                           oam_feature_enum_uint8 en_feature_id,
                                           oal_uint16 us_file_id,
                                           oal_uint16 us_line_num,
                                           oam_log_level_enum_uint8 en_log_level,
                                           oal_int8 *pc_string,
                                           oal_int32 l_param1,
                                           oal_int32 l_param2,
                                           oal_int32 l_param3,
                                           oal_int32 l_param4)
{
    oal_uint32 ul_ret;
    oam_log_info_stru st_log_info;

    memset_s(&st_log_info, OAL_SIZEOF(oam_log_info_stru), 0, OAL_SIZEOF(oam_log_info_stru));

    oam_set_log_info_stru(&st_log_info,
                          uc_vap_id,
                          en_feature_id,
                          us_file_id,
                          us_line_num,
                          en_log_level,
                          l_param1,
                          l_param2,
                          l_param3,
                          l_param4);

    /* WARNING��ERROR�������� */
    if ((en_log_level != OAM_LOG_LEVEL_INFO) &&
        (oam_log_ratelimit(OAM_RATELIMIT_TYPE_LOG) == OAM_RATELIMIT_NOT_OUTPUT)) {
        return OAL_SUCC;
    }

    ul_ret = oam_upload_log_to_sdt((oal_int8 *)&st_log_info);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_print_n_param
 * ��������  : LOGģ���ṩ��LOG��ӡ���������(N������)
 */
OAL_STATIC oal_uint32 oam_log_print_n_param(oal_uint32 ul_para, oal_uint16 us_line_num,
    oal_int8 *pc_string, oal_uint8 uc_param_cnt, oal_int32 l_param1, oal_int32 l_param2,
    oal_int32 l_param3, oal_int32 l_param4)
{
    oal_uint32 ul_ret = OAL_SUCC;
    oam_output_type_enum_uint8 en_output_type;

    oal_uint8 uc_vap_id = (ul_para >> 24) & 0xf;
    oam_feature_enum_uint8 en_feature_id = (ul_para >> 16) & 0xff;
    oal_uint16 us_file_id = ul_para & 0xffff;
    oam_log_level_enum_uint8 en_log_level = (ul_para >> 28) & 0xf;

    if (OAL_UNLIKELY(pc_string == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_log_chr_report(us_file_id, us_line_num, uc_param_cnt, en_log_level,
                       l_param1, l_param2, l_param3, l_param4);

    /* ������������㣬�ж�������� */
    oam_get_output_type(&en_output_type);
    switch (en_output_type) {
        /* ���������̨ */
        case OAM_OUTPUT_TYPE_CONSOLE:
            ul_ret = oam_log_print_to_console(uc_vap_id, en_feature_id, us_file_id, us_line_num,
                                              en_log_level, pc_string, uc_param_cnt, l_param1,
                                              l_param2, l_param3, l_param4);
            break;

        /* ������ļ�ϵͳ�� */
        case OAM_OUTPUT_TYPE_FS:
            ul_ret = oam_log_print_to_file(uc_vap_id, en_feature_id, us_file_id, us_line_num,
                                           en_log_level, pc_string, uc_param_cnt, l_param1,
                                           l_param2, l_param3, l_param4);
            break;

        /* �����PC����⹤��ƽ̨ */
        case OAM_OUTPUT_TYPE_SDT:
            ul_ret = oam_log_print_to_sdt(uc_vap_id, en_feature_id, us_file_id, us_line_num,
                                          en_log_level, pc_string, l_param1, l_param2,
                                          l_param3, l_param4);
            break;

        /* ��Ч���� */
        default:
            ul_ret = OAL_ERR_CODE_INVALID_CONFIG;
            break;
    }

    return ul_ret;
}

#ifdef _PRE_WLAN_REPORT_PRODUCT_LOG
oam_pdt_log_rpt_stru *oam_pdt_log_new(oal_void)
{
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    oal_uint ul_irq_save;
    oam_pdt_log_stru *pst_mgr = &g_oam_product_log;

    if (pst_mgr->pst_log_mem == NULL) {
        return OAL_PTR_NULL;
    }

    oal_spin_lock_irq_save(&pst_mgr->st_spin_lock, &ul_irq_save);
    if (!oal_dlist_is_empty(&pst_mgr->st_pdt_free_list)) {
        pst_entry = oal_dlist_delete_head(&pst_mgr->st_pdt_free_list);
    }
    oal_spin_unlock_irq_restore(&pst_mgr->st_spin_lock, &ul_irq_save);

    return pst_entry ? OAL_DLIST_GET_ENTRY(pst_entry, oam_pdt_log_rpt_stru, st_list_entry) : OAL_PTR_NULL;
}

oal_void oam_pdt_log_free(oam_pdt_log_rpt_stru *pst_log)
{
    oal_uint ul_irq_save;
    oam_pdt_log_stru *pst_mgr = &g_oam_product_log;

    if ((pst_mgr->pst_log_mem == NULL) || pst_log == NULL) {
        return;
    }

    oal_spin_lock_irq_save(&pst_mgr->st_spin_lock, &ul_irq_save);
    oal_dlist_add_tail(&pst_log->st_list_entry, &pst_mgr->st_pdt_free_list);
    oal_spin_unlock_irq_restore(&pst_mgr->st_spin_lock, &ul_irq_save);
}

/*
 * �� �� ��  : oam_pdt_log_add
 * ��������  : ��log�ڵ���ӵ�used����
 */
oal_uint32 oam_pdt_log_add(oam_pdt_log_rpt_stru *pst_log)
{
    oal_uint ul_irq_save;
    oam_pdt_log_stru *pst_mgr = &g_oam_product_log;

    if (pst_mgr->pst_log_mem == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_spin_lock_irq_save(&pst_mgr->st_spin_lock, &ul_irq_save);
    oal_dlist_add_tail(&pst_log->st_list_entry, &pst_mgr->st_pdt_used_list);
    oal_spin_unlock_irq_restore(&pst_mgr->st_spin_lock, &ul_irq_save);

    oal_workqueue_schedule(&pst_mgr->st_pdt_log_work);
    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_pdt_log_wq
 * ��������  : workqueue����ں��������ò�Ʒ��־�ӿ�
 */
oal_void oam_pdt_log_wq(oal_work_stru *pst_work)
{
    oal_dlist_head_stru *pst_entry = NULL;
    oam_pdt_log_rpt_stru *pst_log_rpt = NULL;
    oal_uint ul_irq_save;
    oal_dlist_head_stru st_used;
    oam_pdt_log_stru *pst_mgr = &g_oam_product_log;

    if (pst_mgr->pst_log_mem == NULL) {
        return;
    }

    oal_dlist_init_head(&st_used);

    oal_spin_lock_irq_save(&pst_mgr->st_spin_lock, &ul_irq_save);
    if (!oal_dlist_is_empty(&pst_mgr->st_pdt_used_list)) {
        oal_dlist_remove_head(&st_used, &pst_mgr->st_pdt_used_list, pst_mgr->st_pdt_used_list.pst_prev);
    }
    oal_spin_unlock_irq_restore(&pst_mgr->st_spin_lock, &ul_irq_save);

    // ������ͷ��ȡһ���ڵ�
    OAL_DLIST_SEARCH_FOR_EACH(pst_entry, &st_used)
    {
        // ��ȡ�ڵ�ָ�������
        pst_log_rpt = OAL_DLIST_GET_ENTRY(pst_entry, oam_pdt_log_rpt_stru, st_list_entry);

        OAM_REPORT_PRODUCT_LOG_FUN(pst_log_rpt->uc_chip_id, pst_log_rpt->uc_event_id, 0,
                                   pst_log_rpt->auc_log_string);
    }

    if (!oal_dlist_is_empty(&st_used)) {
        oal_spin_lock_irq_save(&pst_mgr->st_spin_lock, &ul_irq_save);
        oal_dlist_join_tail(&pst_mgr->st_pdt_free_list, &st_used);
        oal_spin_unlock_irq_restore(&pst_mgr->st_spin_lock, &ul_irq_save);
    }
}

oal_void oam_pdt_log_init(oal_void)
{
    oal_uint32 i;
    oam_pdt_log_rpt_stru *pst_log = NULL;
    oam_pdt_log_stru *pst_mgr = &g_oam_product_log;

    OAL_IO_PRINT("oam_pdt_log_init\n");
    memset_s(pst_mgr, sizeof(oam_pdt_log_stru), 0, sizeof(oam_pdt_log_stru));

    pst_log = oal_memalloc(OAL_SIZEOF(oam_pdt_log_rpt_stru) * OAM_ONT_LOG_MAX_COUNT);
    if (pst_log == NULL) {
        OAL_IO_PRINT("{oam_pdt_log_init:alloc mem failed, size=%d\n",
                     OAL_SIZEOF(oam_pdt_log_rpt_stru) * OAM_ONT_LOG_MAX_COUNT);
        return;
    }

    /* ��ʼ���������� */
    oal_dlist_init_head(&pst_mgr->st_pdt_used_list);
    oal_dlist_init_head(&pst_mgr->st_pdt_free_list);
    OAL_INIT_WORK(&pst_mgr->st_pdt_log_work, oam_pdt_log_wq);

    for (i = 0; i < OAM_ONT_LOG_MAX_COUNT; i++) {
        oal_dlist_add_tail(&pst_log[i].st_list_entry, &pst_mgr->st_pdt_free_list);
    }

    pst_mgr->pst_log_mem = pst_log;
}

oal_void oam_pdt_log_exit(oal_void)
{
    oam_pdt_log_stru *pst_mgr = &g_oam_product_log;

    OAL_IO_PRINT("oam_pdt_log_exit\n");

    if (pst_mgr->pst_log_mem == NULL) {
        return;
    }

    oal_cancel_work_sync(&g_oam_product_log.st_pdt_log_work);
    oal_free(pst_mgr->pst_log_mem);
    memset_s(pst_mgr, sizeof(oam_pdt_log_stru), 0, sizeof(oam_pdt_log_stru));
}

/*
 * �� �� ��  : __oam_report_product_log
 * ��������  : ��־��ӡ�������޲�����־���
 */
oal_void __oam_report_product_log(oal_uint32 ul_para,
                                  oal_int8 *pc_string,
                                  oal_uint l_param1,
                                  oal_uint l_param2,
                                  oal_uint l_param3,
                                  oal_uint l_param4)
{
    oal_uint8 uc_vap_id = (ul_para >> 24) & 0xf;
    oam_feature_enum_uint8 en_feature_id = (ul_para >> 16) & 0xff;
    oal_int32 l_ret_len;
    oal_int32 l_sum = 0;  // ʵ�ʳ���
    oal_uint8 uc_chip_id;
    oal_time_stru st_time;
    oam_pdt_log_rpt_stru *pst_log_rpt = oam_pdt_log_new();

    if (pst_log_rpt == NULL) {
        OAL_IO_PRINT("__oam_report_product_log:: alloc log failed\n");
        return;
    }

    // ont��Ʒchip0��Ӧ5g��chip1��Ӧ2g;
    uc_chip_id = g_vapid_to_chipid[uc_vap_id];

    // uc_chip_idΪ1ʱ����Ӧ2g; ת����0��Ӧ2g��1��Ӧ5g;
    uc_chip_id = (uc_chip_id + 1) % 2;

    memset_s(&st_time, OAL_SIZEOF(oal_time_stru), 0, OAL_SIZEOF(oal_time_stru));
    OAL_GET_REAL_TIME(&st_time);

    l_ret_len = snprintf_s(pst_log_rpt->auc_log_string, OAL_SIZEOF(pst_log_rpt->auc_log_string),
                           OAL_SIZEOF(pst_log_rpt->auc_log_string) - 1,
                           "%04d-%02d-%02d %02d:%02d:%02d, [%s], [vap%d], ",
                           st_time.tm_year + 1900, st_time.tm_mon + 1, st_time.tm_mday,
                           st_time.tm_hour, st_time.tm_min, st_time.tm_sec,
                           uc_chip_id == 1 ? "5G" : "2.4G", uc_vap_id - OAM_LOG_VAP_INDEX_INTERVAL);
    if (l_ret_len < 0) {
        oam_pdt_log_free(pst_log_rpt);
        return;
    } else if (l_ret_len < (oal_int32)OAL_SIZEOF(pst_log_rpt->auc_log_string) - 1) { // ���пռ�
        l_sum += l_ret_len;
        l_ret_len = snprintf_s(pst_log_rpt->auc_log_string + l_sum,
                               OAL_SIZEOF(pst_log_rpt->auc_log_string) - (oal_uint32)l_sum,
                               OAL_SIZEOF(pst_log_rpt->auc_log_string) - (oal_uint32)l_sum - 1,
                               pc_string, (oal_int32)l_param1, (oal_int32)l_param2,
                               (oal_int32)l_param3, (oal_int32)l_param4);
        if (l_ret_len < 0) {
            oam_pdt_log_free(pst_log_rpt);
            return;
        }

        l_sum += l_ret_len;
    } else {
        l_sum = (oal_int32)OAL_SIZEOF(pst_log_rpt->auc_log_string) - 1;
    }

    l_ret_len = (oal_int32)OAL_STRLEN(pc_string);
    if ((l_ret_len > 0 && pc_string[l_ret_len - 1] != '\n') || (l_ret_len == 0)) {
        /* ��Ҫ���������ֽڴ��'\n'��'\0' */
        if (l_sum > (oal_int32)OAL_SIZEOF(pst_log_rpt->auc_log_string) - 2) {
            l_sum = (oal_int32)OAL_SIZEOF(pst_log_rpt->auc_log_string) - 2;
        }
        pst_log_rpt->auc_log_string[l_sum] = '\n';
        pst_log_rpt->auc_log_string[l_sum + 1] = 0;
    }

    pst_log_rpt->uc_chip_id = uc_chip_id;
    pst_log_rpt->uc_event_id = g_featureid_to_eventid[en_feature_id];

    // ��ӽڵ�
    oam_pdt_log_add(pst_log_rpt);
}
oal_void oam_report_product_log(oal_uint32 ul_para,
                                oal_int8 *pc_string,
                                oal_uint l_param1,
                                oal_uint l_param2,
                                oal_uint l_param3,
                                oal_uint l_param4)
{
    oam_feature_enum_uint8 en_feature_id = (ul_para >> 16) & 0xff;
    oam_log_level_enum_uint8 en_log_level = (ul_para >> 28) & 0xf;

    if (g_en_report_product_log_flag == OAL_FALSE) {
        return;
    }
    // info�Ĵ�ӡ̫�࣬��ʱ����ӡ��ont��־�У�ֻ��ӡwarning��error��־
    if (en_log_level >= OAM_LOG_LEVEL_INFO) {
        return;
    }

    // �����HW_KER_WIFI_LOG_BUTT����ֱ�ӷ���
    if (g_featureid_to_eventid[en_feature_id] == HW_KER_WIFI_LOG_BUTT) {
        return;
    }

    __oam_report_product_log(ul_para, pc_string, l_param1, l_param2, l_param3, l_param4);
}

#endif

/*
 * �� �� ��  : oam_log_print0
 * ��������  : ��־��ӡ�������޲�����־���
 */
oal_uint32 oam_log_print0(oal_uint32 ul_para,
                          oal_uint16 us_line_num,
                          oal_int8 *pc_string)
{
    oal_uint32 ul_ret = OAL_SUCC;
    if (oam_get_log_switch(ul_para) == OAL_SWITCH_ON) {
#ifdef _PRE_WLAN_DFT_LOG
        OAL_PRINT2KERNEL(ul_para, us_line_num, pc_string, 0, 0, 0, 0);
#endif
        ul_ret = oam_log_print_n_param(ul_para, us_line_num, pc_string, 0, 0, 0, 0, 0);
    }

#ifdef _PRE_WLAN_REPORT_PRODUCT_LOG
    oam_report_product_log(ul_para, pc_string, 0, 0, 0, 0);
#endif

    return ul_ret;
}

/*
 * �� �� ��  : oam_log_print1
 * ��������  : ��־��ӡ������һ������
 */
oal_uint32 oam_log_print1(oal_uint32 ul_para,
                          oal_uint16 us_line_num,
                          oal_int8 *pc_string,
                          oal_uint l_param1)
{
    oal_uint32 ul_ret = OAL_SUCC;
    if (oam_get_log_switch(ul_para) == OAL_SWITCH_ON) {
#ifdef _PRE_WLAN_DFT_LOG
        OAL_PRINT2KERNEL(ul_para, us_line_num, pc_string, l_param1, 0, 0, 0);
#endif
        ul_ret = oam_log_print_n_param(ul_para, us_line_num,
                                       pc_string, 1, (oal_int32)l_param1, 0, 0, 0);
    }

#ifdef _PRE_WLAN_REPORT_PRODUCT_LOG
    oam_report_product_log(ul_para, pc_string, l_param1, 0, 0, 0);
#endif

    return ul_ret;
}

/*
 * �� �� ��  : oam_log_print2
 * ��������  : ��־��ӡ��������������
 */
oal_uint32 oam_log_print2(oal_uint32 ul_para,
                          oal_uint16 us_line_num,
                          oal_int8 *pc_string,
                          oal_uint l_param1,
                          oal_uint l_param2)
{
    oal_uint32 ul_ret = OAL_SUCC;
    if (oam_get_log_switch(ul_para) == OAL_SWITCH_ON) {
#ifdef _PRE_WLAN_DFT_LOG
        OAL_PRINT2KERNEL(ul_para, us_line_num, pc_string, l_param1, l_param2, 0, 0);
#endif
        /* ��������Ϊ2 */
        ul_ret = oam_log_print_n_param(ul_para, us_line_num,
                                       pc_string, 2, (oal_int32)l_param1, (oal_int32)l_param2, 0, 0);
    }

#ifdef _PRE_WLAN_REPORT_PRODUCT_LOG
    oam_report_product_log(ul_para, pc_string, l_param1, l_param2, 0, 0);
#endif

    return ul_ret;
}

/*
 * �� �� ��  : oam_log_print3
 * ��������  : ��־��ӡ��������������
 */
oal_uint32 oam_log_print3(oal_uint32 ul_para,
                          oal_uint16 us_line_num,
                          oal_int8 *pc_string,
                          oal_uint l_param1,
                          oal_uint l_param2,
                          oal_uint l_param3)
{
    oal_uint32 ul_ret = OAL_SUCC;
    if (oam_get_log_switch(ul_para) == OAL_SWITCH_ON) {
#ifdef _PRE_WLAN_DFT_LOG
        OAL_PRINT2KERNEL(ul_para, us_line_num, pc_string, l_param1, l_param2, l_param3, 0);
#endif
        /* ��������Ϊ3 */
        ul_ret = oam_log_print_n_param(ul_para, us_line_num,
                                       pc_string, 3, (oal_int32)l_param1, (oal_int32)l_param2, (oal_int32)l_param3, 0);
    }

#ifdef _PRE_WLAN_REPORT_PRODUCT_LOG
    oam_report_product_log(ul_para, pc_string, l_param1, l_param2, l_param3, 0);
#endif

    return ul_ret;
}

/*
 * �� �� ��  : oam_log_print4
 * ��������  : ��־��ӡ�������ĸ�����
 */
oal_uint32 oam_log_print4(oal_uint32 ul_para,
                          oal_uint16 us_line_num,
                          oal_int8 *pc_string,
                          oal_uint l_param1,
                          oal_uint l_param2,
                          oal_uint l_param3,
                          oal_uint l_param4)
{
    oal_uint32 ul_ret = OAL_SUCC;
    if (oam_get_log_switch(ul_para) == OAL_SWITCH_ON) {
#ifdef _PRE_WLAN_DFT_LOG
        OAL_PRINT2KERNEL(ul_para, us_line_num, pc_string, l_param1, l_param2, l_param3, l_param4);
#endif
        /* ��������Ϊ4 */
        ul_ret = oam_log_print_n_param(ul_para, us_line_num, pc_string, 4, (oal_int32)l_param1,
                                       (oal_int32)l_param2, (oal_int32)l_param3, (oal_int32)l_param4);
    }

#ifdef _PRE_WLAN_REPORT_PRODUCT_LOG
    oam_report_product_log(ul_para, pc_string, l_param1, l_param2, l_param3, l_param4);
#endif

    return ul_ret;
}

/*
 * �� �� ��  : oam_log_printk
 * ��������  : ���������־
 */
OAL_STATIC oal_uint32 oam_log_printk(oal_uint16 us_file_no,
                                     oal_uint16 us_line_num,
                                     const oal_int8 *pc_func_name,
                                     const char *pc_args_buf)
{
    oal_int8 ac_output_data[OAM_PRINT_FORMAT_LENGTH]; /* ���ڱ���д�뵽�ļ��еĸ�ʽ */
    oal_int8 *pac_printk_format = (oal_int8 *)"Tick=%lu, FileId=%d, LineNo=%d, FuncName::%s, \"%s\"\r\n";
    oal_uint32 ul_tick;
    oal_int32 ret;

    ul_tick = (oal_uint32)OAL_TIME_GET_STAMP_MS();

    ret = snprintf_s(ac_output_data, OAM_PRINT_FORMAT_LENGTH, OAM_PRINT_FORMAT_LENGTH - 1,
                     pac_printk_format, ul_tick, us_file_no,
                     us_line_num, pc_func_name, pc_args_buf);
    if (ret < 0) {
        OAL_IO_PRINT("log str format err\n");
        return OAL_FAIL;
    }
    OAL_IO_PRINT("%s\r\n", ac_output_data);

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_console_printk
 * ��������  : ���������־����
 *             ��ֹ����Ƶ����ӡ���ô���ʹ��������
 */
oal_uint32 oam_log_console_printk(oal_uint16 us_file_no,
                                  oal_uint16 us_line_num,
                                  const oal_int8 *pc_func_name,
                                  const oal_int8 *pc_fmt, ...)
{
    oal_int8 ac_args_buf[OAM_PRINT_FORMAT_LENGTH];
    OAL_VA_LIST pc_args;
    oal_int32 ret;

    if (OAL_UNLIKELY(OAL_ANY_NULL_PTR2(pc_func_name, pc_fmt))) {
        OAM_IO_PRINTK("null param. \r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* �����ж� */
    if (oam_log_ratelimit(OAM_RATELIMIT_TYPE_PRINTK) == OAM_RATELIMIT_NOT_OUTPUT) {
        return OAL_SUCC;
    }

    OAL_VA_START(pc_args, pc_fmt);
    ret = vsnprintf_s(ac_args_buf, OAM_PRINT_FORMAT_LENGTH, OAM_PRINT_FORMAT_LENGTH - 1, pc_fmt, pc_args);
    OAL_VA_END(pc_args);

    if (ret < 0) {
        OAL_IO_PRINT("print str format err\n");
        return OAL_FAIL;
    }
    oam_log_printk(us_file_no, us_line_num, pc_func_name, ac_args_buf);

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_log_ratelimit_init
 * ��������  : printk��־���س�ʼ��
 */
oal_uint32 oam_log_ratelimit_init(oal_void)
{
    oal_uint32 ul_ret = OAL_SUCC;
    oam_ratelimit_type_enum_uint8 en_type_idx;

    memset_s(&oam_mng_ctx.st_log_ctx.st_ratelimit, OAL_SIZEOF(oam_ratelimit_stru) * OAM_RATELIMIT_TYPE_BUTT, 0,
             OAL_SIZEOF(oam_ratelimit_stru) * OAM_RATELIMIT_TYPE_BUTT);

    for (en_type_idx = 0; en_type_idx < OAM_RATELIMIT_TYPE_BUTT; en_type_idx++) {
        oal_spin_lock_init(&oam_mng_ctx.st_log_ctx.st_ratelimit[en_type_idx].spin_lock);
        ul_ret += oam_log_set_ratelimit_switch(en_type_idx, OAL_SWITCH_OFF);
        ul_ret += oam_log_set_ratelimit_intervel(en_type_idx, OAM_RATELIMIT_DEFAULT_INTERVAL);
        ul_ret += oam_log_set_ratelimit_burst(en_type_idx, OAM_RATELIMIT_DEFAULT_BURST);
    }
    return ul_ret;
}

oal_void oam_log_param_init(oal_void)
{
#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
#ifdef CONFIG_PRINTK
    oal_int32 i;
    for (i = 0; i < OAM_LOG_LEVEL_BUTT; i++) {
        g_loglevel_string[i] = KERN_INFO "[X][HI11XX]";
    }
    g_loglevel_string[OAM_LOG_LEVEL_ERROR] = KERN_ERR "[E][HI11XX]";
    g_loglevel_string[OAM_LOG_LEVEL_WARNING] = KERN_INFO "[W][HI11XX]";
    g_loglevel_string[OAM_LOG_LEVEL_INFO] = KERN_INFO "[I][HI11XX]";
#endif
#endif
}

/*
 * �� �� ��  : oam_log_init
 * ��������  : ��LOGģ����г�ʼ������
 *             (ϵͳ�ϵ��ϵͳ��λ���ã���oam_init����ִ��������)
 */
oal_uint32 oam_log_init(oal_void)
{
    oal_uint8 uc_vap_idx;
    oal_uint32 ul_ret;
#ifdef _PRE_WLAN_REPORT_PRODUCT_LOG
    oal_uint8 uc_feature_idx;
    oal_uint8 auc_feature_ont_cfg[] = {};
    oal_uint8 auc_feature_ont_conn[] = { OAM_SF_CONN };
    oal_uint8 auc_feature_ont_cmdout[] = {};
    oal_uint8 auc_feature_ont_channel[] = { OAM_SF_CHAN };
    oal_uint8 auc_feature_ont_collision[] = {};
#endif

    oam_log_param_init();

    /* ��־ȫ�ֿ���Ĭ��Ϊ�� */
    ul_ret = oam_log_set_global_switch(OAL_SWITCH_ON);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    /* VAP������־���� */
    for (uc_vap_idx = 0; uc_vap_idx < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; uc_vap_idx++) {
        /* ����VAP��־���� */
        ul_ret += oam_log_set_vap_switch(uc_vap_idx, OAL_SWITCH_ON);

        /* ����VAP��־���� */
        ul_ret += oam_log_set_vap_level(uc_vap_idx, OAM_LOG_DEFAULT_LEVEL);

#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
        /* ����feature��ӡ���� */
        ul_ret += oam_log_set_feature_level(uc_vap_idx, OAM_SF_WPA, OAM_LOG_LEVEL_INFO);
#endif
        if (ul_ret != OAL_SUCC) {
            return ul_ret;
        }
    }

    /* printk��־���س�ʼ�� */
    ul_ret = oam_log_ratelimit_init();

#ifdef _PRE_WLAN_REPORT_PRODUCT_LOG
    // ont log ��ʼ����Ĭ��ΪOAM_ONT_LOG_DEFAULT_EVENT
    for (uc_feature_idx = 0; uc_feature_idx < OAM_SOFTWARE_FEATURE_BUTT; uc_feature_idx++) {
        g_featureid_to_eventid[uc_feature_idx] = OAM_ONT_LOG_DEFAULT_EVENT;
    }

    // ��ʼ�� cfg
    for (uc_feature_idx = 0; uc_feature_idx < OAL_SIZEOF(auc_feature_ont_cfg) / OAL_SIZEOF(oal_uint8);
         uc_feature_idx++) {
        g_featureid_to_eventid[auc_feature_ont_cfg[uc_feature_idx]] = HW_KER_WIFI_LOG_CONFIG;
    }
    // ��ʼ�� connect
    for (uc_feature_idx = 0; uc_feature_idx < OAL_SIZEOF(auc_feature_ont_conn) / OAL_SIZEOF(oal_uint8);
         uc_feature_idx++) {
        g_featureid_to_eventid[auc_feature_ont_conn[uc_feature_idx]] = HW_KER_WIFI_LOG_CONNECT;
    }
    // ��ʼ�� cmdout
    for (uc_feature_idx = 0; uc_feature_idx < OAL_SIZEOF(auc_feature_ont_cmdout) / OAL_SIZEOF(oal_uint8);
         uc_feature_idx++) {
        g_featureid_to_eventid[auc_feature_ont_cmdout[uc_feature_idx]] = HW_KER_WIFI_LOG_CMDOUT;
    }
    // ��ʼ�� channel
    for (uc_feature_idx = 0; uc_feature_idx < OAL_SIZEOF(auc_feature_ont_channel) / OAL_SIZEOF(oal_uint8);
         uc_feature_idx++) {
        g_featureid_to_eventid[auc_feature_ont_channel[uc_feature_idx]] = HW_KER_WIFI_LOG_CHANNEL;
    }
    // ��ʼ�� collision
    for (uc_feature_idx = 0; uc_feature_idx < OAL_SIZEOF(auc_feature_ont_collision) / OAL_SIZEOF(oal_uint8);
         uc_feature_idx++) {
        g_featureid_to_eventid[auc_feature_ont_collision[uc_feature_idx]] = HW_KER_WIFI_LOG_COLLISION;
    }
#endif

    return ul_ret;
}

/*
 * �� �� ��  : oam_exception_record
 * ��������  : �쳣ͳ�Ƽ�¼����
 */
oal_uint32 oam_exception_record(oal_uint8 uc_vap_id, oam_excp_type_enum_uint8 en_excp_id)
{
    if (OAL_UNLIKELY(uc_vap_id >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT)) {
        OAM_IO_PRINTK("invalid uc_vap_id[%d] \r\n", uc_vap_id);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    if (OAL_UNLIKELY(en_excp_id >= OAM_EXCP_TYPE_BUTT)) {
        OAM_IO_PRINTK("invalid en_excp_id[%d]. \r\n", en_excp_id);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    oam_mng_ctx.st_exception_ctx[uc_vap_id].ast_excp_record_tbl[en_excp_id].ul_record_cnt++;

    /* ��ˢ�£����ϱ� */
    oam_mng_ctx.st_exception_ctx[uc_vap_id].en_status = OAM_EXCP_STATUS_REFRESHED;

    oam_mng_ctx.st_exception_ctx[uc_vap_id].ast_excp_record_tbl[en_excp_id].en_status = OAM_EXCP_STATUS_REFRESHED;

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_exception_stat_report
 * ��������  : �쳣ͳ���ϱ��������
 */
oal_uint32 oam_exception_stat_report(oal_uint8 uc_vap_id,
                                     oam_excp_type_enum_uint8 en_excp_id,
                                     oal_uint32 ul_cnt)
{
    switch (en_excp_id) {
        case OAM_HAL_MAC_ERROR_PARA_CFG_ERR:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY, "{Exception Statistics::OAM_HAL_MAC_ERROR_PARA_CFG_ERR.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_RXBUFF_LEN_TOO_SMALL:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_RXBUFF_LEN_TOO_SMALL.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_BA_ENTRY_NOT_FOUND:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_BA_ENTRY_NOT_FOUND.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_PHY_TRLR_TIME_OUT:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_PHY_TRLR_TIME_OUT.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_PHY_RX_FIFO_OVERRUN:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_PHY_RX_FIFO_OVERRUN.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_TX_DATAFLOW_BREAK:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_TX_DATAFLOW_BREAK.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_TX_FSM_ST_TIMEOUT:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_TX_FSM_ST_TIMEOUT.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_RX_HANDLER_ST_TIMEOUT:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_RX_HANDLER_ST_TIMEOUT.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_TX_HANDLER_ST_TIMEOUT:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_TX_HANDLER_ST_TIMEOUT.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_TX_INTR_FIFO_OVERRUN:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_TX_INTR_FIFO_OVERRUN.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_RX_INTR_FIFO_OVERRUN:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_RX_INTR_FIFO_OVERRUN.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_HIRX_INTR_FIFO_OVERRUN:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_HIRX_INTR_FIFO_OVERRUN.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_RX_Q_EMPTY:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_RX_Q_EMPTY.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_HIRX_Q_EMPTY:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_HIRX_Q_EMPTY.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_BUS_RLEN_ERR:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_BUS_RLEN_ERR.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_BUS_RADDR_ERR:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_BUS_RADDR_ERR.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_BUS_WLEN_ERR:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_BUS_WLEN_ERR.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_BUS_WADDR_ERR:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_BUS_WADDR_ERR.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_TX_ACBK_Q_OVERRUN:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_TX_ACBK_Q_OVERRUN.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_TX_ACBE_Q_OVERRUN:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_TX_ACBE_Q_OVERRUN.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_TX_ACVI_Q_OVERRUN:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_TX_ACVI_Q_OVERRUN.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_TX_ACVO_Q_OVERRUN:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_TX_ACVO_Q_OVERRUN.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_TX_HIPRI_Q_OVERRUN:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_TX_HIPRI_Q_OVERRUN.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_MATRIX_CALC_TIMEOUT:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_MATRIX_CALC_TIMEOUT.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_CCA_TIMEOUT:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_CCA_TIMEOUT.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_DCOL_DATA_OVERLAP:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_DCOL_DATA_OVERLAP.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_BEACON_MISS:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_BEACON_MISS.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_UNKOWN_28:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_UNKOWN_28.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_UNKOWN_29:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_UNKOWN_29.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_UNKOWN_30:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_UNKOWN_30.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_MAC_ERROR_UNKOWN_31:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_MAC_ERROR_UNKOWN_31.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_SOC_ERROR_BUCK_OCP:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_SOC_ERROR_BUCK_OCP.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_SOC_ERROR_BUCK_SCP:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_SOC_ERROR_BUCK_SCP.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_SOC_ERROR_OCP_RFLDO1:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_SOC_ERROR_OCP_RFLDO1.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_SOC_ERROR_OCP_RFLDO2:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_SOC_ERROR_OCP_RFLDO2.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_SOC_ERROR_OCP_CLDO:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_SOC_ERROR_OCP_CLDO.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_SOC_ERROR_RF_OVER_TEMP:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_SOC_ERROR_RF_OVER_TEMP.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_SOC_ERROR_CMU_UNLOCK:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_SOC_ERROR_CMU_UNLOCK.[%d]}\r\n",
                           ul_cnt);
            break;

        case OAM_HAL_SOC_ERROR_PCIE_SLV_ERR:
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_ANY,
                           "{Exception Statistics::OAM_HAL_SOC_ERROR_PCIE_SLV_ERR.[%d]}\r\n",
                           ul_cnt);
            break;

        default:
            break;
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : oam_exception_stat_handler
 * ��������  : ����쳣�Ĵ�����
 */
oal_void oam_exception_stat_handler(oal_uint8 en_moduleid, oal_uint8 uc_vap_idx)
{
    oam_excp_record_stru *pst_excp_record = NULL;
    oam_excp_type_enum_uint8 en_excp_idx;

    switch (en_moduleid) {
        case OM_WIFI: {
            /* ��ǰVAP�쳣ͳ��Ϊ0 */
            if (oam_mng_ctx.st_exception_ctx[uc_vap_idx].en_status != OAM_EXCP_STATUS_REFRESHED) {
            } else {
                pst_excp_record = oam_mng_ctx.st_exception_ctx[uc_vap_idx].ast_excp_record_tbl;

                for (en_excp_idx = 0; en_excp_idx < OAM_EXCP_TYPE_BUTT; en_excp_idx++) {
                    /* ��¼����ˢ�� */
                    if (pst_excp_record[en_excp_idx].en_status == OAM_EXCP_STATUS_REFRESHED) {
                        oam_exception_stat_report(uc_vap_idx, en_excp_idx, pst_excp_record[en_excp_idx].ul_record_cnt);
                        oam_mng_ctx.st_exception_ctx[uc_vap_idx].ast_excp_record_tbl[en_excp_idx].en_status = OAM_EXCP_STATUS_INIT;
                    }
                }

                /* ���ϱ����ó�ʼ״̬ */
                oam_mng_ctx.st_exception_ctx[uc_vap_idx].en_status = OAM_EXCP_STATUS_INIT;
            }
        }
        break;

        default:
            break;
    }
}

/*lint -e19*/
oal_module_symbol(OAL_PRINT2KERNEL);
#ifdef _PRE_WLAN_REPORT_PRODUCT_LOG
oal_module_symbol(oam_report_product_log);
#endif
oal_module_symbol(oam_log_print0);
oal_module_symbol(oam_log_set_global_switch);
oal_module_symbol(oam_log_set_vap_switch);
oal_module_symbol(oam_log_set_vap_level);
oal_module_symbol(oam_log_set_feature_level);
oal_module_symbol(oam_log_console_printk);
oal_module_symbol(oam_log_set_ratelimit_param);
oal_module_symbol(oam_get_feature_id);
oal_module_symbol(oam_log_get_vap_level);
oal_module_symbol(oam_show_feature_list);

oal_module_symbol(oam_log_print1);
oal_module_symbol(oam_log_print2);
oal_module_symbol(oam_log_print3);
oal_module_symbol(oam_log_print4);
oal_module_symbol(oam_exception_record);
oal_module_symbol(oam_exception_stat_handler);

#ifdef _PRE_DEBUG_MODE
oal_module_symbol(g_ast_tx_complete_stat);
#endif

#ifdef _PRE_WLAN_REPORT_PRODUCT_LOG
oal_module_symbol(g_vapid_to_chipid);
#endif

#ifdef _PRE_WLAN_REPORT_PRODUCT_LOG
oal_module_symbol(g_en_report_product_log_flag);
#endif
