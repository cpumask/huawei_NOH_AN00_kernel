

#ifdef CONFIG_HI1102_PLAT_HW_CHR
/* ͷ�ļ����� */
#include "chr_devs.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/semaphore.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <asm/atomic.h>
#include <stdarg.h>
#include <linux/slab.h>
#include <linux/unistd.h>
#include <linux/un.h>

#if defined(CONFIG_LOG_EXCEPTION) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
#include <log/log_usertype.h>
#endif

#ifdef CONFIG_HWCONNECTIVITY
#include "hisi_oneimage.h"
#endif
#include "oneimage.h"
#include "board.h"
#include "oal_schedule.h"
#include "chr_errno.h"
#include "oal_hcc_host_if.h"
#include "frw_ext_if.h"
#include "hal_commom_ops.h"
#include "frw_event_main.h"
#include "hw_bfg_ps.h"
#include "plat_pm.h"
#include "plat_pm_wlan.h"
#include "securec.h"
#include "bfgx_exception_rst.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_COMMON_CHR_DEVS_C


/* �������� */
static int32 chr_misc_open(struct inode *fd, struct file *fp);
static ssize_t chr_misc_read(struct file *fp, int8 __user *buff, size_t count, loff_t *loff);
static int64 chr_misc_ioctl(struct file *fp, uint32 cmd, uintptr_t arg);
static int32 chr_misc_release(struct inode *fd, struct file *fp);
void chr_rx_errno_to_dispatch(uint32 errno);
int32 chr_wifi_tx_handler(uint32 errno);
int32 chr_bfg_dev_tx_handler(uint32 ul_errno);
uint32 chr_rx_proc_test(uint32 errno);
static int32 chr_rx_process(oal_uint8 *buff, chr_dev_exception_stru_para *chr_dev_exception_p);


/* ȫ�ֱ������� */
static chr_event g_chr_event;
chr_callback_stru gst_chr_get_wifi_info_callback;
chr_rx_callback_stru g_chr_rx_callback[CHR_INDEX_MUTT];

int g_oam_beta_ver = 0;
oal_debug_module_param(g_oam_beta_ver, int, S_IRUGO | S_IWUSR);


/* ��ģ��debug����ȫ�ֱ��� */
static int32 log_enable = CHR_LOG_DISABLE;

static const struct file_operations chr_misc_fops = {
    .owner = THIS_MODULE,
    .open = chr_misc_open,
    .read = chr_misc_read,
    .release = chr_misc_release,
    .unlocked_ioctl = chr_misc_ioctl,
};

static struct miscdevice chr_misc_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = CHR_DEV_KMSG_PLAT,
    .fops = &chr_misc_fops,
};

/* ����У׼�쳣����dfr�ָ��Ĵ��� */
extern oal_uint32 g_cali_excp_dfr_count;
oal_uint32        g_cali_excp_trigger_dfr_flag = OAL_FALSE;
oal_uint32        g_at_test = OAL_FALSE;
oal_uint32 chr_get_at_test(void)
{
    return g_at_test;
}
void chr_set_at_test(oal_uint32 at_test)
{
    g_at_test = at_test ;
}

/*
 * �� �� ��  : chr_misc_open
 * ��������  : ���豸�ڵ�ӿ�
 */
static int32 chr_misc_open(struct inode *fd, struct file *fp)
{
    if (log_enable != CHR_LOG_ENABLE) {
        chr_err("chr %s open fail, module is disable\n", chr_misc_dev.name);
        return -EBUSY;
    }
    chr_dbg("chr %s open success\n", chr_misc_dev.name);
    return CHR_SUCC;
}

/*
 * �� �� ��  : chr_misc_read
 * ��������  : ��ȡ�豸�ڵ�ӿ�
 */
static ssize_t chr_misc_read(struct file *fp, int8 __user *buff, size_t count, loff_t *loff)
{
    int32 ret;
    uint32 __user *puser = (uint32 __user *)buff;
    struct sk_buff *skb = NULL;
    uint16 data_len = 0;

    if (log_enable != CHR_LOG_ENABLE) {
        chr_err("chr %s read fail, module is disable\n", chr_misc_dev.name);
        return -EBUSY;
    }

    if (count < sizeof(chr_dev_exception_stru_para)) {
        chr_err("The user space buff is too small\n");
        return -CHR_EFAIL;
    }

    if (buff == NULL) {
        chr_err("chr %s read fail, user buff is NULL", chr_misc_dev.name);
        return -EAGAIN;
    }
    skb = skb_dequeue(&g_chr_event.errno_queue);
    if (skb == NULL) {
        if (fp->f_flags & O_NONBLOCK) {
            chr_dbg("Thread read chr with NONBLOCK mode\n");
            /* for no data with O_NONBOCK mode return 0 */
            return 0;
        } else {
            if (wait_event_interruptible(g_chr_event.errno_wait,
                                         (skb = skb_dequeue(&g_chr_event.errno_queue)) != NULL)) {
                if (skb != NULL) {
                    skb_queue_head(&g_chr_event.errno_queue, skb);
                }
                chr_warning("Thread interrupt with signel\n");
                return -ERESTARTSYS;
            }
        }
    }

    chr_warning(">>>>>>>>>>>>>>>>>>[chr test]skb len=[%d], chr send event_id[%d] to user\n",
                skb->len, *(uint32 *)skb->data);
    /* event_id(4 bytes) + len(2 bytes) + type(1 byte) + resv(1 byte) + real_errid(16 bytes) = 24 bytes */
    if (skb->len == 24)
    {
        chr_warning(">>>>>>>>>>>>>>>>>>[chr test]chr send real_errid[%d] to user\n",
            *(uint32 *)(skb->data + 8) + *(uint32 *)(skb->data + 12) +
            (*(uint32 *)(skb->data + 16) << 8) + *(uint32 *)(skb->data + 20));
    }

    data_len = min_t(size_t, skb->len, count);
    ret = copy_to_user(puser, skb->data, data_len);

    if (ret) {
        chr_warning("copy_to_user err!restore it, len=%d\n", data_len);
        skb_queue_head(&g_chr_event.errno_queue, skb);
        return -EFAULT;
    }

    /* have read count1 byte */
    skb_pull(skb, data_len);

    /* if skb->len = 0: read is over */
    if (skb->len == 0) { /* curr skb data have read to user */
        kfree_skb(skb);
    } else { /* if don,t read over; restore to skb queue */
        skb_queue_head(&g_chr_event.errno_queue, skb);
    }

    return data_len;
}

/*
 * �� �� ��  : chr_errno_convert_to_bfg_id
 * ��������  : ��CHR ����ţ�ת��bfg id��
 */
static int32 chr_errno_convert_to_bfg_id(uint32 errno)
{
    uint32 chr_id;

    chr_id = errno / CHR_ID_MSK;

    if (chr_id == CHR_BT) {
        return BFGX_BT;
    } else if (chr_id == CHR_GNSS) {
        return BFGX_GNSS;
    } else {
        chr_err("unsupport errno [%d]\n", errno);
        return -EINVAL;
    }
}


/*
 * �� �� ��  : chr_write_errno_to_queue
 * ��������  : ���쳣��д�����
 */
static int32 chr_write_errno_to_queue(uint32 ul_errno, chr_report_flags_enum_uint16 us_flag, uint8 *ptr_data, uint16 ul_len)
{
    struct sk_buff *skb = NULL;
    uint16 sk_len;
    int32 ret = 0;

    if (skb_queue_len(&g_chr_event.errno_queue) > CHR_ERRNO_QUEUE_MAX_LEN) {
        chr_warning("chr errno queue is full, dispose errno=%x\n", ul_errno);
        return CHR_SUCC;
    }

    /* for code run in interrupt context */
    sk_len = sizeof(chr_dev_exception_stru_para) + ul_len;
    if (oal_in_interrupt() || oal_in_atomic() || irqs_disabled()) {
        skb = alloc_skb(sk_len, GFP_ATOMIC);
    } else {
        skb = alloc_skb(sk_len, GFP_KERNEL);
    }
    if (skb == NULL) {
        chr_err("chr errno alloc skbuff failed! len=%d, errno=%x\n", sk_len, ul_errno);
        return -ENOMEM;
    }

    skb_put(skb, sk_len);
    *(uint32 *)skb->data = ul_errno;
    *((uint16 *)(skb->data + 4)) = ul_len;  /* ƫ�ƴ��errno��ǰ4���ֽ� */
    *((uint16 *)(skb->data + 6)) = us_flag; /* ƫ�ƴ��errno�ӳ��ȵ�ǰ6���ֽ� */

    if ((ul_len > 0) && (ptr_data != NULL)) {
        ret = memcpy_s(((uint8 *)skb->data + OAL_SIZEOF(chr_dev_exception_stru_para)),
                       sk_len - OAL_SIZEOF(chr_dev_exception_stru_para),
                       ptr_data, ul_len);
        if (ret != EOK) {
            chr_err("memcpy_s error, destlen=%lu, srclen=%d\n ",
                    sk_len - OAL_SIZEOF(chr_dev_exception_stru_para),
                    ul_len);
        }
    }

    skb_queue_tail(&g_chr_event.errno_queue, skb);
    wake_up_interruptible(&g_chr_event.errno_wait);
    return CHR_SUCC;
}

static int64 chr_misc_errno_write(uint32 __user *puser)
{
    uint32 ret;
    uint8 *pst_mem = NULL;
    chr_host_exception_stru chr_rx_data;

    ret = copy_from_user(&chr_rx_data, puser, OAL_SIZEOF(chr_host_exception_stru));
    if (ret) {
        chr_err("chr %s ioctl fail, get data from user fail", chr_misc_dev.name);
        return -EINVAL;
    }

    if (chr_rx_data.chr_len == 0) {
        chr_write_errno_to_queue(chr_rx_data.chr_errno, CHR_HOST, NULL, 0);
    } else {
        pst_mem = oal_memalloc(chr_rx_data.chr_len);
        if (pst_mem == NULL) {
            chr_err("chr mem alloc failed len %u\n", chr_rx_data.chr_len);
            return -EINVAL;
        }

        if (chr_rx_data.chr_ptr == NULL) {
            chr_err("chr input arg is invalid!\n");
            oal_free(pst_mem);
            return -EINVAL;
        }
        ret = copy_from_user(pst_mem, (void __user *)(chr_rx_data.chr_ptr), chr_rx_data.chr_len);
        if (ret) {
            chr_err("chr %s ioctl fail, get data from user fail", chr_misc_dev.name);
            oal_free(pst_mem);
            return -EINVAL;
        }

        chr_write_errno_to_queue(chr_rx_data.chr_errno, CHR_HOST, pst_mem, chr_rx_data.chr_len);
        oal_free(pst_mem);
    }

    return CHR_SUCC;
}

/*
 * �� �� ��  : chr_misc_ioctl
 * ��������  : �����豸�ڵ�ӿ�
 */
static int64 chr_misc_ioctl(struct file *fp, uint32 cmd, uintptr_t arg)
{
    uint32 __user *puser = (uint32 __user *)arg;
    uint32 ret;
    uint32 value = 0;

    if (log_enable != CHR_LOG_ENABLE) {
        chr_err("chr %s ioctl fail, module is disable\n", chr_misc_dev.name);
        return -EBUSY;
    }

    if (_IOC_TYPE(cmd) != CHR_MAGIC) {
        chr_err("chr %s ioctl fail, the type of cmd is error type is %d\n",
                chr_misc_dev.name, _IOC_TYPE(cmd));
        return -EINVAL;
    }

    if (_IOC_NR(cmd) > CHR_MAX_NR) {
        chr_err("chr %s ioctl fail, the nr of cmd is error, nr is %d\n",
                chr_misc_dev.name, _IOC_NR(cmd));
        return -EINVAL;
    }

    switch (cmd) {
        case CHR_ERRNO_WRITE:
            if (chr_misc_errno_write(puser) < 0) {
                return -EINVAL;
            }
            break;
        case CHR_ERRNO_ASK:
            ret = get_user(value, puser);
            if (ret) {
                chr_err("chr %s ioctl fail, get data from user fail", chr_misc_dev.name);
                return -EINVAL;
            }

            chr_rx_errno_to_dispatch(value);
            break;
        default:
            chr_warning("chr ioctl not support cmd=0x%x\n", cmd);
            return -EINVAL;
    }
    return CHR_SUCC;
}

/*
 * �� �� ��  : chr_misc_release
 * ��������  : �ͷŽڵ��豸�ӿ�
 */
static int32 chr_misc_release(struct inode *fd, struct file *fp)
{
    if (log_enable != CHR_LOG_ENABLE) {
        chr_err("chr %s release fail, module is disable\n", chr_misc_dev.name);
        return -EBUSY;
    }
    chr_dbg("chr %s release success\n", chr_misc_dev.name);
    return CHR_SUCC;
}

/*
 * �� �� ��  : __chr_printLog
 * ��������  : �ں���־��ӡ�ӿ�
 */
int32 __chr_printLog(CHR_LOGPRIORITY prio, CHR_DEV_INDEX dev_index, const int8 *fmt, ...)
{
    return CHR_SUCC;
}
EXPORT_SYMBOL(__chr_printLog);

/*
 * �� �� ��  : __chr_exception
 * ��������  : �ں˿ռ����쳣��ӿ�
 */
int32 __chr_exception(uint32 errno)
{
    if (log_enable != CHR_LOG_ENABLE) {
        chr_dbg("chr throw exception fail, module is disable\n");
        return -CHR_EFAIL;
    }

    chr_write_errno_to_queue(errno, CHR_HOST, NULL, 0);
    return CHR_SUCC;
}

int32 __chr_exception_para(uint32 chr_errno, uint8 *chr_ptr, uint16 chr_len)
{
    if (log_enable != CHR_LOG_ENABLE) {
        chr_dbg("chr throw exception fail, module is disable\n");
        return -CHR_EFAIL;
    }

    chr_write_errno_to_queue(chr_errno, CHR_HOST, chr_ptr, chr_len);
    return CHR_SUCC;
}

int32 __chr_exception_para_q(uint32 chr_errno, chr_report_flags_enum_uint16 chr_flag, uint8 *chr_ptr, uint16 chr_len)
{
    if (log_enable != CHR_LOG_ENABLE) {
        chr_dbg("chr throw exception fail, module is disable\n");
        return -CHR_EFAIL;
    }
    chr_write_errno_to_queue(chr_errno, chr_flag, chr_ptr, chr_len);
    return CHR_SUCC;
}

typedef struct {
    char ApErrCode;
    char arpTxErrCode;
    char macHardErrCode;
} dhcpFailChipInfo_STRU;

#define CHR_WIFI_TEST_ERRNO  909050004
#define CHR_BT_TEST_ERRNO    913050006

void chr_test(void)
{
    dhcpFailChipInfo_STRU aa = {0};
    aa.ApErrCode = 'j';
    aa.arpTxErrCode = 'p';
    aa.macHardErrCode = 'p';

    __chr_exception_para(CHR_WIFI_TEST_ERRNO, (oal_uint8 *)&aa, OAL_SIZEOF(dhcpFailChipInfo_STRU));

    chr_rx_errno_to_dispatch(CHR_BT_TEST_ERRNO);
    chr_rx_errno_to_dispatch(CHR_WIFI_TEST_ERRNO);

    OAL_IO_PRINT("{chr_test:: end !}\r\n");
}

uint32 chr_rx_proc_test(uint32 errno)
{
    OAL_IO_PRINT("{chr_rx_proc_test:: errno = %u !}", errno);
    CHR_EXCEPTION_P(errno, NULL, 0);
    return CHR_SUCC;
}

EXPORT_SYMBOL(__chr_exception);
EXPORT_SYMBOL(__chr_exception_para);
EXPORT_SYMBOL(__chr_exception_para_q);

/*
 * �� �� ��  : chr_get_exception_info
 * ��������  : ��ȡCHR ��Ϣ
 */
void chr_get_exception_info(chr_dev_exception_stru_para *pst_dmac_dev_exception_info,
    chr_dev_exception_stru_para *pst_hmac_dev_exception_info)
{
    pst_hmac_dev_exception_info->errno  = pst_dmac_dev_exception_info->errno;
    pst_hmac_dev_exception_info->errlen = OAL_SIZEOF(chr_hmac_info_stru) - OAL_SIZEOF(chr_dev_exception_stru_para);
    pst_hmac_dev_exception_info->flag   = pst_dmac_dev_exception_info->flag;
}

/*
 * �� �� ��  : chr_get_common_info
 * ��������  : ��ȡоƬƽ̨��Ϣ
 */
void chr_get_common_info(chr_dmac_common_info_stru *pst_dmac_common_info,
    chr_hmac_common_info_stru *pst_hmac_common_info)
{
    oal_int32 l_ret;

    /* ac_fw_ver device�����鳤��Ϊ20��host�೤��Ϊ50������ֻ��Ҫcopy dmac��ac_fw_ver����ĳ��� */
    l_ret = memcpy_s(pst_hmac_common_info->ac_fw_ver, OAL_SIZEOF(pst_hmac_common_info->ac_fw_ver),
        pst_dmac_common_info->ac_fw_ver,
        OAL_MIN(OAL_SIZEOF(pst_dmac_common_info->ac_fw_ver), OAL_SIZEOF(pst_hmac_common_info->ac_fw_ver)));
    l_ret += memcpy_s(pst_hmac_common_info->ac_ko_ver, OAL_SIZEOF(pst_hmac_common_info->ac_ko_ver),
        pst_dmac_common_info->ac_ko_ver, OAL_SIZEOF(pst_hmac_common_info->ac_ko_ver));
    l_ret += memcpy_s(pst_hmac_common_info->ac_dieid, OAL_SIZEOF(pst_hmac_common_info->ac_dieid),
        pst_dmac_common_info->ac_dieid, OAL_SIZEOF(pst_hmac_common_info->ac_dieid));
    pst_hmac_common_info->s_cur_temp_state = pst_dmac_common_info->s_cur_temp_state;

    if (l_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "chr_get_common_info::memcpy fail!");
    }
}

/*
 * �� �� ��  : chr_get_rate_info
 * ��������  : ��ȡ����������Ϣ
 */
void chr_get_rate_info(chr_dmac_rate_info_stru *pst_dmac_rate_info, chr_hmac_rate_info_stru *pst_hmac_rate_info)
{
    if (memcpy_s(pst_hmac_rate_info, OAL_SIZEOF(chr_hmac_rate_info_stru),
        pst_dmac_rate_info, OAL_SIZEOF(chr_hmac_rate_info_stru))) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "chr_get_rate_info::memcpy fail.");
    }
}

/*
 * �� �� ��  : chr_get_radio_link_quality_and_btcoex_info
 * ��������  : ��ȡ�տ���·�����͹��������Ϣ
 */
void chr_get_radio_link_quality_and_btcoex_info(chr_dmac_radio_link_quality_info_stru *pst_dmac_radio_link_quality_info,
                                                chr_hmac_radio_link_quality_info_stru *pst_hmac_radio_link_quality_info,
                                                chr_hmac_btcoex_status_stru *pst_hmac_btcoex_status)
{
    int32                           l_ret;
    hmac_get_wifi_info_ext_stru     st_get_wifi_info_ext;
    oal_int32                       l_memcpy_ret;
    /* ʹ�ù��Ӻ�����ȡhmac�����,����ͨ��deviceͨ���ϱ� */
    if (gst_chr_get_wifi_info_callback.chr_get_wifi_ext_info_from_host == OAL_PTR_NULL) {
        return;
    }
    l_ret = gst_chr_get_wifi_info_callback.chr_get_wifi_ext_info_from_host(&st_get_wifi_info_ext);
    if (l_ret != CHR_SUCC) {
        return;
    }

    /* ��ȡ���Ų�����оƬ���Ͳ��� */
    pst_hmac_radio_link_quality_info->en_alg_distance_stat = st_get_wifi_info_ext.uc_device_distance;
    pst_hmac_radio_link_quality_info->en_adj_intf_state = st_get_wifi_info_ext.uc_intf_state_cca;
    pst_hmac_radio_link_quality_info->en_co_intf_state = st_get_wifi_info_ext.uc_intf_state_co;
    pst_hmac_radio_link_quality_info->c_chip_type = st_get_wifi_info_ext.uc_chip_type;

    /* ��ȡ������� */
    l_memcpy_ret = memcpy_s(&pst_hmac_btcoex_status->st_bt_status, OAL_SIZEOF(bt_status_stru),
        &st_get_wifi_info_ext.st_bt_status, OAL_SIZEOF(bt_status_stru));
    l_memcpy_ret += memcpy_s(&pst_hmac_btcoex_status->st_ble_status, OAL_SIZEOF(btcoex_ble_status_stru),
        &st_get_wifi_info_ext.st_ble_status, OAL_SIZEOF(btcoex_ble_status_stru));
    if (l_memcpy_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "chr_get_radio_link_quality_and_btcoex_info::memcpy fail.");
    }
    pst_hmac_radio_link_quality_info->s_ant0_rssi = pst_dmac_radio_link_quality_info->s_ant0_rssi;
}


/*
 * �� �� ��  : chr_get_chip_count_info
 * ��������  : ��ȡоƬͳ����Ϣ
 */
void chr_get_chip_count_info(chr_dmac_chip_count_info_stru *pst_dmac_chip_count_info,
                             chr_hmac_chip_count_info_stru *pst_hmac_chip_count_info)
{
    if (memcpy_s(pst_hmac_chip_count_info, OAL_SIZEOF(chr_hmac_chip_count_info_stru),
        pst_dmac_chip_count_info, OAL_SIZEOF(chr_hmac_chip_count_info_stru)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "chr_get_chip_count_info::memcpy fail.");
    }
}

/*
 * �� �� ��  : chr_get_queue_info
 * ��������  : ��ȡ����״̬��Ϣ
 */
void chr_get_queue_info(chr_dmac_queue_info_stru *pst_dmac_queue_info, chr_hmac_queue_info_stru *pst_hmac_queue_info)
{
    oal_int32 l_ret;

    l_ret = memcpy_s(pst_hmac_queue_info->aus_tx_mpdu_num, OAL_SIZEOF(uint16) * CHR_WLAN_TID_MAX_NUM,
        pst_dmac_queue_info->aus_tx_mpdu_num, OAL_SIZEOF(uint16) * CHR_WLAN_TID_MAX_NUM);
    l_ret += memcpy_s(pst_hmac_queue_info->auc_tx_ppdu_num, OAL_SIZEOF(uint8) * CHR_HAL_TX_QUEUE_NUM,
        pst_dmac_queue_info->auc_tx_ppdu_num, OAL_SIZEOF(uint8) * CHR_HAL_TX_QUEUE_NUM);
    l_ret += memcpy_s(pst_hmac_queue_info->aus_rx_ppdu_num, OAL_SIZEOF(uint16) * CHR_HAL_RX_QUEUE_NUM,
        pst_dmac_queue_info->aus_rx_ppdu_num, OAL_SIZEOF(uint16) * CHR_HAL_RX_QUEUE_NUM);
    if (l_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "chr_get_queue_info::memcpy fail.");
    }
    pst_hmac_queue_info->us_tx_free_ppdu_cnt = pst_dmac_queue_info->us_tx_free_ppdu_cnt;
}

/*
 * �� �� ��  : chr_get_send_mode_info
 * ��������  : ��ȡ����ģʽ��Ϣ
 */
void chr_get_send_mode_info(chr_dmac_send_mode_info_stru *pst_dmac_send_mode_info,
    chr_hmac_send_mode_info_stru *pst_hmac_send_mode_info)
{
    oal_int32 l_ret;

    /* host����device��ṹ�����һ������auc_tx_ant��vht��һ����Ա���������Էֶ�copy */
    l_ret = memcpy_s(pst_hmac_send_mode_info, OAL_SIZEOF(chr_hmac_send_mode_info_stru), pst_dmac_send_mode_info,
        OAL_OFFSET_OF(chr_hmac_send_mode_info_stru, auc_tx_ant));
    /* ������ֵ����4������ */
    l_ret += memcpy_s(pst_hmac_send_mode_info->auc_tx_chain, CHR_HAL_TX_RATE_MAX_NUM,
        pst_dmac_send_mode_info->auc_tx_chain, OAL_SIZEOF(uint8)*CHR_HAL_TX_RATE_MAX_NUM);
    l_ret += memcpy_s(pst_hmac_send_mode_info->auc_phy_mode, CHR_HAL_TX_RATE_MAX_NUM,
        pst_dmac_send_mode_info->auc_phy_mode, OAL_SIZEOF(uint8)*CHR_HAL_TX_RATE_MAX_NUM);
    l_ret += memcpy_s(pst_hmac_send_mode_info->auc_edca_cwmax, CHR_HAL_TX_RATE_MAX_NUM,
        pst_dmac_send_mode_info->auc_edca_cwmax, OAL_SIZEOF(uint8)*CHR_HAL_TX_RATE_MAX_NUM);
    l_ret += memcpy_s(pst_hmac_send_mode_info->auc_edca_cwmin, CHR_HAL_TX_RATE_MAX_NUM,
        pst_dmac_send_mode_info->auc_edca_cwmin, OAL_SIZEOF(uint8)*CHR_HAL_TX_RATE_MAX_NUM);

    l_ret += memcpy_s(pst_hmac_send_mode_info->ac_acc_limit, CHR_WLAN_WME_AC_BUTT,
        pst_dmac_send_mode_info->ac_acc_limit, OAL_SIZEOF(int8)*CHR_WLAN_WME_AC_BUTT);
    if (l_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "chr_get_send_mode_info::memcpy fail.");
    }

    pst_hmac_send_mode_info->en_bandwidth        = pst_dmac_send_mode_info->en_bandwidth;
    pst_hmac_send_mode_info->uc_ldpc_implement   = pst_dmac_send_mode_info->uc_ldpc_implement;
    pst_hmac_send_mode_info->uc_ldpc_activate    = pst_dmac_send_mode_info->uc_ldpc_activate;
}

/*
 * �� �� ��  : chr_get_txrx_packets
 * ��������  : ��ȡ�շ���ͳ����Ϣ
 */
void chr_get_txrx_packets(chr_dmac_txrx_pkts_stru *pst_dmac_txrx_pkts, chr_hmac_txrx_pkts_stru *pst_hmac_txrx_pkts)
{
    oal_int32 l_ret;

    /* host���device������һ����Ա����rssi����Ҫ�ֶ�copy */
    l_ret = memcpy_s(pst_hmac_txrx_pkts, OAL_SIZEOF(chr_hmac_txrx_pkts_stru),
        pst_dmac_txrx_pkts, OAL_OFFSET_OF(chr_dmac_txrx_pkts_stru, l_rssi));

    l_ret += memcpy_s(&pst_hmac_txrx_pkts->ul_rx_total_pkts,
        OAL_SIZEOF(chr_dmac_txrx_pkts_stru) - OAL_OFFSET_OF(chr_dmac_txrx_pkts_stru, ul_rx_total_pkts),
        &pst_dmac_txrx_pkts->ul_rx_total_pkts,
        OAL_SIZEOF(chr_dmac_txrx_pkts_stru) - OAL_OFFSET_OF(chr_dmac_txrx_pkts_stru, ul_rx_total_pkts));
    if (l_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "chr_get_txrx_packets::memcpy fail.");
    }
}

/*
 * �� �� ��  : chr_get_pm_info
 * ��������  : ��ȡоƬ�͹�����Ϣ
 */
void chr_get_pm_info(chr_dmac_pm_info_stru *pst_dmac_pm_info, chr_hmac_pm_info_stru *pst_hmac_pm_info)
{
    /* ������ֵǰ�������� */
    if (memcpy_s(pst_hmac_pm_info, OAL_SIZEOF(chr_hmac_pm_info_stru),
        pst_dmac_pm_info, OAL_OFFSET_OF(chr_hmac_pm_info_stru, ul_duty_ratio)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "chr_get_pm_info::memcpy fail.");
    }

    pst_hmac_pm_info->ul_cpu_freq_level = pst_dmac_pm_info->ul_cpu_freq_level;

    /* host����device��ṹ���8bit��ͬ,�����ܶ�λ��ȡ��ַ,���Բ���ֱ��memcopy */
    pst_hmac_pm_info->bit_pm_more_data_expected = pst_dmac_pm_info->bit_pm_more_data_expected;
    pst_hmac_pm_info->bit_11k_enable            = pst_dmac_pm_info->bit_11k_enable;
    pst_hmac_pm_info->bit_11v_enable            = pst_dmac_pm_info->bit_11v_enable;
    pst_hmac_pm_info->bit_11r_enable            = pst_dmac_pm_info->bit_11r_enable;
    pst_hmac_pm_info->bit_ap_11k_enable         = pst_dmac_pm_info->bit_ap_11k_enable;
    pst_hmac_pm_info->bit_ap_11v_enable         = pst_dmac_pm_info->bit_ap_11v_enable;
    pst_hmac_pm_info->bit_ap_11r_enable         = pst_dmac_pm_info->bit_ap_11r_enable;
}

/*
 * �� �� ��  : chr_dmac_transition_to_hmac
 * ��������  : ��DMAC�ϱ���CHR����ת��HMAC��Ӧ�����ݽṹ
 * �����ϲ�����������
 */
void chr_dmac_transition_to_hmac(chr_dmac_info_stru *buff, uint16 len)
{
    chr_dmac_info_stru *pst_chr_dmac_info = NULL;
    chr_hmac_info_stru *pst_chr_hmac_info = NULL;
    oal_uint8 *chr_data = NULL;

    if (buff == NULL) {
        return;
    }

    if (len != OAL_SIZEOF(chr_dmac_info_stru)) {
        chr_warning("chr recv device data fail, len %d is unavailable, chr_len %d\n",
                    (int32)len, (int32)OAL_SIZEOF(chr_dmac_info_stru));
        return;
    }

    pst_chr_dmac_info = (chr_dmac_info_stru *)buff;
    pst_chr_hmac_info = (chr_hmac_info_stru *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_LOCAL,
                                                            OAL_SIZEOF(chr_hmac_info_stru), OAL_TRUE);
    if (pst_chr_hmac_info == OAL_PTR_NULL) {
        return;
    }

    memset_s(pst_chr_hmac_info, OAL_SIZEOF(chr_hmac_info_stru), 0, OAL_SIZEOF(chr_hmac_info_stru));

    chr_get_exception_info(&pst_chr_dmac_info->st_dev_exception_info, &pst_chr_hmac_info->st_dev_exception_info);
    chr_get_common_info(&pst_chr_dmac_info->st_common_info, &pst_chr_hmac_info->st_common_info);
    chr_get_rate_info(&pst_chr_dmac_info->st_chr_rate_info, &pst_chr_hmac_info->st_chr_rate_info);
    chr_get_radio_link_quality_and_btcoex_info(&pst_chr_dmac_info->st_radio_link_quality_info,
        &pst_chr_hmac_info->st_radio_link_quality_info, &pst_chr_hmac_info->st_btcoex_status);
    chr_get_chip_count_info(&pst_chr_dmac_info->st_chip_count_info, &pst_chr_hmac_info->st_chip_count_info);
    chr_get_queue_info(&pst_chr_dmac_info->st_queue_info, &pst_chr_hmac_info->st_queue_info);
    chr_get_send_mode_info(&pst_chr_dmac_info->st_send_mode_info, &pst_chr_hmac_info->st_send_mode_info);
    chr_get_txrx_packets(&pst_chr_dmac_info->st_txrx_pkts, &pst_chr_hmac_info->st_txrx_pkts);
    chr_get_pm_info(&pst_chr_dmac_info->st_pm_info, &pst_chr_hmac_info->st_pm_info);

    chr_data = (oal_uint8 *)pst_chr_hmac_info + OAL_SIZEOF(chr_dev_exception_stru_para);
    chr_write_errno_to_queue(pst_chr_hmac_info->st_dev_exception_info.errno,
        pst_chr_hmac_info->st_dev_exception_info.flag, chr_data, pst_chr_hmac_info->st_dev_exception_info.errlen);
    if (gst_chr_get_wifi_info_callback.chr_process_web_fail != OAL_PTR_NULL) {
        gst_chr_get_wifi_info_callback.chr_process_web_fail(pst_chr_dmac_info->st_dev_exception_info.errno,
                                                            pst_chr_dmac_info->st_chr_rate_info.ul_rx_rate);
    }
    /* �����ϲ�,add chr�ϱ�������־ */
    CHR_EXCEPTION_Q(pst_chr_hmac_info->st_dev_exception_info.errno, CHR_REPORT_FINISH, NULL, 0);

    OAL_MEM_FREE(pst_chr_hmac_info, OAL_TRUE);
}


void chr_dev_cali_excp_trigger_dfr(oal_void)
{
    hcc_bus *hi_bus = hcc_get_current_110x_bus();
    OAM_WARNING_LOG1(0, OAM_SF_DFR, "chr_dev_cali_excp_trigger_dfr::g_cali_excp_trigger_dfr_flag[%d]\n", g_cali_excp_trigger_dfr_flag);
    if (g_cali_excp_trigger_dfr_flag == OAL_TRUE) {
        if (hi_bus != NULL && g_exception_info->exception_reset_enable == PLAT_EXCEPTION_ENABLE) {
            hcc_bus_exception_submit(hi_bus, RF_CALI_FAIL);
        }
        g_cali_excp_trigger_dfr_flag = OAL_FALSE;
    }
}

static void chr_dev_cali_excp_process(oal_void)
{
    hal_chr_cali_err_stru st_chr_cali_err = {0};

    if (g_cali_excp_dfr_count < 1 && chr_get_at_test() == OAL_FALSE) {
        g_cali_excp_dfr_count++;
        g_cali_excp_trigger_dfr_flag = OAL_TRUE;
        OAM_WARNING_LOG0(0, OAM_SF_DFR, "cali excp sub wifi system set dfr trigger flag OK!\n");
    } else {
        OAM_WARNING_LOG1(0, OAM_SF_DFR, "cali excp sub wifi system. g_cali_excp_dfr_count[%d]!\n", g_cali_excp_dfr_count);

        st_chr_cali_err.uc_chr_cali_err = HAL_CHR_CALI_ERR_RECOVER_FAIL;
        CHR_EXCEPTION_P(CHR_CHIP_CALI_ERR_REPORT_EVENTID,
            (oal_uint8*)(&st_chr_cali_err), OAL_SIZEOF(hal_chr_cali_err_stru));
    }
}

static oal_uint32 chr_dev_cali_excp_check(hal_chr_cali_err_enum_uint8 chr_cali_err)
{
    switch (chr_cali_err)
    {
        case HAL_CHR_CALI_ERR_PLL_UPC_2G:
        case HAL_CHR_CALI_ERR_PLL_UPC_5G:
        case HAL_CHR_CALI_ERR_PHASE1_SHIFT:
        case HAL_CHR_CALI_ERR_PHASE2_SHIFT:
        case HAL_CHR_CALI_ERR_MATRIX_NULL:
        case HAL_CHR_CALI_ERR_FEM_FAIL:
            return OAL_TRUE;
        default:
            break;
    }
    return OAL_FALSE;
}


static void chr_dev_exception_process(oal_uint8 *buff, chr_dev_exception_stru_para *chr_dev_exception_p)
{
    oal_uint8 *chr_data = NULL;
    hal_chr_cali_err_stru *pst_cali_err_code = NULL;

    chr_data = (oal_uint8 *)buff + OAL_SIZEOF(chr_dev_exception_stru_para);
    pst_cali_err_code = (hal_chr_cali_err_stru *)chr_data;
    if ((chr_dev_exception_p->errno == CHR_CHIP_CALI_ERR_REPORT_EVENTID) &&
        (chr_dev_cali_excp_check(pst_cali_err_code->uc_chr_cali_err) == OAL_TRUE)) {
        chr_dev_cali_excp_process();
    }
    if (chr_rx_process(buff, chr_dev_exception_p) == CHR_EFAIL) {
        chr_write_errno_to_queue(chr_dev_exception_p->errno, chr_dev_exception_p->flag,
                                 chr_data, chr_dev_exception_p->errlen);
    }
}

/*
 * �� �� ��  : chr_dev_exception_callback
 * ��������  : device�쳣�ص��ӿ�
 */
void chr_dev_exception_callback(void *buff, uint16 len)
{
    chr_dev_exception_stru_para *chr_dev_exception_p = NULL;
    chr_dev_exception_stru *chr_dev_exception = NULL;
    oal_uint32 chr_len = 0;

    if (log_enable != CHR_LOG_ENABLE) {
        chr_dbg("chr throw exception fail, module is disable\n");
        return;
    }

    if (buff == NULL) {
        chr_warning("chr recv device errno fail, buff is NULL\n");
        return;
    }

    chr_dev_exception = (chr_dev_exception_stru *)buff;

    /* mode select */
    if ((chr_dev_exception->framehead == 0x7e) && (chr_dev_exception->frametail == 0x7e)) {
        /* old interface: chr upload has only errno */
        chr_len = sizeof(chr_dev_exception_stru);

        if (len != chr_len) {
            chr_warning("chr recv device errno fail, len %d is unavailable,chr_len %d\n", (int32)len, chr_len);
            return;
        }

        chr_write_errno_to_queue(chr_dev_exception->error, CHR_DEVICE, NULL, 0);
    } else {
        /* new interface:chr upload eigher has data or not */
        chr_dev_exception_p = (chr_dev_exception_stru_para *)buff;
        chr_len = sizeof(chr_dev_exception_stru_para) + chr_dev_exception_p->errlen;

        if (len != chr_len) {
            chr_warning("chr recv device errno fail, len %d is unavailable,chr_len %d\n", (int32)len, chr_len);
            return;
        }

        if (chr_dev_exception_p->errlen == 0) {
            chr_write_errno_to_queue(chr_dev_exception_p->errno, chr_dev_exception_p->flag, NULL, 0);
        } else {
            if ((chr_dev_exception_p->errno <= CHR_WIFI_WEB_SLOW_EVENTID) &&
                (chr_dev_exception_p->errno >= CHR_WIFI_DISCONNECT_EVENTID)) {
                chr_dmac_transition_to_hmac(buff, len);
            } else {
                chr_dev_exception_process(buff, chr_dev_exception_p);
            }
        }
    }
}
EXPORT_SYMBOL(chr_dev_exception_callback);
EXPORT_SYMBOL(g_at_test);


/*
 * �� �� ��  : chr_host_callback_register
 * ��������  : host�ص�ע�ắ��
 */
void chr_host_callback_register(chr_get_wifi_info pfunc)
{
    if (pfunc == NULL) {
        chr_err("chr_host_callback_register::pfunc is null !");
        return;
    }
    gst_chr_get_wifi_info_callback.chr_get_wifi_info = pfunc;

    return;
}

void chr_host_callback_unregister(void)
{
    gst_chr_get_wifi_info_callback.chr_get_wifi_info = OAL_PTR_NULL;

    return;
}

void chr_process_web_fail_callback_register(chr_process_web_fail pfunc)
{
    gst_chr_get_wifi_info_callback.chr_process_web_fail = pfunc;
}
void chr_process_web_fail_callback_unregister(void)
{
    gst_chr_get_wifi_info_callback.chr_process_web_fail = OAL_PTR_NULL;
}
/*
 * �� �� ��  : chr_get_wifi_ext_info_callback_register
 * ��������  : host�ص�ע�ắ��:���ڻ�ȡdevice�ϱ��Ķ�����Ϣ
 */
void chr_get_wifi_ext_info_callback_register(chr_get_wifi_info_ext pfunc)
{
    if (pfunc == NULL) {
        chr_err("chr_get_wifi_info_callback_register::pfunc is null !");
        return;
    }
    gst_chr_get_wifi_info_callback.chr_get_wifi_ext_info_from_host = pfunc;

    return;
}
void chr_get_wifi_ext_info_callback_unregister(void)
{
    gst_chr_get_wifi_info_callback.chr_get_wifi_ext_info_from_host = OAL_PTR_NULL;

    return;
}


EXPORT_SYMBOL(chr_host_callback_register);
EXPORT_SYMBOL(chr_host_callback_unregister);
EXPORT_SYMBOL(chr_get_wifi_ext_info_callback_register);
EXPORT_SYMBOL(chr_get_wifi_ext_info_callback_unregister);
EXPORT_SYMBOL(chr_dev_cali_excp_trigger_dfr);
EXPORT_SYMBOL(chr_process_web_fail_callback_register);
EXPORT_SYMBOL(chr_process_web_fail_callback_unregister);


/*
 * �� �� ��  : chr_rx_errno_to_dispatch
 * ��������  : �����յ���errno���н���������
 */
void chr_rx_errno_to_dispatch(uint32 errno)
{
    uint32 chr_num;
    chr_num = errno / CHR_ID_MSK;
    switch (chr_num) {
        case CHR_WIFI:

            if (chr_wifi_tx_handler(errno) != CHR_SUCC) {
                chr_err("chr_rx_errno_to_dispatch::wifi tx failed,0x%x", errno);
            }
            break;

        case CHR_BT:
        case CHR_GNSS:

            if (chr_bfg_dev_tx_handler(errno) != CHR_SUCC) {
                chr_err("chr_rx_errno_to_dispatch::bt/gnss tx failed,0x%x", errno);
            }
            break;

        default:
            chr_err("chr_rx_errno_to_dispatch::rcv error num 0x%x", errno);
    }
}

/*
 * �� �� ��  : chr_wifi_dev_tx_handler
 * ��������  : ͨ��hccͨ����errno�·���wifi device
 */
int32 chr_wifi_dev_tx_handler(uint32 errno)
{
    struct hcc_transfer_param st_hcc_transfer_param = {0};
    struct hcc_handler *hcc = hcc_get_110x_handler();
    oal_netbuf_stru *pst_netbuf = NULL;
    int32 l_ret;

    if (hcc == NULL) {
        OAL_IO_PRINT("chr_wifi_dev_tx_handler::hcc is null\n");
        return -CHR_EFAIL;
    }

    pst_netbuf = hcc_netbuf_alloc(OAL_SIZEOF(uint32));
    if (pst_netbuf == NULL) {
        OAL_IO_PRINT("hwifi alloc skb fail.\n");
        return -CHR_EFAIL;
    }

    l_ret = memcpy_s(oal_netbuf_put(pst_netbuf, OAL_SIZEOF(uint32)), OAL_SIZEOF(uint32), &errno, OAL_SIZEOF(uint32));
    if (l_ret != EOK) {
        OAL_IO_PRINT("chr_wifi errno copy failed\n");
        oal_netbuf_free(pst_netbuf);
        return -CHR_EFAIL;
    }

    hcc_hdr_param_init(&st_hcc_transfer_param,
                       HCC_ACTION_TYPE_CHR,
                       0,
                       0,
                       HCC_FC_NONE,
                       DATA_HI_QUEUE);
    l_ret = hcc_tx(hcc, pst_netbuf, &st_hcc_transfer_param);
    if (l_ret != CHR_SUCC) {
        OAL_IO_PRINT("chr_wifi_dev_tx_handler::hcc tx is fail,ret=%d\n", l_ret);
        oal_netbuf_free(pst_netbuf);
        return -CHR_EFAIL;
    }

    return CHR_SUCC;
}

/*
 * �� �� ��  : chr_host_tx_handler
 * ��������  : ���ûص��ӿڽ�errno����hmac
 */
int32 chr_host_tx_handler(uint32 errno)
{
    if (gst_chr_get_wifi_info_callback.chr_get_wifi_info == OAL_PTR_NULL) {
        OAL_IO_PRINT("{chr_host_tx_handler:: callback is null!}");
        return CHR_EFAIL;
    }
    if (gst_chr_get_wifi_info_callback.chr_get_wifi_info(errno) != CHR_SUCC) {
        OAL_IO_PRINT("{chr_host_tx_handler:: tx faild, errno = %u !}", errno);
        return CHR_EFAIL;
    }

    return CHR_SUCC;
}

int32 chr_wifi_tx_handler(uint32 errno)
{
    int32 ret1;
    uint32 ret2;

    if (wlan_pm_is_poweron() == OAL_FALSE) {
        chr_info("handle error[%d] fail, wifi device not active\n", errno);
        return -CHR_EFAIL;
    }

    ret1 = chr_host_tx_handler(errno);
    ret2 = chr_wifi_dev_tx_handler(errno);
    if ((ret1 != CHR_SUCC) || (ret2 != CHR_SUCC)) {
        chr_err("chr_wifi_tx_handler::wifi tx failed,errno[0x%x],host tx ret1[%u],device tx ret2[%u]",
                errno, ret1, ret2);
        return -CHR_EFAIL;
    }
    chr_info("chr_wifi_tx_handler::tx is succ,errno %u\n", errno);

    return CHR_SUCC;
}

/*
 * �� �� ��  : chr_rx_process
 * ��������  : ���ûص��������Խ��յ���chr����
 */
static int32 chr_rx_process(oal_uint8 *buff, chr_dev_exception_stru_para *chr_dev_exception_p)
{
    int32 chr_index;
    int32 ret;
    oal_uint8 *chr_data = NULL;

    chr_data = (oal_uint8 *)buff + OAL_SIZEOF(chr_dev_exception_stru_para);

    for (chr_index = 0; chr_index < CHR_INDEX_MUTT; chr_index++) {
        if (g_chr_rx_callback[chr_index].rx_process == NULL) {
            continue;
        }

        ret = g_chr_rx_callback[chr_index].rx_process(chr_data, chr_dev_exception_p);
        if (ret == CHR_SUCC) {
            return  CHR_SUCC;
        }
    }

    return CHR_EFAIL;
}

/*
 * �� �� ��  : chr_rx_callback_register
 * ��������  : ע��Խ��յ�CHR ����Ļص�����
 */
int32 chr_rx_callback_register(uint32 chr_index, rx_process_callback rx_process)
{
    if (chr_index >= CHR_INDEX_MUTT) {
        chr_err("chr id invalid\n");
        return CHR_EFAIL;
    }

    if (g_chr_rx_callback[chr_index].rx_process != NULL) {
        return CHR_EFAIL;
    }

    g_chr_rx_callback[chr_index].rx_process = rx_process;

    return CHR_SUCC;
}

/*
 * �� �� ��  : chr_rx_callback_register
 * ��������  : �Խ��յ�CHR ȥע��
 */
void chr_rx_callback_remove(uint32 chr_index)
{
    if (chr_index >= CHR_INDEX_MUTT) {
        chr_err("chr id invalid\n");
        return;
    }

    g_chr_rx_callback[chr_index].rx_process = NULL;
}

typedef struct {
    uint32 file_id; // �ļ�id
    int32 line_id;  // �к�
} limit_file_line_str;

/*
 * �����ļ�id && �кŹ��˵�����Ҫ�ϱ���OAM ERR CHR
 * ����к�Ϊ-1�� �������ļ����еĴ�����־��������Ҫ�ϱ�
 */
static limit_file_line_str g_limit_file_line_arr[] = {
    {4016, -1}, // pm_core_wifi.c
    {3537, 5961}, // hal_mac.c HAL_MAC_ERROR_PHY_TRLR_TIME_OUT
    {3352, 1478}, // damc_scan.c RRM BEACON REQ SCAN FAIL
};

/*
 * �� �� ��  : chr_oam_blacklist_limit
 * ��������  : ����device�У��Ѿ�rom����ERROR��־��ӡ
 */
static int32 chr_oam_blacklist_limit(uint32 file_line)
{
    const int file_id_offset = 16;
    const int file_id_mask = 0xFFFF;
    const int line_offset = 0;
    const int line_mask = 0xFFFF;
    int32 file_id, line, index;

    file_id = (file_line >> file_id_offset) & file_id_mask;
    line  = (file_line >> line_offset) & line_mask;

    for (index = 0; index < OAL_ARRAY_SIZE(g_limit_file_line_arr); index++) {
        if (g_limit_file_line_arr[index].file_id != file_id) {
            continue;
        }

        if ((g_limit_file_line_arr[index].line_id < 0) ||
            (g_limit_file_line_arr[index].line_id == line)) {
            chr_info("ERROR LOG file id %d, line %d in blacklist limit\n", file_id, line);
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}

/*
 * �� �� ��  : chr_platform_rx_callback
 * ��������  : ƽ̨ע��Ľ��ջص�����
 */
int32 chr_platform_rx_callback(oal_uint8 *chr_data, chr_dev_exception_stru_para *chr_dev_exception_p)
{
#define LIMIT_SAME_ERR_TIME 24
    static uint32 last_flie_line = 0;
    chr_oam_stru *oam_err = NULL;
    chr_platform_exception_event_info_stru *plaform_exception_info = NULL;

    if (chr_dev_exception_p->errno == CHR_PLATFORM_EXCEPTION_EVENTID) {
        plaform_exception_info = (chr_platform_exception_event_info_stru *)chr_data;

        if (plaform_exception_info->ul_module == CHR_SYSTEM_OAM) {
            // ��beta�û����ϱ�
            if ((g_oam_beta_ver > 0)
#if defined(CONFIG_LOG_EXCEPTION) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
               || (get_logusertype_flag() != BETA_USER)
#endif
            ) {
                return CHR_SUCC;
            }

            oam_err = (chr_oam_stru *)chr_data;

            // ����ں������У����ϱ�CHR
            if (chr_oam_blacklist_limit(oam_err->file_line) == OAL_TRUE) {
                return CHR_SUCC;
            }

            // �������ϱ�ͬһ��������־�ĳ���������24 Hour��ֻ�ϱ�һ��
            if (!oal_print_rate_limit(LIMIT_SAME_ERR_TIME * PRINT_RATE_HOUR) &&
                (oam_err->file_line == last_flie_line)) {
                return CHR_SUCC;
            }

            /* 1Hour ��ֻ�����ϱ�һ��oam error CHR
             */
            if (oal_print_rate_limit(1 * PRINT_RATE_HOUR)) {
                chr_info("revice device ERROR LOG,file_line 0x%x, oam sn %d, err log cnt %d \n",
                         oam_err->file_line, oam_err->oam_sn, oam_err->err_log_cnt);
                last_flie_line = oam_err->file_line;
                chr_write_errno_to_queue(chr_dev_exception_p->errno, chr_dev_exception_p->flag,
                                         chr_data, chr_dev_exception_p->errlen);
            }

            return CHR_SUCC;
        }
    }

    return CHR_EFAIL;
}

/*
 * �� �� ��  : chr_bfg_dev_tx_handler
 * ��������  : ����uartͨ����errno����bfg
 */
int32 chr_bfg_dev_tx_handler(uint32 ul_errno)
{
    struct ps_core_s *ps_core_d = NULL;
    struct sk_buff *skb = NULL;
    uint16 sk_len;
    int32 ret, bfgx_subsys;

    ps_get_core_reference(&ps_core_d);
    if (unlikely((ps_core_d == NULL) || (ps_core_d->ps_pm == NULL))) {
        chr_err("ps_core_d is NULL\n");
        return CHR_EFAIL;
    }

    bfgx_subsys = chr_errno_convert_to_bfg_id(ul_errno);
    if (bfgx_subsys < 0) {
        return CHR_EFAIL;
    }

    if (!ps_bfg_subsys_active(bfgx_subsys)) {
        chr_info("handle error[%d] fail, bfgx device not active\n", ul_errno);
        return CHR_EFAIL;
    }

    /* if high queue num > MAX_NUM and don't write */
    if (ps_core_d->tx_high_seq.qlen > TX_HIGH_QUE_MAX_NUM) {
        chr_err("bt tx high seqlen large than MAXNUM\n");
        return CHR_EFAIL;
    }

    ret = prepare_to_visit_node(ps_core_d);
    if (ret != CHR_SUCC) {
        chr_err("chr_bfg_dev_tx_handler::prepare work fail, bring to reset work\n");
        plat_exception_handler(SUBSYS_BFGX, THREAD_BT, BFGX_WAKEUP_FAIL);
        return ret;
    }

    /* modify expire time of uart idle timer */
    mod_timer(&ps_core_d->ps_pm->pm_priv_data->bfg_timer, jiffies + (BT_SLEEP_TIME * HZ / 1000));
    ps_core_d->ps_pm->pm_priv_data->bfg_timer_mod_cnt++;

    /* alloc skb buf */
    sk_len = sizeof(uint32) + sizeof(struct ps_packet_head) + sizeof(struct ps_packet_end);
    if (oal_in_interrupt() || oal_in_atomic() || irqs_disabled()) {
        skb = alloc_skb(sk_len, GFP_ATOMIC);
    } else {
        skb = alloc_skb(sk_len, GFP_KERNEL);
    }
    if (skb == NULL) {
        chr_err("chr_bfg_dev_tx_handler::alloc skbuff failed! len=%d, errno=0x%x\n", sk_len, ul_errno);
        post_to_visit_node(ps_core_d);
        return -CHR_EFAIL;
    }

    skb_put(skb, sk_len);

    /* skb data init,reuse the type of mem_dump to prevent the change of rom */
    ps_add_packet_head(skb->data, MEM_DUMP, sk_len);

    /* put errno into skb_data */
    *(uint32 *)(skb->data + sizeof(struct ps_packet_head)) = ul_errno;

    ps_skb_enqueue(ps_core_d, skb, TX_HIGH_QUEUE);
    queue_work(ps_core_d->ps_tx_workqueue, &ps_core_d->tx_skb_work);

    post_to_visit_node(ps_core_d);

    chr_warning("chr_bfg_dev_tx_handler::tx is succ,errno %u\n", ul_errno);

    return CHR_SUCC;
}

int32 chr_miscdevs_init(void)
{
    int32 ret;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (!is_my_chip()) {
        chr_info("cfg chr log chip type is not match, skip driver init");
        log_enable = CHR_LOG_DISABLE;
        return -EINVAL;
    } else {
        chr_info("cfg chr log is matched with hi110x, continue");
    }
#endif
    init_waitqueue_head(&g_chr_event.errno_wait);
    skb_queue_head_init(&g_chr_event.errno_queue);

    ret = misc_register(&chr_misc_dev);
    if (ret != CHR_SUCC) {
        chr_err("chr module init fail\n");
        return -CHR_EFAIL;
    }

    ret = chr_rx_callback_register(CHR_INDEX_KMSG_PLAT, chr_platform_rx_callback);
    if (ret != CHR_SUCC) {
        chr_err("chr callback register fail\n");
        return -CHR_EFAIL;
    }

    log_enable = CHR_LOG_ENABLE;
    chr_info("chr module init succ\n");

    return CHR_SUCC;
}

void chr_miscdevs_exit(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (!is_my_chip()) {
        chr_info("cfg chr log chip type is not match, skip driver init");
        return;
    } else {
        chr_info("cfg chr log is matched with hi110x, continue");
    }
#endif
    if (log_enable != CHR_LOG_ENABLE) {
        chr_info("chr module is diabled\n");
        return;
    }

    chr_rx_callback_remove(CHR_INDEX_KMSG_PLAT);

    misc_deregister(&chr_misc_dev);
    log_enable = CHR_LOG_DISABLE;
    chr_info("chr module exit succ\n");
}

MODULE_AUTHOR("Hisilicon platform Driver Group");
MODULE_DESCRIPTION("hi110x chr log driver");
MODULE_LICENSE("GPL");
#endif
