DESCRIPTION = "Linux Kernel for Mediatek ARM SoCs (MT7623A/N)"
SECTION = "kernel"
LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LINUX_VERSION ?= "4.14.13"
KBRANCH ?= "linux-4.14.y"
SRCREV ?= "85190b5c24f0b42b46c35fd4cd98c6739a5106e8"
SRC_URI = "git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git;branch=${KBRANCH}"

DEPENDS = "lzop-native"
#DEPENDS += "u-boot-mkimage-native"

inherit kernel siteinfo
require recipes-kernel/linux/linux-yocto.inc

do_patch[depends] = "quilt-native:do_populate_sysroot"

SRC_URI += " \
		file://${MACHINE}/defconfig \
		file://${MACHINE}/source/arch/arm/boot/dts/mt7623-eMMC.dts \
		file://${MACHINE}/source/arch/arm/boot/dts/_mt7623.dtsi \
		"


PV = "${LINUX_VERSION}+git${SRCPV}"

KCONF_BSP_AUDIT_LEVEL = "0"

# copy new source files directly instead of using patch mechanism to
# generate new files
linux_mtk_copy_sources() {
	cp -r ${WORKDIR}/${MACHINE}/source/* ${S}

}

# We invoke base do_patch at end, to incorporate any local patch
python do_patch() {
    bb.build.exec_func('linux_mtk_copy_sources', d)
    bb.build.exec_func('patch_do_patch', d)
}


do_compile_append_odroid-c2 () {
	uboot-mkimage -A arm64 -O linux -T kernel -C none -a 0x1080000 -e 0x1080000 -n master -d ${B}/arch/${ARCH}/boot/Image ${KERNEL_OUTPUT_DIR}/uImage
}

do_install_append_odroid-c2 () {
	install -m 0644 ${KERNEL_OUTPUT_DIR}/uImage ${D}/${KERNEL_IMAGEDEST}/uImage
}

do_deploy_append_odroid-c2 () {
	 install -m 0644 ${D}/${KERNEL_IMAGEDEST}/uImage ${DEPLOY_DIR_IMAGE}/uImage
}

#FILES_kernel-base += " /boot/uImage "

COMPATIBLE_MACHINE = "(mt7623-evb|oxygen-ap)"
