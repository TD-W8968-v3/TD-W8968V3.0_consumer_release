#	$Id: Makefile.linux,v 1.9 1999/08/13 01:54:32 paulus Exp $
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common

EXE=chat

CDEF1=	-DTERMIOS			# Use the termios structure
CDEF2=	-DSIGTYPE=void			# Standard definition
CDEF3=	-UNO_SLEEP			# Use the usleep function
CDEF4=	-DFNDELAY=O_NDELAY		# Old name value
CDEFS=	$(CDEF1) $(CDEF2) $(CDEF3) $(CDEF4)

CFLAGS=	-O2 -g -pipe $(CDEFS)

all install:	$(EXE) generic_exe_install

$(EXE):	chat.o
	@echo "Build chat ..."
	$(CC) -o chat chat.o

chat.o:	chat.c
	$(CC) -c $(CFLAGS) -o chat.o chat.c

.PHONY : clean	
clean:
	rm -f chat.o chat *~
	rm -f $(INSTALL_DIR)/bin/$(EXE)
