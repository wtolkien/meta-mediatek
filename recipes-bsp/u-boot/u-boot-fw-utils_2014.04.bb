# Mediatek MT7623 u-boot-fw-utils
DESCRIPTION = "u-boot-fw-utils for Mediatek MT7623 \
               Based on u-boot 2014-04"
LICENSE = "GPLv2+"

# using the same old base version that Mediatek provides with their
# MT7623 SDK
SRCREV = "e4b87e5b1d026bd010e2ba3abbf89561e8320287"
LIC_FILES_CHKSUM = "file://Licenses/README;md5=025bf9f768cbcb1a165dbe1a110babfb"
PV = "2014.04"

require recipes-bsp/u-boot/u-boot.inc

SRC_URI = " \
    git://github.com/u-boot/u-boot.git;protocol=https \
    file://0000-compile-with-gcc6.patch \
    file://0098-compile-u-boot-fw-utils.patch \
    file://0099-backport-u-boot-fw-utils.patch \
    file://fw_env.config \
    "

S = "${WORKDIR}/git"

PROVIDES = "u-boot-fw-utils"
RPROVIDES_${PN} = "u-boot-fw-utils"

INSANE_SKIP_${PN} = "ldflags"

do_compile () {
    oe_runmake -C ${S} sandbox_config
    oe_runmake -C ${S} env
}

do_install () {
    install -d ${D}${base_sbindir}
    install -m 755 ${S}/tools/env/fw_printenv ${D}${base_sbindir}/fw_printenv
    install -m 755 ${S}/tools/env/fw_printenv ${D}${base_sbindir}/fw_setenv
    install -d ${D}${sysconfdir}
    install -m 644 ${WORKDIR}/fw_env.config ${D}${sysconfdir}/fw_env.config
}

# remove deploy task which would otherwise be provided by u-boot.inc
do_deploy[noexec] = "1"

FILES_${PN} = " \
    ${base_sbindir}/fw_printenv \
    ${base_sbindir}/fw_setenv \
    ${sysconfdir}/fw_env.config \
    "

# post-installation actions on target device: we need to figure out what MMC
# device we booted from to setup the u-boot environment config file accordingly:
# 'mmcblk0' -> eMMC
# 'mmcblk1' -> SD card
pkg_postinst_${PN} () {
    if [ -n "`cat ${sysconfdir}/fw_env.config | grep \"/dev/null\"`" ]; then
        for bootarg in `cat /proc/cmdline` ; do
            if [ ${bootarg:0:5} = "root=" ]; then
                if [ ${bootarg:10:7} = "mmcblk0" ]; then
                    sed -i 's|null|mmcblk0|' ${sysconfdir}/fw_env.config
                fi
                if [ ${bootarg:10:7} = "mmcblk1" ]; then
                    sed -i 's|null|mmcblk1|' ${sysconfdir}/fw_env.config
                fi
            fi
        done
    fi
}
