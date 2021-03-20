#
# Makefile for the memory technology device drivers.
#
# Core functionality.
-include $(srctree)/drivers/hisi/modem/config/product/$(OBB_PRODUCT_NAME)/$(OBB_MODEM_CUST_CONFIG_DIR)/config/balong_product_config.mk

ifeq ($(strip $(llt_gcov)),y)
GCOV_PROFILE := y
endif

#A+B CP
ifeq ($(strip $(CFG_DIAG_SYSTEM_A_PLUS_B_CP)),YES)
obj-y				+= ppm_port_switch.o
obj-y				+= ppm_modem_agent.o
obj-y				+= ppm_common.o 
else
#A+B AP
ifeq ($(strip $(CFG_DIAG_SYSTEM_A_PLUS_B_AP)),YES)
obj-y				+= ppm_port_switch.o
obj-y				+= ppm_app_agent.o
obj-y				+= ppm_socp_agent.o
obj-y				+= ppm_common.o
obj-y				+= ppm_socket.o
obj-y				+= ppm_usb.o 
obj-y				+= ppm_vcom.o
else
#SOC
obj-y				+= ppm_port_switch.o
obj-y				+= ppm_common.o 
obj-y				+= ppm_socket.o 
obj-y				+= ppm_usb.o 
obj-y				+= ppm_vcom.o 
endif
endif


