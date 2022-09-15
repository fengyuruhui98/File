#!/bin/bash

#----------------------------------------------------------
#16m(u-boot\env\logo),64m(dtb\kernel),512m(rootfs)256m(appfs) "
KERNELIMGSIZE=18874368	# 18M
ROOTIMGSIZE=268435456	# 256M
APPIMGSIZE=209715200	# 200M

MKE4FS=make_ext4fs
OUTDIR=./output

TOPDIR=`pwd`
KERNELDIR=kernelfs
ROOTDIR=rootfs
APPDIR=wx3030
#IMGFILE=debian-jessie-images.tgz

#[ -d ${KERNELDIR} ] || { echo "Error: ./${KERNELDIR}: not found"; exit 1; }
#[ -d ${ROOTDIR} ] || { echo "Error: ./${ROOTDIR}: not found"; exit 1; }
[ -d ${APPDIR} ] || { echo "Error: ./${APPDIR}: not found"; exit 1; }

#----------------------------------------------------------
# Execute an action
FA_DoExec() {
	echo "${@}"
	eval $@ || exit $?
}

#----------------------------------------------------------
# Make ext4 image

execute "rm -rf ./output/*.img"
mkdir -p ${OUTDIR}

#FA_DoExec ${MKE4FS} -s -l 64M -u boot -L kernel ${OUTDIR}/kernel.img ${KERNELDIR}
#FA_DoExec ${MKE4FS} -s -l 300M -u root -L rootfs ${OUTDIR}/rootfs.img ${ROOTDIR}
FA_DoExec ${MKE4FS} -s -l 100M -u app -L appfs ${OUTDIR}/appfs.img ${APPDIR}

#----------------------------------------------------------
# Create package & md5sum

#if [ ! -f ${OUTDIR}/partma.txt ]; then
#	cp config/partmap.txt ${OUTDIR}/ -avf
#fi

#FA_DoExec "(cd ${OUTDIR} && tar cvzf ${TOPDIR}/${IMGFILE} rootfs.img boot.img partmap.txt)"
#FA_DoExec "md5sum ${IMGFILE} > ${IMGFILE}.hash.md5"

#ls -l ${IMGFILE}*
echo "...done."

