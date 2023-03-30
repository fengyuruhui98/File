#ifndef _KM_BIN_API_H_
#define _KM_BIN_API_H_
//start of file

typedef unsigned long xdr_ver32;

int code_convert(char *from_charset, char * to_charset, char * inbuf, int inlen, char *outbuf, int outlen);
unsigned char *ShortToByte(short sh_in, unsigned char *out_buf);
unsigned char *LongToByte(long lng_in, unsigned char *out_buf);
short ByteToShort(short *sh_in, unsigned char *out_buf);
long ByteToLong(long *lng_in, unsigned char *out_buf);

long sz_round(long x);

unsigned char *HexToBcd(unsigned char *in_buf, unsigned char *out_bcd);

int	nib2str(unsigned char *hexdata, unsigned char *strdata, int length);
int	str2nib(unsigned char *strdata, unsigned char *hexdata);

//end of file
#endif
