#ifndef SHA_H
#define SHA_H

//#include "global_rsa.h"
#include "global.h"
//#include "linux2440lib.h"
#define SHA_VERSION 1

//in PC or C51 it is 1234
#define SHA_BYTE_ORDER 1234
//in MC68Kvz328 it is 4321
//#define SHA_BYTE_ORDER 4321

/* NIST Secure Hash Algorithm */
/* heavily modified by Uwe Hollerbach <uh@alumni.caltech edu> */
/* from Peter C. Gutmann's implementation as found in */
/* Applied Cryptography by Bruce Schneier */

/* This code is in the public domain */

/* Useful defines & typedefs */
//typedef unsigned char BYTE;	/* 8-bit quantity */
//typedef unsigned char UBYTE;	/* 8-bit quantity */
typedef unsigned long UNLONG;	/* 32-or-more-bit quantity */

#define SHA_BLOCKSIZE		64
#define SHA_DIGESTSIZE		20


typedef struct {
    UNLONG digest[5];		/* message digest */
    UNLONG count_lo, count_hi;	/* 64-bit bit count */
    BYTE data[SHA_BLOCKSIZE];	/* SHA data buffer */
    int local;			/* unprocessed amount in data */
} SHA_INFO;

void sha_init(SHA_INFO *);
void sha_update(SHA_INFO *, BYTE *, int);
void sha_final(unsigned char [20], SHA_INFO *);

void sha(const BYTE* DataIn,unsigned int DataInLen,BYTE* DataOut);

#endif
