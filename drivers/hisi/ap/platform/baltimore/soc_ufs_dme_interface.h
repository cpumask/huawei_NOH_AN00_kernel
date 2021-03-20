#ifndef __SOC_UFS_DME_INTERFACE_H__
#define __SOC_UFS_DME_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_UFS_DME_DME_TX_DATA_OFL_ADDR(base) ((base) + (0x5100UL))
#define SOC_UFS_DME_DME_TX_NAC_RECEIVED_ADDR(base) ((base) + (0x5101UL))
#define SOC_UFS_DME_DME_TX_QOS_COUNT_ADDR(base) ((base) + (0x5102UL))
#define SOC_UFS_DME_DME_TX_DL_LM_ERROR_ADDR(base) ((base) + (0x5103UL))
#define SOC_UFS_DME_DME_RX_DATA_OFL_ADDR(base) ((base) + (0x5110UL))
#define SOC_UFS_DME_DME_RX_CRC_ERROR_ADDR(base) ((base) + (0x5111UL))
#define SOC_UFS_DME_DME_RX_QOS_COUNT_ADDR(base) ((base) + (0x5112UL))
#define SOC_UFS_DME_DME_RX_DL_LM_ERROR_ADDR(base) ((base) + (0x5113UL))
#define SOC_UFS_DME_DME_TXRX_DATA_OFL_ADDR(base) ((base) + (0x5120UL))
#define SOC_UFS_DME_DME_TXRX_PA_INIT_REQUEST_ADDR(base) ((base) + (0x5121UL))
#define SOC_UFS_DME_DME_TXRX_QOS_COUNT_ADDR(base) ((base) + (0x5122UL))
#define SOC_UFS_DME_DME_TXRX_DL_LM_ERROR_ADDR(base) ((base) + (0x5123UL))
#define SOC_UFS_DME_DME_QOS_ENABLE_ADDR(base) ((base) + (0x5130UL))
#define SOC_UFS_DME_DME_QOS_STATUS_ADDR(base) ((base) + (0x5131UL))
#define SOC_UFS_DME_HIBERNATE_EXIT_MODE_ADDR(base) ((base) + (0xD000UL))
#define SOC_UFS_DME_UNIPRO_RESUME_STATE_ADDR(base) ((base) + (0xD001UL))
#define SOC_UFS_DME_DME_LAYERENABLE_ADDR(base) ((base) + (0xD002UL))
#define SOC_UFS_DME_DME_LAYERENABLE_STATE_ADDR(base) ((base) + (0xD003UL))
#define SOC_UFS_DME_DME_RESET_ADDR(base) ((base) + (0xD004UL))
#define SOC_UFS_DME_DME_RESET_STATE_ADDR(base) ((base) + (0xD005UL))
#define SOC_UFS_DME_DME_ENDPOINTRESET_ADDR(base) ((base) + (0xD006UL))
#define SOC_UFS_DME_DME_ENDPOINTRESET_STATE_ADDR(base) ((base) + (0xD007UL))
#define SOC_UFS_DME_DME_LINKSTARTUPREQ_ADDR(base) ((base) + (0xD008UL))
#define SOC_UFS_DME_DME_LINKSTARTUP_STATE_ADDR(base) ((base) + (0xD009UL))
#define SOC_UFS_DME_DME_LINKLOSTIND_STATE_ADDR(base) ((base) + (0xD00AUL))
#define SOC_UFS_DME_DME_HIBERNATEENTER_ADDR(base) ((base) + (0xD00BUL))
#define SOC_UFS_DME_DME_HIBERNATEENTER_STATE_ADDR(base) ((base) + (0xD00CUL))
#define SOC_UFS_DME_DME_HIBERNATEENTERIND_ADDR(base) ((base) + (0xD00DUL))
#define SOC_UFS_DME_DME_HIBERNATEEXIT_ADDR(base) ((base) + (0xD00EUL))
#define SOC_UFS_DME_DME_HIBERNATEEXIT_STATE_ADDR(base) ((base) + (0xD00FUL))
#define SOC_UFS_DME_DME_HIBERNATEEXITIND_ADDR(base) ((base) + (0xD010UL))
#define SOC_UFS_DME_DME_POWERMODEIND_ADDR(base) ((base) + (0xD011UL))
#define SOC_UFS_DME_DME_TESTMODE_ADDR(base) ((base) + (0xD012UL))
#define SOC_UFS_DME_DME_TESTMODE_STATE_ADDR(base) ((base) + (0xD013UL))
#define SOC_UFS_DME_DME_MPHYCFGUPDT_ADDR(base) ((base) + (0xD014UL))
#define SOC_UFS_DME_DME_MPHYCFGUPDT_STATE_ADDR(base) ((base) + (0xD015UL))
#define SOC_UFS_DME_DME_ENDPOINTRESET_IND_ADDR(base) ((base) + (0xD016UL))
#define SOC_UFS_DME_DME_LINKSTARTUP_IND_ADDR(base) ((base) + (0xD017UL))
#define SOC_UFS_DME_DME_TESTMODE_IND_ADDR(base) ((base) + (0xD018UL))
#define SOC_UFS_DME_DME_FC0PROTECTIONTIMEOUTVAL_ADDR(base) ((base) + (0xD041UL))
#define SOC_UFS_DME_DME_TC0REPLAYTIMEOUTVAL_ADDR(base) ((base) + (0xD042UL))
#define SOC_UFS_DME_DME_AFC0REQTIMEOUTVAL_ADDR(base) ((base) + (0xD043UL))
#define SOC_UFS_DME_DME_FC1PROTECTIONTIMEOUTVAL_ADDR(base) ((base) + (0xD044UL))
#define SOC_UFS_DME_DME_TC1REPLAYTIMEOUTVAL_ADDR(base) ((base) + (0xD045UL))
#define SOC_UFS_DME_DME_AFC1REQTIMEOUTVAL_ADDR(base) ((base) + (0xD046UL))
#define SOC_UFS_DME_INTR_RAW_STATUS_ADDR(base) ((base) + (0xD049UL))
#define SOC_UFS_DME_INTR_ENABLE_ADDR(base) ((base) + (0xD050UL))
#define SOC_UFS_DME_INTR_CLR_ADDR(base) ((base) + (0xD051UL))
#define SOC_UFS_DME_INTR_STATUS_ADDR(base) ((base) + (0xD052UL))
#define SOC_UFS_DME_DME_UNIPRO_STATE_ADDR(base) ((base) + (0xD053UL))
#define SOC_UFS_DME_DEBUGCOUNTER0_OFL_ADDR(base) ((base) + (0xD054UL))
#define SOC_UFS_DME_DEBUGCOUNTER1_OFL_ADDR(base) ((base) + (0xD055UL))
#define SOC_UFS_DME_DEBUGCOUNTER0_ADDR(base) ((base) + (0xD056UL))
#define SOC_UFS_DME_DEBUGCOUNTER1_ADDR(base) ((base) + (0xD057UL))
#define SOC_UFS_DME_DBG_CNT0_MASK0_ADDR(base) ((base) + (0xD058UL))
#define SOC_UFS_DME_DBG_CNT0_MASK1_ADDR(base) ((base) + (0xD059UL))
#define SOC_UFS_DME_DBG_CNT1_MASK0_ADDR(base) ((base) + (0xD05AUL))
#define SOC_UFS_DME_DBG_CNT1_MASK1_ADDR(base) ((base) + (0xD05BUL))
#define SOC_UFS_DME_DEBUGCOUNTER_EN_ADDR(base) ((base) + (0xD05CUL))
#define SOC_UFS_DME_DEBUGCOUNTER_CLR_ADDR(base) ((base) + (0xD05DUL))
#define SOC_UFS_DME_DBG_CNT_OVERFLOW_ADDR(base) ((base) + (0xD05EUL))
#define SOC_UFS_DME_DME_PEER_OPC_CTRL_ADDR(base) ((base) + (0xD060UL))
#define SOC_UFS_DME_DME_PEER_OPC_STATE_ADDR(base) ((base) + (0xD061UL))
#define SOC_UFS_DME_DME_PEER_OPC_WDATA_ADDR(base) ((base) + (0xD062UL))
#define SOC_UFS_DME_DME_PEER_OPC_RDATA_ADDR(base) ((base) + (0xD063UL))
#define SOC_UFS_DME_DME_LOCAL_OPC_DBG_ADDR(base) ((base) + (0xD064UL))
#define SOC_UFS_DME_DME_PEER_OPC_DBG_ADDR(base) ((base) + (0xD065UL))
#define SOC_UFS_DME_DME_LOCAL_OPC_STATE_ADDR(base) ((base) + (0xD066UL))
#define SOC_UFS_DME_UECPHY_ADDR(base) ((base) + (0xD070UL))
#define SOC_UFS_DME_UECPA_ADDR(base) ((base) + (0xD071UL))
#define SOC_UFS_DME_UECDL_ADDR(base) ((base) + (0xD072UL))
#define SOC_UFS_DME_UECNL_ADDR(base) ((base) + (0xD073UL))
#define SOC_UFS_DME_UECTL_ADDR(base) ((base) + (0xD074UL))
#define SOC_UFS_DME_UECDME_ADDR(base) ((base) + (0xD075UL))
#define SOC_UFS_DME_UNIPRO_ECO_REG0_ADDR(base) ((base) + (0xD076UL))
#define SOC_UFS_DME_UNIPRO_ECO_REG1_ADDR(base) ((base) + (0xD077UL))
#define SOC_UFS_DME_DME_CTRL0_ADDR(base) ((base) + (0xD100UL))
#define SOC_UFS_DME_DME_CTRL1_ADDR(base) ((base) + (0xD101UL))
#define SOC_UFS_DME_DME_CTRL2_ADDR(base) ((base) + (0xD102UL))
#define SOC_UFS_DME_dfx_dme_state0_ADDR(base) ((base) + (0xD200UL))
#define SOC_UFS_DME_dfx_dme_state1_ADDR(base) ((base) + (0xD201UL))
#define SOC_UFS_DME_dfx_rmmi_tx_cfg_state_ADDR(base) ((base) + (0xD202UL))
#define SOC_UFS_DME_dfx_rmmi_rx_cfg_state_ADDR(base) ((base) + (0xD203UL))
#define SOC_UFS_DME_dfx_rmmi_tx_sym_state0_ADDR(base) ((base) + (0xD204UL))
#define SOC_UFS_DME_dfx_rmmi_tx_sym_state1_ADDR(base) ((base) + (0xD205UL))
#define SOC_UFS_DME_dfx_rmmi_rx_sym_state0_ADDR(base) ((base) + (0xD206UL))
#define SOC_UFS_DME_dfx_rmmi_rx_sym_state1_ADDR(base) ((base) + (0xD207UL))
#define SOC_UFS_DME_dfx_tx_latl_itf_state_ADDR(base) ((base) + (0xD208UL))
#define SOC_UFS_DME_dfx_rx_latl_itf_state_ADDR(base) ((base) + (0xD209UL))
#define SOC_UFS_DME_dfx_tlnl_itf_state_ADDR(base) ((base) + (0xD210UL))
#define SOC_UFS_DME_dfx_tx_nldl_itf_state_ADDR(base) ((base) + (0xD211UL))
#define SOC_UFS_DME_dfx_rx_nldl_itf_state_ADDR(base) ((base) + (0xD212UL))
#define SOC_UFS_DME_dfx_tx_dlpa_itf_state_ADDR(base) ((base) + (0xD213UL))
#define SOC_UFS_DME_dfx_rx_dlpa_itf_state_ADDR(base) ((base) + (0xD214UL))
#else
#define SOC_UFS_DME_DME_TX_DATA_OFL_ADDR(base) ((base) + (0x5100))
#define SOC_UFS_DME_DME_TX_NAC_RECEIVED_ADDR(base) ((base) + (0x5101))
#define SOC_UFS_DME_DME_TX_QOS_COUNT_ADDR(base) ((base) + (0x5102))
#define SOC_UFS_DME_DME_TX_DL_LM_ERROR_ADDR(base) ((base) + (0x5103))
#define SOC_UFS_DME_DME_RX_DATA_OFL_ADDR(base) ((base) + (0x5110))
#define SOC_UFS_DME_DME_RX_CRC_ERROR_ADDR(base) ((base) + (0x5111))
#define SOC_UFS_DME_DME_RX_QOS_COUNT_ADDR(base) ((base) + (0x5112))
#define SOC_UFS_DME_DME_RX_DL_LM_ERROR_ADDR(base) ((base) + (0x5113))
#define SOC_UFS_DME_DME_TXRX_DATA_OFL_ADDR(base) ((base) + (0x5120))
#define SOC_UFS_DME_DME_TXRX_PA_INIT_REQUEST_ADDR(base) ((base) + (0x5121))
#define SOC_UFS_DME_DME_TXRX_QOS_COUNT_ADDR(base) ((base) + (0x5122))
#define SOC_UFS_DME_DME_TXRX_DL_LM_ERROR_ADDR(base) ((base) + (0x5123))
#define SOC_UFS_DME_DME_QOS_ENABLE_ADDR(base) ((base) + (0x5130))
#define SOC_UFS_DME_DME_QOS_STATUS_ADDR(base) ((base) + (0x5131))
#define SOC_UFS_DME_HIBERNATE_EXIT_MODE_ADDR(base) ((base) + (0xD000))
#define SOC_UFS_DME_UNIPRO_RESUME_STATE_ADDR(base) ((base) + (0xD001))
#define SOC_UFS_DME_DME_LAYERENABLE_ADDR(base) ((base) + (0xD002))
#define SOC_UFS_DME_DME_LAYERENABLE_STATE_ADDR(base) ((base) + (0xD003))
#define SOC_UFS_DME_DME_RESET_ADDR(base) ((base) + (0xD004))
#define SOC_UFS_DME_DME_RESET_STATE_ADDR(base) ((base) + (0xD005))
#define SOC_UFS_DME_DME_ENDPOINTRESET_ADDR(base) ((base) + (0xD006))
#define SOC_UFS_DME_DME_ENDPOINTRESET_STATE_ADDR(base) ((base) + (0xD007))
#define SOC_UFS_DME_DME_LINKSTARTUPREQ_ADDR(base) ((base) + (0xD008))
#define SOC_UFS_DME_DME_LINKSTARTUP_STATE_ADDR(base) ((base) + (0xD009))
#define SOC_UFS_DME_DME_LINKLOSTIND_STATE_ADDR(base) ((base) + (0xD00A))
#define SOC_UFS_DME_DME_HIBERNATEENTER_ADDR(base) ((base) + (0xD00B))
#define SOC_UFS_DME_DME_HIBERNATEENTER_STATE_ADDR(base) ((base) + (0xD00C))
#define SOC_UFS_DME_DME_HIBERNATEENTERIND_ADDR(base) ((base) + (0xD00D))
#define SOC_UFS_DME_DME_HIBERNATEEXIT_ADDR(base) ((base) + (0xD00E))
#define SOC_UFS_DME_DME_HIBERNATEEXIT_STATE_ADDR(base) ((base) + (0xD00F))
#define SOC_UFS_DME_DME_HIBERNATEEXITIND_ADDR(base) ((base) + (0xD010))
#define SOC_UFS_DME_DME_POWERMODEIND_ADDR(base) ((base) + (0xD011))
#define SOC_UFS_DME_DME_TESTMODE_ADDR(base) ((base) + (0xD012))
#define SOC_UFS_DME_DME_TESTMODE_STATE_ADDR(base) ((base) + (0xD013))
#define SOC_UFS_DME_DME_MPHYCFGUPDT_ADDR(base) ((base) + (0xD014))
#define SOC_UFS_DME_DME_MPHYCFGUPDT_STATE_ADDR(base) ((base) + (0xD015))
#define SOC_UFS_DME_DME_ENDPOINTRESET_IND_ADDR(base) ((base) + (0xD016))
#define SOC_UFS_DME_DME_LINKSTARTUP_IND_ADDR(base) ((base) + (0xD017))
#define SOC_UFS_DME_DME_TESTMODE_IND_ADDR(base) ((base) + (0xD018))
#define SOC_UFS_DME_DME_FC0PROTECTIONTIMEOUTVAL_ADDR(base) ((base) + (0xD041))
#define SOC_UFS_DME_DME_TC0REPLAYTIMEOUTVAL_ADDR(base) ((base) + (0xD042))
#define SOC_UFS_DME_DME_AFC0REQTIMEOUTVAL_ADDR(base) ((base) + (0xD043))
#define SOC_UFS_DME_DME_FC1PROTECTIONTIMEOUTVAL_ADDR(base) ((base) + (0xD044))
#define SOC_UFS_DME_DME_TC1REPLAYTIMEOUTVAL_ADDR(base) ((base) + (0xD045))
#define SOC_UFS_DME_DME_AFC1REQTIMEOUTVAL_ADDR(base) ((base) + (0xD046))
#define SOC_UFS_DME_INTR_RAW_STATUS_ADDR(base) ((base) + (0xD049))
#define SOC_UFS_DME_INTR_ENABLE_ADDR(base) ((base) + (0xD050))
#define SOC_UFS_DME_INTR_CLR_ADDR(base) ((base) + (0xD051))
#define SOC_UFS_DME_INTR_STATUS_ADDR(base) ((base) + (0xD052))
#define SOC_UFS_DME_DME_UNIPRO_STATE_ADDR(base) ((base) + (0xD053))
#define SOC_UFS_DME_DEBUGCOUNTER0_OFL_ADDR(base) ((base) + (0xD054))
#define SOC_UFS_DME_DEBUGCOUNTER1_OFL_ADDR(base) ((base) + (0xD055))
#define SOC_UFS_DME_DEBUGCOUNTER0_ADDR(base) ((base) + (0xD056))
#define SOC_UFS_DME_DEBUGCOUNTER1_ADDR(base) ((base) + (0xD057))
#define SOC_UFS_DME_DBG_CNT0_MASK0_ADDR(base) ((base) + (0xD058))
#define SOC_UFS_DME_DBG_CNT0_MASK1_ADDR(base) ((base) + (0xD059))
#define SOC_UFS_DME_DBG_CNT1_MASK0_ADDR(base) ((base) + (0xD05A))
#define SOC_UFS_DME_DBG_CNT1_MASK1_ADDR(base) ((base) + (0xD05B))
#define SOC_UFS_DME_DEBUGCOUNTER_EN_ADDR(base) ((base) + (0xD05C))
#define SOC_UFS_DME_DEBUGCOUNTER_CLR_ADDR(base) ((base) + (0xD05D))
#define SOC_UFS_DME_DBG_CNT_OVERFLOW_ADDR(base) ((base) + (0xD05E))
#define SOC_UFS_DME_DME_PEER_OPC_CTRL_ADDR(base) ((base) + (0xD060))
#define SOC_UFS_DME_DME_PEER_OPC_STATE_ADDR(base) ((base) + (0xD061))
#define SOC_UFS_DME_DME_PEER_OPC_WDATA_ADDR(base) ((base) + (0xD062))
#define SOC_UFS_DME_DME_PEER_OPC_RDATA_ADDR(base) ((base) + (0xD063))
#define SOC_UFS_DME_DME_LOCAL_OPC_DBG_ADDR(base) ((base) + (0xD064))
#define SOC_UFS_DME_DME_PEER_OPC_DBG_ADDR(base) ((base) + (0xD065))
#define SOC_UFS_DME_DME_LOCAL_OPC_STATE_ADDR(base) ((base) + (0xD066))
#define SOC_UFS_DME_UECPHY_ADDR(base) ((base) + (0xD070))
#define SOC_UFS_DME_UECPA_ADDR(base) ((base) + (0xD071))
#define SOC_UFS_DME_UECDL_ADDR(base) ((base) + (0xD072))
#define SOC_UFS_DME_UECNL_ADDR(base) ((base) + (0xD073))
#define SOC_UFS_DME_UECTL_ADDR(base) ((base) + (0xD074))
#define SOC_UFS_DME_UECDME_ADDR(base) ((base) + (0xD075))
#define SOC_UFS_DME_UNIPRO_ECO_REG0_ADDR(base) ((base) + (0xD076))
#define SOC_UFS_DME_UNIPRO_ECO_REG1_ADDR(base) ((base) + (0xD077))
#define SOC_UFS_DME_DME_CTRL0_ADDR(base) ((base) + (0xD100))
#define SOC_UFS_DME_DME_CTRL1_ADDR(base) ((base) + (0xD101))
#define SOC_UFS_DME_DME_CTRL2_ADDR(base) ((base) + (0xD102))
#define SOC_UFS_DME_dfx_dme_state0_ADDR(base) ((base) + (0xD200))
#define SOC_UFS_DME_dfx_dme_state1_ADDR(base) ((base) + (0xD201))
#define SOC_UFS_DME_dfx_rmmi_tx_cfg_state_ADDR(base) ((base) + (0xD202))
#define SOC_UFS_DME_dfx_rmmi_rx_cfg_state_ADDR(base) ((base) + (0xD203))
#define SOC_UFS_DME_dfx_rmmi_tx_sym_state0_ADDR(base) ((base) + (0xD204))
#define SOC_UFS_DME_dfx_rmmi_tx_sym_state1_ADDR(base) ((base) + (0xD205))
#define SOC_UFS_DME_dfx_rmmi_rx_sym_state0_ADDR(base) ((base) + (0xD206))
#define SOC_UFS_DME_dfx_rmmi_rx_sym_state1_ADDR(base) ((base) + (0xD207))
#define SOC_UFS_DME_dfx_tx_latl_itf_state_ADDR(base) ((base) + (0xD208))
#define SOC_UFS_DME_dfx_rx_latl_itf_state_ADDR(base) ((base) + (0xD209))
#define SOC_UFS_DME_dfx_tlnl_itf_state_ADDR(base) ((base) + (0xD210))
#define SOC_UFS_DME_dfx_tx_nldl_itf_state_ADDR(base) ((base) + (0xD211))
#define SOC_UFS_DME_dfx_rx_nldl_itf_state_ADDR(base) ((base) + (0xD212))
#define SOC_UFS_DME_dfx_tx_dlpa_itf_state_ADDR(base) ((base) + (0xD213))
#define SOC_UFS_DME_dfx_rx_dlpa_itf_state_ADDR(base) ((base) + (0xD214))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_UFS_DME_DME_Ddbl1_Revision_ADDR(base) ((base) + (0x5000UL))
#define SOC_UFS_DME_DME_Ddbl1_Level_ADDR(base) ((base) + (0x5001UL))
#define SOC_UFS_DME_DME_Ddbl1_DeviceClass_ADDR(base) ((base) + (0x5002UL))
#define SOC_UFS_DME_DME_Ddbl1_ManufacturerId_ADDR(base) ((base) + (0x5003UL))
#define SOC_UFS_DME_DME_Ddbl1_ProductId_ADDR(base) ((base) + (0x5004UL))
#define SOC_UFS_DME_DME_Ddbl1_Length_ADDR(base) ((base) + (0x5005UL))
#else
#define SOC_UFS_DME_DME_Ddbl1_Revision_ADDR(base) ((base) + (0x5000))
#define SOC_UFS_DME_DME_Ddbl1_Level_ADDR(base) ((base) + (0x5001))
#define SOC_UFS_DME_DME_Ddbl1_DeviceClass_ADDR(base) ((base) + (0x5002))
#define SOC_UFS_DME_DME_Ddbl1_ManufacturerId_ADDR(base) ((base) + (0x5003))
#define SOC_UFS_DME_DME_Ddbl1_ProductId_ADDR(base) ((base) + (0x5004))
#define SOC_UFS_DME_DME_Ddbl1_Length_ADDR(base) ((base) + (0x5005))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_tx_data_ofl : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_UFS_DME_DME_TX_DATA_OFL_UNION;
#endif
#define SOC_UFS_DME_DME_TX_DATA_OFL_dme_tx_data_ofl_START (0)
#define SOC_UFS_DME_DME_TX_DATA_OFL_dme_tx_data_ofl_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_tx_nac_received : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_UFS_DME_DME_TX_NAC_RECEIVED_UNION;
#endif
#define SOC_UFS_DME_DME_TX_NAC_RECEIVED_dme_tx_nac_received_START (0)
#define SOC_UFS_DME_DME_TX_NAC_RECEIVED_dme_tx_nac_received_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_tx_qos_count : 32;
    } reg;
} SOC_UFS_DME_DME_TX_QOS_COUNT_UNION;
#endif
#define SOC_UFS_DME_DME_TX_QOS_COUNT_dme_tx_qos_count_START (0)
#define SOC_UFS_DME_DME_TX_QOS_COUNT_dme_tx_qos_count_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tx_nac_received_cnt : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_UFS_DME_DME_TX_DL_LM_ERROR_UNION;
#endif
#define SOC_UFS_DME_DME_TX_DL_LM_ERROR_tx_nac_received_cnt_START (0)
#define SOC_UFS_DME_DME_TX_DL_LM_ERROR_tx_nac_received_cnt_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_rx_data_ofl : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_UFS_DME_DME_RX_DATA_OFL_UNION;
#endif
#define SOC_UFS_DME_DME_RX_DATA_OFL_dme_rx_data_ofl_START (0)
#define SOC_UFS_DME_DME_RX_DATA_OFL_dme_rx_data_ofl_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_rx_crc_error : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_UFS_DME_DME_RX_CRC_ERROR_UNION;
#endif
#define SOC_UFS_DME_DME_RX_CRC_ERROR_dme_rx_crc_error_START (0)
#define SOC_UFS_DME_DME_RX_CRC_ERROR_dme_rx_crc_error_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_rx_qos_count : 32;
    } reg;
} SOC_UFS_DME_DME_RX_QOS_COUNT_UNION;
#endif
#define SOC_UFS_DME_DME_RX_QOS_COUNT_dme_rx_qos_count_START (0)
#define SOC_UFS_DME_DME_RX_QOS_COUNT_dme_rx_qos_count_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rx_crc_err_cnt : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_UFS_DME_DME_RX_DL_LM_ERROR_UNION;
#endif
#define SOC_UFS_DME_DME_RX_DL_LM_ERROR_rx_crc_err_cnt_START (0)
#define SOC_UFS_DME_DME_RX_DL_LM_ERROR_rx_crc_err_cnt_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_txrx_data_ofl : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_UFS_DME_DME_TXRX_DATA_OFL_UNION;
#endif
#define SOC_UFS_DME_DME_TXRX_DATA_OFL_dme_txrx_data_ofl_START (0)
#define SOC_UFS_DME_DME_TXRX_DATA_OFL_dme_txrx_data_ofl_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_txrx_pa_init_request : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_UFS_DME_DME_TXRX_PA_INIT_REQUEST_UNION;
#endif
#define SOC_UFS_DME_DME_TXRX_PA_INIT_REQUEST_dme_txrx_pa_init_request_START (0)
#define SOC_UFS_DME_DME_TXRX_PA_INIT_REQUEST_dme_txrx_pa_init_request_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_txrx_qos_count : 32;
    } reg;
} SOC_UFS_DME_DME_TXRX_QOS_COUNT_UNION;
#endif
#define SOC_UFS_DME_DME_TXRX_QOS_COUNT_dme_txrx_qos_count_START (0)
#define SOC_UFS_DME_DME_TXRX_QOS_COUNT_dme_txrx_qos_count_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int txrx_pa_init_cnt : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_UFS_DME_DME_TXRX_DL_LM_ERROR_UNION;
#endif
#define SOC_UFS_DME_DME_TXRX_DL_LM_ERROR_txrx_pa_init_cnt_START (0)
#define SOC_UFS_DME_DME_TXRX_DL_LM_ERROR_txrx_pa_init_cnt_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_qos_enable : 3;
        unsigned int reserved : 29;
    } reg;
} SOC_UFS_DME_DME_QOS_ENABLE_UNION;
#endif
#define SOC_UFS_DME_DME_QOS_ENABLE_dme_qos_enable_START (0)
#define SOC_UFS_DME_DME_QOS_ENABLE_dme_qos_enable_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmlm_qos_ind : 3;
        unsigned int reserved : 29;
    } reg;
} SOC_UFS_DME_DME_QOS_STATUS_UNION;
#endif
#define SOC_UFS_DME_DME_QOS_STATUS_dmlm_qos_ind_START (0)
#define SOC_UFS_DME_DME_QOS_STATUS_dmlm_qos_ind_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cfg_hibernate_exit_mode : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_UFS_DME_HIBERNATE_EXIT_MODE_UNION;
#endif
#define SOC_UFS_DME_HIBERNATE_EXIT_MODE_cfg_hibernate_exit_mode_START (0)
#define SOC_UFS_DME_HIBERNATE_EXIT_MODE_cfg_hibernate_exit_mode_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int unipro_reg_load_ack : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_UFS_DME_UNIPRO_RESUME_STATE_UNION;
#endif
#define SOC_UFS_DME_UNIPRO_RESUME_STATE_unipro_reg_load_ack_START (0)
#define SOC_UFS_DME_UNIPRO_RESUME_STATE_unipro_reg_load_ack_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cfg_layerenable : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_UFS_DME_DME_LAYERENABLE_UNION;
#endif
#define SOC_UFS_DME_DME_LAYERENABLE_cfg_layerenable_START (0)
#define SOC_UFS_DME_DME_LAYERENABLE_cfg_layerenable_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmlm_EnableCnf : 2;
        unsigned int reserved : 30;
    } reg;
} SOC_UFS_DME_DME_LAYERENABLE_STATE_UNION;
#endif
#define SOC_UFS_DME_DME_LAYERENABLE_STATE_dmlm_EnableCnf_START (0)
#define SOC_UFS_DME_DME_LAYERENABLE_STATE_dmlm_EnableCnf_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cfg_dme_cold_reset : 1;
        unsigned int cfg_dme_warm_reset : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_UFS_DME_DME_RESET_UNION;
#endif
#define SOC_UFS_DME_DME_RESET_cfg_dme_cold_reset_START (0)
#define SOC_UFS_DME_DME_RESET_cfg_dme_cold_reset_END (0)
#define SOC_UFS_DME_DME_RESET_cfg_dme_warm_reset_START (1)
#define SOC_UFS_DME_DME_RESET_cfg_dme_warm_reset_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmlm_ResetCnf : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_UFS_DME_DME_RESET_STATE_UNION;
#endif
#define SOC_UFS_DME_DME_RESET_STATE_dmlm_ResetCnf_START (0)
#define SOC_UFS_DME_DME_RESET_STATE_dmlm_ResetCnf_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cfg_endpointreset : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_UFS_DME_DME_ENDPOINTRESET_UNION;
#endif
#define SOC_UFS_DME_DME_ENDPOINTRESET_cfg_endpointreset_START (0)
#define SOC_UFS_DME_DME_ENDPOINTRESET_cfg_endpointreset_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmlm_EndpointResetCnf : 2;
        unsigned int reserved : 30;
    } reg;
} SOC_UFS_DME_DME_ENDPOINTRESET_STATE_UNION;
#endif
#define SOC_UFS_DME_DME_ENDPOINTRESET_STATE_dmlm_EndpointResetCnf_START (0)
#define SOC_UFS_DME_DME_ENDPOINTRESET_STATE_dmlm_EndpointResetCnf_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cfg_linkstartup : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_UFS_DME_DME_LINKSTARTUPREQ_UNION;
#endif
#define SOC_UFS_DME_DME_LINKSTARTUPREQ_cfg_linkstartup_START (0)
#define SOC_UFS_DME_DME_LINKSTARTUPREQ_cfg_linkstartup_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmlm_LinkStartupCnf : 2;
        unsigned int reserved : 30;
    } reg;
} SOC_UFS_DME_DME_LINKSTARTUP_STATE_UNION;
#endif
#define SOC_UFS_DME_DME_LINKSTARTUP_STATE_dmlm_LinkStartupCnf_START (0)
#define SOC_UFS_DME_DME_LINKSTARTUP_STATE_dmlm_LinkStartupCnf_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmlm_LinkLostInd : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_UFS_DME_DME_LINKLOSTIND_STATE_UNION;
#endif
#define SOC_UFS_DME_DME_LINKLOSTIND_STATE_dmlm_LinkLostInd_START (0)
#define SOC_UFS_DME_DME_LINKLOSTIND_STATE_dmlm_LinkLostInd_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cfg_hibernateenter : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_UFS_DME_DME_HIBERNATEENTER_UNION;
#endif
#define SOC_UFS_DME_DME_HIBERNATEENTER_cfg_hibernateenter_START (0)
#define SOC_UFS_DME_DME_HIBERNATEENTER_cfg_hibernateenter_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmlm_HibernateEnterCnf : 2;
        unsigned int reserved : 30;
    } reg;
} SOC_UFS_DME_DME_HIBERNATEENTER_STATE_UNION;
#endif
#define SOC_UFS_DME_DME_HIBERNATEENTER_STATE_dmlm_HibernateEnterCnf_START (0)
#define SOC_UFS_DME_DME_HIBERNATEENTER_STATE_dmlm_HibernateEnterCnf_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int HibernateEnterInd : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_UFS_DME_DME_HIBERNATEENTERIND_UNION;
#endif
#define SOC_UFS_DME_DME_HIBERNATEENTERIND_HibernateEnterInd_START (0)
#define SOC_UFS_DME_DME_HIBERNATEENTERIND_HibernateEnterInd_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cfg_hibernateexit : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_UFS_DME_DME_HIBERNATEEXIT_UNION;
#endif
#define SOC_UFS_DME_DME_HIBERNATEEXIT_cfg_hibernateexit_START (0)
#define SOC_UFS_DME_DME_HIBERNATEEXIT_cfg_hibernateexit_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmlm_HibernateExitCnf : 2;
        unsigned int reserved : 30;
    } reg;
} SOC_UFS_DME_DME_HIBERNATEEXIT_STATE_UNION;
#endif
#define SOC_UFS_DME_DME_HIBERNATEEXIT_STATE_dmlm_HibernateExitCnf_START (0)
#define SOC_UFS_DME_DME_HIBERNATEEXIT_STATE_dmlm_HibernateExitCnf_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int HibernateExitInd : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_UFS_DME_DME_HIBERNATEEXITIND_UNION;
#endif
#define SOC_UFS_DME_DME_HIBERNATEEXITIND_HibernateExitInd_START (0)
#define SOC_UFS_DME_DME_HIBERNATEEXITIND_HibernateExitInd_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int PowerModeInd : 6;
        unsigned int reserved : 26;
    } reg;
} SOC_UFS_DME_DME_POWERMODEIND_UNION;
#endif
#define SOC_UFS_DME_DME_POWERMODEIND_PowerModeInd_START (0)
#define SOC_UFS_DME_DME_POWERMODEIND_PowerModeInd_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cfg_testmodereq : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_UFS_DME_DME_TESTMODE_UNION;
#endif
#define SOC_UFS_DME_DME_TESTMODE_cfg_testmodereq_START (0)
#define SOC_UFS_DME_DME_TESTMODE_cfg_testmodereq_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmlm_TestModeCnf : 2;
        unsigned int reserved : 30;
    } reg;
} SOC_UFS_DME_DME_TESTMODE_STATE_UNION;
#endif
#define SOC_UFS_DME_DME_TESTMODE_STATE_dmlm_TestModeCnf_START (0)
#define SOC_UFS_DME_DME_TESTMODE_STATE_dmlm_TestModeCnf_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cfg_mphycfg_updt : 32;
    } reg;
} SOC_UFS_DME_DME_MPHYCFGUPDT_UNION;
#endif
#define SOC_UFS_DME_DME_MPHYCFGUPDT_cfg_mphycfg_updt_START (0)
#define SOC_UFS_DME_DME_MPHYCFGUPDT_cfg_mphycfg_updt_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mphy_updt_cnf : 2;
        unsigned int reserved : 30;
    } reg;
} SOC_UFS_DME_DME_MPHYCFGUPDT_STATE_UNION;
#endif
#define SOC_UFS_DME_DME_MPHYCFGUPDT_STATE_mphy_updt_cnf_START (0)
#define SOC_UFS_DME_DME_MPHYCFGUPDT_STATE_mphy_updt_cnf_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmlm_EndpointResetInd : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_UFS_DME_DME_ENDPOINTRESET_IND_UNION;
#endif
#define SOC_UFS_DME_DME_ENDPOINTRESET_IND_dmlm_EndpointResetInd_START (0)
#define SOC_UFS_DME_DME_ENDPOINTRESET_IND_dmlm_EndpointResetInd_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmlm_LinkStartupInd : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_UFS_DME_DME_LINKSTARTUP_IND_UNION;
#endif
#define SOC_UFS_DME_DME_LINKSTARTUP_IND_dmlm_LinkStartupInd_START (0)
#define SOC_UFS_DME_DME_LINKSTARTUP_IND_dmlm_LinkStartupInd_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmlm_TestModeInd : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_UFS_DME_DME_TESTMODE_IND_UNION;
#endif
#define SOC_UFS_DME_DME_TESTMODE_IND_dmlm_TestModeInd_START (0)
#define SOC_UFS_DME_DME_TESTMODE_IND_dmlm_TestModeInd_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 16;
        unsigned int reserved_1: 16;
    } reg;
} SOC_UFS_DME_DME_FC0PROTECTIONTIMEOUTVAL_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 16;
        unsigned int reserved_1: 16;
    } reg;
} SOC_UFS_DME_DME_TC0REPLAYTIMEOUTVAL_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 16;
        unsigned int reserved_1: 16;
    } reg;
} SOC_UFS_DME_DME_AFC0REQTIMEOUTVAL_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 16;
        unsigned int reserved_1: 16;
    } reg;
} SOC_UFS_DME_DME_FC1PROTECTIONTIMEOUTVAL_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 16;
        unsigned int reserved_1: 16;
    } reg;
} SOC_UFS_DME_DME_TC1REPLAYTIMEOUTVAL_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 16;
        unsigned int reserved_1: 16;
    } reg;
} SOC_UFS_DME_DME_AFC1REQTIMEOUTVAL_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_raw_state : 32;
    } reg;
} SOC_UFS_DME_INTR_RAW_STATUS_UNION;
#endif
#define SOC_UFS_DME_INTR_RAW_STATUS_intr_raw_state_START (0)
#define SOC_UFS_DME_INTR_RAW_STATUS_intr_raw_state_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_eble : 32;
    } reg;
} SOC_UFS_DME_INTR_ENABLE_UNION;
#endif
#define SOC_UFS_DME_INTR_ENABLE_intr_eble_START (0)
#define SOC_UFS_DME_INTR_ENABLE_intr_eble_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_clr : 32;
    } reg;
} SOC_UFS_DME_INTR_CLR_UNION;
#endif
#define SOC_UFS_DME_INTR_CLR_intr_clr_START (0)
#define SOC_UFS_DME_INTR_CLR_intr_clr_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_state : 32;
    } reg;
} SOC_UFS_DME_INTR_STATUS_UNION;
#endif
#define SOC_UFS_DME_INTR_STATUS_intr_state_START (0)
#define SOC_UFS_DME_INTR_STATUS_intr_state_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int debug_dme_fsm_state : 7;
        unsigned int debug_unipro_state : 7;
        unsigned int reserved : 18;
    } reg;
} SOC_UFS_DME_DME_UNIPRO_STATE_UNION;
#endif
#define SOC_UFS_DME_DME_UNIPRO_STATE_debug_dme_fsm_state_START (0)
#define SOC_UFS_DME_DME_UNIPRO_STATE_debug_dme_fsm_state_END (6)
#define SOC_UFS_DME_DME_UNIPRO_STATE_debug_unipro_state_START (7)
#define SOC_UFS_DME_DME_UNIPRO_STATE_debug_unipro_state_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dbg_cnt0_ofl : 32;
    } reg;
} SOC_UFS_DME_DEBUGCOUNTER0_OFL_UNION;
#endif
#define SOC_UFS_DME_DEBUGCOUNTER0_OFL_dbg_cnt0_ofl_START (0)
#define SOC_UFS_DME_DEBUGCOUNTER0_OFL_dbg_cnt0_ofl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dbg_cnt1_ofl : 32;
    } reg;
} SOC_UFS_DME_DEBUGCOUNTER1_OFL_UNION;
#endif
#define SOC_UFS_DME_DEBUGCOUNTER1_OFL_dbg_cnt1_ofl_START (0)
#define SOC_UFS_DME_DEBUGCOUNTER1_OFL_dbg_cnt1_ofl_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int debugcounter0 : 32;
    } reg;
} SOC_UFS_DME_DEBUGCOUNTER0_UNION;
#endif
#define SOC_UFS_DME_DEBUGCOUNTER0_debugcounter0_START (0)
#define SOC_UFS_DME_DEBUGCOUNTER0_debugcounter0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int debugcounter1 : 32;
    } reg;
} SOC_UFS_DME_DEBUGCOUNTER1_UNION;
#endif
#define SOC_UFS_DME_DEBUGCOUNTER1_debugcounter1_START (0)
#define SOC_UFS_DME_DEBUGCOUNTER1_debugcounter1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dbg_cnt0_mask0 : 32;
    } reg;
} SOC_UFS_DME_DBG_CNT0_MASK0_UNION;
#endif
#define SOC_UFS_DME_DBG_CNT0_MASK0_dbg_cnt0_mask0_START (0)
#define SOC_UFS_DME_DBG_CNT0_MASK0_dbg_cnt0_mask0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dbg_cnt0_mask1 : 32;
    } reg;
} SOC_UFS_DME_DBG_CNT0_MASK1_UNION;
#endif
#define SOC_UFS_DME_DBG_CNT0_MASK1_dbg_cnt0_mask1_START (0)
#define SOC_UFS_DME_DBG_CNT0_MASK1_dbg_cnt0_mask1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dbg_cnt1_mask0 : 32;
    } reg;
} SOC_UFS_DME_DBG_CNT1_MASK0_UNION;
#endif
#define SOC_UFS_DME_DBG_CNT1_MASK0_dbg_cnt1_mask0_START (0)
#define SOC_UFS_DME_DBG_CNT1_MASK0_dbg_cnt1_mask0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dbg_cnt1_mask1 : 32;
    } reg;
} SOC_UFS_DME_DBG_CNT1_MASK1_UNION;
#endif
#define SOC_UFS_DME_DBG_CNT1_MASK1_dbg_cnt1_mask1_START (0)
#define SOC_UFS_DME_DBG_CNT1_MASK1_dbg_cnt1_mask1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int enablecounter0 : 1;
        unsigned int enablecounter1 : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_UFS_DME_DEBUGCOUNTER_EN_UNION;
#endif
#define SOC_UFS_DME_DEBUGCOUNTER_EN_enablecounter0_START (0)
#define SOC_UFS_DME_DEBUGCOUNTER_EN_enablecounter0_END (0)
#define SOC_UFS_DME_DEBUGCOUNTER_EN_enablecounter1_START (1)
#define SOC_UFS_DME_DEBUGCOUNTER_EN_enablecounter1_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int clearcounter0 : 1;
        unsigned int clearcounter1 : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_UFS_DME_DEBUGCOUNTER_CLR_UNION;
#endif
#define SOC_UFS_DME_DEBUGCOUNTER_CLR_clearcounter0_START (0)
#define SOC_UFS_DME_DEBUGCOUNTER_CLR_clearcounter0_END (0)
#define SOC_UFS_DME_DEBUGCOUNTER_CLR_clearcounter1_START (1)
#define SOC_UFS_DME_DEBUGCOUNTER_CLR_clearcounter1_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int counter0overflow : 1;
        unsigned int counter1overflow : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_UFS_DME_DBG_CNT_OVERFLOW_UNION;
#endif
#define SOC_UFS_DME_DBG_CNT_OVERFLOW_counter0overflow_START (0)
#define SOC_UFS_DME_DBG_CNT_OVERFLOW_counter0overflow_END (0)
#define SOC_UFS_DME_DBG_CNT_OVERFLOW_counter1overflow_START (1)
#define SOC_UFS_DME_DBG_CNT_OVERFLOW_counter1overflow_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int lmdm_AttrIDPeer : 27;
        unsigned int lmdm_AttrTypePeer : 1;
        unsigned int lmdm_AttrWrEnPeer : 1;
        unsigned int reserved : 2;
        unsigned int lmdm_AttrValidPeer : 1;
    } reg;
} SOC_UFS_DME_DME_PEER_OPC_CTRL_UNION;
#endif
#define SOC_UFS_DME_DME_PEER_OPC_CTRL_lmdm_AttrIDPeer_START (0)
#define SOC_UFS_DME_DME_PEER_OPC_CTRL_lmdm_AttrIDPeer_END (26)
#define SOC_UFS_DME_DME_PEER_OPC_CTRL_lmdm_AttrTypePeer_START (27)
#define SOC_UFS_DME_DME_PEER_OPC_CTRL_lmdm_AttrTypePeer_END (27)
#define SOC_UFS_DME_DME_PEER_OPC_CTRL_lmdm_AttrWrEnPeer_START (28)
#define SOC_UFS_DME_DME_PEER_OPC_CTRL_lmdm_AttrWrEnPeer_END (28)
#define SOC_UFS_DME_DME_PEER_OPC_CTRL_lmdm_AttrValidPeer_START (31)
#define SOC_UFS_DME_DME_PEER_OPC_CTRL_lmdm_AttrValidPeer_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int attrpeer_result : 4;
        unsigned int attrpeer_done : 1;
        unsigned int reserved : 27;
    } reg;
} SOC_UFS_DME_DME_PEER_OPC_STATE_UNION;
#endif
#define SOC_UFS_DME_DME_PEER_OPC_STATE_attrpeer_result_START (0)
#define SOC_UFS_DME_DME_PEER_OPC_STATE_attrpeer_result_END (3)
#define SOC_UFS_DME_DME_PEER_OPC_STATE_attrpeer_done_START (4)
#define SOC_UFS_DME_DME_PEER_OPC_STATE_attrpeer_done_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int lmdm_AttrWrValPeer : 32;
    } reg;
} SOC_UFS_DME_DME_PEER_OPC_WDATA_UNION;
#endif
#define SOC_UFS_DME_DME_PEER_OPC_WDATA_lmdm_AttrWrValPeer_START (0)
#define SOC_UFS_DME_DME_PEER_OPC_WDATA_lmdm_AttrWrValPeer_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int attrpeer_rdata : 32;
    } reg;
} SOC_UFS_DME_DME_PEER_OPC_RDATA_UNION;
#endif
#define SOC_UFS_DME_DME_PEER_OPC_RDATA_attrpeer_rdata_START (0)
#define SOC_UFS_DME_DME_PEER_OPC_RDATA_attrpeer_rdata_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int attrlocal_err_result : 4;
        unsigned int attrlocal_err_addr : 27;
        unsigned int reserved : 1;
    } reg;
} SOC_UFS_DME_DME_LOCAL_OPC_DBG_UNION;
#endif
#define SOC_UFS_DME_DME_LOCAL_OPC_DBG_attrlocal_err_result_START (0)
#define SOC_UFS_DME_DME_LOCAL_OPC_DBG_attrlocal_err_result_END (3)
#define SOC_UFS_DME_DME_LOCAL_OPC_DBG_attrlocal_err_addr_START (4)
#define SOC_UFS_DME_DME_LOCAL_OPC_DBG_attrlocal_err_addr_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int attrpeer_err_result : 4;
        unsigned int attrpeer_err_addr : 27;
        unsigned int reserved : 1;
    } reg;
} SOC_UFS_DME_DME_PEER_OPC_DBG_UNION;
#endif
#define SOC_UFS_DME_DME_PEER_OPC_DBG_attrpeer_err_result_START (0)
#define SOC_UFS_DME_DME_PEER_OPC_DBG_attrpeer_err_result_END (3)
#define SOC_UFS_DME_DME_PEER_OPC_DBG_attrpeer_err_addr_START (4)
#define SOC_UFS_DME_DME_PEER_OPC_DBG_attrpeer_err_addr_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int attrlocal_result : 4;
        unsigned int reserved : 28;
    } reg;
} SOC_UFS_DME_DME_LOCAL_OPC_STATE_UNION;
#endif
#define SOC_UFS_DME_DME_LOCAL_OPC_STATE_attrlocal_result_START (0)
#define SOC_UFS_DME_DME_LOCAL_OPC_STATE_attrlocal_result_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int uec_phy_ec : 4;
        unsigned int reserved : 27;
        unsigned int uec_phy_err : 1;
    } reg;
} SOC_UFS_DME_UECPHY_UNION;
#endif
#define SOC_UFS_DME_UECPHY_uec_phy_ec_START (0)
#define SOC_UFS_DME_UECPHY_uec_phy_ec_END (3)
#define SOC_UFS_DME_UECPHY_uec_phy_err_START (31)
#define SOC_UFS_DME_UECPHY_uec_phy_err_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int uec_pa_ec : 2;
        unsigned int reserved : 29;
        unsigned int uec_pa_err : 1;
    } reg;
} SOC_UFS_DME_UECPA_UNION;
#endif
#define SOC_UFS_DME_UECPA_uec_pa_ec_START (0)
#define SOC_UFS_DME_UECPA_uec_pa_ec_END (1)
#define SOC_UFS_DME_UECPA_uec_pa_err_START (31)
#define SOC_UFS_DME_UECPA_uec_pa_err_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int uec_dl_ec : 16;
        unsigned int reserved : 15;
        unsigned int uec_dl_err : 1;
    } reg;
} SOC_UFS_DME_UECDL_UNION;
#endif
#define SOC_UFS_DME_UECDL_uec_dl_ec_START (0)
#define SOC_UFS_DME_UECDL_uec_dl_ec_END (15)
#define SOC_UFS_DME_UECDL_uec_dl_err_START (31)
#define SOC_UFS_DME_UECDL_uec_dl_err_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int uec_nl_ec : 4;
        unsigned int reserved : 27;
        unsigned int uec_nl_err : 1;
    } reg;
} SOC_UFS_DME_UECNL_UNION;
#endif
#define SOC_UFS_DME_UECNL_uec_nl_ec_START (0)
#define SOC_UFS_DME_UECNL_uec_nl_ec_END (3)
#define SOC_UFS_DME_UECNL_uec_nl_err_START (31)
#define SOC_UFS_DME_UECNL_uec_nl_err_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int uec_tl_ec : 8;
        unsigned int reserved : 23;
        unsigned int uec_tl_err : 1;
    } reg;
} SOC_UFS_DME_UECTL_UNION;
#endif
#define SOC_UFS_DME_UECTL_uec_tl_ec_START (0)
#define SOC_UFS_DME_UECTL_uec_tl_ec_END (7)
#define SOC_UFS_DME_UECTL_uec_tl_err_START (31)
#define SOC_UFS_DME_UECTL_uec_tl_err_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int uec_dme_ec : 2;
        unsigned int reserved : 29;
        unsigned int uec_dme_err : 1;
    } reg;
} SOC_UFS_DME_UECDME_UNION;
#endif
#define SOC_UFS_DME_UECDME_uec_dme_ec_START (0)
#define SOC_UFS_DME_UECDME_uec_dme_ec_END (1)
#define SOC_UFS_DME_UECDME_uec_dme_err_START (31)
#define SOC_UFS_DME_UECDME_uec_dme_err_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int unipro_eco_reg0 : 32;
    } reg;
} SOC_UFS_DME_UNIPRO_ECO_REG0_UNION;
#endif
#define SOC_UFS_DME_UNIPRO_ECO_REG0_unipro_eco_reg0_START (0)
#define SOC_UFS_DME_UNIPRO_ECO_REG0_unipro_eco_reg0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int unipro_eco_reg1 : 32;
    } reg;
} SOC_UFS_DME_UNIPRO_ECO_REG1_UNION;
#endif
#define SOC_UFS_DME_UNIPRO_ECO_REG1_unipro_eco_reg1_START (0)
#define SOC_UFS_DME_UNIPRO_ECO_REG1_unipro_eco_reg1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int clk_gt_force : 4;
        unsigned int rx_clk_use_lane0 : 1;
        unsigned int hibernate_rst_en : 5;
        unsigned int hibernate_rst_mode : 2;
        unsigned int rx_phy_rst : 4;
        unsigned int tx_phy_rst : 4;
        unsigned int mphy_uncon_lane_rst_mode : 1;
        unsigned int dme_ctrl0 : 11;
    } reg;
} SOC_UFS_DME_DME_CTRL0_UNION;
#endif
#define SOC_UFS_DME_DME_CTRL0_clk_gt_force_START (0)
#define SOC_UFS_DME_DME_CTRL0_clk_gt_force_END (3)
#define SOC_UFS_DME_DME_CTRL0_rx_clk_use_lane0_START (4)
#define SOC_UFS_DME_DME_CTRL0_rx_clk_use_lane0_END (4)
#define SOC_UFS_DME_DME_CTRL0_hibernate_rst_en_START (5)
#define SOC_UFS_DME_DME_CTRL0_hibernate_rst_en_END (9)
#define SOC_UFS_DME_DME_CTRL0_hibernate_rst_mode_START (10)
#define SOC_UFS_DME_DME_CTRL0_hibernate_rst_mode_END (11)
#define SOC_UFS_DME_DME_CTRL0_rx_phy_rst_START (12)
#define SOC_UFS_DME_DME_CTRL0_rx_phy_rst_END (15)
#define SOC_UFS_DME_DME_CTRL0_tx_phy_rst_START (16)
#define SOC_UFS_DME_DME_CTRL0_tx_phy_rst_END (19)
#define SOC_UFS_DME_DME_CTRL0_mphy_uncon_lane_rst_mode_START (20)
#define SOC_UFS_DME_DME_CTRL0_mphy_uncon_lane_rst_mode_END (20)
#define SOC_UFS_DME_DME_CTRL0_dme_ctrl0_START (21)
#define SOC_UFS_DME_DME_CTRL0_dme_ctrl0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int clk_gt_en : 24;
        unsigned int cfg_access_restrict_bypass : 1;
        unsigned int sc_h8_autogate_mode : 2;
        unsigned int dme_ctrl1 : 5;
    } reg;
} SOC_UFS_DME_DME_CTRL1_UNION;
#endif
#define SOC_UFS_DME_DME_CTRL1_clk_gt_en_START (0)
#define SOC_UFS_DME_DME_CTRL1_clk_gt_en_END (23)
#define SOC_UFS_DME_DME_CTRL1_cfg_access_restrict_bypass_START (24)
#define SOC_UFS_DME_DME_CTRL1_cfg_access_restrict_bypass_END (24)
#define SOC_UFS_DME_DME_CTRL1_sc_h8_autogate_mode_START (25)
#define SOC_UFS_DME_DME_CTRL1_sc_h8_autogate_mode_END (26)
#define SOC_UFS_DME_DME_CTRL1_dme_ctrl1_START (27)
#define SOC_UFS_DME_DME_CTRL1_dme_ctrl1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int idle_cfg_cnt_threshold : 4;
        unsigned int idle_tx_cnt_threshold : 4;
        unsigned int idle_rx_cnt_threshold : 4;
        unsigned int reserved : 20;
    } reg;
} SOC_UFS_DME_DME_CTRL2_UNION;
#endif
#define SOC_UFS_DME_DME_CTRL2_idle_cfg_cnt_threshold_START (0)
#define SOC_UFS_DME_DME_CTRL2_idle_cfg_cnt_threshold_END (3)
#define SOC_UFS_DME_DME_CTRL2_idle_tx_cnt_threshold_START (4)
#define SOC_UFS_DME_DME_CTRL2_idle_tx_cnt_threshold_END (7)
#define SOC_UFS_DME_DME_CTRL2_idle_rx_cnt_threshold_START (8)
#define SOC_UFS_DME_DME_CTRL2_idle_rx_cnt_threshold_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_dme_state0 : 32;
    } reg;
} SOC_UFS_DME_dfx_dme_state0_UNION;
#endif
#define SOC_UFS_DME_dfx_dme_state0_dfx_dme_state0_START (0)
#define SOC_UFS_DME_dfx_dme_state0_dfx_dme_state0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_dme_state1 : 32;
    } reg;
} SOC_UFS_DME_dfx_dme_state1_UNION;
#endif
#define SOC_UFS_DME_dfx_dme_state1_dfx_dme_state1_START (0)
#define SOC_UFS_DME_dfx_dme_state1_dfx_dme_state1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_rmmi_tx_cfg_state : 32;
    } reg;
} SOC_UFS_DME_dfx_rmmi_tx_cfg_state_UNION;
#endif
#define SOC_UFS_DME_dfx_rmmi_tx_cfg_state_dfx_rmmi_tx_cfg_state_START (0)
#define SOC_UFS_DME_dfx_rmmi_tx_cfg_state_dfx_rmmi_tx_cfg_state_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_rmmi_rx_cfg_state : 32;
    } reg;
} SOC_UFS_DME_dfx_rmmi_rx_cfg_state_UNION;
#endif
#define SOC_UFS_DME_dfx_rmmi_rx_cfg_state_dfx_rmmi_rx_cfg_state_START (0)
#define SOC_UFS_DME_dfx_rmmi_rx_cfg_state_dfx_rmmi_rx_cfg_state_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_rmmi_tx_sym_state0 : 32;
    } reg;
} SOC_UFS_DME_dfx_rmmi_tx_sym_state0_UNION;
#endif
#define SOC_UFS_DME_dfx_rmmi_tx_sym_state0_dfx_rmmi_tx_sym_state0_START (0)
#define SOC_UFS_DME_dfx_rmmi_tx_sym_state0_dfx_rmmi_tx_sym_state0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_rmmi_tx_sym_state1 : 32;
    } reg;
} SOC_UFS_DME_dfx_rmmi_tx_sym_state1_UNION;
#endif
#define SOC_UFS_DME_dfx_rmmi_tx_sym_state1_dfx_rmmi_tx_sym_state1_START (0)
#define SOC_UFS_DME_dfx_rmmi_tx_sym_state1_dfx_rmmi_tx_sym_state1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_rmmi_rx_sym_state0 : 32;
    } reg;
} SOC_UFS_DME_dfx_rmmi_rx_sym_state0_UNION;
#endif
#define SOC_UFS_DME_dfx_rmmi_rx_sym_state0_dfx_rmmi_rx_sym_state0_START (0)
#define SOC_UFS_DME_dfx_rmmi_rx_sym_state0_dfx_rmmi_rx_sym_state0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_rmmi_rx_sym_state1 : 32;
    } reg;
} SOC_UFS_DME_dfx_rmmi_rx_sym_state1_UNION;
#endif
#define SOC_UFS_DME_dfx_rmmi_rx_sym_state1_dfx_rmmi_rx_sym_state1_START (0)
#define SOC_UFS_DME_dfx_rmmi_rx_sym_state1_dfx_rmmi_rx_sym_state1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_tx_latl_itf_state : 32;
    } reg;
} SOC_UFS_DME_dfx_tx_latl_itf_state_UNION;
#endif
#define SOC_UFS_DME_dfx_tx_latl_itf_state_dfx_tx_latl_itf_state_START (0)
#define SOC_UFS_DME_dfx_tx_latl_itf_state_dfx_tx_latl_itf_state_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_rx_latl_itf_state : 32;
    } reg;
} SOC_UFS_DME_dfx_rx_latl_itf_state_UNION;
#endif
#define SOC_UFS_DME_dfx_rx_latl_itf_state_dfx_rx_latl_itf_state_START (0)
#define SOC_UFS_DME_dfx_rx_latl_itf_state_dfx_rx_latl_itf_state_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_tlnl_itf_state : 32;
    } reg;
} SOC_UFS_DME_dfx_tlnl_itf_state_UNION;
#endif
#define SOC_UFS_DME_dfx_tlnl_itf_state_dfx_tlnl_itf_state_START (0)
#define SOC_UFS_DME_dfx_tlnl_itf_state_dfx_tlnl_itf_state_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_tx_nldl_itf_state : 32;
    } reg;
} SOC_UFS_DME_dfx_tx_nldl_itf_state_UNION;
#endif
#define SOC_UFS_DME_dfx_tx_nldl_itf_state_dfx_tx_nldl_itf_state_START (0)
#define SOC_UFS_DME_dfx_tx_nldl_itf_state_dfx_tx_nldl_itf_state_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_rx_nldl_itf_state : 32;
    } reg;
} SOC_UFS_DME_dfx_rx_nldl_itf_state_UNION;
#endif
#define SOC_UFS_DME_dfx_rx_nldl_itf_state_dfx_rx_nldl_itf_state_START (0)
#define SOC_UFS_DME_dfx_rx_nldl_itf_state_dfx_rx_nldl_itf_state_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_tx_dlpa_itf_state : 32;
    } reg;
} SOC_UFS_DME_dfx_tx_dlpa_itf_state_UNION;
#endif
#define SOC_UFS_DME_dfx_tx_dlpa_itf_state_dfx_tx_dlpa_itf_state_START (0)
#define SOC_UFS_DME_dfx_tx_dlpa_itf_state_dfx_tx_dlpa_itf_state_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfx_rx_dlpa_itf_state : 32;
    } reg;
} SOC_UFS_DME_dfx_rx_dlpa_itf_state_UNION;
#endif
#define SOC_UFS_DME_dfx_rx_dlpa_itf_state_dfx_rx_dlpa_itf_state_START (0)
#define SOC_UFS_DME_dfx_rx_dlpa_itf_state_dfx_rx_dlpa_itf_state_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_ddbl1_revision : 8;
        unsigned int reserved : 24;
    } reg;
} SOC_UFS_DME_DME_Ddbl1_Revision_UNION;
#endif
#define SOC_UFS_DME_DME_Ddbl1_Revision_dme_ddbl1_revision_START (0)
#define SOC_UFS_DME_DME_Ddbl1_Revision_dme_ddbl1_revision_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_ddbl1_level : 8;
        unsigned int reserved : 24;
    } reg;
} SOC_UFS_DME_DME_Ddbl1_Level_UNION;
#endif
#define SOC_UFS_DME_DME_Ddbl1_Level_dme_ddbl1_level_START (0)
#define SOC_UFS_DME_DME_Ddbl1_Level_dme_ddbl1_level_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_ddbl1_deviceclass : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_UFS_DME_DME_Ddbl1_DeviceClass_UNION;
#endif
#define SOC_UFS_DME_DME_Ddbl1_DeviceClass_dme_ddbl1_deviceclass_START (0)
#define SOC_UFS_DME_DME_Ddbl1_DeviceClass_dme_ddbl1_deviceclass_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_ddbl1_manufacturerid : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_UFS_DME_DME_Ddbl1_ManufacturerId_UNION;
#endif
#define SOC_UFS_DME_DME_Ddbl1_ManufacturerId_dme_ddbl1_manufacturerid_START (0)
#define SOC_UFS_DME_DME_Ddbl1_ManufacturerId_dme_ddbl1_manufacturerid_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_ddbl1_productid : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_UFS_DME_DME_Ddbl1_ProductId_UNION;
#endif
#define SOC_UFS_DME_DME_Ddbl1_ProductId_dme_ddbl1_productid_START (0)
#define SOC_UFS_DME_DME_Ddbl1_ProductId_dme_ddbl1_productid_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dme_ddbl1_length : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_UFS_DME_DME_Ddbl1_Length_UNION;
#endif
#define SOC_UFS_DME_DME_Ddbl1_Length_dme_ddbl1_length_START (0)
#define SOC_UFS_DME_DME_Ddbl1_Length_dme_ddbl1_length_END (15)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
