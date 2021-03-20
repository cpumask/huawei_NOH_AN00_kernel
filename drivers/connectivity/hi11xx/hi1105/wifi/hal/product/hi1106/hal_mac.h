

#ifndef __HAL_MAC_H__
#define __HAL_MAC_H__

#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "hal_common.h"
#include "hal_device.h"

#define HI1106_MAC_CFG_HOST_REG_BASE                        (0x40020000)
#define HI1106_MAC_HOST_INTR_CLR_REG                        (HI1106_MAC_CFG_HOST_REG_BASE + 0x50)
#define HI1106_MAC_HOST_INTR_MASK_REG                       (HI1106_MAC_CFG_HOST_REG_BASE + 0x54)
#define HI1106_MAC_RPT_HOST_REG_BASE                        (0x40030000)
#define HI1106_DFX_REG_BANK_BASE                            (0x40064000)
#define HI1106_DFX_REG_BANK_PHY_INTR_RPT_REG                (HI1106_DFX_REG_BANK_BASE + 0xBC)     /* PHYÖÐ¶Ï×´Ì¬ */
#define HI1106_DFX_REG_BANK_CFG_INTR_MASK_HOST_REG          (HI1106_DFX_REG_BANK_BASE + 0xB0)
#define HI1106_DFX_REG_BANK_CFG_INTR_CLR_REG                (HI1106_DFX_REG_BANK_BASE + 0xB8)

#define HI1106_MAC_HOST_INTR_STATUS_REG                     (HI1106_MAC_RPT_HOST_REG_BASE + 0x0)  /* HostµÄÖÐ¶Ï×´Ì¬ */
#define HI1106_MAC_BA_INFO_BUF_WPTR_REG                     (HI1106_MAC_RPT_HOST_REG_BASE + 0x4)  /* BA INFO BUFFERµÄÐ´Ö¸ÕëÉÏ±¨ */
#define HI1106_MAC_RX_NORM_DATA_FREE_RING_RPTR_REG          (HI1106_MAC_RPT_HOST_REG_BASE + 0x8)  /* rx-ndata-frrptr */
#define HI1106_MAC_RX_SMALL_DATA_FREE_RING_RPTR_REG         (HI1106_MAC_RPT_HOST_REG_BASE + 0xC)  /* rx-sdata-frrptr */
#define HI1106_MAC_RX_DATA_CMP_RING_WPTR_REG                (HI1106_MAC_RPT_HOST_REG_BASE + 0x10)
#define HI1106_MAC_RPT_HOST_FREE_RING_STATUS_REG            (HI1106_MAC_RPT_HOST_REG_BASE + 0x14) /* F RINGÉÏ±¨ÐÅÏ¢ */
#define HI1106_MAC_RX_PPDU_HOST_DESC_FREE_RING_RPTR_REG     (HI1106_MAC_RPT_HOST_REG_BASE + 0x18)
#define HI1106_MAC_RPT_PPDU_HOST_FREE_RING_STATUS_REG       (HI1106_MAC_RPT_HOST_REG_BASE + 0x1C)

#define HI1106_MAC_TX_BA_INFO_BUF_DEPTH_REG                 (HI1106_MAC_CFG_HOST_REG_BASE + 0x0)  /* TX BA INFO BUFFERÉî¶ÈÅäÖÃ¼Ä´æÆ÷ */
#define HI1106_MAC_TX_BA_INFO_BUF_ADDR_LSB_REG              (HI1106_MAC_CFG_HOST_REG_BASE + 0x4)  /* TX BA INFO BUFFER»ùµØÖ·ÅäÖÃ¼Ä´æÆ÷ */
#define HI1106_MAC_TX_BA_INFO_BUF_ADDR_MSB_REG              (HI1106_MAC_CFG_HOST_REG_BASE + 0x8)  /* TX BA INFO BUFFER»ùµØÖ·ÅäÖÃ¼Ä´æÆ÷ */
#define HI1106_MAC_TX_BA_INFO_WPTR_REG                      (HI1106_MAC_CFG_HOST_REG_BASE + 0xC)  /* TX BA INFO WPTR BUFFER»ùµØÖ·ÅäÖÃ¼Ä´æÆ÷ */
#define HI1106_MAC_TX_BA_INFO_RPTR_REG                      (HI1106_MAC_CFG_HOST_REG_BASE + 0x10) /* TX BA INFO RPTR BUFFER»ùµØÖ·ÅäÖÃ¼Ä´æÆ÷ */
#define HI1106_MAC_RX_DATA_BUFF_LEN_REG                     (HI1106_MAC_CFG_HOST_REG_BASE + 0x1C)
#define HI1106_MAC_RX_NORM_DATA_FREE_RING_ADDR_LSB_REG      (HI1106_MAC_CFG_HOST_REG_BASE + 0x20)
#define HI1106_MAC_RX_NORM_DATA_FREE_RING_ADDR_MSB_REG      (HI1106_MAC_CFG_HOST_REG_BASE + 0x24)
#define HI1106_MAC_RX_SMALL_DATA_FREE_RING_ADDR_LSB_REG     (HI1106_MAC_CFG_HOST_REG_BASE + 0x28)
#define HI1106_MAC_RX_SMALL_DATA_FREE_RING_ADDR_MSB_REG     (HI1106_MAC_CFG_HOST_REG_BASE + 0x2C)
#define HI1106_MAC_RX_DATA_CMP_RING_ADDR_LSB_REG            (HI1106_MAC_CFG_HOST_REG_BASE + 0x30)
#define HI1106_MAC_RX_DATA_CMP_RING_ADDR_MSB_REG            (HI1106_MAC_CFG_HOST_REG_BASE + 0x34)
#define HI1106_MAC_RX_NORM_DATA_FREE_RING_SIZE_REG          (HI1106_MAC_CFG_HOST_REG_BASE + 0x38)
#define HI1106_MAC_RX_SMALL_DATA_FREE_RING_SIZE_REG         (HI1106_MAC_CFG_HOST_REG_BASE + 0x3C)
#define HI1106_MAC_RX_DATA_CMP_RING_SIZE_REG                (HI1106_MAC_CFG_HOST_REG_BASE + 0x40)
#define HI1106_MAC_RX_NORM_DATA_FREE_RING_WPTR_REG          (HI1106_MAC_CFG_HOST_REG_BASE + 0x44)
#define HI1106_MAC_RX_SMALL_DATA_FREE_RING_WPTR_REG         (HI1106_MAC_CFG_HOST_REG_BASE + 0x48)
#define HI1106_MAC_RX_DATA_CMP_RING_RPTR_REG                (HI1106_MAC_CFG_HOST_REG_BASE + 0x4C)
#define HI1106_MAC_HOST_INTR_CLR_REG                        (HI1106_MAC_CFG_HOST_REG_BASE + 0x50) /* HostÖÐ¶ÏÇå³ý */
#define HI1106_MAC_HOST_INTR_MASK_REG                       (HI1106_MAC_CFG_HOST_REG_BASE + 0x54) /* HostÖÐ¶ÏÆÁ±Î */
#define HI1106_MAC_RX_DATA_CMP_RING_WPTR_CFG_REG            (HI1106_MAC_CFG_HOST_REG_BASE + 0x58)
#define HI1106_MAC_RX_NORM_DATA_FREE_RING_RPTR_CFG_REG      (HI1106_MAC_CFG_HOST_REG_BASE + 0x5C)
#define HI1106_MAC_RX_SMALL_DATA_FREE_RING_RPTR_CFG_REG     (HI1106_MAC_CFG_HOST_REG_BASE + 0x60)


#define HI1106_MAC_RPT_HOST_INTR_MSDU_TABLE_RPTR_MISMATCH_LEN      1
#define HI1106_MAC_RPT_HOST_INTR_MSDU_TABLE_RPTR_MISMATCH_OFFSET   12
#define HI1106_MAC_RPT_HOST_INTR_MSDU_TABLE_RPTR_MISMATCH_MASK     0x1000
#define HI1106_MAC_RPT_HOST_INTR_LOCATION_INTR_FIFO_OVERRUN_LEN    1
#define HI1106_MAC_RPT_HOST_INTR_LOCATION_INTR_FIFO_OVERRUN_OFFSET 11
#define HI1106_MAC_RPT_HOST_INTR_LOCATION_INTR_FIFO_OVERRUN_MASK   0x800
#define HI1106_MAC_RPT_HOST_INTR_RX_PPDU_DESC_LEN                  1
#define HI1106_MAC_RPT_HOST_INTR_RX_PPDU_DESC_OFFSET               10
#define HI1106_MAC_RPT_HOST_INTR_RX_PPDU_DESC_MASK                 0x400
#define HI1106_MAC_RPT_HOST_INTR_BA_WIN_EXCEED_LEN                 1
#define HI1106_MAC_RPT_HOST_INTR_BA_WIN_EXCEED_OFFSET              9
#define HI1106_MAC_RPT_HOST_INTR_BA_WIN_EXCEED_MASK                0x200
#define HI1106_MAC_RPT_HOST_INTR_BA_INFO_RING_OVERRUN_LEN          1
#define HI1106_MAC_RPT_HOST_INTR_BA_INFO_RING_OVERRUN_OFFSET       8
#define HI1106_MAC_RPT_HOST_INTR_BA_INFO_RING_OVERRUN_MASK         0x100
#define HI1106_MAC_RPT_HOST_INTR_MSDU_TABLE_RPTR_OVERTAKE_LEN      1
#define HI1106_MAC_RPT_HOST_INTR_MSDU_TABLE_RPTR_OVERTAKE_OFFSET   7
#define HI1106_MAC_RPT_HOST_INTR_MSDU_TABLE_RPTR_OVERTAKE_MASK     0x80
#define HI1106_MAC_RPT_HOST_INTR_LOCATION_COMPLETE_LEN             1
#define HI1106_MAC_RPT_HOST_INTR_LOCATION_COMPLETE_OFFSET          6
#define HI1106_MAC_RPT_HOST_INTR_LOCATION_COMPLETE_MASK            0x40
#define HI1106_MAC_RPT_HOST_INTR_DATA_RING_OVERRUN_LEN             1
#define HI1106_MAC_RPT_HOST_INTR_DATA_RING_OVERRUN_OFFSET          5
#define HI1106_MAC_RPT_HOST_INTR_DATA_RING_OVERRUN_MASK            0x20
#define HI1106_MAC_RPT_HOST_INTR_SMALL_RING_EMPTY_LEN              1
#define HI1106_MAC_RPT_HOST_INTR_SMALL_RING_EMPTY_OFFSET           4
#define HI1106_MAC_RPT_HOST_INTR_SMALL_RING_EMPTY_MASK             0x10
#define HI1106_MAC_RPT_HOST_INTR_NORM_RING_EMPTY_LEN               1
#define HI1106_MAC_RPT_HOST_INTR_NORM_RING_EMPTY_OFFSET            3
#define HI1106_MAC_RPT_HOST_INTR_NORM_RING_EMPTY_MASK              0x8
#define HI1106_MAC_RPT_HOST_INTR_RX_COMPLETE_LEN                   1
#define HI1106_MAC_RPT_HOST_INTR_RX_COMPLETE_OFFSET                2
#define HI1106_MAC_RPT_HOST_INTR_RX_COMPLETE_MASK                  0x4
#define HI1106_MAC_RPT_HOST_INTR_BA_INFO_LEN                       1
#define HI1106_MAC_RPT_HOST_INTR_BA_INFO_OFFSET                    1
#define HI1106_MAC_RPT_HOST_INTR_BA_INFO_MASK                      0x2
#define HI1106_MAC_RPT_HOST_INTR_TX_COMPLETE_LEN                   1
#define HI1106_MAC_RPT_HOST_INTR_TX_COMPLETE_OFFSET                0
#define HI1106_MAC_RPT_HOST_INTR_TX_COMPLETE_MASK                  0x1


oal_void  hi1106_host_mac_irq_mask(hal_host_device_stru *hal_device, uint32_t irq);
oal_void  hi1106_host_mac_irq_unmask(hal_host_device_stru *hal_device, uint32_t irq);
oal_void hi1106_clear_host_mac_int_status(hal_host_device_stru *hal_device, uint32_t status);
oal_void hi1106_get_host_mac_int_mask(hal_host_device_stru *hal_device, uint32_t *p_mask);
oal_void hi1106_get_host_mac_int_status(hal_host_device_stru *hal_device, uint32_t *p_status);

oal_void hi1106_clear_host_phy_int_status(hal_host_device_stru *hal_device, uint32_t status);
oal_void hi1106_get_host_phy_int_mask(hal_host_device_stru *hal_device, uint32_t *p_mask);
oal_void hi1106_get_host_phy_int_status(hal_host_device_stru *hal_device, uint32_t *p_status);
int32_t hi1106_host_regs_addr_init(hal_host_device_stru *hal_device);

#endif

