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

all dynamic: install

install:
	cp -f acs_cli_$(PROFILE)_save acs_cli
	cp -f acsd_$(PROFILE)_save acsd
	cp -f eapd_$(PROFILE)_save eapd
	#cp -f epi_ttcp_$(PROFILE)_save epi_ttcp
	cp -f libnvram.so_$(PROFILE)_save libnvram.so
	cp -f libwl_server_socket.so_$(PROFILE)_save libwl_server_socket.so
	cp -f libwlbcmcrypto.so_$(PROFILE)_save libwlbcmcrypto.so
	cp -f libwlbcmshared.so_$(PROFILE)_save libwlbcmshared.so
	cp -f libwlctl.so_$(PROFILE)_save libwlctl.so
	cp -f libwlmngr.so_$(PROFILE)_save libwlmngr.so
	cp -f libwlupnp.so_$(PROFILE)_save libwlupnp.so
	cp -f libwps.so_$(PROFILE)_save libwps.so
	cp -f lld2d_$(PROFILE)_save lld2d
	cp -f nas_$(PROFILE)_save nas
	cp -f nvram_params_$(PROFILE)_save nvram_params
	cp -f nvram_$(PROFILE)_save nvram
	cp -f nvramUpdate_$(PROFILE)_save nvramUpdate
	cp -f wl_server_socket_$(PROFILE)_save wl_server_socket
	ln -sf wl_server_socket wl_server
	cp -f wlctl_$(PROFILE)_save wlctl
	cp -f wlevt_$(PROFILE)_save wlevt
	cp -f wlmngr_$(PROFILE)_save wlmngr
	cp -f wps_monitor_$(PROFILE)_save wps_monitor
	cp -f wrt54g.large.ico_$(PROFILE)_save wrt54g.large.ico
	cp -f wrt54g.small.ico_$(PROFILE)_save wrt54g.small.ico
	install -m 755 acs_cli $(INSTALL_DIR)/bin/
	install -m 755 acsd $(INSTALL_DIR)/bin/
	install -m 755 eapd $(INSTALL_DIR)/bin/
	#install -m 755 epi_ttcp $(INSTALL_DIR)/bin/
	install -m 755 libnvram.so $(INSTALL_DIR)/lib/
	install -m 755 libwl_server_socket.so $(INSTALL_DIR)/lib/
	install -m 755 libwlbcmcrypto.so $(INSTALL_DIR)/lib/
	install -m 755 libwlbcmshared.so $(INSTALL_DIR)/lib/
	install -m 755 libwlctl.so $(INSTALL_DIR)/lib/
	install -m 755 libwlmngr.so $(INSTALL_DIR)/lib/
	install -m 755 libwlupnp.so $(INSTALL_DIR)/lib/
	install -m 755 libwps.so $(INSTALL_DIR)/lib/
	install -m 755 lld2d $(INSTALL_DIR)/bin/
	install -m 755 nas $(INSTALL_DIR)/bin/
	install -m 755 nvram_params $(INSTALL_DIR)/etc/wlan/
	install -m 755 nvram $(INSTALL_DIR)/bin/
	install -m 755 nvramUpdate $(INSTALL_DIR)/bin/
	install -m 755 wl_server_socket $(INSTALL_DIR)/bin/
	ln -sf wl_server_socket $(INSTALL_DIR)/bin/wl_server
	install -m 755 wlctl $(INSTALL_DIR)/bin/
	ln -sf wlctl $(INSTALL_DIR)/bin/wl
	install -m 755 wlevt $(INSTALL_DIR)/bin/
	install -m 755 wlmngr $(INSTALL_DIR)/bin/
	install -m 755 wps_monitor $(INSTALL_DIR)/bin/
	install -m 755 wrt54g.large.ico $(INSTALL_DIR)/etc/
	install -m 755 wrt54g.small.ico $(INSTALL_DIR)/etc/
	
clean:
	@rm -f acs_cli acsd eapd epi_ttcp libnvram.so libwl_server_socket.so libwlbcmcrypto.so libwlbcmshared.so libwlctl.so libwlmngr.so libwlupnp.so lld2d nas nvram_params nvram nvramUpdate wl_server_socket wlctl wlevt wlmngr wps_monitor wrt54g.large.ico wrt54g.small.ico

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common