/***********************************************************************
 *
 *  Copyright (c) 2007  Broadcom Corporation
 *  All Rights Reserved
 *
<:label-BRCM:2012:DUAL/GPL:standard

Unless you and Broadcom execute a separate written software license
agreement governing use of this software, this software is licensed
to you under the terms of the GNU General Public License version 2
(the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
with the following added to such license:

   As a special exception, the copyright holders of this software give
   you permission to link this software with independent modules, and
   to copy and distribute the resulting executable under terms of your
   choice, provided that you also meet, for each linked independent
   module, the terms and conditions of the license of that module.
   An independent module is a module which is not derived from this
   software.  The special exception does not apply to any modifications
   of the software.

Not withstanding the above, under no circumstances may you combine
this software in any way with any other Broadcom software provided
under a license other than the GPL, without Broadcom's express prior
written consent.

:>
 *
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <net/if.h>

#include "cms.h"
#include "cms_util.h"
#include "cms_msg.h"
#include "cms_boardcmds.h"
#include "cms_boardioctl.h"

/* need do tplink image check, wwj 130226 */
//#include "md5_interface.h"
#include "../../../private/include/tp_product.h"
#include "rsa.h"
/*end added */

#include "bcmTag.h" /* in shared/opensource/include/bcm963xx, for FILE_TAG */
#include "board.h" /* in bcmdrivers/opensource/include/bcm963xx, for BCM_IMAGE_CFE */

static UBOOL8 matchChipId(const char *strTagChipId, const char *signature2);
CmsRet verifyBroadcomFileTag(FILE_TAG *pTag, int imageLen);
CmsRet verifyTplinkFileTag(LINUX_FILE_TAG *pTag, int imageLen, int doCheck); /*added by wwj 130226 */
static CmsRet flashImage(char *imagePtr, UINT32 imageLen, CmsImageFormat format);
static CmsRet sendConfigMsg(const char *imagePtr, UINT32 imageLen, void *msgHandle, CmsMsgType msgType);
#ifdef SUPPORT_MOD_SW_UPDATE
static void sendStartModupdtdMsg(void *msgHandle);
#endif

static UBOOL8 checkFwSign(UINT8 *pFirmwareAddr, UINT32 firmwareLength);

/*Hanjiayan add for usb 3g*/
#ifdef SUPPORT_USB_3G

/* macros for upload 3g .bin file. */
#define USB_MODEM_COUNT_MAX 		4
#define MODEM_3G_FILE_MAX_SIZE	(USB_MODEM_COUNT_MAX * 512)
#define MODEM_CMD_INDEX_LEN		5
#define MODEM_CMD_TAG				"[TP-3G]"
#define MODEM_CMD_EQUAL			"="
#define MODEM_CMD_COUNT			"count"
#define MODEM_CMD_INDEX			"index"
#define MODEM_CMD_VID				"vid"
#define MODEM_CMD_PID				"pid"
#define MODEM_CMD_MSG				"msg"
#define MODEM_CMD_VENDOR_NAME	"vendor"
#define MODEM_CMD_MODEL_NAME	"model"
#define USB_MODEM_VENDOR_ID_MAX_HEX		4
#define USB_MODEM_PRODUCT_ID_MAX_HEX		4
#define MD5_DIGEST_LEN			16		// length of MD5 digest result

static UINT8 l_ckey_copy[8] = {0x47, 0x8D, 0xA5, 0x0B, 0xF9, 0xE3, 0xD2, 0xCF};

UBOOL8 cmsImg_isModem3gFile(const unsigned char *pAddr, UINT32 len)
{
	/* for decrypting uploaded file */
	UINT8 * pBuf = NULL;
	UINT8 *pRawBuf = NULL;
	UINT8 *pMd5Digest = NULL;
	int tmpBufLen = len + 10;
	int fileLength = 0;

	if(NULL == pAddr)
	{
		cmsLog_error("Invalid file address!");
		goto ERROR1;
	}

	/* check input file size */
	if (len > MODEM_3G_FILE_MAX_SIZE)
	{
		cmsLog_error("Invalid file length %d, max %d\n", len, MODEM_3G_FILE_MAX_SIZE);
		goto ERROR1;
	}

	if ((pBuf = (UINT8 *) cmsMem_alloc(tmpBufLen, 0)) == NULL)
	{
		cmsLog_error("Failed to allocate memory for the 3g settings. Size required %d", tmpBufLen);
		goto ERROR1;
	}
	else
	{
		cmsLog_error("%d bytes allocated for 3g setting data at %p", tmpBufLen, pBuf);
	}

	pRawBuf = pBuf;

	/* do DES decrypt */
	fileLength = cen_desMinDo(pAddr, len, pRawBuf, tmpBufLen,
							l_ckey_copy, DES_DECRYPT);

	if (0 == fileLength)
	{
		cmsLog_error("DES encrypt error\n");
       	goto ERROR2;
	}
	pMd5Digest = pRawBuf;
	/* skip MD5 digest */
	pRawBuf += MD5_DIGEST_LEN;

	/* get rid of paddings */
	while((0 == pMd5Digest[fileLength - 1]) && (fileLength > 0))
	{
		fileLength--;
	}

	if(fileLength <= MD5_DIGEST_LEN)
	{
		cmsLog_error("File(usb 3g) size %d is too small\n", fileLength);
		goto ERROR2;
	}

	/* verify md5 */
	if (0 == md5_verify_digest(pMd5Digest, pRawBuf, (fileLength - MD5_DIGEST_LEN)))
	{
		cmsLog_error("usb 3g bin file MD5 check fail\n");
		goto ERROR2;
	}

	/* check the decrypted file size. */
	if (strlen((char *)pRawBuf) > MODEM_3G_FILE_MAX_SIZE)
	{
		cmsLog_error("usb 3g file length %d error, max is %d\n", len, MODEM_3G_FILE_MAX_SIZE);
		goto ERROR2;
	}

	cmsLog_error("----- 3g modem image! %d-------", fileLength);
	cmsLog_error("pRawBuf \n%s", pRawBuf);

	CMSMEM_FREE_BUF_AND_NULL_PTR(pBuf);
	return TRUE;
	
ERROR2:
	CMSMEM_FREE_BUF_AND_NULL_PTR(pBuf);
ERROR1:	
	return FALSE;
}

CmsRet send3gSettingsMsg(const char *imagePtr, UINT32 imageLen, void *msgHandle, CmsMsgType msgType)
{
	char *buf=NULL;
	char *body=NULL;
	CmsMsgHeader *msg;
	CmsRet ret;

	if ((buf = cmsMem_alloc(sizeof(CmsMsgHeader) + imageLen, ALLOC_ZEROIZE)) == NULL)
	{
		cmsLog_error("failed to allocate %d bytes for msg 0x%x", 
		       sizeof(CmsMsgHeader) + imageLen, msgType);
		return CMSRET_RESOURCE_EXCEEDED;
	}

	msg = (CmsMsgHeader *) buf;
	body = (char *) (msg + 1);

	msg->type = msgType;
	msg->src = cmsMsg_getHandleEid(msgHandle);
	msg->dst = EID_SSK;
	msg->flags_request = 1;
	msg->dataLength = imageLen;

	memcpy(body, imagePtr, imageLen);
	
	ret = cmsMsg_send(msgHandle, msg);

	cmsMem_free(buf);
   
	return ret;
}

CmsRet cmsImg_write3gSettings(char *imagePtr, UINT32 imageLen, void *msgHandle)
{
	CmsRet ret;

	ret = send3gSettingsMsg(imagePtr, imageLen, msgHandle, CMS_MSG_WRITE_3G_SETTING_FILE);

	return ret;
}

#endif/*SUPPORT_USB_3G*/


/* 
 * fn		static UBOOL8 checkFwSign(UINT8 *pFirmwareAddr, UINT32 firmwareLength)
 * brief	Check image signature
 *
 * param[in]	pFirmwareAddr - address of firmware
 * param[in]	firmwareLength - length of firmware			
 */
static UBOOL8 checkFwSign(UINT8 *pFirmwareAddr, UINT32 firmwareLength)
{
	UINT8  public_key[] = 
		"BgIAAACkAABSU0ExAAQAAAEAAQCtdVIi5h5+e4v16PPyGj8o10hKva+bycG1F5TW7abW1RDK6PEanVepTEs0hVZgTL"
		"09z7taV3JyD7m2Mtfj6JTK0+U9VsPg61mvOmoHR0ibiy6mehK0KTqPO2gAUjmpAZhX32BKBFG8LPEJVNN0e/eeN1UD"
		"HpFwNzqYHdEF7tu4wA==";
	UINT32 publicKeyLen = sizeof(public_key) - 1;
	UINT8  signBuf[TP_RSA_SIG_LEN] = {0};
	UINT8  digst[MD5_DIGEST_LEN] = {0};
	int ret = 0;
	
	LINUX_FILE_TAG *pKernelTag = (LINUX_FILE_TAG *)pFirmwareAddr;
	//void *pKernelTag = (void *)pFirmwareAddr;

	memcpy(signBuf, pKernelTag->rsaSig, TP_RSA_SIG_LEN);
/*	memset(pKernelTag->rsaSig, 0, SIG_LEN);*/
	
	md5_make_digest(digst, pFirmwareAddr + (TP_TAG_LEN + TAG_LEN), firmwareLength - (TP_TAG_LEN + TAG_LEN));
	if (0)
	{
		int i = 0;
		printf("%s: digst\n", __FUNCTION__);
		for(i = 0; i < MD5_DIGEST_LEN; i++)
		{
			printf("%02x ", digst[i]);
		}
		printf("\n");
		
	}

	/* compatible two digital signature */
	ret = cen_rsaVerifySignByBase64EncodePublicKeyBlob(
						public_key,
						publicKeyLen,
						digst,
						MD5_DIGEST_LEN,
						signBuf,
						TP_RSA_SIG_LEN);
	
	/* restore rsa */
	memcpy(pKernelTag->rsaSig, signBuf, TP_RSA_SIG_LEN);

	return (ret == 1)?TRUE:FALSE;
}


/**
 * @return TRUE if the modem's chip id matches that of the image.
 */
UBOOL8 matchChipId(const char *strTagChipId, const char *signature2 __attribute__((unused)))
{
    UINT32 tagChipId = 0;
    UINT32 chipId; 
    UBOOL8 match;

    /* this is the image's chip id */
    tagChipId = (UINT32) strtol(strTagChipId, NULL, 16);
    
    /* get the system's chip id */
    devCtl_getChipId(&chipId);
    if (tagChipId == chipId)
    {
        match = TRUE;
    }
    else if (tagChipId == BRCM_CHIP_HEX)
    {
        match = TRUE;
    }
    else
    {
        cmsLog_error("Chip Id error.  Image Chip Id = %04x, Board Chip Id = %04x.", tagChipId, chipId);
        match = FALSE;
    }

    return match;
}
/* added by wwj from v4.0 sc for tp image verify 130226*/
CmsRet verifyTplinkFileTag(LINUX_FILE_TAG *pTag, int imageLen, int doCheck)
{
	/*printf("%s(): enter!\n", __FUNCTION__);*/
	UINT8 md5Key[16] = 
	{	/* linux - wr841n */
		0xDC, 0xD7, 0x3A, 0xA5, 0xC3, 0x95, 0x98, 0xFB, 
		0xDD, 0xF9, 0xE7, 0xF4, 0x0E, 0xAE, 0x47, 0x38
	};
	#if 0
	UINT8 md5Key_bootloader[16] =
	{	/* linux bootloader - u-boot/redboot */
			0x8C, 0xEF, 0x33, 0x5B, 0xD5, 0xC5, 0xCE, 0xFA,
			0xA7, 0x9C, 0x28, 0xDA, 0xB2, 0xE9, 0x0F, 0x42
	};
	#endif
	UINT8	*pMd5Key = NULL;
	UINT8	fileMd5Checksum[16];

	LINUX_FILE_TAG* tag = (LINUX_FILE_TAG *)pTag;

	if (pTag == NULL)
	{
		return CMSRET_INVALID_IMAGE;
	}
	/* Limits the size of the file. */
	if(imageLen < TFTP_MIN_APP_SIZE || imageLen > TFTP_MAX_APP_SIZE)
	{
		cmsLog_error("The file's length is bad: %d\n", imageLen);
		return CMSRET_INVALID_IMAGE;
	}
	/* yuanshang : 2010-12-15 check totalImageLen is right ? */
	/*printf("%s(): tag->totalImageLen %d, %d\n", 
					__FUNCTION__, tag->totalImageLen, imageLen - TP_TAG_LEN);*/
	if ( tag->totalImageLen != (imageLen - TP_TAG_LEN) )
	{
		cmsLog_error("Image totalImageLen is not correct!");
		return CMSRET_INVALID_IMAGE;
	}
	if (doCheck)
	{
		/* do MD5 checksum check now, by lqm, 22Apr04 */
		// save the checksum in downloaded file
		memcpy(fileMd5Checksum, tag->imageValidationToken, MD5_DIGEST_LEN);

		pMd5Key = md5Key;

		if (!md5_verify_digest(fileMd5Checksum, (UINT8 *)tag + TP_TAG_LEN, tag->totalImageLen))
		{
			cmsLog_error("Image md5 checksum is not correct!");
			return CMSRET_INVALID_IMAGE;
		}
		/* restore checksum */
		memcpy(tag->imageValidationToken, fileMd5Checksum, MD5_DIGEST_LEN);

		/* verify the tag+fs_kernel MD5 checksum */
		memcpy(fileMd5Checksum, tag->tpImageValidationToken, MD5_DIGEST_LEN);
		
		memcpy(tag->tpImageValidationToken, pMd5Key, MD5_DIGEST_LEN);

		if (!md5_verify_digest(fileMd5Checksum, (UINT8 *)tag, tag->totalImageLen + TP_TAG_LEN))
		{
			cmsLog_error("tp-tagged image md5 checksum is not correct!");
			return CMSRET_INVALID_IMAGE;
		}
		/* restore checksum */
		memcpy(tag->tpImageValidationToken, fileMd5Checksum, MD5_DIGEST_LEN);

		/* RSA signature from "tpbsp", yuanshang, 2013-05-28 */
		if (!checkFwSign((UINT8 *)pTag, imageLen))
		{
			cmsLog_error("Image RSA Signature is not correct!");
			return CMSRET_INVALID_IMAGE;
		}

		
		
	}
	/* Version check --  lsz */
	if (tag->productId != PRODUCT_ID || tag->productVer != PRODUCT_VER)
	{
		cmsLog_error("Firmware version check failed!");
		return CMSRET_INVALID_IMAGE;
	}
	/* RSA signature from "tpbsp", write to nvram, yuanshang, 2013-05-28 */
	if (doCheck){
		/* backup RSA signature and length to nvram, yuanshang, 2013-05-28 */
		//#define NVRAM_RSA_FILE	"/proc/nvram/RsaSig"
		//#define NVRAM_LEN_FILE	"/proc/nvram/fs_kernel_len"
		FILE *fp;
		
		if ((fp = fopen(NVRAM_RSA_FILE, "w")) != NULL)
		{
			int j=0;
			char strRsa[TP_RSA_SIG_LEN * 2 + 1]={0};
			printf("Restore RSA Signature...\n");
			
			for (j = 0; j < TP_RSA_SIG_LEN; j++)
			{
				sprintf(strRsa + j * 2, "%02x", tag->rsaSig[j]);
			}
			fputs(strRsa, fp);
			fclose(fp);

			/*system("cat /proc/nvram/RsaSig");*/
		}
		if ((fp = fopen(NVRAM_LEN_FILE, "w")) != NULL)
		{
			char length[20]={0};
			printf("Restore fs_kernel length...\n");
			sprintf(length, "%d", imageLen - (TP_TAG_LEN + TAG_LEN));
			fputs(length, fp);
			fclose(fp);
			/*system("cat /proc/nvram/fs_kernel_len");*/
		}
	}
	/* todo, check tag->hardwareVer */

	cmsLog_notice("** Recv a correct TD-%s firmware. **",PRODUCT_MODEL);
	
	return CMSRET_SUCCESS;
}
/* end added */
// verify the tag of the image
CmsRet verifyBroadcomFileTag(FILE_TAG *pTag, int imageLen)
{
    UINT32 crc;
    int totalImageSize;
    int tagVer, curVer;
    UINT32 tokenCrc, imageCrc;

        
    cmsLog_debug("start of pTag=%p tagversion %02x %02x %02x %02x", pTag,
                  pTag->tagVersion[0], 
                  pTag->tagVersion[1], 
                  pTag->tagVersion[2], 
                  pTag->tagVersion[3]);

    tokenCrc = *((UINT32 *)pTag->tagValidationToken);
    imageCrc = *((UINT32 *)pTag->imageValidationToken);                  
#ifdef DESKTOP_LINUX
    /* assume desktop is running on little endien intel, but the CRC has been
     * written for big endien mips, so swap before compare.
     */
    tokenCrc = htonl(tokenCrc);
    imageCrc = htonl(imageCrc);
#endif
                  
                  
    // check tag validate token first
    crc = CRC_INITIAL_VALUE;
    crc = cmsCrc_getCrc32((UINT8 *) pTag, (UINT32)TAG_LEN-TOKEN_LEN, crc);
    if (crc != tokenCrc)
    {
        /* this function is called even when we are not sure the image is
         * a broadcom image.  So if crc fails, it is not a big deal.  It just
         * means this is not a broadcom image.
         */
        cmsLog_debug("token crc failed, this is not a valid broadcom image");
        cmsLog_debug("calculated crc=0x%x tokenCrc=0x%x", crc, tokenCrc);
        return CMSRET_INVALID_IMAGE;
    }
    cmsLog_debug("header CRC is OK.");

    
    // check imageCrc
    totalImageSize = atoi((char *) pTag->totalImageLen);
    cmsLog_debug("totalImageLen=%d, imageLen=%d, TAG_LEN=%d\n", totalImageSize, imageLen, TAG_LEN);

    if (totalImageSize > (imageLen -TAG_LEN)) {
	 cmsLog_error("invalid size\n");
        return CMSRET_INVALID_IMAGE;
	}
    crc = CRC_INITIAL_VALUE;
    crc = cmsCrc_getCrc32(((UINT8 *)pTag + TAG_LEN), (UINT32) totalImageSize, crc);      
    if (crc != imageCrc)
    {
        /*
         * This should not happen.  We already passed the crc check on the header,
         * so we should pass the crc check on the image.  If this fails, something
         * is wrong.
         */
        cmsLog_error("image crc failed after broadcom header crc succeeded");
        cmsLog_error("calculated crc=0x%x imageCrc=0x%x totalImageSize", crc, imageCrc, totalImageSize); 
        return CMSRET_INVALID_IMAGE;
    }
    cmsLog_debug("image crc is OK");


    tagVer = atoi((char *) pTag->tagVersion);
    curVer = atoi(BCM_TAG_VER);

    if (tagVer != curVer)

    {
       cmsLog_error("Firmware tag version [%d] is not compatible with the current Tag version [%d]", tagVer, curVer);
       return CMSRET_INVALID_IMAGE;
    }
    cmsLog_debug("tarVer=%d, curVar=%d", tagVer, curVer);

    if (!matchChipId((char *) pTag->chipId, (char *) pTag->signiture_2))
    {
       cmsLog_error("chipid check failed");
       return CMSRET_INVALID_IMAGE;
    }

    cmsLog_debug("Good broadcom image");

    return CMSRET_SUCCESS;
}


#ifdef DMP_DEVICE2_HOMEPLUG_1
CmsRet cmsImg_setPLCconfigState(const char *state)
{
  if (cmsFil_writeToProc("/data/plc/plc_pconfig_state", state) != CMSRET_SUCCESS)
  {
    cmsLog_error("[PLC] Can't write the paramconfig state\n");
    return CMSRET_INTERNAL_ERROR;
  }

  return CMSRET_SUCCESS;
}
#endif


// depending on the image type, do the brcm image or whole flash image
CmsRet flashImage(char *imagePtr, UINT32 imageLen, CmsImageFormat format)
{
    FILE_TAG *pTag = (FILE_TAG *) imagePtr;
    int cfeSize, rootfsSize, kernelSize, noReboot;
    unsigned long cfeAddr, rootfsAddr, kernelAddr;
    CmsRet ret;
 
    // Extract partition and reboot bits from the 'format' variable to
    // a separate variable, 'noReboot'.
    if( (format & CMS_IMAGE_FORMAT_PART1) == CMS_IMAGE_FORMAT_PART1 )
    {
        noReboot = ((format & CMS_IMAGE_FORMAT_NO_REBOOT) == 0)
            ? FLASH_PART1_REBOOT : FLASH_PART1_NO_REBOOT;
    }
    else if( (format & CMS_IMAGE_FORMAT_PART2) == CMS_IMAGE_FORMAT_PART2 )
    {
        noReboot = ((format & CMS_IMAGE_FORMAT_NO_REBOOT) == 0)
            ? FLASH_PART2_REBOOT : FLASH_PART2_NO_REBOOT;
    }
    else
        noReboot = ((format & CMS_IMAGE_FORMAT_NO_REBOOT) == 0)
            ? FLASH_PARTDEFAULT_REBOOT : FLASH_PARTDEFAULT_NO_REBOOT;

    // Clear unneeded partition and reboot bits from 'format' variable
    format &= ~(CMS_IMAGE_FORMAT_NO_REBOOT | CMS_IMAGE_FORMAT_PART1 |
       CMS_IMAGE_FORMAT_PART2); 

    if (format != CMS_IMAGE_FORMAT_FLASH && format != CMS_IMAGE_FORMAT_BROADCOM)
    {
       cmsLog_error("invalid image format %d", format);
       return CMSRET_INVALID_IMAGE;
    }

#ifdef DMP_DEVICE2_HOMEPLUG_1
    cmsImg_setPLCconfigState("2");
#endif

    if (format == CMS_IMAGE_FORMAT_FLASH)  
    {
        cmsLog_notice("Flash whole image...");
        // Pass zero for the base address of whole image flash. It will be filled by kernel code
        // was sysFlashImageSet
        ret = devCtl_boardIoctl(BOARD_IOCTL_FLASH_WRITE,
                                BCM_IMAGE_WHOLE,
                                imagePtr,
                                imageLen-TOKEN_LEN,
                                noReboot, 0);
        if (ret != CMSRET_SUCCESS)
        {
           cmsLog_error("Failed to flash whole image");
           return CMSRET_IMAGE_FLASH_FAILED;
        }
        else
        {
           return CMSRET_SUCCESS;
        }
    }

    /* this must be a broadcom format image */
    // check imageCrc
    cfeSize = rootfsSize = kernelSize = 0;

    // check cfe's existence
    cfeAddr = (unsigned long) strtoul((char *) pTag->cfeAddress, NULL, 10);
    cfeSize = atoi((char *) pTag->cfeLen);
    // check kernel existence
    kernelAddr = (unsigned long) strtoul((char *) pTag->kernelAddress, NULL, 10);
    kernelSize = atoi((char *) pTag->kernelLen);
    // check root filesystem existence
    rootfsAddr = (unsigned long) strtoul((char *) pTag->rootfsAddress, NULL, 10);
    rootfsSize = atoi((char *) pTag->rootfsLen);
    cmsLog_debug("cfeSize=%d kernelSize=%d rootfsSize=%d", cfeSize, kernelSize, rootfsSize);
    
    if (cfeAddr) 
    {
        printf("Flashing CFE...\n");
	fflush(stdout);
        ret = devCtl_boardIoctl(BOARD_IOCTL_FLASH_WRITE,
                                BCM_IMAGE_CFE,
                                imagePtr+TAG_LEN,
                                cfeSize,
                                (int) cfeAddr, 0);
        if (ret != CMSRET_SUCCESS)
        {
            cmsLog_error("Failed to flash CFE");
            return CMSRET_IMAGE_FLASH_FAILED;
        }
    }

    if (rootfsAddr && kernelAddr) 
    {
        char *tagFs = imagePtr;

        // tag is alway at the sector start of fs
        if (cfeAddr)
        {
            tagFs = imagePtr + cfeSize;       // will trash cfe memory, but cfe is already flashed
            memcpy(tagFs, imagePtr, TAG_LEN);
        }

        printf("Flashing root file system and kernel...\n");
	fflush(stdout);
        /* only the buf pointer and length is needed, the offset parameter
         * was present in the legacy code, but is not used. */
        ret = devCtl_boardIoctl(BOARD_IOCTL_FLASH_WRITE,
                                BCM_IMAGE_FS,
                                tagFs,
                                TAG_LEN+rootfsSize+kernelSize,
                                noReboot, 0);
        if (ret != CMSRET_SUCCESS)
        {
            cmsLog_error("Failed to flash root file system and kernel");
            return CMSRET_IMAGE_FLASH_FAILED;
        }
    }

    cmsLog_notice("Image flash done.");
    
    return CMSRET_SUCCESS;
}


UINT32 cmsImg_getImageFlashSize(void)
{
   UINT32 flashSize=0;
   CmsRet ret;
   
   ret = devCtl_boardIoctl(BOARD_IOCTL_FLASH_READ,
                           FLASH_SIZE,
                           0, 0, 0, &flashSize);
   if (ret != CMSRET_SUCCESS)
   {
      cmsLog_error("Could not get flash size, return 0");
      flashSize = 0;
   }
   
   return flashSize;
}


UINT32 cmsImg_getBroadcomImageTagSize(void)
{
   return TOKEN_LEN;
}


UINT32 cmsImg_getConfigFlashSize(void)
{
   UINT32 realSize;

   realSize = cmsImg_getRealConfigFlashSize();

#ifdef COMPRESSED_CONFIG_FILE   
   /*
    * A multiplier of 2 is now too small for some of the big voice and WLAN configs,   
    * so allow for the possibility of 4x compression ratio.  In a recent test on the
    * 6368 with wireless enabled, I got a compression ratio of 3.5.
    * The real test comes in management.c: after we do the
    * compression, writeValidatedConfigBuf will verify that the compressed buffer can
    * fit into the flash.
    * A 4x multiplier should be OK for small memory systems such as the 6338.
    * The kernel does not allocate physical pages until they are touched.
    * However, allocating an overly large buffer could be rejected immediately by the
    * kernel because it does not know we don't actually plan to use the entire buffer.
    * So if this is a problem on the 6338, we could improve this algorithm to
    * use a smaller factor on low memory systems.
    */
   realSize = realSize * 4;
#endif

   return realSize;
}


UINT32 cmsImg_getRealConfigFlashSize(void)
{
   CmsRet ret;
   UINT32 size=0;

   ret = devCtl_boardIoctl(BOARD_IOCTL_GET_PSI_SIZE, 0, NULL, 0, 0, (void *)&size);
   if (ret != CMSRET_SUCCESS)
   {
      cmsLog_error("boardIoctl to get config flash size failed.");
      size = 0;
   }

   return size;
}


UBOOL8 cmsImg_willFitInFlash(UINT32 imageSize)
{
   UINT32 flashSize;
   
   flashSize = cmsImg_getImageFlashSize();

   cmsLog_debug("flash size is %u bytes, imageSize=%u bytes", flashSize, imageSize);
                     
   return (flashSize > (imageSize + CMS_IMAGE_OVERHEAD));
}


UBOOL8 cmsImg_isBackupConfigFlashAvailable(void)
{
   static UBOOL8 firstTime=TRUE;
   static UBOOL8 avail=FALSE;
   UINT32 size=0;
   CmsRet ret;

   if (firstTime)
   {
      ret = devCtl_boardIoctl(BOARD_IOCTL_GET_BACKUP_PSI_SIZE, 0, NULL, 0, 0, (void *)&size);
      if (ret == CMSRET_SUCCESS && size > 0)
      {
         avail = TRUE;
      }

      firstTime = FALSE;
   }

   return avail;
}


UBOOL8 cmsImg_isConfigFileLikely(const char *buf)
{
   const char *header = "<?xml version";
   const char *dslCpeConfig = "<DslCpeConfig";
   UINT32 len, i;
   UBOOL8 likely=FALSE;
   
   if (strncmp(buf, "<?xml version", strlen(header)) == 0)
   {
      len = strlen(dslCpeConfig);
      for (i=20; i<50 && !likely; i++)
      {
         if (strncmp(&(buf[i]), dslCpeConfig, len) == 0)
         {
            likely = TRUE;
         }
      }
   }
   
   cmsLog_debug("returning likely=%d", likely);
   
   return likely;
}


/** General entry point for writing the image.
 *  The image can be a flash image or a config file.
 *  This function will first determine the image type, which has the side
 *  effect of validating it.
 */
CmsRet cmsImg_writeImage(char *imagePtr, UINT32 imageLen, void *msgHandle)
{
   CmsImageFormat format;
   CmsRet ret;
   
   if ((format = cmsImg_validateImage(imagePtr, imageLen, msgHandle)) == CMS_IMAGE_FORMAT_INVALID)
   {
      ret = CMSRET_INVALID_IMAGE;
   }
   else
   {
      ret = cmsImg_writeValidatedImage(imagePtr, imageLen, format, msgHandle);
   }

   return ret;
}


CmsRet cmsImg_writeValidatedImage(char *imagePtr, UINT32 imageLen, CmsImageFormat format, void *msgHandle)
{
   CmsRet ret=CMSRET_SUCCESS;
   
   switch(format & ~(CMS_IMAGE_FORMAT_NO_REBOOT | CMS_IMAGE_FORMAT_PART1 | CMS_IMAGE_FORMAT_PART2))
   {
   case CMS_IMAGE_FORMAT_BROADCOM:
   case CMS_IMAGE_FORMAT_FLASH:
      // BcmNtwk_unInit(); mwang_todo: is it important to let Wireless do some
      // uninit before we write the flash image?
      /* before flash image, we should strip tplink tag first, wwj, 130226 from v4.0 sc */
	  /* Check TPLINK FileTag First, Yuan Shang, 2011-11-29 */
		if (verifyTplinkFileTag((LINUX_FILE_TAG *) imagePtr, imageLen, 0) != CMSRET_SUCCESS)
		{
			cmsLog_error("Invalid TPLINK image");
			return CMS_IMAGE_FORMAT_INVALID;
		}
		printf("%s(): Recv a correct TD-%s firmware, Strip tp-tag\n", 
						__FUNCTION__, PRODUCT_MODEL);
		imagePtr = imagePtr + TP_TAG_LEN;
		imageLen -= TP_TAG_LEN;
		printf("%s(): Flash Image...\n", __FUNCTION__);
      /* end added */
      ret = flashImage(imagePtr, imageLen, format);
      break;
      
   case CMS_IMAGE_FORMAT_XML_CFG:
      ret = sendConfigMsg(imagePtr, imageLen, msgHandle, CMS_MSG_WRITE_CONFIG_FILE);
      if (ret == CMSRET_SUCCESS)
      {
         /*
          * Emulate the behavior of the driver when a flash image is written.
          * When we write a config image, also request immediate reboot
          * because we don't want to let any other app save the config file
          * to flash, thus wiping out what we just written.
          */
         cmsLog_debug("config file download written, request reboot");
         cmsUtil_sendRequestRebootMsg(msgHandle);
      }
      break;
      
#ifdef SUPPORT_MOD_SW_UPDATE
   case CMS_IMAGE_FORMAT_MODSW_PKG:
   {
      char filename[BUFLEN_1024]={0};

      cmsFil_removeDir(SW_UPDATE_DATA_DIR);
      cmsFil_makeDir(SW_UPDATE_DATA_DIR);

      snprintf(filename, sizeof(filename)-1, "%s/%s",
                         SW_UPDATE_DATA_DIR, SW_UPDATE_PKG_FILE);
      cmsFil_writeBufferToFile(filename, (UINT8 *)imagePtr, imageLen);
      /*
       * modupdtd will unpack the sw package, and then send a graceful
       * shutdown msg to smd.
       */
      sendStartModupdtdMsg(msgHandle);
      break;
   }
#endif

   default:
       cmsLog_error("Unrecognized image format=%d", format);
       ret = CMSRET_INVALID_IMAGE;
       break;
    }
   
   return ret;
}

 
CmsImageFormat cmsImg_validateImage(const char *imageBuf, UINT32 imageLen, void *msgHandle)
{
   CmsImageFormat result = CMS_IMAGE_FORMAT_INVALID;
   CmsRet ret;
   
   if (imageBuf == NULL)
   {
      return CMS_IMAGE_FORMAT_INVALID;
   }
   
   if (imageLen > CMS_CONFIG_FILE_DETECTION_LENGTH &&
       cmsImg_isConfigFileLikely(imageBuf))
   {
      cmsLog_debug("possible CMS XML config file format detected");
      ret = sendConfigMsg(imageBuf, imageLen, msgHandle, CMS_MSG_VALIDATE_CONFIG_FILE);
      if (ret == CMSRET_SUCCESS)
      { 
         cmsLog_debug("CMS XML config format verified.");
         return CMS_IMAGE_FORMAT_XML_CFG;
      }
   } 
   
   cmsLog_debug("not a config file");
   
#ifdef SUPPORT_MOD_SW_UPDATE
   if (cmsImg_isModSwPkg((unsigned char *) imageBuf, imageLen))
   {
      cmsLog_debug("detected modular sw pkg format!");
      return CMS_IMAGE_FORMAT_MODSW_PKG;
   }

   cmsLog_debug("not a modular sw pkg");
#endif

/*Hanjiayan add for usb 3G*/
#ifdef SUPPORT_USB_3G
	if(cmsImg_isModem3gFile((unsigned char *) imageBuf, imageLen))
	{
		cmsLog_debug("detected 3g modem settings format!");
		return CMS_IMAGE_FORMAT_3G_SETTINGS;
	}
#endif

/* Check TPLINK FileTag First, wwj, 130226 from v4.0 */
	if (verifyTplinkFileTag((LINUX_FILE_TAG *) imageBuf, imageLen, 1) != CMSRET_SUCCESS)
	{
		cmsLog_error("Invalid TPLINK image");
		return CMS_IMAGE_FORMAT_INVALID;
	}
	imageBuf = imageBuf + TP_TAG_LEN;	/* temporarily strip tp header for BCM check */
	imageLen -= TP_TAG_LEN;
/* end added */

   if ((imageLen > sizeof(FILE_TAG)) 
        && (verifyBroadcomFileTag((FILE_TAG *) imageBuf, imageLen) == CMSRET_SUCCESS))
   {
      UINT32 maxLen;
      
      /* Found a valid Broadcom defined TAG record at the beginning of the image */
      cmsLog_debug("Broadcom format verified.");
      maxLen = cmsImg_getImageFlashSize() + cmsImg_getBroadcomImageTagSize();
      if (imageLen > maxLen)
      {
         cmsLog_error("broadcom image is too large for flash, got %u, max %u", imageLen, maxLen);
      }
      else
      {
         result = CMS_IMAGE_FORMAT_BROADCOM;
      }
   }
   else
   {
      /* if It is not a Broadcom flash format file.  Now check if it is a
       * flash image format file.  A flash image format file must have a
       * CRC at the end of the image.
       */
      UINT32 crc = CRC_INITIAL_VALUE;
      UINT32 imageCrc;
      UINT8 *crcPtr;
      
      if (imageLen > TOKEN_LEN)
      {
         crcPtr = (UINT8 *) (imageBuf + (imageLen - TOKEN_LEN));
         /*
          * CRC may not be word aligned, so extract the bytes out one-by-one.
          * Whole image CRC is calculated, then htonl, then written out using
          * fwrite (see addvtoken.c in hostTools).  Because of the way we are
          * extracting the CRC here, we don't have to swap for endieness when
          * doing compares on desktop Linux and modem (?).
          */
         imageCrc = (crcPtr[0] << 24) | (crcPtr[1] << 16) | (crcPtr[2] << 8) | crcPtr[3];
      
         crc = cmsCrc_getCrc32((unsigned char *) imageBuf, imageLen - TOKEN_LEN, crc);      
         if (crc == imageCrc)
         {
            UINT32 maxLen;
          
            cmsLog_debug("Whole flash image format [%ld bytes] verified.", imageLen);
            maxLen = cmsImg_getImageFlashSize();
            if (imageLen > maxLen)
            {
               cmsLog_error("whole image is too large for flash, got %u, max %u", imageLen, maxLen);
            }
            else
            {
               result = CMS_IMAGE_FORMAT_FLASH;
            }
         }
         else
         {
#if defined(EPON_SDK_BUILD)
            cmsLog_debug("Could not determine image format [%d bytes]", imageLen);
#else
            cmsLog_error("Could not determine image format [%d bytes]", imageLen);
#endif
            cmsLog_debug("calculated crc=0x%x image crc=0x%x", crc, imageCrc);
         }
      }
   }

   cmsLog_debug("returning image format %d", result);

   return result;
}

CmsRet sendConfigMsg(const char *imagePtr, UINT32 imageLen, void *msgHandle, CmsMsgType msgType)
{
   char *buf=NULL;
   char *body=NULL;
   CmsMsgHeader *msg;
   CmsRet ret;

   if ((buf = cmsMem_alloc(sizeof(CmsMsgHeader) + imageLen, ALLOC_ZEROIZE)) == NULL)
   {
      cmsLog_error("failed to allocate %d bytes for msg 0x%x", 
                   sizeof(CmsMsgHeader) + imageLen, msgType);
      return CMSRET_RESOURCE_EXCEEDED;
   }
   
   msg = (CmsMsgHeader *) buf;
   body = (char *) (msg + 1);
    
   msg->type = msgType;
   msg->src = cmsMsg_getHandleEid(msgHandle);
   msg->dst = EID_SMD;
   msg->flags_request = 1;
   msg->dataLength = imageLen;
   
   memcpy(body, imagePtr, imageLen);

   ret = cmsMsg_sendAndGetReply(msgHandle, msg);
   
   cmsMem_free(buf);
   
   return ret;
}


void cmsImg_sendLoadStartingMsg(void *msgHandle, const char *connIfName)
{
   CmsMsgHeader *msg;
   char *data;
   void *msgBuf;
   UINT32 msgDataLen=0;

   /* for the msg and the connIfName */
   if (connIfName)
   {
      msgDataLen = strlen(connIfName) + 1;
      msgBuf = cmsMem_alloc(sizeof(CmsMsgHeader) + msgDataLen, ALLOC_ZEROIZE);
   } 
   else
   {
      cmsLog_error("msg without connIfName");
      msgBuf = cmsMem_alloc(sizeof(CmsMsgHeader), ALLOC_ZEROIZE);
   }
   
   msg = (CmsMsgHeader *)msgBuf;
   msg->src = cmsMsg_getHandleEid(msgHandle);
   msg->dst = EID_SMD;
   msg->flags_request = 1;
   msg->type = CMS_MSG_LOAD_IMAGE_STARTING;

   if (connIfName)
   {
      data = (char *) (msg + 1);
      msg->dataLength = msgDataLen;
      memcpy(data, (char *)connIfName, msgDataLen);      
   }
   
   cmsLog_debug("connIfName=%s", connIfName);

   cmsMsg_sendAndGetReply(msgHandle, msg);

   CMSMEM_FREE_BUF_AND_NULL_PTR(msgBuf);

}


void cmsImg_sendLoadDoneMsg(void *msgHandle)
{
   CmsMsgHeader msg = EMPTY_MSG_HEADER;

   msg.type = CMS_MSG_LOAD_IMAGE_DONE;
   msg.src = cmsMsg_getHandleEid(msgHandle);
   msg.dst = EID_SMD;
   msg.flags_request = 1;
   
   cmsMsg_sendAndGetReply(msgHandle, &msg);
}


#ifdef SUPPORT_MOD_SW_UPDATE
void sendStartModupdtdMsg(void *msgHandle)
{
   CmsMsgHeader msg = EMPTY_MSG_HEADER;

   msg.type = CMS_MSG_START_APP;
   msg.src = cmsMsg_getHandleEid(msgHandle);
   msg.dst = EID_SMD;
   msg.wordData = EID_MODUPDTD;
   msg.flags_request = 1;

   cmsMsg_sendAndGetReply(msgHandle, &msg);
}
#endif


/* using a cmsUtil_ prefix because this can be used in non-upload scenarios */
void cmsUtil_sendRequestRebootMsg(void *msgHandle)
{
   CmsMsgHeader msg = EMPTY_MSG_HEADER;

   msg.type = CMS_MSG_REBOOT_SYSTEM;
   msg.src = cmsMsg_getHandleEid(msgHandle);
   msg.dst = EID_SMD;
   msg.flags_request = 1;

   cmsMsg_sendAndGetReply(msgHandle, &msg);
}


CmsRet cmsImg_saveIfNameFromSocket(SINT32 socketfd, char *connIfName)
{
  
   SINT32 i = 0;
   SINT32 fd = 0;
   SINT32 numifs = 0;
   UINT32 bufsize = 0;
   struct ifreq *all_ifr = NULL;
   struct ifconf ifc;
   struct sockaddr local_addr;
   socklen_t local_len = sizeof(struct sockaddr_in);

   if (socketfd < 0 || connIfName == NULL)
   {
      cmsLog_error("cmsImg_saveIfNameFromSocket: Invalid parameters: socket=%d, connIfName=%s", socketfd, connIfName);
      return CMSRET_INTERNAL_ERROR;
   }
   memset(&ifc, 0, sizeof(struct ifconf));
   memset(&local_addr, 0, sizeof(struct sockaddr));
   
   if (getsockname(socketfd, &local_addr,&local_len) < 0) 
   {
      cmsLog_error("cmsImg_saveIfNameFromSocket: Error in getsockname.");
      return CMSRET_INTERNAL_ERROR;
   }

   /* cmsLog_error("cmsImg_saveIfNameFromSocket: Session comes from: %s", inet_ntoa(((struct sockaddr_in *)&local_addr)->sin_addr)); */
   
   if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
   {
      cmsLog_error("cmsImg_saveIfNameFromSocket: Error openning socket when getting socket intface info");
      return CMSRET_INTERNAL_ERROR;
   }
   
   numifs = 16;

   bufsize = numifs*sizeof(struct ifreq);
   all_ifr = (struct ifreq *)cmsMem_alloc(bufsize, ALLOC_ZEROIZE);
   if (all_ifr == NULL) 
   {
      cmsLog_error("cmsImg_saveIfNameFromSocket: out of memory");
      close(fd);
      return CMSRET_INTERNAL_ERROR;
   }

   ifc.ifc_len = bufsize;
   ifc.ifc_buf = (char *)all_ifr;
   if (ioctl(fd, SIOCGIFCONF, &ifc) < 0) 
   {
      cmsLog_error("cmsImg_saveIfNameFromSocket: Error getting interfaces\n");
      close(fd);
      cmsMem_free(all_ifr);
      return CMSRET_INTERNAL_ERROR;
   }

   numifs = ifc.ifc_len/sizeof(struct ifreq);
   /* cmsLog_error("cmsImg_saveIfNameFromSocket: numifs=%d\n",numifs); */
   for (i = 0; i < numifs; i ++) 
   {
	  /* cmsLog_error("cmsImg_saveIfNameFromSocket: intface name=%s\n", all_ifr[i].ifr_name); */
	  struct in_addr addr1,addr2;
	  addr1 = ((struct sockaddr_in *)&(local_addr))->sin_addr;
	  addr2 = ((struct sockaddr_in *)&(all_ifr[i].ifr_addr))->sin_addr;
	  if (addr1.s_addr == addr2.s_addr) 
	  {
	      strcpy(connIfName, all_ifr[i].ifr_name);
	  	break;
	  }
   }

   close(fd);
   cmsMem_free(all_ifr);

   cmsLog_debug("connIfName=%s", connIfName);

   return CMSRET_SUCCESS;
   
}

