FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

# use version 2017.11. This is an ugly patch, but currently (Feb 2018) the
# meta-swupdate layer only supports version 2017.07 and we can't easily
# add a swupdate_2017.11.bb file here, because it doesn't work with the
# file paths from the include files in the meta-swupdate layer
#PV = "2017.11"
#SRCREV = "c0fec16b3fc82b0db12d8ac58be7055ed1b8d439"

SRC_URI += "\
	file://head_bg.gif \
	file://swupdate.cfg \
	file://swupdate.default \
	file://suid.patch \
	file://0010-make-notifier-socket-reusable.patch \
"

# original swupdate bb file only sets 'DEPENDS' variable, but not 'RDEPENDS'
RDEPENDS_${PN} += " u-boot-fw-utils e2fsprogs"

do_install_append () {
	install -m 755 ${WORKDIR}/head_bg.gif ${D}/www/
	install -m 644 ${WORKDIR}/swupdate.cfg ${D}${sysconfdir}/
	install -d ${D}${sysconfdir}/default/
	install -m 644 ${WORKDIR}/swupdate.default ${D}${sysconfdir}/default/swupdate
	# setting HW rev to 1.0, individual board scripts can override this
	# if required. Needs to match "hardware-compatibility" in swupdate's
	# sw-description file
	echo "${MACHINE} 1.0" > ${D}${sysconfdir}/hwrevision
}
