# host tools should not use cross compiler
unexport AR AS LD CC CPP CXX NM STRIP SSTRIP OBJCOPY OBJDUMP RANLIB

.PHONY: fakeroot
all: fakeroot


TOPDIR	:= $(shell /bin/pwd)
export FAKEROOT_DIR = $(TOPDIR)/fakeroot

fakeroot:
	cd $(FAKEROOT_DIR); CFLAGS="$(CFLAGS_NOARCH)" ./configure --prefix=/usr --disable-nls
	touch $@
	cd ..
	make -C $(FAKEROOT_DIR)
	sed -e 's,^PREFIX=.*,PREFIX=$(FAKEROOT_DIR),g' -e 's,^BINDIR=.*,BINDIR=$(FAKEROOT_DIR),g' -e 's,^PATHS=.*,PATHS=$(FAKEROOT_DIR)/.libs:/lib:/usr/lib,g' $(FAKEROOT_DIR)/scripts/fakeroot > $(FAKEROOT_DIR)/fakeroot
	chmod u+x $(FAKEROOT_DIR)/fakeroot
	
clean:
	if [ -e $(FAKEROOT_DIR)/Makefile ];  then make -C $(FAKEROOT_DIR) distclean; fi
	rm -f $(FAKEROOT_DIR)/fakeroot

