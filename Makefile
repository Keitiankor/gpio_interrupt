MY_TARGET := gpio_interrupt.ko
obj-m := gpio_interrupt.o

KERNEL_DIR := /lib/modules/$(shell uname -r)/build
MODULE_DIR := /lib/modules/$(shell uname -r)/kernel/gpio_interrupt.c
PWD := $(shell pwd)

default :
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
install :
	mkdir -p $ $(MODULE_DIR)
	cp -f $(MY_TARGET) $(DODULE_DIR)
clean :
	$(MAKE) -C $(KERNEL_DIR_ SUBDIRS=$(PWD) clean
