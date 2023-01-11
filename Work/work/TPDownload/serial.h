//serial
#ifndef _SERIAL_
#define _SERIAL_

//start of file

#define PROT_DT_STX   0x02
#define PROT_DT_ETX   0x03
#define PROT_DT_DLE   0x10

int serial(char * port, unsigned char *inbuf, int inlen, unsigned char *outbuf, int outlen, short timeMs);
int InsertDLE(unsigned char *pbytData, int intLength);
int DeleteDLE(unsigned char *pbytData, int intLength);
unsigned char writecom(int fd, unsigned char *pdata, long lnglen);
unsigned char readcom(int fd, unsigned char *pdata, long lnglen);
void speed_set(int fd, int speed);
int parity_set(int fd, int databits, int stopbits, int parity);

//end of file
#endif
