

#ifndef __HMAC_MAIN_H__
#define __HMAC_MAIN_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "oam_ext_if.h"
#include "hmac_ext_if.h"


#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MAIN_H

/* 2 �궨�� */
#define HMAC_ERR_LOG(_uc_vap_id, _puc_string)
#define HMAC_ERR_LOG1(_uc_vap_id, _puc_string, _l_para1)
#define HMAC_ERR_LOG2(_uc_vap_id, _puc_string, _l_para1, _l_para2)
#define HMAC_ERR_LOG3(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3)
#define HMAC_ERR_LOG4(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3, _l_para4)
#define HMAC_ERR_VAR(_uc_vap_id, _c_fmt, ...)

#define HMAC_WARNING_LOG(_uc_vap_id, _puc_string)
#define HMAC_WARNING_LOG1(_uc_vap_id, _puc_string, _l_para1)
#define HMAC_WARNING_LOG2(_uc_vap_id, _puc_string, _l_para1, _l_para2)
#define HMAC_WARNING_LOG3(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3)
#define HMAC_WARNING_LOG4(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3, _l_para4)
#define HMAC_WARNING_VAR(_uc_vap_id, _c_fmt, ...)

#define HMAC_INFO_LOG(_uc_vap_id, _puc_string)
#define HMAC_INFO_LOG1(_uc_vap_id, _puc_string, _l_para1)
#define HMAC_INFO_LOG2(_uc_vap_id, _puc_string, _l_para1, _l_para2)
#define HMAC_INFO_LOG3(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3)
#define HMAC_INFO_LOG4(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3, _l_para4)
#define HMAC_INFO_VAR(_uc_vap_id, _c_fmt, ...)

/* ��дHMAC��DMAC����ͬ����Ϣͷ */
#define HMAC_INIT_SYN_MSG_HDR(_pst_syn_msg, _en_syn_id, _us_len) \
    do {                                                         \
        (_pst_syn_msg)->en_syn_id = (_en_syn_id);                \
        (_pst_syn_msg)->us_len = (_us_len);                      \
    } while (0)

/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
typedef struct {
    uint32_t ul_time_stamp;
} hmac_timeout_event_stru;

typedef struct {
    uint32_t ul_cfg_id;
    uint32_t ul_ac;
    uint32_t ul_value;
} hmac_config_wmm_para_stru;

typedef struct {
    struct semaphore st_rxdata_sema;
    oal_task_stru *pst_rxdata_thread;
    oal_spin_lock_stru st_lock;
    oal_wait_queue_head_stru st_rxdata_wq;
    oal_netbuf_head_stru st_rxdata_netbuf_head;
    uint32_t ul_pkt_loss_cnt;
    oal_bool_enum_uint8 en_rxthread_enable;
    uint8_t uc_allowed_cpus;
} hmac_rxdata_thread_stru;

#if defined(_PRE_FEATURE_PLAT_LOCK_CPUFREQ) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
typedef struct {
    uint32_t max_cpu_freq;
    uint32_t valid;
}hisi_max_cpu_freq;
#endif
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
uint32_t hmac_event_fsm_register(void);
void hmac_main_exit(void);
int32_t hmac_main_init(void);
uint32_t hmac_config_send_event(mac_vap_stru *pst_mac_vap,
                                             wlan_cfgid_enum_uint16 en_cfg_id,
                                             uint16_t us_len,
                                             uint8_t *puc_param);
uint32_t hmac_sdt_up_reg_val(frw_event_mem_stru *pst_event_mem);

#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
uint32_t hmac_sdt_recv_sample_cmd(mac_vap_stru *pst_mac_vap, uint8_t *puc_buf,
                                           uint16_t us_len);
uint32_t hmac_sdt_up_sample_data(frw_event_mem_stru *pst_event_mem);

#endif

#ifdef _PRE_WLAN_ONLINE_DPD
uint32_t hmac_sdt_up_dpd_data(frw_event_mem_stru *pst_event_mem);
#endif
#ifdef _PRE_WLAN_FEATURE_APF
uint32_t hmac_apf_program_report_event(frw_event_mem_stru *pst_event_mem);
#endif

void hmac_rxdata_netbuf_enqueue(oal_netbuf_stru *pst_netbuf);
void hmac_rxdata_sched(void);
oal_bool_enum_uint8 hmac_get_rxthread_enable(void);

uint32_t hmac_proc_query_response_event(mac_vap_stru *pst_mac_vap, uint8_t uc_len,
                                                     uint8_t *puc_param);
int32_t hmac_rxdata_polling(struct napi_struct *pst_napi, int32_t l_data);

extern oal_wakelock_stru g_st_hmac_wakelock;
#define hmac_wake_lock()   oal_wake_lock(&g_st_hmac_wakelock)
#define hmac_wake_unlock() oal_wake_unlock(&g_st_hmac_wakelock)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_main */
