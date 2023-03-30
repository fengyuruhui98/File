#ifndef	CSC_H
#define CSC_H

#define CRC_ITU_MAGIC 	0xf0b8

unsigned short GetCrc16(unsigned char * pData, int nLength);
unsigned char  IsCrc16Good(unsigned char * pData, int nLength);
unsigned char IsCrc16Good_With_CRC(unsigned char * pData, int nLength, unsigned char CRCH, unsigned char CRCL);

void Init_CRC32_Table();
unsigned long Reflect(unsigned long ref, char ch);
int Get_CRC32(char *chData, unsigned long lngLen);

unsigned short cal_crc(unsigned char *ptr, int len);

unsigned char CRC8_Table(unsigned char *p, int counter);
unsigned char IsCRC8Good_With_CRC(unsigned char *p, int len, unsigned char crc8);
char Wd_Crc8(char *parastr, int len);

#endif
