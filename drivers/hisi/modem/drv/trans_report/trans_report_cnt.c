#include "bsp_hds_ind.h"
#include <mdrv_diag.h>
#ifdef CONFIG_BALONG_ESPE
#include "bsp_espe.h"
#endif
#ifdef CONFIG_MAA_BALONG
#include "bsp_maa.h"
#endif

#include <osl_types.h>
#include "../wan/wan.h"



#include "bsp_trans_report.h"

struct wan_trans_s {
    int transreport_error;
    int get_dbg_info_error;
    struct timer_list trans_timer;
};

#define WAN_TRANS_TIMEOUT 200 /* ms unit */
#define HDS_TRANS_RE_SUCC (0x00abcd00 + 0x5)

struct wan_trans_s g_wan_trans;
#define WAN_SET_TRANS_TIMEOUT() (jiffies + msecs_to_jiffies(WAN_TRANS_TIMEOUT))


#ifdef CONFIG_MAA_BALONG
struct maa_debug_info g_maa_dbg_info;
#endif

#ifdef CONFIG_BALONG_ESPE
struct espe_om_info g_epse_dbg_info;
#endif

#ifdef CONFIG_WAN
struct wan_debug_info_s g_wan_dbg_info;
#endif


void bsp_wan_transreport(unsigned long data)
{
    struct wan_trans_s *wan_trans = (struct wan_trans_s *)((uintptr_t)data);
    int ret, size;
    struct trans_diag_info sTransInd = {0};

    sTransInd.ulModule = 0xf0900;
    sTransInd.ulPid = 0x8003;

#ifdef CONFIG_MAA_BALONG
    sTransInd.ulMsgId = TRANS_MSGID_MAA;
    size = bsp_maa_hds_transreport(&g_maa_dbg_info);
    if (size >= 0) {
        sTransInd.ulLength = sizeof(struct maa_debug_info);
        sTransInd.pData = (void *)(&g_maa_dbg_info);
        ret = bsp_trans_report_trans(&sTransInd);
        if (ret != HDS_TRANS_RE_SUCC) {
            wan_trans->transreport_error++;
        }
    } else {
        wan_trans->get_dbg_info_error++;
    }
#endif

#ifdef CONFIG_BALONG_ESPE
    sTransInd.ulMsgId = TRANS_MSGID_SPE;
    size = bsp_espe_get_om_info(&g_epse_dbg_info);
    if (size >= 0) {
        sTransInd.ulLength = size;
        sTransInd.pData = (void *)(&g_epse_dbg_info);
        ret = bsp_trans_report_trans(&sTransInd);
        if (ret != HDS_TRANS_RE_SUCC) {
            wan_trans->transreport_error++;
        }
    } else {
        wan_trans->get_dbg_info_error++;
    }

#endif

#ifdef CONFIG_WAN
    sTransInd.ulMsgId = TRANS_MSGID_WAN;
    size = wan_transreport(&g_wan_dbg_info);
    if (size >= 0) {
        sTransInd.ulLength = sizeof(struct wan_debug_info_s);
        sTransInd.pData = (void *)(&g_wan_dbg_info);
        ret = bsp_trans_report_trans(&sTransInd);
        if (ret != HDS_TRANS_RE_SUCC) {
            wan_trans->transreport_error++;
        }
    } else {
        wan_trans->get_dbg_info_error++;
    }
#endif



    mod_timer(&wan_trans->trans_timer, WAN_SET_TRANS_TIMEOUT());
    return;
}

void wan_transreport_init_timer(void)
{
    /* init timer */
    struct wan_trans_s *wan_trans = &g_wan_trans;
    init_timer(&wan_trans->trans_timer);
    wan_trans->trans_timer.function = bsp_wan_transreport;
    wan_trans->trans_timer.data = (uintptr_t)wan_trans;
    wan_trans->trans_timer.expires = WAN_SET_TRANS_TIMEOUT();
    add_timer(&wan_trans->trans_timer);
}
