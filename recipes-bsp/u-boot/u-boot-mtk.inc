# Mediatek MT7623 u-boot

# NOTE:
# this recipe contains files and patches provided by Mediatek which contain
# Mediatek copyright notices that are not GPL compatible. However, according
# to GPL rules, Mediatek's contributions should also be GPL'd.
#
# If Mediatek (or anyone else) believes they have a justifiable claim to the
# contrary, please contact the author of this bb recipe to rectify any copyright
# violations


DESCRIPTION = "u-boot for Mediatek MT7623 \
               Based on u-boot 2014-04"
LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://Licenses/README;md5=025bf9f768cbcb1a165dbe1a110babfb"

PV = "2014.04"

require recipes-bsp/u-boot/u-boot.inc

# using the same old base version that Mediatek provides with their
# MT7623 SDK
SRCREV = "e4b87e5b1d026bd010e2ba3abbf89561e8320287"

SRC_URI = " \
    git://github.com/u-boot/u-boot.git;protocol=https \
    file://0000-compile-with-gcc6.patch \
    file://0001-add-mt7623-evb-board.patch \
    file://0010-mtk-eth-driver.patch \
    file://0020-mtk-nand-support.patch \
    file://0030-mtk-nor-flash-support.patch \
    file://0040-mtk-mmc-support.patch \
    file://0050-mtk-misc-driver-support.patch \
    file://0098-compile-u-boot-fw-utils.patch \
    file://0099-backport-u-boot-fw-utils.patch \
    "

S = "${WORKDIR}/git"

# copy all additional MTK files in one fellow swoop
copy_mtk_additions () {
	cp -r ${THISDIR}/files/source/* ${S}
}

# We invoke base do_patch at end, to incorporate patches above...
python do_patch() {
    bb.build.exec_func('copy_mtk_additions', d)
    bb.build.exec_func('patch_do_patch', d)
}

PACKAGE_ARCH = "${MACHINE_ARCH}"
COMPATIBLE_MACHINE = "(mt7623-evb)"
