# File: bcmdrivers/char/vlan/bcm963xx/Makefile
#
# Makefile for the BCM68xx VLAN driver.
#

obj-$(CONFIG_BCM_VLAN) += bcmvlan.o

bcmvlan-objs += bcmvlan_dep.o

$(obj)/bcmvlan_dep.o:
	cp $(obj)/bcmvlan.o_$(PROFILE)_save $(obj)/bcmvlan_dep.o


clean:
	rm -f core *.o *.a *.s .*.cmd *.ko

