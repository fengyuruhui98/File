#ifndef	SM_H
#define	SM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <limits.h>

#define	DEBUG_QR
//#define	DEBUG_PRINT	

const unsigned char *iUnionPay_id = "1234567812345678";
unsigned char *iUnionPay_id_len = "\x00\x80";


void extendsPrintf(unsigned char *msg, unsigned char *inbuf, int in_len);
int SM2_Operation(const unsigned char *id, unsigned char *id_len, unsigned char y_bit, unsigned char *Xa, unsigned char *Ya, 
				  unsigned char *message, unsigned int msgLen, unsigned char *r, unsigned char *s);

#ifdef DEBUG_QR		//为了兼容老读写器，老读写器暂不支持sm2算法

#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/ecdsa.h>
#include <openssl/ecdh.h>
#include <openssl/evp.h>

int SM2_verify(int type, const unsigned char *dgst, int dgst_len,
		const unsigned char *sigbuf, int sig_len, EC_KEY *eckey);

int SM2_Cal_Ya(int y_bit, unsigned char *Xa, unsigned char *Ya);

#endif

void sm3( unsigned char *input, int ilen,
           unsigned char output[32]);


#endif