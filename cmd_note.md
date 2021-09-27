sudo mount -o loop ./disk.raw ./img
sudo umount /dev/loop0



qemu-system-x86_64 -m 512M -smp 4 -kernel ./arch/x86_64/boot/bzImage -drive format=raw,file=./disk.raw -append "init=/linuxrc root=/dev/sda nokaslr"
