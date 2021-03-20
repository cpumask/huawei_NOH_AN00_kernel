/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Kirin partition table header
 * Author: weijiafu
 * Create: 2020-03-27
 */

#ifndef _PARTITION_H_
#define _PARTITION_H_

#include "hisi_partition.h"
#include "partition_def.h"

#if defined(CONFIG_HISI_PARTITION_LIBRA)
#ifdef CONFIG_HISI_PARTITION_KEYWEST
#include "libra_partition.h"
#else
#include "libra_partition_legacy.h"
#endif
#elif defined(CONFIG_HISI_PARTITION_LIBRA_CW)
#include "libra_cw_partition.h"
#elif defined(CONFIG_HISI_PARTITION_CANCER)
#ifdef HISI_EXTERNAL_MODEM
#include "cancer_a_plus_b_partition.h"
#else
#include "cancer_partition.h"
#endif
#elif defined(CONFIG_HISI_PARTITION_CAPRICORN)
#include "capricorn_partition.h"
#elif defined(CONFIG_HISI_PARTITION_PISCES)
#include "pisces_partition.h"
#elif defined(CONFIG_HISI_PARTITION_SCORPIO)
#include "scorpio_partition.h"
#elif defined(CONFIG_HISI_PARTITION_LEO)
#include "leo_partition.h"
#elif defined(CONFIG_HISI_PARTITION_AQUARIUS)
#include "aquarius_partition.h"
#elif defined(CONFIG_HISI_PARTITION_VIRGO)
#include "virgo_partition.h"
#elif defined(CONFIG_HISI_PARTITION_SAGITTARIUS)
#include "sagittarius_partition.h"
#elif defined(CONFIG_HISI_PARTITION_TAURUS)
#ifdef HISI_EXTERNAL_MODEM
#include "taurus_a_plus_b_partition.h"
#elif defined CONFIG_HISI_PARTITION_CS2
#include "taurus_cs2_partition.h"
#elif defined CONFIG_PRODUCT_ARMPC
#include "taurus_armpc_partition.h"
#else
#include "taurus_partition.h"
#endif
#endif

#endif
