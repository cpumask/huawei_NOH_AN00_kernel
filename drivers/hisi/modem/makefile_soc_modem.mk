-include $(srctree)/drivers/hisi/modem/config/product/$(OBB_PRODUCT_NAME)/$(OBB_MODEM_CUST_CONFIG_DIR)/config/balong_product_config.mk
subdir-ccflags-y += -I$(srctree)/lib/libc_sec/securec_v2/include/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/tzdriver/libhwsecurec/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/platform/ccore/$(CFG_PLATFORM)/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/platform/dsp/$(CFG_PLATFORM)/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/platform/acore/$(CFG_PLATFORM)/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/platform/acore/$(CFG_PLATFORM)/drv/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/platform/common/$(CFG_PLATFORM)/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/platform/common/$(CFG_PLATFORM)/soc/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/adrv/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/drv/acore/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/drv/common/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/drv/common/include/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/drv/include/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/tl/drv/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/product/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/tl/oam/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/tl/lps/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/acore/sys/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/acore/drv/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/acore/msp/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/acore/pam/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/acore/guc_as/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/acore/guc_nas/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/common/sys/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/common/drv/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/common/msp/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/common/pam/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/common/guc_as/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/common/guc_nas/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/nv/common/tl_as
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/phy/lphy/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/config/nvim/include/gu/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/taf/common/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/include/taf/acore/
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/config/product/$(OBB_PRODUCT_NAME)/$(OBB_MODEM_CUST_CONFIG_DIR)/config
subdir-ccflags-y += -I$(srctree)/drivers/hisi/modem/config/msg/

ifeq ($(strip $(OBB_LLT_MDRV)),y)
subdir-ccflags-y += -DDRV_BUILD_LLT
endif

ifeq ($(LLT_BUILD_GCOV), y)
subdir-ccflags-y += -DDRV_BUILD_GCOV
endif

obj-$(CONFIG_HISI_BALONG_MODEM) +=drv/

ifneq ($(chip_type),es)
KBUILD_CFLAGS += -DCHIP_TYPE_CS=FEATURE_ON
endif

ifeq ($(secdog),rsa2048)
KBUILD_CFLAGS += -DSECDOG_SUPPORT_RSA_2048=FEATURE_ON
endif


ifeq ($(ota_netlock),true)
KBUILD_CFLAGS += -DFEATURE_OTA_NETLOCK=FEATURE_ON
endif

ifeq ($(simlock_cust),true)
KBUILD_CFLAGS += -DFEATURE_SIMLOCK_CUST=FEATURE_ON
endif

KBUILD_CFLAGS += -DMODULE_EXPORTED=

ifneq ($(OBB_SEPARATE),true)

ifneq ($(CONFIG_MODEM_PS_FUSION_VERSION),y)
ifeq ($(CFG_NAS_NEW_ARCH), YES)
obj-$(CONFIG_HISI_BALONG_MODEM) +=oam/ ps/ tafnew/
else
obj-$(CONFIG_HISI_BALONG_MODEM) +=oam/ ps/ taf/
endif
else
obj-$(CONFIG_HISI_BALONG_MODEM) +=ps/gu/ ps/tl/ ps/gu/psp/ tafnew/
endif

#ifeq ($(CFG_FEATURE_IMS),FEATURE_ON)
obj-$(CONFIG_HISI_BALONG_MODEM) +=ims/
#endif

endif

#add modem user
ifeq ($(choose_variant_modem),user)
KBUILD_CFLAGS += -DCHOOSE_MODEM_USER
endif

ifeq ($(cdma),false)
KBUILD_CFLAGS += -DFEATURE_UE_MODE_CDMA=FEATURE_OFF
KBUILD_CFLAGS += -DFEATURE_CSDR=FEATURE_OFF
KBUILD_CFLAGS += -DFEATURE_BALONG_CL=FEATURE_OFF
endif


ifeq ($(strip $(CONFIG_HISI_BALONG_MODEM)),m)

BALONG_MODEM_MODULE := balong_modem
$(BALONG_MODEM_MODULE)-y :=

ifneq ($(strip $(CONFIG_HISI_AP_CP_DECOUPL)),y)
obj-y	+= modem_symbol.o
endif

# overwrite obj-m
obj-m	:= $(BALONG_MODEM_MODULE).o

subdir-ccflags-y += -DCONFIG_HISI_BALONG_MODEM -Wno-unused-function -Wno-undef

# drv
include $(srctree)/drivers/hisi/modem/drv/Makefile
$(BALONG_MODEM_MODULE)-y += $(addprefix drv/, $(drv-y))
obj-y	+= $(addprefix drv/, $(drv-builtin))

ifneq ($(OBB_SEPARATE),true)
ifneq ($(CONFIG_MODEM_PS_FUSION_VERSION),y)
# gumsp
ifeq ($(CONFIG_PAM_NR_MODEM_BUILD),y)
-include $(srctree)/drivers/hisi/modem/oam/nr/Kbuild
$(BALONG_MODEM_MODULE)-y += $(addprefix oam/nr/, $(gumsp-objs))
else
-include $(srctree)/drivers/hisi/modem/oam/gu/Kbuild
$(BALONG_MODEM_MODULE)-y += $(addprefix oam/gu/, $(gumsp-objs))
endif
# tlmsp^M
-include $(srctree)/drivers/hisi/modem/oam/lt/acore/Makefile
$(BALONG_MODEM_MODULE)-y += $(addprefix oam/lt/acore/, $(tlmsp-y))
else
# psp
-include $(srctree)/drivers/hisi/modem/ps/gu/psp/Kbuild
$(BALONG_MODEM_MODULE)-y += $(addprefix ps/gu/psp/, $(psp-objs))
endif

# gups
-include $(srctree)/drivers/hisi/modem/ps/gu/Kbuild
$(BALONG_MODEM_MODULE)-y += $(addprefix ps/gu/, $(gups-objs))

# tlps
-include $(srctree)/drivers/hisi/modem/ps/tl/Kbuild
$(BALONG_MODEM_MODULE)-y += $(addprefix ps/tl/, $(tlps-objs))

# taf
ifeq ($(CFG_NAS_NEW_ARCH), YES)
-include $(srctree)/drivers/hisi/modem/tafnew/Kbuild
$(BALONG_MODEM_MODULE)-y += $(addprefix tafnew/, $(taf-objs))
else
-include $(srctree)/drivers/hisi/modem/taf/Kbuild
$(BALONG_MODEM_MODULE)-y += $(addprefix taf/, $(taf-objs))
endif

# ims
#ifeq ($(CFG_FEATURE_IMS),FEATURE_ON)
-include $(srctree)/drivers/hisi/modem/ims/Makefile
$(BALONG_MODEM_MODULE)-y += $(addprefix ims/, $(ims-objs))
#endif
endif
endif
