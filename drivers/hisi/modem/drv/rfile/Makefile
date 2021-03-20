ifeq ($(strip $(llt_gcov)),y)
GCOV_PROFILE := y
endif
-include $(srctree)/drivers/hisi/modem/config/product/$(OBB_PRODUCT_NAME)/$(OBB_MODEM_CUST_CONFIG_DIR)/config/balong_product_config.mk 

EXTRA_CFLAGS += -Idrivers/hisi/tzdriver

ifneq ($(strip $(CFG_CONFIG_RFILE_ON)),YES)
obj-y += adp_rfile_stub.o
else
obj-y  += adp_rfile.o
ifneq ($(strip $(CFG_CONFIG_RFILE_USER)),YES)
obj-y  += rfile_balong.o
else
obj-y  += rfile_balong_user.o
ifeq ($(strip $(CFG_CONFIG_ICC)),YES)
obj-y  += rfile_icc.o
else
obj-y  += rfile_eicc.o
endif
endif
endif
obj-$(CONFIG_ENABLE_TEST_CODE)  += rfile_api_test.o
