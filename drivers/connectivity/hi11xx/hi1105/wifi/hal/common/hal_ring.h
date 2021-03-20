

#ifndef __HAL_RING_H__
#define __HAL_RING_H__


#include "oal_types.h"
#include "hal_device.h"


/* �ж�ring�Ƿ�Ϊ���������������TRUE;��֮������FALSE */
#define HAL_RING_IS_FULL(ring_ctl)  \
    (((ring_ctl)->un_write_ptr.st_write_ptr.bit_write_ptr == (ring_ctl)->un_read_ptr.st_read_ptr.bit_read_ptr) && \
    ((ring_ctl)->un_write_ptr.st_write_ptr.bit_wrap_flag != (ring_ctl)->un_read_ptr.st_read_ptr.bit_wrap_flag))

/* �ж�ring�Ƿ�Ϊ�գ�����գ�����TRUE;��֮������FALSE */
#define HAL_RING_IS_EMPTY(ring_ctl) \
    ((ring_ctl)->un_write_ptr.write_ptr == (ring_ctl)->un_read_ptr.read_ptr)

/* �ж�ring�Ƿ�ת�������ת������TRUE;��֮������FALSE */
#define HAL_RING_WRAP_AROUND(ring_ctl) \
    ((ring_ctl)->un_read_ptr.st_read_ptr.bit_wrap_flag != (ring_ctl)->un_write_ptr.st_write_ptr.bit_wrap_flag)


typedef enum {
    HAL_RING_TYPE_INVALID,
    /* Free ring: �������write ptr, HW����read ptr */
    HAL_RING_TYPE_FREE_RING,
    /* Complete ring: �������read ptr, HW����write ptr */
    HAL_RING_TYPE_COMPLETE_RING,

    HAL_RING_TYPE_BUTT
} hal_ring_type_enum;

extern uint32_t hal_ring_init(hal_host_ring_ctl_stru *ring_ctl);
extern void hal_ring_get_hw2sw(hal_host_ring_ctl_stru *ring_ctl);
extern uint32_t hal_ring_set_sw2hw(hal_host_ring_ctl_stru *ring_ctl);
extern uint32_t hal_ring_get_entry_count(hal_host_ring_ctl_stru *ring_ctl, uint16_t *p_count);
extern uint32_t hal_ring_get_entries(hal_host_ring_ctl_stru *ring_ctl,
    uint8_t *entries, uint16_t count);
extern uint32_t hal_ring_set_entries(hal_host_ring_ctl_stru *ring_ctl, uint64_t entry);
extern oal_void hmac_update_free_ring_wptr(hal_host_ring_ctl_stru *ring_ctl, uint16_t count);

#endif /* __HAL_RING_H__ */
