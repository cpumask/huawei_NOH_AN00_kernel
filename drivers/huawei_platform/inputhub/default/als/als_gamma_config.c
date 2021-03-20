/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: als gamma config source file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#include <linux/module.h>
#include <linux/err.h>
#include <linux/types.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/rtc.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include "contexthub_route.h"
#include "contexthub_boot.h"
#include "contexthub_recovery.h"
#include "protocol.h"
#include "sensor_config.h"
#include "sensor_detect.h"
#include "sensor_sysfs.h"
#include "als_channel.h"
#include "als_gamma_config.h"
#ifdef DSS_ALSC_NOISE
#include "display_effect_alsc_interface.h"
#endif
#include <securec.h>

#define DEGAMMA_COEF_LEN 257

enum sensor_als_gamma_type{
	GAMMA_TYPE_DEFAULT,
	GAMMA_TYPE_O_BOE = 0x31,
	GAMMA_TYPE_N_BOE = 0x41,
	GAMMA_TYPE_N_SM = 0x42,
	GAMMA_TYPE_N_LG = 0x43,
	GAMMA_TYPE_NP_BOE = 0x51,
	GAMMA_TYPE_NP_SM = 0x52,
	GAMMA_TYPE_NP_LG = 0x53,
};

#ifdef DSS_ALSC_NOISE
static u32 g_gamma_lut_table_1_6[DEGAMMA_COEF_LEN] = {
	0x0000, 0x0001, 0x0003, 0x0003, 0x0004, 0x0004, 0x0004, 0x0005,
	0x0005, 0x0006, 0x0006, 0x0006, 0x0006, 0x0007, 0x0007, 0x0007,
	0x0007, 0x0007, 0x0007, 0x0008, 0x0008, 0x0011, 0x0011, 0x0019,
	0x0019, 0x0022, 0x002a, 0x0033, 0x0044, 0x0044, 0x0055, 0x0055,
	0x0066, 0x0066, 0x0076, 0x007f, 0x0087, 0x0098, 0x00a1, 0x00a9,
	0x00b2, 0x00ba, 0x00cb, 0x00d3, 0x00dc, 0x00e4, 0x00f5, 0x00fe,
	0x0106, 0x0117, 0x011f, 0x0128, 0x0130, 0x0141, 0x014a, 0x0152,
	0x0163, 0x016b, 0x0174, 0x0185, 0x0185, 0x0196, 0x01a6, 0x01a6,
	0x01b7, 0x01c8, 0x01d1, 0x01d9, 0x01e1, 0x01ea, 0x01fb, 0x0203,
	0x0214, 0x021d, 0x0225, 0x0236, 0x023e, 0x0247, 0x0258, 0x0269,
	0x0269, 0x0279, 0x028a, 0x0293, 0x02a4, 0x02ac, 0x02bd, 0x02c5,
	0x02ce, 0x02df, 0x02e7, 0x02f0, 0x0301, 0x0311, 0x0322, 0x0322,
	0x0333, 0x0344, 0x0355, 0x0366, 0x0377, 0x0377, 0x0390, 0x0399,
	0x03a9, 0x03b2, 0x03ba, 0x03cb, 0x03dc, 0x03ed, 0x03f5, 0x03fe,
	0x040f, 0x0420, 0x0430, 0x0441, 0x0452, 0x0452, 0x046c, 0x0474,
	0x0485, 0x0496, 0x04a7, 0x04af, 0x04c8, 0x04d1, 0x04e2, 0x04fb,
	0x0504, 0x051d, 0x052e, 0x053f, 0x0550, 0x0569, 0x057a, 0x057a,
	0x058b, 0x059c, 0x05ad, 0x05bd, 0x05c6, 0x05df, 0x05e8, 0x0601,
	0x0609, 0x0623, 0x0634, 0x0644, 0x0655, 0x066f, 0x0677, 0x0688,
	0x0699, 0x06aa, 0x06c3, 0x06cb, 0x06e5, 0x06f6, 0x0707, 0x0718,
	0x0728, 0x0742, 0x0753, 0x0763, 0x0774, 0x078e, 0x07a7, 0x07af,
	0x07c9, 0x07da, 0x07eb, 0x07fb, 0x080c, 0x0826, 0x083f, 0x0850,
	0x0872, 0x087a, 0x089c, 0x08ad, 0x08be, 0x08d7, 0x08e8, 0x0901,
	0x0912, 0x0934, 0x094d, 0x095e, 0x096f, 0x0988, 0x0991, 0x09b3,
	0x09cc, 0x09dd, 0x09f6, 0x0a07, 0x0a29, 0x0a42, 0x0a53, 0x0a75,
	0x0a86, 0x0a9f, 0x0ab0, 0x0ac9, 0x0ada, 0x0afc, 0x0b0d, 0x0b2f,
	0x0b40, 0x0b59, 0x0b72, 0x0b94, 0x0ba5, 0x0bb6, 0x0bd8, 0x0bf9,
	0x0c0a, 0x0c24, 0x0c45, 0x0c5f, 0x0c70, 0x0c91, 0x0ca2, 0x0cc4,
	0x0cdd, 0x0cf7, 0x0d18, 0x0d32, 0x0d4b, 0x0d65, 0x0d8f, 0x0da0,
	0x0db9, 0x0dd2, 0x0df4, 0x0e0d, 0x0e27, 0x0e40, 0x0e62, 0x0e84,
	0x0e94, 0x0eb6, 0x0ed0, 0x0ee9, 0x0f0b, 0x0f24, 0x0f46, 0x0f5f,
	0x0f81, 0x0f9a, 0x0fab, 0x0fbc, 0x0fcd, 0x0fd5, 0x0fe6, 0x0fff,
	0x0fff,
};
static u32 g_gamma_lut_table_1_7[DEGAMMA_COEF_LEN] = {
	0x0000, 0x0000, 0x0000, 0x0001, 0x0002, 0x0004, 0x0005,
	0x0007, 0x0009, 0x000b, 0x000e, 0x0011, 0x0014, 0x0017, 0x001a,
	0x001d, 0x0021, 0x0025, 0x0029, 0x002d, 0x0032, 0x0036, 0x003b,
	0x0040, 0x0045, 0x004a, 0x004f, 0x0054, 0x005a, 0x0060, 0x0066,
	0x006c, 0x0072, 0x0078, 0x007f, 0x0085, 0x008c, 0x0093, 0x009a,
	0x00a1, 0x00a8, 0x00b0, 0x00b7, 0x00bf, 0x00c7, 0x00cf, 0x00d7,
	0x00df, 0x00e7, 0x00ef, 0x00f8, 0x0101, 0x0109, 0x0112, 0x011b,
	0x0125, 0x012e, 0x0137, 0x0141, 0x014a, 0x0154, 0x015e, 0x0168,
	0x0172, 0x017c, 0x0187, 0x0191, 0x019b, 0x01a6, 0x01b1, 0x01bc,
	0x01c7, 0x01d2, 0x01dd, 0x01e8, 0x01f4, 0x01ff, 0x020b, 0x0217,
	0x0223, 0x022f, 0x023b, 0x0247, 0x0253, 0x0260, 0x026c, 0x0279,
	0x0285, 0x0292, 0x029f, 0x02ac, 0x02b9, 0x02c6, 0x02d4, 0x02e1,
	0x02ef, 0x02fc, 0x030a, 0x0318, 0x0326, 0x0334, 0x0342, 0x0350,
	0x035e, 0x036d, 0x037b, 0x038a, 0x0399, 0x03a8, 0x03b7, 0x03c6,
	0x03d5, 0x03e4, 0x03f3, 0x0403, 0x0412, 0x0422, 0x0431, 0x0441,
	0x0451, 0x0461, 0x0471, 0x0481, 0x0491, 0x04a2, 0x04b2, 0x04c3,
	0x04d3, 0x04e4, 0x04f5, 0x0506, 0x0517, 0x0528, 0x0539, 0x054a,
	0x055c, 0x056d, 0x057f, 0x0590, 0x05a2, 0x05b4, 0x05c6, 0x05d8,
	0x05ea, 0x05fc, 0x060e, 0x0620, 0x0633, 0x0645, 0x0658, 0x066b,
	0x067d, 0x0690, 0x06a3, 0x06b6, 0x06c9, 0x06dd, 0x06f0, 0x0703,
	0x0717, 0x072a, 0x073e, 0x0752, 0x0766, 0x077a, 0x078e, 0x07a2,
	0x07b6, 0x07ca, 0x07de, 0x07f3, 0x0807, 0x081c, 0x0831, 0x0845,
	0x085a, 0x086f, 0x0884, 0x0899, 0x08af, 0x08c4, 0x08d9, 0x08ef,
	0x0904, 0x091a, 0x092f, 0x0945, 0x095b, 0x0971, 0x0987, 0x099d,
	0x09b3, 0x09c9, 0x09e0, 0x09f6, 0x0a0d, 0x0a23, 0x0a3a, 0x0a51,
	0x0a68, 0x0a7e, 0x0a95, 0x0aad, 0x0ac4, 0x0adb, 0x0af2, 0x0b0a,
	0x0b21, 0x0b39, 0x0b50, 0x0b68, 0x0b80, 0x0b98, 0x0bb0, 0x0bc8,
	0x0be0, 0x0bf8, 0x0c10, 0x0c29, 0x0c41, 0x0c59, 0x0c72, 0x0c8b,
	0x0ca3, 0x0cbc, 0x0cd5, 0x0cee, 0x0d07, 0x0d20, 0x0d3a, 0x0d53,
	0x0d6c, 0x0d86, 0x0d9f, 0x0db9, 0x0dd2, 0x0dec, 0x0e06, 0x0e20,
	0x0e3a, 0x0e54, 0x0e6e, 0x0e88, 0x0ea2, 0x0ebd, 0x0ed7, 0x0ef2,
	0x0f0c, 0x0f27, 0x0f42, 0x0f5d, 0x0f77, 0x0f92, 0x0fad, 0x0fc9,
	0x0fe4, 0x0fff,
};

static u32 g_gamma_lut_table_1_6_t[DEGAMMA_COEF_LEN] = {
	0x0000, 0x0000, 0x0001, 0x0002, 0x0003, 0x0005, 0x0008,
	0x000a, 0x000d, 0x0010, 0x0013, 0x0017, 0x001b, 0x001f, 0x0023,
	0x0027, 0x002c, 0x0031, 0x0036, 0x003b, 0x0040, 0x0046, 0x004b,
	0x0051, 0x0057, 0x005d, 0x0064, 0x006a, 0x0071, 0x0077, 0x007e,
	0x0085, 0x008d, 0x0094, 0x009b, 0x00a3, 0x00ab, 0x00b3, 0x00bb,
	0x00c3, 0x00cb, 0x00d3, 0x00dc, 0x00e5, 0x00ed, 0x00f6, 0x00ff,
	0x0108, 0x0112, 0x011b, 0x0124, 0x012e, 0x0138, 0x0142, 0x014c,
	0x0156, 0x0160, 0x016a, 0x0175, 0x017f, 0x018a, 0x0194, 0x019f,
	0x01aa, 0x01b5, 0x01c0, 0x01cc, 0x01d7, 0x01e3, 0x01ee, 0x01fa,
	0x0206, 0x0211, 0x021d, 0x0229, 0x0236, 0x0242, 0x024e, 0x025b,
	0x0267, 0x0274, 0x0281, 0x028e, 0x029b, 0x02a8, 0x02b5, 0x02c2,
	0x02cf, 0x02dd, 0x02ea, 0x02f8, 0x0306, 0x0314, 0x0321, 0x032f,
	0x033d, 0x034c, 0x035a, 0x0368, 0x0377, 0x0385, 0x0394, 0x03a2,
	0x03b1, 0x03c0, 0x03cf, 0x03de, 0x03ed, 0x03fc, 0x040c, 0x041b,
	0x042b, 0x043a, 0x044a, 0x045a, 0x0469, 0x0479, 0x0489, 0x0499,
	0x04a9, 0x04ba, 0x04ca, 0x04da, 0x04eb, 0x04fb, 0x050c, 0x051d,
	0x052e, 0x053e, 0x054f, 0x0560, 0x0571, 0x0583, 0x0594, 0x05a5,
	0x05b7, 0x05c8, 0x05da, 0x05eb, 0x05fd, 0x060f, 0x0621, 0x0633,
	0x0645, 0x0657, 0x0669, 0x067c, 0x068e, 0x06a0, 0x06b3, 0x06c5,
	0x06d8, 0x06eb, 0x06fe, 0x0710, 0x0723, 0x0736, 0x0749, 0x075d,
	0x0770, 0x0783, 0x0797, 0x07aa, 0x07be, 0x07d1, 0x07e5, 0x07f9,
	0x080c, 0x0820, 0x0834, 0x0848, 0x085c, 0x0871, 0x0885, 0x0899,
	0x08ae, 0x08c2, 0x08d7, 0x08eb, 0x0900, 0x0915, 0x0929, 0x093e,
	0x0953, 0x0968, 0x097d, 0x0993, 0x09a8, 0x09bd, 0x09d2, 0x09e8,
	0x09fd, 0x0a13, 0x0a29, 0x0a3e, 0x0a54, 0x0a6a, 0x0a80, 0x0a96,
	0x0aac, 0x0ac2, 0x0ad8, 0x0aee, 0x0b05, 0x0b1b, 0x0b31, 0x0b48,
	0x0b5f, 0x0b75, 0x0b8c, 0x0ba3, 0x0bba, 0x0bd0, 0x0be7, 0x0bfe,
	0x0c16, 0x0c2d, 0x0c44, 0x0c5b, 0x0c73, 0x0c8a, 0x0ca1, 0x0cb9,
	0x0cd1, 0x0ce8, 0x0d00, 0x0d18, 0x0d30, 0x0d48, 0x0d60, 0x0d78,
	0x0d90, 0x0da8, 0x0dc0, 0x0dd9, 0x0df1, 0x0e09, 0x0e22, 0x0e3a,
	0x0e53, 0x0e6c, 0x0e84, 0x0e9d, 0x0eb6, 0x0ecf, 0x0ee8, 0x0f01,
	0x0f1a, 0x0f33, 0x0f4d, 0x0f66, 0x0f7f, 0x0f99, 0x0fb2, 0x0fcc,
	0x0fe5, 0x0fff,
};
static u32 g_gamma_lut_table_1_5[DEGAMMA_COEF_LEN] = {
	0x0000, 0x0000, 0x0003, 0x0008, 0x000d, 0x0012, 0x0019, 0x001f,
	0x0026, 0x002d, 0x0035, 0x003d, 0x0045, 0x004d, 0x0055, 0x005e,
	0x0067, 0x0070, 0x0079, 0x0082, 0x008c, 0x0096, 0x009f, 0x00a9,
	0x00b3, 0x00be, 0x00c8, 0x00d2, 0x00dd, 0x00e8, 0x00f3, 0x00fe,
	0x0109, 0x0114, 0x011f, 0x012a, 0x0136, 0x0141, 0x014d, 0x0159,
	0x0165, 0x0170, 0x017d, 0x0189, 0x0195, 0x01a1, 0x01ad, 0x01ba,
	0x01c6, 0x01d3, 0x01e0, 0x01ed, 0x01f9, 0x0206, 0x0213, 0x0220,
	0x022d, 0x023b, 0x0248, 0x0255, 0x0263, 0x0270, 0x027e, 0x028b,
	0x0299, 0x02a7, 0x02b5, 0x02c3, 0x02d1, 0x02df, 0x02ed, 0x02fb,
	0x0309, 0x0317, 0x0326, 0x0334, 0x0342, 0x0351, 0x035f, 0x036e,
	0x037d, 0x038b, 0x039a, 0x03a9, 0x03b8, 0x03c7, 0x03d6, 0x03e5,
	0x03f4, 0x0403, 0x0412, 0x0421, 0x0431, 0x0440, 0x0450, 0x045f,
	0x03a3, 0x03b2, 0x03c1, 0x03d0, 0x03df, 0x03ee, 0x03fd,
	0x040c, 0x041b, 0x042b, 0x043a, 0x0449, 0x0459, 0x0469, 0x0478,
	0x0488, 0x0498, 0x04a8, 0x04b8, 0x04c8, 0x04d8, 0x04e8, 0x04f9,
	0x0509, 0x0519, 0x052a, 0x053b, 0x054b, 0x055c, 0x056d, 0x057d,
	0x058e, 0x059f, 0x05b0, 0x05c1, 0x05d3, 0x05e4, 0x05f5, 0x0606,
	0x0618, 0x0629, 0x063b, 0x064d, 0x065e, 0x0670, 0x0682, 0x0694,
	0x06a6, 0x06b8, 0x06ca, 0x06dc, 0x06ee, 0x0700, 0x0713, 0x0725,
	0x0737, 0x074a, 0x075d, 0x076f, 0x0782, 0x0795, 0x07a7, 0x07ba,
	0x07cd, 0x07e0, 0x07f3, 0x0806, 0x081a, 0x082d, 0x0840, 0x0853,
	0x0867, 0x087a, 0x088e, 0x08a1, 0x08b5, 0x08c9, 0x08dc, 0x08f0,
	0x0904, 0x0918, 0x092c, 0x0940, 0x0954, 0x0968, 0x097d, 0x0991,
	0x09a5, 0x09ba, 0x09ce, 0x09e2, 0x09f7, 0x0a0c, 0x0a20, 0x0a35,
	0x0a4a, 0x0a5f, 0x0a73, 0x0a88, 0x0a9d, 0x0ab2, 0x0ac7, 0x0add,
	0x0af2, 0x0b07, 0x0b1c, 0x0b32, 0x0b47, 0x0b5d, 0x0b72, 0x0b88,
	0x0b9d, 0x0bb3, 0x0bc9, 0x0bdf, 0x0bf4, 0x0c0a, 0x0c20, 0x0c36,
	0x0c4c, 0x0c62, 0x0c78, 0x0c8f, 0x0ca5, 0x0cbb, 0x0cd2, 0x0ce8,
	0x0cfe, 0x0d15, 0x0d2b, 0x0d42, 0x0d59, 0x0d6f, 0x0d86, 0x0d9d,
	0x0db4, 0x0dcb, 0x0de2, 0x0df9, 0x0e10, 0x0e27, 0x0e3e, 0x0e55,
	0x0e6c, 0x0e84, 0x0e9b, 0x0eb2, 0x0eca, 0x0ee1, 0x0ef9, 0x0f10,
	0x0f28, 0x0f40, 0x0f58, 0x0f6f, 0x0f87, 0x0f9f, 0x0fb7, 0x0fcf,
	0x0fe7, 0x0fff,
};

static u32 g_gamma_lut_table_1_5_t[DEGAMMA_COEF_LEN] = {
	0x0000, 0x0000, 0x0001, 0x0003, 0x0005, 0x0008, 0x000b,
	0x000f, 0x0013, 0x0017, 0x001b, 0x0020, 0x0025, 0x002a, 0x002f,
	0x0035, 0x003a, 0x0040, 0x0046, 0x004d, 0x0053, 0x005a, 0x0061,
	0x0068, 0x006f, 0x0076, 0x007e, 0x0085, 0x008d, 0x0095, 0x009d,
	0x00a5, 0x00ae, 0x00b6, 0x00bf, 0x00c7, 0x00d0, 0x00d9, 0x00e2,
	0x00ec, 0x00f5, 0x00fe, 0x0108, 0x0112, 0x011c, 0x0126, 0x0130,
	0x013a, 0x0144, 0x014e, 0x0159, 0x0164, 0x016e, 0x0179, 0x0184,
	0x018f, 0x019a, 0x01a5, 0x01b1, 0x01bc, 0x01c8, 0x01d3, 0x01df,
	0x01eb, 0x01f7, 0x0203, 0x020f, 0x021b, 0x0228, 0x0234, 0x0240,
	0x024d, 0x025a, 0x0266, 0x0273, 0x0280, 0x028d, 0x029a, 0x02a7,
	0x02b5, 0x02c2, 0x02d0, 0x02dd, 0x02eb, 0x02f8, 0x0306, 0x0314,
	0x0322, 0x0330, 0x033e, 0x034c, 0x035b, 0x0369, 0x0377, 0x0386,
	0x0395, 0x03a3, 0x03b2, 0x03c1, 0x03d0, 0x03df, 0x03ee, 0x03fd,
	0x040c, 0x041b, 0x042b, 0x043a, 0x0449, 0x0459, 0x0469, 0x0478,
	0x0488, 0x0498, 0x04a8, 0x04b8, 0x04c8, 0x04d8, 0x04e8, 0x04f9,
	0x0509, 0x0519, 0x052a, 0x053b, 0x054b, 0x055c, 0x056d, 0x057d,
	0x058e, 0x059f, 0x05b0, 0x05c1, 0x05d3, 0x05e4, 0x05f5, 0x0606,
	0x0618, 0x0629, 0x063b, 0x064d, 0x065e, 0x0670, 0x0682, 0x0694,
	0x06a6, 0x06b8, 0x06ca, 0x06dc, 0x06ee, 0x0700, 0x0713, 0x0725,
	0x0737, 0x074a, 0x075d, 0x076f, 0x0782, 0x0795, 0x07a7, 0x07ba,
	0x07cd, 0x07e0, 0x07f3, 0x0806, 0x081a, 0x082d, 0x0840, 0x0853,
	0x0867, 0x087a, 0x088e, 0x08a1, 0x08b5, 0x08c9, 0x08dc, 0x08f0,
	0x0904, 0x0918, 0x092c, 0x0940, 0x0954, 0x0968, 0x097d, 0x0991,
	0x09a5, 0x09ba, 0x09ce, 0x09e2, 0x09f7, 0x0a0c, 0x0a20, 0x0a35,
	0x0a4a, 0x0a5f, 0x0a73, 0x0a88, 0x0a9d, 0x0ab2, 0x0ac7, 0x0add,
	0x0af2, 0x0b07, 0x0b1c, 0x0b32, 0x0b47, 0x0b5d, 0x0b72, 0x0b88,
	0x0b9d, 0x0bb3, 0x0bc9, 0x0bdf, 0x0bf4, 0x0c0a, 0x0c20, 0x0c36,
	0x0c4c, 0x0c62, 0x0c78, 0x0c8f, 0x0ca5, 0x0cbb, 0x0cd2, 0x0ce8,
	0x0cfe, 0x0d15, 0x0d2b, 0x0d42, 0x0d59, 0x0d6f, 0x0d86, 0x0d9d,
	0x0db4, 0x0dcb, 0x0de2, 0x0df9, 0x0e10, 0x0e27, 0x0e3e, 0x0e55,
	0x0e6c, 0x0e84, 0x0e9b, 0x0eb2, 0x0eca, 0x0ee1, 0x0ef9, 0x0f10,
	0x0f28, 0x0f40, 0x0f58, 0x0f6f, 0x0f87, 0x0f9f, 0x0fb7, 0x0fcf,
	0x0fe7, 0x0fff,
};
static u32 g_gamma_lut_table_2_2[DEGAMMA_COEF_LEN] = {
	0x0000, 0x0001, 0x0002, 0x0004, 0x0005, 0x0006, 0x0007, 0x0009,
	0x000a, 0x000b, 0x000c, 0x000e, 0x000f, 0x0010, 0x0012, 0x0013,
	0x0015, 0x0017, 0x0019, 0x001b, 0x001d, 0x001f, 0x0021, 0x0023,
	0x0025, 0x0028, 0x002a, 0x002d, 0x002f, 0x0032, 0x0035, 0x0038,
	0x003b, 0x003e, 0x0041, 0x0045, 0x0048, 0x004b, 0x004f, 0x0053,
	0x0057, 0x005a, 0x005e, 0x0063, 0x0067, 0x006b, 0x006f, 0x0074,
	0x0079, 0x007d, 0x0082, 0x0087, 0x008c, 0x0091, 0x0097, 0x009c,
	0x00a1, 0x00a7, 0x00ad, 0x00b2, 0x00b8, 0x00be, 0x00c5, 0x00cb,
	0x00d1, 0x00d8, 0x00de, 0x00e5, 0x00ec, 0x00f3, 0x00fa, 0x0101,
	0x0108, 0x0110, 0x0117, 0x011f, 0x0127, 0x012f, 0x0137, 0x013f,
	0x0147, 0x0150, 0x0158, 0x0161, 0x016a, 0x0173, 0x017c, 0x0185,
	0x018e, 0x0198, 0x01a1, 0x01ab, 0x01b5, 0x01bf, 0x01c9, 0x01d3,
	0x01dd, 0x01e8, 0x01f2, 0x01fd, 0x0208, 0x0213, 0x021e, 0x0229,
	0x0235, 0x0240, 0x024c, 0x0258, 0x0264, 0x0270, 0x027c, 0x0289,
	0x0295, 0x02a2, 0x02af, 0x02bb, 0x02c9, 0x02d6, 0x02e3, 0x02f1,
	0x02fe, 0x030c, 0x031a, 0x0328, 0x0336, 0x0345, 0x0353, 0x0362,
	0x0371, 0x0380, 0x038f, 0x039e, 0x03ad, 0x03bd, 0x03cd, 0x03dd,
	0x03ed, 0x03fd, 0x040d, 0x041d, 0x042e, 0x043f, 0x0450, 0x0461,
	0x0472, 0x0483, 0x0495, 0x04a6, 0x04b8, 0x04ca, 0x04dc, 0x04ef,
	0x0501, 0x0514, 0x0526, 0x0539, 0x054c, 0x0560, 0x0573, 0x0587,
	0x059a, 0x05ae, 0x05c2, 0x05d6, 0x05eb, 0x05ff, 0x0614, 0x0629,
	0x063e, 0x0653, 0x0668, 0x067e, 0x0693, 0x06a9, 0x06bf, 0x06d5,
	0x06eb, 0x0702, 0x0718, 0x072f, 0x0746, 0x075d, 0x0775, 0x078c,
	0x07a4, 0x07bb, 0x07d3, 0x07eb, 0x0804, 0x081c, 0x0835, 0x084e,
	0x0867, 0x0880, 0x0899, 0x08b3, 0x08cc, 0x08e6, 0x0900, 0x091a,
	0x0935, 0x094f, 0x096a, 0x0985, 0x09a0, 0x09bb, 0x09d6, 0x09f2,
	0x0a0d, 0x0a29, 0x0a45, 0x0a62, 0x0a7e, 0x0a9b, 0x0ab8, 0x0ad5,
	0x0af2, 0x0b0f, 0x0b2c, 0x0b4a, 0x0b68, 0x0b86, 0x0ba4, 0x0bc3,
	0x0be1, 0x0c00, 0x0c1f, 0x0c3e, 0x0c5d, 0x0c7d, 0x0c9c, 0x0cbc,
	0x0cdc, 0x0cfd, 0x0d1d, 0x0d3e, 0x0d5e, 0x0d7f, 0x0da0, 0x0dc2,
	0x0de3, 0x0e05, 0x0e27, 0x0e49, 0x0e6b, 0x0e8d, 0x0eb0, 0x0ed3,
	0x0ef6, 0x0f19, 0x0f3c, 0x0f60, 0x0f84, 0x0fa8, 0x0fcc, 0x0ff0,
	0x0fff,
};
static u32 g_gamma_lut_table_2_3[DEGAMMA_COEF_LEN] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001,
	0x0001, 0x0001, 0x0001, 0x0001, 0x0002, 0x0002, 0x0002, 0x0003,
	0x0003, 0x0004, 0x0004, 0x0005, 0x0005, 0x0006, 0x0006, 0x0007,
	0x0008, 0x0008, 0x0009, 0x000a, 0x000b, 0x000b, 0x000c, 0x000d,
	0x000e, 0x0018, 0x0019, 0x001a, 0x0023, 0x0024, 0x0025, 0x0026,
	0x0030, 0x0031, 0x003a, 0x003b, 0x003d, 0x003e, 0x0048, 0x0051,
	0x0053, 0x0055, 0x005e, 0x0060, 0x006a, 0x006c, 0x0075, 0x0077,
	0x0079, 0x007b, 0x0085, 0x008f, 0x0091, 0x0093, 0x00a5, 0x00a7,
	0x00a9, 0x00ac, 0x00be, 0x00c1, 0x00c3, 0x00c5, 0x00d8, 0x00da,
	0x00dd, 0x00e7, 0x00f2, 0x00f5, 0x00ff, 0x010b, 0x010e, 0x0118,
	0x011b, 0x0126, 0x0129, 0x0134, 0x0140, 0x014a, 0x014d, 0x0159,
	0x0164, 0x0167, 0x0172, 0x017e, 0x0189, 0x0195, 0x0199, 0x01a4,
	0x01b0, 0x01bb, 0x01bf, 0x01d3, 0x01d7, 0x01eb, 0x01ef, 0x0203,
	0x0207, 0x0212, 0x021f, 0x022b, 0x022f, 0x0243, 0x0247, 0x025c,
	0x0260, 0x0275, 0x0281, 0x028e, 0x029a, 0x029f, 0x02b3, 0x02b8,
	0x02cd, 0x02d2, 0x02e7, 0x02ec, 0x02f8, 0x0306, 0x0312, 0x0327,
	0x0335, 0x0342, 0x0357, 0x035c, 0x0372, 0x0377, 0x038d, 0x039b,
	0x03a8, 0x03bd, 0x03cb, 0x03d9, 0x03e6, 0x03f4, 0x040a, 0x0418,
	0x042e, 0x0434, 0x044a, 0x0460, 0x046e, 0x047c, 0x0493, 0x04a1,
	0x04b7, 0x04c6, 0x04d4, 0x04ea, 0x0501, 0x0508, 0x051e, 0x0535,
	0x054c, 0x0553, 0x056a, 0x0581, 0x058f, 0x05a6, 0x05b6, 0x05cc,
	0x05dc, 0x05f3, 0x060a, 0x0622, 0x0639, 0x0640, 0x0658, 0x066f,
	0x0687, 0x0696, 0x06ae, 0x06c6, 0x06de, 0x06f6, 0x06fd, 0x0715,
	0x072d, 0x0746, 0x075e, 0x076d, 0x0786, 0x07a6, 0x07b6, 0x07cf,
	0x07e0, 0x0800, 0x0818, 0x0828, 0x0841, 0x085b, 0x0873, 0x088b,
	0x08ad, 0x08c6, 0x08d6, 0x08ef, 0x0908, 0x0921, 0x0942, 0x0964,
	0x097d, 0x0997, 0x09b8, 0x09d1, 0x09fb, 0x0a14, 0x0a2e, 0x0a4f,
	0x0a69, 0x0a8b, 0x0aad, 0x0ac7, 0x0af1, 0x0b0b, 0x0b35, 0x0b57,
	0x0b71, 0x0b93, 0x0bb5, 0x0bd7, 0x0bfa, 0x0c1c, 0x0c3f, 0x0c61,
	0x0c8c, 0x0cae, 0x0cc9, 0x0cf4, 0x0d17, 0x0d3a, 0x0d5c, 0x0d87,
	0x0dab, 0x0dce, 0x0df9, 0x0e24, 0x0e47, 0x0e6b, 0x0e8e, 0x0ec2,
	0x0ee5, 0x0f09, 0x0f2c, 0x0f58, 0x0f8b, 0x0faf, 0x0fd3, 0x0fff,
	0x0fff,
};
static u32 g_gamma_lut_table_2_0[DEGAMMA_COEF_LEN] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0001, 0x0002, 0x0002,
	0x0003, 0x0004, 0x0005, 0x0006, 0x0008, 0x0009, 0x000b, 0x000c,
	0x000e, 0x0010, 0x0012, 0x0014, 0x0017, 0x0019, 0x001c, 0x001e,
	0x0021, 0x0024, 0x0027, 0x002b, 0x002e, 0x0031, 0x0035, 0x0039,
	0x003d, 0x0040, 0x0045, 0x0049, 0x004d, 0x0052, 0x0056, 0x005b,
	0x0060, 0x0065, 0x006a, 0x006f, 0x0074, 0x007a, 0x0080, 0x0085,
	0x008b, 0x0091, 0x0097, 0x009d, 0x00a4, 0x00aa, 0x00b1, 0x00b8,
	0x00bf, 0x00c5, 0x00cd, 0x00d4, 0x00db, 0x00e3, 0x00ea, 0x00f2,
	0x00fa, 0x0102, 0x010a, 0x0112, 0x011b, 0x0123, 0x012c, 0x0135,
	0x013d, 0x0146, 0x0150, 0x0159, 0x0162, 0x016c, 0x0175, 0x017f,
	0x0189, 0x0193, 0x019d, 0x01a7, 0x01b2, 0x01bc, 0x01c7, 0x01d2,
	0x01dd, 0x01e8, 0x01f3, 0x01fe, 0x020a, 0x0215, 0x0221, 0x022c,
	0x0238, 0x0244, 0x0251, 0x025d, 0x0269, 0x0276, 0x0282, 0x028f,
	0x029c, 0x02a9, 0x02b6, 0x02c4, 0x02d1, 0x02df, 0x02ec, 0x02fa,
	0x0308, 0x0316, 0x0324, 0x0332, 0x0341, 0x034f, 0x035e, 0x036d,
	0x037c, 0x038b, 0x039a, 0x03a9, 0x03b9, 0x03c8, 0x03d8, 0x03e8,
	0x03f8, 0x0408, 0x0418, 0x0428, 0x0439, 0x0449, 0x045a, 0x046b,
	0x047c, 0x048d, 0x049e, 0x04af, 0x04c1, 0x04d2, 0x04e4, 0x04f6,
	0x0508, 0x051a, 0x052c, 0x053e, 0x0551, 0x0563, 0x0576, 0x0589,
	0x059c, 0x05af, 0x05c2, 0x05d6, 0x05e9, 0x05fd, 0x0610, 0x0624,
	0x0638, 0x064c, 0x0660, 0x0675, 0x0689, 0x069e, 0x06b3, 0x06c7,
	0x06dc, 0x06f1, 0x0707, 0x071c, 0x0731, 0x0747, 0x075d, 0x0773,
	0x0789, 0x079f, 0x07b5, 0x07cb, 0x07e2, 0x07f8, 0x080f, 0x0826,
	0x083d, 0x0854, 0x086b, 0x0883, 0x089a, 0x08b2, 0x08ca, 0x08e1,
	0x08f9, 0x0912, 0x092a, 0x0942, 0x095b, 0x0973, 0x098c, 0x09a5,
	0x09be, 0x09d7, 0x09f0, 0x0a0a, 0x0a23, 0x0a3d, 0x0a57, 0x0a70,
	0x0a8a, 0x0aa5, 0x0abf, 0x0ad9, 0x0af4, 0x0b0e, 0x0b29, 0x0b44,
	0x0b5f, 0x0b7a, 0x0b95, 0x0bb1, 0x0bcc, 0x0be8, 0x0c04, 0x0c20,
	0x0c3c, 0x0c58, 0x0c74, 0x0c91, 0x0cad, 0x0cca, 0x0ce7, 0x0d03,
	0x0d20, 0x0d3e, 0x0d5b, 0x0d78, 0x0d96, 0x0db3, 0x0dd1, 0x0def,
	0x0e0d, 0x0e2b, 0x0e4a, 0x0e68, 0x0e87, 0x0ea5, 0x0ec4, 0x0ee3,
	0x0f02, 0x0f21, 0x0f41, 0x0f60, 0x0f80, 0x0f9f, 0x0fbf, 0x0fdf,
	0x0fff,
};

static void gamma_choice(u32 *r_gamma, u32 *g_gamma, u32 *b_gamma)
{
	int i;

	for(i = 0; i < DEGAMMA_COEF_LEN; i++) {
		hisi_dss_alsc_init.degamma_lut_r[i] = r_gamma[i];
		hisi_dss_alsc_init.degamma_lut_g[i] = g_gamma[i];
		hisi_dss_alsc_init.degamma_lut_b[i] = b_gamma[i];
	}
}

void als_gamma_param_config(void)
{
	uint32_t nv_data;
	struct als_device_info *dev_info = NULL;
	u32 *r_gamma = NULL;
	u32 *g_gamma = NULL;
	u32 *b_gamma = NULL;

	dev_info = als_get_device_info(TAG_ALS);
	if (dev_info == NULL)
		return;

	/* 29 save nv */
	nv_data = (uint32_t)(dev_info->als_under_tp_cal_data.b[29]);

	hwlog_info("%s nv_data=%u g_gamma_2_3=%u,g_gamma_1_6=%u,g_gamma_1_5=%u",
		__func__,nv_data, g_gamma_lut_table_2_3[0],
		g_gamma_lut_table_1_6[0],g_gamma_lut_table_1_5[0]);
	/* hight 8bit */
	nv_data = (nv_data >> 8);
	hwlog_info("%s gamma type: %d", __func__, nv_data);

	if (nv_data == GAMMA_TYPE_O_BOE) {
		r_gamma = g_gamma_lut_table_1_6_t;
		g_gamma = g_gamma_lut_table_1_5_t;
		b_gamma = g_gamma_lut_table_2_2;
	} else if ((nv_data == GAMMA_TYPE_N_BOE) ||
		(nv_data == GAMMA_TYPE_NP_BOE)) {
		if(dev_info->chip_type == ALS_CHIP_AMS_TSL2540_RGB) {
			hwlog_info("%s chip info G-IR", __func__);
			r_gamma = g_gamma_lut_table_2_0;
			g_gamma = g_gamma_lut_table_1_6_t;
			b_gamma = g_gamma_lut_table_2_2;

		} else {
			r_gamma = g_gamma_lut_table_1_6_t;
			g_gamma = g_gamma_lut_table_1_5_t;
			b_gamma = g_gamma_lut_table_2_2;
		}
	} else if ((nv_data == GAMMA_TYPE_N_SM) ||
		(nv_data == GAMMA_TYPE_NP_SM)) {
		r_gamma = g_gamma_lut_table_2_0;
		g_gamma = g_gamma_lut_table_2_0;
		b_gamma = g_gamma_lut_table_2_2;
	} else if ((nv_data == GAMMA_TYPE_N_LG) ||
		(nv_data == GAMMA_TYPE_NP_LG)) {
		r_gamma = g_gamma_lut_table_1_5;
		g_gamma = g_gamma_lut_table_1_7;
		b_gamma = g_gamma_lut_table_2_2;
	} else {
		hwlog_info("%s gamma type default", __func__);
		r_gamma = g_gamma_lut_table_1_7;
		g_gamma = g_gamma_lut_table_1_7;
		b_gamma = g_gamma_lut_table_2_2;
	}
	gamma_choice(r_gamma, g_gamma, b_gamma);
}
#endif
