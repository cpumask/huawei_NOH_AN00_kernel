/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Memory protection library - shared configuration header
 * Author: Igor Stoppa <igor.stoppa@huawei.com>
 * Creator: security-ap
 * Date: 2020/04/15
 */

#ifndef _LINUX_PRMEM_CFG_H
#define _LINUX_PRMEM_CFG_H

#ifdef CONFIG_HISI_PRMEM

extern bool prmem_bypass_protection; /* DO NOT ACCESS DIRECTLY */

/*
 * prmem_bypass: tells if prmem is in bypass mode
 *
 * Returns true if prmem is in bypass mode, false otherwise
 *
 * Since prmem_bypass_protection can be configured at boot time, it cannot
 * be a const, so the inlined getter prevents bugs attempting to change
 * its value, which the compiler could not detect.
 */
static inline bool prmem_bypass(void)
{
	return prmem_bypass_protection;
}


/*
 * prmem_enabled - can be used at runtime to assess prmem being enabled
 *
 * Returns true if prmem is enabled (even if it might be in bypass mode)
 */
static inline bool prmem_enabled(void)
{
	return true;
}

#else
static inline bool prmem_bypass(void)
{
	return false;
}

static inline bool prmem_enabled(void)
{
	return false;
}
#endif

/*
 * prmem_active - tells if data will be really write protected
 *
 * Returns true when HW-backed write protection is available
 */
static inline bool prmem_active(void)
{
	return prmem_enabled() && !prmem_bypass();
}

#endif
