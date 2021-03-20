// *********************************************************
// Copyright    Copyright (c) 2017- Hisilicon Technologies CO., Ltd.
// File name    cfg_table_mc.h
// Description:
//
// Version      1.0
// Date         2019-05-21
// *********************************************************

#ifndef __CFG_TABLE_MC_CS_H__
#define __CFG_TABLE_MC_CS_H__

#define KPT_NUM_MAX  3200
#define SVD_ITERATIONS_MIN 2
#define SVD_ITERATIONS_MAX 30
#define H_ITERATIONS_MIN   1
#define H_ITERATIONS_MAX   4095
#define CFG_MODE_INDEX_PAIRS 0
#define CFG_MODE_COOR_PAIRS  1
#define MC_H_MATRIX_NUM   18
#define MC_COORDINATE_PAIRS   (3200*2)
#define CMP_INDEX_PAIRS   (3200)

struct mc_ctrl_cfg_t {
	unsigned int       to_use;

	unsigned int svd_iterations;
	unsigned int max_iterations;
	unsigned int flag_10_11;
	unsigned int bf_mode;
	unsigned int push_inliers_en;
	unsigned int h_cal_en;
	unsigned int cfg_mode;
	unsigned int mc_en;
};

struct mc_inlier_thld_t {
	unsigned int       to_use;

	unsigned int       inlier_th;
};

struct mc_kpts_num_t {
	unsigned int       to_use;

	unsigned int matched_kpts;
};

struct mc_prefech_t {
	unsigned int       to_use;

	unsigned int ref_prefetch_enable;
	unsigned int ref_first_32k_page;
	unsigned int cur_prefetch_enable;
	unsigned int cur_first_32k_page;
};

struct mc_index_pairs_t {
	unsigned int       to_use;

	unsigned int cfg_best_dist[KPT_NUM_MAX];
	unsigned int cfg_ref_index[KPT_NUM_MAX];
	unsigned int cfg_cur_index[KPT_NUM_MAX];
};

struct mc_coord_pairs_t {
	unsigned int       to_use;

	unsigned int cur_cfg_coordinate_y[KPT_NUM_MAX];
	unsigned int cur_cfg_coordinate_x[KPT_NUM_MAX];
	unsigned int ref_cfg_coordinate_y[KPT_NUM_MAX];
	unsigned int ref_cfg_coordinate_x[KPT_NUM_MAX];
};

struct cfg_tab_mc_t {
	unsigned int to_use;

	struct mc_ctrl_cfg_t      mc_ctrl_cfg;
	struct mc_inlier_thld_t  mc_inlier_thld_cfg;
	struct mc_kpts_num_t   mc_kpt_num_cfg;
	struct mc_prefech_t       mc_prefech_cfg;
	struct mc_index_pairs_t mc_index_pairs_cfg;
	struct mc_coord_pairs_t mc_coord_pairs_cfg;

	unsigned int           mc_cascade_en;
	unsigned int           mc_match_points_addr; // address in cmdlst_buffer
	unsigned int           mc_index_addr; // address in cmdlst_buffer
};

#endif/*__CFG_TABLE_MC_CS_H__*/

