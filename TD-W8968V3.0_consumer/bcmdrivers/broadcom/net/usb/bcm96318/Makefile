# File: bcmdrivers/broadcom/net/usb/Makefile
#
# Makefile for the BCM63xx USB driver
#

obj-$(CONFIG_BCM_USB) += bcm_usb.o
bcm_usb-objs += bcm_usb_dep.o

$(obj)/bcm_usb_dep.o:
	cp $(obj)/bcm_usb.o_$(PROFILE)_save $(obj)/bcm_usb_dep.o

clean:
	rm -f core *.ko *.o *.a *.s
