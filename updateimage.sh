#!/bin/bash

HOST=`uname -s`
if [ "${HOST}" = "Darwin" ]
then
 hdiutil attach grub.img
 cp kernel.x86 /Volumes/Untitled/kernel.x86
 hdiutil detach /dev/disk2
else
 mount -o loop /mnt/grub.img /mnt/floppy
 cp kernel.x86 /mnt/floppy/kernel
 umount /mnt/floppy
fi



