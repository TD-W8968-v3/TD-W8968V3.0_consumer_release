# File: bcmdrivers/char/xtmcfg/bcm963xx/Makefile
#
# Makefile for the BCM63xx bcmxtmcfg driver.
#

obj-$(CONFIG_BCM_XTMCFG) += bcmxtmcfg.o

bcmxtmcfg-objs += bcmxtmcfg_dep.o

$(obj)/bcmxtmcfg_dep.o:
	cp $(obj)/bcmxtmcfg.o_$(PROFILE)_save $(obj)/bcmxtmcfg_dep.o

clean:
	rm -f core *.o *.a *.s

