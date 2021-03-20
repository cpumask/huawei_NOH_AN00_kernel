

#ifdef _PRE_MEM_TRACE

#include "oal_list.h"
#include "oal_schedule.h"
#include "mem_trace.h"

// note: ���������궨��ֵ��Ҫ����ʵ����Ҫ���ٵ�ģ�����޸ģ������ػ�����Լ��
/*
 * @brief �˺궨���ʾĳģ��������ʱ��Ҫ���ٵĶ�̬������ڴ�ڵ�����ֵ
 *        Ϊ��ģ���ڵ��߼����㣬��ֵ���붨��Ϊ2��N�η���(N>=5)
 */
#define MEM_TRACE_TBL_SIZE 8192  // ��Ҫ���ݳ�פ�ڴ����ˮλ����

/* @brief �˺궨���ʾĳģ��������ʱ��Ҫ���ٵĶ�̬������ڴ�ڵ����ڵ����ֵ */
#define MEM_TRACE_OWNER_TBL_SIZE 64
/*
 * ��ģ�����ĵ��ڴ���Դ = 44*MEM_TRACE_TBL_SIZE
 *                      + 20
 *                      + 20*MEM_TRACE_OWNER_TBL_SIZE
 *                      + 8*(MEM_TRACE_TBL_SIZE>>5)
 *                      + sizeof(oal_spin_lock) = 178K
 */
// ÿ�������ٵ��ڴ�����Ϣ
typedef struct {
    uintptr_t ul_addr;                // �ڴ���ַ
    uint32_t ul_time;               // �ڴ�����ʱ���
    uint32_t ul_fileid;             // ������ڴ����ļ�ID
    uint32_t ul_linenum;            // ������ڴ��Ĵ����к�
    uint32_t ul_last_trace_fileid;  // ���ڴ�����һ�εĸ��ٵ�
    uint32_t ul_last_trace_line;    // ���ڴ�����һ�εĸ��ٵ�
    uint32_t ul_last_trace_time;    // ���ڴ�����һ�εĸ���ʱ���
    oal_dlist_head_stru st_list_entry;
    oal_dlist_head_stru st_hash_list_entry;
    oal_dlist_head_stru st_owner_list_entry;
} mem_trace_node;

// �ڴ��������Ϣ
typedef struct _mem_node_owner {
    uint32_t ul_fileid;   // ������ڴ����ļ�ID
    uint32_t ul_linenum;  // ������ڴ��Ĵ����к�
    oal_dlist_head_stru st_owner_list_head;
    uint32_t ul_cnt;
} mem_node_owner;

#define MEM_TRACE_HASH_TBL_SIZE      (MEM_TRACE_TBL_SIZE >> 5)
#define MEM_TRACE_HASH_TBL_SIZE_MASK (MEM_TRACE_HASH_TBL_SIZE - 1)

// �ڴ����ģ��Ĺ���
typedef struct {
    mem_trace_node ast_mem_trace_tbl[MEM_TRACE_TBL_SIZE];
    oal_dlist_head_stru st_free_list_head;
    oal_dlist_head_stru st_used_list_head;
    oal_dlist_head_stru ast_hast_table[MEM_TRACE_HASH_TBL_SIZE];  // ���������ڵ���ٶ�
    mem_node_owner ast_owner[MEM_TRACE_OWNER_TBL_SIZE];
    oal_spin_lock_stru st_spin_lock;
    uint32_t ul_cnt;
    uint32_t ul_node_shortage;  // ������node�����õ�����
} mem_trace_mgmt;

OAL_STATIC mem_trace_mgmt mem_trace_mgmt;

/*
 * �� �� ��  : mem_trace_get_mgr
 * ��������  : ��ȡ���Ľṹ�塣ά��֮�ã����Է���ֵ���Ϸ���У��
 */
OAL_STATIC OAL_INLINE mem_trace_mgmt *mem_trace_get_mgr(void)
{
    return &mem_trace_mgmt;
}

/*
 * �� �� ��  : mem_trace_init
 * ��������  : ��ʼ��mem_traceģ��
 */
void mem_trace_init(void)
{
    uint32_t i;
    mem_trace_mgmt *pst_mgr = mem_trace_get_mgr();

    memset_s((void *)pst_mgr, OAL_SIZEOF(mem_trace_mgmt), 0, OAL_SIZEOF(mem_trace_mgmt));
    oal_spin_lock_init(&pst_mgr->st_spin_lock);
    oal_dlist_init_head(&pst_mgr->st_free_list_head);
    oal_dlist_init_head(&pst_mgr->st_used_list_head);
    pst_mgr->ul_cnt = 0;

    for (i = 0; i < MEM_TRACE_HASH_TBL_SIZE; i++) {
        oal_dlist_init_head(&pst_mgr->ast_hast_table[i]);
    }

    // ��ʼ��ʱ�����нڵ㶼����free����
    for (i = 0; i < MEM_TRACE_TBL_SIZE; i++) {
        oal_dlist_add_tail(&pst_mgr->ast_mem_trace_tbl[i].st_list_entry, &pst_mgr->st_free_list_head);
    }

    for (i = 0; i < MEM_TRACE_OWNER_TBL_SIZE; i++) {
        pst_mgr->ast_owner[i].ul_cnt = 0;
        pst_mgr->ast_owner[i].ul_fileid = ~0;
        pst_mgr->ast_owner[i].ul_linenum = 0;
        oal_dlist_init_head(&pst_mgr->ast_owner[i].st_owner_list_head);
    }

    oal_io_print("mem_trace_init done!\r\n");
}

/*
 * �� �� ��  : mem_trace_exit
 * ��������  : �˳�mem_traceģ��
 */
void mem_trace_exit(void)
{
    // nothing todo
}

/*
 * �� �� ��  : mem_trace_owner_search
 * ��������  : �������ڴ�ڵ��������ĸ�ģ������ģ����û���ѵ�����һ���յ�owner����ƫ��
 */
OAL_STATIC mem_node_owner *mem_trace_owner_search(uint32_t ul_fileid, uint32_t ul_linenum)
{
    mem_trace_mgmt *pst_mgr = mem_trace_get_mgr();
    mem_node_owner *pst_owner;

    for (pst_owner = pst_mgr->ast_owner + 0; pst_owner < pst_mgr->ast_owner + MEM_TRACE_OWNER_TBL_SIZE; pst_owner++) {
        // �ҵ�����owner�����߼���owner
        if (((pst_owner->ul_fileid == 0xffffffff) && (pst_owner->ul_linenum == 0)) ||
            ((pst_owner->ul_fileid == ul_fileid) && (pst_owner->ul_linenum == ul_linenum))) {
            return pst_owner;
        }
    }

    return OAL_PTR_NULL;
}

/*
 * �� �� ��  : mem_trace_add_node
 * ��������  : �Ǽ�������Ҫ�����ͷŵ��ڴ�ڵ�(������̫������skb�Լ���������������ڴ�)
 *             ע: û�п�����̫������skb���������������
 */
void __mem_trace_add_node(uintptr_t ul_addr,
                          uint32_t ul_fileid,
                          uint32_t ul_linenum)
{
    mem_trace_node *pst_mem_trace_node = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    oal_uint ul_irq_save;
    mem_node_owner *pst_owner = OAL_PTR_NULL;
    mem_trace_mgmt *pst_mgr = mem_trace_get_mgr();

    oal_spin_lock_irq_save(&pst_mgr->st_spin_lock, &ul_irq_save);
    if (oal_dlist_is_empty(&pst_mgr->st_free_list_head)) {
        oal_io_print("mem trace mod not initialized or the trace space not enough!!!\n");
        pst_mgr->ul_node_shortage = OAL_TRUE;
        oal_spin_unlock_irq_restore(&pst_mgr->st_spin_lock, &ul_irq_save);
        return;
    }
    // ��free����ͷ��ȡ����free����β�����գ���ȷ��free�����нڵ���ʷ��
    pst_entry = oal_dlist_delete_head(&(pst_mgr->st_free_list_head));
    oal_dlist_add_tail(pst_entry, &(pst_mgr->st_used_list_head));

    pst_mem_trace_node = oal_dlist_get_entry(pst_entry, mem_trace_node, st_list_entry);
    pst_mem_trace_node->ul_addr = ul_addr;
    pst_mem_trace_node->ul_time = (uint32_t)OAL_TIME_JIFFY;
    pst_mem_trace_node->ul_last_trace_time = pst_mem_trace_node->ul_time;
    pst_mem_trace_node->ul_fileid = ul_fileid;
    pst_mem_trace_node->ul_linenum = ul_linenum;
    pst_mem_trace_node->ul_last_trace_fileid = ul_fileid;
    pst_mem_trace_node->ul_last_trace_line = ul_linenum;

    oal_dlist_add_tail(&(pst_mem_trace_node->st_hash_list_entry),
                       &(pst_mgr->ast_hast_table[ul_addr & MEM_TRACE_HASH_TBL_SIZE_MASK]));
    pst_mgr->ul_cnt++;
    pst_owner = mem_trace_owner_search(ul_fileid, ul_linenum);
    if (pst_owner != OAL_PTR_NULL) {
        pst_owner->ul_fileid = ul_fileid;
        pst_owner->ul_linenum = ul_linenum;
        pst_owner->ul_cnt++;
        oal_dlist_add_tail(&(pst_mem_trace_node->st_owner_list_entry), &pst_owner->st_owner_list_head);
    } else {
        oal_warn_on(1);
    }

    oal_spin_unlock_irq_restore(&pst_mgr->st_spin_lock, &ul_irq_save);
}

/*
 * �� �� ��  : mem_trace_delete_node
 * ��������  : ע�������ͷŵ��ڴ�ڵ�(����������̫��skb�Լ����������ͷŵ��ڴ�)
 */
void __mem_trace_delete_node(uintptr_t ul_addr,
                             uint32_t ul_fileid,
                             uint32_t ul_linenum)
{
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry_tmp = OAL_PTR_NULL;
    mem_trace_node *pst_mem_trace_node = OAL_PTR_NULL;
    oal_bool_enum_uint8 en_match_flag = OAL_FALSE;
    oal_uint ul_irq_save;
    mem_node_owner *pst_owner = OAL_PTR_NULL;
    mem_trace_mgmt *pst_mgr = mem_trace_get_mgr();

    oal_spin_lock_irq_save(&pst_mgr->st_spin_lock, &ul_irq_save);

    oal_dlist_search_for_each_safe(pst_entry, pst_entry_tmp,
                                   &(pst_mgr->ast_hast_table[ul_addr & MEM_TRACE_HASH_TBL_SIZE_MASK]))
    {
        pst_mem_trace_node = oal_dlist_get_entry(pst_entry, mem_trace_node, st_hash_list_entry);
        if (pst_mem_trace_node->ul_addr == ul_addr) {
            oal_dlist_delete_entry(pst_entry);  // ��hash����ɾ��
            pst_mem_trace_node->ul_last_trace_time = (uint32_t)OAL_TIME_JIFFY;
            pst_mem_trace_node->ul_last_trace_fileid = ul_fileid;
            pst_mem_trace_node->ul_last_trace_line = ul_linenum;
            en_match_flag = OAL_TRUE;
            break;
        }
    }

    if (en_match_flag) {
        pst_mgr->ul_cnt--;
        oal_dlist_delete_entry(&(pst_mem_trace_node->st_list_entry));                           // ��used ����ɾ��
        oal_dlist_add_tail(&(pst_mem_trace_node->st_list_entry), &pst_mgr->st_free_list_head);  // ���ӵ�free����
        pst_owner = mem_trace_owner_search(pst_mem_trace_node->ul_fileid, pst_mem_trace_node->ul_linenum);
        if (pst_owner != OAL_PTR_NULL) {
            oal_dlist_delete_entry(&(pst_mem_trace_node->st_owner_list_entry));  // ��owner����ɾ��
            pst_owner->ul_cnt--;
        } else {
            oal_io_print("%s error:pst_owner is null\n", __FUNCTION__);
        }
    } else if (!pst_mgr->ul_node_shortage) {
        // �ͷ�һ��û�еǼǵ��ڴ�
        oal_bool_enum_uint8 en_ever_del = OAL_FALSE;
        oal_dlist_search_for_each(pst_entry, &pst_mgr->st_free_list_head)
        {
            pst_mem_trace_node = oal_dlist_get_entry(pst_entry, mem_trace_node, st_list_entry);
            if (pst_mem_trace_node->ul_addr == ul_addr) {
                oal_io_print("0x%lx add@[%d:%d:%u] del@[%d:%d:%u]\n",
                             ul_addr,
                             pst_mem_trace_node->ul_fileid, pst_mem_trace_node->ul_linenum,
                             pst_mem_trace_node->ul_time,
                             pst_mem_trace_node->ul_last_trace_fileid,
                             pst_mem_trace_node->ul_last_trace_line,
                             pst_mem_trace_node->ul_last_trace_time);
                en_ever_del = OAL_TRUE;
            }
        }

        if (en_ever_del) {
            oal_io_print("maybe double-free 0x%lx@[%d:%d] time=%u, see last above\n---cut---\n",
                         ul_addr, ul_fileid, ul_linenum, (uint32_t)OAL_TIME_JIFFY);
        } else {
            oal_io_print("delete node not registered, addr=0x%lx)\n", ul_addr);
            oal_warn_on(1);
        }
    }

    oal_spin_unlock_irq_restore(&pst_mgr->st_spin_lock, &ul_irq_save);
}

/*
 * �� �� ��  : mem_trace_probe
 * ��������  : ��ĳ��·������һ���ڴ���ٵ�̽��
 */
void __mem_trace_probe(uintptr_t ul_addr,
                       uint32_t ul_probe_fileid,
                       uint32_t ul_probe_line)
{
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    mem_trace_node *pst_mem_trace_node = OAL_PTR_NULL;
    oal_bool_enum_uint8 en_match_flag = OAL_FALSE;
    oal_uint ul_irq_save;
    mem_trace_mgmt *pst_mgr = mem_trace_get_mgr();

    oal_spin_lock_irq_save(&pst_mgr->st_spin_lock, &ul_irq_save);

    oal_dlist_search_for_each(pst_entry, &(pst_mgr->ast_hast_table[ul_addr & MEM_TRACE_HASH_TBL_SIZE_MASK]))
    {
        pst_mem_trace_node = oal_dlist_get_entry(pst_entry, mem_trace_node, st_hash_list_entry);
        if (pst_mem_trace_node->ul_addr == ul_addr) {
            pst_mem_trace_node->ul_last_trace_fileid = ul_probe_fileid;
            pst_mem_trace_node->ul_last_trace_line = ul_probe_line;
            pst_mem_trace_node->ul_last_trace_time = (uint32_t)OAL_TIME_JIFFY;
            en_match_flag = OAL_TRUE;
            break;
        }
    }

    oal_spin_unlock_irq_restore(&pst_mgr->st_spin_lock, &ul_irq_save);

    if (!en_match_flag) {
        if (!pst_mgr->ul_node_shortage) {
            // �Ҳ������ڴ��
            oal_io_print("the node(0x%p) maybe not register!\n", (void *)ul_addr);
            mem_trace_info_show(MEM_TRACE_INFO_MODE2, 0, 0);
            mem_trace_info_show(MEM_TRACE_INFO_MODE1, 0, 0);
            oal_warn_on(1);
        }
    }
}

/*
 * �� �� ��  : mem_trace_info_show
 * ��������  : ��ʾ׷����Ϣ
 */
void mem_trace_info_show(uint32_t ul_mode, uint32_t ul_fileid, uint32_t ul_line)
{
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    mem_trace_node *pst_mem_trace_node = OAL_PTR_NULL;
    uint32_t i;
    oal_uint ul_irq_save;
    mem_node_owner *pst_owner = OAL_PTR_NULL;
    mem_trace_mgmt *pst_mgr = mem_trace_get_mgr();

    oal_spin_lock_irq_save(&pst_mgr->st_spin_lock, &ul_irq_save);

    oal_io_print("==============show mem trace info(mode=%d) shortage=%d:==================\r\n",
                 ul_mode, pst_mgr->ul_node_shortage);

    if (ul_mode == MEM_TRACE_INFO_MODE0) {
        oal_io_print("    Addr    | File_id | Line |    Time    | trace_file | trace_line | Last_time\r\n");
        pst_owner = mem_trace_owner_search(ul_fileid, ul_line);

        if (pst_owner != OAL_PTR_NULL) {
            if (pst_owner->ul_fileid != ul_fileid) {
                oal_io_print("no mem trace owner find!\r\n");
            } else {
                oal_dlist_search_for_each(pst_entry, &(pst_owner->st_owner_list_head))
                {
                    pst_mem_trace_node = oal_dlist_get_entry(pst_entry, mem_trace_node, st_owner_list_entry);
                    oal_io_print("0x%p |%8u | %4u | %10u | %10u | %10u | %10u\r\n",
                                 (void *)pst_mem_trace_node->ul_addr,
                                 pst_mem_trace_node->ul_fileid,
                                 pst_mem_trace_node->ul_linenum,
                                 pst_mem_trace_node->ul_time,
                                 pst_mem_trace_node->ul_last_trace_fileid,
                                 pst_mem_trace_node->ul_last_trace_line,
                                 pst_mem_trace_node->ul_last_trace_time);
                }
            }
        } else {
            oal_io_print("%s error:pst_owner is null\n", __FUNCTION__);
        }
    } else if (ul_mode == MEM_TRACE_INFO_MODE1) {
        oal_io_print(" File_id | Line | Cnt \r\n");

        for (i = 0; i < MEM_TRACE_OWNER_TBL_SIZE; i++) {
            if ((pst_mgr->ast_owner[i].ul_fileid == 0xffffffff)
                && (pst_mgr->ast_owner[i].ul_linenum == 0)) {
                break;
            } else {
                oal_io_print("%8u | %4u | %4u\r\n",
                             pst_mgr->ast_owner[i].ul_fileid,
                             pst_mgr->ast_owner[i].ul_linenum,
                             pst_mgr->ast_owner[i].ul_cnt);
            }
        }
    } else if (ul_mode == MEM_TRACE_INFO_MODE2) {
        oal_io_print(" current mem trace cnt:%d\r\n", pst_mgr->ul_cnt);
    }
    oal_spin_unlock_irq_restore(&pst_mgr->st_spin_lock, &ul_irq_save);

    oal_io_print("========================End=====================\r\n");
}

oal_module_symbol(__mem_trace_add_node);
oal_module_symbol(__mem_trace_delete_node);
oal_module_symbol(__mem_trace_probe);
oal_module_symbol(mem_trace_info_show);

#endif /* #ifdef _PRE_MEM_TRACE */
