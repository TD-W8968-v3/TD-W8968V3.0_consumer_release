obj-m += domain.o

domain-objs += domain_dep.o

$(obj)/domain_dep.o:
	cp $(obj)/domain.o_$(PROFILE)_save $(obj)/domain_dep.o

clean:
	rm -rf *.o *.ko *.mod.c 