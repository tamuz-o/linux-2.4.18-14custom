#!/bin/bash
tar -xvf my/upload.tar
cd /usr/src/linux-2.4.18-14custom &&
make bzImage  &&
cd /usr/src/linux-2.4.18-14custom/arch/i386/boot &&
yes | cp bzImage /boot/vmlinuz-2.4.18-14custom &&
reboot
