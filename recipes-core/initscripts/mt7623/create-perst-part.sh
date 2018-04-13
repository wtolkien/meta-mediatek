#!/bin/sh
### BEGIN INIT INFO
# Provides: create-perst-part
# Required-Start:
# Required-Stop:
# Default-Start:     S
# Default-Stop:
### END INIT INFO

# find out which device we booted from, use same device for data
# partition
for i in `cat /proc/cmdline`; do
	if [ ${i:0:16} = "root=/dev/mmcblk" ]; then
		DATA_PART="${i:5:13}SED_PPART"
	fi
done

if [ -z "`file -sL ${DATA_PART} | grep \"ext4 filesystem data\"`" ]; then
	test "$VERBOSE" != no && echo "creating persistent data partition on ${DATA_PART}..."
	/sbin/mkfs.ext4 SED_EXT4ARGS ${DATA_PART} > /dev/null
fi
sed -i "s|#placeholder|${DATA_PART}|g" /etc/fstab
mkdir -p /var/persistent
