#ifndef _BOM_FUNC_H_
#define _BOM_FUNC_H_


#define CityCode_KunShan    "\x21\x53"
#define CityCode_ChangShu   "\x21\x55"
#define CityCode_WuXi       "\x21\x40"

#define REQ_CNT    3 //global request times,20090831
#define ADD_CLR_FLAG  0x66
#define SALE_CLR_FLAG 0x77 
#define AUTH_MODE_EXT_KEYA   0    //外部
#define AUTH_MODE_EXT_KEYB   1

#define AUTH_MODE_INT_KEYA   4    //内部
#define AUTH_MODE_INT_KEYB   5

uchar FM_Module_NO[6]; //模块编号
uchar gRandom[8];
extern uchar CntCardNewStartDate[4];
uchar gCardNewExpiredDate[4];
extern uchar  gM1AddValKey[16];//
extern uchar  gM1AddValBlock[16];//
extern uchar  gAddValMAC[6];//
extern uchar  gCardRecord[16];
uchar gM1ConsumptionStatus;
uchar  gCardIssueNo[4];
uchar  gCardMac[4];
extern UBYTE  gB8[16],gB9[16],gB10[16],gB1[16],gB2[16];
extern uchar  gLineProtectionMAC[6];
extern UBYTE gShFormalCardKeyA0[6];
extern UBYTE gShTestCardKeyA0[6];
extern UBYTE gB1[16];

char gThisCpuCardSnr[8];
extern uchar gTransType;
extern uchar gApplicationSn[10]; //应用序列号	10
extern uchar gNewACardSnr[5];
extern uchar gApplicationTypeTag; //应用类型标识	1
extern uchar  gM1SaleValKey[16];//
extern uchar  gM1SaleValBlock[16];//
extern uchar  gSaleValMAC[6];//1status+4MAC

uchar trans_setup_bakup(void);
char trans_credit_ini(void);
char trans_credit(void);
char trans_cpu_app_unlock(void);
char trans_cpu_get_random(void);
char trans_expiredate1(void);
char trans_forbid_mifare(void);
char trans_cpu_lock(char lock_type);
char trans_add(void);
char trans_read_cpu_record(uchar recordno);
char trans_credit_ini1(void);
char trans_sale_cpu(void);
uchar psam_get_chk(uchar *inbuf,uchar *outbuf);
char trans_sale(void);

char sam_getnewkeya(void);


char jtb_credit(void);
char jtb_credit_ini(void);

#endif