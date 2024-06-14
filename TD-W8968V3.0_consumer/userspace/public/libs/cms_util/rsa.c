
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rsa.h"

#include "shaAndSha512.h"
#include "bigNumber.h"

//#define TEST_RSA
#undef TEST_RSA
#if 1
typedef struct _MY_PUBLICKEYSTRUC {
	char bType;
	char bVersion;
	short reserved;
	unsigned long aiKeyAlg;
}MY_BLOBHEADER, MY_PUBLICKEYSTRUC;

typedef struct _MY_RSAPUBKEY {
	unsigned long magic;
	unsigned long bitlen;
	unsigned long pubexp;
} MY_RSAPUBKEY;

typedef struct _MY_RSA_PUBLICK_BLOB{
	MY_PUBLICKEYSTRUC blob_header;
	MY_RSAPUBKEY      rsa_pub_key;
}MY_RSA_PUBLICK_BLOB;


struct RSAKey {
	int bits;
	int bytes;
	Bignum modulus;
	Bignum exponent;
	Bignum private_exponent;
	Bignum p;
	Bignum q;
	Bignum iqmp;
	char *comment;
};


static int  rsaVerifySign(
     unsigned char *public_key_blob,
	 unsigned long  public_key_blob_len,
	 unsigned char *in_data,      
  	 unsigned long  in_data_len,		   
	 unsigned char *in_Signature,   
	 unsigned long  in_Signature_len);


static unsigned long rsa_read_long_from_public_key(char *bit)
{
    unsigned long re =0;
	re = ((bit[0])&(0xFF)) |
		(((bit[1])&(0xFF))<<8) |
		(((bit[2])&(0xFF))<<16) |
		(((bit[3])&(0xFF))<<24);
   return re;
}


static void *rsa2_newkey_ex(char *data, int len)
{
    char *p;
    unsigned long bit_len;
    struct RSAKey *rsa;
	char temp_buf[256];
	int i;
	MY_RSA_PUBLICK_BLOB *rsa_pub_blob;
	rsa_pub_blob = (MY_RSA_PUBLICK_BLOB *)data;
	
    rsa = snewn(1,struct RSAKey);
    if (!rsa)
		return NULL;
	//������δ�����ר��MS PUBKEYBLOB �ṹ�õ�,��ע��
    
	p = data + sizeof(MY_PUBLICKEYSTRUC);           //RSAPUBKEY
	if(memcmp(p, "RSA1", 4))
	{
		goto error_exit;
	}

	bit_len = rsa_read_long_from_public_key((char*)&(rsa_pub_blob->rsa_pub_key.bitlen));
	
	rsa->exponent = RSA_bignum_from_long(rsa_read_long_from_public_key((char*)(&(rsa_pub_blob->rsa_pub_key.pubexp))));

	p=data + sizeof(MY_RSA_PUBLICK_BLOB);
	
	memset(temp_buf,0,sizeof(temp_buf));
	for(i=0; (i<(int)(bit_len/8))&&(i<sizeof(temp_buf)); i++)
	{
		temp_buf[i] = p[(bit_len/8)-1 -i];
	}

    rsa->modulus = RSA_bignum_from_bytes((unsigned char *)temp_buf, bit_len/8);

    rsa->private_exponent = NULL;

    rsa->comment = NULL;
	
    return rsa;
error_exit:
	sfree(rsa);
	return NULL;
}

static const unsigned char asn1_weird_stuff[] = {
    0x00, 0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2B,
		0x0E, 0x03, 0x02, 0x1A, 0x05, 0x00, 0x04, 0x14,
};

#define ASN1_LEN ( (int) sizeof(asn1_weird_stuff) )

static int rsa2_verifysig(void *key, char *sig, int siglen,
			   char *data, int datalen)
{
    struct RSAKey *rsa = (struct RSAKey *) key;
    Bignum in, out;
    int slen;
    int bytes, i, j, ret;
    unsigned char hash[20];
   
	in = RSA_bignum_from_bytes((unsigned char *)sig, siglen);

    out = RSA_modpow(in, rsa->exponent, rsa->modulus);

    RSA_freebn(in);

    ret = 1;

	slen = (int)(out[0]);

#ifdef TEST_RSA
	printf("slen %d\n",slen);
	for(i = 1; i <=slen; i++)
	{
		printf("%02x",out[i]);
	}
	printf("\n");
#endif 
    bytes = (RSA_bignum_bitcount(rsa->modulus)+7) / 8;
    /* Top (partial) byte should be zero. */
    if (RSA_bignum_byte(out, bytes - 1) != 0)
	ret = 0;
    /* First whole byte should be 1. */
    if (RSA_bignum_byte(out, bytes - 2) != 1)
	ret = 0;
    /* Most of the rest should be FF. */
    for (i = bytes - 3; i >= 20 + ASN1_LEN; i--) {
	if (RSA_bignum_byte(out, i) != 0xFF)
	    ret = 0;
    }
    /* Then we expect to see the asn1_weird_stuff. */
    for (i = 20 + ASN1_LEN - 1, j = 0; i >= 20; i--, j++) {
	if (RSA_bignum_byte(out, i) != asn1_weird_stuff[j])
	    ret = 0;
    }
    /* Finally, we expect to see the SHA-1 hash of the signed data. */
    //����,����Ҫ��һ��,����Ҫ��MD5�㷨��һ��.
	//Ҳ������������HASH
    RSA_SHA_Simple(data, datalen, hash);
#ifdef TEST_RSA
	printf("\n");
	for(i = 0; i < 20; i++)
	{
		printf("%2x ",hash[i]);
	}
	printf("\n");
#endif
    for (i = 19, j = 0; i >= 0; i--, j++) {
	if (RSA_bignum_byte(out, i) != hash[j])
	    ret = 0;
    }
    RSA_freebn(out);

    return ret;
}
//base 64 ����
#define CHAR64(c) (((c) < 0 || (c) > 127) ? -1 : index_64[(c)])
static int app_base64decode(const unsigned char *in, unsigned inlen, unsigned char *out)
{
    unsigned  len = 0, lup;
    int    c1,c2,c3,c4;
	static char     index_64[128] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
			-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
			-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
			41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
	};
	
	/* xxx these necessary? */
    if (in[0] == '+' && in[1] == ' ')
        in += 2;
	
    if (*in == '\0')
        return 0;
	
    for (lup = 0; lup < inlen / 4; lup++)
    {
        c1 = in[0];
        if (CHAR64(c1) == -1)
            return 0;
        c2 = in[1];
        if (CHAR64(c2) == -1)
            return 0;
        c3 = in[2];
        if (c3 != '=' && CHAR64(c3) == -1)
            return 0;
        c4 = in[3];
        if (c4 != '=' && CHAR64(c4) == -1)
            return 0;
        in += 4;
        *out++ = (CHAR64(c1) << 2) | (CHAR64(c2) >> 4);
        ++len;
        if (c3 != '=')
        {
            *out++ = ((CHAR64(c2) << 4) & 0xf0) | (CHAR64(c3) >> 2);
            ++len;
            if (c4 != '=')
            {
                *out++ = ((CHAR64(c3) << 6) & 0xc0) | CHAR64(c4);
                ++len;
            }
        }
    }
	
    *out = 0; 
	return len;
}


static int rsaVerifySign(
	unsigned char *public_key_blob,
	unsigned long        public_key_blob_len,
    unsigned char *in_data,      
	unsigned long        in_data_len,		   
    unsigned char *in_Signature,   
	unsigned long        in_Signature_len)
{
	char signBuf[256];
    struct RSAKey *rsa=NULL;
	int re=0;
	unsigned long i=0;
	if(sizeof(signBuf)<in_Signature_len)
	{
		re = 0;
		goto err;
	}
	if(!public_key_blob || !in_data || !in_Signature)
	{
		re = 0;
		goto err;
	}

    rsa = rsa2_newkey_ex((char *)public_key_blob, public_key_blob_len);
	if(rsa==NULL)
	{   
		re = 0;
		goto err;
	}
	memset(signBuf, 0 ,sizeof(signBuf));
	for(i=0;i<in_Signature_len; i++)
	{
		signBuf[i] = in_Signature[in_Signature_len-1 - i];
	}


    re = rsa2_verifysig(rsa,
						signBuf,
				   		in_Signature_len,
				  		(char *)in_data,
		          		in_data_len);
err:
	if(rsa)
		sfree(rsa);
	return re;   
}



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
 												 unsigned long inSignatureLen)
{
	unsigned char buf[256];                
	int len;
	
	if(PublicKeyBlobLen > sizeof(buf))
	{
		goto error_exit;
	}
	
	len = app_base64decode(pPublicKeyBlob, PublicKeyBlobLen, buf);
	
	if(len <= 0)
	{
		goto error_exit;
	}
	
	return  rsaVerifySign(buf,
						  len,
						  pInData,      
						  inDataLen,		   
						  PInSignature,   
						  inSignatureLen);

error_exit:
	return 0;	
}

#if 0
int  rsaVerifySignByBase64EncodePublicKeyBlob(
	unsigned char *public_key_blob,
	unsigned long  public_key_blob_len,
	unsigned char *in_data,      
	unsigned long  in_data_len,		   
	unsigned char *in_Signature,   
	unsigned long  in_Signature_len)
{
	unsigned char buf[256];                
	int len;
	if(public_key_blob_len> sizeof(buf))
	{
		goto error_exit;
	}
	len = app_base64decode(public_key_blob,public_key_blob_len,buf);
	if(len<=0)
	{
		goto error_exit;
	}
	return  rsaVerifySign(buf,
						  len,
						  in_data,      
						  in_data_len,		   
						  in_Signature,   
						  in_Signature_len);

error_exit:
	return 0;
}
#endif

#ifdef TEST_RSA


char    publickeyBuf2[] ="BgIAAACkAABSU0ExAAQAAAEAAQBvDl7svGyTKAaVCkaaGzMOkK1rdQiS1v3SyrVRVE/ee8N/9POQRxYIzr2+hARJUo31SuNjFleoge2NaIiuYEtIYjSmxR7NbLkx+nIztZqmhZy9qNbjsPNnmEMBtsMm1iFIqFQgFg61i+sAV3FW7hvXbZCoCWI3J7/JsslPEsO07A==";
int     publicKeyLen2 = sizeof(publickeyBuf2)-1;

char	dataBuf[] = "12345678";
int  	dataLen = sizeof(dataBuf) - 1;

char	rsaSignBuf[128] = {
	    0xa3, 0xd9, 0x32, 0xca, 0xd3, 0x71, 0x47, 0x25,  0xdd, 0x2c, 0xa9, 0xab, 0x08, 0xce, 0x26, 0x81,
		0x86, 0xc9, 0x3d, 0xe8, 0x5f, 0x35, 0x9d, 0xa5,  0xc1, 0x2c, 0x83, 0x42, 0x51, 0x91, 0xb9, 0x35,
		0x44, 0x3c, 0x48, 0x3f, 0x94, 0xda, 0x5f, 0x3d,  0x6c, 0xb5, 0xd7, 0x3b, 0x32, 0x6d, 0x77, 0x12,
		0x45, 0x10, 0xe0, 0xba, 0xe1, 0xcb, 0x1a, 0x16,  0xec, 0x49, 0xb0, 0x1c, 0x21, 0xbc, 0x64, 0x34,
		0x0d, 0x60, 0x98, 0xc0, 0xe1, 0xd4, 0x3d, 0xd9,  0xb7, 0x90, 0xff, 0x94, 0xd5, 0x14, 0xa2, 0x70,
		0x14, 0xae, 0xad, 0x49, 0x71, 0x1d, 0x66, 0x07,  0xff, 0x86, 0x08, 0xf5, 0xe0, 0x28, 0xe3, 0x45,
		0x0b, 0x9e, 0x2d, 0xa0, 0x6a, 0x2a, 0xc7, 0xb7,  0xf4, 0xb8, 0xeb, 0x7d, 0x75, 0x79, 0x68, 0xa5,
		0xee, 0x22, 0xe2, 0x78, 0xc3, 0x5c, 0x76, 0x4c,  0xb8, 0x5f, 0x8f, 0xf4, 0xcf, 0x9b, 0x63, 0x94
};

int 	rsaSignLen = 128;

int testRsaSign()
{

	if(!rsaVerifySignByBase64EncodePublicKeyBlob(
		publickeyBuf2,
		publicKeyLen2,
		dataBuf,
		dataLen,
		rsaSignBuf,
		rsaSignLen))
	{
		printf("rsa VerifySign error \n");
	}else
	{
		printf("rsa VerifySign OK \n");
	}
	printf("end rsa verify sign\n");
	

}



void main(void)
{		
	testRsaSign();	

    return ;
}

#endif

#endif /* end #if 1 */

///////////////////////////////////////////////////////////////////////////////////////////////
///lvwg  2009-05-27

//16�����ַ�ת��Ϊ��ֵ
int xtoi(char c)
{
	int num;

	if (c <= 'F' && c >= 'A')
	{
		num = 10 + (c - 'A');
	}
	else if (c <= 'f' && c >= 'a')
	{
		num = 10 + (c - 'a');
	}
	else if (c <= '9' && c >= '0')
	{
		num = c - '0';
	}else 
	{
		return -1;
	}

	return num;
}

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
int cen_rsaVerifySignature(unsigned char *in_pp,			//p
	unsigned long  in_pp_len,
	unsigned char *in_nn,			//n
	unsigned long  in_nn_len,
	unsigned char *in_data,			//data
	unsigned long  in_data_len,		   
	unsigned char *in_Signature,	//sign   
	unsigned long  in_Signature_len)
{
	if (in_pp_len % 2 != 0 || in_nn_len % 2 != 0)
	{
		return 0;
	}
{
	/*ת��16���Ƶ���Կ�ַ���Ϊ��ֵ���͵��ַ�������"C773"->{0xC7, 0x73}*/
	int pp_len = in_pp_len / 2;
	unsigned char* pp = (unsigned char*)malloc(pp_len);
	int nn_len = in_nn_len / 2;
	unsigned char* nn = (unsigned char*)malloc(nn_len);

	int	i, h1, h2;
	for (i = 0; i < pp_len; i++)
	{
		h1 = xtoi(in_pp[2 * i]);
		h2 = xtoi(in_pp[2 * i + 1]);
		if (h1 == -1 || h2 == -1)
		{
			free(pp);
			free(nn);
			return 0;
		}
		pp[i] = h1 * 16 + h2;
	}
	for (i = 0; i < nn_len; i++)
	{
		h1 = xtoi(in_nn[2 * i]);
		h2 = xtoi(in_nn[2 * i + 1]);
		if (h1 == -1 || h2 == -1)
		{
			free(pp);
			free(nn);
			return 0;
		}
		nn[i] = h1 * 16 + h2;
	}
{
	//�Ѳ������������ת��Ϊ����
	Bignum in = RSA_bignum_from_bytes(in_Signature, in_Signature_len);

	Bignum	p = RSA_bignum_from_bytes(pp, pp_len);
	Bignum	n = RSA_bignum_from_bytes(nn, nn_len);

	//����ǩ��
	Bignum out = RSA_modpow(in, p, n);

	//����SHA-1��hash
	unsigned char hash[20];
	RSA_SHA_Simple(in_data, in_data_len, hash);

	//��֤
	for (i = 0; i < 20; i++)
	{
		if(RSA_bignum_byte(out, i) != hash[20 - i - 1])
		{
			free(pp);
			free(nn);
			RSA_freebn(in);
			RSA_freebn(p);
			RSA_freebn(n);
			RSA_freebn(out);

			return 0;
		}
	}

	//�ͷ���Դ
	free(pp);
	free(nn);
	RSA_freebn(in);
	RSA_freebn(p);
	RSA_freebn(n);
	RSA_freebn(out);
}
}
	return 1;
}

