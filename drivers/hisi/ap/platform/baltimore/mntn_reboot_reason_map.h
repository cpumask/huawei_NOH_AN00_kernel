#include "mntn_public_interface.h"
#include "mntn_subtype_exception.h"
#ifndef __REBOOT_REASON_MAP
#define __REBOOT_REASON_MAP(x,y) 
#endif
__REBOOT_REASON_MAP(AP_S_COLDBOOT, NORMALBOOT)
__REBOOT_REASON_MAP(bootloader, NORMALBOOT)
__REBOOT_REASON_MAP(recovery, NORMALBOOT)
__REBOOT_REASON_MAP(fastbootd, NORMALBOOT)
__REBOOT_REASON_MAP(resetfactory, NORMALBOOT)
__REBOOT_REASON_MAP(at2resetfactory, NORMALBOOT)
__REBOOT_REASON_MAP(atfactoryreset0, NORMALBOOT)
__REBOOT_REASON_MAP(resetuser, NORMALBOOT)
__REBOOT_REASON_MAP(sdupdate, NORMALBOOT)
__REBOOT_REASON_MAP(chargereboot, NORMALBOOT)
__REBOOT_REASON_MAP(resize, NORMALBOOT)
__REBOOT_REASON_MAP(erecovery, NORMALBOOT)
__REBOOT_REASON_MAP(usbupdate, NORMALBOOT)
__REBOOT_REASON_MAP(cust, NORMALBOOT)
__REBOOT_REASON_MAP(oem_rtc, NORMALBOOT)
__REBOOT_REASON_MAP(UNKNOWN, NORMALBOOT)
__REBOOT_REASON_MAP(mountfail, NORMALBOOT)
__REBOOT_REASON_MAP(hungdetect, NORMALBOOT)
__REBOOT_REASON_MAP(COLDBOOT, NORMALBOOT)
__REBOOT_REASON_MAP(updatedataimg, NORMALBOOT)
__REBOOT_REASON_MAP(AP_S_FASTBOOTFLASH, NORMALBOOT)
__REBOOT_REASON_MAP(AP_S_PRESS6S, PRESS6S)
__REBOOT_REASON_MAP(AP_S_PRESSKEY_COUNT, NORMALBOOT)
__REBOOT_REASON_MAP(gpscoldboot, NORMALBOOT)
__REBOOT_REASON_MAP(AP_S_ABNORMAL, UNKNOWN)
__REBOOT_REASON_MAP(AP_S_TSENSOR0, HARDWARE_FAULT)
__REBOOT_REASON_MAP(AP_S_TSENSOR1,HARDWARE_FAULT)
__REBOOT_REASON_MAP(AP_S_AWDT, SUBTYPE)
__REBOOT_REASON_MAP(CHARGER_S_WDT, HWWATCHDOG)
__REBOOT_REASON_MAP(G3D_S_G3DTSENSOR, HARDWARE_FAULT)
__REBOOT_REASON_MAP(LPM3_S_LPMCURST, LPM3EXCEPTION)
__REBOOT_REASON_MAP(CP_S_CPTSENSOR, HARDWARE_FAULT)
__REBOOT_REASON_MAP(IOM3_S_IOMCURST,SENSORHUBCRASH)
__REBOOT_REASON_MAP(ASP_S_ASPWD, HIFICRASH)
__REBOOT_REASON_MAP(CP_S_CPWD, MODEM_REBOOTSYS)
__REBOOT_REASON_MAP(IVP_S_IVPWD, IVPCRASH)
__REBOOT_REASON_MAP(ISP_S_ISPWD, ISPCRASH)
__REBOOT_REASON_MAP(AP_S_DDR_UCE_WD,HARDWARE_FAULT)
__REBOOT_REASON_MAP(AP_S_DDR_FATAL_INTER, HARDWARE_FAULT)
__REBOOT_REASON_MAP(OCBC_S_WD, HWWATCHDOG)
__REBOOT_REASON_MAP(AP_S_PANIC, SUBTYPE)
__REBOOT_REASON_MAP(AP_S_NOC, PANIC)
__REBOOT_REASON_MAP(AP_S_RESUME_SLOWY, PANIC)
__REBOOT_REASON_MAP(AP_S_DDRC_SEC, SUBTYPE)
__REBOOT_REASON_MAP(AP_S_F2FS, PANIC)
__REBOOT_REASON_MAP(AP_S_COMBINATIONKEY, NORMALBOOT)
__REBOOT_REASON_MAP(AP_S_BL31_PANIC, SUBTYPE)
__REBOOT_REASON_MAP(AP_S_MAILBOX, PANIC)
__REBOOT_REASON_MAP(AP_S_HHEE_PANIC, PANIC)
__REBOOT_REASON_MAP(AP_S_SUBPMU, HARDWARE_FAULT)
__REBOOT_REASON_MAP(AP_S_VENDOR_PANIC, SUBTYPE)
__REBOOT_REASON_MAP(CP_S_MODEMDMSS,MODEM_REBOOTSYS)
__REBOOT_REASON_MAP(CP_S_MODEMAP, MODEM_REBOOTSYS)
__REBOOT_REASON_MAP(CP_S_MODEMNOC, MODEM_REBOOTSYS)
__REBOOT_REASON_MAP(CP_S_EXCEPTION, MODEMCRASH)
__REBOOT_REASON_MAP(CP_S_RESETFAIL, MODEM_REBOOTSYS)
__REBOOT_REASON_MAP(CP_S_NORMALRESET, NORMALBOOT)
__REBOOT_REASON_MAP(CP_S_RILD_EXCEPTION, MODEMCRASH)
__REBOOT_REASON_MAP(CP_S_DRV_EXC, MODEMCRASH)
__REBOOT_REASON_MAP(CP_S_PAM_EXC, MODEMCRASH)
__REBOOT_REASON_MAP(CP_S_GUAS_EXC, MODEMCRASH)
__REBOOT_REASON_MAP(CP_S_CTTF_EXC, MODEMCRASH)
__REBOOT_REASON_MAP(CP_S_CAS_CPROC_EXC, MODEMCRASH)
__REBOOT_REASON_MAP(CP_S_GUDSP_EXC, MODEMCRASH)
__REBOOT_REASON_MAP(CP_S_TLPS_EXC, MODEMCRASH)
__REBOOT_REASON_MAP(CP_S_TLDSP_EXC, MODEMCRASH)
__REBOOT_REASON_MAP(CP_S_CPHY_EXC, MODEMCRASH)
__REBOOT_REASON_MAP(CP_S_GUCNAS_EXC, MODEMCRASH)
__REBOOT_REASON_MAP(CP_S_3RD_EXCEPTION, MODEMCRASH)
__REBOOT_REASON_MAP(LPM3_S_EXCEPTION, SUBTYPE)
__REBOOT_REASON_MAP(SOCHIFI_S_EXCEPTION, HIFICRASH)
__REBOOT_REASON_MAP(AUDIO_CODEC_EXCEPTION, AUDIO_CODEC_CRASH)
__REBOOT_REASON_MAP(HIFI_S_RESETFAIL, HIFICRASH)
__REBOOT_REASON_MAP(ISP_S_EXCEPTION, ISPCRASH)
__REBOOT_REASON_MAP(IVP_S_EXCEPTION, IVPCRASH)
__REBOOT_REASON_MAP(IOM3_S_EXCEPTION, SENSORHUBCRASH)
__REBOOT_REASON_MAP(TEE_S_EXCEPTION, TRUSTZONECRASH)
__REBOOT_REASON_MAP(MMC_S_EXCEPTION, SUBTYPE)
__REBOOT_REASON_MAP(CODECHIFI_S_EXCEPTION, HIFICRASH)
__REBOOT_REASON_MAP(IOM3_S_USER_EXCEPTION, NORMALBOOT)
__REBOOT_REASON_MAP(HISEE_S_EXCEPTION, SUBTYPE)
__REBOOT_REASON_MAP(NPU_S_EXCEPTION, SUBTYPE)
__REBOOT_REASON_MAP(BFGX_S_EXCEPTION, SUBTYPE)
__REBOOT_REASON_MAP(DSS_S_EXCEPTION, SUBTYPE)
__REBOOT_REASON_MAP(WIFI_S_EXCEPTION, SUBTYPE)
__REBOOT_REASON_MAP(AP_S_PMU, SUBTYPE)
__REBOOT_REASON_MAP(AP_S_SMPL, HARDWARE_FAULT)
__REBOOT_REASON_MAP(AP_S_SCHARGER, SUBTYPE)
__REBOOT_REASON_MAP(BR_UPDATE_USB, NORMALBOOT)
__REBOOT_REASON_MAP(BR_UPDATA_SD_FORCE, NORMALBOOT)
__REBOOT_REASON_MAP(BR_KEY_VOLUMN_UP, NORMALBOOT)
__REBOOT_REASON_MAP(BR_PRESS_1S, NORMALBOOT)
__REBOOT_REASON_MAP(BR_PRESS_10S, PRESS10S)
__REBOOT_REASON_MAP(BR_CHECK_RECOVERY, NORMALBOOT)
__REBOOT_REASON_MAP(BR_CHECK_ERECOVERY, NORMALBOOT)
__REBOOT_REASON_MAP(BR_CHECK_SDUPDATE, NORMALBOOT)
__REBOOT_REASON_MAP(BR_CHECK_USBUPDATE, NORMALBOOT)
__REBOOT_REASON_MAP(BR_CHECK_RESETFACTORY, NORMALBOOT)
__REBOOT_REASON_MAP(BR_CHECK_HOTAUPDATE, NORMALBOOT)
__REBOOT_REASON_MAP(BR_POWERONNOBAT, NORMALBOOT)
__REBOOT_REASON_MAP(BR_NOGUI, NORMALBOOT)
__REBOOT_REASON_MAP(BR_FACTORY_VERSION, NORMALBOOT)
__REBOOT_REASON_MAP(BR_RESET_HAPPEN, NORMALBOOT)
__REBOOT_REASON_MAP(BR_POWEROFF_ALARM, NORMALBOOT)
__REBOOT_REASON_MAP(BR_POWEROFF_CHARGE, NORMALBOOT)
__REBOOT_REASON_MAP(BR_POWERON_BY_SMPL, NORMALBOOT)
__REBOOT_REASON_MAP(BR_CHECK_UPDATEDATAIMG, NORMALBOOT)
__REBOOT_REASON_MAP(BR_REBOOT_CPU_BUCK, HARDWARE_FAULT)
__REBOOT_REASON_MAP(BR_L2_CACHE_FAIL, HARDWARE_FAULT)
__REBOOT_REASON_MAP(BR_POWERON_CHARGE, NORMALBOOT)
__REBOOT_REASON_MAP(XLOADER_S_DDRINIT_FAIL, HARDWARE_FAULT)
__REBOOT_REASON_MAP(XLOADER_S_EMMCINIT_FAIL, HARDWARE_FAULT)
__REBOOT_REASON_MAP(XLOADER_S_LOAD_FAIL, BOOTLOADER_CRASH)
__REBOOT_REASON_MAP(XLOADER_S_VERIFY_FAIL, BOOTLOADER_CRASH)
__REBOOT_REASON_MAP(XLOADER_S_WATCHDOG, BOOTLOADER_CRASH)
__REBOOT_REASON_MAP(XLOADER_INSE_S_PANIC, HISEECRASH)
__REBOOT_REASON_MAP(XLOADER_MEMORY_REPAIR, HARDWARE_FAULT)
__REBOOT_REASON_MAP(XLOADER_AUTH_USB_FAIL, BOOTLOADER_CRASH)
__REBOOT_REASON_MAP(FASTBOOT_EMMCINIT_FAIL, HARDWARE_FAULT)
__REBOOT_REASON_MAP(FASTBOOT_S_PANIC, BOOTLOADER_CRASH)
__REBOOT_REASON_MAP(FASTBOOT_S_SOC_SPEC, HARDWARE_FAULT)
__REBOOT_REASON_MAP(FASTBOOT_S_WATCHDOG, BOOTLOADER_CRASH)
__REBOOT_REASON_MAP(FASTBOOT_OCV_VOL_ERR, HARDWARE_FAULT)
__REBOOT_REASON_MAP(FASTBOOT_BAT_TEMP_ERR, HARDWARE_FAULT)
__REBOOT_REASON_MAP(FASTBOOT_MISC_ERR, BOOTLOADER_CRASH)
__REBOOT_REASON_MAP(FASTBOOT_LD_DTIMG_ERR, BOOTLOADER_CRASH)
__REBOOT_REASON_MAP(FASTBOOT_LD_OTHRIMG_ERR, BOOTLOADER_CRASH)
__REBOOT_REASON_MAP(FASTBOOT_KERNELIMG_ERR, BOOTLOADER_CRASH)
__REBOOT_REASON_MAP(FASTBOOT_LOADLPMCU_FAIL, BOOTLOADER_CRASH)
__REBOOT_REASON_MAP(FASTBOOT_VERIFY_FAIL, BOOTLOADER_CRASH)
__REBOOT_REASON_MAP(FASTBOOT_SOC_TEMP_ERR, HARDWARE_FAULT)
__REBOOT_REASON_MAP(FASTBOOT_FLASHCERT_FAIL, HARDWARE_FAULT)
__REBOOT_REASON_MAP(FASTBOOT_MULCOREON_FAIL, BOOTFAIL)
__REBOOT_REASON_MAP(FASTBOOT_MULCOREOFF_FAIL, BOOTFAIL)
__REBOOT_REASON_MAP(BFM_S_NATIVE_BOOT_FAIL, BOOTFAIL)
__REBOOT_REASON_MAP(BFM_S_BOOT_TIMEOUT, BOOTFAIL)
__REBOOT_REASON_MAP(BFM_S_FW_BOOT_FAIL, BOOTFAIL)
__REBOOT_REASON_MAP(BFM_S_NATIVE_DATA_FAIL, BOOTFAIL)
__REBOOT_REASON_MAP(IOM3_S_FDUL_EXCEPTION, FDULCRASH)
