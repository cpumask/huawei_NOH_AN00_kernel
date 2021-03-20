/*
 * pcie-slt.c
 *
 * PCIe SLT test functions
 *
 * Copyright (c) 2016-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifdef CONFIG_HISI_DEBUG_FS

#include "pcie-kirin-common.h"
#include "pcie-kirin-idle.h"

#include <linux/file.h>
#include <linux/module.h>
#include <linux/compat.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/pci-aspm.h>
#include <linux/fs.h>
#include <linux/mfd/hisi_pmic.h>
#include <asm/memory.h>
#include <hitest_slt.h>
#include <securec.h>

/* PCIe0<->PCIe1 mode, PCIe1 is RC and PCIe0 is EP */
#define APR_RC_ID 1
#define APR_EP_ID 0

#define SLT_RANDOM_DATA    0x12
#define SLT_TEST_DATA_SIZE 0xc0000

#define SIZE_M         0x400000
#define PCIE_SLT_NAME  "pcie-slt"
#define RC_NUMS        2
#ifdef CONFIG_KIRIN_PCIE_APR
#define RC_ID 1
#else
#define RC_ID 0
#endif
#define PCIE_SLT_DATA_DFT_LOOP_TIMES 2
#define PCIE_SLT_DATA_MAX_LOOP_TIMES 10000

enum aspm_stat {
	L0_MODE = 0,
	L0S_MODE = 1,
	L1_MODE = 2,
	L1_1_MODE = 3,
	L1_2_MODE = 4,
};

enum {
	H2D = 1,
	D2H = 2,
};

enum pcie_test_result {
	RESULT_OK = 0,
	ERR_DATA_TRANS,
	ERR_L0,
	ERR_L0S,
	ERR_L1,
	ERR_L0S_L1,
	ERR_L1_1,
	ERR_L1_2,
	ERR_EP_ON,
	ERR_EP_OFF,
	ERR_OTHER
};

enum pcie_voltage {
	NORMAL_VOL,
	LOW_VOL,
	HIGH_VOL,
};

#define PCIETESTCMD      _IOWR('p', 0xc1, unsigned long)
#define PCIETESTCMD_NEW  _IOWR('p', 0xc2, unsigned long)

struct pcie_ep_ops {
	int (*init)(void *data);
	int (*on)(void *data);
	int (*off)(void *data);
	int (*setup)(void *data);
	int (*data_transfer)(void *axi_addr, u32 data_size, u32 dir);
};

struct pcie_slt_cfg {
	u32 ldo5_offset;
	u32 ldo30_offset;
	u32 ldo5_normal;
	u32 ldo5_low;
	u32 ldo30_normal;
	u32 ldo30_low;
	struct pcie_ep_ops ep_ops;
};
struct pcie_slt_cfg g_pcie_slt_cfg[RC_NUMS];

struct pcie_slt {
	atomic_t ioctl_excl;
	atomic_t open_excl;
	int pcie_slt_major_number;
	struct class *pcie_slt_class;
};
struct pcie_slt g_pcie_slt;

struct kirin_pcie_scb_union {
	u32 rc_id;
	u32 loop_times;
	u32 test_result;
};

int pcie_slt_hook_register(u32 rc_id, u32 device_type, int (*init)(void *),
			   int (*on)(void *), int (*off)(void *),
			   int (*setup)(void *),
			   int (*data_transfer)(void *, u32, u32))
{
	if (rc_id > g_rc_num) {
		PCIE_PR_E("rc_id invalid");
		return -EINVAL;
	}

	if (!data_transfer || (g_kirin_pcie[rc_id].dtsinfo.ep_device_type != device_type)) {
		PCIE_PR_E("Hook func invalid");
		g_pcie_slt_cfg[rc_id].ep_ops.data_transfer = NULL;
		return -EINVAL;
	}

	g_pcie_slt_cfg[rc_id].ep_ops.init = init;
	g_pcie_slt_cfg[rc_id].ep_ops.on = on;
	g_pcie_slt_cfg[rc_id].ep_ops.off = off;
	g_pcie_slt_cfg[rc_id].ep_ops.setup = setup;
	g_pcie_slt_cfg[rc_id].ep_ops.data_transfer = data_transfer;

	return RESULT_OK;
}
EXPORT_SYMBOL(pcie_slt_hook_register);

static int32_t pcie_get_ldoinfo(struct kirin_pcie *pcie)
{
	struct device_node *np = NULL;
	u32 val[3]; /* bias volt: offset, normal_val, low_val */

	np = pcie->pci->dev->of_node;

	if (of_property_read_u32_array(np, "ldo5", val, 3)) {
		PCIE_PR_E("Failed to get ldo5 info");
		return -1;
	}
	g_pcie_slt_cfg[pcie->rc_id].ldo5_offset = val[0];
	g_pcie_slt_cfg[pcie->rc_id].ldo5_normal = val[1];
	g_pcie_slt_cfg[pcie->rc_id].ldo5_low = val[2];

	if (of_property_read_u32_array(np, "ldo30", val, 3)) {
		PCIE_PR_E("Failed to get ldo30 info");
		return -1;
	}
	g_pcie_slt_cfg[pcie->rc_id].ldo30_offset = val[0];
	g_pcie_slt_cfg[pcie->rc_id].ldo30_normal = val[1];
	g_pcie_slt_cfg[pcie->rc_id].ldo30_low = val[2];

	return 0;
}

static void pcie_set_vlotage(struct kirin_pcie *pcie, enum pcie_voltage vol)
{
	if (pcie_get_ldoinfo(pcie))
		return;

	switch (vol) {
	case LOW_VOL:
		/* low voltage LDO5:1.72, LDO30:0.725 */
		hisi_pmic_reg_write(g_pcie_slt_cfg[pcie->rc_id].ldo5_offset,
				    g_pcie_slt_cfg[pcie->rc_id].ldo5_low);
		hisi_pmic_reg_write(g_pcie_slt_cfg[pcie->rc_id].ldo30_offset,
				    g_pcie_slt_cfg[pcie->rc_id].ldo30_low);
		break;
	case NORMAL_VOL:
		/* fall-through */
	default:
		/* normal voltage LDO5:1.8, LDO30:0.75 */
		hisi_pmic_reg_write(g_pcie_slt_cfg[pcie->rc_id].ldo5_offset,
				    g_pcie_slt_cfg[pcie->rc_id].ldo5_normal);
		hisi_pmic_reg_write(g_pcie_slt_cfg[pcie->rc_id].ldo30_offset,
				    g_pcie_slt_cfg[pcie->rc_id].ldo30_normal);
	}
}

/*
 * wait_for_power_status - wait for link Entry lowpower mode
 * @mode: lowpower mode index
 */
static int wait_for_power_status(struct kirin_pcie *pcie, enum aspm_stat mode)
{
	u32 status4 = 0;
	u32 status5 = 0;
	unsigned long prev_jffy;

	prev_jffy = jiffies;
	while (!(time_after(jiffies, prev_jffy + HZ / 10))) { /* 100ms timer */
		status4 = kirin_elb_readl(pcie, SOC_PCIECTRL_STATE4_ADDR);
		status5 = kirin_elb_readl(pcie, SOC_PCIECTRL_STATE5_ADDR);
		switch (mode) {
		case L0_MODE:
			if ((status4 & LTSSM_STATUE_MASK) == LTSSM_L0)
				goto LTSSM_OK;
			break;
		case L0S_MODE:
			if ((status4 & LTSSM_STATUE_MASK) == LTSSM_L0S)
				goto LTSSM_OK;
			break;
		case L1_MODE:
			if ((status4 & LTSSM_STATUE_MASK) == LTSSM_L1)
				goto LTSSM_OK;
			break;
		case L1_1_MODE:
			if ((status5 & LTSSM_L1SS_MASK) == LTSSM_L1_1)
				goto LTSSM_OK;
			break;
		case L1_2_MODE:
			if ((status5 & LTSSM_L1SS_MASK) == LTSSM_L1_2)
				goto LTSSM_OK;
			break;
		default:
			PCIE_PR_E("unknown lowpower mode");
			break;
		}

		udelay(2);
	}

	PCIE_PR_E("PCIECTRL_STATE4: 0x%x, PCIECTRL_STATE5: 0x%x",
		    status4, status5);
	return mode;

LTSSM_OK:
	return 0;
}

static int loopback_poweroff(void *data)
{
	int ret;
	struct kirin_pcie *pcie_rc = &g_kirin_pcie[APR_RC_ID];
	struct kirin_pcie *pcie_ep = &g_kirin_pcie[APR_EP_ID];

	if (pcie_rc->phy_ops && pcie_rc->phy_ops->irq_disable)
		pcie_rc->phy_ops->irq_disable(pcie_rc->phy);

	if (ltssm_enable(pcie_rc->rc_id, DISABLE)) {
		PCIE_PR_E("ltssm_disable fail");
		return  ERR_OTHER;
	}

	ret = kirin_pcie_power_ctrl(pcie_ep, RC_POWER_OFF);
	if (ret) {
		PCIE_PR_E("Failed to power off EP");
		return ret;
	}

	ret = kirin_pcie_power_ctrl(pcie_rc, RC_POWER_OFF);
	if (ret) {
		PCIE_PR_E("Failed to power off RC");
		return ret;
	}

	return RESULT_OK;
}

static int loopback_setup(void *data)
{
	int ret;
	struct kirin_pcie *pcie_rc = &g_kirin_pcie[APR_RC_ID];
	struct kirin_pcie *pcie_ep = &g_kirin_pcie[APR_EP_ID];

	ret = kirin_pcie_set_ep_mode(pcie_ep->rc_id);
	if (ret) {
		PCIE_PR_E("Failed to set_ep_mode");
		return ret;
	}

	/* Power on RC firstly */
	ret = kirin_pcie_power_ctrl(pcie_rc, RC_POWER_ON);
	if (ret) {
		PCIE_PR_E("Failed to power RC");
		dsm_pcie_dump_info(pcie_rc, DSM_ERR_POWER_ON);
		return ret;
	}

	/* Power on EP since EP need RC's reference clock */
	ret = kirin_pcie_power_ctrl(pcie_ep, RC_POWER_ON);
	if (ret) {
		PCIE_PR_E("Failed to power EP");
		dsm_pcie_dump_info(pcie_ep, DSM_ERR_POWER_ON);
		return ret;
	}

	if (pcie_ep->phy_ops && pcie_ep->phy_ops->irq_disable)
		pcie_ep->phy_ops->irq_disable(pcie_ep->phy);

	/* EP MAC initialization */
	ret = kirin_pcie_ep_mac_init(pcie_ep->rc_id);
	if (ret) {
		PCIE_PR_E("Failed to initialize EP MAC");
		return ret;
	}

	if (IS_ENABLED(CONFIG_KIRIN_PCIE_MAY))
		(void)set_link_speed(pcie_rc->rc_id, GEN2);

	if (atomic_read(&pcie_rc->is_enumerated)) {
		if (kirin_pcie_pm_control(POWERON, pcie_rc->rc_id)) {
			PCIE_PR_E("kirin_pcie_pm_control fail");
			return  ERR_OTHER;
		}
	} else {
		if (kirin_pcie_enumerate(pcie_rc->rc_id)) {
			PCIE_PR_E("kirin_pcie_enumerate fail");
			return  ERR_OTHER;
		}
	}

	if (pci_enable_device(pcie_rc->ep_dev)) {
		PCIE_PR_E("Failed to enable APR EP");
		return ERR_OTHER;
	}

	pci_set_master(pcie_rc->ep_dev);

	return RESULT_OK;
}

static void pcie_data_trans(void *dst, u32 dst_size, void *src, u32 src_size)
{
	u32 i, val;
	u32 size = src_size < dst_size ? src_size : dst_size;

	for (i = 0; i < size; i += 0x4) {
		val = readl(src + i);
		writel(val, dst + i);
	}
}

#define BAR_INDEX0                  0
#define ARP_DATA_TRANSFER_MAX_SIZE  0x400000
#define APR_DEVICE_TARGET_ADDR      0x100000000
static int loopback_datatransfer(void *ddr, u32 size, u32 dir)
{
	void __iomem *wl_cpu_base = NULL;
	int result = 0;
	struct kirin_pcie *pcie_rc = &g_kirin_pcie[APR_RC_ID];
	struct kirin_pcie *pcie_ep = &g_kirin_pcie[APR_EP_ID];
	u64 axi_addr;

	/* Get EP BAR0 base address */
	axi_addr = pci_resource_start(pcie_rc->ep_dev, BAR_INDEX0);
	if (!axi_addr) {
		PCIE_PR_E("Failed to get APR device base_addr");
		return ERR_OTHER;
	}

	wl_cpu_base = ioremap_nocache(axi_addr, SIZE_M);
	if (!wl_cpu_base) {
		PCIE_PR_E("Failed to ioremap loop_back_src");
		result = ERR_OTHER;
		goto TEST_FAIL_UNMAP;
	}

	if (size > ARP_DATA_TRANSFER_MAX_SIZE)
		size = ARP_DATA_TRANSFER_MAX_SIZE;

	kirin_pcie_outbound_atu(pcie_rc->rc_id, KIRIN_PCIE_ATU_REGION_INDEX0,
				KIRIN_PCIE_ATU_TYPE_MEM, axi_addr, axi_addr,
				ARP_DATA_TRANSFER_MAX_SIZE);
	kirin_pcie_inbound_atu(pcie_ep->rc_id, KIRIN_PCIE_ATU_REGION_INDEX0,
			       KIRIN_PCIE_ATU_TYPE_MEM, APR_DEVICE_TARGET_ADDR,
			       axi_addr, ARP_DATA_TRANSFER_MAX_SIZE);

	/* Write to EP Ram */
	if (dir == H2D)
		pcie_data_trans((void *)(uintptr_t)wl_cpu_base, SIZE_M,
				  (void *)(uintptr_t)ddr, size);
	else if (dir == D2H)
		pcie_data_trans((void *)(uintptr_t)ddr, SIZE_M,
				  (void *)(uintptr_t)wl_cpu_base, size);
	else
		result = -EINVAL;

TEST_FAIL_UNMAP:
	iounmap(wl_cpu_base);
	return result;
}

#define DATA_ARR_SIZE 10
static int slt_data_transfer(struct kirin_pcie *pcie, void *slt_data_src,
			     void *slt_data_cmp, u32 loop_times)
{
	int index;
	u32 j;
	const u32 data_size[DATA_ARR_SIZE] = { 0x4, 0x8, 0x10, 0x100, 0x1000, 0x4000,
					       0xc000, 0x10000, 0x20000, 0x40000 };
	int ret0, ret1, ret2, ret3;

	if (!loop_times) {
		PCIE_PR_E("Data transfer loop times is 0, set default[1]");
		loop_times = 1;
	}

	if (!g_pcie_slt_cfg[pcie->rc_id].ep_ops.data_transfer) {
		PCIE_PR_E("maybe NullFuncPointer!");
		return -EINVAL;
	}

	for (j = 0; j < loop_times; j++) {
		for (index = 0; index < DATA_ARR_SIZE; index++) {
			ret0 = memset_s(slt_data_src, SLT_TEST_DATA_SIZE, SLT_RANDOM_DATA + index, SLT_TEST_DATA_SIZE);
			ret1 = g_pcie_slt_cfg[pcie->rc_id].ep_ops.data_transfer(slt_data_src, data_size[index], H2D);
			ret2 = g_pcie_slt_cfg[pcie->rc_id].ep_ops.data_transfer(slt_data_cmp, data_size[index], D2H);
			ret3 = memcmp(slt_data_src, slt_data_cmp, ret2);
			if (ret0 != EOK || ret1 != ret2 || ret3) {
				PCIE_PR_E("Data transfer failed[%u, %d], ret1: %d, ret2: %d, ret3: %d",
					    loop_times, index, ret1, ret2, ret3);
				return ERR_DATA_TRANS;
			}
			udelay(50);
		}
	}

	return 0;
}

static int pcie_slt_prepare(struct kirin_pcie *pcie)
{
	const u32 rc_idx = pcie->rc_id;

	if (g_pcie_slt_cfg[rc_idx].ep_ops.on) {
		if (g_pcie_slt_cfg[rc_idx].ep_ops.on(pcie)) {
			PCIE_PR_E("Device pwrup fail");
			return ERR_EP_ON;
		}
	}

	if (g_pcie_slt_cfg[rc_idx].ep_ops.setup) {
		if (g_pcie_slt_cfg[rc_idx].ep_ops.setup(pcie)) {
			PCIE_PR_E("Setup fail");
			return ERR_OTHER;
		}
	}

	if ((!atomic_read(&(pcie->is_power_on))) || (!g_pcie_slt_cfg[rc_idx].ep_ops.data_transfer)) {
		PCIE_PR_E("Not ready");
		return ERR_OTHER;
	}

#ifdef CONFIG_KIRIN_PCIE_L1SS_IDLE_SLEEP
	kirin_pcie_refclk_host_vote(&pcie->pci->pp, 1);
#endif
	return RESULT_OK;
}

static int pcie_slt_clear(struct kirin_pcie *pcie)
{
	if (g_pcie_slt_cfg[pcie->rc_id].ep_ops.off)
		g_pcie_slt_cfg[pcie->rc_id].ep_ops.off(pcie);

#ifdef CONFIG_KIRIN_PCIE_L1SS_IDLE_SLEEP
	kirin_pcie_refclk_host_vote(&pcie->pci->pp, 0);
#endif
	return RESULT_OK;
}

static enum pcie_test_result set_loopback_test(struct kirin_pcie *pcie, u32 loop_times)
{
	int ret;
	enum pcie_test_result result;
	void __iomem *loop_back_cmp = NULL;
	void __iomem *loop_back_src = NULL;

	loop_back_src = vmalloc(SLT_TEST_DATA_SIZE * 2);
	if (!loop_back_src) {
		PCIE_PR_E("Failed to alloc memory");
		return ERR_OTHER;
	}

	ret = memset_s(loop_back_src, SLT_TEST_DATA_SIZE * 2, 0, SLT_TEST_DATA_SIZE * 2);
	if (ret != EOK) {
		PCIE_PR_E("Failed to set default val[0x0]");
		result = ret;
		goto TEST_FREE_SRC;
	}

	loop_back_cmp = vmalloc(SLT_TEST_DATA_SIZE * 2);
	if (!loop_back_cmp) {
		PCIE_PR_E("Failed to alloc memory");
		result = ERR_OTHER;
		goto TEST_FREE_SRC;
	}

	ret = memset_s(loop_back_cmp, SLT_TEST_DATA_SIZE * 2, 0, SLT_TEST_DATA_SIZE * 2);
	if (ret != EOK) {
		PCIE_PR_E("Failed to set default val[0x0]");
		result = ret;
		goto TEST_FAIL_CMP;
	}

	kirin_pcie_config_l1ss(pcie->rc_id, L1SS_CLOSE);
	kirin_pcie_config_l0sl1(pcie->rc_id, ASPM_CLOSE);
	if (wait_for_power_status(pcie, L0_MODE)) {
		PCIE_PR_E("Enter L0 failed");
		result = ERR_L0;
		goto TEST_FAIL_CMP;
	}
	PCIE_PR_I("Enter L0 successful");

	ret = slt_data_transfer(pcie, loop_back_src, loop_back_cmp, loop_times);
	if (ret) {
		PCIE_PR_E("Data Transfer failed[PM_ASPM_CLOSE]");
		result = ERR_DATA_TRANS;
		goto TEST_FAIL_CMP;
	}
	PCIE_PR_I("Data Transfer successful[PM_ASPM_CLOSE]");

	kirin_pcie_config_l0sl1(pcie->rc_id, ASPM_L0S);
	if (wait_for_power_status(pcie, L0S_MODE)) {
		PCIE_PR_E("Enter L0s failed");
		result = ERR_L0S;
		goto TEST_FAIL_CMP;
	}
	PCIE_PR_I("Enter L0s successful");

	kirin_pcie_config_l0sl1(pcie->rc_id, ASPM_L1);
	if (wait_for_power_status(pcie, L1_MODE)) {
		PCIE_PR_E("Enter L1 failed");
		result = ERR_L1;
		goto TEST_FAIL_CMP;
	}
	PCIE_PR_I("Enter L1 successful");

	kirin_pcie_config_l0sl1(pcie->rc_id, ASPM_L0S_L1);
	if (wait_for_power_status(pcie, L1_MODE)) {
		PCIE_PR_E("Enter L0s&L1 failed");
		result = ERR_L0S_L1;
		goto TEST_FAIL_CMP;
	}
	PCIE_PR_I("Enter L0s&L1 successful");

	kirin_pcie_config_l0sl1(pcie->rc_id, ASPM_L1);
	kirin_pcie_config_l1ss(pcie->rc_id, L1SS_ASPM_1_1);
	if (wait_for_power_status(pcie, L1_1_MODE)) {
		PCIE_PR_E("Enter L1_1 failed");
		result = ERR_L1_1;
		goto TEST_FAIL_CMP;
	}
	PCIE_PR_I("Enter L1_1 successful");

	kirin_pcie_config_l0sl1(pcie->rc_id, ASPM_L1);
	kirin_pcie_config_l1ss(pcie->rc_id, L1SS_PM_ASPM_ALL);
	if (wait_for_power_status(pcie, L1_2_MODE)) {
		PCIE_PR_E("Enter L1_2 failed");
		result = ERR_L1_2;
		goto TEST_FAIL_CMP;
	}
	PCIE_PR_I("Enter L1_2 successful");

	ret = slt_data_transfer(pcie, loop_back_src, loop_back_cmp, loop_times);
	if (ret) {
		PCIE_PR_E("Data Transfer failed[PM_ASPM_ALL]");
		result = ERR_DATA_TRANS;
		goto TEST_FAIL_CMP;
	}
	PCIE_PR_I("Data Transfer successful[PM_ASPM_ALL]");

	result = RESULT_OK;

TEST_FAIL_CMP:
	vfree(loop_back_cmp);
TEST_FREE_SRC:
	vfree(loop_back_src);
	return result;
}

static enum pcie_test_result pcie_slt_vary_voltage_test(struct kirin_pcie *pcie, u32 loop_times)
{
	enum pcie_test_result ret = RESULT_OK;

	PCIE_PR_I("+%s+", __func__);

	if (pcie->dtsinfo.ep_device_type == EP_DEVICE_BCM ||
	    pcie->dtsinfo.ep_device_type == EP_DEVICE_NODEV ||
	    pcie->dtsinfo.ep_device_type == EP_DEVICE_MODEM) {
		PCIE_PR_E("Bypass, ep_device_type: %u", pcie->dtsinfo.ep_device_type);
		return ret;
	}

	if (pcie_slt_prepare(pcie))
		return ERR_OTHER;

	PCIE_PR_I("Test under default voltage");
	ret = set_loopback_test(pcie, loop_times);
	if (ret) {
		PCIE_PR_E("Default voltage pcie slt test fail");
		goto SLT_REALEASE;
	}

	PCIE_PR_I("Test under lower voltage");
	pcie_set_vlotage(pcie, LOW_VOL);
	ret = set_loopback_test(pcie, loop_times);
	if (ret) {
		PCIE_PR_E("Low voltage pcie slt test fail");
		pcie_set_vlotage(pcie, NORMAL_VOL);
		goto SLT_REALEASE;
	}

	PCIE_PR_I("Test under normal voltage");
	pcie_set_vlotage(pcie, NORMAL_VOL);
	ret = set_loopback_test(pcie, loop_times);
	if (ret) {
		PCIE_PR_E("Normal voltage pcie slt test fail");
		goto SLT_REALEASE;
	}

SLT_REALEASE:
	(void)pcie_slt_clear(pcie);

	PCIE_PR_I("-%s-", __func__);
	return ret;
}

static int pcie_slt_lock(atomic_t *excl)
{
	if (atomic_inc_return(excl) == 1)
		return 0;

	atomic_dec(excl);
	return -1;
}

static inline void pcie_slt_unlock(atomic_t *excl)
{
	atomic_dec(excl);
}

static void pcie_slt_resource_init(struct kirin_pcie *pcie)
{
	if (pcie->dtsinfo.ep_device_type == EP_DEVICE_APR) {
		PCIE_PR_I("PCIe0<->PCIe1 loopback mode, Register callback func");
		if (pcie_slt_hook_register(APR_RC_ID, EP_DEVICE_APR, NULL, NULL,
					loopback_poweroff, loopback_setup,
					loopback_datatransfer)) {
			PCIE_PR_E("Failed to register callback func");
			return;
		}
	}

	if (g_pcie_slt_cfg[pcie->rc_id].ep_ops.init)
		if (g_pcie_slt_cfg[pcie->rc_id].ep_ops.init(pcie))
			PCIE_PR_E("Init Device resouce Fail");
}

static long pcie_slt_ioctl(struct file *file, u_int cmd, unsigned long result)
{
	int ret;
	struct kirin_pcie_scb_union scb;

	ret = memset_s((void *)&scb, sizeof(struct kirin_pcie_scb_union), 0, sizeof(struct kirin_pcie_scb_union));
	if (ret != EOK)
		return (long)ret;

	if (pcie_slt_lock(&(g_pcie_slt.ioctl_excl)))
		return -EBUSY;

	switch (cmd) {
	case PCIETESTCMD:
		scb.rc_id = RC_ID;
		scb.loop_times = PCIE_SLT_DATA_DFT_LOOP_TIMES;
		scb.test_result = pcie_slt_vary_voltage_test(&g_kirin_pcie[scb.rc_id], scb.loop_times);
		ret = copy_to_user((void __user *)(uintptr_t)result,
				   (const void *)&(scb.test_result),
				   sizeof(unsigned int));
		break;

	case PCIETESTCMD_NEW:
		ret = copy_from_user((void *)&scb, (void __user *)(uintptr_t)result,
				     sizeof(struct kirin_pcie_scb_union));
		if (ret)
			goto FAIL;

		if (scb.rc_id >= g_rc_num || scb.loop_times <= 0 || scb.loop_times > PCIE_SLT_DATA_MAX_LOOP_TIMES) {
			PCIE_PR_E("InputPara is invalid, rc_id[%u], loop_times[%u]", scb.rc_id, scb.loop_times);
			ret = -1;
			goto FAIL;
		}

		scb.test_result = pcie_slt_vary_voltage_test(&g_kirin_pcie[scb.rc_id], scb.loop_times);
		ret = copy_to_user((void __user *)(uintptr_t)result,
				   (const void *)&scb,
				   sizeof(struct kirin_pcie_scb_union));
		break;

	default:
		ret = -1;
		break;
	}
FAIL:
	pcie_slt_unlock(&(g_pcie_slt.ioctl_excl));
	return (long)ret;
}

static int pcie_slt_open(struct inode *ip, struct file *fp)
{
	PCIE_PR_I("start");

	if (pcie_slt_lock(&(g_pcie_slt.open_excl)))
		return -EBUSY;

	PCIE_PR_I("success");
	return 0;
}

static int pcie_slt_release(struct inode *ip, struct file *fp)
{
	PCIE_PR_I("pcie_slt_release");

	pcie_slt_unlock(&(g_pcie_slt.open_excl));

	return 0;
}

static const struct file_operations pcie_slt_fops = {
	.unlocked_ioctl = pcie_slt_ioctl,
	.open           = pcie_slt_open,
	.release        = pcie_slt_release,
};

static int __init pcie_slt_init(void)
{
	int error = 0;
	struct device *pdevice = NULL;
	unsigned int i;

	if (!is_running_kernel_slt() && !runmode_is_factory())
		return 0;

	for (i = 0; i < g_rc_num; i++)
		pcie_slt_resource_init(&g_kirin_pcie[i]);

	/* semaphore initial */
	g_pcie_slt.pcie_slt_major_number = register_chrdev(0, PCIE_SLT_NAME, &pcie_slt_fops);
	if (g_pcie_slt.pcie_slt_major_number < 0) {
		PCIE_PR_E("register_chrdev error: %d", g_pcie_slt.pcie_slt_major_number);
		error = -EAGAIN;
		goto failed_register_pcie;
	}
	atomic_set(&g_pcie_slt.open_excl, 0);
	atomic_set(&g_pcie_slt.ioctl_excl, 0);

	g_pcie_slt.pcie_slt_class = class_create(THIS_MODULE, PCIE_SLT_NAME);
	if (IS_ERR(g_pcie_slt.pcie_slt_class)) {
		unregister_chrdev(g_pcie_slt.pcie_slt_major_number, PCIE_SLT_NAME);
		g_pcie_slt.pcie_slt_major_number = 0;
		error = PTR_ERR(g_pcie_slt.pcie_slt_class);
		PCIE_PR_E("class_create error");
		goto failed_register_pcie;
	}

	pdevice = device_create(g_pcie_slt.pcie_slt_class, NULL,
				MKDEV(g_pcie_slt.pcie_slt_major_number, 0),
				NULL, PCIE_SLT_NAME);
	if (IS_ERR(pdevice)) {
		class_destroy(g_pcie_slt.pcie_slt_class);
		unregister_chrdev(g_pcie_slt.pcie_slt_major_number, PCIE_SLT_NAME);
		g_pcie_slt.pcie_slt_class = NULL;
		g_pcie_slt.pcie_slt_major_number = 0;
		error = -EFAULT;
		PCIE_PR_E("device_create error");
		goto failed_register_pcie;
	}

	PCIE_PR_I("pcie-slt init ok!");

	return 0;

failed_register_pcie:
	return error;
}
static void __exit pcie_slt_cleanup(void)
{
	device_destroy(g_pcie_slt.pcie_slt_class, MKDEV(g_pcie_slt.pcie_slt_major_number, 0));
	class_destroy(g_pcie_slt.pcie_slt_class);
	unregister_chrdev(g_pcie_slt.pcie_slt_major_number, PCIE_SLT_NAME);
}
module_init(pcie_slt_init);
module_exit(pcie_slt_cleanup);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Hisilicon Kirin pcie driver");
MODULE_AUTHOR("Kai Liu <liukai833@huawei.com>");
#endif /* CONFIG_HISI_DEBUG_FS */
