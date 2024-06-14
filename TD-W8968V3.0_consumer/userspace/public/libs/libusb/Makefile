CURR_DIR := $(shell pwd)

all : libusb libusb-compat

.PHONY:libusb
libusb:
	@echo "Building libusb-1.0.8 ..."
	@if [ ! -e ./libusb-1.0.8/Makefile ];then \
	cd ./libusb-1.0.8; ./configure --disable-shared --build=i686-linux --host=mips-linux-uclibc --prefix=$(CURR_DIR)/libusb-1.0.8/install; \
	fi
	$(MAKE) -C ./libusb-1.0.8 install

.PHONY:libusb-compat
libusb-compat:
	@echo "Building usb compat ..."
	@if [ ! -e ./libusb-compat-0.1.3/Makefile ];then \
	cd ./libusb-compat-0.1.3; ./configure --disable-shared --build=i686-linux --host=mips-linux-uclibc --prefix=$(CURR_DIR)/libusb-compat-0.1.3/install LIBUSB_1_0_CFLAGS=-I$(USERSPACE_PUBLIC_LIBS_DIR)/libusb/libusb-1.0.8/install/include/libusb-1.0 LIBUSB_1_0_LIBS=\ -Bstatic\ -L$(USERSPACE_PUBLIC_LIBS_DIR)/libusb/libusb-1.0.8/install/lib\ -lusb-1.0; \
	fi
	$(MAKE) -C ./libusb-compat-0.1.3 install
	
.PHONY:bcm_install
bcm_install:
	@echo "install usb libs ..."
#	mkdir -p $(INSTALL_DIR)/lib/public
#	cp --no-dereference -p ./libusb-compat-0.1.3/install/lib/libusb.so $(INSTALL_DIR)/lib/public
#	cp --no-dereference -p ./libusb-1.0.8/install/lib/libusb-1.0.so* $(INSTALL_DIR)/lib/public
#	cp --no-dereference -p ./libusb-compat-0.1.3/install/lib/libusb-0.1.so* $(INSTALL_DIR)/lib/public

.PHONY:clean
clean:
#	cd $(INSTALL_DIR)/lib/public && rm -f libusb*
	cd ./libusb-1.0.8 && $(MAKE) clean && rm -rf ./install/* Makefile
	cd ./libusb-compat-0.1.3 && $(MAKE) clean && rm -rf ./install/* Makefile
