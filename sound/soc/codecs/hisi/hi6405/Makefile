EXTRA_CFLAGS += -Idrivers/hisi/slimbus/
EXTRA_CFLAGS += -Idrivers/devkit/audiokit/
EXTRA_CFLAGS += -Idrivers/hisi/hi64xx_dsp/
EXTRA_CFLAGS += -Isound/soc/codecs/hisi/
EXTRA_CFLAGS += -Idrivers/hisi/hi64xx_dsp/config/
EXTRA_CFLAGS += -Isound/soc/codecs/hisi/hi6405/multi_kcontrol/
EXTRA_CFLAGS += -Isound/soc/codecs/hisi/hi6405/single_kcontrol/

ifeq ($(TARGET_VERSION_MODE),factory)
EXTRA_CFLAGS += -DAUDIO_FACTORY_MODE
endif

snd-soc-hi6405-objs :=hi6405.o
snd-soc-hi6405-kcontrol-objs :=hi6405_kcontrol.o
snd-soc-hi6405-codec-probe-objs :=hi6405_codec_probe.o
snd-soc-hi6405-utils-objs :=hi6405_utils.o
snd-soc-hi6405-codec-pm-objs := hi6405_codec_pm.o
snd-soc-hi6405-multi-objs :=/multi_kcontrol/hi6405_resource_widget.o /multi_kcontrol/hi6405_path_widget.o /multi_kcontrol/hi6405_switch_widget.o /multi_kcontrol/hi6405_switch_widget_utils.o /multi_kcontrol/hi6405_route.o /multi_kcontrol/hi6405_pga_widget.o
snd-soc-hi6405-single-objs :=/single_kcontrol/hi6405_single_drv_widget.o /single_kcontrol/hi6405_single_pga_widget.o /single_kcontrol/hi6405_single_switch_route.o /single_kcontrol/hi6405_single_switch_widget.o

obj-y += snd-soc-hi6405.o
obj-y += snd-soc-hi6405-kcontrol.o
obj-y += snd-soc-hi6405-single.o
obj-y += snd-soc-hi6405-multi.o
obj-y += snd-soc-hi6405-codec-probe.o
obj-y += snd-soc-hi6405-utils.o
obj-y += snd-soc-hi6405-codec-pm.o
