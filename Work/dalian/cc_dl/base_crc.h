#ifndef _BASE_CRC_H
#define _BASE_CRC_H
 
#include <string.h>
 
#define CRC_ITU_MAGIC 	0xf0b8

class  Crc
{
public:
    static unsigned char  crc8(unsigned char *ptr, unsigned char len);
    static unsigned short GetCrc16(unsigned char * pData, int nLength);
    static unsigned char  IsCrc16Good(unsigned char * pData, int nLength);
    static unsigned char  IsCrc16Good_With_CRC(unsigned char * pData, int nLength, unsigned char CRCH, unsigned char CRCL);

    static void 					Init_CRC32_Table();
    static unsigned int 	Reflect(unsigned int ref, char ch);
    static int 						Get_CRC32(char *chData, unsigned int lngLen);
    static unsigned int 	Crc32(void *pStart, unsigned int length, unsigned int crc32);

    static unsigned short cal_crc(unsigned char *ptr, int len);
};

#endif
