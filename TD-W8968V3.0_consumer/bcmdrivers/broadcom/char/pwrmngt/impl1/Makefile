# File: bcmdrivers/char/pwrmngt/bcm963xx/Makefile
#
# Makefile for the Power Management Module
#
obj-$(CONFIG_BCM_PWRMNGT) += pwrmngtd.o

pwrmngtd-objs += pwrmngtd_dep.o

$(obj)/pwrmngtd_dep.o:
	cp $(obj)/pwrmngtd.o_$(PROFILE)_save $(obj)/pwrmngtd_dep.o

clean:
	rm -f core *.o *.a *.s

