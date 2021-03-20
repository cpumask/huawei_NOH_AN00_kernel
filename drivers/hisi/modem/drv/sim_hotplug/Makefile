
-include $(srctree)/drivers/hisi/modem/config/product/$(OBB_PRODUCT_NAME)/$(OBB_MODEM_CUST_CONFIG_DIR)/config/balong_product_config.mk

ifeq ($(strip $(llt_gcov)),y)
GCOV_PROFILE := y
endif


ifeq ($(strip $(CFG_CONFIG_SCI_DECP)),YES)
obj-$(CONFIG_HISI_SIM_HOTPLUG_SPMI)     += hisi_sim_hotplug.o
obj-$(CONFIG_HISI_SIM_HOTPLUG_SPMI)     += hisi_sim_hw_mgr.o
obj-$(CONFIG_HISI_SIM_HOTPLUG_SPMI)     += hisi_sim_hw_service.o
else
obj-$(CONFIG_HISI_SIM_HOTPLUG_SPMI)     += hisi_sim_hotplug_legacy.o
endif
