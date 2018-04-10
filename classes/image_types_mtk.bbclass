#
# Mediatek MT7623 image generation: eMMC and SD card
#

DEPENDS += "parted-native u-boot"

# change suffix to 'bin', default is 'rootfs' which may be a bit misleading Since
# our rootfs also contains the linux kernel and, in case of the SD image, bootloader
# and preloader
IMAGE_NAME_SUFFIX = ".bin"

inherit image_types

EMMC_TMP_FILE = "${IMGDEPLOYDIR}/ptbl-gpt.tmp"
EMMC_PART_TBL_FILE = "${IMGDEPLOYDIR}/ptbl-gpt-${MACHINE}.bin"
SCATTER_TPL = "${IMGDEPLOYDIR}/scatter.template"

SD_TMP_FILE = "${IMGDEPLOYDIR}/ptbl-msdos.tmp"
SD_PART_TBL_FILE = "${IMGDEPLOYDIR}/ptbl-msdos-${MACHINE}.bin"
SD_FILE = "${IMGDEPLOYDIR}/${IMAGE_NAME}${IMAGE_NAME_SUFFIX}.mtk.sd"

# Create the partition table for eMMC. Since we only have 3 partitions we
# could get away with an MSDOS style MBR, but we still use GPT for no
# particular reason.
generate_part_tbl_emmc () {
    # Initialize a sparse file
    dd if=/dev/zero of=${EMMC_TMP_FILE} bs=1 count=0 seek=$(expr 1024 \* ${EMMC_SIZE})

    # Create partition table
    parted -s ${EMMC_TMP_FILE} mklabel gpt
    parted -a minimal -s ${EMMC_TMP_FILE} unit B mkpart primary \
        `printf "%d" ${MTK_ROOTFS1_OFFSET}` \
        $(expr `printf "%d" ${MTK_ROOTFS1_OFFSET}` + `printf "%d" ${MTK_ROOTFS_SIZE}` - 1)
    parted -a minimal -s ${EMMC_TMP_FILE} unit B mkpart primary \
        `printf "%d" ${MTK_ROOTFS2_OFFSET}` \
        $(expr `printf "%d" ${MTK_ROOTFS2_OFFSET}` + `printf "%d" ${MTK_ROOTFS_SIZE}` - 1)
    parted -a minimal -s ${EMMC_TMP_FILE} unit B mkpart primary \
        `printf "%d" ${EMMC_DATA_OFFSET}` \
        $(expr `printf "%d" ${EMMC_DATA_OFFSET}` + `printf "%d" ${EMMC_DATA_SIZE}` - 1)
    parted -s ${EMMC_TMP_FILE} name 1 "ROOTFS1"
    parted -s ${EMMC_TMP_FILE} name 2 "ROOTFS2"
    parted -s ${EMMC_TMP_FILE} name 3 "DATA"

    # show what we have done...
    parted ${EMMC_TMP_FILE} unit MiB print

    # extract partition table and delete original file
    dd if=${EMMC_TMP_FILE} of=${EMMC_PART_TBL_FILE} conv=fsync bs=512 count=34
    rm -f ${EMMC_TMP_FILE}
}

# Create the partition table for SD cards. We need to use legacy MSDOS format
# since the preloader is expected at offset 0x800.
generate_part_tbl_sd () {
    # Initialize a sparse file
    dd if=/dev/zero of=${SD_TMP_FILE} bs=1 count=0 seek=$(expr 1024 \* ${SD_SIZE})

    # Create partition table
    parted -s ${SD_TMP_FILE} mklabel msdos
    parted -a minimal -s ${SD_TMP_FILE} unit B mkpart primary \
        `printf "%d" ${MTK_ROOTFS1_OFFSET}` \
        $(expr `printf "%d" ${MTK_ROOTFS1_OFFSET}` + `printf "%d" ${MTK_ROOTFS_SIZE}` - 1)
    parted -a minimal -s ${SD_TMP_FILE} unit B mkpart primary \
        `printf "%d" ${MTK_ROOTFS2_OFFSET}` \
        $(expr `printf "%d" ${MTK_ROOTFS2_OFFSET}` + `printf "%d" ${MTK_ROOTFS_SIZE}` - 1)
    parted -a minimal -s ${SD_TMP_FILE} unit B mkpart primary \
        `printf "%d" ${SD_DATA_OFFSET}` \
        $(expr `printf "%d" ${SD_DATA_OFFSET}` + `printf "%d" ${SD_DATA_SIZE}` - 1)

    # show what we have done...
    parted ${SD_TMP_FILE} unit MiB print

    # extract partition table and delete original file
    dd if=${SD_TMP_FILE} of=${SD_PART_TBL_FILE} conv=fsync bs=512 count=1
    rm -f ${SD_TMP_FILE}
}



generate_scatter_file () {
    cp ${MTK_IMAGE_FILEDIR}/scatter.template ${IMGDEPLOYDIR}
    sed -i "s/__PRELOADER_FILE__/preloader-${MACHINE}.bin/g" ${SCATTER_TPL}
    sed -i "s/__PART_FILE__/ptbl-gpt-${MACHINE}.bin/g" ${SCATTER_TPL}
    sed -i "s/__UBOOT_ENV_FILE__/u-boot-env-${MACHINE}-emmc.bin/g" ${SCATTER_TPL}
    sed -i "s/__UBOOT_FILE__/u-boot-${MACHINE}.bin-emmc/g" ${SCATTER_TPL}
    sed -i "s/__ROOTFS1_FILE__/${PN}-${MACHINE}.ext4/g" ${SCATTER_TPL}

    sed -i "s/__UBOOT_ENV_OFFSET__/${EMMC_UBOOT_ENV_OFFSET}/g" ${SCATTER_TPL}
    sed -i "s/__UBOOT_ENV_SIZE__/${EMMC_UBOOT_ENV_SIZE}/g" ${SCATTER_TPL}
    sed -i "s/__UBOOT_OFFSET__/${EMMC_UBOOT_OFFSET}/g" ${SCATTER_TPL}
    sed -i "s/__UBOOT_SIZE__/${EMMC_UBOOT_SIZE}/g" ${SCATTER_TPL}
    sed -i "s/__ROOTFS1_OFFSET__/${MTK_ROOTFS1_OFFSET}/g" ${SCATTER_TPL}
    sed -i "s/__ROOTFS2_OFFSET__/${MTK_ROOTFS2_OFFSET}/g" ${SCATTER_TPL}
    sed -i "s/__ROOTFS_SIZE__/${MTK_ROOTFS_SIZE}/g" ${SCATTER_TPL}
    sed -i "s/__DATA_OFFSET__/${EMMC_DATA_OFFSET}/g" ${SCATTER_TPL}
    sed -i "s/__DATA_SIZE__/${EMMC_DATA_SIZE}/g" ${SCATTER_TPL}
    mv ${SCATTER_TPL} ${IMGDEPLOYDIR}/scatter-${MACHINE}.txt
}

copy_emmc_preloader () {
    cp ${MTK_IMAGE_FILEDIR}/preloader-${MACHINE}.bin ${IMGDEPLOYDIR}
}

assemble_sd_image () {
    # Initialize a sparse SD image file that's just big enough to cover the
    # space before the first rootfs
    dd if=/dev/zero of=${SD_FILE} bs=1 count=0 seek=`printf "%d" ${MTK_ROOTFS1_OFFSET}`

    # now fill that space, start with proprietary 2k header provided by MTK
    dd if=${MTK_IMAGE_FILEDIR}/sd-card-header.bin of=${SD_FILE} bs=512 count=4 conv=notrunc
    # replace primary partions with our own (but leave rest of MBR as-is)
    dd if=${SD_PART_TBL_FILE} of=${SD_FILE} seek=446 skip=446 bs=1 conv=notrunc
    # add preloader at address 0x800
    dd if=${MTK_IMAGE_FILEDIR}/preloader-${MACHINE}.bin of=${SD_FILE} bs=512 seek=4 conv=notrunc
    # add u-boot and u-boot environment
    dd if=${DEPLOY_DIR_IMAGE}/u-boot-env-${MACHINE}-sd.bin of=${SD_FILE} bs=1 seek=`printf "%d" ${EMMC_UBOOT_ENV_OFFSET}` conv=notrunc
    dd if=${DEPLOY_DIR_IMAGE}/u-boot-${MACHINE}.bin-sd of=${SD_FILE} bs=1 seek=`printf "%d" ${EMMC_UBOOT_OFFSET}` conv=notrunc

    # append the rootfs
    dd if=${IMGDEPLOYDIR}/${PN}-${MACHINE}.ext4 of=${SD_FILE} bs=64k oflag=append conv=notrunc
}


IMAGE_CMD_mtk.emmc () {
    generate_part_tbl_emmc
    generate_scatter_file
    copy_emmc_preloader
}

IMAGE_CMD_mtk.sd () {
    generate_part_tbl_sd
    assemble_sd_image
}

# Both eMMC and SD requires the ext4 rootfs filesystem to be built before using
# it so we must make this dependency explicit.
IMAGE_TYPEDEP_mtk.emmc += "ext4"
IMAGE_TYPEDEP_mtk.sd += "ext4"
