

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE

#define HISI_LOG_TAG "[PCIEF]"
#include "oal_util.h"

#include "oal_net.h"
#include "oal_ext_if.h"

#include "oam_ext_if.h"

#include "oal_pcie_host.h"
#include "oal_pcie_linux.h"
#include "oal_pcie_pm.h"
#include "oal_hcc_host_if.h"
#include "securec.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_OAL_PCIE_FIRMWARE_C

#define STR_WRITEM_OK   "WRITEM OK:"
#define STR_FILES_READY "READY:FILE"
#define STR_FILES_OK    "FILES OK:"

#define COMPART_CHAR ' '

#define FILES_CMD_SEND 1
#define FILES_BIN_SEND 2

#define FIRMWARE_MAXPARAMNUM 16
#define FIRMWARECMD_NAME_LEN 15

typedef int32_t (*FIRMWARE_CMDSENDFUNC)(oal_pcie_res *pst_pcie_res, uint8_t *buff, int32_t len);
typedef int32_t (*FIRMWARE_CMDREADFUNC)(oal_pcie_res *pst_pcie_res, uint8_t *buff,
                                        int32_t len, uint32_t timeout);

typedef struct FirmwareCmd {
    char acCmdName[FIRMWARECMD_NAME_LEN];
    uint8_t ucArgs;
    FIRMWARE_CMDSENDFUNC pCmdSend;
    FIRMWARE_CMDREADFUNC pCmdRead;
} FIRMWARECMD_STRU;

OAL_STATIC int32_t oal_pcie_firmware_cmd_send_quit(oal_pcie_res *pst_pcie_res, uint8_t *buff, int32_t len);
OAL_STATIC int32_t oal_pcie_firmware_cmd_read_quit(oal_pcie_res *pst_pcie_res, uint8_t *buff,
                                                   int32_t len, uint32_t timeout);
OAL_STATIC int32_t oal_pcie_firmware_cmd_send_files(oal_pcie_res *pst_pcie_res, uint8_t *buff, int32_t len);
OAL_STATIC int32_t oal_pcie_firmware_cmd_read_files(oal_pcie_res *pst_pcie_res, uint8_t *buff,
                                                    int32_t len, uint32_t timeout);
OAL_STATIC int32_t oal_pcie_firmware_cmd_send_version(oal_pcie_res *pst_pcie_res, uint8_t *buff, int32_t len);
OAL_STATIC int32_t oal_pcie_firmware_cmd_read_version(oal_pcie_res *pst_pcie_res, uint8_t *buff,
                                                      int32_t len, uint32_t timeout);
OAL_STATIC int32_t oal_pcie_firmware_cmd_send_writem(oal_pcie_res *pst_pcie_res, uint8_t *buff, int32_t len);
OAL_STATIC int32_t oal_pcie_firmware_cmd_read_writem(oal_pcie_res *pst_pcie_res, uint8_t *buff,
                                                     int32_t len, uint32_t timeout);
OAL_STATIC int32_t oal_pcie_firmware_cmd_send_readm(oal_pcie_res *pst_pcie_res, uint8_t *buff, int32_t len);
OAL_STATIC int32_t oal_pcie_firmware_cmd_read_readm(oal_pcie_res *pst_pcie_res, uint8_t *buff,
                                                    int32_t len, uint32_t timeout);
OAL_STATIC int32_t oal_pcie_firmware_cmd_send_jump(oal_pcie_res *pst_pcie_res, uint8_t *buff, int32_t len);
OAL_STATIC int32_t oal_pcie_firmware_cmd_read_jump(oal_pcie_res *pst_pcie_res, uint8_t *buff,
                                                   int32_t len, uint32_t timeout);

OAL_STATIC const FIRMWARECMD_STRU g_firmware_cmd_table[] = {
    {   "QUIT",
        0,
        oal_pcie_firmware_cmd_send_quit,
        oal_pcie_firmware_cmd_read_quit
    },
    {   "FILES",
        1,
        oal_pcie_firmware_cmd_send_files,
        oal_pcie_firmware_cmd_read_files
    },
    {   "VERSION",
        0,
        oal_pcie_firmware_cmd_send_version,
        oal_pcie_firmware_cmd_read_version
    },
    {   "WRITEM",
        3,
        oal_pcie_firmware_cmd_send_writem,
        oal_pcie_firmware_cmd_read_writem
    },
    {   "READM",
        2,
        oal_pcie_firmware_cmd_send_readm,
        oal_pcie_firmware_cmd_read_readm
    },
    {   "JUMP",
        1,
        oal_pcie_firmware_cmd_send_jump,
        oal_pcie_firmware_cmd_read_jump
    },
};

OAL_STATIC const uint32_t g_firmware_cmd_num = sizeof(g_firmware_cmd_table) / sizeof(FIRMWARECMD_STRU);

OAL_STATIC FIRMWARECMD_STRU *g_current_firmware_cmd = NULL;
OAL_STATIC uint32_t g_param_buf[FIRMWARE_MAXPARAMNUM] = {0};
OAL_STATIC uint32_t g_files_flag = FILES_CMD_SEND;

OAL_STATIC int32_t oal_pcie_firmware_cmd_send_quit(oal_pcie_res *pst_pcie_res, uint8_t *buff, int32_t len)
{
#define DEV_PCIE_SDIO_SEL_REG   0x00000204
#define PCIE_SDIO_SEL_REG_VALUE 0x7070
    oal_pci_dev_stru *pst_pcie_dev = NULL;
    pci_addr_map addr_map;
    int32_t ret;
    uint16_t old;
    uint16_t value = PCIE_SDIO_SEL_REG_VALUE;

    pst_pcie_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pcie_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_dev is null");
        return -OAL_ENODEV;
    }

    pci_print_log(PCI_LOG_DBG, "pcie quit cmd send");

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, pst_pcie_res->chip_info.addr_info.glb_ctrl + DEV_PCIE_SDIO_SEL_REG,
                                    &addr_map);
    if (ret != OAL_SUCC) {
        oal_io_print("pcie send quit 0x%8x unmap, failed!\n", pst_pcie_res->chip_info.addr_info.glb_ctrl);
        return -OAL_EFAUL;
    }

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_inv(pst_pcie_dev, (void *)addr_map.pa, sizeof(value));
#endif
    old = oal_readw((void *)addr_map.va);

    oal_writew(value, (void *)addr_map.va);
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_flush(pst_pcie_dev, (void *)addr_map.pa, sizeof(value));
    oal_pci_cache_inv(pst_pcie_dev, (void *)addr_map.pa, sizeof(value));
#endif
    oal_msleep(10); /* wait tcxo effect */
    pci_print_log(PCI_LOG_INFO, "pcie quit cmd,  change 0x%8x from 0x%4x to 0x%4x callback-read= 0x%4x\n",
                  DEV_PCIE_SDIO_SEL_REG, old, value, oal_readw((void *)addr_map.va));

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_firmware_cmd_read_quit(oal_pcie_res* pst_pcie_res, uint8_t* buff,
                                                   int32_t len, uint32_t timeout)
{
    pci_print_log(PCI_LOG_DBG, "pcie quit cmd read");
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_firmware_cmd_send_files(oal_pcie_res *pst_pcie_res, uint8_t *buff, int32_t len)
{
    oal_pci_dev_stru *pst_pcie_dev = NULL;
    pci_addr_map addr_map;
    uint32_t cpu_addr;
    int32_t ret;

    pci_print_log(PCI_LOG_DBG, "pcie files cmd send");

    pst_pcie_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pcie_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_dev is null");
        g_files_flag = FILES_CMD_SEND;
        return -OAL_ENODEV;
    }

    if (g_files_flag == FILES_CMD_SEND) {
        pci_print_log(PCI_LOG_DBG, "pcie send files cmd");
        return OAL_SUCC;
    } else if (g_files_flag == FILES_BIN_SEND) {
        pci_print_log(PCI_LOG_DBG, "pcie send files bin");

        if (g_param_buf[0] != 1) {
            oal_io_print("pcie file number must be 1, current is %u\n", g_param_buf[0]);
            g_files_flag = FILES_CMD_SEND;
            return -OAL_EFAUL;
        }

        cpu_addr = g_param_buf[1];
        ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, (cpu_addr + len - 1), &addr_map);
        if (ret != OAL_SUCC) {
            oal_io_print("pcie send file len %u outof memory, addr 0x%8x!\n", len, cpu_addr);
            g_files_flag = FILES_CMD_SEND;
            return -OAL_EFAUL;
        }

        ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_addr, &addr_map);
        if (ret != OAL_SUCC) {
            oal_io_print("pcie send file addr 0x%8x unmap!\n", cpu_addr);
            g_files_flag = FILES_CMD_SEND;
            return -OAL_EFAUL;
        }

        oal_pcie_io_trans((uintptr_t)addr_map.va, (uintptr_t)buff, len);
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        oal_pci_cache_flush(pst_pcie_dev, (void *)addr_map.pa, len);
        oal_pci_cache_inv(pst_pcie_dev, (void *)addr_map.pa, len);
#endif

        return OAL_SUCC;
    } else {
        pci_print_log(PCI_LOG_ERR, "pcie files flag %u error", g_files_flag);
        g_files_flag = FILES_CMD_SEND;
        return -OAL_EINVAL;
    }
}

OAL_STATIC int32_t oal_pcie_firmware_cmd_read_files(oal_pcie_res* pst_pcie_res,
                                                    uint8_t* buff,
                                                    int32_t len,
                                                    uint32_t timeout)
{
    uint32_t size;

    pci_print_log(PCI_LOG_DBG, "pcie files cmd read");

    if (g_files_flag == FILES_CMD_SEND) {
        pci_print_log(PCI_LOG_DBG, "pcie read files READY ack");

        size = oal_min(strlen(STR_FILES_READY), len - 1);
        if (strncpy_s(buff, len, STR_FILES_READY, size) != EOK) {
            pci_print_log(PCI_LOG_ERR, "strncpy_s error, destlen=%d, srclen=%d\n ", len, size);
            return -OAL_EFAIL;
        }

        buff[len - 1] = '\0';

        g_files_flag = FILES_BIN_SEND;
        return OAL_SUCC;
    } else if (g_files_flag == FILES_BIN_SEND) {
        pci_print_log(PCI_LOG_DBG, "pcie read files FILES OK ack");

        size = oal_min(strlen(STR_FILES_OK), len - 1);
        if (strncpy_s(buff, len, STR_FILES_OK, size) != EOK) {
            pci_print_log(PCI_LOG_ERR, "strncpy_s error, destlen=%d, srclen=%d\n ", len, size);
            return -OAL_EFAIL;
        }

        buff[len - 1] = '\0';

        g_files_flag = FILES_CMD_SEND;
        return OAL_SUCC;
    } else {
        pci_print_log(PCI_LOG_ERR, "pcie files flag %u error", g_files_flag);

        g_files_flag = FILES_CMD_SEND;

        return -OAL_EINVAL;
    }
}

OAL_STATIC int32_t oal_pcie_firmware_cmd_send_version(oal_pcie_res *pst_pcie_res, uint8_t *buff, int32_t len)
{
    pci_print_log(PCI_LOG_DBG, "pcie version cmd send");
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_firmware_cmd_read_version(oal_pcie_res *pst_pcie_res, uint8_t *buff,
                                                      int32_t len, uint32_t timeout)
{
    oal_pci_dev_stru *pst_pcie_dev = NULL;
    pci_addr_map addr_map;
    int32_t ret;
    uint64_t version_address = pst_pcie_res->chip_info.addr_info.boot_version;

    pst_pcie_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pcie_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_dev is null");
        return -OAL_ENODEV;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, (version_address + len - 1), &addr_map);
    if (ret != OAL_SUCC) {
        oal_io_print("pcie read version len %u outof memory, addr 0x%8llx!\n", len, version_address);
        return -OAL_EFAUL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, version_address, &addr_map);
    if (ret != OAL_SUCC) {
        oal_io_print("pcie read version addr 0x%8llx unmap!\n", version_address);
        return -OAL_EFAUL;
    }

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_inv(pst_pcie_dev, (void *)addr_map.pa, len);
#endif

    oal_pcie_io_trans((uintptr_t)buff, addr_map.va, len);

    if (pci_dbg_condtion()) {
        oal_print_hex_dump((uint8_t *)addr_map.va, len, HEX_DUMP_GROUP_SIZE, "version: ");
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_firmware_cmd_send_writem(oal_pcie_res *pst_pcie_res, uint8_t *buff, int32_t len)
{
    oal_pci_dev_stru *pst_pcie_dev = NULL;
    pci_addr_map addr_map;
    int32_t ret;
    uint32_t value, cpu_addr, reg_width;

    pst_pcie_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pcie_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_dev is null");
        return -OAL_ENODEV;
    }

    pci_print_log(PCI_LOG_DBG, "pcie writem cmd send");

    /* buff֮ǰ���ʼ��������ȡ�� */
    reg_width = g_param_buf[0];
    cpu_addr = g_param_buf[1];
    value = g_param_buf[2];

    pci_print_log(PCI_LOG_INFO, "writem:reg width %u, addr 0x%x, value 0x%x", reg_width, cpu_addr, value);

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_addr, &addr_map);
    if (ret != OAL_SUCC) {
        oal_io_print("pcie send quit 0x%8x unmap, failed!\n", cpu_addr);
        return -OAL_EFAUL;
    }

    /* ��ַ��� */
    switch (reg_width) {
        case 1:
            *(uint8_t *)addr_map.va = (uint8_t)value;
            ret = OAL_SUCC;
            break;
        case 2:
            *(uint16_t *)addr_map.va = (uint16_t)value;
            ret = OAL_SUCC;
            break;
        case 4:
            *(uint32_t *)addr_map.va = (uint32_t)value;
            ret = OAL_SUCC;
            break;
        default:
            pci_print_log(PCI_LOG_ERR, "pcie writem cmd, reg width %u not support", reg_width);
            ret = -OAL_EINVAL;
            break;
    }

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_flush(pst_pcie_dev, (void *)addr_map.pa, sizeof(value));
    oal_pci_cache_inv(pst_pcie_dev, (void *)addr_map.pa, sizeof(value));
#endif

    pci_print_log(PCI_LOG_DBG, "pcie writem cmd, write 0x%8x to 0x%4x callback-read= 0x%4x\n",
                  cpu_addr, value, oal_readl((void *)addr_map.va));

    return ret;
}

OAL_STATIC int32_t oal_pcie_firmware_cmd_read_writem(oal_pcie_res *pst_pcie_res, uint8_t *buff,
                                                     int32_t len, uint32_t timeout)
{
    uint32_t size;

    pci_print_log(PCI_LOG_DBG, "pcie writem cmd read");

    size = oal_min(strlen(STR_WRITEM_OK), len - 1);
    if (strncpy_s(buff, len, STR_WRITEM_OK, size) != EOK) {
        pci_print_log(PCI_LOG_ERR, "strncpy_s error, destlen=%d, srclen=%d\n ", len, size);
        return -OAL_EFAIL;
    }

    buff[len - 1] = '\0';

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_firmware_cmd_send_readm(oal_pcie_res *pst_pcie_res, uint8_t *buff, int32_t len)
{
    uint32_t cpu_addr;
    uint32_t len_t;

    pci_print_log(PCI_LOG_DBG, "pcie readm cmd send");

    cpu_addr = g_param_buf[0];
    len_t = g_param_buf[1];

    /* ��ʼ��ַ�ͳ�����Ҫ���ֽڶ��� */
    if (((cpu_addr % 4) != 0) || ((len_t % 4) != 0) || (len_t == 0)) {
        pci_print_log(PCI_LOG_ERR, "pcie readm cmd error, addr 0x%x len %u", cpu_addr, len_t);
        return -OAL_EINVAL;
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_firmware_cmd_read_readm(oal_pcie_res *pst_pcie_res, uint8_t *buff,
                                                    int32_t len, uint32_t timeout)
{
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pcie_dev = NULL;
#endif
    pci_addr_map addr_map;
    uint32_t cpu_addr;
    uint32_t len_t;
    int32_t ret;

    pci_print_log(PCI_LOG_DBG, "pcie readm cmd read");

    cpu_addr = g_param_buf[0];
    len_t = g_param_buf[1];

    if (len_t < len) {
        oal_io_print("pcie readm read len %u larger than buf len %d!\n", len_t, len);
        return -OAL_EFAUL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_addr + len_t - 1, &addr_map);
    if (ret != OAL_SUCC) {
        oal_io_print("pcie read readm len %u out of memory, addr 0x%8x!\n", len_t, cpu_addr);
        return -OAL_EFAUL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_addr, &addr_map);
    if (ret != OAL_SUCC) {
        oal_io_print("pcie read readm 0x%8x unmap, failed!\n", cpu_addr);
        return -OAL_EFAUL;
    }

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_inv(pst_pcie_dev, (void *)addr_map.pa, len_t);
#endif

    oal_pcie_io_trans((uintptr_t)buff, (uintptr_t)addr_map.va, len_t);

    return len_t;
}

OAL_STATIC int32_t oal_pcie_firmware_cmd_send_jump(oal_pcie_res *pst_pcie_res, uint8_t *buff, int32_t len)
{
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_firmware_cmd_read_jump(oal_pcie_res *pst_pcie_res, uint8_t *buff,
                                                   int32_t len, uint32_t timeout)
{
    return OAL_SUCC;
}

OAL_STATIC FIRMWARECMD_STRU *oal_pcie_firmware_search_cmd(uint8_t *buff, int32_t len)
{
    uint32_t ulLoop;
    uint32_t ulCmdLen;

    if (g_files_flag == FILES_BIN_SEND) {
        pci_print_log(PCI_LOG_DBG, "pcie firmware:send file bin, no need search cmd");
        return g_current_firmware_cmd;
    }

    for (ulLoop = 0; ulLoop < g_firmware_cmd_num; ulLoop++) {
        ulCmdLen = strlen(g_firmware_cmd_table[ulLoop].acCmdName);
        if (strncmp(g_firmware_cmd_table[ulLoop].acCmdName, (char *)buff, ulCmdLen) == 0) {
            return ((FIRMWARECMD_STRU *)&g_firmware_cmd_table[ulLoop]);
        }
    }

    return NULL;
}

OAL_STATIC int32_t oal_pcie_firmware_get_param(uint8_t *buff, int32_t len)
{
    uint8_t *pst_data_buff = buff;
    uint8_t *pst_tmp = NULL;
    uint32_t len_t;
    uint32_t param_t;
    uint32_t param_index = 0;

    if (g_current_firmware_cmd == NULL) {
        pci_print_log(PCI_LOG_ERR, "g_current_firmware_cmd is null");
        return -OAL_EINVAL;
    }

    if (g_current_firmware_cmd->ucArgs == 0) {
        return OAL_SUCC;
    }

    if (g_files_flag == FILES_BIN_SEND) {
        return OAL_SUCC;
    }

    memset_s(g_param_buf, sizeof(g_param_buf), 0, sizeof(g_param_buf));

    len_t = strlen(g_current_firmware_cmd->acCmdName);
    pst_data_buff += len_t;

    while ((pst_data_buff != NULL) && (param_index < FIRMWARE_MAXPARAMNUM)) {
        if (*pst_data_buff != COMPART_CHAR) {
            pci_print_log(PCI_LOG_ERR, "param buff is error,[%s]", pst_data_buff);
            return -OAL_EINVAL;
        }

        pst_data_buff++;

        if (pst_data_buff - buff >= len) {
            break;
        }

        pst_tmp = pst_data_buff;
        param_t = simple_strtoul(pst_tmp, (char **)&pst_data_buff, 0);  // 0�Զ�ʶ�����

        g_param_buf[param_index] = param_t;
        param_index++;
    }

    if (param_index >= g_current_firmware_cmd->ucArgs) {
        return OAL_SUCC;
    } else {
        pci_print_log(PCI_LOG_ERR, "param number is error");
        return -OAL_EINVAL;
    }
}

int32_t oal_pcie_firmware_read(oal_pcie_linux_res *pst_pcie_lres, uint8_t *buff, int32_t len, uint32_t timeout)
{
    oal_pcie_res *pst_pcie_res = pst_pcie_lres->pst_pci_res;

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_ENODEV;
    }

    if (buff == NULL) {
        pci_print_log(PCI_LOG_ERR, "buff is null");
        return -OAL_EFAUL;
    }

    if (g_current_firmware_cmd != NULL) {
        return g_current_firmware_cmd->pCmdRead(pst_pcie_res, buff, len, timeout);
    } else {
        pci_print_log(PCI_LOG_ERR, "current firmware cmd struct is null");
        return -OAL_EFAUL;
    }
}

int32_t oal_pcie_firmware_write(oal_pcie_linux_res *pst_pcie_lres, uint8_t *buff, int32_t len)
{
    int32_t state;
    const uint32_t ul_max_dump_pcie_bytes = 128;
    const uint32_t ul_max_dump_firmware_bytes = 1024;
    oal_pcie_res *pst_pcie_res = pst_pcie_lres->pst_pci_res;

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_ENODEV;
    }

    if (buff == NULL) {
        pci_print_log(PCI_LOG_ERR, "buff is null");
        return -OAL_EFAUL;
    }

    if (pci_dbg_condtion()) {
        oal_print_hex_dump(buff, (len > ul_max_dump_pcie_bytes) ? ul_max_dump_pcie_bytes : len,
                           HEX_DUMP_GROUP_SIZE, "pcie patch write: ");
    }

    g_current_firmware_cmd = oal_pcie_firmware_search_cmd(buff, len);
    if (g_current_firmware_cmd != NULL) {
        state = oal_pcie_firmware_get_param(buff, len);
        if (state == OAL_SUCC) {
            return g_current_firmware_cmd->pCmdSend(pst_pcie_res, buff, len);
        } else {
            pci_print_log(PCI_LOG_ERR, "pcie firmware get param error");
            return -OAL_EFAIL;
        }
    } else {
        pci_print_log(PCI_LOG_ERR, "not support pcie firmware cmd");
        oal_print_hex_dump((uint8_t *)buff, (len > ul_max_dump_firmware_bytes) ? ul_max_dump_firmware_bytes : len,
                           HEX_DUMP_GROUP_SIZE, "firmware cmd: ");
        return -OAL_EFAIL;
    }
}

#endif
