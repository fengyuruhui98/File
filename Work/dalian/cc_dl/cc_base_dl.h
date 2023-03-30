#ifndef CC_BASE_DL_H
#define CC_BASE_DL_H

#define PRIMARY_VERSION 23
#define SECOND_VERSION  2
#define THIRD_VERSION   15
 
 
#define LEN_OF_PackageHead      sizeof(MsgHead)         					 /* --- 包头的长度 ------------- */
#define LEN_OF_PackageTail      4                                  /* --- CRC长度 -------------- */
#define LEN_OF_ACCPackageHead   sizeof(struct ACC_PackageHead)     /* --- ACC包头的长度 ------------- */
#define LEN_CRITICALSESSION	    sizeof(CRITICAL_SECTION)

#define MAX_PARAM_COUNT     		60
#define MAX_SPLITPACKAGE    		500

 
#define LEN_OF_HEAD             2           /* --- 报文长度所占字节数 ----- */

#ifndef MAX_BAOWEN_LENGTH
#define MAX_BAOWEN_LENGTH				8192	   		/* ------- 报文的最大长度 ----- */
#endif

#define MAX_PATH     	        	256	    		/* ------- 文件名的最大长度 ---- */
#define BLOCKSIZE               1024        /* --- 一次接收的字节(FTP) ---- */
#define MAX_FILE_SIZE           5120*1024   /* --- 日志文件最大的字节数 --- */
#define MAX_SLE_COUNT           99   
 
 

#include "sys_head.h"
#include "base_funcs.h"

#pragma pack (1)
typedef struct _ScList
{
	int  lineId;
	char scip[20];
	int  scport;
	int  downloadflag;
	char sccode[9];
	BOOL IsConnect;
	int  sockhandle;
	unsigned char recv_buf[MAX_BAOWEN_LENGTH];
	int  buflen;
	WORD  msgtype;
	int  EntryFlow;
	int  ExitFlow;
	int  SJTFlow;
	int  uploadfag;
}ScList;

typedef struct _SC_FLOW
{
	int  EntryFlow;
	int  ExitFlow;
	int  SJTFlow;
}SC_FLOW;

typedef struct _MlcLineList
{
	unsigned int	LINE_ID;									/* --- 线路编号 ------- */
	char					LINE_NID[9];							/* --- 线路节点标识 --- */
	char					UP_FTP_IP[17];						/* ---上传IP地址  ----- */
	unsigned int	UP_FTP_PORT;							/* ---上传端口号   ---- */
	char					UP_FTP_USER[33];					/* ---上传用户名  ----- */
	char					UP_FTP_PASSWD[33];				/* ---上传密码--------- */	
	char					DIR_DATA_TRANS[65];				/* ---上传数据目录 ---- */
	char					DIR_DATA_STATUS[65];			/* ---上传状态目录----- */	
	char					DIR_DATA_OTHER[65];				/* ---上传其他数据目录- */
	char					DOWN_NORMAL_FTP_IP[17];						/* ---正常下载IP地址  ----- */
	unsigned int	DOWN_NORMAL_FTP_PORT;							/* ---正常下载端口号   ---- */
	char					DOWN_NORMAL_FTP_USER[33];					/* ---正常下载用户名  ----- */
	char					DOWN_NORMAL_FTP_PASSWD[33];				/* ---正常下载密码--------- */	
	char					DIR_PARAM_MODE[65];								/* ---模式履历文件 -------- */
	char					DIR_SETTLE[65];										/* ---对账文件下载--------- */	
	char					DIR_STOCK[65];										/* ---库存文件下载--------- */
	char					DOWN_SPECIAL_FTP_IP[17];					/* ---特殊下载IP地址  ----- */
	unsigned int	DOWN_SPECIAL_FTP_PORT;						/* ---特殊下载端口号   ---- */
	char					DOWN_SPECIAL_FTP_USER[33];				/* ---特殊下载用户名  ----- */
	char					DOWN_SPECIAL_FTP_PASSWD[33];			/* ---特殊下载密码--------- */	
	char					DIR_PARAM_RUN[65];								/* ---运营参数文件下载 ---- */
	char					DIR_PARAM_OTHER[65];							/* ---其他参数下载--------- */	
	char					DIR_PARAM_BLACKLIST[65];					/* ---黑名单文件下载------- */
}MlcLineList;


typedef struct _IscsStation
{
	char AFC_LINE_NID[3];
	char AFC_STATION_NID[5];
	char ISCS_LINE_NID[3];
	char ISCS_STATION_NID[5];          
	int	ISCS_STATION_ID;         
	int	ISCS_BOM_START_BYTE;        
	int	ISCS_TVM_START_BYTE;        
	int	ISCS_TCM_START_BYTE;        
	int	ISCS_AGM_START_BYTE;        
	int	ISCS_BOM_START_OFFSET;        
	int	ISCS_TVM_START_OFFSET;        
	int	ISCS_TCM_START_OFFSET;   
	int	ISCS_AGM_START_OFFSET;
	int	ISCS_SNC_START_BYTE;        
	int	ISCS_RUNMODE_START_BYTE;    
	int	ISCS_EPF_START_BYTE;        
	int	ISCS_XPF_START_BYTE;        
	int	ISCS_SPF_START_BYTE;	
}IscsStation;






#pragma pack ()

class CConfig
{
public:
		static void InitConfigStaticVar(const char* iniAppPath);

public:
	
  //数据库连接
	static char 	m_DBUserName[32];
	static char		m_DBUserPasswd[32];
	static char		m_DBHostName[32];
 
  //日志路径
  static char	m_AppLogPath[MAX_PATH];
 
	// NTP变量
	static int	m_NtpFlag;
	static int	m_NtpPort;
	static int  m_NtpGap;
	static int	m_NtpTimedout;
	static char m_NtpIP[16];

	//ACC相关配置
  static char 	g_ACC_ID[9];		 	//ACC
	static BYTE 	g_ACCIDCode[4];	 	//ACC标识
	static char	 	g_Socket_ACCIP[16];
	static int		g_ACCPort;

	//CC节点标识
  static char 	g_CCID[9];
	static BYTE 	g_CCIDCode[4];

	//参数数据目录
  static char 	g_CurParamPath[MAX_PATH];
  static char 	g_TmpParamPath[MAX_PATH];
  static char 	g_FutParamPath[MAX_PATH];	
  static char 	g_ImportParamPath[MAX_PATH];		
  static char 	g_SleSoftfilePath[MAX_PATH];		
	
	
	//历史数据目录
	static char 	HisDataPath_ACC[MAX_PATH];
	static char 	HisDataPath_LCC[MAX_PATH];
 
	//入库数据目录	
  static char 	InsertStatusFilePath[MAX_PATH];
  static char 	InsertTransFilePath[MAX_PATH];
	static char 	ErrorPath[MAX_PATH];	
   
	//ACC数据目录	
	static char 	TempSendFilePath[MAX_PATH];   
	static char 	RecvPath[MAX_PATH];
	static char 	SendFilePath_ACC[MAX_PATH];
 
 	/* ----- FTP目录 ---*/
	static char 	g_Ftp_Para_Run[MAX_PATH];
	static char 	g_Ftp_Para_BlackList[MAX_PATH];
	static char 	g_Ftp_Para_Mode[MAX_PATH];
	static char 	g_Ftp_Para_Other[MAX_PATH];
	static char 	g_Ftp_Settle[MAX_PATH];
	static char 	g_Ftp_Stock[MAX_PATH];
	static char 	g_Ftp_Data_Trans[MAX_PATH];
	static char 	g_Ftp_Data_Status[MAX_PATH];
	static char 	g_Ftp_Data_Other[MAX_PATH];

	static char 	StockPath[MAX_PATH]; 
 
 
 	static char 	g_ACCFtpUser[MAX_PATH];
	static char 	g_FTP_ACCIP[MAX_PATH];
	static char 	g_ACCFtpPass[MAX_PATH];
 
	
	//车站列表
  static ScList  g_ScList[MAX_SLE_COUNT];
  
  //线路列表
  static MlcLineList  ms_MlcLineList[MAX_SLE_COUNT];
  
 	//综合监控车站列表
  static IscsStation  ms_IscsStation[MAX_SLE_COUNT]; 
   
	 
	//监听端口配置
  static int  g_Lsn_AccPort;
  static int  g_Lsn_ScPort;
  static int  g_Lsn_ISCSPort; 
	 
	//时间配置 
  static int 	ParamValidTime;	
  static int  QueryScStatusTime;
  static int  QueryScParamTime;
  static int  ClockTime;
  static int  g_Wait_Ack_Time;	
	
	//数值配置 
 
  static int	g_MaxRecord;
  static int	g_MaxDelay;     
 
  
  
  
  //标志位配置 
  static int 	g_Auto_Upload_Flag;
  static int	g_Conn_L; 
  static int	g_RunMode_Flag;
    
  //全局变量
  static int	g_ExecBatch_Flag;
  static char g_LocalIP[16];
  static BYTE g_b_LocalIP[4];
  
  static bool    			ms_AccIsConnect;	//ACC是否已经连接
  static bool    			ms_DiskIsFull;		//服务器磁盘空间是否已满
  static bool    			ms_SynTimeError;	//同步时间是否失败
  static bool    			ms_SynTimeOverDiff;////同步时间差是否超过指定时间	
};

 

class CDir
{
public:
    static char m_AppPath[MAX_PATH];
    static char m_AppLogPath[MAX_PATH];
private:
    static int GetAppPath();
    static int AppDirIsExists(const char *dir);
    static int CreateAppDir(const char *dir);
    static int CreateLogDir();
    static int CreateDataDir();
    static int CreateParamDir();

    static int CreateSoftDir(); 	
public:   
    static int CreateAppAllDir();
    static int CreateLineParamDir(int lineId);
};

 
#endif

 