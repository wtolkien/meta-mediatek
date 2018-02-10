#
# Mediatek MT7623 image generation
#

DEPENDS += "parted-native u-boot"

inherit image_types

EMMC_FILE = "${IMGDEPLOYDIR}/tmp.emmc"
GPT_FILE = "${IMGDEPLOYDIR}/gpt-${MACHINE}.bin"
SCATTER_TPL = "${IMGDEPLOYDIR}/scatter.template"

# Create a the partition table. Since we only have 3 partitions we
# could get away with an MSDOS style MBR, but we still use GPT for no
# particular reason
generate_gpt () {
    # Initialize a sparse file
    dd if=/dev/zero of=${EMMC_FILE} bs=1 count=0 seek=$(expr 1024 \* ${EMMC_SIZE})

    # Create partition table
    parted -s ${EMMC_FILE} mklabel gpt
    parted -a minimal -s ${EMMC_FILE} unit B mkpart primary \
        `printf "%d" ${EMMC_ROOTFS1_OFFSET}` \
        $(expr `printf "%d" ${EMMC_ROOTFS1_OFFSET}` + `printf "%d" ${EMMC_ROOTFS_SIZE}` - 1)
    parted -s ${EMMC_FILE} name 1 "ROOTFS1"
    parted -a minimal -s ${EMMC_FILE} unit B mkpart primary \
        `printf "%d" ${EMMC_ROOTFS2_OFFSET}` \
        $(expr `printf "%d" ${EMMC_ROOTFS2_OFFSET}` + `printf "%d" ${EMMC_ROOTFS_SIZE}` - 1)
    parted -s ${EMMC_FILE} name 2 "ROOTFS2"
    parted -a minimal -s ${EMMC_FILE} unit B mkpart primary \
        `printf "%d" ${EMMC_DATA_OFFSET}` \
        $(expr `printf "%d" ${EMMC_DATA_OFFSET}` + `printf "%d" ${EMMC_DATA_SIZE}` - 1)
    parted -s ${EMMC_FILE} name 3 "DATA"

    # show what we have done...
    parted ${EMMC_FILE} unit MiB print

    # extract GPT and delete original file
    dd if=${EMMC_FILE} of=${GPT_FILE} conv=fsync bs=512 count=34
    rm -f ${EMMC_FILE}
}

generate_scatter_file () {
    cp ${THISDIR}/scatter.template ${IMGDEPLOYDIR}
    sed -i "s/__PRELOADER_FILE__/preloader-${MACHINE}.bin/g" ${SCATTER_TPL}
    sed -i "s/__PART_FILE__/gpt-${MACHINE}.bin/g" ${SCATTER_TPL}
    sed -i "s/__UBOOT_ENV_FILE__/u-boot-env-${MACHINE}.bin/g" ${SCATTER_TPL}
    sed -i "s/__UBOOT_FILE__/u-boot-${MACHINE}.bin/g" ${SCATTER_TPL}
    sed -i "s/__ROOTFS1_FILE__/${PN}-${MACHINE}.ext4/g" ${SCATTER_TPL}

    sed -i "s/__UBOOT_ENV_OFFSET__/${EMMC_UBOOT_ENV_OFFSET}/g" ${SCATTER_TPL}
    sed -i "s/__UBOOT_ENV_SIZE__/${EMMC_UBOOT_ENV_SIZE}/g" ${SCATTER_TPL}
    sed -i "s/__UBOOT_OFFSET__/${EMMC_UBOOT_OFFSET}/g" ${SCATTER_TPL}
    sed -i "s/__UBOOT_SIZE__/${EMMC_UBOOT_SIZE}/g" ${SCATTER_TPL}
    sed -i "s/__ROOTFS1_OFFSET__/${EMMC_ROOTFS1_OFFSET}/g" ${SCATTER_TPL}
    sed -i "s/__ROOTFS2_OFFSET__/${EMMC_ROOTFS2_OFFSET}/g" ${SCATTER_TPL}
    sed -i "s/__ROOTFS_SIZE__/${EMMC_ROOTFS_SIZE}/g" ${SCATTER_TPL}
    sed -i "s/__DATA_OFFSET__/${EMMC_DATA_OFFSET}/g" ${SCATTER_TPL}
    sed -i "s/__DATA_SIZE__/${EMMC_DATA_SIZE}/g" ${SCATTER_TPL}
    mv ${SCATTER_TPL} ${IMGDEPLOYDIR}/scatter-${MACHINE}.txt
}

copy_preloader () {
    cp ${THISDIR}/preloader-${MACHINE}.bin ${IMGDEPLOYDIR}
}


IMAGE_CMD_emmc () {
    generate_gpt
    generate_scatter_file
    copy_preloader
}

# The emmc requires the ext4 rootfs filesystem to be built before using
# it so we must make this dependency explicit.
IMAGE_TYPEDEP_emmc += "ext4"
