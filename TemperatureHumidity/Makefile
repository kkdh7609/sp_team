KERNELDIR = /lib/modules/$(shell uname -r)build

obj-m := Kthseonser.o

PWD := $(shell pwd)

default :
	$(MAKE) -c $(KERNELDIR) M=$(PWD) modules

clean :
	$(MAKE) -c $(KERNELDIR) M=$(PWD) clean
