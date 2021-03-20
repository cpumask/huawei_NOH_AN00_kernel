#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/errno.h>

#include "npu_log.h"
#include "npu_platform.h"

int devdrv_hisi_npu_iova_probe(struct platform_device *pdev)
{
	struct devdrv_platform_info *platform_info = NULL;

	NPU_DRV_DEBUG("devdrv_smmu_probe start \n");

	platform_info = devdrv_plat_get_info();
	if (platform_info == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_info failed.\n");
		return -ENODEV;
	}

	DEVDRV_PLAT_GET_PSMMUDEV(platform_info) = &pdev->dev;
	return 0;
}

int devdrv_hisi_npu_iova_remove(struct platform_device *pdev)
{
	return 0;
}

static const struct of_device_id hisi_npu_iova_of_match[] = {
	{ .compatible = "hisi,npu_iova",},
	{},
};
MODULE_DEVICE_TABLE(of, hisi_npu_iova_of_match);

static struct platform_driver devdrv_hisi_npu_iova_driver = {
	.probe = devdrv_hisi_npu_iova_probe,
	.remove = devdrv_hisi_npu_iova_remove,
	.driver = {
		.name = "hisi_npu_iova",
		.of_match_table = hisi_npu_iova_of_match,
	},
};

int devdrv_hisi_npu_iova_init(void)
{
	int ret;

	NPU_DRV_DEBUG("devdrv_hisi_npu_iova_init started\n");

	ret = platform_driver_register(&devdrv_hisi_npu_iova_driver);
	if (ret) {
		NPU_DRV_ERR("insmod devdrv hisi_npu_iova driver fail\n");
		return ret;
	}
	NPU_DRV_DEBUG("devdrv_hisi_npu_iova_init succeed\n");

	return ret;
}

void devdrv_hisi_npu_iova_exit(void)
{
	platform_driver_unregister(&devdrv_hisi_npu_iova_driver);
}
