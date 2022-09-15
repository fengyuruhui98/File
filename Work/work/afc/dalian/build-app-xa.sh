#!/bin/bash

#----------------------------------------------------------
ROOTIMGSIZE=2097152000	#  2G
BOOTIMGSIZE=67108864	# 64M

MKE4FS=make_ext4fs
OUTDIR=.

TOPDIR=`pwd`
ROOTDIR=rootfs
BOOTDIR=boot
APPDIR=appfs-dl
IMGFILE=debian-jessie-images.tgz

#[ -d ${BOOTDIR} ] || { echo "Error: ./${BOOTDIR}: not found"; exit 1; }
#[ -d ${ROOTDIR} ] || { echo "Error: ./${ROOTDIR}: not found"; exit 1; }

#----------------------------------------------------------
# Execute an action
FA_DoExec() {
	echo "${@}"
	eval $@ || exit $?
}

#----------------------------------------------------------
# Make ext4 image

mkdir -p ${OUTDIR}

#FA_DoExec ${MKE4FS} -s -l ${BOOTIMGSIZE} -a root -L boot   ${OUTDIR}/boot.img   ${BOOTDIR}
#FA_DoExec ${MKE4FS} -s -l 350M -u root -L rootfs ${OUTDIR}/rootfs.img ${ROOTDIR}

FA_DoExec ${MKE4FS} -s -l 220M -u app -L appfs ${OUTDIR}/appfs.img ${APPDIR}
#----------------------------------------------------------
# Create package & md5sum

#if [ ! -f ${OUTDIR}/partma.txt ]; then
#	cp config/partmap.txt ${OUTDIR}/ -avf
#fi

#FA_DoExec "(cd ${OUTDIR} && tar cvzf ${TOPDIR}/${IMGFILE} rootfs.img boot.img partmap.txt)"
#FA_DoExec "md5sum ${IMGFILE} > ${IMGFILE}.hash.md5"

#ls -l ${IMGFILE}*
echo "...done."

