

#ifndef __WLAN_CFG_ID_H__
#define __WLAN_CFG_ID_H__


/*****************************************************************************
    �������� ID
    ��һ��  MIB ������
    �ڶ���  ��MIB������
*****************************************************************************/
typedef enum {
    /************************************************************************
        ��һ�� MIB ������
    *************************************************************************/
    /* *********************dot11smt OBJECT IDENTIFIER ::= { ieee802dot11 1 }**************************/
    /* --  dot11StationConfigTable ::= { dot11smt 1 } */
    WLAN_CFGID_STATION_ID = 0,      /* dot11StationID MacAddress, MAC��ַ */
    WLAN_CFGID_BSS_TYPE = 4,        /* dot11DesiredBSSType INTEGER, */
    WLAN_CFGID_SSID = 5,            /* dot11DesiredSSID OCTET STRING, SIZE(0..32) */
    WLAN_CFGID_BEACON_INTERVAL = 6, /* dot11BeaconPeriod Unsigned32, */
    WLAN_CFGID_DTIM_PERIOD = 7,     /* dot11DTIMPeriod Unsigned32, */
    WLAN_CFGID_UAPSD_EN = 11,       /* dot11APSDOptionImplemented TruthValue, */
    WLAN_CFGID_SMPS_EN = 13,
    /* --  dot11PrivacyTable ::= { dot11smt 5 } */
    /************************dot11mac OBJECT IDENTIFIER ::= { ieee802dot11 2 } **************************/
    /* --  dot11OperationTable ::= { dot11mac 1 } */
    /* --  dot11CountersTable ::= { dot11mac 2 }  */
    /* --  dot11EDCATable ::= { dot11mac 4 } */
    WLAN_CFGID_EDCA_TABLE_CWMIN = 41,         /* dot11EDCATableCWmin Unsigned32 */
    WLAN_CFGID_EDCA_TABLE_CWMAX = 42,         /* dot11EDCATableCWmax Unsigned32 */
    WLAN_CFGID_EDCA_TABLE_AIFSN = 43,         /* dot11EDCATableAIFSN Unsigned32 */
    WLAN_CFGID_EDCA_TABLE_TXOP_LIMIT = 44,    /* dot11EDCATableTXOPLimit Unsigned32 */
    WLAN_CFGID_EDCA_TABLE_MSDU_LIFETIME = 45, /* dot11EDCATableMSDULifetime Unsigned32 */
    WLAN_CFGID_EDCA_TABLE_MANDATORY = 46,     /* dot11EDCATableMandatory TruthValue */

    /* --  dot11QAPEDCATable ::= { dot11mac 5 } */
    WLAN_CFGID_QEDCA_TABLE_CWMIN = 51,         /* dot11QAPEDCATableCWmin Unsigned32 */
    WLAN_CFGID_QEDCA_TABLE_CWMAX = 52,         /* dot11QAPEDCATableCWmax Unsigned32 */
    WLAN_CFGID_QEDCA_TABLE_AIFSN = 53,         /* dot11QAPEDCATableAIFSN Unsigned32 */
    WLAN_CFGID_QEDCA_TABLE_TXOP_LIMIT = 54,    /* dot11QAPEDCATableTXOPLimit Unsigned32 */
    WLAN_CFGID_QEDCA_TABLE_MSDU_LIFETIME = 55, /* dot11QAPEDCATableMSDULifetime Unsigned32 */
    WLAN_CFGID_QEDCA_TABLE_MANDATORY = 56,     /* dot11QAPEDCATableMandatory TruthValue */

    /*************************dot11res OBJECT IDENTIFIER ::= { ieee802dot11 3 } **************************/
    /*************************dot11phy OBJECT IDENTIFIER ::= { ieee802dot11 4 } *************************/
    /* --  dot11PhyHRDSSSTable ::= { dot11phy 12 } */
    WLAN_CFGID_SHORT_PREAMBLE = 60, /* dot11ShortPreambleOptionImplemented TruthValue */

    /* --  dot11PhyERPTable ::= { dot11phy 14 } */
    /* --  dot11PhyHTTable  ::= { dot11phy 15 } */
    WLAN_CFGID_SHORTGI = 80,        /* dot11ShortGIOptionInTwentyActivated TruthValue */
    WLAN_CFGID_SHORTGI_FORTY = 81,  /* dot11ShortGIOptionInFortyActivated TruthValue */
    WLAN_CFGID_CURRENT_CHANEL = 82, /* dot11CurrentChannel Unsigned32 */

    /* --  dot11PhyVHTTable  ::= { dot11phy 23 }  -- */
    WLAN_CFGID_SHORTGI_EIGHTY = 90, /* dot11VHTShortGIOptionIn80Activated TruthValue */

    /************************dot11Conformance OBJECT IDENTIFIER ::= { ieee802dot11 5 } ********************/
    /************************dot11imt         OBJECT IDENTIFIER ::= {ieee802dot11 6} **********************/
    /************************dot11MSGCF       OBJECT IDENTIFIER ::= { ieee802dot11 7} *********************/
    /************************************************************************
        �ڶ��� ��MIB ������
    *************************************************************************/
    /* ����Ϊioctl�·������� */
    WLAN_CFGID_ADD_VAP = 100,   /* ����VAP */
    WLAN_CFGID_START_VAP = 101, /* ����VAP */
    WLAN_CFGID_DEL_VAP = 102,   /* ɾ��VAP */
    WLAN_CFGID_DOWN_VAP = 103,  /* ͣ��VAP */
    WLAN_CFGID_MODE = 105,      /* ģʽ: ����Э�� Ƶ�� ���� */
#ifdef _PRE_WLAN_FEATURE_MONITOR
    WLAN_CFGID_SNIFFER = 106,      /* ץ������ */
    WLAN_CFGID_MONITOR_MODE = 107, /* ����monitorģʽ��������֡����,�ر�monitorģʽ������֡���� */
#endif
    WLAN_CFGID_PROT_MODE = 108, /* ����ģʽ */
    WLAN_CFGID_AUTH_MODE = 109, /* ��֤ģʽ */
    WLAN_CFGID_NO_BEACON = 110,

    WLAN_CFGID_TX_POWER = 113, /* ���书�� */
    WLAN_CFGID_VAP_INFO = 114, /* ��ӡvap������Ϣ */
    WLAN_CFGID_VAP_STATE_SYN = 115,
    WLAN_CFGID_BANDWIDTH = 116,
    WLAN_CFGID_CHECK_FEM_PA = 117,

    WLAN_CFGID_STOP_SCHED_SCAN = 118,
    WLAN_CFGID_STA_VAP_INFO_SYN = 119,

    /* wpa-wpa2 */
    WLAN_CFGID_ADD_KEY = 120,
    WLAN_CFGID_DEFAULT_KEY = 121,
    WLAN_CFGID_REMOVE_KEY = 122,
    WLAN_CFGID_GET_KEY = 123,

#ifdef _PRE_WLAN_FEATURE_TXOPPS
    WLAN_CFGID_STA_TXOP_AID = 124,
    WLAN_CFGID_TXOP_PS_MACHW = 125, /* ����mac txoppsʹ�ܼĴ��� */
#endif
    WLAN_CFGID_CHECK_LTE_GPIO = 126,
    WLAN_CFGID_REMOVE_WEP_KEY = 131,
    WLAN_CFGID_ADD_WEP_ENTRY = 132,
    /* ��֤����ģʽ���� */
    WLAN_CFGID_EVENT_SWITCH = 144,       /* eventģ�鿪�� */ /* ���� */
    WLAN_CFGID_ETH_SWITCH = 145,         /* ��̫��֡�շ����� */
    WLAN_CFGID_80211_UCAST_SWITCH = 146, /* 80211����֡�ϱ����� */
    WLAN_CFGID_BTCOEX_PREEMPT_TYPE = 153,   /* preempt frame type */
    WLAN_CFGID_BTCOEX_SET_PERF_PARAM = 154, /* �������ܲ��� */
#ifdef _PRE_WLAN_FEATURE_NRCOEX
    WLAN_CFGID_NRCOEX_PRIORITY_SET = 156,
    WLAN_CFGID_NRCOEX_TEST = 157,
    WLAN_CFGID_QUERY_NRCOEX_STAT = 158,
#endif

    WLAN_CFGID_REPORT_VAP_INFO = 159, /* �ϱ�vap��Ϣ */

#ifdef _PRE_WLAN_DFT_STAT
    WLAN_CFGID_PHY_STAT_EN = 161,    /* ����phyͳ��ʹ�ܽڵ�,��������06��ɾ��,��05ͬ����ɾ�� */
    WLAN_CFGID_DBB_ENV_PARAM = 165,  /* �տڻ���������ϱ�����ֹͣ�ϱ�,��������06��ɾ��,��05ͬ����ɾ��  */
    WLAN_CFGID_USR_QUEUE_STAT = 166, /* �û�tid���кͽ��ܶ���ͳ����Ϣ */
    WLAN_CFGID_VAP_STAT = 167,       /* vap����ͳ�Ƽ���Ϣ */
    WLAN_CFGID_ALL_STAT = 168,       /* ����ͳ����Ϣ */
#endif

    WLAN_CFGID_PHY_DEBUG_SWITCH = 169,        /* ��ӡ���ձ��ĵ�phy debug��Ϣ(inc: rssi, snr, trlr etc.)�ĵ��Կ��� */
    WLAN_CFGID_80211_MCAST_SWITCH = 170,      /* 80211�鲥\�㲥֡�ϱ����� */
    WLAN_CFGID_PROBE_SWITCH = 171,            /* probe requese �� probe response�ϱ����� */
    WLAN_CFGID_GET_MPDU_NUM = 172,            /* ��ȡmpdu��Ŀ */ /* 03 05 rom������, ���� */
    WLAN_CFGID_OTA_RX_DSCR_SWITCH = 174,      /* otaģ������ota�����������ϱ����� */
    WLAN_CFGID_OTA_BEACON_SWITCH = 175,       /* �����Ƿ��ϱ�beacon֡�Ŀ��� */
    WLAN_CFGID_OAM_OUTPUT_TYPE = 176,         /* oamģ�������λ�� */ /* ���� */
    WLAN_CFGID_ADD_USER = 178,                /* ����û��������� */
    WLAN_CFGID_DEL_USER = 179,                /* ɾ���û��������� */
    WLAN_CFGID_DEL_MULTI_USER = 180,          /* ɾ���û��������� */
    WLAN_CFGID_AMPDU_END = 182,               /* �ر�AMPDU���������� */
    WLAN_CFGID_ADDBA_REQ = 185,               /* ����BA�Ự���������� */
    WLAN_CFGID_DELBA_REQ = 186,               /* ɾ��BA�Ự���������� */
    WLAN_CFGID_SET_LOG_LEVEL = 187,           /* ����LOG���ü��𿪹� */
    WLAN_CFGID_SET_FEATURE_LOG = 188,         /* ������־���Կ��� */
    WLAN_CFGID_SET_ALL_OTA = 189,             /* ���������û�֡�ϱ������п��� */
    WLAN_CFGID_SET_BEACON_OFFLOAD_TEST = 190, /* Beacon offload��صĲ��ԣ������ڲ��� */

    WLAN_CFGID_SET_DHCP_ARP = 193,             /* ����dhcp��arp�ϱ������п��� */
    WLAN_CFGID_SET_RANDOM_MAC_ADDR_SCAN = 194, /* �������mac addrɨ�迪�� */
    WLAN_CFGID_SET_RANDOM_MAC_OUI = 195,       /* �������mac oui */
    WLAN_CFGID_LIST_AP = 200,                  /* �о�ɨ�赽��AP */ /* ���� */
    WLAN_CFGID_LIST_STA = 201,                 /* �оٹ�����STA */
    WLAN_CFGID_DUMP_ALL_RX_DSCR = 203,         /* ��ӡ���еĽ��������� */ /* ���� */
    WLAN_CFGID_START_SCAN = 204,               /* ������ʼɨ�� */
    WLAN_CFGID_START_JOIN = 205,               /* ����������֤������ */
    WLAN_CFGID_START_DEAUTH = 206,             /* ����ȥ��֤ */
    WLAN_CFGID_DUMP_TIEMR = 207,               /* ��ӡ����timer��ά����Ϣ */ /* 03 05 rom������, ���� */
    WLAN_CFGID_KICK_USER = 208,                /* ȥ����1���û� */

    WLAN_CFGID_PAUSE_TID = 209,                /* ��ָͣ���û���ָ��tid */ /* 03 05 rom������, ���� */
    WLAN_CFGID_SET_USER_VIP = 210,             /* �����û�ΪVIP�û� */ /* 03 05 rom������, ���� */
    WLAN_CFGID_SET_VAP_HOST = 211,             /* ����VAP�Ƿ�Ϊhost vap */ /* 03 05 rom������, ���� */

    WLAN_CFGID_AMPDU_TX_ON = 212,              /* ������ر�ampdu���͹��� */
    WLAN_CFGID_AMSDU_TX_ON = 213,              /* ������ر�ampdu���͹��� */
    WLAN_CFGID_SEND_BAR = 215,                 /* ָ���û���ָ��tid����bar */
    WLAN_CFGID_LIST_CHAN = 217,                /* �о�֧�ֵĹ������ŵ� */
    WLAN_CFGID_REGDOMAIN_PWR = 218,            /* ���ù������� *//* ���� */ /* 05 rom������ */
    WLAN_CFGID_TXBF_SWITCH = 219,              /* ������ر�TXBF����/���չ��� */
    WLAN_CFGID_TXBF_MIB_UPDATE = 220,          /* ˢ��txbf mib���� */
    WLAN_CFGID_FRAG_THRESHOLD_REG = 221,       /* ���÷�Ƭ���޳��� */
    WLAN_CFGID_SET_VOWIFI_KEEP_ALIVE = 222,    /* vowifi nat keep alive */

    WLAN_CFGID_RESUME_RX_INTR_FIFO = 226, /* �Ƿ�ʹ�ָܻ�RX INTR FIFO���� */
    WLAN_CFGID_SET_PSM_PARAM = 231, /* STA �͹���tbtt offset/listen interval���� */
    WLAN_CFGID_SET_STA_PM_ON = 232, /* STA�͹��Ŀ��ؽӿ� */

    WLAN_CFGID_DUMP_BA_BITMAP = 241, /* ��ָ�������ı��� */ /* 03 05 rom������, ���� */

    WLAN_CFGID_SHOW_PROFILING = 245,
    WLAN_CFGID_AMSDU_AMPDU_SWITCH = 246,
    WLAN_CFGID_COUNTRY = 247,             /* ���ù������������Ϣ */
    WLAN_CFGID_TID = 248,                 /* ��ȡ���½��յ�����֡��TID */
    WLAN_CFGID_RESET_HW = 249,            /* Reset mac&phy */

    WLAN_CFGID_UAPSD_DEBUG = 250,         /* UAPSDά����Ϣ */ /* 03 05 rom������, ���� */
    WLAN_CFGID_DUMP_RX_DSCR = 251,        /* dump������������ */ /* 03 05 rom������, ���� */
    WLAN_CFGID_DUMP_TX_DSCR = 252,        /* dump�������������� */ /* 03 05 rom������, ���� */

    WLAN_CFGID_DUMP_MEMORY = 253,         /* dump�ڴ� */
    WLAN_CFGID_ALG_PARAM = 254,           /* �㷨�������� */
    WLAN_CFGID_BEACON_CHAIN_SWITCH = 255, /* ����beacon֡���Ͳ��ԣ�0Ϊ�ر�˫·�������ͣ�1Ϊ���� */
    WLAN_CFGID_PROTOCOL_DBG = 256,        /* ����Э������صĴ������Ϣ���� */
    WLAN_CFGID_2040_COEXISTENCE = 260,    /* ����20/40����ʹ�ܣ�0: 20/40����ʹ�ܣ�1: 20/40���治ʹ�� */
    WLAN_CFGID_RX_FCS_INFO = 261,         /* ��ӡ����֡FCS������Ϣ */
    WLAN_CFGID_SEND_FRAME = 262,          /* ָ���û����Ϳ���֡���߹���֡ */

#if (_PRE_PMF_NOT_SUPPORT != _PRE_WLAN_FEATURE_PMF)
    WLAN_CFGID_PMF_ENABLE = 264, /* ����ǿ��ʹ��pmf */
#endif
    WLAN_CFGID_LPM_SOC_MODE = 273, /* soc�͹��Ĳ���ģʽ���� */
    WLAN_CFGID_ACS_CONFIG = 286,       /* ACS���� */
    WLAN_CFGID_SCAN_ABORT = 287,       /* ɨ����ֹ */
    /* ��������Ϊcfg80211�·�������(ͨ���ں�) */
    WLAN_CFGID_CFG80211_START_SCHED_SCAN = 288, /* �ں��·�����PNO����ɨ������ */
    WLAN_CFGID_CFG80211_STOP_SCHED_SCAN = 289,  /* �ں��·�ֹͣPNO����ɨ������ */
    WLAN_CFGID_CFG80211_START_SCAN = 290,       /* �ں��·�����ɨ������ */
    WLAN_CFGID_CFG80211_START_CONNECT = 291,    /* �ں��·�����JOIN(connect)���� */
    WLAN_CFGID_CFG80211_SET_CHANNEL = 292,      /* �ں��·������ŵ����� */
    WLAN_CFGID_CFG80211_SET_WIPHY_PARAMS = 293, /* �ں��·�����wiphy �ṹ������ */
    WLAN_CFGID_CFG80211_CONFIG_BEACON = 295,    /* �ں��·�����VAP��Ϣ */
    WLAN_CFGID_ALG = 296,                       /* �㷨�������� */
    WLAN_CFGID_ACS_PARAM = 297,                 /* ACS���� */
    WLAN_CFGID_ALG_CFG = 298,                   /* alg_cfg�㷨���� */
    WLAN_CFGID_CALI_CFG = 299,                  /* У׼�������� */
    WLAN_CFGID_RADARTOOL = 310,                 /* DFS�������� */ /* 03 05 rom������, ���� */

    /* BEGIN:��������Ϊ��ԴAPP �����·���˽������ */
    WLAN_CFGID_GET_ASSOC_REQ_IE = 311,   /* hostapd ��ȡASSOC REQ ֡��Ϣ */
    WLAN_CFGID_SET_WPS_IE = 312,         /* hostapd ����WPS ��ϢԪ�ص�VAP */
    WLAN_CFGID_SET_RTS_THRESHHOLD = 313, /* hostapd ����RTS ���� */
    WLAN_CFGID_SET_WPS_P2P_IE = 314,     /* wpa_supplicant ����WPS/P2P ��ϢԪ�ص�VAP */
    WLAN_CFGID_CHAN_STAT = 315,

    /* END:��������Ϊ��ԴAPP �����·���˽������ */
    WLAN_CFGID_ADJUST_PPM = 319,     /* ����PPMУ׼�㷨 */ /* ���� */
    WLAN_CFGID_USER_INFO = 320,      /* �û���Ϣ */
    WLAN_CFGID_SET_DSCR = 321,       /* �����û���Ϣ */
    WLAN_CFGID_SET_RATE = 322,       /* ����non-HT���� */
    WLAN_CFGID_SET_MCS = 323,        /* ����HT���� */
    WLAN_CFGID_SET_MCSAC = 324,      /* ����VHT���� */
    WLAN_CFGID_SET_NSS = 325,        /* ���ÿռ������� */
    WLAN_CFGID_SET_RFCH = 326,       /* ���÷���ͨ�� */
    WLAN_CFGID_SET_BW = 327,         /* ���ô��� */
    WLAN_CFGID_SET_ALWAYS_RX = 329,  /* ���ó���ģʽ */
    WLAN_CFGID_GET_THRUPUT = 330,    /* ��ȡоƬ��������Ϣ */ /* 03 05 rom������, ���� */
    WLAN_CFGID_REG_INFO = 332,       /* �Ĵ�����ַ��Ϣ */
    WLAN_CFGID_REG_WRITE = 333,      /* д��Ĵ�����Ϣ */
    WLAN_CFGID_OPEN_ADDR4 = 336,     /* macͷΪ4��ַ */
    WLAN_CFGID_WMM_SWITCH = 338,     /* �򿪻��߹ر�wmm */
    WLAN_CFGID_HIDE_SSID = 339,      /* �򿪻��߹ر�����ssid */ /* 03 05 rom������, ���� */
    WLAN_CFGID_CHIP_TEST_OPEN = 340, /* ��оƬ��֤���� */
    WLAN_CFGID_SET_ALWAYS_TX = 343, /* ����1102����ģʽ */

    WLAN_CFGID_PROTECTION_UPDATE_STA_USER = 348, /* �����û����±���ģʽ */
    WLAN_CFGID_40M_INTOL_UPDATE = 349, /* ����host�ı�����¼� */
    WLAN_CFGID_SET_MCSAX = 350,    /* ����HE���� */
    WLAN_CFGID_SET_MCSAX_ER = 351, /* ����HE ER���� */
    WLAN_CFGID_SET_RU_INDEX = 352, /* ����RU index */
#ifdef _PRE_WLAN_FEATURE_OFFLOAD_FLOWCTL
    WLAN_CFGID_SET_FLOWCTL_PARAM = 353, /* ����������ز��� */
    WLAN_CFGID_GET_FLOWCTL_STAT = 354,  /* ��ȡ�������״̬��Ϣ */
    WLAN_CFGID_GET_HIPKT_STAT = 355,    /* ��ȡ�����ȼ����ĵ�ͳ����� */
#endif
    WLAN_CFGID_SET_11AX_CLOSE_HTC       = 356,
    WLAN_CFGID_SET_11AX_WIFI6_BLACKLIST = 357,
    WALN_CFGID_SET_P2P_SCENES           = 358,
    WLAN_CFGID_SET_CLEAR_11N_BLACKLIST  = 359,
    WALN_CFGID_SET_CHANGE_GO_CHANNEL    = 360,

    WLAN_CFGID_SET_AUTO_PROTECTION = 362, /* ����auto protection���� */

    WLAN_CFGID_SEND_2040_COEXT = 370, /* ����20/40�������֡ */
    WLAN_CFGID_2040_COEXT_INFO = 371, /* VAP������20/40������Ϣ */ /* ���� */

#ifdef _PRE_WLAN_FEATURE_FTM
    WLAN_CFGID_FTM_DBG = 374, /* FTM�������� */
#endif

    WLAN_CFGID_GET_VERSION = 375, /* ��ȡ�汾 */
#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
    WLAN_CFGID_SET_OPMODE_NOTIFY = 400, /* ���ù���ģʽ֪ͨ���� */ /* 03 05 rom������, ���� */
    WLAN_CFGID_GET_USER_RSSBW = 401,
#endif
#ifdef _PRE_WLAN_FEATURE_M2S
    WLAN_CFGID_SET_M2S_SWITCH = 402,    /* ����mimo-siso�л� */
    WLAN_CFGID_SET_M2S_MSS = 403,       /* �ϲ��·�MSS�л� */
    WLAN_CFGID_SET_M2S_BLACKLIST = 404, /* �ϲ��·�m2s������ */
    WLAN_CFGID_M2S_SWITCH_COMP = 405,   /* m2s�л������Ϣ�ϱ�host */
#endif
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    WLAN_CFGID_ANT_TAS_SWITCH_RSSI_NOTIFY = 406, /* TAS/Ĭ��̬�л����RSSI�ϱ�host */
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
    WLAN_CFGID_HID2D_SEQNUM_REPORT = 407, /* HID2D�������к��ϱ�host */
#endif
    WLAN_CFGID_AMSDU_SWITCH_REPORT = 408, /* dmac�Ƿ���Ҫ��amsdu��Ϣ֪ͨhost */
    WLAN_CFGID_SET_VAP_NSS = 410, /* ����VAP�Ľ��տռ���  */ /* 03 05 rom������, ���� */

#ifdef _PRE_WLAN_DFT_REG
    WLAN_CFGID_DUMP_REG = 420,
#endif

#ifdef _PRE_WLAN_FEATURE_PSD_ANALYSIS
    WLAN_CFGID_SET_PSD = 425, /* PSDʹ�� */
    WLAN_CFGID_CFG_PSD = 426, /* PSD���� */
#endif
#ifdef _PRE_WLAN_FEATURE_CSI
    WLAN_CFGID_SET_CSI = 427, /* CSIʹ�� */
#endif

    WLAN_CFGID_ADD_BLACK_LIST = 440, /* ��Ӻ����� */
    WLAN_CFGID_DEL_BLACK_LIST = 441, /* ɾ�������� */
    WLAN_CFGID_BLACKLIST_MODE = 442,
    WLAN_CFGID_BLACKLIST_SHOW = 443,

    WLAN_CFGID_ADD_BLACK_LIST_ONLY = 458, /* ��ӵ�������,����check user��ɾ��user��Ϊ */

    WLAN_CFGID_WIFI_EN = 460,                /* WIFIʹ�ܿ��� */
    WLAN_CFGID_PM_INFO = 461,                /* PM��Ϣ */
    WLAN_CFGID_PM_EN = 462,                  /* PM���� */
    WLAN_CFGID_CFG80211_SET_MIB_BY_BW = 463, /* ���ݴ���������mibֵ:GI & bw_cap */
    WLAN_CFGID_SET_AGGR_NUM = 475, /* ���þۺϸ��� */
    WLAN_CFGID_FREQ_ADJUST = 476,  /* Ƶƫ���� */

    WLAN_CFGID_SET_STBC_CAP = 477, /* ����STBC���� */
    WLAN_CFGID_SET_LDPC_CAP = 478, /* ����LDPC���� */

    WLAN_CFGID_VAP_CLASSIFY_EN = 479,  /* VAP���ȼ����� */
    WLAN_CFGID_VAP_CLASSIFY_TID = 480, /* VAP���ȼ� */

    WLAN_CFGID_RESET_HW_OPERATE = 481,    /* Reset ͬ�� */
    WLAN_CFGID_SCAN_TEST = 482,           /* ɨ��ģ��������� */
    WLAN_CFGID_QUERY_STATION_STATS = 483, /* ��Ϣ�ϱ���ѯ���� */
    WLAN_CFGID_CONNECT_REQ = 484,         /*  */
    WLAN_CFIGD_BGSCAN_ENABLE = 485,       /* ���ñ���ɨ������ */
    WLAN_CFGID_QUERY_RSSI = 486,          /* ��ѯ�û�dmac rssi��Ϣ */
    WLAN_CFGID_QUERY_RATE = 487,          /* ��ѯ�û���ǰʹ�õ�tx rx phy rate */
    WLAN_CFGID_QUERY_PSM_STAT = 488,      /* ��ѯ�͹���ͳ������ */

    WLAN_CFGID_CFG80211_REMAIN_ON_CHANNEL = 490,        /* ֹͣ��ָ���ŵ� */
    WLAN_CFGID_CFG80211_CANCEL_REMAIN_ON_CHANNEL = 491, /* ȡ��ֹͣ��ָ���ŵ� */

    WLAN_CFGID_DEVICE_MEM_LEAK = 492, /* device ��mem leak��ӡ�ӿ� */ /* ���� */
    WLAN_CFGID_DEVICE_MEM_INFO = 493, /* device ��mem ʹ�������ӡ�ӿ� */

    WLAN_CFGID_SET_PS_MODE = 494, /* ����pspoll mode */
    WLAN_CFGID_SHOW_PS_INFO = 495, /* PSM״̬�鿴 */

    WLAN_CFGID_SET_UAPSD_PARA = 496, /* ����UAPSD���� */

    WLAN_CFGID_CFG80211_MGMT_TX = 498, /* ���͹���֡ */
    WLAN_CFGID_CFG80211_MGMT_TX_STATUS = 499,
    WLAN_CFGID_THRUPUT_INFO = 501, /* ������������dmacͬ����hmac */
    WLAN_CFGID_COUNTRY_FOR_DFS = 503, /* ���ù����������dfs��Ϣ */
    WLAN_CFGID_SET_P2P_PS_OPS = 504,  /* ����P2P OPS���� */
    WLAN_CFGID_SET_P2P_PS_NOA = 505,  /* ����P2P NOA���� */
    WLAN_CFGID_SET_P2P_PS_STAT = 506, /* ����P2P ����ͳ�� */
#ifdef _PRE_WLAN_FEATURE_HS20
    WLAN_CFGID_SET_QOS_MAP = 507, /* ����HotSpot 2.0 QoSMap���� */
#endif
    WLAN_CFGID_SET_P2P_MIRACAST_STATUS = 508, /* ����P2PͶ��״̬ */

    WLAN_CFGID_UAPSD_UPDATE = 510,

    WLAN_CFGID_SDIO_FLOWCTRL = 523,
    WLAN_CFGID_NSS = 524, /* �ռ�����Ϣ��ͬ�� */

    WLAN_CFGID_ENABLE_ARP_OFFLOAD = 526,   /* arp offload�������¼� */
    WLAN_CFGID_SET_IP_ADDR = 527,          /* IPv4/IPv6��ַ�������¼� */
    WLAN_CFGID_SHOW_ARPOFFLOAD_INFO = 528, /* ��ӡdevice���IP��ַ */

    WLAN_CFGID_CFG_VAP_H2D = 529,   /* ����vap�·�device�¼� */
    WLAN_CFGID_HOST_DEV_INIT = 530, /* �·���ʼ��host dev init�¼� */
    WLAN_CFGID_HOST_DEV_EXIT = 531, /* �·�ȥ��ʼ��host dev exit�¼� */

    WLAN_CFGID_AMPDU_MMSS = 532,

#ifdef _PRE_WLAN_TCP_OPT
    WLAN_CFGID_GET_TCP_ACK_STREAM_INFO = 533, /* ��ʾTCP ACK ����ͳ��ֵ */
    WLAN_CFGID_TX_TCP_ACK_OPT_ENALBE = 534,   /* ���÷���TCP ACK�Ż�ʹ�� */
    WLAN_CFGID_RX_TCP_ACK_OPT_ENALBE = 535,   /* ���ý���TCP ACK�Ż�ʹ�� */
    WLAN_CFGID_TX_TCP_ACK_OPT_LIMIT = 536,    /* ���÷���TCP ACK LIMIT */
    WLAN_CFGID_RX_TCP_ACK_OPT_LIMIT = 537,    /* ���ý���TCP ACK LIMIT  */
#endif

    WLAN_CFGID_SET_MAX_USER = 538, /* ��������û��� */
    WLAN_CFGID_GET_STA_LIST = 539, /* ��������û��� */
    WLAN_CFGID_BTCOEX_RX_DELBA_TRIGGER = 540,
#ifdef _PRE_WLAN_DFT_STAT
    WLAN_CFGID_SET_PERFORMANCE_LOG_SWITCH = 541, /* �������ܴ�ӡ���ƿ��� */ /* ���� */
#endif

#ifdef _PRE_WLAN_FEATURE_WAPI
    WLAN_CFGID_WAPI_INFO = 542,
    WLAN_CFGID_ADD_WAPI_KEY = 543,
#endif

#ifdef _PRE_WLAN_DFT_STAT
    WLAN_CFGID_QUERY_ANI = 544, /* ��ѯVAP�����Ų��� */
#endif

    WLAN_CFGID_ROAM_ENABLE = 550, /* ����ʹ�� */
    WLAN_CFGID_ROAM_ORG = 551,    /* ������������ */ /* ���� */
    WLAN_CFGID_ROAM_BAND = 552,   /* ����Ƶ������ */ /* ���� */
    WLAN_CFGID_ROAM_START = 553,  /* ���ο�ʼ */
    WLAN_CFGID_ROAM_INFO = 554,   /* ���δ�ӡ */ /* ���� */
    WLAN_CFGID_SET_ROAMING_MODE = 555,
    WLAN_CFGID_SET_ROAM_TRIGGER = 556,
    WLAN_CFGID_ROAM_HMAC_SYNC_DMAC = 557,
    WLAN_CFGID_SET_FT_IES = 558,
    WLAN_CFGID_ROAM_NOTIFY_STATE = 559,
    WLAN_CFGID_ROAM_SUCC_H2D_SYNC = 560,
    WLAN_CFGID_CFG80211_SET_PMKSA = 561,     /* ����PMK���� */
    WLAN_CFGID_CFG80211_DEL_PMKSA = 562,     /* ɾ��PMK���� */
    WLAN_CFGID_CFG80211_FLUSH_PMKSA = 563,   /* ���PMK���� */
    WLAN_CFGID_CFG80211_EXTERNAL_AUTH = 564, /* ����SAE��֤ */

    WLAN_CFGID_SET_PM_SWITCH = 570, /* ȫ�ֵ͹���ʹ��ȥʹ�� */
    WLAN_CFGID_SET_DEVICE_FREQ = 571, /* ����device��Ƶ */
    WLAN_CFGID_SET_POWER_TEST = 572,      /* ���Ĳ���ģʽʹ�� */
    WLAN_CFGID_SET_DEVICE_PKT_STAT = 573, /* ����device��֡ͳ�� */

    WLAN_CFGID_2040BSS_ENABLE = 574, /* 20/40 bss�ж�ʹ�ܿ��� */
    WLAN_CFGID_DESTROY_VAP = 575,

    WLAN_CFGID_SET_ANT = 576, /* ����ʹ�õ����� */
    WLAN_CFGID_GET_ANT = 577, /* ��ȡ����״̬ */

    WLAN_CFGID_GREEN_AP_EN = 578,

    WLAN_CFGID_SET_TX_AMPDU_TYPE = 583,
    WLAN_CFGID_SET_RX_AMPDU_AMSDU = 584,
    WLAN_CFGID_SET_ADDBA_RSP_BUFFER = 585,
    WLAN_CFGID_SET_SK_PACING_SHIFT = 586,
    WLAN_CFGID_SET_TRX_STAT_LOG = 587,
    WLAN_CFGID_MIMO_COMPATIBILITY = 588, /* mimo������AP */
    WLAN_CFGID_DATA_COLLECT = 589, /* ���ɿ��أ�ģʽ���� */

    /* HISI-CUSTOMIZE */
    WLAN_CFGID_SET_CUS_DYN_CALI_PARAM = 605, /* ��̬У׼�������ƻ� */
    WLAN_CFGID_SET_ALL_LOG_LEVEL = 606,      /* ��������vap log level */
    WLAN_CFGID_SET_CUS_RF = 607,             /* RF���ƻ� */
    WLAN_CFGID_SET_CUS_DTS_CALI = 608,       /* DTSУ׼���ƻ� */
    WLAN_CFGID_SET_CUS_NVRAM_PARAM = 609,    /* NVRAM�������ƻ� */
    /* HISI-CUSTOMIZE INFOS */
    WLAN_CFGID_SHOW_DEV_CUSTOMIZE_INFOS = 610, /* show device customize info */

#ifdef _PRE_WLAN_FEATURE_DFR
    WLAN_CFGID_TEST_DFR_START = 611,
#endif  // _PRE_WLAN_FEATURE_DFR

    WLAN_CFGID_WFA_CFG_AIFSN = 612,
    WLAN_CFGID_WFA_CFG_CW = 613,

#ifdef _PRE_WLAN_FEATURE_TX_CLASSIFY_LAN_TO_WLAN
    WLAN_CFGID_TX_CLASSIFY_LAN_TO_WLAN_SWITCH = 614, /* ����ҵ��ʶ���ܿ��� */
#endif
#ifdef _PRE_WLAN_FEATURE_TPC_OPT
    WLAN_CFGID_REDUCE_SAR = 615, /* ͨ�����ͷ��书��������SAR */
#endif
    WLAN_CFGID_DBB_SCALING_AMEND = 616, /* ����dbb scalingֵ */
#if (defined(_PRE_WLAN_FEATURE_TPC_OPT) && defined(_PRE_WLAN_FEATURE_TAS_ANT_SWITCH))
    WLAN_CFGID_TAS_PWR_CTRL = 620, /* TAS���ʿ��Ʒ��� */
#endif
    WLAN_CFGID_SET_VENDOR_IE = 630, /* hostapd ����IEԪ�ص�VAP */

#ifdef _PRE_WLAN_FEATURE_11K
    WLAN_CFGID_SEND_NEIGHBOR_REQ = 644,
    WLAN_CFGID_REQ_SAVE_BSS_INFO = 645,
    WLAN_CFGID_BCN_TABLE_SWITCH = 646,
#endif
    WLAN_CFGID_VOE_ENABLE = 647,

    WLAN_CFGID_SET_TXRX_CHAIN = 648,   /* �����շ�ͨ�� */
    WLAN_CFGID_SET_2G_TXRX_PATH = 649, /* ����2Gͨ· */

    WLAN_CFGID_VENDOR_CMD_GET_CHANNEL_LIST = 650, /* 1102 vendor cmd, ��ȡ�ŵ��б� */

    WLAN_CFGID_VOWIFI_INFO = 653,   /* host->device��VoWiFiģʽ����; device->host,�ϱ��л�VoWiFi/VoLTE */
    WLAN_CFGID_VOWIFI_REPORT = 654, /* device->host,�ϱ��л�VoWiFi/VoLTE */

    WLAN_CFGID_SET_ALWAYS_TX_HW_CFG = 657,    /* ���� */ /* 05 rom������ */

    WLAN_CFGID_NARROW_BW = 659,

    WLAN_CFGID_SET_ALWAYS_TX_NUM = 660, /* ����1102������Ŀ */

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    WLAN_CFGID_TAS_RSSI_ACCESS = 661, /* ����RSSI���� */
#endif

    WLAN_CFGID_DC_STATUS = 667, /* DC����״̬ʶ�� */

#ifdef _PRE_WLAN_FEATURE_WMMAC
    WLAN_CFGID_ADDTS_REQ = 671,    /* ����ADDTS REQ���������� */
    WLAN_CFGID_DELTS = 672,        /* ����DELTS���������� */
    WLAN_CFGID_REASSOC_REQ = 673,  /* ����reassoc req ���������� */ /* ���� */
    WLAN_CFGID_WMMAC_SWITCH = 674, /* ����WMMAC SWITCH���ص��������� */
#endif
    WLAN_CFGID_SET_BW_FIXED = 681, /* ���ñ���û�������ģʽ����Ϊ�̶���ģʽ */
    WLAN_CFGID_SET_TX_POW = 682,   /* ���÷��͹��� */

    WLAN_CFGID_CLR_BLACK_LIST = 693,

#if defined(_PRE_WLAN_FEATURE_11V_ENABLE)
    WLAN_CFGID_11V_BSST_SWITCH = 701, /* ����|����11v bss transition���Թ��ܿ��� */
    WLAN_CFGID_11V_TX_QUERY = 702,    /* 11v ����STA����Query֡ */
#endif
    WLAN_CFGID_SET_MLME = 707,

    WLAN_CFGID_RSSI_LIMIT_CFG = 719, /* 03, 05 rom������������ */
    WLAN_CFGID_GET_BCAST_RATE = 720,
    WLAN_CFGID_GET_ALL_STA_INFO = 722,    /* ��ȡ�����ѹ���sta��Ϣ */
    WLAN_CFGID_GET_STA_INFO_BY_MAC = 723, /* ��ȡĳ��ָ������sta��Ϣ */

#ifdef _PRE_WLAN_ONLINE_DPD
#ifdef _PRE_WLAN_RF_CALI_DEBUG
    WLAN_CFGID_DPD = 728,       /* DPD�������� */
#endif
    WLAN_CFGID_DPD_START = 729, /* DPD�������� */
#endif
    WLAN_CFGID_GET_2040BSS_SW = 730, /* ��ȡ20/40 bss��ʹ�ܿ��� */

    WLAN_CFGID_FBT_GET_STA_11H_ABILITY = 734, /* hilink��ȡsta 11h������Ϣ */

    WLAN_CFGID_DSCP_MAP_TO_TID = 739,    /* ����DSCP��TID��ӳ���ϵ */
    WLAN_CFGID_GET_DSCP_TID_MAP = 740,   /* ��ȡDSCP��TID��ӳ���ϵ */
    WLAN_CFGID_CLEAN_DSCP_TID_MAP = 741, /* ���DSCP��TID��ӳ���ϵ */
    WLAN_CFGID_QUERY_PSST = 752,

    WLAN_CFGID_CALI_POWER = 755,
    WLAN_CFGID_SET_POLYNOMIAL_PARA = 756,
    WLAN_CFGID_GET_STA_INFO = 757,
    WLAN_CFGID_SET_DEVICE_MEM_FREE_STAT = 759, /* ����device�ڴ�ͳ�� */
    WLAN_CFGID_GET_PPM = 760,                  /* ��ȡppmֵ */
    WLAN_CFGID_GET_CALI_POWER = 761,           /* ��ȡУ׼����ֵ */
    WLAN_CFGID_GET_DIEID = 762,

    WLAN_CFGID_VAP_WMM_SWITCH = 766, /* �򿪻��߹ر�vap����wmm���� */

    WLAN_CFGID_PM_DEBUG_SWITCH = 768, /* �͹��ĵ������� */
    WLAN_CFGID_GET_BG_NOISE = 769, /* ��ȡͨ������ */

    WLAN_CFGID_IP_FILTER = 770, /* ����IP�˿ڹ��˵����� */

    WLAN_CFGID_GET_UPC_PARA = 771, /* ��ȡ����У׼upcֵ */
    WLAN_CFGID_SET_UPC_PARA = 772, /* д����У׼upcֵ */
    WLAN_CFGID_SET_LOAD_MODE = 777, /* ���ø���ģʽ */

#ifndef CONFIG_HAS_EARLYSUSPEND
    WLAN_CFGID_SET_SUSPEND_MODE = 778, /* ����������״̬ */
#endif

#ifdef _PRE_WLAN_FEATURE_APF
    WLAN_CFGID_SET_APF_FILTER = 779, /* ����APF PROGRAM */
#endif
    WLAN_CFGID_REMOVE_APP_IE = 780, /* �Ƴ�ĳ���ϲ����õ�IE */

    WLAN_CFGID_SET_DFS_MODE = 781, /* dfs���ܵļĴ��������ӿ� */
    WLAN_CFGID_SET_RADAR = 782,    /* �����״��¼� */

    WLAN_CFGID_MIMO_BLACKLIST = 783,
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
    WLAN_CFGID_SET_HID2D_STATE = 784, /* hid2d���ܵ������رսӿ� */
    WLAN_CFGID_HID2D_DEBUG_MODE = 785,
    WLAN_CFGID_HID2D_DEBUG_SWITCH = 786,
#endif
    WLAN_CFGID_SET_ADC_DAC_FREQ = 790, /* dfs���ܵļĴ��������ӿ� */

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
    WLAN_CFGID_TCP_ACK_BUF = 791,
#endif

    WLAN_CFGID_FORCE_STOP_FILTER = 792,

    WLAN_CFGID_GET_DBDC_INFO = 793,
#ifdef _PRE_WLAN_DELAY_STATISTIC
    WLAN_CFGID_PKT_TIME_SWITCH = 794,
#endif

    WLAN_CFGID_SET_OWE = 795, /* ����APUTģʽ��֧�ֵ�OWE group */

    WLAN_CFGID_SET_MAC_FREQ = 798, /* MACƵ�ʵļĴ��������ӿ� */

#ifdef _PRE_WLAN_FEATURE_11AX
    WLAN_CFGID_11AX_DEBUG = 799,
    WLAN_CFGID_TWT_SETUP_REQ = 800,    /* ����TWT�Ự���������� */
    WLAN_CFGID_TWT_TEARDOWN_REQ = 801, /*  ɾ��TWT�Ự���������� */
#endif

    WLAN_CFGID_SPEC_FEMLP_EN = 802, /* ���ⳡ��fem�͹��Ŀ������� */
    WLAN_CFGID_SOFTAP_MIMO_MODE = 803,

#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
    WLAN_CFGID_RX_LISTEN_PS_SWITCH = 804, /* �ն��·�rx listen ps����� */
#endif

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    WLAN_CFGID_PMF_CAP = 805, /* ����PMF���� */
#endif

    WLAN_CFGID_ASSIGNED_FILTER = 806, /* ����ָ������֡�������� */

    WLAN_CFGID_SET_ALWAYS_TX_AGGR_NUM = 807, /* ���ó����ۺϸ��� */

    WLAN_CFGID_MAC_TX_COMMON_REPORT = 809, /* mac tx common report���� */
    WLAN_CFGID_MAC_RX_COMMON_REPORT = 810, /* mac rx common report���� */
    WLAN_CFGID_COMMON_DEBUG = 811,         /* 1105 common debug���� */

    WLAN_CFGID_SET_FASTSLEEP_PARA = 813, /* ����fastsleep��ز��� */
    WLAN_CFGID_SET_RU_TEST = 814,
    WLAN_CFGID_SET_WARNING_MODE = 815,
    WLAN_CFGID_SET_CHR_MODE = 816,

#ifdef _PRE_WLAN_FEATURE_M2S
    WLAN_CFGID_SET_M2S_MODEM = 817,
#endif
    WLAN_CFGID_SET_LINKLOSS_DISABLE_CSA    = 818,  /* ����linkloss�Ƿ��ֹcsa�л� */
    WLAN_CFGID_LOG_DEBUG = 819,
#ifdef _PRE_WLAN_FEATURE_LINKLOSS_OPT
    WLAN_CFGID_TRIGGER_REASSOC = 820,    /* �����ع��� */
#endif

#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG
    WLAN_CFGID_ENABLE_RR_TIMEINFO = 821,    /* RR���ܼ��������� */
#endif
#ifdef _PRE_WLAN_FEATURE_NAN
    WLAN_CFGID_NAN = 822,
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D
#ifdef _PRE_WLAN_FEATURE_HID2D_PRESENTATION
    WLAN_CFGID_HID2D_SWITCH_CHAN = 823,  /* ����GO ͨ��CSA �����л��ŵ� */
    WLAN_CFGID_HID2D_SCAN_CHAN = 824,  /* ������apk�·�ɨ������ */
    WLAN_CFGID_HID2D_PRESENTATION_MODE = 825, /* ���÷����᳡�� */
#endif
    WLAN_CFGID_HID2D_ACS_MODE = 826, /* HiD2D�Զ��ŵ��л��ܿ��� */
    WLAN_CFGID_HID2D_ACS_REPORT = 827, /* HiD2D�Զ��ŵ��л�dmac�ϱ���Ϣ */
    WLAN_CFGID_HID2D_ACS_DEBUG = 828, /* HiD2D�Զ��ŵ������������� */
#endif

    /************************************************************************
        ������ ��MIB���ڲ�����ͬ������Ҫ�ϸ��ܿ�
    *************************************************************************/
    WLAN_CFGID_SET_MULTI_USER = 2000,
    WLAN_CFGID_USR_INFO_SYN = 2001,
    WLAN_CFGID_USER_ASOC_STATE_SYN = 2002,
    WLAN_CFGID_INIT_SECURTIY_PORT = 2004,
    WLAN_CFGID_USER_RATE_SYN = 2005,
#if defined(_PRE_WLAN_FEATURE_OPMODE_NOTIFY) || defined(_PRE_WLAN_FEATURE_SMPS) || defined(_PRE_WLAN_FEATURE_M2S)
    WLAN_CFGID_USER_M2S_INFO_SYN = 2006, /* ����opmode smps(m2s)��user�����Ϣ */
#endif
    WLAN_CFGID_USER_CAP_SYN = 2007, /* hmac��dmacͬ��mac user��cap������Ϣ */

    WLAN_CFGID_SUSPEND_ACTION_SYN = 2008,
    WLAN_CFGID_SYNC_CH_STATUS = 2009,
    WLAN_CFGID_DYN_CALI_CFG = 2010,

#ifdef _PRE_WLAN_FEATURE_M2S
    WLAN_CFGID_DEVICE_M2S_INFO_SYN = 2011,
    WLAN_CFGID_VAP_M2S_INFO_SYN = 2012,
#endif

    WLAN_CFGID_PROFILING_PACKET_ADD = 2014,

    WLAN_CFGID_DBDC_DEBUG_SWITCH = 2015, /* DBDC���� */

    WLAN_CFGID_VAP_MIB_UPDATE = 2016, /* d2h ����hal cap��VAP mib ����ͬ����host�� */
#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
    WLAN_CFGID_SYNC_PK_MODE = 2017,
#endif
    WLAN_CFGID_VAP_CAP_UPDATE = 2018, /* d2h ����hal capͬ����host�� */

    WLAN_CFGID_DYN_EXTLNA_BYPASS_SWITCH = 2019, /* ��̬����LNA bypass���� */

    WLAN_CFGID_VAP_CHANNEL_INFO_SYN = 2020,
    WLAN_CFGID_GET_MNGPKT_SENDSTAT = 2021, /* ��ȡ����֡����״̬ */

#ifdef _PRE_WLAN_DELAY_STATISTIC
    WLAN_CFGID_NOTIFY_STA_DELAY = 2022,
#endif
    WLAN_CFGID_PROBE_PESP_MODE         = 2023,

#ifdef _PRE_WLAN_FEATURE_DDR_BUGFIX
    WLAN_CFGID_SET_DDR          = 2024, /* ����DDR status */
#endif
    WLAN_UL_OFDMA_AMSDU_SYN   = 2025,
    WLAN_CFGID_SET_SEND_TB_PPDU_FLAG = 2026, /* ���õ�ǰ���͹�tb ppdu flag */
#ifdef _PRE_WLAN_FEATURE_HIEX
    WLAN_CFGID_USER_HIEX_ENABLE = 2027, /* ����user hiex���� */
#endif
    WLAN_CFGID_PS_RX_DELBA_TRIGGER = 2028,
    WLAN_CFGID_PS_ARP_PROBE = 2029,
#ifdef _PRE_WLAN_FEATURE_FTM
    WLAN_CFGID_FTM_STARTRANGING = 2030,
#endif
    WLAN_CFGID_D2H_MAX_TX_POWER = 2031, /* d2h�ϱ����͹��������Ϣ��host�� */

    /* 1106����,0X�������������˴����� */
    WLAN_CFGID_HI1106_PRIV_START = 4000,
    WLAN_CFGID_TX_TID_UPDATE = WLAN_CFGID_HI1106_PRIV_START,
    WLAN_CFGID_TX_BA_COMP = 4001,
    WLAN_CFGID_ENABLE_RX_DDR = 4002,
    WLAN_CFGID_RX_DDR_SWITCH = 4003,
    WLAN_CFGID_HAL_VAP_UPDATE = 4004,
    WLAN_CFGID_LUT_USR_UPDATE = 4005,
    WLAN_CFGID_SET_AMSDU_AGGR_NUM = 4006,

    WLAN_CFGID_HE_UORA_OCW_UPDATE = 4011,
    WLAN_CFGID_HI1106_PRIV_END,

    WLAN_CFGID_BUTT,
} wlan_cfgid_enum;
typedef uint16_t wlan_cfgid_enum_uint16;

#endif /* end of wlan_cfg_id.h */

