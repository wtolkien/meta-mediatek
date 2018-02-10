SUMMARY = "Adds an SSH server and swupdate to core-image-minimal"

# base image, change if required:
require recipes-core/images/core-image-minimal.bb

IMAGE_FEATURES += "ssh-server-openssh"

CORE_IMAGE_EXTRA_INSTALL += " \
    swupdate \
    swupdate-www \
    "
