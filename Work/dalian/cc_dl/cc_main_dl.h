#ifndef CC_MAIN_DL_H
#define CC_MAIN_DL_H

#include "sys_head.h"
#include "base_define.h"
#include "cc_struct_dl.h"


#ifndef MAX_PATH
#define MAX_PATH     	        256	    /* ------- 文件名的最大长度 ---- */
#endif

#ifndef MAX_FTP_COUNT 
#define MAX_FTP_COUNT     	  200    /* ------- FTP目录最大个数 ---- */
#endif


//所有有关ACC处理的函数
class ACC
{
	private:
		static bool    			ms_AccIsFirstConnect;	//ACC是否第一次连接
		static int    			g_Acc_SockHandle;	//ACC长连接句柄
		
		static sem_t  			ms_CsRcv;
		static MsgHead			ms_MsgHeadRcv;
	
 

	public:		
		static  void  InitAccStaticVar();
		static  void  ReleaseAccStaticVar();
	
  public:	
		static  int getlineinfo(char *lccode);
		static  int getlineinfo(int lineId);
		static  int GetFtpMsg(unsigned short msgtype,int lineIndex,FtpMsg *m_FtpMsg);
		static  int 	DealAccStatFile(char *path,unsigned short wMsgCode,int lineIndex,char *b_SettleDate);
		static  int 	DealAccFile3002(char *path,unsigned short wMsgCode,int lineIndex,char *name);
 	
		static  void* ThreadSendData_ACC(void* lpParam);
		static  int   PrepareSendData_ACC();
		static  int   AppartFile(char *filname,char *name,char *filebuf,unsigned short msgtype,int filelen,int lineid);
		static  int   CreateSendFile();
		static  void* ThreadCreateSendFile(void* lpParam);
		static  void  Send_Acc_6101(WORD msgtype,char *pkgbody,int len,char *scid);
		static  void  Send_Acc_6102(WORD msgtype,int lineIndex);
		static  int   OnACCMessageIn(para *tmp_para,int tmp_handle,char *clnip,int clnport);


		static  void  del_Acc_0015(int flag,char *filebuf,int count,char *name,char *fileExistPath);

		static  int   Get_Acc_Ctl_ParamFile(int lineIndex,char *name);
		static  int   GetAccParamFile(unsigned short msgtype,int lineIndex,char *filename);
		static  void* ThreadDealACCCmd(void* inpara);


};



//所有有关SC处理的函数
class SC
{
	private:
		static unsigned int  ms_chatseq;
  public:	 	
  	static 	ParamVer 			tempParamVerList[MAX_PARAM_COUNT];	
  		 		
  public:	
		static void   OccurStatusFile(BYTE *deviceNid,BYTE eventLevel);
		static int 		CreateTransFile(char *dir,int flag,unsigned char* recv_buf,int buflen);
		 	
		static int 	 format_4001(BYTE *body,int bodylen); 	
		static int   OnSCMessageIn(para *tmp_para,int tmp_handle,char *clnip,int clnport);
		static void* ThreadDealSCCmd(void* inpara);
		static int   getscinfo(char *sccodeOrScip); 

		static void  Upload_SC_StockFile(char *filename,char *sendbuf,int sendlen);	
		static void* senddatatosle(void* inpara);
		static int   getdatasle(WORD msgtype,WORD paramtype,char *scid,unsigned char * recvbuf,int buflen);

		static int   GetParamVerList(int scIndex);
		static int   CreateParamQuery(int scIndex,int paramCount,char *sendbuf);
		static int   CreateParamSync(WORD msgtype,unsigned char *sccode,unsigned char *ParamVer,char *sendbuf);	
		static void  SyncScParam(int scIndex);
		
		static int  SyncScTime(int scIndex);

		static void* ThreadQueryScParam(void* inpara);		
		static void* ThreadQueryScStatus(void* inpara);

		static void  UpdateScStatus(int index,int flag);
		static int   DealAccCtlParamFile(int lineIndex,char *name);
		static void  DealImportParam(WORD wMsgCode,int lineIndex);
};


 



//所有有关参数处理的函数
class CPara
{
	public:
	 	static 	sem_t  				g_CSFTP;
		static  int  					g_Ftp_Inform;
	 	
	public:
	 	static 	void InitCParaStaticVar();
	 	static 	void ReleaseCParaStaticVar();


		static int   DealParam(unsigned short wMsgCode,char *fileName,int lineIndex,char *fileExistPath);
		static int   GetParamVer(const char *scid,WORD type,char *dir,char *ver);
		static  void  Rename_ParamFile(int flag,char *oldpath,char *name,char *newpath);		
		
					 	
		static void* 	ThreadValidateParam(void* lpParam); 
		static 	void  Copy_ParamFile(char *oldpath,char *oldname,const char *newname,const char *newpath);

		static int  	Formate_ParamFile(char *path,char *name);	

		static void  CreateParamFile();
};




 //有关线程的类
class CThread
{
	private:

	public:
		
		static void  DelErrorData();
		static void  DelData(char *delpath,int curmonth,int curday);
		static void  DelHistoryData();
		static void* ThreadDelHistoryData(void* lpParam);
 
		static int   PrepareInsertTransData(char *filepath,WORD in_msgtype);
		static int   InsertTransData(char *filename,char *name,WORD msgtype);
		static void* ThreadInsertTransData_1001(void* lpParam);
		static void* ThreadInsertTransData_1002(void* lpParam);
		static void* ThreadInsertTransData_1003(void* lpParam);
		static void* ThreadInsertTransData_1004(void* lpParam);
		static void* ThreadInsertTransData_1005(void* lpParam);
		static void* ThreadInsertTransData_1006(void* lpParam);
												
		static int   PrepareInsertStatusData(char *filepath,WORD in_msgtype);
		static int   InsertStatusData(char *filename,char *name);
		static void* ThreadInsertStatusData_TvmAr(void* lpParam);
		static void* ThreadInsertStatusData_BomAr(void* lpParam);		
		static void* ThreadInsertStatusData_AgmAr(void* lpParam);
		static void* ThreadInsertStatusData_Status(void* lpParam);		
		static void* ThreadInsertStatusData_Other(void* lpParam);		
		
		
		static void* ThreadQueryCCPort1(void* lpParam);
	

}; 
 
//负责时钟同步
class CSynTime
{
	private:
		static int  construct_packet(char *packet);
		static int  get_ntp_time(int sockfd, struct sockaddr_in *server_addr, struct tm *net_tm);
		static int  set_local_time(char *c_time);	

	public:
		static void Adjust_System_Time();
		static int  UpdateSystemTime (time_t ulTime,char *c_time);	
		static void* ThreadAdjustSystemTime(void* inpara); //NTP校对时钟函数	
};
 
 
 
 //所有有关main函数但又无法归类的类
class CMainManage
{
	public:
		static int 		ms_CommExitFlag;
	public:		 
		static int   	MainStart(); //负责所有类的初始化
		static void  	MainStop();	//负责调用所有类的结束		
 
		static void  	CatchSignal(int the_sig);
		static void  	ReleaseMem();
		static void  	RestartProgram(char *path);	
		static int 		Wait_MsgReadSem();

}; 

 
 
#endif

