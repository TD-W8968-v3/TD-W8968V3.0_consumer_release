#***********************************************************************
#
#  Copyright (c) 2006  Broadcom Corporation
#  All Rights Reserved
#
# 
# 
# This program is the proprietary software of Broadcom Corporation and/or its 
# licensors, and may only be used, duplicated, modified or distributed pursuant 
# to the terms and conditions of a separate, written license agreement executed 
# between you and Broadcom (an "Authorized License").  Except as set forth in 
# an Authorized License, Broadcom grants no license (express or implied), right 
# to use, or waiver of any kind with respect to the Software, and Broadcom 
# expressly reserves all rights in and to the Software and all intellectual 
# property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE 
# NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY 
# BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE. 
# 
# Except as expressly set forth in the Authorized License, 
# 
# 1. This program, including its structure, sequence and organization, 
#    constitutes the valuable trade secrets of Broadcom, and you shall use 
#    all reasonable efforts to protect the confidentiality thereof, and to 
#    use this information only in connection with your use of Broadcom 
#    integrated circuit products. 
# 
# 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
#    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR 
#    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH 
#    RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND 
#    ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, 
#    FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR 
#    COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE 
#    TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR 
#    PERFORMANCE OF THE SOFTWARE. 
# 
# 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR 
#    ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, 
#    INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY 
#    WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN 
#    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; 
#    OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE 
#    SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS 
#    SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY 
#    LIMITED REMEDY. 
#
#
#***********************************************************************/

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
#
MDK := $(shell pwd)
CDK = $(MDK)/cdk
BMD = $(MDK)/bmd
PHY = $(MDK)/phy
MDK_BLD_DIR = $(MDK)/examples/linux-user

.PHONY: all cdk bmd phy_bld clean cdk_clean bmd_clean phy_clean

all:
	cd $(MDK_BLD_DIR)
	$(MAKE) -C $(MDK_BLD_DIR)

cdk:
	cd $(MDK_BLD_DIR)
	$(MAKE) -C $(MDK_BLD_DIR) cdk

bmd:
	cd $(MDK_BLD_DIR)
	$(MAKE) -C $(MDK_BLD_DIR) bmd

phy_bld:
	cd $(MDK_BLD_DIR)
	$(MAKE) -C $(MDK_BLD_DIR) phy


clean:
	$(MAKE) -C $(MDK_BLD_DIR) clean

cdk_clean:
	$(MAKE) -C $(MDK_BLD_DIR) cdk_clean

bmd_clean:
	$(MAKE) -C $(MDK_BLD_DIR) bmd_clean

phy_clean:
	$(MAKE) -C $(MDK_BLD_DIR) phy_clean


#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

#
# Private apps and libs are allowed to include header files from the
# private and public directories.
#
# WARNING: Do not modify this section unless you understand the
# license implications of what you are doing.
#
 
#
# Private apps and libs are allowed to link with libraries from the
# private and public directories.
#
# WARNING: Do not modify this section unless you understand the
# license implications of what you are doing.
#

include $(BUILD_DIR)/make.common

ALLOWED_INCLUDE_PATHS := -I.\
                         -I$(BUILD_DIR)/kernel/linux/include \
                         -I$(BUILD_DIR)/userspace/private/include  \
                         -I$(BUILD_DIR)/userspace/private/include/$(OALDIR) \
                         -I$(BUILD_DIR)/userspace/public/include  \
                         -I$(BUILD_DIR)/userspace/public/include/$(OALDIR) \
                         -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)  \
                         -I$(INC_BRCMDRIVER_PRIV_PATH)/$(BRCM_BOARD) \
                         -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)

ALLOWED_LIB_DIRS := /lib:/lib/private:/lib/public

ifndef CROSS_COMPILE
ifndef TOOLCHAIN_TOP
TOOLCHAIN_TOP = /opt/toolchains/uclibc-crosstools-gcc-4.2.3-3
endif
ifndef TOOLCHAIN
TOOLCHAIN = $(TOOLCHAIN_TOP)/usr
endif
CROSS_COMPILE = $(TOOLCHAIN)/bin/mips-linux-uclibc-
endif

LINUX_SHARED_LIBRARY=1

ifneq ($(strip $(BUILD_BRCM_CMS)),)
COMPILE_FLAGS = -DBRCM_CMS_BUILD
endif

ifeq ($(BCM_GPON_802_1Q_ENABLED),y)
COMPILE_FLAGS += -DCONFIG_BCM_GPON_802_1Q_ENABLED
endif

ifneq ($(strip $(BCM_ETH_JUMBO_FRAME)),)
COMPILE_FLAGS += -DCONFIG_BCM_ETH_JUMBO_FRAME
endif

ifneq ($(strip $(BUILD_MCAST_SNOOP)),)
COMPILE_FLAGS += -DCONFIG_BCM_MCAST_SNOOP
endif

ifneq ($(strip $(BRCM_SWITCH_SCHED_SP)),)
COMPILE_FLAGS += -DBCM_SWITCH_SCHED_SP
endif
ifneq ($(strip $(BRCM_SWITCH_SCHED_WRR)),)
COMPILE_FLAGS += -DBCM_SWITCH_SCHED_WRR
endif

ifneq ($(strip $(BRCM_DRIVER_MoCA)),)
ifeq ($(strip $(BRCM_DRIVER_GPON)),)
COMPILE_FLAGS += -DBCM_MOCA_NO_GPON
endif
endif

COMPILE_FLAGS += -DCONFIG_EPON_NUM_FE_PORTS=$(EPON_NUM_FE_PORTS)
COMPILE_FLAGS += -DCONFIG_EPON_PORT=$(EPON_PORT)

ifneq ($(strip $(BUILD_EPON_SDK)),)
COMPILE_FLAGS += -DEPON_SDK_BUILD
endif

ifneq ($(strip $(BRCM_6362_PORTS_INT_EXT_SW)),)
COMPILE_FLAGS += -DBRCM_6362_PORTS_INT_EXT_SW
endif

COMPILE_FLAGS +=  -DCHIP_$(BRCM_CHIP)

ifneq ($(strip $(EPON_ONU_TYPE)),)
COMPILE_FLAGS +=   -D$(EPON_ONU_TYPE)
endif

export CDK BMD PHY CROSS_COMPILE ALLOWED_INCLUDE_PATHS ALLOWED_LIB_DIRS CMS_LIB_PATH 
export BUILD_DIR CMS_LIB_RPATH CMS_LOG_FLAGS COMPILE_FLAGS LINUX_SHARED_LIBRARY

