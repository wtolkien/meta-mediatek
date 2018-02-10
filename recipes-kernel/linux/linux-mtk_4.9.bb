DESCRIPTION = "Linux Kernel for Mediatek ARM SoCs (MT7623A/N)"
SECTION = "kernel"
LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LINUX_VERSION ?= "4.9"
KBRANCH ?= "linux-4.9.y"
KERNEL_VERSION_SANITY_SKIP = "1"
SRCREV ?= "7bbc6ca4887794cc44b41412a35bdfbe0cbd1c50"
SRC_URI = " \
	git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git;branch=${KBRANCH} \
	file://${MACHINE}/defconfig \
	"
KERNEL_IMAGETYPE="zImage"

PROVIDES += " virtual/kernel"

DEPENDS = "lzop-native u-boot"

inherit kernel siteinfo
require recipes-kernel/linux/linux-yocto.inc

do_patch[depends] = "quilt-native:do_populate_sysroot"

PV = "${LINUX_VERSION}+git${SRCPV}"

KCONF_BSP_AUDIT_LEVEL = "0"

linux_mtk_copy_and_patch() {
	# copy new source files directly instead of using patch mechanism to
	# generate new files
	cp -r ${THISDIR}/${PN}-${LINUX_VERSION}/openwrt/files-generic/* ${S}
	cp -r ${THISDIR}/${PN}-${LINUX_VERSION}/openwrt/files-mtk/* ${S}

	# now apply all OpenWRT patches
	cd ${S}
	for i in backport pending hack patches; do \
		QUILT_PATCHES=${THISDIR}/${PN}-${LINUX_VERSION}/openwrt/$i-${LINUX_VERSION} quilt push -a; \
		rm -rf .pc; \
	done
}

# We invoke base do_patch at end, to incorporate any local patch
python do_patch() {
    bb.build.exec_func('linux_mtk_copy_and_patch', d)
    bb.build.exec_func('patch_do_patch', d)
}

COMPATIBLE_MACHINE = "(mt7623-evb|oxygen-ap)"
