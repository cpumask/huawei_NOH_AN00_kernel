
#ifndef __DRV_VENC_EFL_H__
#define __DRV_VENC_EFL_H__

#include "drv_common.h"
#include "drv_mem.h"

typedef enum {
	YUV420_SEMIPLANAR     = 0,
	YUV420_PLANAR         = 3,
	YUV422_PLANAR         = 4,
	YUV422_PACKAGE        = 6,
	RGB_32BIT             = 8,
	YUV420_SEMIPLANAR_CMP = 10,
} color_format_t;

typedef struct {
	mem_buffer_t internal_buffer;
	mem_buffer_t image_buffer;
	mem_buffer_t stream_buffer[MAX_SLICE_NUM];
	mem_buffer_t stream_head_buffer;
} venc_buffer_info_t;

HI_S32 venc_drv_alloc_encode_done_info_buffer(struct file  *file);
HI_S32 venc_drv_free_encode_done_info_buffer(const struct file  *file);
HI_S32 venc_drv_encode(struct encode_info *encode_info, struct venc_fifo_buffer *buffer);
HI_S32 venc_drv_create_queue(void);
HI_VOID venc_drv_destroy_queue(void);
HI_S32 venc_drv_open_vedu(void);
HI_S32 venc_drv_close_vedu(void);
HI_S32 venc_drv_suspend_vedu(void);
HI_S32 venc_drv_resume_vedu(void);
HI_S32 venc_drv_resume(struct platform_device *pdev);
HI_S32 venc_drv_suspend(struct platform_device *pdev, pm_message_t state);

#endif //__DRV_VENC_EFL_H__

