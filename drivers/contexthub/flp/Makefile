#
# FLP PDR configuration
#
ccflags-$(CONFIG_EXT_INPUTHUB) += -I$(srctree)/drivers/huawei_platform/sensor/ext_inputhub/default
ifeq ($(wildcard $(srctree)/drivers/huawei_platform/inputhub/$(TARGET_BOARD_PLATFORM)),)
ccflags-y  += -I$(srctree)/drivers/huawei_platform/inputhub/default/
else
ccflags-y  += -I$(srctree)/drivers/huawei_platform/inputhub/$(TARGET_BOARD_PLATFORM)/
endif
EXTRA_CFLAGS += -Idrivers/hisi/tzdriver
obj-$(CONFIG_HISI_SOFTTIMER) += hisi_softtimer.o
obj-$(CONFIG_HISI_AR) += hismart_ar.o
obj-$(CONFIG_HISI_FLP) += hisi_flp.o
obj-$(CONFIG_HISI_FLP) += hisi_flp_dt.o
obj-$(CONFIG_EXT_INPUTHUB) += hisi_1135_ar.o
