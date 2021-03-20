ifeq ($(strip $(TARGET_PRODUCT)), mini)
	ccflags-y += -DCONFIG_MINI_PLATFORM
endif

ifneq ($(TARGET_BUILD_VARIANT),user)
	ccflags-y += -DDEBUG_TUI
	ccflags-y += -DDEF_ENG
endif

obj-$(CONFIG_TZDRIVER) += smc_smp.o tc_client_driver.o tc_client_sub_driver.o mailbox_mempool.o
obj-$(CONFIG_TZDRIVER) += agent.o gp_ops.o mem.o cmdmonitor.o tzdebug.o tz_spi_notify.o tee_s4.o

ifeq ($(findstring y, $(CONFIG_TEEECD_AUTH) $(CONFIG_CLIENT_AUTH)), y)
	obj-$(CONFIG_TZDRIVER) += auth_base_impl.o
endif

obj-$(CONFIG_TEEECD_AUTH) += teec_daemon_auth.o
obj-$(CONFIG_CLIENT_AUTH) += client_hash_auth.o
obj-$(CONFIG_AUTH_ENHANCE) += security_auth_enhance.o
obj-$(CONFIG_STATIC_ION) += static_ion_mem.o
obj-$(CONFIG_DYNAMIC_ION) += dynamic_ion_mem.o
obj-$(CONFIG_TEELOG) += tlogger.o

ifeq ($(CONFIG_RDR_MEM),y)
	ifeq ($(CONFIG_MNTN),y)
		obj-$(CONFIG_MNTN) += log_bbox_cfg.o
	else
		obj-$(CONFIG_HISI_BB) += log_rdr_cfg.o
	endif
endif

obj-$(CONFIG_KERNEL_CLIENT) += teek_client_api.o

ifeq ($(CONFIG_TZDRIVER),y)
	obj-$(CONFIG_HISI_RPMB) += agent_rpmb.o
endif

obj-$(CONFIG_TEE_TUI) += tui.o

ccflags-y += -Idrivers/video/hisi/dss \
             -Idrivers/video/fbdev/hisi/dss \
             -Idrivers/staging/android \
             -Idrivers/tzdriver/libhwsecurec
