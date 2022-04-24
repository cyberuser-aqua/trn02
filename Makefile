# modules
obj-m := khello.o
RPI-KENL = /usr/src/rpi-kernel
MOUDULE_FLAGS = -C $(RPI-KENL)  M=$(PWD) ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-

# files
CC=arm-linux-gnueabihf-gcc
CFLAGS=-o ./files -g

.PHONY: files

module:
	make $(MOUDULE_FLAGS) modules
clean:
	make $(MOUDULE_FLAGS) clean
load:
	insmod ./khello.ko
unload:
	rmmod ./khello.ko
files:
	$(CC) $(CFLAGS) files.c
