# get directory of this append file while in global context:
# required for eMMC/SD image build to pick up scatter file,
# preloader, etc from this directory

MTK_IMAGE_FILEDIR_mt7623-evb := "${THISDIR}"
MTK_IMAGE_FILEDIR_mt7623-bpi-r2 := "${THISDIR}"
