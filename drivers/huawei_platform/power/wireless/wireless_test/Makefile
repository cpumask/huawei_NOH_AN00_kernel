obj-$(CONFIG_WIRELESS_CHARGER) += wireless_charge_mmi/
obj-$(CONFIG_WIRELESS_CHARGER) += wireless_hardware/
obj-$(CONFIG_WIRELESS_CHARGER) += wireless_coil/

ifeq ($(TARGET_VERSION_MODE),factory)
	obj-$(CONFIG_WIRELESS_CHARGER) += wireless_charge_wp/
endif
