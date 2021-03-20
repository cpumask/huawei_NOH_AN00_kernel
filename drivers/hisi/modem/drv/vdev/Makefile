ifeq ($(strip $(llt_gcov)),y)
GCOV_PROFILE := y
endif

 -include $(srctree)/drivers/hisi/modem/config/product/$(OBB_PRODUCT_NAME)/$(OBB_MODEM_CUST_CONFIG_DIR)/config/balong_product_config.mk

obj-y        += vdev_core.o vdev_app.o vdev_relay.o
obj-y        += vdev_msg.o vdev_atp.o
subdir-ccflags-y := -Werror
