

#ifndef __OAL_LINUX_MM_H__
#define __OAL_LINUX_MM_H__

/* ����ͷ�ļ����� */
/*lint -e322*/
#include <linux/slab.h>
#include <linux/hardirq.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>
#ifdef _PRE_WLAN_CACHE_COHERENT_SUPPORT
#include <linux/dma-mapping.h>
#endif

/*lint +e322*/
/* �궨�� */
typedef dma_addr_t oal_dma_addr;

/* ö�ٶ��� */
typedef enum {
    OAL_BIDIRECTIONAL = 0,
    OAL_TO_DEVICE = 1,
    OAL_FROM_DEVICE = 2,
    OAL_NONE = 3,
} oal_data_direction;
typedef oal_uint8 oal_direction_uint8;

/*
 * �� �� ��  : oal_memalloc
 * ��������  : �������̬���ڴ�ռ䣬�����0������Linux����ϵͳ���ԣ�
 *             ��Ҫ�����ж������ĺ��ں������ĵĲ�ͬ���(GFP_KERNEL��GFP_ATOMIC)��
 */
OAL_STATIC OAL_INLINE oal_void *oal_memalloc(oal_uint32 ul_size)
{
    oal_int32 l_flags = GFP_KERNEL;
    oal_void *puc_mem_space = NULL;

    /* ��˯�߻����жϳ����б�־��ΪGFP_ATOMIC */
    if (in_interrupt() || irqs_disabled() || in_atomic()) {
        l_flags = GFP_ATOMIC;
    }

    if (unlikely(ul_size == 0)) {
        return OAL_PTR_NULL;
    }

    puc_mem_space = kmalloc(ul_size, l_flags);
    if (puc_mem_space == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    return puc_mem_space;
}

/*
 * �� �� ��  : oal_mem_dma_blockalloc
 * ��������  : ����������DMA�ڴ棬�������룬ֱ������ɹ����������ó�ʱ
 */
OAL_STATIC OAL_INLINE oal_void *oal_mem_dma_blockalloc(oal_uint32 size, oal_ulong timeout)
{
    gfp_t flags = 0;
    oal_void *puc_mem_space = NULL;
    unsigned long timeout2, timeout1;

    /* ��˯�߻����жϳ����б�־��ΪGFP_ATOMIC */
    if (in_interrupt() || irqs_disabled() || in_atomic()) {
        flags |= GFP_ATOMIC;

        return kmalloc(size, flags);
    } else {
        flags |= GFP_KERNEL;
    }

    flags |= __GFP_NOWARN;

    timeout2 = jiffies + msecs_to_jiffies(timeout);     /* ms */
    timeout1 = jiffies + msecs_to_jiffies(timeout / 2); /* half */

    do {
        puc_mem_space = kmalloc(size, flags);
        if (likely(puc_mem_space != NULL)) {
            /* sucuess */
            return puc_mem_space;
        }

        if (!time_after(jiffies, timeout1)) {
            cpu_relax();
            continue; /* δ��ʱ������ */
        }

        if (!time_after(jiffies, timeout2)) {
            msleep(1);
            continue; /* ��ʱ��δ���뵽����ʼ�ó����� */
        } else {
            if (flags & __GFP_NOWARN) {
                /* ��ʱ������������α�ǣ��������һ�� */
                flags &= ~__GFP_NOWARN;
                continue;
            } else {
                /* ��ʱ����ʧ�� */
                break;
            }
        }
    } while (1);

    return NULL;
}

/*
 * �� �� ��  : oal_mem_dma_blockfree
 * ��������  : �ͷ�DMA�ڴ�
 */
OAL_STATIC OAL_INLINE oal_void oal_mem_dma_blockfree(oal_void *puc_mem_space)
{
    if (likely(puc_mem_space != NULL)) {
        kfree(puc_mem_space);
    }
}

/*
 * �� �� ��  : oal_memtry_alloc
 * ��������  : �����������ڴ棬ָ�����������������С�ڴ棬
 *             ����ʵ��������ڴ棬����ʧ�ܷ���NULL
 */
OAL_STATIC OAL_INLINE oal_void *oal_memtry_alloc(oal_uint32 request_maxsize, oal_uint32 request_minsize,
                                                 oal_uint32 *actual_size)
{
    gfp_t flags = 0;
    oal_void *puc_mem_space = NULL;
    oal_uint32 request_size;

    if (WARN_ON(actual_size == NULL)) {
        return NULL;
    }

    *actual_size = 0;

    /* ��˯�߻����жϳ����б�־��ΪGFP_ATOMIC */
    if (in_interrupt() || irqs_disabled()) {
        flags |= GFP_ATOMIC;
    } else {
        flags |= GFP_KERNEL;
    }

    flags |= __GFP_NOWARN;

    request_size = ((request_maxsize >= request_minsize) ? request_maxsize : request_minsize);

    do {
        if (request_size <= request_minsize) {
            flags &= ~__GFP_NOWARN;
        }
        puc_mem_space = kmalloc(request_size, flags);
        if (puc_mem_space != NULL) {
            *actual_size = request_size;
            return puc_mem_space;
        }

        if (request_size <= request_minsize) {
            /* ����СSIZE������Ȼʧ�ܷ���NULL */
            break;
        }

        /* ����ʧ��, �۰��������� */
        request_size = request_size >> 1;
        request_size = ((request_size >= request_minsize) ? request_size : request_minsize);
    } while (1);

    return NULL;
}

#ifdef _PRE_WLAN_CACHE_COHERENT_SUPPORT
/*
 * �� �� ��  : oal_mem_uncache_alloc
 * ��������  : һ�����ڴ�(��cache)���亯��
 * �������  : �������mem�Ĵ�С(size),���ص������ַ
 * �������  : ���صĵ�ַ�������ַ���׵�ַ(phy addr)
 * �� �� ֵ  : �����������ַ
 */
OAL_STATIC OAL_INLINE oal_void *oal_mem_uncache_alloc(oal_uint32 ul_size, oal_uint32 *pul_phy_addr)
{
    oal_int32 l_flags = GFP_KERNEL;
    oal_void *puc_mem_space = NULL;
    oal_uint32 ul_dma_real_addr;

    if (OAL_UNLIKELY(pul_phy_addr == NULL)) {
        OAL_WARN_ON(1);
        return NULL;
    }

    /* ��˯�߻����жϳ����б�־��ΪGFP_ATOMIC */
    if (in_interrupt() || irqs_disabled()) {
        l_flags = GFP_ATOMIC;
    }

    puc_mem_space = dma_alloc_coherent(NULL, ul_size, &ul_dma_real_addr, l_flags);

    /* �����cache�ڴ�������ַ */
    *pul_phy_addr = (oal_uint32)ul_dma_real_addr;

    if (puc_mem_space == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    return puc_mem_space;
}

/*
 * �� �� ��  : oal_mem_uncache_free
 * ��������  : һ�����ڴ�(��cache)�ͷź���
 * �������  : �������mem�Ĵ�С(size),�����ڴ�������ַ(p_buf),
 *             ��Ӧ�������ַ(ul_dma_addr)
 */
OAL_STATIC OAL_INLINE oal_void oal_mem_uncache_free(oal_uint32 ul_size, oal_void *p_buf, oal_uint32 ul_dma_addr)
{
    dma_free_coherent(NULL, ul_size, p_buf, ul_dma_addr);
}

/*
 * �� �� ��  : oal_dma_map_single
 * ��������  : ˢcache�ӿڣ���cache��Ч�����߽�cache�е�����ͬ����DDR
 * �������  : ��һ������:struct device,�ڶ�������:��Ҫcache��ָ�룬����������:cache������size��С
 *             ���ĸ�����:����(0:˫����;1:��cacheͬ����DDR;2:��cache��Ч)
 * �� �� ֵ  : cache���ָ��ӳ��������ַ
 */
OAL_STATIC OAL_INLINE oal_uint32 oal_dma_map_single(struct device *pst_dev, oal_void *p_buf,
                                                    oal_uint32 ul_size, oal_direction_uint8 uc_dir)
{
    return dma_map_single(pst_dev, p_buf, ul_size, uc_dir);
}

/*
 * �� �� ��  : oal_dma_unmap_single
 * ��������  : �ýӿ���Ҫ��Ϊmap֮��ӳ��ĵ�ַȡ��ӳ�䣬ͬʱ���ڰ���L1��L2��cache�������֮ǰmap�������unmap
 * �������  : ��һ������:struct device,�ڶ�������:��Ҫcache��ָ��ӳ��������ַ������������:cache������size��С
 *             ���ĸ�����:����(0:˫����;1:��cacheͬ����DDR;2:��cache��Ч)
 */
OAL_STATIC OAL_INLINE oal_void oal_dma_unmap_single(struct device *pst_dev, oal_dma_addr ul_addr,
                                                    oal_uint32 ul_size, oal_direction_uint8 uc_dir)
{
    dma_unmap_single(pst_dev, ul_addr, ul_size, uc_dir);
}
#endif

/*
 * �� �� ��  : oal_free
 * ��������  : �ͷź���̬���ڴ�ռ䡣
 * �������  : pbuf:���ͷŵ��ڴ��ַ
 */
OAL_STATIC OAL_INLINE oal_void oal_free(oal_void *p_buf)
{
    kfree(p_buf);
}
#endif /* end of oal_mm.h */

