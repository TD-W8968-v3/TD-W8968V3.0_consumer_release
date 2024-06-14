/*  Copyright(c) 2009-2013 Shenzhen TP-LINK Technologies Co.Ltd.
 *
 * file		encrypt.h
 * brief		
 * details	
 *
 * author		Han Jiayan
 * version	
 * date		25Oct13
 *
 * history 	\arg	
 */
#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

#define DES_DECRYPT 0
#define DES_ENCRYPT 1

int cen_desMinDo(const unsigned char *in, 
			   	 int inLen, 
			   	 unsigned char *out, 
			   	 int outLen, 
			   	 const unsigned char *key, 
			   	 int enc);

#endif/*__ENCRYPT_H__*/