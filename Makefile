obj-m += chardev.o

KERNEL_VERS=$(shell uname -r)
KERNEL_BUILDDIR=/lib/modules/$(KERNEL_VERS)/build
ROOT_PATH=$(shell pwd)

modules:
	make -C $(KERNEL_BUILDDIR) M=$(ROOT_PATH) modules

install:
	make -C $(KERNEL_BUILDDIR) M=$(ROOT_PATH) install

clean:
	make -C $(KERNEL_BUILDDIR) M=$(ROOT_PATH) clean
