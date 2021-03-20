#ifndef DUBAI_DISPLAY_STATS_H
#define DUBAI_DISPLAY_STATS_H

#ifdef CONFIG_HUAWEI_DUBAI_RGB_STATS
#include <video/fbdev/hisi/dss/product/rgb_stats/hisi_fb_rgb_stats.h>
#else /* !CONFIG_HUAWEI_DUBAI_RGB_STATS */
struct hisifb_rgb_data {};
#endif /* !CONFIG_HUAWEI_DUBAI_RGB_STATS */
typedef struct hisifb_rgb_data rgb_data_t;

void dubai_display_stats_init(void);
int dubai_set_brightness_enable(void __user *argp);
int dubai_get_brightness_info(void __user *argp);
int dubai_set_rgb_enable(void __user *argp);
int dubai_get_rgb_info(void __user *argp);

#endif // DUBAI_DISPLAY_STATS_H
