# File: bcmdrivers/char/arl/bcm963xx/Makefile
#
# Makefile for the BCM68xx ARL driver.
#

obj-$(CONFIG_BCM_ARL) += bcmarl.o

bcmarl-objs += bcmarl_dep.o

$(obj)/bcmarl_dep.o:
	cp $(obj)/bcmarl.o_$(PROFILE)_save $(obj)/bcmarl_dep.o

clean:
	rm -f core *.ko *.o *.a *.s

