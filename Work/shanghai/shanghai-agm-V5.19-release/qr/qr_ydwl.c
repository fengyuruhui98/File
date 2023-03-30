#include <sys/time.h>
#include <fcntl.h>

int intQRFile;

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


	if(intQRFile <= 0)
		return -1;

	return 0;
}

long DisableAll()    //禁用所有的条码
{
unsigned char	 bytBCD[7];
char bytDisableAll[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;


	if(intQRFile <= 0)
		return -1;
		
	return 0;
}

long SetMode()	//间隔（1s）模式
{
unsigned char	 bytBCD[7];
char bytSetMode[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;


	if(intQRFile <= 0)
		return -1;
		

	return 0;
}

long SetInterval()   //设置扫码成功后间隔
{
unsigned char	 bytBCD[7];
char bytSetInterval[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;

	if(intQRFile <= 0)
		return -1;
		
	return 0;
}

long LightControl(char LEDSwitch)	//LED灯控制
{
unsigned char	 bytBCD[7];
char bytLightControl[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;


	if(intQRFile <= 0)
		return -1;
		
	return 0;
}

long BeepControl(char bytBeepFlag)	//蜂鸣器控制
{
unsigned char	 bytBCD[7];
char bytBeepControl[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;

	if(intQRFile <= 0)
		return -1;
		

	return 0;
}

//void GetLocalTime(struct SYSTEMTIME *lpsystemtime)	//获取当前时间
//{
//time_t lnglocaltime;
//struct tm *tplocaltime;
//struct timeval tv1,tv2;
//struct timezone tz1,tz2;
//
//	gettimeofday(&tv1, &tz1);
//	time(&lnglocaltime);
//	lnglocaltime= tv1.tv_sec;
//	tplocaltime = localtime(&lnglocaltime);
//
//	lpsystemtime->wYear = 1900 + tplocaltime->tm_year;
//	lpsystemtime->wMonth = tplocaltime->tm_mon + 1;
//	lpsystemtime->wDay = tplocaltime->tm_mday;
//	lpsystemtime->wHour = tplocaltime->tm_hour;
//	lpsystemtime->wMinute = tplocaltime->tm_min;
//	lpsystemtime->wSecond = tplocaltime->tm_sec;
//	
//	lpsystemtime->wMilliseconds = tv1.tv_usec / 1000;
//
//	return ;
//}


//接口函数，包括各种设置请求
int openQR(unsigned char *com)
{

	intQRFile = open_port(com);
	if(intQRFile < 0)
	{
		printf("open QR comm failure \n");
		return -1;
	}
	speed_set(intQRFile, 115200);
	parity_set(intQRFile, 8, 1, 'n');
	
	return intQRFile;
}

void closeQR()
{
	close_port(intQRFile);
}

void InitScan()  //初始化扫描头
{
	//
	tcflush(intQRFile, TCIOFLUSH);
	
	SetMode();//设置扫码工作模式
    SetInterval();//设置扫码成功后间隔
    DisableAll();//禁用所有的条码
    LightControl(0x00);//关闭LED灯
    BeepControl(0x00);//关闭蜂鸣器
}

void StartScan() //开始扫描
{

	BeepControl(0x01);
    LightControl(0x01);
    EnableQR();
    
    listened = true;
}

void EndScan()   //结束扫描
{
	DisableAll();
    LightControl(0x00);
    BeepControl(0x00);
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
char verify , ver;

	//printf("\nscancontrol...2\n");
	memset(chinfo, 0x00, sizeof(chinfo));
	
	lreadlen = 0;
	
	verify = 0x00; //校验位
	ver=0x00;//读数据的校验
	
	while (listened)
	{
		FD_ZERO(&readfd);
		FD_SET(intQRFile, &readfd);
		readTimeout.tv_sec = 0;
		readTimeout.tv_usec = 20000;
		ret = select(intQRFile + 1, &readfd, NULL, NULL, &readTimeout);
		if(ret > 0)
		{
			printf("\n");
			blnTimeout = 0xff;
			do
			{
				fStat = readcom(intQRFile, buff, 1);
				if(fStat)
				{
					printf(" %02X", buff[0]);
					//printf("\n");
					byttemp[lreadlen] = buff[0];
					sprintf(chinfo, "%02X ", buff[0]);
					lreadlen += 1;
					if(lreadlen >= 400)
					{
						return ;
					}
				}
				FD_ZERO(&intervalfd);
				FD_SET(intQRFile, &intervalfd);
				intervalTimeout.tv_sec = 0;
				intervalTimeout.tv_usec = 1000;
				ret = select(intQRFile + 1, &intervalfd, NULL, NULL, &intervalTimeout);
				if(ret > 0)
				{
					continue;
				}else
				{
					printf("len %d info: %s\n", lreadlen, chinfo);
					if( (lreadlen > 2) && (memcmp(&byttemp[lreadlen - 2], "\xd\xa", 2) == 0) )
					{
						memcpy(backdata, byttemp, lreadlen - 2);
						*backdatalength = lreadlen - 2;
					}else
						return -3;
					blnTimeout = 0;
				}
			}while(blnTimeout);
		
			break;
		}
		else if(ret == 0)
		{
			lreadlen = 0;
			return 1;
		}
		else
		{
			lreadlen = 0;
			return 2;
		}
	}
	return 0;
}
//结束