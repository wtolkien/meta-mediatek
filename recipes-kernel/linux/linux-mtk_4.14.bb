DESCRIPTION = "Linux Kernel for Mediatek ARM SoCs (MT7623A/N)"
SECTION = "kernel"
LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LINUX_VERSION ?= "4.14"
KBRANCH ?= "linux-4.14.y"
KERNEL_VERSION_SANITY_SKIP = "1"

# kernel 4.14.26
SRCREV ?= "9d760dedb949debdbc7c810196932da570e83139"

SRC_URI = " \
	git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git;branch=${KBRANCH} \
	file://defconfig \
	\
	file://mt7623a-rfb-emmc.dts \
	file://mt7623n-bananapi-bpi-r2.dts \
	\
	file://0006-reset-mediatek-mt2701-reset-driver.patch \
	file://0012-clk-dont-disable-unused-clocks.patch \
	file://0027-net-next-mediatek-fix-DQL-support.patch \
	file://0032-net-dsa-mediatek-add-support-for-GMAC2-wired-to-ext-.patch \
	file://0033-dsa-multi-cpu.patch \
	file://0035-net-mediatek-disable-RX-VLan-offloading.patch \
	file://0042-net-next-mediatek-honour-special-tag-bit-inside-RX-D.patch \
	file://0043-net-next-mediatek-enable-special-tag-indication-for-.patch \
	file://0044-net-next-dsa-mediatek-tell-GDMA-when-we-are-turning-.patch \
	file://0045-net-dsa-mediatek-turn-into-platform-driver.patch \
	file://0046-net-mediatek-add-irq-delay.patch \
	file://0048-net-core-add-RPS-balancer.patch \
	file://0051-net-mediatek-increase-tx_timeout.patch \
	file://0052-net-phy-add-FC.patch \
	file://0062-mdio-atomic.patch \
	file://0063-atomic-sleep.patch \
	file://0064-dts.patch \
	file://0400-iommu.patch \
	file://0500-mt7623a-disable-scpsys_mfg-clk.patch \
	"

# BPI-R2 uses MT7623N chip, remove patch that's for MT7623A only
SRC_URI_remove_mt7623-bpi-r2 = " \
	file://0500-mt7623a-disable-scpsys_mfg-clk.patch \
	"

# BPI-R2 requires uImage with appended devicetree
SRC_URI_append_mt7623-bpi-r2 = " \
	file://defconfig-fragement-mt7623-bpi-r2.cfg \
	"

# copy our own devicetree files to kernel source directory
do_configure_prepend () {
	cp ${B}/../*.dts ${S}/arch/arm/boot/dts
}

PROVIDES += " virtual/kernel"

DEPENDS = "lzop-native u-boot u-boot-mkimage-native"

inherit kernel siteinfo
require recipes-kernel/linux/linux-yocto.inc

PV = "${LINUX_VERSION}+git${SRCPV}"

KCONF_BSP_AUDIT_LEVEL = "0"

COMPATIBLE_MACHINE = "(mt7623-evb|mt7623-bpi-r2)"

# BPI-R2 requires uImage (no idea why zImage doesn't work...)
# Yocto's default uImage target does not append devicetree, so we strip the existing
# uImage header, add the dtb file and re-create the uImage header:
do_compile_append_mt7623-bpi-r2 () {
	if [ ! `${STAGING_BINDIR_NATIVE}/uboot-mkimage -l ${B}/arch/${ARCH}/boot/uImage | grep "Image Name" | grep "dtb"`]; then
		dd if=${B}/arch/${ARCH}/boot/uImage of=${B}/arch/${ARCH}/boot/zImage bs=64 skip=1
		cat ${B}/arch/${ARCH}/boot/zImage ${B}/arch/${ARCH}/boot/dts/${KERNEL_DEVICETREE} > ${B}/arch/${ARCH}/boot/zImage.dtb
		uboot-mkimage -A arm -O linux -T kernel -C none -a ${UBOOT_LOADADDRESS} -e ${UBOOT_ENTRYPOINT} -n "Linux-dtb-${PV}" \
			-d ${B}/arch/${ARCH}/boot/zImage.dtb ${B}/arch/${ARCH}/boot/uImage
		rm ${B}/arch/${ARCH}/boot/zImage.dtb
	fi
}
