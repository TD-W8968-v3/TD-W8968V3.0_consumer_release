/***********************************************************************
 *
 * Copyright (c) 2006-2012  Broadcom Corporation
 * All Rights Reserved
 *
 * <:label-BRCM:2006-2012:DUAL/GPL:standard
 * 
 * Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed
 * to you under the terms of the GNU General Public License version 2
 * (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
 * with the following added to such license:
 * 
 *    As a special exception, the copyright holders of this software give
 *    you permission to link this software with independent modules, and
 *    to copy and distribute the resulting executable under terms of your
 *    choice, provided that you also meet, for each linked independent
 *    module, the terms and conditions of the license of that module.
 *    An independent module is a module which is not derived from this
 *    software.  The special exception does not apply to any modifications
 *    of the software.
 * 
 * Not withstanding the above, under no circumstances may you combine
 * this software in any way with any other Broadcom software provided
 * under a license other than the GPL, without Broadcom's express prior
 * written consent.
 * 
 * :>
 *
 ************************************************************************/

#ifndef __CMS_MSG_H__
#define __CMS_MSG_H__

#include "cms.h"
#include "cms_eid.h"


/*!\file cms_msg.h
 * \brief Public header file for messaging.
 * Code which need to handle messages must include this file.
 *
 * Here is a general description of how to use this interface.
 *
 * Early in application startup code, call cmsMsg_init() with a pointer
 * to void *.
 *
 * To receive a message, call cmsMsg_receive().  This returns a
 * pointer to a buffer that has a CmsMsgHeader at the beginning
 * and optional data after the header.  Free this buffer when you
 * are done with it by calling cmsMsg_free().
 *
 * To send a message, allocate a buffer big enough to hold a
 * CmsMsgHeader and any optional data you need to send with the
 * message.  Fill in the buffer (header and data portion), and
 * call cmsMsg_send().
 *
 * Before the application exits, call cmsMsg_cleanup().
 */


/*!\enum CmsMsgType
 * \brief  Enumeration of possible message types
 *
 * This is the complete list of message types.
 * By convention:
 * system event message types are from            0x10000250-0x100007ff
 * system request/response message types are from 0x10000800-0x10000fff
 * Voice event messages are from                  0x10002000-0x100020ff
 * Voice request/response messages are from       0x10002100-0x100021ff
 * GPON OMCI request/response messages are from   0x10002200-0x100022ff
 * Customers should add messages from             0x20000000-0x2fffffff
 * All other values are reserved for future use.
 *
 * Note that a message type does not specify whether it is a
 * request or response, that is indicated via the flags field of
 * CmsMsgHeader.
 */
typedef enum 
{
   CMS_MSG_SYSTEM_BOOT                          = 0x10000250, /**< system has booted, delivered to apps
                                                                 *   EIF_LAUNCH_ON_STARTUP set in their
                                                                 *   CmsEntityInfo.flags structure.
                                                                 */
   CMS_MSG_APP_LAUNCHED                         = 0x10000251, /**< Used by apps to confirm that launch succeeded.
                                                                 *   Sent from app to smd in cmsMsg_init.
                                                                 */
   CMS_MSG_WAN_LINK_UP                          = 0x10000252, /**< wan link is up (includes dsl, ethernet, etc) */
   CMS_MSG_WAN_LINK_DOWN                        = 0x10000253, /**< wan link is down */
   CMS_MSG_WAN_CONNECTION_UP                    = 0x10000254, /**< WAN connection is up (got IP address) */
   CMS_MSG_WAN_CONNECTION_DOWN                  = 0x10000255, /**< WAN connection is down (lost IP address) */
   CMS_MSG_ETH_LINK_UP                          = 0x10000256, /**< eth link is up (only if eth is used as LAN interface) */
   CMS_MSG_ETH_LINK_DOWN                        = 0x10000257, /**< eth link is down (only if eth is used as LAN interface) */
   CMS_MSG_USB_LINK_UP                          = 0x10000258, /**< usb link is up (only if eth is used as LAN interface) */
   CMS_MSG_USB_LINK_DOWN                        = 0x10000259, /**< usb link is down (only if eth is used as LAN interface) */
   CMS_MSG_ACS_CONFIG_CHANGED                   = 0x1000025A, /**< ACS configuration has changed. */
   CMS_MSG_DELAYED_MSG                          = 0x1000025B, /**< This message is delivered to when delayed msg timer expires. */
   CMS_MSG_TR69_ACTIVE_NOTIFICATION             = 0x1000025C, /**< This message is sent to tr69c when one or more
                                                       *   parameters with active notification attribute
                                                       *   has had their value changed.
                                                       */
   CMS_MSG_WAN_ERRORSAMPLES_AVAILABLE           = 0x1000025D,/**< WAN connection has vectoring error samples available */
   CMS_MSG_WAN_ERRORSAMPLES_AVAILABLE_LINE1     = 0x1000025E,/**< WAN connection has vectoring error samples available for line 1, keep CMS_MSG_WAN_ERRORSAMPLES_AVAILABLE+1*/
   CMS_MSG_GET_SHMID                            = 0x10000261, /**< Used by apps to get shmid from smd. */
   CMS_MSG_SHMID                                = 0x10000262, /**< Sent from ssk to smd when shmid is obtained. */
   CMS_MSG_MDM_INITIALIZED                      = 0x10000263, /**< Sent from ssk to smd when MDM has been initialized. */
   CMS_MSG_DHCPC_STATE_CHANGED                  = 0x10000264, /**< Sent from dhcp client when state changes, see also DhcpcStateChangeMsgBody */
   CMS_MSG_PPPOE_STATE_CHANGED                  = 0x10000265, /**< Sent from pppoe when state changes, see also PppoeStateChangeMsgBody */
   CMS_MSG_DHCP6C_STATE_CHANGED                 = 0x10000266, /**< Sent from dhcpv6 client when state changes, see also Dhcp6cStateChangeMsgBody */
   CMS_MSG_PING_STATE_CHANGED                   = 0x10000267, /**< Ping state changed (completed, or stopped) */
   CMS_MSG_DHCPD_RELOAD		                    = 0x10000268, /**< Sent to dhcpd to force it reload config file without restart */
   CMS_MSG_DHCPD_DENY_VENDOR_ID	                = 0x10000269, /**< Sent from dhcpd to notify a denied request with some vendor ID */
   CMS_MSG_DHCPD_HOST_INFO                      = 0x1000026A, /**< Sent from dhcpd to ssk to inform of lan host add/delete */
   CMS_MSG_TRACERT_STATE_CHANGED                = 0x1000026B, /**< Traceroute state changed (completed, or stopped) */
   CMS_MSG_DNSPROXY_RELOAD	                    = 0x10000270, /**< Sent to dnsproxy to force it reload config file without restart */
   CMS_MSG_SNTP_STATE_CHANGED 	                = 0x10000271, /**< SNTP state changed */
#ifdef DMP_X_BROADCOM_COM_IPV6_1
   CMS_MSG_DNSPROXY_IPV6_CHANGED                = 0x10000272, /**< Sent to dnsproxy to inform the DProxy IPv6 DNS server address */
#endif
   CMS_MSG_DNSPROXY_GET_STATS	                = 0x10000273, /**< Sent to dnsproxy to get DNS query error statistic */
   CMS_MSG_MCPD_RELOAD	                        = 0x10000276, /**< Sent to mcpd to force it reload config file without restart */
   CMS_MSG_MCPD_CTL   	                        = 0x10000277, /**< Sent to mcpd to force it reload config file without restart */
   CMS_MSG_CONFIG_WRITTEN                       = 0x10000280, /**< Event sent when a config file is written. */
   CMS_MSG_CONFIG_UPLOAD_COMPLETE               = 0x10000281, /**< Event sent when a remote configuration cycle has ended. */

   CMS_MSG_SET_PPP_UP                           = 0x10000290, /* Sent to ppp when set ppp up manually */
   CMS_MSG_SET_PPP_DOWN                         = 0x10000291, /* Sent to ppp when set ppp down manually */  

   CMS_MSG_DNSPROXY_DUMP_STATUS                 = 0x100002A1, /* Tell dnsproxy to dump its current status */
   CMS_MSG_DNSPROXY_DUMP_STATS                  = 0x100002A2, /* Tell dnsproxy to dump its statistics */
#ifdef DMP_X_BROADCOM_COM_IPV6_1
   CMS_MSG_RASTATUS6_INFO                       = 0x100002A3, /**< Sent from rastatus6 when RA is received, see also RAStatus6MsgBody */
#endif
   
#ifdef BRCM_WLAN
   CMS_MSG_WLAN_CHANGED          	            = 0x10000300,  /**< wlmngr jhc*/
#endif
   CMS_MSG_SNMPD_CONFIG_CHANGED                 = 0x1000301, /**< ACS configuration has changed. */
   CMS_MSG_MANAGEABLE_DEVICE_NOTIFICATION_LIMIT_CHANGED = 0x1000302, /**< Notification Limit of number of management device. */

   CMS_MSG_STORAGE_ADD_PHYSICAL_MEDIUM          = 0x1000310,
   CMS_MSG_STORAGE_REMOVE_PHYSICAL_MEDIUM       = 0x1000311,
   CMS_MSG_STORAGE_ADD_LOGICAL_VOLUME           = 0x1000312,
   CMS_MSG_STORAGE_REMOVE_LOGICAL_VOLUME        = 0x1000313,

   CMS_MSG_REGISTER_DELAYED_MSG                 = 0x10000800, /**< request a message sometime in the future. */
   CMS_MSG_UNREGISTER_DELAYED_MSG               = 0x10000801, /**< cancel future message delivery. */
   CMS_MSG_REGISTER_EVENT_INTEREST              = 0x10000802, /**< request receipt of the specified event msg. */
   CMS_MSG_UNREGISTER_EVENT_INTEREST            = 0x10000803, /**< cancel receipt of the specified event msg. */
   CMS_MSG_DIAG                                 = 0x10000805, /**< request diagnostic to be run */
   CMS_MSG_TR69_GETRPCMETHODS_DIAG              = 0x10000806, /**< request tr69c send out a GetRpcMethods */
   CMS_MSG_DSL_LOOP_DIAG_COMPLETE               = 0x10000807, /**< dsl loop diagnostic completes */

   CMS_MSG_START_APP                            = 0x10000807, /**< request smd to start an app; pid is returned in the wordData */
   CMS_MSG_RESTART_APP                          = 0x10000809, /**< request smd to stop and then start an app; pid is returned in the wordData */
   CMS_MSG_STOP_APP                             = 0x1000080A, /**< request smd to stop an app */
   CMS_MSG_IS_APP_RUNNING                       = 0x1000080B, /**< request to check if the the application is running or not */
   CMS_MSG_APP_TERMINATED                       = 0x1000080C, /**< register to smd for application termination info. */
   CMS_MSG_TERMINATE                            = 0x1000080D,  /**< request app to terminate, a response means action has started. */

   CMS_MSG_REBOOT_SYSTEM                        = 0x10000850,  /**< request smd to reboot, a response means reboot sequence has started. */

   CMS_MSG_DUMP_EID_INFO                        = 0x1000085A,  /**< request smd to dump its eid info DB */
   CMS_MSG_RESCAN_EID_INFO                      = 0x1000085B,  /**< request smd to rescan the eid info files (future) */
   CMS_MSG_ADD_EID_INFO                         = 0x1000085C,  /**< request smd to add given eid info to its DB (future) */
   CMS_MSG_APPLY_EID_INFO                       = 0x1000085D,  /**< request smd to apply EID settings (future) */

   CMS_MSG_SET_LOG_LEVEL                        = 0x10000860,  /**< request app to set its log level. */
   CMS_MSG_SET_LOG_DESTINATION                  = 0x10000861,  /**< request app to set its log destination. */

   CMS_MSG_MEM_DUMP_STATS                       = 0x1000086A,  /**< request app to dump its memstats */
   CMS_MSG_MEM_DUMP_TRACEALL                    = 0x1000086B,  /**< request app to dump all of its mem leak traces */
   CMS_MSG_MEM_DUMP_TRACE50                     = 0x1000086C,  /**< request app to its last 50 mem leak traces */
   CMS_MSG_MEM_DUMP_TRACECLONES                 = 0x1000086D,  /**< request app to dump mem leak traces with clones */

   CMS_MSG_LOAD_IMAGE_STARTING                  = 0x10000870,  /**< notify smd that image network loading is starting. */
   CMS_MSG_LOAD_IMAGE_DONE                      = 0x10000871,  /**< notify smd that image network loading is done. */
   CMS_MSG_GET_CONFIG_FILE                      = 0x10000872,  /**< ask smd for a copy of the config file. */
   CMS_MSG_VALIDATE_CONFIG_FILE                 = 0x10000873,  /**< ask smd to validate the given config file. */
   CMS_MSG_WRITE_CONFIG_FILE                    = 0x10000874,  /**< ask smd to write the config file. */
   CMS_MSG_VENDOR_CONFIG_UPDATE                 = 0x10000875,  /**<  the config file. */

   CMS_MSG_GET_WAN_LINK_STATUS                  = 0x10000880,  /**< request current WAN LINK status. */
   CMS_MSG_GET_WAN_CONN_STATUS                  = 0x10000881,  /**< request current WAN Connection status. */
   CMS_MSG_GET_LAN_LINK_STATUS                  = 0x10000882,  /**< request current LAN LINK status. */
  
   CMS_MSG_WATCH_WAN_CONNECTION                 = 0x10000890,  /**< request ssk to watch the dsl link status and then change the connectionStatus for bridge, static MER and ipoa */
   CMS_MSG_WATCH_DSL_LOOP_DIAG                  = 0x10000891,  /**< request ssk to watch the dsl loop diag and then update the stats */

   CMS_MSG_GET_LEASE_TIME_REMAINING             = 0x100008A0,  /**< ask dhcpd how much time remains on lease for particular LAN host */
   CMS_MSG_GET_DEVICE_INFO                      = 0x100008A1,  /**< request system/device's info */
   CMS_MSG_REQUEST_FOR_PPP_CHANGE               = 0x100008A2,  /**< request for disconnect/connect ppp  */
   CMS_MSG_EVENT_SNTP_SYNC                      = 0x100008A3,  /**< sntp send sync delta value */

   CMS_MSG_MOCA_WRITE_LOF                       = 0x100008B1, /**< mocad reporting last operational frequency */
   CMS_MSG_MOCA_READ_LOF                        = 0x100008B2, /**< mocad reporting last operational frequency */
   CMS_MSG_MOCA_WRITE_MRNONDEFSEQNUM            = 0x100008B3, /**< mocad reporting moca reset non-def sequence number */
   CMS_MSG_MOCA_READ_MRNONDEFSEQNUM             = 0x100008B4, /**< mocad reporting moca reset non-def sequence number */
   CMS_MSG_MOCA_NOTIFICATION                    = 0x100008B5, /**< application reporting that it has updated moca parameters */

   CMS_MSG_QOS_DHCP_OPT60_COMMAND               = 0x100008C0, /**< QoS Vendor Class ID classification command */
   CMS_MSG_QOS_DHCP_OPT77_COMMAND               = 0x100008C1, /**< QoS User   Class ID classification command */

   CMS_MSG_VOICE_CONFIG_CHANGED                 = 0x10002000, /**< Voice Configuration parameter changed private event msg. */
   CMS_MSG_VODSL_BOUNDIFNAME_CHANGED            = 0x10002001, /**< vodsl BoundIfName param has changed. */
   CMS_MSG_SHUTDOWN_VODSL                       = 0x10002002, /**< Voice shutdown request. */
   CMS_MSG_START_VODSL                          = 0x10002003, /**< Voice start request. */
   CMS_MSG_REBOOT_VODSL                         = 0x10002004, /**< Voice reboot request. This is for the voice reboot command */
   CMS_MSG_RESTART_VODSL                        = 0x10002005, /**< Voice re-start request. This is to restart the call manager when the IP address changes*/
   CMS_MSG_INIT_VODSL                           = 0x10002006, /**< Voice init request. */
   CMS_MSG_DEINIT_VODSL                         = 0x10002007, /**< Voice init request. */
   CMS_MSG_RESTART_VODSL_CALLMGR                = 0x10002008, /**< Voice call manager re-start request. */
   CMS_MSG_DEFAULT_VODSL                        = 0x10002009, /**< Voice call manager set defaults request. */
#ifdef DMP_X_BROADCOM_COM_NTR_1
   CMS_MSG_VOICE_NTR_CONFIG_CHANGED             = 0x10002009, /**< Voice NTR Configuration parameter changed private event msg. */
#endif /* DMP_X_BROADCOM_COM_NTR_1 */
   CMS_MSG_VOICE_GET_RTP_STATS                  = 0x10002010, /**< Voice get RTP PM stats msg (OMCI). */

#ifdef DMP_X_BROADCOM_COM_EPON_1
   CMS_MSG_EPONMAC_BOOT_COMPLETE                = 0x10002011, /**< Voice notification from eponapp that EPON mac has booted. */
#endif /* DMP_X_BROADCOM_COM_EPON_1 */
   CMS_MSG_ROUTING_UPDATE                       = 0x10002012, /**< Voice routing-specific element changed. */

   CMS_MSG_VOICE_DIAG                           = 0x10002100, /**< request voice diagnostic to be run */
   CMS_MSG_VOICE_STATISTICS_REQUEST             = 0x10002101, /**< request for Voice call statistics */
   CMS_MSG_VOICE_STATISTICS_RESPONSE            = 0x10002102, /**< response for Voice call statistics */
   CMS_MSG_VOICE_STATISTICS_RESET               = 0x10002103, /**< request to reset Voice call statistics */
   CMS_MSG_VOICE_CM_ENDPT_STATUS                = 0x10002104, /**< request for Voice Line obj */
   CMS_MSG_VODSL_IS_READY_FOR_DEINIT            = 0x10002105, /**< query if the voice app is ready to deinit */
#ifdef DMP_X_BROADCOM_COM_DECTENDPOINT_1
   CMS_MSG_VOICE_DECT_START                     = 0x100021A0, /**< request for number of DECT endpoints */
   CMS_MSG_VOICE_DECT_STOP                      = 0x100021A1, /**< request for number of DECT endpoints */
   CMS_MSG_VOICE_DECT_MEM_SET                   = 0x100021A2, /**< request for number of DECT endpoints */
   CMS_MSG_VOICE_DECT_MEM_GET                   = 0x100021A3, /**< request for number of DECT endpoints */
   CMS_MSG_VOICE_DECT_MODE_SET                  = 0x100021A4, /**< request for number of DECT endpoints */
   CMS_MSG_VOICE_DECT_MODE_GET                  = 0x100021A5, /**< request for number of DECT endpoints */
   CMS_MSG_VOICE_DECT_EVENT                     = 0x100021A6, /**< DECT EVENT raised by DECT endpoints */
   CMS_MSG_VOICE_DECT_READY                     = 0x100021A7, /**< DECT Module is ready */
   CMS_MSG_VOICE_DECT_CM_EVENT                  = 0x100021A8, /**< Call manager events or states */
   CMS_MSG_VOICE_DECT_CALL_CTL_CMD              = 0x100021A9, /**< DECT Call Control commands*/

   CMS_MSG_VOICE_DECT_OPEN_REG_WND              = 0x100021F0, /**< request for opening DECT registration window */
   CMS_MSG_VOICE_DECT_CLOSE_REG_WND             = 0x100021F1, /**< request for closing DECT registration window */
   CMS_MSG_VOICE_DECT_INFO_REQ                  = 0x100021F2, /**< request for Voice DECT status information */
   CMS_MSG_VOICE_DECT_INFO_RSP                  = 0x100021F3, /**< response for Voice DECT status information */
   CMS_MSG_VOICE_DECT_AC_SET                    = 0x100021F4, /**< request for Voice DECT Access Code Set */
   CMS_MSG_VOICE_DECT_HS_INFO_REQ               = 0x100021F5, /**< request for Voice DECT handset status information */
   CMS_MSG_VOICE_DECT_HS_INFO_RSP               = 0x100021F6, /**< response for Voice DECT handset status information */
   CMS_MSG_VOICE_DECT_HS_DELETE                 = 0x100021F7, /**< request for deleting a handset from DECT module */
   CMS_MSG_VOICE_DECT_HS_PING                   = 0x100021F8, /**< request for pinging a handset from DECT module */
   CMS_MSG_VOICE_DECT_NUM_ENDPT                 = 0x100021F9, /**< request for number of DECT endpoints */
   CMS_MSG_VOICE_DECT_REGHSETLIST_UPDATE        = 0x100021FA, /**< Event indicating change in number of registered dect handset */
   CMS_MSG_VOICE_DECT_SYNC_DATE_TIME            = 0x100021FB, /**< request for date and time synchronization */
#endif /* DMP_X_BROADCOM_COM_DECTENDPOINT_1 */

   CMS_MSG_GET_GPON_OMCI_STATS                  = 0x10002200, /**< request GPON OMCI statistics */
   CMS_MSG_OMCI_COMMAND_REQUEST                 = 0x10002201, /**< GPON OMCI command message request */
   CMS_MSG_OMCI_COMMAND_RESPONSE                = 0x10002202, /**< GPON OMCI command message response */
   CMS_MSG_OMCI_DEBUG_GET_REQUEST               = 0x10002203, /**< GPON OMCI debug get message request */
   CMS_MSG_OMCI_DEBUG_GET_RESPONSE              = 0x10002204, /**< GPON OMCI debug get message response */
   CMS_MSG_OMCI_DEBUG_SET_REQUEST               = 0x10002205, /**< GPON OMCI debug set message request */
   CMS_MSG_OMCI_DEBUG_SET_RESPONSE              = 0x10002206, /**< GPON OMCI debug set message response */
   CMS_MSG_OMCI_DEBUG_MKERR_SWDLERR1            = 0x10002207, /**< GPON OMCI debug drop next section to cause missing section error */
   CMS_MSG_OMCI_DEBUG_MKERR_SWDLERR2            = 0x10002208, /**< GPON OMCI debug drop final section of next window to cause no response on final window section error */
   CMS_MSG_OMCI_DEBUG_MKERR_SWDLERR3            = 0x10002209, /**< GPON OMCI debug corrupt next section to cause CRC error on SW DL image */


   CMS_MSG_OMCI_IGMP_ADMISSION_CONTROL          = 0x10002220, /**< mcpd request admission control from omcid */
   CMS_MSG_OMCI_MLD_ADMISSION_CONTROL           = 0x10002221, /**< mcpd request admission control from omcid */
   CMS_MSG_OMCI_CAPTURE_STATE_ON                = 0x10002230, /**< Start the capture of OMCI msgs from OLT */
   CMS_MSG_OMCI_CAPTURE_STATE_OFF               = 0x10002231, /**< Stop the capture of OMCI msgs from OLT */
   CMS_MSG_OMCI_CAPTURE_REPLAY_ON               = 0x10002232, /**< Start the playback of OMCI msgs */
   CMS_MSG_OMCI_CAPTURE_REPLAY_OFF              = 0x10002233, /**< Start the playback of OMCI msgs */
   CMS_MSG_OMCI_CAPTURE_VIEW                    = 0x10002234, /**< Start the display OMCI msgs from a file */
   CMS_MSG_OMCI_CAPTURE_DOWNLOAD                = 0x10002235, /**< Download internal OMCI msg capture file */
   CMS_MSG_OMCI_CAPTURE_UPLOAD                  = 0x10002236, /**< Upload a file of OMCI msgs to replace internal file */
   CMS_MSG_OMCI_PROMISC_SET_REQUEST             = 0x10002240, /**< GPON OMCI Promisc set message request */
   CMS_MSG_OMCI_PROMISC_SET_RESPONSE            = 0x10002241, /**< GPON OMCI Promisc set message response */
   CMS_MSG_OMCI_GPON_WAN_SERVICE_STATUS_CHANGE  = 0x10002250, /**< OMCI-->RG - Wan service status change notification */
   CMS_MSG_OMCI_RG_WAN_SERVICE_STAUTS_CHANGE    = 0x10002251, /**< RG-->OMCI - WAN service status change notification */
   CMS_MSG_OMCI_MCPD_MIB_RESET                  = 0x10002252, /**< OMCID to MCPD Mib Reset message to clean up  VLANCTL rules and CMF flows */
   CMS_MSG_OMCI_VOIP_MIB_RESET                  = 0x10002253, /**< OMCID to VOIP Mib Reset message to clean up Voice stack */

   CMS_MSG_CMF_SEND_REQUEST                     = 0x10002301, /**< CMF File Send message request */
   CMS_MSG_CMF_SEND_RESPONSE                    = 0x10002302, /**< CMF File Send message response */

   CMS_MSG_OMCIPMD_SYNC                         = 0x10002400, /**< OMCIPMD command message request to re-sync timers and PMs. */
   CMS_MSG_OMCIPMD_DEBUG                        = 0x10002401, /**< OMCIPMD command to turn debug on/off for specific PMs. */
   CMS_MSG_OMCIPMD_MIB_RESET                    = 0x10002402, /**< OMCIPMD command to execute MIB_RESET. */
   CMS_MSG_OMCIPMD_ALARM_SEQ_SET                = 0x10002403, /**< OMCIPMD command to set OMCI Alarm Sequence Number. */
   CMS_MSG_OMCIPMD_ALARM_SEQ_GET                = 0x10002404, /**< OMCIPMD command to get OMCI Alarm Sequence Number. */
   CMS_MSG_OMCIPMD_SYNC_MIB_ADMINSTATE          = 0x10002405, /**< Make OMCIPMD to sync the admin states from CMS. */
   CMS_MSG_OMCIPMD_GET_STATS                    = 0x10002406, /**< OMCIPMD command to get OMCI PM stats for CLI. */

   CMS_MSG_OMCIPMD_MCAST_COUNTER_REQ            = 0x10002410, /**< OMCIPMD command message request for multicast RX octets. */
   CMS_MSG_OMCIPMD_MCAST_COUNTER_RSP            = 0x10002411, /**< OMCIPMD command message response for multicast RX octets. */

   CMS_MSG_OMCID_OMCIPMD_REQUEST                = 0x10002420, /**< OMCIPMD command message request. */
   CMS_MSG_ENCAPSULATED_OMCI                    = 0x10002421, /**< OMCIPMD encapsulated OMCI raw frame. */

   CMS_MSG_REQUEST_DU_STATE_CHANGE              = 0x10002500, /**< request Deployment Unit state change */
   CMS_MSG_REQUEST_EU_STATE_CHANGE              = 0x10002501, /**< request Execution Unit state change */
   CMS_MSG_GET_FILE_FROM_SERVER                 = 0x10002502, /**< get file from file server */
   CMS_MSG_PUT_FILE_TO_SERVER                   = 0x10002503, /**< put file on file server */
   CMS_MSG_OSGI_EXE_ENV_STATUS_CHANGE           = 0x10002504, /**< OSGI execution environment state change */
   CMS_MSG_REQUEST_BUNDLE_LIST                  = 0x10002505, /**< request for bundle list */
   CMS_MSG_GET_NTH_GPON_WAN_LINK_INFO           = 0x10002506,  /**< Get GPON WAN LINK connection info */
   CMS_MSG_RESPONSE_DU_STATE_CHANGE             = 0x10002507, /**< response Deployment Unit state change */
   CMS_MSG_GPON_LINK_STATUS_CHANGE              = 0x10002508, /**< GPON Link Status Change */

   CMS_MSG_BMU_CLI                              = 0x10002600, /**< Internal commands from command line interface */
   CMS_MSG_BMU_GET_STATUS                       = 0x10002601, /**<  */

   CMS_MSG_OSGID_PRINT                          = 0x10002654, /**< OSGID command to print debug info to console */

/*hanjiayan add for DLNA*/
   CMS_MSG_DLNA_INIT = 0x10002701,
   CMS_MSG_DLNA_MANUAL_SCAN = 0x10002702,
   CMS_MSG_DLNA_OP_FOLDER = 0x10002703,
   CMS_MSG_DLNA_RELOAD = 0x10002704,
   CMS_MSG_DLNA_STATE_CHANGE = 0x10002705,
/*hanjiayan add for DLNA*/

   CMS_MSG_IPSEC_CFG_CHANGED = 0x10002800,
   CMS_MSG_IPSEC_DFL_GW_CHANGED= 0x10002801,

   CMS_MSG_CLI_WLAN_GETVAR = 0x10002900, /* added by yanglei for cli wlan config, 2012-3-28 */
   CMS_MSG_CLI_WLAN_SETVAR = 0x10002901,

   /* added by wwj for cli locked 130715 */
   CMS_MSG_CLI_SET_INFO = 0x10003002,
   CMS_MSG_CLI_GET_INFO = 0x10003003,
   /* end added */

#ifdef SUPPORT_USB_3G
   /*hanjiayan add for usb 3G*/
   CMS_MSG_USB_3G_CHANGE = 0x10003100,
   CMS_MSG_WRITE_3G_SETTING_FILE = 0x10003101,
   /*end add*/
#endif

   /* added by xieping 130817*/
   CMS_MSG_GET_PPP_CONN_STATUS = 0x10004000,
   /* end added */
} CmsMsgType;



/** This header must be at the beginning of every message.
 *
 * The header may then be followed by additional optional data, depending on
 * the message type.
 * Most of the fields should be self-explainatory.
 *
 */
typedef struct cms_msg_header
{
   CmsMsgType  type;  /**< specifies what message this is. */
   CmsEntityId src;   /**< CmsEntityId of the sender; for apps that can have
                       *   multiple instances, use the MAKE_SPECIFI_EID macro. */
   CmsEntityId dst;   /**< CmsEntityId of the receiver; for apps that can have
                       *   multiple instances, use the MAKE_SPECIFI_EID macro. */
   union {
      UINT16 all;     /**< All 16 bits of the flags at once. */
      struct {
         UINT16 event:1;    /**< This is a event msg. */
         UINT16 request:1;  /**< This is a request msg. */
         UINT16 response:1; /**< This is a response msg. */
         UINT16 requeue:1;  /**< Tell smd to send this msg back to sender. */
         UINT16 bounceIfNotRunning:1; /**< Do not launch the app to receive this message if
                                       *  it is not already running. */
         UINT16 unused:11;  /**< For future expansion. */
      } bits;
   } flags;  /**< Modifiers to the type of message. */
   UINT16 sequenceNumber;     /**< "Optional", but read the explanation below.
                               *
                               * Senders of request or event message types
                               * are free to set this to whatever
                               * they want, or leave it unitialized.  Senders
                               * are not required to increment the sequence
                               * number with every new message sent.
                               * However, response messages must 
                               * return the same sequence number as the
                               * request message.
                               * 
                               */
   struct cms_msg_header *next;   /**< Allows CmsMsgHeaders to be chained. */
   UINT32 wordData;   /**< As an optimization, allow one word of user
                       *   data in msg hdr.
                       *
                       * For messages that have only one word of data,
                       * we can just put the data in this field.
                       * One good use is for response messages that just
                       * need to return a status code.  The message type
                       * determines whether this field is used or not.
                       */
   UINT32 dataLength; /**< Amount of data following the header.  0 if no additional data. */
} CmsMsgHeader;

#define flags_event        flags.bits.event      /**< Convenience macro for accessing event bit in msg hdr */
#define flags_request      flags.bits.request    /**< Convenience macro for accessing request bit in msg hdr */
#define flags_response     flags.bits.response   /**< Convenience macro for accessing response bit in msg hdr */
#define flags_requeue      flags.bits.requeue    /**< Convenience macro for accessing requeue bit in msg hdr */
#define flags_bounceIfNotRunning flags.bits.bounceIfNotRunning   /**< Convenience macro for accessing bounceIfNotRunning bit in msg hdr */

#define EMPTY_MSG_HEADER   {0, 0, 0, {0}, 0, 0, 0, 0} /**< Initialize msg header to empty */


/*
* Hanjiayan add For DLNA
*/

#define  DLNA_SERVERNAME_LEN 16
#define  DLNA__FOLDER_ALIAS_LEN 32
#define  DLNA_FOLDER_NAME_LEN 128

/*manufacturer info*/
typedef struct 
{
	char 	devManufacturerURL[64];
	char 	manufacturer[64];
	char 	modelName[64];
	char 	devModelVersion[16];
	char 	description[256];
}MANUFACT_SPEC_INFO;

/*Dlna folder info*/
typedef struct 
{
	char		dispName[DLNA__FOLDER_ALIAS_LEN];
	char 	path[DLNA_FOLDER_NAME_LEN];
}DLNA_FOLDER_INFO;


/*Folder operation*/
typedef enum 
{
	DLNA_INIT_FOLDER = 0,
	DLNA_DEL_FOLDER = 1,
	DLNA_ADD_FOLDER = 2
}DLNA_FOLDER_OP;

/*Dlna init message body*/
typedef struct
{
	UBOOL8			serverState;			/* ServerState */
	char				serverName[DLNA_SERVERNAME_LEN];
	UBOOL8			scanFlag;				/*scan*/
	unsigned int		scanInterval;		/*scan interval*/
	MANUFACT_SPEC_INFO	manuInfo;				/*oem等不同厂商的信息*/
	unsigned int		folderCnt;			/*how many folde is shared now*/
}DlnaInitInfoBody;

/*Dlna reload message body*/
typedef struct 
{
	UBOOL8			serverState;			/* ServerState */
	char				serverName[DLNA_SERVERNAME_LEN];
	UBOOL8			scanFlag;				/*scan*/
	unsigned int		scanInterval;		/*scan interval*/	
}DlnaReloadInfoBody;

/*Dlna folder change message body*/
typedef struct
{
	 DLNA_FOLDER_OP			op;
	 DLNA_FOLDER_INFO		folder;
}DlnaFolderInfoBody;
/*hanjiayan add end*/

#if 1
/*hanjiayan add for usb 3G*/
#define MAX_SUPPORT_INTERFACE		16
#define USB_MODEM_NAME_LEN		64
#define MAX_DEVICE_NAME_LEN		64
#define MODE_SWITCH_CMD_LEN	    	68	

/*3G modem state*/
typedef enum
{
	MODEM_UNLINK = 0,
	MODEM_HANDLING,
	MODEM_LINK,
	MODEM_UNSUPPORT,
	MODEM_END,
}MODEM_STATUS;

typedef struct MODEM_INFO
{
	u_short vendorID; 
	u_short productID;
	u_char  modeswitchcmd[MODE_SWITCH_CMD_LEN];

	int interfaceNum;		
	int indexInterface;		
	int modemInterface[MAX_SUPPORT_INTERFACE];

	char  modemName[USB_MODEM_NAME_LEN];
	char deviceName[MAX_DEVICE_NAME_LEN]; 
	int modemSignalQua;
	int simLock;
	int modemChipType;
	u_short targetVendorID; 
	u_short targetProductID;
    	int ISP;    
	int modemStatus;
}modem_info;
/*end add*/
#endif

/** Data body for CMS_MSG_REGISTER_DELAYED_MSG.
 */
typedef struct
{
   UINT32  delayMs; /**< Number of milliseconds in the future to deliver this message. */

} RegisterDelayedMsgBody;


/** Data body for CMS_MSG_DHCPC_STATE_CHANGED message type.
 *
 */
typedef struct
{
   UBOOL8 addressAssigned; /**< Have we been assigned an IP address ? */
   UBOOL8 isExpired; /**< Is the lease time expired ? */
   char ip[BUFLEN_32];   /**< New IP address, if addressAssigned==TRUE */
   char mask[BUFLEN_32]; /**< New netmask, if addressAssigned==TRUE */
   char gateway[BUFLEN_32];    /**< New gateway, if addressAssigned==TRUE */
   char nameserver[BUFLEN_64]; /**< New nameserver, if addressAssigned==TRUE */
   char ntpserver[BUFLEN_64];  /**< New ntp server(s), if addressAssigned==TRUE */
   UINT8 ipv4MaskLen; /**< For 6rd parameters (option 212) */
   UINT8 ipv6PrefixLen;
   char prefix[BUFLEN_48];
   char brAddr[BUFLEN_32];
   char hostName[BUFLEN_32];
   char domain[BUFLEN_32];
   char acsURL[CMS_MAX_ACS_URL_LENGTH];    /**< dhcp server may provide this */
   char acsProvisioningCode[CMS_MAX_ACS_PROVISIONING_CODE_LENGTH];  /**< dhcp server may provide this */
   UINT32 cwmpRetryMinimumWaitInterval; /**< dhcp server may provide this */
   UINT32 cwmpRetryIntervalMultiplier; /**< dhcp server may provide this */
} DhcpcStateChangedMsgBody;


/** Data body for CMS_MSG_DHCP6C_STATE_CHANGED message type.
 *
 */
typedef struct
{
   UBOOL8 prefixAssigned;  /**< Have we been assigned a site prefix ? */
   UBOOL8 addrAssigned;    /**< Have we been assigned an IPv6 address ? */
   UBOOL8 dnsAssigned;     /**< Have we been assigned dns server addresses ? */
   UBOOL8 domainNameAssigned;     /**< Have we been assigned dns server addresses ? */
   UBOOL8 aftrAssigned;     /**< Have we been assigned aftr name ? */
   char sitePrefix[BUFLEN_48];   /**< New site prefix, if prefixAssigned==TRUE */
   UINT32 prefixPltime;
   UINT32 prefixVltime;
   UINT32 prefixCmd;
   char ifname[BUFLEN_32];
   char address[BUFLEN_48];      /**< New IPv6 address, if addrAssigned==TRUE */
   char pdIfAddress[BUFLEN_48];      /**< New IPv6 address of PD interface */
   UINT32 addrCmd;
   char nameserver[BUFLEN_128];  /**< New nameserver, if addressAssigned==TRUE */
   char domainName[BUFLEN_64];  /**< New domain Name, if addressAssigned==TRUE */
   char ntpserver[BUFLEN_128];  /**< New ntp server(s), dhcp server may provide this */
   char acsURL[CMS_MAX_ACS_URL_LENGTH];      /**< dhcp server may provide this */
   char acsProvisioningCode[CMS_MAX_ACS_PROVISIONING_CODE_LENGTH];  /** dhcp server may provide this */
   UINT32 cwmpRetryMinimumWaitInterval; /**< dhcp server may provide this */
   UINT32 cwmpRetryIntervalMultiplier; /**< dhcp server may provide this */
   char aftr[CMS_AFTR_NAME_LENGTH];      /**< dhcp server may provide this */
} Dhcp6cStateChangedMsgBody;


/** Data body for CMS_MSG_RASTATUS6_INFO message type.
 *
 */
typedef struct
{
   char pio_prefix[CMS_IPADDR_LENGTH];  /**< prefix info in PIO, we only support one prefix in one RA message */
   UINT8 pio_prefixLen;
   UINT32 pio_plt;
   UINT32 pio_vlt;
   UBOOL8 pio_A_flag;
   UBOOL8 pio_L_flag;
   char dns_servers[CMS_IPADDR_LENGTH*2];  /**< RFC6106, we only support up to two DNS servers in one RA message */
   UINT32 dns_lifetime;
   char domainName[BUFLEN_32];  /**< RFC6106, we only support up to 32 characters in one RA message */
   UINT32 domainName_lifetime;
   char router[BUFLEN_40];  /**< source IP of the RA message */
   UINT32 router_lifetime;
   UBOOL8 router_M_flags;
   UBOOL8 router_O_flags;
   char ifName[BUFLEN_32];  /** < the interface which receives the RA */
} RAStatus6MsgBody;


/*!\PPPOE state defines 
 * (was in syscall.h before)
 */

#define BCM_PPPOE_CLIENT_STATE_PADO          0   /* waiting for PADO */
#define BCM_PPPOE_CLIENT_STATE_PADS          1   /* got PADO, waiting for PADS */
#define BCM_PPPOE_CLIENT_STATE_CONFIRMED     2   /* got PADS, session ID confirmed */
#define BCM_PPPOE_CLIENT_STATE_DOWN          3   /* totally down */
#define BCM_PPPOE_CLIENT_STATE_UP            4   /* totally up */
#define BCM_PPPOE_SERVICE_AVAILABLE          5   /* ppp service is available on the remote */
#define BCM_PPPOE_AUTH_FAILED                7
#define BCM_PPPOE_RETRY_AUTH                 8
#define BCM_PPPOE_REPORT_LASTCONNECTERROR    9
#define BCM_PPPOE_CLIENT_STATE_UNCONFIGURED   10 
#define BCM_PPPOE_CLIENT_IPV6_STATE_UP   11
#define BCM_PPPOE_CLIENT_IPV6_STATE_DOWN   12
#define BCM_PPPOE_DEMAND_SET_ROUTE       13  /* for ppp-demand , by xieping*/ 
#define BCM_PPPOE_3G_CHANGE_INTERFACE          14   /* For 3g fail to connect, by hanjiayan */

/** These values are returned in the wordData field of the response msg to
 *  CMS_MSG_GET_WAN_LINK_STATUS.
 *  See dslIntfStatusValues in cms-data-model.xml
 * There is a bit of confusion here.  These values are associated with the
 * WANDSLInterfaceConfig object, but usually, a caller is asking about
 * a WANDSLLinkConfig object. For now, the best thing to do is just check
 * for WAN_LINK_UP.  All other values basically mean the requested link is
 * not up.
 */
#define WAN_LINK_UP                   0
#define WAN_LINK_INITIALIZING         1
#define WAN_LINK_ESTABLISHINGLINK     2
#define WAN_LINK_NOSIGNAL             3
#define WAN_LINK_ERROR                4
#define WAN_LINK_DISABLED             5

#define LAN_LINK_UP                   0
#define LAN_LINK_DISABLED             1


/** Data body for CMS_MSG_PPPOE_STATE_CHANGED message type.
 *
 */
typedef struct
{
   UINT8 pppState;       /**< pppoe states */
   char ip[BUFLEN_32];   /**< New IP address, if pppState==BCM_PPPOE_CLIENT_STATE_UP */
   char mask[BUFLEN_32]; /**< New netmask, if pppState==BCM_PPPOE_CLIENT_STATE_UP */
   char gateway[BUFLEN_32];    /**< New gateway, if pppState==BCM_PPPOE_CLIENT_STATE_UP */
   char nameserver[BUFLEN_64]; /**< New nameserver, if pppState==BCM_PPPOE_CLIENT_STATE_UP */
   char servicename[BUFLEN_256]; /**< service name, if pppState==BCM_PPPOE_CLIENT_STATE_UP */
   char ppplastconnecterror[PPP_CONNECT_ERROR_REASON_LEN];
} PppoeStateChangeMsgBody;


/*!\enum NetworkAccessMode
 * \brief Different types of network access modes, returned by cmsDal_getNetworkAccessMode
 *        and also in the wordData field of CMS_MSG_GET_NETWORK_ACCESS_MODE
 */
typedef enum {
   NETWORK_ACCESS_DISABLED   = 0,  /**< access denied */
   NETWORK_ACCESS_LAN_SIDE   = 1,  /**< access from LAN */
   NETWORK_ACCESS_WAN_SIDE   = 2,  /**< access from WAN */
   NETWORK_ACCESS_CONSOLE    = 3   /**< access from serial console */
} NetworkAccessMode;


/** Data body for CMS_MSG_DHCPD_HOST_INFO message type.
 *
 */
typedef struct
{
   UBOOL8 deleteHost;  /**< TRUE if we are deleting a LAN host, otherwise we are adding or editing LAN host */
   SINT32 leaseTimeRemaining;      /** Number of seconds left in the lease, -1 means no expiration */
   char ifName[CMS_IFNAME_LENGTH]; /**< brx which this host is on */
   char ipAddr[BUFLEN_48];         /**< IP Address of the host */
   char macAddr[MAC_STR_LEN+1];    /**< mac address of the host */
   char addressSource[BUFLEN_32];  /** source of IP address assignment, same value as
                                     * LANDevice.{i}.Hosts.Host.{i}.addressSource */
   char interfaceType[BUFLEN_32];  /** type of interface used by LAN host, same values as 
                                     * LANDevice.{i}.Hosts.Host.{i}.InterfaceType */
   char hostName[BUFLEN_64];       /** Both dhcpd and data model specify hostname as 64 bytes */
   char oui[BUFLEN_8];             /** Host's manufacturing OUI */
   char serialNum[BUFLEN_64];      /** Host's serial number */
   char productClass[BUFLEN_64];   /** Host's product class */
} DhcpdHostInfoMsgBody;


/** Data body for CMS_MSG_GET_LEASE_TIME_REMAINING message type.
 *
 * The lease time remaing is returned in the wordData field of the
 * response message.  A -1 means the lease does not expire.
 * A 0 could mean the lease is expired, or that dhcpd has not record
 * of the mac address that was given.
 *
 */
typedef struct
{
   char ifName[CMS_IFNAME_LENGTH]; /**< brx which this host is on */
   char macAddr[MAC_STR_LEN+1];    /**< mac address of the host */
} GetLeaseTimeRemainingMsgBody;



typedef enum 
{
   VOICE_DIAG_CFG_SHOW           = 0,
   VOICE_DIAG_EPTCMD             = 1,
   VOICE_DIAG_STUNLKUP           = 2,
   VOICE_DIAG_STATS_SHOW         = 3,
   VOICE_DIAG_PROFILE            = 4,
   VOICE_DIAG_EPTAPP_HELP        = 5,
   VOICE_DIAG_EPTAPP_SHOW        = 6,
   VOICE_DIAG_EPTAPP_CREATECNX   = 7,
   VOICE_DIAG_EPTAPP_DELETECNX   = 8,
   VOICE_DIAG_EPTAPP_MODIFYCNX   = 9,
   VOICE_DIAG_EPTAPP_EPTSIG      = 10,
   VOICE_DIAG_EPTAPP_SET         = 11,
   VOICE_DIAG_EPTPROV            = 12,
   VOICE_DIAG_EPTAPP_DECTTEST    = 13,
   VOICE_DIAG_DECT_MEM_SET       = 14,
   VOICE_DIAG_DECT_MEM_GET       = 15,
   VOICE_DIAG_DECT_MODE_SET      = 16,
   VOICE_DIAG_DECT_MODE_GET      = 17,
   VOICE_DIAG_EPTPROBE           = 18,
} VoiceDiagType;

/** Data body for Voice diagonistic message */
typedef struct
{
  VoiceDiagType type;
  char cmdLine[BUFLEN_128];  
} VoiceDiagMsgBody;

/** Data body for Voice Line Obj */
typedef struct
{
   char regStatus[BUFLEN_128];
   char callStatus[BUFLEN_128];
} VoiceLineObjStatus;


/** Data body for CMS_MSG_PING_DATA message type.
 *
 */
typedef struct
{
   char diagnosticsState[BUFLEN_32];  /**< Ping state: requested, none, completed... */
   char interface[BUFLEN_32];   /**< interface on which ICMP request is sent */
   char host[BUFLEN_32]; /**< Host -- either IP address form or hostName to send ICMP request to */
   UINT32 numberOfRepetitions; /**< Number of ICMP requests to send */
   UINT32    timeout;	/**< Timeout in seconds */
   UINT32    dataBlockSize;	/**< DataBlockSize  */
   UINT32    DSCP;	/**< DSCP */
   UINT32    successCount;	/**< SuccessCount */
   UINT32    failureCount;	/**< FailureCount */
   UINT32    averageResponseTime;	/**< AverageResponseTime */
   UINT32    minimumResponseTime;	/**< MinimumResponseTime */
   UINT32    maximumResponseTime;	/**< MaximumResponseTime */
   CmsEntityId requesterId;
}PingDataMsgBody;



/** Data body for the CMS_MSG_WATCH_WAN_CONNECTION message type.
 *
 */
 typedef struct
{
   MdmObjectId oid;              /**< Object Identifier */
   InstanceIdStack iidStack;     /**< Instance Id Stack. for the ip/ppp Conn Object */
   UBOOL8 isAdd;                 /**< add  wan connection to ssk list if TRUE.  */
   UBOOL8 isStatic;              /**< If TRUE, it is is bridge, static IPoE and IPoA, FALSE: pppox or dynamic IPoE */   
   UBOOL8 isDeleted;             /**< Used for auto detect feature only.  If TRUE, the wan interface is removed.*/  
   UBOOL8 isAutoDetectChange;    /**< Used for auto detect feature only.  If TRUE, there is a change on the layer 2 auto detect flag */  
} WatchedWanConnection;

/*
 * Data body for the CMS_MSG_DHCPD_DENY_VENDOR_ID message type
 */
typedef struct
{
   long deny_time; /* Return from time(), in host order */
   unsigned char chaddr[16]; /* Usually the MAC address */
   char vendor_id[BUFLEN_256]; /**< max length in RFC 2132 is 255 bytes, add 1 for null terminator */
   char ifName[CMS_IFNAME_LENGTH];  /**< The interface that dhcpd got this msg on */
}DHCPDenyVendorID;

/*
 * Data body for CMS_MSG_GET_DEVICE_INFO message type.
 */
typedef struct
{
   char oui[BUFLEN_8];              /**< manufacturer OUI of device */
   char serialNum[BUFLEN_64];       /**< serial number of device */
   char productClass[BUFLEN_64];    /**< product class of device */
} GetDeviceInfoMsgBody;


typedef enum 
{
   USER_REQUEST_CONNECTION_DOWN  = 0,
   USER_REQUEST_CONNECTION_UP    = 1
} UserConnectionRequstType;


/*
 * Data body for CMS_MSG_WATCH_DSL_LOOP_DIAG message type.
 */
typedef struct
{
   InstanceIdStack iidStack;
} dslDiagMsgBody;


/** Data body for CMS_MSG_VENDOR_CONFIG_UPDATE message type.
 *
 */
typedef struct
{
   char name[BUFLEN_64];              /**< name of configuration file */
   char version[BUFLEN_16];           /**< version of configuration file */
   char date[BUFLEN_64];              /**< date when config is updated */
   char description[BUFLEN_256];      /**< description of config file */
} vendorConfigUpdateMsgBody;

typedef enum
{
    MCAST_INTERFACE_ADD = 0,
    MCAST_INTERFACE_DELETE
} McastInterfaceAction;

typedef enum
{
    OMCI_IGMP_PHY_NONE = 0,
    OMCI_IGMP_PHY_ETHERNET,
    OMCI_IGMP_PHY_MOCA,
    OMCI_IGMP_PHY_WIRELESS,
    OMCI_IGMP_PHY_POTS,
    OMCI_IGMP_PHY_GPON
} OmciIgmpPhyType;

typedef enum
{
    OMCI_IGMP_MSG_JOIN = 0,
    OMCI_IGMP_MSG_RE_JOIN,
    OMCI_IGMP_MSG_LEAVE
} OmciIgmpMsgType;

/** Data body for CMS_MSG_OMCI_IGMP_ADMISSION_CONTROL message type.
 *
 */
typedef struct
{
    UINT16  tci;
    UINT32  sourceIpAddress;
    UINT32  groupIpAddress;
    UINT32  clientIpAddress;
    UINT16  phyPort;
    OmciIgmpPhyType phyType;
    OmciIgmpMsgType msgType;
} OmciIgmpMsgBody;


typedef enum
{
    OMCI_SERVICE_UNICAST = 0, 
    OMCI_SERVICE_MULTICAST,
    OMCI_SERVICE_BROADCAST
} OmciGponWanServiceType;

/** Data body for message types :
 *  CMS_MSG_GET_NTH_GPON_WAN_LINK_IF_NAME
 *  CMS_MSG_OMCI_GPON_WAN_SERVICE_STATUS_CHANGE
 *  CMS_MSG_OMCI_RG_WAN_SERVICE_STAUTS_CHANGE
 *
 */
typedef struct {
   SINT32 pbits;             /**< L2 gpon ifname vlan p-bits */
   SINT32 vlanId;            /**< L2 gpon ifname vlan ID */
   UBOOL8 noMcastVlanFilter; /**< noMcastVlanFilter */
   UBOOL8 serviceStatus;     /**< If TRUE, gpon link or RG-WAN-service is created already */
   UBOOL8 igmpEnabled;       /**< Is IGMP Snooping/Proxy enabled on this WAN service. */
} GponWanServiceParams;

typedef struct {
   UINT32 gemPortIndex;               /**< GEM port index - RG-Full only. RG-Light uses the gemIdxArrayStruct in gponInterfaceArray  */   
   UINT16 portID;                     /**< It is a logical port ID which maps o the gem port index. eg. 3000 maps to gem port 0 */  
   OmciFLowDirection flowDirection;   /**< Gem Port flow direction */
   OmciGponWanServiceType serviceType;/**< Type of GemPort - Bi-directional, Multicast or Broadcast  */
} GponWanLinkParams;

typedef struct {
    char   l2Ifname[CMS_IFNAME_LENGTH];
    GponWanServiceParams serviceParams;
    GponWanLinkParams    linkParams;
} OmciServiceMsgBody;

/** Data body for CMS_MSG_PING_DATA message type of OMCI.
 *
 */
typedef struct
{
   UINT16 tcid;                /**< transaction id of omci ME*/
   UINT16 result;              /**< ping result */
   union 
   {
     UINT16 responseTime[15];  /**< response time */
     UINT8 icmpReply[30];      /**< pkt of unexpected ICMP response */
   } msg;
}OmciPingDataMsgBody;

/** Data body for CMS_MSG_TRACERT_DATA message type of OMCI.
 *
 */
typedef struct
{
   UINT16 tcid;                /**< transaction id of omci ME*/
   UINT16 result;              /**< traceroute result */ 
   UINT16 hops;                /**< neighbour count */
   union 
   {
     UINT32 neighbour[7];      /**< neighbour list */    
     UINT8 icmpReply[30];      /**< pkt of unexpected ICMP response */
   } msg;
}OmciTracertDataMsgBody;

/** Data body for CMS_MSG_APP_TERMINATED message type.
 *
 */
typedef struct
{
   CmsEntityId eid;      /**< Entity id of the exit process */
   SINT32 sigNum;        /**< signal number */   
   SINT32 exitCode;      /**< process exit code */   
} appTermiatedMsgBody;


/** Data body for the CMS_MSG_GET_NTH_GPON_WAN_LINK_IF_NAME
 *
 */
typedef struct
{
   UINT32 linkEntryIdx;       /**< Nth WAN link entry - send in request */
   char   L2Ifname[CMS_IFNAME_LENGTH];/**< L2 gpon ifname - send in response */
   GponWanServiceParams serviceParams;
} GponNthWanLinkInfo;

/** Data body for CMS_MSG_REQUEST_DU_STATE_CHANGE message type.
 *
 */
typedef struct
{
   char operation[BUFLEN_32];   /**< Install, Update, Uninstall, install_at_bootup */
   char URL[BUFLEN_1024]; /**< Location of DU to be installed/update */
   char UUID[BUFLEN_40];    /**< Unique ID to describe DU,36 bytes */
   char username[BUFLEN_256]; /**< Optional username for file server */
   char password[BUFLEN_256]; /**< Optional password for file server */
   char executionEnv[BUFLEN_256]; /**< Environment to execute EU */
   char version[BUFLEN_32]; /**< Version of DU */
   char commandKey[BUFLEN_32]; /**< Command Key of op request */
   UINT16 reqId;
   int FaultCode;
} DUrequestStateChangedMsgBody;

/*!\Software modules operation defines 
 */
#define SW_MODULES_OPERATION_INSTALL              "install"
#define SW_MODULES_OPERATION_UNINSTALL            "uninstall"
#define SW_MODULES_OPERATION_UPDATE               "update"
#define SW_MODULES_OPERATION_START                "start"
#define SW_MODULES_OPERATION_STOP                 "stop"
#define SW_MODULES_OPERATION_INSTALL_AT_BOOTUP    "installBootup"
#define SW_MODULES_OPERATION_START_AT_BOOTUP      "startBootup"
#define SW_MODULES_OPERATION_LB                   "LB"

/** Data body for CMS_MSG_RESPONSE_DU_STATE_CHANGE message type.
 *
 */
typedef struct
{
   char operation[BUFLEN_32];   /**< Install, Update, Uninstall, install_at_bootup */
   char URL[BUFLEN_1024]; /**< Location of DU to be installed/update */
   char UUID[BUFLEN_40];    /**< Unique ID to describe DU,36 bytes */
   char username[BUFLEN_256]; /**< Optional username for file server */
   char password[BUFLEN_256]; /**< Optional password for file server */
   UINT32 DUref; /**< Local DU index in MDM */
   char version[BUFLEN_32]; /**< Version of DU */
   char currentState[BUFLEN_32]; /**< Version of DU */
   UBOOL8 resolved; /**<dependency resolved or not */
   char EUlist[BUFLEN_1024]; /**< list of comma seperated EU bundle ID */
   char DUlist[BUFLEN_256];  /**< list of comma seperated DU bundle ID */
   char startTime[BUFLEN_64]; /**< time when DU request operation started */
   char completeTime[BUFLEN_64]; /**< time when DU request operation completed */
   int faultCode;
   char faultString[BUFLEN_64]; /**< DU operation fault string */
   char commandKey[BUFLEN_32]; /**< Command Key of op request */
   UINT16 reqId;
} DUresponseStateChangedMsgBody;

/** Data body for CMS_MSG_REQUEST_EU_STATE_CHANGE message type.
 *
 */
typedef struct
{
   char operation[BUFLEN_32];   /**< Install, Update, Uninstall */
   char name[BUFLEN_32]; /**< Name of EU */
   char alias[BUFLEN_32]; /**< Alias Name of EU */
   char euid[BUFLEN_64]; /**< EUID of EU */
   char status[BUFLEN_32]; /**< Status of EU; idle, starting, active and stopping */
   char description[BUFLEN_128]; /**< Description of EU */
} EUrequestStateChangedMsgBody;

/** Data body for the reply of CMS_MSG_DNSPROXY_GET_STATS
 *
 */
 typedef struct
{
   UINT32 dnsErrors; 	      /**< dns query error counter  */
} DnsGetStatsMsgBody;

/* 
 * Data body for CMS_MSG_IPSEC_CFG_CHANGED message type.
 */
typedef struct
{
	unsigned int  local_ip_mode;
	unsigned int  remote_ip_mode;
	unsigned int  entryID;
	unsigned int  op;
	unsigned char  enable;
	char local_ip[BUFLEN_16];
	char local_mask[BUFLEN_16];	
	char remote_ip[BUFLEN_16];
	char remote_mask[BUFLEN_16];
	char real_remote_gw_ip[BUFLEN_16];
	char spi[BUFLEN_16];
	char second_spi[BUFLEN_16];
	InstanceIdStack stack;
}IpsecCfgChangedMsgBody;

/* 
 * Data body for CMS_MSG_IPSEC_WAN_CHANGED message type.
 */
typedef struct
{
	char ifName[BUFLEN_32];
	int  isUP;  
}IPSecWanChangedMsgBody;

/* 
 * Data body for CMS_MSG_CLI_WLAN_GETVAR message type.
 */
typedef struct
{
	char varName[BUFLEN_32];
	UINT32 varSize;
} CliWlanGetVarMsgBody;

/*!\SNTP state defines 
 */
#define SNTP_STATE_DISABLED                0   
#define SNTP_STATE_UNSYNCHRONIZED          1   
#define SNTP_STATE_SYNCHRONIZED            2
#define SNTP_STATE_FAIL_TO_SYNCHRONIZE     3
#define SNTP_STATE_ERROR                   4

#ifdef __cplusplus
extern "C" {
#endif

/** Initialize messaging system.
 *
 * This function should be called early in startup.
 * 
 * @param eid       (IN)  Entity id of the calling process.
 * @param msgHandle (OUT) On successful return, this will point
 *                        to a msg_handle which should be used in subsequent messaging calls.
 *                        The caller is responsible for freeing the msg_handle by calling
 *                        cmsMsg_cleanup().
 *
 * @return CmsRet enum.
 */
CmsRet cmsMsg_init(CmsEntityId eid, void **msgHandle);


/** Clean up messaging system.
 *
 * This function should be called before the application exits.
 * @param msgHandle (IN) This was the msg_handle that was
 *                       created by cmsMsg_init().
 */
void cmsMsg_cleanup(void **msgHandle);


/** Send a message (blocking).
 *
 * This call is potentially blocking if the communcation channel is
 * clogged up, but in practice, it will not block.  If blocking becomes
 * a real problem, we can create a non-blocking version of this function.
 *
 * @param msgHandle (IN) This was the msgHandle created by
 *                       cmsMsg_init().
 * @param buf       (IN) This buf contains a CmsMsgHeader and possibly
 *                       more data depending on the message type.
 *                       The caller must fill in all the fields.
 *                       The total length of the message is the length
 *                       of the message header plus any additional data
 *                       as specified in the dataLength field in the CmsMsgHeader.
 * @return CmsRet enum.
 */
CmsRet cmsMsg_send(void *msgHandle, const CmsMsgHeader *buf);


/** Send a reply/response message to the given request message.
 *
 * Same notes about blocking from cmsMsg_send() apply.
 * Note that only a message header will be sent by this
 * function.  If the initial request message contains additional
 * data, this function will not send that data back.
 *
 * @param msgHandle (IN) This was the msgHandle created by
 *                       cmsMsg_init().
 * @param msg       (IN) The request message that we want to send
 *                       a response to.  This function does not modify
 *                       or free this message.  Caller is still required
 *                       to deal with it appropriately.
 * @param retCode   (IN) The return code to put into the wordData
 *                       field of the return message.
 * @return CmsRet enum.
 */
CmsRet cmsMsg_sendReply(void *msgHandle, const CmsMsgHeader *msg, CmsRet retCode);


/** Send a message and wait for a simple response.
 *
 * This function starts out by calling cmsMsg_send().
 * Then it waits for a response.  The result of the response is expected in
 * the wordData field of the response message.  The value in the wordData is
 * returned to the caller.  The response message must not have any additional
 * data after the header.  The response message is freed by this function.
 *
 * @param msgHandle (IN) This was the msgHandle created by
 *                       cmsMsg_init().
 * @param buf       (IN) This buf contains a CmsMsgHeader and possibly
 *                       more data depending on the message type.
 *                       The caller must fill in all the fields.
 *                       The total length of the message is the length
 *                       of the message header plus any additional data
 *                       as specified in the dataLength field in the CmsMsgHeader.
 *
 * @return CmsRet enum, which is either the response code from the recipient of the msg,
 *                      or a local error.
 */
CmsRet cmsMsg_sendAndGetReply(void *msgHandle, const CmsMsgHeader *buf);


/** Send a message and wait up to a timeout time for a simple response.
 *
 * This function is the same as cmsMsg_sendAndGetReply() except there
 * is a limit, expressed as a timeout, on how long this function will
 * wait for a response.
 *
 * @param msgHandle (IN) This was the msgHandle created by
 *                       cmsMsg_init().
 * @param buf       (IN) This buf contains a CmsMsgHeader and possibly
 *                       more data depending on the message type.
 *                       The caller must fill in all the fields.
 *                       The total length of the message is the length
 *                       of the message header plus any additional data
 *                       as specified in the dataLength field in the CmsMsgHeader.
 * @param timeoutMilliSeconds (IN) Timeout in milliseconds.
 *
 * @return CmsRet enum, which is either the response code from the recipient of the msg,
 *                      or a local error.
 */
CmsRet cmsMsg_sendAndGetReplyWithTimeout(void *msgHandle,
                                         const CmsMsgHeader *buf,
                                         UINT32 timeoutMilliSeconds);


/** Send a message and wait up to a timeout time for a response that can
 *	have a data section.
 *
 * This function is the same as cmsMsg_sendAndGetReply() except this
 * returns a cmsMsgHeader and a data section if applicable.
 *
 * @param msgHandle (IN) This was the msgHandle created by
 *                       cmsMsg_init().
 * @param buf       (IN) This buf contains a CmsMsgHeader and possibly
 *                       more data depending on the message type.
 *                       The caller must fill in all the fields.
 *                       The total length of the message is the length
 *                       of the message header plus any additional data
 *                       as specified in the dataLength field in the CmsMsgHeader.
 * @param replyBuf (IN/OUT) On entry, replyBuf is the address of a pointer to
 *                       a buffer that will hold the reply message.
 *                       The caller must allocate enough space in the replyBuf
 *                       to hold the message header and any data that might
 *                       come back in the reply message.  (This is a dangerous
 *                       interface!  This function does not verify that the
 *                       caller has allocated enough space to hold the reply
 *                       message.  Memory corruption will occur if the reply
 *                       message contains more data than the caller has
 *                       allocated.  Note there is also no reason for this
 *                       parameter to be address of pointer, a simple pointer
 *                       to replyBuf would have been sufficient.)
 *                       On successful return, replyBuf will point to a
 *                       CmsMsgHeader possibly followed by more data if the
 *                       reply message contains a data section.
 *
 * @return CmsRet enum, which is either the response code from the recipient of the msg,
 *                      or a local error.
 */
CmsRet cmsMsg_sendAndGetReplyBuf(void *msgHandle, 
                                 const CmsMsgHeader *buf,
                                 CmsMsgHeader **replyBuf);


/** Send a message and wait up to a timeout time for a response that can
 *	have a data section.
 *
 * This function is the same as cmsMsg_sendAndGetReplyBuf() except there
 * is a limit, expressed as a timeout, on how long this function will
 * wait for a response.
 *
 * @param msgHandle (IN) This was the msgHandle created by
 *                       cmsMsg_init().
 * @param buf       (IN) This buf contains a CmsMsgHeader and possibly
 *                       more data depending on the message type.
 *                       The caller must fill in all the fields.
 *                       The total length of the message is the length
 *                       of the message header plus any additional data
 *                       as specified in the dataLength field in the CmsMsgHeader.
 * @param replyBuf (IN/OUT) On entry, replyBuf is the address of a pointer to
 *                       a buffer that will hold the reply message.
 *                       The caller must allocate enough space in the replyBuf
 *                       to hold the message header and any data that might
 *                       come back in the reply message.  (This is a dangerous
 *                       interface!  This function does not verify that the
 *                       caller has allocated enough space to hold the reply
 *                       message.  Memory corruption will occur if the reply
 *                       message contains more data than the caller has
 *                       allocated.  Note there is also no reason for this
 *                       parameter to be address of pointer, a simple pointer
 *                       to replyBuf would have been sufficient.)
 *                       On successful return, replyBuf will point to a
 *                       CmsMsgHeader possibly followed by more data if the
 *                       reply message contains a data section.
 *
 * @param timeoutMilliSeconds (IN) Timeout in milliseconds.
 *
 * @return CmsRet enum, which is either the response code from the recipient of the msg,
 *                      or a local error.
 */
CmsRet cmsMsg_sendAndGetReplyBufWithTimeout(void *msgHandle, 
                                            const CmsMsgHeader *buf,
                                            CmsMsgHeader **replyBuf,
                                            UINT32 timeoutMilliSeconds);


/** Receive a message (blocking).
 *
 * This call will block until a message is received.
 * @param msgHandle (IN) This was the msgHandle created by cmsMsg_init().
 * @param buf      (OUT) On successful return, buf will point to a CmsMsgHeader
 *                       and possibly followed by more data depending on msg type.
 *                       The caller is responsible for freeing the message by calling
 *                       cmsMsg_free().
 * @return CmsRet enum.
 */
CmsRet cmsMsg_receive(void *msgHandle, CmsMsgHeader **buf);


/** Receive a message with timeout.
 *
 * This call will block until a message is received or until the timeout is reached.
 *
 * @param msgHandle (IN) This was the msgHandle created by cmsMsg_init().
 * @param buf      (OUT) On successful return, buf will point to a CmsMsgHeader
 *                       and possibly followed by more data depending on msg type.
 *                       The caller is responsible for freeing the message by calling
 *                       cmsMsg_free().
 * @param timeoutMilliSeconds (IN) Timeout in milliseconds.  0 means do not block,
 *                       otherwise, block for the specified number of milliseconds.
 * @return CmsRet enum.
 */
CmsRet cmsMsg_receiveWithTimeout(void *msgHandle,
                                 CmsMsgHeader **buf,
                                 UINT32 timeoutMilliSeconds);


/** Put a received message back into a temporary "put-back" queue.
 *
 * Since the RCL calls cmsMsg_receive, it may get an asynchronous event
 * message that is intended for the higher level application.  So it needs
 * to preserve the message in the msgHandle so the higher level application
 * can detect and receive it.  This happens in two steps: first the message
 * is put in a temporary "put-back" queue in the msgHandle (this function),
 * and then all messages in the put-back queue are sent smd with the
 * requeue bit set.  Smd will send the message back to this app again
 * therefore allowing the upper level application to receive it.
 *
 * @param msgHandle (IN) This was the msgHandle created by cmsMsg_init().
 * @param buf       (IN) The message to put back.
 */
void cmsMsg_putBack(void *msgHandle, CmsMsgHeader **buf);


/** Cause all messages in the put-back queue to get requeued in the 
 *  msgHandle's communications link.
 *
 * See the comments in cmsMsg_putBack() for a description of how
 * this function works in conjunction with cmsMsg_putBack().
 *
 * @param msgHandle (IN) This was the msgHandle created by cmsMsg_init().
 */
void cmsMsg_requeuePutBacks(void *msgHandle);


/** Make a copy of the specified message, including any additional data beyond the header.
 *
 * @param  buf      (IN) The message to copy.
 * @return duplicate of the specified message.
 */
CmsMsgHeader *cmsMsg_duplicate(const CmsMsgHeader *buf);



/** Get operating system dependent handle to detect available message to receive.
 *
 * This allows the application to get the operating system dependent handle
 * to detect a message that is available to be received so it can wait on the handle along
 * with other private event handles that the application manages.
 * In UNIX like operating systems, this will return a file descriptor
 * which the application can then use in select.
 * 
 * @param msgHandle    (IN) This was the msgHandle created by cmsMsg_init().
 * @param eventHandle (OUT) This is the OS dependent event handle.  For LINUX,
 *                          eventHandle is the file descriptor number.
 * @return CmsRet enum.
 */
CmsRet cmsMsg_getEventHandle(const void *msgHandle, void *eventHandle);


/** Get the eid of the creator of this message handle.
 * 
 * This function is used by the CMS libraries which are given a message handle
 * but needs to find out who the message handle belongs to.
 * 
 * @param msgHandle    (IN) This was the msgHandle created by cmsMsg_init().
 * 
 * @return CmsEntityId of the creator of the msgHandle.
 */
CmsEntityId cmsMsg_getHandleEid(const void *msgHandle);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif // __CMS_MSG_H__
