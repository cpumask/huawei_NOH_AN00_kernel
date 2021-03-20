#ifndef __EFUSE_BALONG_AGENT__H
#define __EFUSE_BALONG_AGENT__H

#include <osl_types.h>
#include <hi_efuse.h>

typedef struct {
    u32 opt;
    u32 start;
    u32 len;
    int ret;
    u32 buf[EFUSE_MAX_SIZE];
} efuse_data_s;

typedef struct {
    u32 icc_callback_stamp;
    u32 rw_start_stamp;
    u32 rw_end_stamp;
    u32 icc_send_stamp;
} efuse_debug_info_s;

#ifdef EFUSE_FUSION_MSG
typedef struct {
    struct msg_head hdr;
    efuse_data_s content;
}efuse_msg_s;
#endif
#endif
