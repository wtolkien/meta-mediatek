# Mediatek MT7623 default BSP

#include conf/machine/include/samsung-exynos.inc
#include conf/machine/include/odroid-default-settings.inc

# TODO: try adding hard float (hf)
#DEFAULTTUNE ?= "cortexa7thf-neon-vfpv4"

# Cortex A7 Thumb, Neon VFPv4
DEFAULTTUNE ?= "cortexa7t-neon-vfpv4"
require conf/machine/include/tune-cortexa7.inc

PREFERRED_PROVIDER_virtual/kernel ?= "linux-mtk"
PREFERRED_VERSION_linux-mtk ?= "4.9%"
LINUX_VERSION_EXTENSION ?= "-mt7623"

KERNEL_IMAGETYPE ?= "zImage"

PREFERRED_PROVIDER_virtual/bootloader = "u-boot"
PREFERRED_PROVIDER_u-boot = "u-boot"
PREFERRED_VERSION_u-boot = "2014.04"

SOC_FAMILY = "mediatek:mt7623"
require conf/machine/include/soc-family.inc

SERIAL_CONSOLE = "115200 ttyS0"
