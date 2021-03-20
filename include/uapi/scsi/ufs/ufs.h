#ifndef UAPI_UFS_H_
#define UAPI_UFS_H_

#define TURBO_ZONE_INFO_SIZE (sizeof(struct tz_status))

struct tz_status {
	uint8_t enable;		/* 0: disable; 1: enable; */
	uint8_t lu_number;      /* LU number */
	uint16_t max_ec;	/* max EC of Turbo Zone */
	uint16_t avg_ec;	/* average EC of turbo zone */
	uint8_t return_en;      /* return to tlc enable */
	uint8_t return_back_en; /* return back to slc enable */
	uint32_t max_lba;       /* max LBA(4KB unit) */
	uint32_t total_vpc;     /* Total VPC, Valid when TurboZone Enable */
	uint8_t close_qcnt;     /* blocks to return */
	uint8_t forbidden;      /* already used once, cannot enable again, reserved for 2.0 */
	uint16_t tz_close_cnt;  /* for 2.0 */
};

struct tz_cap_info {
	uint32_t marked_slc_blks;  /* turbo zone marked slc blks, unit: 4K */
	uint32_t marked_tlc_blks;  /* turbo zone marked tlc blks, unit: 4K */
	uint32_t remain_slc_blks;  /* remained slc blks, unit: 4K */
	uint32_t slc_exist_status; /* slc exist flag */
};

struct tz_blk_info {
	uint32_t addr;  /* query blk info lba */
	uint16_t len;   /* query blk info length */
	uint8_t *flags; /* the return info of query lba */
	uint16_t buf_len; /* the flags buffer length */
};

enum {
	TZ_INVALID = 0,
	TZ_VER_1_0 = 1,
	TZ_VER_2_0 = 2,
};

enum {
	TZ_READ_DESC	= 1,
	TZ_STATUS	= 2,
	TZ_CAP_INFO	= 3,
	TZ_BLK_INFO	= 4,
};

enum tz_desc_id {
	TZ_RETURN_FLAG      = 0x01,
#ifdef CONFIG_HISI_DEBUG_FS
	TZ_RETURN_BACK_FLAG = 0x04,
#endif
	TZ_FORCE_CLOSE_FLAG = 0x08,
#ifdef CONFIG_HISI_DEBUG_FS
	TZ_FORCE_OPEN_FLAG  = 0x10,
#endif
	TZ_DESC_MAX,
};

enum vendor_ctrl_desc_id {
	DEVICE_CAPABILITY_FLAG = 0x06,
	SET_TZ_STREAM_ID       = 0x0C,
	VENDOR_CTRL_MAX,
};

/* UTP QUERY Transaction Specific Fields OpCode */
enum query_opcode {
	UPIU_QUERY_OPCODE_NOP		= 0x0,
	UPIU_QUERY_OPCODE_READ_DESC	= 0x1,
	UPIU_QUERY_OPCODE_WRITE_DESC	= 0x2,
	UPIU_QUERY_OPCODE_READ_ATTR	= 0x3,
	UPIU_QUERY_OPCODE_WRITE_ATTR	= 0x4,
	UPIU_QUERY_OPCODE_READ_FLAG	= 0x5,
	UPIU_QUERY_OPCODE_SET_FLAG	= 0x6,
	UPIU_QUERY_OPCODE_CLEAR_FLAG	= 0x7,
	UPIU_QUERY_OPCODE_TOGGLE_FLAG	= 0x8,
#ifdef CONFIG_HISI_UFS_MANUAL_BKOPS
	/* Hi1861 specific manual GC OPCODE */
	UPIU_QUERY_OPCODE_M_GC_START = 0xF0,
	UPIU_QUERY_OPCODE_M_GC_STOP = 0xF1,
	UPIU_QUERY_OPCODE_M_GC_CHECK = 0xF2,
#endif /* CONFIG_HISI_UFS_MANUAL_BKOPS */
	UPIU_QUERY_OPCODE_READ_HI1861_FSR = 0xF3, /*for hi1861*/
	UPIU_QUERY_OPCODE_VENDOR_READ = 0xF8,
	UPIU_QUERY_OPCODE_VENDOR_WRITE = 0xF9,
	UPIU_QUERY_OPCODE_TZ_CTRL = 0xFA,
	UPIU_QUERY_OPCODE_READ_TZ_DESC = 0xFB,
	UPIU_QUERY_OPCODE_MAX,
};

#ifndef CONFIG_HISI_DEBUG_FS
void delete_ufs_product_name(char *cmdline);
#endif

#endif /* UAPI_UFS_H_ */
