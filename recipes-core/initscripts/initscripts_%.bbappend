FILESEXTRAPATHS_prepend := "${THISDIR}:"

SRC_URI += " \
    file://create-perst-part.sh \
    "

MASKED_SCRIPTS += " \
    create-perst-part.sh \
    "

do_configure_append_mt7623 () {
    sed -i -e "s:SED_EXT4ARGS:${EXTRA_IMAGECMD_ext4}:g" ${WORKDIR}/create-perst-part.sh
    sed -i -e "s:SED_PPART:${PERSISTENT_DATA_PART}:g" ${WORKDIR}/create-perst-part.sh
}

do_install_append () {
    install -m 0755 ${WORKDIR}/create-perst-part.sh	${D}${sysconfdir}/init.d
	update-rc.d -r ${D} create-perst-part.sh start 01 S .
}
