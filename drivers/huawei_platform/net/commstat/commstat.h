/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2016-2020. All rights reserved.
 * Description: CommStat head file.
 * Author: zhuweichen@huawei.com
 * Create: 2016-06-20
 */

#ifndef _COMMSTAT_H
#define _COMMSTAT_H

void inet_save_comm_stat(struct socket *sock, int tx, int len);
int comm_stat_init(void);

#endif /* _COMMSTAT_H */
