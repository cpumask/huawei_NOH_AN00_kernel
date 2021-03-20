#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/regulator/consumer.h>
#include <huawei_platform/log/hw_log.h>
#include <dsm/dsm_pub.h>

#ifdef CONFIG_SND_SOC_HISI_PA
#include <sound/hisi_pa.h>
#endif

#ifdef CONFIG_HUAWEI_DSM_AUDIO
#include <dsm_audio/dsm_audio.h>
#endif

#ifdef CONFIG_HUAWEI_DEVICEBOX_AUDIO
#include <boxid/boxid.h>
#endif

#include "smartpakit.h"

#define HWLOG_TAG smartpakit
HWLOG_REGIST();

int smartpakit_i2c_freeze(struct device *dev)
{
	struct smartpakit_i2c_priv *i2c_priv = NULL;
	struct smartpakit_gpio_irq *irq_handler = NULL;
	hwlog_info("%s: begin\n", __func__);
	if (dev == NULL) {
		hwlog_err("%s: invalid argument\n", __func__);
		return -EINVAL;
	}
	i2c_priv = dev_get_drvdata(dev);
	if (i2c_priv == NULL) {
		hwlog_err("%s: invalid i2c_priv\n", __func__);
		return -EINVAL;
	}

	irq_handler = i2c_priv->irq_handler;
	if (irq_handler != NULL) {
		disable_irq(irq_handler->irq);
		hwlog_info("%s: end and disable irq : %d\n", __func__, irq_handler->irq);
	}

	return 0;
}

int smartpakit_i2c_restore(struct device *dev)
{
	int ret = 0;
	struct smartpakit_i2c_priv *i2c_priv = NULL;
	struct smartpakit_priv *pakit_priv = NULL;
	struct smartpakit_gpio_irq *irq_handler = NULL;
	if (dev == NULL) {
		hwlog_err("%s: invalid argument\n", __func__);
		return -EINVAL;
	}

	i2c_priv = dev_get_drvdata(dev);
	if (i2c_priv == NULL) {
		hwlog_err("%s: invalid i2c_priv\n", __func__);
		return -EINVAL;
	}

	hwlog_info("%s: begin \n", __func__);

	// hw reset chips, init and resume power
	pakit_priv = i2c_priv->priv_data;
	if (pakit_priv == NULL) {
		hwlog_err("%s: invalid pakit_priv\n", __func__);
		return -EINVAL;
	}

	irq_handler = i2c_priv->irq_handler;
	mutex_lock(&pakit_priv->irq_handler_lock);
	mutex_lock(&pakit_priv->hw_reset_lock);
	mutex_lock(&pakit_priv->dump_regs_lock);
	mutex_lock(&pakit_priv->i2c_ops_lock);

	ret = smartpakit_hw_reset_chips(pakit_priv);
	if (ret != 0) {
		hwlog_err("%s: smartpakit_hw_reset_chips error[%d]\n", __func__, ret);
		goto err_out;
	}

	ret = smartpakit_resume_chips(pakit_priv);
	if (ret != 0) {
		hwlog_err("%s: smartpakit_resume_chips error[%d]\n", __func__, ret);
		goto err_out;
	}

	mutex_unlock(&pakit_priv->i2c_ops_lock);
	mutex_unlock(&pakit_priv->dump_regs_lock);
	mutex_unlock(&pakit_priv->hw_reset_lock);
	mutex_unlock(&pakit_priv->irq_handler_lock);
	if (irq_handler != NULL) {
		enable_irq(irq_handler->irq);
		hwlog_info("%s: end and enable irq : %d\n", __func__, irq_handler->irq);
	}

	return 0;

err_out:
	mutex_unlock(&pakit_priv->i2c_ops_lock);
	mutex_unlock(&pakit_priv->dump_regs_lock);
	mutex_unlock(&pakit_priv->hw_reset_lock);
	mutex_unlock(&pakit_priv->irq_handler_lock);
	return -EINVAL;
}