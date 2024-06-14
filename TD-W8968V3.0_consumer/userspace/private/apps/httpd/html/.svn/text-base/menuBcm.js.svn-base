//Note: The options array initialized in menu.html follows the
//      MENU_OPTION order defined here. Both sides must be in the same order.
var i = 0;
var MENU_OPTION_USER              = i++;
var MENU_OPTION_WANACCESS         = i++;
var MENU_OPTION_SYSTEM_MODE	  = i++;			// added by pj for system mode selection
var MENU_OPTION_3G_SETTINGS		  = i++;			// added by pj for usb 3g settings
var MENU_OPTION_STANDARD          = i++;
var MENU_OPTION_PROTOCOL          = i++;
var MENU_OPTION_FIREWALL          = i++;
var MENU_OPTION_NAT               = i++;
var MENU_OPTION_IP_EXTENSION      = i++;
var MENU_OPTION_WIRELESS          = i++;
var MENU_OPTION_VOICE             = i++;
var MENU_OPTION_SNMP              = i++;
var MENU_OPTION_UPNP              = i++;
var MENU_OPTION_DDNSD             = i++;
var MENU_OPTION_SNTP              = i++;
var MENU_OPTION_EBTABLES          = i++;
var MENU_OPTION_BRIDGE            = i++;
var MENU_OPTION_TOD               = i++;
var MENU_OPTION_SIPROXD           = i++;
var MENU_OPTION_DHCPEN            = i++;
var MENU_OPTION_QOS               = i++;
var MENU_OPTION_PORTMAP           = i++;
var MENU_OPTION_IPP               = i++;
var MENU_OPTION_WIRELESS_SES      = i++;
var MENU_OPTION_RIP               = i++;
var MENU_OPTION_IPSEC             = i++;
var MENU_OPTION_CERT              = i++;
var MENU_OPTION_WL_QOS            = i++;
var MENU_OPTION_TR69C             = i++;
var MENU_OPTION_VDSL              = i++;
var MENU_OPTION_URLFILTER         = i++;
var MENU_OPTION_IPV6_SUPPORT      = i++;
var MENU_OPTION_IPV6_ENABLE       = i++;
var MENU_OPTION_DNSPROXY          = i++;
var MENU_OPTION_POLICY_ROUTING    = i++;
var MENU_OPTION_OMCI              = i++;
var MENU_OPTION_CHIPID            = i++;
var MENU_OPTION_WIRELESS_NUM_ADAPTOR = i++;
var MENU_OPTION_DIAG_P8021AG      = i++;
var MENU_OPTION_ETHWAN            = i++;
var MENU_OPTION_PTMWAN            = i++;
var MENU_OPTION_DHCP_RELAY        = i++;
var MENU_OPTION_PWRMNGT           = i++;
var MENU_OPTION_VOICE_NTR         = i++;
var MENU_OPTION_ATMWAN            = i++;
var MENU_OPTION_USB_3G		= i++;		//13/9/9 added by pj for usb 3g
var MENU_OPTION_MOCAWAN           = i++;
var MENU_OPTION_VOICE_DECT        = i++;
var MENU_OPTION_DSL_BONDING       = i++;
var MENU_OPTION_MULTICAST         = i++;
var MENU_OPTION_L2TP              = i++;
var MENU_OPTION_PPTP              = i++;
var MENU_OPTION_STORAGESERVICE    = i++;
var MENU_OPTION_SUPPORT_MOCA      = i++;
var MENU_OPTION_STANDBY           = i++;
var MENU_OPTION_DLNA              = i++;
var MENU_OPTION_WIRELESS_WAPI_AS  = i++;
var MENU_OPTION_AUTODETECTION     = i++;
var MENU_OPTION_GPONWAN           = i++;
var MENU_OPTION_POLICE_ENABLE     = i++;
var MENU_OPTION_OSGI_JVM          = i++;
var MENU_OPTION_EPONWAN           = i++;
var MENU_OPTION_SAMBA             = i++;
var MENU_OPTION_BMU               = i++;
var MENU_OPTION_DEBUG_PORT        = i++;
var MENU_OPTION_IPTV              = i++;

var wlItemsCgiCmd = new Array(
	'wlswitchinterface0.wl',
	'wlswitchinterface1.wl',
	'wlswitchinterface2.wl',
	'wlswitchinterface3.wl'
);
var wlmenuTitle = new Array(
	'wl0',
	'wl1',
	'wl2',
	'wl3'
);

function menuAdmin(options) {
	var localAccess = options[MENU_OPTION_WANACCESS] != '1';
	var sysmode = options[MENU_OPTION_SYSTEM_MODE];		// added by pj for system mode selection
	var usb3gSetting = options[MENU_OPTION_3G_SETTINGS];	// added by pj for usb 3g settings
	var std = options[MENU_OPTION_STANDARD];
	var proto = options[MENU_OPTION_PROTOCOL];
	var firewall = options[MENU_OPTION_FIREWALL];
	var ipExt = options[MENU_OPTION_IP_EXTENSION];
	var wireless = options[MENU_OPTION_WIRELESS];
	var voice = options[MENU_OPTION_VOICE];
	var snmp = options[MENU_OPTION_SNMP];
	var ddnsd = options[MENU_OPTION_DDNSD];
	var sntp = options[MENU_OPTION_SNTP];
	var ebtables = options[MENU_OPTION_EBTABLES];
	var bridge = options[MENU_OPTION_BRIDGE];
	var tod = options[MENU_OPTION_TOD];
	var QosEnabled = options[MENU_OPTION_QOS];
	var vlanconfig = options[MENU_OPTION_PORTMAP];
	var ipp = options[MENU_OPTION_IPP];
	var dlna = options[MENU_OPTION_DLNA];
	var wireless_ses = options[MENU_OPTION_WIRELESS_SES];
	var rip = options[MENU_OPTION_RIP];
	var ipsec = options[MENU_OPTION_IPSEC];
	var certificate = options[MENU_OPTION_CERT];
	var wlqos = options[MENU_OPTION_WL_QOS];
	var tr69c = options[MENU_OPTION_TR69C];
	var ipv6Support = options[MENU_OPTION_IPV6_SUPPORT];
	var ipv6Enable = options[MENU_OPTION_IPV6_ENABLE];
	var upnp = options[MENU_OPTION_UPNP];
	var urlfilter = options[MENU_OPTION_URLFILTER];
	var dnsproxy = options[MENU_OPTION_DNSPROXY];
	var pr = options[MENU_OPTION_POLICY_ROUTING];
	var omci = options[MENU_OPTION_OMCI];
	var chipId = options[MENU_OPTION_CHIPID];
	var numWl = options[MENU_OPTION_WIRELESS_NUM_ADAPTOR];
	var p8021ag = options[MENU_OPTION_DIAG_P8021AG];
	var ethwan = options[MENU_OPTION_ETHWAN];
	var ptm = options[MENU_OPTION_PTMWAN];
//	var pwrmngt = options[MENU_OPTION_PWRMNGT];
	var pwrmngt = 0; // do not display this page
	var standby = options[MENU_OPTION_STANDBY];
	var voiceNtr = options[MENU_OPTION_VOICE_NTR];
	var atm = options[MENU_OPTION_ATMWAN];
	var usb3g = options[MENU_OPTION_USB_3G];		// 13/9/9 added by pj for usb 3G
	var mocawan = options[MENU_OPTION_MOCAWAN];
	var gponwan = options[MENU_OPTION_GPONWAN];
	var eponwan = options[MENU_OPTION_EPONWAN];
	var dect = options[MENU_OPTION_VOICE_DECT];
	var dslbonding = options[MENU_OPTION_DSL_BONDING];
	var multicast = options[MENU_OPTION_MULTICAST];
	var l2tp = options[MENU_OPTION_L2TP];
	var pptp = options[MENU_OPTION_PPTP];
	var storageservice = options[MENU_OPTION_STORAGESERVICE];
	var sambaservice = options[MENU_OPTION_SAMBA];
	var tpUsbService = sambaservice; /* reserved */
	var mocaCfg = options[MENU_OPTION_SUPPORT_MOCA];
	var wireless_wapi = options[MENU_OPTION_WIRELESS_WAPI_AS];
	var autoDetection = options[MENU_OPTION_AUTODETECTION];
	var policeEnable = options[MENU_OPTION_POLICE_ENABLE];
	var engDebug = options[MENU_OPTION_DEBUG_PORT]; /* test only */
        var iptv = options[MENU_OPTION_IPTV];
	var isDsl = 0;
	// var osgi_jvm = options[MENU_OPTION_OSGI_JVM];
	var bmu = options[MENU_OPTION_BMU];
	var arr, i;

	insDoc(foldersTree, gLnk('R', getMenuTitle(MENU_QUICK_SETUP), 'qsstart.html'));
	if(sysmode == '1') insDoc(foldersTree, gLnk('R', getMenuTitle(MENU_SYSTEM_MODE), 'sysmode.html'));		// added by pj for system mode selection
	// Configure advanced setup/layer 2 interface
	arr = new Array();
	if (atm == '1') {
		isDsl = 1;
		arr.push(getMenuTitle(MENU_DSL_ATM_INTERFACE), 'dslatm.cmd');
	}
	if (ptm == '1') {
		isDsl = 1;
		arr.push(getMenuTitle(MENU_DSL_PTM_INTERFACE), 'dslptm.cmd');
	}
	if (gponwan == '1' ) arr.push(getMenuTitle(MENU_GPONWAN_INTERFACE), 'gponwan.cmd');
	if (eponwan == '1' ) arr.push(getMenuTitle(MENU_EPONWAN_INTERFACE), 'eponwan.cmd');
	if (ethwan == '1' )  arr.push(getMenuTitle(MENU_ETH_INTERFACE), 'ethwan.cmd');
	if (usb3g == '1') arr.push(getMenuTitle(MENU_USB_3G_INTERFACE), 'usb3g.cmd?action=view');	// 2013/9/9 added by pj for usb 3g
	if (mocawan == '1' ) arr.push(getMenuTitle(MENU_MOCA_INTERFACE), 'mocawan.cmd');
	var nodeAdvancedSetup;
	if (arr.length > 0) {
		nodeAdvancedSetup = insFld(foldersTree, gFld(getMenuTitle(MENU_ADVANCED_SETUP), arr[1]));
		var nodeLayer2Inteface = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_LAYER2_INTERFACE), arr[1]));
		for (i = 0; i < arr.length; i += 2) {
			insDoc(nodeLayer2Inteface, gLnk('R', arr[i], arr[i+1]));
		}
	} else {
		nodeAdvancedSetup = insFld(foldersTree, gFld(getMenuTitle(MENU_ADVANCED_SETUP), 'wancfg.cmd'));
	}
	// modified by pj for usb 3g
	if(ethwan == '1' || atm == '1')insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_WAN), 'wancfg.cmd'));
	if(usb3gSetting == '1')insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_3G_SETTINGS), 'usb3g.cmd?action=viewcfg'));	// added by pj for usb 3g settings

	arr = new Array();
	if (l2tp == '1') arr.push(getMenuTitle(MENU_VPN_L2TPAC), 'l2tpacwan.cmd');
	if (pptp == '1') arr.push(getMenuTitle(MENU_VPN_PPTPAC), 'pptpacwan.cmd');
	if (arr.length > 0) {
		var nodeVPN = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_VPN), arr[1]));
		for (i = 0; i < arr.length; i += 2) {
			insDoc(nodeVPN, gLnk('R', arr[i], arr[i+1]));
		}
	}

	insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_MACCLONE), 'macClone.cmd?action=view'));

	if ( ipv6Enable == '1' ) {
		var nodeLAN = insDoc(nodeAdvancedSetup, gFld(getMenuTitle(MENU_LAN),'lancfg2.html'));
		insDoc(nodeLAN, gLnk('R', getMenuTitle(MENU_LAN4),'lancfg2.html'));
		insDoc(nodeLAN, gLnk('R', getMenuTitle(MENU_LAN6),'ipv6lancfg.html'));
	}
	else {
		insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_LAN),'lancfg2.html'));
	}

	// Configure connection auto detection
	if (autoDetection == '1')
		insDoc(nodeAdvancedSetup, gFld(getMenuTitle(MENU_AUTODETECTION), 'autodetection.cmd?action=view'));

	if ( mocaCfg == '1' ) {
		insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_MOCA_CONFIGURATION),'mocacfg.html'));
	}

	// Configure security menu
	// If firewall is enabled and not in ipExt mode enable firewall menus
	// if (proto != 'Bridge' && ipExt != '1' ) {
	if ( proto != 'Not Applicable' && ipExt != '1' ) {
		// NAT menu is always displayed now
		var nodeNat = insDoc(nodeAdvancedSetup, gFld(getMenuTitle(MENU_SC_NAT), 'scvrtsrv.cmd?action=view'));
		insDoc(nodeNat, gLnk('R', getMenuTitle(MENU_SC_VIRTUAL_SERVER), 'scvrtsrv.cmd?action=view'));
		insDoc(nodeNat, gLnk('R', getMenuTitle(MENU_SC_PORT_TRIGGER), 'scprttrg.cmd?action=view'));
		insDoc(nodeNat, gLnk('R', getMenuTitle(MENU_SC_DMZ_HOST), 'scdmz.html'));
		insDoc(nodeNat, gLnk('R', getMenuTitle(MENU_ALGS), 'algcfg.cmd?action=view'));

		// Security menu is always displayed now
		var nodeFirewall = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_SC_SECURITY), 'scoutflt.cmd?action=view'));
		//nodeIpFlt = insFld(nodeFirewall, gFld(getMenuTitle(MENU_SC_IP_FILTER), 'scoutflt.cmd?action=view'));
		//insDoc(nodeIpFlt, gLnk('R', getMenuTitle(MENU_SC_OUTGOING), 'scoutflt.cmd?action=view'));
		//insDoc(nodeIpFlt, gLnk('R', getMenuTitle(MENU_SC_INCOMING), 'scinflt.cmd?action=view'));
		insFld(nodeFirewall, gFld(getMenuTitle(MENU_SC_IP_FILTER),'scoutflt.cmd?action=view'));
		insFld(nodeFirewall, gFld(getMenuTitle(MENU_MAC_FILTER),'scmacflt.cmd?action=view'));

		if ( tod == '1' ) 
		{
			var nodeParentalControl = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_PARENTAL_CNTL),'todmngr.tod?action=view'));
			insDoc(nodeParentalControl, gFld(getMenuTitle(MENU_TOD),'todmngr.tod?action=view'));
		}
		if ( urlfilter == '1' )
			insDoc(nodeParentalControl, gFld(getMenuTitle(MENU_URLFILTER),'urlfilter.cmd?action=view'));
	}

	// Configure QoS class menu
	var nodeQos = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_QOS),'qosqmgmt.html'));
	insDoc(nodeQos, gLnk('R', getMenuTitle(MENU_QOS_QUEUE), 'qosqueue.cmd?action=view'));
	if (policeEnable == '1')
		insDoc(nodeQos, gLnk('R', getMenuTitle(MENU_QOS_POLICER), 'qospolicer.cmd?action=view'));
	insDoc(nodeQos, gLnk('R', getMenuTitle(MENU_QOS_CLASS), 'qoscls.cmd?action=view'));

	//Configure TC menu
	var nodeTc = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_TC),'trafficcontrol.html'));
	insDoc(nodeTc, gLnk('R', getMenuTitle(MENU_TC_RULE), 'tcmgmt.cmd?action=view'));

	// Configure routing menu
	var nodeRouting = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_ROUTING), 'rtdefaultcfg.html'));
	insDoc(nodeRouting, gLnk('R', getMenuTitle(MENU_RT_DEFAULT_ROUTE), 'rtdefaultcfg.html'));
	insDoc(nodeRouting, gLnk('R', getMenuTitle(MENU_RT_STATIC_ROUTE),'rtroutecfg.cmd?action=viewcfg'));
	if (pr == '1' )
		insDoc(nodeRouting, gLnk('R', getMenuTitle(MENU_POLICY_ROUTING),'prmngr.cmd?action=view'));

/*	if ( (proto == 'PPPoE' && ipExt == '0') ||
		  (proto == 'PPPoA' && ipExt == '0') ||
		  (proto == 'MER') ||
		  (proto == 'IPoA') ) */{
		// configure rip
		if ( rip == '1' )
			insDoc(nodeRouting, gLnk('R', getMenuTitle(MENU_RT_RIP),'ripcfg.cmd?action=view'));
		// configure dns server
		var nodeDnsSetup = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_DNS), 'dnscfg.html'));
		insDoc(nodeDnsSetup, gLnk('R', getMenuTitle(MENU_DNS_SETUP), 'dnscfg.html'));
		// configure ddns client
		if ( ddnsd == '1' )
			insDoc(nodeDnsSetup, gLnk('R', getMenuTitle(MENU_DDNS), 'ddnsmngr.cmd'));
	}

	if (isDsl == 1)
	{
		// Configure ADSL Setting Menu based on Annex
		if ( std == 'annex_c' )
			insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_DSL), 'adslcfgc.html'));
		else if (chipId != '6368')
			insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_DSL), 'adslcfg.html'));
		else
			insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_DSL), 'xdslcfg.html'));

		if (dslbonding == '1')
			insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_DSL_BONDING), 'dslbondingcfg.html'));
	}

	// Configure upnp
	if (upnp == '1')
		insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_UPNP), 'upnpcfg.html'));

	// Configure dnsproxy
	//if (dnsproxy == '1')
		//insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_DNSPROXY), 'dnsproxycfg.html'));

	// Configure print server
	if ( ipp == '1' )
		insDoc(nodeAdvancedSetup, gFld(getMenuTitle(MENU_IPP), 'ippcfg.html'));

	// Configure dlna
/*	if ( dlna == '1' )
		insDoc(nodeAdvancedSetup, gFld(getMenuTitle(MENU_DLNA), 'dlnacfg.html'));
*/

        // Configure IPTV menu
    if (atm == '1' && iptv == '1')//modified by xieping for hiding iptv when wlr & 3G mode
	    insDoc(foldersTree, gLnk('R', getMenuTitle(MENU_IPTV), 'iptv.html')); //added by xieping for iptv

	// Configure wireless menu

	if ( parseInt(numWl) != 0 ) {
		if(numWl != '1')
			var wlanMenu = insFld(foldersTree, gFld(getMenuTitle(MENU_WIRELESS_SETTINGS), wlItemsCgiCmd[0]));
 
		for(i = 0; i < parseInt(numWl); i++)
		{
			// Configure wireless menu
			if(numWl == '1')
				var nodeWireless = insFld(foldersTree, gFld(getMenuTitle(MENU_WIRELESS_SETTINGS), wlItemsCgiCmd[0]));
			else
				var nodeWireless = insFld(wlanMenu, gFld(wlmenuTitle[i], wlItemsCgiCmd[i]));

			insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_BASIC), 'wlcfg.html'));
			insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_SECURITY), 'wlsecurity.html'));
			insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_WIRELESS_SCHEDULE), 'wlschedule.html'));//add wireless schedule menu by JiYu
			insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_MAC_FILTERING), 'wlmacflt.cmd?action=view'));
			insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_WDS), 'wlwds.cmd?action=view'));
			insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_ADVANCED), 'wlcfgadv.html'));
			//SUPPORT_SES
			if ( wireless_ses == '1' ) { 
				insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_SES), 'wlses.html'));
			}
		
			insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_STATION_LIST), 'wlstationlist.cmd'));
		}

		if ( wireless_wapi == '1' ) {
			 if (numWl == '1') {
				 insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_WAPI_AS), 'wlwapias.html'));
			 }
			 else {
				 insDoc(wlanMenu, gLnk('R', getMenuTitle(MENU_WL_WAPI_AS), 'wlwapias.html'));
			 }
		}
	}

	//configure guest network menu
    var nodeGuestNetwork = insFld(foldersTree, gFld(getMenuTitle(MENU_GUEST_NETWORK_SETTING), 'wlgstnwk.html'));
	
	insDoc(nodeGuestNetwork, gLnk('R', getMenuTitle(MENU_GUEST_NETWORK_BASIC),'wlgstnwk.html'));
	insDoc(nodeGuestNetwork, gLnk('R', getMenuTitle(MENU_GUEST_NETWORK_STATUS),'wlgststatlist.cmd'));
	
	if (tpUsbService == '1')
	{
		/* Configure USB menu */
		var nodeUsb = insFld(foldersTree, gFld(getMenuTitle(MENU_USB_SETTINGS), 'usbManage.cmd?action=view'));
		insDoc(nodeUsb, gLnk('R', getMenuTitle(MENU_USB_MANAGE), 'usbManage.cmd?action=view'));
		insDoc(nodeUsb, gLnk('R', getMenuTitle(MENU_USB_USR_ACCOUNTS), 'usbUsrAccount.cmd?action=view'));
		insDoc(nodeUsb, gLnk('R', getMenuTitle(MENU_USB_SMB_SERVER), 'usbSmbSrv.cmd?action=view'));
		insDoc(nodeUsb, gLnk('R', getMenuTitle(MENU_USB_FTP_SERVER), 'usbFtpSrv.cmd?action=view'));
		insDoc(nodeUsb, gLnk('R', getMenuTitle(MENU_USB_DLNA_SERVER), 'usbDlnaManage.cmd?action=view'));
		insDoc(nodeUsb, gLnk('R', getMenuTitle(MENU_USB_PRINT_SERVER), 'usbPrintSrv.cmd?action=view'));
	}
	
	  /*Storage Service menu */
/*	if(storageservice == '1')
	{
		var nodeStorage = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_STORAGESERVICE), 'storageservicecfg.cmd?view'));
		insDoc(nodeStorage, gLnk('R', getMenuTitle(MENU_STORAGE_INFO), 'storageservicecfg.cmd?view'));
		if(sambaservice == '1'){
			insDoc(nodeStorage, gLnk('R', getMenuTitle(MENU_STORAGE_USERACCOUNT), 'storageuseraccountcfg.cmd?view'));
		}
	}
*/
	// Configure voice menu
	if ( voice == 'MGCP' ) {
		var nodeVoice = insFld(foldersTree, gFld(getMenuTitle(MENU_VOICE_SETTINGS), 'voicemgcp_basic.html'));
		insDoc(nodeVoice, gLnk('R', getMenuTitle(MENU_VOICE_MGCP), 'voicemgcp_basic.html'));
		if( voiceNtr != '2' ) {
			insDoc(nodeVoice, gLnk('R', getMenuTitle(MENU_VOICE_NTR), 'voicentr.html'));
		}
	}
	else if ( voice == 'SIP' ) {
		var nodeVoice = insFld(foldersTree, gFld(getMenuTitle(MENU_VOICE_SETTINGS), 'voicesip_basic.html'));
		insDoc(nodeVoice, gLnk('R', getMenuTitle(MENU_VOICE_SIP_BASIC), 'voicesip_basic.html'));
		insDoc(nodeVoice, gLnk('R', getMenuTitle(MENU_VOICE_SIP_ADVANCED), 'voicesip_advanced.html'));
		insDoc(nodeVoice, gLnk('R', getMenuTitle(MENU_VOICE_SIP_DEBUG), 'voicesip_debug.html'));
		if( voiceNtr != '2' ) {
			insDoc(nodeVoice, gLnk('R', getMenuTitle(MENU_VOICE_NTR), 'voicentr.html'));
		}
		if( dect == '1' ) {
			insDoc(nodeVoice, gLnk('R', getMenuTitle(MENU_VOICE_DECT), 'voicedect.html'));
		}
	}

	// Configure VLAN port mapping menu
	if ((atm == '1' || usb3g == '1') && vlanconfig == '1' ) {
		insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_INTF_GROUPING),'portmap.cmd'));
	}

	if ( ipv6Support == '1' ) {
		var nodeIpTunnel = insDoc(nodeAdvancedSetup, gFld(getMenuTitle(MENU_IP_TUNNEL),'tunnelcfg.cmd?action=viewcfg'));
		insDoc(nodeIpTunnel, gLnk('R', getMenuTitle(MENU_6IN4_TUNNEL),'tunnelcfg.cmd?action=viewcfg'));
		insDoc(nodeIpTunnel, gLnk('R', getMenuTitle(MENU_4IN6_TUNNEL),'tunnelcfg.cmd?action=view'));
	}

	if (ipsec == '1') {
		insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_SC_IPSEC), 'ipsec.cmd?action=view'));
	}

	if (certificate == '1')  {
		var nodeCert = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_CERT), 'certlocal.cmd?action=view'));
		insDoc(nodeCert, gLnk('R', getMenuTitle(MENU_CERT_LOCAL), 'certlocal.cmd?action=view'));
		insDoc(nodeCert, gLnk('R', getMenuTitle(MENU_CERT_CA), 'certca.cmd?action=view'));
	}

	// Configure standby menu item 
	if ( standby == '1' ) 
		insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_STANDBY), 'standby.html'));

	// Configure power management 
	if ( pwrmngt == '1' ) 
		insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_PWRMNGT), 'pwrmngt.html'));
  
	if ( bmu == '1' ) 
		insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_BMU), 'bmu.html'));

	if ( multicast == '1' ) 
		insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_MULTICAST), 'multicast.html'));
  
	// Configure diagnostics menu
	var nodeDiagnostics = insFld(foldersTree, gFld(getMenuTitle(MENU_DIAGNOSTICS), 'diag.html'));
	if (p8021ag == '1') {
		insDoc(nodeDiagnostics, gLnk('R', getMenuTitle(MENU_DIAGNOSTICS),'diag.html'));
		insDoc(nodeDiagnostics, gLnk('R', getMenuTitle(MENU_DIAGP8021AG),'diag8021ag.html'));
	}

	// Configure management menu
	if (localAccess){
	var nodeMngr = insFld(foldersTree, gFld(getMenuTitle(MENU_MANAGEMENT), 'backupsettings.html'));

	var nodeSettings = insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SETTINGS), 'backupsettings.html'));
	insDoc(nodeSettings, gLnk('R', getMenuTitle(MENU_TL_SETTINGS_BACKUP),'backupsettings.html'));
	insDoc(nodeSettings, gLnk('R', getMenuTitle(MENU_TL_SETTINGS_UPDATE),'updatesettings.html'));
	insDoc(nodeSettings, gLnk('R', getMenuTitle(MENU_TL_SETTINGS_DEFAULT), 'defaultsettings.html'));
	} else {
		var nodeMngr = insFld(foldersTree, gFld(getMenuTitle(MENU_MANAGEMENT), 'logintro.html'));
	}

	insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SYSTEM_LOG), 'logintro.html'));
	
	if (engDebug == '1')
	{
		/* Configure Debug menu */
		insDoc(nodeMngr, gLnk('R', getMenuTitle(MENU_ENGDEBUG), 'engdebug.cmd'));
	}
	
	if (0)
		insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SECURITY_LOG), 'seclogintro.html'));
	
	if ( snmp == '1' )
		insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SNMP), 'snmpconfig.html'));
	if ( tr69c == '1' )
		insFld(nodeMngr, gFld(getMenuTitle(MENU_TR69C), 'tr69cfg.html'));
	if ( omci == '1' ) {
		nodeOmci = insFld(nodeMngr, gFld(getMenuTitle(MENU_OMCI_CFG), 'omcicfg.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_GET_SET),'omcicfg.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_CREATE),'omcicreate.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_GET_NEXT),'omcigetnext.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_MACRO),'omcimacro.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_DOWNLOAD),'omcidownload.html'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_SYSTEM),'omcisystem.html'));
	}	  
	if ( sntp == '1' /*&& proto != 'Bridge' && !(proto=='PPPoE' && ipExt=='1') && !(proto=='PPPoA' && ipExt=='1') */) /* modify by wwj */
		insFld(nodeMngr, gFld(getMenuTitle(MENU_SNTP), 'sntpcfg.html'));

	nodeAccCntr = insFld(nodeMngr, gFld(getMenuTitle(MENU_ACC_CNTR), 'password.html'));
	insDoc(nodeAccCntr, gLnk('R', getMenuTitle(MENU_ACC_CNTR_PASSWORD), 'password.html'));
	insDoc(nodeAccCntr, gLnk('R', getMenuTitle(MENU_ACC_CNTR_REMOTE), 'remote.cmd?action=view'));

//	if (osgi_jvm == 1)
//	{
//		var nodeSWModules = insFld(nodeMngr, gFld(getMenuTitle(MENU_SW_MODULES), 'swmodulesEE.cmd'));
//		insDoc(nodeSWModules, gLnk('R', getMenuTitle(MENU_SW_MODULES_EE),'swmodulesEE.cmd'));
//		insDoc(nodeSWModules, gLnk('R', getMenuTitle(MENU_SW_MODULES_DU),'swmodulesDU.cmd'));
//		insDoc(nodeSWModules, gLnk('R', getMenuTitle(MENU_SW_MODULES_EU), 'swmodulesEU.cmd'));
//	}
	if (localAccess)
	insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_UPDATE_SOFTWARE), 'upload.html'));

	insFld(nodeMngr, gFld(getMenuTitle(MENU_RESET_ROUTER), 'resetrouter.html'));
}

function menuSupport(options) {
	var localAccess = options[MENU_OPTION_WANACCESS] != '1';
	var std = options[MENU_OPTION_STANDARD];
	var proto = options[MENU_OPTION_PROTOCOL];
	var ipExt = options[MENU_OPTION_IP_EXTENSION];
	var wireless = options[MENU_OPTION_WIRELESS];
	var voice = options[MENU_OPTION_VOICE];
	var snmp = options[MENU_OPTION_SNMP];
	var ddnsd = options[MENU_OPTION_DDNSD];
	var sntp = options[MENU_OPTION_SNTP];
	var QosEnabled = options[MENU_OPTION_QOS];
	var ipp = options[MENU_OPTION_IPP];
	var rip = options[MENU_OPTION_RIP];
	var tr69c = options[MENU_OPTION_TR69C];
	var ipv6Support = options[MENU_OPTION_IPV6_SUPPORT];
	var ipv6Enable = options[MENU_OPTION_IPV6_ENABLE];
	var upnp = options[MENU_OPTION_UPNP];
	var dnsproxy = options[MENU_OPTION_DNSPROXY];
	var omci = options[MENU_OPTION_OMCI];
	var chipId = options[MENU_OPTION_CHIPID];
	var numWl = options[MENU_OPTION_WIRELESS_NUM_ADAPTOR];
	var wireless_ses = options[MENU_OPTION_WIRELESS_SES];
	var ethwan = options[MENU_OPTION_ETHWAN];
	var ptm = options[MENU_OPTION_PTMWAN];
//	var pwrmngt = options[MENU_OPTION_PWRMNGT];	
	var pwrmngt = 0; // do not display this page
	var standby = options[MENU_OPTION_STANDBY];
	var voiceNtr = options[MENU_OPTION_VOICE_NTR];
	var atm = options[MENU_OPTION_ATMWAN];
	var usb3g = options[MENU_OPTION_USB_3G];			// 13/9/9 added by pj for usb 3g
	var mocawan = options[MENU_OPTION_MOCAWAN];
	var gponwan = options[MENU_OPTION_GPONWAN];
	var eponwan = options[MENU_OPTION_EPONWAN];
	var dect = options[MENU_OPTION_VOICE_DECT];
	var dslbonding = options[MENU_OPTION_DSL_BONDING];
	var multicast = options[MENU_OPTION_MULTICAST];
	var l2tp = options[MENU_OPTION_L2TP];
	var storageservice = options[MENU_OPTION_STORAGESERVICE];
	var sambaservice = options[MENU_OPTION_SAMBA];
	var tpUsbService = sambaservice; /* reserved */
	var mocaCfg = options[MENU_OPTION_SUPPORT_MOCA];
	var wireless_wapi = options[MENU_OPTION_WIRELESS_WAPI_AS];
	var policeEnable = options[MENU_OPTION_POLICE_ENABLE];
	var bmu = options[MENU_OPTION_BMU];
	var isDsl = 0;
	var osgi_jvm = options[MENU_OPTION_OSGI_JVM];
	var arr, i;

	// Configure advanced setup/layer 2 interface
	arr = new Array();
	if (atm == '1') {
		isDsl = 1;
		arr.push(getMenuTitle(MENU_DSL_ATM_INTERFACE), 'dslatm.cmd');
	}
	if (usb3g == '1') arr.push(getMenuTitle(MENU_USB_3G_INTERFACE), 'usb3g.cmd?action=view');	// 2013/9/9 added by pj for usb 3g
	if (ptm == '1') {
		isDsl = 1;
		arr.push(getMenuTitle(MENU_DSL_PTM_INTERFACE), 'dslptm.cmd');
	}
	if (gponwan == '1' ) arr.push(getMenuTitle(MENU_GPONWAN_INTERFACE), 'gponwan.cmd');
	if (eponwan == '1' ) arr.push(getMenuTitle(MENU_EPONWAN_INTERFACE), 'eponwan.cmd');
	if (ethwan == '1' )  arr.push(getMenuTitle(MENU_ETH_INTERFACE), 'ethwan.cmd');
	if (mocawan == '1' ) arr.push(getMenuTitle(MENU_MOCA_INTERFACE), 'mocawan.cmd');
	var nodeAdvancedSetup;
	if (arr.length > 0) {
		nodeAdvancedSetup = insFld(foldersTree, gFld(getMenuTitle(MENU_ADVANCED_SETUP), arr[1]));
		var nodeLayer2Inteface = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_LAYER2_INTERFACE), arr[1]));
		for (i = 0; i < arr.length; i += 2) {
			insDoc(nodeLayer2Inteface, gLnk('R', arr[i], arr[i+1]));
		}
	} else {
		nodeAdvancedSetup = insFld(foldersTree, gFld(getMenuTitle(MENU_ADVANCED_SETUP), 'wancfg.cmd'));
	}
	insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_WAN), 'wancfg.cmd'));

	arr = new Array();
	if (l2tp == '1') arr.push(getMenuTitle(MENU_VPN_L2TPAC), 'l2tpacwan.cmd');
	if (arr.length > 0) {
		var nodeVPN = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_VPN), arr[1]));
		for (i = 0; i < arr.length; i += 2) {
			insDoc(nodeVPN, gLnk('R', arr[i], arr[i+1]));
		}
	}

	if ( ipv6Enable == '1' ) {
		var nodeLAN = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_LAN),'lancfg2.html'));
		insDoc(nodeLAN, gLnk('R', getMenuTitle(MENU_LAN6),'ipv6lancfg.html'));
	}
	else {
		insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_LAN),'lancfg2.html'));
	}

	if ( mocaCfg == '1' ) {
		insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_MOCA_CONFIGURATION),'mocacfg.html'));
	}

	// Configure QoS class menu
	var nodeQos = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_QOS),'qosqmgmt.html'));
	insDoc(nodeQos, gLnk('R', getMenuTitle(MENU_QOS_QUEUE), 'qosqueue.cmd?action=view'));
	if (policeEnable == '1')
		insDoc(nodeQos, gLnk('R', getMenuTitle(MENU_QOS_POLICER), 'qospolicer.cmd?action=view'));
	insDoc(nodeQos, gLnk('R', getMenuTitle(MENU_QOS_CLASS), 'qoscls.cmd?action=view'));

	// Configure routing menu
	var nodeRouting = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_ROUTING), 'rtdefaultcfg.html'));
	insDoc(nodeRouting, gLnk('R', getMenuTitle(MENU_RT_DEFAULT_ROUTE), 'rtdefaultcfg.html'));
	insDoc(nodeRouting, gLnk('R', getMenuTitle(MENU_RT_STATIC_ROUTE),'rtroutecfg.cmd?action=viewcfg'));

	if ( (proto == 'PPPoE' && ipExt == '0') ||
		  (proto == 'PPPoA' && ipExt == '0') ||
		  (proto == 'MER') ||
		  (proto == 'IPoA') ) {
		// configure rip
		if ( rip == '1' )
			insDoc(nodeRouting, gLnk('R', getMenuTitle(MENU_RT_RIP),'ripcfg.cmd?action=view'));
		// configure dns server
		var nodeDnsSetup = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_DNS), 'dnscfg.html'));
		insDoc(nodeDnsSetup, gLnk('R', getMenuTitle(MENU_DNS_SETUP), 'dnscfg.html'));
		// configure ddns client
		if ( ddnsd == '1' )
			insDoc(nodeDnsSetup, gLnk('R', getMenuTitle(MENU_DDNS), 'ddnsmngr.cmd'));
	}

	if (isDsl == 1)
	{
		// Configure ADSL Setting Menu based on Annex
		if ( std == 'annex_c' )
			insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_DSL), 'adslcfgc.html'));
		else if (chipId != '6368')
			insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_DSL), 'adslcfg.html'));
		else
			insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_DSL), 'xdslcfg.html'));

		if (dslbonding == '1')
			insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_DSL_BONDING), 'dslbondingcfg.html'));
	}

	// Configure print server
	if ( ipp == '1' )
		insDoc(nodeAdvancedSetup, gFld(getMenuTitle(MENU_IPP), 'ippcfg.html'));

	// Configure upnp
	if (upnp == '1')
		insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_UPNP), 'upnpcfg.html'));

	// Configure dnsproxy
	if (dnsproxy == '1')
		insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_DNSPROXY), 'dnsproxycfg.html'));

	// Configure standby menu item 
	if ( standby == '1' ) 
		insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_STANDBY), 'standby.html'));

	// Configure power management 
	if ( pwrmngt == '1' ) 
		insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_PWRMNGT), 'pwrmngt.html'));

	if ( bmu == '1' ) 
		insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_BMU), 'bmu.html'));

	// Configure wireless menu
	if ( parseInt(numWl) != 0 ) {
		 if(numWl != '1')
			  var wlanMenu = insFld(foldersTree, gFld(getMenuTitle(MENU_WIRELESS_SETTINGS), wlItemsCgiCmd[0]));
 
		 for(i = 0; i < parseInt(numWl); i++)
		 {
			if(numWl == '1')
				 var nodeWireless = insFld(foldersTree, gFld(getMenuTitle(MENU_WIRELESS_SETTINGS), wlItemsCgiCmd[0]));
			else
				var nodeWireless = insFld(wlanMenu, gFld(wlmenuTitle[i], wlItemsCgiCmd[i]));
			insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_BASIC), 'wlcfg.html'));
			insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_SECURITY), 'wlsecurity.html'));
			insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_MAC_FILTERING), 'wlmacflt.cmd?action=view'));
			insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_WDS), 'wlwds.cmd?action=view'));
			insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_ADVANCED), 'wlcfgadv.html'));
			//SUPPORT_SES
			if ( wireless_ses == '1' ) { 
				insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_SES), 'wlses.html'));
			}

			insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_STATION_LIST), 'wlstationlist.cmd'));
		}

		if ( wireless_wapi == '1' ) {
			 if (numWl == '1') {
				 insDoc(nodeWireless, gLnk('R', getMenuTitle(MENU_WL_WAPI_AS), 'wlwapias.html'));
			 }
			 else {
				 insDoc(wlanMenu, gLnk('R', getMenuTitle(MENU_WL_WAPI_AS), 'wlwapias.html'));
			 }
		}
	}

	if (tpUsbService == '1')
	{
		/* Configure USB menu */
		var nodeUsb = insFld(foldersTree, gFld(getMenuTitle(MENU_USB_SETTINGS), 'usbManage.cmd?action=view'));
		insDoc(nodeUsb, gLnk('R', getMenuTitle(MENU_USB_MANAGE), 'usbManage.cmd?action=view'));
		insDoc(nodeUsb, gLnk('R', getMenuTitle(MENU_USB_USR_ACCOUNTS), 'usbUsrAccount.cmd?action=view'));
		insDoc(nodeUsb, gLnk('R', getMenuTitle(MENU_USB_SMB_SERVER), 'usbSmbSrv.cmd?action=view'));
		insDoc(nodeUsb, gLnk('R', getMenuTitle(MENU_USB_FTP_SERVER), 'usbFtpSrv.cmd?action=view'));
		insDoc(nodeUsb, gLnk('R', getMenuTitle(MENU_USB_DLNA_SERVER), 'usbDlnaManage.cmd?action=view'));
		insDoc(nodeUsb, gLnk('R', getMenuTitle(MENU_USB_PRINT_SERVER), 'usbPrintSrv.cmd?action=view'));
	}

	  /*Storage Service menu */
/*	if(storageservice == '1')
	{
		var nodeStorage = insFld(nodeAdvancedSetup, gFld(getMenuTitle(MENU_STORAGESERVICE), 'storageservicecfg.cmd?view'));
		insDoc(nodeStorage, gLnk('R', getMenuTitle(MENU_STORAGE_INFO), 'storageservicecfg.cmd?view'));
		if(sambaservice == '1'){
			insDoc(nodeStorage, gLnk('R', getMenuTitle(MENU_STORAGE_USERACCOUNT), 'storageuseraccountcfg.cmd?view'));
		}
	}
*/
	// Configure voice menu
	if ( voice == 'MGCP' ) {
		var nodeVoice = insFld(foldersTree, gFld(getMenuTitle(MENU_VOICE_SETTINGS), 'voicemgcp_basic.html'));
		insDoc(nodeVoice, gLnk('R', getMenuTitle(MENU_VOICE_MGCP), 'voicemgcp_basic.html'));
		if( voiceNtr != '2' ) {
			insDoc(nodeVoice, gLnk('R', getMenuTitle(MENU_VOICE_NTR), 'voicentr.html'));
		}
	}
	else if ( voice == 'SIP' ) {
		var nodeVoice = insFld(foldersTree, gFld(getMenuTitle(MENU_VOICE_SETTINGS), 'voicesip_basic.html'));
		insDoc(nodeVoice, gLnk('R', getMenuTitle(MENU_VOICE_SIP_BASIC), 'voicesip_basic.html'));
		insDoc(nodeVoice, gLnk('R', getMenuTitle(MENU_VOICE_SIP_ADVANCED), 'voicesip_advanced.html'));
		insDoc(nodeVoice, gLnk('R', getMenuTitle(MENU_VOICE_SIP_DEBUG), 'voicesip_debug.html'));
		if( voiceNtr != '2' ) {
			insDoc(nodeVoice, gLnk('R', getMenuTitle(MENU_VOICE_NTR), 'voicentr.html'));
		}
		if( dect == '1' ) {
			insDoc(nodeVoice, gLnk('R', getMenuTitle(MENU_VOICE_DECT), 'voicedect.html'));
		}
	}

	 // Configure diagnostics menu
	insFld(foldersTree, gFld(getMenuTitle(MENU_DIAGNOSTICS), 'diag.html'));

	// Configure management menu
	if (localAccess) {
	var nodeMngr = insFld(foldersTree, gFld(getMenuTitle(MENU_MANAGEMENT), 'backupsettings.html'));
	nodeSettings = insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SETTINGS), 'backupsettings.html'));
	insDoc(nodeSettings, gLnk('R', getMenuTitle(MENU_TL_SETTINGS_BACKUP),'backupsettings.html'));
	insDoc(nodeSettings, gLnk('R', getMenuTitle(MENU_TL_SETTINGS_UPDATE),'updatesettings.html'));
	insDoc(nodeSettings, gLnk('R', getMenuTitle(MENU_TL_SETTINGS_DEFAULT), 'defaultsettings.html'));
	} else {
		var nodeMngr = insFld(foldersTree, gFld(getMenuTitle(MENU_MANAGEMENT), 'logintro.html'));
	}

	insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SYSTEM_LOG), 'logintro.html'));
	if (0)
		insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SECURITY_LOG), 'seclogintro.html'));
	if ( snmp == '1' )
		insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SNMP), 'snmpconfig.html'));
	if ( tr69c == '1' )
		insFld(nodeMngr, gFld(getMenuTitle(MENU_TR69C), 'tr69cfg.html'));
	if ( omci == '1' ) {
		nodeOmci = insFld(nodeMngr, gFld(getMenuTitle(MENU_OMCI_CFG), 'omcicfg.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_GET_SET),'omcicfg.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_CREATE),'omcicreate.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_GET_NEXT),'omcigetnext.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_MACRO),'omcimacro.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_DOWNLOAD),'omcidownload.html'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_SYSTEM),'omcisystem.html'));
	}
	if ( sntp == '1' && proto != 'Bridge' && !(proto=='PPPoE' && ipExt=='1') && !(proto=='PPPoA' && ipExt=='1') )
		insFld(nodeMngr, gFld(getMenuTitle(MENU_SNTP), 'sntpcfg.html'));

	if (osgi_jvm == 1)
	{
		var nodeSWModules = insFld(nodeMngr, gFld(getMenuTitle(MENU_SW_MODULES), 'swmodulesEE.cmd'));
		insDoc(nodeSWModules, gLnk('R', getMenuTitle(MENU_SW_MODULES_EE),'swmodulesEE.cmd'));
		insDoc(nodeSWModules, gLnk('R', getMenuTitle(MENU_SW_MODULES_DU),'swmodulesDU.cmd'));
		insDoc(nodeSWModules, gLnk('R', getMenuTitle(MENU_SW_MODULES_EU), 'swmodulesEU.cmd'));
	}

	if (localAccess)
	insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_UPDATE_SOFTWARE), 'upload.html'));
	insFld(nodeMngr, gFld(getMenuTitle(MENU_RESET_ROUTER), 'resetrouter.html'));
}

function menuUser() {
	var localAccess = options[MENU_OPTION_WANACCESS] != '1';
	var snmp = options[MENU_OPTION_SNMP];
	var tr69c = options[MENU_OPTION_TR69C];
	var omci = options[MENU_OPTION_OMCI];
	
	// Configure diagnostics menu
	var nodeDiagnostics = insFld(foldersTree, gFld(getMenuTitle(MENU_DIAGNOSTICS), 'diag.html'));

	// Configure management menu
	var nodeMngr = insFld(foldersTree, gFld(getMenuTitle(MENU_MANAGEMENT), 'logintro.html'));
	insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SYSTEM_LOG), 'logintro.html'));
	if ( snmp == '1' )
	insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SNMP), 'snmpconfig.html'));
	if ( tr69c == '1' )
		insFld(nodeMngr, gFld(getMenuTitle(MENU_TR69C), 'tr69cfg.html'));
	if ( omci == '1' ) {
		var nodeOmci = insFld(nodeMngr, gFld(getMenuTitle(MENU_OMCI_CFG), 'omcicfg.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_GET_SET),'omcicfg.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_CREATE),'omcicreate.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_GET_NEXT),'omcigetnext.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_MACRO),'omcimacro.cmd?action=view'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_DOWNLOAD),'omcidownload.html'));
		insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_SYSTEM),'omcisystem.html'));
	}
	if (localAccess) insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_UPDATE_SOFTWARE), 'upload.html'));
}

function createBcmMenu(options) {
	var user = options[MENU_OPTION_USER];
	var proto = options[MENU_OPTION_PROTOCOL];
	var ipExt = options[MENU_OPTION_IP_EXTENSION];
	var dhcpen = options[MENU_OPTION_DHCPEN];
	var vdslIncluded = options[MENU_OPTION_VDSL];
	var mocaStats = options[MENU_OPTION_SUPPORT_MOCA];
	var ptm = options[MENU_OPTION_PTMWAN];
	var atm = options[MENU_OPTION_ATMWAN];
	var mocawan = options[MENU_OPTION_MOCAWAN];
	var ethwan = options[MENU_OPTION_ETHWAN];
	var gponwan = options[MENU_OPTION_GPONWAN];
	var eponwan = options[MENU_OPTION_EPONWAN];

	foldersTree = gFld('', 'info.html');
	// device info menu
	var nodeDeviceInfo = insFld(foldersTree, gFld(getMenuTitle(MENU_DEVICE_INFO), 'info.html'));
	// device summary menu
	insFld(nodeDeviceInfo, gFld(getMenuTitle(MENU_DEVICE_SUMMARY), 'info.html'));
	// device wan menu
	insFld(nodeDeviceInfo, gFld(getMenuTitle(MENU_DEVICE_WAN), 'wancfg.cmd?action=view'));
	// device statistics menu
	var nodeSts = insFld(nodeDeviceInfo, gFld(getMenuTitle(MENU_STATISTICS), 'statsifc.html'));
	insDoc(nodeSts, gLnk('R', getMenuTitle(MENU_ST_LAN), 'statsifc.html'));
	if (mocaStats == '1')
		insDoc(nodeSts, gLnk('R', getMenuTitle(MENU_ST_MOCA), 'statsmoca.cmd?choice=LAN'));
	if (ptm == '1' || atm == '1' || mocawan == '1' ||
		 ethwan == '1' || gponwan == '1' || eponwan == '1')
		insDoc(nodeSts, gLnk('R', getMenuTitle(MENU_WAN), 'statswan.cmd'));
	if (ptm == '1' || atm == '1') {
		insDoc(nodeSts, gLnk('R', getMenuTitle(MENU_ST_ATM), 'statsxtm.cmd'));
		insDoc(nodeSts, gLnk('R', getMenuTitle(MENU_ST_ADSL), 'statsadsl.html'));
		if (vdslIncluded == 'true')
			insDoc(nodeSts, gLnk('R', getMenuTitle(MENU_ST_VDSL), 'statsvdsl.html'));
	}
	// device route menu
	insFld(nodeDeviceInfo, gFld(getMenuTitle(MENU_DEVICE_ROUTE), 'rtroutecfg.cmd?action=view'));
	insFld(nodeDeviceInfo, gFld(getMenuTitle(MENU_RT_ARP),'arpview.cmd'));
	// dhcp info
	if (!(/*proto == 'Bridge' ||*/ ipExt == '1') && dhcpen == '1') {
		insFld(nodeDeviceInfo, gFld(getMenuTitle(MENU_DHCPINFO),'dhcpinfo.html'));
	}
	if ( user == 'admin' )
		menuAdmin(options);
	else if ( user == 'support' )
		menuSupport(options);
	else if ( user == 'user' )
		menuUser();
		
	//insFld(foldersTree, gFld(getMenuTitle(MENU_LOGOUT), 'javascript:document.cookie="Authorization=;expires="+(new Date()).toGMTString();window.top.location.href="main.html";'));
	insFld(foldersTree, gFld(getMenuTitle(MENU_LOGOUT), 'javascript:if (confirm("Do you want to logout?")){document.cookie="Authorization=;expires="+(new Date()).toGMTString();window.top.location.href="main.html";};'));
}
