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

long EnableQR()  //ʹ��QR����0x21, 0x01, 0x00, 0x01
{
unsigned char	 bytBCD[7];
char bytEnableQR[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;

	if(intQRFile == 0)
		return 0;
	return (blnEnableQR = FD_EnableQR());
}

long DisableAll()    //�������е�����
{
unsigned char	 bytBCD[7];
char bytDisableAll[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;

	if(intQRFile == 0)
		return 0;
	return FD_DisableQR();
}

long SetMode()	//�����1s��ģʽ
{
unsigned char	 bytBCD[7];
char bytSetMode[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;



	return 0;
}

long SetInterval()   //����ɨ��ɹ�����
{
unsigned char	 bytBCD[7];
char bytSetInterval[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;

		
	return 0;
}

long LightControl(char LEDSwitch)	//LED�ƿ���
{
unsigned char	 bytBCD[7];
char bytLightControl[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;

		
	return 0;
}

long BeepControl(char bytBeepFlag)	//����������
{
unsigned char	 bytBCD[7];
char bytBeepControl[100], bytrecbuff[100];
int i, intInsertLen;
long rtn;

	

	return 0;
}



//�ӿں���������������������
int openQR(unsigned char *com)
{
	intQRFile = 0xff;
	
	return 0xff;
}

void closeQR()
{
	return ;
}

void InitScan()  //��ʼ��ɨ��ͷ
{
	//
	return ;
}

void StartScan() //��ʼɨ��
{

    EnableQR();
    //printf("start scaning...\n");
    listened = true;
}

void EndScan()   //����ɨ��
{
	DisableAll();
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
//����