

#ifndef __MSPC_FS_H__
#define __MSPC_FS_H__

#include <mspc_upgrade.h>
#include <linux/types.h>

#define MSPC_MAX_EMMC_COS_NUMBER     5
#define MSPC_MIN_EMMC_COS_NUMBER     0

#define MSPC_ENCOS_MAGIC_LEN         8
#define MSPC_ENCOS_START_ID          3
#define MSPC_FLASH_COS_STRING        "COS5"
#define MSPC_ENCOS_MAGIC_STRING      "encd_cos"

#define MSPC_PTN_PATH_BUF_SIZE       128
#define MSPC_ENCOS_PARTITION_NAME    "hisee_encos"
#define MSPC_FS_PARTITION_NAME       "hisee_fs"
#define MSPC_IMAGE_PARTITION_NAME    "hisee_img"

#define MSPC_ENCOS_FILE_MAX          MSPC_MAX_EMMC_COS_NUMBER

#define MSPC_ENCOS_FILE_NAME_LEN       8
#define MSPC_ENCOS_FILE_LEN            MSPC_MAX_IMG_SIZE
#define MSPC_ENCOS_TOTAL_FILE_SIZE    (MSPC_ENCOS_FILE_MAX * \
	MSPC_ENCOS_FILE_LEN)

/* Default mode when creating a file or dir if user doesn't set mode */
#define MSPC_FILESYS_DEFAULT_MODE     0660

enum {
	MSPC_PARTITON_IMG   = 0x5A,
	MSPC_PARTITON_ENCOS = 0xA5,
};

enum {
	FLASH_COS_EMPTY = 0,  /* encos partition is empty */
	FLASH_COS_EXIST,  /* exist cosflash, can run */
	FLASH_COS_ERASE,  /* cosflash has been erased by kernel */
};

enum mspc_image_access_type {
	MSPC_FLAG_INFO_READ   = 0,
	MSPC_FLAG_INFO_WRITE,
	MSPC_UPGRADE_FLAG_READ,
	MSPC_UPGRADE_FLAG_WRITE,
	MSPC_IMAGE_HEADER_READ,
	MSPC_IMAGE_HEADER_WRITE,
	MSPC_ENCOS_HEADER_READ,
	MSPC_ENCOS_HEADER_WRITE,
	MSPC_ENCOS_IMAGE_READ,
	MSPC_IMAGE_READ,
	MSPC_IMAGE_WRITE,
};

struct encos_file_info {
	char name[MSPC_ENCOS_FILE_NAME_LEN];
	uint32_t offset;
	uint32_t size;
};

struct mspc_encos_header {
	char magic[MSPC_ENCOS_MAGIC_LEN];
	uint32_t total_size;
	uint32_t file_cnt;
	struct encos_file_info file[MSPC_ENCOS_FILE_MAX];
};

struct mspc_access_info {
	int8_t fullpath[MSPC_PTN_PATH_BUF_SIZE];
	uint32_t partition_type;
	uint32_t access_type;
	int32_t flags;
	uint64_t size;
	int64_t offset;
};

int32_t mspc_get_encos_img_header(struct mspc_encos_header *header);
int32_t mspc_get_and_parse_img_header(struct mspc_img_header *header);
void mspc_parse_timestamp(const int8_t *time, uint32_t len,
			  union mspc_timestamp_info *timestamp);
int32_t mspc_access_partition(int8_t *buf, uint32_t size,
			      struct mspc_access_info *access_info);
int32_t mspc_check_flash_cos_file(uint32_t *state);
int32_t mspc_remove_flash_cos(void);
int32_t mspc_check_hisee_fs_empty(void);
#ifdef CONFIG_HISI_DEBUG_FS
int mspc_read_input_test_file(const char *fullname, char *buffer, size_t offset,
			      size_t size);
#endif
#endif /* __MSPC_FS_H__ */
