ifeq ($(wildcard $(srctree)/drivers/huawei_platform/inputhub/$(TARGET_BOARD_PLATFORM)),)
ccflags-y  += -I$(srctree)/drivers/huawei_platform/inputhub/default/
else
ccflags-y  += -I$(srctree)/drivers/huawei_platform/inputhub/$(TARGET_BOARD_PLATFORM)/
endif
EXTRA_CFLAGS += -Idrivers/hisi/tzdriver

obj-$(CONFIG_CONTEXTHUB_SWING) += swing_dev.o
obj-$(CONFIG_CONTEXTHUB_SWING_DBG) += swing_dbg_dev.o
obj-$(CONFIG_CONTEXTHUB_SWING_TOF) += swing_tof_dev.o
