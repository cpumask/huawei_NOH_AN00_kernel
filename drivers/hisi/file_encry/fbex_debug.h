/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Hisi fbex ca header files
 * Author : security-ap
 * Create : 2020/01/02
 */

#ifndef __HISI_FBEX_DEBUG_H_
#define __HISI_FBEX_DEBUG_H_

#include <linux/types.h>
#include <linux/hisi/hisi_fbe_ctrl.h>

#define FBEX_MAGIC01  0xA5
#define FBEX_MAGIC02  0x5A
#define TEST_TIMEOUT  4000

#ifdef CONFIG_HISI_FILE_BASED_ENCRYPTO_DBG

int fbex_init_debugfs(void);
void fbex_cleanup_debugfs(void);
#else
static inline int fbex_init_debugfs(void) {return 0; }
static inline void fbex_cleanup_debugfs(void) {}
#endif
#endif /* __HISI_FBEX_DEBUG_H_ */
