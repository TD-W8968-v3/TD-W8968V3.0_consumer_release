#***********************************************************************
#
#  Copyright (c) 2006-2010  Broadcom Corporation
#  All Rights Reserved
#
#
#***********************************************************************/

#
# You probably don't need to modify anything in this Makefile.
#



#
# This is the list of directories to traverse.  
#
PRIVATE_DIRS = libs apps


#
# We need to declare these directory names as phony targets,
# otherwise make will think the directories themselves are
# the targets and will never execute the cmds.
# See GNU Make, 4.6 
#
.PHONY : $(PRIVATE_DIRS)
all    : $(PRIVATE_DIRS)


#
# Generic rule for making the dirs
#
$(PRIVATE_DIRS):
	$(MAKE) -C $@


#
# Cleaning rules
# See example in GNU Make, 4.6
#
clean: 
	for dir in $(PRIVATE_DIRS); do \
		$(MAKE) -C $$dir clean; \
	done

