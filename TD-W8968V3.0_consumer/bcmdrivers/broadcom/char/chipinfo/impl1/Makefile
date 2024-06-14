# Makefile for the bcm963xx board drivers
#


obj-$(CONFIG_BCM_CHIPINFO) += chipinfo.o 
chipinfo-objs += chipinfo_dep.o

$(obj)/chipinfo_dep.o:
	cp $(obj)/chipinfo.o_$(PROFILE)_save $(obj)/chipinfo_dep.o

clean:
	rm -f core *.ko *.o *.a *.s

