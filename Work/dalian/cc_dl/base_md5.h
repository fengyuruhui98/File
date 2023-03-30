//
// Created by HHJT_CJP on 19-12-9.
//

#ifndef _BASE_MD5_H
#define _BASE_MD5_H

#include "sys_head.h"

typedef unsigned int md5_uint32;

/****************************************************************/
#define ENC_OK		    0				/*正确    */
#define ENC_PARAM	    1				/*参数错误 */
#define ENC_RESULT		2				/*结果错误 */

struct md5_ctx
{
    md5_uint32 A;
    md5_uint32 B;
    md5_uint32 C;
    md5_uint32 D;

    md5_uint32 total[2];
    md5_uint32 buflen;
    char buffer[128];// __attribute__ ((__aligned__ (__alignof__ (md5_uint32))));
    //char buffer[3072];
};

#ifdef WORDS_BIGENDIAN
# define SWAP(n)							\
    (((n) << 24) | (((n) & 0xff00) << 8) | (((n) >> 8) & 0xff00) | ((n) >> 24))
#else
# define SWAP(n) (n)
#endif



/* This array contains the bytes used to pad the buffer to the next
   64-byte boundary.  (RFC 1321, 3.1: Step 1)  */
static const unsigned char fillbuf[64] = { 0x80, 0 /* , 0, 0, ...  */ };

#define FF(b, c, d) (d ^ (b & (c ^ d)))
#define FG(b, c, d) FF (d, b, c)
#define FH(b, c, d) (b ^ c ^ d)
#define FI(b, c, d) (c ^ (b | ~d))


class  CMd5
{
private:
    static void md5_init_ctx (struct md5_ctx *ctx);

    static void md5_process_block (const void *buffer, size_t len,struct md5_ctx *ctx);

    static void md5_process_bytes (const void *buffer, size_t len,struct md5_ctx *ctx);

    static void *md5_finish_ctx(struct md5_ctx *ctx, void *resbuf);

    static void *md5_read_ctx(const struct md5_ctx *ctx, void *resbuf);

public:
    static int CreateMAC(unsigned char *UserInputPIN,unsigned char *EncryptedChar,unsigned int lngLen);
    
    static int MD5_MAC(unsigned char *UserInputPIN,unsigned char *EncryptedChar,unsigned int lngLen);
    

};


#endif //_BASE_MD5_H
