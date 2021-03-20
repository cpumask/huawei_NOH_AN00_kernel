

/* ͷ�ļ����� */
#include "sdt_drv.h"

#include "oal_mem.h"
#include "wal_ext_if.h"
#include "oam_ext_if.h"
#include "hw_bfg_ps.h"
#include "securec.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_SDT_DRV_C

/* ȫ�ֱ������� */
sdt_drv_mng_stru g_sdt_drv_mng_entry;
oam_sdt_func_hook_stru g_sdt_drv_func_hook;
#if ((_PRE_TARGET_PRODUCT_TYPE_5610DMB == _PRE_CONFIG_TARGET_PRODUCT) || \
     (_PRE_TARGET_PRODUCT_TYPE_VSPM310DMB == _PRE_CONFIG_TARGET_PRODUCT) || \
     (_PRE_TARGET_PRODUCT_TYPE_WS835DMB == _PRE_CONFIG_TARGET_PRODUCT))
oal_uint8 g_st_count = 0;
oal_uint32 g_buf_offset = 0;

oal_netbuf_stru *g_copy_netbuf = NULL;
oal_nlmsghdr_stru *g_nlhdr = NULL;
#endif

/* ����ʵ�� */
OAL_STATIC oal_uint32 sdt_drv_netlink_send(oal_netbuf_stru *pst_netbuf, oal_uint32 ul_len);

/*
 * �� �� ��  : sdt_drv_set_mng_entry_usepid
 * ��������  : ����sdt drv��Զ�netlink���̵�pid��
 */
oal_void sdt_drv_set_mng_entry_usepid(oal_uint32 ulpid)
{
    oal_uint ui_irq_save;

    oal_spin_lock_irq_save(&g_sdt_drv_mng_entry.st_spin_lock, &ui_irq_save);

    g_sdt_drv_mng_entry.ul_usepid = ulpid;

    oal_spin_unlock_irq_restore(&g_sdt_drv_mng_entry.st_spin_lock, &ui_irq_save);
}

/*
 * �� �� ��  : sdt_drv_netbuf_add_to_list_tail
 * ��������  : sdt�շ���Ϣ���
 */
OAL_STATIC OAL_INLINE oal_void sdt_drv_netbuf_add_to_list(oal_netbuf_stru *pst_netbuf)
{
    oal_uint ui_irq_save;

    oal_spin_lock_irq_save(&g_sdt_drv_mng_entry.st_spin_lock, &ui_irq_save);

    oal_netbuf_add_to_list_tail(pst_netbuf, &g_sdt_drv_mng_entry.rx_wifi_dbg_seq);

    oal_spin_unlock_irq_restore(&g_sdt_drv_mng_entry.st_spin_lock, &ui_irq_save);
}

/*
 * �� �� ��  : sdt_drv_netbuf_delist
 * ��������  : sdt��Ϣ���г���
 * �� �� ֵ  : oal_netbuf_stru*
 */
oal_netbuf_stru *sdt_drv_netbuf_delist(oal_void)
{
    oal_uint ui_irq_save;
    oal_netbuf_stru *pst_netbuf = NULL;

    oal_spin_lock_irq_save(&g_sdt_drv_mng_entry.st_spin_lock, &ui_irq_save);

    pst_netbuf = oal_netbuf_delist(&g_sdt_drv_mng_entry.rx_wifi_dbg_seq);

    oal_spin_unlock_irq_restore(&g_sdt_drv_mng_entry.st_spin_lock, &ui_irq_save);

    return pst_netbuf;
}

/*
 * �� �� ��  : sdt_drv_check_isdevlog
 * ��������  : ����ϱ���־�Ƿ�Ϊdevice log
 */
OAL_STATIC OAL_INLINE oal_int32 sdt_drv_check_isdevlog(oal_netbuf_stru *pst_netbuf)
{
    oal_uint8 *puc_pkt_tail;
    sdt_drv_pkt_hdr_stru *pst_pkt_hdr;
    pst_pkt_hdr = (sdt_drv_pkt_hdr_stru *)oal_netbuf_data(pst_netbuf);
    puc_pkt_tail = (oal_uint8 *)pst_pkt_hdr + OAL_NETBUF_LEN(pst_netbuf);
    OAL_IO_PRINT("devlog {%s}\n", oal_netbuf_data(pst_netbuf));
    if (*puc_pkt_tail == SDT_DRV_PKT_END_FLG ||
        pst_pkt_hdr->uc_data_start_flg == SDT_DRV_PKT_START_FLG) {
        OAL_IO_PRINT("check out is device log\n");
        return OAL_SUCC;
    }

    return -OAL_EFAIL;
}

/*
 * �� �� ��  : sdt_drv_add_pkt_head
 * ��������  : ����������յ����ݣ�ǰ��������͵���Ϣ
 */
OAL_STATIC OAL_INLINE oal_void sdt_drv_add_pkt_head(oal_netbuf_stru *pst_netbuf,
                                                    oam_data_type_enum_uint8 en_type,
                                                    oam_primid_type_enum_uint8 en_prim_id)
{
    /*                         buffer structure                               */
    /*                      |data_hdr | data | data_tail |                    */
    /* ---------------------------------------------------------------------- */
    /*                  |  8Byte  |      |    1Byte  |                        */
    /*                       data header structure                            */
    /* ucFrameStart | ucFuncType | ucPrimId | ucReserver | usFrameLen | usSN  */
    /* ---------------------------------------------------------------------  */
    /*    1Byte     |    1Byte   |  1Byte   |   1Byte    |  2Bytes    |2Bytes */
    oal_uint8 *puc_pkt_tail = NULL;
    sdt_drv_pkt_hdr_stru *pst_pkt_hdr = NULL;
    oal_uint16 us_tmp_data;

    oal_netbuf_push(pst_netbuf, WLAN_SDT_SKB_HEADROOM_LEN);
    oal_netbuf_put(pst_netbuf, WLAN_SDT_SKB_TAILROOM_LEN);

    /* SDT�յ�����Ϣ��Ŀ��1 */
    g_sdt_drv_mng_entry.us_sn_num++;

    /* Ϊ����ͷ��ÿһ����Ա��ֵ */
    pst_pkt_hdr = (sdt_drv_pkt_hdr_stru *)oal_netbuf_data(pst_netbuf);

    pst_pkt_hdr->uc_data_start_flg = SDT_DRV_PKT_START_FLG;
    pst_pkt_hdr->en_msg_type = en_type;
    pst_pkt_hdr->uc_prim_id = en_prim_id;
    pst_pkt_hdr->uc_resv[0] = 0;

    us_tmp_data = (oal_uint16)OAL_NETBUF_LEN(pst_netbuf);
    pst_pkt_hdr->uc_data_len_low_byte = SDT_DRV_GET_LOW_BYTE(us_tmp_data);
    pst_pkt_hdr->uc_data_len_high_byte = SDT_DRV_GET_HIGH_BYTE(us_tmp_data);

    us_tmp_data = g_sdt_drv_mng_entry.us_sn_num;
    pst_pkt_hdr->uc_sequence_num_low_byte = SDT_DRV_GET_LOW_BYTE(us_tmp_data);
    pst_pkt_hdr->uc_sequence_num_high_byte = SDT_DRV_GET_HIGH_BYTE(us_tmp_data);

    /* Ϊ����β��ֵ0x7e */
    puc_pkt_tail = (oal_uint8 *)pst_pkt_hdr + OAL_NETBUF_LEN(pst_netbuf);
    puc_pkt_tail--;
    *puc_pkt_tail = SDT_DRV_PKT_END_FLG;
}

/*
 * �� �� ��  : sdt_drv_report_data2app
 * ��������  : �ϱ��ں˲����Ϣ
 */
OAL_STATIC OAL_INLINE oal_int32 sdt_drv_report_data2app(oal_netbuf_stru *pst_netbuf,
                                                        oam_data_type_enum_uint8 en_type,
                                                        oam_primid_type_enum_uint8 en_prim)
{
    /* ���ϲ���ýӿ��ж�ָ��ǿ� */
    oal_int32 l_ret;

    // disable report
    if (!ps_is_device_log_enable()) {
        oal_mem_sdt_netbuf_free(pst_netbuf, OAL_TRUE);
        return 0;
    }

    /* �����device log ����Ҫ��pkt ��ͷ */
    if (en_type != OAM_DATA_TYPE_DEVICE_LOG) {
        sdt_drv_add_pkt_head(pst_netbuf, en_type, en_prim);
    }

    sdt_drv_netbuf_add_to_list(pst_netbuf);

    l_ret = oal_workqueue_schedule(&g_sdt_drv_mng_entry.rx_wifi_work);

    return l_ret;
}

/*
 * �� �� ��  : sdt_drv_get_wq_len
 * ��������  : ����sdt�������Ϣ�Ķ��г���
 */
OAL_STATIC OAL_INLINE oal_int32 sdt_drv_get_wq_len(oal_void)
{
    return (oal_int32)oal_netbuf_list_len(&g_sdt_drv_mng_entry.rx_wifi_dbg_seq);
}

/*
 * �� �� ��  : sdt_drv_send_data_to_wifi
 * ��������  : ��app����յ����ݣ����������ݵ�ͷ����Ϣ��ȷ�������ݵ�ȥ��Ȼ��
 *             ������Ӧ��51������ӿ�
 */
oal_int32 sdt_drv_send_data_to_wifi(oal_uint8 *puc_param, oal_int32 l_len)
{
    oal_netbuf_stru *pst_netbuf = NULL;
    oal_int8 *pc_buf = NULL;
    oal_int i_len; /* SDIO CRC ERROR */
    oal_int32 l_ret = OAL_EFAIL;
    oal_uint8 *puc_data = NULL;
    const oal_uint32 ul_max_len = 300;

    if (puc_param == OAL_PTR_NULL) {
        OAL_IO_PRINT("sdt_drv_send_data_to_wifi::puc_param is null!\n");
        return -OAL_EFAIL;
    }

    if (l_len <= 0) {
        OAL_IO_PRINT("sdt_drv_send_data_to_wifi::data len little then zero!\n");
        return -OAL_EFAIL;
    }
    /* i_len��ʹ���޷�����Ϊ�˷�ֹ���������д��븺ֵ��ɳ��������޷������� */
    i_len = (oal_int)l_len > ul_max_len ? (oal_int)l_len : ul_max_len;

    /* ������Ϣ�������ͷ��ֱ��ʹ�� */
    pst_netbuf = oal_mem_sdt_netbuf_alloc((oal_uint16)i_len, OAL_TRUE);
    if (pst_netbuf == OAL_PTR_NULL) {
        OAL_IO_PRINT("sdt_drv_send_data_to_wifi::netbuf null pointer!! \n");
        return -OAL_EFAIL;
    }

    pc_buf = (oal_int8 *)oal_netbuf_put(pst_netbuf, (oal_uint32)l_len);
    /* i_len greater than or equal to l_len */
    memcpy_s((oal_void *)pc_buf, l_len, (const oal_void *)puc_param, (oal_uint32)l_len);

    /* ���pc_buf��С��0��ֵ�������Լ����� */
    i_len = pc_buf[5] * MAX_NUM;
    i_len = pc_buf[4] + i_len;
    i_len = i_len - OAM_RESERVE_SKB_LEN;

    puc_data = oal_netbuf_data(pst_netbuf);

    if (i_len < 0) {
        OAL_IO_PRINT("sdt_drv_send_data_to_wifi::need len large then zero!! \n");
        oal_mem_sdt_netbuf_free(pst_netbuf, OAL_TRUE);
        return -OAL_EFAIL;
    }

#ifdef _PRE_PRODUCT_ID_HI110X_HOST
    OAL_IO_PRINT("[DEBUG]sdt_drv_send_data_to_wif:: type [%d].\n", pc_buf[1]);
#endif

    switch (pc_buf[1]) {
        case OAM_DATA_TYPE_MEM_RW:
            if (oam_wal_func_hook.p_wal_recv_mem_data_func != OAL_PTR_NULL) {
                l_ret = oam_wal_func_hook.p_wal_recv_mem_data_func(&puc_data[SDT_DEV_NAME_INDEX],
                                                                   (oal_uint16)i_len);
            }
            break;

        case OAM_DATA_TYPE_REG_RW:
            if (oam_wal_func_hook.p_wal_recv_reg_data_func != OAL_PTR_NULL) {
                l_ret = oam_wal_func_hook.p_wal_recv_reg_data_func(&puc_data[SDT_DEV_NAME_INDEX],
                                                                   (oal_uint16)i_len);
            }
            break;

        case OAM_DATA_TYPE_CFG:
            if (oam_wal_func_hook.p_wal_recv_cfg_data_func != OAL_PTR_NULL) {
                l_ret = oam_wal_func_hook.p_wal_recv_cfg_data_func(&puc_data[SDT_DEV_NAME_INDEX],
                                                                   (oal_uint16)i_len);
            }
            break;

        case OAM_DATA_TYPE_GVAR_RW:
            if (oam_wal_func_hook.p_wal_recv_global_var_func != OAL_PTR_NULL) {
                l_ret = oam_wal_func_hook.p_wal_recv_global_var_func(&puc_data[SDT_DEV_NAME_INDEX],
                                                                     (oal_uint16)i_len);
            }
            break;

#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
        case OAM_DATA_TYPE_SAMPLE:
            if (oam_wal_func_hook.p_wal_recv_sample_data_func != OAL_PTR_NULL) {
                l_ret = oam_wal_func_hook.p_wal_recv_sample_data_func(&puc_data[SDT_DEV_NAME_INDEX],
                                                                      (oal_uint16)i_len);
            }
            break;
#endif

#ifdef _PRE_WLAN_RF_AUTOCALI
        case OAM_DATA_TYPE_AUTOCALI:
            if (oam_wal_func_hook.p_wal_recv_autocali_data_func != OAL_PTR_NULL) {
                l_ret = oam_wal_func_hook.p_wal_recv_autocali_data_func(&puc_data[SDT_DEV_NAME_INDEX],
                                                                        (oal_uint16)i_len);
            }
            break;
#endif

        default:
            OAL_IO_PRINT("sdt_drv_send_data_to_wifi::cmd is invalid!!-->%d\n", pc_buf[1]);
            break;
    }
#if (_PRE_OS_VERSION_RAW != _PRE_OS_VERSION)
    oal_mem_sdt_netbuf_free(pst_netbuf, OAL_TRUE);
#endif
    return l_ret;
}

/*
 * �� �� ��  : sdt_drv_netlink_send
 * ��������  : SDT������ͨ��netlink��SDT APP��������
 *             ��¼�����͵������뷢��ʧ�ܵ����ݣ�ͨ�����кż�¼
 */
oal_uint32 sdt_drv_netlink_send(oal_netbuf_stru *pst_netbuf, oal_uint32 ul_len)
{
#if (_PRE_OS_VERSION_RAW != _PRE_OS_VERSION)
    oal_netbuf_stru *pst_copy_netbuf = NULL;
    oal_nlmsghdr_stru *pst_nlhdr = NULL;
    oal_uint32 ul_nlmsg_len;
    oal_int32 l_unicast_bytes;
    oal_int32 ret;

    /* ���ϲ㱣֤�����ǿ� */
    /* ���û����app�������ӣ���ֱ�ӷ��أ�ÿ500�δ�ӡһ����ʾ��Ϣ */
    if (g_sdt_drv_mng_entry.ul_usepid == 0) {
        if ((oal_atomic_read(&g_sdt_drv_mng_entry.ul_unconnect_cnt) % SDT_DRV_REPORT_NO_CONNECT_FREQUENCE) == 0) {
            OAL_IO_PRINT("Info:waitting app_sdt start...\r\n");
            oal_atomic_inc(&g_sdt_drv_mng_entry.ul_unconnect_cnt);
        }

        oal_mem_sdt_netbuf_free(pst_netbuf, OAL_TRUE);

        return OAL_FAIL;
    }

    /* ��дnetlink��Ϣͷ */
    ul_nlmsg_len = OAL_NLMSG_SPACE(ul_len);
    pst_copy_netbuf = oal_netbuf_alloc(ul_nlmsg_len, 0, WLAN_MEM_NETBUF_ALIGN);
    if (OAL_UNLIKELY(pst_copy_netbuf == OAL_PTR_NULL)) {
        oal_mem_sdt_netbuf_free(pst_netbuf, OAL_TRUE);
        OAL_IO_PRINT("oal_netbuf_alloc failed. \r\n");
        return OAL_FAIL;
    }

    pst_nlhdr = oal_nlmsg_put(pst_copy_netbuf, 0, 0, 0, (oal_int32)ul_len, 0);
    ret = memcpy_s((oal_void *)OAL_NLMSG_DATA(pst_nlhdr), ul_len,
                   (const oal_void *)oal_netbuf_data(pst_netbuf), ul_len);
    if (ret != EOK) {
        oal_mem_sdt_netbuf_free(pst_netbuf, OAL_TRUE);
        oal_netbuf_free(pst_copy_netbuf);
        OAL_IO_PRINT("sdt_drv_netlink_send::memcpy_s failed. \r\n");
        return OAL_FAIL;
    }

    l_unicast_bytes = oal_netlink_unicast(g_sdt_drv_mng_entry.pst_nlsk, pst_copy_netbuf,
                                          g_sdt_drv_mng_entry.ul_usepid, OAL_MSG_DONTWAIT);

    oal_mem_sdt_netbuf_free(pst_netbuf, OAL_TRUE);

    OAM_SDT_STAT_INCR(ul_nlk_sd_cnt);
    if (l_unicast_bytes <= 0) {
        OAM_SDT_STAT_INCR(ul_nlk_sd_fail);
        return OAL_FAIL;
    }
#endif
    return OAL_SUCC;
}

/*
 * �� �� ��  : sdt_drv_netlink_recv
 * ��������  : ��SDT APP����յ�����
 */
oal_void sdt_drv_netlink_recv(oal_netbuf_stru *pst_netbuf)
{
    oal_nlmsghdr_stru *pst_nlhdr = OAL_PTR_NULL;
    sdt_drv_netlink_msg_hdr_stru st_msg_hdr;
    oal_uint32 ul_len;
    oal_int32 ret;

#ifdef _PRE_PRODUCT_ID_HI110X_HOST
    OAL_IO_PRINT("sdt_drv_netlink_recv::recv oam_hisi message!\n");
#endif

    if (pst_netbuf == OAL_PTR_NULL) {
        OAL_IO_PRINT("sdt_drv_netlink_recv::pst_netbuf is null!\n");
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{sdt_drv_netlink_recv::pst_netbuf is null.}");
        return;
    }
    memset_s(g_sdt_drv_mng_entry.puc_data, DATA_BUF_LEN, 0, DATA_BUF_LEN);
    memset_s(&st_msg_hdr, OAL_SIZEOF(sdt_drv_netlink_msg_hdr_stru), 0, OAL_SIZEOF(sdt_drv_netlink_msg_hdr_stru));
    if (OAL_NETBUF_LEN(pst_netbuf) >= OAL_NLMSG_SPACE(0)) {
        pst_nlhdr = oal_nlmsg_hdr((OAL_CONST oal_netbuf_stru *)pst_netbuf);
        /* �Ա��ĳ��Ƚ��м�� */
        if (!OAL_NLMSG_OK(pst_nlhdr, OAL_NETBUF_LEN(pst_netbuf))) {
            OAL_IO_PRINT("sdt_drv_netlink_recv::invaild netlink buff data packge data len = :%u,skb_buff data len = %u\n",
                         pst_nlhdr->nlmsg_len, OAL_NETBUF_LEN(pst_netbuf));
            OAM_ERROR_LOG2(0, OAM_SF_ANY,
                           "{sdt_drv_netlink_recv::[ERROR]invaild netlink buff data packge data len = :%u,skb_buff data len = %u\n.}",
                           pst_nlhdr->nlmsg_len, OAL_NETBUF_LEN(pst_netbuf));
            return;
        }
        ul_len = OAL_NLMSG_PAYLOAD(pst_nlhdr, 0);
        /* ������Ҫ����OAL_SIZEOF(st_msg_hdr)���ж�֮ */
        if (ul_len <= DATA_BUF_LEN && ul_len >= (oal_uint32)OAL_SIZEOF(st_msg_hdr)) {
            ret = memcpy_s((oal_void *)g_sdt_drv_mng_entry.puc_data, DATA_BUF_LEN,
                           (const oal_void *)OAL_NLMSG_DATA(pst_nlhdr),
                           ul_len);
            if (ret < 0) {
                OAL_IO_PRINT("sdt_drv_netlink_recv::memcpy_s failed, ret = %d\n", ret);
                return;
            }
        } else {
            /* overflow */
            OAL_IO_PRINT("sdt_drv_netlink_recv::invaild netlink buff len:%u,max len:%u\n", ul_len, DATA_BUF_LEN);
            OAM_ERROR_LOG2(0, OAM_SF_ANY, "{sdt_drv_netlink_recv::invaild netlink buff len:%u,max len:%u\n.}",
                           ul_len, DATA_BUF_LEN);
            return;
        }
        ret = memcpy_s((oal_void *)&st_msg_hdr, (oal_uint32)OAL_SIZEOF(st_msg_hdr),
                       (const oal_void *)g_sdt_drv_mng_entry.puc_data,
                       (oal_uint32)OAL_SIZEOF(st_msg_hdr));
        if (ret < 0) {
            OAL_IO_PRINT("sdt_drv_netlink_recv::memcpy_s failed, ret = %d\n", ret);
            return;
        }

        if (st_msg_hdr.ul_cmd == NETLINK_MSG_HELLO) {
            g_sdt_drv_mng_entry.ul_usepid = pst_nlhdr->nlmsg_pid; /* pid of sending process */
            OAL_IO_PRINT("sdt_drv_netlink_recv::%s pid is-->%d \n", OAL_FUNC_NAME, g_sdt_drv_mng_entry.ul_usepid);
        } else {
            OAL_IO_PRINT("sdt_drv_netlink_recv::user mode not accept msg except hello from sdt!\n");
        }
    }
}

/*
 * �� �� ��  : sdt_drv_netlink_create
 * ��������  : ����netlink
 */
oal_int32 sdt_drv_netlink_create(oal_void)
{
    g_sdt_drv_mng_entry.pst_nlsk = oal_netlink_kernel_create(&OAL_INIT_NET, NETLINK_TEST,
                                                           0, sdt_drv_netlink_recv,
                                                           OAL_PTR_NULL, OAL_THIS_MODULE);
    if (g_sdt_drv_mng_entry.pst_nlsk == OAL_PTR_NULL) {
        OAL_IO_PRINT("sdt_drv_netlink_create return fail!\n");
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : sdt_drv_push_wifi_log_work
 * ��������  : oam rx wifi skb log data
 */
oal_void sdt_drv_push_wifi_log_work(oal_work_stru *work)
{
    oal_netbuf_stru *pst_netbuf;

    pst_netbuf = sdt_drv_netbuf_delist();

    while (pst_netbuf != OAL_PTR_NULL) {
        sdt_drv_netlink_send(pst_netbuf, OAL_NETBUF_LEN(pst_netbuf));

        pst_netbuf = sdt_drv_netbuf_delist();
    }
    return;
}

/*
 * �� �� ��  : sdt_drv_func_hook_init
 * ��������  : sdt����ģ�鹳�Ӻ�����ʼ��
 */
oal_void sdt_drv_func_hook_init(oal_void)
{
    g_sdt_drv_func_hook.p_sdt_report_data_func = sdt_drv_report_data2app;
    g_sdt_drv_func_hook.p_sdt_get_wq_len_func = sdt_drv_get_wq_len;
}

/*
 * �� �� ��  : sdt_drv_main_init
 * ��������  : sdt_drv��ʼ������
 */
oal_int32 sdt_drv_main_init(oal_void)
{
    oal_int32 l_nl_return_val;

    memset_s((void *)&g_sdt_drv_mng_entry, OAL_SIZEOF(g_sdt_drv_mng_entry),
             0, OAL_SIZEOF(g_sdt_drv_mng_entry));

    g_sdt_drv_mng_entry.puc_data = oal_memalloc(DATA_BUF_LEN);
    if (g_sdt_drv_mng_entry.puc_data == OAL_PTR_NULL) {
        OAL_IO_PRINT("alloc g_sdt_drv_mng_entry.puc_data fail!\n");
        return -OAL_EFAIL;
    }

    memset_s(g_sdt_drv_mng_entry.puc_data, DATA_BUF_LEN, 0, DATA_BUF_LEN);

    l_nl_return_val = sdt_drv_netlink_create();
    if (l_nl_return_val < 0) {
        OAL_IO_PRINT("sdt_drv_main_init::create netlink returns fail! l_nl_return_val--> \
                      %d\n",
                     l_nl_return_val);
        return -l_nl_return_val;
    }

    OAL_INIT_WORK(&g_sdt_drv_mng_entry.rx_wifi_work, sdt_drv_push_wifi_log_work);
    oal_spin_lock_init(&g_sdt_drv_mng_entry.st_spin_lock);
    oal_netbuf_list_head_init(&g_sdt_drv_mng_entry.rx_wifi_dbg_seq);

    /* sdtģ�鹳�Ӻ�����ʼ�� */
    sdt_drv_func_hook_init();

    /* ��sdt���Ӻ���ע����oamģ�� */
    oam_sdt_func_fook_register(&g_sdt_drv_func_hook);

    /* sdt��������֮�������ʽ��ΪOAM_OUTPUT_TYPE_SDT */
    if (oam_set_output_type(OAM_OUTPUT_TYPE_SDT) != OAL_SUCC) {
        OAL_IO_PRINT("oam set output type fail!");
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}

/*
 * �� �� ��  : sdt_drv_main_exit
 * ��������  : sdt_drvж��
 */
oal_void sdt_drv_main_exit(oal_void)
{
    oam_sdt_func_fook_unregister();

    if (g_sdt_drv_mng_entry.pst_nlsk != OAL_PTR_NULL) {
        oal_netlink_kernel_release(g_sdt_drv_mng_entry.pst_nlsk);
    }

    if (g_sdt_drv_mng_entry.puc_data != OAL_PTR_NULL) {
        oal_free(g_sdt_drv_mng_entry.puc_data);
    }

    oal_cancel_work_sync(&g_sdt_drv_mng_entry.rx_wifi_work);
    oal_netbuf_queue_purge(&g_sdt_drv_mng_entry.rx_wifi_dbg_seq);

    return;
}

/*lint -e578*/ /*lint -e19*/
oal_module_symbol(sdt_drv_main_init);
oal_module_symbol(sdt_drv_main_exit);

oal_module_license("GPL");
