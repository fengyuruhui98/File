#ifndef _BLE_C_
#define _BLE_C_

#include <sys/time.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>

pthread_t	g_pthbleID;
sem_t g_bleWait, g_bleReturn;
int bleCommOnlineRet, bleCommSendRet, bleStep;
unsigned char doubleAuth[20];
unsigned char *pSend, sendLen, retBuf[300];
unsigned char blnBLEOpen = 0xff;
unsigned char OpenBle[500];

int retLen;

//#define DEBUG_BLE

//接口函数，包括各种设置请求
int openBLE(unsigned char *com)
{
int rtn;

	rtn = open_BLE(com);

	sem_init(&g_bleWait, 0, 0);
	sem_init(&g_bleReturn, 0, 0);
	pthread_create(&g_pthbleID, NULL, &pthBLE, NULL);
	
	blnBLEOpen = 0x00;
	return rtn;
}

void closeBLE()
{
	close_BLE();
}


void startBLEOnline(unsigned char *inbuf)
{
	memcpy(OpenBle, inbuf, 100);

	bleStep = 0;
	sem_init(&g_bleReturn, 0, 0);
	
	sem_post(&g_bleWait);
}

int getBlEOnlineReturn(unsigned char *outbuf)
{
	if (blnBLEOpen)
		return 0x404C;
	sem_wait(&g_bleReturn);
	
	memcpy(outbuf, doubleAuth, 8);
	return bleCommOnlineRet;
}

void sendBLEData(unsigned char *inbuf, unsigned char inlen)
{
	bleStep = 1;
	pSend = inbuf;
	sendLen = inlen;
	
	sem_post(&g_bleWait);
}

int getBLESendReturn(unsigned char *outbuf, int *outlen)
{
	if (blnBLEOpen)
		return 0x404C;
	sem_wait(&g_bleReturn);
	
	memcpy(outbuf, retBuf, retLen);
	*outlen = retLen;
	
	return bleCommSendRet;
}

void stopBLEOffline()
{
	bleStep = 2;
	sem_post(&g_bleWait);
}

/*
function:
*/
void *pthBLE()
{
char	led_ctrl = 0;

#ifdef DEBUG_BLE
	printf("ble thread start\n");
#endif
	for(;;)
	{
		sem_wait(&g_bleWait);
		switch(bleStep)
		{
		case 0://链接BLE
			bleCommOnlineRet = bleOnline(OpenBle, 84, doubleAuth);
			sem_post(&g_bleReturn);
			break;
		case 1://发送数据
			bleCommSendRet = bleSend(pSend, sendLen, retBuf, &retLen);
			sem_post(&g_bleReturn);
			break;
		default:
			bleOffline();
		}
	}
}
#endif