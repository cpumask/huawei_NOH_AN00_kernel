subdir-ccflags-y += -Wall -Werror

EXTRA_CFLAGS += -Idrivers/devkit/audiokit
ifeq ($(CONFIG_HIFI_MAILBOX), y)
EXTRA_CFLAGS += -I$(srctree)/drivers/hisi/hifi_mailbox/mailbox
EXTRA_CFLAGS += -I$(srctree)/drivers/hisi/hifi_mailbox/ipcm
EXTRA_CFLAGS += -I$(srctree)/drivers/hisi/hifi_dsp
endif

ifneq ($(filter hi3660 kirin970, $(TARGET_BOARD_PLATFORM)),)
EXTRA_CFLAGS += -DAUDIO_LOW_LATENCY_LEGACY
endif



EXTRA_CFLAGS +=	-Idrivers/hisi/slimbus/
EXTRA_CFLAGS +=	-Iinclude/linux/hisi/hi64xx/
# HI3630 Platform Support
hisi-pcm-obj = /hisi_pcm_codec/format.o /hisi_pcm_codec/platform_io.o /hisi_pcm_codec/hisi_pcm_codec.o
ifeq ($(CONFIG_HISI_DEBUG_FS),y)
EXTRA_CFLAGS += -DAUDIO_HISI_PCM_CODEC_DEBUG
hisi-pcm-obj += /hisi_pcm_codec/om.o
endif
snd-soc-hi3630-objs := hisi_pcm_hifi.o
snd-soc-hi3650-objs := hisi_pcm_hifi.o slimbus_dai.o $(hisi-pcm-obj)
snd-soc-hi3660-objs := hisi_pcm_hifi.o slimbus_dai.o $(hisi-pcm-obj) asp_cfg.o


ifeq ($(CONFIG_HDMI_K3), y)
endif
obj-$(CONFIG_SND_SOC_PCM_HI3630) += snd-soc-hi3630.o
obj-$(CONFIG_SND_SOC_PCM_HI3650) += snd-soc-hi3650.o
obj-$(CONFIG_SND_SOC_PCM_HI3660) += snd-soc-hi3660.o

# DP Support
ifeq ($(CONFIG_HISI_FB_V600),y)
EXTRA_CFLAGS += -DDP_AUDIO_ASP_HDMI_I2S
endif
snd-soc-dp-objs := hisi_dp_machine.o hisi_pcm_dp.o hisi_dp_i2s.o asp_hdmi_dma.o

# HI6210 Platform Support
snd-soc-hi6210-objs := hisi_pcm_hifi.o
# HI6210-HISSC Machine Support
obj-$(CONFIG_SND_SOC_PCM_HI6210) += snd-soc-hi6210.o
# HI6250-HI6402 Machine Support
snd-soc-hi6250-hi6402-objs += hi6250_hi6402.o
# HI6250-HI6555c Machine Support
snd-soc-hi6250-hi6555c-objs += hi6250_hi6555c.o
# HI3660-HI6403 Machine Support
snd-soc-hi3660-hi6403-objs := hi3660_hi6403.o
# HI3XXX-HI6405 Machine Support
snd-soc-hi3xxx-hi6405-objs := hi3xxx_hi6405.o
snd-soc-hi3xxx-asp-codec-objs := hi3xxx_asp_codec.o

obj-$(CONFIG_SND_HI6250_HI6555C) +=snd-soc-hi6250-hi6555c.o
obj-$(CONFIG_SND_HI6250_HI6402) += snd-soc-hi6250-hi6402.o
obj-$(CONFIG_SND_HI3660_HI6403) += snd-soc-hi3660-hi6403.o
obj-$(CONFIG_SND_HI3XXX_HI6405) += snd-soc-hi3xxx-hi6405.o
obj-$(CONFIG_SND_HI3XXX_ASP_CODEC) += snd-soc-hi3xxx-asp-codec.o
obj-$(CONFIG_SND_SOC_DP_AUDIO) += snd-soc-dp.o
obj-$(CONFIG_SND_AUTO_TEST) += auto_test/
