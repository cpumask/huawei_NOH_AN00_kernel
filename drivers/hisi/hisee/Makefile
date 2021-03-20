EXTRA_CFLAGS += -Idrivers/hisi/rpmb
EXTRA_CFLAGS += -Idrivers/hisi/mntn/blackbox/platform_lpm3
EXTRA_CFLAGS += -Idrivers/hisi/tzdriver
EXTRA_CFLAGS += -Idrivers/hisi/mntn/hisee

subdir-ccflags-y              += -Wall -Werror

ifneq ($(CONFIG_HISEE_DISABLE_KEY),y)
ifeq ($(TARGET_VERSION_MODE),factory)
EXTRA_CFLAGS += -DCONFIG_HISI_HISEE_CHIPTEST_RT
endif

ifneq ($(TARGET_BUILD_VARIANT),user)
EXTRA_CFLAGS += -DCONFIG_HISI_HISEE_CHIPTEST_SLT
endif
endif

obj-$(CONFIG_HISI_HISEE)  += hisi_hisee.o
obj-$(CONFIG_HISI_HISEE)  += hisi_hisee_power.o
obj-$(CONFIG_HISI_HISEE)  += hisi_hisee_upgrade.o
obj-$(CONFIG_HISI_HISEE)  += hisi_hisee_chip_test.o
obj-$(CONFIG_HISI_HISEE)  += hisi_hisee_fs.o
obj-$(CONFIG_HISI_HISEE_SUPPORT_CASDKEY) += hisi_casdcert.o
obj-$(CONFIG_HISEE_SUPPORT_DCS) += hisi_hisee_dcs.o

ifeq ($(CONFIG_HISI_HISEE),y)
obj-$(CONFIG_HISI_DEBUG_FS)  += hisi_hisee_autotest.o
endif
