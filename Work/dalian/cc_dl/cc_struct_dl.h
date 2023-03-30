#ifndef CC_STRUCT_DL_H
#define CC_STRUCT_DL_H

#include "base_define.h"

 
#ifndef MAX_BAOWEN_LENGTH
#define MAX_BAOWEN_LENGTH	8*1024	    /* ------- 报文的最大长度 ----- */
#endif


#pragma pack (1)
  
typedef struct _para
{
	unsigned char recv_buf[MAX_BAOWEN_LENGTH];
	int buflen;
	int sockhandle;
	char ipaddr[20];
	int  port;
	unsigned char addinfo[3];
	char nodeid[9];
}para;
 
typedef struct _ParamStru
{	
	unsigned char* recv_buf;
	WORD paramtype;
	int buflen;
	char ipaddr[20];
	int  port;
	unsigned char addinfo[3];
}ParamStru;


typedef struct _Lcc_Ftp_File
{
	char FILE_NAME[64];
	int  FILE_LENGTH;
	int  FILE_RECORDS;
	char BELONGS_FILE_NAME[64];
}Lcc_Ftp_File;

 
typedef struct _ParaHead
{
	BYTE	ParamVer[7];		/* ---  --- */
	BYTE	ValidDate[7];		/* ---  --- */
}ParaHead;


typedef struct _ParamVer
{
	unsigned short	wParaCode;
	unsigned char	  ParamVer[7];
}ParamVer;


typedef struct _MsgHead
{
	unsigned int	dw_PkgDefault;			/* ---  --- */
	unsigned char	b_SourceID[4];			/* ---  --- */
	unsigned char	b_DestID[4];			/* ---  --- */
	unsigned short	w_MsgCode;				/* ---  --- */
	unsigned int	dw_PkgID;				/* ---  --- */
	unsigned char	b_PkgID[12];			/* ---  --- */
	unsigned int	dw_PkgLen;				/* ---  --- */
	unsigned char	b_IsACK;				/* ---  --- */
	unsigned char	b_ZipType;				/* ---  --- */
	unsigned int	dw_ZipPkgLen;			/* ---  --- */
	unsigned char	b_EncryptType;			/* ---  --- */
	unsigned char	b_EncryptCRC[4];		/* ---  --- */
}MsgHead;



 
typedef struct _ftp_msg 
{ 
	char	ftpIP[16]; 
	int		ftpPort; 		   
	char	ftpUser[32];	
	char	ftpPass[32];
	char	ftpPath[64]; 
}FtpMsg; 


/*----------------------------------------
------------------参数数据------------------
-----------------------------------------*/
//HHJT_CJP 20210319  线路参数
typedef struct  _Para0001_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
	WORD  Records;        /* ---  --- */
}Para0001_Head;
typedef struct _Para0001_Record
{
	BYTE	PARTICIPANT_ID;		/* ---  --- */
	BYTE  CENTER_CODE;		 /* ---  --- */
	BYTE	LINE_ID;			   /* ---  --- */
	char  LINE_NAME_CN[40];			/* ---  --- */
	char	LINE_NAME_EN[40];		/* ---  --- */
	char	LINE_NAME_EN_SIMP[40];			/* ---  --- */
	char	UP_FIRST_TIME[3];				/* ---  --- */
	char	UP_LAST_TIME[3];				/* ---  --- */
	char	DOWN_FIRST_TIME[3];				/* ---  --- */
	char	DOWN_LAST_TIME[3];				/* ---  --- */
}Para0001_Record;		 

//HHJT_CJP 20210319  车站参数
typedef struct  _Para0002_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
	WORD  Records;        /* ---  --- */
}Para0002_Head;
typedef struct _Para0002_Record
{
BYTE	STATION_NID[2];			/* ---  --- */
char  STATION_NAME_CN[40];			/* ---  --- */
char	STATION_NAME_EN[40];		/* ---  --- */
}Para0002_Record;		 

//HHJT_CJP 20210319  换乘站参数
typedef struct  _Para0004_Head
{
	BYTE	ParamVer[7];		
	WORD  Records;        
}Para0004_Head;
typedef struct _Para0004_Record
{
BYTE						STATION_NID[2];			
BYTE  					ORIG_LINE_ID;		
BYTE						DEST_LINE_ID;	
unsigned short 	EXCHANGE_TIME;
}Para0004_Record;		

//HHJT_CJP 20210319  票型信息参数
typedef struct  _Para0005_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
	WORD  Records;        /* ---  --- */
}Para0005_Head;
typedef struct _Para0005_Record_1
{		
BYTE						TICKET_TYPE[2];	
char						TICKET_NAME[50];	
BYTE						TICKET_CLASS;	
BYTE						NORMAL_VALUE;	
BYTE						HOLIDAY_VALUE;	
BYTE						WEEKEND_VALUE;	
BYTE						PRAK_VALUE;	
BYTE						CTRL_BITMAP1;	
BYTE						CTRL_BITMAP2;	
BYTE						CTRL_BITMAP3;	
BYTE						CTRL_BITMAP4;	
unsigned int		REFUND_MAX_VALUE;		
BYTE						LIGHT_INDEX;
BYTE						i_MAX_COUNT_PER_DAY;
unsigned short	INOUT_MAX_PERIOD;		
unsigned int		MAX_OVERDRAFT;	
unsigned short	FREE_IN_UPDATE_MAX_PERIOD;		
unsigned short	FREE_OUT_UPDATE_MAX_PERIOD;		
BYTE						VALID_TYPE;	
unsigned int		VALID_VALUE;	
BYTE						i_VALID_PRE_DAYS;	
char 						VALID_START_DATE[4];
char 						VALIE_END_DATE[4];
unsigned int		DISCOUNT_PERCENT;		
unsigned int		DISCOUNT_VALUE;		
unsigned int		MIN_VALUE;		
unsigned int		MAX_VALUE;		
unsigned int		LOAD_MIN_VLAUE;			
unsigned int		LOAD_MAX_VALUE;	
unsigned int		INIT_SALE_DISCOUNT;		
unsigned int		LOAD_VALUE;		
unsigned int		LOAD_COUNT;		
unsigned int		DEPOSIT;		
unsigned int		REFUND_FEE;		
unsigned int		CHANGE_TICKET_FEE;		
unsigned int		LOST_PENALTY;		
unsigned int		DAMAGE_PENALTY;		
unsigned int		TIMEOUT_PENALTY;		
unsigned int		IN_UPDATE_VALUE;		
unsigned int		OUT_UPDATE_VALUE;		
BYTE						VALUE_TYPE;
BYTE						AWARD_MODE;
BYTE						i_UNITED_AWARD_INDUSTRY;		
unsigned short	UNITED_AWARD_PERIOD;	
BYTE						UNITED_AWARD_TYPE;		
unsigned int		UNITED_AWARD_VALUE;	
BYTE						CUMULATE_AWARD_TYPE;	
BYTE						CUMULATE_TYPE;	
unsigned short  Records;  
}Para0005_Record_1;
typedef struct _Para0005_Record_2
{		
unsigned int		CUMULATE_VALUE;			
unsigned int		CUMULATE_AWARD_VALUE;		
}Para0005_Record_2;
typedef struct _Para0005_Record_3
{				
unsigned int		INOUT_VALUE;		
}Para0005_Record_3;

//HHJT_CJP 20210319  车站区间信息参数
typedef struct  _Para0006_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
	DWORD  Records;        /* ---  --- */
}Para0006_Head;
typedef struct _Para0006_Record
{		
BYTE	ORIG_STATION_NID[2];	
BYTE	DESC_STATION_NID[2];	
BYTE	FARE_ZONE_CODE;		
}Para0006_Record;

//HHJT_CJP 20210319  区间票价信息参数
typedef struct  _Para0007_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
	WORD  Records;        /* ---  --- */
}Para0007_Head;
typedef struct _Para0007_Record
{		
BYTE	ZONE_CODE;	
char	ZONE_NAME[15];	
unsigned short	FARE_VALUE_1;		
unsigned short	FARE_VALUE_2;		
unsigned short	FARE_VALUE_3;		
unsigned short	FARE_VALUE_4;		
unsigned short	FARE_VALUE_5;		
unsigned short	FARE_VALUE_6;		
unsigned short	FARE_VALUE_7;		
unsigned short	FARE_VALUE_8;		
unsigned short	FARE_VALUE_9;		
unsigned short	FARE_VALUE_10;		
unsigned short	FARE_VALUE_11;		
unsigned short	FARE_VALUE_12;		
unsigned short	FARE_VALUE_13;		
unsigned short	FARE_VALUE_14;		
unsigned short	FARE_VALUE_15;		
unsigned short	FARE_VALUE_16;	
unsigned short	STRANDED_MAX_PERIOD;		
}Para0007_Record;

//HHJT_CJP 20210319 区段计次票区间票价信息
typedef struct  _Para0008_Head
{
BYTE						ParamVer[7];		/* ---  --- */
unsigned short	Records;        /* ---  --- */
}Para0008_Head;
typedef struct _Para0008_Record_1
{
BYTE						ZONE_CODE;			
BYTE						TICKET_TYPE[2];
unsigned short  Records;
}Para0008_Record_1;
typedef struct _Para0008_Record_2
{		
unsigned short		RIDES;			
unsigned short		SALE_VALUE;		
}Para0008_Record_2;

//HHJT_CJP 20210319 区段定期票发售金额
typedef struct  _Para0009_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
	WORD  Records;        /* ---  --- */
}Para0009_Head;
typedef struct _Para0009_Record
{
BYTE						ZONE_CODE;			
BYTE						TICKET_TYPE[2];
unsigned short  SALE_VALUE;
}Para0009_Record;

//HHJT_CJP 20210319 纪念票号段发售金额
typedef struct  _Para000A_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
	WORD  Records;        /* ---  --- */
}Para000A_Head;
typedef struct _Para000A_Record
{
BYTE						TICKET_TYPE[2];			
BYTE						START_TICKET_NO[5];
BYTE						END_TICKET_NO[5];
unsigned short  SALE_VALUE;
char            MEMO[100];
}Para000A_Record;

//HHJT_CJP 20210319 一票通全量黑名单信息
typedef struct  _Para000B_Head
{
	BYTE						ParamVer[7];		/* ---  --- */
	BYTE  					BLACKLIST_CLASS;
	unsigned short  wRecords_1;
	unsigned short  wRecords_2;
}Para000B_Head;
typedef struct  _Para000B_Record_1
{
	BYTE						TICKET_NO[5];		/* ---  --- */
	BYTE  					BLACKLIST_USAGE;
	BYTE            BLACKLIST_TYPE;
	BYTE  					ADD_FLAG;
	BYTE  					SALE_ACTION_FLAG;
	BYTE  					TVM_ACTION_FLAG;
}Para000B_Record_1;
typedef struct  _Para000B_Record_2
{
	BYTE						START_TICKET_NO[5];		/* ---  --- */
	BYTE  					END_TICKET_NO[5];
	BYTE  					BLACKLIST_USAGE;
	BYTE            BLACKLIST_TYPE;
	BYTE  					SALE_ACTION_FLAG;
	BYTE  					CHECK_ACTION_FLAG;
}Para000B_Record_2;
typedef struct  _Para000B_Record_3
{
	BYTE						CRC[4];	
}Para000B_Record_3;

//HHJT_CJP 20210319 SAM卡黑名单信息
typedef struct  _Para000C_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
	WORD  Records;        /* ---  --- */
}Para000C_Head;
typedef struct  _Para000C_Record_1
{
	BYTE						SAM_ID[6];	
	BYTE						TVM_ACTION_FLAG;	
}Para000C_Record_1;
typedef struct  _Para000C_Record_2
{
	BYTE						CRC[4];	
}Para000C_Record_2;

//HHJT_CJP 20210319 一卡通黑名单信息
typedef struct  _Para000D_Head
{
	BYTE						ParamVer[7];		/* ---  --- */
	BYTE  					BLACKLIST_CLASS;
	unsigned short  wRecords_1;
	unsigned short  wRecords_2;
}Para000D_Head;
typedef struct  _Para000D_Record_1
{
	BYTE						TICKET_NO[8];		/* ---  --- */
	BYTE  					BLACKLIST_USAGE;
	BYTE            BLACKLIST_TYPE;
	BYTE  					ADD_FLAG;
	BYTE  					SALE_ACTION_FLAG;
	BYTE  					TVM_ACTION_FLAG;
}Para000D_Record_1;
typedef struct  _Para000D_Record_2
{
	BYTE						START_TICKET_NO[8];		/* ---  --- */
	BYTE  					END_TICKET_NO[8];
	BYTE  					BLACKLIST_USAGE;
	BYTE            BLACKLIST_TYPE;
	BYTE  					SALE_ACTION_FLAG;
	BYTE  					TVM_ACTION_FLAG;
}Para000D_Record_2;
typedef struct  _Para000D_Record_3
{
	BYTE						CRC[4];	
}Para000D_Record_3;

//HHJT_CJP 20210319 一票通增量黑名单信息
typedef struct  _Para000E_Head
{
	BYTE						ParamVer[7];		/* ---  --- */
	BYTE  					BLACKLIST_CLASS;
	unsigned short  wRecords_1;
	unsigned short  wRecords_2;
}Para000E_Head;
typedef struct  _Para000E_Record_1
{
	BYTE						TICKET_NO[5];		/* ---  --- */
	BYTE  					BLACKLIST_USAGE;
	BYTE            BLACKLIST_TYPE;
	BYTE  					ADD_FLAG;
	BYTE  					SALE_ACTION_FLAG;
	BYTE  					TVM_ACTION_FLAG;
}Para000E_Record_1;
typedef struct  _Para000E_Record_2
{
	BYTE						START_TICKET_NO[5];		/* ---  --- */
	BYTE  					END_TICKET_NO[5];
	BYTE  					BLACKLIST_USAGE;
	BYTE            BLACKLIST_TYPE;
	BYTE  					SALE_ACTION_FLAG;
	BYTE  					CHECK_ACTION_FLAG;
}Para000E_Record_2;
typedef struct  _Para000E_Record_3
{
	BYTE						CRC[4];	
}Para000E_Record_3;

//HHJT_CJP 20210319 交通部白名单信息
typedef struct  _Para000F_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
	BYTE  Type;
	WORD  Records;        /* ---  --- */
}Para000F_Head;
typedef struct  _Para000F_Record_1
{
	BYTE						WHITE_LIST_CODE[8];	
	BYTE						WHITE_LIST_INFO[16];	
}Para000F_Record_1;
typedef struct  _Para000F_Record_2
{
	BYTE						CRC[4];	
}Para000F_Record_2;

//HHJT_CJP 20210319  节假日信息参数
typedef struct  _Para0011_Head
{
	BYTE	ParamVer[7];		
	WORD  Records;        
}Para0011_Head;
typedef struct _Para0011_Record
{
BYTE						HOLIDAY_DATE[4];			
BYTE  					HOLIDAY_TYPE;		
}Para0011_Record;	

//HHJT_CJP 20210319  高峰时段信息参数
typedef struct  _Para0012_Head
{
	BYTE	ParamVer[7];		
	WORD  Records;        
}Para0012_Head;
typedef struct _Para0012_Record
{
char						START_TIME[3];			
char  					END_TIME[3];		
}Para0012_Record;	

//HHJT_CJP 20210319  TVM主界面版本参数
typedef struct  _Para0013_Head
{
	BYTE	ParamVer[7];		
	BYTE  Records_1;        
}Para0013_Head;
typedef struct _Para0013_Record_1
{
BYTE						LINE_ID;			
BYTE  					LANG_VER;		
unsigned int		PIC_LENGTH;
}Para0013_Record_1;	
typedef struct _Para0013_Record_2
{	
unsigned int		Records_2;
}Para0013_Record_2;
typedef struct _Para0013_Record_3
{
BYTE						NODE_TYPE;			
BYTE  					NODE_ID;		
BYTE  					NODE_PROPERTIE[2];	
unsigned int		X_POINT;
unsigned int		Y_POINT;
unsigned int		BUTTON_WIDTH;
unsigned int		BUTTON_HEIGHT;
}Para0013_Record_3;	

//HHJT_CJP 20210319  参数版本控制信息
typedef struct _Para_0015_1
{
	unsigned char	b_ParamVer[7];			/* ---  --- */
	unsigned char	b_ParamValidTime[7];	/* ---  --- */
	unsigned int	dw_ParamRecords;		/* ---  --- */
}Para_0015_1;
typedef struct _Para_0015_2
{
	unsigned short	w_ParamType;			/* ---  --- */
	unsigned char	b_ParamVer[7];			/* ---  --- */
}Para_0015_2;

//HHJT_CJP 20210325  CRW程序参数
typedef struct  _Para0016_Head
{
	BYTE						ParamVer[7];			/* ---参数版本 --- */
	char						SoftVersion[12];	/* ---软件版本 --- */
	unsigned int		SoftLength;				/* ---软件长度 --- */	
}Para0016_Head;

//HHJT_CJP 20210325  CRW参数信息
typedef struct  _Para0017_Head
{
	BYTE						b_records;				/* ---参数记录数 --- */
	BYTE						ParamVer[7];			/* ---参数版本 --- */
}Para0017_Head;
 

//HHJT_CJP 20210319 模式履历参数
typedef struct  _Para0018_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
	WORD  Records;        /* ---  --- */
}Para0018_Head;
typedef struct _Para0018_Record
{
	BYTE	LINE_ID;			
	BYTE	STATION_NID[2];			
	BYTE	RUNMODE_CODE;	
	BYTE  OCCUR_TIME[7];			
	BYTE	SHIFT_TIME[7];			
	BYTE	AFFECT_TIME[7];		
}Para0018_Record;

//HHJT_CJP 20210319 票卡目录参数
typedef struct  _Para0019_Head
{	
	WORD  Records;        
}Para0019_Head;
typedef struct _Para0019_Record
{
BYTE						DIREC_CODE[2];			
char  					DIREC_NAME[40];		
}Para0019_Record;	

//HHJT_CJP 20210319 节点通信参数信息
typedef struct _Para001A_Record
{
unsigned short	FILE_MAX_RECORD;		
unsigned short	COMMIT_INTERVAL;	
unsigned short	AUDIT_INTERVAL;	
unsigned short	DEVICE_INTERVAL;	
BYTE            LOAD_AUTH_IP[4];	
unsigned short	LOAD_AUTH_PORT;
BYTE            SEND_COUNT;	
BYTE            CLEAR_SHIFT_TIME[2];	
BYTE            TRANS_SHIFT_TIME[2];	
}Para001A_Record;

//HHJT_CJP 20210319 银联PAY公钥信息
typedef struct  _Para0401_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
	WORD  Records;        /* ---  --- */
}Para0401_Head;

//HHJT_CJP 20210319 互联网应用参数设置信息
typedef struct  _Para0402_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
}Para0402_Head;

//HHJT_CJP 20210319 脱机二维码公钥信息
typedef struct  _Para0403_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
}Para0403_Head;

//HHJT_CJP 20210319 银联PAY黑名单信息
typedef struct  _Para0406_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
	WORD  Records;        /* ---  --- */
}Para0406_Head;


//HHJT_CJP 20210319  LC参数版本控制信息
typedef struct  _Para0300_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
	BYTE	ValidTime[7];				/* ---  --- */
	DWORD	Records;			/* ---  --- */
}Para0300_Head;
typedef struct _Para0300_Record
{
	BYTE	PARA_TYPE[2];	/* ---  --- */
	BYTE  SUB_PARA_INDEX[7];
}Para0300_Record;	


typedef struct _Para_0300_1
{
	unsigned char	b_ParamVer[7];			/* ---  --- */
	unsigned char	b_ParamValidTime[7];	/* ---  --- */
	unsigned int	dw_ParamRecords;		/* ---  --- */
}Para_0300_1;
typedef struct _Para_0300_2
{
	unsigned short	w_ParamType;			/* ---  --- */
	unsigned char	  b_ParamVer[7];			/* ---  --- */
}Para_0300_2;


//HHJT_CJP 20210319  车站节点配置参数
typedef struct  _Para0301_Head
{
	BYTE	ParamVer[7];		/* ---  --- */
	unsigned int	ValidTime;				/* ---  --- */
	BYTE	STATION_NID[4];	/* ---  --- */
	char	STATION_MAP[64];		/* ---  --- */
	BYTE	MASTER_SERVER_IP[4];		/* ---  --- */
	BYTE	STANDY_SERVER_IP[4];		/* ---  --- */
	WORD	SERVER_PORT;			/* ---  --- */
	BYTE	DeviceGroup[16*67];/* ---  --- */
	DWORD	Records;			/* ---  --- */
}Para0301_Head;
typedef struct _Para0301_Record
{
BYTE	DEVICE_NID[4];		/* ---  --- */
char  DEVICE_NAME[8];		/* ---  --- */
BYTE	DEVICE_TYPE;			/* ---  --- */
BYTE	DEVICE_IP[4];		/* ---  --- */
BYTE  DEVICE_MAC[8];			/* ---  --- */
WORD	SERVER_PORT;		/* ---  --- */
BYTE	Group;			/* ---  --- */
DWORD	X_POINT;				/* ---  --- */
DWORD	Y_POINT;				/* ---  --- */
BYTE	ROTATE_ANGEL;			/* ---  --- */
BYTE  USE_FLAG;		/* ---  --- */
}Para0301_Record;		/* ---  --- */
typedef struct _Para0301_Tail
{
	char	START_OPERATE_TIME[3];		/* ---  --- */
	WORD	START_DELAY_TIME;	/* ---  --- */
	char	END_OPERATE_TIME[3];			/* ---  --- */
	WORD	END_DELAY_TIME;	/* ---  --- */
	BYTE  DEVICE_RESTART_TIME;
	BYTE  RFU[7];				/* ---  --- */
}Para0301_Tail;	

//HHJT_CJP 20210319  操作员权限参数
typedef struct  _Para0302_Head
{
	BYTE	ParamVer[7];	
	unsigned int	ValidTime;				/* ---  --- */			
}Para0302_Head;
typedef struct  _Para0302_Record_1
{	
	unsigned int	Records_1;	
}Para0302_Record_1;
typedef struct _Para0302_Record_2
{
	BYTE					ROLE_ID;		
	char					ROLE_DESC[32];
	unsigned int	Records_2;	
}Para0302_Record_2;	
typedef struct _Para0302_Record_3
{
	BYTE					DEVICE_TYPE;		
	BYTE					ROLE_RIGHT;
	char					FUNC_DESC[64];
}Para0302_Record_3;	
typedef struct  _Para0302_Record_4
{	
	unsigned int	Records_3;	
}Para0302_Record_4;
typedef struct  _Para0302_Record_5
{	
	BYTE					USER_ID[4];	
	char					USER_NAME[32];
	char					COMPANY_NAME[64];
	BYTE					USER_PIN[16];
	BYTE					USE_TIME[7];
	BYTE					VALID_TIME[7];
	BYTE					DEVICE_PRIV;	
	unsigned int	Records_4;
}Para0302_Record_5;
typedef struct  _Para0302_Record_6
{	
	BYTE					ROLE_ID;	
}Para0302_Record_6;
typedef struct  _Para0302_Record_7
{	
	BYTE					RFU_1[4];	
	BYTE					RFU_2[4];
	BYTE					RFU_3[4];	
}Para0302_Record_7;

//HHJT_CJP 20210319  设备运营参数
typedef struct _Para0303
{
	BYTE	ParamVer[7];												/* ---  --- */
	unsigned int	ValidTime;				/* ---  --- */
	BYTE	ERROR_NBR;													/* ---  --- */
	BYTE	WRITE_INCOM_TIME;										/* ---  --- */
	BYTE	WRITE_INCOM_RETRY_COUNT;						/* ---  --- */
	BYTE	TICKET_TYPE[2];											/* ---  --- */
	BYTE	PASSWD_ENTRY_TIME;									/* ---  --- */
	BYTE	LOGIN_RETRY_COUNT;									/* ---  --- */
	BYTE	AUTO_LOGOUT_TIME;										/* ---  --- */
	BYTE	DOOR_CLOSE_ALERT;										/* ---  --- */
	BYTE	TICKETBOX_PRE_EMPTY_PERCENTAGE;			/* ---  --- */
	BYTE	TICKETBOX_EMPTY_PERCENTAGE;					/* ---  --- */
	BYTE	TICKETBOX_PRE_FULL_PERCENTAGE;			/* ---  --- */
	BYTE	TICKETBOX_FULL_PERCENTAGE;					/* ---  --- */
	BYTE	TRASH_PRE_FULL_PERCENTAGE;					/* ---  --- */
	BYTE	TRASH_FULL_PERCENTAGE;							/* ---  --- */
	BYTE	CNT_BOTTOM_LINE;										/* ---  --- */
	BYTE	TIME_TOP_LINE;											/* ---  --- */
	BYTE	TIME_REPORT_STATUS;									/* ---  --- */
	BYTE	TIME_REPORT_AUDIT;									/* ---  --- */
	BYTE	RFU2[7];										        /* ---  --- */
}Para0303;

//HHJT_CJP 20210319  TVM参数信息
typedef struct _Para0304
{
	BYTE						ParamVer[7];										/* ---  --- */
	unsigned int	ValidTime;				/* ---  --- */
	BYTE						MAX_SALE;												/* ---  --- */
	BYTE						TICKET_TIMEOUT;									/* ---  --- */
	BYTE						COIN_WAIT;						          /* ---  --- */
	BYTE						RFU2[20];												/* ---  --- */
	BYTE						ALLOW_STANDBY;									/* ---  --- */
	unsigned short	STANDBY_TIME;										/* ---  --- */
	BYTE						SALE_CASH_TYPE[2];							/* ---  --- */
	BYTE						SALE_COIN_TYPE[2];							/* ---  --- */
	unsigned short	MAX_CASH_AMOUNT;								/* ---  --- */
	BYTE						MAX_CASH_COUNT;									/* ---  --- */
	BYTE						LOAD_CASH_TYPE[2];							/* ---  --- */
	unsigned int		LOAD_CASH_AMOUNT;								/* ---  --- */
	BYTE						LOAD_CASH_COUNT;								/* ---  --- */
	BYTE						IS_SALE_NO_COIN;								/* ---  --- */
	BYTE						MAX_COIN_CHANGE_COUNT;					/* ---  --- */
	BYTE						IS_BILL_CHANGE;									/* ---  --- */
	BYTE						MAX_CHANGE_BILL_COUNT;					/* ---  --- */
	unsigned short	MAX_CHANGE_BILL_AMOUNT;					/* ---  --- */
	unsigned short	CASHBOX_PRE_FULL_COUNT;					/* ---  --- */
	unsigned short	CASHBOX_FULL_COUNT;							/* ---  --- */
	unsigned short	COIN_TRASH_PRE_FULL_COUNT;			/* ---  --- */
	unsigned short	COIN_TRASH_FULL_COUNT;					/* ---  --- */
	unsigned short	COIN_CHANGE_PRE_EMPTY_COUNT;		/* ---  --- */
	unsigned short	CASH_CHANGE_PRE_EMPTY_COUNT;		/* ---  --- */
}Para0304;

//HHJT_CJP 20210319  BOM参数信息
typedef struct _Para0305
{
	BYTE	ParamVer[7];												/* ---  --- */
	unsigned int	ValidTime;				/* ---  --- */
	BYTE	ERROR_NBR;													/* ---  --- */
	BYTE	WRITE_RETRY_OVER_TIME;										/* ---  --- */
	BYTE	WRITE_RETRY_COUNT;						/* ---  --- */
	unsigned short	AUTO_LOGOUT_TIME;										/* ---  --- */
	BYTE	TRANS_OVER_TIME;										/* ---  --- */
	BYTE	TICKETBOX_PRE_EMPTY_PERCENTAGE;			/* ---  --- */
	BYTE	TICKETBOX_EMPTY_PERCENTAGE;					/* ---  --- */
	BYTE	TRASH_PRE_FULL_PERCENTAGE;					/* ---  --- */
	BYTE	TRASH_FULL_PERCENTAGE;							/* ---  --- */	 
}Para0305;

//HHJT_CJP 20210319  AGM参数信息 
typedef struct _Para0306
{
	BYTE						ParamVer[7];										/* ---  --- */
	unsigned int		ValidTime;				/* ---  --- */
	BYTE						MAX_PASS;												/* ---  --- */
	BYTE						RELEASE_TIME;										/* ---  --- */
	BYTE						DELAY_TIME;											/* ---  --- */
	BYTE						BLACKLIST_LIGHT_ACTION[2];			/* ---  --- */
	BYTE						BLACKLIST_SOUND_ACTION[2];			/* ---  --- */
	BYTE						ILLEGAL_LIGHT_ACTION[2];				/* ---  --- */
	BYTE						ILLEGAL_SOUND_ACTION[2];				/* ---  --- */
}Para0306;


//HHJT_CJP 20210322  TVM软件版本 
typedef struct _Para0501
{
	BYTE						ParamVer[7];			/* ---参数版本 --- */
	char						SoftVersion[12];	/* ---软件版本 --- */
	unsigned int		SoftLength;				/* ---软件长度 --- */
}Para0501;

//HHJT_CJP 20210322  AGM软件版本 
typedef struct _Para0502
{
	BYTE						ParamVer[7];			/* ---参数版本 --- */
	char						SoftVersion[12];	/* ---软件版本 --- */
	unsigned int		SoftLength;				/* ---软件长度 --- */
}Para0502;

//HHJT_CJP 20210322  BOM软件版本 
typedef struct _Para0503
{
	BYTE						ParamVer[7];			/* ---参数版本 --- */
	char						SoftVersion[12];	/* ---软件版本 --- */
	unsigned int		SoftLength;				/* ---软件长度 --- */
}Para0503;

//HHJT_CJP 20210324  TCM软件版本 
typedef struct _Para0504
{
	BYTE						ParamVer[7];			/* ---参数版本 --- */
	char						SoftVersion[12];	/* ---软件版本 --- */
	unsigned int		SoftLength;				/* ---软件长度 --- */
}Para0504;

/*----------------------------------------
------------------交易数据----------------
-----------------------------------------*/

//HHJT_CJP 20210319  单程类票卡 
typedef struct _Trans_1001_1
{
	unsigned short w_Records;		/* ---  --- */
}Trans_1001_1;
typedef struct _Trans_1001_2
{
	unsigned char LINE_ID;				/* ---  --- */
	unsigned char STATION_NID[2];			/* ---  --- */
	unsigned char DEVICE_TYPE;				/* ---  --- */
	unsigned char DEVICE_NID[4];				/* ---  --- */
	unsigned char LAST_DEVICE_NID[4];			/* ---  --- */
	unsigned char LAST_STATION_NID[2];		/* ---  --- */
	unsigned char SALE_DEVICE_NID[4];			/* ---  --- */
	unsigned char SALE_TIME[7];			/* ---  --- */
	unsigned char CARD_ID[5];				/* ---  --- */
	unsigned char CARD_PHY_ID[8];			/* ---  --- */
	unsigned int  INIT_ID;			/* ---  --- */
	unsigned char TICKET_TYPE[2];			/* ---  --- */
	unsigned char TRANS_TYPE;				/* ---  --- */
	unsigned char PAYMENT_METHOD;			/* ---  --- */
	unsigned int  TRANS_VALUE;			/* ---  --- */
	unsigned int  DISCOUNT;	/* ---  --- */
	unsigned int  VALUE_OVERAGE;	/* ---  --- */
	unsigned char TRANS_TIME[7];			/* ---  --- */
	unsigned char LAST_TRANS_TIME[7];	/* ---  --- */
	unsigned short CARD_NBR;		/* ---  --- */
	unsigned int  DEVICE_TRACE_NBR;				/* ---  --- */
	unsigned char SAM_ID[6];				/* ---  --- */
	unsigned int  SAM_NBR;			/* ---  --- */
	unsigned char  CRW_ID[2];				/* ---  --- */
	unsigned int  CRW_NBR;			/* ---  --- */
	unsigned char TAC[4];					/* ---  --- */
	unsigned char OPERATOR_ID[4];			/* ---  --- */
	unsigned char RUN_MODE;			/* ---  --- */
	unsigned char MODE_DATA;			/* ---  --- */
	unsigned char IS_TEST;				/* ---  --- */
	unsigned char REL_CARD_ID[5];				/* ---  --- */
}Trans_1001_2;

//HHJT_CJP 20210319  储值类票卡 
typedef struct _Trans_1002_1
{
	unsigned short w_Records;		/* ---  --- */
}Trans_1002_1;
typedef struct _Trans_1002_2
{
	unsigned char LINE_ID;				/* ---  --- */
	unsigned char STATION_NID[2];			/* ---  --- */
	unsigned char DEVICE_TYPE;				/* ---  --- */
	unsigned char DEVICE_NID[4];				/* ---  --- */
	unsigned char LAST_DEVICE_NID[4];			/* ---  --- */
	unsigned char LAST_STATION_NID[2];		/* ---  --- */
	unsigned char CARD_ID[5];				/* ---  --- */
	unsigned char CARD_PHY_ID[8];			/* ---  --- */
	unsigned char CARD_VER;				/* ---  --- */
	unsigned char ALGORITHM_FLAG;					/* ---  --- */
	unsigned char TICKET_TYPE[2];			/* ---  --- */
	unsigned char TRANS_TYPE;				/* ---  --- */
	unsigned char PAYMENT_METHOD;			/* ---  --- */
	unsigned char VALUE_TYPE;				/* ---  --- */
	unsigned int  TRANS_VALUE;			/* ---  --- */
	unsigned short TRANS_RIDE;			/* ---  --- */
	unsigned int  DEPOSIT;					/* ---  --- */
	BYTE          TRANS_POST_VALUE[4];		/* ---  --- */
	unsigned short TRANS_POST_RIDE;		/* ---  --- */
	unsigned int  DISCOUNT;	/* ---  --- */
	unsigned int  OVERDRAFT_LIMIT_VALUE;			/* ---  --- */
	unsigned int  CURRENT_ACC_RIDE;				/* ---  --- */
	unsigned int  SUM_ACC_RIDE;				/* ---  --- */
	unsigned int  CURRENT_ACC_VALUE;				/* ---  --- */
	unsigned int  SUM_ACC_VALUE;				/* ---  --- */
	unsigned int  TRANS_FEE;				/* ---  --- */
	unsigned char VALID_DATE[4];				/* ---  --- */
	unsigned char TRANS_TIME[7];			/* ---  --- */
	unsigned char LAST_TRANS_TIME[7];	/* ---  --- */
	unsigned char ACC_START_TIME[7];			/* ---  --- */
	unsigned short LAST_CARD_NBR;	/* ---  --- */
	unsigned short CARD_NBR;		/* ---  --- */
	unsigned int  DEVICE_TRACE_NBR;				/* ---  --- */
	unsigned char SAM_ID[6];				/* ---  --- */
	unsigned int  SAM_NBR;			/* ---  --- */
	unsigned char CRW_ID[2];				/* ---  --- */
	unsigned int  CRW_NBR;				/* ---  --- */
	unsigned char TAC[4];					/* ---  --- */
	unsigned char OPERATOR_ID[4];			/* ---  --- */
	unsigned char RUN_MODE;			/* ---  --- */
	unsigned char RELE_CARD_NO[5];				/* ---  --- */
	unsigned char RELE_APPLY_NO[12];			/* ---  --- */
	unsigned char MODE_DATA;			/* ---  --- */
	unsigned char IS_TEST;				/* ---  --- */
}Trans_1002_2;

//HHJT_CJP 20210319  一卡通交易
typedef struct _Trans_1003_1
{
	unsigned short w_Records;		/* ---  --- */
}Trans_1003_1;
typedef struct _Trans_1003_2
{
	unsigned char LINE_ID;				/* ---  --- */
	unsigned char STATION_NID[2];			/* ---  --- */
	unsigned char DEVICE_TYPE;				/* ---  --- */
	unsigned char DEVICE_NID[4];				/* ---  --- */
	unsigned char LAST_DEVICE_NID[4];			/* ---  --- */
	unsigned char LAST_STATION_NID[2];		/* ---  --- */
	unsigned char CARD_ID[5];			/* ---  --- */
	unsigned char CARD_PHY_ID[8];			/* ---  --- */
	unsigned char ALGORITHM_FLAG;				/* ---  --- */
	unsigned char TICKET_TYPE[2];			/* ---  --- */
	unsigned char TRANS_TYPE;				/* ---  --- */
	unsigned char PAYMENT_METHOD;			/* ---  --- */
	unsigned int  TRANS_VALUE;			/* ---  --- */
	unsigned int  DEPOSIT;	/* ---  --- */
	BYTE          TRANS_POST_VALUE[4];		/* ---  --- */
	unsigned int  DISCOUNT;	/* ---  --- */
	unsigned int  TRANS_FEE;	/* ---  --- */
	unsigned char TRANS_TIME[7];			/* ---  --- */
	unsigned char RELE_CARD_NO[5];			/* ---  --- */
	unsigned char LAST_TRANS_TIME[7];	/* ---  --- */
	unsigned short LAST_CARD_NBR;		/* ---  --- */
	unsigned short CARD_NBR;		/* ---  --- */
	unsigned int  DEVICE_TRACE_NBR;				/* ---  --- */
	unsigned char SAM_ID[6];				/* ---  --- */
	unsigned int  SAM_NBR;			/* ---  --- */
	unsigned char  CRW_ID[2];				/* ---  --- */
	unsigned int  CRW_NBR;			/* ---  --- */
	unsigned char TAC[4];					/* ---  --- */
	unsigned char OPERATOR_ID[4];			/* ---  --- */
	unsigned char RUN_MODE;			/* ---  --- */
	unsigned char MODE_DATA;			/* ---  --- */
	unsigned char IS_TEST;				/* ---  --- */
}Trans_1003_2;

//HHJT_CJP 20210319 银联PAY交易数据 
typedef struct _Trans_1004_1
{
	unsigned short w_Records;		/* ---  --- */
}Trans_1004_1;
typedef struct _Trans_1004_2
{
	unsigned char DEVICE_TYPE;				/* ---  --- */
	unsigned char TICKET_TYPE[2];			/* ---  --- */
	unsigned char TRANS_TYPE;				/* ---  --- */
	unsigned char TRANS_TIME[7];			/* ---  --- */
	unsigned char CHARGING_METHOD;			/* ---  --- */
	unsigned char PAYMENT_METHOD;			/* ---  --- */
	unsigned char LAST_DEVICE_NID[4];			/* ---  --- */
	unsigned char LAST_STATION_NID[2];		/* ---  --- */
	unsigned char LAST_TRANS_TIME[7];	/* ---  --- */
	unsigned char SAM_ID[6];				/* ---  --- */
	unsigned int  SAM_NBR;			/* ---  --- */
	unsigned char CRW_ID[2];				/* ---  --- */
	unsigned int  CRW_NBR;			/* ---  --- */
	unsigned char OPERATOR_ID[4];			/* ---  --- */
	unsigned char RUN_MODE;			/* ---  --- */
	unsigned char MODE_DATA;			/* ---  --- */
	unsigned char DEVICE_TRACE_NBR[8];				/* ---  --- */
	unsigned char BUSINESS_ID;			/* ---  --- */
	unsigned char RFU1;			/* ---  --- */
	unsigned char LINE_ID;				/* ---  --- */
	unsigned char STATION_NID[2];			/* ---  --- */
	unsigned char ENTRY_EXIT_FLAG;			/* ---  --- */
	unsigned char DEVICE_NID[4];				/* ---  --- */
	unsigned char ACCOUNT_NO[10];			/* ---  --- */
	unsigned int  TRANS_VALUE;			/* ---  --- */
	unsigned char VALID_DATE[2];				/* ---  --- */
	unsigned char ACCOUNT_NBR[2];			/* ---  --- */
	unsigned char ODA35_LEN;			/* ---  --- */
	unsigned char ODA35[20];			/* ---  --- */
	unsigned char ODA55_LEN;			/* ---  --- */
	unsigned char ODA55[124];			/* ---  --- */
	unsigned char AUTHENTICATION_FLAG;
	unsigned char PLATFORM_TRACE_NBR[9];
	unsigned char RFU2[20];			/* ---  --- */
}Trans_1004_2;

//HHJT_CJP 20210319 二维码交易数据
typedef struct _Trans_1005_1
{
	unsigned short w_Records;		/* ---  --- */
}Trans_1005_1;
typedef struct _Trans_1005_2
{
	unsigned char DEVICE_TYPE;				/* ---  --- */
	unsigned char TICKET_TYPE[2];			/* ---  --- */
	unsigned char TRANS_TYPE;				/* ---  --- */
	unsigned char TRANS_TIME[7];			/* ---  --- */
	unsigned char CHARGING_METHOD;			/* ---  --- */
	unsigned char PAYMENT_METHOD;			/* ---  --- */
	unsigned int  TRANS_VALUE;			/* ---  --- */
	unsigned char LAST_DEVICE_NID[4];			/* ---  --- */
	unsigned char LAST_STATION_NID[2];		/* ---  --- */
	unsigned char LAST_TRANS_TIME[7];	/* ---  --- */
	unsigned char SAM_ID[6];				/* ---  --- */
	unsigned int  SAM_NBR;			/* ---  --- */
	unsigned char CRW_ID[2];				/* ---  --- */
	unsigned int  CRW_NBR;			/* ---  --- */
	unsigned char OPERATOR_ID[4];			/* ---  --- */
	unsigned char RUN_MODE;			/* ---  --- */
	unsigned char MODE_DATA;			/* ---  --- */
	unsigned char QR_VER;
	unsigned char QR_TYPE;
	unsigned char KID_1[2];
	unsigned char KID_2[2];
	unsigned char MECHANISM_ID[4];
	char          QR_VALID_DATE[7];
	unsigned char INDUSTRY_RANGE[2];
	unsigned char ENTRY_EXIT_LIMIT;
	unsigned char STATION_LIMIT[8];
	unsigned char PLATFORM_SIGNATURE[4];
	unsigned char USER_SIGNATURE[4];
	unsigned char RFU1[20];	
	unsigned char DEVICE_TRACE_NBR[8];				/* ---  --- */
	unsigned char QR_ID[8];				/* ---  --- */
	unsigned char JOURNEY_ID[8];				/* ---  --- */
	unsigned char APP_ID[8];				/* ---  --- */
	unsigned char BUSINESS_ID;			/* ---  --- */
	unsigned char USER_ID[8];			/* ---  --- */
	unsigned char USER_TYPE;			/* ---  --- */
	char          QR_CREATE_DATE[7];
	unsigned char RFU2;	
	unsigned char LINE_ID;				/* ---  --- */
	unsigned char STATION_NID[2];			/* ---  --- */
	unsigned char ENTRY_EXIT_FLAG;			/* ---  --- */
	unsigned char DEVICE_NID[4];				/* ---  --- */
	unsigned char RFU3[302];	
}Trans_1005_2;


//HHJT_CJP 20211221 交通部二维码交易数据
typedef struct _Trans_1006_1
{
	unsigned short w_Records;		/* ---  --- */
}Trans_1006_1;
typedef struct _Trans_1006_2
{
	unsigned char DEVICE_TYPE;				/* ---  --- */
	unsigned char TICKET_TYPE[2];			/* ---  --- */
	unsigned char TRANS_TYPE;				/* ---  --- */
	unsigned char TRANS_TIME[7];			/* ---  --- */
	unsigned char CHARGING_METHOD;			/* ---  --- */
	unsigned char PAYMENT_METHOD;			/* ---  --- */
	unsigned int  TRANS_VALUE;			/* ---  --- */
	unsigned char LAST_DEVICE_NID[4];			/* ---  --- */
	unsigned char LAST_STATION_NID[2];		/* ---  --- */
	unsigned char LAST_TRANS_TIME[7];	/* ---  --- */
	unsigned char SAM_ID[6];				/* ---  --- */
	unsigned int  SAM_NBR;			/* ---  --- */
	unsigned char CRW_ID[2];				/* ---  --- */
	unsigned int  CRW_NBR;			/* ---  --- */
	unsigned char OPERATOR_ID[4];			/* ---  --- */
	unsigned char RUN_MODE;			/* ---  --- */
	unsigned char MODE_DATA;			/* ---  --- */
	unsigned char QR_VER;
	unsigned char USER_ID[10];			/* ---  --- */
	unsigned char USER_TYPE;			/* ---  --- */
	unsigned int  TRANS_VALUE_TOPLIMIT;
	unsigned int  AUTHO_INVALID_TIME;
	char          QR_VALID_DATE[7];
	unsigned char CARD_NO[8];
	unsigned char CARD_TYPE[2];	
	unsigned int  VALID_TIME;
	unsigned int  INVALID_TIME;
	unsigned char CHANNEL_NO;
	unsigned char METRO_CARD_TYPE[2];	
	unsigned char RFU_1[20];	
	unsigned char DEVICE_TRACE_NBR[8];				/* ---  --- */
	unsigned char QR_ID[8];				/* ---  --- */
	unsigned char JOURNEY_ID[8];				/* ---  --- */
	unsigned char MECHANISM_ID[4];
	unsigned char PLATFORM_ID[4];
	unsigned char BUSINESS_ID;			/* ---  --- */
	unsigned char USER_ID_2[8];			/* ---  --- */
	unsigned char USER_TYPE_2;			/* ---  --- */
	char          QR_CREATE_DATE[7];
	unsigned char METRO_CARD_TYPE_2;			/* ---  --- */
	unsigned char LINE_ID;				/* ---  --- */
	unsigned char STATION_NID[2];			/* ---  --- */
	unsigned char ENTRY_EXIT_FLAG;			/* ---  --- */
	unsigned char DEVICE_NID[4];				/* ---  --- */
	unsigned char RFU_2[502];	
}Trans_1006_2;

/*----------------------------------------
------------------对账数据----------------
-----------------------------------------*/

//HHJT_CJP 20210319 清分异议数据
typedef struct _Trans_2001_1
{
	unsigned short 	w_Records;	
}Trans_2001_1;
typedef struct _Trans_2001_2
{
	unsigned char 	CENTER_NID;
  BYTE          	SETTLE_DATE[4];
  char            OBJECTION_MEMO[200];
}Trans_2001_2;

//HHJT_CJP 20210319 一票通异常交易数据明细
typedef struct _Trans_2002_1
{
	BYTE 						SETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned short 	w_Records;	
}Trans_2002_1;
typedef struct _Trans_2002_2
{
	unsigned char 	FILE_ID[12];
  unsigned char 	CARD_ID[5];
  unsigned char 	CARD_PHY_ID[8];
  BYTE          	TRANS_TIME[7];
  unsigned short 	CARD_NBR;
  unsigned char   DEVICE_NID[4];
	unsigned int 		DEVICE_TRACE_NBR;
	unsigned char 	CHECK_RESULT;
	unsigned char 	DATA_TYPE;
}Trans_2002_2;

//HHJT_CJP 20210319 一票通异常重分配数据明细
typedef struct _Trans_2003_1
{
	BYTE 						SETTLE_DATE[4];
	BYTE            RESETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned short 	w_Records;	
}Trans_2003_1;
typedef struct _Trans_2003_2
{
	unsigned char 	FILE_ID[12];
  unsigned char 	CARD_ID[5];
  unsigned char 	CARD_PHY_ID[8];
  BYTE          	TRANS_TIME[7];
  unsigned short 	CARD_NBR;
  unsigned char   DEVICE_NID[4];
	unsigned int 		DEVICE_TRACE_NBR;
}Trans_2003_2;

//HHJT_CJP 20210319 一票通清分日结数据
typedef struct _Trans_2004_1
{
	BYTE 						SETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned short 	w_Records;	
	
}Trans_2004_1;
typedef struct _Trans_2004_2
{
	unsigned int 		TRANS_COUNT;
	unsigned int 		NORMAL_COUNT;
	unsigned int 		NORMAL_AMOUNT;
	unsigned int 		DOUBT_COUNT;
	unsigned int 		DOUBT_AMOUNT;
	unsigned int 		RESETTLE_COUNT;
	unsigned int 		RESETTLE_AMOUNT;
	unsigned int 		DELAY_SETTLE_COUNT;
	unsigned int 		DELAY_SETTLE_AMOUNT;
	unsigned int 		FEE;
	unsigned int 		ADJUST_AMOUNT;
	char				 		ADJUST_REASON[100];
	BYTE        		ADJUST_TIME[7];
}Trans_2004_2;

//HHJT_CJP 20210319 一票通清分对账数据
typedef struct _Trans_2005_1
{
	BYTE 						SETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned short 	w_Records;	
	
}Trans_2005_1;
typedef struct _Trans_2005_2
{
	unsigned char 	TICKET_TYPE[2];
	unsigned char 	TRANS_TYPE;
	unsigned int 		UPLOAD_COUNT_TODAY;
	unsigned int 		UPLOAD_AMOUNT_TODAY;
	unsigned int 		UPLOAD_SETTLE_COUNT_TODAY;
	unsigned int 		UPLOAD_SETTLE_AMOUNT_TODAY;
	unsigned int 		DOUBT_COUNT_TODAY;
	unsigned int 		DOUBT_AMOUNT_TODAY;
	unsigned int 		UNI_COUNT_TODAY;
	unsigned int 		UNI_AMOUNT_TODAY;
	unsigned int 		DELAY_COUNT_TODAY;
	unsigned int 		DALAY_AMOUNT_TODAY;
	unsigned int 		DOUBT_RESETTLE_COUNT_YES;
	unsigned int 		DOUBT_RESETTLE_AMOUNT_YES;
	unsigned int 		UNI_RESETTLE_COUNT_YES;
	unsigned int 		UNI_RESETTLE_AMOUNT_YES;
	unsigned int 		DELAY_RESETTLE_COUNT_YES;
	unsigned int 		DELAY_RESETTLE_AMOUNT_YES;
	unsigned int 		TRANS_AMOUNT_TODAY;
}Trans_2005_2;

//HHJT_CJP 20210319 设备交易记录缺失数据
typedef struct _Trans_2006_1
{
	BYTE 						SETTLE_DATE[4];
  unsigned char 	CENTER_NID;
	unsigned short 	w_Records;	
}Trans_2006_1;
typedef struct _Trans_2006_2
{
	unsigned char 	LINE_ID;
	unsigned char 	STATION_NID[2];
	unsigned char 	DEVICE_TYPE;
  unsigned char   DEVICE_NID[4];
	unsigned char 	TICKET_CLASS;
	unsigned int 		START_TRACE_NBR;
	unsigned int 		END_TRACE_NBR;
}Trans_2006_2;

//HHJT_CJP 20210319 一票通数据包清分明细数据
typedef struct _Trans_2007_1
{
  unsigned char 	CENTER_NID;
	unsigned short 	w_Records;	
	
}Trans_2007_1;
typedef struct _Trans_2007_2
{
	unsigned char 	FILE_ID[12];
	unsigned char 	UPLOAD_TIME[7];
	unsigned char 	MEASURE_TIME[7];
	unsigned char 	SETTLE_TIME[7];
	unsigned char 	SETTLE_DATE[4];
	unsigned char 	FILE_TYPE;
	unsigned char 	FILE_STATUS;
	unsigned int 		ROW_NUM;
	unsigned int 		MEASURE_ROW_NUM;
	unsigned int 		NORMAL_ROW_NUM;
	unsigned int 		DOUBT_ROW_NUM;
	unsigned int 		TEST_NUM;
	unsigned int 		SETTLE_ERROR_ROW_NUM;
}Trans_2007_2;

//HHJT_CJP 20210319 一卡通清分日结数据
typedef struct _Trans_2008_1
{
	BYTE 						SETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned short 	w_Records;	
	
}Trans_2008_1;
typedef struct _Trans_2008_2
{
	unsigned int 		TRANS_COUNT;
	unsigned int 		NORMAL_COUNT;
	unsigned int 		NORMAL_AMOUNT;
	unsigned int 		DOUBT_COUNT;
	unsigned int 		DOUBT_AMOUNT;
	unsigned int 		RESETTLE_COUNT;
	unsigned int 		RESETTLE_AMOUNT;
	unsigned int 		DELAY_SETTLE_COUNT;
	unsigned int 		DELAY_SETTLE_AMOUNT;
	unsigned int 		FEE;
	unsigned int 		ADJUST_AMOUNT;
	char				 		ADJUST_REASON[100];
	BYTE        		ADJUST_TIME[7];
}Trans_2008_2;

//HHJT_CJP 20210319 一卡通异常交易数据明细
typedef struct _Trans_2009_1
{
	BYTE 						SETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned short 	w_Records;	
}Trans_2009_1;
typedef struct _Trans_2009_2
{
	unsigned char 	FILE_ID[12];
  unsigned char 	CARD_ID[5];
  unsigned char 	CARD_PHY_ID[8];
  BYTE          	TRANS_TIME[7];
  unsigned short 	CARD_NBR;
  unsigned char   DEVICE_NID[4];
	unsigned int 		DEVICE_TRACE_NBR;
	unsigned char 	CHECK_RESULT;
	unsigned char 	DATA_TYPE;
}Trans_2009_2;

//HHJT_CJP 20210319 一卡通异常重分配数据明细
typedef struct _Trans_200A_1
{
	BYTE 						SETTLE_DATE[4];
	BYTE            RESETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned short 	w_Records;	
	
}Trans_200A_1;
typedef struct _Trans_200A_2
{
	unsigned char 	FILE_ID[12];
  unsigned char 	CARD_ID[5];
  unsigned char 	CARD_PHY_ID[8];
  BYTE          	TRANS_TIME[7];
  unsigned short 	CARD_NBR;
  unsigned char   DEVICE_NID[4];
	unsigned int 		DEVICE_TRACE_NBR;
}Trans_200A_2;

//HHJT_CJP 20210319 一卡通清分对账数据
typedef struct _Trans_200B_1
{
	BYTE 						SETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned short 	w_Records;	
	
}Trans_200B_1;
typedef struct _Trans_200B_2
{
	unsigned char 	TICKET_TYPE[2];
	unsigned char 	TRANS_TYPE;
	unsigned int 		UPLOAD_COUNT_TODAY;
	unsigned int 		UPLOAD_AMOUNT_TODAY;
	unsigned int 		UPLOAD_SETTLE_COUNT_TODAY;
	unsigned int 		UPLOAD_SETTLE_AMOUNT_TODAY;
	unsigned int 		DOUBT_COUNT_TODAY;
	unsigned int 		DOUBT_AMOUNT_TODAY;
	unsigned int 		UNI_COUNT_TODAY;
	unsigned int 		UNI_AMOUNT_TODAY;
	unsigned int 		DELAY_COUNT_TODAY;
	unsigned int 		DALAY_AMOUNT_TODAY;
	unsigned int 		DOUBT_RESETTLE_COUNT_YES;
	unsigned int 		DOUBT_RESETTLE_AMOUNT_YES;
	unsigned int 		UNI_RESETTLE_COUNT_YES;
	unsigned int 		UNI_RESETTLE_AMOUNT_YES;
	unsigned int 		DELAY_RESETTLE_COUNT_YES;
	unsigned int 		DELAY_RESETTLE_AMOUNT_YES;
	BYTE            PERL_SETTLE_DATE[4];
	unsigned int 		TRANS_AMOUNT_TODAY;
}Trans_200B_2;

//HHJT_CJP 20210319 一卡通数据包清分明细数据
typedef struct _Trans_200C_1
{
  unsigned char 	CENTER_NID;
	unsigned short 	w_Records;	
	
}Trans_200C_1;
typedef struct _Trans_200C_2
{
	unsigned char 	FILE_ID[12];
	unsigned char 	UPLOAD_TIME[7];
	unsigned char 	MEASURE_TIME[7];
	unsigned char 	SETTLE_TIME[7];
	unsigned char 	SETTLE_DATE[4];
	unsigned char 	FILE_TYPE;
	unsigned char 	FILE_STATUS;
	unsigned int 		ROW_NUM;
	unsigned int 		MEASURE_ROW_NUM;
	unsigned int 		NORMAL_ROW_NUM;
	unsigned int 		DOUBT_ROW_NUM;
	unsigned int 		TEST_NUM;
	unsigned int 		SETTLE_ERROR_ROW_NUM;
}Trans_200C_2;

//HHJT_CJP 20210319 银联PAY清分日结数据
typedef struct _Trans_2010
{
	BYTE 						SETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned int 		TRANS_COUNT;
	unsigned int 		NORMAL_COUNT;
	unsigned int 		NORMAL_ACTUAL_AMOUNT;
	unsigned int 		NORMAL_SHOULD_AMOUNT;
	unsigned int 		NORMAL_DISCOUNT_AMOUNT;
	unsigned int 		DOUBT_COUNT;
	unsigned int 		DOUBT_AMOUNT;
	unsigned int 		RESETTLE_COUNT;
	unsigned int 		RESETTLE_AMOUNT;
	unsigned int 		DELAY_SETTLE_COUNT;
	unsigned int 		DELAY_SETTLE_AMOUNT;
	unsigned int 		FEE;
	unsigned int 		ADJUST_AMOUNT;
	char				 		ADJUST_REASON[100];
	BYTE        		ADJUST_TIME[7];
}Trans_2010;


//HHJT_CJP 20210319 银联PAY异常交易数据明细
typedef struct _Trans_2011_1
{
	BYTE 						SETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned int   	dw_Records;	
}Trans_2011_1;
typedef struct _Trans_2011_2
{
	unsigned char 	FILE_ID[12];
  unsigned char 	CARD_ID[10];
  BYTE          	TRANS_TIME[7];
  unsigned char   DEVICE_NID[4];
	unsigned int 		DEVICE_TRACE_NBR;
	unsigned char 	CHECK_RESULT;
	unsigned char 	DATA_TYPE;
}Trans_2011_2;

//HHJT_CJP 20210319 银联PAY异常重分配数据明细
typedef struct _Trans_2012_1
{
	BYTE 						SETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned int  	dw_Records;	
}Trans_2012_1;
typedef struct _Trans_2012_2
{
	BYTE 						RESETTLE_DATE[4];
	unsigned char 	FILE_ID[12];
  unsigned char 	CARD_ID[10];
  BYTE          	TRANS_TIME[7];
  unsigned char   DEVICE_NID[4];
	unsigned int 		DEVICE_TRACE_NBR;
	unsigned char 	RESETTLE_REASON;
}Trans_2012_2;

//HHJT_CJP 20210319 银联PAY清分对账数据
typedef struct _Trans_2013_1
{
	BYTE 						SETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned short  w_Records;	
}Trans_2013_1;
typedef struct _Trans_2013_2
{
	unsigned char 	TICKET_TYPE[2];
	unsigned char 	TRANS_TYPE;
	unsigned int 		UPLOAD_COUNT_TODAY;
	unsigned int 		UPLOAD_AMOUNT_TODAY;
	unsigned int 		UPLOAD_SETTLE_COUNT_TODAY;
	unsigned int 		UPLOAD_SETTLE_AMOUNT_TODAY;
	unsigned int 		DOUBT_COUNT_TODAY;
	unsigned int 		DOUBT_AMOUNT_TODAY;
	unsigned int 		UNI_COUNT_TODAY;
	unsigned int 		UNI_AMOUNT_TODAY;
	unsigned int 		DELAY_COUNT_TODAY;
	unsigned int 		DALAY_AMOUNT_TODAY;
	unsigned int 		DOUBT_RESETTLE_COUNT_YES;
	unsigned int 		DOUBT_RESETTLE_AMOUNT_YES;
	unsigned int 		UNI_RESETTLE_COUNT_YES;
	unsigned int 		UNI_RESETTLE_AMOUNT_YES;
	unsigned int 		DELAY_RESETTLE_COUNT_YES;
	unsigned int 		DELAY_RESETTLE_AMOUNT_YES;
	BYTE 						UNION_PAY_SETTLE_DATE[4];
	unsigned int 		TRANS_AMOUNT_TODAY;
}Trans_2013_2;

//HHJT_CJP 20210319 银联PAY数据包清分明细数据
typedef struct _Trans_2014_1
{
  unsigned char 	CENTER_NID;
	unsigned short 	w_Records;	
}Trans_2014_1;
typedef struct _Trans_2014_2
{
	unsigned char 	FILE_ID[12];
	unsigned char 	UPLOAD_TIME[7];
	unsigned char 	MEASURE_TIME[7];
	unsigned char 	SETTLE_TIME[7];
	unsigned char 	SETTLE_DATE[4];
	unsigned char 	FILE_TYPE;
	unsigned char 	FILE_STATUS;
	unsigned int 		ROW_NUM;
	unsigned int 		MEASURE_ROW_NUM;
	unsigned int 		NORMAL_ROW_NUM;
	unsigned int 		DOUBT_ROW_NUM;
	unsigned int 		TEST_NUM;
	unsigned int 		SETTLE_ERROR_ROW_NUM;
}Trans_2014_2;

//HHJT_CJP 20210319 二维码清分日结数据
typedef struct _Trans_2015
{
	BYTE 						SETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned int 		TRANS_COUNT;
	unsigned int 		NORMAL_COUNT;
	unsigned int 		NORMAL_ACTUAL_AMOUNT;
	unsigned int 		NORMAL_SHOULD_AMOUNT;
	unsigned int 		NORMAL_DISCOUNT_AMOUNT;
	unsigned int 		DOUBT_COUNT;
	unsigned int 		DOUBT_AMOUNT;
	unsigned int 		RESETTLE_COUNT;
	unsigned int 		RESETTLE_AMOUNT;
	unsigned int 		DELAY_SETTLE_COUNT;
	unsigned int 		DELAY_SETTLE_AMOUNT;
	unsigned int 		FEE;
	unsigned int 		ADJUST_AMOUNT;
	char				 		ADJUST_REASON[100];
	BYTE        		ADJUST_TIME[7];
}Trans_2015;

//HHJT_CJP 20210319 二维码异常交易数据明细
typedef struct _Trans_2016_1
{
	BYTE 						SETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned int   	dw_Records;	
}Trans_2016_1;
typedef struct _Trans_2016_2
{
	unsigned char 	FILE_ID[12];
	unsigned char   QR_VER;
	unsigned char   QR_TYPE;
	unsigned char   KID_1[2];
	unsigned char   KID_2[2];
	unsigned char   QR_ID[8];
	unsigned char   APP_ID[8];
	unsigned char   MECHANISM_ID[4];
	unsigned char   USER_ID[8];
	char            QR_CREATE_DATE[7];
	char            QR_VALID_DATE[7];
	unsigned char   MECHANISM_SIGNATURE[4];
	unsigned char   APP_SIGNATURE[4];
  BYTE          	TRANS_TIME[7];
  unsigned char   DEVICE_NID[4];
	unsigned int 		DEVICE_TRACE_NBR;
	unsigned char 	CHECK_RESULT;
	unsigned char 	DATA_TYPE;
}Trans_2016_2;

//HHJT_CJP 20210319 二维码异常重分配数据明细
typedef struct _Trans_2017_1
{
	BYTE 						SETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned int   	dw_Records;	
}Trans_2017_1;
typedef struct _Trans_2017_2
{
	BYTE 						RESETTLE_DATE[4];
	unsigned char 	FILE_ID[12];
	unsigned char   QR_VER;
	unsigned char   QR_TYPE;
	unsigned char   KID_1[2];
	unsigned char   KID_2[2];
	unsigned char   QR_ID[8];
	unsigned char   APP_ID[8];
	unsigned char   MECHANISM_ID[4];
	unsigned char   USER_ID[8];
	char            QR_CREATE_DATE[7];
	char            QR_VALID_DATE[7];
	unsigned char   MECHANISM_SIGNATURE[4];
	unsigned char   APP_SIGNATURE[4];
  BYTE          	TRANS_TIME[7];
  unsigned char   DEVICE_NID[4];
	unsigned int 		DEVICE_TRACE_NBR;
	unsigned char 	REASON;
}Trans_2017_2;

//HHJT_CJP 20210319 二维码清分对账数据
typedef struct _Trans_2018_1
{
	BYTE 						SETTLE_DATE[4];
	unsigned char 	SETTLE_TYPE;
  unsigned char 	CENTER_NID;
	unsigned short  w_Records;	
}Trans_2018_1;
typedef struct _Trans_2018_2
{
	unsigned char 	TICKET_TYPE[2];
	unsigned char 	TRANS_TYPE;
	unsigned int 		UPLOAD_COUNT_TODAY;
	unsigned int 		UPLOAD_AMOUNT_TODAY;
	unsigned int 		UPLOAD_SETTLE_COUNT_TODAY;
	unsigned int 		UPLOAD_SETTLE_AMOUNT_TODAY;
	unsigned int 		DOUBT_COUNT_TODAY;
	unsigned int 		DOUBT_AMOUNT_TODAY;
	unsigned int 		UNI_COUNT_TODAY;
	unsigned int 		UNI_AMOUNT_TODAY;
	unsigned int 		DELAY_COUNT_TODAY;
	unsigned int 		DALAY_AMOUNT_TODAY;
	unsigned int 		DOUBT_RESETTLE_COUNT_YES;
	unsigned int 		DOUBT_RESETTLE_AMOUNT_YES;
	unsigned int 		UNI_RESETTLE_COUNT_YES;
	unsigned int 		UNI_RESETTLE_AMOUNT_YES;
	unsigned int 		DELAY_RESETTLE_COUNT_YES;
	unsigned int 		DELAY_RESETTLE_AMOUNT_YES;
	BYTE 						QR_PAY_SETTLE_DATE[4];
	unsigned int 		TRANS_AMOUNT_TODAY;
}Trans_2018_2;

//HHJT_CJP 20210319 二维码数据包清分明细数据
typedef struct _Trans_2019_1
{
  unsigned char 	CENTER_NID;
	unsigned short 	w_Records;	
}Trans_2019_1;
typedef struct _Trans_2019_2
{
	unsigned char 	FILE_ID[12];
	unsigned char 	UPLOAD_TIME[7];
	unsigned char 	MEASURE_TIME[7];
	unsigned char 	SETTLE_TIME[7];
	unsigned char 	SETTLE_DATE[4];
	unsigned char 	FILE_TYPE;
	unsigned char 	FILE_STATUS;
	unsigned int 		ROW_NUM;
	unsigned int 		MEASURE_ROW_NUM;
	unsigned int 		NORMAL_ROW_NUM;
	unsigned int 		DOUBT_ROW_NUM;
	unsigned int 		TEST_NUM;
	unsigned int 		SETTLE_ERROR_ROW_NUM;
}Trans_2019_2;

/*----------------------------------------
------------------票务数据----------------
-----------------------------------------*/
//HHJT_CJP 20210319 车票库存调配请求信息
typedef struct _Trans_3001_1
{
	BYTE            APPLY_NO[12];
	BYTE            APPLY_TYPE;
	BYTE            APPLY_NID[2];
	char            APPLY_DATE[7];
	unsigned short 	w_Records;	
}Trans_3001_1;
typedef struct _Trans_3001_2
{
	unsigned char 	TICKET_DIRECTORY[2];
  BYTE          	TICKET_STATUS;
	unsigned int 		APPLY_NUM;
	char            APPLY_DEPLOY_DATE[7];
}Trans_3001_2;

typedef struct _Trans_3001_3
{
	BYTE            APPLY_USER_ID[4];
  char            MEMO[200];
}Trans_3001_3;

//HHJT_CJP 20210319 车票库存调配结果通知
typedef struct _Trans_3002_1
{
	BYTE            DEPLOY_NO[12];
	unsigned short 	w_Records;	
}Trans_3002_1;
typedef struct _Trans_3002_2
{
  BYTE          	TICKET_DIRECTORY[2];
	unsigned char   TICKET_STATUS;
	char            DEPLOY_DATE[7];
	char            APPLY_DEPLOY_DATE[7];
	BYTE            APPLY_NO[12];
	char            APPLY_DATE[7];
	unsigned int 		DEPLOY_NUM;
	BYTE            IN_NID[2];
	BYTE            DEPLOY_IN_TYPE;
	BYTE            OUT_NID[2];
	BYTE            DEPLOY_OUT_TYPE;
	unsigned int 		AMOUNT;
	unsigned int 		UNIT_PRICE;
}Trans_3002_2;

//HHJT_CJP 20210319 车票库存盘点数据
typedef struct _Trans_3003_1
{
	BYTE            CENTER_ID[2];
	char            INVENTORY_DATE[7];
	unsigned short 	w_Records;	
}Trans_3003_1;
typedef struct _Trans_3003_2
{
  BYTE          	TICKET_DIRECTORY[2];
	unsigned int 		GOOD_COUNT;
	unsigned int 		BAD_COUNT;
	unsigned int 		EXPIRATION_COUNT;
	unsigned int 		INUSE_COUNT;
	unsigned int 		PROFIT_COUNT;
	unsigned int 		LOSS_COUNT;
}Trans_3003_2;

//HHJT_CJP 20210319 票务中心车票入库记录
typedef struct _Trans_3005_1
{
	BYTE            IN_NO[12];
	BYTE            CENTER_ID[2];
	unsigned short 	w_Records;	
}Trans_3005_1;
typedef struct _Trans_3005_2
{
	unsigned char 	IN_TYPE;
  BYTE          	TICKET_DIRECTORY[2];
	unsigned char   TICKET_STATUS;
	char            IN_DATE[7];
	unsigned int 		IN_COUNT;
	BYTE            RELE_NO[12];
	unsigned int 		AMOUNT;
	unsigned int 		UNIT_PRICE;
	BYTE            OPERATOR_ID[4];
	BYTE            APPROVAL_ID[4];
	BYTE            START_CARD_ID[5];
	BYTE            END_CARD_ID[5];
	BYTE            DEVICE_NID[4];
}Trans_3005_2;

//HHJT_CJP 20210319 票务中心车票出库记录
typedef struct _Trans_3006_1
{
	BYTE            OUT_NO[12];
	BYTE            CENTER_ID[2];
	unsigned short 	w_Records;	
}Trans_3006_1;
typedef struct _Trans_3006_2
{
	unsigned char 	OUT_TYPE;
  BYTE          	TICKET_DIRECTORY[2];
	unsigned char   TICKET_STATUS;
	char            OUT_DATE[7];
	unsigned int 		OUT_COUNT;
	BYTE            RELE_NO[12];
	BYTE            OUT_USER_ID[4];
	unsigned int 		AMOUNT;
	unsigned int 		UNIT_PRICE;
	BYTE            OPERATOR_ID[4];
	BYTE            APPROVAL_ID[4];
	BYTE            START_CARD_ID[5];
	BYTE            END_CARD_ID[5];
	BYTE            DEVICE_NID[4];
}Trans_3006_2;

//HHJT_CJP 20210319 个性化卡申请
typedef struct _Trans_3007_1
{
	unsigned short 	w_Records;	
}Trans_3007_1;
typedef struct _Trans_3007_2
{
  BYTE          	APPLY_ID[12];
	BYTE            CENTER_ID;
	BYTE            LINE_ID;
	BYTE            STATION_NID[2];
	BYTE            DEVICE_NID[4];
	BYTE            TICKET_TYPE[2];
	BYTE         		APPLY_DATE[7];
	BYTE         		VALID_DATE[4];
	BYTE         		OPERATOR_ID[4];
	char            APPLY_NAME[30];
	BYTE            GENDER;
	BYTE         		BIRTH_DATE[4];
	BYTE            CERT_TYPE;
	char            CERT_NO[30];
	char            TELPHONE[20];
	char            TELPHONE2[20];
	char            ADDRESS[50];
	char            MEMO[100];
	unsigned int 		PHOTO_LENGTH;
}Trans_3007_2;

//HHJT_CJP 20210319 非即时退换卡申请
typedef struct _Trans_3008_1
{
	unsigned short 	w_Records;	
}Trans_3008_1;
typedef struct _Trans_3008_2
{
  BYTE          	APPLY_NO[12];
	BYTE            APPLY_TYPE;
	BYTE            LINE_ID;
	BYTE            STATION_NID[2];
	BYTE            DEVICE_NID[4];
	BYTE         		APPLY_DATE[7];
	BYTE         		OPERATOR_ID[4];
	BYTE            IDENTIFY_RESULT;
	BYTE            IS_FACE_DAMAGE;
	BYTE            IS_DATA_DAMAGE;
	BYTE            IS_NONRT;
	BYTE            TICKET_TYPE[2];
	BYTE         		ISSUE_DATE[4];
	BYTE         		PRE_CARD_ID[5];
	unsigned int    VALUE_REMAIN;
	BYTE            TEST_FLAG;
	char            MEMO[200];
}Trans_3008_2;

//HHJT_CJP 20210319 非即时退换卡鉴定结果下发
typedef struct _Trans_3009_1
{
	unsigned short 	w_Records;	
}Trans_3009_1;
typedef struct _Trans_3009_2
{
  BYTE          	APPLY_NO[12];
	BYTE         		APPLY_DATE[7];
	BYTE         		MEASURE_DATE[7];
	unsigned int    VALUE_REMAIN;
	BYTE            IDENTIFY_RESULT;
	unsigned int    REFUND_VALUE;
	unsigned int    REFUND_DEPOSIT;
	unsigned int    FEE;
	unsigned int    REASON_LENGTH;
	char            REASON[1024];
}Trans_3009_2;

/*----------------------------------------
------------------维护数据----------------
-----------------------------------------*/

//HHJT_CJP 20210319 设备变更信息
typedef struct _Trans_4001_Head
{
	WORD wRecords;				
}Trans_4001_Head;
typedef struct _Trans_4001_Record_1
{
	BYTE  DEVICE_NID[4];					
	BYTE  OPERATOR_ID[4];		
	BYTE  DEVICE_TYPE;	
	WORD  wRecords_1;	
}Trans_4001_Record_1;
typedef struct _Trans_4001_Record_2
{
	BYTE  Old_CRW_NO[2];	
	BYTE  CRW_NO[2];					
	WORD  wRecords_2;	
}Trans_4001_Record_2;
typedef struct _Trans_4001_Record_3
{
	BYTE  Old_PSAM_NO[6];	
	BYTE  PSAM_NO[6];		
}Trans_4001_Record_3;
typedef struct _Trans_4001_Record_4
{
	BYTE  CHANGE_TYPE;		
	char  CHANGE_MEMO[50];
}Trans_4001_Record_4;


//HHJT_CJP 20210319 钱箱更换信息
typedef struct _Trans_4002_Head
{
	BYTE  DEVICE_NID[4];			
	BYTE  OPERATOR_ID[4];	
	BYTE  CHANGE_TIME[7];	
	DWORD CHANGE_SEQ;		
	BYTE  OPERATE_TYPE;	
	BYTE  CASHBOX_ID[4];	
	BYTE  LOC_ID;				
	BYTE  CHANGE_STATUS;	
	DWORD Records;		
}Trans_4002_Head;
typedef struct _Trans_4002_Record
{
	BYTE  BILL_TYPE;		
	DWORD BILL_COUNT;			
}Trans_4002_Record;

//HHJT_CJP 20210319 票箱更换信息
typedef struct _Trans_4003
{
	BYTE  DEVICE_NID[4];	
	BYTE  OPERATOR_ID[4];	
	BYTE  CHANGE_TIME[7];
	DWORD CHANGE_SEQ;		
	BYTE  OPERATE_TYPE;		
	BYTE  TICKETBOX_ID[4];	
	BYTE  LOC_ID;			
	BYTE  TICKET_TYPE[2];
	DWORD TICKET_COUNT;		
	BYTE  CHANGE_STATUS;	
}Trans_4003;


/*----------------------------------------
------------------审计数据----------------
-----------------------------------------*/

//HHJT_CJP 20210319 TVM审计数据
typedef struct _Trans_5001_1
{
	unsigned short w_Records;		/* ---  --- */
}Trans_5001_1;
typedef struct _Trans_5001_2
{
	BYTE			CURRENT_TIME[7];
	BYTE			DEVICE_NID[4];
	unsigned int	CURRENT_TRANS_NBR;
	unsigned int	SJT_SALE_COUNT;
	unsigned int	SJT_SALE_AMOUNT;
	unsigned int	SVT_LOAD_COUNT;
	unsigned int	SVT_LOAD_AMOUNT;
	unsigned int	PERL_LOAD_COUNT;
	unsigned int	PERL_LOAD_AMOUNT;
	unsigned int	COIN_RECV_COUNT;
	unsigned int	COIN_RECV_AMOUNT;
	unsigned int	CASH_RECV_COUNT;
	unsigned int	CASH_RECV_AMOUNT;
	unsigned int	COIN_BOX1_RECLAIM;
	unsigned int	COIN_BOX2_RECLAIM;
	unsigned int	COIN_BOX1_CHANGE;
	unsigned int	COIN_BOX2_CHANGE;
	unsigned int	COIN_BOX1_CHANGE_CYCLE;
	unsigned int	COIN_BOX2_CHANGE_CYCLE;
	unsigned int	CASH_BOX1_RECLAIM;
	unsigned int	CASH_BOX2_RECLAIM;
	unsigned int	CASH_BOX3_RECLAIM;
	unsigned int	CASH_BOX4_RECLAIM;
	unsigned int	CASH_BOX1_CHANGE;
	unsigned int	CASH_BOX2_CHANGE;
	unsigned int	CASH_BOX3_CHANGE;
	unsigned int	CASH_BOX4_CHANGE;
	unsigned int	CASH_BOX1_CHANGE_CYCLE;
	unsigned int	CASH_BOX2_CHANGE_CYCLE;
	unsigned int	CASH_BOX3_CHANGE_CYCLE;
	unsigned int	CASH_BOX4_CHANGE_CYCLE;
}Trans_5001_2;

//HHJT_CJP 20210319 BOM审计数据
typedef struct _Trans_5002_1
{
	unsigned short w_Records;		/* ---  --- */
}Trans_5002_1;
typedef struct _Trans_5002_2
{
	BYTE			CURRENT_TIME[7];
	BYTE			DEVICE_NID[4];
	unsigned int	CURRENT_TRANS_NBR;
	BYTE            OPERATOR_ID[4];
	unsigned int	SJT_SALE_COUNT;
	unsigned int	SJT_SALE_AMOUNT;
	unsigned int	SJT_UPDATE_COUNT;
	unsigned int	SJT_UPDATE_AMOUNT;
	unsigned int	SJT_REFUND_COUNT;
	unsigned int	SJT_REFUND_AMOUNT;
	unsigned int	SVT_LOAD_COUNT;
	unsigned int	SVT_LOAD_AMOUNT;
	unsigned int	SVT_OFFSET_COUNT;
	unsigned int	SVT_OFFSET_AMOUNT;
	unsigned int	SVT_REFUND_COUNT;
	unsigned int	SVT_REFUND_AMOUNT;
	unsigned int	TICKET_MEASURE_COUNT;
	unsigned int	TICKET_MEASURE_AMOUNT;
}Trans_5002_2;

//HHJT_CJP 20210319 AGM审计数据
typedef struct _Trans_5003_1
{
	unsigned short w_Records;		/* ---  --- */
}Trans_5003_1;
typedef struct _Trans_5003_2
{
	BYTE			CURRENT_TIME[7];
	BYTE			DEVICE_NID[4];
	unsigned int	CURRENT_TRANS_NBR;
	unsigned int	IN_NUM;
	unsigned int	OUT_NUM;
	unsigned int	OUT_AMOUNT;
	unsigned int	RECYCLE_TICKET_COUNT;
	unsigned int	INVALID_NUM;
}Trans_5003_2;

//HHJT_CJP 20210319 BOM日结数据
typedef struct _Trans_5004_Head
{
	unsigned short w_Records;
}Trans_5004_Head;
typedef struct _Trans_5004_Record_1
{
	BYTE  				DEVICE_NID[4];		
	unsigned int 	SETTLE_NBR;	
	char  				RUN_DATE[4];
	BYTE  				SETTLE_START_DATE[7];		
	BYTE  				SETTLE_END_DATE[7];	
	BYTE  				SETTLE_OPERATOR_ID[4];	
	BYTE        	REAL_INCOME_ACCOUNT[4];
	unsigned int 	UNBUSSINESS_INCOME_NUM;
	unsigned int 	UNBUSINESS_INCOME_AMOUNT;
	unsigned int 	UNBUSINESS_PAY_NUM;
	unsigned int 	UNBUSINESS_PAY_AMOUNT;
	unsigned int 	FREE_OUT_NUM;
	unsigned int 	PAY_OUT_NUM;
	unsigned int 	PAY_OUT_AMOUNT;
	unsigned int 	WASTE_NUM;	
	unsigned int 	FAULT_NUM;	
	unsigned int 	FAULT_AMOUNT;
}Trans_5004_Record_1;
typedef struct _Trans_5004_Record_2
{		
	unsigned int 	dw_Records_1;
}Trans_5004_Record_2;
typedef struct _Trans_5004_Record_3
{		
	unsigned char TICKET_TYPE[2];
	unsigned int 	PAY_UPDATE_NUM;
	unsigned int 	PAY_UPDATE_AMOUNT;
	unsigned int 	FREE_UPDATE_NUM;
	unsigned int 	FREE_UPDATE_AMOUNT;
}Trans_5004_Record_3;
typedef struct _Trans_5004_Record_4
{		
	unsigned int 	dw_Records_2;
}Trans_5004_Record_4;
typedef struct _Trans_5004_Record_5
{		
	BYTE 					SETTLE_DATE[4];
	unsigned char OPERATOR_ID[4];
	unsigned char TICKET_TYPE[2];
	unsigned char TRANS_TYPE;
	unsigned char TRANS_SUBTYPE;
	unsigned int 	TRANS_NUM;
	unsigned int 	DEPOSIT_VALUE;
	unsigned int 	TRANS_AMOUNT;
	unsigned int 	TRANS_FEE;
	unsigned int 	CARD_COST;
}Trans_5004_Record_5;

//HHJT_CJP 20210319 TVM日结数据
typedef struct _Trans_5005_Head
{
	unsigned short w_Records;
}Trans_5005_Head;
typedef struct _Trans_5005_Record_1
{
	BYTE  				DEVICE_NID[4];
	BYTE  				OPERATOR_ID[4];
	BYTE  				SETTLE_DATE[4];		
	unsigned int 	SETTLE_SEQ;		
	BYTE  				START_SETTLE_TIME[7];		
	BYTE  				END_SETTLE_TIME[7];	
	unsigned int 	CASH_SALE_COUNT;
	unsigned int 	CASH_SALE_AMOUNT;
	unsigned int 	SVT_SALE_COUNT;
	unsigned int 	SVT_SALE_AMOUNT;
	unsigned int 	SVT_LOAD_COUNT;
	unsigned int 	SVT_LOAD_AMOUNT;
	unsigned int 	PERL_LOAD_COUNT;
	unsigned int 	PERL_LOAD_AMOUNT;
	unsigned int 	BANKCARD_LOAD_COUNT;
	unsigned int 	BANKCARD_LOAD_AMOUNT;
	unsigned int 	DEBITCARD_LOAD_COUNT;
	unsigned int 	DEBITCARD_LOAD_AMOUNT;
	unsigned int 	CREDITCARD_LOAD_COUNT;
	unsigned int 	CREDITCARD_LOAD_AMOUNT;
	unsigned int 	ERROR_TRANS_COUNT;
	unsigned int 	ERROR_TRANS_AMOUNT;
	unsigned int 	PRE_COIN_REMAIN_AMOUNT;
	unsigned int 	COIN_SUPPLY_AMOUNT;
	unsigned int 	RECYCLE_COIN_AMOUNT;
	unsigned int 	IN_COIN_AMOUNT;
	unsigned int 	RECLAIM_COIN_AMOUNT;
	unsigned int 	COIN_REMAIN_AMOUNT;
	unsigned int 	PRE_CASH_REMAIN_AMOUNT;
	unsigned int 	CASH_SUPPLY_AMOUNT;
	unsigned int 	CASH_RECYCLE_AMOUNT;
	unsigned int 	IN_CASH_AMOUNT;
	unsigned int 	RECLAIM_CASH_AMOUNT;
	unsigned int 	CASH_REMAIN_AMOUNT;
	unsigned int 	PRE_TICKET_REMAIN;
	unsigned int 	DAMAGE_SJT_COUNT;
	unsigned int 	TICKET_SUPPLY_COUNT;
	unsigned int 	TICKET_SALE_COUNT;
	unsigned int 	TICKET_RECYCLE_COUNT;	
}Trans_5005_Record_1;
 
//HHJT_CJP 20210321 【0x5010】BOM签到/签退接口
typedef struct _Trans_5010
{	
	BYTE	         DEVICE_NID[4];	
	BYTE	         OPERATOR_ID[4];
	BYTE	         OPERATE_TYPE;
	BYTE           OCCUR_TIME[7];
	BYTE           RFU[4];
}Trans_5010;
 
//HHJT_CJP 20210321【0x5011】操作员密码修改接口
typedef struct _Trans_5011
{	
	BYTE	         USER_ID[4];
	BYTE	         USER_PIN[16];
	BYTE           UPDATE_FLAG;
	BYTE           RFU[3];
}Trans_5011;
 
//HHJT_CJP 20210321【0x5012】操作员密码修改广播
typedef struct _Trans_5012
{	
	BYTE	         USER_ID[4];
	BYTE	         USER_PIN[16];
	BYTE           RFU[4];
}Trans_5012;


//HHJT_CJP 20210319 TVM故障单信息
typedef struct _Trans_5020
{
	BYTE           DEVICE_NID[4];  
	BYTE           OCCUR_TIME[7];         
	unsigned int   ERROR_SEQ;    
	unsigned int   ERROR_NO;        
	unsigned int   TRANS_SEQ;   
	BYTE           VALUE_TRANS[4];      
	BYTE           ERROR_REASON[4];        
}Trans_5020;

//HHJT_CJP 20230129【0x5021】BOM退故障单信息 
typedef struct _Trans_5021
{
	BYTE           DEVICE_NID[4];  
	BYTE           OCCUR_TIME[7];         
	unsigned int   TRANS_SEQ;  
	BYTE	         OPERATOR_ID[4];
	BYTE           FAULT_DEVICE_NID[4];
	unsigned int   ERROR_NO;  
	BYTE           TRANS_VALUE[4]; 		  
	BYTE				   PAYMENT_METHOD;         
	BYTE           RFU[8];        
}Trans_5021;

//HHJT_CJP 20230129 【0x5022】BOM非营业性收支数据
typedef struct _Trans_5022
{
	BYTE           DEVICE_NID[4];  
	BYTE           OCCUR_TIME[7];         
	unsigned int   TRANS_SEQ;  
	BYTE	         OPERATOR_ID[4];
	BYTE           INOUT_FLAG;
	BYTE				   INOUT_REASON;  
	BYTE           TRANS_VALUE[4]; 		  
	BYTE				   PAYMENT_METHOD;
	char					 MEMO[64];         
	BYTE           RFU[8];  
	
	BYTE           SALE_TICKET_FLAG; 	
	BYTE           SALE_TICKET_TYPE[2]; 	
	BYTE           SALE_TRANS_VALUE[4];
	BYTE				   SALE_PAYMENT_METHOD; 
	unsigned char  SALE_CARD_PHY_ID[8];
	unsigned int   SALE_DEVICE_TRACE_NBR;				      
	BYTE           RFU_2[8];
}Trans_5022;


/*----------------------------------------
------------------审计数据----------------
-----------------------------------------*/

//HHJT_CJP 20210319  通用命令
typedef struct _CMD_6001
{
	unsigned char	b_CMDID[8];				/* ---  --- */
	unsigned char	b_SourceID[4];			/* ---  --- */
	unsigned char	b_DestID[4];			/* ---  --- */
	unsigned char	b_CMDType;				/* ---  --- */
}CMD_6001;

//HHJT_CJP 20210319 模式切换命令、广播
typedef struct _CMD_6002
{
	unsigned char	b_CMDID[8];				/* ---  --- */
	unsigned char	b_SourceID[4];		/* ---  --- */
	unsigned char	b_DestID[4];			/* ---  --- */
	unsigned char	b_CmdType;				/* ---  --- */
	unsigned char	b_RunMode;				/* ---  --- */
	unsigned char	b_SCID[2];				/* ---  --- */
	unsigned char	b_TransactionTime[7];	/* ---  --- */
}CMD_6002;

//HHJT_CJP 20210319 上传指定数据包
typedef struct _CMD_6003 
{
	unsigned char	b_CMDID[8];				/* ---  --- */
	unsigned char	b_SourceID[4];			/* ---  --- */
	unsigned char	b_DestID[4];			/* ---  --- */
	unsigned char	b_StartPkgID[12];		/* ---  --- */
	unsigned char	b_EndPkgID[12];			/* ---  --- */
}CMD_6003;

//HHJT_CJP 20210319 上传指定时间数据包
typedef struct _CMD_6004
{
	unsigned char	b_CMDID[8];				/* ---  --- */
	unsigned char	b_SourceID[4];			/* ---  --- */
	unsigned char	b_DestID[4];			/* ---  --- */
	unsigned char	b_StartDate[7];			/* ---  --- */
	unsigned char	b_EndDate[7];			/* ---  --- */
}CMD_6004;

//HHJT_CJP 20210319 参数下载命令
typedef struct _CMD_6005
{
	unsigned char	b_CMDID[8];				/* ---  --- */
	unsigned char	b_SourceID[4];			/* ---  --- */
	unsigned char	b_DestID[4];			/* ---  --- */
	unsigned char	b_ParamType;			/* ---  --- */
	unsigned char	b_ParamVer[7];			/* ---  --- */
	unsigned char	b_ParamValie[7];		/* ---  --- */
}CMD_6005;

//HHJT_CJP 20210319 车票回收命令
typedef struct _CMD_6006
{
	unsigned char	b_CMDID[8];				/* ---  --- */
	unsigned char	b_SourceID[4];			/* ---  --- */
	unsigned char	b_DestID[4];			/* ---  --- */
	unsigned int	dw_BatchNum;			/* ---  --- */
}CMD_6006;
 
//HHJT_CJP 20210319 下发对账数据
typedef struct _CMD_6007
{
	unsigned char	b_CMDID[8];				/* ---  --- */
	unsigned char	b_SourceID[4];			/* ---  --- */
	unsigned char	b_DestID[4];			/* ---  --- */
	unsigned char	b_FileType;				/* ---  --- */
	unsigned char	b_SettleDate[4];		/* ---  --- */
}CMD_6007;

//HHJT_CJP 20210319 调票结果通知指令
typedef struct _CMD_6008
{
	unsigned char	b_CMDID[8];				/* ---  --- */
	unsigned char	b_SourceID[4];			/* ---  --- */
	unsigned char	b_DestID[4];			/* ---  --- */
	unsigned char	b_OrderNum[12];			/* ---  --- */
}CMD_6008;

//HHJT_CJP 20210324 控制命令下发
typedef struct _CMD_6010
{
	unsigned char	b_SourceID[4];
	unsigned char	b_DestID[4];
	unsigned char	b_CmdCode;	
	unsigned char	b_OperatorID[4];
}CMD_6010;

//HHJT_CJP 20210319 设置时钟
typedef struct _CMD_6011
{
	unsigned int	dwNowTime;
}CMD_6011;

//HHJT_CJP 20230129 软件更新请求
typedef struct _CMD_6022
{
	unsigned char	b_DestID[4];
	char					ftpFullPath[64];
	char					ftpFileName[16];	
	unsigned char	RFU[32];
}CMD_6022;

//HHJT_CJP 20210323 查询参数版本请求
typedef struct _CMD_6081_Req_Head
{
	unsigned char	b_DestID[4];
	unsigned char	b_ParamType;
}CMD_6081_Req_Head;

typedef struct _CMD_6081_Req_Record
{
	unsigned short	w_ParamType;
	unsigned char		b_ParamVer[7];
}CMD_6081_Req_Record;

//HHJT_CJP 202103234 查询参数版本应答
typedef struct _CMD_6081_Ack_Head
{
	unsigned char  b_AckCode; 	/* --- 应答码 --- */
	unsigned char	 b_ParamType;	/* --- 参数类型 --- */
}CMD_6081_Ack_Head;
typedef struct _CMD_6081_Ack_Record
{
	unsigned short wParaCode;    	/* ---参数类型  --- */
	BYTE		   		 ParamVer[7];  	/* ---参数版本  --- */
	BYTE           ParamValid[7]; /* ---参数生效时间  --- */
}CMD_6081_Ack_Record;


 
//HHJT_CJP 20210324 查询软件版本请求
typedef struct _CMD_6082_Req_Head
{
	unsigned char	b_DestID[4];
	unsigned char	b_Type;
}CMD_6082_Req_Head;



/*----------------------------------------
------------------请求数据----------------
-----------------------------------------*/
//HHJT_CJP 20210319 模式切换通知
typedef struct _Req_6101
{
	unsigned char	b_SCID[2];				/* ---  --- */
	unsigned char	b_RunMode;				/* ---  --- */
	unsigned char	b_TransactionTime[7];	/* ---  --- */
}Req_6101;

//HHJT_CJP 20210319 运营参数版本查询请求
typedef struct _Req_6102
{
	unsigned short wMsgCode;				/* ---  --- */
	unsigned char	 b_ParamStatus;	/* ---  --- */
}Req_6102;

//HHJT_CJP 20210319 个性化票卡卡号查询请求
typedef struct _Req_6103
{
	unsigned char	b_CardType;			/* ---  --- */
	unsigned short	w_CardLen;			/* ---  --- */
	unsigned char	b_CardNum[50];		/* ---  --- */
	unsigned short	w_NameLen;			/* ---  --- */
	unsigned char	b_Name[50];			/* ---  --- */
}Req_6103;

/*----------------------------------------
------------------反馈数据----------------
-----------------------------------------*/

//HHJT_CJP 20210319 命令数据执行应答
typedef struct _ACK_6201
{
	unsigned char	b_CMDID[8];				/* ---  --- */
	unsigned char	b_SourceID[4];			/* ---  --- */
	unsigned char	b_DestID[4];			/* ---  --- */
	unsigned char	b_ACK;					/* ---  --- */
}ACK_6201;

//HHJT_CJP 20210319 模式切换通知应答
typedef struct _Ack_6301
{
	unsigned char	b_AckCode;				/* ---  --- */
}Ack_6301;

//HHJT_CJP 20210319 参数版本查询应答
typedef struct _Ack_6302
{
	unsigned char		b_AckCode;				/* ---  --- */
	unsigned short	wMsgCode;				/* ---  --- */
	unsigned char   b_CurParamVer[7];		/* ---  --- */
	unsigned char   b_FutParamVer[7];		/* ---  --- */
}Ack_6302;

//HHJT_CJP 20210319 个性化票卡卡号查询应答
typedef struct _Ack_6303
{
	unsigned char	b_AckCode;				/* ---  --- */
	unsigned char   b_TicketNum[5];		/* ---  --- */
}Ack_6303;


/*----------------------------------------
------------------状态数据----------------
-----------------------------------------*/

//HHJT_CJP 20210319 设备状态信息
typedef struct _Trans_7001_1
{
	unsigned short 	w_Records;	
}Trans_7001_1;
typedef struct _Trans_7001_2
{
  BYTE          	SLE_NID[4];
	BYTE            ParamVer[7];
	char            READER1[12];
	char            READER2[12];
}Trans_7001_2;

//HHJT_CJP 20210319 设备状态统计信息
typedef struct _Trans_7002_1
{
  BYTE          	CC_NID;
	unsigned short 	w_Records;	
}Trans_7002_1;
typedef struct _Trans_7002_2
{
  BYTE          	SC_NID[2];
  BYTE          	SLE_TYPE;
	unsigned short  Right_Count;
	unsigned short  Offline_Count;
	unsigned short  Failure_Count;
}Trans_7002_2;

//HHJT_CJP 20210321 终端设备状态接口
typedef struct _Trans_7010_1
{
  BYTE          	DEVICE_NID[4];
  BYTE						OCCUR_TIME[7];
  BYTE						DEVICE_STATUS;
  BYTE						DEVICE_MODE;
	unsigned short 	w_Records;	
}Trans_7010_1;
typedef struct _Trans_7010_2
{
  BYTE						EventCode[4];	
  BYTE          	RFU[4];
}Trans_7010_2;


/*----------------------------------------
------------现金管理系统------------------
-----------------------------------------*/

//HHJT_CJP 20210319 BOM营业款缴款接口
typedef struct _Trans_F001_1
{
	unsigned short 	w_Records;	
}Trans_F001_1;
typedef struct _Trans_F001_2
{
	char            DATA_SEQ[20];
  BYTE          	LINE_NID;	
  BYTE          	STATION_NID[2];
  BYTE          	DEVICE_NID[4]; 
  BYTE            CHECK_TIME[7];
  BYTE            CHECK_AMOUNT[4];
	BYTE            SPECIAL_REFUND_AMOUNT[4];
	BYTE            TRANSACTION_AMOUNT[4];
	BYTE            TRANS_DATE[4];
  BYTE          	BOM_OPERATOR[4];
  BYTE          	OPERATOR_ID[4];
	char            MEMO[80];
	char						RFU[20];
}Trans_F001_2;

//HHJT_CJP 20210319 TVM营业款缴款接口
typedef struct _Trans_F002_1
{
	unsigned short 	w_Records;	
}Trans_F002_1;
typedef struct _Trans_F002_2
{
	char            DATA_SEQ[20];
  BYTE          	LINE_NID;	
  BYTE          	STATION_NID[2];
  BYTE          	DEVICE_NID[4]; 
  BYTE            CHECK_TIME[7];
  BYTE          	CASHBOX_NBR;
	BYTE            CHECK_AMOUNT[4];
	BYTE            TRANS_DATE[4];
  BYTE          	TVM_OPERATOR[4];
  BYTE          	OPERATOR_ID[4];
	char            MEMO[80];
	char						RFU[20];
}Trans_F002_2;

//HHJT_CJP 20210319 TVM手工清点缴款
typedef struct _Trans_F003_1
{
	unsigned short 	w_Records;	
}Trans_F003_1;
typedef struct _Trans_F003_2
{
	char            DATA_SEQ[20];
  BYTE          	LINE_NID;	
  BYTE          	STATION_NID[2];
  BYTE          	DEVICE_NID[4]; 
  BYTE            CHECK_TIME[7];
	BYTE            CHECK_AMOUNT[4];
	BYTE            TRANS_DATE[4];
	BYTE          	OPERATOR_ID[4];
	char            MEMO[80];
	char						RFU[20];
}Trans_F003_2;


//HHJT_CJP 20210319 银行现金缴款
typedef struct _Trans_F004_1
{
	unsigned short 	w_Records;	
}Trans_F004_1;
typedef struct _Trans_F004_2
{
	char            DATA_SEQ[20];
  BYTE          	LINE_NID;	
  BYTE          	STATION_NID[2];
  BYTE            OPERATE_TIME[7];
 	BYTE            PAYMENT_AMOUNT[4];
	BYTE            PAYMENT_DATE[4];	 	
  BYTE          	OPERATOR_ID[4];
 	char            MEMO[80];
	char						RFU[20];
  //BYTE          PAYMENT_OPERATOR[4];
}Trans_F004_2;

//HHJT_CJP 20210319 备用金领用上缴
typedef struct _Trans_F005_1
{
	unsigned short 	w_Records;	
}Trans_F005_1;
typedef struct _Trans_F005_2
{
	char            DATA_SEQ[20];
  BYTE          	LINE_NID;	
  BYTE          	STATION_NID[2];
  BYTE          	DEVICE_NID[4];
	BYTE            INOUT_TIME[7];  
  BYTE          	CASHBOX_NBR;  
  BYTE          	OPERATE_TYPE; 
 	BYTE            INOUT_AMOUNT[4];
 	BYTE            TRANS_DATE[4];
  BYTE          	INOUT_OPERATOR[4];	
  BYTE          	OPERATOR_ID[4]; 		 
 	char            MEMO[80];
	char						RFU[20];
}Trans_F005_2;


//HHJT_CJP 20210319 备用金调入/调出/盘点
typedef struct _Trans_F006_1
{
	unsigned short 	w_Records;	
}Trans_F006_1;
typedef struct _Trans_F006_2
{
	char            DATA_SEQ[20];
  BYTE          	LINE_NID;	
  BYTE          	STATION_NID[2];
  BYTE          	DEVICE_NID[4];
	BYTE            SE_TIME[7]; 
  BYTE          	SE_TYPE;
	BYTE            SE_AMOUNT[4];
 	BYTE            TRANS_DATE[4];	
  BYTE          	SE_OPERATOR[4];
 	char            MEMO[80];
	char						RFU[20];  
	//BYTE          	OPERATOR_ID[4];
}Trans_F006_2;



/*----------------------------------------
------------COC->CC 定义报文------------------
-----------------------------------------*/

//HHJT_CJP 20210322 参数下载通知
typedef struct _Cfg8006
{
	BYTE	       SCID[4];			/* ---可以填MLC/CC/SC节点 --- */
	WORD  			 ParamType;	  
	BYTE		     ParamVer[7];  /* ---参数版本号  --- */
	BYTE         RFU[4];
}Cfg8006;

//HHJT_CJP 20210323 票务数据通知
typedef struct _Cfg8007
{
	BYTE	       SCID[4];	/* ---申请单位 可以填CC/SC节点 --- */	
	WORD  			 ParamType;	  
	BYTE		     APPLY_NO[12];  /* ---  --- */
	BYTE         RFU[4];
}Cfg8007;

//HHJT_CJP 20210407 参数导入通知
typedef struct _Cfg9405
{
	BYTE	       SCID[4];			/* ---填LC节点 --- */
	WORD  			 ParamType;	
	BYTE         RFU[4];
}Cfg9405;


#pragma pack ()

#endif
