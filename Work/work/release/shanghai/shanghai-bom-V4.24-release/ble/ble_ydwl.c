#ifndef _BLE_YDWL_C_
#define _BLE_YDWL_C_

#include <sys/time.h>
#include <fcntl.h>

int intBLEFile;
unsigned short bleSN;

//#define DEBUG_BLE

int open_BLE(unsigned char *com)
{

	intBLEFile = open_port(com);
	if(intBLEFile < 0)
	{
		printf("open ble comm failure \n");
		return -1;
	}
	printf("open ble sucess\n");
	speed_set(intBLEFile, 115200);
	parity_set(intBLEFile, 8, 1, 'n');
	
	bleSN = 1;
	
	return intBLEFile;
}

void close_BLE()
{
	close_port(intBLEFile);
}

/*
��������ʶλ��У����֮�������
*/
unsigned char sumcheck(unsigned char *inbuf, unsigned short len)
{
unsigned char sc = 0;
unsigned char buf[500];
int i, sum = 0;

	memcpy(buf, inbuf, len);
	
	for(i = 0; i < len; i++)
		sum += buf[i];
		
	sc = (unsigned char)sum;
//#ifdef DEBUG_BLE
//	printf("sum check len %d Data : ", len);
//	for(i = 0; i < len; i++)
//		printf("%02x ", inbuf[i]);
//	printf("\n sum is %x return is %x\n", sum ,sc);
//#endif
	
	return sc;
}

/*
��ʶλSb	У����Cm	�汾��Vn	���к�Sn	�豸����Dt	��������Ct	�û�����Ud	��ʶλSb
1 byte		1 byte		1 byte		2 bytes	 	1 byte		1 byte		N byte		1 byte
						from here
*/
long ydwl_Insert7E(unsigned char *pbytData, int intLength)
{
unsigned char	tempData[600], bytCheck;
int	i, intInsertLen = 1;
	
	memcpy(&tempData[2], pbytData, intLength);
	pbytData[0] = tempData[0] = 0x7e;
	pbytData[1] = tempData[1] = sumcheck(&tempData[2], intLength);

	//
	for(i = 0; i <= intLength; i++)
	{
		bytCheck = tempData[i + 1];
		if((bytCheck == 0x7e) ) 
		{
			pbytData[intInsertLen++] = 0x7d;
			pbytData[intInsertLen++] = 0x02;
			continue;
		}
		
		if(bytCheck == 0x7d)
		{
			pbytData[intInsertLen++] = 0x7d;
			pbytData[intInsertLen++] = 0x01;
			continue;
		}
		pbytData[intInsertLen++] = bytCheck;
	}
	pbytData[intInsertLen++] = 0x7E;
	
	return intInsertLen;
}

/*
delete the DLE 
the data including the all data block from csc reader
stx+len+cmd+lrc+etx
*/
long ydwl_Delete7E(unsigned char *pbytData, int intLength)
{
unsigned char 	bytTempData[600], bytCheck;
int 		intDeleteLen, i;

#ifdef DEBUG_BLE
	printf("resv len %d Data : ", intLength);
	for(i = 0; i < intLength; i++)
		printf("%02x ", pbytData[i]);
	printf("\n");
#endif
	
	if(pbytData[0] != 0x7E) return 0;
	
	memcpy(bytTempData, pbytData, intLength);
	intDeleteLen = intLength;
	for(i = 1; i < intLength; i++)
	{
		if(bytTempData[i] == 0x7D)
		{
			memcpy(&bytTempData[i], &bytTempData[i + 1], intLength - 1 - i);
			if(bytTempData[i] == 0x02)
				bytTempData[i] = 0x7E;
			else
				bytTempData[i] = 0x7D;
			intDeleteLen--;
		}
	}
	
	bytCheck = sumcheck(&bytTempData[2], intDeleteLen - 3); 

	//printf("ble resv data sumchekc is %02x\n", bytCheck);
	//if(bytCheck == bytTempData[1])
	{
		memcpy(pbytData, bytTempData, intDeleteLen);
		return intDeleteLen;
	}
	//else
	{
		return 0;
	}
}

int ydwl_Comm(unsigned char *inbuf, unsigned char inlen, unsigned char *outbuf, unsigned short *outlen, unsigned short timeMs)
{
	unsigned char buf[300], resbuf[300], buff[10];
	int len, lreadlen;
	fd_set readfd, intervalfd;
	struct timeval timeout, intervalTimeout;
	unsigned char blnTimeout, fStat;
	long rtn;
	int i, ret;

	if( intBLEFile <= 0)
		return 0xff;
		
	if(inlen != 0)
	{
		memcpy(buf, inbuf, inlen);
		len = ydwl_Insert7E(buf, inlen);
	
#ifdef DEBUG_BLE
		printf("\nble Send len %d: Data:", len);
		for(i= 0; i <len; i++) printf("%02x ", buf[i]);
		printf("\n");
#endif			
		if(!writecom(intBLEFile, buf, len))
		{
			printf("write ble comm failure\n"); 
			return 0xff;
		}
	}

	do
	{
		//watchdog();
		FD_ZERO(&readfd);
		FD_SET(intBLEFile, &readfd);
		timeout.tv_sec = 0;
		timeout.tv_usec = timeMs * 1000;
		lreadlen = 0;
		ret = select(intBLEFile + 1, &readfd, NULL, NULL, &timeout);
		if(ret <= 0)
		{
			printf("ble select return %d command %02x\n", ret, buf[6]);
			return 0xff;			
		}
		
		
		blnTimeout = 0x00;
#ifdef DEBUG_BLE
		printf("bleComm data comming....\n");
#endif
		do
		{		
			fStat = readcom(intBLEFile, buff, 1);
			if(fStat)
			{
				resbuf[lreadlen] = buff[0];
				lreadlen++;
				if(lreadlen > 6)
				{
					if(resbuf[lreadlen - 1] == 0x7E)
					{
						rtn = ydwl_Delete7E(resbuf, lreadlen);
						if(rtn > 0)
						{
							memcpy(outbuf, resbuf, rtn);
							*outlen = rtn;
							return 0;
						}else
							return 0xff;
					}
				} 
			}
			
			//
			FD_ZERO(&intervalfd);
			FD_SET(intBLEFile, &intervalfd);
			intervalTimeout.tv_sec = 0;
			intervalTimeout.tv_usec = 5000;
			ret = select(intBLEFile + 1, &intervalfd, NULL, NULL, &intervalTimeout);
			if(ret > 0) continue;

			return -3;
			
		}while(1);

	}while(1);
}


int bleOffline()
{
unsigned char buf[100];
unsigned char obuf[500];
unsigned short oLen;
int rtn;

	memset(buf, 0x00, sizeof(buf));
	
	memcpy(buf, "\x01\x01\x00\x0c\x14", 5);
	memcpy(&buf[1], &bleSN, 2);
	//�Ͽ�����
	rtn = ydwl_Comm(buf, 5, obuf, &oLen, 990);
	if(rtn)
		return rtn;
	bleSN++;
	if((obuf[6] != 0x15) && (obuf[7] != 0x00) )
		return 0xff;
		
	return 0;
}


int bleSend(unsigned char *inbuf, unsigned char inlen, unsigned char *backdata ,int *backdatalength)
{
unsigned char buf[100];
unsigned char obuf[500];
unsigned short oLen, dataLen;
int rtn;

	memset(buf, 0x00, sizeof(buf));
	
		
	//��������
	memcpy(buf, "\x01\x01\x00\x0c\x12", 5);
	memcpy(&buf[1], &bleSN, 2);
	memcpy(&buf[5], &inbuf[0], inlen);
	rtn = ydwl_Comm(buf, 5 + inlen, obuf, &oLen, 990);
	if(rtn)
		return rtn;
	bleSN++;
	if((obuf[6] != 0x40) )
		return 0xff;
	dataLen = (obuf[7] << 8) + obuf[8];
	//if(dataLen + 10 != oLen)
	//	return 0xff;
	//Ӧ�����
	memcpy(backdata, &obuf[7], dataLen + 2);
	
	*backdatalength = dataLen+2;
	
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
	
	memcpy(buf, "\x01\x01\x00\x0c\x10", 5);
	memcpy(&buf[1], &bleSN, 2);
	memcpy(&buf[5], &inbuf[1], 6);
	
	//��������
	rtn = ydwl_Comm(buf, 11, obuf, &oLen, 900);
	if(rtn)
		return rtn;
	bleSN++;
	if((obuf[6] != 0x11) && (obuf[7] != 0x00) )
		return 0xff;
		
	//��ȫ��֤
	memcpy(buf, "\x01\x01\x00\x0c\x12", 5);
	//����͸�� L + V
	buf[5] = 0x00;
	buf[6] = 0x05;
	buf[7] = 0x33;
	memcpy(&buf[8], &inbuf[80], 4);
	rtn = ydwl_Comm(buf, 5 + 7, obuf, &oLen, 900);
	if(rtn)
	{
		return rtn;
	}
	printf("ble auth return %02x %02x \n", obuf[6], obuf[7]);
	if(obuf[6] != 0x40)
		return 0xff;
	
	printf("%02x%02x%02x%02x %02x%02x%02x%02x\n", obuf[10], obuf[11], obuf[12], obuf[13], obuf[14], obuf[15], obuf[16], obuf[17]);
	memcpy(outbuf, &obuf[10], 8);
	dataLen = (obuf[7] << 8) + obuf[8];
	//if(dataLen + 10 != oLen)
	//	return 0xff;
	
	
	return 0x0;
}


#endif