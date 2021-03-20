#include <linux/platform_device.h>
#include <drm/drm_crtc.h>
#include <drm/drm_dp_helper.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/regmap.h>
#include <linux/kernel.h>
#include <linux/clk.h>

#include "hisi_panel.h"
#include "hisi_drm_drv.h"
#include "hisi_drm_dsi.h"

#define  DEFAULT_MAX_BRIGHTNESS 120000
struct mipi2edp;

enum mipi2edp_type {
	LT9711A = 0,
	SN65DSIX6 = 1,
};

enum mipi2edp_output {
	OUTPUT_LCD = 0,
	OUTPUT_VGA = 1,
};

#define DEFAULT_BRIDGE SN65DSIX6

struct gpio_config {
	int hw_enable_gpio;
	int hw_suspend_gpio;
	int lcd_vdd_enable_gpio;
	int bl_config_enable_gpio;
};

struct bridge_chip_ops {
	int (*is_chip_onboard)(struct mipi2edp  *pdata);
	void (*pre_enable)(struct mipi2edp  *pdata);
	int (*enable)(struct mipi2edp  *pdata);
	void (*disable)(struct mipi2edp  *pdata);
	void (*post_disable)(struct mipi2edp  *pdata);
};

struct mipi2edp {
	struct pinctrl_data *pinctrl;
	struct gpio_config *gpio_set;
	struct device *dev;
	struct i2c_client *client;
	struct regmap *regmap;
	const struct regmap_config *regmap_config;
	struct drm_bridge bridge;
	struct drm_connector connector;
	struct drm_encoder *encoder;
	struct clk *mipi2edp_clk;
	/* DSI RX related params */
	struct device_node *host_node;
	struct mipi_dsi_device *dsi;
	u8 num_dsi_lanes;
	unsigned int hw_hpd_gpio;
	unsigned int hw_hpd_irq;
	unsigned int patch;
	struct hisi_panel_info panel_info;
	struct backlight_device *bl;
	enum mipi2edp_output output;
	enum mipi2edp_type type;
	struct drm_dp_aux  aux;
	struct bridge_chip_ops chip;
	struct regulator *ec_1v2_en;
	struct regulator *ec_dsi_vccio_on;
	struct regulator *ec_1v8_en;
};

#define to_mipi2edp(x)  container_of(x, struct mipi2edp, bridge)
