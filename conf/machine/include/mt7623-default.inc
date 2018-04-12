# Mediatek MT7623 default BSP

# TODO: try adding hard float (hf) - this currently makes u-boot choke and
# the perl cross-compile config probably needs adjusting as well. Not a
# priority since performance gains are likely minimal - routers don't use FP
#DEFAULTTUNE ?= "cortexa7thf-neon-vfpv4"

# Cortex A7 Thumb, Neon VFPv4
DEFAULTTUNE ?= "cortexa7t-neon-vfpv4"
require conf/machine/include/tune-cortexa7.inc

PREFERRED_PROVIDER_virtual/kernel = "linux-mtk"
PREFERRED_VERSION_linux-mtk = "4.14%"
LINUX_VERSION_EXTENSION ?= "-mt7623"

KERNEL_IMAGETYPE ?= "zImage"

UBOOT_LOADADDRESS = "0x80008000"
UBOOT_ENTRYPOINT = "0x80008000"

PREFERRED_PROVIDER_virtual/bootloader = "u-boot"
PREFERRED_VERSION_u-boot = "2014.04"
PREFERRED_VERSION_u-boot-fw-utils = "2014.04"

# kernel and devicetree should be part of rootfs
IMAGE_INSTALL_append = " kernel kernel-modules"

# extra options for embedded to minimize Flash wear
EXTRA_IMAGECMD_ext4 = "-b 1024 -j -O sparse_super,filetype,^resize_inode,dir_index,^ext_attr,^64bit,^dir_nlink,extent,extra_isize,flex_bg,^huge_file,large_file,^meta_bg,^quota -I 128 -i 4096"

SOC_FAMILY = "mt7623"
require conf/machine/include/soc-family.inc

# turn errors into warnings in cases where original recipe is missing
BB_DANGLINGAPPENDS_WARNONLY = "1"

# all MT7623 platforms so far use either eMMC or SD cards, so the persistent data
# partition shall be the third partition of either one of these.
# This partition will be created during first boot if it doesn't already exist.
# Check the 'initscripts' package extension that's part of this layer
PERSISTENT_DATA_PART = "3"
