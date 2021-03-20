

#ifndef __HMAC_TCP_OPT_STRUC_H__
#define __HMAC_TCP_OPT_STRUC_H__

#ifdef _PRE_WLAN_TCP_OPT

/* 1 ����ͷ�ļ����� */
#include "oal_types.h"
#include "oal_net.h"
#include "oam_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_TCP_OPT_STRUC_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 2 �궨�� */
#define HMAC_TCP_STREAM         32
#define TCPDATA_INFO_MAXNUM     4
#define TCPDATA_PSH_INFO_MAXNUM (8 * TCPDATA_INFO_MAXNUM)

/* 3 ö�ٶ��� */
enum {
    TCP_ACK_FILTER_TYPE = 1,
    TCP_ACK_DUP_TYPE = 2,
    TCP_TYPE_ERROR
};
typedef uint8_t oal_tcp_ack_type_enum_uint8;
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
typedef struct _wlan_perform_tcp_impl_ {
    uint64_t ull_drop_count;
    uint64_t ull_send_count;
    uint64_t ull_dup_ack_count;
} wlan_perform_tcp_impl;

typedef struct _wlan_perform_tcp_impls_ {
    uint32_t ul_ack_limit;
    uint64_t ull_ignored_count;
    wlan_perform_tcp_impl st_tcp_info[HMAC_TCP_STREAM];
} wlan_perform_tcp_impls;

struct skb_tcp_cb {
    uint16_t us_index;
    uint16_t us_reserve;
    uint32_t ul_tcp_type;
    uint32_t ul_tcp_ack_no;
};

struct wlan_perform_tcp {
    /* hcc rx/tx tcp ack param */
    oal_netbuf_head_stru hcc_ack_queue[HMAC_TCP_STREAM];
    uint32_t aul_hcc_ack_count[HMAC_TCP_STREAM]; /* ack num in each queue */
    uint32_t aul_hcc_duplicate_ack_count[HMAC_TCP_STREAM];
    uint32_t aul_hcc_tcp_ack_no[HMAC_TCP_STREAM];
    oal_spin_lock_stru hmac_tcp_ack_lock;
#ifdef HCC_DEBUG
    uint32_t aul_ack_count[HMAC_TCP_STREAM];
    uint32_t aul_duplicate_ack_count[HMAC_TCP_STREAM];
#endif
};
struct wlan_tcp_flow {
    uint32_t ul_src_ip;
    uint32_t ul_dst_ip;
    uint16_t us_src_port;
    uint16_t us_dst_port;
    uint8_t uc_protocol;
};
struct tcp_list_node {
    oal_dlist_head_stru list;
    uint32_t ul_used; /* 0-free, 1-used */
    struct wlan_tcp_flow wlan_tcp_info;
    uint32_t ul_index;
    unsigned long ul_last_ts; /* time of last frame */
};
struct wlan_perform_tcp_list {
    struct tcp_list_node tcp_pool[HMAC_TCP_STREAM];
    uint32_t ul_free_count;
    oal_spin_lock_stru tcp_lock;      /* lock for tcp pool */
    oal_spin_lock_stru tcp_list_lock; /* lock for tcp list */
    oal_dlist_head_stru tcp_list;
};

/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
/* 11 ������������ */
#endif /* end of _PRE_WLAN_TCP_OPT */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_tcp_opt_struc.h */
