
#losetup /dev/loop7  image_360
#卸载loop7设备
#losetup -d /dev/loop7                  
make clean;make
umount /mnt
rmmod minix.ko
dmesg -c
insmod minix.ko
mount /dev/loop7 /mnt
