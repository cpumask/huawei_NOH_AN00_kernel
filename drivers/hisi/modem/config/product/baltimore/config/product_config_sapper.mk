#lr
RTOSCK_OS_PATH := $(BALONG_TOPDIR)/modem/system/rtosck_smp/smpv2r2
RTOSCK_INCLUDE_PATH := $(BALONG_TOPDIR)/modem/include/rtosck_smp/smpv2r2
ifeq ($(modem_sanitizer),true)
RTOSCK_OS_LIB_PATH := $(BALONG_TOPDIR)/build/delivery/$(OBB_PRODUCT_NAME)/lib/rtosck_os/smpv2r2_lr_asan
else
RTOSCK_OS_LIB_PATH := $(BALONG_TOPDIR)/build/delivery/$(OBB_PRODUCT_NAME)/lib/rtosck_os/smpv2r2
endif
#nr
RTOSCK_NR_OS_PATH := $(BALONG_TOPDIR)/modem/system/rtosck_smp/smpv2r2
RTOSCK_NR_INCLUDE_PATH := $(BALONG_TOPDIR)/modem/include/rtosck_smp/smpv2r2
ifeq ($(modem_sanitizer),true)
RTOSCK_NR_OS_LIB_PATH := $(BALONG_TOPDIR)/build/delivery/$(OBB_PRODUCT_NAME)/lib/rtosck_os/smpv2r2_nr_asan
else
RTOSCK_NR_OS_LIB_PATH := $(BALONG_TOPDIR)/build/delivery/$(OBB_PRODUCT_NAME)/lib/rtosck_os/smpv2r2
endif
#hac
RTOSCK_HAC_OS_PATH := $(BALONG_TOPDIR)/modem/system/rtosck_smp/smpv2r2cut
RTOSCK_HAC_INCLUDE_PATH := $(BALONG_TOPDIR)/modem/include/rtosck_smp/smpv2r2cut

CFG_CONFIG_SAPPER_SETS := 
CFG_NR_MODEM_BUILD :=true

ifeq ($(chip_type),tc)
CFG_CHIP_TYPE_TC := FEATURE_ON
CFG_CONFIG_SAPPER_SETS += CFG_CHIP_TYPE_TC
endif

ifeq ($(chip_type),es)
CFG_CHIP_TYPE_ES := FEATURE_ON
CFG_CHIP_TYPE_CS := FEATURE_OFF
CFG_CONFIG_SAPPER_SETS += CFG_CHIP_TYPE_ES
CFG_CONFIG_SAPPER_SETS += CFG_CHIP_TYPE_CS
endif

ifeq ($(chip_type),)
CFG_CHIP_TYPE_CS := FEATURE_ON
CFG_CONFIG_SAPPER_SETS += CFG_CHIP_TYPE_CS
endif


ifeq ($(bbit_type),nr)
CFG_BBIT_TYPE_NR := FEATURE_ON
CFG_CONFIG_SAPPER_SETS += CFG_BBIT_TYPE_NR
endif

ifeq ($(emulate),)
CFG_UPHY_PLATFORM := UPHY_ASIC
endif

ifeq ($(emulate),esl)
CFG_UPHY_PLATFORM := UPHY_ASIC
endif

ifeq ($(emulate),porting)
CFG_UPHY_PLATFORM := UPHY_PORTING
endif

ifeq ($(emulate),sft)
CFG_UPHY_PLATFORM := UPHY_SFT
endif

ifeq ($(emulate),esl)
CFG_UPHY_PLATFORM := UPHY_ESL
CFG_EMU_TYPE_ESL := FEATURE_ON
CFG_CONFIG_SAPPER_SETS += CFG_EMU_TYPE_ESL
endif

ifeq ($(emulate),emu)
CFG_UPHY_PLATFORM := UPHY_EMU
CFG_EMU_TYPE_EMU := FEATURE_ON
CFG_CONFIG_SAPPER_SETS += CFG_EMU_TYPE_EMU
endif

ifeq ($(emulate),slt)
CFG_UPHY_PLATFORM := UPHY_PORTING
CFG_EMU_TYPE_SLT := FEATURE_ON
CFG_CONFIG_SAPPER_SETS += CFG_EMU_TYPE_SLT
endif

ifeq ($(emulate),eslps)
CFG_UPHY_PLATFORM := UPHY_ESL
CFG_EMU_TYPE_ESL := FEATURE_ON
CFG_EMU_TYPE_ESLPS := FEATURE_ON
CFG_CONFIG_SAPPER_SETS += CFG_EMU_TYPE_ESL
CFG_CONFIG_SAPPER_SETS += CFG_EMU_TYPE_ESLPS
endif

ifeq ($(lphy_porting),true)
CFG_LPHY_LATE_MODE := FEATURE_ON
endif

ifneq ($(extra_modem),)
CFG_EXTRA_MODEM_MODE_MBB := FEATURE_ON
CFG_CONFIG_SAPPER_SETS += CFG_EXTRA_MODEM_MODE_MBB
endif
