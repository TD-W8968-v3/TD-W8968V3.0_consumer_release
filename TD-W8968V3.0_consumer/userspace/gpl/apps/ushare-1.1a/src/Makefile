ifeq (,$(wildcard ../config.mak))
$(error "../config.mak is not present, run configure !")
endif
include ../config.mak

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common

EXTRALIBS = -lthreadutil -lpthread -lupnp -lixml -ldlna -lavformat \
			-lavcodec -lavutil -lm -lcms_msg $(CMS_COMMON_LIBS)
			
OPTFLAGS += -I. \
		-I$(BUILD_DIR)/userspace/public/include  \
		-I$(BUILD_DIR)/userspace/public/include/$(OALDIR) \
		-I$(BUILD_DIR)/userspace/gpl/apps/ushare-1.1a \
		-I$(BUILD_DIR)/userspace/gpl/apps/ffmpeg-0.6 \
		-I$(BUILD_DIR)/userspace/gpl/apps/libdlna-0.2.3/src \
		-I$(BUILD_DIR)/userspace/gpl/apps/upnp_libs/inc 
		   
 
LDFLAGS += -L$(BUILD_DIR)/userspace/gpl/apps/libdlna-0.2.3/src \
		-L$(BUILD_DIR)/userspace/gpl/apps/upnp_libs/libs \
		-L$(BUILD_DIR)/userspace/gpl/apps/ffmpeg-0.6/libavcodec \
		-L$(BUILD_DIR)/userspace/gpl/apps/ffmpeg-0.6/libavformat \
		-L$(BUILD_DIR)/userspace/gpl/apps/ffmpeg-0.6/libavutil
#
# GPL apps and libs are only allowed to link with libraries from the
# gpl and public directories.
#
# WARNING: Do not modify this section unless you understand the
# license implications of what you are doing.
#
ALLOWED_LIB_DIRS := /lib:/lib/gpl:/lib/public		
		
LDFLAGS += $(CMS_LIB_RPATH) $(CMS_LIB_PATH)		

		   

CFLAGS += $(DF_FLAGS)
PROG = ushare
CONFIG_TELNET=n

EXTRADIST = ushare.1 \
	cds.h \
	cms.h \
	msr.h \
	http.h \
	presentation.h \
	metadata.h \
	mime.h \
	services.h \
	buffer.h \
	util_iconv.h \
	content.h \
	cfgparser.h \
	trace.h \
	redblack.h \
	osdep.h \
	ushare.h \
	gettext.h \
	minmax.h \
	template_list.h \
	ushare_tplink.h \


SRCS = \
        cds.c \
        cms.c \
        msr.c \
        http.c \
        presentation.c \
        metadata.c \
        mime.c \
        services.c \
        buffer.c \
        util_iconv.c \
        content.c \
        cfgparser.c \
        trace.c \
        redblack.c \
        osdep.c \
        ushare.c \
		ushare_tplink.c


ifeq ($(CONFIG_TELNET), y)
	EXTRADIST += ctrl_telnet.h
    SRCS += ctrl_telnet.c

    CFLAGS += -DINCLUDE_TELNET
endif


OBJS = $(SRCS:.c=.o)

.SUFFIXES: .c .o

all: depend $(PROG)

.c.o:
	$(CC) -c $(CFLAGS) $(OPTFLAGS) -o $@ $<

$(PROG): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(EXTRALIBS) -o $@
	$(STRIP) $@
	@chmod 777 $@
	install -m 755 ./$@ $(INSTALL_DIR)/bin
	install -m 444 ../scripts/ushare.conf $(INSTALL_DIR)/etc
	
	

clean:
	-$(RM) -f *.o $(PROG)
	-$(RM) -f .depend

distclean:

#hanjiayan add
#install: $(PROG)
#	$(INSTALL) -d $(bindir)
#	$(INSTALL) $(PROG) $(bindir)
#	$(STRIP) $(INSTALLSTRIP) $(bindir)/$(PROG)



depend:
	$(CC) -I.. -MM $(CFLAGS) $(OPTFLAGS) $(SRCS) 1>.depend

.PHONY: clean distclean install depend

dist-all:
	cp $(EXTRADIST) $(SRCS) Makefile $(DIST)

.PHONY: dist-all

#
# include dependency files if they exist
#
ifneq ($(wildcard .depend),)
include .depend
endif
