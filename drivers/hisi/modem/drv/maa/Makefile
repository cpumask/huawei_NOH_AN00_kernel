ifeq ($(strip $(llt_gcov)),y)
GCOV_PROFILE := y
endif

subdir-ccflags-y := -Werror

-include $(srctree)/drivers/hisi/modem/config/product/$(OBB_PRODUCT_NAME)/$(OBB_MODEM_CUST_CONFIG_DIR)/config/balong_product_config.mk
ifeq ($(strip $(CFG_CONFIG_MAA_BALONG)),YES)
ifeq ($(strip $(CFG_CONFIG_MAA_V3)),YES)
obj-y           += v3/maa_core.o v3/bsp_maa.o v3/maa_hal.o
else
obj-y           += v1/maa_acore.o v1/bsp_maa.o
endif
endif
