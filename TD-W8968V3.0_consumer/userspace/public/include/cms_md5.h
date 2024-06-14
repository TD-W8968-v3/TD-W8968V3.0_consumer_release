/*  Copyright(c) 2009-2013 Shenzhen TP-LINK Technologies Co.Ltd.
 *
 * file		cms_md5.h
 * brief		
 * details	
 *
 * author		Han Jiayan
 * version	
 * date		25Oct13
 *
 * history 	\arg	
 */


#ifndef __CMS_MD5_H__
#define __CMS_MD5_H__

#define MD5_HASH_SIZE		16

void md5_make_digest(unsigned char* digest, unsigned char* input, int len);

int md5_verify_digest(unsigned char* digest, unsigned char* input, int len);

void hmac_md5(unsigned char* text, int text_len, unsigned char* key, 
			int key_len, unsigned char * digest);


#endif /* __CMS_MD5_H__ */

