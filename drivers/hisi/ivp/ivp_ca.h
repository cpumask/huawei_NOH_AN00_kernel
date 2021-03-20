/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description:  this file define interfaces for hisi-ivp secure ca
 * Author:  donghaixu
 * Create:  2020-04-01
 */
#ifndef _IVP_CA_H_
#define _IVP_CA_H_

#include <linux/types.h>
#include "ivp.h"

int teek_secivp_ca_probe(void);
void teek_secivp_ca_remove(void);
int teek_secivp_sec_cfg(int sharefd, unsigned int size);
int teek_secivp_sec_uncfg(int sharefd, unsigned int size);
unsigned int teek_secivp_secmem_map(unsigned int sharefd, unsigned int size);
int teek_secivp_secmem_unmap(unsigned int sharefd, unsigned int size);
unsigned int teek_secivp_nonsecmem_map(int sharefd, unsigned int size,
	struct sglist *sgl);
int teek_secivp_nonsecmem_unmap(int sharefd, unsigned int size);
void teek_secivp_clear(void);

#endif /* _IVP_CA_H_ */
