#!/bin/sh

# define primary and secondary rootfs partition
ROOTFS1="1"
ROOTFS2="2"

if [ $# -lt 1 ]; then
	exit 0;
fi

get_current_root_device ()
{
	for i in `cat /proc/cmdline`; do
		if [ ${i:0:5} = "root=" ]; then
			CURRENT_ROOT="${i:5}"
		fi
	done
	echo "current root partition: $CURRENT_ROOT"
}

get_update_part ()
{
	ROOT_LEN=${#CURRENT_ROOT}
	PART_POS=$((ROOT_LEN - 1))
	CURRENT_PART="${CURRENT_ROOT:$PART_POS:1}"
	if [ $CURRENT_PART = $ROOTFS1 ]; then
		UPDATE_PART=$ROOTFS2;
	else
		UPDATE_PART=$ROOTFS1;
	fi
	echo "update partition number: $UPDATE_PART"
}

get_update_device ()
{
	UPDATE_ROOT=${CURRENT_ROOT%?}${UPDATE_PART}
	echo "update root partition: $UPDATE_ROOT"
}

format_update_device ()
{
	umount $UPDATE_ROOT
	mke2fs -b 1024 -j -O sparse_super,filetype,^resize_inode,dir_index,^ext_attr,^64bit,^dir_nlink,extent,extra_isize,flex_bg,^huge_file,large_file,^meta_bg,^quota -I 128 -i 4096 -F -L rootfs${UPDATE_PART} $UPDATE_ROOT
}

##################################################################

# get the current root device and update partition number
get_current_root_device
get_update_part

if [ $1 == "preinst" ]; then
	# get the device to be updated
	get_update_device

	# format the device to be updated
	format_update_device

	# create a symlink for the update process
	ln -sf $UPDATE_ROOT /dev/update
fi

if [ $1 == "postinst" ]; then
	# adjust u-boot boot partition variable
	fw_setenv boot_part $UPDATE_PART
fi
