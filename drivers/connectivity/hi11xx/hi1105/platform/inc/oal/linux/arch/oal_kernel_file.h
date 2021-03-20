

#ifndef __OAL_LINUX_KERNEL_FILE_H__
#define __OAL_LINUX_KERNEL_FILE_H__

/* ����ͷ�ļ����� */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>

/* �궨�� */
#define OAL_KERNEL_DS KERNEL_DS

/* �ļ����� */
#define OAL_O_ACCMODE O_ACCMODE
#define OAL_O_RDONLY  O_RDONLY
#define OAL_O_WRONLY  O_WRONLY
#define OAL_O_RDWR    O_RDWR
#define OAL_O_CREAT   O_CREAT
#define OAL_O_TRUNC   O_TRUNC
#define OAL_O_APPEND  O_APPEND

#define OAL_PRINT_FORMAT_LENGTH 200 /* ��ӡ��ʽ�ַ�������󳤶� */

typedef struct file oal_file;
typedef mm_segment_t oal_mm_segment_t;

/* �������� */
OAL_STATIC OAL_INLINE oal_mm_segment_t oal_get_fs(void)
{
    return get_fs();
}

OAL_STATIC OAL_INLINE void oal_set_fs(oal_mm_segment_t fs)
{
    return set_fs(fs);
}

OAL_STATIC OAL_INLINE int oal_kernel_file_close(oal_file *pst_file)
{
    return filp_close(pst_file, NULL);
}

OAL_STATIC OAL_INLINE int oal_sysfs_create_group(struct kobject *kobj,
                                                 const struct attribute_group *grp)
{
    return sysfs_create_group(kobj, grp);
}

OAL_STATIC OAL_INLINE void oal_sysfs_remove_group(struct kobject *kobj,
                                                  const struct attribute_group *grp)
{
    sysfs_remove_group(kobj, grp);
}

OAL_STATIC OAL_INLINE int oal_debug_sysfs_create_group(struct kobject *kobj,
                                                       const struct attribute_group *grp)
{
#ifdef PLATFORM_DEBUG_ENABLE
    return sysfs_create_group(kobj, grp);
#else
    oal_reference(kobj);
    oal_reference(grp);
    return 0;
#endif
}

OAL_STATIC OAL_INLINE void oal_debug_sysfs_remove_group(struct kobject *kobj,
                                                        const struct attribute_group *grp)
{
#ifdef PLATFORM_DEBUG_ENABLE
    sysfs_remove_group(kobj, grp);
#else
    oal_reference(kobj);
    oal_reference(grp);
#endif
}
#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
extern oal_kobject *oal_get_sysfs_root_object(void);
extern oal_kobject *oal_get_sysfs_root_boot_object(void);
extern oal_kobject *oal_conn_sysfs_root_obj_init(void);
extern void oal_conn_sysfs_root_obj_exit(void);
extern void oal_conn_sysfs_root_boot_obj_exit(void);
#endif
extern oal_file *oal_kernel_file_open(uint8_t *file_path, int32_t ul_attribute);
extern loff_t oal_kernel_file_size(oal_file *pst_file);
extern ssize_t oal_kernel_file_read(oal_file *pst_file, uint8_t *pst_buff, loff_t ul_fsize);
extern int oal_kernel_file_write(oal_file *pst_file, uint8_t *pst_buf, loff_t fsize);
extern int oal_kernel_file_print(oal_file *pst_file, const char *pc_fmt, ...);

#endif /* end of oal_main */
