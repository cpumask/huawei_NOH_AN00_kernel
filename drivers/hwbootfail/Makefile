# Makefile for the Boot Detector modules.

obj-$(CONFIG_BOOT_DETECTOR) += \
	core/boot_detector.o \
	core/boot_recovery.o \
	common/bootfail_common.o \
	common/adapter_common.o \
	common/bootfail_timer.o

ifeq ($(CONFIG_BOOT_DETECTOR_HISI),y)
obj-$(CONFIG_BOOT_DETECTOR) += chipsets/hisi/bootfail_hisi.o
obj-$(CONFIG_BOOT_DETECTOR) += chipsets/hisi/adapter_hisi.o
obj-$(CONFIG_BOOT_DETECTOR) += chipsets/hisi/ocp_hisi.o
endif

ifeq ($(CONFIG_BOOT_DETECTOR_HONGHU),y)
obj-$(CONFIG_BOOT_DETECTOR) += chipsets/honghu/bootfail_honghu.o
obj-$(CONFIG_BOOT_DETECTOR) += chipsets/honghu/adapter_honghu.o
endif
