VERSION = 2
PATCHLEVEL = 6
SUBLEVEL = 29
#EXTRAVERSION =

obj-m += iomonitor.o

KDIR=~/Desktop/AOSP/device/tegra
PWD := $(shell pwd)

default:
	make -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	make -C $(KDIR) SUBDIRS=$(PWD) clean
