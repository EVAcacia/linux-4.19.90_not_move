#
# Makefile for the Linux minix filesystem routines.
# Linux minix文件系统例程的Makefile。
#




#obj-$(CONFIG_MINIX_FS) += minix.o
obj-m += minix.o
minix-objs := bitmap.o itree_v1.o itree_v2.o namei.o inode.o file.o dir.o

#obj-m := minix.o


drive:
#make -C /mnt/d/work/linux-4.19.90_not_move/ M=$(PWD) modules
	make -C  /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
#make -C /mnt/d/work/linux-4.19.90_not_move/ M=$(PWD) clean
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean