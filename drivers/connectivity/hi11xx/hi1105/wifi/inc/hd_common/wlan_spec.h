 

#ifndef __WLAN_SPEC_H__
#define __WLAN_SPEC_H__

// 此处不加extern "C" UT编译不过
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_HOST)
#include "wlan_oneimage_define.h"
#endif

#include "platform_spec.h"
#include "wlan_types.h"
#include "wlan_spec_1103.h"
#include "wlan_spec_1105.h"
#include "wlan_spec_1106.h"


#define WLAN_M2S_BLACKLIST_MAX_NUM 16 /* m2s下发黑名单用户数，需要和上层保持一致 */


/* 0.0 定制化变量声明 */
/* 定制化待设计为一个结构体，并对外提供内联函数访问形态，而不是预编译访问形态 */
/* 0.1.2 热点入网功能 */
/* 作为P2P GO 允许关联最大用户数 */
#define WLAN_P2P_GO_ASSOC_USER_MAX_NUM_DEFAULT WLAN_P2P_GO_ASSOC_USER_MAX_NUM_1103


/* 1.0 WLAN芯片对应的spec */
/* 每个board支持chip的最大个数放入平台 */
/* 每个chip支持device的最大个数放入平台 */
/* 最多支持的MAC硬件设备个数放入平台 */
/* 1.3 oam相关的spec */
/* oam 放入平台 */
/* 1.4 mem对应的spec */
/* 1.4.1 内存池规格 */
/* 内存 spec 放入平台 */
/* 1.4.2 共享描述符内存池配置信息 */
/* 内存 spec 放入平台 */
/* 1.4.3 共享管理帧内存池配置信息 */
/* 内存 spec 放入平台 */
/* 1.4.4 共享数据帧内存池配置信息 */
/* 内存 spec 放入平台 */
/* 1.4.5 本地内存池配置信息 */
/* 内存 spec 放入平台 */
/* 1.4.6 事件结构体内存池 */
/* 内存 spec 放入平台 */
/* 1.4.7 用户内存池 */
/* 1.4.8 MIB内存池  TBD :最终个子池的空间大小及个数需要重新考虑 */
/* 内存 spec 放入平台 */
/* 1.4.9 netbuf内存池  TBD :最终个子池的空间大小及个数需要重新考虑 */
/* 内存 spec 放入平台 */
/* 1.4.9.1 sdt netbuf内存池 */
/* 内存 spec 放入平台 */
/* 1.5 frw相关的spec */
/* 事件调度 spec 放入平台 */
/* 2 宏定义，分类和DR保持一致 */
/* 2.1 基础协议/定义物理层协议类别的spec */
/* 2.1.1 扫描侧STA 功能 */
/* TBD 一次可以扫描的最大BSS个数，两个规格可以合并 */
#define WLAN_SCAN_REQ_MAX_BSSID 2
#define WLAN_SCAN_REQ_MAX_SSID  8

/* TBD  一个信道下记录扫描到的最大BSS个数 。过少，需要扩大到200 */
#define WLAN_MAX_SCAN_BSS_PER_CH 8

#define WLAN_DEFAULT_FG_SCAN_COUNT_PER_CHANNEL        2 /* 前景扫描每信道扫描次数 */
#define WLAN_DEFAULT_BG_SCAN_COUNT_PER_CHANNEL        1 /* 背景扫描每信道扫描次数 */
#define WLAN_DEFAULT_SEND_PROBE_REQ_COUNT_PER_CHANNEL 1 /* 每次信道扫描发送probe req帧的次数 */

#define WLAN_DEFAULT_MAX_TIME_PER_SCAN (3 * 1500) /* 扫描的默认的最大执行时间，超过此时间，做超时处理 */

/* TBD 扫描时，主被动扫描定时时间，单位ms，变量命名有误 */
#define WLAN_DEFAULT_ACTIVE_SCAN_TIME      20
#define WLAN_DEFAULT_PASSIVE_SCAN_TIME     60
#define WLAN_DEFAULT_DFS_CHANNEL_SCAN_TIME 240
#define WLAN_LONG_ACTIVE_SCAN_TIME         40 /* 指定SSID扫描个数超过3个时,1次扫描超时时间为40ms */

/* 2.1.1 STA入网功能 */
/* 入网延迟，单位ms */
#define WLAN_AUTH_TIMEOUT       300 /* STA auth timeout */
#define WLAN_AUTH_AP_TIMEOUT    500 /* APUT auth timeout */
#define WLAN_ASSOC_TIMEOUT      600

/* 2.1.2 热点入网功能 */
/*
 * The 802.11 spec says at most 2007 stations may be
 * associated at once.  For most AP's this is way more
 * than is feasible so we use a default of 128.  This
 * number may be overridden by the driver and/or by
 * user configuration.
 */
#define WLAN_AID_MAX     2007
#define WLAN_AID_DEFAULT 128

/* 单位ms */
#define WLAN_AP_USER_AGING_TIME    (60 * 1000) /* AP 用户老化时间 60S */
#define WLAN_P2PGO_USER_AGING_TIME (60 * 1000) /* GO 用户老化时间 60S */

/* AP keepalive参数,单位ms */
#define WLAN_AP_KEEPALIVE_TRIGGER_TIME (15 * 1000) /* keepalive定时器触发周期 */
#define WLAN_AP_KEEPALIVE_INTERVAL     (25 * 1000) /* ap发送keepalive null帧间隔 */
#define WLAN_GO_KEEPALIVE_INTERVAL     (25 * 1000) /* P2P GO发送keepalive null帧间隔  */

/* STA keepalive参数,单位ms */
#define WLAN_STA_KEEPALIVE_TIME (25 * 1000) /* wlan0发送keepalive null帧间隔,同1101 keepalive 25s */
#define WLAN_CL_KEEPALIVE_TIME  (20 * 1000) /* P2P CL发送keepalive null帧间隔,避免CL被GO pvb唤醒,P2P cl 20s */

/* STA TBTT中断不产生时，驱动linkloss保护机制,单位ms */
#define WLAN_STA_TBTT_PROTECT_TIME1 (15 * 1000) /* wlan0 */
#define WLAN_STA_TBTT_PROTECT_TIME2 (1000) /*wlan1 */

/* Beacon Interval参数 */
/* max beacon interval, ms */
#define WLAN_BEACON_INTVAL_MAX 3500
/* min beacon interval */
#define WLAN_BEACON_INTVAL_MIN 40
/* min beacon interval */
#define WLAN_BEACON_INTVAL_DEFAULT 100
/* AP IDLE状态下beacon interval值 */
#define WLAN_BEACON_INTVAL_IDLE 1000

/* 2.1.6 保护模式功能 */
/* TBD RTS开启门限，实际可删除 */
#define WLAN_RTS_MAX     2346

/* 2.1.7 分片功能 */
/* Fragmentation limits */
/* default frag threshold */
#define WLAN_FRAG_THRESHOLD_DEFAULT 1544
/* min frag threshold */
#define WLAN_FRAG_THRESHOLD_MIN 200 /* 为了保证分片数小于16: (1472(下发最大长度)/16)+36(数据帧最大帧头) = 128  */
/* max frag threshold */
#define WLAN_FRAG_THRESHOLD_MAX 2346

/* 2.1.14 数据速率功能 */
/* 速率相关参数 */
/* 用于记录03支持的速率最大个数 */
#define WLAN_MAX_SUPP_RATES 12

/* 2.2 其他协议/定义MAC 层协议类别的spec */
/* 2.2.8 国家码功能 */
/* 管制类最大个数 */
#define WLAN_MAX_RC_NUM 20

/* 2.2.9 WMM功能 */
/* EDCA参数 */
/* STA所用WLAN_EDCA_XXX参数同WLAN_QEDCA_XXX */
#define WLAN_QEDCA_TABLE_CWMIN_MIN         1
#define WLAN_QEDCA_TABLE_CWMIN_MAX         10
#define WLAN_QEDCA_TABLE_CWMAX_MIN         1
#define WLAN_QEDCA_TABLE_CWMAX_MAX         10
#define WLAN_QEDCA_TABLE_AIFSN_MIN         2
#define WLAN_QEDCA_TABLE_AIFSN_MAX         15
#define WLAN_QEDCA_TABLE_TXOP_LIMIT_MIN    1
#define WLAN_QEDCA_TABLE_TXOP_LIMIT_MAX    65535
#define WLAN_QEDCA_TABLE_MSDU_LIFETIME_MAX 500

/* TID个数放入平台SPEC */
/* 接收队列的个数 与HAL_RX_DSCR_QUEUE_ID_BUTT相等 */
#define HAL_RX_QUEUE_NUM 3
/* 发送队列的个数 */
#define HAL_TX_QUEUE_NUM 6

/* 2.2.10 协议节能STA侧功能 */
/* PSM特性规格 */
/* default DTIM period */
#define WLAN_DTIM_DEFAULT 3

/* 2.2.11 协议节能AP侧功能 */
/* 2.3 校准类别的spec */
/* 2.4 安全协议类别的spec */
/* 2.4.7 PMF STA功能 */
/* SA Query流程间隔时间,老化时间的三分之一 */
#define WLAN_SA_QUERY_RETRY_TIME 201

/* SA Query流程超时时间,小于老化时间 */
#define WLAN_SA_QUERY_MAXIMUM_TIME 1000

/* 2.5 性能类别的spec */
/* 2.5.1 块确认功能 */
#define WLAN_ADDBA_TIMEOUT   500
#define WLAN_BAR_SEND_TIMEOUT 500

/* 支持的建立rx ba 的最大个数 */
#define WLAN_MAX_RX_BA 32   /* WTODO:需要区分芯片 */

/* 支持的建立tx ba 的最大个数 */
#define WLAN_MAX_TX_BA 32   /* WTODO:需要区分芯片 */

/* 2.5.2 AMPDU功能 */
#define WLAN_AMPDU_RX_HE_BUFFER_SIZE 256 /* AMPDU接收端接收缓冲区的buffer size的大小 */
#define WLAN_AMPDU_RX_BUFFER_SIZE    64  /* AMPDU接收端接收缓冲区的buffer size的大小 */
#define WLAN_AMPDU_RX_BA_LUT_WSIZE   64  /* AMPDU接收端用于填写BA RX LUT表的win size,
                                               要求大于等于WLAN_AMPDU_RX_BUFFER_SIZE */

#define WLAN_AMPDU_TX_MAX_NUM 64 /* AMPDU发送端最大聚合子MPDU个数 */ /* WTODO:需要区分芯片 */
#define WLAN_AMPDU_TX_DEFAULT_NUM (WLAN_AMPDU_TX_MAX_NUM)

#define WLAN_AMPDU_TX_MAX_BUF_SIZE     64 /* 发送端的buffer size */

/* MAC RX BA_LUT表共32行 */
#define HAL_MAX_RX_BA_LUT_SIZE 32
/* MAC TX BA_LUT表共32行 */
#define HAL_MAX_TX_BA_LUT_SIZE 32

/* 2.5.8 自动调频 */
/* 未建立聚合时pps门限 */
#define NO_BA_PPS_VALUE_0 0
#define NO_BA_PPS_VALUE_1 1000
#define NO_BA_PPS_VALUE_2 2500
#define NO_BA_PPS_VALUE_3 4000
/* mate7规格 */
/* pps门限       CPU主频下限     DDR主频下限 */
/* mate7 pps门限 */
#define PPS_VALUE_0 0
#define PPS_VALUE_1 2000  /* 20M up limit */
#define PPS_VALUE_2 4000  /* 40M up limit */
#define PPS_VALUE_3 10000 /* 100M up limit */
/* mate7 CPU主频下限 */
#define CPU_MIN_FREQ_VALUE_0 403200
#define CPU_MIN_FREQ_VALUE_1 604800
#define CPU_MIN_FREQ_VALUE_2 806400
#define CPU_MIN_FREQ_VALUE_3 1305600
/* mate7 DDR主频下限 */
#define DDR_MIN_FREQ_VALUE_0 0
#define DDR_MIN_FREQ_VALUE_1 3456
#define DDR_MIN_FREQ_VALUE_2 6403
#define DDR_MIN_FREQ_VALUE_3 9216

/* 2.6 算法类别的spec */
/* 2.6.3 算法工作模式区分ASIC和FPGA */
// 注意此处定义的算法开关宏，请不要在host使用
/* TBD 23 最大传输功率，单位dBm */
#define WLAN_MAX_TXPOWER  25
#define WLAN_MIN_TXPOWER (-25)

/* 2.8.2 HAL Device0芯片适配规格 */
/* HAL DEV0支持的空间流数 */
#define WLAN_HAL0_NSS_NUM WLAN_DOUBLE_NSS

/* HAL DEV0支持的最大带宽 FPGA只支持80M */
#if ((_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1105_DEV) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV))
#if (_PRE_WLAN_CHIP_ASIC == _PRE_WLAN_CHIP_VERSION)
#define WLAN_HAL0_BW_MAX_WIDTH WLAN_BW_CAP_160M
#else
#define WLAN_HAL0_BW_MAX_WIDTH WLAN_BW_CAP_20M
#endif
#else
#if (_PRE_WLAN_CHIP_ASIC == _PRE_WLAN_CHIP_VERSION)
#if defined(_PRE_WLAN_110X_PILOT) && defined(_PRE_WLAN_FEATURE_160M)
#define WLAN_HAL0_BW_MAX_WIDTH WLAN_BW_CAP_160M
#else
#define WLAN_HAL0_BW_MAX_WIDTH WLAN_BW_CAP_80M
#endif
#else
#define WLAN_HAL0_BW_MAX_WIDTH WLAN_BW_CAP_40M
#endif
#endif

/* HAL DEV0 支持SOUNDING功能 */
#define WLAN_HAL0_11N_SOUNDING OAL_TRUE

/* HAL DEV0 支持Green Field功能 */
#define WLAN_HAL0_GREEN_FIELD OAL_TRUE

/* HAL DEV0是否支持窄带 */
#define WLAN_HAL0_NB_IS_EN OAL_FALSE

/* HAL DEV0是否支持1024QAM */
#define WLAN_HAL0_1024QAM_IS_EN OAL_TRUE
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1105_DEV)
/* HAL DEV0的SU_BFEE能力 */
#define WLAN_HAL0_SU_BFEE_ANT_SUPPORT_NUM 8

/* HE SU BFEE支持的接收空时流数 */
#define WLAN_HAL0_NTX_STS_BELOW_80M 8

/* HE SU BFEE支持的接收空时流数 */
#define WLAN_HAL0_NTX_STS_OVER_80M 8
#else
/* HAL DEV0的SU_BFEE能力 */
#define WLAN_HAL0_SU_BFEE_ANT_SUPPORT_NUM 4

/* HE SU BFEE支持的接收空时流数 */
#define WLAN_HAL0_NTX_STS_BELOW_80M 4

/* HE SU BFEE支持的接收空时流数 */
#define WLAN_HAL0_NTX_STS_OVER_80M 4
#endif

/* HAL DEV0的SU_BFER能力 */
#define WLAN_HAL0_NUM_DIM_BELOW_80M 1

/* HAL DEV0的SU_BFER能力 */
#define WLAN_HAL0_NUM_DIM_OVER_80M 1
/* HAL DEV0是否支持11MC */
#define WLAN_HAL0_11MC_IS_EN OAL_FALSE

/* HAL DEV0的通道选择 */
#define WLAN_HAL0_PHY_CHAIN_SEL WLAN_PHY_CHAIN_DOUBLE

/* HAL DEV0的需要用单天线发送11b等帧时的TX通道选择 */
#define WLAN_HAL0_SNGL_TX_CHAIN_SEL WLAN_TX_CHAIN_ZERO

/* HAL DEV0的RF通道选择 */
#define WLAN_HAL0_RF_CHAIN_SEL WLAN_RF_CHAIN_DOUBLE

/* HAL DEV0是否support 2.4g dpd */
#define WLAN_HAL0_DPD_2G_IS_EN OAL_FALSE

/* HAL DEV0是否support 5g dpd */
#define WLAN_HAL0_DPD_5G_IS_EN OAL_FALSE

/* HAL DEV0是否support tx stbc, su/mu txbfer */
/* 当前double nss下的能力以ASIC定义，FPGA可通过私有定制化文件ini来覆盖刷新 */
#define WLAN_HAL0_TX_STBC_IS_EN OAL_TRUE
#define WLAN_HAL0_SU_BFER_IS_EN OAL_TRUE
#define WLAN_HAL0_MU_BFER_IS_EN OAL_FALSE

/* HAL DEV0是否support rx stbc, su/mu txbfee */
/* 当前能力以ASIC的能力来定义，FPGA可通过私有定制化文件ini来覆盖刷新 */
#define WLAN_HAL0_RX_STBC_IS_EN                    OAL_TRUE
#define WLAN_HAL0_SU_BFEE_IS_EN                    OAL_TRUE
#define WLAN_HAL0_MU_BFEE_IS_EN                    OAL_FALSE
#define WLAN_HAL0_11N_TXBF_IS_EN                   OAL_FALSE
#define WLAN_HAL0_CONTROL_FRM_TX_DOUBLE_CHAIN_FLAG OAL_FALSE

#define WLAN_HAL0_LDPC_IS_EN OAL_TRUE
#ifdef _PRE_WLAN_110X_PILOT
#define WLAN_HAL0_11AX_IS_EN OAL_TRUE
#else
#define WLAN_HAL0_11AX_IS_EN OAL_FALSE
#endif
#define WLAN_HAL0_DPD_IS_EN OAL_TRUE

#define WLAN_HAL0_RADAR_DETECTOR_IS_EN OAL_TRUE
/* HAL DEV0支持TXOP PS */
#define WLAN_HAL0_TXOPPS_IS_EN OAL_TRUE

/* SU BFER支持的发送空时流数，主路默认支持2流 */
#define WLAN_HAL0_NUMBER_SOUNDING_DIM 1

/* HAL DEV0是否支持160M和80+80M的short GI */
#ifdef _PRE_WLAN_FEATURE_160M
#define WLAN_HAL0_VHT_SGI_SUPP_160_80P80 OAL_TRUE
#else
#define WLAN_HAL0_VHT_SGI_SUPP_160_80P80 OAL_FALSE
#endif

/* 芯片版本已放入平台的定制化 */
/* 2.8.2 STA AP规格 */
/* 通道数 */
#define WLAN_RF_CHANNEL_NUMS 2

/* 最大通道数 */
#define WLAN_RF_CHANNEL_MAX_NUMS 4

/* AP VAP的规格、STA VAP的规格、STA P2P共存的规格放入平台 */
/* PROXY STA模式下VAP规格宏定义放入平台 */
/* 每个device支持vap的最大个数已放入平台 */
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV)
#define WLAN_HAL_OHTER_BSS_ID            0x1F  /* 06 rx cb vap id 5bit 全1表示其他bSS */
#else
#define WLAN_HAL_OHTER_BSS_ID 14 /* 其他BSS的广播帧上报hal vap ID */
#define WLAN_HAL_OHTER_BSS_MC_ID 15 /* 其他BSS的组播帧上报hal vap ID */
#endif
#define WLAN_HAL_OTHER_BSS_UCAST_ID      7  /* 来自其他bss的单播管理帧和数据帧，维测用 */

/* 整个BOARD支持的最大的device数目放入平台 */
/* 整个BOARD支持的最大的VAP数目已放入平台 */
/* 整个BOARD支持的最大业务VAP的数目 已放入平台 */

/* 2.8.7 特性默认开启关闭定义 */
/* TBD Feature动态当前未使用，待清理。能力没有使用 */
#define WLAN_FEATURE_UAPSD_IS_OPEN       OAL_TRUE

/* 2.9.18 Proxy STA特性 */
/* 2.10 MAC FRAME特性 */
/* 2.10.1 ht cap info */
#define HT_GREEN_FILED_DEFAULT_VALUE 0
#define HT_TX_STBC_DEFAULT_VALUE     0
#define HT_BFEE_NTX_SUPP_ANTA_NUM    4 /* 11n支持的bfer发送sounding时的最大天线个数 */
/* 2.10.2 vht cap info */
#define VHT_TX_STBC_DEFAULT_VALUE 0
#define VHT_BFEE_NTX_SUPP_STS_CAP 4 /* 协议中表示最大接收NDP Nsts个数 */

/* 2.10.3 RSSI */
#define WLAN_NEAR_DISTANCE_RSSI              (-20) /* 默认近距离信号门限-35dBm */
#define WLAN_NEAR_DISTANCE_ADJUST_RSSI       15    /* 近距离默认校准15dB */
#define WLAN_CLOSE_DISTANCE_RSSI             (-15) /* 关联前距离判断门限-25dBm */
#define WLAN_FAR_DISTANCE_RSSI               (-73) /* 默认远距离信号门限-73dBm */
#define WLAN_NORMAL_DISTANCE_RSSI_UP         (-42) /* 信号强度小于-42dBm时，才认为非超近距离 */
#define WLAN_NORMAL_DISTANCE_RSSI_DOWN       (-66) /* 信号强度大于-66dBm时，才认为是非超远距离 */
#define WLAN_FIX_MAX_POWER_RSSI              (-55) /* 固定最大功率信号门限 */
#define WLAN_RSSI_ADJUST_TH                  (-88) /* 软件上报RSSI的调整阈值, 低于改阈值rssi-=2 */
#define WLAN_LOW_RATE_OPEN_NOISEEST_TH       200   /* 每100ms打开二次噪声估计收到低速率包的个数 */
#define WLAN_LOW_RATE_OPEN_MLD_TH            100   /* 每100ms收到低速率包打开MLD算法的门限 */
#define WLAN_LOW_RATE_OPEN_MLD_COUNT         10    /* 每100ms收到低速率包打开MLD算法的次数门限 */
#define WLAN_MIRACAST_DEFAULT_RSSI           (-58) /* miracast功能上层下发probe resp时刻使用的默认rssi,正常距离 */

#define WLAN_LOW_RATE_DEC_AGC_TH    100 /* 每100ms收到低速率包降低agc target的门限 */
#define WLAN_LOW_RATE_DEC_AGC_COUNT 3   /* 每100ms收到低速率包降低agc target的次数门限 */
#define WLAN_DEC_AGC_INTF_RSSI_MAX  (-57)
#define WLAN_DEC_AGC_INTF_RSSI_MIN  (-80)

#define WLAN_PHY_EXTLNA_CHGPTDBM_TH          (-25) /* 方向性攻关，修改切换门限为-25dBm */
#define WLAN_PHY_EXTLNA_CHGPTDBM_TH_VAL(_en_band, _en_pm_flag) ( \
    ((_en_pm_flag) == OAL_FALSE) ? (WLAN_PHY_EXTLNA_CHGPTDBM_TH) : (((_en_band) == WLAN_BAND_2G) ? (-50) : (-40)))

/* mib初始化 */
#define HE_BFEE_NTX_SUPP_STS_CAP_EIGHT 8 /* 1105表示最大接收NDP Nsts个数 */
#define HE_BFEE_NTX_SUPP_STS_CAP_FOUR  4 /* 1103表示最大接收NDP Nsts个数 */

/* 2.11 描述符偏移 */
#define WLAN_RX_DSCR_SIZE WLAN_MEM_SHARED_RX_DSCR_SIZE  // 实际接收描述符大小


/* 2.13 Calibration FEATURE spec */
/* 校准数据上传下发MASK */
#define CALI_DATA_REFRESH_MASK  BIT0
#define CALI_DATA_REUPLOAD_MASK BIT1
#define CALI_POWER_LVL_DBG_MASK BIT3
#define CALI_MIMO_MASK          (BIT18 | BIT17 | BIT16)
#define CALI_HI1106_MIMO_MASK   (BIT15 | BIT13 | BIT9)
#define CALI_MUL_TIME_CALI_MASK BIT30
#define CALI_INTVL_MASK         0xe0
#define CALI_INTVL_OFFSET       5

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of wlan_spec.h */

