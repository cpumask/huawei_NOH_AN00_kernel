/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Hisi fbex common headers
 * Author : security-ap
 * Create : 2020/01/02
 */

#ifndef __HISI_FBE_CTRL_H_
#define __HISI_FBE_CTRL_H_

#include <linux/types.h>

#ifndef _unused
#define _unused __attribute__((unused))
#endif

enum FILE_ENCRY_CMD {
	SEC_FILE_ENCRY_CMD_ID_VOLD_ADD_IV = 0x1,
	SEC_FILE_ENCRY_CMD_ID_VOLD_DELETE_IV = 0x2,
	SEC_FILE_ENCRY_CMD_ID_LOCK_SCREEN = 0x3,
	SEC_FILE_ENCRY_CMD_ID_UNLOCK_SCREEN = 0x4,
	SEC_FILE_ENCRY_CMD_ID_KEY_RESTORE = 0x5,
	SEC_FILE_ENCRY_CMD_ID_NEW_SECE = 0x6,
	SEC_FILE_ENCRY_CMD_ID_GEN_METADATA = 0x7,
	SEC_FILE_ENCRY_CMD_ID_USER_LOGOUT = 0x8,
	SEC_FILE_ENCRY_CMD_ID_ENABLE_KDF = 0x9,
	SEC_FILE_ENCRY_CMD_ID_PRELOADING = 0xA,
	SEC_FILE_ENCRY_CMD_ID_MSPC_CHECK = 0xB,
	SEC_FILE_ENCRY_CMD_ID_STATUS_REPORT = 0xC,
	/* Reserve cmd id 0xC - 0xF for future using */
	SEC_FILE_ENCRY_CMD_ID_MAX_SUPPORT = 0x10,
};
#define SEC_FILE_ENCRY_CMD_ID_MASK 0xF

#define KEY_LEN      64 /* 64 * 8 = 512bits */
#define METADATA_LEN 16 /* 16 * 8 = 128bits */
#ifdef FILE_ENCRY_P384_USING
#define PUBKEY_LEN   0x61 /* pubkey len, 0x61 for p384 */
#else
#define PUBKEY_LEN   0x41 /* pubkey len, 0x41 for p256 */
#endif

#ifdef CONFIG_HISI_FILE_BASED_ENCRYPTO
bool hisi_fbex_slot_clean(u32 slot);
void hisi_fbe3_flushcache_done(void);
u32 hisi_fbex_enable_kdf(void);
u32 hisi_fbex_restore_key(void);
u32 hisi_get_metadata(u8 *buf, u32 len);
u32 hisi_get_metadata_sece(u32 cmd, u32 index, u8 *pubkey, u32 key_len,
			   u8 *metadata, u32 iv_len);
#else
static bool hisi_fbex_slot_clean(u32 slot) {return false; }
static inline void hisi_fbe3_flushcache_done(void) {}
static inline u32 hisi_fbex_enable_kdf(void) {return 0; }
static inline u32 hisi_fbex_restore_key(void) {return 0; }
static inline u32 hisi_get_metadata(u8 *buf _unused, u32 len _unused) {return 0; }
static inline u32 hisi_get_metadata_sece(u32 cmd _unused, u32 index _unused,
					 u8 *pubkey _unused, u32 key_len _unused,
					 u8 *metadata _unused, u32 iv_len _unused)
{
	return 0;
}
#endif

#endif /* __HISI_FBE_CTRL_H_ */
