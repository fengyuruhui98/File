
#include "cc_base_dl.h"
	
  //���ݿ�����
	char 	CConfig::m_DBUserName[32];
	char		CConfig::m_DBUserPasswd[32];
	char		CConfig::m_DBHostName[32];
 
  //��־·��
  char		CConfig::m_AppLogPath[MAX_PATH];
 
	// NTP����
	int	CConfig::m_NtpFlag;
	int	CConfig::m_NtpPort;
	int  CConfig::m_NtpGap;
	int	CConfig::m_NtpTimedout;
	char CConfig::m_NtpIP[16];

	//ACC�������
  char 	CConfig::g_ACC_ID[9];		 	//ACC
	BYTE 	CConfig::g_ACCIDCode[4];	//ACC��ʶ
	char	CConfig::g_Socket_ACCIP[16];
	int		CConfig::g_ACCPort;

	//CC�ڵ��ʶ
  char 	CConfig::g_CCID[9];
	BYTE 	CConfig::g_CCIDCode[4];

	//��������Ŀ¼
  char CConfig::g_CurParamPath[MAX_PATH];
  char CConfig::g_TmpParamPath[MAX_PATH];
  char CConfig::g_FutParamPath[MAX_PATH];		
  char CConfig::g_ImportParamPath[MAX_PATH];
  char CConfig::g_SleSoftfilePath[MAX_PATH]; 	
  	
	
	//��ʷ����Ŀ¼
	char 	CConfig::HisDataPath_ACC[MAX_PATH];
	char 	CConfig::HisDataPath_LCC[MAX_PATH];
 
 
	//�������Ŀ¼	
  char 	CConfig::InsertStatusFilePath[MAX_PATH];
  char 	CConfig::InsertTransFilePath[MAX_PATH];
	char 	CConfig::ErrorPath[MAX_PATH];	
   
	//ACC����Ŀ¼			
	char 	CConfig::TempSendFilePath[MAX_PATH];   
	char 	CConfig::RecvPath[MAX_PATH];
	char 	CConfig::SendFilePath_ACC[MAX_PATH]; 	
			
 	/* ----- FTPĿ¼ ---*/
	char 	CConfig::g_Ftp_Para_Run[MAX_PATH];
	char 	CConfig::g_Ftp_Para_BlackList[MAX_PATH];
	char 	CConfig::g_Ftp_Para_Mode[MAX_PATH];
	char 	CConfig::g_Ftp_Para_Other[MAX_PATH];
	char 	CConfig::g_Ftp_Settle[MAX_PATH];
	char 	CConfig::g_Ftp_Stock[MAX_PATH];
	char 	CConfig::g_Ftp_Data_Trans[MAX_PATH];
	char 	CConfig::g_Ftp_Data_Status[MAX_PATH];
	char 	CConfig::g_Ftp_Data_Other[MAX_PATH];


	char 	CConfig::StockPath[MAX_PATH]; 
	
	
 	char 	CConfig::g_ACCFtpUser[MAX_PATH];
	char 	CConfig::g_FTP_ACCIP[MAX_PATH];
	char 	CConfig::g_ACCFtpPass[MAX_PATH];


	//��վ�б�
  ScList  CConfig::g_ScList[MAX_SLE_COUNT];

	//��·�б�	 
	MlcLineList  CConfig::ms_MlcLineList[MAX_SLE_COUNT]; 
	   
	//�ۺϼ�س�վ�б�
 	IscsStation   CConfig::ms_IscsStation[MAX_SLE_COUNT]; 
	   
	//�����˿�����
  int  CConfig::g_Lsn_AccPort;
  int  CConfig::g_Lsn_ScPort;
  int  CConfig::g_Lsn_ISCSPort; 
	 
	//ʱ������ 
  int	 CConfig::ParamValidTime;	
  int  CConfig::QueryScStatusTime;
  int  CConfig::QueryScParamTime;
	int  CConfig::ClockTime;
  int  CConfig::g_Wait_Ack_Time;	
	
	//��ֵ���� 
 
  int		CConfig::g_MaxRecord;
  int		CConfig::g_MaxDelay;     
 
  
  //��־λ����
  int 	CConfig::g_Auto_Upload_Flag;
  int	  CConfig::g_Conn_L; 
  int	  CConfig::g_RunMode_Flag;
  	  
  //ȫ�ֱ���
  int	CConfig::g_ExecBatch_Flag;
  char CConfig::g_LocalIP[16];
  BYTE CConfig::g_b_LocalIP[4];
  	  
  bool    			CConfig::ms_AccIsConnect;	//ACC�Ƿ��Ѿ�����
  bool    			CConfig::ms_DiskIsFull;		//���������̿ռ��Ƿ�����
  bool    			CConfig::ms_SynTimeError;	//ͬ��ʱ���Ƿ�ʧ��
  bool    			CConfig::ms_SynTimeOverDiff;	//ͬ��ʱ����Ƿ񳬹�ָ��ʱ��	
 
void CConfig::InitConfigStaticVar(const char* iniAppPath) 
{
    CIniFile ini;
    int      rtn;
  	char     c_temp[20],*stopstring;

		/* --------- �������ļ� ----------------*/
    char inifilename[MAX_PATH];
    memset(inifilename, 0x00, sizeof(inifilename));
    sprintf(inifilename, "%spara.ini", iniAppPath);
    ini.OpenIniFile(inifilename);
 
		/* --------- ��ȡ�����ļ� ---------------- */
		
		/*----���ݿ������ֵ-------------*/
		memset(m_DBUserName, 0x00, sizeof(m_DBUserName));
    memset(m_DBUserPasswd, 0x00, sizeof(m_DBUserPasswd));
    memset(m_DBHostName, 0x00, sizeof(m_DBHostName));
    
    strcpy(m_DBUserName, ini.ReadString("DATABASE", "DB_USERNAME", (char *) "scdb"));
    strcpy(m_DBUserPasswd, ini.ReadString("DATABASE", "DB_PASSWORD", (char *) "scdb"));
    strcpy(m_DBHostName, ini.ReadString("DATABASE", "DB_HOSTNAME", (char *) "scdb"));
        
    /*--------��־·��-------------*/
    memset(m_AppLogPath,0x00,sizeof(m_AppLogPath));
		sprintf(m_AppLogPath,"%slog/",iniAppPath);

		/* --------- NTP���� ----------------*/
    m_NtpFlag = ini.ReadInt("NTP", "NTP_FLAG", 0);
    m_NtpPort = ini.ReadInt("NTP", "NTP_PORT", 123);
    m_NtpGap = ini.ReadInt("NTP", "NTP_GAP", 300);
    m_NtpTimedout = ini.ReadInt("NTP", "NTP_TIMEDOUT", 3);
    memset(m_NtpIP, 0x00, sizeof(m_NtpIP));
    strcpy(m_NtpIP, ini.ReadString("NTP", "NTP_IP", (char *) "172.28.100.47"));
    
		/* --------- ACC������� ----------------*/    
    memset(g_ACC_ID, 0x00, sizeof(g_ACC_ID));
    memset(g_ACCIDCode, 0x00, sizeof(g_ACCIDCode));
 		strcpy(g_ACC_ID, ini.ReadString("NID", "ACC_NID", (char *) "03000000"));
 		CommonFuncs::chartobyte(g_ACC_ID, (char *) g_ACCIDCode);
		
		memset(g_Socket_ACCIP,0x00,sizeof(g_Socket_ACCIP)); 			
		strcpy(g_Socket_ACCIP, ini.ReadString("COMMNICATION", "ACC_SOCKET_IP", (char *)"172.24.100.5"));			    
		g_ACCPort = ini.ReadInt("COMMNICATION", "ACC_PORT", 8000); 			    

		/* --------- CC�ڵ��ʶ ----------------*/   	 
		memset(g_CCID,0x00,sizeof(g_CCID));
		memset(g_CCIDCode,0x00,sizeof(g_CCIDCode));	 
    strcpy(g_CCID, ini.ReadString("NID", "LCC_NID", (char *) "03000000"));
    memset(g_CCIDCode, 0x00, sizeof(g_CCIDCode));
    CommonFuncs::chartobyte(g_CCID, (char *) g_CCIDCode);
    	
		/* --------- ��������Ŀ¼----------------*/   	    	
		memset(g_CurParamPath,0x00,sizeof(g_CurParamPath));
		sprintf(g_CurParamPath,"%sparafile/curparafile/",iniAppPath);
		memset(g_FutParamPath,0x00,sizeof(g_FutParamPath));
		sprintf(g_FutParamPath,"%sparafile/futparafile/",iniAppPath);
		memset(g_TmpParamPath,0x00,sizeof(g_TmpParamPath));
		sprintf(g_TmpParamPath,"%sparafile/tempparafile/",iniAppPath);   
		memset(g_ImportParamPath,0x00,sizeof(g_ImportParamPath));
		sprintf(g_ImportParamPath,"%sparafile/importparafile/",iniAppPath);		
		memset(g_SleSoftfilePath,0x00,sizeof(g_SleSoftfilePath));
		sprintf(g_SleSoftfilePath,"%ssoftfile/slesoftfile/",iniAppPath);			
		 	
    	
		/* --------- ��ʷ����Ŀ¼----------------*/    
		memset(HisDataPath_LCC,0x00,sizeof(HisDataPath_LCC));
		sprintf(HisDataPath_LCC,"%sdatafile/end/LCC/",iniAppPath);
		memset(HisDataPath_ACC,0x00,sizeof(HisDataPath_ACC));
		sprintf(HisDataPath_ACC,"%sdatafile/end/ACC/",iniAppPath);
 
		/* --------- �������Ŀ¼----------------*/       		
		memset(InsertStatusFilePath,0x00,sizeof(InsertStatusFilePath));
		sprintf(InsertStatusFilePath,"%sdatafile/prepareinsert/statusfile/",iniAppPath);
		memset(InsertTransFilePath,0x00,sizeof(InsertTransFilePath));
		sprintf(InsertTransFilePath,"%sdatafile/prepareinsert/transfile/",iniAppPath);    		
		memset(ErrorPath,0x00,sizeof(ErrorPath));
		sprintf(ErrorPath,"%sdatafile/error/",iniAppPath);    	
    	
		/* --------- ACC����Ŀ¼----------------*/   ; 			   		
	 	memset(SendFilePath_ACC,0x00,sizeof(SendFilePath_ACC));
		sprintf(SendFilePath_ACC,"%sdatafile/preparesendtoacc/",iniAppPath);
		memset(RecvPath,0x00,sizeof(RecvPath));
		sprintf(RecvPath,"%sdatafile/preparerecvacc/",iniAppPath);
		memset(TempSendFilePath,0x00,sizeof(TempSendFilePath));
		sprintf(TempSendFilePath,"%sdatafile/temp/",iniAppPath);
 
		/* --------- �����˿�����	----------------*/     	 			 
		g_Lsn_AccPort 	= ini.ReadInt("COMMNICATION", "LISTEN_ACC_PORT", 7000);
		g_Lsn_ScPort 		= ini.ReadInt("COMMNICATION", "LISTEN_SC_PORT", 6001);
		g_Lsn_ISCSPort	= ini.ReadInt("COMMNICATION", "LISTEN_ISCS_PORT", 9000);   	 			 
   	 			 
		/* --------- ʱ������	----------------*/      
		ParamValidTime 		= ini.ReadInt("TIMER", "PARAM_VALIDATE_TIME", 60);
		QueryScStatusTime = ini.ReadInt("TIMER", "SC_CONNECT_QUERY_TIME", 5);
		QueryScParamTime	= ini.ReadInt("TIMER", "SC_PARAM_QUERY_TIME", 300);
		ClockTime 				= ini.ReadInt("TIMER", "CLOCK_TIME", 900);
		g_Wait_Ack_Time 	= ini.ReadInt("TIMER", "WAIT_ACK_TIME", 5);   
		
		/* --------- ��ֵ����	----------------*/  	
		g_MaxRecord = ini.ReadInt("PACKAGE", "MAX_TRANS_RECORD", 100);
		g_MaxDelay  = ini.ReadInt("PACKAGE", "MAX_TRANS_DELAY", 5);
 
		 
		/* --------- ��־λ����	----------------*/    
 		g_RunMode_Flag = ini.ReadInt("OTHTER", "RUNMODE_TRANSFER_ACC_FLAG", 0);
 		g_Auto_Upload_Flag = ini.ReadInt("OTHTER", "AUTO_UPLOAD_PARAM_FLAG", 0);
		g_Conn_L      		= ini.ReadInt("OTHTER", "SC_CONNECT_L_FLAG", 0); 	 


		/* --------- �ر������ļ� ----------------*/
    ini.CloseIniFile();
    
		/* --------- ȫ�ֱ��� ----------------*/
		g_ExecBatch_Flag = 0;
		memset(g_LocalIP,0x00,sizeof(g_LocalIP));
		CommonFuncs::GetLocalHostInfo(g_LocalIP);
		memset(g_b_LocalIP,0x00,sizeof(g_b_LocalIP));
		CommonFuncs::GetHexIP(g_LocalIP,g_b_LocalIP);
			
			
		ms_AccIsConnect = false  ;
		ms_DiskIsFull   = false  ;
		ms_SynTimeError = true  ; 
		ms_SynTimeOverDiff = false; 
}

 
char CDir::m_AppPath[MAX_PATH];
/* ------------------------------------------------------
            ���Ӧ�ó���ǰ·��
--------------------------------------------------------- */
int CDir::GetAppPath()
{
    char pathname[MAX_PATH];
    if(!getcwd(pathname, MAX_PATH))
    {
        return 0;
    }
    if(pathname[strlen(pathname) - 1] != '/')
    {
        strcat(pathname,"/");
    }

    memset(m_AppPath,0x00,sizeof(m_AppPath));
    strcpy(m_AppPath,pathname);

    return 1;
}

/* ----------------------------------------------------- */
/*                       �ж�Ŀ¼�Ƿ����                */
/* ----------------------------------------------------- */
int CDir::AppDirIsExists(const char *dir)
{
    int             iRet;
    DIR             *dp;

    iRet = 0;
    dp = opendir(dir);
    if (dp != NULL)
    {
        closedir(dp);
        iRet = 1;
    }
    return iRet;
}

/* ----------------------------------------------------- */
/*                       ����Ŀ¼                        */
/* ----------------------------------------------------- */
int CDir::CreateAppDir(const char *dir)
{
    int             bRet;
    int              rtn;

    rtn = 1;
    if(AppDirIsExists(dir) == 0)
    {
        bRet = mkdir(dir,0777);
        if(bRet != 0)
        {
            rtn = 0;
        }
    }

    return rtn;
}

char CDir::m_AppLogPath[MAX_PATH];
/* ----------------------------------------------------- */
/*                       ������־Ŀ¼                    */
/* ----------------------------------------------------- */
int CDir::CreateLogDir()
{
    int  i,rtn;
    char dirname[MAX_PATH];

    /* --- ������־��Ŀ¼ --- */
    memset(dirname,0x00,sizeof(dirname));
    sprintf(dirname,"%slog/",m_AppPath);
    rtn = CreateAppDir(dirname);
    if(rtn == 0)
        return 0;
        
   	memset(m_AppLogPath,0x00,sizeof(m_AppLogPath));
    strcpy(m_AppLogPath,dirname);
 
    /* --- ������־��Ŀ¼ --- */
    for(i = 0;i < 31;i++)
    {
        memset(dirname,0x00,sizeof(dirname));
        sprintf(dirname,"%slog/%02d/",m_AppPath,i+1);
        rtn = CreateAppDir(dirname);
        if(rtn == 0)
            return 0;
    }

    return 1;
}

/* ----------------------------------------------------- */
/*                       ��������Ŀ¼                    */
/* ----------------------------------------------------- */
int CDir::CreateDataDir( )
{
	int  i,rtn;
	char dirname[MAX_PATH];

	/* --- ����������Ŀ¼ --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sdatafile/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ��������Ŀ¼(debug) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sdatafile/debug/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ��������Ŀ¼(preparesendtoacc) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sdatafile/preparesendtoacc/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ��������Ŀ¼(temp) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sdatafile/temp/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ��������Ŀ¼(preparerecvacc) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sdatafile/preparerecvacc/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ��������Ŀ¼(error) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sdatafile/error/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ��������Ŀ¼(prepareinsert) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sdatafile/prepareinsert/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ��������Ŀ¼(prepareinsert/transfile) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sdatafile/prepareinsert/transfile/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ��������Ŀ¼(prepareinsert/statusfile) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sdatafile/prepareinsert/statusfile/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ��������Ŀ¼(end) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sdatafile/end/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ��������Ŀ¼(end/LCC) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sdatafile/end/LCC/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ��������Ŀ¼(end/ACC) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sdatafile/end/ACC",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ����������Ŀ¼(end/LCC/xx/) --- */
	for(i = 0;i < 31;i++)
	{
		memset(dirname,0x00,sizeof(dirname));
		sprintf(dirname,"%sdatafile/end/LCC/%02d/",m_AppPath,i+1);
		rtn = CreateAppDir(dirname);
		if(rtn == 0)
			return 0;
	}
	/* --- ����������Ŀ¼(end/ACC/xx/) --- */
	for(i = 0;i < 31;i++)
	{
		memset(dirname,0x00,sizeof(dirname));
		sprintf(dirname,"%sdatafile/end/ACC/%02d/",m_AppPath,i+1);
		rtn = CreateAppDir(dirname);
		if(rtn == 0)
			return 0;
	}
 
	return 1;
}

/* ----------------------------------------------------- */
/*                       ��������Ŀ¼                    */
/* ----------------------------------------------------- */
int CDir::CreateParamDir()
{
	int  rtn;
	char dirname[MAX_PATH];

	/* --- ����������Ŀ¼ --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sparafile/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ��������Ŀ¼(curparafile) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sparafile/curparafile/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ��������Ŀ¼(tempparafile) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sparafile/tempparafile/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
	/* --- ������������Ŀ¼(importparafile) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sparafile/importparafile/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;			
	/* --- ��������Ŀ¼(stockfile) --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sparafile/stockfile/",m_AppPath);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;
		
	return 1;
}

/* ----------------------------------------------------- */
/*                       ������·����Ŀ¼                */
/* ----------------------------------------------------- */
int CDir::CreateLineParamDir(int lineId)
{
	int  i,rtn;
	char dirname[MAX_PATH];
	
	/* --- ������·������Ŀ¼ --- */
	memset(dirname,0x00,sizeof(dirname));
	sprintf(dirname,"%sparafile/curparafile/lc%d",m_AppPath,lineId);
	rtn = CreateAppDir(dirname);
	if(rtn == 0)
		return 0;	
		
	return 1;
}



/* ----------------------------------------------------- */
/*                       �������Ŀ¼                    */
/* ----------------------------------------------------- */
int CDir::CreateSoftDir()
{
    int  rtn;
    char dirname[MAX_PATH];

    /* --- ����ͼƬ��Ŀ¼ --- */
    memset(dirname,0x00,sizeof(dirname));
    sprintf(dirname,"%simage/",m_AppPath);
    rtn = CreateAppDir(dirname);
    if(rtn == 0)
        return 0;

    /* --- ���������Ŀ¼ --- */
    memset(dirname,0x00,sizeof(dirname));
    sprintf(dirname,"%ssoftfile/",m_AppPath);
    rtn = CreateAppDir(dirname);
    if(rtn == 0)
        return 0;
    /* --- �������Ŀ¼(stationsoftfile) --- */
    memset(dirname,0x00,sizeof(dirname));
    sprintf(dirname,"%ssoftfile/stationsoftfile/",m_AppPath);
    rtn = CreateAppDir(dirname);
    if(rtn == 0)
        return 0;
    /* --- �������Ŀ¼(slesoftfile) --- */
    memset(dirname,0x00,sizeof(dirname));
    sprintf(dirname,"%ssoftfile/slesoftfile/",m_AppPath);
    rtn = CreateAppDir(dirname);
    if(rtn == 0)
        return 0;
    /* --- �������Ŀ¼(socsoftfile) --- */
    memset(dirname,0x00,sizeof(dirname));
    sprintf(dirname,"%ssoftfile/socsoftfile/",m_AppPath);
    rtn = CreateAppDir(dirname);
    if(rtn == 0)
        return 0;
    /* --- �������Ŀ¼(update) --- */
    memset(dirname,0x00,sizeof(dirname));
    sprintf(dirname,"%ssoftfile/update/",m_AppPath);
    rtn = CreateAppDir(dirname);
    if(rtn == 0)
        return 0;

    return 1;
}

/* ----------------------------------------------------- */
/*                       ������������Ŀ¼                   */
/* ----------------------------------------------------- */
int CDir::CreateAppAllDir()
{
    int       ret;

    /*---------APP����·����ֵ(m_AppPath)-------------------*/
    ret = GetAppPath();
    if(ret == 0)
        return 0;


    /* --------����Ŀ¼ ---------- */
    ret = CreateLogDir();
    if(ret == 0)
        return 0;

	 
    ret = CreateDataDir();
    if(ret == 0)
        return 0;

    ret = CreateParamDir();
    if(ret == 0)
        return 0;

    ret = CreateSoftDir();
    if(ret == 0)
        return 0;
     
    return 1; 
} 
 
 