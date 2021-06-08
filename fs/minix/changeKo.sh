
#losetup /dev/loop7  image_360
make clean;make
umount /mnt
rmmod minix.ko
dmesg -c
insmod minix.ko
mount /dev/loop7 /mnt
