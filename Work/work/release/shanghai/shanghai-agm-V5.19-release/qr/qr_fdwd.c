#include <sys/time.h>
#include <fcntl.h>

int intQRFile = 0;
unsigned char blnEnableQR;

typedef enum
{
    false,
    true
}bool;
bool listened = false;

long EnableQR()  //使能QR条码0x21, 0x01, 0x00, 0x01
{
unsigned char	 bytBCD[7];
char bytEnableQR[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;

	if(intQRFile == 0)
		return 0;
	return (blnEnableQR = FD_EnableQR());
}

long DisableAll()    //禁用所有的条码
{
unsigned char	 bytBCD[7];
char bytDisableAll[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;

	if(intQRFile == 0)
		return 0;
	return FD_DisableQR();
}

long SetMode()	//间隔（1s）模式
{
unsigned char	 bytBCD[7];
char bytSetMode[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;



	return 0;
}

long SetInterval()   //设置扫码成功后间隔
{
unsigned char	 bytBCD[7];
char bytSetInterval[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;

		
	return 0;
}

long LightControl(char LEDSwitch)	//LED灯控制
{
unsigned char	 bytBCD[7];
char bytLightControl[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;

		
	return 0;
}

long BeepControl(char bytBeepFlag)	//蜂鸣器控制
{
unsigned char	 bytBCD[7];
char bytBeepControl[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;

	

	return 0;
}



//接口函数，包括各种设置请求
int openQR(unsigned char *com)
{
	intQRFile = 0xff;
	
	return 0xff;
}

void closeQR()
{
	return ;
}

void InitScan()  //初始化扫描头
{
	//
	return ;
}

void StartScan() //开始扫描
{

    EnableQR();
    //printf("start scaning...\n");
    listened = true;
}

void EndScan()   //结束扫描
{
	DisableAll();
    listened = false;
}

//void scancontrol((*CallBackFun)(char *p) *backdata ,long *backdatalength)//与读写器回掉函数的接口
int scancontrol(unsigned char *backdata ,long *backdatalength)//与读写器回掉函数的接口
{
unsigned char fStat, blnTimeout;
unsigned char buff[500], byttemp[500];
char chinfo[1200], chtempinfo[10];
int lreadlen, rtn, ret;
fd_set	readfd, intervalfd;
struct timeval readTimeout, intervalTimeout;
unsigned short 	len;
int i;
	//printf("\nscancontrol...1\n");
	if(intQRFile == 0)
		return 0xff;
		
	if(blnEnableQR)
		EnableQR();

	rtn = GetPollData(backdata, &len);
	
	//if(len < 84)
	//	return 0xff;
#ifdef	DEBUG_FDWD
	printf("\nQR Len:%d\n",len);
	printf("QR data :");
	for(i = 0; i < len; i++)
	{
		printf("%02X ", backdata[i]);
	}
	printf("\n");
#endif
	*backdatalength = len;
	return rtn;
}
//结束