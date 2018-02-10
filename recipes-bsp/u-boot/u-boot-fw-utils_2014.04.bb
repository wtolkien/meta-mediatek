# Mediatek MT7623 u-boot-fw-utils

include u-boot-mtk.inc

PROVIDES = "u-boot-fw-utils"

FILES_${PN} += " \
    ${base_sbindir}/fw_printenv \
    ${base_sbindir}/fw_setenv \
    "

INSANE_SKIP_${PN} = "ldflags"

SRC_URI += " \
    file://fw_env.config \
    "

do_compile () {
    cp ${WORKDIR}/fw_env.config ${S}/tools/env
    oe_runmake -C ${S} ${UBOOT_MACHINE}
    oe_runmake -C ${S} env
}

do_install () {
    install -d ${D}${base_sbindir}
    install -d ${D}${sysconfdir}
	install -m 755 ${S}/tools/env/fw_printenv ${D}${base_sbindir}/fw_printenv
	install -m 755 ${S}/tools/env/fw_printenv ${D}${base_sbindir}/fw_setenv
	install -m 0644 ${S}/tools/env/fw_env.config ${D}${sysconfdir}/fw_env.config
}

do_deploy () {
}
