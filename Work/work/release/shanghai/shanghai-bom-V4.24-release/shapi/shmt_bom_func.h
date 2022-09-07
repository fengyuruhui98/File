//shmt_bom_func.h

#ifndef _SHMT_BOM_FUNC_H_
#define _SHMT_BOM_FUNC_H_
#define OP_ERR			0xff
#define MAX_BUF 250
//交通卡指令

#define SM_INITREADER    0x20
#define SM_RESETREADER   0x21
#define SM_STOPREADER    0x22
#define SM_READERSTATUS  0x23
#define SM_BEEP		     0x2a
#define SM_DISPSIGNAL    0x2f
#define SM_READCARD      0x30
#define SM_ENTRY         0x31
#define SM_EXIT          0x32
#define SM_LOCKCARD      0x33
#define SM_ADDVALUE      0x34
#define SM_ENABLECARD    0x35
#define SM_UPDATE        0x36
#define SM_EXPIREDATE    0x37
#define SM_REPLACECARD   0x38
#define SM_READRECORD    0x39
#define SM_GETPROOF	     0x3a
#define SM_ABORT         0x3f
#define SM_TMOUNT        0x40
#define SM_AUTH          0x41
#define SM_GETAUTH       0x42
#define SM_VERIFYPIN     0x70
#define SM_MODIFYPIN     0x71 
#define OTX_GETPROOF	   0x5a
#define OT_PAY					 0x3b
#define SM_CHANGE_BAUD	 0x80

//中移动手机支付
#define MT_READCARD			 0x10
#define MT_ENTRY				 0xb1
#define MT_EXIT					 0xb2
#define MT_LOCK					 0xb3
#define MT_UPDATE				 0xb4
#define MT_GETPROOF			 0xb5

////磁浮命令 
//#define MM_READCARD		0xb0
//#define MM_ENTRY		0xb1
//#define MM_EXIT			0xb2

//薄型IC卡指令
#define HH_READCARD	 		0xC0
#define HH_ENTRY	 		0xC1
#define HH_EXIT		 		0xC2
#define HH_ABORT	 		0xCF
#define HH_ADDVALUE	 		0xC4  //单程票加值
#define HH_SALECARD	 		0xC5
#define HH_UPDATE	 		0xC6
#define HH_EXPIREDATA	 	0xC7  //单程票续期
#define HH_LOCK		 		0xC8
#define HH_ENTRY2	 		0xCA
#define HH_TK_KEY_LOAD	 	0xCC
#define HH_READRECORD	 	0xD0
#define HH_RETURNCARD   0xD2  //单程票退票
#define HH_GETPROOF	 		0xD3
//二维码指令
#define QR_READCARD			0xA0
#define QR_READ				0xA4
#define QR_ENTRY			0x6A
#define QR_EXIT				0x6B
#define	QR_UPDATE			0x6C
#define	QR_GETPROOF			0x6D

//测试指令	 
#define TEST_OPENRED 0x50
#define TEST_OPENGREEN 0x51
#define TEST_CLOSERED 0x52
#define	TEST_CLOSEGREEN 0x53
#define TEST_SJTSAM  0x54
#define TEST_CSCSAM  0x55
#define TEST_REQUEST 0x56
#define TEST_LOAD_SJT_KEY 0x62
#define TEST_SJTAuthen 0x63
#define TEST_SJTREAD   0x64
#define TEST_SJTWRITE  0x65
#define readstep	0xfe

#define TEST_MODE      0x00
#define ENTRY_MODE     0x20
#define EXIT_MODE      0x40
#define BOM_MODE       0x60
#define ATVM_MODE	   	 0x80
#define STOP_MODE      0xe0

#define IDLE_STATUS    0x00
#define READ_STATUS    0x01
#define TRANS_STATUS   0x02
#define WRITE_STATUS   0x04
#define STOP_STATUS    0x1f

//Pf
#define PBOC_READCARD    0x11
#define PBOC_READRECORD	 0xB9
#define PBOC_ENTRY       0xBA
#define PBOC_EXIT        0xBB
#define PBOC_UPDATE      0xBC
#define PBOC_GETPROOF    0xBD
#define PBOC_GETPARAMVER 0xBE
#define PBOC_SETPARAM    0xBF
#define EMPLOY_READCARD  0x1F
#define ET_ENTRY         0xE1
#define ET_EXIT          0xE2
#define ET_UPDATE        0xE3
#define ET_GETPROOF      0xE4
#define ET_LOCKCARD      0xE5
#define ET_READRECORD    0xE6
#define READER_VER			 0xF5

#define ST_ACTIVECARD    0xE0

#define FILE_TRANSFER    0xf1  
#define ACTIVE_TP_FILE   0xf2

#define	OT_GETHWVER			 0x2E

//硬件检测
#define TEST_HARDWARE_1		 0xF7
#define TEST_HARDWARE_2		 0xF6
//2.4GTest
#define TEST_24G 0xF8
//时钟设置
#define	SET_DATETIME	0xF9

//BOM相关命令
#define OTX_GETKEY		0X53
#define OT_CREDIT_INI	0X54
#define OT_CREDIT		0X55
#define OTX_UPDATE		0X56
#define OT_UNBLOCK		0X57
#define OT_GETRAN		0X58
#define OT_EXPIREDATE1	0X59
#define OT_FORBID		0X5B
#define OT_LOCKCARD1	0X5C
#define OT_CalcMAC		0X5D
#define OT_CheckMAC		0X5E
#define OT_ADDVALUE1	0X5F
#define OT_ENABLECARD1	0X60
#define OT_READRECORD1	0X61
#define OT_CREDIT_INI1	0x62
#define OT_ENABLECARD2	0x63
//ODA相关命令
#define	ODA_READOK		0xA1
#define	ODA_READERR		0xA2
#define	ODA_ENTRY		0xAA
#define	ODA_EXIT		0xAB
#define ODA_UPDATE		0xAC
#define	ODA_GETPROOF 	0xAD
#define	ODA_ENROLL		0xAE
#define	ODA_DISABLE		0xAF

//交通部相关命令
//#define	TRANSPORT_GETCARDINFO	0xD0
#define	TRANSPORT_ENTRY		  	0x41
#define	TRANSPORT_EXIT				0x42
#define TRANSPORT_UPDATE			0x43
#define TRANSPORT_GRTPROOF		0x44
#define TRANSPORT_LOCKCARD		0x45
#define TRANSPORT_RECORD			0x46
#define TRANSPORT_SETPARAM		0xB8
#define	JTB_CREDIT_INI			0x47
#define	JTB_CREDIT				0x48
//交通部二维码
#define	MQR_READCARD			0xAF
#define	MQR_SETPARAM			0x68
#define	MQR_TAC					0x69

extern UBYTE gStatus, gStatusNot;
extern cmd_sub_func_type *shmt_bom_func[256];
extern UBYTE oldsend[100],oldcmd,oldtimes;
extern UWORD oldsendcounter;
extern UBYTE gReadCmdTimes;

extern UBYTE gMode;
extern UBYTE gRespStep;

extern unsigned char  gBufBytes;
extern unsigned char  gBuf[MAX_BUF+1];
extern unsigned char  gOldCmd,gOldCmdTimes;
extern unsigned char  gSBuf[MAX_BUF+1];
extern unsigned char  gSBufBytes;
extern unsigned char  gMAC[6];

extern unsigned int  gNumTotal,gNumTotalNot;
extern unsigned long gValueTotal,gValueTotalNot;
extern unsigned long gShouquanSum,gShouquanSumNot;
extern unsigned long gShouquanTime,gShouquanTimeNot;
extern unsigned char gShouquanDate[7];
extern unsigned char gShaoquanSign,gShaoquanSignNot;
extern unsigned long gBackTransCnt,gBackTransCntNot;

//
void shmt_bom_ini(void);

void shmt_bom(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

void IniReader(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ResetReader(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void StopReader(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ReaderStatus(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ReaderDisp(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void AbortReader(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ReaderChangeBaud(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ReaderBeep(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

void ReadyToRead(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShCardEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShCardExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShCardLock(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShCardReadRecord(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShCardGetProof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShCardGetProofEx(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShCardPay(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShCardUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

void ShMetroActive(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroSale(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroSPEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroLock(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroGetProof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroAddValue(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroExpireData(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroReturnCard(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

void ShCardLockCPU(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShCardReadRecordCPU(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

void ShMobileExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMobileEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMobileLock(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMobileUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMobileGetproof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

void ShMetrotestopenred(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetrotestopengreen(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetrotestclosered(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetrotestclosegreen(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetrotestrequest(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetrotestloadkey(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetrotestSJTauth(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetrotestSJTread(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetrotestSJTwrite(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetrotestReadDebugstep(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void shmt_bom_func_err(UBYTE *outbuf,UWORD *outbytes);
void shmt_bom_func_bad_cmd(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

//PF
void ShMetroPfFileTransfer(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroPfActiveTpFile(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

void ShMetroPfPbocEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroPfPbocExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroPfPbocUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroPfPbocGetProof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroPfGetRec(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroPfPbocGetParamVer(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroPfPbocSetParam(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroPfEtEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroPfEtExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroPfEtGetRec(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroPfEtUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroPfEtGetProof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroPfEtLockCard(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetroReaderVer(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);	
void ShMetroGethwVer(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShTestHardWare(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShMetro24GModel(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

void ShQREntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShQRExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShQRUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShQRGetproof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void ShQRRead(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

void shSetDateTime(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
//BOM
void ShMetroRecord(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shCardGetkey(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);

void shCardCreditInit(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shCardCredit(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shCardUpdateEx(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shCardUnlock(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shCardGetRan(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shCardExpiredate1(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shCardForbid(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shCardLockcard1(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shCardCalcmac(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shCardAddvalue1(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shCardReadrecord1(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shCardCreditInit1(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shCardEnableCard2(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shCardEnableCard1(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);

//
void shODAEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void shODAExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void shODAUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void shODAGetProof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void shODAEnroll(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void shODADisable(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

//交通部
void shTransportGetCardInfo();
void shTransportEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void shTransportExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void shTransportUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void shTransportGetProof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void shTransportLockCard(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void shWhiteListSetParam(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void shTransportRecord(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shJTBCredit(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);
void shJTBCreditInit(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes);

void ShMQRTac(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void shCertSetParam(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

#endif