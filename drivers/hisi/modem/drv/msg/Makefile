#gcov
ifeq ($(strip $(llt_gcov)),y)
GCOV_PROFILE := y
GCOV_PROFILE_msg_test.o := n
endif

-include $(srctree)/drivers/hisi/modem/config/product/$(OBB_PRODUCT_NAME)/$(OBB_MODEM_CUST_CONFIG_DIR)/config/balong_product_config.mk

subdir-ccflags-y := -Wall -Werror
obj-y       += msg_base.o
obj-y       += msg_tskque.o
obj-y       += msg_event.o
obj-y       += msg_mem.o
obj-y       += msg_core.o
obj-y       += msg_cmsg.o
obj-y       += msg_fixed.o
obj-y       += msg_mntn.o
obj-y       += msg_test.o
