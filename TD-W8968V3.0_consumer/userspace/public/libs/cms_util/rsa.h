#ifndef TP_RSA
#define TP_RSA

/* 
 * fn		int cen_rsaVerifySignByBase64EncodePublicKeyBlob(unsigned char *pPublicKeyBlob,
 *			 												 unsigned long PublicKeyBlobLen,
 *			 												 unsigned char *pInData,      
 *			 												 unsigned long inDataLen,		   
 *			 												 unsigned char *PInSignature,   
 *			 												 unsigned long inSignatureLen)
 * brief	Check signature with public key
 * details	
 *
 * param[in]	pPublicKeyBlob - public key	
 * param[in]	PublicKeyBlobLen - public key length
 * param[in]	pInData	- mod number
 * param[in]	inDataLen - mod number length
 * param[in]	PInSignature - signature	
 * param[in]	inSignatureLen - signature length
 *
 * return	1 is returned if signature is OK, otherwise the return value is 0 		
 */
int cen_rsaVerifySignByBase64EncodePublicKeyBlob(unsigned char *pPublicKeyBlob,
 												 unsigned long PublicKeyBlobLen,
 												 unsigned char *pInData,      
 												 unsigned long inDataLen,		   
 												 unsigned char *PInSignature,   
 												 unsigned long inSignatureLen);



/* 
 * fn		int cen_rsaVerifySignature(unsigned char *in_pp,			//p
 *									   unsigned long  in_pp_len,
 *									   unsigned char *in_nn,			//n
 *									   unsigned long  in_nn_len,
 *									   unsigned char *in_data,			//data
 *									   unsigned long  in_data_len,		   
 *									   unsigned char *in_Signature,		//sign   
 *									   unsigned long  in_Signature_len)
 * brief	verify signature	
 *
 * param[in]	in_pp - p's content
 * param[in]	in_pp_len - p's length
 * param[in]	in_nn - n's content
 * param[in]	in_nn_len - n's length
 * param[in]	in_data - data's content
 * param[in]	in_data_len - data's length
 * param[in]	in_Signature - signature's content
 * param[in]	in_Signature_len - signature's length
 *
 * return	1 is returned if signature is OK, otherwise the return value is 0 	
 */
int cen_rsaVerifySignature(unsigned char *in_pp,
						   unsigned long  in_pp_len,
						   unsigned char *in_nn,
						   unsigned long  in_nn_len,
						   unsigned char *in_data,
						   unsigned long  in_data_len,   
						   unsigned char *in_Signature, 
						   unsigned long  in_Signature_len);

#endif /* TP_RSA */

