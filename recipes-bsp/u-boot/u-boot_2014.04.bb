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

# using the same old base version that Mediatek provides with their
# MT7623 SDK
SRCREV = "e4b87e5b1d026bd010e2ba3abbf89561e8320287"
LIC_FILES_CHKSUM = "file://Licenses/README;md5=025bf9f768cbcb1a165dbe1a110babfb"
PV = "2014.04"

include u-boot-mtk.inc
require recipes-bsp/u-boot/u-boot.inc

SRC_URI = " \
    git://github.com/u-boot/u-boot.git;protocol=https \
    file://0000-compile-with-gcc6.patch \
    file://0001-add-mt7623-boards.patch \
    file://0010-mtk-eth-driver.patch \
    file://0020-mtk-nand-support.patch \
    file://0030-mtk-nor-flash-support.patch \
    file://0040-mtk-mmc-support.patch \
    file://0050-mtk-misc-driver-support.patch \
    file://0098-compile-u-boot-fw-utils.patch \
    file://0099-backport-u-boot-fw-utils.patch \
    "

S = "${WORKDIR}/git"

UBOOT_MAKE_TARGET = "all env"

# copy all additional MTK files in one fellow swoop
copy_mtk_additions () {
	cp -r ${THISDIR}/files/source/* ${S}
}

# when adding additional machine layers, set this variable
# in that layer's .bbappend file to point to local config
# files
MTK_UBOOT_CFGDIR := "${THISDIR}/files"

# config files are machine specific and there can be multiple
# targets (SD, eMMC) per machine
copy_cfg_files () {
    for type in ${UBOOT_CONFIG}; do
        cp ${MTK_UBOOT_CFGDIR}/${MACHINE}/${MACHINE}-${type}.h ${S}/include/configs
        cp ${MTK_UBOOT_CFGDIR}/${MACHINE}/${MACHINE}-${type}-env.txt ${S}
    done
}

python do_unpack_append () {
    bb.build.exec_func('copy_mtk_additions', d)
    bb.build.exec_func('copy_cfg_files', d)
}

# this seems to be mysteriously missing from uboot-config.bbclass
# Yocto bug?
do_configure_append () {
    if [ -n "${UBOOT_CONFIG}" ]; then
        unset i j
        for config in ${UBOOT_MACHINE}; do
            i=$(expr $i + 1);
            for type in ${UBOOT_CONFIG}; do
                j=$(expr $j + 1);
                if [ $j -eq $i ]; then
                    mkdir -p ${B}/${config}
                fi
            done
            unset  j
        done
        unset  i
    fi
}


do_compile_append () {
    # using UBOOT_CONFIG instead of UBOOT_MACHINE will create builds in the
    # ${config} subdirectory. We need to take this into account when calling
    # mkenvimage.
    if [ -n "${UBOOT_CONFIG}" ]; then
        unset i j
        for config in ${UBOOT_MACHINE}; do
            i=$(expr $i + 1);
            for type in ${UBOOT_CONFIG}; do
                j=$(expr $j + 1);
                if [ $j -eq $i ]; then
                    ${B}/${config}/tools/mkenvimage -s 16384 -o u-boot-env-${MACHINE}-${type}.bin ${S}/${MACHINE}-${type}-env.txt
                fi
            done
            unset  j
        done
        unset  i
    else
        tools/mkenvimage -s 16384 -o u-boot-env-${MACHINE}.bin ${S}/${MACHINE}-env.txt
    fi
}

do_deploy_append () {
    for env in `ls ${B}/u-boot-env-*` ; do
        install ${env} ${DEPLOYDIR}
    done
}

# Some MT7623 preloader seem to require u-boot to have a custom 512 byte
# header created by MTK's 'mkimage' app:
do_compile_append_mt7623-evb () {
    if [ -n "${UBOOT_CONFIG}" ]; then
        unset i j
        for config in ${UBOOT_MACHINE}; do
            i=$(expr $i + 1);
            for type in ${UBOOT_CONFIG}; do
                j=$(expr $j + 1);
                if [ $j -eq $i ]; then
                    ${S}/mkimage ${B}/${config}/u-boot-${type}.bin UBOOT > ${B}/${config}/u-boot-${type}-mtk.bin
                    mv ${B}/${config}/u-boot-${type}-mtk.bin ${B}/${config}/u-boot-${type}.bin
                fi
            done
            unset  j
        done
        unset  i
    else
        ${S}/mkimage u-boot.bin UBOOT > u-boot-mtk.bin
        mv u-boot-mtk.bin u-boot.bin
    fi
}


PACKAGE_ARCH = "${MACHINE_ARCH}"
COMPATIBLE_MACHINE = "(mt7623-evb|mt7623-bpi-r2)"
