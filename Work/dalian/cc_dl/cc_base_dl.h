#ifndef CC_BASE_DL_H
#define CC_BASE_DL_H

#define PRIMARY_VERSION 23
#define SECOND_VERSION  2
#define THIRD_VERSION   15
 
 
#define LEN_OF_PackageHead      sizeof(MsgHead)         					 /* --- ��ͷ�ĳ��� ------------- */
#define LEN_OF_PackageTail      4                                  /* --- CRC���� -------------- */
#define LEN_OF_ACCPackageHead   sizeof(struct ACC_PackageHead)     /* --- ACC��ͷ�ĳ��� ------------- */
#define LEN_CRITICALSESSION	    sizeof(CRITICAL_SECTION)

#define MAX_PARAM_COUNT     		60
#define MAX_SPLITPACKAGE    		500

 
#define LEN_OF_HEAD             2           /* --- ���ĳ�����ռ�ֽ��� ----- */

#ifndef MAX_BAOWEN_LENGTH
#define MAX_BAOWEN_LENGTH				8192	   		/* ------- ���ĵ���󳤶� ----- */
#endif

#define MAX_PATH     	        	256	    		/* ------- �ļ�������󳤶� ---- */
#define BLOCKSIZE               1024        /* --- һ�ν��յ��ֽ�(FTP) ---- */
#define MAX_FILE_SIZE           5120*1024   /* --- ��־�ļ������ֽ��� --- */
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
	unsigned int	LINE_ID;									/* --- ��·��� ------- */
	char					LINE_NID[9];							/* --- ��·�ڵ��ʶ --- */
	char					UP_FTP_IP[17];						/* ---�ϴ�IP��ַ  ----- */
	unsigned int	UP_FTP_PORT;							/* ---�ϴ��˿ں�   ---- */
	char					UP_FTP_USER[33];					/* ---�ϴ��û���  ----- */
	char					UP_FTP_PASSWD[33];				/* ---�ϴ�����--------- */	
	char					DIR_DATA_TRANS[65];				/* ---�ϴ�����Ŀ¼ ---- */
	char					DIR_DATA_STATUS[65];			/* ---�ϴ�״̬Ŀ¼----- */	
	char					DIR_DATA_OTHER[65];				/* ---�ϴ���������Ŀ¼- */
	char					DOWN_NORMAL_FTP_IP[17];						/* ---��������IP��ַ  ----- */
	unsigned int	DOWN_NORMAL_FTP_PORT;							/* ---�������ض˿ں�   ---- */
	char					DOWN_NORMAL_FTP_USER[33];					/* ---���������û���  ----- */
	char					DOWN_NORMAL_FTP_PASSWD[33];				/* ---������������--------- */	
	char					DIR_PARAM_MODE[65];								/* ---ģʽ�����ļ� -------- */
	char					DIR_SETTLE[65];										/* ---�����ļ�����--------- */	
	char					DIR_STOCK[65];										/* ---����ļ�����--------- */
	char					DOWN_SPECIAL_FTP_IP[17];					/* ---��������IP��ַ  ----- */
	unsigned int	DOWN_SPECIAL_FTP_PORT;						/* ---�������ض˿ں�   ---- */
	char					DOWN_SPECIAL_FTP_USER[33];				/* ---���������û���  ----- */
	char					DOWN_SPECIAL_FTP_PASSWD[33];			/* ---������������--------- */	
	char					DIR_PARAM_RUN[65];								/* ---��Ӫ�����ļ����� ---- */
	char					DIR_PARAM_OTHER[65];							/* ---������������--------- */	
	char					DIR_PARAM_BLACKLIST[65];					/* ---�������ļ�����------- */
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
	
  //���ݿ�����
	static char 	m_DBUserName[32];
	static char		m_DBUserPasswd[32];
	static char		m_DBHostName[32];
 
  //��־·��
  static char	m_AppLogPath[MAX_PATH];
 
	// NTP����
	static int	m_NtpFlag;
	static int	m_NtpPort;
	static int  m_NtpGap;
	static int	m_NtpTimedout;
	static char m_NtpIP[16];

	//ACC�������
  static char 	g_ACC_ID[9];		 	//ACC
	static BYTE 	g_ACCIDCode[4];	 	//ACC��ʶ
	static char	 	g_Socket_ACCIP[16];
	static int		g_ACCPort;

	//CC�ڵ��ʶ
  static char 	g_CCID[9];
	static BYTE 	g_CCIDCode[4];

	//��������Ŀ¼
  static char 	g_CurParamPath[MAX_PATH];
  static char 	g_TmpParamPath[MAX_PATH];
  static char 	g_FutParamPath[MAX_PATH];	
  static char 	g_ImportParamPath[MAX_PATH];		
  static char 	g_SleSoftfilePath[MAX_PATH];		
	
	
	//��ʷ����Ŀ¼
	static char 	HisDataPath_ACC[MAX_PATH];
	static char 	HisDataPath_LCC[MAX_PATH];
 
	//�������Ŀ¼	
  static char 	InsertStatusFilePath[MAX_PATH];
  static char 	InsertTransFilePath[MAX_PATH];
	static char 	ErrorPath[MAX_PATH];	
   
	//ACC����Ŀ¼	
	static char 	TempSendFilePath[MAX_PATH];   
	static char 	RecvPath[MAX_PATH];
	static char 	SendFilePath_ACC[MAX_PATH];
 
 	/* ----- FTPĿ¼ ---*/
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
 
	
	//��վ�б�
  static ScList  g_ScList[MAX_SLE_COUNT];
  
  //��·�б�
  static MlcLineList  ms_MlcLineList[MAX_SLE_COUNT];
  
 	//�ۺϼ�س�վ�б�
  static IscsStation  ms_IscsStation[MAX_SLE_COUNT]; 
   
	 
	//�����˿�����
  static int  g_Lsn_AccPort;
  static int  g_Lsn_ScPort;
  static int  g_Lsn_ISCSPort; 
	 
	//ʱ������ 
  static int 	ParamValidTime;	
  static int  QueryScStatusTime;
  static int  QueryScParamTime;
  static int  ClockTime;
  static int  g_Wait_Ack_Time;	
	
	//��ֵ���� 
 
  static int	g_MaxRecord;
  static int	g_MaxDelay;     
 
  
  
  
  //��־λ���� 
  static int 	g_Auto_Upload_Flag;
  static int	g_Conn_L; 
  static int	g_RunMode_Flag;
    
  //ȫ�ֱ���
  static int	g_ExecBatch_Flag;
  static char g_LocalIP[16];
  static BYTE g_b_LocalIP[4];
  
  static bool    			ms_AccIsConnect;	//ACC�Ƿ��Ѿ�����
  static bool    			ms_DiskIsFull;		//���������̿ռ��Ƿ�����
  static bool    			ms_SynTimeError;	//ͬ��ʱ���Ƿ�ʧ��
  static bool    			ms_SynTimeOverDiff;////ͬ��ʱ����Ƿ񳬹�ָ��ʱ��	
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

 