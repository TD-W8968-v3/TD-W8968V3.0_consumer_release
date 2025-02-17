/***********************************************************************
 *
 *  Copyright (c) 2007  Broadcom Corporation
 *  All Rights Reserved
 *
 * <:label-BRCM:2011:DUAL/GPL:standard
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
:>
 *
 ************************************************************************/


#ifndef __CMS_IMAGE_H__
#define __CMS_IMAGE_H__

#include "cms.h"


/*!\file cms_image.h
 * \brief Header file for public flash image functions.
 *
 * These functions can be included by any application, including GPL applications.
 * The image can be software images (cfe+fs+kernel, fs+kernel) or config files.
 *
 */


/** Minimum size for image uploads.
 *  Config file uploads can be less than 2K.
 */
#define  CMS_IMAGE_MIN_LEN    2048


/*!\enum CmsImageFormat
 * \brief cms image formats that we recognize.
 */
typedef enum
{
    CMS_IMAGE_FORMAT_INVALID=0,   /**< invalid or unrecognized format */
    CMS_IMAGE_FORMAT_BROADCOM=1,  /**< broadcom image (with our header) */
    CMS_IMAGE_FORMAT_FLASH=2,     /**< raw flash image */
    CMS_IMAGE_FORMAT_XML_CFG=3,   /**< CMS XML config file */
    CMS_IMAGE_FORMAT_MODSW_PKG=4, /**< modular software package */
    CMS_IMAGE_FORMAT_PART1=0x10,   /**< Specify to write to partition1 */
    CMS_IMAGE_FORMAT_PART2=0x20,   /**< Specify to write to partition2 */
    CMS_IMAGE_FORMAT_NO_REBOOT=0x80, /**< Do not reboot after flashing  */
                                    /**  image to non-active partition */
    CMS_IMAGE_FORMAT_3G_SETTINGS = 0x100,	/*usb 3g settings*/
} CmsImageFormat;

/*added by wwj from 8960 v4.0 sc */
/* ZJin: tp tag definition! */
#define TFTP_MIN_APP_SIZE     	0x00030000  	/* 192K bytes */
#define TFTP_MAX_APP_SIZE     	0x00800000		/* 8M bytes */	//0x00400000  	/* 4M bytes */
#define MD5_DIGEST_LEN			16		// length of MD5 digest result
// file tag (head) structure all is in clear text except validationTokens (crc, md5, sha1, etc). Total: 128 unsigned chars
#define TP_TAG_LEN         512
#define TP_SIG_LEN         24
#define TP_SIG_LEN_2       12   // Original second SIG = 20 is now devided into 12 for TP_SIG_LEN_2 and 8 for TP_CHIP_ID
#define TP_CHIP_ID_LEN		8	
#define TP_TOKEN_LEN       20
#define TP_BOARD_ID_LEN    16// TAG for downloadable image (kernel plus file system)// integer in the structure is stored in Network-Byte order (BIG-endian)

#define NVRAM_RSA_FILE	"/proc/nvram/RsaSig"
#define NVRAM_LEN_FILE	"/proc/nvram/fs_kernel_len"


/* RSA signature from "tpbsp", yuanshang, 2013-05-28 */
#define TP_RSA_SIG_LEN	128

typedef struct _LINUX_FILE_TAG
{	
	unsigned long tagVersion;    
	char 		  signiture_1[TP_SIG_LEN];         // text line for company info	
	char 		  signiture_2[TP_SIG_LEN_2];       // additional info (can be version number)    
	char 		  chipId[TP_CHIP_ID_LEN];			// chip id     
	char 		  boardId[TP_BOARD_ID_LEN];        // board id    
	unsigned long productId;					// product id    
	unsigned long productVer;					// product version    
	unsigned long hardwareVer;					// hardware version /**yuanshang*/
	
	unsigned char imageValidationToken[TP_TOKEN_LEN];// image validation token - md5 checksum
	unsigned char tpImageValidationToken[TP_TOKEN_LEN];	// kernel+tag validation token - md5 checksum
	
	unsigned long kernelTextAddr;				// text section address of kernel	
	unsigned long kernelEntryPoint;				// entry point address of kernel		
	
	unsigned long totalImageLen;				// the sum of kernelLen+rootfsLen+tagLen
	
	unsigned long kernelAddress;				// starting address (offset from the beginning of FILE_TAG) of kernel image	
	unsigned long kernelLen;					// length of kernel image
	
	unsigned long rootfsAddress;				// starting address (offset) of filesystem image	
	unsigned long rootfsLen;					// length of filesystem image
	
	unsigned long bootloaderAddress;			// starting address (offset) of boot loader image	
	unsigned long bootloaderLen;				// length of boot loader image

	/* RSA signature from "tpbsp", yuanshang, 2013-05-28 */
	unsigned char rsaSig[TP_RSA_SIG_LEN];		/* signature for update */
	unsigned char resRsaSig[TP_RSA_SIG_LEN];	/* reserved for signature */
	
} LINUX_FILE_TAG;
/* end of tp tag definition! */
/* end added */

/** Validate the given image and return the image format of the given image.
 * 
 * The image can be a broadcom image, whole image, or config file.
 * This function will also verify that the image will fit in flash.
 *
 * @param imagePtr (IN) image to be parsed.
 * @param imageLen (IN) Length of image in bytes.
 * @param msgHandle (IN) message handle from the caller.
 * 
 * @return CmsImageFormat enum.
 */
CmsImageFormat cmsImg_validateImage(const char *imagePtr, UINT32 imageLen, void *msgHandle);


/** Write the image to flash.
 *  The image can be a config file, a broadcom image, or flash image.
 *  This function will validate the image before writing to flash.
 *
 * @param imagePtr (IN) image to be written.  Surprisingly, for cfe+kernel+fs
 *                      flash writes, the image is modified, so we cannot
 *                      declare this parameter as const.
 * @param imageLen (IN) Length of image in bytes.
 * @param msgHandle (IN) message handle from the caller.
 * 
 * @return CmsRet enum.
 */
CmsRet cmsImg_writeImage(char *imagePtr, UINT32 imageLen, void *msgHandle);


/** Write a validated image in known format to flash.
 *  The image can be a config file, a broadcom image, or flash image.
 *
 * @param imagePtr (IN) image to be written.  Surprisingly, for cfe+kernel+fs
 *                      flash writes, the image is modified, so we cannot
 *                      declare this parameter as const.
 * @param imageLen (IN) Length of image in bytes.
 * @param format   (IN) CmsImageFormat of the image.
 * @param msgHandle (IN) message handle from the caller.
 * 
 * @return CmsRet enum.
 */
CmsRet cmsImg_writeValidatedImage(char *imagePtr, UINT32 imageLen, CmsImageFormat format, void *msgHandle);


/** Return the number of bytes available in the flash for storing an image.
 * 
 * Note this function does not return the size of the entire flash.
 * Rather, it returns the number of bytes available in
 * the flash for storing the image.
 * 
 * @return the number of bytes available in the flash for storing an image.
 */
UINT32 cmsImg_getImageFlashSize(void);


/** Return the number of bytes available in the flash for storing a config file.
 *
 * Note, if compressed config file is enabled, this function will return
 * a number that is larger than the actual number of bytes available in
 * the flash for storing the config file
 *
 * @return the number of bytes available for storing a config file.
 */
UINT32 cmsImg_getConfigFlashSize(void);


/** Return the actual number of bytes available in the flash for storing a config file.
 *
 * This returns the actual number of bytes available in the flash for
 * storing a config file.  External callers should not use this function.
 *
 * @return the number of bytes available for storing a config file.
 */
UINT32 cmsImg_getRealConfigFlashSize(void);


/** Return TRUE if the flash has space for a backup config file.
 *
 * @return TRUE if the flash has space for a backup config file.
 */
UBOOL8 cmsImg_isBackupConfigFlashAvailable(void);


/** Return the size, in bytes, of the broadcom image tag header.
 * 
 * @return the number of bytes in the broadcom image tag header that is present
 *         at the beginning of broadcom images.
 */
UINT32 cmsImg_getBroadcomImageTagSize(void);


/** Safety margin to use when determining if an image will fit into the flash.
 *  Used by cmsImg_willFitInFlash().
 *
 * Not clear if this is really needed.
 */
#define  CMS_IMAGE_OVERHEAD          256

/** Return true if the image will fit in the flash.
 * 
 * Compares the given image length with the image flash size minus a
 * CMS_IMAGE_OVERHEAD margin.  Do we really need to have this margin?
 * Currently, only httpd uses this code.
 * 
 * @param imageLen (IN) Length of image in bytes.
 * 
 * @return True if the image will fit in the flash.
 */
UBOOL8 cmsImg_willFitInFlash(UINT32 imageLen);


/** Minimum length needed to detect if a buffer could be a config file.
 */
#define CMS_CONFIG_FILE_DETECTION_LENGTH 64

/** Check the first CMS_CONFIG_FILE_DETECTION_LENGTH bytes of the given
 *  buffer to see if this is a config file.
 * 
 * @param buf (IN) buffer containing the image to be analyzed.
 * 
 * @return TRUE if the image is likely to be a config file.
 */
UBOOL8 cmsImg_isConfigFileLikely(const char *buf);


#define SW_UPDATE_DATA_DIR  "/data/swupdate.d"
#define SW_UPDATE_PKG_FILE  "__modswpkg"


/** Check if the buffer is a valid/recognized modular software package.
 *
 * @param buf (IN) buffer containing the possible modular software package.
 * @param len (IN) length of buffer.
 *
 * @return TRUE if the buffer contains a valid/recognized modular sw pkg.
 */
UBOOL8 cmsImg_isModSwPkg(const unsigned char *buf, UINT32 len);


/** Send a message to smd informing it that we are starting a big image download.
 * 
 * This function only needs to be called if we are doing a broadcom or
 * flash image download.  Not needed for config file downloads.
 * 
 * @param msgHandle (IN) the message handle to use to send the message.
 * @param connIfName (IN) the connection interface name saved at connection time.
 */
void cmsImg_sendLoadStartingMsg(void *msgHandle, const char *connIfName); 


/** Send a message to smd informing it that we are done with a big image download.
 * 
 * smd will now restart any applications that it killed while the image download
 * was in progress.  This only needs to be called if the image download failed
 * and we want the modem to continue working as normal.  If the image download
 * succeeded, then we will probably reboot the modem as the last step.  No
 * need to restart apps again.
 * 
 * @param msgHandle (IN) the message handle to use to send the message.
 */
void cmsImg_sendLoadDoneMsg(void *msgHandle); 


/** Send a reboot request msg to smd.
 * 
 * @param msgHandle (IN) the message handle to use to send the message.
 */
void cmsUtil_sendRequestRebootMsg(void *msgHandle);


/** Given a socket file descriptor, return the interface that the socket is bound to.
 *
 * This function is useful during image upload when we need to know if the 
 * image data will be coming from the LAN side or WAN side, and if WAN side, 
 * which interface on the WAN it will be coming from (so we can bring down the
 * other WAN interfaces to save memory.)
 *
 * @param socketfd    (IN) the socket of the connection
 * @param connIfName (OUT) on successful return, this buffer will contain the
 *                         linux interface name that is associated with the socket.
 *                         Caller must supply a buffer of at least CMS_IFNAME_LENGTH bytes.
 *
 * @return CmsRet enum.
 */
CmsRet cmsImg_saveIfNameFromSocket(SINT32 socketfd, char *connIfName);


#ifdef DMP_DEVICE2_HOMEPLUG_1
/** Update the PowerLine configuration State variable for image
 *  upgrades or for restoring factory defaults.
 *  
 * This function is only useful to PLC code and should not be 
 * used by other applications. 
 *
 * @param state      (IN) a char pointer to the state value to 
 *                   be written
 * @return CmsRet enum.
 */
CmsRet cmsImg_setPLCconfigState(const char *state);
#endif

/*Hanjaiyan add for usb 3G*/
#ifdef SUPPORT_USB_3G
CmsRet cmsImg_write3gSettings(char *imagePtr, UINT32 imageLen, void *msgHandle);
#endif

#endif /*__CMS_IMAGE_H__*/

