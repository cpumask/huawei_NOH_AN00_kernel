

#ifndef __RPMB_H__
#define __RPMB_H__

#include <linux/mmc/ioctl.h>
#include <linux/fs.h>
#include <uapi/linux/bsg.h>

#define MAX_CDB_CMD_LENGTH 16
#define UFS_IOC_MAX_RPMB_CMD 3
#define STORAGE_IOC_MAX_RPMB_CMD 3
#define MAX_IOC_RPMB_BYTES (32 * 512)  /* max 32 blks, 1 blk = 512bytes */

enum rpmb_op_type { RPMB_OP_RD = 0, RPMB_OP_WR_DATA = 1, RPMB_OP_WR_CNT = 2 };
enum func_id {
	RPMB_FUNC_ID_RESERVED,
	RPMB_FUNC_ID_SE,
	RPMB_FUNC_ID_SECURE_OS,
	RPMB_FUNC_ID_MAX,
};

enum rpmb_version {
	RPMB_VER_INVALID = 0,
	RPMB_VER_MMC_51,
	RPMB_VER_UFS_20,
	RPMB_VER_UFS_21,
	RPMB_VER_UFS_30,
	RPMB_VER_MAX,
};

struct storage_blk_ioc_data {
	unsigned char *buf;
	u64 buf_bytes;
	u32 blocks;
};

struct mmc_blk_ioc_data {
	struct mmc_ioc_cmd ic;
	unsigned char *buf;
	u64 buf_bytes;
};

struct ufs_blk_ioc_data {
	/* struct sg_io_hdr sih; */
	struct sg_io_v4 siv;
	unsigned char *buf;
	u64 buf_bytes;
};

struct storage_blk_ioc_rpmb_data {
	struct storage_blk_ioc_data data[STORAGE_IOC_MAX_RPMB_CMD];
};

struct mmc_blk_ioc_rpmb_data {
	struct mmc_blk_ioc_data data[MMC_IOC_MAX_RPMB_CMD];
};

struct ufs_blk_ioc_rpmb_data {
	struct ufs_blk_ioc_data data[UFS_IOC_MAX_RPMB_CMD];
	u8 sdb_command[UFS_IOC_MAX_RPMB_CMD][MAX_CDB_CMD_LENGTH];
};

extern u64 g_rpmb_ufs_start_time;
extern struct mutex rpmb_counter_lock;
extern void hisi_rpmb_active(void);
extern int rpmb_get_dev_ver(enum rpmb_version *ver);
extern int get_rpmb_key_status(void);
extern u32 get_rpmb_support_key_num(void);
extern int wait_hisee_rpmb_request_is_finished(void);
extern int check_hisee_rpmb_request_is_cleaned(void);
extern int32_t hisee_exception_to_reset_rpmb(void);

#ifdef CONFIG_HISI_RPMB_UFS
int rpmb_ufs_init(void);
#endif
#ifdef CONFIG_HISI_RPMB_MMC
int rpmb_mmc_init(void);
int mmc_blk_ioctl_rpmb_cmd(enum func_id id, struct block_device *bdev,
			   struct mmc_blk_ioc_rpmb_data *idata);
#endif

#endif /* __RPMB_H__ */
