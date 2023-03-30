#include <sys/time.h>
#include <fcntl.h>

int intQRFile;

typedef enum
{
    false,
    true
}bool;
bool listened = false;

long EnableQR()  //ʹ��QR����0x21, 0x01, 0x00, 0x01
{
unsigned char	 bytBCD[7];
char bytEnableQR[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;


	if(intQRFile <= 0)
		return -1;

	return 0;
}

long DisableAll()    //�������е�����
{
unsigned char	 bytBCD[7];
char bytDisableAll[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;


	if(intQRFile <= 0)
		return -1;
		
	return 0;
}

long SetMode()	//�����1s��ģʽ
{
unsigned char	 bytBCD[7];
char bytSetMode[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;


	if(intQRFile <= 0)
		return -1;
		

	return 0;
}

long SetInterval()   //����ɨ��ɹ�����
{
unsigned char	 bytBCD[7];
char bytSetInterval[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;

	if(intQRFile <= 0)
		return -1;
		
	return 0;
}

long LightControl(char LEDSwitch)	//LED�ƿ���
{
unsigned char	 bytBCD[7];
char bytLightControl[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;


	if(intQRFile <= 0)
		return -1;
		
	return 0;
}

long BeepControl(char bytBeepFlag)	//����������
{
unsigned char	 bytBCD[7];
char bytBeepControl[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;

	if(intQRFile <= 0)
		return -1;
		

	return 0;
}

//void GetLocalTime(struct SYSTEMTIME *lpsystemtime)	//��ȡ��ǰʱ��
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


//�ӿں���������������������
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

void InitScan()  //��ʼ��ɨ��ͷ
{
	//
	tcflush(intQRFile, TCIOFLUSH);
	
	SetMode();//����ɨ�빤��ģʽ
    SetInterval();//����ɨ��ɹ�����
    DisableAll();//�������е�����
    LightControl(0x00);//�ر�LED��
    BeepControl(0x00);//�رշ�����
}

void StartScan() //��ʼɨ��
{

	BeepControl(0x01);
    LightControl(0x01);
    EnableQR();
    
    listened = true;
}

void EndScan()   //����ɨ��
{
	DisableAll();
    LightControl(0x00);
    BeepControl(0x00);
    listened = false;
}

//void scancontrol((*CallBackFun)(char *p) *backdata ,long *backdatalength)//���д���ص������Ľӿ�
int scancontrol(unsigned char *backdata ,long *backdatalength)//���д���ص������Ľӿ�
{
unsigned char fStat, blnTimeout;
unsigned char buff[500], byttemp[500];
char chinfo[1200], chtempinfo[10];
int lreadlen, rtn, ret;
fd_set	readfd, intervalfd;
struct timeval readTimeout, intervalTimeout;
char verify , ver;

	memset(chinfo, 0x00, sizeof(chinfo));
	
	lreadlen = 0;
	
	verify = 0x00; //У��λ
	ver=0x00;//�����ݵ�У��
	
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
//����