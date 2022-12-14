//sh_globle.h

#ifndef _SH_GLOBLE_H_
#define _SH_GLOBLE_H_
//start of file

//南京
#define TASK_KEY_IDLE         0
#define TASK_KEY_GET_COMMON   1
#define TASK_KEY_GET_METRO    2
#define TASK_KEY_GET_TAC      3
#define TASK_KEY_FINISH       255
//上海
#define TASK_KEY_GET_SHKEYA	  0x11
#define TASK_KEY_GET_SHTAC	  0x12
#define TASK_KEY_GET_SMKEYA		0x13
#define TASK_KEY_GET_SMTAC		0x14
#define TASK_KEY_GET_SHB28		0x15
#define TASK_KEY_SEND_AUTHCODE	0x16
#define TASK_KEY_INIT_FOR_SAM	0x17
#define TASK_KEY_INIT_FOR_SAM_2	0x18
//20130901新增定义 
#define TASK_KEY_SELECT_ADF		0x30
//世博
#define TASK_KEY_GET_SEAUTHCODE	0x20
#define TASK_KEY_GET_SEKEY			0x21
//二维码
#define TASK_KEY_GET_QRKEYA		0x40
#define TASK_KEY_GET_QRTAC		0x41
//ODA
#define TASK_KEY_GET_ODATAC		0x51

//交通部 20190404
#define TASK_KEY_GET_JTBKEYA		0x60
#define TASK_KEY_GET_JTBTAC		0x61

typedef void cmd_sub_func_type(UBYTE *,UWORD,UBYTE *,UWORD *); 


//返回码定义---------------------------------------------------------------------
#define ACK_OK	              0x00	//执行成功
#define ACK_SJTOK	            0x01	//单程票有效
#define ACK_UNKNOWNCMD	      0x02	//无效命令
#define ACK_BADPARAM	        0x03	//无效参数
#define ACK_READERTIMEOUT	    0x04	//读卡器超时
#define ACK_READERERROR	      0x05	//读卡器硬件错误
#define ACK_SENDFILEERROR	    0x06	//发送文件错误
#define ACK_TRANSACTIONERROR	0x07	//处理失败
#define ACK_READCARDERROR	    0x08	//读卡失败
#define ACK_WRITECARDERROR	  0x09	//写卡失败
#define ACK_UDNOTEXIST	      0x0A	//UD不存在
#define ACK_LOGNOTEXIST	      0x0B	//日志不存在


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
//上海卡
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

//上海单程票
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

//移动手机支付
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

//函数 
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
//交通部存、取记录
UBYTE save_transport_record(UBYTE *record);
UBYTE get_transport_record(UBYTE *inbuf,UBYTE *record);

//4016文件
struct HEAD_4016{
	UDWORD paramver[4];	//参数版本
	UBYTE macflag;			//R-MAC启用标志
	UBYTE favorflag;		//联程优惠启用标志
	UDWORD latedays;	//逾期天数
	UBYTE rfu[10];   	//RFU
	UBYTE recordnu;			//记录条数
};
struct DATA_4016{
	UBYTE rid[5];				//RID   			与认证中心公钥索引一起标识认证中心的公钥
	UBYTE pubkeyindex;	//公钥索引		与RID一起标识认证中心的公钥
	UBYTE ackeydate[4];	//认证中心公钥有效期
	UBYTE keyhashflag;	//认证中心公钥哈希算法标识
	UBYTE keyalgflag;		//认证中心公钥算法标识
	UWORD keylength;	//认证中心公钥模长度
	UBYTE	ackey[256];		//认证中心公钥模 n
	UDWORD keydata;		//公钥内容e
	UBYTE keylrc[20];		//认证中心公钥校验值
};

extern char *temp_4016;
extern UBYTE buf_4016[4];
//20190530 add
extern char *temp_4020;
extern UBYTE buf_4020[4];

//end of file
#endif