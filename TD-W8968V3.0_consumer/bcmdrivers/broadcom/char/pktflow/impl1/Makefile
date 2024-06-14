# File: bcmdrivers/char/fcache/bcm963xx/Makefile
#
# Makefile for the BCM63xx Flow Forwarding Cache module
#

obj-$(CONFIG_BCM_PKTFLOW) += pktflow.o

pktflow-objs += pktflow_dep.o

$(obj)/pktflow_dep.o:
	cp $(obj)/pktflow.o_$(PROFILE)_save $(obj)/pktflow_dep.o

clean:
	rm -f core *.o *.a *.s

