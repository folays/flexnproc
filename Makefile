obj-m = flexnproc.o
KVERSION = $(shell uname -r)
EXTRA_CFLAGS += -Os -g -ggdb
#ccflags-y += -O0 -g -ggdb

flexnproc-objs := main.o

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean

re: clean all
