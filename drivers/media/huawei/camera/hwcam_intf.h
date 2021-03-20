/* 
 * Hisilicon K3 SOC camera driver source file
 *
 * Copyright (C) Huawei Technology Co., Ltd.
 *
 * Author:
 * Email:
 * Date:	  2013-11-16
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef __HW_ALAN_KERNEL_CAMERA_OBJ_MDL_INTERFACE__
#define __HW_ALAN_KERNEL_CAMERA_OBJ_MDL_INTERFACE__

#include <linux/dma-buf.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/videodev2.h>
#include <media/huawei/camera.h>
#include <media/v4l2-subdev.h>
#include <media/videobuf2-core.h>

#define HWCAM_CFG_ERR(fmt, arg...) \
    printk(KERN_ERR "%s(%d, %d): " fmt "\n", \
           __func__, __LINE__, current->pid, ## arg)

#define HWCAM_CFG_INFO(fmt, arg...) \
    printk(KERN_INFO "%s(%d, %d): " fmt "\n", \
           __func__, __LINE__, current->pid, ## arg)

#define HWCAM_CFG_DEBUG(fmt, arg...) \
    printk(KERN_DEBUG "%s(%d, %d): " fmt "\n", \
           __func__, __LINE__, current->pid, ## arg)
#define AVAIL_NAME_LENGTH  31

typedef struct _tag_hwcam_vbuf
{
    struct vb2_buffer                           buf;
    struct list_head                            node;
} hwcam_vbuf_t;

struct _tag_hwcam_cfgstream_intf;
struct _tag_hwcam_cfgpipeline_intf;
struct _tag_hwcam_dev_intf;
struct _tag_hwcam_user_intf;
struct _tag_hwcam_cfgreq_mount_stream_intf;
struct _tag_hwcam_cfgreq_mount_pipeline_intf;
struct _tag_hwcam_cfgreq_intf;
struct _tag_hwcam_cfgack;

////////////////////////////////////////////////////////////////////////////////
//
//          hwcam_cfgreq interface definition begin
//
////////////////////////////////////////////////////////////////////////////////
typedef struct _tag_hwcam_cfgreq_vtbl
{
    void (*get)(struct _tag_hwcam_cfgreq_intf* intf);
    int (*put)(struct _tag_hwcam_cfgreq_intf* intf);
    int (*on_req)(struct _tag_hwcam_cfgreq_intf* intf,
                  struct v4l2_event* ev);
    int (*on_cancel)(struct _tag_hwcam_cfgreq_intf* intf,
                     int reason);
    int (*on_ack)(struct _tag_hwcam_cfgreq_intf* intf,
                  struct _tag_hwcam_cfgack* ack);
} hwcam_cfgreq_vtbl_t;

typedef struct _tag_hwcam_cfgreq_intf
{
    struct _tag_hwcam_cfgreq_vtbl const* vtbl;
} hwcam_cfgreq_intf_t;

static inline void
hwcam_cfgreq_intf_get(hwcam_cfgreq_intf_t* intf)
{
    return intf->vtbl->get(intf);
}

static inline int
hwcam_cfgreq_intf_put(hwcam_cfgreq_intf_t* intf)
{
    return intf->vtbl->put(intf);
}

static inline int
hwcam_cfgreq_intf_on_req(hwcam_cfgreq_intf_t* intf,
                         struct v4l2_event* ev)
{
    return intf->vtbl->on_req(intf, ev);
}

static inline int
hwcam_cfgreq_intf_on_cancel(hwcam_cfgreq_intf_t* intf,
                            int reason)
{
    return intf->vtbl->on_cancel(intf, reason);
}

static inline int
hwcam_cfgreq_intf_on_ack(hwcam_cfgreq_intf_t* intf,
                         struct _tag_hwcam_cfgack* ack)
{
    return intf->vtbl->on_ack(intf, ack);
}

static inline int
hwcam_cfgreq_on_ack_noop(hwcam_cfgreq_intf_t* pintf,
                         struct _tag_hwcam_cfgack* ack)
{
    return 0;
}

typedef void (*pfn_hwcam_cfgdev_release_ack)(struct _tag_hwcam_cfgack* ack);

typedef struct _tag_hwcam_cfgack
{
    struct list_head                            node;
    pfn_hwcam_cfgdev_release_ack                release;
    struct v4l2_event                           ev;
} hwcam_cfgack_t;

static inline int
hwcam_cfgack_result(struct _tag_hwcam_cfgack* ack)
{
    return ((hwcam_cfgreq_t*)ack->ev.u.data)->rc;
}



////////////////////////////////////////////////////////////////////////////////
//
//          hwcam_usr interface definition begin
//
////////////////////////////////////////////////////////////////////////////////

typedef struct _tag_hwcam_user_vtbl
{
    void (*get)(struct _tag_hwcam_user_intf* intf);
    int (*put)(struct _tag_hwcam_user_intf* intf);
    void (*wait_begin)(struct _tag_hwcam_user_intf* intf);
    void (*wait_end)(struct _tag_hwcam_user_intf* intf);
    void (*notify)(struct _tag_hwcam_user_intf* intf,
                   struct v4l2_event* ev);
} hwcam_user_vtbl_t;

typedef struct _tag_hwcam_user_intf
{
    hwcam_user_vtbl_t const*                    vtbl;
} hwcam_user_intf_t;

static inline void
hwcam_user_intf_get(hwcam_user_intf_t* intf)
{
    intf->vtbl->get(intf);
}

static inline int
hwcam_user_intf_put(hwcam_user_intf_t* intf)
{
    return intf->vtbl->put(intf);
}

static inline void
hwcam_user_intf_wait_begin(hwcam_user_intf_t* intf)
{
    intf->vtbl->wait_begin(intf);
}

static inline void
hwcam_user_intf_wait_end(hwcam_user_intf_t* intf)
{
    intf->vtbl->wait_end(intf);
}

static inline void
hwcam_user_intf_notify(hwcam_user_intf_t* intf,
                       struct v4l2_event* ev)
{
    intf->vtbl->notify(intf, ev);
}

//  hwcam_usr interface definition end


////////////////////////////////////////////////////////////////////////////////
//
//          hwcam_dev interface definition begin
//
////////////////////////////////////////////////////////////////////////////////

extern int
hwcam_dev_create(struct device* dev, int* dev_num);

typedef struct _tag_hwcam_dev_vtbl
{
    void (*notify)(struct _tag_hwcam_dev_intf* intf,
                   struct v4l2_event* ev);
} hwcam_dev_vtbl_t;

typedef struct _tag_hwcam_dev_intf
{
    hwcam_dev_vtbl_t const*                     vtbl;
} hwcam_dev_intf_t;

static inline void
hwcam_dev_intf_notify(hwcam_dev_intf_t* intf,
                      struct v4l2_event* ev)
{
    intf->vtbl->notify(intf, ev);
}
//  hwcam_dev interface definition end


////////////////////////////////////////////////////////////////////////////////
//
//          hwcam_cfgdev interface definition begin
//
////////////////////////////////////////////////////////////////////////////////

extern void
hwcam_cfgdev_lock(void);

extern void
hwcam_cfgdev_unlock(void);

extern int
hwcam_cfgdev_queue_ack(struct v4l2_event* ev);

extern int
hwcam_cfgdev_send_req(hwcam_user_intf_t* user,
                      struct v4l2_event* ev,
                      struct v4l2_fh* target,
                      int one_way,
                      int* rc);

extern int
hwcam_cfgdev_register_subdev(struct v4l2_subdev* subdev,hwcam_device_id_constants_t dev_const);

extern int
hwcam_cfgdev_unregister_subdev(struct v4l2_subdev* subdev);

//  hwcam_cfgdev interface definition end

extern int
hw_is_binderized(void);

extern char*
gen_media_prefix(char* media_ent,hwcam_device_id_constants_t dev_const, size_t dst_size);

extern int
init_subdev_media_entity(struct v4l2_subdev* subdev,hwcam_device_id_constants_t dev_const);

#endif // __HW_ALAN_KERNEL_CAMERA_OBJ_MDL_INTERFACE__

