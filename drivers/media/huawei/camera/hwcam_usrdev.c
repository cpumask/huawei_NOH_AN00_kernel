/*
 *  Hisilicon K3 SOC camera driver source file
 *
 *  Copyright (C) Huawei Technology Co., Ltd.
 *
 * Author:
 * Email:
 * Date:	  2013-10-30
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


#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/videodev2.h>
#include <media/v4l2-dev.h>
#include <media/v4l2-device.h>
#include <media/v4l2-event.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-ioctl.h>
#include <media/videobuf2-core.h>
#include <securec.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
#include <media/videobuf2-v4l2.h>
#endif

#include "hwcam_intf.h"
#include "cam_log.h"
//lint -save -e593 -e455 -e454 -e31

typedef struct _tag_hwcam_dev
{
    struct v4l2_device                          v4l2;
    struct mutex                                lock;
    struct video_device*                        vdev;
    struct media_device*                        mdev;

    hwcam_dev_intf_t                            intf;
    struct mutex                                devlock;
} hwcam_dev_t;

typedef struct _tag_hwcam_user
{
	struct v4l2_fh                              eq;
    hwcam_dev_t*                                cam;

    hwcam_user_intf_t                           intf;
    struct kref                                 ref;

    struct vb2_queue                            vb2q;
    struct v4l2_format                          format;
    unsigned long                               f_format_valid : 1;
    unsigned long                               f_pipeline_owner : 1;
    struct mutex                                lock;
} hwcam_user_t;

#define VO2USER(fpd) container_of(fpd, hwcam_user_t, eq)

#define I2USER(i) container_of(i, hwcam_user_t, intf)

#define REF2USER(r) container_of(r, hwcam_user_t, ref)

static void
hwcam_user_release(
        struct kref* r)
{
    hwcam_user_t* user = REF2USER(r);
    if (NULL == user){
        HWCAM_CFG_ERR("REF2USER returns null. \n");
        return;
    }

    HWCAM_CFG_DEBUG("instance(0x%pK)", &user->intf);

    vb2_queue_release(&user->vb2q);
    mutex_destroy(&user->lock);
    user->intf.vtbl = NULL;
    kzfree(user);
}

static hwcam_user_vtbl_t s_vtbl_hwcam_user;

static hwcam_user_t*
hwcam_user_create_instance(
        hwcam_dev_t* cam)
{
	hwcam_user_t* user = NULL;
	errno_t err = 0;
	if(!cam){
		goto exit_create_instance;
	}

	user = kzalloc(sizeof(hwcam_user_t), GFP_KERNEL);
	if (!user) {
		goto exit_create_instance;
    }

    user->cam = cam;
    user->intf.vtbl = &s_vtbl_hwcam_user;
    kref_init(&user->ref);

    user->f_format_valid = 0;
    err = memset_s(&user->format, sizeof(user->format), 0, sizeof(user->format));
    if (err != EOK) {
        HWCAM_CFG_ERR("%s: memset_s fail", __func__);
    }
    user->vb2q.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    user->vb2q.io_modes = VB2_USERPTR;
    user->vb2q.buf_struct_size = sizeof(hwcam_vbuf_t);
    user->vb2q.drv_priv = user;
    user->vb2q.timestamp_flags = V4L2_BUF_FLAG_TIMESTAMP_COPY;
    if (vb2_queue_init(&user->vb2q)) {
        HWCAM_CFG_ERR("failed to initialize v4l2 buffer queue. \n");
        kzfree(user);
        user = NULL;
        goto exit_create_instance;
    }
    mutex_init(&user->lock);

    HWCAM_CFG_DEBUG("instance(0x%pK)", &user->intf);

exit_create_instance:
    return user;
}

static void
hwcam_user_get(
        hwcam_user_intf_t* intf)
{
    hwcam_user_t* user = I2USER(intf);
    kref_get(&user->ref);
}

static int
hwcam_user_put(
        hwcam_user_intf_t* intf)
{
    hwcam_user_t* user = I2USER(intf);
    return kref_put(&user->ref, hwcam_user_release);
}

static void
hwcam_user_wait_begin(
        hwcam_user_intf_t* intf)
{
    hwcam_user_t* user = I2USER(intf);
    mutex_unlock(&user->cam->lock);
}

static void
hwcam_user_wait_end(
        hwcam_user_intf_t* intf)
{
    hwcam_user_t* user = I2USER(intf);
    mutex_lock(&user->cam->lock);
}

static void
hwcam_user_notify(
        hwcam_user_intf_t* intf,
        struct v4l2_event* ev)
{
    hwcam_user_t* user = I2USER(intf);
    mutex_lock(&user->cam->lock);
    if (user->eq.vdev) {
        v4l2_event_queue_fh(&user->eq, ev);
    }
    mutex_unlock(&user->cam->lock);
}

static hwcam_user_vtbl_t
s_vtbl_hwcam_user =
{
    .get = hwcam_user_get,
    .put = hwcam_user_put,
    .wait_begin = hwcam_user_wait_begin,
    .wait_end = hwcam_user_wait_end,
    .notify = hwcam_user_notify,
};

static unsigned int
hwcam_dev_vo_poll(
        struct file* filep,
        struct poll_table_struct* wait)
{
	unsigned int rc = 0;
    hwcam_dev_t* cam = video_drvdata(filep);
	hwcam_user_t* user = VO2USER(filep->private_data);

    if (!cam || !user) {
        HWCAM_CFG_ERR("%s(%d): cam or user is NULL!", __func__, __LINE__);
        return (unsigned int)(-EINVAL);
    }

    mutex_lock(&user->lock);
    if (user->f_format_valid) {
        rc = vb2_poll(&user->vb2q, filep, wait);
        rc &= ~POLLERR;
    }
    poll_wait(filep, &user->eq.wait, wait);
    if (v4l2_event_pending(&user->eq)) {
        rc |= POLLPRI | POLLOUT | POLLIN;
    }
    mutex_unlock(&user->lock);

	return rc;
}

static int
hwcam_dev_vo_close(
        struct file* filep)
{
    hwcam_dev_t* cam = NULL;
    struct v4l2_fh* eq = NULL;

    cam = (hwcam_dev_t*)video_drvdata(filep);

    if (!cam) {
        HWCAM_CFG_ERR("%s(%d): cam is NULL!", __func__, __LINE__);
        return -EINVAL;
    }

    mutex_lock(&cam->devlock);
    swap(filep->private_data, eq);

    if (eq){
        hwcam_user_t* user = VO2USER(eq);

        v4l2_fh_del(eq);
        v4l2_fh_exit(eq);

        hwcam_user_intf_put(&user->intf);

        HWCAM_CFG_INFO("instance(0x%pK, /dev/video%d)",
                user, cam->vdev->num);
    }else{
        HWCAM_CFG_ERR("%s(%d): eq is NULL!", __func__, __LINE__);
        mutex_unlock(&cam->devlock);
        return -EINVAL;
    }

    mutex_unlock(&cam->devlock);

    return 0;
}

static int
hwcam_dev_vo_open(
        struct file* filep)
{
    long rc = 0;
    hwcam_dev_t* cam = NULL;
    hwcam_user_t* user = NULL;

    cam = (hwcam_dev_t*)video_drvdata(filep);
    user = hwcam_user_create_instance(cam);
    if (!user) {
        HWCAM_CFG_ERR("failed to failed to create instance! \n");
        rc = -ENOMEM;
        return rc;
    }

    mutex_lock(&cam->devlock);
	v4l2_fh_init(&user->eq, cam->vdev);
	v4l2_fh_add(&user->eq);
    filep->private_data = &user->eq;

    HWCAM_CFG_INFO("instance(0x%pK, /dev/video%d)",
            user, cam->vdev->num);

    mutex_unlock(&cam->devlock);
    return rc;
}

static struct v4l2_file_operations
s_fops_hwcam_dev =
{
	.owner = THIS_MODULE,
	.open	 = hwcam_dev_vo_open,
	.poll	 = hwcam_dev_vo_poll,
	.release = hwcam_dev_vo_close,
	.unlocked_ioctl = video_ioctl2,
#ifdef CONFIG_COMPAT
	.compat_ioctl32 = video_ioctl2,
#endif
};

#define I2DEV(i) container_of(i, hwcam_dev_t, intf)

static void
hwcam_dev_notify(
        hwcam_dev_intf_t* intf,
        struct v4l2_event* ev)
{
    hwcam_dev_t* cam = I2DEV(intf);
    v4l2_event_queue(cam->vdev, ev);
}

static hwcam_dev_vtbl_t
s_vtbl_hwcam_dev =
{
    .notify = hwcam_dev_notify,
};

int hwsensor_notify(struct device* pdev,struct v4l2_event* ev)
{
    struct v4l2_device *pv4l2 = NULL;
    hwcam_dev_t *pcam = NULL;
    if(pdev){
        pv4l2 = (struct v4l2_device*)dev_get_drvdata(pdev);
    }
    else{
        HWCAM_CFG_ERR("NULL pointer!");
        return -1;
    }
    if(pv4l2){
        pcam = container_of(pv4l2,hwcam_dev_t,v4l2);
    }
    else{
        HWCAM_CFG_ERR("fail to get hwcam_dev_t,fail to send notify");
        return -1;
    }
    if(pcam){
        pcam->intf.vtbl->notify(&pcam->intf,ev);
    }
    return 0;
}

void hwcam_dev_format_name(struct video_device* vdev, char *prefix)
{
	int rc;
	rc = snprintf_s(vdev->name + strlen(vdev->name), sizeof(vdev->name) - strlen(vdev->name),
			sizeof(vdev->name) - strlen(vdev->name) - 1, "%s", video_device_node_name(vdev));
	if (rc < 0) {
		HWCAM_CFG_ERR("Truncation Occurred\n");
		(void)snprintf_s(vdev->name, sizeof(vdev->name), sizeof(vdev->name) - 1, "%s", prefix);
		(void)snprintf_s(vdev->name + strlen(vdev->name), sizeof(vdev->name) - strlen(vdev->name),
				sizeof(vdev->name) - strlen(vdev->name) - 1, "%s", video_device_node_name(vdev));
	}
}

int
hwcam_dev_create(
        struct device* dev,
        int* dev_num)
{
	int rc = 0;
	int ret = 0;
	char media_prefix[10];
    struct v4l2_device* v4l2 = NULL;
    struct video_device* vdev = NULL;
    struct media_device* mdev = NULL;
    hwcam_dev_t* cam = NULL;

    cam = kzalloc(sizeof(hwcam_dev_t), GFP_KERNEL);
	if (!cam) {
		rc = -ENOMEM;
		goto init_end;
	}
    v4l2 = &cam->v4l2;

    vdev = video_device_alloc();
    if (!vdev) {
		rc = -ENOMEM;
		goto video_alloc_fail;
    }

    mdev = kzalloc(sizeof(struct media_device), GFP_KERNEL);
	if (!mdev) {
		rc = -ENOMEM;
		goto media_alloc_fail;
	}
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,9,0))
    media_device_init(mdev);
#endif
    strlcpy(mdev->model, HWCAM_MODEL_USER, sizeof(mdev->model));
    mdev->dev = dev;
    rc = media_device_register(mdev);
    if (rc < 0) {
        goto media_register_fail;
    }

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,9,0))
	rc = media_entity_pads_init(&vdev->entity, 0, NULL);
#else
	rc = media_entity_init(&vdev->entity, 0, NULL, 0);
#endif
	if (rc < 0) {
		goto entity_init_fail;
    }

    v4l2->mdev = mdev;
	v4l2->notify = NULL;
	rc = v4l2_device_register(dev, v4l2);
	if (rc < 0) {
		goto v4l2_register_fail;
    }

    gen_media_prefix(media_prefix, HWCAM_DEVICE_GROUP_ID, sizeof(media_prefix));
    ret = snprintf_s(vdev->name, sizeof(vdev->name), sizeof(vdev->name) - 1, "%s", media_prefix);
    if (ret < 0) {
        HWCAM_CFG_ERR("snprintf_s media_prefix failed");
    }
    strlcpy(vdev->name + strlen(vdev->name), "hwcam-userdev", sizeof(vdev->name) - strlen(vdev->name));
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,9,0))
	vdev->entity.obj_type = MEDIA_ENTITY_TYPE_VIDEO_DEVICE;
#else
	vdev->entity.type = MEDIA_ENT_T_DEVNODE_V4L;
	vdev->entity.group_id = HWCAM_DEVICE_GROUP_ID;
#endif
	vdev->v4l2_dev = v4l2;
	vdev->release = video_device_release;
	vdev->fops = &s_fops_hwcam_dev;
	vdev->minor = -1;
	vdev->vfl_type = VFL_TYPE_GRABBER;
	vdev->vfl_dir = VFL_DIR_TX;
        rc = video_register_device(vdev, VFL_TYPE_GRABBER, -1);
        if (rc < 0) {
        goto video_register_fail;
    }
    cam_debug("video dev name %s %s", vdev->dev.kobj.name, vdev->name);
    mutex_init(&cam->lock);
    mutex_init(&cam->devlock);
    vdev->lock = &cam->lock;

    hwcam_dev_format_name(vdev, media_prefix);

    rc = 0;
    vdev->entity.name = vdev->name;
    video_set_drvdata(vdev, cam);
    cam->vdev = vdev;
    cam->mdev = mdev;
    cam->intf.vtbl = &s_vtbl_hwcam_dev;
    *dev_num = vdev->num;

    goto init_end;

video_register_fail:
    v4l2_device_unregister(v4l2);

v4l2_register_fail:
    media_entity_cleanup(&vdev->entity);

entity_init_fail:
    media_device_unregister(mdev);

media_register_fail:
    kzfree(mdev);

media_alloc_fail:
    video_device_release(vdev);

video_alloc_fail:
    kzfree(cam);

init_end:
    return rc;
}
//lint -restore


