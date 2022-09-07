//sh_globle.h

#ifndef _SH_GLOBLE_H_
#define _SH_GLOBLE_H_
//start of file

//�Ͼ�
#define TASK_KEY_IDLE         0
#define TASK_KEY_GET_COMMON   1
#define TASK_KEY_GET_METRO    2
#define TASK_KEY_GET_TAC      3
#define TASK_KEY_FINISH       255
//�Ϻ�
#define TASK_KEY_GET_SHKEYA	  0x11
#define TASK_KEY_GET_SHTAC	  0x12
#define TASK_KEY_GET_SMKEYA		0x13
#define TASK_KEY_GET_SMTAC		0x14
#define TASK_KEY_GET_SHB28		0x15
#define TASK_KEY_SEND_AUTHCODE	0x16
#define TASK_KEY_INIT_FOR_SAM	0x17
#define TASK_KEY_INIT_FOR_SAM_2	0x18
//20130901�������� 
#define TASK_KEY_SELECT_ADF		0x30
//����
#define TASK_KEY_GET_SEAUTHCODE	0x20
#define TASK_KEY_GET_SEKEY			0x21
//��ά��
#define TASK_KEY_GET_QRKEYA		0x40
#define TASK_KEY_GET_QRTAC		0x41
//ODA
#define TASK_KEY_GET_ODATAC		0x51

//��ͨ�� 20190404
#define TASK_KEY_GET_JTBKEYA		0x60
#define TASK_KEY_GET_JTBTAC		0x61

typedef void cmd_sub_func_type(UBYTE *,UWORD,UBYTE *,UWORD *); 


//�����붨��---------------------------------------------------------------------
#define ACK_OK	              0x00	//ִ�гɹ�
#define ACK_SJTOK	            0x01	//����Ʊ��Ч
#define ACK_UNKNOWNCMD	      0x02	//��Ч����
#define ACK_BADPARAM	        0x03	//��Ч����
#define ACK_READERTIMEOUT	    0x04	//��������ʱ
#define ACK_READERERROR	      0x05	//������Ӳ������
#define ACK_SENDFILEERROR	    0x06	//�����ļ�����
#define ACK_TRANSACTIONERROR	0x07	//����ʧ��
#define ACK_READCARDERROR	    0x08	//����ʧ��
#define ACK_WRITECARDERROR	  0x09	//д��ʧ��
#define ACK_UDNOTEXIST	      0x0A	//UD������
#define ACK_LOGNOTEXIST	      0x0B	//��־������


extern UBYTE gGetCardInfoStep;
extern UWORD gDebugStep;
extern UBYTE gPowerOffFlag;
extern UDWORD mobilestarttime,mobilecurrent;
extern UBYTE TimeBuf[80];

extern UBYTE bgTaskKeyStep;
extern UBYTE bgTaskKeyInbuf[256];
extern UBYTE bgTaskKeyOutbuf[256];
extern UBYTE bgTaskKeyFinishState;
extern UBYTE gbTaskKeyNum;
extern UBYTE gThisCardSnr[16];
extern UBYTE temp_uid[4];
extern UBYTE bgHaltFlag;
//�Ϻ���
extern UBYTE gRedSign;
extern UBYTE gResponseStatus;
extern UBYTE gGetCardInfoStep;
extern UBYTE  gCardType;
extern UBYTE  cardversion;	
extern UBYTE  gCardStatus,gCardRecPtr;
extern UWORD  gCardCnt;
extern UBYTE  gCardExpiredDate[4];
extern UBYTE  NormalCard;
extern UBYTE  ReadMode;	
extern UBYTE  UseType;		
extern UBYTE  StartUseDay[4];	
extern UBYTE  FirstDay[4];	
extern UWORD  UseTimes;		
extern UBYTE  DayToAllow;
extern UBYTE  info[5];	
extern UBYTE leisure[5];	
extern UBYTE  gMetroIoFlag;
extern UBYTE  gMetroIoFlag34,gMetroIoFlagBk,gMetroIoFlag34Valid,gMetroIoFlagBkValid;
extern UDWORD gCardPurse1,gCardPurse2,gCardPurse;
extern UDWORD gCardAddSum;	
extern UDWORD gCardLastRemain;
extern UDWORD gCardThisTransValue;
extern UDWORD gCardThisRemain;
extern UBYTE  gStationTimeOverRideFlag;
extern UDWORD gCardLastTransValue;

//�Ϻ�����Ʊ
extern UBYTE hh_key[6];											
extern UBYTE hh_mid[2];											
extern UBYTE hh_SaleBuf[12];//b5,6,7					
extern UBYTE hh_B9[4];												
extern UBYTE hh_Entry[8];										
extern UBYTE hh_Exit[12];										
extern UBYTE hh_trans_Line,hh_trans_Station,hh_trans_Equip;		


extern UBYTE  gPosNo[4];
extern UBYTE  gTransTime[7];
extern UDWORD gPosTransCnt;
extern UBYTE  gRelativePtr;
extern UBYTE  gTAC[4];	

extern UBYTE Rand[10];
extern int gCurBaudRate;
extern UBYTE Eepromptr1,Eepromptr2;
extern UBYTE bgRledState,bgGledState,bgRState,bgGState,bgBuzzerState;

//extern UBYTE gTemp[200];
//extern UBYTE gTemp2[200];
extern UBYTE timeflag;
extern UDWORD dwgCmdExeTime;
extern UBYTE bgDeviceStatus;

//�ƶ��ֻ�֧��
#define DEFAULT_KEY_INDEX		0x03

typedef struct 
{
	UBYTE lastSimSn[10];
	UBYTE lastCardCnt[2];
	UBYTE lastPosTransCnt[4];
	UBYTE keybacon;
}TRANSPROVE_inram;


//2.4G
//extern UBYTE bgZTEReceLen;
//extern UBYTE bpgZTEReceBlock[300];
//extern UBYTE bgZTEReceState;

#define set_to_reset() {timer_set(TIMER_RESET,200); bgHaltFlag	=1;}
 
#define token_mcml_read(p1,p2)       mcml_read_4bytes(p1,p2)
#define token_mcml_write(p1,p2)      mcml_write_4bytes(p1,p2)
#define tocken_read(p1,p2)       mcml_read_4bytes(p1,p2)
#define tocken_write(p1,p2)      mcml_write_4bytes(p1,p2)

//���� 
int mifpro_ats0(UBYTE *outbuf);
int mifpro_icmd0(UBYTE len,UBYTE *sbuf,UBYTE *outbuf2);
UBYTE save_trans_record(UBYTE *record);
UBYTE save_sigal_record(UBYTE *record);
UBYTE get_trans_record(UBYTE *transptr,UBYTE *record_out,UBYTE ex_get);
UBYTE get_sigal_record(UBYTE *transptr,UBYTE *record_out);
void buzzerantenna(UBYTE option);
void gledantenna(UBYTE option);
void rledantenna(UBYTE option);
void display_set(UBYTE mode,UBYTE para);
void buzzer_set(UBYTE mode,UBYTE para);
//char tocken_read(UBYTE addr, UBYTE *readdata);
//char tocken_write(UBYTE addr, UBYTE *writedata);
int InitComm(int port,int baudrate,int stopbits,char parity);
void	CmdWatchCalMac(UWORD nLenIn,UBYTE *pszBufIn,UBYTE *pszInitData,UBYTE *pszKey,UBYTE *pszMAC,UBYTE	bTriDes);
UBYTE	WatchDiversity(UBYTE *pszMKKey,UBYTE *pszPID,UBYTE	*pszSKKey,UBYTE	bTriDes);
UBYTE mcmh_writewithout(UBYTE addr, UBYTE *writedata,UBYTE type,UBYTE key);
UBYTE mcmh_authkeyblock(UBYTE addr, UBYTE key);
int ISO7816CommandSend(int index,int ibytes,char* ibuf);
int ISO7816CommandRece(int channel,int* obytes,char* obuf,char *sw1sw2);
void shmt_prot_send(UBYTE *buf,UWORD bytes);

void shmt_read_pf4016(UBYTE *namebuf);
//void shmt_read_data4016(FILE *fp, unsigned char *inbuf, int len);

//20190530 add
void shmt_read_4020(UBYTE *namebuf);
//��ͨ���桢ȡ��¼
UBYTE save_transport_record(UBYTE *record);
UBYTE get_transport_record(UBYTE *inbuf,UBYTE *record);

//4016�ļ�
struct HEAD_4016{
	UDWORD paramver[4];	//�����汾
	UBYTE macflag;			//R-MAC���ñ�־
	UBYTE favorflag;		//�����Ż����ñ�־
	UDWORD latedays;	//��������
	UBYTE rfu[10];   	//RFU
	UBYTE recordnu;			//��¼����
};
struct DATA_4016{
	UBYTE rid[5];				//RID   			����֤���Ĺ�Կ����һ���ʶ��֤���ĵĹ�Կ
	UBYTE pubkeyindex;	//��Կ����		��RIDһ���ʶ��֤���ĵĹ�Կ
	UBYTE ackeydate[4];	//��֤���Ĺ�Կ��Ч��
	UBYTE keyhashflag;	//��֤���Ĺ�Կ��ϣ�㷨��ʶ
	UBYTE keyalgflag;		//��֤���Ĺ�Կ�㷨��ʶ
	UWORD keylength;	//��֤���Ĺ�Կģ����
	UBYTE	ackey[256];		//��֤���Ĺ�Կģ n
	UDWORD keydata;		//��Կ����e
	UBYTE keylrc[20];		//��֤���Ĺ�ԿУ��ֵ
};

extern char *temp_4016;
extern UBYTE buf_4016[4];
//20190530 add
extern char *temp_4020;
extern UBYTE buf_4020[4];

//end of file
#endif