#ifndef _BLE_FDWD_C_
#define _BLE_FDWD_C_

#include <sys/time.h>
#include <fcntl.h>

int intBLEFile;
unsigned short bleSN;

//#define DEBUG_BLE

int open_BLE(unsigned char *com)
{

	return 0xff;
}

void close_BLE()
{
	return ;
}




int bleOffline()
{
unsigned char buf[100];
unsigned char obuf[500];
unsigned short oLen;
int rtn;

	rtn = BLEClose();
	return 0;
}


int bleSend(unsigned char *inbuf, unsigned char inlen, unsigned char *backdata ,int *backdatalength)
{
unsigned char buf[100];
unsigned char obuf[500];
unsigned short oLen, dataLen;
int rtn;

		
	//��������
	*backdatalength = 0;
	rtn = BLEWrite(inbuf, inlen, obuf, &oLen);
	if(rtn)
		return rtn;
	//Ӧ�����
	memcpy(backdata, &obuf[0], oLen);
	
	*backdatalength = oLen;
	
	return 0;
}


/*
��ά������
*/
int bleOnline(unsigned char *inbuf, unsigned char inlen, unsigned char *outbuf)
{
unsigned char buf[100];
unsigned char obuf[500];
unsigned short oLen, dataLen;
int rtn;

	memset(buf, 0x00, sizeof(buf));
	
//	printf("inbuf[84] %02x\n", inbuf[84]);
	rtn = BLEOpen(inbuf, inlen, obuf, &oLen);
	
	if(rtn)
		return rtn;
		
#ifdef DEBUG_BLE
	printf("%02x%02x%02x%02x %02x%02x%02x%02x\n", obuf[3], obuf[4], obuf[5], obuf[6], obuf[7], obuf[8], obuf[9], obuf[10]);
#endif
	memcpy(outbuf, &obuf[3], 8);
	
	return 0x0;
}

//int bleStatus(unsigned char *outbuf)
//{
//int rtn;
//unsigned char buf[100];
//unsigned char obuf[500];
//unsigned short oLen, dataLen;
//
//	rtn = fdwd_Status(obuf, &oLen);
//	
//	outbuf[0] = obuf[0];
//
//	return rtn;
//}

#endif