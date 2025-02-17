///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000-2003 Intel Corporation 
// All rights reserved. 
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met: 
//
// * Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer. 
// * Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution. 
// * Neither name of Intel Corporation nor the names of its contributors 
// may be used to endorse or promote products derived from this software 
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#include "config.h"
#if EXCLUDE_SSDP == 0

#include "membuffer.h"
#include "ssdplib.h"
#include <stdio.h>
#include "ThreadPool.h"
#include "miniserver.h"

#include "upnpapi.h"
#include "httpparser.h"
#include "httpreadwrite.h"

#ifdef WIN32
	#include <string.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include "unixutil.h"
#endif

#define MAX_TIME_TOREAD  45

CLIENTONLY( SOCKET gSsdpReqSocket = 0; )

void RequestHandler();
Event ErrotEvt;

enum Listener { Idle, Stopping, Running };

unsigned short ssdpStopPort;

struct SSDPSockArray {
	// socket for incoming advertisments and search requests
	int ssdpSock;
	// socket for sending search requests and receiving search replies
	CLIENTONLY( int ssdpReqSock; )
};

#ifdef INCLUDE_DEVICE_APIS
#if EXCLUDE_SSDP == 0

/************************************************************************
 * Function : AdvertiseAndReply
 *
 * Parameters:
 *	IN int AdFlag:
 *		-1 = Send shutdown,
 *		 0 = send reply, 
 *		 1 = Send Advertisement
 *	IN UpnpDevice_Handle Hnd: Device handle
 *	IN enum SsdpSearchType SearchType:Search type for sending replies
 *	IN struct sockaddr_in *DestAddr:Destination address
 *   IN char *DeviceType:Device type
 *	IN char *DeviceUDN:Device UDN
 *   IN char *ServiceType:Service type
 *	IN int Exp:Advertisement age
 *
 * Description:
 *	This function sends SSDP advertisements, replies and shutdown messages.
 *
 * Returns: int
 *	UPNP_E_SUCCESS if successful else appropriate error
 ***************************************************************************/
int AdvertiseAndReply( IN int AdFlag,
                       IN UpnpDevice_Handle Hnd,
                       IN enum SsdpSearchType SearchType,
                       IN struct sockaddr_in *DestAddr,
                       IN char *DeviceType,
                       IN char *DeviceUDN,
                       IN char *ServiceType,
                       int Exp )
{
    int i,
      j;
    int defaultExp = DEFAULT_MAXAGE;
    struct Handle_Info *SInfo = NULL;
    char UDNstr[100],
      devType[100],
      servType[100];
    IXML_NodeList *nodeList = NULL;
    IXML_NodeList *tmpNodeList = NULL;
    IXML_Node *tmpNode = NULL;
    IXML_Node *tmpNode2 = NULL;
    IXML_Node *textNode = NULL;
    const DOMString tmpStr;
    char SERVER[200];

    const DOMString dbgStr;
    UpnpPrintf( UPNP_ALL, API, __FILE__, __LINE__,
        "Inside AdvertiseAndReply with AdFlag = %d\n",
        AdFlag );

    // Use a read lock
    HandleReadLock();
    if( GetHandleInfo( Hnd, &SInfo ) != HND_DEVICE ) {
        HandleUnlock();
        return UPNP_E_INVALID_HANDLE;
    }
    defaultExp = SInfo->MaxAge;

    //get server info

    get_sdk_info( SERVER );

    // parse the device list and send advertisements/replies 
    for( i = 0;; i++ ) {

        UpnpPrintf( UPNP_ALL, API, __FILE__, __LINE__,
            "Entering new device list with i = %d\n\n", i );

        tmpNode = ixmlNodeList_item( SInfo->DeviceList, i );
        if( tmpNode == NULL ) {
            UpnpPrintf( UPNP_ALL, API, __FILE__, __LINE__,
                "Exiting new device list with i = %d\n\n", i );
            break;
        }

        dbgStr = ixmlNode_getNodeName( tmpNode );
        UpnpPrintf( UPNP_INFO, API, __FILE__, __LINE__,
            "Extracting device type once for %s\n", dbgStr );
        // extract device type 
        ixmlNodeList_free( nodeList );
        nodeList = NULL;
        nodeList = ixmlElement_getElementsByTagName(
            ( IXML_Element * ) tmpNode, "deviceType" );
        if( nodeList == NULL ) {
            continue;
        }

        dbgStr = ixmlNode_getNodeName( tmpNode );
        UpnpPrintf( UPNP_ALL, API, __FILE__, __LINE__,
            "Extracting UDN for %s\n", dbgStr );
        UpnpPrintf( UPNP_ALL, API, __FILE__, __LINE__,
            "Extracting device type\n" );

        tmpNode2 = ixmlNodeList_item( nodeList, 0 );
        if( tmpNode2 == NULL ) {
            continue;
        }
        textNode = ixmlNode_getFirstChild( tmpNode2 );
        if( textNode == NULL ) {
            continue;
        }

        UpnpPrintf( UPNP_ALL, API, __FILE__, __LINE__,
            "Extracting device type \n" );

        tmpStr = ixmlNode_getNodeValue( textNode );
        if( tmpStr == NULL ) {
            continue;
        }

        strcpy( devType, tmpStr );
        if( devType == NULL ) {
            continue;
        }

        UpnpPrintf( UPNP_ALL, API, __FILE__, __LINE__,
            "Extracting device type = %s\n", devType );
        if( tmpNode == NULL ) {
            UpnpPrintf( UPNP_ALL, API, __FILE__, __LINE__,
                "TempNode is NULL\n" );
	}
        dbgStr = ixmlNode_getNodeName( tmpNode );
        UpnpPrintf( UPNP_ALL, API, __FILE__, __LINE__,
            "Extracting UDN for %s\n", dbgStr );
        // extract UDN 
        ixmlNodeList_free( nodeList );
        nodeList = NULL;
        nodeList = ixmlElement_getElementsByTagName( ( IXML_Element * )
                                                     tmpNode, "UDN" );
        if( nodeList == NULL ) {
            UpnpPrintf( UPNP_CRITICAL, API, __FILE__,
                __LINE__, "UDN not found!!!\n" );
                continue;
        }
        tmpNode2 = ixmlNodeList_item( nodeList, 0 );
        if( tmpNode2 == NULL ) {
            UpnpPrintf( UPNP_CRITICAL, API, __FILE__,
                __LINE__, "UDN not found!!!\n" );
            continue;
        }
        textNode = ixmlNode_getFirstChild( tmpNode2 );
        if( textNode == NULL ) {
            UpnpPrintf( UPNP_CRITICAL, API, __FILE__,
                __LINE__, "UDN not found!!!\n" );
            continue;
        }
        tmpStr = ixmlNode_getNodeValue( textNode );
        if( tmpStr == NULL ) {
            UpnpPrintf( UPNP_CRITICAL, API, __FILE__, __LINE__,
                "UDN not found!!!!\n" );
                continue;
        }
        strcpy( UDNstr, tmpStr );
        if( UDNstr == NULL ) {
            continue;
        }

        UpnpPrintf( UPNP_INFO, API, __FILE__, __LINE__,
            "Sending UDNStr = %s \n", UDNstr );
        if( AdFlag ) {
			/*  by lcl, 04May11 */

            // send the device advertisement 
            if( AdFlag == 1 ) {
			//printf("DeviceAdvertisement:%s-----%d\n", __func__, __LINE__);
                DeviceAdvertisement( devType, i == 0,
                                     UDNstr, SInfo->DescURL, Exp );
            } else {             // AdFlag == -1
            		//printf("DeviceShutdown:%s-----%d\n", __func__, __LINE__);
                DeviceShutdown( devType, i == 0, UDNstr,
                                SERVER, SInfo->DescURL, Exp );
            }
        } else {
            switch ( SearchType ) {

                case SSDP_ALL:
                    DeviceReply( DestAddr,
                                 devType, i == 0,
                                 UDNstr, SInfo->DescURL, defaultExp );
                    break;

                case SSDP_ROOTDEVICE:
                    if( i == 0 ) {
                        SendReply( DestAddr, devType, 1,
                                   UDNstr, SInfo->DescURL, defaultExp, 0 );
                    }
                    break;
                case SSDP_DEVICEUDN:
                    {
                        if( DeviceUDN != NULL && strlen( DeviceUDN ) != 0 ) {
                            if( strcasecmp( DeviceUDN, UDNstr ) ) {
                                UpnpPrintf( UPNP_INFO, API, __FILE__, __LINE__,
                                    "DeviceUDN=%s and search "
                                    "UDN=%s did not match\n",
                                    UDNstr, DeviceUDN );
                                    break;
                            } else {
                                UpnpPrintf( UPNP_INFO, API, __FILE__, __LINE__,
                                    "DeviceUDN=%s and search "
                                    "UDN=%s MATCH\n", UDNstr,
                                    DeviceUDN );
                                    SendReply( DestAddr, devType, 0,
                                               UDNstr, SInfo->DescURL,
                                               defaultExp, 0 );
                                break;
                            }
                        }
                    }
                case SSDP_DEVICETYPE:
                    {
                        if( !strncasecmp
                            ( DeviceType, devType,
                              strlen( DeviceType ) ) ) {
                            UpnpPrintf( UPNP_INFO, API, __FILE__, __LINE__,
                                "DeviceType=%s and search devType=%s MATCH\n",
                                devType, DeviceType );
                            SendReply( DestAddr, devType, 0, UDNstr,
                                       SInfo->DescURL, defaultExp, 1 );
                        } else {
                            UpnpPrintf( UPNP_INFO, API, __FILE__, __LINE__,
                                "DeviceType=%s and search devType=%s"
                                " DID NOT MATCH\n",
                                devType, DeviceType );
			}
                        break;
                    }
                default:
                    break;
            }
        }
        // send service advertisements for services corresponding 
        // to the same device 
        UpnpPrintf( UPNP_INFO, API, __FILE__, __LINE__,
            "Sending service Advertisement\n" );

        tmpNode = ixmlNodeList_item( SInfo->ServiceList, i );
        if( tmpNode == NULL ) {
            continue;
        }
        ixmlNodeList_free( nodeList );
        nodeList = NULL;
        nodeList = ixmlElement_getElementsByTagName( ( IXML_Element * )
                                                     tmpNode, "service" );
        if( nodeList == NULL ) {
            UpnpPrintf( UPNP_INFO, API, __FILE__, __LINE__,
                "Service not found 3\n" );
            continue;
        }
        for( j = 0;; j++ ) {
            tmpNode = ixmlNodeList_item( nodeList, j );
            if( tmpNode == NULL ) {
                break;
            }

            ixmlNodeList_free( tmpNodeList );
            tmpNodeList = NULL;
            tmpNodeList = ixmlElement_getElementsByTagName(
                ( IXML_Element *)tmpNode, "serviceType" );
            if( tmpNodeList == NULL ) {
                UpnpPrintf( UPNP_CRITICAL, API, __FILE__, __LINE__,
                    "ServiceType not found \n" );
                continue;
            }
            tmpNode2 = ixmlNodeList_item( tmpNodeList, 0 );
            if( tmpNode2 == NULL ) {
                continue;
            }
            textNode = ixmlNode_getFirstChild( tmpNode2 );
            if( textNode == NULL ) {
                continue;
            }
            // servType is of format Servicetype:ServiceVersion
            tmpStr = ixmlNode_getNodeValue( textNode );
            if( tmpStr == NULL ) {
                continue;
            }
            strcpy( servType, tmpStr );
            if( servType == NULL ) {
                continue;
            }

            UpnpPrintf( UPNP_INFO, API, __FILE__, __LINE__,
                "ServiceType = %s\n", servType );
            if( AdFlag ) {
                if( AdFlag == 1 ) {
			//printf("ServiceAdvertisement:%s-----%d\n", __func__, __LINE__);
                    ServiceAdvertisement( UDNstr, servType,
                                          SInfo->DescURL, Exp );
                } else {         // AdFlag == -1
               	//printf("ServiceShutdown:%s-----%d\n", __func__, __LINE__);
                    ServiceShutdown( UDNstr, servType,
                                     SInfo->DescURL, Exp );
                }
            } else {
                switch ( SearchType ) {
                    case SSDP_ALL:
                        ServiceReply( DestAddr, servType,
                                      UDNstr, SInfo->DescURL,
                                      defaultExp );
                        break;
                    case SSDP_SERVICE:
                        if( ServiceType != NULL ) {
                            if( !strncasecmp( ServiceType,
                                              servType,
                                              strlen( ServiceType ) ) ) {
                                ServiceReply( DestAddr, servType,
                                              UDNstr, SInfo->DescURL,
                                              defaultExp );
                            }
                        }
                        break;
                    default:
                        break;
                }               // switch(SearchType)               

            }
        }
        ixmlNodeList_free( tmpNodeList );
        tmpNodeList = NULL;
        ixmlNodeList_free( nodeList );
        nodeList = NULL;
    }
    UpnpPrintf( UPNP_ALL, API, __FILE__, __LINE__,
        "Exiting AdvertiseAndReply : \n" );

    HandleUnlock();

    return UPNP_E_SUCCESS;

}  /****************** End of AdvertiseAndReply *********************/

#endif /* EXCLUDE_SSDP == 0 */
#endif /* INCLUDE_DEVICE_APIS */

/************************************************************************
 * Function : Make_Socket_NoBlocking
 *
 * Parameters:
 *	IN int sock: socket
 *
 * Description:
 *	This function makes socket non-blocking.
 *
 * Returns: int
 *	0 if successful else -1 
 ***************************************************************************/
int
Make_Socket_NoBlocking( int sock )
{
#ifdef WIN32
     u_long val=1;
     return ioctlsocket(sock, FIONBIO, &val);
#else
    int val;

    val = fcntl( sock, F_GETFL, 0 );
    if( fcntl( sock, F_SETFL, val | O_NONBLOCK ) == -1 ) {
        return -1;
    }
#endif
    return 0;
}

/************************************************************************
 * Function : unique_service_name
 *
 * Parameters:
 *	IN char *cmd: Service Name string
 *	OUT SsdpEvent *Evt: The SSDP event structure partially filled
 *		by all the function.
 *
 * Description:
 *	This function fills the fields of the event structure like DeviceType,
 *	Device UDN and Service Type
 *
 * Returns: int
 *	0 if successful else -1
 ***************************************************************************/
int unique_service_name(IN char *cmd, IN SsdpEvent *Evt)
{
    char TempBuf[COMMAND_LEN];
    char *TempPtr = NULL;
    char *Ptr = NULL;
    char *ptr1 = NULL;
    char *ptr2 = NULL;
    char *ptr3 = NULL;
    int CommandFound = 0;
    size_t n = (size_t)0;

    if( ( TempPtr = strstr( cmd, "uuid:schemas" ) ) != NULL ) {
        ptr1 = strstr( cmd, ":device" );
        if( ptr1 != NULL ) {
            ptr2 = strstr( ptr1 + 1, ":" );
        } else {
            return -1;
        }

        if( ptr2 != NULL ) {
            ptr3 = strstr( ptr2 + 1, ":" );
        } else {
            return -1;
        }

        if( ptr3 != NULL ) {
             if (strlen("uuid:") + strlen(ptr3 + 1) >= sizeof(Evt->UDN))
			return -1;
		snprintf(Evt->UDN, sizeof(Evt->UDN), "uuid:%s", ptr3 + 1);
        } else {
            return -1;
        }

        ptr1 = strstr( cmd, ":" );
        if( ptr1 != NULL ) {
	     n = (size_t)ptr3 - (size_t)ptr1;
	     n = n >= sizeof(TempBuf) ? sizeof(TempBuf) -1 : n;
            strncpy( TempBuf, ptr1, n );
            TempBuf[n] = '\0';
	     if (strlen("urn") + strlen(TempBuf) >= sizeof(Evt->DeviceType))
		 	return -1;
	     snprintf(Evt->DeviceType, sizeof(Evt->DeviceType),
				"urn%s", TempBuf);
        } else {
            return -1;
        }
        return 0;
    }

    if( ( TempPtr = strstr( cmd, "uuid" ) ) != NULL ) {
        if( ( Ptr = strstr( cmd, "::" ) ) != NULL ) {
	     n = (size_t)Ptr - (size_t)TempPtr;
	     n = n >= sizeof(Evt->UDN) ? sizeof(Evt->UDN) -1 : n;
            strncpy( Evt->UDN, TempPtr, n );
            Evt->UDN[n] = '\0';
        } else {
            memset( Evt->UDN, 0, sizeof(Evt->UDN) );
            strncpy( Evt->UDN, TempPtr, sizeof(Evt->UDN) - 1 );
        }
        CommandFound = 1;
    }

    if( strstr( cmd, "urn:" ) != NULL
        && strstr( cmd, ":service:" ) != NULL ) {
        if( ( TempPtr = strstr( cmd, "urn" ) ) != NULL ) {
	     memset(Evt->ServiceType, 0, sizeof(Evt->ServiceType));
            strncpy( Evt->ServiceType, TempPtr, sizeof(Evt->ServiceType) - 1 );
            CommandFound = 1;
        }
    }

    if( strstr( cmd, "urn:" ) != NULL
        && strstr( cmd, ":device:" ) != NULL ) {
        if( ( TempPtr = strstr( cmd, "urn" ) ) != NULL ) {
	     memset(Evt->DeviceType, 0, sizeof(Evt->DeviceType));
            strncpy( Evt->DeviceType, TempPtr, sizeof(Evt->DeviceType) -1 );
            CommandFound = 1;
        }
    }

    if( ( TempPtr = strstr( cmd, "::upnp:rootdevice" ) ) != NULL ) {
        /* Everything before "::upnp::rootdevice" is the UDN. */
        if( TempPtr != cmd ) {
            n = (size_t)TempPtr - (size_t)cmd;
	     n = n >= sizeof(Evt->UDN) ? sizeof(Evt->UDN) - 1 : n;
            strncpy(Evt->UDN, cmd, n);
            Evt->UDN[n] = 0;
            CommandFound = 1;
        }
    }
   
    if( CommandFound == 0 ) {
        return -1;
    }

    return 0;
}

/************************************************************************
 * Function : ssdp_request_type1
 *
 * Parameters:
 *	IN char *cmd: command came in the ssdp request
 *
 * Description:
 *	This function figures out the type of the SSDP search in the
 *	in the request.
 *
 * Returns: enum SsdpSearchType
 *	return appropriate search type else returns SSDP_ERROR
 ***************************************************************************/
enum SsdpSearchType
ssdp_request_type1( IN char *cmd )
{
    if( strstr( cmd, ":all" ) != NULL ) {
        return SSDP_ALL;
    }
    if( strstr( cmd, ":rootdevice" ) != NULL ) {
        return SSDP_ROOTDEVICE;
    }
    if( strstr( cmd, "uuid:" ) != NULL ) {
        return SSDP_DEVICEUDN;
    }
    if( ( strstr( cmd, "urn:" ) != NULL )
        && ( strstr( cmd, ":device:" ) != NULL ) ) {
        return SSDP_DEVICETYPE;
    }
    if( ( strstr( cmd, "urn:" ) != NULL )
        && ( strstr( cmd, ":service:" ) != NULL ) ) {
        return SSDP_SERVICE;
    }
    return SSDP_SERROR;
}

/************************************************************************
 * Function : ssdp_request_type
 *
 * Parameters:
 *	IN char *cmd: command came in the ssdp request
 *	OUT SsdpEvent *Evt: The event structure partially filled by
 *		 this function.
 *
 * Description:
 *	This function starts filling the SSDP event structure based upon the 
 *	request received.
 *
 * Returns: int
 *	0 on success; -1 on error
 ***************************************************************************/
int
ssdp_request_type( IN char *cmd,
                   OUT SsdpEvent * Evt )
{
    // clear event
    memset( Evt, 0, sizeof( SsdpEvent ) );
    unique_service_name( cmd, Evt );
    Evt->ErrCode = NO_ERROR_FOUND;

    if( ( Evt->RequestType = ssdp_request_type1( cmd ) ) == SSDP_SERROR ) {
        Evt->ErrCode = E_HTTP_SYNTEX;
        return -1;
    }
    return 0;
}

/************************************************************************
 * Function : free_ssdp_event_handler_data
 *
 * Parameters:
 *	IN void *the_data: ssdp_thread_data structure. This structure contains
 *			SSDP request message.
 *
 * Description:
 *	This function frees the ssdp request
 *
 * Returns: VOID
 *
 ***************************************************************************/
static void
free_ssdp_event_handler_data( void *the_data )
{
    ssdp_thread_data *data = ( ssdp_thread_data * ) the_data;

    if( data != NULL ) {
        http_message_t *hmsg = &data->parser.msg;

        // free data
        httpmsg_destroy( hmsg );
        free( data );
    }
}

/************************************************************************
 * Function : valid_ssdp_msg
 *
 * Parameters:
 *	IN void *the_data: ssdp_thread_data structure. This structure contains
 *			SSDP request message.
 *
 * Description:
 *	This function do some quick checking of the ssdp msg
 *
 * Returns: xboolean
 *	returns TRUE if msg is valid else FALSE
 ***************************************************************************/
static UPNP_INLINE xboolean
valid_ssdp_msg( IN http_message_t * hmsg )
{
    memptr hdr_value;

    // check for valid methods - NOTIFY or M-SEARCH
    if( hmsg->method != HTTPMETHOD_NOTIFY &&
        hmsg->method != HTTPMETHOD_MSEARCH
        && hmsg->request_method != HTTPMETHOD_MSEARCH ) {
        return FALSE;
    }
    if( hmsg->request_method != HTTPMETHOD_MSEARCH ) {
        // check PATH == *
        if( hmsg->uri.type != RELATIVE ||
            strncmp( "*", hmsg->uri.pathquery.buff,
                     hmsg->uri.pathquery.size ) != 0 ) {
            return FALSE;
        }
        // check HOST header
        if( ( httpmsg_find_hdr( hmsg, HDR_HOST, &hdr_value ) == NULL ) ||
            ( memptr_cmp( &hdr_value, "239.255.255.250:1900" ) != 0 )
             ) {
            return FALSE;
        }
    }
    return TRUE;                // passed quick check
}

/************************************************************************
 * Function : start_event_handler
 *
 * Parameters:
 *	IN void *the_data: ssdp_thread_data structure. This structure contains
 *			SSDP request message.
 *
 * Description:
 *	This function parses the message and dispatches it to a handler
 *	which handles the ssdp request msg
 *
 * Returns: int
 *	0 if successful -1 if error
 ***************************************************************************/
static UPNP_INLINE int
start_event_handler( void *Data )
{

    http_parser_t *parser = NULL;
    parse_status_t status;
    ssdp_thread_data *data = ( ssdp_thread_data * ) Data;

    parser = &data->parser;

    status = parser_parse( parser );
    if( status == PARSE_FAILURE ) {
        if( parser->msg.method != HTTPMETHOD_NOTIFY ||
            !parser->valid_ssdp_notify_hack ) {
            UpnpPrintf( UPNP_INFO, SSDP, __FILE__, __LINE__,
                "SSDP recvd bad msg code = %d\n",
                status );
            // ignore bad msg, or not enuf mem
            goto error_handler;
        }
        // valid notify msg
    } else if( status != PARSE_SUCCESS ) {
        UpnpPrintf( UPNP_INFO, SSDP, __FILE__, __LINE__,
            "SSDP recvd bad msg code = %d\n", status );

        goto error_handler;
    }
    // check msg
    if( !valid_ssdp_msg( &parser->msg ) ) {
        goto error_handler;
    }
    return 0;                   //////// done; thread will free 'data'

  error_handler:
    free_ssdp_event_handler_data( data );
    return -1;
}

/************************************************************************
 * Function : ssdp_event_handler_thread
 *
 * Parameters:
 *	IN void *the_data: ssdp_thread_data structure. This structure contains
 *			SSDP request message.
 *
 * Description:
 *	This function is a thread that handles SSDP requests.
 *
 * Returns: void
 *
 ***************************************************************************/
static void
ssdp_event_handler_thread( void *the_data )
{
    ssdp_thread_data *data = ( ssdp_thread_data * ) the_data;
    http_message_t *hmsg = &data->parser.msg;

    if( start_event_handler( the_data ) != 0 ) {
        return;
    }
    // send msg to device or ctrlpt
    if( ( hmsg->method == HTTPMETHOD_NOTIFY ) ||
        ( hmsg->request_method == HTTPMETHOD_MSEARCH ) ) {
        CLIENTONLY( ssdp_handle_ctrlpt_msg( hmsg, &data->dest_addr, FALSE, NULL );)
    } else {
        ssdp_handle_device_request( hmsg, &data->dest_addr );
    }

    // free data
    free_ssdp_event_handler_data( data );
}

/************************************************************************
 * Function : readFromSSDPSocket
 *
 * Parameters:
 *	IN SOCKET socket: SSDP socket
 *
 * Description:
 *	This function reads the data from the ssdp socket.
 *
 * Returns: void
 *
 ***************************************************************************/
void
readFromSSDPSocket( SOCKET socket )
{
    char *requestBuf = NULL;
    char staticBuf[BUFSIZE];
    struct sockaddr_in clientAddr;
	
/*
 * brief: Added by LI CHENGLONG, 2012 Mar 25 12:06:41 PM
 *		  去除线程处理，解决emule, pplive(pptv)不能添加端口映射的bug.	  
 */
#if 0
    ThreadPoolJob job;
#endif

    ssdp_thread_data *data = NULL;
    socklen_t socklen = 0;
    int byteReceived = 0;

    requestBuf = staticBuf;

    //in case memory
    //can't be allocated, still drain the 
    //socket using a static buffer

    socklen = sizeof( struct sockaddr_in );

    data = ( ssdp_thread_data * )
        malloc( sizeof( ssdp_thread_data ) );

    if( data != NULL ) {
        //initialize parser

#ifdef INCLUDE_CLIENT_APIS
        if( socket == gSsdpReqSocket ) {
            parser_response_init( &data->parser, HTTPMETHOD_MSEARCH );
        } else {
            parser_request_init( &data->parser );
        }
#else
        parser_request_init( &data->parser );
#endif

        //set size of parser buffer

        if( membuffer_set_size( &data->parser.msg.msg, BUFSIZE ) == 0 ) {
            //use this as the buffer for recv
            requestBuf = data->parser.msg.msg.buf;

        } else {
            free( data );
            data = NULL;
        }
    }
    byteReceived = recvfrom( socket, requestBuf,
                             BUFSIZE - 1, 0,
                             ( struct sockaddr * )&clientAddr, &socklen );

    if( byteReceived > 0 ) {
        requestBuf[byteReceived] = '\0';
        UpnpPrintf( UPNP_INFO, SSDP,
            __FILE__, __LINE__,
            "Start of received response ----------------------------------------------------\n"
            "%s\n"
            "End of received response ------------------------------------------------------\n"
            "From host %s\n",
            requestBuf,
            inet_ntoa( clientAddr.sin_addr ) );
        UpnpPrintf( UPNP_PACKET, SSDP, __FILE__, __LINE__,
            "Start of received multicast packet --------------------------------------------\n"
            "%s\n"
            "End of received multicast packet ----------------------------------------------\n",
            requestBuf );
        //add thread pool job to handle request
        if( data != NULL ) {
            data->parser.msg.msg.length += byteReceived;
            // null-terminate
            data->parser.msg.msg.buf[byteReceived] = 0;
            data->dest_addr = clientAddr;
/*
 * brief: Added by LI CHENGLONG, 2012 Mar 25 12:06:41 PM
 *		  去除线程处理，解决emule, pplive(pptv)不能添加端口映射的bug.	  
 */
#if 0
            TPJobInit( &job, ( start_routine )
                       ssdp_event_handler_thread, data );
            TPJobSetFreeFunction( &job, free_ssdp_event_handler_data );
            TPJobSetPriority( &job, MED_PRIORITY );

            if( ThreadPoolAdd( &gRecvThreadPool, &job, NULL ) != 0 ) {
                free_ssdp_event_handler_data( data );
            }
#endif
		
			ssdp_event_handler_thread(data);
        }
    } else {
        free_ssdp_event_handler_data( data );
    }
}

/************************************************************************
 * Function : get_ssdp_sockets								
 *
 * Parameters:
 *	OUT MiniServerSockArray *out: Arrays of SSDP sockets
 *
 * Description:
 *	This function creates the ssdp sockets. It set their option to listen
 *	for multicast traffic.
 *
 * Returns: int
 *	return UPNP_E_SUCCESS if successful else returns appropriate error
 ***************************************************************************/
int
get_ssdp_sockets( MiniServerSockArray * out )
{
    char errorBuffer[ERROR_BUFFER_LEN];
    int onOff = 1;
    u_char ttl = 4;
    struct ip_mreq ssdpMcastAddr;
    struct sockaddr_in ssdpAddr;
    int option = 1;
    int ret = 0;
    struct in_addr addr;
    SOCKET ssdpSock;

	/* control point  不需要绑定1900端口，
	M-SEARCH的结果 而M-Search 的返回端口可以在发送时指定by LiChenglong*/
#if INCLUDE_CLIENT_APIS
    SOCKET ssdpReqSock;

    ssdpReqSock = socket( AF_INET, SOCK_DGRAM, 0 );
    if ( ssdpReqSock == -1 ) {
        strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
        UpnpPrintf( UPNP_CRITICAL, SSDP, __FILE__, __LINE__,
            "Error in socket(): %s\n", errorBuffer );

            return UPNP_E_OUTOF_SOCKET;
    }
    ret = setsockopt( ssdpReqSock, IPPROTO_IP, IP_MULTICAST_TTL,
        &ttl, sizeof (ttl) );
    // just do it, regardless if fails or not.
    Make_Socket_NoBlocking( ssdpReqSock );
    gSsdpReqSocket = ssdpReqSock;
#endif /* INCLUDE_CLIENT_APIS */

    ssdpSock = socket( AF_INET, SOCK_DGRAM, 0 );
    if ( ssdpSock == -1 ) {
        strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
        UpnpPrintf( UPNP_CRITICAL, SSDP, __FILE__, __LINE__,
            "Error in socket(): %s\n", errorBuffer );
        CLIENTONLY( shutdown( ssdpReqSock, SD_BOTH ); )
        CLIENTONLY( UpnpCloseSocket( ssdpReqSock ); )

        return UPNP_E_OUTOF_SOCKET;
    }

	
    onOff = 1;
    ret = setsockopt( ssdpSock, SOL_SOCKET, SO_REUSEADDR,
        (char *)&onOff, sizeof(onOff) );
    if ( ret == -1) {
        strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
        UpnpPrintf( UPNP_CRITICAL, SSDP, __FILE__, __LINE__,
            "Error in setsockopt() SO_REUSEADDR: %s\n", errorBuffer );
        CLIENTONLY( shutdown( ssdpReqSock, SD_BOTH ); )
        CLIENTONLY( UpnpCloseSocket( ssdpReqSock ); )
        shutdown( ssdpSock, SD_BOTH );
        UpnpCloseSocket( ssdpSock );

        return UPNP_E_SOCKET_ERROR;
    }
    
#if defined(__FreeBSD__) || defined(__OSX__) || defined(__APPLE__)
    ret = setsockopt( ssdpSock, SOL_SOCKET, SO_REUSEPORT,
        (char *)&onOff, sizeof (onOff) );
    if ( ret == -1 ) {
        strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
        UpnpPrintf( UPNP_CRITICAL, SSDP, __FILE__, __LINE__,
            "Error in setsockopt() SO_REUSEPORT: %s\n", errorBuffer );
        CLIENTONLY( shutdown( ssdpReqSock, SD_BOTH ); )
        CLIENTONLY( UpnpCloseSocket( ssdpReqSock ); )
        shutdown( ssdpSock, SD_BOTH );
        UpnpCloseSocket( ssdpSock );

        return UPNP_E_SOCKET_ERROR;
    }
#endif /* __FreeBSD__ */

    memset( (void *)&ssdpAddr, 0, sizeof( struct sockaddr_in ) );
    ssdpAddr.sin_family = AF_INET;
    //  ssdpAddr.sin_addr.s_addr = inet_addr(LOCAL_HOST);
    ssdpAddr.sin_addr.s_addr = htonl( INADDR_ANY );
    ssdpAddr.sin_port = htons( SSDP_PORT );
	
   /*must can be successful listen on 1900  by LiChenglong*/
    ret = bind( ssdpSock, (struct sockaddr *)&ssdpAddr, sizeof (ssdpAddr) );
    if ( ret == -1 ) {
        strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
        UpnpPrintf( UPNP_CRITICAL, SSDP, __FILE__, __LINE__,
            "Error in bind(), addr=0x%08X, port=%d: %s\n",
            INADDR_ANY, SSDP_PORT, errorBuffer );
            shutdown( ssdpSock, SD_BOTH );
        UpnpCloseSocket( ssdpSock );
        CLIENTONLY( shutdown( ssdpReqSock, SD_BOTH ); )
        CLIENTONLY( UpnpCloseSocket( ssdpReqSock ); )

        return UPNP_E_SOCKET_BIND;
    }

    memset( (void *)&ssdpMcastAddr, 0, sizeof (struct ip_mreq) );
    ssdpMcastAddr.imr_interface.s_addr = inet_addr( LOCAL_HOST );/*listen on interface   by LiChenglong*/
    ssdpMcastAddr.imr_multiaddr.s_addr = inet_addr( SSDP_IP );
    ret = setsockopt( ssdpSock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
        (char *)&ssdpMcastAddr, sizeof (struct ip_mreq) );
    if ( ret == -1 ) {
        strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
        UpnpPrintf( UPNP_CRITICAL, SSDP, __FILE__, __LINE__,
            "Error in setsockopt() IP_ADD_MEMBERSHIP (join multicast group): %s\n",
            errorBuffer );
        shutdown( ssdpSock, SD_BOTH );
        CLIENTONLY( shutdown( ssdpReqSock, SD_BOTH ); )
        UpnpCloseSocket( ssdpSock );
        CLIENTONLY( UpnpCloseSocket( ssdpReqSock ); )

        return UPNP_E_SOCKET_ERROR;
    }

    /* Set multicast interface. */
    memset( (void *)&addr, 0, sizeof (struct in_addr) );
    addr.s_addr = inet_addr(LOCAL_HOST);
    ret = setsockopt(ssdpSock, IPPROTO_IP, IP_MULTICAST_IF,
        (char *)&addr, sizeof addr);
    if ( ret == -1 ) {
        strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
        UpnpPrintf( UPNP_INFO, SSDP, __FILE__, __LINE__,
            "Error in setsockopt() IP_MULTICAST_IF (set multicast interface): %s\n",
            errorBuffer );
        /* This is probably not a critical error, so let's continue. */
    }

    /* result is not checked becuase it will fail in WinMe and Win9x. */
    ret = setsockopt( ssdpSock, IPPROTO_IP,
        IP_MULTICAST_TTL, &ttl, sizeof (ttl) );

    ret = setsockopt( ssdpSock, SOL_SOCKET, SO_BROADCAST,
        (char *)&option, sizeof (option) );
    if( ret == -1) {
        strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
        UpnpPrintf( UPNP_CRITICAL, SSDP, __FILE__, __LINE__,
            "Error in setsockopt() SO_BROADCAST (set broadcast): %s\n",
            errorBuffer );
        shutdown( ssdpSock, SD_BOTH );
        CLIENTONLY( shutdown( ssdpReqSock, SD_BOTH ); )
        UpnpCloseSocket( ssdpSock );
        CLIENTONLY( UpnpCloseSocket( ssdpReqSock ); )

        return UPNP_E_NETWORK_ERROR;
    }

    CLIENTONLY( out->ssdpReqSock = ssdpReqSock; )
    out->ssdpSock = ssdpSock;

    return UPNP_E_SUCCESS;
}

#endif /* EXCLUDE_SSDP */

