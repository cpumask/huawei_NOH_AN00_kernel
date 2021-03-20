

#ifndef __HI_DRV_GPIO_H__
#define __HI_DRV_GPIO_H__

/* 其他头文件包含 */
#ifdef _PRE_HI_DRV_GPIO
#define BFGX_WKAEUP_HOST_GPIO (19 * 8 + 1)
#define BFGX_INT_ENABLE (1)
#define BFGX_INT_DISABLE (0)

#define GPIO_MUX_REG_ADDR 0xF8A21218
#define GPIO_MUX_REG_VALUE 0x20
#define GPIO_MUX_REG_OFFSET 4
#define SSI_CLK_HISI_GPIO  (7 * 8)
#define SSI_DATA_HISI_GPIO (6 * 8 + 7)

/* type of GPIO interrupt */
typedef enum hiBFGXUNF_GPIO_INTTYPE_E {
    BFGX_HI_UNF_GPIO_INTTYPE_UP,   /* spring by the up edge */
    BFGX_HI_UNF_GPIO_INTTYPE_DOWN, /* spring by the down edge */
    BFGX_HI_UNF_GPIO_INTTYPE_UPDOWN, /* spring by both the up and down edge */
    BFGX_HI_UNF_GPIO_INTTYPE_HIGH, /* spring by the high level */
    BFGX_HI_UNF_GPIO_INTTYPE_LOW, /* spring by the low level */
    BFGX_HI_UNF_GPIO_INTTYPE_BUTT, /* Invalid value */
} BFGX_HI_UNF_GPIO_INTTYPE_E;

#ifdef _PRE_PRODUCT_HI3751_PLATO
#define HI_DRV_MODULE_GetFunction hi_drv_module_get_function
#define HI_DRV_GPIO_UnregisterServerFunc hi_drv_gpio_unregister_server_func
#endif
extern int HI_DRV_MODULE_GetFunction(unsigned int u32ModuleID, void** ppFunc);

typedef enum hiMOD_ID_E {
    HI_ID_STB = 0,

    /*  common. */
    HI_ID_SYS = 1,
    HI_ID_MODULE,
    HI_ID_LOG,
    HI_ID_PROC,
    HI_ID_MEM,
    HI_ID_STAT,
    HI_ID_PDM,
    HI_ID_MEMDEV,
    HI_ID_CUSTOM,
    HI_ID_COMMON_BUTT,

    HI_ID_DEMUX = HI_ID_COMMON_BUTT + 1,
    HI_ID_DEMUX_BUTT,

    /*  audio. */
    HI_ID_ADEC = HI_ID_DEMUX_BUTT + 1,
    HI_ID_AO,
    HI_ID_SIO_AI,
    HI_ID_SIO_AO,
    HI_ID_SIO,
    HI_ID_AI,
    HI_ID_AENC,
    HI_ID_SRC,
    HI_ID_AIAO,
    HI_ID_AFLT,
    HI_ID_ADSP,
    HI_ID_AMP,
    HI_ID_SIF,
    HI_ID_TTS,
    HI_ID_AUDIO_BUTT,

    /* video. */
    HI_ID_VFMW = HI_ID_AUDIO_BUTT + 1,
    HI_ID_SVDEC,
    HI_ID_DISP,
    HI_ID_HDMI,
    HI_ID_VO,
    HI_ID_FRC,
    HI_ID_VPSS,
    HI_ID_VDEC,
    HI_ID_VI,
    HI_ID_VENC,
    HI_ID_PQ,
    HI_ID_EDID,
    HI_ID_MEMC,
    HI_ID_VICAP,
    HI_ID_PANEL,
    HI_ID_FDMNG,
    HI_ID_RM,
    HI_ID_VIDEO_BUTT,

    /* graphics. */
    HI_ID_TDE = HI_ID_VIDEO_BUTT + 1,
    HI_ID_JPGDEC,
    HI_ID_JPGENC,
    HI_ID_FB,
    HI_ID_PNG,
    HI_ID_HIGO,
    HI_ID_GRAPHICS_BUTT,

    /* player. */
    HI_ID_PVR = HI_ID_GRAPHICS_BUTT + 1,
    HI_ID_AVPLAY,
    HI_ID_SYNC,
    HI_ID_VSYNC,
    HI_ID_ASYNC,
    HI_ID_FASTPLAY,
    HI_ID_PLAYER_BUTT,

    /* ecs. */
    HI_ID_FLASH = HI_ID_PLAYER_BUTT + 1,
    HI_ID_IR,
    HI_ID_RTC,
    HI_ID_I2C,
    HI_ID_SCI,
    HI_ID_ETH,
    HI_ID_USB_PROTECT,
    HI_ID_WDG,   /* watch dog used 'W' */
    HI_ID_GPIO,
    HI_ID_GPIO_I2C,
    HI_ID_DMAC,
    HI_ID_PMOC,
    HI_ID_TUNER,
    HI_ID_KEYLED,
    HI_ID_CIPHER,
    HI_ID_OTP,
    HI_ID_CA,
    HI_ID_PM,
    HI_ID_CI,
    HI_ID_CIMAXPLUS,
    HI_ID_TVP5150,
    HI_ID_SIL9293,
    HI_ID_PWM,
    HI_ID_LSADC,
    HI_ID_SPI,
    HI_ID_ECS_BUTT,

    /* ATV */
    HI_ID_VFE = HI_ID_ECS_BUTT + 1,
    HI_ID_TVD,
    HI_ID_HDDEC,
    HI_ID_HDMIRX,
    HI_ID_VBI,
    HI_ID_ATV_BUTT,

    /* voip, bluetooth,alsa. */
    HI_ID_VOIP_HME = HI_ID_ATV_BUTT + 1,
    HI_ID_NDPT,
    HI_ID_AUDADP,
    HI_ID_BT,
    HI_ID_ALSA,
    HI_ID_3G,
    HI_ID_KARAOKE,
    HI_ID_VOIP_BUTT,

    /* vp. */
    HI_ID_VP = HI_ID_VOIP_BUTT + 1,
    HI_ID_HDCP,
    HI_ID_VP_BUTT,

    /* subtitle. */
    HI_ID_SUBT = HI_ID_VP_BUTT + 1,
    HI_ID_TTX,
    HI_ID_CC,
    HI_ID_SUBTITLE_BUTT,

    /* loader. */
    HI_ID_LOADER = HI_ID_SUBTITLE_BUTT + 1,
    HI_ID_LOADER_BUTT,

    /* user definition. */
    HI_ID_USR_START = HI_ID_LOADER_BUTT + 1,
    HI_ID_USER,
    HI_ID_USR_END = 0xFE,    /* Caution: to avoid overflow */

    HI_ID_BUTT = 0xFF
} HI_MOD_ID_E;

typedef struct hiGPIO_GET_GPIONUM_S {
    unsigned char u8GpioGrpNum;     /* gpio group number */
    unsigned char u8GpioMaxNum;     /* gpio  max number */
} GPIO_GET_GPIONUM_S;

extern int HI_DRV_GPIO_UnregisterServerFunc(unsigned int u32GpioNo);

typedef int (*FN_GPIO_Get_Bit)(unsigned int, unsigned int*);
typedef int (*FN_GPIO_Set_Bit)(unsigned int, unsigned int);
typedef int (*FN_GPIO_Get_Num)(GPIO_GET_GPIONUM_S*);

typedef int (*FN_GPIO_Register_Server_Func)(unsigned int, void (*func)(unsigned int));
typedef int (*FN_GPIO_UnRegister_Server_Func)(unsigned int);
typedef int (*FN_GPIO_Set_Int_Type)(unsigned int, HI_UNF_GPIO_INTTYPE_E);
typedef int (*FN_GPIO_Set_Int_Enable)(unsigned int, bool);
typedef int (*FN_GPIO_Clear_GroupInt)(unsigned int);
typedef int (*FN_GPIO_Clear_BitInt)(unsigned int);
typedef int (*FN_GPIO_Get_UsrAddr)(unsigned int, void **);

typedef struct {
    FN_GPIO_Get_Bit                pfnGpioDirGetBit;
    FN_GPIO_Set_Bit                pfnGpioDirSetBit;
    FN_GPIO_Get_Bit                pfnGpioReadBit;
    FN_GPIO_Set_Bit                pfnGpioWriteBit;
    FN_GPIO_Get_Num                pfnGpioGetNum;
    FN_GPIO_Register_Server_Func   pfnGpioRegisterServerFunc;
    FN_GPIO_UnRegister_Server_Func pfnGpioUnRegisterServerFunc;
    FN_GPIO_Set_Int_Type           pfnGpioSetIntType;
    FN_GPIO_Set_Int_Enable         pfnGpioSetIntEnable;
    FN_GPIO_Clear_GroupInt         pfnGpioClearGroupInt;
    FN_GPIO_Clear_BitInt           pfnGpioClearBitInt;
    FN_GPIO_Get_UsrAddr            pfnGPIOGetUsrAddr;
} GPIO_EXT_FUNC_S;

typedef enum hiUNF_GPIO_INTTYPE_E {
    HI_UNF_GPIO_INTTYPE_UP, /* <spring by the up edge */
    HI_UNF_GPIO_INTTYPE_DOWN, /* <spring by the down edge */
    HI_UNF_GPIO_INTTYPE_UPDOWN, /* <spring by both the up and down edge */
    HI_UNF_GPIO_INTTYPE_HIGH, /* <spring by the high level */
    HI_UNF_GPIO_INTTYPE_LOW, /* <spring by the low level */
    HI_UNF_GPIO_INTTYPE_BUTT, /* <Invalid value */
} HI_UNF_GPIO_INTTYPE_E;

#ifdef _PRE_PRODUCT_HI3751_PLATO
#define HI_DRV_GPIO_SetDirBit hi_drv_gpio_set_dir_bit
#define HI_DRV_GPIO_WriteBit hi_drv_gpio_write_bit
#define HI_DRV_GPIO_ReadBit hi_drv_gpio_read_bit
#define HI_DRV_GPIO_SetIntType hi_drv_gpio_set_int_type
#define HI_DRV_GPIO_SetBitIntEnable hi_drv_gpio_set_bit_int_enable
#define HI_DRV_GPIO_UnregisterServerFunc hi_drv_gpio_unregister_server_func
#define HI_DRV_GPIO_RegisterServerFunc hi_drv_gpio_register_server_func
#define HI_DRV_GPIO_ClearBitInt hi_drv_gpio_clear_bit_int
#endif
extern int HI_DRV_GPIO_SetDirBit(unsigned int u32GpioNo, unsigned int u32DirBit);
extern int HI_DRV_GPIO_WriteBit(unsigned int u32GpioNo, unsigned int u32BitValue);
extern int HI_DRV_GPIO_ReadBit(unsigned int u32GpioNo, unsigned int* pu32BitValue);
extern int HI_DRV_GPIO_SetIntType(unsigned int u32GpioNo, HI_UNF_GPIO_INTTYPE_E enIntTriMode);
extern int HI_DRV_GPIO_SetBitIntEnable(unsigned int u32GpioNo, unsigned int bEnable);
extern int HI_DRV_GPIO_UnregisterServerFunc(unsigned int u32GpioNo);
extern int HI_DRV_GPIO_RegisterServerFunc(unsigned int u32GpioNo, int (*func)(unsigned int));
extern int HI_DRV_GPIO_ClearBitInt(unsigned int u32GpioNo);

OAL_STATIC OAL_INLINE unsigned int hitv_gpio_request(unsigned int gpio, int dir) /* 1:input 0:output */
{
    return HI_DRV_GPIO_SetDirBit(gpio, !!dir);
}

OAL_STATIC OAL_INLINE unsigned int hitv_gpio_get_value(unsigned int gpio)
{
    unsigned int value = 0;
    HI_DRV_GPIO_ReadBit(gpio, &value);
    return value;
}

OAL_STATIC OAL_INLINE int hitv_gpio_direction_output(unsigned int gpio, int value)
{
    return HI_DRV_GPIO_WriteBit(gpio, !!value);
}

OAL_STATIC OAL_INLINE void hitv_gpio_clear_irq(unsigned int gpio)
{
    (void)HI_DRV_GPIO_ClearBitInt(gpio);
}

OAL_STATIC OAL_INLINE unsigned int hitv_gpio_disable_irq(unsigned int gpio)
{
    return HI_DRV_GPIO_SetBitIntEnable(gpio, 0);
}

OAL_STATIC OAL_INLINE unsigned int hitv_gpio_enable_irq(unsigned int gpio)
{
    return HI_DRV_GPIO_SetBitIntEnable(gpio, 1);
}

OAL_STATIC OAL_INLINE unsigned int hitv_gpio_request_irq(int gpio, int (*func)(unsigned int),
                                                         HI_UNF_GPIO_INTTYPE_E mode, unsigned int param)
{
    HI_DRV_GPIO_SetIntType(gpio, mode);
    return HI_DRV_GPIO_RegisterServerFunc(gpio, func);
}

#endif

#endif
