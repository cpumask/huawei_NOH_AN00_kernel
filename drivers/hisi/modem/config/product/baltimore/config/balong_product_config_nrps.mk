# MD5: 87b8b7b2771ad4ff7f3a944f82e0ca32
CFG_FEATURE_ON                                  := 1
CFG_FEATURE_OFF                                 := 0
ifeq ($(EMU_TYPE_ESLPS),FEATURE_ON)
CFG_FEATURE_NPHY_STUB_ESL := FEATURE_ON
else
CFG_FEATURE_NPHY_STUB_ESL := FEATURE_OFF
endif
ifeq ($(EMU_TYPE_ESL),FEATURE_ON)
CFG_FEATURE_NRRC_EMU_ESL := FEATURE_ON
else
CFG_FEATURE_NRRC_EMU_ESL := FEATURE_OFF
endif
CFG_FEATURE_NL2_MAA_ALLOC := FEATURE_ON
CFG_NR_PROTOL_STACK_ENG := YES
CFG_NR_MAX_SERVING_CC_NUM    := 2
CFG_NR_MAX_UL_SERVING_CC_NUM := 2
CFG_NR_MAX_CG_NUM    := 1
CFG_FEATURE_MODEM1_SUPPORT_NR := FEATURE_OFF
CFG_NR_MAX_PER_PLMN_NRSA_BC_NUM := 128
CFG_NR_MAX_PER_PLMN_ENDC_BC_NUM := 1536
CFG_NR_MAX_PER_PLMN_NRDC_BC_NUM := 16
CFG_NR_MAX_NRSA_BC_NUM := 512
CFG_NR_MAX_ENDC_BC_NUM := 2048
CFG_NR_MAX_NRDC_BC_NUM := 16
CFG_NR_MAX_NR_FSD_OTHER_PARA_NUM:=128
CFG_NR_MAX_NR_FSU_OTHER_PARA_NUM:=128
CFG_NR_MAX_NR_FSD_PARA_NUM:=256
CFG_NR_MAX_NR_FSU_PARA_NUM:=256
CFG_NR_MAX_LTE_FSD_PARA_NUM:=256
CFG_NR_MAX_LTE_FSU_PARA_NUM:=256
CFG_NR_MAX_NR_FSPC_DL_NUM:=128
CFG_NR_MAX_NR_FSPC_UL_NUM:=128
CFG_NR_MAX_PER_BC_NR_BAND_NUM := 3
CFG_NR_MAX_PER_BC_LTE_BAND_NUM := 5
CFG_NR_MAX_PER_BC_BAND_NUM := 6
CFG_NR_MAX_PER_BAND_CC_NUM := 6
CFG_FEATURE_NR_R16 := FEATURE_OFF
CFG_FEATURE_NR_R16_TODO := FEATURE_OFF
