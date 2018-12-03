KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := servo180_dev_2.o

PWD := $(shell pwd)

default :
	make -C $(KERNELDIR) M=$(PWD) modules

clean : 
	make -C $(KERNELDIR) M=$(PWD) clean
