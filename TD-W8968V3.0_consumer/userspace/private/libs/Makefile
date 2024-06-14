#***********************************************************************
#
#  Copyright (c) 2006  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/


#
# You do not need to modify this Makefile to add an app.
# Simply create a .autodetect in the subdir and this Makefile will enter
# that directory during make and make clean.  (Auto detected directories
# are listed in the AUTODETECT_SUBDIRS variable.  Type make show_subdirs)
#
HARDCODED_SUBDIRS := mdm cms_core cms_dal cms_cli atmctl xdslctl bcmssl \
           omci mocactl \
           vlanctl gponif ethswctl cmfapi spuctl pwrctl omcipm_drv gponctl \
           nanoxml fcctl mdk212 eponsdk iqctl eponctl snoopctl gmacctl fapctl
SUBDIRS = $(HARDCODED_SUBDIRS) $(AUTODETECT_SUBDIRS) $(MODSW_SUBDIRS)


# === this is the default target ===
# Need to add a level of indirection here so that AUTODETECT_SUBDIRS and
# MODSW_SUBDIRS can be populated in make.common/make.modsw before we build.
#
all : make_all_subdirs



#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

# automatically set BCM_BASE_BUILD_DIR so that the modsw dirs can compile
BCM_BASE_BUILD_DIR := $(BUILD_DIR)
export BCM_BASE_BUILD_DIR


include $(BUILD_DIR)/make.common

ifneq ($(strip $(BUILD_VODSL)),)
include $(BUILD_DIR)/make.voice
endif

export BUILD_DIR


#
# We need to declare these directory names as phony targets,
# otherwise make will think the directories themselves are
# the targets and will never execute the cmds.
# See GNU Make, 4.6 
#
.PHONY : $(SUBDIRS)
make_all_subdirs : $(SUBDIRS)


clean: generic_subdir_clean

modsw_clean: generic_modsw_base_clean


##############################################################
# 
# Rules for inidividual libs
# libs can be compiled or not depending on the profile (which
# is modified via make menuconfig)
#
##############################################################


$(AUTODETECT_SUBDIRS) $(MODSW_SUBDIRS): 
	$(MAKE) -C $@


# always build this lib
mdm:
	$(MAKE) -C $@


# always build this lib
#modify for creating tp_product.h by wwj 130224
cms_core: tp_product
	$(MAKE) -C $@

# always build this lib
cms_dal:
	$(MAKE) -C $@

# always build this lib
nanoxml:
	$(MAKE) -C $@


# Always build this lib, but if CLI_CMD and CLI_MENU is not defined
# it will be a very small lib (should be a zero length lib)
cms_cli:
	$(MAKE) -C $@


ifneq ($(strip $(BUILD_XTMCTL)),)
atmctl:
	$(MAKE) -C atmctl $(BUILD_XTMCTL)
else
atmctl:
	@echo "skipping atmctl (not configured)"
endif


ifneq ($(strip $(BUILD_XDSLCTL)),)
xdslctl:
	$(MAKE) -C xdslctl $(BUILD_XDSLCTL)
else
xdslctl:
	@echo "skipping xdslctl (not configured)"
endif


ifneq ($(strip $(BUILD_TR69C_BCM_SSL)),)
bcmssl: sanity_check
	(tar -xkjf bcmssl.tar.bz2 2> /dev/null || true)
	$(MAKE) -C bcmssl
else
bcmssl: sanity_check
	@echo "skipping bcmssl (not configured)"
endif

ifneq ($(strip $(BUILD_OMCI)),)
omci: omcipm_drv
	$(MAKE) -C $@
omcipm_drv:
	$(MAKE) -C $@
else
omci:
	@echo "skipping omci (not configured)"
endif

ifneq ($(strip $(BUILD_GPONCTL)),)
gponctl:
	$(MAKE) -C $@
else
gponctl:
	@echo "skipping gponctl (not configured)"
endif

ifneq ($(strip $(BUILD_MoCACTL)),)
mocactl:
	$(MAKE) -C $@
else
mocactl:
	@echo "skipping mocactl (not configured)"
endif

ifneq ($(strip $(BUILD_VLANCTL)),)
vlanctl: 
	$(MAKE) -C $@
else
vlanctl:
	@echo "skipping vlanctl (not configured)"
endif

ifneq ($(strip $(BUILD_GPONIF)),)
gponif: 
	$(MAKE) -C $@
else
gponif:
	@echo "skipping gponif (not configured)"
endif

ifneq ($(strip $(BUILD_ETHSWCTL)),)
ethswctl: 
	$(MAKE) -C $@
else
ethswctl:
	@echo "skipping ethswctl (not configured)"
endif

ifneq ($(strip $(BUILD_CMFAPI)),)
cmfapi: 
	$(MAKE) -C $@
else
cmfapi:
	@echo "skipping cmfapi (not configured)"
endif

ifneq ($(strip $(BUILD_SPUCTL)),)
spuctl:
	$(MAKE) -C spuctl $(BUILD_SPUCTL)
else
spuctl:
	@echo "skipping spuctl (not configured)"
endif

ifneq ($(strip $(BUILD_PWRCTL)),)
pwrctl:
	$(MAKE) -C pwrctl $(BUILD_PWRCTL)
else
pwrctl:
	@echo "skipping pwrctl (not configured)"
endif

ifneq ($(strip $(BUILD_FCCTL)),)
fcctl: 
	$(MAKE) -C $@
else
fcctl:
	@echo "skipping fcctl (not configured)"
endif

ifneq ($(strip $(BUILD_EPON_SDK)),)
eponsdk: mdk212
	$(MAKE) -C $@
else
eponsdk:
	@echo "Skipping epon sdk "
endif

# Switch MDK
mdk212: sanity_check
ifneq ($(strip $(BUILD_SWMDK)),)
	if [ ! -e mdk212/RELEASE ]; then \
         echo untaring original mdk; \
	  (tar -xkfj mdk212/$(ORIGINAL_MDK) 2> /dev/null || true); \
	fi
	$(MAKE) -j 1 -C $@
else
	@echo "Skipping switch mdk "
endif

# Ingress QoS
ifneq ($(strip $(BUILD_IQCTL)),)
iqctl: 
	$(MAKE) -C $@
else
iqctl:
	@echo "skipping iqctl (not configured)"
endif

# EPONCTL Library
ifneq ($(strip $(BUILD_EPONCTL)),)
eponctl: 
	$(MAKE) -C $@
else
eponctl:
	@echo "skipping eponctl (not configured)"
endif

# snoopctl Library
ifneq ($(strip $(BUILD_MCAST_SNOOP)),)
snoopctl: 
	echo Doing Make
	$(MAKE) -C snoopctl
else
snoopctl:
	@echo "skipping snoopctl (not configured)"
endif

# GMAC
ifneq ($(strip $(BUILD_GMACCTL)),)
gmacctl: 
	$(MAKE) -C $@
else
gmacctl:
	@echo "skipping gmacctl (not configured)"
endif

# FAP
ifneq ($(strip $(BUILD_FAPCTL)),)
fapctl: 
	$(MAKE) -C $@
else
fapctl:
	@echo "skipping fapctl (not configured)"
endif

#added for creating tp_product.h by wwj 130224
TP_PRODUCT_FILE := ../include/tp_product.h
tp_product:
	@echo "Creating TP-LINK header file (tp_product.h)"
	@echo "/* this file is automatically generated, yuanshang */" > $(TP_PRODUCT_FILE)
	@echo "#ifndef __TP_PRODUCT_H__" >> $(TP_PRODUCT_FILE)
	@echo "#define __TP_PRODUCT_H__" >> $(TP_PRODUCT_FILE)
	# TP_PRODUCT_MODEL
	@echo -n "#define PRODUCT_MODEL \"" >> $(TP_PRODUCT_FILE)
	@echo -n $(TP_PRODUCT_MODEL)|tr "a-z" "A-Z" >> $(TP_PRODUCT_FILE)
	@echo "\"" >> $(TP_PRODUCT_FILE)
	# TP_PRODUCT_ID & VER
	@echo "#define PRODUCT_ID $(TP_PRODUCT_ID)" >> $(TP_PRODUCT_FILE)
	@echo "#define PRODUCT_VER $(TP_PRODUCT_VER)" >> $(TP_PRODUCT_FILE)
	# TP_SW_VER
	@echo -n "#define SW_VER \"" >> $(TP_PRODUCT_FILE)
	@echo -n "$(TP_SW_VER_MAJOR).$(TP_SW_VER_MINOR).$(TP_SW_VER_REV)" >> $(TP_PRODUCT_FILE)
	@echo "\"" >> $(TP_PRODUCT_FILE)
	# TP_HW_VER
	@echo -n "#define HW_VER \"TD-" >> $(TP_PRODUCT_FILE)
	@echo -n $(TP_PRODUCT_MODEL)|tr "a-z" "A-Z" >> $(TP_PRODUCT_FILE)
	@echo -n " V$(TP_PRODUCT_VER) $(TP_HARDWARE_VER)" >> $(TP_PRODUCT_FILE)
	@echo "\"" >> $(TP_PRODUCT_FILE)
	# TP_PRODUCT_DESCRIPTION
	@echo -n "#define PRODUCT_DESCRIPTION \"" >> $(TP_PRODUCT_FILE)
	@echo -n $(TP_PRODUCT_DESCRIPTION) >> $(TP_PRODUCT_FILE)
	@echo "\"" >> $(TP_PRODUCT_FILE)
	# ANNEX
	@echo -n "#define CPE_ANNEX \"" >> $(TP_PRODUCT_FILE)
	@echo -n "$(BRCM_ADSL_STANDARD)" >> $(TP_PRODUCT_FILE)
	@echo "\"" >> $(TP_PRODUCT_FILE)
	# FIRMWARE_VER_PREFIX(like "1.4.0 Build 111128 Rel.")
	@echo -n "#define FIRMWARE_VER_PREFIX \"" >> $(TP_PRODUCT_FILE)
	@echo -n "$(TP_SW_VER_MAJOR).$(TP_SW_VER_MINOR).$(TP_SW_VER_REV) Build " >> $(TP_PRODUCT_FILE)
	@echo -n `date +%y%m%d` >> $(TP_PRODUCT_FILE)
	@echo -n " Rel." >> $(TP_PRODUCT_FILE)
	@echo "\"" >> $(TP_PRODUCT_FILE)
	@echo "#define BUILD_DATE \"`date +%y` `date +%m` `date +%d`\"" >> $(TP_PRODUCT_FILE)
	@echo "#define BUILD_TIME \"`date +%H` `date +%M` `date +%S`\"" >> $(TP_PRODUCT_FILE)

	@echo "#endif /* __TP_PRODUCT_H__ */" >> $(TP_PRODUCT_FILE)

