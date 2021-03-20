#ifndef __C_UNION_DEFINE_H__
#define __C_UNION_DEFINE_H__ 
typedef union
{
    struct
    {
        unsigned int ch0_done_mask : 1 ;
        unsigned int ch0_data_r_err_mask : 1 ;
        unsigned int ch0_data_w_err_mask : 1 ;
        unsigned int ch0_sq_r_err_mask : 1 ;
        unsigned int ch0_cq_w_err_mask : 1 ;
        unsigned int ch0_tout_err_mask : 1 ;
        unsigned int reserved_0 : 2 ;
        unsigned int ch1_done_mask : 1 ;
        unsigned int ch1_data_r_err_mask : 1 ;
        unsigned int ch1_data_w_err_mask : 1 ;
        unsigned int ch1_sq_r_err_mask : 1 ;
        unsigned int ch1_cq_w_err_mask : 1 ;
        unsigned int ch1_tout_err_mask : 1 ;
        unsigned int reserved_1 : 2 ;
        unsigned int ch2_done_mask : 1 ;
        unsigned int ch2_data_r_err_mask : 1 ;
        unsigned int ch2_data_w_err_mask : 1 ;
        unsigned int ch2_sq_r_err_mask : 1 ;
        unsigned int ch2_cq_w_err_mask : 1 ;
        unsigned int ch2_tout_err_mask : 1 ;
        unsigned int reserved_2 : 2 ;
        unsigned int ch3_done_mask : 1 ;
        unsigned int ch3_data_r_err_mask : 1 ;
        unsigned int ch3_data_w_err_mask : 1 ;
        unsigned int ch3_sq_r_err_mask : 1 ;
        unsigned int ch3_cq_w_err_mask : 1 ;
        unsigned int ch3_tout_err_mask : 1 ;
        unsigned int reserved_3 : 2 ;
    } bits;
    unsigned int u32;
} U_INT_MASK0;
typedef union
{
    struct
    {
        unsigned int ch4_done_mask : 1 ;
        unsigned int ch4_data_r_err_mask : 1 ;
        unsigned int ch4_data_w_err_mask : 1 ;
        unsigned int ch4_sq_r_err_mask : 1 ;
        unsigned int ch4_cq_w_err_mask : 1 ;
        unsigned int ch4_tout_err_mask : 1 ;
        unsigned int reserved_0 : 2 ;
        unsigned int ch5_done_mask : 1 ;
        unsigned int ch5_data_r_err_mask : 1 ;
        unsigned int ch5_data_w_err_mask : 1 ;
        unsigned int ch5_sq_r_err_mask : 1 ;
        unsigned int ch5_cq_w_err_mask : 1 ;
        unsigned int ch5_tout_err_mask : 1 ;
        unsigned int reserved_1 : 2 ;
        unsigned int ch6_done_mask : 1 ;
        unsigned int ch6_data_r_err_mask : 1 ;
        unsigned int ch6_data_w_err_mask : 1 ;
        unsigned int ch6_sq_r_err_mask : 1 ;
        unsigned int ch6_cq_w_err_mask : 1 ;
        unsigned int ch6_tout_err_mask : 1 ;
        unsigned int reserved_2 : 2 ;
        unsigned int ch7_done_mask : 1 ;
        unsigned int ch7_data_r_err_mask : 1 ;
        unsigned int ch7_data_w_err_mask : 1 ;
        unsigned int ch7_sq_r_err_mask : 1 ;
        unsigned int ch7_cq_w_err_mask : 1 ;
        unsigned int ch7_tout_err_mask : 1 ;
        unsigned int reserved_3 : 2 ;
    } bits;
    unsigned int u32;
} U_INT_MASK1;
typedef union
{
    struct
    {
        unsigned int ch0_done_mask_dis : 1 ;
        unsigned int ch0_data_r_err_mask_dis : 1 ;
        unsigned int ch0_data_w_err_mask_dis : 1 ;
        unsigned int ch0_sq_r_err_mask_dis : 1 ;
        unsigned int ch0_cq_w_err_mask_dis : 1 ;
        unsigned int ch0_tout_err_mask_dis : 1 ;
        unsigned int reserved_0 : 2 ;
        unsigned int ch1_done_mask_dis : 1 ;
        unsigned int ch1_data_r_err_mask_dis : 1 ;
        unsigned int ch1_data_w_err_mask_dis : 1 ;
        unsigned int ch1_sq_r_err_mask_dis : 1 ;
        unsigned int ch1_cq_w_err_mask_dis : 1 ;
        unsigned int ch1_tout_err_mask_dis : 1 ;
        unsigned int reserved_1 : 2 ;
        unsigned int ch2_done_mask_dis : 1 ;
        unsigned int ch2_data_r_err_mask_dis : 1 ;
        unsigned int ch2_data_w_err_mask_dis : 1 ;
        unsigned int ch2_sq_r_err_mask_dis : 1 ;
        unsigned int ch2_cq_w_err_mask_dis : 1 ;
        unsigned int ch2_tout_err_mask_dis : 1 ;
        unsigned int reserved_2 : 2 ;
        unsigned int ch3_done_mask_dis : 1 ;
        unsigned int ch3_data_r_err_mask_dis : 1 ;
        unsigned int ch3_data_w_err_mask_dis : 1 ;
        unsigned int ch3_sq_r_err_mask_dis : 1 ;
        unsigned int ch3_cq_w_err_mask_dis : 1 ;
        unsigned int ch3_tout_err_mask_dis : 1 ;
        unsigned int reserved_3 : 2 ;
    } bits;
    unsigned int u32;
} U_INT_MASK_DIS0;
typedef union
{
    struct
    {
        unsigned int ch4_done_mask_dis : 1 ;
        unsigned int ch4_data_r_err_mask_dis : 1 ;
        unsigned int ch4_data_w_err_mask_dis : 1 ;
        unsigned int ch4_sq_r_err_mask_dis : 1 ;
        unsigned int ch4_cq_w_err_mask_dis : 1 ;
        unsigned int ch4_tout_err_mask_dis : 1 ;
        unsigned int reserved_0 : 2 ;
        unsigned int ch5_done_mask_dis : 1 ;
        unsigned int ch5_data_r_err_mask_dis : 1 ;
        unsigned int ch5_data_w_err_mask_dis : 1 ;
        unsigned int ch5_sq_r_err_mask_dis : 1 ;
        unsigned int ch5_cq_w_err_mask_dis : 1 ;
        unsigned int ch5_tout_err_mask_dis : 1 ;
        unsigned int reserved_1 : 2 ;
        unsigned int ch6_done_mask_dis : 1 ;
        unsigned int ch6_data_r_err_mask_dis : 1 ;
        unsigned int ch6_data_w_err_mask_dis : 1 ;
        unsigned int ch6_sq_r_err_mask_dis : 1 ;
        unsigned int ch6_cq_w_err_mask_dis : 1 ;
        unsigned int ch6_tout_err_mask_dis : 1 ;
        unsigned int reserved_2 : 2 ;
        unsigned int ch7_done_mask_dis : 1 ;
        unsigned int ch7_data_r_err_mask_dis : 1 ;
        unsigned int ch7_data_w_err_mask_dis : 1 ;
        unsigned int ch7_sq_r_err_mask_dis : 1 ;
        unsigned int ch7_cq_w_err_mask_dis : 1 ;
        unsigned int ch7_tout_err_mask_dis : 1 ;
        unsigned int reserved_3 : 2 ;
    } bits;
    unsigned int u32;
} U_INT_MASK_DIS1;
typedef union
{
    struct
    {
        unsigned int ch0_done_mask_val : 1 ;
        unsigned int ch0_data_r_err_mask_val : 1 ;
        unsigned int ch0_data_w_err_mask_val : 1 ;
        unsigned int ch0_sq_r_err_mask_val : 1 ;
        unsigned int ch0_cq_w_err_mask_val : 1 ;
        unsigned int ch0_tout_err_mask_val : 1 ;
        unsigned int reserved_0 : 2 ;
        unsigned int ch1_done_mask_val : 1 ;
        unsigned int ch1_data_r_err_mask_val : 1 ;
        unsigned int ch1_data_w_err_mask_val : 1 ;
        unsigned int ch1_sq_r_err_mask_val : 1 ;
        unsigned int ch1_cq_w_err_mask_val : 1 ;
        unsigned int ch1_tout_err_mask_val : 1 ;
        unsigned int reserved_1 : 2 ;
        unsigned int ch2_done_mask_val : 1 ;
        unsigned int ch2_data_r_err_mask_val : 1 ;
        unsigned int ch2_data_w_err_mask_val : 1 ;
        unsigned int ch2_sq_r_err_mask_val : 1 ;
        unsigned int ch2_cq_w_err_mask_val : 1 ;
        unsigned int ch2_tout_err_mask_val : 1 ;
        unsigned int reserved_2 : 2 ;
        unsigned int ch3_done_mask_val : 1 ;
        unsigned int ch3_data_r_err_mask_val : 1 ;
        unsigned int ch3_data_w_err_mask_val : 1 ;
        unsigned int ch3_sq_r_err_mask_val : 1 ;
        unsigned int ch3_cq_w_err_mask_val : 1 ;
        unsigned int ch3_tout_err_mask_val : 1 ;
        unsigned int reserved_3 : 2 ;
    } bits;
    unsigned int u32;
} U_INT_MASK_VAL0;
typedef union
{
    struct
    {
        unsigned int ch4_done_mask_val : 1 ;
        unsigned int ch4_data_r_err_mask_val : 1 ;
        unsigned int ch4_data_w_err_mask_val : 1 ;
        unsigned int ch4_sq_r_err_mask_val : 1 ;
        unsigned int ch4_cq_w_err_mask_val : 1 ;
        unsigned int ch4_tout_err_mask_val : 1 ;
        unsigned int reserved_0 : 2 ;
        unsigned int ch5_done_mask_val : 1 ;
        unsigned int ch5_data_r_err_mask_val : 1 ;
        unsigned int ch5_data_w_err_mask_val : 1 ;
        unsigned int ch5_sq_r_err_mask_val : 1 ;
        unsigned int ch5_cq_w_err_mask_val : 1 ;
        unsigned int ch5_tout_err_mask_val : 1 ;
        unsigned int reserved_1 : 2 ;
        unsigned int ch6_done_mask_val : 1 ;
        unsigned int ch6_data_r_err_mask_val : 1 ;
        unsigned int ch6_data_w_err_mask_val : 1 ;
        unsigned int ch6_sq_r_err_mask_val : 1 ;
        unsigned int ch6_cq_w_err_mask_val : 1 ;
        unsigned int ch6_tout_err_mask_val : 1 ;
        unsigned int reserved_2 : 2 ;
        unsigned int ch7_done_mask_val : 1 ;
        unsigned int ch7_data_r_err_mask_val : 1 ;
        unsigned int ch7_data_w_err_mask_val : 1 ;
        unsigned int ch7_sq_r_err_mask_val : 1 ;
        unsigned int ch7_cq_w_err_mask_val : 1 ;
        unsigned int ch7_tout_err_mask_val : 1 ;
        unsigned int reserved_3 : 2 ;
    } bits;
    unsigned int u32;
} U_INT_MASK_VAL1;
typedef union
{
    struct
    {
        unsigned int ch0_done_clr : 1 ;
        unsigned int ch0_data_r_err_clr : 1 ;
        unsigned int ch0_data_w_err_clr : 1 ;
        unsigned int ch0_sq_r_err_clr : 1 ;
        unsigned int ch0_cq_w_err_clr : 1 ;
        unsigned int ch0_tout_err_clr : 1 ;
        unsigned int reserved_0 : 2 ;
        unsigned int ch1_done_clr : 1 ;
        unsigned int ch1_data_r_err_clr : 1 ;
        unsigned int ch1_data_w_err_clr : 1 ;
        unsigned int ch1_sq_r_err_clr : 1 ;
        unsigned int ch1_cq_w_err_clr : 1 ;
        unsigned int ch1_tout_err_clr : 1 ;
        unsigned int reserved_1 : 2 ;
        unsigned int ch2_done_clr : 1 ;
        unsigned int ch2_data_r_err_clr : 1 ;
        unsigned int ch2_data_w_err_clr : 1 ;
        unsigned int ch2_sq_r_err_clr : 1 ;
        unsigned int ch2_cq_w_err_clr : 1 ;
        unsigned int ch2_tout_err_clr : 1 ;
        unsigned int reserved_2 : 2 ;
        unsigned int ch3_done_clr : 1 ;
        unsigned int ch3_data_r_err_clr : 1 ;
        unsigned int ch3_data_w_err_clr : 1 ;
        unsigned int ch3_sq_r_err_clr : 1 ;
        unsigned int ch3_cq_w_err_clr : 1 ;
        unsigned int ch3_tout_err_clr : 1 ;
        unsigned int reserved_3 : 2 ;
    } bits;
    unsigned int u32;
} U_INT_CLR0;
typedef union
{
    struct
    {
        unsigned int ch4_done_clr : 1 ;
        unsigned int ch4_data_r_err_clr : 1 ;
        unsigned int ch4_data_w_err_clr : 1 ;
        unsigned int ch4_sq_r_err_clr : 1 ;
        unsigned int ch4_cq_w_err_clr : 1 ;
        unsigned int ch4_tout_err_clr : 1 ;
        unsigned int reserved_0 : 2 ;
        unsigned int ch5_done_clr : 1 ;
        unsigned int ch5_data_r_err_clr : 1 ;
        unsigned int ch5_data_w_err_clr : 1 ;
        unsigned int ch5_sq_r_err_clr : 1 ;
        unsigned int ch5_cq_w_err_clr : 1 ;
        unsigned int ch5_tout_err_clr : 1 ;
        unsigned int reserved_1 : 2 ;
        unsigned int ch6_done_clr : 1 ;
        unsigned int ch6_data_r_err_clr : 1 ;
        unsigned int ch6_data_w_err_clr : 1 ;
        unsigned int ch6_sq_r_err_clr : 1 ;
        unsigned int ch6_cq_w_err_clr : 1 ;
        unsigned int ch6_tout_err_clr : 1 ;
        unsigned int reserved_2 : 2 ;
        unsigned int ch7_done_clr : 1 ;
        unsigned int ch7_data_r_err_clr : 1 ;
        unsigned int ch7_data_w_err_clr : 1 ;
        unsigned int ch7_sq_r_err_clr : 1 ;
        unsigned int ch7_cq_w_err_clr : 1 ;
        unsigned int ch7_tout_err_clr : 1 ;
        unsigned int reserved_3 : 2 ;
    } bits;
    unsigned int u32;
} U_INT_CLR1;
typedef union
{
    struct
    {
        unsigned int ch0_done_mint : 1 ;
        unsigned int ch0_data_r_err_mint : 1 ;
        unsigned int ch0_data_w_err_mint : 1 ;
        unsigned int ch0_sq_r_err_mint : 1 ;
        unsigned int ch0_cq_w_err_mint : 1 ;
        unsigned int ch0_tout_err_mint : 1 ;
        unsigned int reserved_0 : 2 ;
        unsigned int ch1_done_mint : 1 ;
        unsigned int ch1_data_r_err_mint : 1 ;
        unsigned int ch1_data_w_err_mint : 1 ;
        unsigned int ch1_sq_r_err_mint : 1 ;
        unsigned int ch1_cq_w_err_mint : 1 ;
        unsigned int ch1_tout_err_mint : 1 ;
        unsigned int reserved_1 : 2 ;
        unsigned int ch2_done_mint : 1 ;
        unsigned int ch2_data_r_err_mint : 1 ;
        unsigned int ch2_data_w_err_mint : 1 ;
        unsigned int ch2_sq_r_err_mint : 1 ;
        unsigned int ch2_cq_w_err_mint : 1 ;
        unsigned int ch2_tout_err_mint : 1 ;
        unsigned int reserved_2 : 2 ;
        unsigned int ch3_done_mint : 1 ;
        unsigned int ch3_data_r_err_mint : 1 ;
        unsigned int ch3_data_w_err_mint : 1 ;
        unsigned int ch3_sq_r_err_mint : 1 ;
        unsigned int ch3_cq_w_err_mint : 1 ;
        unsigned int ch3_tout_err_mint : 1 ;
        unsigned int reserved_3 : 2 ;
    } bits;
    unsigned int u32;
} U_MASK_INT0;
typedef union
{
    struct
    {
        unsigned int ch4_done_mint : 1 ;
        unsigned int ch4_data_r_err_mint : 1 ;
        unsigned int ch4_data_w_err_mint : 1 ;
        unsigned int ch4_sq_r_err_mint : 1 ;
        unsigned int ch4_cq_w_err_mint : 1 ;
        unsigned int ch4_tout_err_mint : 1 ;
        unsigned int reserved_0 : 2 ;
        unsigned int ch5_done_mint : 1 ;
        unsigned int ch5_data_r_err_mint : 1 ;
        unsigned int ch5_data_w_err_mint : 1 ;
        unsigned int ch5_sq_r_err_mint : 1 ;
        unsigned int ch5_cq_w_err_mint : 1 ;
        unsigned int ch5_tout_err_mint : 1 ;
        unsigned int reserved_1 : 2 ;
        unsigned int ch6_done_mint : 1 ;
        unsigned int ch6_data_r_err_mint : 1 ;
        unsigned int ch6_data_w_err_mint : 1 ;
        unsigned int ch6_sq_r_err_mint : 1 ;
        unsigned int ch6_cq_w_err_mint : 1 ;
        unsigned int ch6_tout_err_mint : 1 ;
        unsigned int reserved_2 : 2 ;
        unsigned int ch7_done_mint : 1 ;
        unsigned int ch7_data_r_err_mint : 1 ;
        unsigned int ch7_data_w_err_mint : 1 ;
        unsigned int ch7_sq_r_err_mint : 1 ;
        unsigned int ch7_cq_w_err_mint : 1 ;
        unsigned int ch7_tout_err_mint : 1 ;
        unsigned int reserved_3 : 2 ;
    } bits;
    unsigned int u32;
} U_MASK_INT1;
typedef union
{
    struct
    {
        unsigned int ch0_done_rint : 1 ;
        unsigned int ch0_data_r_err_rint : 1 ;
        unsigned int ch0_data_w_err_rint : 1 ;
        unsigned int ch0_sq_r_err_rint : 1 ;
        unsigned int ch0_cq_w_err_rint : 1 ;
        unsigned int ch0_tout_err_rint : 1 ;
        unsigned int reserved_0 : 2 ;
        unsigned int ch1_done_rint : 1 ;
        unsigned int ch1_data_r_err_rint : 1 ;
        unsigned int ch1_data_w_err_rint : 1 ;
        unsigned int ch1_sq_r_err_rint : 1 ;
        unsigned int ch1_cq_w_err_rint : 1 ;
        unsigned int ch1_tout_err_rint : 1 ;
        unsigned int reserved_1 : 2 ;
        unsigned int ch2_done_rint : 1 ;
        unsigned int ch2_data_r_err_rint : 1 ;
        unsigned int ch2_data_w_err_rint : 1 ;
        unsigned int ch2_sq_r_err_rint : 1 ;
        unsigned int ch2_cq_w_err_rint : 1 ;
        unsigned int ch2_tout_err_rint : 1 ;
        unsigned int reserved_2 : 2 ;
        unsigned int ch3_done_rint : 1 ;
        unsigned int ch3_data_r_err_rint : 1 ;
        unsigned int ch3_data_w_err_rint : 1 ;
        unsigned int ch3_sq_r_err_rint : 1 ;
        unsigned int ch3_cq_w_err_rint : 1 ;
        unsigned int ch3_tout_err_rint : 1 ;
        unsigned int reserved_3 : 2 ;
    } bits;
    unsigned int u32;
} U_RAW_INT0;
typedef union
{
    struct
    {
        unsigned int ch4_done_rint : 1 ;
        unsigned int ch4_data_r_err_rint : 1 ;
        unsigned int ch4_data_w_err_rint : 1 ;
        unsigned int ch4_sq_r_err_rint : 1 ;
        unsigned int ch4_cq_w_err_rint : 1 ;
        unsigned int ch4_tout_err_rint : 1 ;
        unsigned int reserved_0 : 2 ;
        unsigned int ch5_done_rint : 1 ;
        unsigned int ch5_data_r_err_rint : 1 ;
        unsigned int ch5_data_w_err_rint : 1 ;
        unsigned int ch5_sq_r_err_rint : 1 ;
        unsigned int ch5_cq_w_err_rint : 1 ;
        unsigned int ch5_tout_err_rint : 1 ;
        unsigned int reserved_1 : 2 ;
        unsigned int ch6_done_rint : 1 ;
        unsigned int ch6_data_r_err_rint : 1 ;
        unsigned int ch6_data_w_err_rint : 1 ;
        unsigned int ch6_sq_r_err_rint : 1 ;
        unsigned int ch6_cq_w_err_rint : 1 ;
        unsigned int ch6_tout_err_rint : 1 ;
        unsigned int reserved_2 : 2 ;
        unsigned int ch7_done_rint : 1 ;
        unsigned int ch7_data_r_err_rint : 1 ;
        unsigned int ch7_data_w_err_rint : 1 ;
        unsigned int ch7_sq_r_err_rint : 1 ;
        unsigned int ch7_cq_w_err_rint : 1 ;
        unsigned int ch7_tout_err_rint : 1 ;
        unsigned int reserved_3 : 2 ;
    } bits;
    unsigned int u32;
} U_RAW_INT1;
typedef union
{
    struct
    {
        unsigned int dbg_en : 1 ;
        unsigned int dbg_mode : 3 ;
        unsigned int dbg_ch_sel : 3 ;
        unsigned int reserved_0 : 1 ;
        unsigned int dbg_w_pattern : 8 ;
        unsigned int switch_en : 1 ;
        unsigned int l2_r_err_clr : 1 ;
        unsigned int l2_w_err_clr : 1 ;
        unsigned int reserved_1 : 13 ;
    } bits;
    unsigned int u32;
} U_SDMA_CTRL;
typedef union
{
    struct
    {
        unsigned int ie : 1 ;
        unsigned int mode : 3 ;
        unsigned int status : 4 ;
        unsigned int w_pattern : 8 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_DEBUG_SQE0;
typedef union
{
    struct
    {
        unsigned int s_sid : 8 ;
        unsigned int s_subsid : 8 ;
        unsigned int d_sid : 8 ;
        unsigned int d_subsid : 8 ;
    } bits;
    unsigned int u32;
} U_DEBUG_SQE1;
typedef union
{
    struct
    {
        unsigned int s_addr_h : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_DEBUG_SQE5;
typedef union
{
    struct
    {
        unsigned int d_addr_h : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_DEBUG_SQE7;
typedef union
{
    struct
    {
        unsigned int ch_en : 1 ;
        unsigned int ch_srst : 1 ;
        unsigned int qos : 2 ;
        unsigned int sq_size : 4 ;
        unsigned int cq_size : 4 ;
        unsigned int cqe_fmt_sel : 1 ;
        unsigned int reserved_0 : 19 ;
    } bits;
    unsigned int u32;
} U_CH_CTRL;
typedef union
{
    struct
    {
        unsigned int ch_status : 1 ;
        unsigned int reserved_0 : 7 ;
        unsigned int blk_cnt : 24 ;
    } bits;
    unsigned int u32;
} U_STATUS;
typedef union
{
    struct
    {
        unsigned int sq_sid : 8 ;
        unsigned int reserved_0 : 8 ;
        unsigned int sq_sub_sid : 8 ;
        unsigned int reserved_1 : 8 ;
    } bits;
    unsigned int u32;
} U_SQ_SID;
typedef union
{
    struct
    {
        unsigned int cq_sid : 8 ;
        unsigned int reserved_0 : 8 ;
        unsigned int cq_sub_sid : 8 ;
        unsigned int reserved_1 : 8 ;
    } bits;
    unsigned int u32;
} U_CQ_SID;
typedef union
{
    struct
    {
        unsigned int sq_base_h : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_SQ_BASE_H;
typedef union
{
    struct
    {
        unsigned int cq_base_h : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_CQ_BASE_H;
typedef union
{
    struct
    {
        unsigned int sq_wp : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_SQ_WP;
typedef union
{
    struct
    {
        unsigned int sq_rp : 16 ;
        unsigned int sq_rp_p : 16 ;
    } bits;
    unsigned int u32;
} U_SQ_RP;
typedef union
{
    struct
    {
        unsigned int cq_wp : 16 ;
        unsigned int cq_wp_p : 16 ;
    } bits;
    unsigned int u32;
} U_CQ_WP;
typedef union
{
    struct
    {
        unsigned int cq_rp : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_CQ_RPL;
typedef union
{
    struct
    {
        unsigned int l2_size : 3 ;
        unsigned int reserved_0 : 29 ;
    } bits;
    unsigned int u32;
} U_L2_SIZE;
typedef union
{
    struct
    {
        unsigned int r_credit_en : 1 ;
        unsigned int r_credit_en_dynam : 1 ;
        unsigned int w_credit_en : 1 ;
        unsigned int w_credit_en_dynam : 1 ;
        unsigned int credit_mode_dynam : 2 ;
        unsigned int reserved_0 : 2 ;
        unsigned int rosd_cfg : 6 ;
        unsigned int reserved_1 : 2 ;
        unsigned int wosd_cfg : 5 ;
        unsigned int reserved_2 : 3 ;
        unsigned int mon_en : 1 ;
        unsigned int mon_axi_sel : 1 ;
        unsigned int aif_clk_en : 1 ;
        unsigned int aif_clk_sel : 1 ;
        unsigned int reserved_3 : 4 ;
    } bits;
    unsigned int u32;
} U_BIU_CTRL0;
typedef union
{
    struct
    {
        unsigned int r_osd_th_dynam : 6 ;
        unsigned int reserved_0 : 10 ;
        unsigned int w_osd_th_dynam : 5 ;
        unsigned int reserved_1 : 11 ;
    } bits;
    unsigned int u32;
} U_BIU_CTRL1;
typedef union
{
    struct
    {
        unsigned int r_credit_step : 7 ;
        unsigned int r_credit_lth : 9 ;
        unsigned int r_credit_uth : 9 ;
        unsigned int reserved_0 : 7 ;
    } bits;
    unsigned int u32;
} U_BIU_CTRL2;
typedef union
{
    struct
    {
        unsigned int r_credit_step_dynam : 7 ;
        unsigned int r_credit_lth_dynam : 9 ;
        unsigned int r_credit_uth_dynam : 9 ;
        unsigned int reserved_0 : 7 ;
    } bits;
    unsigned int u32;
} U_BIU_CTRL3;
typedef union
{
    struct
    {
        unsigned int w_credit_step_dynam : 7 ;
        unsigned int w_credit_lth_dynam : 9 ;
        unsigned int w_credit_uth_dynam : 9 ;
        unsigned int reserved_0 : 7 ;
    } bits;
    unsigned int u32;
} U_BIU_CTRL4;
typedef union
{
    struct
    {
        unsigned int w_credit_step : 7 ;
        unsigned int w_credit_lth : 9 ;
        unsigned int w_credit_uth : 9 ;
        unsigned int reserved_0 : 7 ;
    } bits;
    unsigned int u32;
} U_BIU_CTRL5;
typedef union
{
    struct
    {
        unsigned int axi0_wstrob : 16 ;
        unsigned int axi1_wstrob : 16 ;
    } bits;
    unsigned int u32;
} U_BIU_STATUS7;
typedef union
{
    struct
    {
        unsigned int axi0_rosd : 8 ;
        unsigned int axi0_wosd : 8 ;
        unsigned int axi1_rosd : 8 ;
        unsigned int axi1_wosd : 8 ;
    } bits;
    unsigned int u32;
} U_BIU_STATUS8;
typedef union
{
    struct
    {
        unsigned int biu_idle : 1 ;
        unsigned int l2_r_err : 1 ;
        unsigned int l2_w_err : 1 ;
        unsigned int reserved_0 : 29 ;
    } bits;
    unsigned int u32;
} U_BIU_STATUS9;
typedef union
{
    struct
    {
        unsigned int brif_retire_cnt : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_BRIF_RETIRE_CNT;
typedef union
{
    struct
    {
        unsigned int bwif_retire_cnt : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_BWIF_RETIRE_CNT;
typedef union
{
    struct
    {
        unsigned int sqe_rd_req_cnt : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_SQE_RD_REQ_CNT;
typedef union
{
    struct
    {
        unsigned int sqe_rd_rsp_cnt : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_SQE_RD_RSP_CNT;
typedef union
{
    struct
    {
        unsigned int cqe_wr_req_cnt : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_CQE_WR_REQ_CNT;
typedef union
{
    struct
    {
        unsigned int cqe_wr_rsp_cnt : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_CQE_WR_RSP_CNT;
typedef union
{
    struct
    {
        unsigned int blk_rd_req_cnt : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_BLK_RD_REQ_CNT;
typedef union
{
    struct
    {
        unsigned int blk_wr_req_cnt : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_BLK_WR_REQ_CNT;
typedef union
{
    struct
    {
        unsigned int ch0_cur_st : 2 ;
        unsigned int ch1_cur_st : 2 ;
        unsigned int ch2_cur_st : 2 ;
        unsigned int ch3_cur_st : 2 ;
        unsigned int ch4_cur_st : 2 ;
        unsigned int ch5_cur_st : 2 ;
        unsigned int ch6_cur_st : 2 ;
        unsigned int ch7_cur_st : 2 ;
        unsigned int srif_cur_st : 3 ;
        unsigned int sdec_cur_st : 3 ;
        unsigned int cwif_cur_st : 3 ;
        unsigned int reserved_0 : 7 ;
    } bits;
    unsigned int u32;
} U_DMA_INER_FSM;
typedef union
{
    struct
    {
        unsigned int bwif_pld_fifo_ufl : 1 ;
        unsigned int bwif_pld_fifo_ovf : 1 ;
        unsigned int bwif_pld_fifo_ful : 1 ;
        unsigned int bwif_pld_fifo_emp : 1 ;
        unsigned int bwif_pld_fifo_fill : 2 ;
        unsigned int reserved_0 : 2 ;
        unsigned int cqe_fifo_ufl : 1 ;
        unsigned int cqe_fifo_ovf : 1 ;
        unsigned int cqe_fifo_ful : 1 ;
        unsigned int cqe_fifo_emp : 1 ;
        unsigned int cqe_fifo_fill : 2 ;
        unsigned int reserved_1 : 2 ;
        unsigned int bwif_err_fifo_ufl : 1 ;
        unsigned int bwif_err_fifo_ovf : 1 ;
        unsigned int bwif_err_fifo_ful : 1 ;
        unsigned int bwif_err_fifo_emp : 1 ;
        unsigned int bwif_err_fifo_fill : 4 ;
        unsigned int brif_err_fifo_ufl : 1 ;
        unsigned int brif_err_fifo_ovf : 1 ;
        unsigned int brif_err_fifo_ful : 1 ;
        unsigned int brif_err_fifo_emp : 1 ;
        unsigned int brif_err_fifo_fill : 4 ;
    } bits;
    unsigned int u32;
} U_DMA_INER_FIFO_0;
typedef union
{
    struct
    {
        unsigned int cid_fifo_ufl : 1 ;
        unsigned int cid_fifo_ovf : 1 ;
        unsigned int cid_fifo_ful : 1 ;
        unsigned int cid_fifo_emp : 1 ;
        unsigned int cid_fifo_fill : 2 ;
        unsigned int reserved_0 : 2 ;
        unsigned int bwif_stp_fifo_ufl : 1 ;
        unsigned int bwif_stp_fifo_ovf : 1 ;
        unsigned int bwif_stp_fifo_ful : 1 ;
        unsigned int bwif_stp_fifo_emp : 1 ;
        unsigned int bwif_stp_fifo_fill : 4 ;
        unsigned int brif_stp_fifo_ufl : 1 ;
        unsigned int brif_stp_fifo_ovf : 1 ;
        unsigned int brif_stp_fifo_ful : 1 ;
        unsigned int brif_stp_fifo_emp : 1 ;
        unsigned int brif_stp_fifo_fill : 4 ;
        unsigned int sw_en_patten : 4 ;
        unsigned int reserved_1 : 4 ;
    } bits;
    unsigned int u32;
} U_DMA_INER_FIFO_1;
typedef union
{
    struct
    {
        unsigned int wr_use_cnt : 16 ;
        unsigned int reserved_0 : 16 ;
    } bits;
    unsigned int u32;
} U_WR_USE_CNT;
typedef union
{
    unsigned int u32;
    struct
    {
        unsigned int sec_enable_ch : 8 ;
        unsigned int reserved : 8 ;
        unsigned int sec_enable_cmn : 1 ;
        unsigned int sec_enable_dbg : 1 ;
        unsigned int sec_enable_int0 : 1 ;
        unsigned int sec_enable_int1 : 1 ;
        unsigned int sec_enable_int2 : 1 ;
        unsigned int reserved_0 : 11 ;
    } bits;
} U_DOMAIN_SEC_EN;
typedef struct
{
    volatile U_INT_MASK0 INT_MASK0;
    volatile U_INT_MASK1 INT_MASK1;
    volatile U_INT_MASK_DIS0 INT_MASK_DIS0;
    volatile U_INT_MASK_DIS1 INT_MASK_DIS1;
    volatile U_INT_MASK_VAL0 INT_MASK_VAL0;
    volatile U_INT_MASK_VAL1 INT_MASK_VAL1;
    volatile U_INT_CLR0 INT_CLR0;
    volatile U_INT_CLR1 INT_CLR1;
    volatile U_MASK_INT0 MASK_INT0;
    volatile U_MASK_INT1 MASK_INT1;
    volatile U_RAW_INT0 RAW_INT0;
    volatile U_RAW_INT1 RAW_INT1;
    volatile U_SDMA_CTRL SDMA_CTRL;
    volatile U_DEBUG_SQE0 DEBUG_SQE0;
    volatile U_DEBUG_SQE1 DEBUG_SQE1;
    volatile unsigned int DEBUG_SQE2;
    volatile unsigned int DEBUG_SQE3;
    volatile unsigned int DEBUG_SQE4;
    volatile U_DEBUG_SQE5 DEBUG_SQE5;
    volatile unsigned int DEBUG_SQE6;
    volatile U_DEBUG_SQE7 DEBUG_SQE7;
    volatile unsigned int DMA_STATUS0;
    volatile unsigned int DMA_STATUS1;
    volatile unsigned int DMA_STATUS2;
    volatile unsigned int DMA_STATUS3;
    volatile unsigned int DMA_STATUS4;
    volatile unsigned int DMA_STATUS5;
    volatile unsigned int DMA_STATUS6;
    volatile unsigned int DMA_STATUS7;
    volatile unsigned int DMA_STATUS8;
    volatile unsigned int DMA_STATUS9;
    volatile unsigned int DMA_STATUS10;
    volatile unsigned int DMA_STATUS11;
    volatile unsigned int DMA_STATUS12;
    volatile unsigned int DMA_STATUS13;
    volatile unsigned int DMA_STATUS14;
    volatile unsigned int DMA_STATUS15;
    volatile unsigned int DMA_TIMEOUT_TH;
    volatile U_CH_CTRL CH_CTRL[8];
    volatile U_STATUS STATUS[8];
    volatile U_SQ_SID SQ_SID[8];
    volatile U_CQ_SID CQ_SID[8];
    volatile U_SQ_BASE_H SQ_BASE_H[8];
    volatile unsigned int SQ_BASE_L[8];
    volatile U_CQ_BASE_H CQ_BASE_H[8];
    volatile unsigned int CQ_BASE_L[8];
    volatile U_SQ_WP SQ_WP[8];
    volatile U_SQ_RP SQ_RP[8];
    volatile U_CQ_WP CQ_WP[8];
    volatile U_CQ_RPL CQ_RPL[8];
    volatile unsigned int L2_REMAP_VADDR_BASE_L;
    volatile unsigned int L2_REMAP_VADDR_BASE_H;
    volatile U_L2_SIZE L2_SIZE;
    volatile U_BIU_CTRL0 BIU_CTRL0;
    volatile U_BIU_CTRL1 BIU_CTRL1;
    volatile U_BIU_CTRL2 BIU_CTRL2;
    volatile U_BIU_CTRL3 BIU_CTRL3;
    volatile U_BIU_CTRL4 BIU_CTRL4;
    volatile U_BIU_CTRL5 BIU_CTRL5;
    volatile unsigned int BIU_STATUS0;
    volatile unsigned int BIU_STATUS1;
    volatile unsigned int BIU_STATUS2;
    volatile unsigned int BIU_STATUS3;
    volatile unsigned int BIU_STATUS4;
    volatile unsigned int BIU_STATUS5;
    volatile unsigned int BIU_STATUS6;
    volatile U_BIU_STATUS7 BIU_STATUS7;
    volatile U_BIU_STATUS8 BIU_STATUS8;
    volatile U_BIU_STATUS9 BIU_STATUS9;
    volatile unsigned int BIU_STATUS10;
    volatile unsigned int BIU_STATUS11;
    volatile unsigned int BRIF_DBG0_H;
    volatile unsigned int BRIF_DBG0_L;
    volatile unsigned int BRIF_DBG1_H;
    volatile unsigned int BRIF_DBG1_L;
    volatile unsigned int BRIF_DBG2_H;
    volatile unsigned int BRIF_DBG2_L;
    volatile unsigned int BRIF_DBG3_H;
    volatile unsigned int BRIF_DBG3_L;
    volatile unsigned int BWIF_DBG0_H;
    volatile unsigned int BWIF_DBG0_L;
    volatile unsigned int BWIF_DBG1_H;
    volatile unsigned int BWIF_DBG1_L;
    volatile unsigned int BWIF_DBG2_H;
    volatile unsigned int BWIF_DBG2_L;
    volatile unsigned int BWIF_DBG3_H;
    volatile unsigned int BWIF_DBG3_L;
    volatile U_BRIF_RETIRE_CNT BRIF_RETIRE_CNT;
    volatile U_BWIF_RETIRE_CNT BWIF_RETIRE_CNT;
    volatile U_SQE_RD_REQ_CNT SQE_RD_REQ_CNT;
    volatile U_SQE_RD_RSP_CNT SQE_RD_RSP_CNT;
    volatile U_CQE_WR_REQ_CNT CQE_WR_REQ_CNT;
    volatile U_CQE_WR_RSP_CNT CQE_WR_RSP_CNT;
    volatile U_BLK_RD_REQ_CNT BLK_RD_REQ_CNT;
    volatile U_BLK_WR_REQ_CNT BLK_WR_REQ_CNT;
    volatile U_DMA_INER_FSM DMA_INER_FSM;
    volatile U_DMA_INER_FIFO_0 DMA_INER_FIFO_0;
    volatile U_DMA_INER_FIFO_1 DMA_INER_FIFO_1;
    volatile U_WR_USE_CNT WR_USE_CNT;
} S_aic_sysdma_reg_REGS_TYPE;
extern volatile S_aic_sysdma_reg_REGS_TYPE *gopaic_sysdma_regAllReg;
int iSetSDMA_CTRLdbg_en(unsigned int udbg_en);
int iSetSDMA_CTRLdbg_mode(unsigned int udbg_mode);
int iSetSDMA_CTRLdbg_ch_sel(unsigned int udbg_ch_sel);
int iSetSDMA_CTRLdbg_w_pattern(unsigned int udbg_w_pattern);
int iSetSDMA_CTRLswitch_en(unsigned int uswitch_en);
int iSetSDMA_CTRLl2_r_err_clr(unsigned int ul2_r_err_clr);
int iSetSDMA_CTRLl2_w_err_clr(unsigned int ul2_w_err_clr);
int iSetDEBUG_SQE0ie(unsigned int uie);
int iSetDEBUG_SQE0mode(unsigned int umode);
int iSetDEBUG_SQE0status(unsigned int ustatus);
int iSetDEBUG_SQE0w_pattern(unsigned int uw_pattern);
int iSetDEBUG_SQE1s_sid(unsigned int us_sid);
int iSetDEBUG_SQE1s_subsid(unsigned int us_subsid);
int iSetDEBUG_SQE1d_sid(unsigned int ud_sid);
int iSetDEBUG_SQE1d_subsid(unsigned int ud_subsid);
int iSetDEBUG_SQE3length(unsigned int ulength);
int iSetDEBUG_SQE4s_addr_l(unsigned int us_addr_l);
int iSetDEBUG_SQE5s_addr_h(unsigned int us_addr_h);
int iSetDEBUG_SQE6d_addr_l(unsigned int ud_addr_l);
int iSetDEBUG_SQE7d_addr_h(unsigned int ud_addr_h);
int iSetDMA_TIMEOUT_THtimeout_th(unsigned int utimeout_th);
int iSetCH_CTRLch_en(unsigned int *upch_en);
int iSetCH_CTRLch_srst(unsigned int *upch_srst);
int iSetCH_CTRLqos(unsigned int *upqos);
int iSetCH_CTRLsq_size(unsigned int *upsq_size);
int iSetCH_CTRLcq_size(unsigned int *upcq_size);
int iSetCH_CTRLcqe_fmt_sel(unsigned int *upcqe_fmt_sel);
int iSetSQ_SIDsq_sid(unsigned int *upsq_sid);
int iSetSQ_SIDsq_sub_sid(unsigned int *upsq_sub_sid);
int iSetCQ_SIDcq_sid(unsigned int *upcq_sid);
int iSetCQ_SIDcq_sub_sid(unsigned int *upcq_sub_sid);
int iSetSQ_BASE_Hsq_base_h(unsigned int *upsq_base_h);
int iSetSQ_BASE_Lsq_base_l(unsigned int *upsq_base_l);
int iSetCQ_BASE_Hcq_base_h(unsigned int *upcq_base_h);
int iSetCQ_BASE_Lcq_base_l(unsigned int *upcq_base_l);
int iSetSQ_WPsq_wp(unsigned int *upsq_wp);
int iSetCQ_RPLcq_rp(unsigned int *upcq_rp);
int iSetL2_REMAP_VADDR_BASE_Ll2_vaddr_base_l(unsigned int ul2_vaddr_base_l);
int iSetL2_REMAP_VADDR_BASE_Hl2_vaddr_base_h(unsigned int ul2_vaddr_base_h);
int iSetL2_SIZEl2_size(unsigned int ul2_size);
int iSetBIU_CTRL0r_credit_en(unsigned int ur_credit_en);
int iSetBIU_CTRL0r_credit_en_dynam(unsigned int ur_credit_en_dynam);
int iSetBIU_CTRL0w_credit_en(unsigned int uw_credit_en);
int iSetBIU_CTRL0w_credit_en_dynam(unsigned int uw_credit_en_dynam);
int iSetBIU_CTRL0credit_mode_dynam(unsigned int ucredit_mode_dynam);
int iSetBIU_CTRL0rosd_cfg(unsigned int urosd_cfg);
int iSetBIU_CTRL0wosd_cfg(unsigned int uwosd_cfg);
int iSetBIU_CTRL0mon_en(unsigned int umon_en);
int iSetBIU_CTRL0mon_axi_sel(unsigned int umon_axi_sel);
int iSetBIU_CTRL0aif_clk_en(unsigned int uaif_clk_en);
int iSetBIU_CTRL0aif_clk_sel(unsigned int uaif_clk_sel);
int iSetBIU_CTRL1r_osd_th_dynam(unsigned int ur_osd_th_dynam);
int iSetBIU_CTRL1w_osd_th_dynam(unsigned int uw_osd_th_dynam);
int iSetBIU_CTRL2r_credit_step(unsigned int ur_credit_step);
int iSetBIU_CTRL2r_credit_lth(unsigned int ur_credit_lth);
int iSetBIU_CTRL2r_credit_uth(unsigned int ur_credit_uth);
int iSetBIU_CTRL3r_credit_step_dynam(unsigned int ur_credit_step_dynam);
int iSetBIU_CTRL3r_credit_lth_dynam(unsigned int ur_credit_lth_dynam);
int iSetBIU_CTRL3r_credit_uth_dynam(unsigned int ur_credit_uth_dynam);
int iSetBIU_CTRL4w_credit_step_dynam(unsigned int uw_credit_step_dynam);
int iSetBIU_CTRL4w_credit_lth_dynam(unsigned int uw_credit_lth_dynam);
int iSetBIU_CTRL4w_credit_uth_dynam(unsigned int uw_credit_uth_dynam);
int iSetBIU_CTRL5w_credit_step(unsigned int uw_credit_step);
int iSetBIU_CTRL5w_credit_lth(unsigned int uw_credit_lth);
int iSetBIU_CTRL5w_credit_uth(unsigned int uw_credit_uth);
#endif
